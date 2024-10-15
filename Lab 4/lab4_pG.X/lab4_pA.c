#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#include <usart.h>
#pragma config OSC = INTIO67
#pragma config BOREN = OFF
#pragma config WDT = OFF
#pragma config LVP = OFF
/*************************************************************/
/* Prototype Area */
/*************************************************************/ 

void main() 
{ 
char InA; 
char Result; 

 ADCON1 = 0x0f; 
 TRISA = 0xFF; // make sure PORT A is input 
 TRISB = 0xFF; // make sure PORT B is input 
 TRISC = 0x00; // make sure PORT C is output 
 TRISD = 0x00; // make sure PORT D is output 
 TRISE = 0xFF; // make sure PORT E is input 
 while (1) 
{ 
 InA = PORTA; // Read from PORT A 
 InA = InA & 0x0F; // Mask out upper 4 bits 
 Result = ~InA; // Take the 1?s complement for the lower 4 bits 
 Result = Result & 0x0F; // Mask out the upper 4 bits
 PORTD = Result; 
 } 
} 
