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

void Delay_One_Sec();

void main(void) 
{ 
    char in;
    TRISA =0xFF;        // make PORTA as all inputs
    TRISB =0x00;        // make PORTB as all outputs 
    TRISC =0x00;        // make PORTC as all outputs
    ADCON1 =0x0F;       // make the register digital
    
    while (1)
    {
        for (char i=0; i<8; i++)
        {
            PORTB = i;
            Delay_One_Sec();
        }
    }
} 


#define delay 17880
void Delay_One_Sec()
{
 for (int I=0; I <delay; I++);
}



