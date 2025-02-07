
#include <stdio.h>
#include <tuple>
#include <vector>

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
    printf("error there are not enough values for a proc to take on  "
           "children_num\n");
    return indices;
  }
  if (non_zero_count == 0) {
    printf("error non_zero_count == 0\n");
    return indices;
  }
  int chunk_size = non_zero_count / children_num;
  int start = 0;
  non_zero_count = 0;
  int done_children = 0;
  for (int i = 0; i < vec.size(); i++) {
    if (vec[i] != 0) {
      non_zero_count++;
    }
    if (non_zero_count == chunk_size) {
      indices.emplace_back(start, i);
      non_zero_count = 0;
      done_children += 1;
      start = i + 1;
      if (done_children == children_num - 1) {
        indices.emplace_back(start, vec.size() - 1);
        break;
      }
    }
  }
  return indices;
}

int main() {
  std::vector<int> vec = {0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1};
  int children_num = 3;
  std::vector<std::tuple<int, int>> indices =
      load_balanced_indices(vec, children_num);
  for (int i = 0; i < indices.size(); i++) {
    printf("start: %d, end: %d\n", std::get<0>(indices[i]),
           std::get<1>(indices[i]));
  }
  return 0;
}