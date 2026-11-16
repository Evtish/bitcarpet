#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitcarpet.h"

#define PBM_FILENAME "bitcarpet_bw.pbm"
#define PPM_FILENAME "bitcarpet_fancy.ppm"
#define WIDTH 256  // In cells
#define HEIGHT 256 // In cells
#define CELL_DX 4  // In pixels
#define CELL_DY 4  // In pixels
#define BLANK_R 0x1a
#define BLANK_G 0x1b
#define BLANK_B 0x26
#define FILLED_R 0xac
#define FILLED_G 0xb0
#define FILLED_B 0xd0

static void fclose_or_exit(FILE *stream) {
	if (fclose(stream) == EOF) {
		perror("fclose");
		exit(EXIT_FAILURE);
	}
}

int main(void) {
	unsigned char blank[BITCARPET_COLOR_N] = {BLANK_R, BLANK_G, BLANK_B},
				  filled[BITCARPET_COLOR_N] = {FILLED_R, FILLED_G, FILLED_B};
	FILE *image;
	image = fopen(PBM_FILENAME, "w");
	if (!image) {
		perror("fopen");
		return EXIT_FAILURE;
	}
	if (bitcarpet_write_bw(image, WIDTH, HEIGHT, CELL_DX, CELL_DY) == -1) {
		perror("bitcarpet_write_bw");
		fclose_or_exit(image);
		return EXIT_FAILURE;
	}
	fclose_or_exit(image);
	printf("PBM image %s generated successfully\n", PBM_FILENAME);
	image = fopen(PPM_FILENAME, "w");
	if (!image) {
		perror("fopen");
		return EXIT_FAILURE;
	}
	if (bitcarpet_write_fancy(image, WIDTH, HEIGHT, CELL_DX, CELL_DY, blank,
							  filled) == -1) {
		perror("bitcarpet_write_fancy");
		fclose_or_exit(image);
		return EXIT_FAILURE;
	}
	fclose_or_exit(image);
	printf("PPM image %s generated successfully\n", PPM_FILENAME);
	return EXIT_SUCCESS;
}
