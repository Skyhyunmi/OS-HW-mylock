#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#include "config.h"
#include "locks.h"

static int nr_slots = 0;

static enum lock_types lock_type;

void (*enqueue_fn)(int value) = NULL;
int (*dequeue_fn)(void) = NULL;

void enqueue_ringbuffer(int value)
{
	//printf("Enqueue %d\n", value);
	assert(enqueue_fn);
	enqueue_fn(value);
}

int dequeue_ringbuffer(void)
{
	int value;
	assert(dequeue_fn);
	value = dequeue_fn();
	//printf("Dequeued %d\n", value);
	return value;
}

/*********************************************************************
 * TODO: Implement using spinlock
 */
void enqueue_using_spinlock(int value)
{
}

int dequeue_using_spinlock(void)
{
	return 0;
}


int init_ringbuffer(const int _nr_slots_, const enum lock_types _lock_type_)
{
	if (_nr_slots_ <= 0) {
		return -EINVAL;
	}
	nr_slots = _nr_slots_;

	lock_type = _lock_type_;

	/* TODO: Initialize your ringbuffer and synchronization mechanism */
	switch (lock_type) {
	case lock_spinlock:
		enqueue_fn = &enqueue_using_spinlock;
		dequeue_fn = &dequeue_using_spinlock;
		break;
	case lock_mutex:
		break;
	case lock_semaphore:
		break;
	}

	return 0;
}

void fini_ringbuffer(void)
{
	/* TODO: Clean up what you allocated */
	switch (lock_type) {
	case lock_spinlock:
		break;
	case lock_mutex:
		break;
	case lock_semaphore:
		break;
	}
}
