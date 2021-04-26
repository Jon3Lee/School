#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#include <usart.h>

#include "I2C.h"
#include "I2C_Support.h"


#pragma config OSC = INTIO67
#pragma config BOREN =OFF
#pragma config WDT=OFF
#pragma config LVP=OFF
#pragma config CCP2MX = PORTBE

char tempSecond = 0xff; 
char second = 0x00;
char minute = 0x30;
char hour = 0x07;
char dow = 0x00;
char day = 0x25;
char month = 0x04;
char year = 0x21;

short nec_ok = 0;
unsigned char Nec_state = 0;
unsigned long long Nec_code;
char Nec_code1;

char setup_second, setup_minute, setup_hour, setup_day, setup_month, setup_year;
char alarm_second, alarm_minute, alarm_hour, alarm_date;
char setup_alarm_second, setup_alarm_minute, setup_alarm_hour;

char TempSecond;

void INT0_isr();


void putch (char c)
{   
    while (!TRMT);       
    TXREG = c;
}

void init_UART()
{
    OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 25);
    OSCCON = 0x70;
}

void init_INTERRUPT()
{
    INTCONbits.INT0IF = 0;                  // Clear external interrupt
    INTCON2bits.INTEDG0 = 0;                // Edge programming for INT0 falling edge H to L
    INTCONbits.INT0IE = 1;                  // Enable external interrupt
    TMR1H = 0;                              // Reset Timer1
    TMR1L = 0;                              //
    PIR1bits.TMR1IF = 0;                    // Clear timer 1 interrupt flag
    PIE1bits.TMR1IE = 1;                    // Enable Timer 1 interrupt
    INTCONbits.PEIE = 1;                    // Enable Peripheral interrupt
    INTCONbits.GIE = 1;                     // Enable global interrupts
    nec_ok = 0;                             // Clear flag
    Nec_code = 0x0;                         // Clear code
}

void Do_Init()                      // Initialize the ports 
{ 
    init_UART();                    // Initialize the uart
    OSCCON=0x70;                    // Set oscillator to 8 MHz 
    
    ADCON1=0x0F;

    TRISB = 0x01;
    TRISC = 0xFF;
    TRISD = 0xFF;

    RBPU=0;
    
    I2C_Init(100000); 
    DS1621_Init();
    init_INTERRUPT();
    
}

void Wait_One_Sec()
{
    for (int j=0; j<0xffff; j++);
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

void main() 
{ 
   Do_Init();
   DS3231_Setup_Time();

   while (1)
   {
       if (nec_ok == 1)
       {
           nec_ok = 0;
           Nec_code1 = (char) ((Nec_code >> 8));
           printf ("NEC_Code = %08lx %x\r\n", Nec_code, Nec_code1);
           INTCONbits.INT0IE = 1;          // Enable external interrupt
           INTCON2bits.INTEDG0 = 0;        // Edge programming for INT0 falling edge

           Activate_Buzzer();
           Wait_One_Sec();
           Deactivate_Buzzer();

           DS3231_Setup_Time();
       }

       DS3231_Read_Time();
       if(tempSecond != second)
       {
           tempSecond = second;
           char tempC = DS1621_Read_Temp();
           char tempF = (tempC * 9 / 5) + 32;
           printf ("%02x:%02x:%02x %02x/%02x/%02x",hour,minute,second,month,day,year);
           printf (" Temperature = %d degreesC = %d degreesF\r\n", tempC, tempF);
       }
   }
}






