#pragma once
#include "debug.hpp"
#include "enums.hpp"
#include <vector>
#include <cassert>
namespace load_balance {
std::vector<int> make_zeros( std::vector<int> &vec, int rank);

void calculateSumVec(std::vector<int> &vec, int rank);

void memoryBoundFilter(std::vector<int> &vec, int rank);
} // namespace load_balance