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
    
    GOTO    MAIN_LOOP
    
SUB_COMP:
    MOVF    PORTA, W ;store inpout from port A into WREG
    ANDLW   0x0F ;Mask upper 4 bits
    MOVWF   InA ; store WREG value into InA
    
    COMF    InA, W; Compliment InA with value from WREG
    ANDLW   0x0F ; Mask upper 4 bits
    MOVWF   Result ; Store value from WREG into Result
    MOVFF   Result, PORTD; move value from Result into Port D
    
    BCF	    PORTD,7 ; Clear bit 7 on port D
    BNZ	    Z_CHECK_DONE1 ;Branch to Z_CHECK_DONE1 if 1.
    BSF	    PORTD,7 ; Turn on LED if result is 0
    
Z_CHECK_DONE1:
    RETURN
 

SUB_ADD:
    MOVF    PORTA, W ;store inpout from port A into WREG
    ANDLW   0x0F ;Mask upper 4 bits
    MOVWF   InA ; store WREG value into InA
    
    MOVF    PORTB, W ;store inpout from port B into WREG
    ANDLW   0X0F ;Mask upper 4 bits
    MOVWF   InB ; store WREG value into InB
    
    ADDWF   InA,W ;Add the two values together
    MOVWF   Result; store result into Result
    MOVFF   Result, PORTD; Output Result to Port D
    
    BCF	    PORTD,7 ; Clear bit 7 on port D
    BNZ	    Z_CHECK_DONE2 ;Branch to Z_CHECK_DONE2 if 1.
    BSF	    PORTD,7 ; Turn on LED if result is 0
Z_CHECK_DONE2:
    RETURN
    
SUB_IOR:
    MOVF    PORTA, W ;store inpout from port A into WREG
    ANDLW   0x0F ;Mask upper 4 bits
    MOVWF   InA ; store WREG value into InA
    
    MOVF    PORTB, W ;store inpout from port B into WREG
    ANDLW   0X0F ;Mask upper 4 bits
    MOVWF   InB ; store WREG value into InB
    
    IORWF   InA,W; Logical OR InA and WREG value
    MOVWF   Result; Store WREG value in Result
    MOVFF   Result, PORTD ;Output Result to Port D
    
    BCF	    PORTD,7 ; Clear bit 7 on port D
    BNZ	    Z_CHECK_DONE3 ;Branch to Z_CHECK_DONE1 if 1.
    BSF	    PORTD,7 ; Turn on LED if result is 0
    
Z_CHECK_DONE3:
    RETURN
    
SUB_AND:
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
    MOVF    Result, F; Store result into F
    
    BCF	    PORTD,7 ; Clear bit 7 on port D
    BNZ	    Z_CHECK_DONE4 ;Branch to Z_CHECK_DONE4 if 1.
    BSF	    PORTD,7 ; Turn on LED if result is 0
    
Z_CHECK_DONE4:
    RETURN
    
SUB_XOR:
    MOVF    PORTA, W ;store inpout from port A into WREG
    ANDLW   0x0F ;Mask upper 4 bits
    MOVWF   InA ; store WREG value into InA
     
    MOVF    PORTB, W ;store inpout from port B into WREG
    ANDLW   0X0F ;Mask upper 4 bits
    MOVWF   InB ; store WREG value into InB
    
    XORWF   InA,W; Logical XOR InA and WREG value
    MOVWF   Result; Store WREG value in Result
    MOVFF   Result, PORTD ;Output Result to Port D
    
    BCF	    PORTD,7 ; Clear bit 7 on port D
    BNZ	    Z_CHECK_DONE5 ;Branch to Z_CHECK_DONE5 if 1.
    BSF	    PORTD,7 ; Turn on LED if result is 0
    
Z_CHECK_DONE5:
    RETURN
    
SUB_BCD:
    MOVF    PORTA, W; Move PORTA into WREG
    ANDLW   0X0F ;Mask upper 4 bits
    MOVWF   InA; Move InA value into WREG;
    MOVLW   9; Load 9 into WREG
    
    CPFSGT  InA,1; Compare 9 and InA value
    GOTO    LESS; Branch if less than 9
    GOTO    GT; Branch if greater than 9
    
LESS
    MOVF    InA,W; Move InA into WREG
    GOTO    JUMP; move to JUMP

GT
    MOVLW   6; move 6 into WREG
    ADDWF   InA,W; Add 6 to InA
    
JUMP
    MOVWF   Result; Store WREG value in Result
    MOVFF   Result, PORTD ;Output Result to Port D
            
    BCF	    PORTD,7 ; Clear bit 7 on port D
    BNZ	    Z_CHECK_DONE6 ;Branch to Z_CHECK_DONE6 if 1.
    BSF	    PORTD,7 ; Turn on LED if result is 0
    
Z_CHECK_DONE6:
    RETURN
   ;
   ; Main loop consists of decoder to determine which operation is going to be done
   ;
MAIN_LOOP:
    BTFSC   PORTE,2 ; If bit 2 is 0 in PORTE then
    GOTO    PORTE_BIT2_EQ1 ; Bit 2 = 1
    GOTO    PORTE_BIT2_EQ0 ; Bit 2 = 0
    
PORTE_BIT2_EQ1:
    BTFSC   PORTE,1 ; If bit 1 is 0 in PORTE then
    GOTO    PORTE_BIT21_EQ11 ; Bit 1 = 1
    GOTO    PORTE_BIT21_EQ10 ; Bit 1 = 0
    
PORTE_BIT2_EQ0:
    BTFSC   PORTE,1 ; If bit 1 is 0 in PORTE then
    GOTO    PORTE_BIT21_EQ01 ; Bit 1 = 1
    GOTO    PORTE_BIT21_EQ00 ; Bit 1 = 0
    
PORTE_BIT21_EQ11:
    GOTO    PERFORM_TASK_BCD ; Port E value of 111 performs BCD
    
PORTE_BIT21_EQ10:
    GOTO    PERFORM_TASK_XOR ; Port E value of 10X performs XOR
    
PORTE_BIT21_EQ01:
    BTFSC   PORTE,0 
    GOTO    PERFORM_TASK_AND ; Port E value of 010 performs AND
    GOTO    PERFORM_TASK_IOR ; Port E value of 011 performs OR
    
PORTE_BIT21_EQ00:
    BTFSC   PORTE,0
    GOTO    PERFORM_TASK_ADD ; Port E value of 001 performs ADD
    GOTO    PERFORM_TASK_COMP ; Port E value of 000 performs COMP
    
PERFORM_TASK_COMP:
    BCF	    PORTC,2 ; This is to clear the Blue LED of the RGB
    BCF	    PORTC,1 ; This is to clear the Green LED of the RGB
    BCF	    PORTC,0 ; This is to clear the Red LED of the RGB 
    
    CALL    SUB_COMP
    GOTO    MAIN_LOOP
    
PERFORM_TASK_ADD:
    BCF	    PORTC,2 ; This is to clear the Blue LED of the RGB
    BCF	    PORTC,1 ; This is to clear the Green LED of the RGB
    BSF	    PORTC,0 ; This is to clear the Red LED of the RGB
    
    CALL    SUB_ADD
    GOTO    MAIN_LOOP
    
PERFORM_TASK_AND:
    BCF	    PORTC,2 ; This is to clear the Blue LED of the RGB
    BSF	    PORTC,1 ; This is to clear the Green LED of the RGB
    BSF	    PORTC,0 ; This is to clear the Red LED of the RGB
    
    CALL    SUB_IOR
    GOTO    MAIN_LOOP
    
PERFORM_TASK_IOR:
    BCF	    PORTC,2 ; This is to clear the Blue LED of the RGB
    BSF	    PORTC,1 ; This is to clear the Green LED of the RGB
    BCF	    PORTC,0 ; This is to clear the Red LED of the RGB
    
    CALL    SUB_AND
    GOTO    MAIN_LOOP
    
PERFORM_TASK_XOR:
    BSF	    PORTC,2 ; This is to clear the Blue LED of the RGB
    BCF	    PORTC,1 ; This is to clear the Green LED of the RGB
    BCF	    PORTC,0 ; This is to clear the Red LED of the RGB
    
    CALL    SUB_XOR
    GOTO    MAIN_LOOP
    
PERFORM_TASK_BCD:
    BSF	    PORTC,2 ; This is to clear the Blue LED of the RGB
    BSF	    PORTC,1 ; This is to clear the Green LED of the RGB
    BCF	    PORTC,0 ; This is to clear the Red LED of the RGB
    
    CALL    SUB_BCD
    GOTO    MAIN_LOOP
    
    END

