/* 
 * File:   Assignment4.c
 * Author: Arthur
 *
 * Created on November 24, 2020, 9:52 AM
 */
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
/*
 * 
 */
/*
void enable_and_configure_spi() {
    PORTB |= (1 << PORTB1) | (1 << PORTB5); // Enables MOSI and SCK output
    PORTB &= ~(1 << PORTB4); // MISO input
    
    SPCR &= ~(1 << CPOL); // Low to high edge triggered
    SPCR |= (1 << MSTR); // Sets MCU to master
    SPCR |= (1 << SPR0) | (1 << SPR1); // Sets SCK to lowest speed
    SPCR |= (1 << SPE); // Enables SPI
}
*/
