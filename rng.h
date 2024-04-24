/* rng.h
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

struct RNG;

struct RNG *rng_create(void);
void rng_init(struct RNG *rng);
void rng_destroy(struct RNG *rng);
int32_t rng_generate(struct RNG *rng);
int32_t rng_generate_k(struct RNG *rng, int32_t k);
int32_t rng_generate_l_r(struct RNG *rng, int32_t l, int32_t r);
