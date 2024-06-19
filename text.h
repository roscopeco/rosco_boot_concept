/*
 *------------------------------------------------------------
 *                                  ___ ___ _
 *  ___ ___ ___ ___ ___       _____|  _| . | |_
 * |  _| . |_ -|  _| . |     |     | . | . | '_|
 * |_| |___|___|___|___|_____|_|_|_|___|___|_,_|
 *                     |_____|          firmware
 * ------------------------------------------------------------
 * Copyright (c)2024 The rosco_m68k Open Source Project
 * See top-level LICENSE.md for licence information.
 *
 * Text rendering for concept boot screen
 * ------------------------------------------------------------
 */

#ifndef __TEXT_H
#define __TEXT_H

#include <stdint.h>

void text_write(const char *str, int x, int y, const uint8_t *font, int font_height);

#endif