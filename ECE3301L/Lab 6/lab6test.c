#include <p18f4620.h>
#include <stdio.h>
#include <math.h>
#include <usart.h>

#pragma config OSC = INTIO67
#pragma config WDT=OFF
#pragma config LVP=OFF
#pragma config BOREN =OFF
#pragma config CCP2MX = PORTBE 

#define D1_RED      PORTBbits.RB0
#define D1_GREEN    PORTBbits.RB1
#define D1_BLUE     PORTBbits.RB2

#define D2_RED      PORTBbits.RB3
#define D2_GREEN    PORTBbits.RB4
#define D2_BLUE     PORTEbits.RE2

#define E1          PORTEbits.RE1
#define DP          PORTDbits.RD7

unsigned int num_step;
float voltage;
float RL;
float RREF = 100;

void init_UART();
void putch (char);
void Init_ADC(void);
unsigned int Get_Full_ADC(void);
void Select_ADC_Channel(char);
void Display_Upper_Digit(char);
void Display_Lower_Digit(char);

char array[10] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};

void WAIT_1_SEC()
{
    for (int j=0;j<17000;j++);
}

void putch (char c)
{
    while (!TRMT);
    TXREG = c;
}

void Select_ADC_Channel(char channel)
{
    ADCON0 = channel * 4 + 1;
}

void init_UART()
{
    OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF &
    USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX &
    USART_BRGH_HIGH, 25);
    OSCCON = 0x60;
}

void Display_Lower_Digit(char digit)
{
    PORTD = array[digit];
}

void Display_Upper_Digit(char digit)
{
    PORTC = array[digit] & 0x3F;
    if ((array[digit] & 0x40 ) == 0x40)
        E1 = 1;
    else
        E1 = 0;
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

void SET_D1_WHITE()
{
D1_RED = 1;
D1_GREEN = 1;
D1_BLUE = 1;
}

void SET_D1_NOLIGHT()
{
D1_RED = 0;
D1_GREEN = 0;
D1_BLUE = 0;
}

void SET_D2_WHITE()
{
D2_RED = 1;
D2_GREEN = 1;
D2_BLUE = 1;
}

void SET_D2_NOLIGHT()
{
D2_RED = 0;
D2_GREEN = 0;
D2_BLUE = 0;
}

void main(void)
{
    Init_ADC();             // initialize the A2D converter
    init_UART();
    TRISA = 0x3F;
    TRISB = 0x00;
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE = 0x01;
    
    while(1)
    {
        Select_ADC_Channel(1);
        num_step = Get_Full_ADC();
        voltage = (num_step * 4.0) / 1000;
        RL = (voltage /(4.096 - voltage)) * (RREF/1000);
        if (RL < 10)
        {
            char U = (int) RL;
            char L = (int) ((RL - U) * 10);
                    
            Display_Upper_Digit(U);
            Display_Lower_Digit(L);
            DP = 0;
            
            SET_D1_NOLIGHT();
            
            WAIT_1_SEC();
        }
        else
        {
            int R = (int) RL;
            char U = R / 10;
            char L = R % 10;
            
            Display_Upper_Digit(U);
            Display_Lower_Digit(L);
            DP = 1;
            
            if (RL > 69)    {SET_D1_WHITE();}
            else            PORTB = U;
            
            WAIT_1_SEC();
        }

        if (RL < 0.070)
        {
            SET_D2_WHITE();
            Activate_Buzzer();
        }
        else
        {
            SET_D2_NOLIGHT();
            Deactivate_Buzzer();
        }
        
        
        printf("r = %f \r\n", RL);
    }
}

void Init_ADC(void)
{
    ADCON1 = 0x19; // select pins AN0 through AN3 as analog signal, VDD-VSS as
    // reference voltage
    ADCON2 = 0xA9; // right justify the result. Set the bit conversion time (TAD) and
    // acquisition time
}

unsigned int Get_Full_ADC(void)
{
    int result;
    ADCON0bits.GO=1;                        // Start Conversion
    while(ADCON0bits.DONE==1);              // wait for conversion to be completed
    result = (ADRESH * 0x100) + ADRESL;     // combine result of upper byte and
                                            // lower byte into result
    return result;                          // return the result.
}