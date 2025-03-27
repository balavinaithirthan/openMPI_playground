#pragma once
#include "debug.hpp"
#include "enums.hpp"
#include "hit.hpp"
#include "mpi.h"
#include <cassert>
#include <iostream>
#include <vector>
namespace load_balance {
void children_code(
    std::vector<std::function<void(std::vector<hits_lib::Hit> &, int)>>
        filter_list,
    int rank);
}