
#pragma once
#include "enums.hpp"
#include "mpi.h"
#include <tuple>
#include <vector>

namespace load_balance {
std::vector<std::tuple<int, int>> load_balanced_indices(std::vector<int> vec,
                                                        int children_num);
}
