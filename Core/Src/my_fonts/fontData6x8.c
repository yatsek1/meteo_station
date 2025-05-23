/*
 * fontData6x8.c
 *
 *  Created on: Apr 25, 2025
 *      Author: sapal
 */

#include "fonts/fontData6x8.h"

const unsigned char fontData6x8[96][6] = {
	{0x00,0x00,0x00,0x00,0x00,0x00}, //
	{0x2e,0x00,0x00,0x00,0x00,0x00}, // !
	{0x03,0x00,0x03,0x00,0x00,0x00}, // "
	{0x0a,0x1f,0x0a,0x1f,0x0a,0x00}, // #
	{0x2e,0x2a,0x6b,0x2a,0x3a,0x00}, // $
	{0x0e,0x2a,0x1e,0x08,0x3c,0x2a}, // %
	{0x3e,0x2a,0x2a,0x22,0x38,0x08}, // &
	{0x03,0x00,0x00,0x00,0x00,0x00}, // '
	{0x1c,0x22,0x00,0x00,0x00,0x00}, // (
	{0x22,0x1c,0x00,0x00,0x00,0x00}, // )
	{0x15,0x0e,0x04,0x0e,0x15,0x00}, // *
	{0x08,0x08,0x3e,0x08,0x08,0x00}, // +
	{0x60,0x00,0x00,0x00,0x00,0x00}, // ,
	{0x08,0x08,0x08,0x08,0x08,0x00}, // -
	{0x20,0x00,0x00,0x00,0x00,0x00}, // .
	{0x20,0x10,0x08,0x04,0x02,0x00}, // /
	{0x3e,0x22,0x2a,0x22,0x3e,0x00}, // 0
	{0x04,0x3e,0x00,0x00,0x00,0x00}, // 1
	{0x3a,0x2a,0x2a,0x2a,0x2e,0x00}, // 2
	{0x2a,0x2a,0x2a,0x2a,0x3e,0x00}, // 3
	{0x0e,0x08,0x08,0x08,0x3e,0x00}, // 4
	{0x2e,0x2a,0x2a,0x2a,0x3a,0x00}, // 5
	{0x3e,0x2a,0x2a,0x2a,0x3a,0x00}, // 6
	{0x02,0x02,0x02,0x02,0x3e,0x00}, // 7
	{0x3e,0x2a,0x2a,0x2a,0x3e,0x00}, // 8
	{0x2e,0x2a,0x2a,0x2a,0x3e,0x00}, // 9
	{0x14,0x00,0x00,0x00,0x00,0x00}, // :
	{0x34,0x00,0x00,0x00,0x00,0x00}, // ;
	{0x08,0x14,0x22,0x00,0x00,0x00}, // <
	{0x14,0x14,0x14,0x14,0x14,0x00}, // =
	{0x22,0x14,0x08,0x00,0x00,0x00}, // >
	{0x06,0x02,0x2a,0x0a,0x0e,0x00}, // ?
	{0x3e,0x02,0x3a,0x2a,0x3e,0x00}, // @
	{0x3e,0x12,0x12,0x12,0x3e,0x00}, // A
	{0x3e,0x2a,0x2a,0x2a,0x36,0x00}, // B
	{0x3e,0x22,0x22,0x22,0x22,0x00}, // C
	{0x3e,0x22,0x22,0x22,0x1c,0x00}, // D
	{0x3e,0x2a,0x2a,0x2a,0x22,0x00}, // E
	{0x3e,0x0a,0x0a,0x0a,0x02,0x00}, // F
	{0x3e,0x22,0x2a,0x2a,0x3a,0x00}, // G
	{0x3e,0x08,0x08,0x08,0x3e,0x00}, // H
	{0x22,0x3e,0x22,0x00,0x00,0x00}, // I
	{0x38,0x20,0x20,0x20,0x3e,0x00}, // J
	{0x3e,0x08,0x08,0x14,0x22,0x00}, // K
	{0x3e,0x20,0x20,0x20,0x20,0x00}, // L
	{0x3e,0x04,0x38,0x04,0x3e,0x00}, // M
	{0x3e,0x04,0x08,0x10,0x3e,0x00}, // N
	{0x3e,0x22,0x22,0x22,0x3e,0x00}, // O
	{0x3e,0x0a,0x0a,0x0a,0x0e,0x00}, // P
	{0x3e,0x22,0x72,0x22,0x3e,0x00}, // Q
	{0x3e,0x0a,0x0a,0x1a,0x2e,0x00}, // R
	{0x2e,0x2a,0x2a,0x2a,0x3a,0x00}, // S
	{0x02,0x02,0x3e,0x02,0x02,0x00}, // T
	{0x1e,0x20,0x20,0x20,0x1e,0x00}, // U
	{0x0e,0x10,0x20,0x10,0x0e,0x00}, // V
	{0x3e,0x10,0x0e,0x10,0x3e,0x00}, // W
	{0x22,0x14,0x08,0x14,0x22,0x00}, // X
	{0x02,0x04,0x38,0x04,0x02,0x00}, // Y
	{0x3a,0x2a,0x2a,0x2a,0x2e,0x00}, // Z
	{0x3e,0x22,0x00,0x00,0x00,0x00}, // [
	{0x3f,0x21,0x3f,0x00,0x00,0x00}, // "\"
	{0x22,0x3e,0x00,0x00,0x00,0x00}, // ]
	{0x0c,0x1e,0x3c,0x1e,0x0c,0x00}, // ^
	{0x20,0x20,0x20,0x20,0x20,0x00}, // _
	{0x1c,0x3e,0x3e,0x3e,0x1c,0x00}, // `
	{0x3c,0x24,0x24,0x24,0x3c,0x20}, // a
	{0x3e,0x24,0x24,0x24,0x3c,0x00}, // b
	{0x3c,0x24,0x24,0x24,0x24,0x00}, // c
	{0x3c,0x24,0x24,0x24,0x3e,0x00}, // d
	{0x3c,0x24,0x34,0x2c,0x24,0x00}, // e
	{0x08,0x3e,0x0a,0x0a,0x00,0x00}, // f
	{0x1c,0x54,0x54,0x54,0x7c,0x00}, // g
	{0x3e,0x04,0x04,0x04,0x3c,0x00}, // h
	{0x3a,0x00,0x00,0x00,0x00,0x00}, // i
	{0x7a,0x00,0x00,0x00,0x00,0x00}, // j
	{0x3e,0x08,0x14,0x22,0x00,0x00}, // k
	{0x02,0x3e,0x00,0x00,0x00,0x00}, // l
	{0x3c,0x04,0x3c,0x04,0x3c,0x00}, // m
	{0x3c,0x04,0x04,0x04,0x3c,0x00}, // n
	{0x3c,0x24,0x24,0x24,0x3c,0x00}, // o
	{0x7c,0x24,0x24,0x24,0x3c,0x00}, // p
	{0x3c,0x24,0x24,0x24,0x7c,0x00}, // q
	{0x3c,0x04,0x04,0x04,0x00,0x00}, // r
	{0x24,0x2c,0x34,0x24,0x00,0x00}, // s
	{0x04,0x3e,0x24,0x24,0x00,0x00}, // t
	{0x3c,0x20,0x20,0x20,0x3c,0x00}, // u
	{0x0c,0x10,0x20,0x10,0x0c,0x00}, // v
	{0x3c,0x20,0x3c,0x20,0x3c,0x00}, // w
	{0x24,0x24,0x18,0x24,0x24,0x00}, // x
	{0x1c,0x50,0x50,0x50,0x7c,0x00}, // y
	{0x24,0x34,0x2c,0x24,0x00,0x00}, // z
	{0x08,0x3e,0x22,0x00,0x00,0x00}, // {
	{0x1c,0x22,0x22,0x22,0x1c,0x00}, // |
	{0x22,0x3e,0x08,0x00,0x00,0x00}, // }
	{0x0C,0x12,0x12,0x0C,0x00,0x00}, // ~ (degree)
	{0x00,0x00,0x00,0x00,0x00,0x00}
};

const fontStruct font6x8 = {
	(uint8_t *)fontData6x8,
	96,
	32,
	127,
	6,
	8,
	1,
	true
};
