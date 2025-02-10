#include "mpi_kernel.hpp"
#include "debug.hpp"

namespace load_balance {
std::vector<int> MPI_kernel(const int number_of_procs, const int rank, const std::function<std::vector<int>(const std::vector<int> &, int)>
      filterFunction, const int number_of_filters, const int problem_size, const std::vector<std::tuple<int, int>> filter_order) {
  const int CHUNKS = number_of_procs - 1;
  if (number_of_procs < CHUNKS + 1) {
    std::cerr << "This program requires at least " << CHUNKS + 1
              << " processes.\n ";
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  if (rank == 0) {
    std::vector<int> parent_vec(problem_size);
    for (int i = 0; i < problem_size; i++) {
      parent_vec[i] = i;
    }
    if (problem_size < number_of_procs) {
      std::cerr << "This program requires at least " << problem_size
                << " processes.\n ";
      MPI_Abort(MPI_COMM_WORLD, 1);
    }
    send_out_work(CHUNKS, parent_vec, EQUAL_CHUNKING);
  }

  std::vector<std::function<std::vector<int>(const std::vector<int> &, int)>>
      filter_list;
  for (int i = 0; i < number_of_filters; i++) {
    filter_list.push_back(filterFunction);
  }

  if (filter_list.empty()) {
    std::cerr << "Filter list is empty\n";
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  int filter_count = 0; // This represents the current filter up (hasn't been run yet)
  while (true) {
    if (filter_count == filter_order.size() - 1) {
      break;
    }
    if (rank != 0) {
      int start = std::get<0>(filter_order[filter_count]);
      int end = std::get<1>(filter_order[filter_count]);
      auto filterSlice = std::vector<
          std::function<std::vector<int>(const std::vector<int> &, int)>>(
          filter_list.begin() + start, filter_list.begin() + end);
      children_code(filterSlice, rank);
    } else {
      auto vec = combineAndSendOutFn(CHUNKS);
#if DEBUG == 1
      load_balance::debug_vector(vec);
#endif
    }
    filter_count++;
#if DEBUG == 1
    printf("filter count %d\n", filter_count);
#endif
  }
  std::vector<int> final;
  if (rank != 0) {
    int start = std::get<0>(filter_order[filter_count]);
    int end = std::get<1>(filter_order[filter_count]);
    auto filterSlice = std::vector<
        std::function<std::vector<int>(const std::vector<int> &, int)>>(
        filter_list.begin() + start, filter_list.begin() + end);
    assert(filterSlice.size() > 0);
    children_code(filterSlice, rank);
  } else {
    auto final = combine_work(CHUNKS);
    print_vector(final);
#if SUPER_DEBUG == 1
    load_balance::debug_vector(final);
#endif
  }
}
}
