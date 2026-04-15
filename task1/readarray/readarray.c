#include "readarray.h"

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "../readline/readline.h"

int append_value(int **data, size_t *size, size_t *capacity, int value) {
  if (*size == *capacity) {
    int *new_data = NULL;
    size_t new_capacity = 16;

    if (*capacity != 0) {
      new_capacity = *capacity * 2;
    }

    new_data = realloc(*data, new_capacity * sizeof(**data));
    if (new_data == NULL) {
      return 0;
    }
    *data = new_data;
    *capacity = new_capacity;
  }

  (*data)[*size] = value;
  *size += 1;
  return 1;
}

int read_array(FILE *stream, int **data, size_t *size) {
  int *buffer = NULL;
  size_t count = 0;
  size_t capacity = 0;
  char *line = NULL;

  while ((line = readline(stream)) != NULL) {
    char *token = strtok(line, " \t\r\n");

    while (token != NULL) {
      char *end = NULL;
      long value = 0;
      errno = 0;
      value = strtol(token, &end, 10);
      if (errno != 0 || *end != '\0' || value < INT_MIN || value > INT_MAX) {
        free(line);
        free(buffer);
        return 0;
      }

      if (!append_value(&buffer, &count, &capacity, (int)value)) {
        free(line);
        free(buffer);
        return 0;
      }

      token = strtok(NULL, " \t\r\n");
    }

    free(line);
  }

  if (ferror(stream)) {
    free(buffer);
    return 0;
  }

  *data = buffer;
  *size = count;
  return 1;
}
