/* 
 * File:   Exercise7.c
 * Author: Arthur
 *
 * Created on October 27, 2020, 3:14 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * 
 */
/*int main() {

    PORTD &= ~(1 << PORTD3); // Assuming PD3 is being driven by a push-pull driver
    DDRD &= ~(1 << DDD3);

    DDRD |= (1 << PORTD0);

    EICRA &= ~(1 << ISC10);
    EICRA |= (1 << ISC11);

    EIMSK |= (1 << INT1);

    sei();

    while (1) {}
    
    return (EXIT_SUCCESS);
}

ISR(INT1_vect, ISR_BLOCK) {

    if (PORTD & (1 << PORTD0)) {

        PORTD &= ~(1 << PORTD0);

    } else {

        PORTD |= (1 << PORTD0);
    }
} */

