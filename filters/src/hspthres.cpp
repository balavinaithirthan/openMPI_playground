#include "hspthres.hpp"
namespace filters {

void hspthreshCompute(std::vector<filters::Hit> &hits, const std::string &query,
                      const std::string &reference) {
  // printf("hspthreshCompute\n");
  int i = 0;
  for (auto &hit : hits) {
    if (i % 2 == 0) {
      // hspthreshFilter(hit, query, reference);
      hit.on = true;
    } else {
      hit.on = false;
    }
    i++;
  }
}

void hspthreshFilter(Hit &hit, const std::string &query,
                     const std::string &reference) {};

} // namespace filters