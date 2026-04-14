#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

static int compare_ints(const void *left, const void *right) {
  int left_value = *(const int *)left;
  int right_value = *(const int *)right;

  if (left_value < right_value) {
    return -1;
  }

  if (left_value > right_value) {
    return 1;
  }

  return 0;
}

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
  int data[] = {7, 3, 9, 1, 5, 2, 8, 4, 6};
  size_t size = sizeof(data) / sizeof(data[0]);

  qsort(data, size, sizeof(data[0]), compare_ints);
  print_array(data, size);
  return 0;
}
