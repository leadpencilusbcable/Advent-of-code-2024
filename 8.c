#include "file.h"
#include "test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
  int value;
  struct Node* left;
  struct Node* right;
};

struct BST {
  struct Node* root;
  int size;
};

void insert_node(struct BST* bst, struct Node* node, int value){
   if(value < node->value){
    if(node->left == NULL){
      node->left = malloc(sizeof(struct Node));
      node->left->value = value;
      node->left->left = NULL;
      node->left->right = NULL;

      bst->size++;
    } else {
      insert_node(bst, node->left, value);
    }
  } else if(value > node->value){
    if(node->right == NULL){
      node->right = malloc(sizeof(struct Node));
      node->right->value = value;
      node->right->left = NULL;
      node->right->right = NULL;

      bst->size++;
    } else {
      insert_node(bst, node->right, value);
    }
  }
}

void insert(struct BST* bst, int value){
  if(bst->root == NULL){
    bst->root = malloc(sizeof(struct Node));
    bst->root->value = value;
    bst->root->left = NULL;
    bst->root->right = NULL;

    bst->size++;
  } else {
    insert_node(bst, bst->root, value);
  }
}

bool search_node(struct Node* node, int value){
  if(node == NULL){
    return false;
  }

  if(node->value == value){
    return true;
  } else if(value < node->value){
    return search_node(node->left, value);
  } else{
    return search_node(node->right, value);
  }
}

bool search(struct BST* bst, int value){
  return search_node(bst->root, value);
}

struct Array {
  int* data;
  int size;
  int capacity;
};

void push(struct Array* arr, int value){
  if(arr->data == NULL){
    arr->data = malloc(10 * sizeof(int));
    arr->size = 1;
    arr->capacity = 10;
    arr->data[0] = value;
  } else if(arr->size == arr->capacity){
    arr->capacity += 10;
    arr->data = realloc(arr->data, arr->capacity);
    arr->data[arr->size] = value;
    arr->size++;
  } else {
    arr->data[arr->size] = value;
    arr->size++;
  }
}

int count_antinodes_2(char* in, int size){
  struct Array chars[255] = { 0 };
  struct BST antinodes = { 0 };

  int line_length = 0;
  int total_lines = 0;

  for(int i = 0; i < size; i++){
    if(in[i] == '.'){
      continue;
    } else if(in[i] == '\n'){
      if(line_length == 0){
        line_length = i;
        total_lines = size / line_length - 1;
      }

      continue;
    }

    struct Array* same_char = &chars[in[i]];

    if(same_char->data != NULL){
      int x1 = i % (line_length + 1);
      int y1 = i / (line_length + 1);

      for(int j = 0; j < same_char->size; j++){
        int ind = same_char->data[j]; 

        int x2 = ind % (line_length + 1);
        int y2 = ind / (line_length + 1);

        int x_dist = x2 - x1;
        int y_dist = y2 - y1;

        int antinode1_x = x1; 
        int antinode1_y = y1; 

        while(antinode1_x >= 0 && antinode1_x < line_length && antinode1_y >= 0 && antinode1_y < total_lines){
          int antinode_ind = antinode1_y * (line_length + 1) + antinode1_x;
          insert(&antinodes, antinode_ind);

          antinode1_x -= x_dist;
          antinode1_y -= y_dist;
        }

        int antinode2_x = x2; 
        int antinode2_y = y2; 

        while(antinode2_x >= 0 && antinode2_x < line_length && antinode2_y >= 0 && antinode2_y < total_lines){
          int antinode_ind = antinode2_y * (line_length + 1) + antinode2_x;
          insert(&antinodes, antinode_ind);

          antinode2_x += x_dist;
          antinode2_y += y_dist;
        }
      }
    }

    push(same_char, i);
  }

  return antinodes.size;
}

int count_antinodes(char* in, int size){
  struct Array chars[255] = { 0 };
  struct BST antinodes = { 0 };

  int line_length = 0;
  int total_lines = 0;

  for(int i = 0; i < size; i++){
    if(in[i] == '.'){
      continue;
    } else if(in[i] == '\n'){
      if(line_length == 0){
        line_length = i;
        total_lines = size / line_length - 1;
      }

      continue;
    }

    struct Array* same_char = &chars[in[i]];

    if(same_char->data != NULL){
      int x1 = i % (line_length + 1);
      int y1 = i / (line_length + 1);

      for(int j = 0; j < same_char->size; j++){
        int ind = same_char->data[j]; 

        int x2 = ind % (line_length + 1);
        int y2 = ind / (line_length + 1);

        int x_dist = x2 - x1;
        int y_dist = y2 - y1;

        int antinode1_x = x1 - x_dist; 
        int antinode1_y = y1 - y_dist; 

        int antinode2_x = x2 + x_dist; 
        int antinode2_y = y2 + y_dist; 

        if(antinode1_x >= 0 && antinode1_x < line_length && antinode1_y >= 0 && antinode1_y < total_lines){
          int antinode_ind = antinode1_y * (line_length + 1) + antinode1_x;
          insert(&antinodes, antinode_ind);
        }

        if(antinode2_x >= 0 && antinode2_x < line_length && antinode2_y >= 0 && antinode2_y < total_lines){
          int antinode_ind = antinode2_y * (line_length + 1) + antinode2_x;
          insert(&antinodes, antinode_ind);
        }
      }
    }

    push(same_char, i);
  }

  return antinodes.size;
}

void test(){
  struct Array arr = { 0 };
  push(&arr, 1);

  expect_int(arr.data[0], 1, "arr[0]");
  expect_int(arr.size, 1, "arr size");
  expect_int(arr.capacity, 10, "arr capacity");

  for(int i = 0; i < 10; i++){
    push(&arr, i);
  }

  expect_int(arr.data[10], 9, "arr[10]");
  expect_int(arr.size, 11, "arr size");
  expect_int(arr.capacity, 20, "arr capacity");

  struct BST bst = { 0 };
  insert(&bst, 5);
  insert(&bst, 3);
  insert(&bst, 7);
  insert(&bst, 6);

  expect_int(bst.root->value, 5, "node is expected value");
  expect_int(bst.root->left->value, 3, "node is expected value");
  expect_int(bst.root->right->value, 7, "node is expected value");
  expect_int(bst.root->right->left->value, 6, "node is expected value");

  expect_int(search(&bst, 5), true, "5 in tree");
  expect_int(search(&bst, 3), true, "3 in tree");
  expect_int(search(&bst, 7), true, "7 in tree");
  expect_int(search(&bst, 6), true, "6 in tree");

  expect_int(search(&bst, 11), false, "11 not in tree");
  expect_int(search(&bst, 2), false, "2 not in tree");

  char* in = "............\n........0...\n.....0......\n.......0....\n....0.......\n......A.....\n............\n............\n........A...\n.........A..\n............\n...........\n";

  expect_int(count_antinodes(in, 156), 14, "test case 1");
  expect_int(count_antinodes_2(in, 156), 34, "test case 2");
}

int main(int argc, char** argv){
  if(argc > 1 && strcmp(argv[1], "test") == 0){
    test();
    return 0;
  }

  struct File file;
  bool success = read_file("./input8", &file);

  printf("antinodes: %d\n", count_antinodes(file.data, file.size));
  printf("antinodes2: %d\n", count_antinodes_2(file.data, file.size));

  free(file.data);

  return 0;
}

