#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>

#define PD7     PORTDbits.RD7

void main()
{
    TRISD = 0x00;       //Set TRISD to 0x00 since PORTD is output
    T1CON = 0xF0;       //Timer1, scale is 1:8
    TMR1L = 0x0D;
    TMR1H = 0xFE;
    PIR1bits.TMR1IF = 0; //Clear interrupt flag
    T1CONbits.TMR1ON = 1;  // Turn on timer 1

    while (1)
    {
        while(PIR1bits.TMR1IF == 0);
        T1CONbits.TMR1ON = 0;
        PD7 = ~PD7;
        TMR1L = 0x0D;
        TMR1H = 0xFE;
        P1R1bits.TMR1F = 0;
        T1CONbits.TMR1ON = 1;
    }
}