#include "lines.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../config/config.h"
#include "../readline/readline.h"

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

int read_lines(FILE *stream, struct LineArray *lines) {
  char *line = NULL;
  size_t capacity = 0;

  lines->items = NULL;
  lines->size = 0;

  while ((line = readline(stream)) != NULL) {
    trim_line_endings(line);

    if (!has_visible_chars(line)) {
      free(line);
      continue;
    }

    if (!append_line(lines, &capacity, line)) {
      free(line);
      free_lines(lines);
      return 0;
    }
  }

  if (ferror(stream)) {
    free_lines(lines);
    return 0;
  }

  return 1;
}

void free_lines(struct LineArray *lines) {
  size_t index = 0;

  for (index = 0; index < lines->size; ++index) {
    free(lines->items[index]);
  }

  free(lines->items);
  lines->items = NULL;
  lines->size = 0;
}
