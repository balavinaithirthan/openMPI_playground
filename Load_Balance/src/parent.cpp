#include "parent.hpp"
#include <iostream>
namespace load_balance {
std::vector<int> flush(const std::vector<int> &vec) {
  std::vector<int> newVec;
  newVec.reserve(vec.size()); // Optimize by reserving max possible size

  std::copy_if(vec.begin(), vec.end(), std::back_inserter(newVec),
               [](int x) { return x != 0; });

  return newVec;
}
// std::vector<int> redistribute(std::vector<int> vec, int
// redistribution_strategy,
//                               int rank) {
//   vec =
//   return
// }

void send_out_work(int children_num, std::vector<int> parent_vec,
                   int redistribution_strategy) {
    int chunk_size = parent_vec.size() / children_num;
    assert(chunk_size != 0);
    for (int i = 0; i < children_num - 1; i++) {
      std::vector<int> chunk((parent_vec.begin() + (i * chunk_size)),
                             parent_vec.begin() + ((i + 1) * chunk_size));
      MPI_Send(&chunk_size, 1, MPI_INT, i + 1, VEC_SIZE, MPI_COMM_WORLD);
      MPI_Send(chunk.data(), chunk.size(), MPI_INT, i + 1, VEC_DATA,
               MPI_COMM_WORLD);
#if DEBUG == 1
      printf("parent sending to %d\n", i + 1);
      load_balance::debug_vector(chunk);
#endif
    }
    // send out last non even chunk
    int last_index = children_num - 1;
    std::vector<int> chunk((parent_vec.begin() + (last_index * chunk_size)),
                           parent_vec.end());
    size_t final_chunk_size = chunk.size();
    MPI_Send(&final_chunk_size, 1, MPI_INT, last_index + 1, VEC_SIZE,
             MPI_COMM_WORLD);
    MPI_Send(chunk.data(), chunk.size(), MPI_INT, last_index + 1, VEC_DATA,
             MPI_COMM_WORLD);
}

std::vector<int> combine_work(int children_num) {
  std::vector<int> sumVec;
  for (int childRank = 1; childRank < children_num + 1; childRank++) {
    int chunkSize;
    MPI_Recv(&chunkSize, 1, MPI_INT, childRank, VEC_SIZE, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    std::vector<int> chunkVec(chunkSize);
    MPI_Recv(chunkVec.data(), chunkSize, MPI_INT, childRank, VEC_DATA,
             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    sumVec.insert(sumVec.end(), chunkVec.begin(), chunkVec.end()); // insert into the end of sumVec
#if DEBUG == 1
    std::cout << "parent receiving from " << childRank << std::endl;
    load_balance::debug_vector(chunkVec);
#endif
  }
  return sumVec;
}

std::vector<int> combineAndSendOutFn(const int children_num) {
  std::vector<int> combined = combine_work(children_num);
#if DEBUG == 1
  load_balance::debug_vector(combined);
#endif
  // combined = flush(combined);
  send_out_work(children_num, combined, EQUAL_CHUNKING);
  return combined;
}
} // namespace load_balance