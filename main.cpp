#include <functional>
#include <iostream>
#include <mpi.h>
#include <vector>

std::vector<int> calculateSumVec(const std::vector<int> &vec) {
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
  // include redistribution strategy
  int chunk_size;
  for (int i = 0; i < children_num; i++) {
    MPI_Send(&chunk_size, 1, MPI_INT, i + 1, 0, MPI_COMM_WORLD);

    std::vector<int> chunk((parent_vec.begin() + (i * chunk_size)),
                           parent_vec.begin() + ((i + 1) * chunk_size));
    MPI_Send(chunk.data(), chunk.size(), MPI_INT, i + 1, 1, MPI_COMM_WORLD);
    debug_vector(chunk);
  }
}

std::vector<int> combine_work(int children_num) {
  std::vector<int> sumVec;
  for (int childRank = 1; childRank < children_num + 1; childRank++) {
    int chunkSize;
    MPI_Recv(&chunkSize, 1, MPI_INT, childRank, 2, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    std::vector<int> chunkVec(chunkSize);
    MPI_Recv(chunkVec.data(), chunkSize, MPI_INT, childRank, 3, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    sumVec.insert(sumVec.end(), chunkVec.begin(), chunkVec.end());
  }
  return sumVec;
}

void combineAndSendOutFn(const int children_num) {
  // which children??
  auto combined = combine_work(children_num);
  flush();
  send_out_work(children_num, combined, 0);
  // add flushes here
}

void chunk_children(
    std::vector<std::function<std::vector<int>(std::vector<int>)>>
        filter_list) {
  int vec_size;
  MPI_Recv(&vec_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  std::vector<int> original_vector(vec_size);
  MPI_Recv(original_vector.data(), vec_size, MPI_INT, 0, 1, MPI_COMM_WORLD,
           MPI_STATUS_IGNORE);
  std::vector<int> new_vec;
  auto prevVec = std::move(original_vector);
  for (const auto &filter : filter_list) {
    new_vec = filter(prevVec);
    prevVec = new_vec;
  }

  MPI_Send(&vec_size, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
  MPI_Send(new_vec.data(), new_vec.size(), MPI_INT, 0, 3, MPI_COMM_WORLD);
}

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  const int CHUNKS = 4;
  if (size < CHUNKS + 1) {
    std::cerr << "This program requires at least " << CHUNKS + 1
              << " processes.\n ";
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  std::vector<std::function<std::vector<int>(std::vector<int>)>> filter_list;
  if (rank == 0) { // initial setup
    filter_list.emplace_back(calculateSumVec);
    // initialize filter list
    const int VEC_SIZE = 100;
    std::vector<int> parent_vec;
    for (int i = 0; i < VEC_SIZE; i++) {
      parent_vec.push_back(i);
    }
    int chunk_size = VEC_SIZE / CHUNKS; // TODO: assume perfectly divisible
    for (int i = 0; i < CHUNKS; i++) {
      std::vector<int> chunk((parent_vec.begin() + (i * chunk_size)),
                             parent_vec.begin() + ((i + 1) * chunk_size));
      MPI_Send(&chunk_size, 1, MPI_INT, i + 1, 0, MPI_COMM_WORLD);
      MPI_Send(chunk.data(), chunk.size(), MPI_INT, i + 1, 1, MPI_COMM_WORLD);
      debug_vector(chunk);
    }
  }

  if (rank != 0) {
    std::vector<std::function<std::vector<int>(std::vector<int>)>> filter_slice(
        filter_list.begin() + 1, filter_list.begin() + 4);
    chunk_children(filter_slice);
  } else {
    combineAndSendOutFn(CHUNKS);
  }

  if (rank == 0) {
    flush();
    redistribute();
  }
  MPI_Finalize();
  return 0;
}
// questions
// should vector be copied over directly?
// should vector be a shared data structure so that we can avoid copying?
// only share numbers/view

// layered distribution

// mpic++ -std=c++11 -stdlib=libc++ -o hello_mpi main.cpp -v