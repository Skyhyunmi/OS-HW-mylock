#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "config.h"
#include "ringbuffer.h"
#include "generator.h"
#include "counter.h"

static enum generator_types generator_type = generator_constant;
static int nr_generators = DEFAULT_NR_GENERATORS;
static unsigned long nr_generate = DEFAULT_NR_GENERATE;

static enum counter_types counter_type = counter_normal;

static int nr_slots = DEFAULT_NR_RINGBUFFER_SLOTS;
static enum lock_types lock_type = lock_spinlock;

int verbose = 1;

int parse_command(int argc, char *argv[])
{
	char opt;
	while ((opt = getopt(argc, argv, "qt:s:n:R:SM012h?")) != -1) {
		switch(opt) {
		case 'q':
			verbose = 0;
			break;
		case '0':
			generator_type = generator_random;
			nr_generators = 8;
			nr_generate = (1 << 24);
			counter_type = counter_normal;
			lock_type = lock_spinlock;
			break;
		case '1':
			generator_type = generator_random;
			nr_generators = 8;
			nr_generate = (1 >> 30);
			counter_type = counter_normal;
			lock_type = lock_mutex;
			break;
		case '2':
			generator_type = generator_random;
			nr_generators = 8;
			nr_generate = (1 >> 30);
			counter_type = counter_normal;
			lock_type = lock_semaphore;
			break;
		case 'R':
			srandom(atoi(optarg));
			break;
		case 't':
			nr_generators = atoi(optarg);
			break;
		case 'n':
			nr_generate = atoll(optarg);
			break;
		case 's':
			nr_slots = atoi(optarg);
			break;
		case 'S':
			lock_type = lock_semaphore;
			break;
		case 'M':
			lock_type = lock_mutex;
			break;
		case 'h':
		case '?':
		default:
			fprintf(stderr, "Usage: %s [-t generators] \n", argv[0]);
			fprintf(stderr, "             [-n # of request for a generator to generate]\n");
			fprintf(stderr, "             [-s ring buffer slots]\n");
			fprintf(stderr, "             [-M]  /* Test mutex */\n");
			fprintf(stderr, "             [-S]  /* Test semaphore */\n");
			return -EINVAL;
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int retval = 0;

	if ((retval = parse_command(argc, argv))) {
		goto exit;
	}
	if (retval = init_ringbuffer(nr_slots, lock_type)) {
		goto exit;
	}

	if ((retval = spawn_counter(counter_type, nr_generators * nr_generate))) {
		goto exit_ring;
	}

	spawn_generators(generator_type, nr_generators, nr_generate);

	fini_generators();
	fini_counter();

exit_ring:
	fini_ringbuffer();
exit:
	return retval;
}
