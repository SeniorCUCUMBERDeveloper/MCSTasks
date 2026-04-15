#include "print.h"

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "../config/config.h"
#include "../write/write.h"

int print_file(const char *title, const char *path) {
  char buffer[BUFFER_SIZE];
  int input_fd = open(path, O_RDONLY);
  int has_data = 0;
  int ends_with_newline = 0;

  if (input_fd < 0) {
    return 0;
  }

  if (!write_all(STDOUT_FILENO, title, strlen(title))) {
    close(input_fd);
    return 0;
  }

  while (1) {
    ssize_t read_size = read(input_fd, buffer, sizeof(buffer));

    if (read_size < 0) {
      close(input_fd);
      return 0;
    }

    if (read_size == 0) {
      break;
    }

    has_data = 1;
    ends_with_newline = buffer[read_size - 1] == '\n';
    if (!write_all(STDOUT_FILENO, buffer, (size_t)read_size)) {
      close(input_fd);
      return 0;
    }
  }

  if (!has_data || !ends_with_newline) {
    if (!write_all(STDOUT_FILENO, "\n", 1)) {
      close(input_fd);
      return 0;
    }
  }

  if (close(input_fd) < 0) {
    return 0;
  }

  return 1;
}
