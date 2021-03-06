#include <P18F4620.inc>
config OSC = INTIO67
config WDT = OFF
config LVP = OFF
config BOREN = OFF

				;DECLARING VARIABLES
InA	equ	0x20		;Set variable InA to constant 0x20
InCC	equ	0x21		;Set variable InCC to constant 0x21 since InC cannot be used.
Result	equ	0x22		;Set variable Result to constant 0x22
 
	ORG 0x0000
	
START:
    MOVLW   0x0F		;Put literal value 0x0F into WREG
    MOVWF   ADCON1		;Move the value of W to ADCON1. This will make ADCON1 all digital.
    MOVLW   0XFF		;Put literal value 0xFF into WREG
    MOVWF   TRISA		;Move the value of W to TRISA. FF makes it so that PORTA is seen as all inputs.
    MOVWF   TRISC		;Move the value of W to TRISC. FF makes it so that PORTC is seen as all inputs.
    MOVLW   0x00		;Put literal value 0x00 into WREG
    MOVWF   TRISB		;Move the value of W to TRISB. 00 makes it so that all of PORTB is seen as outputs.
    MOVWF   TRISE		;Move the value of W to TRISE. 00 makes it so that all of PORTE is seen as outputs.
    MOVLW   0x07		;Put literal value 0x07 into WREG
    MOVWF   TRISD		;Move the value of W to TRISD. makes it so that the first 3 bits are
				;inputs while the rest of the bits are outputs.
     
MAIN_LOOP:
    BTFSC   PORTD,2		;If bit 2 in PORTD is 0, skip next line
    GOTO    PORTD1xx		;Executes PORTD1xx
    GOTO    PORTD0xx		;Executes PORTD0xx

PORTD1xx:
    GOTO    TASK_BCD		;Executes if bit 2 in PORTD is 1, since BCD is the only one with bit 2 as 1
				
PORTD0xx:
    BTFSC   PORTD,1		;If bit 1 in PORTD is 0, skip next line
    GOTO    PORTD01x		;Executes PORTD01x
    GOTO    PORTD00x		;Executes PORTD00x
    
PORTD01x:
    BTFSC   PORTD,0		;If bit 0 in PORTD is 0, skip next line
    GOTO    TASK_XOR		;Go to XOR task
    GOTO    TASK_AND		;Go to AND task
    
PORTD00x:
    BTFSC   PORTD,0		;If bit 0 in PORTD is 0, skip next line
    GOTO    TASK_ADD		;Go to Add task
    GOTO    TASK_COMP		;Go to Comp task
    
TASK_BCD:
    BSF	    PORTD, 5		;Set bit 5 of PORTD
    BCF	    PORTD, 4		;Clear bit 4 of PORTD
    BCF	    PORTD, 3		;Clear bit 3 of PORTD
    CALL    SUBROUTINE_BCD	;Call BCD routine
    GOTO    MAIN_LOOP		;Go back to MAIN_LOOP

TASK_COMP:
    BCF	    PORTD, 5		;Clear bit 5 of PORTD
    BCF	    PORTD, 4		;Clear bit 4 of PORTD
    BCF	    PORTD, 3		;Clear bit 3 of PORTD
    CALL    SUBROUTINE_COMP	;Call COMP routine
    GOTO    MAIN_LOOP		;Go back to MAIN_LOOP
    
TASK_ADD:
    BCF	    PORTD, 5		;Clear bit 5 of PORTD
    BCF	    PORTD, 4		;Clear bit 4 of PORTD
    BSF	    PORTD, 3		;Set bit 3 of PORTD
    CALL    SUBROUTINE_ADD	;Call ADD routine
    GOTO    MAIN_LOOP		;Go back to MAIN_LOOP
    
TASK_AND:
    BCF	    PORTD, 5		;Clear bit 5 of PORTD
    BSF	    PORTD, 4		;Set bit 4 of PORTD
    BCF	    PORTD, 3		;Clear bit 3 of PORTD
    CALL    SUBROUTINE_AND	;Call AND routine
    GOTO    MAIN_LOOP		;Go back to MAIN_LOOP
    
TASK_XOR:
    BCF	    PORTD, 5		;Clear bit 5 of PORTD
    BSF	    PORTD, 4		;Set bit 4 of PORTD
    BSF	    PORTD, 3		;Set bit 3 of PORTD
    CALL    SUBROUTINE_XOR	;Call XOR routine
    GOTO    MAIN_LOOP		;Go back to MAIN_LOOP

SUBROUTINE_COMP:
    MOVF    PORTA, W		;Read from PORTA and move value to WREG
    ANDLW   0x0F		;AND value of W and literal 0x0F, store value in W
    MOVWF   InA			;Move value of W into variable InA
    COMF    InA, 0		;Take one's complement of InA and store into W
    ANDLW   0x0F		;AND value of W and literal 0x0F, store value in W
    MOVWF   Result		;Move value of W into variable Result
    MOVFF   Result, PORTB	;Take value from Result and move into PORTB
    
    BCF	    PORTE, 0		;Clear bit 0 of PORTE
    BNZ	    JUMP		;Branch to JUMP
    BSF	    PORTE, 0		;Set bit 0 of PORTE

JUMP:
    GOTO    MAIN_LOOP		;Loop back to start of code
    
SUBROUTINE_ADD:
    MOVF    PORTA, W		;Read from PORTA and move value to WREG
    ANDLW   0x0F		;AND value of W and literal 0x0F, store value in W, masking with 0x0F
    MOVWF   InA			;Move value of W into variable InA
    
    MOVF    PORTC, W		;Read from PORTC and move value to WREG
    ANDLW   0x0F		;AND value of W and literal 0x0F, store value in W, masking with 0x0F
    MOVWF   InCC		;Move value of W into variable InCC
    
    ADDWF   InA, 0		;Add  InA with W, which holds the value of PORTC
    MOVWF   Result		;Move value of W into variable Result
    MOVFF   Result, PORTB	;Take value from Result and move into PORTB
    
    BCF	    PORTE, 0		;Clear bit 0 of PORTE
    BNZ	    JUMP1		;Branch to JUMP1
    BSF	    PORTE, 0		;Set bit 0 of PORTE

JUMP1:
    GOTO    MAIN_LOOP		;Loop back to start of code
    
SUBROUTINE_AND:
    MOVF    PORTA, W		;Read from PORTA and move value to WREG
    ANDLW   0x0F		;AND value of W and literal 0x0F, store value in W, masking with 0x0F
    MOVWF   InA			;Move value of W into variable InA
    
    MOVF    PORTC, W		;Read from PORTC and move value to WREG
    ANDLW   0x0F		;AND value of W and literal 0x0F, store value in W, masking with 0x0F
    MOVWF   InCC		;Move value of W into variable InC
    
    ANDWF   InA, 0		;AND InA with W, which holds the value of PORTC
    MOVWF   Result		;Move value of W into variable Result
    MOVFF   Result, PORTB	;Take value from Result and move into PORTB
    
    BCF	    PORTE, 0		;Clear bit 0 of PORTE
    BNZ	    JUMP2		;Branch to JUMP2
    BSF	    PORTE, 0		;Set bit 0 of PORTE

JUMP2:
    GOTO    MAIN_LOOP		;Loop back to start of code
      
SUBROUTINE_XOR:
    MOVF    PORTA, W		;Read from PORTA and move value to WREG
    ANDLW   0x0F		;AND value of W and literal 0x0F, store value in W, masking with 0x0F
    MOVWF   InA			;Move value of W into variable InA
    
    MOVF    PORTC, W		;Read from PORTC and move value to WREG
    ANDLW   0x0F		;AND value of W and literal 0x0F, store value in W, masking with 0x0F
    MOVWF   InCC		;Move value of W into variable InC
    
    XORWF   InA, 0		;XOR InA with W, which holds the value of PORTC
    MOVWF   Result		;Move value of W into variable Result
    MOVFF   Result, PORTB	;Take value from Result and move into PORTB
    
    BCF	    PORTE, 0		;Clear bit 0 of PORTE
    BNZ	    JUMP3		;Branch to JUMP3
    BSF	    PORTE, 0		;Set bit 0 of PORTE

JUMP3:
    GOTO    MAIN_LOOP		;Loop back to start of code
    
SUBROUTINE_BCD:
    MOVF    PORTA, W		;Read from PORTA and move value to WREG
    ANDLW   0x0F		;AND value of W and literal 0x0F, store value in W, masking with 0x0F
    MOVWF   InA			;Move value of W into variable InA
    MOVLW   0x09		;Put literal value 0x09 into WREG. This is equal to 9 decimal.
    
    CPFSGT  InA, 1		;Compare InA against the 0x09 in W. 
    GOTO    UNDER_9		;Executed if less than W
    GOTO    OVER_9		;Executed if greater or = W.

UNDER_9:
    MOVFF   InA, PORTB
    
    BCF	    PORTE, 0		;Clear bit 0 of PORTE
    BNZ	    JUMP4		;Branch to JUMP4
    BSF	    PORTE, 0		;Set bit 0 of PORTE

JUMP4:
    GOTO    MAIN_LOOP		;Loop back to start of code
    
OVER_9:
    MOVF    InA, W		;Stores the value of InA into W
    ADDLW   0x06		;Adds literal value 0x06 to W
    
    MOVWF   Result		;Move value of W into variable Result
    MOVFF   Result, PORTB	;Take value from Result and move into PORTB
    
    BCF	    PORTE, 0		;Clear bit 0 of PORTE
    BNZ	    JUMP5		;Branch to JUMP5
    BSF	    PORTE, 0		;Set bit 0 of PORTE

JUMP5:
    GOTO    MAIN_LOOP		;Loop back to start of code
    
    END


