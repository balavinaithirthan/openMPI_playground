#pragma once
#include "debug.hpp"
#include "enums.hpp"
#include "mpi.h"
#include <iostream>
#include <vector>
namespace load_balance {
void children_code(
    std::vector<std::function<std::vector<int>(const std::vector<int> &, int)>>
        filter_list,
    int rank);
}