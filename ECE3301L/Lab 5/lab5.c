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

char array[10] = (0x01, 0x4F, 0x12, 0x06, 0x4C, 0x24, 0x20, 0x0F, 0x00, 0x04);

void Init_ADC(void) 
    {
        ADCON0 = 0x??;                      // select channel AN0, and turn on the ADC subsystem
        ADCON1 = 0x?? ;                     // select pins AN0 through AN3 as analog signal, VDD-VSS as
                                            // reference voltage
        ADCON2 = 0xA9;                      // right justify the result. Set the bit conversion time (TAD) and
                                            // acquisition time
    }


void WAIT_1_SEC() 
{
    for (int j=0; i<0x17000; j++);
}

void Display_Lower_Digit(char digit)
{
    PORTD = array[digit];
}

void Display_Upper_Digit(char digit)
{
    PORTC = array[digit];
}

void main(void)
{
    Init_ADC();
    TRISA = 0X0F;                               //2 bits of port A connect to a LED, the others are input by temperature sensor and photoresistor
    TRISB = 0X00;                               //TRISB leads to a RGB LED so set to output
    TRISC = 0X00;                               //Set TRISC to output because it goes to a 7-Segment LED, which is always output
    TRISD = 0X00;                               //Set TRISD to output because it goes to a 7-Segment LED, which is always output
    TRISE = 0X00;                               //Set TRISE to output because it goes to a 7-Segment LED, which is always output

    while (1)
    {
        for (int i=0; i<10; i++)
        {
            Display_Lower_Digit(i);
            Display_Upper_DIgit(i);
            WAIT_1_SEC();
        }
    }
}