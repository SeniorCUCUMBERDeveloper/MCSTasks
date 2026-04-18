CC = gcc
TARGET = task8
SOURCES = main.c matrix/matrix.c multiply/multiply.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
