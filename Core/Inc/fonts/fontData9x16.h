/*
 * fontData9x16.h
 *
 *  Created on: Apr 25, 2025
 *      Author: sapal
 */

#ifndef INC_FONTS_FONTDATA9X16_H_
#define INC_FONTS_FONTDATA9X16_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "fonts/my_fonts.h"

/*
 *	9x16 font. 95 characters from 32-126.
 *	Bit format: vertical
 *	Memory: 1710 bytes (95x9x2)
 */
extern const uint16_t fontData9x16[95][9];

/*
 *	9x16 font.
 *	Characters: 95 (ASCII 32-126).
 *	Bit format: vertical
 *	Memory: 1710 bytes (95x9x2)
 */
extern const fontStruct font9x16;

#ifdef __cplusplus
}
#endif

#endif /* INC_FONTS_FONTDATA9X16_H_ */
