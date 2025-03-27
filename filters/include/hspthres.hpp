#include "hit.hpp"
#include <string>
#include <vector>

namespace filters {

void hspthreshCompute(std::vector<hits_lib::Hit> &hits,
                      const std::string &query, const std::string &reference);

void hspthreshFilter(hits_lib::Hit &hit, const std::string &query,
                     const std::string &reference);

} // namespace filters