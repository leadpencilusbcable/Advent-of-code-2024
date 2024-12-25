#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "test.h"

uint64_t calculate_checksum(char* in, uint64_t size){
  uint64_t checksum = 0;

  bool even_size = size % 2 == 0;

  uint64_t l = 0;
  uint64_t l_id = 0;

  uint64_t r = even_size ? size - 2 : size - 1;
  uint64_t r_id = r / 2;
  uint64_t r_remaining = in[r] - 48;

  bool l_empty = false;
  uint64_t index = 0;

  uint64_t final_ind = -1;

  for(uint64_t i = 0; i < (even_size ? size - 1 : size); i += 2){
    final_ind += in[i] - 48;
  }

  while(true){
    if(l_empty){
      uint64_t fill_to = index + (in[l] - 48);

      while(index < fill_to){
        checksum += index * r_id;
        index++;

        if(index > final_ind){
          return checksum;
        }

        r_remaining--;

        if(r_remaining == 0){
          r -= 2;
          r_id--;
          r_remaining = in[r] - 48;
        }
      }

      l_empty = false;
    } else {
      uint64_t fill_to = index + (in[l] - 48);
      while(index < fill_to){
        checksum += index * l_id;
        index++;
        if(index > final_ind){
          return checksum;
        }
      }

      l_empty = true;
      l_id++;
    }

    l++;
  }

  return checksum;
}

void test(){
  char* in = "12345";
  expect_uint64_t(calculate_checksum(in, 5), 60, "test checksum");

  char* in2 = "2333133121414131402";
  expect_uint64_t(calculate_checksum(in2, 19), 1928, "test checksum");
}

int main(int argc, char** argv){
  if(argc > 1 && strcmp(argv[1], "test") == 0){
    test();
    return 0;
  }

  struct File file;
  bool success = read_file("./input9", &file);

  if(!success){
    puts("error reading file. exiting...");
  }

  printf("checksum: %lu\n", calculate_checksum(file.data, 20000));

  free(file.data);

  return 0;
}

