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

/********************************************//**
 * \file wp43s.h
 ***********************************************/

//#define SWAP_LAYOUTS    //SWAP THE BELOW TWO DEFINES TO HAVE THE DM42 VERSION ON SIMULATOR
#define V43               //Can only be defined if SWAP LAYOUTS is not set
#define S43               //Can only be defined if SWAP LAYOUTS is not set. Activates only the colour bands on the simulator screen

#define TMP_STR_LENGTH      3000          //JMMAX 3000+ extra to make sure

#ifndef wp43s_H_INCLUDED
#define wp43s_H_INCLUDED

#define YEARMONTH "2020.06"
#define VERSION   "Pre-alpha" STD_SPACE_3_PER_EM "version" STD_SPACE_3_PER_EM YEARMONTH
#define COPYRIGHT "The WP43S team"
#define WHO       "WP" STD_SPACE_3_PER_EM "43S" STD_SPACE_3_PER_EM "v0.1" STD_SPACE_3_PER_EM YEARMONTH STD_SPACE_3_PER_EM "by" STD_SPACE_3_PER_EM "Pauli," STD_SPACE_3_PER_EM "Walter" STD_SPACE_3_PER_EM "&" STD_SPACE_3_PER_EM "Martin"

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-result"

#define EXTRA_INFO_ON_CALC_ERROR    1
#define DEBUG_PANEL                 1
#define DEBUG_REGISTER_L            1
#define SHOW_MEMORY_STATUS          1
#define IBM_DECIMAL                 1
#define LIBGMP                      1
#define TO_BLOCKS(n)                (((n) + 3) >> 2)
#define TO_BYTES(n)                 ((n) << 2)
#define RAM_SIZE                    16384 // 16384 blocks = 65536 bytes  MUST be a multiple of 4 and MUST be <= 262140 (not 262144)
#define WP43S_NULL                  65535
#define TO_PCMEMPTR(p)              ((void *)((p) == WP43S_NULL ? NULL : ram + (p)))
#define TO_WP43SMEMPTR(p)           ((p) == NULL ? WP43S_NULL : (uint16_t)((dataBlock_t *)(p) - ram))
#define FN_KEY_TIMEOUT_TO_NOP       0

#if !defined(PC_BUILD) && !defined(DMCP_BUILD)
  #error One of PC_BUILD and DMCP_BUILD must be defined
#endif

#if defined(PC_BUILD) && defined(DMCP_BUILD)
  #error Only one of PC_BUILD and DMCP_BUILD must be defined
#endif

#ifdef PC_BUILD
  #ifdef __MINGW64__ // No DEBUG_PANEL mode for Windows
    #undef  DEBUG_PANEL
    #define DEBUG_PANEL 0
  #endif
  #ifdef RASPBERRY // No DEBUG_PANEL mode for Raspberry Pi
    #undef  DEBUG_PANEL
    #define DEBUG_PANEL 0
  #endif

  #ifndef SWAP_LAYOUTS
      #define JM_LAYOUT_1A               //Preferred layout
      #undef  JM_LAYOUT_2_DM42_STRICT
  #endif

  #ifdef SWAP_LAYOUTS
      #define JM_LAYOUT_2_DM42_STRICT    //DM42 compatible layout. Temporary SWAP. Change here for screen picture.
      #undef  JM_LAYOUT_1A
      #undef  V43
      #undef  S43
      #define JM_LAYOUT_SHOW_BLUES       //ONLY DEFINE IF BLUE MUST BE DISPLAYED. TEMPORARY FOR CREATING AN EMU FOR THE LAYOUT42
  #endif

#endif

#ifdef DMCP_BUILD
  #undef  DEBUG_PANEL
  #define DEBUG_PANEL 0
  #undef  DEBUG_REGISTER_L
  #define DEBUG_REGISTER_L 0
  #undef  SHOW_MEMORY_STATUS
  #define SHOW_MEMORY_STATUS 0
  #undef  EXTRA_INFO_ON_CALC_ERROR
  #define EXTRA_INFO_ON_CALC_ERROR 0
  #undef  JM_LAYOUT_1A
  #define JM_LAYOUT_2_DM42_STRICT    //DM42 compatible layout
#endif

#if defined(TESTSUITE_BUILD) && !defined(GENERATE_CATALOGS)
  #undef  JM_LAYOUT_1A
  #undef  JM_LAYOUT_2_DM42_STRICT    //DM42 compatible layout

  #undef  PC_BUILD
  #undef  DMCP_BUILD
  #undef  DEBUG_PANEL
  #define DEBUG_PANEL 0
  #undef  DEBUG_REGISTER_L
  #define DEBUG_REGISTER_L 0
  #undef  SHOW_MEMORY_STATUS
  #define SHOW_MEMORY_STATUS 0
  #undef  EXTRA_INFO_ON_CALC_ERROR
  #define EXTRA_INFO_ON_CALC_ERROR 0
  #define addItemToBuffer fnNop
  #define fnOff           fnNop
  #define fnAim           fnNop
  #define registerBrowser fnNop
  #define flagBrowser     fnNop
  #define fontBrowser     fnNop
  #define refreshRegisterLine(a)  {}
  #define clearScreen(a, b, c)    {}
  #define toggleUserMode()        {}
  #define showIntegerMode()       {}
  #define showAngularMode()       {}
  #define showComplexMode()       {}
  #define showPgmBegin()          {}
  #define showFracMode()          {}
  #define displayBugScreen(a)     { printf("\n-----------------------------------------------------------------------\n"); printf("%s\n", a); printf("\n-----------------------------------------------------------------------\n");}
  #define showRealComplexResult() {}
  #define showOverflowCarry()     {}
  #define showDateTime()          {}
  #define showAlphaMode()         {}
  #define showHideUserMode()      {}
  #define showHideLowBattery()    {}
  #define JM_LAYOUT_1A               //Preferred layout
#endif

#ifndef TESTSUITE_BUILD                 //vv dr
#define INLINE_TEST
#undef INLINE_TEST
#endif                                  //^^

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#if (__linux__ == 1)
  #include <math.h>
#endif

#ifdef __APPLE__
    // needed by chdir
    #include<unistd.h>
#endif

#ifdef PC_BUILD
    #include <glib.h>
    #include <gtk/gtk.h>
    #include <gdk/gdk.h>
#endif

#ifdef DMCP_BUILD
  #include <main.h>
  #include <dmcp.h>
  #include <ff_ifc.h>
#endif

/********************************************//**
 * \typedef bool_t
 * \brief Boolean type
 ***********************************************/
typedef enum {
  false = 0,     ///< Value for false
  true  = !false ///< Value for true
} bool_t; // 1 bit


typedef int16_t calcRegister_t;

#if (IBM_DECIMAL == 1)
  #define DECNUMDIGITS 75
  #include "decimal128.h"
  #include "decimal64.h"
  #include "decDouble.h"
  #include "decQuad.h"
  #include "decNumberWrappers.h"
  extern int decGetInt(const decNumber *x);
#endif

#define NUMBER_OF_DISPLAY_DIGITS 16
#define MAX_LONG_INTEGER_SIZE_IN_BITS 9965 //9965   // 43S:3328 //JMMAX
#define NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS   10
#define MAX_FACTORIAL                 1000 //1142   // 43S: 450 //JMMAX

                               // bits  digits  43S     x digits   x! digits
                               //                         69!            98
                               //                        210!           398
                               // 3328  1001    450!     449!           998
                               //                        807!          1997
                               //                        977!          2499
                               // 9965  3000            1142!          2998
                               //15000  4515            1388!
                               //                       2122!          6140

#if (LIBGMP == 1)
  #include <gmp.h>
  #include "gmpWrappers.h"
#endif

#if defined(__MINGW64__)
  #include <locale.h>
#endif


#include "assign.h"
#include "bufferize.h"
#include "browsers/browsers.h"
#include "config.h"
#include "conversionAngles.h"
#include "conversionUnits.h"
#include "curveFitting.h"
#include "dateTime.h"
#include "constants.h"
#include "debug.h"
#include "error.h"
#include "flags.h"
#include "fractions.h"
#include "fonts.h"
#include "charString.h" // must be after font.h
#include "display.h"    // must be after font.h
#include "gui.h"
#include "integers.h"
#include "items.h"
#include "keyboard.h"
#include "logicalOps/logicalOps.h"
#include "mathematics/mathematics.h"
#include "memory.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "screen.h"
#include "timer.h"
#include "saveRestoreCalcState.h"
#include "softmenus.h"
#include "sort.h"
#include "stack.h"
#include "stats.h"
#include "statusBar.h"
#include "stringFuncs.h"
#include "timer.h"
#include "jm_graph.h"                   //JM include 
#include "jm.h"                         //JM include
#include "addons.h"                     //JM include
#include "textfiles.h"                  //JM include
#include "keyboardTweak.h"              //dr
#include "radioButtonCatalog.h"         //dr
#ifdef INLINE_TEST                      //vv dr
#include "inlineTest.h"
#endif                                  //^^

#define min(a,b)                ((a)<(b)?(a):(b))
#define max(a,b)                ((a)>(b)?(a):(b))
#define rmd(n, d)               ((n)%(d))                                                       // rmd(n,d) = n - d*idiv(n,d)   where idiv is the division with decimal part truncature
#define mod(n, d)               (((n)%(d) + (d)) % (d))                                         // mod(n,d) = n - d*floor(n/d)  where floor(a) is the biggest integer <= a
//#define modulo(n, d)            ((n)%(d)<0 ? ((d)<0 ? (n)%(d) - (d) : (n)%(d) + (d)) : (n)%(d)) // modulo(n,d) = rmd(n,d) (+ |d| if rmd(n,d)<0)  thus the result is always >= 0
#define modulo(n, d)            ((n)%(d)<0 ? (n)%(d)+(d) : (n)%(d))                             // This version works only if d > 0

#define NUMBER_OF_CONSTANTS_39   175+2     //JM+2
#define NUMBER_OF_CONSTANTS_51    30
#define NUMBER_OF_CONSTANTS_1071   1
#define NUMBER_OF_CONSTANTS_34     7

#define MAX_FREE_BLOCKS 50

#define nbrOfElements(x)        (sizeof(x) / sizeof((x)[0]))        //dr?? vv
#ifdef DMCP_BUILD
#define LCD_REFRESH_TIMEOUT   125 //timeout for lcd refresh in ms
#else
#define LCD_REFRESH_TIMEOUT   100 //timeout for lcd refresh in ms
#endif 
#define MAX_RADIO_CB_ITEMS     67 //73  //JMMAX                                             //dr build RadioButton, CheckBox

// timer nr for FG and FN use
#define TO_FG_LONG              0
#define TO_CL_LONG              1
#define TO_FG_TIMR              2
#define TO_FN_LONG              3
#define TO_FN_EXEC              4
#define TO_3S_CTFF              5
#define TO_CL_DROP              6
#define TO_KB_ACTV              7                                   //dr ^^

// On/Off 1 bit
#define OFF                     0
#define ON                      1

// Short integer mode 2 bits
#define SIM_UNSIGN              0
#define SIM_1COMPL              1
#define SIM_2COMPL              2
#define SIM_SIGNMT              3

#define SHORT_INTEGER_SIZE      2 // 2 blocks = 8 bytes = 64 bits

// Display format 2 bits
#define DF_ALL                  0
#define DF_FIX                  1
#define DF_SCI                  2
#define DF_ENG                  3
#define DF_SF                   4   //JM
#define DF_UN                   5   //JM

// Angular mode 3 bits
#define AM_DEGREE               0 // degree must be 0
#define AM_GRAD                 1 // grad   must be 1
#define AM_RADIAN               2 // radian must be 2
#define AM_MULTPI               3 // multpi must be 3
#define AM_DMS                  4 // dms    must be 4
#define AM_NONE                 5

// Date format 2 bits
#define DF_DMY                  0
#define DF_YMD                  1
#define DF_MDY                  2

// Curve fitting 3 bits
#define CF_LINEAR_FITTING       0
#define CF_EXPONENTIAL_FITTING  1
#define CF_LOGARITHMIC_FITTING  2
#define CF_POWER_FITTING        3
#define CF_BEST_FITTING         4

// Rounding mode 3 bits
#define RM_HALF_EVEN            0
#define RM_HALF_UP              1
#define RM_HALF_DOWN            2
#define RM_UP                   3
#define RM_DOWN                 4
#define RM_CEIL                 5
#define RM_FLOOR                6

// Calc mode 4 bits
#define CM_NORMAL               0 // Normal operation
#define CM_AIM                  1 // Alpha input mode
#define CM_TAM                  2 // Temporary input mode
#define CM_NIM                  3 // Numeric input mode
#define CM_ASM                  4 // Alpha selection in FCNS, MENU, and CNST menu
#define CM_ASM_OVER_TAM         5 // Alpha selection for system flags selection in TAM
#define CM_ASM_OVER_AIM         6 // Alpha selection for system flags selection in TAM
#define CM_ASSIGN               7 // Assign mode
#define CM_REGISTER_BROWSER     8 // Register browser
#define CM_FLAG_BROWSER         9 // Flag browser
#define CM_FONT_BROWSER        10 // Font browser
#define CM_ERROR_MESSAGE       11 // Error message in one of the register lines
#define CM_BUG_ON_SCREEN       12 // Bug message on screen
#define CM_CONFIRMATION        13 // Waiting for confirmation or canceling
#define CM_FLAG_BROWSER_OLD    99 // Flag browser old                                      //JM

// Next character 2 bits
#define NC_NORMAL               0
#define NC_SUBSCRIPT            1
#define NC_SUPERSCRIPT          2

// Alpha case 1 bit
#define AC_UPPER                0
#define AC_LOWER                1

// Cursor font 1 bit
#define CF_NUMERIC              0
#define CF_STANDARD             1

// TAM mode
#define TM_VALUE            10001 // TM_VALUE must be the 1st in this list
#define TM_VALUE_CHB        10002 // same as TM_VALUE but for ->INT (#) change base
#define TM_REGISTER         10003
#define TM_FLAGR            10004
#define TM_FLAGW            10005
#define TM_STORCL           10006
#define TM_CMP              10007 // TM_CMP must be the last in this list

// NIM number part
#define NP_EMPTY                0
#define NP_INT_10               1
#define NP_INT_16               2
#define NP_INT_BASE             3
#define NP_REAL_FLOAT_PART      4
#define NP_REAL_EXPONENT        5
#define NP_FRACTION_DENOMINATOR 6
#define NP_COMPLEX_INT_PART     7
#define NP_COMPLEX_FLOAT_PART   8
#define NP_COMPLEX_EXPONENT     9

// Temporary information
#define TI_NO_INFO              0
#define TI_RADIUS_THETA         1
#define TI_THETA_RADIUS         2
#define TI_X_Y                  3
#define TI_RE_IM                4
#define TI_STATISTIC_SUMS       5
#define TI_RESET                6
#define TI_ARE_YOU_SURE         7
#define TI_VERSION              8
#define TI_WHO                  9
#define TI_FALSE               10
#define TI_TRUE                11
#define TI_SHOW_REGISTER       12
#define TI_VIEW_REGISTER       13
#define TI_SUMX_SUMY           14
#define TI_MEANX_MEANY         15
#define TI_GEOMMEANX_GEOMMEANY 16
#define TI_WEIGHTEDMEANX       17
#define TI_HARMMEANX_HARMMEANY 18
#define TI_RMSMEANX_RMSMEANY   19
#define TI_WEIGHTEDSAMPLSTDDEV 20
#define TI_WEIGHTEDPOPLSTDDEV  21
#define TI_WEIGHTEDSTDERR      22
#define TI_SAMPLSTDDEV         23
#define TI_POPLSTDDEV          24
#define TI_STDERR              25
#define TI_GEOMSAMPLSTDDEV     26
#define TI_GEOMPOPLSTDDEV      27
#define TI_GEOMSTDERR          28
#define TI_SHOW_REGISTER_BIG   99    //JM_SHOW temporary high number not to clash with new codes

// Register browser mode
#define RBR_GLOBAL              0
#define RBR_LOCAL               1
#define RBR_NAMED               2

// Debug window
#define DBG_BIT_FIELDS                 0
#define DBG_FLAGS                      1
#define DBG_REGISTERS                  2
#define DBG_LOCAL_REGISTERS            3
#define DBG_STATISTICAL_SUMS           4
#define DBG_NAMED_VARIABLES            5
#define DBG_TMP_SAVED_STACK_REGISTERS  6

// alpha selection menus
#define ASM_NONE 0
#define ASM_CNST 1
#define ASM_FCNS 2
#define ASM_MENU 3
#define ASM_SYFL 4
#define ASM_AINT 5
#define ASM_aint 6

// String comparison type
#define CMP_CLEANED_STRING_ONLY 1
#define CMP_EXTENSIVE           2

// Combination / permutation
#define CP_PERMUTATION 0
#define CP_COMBINATION 1

// Statistical sums
#define NUMBER_OF_STATISTICAL_SUMS 23
#define SIGMA_N      ((real_t *)(statisticalSumsPointer))
#define SIGMA_X      ((real_t *)(statisticalSumsPointer + REAL_SIZE))
#define SIGMA_Y      ((real_t *)(statisticalSumsPointer + REAL_SIZE *  2))
#define SIGMA_X2     ((real_t *)(statisticalSumsPointer + REAL_SIZE *  3))
#define SIGMA_X2Y    ((real_t *)(statisticalSumsPointer + REAL_SIZE *  4))
#define SIGMA_Y2     ((real_t *)(statisticalSumsPointer + REAL_SIZE *  5))
#define SIGMA_XY     ((real_t *)(statisticalSumsPointer + REAL_SIZE *  6))
#define SIGMA_lnXlnY ((real_t *)(statisticalSumsPointer + REAL_SIZE *  7))
#define SIGMA_lnX    ((real_t *)(statisticalSumsPointer + REAL_SIZE *  8))
#define SIGMA_ln2X   ((real_t *)(statisticalSumsPointer + REAL_SIZE *  9))
#define SIGMA_YlnX   ((real_t *)(statisticalSumsPointer + REAL_SIZE * 10))
#define SIGMA_lnY    ((real_t *)(statisticalSumsPointer + REAL_SIZE * 11))
#define SIGMA_ln2Y   ((real_t *)(statisticalSumsPointer + REAL_SIZE * 12))
#define SIGMA_XlnY   ((real_t *)(statisticalSumsPointer + REAL_SIZE * 13))
#define SIGMA_lnYonX ((real_t *)(statisticalSumsPointer + REAL_SIZE * 14))
#define SIGMA_X2onY  ((real_t *)(statisticalSumsPointer + REAL_SIZE * 15))
#define SIGMA_1onX   ((real_t *)(statisticalSumsPointer + REAL_SIZE * 16))
#define SIGMA_1onX2  ((real_t *)(statisticalSumsPointer + REAL_SIZE * 17))
#define SIGMA_XonY   ((real_t *)(statisticalSumsPointer + REAL_SIZE * 18))
#define SIGMA_1onY   ((real_t *)(statisticalSumsPointer + REAL_SIZE * 19))
#define SIGMA_1onY2  ((real_t *)(statisticalSumsPointer + REAL_SIZE * 20))
#define SIGMA_X3     ((real_t *)(statisticalSumsPointer + REAL_SIZE * 21))
#define SIGMA_X4     ((real_t *)(statisticalSumsPointer + REAL_SIZE * 22))

#if defined(__arm__)
  #define FMT64U  "llu"
  #define FMT64S  "lld"
  #define FMT32U  "lu"
  #define FMT32S  "ld"
  #define FMT16U  "u"
  #define FMT16S  "d"
  #define FMT8U   "u"
  #define FMT8S   "d"
  #define FMTPTR  "d"
  #define FMTSIZE "d"
#elif (__linux__ == 1)
  #define FMT64U  "lu"
  #define FMT64S  "ld"
  #define FMT32U  "u"
  #define FMT32S  "d"
  #define FMT16U  "u"
  #define FMT16S  "d"
  #define FMT8U   "u"
  #define FMT8S   "d"
  #define FMTPTR  "lu"
  #define FMTSIZE "zd"
#elif defined(__MINGW64__)
  #define FMT64U  "I64u"
  #define FMT64S  "I64d"
  #define FMT32U  "u"
  #define FMT32S  "d"
  #define FMT16U  "u"
  #define FMT16S  "d"
  #define FMT8U   "u"
  #define FMT8S   "d"
  #define FMTPTR  "lu"
  #define FMTSIZE "I64u"
#elif defined(__APPLE__)
  #define FMT64U  "llu"
  #define FMT64S  "lld"
  #define FMT32U  "u"
  #define FMT32S  "d"
  #define FMT16U  "u"
  #define FMT16S  "d"
  #define FMT8U   "u"
  #define FMT8S   "d"
  #define FMTPTR  "lu"
  #define FMTSIZE "zd"
#else
  #error Only Linux, MacOS, ARM, and Windows MINGW64 are supported for now
#endif

//#define tamMode                    ((*bits5 >> OFFSET_TAMMODE        ) & ((1 << LENGTH_TAMMODE        ) - 1)) // TAM mode
//#define setTamMode(x)              *bits5 = (*bits5 & ~(((1 << LENGTH_TAMMODE        ) - 1) << OFFSET_TAMMODE        )) | ((x) << OFFSET_TAMMODE        )

#ifdef PC_BUILD
  #define STACK_LIFT_ENABLE  stackLiftEnable();
  #define STACK_LIFT_DISABLE stackLiftDisable();
#else
  #define STACK_LIFT_ENABLE  setSystemFlag(FLAG_ASLIFT);
  #define STACK_LIFT_DISABLE clearSystemFlag(FLAG_ASLIFT);
#endif

// Variables for the simulator
#if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
  extern bool_t             debugMemAllocation;
#endif
#ifdef PC_BUILD
  extern bool_t              calcLandscape;
  extern bool_t              calcAutoLandscapePortrait;
  extern GtkWidget          *screen;
  extern GtkWidget          *frmCalc;
  extern int16_t             screenStride;
  extern int16_t             debugWindow;
  extern uint32_t           *screenData;
  extern bool_t              screenChange;
  extern char                debugString[10000];
  #if (DEBUG_REGISTER_L == 1)
    extern GtkWidget        *lblRegisterL1;
    extern GtkWidget        *lblRegisterL2;
  #endif
  #if (SHOW_MEMORY_STATUS == 1)
    extern GtkWidget        *lblMemoryStatus;
  #endif
#endif

extern dataBlock_t          *ram;
extern bool_t                allowScreenUpdate;
extern bool_t                funcOK;
extern bool_t                keyActionProcessed;

// Variables stored in FLASH
extern const item_t          indexOfItems[];
extern const char           *errorMessages[NUMBER_OF_ERROR_CODES];
extern const calcKey_t       kbd_std[37];
extern const font_t          standardFont, numericFont;
extern const font_t         *fontForShortInteger;
extern void                  (* const addition[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void);
extern void                  (* const subtraction[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void);
extern void                  (* const multiplication[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void);
extern void                  (* const division[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void);
extern const softmenu_t      softmenu[];

// Variables stored in RAM
extern realContext_t         ctxtReal34;   //   34 digits
extern realContext_t         ctxtReal39;   //   39 digits: used for 34 digits intermediate calculations
extern realContext_t         ctxtReal51;   //   51 digits: used for 34 digits intermediate calculations
extern realContext_t         ctxtReal75;   //   75 digits: used in SLVQ
extern realContext_t         ctxtReal1071; // 1071 digits: used in radian angle reduction
//extern realContext_t         ctxtReal2139; // 2139 digits: used for really big modulo
extern uint16_t              flags[7];
//#define TMP_STR_LENGTH              3000 //Move to beginning of file due to errors
#define ERROR_MESSAGE_LENGTH           384 //512          //JMMAX Temporarily reduced - ORG:512.
#define DISPLAY_VALUE_LEN               80
#define MAX_NUMBER_OF_GLYPHS_IN_STRING 196
extern char                  tmpStr3000[TMP_STR_LENGTH];
extern char                  errorMessage[ERROR_MESSAGE_LENGTH];
extern char                  aimBuffer[AIM_BUFFER_LENGTH];
extern char                  nimBuffer[NIM_BUFFER_LENGTH];
extern char                  nimBufferDisplay[NIM_BUFFER_LENGTH];
extern char                  tamBuffer[TAM_BUFFER_LENGTH];
extern char                  asmBuffer[5];
extern char                  oldTime[8];
extern char                  dateTimeString[12];
extern softmenuStack_t       softmenuStack[7];
extern registerDescriptor_t  reg[112];
extern registerDescriptor_t  savedStackRegister[9+1];
extern int16_t               tamFunction;
extern int16_t               tamNumber;
extern int16_t               tamNumberMin;
extern int16_t               tamNumberMax;
extern int16_t               tamDigit;
extern int16_t               tamOperation;
extern int16_t               tamLetteredRegister;
extern int16_t               tamCurrentOperation;
extern int16_t               currentRegisterBrowserScreen;
extern int16_t               lineTWidth;
extern int16_t               rbrRegister;
extern int16_t               alphaSelectionMenu;
extern int16_t               lastFcnsMenuPos;
extern int16_t               lastMenuMenuPos;
extern int16_t               lastCnstMenuPos;
extern int16_t               lastSyFlMenuPos;
extern int16_t               lastAIntMenuPos;
extern int16_t               showFunctionNameItem;
extern int16_t               SHOWregis;                   //JMSHOW
extern uint16_t              numberOfLocalFlags;
extern dataBlock_t          *allLocalRegisterPointer;
extern dataBlock_t          *allNamedVariablePointer;
extern dataBlock_t          *statisticalSumsPointer;
extern uint16_t              programCounter;
extern uint16_t              xCursor;
extern uint16_t              yCursor;
extern uint16_t              tamMode;
extern uint32_t              firstGregorianDay;
extern uint32_t              denMax;
extern uint32_t              lastIntegerBase;
extern uint32_t              alphaSelectionTimer;
extern uint8_t               softmenuStackPointer;
extern uint8_t               softmenuStackPointerBeforeAIM;
extern uint8_t               transitionSystemState;
extern uint8_t               cursorBlinkCounter;
extern uint8_t               numScreensStandardFont;
extern uint8_t               currentFntScr;
extern uint8_t               currentFlgScr;
extern uint8_t               displayFormat;
extern uint8_t               displayFormatDigits;
extern uint8_t               shortIntegerWordSize;
extern uint8_t               significantDigits;
extern uint8_t               shortIntegerMode;
extern uint8_t               previousCalcMode;
extern uint8_t               groupingGap;
extern uint8_t               curveFitting;
extern uint8_t               roundingMode;
extern uint8_t               calcMode;
extern uint8_t               nextChar;
extern uint8_t               displayStack;
extern uint8_t               productSign;
extern uint8_t               displayModeOverride;
extern uint8_t               alphaCase;
extern uint8_t               numLinesNumericFont;
extern uint8_t               numLinesStandardFont;
extern uint8_t               cursorEnabled;
extern uint8_t               cursorFont;
extern uint8_t               nimNumberPart;
extern uint8_t               hexDigits;
extern uint8_t               lastErrorCode;
extern uint8_t               serialIOIconEnabled;
extern uint8_t               temporaryInformation;
extern uint8_t               rbrMode;
extern uint8_t               numScreensNumericFont;
extern uint8_t               currentAngularMode;
extern int8_t                showFunctionNameCounter;
extern bool_t                hourGlassIconEnabled;
extern bool_t                watchIconEnabled;
extern bool_t                printerIconEnabled;
extern bool_t                shiftF;
extern bool_t                shiftG;
//extern bool_t              shiftStateChanged;    //dr
extern radiocb_t             indexOfRadioCbItems[MAX_RADIO_CB_ITEMS];            //vv dr build RadioButton, CheckBox
//extern uint16_t            cntOfRadioCbItems;                                  //^^
extern bool_t                showContent;
extern bool_t                savedStackLiftEnabled;
extern bool_t                rbr1stDigit;
extern bool_t                updateDisplayValueX;
extern bool_t                AlphaSelectionBufferTimerRunning;                  //JM
extern calcKey_t             kbd_usr[37];
extern calcRegister_t        errorMessageRegisterLine;
extern calcRegister_t        errorRegisterLine;
extern uint16_t              row[100];
extern uint64_t              shortIntegerMask;
extern uint64_t              shortIntegerSignBit;
extern uint64_t              systemFlags;
extern glyph_t               glyphNotFound;
extern char                  transitionSystemOperation[4];
extern char                  displayValueX[DISPLAY_VALUE_LEN];
extern int16_t               exponentSignLocation;
extern int16_t               denominatorLocation;
extern int16_t               imaginaryExponentSignLocation;
extern int16_t               imaginaryMantissaSignLocation;
extern int16_t               exponentLimit;
extern size_t                gmpMemInBytes;
extern size_t                wp43sMemInBytes;
extern freeBlock_t           freeBlocks[MAX_FREE_BLOCKS];
extern int32_t               numberOfFreeBlocks;
extern int32_t               lgCatalogSelection;
extern void                  (*confirmedFunction)(uint16_t);
extern real51_t       const *gammaLanczosCoefficients;
extern real39_t       const *angle180;
extern real39_t       const *angle90;
extern real39_t       const *angle45;
extern pcg32_random_t        pcg32_global;
#ifdef DMCP_BUILD
  extern bool_t              backToDMCP;
  extern uint32_t             nextTimerRefresh;   //dr
#endif // DMCP_BUILD

#define MAX_DENMAX           9999
#define COMPLEX_UNIT         (getSystemFlag(FLAG_CPXj)   ? STD_j     : STD_i)
#define RADIX34_MARK_CHAR    (getSystemFlag(FLAG_DECIMP) ? '.'       : ',')
#define RADIX34_MARK_STRING  (getSystemFlag(FLAG_DECIMP) ? "."       : ",")
#define PRODUCT_SIGN         (getSystemFlag(FLAG_MULTx)  ? STD_CROSS : STD_DOT)

#include "constantPointers.h"

#define TEST_REG(r, comment) { \
                               if(reg[r].dataPointer >= 500) { \
                                 uint32_t a, b; \
                                 a = 1; \
                                 b = 0; \
                                 printf("\n=====> BAD  REGISTER %d DATA POINTER: %u <===== %s\n", r, reg[r].dataPointer, comment); \
                                 reg[r].dataType = a/b; \
                               } \
                               else { \
                                 printf("\n=====> good register %d data pointer: %u <===== %s\n", r, reg[r].dataPointer, comment); \
                               } \
                             }

#define PRINT_LI(lint, comment) { \
                                  int i; \
                                  printf("\n%s", comment); \
                                  if((lint)->_mp_size == 0) printf(" lint=0"); \
                                  else if((lint)->_mp_size < 0) printf(" lint=-"); \
                                  else printf(" lint=+"); \
                                  for(i=0; i<abs((lint)->_mp_size); i++) { \
                                    printf("%lu ", (unsigned long)((lint)->_mp_d[i])); \
                                  } \
                                  printf("  _mp_alloc=%dlimbs=", (lint)->_mp_alloc); \
                                  printf("%lubytes", LIMB_SIZE * (lint)->_mp_alloc); \
                                  printf(" _mp_size=%dlimbs=", abs((lint)->_mp_size)); \
                                  printf("%lubytes", LIMB_SIZE * abs((lint)->_mp_size)); \
                                  printf(" PCaddress=%p", (lint)->_mp_d); \
                                  printf(" 43address=%d", TO_WP43SMEMPTR((lint)->_mp_d)); \
                                  printf("\n"); \
                                }


#define PRINT_LI_REG(reg, comment) { \
                                     int i; \
                                     mp_limb_t *p; \
                                     printf("\n%s", comment); \
                                     if(getRegisterLongIntegerSign(reg) == LI_ZERO) printf("lint=0"); \
                                     else if(getRegisterLongIntegerSign(reg) == LI_NEGATIVE) printf("lint=-"); \
                                     else printf("lint=+"); \
                                     for(i=*REGISTER_DATA_MAX_LEN(reg)/LIMB_SIZE, p=REGISTER_LONG_INTEGER_DATA(reg); i>0; i--, p++) { \
                                       printf("%lu ", *p); \
                                     } \
                                     printf(" maxLen=%dbytes=", *REGISTER_DATA_MAX_LEN(reg)); \
                                     printf("%lulimbs", *REGISTER_DATA_MAX_LEN(reg) / LIMB_SIZE); \
                                     printf("\n"); \
                                    }

#endif // wp43s_H_INCLUDED
