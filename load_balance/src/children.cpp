#include "children.hpp"
#include "debug.hpp"

namespace load_balance {

void children_code(
    std::vector<std::function<void(std::vector<filters::Hit> &, int)>>
        filter_list,
    int rank) {
  MPI_Datatype hit_type;
  {
    filters::Hit temp_hit;

    // Define the data types for each member of the Hit class
    int block_lengths[4] = {1, 1, 1, 1};
    MPI_Datatype types[4] = {MPI_C_BOOL, MPI_DOUBLE, MPI_UNSIGNED_LONG,
                             MPI_UNSIGNED_LONG};
    MPI_Aint displacements[4];

    // Calculate the displacements of each field
    displacements[0] = offsetof(filters::Hit, on);         // bool on
    displacements[1] = offsetof(filters::Hit, length);     // double length
    displacements[2] = offsetof(filters::Hit, position_x); // size_t position_x
    displacements[3] = offsetof(filters::Hit, position_y); // size_t position_y

    MPI_Type_create_struct(4, block_lengths, displacements, types, &hit_type);
    MPI_Type_commit(&hit_type);
  };

  /////////////////////
  int vec_size;
  MPI_Recv(&vec_size, 1, MPI_INT, RANK_0, VEC_SIZE, MPI_COMM_WORLD,
           MPI_STATUS_IGNORE);

  std::vector<filters::Hit> original_vector(vec_size);
  MPI_Recv(original_vector.data(), vec_size, hit_type, RANK_0, VEC_DATA,
           MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  print_vector(original_vector);
#if DEBUG == 1
  printf("filter list has %lu\n", filter_list.size());
#endif
  assert(filter_list.size() > 0);
  for (const auto &filterFun : filter_list) {
    // if (!filterFun) {
    //   std::cerr << "Error: encountered an uninitialized function in
    //   filter_list" << std::endl; continue;
    // }
    // try {
    filterFun(original_vector, rank);
    // } catch (const std::bad_function_call &e) {
    //   std::cerr << "Error: bad function call - " << e.what() << std::endl;
    //   continue;
    // }
    // printf("fopefjiowefjoiwefjowiefjweoifjwfjwefjweoo\n");
    assert(original_vector.size() == vec_size);
  }
  int new_vec_size = original_vector.size();

  MPI_Send(&new_vec_size, 1, MPI_INT, RANK_0, VEC_SIZE, MPI_COMM_WORLD);
  MPI_Send(original_vector.data(), original_vector.size(), hit_type, RANK_0,
           VEC_DATA, MPI_COMM_WORLD);
#if DEBUG == 1
  std::cout << "child sending back " << std::endl;
  load_balance::debug_vector(original_vector);
#endif

  ///////////////////
  MPI_Type_free(&hit_type);
}
} // namespace load_balance