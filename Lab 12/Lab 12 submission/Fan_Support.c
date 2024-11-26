#include <p18f4620.h>
#include "main.h"
#include "Fan_Support.h"
#include "stdio.h"
#include "utils.h"

extern char FAN;
extern char duty_cycle;


int get_RPM()
{
    int RPS = TMR3L / 2;    // read the count. Since there are 2 pulses per rev
                            // then RPS = count /2
    TMR3L = 0;              // clear out the count
    return (RPS * 60);      // return RPM = 60 * RPS
}

void Toggle_Fan()
{
    if (FAN == 1)
        Turn_Off_Fan();
    else
        Turn_On_Fan();
}

void Turn_Off_Fan()
{
    FAN = 0;
    FAN_EN = 0;
    FANON_LED = 0; //fan on led
}

void Turn_On_Fan()
{
    FAN = 1;
    do_update_pwm(duty_cycle);
    FAN_EN = 1;
    FANON_LED = 1; //fan on led
}

void Increase_Speed()
{
    if (duty_cycle == 100)
    {
        Do_Beep(); //Beep to indicate maximum speed
        do_update_pwm(duty_cycle);//update PWM after beeping so the fan works correctly
    }
    else
    {
        duty_cycle = duty_cycle + 5; //increase duty cycle and this fan speed
        do_update_pwm(duty_cycle); //update with new duty cycle value 
    }
}

void Decrease_Speed()
{
    if (duty_cycle == 0)
    {
        Do_Beep(); //Beep if lowest possible value has been reached
        do_update_pwm(duty_cycle);//update pwm after beeping so fan keeps working
    }
    else
    {
        duty_cycle = duty_cycle - 5;//decrease duty cycle and thus fan speed
        do_update_pwm(duty_cycle);//update with new duty cycle value
    }
}

void do_update_pwm(char duty_cycle) 
{ 
    float dc_f;
    int dc_I;
    PR2 = 0b00000100; // set the frequency for 25 Khz
    T2CON = 0b00000111; //
    dc_f = (4.0 * duty_cycle / 20.0); // calculate factor of duty cycle versus a 25 Khz
    // signal
    dc_I = (int) dc_f; // get the integer part
    if (dc_I > duty_cycle) dc_I++; // round up function
    CCP1CON = ((dc_I & 0x03) << 4) | 0b00001100;
    CCPR1L = (dc_I) >> 2; 
}

void Set_DC_RGB(int duty_cycle)
{
    int range = duty_cycle/10;
    if (range > 7) //if duty cycle is less than 10
        range = 7; //set to 0
    PORTD = (PORTD & 0x8F) | (range<<4);// output to port D with some masking for bits 4-6
    
}

void Set_RPM_RGB(int rpm)
{
    int range = (rpm/500)+1;//divide by 500 to get range from 1-7, corresponding to RGB values
    if (range > 7)
        range = 7;//correct for values above the 7 thresholds
    if (rpm == 0)
        range = 0;// no color for 0 RPM
    PORTE = range;
}

void Set_TempC_RGB(signed char)
{
    
}
