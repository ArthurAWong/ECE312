/* 
 * File:   WS2812_CONFIG.h
 * Author: Arthur
 *
 * Created on October 31, 2020, 5:23 PM
 */

#include <avr/io.h>

#ifndef WS2812_CONFIG_H_
#define WS2812_CONFIG_H_
// Define Reset time in µs.
// This is the time the library spends waiting after writing the data.
// WS2812 and clones only need 50 µs
#define ws2812_resettime 50
// Define I/O pin
#define ws2812_port B // Data port
#define ws2812_pin 2 // Data out pin

#if !defined(CONCAT)
#define CONCAT(a, b) a ## b
#endif

#if !defined(CONCAT_EXP)
#define CONCAT_EXP(a, b) CONCAT(a, b)
#endif

#if !defined(ws2812_PORTREG)
#define ws2812_PORTREG CONCAT_EXP(PORT, ws2812_port)
#define ws2812_DDRREG CONCAT_EXP(DDR, ws2812_port)
#endif

#endif /* WS2812_CONFIG_H_ */

