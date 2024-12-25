#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "test.h"

bool expect_int(int value, int expected, char* desc){
  if(value != expected){
    printf(RED("TEST FAILED (%s): expected %d, got %d.\n") RESET, desc, expected, value);
    return false;
  }

  printf(GRN("TEST PASSED (%s): expected %d, got %d.\n") RESET, desc, expected, value);

  return true;
}

bool expect_uint64_t(uint64_t value, uint64_t expected, char* desc){
  if(value != expected){
    printf(RED("TEST FAILED (%s): expected %lu, got %lu.\n") RESET, desc, expected, value);
    return false;
  }

  printf(GRN("TEST PASSED (%s): expected %lu, got %lu.\n") RESET, desc, expected, value);

  return true;
}

