#include <limits.h>
#include <stdio.h>

#include "bitcarpet.h"

static unsigned int f(unsigned int x, unsigned int y) {
	return ((y + 1) * (y + 1) * (x + 1) >> 11) & 1;
}

int bitcarpet_write_bw(FILE *fs, unsigned int w, unsigned int h) {
	unsigned int x, y;
	unsigned char bit_val, bit_pos, chunk = 0;
	if (w <= 0 || h <= 0)
		return 1;
	if (fprintf(fs, "P4\n%u %u\n", w, h) < 0)
		return 1;
	for (y = 0; y < h; ++y) {
		for (x = 0; x < w; ++x) {
			bit_val = f(x, y);
			bit_pos = CHAR_BIT - (x % CHAR_BIT) - 1;
			chunk |= (bit_val << bit_pos);
			if (x % CHAR_BIT == CHAR_BIT - 1 || x + 1 >= w) {
				if (fputc(chunk, fs) == EOF && ferror(fs))
					return 1;
				chunk = 0;
			}
		}
	}
	return 0;
}

int bitcarpet_write_fancy(FILE *fs, unsigned int w, unsigned int h,
						  unsigned char blank[BITCARPET_COLOR_N],
						  unsigned char filled[BITCARPET_COLOR_N]) {
	unsigned int x, y;
	unsigned char *pixel_color;
	if (w <= 0 || h <= 0)
		return 1;
	if (fprintf(fs, "P6\n%u %u\n255\n", w, h) < 0)
		return 1;
	for (y = 0; y < h; ++y) {
		for (x = 0; x < w; ++x) {
			if (f(x, y))
				pixel_color = filled;
			else
				pixel_color = blank;
			if (fwrite(pixel_color, sizeof(*pixel_color), BITCARPET_COLOR_N,
					   fs) < BITCARPET_COLOR_N)
				return 1;
		}
	}
	return 0;
}
