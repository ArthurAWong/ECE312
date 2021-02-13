/* 
 * File:   Lab1.c
 * Author: Arthur Wong, 1570501
 *
 * Created on October 24, 2020, 2:34 AM
 */

#define F_CPU 9600000
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

/*
 * 
 */
int main() {

    PORTB |= (1 << PORTB0);
    PORTB |= (1 << PORTB1);
    PORTB |= (1 << PORTB3);
    
    DDRB |= (1 << DDB0);
    DDRB |= (1 << DDB1);
    DDRB |= (1 << DDB2);
    DDRB &= ~(1 << DDB3);
    
    _delay_ms (500);
    PORTB &= ~(1 << PORTB1);
    
    while (1) {
        while(PINB & 1 << PINB3) {}
        PORTB &= ~(1 << PORTB0);
        PORTB |= (1 << PORTB1);
        _delay_ms (100);            
        while(PINB & 1 << PINB3) {}
        PORTB |= (1 << PORTB0);
        _delay_ms (500);
        PORTB &= ~(1 << PORTB1);
        }
    
    return (EXIT_SUCCESS);
}