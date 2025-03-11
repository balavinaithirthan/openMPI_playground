#pragma once
#include "enums.hpp"
#include "hit.hpp"
#include "load_balance_indices.hpp"
#include <cassert>
#include <iostream>
#include <mpi.h>
#include <tuple>
#include <vector>
namespace load_balance {
std::vector<filters::Hit> flush(const std::vector<filters::Hit> &vec);

void send_out_work(int children_num, std::vector<filters::Hit> parent_vec,
                   int redistribution_strategy);

std::vector<filters::Hit> combine_work(int children_num);

std::vector<filters::Hit> combineAndSendOutFn(const int children_num);
} // namespace load_balance