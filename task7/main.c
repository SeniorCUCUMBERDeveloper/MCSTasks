#include <fcntl.h>
#include <unistd.h>

#include "config/config.h"
#include "copy/copy.h"
#include "print/print.h"

int main(int argc, char **argv) {
  int input_fd = -1;

  if (argc != 2) {
    return 1;
  }

  input_fd = open(argv[1], O_RDONLY);
  if (input_fd < 0) {
    return 1;
  }

  if (!copy_to_file(input_fd, PARENT_COPY_PATH)) {
    close(input_fd);
    return 1;
  }

  if (close(input_fd) < 0) {
    return 1;
  }

  if (!print_file(PARENT_TITLE, sizeof(PARENT_TITLE) - 1, PARENT_COPY_PATH)) {
    return 1;
  }

  return 0;
}
