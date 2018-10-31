#ifndef __COUNTER_H__
#define __COUNTER_H__
int spawn_counter(const unsigned long nr_requests);
void fini_counter(void);
void dump_counting_result(void);
#endif
