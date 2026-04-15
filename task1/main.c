#include <stdio.h>
#include <stdlib.h>

#include "readarray/readarray.h"
#include "sort/sort.h"

static void print_array(const int *data, size_t size) {
  size_t index = 0;

  for (index = 0; index < size; ++index) {
    if (index > 0) {
      printf(" ");
    }
    printf("%d", data[index]);
  }

  printf("\n");
}

int main(void) {
  int *data = NULL;
  size_t size = 0;
  size_t thread_count = 3;

  if (!read_array(stdin, &data, &size)) {
    return 1;
  }

  if (!parallel_sort(data, size, thread_count)) {
    free(data);
    return 1;
  }

  print_array(data, size);
  free(data);
  return 0;
}
