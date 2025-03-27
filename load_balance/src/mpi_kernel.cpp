#include "mpi_kernel.hpp"
#include "debug.hpp"
namespace load_balance {
void MPI_kernel(
    const int number_of_procs, const int rank, const int problem_size,
    std::vector<std::function<void(std::vector<hits_lib::Hit> &, int)>>
        filter_list,
    const std::vector<std::tuple<int, int>> filter_order,
    std::vector<hits_lib::Hit> &parent_vec) {

#if DEBUG_INPUT
  print_filter_list(filter_list);
  print_filter_order(filter_order);
#endif

  const int CHUNKS = number_of_procs - 1;
  if (number_of_procs < CHUNKS + 1) {
    std::cerr << "This program requires at least " << CHUNKS + 1
              << " processes.\n ";
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  auto start = time_start();

  if (rank == 0) {
    if (problem_size < number_of_procs) {
      std::cerr << "This program requires at least " << problem_size
                << " processes.\n ";
      MPI_Abort(MPI_COMM_WORLD, 1);
    }
    start = time_start();
    send_out_work(CHUNKS, parent_vec, EQUAL_CHUNKING);
  }
  if (filter_list.empty()) {
    std::cerr << "Filter list is empty\n";
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  int filter_count =
      0; // This represents the current filter up (hasn't been run yet)
  while (true) {
    if (filter_count == filter_order.size() - 1) {
      break;
    }
    if (rank != 0) {
      int start = std::get<0>(filter_order[filter_count]);
      int end = std::get<1>(filter_order[filter_count]);
      auto filterSlice =
          std::vector<std::function<void(std::vector<hits_lib::Hit> &, int)>>(
              filter_list.begin() + start, filter_list.begin() + end);
      assert(filterSlice.size() > 0);
      children_code(filterSlice, rank);
    } else {
      auto vec = combineAndSendOutFn(CHUNKS);
#if DEBUG_PARENT == 1
      load_balance::debug_vector(vec);
#endif
    }
    filter_count++;
  }
  std::vector<hits_lib::Hit> final;
  if (rank != 0) {
    int start = std::get<0>(filter_order[filter_count]);
    int end = std::get<1>(filter_order[filter_count]);
    auto filterSlice =
        std::vector<std::function<void(std::vector<hits_lib::Hit> &, int)>>(
            filter_list.begin() + start, filter_list.begin() + end);
    assert(filterSlice.size() > 0);
    children_code(filterSlice, rank);
  } else {
    auto final = combine_work(CHUNKS);
    time_end(start, rank);
  }
}
} // namespace load_balance
