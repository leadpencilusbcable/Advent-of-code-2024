#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "file.h"

bool read_file(const char* path, struct File* file){
  FILE* fileptr;
  fileptr = fopen(path, "r");

  if(fileptr == NULL)
    return 0;

  int file_size;

  fseek(fileptr, 0L, SEEK_END);
  file_size = ftell(fileptr);
  rewind(fileptr);

  char* data = (char*) malloc(file_size);

  fread(data, sizeof(char), file_size, fileptr);
  fclose(fileptr);

  file->data = data;
  file->size = file_size;

  return 1;
}

