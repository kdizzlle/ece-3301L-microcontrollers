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
void Set_ADCON0 (char);
void Init_ADC(void);
unsigned int get_full_ADC(void);
void Init_TRIS();
void SET_D2_RED();
void SET_D2_GREEN();
void SET_D2_YELLOW();
void SET_D2_BLUE();
void SET_D2_PURPLE();
void SET_D2_CYAN();
void SET_D2_WHITE();
void SET_D2_OFF();
void display_D1(int);
void display_D2(int);
void display_D3(int);

    #define D3_RED PORTCbits.RC0 
    #define D3_GREEN PORTCbits.RC1 
    #define D3_BLUE PORTCbits.RC2

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

void wait_1_sec()
{
    for (int j=0; j<0xffff; j++);
}

void main(void) 
{ 
    init_UART();
    Init_ADC();
    Init_TRIS();
    
    //int tempF;
    char LED_7seg[10] = { 0x01, 0x4F, 0x12, 0x06, 0x4C, 0x24, 0x20, 0x0F, 0x00, 0x0C };
    
    
    while (1)
    {
        Set_ADCON0(0);
        int num_step = get_full_ADC();
        
        float voltage = num_step * 4;
        voltage = 4*get_full_ADC();
        float tempC = (1035 - voltage) / 5.50;
        float temperatureF = (1.8 * tempC) + 32.0;
        
        int tempF = (int)temperatureF;
        char upper = tempF/10;
        char lower = tempF % 10;
        
        wait_1_sec();
        PORTB = LED_7seg[upper];
        PORTD = LED_7seg[lower];
        
        printf ("Voltage = %f (mV)\r\n", voltage);
        printf ("Temperature = %d F\r\n   ", tempF);
        
        display_D1(tempF);
        display_D2(tempF);
        
        Set_ADCON0(1);
        int num_step = get_full_ADC();
        int volt = num_step * 4;
        printf ("Light Volt = %d mV \r\n\n", volt);
        display_D3(volt);
    }
    
} 

void display_D1(int T)
{
    int range = T/10;
    if(range > 7)
        range = 7;
    PORTC = range<<3;
}

void display_D2(int T)
{
    if(T < 30) SET_D2_OFF();
    else if (T >= 30 && T < 39) SET_D2_RED;
    else if (T >= 40 && T < 49) SET_D2_GREEN;
    else if (T >= 50 && T < 59) SET_D2_YELLOW;
    else if (T >= 60 && T < 69) SET_D2_BLUE;
    else if (T >= 70 && T < 74) SET_D2_PURPLE;
    else if (T >= 75 && T < 79) SET_D2_CYAN;
    else SET_D2_WHITE;
}

void display_D3(int mv)
{
    if (mv < 2600) 
        PORTE = 0x01;
    else if (mv >= 2600 && mv < 2900)
        PORTE = 0x02;
    else if (mv >= 2900 && mv < 3200)
        PORTE = 0x03;
    else if (mv >= 3200 && mv < 3600)
        PORTE = 0x04;
    else
        PORTE = 0x05;
}

void Set_ADCON0 (char AN_pin) 
{ 
    ADCON0 = AN_pin * 4 + 1; 
} 

void Init_ADC(void) 
{ 
    ADCON0=0x01; // select channel AN0, and turn on the ADC subsystem 
    ADCON1=0x1B ; // select pins AN0 through AN3 as analog signal, VDD-VSS as 
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
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE = 0x00;
}

void SET_D2_RED() 
{ 
    D3_RED = 1; 
    D3_GREEN = 0; 
    D3_BLUE = 0; 
}

void SET_D2_GREEN() 
{ 
    D3_RED = 0; 
    D3_GREEN = 1; 
    D3_BLUE = 0; 
} 

void SET_D2_YELLOW() 
{ 
    D3_RED = 1; 
    D3_GREEN = 1; 
    D3_BLUE = 0; 
} 

void SET_D2_BLUE() 
{ 
    D3_RED = 0; 
    D3_GREEN = 0; 
    D3_BLUE = 1; 
} 

void SET_D2_PURPLE() 
{ 
    D3_RED = 1; 
    D3_GREEN = 0; 
    D3_BLUE = 1; 
} 

void SET_D2_CYAN() 
{ 
    D3_RED = 0; 
    D3_GREEN = 1; 
    D3_BLUE = 1; 
} 

void SET_D2_WHITE() 
{ 
    D3_RED = 1; 
    D3_GREEN = 1; 
    D3_BLUE = 1; 
} 

void SET_D2_OFF() 
{ 
    D3_RED = 0; 
    D3_GREEN = 0; 
    D3_BLUE = 0; 
} 