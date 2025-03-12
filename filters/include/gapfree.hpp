#include "constants.hpp"
#include "hit.hpp"
#include <string>
#include <vector>
namespace filters {

void gapFreeCompute(std::vector<filters::Hit> &hits, const std::string &query,
                    const std::string &reference);

void gapFreeCompute(Hit &hit, const std::string &query,
                    const std::string &reference);

} // namespace filters