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

#define D1_RED PORTBbits.RB0
#define D1_GREEN PORTBbits.RB1
#define D1_BLUE PORTBbits.RB2

#define D2_RED PORTBbits.RB4
#define D2_GREEN PORTBbits.RB5
#define D2_BLUE PORTAbits.RA4

unsigned int nstep;
float volt;
float R;

void init_UART();
void putch(char);
unsigned int get_full_ADC(void);
void Init_ADC(void);
void select_ADC_Channel(char);
void Display_Lower_Digit(char);
void Display_Upper_Digit(char);

char array[10] = {0x01, 0x4F, 0x12, 0x06, 0x4C, 0x24, 0x20, 0x0F, 0x00, 0x04};             //This array dictates which digits display on the 7 segment display


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
    ADCON1 = 0x19;                                                                          // select pins AN0 through AN3 as analog signal, VDD-VSS as
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
    PORTC = array[digit] & 0x3F;                                                                                                                                    
    if ((array[digit] & 0x40) == 0x40)                                                                 
    {                                                                                       
        E1 = 1;                                                                       
    }
    else                                                                                    
    {
        E1 = 0;                                                                       
    }
}