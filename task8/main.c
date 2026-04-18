#include "matrix/matrix.h"

int main(void) {
  int row = 0;
  int col = 0;
  int value = 1;
  struct Matrix matrix = {0};

  if (!init_matrix(&matrix, 3)) {
    return 1;
  }

  for (row = 0; row < matrix.size; ++row) {
    for (col = 0; col < matrix.size; ++col) {
      set_matrix_element(&matrix, row, col, value);
      value += 1;
    }
  }

  print_matrix(&matrix);
  delete_matrix(&matrix);
  return 0;
}
