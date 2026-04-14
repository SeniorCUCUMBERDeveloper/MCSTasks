CC = gcc
TARGET = task1

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -pthread -o $(TARGET) main.c

clean:
	rm -f $(TARGET)
