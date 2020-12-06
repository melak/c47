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
 * \file defines.h
 ***********************************************/

#define SWAP_TO_L42_ON_SIM           //JM SWAP THE BELOW TWO DEFINES TO HAVE THE DM42 VERSION ON SIMULATOR
#undef  SWAP_TO_L42_ON_SIM
   #define BLUES_WHEN_SWAPPED        //JM Only applicable if SWAPLAYOUTS is defined. Otherwise ignored
   #undef  BLUES_WHEN_SWAPPED

#define SWAP_TO_L1_ON_DM42           //JM Normally L2 in on DM42
#undef  SWAP_TO_L1_ON_DM42

//*********************************
// JM VARIOUS OPTIONS
//*********************************

#define verbose_default 0               //0 = no text; 1 = essential text; 2 = extra debugging
#define PC_BUILD_TELLTALE
#undef PC_BUILD_TELLTALE           //verbose

//This is to allow the cursors to change the case. Normal on 43S. Off on C43
#define arrowCasechange    false

//This is to allow the creation of a logfile while you type
#define RECORDLOG
#undef  RECORDLOG

//This is to really see what the LCD in the SIM does while programs are running. UGLY.
#define FULLUPDATE
//#undef  FULLUPDATE

//* Key buffer and double clicck detection
#define BUFFER_CLICK_DETECTION    //jm Evaluate the Single/Double/Triple presses
#undef BUFFER_CLICK_DETECTION

#define BUFFER_KEY_COUNT          //dr BUFFER_SIZE has to be at least 8 to become accurate results
#undef BUFFER_KEY_COUNT

#define BUFFER_SIZE 2             //dr muss 2^n betragen (8, 16, 32, 64 ...)
//* Longpress repeat 
#define FUNCTION_NOPTIME 800      //JM SCREEN NOP TIMEOUT FOR FIRST 15 FUNCTIONS

#define JMSHOWCODES_KB3   // top line right   Single Double Triple
#undef JMSHOWCODES_KB3

//wrapping editor
#define  combinationFontsDefault 2; //JM 0 = no large font; 1 = enlarged standardfont; 2 = combination font enlargement
                                    //JM for text wrapping editor. 
                                    //JM Combintionfonts uses large numericfont characters, and if glyph not available then takes standardfont and enlarges it
                                    //JM Otherwise, full enlarged standardfont is used.



//*********************************
//* General configuration defines *
//*********************************
#define EXTRA_INFO_ON_CALC_ERROR         1 // Print extra information on the console about an error
#define DEBUG_PANEL                      1 // Showing registers, local registers, saved stack registers, flags, statistical sums, ... in a debug panel
#define DEBUG_REGISTER_L                 1 // Showing register L content on the PC GUI
#define SHOW_MEMORY_STATUS               1 // Showing the memory status on the PC GUI
#define LIBGMP                           1 // Use GMP for the big integers
#define MMHG_PA_133_3224                 0 //JM mmHg to Pa conversion coefficient is 133.3224 an not 133.322387415
#define FN_KEY_TIMEOUT_TO_NOP            0 // Set to 1 if you want the 6 function keys to timeout
#define MAX_LONG_INTEGER_SIZE_IN_BITS    3328 //JMMAX 9965   // 43S:3328 //JMMAX // 1001 decimal digits: 3328 ≃ log2(10^1001)
#define MAX_FACTORIAL                    449 //JMMAX  1142   // 43S: 450 //JMMAX

                               // bits  digits  43S     x digits   x! digits
                               //                         69!            98
                               //                        210!           398
                               // 3328  1001    450!     449!           998
                               //                        807!          1997
                               //                        977!          2499
                               // 9965  3000            1142!          2998
                               //15000  4515            1388!
                               //                       2122!          6140

#define SHORT_INTEGER_SIZE               2 // 2 blocks = 8 bytes = 64 bits

#define IBM_DECIMAL                      1 // Use the IBM decNumber library for the floating point data type
#define DECNUMDIGITS                    75 // Default number of digits used in the decNumber library

#define SCREEN_800X480                   1 // Set to 0 if you want a keyboard in addition to the screen on Raspberry pi




//*************************
//* Other defines         *
//*************************
#define YEARMONTH                                 "2020.12"
#define VERSION                                   "Pre-alpha" STD_SPACE_3_PER_EM "version" STD_SPACE_3_PER_EM YEARMONTH
#define COPYRIGHT                                 "The WP43S team"
#define WHO                                       "WP" STD_SPACE_3_PER_EM "43S" STD_SPACE_3_PER_EM "v0.1" STD_SPACE_3_PER_EM YEARMONTH STD_SPACE_3_PER_EM "by" STD_SPACE_3_PER_EM "Pauli," STD_SPACE_3_PER_EM "Walter" STD_SPACE_3_PER_EM "&" STD_SPACE_3_PER_EM "Martin"

#define CHARS_PER_LINE                            80 // Used in the flag browser application

#define NUMERIC_FONT_HEIGHT                       36 // In pixel. Used in the font browser application
#define STANDARD_FONT_HEIGHT                      22 // In Pixel. Used in the font browser application
#define NUMBER_OF_NUMERIC_FONT_LINES_PER_SCREEN    5 // Used in the font browser application
#define NUMBER_OF_STANDARD_FONT_LINES_PER_SCREEN   8 // Used in the font browser application

#define AIM_BUFFER_LENGTH                        220 //JMMAX changed from 400 // 199 double byte glyphs + trailing 0 + 1 byte to round up to a 4 byte boundary
#define TAM_BUFFER_LENGTH                         32                          // TODO: find the exact maximum needed
#ifdef BUFFER_CLICK_DETECTION
#define NIM_BUFFER_LENGTH                        100-24-10 //TEMP POC CHANGE FROM 100//JMMAX changed from 200 // TODO: find the exact maximum needed
#else
#define NIM_BUFFER_LENGTH                        100-24 //TEMP POC CHANGE FROM 100//JMMAX changed from 200 // TODO: find the exact maximum needed
#endif

// TAM transition system
#define TT_OPERATION                               0 // +, -, *, /, min, max
#define TT_LETTER                                  1
#define TT_VARIABLE                                2
#define TT_DIGIT                                   3
#define TT_ENTER                                   4
#define TT_DOT                                     5 // For local flags and registers
#define TT_INDIRECT                                6 // For indirect addressing
#define TT_BACKSPACE                               7
#define TT_BASE10                                  8
#define TT_BASE16                                  9
#define TT_NOTHING                                10

#define DEBUG_LINES                               68 // Used in for the debug panel

// List of errors
#define ERROR_NONE                                 0
#define ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN          1
#define ERROR_BAD_TIME_OR_DATE_INPUT               2
#define ERROR_UNDEFINED_OPCODE                     3
#define ERROR_OVERFLOW_PLUS_INF                    4
#define ERROR_OVERFLOW_MINUS_INF                   5
#define ERROR_LABEL_NOT_FOUND                      6
#define ERROR_FUNCTION_NOT_FOUND                   7
#define ERROR_OUT_OF_RANGE                         8
#define ERROR_INVALID_INTEGER_INPUT                9
#define ERROR_INPUT_TOO_LONG                      10
#define ERROR_RAM_FULL                            11
#define ERROR_STACK_CLASH                         12
#define ERROR_OPERATION_UNDEFINED                 13
#define ERROR_WORD_SIZE_TOO_SMALL                 14
#define ERROR_TOO_FEW_DATA                        15
#define ERROR_INVALID_DISTRIBUTION_PARAM          16
#define ERROR_IO                                  17
#define ERROR_INVALID_CORRUPTED_DATA              18
#define ERROR_FLASH_MEMORY_WRITE_PROTECTED        19
#define ERROR_NO_ROOT_FOUND                       20
#define ERROR_MATRIX_MISMATCH                     21
#define ERROR_SINGULAR_MATRIX                     22
#define ERROR_FLASH_MEMORY_FULL                   23
#define ERROR_INVALID_DATA_TYPE_FOR_OP            24
#define ERROR_WP34S_COMPAT                        25
#define ERROR_ENTER_NEW_NAME                      26
#define ERROR_CANNOT_DELETE_PREDEF_ITEM           27
#define ERROR_NO_SUMMATION_DATA                   28
#define ERROR_ITEM_TO_BE_CODED                    29
#define ERROR_FUNCTION_TO_BE_CODED                30
#define ERROR_INPUT_DATA_TYPE_NOT_MATCHING        31
#define ERROR_WRITE_PROTECTED_SYSTEM_FLAG         32
#define ERROR_STRING_WOULD_BE_TOO_LONG            33
#define ERROR_EMPTY_STRING                        34
#define ERROR_NO_BACKUP_DATA                      35
#define ERROR_BAD_INPUT                           36 // This error is not in ReM and cannot occur (theoretically).

#define NUMBER_OF_ERROR_CODES                     37

#define NUMBER_OF_GLOBAL_FLAGS                   112
#define NUMBER_OF_LOCAL_FLAGS                     16 // Could be 32 because 32 bits are available

// Global flags
#define FLAG_X                                   100
#define FLAG_Y                                   101
#define FLAG_Z                                   102
#define FLAG_T                                   103
#define FLAG_A                                   104
#define FLAG_B                                   105
#define FLAG_C                                   106
#define FLAG_D                                   107
#define FLAG_L                                   108
#define FLAG_I                                   109
#define FLAG_J                                   110
#define FLAG_K                                   111

// System flags
// Bit 15 (MSB) is always set for a system flag
// If bit 14 is set the system flag is read only for the user
#define FLAG_TDM24                            0x8000
#define FLAG_YMD                              0xc001
#define FLAG_DMY                              0xc002
#define FLAG_MDY                              0xc003
#define FLAG_CPXRES                           0x8004
#define FLAG_CPXj                             0x8005
#define FLAG_POLAR                            0x8006
#define FLAG_FRACT                            0x8007
#define FLAG_PROPFR                           0x8008
#define FLAG_DENANY                           0x8009
#define FLAG_DENFIX                           0x800a
#define FLAG_CARRY                            0x800b
#define FLAG_OVERFLOW                         0x800c
#define FLAG_LEAD0                            0x800d
#define FLAG_ALPHA                            0xc00e
#define FLAG_alphaCAP                         0xc00f
#define FLAG_RUNTIM                           0xc010
#define FLAG_RUNIO                            0xc011
#define FLAG_PRINT                            0xc012
#define FLAG_TRACE                            0x8013
#define FLAG_USER                             0x8014
#define FLAG_LOWBAT                           0xc015
#define FLAG_SLOW                             0x8016
#define FLAG_SPCRES                           0x8017
#define FLAG_SSIZE8                           0x8018
#define FLAG_QUIET                            0x8019
#define FLAG_DECIMP                           0x801a
#define FLAG_MULTx                            0x801b
#define FLAG_ALLENG                           0x801c
#define FLAG_GROW                             0x801d
#define FLAG_AUTOFF                           0x801e
#define FLAG_AUTXEQ                           0x801f
#define FLAG_PRTACT                           0x8020
#define FLAG_NUMIN                            0xc021
#define FLAG_ALPIN                            0xc022
#define FLAG_ASLIFT                           0xc023
#define FLAG_IGN1ER                           0x8024
#define FLAG_INTING                           0xc025
#define FLAG_SOLVING                          0xc026
#define FLAG_VMDISP                           0xc027
#define FLAG_USB                              0xc028
#define NUMBER_OF_SYSTEM_FLAGS                    41

#define LI_ZERO                                    0 // Long integer sign 0
#define LI_NEGATIVE                                1 // Long integer sign -
#define LI_POSITIVE                                2 // Long integer sign +


#ifdef PC_BUILD
  #ifndef SWAP_TO_L42_ON_SIM
      #define JM_LAYOUT_1A               //Preferred layout
      #undef  JM_LAYOUT_2_DM42_STRICT
  #endif

  #ifdef SWAP_TO_L42_ON_SIM
      #define JM_LAYOUT_2_DM42_STRICT    //DM42 compatible layout. Temporary SWAP. Change here for screen picture.
      #undef  JM_LAYOUT_1A
      #undef  JM_LAYOUT_SHOW_BLUES       //ONLY DEFINE IF BLUE MUST BE DISPLAYED. TEMPORARY FOR CREATING AN EMU FOR THE LAYOUT42
      #ifdef BLUES_WHEN_SWAPPED
        #define JM_LAYOUT_SHOW_BLUES       //ONLY DEFINE IF BLUE MUST BE DISPLAYED. TEMPORARY FOR CREATING AN EMU FOR THE LAYOUT42
      #endif
  #endif
#endif


// PC GUI
#if   defined(JM_LAYOUT_2_DM42_STRICT) && !defined(JM_LAYOUT_SHOW_BLUES)                    //JM LAYOUT 2
  #define CSSFILE "c43_pre_L2.css"              //JM L
#elif defined(JM_LAYOUT_2_DM42_STRICT) && defined(JM_LAYOUT_SHOW_BLUES)                    //JM LAYOUT 2
  #define CSSFILE "c43_pre_L2B.css"              //JM L
#elif defined(JM_LAYOUT_1A) || defined(JM_LAYOUT_SHOW_BLUES)                                //JM LAYOUT 1
  #define CSSFILE "c43_pre.css"
#endif //JM L

#define GAP                                        6 //JM original GUI legacy
#define Y_OFFSET_LETTER                           18 //JM original GUI legacy
#define X_OFFSET_LETTER                            3 //JM original GUI legacy
#define Y_OFFSET_SHIFTED_LABEL                    25 //JM original GUI legacy
#define Y_OFFSET_GREEK                            27 //JM original GUI legacy

#define DELTA_KEYS_X                              78 // Horizontal key step in pixel (row of 6 keys)
#define DELTA_KEYS_Y                              74 // Vertical key step in pixel
#define KEY_WIDTH_1                               47 // Width of small keys (STO, RCL, ...)
#define KEY_WIDTH_2                               56 // Width of large keys (1, 2, 3, ...)

#define X_LEFT_PORTRAIT                           45 // Horizontal offset for a portrait calculator
#define X_LEFT_LANDSCAPE                         544 // Horizontal offset for a landscape calculator
#define Y_TOP_PORTRAIT                           376 // Vertical offset for a portrait calculator
#define Y_TOP_LANDSCAPE                           30 // vertical offset for a landscape calculator

// Stack Lift Status
#define SLS_ENABLED                                0 // Stack lift enabled after item execution
#define SLS_DISABLED                               1 // Stack lift disabled after item execution
#define SLS_UNCHANGED                              2 // Stack lift unchanged after item execution

// Undo Status
#define US_ENABLED                                 0 // The command saves the stack, the statistical sums, and the system flags for later UNDO
#define US_CANCEL                                  1 // The command cancels the last UNDO data
#define US_UNCHANGED                               2 // The command leaves the existing UNDO data as is

// Item category
#define CAT_NONE                                   0 // None of the others
#define CAT_FNCT                                   1 // Function
#define CAT_MENU                                   2 // Menu
#define CAT_CNST                                   3 // Constant
#define CAT_FREE                                   4 // To identify and find the free items
#define CAT_REGS                                   5 // Registers
#define CAT_RVAR                                   6 // Reserved variable
#define CAT_DUPL                                   7 // Duplicate of another item e.g. acus->m^2
#define CAT_SYFL                                   8 // System flags
#define CAT_AINT                                   9 // Upper case alpha_INTL
#define CAT_aint                                  10 // Lower case alpha_intl

#define INC_FLAG                                   0
#define DEC_FLAG                                   1

///////////////////////////////////////////////////////
// Register numbering:
//    0 to  111 global resisters
//  112 to  211 local registers (from .00 to .99) this are 100 local registers but TAM allows only a parameter from 0 to 99 (without indirection)
// 1000 to 1999 named variables
// 2000 to 2009 saved stack registers (UNDO item)
#define REGISTER_X                               100
#define REGISTER_Y                               101
#define REGISTER_Z                               102
#define REGISTER_T                               103
#define REGISTER_A                               104
#define REGISTER_B                               105
#define REGISTER_C                               106
#define REGISTER_D                               107
#define REGISTER_L                               108
#define REGISTER_I                               109
#define REGISTER_J                               110
#define REGISTER_K                               111
#define FIRST_LOCAL_REGISTER                     112 // There are 112 global registers from 0 to 111
#define FIRST_NAMED_VARIABLE                    1000
#define FIRST_SAVED_STACK_REGISTER              2000
#define SAVED_REGISTER_X                        2000
#define SAVED_REGISTER_Y                        2001
#define SAVED_REGISTER_Z                        2002
#define SAVED_REGISTER_T                        2003
#define SAVED_REGISTER_A                        2004
#define SAVED_REGISTER_B                        2005
#define SAVED_REGISTER_C                        2006
#define SAVED_REGISTER_D                        2007
#define SAVED_REGISTER_L                        2008
#define LAST_SAVED_REGISTER                     2009
#define TEMP_REGISTER                           2009


// If one of the 4 next defines is changed: change also Y_POSITION_OF_???_LINE below
#define AIM_REGISTER_LINE                 REGISTER_X
#define TAM_REGISTER_LINE                 REGISTER_T
#define NIM_REGISTER_LINE                 REGISTER_X // MUST be REGISTER_X
#define ERR_REGISTER_LINE                 REGISTER_Z
#define TRUE_FALSE_REGISTER_LINE          REGISTER_Z

// If one of the 4 next defines is changed: change also ???_REGISTER_LINE above
#define Y_POSITION_OF_AIM_LINE        Y_POSITION_OF_REGISTER_X_LINE
#define Y_POSITION_OF_TAM_LINE        Y_POSITION_OF_REGISTER_T_LINE
#define Y_POSITION_OF_NIM_LINE        Y_POSITION_OF_REGISTER_X_LINE
#define Y_POSITION_OF_ERR_LINE        Y_POSITION_OF_REGISTER_Z_LINE
#define Y_POSITION_OF_TRUE_FALSE_LINE Y_POSITION_OF_REGISTER_Z_LINE



#define SCREEN_WIDTH                             400 // Width of the screen
#define SCREEN_HEIGHT                            240 // Height of the screen
#define ON_PIXEL                            0x303030 // blue red green
#define OFF_PIXEL                           0xe0e0e0 // blue red green
#define SOFTMENU_STACK_SIZE                        7 //JMMAX 7 // maximum is 14 else we need to increase LENGTH_SOFTMENUSTKPTR
#define TEMPORARY_INFO_OFFSET                     10 // Vertical offset for temporary informations. I find 4 looks better
#define REGISTER_LINE_HEIGHT                      36 //

#define Y_POSITION_OF_REGISTER_T_LINE             24 // 135 - REGISTER_LINE_HEIGHT*(registerNumber - REGISTER_X)
#define Y_POSITION_OF_REGISTER_Z_LINE             60
#define Y_POSITION_OF_REGISTER_Y_LINE             96
#define Y_POSITION_OF_REGISTER_X_LINE            132

#define MY_ALPHA_MENU_CNST    0  //JM This is the index of the MyAlpha   softmenu in the softmenu[] array. //JM changed this to a variable: int16_t MY_ALPHA_MENU;

#define SOFTMENU_HEIGHT                           23

// Horizontal offsets in the status bar
#define X_DATE                                     1
#define X_REAL_COMPLEX                           133
#define X_COMPLEX_MODE                           143
#define X_ANGULAR_MODE                           157
#define X_FRAC_MODE                              185
#define X_INTEGER_MODE                           260
#define X_OVERFLOW_CARRY                         292
#define X_ALPHA_MODE                             300
#define X_HOURGLASS                              311
#define X_PROGRAM_BEGIN                          324
#define X_WATCH                                  336
#define X_SERIAL_IO                              351
#define X_PRINTER                                361
#define X_USER_MODE                              375
#define X_BATTERY                                389

#define TIMER_IDX_SCREEN_REFRESH                   0 // use timer 0 to wake up for screen refresh

// timer nr for FG and FN use                                       //dr vv
#define TO_FG_LONG                                 0
#define TO_CL_LONG                                 1
#define TO_FG_TIMR                                 2
#define TO_FN_LONG                                 3
#define TO_FN_EXEC                                 4
#define TO_3S_CTFF                                 5
#define TO_CL_DROP                                 6
#define TO_KB_ACTV                                 7                //dr ^^



#ifdef PC_BUILD
  #if (__linux__ == 1)
    #define LINEBREAK                           "\n"
  #elif defined(__MINGW64__)
    #define LINEBREAK                         "\n\r"
  #elif defined(__APPLE__)
    #define LINEBREAK                         "\r\n"
  #else
    #error Only Linux, MacOS, and Windows MINGW64 are supported for now
  #endif
#endif

#define NUMBER_OF_DISPLAY_DIGITS                  16
#define NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS     10

// Number of constants
#define NUMBER_OF_CONSTANTS_39                   177+2   //JM 2 additionalconstants
#define NUMBER_OF_CONSTANTS_51                    30
#define NUMBER_OF_CONSTANTS_1071                   1
#define NUMBER_OF_CONSTANTS_34                     8

#define MAX_FREE_REGION                           50 // Maximum number of free memory regions

// On/Off 1 bit
#define OFF                                        0
#define ON                                         1

// Short integer mode 2 bits
#define SIM_UNSIGN                                 0
#define SIM_1COMPL                                 1
#define SIM_2COMPL                                 2
#define SIM_SIGNMT                                 3

// Display format 2 bits
#define DF_ALL                                     0
#define DF_FIX                                     1
#define DF_SCI                                     2
#define DF_ENG                                     3
#define DF_SF                                      4   //JM
#define DF_UN                                      5   //JM

// Angular mode 3 bits
#define AM_DEGREE                                  0 // degree must be 0
#define AM_GRAD                                    1 // grad   must be 1
#define AM_RADIAN                                  2 // radian must be 2
#define AM_MULTPI                                  3 // multpi must be 3
#define AM_DMS                                     4 // dms    must be 4
#define AM_NONE                                    5
#define AM_HMS                                     6   //JM

// Date format 2 bits
#define DF_DMY                                     0
#define DF_YMD                                     1
#define DF_MDY                                     2

// Curve fitting 10 bits
#define CF_LINEAR_FITTING                          1
#define CF_EXPONENTIAL_FITTING                     2
#define CF_LOGARITHMIC_FITTING                     4
#define CF_POWER_FITTING                           8
#define CF_ROOT_FITTING                           16
#define CF_HYPERBOLIC_FITTING                     32
#define CF_PARABOLIC_FITTING                      64
#define CF_CAUCHY_FITTING                        128
#define CF_GAUSS_FITTING                         256
#define CF_ORTHOGONAL_FITTING                    512

// Rounding mode 3 bits
#define RM_HALF_EVEN                               0
#define RM_HALF_UP                                 1
#define RM_HALF_DOWN                               2
#define RM_UP                                      3
#define RM_DOWN                                    4
#define RM_CEIL                                    5
#define RM_FLOOR                                   6

// Calc mode 4 bits
#define CM_NORMAL                                  0 // Normal operation
#define CM_AIM                                     1 // Alpha input mode
#define CM_TAM                                     2 // Temporary input mode
#define CM_NIM                                     3 // Numeric input mode
#define CM_ASM                                     4 // Alpha selection in FCNS, MENU, and CNST menu
#define CM_ASM_OVER_TAM                            5 // Alpha selection for system flags selection in TAM
#define CM_ASM_OVER_AIM                            6 // Alpha selection for system flags selection in TAM
#define CM_ASSIGN                                  7 // Assign mode
#define CM_REGISTER_BROWSER                        8 // Register browser
#define CM_FLAG_BROWSER                            9 // Flag browser
#define CM_FONT_BROWSER                           10 // Font browser
#define CM_ERROR_MESSAGE                          11 // Error message in one of the register lines
#define CM_BUG_ON_SCREEN                          12 // Bug message on screen
#define CM_CONFIRMATION                           13 // Waiting for confirmation or canceling
#define CM_GRAPH                                  97 //JM Display graph       //JM
#define CM_LISTXY                                 98 //JM Display stat list   //JM
#define CM_FLAG_BROWSER_OLD                       99 //JM Flag browser old                                      //JM

// Next character in AIM 2 bits
#define NC_NORMAL                                  0
#define NC_SUBSCRIPT                               1
#define NC_SUPERSCRIPT                             2

// Alpha case 1 bit
#define AC_UPPER                                   0
#define AC_LOWER                                   1

// TAM mode
#define TM_VALUE                               10001 // TM_VALUE must be the 1st in this list
#define TM_VALUE_CHB                           10002 // same as TM_VALUE but for ->INT (#) change base
#define TM_REGISTER                            10003
#define TM_FLAGR                               10004
#define TM_FLAGW                               10005
#define TM_STORCL                              10006
#define TM_SHUFFLE                             10007
#define TM_CMP                                 10008 // TM_CMP must be the last in this list

// NIM number part
#define NP_EMPTY                                   0
#define NP_INT_10                                  1 // Integer base 10
#define NP_INT_16                                  2 // Integer base > 10
#define NP_INT_BASE                                3 // Integer: the base
#define NP_REAL_FLOAT_PART                         4 // Decimal part of the real
#define NP_REAL_EXPONENT                           5 // Ten exponent of the real
#define NP_FRACTION_DENOMINATOR                    6 // Denominator of the fraction
#define NP_COMPLEX_INT_PART                        7 // Integer part of the complex imaginary part
#define NP_COMPLEX_FLOAT_PART                      8 // Decimal part of the complex imaginary part
#define NP_COMPLEX_EXPONENT                        9 // Ten exponent of the complex imaginary part

// Temporary information
#define TI_NO_INFO                                 0
#define TI_RADIUS_THETA                            1
#define TI_THETA_RADIUS                            2
#define TI_X_Y                                     3
#define TI_RE_IM                                   4
#define TI_STATISTIC_SUMS                          5
#define TI_RESET                                   6
#define TI_ARE_YOU_SURE                            7
#define TI_VERSION                                 8
#define TI_WHO                                     9
#define TI_FALSE                                  10
#define TI_TRUE                                   11
#define TI_SHOW_REGISTER                          12
#define TI_VIEW_REGISTER                          13
#define TI_SUMX_SUMY                              14
#define TI_MEANX_MEANY                            15
#define TI_GEOMMEANX_GEOMMEANY                    16
#define TI_WEIGHTEDMEANX                          17
#define TI_HARMMEANX_HARMMEANY                    18
#define TI_RMSMEANX_RMSMEANY                      19
#define TI_WEIGHTEDSAMPLSTDDEV                    20
#define TI_WEIGHTEDPOPLSTDDEV                     21
#define TI_WEIGHTEDSTDERR                         22
#define TI_SAMPLSTDDEV                            23
#define TI_POPLSTDDEV                             24
#define TI_STDERR                                 25
#define TI_GEOMSAMPLSTDDEV                        26
#define TI_GEOMPOPLSTDDEV                         27
#define TI_GEOMSTDERR                             28
#define TI_SAVED                                  29
#define TI_BACKUP_RESTORED                        30
#define TI_XMIN_YMIN                              31
#define TI_XMAX_YMAX                              32

// Register browser mode
#define RBR_GLOBAL                                 0 // Global registers are browsed
#define RBR_LOCAL                                  1 // Local registers are browsed
#define RBR_NAMED                                  2 // Named variables are browsed

// Debug window
#define DBG_BIT_FIELDS                             0
#define DBG_FLAGS                                  1
#define DBG_REGISTERS                              2
#define DBG_LOCAL_REGISTERS                        3
#define DBG_STATISTICAL_SUMS                       4
#define DBG_NAMED_VARIABLES                        5
#define DBG_TMP_SAVED_STACK_REGISTERS              6

// alpha selection menus
#define ASM_NONE                                   0
#define ASM_CNST                                   1
#define ASM_FCNS                                   2
#define ASM_MENU                                   3
#define ASM_SYFL                                   4
#define ASM_AINT                                   5
#define ASM_aint                                   6

// String comparison type
#define CMP_CLEANED_STRING_ONLY                    1
#define CMP_EXTENSIVE                              2

// Combination / permutation
#define CP_PERMUTATION                             0
#define CP_COMBINATION                             1

#define GD_DIRECT_FUNCTION                         0
#define GD_INVERSE_FUNCTION                        1

// Load mode
#define LM_ALL                                     0
#define LM_PROGRAMS                                1
#define LM_REGISTERS                               2
#define LM_NAMED_VARIABLES                         3
#define LM_SUMS                                    4
#define LM_SYSTEM_STATE                            5

// Statistical sums
#define NUMBER_OF_STATISTICAL_SUMS                27
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
#define SIGMA_XMIN   ((real_t *)(statisticalSumsPointer + REAL_SIZE * 23))
#define SIGMA_XMAX   ((real_t *)(statisticalSumsPointer + REAL_SIZE * 24))
#define SIGMA_YMIN   ((real_t *)(statisticalSumsPointer + REAL_SIZE * 25))
#define SIGMA_YMAX   ((real_t *)(statisticalSumsPointer + REAL_SIZE * 26))

#define TMP_STR_LENGTH                          3000 //JMMAX ORG:3000
#define ERROR_MESSAGE_LENGTH                     325 //JMMAX 512          //JMMAX Temporarily reduced - ORG:512.
#define DISPLAY_VALUE_LEN                         80
#define MAX_NUMBER_OF_GLYPHS_IN_STRING           196
#define NUMBER_OF_GLYPH_ROWS                     100+6  //JM 100-->106 // Used in the font browser application

#define MAX_DENMAX                              9999 // Biggest denominator in fraction display mode

#ifdef DMCP_BUILD
#define SCREEN_REFRESH_PERIOD                    160 // 500 //JM timeout for lcd refresh in ms 125
#else
#define SCREEN_REFRESH_PERIOD                    100 // 500 //JM timeout for lcd refresh in ms 100
#endif

#define RAM_SIZE                               16384 // 16384 blocks = 65536 bytes  MUST be a multiple of 4 and MUST be <= 262140 (not 262144)
#define WP43S_NULL                             65535 // NULL pointer

#define CONFIG_SIZE            TO_BLOCKS(sizeof(dtConfigDescriptor_t))




//******************************
//* Macros replacing functions *
//******************************
#if (EXTRA_INFO_ON_CALC_ERROR == 0) || defined(TESTSUITE_BUILD) || defined(DMCP_BUILD)
  #define EXTRA_INFO_MESSAGE(function, msg)
#else // EXTRA_INFO_ON_CALC_ERROR == 1
  #define EXTRA_INFO_MESSAGE(function, msg)  {sprintf(errorMessage, msg); moreInfoOnError("In function ", function, errorMessage, NULL);}
#endif // EXTRA_INFO_ON_CALC_ERROR

#define isSystemFlagWriteProtected(sf)       ((sf & 0x4000) != 0)
#define getSystemFlag(sf)                    ((systemFlags &   ((uint64_t)1 << (sf & 0x3fff))) != 0)
#define setSystemFlag(sf)                    { systemFlags |=  ((uint64_t)1 << (sf & 0x3fff)); systemFlagAction(sf, 1); }
#define clearSystemFlag(sf)                  { systemFlags &= ~((uint64_t)1 << (sf & 0x3fff)); systemFlagAction(sf, 0); }
#define flipSystemFlag(sf)                   { systemFlags ^=  ((uint64_t)1 << (sf & 0x3fff)); systemFlagAction(sf, 2); }
#define shortIntegerIsZero(op)               (((*(uint64_t *)(op)) == 0) || (shortIntegerMode == SIM_SIGNMT && (((*(uint64_t *)(op)) == 1u<<((uint64_t)shortIntegerWordSize-1)))))
#define getStackTop()                        (getSystemFlag(FLAG_SSIZE8) ? REGISTER_D : REGISTER_T)
#define freeRegisterData(regist)             freeWp43s((void *)getRegisterDataPointer(regist), TO_BYTES(getRegisterFullSize(regist)))
#define storeToDtConfigDescriptor(config)    (configToStore->config = config)
#define recallFromDtConfigDescriptor(config) (config = configToRecall->config)
#define getRecalledSystemFlag(sf)            ((configToRecall->systemFlags &   ((uint64_t)1 << (sf & 0x3fff))) != 0)
#define TO_BLOCKS(n)                         (((n) + 3) >> 2)
#define TO_BYTES(n)                          ((n) << 2)
#define TO_PCMEMPTR(p)                       ((void *)((p) == WP43S_NULL ? NULL : ram + (p)))
#define TO_WP43SMEMPTR(p)                    ((p) == NULL ? WP43S_NULL : (uint16_t)((dataBlock_t *)(p) - ram))
#define min(a,b)                             ((a)<(b)?(a):(b))
#define max(a,b)                             ((a)>(b)?(a):(b))
#define rmd(n, d)                            ((n)%(d))                                                       // rmd(n,d) = n - d*idiv(n,d)   where idiv is the division with decimal part truncature
#define mod(n, d)                            (((n)%(d) + (d)) % (d))                                         // mod(n,d) = n - d*floor(n/d)  where floor(a) is the biggest integer <= a
//#define modulo(n, d)                         ((n)%(d)<0 ? ((d)<0 ? (n)%(d) - (d) : (n)%(d) + (d)) : (n)%(d)) // modulo(n,d) = rmd(n,d) (+ |d| if rmd(n,d)<0)  thus the result is always >= 0
#define modulo(n, d)                         ((n)%(d)<0 ? (n)%(d)+(d) : (n)%(d))                             // This version works only if d > 0
#define COMPLEX_UNIT                         (getSystemFlag(FLAG_CPXj)   ? STD_j     : STD_i)
#define RADIX34_MARK_CHAR                    (getSystemFlag(FLAG_DECIMP) ? '.'       : ',')
#define RADIX34_MARK_STRING                  (getSystemFlag(FLAG_DECIMP) ? "."       : ",")
#define PRODUCT_SIGN                         (getSystemFlag(FLAG_MULTx)  ? STD_CROSS : STD_DOT)


//************************
//* Macros for debugging *
//************************
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
