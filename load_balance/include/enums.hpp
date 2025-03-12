#pragma once

#define DEBUG_OUTPUT 1
// #define DEBUG_INPUT 1
// #define DEBUG_PARENT 1
// #define DEBUG_CHILD 1

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