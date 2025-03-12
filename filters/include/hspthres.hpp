#include "hit.hpp"
#include <string>
#include <vector>

namespace filters {

void hspthreshCompute(std::vector<filters::Hit> &hits, const std::string &query,
                      const std::string &reference);

void hspthreshFilter(Hit &hit, const std::string &query,
                     const std::string &reference);

} // namespace filters