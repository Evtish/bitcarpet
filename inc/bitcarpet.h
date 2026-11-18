#pragma once

#include <stdbool.h>
#include <stdio.h>

#define BITCARPET_COLOR_N 3

struct bitcarpet_t {
	FILE *fs;			 // File stream, typically an image.
	unsigned int w, h;	 // Width and height of an image in cells.
	unsigned int dx, dy; // Width and height of a cell in pixels.
	bool is_inverted;	 // Whether to switch blank and filled colors.
};

enum bitcarpet_err_t {
	BITCARPET_E_OK, // Success.
	BITCARPET_E_IO, // IO error.
	BITCARPET_E_OVF // Overflow error.
};

enum bitcarpet_err_t bitcarpet_write_bw(const struct bitcarpet_t *const bc);
enum bitcarpet_err_t bitcarpet_write_color(const struct bitcarpet_t *const bc,
										   const unsigned char *const blank,
										   const unsigned char *const filled);
