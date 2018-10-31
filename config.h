#ifndef _CONFIG_H_
#define _CONFIG_H_

enum lock_types {
	lock_spinlock = 0,
	lock_blocking = 1,
	lock_semaphore = 2,
};

#define MIN_VALUE 0
#define MAX_VALUE 128

#define DEFAULT_NR_GENERATORS 1
#define DEFAULT_NR_GENERATE 16
#define DEFAULT_NR_RINGBUFFER_SLOTS 16

#endif
