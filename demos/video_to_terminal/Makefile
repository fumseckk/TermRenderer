CC = gcc
CFLAGS = -Wall -L../../bin/static/ -lrenderer


SRCS := $(wildcard ./src/*.c)
HDRS := $(wildcard ./src/*.h)
default: all

all: $(HDRS) $(SRCS)
	mkdir -p bin
	$(CC) $(SRCS) -o bin/video_to_terminal $(CFLAGS)

clean:
	-rm -f bin/video_to_terminal