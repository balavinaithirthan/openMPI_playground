#include "enums.hpp"
#include <chrono>
#include <iostream>
namespace load_balance {
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<long long, std::ratio<1, 1000000000>>> time_start();
    void time_end(int rank);
    void time_end(std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<long long, std::ratio<1, 1000000000>>> start,
int rank);
} // namespace load_balance