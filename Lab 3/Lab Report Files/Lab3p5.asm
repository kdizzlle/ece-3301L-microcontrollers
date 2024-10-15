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
 MOVLW 0X06 ; move 6 into working register
 MOVWF 0X40 ; store 6(Cyan) into memory location
 
 MOVLW 0X01 ; move 1 into working register
 MOVWF 0X41 ; store 1(Red) into memory location
 
 MOVLW 0X02 ; move 2 into working register
 MOVWF 0X42 ; store 2(Green) into memory location
 
 MOVLW 0X03 ; move 3 into working register
 MOVWF 0X43 ; store 3(Yellow) into memory location
 
 MOVLW 0X04 ; move 4 into working register
 MOVWF 0X44 ; store 4(Blue) into memory location
 
 MOVLW 0X00 ; move 0 into working register
 MOVWF 0X45 ; store 0(No light) into memory location
 
 MOVLW 0X05 ; move 5 into working register
 MOVWF 0X46 ; store 5(Purple) into memory location
 
 MOVLW 0X07 ; move 7 into working register
 MOVWF 0X47 ; store 7(White) into memory location
 
 ; ARRAY 2
 ;each value will be shifted by 2 bits due to wiring offset
 MOVLW 0X02<<2 ; move 2 into working register
 MOVWF 0X50    ; store 2(Green) into memory location
 
 MOVLW 0X03<<2 ; move 3 into working register
 MOVWF 0X51    ; store 3(Yellow) into memory location
 
 MOVLW 0X04<<2 ; move 4 into working register
 MOVWF 0X52    ; store 4(Blue) into memory location
 
 MOVLW 0X05<<2 ; move 5 into working register
 MOVWF 0X53    ; store 5(Purple) into memory location
 
 MOVLW 0X00<<2 ; move 0 into working register
 MOVWF 0X54    ; store 0(No light) into memory location
 
 MOVLW 0X06<<2 ; move 6 into working register
 MOVWF 0X55    ; store 6(Cyan) into memory location

 MOVLW 0X07<<2 ; move 7 into working register
 MOVWF 0X56    ; store 7(White) into memory location
 
 MOVLW 0X01<<2 ; move 1 into working register
 MOVWF 0X57    ; store 1(Red) into memory location
 
WHILE_LOOP: ;
    MOVLW 0x00
    MOVWF Color_Value ;uneeded code remnant from part 4
    MOVLW 0x08 ; move 8 into working register
    MOVWF Loop_Count ; store 8 from working register into loop_count
    LFSR 0,0x0040 ; pointer to start of array for RGB1
    LFSR 1,0x0050 ; pointer to start of array for RGB2

FOR_LOOP:
    MOVF INDF0,W ; move data from array1 to working register
    MOVWF PORTB ; output value from array to the RGB
    MOVF INDF1,W ; move data from array2 to working register
    MOVWF PORTD ; output value from array to the RGB
    
    CALL DELAY_ONE_SEC ; delay one second
    
    INCF FSR0L ; increment pointer to next array value
    INCF FSR1L ; increment pointer to next array value
    
    DECF Loop_Count,F ; Decrement loop_counter
    BNZ FOR_LOOP ; Repeat for_loop if loop_counter isnt 0
 ;
 GOTO WHILE_LOOP ;repeate entire function perpetually

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