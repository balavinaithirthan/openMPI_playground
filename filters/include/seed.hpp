#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace filters {
std::map<std::string, std::vector<size_t>> findSeeds(const std::string &input,
                                                     double n);
} // namespace filters