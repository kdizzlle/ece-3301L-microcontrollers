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

void Delay_One_Sec();//instantiate delay function

char array1[8]={0x06, 0x01, 0x02, 0x03, 0x04, 0x00, 0x05, 0x07};//list color values for RBG1 in specific order
char array2[8]={0x02, 0x03, 0x04, 0x05, 0x00, 0x06, 0x07, 0x01};//list color values for RBG2 in specific order

void main(void) 
{ 
    char in;
    TRISA =0xFF;        // make PORTA as all inputs
    TRISB =0x00;        // make PORTB as all outputs 
    TRISC =0x00;        // make PORTC as all outputs
    TRISD = 0x00;       // make PORTD as all outputs
    ADCON1 =0x0F;       // make the register digital
    
    /*************************************************************/
   /*    Infinite While loop                                     */
   /*************************************************************/
    
    while (1)
    {
        for (char i=0; i<8; i++)//for loop that loops through all assigned values for each color of RGB LED
        {
            PORTB = array1[i];//send incrementing value to PORTB
            PORTD = array2[i]<<2;//send incrementing value to PORTD by 2 shifted bits due to not starting at D0
            Delay_One_Sec();//delay a second to make change in light visible
        }
        
    }
    
} 

/*************************************************************/
/*   For loop that delays by 1 second                        */
/*************************************************************/

#define delay 17880//define delay value tunedd to be 1 second
void Delay_One_Sec()
{
 for (int I=0; I <delay; I++);//computer increments time until delay number is met
}



