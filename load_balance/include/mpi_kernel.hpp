#pragma once
#include "children.hpp"
#include "debug.hpp"
#include "enums.hpp"
#include "hit.hpp"
#include "parent.hpp"
#include "timing.hpp"
#include <functional>
#include <iostream>
#include <mpi.h>
#include <tuple>
#include <vector>
namespace load_balance {
void MPI_kernel(
    const int number_of_procs, const int rank, const int problem_size,
    std::vector<std::function<void(std::vector<filters::Hit> &, int)>>
        filter_list,
    const std::vector<std::tuple<int, int>> filter_order,
    std::vector<filters::Hit> &parent_vec);
} // namespace load_balance