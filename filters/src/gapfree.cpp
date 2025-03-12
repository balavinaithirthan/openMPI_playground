#include "gapped.hpp"

// Todo, masking
namespace filters {

void gapFreeCompute(std::vector<filters::Hit> &hits, const std::string &query,
                    const std::string &reference) {
  for (auto &hit : hits) {
    if (hit.on == true) {
      gappedFilter(hit, query, reference);
    }
  }
}

// TODO: how bad is this for loop compared to checking if true

void gapFreeCompute(Hit &hit, const std::string &query,
                    const std::string &reference) {
  auto x1 = hit.getX();
  auto y1 = hit.getY();
  auto x2 = hit.getX() + hit.length;
  auto y2 = hit.getY() + hit.length;

  // right side
  auto i = y2;
  auto j = x2;
  while (i < query.size() && j < reference.size() &&
         query.at(i) == reference.at(j)) {
    hit.length += 1;
    i += 1;
    j += 1;
  }

  if (hit.length < 10) {
    hit.on = false;
    return;
  }

  i = y1;
  j = x1;
  // left side
  while (i > 0 && j > 0 && query.at(i) == reference.at(j)) {
    hit.length += 1;
    i -= 1;
    j -= 1;
    hit.position_x -= 1;
    hit.position_y -= 1;
  }
  if (hit.length < 10) {
    hit.on = false;
    return;
  }
}
} // namespace filters