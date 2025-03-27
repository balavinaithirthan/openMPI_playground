#pragma once
#include <cstddef>
#include <tuple>
#include <vector>

namespace hits_lib {

struct Hit {
  bool on;
  double length;
  size_t position_x;
  size_t position_y;

  Hit() = default;

  Hit(std::tuple<size_t, size_t> position, double length)
      : position_x(std::get<0>(position)), position_y(std::get<1>(position)),
        length(length), on(true) {}

  double getX() const { return position_x; }
  double getY() const { return position_y; }
  void setPosition(size_t x1, size_t y1) {
    position_x = x1;
    position_y = y1;
  }
};

} // namespace hits_lib
