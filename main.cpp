#include <chrono>
#include <cstdio>
#include <functional>
#include <iostream>
#include <mpi.h>
#include <vector>

// #define DEBUG 1
#define SUPER_DEBUG 1
// #define TIMING 1

enum {
  RANK_0 = 0,
};

enum {
  VEC_SIZE = 0,
  VEC_DATA = 1,
};

enum {
  EQUAL_CHUNKING = 0,
  LOAD_BALANCED = 1,
};

std::vector<int> load_balanced_indices(std::vector<int> vec, int children_num) {
  std::vector<std::tuple<int, int>> indices;
  int non_zero_count = 0;
  for (int i = 0; i < vec.size();
       i++) { // TODO: children could keep track of their non_zero_count
    if (vec[i] == 0) {
      non_zero_count++;
    }
  }
  int chunk_size = non_zero_count / children_num;
  int start = 0;
  int end = 0;
  non_zero_count = 0;
  int done_children = 0;
  for (int i = 0; i < vec.size(); i++) {
    if (vec[i] == 0) {
      non_zero_count++;
    }
    if (non_zero_count == chunk_size) {
      non_zero_count = 0;
      done_children += 1;
      start = end;
      if (done_children == children_num - 1) {
        indices.emplace_back(end, vec.size());
        break;
      }
    }
  }
  return vec;
}

std::vector<int> make_zeros(const std::vector<int> &vec, int rank) {
  if (rank == 1) {
    return vec;
  }
  std::vector<int> newVec(vec.size());
  for (size_t i = 0; i < newVec.size(); ++i) {
    if (i % 5 != 4) { // Keep every 5th element (0-based index)
      newVec[i] = 0;
    } else {
      newVec[i] = vec[i];
    }
  }
  return newVec;
}

std::vector<int> calculateSumVec(const std::vector<int> &vec, int rank) {
#if DEBUG == 1
  printf("calculating sum \n");
#endif
  std::vector<int> vecCpy;
  for (int i = 0; i < vecCpy.size(); i++) {
    if (vecCpy[i] == 0) {
      continue;
    }
    for (int j = i + 1; j < 10000000; j++) {
      vecCpy[i] = vecCpy[i] + j;
    }
  }
  return vecCpy;
}

void debug_vector(std::vector<int> vec) {
#if SUPER_DEBUG == 1
  printf("[");
  for (int i = 0; i < vec.size(); i++) {
    printf("%d", vec[i]);
    if (i != vec.size() - 1) {
      printf(", ");
    }
  }
  printf("]\n");
#endif
}

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
  if (redistribution_strategy == EQUAL_CHUNKING) {
    int chunk_size = parent_vec.size() / children_num;
    for (int i = 0; i < children_num - 1; i++) {
      std::vector<int> chunk((parent_vec.begin() + (i * chunk_size)),
                             parent_vec.begin() + ((i + 1) * chunk_size));
      MPI_Send(&chunk_size, 1, MPI_INT, i + 1, VEC_SIZE, MPI_COMM_WORLD);
      MPI_Send(chunk.data(), chunk.size(), MPI_INT, i + 1, VEC_DATA,
               MPI_COMM_WORLD);
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

  if (redistribution_strategy == LOAD_BALANCED) { // auto flush
    int non_zero_count = 0;
    // Collect non-zero values
    for (int val : parent_vec) {
      if (val != 0) {
        non_zero_count++;
      }
    }

    int remainder = non_zero_count % children_num;
    int start = 0;
    for (int i = 0; i < children_num; i++) {
      int chunk_size = non_zero_count / children_num;
      if (i < remainder) {
        chunk_size++;
      }
      std::vector<int> chunk(non_zero_values.begin() + start,
                             non_zero_values.begin() + start + chunk_size);
      start += chunk_size;
      MPI_Send(&chunk_size, 1, MPI_INT, i + 1, VEC_SIZE, MPI_COMM_WORLD);
      MPI_Send(chunk.data(), chunk.size(), MPI_INT, i + 1, VEC_DATA,
               MPI_COMM_WORLD);
      debug_vector(chunk);
    }
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
    debug_vector(chunkVec);
#endif
  }
  return sumVec;
}

std::vector<int> combineAndSendOutFn(const int children_num) {
  auto combined = combine_work(children_num);
#if DEBUG == 1
  debug_vector(combined);
#endif
  flush(combined);
  send_out_work(children_num, combined, LOAD_BALANCED);
  return combined;
}

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
  }
  MPI_Send(&vec_size, 1, MPI_INT, RANK_0, VEC_SIZE, MPI_COMM_WORLD);
  MPI_Send(original_vector.data(), original_vector.size(), MPI_INT, RANK_0,
           VEC_DATA, MPI_COMM_WORLD);
#if DEBUG == 1
  std::cout << "child sending back " << std::endl;
  debug_vector(original_vector);
#endif
}

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

#if TIMING == 1
  auto start = std::chrono::high_resolution_clock::now();
#endif

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  const int CHUNKS = size - 1;
  if (size < CHUNKS + 1) {
    std::cerr << "This program requires at least " << CHUNKS + 1
              << " processes.\n ";
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  if (rank == 0) {
    const int PARENT_VEC_SIZE = 200;
    std::vector<int> parent_vec(PARENT_VEC_SIZE);
    for (int i = 0; i < PARENT_VEC_SIZE; i++) {
      parent_vec[i] = i;
    }
    if (PARENT_VEC_SIZE < size) {
      std::cerr << "This program requires at least " << PARENT_VEC_SIZE
                << " processes.\n ";
      MPI_Abort(MPI_COMM_WORLD, 1);
    }
    send_out_work(CHUNKS, parent_vec, EQUAL_CHUNKING);
  }

  std::vector<std::function<std::vector<int>(const std::vector<int> &, int)>>
      filter_list;
  filter_list.emplace_back(make_zeros);
  filter_list.emplace_back(calculateSumVec);
  // filter_list.emplace_back(calculateSumVec);
  std::vector<std::tuple<int, int>> filterOrder = {
      {0, 1}, {1, 2}}; // filter start up to but not including end

  if (filter_list.empty()) {
    std::cerr << "Filter list is empty\n";
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  int filter_count = 0;
  while (true) {
    if (filter_count == filter_list.size() - 1) {
      break;
    }
    if (rank != 0) {
      int start = std::get<0>(filterOrder[filter_count]);
      int end = std::get<1>(filterOrder[filter_count]);
      auto filterSlice = std::vector<
          std::function<std::vector<int>(const std::vector<int> &, int)>>(
          filter_list.begin() + start, filter_list.begin() + end);
      children_code(filterSlice, rank);
    } else {
      auto vec = combineAndSendOutFn(CHUNKS);
#if DEBUG == 1
      debug_vector(vec);
#endif
    }
    filter_count++;
#if DEBUG == 1
    printf("filter count %d\n", filter_count);
#endif
  }

  if (rank != 0) {
    int start = std::get<0>(filterOrder[filter_count]);
    int end = std::get<1>(filterOrder[filter_count]);
    auto filterSlice = std::vector<
        std::function<std::vector<int>(const std::vector<int> &, int)>>(
        filter_list.begin() + start, filter_list.begin() + end);
    children_code(filterSlice, rank);
  } else {
    auto final = combine_work(CHUNKS);
#if SUPER_DEBUG == 1
    debug_vector(final);
#endif
  }

#if TIMING == 1
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;
  if (rank == 0) {
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
  }
#endif

  MPI_Finalize();
  return 0;
}

// mpirun -np 5 ./hello_mpi