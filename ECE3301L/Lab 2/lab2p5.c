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

void Delay_One_Sec();                                                   //Initialize function
void main()
{
char array[8] = {0xa6, 0x23, 0x05, 0x82, 0xc0, 0x67, 0x41, 0xe4};       //Array to set the colors of each LED
char in;                                                                // Use variable ‘in’ as char

    TRISC = 0x00;                                                       //Output to C
    TRISD = 0x00;                                                       //Output to D
    ADCON1 = 0x0F;                                                      // Analog/Digital control register set to digital

    while (1)
    {
        for (char i=0; i<8; i++)
        {
            PORTC = i;                                                  //Colors of LED will count up
            PORTD = array[i];                                           //2 LEDs will light up according to the first 4 bits and last 4 bits.
            Delay_One_Sec();                                            //One second delay between each count
        }
    }       
}
void Delay_One_Sec()
{
    for(int I =0; I<17000; I++)
    {
    }
    
} 