#include "file.h"
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef char dir_flag;

#define U 0b0001
#define D 0b0010
#define L 0b0100
#define R 0b1000

dir_flag get_possible_directions(int line_length, int line_count, int current_pos, int word_length){
  int current_vertical_offset = current_pos / (line_length + 1);
  int current_horizontal_offset = current_pos % (line_length + 1);

  dir_flag possible_directions = 0;

  if(current_vertical_offset >= word_length - 1){
    possible_directions = U;
  }
  if(current_vertical_offset <= line_count - word_length + 1){
    possible_directions |= D;
  }
  if(current_horizontal_offset >= word_length - 1){
    possible_directions |= L;
  }
  if(current_horizontal_offset <= line_length - word_length + 1){
    possible_directions |= R;
  }

  return possible_directions;
}

/*
 * generalised function to count occurences of two MAS in the shape of an X
*/
int count_mas(char* input, int input_length){
  int count = 0;

  int line_length = 0;
  char* iter = input;

  while(*iter != '\n'){
    if(*iter == '\0'){
      return 0;
    }

    iter++;
    line_length++;
  }

  int line_count = input_length / line_length;

  for(int i = 0; i < input_length; i++){
    if(input[i] == 'A'){
      int current_vertical_offset = i / (line_length + 1);
      int current_horizontal_offset = i % (line_length + 1);

      dir_flag dir = 0;

      if(current_vertical_offset >= 1){
        dir = U;
      }
      if(current_vertical_offset <= line_count - 2){
        dir |= D;
      }
      if(current_horizontal_offset >= 1){
        dir |= L;
      }
      if(current_horizontal_offset <= line_length - 2){
        dir |= R;
      }

      if(dir & U && dir & D && dir & L && dir & R){
        int top_left = i - line_length - 2;
        int top_right = i - line_length;
 
        int bottom_left = i + line_length; 
        int bottom_right = i + line_length + 2;

        int mas_count = 0;

        if(input[top_left] == 'M' && input[bottom_right] == 'S'){
           mas_count++; 
        }
        if(input[top_right] == 'M' && input[bottom_left] == 'S'){
           mas_count++; 
        }
        if(input[bottom_left] == 'M' && input[top_right] == 'S'){
           mas_count++; 
        }
        if(input[bottom_right] == 'M' && input[top_left] == 'S'){
           mas_count++; 
        }

        if(mas_count == 2){
          count++;
        }
      }
    }
  }

  return count;
}

void test(){
  char* in = "MMMSXXMASM\nMSAMXMSMSA\nAMXSXMAAMM\nMSAMASMSMX\nXMASAMXAMM\nXXAMMXXAMA\nSMSMSASXSS\nSAXAMASAAA\nMAMMMXMMMM\nMXMXAXMASX";
  int input_length = 109;

  assert(count_mas(in, input_length) == 9);
}

int main(int argc, char** argv){
  if(argc > 1 && strcmp(argv[1], "test") == 0){
    test();
    return 0;
  }

  struct File file;
  bool success = read_file("./input4", &file);

  if(!success){
    puts("failed to read file");
    return -1;
  }

  int count = count_mas(file.data, file.size);
  printf("count: %d\n", count);

  return 0;
}
