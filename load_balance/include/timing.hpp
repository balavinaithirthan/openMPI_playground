#include <iostream>
#include <chrono>
#include "enums.hpp"

namespace load_balance {

std::chrono::time_point<std::chrono::steady_clock> time_start();

void time_end(std::chrono::time_point<std::chrono::steady_clock> start, int rank);

} // namespace load_balance
