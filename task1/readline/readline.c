#include "readline.h"

#include <stdlib.h>
#include <string.h>

#define READ_BUFFER_SIZE 256

char *readline(FILE *stream) {
  static char buffer[READ_BUFFER_SIZE];
  char *line = NULL;
  size_t length = 0;

  while (fgets(buffer, sizeof(buffer), stream) != NULL) {
    size_t part_length = strlen(buffer);
    char *new_line = realloc(line, length + part_length + 1);

    if (new_line == NULL) {
      free(line);
      return NULL;
    }

    line = new_line;
    memcpy(line + length, buffer, part_length + 1);
    length += part_length;

    if (length > 0 && line[length - 1] == '\n') {
      break;
    }
  }

  if (ferror(stream)) {
    free(line);
    return NULL;
  }

  return line;
}
