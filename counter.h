#ifndef __COUNTER_H__
#define __COUNTER_H__

enum counter_types {
	counter_normal = 0,
	counter_test0,
};

int spawn_counter(const enum counter_types, const unsigned long nr_requests);
void fini_counter(void);

#endif
