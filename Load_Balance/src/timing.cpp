#include "timing.hpp"

namespace load_balance {
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<long long, std::ratio<1, 1000000000>>> time_start() {
#if TIMING == 1
  auto start = std::chrono::high_resolution_clock::now();
  return start;
#endif
}

void time_end(std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<long long, std::ratio<1, 1000000000>>> start,
int rank) {
#if TIMING == 1
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;
  if (rank == 0) {
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
  }
#endif
}
} // namespace load_balance