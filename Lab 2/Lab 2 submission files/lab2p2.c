#include <stdio.h> 
#include <stdlib.h> 
#include <xc.h> 
#include <math.h> 
#include <p18f4620.h> 
 
#pragma config OSC = INTIO67 
#pragma config WDT = OFF 
#pragma config LVP = OFF 
#pragma config BOREN = OFF 

/*************************************************************/
/* Prototype Area                                            */
/*************************************************************/

void main(void) 
{ 
    /*************************************************************/
   /*    Establish Port outputs                                  */
   /*************************************************************/
    
    char in;
    TRISA =0xFF;        // make PORTA as all inputs
    TRISB =0x00;        // make PORTB as all outputs 
    TRISC =0x00;        // make PORTC as all outputs
    ADCON1 =0x0F;       // make the register digital
    
    
    /*************************************************************/
   /*    Infinite While loop that reads user input               */
   /*************************************************************/
    
    while (1)
    {
        in = PORTA;     // read data from PORTA and save it
                        // into ?in?
        in = in & 0x07; // Mask out the unused upper four bits
                        // while preserving the lower 4-bits
        PORTB = in;     // Output the data to PORTB(RGB LED))
    }
    
} 