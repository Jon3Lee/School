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

#define D1_RED PORTBbits.RB0
#define D1_GREEN PORTBbits.RB1
#define D1_BLUE PORTBbits.RB2

#define D2_RED PORTBbits.RB3
#define D2_GREEN PORTBbits.RB4
#define D2_BLUE PORTBbits.RB5

#define D1_RED PORTEbits.RE1
#define D1_GREEN PORTEbits.RE2

char array[10] = (0x01, 0x4F, 0x12, 0x06, 0x4C, 0x24, 0x20, 0x0F, 0x00, 0x04);

void putch (char c)
{
    while (!TRMT);
    TXREG = c;
}

void Init_ADC(void) 
{
    ADCON0 = 0x00;                          // select channel AN0, and turn on the ADC subsystem
    ADCON1 = 0x1B;                          // select pins AN0 through AN3 as analog signal, VDD-VSS as
                                            // reference voltage
    ADCON2 = 0xA9;                          // right justify the result. Set the bit conversion time (TAD) and
                                            // acquisition time
}

unsigned int get_full_ADC(void)
{
int result
    ADCON0bits.GO=1;                        // Start Conversion
    while(ADCON0bits.DONE==1);              // wait for conversion to be completed
    result = (ADRESH * 0x100) + ADRESL;     // combine result of upper byte and
                                            // lower byte into result
    return result;                          // return the result.
}

void WAIT_1_SEC() 
{
    for (int j=0; j<17000; j++);
}

void Display_Lower_Digit(char digit)
{
    PORTD = array[digit];
}

void Display_Upper_Digit(char digit)
{
    PORTC = array[digit];
    if ()
}

void init_UART()
{
    OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 25);
    OSCCON = 0x60;
}

void main(void)
{
    Init_ADC();
    init_UART();
    TRISA = 0X0F;                               //2 bits of port A connect to a LED, the others are input by temperature sensor and photoresistor
    TRISB = 0X00;                               //TRISB leads to a RGB LED so set to output
    TRISC = 0X00;                               //Set TRISC to output because it goes to a 7-Segment LED, which is always output
    TRISD = 0X00;                               //Set TRISD to output because it goes to a 7-Segment LED, which is always output
    TRISE = 0X00;                               //Set TRISE to output because it goes to a 7-Segment LED, which is always output

    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0xFF;
    PORTD = 0xFF;
    PORTE = 0xFF;



    while (1)
    {
        for (int i=0; i<10; i++)
        {
            Display_Lower_Digit(i);
            Display_Upper_Digit(i);
            WAIT_1_SEC();
        }
    }

    Select_ADC_Channel (0);
    int num_step = get_full_ADC();
    float voltage_mv = num_step * 4.0;
    float temperature_C = (1035.0 - voltage_mv_) / 5.50;
    float temperature_F = 1.80 * temperature_C + 32.0;
    int tempF = (int) temperature_F;
    char U = tempF / 10;
    char L = tempF % 10;
    Display_Upper_Digit(U);
    Display_Lower_Digit(L);

    printf("Temperature = %d F \r\n", tempF);

    Select_ADC_Channel(1);
}

void SET_D2_OFF()
{
D2_RED = 0;
D2_GREEN = 0;
D2_BLUE = 0;
}
void SET_D2_RED()
{
D2_RED = 1;
D2_GREEN = 0;
D2_BLUE = 0;
}
void SET_D2_GREEN()
{
D2_RED = 0;
D2_GREEN =1;
D2_BLUE = 0;
}
void SET_D2_BLUE()
{
D2_RED = 0;
D2_GREEN =0;
D2_BLUE = 1;
}
void SET_D2_WHITE()
{
D2_RED = 1;
D2_GREEN =1;
D2_BLUE = 1;
}