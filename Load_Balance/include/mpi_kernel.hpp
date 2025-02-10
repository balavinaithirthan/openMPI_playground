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
std::vector<int> MPI_kernel(const int number_of_procs, const int rank, const std::function<std::vector<int>(const std::vector<int> &, int)>
      filterFunction, const int number_of_filters, const int problem_size, const std::vector<std::tuple<int, int>> filter_order);
      } // namespace load_balance