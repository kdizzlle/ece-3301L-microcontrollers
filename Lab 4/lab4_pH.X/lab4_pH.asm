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
    
    GOTO    MAIN_LOOP
    
SUB_COMP:
    MOVF    PORTA, W
    ANDLW   0x0F
    MOVWF   InA
    
    COMF    InA, W
    ANDLW   0x0F
    MOVWF   Result
    MOVFF   Result, PORTD
    
    BCF	    PORTD,7
    BNZ	    Z_CHECK_DONE1
    BSF	    PORTD,7
    
Z_CHECK_DONE1:
    RETURN
 

SUB_ADD:
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
    RETURN
    
SUB_IOR:
    MOVF    PORTA, W
    ANDLW   0x0F
    MOVWF   InA
    
    MOVF    PORTB, W
    ANDLW   0X0F
    MOVWF   InB
    
    IORWF   InA,W
    MOVWF   Result
    MOVFF   Result, PORTD
    
    BCF	    PORTD,7
    BNZ	    Z_CHECK_DONE3
    BSF	    PORTD,7
    
Z_CHECK_DONE3:
    RETURN
    
SUB_AND:
    MOVF    PORTA, W
    ANDLW   0x0F
    MOVWF   InA
    
    MOVF    PORTB, W
    ANDLW   0X0F
    MOVWF   InB
    
    MOVF   InA,W
    ANDWF   InB, W
    MOVWF   Result
    MOVFF   Result, PORTD
    MOVF    Result, F
    
    BCF	    PORTD,7
    BNZ	    Z_CHECK_DONE4
    BSF	    PORTD,7
    
Z_CHECK_DONE4:
    RETURN
    
SUB_XOR:
    MOVF    PORTA, W
    ANDLW   0x0F
    MOVWF   InA
    
    MOVF    PORTB, W
    ANDLW   0X0F
    MOVWF   InB
    
    XORWF   InA,W
    MOVWF   Result
    MOVFF   Result, PORTD
    
    BCF	    PORTD,7
    BNZ	    Z_CHECK_DONE5
    BSF	    PORTD,7
    
Z_CHECK_DONE5:
    RETURN
    
SUB_BCD:
    MOVF    PORTA, W
    ANDLW   0x0F
    MOVWF   InA
    MOVLW   9
    
    CPFSGT  InA,1
    GOTO    LESS
    GOTO    GT
    
LESS
    MOVF    InA,W
    GOTO    JUMP

GT
    MOVLW   6
    ADDWF   InA,W
    
JUMP
    MOVWF   Result
    MOVFF   Result,PORTD
            
    BCF	    PORTD,7
    BNZ	    Z_CHECK_DONE6
    BSF	    PORTD,7
    
Z_CHECK_DONE6:
    RETURN
    
MAIN_LOOP:
    BTFSC   PORTE,2
    GOTO    PORTE_BIT2_EQ1
    GOTO    PORTE_BIT2_EQ0
    
PORTE_BIT2_EQ1:
    BTFSC   PORTE,1
    GOTO    PORTE_BIT21_EQ11
    GOTO    PORTE_BIT21_EQ10
    
PORTE_BIT2_EQ0:
    BTFSC   PORTE,1
    GOTO    PORTE_BIT21_EQ01
    GOTO    PORTE_BIT21_EQ00
    
PORTE_BIT21_EQ11:
    GOTO    PERFORM_TASK_BCD
    
PORTE_BIT21_EQ10:
    GOTO    PERFORM_TASK_XOR
    
PORTE_BIT21_EQ01:
    BTFSC   PORTE,0
    GOTO    PERFORM_TASK_AND
    GOTO    PERFORM_TASK_IOR
    
PORTE_BIT21_EQ00:
    BTFSC   PORTE,0
    GOTO    PERFORM_TASK_ADD
    GOTO    PERFORM_TASK_COMP
    
PERFORM_TASK_COMP:
    BCF	    PORTC,2
    BCF	    PORTC,1
    BCF	    PORTC,0
    
    CALL    SUB_COMP
    GOTO    MAIN_LOOP
    
PERFORM_TASK_ADD:
    BCF	    PORTC,2
    BCF	    PORTC,1
    BSF	    PORTC,0
    
    CALL    SUB_ADD
    GOTO    MAIN_LOOP
    
PERFORM_TASK_AND:
    BCF	    PORTC,2
    BSF	    PORTC,1
    BSF	    PORTC,0
    
    CALL    SUB_IOR
    GOTO    MAIN_LOOP
    
PERFORM_TASK_IOR:
    BCF	    PORTC,2
    BSF	    PORTC,1
    BCF	    PORTC,0
    
    CALL    SUB_AND
    GOTO    MAIN_LOOP
    
PERFORM_TASK_XOR:
    BSF	    PORTC,2
    BCF	    PORTC,1
    BCF	    PORTC,0
    
    CALL    SUB_XOR
    GOTO    MAIN_LOOP
    
PERFORM_TASK_BCD:
    BSF	    PORTC,2
    BSF	    PORTC,1
    BCF	    PORTC,0
    
    CALL    SUB_BCD
    GOTO    MAIN_LOOP
    
    END

