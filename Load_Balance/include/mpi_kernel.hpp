#pragma once
#include <vector>
#include <tuple>
#include <functional>
#include <mpi.h>
#include <iostream>
#include "debug.hpp"
#include "enums.hpp"
#include "parent.hpp"
#include "children.hpp"

namespace load_balance {
    void MPI_kernel(int number_of_procs, int rank, int filter_number, std::function<std::vector<int>(const std::vector<int> &, int)>
      filterFunction, int number_of_filters, int problem_size, std::vector<std::tuple<int, int>> filter_order);
}