#pragma once

#define DEBUG 0
// #define SUPER_DEBUG 1
// #define TIMING 1
// #define FINAL 1

namespace load_balance {
enum {
  RANK_0 = 0,
};

enum {
  VEC_SIZE = 0,
  VEC_DATA = 1,
};

enum {
  EQUAL_CHUNKING = 0,
  LOAD_BALANCED = 1,
};
} // namespace load_balance