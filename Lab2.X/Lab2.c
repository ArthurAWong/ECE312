/* 
 * File:   Lab2.c
 * Author: Arthur
 *
 * Created on October 19, 2020, 4:34 PM
 */

#define F_CPU 9600000
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "WS2812_CONFIG.h"

#define NoOfLEDs 8
#define ButtonDelay 500
#define BaseDelaySmoothOscil 225
#define SmoothingNumber 12

/*ASM code requires 3x8 array via struct, must wait 50ms between each output to LED strip*/
struct cRGB  { uint8_t green; uint8_t red; uint8_t blue; } led[NoOfLEDs]; 

void sws2812_sendarray_mask(uint8_t *data);
void DisplayLED(struct cRGB *);
void change_brightness(struct cRGB *);
void wait_for_button_presses(struct cRGB *);

uint8_t BrightnessState = 0;
uint8_t IsBrightnessButtonHeld = 0;

int main() { 
    
    PORTB |= (1 << PORTB0);
    PORTB |= (1 << PORTB1);
    
    DDRB &= ~(1 << DDB0);
    DDRB &= ~(1 << DDB1);
    
    while(1) {
        for (uint8_t i = 0; i < NoOfLEDs; i++) {
            led[i].green = 63;
            led[i].red = 63;
            led[i].blue = 63;
        }
        DisplayLED(led);
        wait_for_button_presses(led);
        
        for (uint8_t i = 0; i < NoOfLEDs; i++) {
            led[i].green = 0;
            led[i].blue = 0;
        }
        DisplayLED(led);
        wait_for_button_presses(led);
        
        for (uint8_t i = 0; i < NoOfLEDs; i++) {
            led[i].green = 63;
            led[i].red = 0;
        }
        DisplayLED(led);
        wait_for_button_presses(led);
        
        for (uint8_t i = 0; i < NoOfLEDs; i++) {
            led[i].green = 0;
            led[i].blue = 63;
        }
        DisplayLED(led);
        wait_for_button_presses(led);

        for (uint8_t i = 0; i < 8; i++) {
            led[i].green = 63*(i%2);
            led[i].red = 63*(1-(i%2));
            led[i].blue = 0;
        }
        DisplayLED(led);
        while(!(PINB & (1 << PINB1))) {}
        while(PINB & 1 << PINB1) {}
        
        for (uint8_t i = 0; i < NoOfLEDs; i++) {
            // LED is not very good at color accuracy, so RGB values are "random"
            led[i].green = 44;
            led[i].red = 105;
            led[i].blue = 3;
        }
        sws2812_sendarray_mask(&led[0].green);
        _delay_ms(ButtonDelay);
        PORTB |= (1 << PORTB1);
        int8_t counter = 1;
        while(!(PINB & (1 << PINB1))) {}
        while(PINB & 1 << PINB1) {
            if(counter == SmoothingNumber) {
                counter = -1;
            }
            if(counter == -SmoothingNumber) {
                counter = 1;
            }
            for (uint8_t i = 0; i < NoOfLEDs; i++) {
                led[i].green += counter * 1/SmoothingNumber;
                led[i].blue += counter * 3/SmoothingNumber;
            }
            sws2812_sendarray_mask(&led[0].green);
            PORTB |= (1 << PORTB1);
            _delay_ms(BaseDelaySmoothOscil);
            if(counter > 0) {
                counter++;
            } 
            if(counter < 0) {
                counter--; 
            }
        }
        
        for (uint8_t i = 0; i < NoOfLEDs; i++) {
            led[i].green = 0;
            led[i].red = 0;
            led[i].blue = 0;
        }
        
        sws2812_sendarray_mask(&led[0].green);
        PORTB |= (1 << PORTB1);
        _delay_ms(500);
        while(!(PINB & (1 << PINB1))) {}
        while(PINB & 1 << PINB1) {}
    }
    
    return (EXIT_SUCCESS);
}

void sws2812_sendarray_mask(uint8_t *data) {
    static uint8_t curbyte,ctr,masklo;
    static uint8_t datlen = 24; // 24 bit per LED
    static uint8_t maskhi = _BV (ws2812_pin);
    ws2812_DDRREG |= maskhi; // Enable output
    masklo = ~maskhi & ws2812_PORTREG;
    maskhi |= ws2812_PORTREG;
    while (datlen--) {
        curbyte = *data++;
        asm volatile (
            " ldi %0,8 \n\t"
            " loop%= : \n\t"
            " out %2, %3 \n\t" // ?1' [01] '0' [01] - re
            " nop \n\t"
            " sbrs %1,7 \n\t" // ?1' [03] '0' [02]
            " out %2, %4 \n\t" // ?1' [--] '0' [03] - fe-low
            " lsl %1 \n\t" // ?1' [04] '0' [04]
            " out %2, %4 \n\t" // ?1' [+1] '0' [+1] - fe-high
            " dec %0 \n\t" // ?1' [+2] '0' [+2]
            " brne loop%= \n\t" // ?1' [+3] '0' [+4]
            : "=&d" (ctr)
            : "r" (curbyte), "I" (_SFR_IO_ADDR (ws2812_PORTREG)), "r" (maskhi), "r" (masklo)
        ); 
    }
} 

void DisplayLED(struct cRGB *ptr) {
    BrightnessState = 0;
    sws2812_sendarray_mask(&ptr[0].green);
    PORTB |= (1 << PORTB0);
    PORTB |= (1 << PORTB1);
    _delay_ms(ButtonDelay);
}

void change_brightness(struct cRGB *ptr) {
    if (!(PINB & 1 << PINB0)) {
        if (BrightnessState >= 3) {
            for (uint8_t i = 0; i < NoOfLEDs; i++) {
                ptr[i].green /= 4;
                ptr[i].red /= 4;
                ptr[i].blue /= 4;
            }
            BrightnessState = 0;
        } else {
            for (uint8_t i = 0; i < NoOfLEDs; i++) {
                ptr[i].green += (ptr[i].green / (BrightnessState + 1));
                ptr[i].red += (ptr[i].red / (BrightnessState + 1));
                ptr[i].blue += (ptr[i].blue / (BrightnessState + 1));
            }
            BrightnessState++;
        }
        sws2812_sendarray_mask(&ptr[0].green);
        PORTB |= (1 << PORTB0);
        PORTB |= (1 << PORTB1);
        _delay_ms(ButtonDelay);
        if (!(PINB & (1 << PINB0))) {
            IsBrightnessButtonHeld = 1;
        }
    }
};

void wait_for_button_presses (struct cRGB *ptr) {
    while(!(PINB & (1 << PINB1))) {}
    while(PINB & (1 << PINB1)) {
        if (IsBrightnessButtonHeld == 1) {
            while(!(PINB & (1 << PINB0))) {}
            IsBrightnessButtonHeld = 0;
        }
        change_brightness(led);
    }
}