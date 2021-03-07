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

#define D3_RED PORTAbits.RA4
#define D3_GREEN PORTAbits.RA5

unsigned int get_full_ADC(void);

void Init_ADC(void);
void Select_ADC_Channel(char channel);

char array[10] = {0x01, 0x4F, 0x12, 0x06, 0x4C, 0x24, 0x20, 0x0F, 0x00, 0x04};


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

void Init_ADC(void) 
{
    ADCON1 = 0x1B;                          // select pins AN0 through AN3 as analog signal, VDD-VSS as
                                            // reference voltage
    ADCON2 = 0xA9;                          // right justify the result. Set the bit conversion time (TAD) and
                                            // acquisition time
}

void Select_ADC_Channel(char channel)
{
    ADCON0 = channel * 4 + 1;
}

void WAIT_1_SEC() 
{
    for (unsigned int j=0; j<0xffff; j++);
}

unsigned int get_full_ADC(void)
{
int result;
    ADCON0bits.GO=1;                        // Start Conversion
    while(ADCON0bits.DONE==1);              // wait for conversion to be completed
    result = (ADRESH * 0x100) + ADRESL;     // combine result of upper byte and
                                            // lower byte into result
    return result;                          // return the result.
}

void Display_Lower_Digit(char digit)
{
    PORTD = array[digit];
}

void Display_Upper_Digit(char digit)
{
    PORTC = array[digit];
    if (digit == 0 || digit == 2 || digit == 3 || digit == 5 || digit == 6 || digit == 7 || digit == 8 || digit == 9)
    {
        PORTE = 0x00;
    }
    else 
    {
        PORTE = 0x02;
    }
}

void SD1_OFF()
{
    D1_RED = 0;
    D1_BLUE = 0;
    D1_GREEN = 0;
}
void SD1_RED()
{
    D1_RED = 1;
    D1_BLUE = 0;
    D1_GREEN = 0;
}
void SD1_GREEN()
{
    D1_RED = 0;
    D1_BLUE = 0;
    D1_GREEN = 1;
}
void SD1_YELLOW()
{
    D1_RED = 1;
    D1_BLUE = 0;
    D1_GREEN = 1;
}
void SD1_BLUE()
{
    D1_RED = 0;
    D1_BLUE = 1;
    D1_GREEN = 0;
}
void SD1_PURPLE()
{
    D1_RED = 1;
    D1_BLUE = 1;
    D1_GREEN = 0;
}
void SD1_CYAN()
{
    D1_RED = 0;
    D1_BLUE = 1;
    D1_GREEN = 1;
}
void SD1_WHITE()
{
    D1_RED = 1;
    D1_BLUE = 1;
    D1_GREEN = 1;
}
void SD2_OFF()
{
    D2_RED = 0;
    D2_BLUE = 0;
    D2_GREEN = 0;
}
void SD2_RED()
{
    D2_RED = 1;
    D2_BLUE = 0;
    D2_GREEN = 0;
}
void SD2_GREEN()
{
    D2_RED = 0;
    D2_BLUE = 0;
    D2_GREEN = 1;
}
void SD2_BLUE()
{
    D2_RED = 0;
    D2_BLUE = 1;
    D2_GREEN = 0;
}
void SD2_WHITE()
{
    D2_RED = 1;
    D2_BLUE = 1;
    D2_GREEN = 1;
}
void SD3_RED()
{
    D3_RED = 1;
    D3_GREEN = 0;
}
void SD3_GREEN()
{
    D3_RED = 0;
    D3_GREEN = 1;
}
void SD3_YELLOW()
{
    D3_RED = 1;
    D3_GREEN = 1;
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
        Select_ADC_Channel (0);
        int num_step = get_full_ADC();
        float voltage_mv = num_step * 4.0;
        float temperature_C = (1035.0 - voltage_mv) / 5.50;
        float temperature_F = 1.80 * temperature_C + 32.0;
        int tempF = (int) temperature_F;
        char U = tempF / 10;
        char L = tempF % 10;
        Display_Upper_Digit(U);
        Display_Lower_Digit(L);
        WAIT_1_SEC();
        

        //BEGIN D1
        if (tempF < 10)
        {
            SD1_OFF();
        }
        else if (tempF > 9 && tempF < 20)
        {
            SD1_RED();
        }
        else if (tempF > 19 && tempF < 30)
        {
            SD1_GREEN();
        }
        else if (tempF > 29 && tempF < 40)
        {
            SD1_YELLOW();
        }
        else if (tempF > 39 && tempF < 50)
        {
            SD1_BLUE();
        }
        else if (tempF > 49 && tempF < 60)
        {
            SD1_PURPLE();
        }
        else if (tempF > 59 && tempF < 70)
        {
            SD1_CYAN();
        }
        else
        {
            SD1_WHITE();
        }

        //BEGIN D2
        if (tempF < 45)
        {
            SD2_OFF();
        }
        else if (tempF > 45 && tempF < 56)
        {
            SD2_RED();
        }
        else if (tempF > 55 && tempF < 66)
        {
            SD2_GREEN();
        }
        else if (tempF > 65 && tempF < 76)
        {
            SD2_BLUE();
        }
        else
        {
            SD2_WHITE();
        }

        //BEGIN D3
        if (voltage_mv < 2500)
        {
            SD3_RED();
        }
        else if (voltage_mv > 2499 && voltage_mv < 3400)
        {
            SD3_GREEN();
        }
        else
        {
            SD3_YELLOW();
        }

        printf("Temperature = %d F \r\n", tempF);
        Select_ADC_Channel(2);
    }

}
