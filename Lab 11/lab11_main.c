#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#include <usart.h>
#include <string.h>

#include "ST7735_TFT.h"
#include "utils.h"
#include "Main_Screen.h"
#include "ST7735_TFT.h"
#include "Interrupt.h"
#include "main.h"
#include "I2C.h"
#include "I2C_Support.h"

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF
//#pragma config CCP2MX = PORTBE
char tempSecond = 0xff;
char second = 0x00;
char minute = 0x00;
char hour = 0x00;
char dow = 0x00;
char day = 0x00;
char month = 0x00;
char year = 0x00;
char setup_second, setup_minute, setup_hour, setup_day, setup_month, setup_year;
char alarm_second, alarm_minute, alarm_hour, alarm_date;
char setup_alarm_second, setup_alarm_minute, setup_alarm_hour; 

#define _XTAL_FREQ  8000000             // Set operation for 8 Mhz


short Nec_OK = 0;
char Nec_Button;
extern unsigned long long Nec_code;



char array1[21]={0xa2, 0x62, 0xe2, 0x22, 0x02, 0xc2, 0xe0, 0xa8, 0x90, 0x68, 0x98, 0xb0, 0x30, 0x18, 0x7a, 0x10, 0x38, 0x5a, 0x42, 0x4a, 0x52};
char txt1[21][4] ={"CH-\0", "CH\0","CH+\0","BWD\0","FWD\0","PLY\0","VL-\0","VL+\0","EQ \0"," 0 \0","100\0","200\0"," 1 \0"," 2 \0"," 3 \0"," 4 \0"," 5 \0"," 6 \0"," 7 \0"," 8 \0"," 9 \0"};
int color[21]={RD, RD, RD, BU, BU, GR, BU, BU, MA, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK};



void main()
{
    Init_UART();
    OSCCON = 0x70;                          // 8 Mhz
    nRBPU = 0;                              // Enable PORTB internal pull up resistor
    TRISB = 0x04;
    TRISC = 0x00;                           // PORTC as output
    TRISD = 0x00;
    ADCON1 = 0x0F;                          //
    TRISA = 0x00;
    TRISE = 0x00;
 
    Initialize_LCD_Screen();
    I2C_Init(100000);
    DS1621_Init();
    Init_Interrupt();

    Nec_code = 0x0;                         // Clear code
    char found;
    
    while(1)
    {
        Deactivate_Buzzer();
//        printf("testing... \r\n");
        char tempC = DS1621_Read_Temp();
        char tempF = (tempC * 9/5) + 32;
        printf("Temperature = %d degreesC = %d degreesF\r\n", tempC, tempF);
        Wait_One_Sec_Soft();
        
        if (Nec_OK == 1)
        {
            Nec_OK = 0;
            Enable_INT_Interrupt();
            printf ("NEC_Button = %x \r\n", Nec_Button);  

            found = 0xff;
            
            // add code here to look for code using a FOR loop
            for (char i=0; i<21; i++)
            {
                if (Nec_Button == array1[i])
                {
                        found = i;
//                        printf ("array1[i] = %x \r\n", array1[i]); 
                }
            }
            
            if (found != 0xff) 
            {
				printf ("Key Location = %d \r\n\n", found);      
                fillCircle(Circle_X, Circle_Y, Circle_Size, color[found]); 
                drawCircle(Circle_X, Circle_Y, Circle_Size, ST7735_WHITE);  
                drawtext(Text_X, Text_Y, txt1[found], ST7735_WHITE, ST7735_BLACK,TS_1);
   
			// add code to output color for the RGB LEDS
                if(color[found] == RD)
                {
                    //LED D1 to red
                    //reset D1 to red
                    PORTDbits.RD1 = 1;
                    PORTDbits.RD2 = 0;
                    PORTDbits.RD3 = 0;
                    //reset D2 to off
                    PORTEbits.RE0 = 0;
                    PORTEbits.RE1 = 0;
                    PORTEbits.RE2 = 0;
                    //reset D3 to off
                    PORTDbits.RD4 = 0;
                    PORTDbits.RD5 = 0;
                    PORTDbits.RD6 = 0;
                }
                if(color[found] == BU)
                {
                    if(found==3 || found==4)
                    {
                        //LED D1 to blue
                        //reset D1 to blue
                        PORTDbits.RD1 = 0;
                        PORTDbits.RD2 = 0;
                        PORTDbits.RD3 = 1;
                        //reset D2 to off
                        PORTEbits.RE0 = 0;
                        PORTEbits.RE1 = 0;
                        PORTEbits.RE2 = 0;
                        //reset D3 to off
                        PORTDbits.RD4 = 0;
                        PORTDbits.RD5 = 0;
                        PORTDbits.RD6 = 0;
                    }
                    else
                    {
                        //LED D2 to blue
                    //reset D1 to off
                    PORTDbits.RD1 = 0;
                    PORTDbits.RD2 = 0;
                    PORTDbits.RD3 = 0;
                    //reset D2 to blue
                    PORTEbits.RE0 = 0;
                    PORTEbits.RE1 = 0;
                    PORTEbits.RE2 = 1;
                    //reset D3 to off
                    PORTDbits.RD4 = 0;
                    PORTDbits.RD5 = 0;
                    PORTDbits.RD6 = 0;
                    }
                }
                if(color[found] == GR)
                {
                    //LED D1 to green
                    //reset D1 to green
                    PORTDbits.RD1 = 0;
                    PORTDbits.RD2 = 1;
                    PORTDbits.RD3 = 0;
                    //reset D2 to off
                    PORTEbits.RE0 = 0;
                    PORTEbits.RE1 = 0;
                    PORTEbits.RE2 = 0;
                    //reset D3 to off
                    PORTDbits.RD4 = 0;
                    PORTDbits.RD5 = 0;
                    PORTDbits.RD6 = 0;
                }
                if(color[found] == MA)
                {
                    //LED D2 to magenta
                    //reset D1 to off
                    PORTDbits.RD1 = 0;
                    PORTDbits.RD2 = 0;
                    PORTDbits.RD3 = 0;
                    //reset D2 to magenta
                    PORTEbits.RE0 = 1;
                    PORTEbits.RE1 = 0;
                    PORTEbits.RE2 = 1;
                    //reset D3 to off
                    PORTDbits.RD4 = 0;
                    PORTDbits.RD5 = 0;
                    PORTDbits.RD6 = 0;
                }
                if(color[found] == BK)
                {
                    if (found==9 || found==10 || found==11)
                    {
                        //LED D2 to white
                        //reset D1 to off
                        PORTDbits.RD1 = 0;
                        PORTDbits.RD2 = 0;
                        PORTDbits.RD3 = 0;
                        //reset D2 to white
                        PORTEbits.RE0 = 1;
                        PORTEbits.RE1 = 1;
                        PORTEbits.RE2 = 1;
                        //reset D3 to off
                        PORTDbits.RD4 = 0;
                        PORTDbits.RD5 = 0;
                        PORTDbits.RD6 = 0;
                    }
                    else
                    {
                        //LED D2 to white
                        //reset D1 to off
                        PORTDbits.RD1 = 0;
                        PORTDbits.RD2 = 0;
                        PORTDbits.RD3 = 0;
                        //reset D2 to off
                        PORTEbits.RE0 = 0;
                        PORTEbits.RE1 = 0;
                        PORTEbits.RE2 = 0;
                        //reset D3 to white
                        PORTDbits.RD4 = 1;
                        PORTDbits.RD5 = 1;
                        PORTDbits.RD6 = 1;
                    }
                }
			
			// add code to handle the KEY_PRESSED LED and do the buzzer sound
                KEY_PRESSED = 1;
                Activate_Buzzer();
                Wait_One_Sec();
                Deactivate_Buzzer();
                KEY_PRESSED = 0;
            }
        }
    }
}


