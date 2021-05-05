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

void init_UART();
void putch(char);
void Wait_One_Second();
void Wait_Half_Second();
void Activate_Buzzer();
void Deactivate_Buzzer();

char array[10] = {0x7F, 0x4F, 0x12, 0x06, 0x4C, 0x24, 0x20, 0x0F, 0x00, 0x04}; 
                                                //Change 0 in the array to blank

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

void Activate_Buzzer()
{
    PR2 = 0b11111001 ;
    T2CON = 0b00000101 ;
    CCPR2L = 0b01001010 ;
    CCP2CON = 0b00111100 ;
}

void Deactivate_Buzzer()
{
    CCP2CON = 0x0;
    PORTBbits.RB3 = 0;
}

void Wait_N_Seconds(char seconds)
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
    Activate_Buzzer();                              // Activate the buzzer
    Wait_Half_Second();                             // Wait for half second (or 500 msec)
    SEC_LED = 0;                                    // then turn off the SEC LED
    Deactivate_Buzzer();                            // Deactivate the buzzer
    Wait_Half_Second();                             // Wait for half second (or 500 msec)
}

void Display_Upper_Digit(char digit)
{
    PORTC = array[digit] & 0x3F;
    if ((array[digit] & 0x40) == 0x40)
    {
        SEG_A = 1;
    }
    else
    {
        SEG_A = 0;
    }
}

void Display_Lower_Digit(char digit)
{
    PORTD = (PORTD & 0x80) | array[digit];
}

unsigned int get_full_ADC(void)
{
int result;
    ADCON0bits.GO=1;                                 // Start Conversion
    while(ADCON0bits.DONE==1);                       // wait for conversion to be completed
    result = (ADRESH * 0x100) + ADRESL;              // combine result of upper byte and
                                                     // lower byte into result
    return result;                                   // return the result.
}

void Select_ADC_Channel(char channel)                                                       
{
    ADCON0 = channel * 4 + 1;
}

void Init_ADC(void) 
{
    ADCON1 = 0x0E;                                  // Since we're using up to AN5, we use 1001 for bits 3-0 to set them as analog, and 1 for bit 4,
                                                    // which is the VREF+ AN3 and 0 for bit 5, coming out to 011001
    ADCON2 = 0xA9;                                  // right justify the result. Set the bit conversion time (TAD) and
                                                    // acquisition time
}


void main(void)
{
    init_UART();
    Init_ADC();
    
    TRISA = 0X1F;                                   //Set TRISA to 0x1F since bits 7 and 
    TRISB = 0X00;                                   //Set TRISB to 0x00 since TRISB leads to RGB LEDs, which are outputs.                                    
    TRISC = 0X00;                                   //SET TRISC to 0x00 since its connected to a 7-segment LED which is an output.                                            
    TRISD = 0X00;                                   //SET TRISD to 0x00 since its connected to a 7-segment LED which is an output.                                              
    TRISE = 0X00;                                   //SET TRISE to 0x00 since it is connected to part of the 7-segment and LEDS, which are all outputs.

    while (1)
    {
       
    }

}

