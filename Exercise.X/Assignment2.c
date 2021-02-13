/* 
 * File:   Assignment2.c
 * Author: Arthur
 *
 * Created on November 15, 2020, 6:24 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

/*
 * 
 */
/*int main() {
    
    DDRB |= (1 << DDB3);  //sets PB1 as output
    OCR1A = 0;
    TCCR1B |= (1 << CS10);
    TCCR1A |= (1 << COM1A0);
    
    while(1) {
        while(TIFR & (1 << OCF1A)) {}
        OCR1A += 750;
        TIFR |= (1 << OCF1A);
        while(TIFR & (1 << OCF1A)) {}
        OCR1A += 9250;
        TIFR |= (1 << OCF1A);
    } */
    
    /* For fast PWM
    DDRB |= (1 << DDB3);
    TCCR1A &= ~(1 << COM1A0);
    TCCR1B &= ~(1 << CS11);
    TCCR1B &= ~(1 << CS12);
    ICR1 = 9999;
    OCR1A = 750;
    TCCR1A |= (1 << COM1A1);
    TCCR1B |= (1 << CS10) | (1 << WGM11) | (1 << WGM12) | (1 << WGM13);*/
    
/*    return (EXIT_SUCCESS);
} */