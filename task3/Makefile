CC = gcc
TARGET = task3
SOURCES = main.c readline/readline.c lines/lines.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
