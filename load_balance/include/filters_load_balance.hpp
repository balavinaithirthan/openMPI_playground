#pragma once
#include "debug.hpp"
#include "enums.hpp"
#include "hit.hpp"
#include <cassert>
#include <vector>
namespace load_balance {
std::vector<hits_lib::Hit> make_zeros(std::vector<hits_lib::Hit> &vec,
                                      int rank);

void calculateSumVec(std::vector<hits_lib::Hit> &vec, int rank);

void memoryBoundFilter(std::vector<hits_lib::Hit> &vec, int rank);
} // namespace load_balance