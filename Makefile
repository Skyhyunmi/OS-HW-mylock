CC=gcc
CFLAGS=-g
LDFLAGS=-lpthread

HEADERS=$(wildcard ./*.h)

.PHONY: all
all: pa3

%.o: %.c $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@

pa3: main.o generator.o locks.o ringbuffer.o counter.o
	$(CC) $(LDFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf *.o pa3

.PHONY: test_busy
test_busy:
	make clean
	make

.PHONY: test_blocking
test_blocking:

.PHONY: test_sem
test_sem:
