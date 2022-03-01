CC=gcc
CFLAGS=-Wall -lm -O3 -DNDEBUG

SRC := $(wildcard src/*.c)
HDR := $(wildcard src/*.h)
LIBHDR := $(wildcard headers/*.h)
TESTS := $(wildcard tests/*.c)
OBJS := $(patsubst src/%.c,src/%.o,$(SRC))
OUTTESTS=$(patsubst tests/%.c,bin/%,$(TESTS))

OUTLIB=bin/static/librenderer.a
OUTTESTS=bin/


all: tests pong

lib: $(OUTLIB)

tests: $(TESTS) $(OUTLIB) $(OBJS) $(LIBHDR) $(HDR)
		$(CC) tests/main.c -o bin/tests $(CFLAGS) -L./bin/static/ -lrenderer

pong: $(TESTS) $(OUTLIB) $(OBJS) $(LIBHDR) $(HDR)
		$(CC) tests/pong.c -o bin/pong $(CFLAGS) -L./bin/static/ -lrenderer

$(OUTLIB): $(OBJS) $(LIBHDR) $(HDR)
		ar -crs $@ $(OBJS)


clean:
	rm -f $(OUTLIB) $(OUTTESTS) $(OBJS)