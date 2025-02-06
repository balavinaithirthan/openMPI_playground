#include <cstdio>
#include <functional>
#include <iostream>
#include <mpi.h>
#include <vector>

enum {
  RANK_0 = 0,
};

enum {
  VEC_SIZE = 0,
  VEC_DATA = 1,
};

std::vector<int> calculateSumVec(const std::vector<int> &vec) {
  printf("calculating sum \n");
  std::vector<int> result(vec.size());
  result[0] = vec[0];
  for (int i = 1; i < vec.size(); i++) {
    result[i] = vec[i] + result[i - 1];
  }
  return result;
}

void debug_vector(std::vector<int> vec) {
  printf("[");
  for (int i = 0; i < vec.size(); i++) {
    printf("%d", vec[i]);
    if (i != vec.size() - 1) {
      printf(", ");
    }
  }
  printf("]\n");
}

void flush() {}

void redistribute() {}

void send_out_work(int children_num, std::vector<int> parent_vec,
                   int redistribution_strategy) {
  int chunk_size = parent_vec.size() / children_num;
  for (int i = 0; i < children_num; i++) {
    std::vector<int> chunk((parent_vec.begin() + (i * chunk_size)),
                           parent_vec.begin() + ((i + 1) * chunk_size));
    MPI_Send(&chunk_size, 1, MPI_INT, i + 1, VEC_SIZE, MPI_COMM_WORLD);
    MPI_Send(chunk.data(), chunk.size(), MPI_INT, i + 1, VEC_DATA,
             MPI_COMM_WORLD);
    std::cout << "parent sending out " << i << std::endl;
    debug_vector(chunk);
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
    std::cout << "parent receiving from " << childRank << std::endl;
    debug_vector(chunkVec);
  }
  return sumVec;
}

void combineAndSendOutFn(const int children_num) {
  auto combined = combine_work(children_num);
  debug_vector(combined);
  flush();
  send_out_work(children_num, combined, 0);
}

void children_code(
    std::vector<std::function<std::vector<int>(const std::vector<int> &)>>
        filter_list,
    int rank) {
  int vec_size;
  MPI_Recv(&vec_size, 1, MPI_INT, RANK_0, VEC_SIZE, MPI_COMM_WORLD,
           MPI_STATUS_IGNORE);
  std::vector<int> original_vector(vec_size);
  MPI_Recv(original_vector.data(), vec_size, MPI_INT, RANK_0, VEC_DATA,
           MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  for (const auto &filterFun : filter_list) {
    original_vector = filterFun(original_vector);
  }
  MPI_Send(&vec_size, 1, MPI_INT, RANK_0, VEC_SIZE, MPI_COMM_WORLD);
  MPI_Send(original_vector.data(), original_vector.size(), MPI_INT, RANK_0,
           VEC_DATA, MPI_COMM_WORLD);
  std::cout << "child sending back " << std::endl;
  debug_vector(original_vector);
}

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  const int CHUNKS = size - 1;
  if (size < CHUNKS + 1) {
    std::cerr << "This program requires at least " << CHUNKS + 1
              << " processes.\n ";
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  std::vector<std::function<std::vector<int>(const std::vector<int> &)>>
      filter_list;
  filter_list.emplace_back(calculateSumVec);

  if (rank == 0) {
    const int VEC_SIZE = 100;
    std::vector<int> parent_vec(VEC_SIZE);
    for (int i = 0; i < VEC_SIZE; i++) {
      parent_vec[i] = i;
    }
    send_out_work(CHUNKS, parent_vec, 0);
  }

  if (filter_list.empty()) {
    std::cerr << "Filter list is empty\n";
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  int filter_count = filter_list.size();
  while (true) {
    if (filter_count == 1) {
      break;
    }
    if (rank != 0) {
      children_code(filter_list, rank);
    } else {
      combineAndSendOutFn(CHUNKS);
    }
    filter_count--;
    printf("filter count %d\n", filter_count);
  }

  if (rank != 0) {
    children_code(filter_list, rank);
  } else {
    combine_work(CHUNKS);
  }

  MPI_Finalize();
  return 0;
}

// mpirun -np 5 ./hello_mpi