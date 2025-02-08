#pragma once
#include "enums.hpp"
#include "load_balance_indices.hpp"
#include <iostream>
#include <mpi.h>
#include <tuple>
#include <vector>
namespace load_balance {
std::vector<int> flush(const std::vector<int> &vec);

void send_out_work(int children_num, std::vector<int> parent_vec,
                   int redistribution_strategy);

std::vector<int> combine_work(int children_num);

std::vector<int> combineAndSendOutFn(const int children_num);
} // namespace load_balance