#ifndef _MY_FONTS_H_
#define _MY_FONTS_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct fontStruct {
	const void * bitmap; // The bitmap data to represent the font
	uint16_t characters; // The number of valid characters in a font
	uint8_t start; // The first valid character in a font
	uint8_t end; // The last valid character in a font
	uint8_t width; // The width of the font in pixels
	uint8_t height; // The height of the font in pixels
	uint8_t spacing; // The horizontal spacing required for a font
	bool bitAlignVertical; // True if the data is stored vertically
} fontStruct;

#ifdef __cplusplus
}
#endif

#endif
