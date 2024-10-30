//colors
#define OFF             0               // Defines OFF as decimal value 0
#define RED             1               // Defines RED as decimal value 1
#define GREEN           2               // Defines GREEN as decimal value 2
#define YELLOW          3               // Defines YELLOW as decimal value 3

#define Semester        0               // Type 0 for Sp, 1 for Fa
#define Year            24              // Type 2 digits year
#define Session_Number  1               // Type Session Number 1 through 5
#define Table_Number    01              // Type Table Number from 01 through 14
        

#define EW_RED      PORT?bits.R??       // Defines EW_RED as PORT? bit R??
#define EW_GREEN    PORT?bits.R??       // Defines EW_GREEN as PORT? bit R??

#define EWLT_RED    PORT?bits.R??       // Defines EWLT_RED as PORT? bit R??
#define EWLT_GREEN  PORT?bits.R??       // Defines EWLT_GREEN as PORTE bit R??

#define NS_RED      PORT?bits.R??       // Defines NS_RED as PORTA bits R??
#define NS_GREEN    PORT?bits.R??       // Defines NS_GREEN as PORTA bit R??

#define NSLT_RED    PORT?bits.R??       // Defines NS_LT RED as PORT? bit R??
#define NSLT_GREEN  PORT?bits.R??       // Defines NS_LT GREEN as PORT? bit R??


#define EW_LT_SW    PORT?bits.R??       // Defines EW_PED as PORT? bit R?? for left turn
#define EW_PED_SW   PORT?bits.R??       // Defines NS_PED as PORT? bit R?? for Ped-switch
#define NS_LT_SW    PORT?bits.R??       // Defines NS_LT as PORT? bit R?? for left turn
#define NS_PED_SW   PORT?bits.R??       // Defines EW_LT as PORT? bit R?? for Ped-switch

#define MODE_LED    PORT?bits.R??       // Defines MODE_LED as PORTD bit R?? to differentiate day/night mode
#define SEC_LED     PORT?bits.R??       // Defines SEC_LED as PORTD bit R??


#define PEDESTRIAN_NS_WAIT  ?
#define NS_WAIT             ?
#define EW_LT_WAIT          ?
#define PEDESTRIAN_EW_WAIT  ?
#define EW_WAIT             ?
#define NS_LT_WAIT          ?

#define NIGHT_NS_WAIT       ?
#define NIGHT_EW_LT_WAIT    ?
#define NIGHT_EW_WAIT       ?
#define NIGHT_NS_LT_WAIT    ?

