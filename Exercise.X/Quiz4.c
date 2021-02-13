/* 
 * File:   Quiz4.c
 * Author: Arthur
 *
 * Created on November 17, 2020, 9:37 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * 
 */
int main() {
    DDRB |= (1 << DDB4);
    TCCR1A |= (1 << COM1B1) | (1 << WGM10);
    TCCR1B |= (1 << WGM13);
    
    OCR1A = 64;
    OCR1B = 35;
    
    while (1) {}
    
    
    return (EXIT_SUCCESS);
}

