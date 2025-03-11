#include "seed.hpp"

namespace filters {
std::map<std::string, std::vector<size_t>> findSeeds(const std::string &input,
                                                     double n) {
  std::map<std::string, std::vector<size_t>> seeds;
  // Check if input length is valid
  if (n <= 0 || n > input.size()) {
    std::cerr << "Invalid seed length." << std::endl;
    return seeds;
  }

  // Iterate through the string to extract substrings of length n
  for (size_t i = 0; i <= input.size() - n; ++i) {
    std::string seed = input.substr(i, n);
    seeds[seed].push_back(i);
  }

  return seeds;
}

} // namespace filters