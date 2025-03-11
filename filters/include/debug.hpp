#include "hit.hpp"
#include <iostream>

namespace filters {
inline void debug_hits(std::vector<Hit> hits) {
  for (double i = 0; i < hits.size(); i++) {
    std::cout << "Hit " << i << " is at position " << hits[i].getX() << ", "
              << hits[i].getY() << " with length " << hits[i].length
              << std::endl;
  }
}

inline void debug_filter_order(std::vector<std::tuple<int, int>> filter_order) {
  for (int i = 0; i < filter_order.size(); i++) {
    printf("start: %d, end: %d\n", std::get<0>(filter_order[i]),
           std::get<1>(filter_order[i]));
  }
}

inline void debug_filter_list() {}
} // namespace filters