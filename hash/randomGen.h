#pragma once
// Generates data for a Fibonacci heap

#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#include <math.h>

#define MAX_LEN (2000000)
#define MIN_LEN (100000)
#define DIF_LEN (100000)
#define BIAS (1000)
#define NAIVE_MAX (27)
#define NAIVE_MIN (1)
#define NAIVE_DIF (1)

/*
* This is the xoroshiro128+ random generator, designed in 2016 by David Blackman
* and Sebastiano Vigna, distributed under the CC-0 license. For more details,
* see http://vigna.di.unimi.it/xorshift/.
*/
static uint64_t rng_state[2];

static uint64_t rng_rotl(const uint64_t x, int k)
{
	return (x << k) | (x >> (64 - k));
}

static uint64_t rng_next_u64(void)
{
	uint64_t s0 = rng_state[0], s1 = rng_state[1];
	uint64_t result = s0 + s1;
	s1 ^= s0;
	rng_state[0] = rng_rotl(s0, 55) ^ s1 ^ (s1 << 14);
	rng_state[1] = rng_rotl(s1, 36);
	return result;
}

static uint32_t rng_next_u32(void)
{
	return rng_next_u64() >> 11;
}

static void rng_setup(unsigned int seed)
{
	rng_state[0] = seed * 0xdeadbeef;
	rng_state[1] = seed ^ 0xc0de1234;
	for (int i = 0; i<100; i++)
		rng_next_u64();
}

/*** The RNG ends here ***/
