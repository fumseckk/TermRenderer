CC=gcc
CFLAGS=-Wall

SRC := $(wildcard src/*.c)
HDR := $(wildcard src/*.h)
LIBHDR := $(wildcard headers/*.h)
TESTS := $(wildcard tests/*.c)
OBJS := $(patsubst src/%.c,src/%.o,$(SRC))

OUTLIB=bin/static/librenderer.a
OUTTESTS=bin/test


all: tests

lib: $(OUTLIB)

tests: $(TESTS) $(OUTLIB) $(OBJS) $(LIBHDR) $(HDR)
		$(CC) $< -o $(OUTTESTS) -L./bin/static/ -lrenderer

$(OUTLIB): $(OBJS) $(LIBHDR) $(HDR)
		ar -crs $@ $<


clean:
	rm -f $(OUTLIB) $(OUTTESTS) $(OBJS)