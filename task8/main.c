#include "matrix/matrix.h"
#include "multiply/multiply.h"

#define MATRIX_SIZE 3

static void fill_input_matrices(struct Matrix *first, struct Matrix *second) {
  int row = 0;
  int col = 0;

  for (row = 0; row < first->size; ++row) {
    for (col = 0; col < first->size; ++col) {
      set_matrix_element(first, row, col, row + col + 1);

      if (row == col) {
        set_matrix_element(second, row, col, 1);
      } else {
        set_matrix_element(second, row, col, 0);
      }
    }
  }
}

static void free_all_matrices(struct Matrix *first, struct Matrix *second,
                              struct Matrix *result) {
  delete_matrix(first);
  delete_matrix(second);
  delete_matrix(result);
}

int main(void) {
  struct Matrix first = {0};
  struct Matrix second = {0};
  struct Matrix result = {0};

  if (!init_matrix(&first, MATRIX_SIZE)) {
    return 1;
  }

  if (!init_matrix(&second, MATRIX_SIZE)) {
    free_all_matrices(&first, &second, &result);
    return 1;
  }

  if (!init_matrix(&result, MATRIX_SIZE)) {
    free_all_matrices(&first, &second, &result);
    return 1;
  }

  fill_input_matrices(&first, &second);

  if (!multiply_matrices(&first, &second, &result)) {
    free_all_matrices(&first, &second, &result);
    return 1;
  }

  print_matrix(&first);
  print_matrix(&second);
  print_matrix(&result);

  free_all_matrices(&first, &second, &result);
  return 0;
}
