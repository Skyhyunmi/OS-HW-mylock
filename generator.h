#ifndef __GENERATOR_H__
#define __GENERATOR_H__

enum generator_types {
	generator_unknown = 0,
	generator_random,
	generator_single,
};

int spawn_generators(const int nr_generators, const unsigned long nr_generate);
void fini_generators(void);

#endif
