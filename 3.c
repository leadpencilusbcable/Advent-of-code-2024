#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

bool compare_string(char* s1, char* s2, int len){
  for(int i = 0; i < len; i++){
    if(s1[i] != s2[i]){
      return false;
    }
  }

  return true;
}

/*
 * @param in - text to attempt to convert to int
 * @param end - character that is expected to terminate int
 * @param size - will be modified to amount of digits in the int. 0 if non-int
 * @returns - the int
*/
int extract_int(char* in, char end, int* size){
  int i = 0;

  if(in[0] < 48 || in[0] > 57){
    *size = 0;
    return 0;
  }

  int ret = 0;

  while(true){
    if(in[i] == end || in[i] == 0){
      *size = i; 
      break;
    } else if(in[i] < 48 || in[i] > 57){
      *size = 0;
      return 0;
    }

    ret *= 10;
    ret += in[i] - 48;
      
    i++;
  }

  return ret;
}

/*
 * @param pattern - pattern to extract values from. Only supports %i to extract ints.
 * @param in - text to match against pattern
 * @param values - location to store extracted values in
 * @returns - length of in that complied with pattern. 0 if non-compliant
*/
int extract_values(char* pattern, char* in, int* values){
  int i = 0;
  int cur_pos = 0;
  int cur_val_ind = 0;

  while(in[i]){
    if(pattern[cur_pos] == 37){
      if(pattern[cur_pos + 1] != 105){
        return 0;
      } 

      int size;
      int num = extract_int(&in[i], pattern[cur_pos + 2], &size);

      if(size == 0){
        return 0;
      }

      values[cur_val_ind] = num;

      cur_pos += 2;
      i += size - 1;
      cur_val_ind++;
    } else if(in[i] == pattern[cur_pos]){
      cur_pos++;      
    } 

    if(pattern[cur_pos] == 0){
      puts("here123");
      break;
    }

    i++;
  }

  return i;
}

int calculate_mul(char* input){
  int total = 0;

  char* pattern = "mul(\%i,\%i)";

  int values[2];
  int i = 0;

  bool enabled = true;

  while(input[i]){
    if(!enabled){
      if(input[i] == 100){
        enabled = compare_string(&input[i], "do()", 4);
        i += 4;
        continue;
      } else {
        i++;
        continue;
      }
    }

    if(input[i] == 100){
      enabled = !compare_string(&input[i], "don't()", 7);
      i += 7;
      continue;
    } 

    int size = 0;

    if(input[i] == 109){
      size = extract_values(pattern, &input[i], &values[0]); 
    }

    if(size){
      i += size - 1;
      total += values[0] * values[1];
    } else{
      i++;
    }
  }

  return total;
}

void test(){
  int size;
  int num = extract_int("123456,", 44, &size);

  assert(size == 6); 
  assert(num == 123456); 

  num = extract_int("123456", 44, &size);

  assert(size == 6); 
  assert(num == 123456); 

  num = extract_int("123456{", 44, &size);

  assert(size == 0); 

  int values;
  size = extract_values("abc\%i;", "abc123;", &values);

  assert(size == 7);
  assert(values == 123);

  int values2[2];
  size = extract_values("abc\%i;\%i", "abc123;999", &values2[0]);

  assert(size == 10);
  assert(values2[0] == 123);
  assert(values2[1] == 999);

  size = extract_values("mul(\%i,\%i)", "mul(417,770)", &values2[0]);

  assert(size == 12);
  assert(values2[0] == 417);
  assert(values2[1] == 770);

  size = extract_values("mul(\%i,\%i)", "mul(417,,770)", &values2[0]);

  assert(size == 0);
}

int main(int argc, char** argv){
  if(argc > 1 && strcmp(argv[1], "test") == 0){
    test();
    return 0;
  }

  struct File file;
  bool success = read_file("./input3", &file);

  if(!success){
    puts("Failed to read input3. exiting....");
    return -1;
  }

  int mul = calculate_mul(file.data);
  printf("mul: %d\n", mul);

  free(file.data);

  return 0;
}

