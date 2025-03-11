#pragma once
#include "debug.hpp"
#include "enums.hpp"
#include <cassert>
#include <vector>
namespace load_balance {
std::vector<filters::Hit> make_zeros(std::vector<filters::Hit> &vec, int rank);

void calculateSumVec(std::vector<filters::Hit> &vec, int rank);

void memoryBoundFilter(std::vector<filters::Hit> &vec, int rank);
} // namespace load_balance