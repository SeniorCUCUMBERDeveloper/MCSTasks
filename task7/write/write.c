#include "write.h"

#include <unistd.h>

int write_all(int fd, const char *buffer, size_t size) {
  size_t written = 0;

  while (written < size) {
    ssize_t result = write(fd, buffer + written, size - written);

    if (result <= 0) {
      return 0;
    }

    written += (size_t)result;
  }

  return 1;
}
