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
    MOVWF   TRISC ;
    MOVWF   TRISD ;
 
    MOVLW   0xFF ; input ports
    MOVWF   TRISA ;
    MOVWF   TRISB ;
 
    MOVLW   0x07 ;
    MOVWF   TRISE ;
 
MAIN_LOOP:
    MOVF    PORTA, W
    ANDLW   0x0F
    MOVWF   InA
    
    COMF    InA, W
    ANDLW   0x0F
    MOVWF   Result
    MOVFF   Result, PORTC
    

    
;DONE:
    GOTO    MAIN_LOOP
    
    END

