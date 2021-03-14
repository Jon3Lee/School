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

#define E1          PORTEbits.RE1
#define DP          PORTEbits.RE2

#define D1_RED      PORTBbits.RB0
#define D1_GREEN    PORTBbits.RB1
#define D1_BLUE     PORTBbits.RB2

#define D2_RED      PORTBbits.RB4
#define D2_GREEN    PORTBbits.RB5
#define D2_BLUE     PORTAbits.RA4

void init_UART();
void putch(char);
unsigned int get_full_ADC(void);
void Init_ADC(void);
void Select_ADC_Channel(char channel);

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
    ADCON1 = 0x19;                                                                          // Since we're using up to AN5, we use 1001 for bits 3-0 to set them as analog, and 1 for bit 4,
                                                                                            // which is the VREF+ AN3 and 0 for bit 5, coming out to 011001
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

void SD1_WHITE()
{
    D1_RED = 1;
    D1_BLUE = 1;
    D1_GREEN = 1;
}

void SD1_OFF()
{
    D1_RED = 0;
    D1_BLUE = 0;
    D1_GREEN = 0;
}

void SD2_OFF()
{
    D2_RED = 0;
    D2_BLUE = 0;
    D2_GREEN = 0;
}

void SD2_CYAN()
{
    D2_RED = 0;
    D2_BLUE = 1;
    D2_GREEN = 1;
}
void main(void)
{
    Init_ADC();
    init_UART();
    TRISA = 0X3F;
    TRISB = 0X00;                                                                               //TRISB leads to a RGB LED so set to output
    TRISC = 0X00;                                                                               //Set TRISC to output because it goes to a 7-Segment LED, which is always output
    TRISD = 0X00;                                                                               //Set TRISD to output because it goes to a 7-Segment LED, which is always output
    TRISE = 0X01;                                                                               //Set TRISD to output because it goes to part of a 7-segment LED.

    float RREF = 100;

    while(1)
    {
        Select_ADC_Channel(5);
    int nstep = get_full_ADC();
    float voltage_mv = nstep * 4.0;
    float volt = voltage_mv/1000;
    float RL = (volt / (4.096 - volt)) * (RREF/1000);
        if (RL > 10)
        {
            DP = 1;
        int R = (int) RL;
        char U = R / 10;
        char L = R % 10;
            Display_Upper_Digit(U);
            Display_Lower_Digit(L);

            WAIT_1_SEC();
            if (RL > 69)
            {
                SD1_WHITE();
            }
            else
            {
                PORTB = U;
            }
        }
        else
        {
            DP = 0;
        char U = (int) RL;
        char L = (int) ((RL - U) * 10);
            Display_Upper_Digit(U);
            Display_Lower_Digit(L);
            SD1_OFF();

            WAIT_1_SEC();
        }
        if (RL < .07)
        {
            SD2_CYAN();
            Activate_Buzzer();
        }
        else
        {
            SD2_OFF();
            Deactivate_Buzzer();
        }

        printf("r = %f \r\n", RL);
    }
}