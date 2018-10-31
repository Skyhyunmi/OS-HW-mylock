#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "config.h"
#include "ringbuffer.h"
#include "generator.h"
#include "counter.h"

static enum generator_types generator_type = generator_single;
static int nr_generators = DEFAULT_NR_GENERATORS;
static unsigned long nr_generate = DEFAULT_NR_GENERATE;

static enum counter_types counter_type = counter_test0;

static int nr_slots = DEFAULT_NR_RINGBUFFER_SLOTS;
static enum lock_types lock_type = lock_spinlock;

int parse_command(int argc, char *argv[])
{
	char opt;
	while ((opt = getopt(argc, argv, "t:s:n:R:SM")) != -1) {
		switch(opt) {
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
		default:
			fprintf(stderr, "Usage: %s [-t generators] \n", argv[0]);
			fprintf(stderr, "             [-n # of request to generat]\n");
			fprintf(stderr, "             [-s ring buffer slots]\n");
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
