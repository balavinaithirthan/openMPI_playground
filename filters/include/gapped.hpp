#include "hit.hpp"
#include <string>
#include <vector>

namespace filters {
void gappedCompute(std::vector<filters::Hit> &hits, const std::string &query,
                   const std::string &reference);
void gappedFilter(Hit &hit, const std::string &query,
                  const std::string &reference);

} // namespace filters