#include "debug.hpp"

namespace load_balance {
void debug_vector(const std::vector<int> &vec) {
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

void debug_indices(std::vector<std::tuple<int, int>> indices) {
  for (int i = 0; i < indices.size(); i++) {
    printf("start: %d, end: %d\n", std::get<0>(indices[i]),
           std::get<1>(indices[i]));
  }
}

void print_vector(const std::vector<int> &vec) {
  printf("[");
  for (int i = 0; i < vec.size(); i++) {
    printf("%d", vec[i]);
    if (i != vec.size() - 1) {
      printf(", ");
    }
  }
  printf("]\n");
}

void print_filter_list(
    std::vector<std::function<void(std::vector<int> &, int)>>
        filter_list) {
  for (int i = 0; i < filter_list.size(); i++) {
    printf("filter %d\n", i);
  }
}

} // namespace load_balance