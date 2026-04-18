#define _POSIX_C_SOURCE 199309L

#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "matrix/matrix.h"
#include "multiply/multiply.h"

#define MATRIX_SIZE 100
#define METHOD_SIGNAL 1
#define METHOD_SIGACTION 2

static volatile sig_atomic_t sigint_received = 0;

static void handle_sigint(int signum) {
  if (signum == SIGINT) {
    sigint_received = 1;
  }
}

static int parse_method(int argc, char **argv) {
  if (argc != 2) {
    return 0;
  }

  if (strcmp(argv[1], "--signal") == 0) {
    return METHOD_SIGNAL;
  }

  if (strcmp(argv[1], "--sigaction") == 0) {
    return METHOD_SIGACTION;
  }

  return 0;
}

static int install_handler(int method) {
  struct sigaction action;

  if (method == METHOD_SIGNAL) {
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
      return 0;
    }

    return 1;
  }

  memset(&action, 0, sizeof(action));
  action.sa_handler = handle_sigint;
  sigemptyset(&action.sa_mask);

  if (sigaction(SIGINT, &action, NULL) != 0) {
    return 0;
  }

  return 1;
}

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

int main(int argc, char **argv) {
  int method = 0;
  struct Matrix first = {0};
  struct Matrix second = {0};
  struct Matrix result = {0};

  method = parse_method(argc, argv);
  if (method == 0) {
    fprintf(stderr, "Usage: %s --signal | --sigaction\n", argv[0]);
    return 1;
  }

  if (!install_handler(method)) {
    perror("signal");
    return 1;
  }

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

  free_all_matrices(&first, &second, &result);
  return 0;
}
