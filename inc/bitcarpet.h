#pragma once

#include <stdio.h>

#define BITCARPET_COLOR_N 3

int bitcarpet_write_bw(FILE *fs, unsigned int w, unsigned int h);
int bitcarpet_write_fancy(FILE *fs, unsigned int w, unsigned int h,
						  unsigned char blank[BITCARPET_COLOR_N],
						  unsigned char filled[BITCARPET_COLOR_N]);
