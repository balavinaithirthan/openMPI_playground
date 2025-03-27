#include "anchor.hpp"

namespace filters {

void anchorCompute(std::vector<hits_lib::Hit> &hits, const std::string &query,
                   const std::string &reference) {
  for (auto &hit : hits) {
    if (hit.on) {
      anchorFilter(hit, query, reference);
    }
  }
}

void anchorFilter(hits_lib::Hit &hit, const std::string &query,
                  const std::string &reference) {
  // Compute midpoint
  // printf("anchorFilter\n");
  auto midpoint_x = hit.getX() + hit.length / 2;
  auto midpoint_y = hit.getY() + hit.length / 2;

  // Set anchor to midpoint and reduce length to 1
  hit.position_x = midpoint_x;
  hit.position_y = midpoint_y;
  hit.length = 1;
}

} // namespace filters
