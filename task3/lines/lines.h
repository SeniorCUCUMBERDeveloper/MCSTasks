#pragma once

#include <stddef.h>
#include <stdio.h>

struct LineArray {
  char **items;
  size_t size;
};

int read_lines(FILE *stream, struct LineArray *lines);
void free_lines(struct LineArray *lines);
