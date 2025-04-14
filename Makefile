CC = g++
CFLAGS = -Wall -Wextra -Iinclude

SRCS = $(wildcard src/*.cpp)
TARGET = huff

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
