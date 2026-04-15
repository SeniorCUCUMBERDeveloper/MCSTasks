#pragma once

#include <stddef.h>
#include <stdio.h>

int append_value(int **data, size_t *size, size_t *capacity, int value);
int read_array(FILE *stream, int **data, size_t *size);
