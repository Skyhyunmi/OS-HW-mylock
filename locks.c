#include <stdio.h>

#include "config.h"
#include "locks.h"

struct lock_ops {
	void (*init)(struct lock *, int arg0, int arg1);
	void (*acquire)(struct lock *);
	void (*release)(struct lock *);
};

/******************************************************************
 * Spinlock implementation
 */
static void init_spinlock(struct lock *lock, int arg0, int arg1)
{
	lock->spinlock.A = arg0;
	return;
}

static void acquire_spinlock(struct lock *lock)
{
	return;
}

static void release_spinlock(struct lock *lock)
{
	return;
}

static struct lock_ops lock_ops_spinlock = {
	.init = init_spinlock,
	.acquire = acquire_spinlock,
	.release = release_spinlock,
};


/******************************************************************
 * Blocking lock implementation
 */
static void init_blocking(struct lock *lock, int arg0, int arg1)
{
	lock->blocking.B = arg0; 
	return;
}

static void acquire_blocking(struct lock *lock)
{
	return;
}

static void release_blocking(struct lock *lock)
{
	return;
}

static struct lock_ops lock_ops_blocking = {
	.init = init_blocking,
	.acquire = acquire_blocking,
	.release = release_blocking,
};


/******************************************************************
 * Semaphore implementation
 */
static void init_semaphore(struct lock *lock, int arg0, int arg1)
{
	lock->semaphore.C = arg1;
	return;
}

static void acquire_semaphore(struct lock *lock)
{
	return;
}

static void release_semaphore(struct lock *lock)
{
	return;
}

static struct lock_ops lock_ops_semaphore = {
	.init = init_semaphore,
	.acquire = acquire_semaphore,
	.release = release_semaphore,
};



/******************************************************************
 * Common implementation
 */
void init_lock(struct lock *lock, enum lock_types type, int arg0, int arg1)
{
	lock->type = type;
	switch(type) {
	case lock_spinlock:
		lock->operations = &lock_ops_spinlock;
		break;
	case lock_blocking:
		lock->operations = &lock_ops_blocking;
		break;
	case lock_semaphore:
		lock->operations = &lock_ops_semaphore;
		break;
	}

	lock->operations->init(lock, 0, 0);
}

void acquire_lock(struct lock *lock)
{
	// printf("Lock acquired\n");
	lock->operations->acquire(lock);
	return;
}

void release_lock(struct lock *lock)
{
	// printf("Lock released\n");
	lock->operations->release(lock);
	return;
}
