/* 
 * File:   Exercise6.c
 * Author: Arthur Wong
 *
 * Created on October 15, 2020, 5:59 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

/*
 * 
 */
/*int main() {
    DDRB &= ~(1 << DDB0);
    PORTB &= ~(1 << PORTB1);
    DDRB |= (1 << DDB1);
    
    while(1){
        
         if(PINB & (1 << PINB0)) {
             
            PORTB |= (1 << PORTB1);
            
         } else {
             
             PORTB &= ~(1 << PORTB1);
         }
    }
     
    return (EXIT_SUCCESS);
}

*/