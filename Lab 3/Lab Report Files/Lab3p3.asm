; THIS SECOND ASSEMBLY LANGUAGE PROGRAM WILL READ THE VALUES OF
; ALL THE BITS 0-2 OF PORT A AND OUTPUT THEM
; TO THE PINS 0 THROUGH 2 OF PORT B
#include <P18F4620.inc>
 config OSC = INTIO67
 config WDT = OFF
 config LVP = OFF
 config BOREN = OFF
 ORG 0x0000
 ; Very similar code to part 2, we're just taking the user input to the RGB LED
START:
 MOVLW 0x0F ;
 MOVWF ADCON1 ;
 MOVLW 0xFF ;
 MOVWF TRISA ; Store FF in A register
 MOVLW 0x00 ;
 MOVWF TRISB ; Store 0 in C register
MAIN_LOOP: ;
 MOVF PORTA, W ; ;Reads user input
 ANDLW 0x0F ; store input in working register
 MOVWF PORTB ; output value to port B(RGB)
 GOTO MAIN_LOOP ; Loop for continuous input
 END


