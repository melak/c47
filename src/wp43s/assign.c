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

#include "assign.h"

#include "items.h"

#include "wp43s.h"

//This variable is to store in flash memory
TO_QSPI const calcKey_t kbd_std[37] = {
//keyId primary          fShifted         gShifted         keyLblAim        primaryAim         fShiftedAim      gShiftedAim    primaryTam     fShiftedEim
 {21,   ITM_1ONX,        ITM_TGLFRT,     -MNU_ALPHAFN,     ITM_NULL,        ITM_A,            -MNU_ALPHAINTL,   ITM_ALPHA,     ITM_REG_A,    -MNU_ALPHAINTL       },
 {22,  -MNU_EXP,         ITM_toINT,      -MNU_BITS,        ITM_NUMBER_SIGN, ITM_B,             ITM_NUMBER_SIGN, ITM_BETA,      ITM_REG_B,     ITM_NUMBER_SIGN     },
 {23,  -MNU_TRI,         ITM_DMS,        -MNU_ANGLECONV,   ITM_NULL,        ITM_C,             ITM_NULL,        ITM_GAMMA,     ITM_REG_C,     ITM_NULL            },
 {24,   ITM_LN,          ITM_dotD,        ITM_LOG10,       ITM_NULL,        ITM_D,             ITM_NULL,        ITM_DELTA,     ITM_REG_D,     ITM_NULL            }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case D for decimal base
 {25,   ITM_EXP,         ITM_toHMS,       ITM_10x,         ITM_NULL,        ITM_E,             ITM_NULL,        ITM_EPSILON,   ITM_NULL,      ITM_NULL            },
 {26,   ITM_SQUAREROOTX, ITM_AIM,        -MNU_CPX,         ITM_CHECK_MARK,  ITM_F,             ITM_CHECK_MARK,  ITM_PHI,       ITM_alpha,     ITM_ROOT_PARENTHESES},

 {31,   ITM_STO,         ITM_ASSIGN,      ITM_SAVE,        ITM_NULL,        ITM_G,             ITM_ASSIGN,      ITM_GAMMA,     ITM_NULL,      ITM_ASSIGN          },
 {32,   ITM_RCL,         ITM_RBR,         ITM_VIEW,        ITM_NULL,        ITM_H,             ITM_RBR,         ITM_CHI,       ITM_HEX,       ITM_RBR             }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case H for hexadecimal base
 {33,   ITM_Rdown,       ITM_Rup,        -MNU_STK,         ITM_NULL,        ITM_I,             ITM_DOWN_ARROW,  ITM_IOTA,      ITM_REG_I,     ITM_DOWN_ARROW      },
 {34,   ITM_CC,          ITM_MAGNITUDE,   ITM_ANGLE,       ITM_NULL,        ITM_J,             ITM_NULL,        ITM_ETA,       ITM_REG_J,     ITM_PAIR_OF_BARS    },
 {35,   ITM_SHIFTf,      ITM_NULL,        ITM_SNAP,        ITM_SHIFTf,      ITM_SHIFTf,        ITM_NULL,        ITM_SNAP,      ITM_SHIFTf,    ITM_NULL            },
 {36,   ITM_SHIFTg,      ITM_USERMODE,    ITM_NULL,        ITM_SHIFTg,      ITM_SHIFTg,        ITM_USERMODE,    ITM_NULL,      ITM_SHIFTg,    ITM_USERMODE        },

 {41,   ITM_ENTER,       ITM_FILL,        ITM_DROP,        ITM_ENTER,       ITM_ENTER,         ITM_STATUS,      ITM_DROP,      ITM_ENTER,     ITM_STATUS          },
 {42,   ITM_XexY,        ITM_Xex,        -MNU_UNITCONV,    ITM_ex,          ITM_K,             ITM_ex,          ITM_KAPPA,     ITM_REG_K,     ITM_ex              },
 {43,   ITM_CHS,         ITM_DELTAPC,    -MNU_FIN,         ITM_PLUS_MINUS,  ITM_L,             ITM_PLUS_MINUS,  ITM_LAMBDA,    ITM_REG_L,     ITM_PLUS_MINUS      },
 {44,   ITM_EXPONENT,    ITM_DSP,        -MNU_DISP,        ITM_NULL,        ITM_M,             ITM_UP_ARROW,    ITM_MU,        ITM_NULL,      ITM_UP_ARROW        },
 {45,   ITM_BACKSPACE,   ITM_UNDO,       -MNU_CLR,         ITM_BACKSPACE,   ITM_BACKSPACE,     ITM_UNDO,       -MNU_CLR,       ITM_BACKSPACE, ITM_UNDO            },

 {51,   ITM_DIV,         ITM_PARALLEL,    ITM_MOD,         ITM_SLASH,       ITM_N,             ITM_SLASH,       ITM_NU,        ITM_DIV,       ITM_SLASH           },
 {52,   ITM_7,           ITM_NULL,        ITM_NULL,        ITM_7,           ITM_O,             ITM_7,           ITM_OMEGA,     ITM_7,         ITM_7               },
 {53,   ITM_8,           ITM_NULL,        ITM_NULL,        ITM_8,           ITM_P,             ITM_8,           ITM_PI,        ITM_8,         ITM_8               },
 {54,   ITM_9,           ITM_NULL,        ITM_RTN,         ITM_9,           ITM_Q,             ITM_9,           ITM_RTN,       ITM_9,         ITM_9               },
 {55,   ITM_XEQ,         ITM_GTO,         ITM_LBL,         ITM_NULL,        ITM_NULL,          ITM_NULL,        ITM_NULL,      ITM_NULL,      ITM_NULL            },

 {61,   ITM_MULT,        ITM_XFACT,      -MNU_PROB,        ITM_CROSS,       ITM_R,             ITM_PROD_SIGN,   ITM_RHO,       ITM_MULT,      ITM_PROD_SIGN       },
 {62,   ITM_4,          -MNU_STAT,       -MNU_SUMS,        ITM_4,           ITM_S,             ITM_4,           ITM_SIGMA,     ITM_4,         ITM_4               },
 {63,   ITM_5,           ITM_toREC,       ITM_toPOL,       ITM_5,           ITM_T,             ITM_5,           ITM_TAU,       ITM_5,         ITM_5               },
 {64,   ITM_6,           ITM_TIMER,      -MNU_CLK,         ITM_6,           ITM_U,             ITM_6,           ITM_THETA,     ITM_6,         ITM_6               },
 {65,   ITM_UP,          ITM_BST,        -MNU_FLAGS,       ITM_UP,          ITM_UP,            ITM_BST,        -MNU_FLAGS,     ITM_UP,        ITM_BST             },

 {71,   ITM_SUB,        -MNU_INTS,       -MNU_PARTS,       ITM_MINUS,       ITM_V,             ITM_MINUS,      -MNU_ALPHAMATH, ITM_SUB,       ITM_MINUS           },
 {72,   ITM_1,          -MNU_ADV,        -MNU_EQN,         ITM_1,           ITM_W,             ITM_1,           ITM_PSI,       ITM_1,         ITM_1               },
 {73,   ITM_2,          -MNU_MATX,       -MNU_XFN,         ITM_2,           ITM_X,             ITM_2,           ITM_XI,        ITM_2,         ITM_2               },
 {74,   ITM_3,          -MNU_CONST,       ITM_CONSTpi,     ITM_3,           ITM_Y,             ITM_3,           ITM_UPSILON,   ITM_3,         ITM_3               },
 {75,   ITM_DOWN,        ITM_SST,        -MNU_MODE,        ITM_DOWN,        ITM_DOWN,          ITM_SST,        -MNU_MODE,      ITM_DOWN,      ITM_SST             },

 {81,   ITM_ADD,        -MNU_LOOP,       -MNU_TEST,        ITM_PLUS,        ITM_Z,             ITM_PLUS,        ITM_ZETA,      ITM_ADD,       ITM_PLUS            },
 {82,   ITM_0,          -MNU_IO,         -MNU_PRINT,       ITM_0,           ITM_QUESTION_MARK, ITM_0,           ITM_PRINTER,   ITM_0,         ITM_0               },
 {83,   ITM_PERIOD,      ITM_SHOW,       -MNU_INFO,        ITM_PERIOD,      ITM_COMMA,         ITM_PERIOD,     -MNU_ALPHADOT,  ITM_PERIOD,    ITM_PERIOD          },
 {84,   ITM_RS,          ITM_PR,         -MNU_PFN,         ITM_NULL,        ITM_SPACE,         ITM_NULL,        ITM_NULL,      ITM_NULL,      ITM_NULL            },
 {85,   ITM_EXIT,       -MNU_CATALOG,     ITM_OFF,         ITM_EXIT,        ITM_EXIT,         -MNU_CATALOG,     ITM_OFF,       ITM_EXIT,     -MNU_CATALOG         }
//keyId primary          fShifted         gShifted         keyLblAim        primaryAim         fShiftedAim      gShiftedAim    primaryTam     fShiftedEim
};
