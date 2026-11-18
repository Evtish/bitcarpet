#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitcarpet.h"

static void fclose_or_exit(FILE *stream) {
	if (fclose(stream) == EOF) {
		perror("fclose");
		exit(EXIT_FAILURE);
	}
}

int main(void) {
	const bool use_color = true;
	const char *function_name =
		(use_color) ? "bitcarpet_write_color" : "bitcarpet_write_bw";
	const char *filename =
		(use_color) ? "bitcarpet_color.ppm" : "bitcarpet_bw.pbm";
	enum bitcarpet_err_t bc_err;
	struct bitcarpet_t bc;

	bc.fs = fopen(filename, "w");
	bc.w = 256;
	bc.h = 256;
	bc.dx = 4;
	bc.dy = 4;
	bc.is_inverted = false;
	if (!bc.fs) {
		perror("fopen");
		return EXIT_FAILURE;
	}

	if (use_color) {
		unsigned char blank[BITCARPET_COLOR_N] = {0x1A, 0x1B, 0x26};
		unsigned char filled[BITCARPET_COLOR_N] = {0xAC, 0xB0, 0xD0};
		bc_err = bitcarpet_write_color(&bc, blank, filled);
	} else {
		bc_err = bitcarpet_write_bw(&bc);
	}

	switch (bc_err) {
	case BITCARPET_E_OK:
		fclose_or_exit(bc.fs);
		printf("%s image %s generated successfully\n",
			   (use_color) ? "PPM" : "PBM", filename);
		return EXIT_SUCCESS;
	case BITCARPET_E_IO:
		perror(function_name);
		fclose_or_exit(bc.fs);
		return EXIT_FAILURE;
	case BITCARPET_E_OVF:
		fprintf(stderr, "%s: Multiplication overflow\n", function_name);
		fclose_or_exit(bc.fs);
		return EXIT_FAILURE;
	default:
		fprintf(stderr, "%s: Unknown error\n", function_name);
		fclose_or_exit(bc.fs);
		return EXIT_FAILURE;
	}
}
