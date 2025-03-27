#include "Load_Balance.hpp"
#include "debug.hpp"
#include "filters.hpp"
#include "mpi_kernel.hpp"
#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>
#include <mpi.h>



const auto COMPUTE_BOUND = load_balance::calculateSumVec;
const auto MEMORY_BOUND = load_balance::memoryBoundFilter;
const int PROBLEM_SIZE = 10000;
const int FILTER_NUMBER = 50;

std::vector<std::tuple<int, int>> FILTER_ORDER = {
    std::make_tuple(0, FILTER_NUMBER)
    };
    // this way copy isn't too costly (only 2 copies)



void test_modulate_filters(int size, int rank, int filter_number) {
    if (rank == 0)
        std::cout << "Testing modulate filters. The number of filters is " << filter_number << std::endl;
    load_balance::MPI_kernel(size, rank, COMPUTE_BOUND, filter_number, PROBLEM_SIZE, FILTER_ORDER);
}

void test_modulate_problem_size(int size, int rank, int problem_size) {
    if (rank == 0)
        std::cout << "Testing modulate problem size. The problem size is " << problem_size << std::endl;
    load_balance::MPI_kernel(size, rank, COMPUTE_BOUND, FILTER_NUMBER, problem_size, FILTER_ORDER);
    
}

void test_modulate_number_of_procs(int size, int rank) {
    if (rank == 0)
        std::cout << "Testing number of procs.  " << std::endl;
    load_balance::MPI_kernel(size, rank, COMPUTE_BOUND, FILTER_NUMBER, PROBLEM_SIZE, FILTER_ORDER);

}

void test_modulate_function_type(int size, int rank) {
    if (rank == 0)
        std::cout << "Testing modulate function type. The function type is " << std::endl;
    load_balance::MPI_kernel(size, rank, COMPUTE_BOUND, FILTER_NUMBER, PROBLEM_SIZE, FILTER_ORDER);
}