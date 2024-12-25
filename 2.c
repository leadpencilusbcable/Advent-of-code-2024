#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "file.h"

#define ARRAY_SIZE 1000

int string_to_int(char* start, int length){
  int res = start[0] - 48;

  for(int i = 1; i < length; i++){
    res *= 10;
    res += start[i] - 48;
  }

  return res;
}

int number_of_safe_reports(struct File file){
  int count = 0;
  int start_of_num = 0;
  int end_of_num = 0;

  int report_num = 0;
  int level_num = 0;
  bool increasing = true;
  int safe = true;

  int prev_num;

  for(int i = 0; i < file.size; i++){
    if(file.data[i] == 32 && safe){
      int cur_num = string_to_int(&file.data[start_of_num], end_of_num - start_of_num);

      if(level_num == 1){
        increasing = cur_num > prev_num;
      }

      if(level_num > 0){
        int diff = cur_num - prev_num;

        if(increasing && (diff < 1 || diff > 3)){
          safe = false;
        } else if(!increasing && (diff < -3 || diff > -1)){
          safe = false;
        }
      }

      prev_num = cur_num;

      start_of_num = i + 1;
      end_of_num = i + 1;

      level_num++;
    } else if(file.data[i] == 10){
      int cur_num = string_to_int(&file.data[start_of_num], end_of_num - start_of_num);
      int diff = cur_num - prev_num;

      if(increasing && (diff < 1 || diff > 3)){
        safe = false;
      } else if(!increasing && (diff < -3 || diff > -1)){
        safe = false;
      }

      if(safe){
        count++;
      }

      start_of_num = i + 1;
      end_of_num = i + 1;

      report_num++;
      safe = true;
      level_num = 0;
    } else {
      end_of_num++;
    }
  }

  return count;
}

int main(){
  struct File file;
  bool success = read_file("./input2", &file);

  if(!success){
    printf("file not read successfully. exiting...");
  }

  printf("safe report count: %d", number_of_safe_reports(file));
  free(file.data);

  return 0;
}

