#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "test.h"

enum dir {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  NONE,
};

typedef char dir_flag;

#define U 0b0001
#define D 0b0010
#define L 0b0100
#define R 0b1000

dir_flag dir_to_dir_flag(enum dir dir){
  switch(dir){
    case UP:
      return U;
    case DOWN:
      return D;
    case LEFT:
      return L;
    case RIGHT:
      return R;
    case NONE:
      return 0;
  }
}

void coords_from_ind(int ind, int line_size, int* x, int* y){
  *x = ind % (line_size + 1);
  *y = ind / (line_size + 1);
}

int next_ind(char* in, int size, int cur_pos, enum dir dir, int line_size){
  int next_ind;

  switch(dir){
    case UP:
      next_ind = cur_pos - line_size - 1;
      break;
    case DOWN:
      next_ind = cur_pos + line_size + 1;
      if(next_ind > size - 1) next_ind = -1;
      break;
    case LEFT:
      next_ind = cur_pos - 1;
      if(next_ind < 0 || in[next_ind] == '\n') next_ind = -1;
      break;
    case RIGHT:
      next_ind = cur_pos + 1;
      if(next_ind > size - 1 || in[next_ind] == '\n') next_ind = -1;
      break;
    case NONE:
      next_ind = -1;
      break;
  }

  return next_ind;
}

enum dir next_dir(enum dir dir){
  switch(dir){
    case UP:
      return RIGHT;
    case DOWN:
      return LEFT;
    case LEFT:
      return UP;
    case RIGHT:
      return DOWN;
    case NONE:
      return NONE;
  }
}

int unique_locations_of_obstructions(char* in, int size){
  int locs = 0;
  dir_flag visited[size];

  for(int i = 0; i < size; i++){
    visited[i] = 0;
  }

  int line_size = strchr(in, '\n') - in;
  char* guard_loc = strchr(in, '^');
  int guard_ind = guard_loc - in;

  enum dir dir = UP;
  int i = 0;

  while(guard_ind != -1){
    int next_guard_ind = next_ind(in, size, guard_ind, dir, line_size);

    if(next_guard_ind < 0){
      break;
    }

    enum dir next = next_dir(dir);

    if(in[next_guard_ind] == '.' || in[next_guard_ind] == '^'){
      int next_guard_ind_if_obstacle = next_ind(in, size, guard_ind, next, line_size);
      int x, y;
      coords_from_ind(guard_ind,  line_size, &x, &y);

      enum dir next_if_obstacle = next;

      while(next_guard_ind_if_obstacle >= 0){
        int x1, y1;
        coords_from_ind(next_guard_ind_if_obstacle,  line_size, &x1, &y1);

        if(in[next_guard_ind_if_obstacle] == '#'){
          next = next_dir(next);
          next_guard_ind_if_obstacle = next_ind(in, size, next_guard_ind_if_obstacle, next, line_size);
          continue;
        }

        if(visited[next_guard_ind_if_obstacle] & dir_to_dir_flag(next)){
          locs++;
          break;
        } 
        
        if(next_guard_ind_if_obstacle == guard_ind) break;

        next_guard_ind_if_obstacle = next_ind(in, size, next_guard_ind_if_obstacle, next, line_size);
      }

      visited[guard_ind] |= dir_to_dir_flag(dir);

      guard_ind = next_guard_ind;
    } else if(in[next_guard_ind] == '#'){
      visited[guard_ind] |= dir_to_dir_flag(dir);
      dir = next;
    }
  }

  return locs;
}

int unique_locations(char* in, int size){
  int locs = 1;
  bool visited[size];
  memset(visited, 0, size);

  int line_size = strchr(in, '\n') - in;
  char* guard_loc = strchr(in, '^');
  int guard_ind = guard_loc - in;
  visited[guard_ind] = true;
  enum dir dir = UP;

  while(guard_ind != -1){
    switch (dir) {
      case UP: {
        int next_guard_ind = guard_ind - line_size - 1;

        if(next_guard_ind < 0){
          guard_ind = -1;
          break;
        }

        if(in[next_guard_ind] == '.' || in[next_guard_ind] == '^'){
          guard_ind = next_guard_ind;
          if(!visited[guard_ind]){
            visited[guard_ind] = true;
            locs++;
          }
        } else if(in[next_guard_ind] == '#'){
          dir = RIGHT;
        }

        break;
      }
      case DOWN: {
        int next_guard_ind = guard_ind + line_size + 1;

        if(next_guard_ind > size - 1){
          guard_ind = -1;
          break;
        }

        if(in[next_guard_ind] == '.' || in[next_guard_ind] == '^'){
          guard_ind = next_guard_ind;
          if(!visited[guard_ind]){
            visited[guard_ind] = true;
            locs++;
          }
        } else if(in[next_guard_ind] == '#'){
          dir = LEFT;
        } 

        break;
      }
      case LEFT: {
        int next_guard_ind = guard_ind - 1;

        if(next_guard_ind < 0 || in[next_guard_ind] == '\n'){
          guard_ind = -1;
          break;
        }

        if(in[next_guard_ind] == '.' || in[next_guard_ind] == '^'){
          guard_ind = next_guard_ind;
          if(!visited[guard_ind]){
            visited[guard_ind] = true;
            locs++;
          }
        } else if(in[next_guard_ind] == '#'){
          dir = UP;
        } 

        break;
      }
      case RIGHT: {
        int next_guard_ind = guard_ind + 1;

        if(next_guard_ind > size - 1 || in[next_guard_ind] == '\n'){
          guard_ind = -1;
          break;
        }

        if(in[next_guard_ind] == '.' || in[next_guard_ind] == '^'){
          guard_ind = next_guard_ind;
          if(!visited[guard_ind]){
            visited[guard_ind] = true;
            locs++;
          }
        } else if(in[next_guard_ind] == '#'){
          dir = DOWN;
        } 

        break;
      }
    }
  }

  return locs;
}

void test(){
  char* in = "....#.....\n.........#\n..........\n..#.......\n.......#..\n..........\n.#..^.....\n........#.\n#.........\n......#...\n";

  expect_int(unique_locations(in, 109), 41, "unique locations of test input");

  expect_int(unique_locations_of_obstructions(in, 109), 6, "unique locations of obstructions of test input");
}

int main(int argc, char** argv){
  if(argc > 1 && strcmp(argv[1], "test") == 0){
      test();
      return 0;
  }

  struct File file;
  bool success = read_file("./input6", &file);

  if(!success){
    puts("error reading file. exiting...");
  }

  printf("unique_locations: %d\n", unique_locations(file.data, file.size));
  printf("unique_locations_of_obstructions: %d\n", unique_locations_of_obstructions(file.data, file.size));

  free(file.data);

  return 0;
}

