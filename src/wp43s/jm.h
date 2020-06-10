/* This file is part of 43S.
 *
 * 43S is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * 43S is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 43S.  If not, see <http://www.gnu.org/licenses/>.
 */

//wp43s.h


/* ADDITIONAL C43 functions and routines */

/*
Modes available in the mode menu:

1. HOME.3    This switches on/off whether the HOME menu pops on/off within SH.3T timeout. This is a testing option, makes no sense in real life.
2. SH.4s     ShiftTimoutMode:  This switches off the 4 second shift time-out
3. SH.3T.    Home3TimerMode:   This switches off the 600 ms triple shift timer
4. ERPN      This disables the stack duplication and lift after entry

5. MYMENU
6. MYALPHA
7. HOME
8. ALPHAHOME
*/

// Radix Mark 1 bit
#define RX_PERIOD               103
#define RX_COMMA                104
// Time format 1 bit
#define TF_H24                  105
#define TF_H12                  106
// Complex unit 1 bit
#define CU_I                    107
#define CU_J                    108
// Product sign 1 bit
#define PS_DOT                  109
#define PS_CROSS                110
// Stack size 1 bit
#define SS_4                    111
#define SS_8                    112
// Complex mode 1 bit
#define CM_RECTANGULAR          113
#define CM_POLAR                114
// Display override 1 bit
#define DO_SCI                  115
#define DO_ENG                  116




//This is to allow the cursors to change the case. Normal on 43S. Off on C43
#define arrowCasechange    false




//This section must be part of both Layout1 and Layout2 and can be taken out of the main code here
#define JM_SHIFT_TIMER     4000  //ms TO_FG_TIMR
#define JM_TO_FG_LONG 	   580   //ms TO_FG_LONG

#define JM_FN_DOUBLE_TIMER 150   //ms TO_FN_EXEC
#define JM_TO_FN_LONG 	   400   //ms TO_FN_LONG  //  450 on 2020-03-13

#define JM_CLRDROP_TIMER   500  //ms TO_CL_DROP    //DROP
#define JM_TO_CL_LONG      800   //ms TO_CL_LONG   //CLSTK

#define JM_TO_3S_CTFF 	   600   //ms TO_3S_CTFF
#define JM_TO_KB_ACTV      6000  //ms TO_KB_ACTV

void reset_jm_defaults(void);

uint8_t softmenuStackPointer_MEM; //For popping on and off the HOME menu


//keyboard.c  screen.c
int16_t JM_auto_doublepress_enabled;              //JM TIMER CLRDROP //drop
int16_t JM_auto_longpress_enabled;                //JM TIMER CLRDROP //clstk

uint8_t JM_SHIFT_HOME_TIMER1;                     //Local to keyboard.c, but defined here
int16_t JM_ASN_MODE;                              //JM ASSIGN

bool_t ULFL, ULGL;                                //JM Underline

int16_t FN_key_pressed, FN_key_pressed_last;      //JM LONGPRESS FN
bool_t FN_timeouts_in_progress;                   //JM LONGPRESS FN
bool_t Shft_timeouts;                             //JM SHIFT NEW FN
bool_t FN_timed_out_to_NOP;                       //JM LONGPRESS FN
bool_t FN_timed_out_to_RELEASE_EXEC;              //JM LONGPRESS FN
bool_t FN_handle_timed_out_to_EXEC;


// Confirmation Y or N changed from original WP43S because the alpha keys order changed
#define ITEM_CONF_Y CHR_2 
#define ITEM_CONF_N ITM_CHS

// Define the second tagline
#define WHO2       "C43" STD_SPACE_3_PER_EM YEARMONTH STD_SPACE_3_PER_EM "HP42S" STD_SPACE_3_PER_EM "style" STD_SPACE_3_PER_EM "keys," STD_SPACE_3_PER_EM "by" STD_SPACE_3_PER_EM "Jaymos"   //JM ID

// Define variables that are saved with the config
extern uint8_t SigFigMode;                                           //JM SIGFIG 
extern bool_t eRPN;                                                  //JM eRPN Create a flag to enable or disable eRPN. See bufferize.c
extern bool_t HOME3;                                                 //JM HOME Create a flag to enable or disable triple shift HOME3.
extern bool_t ShiftTimoutMode;                                       //JM HOME Create a flag to enable or disable SHIFT TIMER CANCEL.
extern bool_t Home3TimerMode;                                        //JM HOME Create a flag to enable or disable TRIPLE SHIFT TIMER FOR HOME.
extern bool_t UNITDisplay;                                           //JM UNIT Create a flag to enable or disable unit display 
extern bool_t SH_BASE_HOME;                                          //JM BASEHOME Create a flag to enable or disable triple shift
extern bool_t SH_BASE_AHOME;                                         //JM BASEHOME Create a flag to enable or disable triple shift
extern int16_t Norm_Key_00_VAR;                                      //JM USER NORMAL
extern uint8_t Input_Default;                                        //JM Input Default
extern bool_t jm_FG_LINE;                                            //JM Screen / keyboard operation setup
extern bool_t jm_FG_DOTS;                                            //JM Screen / keyboard operation setup
extern bool_t jm_G_DOUBLETAP;                                        //JM Screen / keyboard operation setup
extern float graph_xmin;                                             //JM Graph
extern float graph_xmax;                                             //JM Graph
extern float graph_ymin;                                             //JM Graph
extern float graph_ymax;                                             //JM Graph
extern float graph_dx;                                               //JM Graph
extern float graph_dy;                                               //JM Graph
extern bool_t jm_VECT;                                               //JM GRAPH setup
extern bool_t jm_HOME_SUM;                                          //JMHOME
extern bool_t jm_HOME_MIR;                                          //JMHOME
extern bool_t jm_HOME_FIX;                                          //JMHOME



// Additional routines needed in jm.c
//void fnSeteRPN(uint16_t unusedParamButMandatory);         //dr - unused
//void fnSetHOME3(uint16_t unusedParamButMandatory);        //dr - unused
void fnSetSetJM(uint16_t jmConfig);
void fnInDefault(uint16_t inputDefault);
void fnSigmaAssign(uint16_t sigmaAssign);
void fnGetSigmaAssignToX(uint16_t unusedParamButMandatory);
void fnJMup(uint16_t unusedParamButMandatory);
void fnJMdown(uint16_t unusedParamButMandatory);
void fnJM_2SI(uint16_t unusedParamButMandatory);
//void fnInfo(bool_t Info);

void fnJM(uint16_t JM_OPCODE);
void fnUserJM(uint16_t jmUser);



//fnUserJM
#define USER_DEFAULTS    23
#define USER_COMPLEX     24
#define USER_SHIFTS      25
#define USER_RESET       26
#define JM_ASSIGN        27
#define JM_SEEK_FN       28
#define USER_ALPHA       33
#define USER_GSHFT       34
#define USER_CC          35
#define USER_MYM         36
#define USER_PRGM        37
#define USER_USER        38
#define USER_SIGMAPLUS   39
#define USER_V43         40
#define USER_SHIFTS2     41
#define USER_HOME        42
#define USER_V43LT       43
 //last number



void fnJM_GetXToNORMmode(uint16_t Rubbish);
void fnJMUSERmode   (uint16_t JM_KEY);
void fnJMUSERmode_f (uint16_t JM_KEY);
void fnJMUSERmode_g (uint16_t JM_KEY);
void fnStrtoX       (char aimBuffer[]);
void fnStrInputReal34 (char inp1[]);
void fnStrInputLongint(char inp1[]);
void fnRCL          (int16_t inp);
void fnKEYSELECT    (void);
void fnASSIGN       (int16_t JM_ASN_MODE, int16_t tempkey);
//void JM_convertReal34ToShortInteger(uint16_t confirmation);
//void JM_convertReal34ToLongInteger(uint16_t confirmation);
//void JM_convertIntegerToShortIntegerRegister(int16_t inp, uint32_t base, calcRegister_t destination);
//char* itoa(int value, char* result, int base);

#define TI_ABC                 96    //JM EE
#define TI_ABBCCA              97    //JM EE
#define TI_012                 98    //JM EE
#define TI_SHOW_REGISTER_BIG   99    //JM_SHOW
#define TI_ms                 100    //JMms

#define ID_43S                  0    //JM Input Default
#define ID_DP                   2    //JM Input Default
#define ID_CPXDP                4    //JM Input Default
#define ID_43D                  5    //JM Input Default
#define ID_SI                   6    //JM Input Default
#define ID_LI                   7    //JM Input Default

#define JC_ERPN                 1    // eRPN
#define JC_HOME_TRIPLE          2    // HOME.3
#define JC_SHFT_4s              3    // SH_4s
#define JC_BASE_HOME            4    // HOME
//#define JC_BASE_MYMENU          5    // MYMNU
#define JC_BASE_AHOME           6    // aHOME
//#define JC_BASE_MYA             7    // MYa
#define JC_SH_3T                8    // SH.3T
#define JC_BCR                  9    // CB ComplexResult
#define JC_BLZ                 10    // CB LeadingZeros
#define JC_PROPER              11    // CB FractionType
#define JC_IMPROPER            12    // CB FractionType
#define JC_BSR                 13    // CB SpecialResult
#define JM_INP_DFLT            15    // Input_Default

#define JC_FG_LINE             20    // screen setup
#define JC_FG_DOTS             21    // screen setup
#define JC_G_DOUBLETAP         22    // screen setup

#define JC_VECT                23    // screen setup

#define JC_H_SUM               24    //JMHOME
#define JC_H_MIR               25    //JMHOME
#define JC_H_FIX               26    //JMHOME


#ifdef PC_BUILD
//keyboard.c
void JM_DOT(int16_t xx, int16_t yy);
#endif

void fnBASE_Hash(uint16_t unusedParamButMandatory);
//void fnKeyDotDjm  (uint16_t unusedParamButMandatory);


//void fnComplexCCCC_CPX  (uint16_t unusedParamButMandatory);  //JM CPX NOT NEEDED
//void fnComplexCCCC_CC   (uint16_t unusedParamButMandatory);  //JM CPX NOT NEEDED 


void fnShowJM  (uint16_t jmConfig);
