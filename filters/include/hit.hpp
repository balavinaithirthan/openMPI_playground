#pragma once
#include <cstddef>
#include <tuple>
#include <vector>

namespace filters {

class Hit {
public:
  Hit() = default;
  Hit(std::tuple<size_t, size_t> position, double length)
      : position(position), length(length) {
    on = true;
  }

  double getX() const { return std::get<0>(position); }
  double getY() const { return std::get<1>(position); }
  void setPosition(size_t x1, size_t y1) {
    position = std::tuple<size_t, size_t>(x1, y1);
  };
  bool on;
  double length;
  std::tuple<size_t, size_t> position;

  void *metadata;
  int metadata_nobytes;
};
} // namespace filters