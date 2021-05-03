#include <p18f4620.h>
#include "Main.h"
#include "Fan_Support.h"
#include "stdio.h"

extern char FAN;
extern char duty_cycle;


int get_RPM()
{
    int RPS = TMR3L / 2;                    // read the count. Since there are 2 pulses per rev
                                            // then RPS = count /2
    TMR3L = 0;                              // clear out the count
    return (RPS * 60);                      // return RPM = 60 * RPS
}

void Toggle_Fan()
{
    if (FAN == 0)
    {
        Turn_On_Fan();
    }
    else
    {
        Turn_Off_Fan();
    }
}

void Turn_Off_Fan()
{
    FAN = 0;
    FAN_EN = 0;
    FANEN_LED = 0;

}

void Turn_On_Fan()
{
    FAN = 1;
    do_update_pwm(duty_cycle);
    FAN_EN = 1;
    FANEN_LED = 1;   
}

void Increase_Speed()
{
    if (duty_cycle == 100)
    {
        Do_Beep();
        Do_Beep();
    }
    else
    {
        duty_cycle = duty_cycle + 5;
    }
}

void Decrease_Speed()
{
    if (duty_cycle == 0)
    {
        Do_Beep();
        Do_Beep();
    }
    else
    {
        duty_cycle = duty_cycle - 5;
    }
}

void do_update_pwm(char duty_cycle) 
{ 
    float dc_f;
    int dc_I;
    PR2 = 0b00000100 ;                      // set the frequency for 25 Khz
    T2CON = 0b00000111 ;                    //
    dc_f = ( 4.0 * duty_cycle / 20.0) ;     // calculate factor of duty cycle versus a 25 Khz
                                            // signal
    dc_I = (int) dc_f;                      // get the integer part
    if (dc_I > duty_cycle) dc_I++;          // round up function
    CCP1CON = ((dc_I & 0x03) << 4) | 0b00001100;
    CCPR1L = (dc_I) >> 2;
}

void Set_DC_RGB(int duty_cycle)
{
    if (duty_cycle == 0)
    {
        PORTEbits.RE0 = 0;                  //D1 red to off
        PORTEbits.RE2 = 0;                  //D1 green to off
    }
    else if (duty_cycle > 0 && duty_cycle < 33)
    {
        PORTEbits.RE0 = 0;                  //D1 red to off
        PORTEbits.RE2 = 1;                  //D1 green to on
    }
    else if (duty_cycle >= 33 && duty_cycle < 66)
    {
        PORTEbits.RE0 = 1;                  //D1 red to on
        PORTEbits.RE2 = 1;                  //D1 green to on
    }
    else if (duty_cycle >= 67)
    {
        PORTEbits.RE0 = 1;                  //D1 red to on
        PORTEbits.RE2 = 0;                  //D1 green to off
    }
}

void Set_RPM_RGB(int rpm)
{
    if (rpm == 0)
    {
        PORTBbits.RB4 = 0;                  //D2 red to off
        PORTBbits.RB5 = 0;                  //D2 blue to off
    }
    else if (rpm > 0 && rpm < 1200)
    {
        PORTBbits.RB4 = 0;                  //D2 red to off
        PORTBbits.RB5 = 1;                  //D2 blue to on
    }
    else if (rpm >= 1200 && rpm < 2400)
    {
        PORTBbits.RB4 = 1;                  //D2 red to on
        PORTBbits.RB5 = 1;                  //D2 blue to on
    }
    else if (rpm >= 2400)
    {
        PORTBbits.RB4 = 1;                  //D2 red to on
        PORTBbits.RB5 = 0;                  //D2 blue to off
    }
}

void Do_Beep()
{
    Activate_Buzzer();
    Wait_One_Sec();
    Deactivate_Buzzer();
    do_update_pwm(duty_cycle);
}

void Wait_One_Sec()
{
    for (int j=0; j<0xffff; j++);           //Delay 1 second
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

