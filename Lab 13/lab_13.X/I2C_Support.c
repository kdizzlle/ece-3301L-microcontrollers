#include <stdio.h>

#include <p18f4620.h>
#include "I2C_Support.h"
#include "I2C.h"
#include "utils.h"

#define ACCESS_CFG      0xAC
#define START_CONV      0xEE
#define READ_TEMP       0xAA
#define CONT_CONV       0x02
#define ACK     1
#define NAK     0


extern unsigned char second, minute, hour, dow, day, month, year;
extern unsigned char setup_second, setup_minute, setup_hour, setup_day, setup_month, setup_year;

void DS1621_Init()
{
    // put original code       
    char Device = 0x48;
    I2C_Write_Cmd_Write_Data(Device, ACCESS_CFG, CONT_CONV);
    I2C_Write_Cmd_Only(Device, START_CONV);
}

int DS1621_Read_Temp()
{
    // put original code
    char Device = 0x48;
    char Cmd = READ_TEMP;
    char Data_Ret;    
    I2C_Start();                      // Start I2C protocol
    I2C_Write((Device << 1) | 0);     // Device address
    I2C_Write(Cmd);                   // Send register address
    I2C_ReStart();                    // Restart I2C
    I2C_Write((Device << 1) | 1);     // Initialize data read
    Data_Ret = I2C_Read(NAK);         //
    I2C_Stop(); 
    return Data_Ret;
}

void DS3231_Read_Time()
{
    // put original code                                         // End I2C protocol
    char Device = 0x68;
    char Address = 0x00;
    char Data_Ret;    
    I2C_Start();                      // Start I2C protocol
    I2C_Write((Device << 1) | 0);     // DS3231 address Write mode
    I2C_Write(Address);               // Send register address
    I2C_ReStart();                    // Restart I2C
    I2C_Write((Device << 1) | 1);     // Initialize data read
    second = I2C_Read(ACK);
    minute = I2C_Read(ACK);
    hour = I2C_Read(ACK);
    dow = I2C_Read(ACK);
    day = I2C_Read(ACK);
    month = I2C_Read(ACK);
    year = I2C_Read(NAK);
    I2C_Stop(); 
}

void DS3231_Write_Time()
{
    char Device = 0x68;                                         // Device ID
    char Address = 0x00;                                        // Beginning Address 0
    second = dec_2_bcd(setup_second);                           // 
    minute = dec_2_bcd(setup_minute);                           // 
    hour = dec_2_bcd(setup_hour);                               // 
    dow = 0x01;                                                 // 
    day = dec_2_bcd(setup_day);                                 // 
    month = dec_2_bcd(setup_month);                             // 
    year = dec_2_bcd(setup_year);                               // 
    I2C_Start();                                                // Start I2C protocol
    I2C_Write((Device << 1) | 0);                               // Device address Write mode
    I2C_Write(Address);                                         // Send register address
    I2C_Write(second);                                          // Write seconds
    I2C_Write(minute);                                          // Write minutes
    I2C_Write(hour);                                            // Write hours
    I2C_Write(dow);                                             // Write DOW
    I2C_Write(day);                                             // Write day
    I2C_Write(month);                                           // Write month
    I2C_Write(year);                                            // Write year
    I2C_Stop();                                                 // End I2C protocol
}

void DS3231_Init()
{
char Device = 0x68;
char Address_7 = 0x07;
char Address_E = 0x0E;  
char control_E;

    control_E = I2C_Write_Address_Read_One_Byte(Device, Address_E);
    control_E = control_E & 0x01;
    control_E = control_E | 0x25; 
    I2C_Write_Address_Write_One_Byte(Device, Address_E, control_E);
    I2C_Start();                                // Start I2C protocol   
    I2C_Write((Device << 1) | 0);               // Device address
    I2C_Write(Address_7);                       // Send register address
    I2C_ReStart();                              // Restart I2C
    I2C_Write((Device << 1) | 1);               // Initialize data read
    
    I2C_Start();                                 // Start I2C protocol
    I2C_Write((Device << 1) | 0);                // Device address Write mode
    I2C_Write(Address_7);                        // Send register address 7
  
    I2C_Stop();   
}


