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
test_spinlock: clean all
	./pa3 -0
	@bin/count.sh GEN* > RESULT.my
	diff RESULT.my RESULT.my
	@echo "[PASS] spinlock"

.PHONY: test_mutex
test_mutex: clean all
	./pa3 -1
	@bin/count.sh GEN* > RESULT.my
	diff RESULT.my RESULT.my
	@echo "[PASS] mutex"

.PHONY: test_sem
test_sem: clean all
	./pa3 -2
	@bin/count.sh GEN* > RESULT.my
	diff RESULT.my RESULT.my
	@echo "[PASS] semaphore"

.PHONY: test_all
test_all: clean all test_spinlock test_mutex test_sem
