#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void) {
  int fd = -1;
  int error_code = 0;

  fd = open("/nothing", O_RDONLY);
  if (fd != -1) {
    close(fd);
    return 0;
  }

  error_code = errno;

  errno = error_code;
  perror("Can't open file");
  fprintf(stderr, "Errno = %d\n", error_code);
  fprintf(stderr, "strerror = %s\n", strerror(error_code));
  return error_code;
}
