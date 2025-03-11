#include "findHits.hpp"

namespace filters {

std::vector<Hit> findHits(std::map<std::string, std::vector<size_t>> a,
                          std::map<std::string, std::vector<size_t>> b,
                          double n) {
  std::vector<Hit> hits;
  for (const auto &entry : a) {
    const std::string &seed = entry.first;
    if (b.find(seed) != b.end()) { // if seed exists in b
      const std::vector<size_t> positions_a = entry.second;
      const std::vector<size_t> positions_b = b.at(seed);
      for (const auto &pos_1 : positions_a) {
        for (const auto &pos_2 : positions_b) {
          // TODO: remove duplicates??
          auto position = std::tuple<size_t, size_t>{
              pos_1, pos_2}; // auto move bc temp value
          hits.emplace_back(position, n);
        }
      }
    }
  }
  return hits;
}
} // namespace filters