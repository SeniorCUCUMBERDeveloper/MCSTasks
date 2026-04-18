#define _POSIX_C_SOURCE 199309L

#include "multiply.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static void set_multiply_positions(struct MultiplyPositions *positions, int row,
                                   int col, int index) {
  positions->first.row = row;
  positions->first.col = index;
  positions->second.row = index;
  positions->second.col = col;
  positions->result.row = row;
  positions->result.col = col;
}

static void restore_default_handler(const struct MultiplyState *state) {
  struct sigaction action;

  if (!state->use_sigaction) {
    signal(SIGINT, SIG_DFL);
    return;
  }

  memset(&action, 0, sizeof(action));
  action.sa_handler = SIG_DFL;
  sigemptyset(&action.sa_mask);
  sigaction(SIGINT, &action, NULL);
}

static void print_current_positions(const struct MultiplyState *state) {
  printf("%d %d\n", state->current_positions.first.row + 1, state->current_positions.first.col + 1);
  printf("%d %d\n", state->current_positions.second.row + 1, state->current_positions.second.col + 1);
  printf("%d %d\n", state->current_positions.result.row + 1, state->current_positions.result.col + 1);
  fflush(stdout);
}

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

static void process_sigint(struct MultiplyState *state) {
  if (state->sigint_received == NULL || !*state->sigint_received) {
    return;
  }

  *state->sigint_received = 0;
  print_current_positions(state);
  restore_default_handler(state);
}

int multiply_matrices(const struct Matrix *first, const struct Matrix *second,
                      struct Matrix *result, struct MultiplyState *state) {
  int row = 0;
  int col = 0;
  int index = 0;
  int value = 0;

  if (state == NULL) {
    return 0;
  }

  for (row = 0; row < result->size; ++row) {
    for (col = 0; col < result->size; ++col) {
      set_matrix_element(result, row, col, 0);

      for (index = 0; index < first->size; ++index) {
        set_multiply_positions(&state->current_positions, row, col, index);
        process_sigint(state);

        value = get_matrix_element(result, row, col);
        value += get_matrix_element(first, state->current_positions.first.row, state->current_positions.first.col) * get_matrix_element(second, state->current_positions.second.row, state->current_positions.second.col);
        set_matrix_element(result, row, col, value);

        if (!sleep_half_second()) {
          return 0;
        }

        process_sigint(state);
      }
    }
  }

  return 1;
}
