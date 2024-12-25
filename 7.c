#include "file.h"
#include "test.h"
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

uint64_t concat(uint64_t x, uint64_t y){
  int y_digits = 0;
  int y_temp = y;

  while(y_temp > 0){
    y_temp /= 10;
    y_digits++;
  }

  uint64_t res = x * pow(10, y_digits); 
  res += y;

  return res;
}

int offset = 0;

void generate_combo(char* combinations, int number_of_positions, char* chars, int chars_size, char* str, int length){
  if(length == number_of_positions){
    for(int i = 0; i < number_of_positions; i++){
      combinations[offset] = str[i];
      offset++;
    }

    return;
  }

  for(int i = 0; i < chars_size; i++){
    str[length] = chars[i];
    generate_combo(combinations, number_of_positions, chars, chars_size, str, length + 1);
  }
}

char* unique_combinations_of_chars(int* size, int number_of_positions, char* chars, int chars_size){
  int total_combinations = pow(chars_size, number_of_positions);
  *size = total_combinations * number_of_positions * sizeof(char);
  char* combinations = malloc(*size);
  
  char* str = malloc(number_of_positions + 1);
  str[number_of_positions] = 0;

  generate_combo(combinations, number_of_positions, chars, chars_size, str, 0);
  offset = 0;
  free(str);

  return combinations;
}

uint64_t get_line_result(char** itr){
  uint64_t res = 0;
  
  while(**itr != ':'){
    res *= 10;
    res += **itr - 48;
    (*itr)++;
  }

  return res;
}

char operators[3] = { '+', '*', '|' };

uint64_t calibrate(char* in, int size){
  uint64_t total = 0;
  char* itr = in;

  while(itr - in < size - 1){
    uint64_t line_res = get_line_result(&itr);
    uint64_t values[15] = { 0 };
    int line_value_count = 0;

    itr += 2;

    while(*itr != '\n'){

      while(*itr != ' ' && *itr != '\n'){
        values[line_value_count] *= 10;
        values[line_value_count] += *itr - 48;
        itr++;
      }

      if(*itr == ' '){
        itr++;
      }
      line_value_count++;
    }

    itr++;

    int size;
    char* operator_combinations = unique_combinations_of_chars(&size, line_value_count - 1, operators, 3);

    for(int i = 0; i < size; i += (line_value_count - 1)){
      uint64_t res = values[0];

      for(int j = 1; j < line_value_count; j++){
        if(operator_combinations[i + j - 1] == '*'){
          res *= values[j]; 
        } else if(operator_combinations[i + j - 1] == '+'){
          res += values[j]; 
        } else {
          res = concat(res, values[j]);
        }
      }

      if(res == line_res){
        total += res;
        break;
      }
    }
  }

  return total;
}

void test(){
  char* in = "190: 10 19\n3267: 81 40 27\n83: 17 5\n156: 15 6\n7290: 6 8 6 15\n161011: 16 10 13\n192: 17 8 14\n21037: 9 7 18 13\n292: 11 6 16 20\n";
  expect_int(calibrate(in,123), 3749, "calibration result matches test");

  int size;

  char* chars = unique_combinations_of_chars(&size, 2, "ab", 2);
  expect_int(size, 8, "total of 8 characters for chars ab of len 2");

  free(chars);

  chars = unique_combinations_of_chars(&size, 2, "abc", 3);
  expect_int(size, 18, "total of 18 characters for chars abc of len 2");

  free(chars);

  expect_uint64_t(concat(10, 15), 1015, "concat correct 1");
  expect_uint64_t(concat(15, 6), 156, "concat correct 2");
  expect_uint64_t(concat(48, 676), 48676, "concat correct 3");
}

int main(int argc, char** argv){
  if(argc > 1 && strcmp(argv[1], "test") == 0){
    test();
    return 0;
  }

  struct File file;
  bool success = read_file("./input7", &file);

  if(!success){
    puts("error reading file. exiting...");
    return -1;
  }

  printf("%lu\n", calibrate(file.data, file.size));
  free(file.data);

  return 0;
}

