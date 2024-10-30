//colors
#define OFF             0               // Defines OFF as decimal value 0
#define RED             1               // Defines RED as decimal value 1
#define GREEN           2               // Defines GREEN as decimal value 2
#define YELLOW          3               // Defines YELLOW as decimal value 3

#define Semester        1               // Type 0 for Sp, 1 for Fa
#define Year            24              // Type 2 digits year
#define Session_Number  3               // Type Session Number 1 through 5
#define Table_Number    13              // Type Table Number from 01 through 14
        

#define EW_RED      PORTCbits.RC2       // Defines EW_RED as PORT? bit R??
#define EW_GREEN    PORTCbits.RC4       // Defines EW_GREEN as PORT? bit R??

#define EWLT_RED    PORTEbits.RE1       // Defines EWLT_RED as PORT? bit R??
#define EWLT_GREEN  PORTEbits.RE2       // Defines EWLT_GREEN as PORTE bit R??

#define NS_RED      PORTAbits.RA5       // Defines NS_RED as PORTA bits R??
#define NS_GREEN    PORTCbits.RC0       // Defines NS_GREEN as PORTA bit R??

#define NSLT_RED    PORTDbits.RD0       // Defines NS_LT RED as PORT? bit R??
#define NSLT_GREEN  PORTDbits.RD1       // Defines NS_LT GREEN as PORT? bit R??


#define EW_LT_SW    PORTAbits.RA3       // Defines EW_PED as PORT? bit R?? for left turn
//#define EW_PED_SW   PORTAbits.RA1       // Defines NS_PED as PORT? bit R?? for Ped-switch
#define NS_LT_SW    PORTAbits.RA4       // Defines NS_LT as PORT? bit R?? for left turn
//#define NS_PED_SW   PORTAbits.RA2       // Defines EW_LT as PORT? bit R?? for Ped-switch

#define MODE_LED    PORTEbits.RE0       // Defines MODE_LED as PORTD bit R?? to differentiate day/night mode
#define SEC_LED     PORTDbits.RD7       // Defines SEC_LED as PORTD bit R??


#define PEDESTRIAN_NS_WAIT  7
#define NS_WAIT             5
#define EW_LT_WAIT          8
#define PEDESTRIAN_EW_WAIT  5
#define EW_WAIT             6
#define NS_LT_WAIT          5

#define NIGHT_NS_WAIT       7
#define NIGHT_EW_LT_WAIT    5
#define NIGHT_EW_WAIT       7
#define NIGHT_NS_LT_WAIT    5

