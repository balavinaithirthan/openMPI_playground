#include "filters.hpp"
#include <array>

namespace load_balance {
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

// can make as in place filter later
void calculateSumVec(std::vector<int> &vec, int rank) {
#if DEBUG == 1
  printf("calculating sum \n");
#endif
  int original_size = vec.size();
  for (int i = 0; i < vec.size(); i++) {
    if (vec[i] == 0) {
      continue;
    }
    for (int j = 0; j < 300000; j++) {
      vec[i] += vec[i] * j * 2 * j + (j/3);
    }
  }
  assert(vec.size() == original_size);
  debug_vector(vec);
  return;
}

constexpr int N = 4;  // Must be known at compile time

constexpr auto make_matrix() {
    std::array<std::array<int, N>, N> arr = {};
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            arr[i][j] = 6.0f;  // Can be constexpr!
        }
    }
    return arr;
}

constexpr auto A = make_matrix(); // Computed at compile time
constexpr auto B = make_matrix();

  
int Transpose(std::array<std::array<int, N>, N> A, std::array<std::array<int, N>, N> B, int i)
{
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            A[i][j] = B[j][i] + i;
        }
    }
    return A[3][3];
}

void memoryBoundFilter(std::vector<int> &vec, int rank) {
  int initial_size = vec.size();
  for (int i = 0; i < vec.size(); i++) {
    if (vec[i] == 0) {
      continue;
    }
    vec[i] = Transpose(A, B, i);
    
  }
  assert(vec.size() == initial_size);
}

} // namespace load_balance