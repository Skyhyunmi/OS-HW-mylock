#include <stdio.h>
#include <pthread.h>

#include "config.h"
#include "ringbuffer.h"

static unsigned long nr_requests = 0;
static pthread_t counter_thread;
static unsigned long occurrances[MAX_VALUE] = { 0 };

static int __initialized = 0;

void *counter_main(void *);

int spawn_counter(const unsigned long _nr_requests_)
{
	nr_requests = _nr_requests_;
	pthread_create(&counter_thread, NULL, counter_main, NULL);
	__initialized = 1;
}

void fini_counter(void)
{
	if (__initialized) {
		pthread_join(counter_thread, NULL);
	}
}

static void dump_counting_result(void)
{
	int i;
	unsigned long nr = 0;
	for (i = 0; i < MAX_VALUE; i++) {
		if (occurrances[i] == 0) {
			continue;
		}
		printf("  %3d : %lu\n", i, occurrances[i]);
		nr += occurrances[i];
	}
	printf("Total : %d\n", nr);
}

void *counter_main(void *_args_)
{
	unsigned int i;
	printf("Counter started...\n");
	for (i = 0; i < nr_requests; i++) {
		int value = dequeue_ringbuffer();
		if (value < 0 || value > MAX_VALUE) {
			fprintf(stderr, "Dequeued wrong value %d\n", value);
			break;
		}
		// printf("Dequeue %d\n", value);
		occurrances[value]++;
	}
	printf("Counter finished...\n");
	dump_counting_result();
	return 0;
}
