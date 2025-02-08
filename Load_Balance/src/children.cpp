#include "children.hpp"

namespace load_balance {

void children_code(
    std::vector<std::function<std::vector<int>(const std::vector<int> &, int)>>
        filter_list,
    int rank) {
  int vec_size;
  MPI_Recv(&vec_size, 1, MPI_INT, RANK_0, VEC_SIZE, MPI_COMM_WORLD,
           MPI_STATUS_IGNORE);
  std::vector<int> original_vector(vec_size);
  MPI_Recv(original_vector.data(), vec_size, MPI_INT, RANK_0, VEC_DATA,
           MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#if DEBUG == 1
  printf("filter list has %lu\n", filter_list.size());
#endif
  for (const auto &filterFun : filter_list) {
    original_vector = filterFun(original_vector, rank);
#if DEBUG == 1
    std::cout << "filter happening " << std::endl;
    load_balance::debug_vector(original_vector);
#endif
  }

  MPI_Send(&vec_size, 1, MPI_INT, RANK_0, VEC_SIZE, MPI_COMM_WORLD);
  MPI_Send(original_vector.data(), original_vector.size(), MPI_INT, RANK_0,
           VEC_DATA, MPI_COMM_WORLD);
#if DEBUG == 1
  std::cout << "child sending back " << std::endl;
  load_balance::debug_vector(original_vector);
#endif
}
} // namespace load_balance