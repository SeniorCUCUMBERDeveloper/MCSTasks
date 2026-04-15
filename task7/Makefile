CC = gcc
TARGET = task7
SOURCES = main.c write/write.c copy/copy.c print/print.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
