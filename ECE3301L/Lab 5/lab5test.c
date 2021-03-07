#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF

#define D1_RED PORTBbits.RB0
#define D1_GREEN PORTBbits.RB1
#define D1_BLUE PORTBbits.RB2

#define D2_RED PORTBbits.RB3
#define D2_GREEN PORTBbits.RB4
#define D2_BLUE PORTBbits.RB5

#define D1_RED PORTEbits.RE1
#define D1_GREEN PORTEbits.RE2


unsigned int get_full_ADC(void);
void Init_ADC(void);
void Do_Display_7Seg(int T);
void SET_D1_RED();
void SET_D1_GREEN();
void SET_D1_BLUE();
void SET_D2_OFF();
void SET_D2_RED();
void SET_D2_GREEN();
void SET_D2_BLUE();
void SET_D2_WHITE();
void Do_Display_D1(int T);
void Do_Display_D2(int T);
void Do_Display_D3(float V);
void putch (char c);
void init_UART();
void Delay_one_sec();

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

void main(void)
{
    float mvolt,Tc,Tf,volt;
    Init_ADC();
    int nStep;
    int ITemp;
    init_UART();
while(1) // forever loop
{
    ADCON0 = 1; // channel 0 
    nStep = get_full_ADC();
    mvolt = 4 * nStep;
    Tc = (1035 - mvolt)/5.5; // temp in C
    Tf = Tc * 9/5 + 32; // temp in f
    ITemp = (int)Tf; // convert temp into integer
    Do_Display_7Seg(ITemp);
    Do_Display_D1(ITemp);
    Do_Display_D2(ITemp);
    ADCON0 = 5;
    nStep = get_full_ADC();
    mvolt = 4 * nStep;
    volt = mvolt/1000;
   
    Do_Display_D3(volt);
     Delay_one_sec();
     Delay_one_sec();
    printf(" Voltage in volts = %f \r\n",volt);    
   
    
    printf (" Temp in f = %d \r\n",ITemp);    
}
    
}

void Init_ADC()
{
TRISA = 0xFF; 
TRISB = 0x00;
TRISC = 0x00;
TRISD = 0x00;
TRISE = 0x00;
ADCON1=0x1B; 
ADCON2=0xA9;

}
unsigned int get_full_ADC()
{
 unsigned int result;
ADCON0bits.GO=1; // Start Conversion
while(ADCON0bits.DONE==1); // wait for conversion to be completed
result = (ADRESH * 0x100) + ADRESL; // combine result of upper byte and
// lower byte into result
return result; // return the result.
}

void Do_Display_7Seg(int T)
{
    int Array[10] = {0x01,0x4F,0x12,0x06,0x4C,0x24,0x20,0x0F,0x00,0x04};
    char L,U;
    L = T %10;
    U = T /10;
    PORTC = Array[U];
    PORTD = Array[L];
}

void Do_Display_D1(int T)
{
    char array[8] = {0,1,2,3,4,5,6,7};
    if(T < 10)
    {
        PORTB = array[0];
    }
    
    else if(T > 70)
    {
        PORTB = array[7];
    }
    else
    {
        PORTB = array[T/10];
    }
}
void Do_Display_D2(int T)
{
    if(T < 46)
    {
        SET_D2_OFF();
    }
    else if(T > 45 && T < 56)
    {
        SET_D2_RED();
    }
    else if(T > 55 && T < 66)
    {
        SET_D2_GREEN();
    }
    else if(T > 65 && T < 76)
    {
        SET_D2_BLUE();
    }
    else if(T > 75)
    {
        SET_D2_WHITE();
    }
  
}
void Do_Display_D3(float V)
{
    if(V < 2.5)
    {
        PORTE = 2;
    }
    else if( V >= 2.5 && V < 3.5)
    {
        PORTE = 4;
    }
    else
    {
        PORTE = 6;
    }
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
void Delay_one_sec()
{
    for(int i = 0; i <17000;i++)
    {
        
    }
}