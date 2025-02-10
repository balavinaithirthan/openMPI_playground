#include "timing.hpp"

namespace load_balance {


std::chrono::time_point<std::chrono::steady_clock> time_start() {
#if TIMING == 1
    return Clock::now();
#else
    return std::chrono::time_point<std::chrono::steady_clock>{}; // Default-initialized
#endif
}

void time_end(std::chrono::time_point<std::chrono::steady_clock> start, int rank) {
#if TIMING == 1
    auto end = Clock::now();
    std::chrono::duration<double> duration = end - start;
    if (rank == 0) {
        std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
    }
#endif
}

} // namespace load_balance
