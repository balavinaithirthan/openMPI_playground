#include "parent.hpp"
namespace load_balance {
void flush(std::vector<int> vec) {
  std::vector<int> newVec = std::vector<int>{};
  for (int i = 0; i < vec.size(); i++) {
    if (vec[i] == 0) {
      continue;
    } else {
      newVec.emplace_back(vec[i]);
    }
  }
}

// std::vector<int> redistribute(std::vector<int> vec, int
// redistribution_strategy,
//                               int rank) {
//   vec =
//   return
// }

void send_out_work(int children_num, std::vector<int> parent_vec,
                   int redistribution_strategy) {
  if (redistribution_strategy == LOAD_BALANCED) { // auto flush
    std::vector<std::tuple<int, int>> vec =
        load_balanced_indices(parent_vec, children_num);
    load_balance::debug_indices(vec);
    if (vec.empty()) {
      redistribution_strategy = EQUAL_CHUNKING;
    }
    if (redistribution_strategy == LOAD_BALANCED) {
      int i = 0;
      for (auto &index : vec) {
        int start = std::get<0>(index);
        int end = std::get<1>(index);
        std::vector<int> chunk(parent_vec.begin() + start,
                               parent_vec.begin() + end);
        int size = end - start;
        MPI_Send(&size, 1, MPI_INT, i + 1, VEC_SIZE, MPI_COMM_WORLD);
        MPI_Send(chunk.data(), size, MPI_INT, i + 1, VEC_DATA, MPI_COMM_WORLD);

        i++;
      }
    }
  }
  if (redistribution_strategy == EQUAL_CHUNKING) {
    int chunk_size = parent_vec.size() / children_num;
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
    sumVec.insert(sumVec.end(), chunkVec.begin(), chunkVec.end());
#if DEBUG == 1
    std::cout << "parent receiving from " << childRank << std::endl;
    load_balance::debug_vector(chunkVec);
#endif
  }
  return sumVec;
}

std::vector<int> combineAndSendOutFn(const int children_num) {
  auto combined = combine_work(children_num);
#if DEBUG == 1
  load_balance::debug_vector(combined);
#endif
  // flush(combined);
  send_out_work(children_num, combined, LOAD_BALANCED);
  return combined;
}
} // namespace load_balance