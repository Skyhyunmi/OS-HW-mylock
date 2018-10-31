#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <assert.h>

#include "config.h"
#include "generator.h"
#include "ringbuffer.h"

static int nr_generators = 0;
static unsigned long nr_generate = 0;

static int running = 0;

int generator_fn_single(int tid)
{
	return 43;
}

int generator_fn_random(int tid)
{
	return MIN_VALUE + (random() % (MAX_VALUE - MIN_VALUE));
}

int generator_fn_sleep(int tid)
{
	sleep(1);
	return 43;
}

struct generator {
	pthread_t thread;
	int tid;
	int (*generator_fn)(int tid);
	FILE *fp;
};
static struct generator *generators = NULL;


int (*assign_generator_fn(enum generator_types type, int tid))(int)
{
	switch(type) {
	case generator_single:
		return &generator_fn_single;
	case generator_random:
		return &generator_fn_random;
	case generator_test0:
		return &generator_fn_sleep;
	default:
		return NULL;
	}
}


void *generator_main(void *_args_)
{
	unsigned long i;
	struct generator *my = (struct generator *)_args_;
	char filename[80];

	printf("Generator %d started...\n", my->tid);
	snprintf(filename, sizeof(filename), GENERATOR_FILENAME, my->tid);
	my->fp = fopen(filename, "w");
	assert(my->fp);

	for (i = 0; i < nr_generate && running; i++) {
		int value;
		assert(my->generator_fn);
		value = my->generator_fn(my->tid);
		//printf("  %d generates %d\n", my->tid, value);
		enqueue_ringbuffer((int)value);
		fprintf(my->fp, "%d\n", value);
	}
	printf("Generator %d finished...\n", my->tid);
	fclose(my->fp);
	return 0;
}

int spawn_generators(const enum generator_types type, const int _nr_generators_, const unsigned long _nr_generate_)
{
	int i;

	if (_nr_generators_ <= 0 || _nr_generators_ > 256) {
		return -EINVAL;
	}
	nr_generators = _nr_generators_;

	if (_nr_generate_ <= 0) {
		return -EINVAL;
	}
	nr_generate = _nr_generate_;

	generators = calloc(nr_generators, sizeof(*generators));
	if (!generators) {
		return -ENOMEM;
	}
	bzero(generators, sizeof(*generators) * nr_generators);

	running = 1;
	for (i = 0; i < nr_generators; i++) {
		struct generator *g = generators + i;
		g->tid = i;
		g->generator_fn = assign_generator_fn(type, i);
		pthread_create(&g->thread, NULL, generator_main, g);
	}
	return 0;
}

void fini_generators(void)
{
	int i;
	if (running) {
		for (i = 0; i < nr_generators; i++) {
			struct generator *g = generators + i;
			if ((unsigned long)(g->thread) != 0) {
				pthread_join(g->thread, NULL);
			}
		}
		free(generators);
	}
}
