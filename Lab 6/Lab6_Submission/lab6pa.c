#include <stdio.h> 
#include <stdlib.h> 
#include <xc.h> 
#include <math.h> 
#include <usart.h> 
#include <p18f4620.h> 
 
#pragma config OSC = INTIO67 
#pragma config WDT = OFF 
#pragma config LVP = OFF 
#pragma config BOREN = OFF 


// Prototype Area to place all the references to the routines used in the program 
void Init_ADC(void);
unsigned int get_full_ADC(void);
void Init_TRIS();
void select_ADC_CH(char);

    #define DP PORTDbits.RD7

float volt;
char LED_7seg[10] = { 0x01, 0x4F, 0x12, 0x06, 0x4C, 0x24, 0x20, 0x0F, 0x00, 0x0C };
float R;
unsigned int num_step;

//Following two functions initialize the usage of TeraTerm through a serial port
void init_UART() 
{ 
    OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF & 
    USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & 
    USART_BRGH_HIGH, 25); 
    OSCCON = 0x60; 
} 

void putch (char c) 
{ 
    while (!TRMT); 
    TXREG = c; 
}

//function to create 1 second delay
void wait_1_sec()
{
    for (int j=0; j<0xffff; j++);
}

//start of main function
void main(void) 
{ 
    init_UART();
    Init_ADC();
    Init_TRIS();
   
    select_ADC_CH(0);
    while (1)
    {
        num_step = get_full_ADC();//get number of steps
        
        volt = (num_step * 4)/1000.0;//store voltage value from input
        char U = (int) volt; //store voltage value
        char L = (int) ((volt-U) * 10); //create lower digit value
        
        
        wait_1_sec(); //wait 1 second to make display readable
        PORTB = LED_7seg[U];//Output upper voltage value
        PORTD = LED_7seg[L];//Output lower voltage value(1/10))
        DP = 0;//Turn on decimal point
        /*
        if((U%10) < 1)
            DP = 1;
        else
            DP = 0;
        */
        printf ("Voltage = %f \r\n", volt);//Print value to teraterm
    }
    
} 

//Initialize A/D converter
void select_ADC_CH (char AN_pin) 
{ 
    ADCON0 = AN_pin * 4 + 1; 
} 

void Init_ADC(void) 
{ 
    ADCON0=0x01; // select channel AN0, and turn on the ADC subsystem 
    ADCON1=0x1A ; // select pins AN0 through AN3 as analog signal, VDD-VSS as 
// reference voltage 
    ADCON2=0xA9; // right justify the result. Set the bit conversion time (TAD) and 
// acquisition time 
}

unsigned int get_full_ADC(void) //provides number of steps in conversion.
{ 
    int result;
    ADCON0bits.GO=1; // Start Conversion 
    while(ADCON0bits.DONE==1); // wait for conversion to be completed 
    result = (ADRESH * 0x100) + ADRESL; // combine result of upper byte and 
// lower byte into result 
    return result; // return the result. 
} //multiply result by voltage per step (4mV/step)

void Init_TRIS()
{
    TRISA =0xFF;        // make PORTA as all inputs
    TRISB =0x00;        // make PORTB as all outputs 
    TRISC = 0x00;       // make PORTC as all outputs
    TRISD = 0x00;       // make PORTD as all outputs
    TRISE = 0x00;       // make PORTE as all outputs
}

