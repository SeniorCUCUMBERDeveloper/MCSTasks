#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>

int init_matrix(struct Matrix *matrix, int size) {
  int row = 0;

  matrix->size = size;
  matrix->data = malloc((size_t)size * sizeof(*matrix->data));
  if (matrix->data == NULL) {
    matrix->size = 0;
    return 0;
  }

  for (row = 0; row < size; ++row) {
    matrix->data[row] = calloc((size_t)size, sizeof(*matrix->data[row]));
    if (matrix->data[row] == NULL) {
      matrix->size = row;
      delete_matrix(matrix);
      return 0;
    }
  }

  return 1;
}

void delete_matrix(struct Matrix *matrix) {
  int row = 0;

  if (matrix->data != NULL) {
    for (row = 0; row < matrix->size; ++row) {
      free(matrix->data[row]);
    }

    free(matrix->data);
  }

  matrix->size = 0;
  matrix->data = NULL;
}

int get_matrix_element(const struct Matrix *matrix, int row, int col) {
  if (row < 0 || row >= matrix->size) {
    return 0;
  }

  if (col < 0 || col >= matrix->size) {
    return 0;
  }

  return matrix->data[row][col];
}

void set_matrix_element(struct Matrix *matrix, int row, int col, int value) {
  if (row < 0 || row >= matrix->size) {
    return;
  }

  if (col < 0 || col >= matrix->size) {
    return;
  }

  matrix->data[row][col] = value;
}

void print_matrix(const struct Matrix *matrix) {
  int row = 0;
  int col = 0;

  for (row = 0; row < matrix->size; ++row) {
    for (col = 0; col < matrix->size; ++col) {
      if (col > 0) {
        printf(" ");
      }

      printf("%d", matrix->data[row][col]);
    }

    printf("\n");
  }
}
