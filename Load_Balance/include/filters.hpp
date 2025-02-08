#pragma once
#include "debug.hpp"
#include <vector>

namespace load_balance {
std::vector<int> make_zeros(const std::vector<int> &vec, int rank);

std::vector<int> calculateSumVec(const std::vector<int> &vec, int rank);
} // namespace load_balance