#ifndef MANDEL_UTIL_H
#define MANDEL_UTIL_H

#include <stdlib.h>
#include <stdio.h>

char* readFile(const char* path, int* out_length){
  FILE* file = fopen(path, "r");

  fseek(file, 0, SEEK_END);
  size_t length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char* contents = (char*)malloc(sizeof(char) * length);
  fread(contents, sizeof(char), length, file);

  if(out_length != NULL){
    *out_length = length;
  }
  
  return contents;
}

#endif
