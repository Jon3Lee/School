#include <P18F4620.inc>
config OSC = INTIO67
config WDT = OFF
config LVP = OFF
config BOREN = OFF

                ;DECLARING VARIABLES
InA    equ    0x20        ;Set variable InA to constant 0x20
InB    equ    0x21        ;Set variable InB to constant 0x21
Result    equ    0x22        ;Set variable Result to constant 0x22
 
    ORG 0x0000
    
START:
    MOVLW   0x0F        ;Put literal value 0x0F into WREG
    MOVWF   ADCON1        ;Move the value of W to ADCON1. This will make ADCON1 all digital.
    MOVLW   0XFF        ;Put literal value 0xfF into WREG
    MOVWF   TRISA        ;Move the value of W to TRISA. FF makes it so that PORTA is seen as all inputs.
    MOVWF   TRISC        ;Move the value of W to TRISC. FF makes it so that PORTC is seen as all inputs.
    MOVLW   0x00        ;Put literal value 0x00 into WREG
    MOVWF   TRISB        ;Move the value of W to TRISB. 00 makes it so that all of PORTB is seen as outputs.
    MOVWF   TRISE        ;Move the value of W to TRISE. 00 makes it so that all of PORTE is seen as outputs.
    
MAIN_LOOP:
    MOVF    PORTA, W        ;Read from PORTA and move value to WREG
    ANDLW   0x0F        ;AND value of W and literal 0x0F, store value in W, masking with 0x0F
    MOVWF   InA            ;Move value of W into variable InA
    COMF    InA, 0        ;Take one's complement of InA and store into W
    ANDLW   0x0F        ;AND value of W and literal 0x0F, store value in W
    MOVWF   Result        ;Move value of W into variable Result
    MOVFF   Result, PORTB    ;Take value from Result and move into PORTB
    
    BZ        SET_TO_1        ;If Result is 0, go to SET_TO_1
    
CLR_TO_0:
    BCF        PORTE, 0        ;Clear bit 0 in PORTE
    GOTO    MAIN_LOOP        ;Loop back to Main_LOOP
SET_TO_1:
    BSF        PORTE, 0        ;Set bit 0 in PORTE to 1
    GOTO    MAIN_LOOP        ;Loop back to Main_LOOP
    
    END


