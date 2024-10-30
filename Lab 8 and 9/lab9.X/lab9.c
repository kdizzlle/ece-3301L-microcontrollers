#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#include <string.h>
#include <usart.h> 

#include "main.h"
#include "utils.h"
#include "Main_Screen.h"
#include "ST7735_TFT.h"
#include "Interrupt.h"

void Set_NS(char color);
void Set_NS_LT(char color);
void Set_EW(char color);
void Set_EW_LT(char color);
void PED_Control( char Direction, char Num_Sec);
void Day_Mode();
void Night_Mode();
void Wait_N_Seconds (char);
void Do_Flashing();
//void Wait_One_Second_With_Beep();
//void Wait_One_Second();


#define _XTAL_FREQ  8000000             // Set operation for 8 Mhz
#define TMR_CLOCK   _XTAL_FREQ/4        // Timer Clock 2 Mhz

char dir;
char Count;                             // RAM variable for Second Count
char PED_Count;                         // RAM variable for Second Pedestrian Count

char  MODE;
char direction;
float volt;

char INT0_Flag;
char INT1_Flag;
char INT2_Flag;

char NS_PED_SW = 0;
char EW_PED_SW = 0;

extern char Light_Sensor;

char Flashing_Request = 0;
char Flashing_Status = 0;

void main(void)
{
    OSCCON = 0x70;                      // set the system clock to be 1MHz 1/4 of the 4MHz
    TRISA = 0x1F;                       //inputs RA0/AN0-RA4 inputs,RA5 output
    TRISB = 0x07;                       // outputs
    TRISC = 0x00;                       // set PORTC as outputs
    TRISD = 0x00;                       // set PORTD as outputs
    TRISE = 0x00;                       // set PORTE as outputs
    Init_ADC();
    Init_UART();

    Initialize_LCD_Screen();                        // Initialize the TFT screen
    
    RBPU = 0;
    init_INTERRUPT();

//     while (1)
//    { // Do nothing,
//        if (INT0_Flag == 1)
//        {
//            INT0_Flag = 0; // clear the flag
//            printf ("INT0 interrupt pin detected \r\n");
//            // print a message that INT0 has
//            // occurred
//        }
//         if (INT1_Flag == 1)
//        {
//            INT1_Flag = 0; // clear the flag
//            printf ("INT1 interrupt pin detected \r\n");
//            // print a message that INT1 has
//            // occurred
//        }
//         if (INT2_Flag == 1)
//        {
//            INT2_Flag = 0; // clear the flag
//            printf ("INT2 interrupt pin detected \r\n");
//            // print a message that INT2 has
//            // occurred
//         }
//    } 

//    while (1)
//    {
//        Rcmd2red();
//        delay_ms(2);
//        
//    }

    volt = Read_Volt(0);                        // 

    Light_Sensor = volt < 2.5 ? 1:0;                 // Mode = 1, Day_mode, Mode = 0 Night_mode

    while(1)                                    // forever loop
    {

        if (Light_Sensor == 1)    
        {
            MODE = 1;
            MODE_LED = 1;
            Day_Mode();                         // calls Day_Mode() function
        }
        else
        {
            MODE = 0;
            MODE_LED = 0;
            Night_Mode();                       // calls Night_Mode() function
        }
        
        if (Flashing_Request == 1)
        {
            Flashing_Request = 0;
            Flashing_Status = 1;
            Do_Flashing();
        }
    } 
}


void Set_NS(char color)
{
    // add code here
    direction = NS;
    update_LCD_color(direction, color);
    switch (color) 
    { 
       case OFF: NS_RED =0;NS_GREEN=0;break; // Turns off the NS LED 
       case RED: NS_RED =1;NS_GREEN=0;break; // Sets NS LED RED 
       case GREEN: NS_RED =0;NS_GREEN=1;break; // sets NS LED GREEN 
       case YELLOW: NS_RED =1;NS_GREEN=1;break; // sets NS LED YELLOW 
    } 
}

void Set_NS_LT(char color)
{
    // add code here
    direction = NSLT;
    update_LCD_color(direction, color);
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
    // add code here
    direction = EW;
    update_LCD_color(direction, color);
    switch (color) 
    { 
        case OFF: EW_RED =0;EW_GREEN=0;break; // Turns off the NS LED 
        case RED: EW_RED =1;EW_GREEN=0;break; // Sets NS LED RED 
        case GREEN: EW_RED =0;EW_GREEN=1;break; // sets NS LED GREEN 
        case YELLOW: EW_RED =1;EW_GREEN=1;break; // sets NS LED YELLOW 
    } 
}

void Set_EW_LT(char color)
{
    // add code here
    direction = EWLT;
    update_LCD_color(direction, color);
    switch (color) 
    { 
        case OFF: EWLT_RED =0;EWLT_GREEN=0;break; // Turns off the NS LED 
        case RED: EWLT_RED =1;EWLT_GREEN=0;break; // Sets NS LED RED 
        case GREEN: EWLT_RED =0;EWLT_GREEN=1;break; // sets NS LED GREEN 
        case YELLOW: EWLT_RED =1;EWLT_GREEN=1;break; // sets NS LED YELLOW 
    } 
}

void PED_Control( char direction, char num_sec)
{ 
    // add code here
    if (direction == 0)
        {
            //north-south direction
            //7seg lower digit off
            update_LCD_PED_Count(direction, 0);
            for (int i=num_sec-1; i>0; i--)
            {
                //7seg upper digit start with num_sec-1
//                PORTD = LED_7seg[i];
                update_LCD_PED_Count(direction, i);
                Wait_One_Second_With_Beep();
            }
            update_LCD_PED_Count(direction, 0);
            NS_PED_SW = 0;
            
        }
    else
        {
            //north-south direction
            //7seg lower digit off
            update_LCD_PED_Count(direction, 0);
            for (int j=num_sec-1; j>0; j--)
            {
                //7seg upper digit start with num_sec-1
//                PORTB = LED_7seg[j];
                update_LCD_PED_Count(direction, j);
                Wait_One_Second_With_Beep();
            }
            update_LCD_PED_Count(direction, 0);
            EW_PED_SW = 0;
        }
    
        Wait_One_Second_With_Beep();
        
}

void Day_Mode()
{
    // add code here
    
    Set_NS_LT(RED);  //step 1
    Set_EW(RED);
    Set_EW_LT(RED);
    Set_NS(GREEN);
    
    if(NS_PED_SW == 1)        //step 2
    {
        PED_Control(NS,PEDESTRIAN_NS_WAIT);       //step 3
    }
    Wait_N_Seconds(NS_WAIT);      //step 4
    Set_NS(YELLOW);       //step 5
    Wait_N_Seconds(3);
    Set_NS(RED);          //step 6

    if(EW_LT_SW == 1)            //step 7
    {
        Set_EW_LT(GREEN);        //step 8
        Wait_N_Seconds(EW_LT_WAIT);
        Set_EW_LT(YELLOW);        //step 9
        Wait_N_Seconds(3);
        Set_EW_LT(RED);          //step 10
    }
    Set_EW(GREEN);              //step 11
    if(EW_PED_SW == 1)        //step 12
    {
        PED_Control(EW,PEDESTRIAN_EW_WAIT);       //step 13
    }
        Wait_N_Seconds(EW_WAIT);      //step 14
        Set_EW(YELLOW);       //step 15
        Wait_N_Seconds(3);
        Set_EW(RED);          //step 16
    
    if(NS_LT_SW == 1)            //step 17
    {
        Set_NS_LT(GREEN);        //step 18
        Wait_N_Seconds(NS_LT_WAIT);
        Set_NS_LT(YELLOW);        //step 19
        Wait_N_Seconds(3);
        Set_NS_LT(RED);          //step 20
    }
    //step 21 - sequence complete
}

void Night_Mode()
{ 
    // add code here
    NS_PED_SW = 0;
    EW_PED_SW = 0;
    MODE_LED = 0;
    Set_NS_LT(RED);  //step 1
    Set_EW(RED);
    Set_EW_LT(RED);
    Set_NS(GREEN);
    Wait_N_Seconds(NIGHT_NS_WAIT);  //step 2
    Set_NS(YELLOW);     //step 3
    Wait_N_Seconds(3);
    Set_NS(RED);        //step 4
    if(EW_LT_SW == 1)        //step 5
    {
        Set_EW_LT(GREEN);        //step 6
        Wait_N_Seconds(NIGHT_EW_LT_WAIT);    
        Set_EW_LT(YELLOW);       //step 7
        Wait_N_Seconds(3);
        Set_EW_LT(RED);          //step 8
    }
    Set_EW(GREEN);              //step 9
    Wait_N_Seconds(NIGHT_EW_WAIT);
    Set_EW(YELLOW);             //step 10
    Wait_N_Seconds(3);
    Set_EW(RED);                //step 11
    if(NS_LT_SW == 1)            //step 12
    {
        Set_NS_LT(GREEN);        //step 13
        Wait_N_Seconds(NIGHT_NS_LT_WAIT);
        Set_NS_LT(YELLOW);        //step 14
        Wait_N_Seconds(3);
        Set_NS_LT(RED);          //step 15
    }
    //step 16 - sequence complete
}

void Wait_N_Seconds (char seconds)
{
    char I;
    for (I = seconds; I> 0; I--)
    {
		// add code here;
        Wait_One_Second();          			// calls Wait_One_Second for x number of times
        update_LCD_count(direction, I);
    }
    Wait_One_Second();
    update_LCD_count(direction, 0);
}
  
void Do_Flashing()
{
    while (Flashing_Status == 1)
    {
        if (Flashing_Request == 0)
        {
            Set_NS_LT(RED); 
            Set_EW(RED);
            Set_EW_LT(RED);
            Set_NS(RED);
            
            Wait_One_Second();
            
            Set_NS_LT(OFF); 
            Set_EW(OFF);
            Set_EW_LT(OFF);
            Set_NS(OFF);
            
            Wait_One_Second();
        }
        else
        {
            Flashing_Request = 0;
            Flashing_Status = 0;
        }
    }
}







