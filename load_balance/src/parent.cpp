#include "parent.hpp"
#include <iostream>
namespace load_balance {
std::vector<hits_lib::Hit> flush(const std::vector<hits_lib::Hit> &vec) {
  std::vector<hits_lib::Hit> newVec;
  newVec.reserve(vec.size()); // Optimize by reserving max possible size

  std::copy_if(vec.begin(), vec.end(), std::back_inserter(newVec),
               [](hits_lib::Hit x) { return x.on != false; });

  return newVec;
}
// std::vector<hits_lib::Hit> redistribute(std::vector<hits_lib::Hit> vec, int
// redistribution_strategy,
//                               int rank) {
//   vec =
//   return
// }

void send_out_work(int children_num, std::vector<hits_lib::Hit> parent_vec,
                   int redistribution_strategy) {
  auto HIT_TYPE = hits_lib::get_hit_type();
  int chunk_size = parent_vec.size() / children_num;
  assert(chunk_size != 0);
  for (int i = 0; i < children_num - 1; i++) {
    std::vector<hits_lib::Hit> chunk((parent_vec.begin() + (i * chunk_size)),
                                     parent_vec.begin() +
                                         ((i + 1) * chunk_size));
    MPI_Send(&chunk_size, 1, MPI_INT, i + 1, VEC_SIZE, MPI_COMM_WORLD);
    MPI_Send(chunk.data(), chunk.size(), HIT_TYPE, i + 1, VEC_DATA,
             MPI_COMM_WORLD);
#if DEBUG_PARENT == 1
    printf("parent sending to %d\n", i + 1);
    load_balance::debug_vector(chunk);
#endif
  }
  // send out last non even chunk
  int last_index = children_num - 1;
  std::vector<hits_lib::Hit> chunk(
      (parent_vec.begin() + (last_index * chunk_size)), parent_vec.end());
  size_t final_chunk_size = chunk.size();
  MPI_Send(&final_chunk_size, 1, MPI_INT, last_index + 1, VEC_SIZE,
           MPI_COMM_WORLD);
  MPI_Send(chunk.data(), chunk.size(), HIT_TYPE, last_index + 1, VEC_DATA,
           MPI_COMM_WORLD);
}

std::vector<hits_lib::Hit> combine_work(int children_num) {

  auto HIT_TYPE = hits_lib::get_hit_type();

  std::vector<hits_lib::Hit> sumVec;
  for (int childRank = 1; childRank < children_num + 1; childRank++) {
    int chunkSize;
    MPI_Recv(&chunkSize, 1, MPI_INT, childRank, VEC_SIZE, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    std::vector<hits_lib::Hit> chunkVec(chunkSize);
    MPI_Recv(chunkVec.data(), chunkSize, HIT_TYPE, childRank, VEC_DATA,
             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    sumVec.insert(sumVec.end(), chunkVec.begin(),
                  chunkVec.end()); // insert into the end of sumVec
#if DEBUG_PARENT == 1
    std::cout << "parent receiving from " << childRank << std::endl;
    load_balance::debug_vector(chunkVec);
#endif
  }

  return sumVec;
}

std::vector<hits_lib::Hit> combineAndSendOutFn(const int children_num) {
  std::vector<hits_lib::Hit> combined = combine_work(children_num);
#if DEBUG_PARENT == 1
  load_balance::debug_vector(combined);
#endif
  // combined = flush(combined); TODO: flush
  send_out_work(children_num, combined, EQUAL_CHUNKING);
  return combined;
}
} // namespace load_balance