#include <P18F4620.inc>
config OSC = INTIO67
config WDT = OFF
config LVP = OFF
config BOREN = OFF
    Delay1	equ	0xFF
    Delay2	equ	0xFF
ORG 0X0000                  
START:
    ORG	  0x0000            
START:
    MOVLW 0x0F		        ; Put literal value 0x0F into WREG
    MOVWF ADCON1	        ; Move the value of W to ADCON1. This will make ADCON1 all digital.
    MOVLW 0xFF		        ; Put literal value 0xFF into WREG
    MOVWF TRISA		        ; Move the value of W to TRISA. FF makes it so that PORTA is seen as all inputs.
    MOVLW 0x00		        ; Put literal value 0x00 into WREG
    MOVWF TRISC		        ; Move the value of W to TRISC. 00 makes it so that all of PORTC is seen as outputs.
   
MAIN_LOOP:		            ; start of While LOOP
    MOVLW 0x00		        ; Put literal value 0x00 into WREG
    MOVWF 0x20		        ; Move the value of W into the address 0x20. This will be the color of the LED.
    MOVLW 0x08		        ; Put literal value 0x08 into WREG
    MOVWF 0x21		        ; Move the value of W into the address 0x21. This will be the counter. 
    
FOR_LOOP:
    MOVF 0x20,W		        ; read content of 0x20, from line 21, and load this value into W.
    MOVWF PORTC		        ; the value of W is then moved into PORTC, where PORTC will output the colors. 
    CALL DELAY_ONE_SEC	    ; wait one second before continuing
    
    INCF 0x20,F		        ; increment location 0x20 by 1, changing the color of the LED.
    DECF 0x21,F		        ; decrement location 0x21 by 1, decreasing the counter by 1.
    BNZ  FOR_LOOP	        ; if the counter is not 0, go back to the beginning of the FOR_LOOP.
			                ; 
    GOTO MAIN_LOOP	        ; else go back to the MAIN_LOOP to reinitialize the values. 
    
DELAY_ONE_SEC:
    MOVLW   Delay1	        ; Load constant Delay1 into WREG
    MOVWF   0x28	        ; Load Delay1 from WREG to memory 0x28
    
LOOP_1_OUTER:
    NOP			            ; Do nothing
    MOVLW   Delay2	        ; Load constant Delay2 into WREG
    MOVWF   0x29	        ; Load Delay2 from WREG to memory 0x29
    
LOOP_1_INNER:
    NOP			            ; Do nothing
    DECF    0x29,F	        ; Decrement memory location 0x29
    BNZ	    LOOP_1_INNER    ; If value not zero, go back to the top of LOOP_1_INNER
    DECF    0x28,F	        ; Decrement memory location 0x28
    BNZ	    LOOP_1_OUTER    ; If value not zero, go back to the top of LOOP_1_OUTER
    RETURN
    END