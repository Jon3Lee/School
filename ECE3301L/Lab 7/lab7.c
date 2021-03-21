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

#define SEC_LED PORTDbits.RD7               //Define SEC_LED as Port D bit 7

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
void Wait_One_Second();
void Wait_Half_Second();


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
    //Activate_Buzzer();                               // Activate the buzzer
    Wait_Half_Second();                             // Wait for half second (or 500 msec)
    SEC_LED = 0;                                    // then turn off the SEC LED
    //Deactivate_ Buzzer ();                          // Deactivate the buzzer
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

void Set_NSLT(char color)
{
    switch (color)
    {
        case OFF: NSLT_RED =0;NS_GREEN=0;break;       // Turns off the NSLT LED
        case RED: NSLT_RED =1;NS_GREEN=0;break;       // Sets NSLT LED RED
        case GREEN: NSLT_RED =0;NS_GREEN=1;break;     // sets NSLT LED GREEN
        case YELLOW: NSLT_RED =1;NS_GREEN=1;break;    // sets NSLT LED YELLOW
    }
}

void Set_EW(char color)
{
    switch (color)
    {
        case OFF: EW_RED =0;NS_GREEN=0;break;         // Turns off the EW LED
        case RED: EW_RED =1;NS_GREEN=0;break;         // Sets EW LED RED
        case GREEN: EW_RED =0;NS_GREEN=1;break;       // sets EW LED GREEN
        case YELLOW: EW_RED =1;NS_GREEN=1;break;      // sets EW LED YELLOW
    }
}

void Set_EWLT(char color)
{
    switch (color)
    {
        case OFF: EWLT_RED =0;NS_GREEN=0;break;       // Turns off the EWLT LED
        case RED: EWLT_RED =1;NS_GREEN=0;break;       // Sets EWLT LED RED
        case GREEN: EWLT_RED =0;NS_GREEN=1;break;     // sets EWLT LED GREEN
        case YELLOW: EWLT_RED =1;NS_GREEN=1;break;    // sets EWLT LED YELLOW
    }
}

void PED_Control( char Direction, char Num_Sec)
{

}

void main(void)
{
    init_UART();
    TRISA = 0X1F;                                   //Set TRISA to 
    TRISB = 0X00;                                                                      
    TRISC = 0X00;                                                                               
    TRISD = 0X00;                                                                               
    TRISE = 0X00;  

    while (1)
    {
        for (int i=0;i<4;i++)
        {
            Set_NS(i); // Set color for North-South direction
            Set_NSLT(i); // Set color for North-South Left-Turn direction
            Set_EW(i); // Set color for East-West direction
            Set_EWLT(i); // Set color for East-West Left-Turn direction
            Wait_N_Seconds(1); // call Wait-N-Second routine to wait for 1 second
        }
    }
}

