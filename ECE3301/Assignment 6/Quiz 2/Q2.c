#include <xc.h>

void main()
{
	TRISC = 0;		
	T2CON = 0b00000100; 				
	TMR2 = 0; 				
	PR2 = 200;
	PIR1bits.TMR2IF = 0;			
	PORTCbits.RC0 = 0; 			
    PIE1bits.TMR2IE = 1;		
	while (PIR1bits.TMR2IF == 0); 	
	PORTCbits.RC0 = 1; 			
}
