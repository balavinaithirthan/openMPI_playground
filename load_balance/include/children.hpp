#pragma once
#include "debug.hpp"
#include "enums.hpp"
#include "mpi.h"
#include <iostream>
#include <vector>
#include <cassert>
namespace load_balance {
void children_code(
    std::vector<std::function<void(std::vector<int> &, int)>>
        filter_list,
    int rank);
}