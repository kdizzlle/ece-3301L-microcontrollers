; THIS FIRST ASSEMBLY LANGUAGE PROGRAM WILL FLASH LEDS
; CONNECTED TO THE PINS 0 THROUGH 3 OF PORT C
#include<P18F4620.inc>
config OSC = INTIO67
config WDT = OFF
config LVP = OFF
config BOREN = OFF
; Constant declarations
Delay1 equ 0xF6;specific value to get 1 second delay
Delay2 equ 0XFF
Counter_L equ 0x20
Counter_H equ 0x21
ORG 0x0000
 ; CODE STARTS FROM THE NEXT LINE
 
START:
 MOVLW 0x0F ;
 MOVWF ADCON1 ;
 MOVLW 0x00 ;
 MOVWF TRISC ;
 MOVWF TRISE ;
 
MAIN_LOOP:
MOVLW 0x05 ; put 5 into working register 
MOVWF PORTC ; turn on LED
MOVWF PORTE ; output to Logic Analyzer
;
 CALL DELAY_ONE_SEC ; delay one second
MOVLW 0x0A ; put 10 into working register
MOVWF PORTC ; turn on LED
MOVWF PORTE ; output to Logic Analyzer
 CALL DELAY_ONE_SEC ; delay one second
 GOTO MAIN_LOOP ; Loop function to repeat blinking
 
; the routine below is a subroutine and it is called ?DELAY_ONE_SEC?.
 ; Values of Delay1 and 2 are essentially multiplied by using and inner and outer loop
 ;This lets us get a large delay despite our smalll number limit
DELAY_ONE_SEC:
MOVLW Delay1 ;
MOVWF Counter_H ;
LOOP_OUTER:
NOP ;
MOVLW Delay2 ;
MOVWF Counter_L ;
LOOP_INNER:
NOP ;
DECF Counter_L,F ;
BNZ LOOP_INNER ;
DECF Counter_H,F ;
BNZ LOOP_OUTER ;
RETURN
; end of subroutine ?DELAY_ONE_SEC?
 END


