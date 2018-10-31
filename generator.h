#ifndef __GENERATOR_H__
#define __GENERATOR_H__

enum generator_types {
	generator_unknown = 0,
	generator_random,
	generator_constant,
	generator_test0,
	generator_test1,
	generator_test2,
};

int spawn_generators(const enum generator_types, const int nr_generators, const unsigned long nr_generate);
void fini_generators(void);

#endif
