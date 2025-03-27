#include <mpi.h>
#include <iostream>
#include <functional>
#include "Load_Balance.hpp"
#include "filters.hpp"
#include "mpi_kernel.hpp"


inline int warmupFunction() {
  int i = 0;
  for (i = 0; i < 1000; i++) {
    i = i + 1;
  }
  return i;
}


int main(int argc, char *argv[]) {
    volatile int x = warmupFunction();
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::function<void(std::vector<int> &, int)> filter;
    int filter_number = 0;
    int problem_size = 0;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find("--filter_type=") == 0) {
            std::string filter_type = arg.substr(14);
            if (filter_type == "compute_bound") {
                filter = load_balance::calculateSumVec;
            } else if (filter_type == "memory_bound") {
                filter = load_balance::memoryBoundFilter;
            } else {
                if (rank == 0) {
                    std::cerr << "Invalid filter type. Valid values are compute_bound, memory_bound." << std::endl;
                }
                MPI_Finalize();
                return 1;
            }
        } else if (arg.find("--filter_number=") == 0) {
            filter_number = std::stoi(arg.substr(16));
            if (filter_number <= 0) {
                if (rank == 0) {
                    std::cerr << "Invalid filter number. It should be greater than 0." << std::endl;
                }
                MPI_Finalize();
                return 1;
            }
        } else if (arg.find("--problem_size=") == 0) {
            problem_size = std::stoi(arg.substr(15));
            if (problem_size <= 0) {
                if (rank == 0) {
                    std::cerr << "Invalid problem size. It should be greater than 0." << std::endl;
                }
                MPI_Finalize();
                return 1;
            }
        }
    }

    std::vector<std::tuple<int, int>> filter_order = {
        std::make_tuple(0, filter_number)
    };

    if (rank == 0) {
        std::cout << "Filter Number: " << filter_number << std::endl;
        std::cout << "Problem Size: " << problem_size << std::endl;
    }

    load_balance::MPI_kernel(size, rank, filter, filter_number, problem_size, filter_order);

    MPI_Finalize();
    return 0;
}

// mpirun -np 5 ./hello_mpi
// mpirun -np 4 xterm -e gdb -ex run --args ./MyMPIProject
