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
    
    MOVF    PORTB, W
    ANDLW   0X0F
    MOVWF   InB
    
    ADDWF   InA,W
    MOVWF   Result
    MOVFF   Result, PORTD
    
    BCF	    PORTD,7
    BNZ	    Z_CHECK_DONE2
    BSF	    PORTD,7
    
Z_CHECK_DONE2:
    GOTO    MAIN_LOOP
    
    END

