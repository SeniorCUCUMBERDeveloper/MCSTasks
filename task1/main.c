#include <pthread.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

struct Range {
  size_t begin;
  size_t end;
};

struct SortTask {
  int *data;
  size_t begin;
  size_t end;
};

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

static void build_ranges(size_t size, size_t part_count, struct Range *ranges) {
  size_t begin = 0;
  size_t index = 0;
  size_t base_size = size / part_count;
  size_t extra = size % part_count;

  for (index = 0; index < part_count; ++index) {
    size_t current_size = base_size;

    if (index < extra) {
      current_size += 1;
    }

    ranges[index].begin = begin;
    ranges[index].end = begin + current_size;
    begin = ranges[index].end;
  }
}

static void *sort_chunk(void *arg) {
  struct SortTask *task = arg;
  size_t size = task->end - task->begin;

  if (size > 1) {
    qsort(task->data + task->begin, size, sizeof(task->data[0]), compare_ints);
  }

  return NULL;
}

static void merge_ranges(int *data, int *buffer, size_t left_begin, size_t middle, size_t right_end) {
  size_t left = left_begin;
  size_t right = middle;
  size_t dest = left_begin;
  size_t index = 0;

  while (left < middle && right < right_end) {
    if (data[left] <= data[right]) {
      buffer[dest] = data[left];
      left += 1;
    } else {
      buffer[dest] = data[right];
      right += 1;
    }
    dest += 1;
  }

  while (left < middle) {
    buffer[dest] = data[left];
    left += 1;
    dest += 1;
  }

  while (right < right_end) {
    buffer[dest] = data[right];
    right += 1;
    dest += 1;
  }

  for (index = left_begin; index < right_end; ++index) {
    data[index] = buffer[index];
  }
}

static int parallel_sort(int *data, size_t size, size_t thread_count) {
  int *buffer = NULL;
  pthread_t *threads = NULL;
  struct SortTask *tasks = NULL;
  struct Range *ranges = NULL;
  struct Range *next_ranges = NULL;
  size_t range_count = 0;
  size_t index = 0;

  if (size < 2) {
    return 1;
  }

  range_count = thread_count;
  if (range_count > size) {
    range_count = size;
  }

  buffer = malloc(size * sizeof(*buffer));
  threads = malloc(range_count * sizeof(*threads));
  tasks = malloc(range_count * sizeof(*tasks));
  ranges = malloc(range_count * sizeof(*ranges));
  next_ranges = malloc(range_count * sizeof(*next_ranges));
  if (buffer == NULL || threads == NULL || tasks == NULL || ranges == NULL || next_ranges == NULL) {
    free(next_ranges);
    free(ranges);
    free(tasks);
    free(threads);
    free(buffer);
    return 0;
  }

  build_ranges(size, range_count, ranges);
  for (index = 0; index < range_count; ++index) {
    tasks[index].data = data;
    tasks[index].begin = ranges[index].begin;
    tasks[index].end = ranges[index].end;

    if (pthread_create(&threads[index], NULL, sort_chunk, &tasks[index]) != 0) {
      size_t join_index = 0;

      for (join_index = 0; join_index < index; ++join_index) {
        pthread_join(threads[join_index], NULL);
      }

      free(next_ranges);
      free(ranges);
      free(tasks);
      free(threads);
      free(buffer);
      return 0;
    }
  }

  for (index = 0; index < range_count; ++index) {
    pthread_join(threads[index], NULL);
  }

  while (range_count > 1) {
    size_t next_count = 0;

    for (index = 0; index + 1 < range_count; index += 2) {
      merge_ranges(data, buffer, ranges[index].begin, ranges[index].end, ranges[index + 1].end);
      next_ranges[next_count].begin = ranges[index].begin;
      next_ranges[next_count].end = ranges[index + 1].end;
      next_count += 1;
    }

    if (index < range_count) {
      next_ranges[next_count] = ranges[index];
      next_count += 1;
    }

    for (index = 0; index < next_count; ++index) {
      ranges[index] = next_ranges[index];
    }

    range_count = next_count;
  }

  free(next_ranges);
  free(ranges);
  free(tasks);
  free(threads);
  free(buffer);
  return 1;
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
  size_t thread_count = 3;

  if (!parallel_sort(data, size, thread_count)) {
    return 1;
  }

  print_array(data, size);
  return 0;
}
