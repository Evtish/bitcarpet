#include <assert.h>
#include <limits.h>
#include <stdio.h>

#include "bitcarpet.h"

static unsigned int f(unsigned int x, unsigned int y) {
	return ((y + 1) * (y + 1) * (x + 1) >> 11) & 1;
}

// TODO: Return failure on multiplication overflow
// Writes a BW carpet to an image
//
// fs - image, must not be NULL
// w - image width in cells, must be positive
// h - image height in cells, must be positive
// cell_dx - cell width in pixels, must be positive
// cell_dy - cell height in pixels, must be positive
//
// Returns 0 on success and -1 on failure
int bitcarpet_write_bw(FILE *fs, unsigned int w, unsigned int h,
					   unsigned int cell_dx, unsigned int cell_dy) {
	unsigned int w_px, h_px, x, y;
	unsigned char bit_val, bit_pos, chunk = 0;
	assert(fs);
	assert(w > 0 && h > 0 && cell_dx > 0 && cell_dy > 0);
	w_px = w * cell_dx;
	h_px = h * cell_dy;
	if (fprintf(fs, "P4\n%u %u\n", w_px, h_px) < 0)
		return -1;
	for (y = 0; y < h_px; ++y) {
		for (x = 0; x < w_px; ++x) {
			bit_val = f(x / cell_dx, y / cell_dy);
			bit_pos = CHAR_BIT - (x % CHAR_BIT) - 1;
			chunk |= (bit_val << bit_pos);
			if (x % CHAR_BIT == CHAR_BIT - 1 || x + 1 >= w_px) {
				if (fputc(chunk, fs) == EOF && ferror(fs))
					return -1;
				chunk = 0;
			}
		}
	}
	return 0;
}

// TODO: Return failure on multiplication overflow
// Writes a colored carpet to an image
//
// fs - image, must not be NULL
// w - image width in cells, must be positive
// h - image height in cells, must be positive
// cell_dx - cell width in pixels, must be positive
// cell_dy - cell height in pixels, must be positive
// blank - color for blank cells
// filled - color for filled cells
//
// Returns 0 on success and -1 on failure
int bitcarpet_write_fancy(FILE *fs, unsigned int w, unsigned int h,
						  unsigned int cell_dx, unsigned int cell_dy,
						  unsigned char blank[BITCARPET_COLOR_N],
						  unsigned char filled[BITCARPET_COLOR_N]) {
	int n;
	unsigned int w_px, h_px, x, y;
	unsigned char *color;
	assert(fs);
	assert(w > 0 && h > 0 && cell_dx > 0 && cell_dy > 0);
	w_px = w * cell_dx;
	h_px = h * cell_dy;
	if (fprintf(fs, "P6\n%u %u\n255\n", w_px, h_px) < 0)
		return -1;
	for (y = 0; y < h_px; ++y) {
		for (x = 0; x < w_px; ++x) {
			if (f(x / cell_dx, y / cell_dy))
				color = filled;
			else
				color = blank;
			n = fwrite(color, sizeof(*color), BITCARPET_COLOR_N, fs);
			if (n < BITCARPET_COLOR_N)
				return -1;
		}
	}
	return 0;
}
