#include "Load_Balance.hpp"
#include "debug.hpp"
#include <chrono>
#include <cstdio>
#include <functional>
#include <iostream>
#include <mpi.h>
#include <vector>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

#if TIMING == 1
  auto start = std::chrono::high_resolution_clock::now();
#endif

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  const int CHUNKS = size - 1;
  if (size < CHUNKS + 1) {
    std::cerr << "This program requires at least " << CHUNKS + 1
              << " processes.\n ";
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  if (rank == 0) {
    const int PARENT_VEC_SIZE = 20;
    std::vector<int> parent_vec(PARENT_VEC_SIZE);
    for (int i = 0; i < PARENT_VEC_SIZE; i++) {
      parent_vec[i] = i;
    }
    if (PARENT_VEC_SIZE < size) {
      std::cerr << "This program requires at least " << PARENT_VEC_SIZE
                << " processes.\n ";
      MPI_Abort(MPI_COMM_WORLD, 1);
    }
    send_out_work(CHUNKS, parent_vec, load_balance::EQUAL_CHUNKING);
  }

  std::vector<std::function<std::vector<int>(const std::vector<int> &, int)>>
      filter_list;
  filter_list.emplace_back(load_balance::make_zeros);
  filter_list.emplace_back(load_balance::calculateSumVec);
  filter_list.emplace_back(load_balance::calculateSumVec);
  filter_list.emplace_back(load_balance::calculateSumVec);
  filter_list.emplace_back(load_balance::calculateSumVec);
  std::vector<std::tuple<int, int>> filterOrder = {
      {0, 2}, {2, 4}, {4, 5}}; // filter start up to but not including end, this is the actual Filter Number

  if (filter_list.empty()) {
    std::cerr << "Filter list is empty\n";
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  int filter_count = 0; // This represents the current filter up (hasn't been)
  while (true) {
    if (filter_count == filterOrder.size() - 1) {
      break;
    }
    if (rank != 0) {
      int start = std::get<0>(filterOrder[filter_count]);
      int end = std::get<1>(filterOrder[filter_count]);
      auto filterSlice = std::vector<
          std::function<std::vector<int>(const std::vector<int> &, int)>>(
          filter_list.begin() + start, filter_list.begin() + end);
      load_balance::children_code(filterSlice, rank);
    } else {
      auto vec = load_balance::combineAndSendOutFn(CHUNKS);
#if DEBUG == 1
      load_balance::debug_vector(vec);
#endif
    }
    filter_count++;
#if DEBUG == 1
    printf("filter count %d\n", filter_count);
#endif
  }

  if (rank != 0) {
    int start = std::get<0>(filterOrder[filter_count]);
    int end = std::get<1>(filterOrder[filter_count]);
    auto filterSlice = std::vector<
        std::function<std::vector<int>(const std::vector<int> &, int)>>(
        filter_list.begin() + start, filter_list.begin() + end);
    assert(filterSlice.size() > 0);
    load_balance::children_code(filterSlice, rank);
  } else {
    auto final = load_balance::combine_work(CHUNKS);
    load_balance::print_vector(final);
#if SUPER_DEBUG == 1
    load_balance::debug_vector(final);
#endif
  }

#if TIMING == 1
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;
  if (rank == 0) {
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
  }
#endif

  MPI_Finalize();
  return 0;
}

// mpirun -np 5 ./hello_mpi
// mpirun -np 4 xterm -e gdb -ex run --args ./MyMPIProject
