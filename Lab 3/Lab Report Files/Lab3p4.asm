


#include <P18F4620.inc>
 config OSC = INTIO67
 config WDT = OFF
 config LVP = OFF
 config BOREN = OFF
Delay1 equ 0xF6
Delay2 equ 0XFF
Counter_L equ 0x20
Counter_H equ 0x21
Color_Value equ 0x28
Loop_Count equ 0x29
 ORG 0x0000
; CODE STARTS FROM THE NEXT LINE
START:
 ORG 0x0000
START:
 MOVLW 0x0F ;
 MOVWF ADCON1 ;
 MOVLW 0x00 ;
 MOVWF TRISB ;

 ;Outer While loop equivalent that 
WHILE_LOOP: ;
 MOVLW 0x00 ; move 0 into working register
 MOVWF Color_Value ; move working register value into color value
 MOVLW 0x08 ; move 8 into the working register
 MOVWF Loop_Count ; sets loop count to 8

FOR_LOOP:
 MOVF Color_Value,W ; intake color_value and put it into working register
 MOVWF PORTB ; output to RGB LED
 CALL DELAY_ONE_SEC ; delay one second
 INCF Color_Value,F ; increase color value by 1
 DECF Loop_Count,F ; decrease color value by 1
 BNZ FOR_LOOP ; repate for_loop if loop_count isnt 0.
 ;
 GOTO WHILE_LOOP ; repeat entire process

; add the code for ?DELAY_ONE_SEC? here
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