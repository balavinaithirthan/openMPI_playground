#pragma once
#include "enums.hpp"
#include <vector>
#include <tuple>
#include <vector>
#include <functional>
#include <iostream>
namespace load_balance {
void debug_vector(const std::vector<int> &vec);
void print_vector(const std::vector<int> &vec);
void debug_indices(std::vector<std::tuple<int, int>> indices);

} // namespace load_balance