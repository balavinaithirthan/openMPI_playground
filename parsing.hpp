// #pragma once
// #include "filters.hpp"
// #include <fstream>
// #include <sstream>
// #include <string>
// #include <vector>

// inline std::vector<hits_lib::Hit> parseHits(const std::string &file_path) {
//   std::vector<hits_lib::Hit> hits;
//   std::ifstream file(file_path);
//   if (!file) {
//     std::cerr << "Error opening file: " << file_path << std::endl;
//     return hits;
//   }

//   std::string line;
//   std::getline(file, line); // Skip header
//   while (std::getline(file, line)) {
//     std::istringstream ss(line);
//     std::string name1, name2, strand2;
//     size_t start1, end1, start2, end2;
//     double score;

//     ss >> name1 >> start1 >> end1 >> name2 >> start2 >> end2 >> strand2 >>
//         score;

//     size_t position_x = (start1 + end1) / 2;
//     size_t position_y = (start2 + end2) / 2;
//     double length = end1 - start1;

//     hits.emplace_back(std::make_tuple(position_x, position_y), length);
//   }

//   return hits;
// }