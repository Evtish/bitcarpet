#pragma once

#include <stdio.h>

#define BITCARPET_COLOR_N 3

enum bitcarpet_err_t {
	BITCARPET_E_OK, // Success.
	BITCARPET_E_IO, // IO error.
	BITCARPET_E_OVF // Overflow error.
};

enum bitcarpet_err_t bitcarpet_write_bw(FILE *fs, unsigned int w,
										unsigned int h, unsigned int cell_dx,
										unsigned int cell_dy);
enum bitcarpet_err_t
bitcarpet_write_fancy(FILE *fs, unsigned int w, unsigned int h,
					  unsigned int cell_dx, unsigned int cell_dy,
					  unsigned char blank[BITCARPET_COLOR_N],
					  unsigned char filled[BITCARPET_COLOR_N]);
