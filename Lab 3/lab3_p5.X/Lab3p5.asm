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
 MOVLW 0x00;
 MOVWF TRISD;
 MOVWF TRISE;
 
 ;ARRAY 1
 MOVLW 0X06 ;
 MOVWF 0X40
 
 MOVLW 0X01 ;
 MOVWF 0X41
 
 MOVLW 0X02 ;
 MOVWF 0X42
 
 MOVLW 0X03 ;
 MOVWF 0X43
 
 MOVLW 0X04 ;
 MOVWF 0X44
 
 MOVLW 0X00 ;
 MOVWF 0X45
 
 MOVLW 0X05 ;
 MOVWF 0X46
 
 MOVLW 0X07 ;
 MOVWF 0X47
 
 ; ARRAY 2
 MOVLW 0X02<<2 ;
 MOVWF 0X50
 
 MOVLW 0X03<<2 ;
 MOVWF 0X51
 
 MOVLW 0X04<<2 ;
 MOVWF 0X52
 
 MOVLW 0X05<<2 ;
 MOVWF 0X53
 
 MOVLW 0X00<<2 ;
 MOVWF 0X54
 
 MOVLW 0X06<<2 ;
 MOVWF 0X55
 
 MOVLW 0X07<<2 ;
 MOVWF 0X56
 
 MOVLW 0X01<<2 ;
 MOVWF 0X57
 
WHILE_LOOP: ;
    MOVLW 0x00
    MOVWF Color_Value
    MOVLW 0x08 ;
    MOVWF Loop_Count ;
    LFSR 0,0x0040 ;
    LFSR 1,0x0050 ;

FOR_LOOP:
    MOVF INDF0,W ;
    MOVWF PORTB ;
    MOVF INDF1,W ;
    MOVWF PORTD
    
    CALL DELAY_ONE_SEC ;
    
    INCF FSR0L ;
    INCF FSR1L ;
    
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