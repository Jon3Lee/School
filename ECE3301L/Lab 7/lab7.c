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

#define SEC_LED PORTDbits.RD7               //Define SEC_LED as Port D bit 7
#define SEG_A PORTEbits.RE1
#define MODE_LED PORTBbits.RB7

#define NS_RED PORTAbits.RA5
#define NS_GREEN PORTBbits.RB0

#define NSLT_RED PORTBbits.RB1
#define NSLT_GREEN PORTBbits.RB2

#define EW_RED PORTBbits.RB4
#define EW_GREEN PORTBbits.RB5

#define EWLT_RED PORTEbits.RE0
#define EWLT_GREEN PORTEbits.RE2

#define NSPED_SW PORTAbits.RA1
#define NSLT_SW PORTAbits.RA2
#define EWPED_SW PORTAbits.RA3
#define EWLT_SW PORTAbits.RA4

#define OFF 0
#define RED 1
#define GREEN 2
#define YELLOW 3

void init_UART();
void putch(char);
void Wait_One_Second();
void Wait_Half_Second();
void Activate_Buzzer();
void Deactivate_Buzzer();

char array[10] = {0x01, 0x4F, 0x12, 0x06, 0x4C, 0x24, 0x20, 0x0F, 0x00, 0x04};

void init_UART()
{
    OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF &
    USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX &
    USART_BRGH_HIGH, 25);
    OSCCON = 0x60;                              //Set to 4 MHz
}

void putch (char c)
{
    while (!TRMT);
    TXREG = c;
}

void Wait_One_Second()
{
    SEC_LED = 0;                                // First, turn off the SEC LED
    Wait_Half_Second();                         // Wait for half second (or 500 msec)
    SEC_LED = 1;                                // then turn on the SEC LED
    Wait_Half_Second();                         // Wait for half second (or 500 msec)
}

void Wait_Half_Second()
{
    T0CON = 0x02;                               // Timer 0, 16-bit mode, prescaler 1:8
                                                // Subtract F424 from FFFF = 0x0BDB
    TMR0L = 0xDB;                               // set the lower byte of TMR to 0xDB
    TMR0H = 0x0B;                               // set the upper byte of TMR to 0x0B
    INTCONbits.TMR0IF = 0;                      // clear the Timer 0 flag
    T0CONbits.TMR0ON = 1;                       // Turn on the Timer 0
    while (INTCONbits.TMR0IF == 0);             // wait for the Timer Flag to be 1 for done
    T0CONbits.TMR0ON = 0;                       // turn off the Timer 0
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

void Wait_N_Seconds(char seconds)
{
char I;
    for (I = 0; I< seconds; I++)
    {
        Wait_One_Second();
    }
}

void Wait_One_Second_With_Beep()
{
    SEC_LED = 1;                                    // First, turn on the SEC LED
    Activate_Buzzer();                              // Activate the buzzer
    Wait_Half_Second();                             // Wait for half second (or 500 msec)
    SEC_LED = 0;                                    // then turn off the SEC LED
    Deactivate_Buzzer();                            // Deactivate the buzzer
    Wait_Half_Second();                             // Wait for half second (or 500 msec)
}

void Set_NS(char color)                             //This is the D1 LED
{
    switch (color)
    {
        case OFF: NS_RED =0;NS_GREEN=0;break;       // Turns off the NS LED
        case RED: NS_RED =1;NS_GREEN=0;break;       // Sets NS LED RED
        case GREEN: NS_RED =0;NS_GREEN=1;break;     // sets NS LED GREEN
        case YELLOW: NS_RED =1;NS_GREEN=1;break;    // sets NS LED YELLOW
    }
}

void Set_NSLT(char color)                           //This is the D3 LED
{
    switch (color)
    {
        case OFF: NSLT_RED =0;NSLT_GREEN=0;break;    // Turns off the NSLT LED
        case RED: NSLT_RED =1;NSLT_GREEN=0;break;    // Sets NSLT LED RED
        case GREEN: NSLT_RED =0;NSLT_GREEN=1;break;  // sets NSLT LED GREEN
        case YELLOW: NSLT_RED =1;NSLT_GREEN=1;break; // sets NSLT LED YELLOW
    }
}

void Set_EW(char color)                             //This is the D2 LED
{
    switch (color)
    {
        case OFF: EW_RED =0;EW_GREEN=0;break;       // Turns off the EW LED
        case RED: EW_RED =1;EW_GREEN=0;break;       // Sets EW LED RED
        case GREEN: EW_RED =0;EW_GREEN=1;break;     // sets EW LED GREEN
        case YELLOW: EW_RED =1;EW_GREEN=1;break;    // sets EW LED YELLOW
    }
}

void Set_EWLT(char color)                           //This is the D4 LED
{
    switch (color)
    {
        case OFF: EWLT_RED =0;EWLT_GREEN=0;break;   // Turns off the EWLT LED
        case RED: EWLT_RED =1;EWLT_GREEN=0;break;   // Sets EWLT LED RED
        case GREEN: EWLT_RED =0;EWLT_GREEN=1;break; // sets EWLT LED GREEN
        case YELLOW: EWLT_RED =1;EWLT_GREEN=1;break;// sets EWLT LED YELLOW
    }
}

void DO_DISPLAY_7SEG_Upper(char digit)
{
    PORTC = array[digit] & 0x3F;
    if ((array[digit] & 0x40) == 0x40)
    {
        SEG_A = 1;
    }
    else
    {
        SEG_A = 0;
    }

    if (digit == 0)
    {
        SEG_A = 0;
        PORTC = 0;
    }
}

void DO_DISPLAY_7SEG_Lower(char digit)
{
    PORTD = (PORTD & 0x80) | array[digit];
    if (digit == 0)
    {
        PORTD = 0x00;
    }
}

void PED_Control(char Direction, char Num_Sec)
{
    DO_DISPLAY_7SEG_Upper(0x00);
    DO_DISPLAY_7SEG_Lower(0x00);
    for(char i = Num_Sec - 1; i>0; i--)
    {
        if(Direction == 0)
        {
            DO_DISPLAY_7SEG_Upper(i);
        }
        else
        {
            DO_DISPLAY_7SEG_Lower(i);
        }
        Wait_One_Second_With_Beep();
    }
    DO_DISPLAY_7SEG_Upper(0x00);
    DO_DISPLAY_7SEG_Lower(0x00);
    Wait_One_Second_With_Beep();
}

unsigned int get_full_ADC(void)
{
int result;
    ADCON0bits.GO=1;                                  // Start Conversion
    while(ADCON0bits.DONE==1);                        // wait for conversion to be completed
    result = (ADRESH * 0x100) + ADRESL;               // combine result of upper byte and
                                                      // lower byte into result
    return result;                                    // return the result.
}

void Select_ADC_Channel(char channel)                                                       
{
    ADCON0 = channel * 4 + 1;
}

void Init_ADC(void) 
{
    ADCON1 = 0x0E;                                    // Since we're using up to AN5, we use 1001 for bits 3-0 to set them as analog, and 1 for bit 4,
                                                      // which is the VREF+ AN3 and 0 for bit 5, coming out to 011001
    ADCON2 = 0xA9;                                    // right justify the result. Set the bit conversion time (TAD) and
                                                      // acquisition time
}

void Night_Mode()
{
    Set_NSLT(RED);
    Set_EW(RED);
    Set_EWLT(RED);
    Set_NS(GREEN);

    Wait_N_Seconds(6);

    Set_NS(YELLOW);
    Wait_N_Seconds(3);

    Set_NS(RED);

    if (EWLT_SW == 1)
    {
        Set_EWLT(GREEN);
        Wait_N_Seconds(6);

        Set_EWLT(YELLOW);
        Wait_N_Seconds(3);

        Set_EWLT(RED);
    }
    
    Set_EW(GREEN);
    Wait_N_Seconds(6);

    Set_EW(YELLOW);
    Wait_N_Seconds(3);

    SET_EW(RED);
    

    if (NSLT_SW == 1)
    {
        Set_NSLT(GREEN);
        Wait_N_Seconds(8);

        Set_NSLT(YELLOW);
        Wait_N_Seconds(3);

        Set_NSLT(RED);
    }
    
}

void Day_Mode()
{
    Set_NS(GREEN);
    Set_EW(RED);
    Set_EWLT(RED);
    Set_NSLT(RED);

    if (NSPED_SW == 1)
    {
        PED_Control(0, 8);
    }

    Wait_N_Seconds(7);

    Set_NS(YELLOW);
    Wait_N_Seconds(3);

    Set_NS(RED);

    if (EWLT_SW == 1)
    {
        Set_EWLT(GREEN);
        Wait_N_Seconds(8);

        Set_EWLT(YELLOW);
        Wait_N_Seconds(3);

        Set_EWLT(RED);
    }

    Set_EW(GREEN);

    if (EWPED_SW == 1)
    {
        PED_Control(1, 9);
    }

    Wait_N_Seconds(9);

    Set_EW(YELLOW);
    Wait_N_Seconds(3);

    Set_EW(RED);

    if (NSLT_SW == 1)
    {
        Set_NSLT(GREEN);
        Wait_N_Seconds(8);
        
        Set_NSLT(YELLOW);
        Wait_N_Seconds(3);

        Set_NSLT(RED);
    }

}

void main(void)
{
    init_UART();
    Init_ADC();
    
    TRISA = 0X1F;                                   //Set TRISA to 
    TRISB = 0X00;                                                                      
    TRISC = 0X00;                                                                               
    TRISD = 0X00;                                                                               
    TRISE = 0X00;  

    Select_ADC_Channel(0);
                                    /*
                                    Find out how to work MODE = light sensor input
                                    MODE_LED is off at night time, on day time
                                    
                                    */
    while (1)
    {
        int num_step = get_full_ADC();
        float MODE = 4.0 * num_step;
        if (MODE < 2500) //Daytime mode below 2.5v
        {
            MODE_LED = 1;
            Day_Mode();
        }
        else
        {
            MODE_LED = 0;
            Night_Mode();
        }
    }

}

