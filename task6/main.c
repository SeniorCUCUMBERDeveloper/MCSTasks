#include <stdio.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/types.h>

static void file_type(mode_t mode, char *symbol, const char **name) {
  *symbol = '?';
  *name = "unknown";

  if (S_ISREG(mode)) {
    *symbol = '-';
    *name = "regular file";
    return;
  }

  if (S_ISDIR(mode)) {
    *symbol = 'd';
    *name = "directory";
    return;
  }

  if (S_ISLNK(mode)) {
    *symbol = 'l';
    *name = "symbolic link";
    return;
  }

  if (S_ISCHR(mode)) {
    *symbol = 'c';
    *name = "character device";
    return;
  }

  if (S_ISBLK(mode)) {
    *symbol = 'b';
    *name = "block device";
    return;
  }

  if (S_ISFIFO(mode)) {
    *symbol = 'p';
    *name = "fifo";
  }
}

static void format_permissions(char symbol, mode_t mode, char permissions[11]) {
  permissions[0] = symbol;
  permissions[1] = (mode & S_IRUSR) ? 'r' : '-';
  permissions[2] = (mode & S_IWUSR) ? 'w' : '-';
  permissions[3] = (mode & S_IXUSR) ? 'x' : '-';
  permissions[4] = (mode & S_IRGRP) ? 'r' : '-';
  permissions[5] = (mode & S_IWGRP) ? 'w' : '-';
  permissions[6] = (mode & S_IXGRP) ? 'x' : '-';
  permissions[7] = (mode & S_IROTH) ? 'r' : '-';
  permissions[8] = (mode & S_IWOTH) ? 'w' : '-';
  permissions[9] = (mode & S_IXOTH) ? 'x' : '-';
  permissions[10] = '\0';
}

static void print_file_info(const char *path, const struct stat *info) {
  char permissions[11];
  char type_symbol = '?';
  const char *type_name = NULL;

  file_type(info->st_mode, &type_symbol, &type_name);
  format_permissions(type_symbol, info->st_mode, permissions);

  printf("File: %s\n", path);
  printf("Size: %lld\n", (long long)info->st_size);
  printf("Blocks: %lld\n", (long long)info->st_blocks);
  printf("IO Block: %lld\n", (long long)info->st_blksize);
  printf("Type: %s\n", type_name);
  printf("Device: %u,%u\n", major(info->st_dev), minor(info->st_dev));
  printf("Inode: %llu\n", (unsigned long long)info->st_ino);
  printf("Links: %llu\n", (unsigned long long)info->st_nlink);
  printf("Access: (%04o/%s)\n", info->st_mode & 0777, permissions);
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

  print_file_info(argv[1], &info);
  return 0;
}
