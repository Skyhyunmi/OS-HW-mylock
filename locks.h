#ifndef __LOCKS_H__
#define __LOCKS_H__
enum lock_types;
struct lock_ops;

struct spinlock {
	int A;
};
struct blocking {
	int B;
};

struct semaphore {
	int C;
};

struct lock {
	enum lock_types type;
	struct lock_ops *operations;
	union {
		struct spinlock spinlock;
		struct blocking blocking;
		struct semaphore semaphore;
	};
};

/* Initialize the lock @lock */
void init_lock(struct lock *lock, enum lock_types type, int arg0, int arg1);

/* Acquire the lock @lock */
void acquire_lock(struct lock *lock);

/* Release the lock @lock. Should be held before calling this function */
void release_lock(struct lock *lock);
#endif
