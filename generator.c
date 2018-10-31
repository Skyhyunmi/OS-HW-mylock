#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "config.h"
#include "generator.h"
#include "ringbuffer.h"

static int nr_generators = 0;
static unsigned long nr_generate = 0;

static int running = 0;

struct generator {
	pthread_t thread;
	int tid;
	int type;
	int arg0;
	int arg1;
	int arg2;
};
static struct generator *generators = NULL;

void *generator_main(void *_args_)
{
	unsigned long i;
	struct generator *my = (struct generator *)_args_;

	printf("Generator %d started...\n", my->tid);
	for (i = 0; i < nr_generate && running; i++) {
		int value = 0;
		switch(my->type) {
		case generator_single:
			value = my->arg0;
			break;
		case generator_random:
			value = MIN_VALUE + random() % (MAX_VALUE - MIN_VALUE);
			break;
		}
		printf("  %d generates %d\n", my->tid, value);
		enqueue_ringbuffer((int)value);
	}
	printf("Generator %d finished...\n", my->tid);
	return 0;
}

int spawn_generators(const int _nr_generators_, const unsigned long _nr_generate_)
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
		// g->type = generator_single;
		g->type = generator_random;
		g->arg0 = 0;
		pthread_create(&g->thread, NULL, generator_main, g);
	}
	return 0;
}

void fini_generators(void)
{
	int i;
	if (running) {
		running = 0;
		for (i = 0; i < nr_generators; i++) {
			struct generator *g = generators + i;
			if ((unsigned long)(g->thread) != 0) {
				pthread_join(g->thread, NULL);
			}
		}
		free(generators);
	}
}
