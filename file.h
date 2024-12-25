#pragma once

#include <stdbool.h>

struct File {
  char* data;
  int size;
};

bool read_file(const char* path, struct File* file);

