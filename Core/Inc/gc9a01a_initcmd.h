#pragma once

#include <stdint.h>
#include "gc9a01a.h"

// Komendy inicjalizacji GC9A01A
static const uint8_t INIT_CMD[] = {
  GC9A01A_INREGEN2, 0,
  0xEB, 1, 0x14,                                                        // ?
  GC9A01A_INREGEN1, 0,
  GC9A01A_INREGEN2, 0,
  0xEB, 1, 0x14,                                                        // ?
  0x84, 1, 0x40,                                                        // ?
  0x85, 1, 0xFF,                                                        // ?
  0x86, 1, 0xFF,                                                        // ?
  0x87, 1, 0xFF,                                                        // ?
  0x88, 1, 0x0A,                                                        // ?
  0x89, 1, 0x21,                                                        // ?
  0x8A, 1, 0x00,                                                        // ?
  0x8B, 1, 0x80,                                                        // ?
  0x8C, 1, 0x01,                                                        // ?
  0x8D, 1, 0x01,                                                        // ?
  0x8E, 1, 0xFF,                                                        // ?
  0x8F, 1, 0xFF,                                                        // ?
  0xB6, 2, 0x00, 0x00,                                                  // ?
  GC9A01A_MADCTL, 1, MADCTL_MX | MADCTL_BGR,
  GC9A01A_COLMOD, 1, 0x05,
  0x90, 4, 0x08, 0x08, 0x08, 0x08,                                      // ?
  0xBD, 1, 0x06,                                                        // ?
  0xBC, 1, 0x00,                                                        // ?
  0xFF, 3, 0x60, 0x01, 0x04,                                            // ?
  GC9A01A1_POWER2, 1, 0x13,
  GC9A01A1_POWER3, 1, 0x13,
  GC9A01A1_POWER4, 1, 0x22,
  0xBE, 1, 0x11,                                                        // ?
  0xE1, 2, 0x10, 0x0E,                                                  // ?
  0xDF, 3, 0x21, 0x0c, 0x02,                                            // ?
  GC9A01A_GAMMA1, 6, 0x45, 0x09, 0x08, 0x08, 0x26, 0x2A,
  GC9A01A_GAMMA2, 6, 0x43, 0x70, 0x72, 0x36, 0x37, 0x6F,
  GC9A01A_GAMMA3, 6, 0x45, 0x09, 0x08, 0x08, 0x26, 0x2A,
  GC9A01A_GAMMA4, 6, 0x43, 0x70, 0x72, 0x36, 0x37, 0x6F,
  0xED, 2, 0x1B, 0x0B,                                                  // ?
  0xAE, 1, 0x77,                                                        // ?
  0xCD, 1, 0x63,                                                        // ?
  // Unsure what this line (from manufacturer's boilerplate code) is
  // meant to do, but users reported issues, seems to work OK without:
  //0x70, 9, 0x07, 0x07, 0x04, 0x0E, 0x0F, 0x09, 0x07, 0x08, 0x03,      // ?
  GC9A01A_FRAMERATE, 1, 0x34,
  0x62, 12, 0x18, 0x0D, 0x71, 0xED, 0x70, 0x70,                         // ?
            0x18, 0x0F, 0x71, 0xEF, 0x70, 0x70,
  0x63, 12, 0x18, 0x11, 0x71, 0xF1, 0x70, 0x70,                         // ?
            0x18, 0x13, 0x71, 0xF3, 0x70, 0x70,
  0x64, 7, 0x28, 0x29, 0xF1, 0x01, 0xF1, 0x00, 0x07,                    // ?
  0x66, 10, 0x3C, 0x00, 0xCD, 0x67, 0x45, 0x45, 0x10, 0x00, 0x00, 0x00, // ?
  0x67, 10, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x01, 0x54, 0x10, 0x32, 0x98, // ?
  0x74, 7, 0x10, 0x85, 0x80, 0x00, 0x00, 0x4E, 0x00,                    // ?
  0x98, 2, 0x3e, 0x07,                                                  // ?
  GC9A01A_TEON, 0,
  GC9A01A_INVON, 0,
  GC9A01A_SLPOUT, 0x80,                                                 // Exit sleep
  GC9A01A_DISPON, 0x80,                                                 // Display on
  0x00                                                                  // End of list
};
