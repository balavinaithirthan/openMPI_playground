#include "children.hpp"
#include "debug.hpp"

namespace load_balance {

void children_code(
    std::vector<std::function<void(std::vector<hits_lib::Hit> &, int)>>
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
  assert(filter_list.size() > 0);
  for (const auto &filterFun : filter_list) {
    try {
      // filterFun(original_vector, rank);
    } catch (const std::bad_function_call &e) {
      std::cerr << "Error: bad function call - " << e.what() << std::endl;
      continue;
    }
    printf("fopefjiowefjoiwefjowiefjweoifjwfjwefjweoo\n");
    assert(original_vector.size() == vec_size);
  }
  int new_vec_size = original_vector.size();

  MPI_Send(&new_vec_size, 1, MPI_INT, RANK_0, VEC_SIZE, MPI_COMM_WORLD);
  MPI_Send(original_vector.data(), original_vector.size(), MPI_INT, RANK_0,
           VEC_DATA, MPI_COMM_WORLD);
#if DEBUG == 1
  std::cout << "child sending back " << std::endl;
  load_balance::debug_vector(original_vector);
#endif
}
} // namespace load_balance