#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "config.h"
#include "locks.h"

static int nr_slots = 0;

/* Use this lock to protect your ring buffer */
static struct lock lock;
static enum lock_types lock_type;

int init_ringbuffer(const int _nr_slots_, const enum lock_types _lock_type_)
{
	if (_nr_slots_ <= 0) {
		return -EINVAL;
	}
	nr_slots = _nr_slots_;

	lock_type = _lock_type_;

	init_lock(&lock, lock_type, 0, 0);

	/* TODO: Implement your own ring buffer from here */

	return 0;
}

void fini_ringbuffer(void)
{
	/* TODO: Clean up what you allocated */
}

void enqueue_ringbuffer(int value)
{
	/* TODO: Implement your multithread-safe enqueue function */
	/* Your implementation will look like this (but not limited to):
	 *
	 * acquire_lock(&lock);
	 * do something to enqueue the element with @value
	 * release_lock(&lock);
	 */
}

int dequeue_ringbuffer(void)
{
	/* TODO: Replace the following stub code with yours */
	return random() % MAX_VALUE;
}
