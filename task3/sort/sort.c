#include "sort.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../config/config.h"

static int normalize_compare(int value) {
  if (value < 0) {
    return -1;
  }

  if (value > 0) {
    return 1;
  }

  return 0;
}

static int compare_lex_strings(const char *left, const char *right) {
  const char *left_ptr = left;
  const char *right_ptr = right;

  while (*left_ptr != '\0' && *right_ptr != '\0') {
    unsigned char left_char = (unsigned char)*left_ptr;
    unsigned char right_char = (unsigned char)*right_ptr;
    int left_lower = tolower(left_char);
    int right_lower = tolower(right_char);

    if (left_lower < right_lower) {
      return -1;
    }

    if (left_lower > right_lower) {
      return 1;
    }

    left_ptr += 1;
    right_ptr += 1;
  }

  if (*left_ptr == '\0' && *right_ptr == '\0') {
    return normalize_compare(strcmp(left, right));
  }

  if (*left_ptr == '\0') {
    return -1;
  }

  return 1;
}

static int compare_plain(const void *left, const void *right) {
  const char *left_value = *(const char *const *)left;
  const char *right_value = *(const char *const *)right;

  return normalize_compare(strcmp(left_value, right_value));
}

static int compare_lex(const void *left, const void *right) {
  const char *left_value = *(const char *const *)left;
  const char *right_value = *(const char *const *)right;

  return compare_lex_strings(left_value, right_value);
}

static int compare_rplain(const void *left, const void *right) {
  return -compare_plain(left, right);
}

static int compare_rlex(const void *left, const void *right) {
  return -compare_lex(left, right);
}

int sort_lines(char **lines, size_t count, const char *mode) {
  int (*compare)(const void *left, const void *right) = NULL;

  if (strcmp(mode, MODE_PLAIN) == 0) {
    compare = compare_plain;
  } else if (strcmp(mode, MODE_LEX) == 0) {
    compare = compare_lex;
  } else if (strcmp(mode, MODE_RPLAIN) == 0) {
    compare = compare_rplain;
  } else if (strcmp(mode, MODE_RLEX) == 0) {
    compare = compare_rlex;
  } else {
    return 0;
  }

  if (count > 1) {
    qsort(lines, count, sizeof(*lines), compare);
  }

  return 1;
}
