#include <P18F4620.inc> 
 config OSC = INTIO67 
 config WDT = OFF 
 config LVP = OFF 
 config BOREN = OFF 

InA	    EQU	    0x20 
InB	    EQU	    0x21 
Result	    EQU	    0x22 
 
    ORG	    0x0000 
 
START:
    MOVLW   0x0F ; 
    MOVWF   ADCON1 ; ADCON1 = 0x0F
 
    MOVLW   0x00 ; output ports
    MOVWF   TRISC ; set port C as output
    MOVWF   TRISD ; set port D as output
 
    MOVLW   0xFF ; input ports
    MOVWF   TRISA ; set Port A as input
    MOVWF   TRISB ; set port B as output
 
    MOVLW   0x07 ;
    MOVWF   TRISE ; set port E as input
 
MAIN_LOOP:
    MOVF    PORTA, W ;store inpout from port A into WREG
    ANDLW   0x0F ;Mask upper 4 bits
    MOVWF   InA ; store WREG value into InA
    
    COMF    InA, W; Compliment InA with value from WREG
    ANDLW   0x0F ; Mask upper 4 bits
    MOVWF   Result ; Store value from WREG into Result
    MOVFF   Result, PORTC; move value from Result into Port C
    

    
;DONE:
    GOTO    MAIN_LOOP; return to main loop to keep program continuous 
    
    END

