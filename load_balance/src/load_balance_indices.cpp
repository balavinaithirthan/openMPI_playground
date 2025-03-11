
#include "load_balance_indices.hpp"

namespace load_balance {
std::vector<std::tuple<int, int>> load_balanced_indices(std::vector<int> vec,
                                                        int children_num) {
  std::vector<std::tuple<int, int>> indices;
  int non_zero_count = 0;
  for (int i = 0; i < vec.size();
       i++) { // TODO: children could keep track of their non_zero_count
    if (vec[i] != 0) {
      non_zero_count++;
    }
  }
  if (non_zero_count < children_num) {
    printf("not enough values to take on children_num\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
    return indices;
  }
  if (non_zero_count == 0) {
    printf("error non_zero_count == 0\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  int chunk_size = non_zero_count / children_num;
  int start = 0;
  non_zero_count = 0;
  int done_children = 0;
  for (int i = 0; i < vec.size(); i++) {
    if (vec[i] != 0) {
      non_zero_count++; // TODO: children could keep track of their
                        // non_zero_count
    }
    if (non_zero_count == chunk_size) {
      indices.emplace_back(start, i); // Make the last index non-inclusive
      non_zero_count = 0;
      done_children += 1;
      start = i;
      if (done_children == children_num - 1) {
        indices.emplace_back(start,
                             vec.size()); // Make the last index non-inclusive
        break;
      }
    }
  }
  return indices;
}
} // namespace load_balance