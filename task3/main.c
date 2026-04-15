#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config/config.h"
#include "readline/readline.h"

struct LineArray {
  char **items;
  size_t size;
};

static void trim_line_endings(char *line) {
  size_t length = strlen(line);

  while (length > 0) {
    char last_char = line[length - 1];

    if (last_char != '\n' && last_char != '\r') {
      break;
    }

    line[length - 1] = '\0';
    length -= 1;
  }
}

static int has_visible_chars(const char *line) {
  while (*line != '\0') {
    if (!isspace((unsigned char)*line)) {
      return 1;
    }

    line += 1;
  }

  return 0;
}

static int append_line(struct LineArray *lines, size_t *capacity, char *line) {
  if (lines->size == *capacity) {
    char **new_items = NULL;
    size_t new_capacity = LINE_ARRAY_CAPACITY;

    if (*capacity != 0) {
      new_capacity = *capacity * 2;
    }

    new_items = realloc(lines->items, new_capacity * sizeof(*lines->items));
    if (new_items == NULL) {
      return 0;
    }

    lines->items = new_items;
    *capacity = new_capacity;
  }

  lines->items[lines->size] = line;
  lines->size += 1;
  return 1;
}

static void free_lines(struct LineArray *lines) {
  size_t index = 0;

  for (index = 0; index < lines->size; ++index) {
    free(lines->items[index]);
  }

  free(lines->items);
  lines->items = NULL;
  lines->size = 0;
}

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

static int (*get_compare(const char *mode))(const void *, const void *) {
  if (strcmp(mode, MODE_PLAIN) == 0) {
    return compare_plain;
  }

  if (strcmp(mode, MODE_LEX) == 0) {
    return compare_lex;
  }

  if (strcmp(mode, MODE_RPLAIN) == 0) {
    return compare_rplain;
  }

  if (strcmp(mode, MODE_RLEX) == 0) {
    return compare_rlex;
  }

  return NULL;
}

static int write_lines(FILE *stream, char *const *lines, size_t count) {
  size_t index = 0;

  for (index = 0; index < count; ++index) {
    if (fputs(lines[index], stream) == EOF) {
      return 0;
    }

    if (fputc('\n', stream) == EOF) {
      return 0;
    }
  }

  return 1;
}

int main(int argc, char **argv) {
  FILE *input = NULL;
  FILE *output = NULL;
  struct LineArray lines = {0};
  char *line = NULL;
  size_t capacity = 0;
  int (*compare)(const void *left, const void *right) = NULL;

  if (argc != 4) {
    return 1;
  }

  compare = get_compare(argv[3]);
  if (compare == NULL) {
    return 1;
  }

  input = fopen(argv[1], "r");
  if (input == NULL) {
    return 1;
  }

  while ((line = readline(input)) != NULL) {
    trim_line_endings(line);

    if (!has_visible_chars(line)) {
      free(line);
      continue;
    }

    if (!append_line(&lines, &capacity, line)) {
      free(line);
      fclose(input);
      free_lines(&lines);
      return 1;
    }
  }

  if (ferror(input)) {
    fclose(input);
    free_lines(&lines);
    return 1;
  }

  if (fclose(input) != 0) {
    free_lines(&lines);
    return 1;
  }

  if (lines.size > 1) {
    qsort(lines.items, lines.size, sizeof(*lines.items), compare);
  }

  output = fopen(argv[2], "w");
  if (output == NULL) {
    free_lines(&lines);
    return 1;
  }

  if (!write_lines(output, lines.items, lines.size)) {
    fclose(output);
    free_lines(&lines);
    return 1;
  }

  if (fclose(output) != 0) {
    free_lines(&lines);
    return 1;
  }

  free_lines(&lines);
  return 0;
}
