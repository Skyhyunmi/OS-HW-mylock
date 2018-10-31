#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "config.h"
#include "ringbuffer.h"
#include "counter.h"

static unsigned long nr_requests = 0;
static pthread_t counter_thread;
static unsigned long occurrances[MAX_VALUE] = { 0 };

static int __initialized = 0;
static int delayed = 0;

void *counter_main(void *);

int spawn_counter(const enum counter_types type, const unsigned long _nr_requests_)
{
	nr_requests = _nr_requests_;
	if (type == counter_delayed) {
		delayed = 1;
	}
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
	FILE *fp = fopen(RESULT_FILENAME, "w");
	if (!fp) {
		fprintf(stderr, "Cannot create the result file\n");
	}
	
	if (verbose) {
		printf("Occurrances ------\n");
	}
	for (i = MIN_VALUE; i < MAX_VALUE; i++) {
		if (occurrances[i] == 0) {
			continue;
		}
		if (verbose) {
			printf("    %3d : %lu\n", i, occurrances[i]);
		}		
		fprintf(fp, "%d %d\n", i, occurrances[i]);
		nr += occurrances[i];
	}
	if (verbose) {
		printf("  Total : %d\n", nr);
	}

	if (fp) {
		fclose(fp);
	}
}

void *counter_main(void *_args_)
{
	unsigned long i;

	if (verbose) {
		printf("Counting %lu numbers...\n", nr_requests);
	}

	for (i = 0; i < nr_requests; i++) {
		int value = dequeue_ringbuffer();
		if (value < MIN_VALUE || value > MAX_VALUE) {
			fprintf(stderr, "Dequeued wrong value %d\n", value);
			break;
		}
		if (verbose && i && i % 1000000 == 0) {
			printf("%lu M / %lu M counted\n", (i >> 20), (nr_requests >> 20));
		}
		// printf("Dequeue %d\n", value);
		occurrances[value]++;
		if (delayed) usleep(100);
	}

	if (verbose) {
		printf("Counting finished...\n");
	}
	dump_counting_result();
	return 0;
}
