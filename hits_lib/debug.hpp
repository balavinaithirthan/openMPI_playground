#pragma once
#include "hit.hpp"
#include <iostream>
#include <map>
namespace hits_lib {
inline void debug_hits(std::vector<hit_lib::Hit> hits) {
  for (double i = 0; i < hits.size(); i++) {
    std::cout << "Hit " << i << " is at position " << hits[i].getX() << ", "
              << hits[i].getY() << " with length " << hits[i].length
              << std::endl;
  }
}

inline void debug_filter_order(std::vector<std::tuple<int, int>> filter_order) {
  for (int i = 0; i < filter_order.size(); i++) {
    printf("start: %d, end: %d\n", std::get<0>(filter_order[i]),
           std::get<1>(filter_order[i]));
  }
}

inline void
debug_seed_hits(std::map<std::string, std::vector<size_t>> seedsOneMap,
                std::map<std::string, std::vector<size_t>> seedsTwoMap) {
  for (auto const &pair : seedsOneMap) {
    std::cout << "Key: " << pair.first << std::endl;
    for (auto const &val : pair.second) {
      std::cout << "Value: " << val << std::endl;
    }
  }
  for (auto const &pair : seedsTwoMap) {
    std::cout << "Key: " << pair.first << std::endl;
    for (auto const &val : pair.second) {
      std::cout << "Value: " << val << std::endl;
    }
  }
}
inline void debug_vector(const std::vector<hits_lib::Hit> &vec) {
  printf("[");
  for (int i = 0; i < vec.size(); i++) {
    printf("%d", vec[i].on);
    if (i != vec.size() - 1) {
      printf(", ");
    }
  }
  printf("]\n");
}

inline void debug_indices(std::vector<std::tuple<int, int>> indices) {
  for (int i = 0; i < indices.size(); i++) {
    printf("start: %d, end: %d\n", std::get<0>(indices[i]),
           std::get<1>(indices[i]));
  }
}

inline void print_vector(const std::vector<hits_lib::Hit> &vec) {
  printf("[");
  for (int i = 0; i < vec.size(); i++) {
    printf("%d", vec[i].on);
    if (i != vec.size() - 1) {
      printf(", ");
    }
  }
  printf("]\n");
}

inline void print_filter_list(
    std::vector<std::function<void(std::vector<hits_lib::Hit> &, int)>>
        filter_list) {
  for (int i = 0; i < filter_list.size(); i++) {
    if (!filter_list[i]) {
      std::cerr
          << "Error: encountefewiofred an uninitialized function in filter_list"
          << std::endl;
      continue;
    }
    printf("filter %d\n", i);
    auto vec = std::vector<hits_lib::Hit>{hits_lib::Hit(), hits_lib::Hit(),
                                          hits_lib::Hit()};
    filter_list[i](vec, 0);
  }
}

inline void print_filter_order(std::vector<std::tuple<int, int>> filter_order) {
  for (const auto &filter : filter_order) {
    std::cout << "filter slice is " << std::get<0>(filter) << " "
              << std::get<1>(filter) << std::endl;
  }
}

inline void debug_hits(std::vector<hits_lib::Hit> hits) {
  for (double i = 0; i < hits.size(); i++) {
    std::cout << "Hit " << i << " is at position " << hits[i].getX() << ", "
              << hits[i].getY() << " with length " << hits[i].length
              << std::endl;
  }
}
} // namespace hits_lib