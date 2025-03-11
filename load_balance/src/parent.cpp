#include "parent.hpp"
#include "debug.hpp"
#include <iostream>
namespace load_balance {
std::vector<filters::Hit> flush(const std::vector<filters::Hit> &vec) {
  std::vector<filters::Hit> newVec;
  newVec.reserve(vec.size()); // Optimize by reserving max possible size

  std::copy_if(vec.begin(), vec.end(), std::back_inserter(newVec),
               [](filters::Hit x) { return x.on != false; });

  return newVec;
}
// std::vector<filters::Hit> redistribute(std::vector<filters::Hit> vec, int
// redistribution_strategy,
//                               int rank) {
//   vec =
//   return
// }

void send_out_work(int children_num, std::vector<filters::Hit> parent_vec,
                   int redistribution_strategy) {

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

  ///////////////////

  int chunk_size = parent_vec.size() / children_num;
  assert(chunk_size != 0);
  for (int i = 0; i < children_num - 1; i++) {
    std::vector<filters::Hit> chunk((parent_vec.begin() + (i * chunk_size)),
                                    parent_vec.begin() +
                                        ((i + 1) * chunk_size));
    MPI_Send(&chunk_size, 1, MPI_INT, i + 1, VEC_SIZE, MPI_COMM_WORLD);
    MPI_Send(chunk.data(), chunk.size(), hit_type, i + 1, VEC_DATA,
             MPI_COMM_WORLD);
    // #if DEBUG == 1
    //     printf("parent sending to %d\n", i + 1);
    //     load_balance::debug_vector(chunk);
    // #endif
  }
  // send out last non even chunk
  int last_index = children_num - 1;
  std::vector<filters::Hit> chunk(
      (parent_vec.begin() + (last_index * chunk_size)), parent_vec.end());
  size_t final_chunk_size = chunk.size();
  MPI_Send(&final_chunk_size, 1, MPI_INT, last_index + 1, VEC_SIZE,
           MPI_COMM_WORLD);
  MPI_Send(chunk.data(), chunk.size(), hit_type, last_index + 1, VEC_DATA,
           MPI_COMM_WORLD);

  ///////////////////
  MPI_Type_free(&hit_type);
}

std::vector<filters::Hit> combine_work(int children_num) {
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
  ///////////////////

  std::vector<filters::Hit> sumVec;
  for (int childRank = 1; childRank < children_num + 1; childRank++) {
    int chunkSize;
    MPI_Recv(&chunkSize, 1, MPI_INT, childRank, VEC_SIZE, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    std::vector<filters::Hit> chunkVec(chunkSize);
    MPI_Recv(chunkVec.data(), chunkSize, hit_type, childRank, VEC_DATA,
             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    sumVec.insert(sumVec.end(), chunkVec.begin(),
                  chunkVec.end()); // insert into the end of sumVec
#if DEBUG == 1
    std::cout << "parent receiving from " << childRank << std::endl;
    load_balance::debug_vector(chunkVec);
#endif
  }
  ///////////////////
  MPI_Type_free(&hit_type);
  ///////////////////

  return sumVec;
}

std::vector<filters::Hit> combineAndSendOutFn(const int children_num) {
  std::vector<filters::Hit> combined = combine_work(children_num);
#if DEBUG == 1
  load_balance::debug_vector(combined);
#endif
  // combined = flush(combined);
  send_out_work(children_num, combined, EQUAL_CHUNKING);
  return combined;
}
} // namespace load_balance