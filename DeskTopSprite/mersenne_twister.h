#pragma once

#define __N__ 624

typedef struct {
	unsigned long v[__N__]; /* the array for the state vector  */
	int i;
} MersenneTwister;

double genrand_res53(MersenneTwister *o);
double genrand_real2(MersenneTwister *o);
MersenneTwister* newMT(unsigned long seed);

typedef double (*GEN_RAND_FUNC)(MersenneTwister* o);
