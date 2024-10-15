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

/////////////////////////
// Code to get serial ports to work so we can use TeraTerm to debug
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
/////////////////////////

void wait_1_sec()
{
    for (int j=0; j<0xffff; j++); /// for loop with long counter to delay one second.
}

void main(void) 
{ 
    init_UART();
    Init_ADC();
    Init_TRIS();
    
    //int tempF;
    char LED_7seg[10] = { 0x01, 0x4F, 0x12, 0x06, 0x4C, 0x24, 0x20, 0x0F, 0x00, 0x0C }; //values that display 0-9 on 7 segmeent display
    
    
    while (1)
    {
        Set_ADCON0(0);//initialize analog to digital converter for temperature sensor
        int num_step = get_full_ADC();//take voltage and store it into num_step
        
        float voltage = num_step * 4;//convert num_step into a float value
        voltage = 4*get_full_ADC(); 
        float tempC = (1035 - voltage) / 5.50; //convert voltage into temperature in celcius
        float temperatureF = (1.8 * tempC) + 32.0;//convert celcius into farenheit. 
        
        int tempF = (int)temperatureF; //convert float temperature value into int
        char upper = tempF/10; //get display value for upper digit LCD
        char lower = tempF % 10;//get display value for lower digit LCD
        
        wait_1_sec();// delay a second to make temperature readable
        PORTB = LED_7seg[upper];//display value to upper segment
        PORTD = LED_7seg[lower];//display value to lower segment
        
        printf ("Voltage = %f (mV)\r\n", voltage);//print voltage vvalue to teraterm
        printf ("Temperature = %d F\r\n   ", tempF);//print temperature value to teraterm
        
        display_D1(tempF);//send temperature value to RGB 1
        display_D2(tempF);//send temperature value to RGB 2
        
        Set_ADCON0(1);//initialize analog to digital converter for photoresister
        int num_step = get_full_ADC(); //take analog input and store it into num_step
        int volt = num_step * 4; //convert num_step into proper volt value
        printf ("Light Volt = %d mV \r\n\n", volt);//display light volts into TeraTerm
        display_D3(volt);//send voltage value to RGB 3
    }
    
} 
//This code takes values that exceed 7 and turn them into 7 for use with out RGB LED to work properly.
void display_D1(int T)
{
    int range = T/10;
    if(range > 7)
        range = 7;
    PORTC = range<<3;//shift 3 bits
}

//This function assigns a color to temperature ranges 0-70+ Each color having a threshold of temperatures.
void display_D2(int T)
{
    if(T < 30) SET_D2_OFF(); //If tempurature is less than 30, turn LED off
    else if (T >= 30 && T < 39) SET_D2_RED; //If tempurature is between 30-39, turn LED Red
    else if (T >= 40 && T < 49) SET_D2_GREEN; //If tempurature is between 30-39, turn LED Green
    else if (T >= 50 && T < 59) SET_D2_YELLOW; //If tempurature is between 30-39, turn LED Yellow
    else if (T >= 60 && T < 69) SET_D2_BLUE; //If tempurature is between 30-39, turn LED Blue
    else if (T >= 70 && T < 74) SET_D2_PURPLE; //If tempurature is between 30-39, turn LED Purple
    else if (T >= 75 && T < 79) SET_D2_CYAN; //If tempurature is between 30-39, turn LED Cyan
    else SET_D2_WHITE; //If tempurature exceeds 79, set LED to White
}
//This function assigns a color to varying light mV  to RGB LED 3
void display_D3(int mv)
{
    if (mv < 2600) 
        PORTE = 0x01; //Set RGB to white if less than 2.6V
    else if (mv >= 2600 && mv < 2900)
        PORTE = 0x02; //Set RGB to Red if greater than 2.6V and less than 2.9V
    else if (mv >= 2900 && mv < 3200)
        PORTE = 0x03; //Set RGB to Blue if greater than 2.9V and less than 3.2V
    else if (mv >= 3200 && mv < 3600)
        PORTE = 0x04; //Set RGB to Green if greater than 3.2 and less than 3.6V
    else
        PORTE = 0x05; //Set RGB to Red if exceeds 3.6V
}
//Initialize the Analog to Digital converter.
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
    TRISC = 0x00;       // make PORTC as all outputs
    TRISD = 0x00;       // make PORTD as all outputs
    TRISE = 0x00;       // make PORTE as all outputs
}

void SET_D2_RED() //function to set RGB led as 001 (RED)
{ 
    D3_RED = 1; 
    D3_GREEN = 0; 
    D3_BLUE = 0; 
}

void SET_D2_GREEN() //function to set RGB led as 010 (GREEN)
{ 
    D3_RED = 0; 
    D3_GREEN = 1; 
    D3_BLUE = 0; 
} 

void SET_D2_YELLOW() //function to set RGB led as 011 (YELLOW)
{ 
    D3_RED = 1; 
    D3_GREEN = 1; 
    D3_BLUE = 0; 
} 

void SET_D2_BLUE() //function to set RGB led as 100 (BLUE)
{ 
    D3_RED = 0; 
    D3_GREEN = 0; 
    D3_BLUE = 1; 
} 

void SET_D2_PURPLE() //function to set RGB led as 101 (PURPLE)
{ 
    D3_RED = 1; 
    D3_GREEN = 0; 
    D3_BLUE = 1; 
} 

void SET_D2_CYAN() //function to set RGB led as 110 (CYAN)
{ 
    D3_RED = 0; 
    D3_GREEN = 1; 
    D3_BLUE = 1; 
} 

void SET_D2_WHITE() //function to set RGB led as 111 (WHITE)
{ 
    D3_RED = 1; 
    D3_GREEN = 1; 
    D3_BLUE = 1; 
} 

void SET_D2_OFF() //function to set RGB led as 000 (OFF)
{ 
    D3_RED = 0; 
    D3_GREEN = 0; 
    D3_BLUE = 0; 
} 