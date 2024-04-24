/* rng.c
 *
 * Copyright (C) 2024  Pranjal Kole <pranjal.kole7@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "misc.h"

struct RNG {
	int32_t a;
	int32_t X;
	int32_t q;
	int32_t r;
	int32_t m;
};

/**
 * rng_create:
 *
 * Create an struct RNG.
 *
 * Returns: A pointer to an `struct RNG`
 **/
struct RNG *rng_create(void) {
	struct RNG *rng = malloc(sizeof(struct RNG));
	if (rng == NULL) {
		perror("Could not allocate memory");
		exit(EXIT_FAILURE);
	}
	return rng;
}

/**
 * rng_init:
 *
 * @rng: A created struct RNG
 *
 * Initialise @rng with a linear congruential random number generator.
 *  
 **/
void rng_init(struct RNG *rng) {
	/* We are using a linear congruential random number generator
	 * variant for C as given in TAoCP Vol 2.
	 *
	 * X <- aX mod m
	 *
	 * `a` should be a primitive root of `m`
	 * `X` should be a random number
	 * `q` is `m` / `a`
	 * `r` is `m` % `a`
	 * `m` should be a prime number
	 */
	/* An `a` of 48271 passes the spectral test */
	rng->a = 48271;

	srand(time(NULL));
	/* Seed our struct RNG with the kernel's struct RNG */
	int32_t X = 0;
	while (X <= 0 || X >= 2147483647) {
		X = rand();
		/*if (getrandom(&X, 4, 0) == -1) {
			perror("Could not get random bytes");
			exit(EXIT_FAILURE);
		}*/
	}
	rng->X = X;

	rng->q = 44488;
	rng->r = 3399;

	/* A mersenne prime - 2^31 - 1 */
	rng->m = 2147483647;
}

/**
 * rng_destroy:
 *
 * @rng: An struct RNG
 *
 * Destroys the linear congruential random number generator created
 * by a previous call to `rng_create`.
 *
 **/
void rng_destroy(struct RNG *rng) {
	free(rng);
}

/**
 * rng_generate:
 *
 * @rng: An struct RNG
 *
 * Returns: A random integer in [0,2147483647)
 *  
 **/
int32_t rng_generate(struct RNG *rng) {
	int32_t X = rng->X;
	X = rng->a * (X % rng->q) - rng->r * (X / rng->q);

	if (X < 0) {
		X += rng->m;
	}

	rng->X = X;
	return X;
}

/**
 * rng_generate_k:
 *
 * @rng: An struct RNG
 *
 * Returns: A random integer in [0,k)
 *  
 **/
int32_t rng_generate_k(struct RNG *rng, int32_t k) {
	int32_t X = rng_generate(rng);
	int32_t m = rng->m;

	while (X >= (m - (m % k))) {
		X = rng_generate(rng);
	}

	// DEBUG("X: %i, k: %i, m: %i", X, k, m);
	return ((uint64_t)(X) * (uint64_t)(k)) / ((uint64_t)(m-1));
}

/**
 * rng_generate_l_r:
 *
 * Returns: A random integer in [l,r)
 *  
 **/
int32_t rng_generate_l_r(struct RNG *rng, int32_t l, int32_t r) {
	return l + rng_generate_k(rng, r - l);
}
