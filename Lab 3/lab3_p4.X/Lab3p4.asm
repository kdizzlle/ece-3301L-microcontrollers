


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

WHILE_LOOP: ;
 MOVLW 0x00 ;
 MOVWF Color_Value ;
 MOVLW 0x08 ;
 MOVWF Loop_Count ;

FOR_LOOP:
 MOVF Color_Value,W ;
 MOVWF PORTB ;
 CALL DELAY_ONE_SEC ;
 INCF Color_Value,F ;
 DECF Loop_Count,F ;
 BNZ FOR_LOOP ;
 ;
 GOTO WHILE_LOOP ;

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