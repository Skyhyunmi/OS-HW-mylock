#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <sys/queue.h>
#include "config.h"
#include "locks.h"

static int nr_slots = 0;
static enum lock_types lock_type;

void (*enqueue_fn)(int value) = NULL;
int (*dequeue_fn)(void) = NULL;

TAILQ_HEAD(ringhead,entry) rhead = TAILQ_HEAD_INITIALIZER(rhead);
struct ringhead *rheadp;
struct entry{
	int val;
	TAILQ_ENTRY(entry) entries;
} *temp;
int tailqsize=0;

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
struct spinlock s;

void enqueue_using_spinlock(int value)
{
	do{
		acquire_spinlock(&s);
		if(tailqsize<nr_slots) break;
		release_spinlock(&s);
	}while(1);
	temp=malloc(sizeof(struct entry));
	temp->val=value;
	TAILQ_INSERT_TAIL(&rhead,temp,entries);
	tailqsize++;
	release_spinlock(&s);
}

int dequeue_using_spinlock(void)
{
	while(1) {
		acquire_spinlock(&s);
		if(!TAILQ_EMPTY(&rhead)) break;
		release_spinlock(&s);
	}
	temp = TAILQ_FIRST(&rhead);
	TAILQ_REMOVE(&rhead,temp,entries);
	tailqsize--;
	int res=temp->val;
	free(temp);
	release_spinlock(&s);
	return res;
}

void init_using_spinlock(void)
{
	init_spinlock(&s);
	enqueue_fn = &enqueue_using_spinlock;
	dequeue_fn = &dequeue_using_spinlock;
}

void fini_using_spinlock(void)
{
	struct entry *n1 = TAILQ_FIRST(&rhead);
	while (n1 != NULL) {
        struct entry *n2 = TAILQ_NEXT(n1, entries);
        free(n1);
        n1 = n2;
	}
	TAILQ_INIT(&rhead);
}

/*********************************************************************
 * TODO: Implement using mutex
 */
struct mutex m;
void enqueue_using_mutex(int value)
{
	do{
		acquire_mutex(&m);
		if(tailqsize<nr_slots) break;
		release_mutex(&m);
	}while(1);
	temp=malloc(sizeof(struct entry));
	temp->val=value;
	TAILQ_INSERT_TAIL(&rhead,temp,entries);
	tailqsize++;
	release_mutex(&m);
}

int dequeue_using_mutex(void)
{
	while(1) {
		acquire_mutex(&m);
		if(!TAILQ_EMPTY(&rhead)) break;
		release_mutex(&m);
	}
	temp = TAILQ_FIRST(&rhead);
	TAILQ_REMOVE(&rhead,temp,entries);
	tailqsize--;
	int res=temp->val;
	free(temp);
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
	struct entry *n1 = TAILQ_FIRST(&rhead);
	while (n1 != NULL) {
        struct entry *n2 = TAILQ_NEXT(n1, entries);
        free(n1);
        n1 = n2;
	}
	TAILQ_INIT(&rhead);
}


/*********************************************************************
 * TODO: Implement using semaphore
 */
struct semaphore se;
struct spinlock sl;
void enqueue_using_semaphore(int value)
{
	do{
		wait_semaphore(&se);
		if(tailqsize<nr_slots) break;
		signal_semaphore(&se);
	}while(1);
	acquire_spinlock(&sl);
	temp=malloc(sizeof(struct entry));
	temp->val=value;
	TAILQ_INSERT_TAIL(&rhead,temp,entries);
	tailqsize++;
	release_spinlock(&sl);
	signal_semaphore(&se);
}

int dequeue_using_semaphore(void)
{
	while(1) {
		wait_semaphore(&se);
		if(!TAILQ_EMPTY(&rhead)) break;
		signal_semaphore(&se);
	}
	acquire_spinlock(&sl);
	temp = TAILQ_FIRST(&rhead);
	TAILQ_REMOVE(&rhead,temp,entries);
	tailqsize--;
	int res=temp->val;
	free(temp);
	release_spinlock(&sl);
	signal_semaphore(&se);
	return res;
}

void init_using_semaphore(void)
{
	init_spinlock(&sl);
	init_semaphore(&se,10);//10으로 설정함.
	enqueue_fn = &enqueue_using_semaphore;
	dequeue_fn = &dequeue_using_semaphore;
}

void fini_using_semaphore(void)
{
	struct entry *n1 = TAILQ_FIRST(&rhead);
	while (n1 != NULL) {
        struct entry *n2 = TAILQ_NEXT(n1, entries);
        free(n1);
        n1 = n2;
	}
	TAILQ_INIT(&rhead);
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
		init_using_semaphore();
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