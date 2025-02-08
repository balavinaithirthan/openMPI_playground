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

std::vector<int> calculateSumVec(const std::vector<int> &vec, int rank) {
#if DEBUG == 1
  printf("calculating sum \n");
#endif
  std::vector<int> vecCpy(vec.size(), 0);
  for (int i = 0; i < vec.size(); i++) {
    if (vec[i] == 0) {
      continue;
    }
    for (int j = i + 1; j < 10000000; j++) {
      vecCpy[i] = vec[i] + j;
      vecCpy[i] = vec[i] - j;
    }
  }
  load_balance::debug_vector(vecCpy);
  return vecCpy;
}
} // namespace load_balance