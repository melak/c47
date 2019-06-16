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



#ifndef wp43s_H_INCLUDED
#define wp43s_H_INCLUDED

#define VERSION   "Pre-alpha" STD_SPACE_3_PER_EM "version" STD_SPACE_3_PER_EM "2019.04"
#define COPYRIGHT "The WP43S team"
#define WHO       "WP" STD_SPACE_3_PER_EM "43S" STD_SPACE_3_PER_EM "v0.1" STD_SPACE_3_PER_EM "2019.04" STD_SPACE_3_PER_EM "by" STD_SPACE_3_PER_EM "Pauli," STD_SPACE_3_PER_EM "Walter" STD_SPACE_3_PER_EM "&" STD_SPACE_3_PER_EM "Martin"

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-result"
#ifdef PC_BUILD
  #pragma GCC diagnostic ignored "-Wstringop-truncation"
#endif

#define EXTRA_INFO_ON_CALC_ERROR 1
#define DEBUG_PANEL              1
#define DEBUG_REGISTER_L         1
#define STACK_LIFT_DEBUG         1
#define IBM_DECIMAL              1
#define TOMS_FAST_MATH           1

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
#endif

#ifdef DMCP_BUILD
  #undef  DEBUG_PANEL
  #define DEBUG_PANEL 0
  #undef  DEBUG_REGISTER_L
  #define DEBUG_REGISTER_L 0
  #undef  EXTRA_INFO_ON_CALC_ERROR
  #define EXTRA_INFO_ON_CALC_ERROR 0
#endif

#ifdef TESTSUITE_BUILD
  #undef  PC_BUILD
  #undef  DMCP_BUILD
  #undef  DEBUG_PANEL
  #define DEBUG_PANEL 0
  #undef  DEBUG_REGISTER_L
  #define DEBUG_REGISTER_L 0
  #undef  STACK_LIFT_DEBUG
  #define STACK_LIFT_DEBUG 0
  #undef  EXTRA_INFO_ON_CALC_ERROR
  #define EXTRA_INFO_ON_CALC_ERROR 0
  #define addItemToBuffer fnNop
  #define fnOff           fnNop
  #define fnAim           fnNop
  #define fnComplexCCCC   fnNop
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
  #define displayBugScreen(a)     do { printf("\n\nERROR: %s\n", a); abort(); } while(0)
  #define showRealComplexResult() {}
  #define showOverflowCarry()     {}
  #define hideUserMode()          {}
  #define showDateTime()          {}
  #define showAlphaMode()         {}
#endif // TESTSUITE_BUILD

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <math.h>

#ifdef PC_BUILD
  #ifndef TESTSUITE_BUILD
    #include <glib.h>
    #include <gtk/gtk.h>
    #include <gdk/gdk.h>
  #endif
#endif

#ifdef DMCP_BUILD
  #include <main.h>
  #include <dmcp.h>
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
  #define DECNUMDIGITS 51
  #include "decimal128.h"
  #include "decimal64.h"
  #include "decDouble.h"
  #include "decQuad.h"
  #include "decNumberWrappers.h"
#endif

#define MAX_LONG_INTEGER_SIZE_IN_BITS 4096

#if (TOMS_FAST_MATH == 1)
  #define FP_MAX_SIZE (MAX_LONG_INTEGER_SIZE_IN_BITS+(8*DIGIT_BIT))

  #include "tfm.h"
  #include "tomsFastMathWrappers.h"
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
#include "testFunctions.h"

#define min(a,b)                ((a)<(b)?(a):(b))
#define max(a,b)                ((a)>(b)?(a):(b))
#define modulo(a, b)            (((a)%(b))<0 ? ((a)%(b))+(b) : ((a)%(b))) // the % operator is remainder rather than modulo!

// On/Off 1 bit
#define OFF                     0
#define ON                      1

// Denominator mode 2 bits
#define DM_FIX                  0
#define DM_FAC                  1
#define DM_ANY                  2
#define DM_DENMAX            9999

// Short integer mode 2 bits
#define SIM_UNSIGN              0
#define SIM_1COMPL              1
#define SIM_2COMPL              2
#define SIM_SIGNMT              3

#define SHORT_INTEGER_SIZE      8 // 8 bytes = 64 bits

// Display format 2 bits
#define DF_ALL                  0
#define DF_FIX                  1
#define DF_SCI                  2
#define DF_ENG                  3

// Display override 1 bit
#define DO_SCI                  0
#define DO_ENG                  1

// Angular mode 3 bits
#define AM_DEGREE               0 // degree must be 0
#define AM_GRAD                 1 // grad   must be 1
#define AM_RADIAN               2 // radian must be 2
#define AM_MULTPI               3 // multpi must be 3
#define AM_DMS                  4 // dms    must be 4

// Long integer sign
#define LI_POSITIVE             0
#define LI_NEGATIVE             1

// Data type tag
#define TAG_NONE                0

// Time format 1 bit
#define TF_H24                  0
#define TF_H12                  1

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
#define CM_ASSIGN               4 // Assign mode
#define CM_REGISTER_BROWSER     5 // Register browser
#define CM_FLAG_BROWSER         6 // Flag browser
#define CM_FONT_BROWSER         7 // Font browser
#define CM_ERROR_MESSAGE        8 // Error message in one of the register lines
#define CM_BUG_ON_SCREEN        9 // Bug message on screen
#define CM_CONFIRMATION        10 // Waiting for confirmation or canceling

// Next character 2 bits
#define NC_NORMAL               0
#define NC_SUBSCRIPT            1
#define NC_SUPERSCRIPT          2

// Complex unit 1 bit
#define CU_I                    0
#define CU_J                    1
#define COMPLEX_UNIT            (complexUnit == CU_I ? NUM_i : NUM_j)

// Product sign 1 bit
#define PS_DOT                  0
#define PS_CROSS                1
#define PRODUCT_SIGN            (productSign == PS_CROSS ? NUM_CROSS : NUM_DOT)

// Fraction type 1 bit
#define FT_PROPER               0 // a b/c
#define FT_IMPROPER             1 // d/c

// Radix Mark 1 bit
#define RM_PERIOD               0
#define RM_COMMA                1
#define RADIX16_MARK_CHAR       (radixMark == RM_PERIOD ? '.' : ',')
#define RADIX16_MARK_STRING     (radixMark == RM_PERIOD ? "." : ",")
#define RADIX34_MARK_STRING     (radixMark == RM_PERIOD ? NUM_PERIOD34 : NUM_COMMA34)

// Stack size 1 bit
#define SS_4                    0
#define SS_8                    1

// Complex mode 1 bit
#define CM_RECTANGULAR          0
#define CM_POLAR                1

// Alpha case 1 bit
#define AC_UPPER                0
#define AC_LOWER                1

// Cursor font 1 bit
#define CF_NUMERIC              0
#define CF_STANDARD             1

// TAM mode
#define TM_VALUE            10001 // TM_VALUE must be the 1st in this list
#define TM_REGISTER         10002
#define TM_FLAG             10003
#define TM_STORCL           10004
#define TM_CMP              10005 // TM_CMP must be the last in this list

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
#define TI_RESET                1
#define TI_RADIUS_THETA         2
#define TI_X_Y                  3
#define TI_STATISTIC_SUMS       4
#define TI_ARE_YOU_SURE         5
#define TI_VERSION              6
#define TI_WHO                  7
#define TI_FALSE                8
#define TI_TRUE                 9

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
#define DBG_NAMED_REGISTERS            5
#define DBG_TMP_SAVED_STACK_REGISTERS  6

#if (__linux__ == 1)
  #define FMT64U "lu"
  #define FMT64S "ld"
  #define FMT32U "u"
  #define FMT32S "d"
  #define FMT16U "u"
  #define FMT16S "d"
  #define FMT8U  "u"
  #define FMT8S  "d"
  #define FMTPTR "lu"
#elif defined(__arm__)
  #define FMT64U "llu"
  #define FMT64S "lld"
  #define FMT32U "lu"
  #define FMT32S "ld"
  #define FMT16U "u"
  #define FMT16S "d"
  #define FMT8U  "u"
  #define FMT8S  "d"
  #define FMTPTR "d"
#elif defined(__MINGW64__)
  #define FMT64U "I64u"
  #define FMT64S "I64d"
  #define FMT32U "u"
  #define FMT32S "d"
  #define FMT16U "u"
  #define FMT16S "d"
  #define FMT8U  "u"
  #define FMT8S  "d"
  #define FMTPTR "lu"
#elif defined(__APPLE__)
  #define FMT64U "llu"
  #define FMT64S "lld"
  #define FMT32U "u"
  #define FMT32S "d"
  #define FMT16U "u"
  #define FMT16S "d"
  #define FMT8U  "u"
  #define FMT8S  "d"
  #define FMTPTR "lu"
#else
  #error Only Linux, MacOS, ARM and Windows MINGW64 are supported for now
#endif

//#define tamMode                    ((*bits5 >> OFFSET_TAMMODE        ) & ((1 << LENGTH_TAMMODE        ) - 1)) // TAM mode
//#define setTamMode(x)              *bits5 = (*bits5 & ~(((1 << LENGTH_TAMMODE        ) - 1) << OFFSET_TAMMODE        )) | ((x) << OFFSET_TAMMODE        )

#if (STACK_LIFT_DEBUG == 1)
  #define STACK_LIFT_ENABLE  stackLiftEnable();
  #define STACK_LIFT_DISABLE stackLiftDisable();
#else
  #define STACK_LIFT_ENABLE  stackLiftEnabled = true;
  #define STACK_LIFT_DISABLE stackLiftEnabled = false;
#endif

// Variables for the simulator
#ifdef PC_BUILD
  extern bool_t         calcLandscape;
  extern bool_t         calcAutoLandscapePortrait;
  extern bool_t         runTestsOnly;
  extern GtkWidget      *screen;
  extern GtkWidget      *frmCalc;
  extern int16_t        screenStride;
  extern int16_t        debugWindow;
  extern uint32_t       *screenData;
  extern bool_t         screenChange;
  #if (DEBUG_REGISTER_L == 1)
    extern GtkWidget    *lblRegisterL1;
    extern GtkWidget    *lblRegisterL2;
  #endif
#endif

#define RAM_SIZE        32768 //98304 // 96kb
extern char             *ram;
extern bool_t           allowScreenUpdate;
extern bool_t           funcOK;

// Variables stored in FLASH
extern const item_t     indexOfItems[];
extern const char       *errorMessages[NUMBER_OF_ERROR_CODES];
extern const calcKey_t  kbd_std[37];
extern const font_t     standardFont, numericFont;
extern void             (* const addition[13][13])(void);
extern void             (* const subtraction[13][13])(void);
extern void             (* const multiplication[13][13])(void);
extern void             (* const division[13][13])(void);
extern const softmenu_t softmenu[];
extern const int16_t    softkeyRow[];

// Variables stored in RAM
extern decContext       ctxtReal16;
extern decContext       ctxtReal34;
extern decContext       ctxtReal51;
extern decContext       ctxtReal451;
extern uint16_t         flags[7];
#define TMP_STR_LENGTH  3000
#define ERROR_MESSAGE_LENGTH 512
extern char             tmpStr3000[TMP_STR_LENGTH];
extern char             errorMessage[ERROR_MESSAGE_LENGTH];
extern char             aimBuffer[AIM_BUFFER_LENGTH];
extern char             nimBuffer[NIM_BUFFER_LENGTH];
extern char             nimBufferDisplay[NIM_BUFFER_LENGTH];
extern char             tamBuffer[TAM_BUFFER_LENGTH];
extern char             oldTime[8];
extern char             dateTimeString[12];
extern softmenuStack_t  softmenuStack[7];
extern uint32_t         reg[112];
extern uint32_t         savedStackRegister[9];
extern uint32_t         tempRegister[NUMBER_OF_TEMPORARY_REGISTERS];
extern int16_t          tamFunction;
extern int16_t          tamNumber;
extern int16_t          tamNumberMin;
extern int16_t          tamNumberMax;
extern int16_t          tamDigit;
extern int16_t          tamOperation;
extern int16_t          tamLetteredRegister;
extern int16_t          tamCurrentOperation;
extern int16_t          currentRegisterBrowserScreen;
extern int16_t          lineTWidth;
extern int16_t          rbrRegister;
extern int16_t          displayHasNDigits;
extern calcRegister_t   result;
extern calcRegister_t   result1;
extern calcRegister_t   result2;
extern calcRegister_t   opX;
extern calcRegister_t   opY;
extern calcRegister_t   opZ;
extern uint16_t         numberOfLocalRegisters;
extern uint16_t         numberOfLocalFlags;
extern uint16_t         numberOfNamedRegisters;
extern uint32_t         allLocalRegisterPointer;
extern uint32_t         allNamedRegisterPointer;
extern uint32_t         statisticalSumsPointer;
extern uint32_t         firstFreeByte;
extern uint32_t         lastFreeByte;
extern uint16_t         programCounter;
extern uint16_t         xCursor;
extern uint16_t         yCursor;
extern uint16_t         tamMode;
extern uint32_t         firstGregorianDay;
extern uint32_t         denMax;
extern uint32_t         lastIntegerBase;
extern uint8_t          softmenuStackPointer;
extern uint8_t          transitionSystemState;
extern uint8_t          cursorBlinkCounter;
extern uint8_t          numScreensStandardFont;
extern uint8_t          currentFntScr;
extern uint8_t          currentFlgScr;
extern uint8_t          displayFormat;
extern uint8_t          displayFormatDigits;
extern uint8_t          shortIntegerWordSize;
extern uint8_t          denominatorMode;
extern uint8_t          significantDigits;
extern uint8_t          shortIntegerMode;
extern uint8_t          previousCalcMode;
extern uint8_t          groupingGap;
extern uint8_t          dateFormat;
extern uint8_t          curveFitting;
extern uint8_t          roundingMode;
extern uint8_t          calcMode;
extern uint8_t          nextChar;
extern uint8_t          complexUnit;
extern uint8_t          displayStack;
extern uint8_t          productSign;
extern uint8_t          fractionType;
extern uint8_t          radixMark;
extern uint8_t          displayModeOverride;
extern uint8_t          stackSize;
extern uint8_t          complexMode;
extern uint8_t          alphaCase;
extern uint8_t          numLinesNumericFont;
extern uint8_t          numLinesStandardFont;
extern uint8_t          cursorEnabled;
extern uint8_t          cursorFont;
extern uint8_t          nimNumberPart;
extern uint8_t          hexDigits;
extern uint8_t          lastErrorCode;
extern uint8_t          serialIOIconEnabled;
extern uint8_t          timeFormat;
extern uint8_t          temporaryInformation;
extern uint8_t          rbrMode;
extern uint8_t          numScreensNumericFont;
extern uint8_t          currentAngularMode;
extern bool_t           hourGlassIconEnabled;
extern bool_t           watchIconEnabled;
extern bool_t           userModeEnabled;
extern bool_t           printerIconEnabled;
extern bool_t           batteryIconEnabled;
extern bool_t           shiftF;
extern bool_t           shiftG;
extern bool_t           showContent;
extern bool_t           stackLiftEnabled;
extern bool_t           displayLeadingZeros;
extern bool_t           displayRealAsFraction;
extern bool_t           savedStackLiftEnabled;
extern bool_t           tempRegistersInUse[NUMBER_OF_TEMPORARY_REGISTERS];
extern bool_t           rbr1stDigit;
extern bool_t           nimInputIsReal34;
extern calcKey_t        kbd_usr[37];
extern calcRegister_t   errorMessageRegisterLine;
extern calcRegister_t   errorRegisterLine;
extern uint16_t         row[100];
extern uint64_t         shortIntegerMask;
extern uint64_t         shortIntegerSignBit;
extern glyph_t          glyphNotFound;
extern char             transitionSystemOperation[4];
extern int16_t          exponentSignLocation;
extern int16_t          denominatorLocation;
extern int16_t          imaginaryExponentSignLocation;
extern int16_t          imaginaryMantissaSignLocation;
extern void             (*confirmedFunction)(uint16_t);

#ifdef DMCP_BUILD
  extern bool_t               endOfProgram;
#endif // DMCP_BUILD

#include "constantPointers.h"
#endif // wp43s_H_INCLUDED
