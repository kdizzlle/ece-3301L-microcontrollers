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
void check_buzzer(float);
void activate_buzzer();
void deactivate_buzzer();
void SET_D2_RED();
void SET_D2_GREEN();
void SET_D2_YELLOW();
void SET_D2_BLUE();
void SET_D2_PURPLE();
void SET_D2_CYAN();
void SET_D2_WHITE();
void SET_D2_OFF();
void display_D1(int);
void display_D2(float);

    #define D3_RED PORTEbits.RE0 
    #define D3_GREEN PORTEbits.RE1 
    #define D3_BLUE PORTEbits.RE2
    #define DP PORTDbits.RD7

float volt;
char LED_7seg[10] = { 0x01, 0x4F, 0x12, 0x06, 0x4C, 0x24, 0x20, 0x0F, 0x00, 0x0C };
float R;
unsigned int num_step;

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
   
    select_ADC_CH(2);
    while (1)
    {
        num_step = get_full_ADC();
        
        volt = (num_step * 4)/1000.0;
        R = (volt / (4.096 - volt)) * 10.0;
        
        
             
        
        wait_1_sec();
        if(R < 10)
        {
            char U = (int) R;
            char L = (int) ((R-U) * 10);
            
            PORTB = LED_7seg[U];
            PORTD = LED_7seg[L];
            DP = 0;     //0 is on
        }
            
        else
        {
            char U = (int)(R/10);
            char L = (int)R % 10;
            
            PORTB = LED_7seg[U];
            PORTD = LED_7seg[L];
            DP = 1;     //1 is off
        }
        
        
        
        printf ("Resistance = %f \r\n", R);
        check_buzzer(R);
        
        display_D1(R);
        display_D2(R);
    }
    
} 

void display_D1(int R)
{
    int range = R/10;
    if(range > 7)
        range = 7;
    PORTC = range<<2;
}

void display_D2(float T)
{
    if(T > 0.2) SET_D2_OFF();
    else if (T >= 0.15 && T <= 0.2) SET_D2_WHITE();
    else if (T >= 0.1 && T < 0.15) SET_D2_BLUE();
    else if (T >= 0.07 && T < 0.1) SET_D2_GREEN();
    else if (T < 0.07) SET_D2_RED();
    else SET_D2_PURPLE();
}

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

void check_buzzer(float r)
{
    if(r<0.07)
        activate_buzzer();
    else
        deactivate_buzzer();
}

void activate_buzzer()
{
    PR2 = 0b1111101;
    T2CON = 0b00000101;
    CCPR2L = 0b01001010;
    CCP2CON = 0b00111100;
}

void deactivate_buzzer()
{
    CCP2CON = 0x0;
    PORTCbits.RC1 = 0;
}
