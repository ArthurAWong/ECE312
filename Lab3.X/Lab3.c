/* 
 * File:   Lab3.c
 * Author: Arthur
 *
 * Created on November 13, 2020, 10:47 PM
 */

#include "defines.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "hd44780.h"
#include "lcd.h"

#define MAXSECONDS 86400
#define COUNTSFORSECOND 31249

FILE lcd_str = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);

volatile unsigned long int TimeSeconds = 0;
volatile int CursorPosition = 0; // Relative to the left-most digit, ignoring colons.
volatile int UpdateClock = 0;
volatile int TimeArray[6] = {0, 0, 0, 0, 0, 0};

unsigned long int Seconds = 0;
unsigned long int Minutes = 0;
unsigned long int Hours = 0;
char InstruArray[3][17] = {"1:Right 2:Left  ", "3:Inc. 4:Dec.   ", "5:Confirm       "};
int NextInstruction = 0;
int UpdateInstructions = 0;
int IsConfirm = 0;
int IntCursorPosition = 0;

int main(void) { 
    
    PORTD |= (1 << PORTD0) | (1 << PORTD1) | (1 << PORTD2) | (1 << PORTD3) | (1 << PORTD4);
    DDRD |= (1 << DDD0) | (1 << DDD1) | (1 << DDD2) | (1 << DDD3) | (1 << DDD4); 
    
    
    lcd_init();
    for (int i = 0; i < 40; i++) {
        fprintf(&lcd_str, "\x1b\x14"); // shift right once
    }
    fprintf(&lcd_str, "%s", InstruArray[0]);
    NextInstruction++;
    fprintf(&lcd_str, "\x1b\x02");
    fprintf(&lcd_str, "    00:00:00");
    for (int i = 0; i < 8; i++) {
        fprintf(&lcd_str, "\x1b\x10"); // shift left once
    }
    fprintf(&lcd_str, "\x1b\x0f"); //enable blinking cursor
    TCCR1B |= (1 << CS12);
    OCR1A = 60000;
    TIMSK1 |= (1 << OCIE1A);
    TCCR1B |= (1 << WGM12);
    sei();
    while (1) {
        if (!(PIND & (1 << PIND0))) {               //move left
            cli();
            _delay_ms(250);
            if (CursorPosition == 0) {
                for (int i = 0; i < 7; i++) {
                    fprintf(&lcd_str, "\x1b\x14"); // shift right once
                } 
                CursorPosition = 5;
            } else if (CursorPosition == 4 || CursorPosition == 2) {
                fprintf(&lcd_str, "\x1b\x10");
                fprintf(&lcd_str, "\x1b\x10");
                CursorPosition--;
            } else {
                fprintf(&lcd_str, "\x1b\x10");
                CursorPosition--;
            }
            sei();
        }
        
        if (!(PIND & (1 << PIND1))) {               //move right
            cli();
            _delay_ms(250);
            if (CursorPosition == 5) {
                for (int i = 0; i < 7; i++) {
                    fprintf(&lcd_str, "\x1b\x10"); 
                } 
                CursorPosition = 0;
            } else if (CursorPosition == 1 || CursorPosition == 3) {
                fprintf(&lcd_str, "\x1b\x14");
                fprintf(&lcd_str, "\x1b\x14");
                CursorPosition++;
            } else {
                fprintf(&lcd_str, "\x1b\x14");
                CursorPosition++;
            }
            sei();
        }
    
        if (!(PIND & (1 << PIND2))) {               //increment
            cli();
            _delay_ms(250);
            switch (CursorPosition) {
                case 0:
                    if(TimeArray[0] == 2) {
                        TimeArray[0] = 0;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[0]);
                    } else if (TimeArray[0] == 1 && TimeArray[1] > 3) {
                        TimeArray[0] = 2;
                        TimeArray[1] = 3;
                        fprintf(&lcd_str, "%d%d\x1b\x10\x1b\x10", TimeArray[0], TimeArray[1]);
                        
                    } else {
                        TimeArray[0]++;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[0]);
                    }
                    break;
                case 1:
                    if(TimeArray[0] == 2 && TimeArray[1] == 3) {
                        TimeArray[1] = 0;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[1]);
                    } else if (TimeArray[1] == 9) {
                        TimeArray[1] = 0;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[1]);
                    } else {
                        TimeArray[1]++;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[1]);
                    }
                    break;
                case 2:
                    if(TimeArray[2] == 5) {
                        TimeArray[2] = 0;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[2]);
                    } else {
                        TimeArray[2]++;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[2]);
                    }
                    break;
                case 3:
                    if (TimeArray[3] == 9) {
                        TimeArray[3] = 0;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[3]);
                    } else {
                        TimeArray[3]++;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[3]);
                    }
                    break;
                case 4:
                    if(TimeArray[4] == 5) {
                        TimeArray[4] = 0;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[4]);
                    } else {
                        TimeArray[4]++;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[4]);
                    }
                    break;
                case 5:
                    if (TimeArray[5] == 9) {
                        TimeArray[5] = 0;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[5]);
                    } else {
                        TimeArray[5]++;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[5]);
                    }
                    break;
            }
            sei();
        }
        
        if (!(PIND & (1 << PIND3))) {               //decrement
            cli();
            _delay_ms(250);
            switch (CursorPosition) {
                case 0:
                    if (TimeArray[0] == 0 && TimeArray[1] > 3) {
                        TimeArray[0] = 2;
                        TimeArray[1] = 3;
                        fprintf(&lcd_str, "%d%d\x1b\x10\x1b\x10", TimeArray[0], TimeArray[1]);
                    } else if(TimeArray[0] == 0) {
                        TimeArray[0] = 2;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[0]);
                    } else {
                        TimeArray[0]--;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[0]);
                    }
                    break;
                case 1:
                    if(TimeArray[0] == 2 && TimeArray[1] == 0) {
                        TimeArray[1] = 3;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[1]);
                    } else if (TimeArray[1] == 0) {
                        TimeArray[1] = 9;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[1]);
                    } else {
                        TimeArray[1]--;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[1]);
                    }
                    break;
                case 2:
                    if(TimeArray[2] == 0) {
                        TimeArray[2] = 5;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[2]);
                    } else {
                        TimeArray[2]--;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[2]);
                    }
                    break;
                case 3:
                    if (TimeArray[3] == 0) {
                        TimeArray[3] = 9;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[3]);
                    } else {
                        TimeArray[3]--;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[3]);
                    }
                    break;
                case 4:
                    if(TimeArray[4] == 0) {
                        TimeArray[4] = 5;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[4]);
                    } else {
                        TimeArray[4]--;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[4]);
                    }
                    break;
                case 5:
                    if (TimeArray[5] == 0) {
                        TimeArray[5] = 9;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[5]);
                    } else {
                        TimeArray[5]--;
                        fprintf(&lcd_str, "%d\x1b\x10", TimeArray[5]);
                    }
                    break;
            }
            sei();
        }
        
        if (!(PIND & (1 << PIND4))) {               //confirm
            IsConfirm = 1;
            cli();
            fprintf(&lcd_str, "\x1b\x01");
            fprintf(&lcd_str, "    %d%d:%d%d:%d%d", TimeArray[0], TimeArray[1], TimeArray[2], TimeArray[3], TimeArray[4], TimeArray[5]);
            for (int i = 0; i <= 7; i++) {
                fprintf(&lcd_str, "\x1b\x10");
            }
            CursorPosition = 0;
            TimeSeconds = (36000*TimeArray[0] + 3600*TimeArray[1]) + (600*TimeArray[2] + 60*TimeArray[3]) + (10*TimeArray[4]+TimeArray[5]);
            fprintf(&lcd_str, "\x1b\x0c");
            OCR1A = COUNTSFORSECOND;
            TCNT1 = 0;
            sei();
            while (1) {
                if (TimeSeconds >= MAXSECONDS) {
                    TimeSeconds = 0;
                    UpdateClock = 1;
                } 
                if(UpdateClock == 1) {
                    cli();
                    Seconds = TimeSeconds % 60;
                    Hours = TimeSeconds / 60;
                    Minutes = Hours % 60;
                    Hours = Hours / 60;
                    TimeArray[0] = Hours / 10;
                    TimeArray[1] = Hours - 10*TimeArray[0];
                    TimeArray[2] = Minutes / 10;
                    TimeArray[3] = Minutes - 10*TimeArray[2];
                    TimeArray[4] = Seconds / 10;
                    TimeArray[5] = Seconds - 10*TimeArray[4];
                    
                    fprintf(&lcd_str, "%d%d:%d%d:%d%d", TimeArray[0], TimeArray[1], TimeArray[2], TimeArray[3], TimeArray[4], TimeArray[5]);
                    for (int i = 0; i <= 7; i++) {
                        fprintf(&lcd_str, "\x1b\x10");
                    }
                    UpdateClock = 0;
                    sei();
                }
            }
        } 
        
        if (UpdateInstructions == 1) {
            cli();
            IntCursorPosition = CursorPosition;
            switch (CursorPosition) {
                case 2:
                case 3:
                    IntCursorPosition += 1; 
                    break;
                case 4:
                case 5:
                    IntCursorPosition += 2;
                    break;
            }
            
            for (int i = 0; i < 29 + (7 - IntCursorPosition); i++) {
                fprintf(&lcd_str, "\x1b\x14"); // shift right once
            }
            fprintf(&lcd_str, "%s", InstruArray[NextInstruction]);
            fprintf(&lcd_str, "\x1b\x02");
            fprintf(&lcd_str, "    %d%d:%d%d:%d%d", TimeArray[0], TimeArray[1], TimeArray[2], TimeArray[3], TimeArray[4], TimeArray[5]);
            for (int i = 0; i <= (7 - IntCursorPosition); i++) {
                fprintf(&lcd_str, "\x1b\x10");
            }
            if(NextInstruction == 2) {
                NextInstruction = 0;
            } else {
                NextInstruction++;
            }
            UpdateInstructions = 0;
            sei();
        }
    }
    return (EXIT_SUCCESS);
}

ISR(TIMER1_COMPA_vect, ISR_BLOCK) {
    if (!IsConfirm) {
        UpdateInstructions = 1;
    } else {
        TimeSeconds++;
        UpdateClock = 1; 
    }
}

/*
 * Send character c to the LCD display.  After a an ESC (0x1B) has been seen,
 * the next character (byte) will be sent to the LCD as a command.
 */
int lcd_putchar(char c, FILE *unused) {
  static bool esc_seen;   // Use of the static keyword makes the value persistent.

  if (!esc_seen) {
      /* The last character was not an escape character. Check to see it is
         this time. */
      if (c == 0x1b) {
        esc_seen = true;
      } else {
        /* The information can be written as data. */
        hd44780_wait_ready(false);
        hd44780_outdata(c);
      }
  } else {
      /* Last character was ESC = 0x1B: treat this byte as a command. 
         Since the command is arbitrary it may be one that takes a long
         time to execute: "Clear Display and "Return Home" are those 
         commands.  However, if USE_BUSY_BIT 
         is true, then a spin-wait on BF is made. */
      hd44780_wait_ready(false);
      hd44780_outcmd(c); // Send the byte as a command
      
      if (c <= 3) {
        /* This is either a Clear Display or Return home command. */
        hd44780_wait_ready(true);  // Long delay.
      }
      
      /* Clear the flag. */
      esc_seen = false;
  }
  
  return 0;
}