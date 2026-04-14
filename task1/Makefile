CC = gcc
TARGET = task1
SOURCES = main.c readline/readline.c readarray/readarray.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -pthread -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
