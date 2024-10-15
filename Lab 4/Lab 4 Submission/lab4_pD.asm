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
    
    MOVF    PORTB, W ;store inpout from port B into WREG
    ANDLW   0X0F ;Mask upper 4 bits
    MOVWF   InB ; store WREG value into InB
    
    MOVF   InA,W ;  Move InA into WREG
    ANDWF   InB, W; Logical AND InB and WREG value and store in WREG
    MOVWF   Result; Store WREG value in Result
    MOVFF   Result, PORTD ;Output Result to Port D
    
    BCF	    PORTD,7 ; Clear bit 7 on port D
    BNZ	    Z_CHECK_DONE2 ;Branch to Z_CHECK_DONE1 if 1.
    BSF	    PORTD,7 ; Turn on LED if result is 0
    
Z_CHECK_DONE2:
    GOTO    MAIN_LOOP
    
    END

