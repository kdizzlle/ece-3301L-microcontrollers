#include <stdio.h> 
#include <stdlib.h> 
#include <xc.h> 
#include <math.h> 
#include <p18f4620.h> 
 
#pragma config OSC = INTIO67 
#pragma config WDT = OFF 
#pragma config LVP = OFF 
#pragma config BOREN = OFF 

// Prototype Area to place all the references to the routines used in the program 

void main(void) 
{ 
    char in;
    TRISA =0xFF;        // make PORTA as all inputs
    TRISB =0x00;        // make PORTB as all outputs 
    TRISC = 0x00;
    TRISD = 0x00;
    ADCON1 =0x0F;       // make the register digital
    
    while (1)
    {
        in = PORTA;     // read data from PORTA and save it
                        // into ?in?
        in = in & 0x0F; // Mask out the unused upper four bits
                        // while preserving the lower 4-bits
        PORTC = in;     // Output the data to PORTB 
    }
} 


