#pragma once
#include "enums.hpp"
#include <functional>
#include <iostream>
#include <tuple>
#include <vector>
namespace load_balance {
void debug_vector(const std::vector<int> &vec);
void print_vector(const std::vector<int> &vec);
void debug_indices(std::vector<std::tuple<int, int>> indices);
void print_filter_list(
    std::vector<std::function<void(std::vector<int> &, int)>> filter_list);

inline void print_filter_order(std::vector<std::tuple<int, int>> filter_order) {
  for (const auto &filter : filter_order) {
    std::cout << "filter slice is " << std::get<0>(filter) << " "
              << std::get<1>(filter) << std::endl;
  }
}
} // namespace load_balance