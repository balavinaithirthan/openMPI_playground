#pragma once
#include "enums.hpp"
#include "hit.hpp"
#include <functional>
#include <iostream>
#include <tuple>
#include <vector>

namespace load_balance {
void debug_vector(const std::vector<filters::Hit> &vec);
void print_vector(const std::vector<filters::Hit> &vec);
void debug_indices(std::vector<std::tuple<int, int>> indices);
void print_filter_list(
    std::vector<std::function<void(std::vector<filters::Hit> &, int)>>
        filter_list);

inline void print_filter_order(std::vector<std::tuple<int, int>> filter_order) {
  for (const auto &filter : filter_order) {
    std::cout << "filter slice is " << std::get<0>(filter) << " "
              << std::get<1>(filter) << std::endl;
  }
}

inline void debug_hits(std::vector<filters::Hit> hits) {
  for (double i = 0; i < hits.size(); i++) {
    std::cout << "Hit " << i << " is at position " << hits[i].getX() << ", "
              << hits[i].getY() << " with length " << hits[i].length
              << std::endl;
  }
}
} // namespace load_balance