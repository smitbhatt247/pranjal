/* main.c
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
#include <sys/random.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#define BOARD_SIZE 4
#define DEBUG(format, ...) 42//printf("[DEBUG] %s(%s:%i) " format "\n", __func__ ,__FILE__, __LINE__, __VA_ARGS__)

struct Game {
	struct Board *board;
	int score;
};

/**
 * generate_k:
 *
 * Returns: A random integer in [0,k)
 *  
 **/
int32_t generate_k(int32_t k) {
	return rand() % k;
}

/**
 * generate_l_r:
 *
 * Returns: A random integer in [l,r)
 *  
 **/
int32_t generate_l_r(int32_t l, int32_t r) {
	return l + generate_k(r - l);
}

/* Represents a single tile in the board */
struct tile {
	/* Value is stored as log2(value) in the struct. This is
	 * because value is a power of 2, so log2(value) will have the
	 * same precision and we can store larger numbers in the same
	 * amount of memory. This is not useful for a BOARD_SIZE of 4,
	 * but will be useful for larger BOARD_SIZEs */
	int log2value;
};

struct tile_coord {
	int row;
	int column;
};

/* Board START */
struct Board {
	/* A 2D array of tiles to represent the board */
	struct tile **tiles;
};

/**
 * board_create:
 *
 * Creates a new board.
 *
 * Returns: A board
 *  
 **/
struct Board *board_create(void) {
	struct Board *board = malloc(sizeof(struct Board));
	DEBUG("board: %p", board);
	if (board == NULL) {
		perror("Could not allocate memory");
		exit(EXIT_FAILURE);
	}

	struct tile **tiles = malloc(BOARD_SIZE * sizeof(struct tile *));
	DEBUG("tiles: %p", tiles);
	if (tiles == NULL) {
		perror("Could not allocate memory");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < BOARD_SIZE; i++) {
		tiles[i] = malloc(BOARD_SIZE * sizeof(struct tile));
		DEBUG("tiles[%i]: %p", i, tiles[i]);
		if (tiles[i] == NULL) {
			perror("Could not allocate memory");
			exit(EXIT_FAILURE);
		}
	}
	board->tiles = tiles;
	return board;
}

/**
 * board_init:
 *
 * @board: A board
 *
 * Initialises a created board according to the rules of 2048.
 *  
 **/
void board_init(struct Board *board) {
	struct tile **tiles = board->tiles;
	DEBUG("tiles: %p", tiles);
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			tiles[i][j].log2value = 0;
		}
	}

	int first_tile_value = generate_l_r(1, 3);
	int second_tile_value = generate_l_r(1, 3);

	int x = generate_k(4);
	int y = generate_k(4);
	tiles[x][y].log2value = first_tile_value;
	while (tiles[x][y].log2value != 0) {
		x = generate_k(4);
		y = generate_k(4);
	}
	tiles[x][y].log2value = second_tile_value;
	board->tiles = tiles;
}

/**
 * board_destroy:
 *
 * @board: A board
 *
 * Destroy a @board created by a previous call to board_create.
 *  
 **/
void board_destroy(struct Board *board) {
	DEBUG("board: %p", board);
	for (int i = 0; i < BOARD_SIZE; i++) {
		free(board->tiles[i]);
	}
	free(board->tiles);
	free(board);
}
/* Board END */

/**
 * max:
 * 
 * @a: A number
 * @b: A number
 *
 * Returns: The maximum number among @a and @b.
 *
 **/
int max(int a, int b) {
	return a > b ? a : b;
}

/**
 * count_digits:
 *
 * @num: An integer
 *
 * Returns: The number of digits in @num
 **/
int count_digits(int num) {
	int digits = 0;
	while (num) {
		num /= 10;
		digits++;
	}
	return digits;
}


/**
 * board_print:
 *
 * @board: A `board`
 *
 * Prints the formatted @board to stdout.
 *  
 **/
void board_print(struct Board *board) {
	/* */
	int max_width[4] = { 0, };
	for (int j = 0; j < BOARD_SIZE; j++) {
		for (int i = 0; i < BOARD_SIZE; i++) {
			max_width[j] = 4;//max(max_width[j], count_digits(1 << board->tiles[i][j].log2value));
		}
		DEBUG("max_width[%i]: %i", j, max_width[j]);
	}
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board->tiles[i][j].log2value == 0) {
				printf("0");
				for (int i = 0; i < max_width[j]-1; i++) {
					putchar(' ');
				}
			} else {
				// DEBUG("count_digits(%i): %i", 1 << board->tiles[i][j].log2value, count_digits(1 << board->tiles[i][j].log2value));
				// DEBUG("max_width[%i]: %i", j, max_width[j]);
				// DEBUG("max_width[j]-count_digits(1 << board->tiles[i][j].log2value: %i", max_width[j]-count_digits(1 << board->tiles[i][j].log2value));
				/*if (j == BOARD_SIZE-1) {
					printf("%i", 1 << board->tiles[i][j].log2value);
					for (int i = 0; i < max_width[j]-count_digits(1 << board->tiles[i][j].log2value); i++) {
						putchar(' ');
					}
					// putchar('\n');
					// printf(" ", count_digits(board->tiles[i][j].log2value));
				} else {*/
				printf("%i", 1 << board->tiles[i][j].log2value);
				for (int k = 0; k < max_width[j]-count_digits(1 << board->tiles[i][j].log2value); k++) {
					putchar(' ');
				}
				// }
			}
			putchar(' ');
		}
		putchar('\n');
	}
}

/**
 * get_num_adjacent_tiles:
 *
 * @board: A `board`
 * @coord: A `tile_coord`
 *
 * Retrieves the number of adjacent tiles to the tile at @coord. If the
 * coordinates are outside the bounds of the board then it will return
 * -1
 *
 * The coordinate system of the @board is similar to that of a spreadsheet. The
 * origin is the upper left corner. Rows increase vertically downward, and
 * columns increase horizontally.
 *  
 * Returns: The number of tiles adjacent to the tile at @coord.
 **/
int get_num_adjacent_tiles(
		struct tile **tiles,
		struct tile_coord coord) {
	int row;
	row = coord.row;

	int column;
	column = coord.column;

	if (row < 0 || row >= BOARD_SIZE || column < 0 || column >= BOARD_SIZE) {
		DEBUG("out of bounds (%i, %i)", row, column);
		exit(EXIT_FAILURE);
	}

	int num_adjacent = 4;

	if (row == 0 || row == BOARD_SIZE-1) {
		num_adjacent--;
	}

	if (column == 0 || column == BOARD_SIZE-1) {
		num_adjacent--;
	}

	return num_adjacent;
}

/**
 * get_adjacent_tiles:
 *
 * @board: A `board`
 * @coord: A `tile_coord`
 *
 * Retrieves the tiles adjacent to the tiles at @coord. If the
 * coordinates are outside the bounds of the board then it will return
 * %NULL
 *
 * The coordinate system of the @board is similar to that of a spreadsheet. The
 * origin is the upper left corner. Rows increase vertically downward, and
 * columns increase horizontally.
 *  
 * Returns: The tiles adjacent to the tile at @coord.
 **/
struct tile *get_adjacent_tiles(
		struct Board *board,
		struct tile_coord coord) {
	int row;
	row = coord.row;

	int column;
	column = coord.column;

	if (row < 0 || row >= BOARD_SIZE || column < 0 || column >= BOARD_SIZE) {
		DEBUG("out of bounds (%i, %i)", row, column);
		exit(EXIT_FAILURE);
	}

	struct tile *tiles = malloc(
		get_num_adjacent_tiles(
			board->tiles, coord) * sizeof(struct tile));
	int idx = 0;
	int adj_coord[][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };
	for (int i = 0; i < 4; i++) {
		int adj_row = row + adj_coord[i][0];
		int adj_col = column + adj_coord[i][1];
		if (adj_row < 0 || adj_row >= BOARD_SIZE || adj_col < 0 || adj_col >= BOARD_SIZE) {
			continue;
		}
		tiles[idx++] = board->tiles[adj_row][adj_col];
	}
	return tiles;
}

/**
 * game_over:
 *
 * @board: A `board`
 *
 * Returns: Whether the game is over for the given board.
 **/
bool game_over(struct Game *game) {
	struct Board *board = game->board;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			/* A log2value of 0 represents an empty tile,
			 * so the game is not over when it is 0 */
			if (board->tiles[i][j].log2value == 0) return false;
			struct tile *tiles = get_adjacent_tiles(board, (struct tile_coord){ i, j });
			for (int i = 0; i < get_num_adjacent_tiles(board->tiles, (struct tile_coord){ i, j }); i++) {
				DEBUG("adj to (%i, %i) - %i: %i", i, j, board->tiles[i][j].log2value, tiles[i].log2value);
			}
		}
	}
	return true;
}

/**
 * game_add_random_tile:
 *
 * @board: A `board`
 *
 * Modifies the board in-place to add a random tile which happens after
 * every move according to the rules of 2048.
 *  
 **/
void game_add_random_tile(struct Game *game) {
	struct Board *board = game->board;
	/* We want to add a random tile to the board, having a value of
	 * either 2 or 4 */
	int coords[BOARD_SIZE*BOARD_SIZE][2];
	int coords_idx = 0;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board->tiles[i][j].log2value == 0) {
				coords[coords_idx][0] = i;
				coords[coords_idx++][1] = j;
			}
		}
	}

	int tile_position = generate_k(coords_idx);
	int tile_pos[] = { coords[tile_position][0], coords[tile_position][1] };
	int tile_value = generate_l_r(1, 3);
	DEBUG("tile_pos: (%i, %i), tile_value: %i", tile_pos[0], tile_pos[1], tile_value);
	board->tiles[tile_pos[0]][tile_pos[1]].log2value = tile_value;
}

uint8_t findTarget(uint8_t array[BOARD_SIZE], uint8_t x, uint8_t stop)
{
        uint8_t t;
        // if the position is already on the first, don't evaluate
        if (x == 0)
        {
                return x;
        }
        for (t = x - 1;; t--)
        {
                if (array[t] != 0)
                {
                        if (array[t] != array[x])
                        {
                                // merge is not possible, take next position
                                return t + 1;
                        }
                        return t;
                }
                else
                {
                        // we should not slide further, return this one
                        if (t == stop)
                        {
                                return t;
                        }
                }
        }
        // we did not find a target
        return x;
}

/**
 * slide_array:
 *
 * @array: An `array`
 *
 * Slides the array @array in the UP direction according to the rules
 * of 2048.
 *  
 **/
bool slide_array(uint8_t array[BOARD_SIZE], uint32_t *score) {
	bool success = false;
	int t, stop = 0;

	for (int x = 0; x < BOARD_SIZE; x++) {
		DEBUG("array[%i]: %i", x, array[x]);
		if (array[x] != 0) {
			t = findTarget(array, x, stop);
			// if target is not original position, then move or merge
			if (t != x) {
				// if target is zero, this is a move
				if (array[t] == 0) {
					array[t] = array[x];
				} else if (array[t] == array[x]) {
					// merge (increase power of two)
					array[t]++;
					// increase score
					*score += (uint32_t)1 << array[t];
					// set stop to avoid double merge
					stop = t + 1;
				}
				array[x] = 0;
				success = true;
			}
		}
	}
	return success;
}


/**
 * rotate_board:
 *
 * @board: A `board`
 *
 * Rotates the board @board clockwise by modifying it in-place.
 *  
 **/
void rotate_board(struct Board *board) {
	struct tile **tiles = board->tiles;
        uint8_t i, j, n = BOARD_SIZE;
        uint8_t tmp;
        for (i = 0; i < n / 2; i++)
        {
                for (j = i; j < n - i - 1; j++)
                {
                        tmp = tiles[i][j].log2value;
                        tiles[i][j].log2value = tiles[j][n - i - 1].log2value;
                        tiles[j][n - i - 1].log2value = tiles[n - i - 1][n - j - 1].log2value;
                        tiles[n - i - 1][n - j - 1].log2value = tiles[n - j - 1][i].log2value;
                        tiles[n - j - 1][i].log2value = tmp;
                }
        }
}

/**
 * move_w:
 *
 * @board: A `board`
 *
 * Modifies the board in-place to make an UP move according to the
 * rules of 2048.
 *  
 **/
void move_w(struct Game *game) {
	struct Board *board = game->board;
	bool success = false;
	uint8_t arr[BOARD_SIZE];
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			arr[j] = (uint8_t)board->tiles[j][i].log2value;
			DEBUG("arr[%i]: %i", j, arr[j]);
		}
		success |= slide_array(arr, &game->score);
		for (int j = 0; j < BOARD_SIZE; j++) {
			board->tiles[j][i].log2value = arr[j];
		}
	}
}

/**
 * move_a:
 *
 * @board: A `board`
 *
 * Modifies the board in-place to make an LEFT move according to the
 * rules of 2048.
 *  
 **/
void move_a(struct Game *game) {
	struct Board *board = game->board;
	bool success = false;
	rotate_board(board);
	rotate_board(board);
	rotate_board(board);
	//uint8_t arr[BOARD_SIZE];
	move_w(game);
//	for (int i = 0; i < BOARD_SIZE; i++) {
//		for (int j = 0; j < BOARD_SIZE; j++) {
//			arr[j] = board->tiles[i][j].log2value;
///		}
//		success |= slide_array(arr, &score);
//		for (int j = 0; j < BOARD_SIZE; j++) {
//			board->tiles[i][j].log2value = arr[j];
//		}
//	}
	rotate_board(board);
}

/**
 * move_s:
 *
 * @board: A `board`
 *
 * Modifies the board in-place to make an DOWN move according to the
 * rules of 2048.
 *  
 **/
void move_s(struct Game *game) {
	struct Board *board = game->board;
	bool success = false;
	rotate_board(board);
	rotate_board(board);
	move_w(game);
	//uint8_t arr[BOARD_SIZE];
	/*for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			arr[j] = board->tiles[i][j].log2value;
		}
		success |= slide_array(arr, &score);
		for (int j = 0; j < BOARD_SIZE; j++) {
			board->tiles[i][j].log2value = arr[j];
		}
	}*/
	rotate_board(board);
	rotate_board(board);
}

/**
 * move_d:
 *
 * @board: A `board`
 *
 * Modifies the board in-place to make an RIGHT move according to the
 * rules of 2048.
 *  
 **/
void move_d(struct Game *game) {
	struct Board *board = game->board;
	bool success = false;
	rotate_board(board);
	/*uint8_t arr[BOARD_SIZE];
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			arr[j] = board->tiles[i][j].log2value;
		}
		success |= slide_array(arr, &score);
		for (int j = 0; j < BOARD_SIZE; j++) {
			board->tiles[i][j].log2value = arr[j];
		}
	}*/
	move_w(game);
	rotate_board(board);
	rotate_board(board);
	rotate_board(board);
}

struct Game *game_create(void) {
	struct Game *game = malloc(sizeof(struct Game));
	game->board = board_create();
	return game;
}

void game_init(struct Game *game) {
	board_init(game->board);
}

void game_render(struct Game *game) {
	printf("\nSCORE: %i\n\n", game->score);
	board_print(game->board);
	putchar('\n');
}

void game_destroy(struct Game *game) {
	board_destroy(game->board);
	free(game);
}

void processInput(struct Game *game) {
	char move;
	do {
		move = getchar();
	} while (move != 'w' && move != 'a' && move != 's' &&
		move != 'd' && move != 'p');

	switch (move) {
		case 'w':
			move_w(game);
			break;
		case 'a':
			move_a(game);
			break;
		case 's':
			move_s(game);
			break;
		case 'd':
			move_d(game);
			break;
		default:
			return;
	}

	game_add_random_tile(game);
}

int main(int argc, char **argv) {
	struct Game *game = game_create();
	game_init(game);

	puts("Welcome to the 2048 game");
	game_render(game);

	/* Main game loop */
	while (!game_over(game)) {
		/* This blocks until a character is received */
		processInput(game);
		game_render(game);
	}

	/* Freeing all the allocated memory */
	game_destroy(game);
	return EXIT_SUCCESS;
}
