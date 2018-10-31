#include <stdio.h>

#include "config.h"
#include "locks.h"

/******************************************************************
 * Spinlock implementation
 */
void init_spinlock(struct spinlock *lock)
{
	return;
}

void acquire_spinlock(struct spinlock *lock)
{
	return;
}

void release_spinlock(struct spinlock *lock)
{
	return;
}


/******************************************************************
 * Blocking lock implementation
 */
void init_mutex(struct mutex *lock)
{
	return;
}

void acquire_mutex(struct mutex *lock)
{
	return;
}

void release_mutex(struct mutex *lock)
{
	return;
}


/******************************************************************
 * Semaphore implementation
 */
void init_semaphore(struct semaphore *sem, int S)
{
	return;
}

void acquire_semaphore(struct semaphore *sem)
{
	return;
}

void release_semaphore(struct semaphore *sem)
{
	return;
}
