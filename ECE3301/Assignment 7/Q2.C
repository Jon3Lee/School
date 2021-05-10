#include <p18f4620.h>
#include <math.h>
unsigned char whole_number;
unsigned char fractional_number;
unsigned char N, rms, total product, x, ADCONRESULT;

void ADC_Result();

void main()
{
    ADC_Result();
    PIE1bits.TMR2IE = 0;
    PIR1bits.TMR2IF = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.DIe = 1;
    T2CON = 0x00;
    TMR2 = 0x00;
    PR2 = 0x11;
    T2CONbits.TMR2ON = 1;

    while(1)
    for (N=0;N<128;N++)
    {
        ADCON0bits.GO = 1;
        while (ADCON0bits.DONE == 1);
        while (PIR1bits.TMR2IF == 0);
        T2CONbits.TMR2ON = 0;
        ASCONRESULT = ADRESH;
        x = ADCONRESULT/51;
        product = x*x;
    }
    
    rms = (sqrt(product/N));
    total += rms;
    whole_number = rms/10;
    fractional_number = rms%10;
    PORTC = whole_number;
    PORTD = fractional_number;
}

void ADC_Result()
{
    OSCCON = 0x60;
    TRISC = 0x00;
    TRISD = 0x00;
    ADCON0 = 0x01;
    ADCON1 = 0x0E;
    ADCON2 = 0x09;
}