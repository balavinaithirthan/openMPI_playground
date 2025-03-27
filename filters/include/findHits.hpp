#include "hit.hpp"
#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <vector>
namespace filters {
std::vector<hits_lib::Hit>
findHits(std::map<std::string, std::vector<size_t>> a,
         std::map<std::string, std::vector<size_t>> b, double c);
} // namespace filters