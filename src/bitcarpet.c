#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

#include "bitcarpet.h"

// TODO: Move the function to main.c and a field in bitcarpet_t for a function
// pointer.
static bool f(unsigned int x, unsigned int y) {
	return ((y + 1) * (y + 1) * (x + 1) >> 11) & 1;
}

// Check if a product of two unsigned ints will overflow.
static int umulovf(unsigned int a, unsigned int b) { return a > UINT_MAX / b; }

// Write a BW carpet to an image.
//
// bc.fs - must not be NULL.
// bc.w - must be positive.
// bc.h - must be positive.
// bc.dx - must be positive.
// bc.dy - must be positive.
//
// Returns the corresponding status code.
enum bitcarpet_err_t bitcarpet_write_bw(const struct bitcarpet_t *const bc) {
	unsigned int w_px, h_px;
	assert(bc->fs);
	assert(bc->w > 0 && bc->h > 0);
	assert(bc->dx > 0 && bc->dy > 0);
	if (umulovf(bc->w, bc->dx) || umulovf(bc->h, bc->dy))
		return BITCARPET_E_OVF;
	w_px = bc->w * bc->dx;
	h_px = bc->h * bc->dy;
	if (fprintf(bc->fs, "P4\n%u %u\n", w_px, h_px) < 0)
		return BITCARPET_E_IO;
	for (unsigned int y = 0; y < h_px; ++y) {
		unsigned char chunk = 0;
		for (unsigned int x = 0; x < w_px; ++x) {
			unsigned char bit_n = x % CHAR_BIT;
			unsigned char bit_shift = CHAR_BIT - bit_n - 1;
			bool pixel_val = f(x / bc->dx, y / bc->dy);
			if (bc->is_inverted)
				pixel_val = !pixel_val;
			chunk |= (pixel_val
					  << bit_shift); // TODO: Should i cast pixel_val to some
									 // unsigned type? Should i even use bool?
			if (bit_n == CHAR_BIT - 1 || x >= w_px - 1) {
				if (fputc(chunk, bc->fs) == EOF && ferror(bc->fs))
					return BITCARPET_E_IO;
				chunk = 0;
			}
		}
	}
	return BITCARPET_E_OK;
}

// Write a colored carpet to an image.
//
// bc - settings of bitcarpet
// bc.fs - must not be NULL.
// bc.w - must be positive.
// bc.h - must be positive.
// bc.dx - must be positive.
// bc.dy - must be positive.
// blank - color for blank cells, must be the size of BITCARPET_COLOR_N.
// filled - color for filled cells, must be the size of BITCARPET_COLOR_N.
//
// Returns the corresponding status code.
//
// TODO: Add length of the arrays as another parameter or use pointers to an
// array to be able to check whether it is equal to BITCARPET_COLOR_N.
enum bitcarpet_err_t bitcarpet_write_color(const struct bitcarpet_t *const bc,
										   const unsigned char *const blank,
										   const unsigned char *const filled) {
	unsigned int w_px, h_px;
	assert(bc->fs);
	assert(bc->w > 0 && bc->h > 0);
	assert(bc->dx > 0 && bc->dy > 0);
	if (umulovf(bc->w, bc->dx) || umulovf(bc->h, bc->dy))
		return BITCARPET_E_OVF;
	w_px = bc->w * bc->dx;
	h_px = bc->h * bc->dy;
	if (fprintf(bc->fs, "P6\n%u %u\n255\n", w_px, h_px) < 0)
		return BITCARPET_E_IO;
	for (unsigned int y = 0; y < h_px; ++y) {
		for (unsigned int x = 0; x < w_px; ++x) {
			int n;
			const unsigned char *color;
			if (f(x / bc->dx, y / bc->dy))
				color = !(bc->is_inverted) ? filled : blank;
			else
				color = !(bc->is_inverted) ? blank : filled;
			n = fwrite(color, sizeof(*color), BITCARPET_COLOR_N, bc->fs);
			if (n < BITCARPET_COLOR_N)
				return BITCARPET_E_IO;
		}
	}
	return BITCARPET_E_OK;
}
