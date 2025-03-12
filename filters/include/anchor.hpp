#include "hit.hpp"
#include <string>
#include <vector>

namespace filters {
void anchorCompute(std::vector<filters::Hit> &hits, const std::string &query,
                   const std::string &reference);

void anchorFilter(Hit &hit, const std::string &query,
                  const std::string &reference);
} // namespace filters