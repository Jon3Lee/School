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

#define SEC_LED PORTDbits.RD7

#define NS_RED PORTAbits.RA5
#define NS_GREEN PORTBbits.RB0

#define NSLT_RED PORTBbits.RB1
#define NSLT_GREEN PORTBbits.RB2

#define EW_RED PORTBbits.RB4
#define EW_GREEN PORTBbits.RB5

#define EWLT_RED PORTEbits.RE0
#define EWLT_GREEN PORTEbits.RE2

#define OFF 0
#define RED 1
#define GREEN 2
#define YELLOW 3

void init_UART();
void putch(char);

void init_UART()
{
    OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF &
    USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX &
    USART_BRGH_HIGH, 25);
    OSCCON = 0x60;                              //Set to 4 MHz
}

void putch (char c)
{
    while (!TRMT);
    TXREG = c;
}

void Wait_One_Second()
{
    SEC_LED = 0;                                // First, turn off the SEC LED
    Wait_Half_Second();                         // Wait for half second (or 500 msec)
    SEC_LED = 1;                                // then turn on the SEC LED
    Wait_Half_Second();                         // Wait for half second (or 500 msec)
}

void Wait_Half_Second()
{
    T0CON = 0x02;                               // Timer 0, 16-bit mode, prescaler 1:8
                                                // Subtract F424 from FFFF = 0x0BDB
    TMR0L = 0xDB;                               // set the lower byte of TMR to 0xDB
    TMR0H = 0x0B;                               // set the upper byte of TMR to 0x0B
    INTCONbits.TMR0IF = 0;                      // clear the Timer 0 flag
    T0CONbits.TMR0ON = 1;                       // Turn on the Timer 0
    while (INTCONbits.TMR0IF == 0);             // wait for the Timer Flag to be 1 for done
    T0CONbits.TMR0ON = 0;                       // turn off the Timer 0
}

void Wait_N_Seconds (char seconds)
{
char I;
    for (I = 0; I< seconds; I++)
    {
        Wait_One_Second();
    }
}

void Wait_One_Second_With_Beep()
{
    SEC_LED = 1;                                    // First, turn on the SEC LED
    Activate_Buzzer()                               // Activate the buzzer
    Wait_Half_Second();                             // Wait for half second (or 500 msec)
    SEC_LED = 0;                                    // then turn off the SEC LED
    Deactivate_ Buzzer ();                          // Deactivate the buzzer
    Wait_Half_Second();                             // Wait for half second (or 500 msec)
}

void Set_NS(char color)
{
    switch (color)
    {
        case OFF: NS_RED =0;NS_GREEN=0;break;       // Turns off the NS LED
        case RED: NS_RED =1;NS_GREEN=0;break;       // Sets NS LED RED
        case GREEN: NS_RED =0;NS_GREEN=1;break;     // sets NS LED GREEN
        case YELLOW: NS_RED =1;NS_GREEN=1;break;    // sets NS LED YELLOW
    }
}

void main(void)
{
    init_UART();
    TRISA = 0X2F;                                                                               //Set TRISA to 2F since RA4 is an output, binary value 00101111
    TRISB = 0X00;                                                                               //TRISB leads to a RGB LED so set to output
    TRISC = 0X00;                                                                               //Set TRISC to output because it goes to a 7-Segment LED, which is always output
    TRISD = 0X00;                                                                               //Set TRISD to output because it goes to a 7-Segment LED, which is always output
    TRISE = 0X01;  

    while (1)
    {
        for (int i=0;i i<4;i++)
        {
            SET_NS(i); // Set color for North-South direction
            SET_NSLT(i); // Set color for North-South Left-Turn direction
            SET_EW(i); // Set color for East-West direction
            SET_EWLT(i); // Set color for East-West Left-Turn direction
            Wait_N_Second(1); // call Wait-N-Second routine to wait for 1 second
        }
    }
}

