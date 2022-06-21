#pragma once
#include "defines.h"

// clang-format off
  #define B    TRANSPARENT
  #define A    RED
  unsigned char car_data_straight[14 * 7 * 4] = {
      B, B, B, A, A, A, A, A, A, A, A, B, B, B,
      B, B, B, B, B, B, A, A, B, B, B, B, B, B,
      B, B, B, B, B, A, A, A, A, B, B, B, B, B,
      B, B, B, B, B, A, A, A, A, B, B, B, B, B,
      A, A, A, B, B, A, A, A, A, B, B, A, A, A,
      A, A, A, A, A, A, A, A, A, A, A, A, A, A,
      A, A, A, B, B, A, A, A, A, B, B, A, A, A,
  };

  unsigned char car_data_right[14 * 7 * 4] = {
      B, B, B, B, B, A, A, A, A, A, A, A, A, B,
      B, B, B, B, B, B, B, B, A, A, B, B, B, B,
      B, B, B, B, B, B, A, A, A, B, B, B, B, B,
      B, B, B, B, B, A, A, A, A, B, B, B, B, B,
      B, A, A, B, B, A, A, A, A, B, B, B, A, A,
      A, A, A, A, A, A, A, A, A, A, A, A, A, A,
      A, A, B, B, B, A, A, A, A, B, B, A, A, B,
  };

  unsigned char car_data_left[14 * 7 * 4] = {
      B, A, A, A, A, A, A, A, A, B, B, B, B, B,
      B, B, B, B, A, A, B, B, B, B, B, B, B, B,
      B, B, B, B, B, A, A, A, B, B, B, B, B, B,
      B, B, B, B, B, A, A, A, A, B, B, B, B, B,
      A, A, B, B, B, A, A, A, A, B, B, A, A, B,
      A, A, A, A, A, A, A, A, A, A, A, A, A, A,
      B, A, A, B, B, B, A, A, A, B, B, B, A, A,
  };
  // clang-format on
