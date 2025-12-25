CC = gcc
CFLAGS = -Wall -g
INCLUDES = -I./include

SRCS = src/main.c src/builtins.c
TARGET = shell

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)