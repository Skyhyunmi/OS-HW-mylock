#ifndef __LOCKS_H__
#define __LOCKS_H__
enum lock_types;

/*************************************************
 * Spinlock
 */
struct spinlock {
	/* Fill this in */
};
void init_spinlock(struct spinlock *);
void acquire_spinlock(struct spinlock *);
void release_spinlock(struct spinlock *);


/*************************************************
 * Mutex
 */
struct mutex {
	/* Fill this in */
};
void init_mutex(struct mutex *);
void acquire_mutex(struct mutex *);
void release_mutex(struct mutex *);


/*************************************************
 * Semaphore
 */
struct semaphore {
	/* Fill this in */
};
void init_semaphore(struct semaphore *, const int);
void wait_semaphore(struct semaphore *);
void signal_semaphore(struct semaphore *);

/*************************************************
 * Lock tester.
 * Will be invoked if the program is run with -T
 */
void test_lock(void);
#endif
