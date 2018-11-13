#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/queue.h>
#include <signal.h>
#include "config.h"
#include "locks.h"
#include "atomic.h"

/******************************************************************
 * Spinlock implementation
 */
void init_spinlock(struct spinlock *lock)
{
    lock->locked=0;
    return;
}

void acquire_spinlock(struct spinlock *lock)
{
    while(1==compare_and_swap(&lock->locked,0,1));
    return;
}

void release_spinlock(struct spinlock *lock)
{
    lock->locked=0;
    return;
}


/******************************************************************
 * Blocking lock implementation
 *
 * Hint: Use pthread_self, pthread_kill, pause, and signal
 */
TAILQ_HEAD(tailhead,entry) head = TAILQ_HEAD_INITIALIZER(head);
struct tailhead *headp;
struct entry{
	pthread_t self;
	TAILQ_ENTRY(entry) entries;
}*t;

void init_mutex(struct mutex *lock)
{
	lock->locked=0;
	init_spinlock(&lock->listsafety);
	TAILQ_INIT(&head);
	return;
}

void acquire_mutex(struct mutex *lock)
{
	signal(SIG_UNBLOCK,SIG_IGN);
	if(lock->locked){
	t=malloc(sizeof(struct entry));
	t->self=pthread_self();
		acquire_spinlock(&lock->listsafety);
			TAILQ_INSERT_TAIL(&head,t,entries);
			printf("acquire %d\n",pthread_self());
		release_spinlock(&lock->listsafety);
		pause();
	}
	compare_and_swap(&(lock->locked),0,1);	
}

void release_mutex(struct mutex *lock)
{
	if(!TAILQ_EMPTY(&head)){
		acquire_spinlock(&lock->listsafety);
		struct entry *tmp = TAILQ_FIRST(&head);
			TAILQ_REMOVE(&head,tmp,entries);
			printf("release %d\n",tmp->self);
		release_spinlock(&lock->listsafety);
		lock->locked=0;
		pthread_kill(tmp->self,SIG_UNBLOCK);
		//free(tmp);
	}
	lock->locked=0;
	//return;
}


/******************************************************************
 * Semaphore implementation
 *
 * Hint: Use pthread_self, pthread_kill, pause, and signal
 */
void init_semaphore(struct semaphore *sem, int S)
{
	sem->S=S;
	return;
}

void wait_semaphore(struct semaphore *sem)
{
	sem->S--;
	if(sem->S<0){
		pause();
	}
	
	return;
}

void signal_semaphore(struct semaphore *sem)
{
	sem->S++;
	if(sem->S<=0){
		//signal(SIG_UNBLOCK,process);
	}
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
	const int nr_testers = 16;
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

