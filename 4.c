#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "file.h"

typedef char dir_flag;

#define U 0b0001
#define D 0b0010
#define L 0b0100
#define R 0b1000

int check_directions(
    char* in,
    int current_pos,
    dir_flag possible_directions,
    char* word,
    int word_length,
    int line_count,
    int line_length
){
  int count = 0;

  if(possible_directions & U){
    int pos = current_pos;
    bool match = true;

    for(int i = 0; i < word_length; i++){
      if(word[i] != in[pos]){
        match = false;
        break;
      }
      
      pos -= line_length + 1;
    }

    count += match;
  }

  if(possible_directions & U && possible_directions & R){
    int pos = current_pos;
    bool match = true;

    for(int i = 0; i < word_length; i++){
      if(word[i] != in[pos]){
        match = false;
        break;
      }
      
      pos -= line_length;
    }

    count += match;
  }

  if(possible_directions & U && possible_directions & L){
    int pos = current_pos;
    bool match = true;

    for(int i = 0; i < word_length; i++){
      if(word[i] != in[pos]){
        match = false;
        break;
      }
      
      pos -= line_length + 2;
    }

    count += match;
  }

  if(possible_directions & D){
    int pos = current_pos;
    bool match = true;

    for(int i = 0; i < word_length; i++){
      if(word[i] != in[pos]){
        match = false;
        break;
      }
      
      pos += line_length + 1;
    }

    count += match;
  }

  if(possible_directions & D && possible_directions & L){
    int pos = current_pos;
    bool match = true;

    for(int i = 0; i < word_length; i++){
      if(word[i] != in[pos]){
        match = false;
        break;
      }
      
      pos += line_length;
    }

    count += match;
  }

  if(possible_directions & D && possible_directions & R){
    int pos = current_pos;
    bool match = true;

    for(int i = 0; i < word_length; i++){
      if(word[i] != in[pos]){
        match = false;
        break;
      }
      
      pos += line_length + 2;
    }

    count += match;
  }

  if(possible_directions & L){
    int pos = current_pos;
    bool match = true;

    for(int i = 0; i < word_length; i++){
      if(word[i] != in[pos]){
        match = false;
        break;
      }
      
      pos--;
    }

    count += match;
  }

  if(possible_directions & R){
    int pos = current_pos;
    bool match = true;

    for(int i = 0; i < word_length; i++){
      if(word[i] != in[pos]){
        match = false;
        break;
      }
      
      pos++;
    }

    count += match;
  }

  return count;
}

/*
 * @param line_length - the length of each line without newline char
 * @param line_count - number of lines
 * @param current_pos - current index inside input string
 * @param word_length - the length of the word to check against
 * @returns - flag indicating possible directions
*/
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
 * @param input - the input string
 * @param input_length - the length of input param
 * @param word - the word to count in all directions
 * @param word_length - the length of word param
 * @returns - number of times word was found in input
*/
int count_word_any_direction(char* input, int input_length, char* word, int word_length){
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
    if(input[i] == *word){
      dir_flag directions = get_possible_directions(line_length, line_count, i, word_length); 

      count += check_directions(
        input,
        i,
        directions,
        word,
        word_length,
        line_count,
        line_length
      );
    }
  }

  return count;
}

void test(){
  char* in = "MMMSXXMASM\nMSAMXMSMSA\nAMXSXMAAMM\nMSAMASMSMX\nXMASAMXAMM\nXXAMMXXAMA\nSMSMSASXSS\nSAXAMASAAA\nMAMMMXMMMM\nMXMXAXMASX";
  int input_length = 109;
  char* word = "XMAS";
  int word_length = 4;

  char* in2 = "AAAASSSS\nABCDAAAA\nAAAAMAAA\nASAMXMAS\nAAAAMAAA\nAAAAAAAA\nAAAASSSS";
  int in2_length = 62;

  dir_flag directions = get_possible_directions(8, 7, 32, word_length);
  printf("U: %d\tD: %d\tL: %d\tR: %d\n", directions & U, directions & D, directions & L, directions & R);
  assert(directions == (U | D | L | R));

  printf("wordz2: %d\n", count_word_any_direction(in2, in2_length, word, word_length));
  printf("wordz: %d\n", count_word_any_direction(in, input_length, word, word_length));
  assert(count_word_any_direction(in, input_length, word, word_length));
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

  int count = count_word_any_direction(file.data, file.size, "XMAS", 4);
  printf("count: %d\n", count);

  return 0;
}

