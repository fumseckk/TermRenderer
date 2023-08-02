CC = gcc
CFLAGS = -Wall -L../../bin/static/ -lrenderer


SRCS := $(wildcard ./*.c)
HDRS := $(wildcard ./*.h)
default: all

all: $(HDRS) $(SRCS)
	$(CC) $(SRCS) -o bin/video $(CFLAGS)

clean:
	-rm -f video