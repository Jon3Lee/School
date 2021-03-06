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

void Delay_One_Sec()
{
    for(int I=0; I <17000; I++);
} 

void main()
{
char in;                            // Use variable ‘in’ as char
    TRISB = 0x00;                   //Output to B
    TRISC = 0x00;                   //Output to C
    ADCON1 = 0x0f;                  //Analog/Digital control register set to digital

    while (1) 
    {
        for (char i=0; i<8; i++) 
        {
            PORTC = i;              //Color of LED matches the counter value as it counts up from 0 to 7
            Delay_One_Sec();        //1 Second delay between each count
        }
    }
        
}