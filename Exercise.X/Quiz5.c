/* 
 * File:   Quiz5.c
 * Author: Arthur
 *
 * Created on November 26, 2020, 9:40 AM
 */
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

/*
 * 
 */
int main() {
    return (EXIT_SUCCESS);
}

void UART_Init() {
    UBRRL = 1; // Set for 115200 baud, at clock 3.6864 MHz and no double speed 
    UCSRC |= (1 << UPM1) | (1 << USBS) | (1 << UCSZ1) | (1 << UCSZ0); // Set even parity, 2 stop bits, and 8 data bits
    PORTD |= (1 << PORTD1); // Enable TXD output
    UCSRB |= (1 << RXEN) | (1 << TXEN); // Enable transmitter and receiver
}