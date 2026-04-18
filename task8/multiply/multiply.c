#define _POSIX_C_SOURCE 199309L

#include "multiply.h"

#include <errno.h>
#include <time.h>

static int sleep_half_second(void) {
  struct timespec delay;

  delay.tv_sec = 0;
  delay.tv_nsec = 500000000L;

  if (nanosleep(&delay, NULL) == 0) {
    return 1;
  }

  if (errno == EINTR) {
    return 1;
  }

  return 0;
}

int multiply_matrices(const struct Matrix *first, const struct Matrix *second, struct Matrix *result) {
  int row = 0;
  int col = 0;
  int index = 0;
  int value = 0;

  for (row = 0; row < result->size; ++row) {
    for (col = 0; col < result->size; ++col) {
      set_matrix_element(result, row, col, 0);

      for (index = 0; index < first->size; ++index) {
        value = get_matrix_element(result, row, col);
        value += get_matrix_element(first, row, index) * get_matrix_element(second, index, col);
        set_matrix_element(result, row, col, value);

        if (!sleep_half_second()) {
          return 0;
        }
      }
    }
  }

  return 1;
}
