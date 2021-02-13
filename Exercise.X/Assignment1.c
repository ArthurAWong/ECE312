/* 
 * File:   Assignment1.c
 * Author: Arthur
 *
 * Created on October 19, 2020, 8:25 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

/*
 * 
 */
/*int main() {
    PORTB |= (1 << PORTB5);
    PORTB |= (1 << PORTB0);
    PORTB |= (1 << PORTB4);
    DDRB |= (1 << DDB5);
    DDRB &= ~(1 << DDB0);
    DDRB &= ~(1 << DDB4);
    
    while(1){
        
         if(!(PINB & (1 << PINB0)) && !(PINB & (1<<PINB4))) {
             
            PORTB &= ~(1 << PORTB5);
            while (!(PINB & (1 << PINB0)) && !(PINB & (1<<PINB4))) {}
            PORTB |= (1 << PORTB5);
            
         } 
    }
     
    return (EXIT_SUCCESS);
}
*/