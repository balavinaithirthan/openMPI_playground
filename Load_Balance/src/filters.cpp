#include "filters.hpp"

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
std::vector<int> calculateSumVec(const std::vector<int> &vec, int rank) {
#if DEBUG == 1
  printf("calculating sum \n");
#endif
  std::vector<int> vecCpy(vec.size(), 0);
  for (int i = 0; i < vec.size(); i++) {
    if (vec[i] == 0) {
      continue;
    }
    for (int j = 0; j < 300000; j++) {
      vecCpy[i] += vec[i] * j * 2 * j + (j/3);
    }
  }
  assert(vecCpy.size() == vec.size());
  debug_vector(vecCpy);
  return vecCpy;
}

int stencil_step(const std::vector<std::vector<double>>& input,
                  std::vector<std::vector<double>>& output,
                  double alpha) {
    int rows = input.size();
    int cols = input[0].size();

    for (int i = 1; i < rows - 1; ++i) {
        for (int j = 1; j < cols - 1; ++j) {
            output[i][j] = input[i][j] +
                           alpha * (input[i - 1][j] + input[i + 1][j] +
                                    input[i][j - 1] + input[i][j + 1] -
                                    4 * input[i][j]);
        }
    }
    return (int)output[rows - 2][cols - 2];
}

std::vector<int> memoryBoundFilter(const std::vector<int> &vec, int rank) {
  std::vector<int> vecCpy(vec.size(), 0);
  for (int i = 0; i < vec.size(); i++) {
    if (vec[i] == 0) {
      continue;
    }
    int rows = 1000;
    int cols = 1000;
    double alpha = 0.1;
    std::vector<std::vector<double>> grid(rows, std::vector<double>(cols, 0.0));
    std::vector<std::vector<double>> new_grid = grid;
    grid[5][5] = 100.0;
       
    vecCpy[i] = stencil_step(grid, new_grid, alpha);
    
  }
  assert(vecCpy.size() == vec.size());
  debug_vector(vecCpy);
  return vecCpy;
}

} // namespace load_balance