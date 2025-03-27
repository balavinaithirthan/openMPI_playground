#include "hit.hpp"
#include <string>
#include <vector>
namespace filters {

void gapFreeCompute(std::vector<hits_lib::Hit> &hits, const std::string &query,
                    const std::string &reference);

void gapFreeCompute(hits_lib::Hit &hit, const std::string &query,
                    const std::string &reference);

} // namespace filters