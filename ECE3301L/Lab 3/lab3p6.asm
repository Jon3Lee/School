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
    MOVLW 0x00		        ; Put literal value 0x00 into WREG
    MOVWF TRISC		        ; Move the value of W to TRISC. 00 makes it so that all of PORTC is seen as outputs.
    MOVLW 0X00		        ; Put literal value 0x00 into WREG
    MOVWF TRISD		        ; Move the value of W to TRISD. 00 makes it so that all of PORTD is seen as outputs.
			                ; Array begins here. This array will determine the colors of both of the LEDs from PORTD.
    MOVLW 0xA6		        ; Put literal value 0xA6 into WREG
    MOVWF 0x30		        ; store the value in WREG to address 0x30
    MOVLW 0x23		        ; Put literal value 0x23 into WREG
    MOVWF 0x31		        ; store the value in WREG to address 0x31
    MOVLW 0x05		        ; Put literal value 0x05 into WREG
    MOVWF 0x32		        ; store the value in WREG to address 0x32
    MOVLW 0x82		        ; Put literal value 0x82 into WREG
    MOVWF 0x33		        ; store the value in WREG to address 0x33
    MOVLW 0xC0		        ; Put literal value 0xC0 into WREG
    MOVWF 0x34		        ; store the value in WREG to address 0x34
    MOVLW 0x67		        ; Put literal value 0x67 into WREG
    MOVWF 0x35		        ; store the value in WREG to address 0x35
    MOVLW 0x41		        ; Put literal value 0x41 into WREG
    MOVWF 0x36		        ; store the value in WREG to address 0x36
    MOVLW 0xE4		        ; Put literal value 0xE4 into WREG
    MOVWF 0x37		        ; store the value in WREG to address 0x37
   
MAIN_LOOP:		            ; start of While LOOP
    MOVLW 0x30		        ; Put literal value 0x30 into WREG
    MOVWF FSR0L		        ; store the value in WREG to address FSR0L, the lower bits of the file service register.
    MOVLW 0x00		        ; Put literal value 0x00 into WREG
    MOVWF FSR0H		        ; store the value in WREG to address FSR0H, the high bits of the file service register.
    
    MOVLW 0x00		        ; Put literal value 0x00 into WREG
    MOVWF 0x20		        ; Move the value of WREG into the address 0x20. This will be the color of the PORTC LED.
    MOVLW 0x08		        ; Put literal value 0x08 into WREG
    MOVWF 0x21		        ; Move the value of WREG into the address 0x21. This will be the counter.

    
FOR_LOOP:
    MOVF 0x20,W		        ; Move values from 0x20 to WREG.
    MOVWF PORTC		        ; the value of WREG is then moved into PORTC, where PORTC will output the colors.
    
    MOVF INDF0,W	        ; Move values from INDF0 into W. This will read from the array.
    MOVWF PORTD		        ; Read WREG, which has the values of the array, into PORTD. This will output colors for both LEDs
                            ; in PORTD.
    
    CALL DELAY_ONE_SEC	    ; wait one second
    INCF FSR0L		        ; increment FSR0L, which is the file service register by 1
    INCF 0x20,F		        ; increment location 0x20 by 1, changing the colors of PORTC
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