#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

#include "config.h"
#include "locks.h"
#include "atomic.h"

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
 *
 * Hint: Use pthread_self, pthread_kill, pause, and signal
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
 *
 * Hint: Use pthread_self, pthread_kill, pause, and signal
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


/******************************************************************
 * Spinlock tester exmaple
 */
struct spinlock testlock;
int testlock_held = 0;

void *test_thread(void *_arg_)
{
	usleep(random() % 1000 * 1000);

	printf("Tester acquiring the lock...\n");
	acquire_spinlock(&testlock);
	printf("Tester acquired\n");
	assert(testlock_held == 0);
	testlock_held = 1;

	sleep(1);

	printf("Tester releases the lock\n");
	testlock_held = 0;
	release_spinlock(&testlock);
	printf("Tester released the lock\n");
	return 0;
}

void test_lock(void)
{
	/* Set nr_testers as you need
	 *  1: one main, one tester. easy :-)
	 * 16: one main, 16 testers contending the lock :-$
	 */
	const int nr_testers = 1;
	int i;
	pthread_t tester[nr_testers];

	printf("Main initializes the lock\n");
	init_spinlock(&testlock);

	printf("Main graps the lock...");
	acquire_spinlock(&testlock);
	assert(testlock_held == 0);
	testlock_held = 1;
	printf("acquired!\n");

	for (i = 0; i < nr_testers; i++) {
		pthread_create(tester + i, NULL, test_thread, NULL);
	}

	sleep(1);

	printf("Main releases the lock\n");
	testlock_held = 0;
	release_spinlock(&testlock);
	printf("Main released the lock\n");

	for (i = 0; i < nr_testers; i++) {
		pthread_join(tester[i], NULL);
	}
	assert(testlock_held == 0);
	printf("Your spinlock implementation looks O.K.\n");

	return;
}

