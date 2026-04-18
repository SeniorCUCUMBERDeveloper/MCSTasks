#pragma once

struct Matrix {
  int size;
  int **data;
};

struct MatrixPosition {
  int row;
  int col;
};

int init_matrix(struct Matrix *matrix, int size);
void delete_matrix(struct Matrix *matrix);
int get_matrix_element(const struct Matrix *matrix, int row, int col);
void set_matrix_element(struct Matrix *matrix, int row, int col, int value);
void print_matrix(const struct Matrix *matrix);
