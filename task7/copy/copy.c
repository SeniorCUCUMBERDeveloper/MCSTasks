#include "copy.h"

#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>

#include "../config/config.h"
#include "../write/write.h"

int copy_to_file(int input_fd, const char *output_path) {
  char buffer[BUFFER_SIZE];
  int output_fd = open(output_path, O_WRONLY | O_CREAT | O_TRUNC, OUTPUT_FILE_MODE);

  if (output_fd < 0) {
    return 0;
  }

  while (1) {
    ssize_t read_size = read(input_fd, buffer, sizeof(buffer));

    if (read_size < 0) {
      close(output_fd);
      return 0;
    }

    if (read_size == 0) {
      break;
    }

    if (!write_all(output_fd, buffer, (size_t)read_size)) {
      close(output_fd);
      return 0;
    }
  }

  if (close(output_fd) < 0) {
    return 0;
  }

  return 1;
}
