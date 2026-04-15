#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
#define OUTPUT_FILE_MODE 0644
#define PARENT_COPY_PATH "parent_copy"
#define PARENT_TITLE "Parent:\n"

static int write_all(int fd, const char *buffer, size_t size) {
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

static int copy_to_file(int input_fd, const char *output_path) {
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

static int print_file(const char *title, size_t title_size, const char *path) {
  char buffer[BUFFER_SIZE];
  int input_fd = open(path, O_RDONLY);
  int has_data = 0;
  int ends_with_newline = 0;

  if (input_fd < 0) {
    return 0;
  }

  if (!write_all(STDOUT_FILENO, title, title_size)) {
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
