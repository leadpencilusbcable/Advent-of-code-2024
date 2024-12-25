#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

void bubble_sort(int* list, int size){
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size - i - 1; j++){
      if(list[j] > list[j + 1]){
        int temp = list[j];

        list[j] = list[j + 1];
        list[j + 1] = temp;
      }
    }
  }
}

int total_distance(int* list1, int* list2, int size){
  int distance = 0;
  bubble_sort(list1, size);
  bubble_sort(list2, size);

  for(int i = 0; i < size; i++){
    distance += abs(list1[i] - list2[i]);
  }

  return distance;
}

int total_similarity(int* list1, int* list2, int size){
  int similarity = 0;

  for(int i = 0; i < size; i++){
    int num = list1[i];
    int total = 0;

    for(int j = 0; j < size; j++){
      if(num == list2[j])
        total++;
    }

    similarity += num * total;
  }

  return similarity;
}

int parse_input(int* list1, int* list2){
  FILE* file;
  file = fopen("input", "r");

  if(file == NULL)
    return -1;

  int file_size;

  fseek(file, 0L, SEEK_END);
  file_size = ftell(file);
  rewind(file);

  char* data = malloc(file_size);

  fread(data, sizeof(char), file_size, file);
  fclose(file);

  bool islist1 = true;
  int cur_ind = 0;
  int cur_pow = 4;
  int cur_num = 0;

  for(int i = 0; i < file_size; i++){
    if(data[i] < 48 || data[i] > 57){
      continue;
    }

    cur_num += (data[i] - 48) * pow(10, cur_pow);
  
    if(cur_pow == 0){
      if(islist1){
        list1[cur_ind] = cur_num; 
        islist1 = false;
      } else{
        list2[cur_ind] = cur_num; 
        islist1 = true;
        cur_ind++;
      }

      cur_pow = 4;
      cur_num = 0;
    } else{
      cur_pow--;
    }
  }

  free(data);

  return 0;
}

int main(){
  int size = 1000;

  int list1[1000];
  int list2[1000];

  parse_input(&list1[0], &list2[0]);

  int similarity = total_similarity(list1, list2, size);
  printf("similarity %d\n", similarity);

  int distance = total_distance(list1, list2, size);
  printf("distance %d\n", distance);
}

