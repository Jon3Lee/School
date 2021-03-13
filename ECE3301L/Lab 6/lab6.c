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

#define E1 PORTEbits.RE1
#define E2 PORTEbits.RE2

void init_UART()
{
    OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF &
    USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX &
    USART_BRGH_HIGH, 25);
    OSCCON = 0x60;
}

void putch (char c)
{
    while (!TRMT);
    TXREG = c;
}

void WAIT_1_SEC()
{
    for (int j=0; j<0xffff; j++);
}

unsigned int get_full_ADC(void)
{
int result;
    ADCON0bits.GO=1;                                                                        // Start Conversion
    while(ADCON0bits.DONE==1);                                                              // wait for conversion to be completed
    result = (ADRESH * 0x100) + ADRESL;                                                     // combine result of upper byte and
                                                                                            // lower byte into result
    return result;                                                                          // return the result.
}

void Init_ADC(void) 
{
    ADCON1 = 0x1B;                                                                          // select pins AN0 through AN3 as analog signal, VDD-VSS as
                                                                                            // reference voltage
    ADCON2 = 0xA9;                                                                          // right justify the result. Set the bit conversion time (TAD) and
                                                                                            // acquisition time
}

void Select_ADC_Channel(char channel)
{
    ADCON0 = channel * 4 + 1;
}

void Display_Lower_Digit(char digit)
{
    PORTD = array[digit];                                                                   //PORTD goes to the lower digit and just takes from the array value to display its digit
}

void Display_Upper_Digit(char digit)
{
    PORTC = array[digit];                                                                   //PORTC goes to the upper digit of the 7 segment display, but segment A
                                                                                            //on the display goes to PORTE.
    if (array[digit] & 0x40)                                                                //Mask off the 6th bit 
    {                                                                                       //If the 6th bit is there
        PORTE = 0x02;                                                                       //PORTE is off 
    }
    else                                                                                    //else
    {
        PORTE = 0x00;                                                                       //PORTE is on
    }
}