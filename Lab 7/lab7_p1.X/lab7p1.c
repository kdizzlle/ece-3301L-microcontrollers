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
void Wait_One_Second_With_Beep();

void Wait_One_Second();
void Wait_Half_Second();
void Wait_N_Seconds (char);
void Set_NS(char);
void Set_NSLT(char);
void Set_EW(char);
void Set_EWLT(char);
void PED_Control(char, char);
void Day_Mode();
void Night_Mode();
   
    #define MODE_LED PORTEbits.RE0
    #define EWPED_SW PORTAbits.RA1
    #define NSPED_SW PORTAbits.RA2
    #define EWLT_SW PORTAbits.RA3
    #define NSLT_SW PORTAbits.RA4
    #define SEC_LED PORTDbits.RD7 
    #define EW_RED PORTCbits.RC4
    #define EW_GREEN PORTCbits.RC5
    #define EWLT_RED PORTEbits.RE1
    #define EWLT_GREEN PORTEbits.RE2
    #define NS_RED PORTAbits.RA5
    #define NS_GREEN PORTCbits.RC0
    #define NSLT_RED PORTCbits.RC2
    #define NSLT_GREEN PORTCbits.RC3
    #define OFF 0
    #define RED 1
    #define GREEN 2
    #define YELLOW 3

char LED_7seg[10] = { 0x01, 0x4F, 0x12, 0x06, 0x4C, 0x24, 0x20, 0x0F, 0x00, 0x0C };

void init_UART() 
{ 
 OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF & 
USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & 
USART_BRGH_HIGH, 25); 
 OSCCON = 0x70; 
} 

void putch (char c) 
{ 
    while (!TRMT); 
    TXREG = c; 
}

void main(void) 
{ 
    init_UART();
    Init_ADC();
    Init_TRIS();
   
//    //for testing purposes
//    while(1)
//        Wait_One_Second();
//    
//    //for testing purposes
//    while(1)
//    {
//        for (int i=0; i<4; i++)
//        {
//        Set_EW(i);
//        Set_EWLT(i);
//        Set_NS(i);
//        Set_NSLT(i);
//        Wait_N_Seconds(1);
//        }
//    }
//      
//    //for testing purposes
//    while(1)
//    {
//        PED_Control(0,8);
//        PED_Control(1,6);
//    }
    
    //part 5 starts here
    while(1)
    {
        select_ADC_CH(0);
        float nStep = get_full_ADC();
        float volt = nStep * 5/1024.0;
        unsigned int MODE = volt < 2.5 ? 1:0;
        
        printf(" Voltage Photo Resistor = %f \r\n",volt);
        
        if (MODE)
        {
            MODE_LED = 1;
            Day_Mode();
        }
        else
        {
            MODE_LED = 0;
            Night_Mode();
        }
    }
} 

void select_ADC_CH (char AN_pin) 
{ 
    ADCON0 = AN_pin * 4 + 1; 
} 

void Init_ADC(void) 
{ 
    ADCON0=0x01; // select channel AN0, and turn on the ADC subsystem 
    ADCON1=0x0E ; // select pins AN0 through AN3 as analog signal, VDD-VSS as 
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
    TRISA =0x1F;        // make PORTA as input and output
    TRISB =0x00;        // make PORTB as all outputs 
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE = 0x00;
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

void Wait_One_Second() 
{ 
 SEC_LED = 1; // First, turn on the SEC LED 
 Wait_Half_Second(); // Wait for half second (or 500 msec) 
 SEC_LED = 0; // then turn off the SEC LED 
 Wait_Half_Second(); // Wait for half second (or 500 msec) 
} 
void Wait_Half_Second() 
{ 
 T0CON = 0x03; // Timer 0, 16-bit mode, prescaler 1:16 
 TMR0L = 0xDC; // set the lower byte of TMR 
 TMR0H = 0x0B; // set the upper byte of TMR 
 INTCONbits.TMR0IF = 0; // clear the Timer 0 flag 
 T0CONbits.TMR0ON = 1; // Turn on the Timer 0 
 while (INTCONbits.TMR0IF == 0); // wait for the Timer Flag to be 1 for done 
 T0CONbits.TMR0ON = 0; // turn off the Timer 0 
}

void Wait_N_Seconds (char seconds) 
{ 
char I; 
    for (I = 0; I< seconds; I++) 
    { 
        Wait_One_Second(); 
    } 
}

void Set_NS(char color) 
{ 
    switch (color) 
    { 
       case OFF: NS_RED =0;NS_GREEN=0;break; // Turns off the NS LED 
       case RED: NS_RED =1;NS_GREEN=0;break; // Sets NS LED RED 
       case GREEN: NS_RED =0;NS_GREEN=1;break; // sets NS LED GREEN 
       case YELLOW: NS_RED =1;NS_GREEN=1;break; // sets NS LED YELLOW 
    } 
} 

void Set_NSLT(char color) 
{ 
    switch (color) 
    { 
        case OFF: NSLT_RED =0;NSLT_GREEN=0;break; // Turns off the NS LED 
        case RED: NSLT_RED =1;NSLT_GREEN=0;break; // Sets NS LED RED 
        case GREEN: NSLT_RED =0;NSLT_GREEN=1;break; // sets NS LED GREEN 
        case YELLOW: NSLT_RED =1;NSLT_GREEN=1;break; // sets NS LED YELLOW 
    } 
} 

void Set_EW(char color) 
{ 
    switch (color) 
    { 
        case OFF: EW_RED =0;EW_GREEN=0;break; // Turns off the NS LED 
        case RED: EW_RED =1;EW_GREEN=0;break; // Sets NS LED RED 
        case GREEN: EW_RED =0;EW_GREEN=1;break; // sets NS LED GREEN 
        case YELLOW: EW_RED =1;EW_GREEN=1;break; // sets NS LED YELLOW 
    } 
} 

void Set_EWLT(char color) 
{ 
    switch (color) 
    { 
        case OFF: EWLT_RED =0;EWLT_GREEN=0;break; // Turns off the NS LED 
        case RED: EWLT_RED =1;EWLT_GREEN=0;break; // Sets NS LED RED 
        case GREEN: EWLT_RED =0;EWLT_GREEN=1;break; // sets NS LED GREEN 
        case YELLOW: EWLT_RED =1;EWLT_GREEN=1;break; // sets NS LED YELLOW 
    } 
} 

void PED_Control(char direction, char num_sec)
{
//    for(int i=0; i<num_sec-1; i++)
//    {
        if (direction == 0)
        {
            //north-south direction
            //7seg lower digit off
            PORTB = 0xFF;
            for (int i=num_sec-1; i>0; i--)
            {
                //7seg upper digit start with num_sec-1
                PORTD = LED_7seg[i];
                Wait_One_Second_With_Beep();
            }
            PORTD = 0xFF;
        }
        if (direction == 1)
        {
            //north-south direction
            //7seg lower digit off
            PORTD = 0xFF;
            for (int j=num_sec-1; j>0; j--)
            {
                //7seg upper digit start with num_sec-1
                PORTB = LED_7seg[j];
                Wait_One_Second_With_Beep();
            }
            PORTB = 0xFF;
        }
//    }
        Wait_One_Second_With_Beep();
}

void Wait_One_Second_With_Beep() 
{ 
    SEC_LED = 1; // First, turn on the SEC LED 
    activate_buzzer(); // Activate the buzzer
    Wait_Half_Second(); // Wait for half second (or 500 msec) 
    SEC_LED = 0; // then turn off the SEC LED 
    deactivate_buzzer(); // Deactivate the buzzer 
    Wait_Half_Second(); // Wait for half second (or 500 msec) 
} 

void Day_Mode()
{
    PORTB = 0xFF;
    PORTD = 0xFF;
    printf(" day mode step 1 \r\n");
    Set_NSLT(RED);  //step 1
    Set_EW(RED);
    Set_EWLT(RED);
    Set_NS(GREEN);
    
    printf(" step 2 \r\n");
    if(NSPED_SW == 1)        //step 2
    {
        printf(" step 3, NSPED_SW = %f \r\n",NSPED_SW);
        PED_Control(0,7);       //step 3
    }
    printf(" step 4 \r\n");
    Wait_N_Seconds(5);      //step 4
    printf(" step 5 \r\n");
    Set_NS(YELLOW);       //step 5
    Wait_N_Seconds(3);
    printf(" step 6 \r\n");
    Set_NS(RED);          //step 6

    printf(" step 7 \r\n");
    if(EWLT_SW == 1)            //step 7
    {
        printf(" step 8, EWLT_SW = %f \r\n",EWLT_SW);
        Set_EWLT(GREEN);        //step 8
        Wait_N_Seconds(8);
        printf(" step 9 \r\n");
        Set_EWLT(YELLOW);        //step 9
        Wait_N_Seconds(3);
        printf(" step 10 \r\n");
        Set_EWLT(RED);          //step 10
    }
    printf(" step 11 \r\n");
    Set_EW(GREEN);              //step 11
    printf(" step 12 \r\n");
    if(EWPED_SW == 1)        //step 12
    {
        printf(" step 13, EWPED_SW = %f \r\n",EWPED_SW);
        PED_Control(1,5);       //step 13
        printf(" step 14 \r\n");
    }
        Wait_N_Seconds(6);      //step 14
        printf(" step 15 \r\n");
        Set_EW(YELLOW);       //step 15
        Wait_N_Seconds(3);
        printf(" step 16 \r\n");
        Set_EW(RED);          //step 16
    
    printf(" step 17 NSLT_SW = %f \r\n", NSLT_SW);
    if(NSLT_SW == 1)            //step 17
    {
        printf(" step 18, NSLT_SW = %f \r\n",NSLT_SW);
        Set_NSLT(GREEN);        //step 18
        Wait_N_Seconds(5);
        printf(" step 19 \r\n");
        Set_NSLT(YELLOW);        //step 19
        Wait_N_Seconds(3);
        printf(" step 20 \r\n");
        Set_NSLT(RED);          //step 20
    }
    //step 21 - sequence complete
}

void Night_Mode()
{
    PORTB = 0xFF;
    PORTD = 0xFF;
    MODE_LED = 0;
    printf(" night mode step 1 \r\n");
    Set_NSLT(RED);  //step 1
    Set_EW(RED);
    Set_EWLT(RED);
    Set_NS(GREEN);
    printf(" step 2 \r\n");
    Wait_N_Seconds(7);  //step 2
    printf(" step 3 \r\n");
    Set_NS(YELLOW);     //step 3
    Wait_N_Seconds(3);
    printf(" step 4 \r\n");
    Set_NS(RED);        //step 4
    printf(" step 5 \r\n");
    if(EWLT_SW == 1)        //step 5
    {
        printf(" step 6, EWLT_SW = %f \r\n",EWLT_SW);
        Set_EWLT(GREEN);        //step 6
        Wait_N_Seconds(5);    
        printf(" step 7 \r\n");
        Set_EWLT(YELLOW);       //step 7
        Wait_N_Seconds(3);
        printf(" step 8 \r\n");
        Set_EWLT(RED);          //step 8
    }
    printf(" step 9 \r\n");
    Set_EW(GREEN);              //step 9
    Wait_N_Seconds(7);
    printf(" step 10 \r\n");
    Set_EW(YELLOW);             //step 10
    Wait_N_Seconds(3);
    printf(" step 11 \r\n");
    Set_EW(RED);                //step 11
    if(NSLT_SW == 1)            //step 12
    {
        printf(" step 13, NSLT_SW = %f \r\n",NSLT_SW);
        Set_NSLT(GREEN);        //step 13
        Wait_N_Seconds(5);
        printf(" step 14 \r\n");
        Set_NSLT(YELLOW);        //step 14
        Wait_N_Seconds(3);
        printf(" step 15 \r\n");
        Set_NSLT(RED);          //step 15
    }
    //step 16 - sequence complete
}