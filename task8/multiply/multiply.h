#pragma once

#include <signal.h>

#include "../matrix/matrix.h"

struct MultiplyPositions {
  struct MatrixPosition first;
  struct MatrixPosition second;
  struct MatrixPosition result;
};

struct MultiplyState {
  struct MultiplyPositions current_positions;
  volatile sig_atomic_t *sigint_received;
  int use_sigaction;
};

int multiply_matrices(const struct Matrix *first, const struct Matrix *second, struct Matrix *result, struct MultiplyState *state);
