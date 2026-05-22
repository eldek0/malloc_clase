# Makefile

CC = gcc

CFLAGS = -Wall -Wextra -g

TARGET = allocator_test

OBJS = \
	allocator_test.o \
	my_malloc.o \
	my_malloc_init.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

allocator_test.o: allocator_test.c my_malloc.h
	$(CC) $(CFLAGS) -c allocator_test.c

my_malloc.o: my_malloc.c my_malloc.h
	$(CC) $(CFLAGS) -c my_malloc.c

my_malloc_init.o: my_malloc_init.c my_malloc.h
	$(CC) $(CFLAGS) -c my_malloc_init.c

clean:
	rm -f $(TARGET) *.o
