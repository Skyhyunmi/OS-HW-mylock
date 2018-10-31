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
	rm -rf *.o pa3 GEN* RESULT*

.PHONY: test_spinlock
test_spinlock:
	make clean
	make
	./pa3 -0
	bin/count.sh GEN* > RESULT.my
	diff RESULT RESULT.my && echo "Spinlock test failed!!"

.PHONY: test_mutex
test_mutex:
	make clean

.PHONY: test_sem
test_sem:
	make clean
