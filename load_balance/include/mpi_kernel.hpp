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
#include "timing.hpp"
#include "debug.hpp"
namespace load_balance {
void MPI_kernel(const int number_of_procs, const int rank, const std::function<void(std::vector<int> &, int)> filterFunction,
       const int number_of_filters, const int problem_size, const std::vector<std::tuple<int, int>> filter_order);
      } // namespace load_balance