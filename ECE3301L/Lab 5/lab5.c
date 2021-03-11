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

#define D1_RED PORTBbits.RB0
#define D1_GREEN PORTBbits.RB1
#define D1_BLUE PORTBbits.RB2

#define D2_RED PORTBbits.RB3
#define D2_GREEN PORTBbits.RB4
#define D2_BLUE PORTBbits.RB5

#define D3_RED PORTAbits.RA4
#define D3_GREEN PORTAbits.RA5

unsigned int get_full_ADC(void);

void Init_ADC(void);
void Select_ADC_Channel(char channel);

char array[10] = {0x01, 0x4F, 0x12, 0x06, 0x4C, 0x24, 0x20, 0x0F, 0x00, 0x04};             //This array dictates which digits display on the 7 segment display


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

void Init_ADC(void) 
{
    ADCON1 = 0x1B;                                                                          // select pins AN0 through AN3 as analog signal, VDD-VSS as
                                                                                            // reference voltage
    ADCON2 = 0xA9;                                                                          // right justify the result. Set the bit conversion time (TAD) and
                                                                                            // acquisition time
}

void Select_ADC_Channel(char channel)
{
    ADCON0 = channel * 4 + 1;
}

void WAIT_1_SEC()                                                                           //This for loop from 0 to FFFF causes the PIC18F to wait one second.
{
    for (unsigned int j=0; j<0xffff; j++);
}

unsigned int get_full_ADC(void)
{
int result;
    ADCON0bits.GO=1;                                                                        // Start Conversion
    while(ADCON0bits.DONE==1);                                                              // wait for conversion to be completed
    result = (ADRESH * 0x100) + ADRESL;                                                     // combine result of upper byte and
                                                                                            // lower byte into result
    return result;                                                                          // return the result.
}

void Display_Lower_Digit(char digit)
{
    PORTD = array[digit];                                                                   //PORTD goes to the lower digit and just takes from the array value to display its digit
}

void Display_Upper_Digit(char digit)
{
    PORTC = array[digit];                                                                   //PORTC goes to the upper digit of the 7 segment display, but segment A
                                                                                            //on the display goes to PORTE.
    if (array[digit] & 0x40)                                                                //Mask off the 6th bit 
    {                                                                                       //If the 6th bit is there
        PORTE = 0x02;                                                                       //PORTE is off 
    }
    else                                                                                    //else
    {
        PORTE = 0x00;                                                                       //PORTE is on
    }
}
                                                                                            //BEGIN DECLARING COLORS for D1, D2, and D3
void SD1_OFF()
{
    D1_RED = 0;
    D1_BLUE = 0;
    D1_GREEN = 0;
}
void SD1_RED()
{
    D1_RED = 1;
    D1_BLUE = 0;
    D1_GREEN = 0;
}
void SD1_GREEN()
{
    D1_RED = 0;
    D1_BLUE = 0;
    D1_GREEN = 1;
}
void SD1_YELLOW()
{
    D1_RED = 1;
    D1_BLUE = 0;
    D1_GREEN = 1;
}
void SD1_BLUE()
{
    D1_RED = 0;
    D1_BLUE = 1;
    D1_GREEN = 0;
}
void SD1_PURPLE()
{
    D1_RED = 1;
    D1_BLUE = 1;
    D1_GREEN = 0;
}
void SD1_CYAN()
{
    D1_RED = 0;
    D1_BLUE = 1;
    D1_GREEN = 1;
}
void SD1_WHITE()
{
    D1_RED = 1;
    D1_BLUE = 1;
    D1_GREEN = 1;
}
void SD2_OFF()
{
    D2_RED = 0;
    D2_BLUE = 0;
    D2_GREEN = 0;
}
void SD2_RED()
{
    D2_RED = 1;
    D2_BLUE = 0;
    D2_GREEN = 0;
}
void SD2_GREEN()
{
    D2_RED = 0;
    D2_BLUE = 0;
    D2_GREEN = 1;
}
void SD2_BLUE()
{
    D2_RED = 0;
    D2_BLUE = 1;
    D2_GREEN = 0;
}
void SD2_WHITE()
{
    D2_RED = 1;
    D2_BLUE = 1;
    D2_GREEN = 1;
}
void SD3_RED()                                                                                  //Since D3 does not have a D3_BLUE defined and it is not used, we do not need it here.
{
    D3_RED = 1;
    D3_GREEN = 0;
}
void SD3_GREEN()
{
    D3_RED = 0;
    D3_GREEN = 1;
}
void SD3_YELLOW()
{
    D3_RED = 1;
    D3_GREEN = 1;
}
void main(void)
{
    Init_ADC();
    init_UART();
    TRISA = 0X0F;                                                                               //2 bits of port A connect to a LED, the others are input by temperature sensor and photoresistor
    TRISB = 0X00;                                                                               //TRISB leads to a RGB LED so set to output
    TRISC = 0X00;                                                                               //Set TRISC to output because it goes to a 7-Segment LED, which is always output
    TRISD = 0X00;                                                                               //Set TRISD to output because it goes to a 7-Segment LED, which is always output
    TRISE = 0X00;                                                                               //Set TRISE to output because it goes to a 7-Segment LED, which is always output

    PORTA = 0x00;                                                                               //Set PORTA to 0x00, making PORTA all inputs. 
    PORTB = 0x00;                                                                               //Set PORTB to 0x00, making PORTB all inputs.
    PORTC = 0xFF;                                                                               //Set PORTC to 0xFF, making PORTC all outputs.
    PORTD = 0xFF;                                                                               //Set PORTC to 0xFF, making PORTC all outputs.
    PORTE = 0xFF;                                                                               //Set PORTC to 0xFF, making PORTC all outputs.



    while (1)
    {
        Select_ADC_Channel (0);                                                                 //Set ADCON0 to 1
    int num_step = get_full_ADC();                                                          
    float voltage_mv = num_step * 4.0;                                                      //Set voltage_mv to the voltage in millivolts
    float temperature_C = (1035.0 - voltage_mv) / 5.50;                                     //Celcius is calculted
    float temperature_F = 1.80 * temperature_C + 32.0;                                      //Convert the celcius into fahrenheit
    int tempF = (int) temperature_F;                                                        //Convert the temperature_F from float to integer
    char U = tempF / 10;                                                                    //The upper digit of the Fahrenheit is the Fahrenheit divided by 10, since it only returns the upper digit.
    char L = tempF % 10;                                                                    //The lower digit is the modulus of Fahrenheit divided by 10, since it only returns the remainder.
        Display_Upper_Digit(U);                                                                 //Display the upper digit onto the display.
        Display_Lower_Digit(L);                                                                 //Display the lower digit onto the display.
        
        

        //BEGIN D1
        if (tempF > 69)
        {                                                                                       //For D1, the light changes every 10 degrees. We can see that
            SD1_WHITE();                                                                        //the upper digit U also changes with every 10 degrees. So, we use
        }                                                                                       //U as the input to portB to change colors. 
        else
        {
            PORTB = U;
        }
        
            
        

        //BEGIN D2                                                                              //For D2, the light changes colors as the temperature changes,
        if (tempF < 45)                                                                         //so we just used an if statement to make the colors change.
        {
            SD2_OFF();
        }
        else if (tempF > 45 && tempF < 56)
        {
            SD2_RED();
        }
        else if (tempF > 55 && tempF < 66)
        {
            SD2_GREEN();
        }
        else if (tempF > 65 && tempF < 76)
        {
            SD2_BLUE();
        }
        else
        {
            SD2_WHITE();
        }



        Select_ADC_Channel (2);                                                                 
    int nstep = get_full_ADC();
    float vmv = 4.0 * nstep;
    int light_volt = (int) vmv;                                                                //Change vmv from a float to integer, it will now display voltage in millivolts.
        
        //BEGIN D3
        if (vmv < 2500)                                                                             //D3 will change colors as the voltage changes.
        {
            SD3_RED();
        }
        else if (vmv > 2499 && vmv < 3400)
        {
            SD3_GREEN();
        }
        else
        {
            SD3_YELLOW();
        }
        

        printf("Temperature = %dF    Light Volt = %dmV \r\n", tempF, light_volt);
        WAIT_1_SEC();                                                                               //Wait 1 second before repeating
    }

}

