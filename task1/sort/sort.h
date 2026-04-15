#pragma once

#include <stddef.h>

struct Range {
  size_t begin;
  size_t end;
};

struct SortTask {
  int *data;
  size_t begin;
  size_t end;
};

int compare_ints(const void *left, const void *right);
void build_ranges(size_t size, size_t part_count, struct Range *ranges);
void *sort_chunk(void *arg);
void merge_ranges(int *data, int *buffer, size_t left_begin, size_t middle, size_t right_end);
int parallel_sort(int *data, size_t size, size_t thread_count);
