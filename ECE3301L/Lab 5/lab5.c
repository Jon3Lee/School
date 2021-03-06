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

void Init_ADC(void) 
    {
        ADCON0 = 0x??; // select channel AN0, and turn on the ADC subsystem
        ADCON1 = 0x?? ; // select pins AN0 through AN3 as analog signal, VDD-VSS as
        // reference voltage
        ADCON2 = 0xA9; // right justify the result. Set the bit conversion time (TAD) and
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