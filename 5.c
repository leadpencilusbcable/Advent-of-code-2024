#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

struct Rules {
  bool before[90];
  bool after[90];
};

char* parse_rules(char* data, struct Rules* output_rules){
  char* iter = data;

  while(*iter != '\n'){
    int before = (iter[0] - 48) * 10 + (iter[1] - 48);
    int after = (iter[3] - 48) * 10 + (iter[4] - 48);

    output_rules[before - 10].before[after - 10] = true;
    output_rules[after - 10].after[before - 10] = true;

    iter += 6;
  }

  return iter + 1;
}

int middle_page_numbers_incorrect_ordered(char* data, struct Rules* rules){
  int count = 0;

  char* iter = data;
  int numbers_in_cur_line = 0;
  bool line_fail = false;
  int line[23];

  while(iter[1] != '\0'){
    int cur_num = (iter[0] - 48) * 10 + (iter[1] - 48);
    line[numbers_in_cur_line] = cur_num;
    int cur_num_pos = numbers_in_cur_line;

    for(int i = numbers_in_cur_line - 1; i >= 0; i--){
      int prev_num = line[i];

      if(!rules[prev_num - 10].before[cur_num - 10]){
        line_fail = true;

        int temp = line[i];
        line[i] = cur_num;
        line[cur_num_pos] = temp;

        cur_num_pos--;
      }
    }

    numbers_in_cur_line++;

    if(iter[2] == '\n'){
      if(line_fail){
        count += line[numbers_in_cur_line / 2];
      }

      numbers_in_cur_line = 0;
      line_fail = false;
    }

    iter += 3;
  }

  return count;
}

int middle_page_numbers(char* data, struct Rules* rules){
  int count = 0;

  char* iter = data;
  int numbers_in_cur_line = 0;
  bool line_fail = false;
  int line[23];

  while(iter[1] != '\0'){
    if(line_fail){
      if(iter[2] == '\n'){
        numbers_in_cur_line = 0;
        line_fail = false;
      }

      iter += 3;
      continue;
    }

    int cur_num = (iter[0] - 48) * 10 + (iter[1] - 48);
    line[numbers_in_cur_line] = cur_num;

    for(int i = 0; i < numbers_in_cur_line; i++){
      int prev_num = line[i];

      if(!rules[prev_num - 10].before[cur_num - 10]){
        line_fail = true;
      }
    }

    numbers_in_cur_line++;

    if(iter[2] == '\n'){
      if(!line_fail){
        count += line[numbers_in_cur_line / 2];
      }

      numbers_in_cur_line = 0;
    }

    iter += 3;
  }

  return count;
}

void test(){
  char* data = "47|53\n97|13\n97|61\n97|47\n75|29\n61|13\n75|53\n29|13\n97|29\n53|29\n61|53\n97|53\n61|29\n47|13\n75|47\n97|75\n47|61\n75|61\n47|29\n75|13\n53|13\n\n75,47,61,53,29\n97,61,53,29,13\n75,29,13\n75,97,47,61,53\n61,13,29\n97,13,75,29,47\n\n";

  struct Rules rules[90] = {
    {{0}},
    {{0}}
  };
  
  data = parse_rules(data, &rules[0]);
  int count = middle_page_numbers_incorrect_ordered(data, &rules[0]);
  
  assert(count == 123);
}

int main(int argc, char** argv){
  if(argc > 1 && strcmp(argv[1], "test") == 0){
    test();
    return 0;
  }

  struct File file;
  bool success = read_file("./input5", &file); 

  if(!success){
    puts("error reading file. exiting...");
  }

  struct Rules rules[90] = {
    {{0}},
    {{0}}
  };
  
  char* data = parse_rules(file.data, &rules[0]);
  int count = middle_page_numbers(data, &rules[0]);
  int count2 = middle_page_numbers_incorrect_ordered(data, &rules[0]);

  printf("count: %d\n", count);
  printf("count 2: %d\n", count2);

  free(file.data);
  
  return 0;
}

