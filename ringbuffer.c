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
int queue[100000000];
int cur=-1;
void enqueue_ringbuffer(int value)
{
	assert(enqueue_fn);
	assert(value >= MIN_VALUE && value < MAX_VALUE);

	enqueue_fn(value);
}

int dequeue_ringbuffer(void)
{
	int value;

	assert(dequeue_fn); 
	value = dequeue_fn();
	if(value==-1) return -1;
	assert(value >= MIN_VALUE && value < MAX_VALUE);

	return value;
}


/*********************************************************************
 * TODO: Implement using spinlock
 */
struct spinlock s,t;
void enqueue_using_spinlock(int value)
{
	acquire_spinlock(&s);
	queue[++cur] = value;
	release_spinlock(&s);
}

int dequeue_using_spinlock(void)
{
	int res=-1;
	acquire_spinlock(&s);
	if(cur==-1); 
	else res=queue[cur--];
	release_spinlock(&s);
	return res;
}

void init_using_spinlock(void)
{
	init_spinlock(&s);
	init_spinlock(&t);
	enqueue_fn = &enqueue_using_spinlock;
	dequeue_fn = &dequeue_using_spinlock;
}

void fini_using_spinlock(void)
{

}


/*********************************************************************
 * TODO: Implement using mutex
 */
struct mutex m;
void enqueue_using_mutex(int value)
{
	acquire_mutex(&m);
	queue[++cur] = value;
	release_mutex(&m);
}

int dequeue_using_mutex(void)
{
	int res=-1;
	acquire_mutex(&m);
	if(cur==-1); 
	else res=queue[cur--];
	release_mutex(&m);
	return res;
}

void init_using_mutex(void)
{
	init_mutex(&m);
	enqueue_fn = &enqueue_using_mutex;
	dequeue_fn = &dequeue_using_mutex;
}

void fini_using_mutex(void)
{
}


/*********************************************************************
 * TODO: Implement using semaphore
 */
void enqueue_using_semaphore(int value)
{
}

int dequeue_using_semaphore(void)
{
	return 0;
}

void init_using_semaphore(int S)
{
	enqueue_fn = &enqueue_using_semaphore;
	dequeue_fn = &dequeue_using_semaphore;
}

void fini_using_semaphore(void)
{
}


/*********************************************************************
 * Common implementation
 */
int init_ringbuffer(const int _nr_slots_, const enum lock_types _lock_type_)
{
	assert(_nr_slots_ > 0);
	nr_slots = _nr_slots_;

	/* Initialize lock! */
	lock_type = _lock_type_;
	switch (lock_type) {
	case lock_spinlock:
		init_using_spinlock();
		break;
	case lock_mutex:
		init_using_mutex();
		break;
	case lock_semaphore:
		init_using_semaphore(10);
		break;
	}

	/* TODO: Initialize your ringbuffer and synchronization mechanism */

	return 0;
}

void fini_ringbuffer(void)
{
	/* TODO: Clean up what you allocated */
	switch (lock_type) {
		case lock_spinlock:
		fini_using_spinlock();
		break;
	case lock_mutex:
		fini_using_mutex();
		break;
	case lock_semaphore:
		fini_using_semaphore();
		break;
	}
}