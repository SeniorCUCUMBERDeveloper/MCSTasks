#include "print.h"

int print_lines(FILE *stream, char *const *lines, size_t count) {
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
