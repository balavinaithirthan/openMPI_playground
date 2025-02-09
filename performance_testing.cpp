#include "Load_Balance.hpp"
#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>
#include <mpi.h>
const auto COMPUTE_BOUND = load_balance::calculateSumVec;
enum {
    MEMORY_BOUND = 1,
    NEEDLEMAN_WUNSCH = 2,
};
const int FILTER_NUMBER = 20;
const int PROBLEM_SIZE = 1000;
const int NUMBER_OF_PROCS = 4;
std::vector<std::tuple<int, int>> FILTER_ORDER = {
    std::make_tuple(0, 5),
    std::make_tuple(5, 10),
    std::make_tuple(10, 15),
    std::make_tuple(15, 20),
};

void time_start() {
#if TIMING == 1
  auto start = std::chrono::high_resolution_clock::now();
#endif
}

void time_end() {
#if TIMING == 1
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;
  if (rank == 0) {
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
  }
#endif
}


void test_modulate_filters(int size, int rank) {
    time_start();
    load_balance::MPI_kernel(size, rank, FILTER_NUMBER, 
      COMPUTE_BOUND, FILTER_NUMBER, PROBLEM_SIZE, FILTER_ORDER);
    time_end();
}

void test_modulate_problem_size() {
    
}

void test_modulate_number_of_procs() {
    
}

void test_modulate_function_type() {
    
}