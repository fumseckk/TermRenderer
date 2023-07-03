# options
CC=gcc
CFLAGS=-Wall -lm
TARGET=sort

# globs
MAIN := $(main.c)
SRCS := $(wildcard src/*.c) $(wildcard src/*/*.c)
HDRS := $(wildcard headers/algorithms/*.h) headers/main.h 
OBJS := $(patsubst src/%.c,bin/%.o,$(SRCS)) # Get all files from src/
OBJS := $(patsubst src/algorithms/%.c,bin/%.c,$(OBJS)) # Then modify the output to also get files from src/algorithms/ 

# link it all together
$(TARGET): $(OBJS) $(HDRS) Makefile
		$(CC) $(OBJS) $(CFLAGS) -o $(TARGET) 
	
# compile an object based on source and headers
bin/%.o: src/%.c $(HDRS) Makefile
		@mkdir -p bin
		@mkdir -p bin/algorithms
		$(CC) -c $< -o $@ $(CFLAGS)

debug: $(OBJS) $(HDRS) Makefile
		@mkdir -p bin
		@mkdir -p bin/algorithms
		$(CC) -c $< -g -o $@ $(CFLAGS)
		$(CC) $(OBJS) $(CFLAGS) -g -o $(TARGET) 

clean:
		rm -f $(TARGET) $(OBJS)