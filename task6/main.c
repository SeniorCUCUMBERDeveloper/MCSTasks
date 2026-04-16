#include <stdio.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>

static void print_basic_info(const char *path, const struct stat *info) {
  printf("File: %s\n", path);
  printf("Size: %lld\n", (long long)info->st_size);
  printf("Blocks: %lld\n", (long long)info->st_blocks);
  printf("IO Block: %lld\n", (long long)info->st_blksize);
  printf("Device: %u,%u\n", major(info->st_dev), minor(info->st_dev));
  printf("Inode: %llu\n", (unsigned long long)info->st_ino);
  printf("Links: %llu\n", (unsigned long long)info->st_nlink);
}

int main(int argc, char **argv) {
  struct stat info;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <path>\n", argv[0]);
    return 1;
  }

  if (stat(argv[1], &info) != 0) {
    perror("stat");
    return 1;
  }

  print_basic_info(argv[1], &info);
  return 0;
}
