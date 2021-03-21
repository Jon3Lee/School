#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#include <usart.h>

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF
#pragma config CCP2MX = PORTBE

void Wait_One_Second()
{
    SEC_LED = 0; // First, turn off the SEC LED
    Wait_Half_Second(); // Wait for half second (or 500 msec)
    SEC_LED = 1; // then turn on the SEC LED
    Wait_Half_Second(); // Wait for half second (or 500 msec)
}

void Wait_Half_Second()
{
    T0CON = 0x02 // Timer 0, 16-bit mode, prescaler 1:8
    TMR0L = 0x??; // set the lower byte of TMR
    TMR0H = 0x??; // set the upper byte of TMR
    INTCONbits.TMR0IF = 0; // clear the Timer 0 flag
    T0CONbits.TMR0ON = 1; // Turn on the Timer 0
    while (INTCONbits.TMR0IF == 0); // wait for the Timer Flag to be 1 for done
    T0CONbits.TMR0ON = 0; // turn off the Timer 0
}