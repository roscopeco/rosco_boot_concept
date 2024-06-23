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
 * Configuration for concept boot screen
 * ------------------------------------------------------------
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define VERSION     "2.50.DEV"

/*
 * General options
 */
#define CENTER_ITEMS                            /* Should items in the list be centered? */
#define HIGHLIGHT_SELECTION                     /* Should the text of the selection be highlighted? */
//#define ENABLE_ANIM                             /* Should the animation system be enabled? */
//#define SHOW_TEST_ANIM                          /* Should the test animation be displayed? */
//#define ENABLE_TIMER                            /* Should the count-down timer be enabled? */
#define ALWAYS_BIZCAT                           /* Always use bizcat font, even at low resolution */

/*
 * Resolution, everything else is driven off of these...
 *
 * Either 640x480 or 320x240 is acceptable.
 */
#ifndef VIEW_HRES
#define VIEW_HRES   320
#endif
#ifndef VIEW_VRES
#define VIEW_VRES   240
#endif

#endif