/* This file is part of WP43.
 *
 * WP43 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WP43 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WP43.  If not, see <http://www.gnu.org/licenses/>.
 */

//wp43.h


/* ADDITIONAL C43 functions and routines */

/*
Modes available in the mode menu:

1. HOME.3    This switches on/off whether the HOME menu pops on/off within SH.3T timeout. This is a testing option, makes no sense in real life.
2. SH.4s     ShiftTimoutMode:  This switches off the 4 second shift time-out
3. HOME.3    This switches off the 600 ms triple shift timer
4. ERPN      This disables the stack duplication and lift after entry

5. MYMENU
6. MYALPHA
7. HOME
8. ALPHAHOME
*/

#ifndef JM_H
#define JM_H

#include "longIntegerType.h"
#include "typeDefinitions.h"
#include <stdint.h>

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

#define FN_BEG                  117
#define FN_END                  118



//BCD options
#define BCDu                    0
#define BCD9c                   1
#define BCD10c                  2
//Longpress Options
#define RB_F14                  0
#define RB_F124                 1
#define RB_F1234                2
#define RB_M14                  10
#define RB_M1234                11


void jm_show_calc_state(char comment[]);
void jm_show_comment   (char comment[]);


//keyboard.c  screen.c


// Confirmation Y or N changed from original WP43 because the alpha keys order changed
#define ITEM_CONF_Y ITM_2 
#define ITEM_CONF_N ITM_CHS


// Define variables that are saved with the config
extern bool_t eRPN;                                                  //JM eRPN Create a flag to enable or disable eRPN. See bufferize.c
extern bool_t HOME3;                                                 //JM HOME Create a flag to enable or disable triple shift HOME3; enable or disable TRIPLE SHIFT TIMER FOR HOME.
extern bool_t ShiftTimoutMode;                                       //JM HOME Create a flag to enable or disable SHIFT TIMER CANCEL.
extern bool_t SH_BASE_HOME;                                          //JM BASEHOME Create a flag to enable or disable triple shift
extern int16_t Norm_Key_00_VAR;                                      //JM USER NORMAL
extern uint8_t Input_Default;                                        //JM Input Default
extern bool_t jm_FG_LINE;                                            //JM Screen / keyboard operation setup
extern bool_t jm_BASE_SCREEN;                                        //JM Screen / keyboard operation setup
extern bool_t jm_G_DOUBLETAP;                                        //JM Screen / keyboard operation setup
extern float graph_xmin;                                             //JM Graph
extern float graph_xmax;                                             //JM Graph
extern float graph_ymin;                                             //JM Graph
extern float graph_ymax;                                             //JM Graph
extern bool_t jm_HOME_SUM;                                          //JMHOME
extern bool_t jm_LARGELI;                                           //JM flag to keep large font numbers on screen
extern bool_t constantFractions;                                    //JM
extern uint8_t constantFractionsMode;                                //JM
extern bool_t constantFractionsOn;                                   //JM
extern uint32_t indic_x;                                            //JM program progress indicators
extern uint32_t indic_y;                                            //JM program progress indicators


// Additional routines needed in jm.c
void fnSigmaAssign(uint16_t sigmaAssign);
void fnGetSigmaAssignToX(uint16_t unusedButMandatoryParameter);

//void fnInfo(bool_t Info);

void fnJM(uint16_t JM_OPCODE);
void fnUserJM(uint16_t jmUser);



//fnUserJM
#define USER_C43ALTA     25
#define USER_C43ALTB     225
#define USER_C43ALT      26
#define JM_ASSIGN        28
#define USER_COPY        29
#define USER_V47         40
#define USER_SHIFTS2     41
#define USER_E47         43
#define USER_43S         44
#define USER_DM42        45
#define USER_C47         46
#define USER_D47         47
#define USER_ARESET      48
#define USER_MRESET      49
#define USER_KRESET      50
#define USER_N47         51
#define USER_C43         52
 //last number



void fnJM_GetXToNORMmode(uint16_t Rubbish);
void fnStrtoX       (char aimBuffer[]);
void fnStrInputReal34 (char inp1[]);
void fnStrInputLongint(char inp1[]);
void fnRCL          (int16_t inp);

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
#define JC_BCR                  9    // CB ComplexResult
#define JC_BLZ                 10    // CB LeadingZeros
#define JC_PROPER              11    // CB FractionType
#define JC_IMPROPER            12    // CB FractionType
#define JC_BSR                 13    // CB SpecialResult
#define DM_ANY                 16    // DENANY
#define DM_FIX                 17    // DENFIX
#define JC_FRC                 18    // CB FRACTION MODE
#define PR_HPRP                19    // POLAR RECT CLASSIC MODE

#define JC_FG_LINE             20    // screen setup
#define JC_BASE_SCREEN         21    // screen setup
#define JC_G_DOUBLETAP         22    // screen setup

#define JC_H_SUM               26    //JMHOME

#define JC_LARGELI             29

#define JC_ITM_TST             31    //dr

#define JC_VECT                39    // graph setup
#define JC_NVECT               40    // graph setup
#define JC_SCALE               41    // graph setup
#define JC_EXTENTX             42    // graph setup
#define JC_EXTENTY             43    // graph setup
#define JC_PLINE               44    // graph setup
#define JC_PCROS               45    // graph setup
#define JC_PBOX                46    // graph setup
#define JC_INTG                47    // graph setup
#define JC_DIFF                48    // graph setup
#define JC_RMS                 49    // graph setup
#define JC_SHADE               50    // graph setup
#define JC_PZOOMX              51
#define JC_PZOOMY              52

#define JC_NL                  53
#define JC_UC                  54

#define JC_LINEAR_FITTING      55
#define JC_EXPONENTIAL_FITTING 56
#define JC_LOGARITHMIC_FITTING 57
#define JC_POWER_FITTING       58
#define JC_ROOT_FITTING        59
#define JC_HYPERBOLIC_FITTING  60
#define JC_PARABOLIC_FITTING   61
#define JC_CAUCHY_FITTING      62
#define JC_GAUSS_FITTING       63
#define JC_ORTHOGONAL_FITTING  64

#define JC_EXFRAC              65
#define JC_UU                  66
#define JC_BCD                 67
#define JC_TOPHEX              68

#define JC_SI_All              69
#define JC_CPXMULT             70

#ifdef PC_BUILD
//keyboard.c
void JM_DOT(int16_t xx, int16_t yy);
#endif

void fnShowJM  (uint16_t jmConfig);

#endif // JM_H
