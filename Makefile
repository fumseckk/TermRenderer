CC=gcc
CFLAGS=-Wall -lm -O3 -DNDEBUG

SRC := $(wildcard src/*.c)
HDR := $(wildcard src/*.h)
LIBHDR := $(wildcard headers/*.h)
TESTS := $(wildcard tests/*.c)
OBJS := $(patsubst src/%.c,src/%.o,$(SRC))
OUTTESTS=$(patsubst tests/%.c,bin/%,$(TESTS))

OUTLIB=bin/static/librenderer.a

all: test pong

lib: $(OUTLIB)

test: $(TESTS) $(OUTLIB) $(OBJS) $(LIBHDR) $(HDR)
		$(CC) tests/main.c -o bin/test $(CFLAGS) -L./bin/static/ -lrenderer

pong: $(TESTS) $(OUTLIB) $(OBJS) $(LIBHDR) $(HDR)
		$(CC) tests/pong.c -o bin/pong $(CFLAGS) -L./bin/static/ -lrenderer

$(OUTLIB): $(OBJS) $(LIBHDR) $(HDR)
		mkdir -p bin/static
		ar -crs $@ $(OBJS)


clean:
	rm -rf $(OUTLIB) $(OBJS) bin/*