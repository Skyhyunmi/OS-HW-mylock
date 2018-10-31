#ifndef __LOCKS_H__
#define __LOCKS_H__
enum lock_types;
struct lock_ops;

struct spinlock {
};
void init_spinlock(struct spinlock *);
void acquire_spinlock(struct spinlock *);
void release_spinlock(struct spinlock *);


struct mutex {
};
void init_mutex(struct mutex *);
void acquire_mutex(struct mutex *);
void release_mutex(struct mutex *);


struct semaphore {
};
void init_semaphore(struct semaphore *, const int);
void acquire_semaphore(struct semaphore *);
void release_semaphore(struct semaphore *);

#endif
