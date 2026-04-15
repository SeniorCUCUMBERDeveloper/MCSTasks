#include <stdio.h>

#include "lines/lines.h"
#include "print/print.h"
#include "sort/sort.h"

int main(int argc, char **argv) {
  FILE *input = NULL;
  FILE *output = NULL;
  struct LineArray lines = {0};

  if (argc != 4) {
    return 1;
  }

  input = fopen(argv[1], "r");
  if (input == NULL) {
    return 1;
  }

  if (!read_lines(input, &lines)) {
    fclose(input);
    return 1;
  }

  if (fclose(input) != 0) {
    free_lines(&lines);
    return 1;
  }

  if (!sort_lines(lines.items, lines.size, argv[3])) {
    free_lines(&lines);
    return 1;
  }

  output = fopen(argv[2], "w");
  if (output == NULL) {
    free_lines(&lines);
    return 1;
  }

  if (!print_lines(output, lines.items, lines.size)) {
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
