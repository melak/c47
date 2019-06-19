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
 * \file assign.c
 ***********************************************/

#include "wp43s.h"

//This variable is to store in flash memory
const calcKey_t kbd_std[37] = {
//keyId primary           fShifted         gShifted      keyLblAim       primaryAim         fShiftedAim      gShiftedAim    primaryTam
 {21,   ITM_1X,           ITM_PROFRC,      ITM_IMPFRC,   ITM_NULL,       CHR_A,             ITM_NULL,        CHR_ALPHA,     ITM_ST_A     },
 {22,   ITM_YX,           ITM_toINT,      -MNU_EXP,      ITM_NULL,       CHR_B,             ITM_NULL,        CHR_BETA,      ITM_ST_B     },
 {23,  -MNU_TRI,          ITM_DMS,         ITM_pi,       ITM_NULL,       CHR_C,             ITM_NULL,        CHR_GAMMA,     ITM_ST_C     },
 {24,   ITM_LN,           KEY_dotD,        ITM_LOG10,    ITM_NULL,       CHR_D,             ITM_NULL,        CHR_DELTA,     ITM_ST_D     }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case D for decimal base
 {25,   ITM_EX,           ITM_toHMS,       ITM_10x,      ITM_NULL,       CHR_E,             ITM_NULL,        CHR_EPSILON,   ITM_NULL     },
 {26,   ITM_SQUAREROOTX,  ITM_AIM,        -MNU_ALPHAFN,  ITM_NULL,       CHR_F,             ITM_NULL,        CHR_PHI,       CHR_alpha    },

 {31,   ITM_STO,          ITM_ASSIGN,      ITM_SAVE,     ITM_NULL,       CHR_G,             ITM_ASSIGN,      CHR_GAMMA,     ITM_NULL     },
 {32,   ITM_RCL,          ITM_VIEW,       -MNU_CNST,     ITM_NULL,       CHR_H,             ITM_NULL,        CHR_CHI,       ITM_HEX      },
 {33,   ITM_Rdown,        ITM_RBR,         ITM_Rup,      ITM_NULL,       CHR_I,             CHR_DOWN_ARROW,  CHR_IOTA,      ITM_REGI     }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case H for hexadecimal base
 {34,   KEY_CC,           ITM_MAGNITUDE,  -MNU_CPX,      ITM_NULL,       CHR_J,             ITM_NULL,        CHR_ETA,       ITM_REGJ     },
 {35,   KEY_f,            ITM_NULL,        ITM_NULL,     KEY_f,          KEY_f,             ITM_NULL,        ITM_NULL,      KEY_f        },
 {36,   KEY_g,            ITM_NULL,        ITM_NULL,     KEY_g,          KEY_g,             ITM_NULL,        ITM_NULL,      KEY_g        },

 {41,   ITM_ENTER,        ITM_FILL,        ITM_DROP,     ITM_ENTER,      ITM_ENTER,         ITM_NULL,        ITM_NULL,      ITM_ENTER    },
 {42,   ITM_XexY,         ITM_SHOW,       -MNU_STK,      CHR_ex,         CHR_K,             CHR_ex,          CHR_KAPPA,     ITM_REGK     },
 {43,   ITM_CHS,          ITM_DELTAPC,     ITM_PC,       CHR_PLUS_MINUS, CHR_L,             CHR_PLUS_MINUS,  CHR_LAMBDA,    ITM_REGL     },
 {44,   ITM_EXPONENT,    -MNU_FIN,        -MNU_DISP,     ITM_NULL,       CHR_M,             CHR_UP_ARROW,    CHR_MU,        ITM_NULL     },
 {45,   KEY_BACKSPACE,    KEY_UNDO,       -MNU_CLR,      KEY_BACKSPACE,  KEY_BACKSPACE,     KEY_UNDO,       -MNU_CLR,       KEY_BACKSPACE},

 {51,   ITM_DIV,          KEY_USERMODE,   -MNU_MODE,     CHR_SLASH,      ITM_NULL,          KEY_USERMODE,   -MNU_MODE,      ITM_DIV      },
 {52,   CHR_7,            ITM_NULL,        ITM_NULL,     CHR_7,          CHR_N,             CHR_7,           CHR_NU,        CHR_7        },
 {53,   CHR_8,            ITM_NULL,        ITM_NULL,     CHR_8,          CHR_O,             CHR_8,           CHR_OMEGA,     CHR_8        },
 {54,   CHR_9,            ITM_toSP,        ITM_toDP,     CHR_9,          CHR_P,             CHR_9,           CHR_PI,        CHR_9        },
 {55,   ITM_XEQ,          ITM_GTO,         ITM_LBL,      ITM_NULL,       CHR_Q,             ITM_NULL,        ITM_NULL,      ITM_NULL     },

 {61,   ITM_MULT,         ITM_XFACT,      -MNU_PROB,     CHR_CROSS,      CHR_R,             CHR_PROD_SIGN,   CHR_RHO,       ITM_MULT     },
 {62,   CHR_4,           -MNU_STAT,       -MNU_SUMS,     CHR_4,          CHR_S,             CHR_4,           CHR_SIGMA,     CHR_4        },
 {63,   CHR_5,            ITM_toREC,       ITM_toPOL,    CHR_5,          CHR_T,             CHR_5,           CHR_TAU,       CHR_5        },
 {64,   CHR_6,           -MNU_ANGLECONV,  -MNU_UNITCONV, CHR_6,          CHR_U,             CHR_6,           CHR_THETA,     CHR_6        },
 {65,   KEY_UP,           KEY_BST,         ITM_RTN,      KEY_UP,         KEY_UP,            KEY_BST,         ITM_NULL,      KEY_UP       },

 {71,   ITM_SUB,         -MNU_BITS,       -MNU_INTS,     CHR_MINUS,      CHR_V,             CHR_MINUS,      -MNU_ALPHAMATH, ITM_SUB      },
 {72,   CHR_1,           -MNU_ADV,        -MNU_EQN,      CHR_1,          CHR_W,             CHR_1,           CHR_PSI,       CHR_1        },
 {73,   CHR_2,           -MNU_MATX,       -MNU_XFN,      CHR_2,          CHR_X,             CHR_2,           CHR_XI,        CHR_2        },
 {74,   CHR_3,            ITM_TIMER,      -MNU_CLK,      CHR_3,          CHR_Y,             CHR_3,           CHR_UPSILON,   CHR_3        },
 {75,   KEY_DOWN,         KEY_SST,        -MNU_FLAGS,    KEY_DOWN,       KEY_DOWN,          KEY_SST,        -MNU_FLAGS,     KEY_DOWN     },

 {81,   ITM_ADD,          KEY_PRTX,       -MNU_IO,       CHR_PLUS,       CHR_QUESTION_MARK, CHR_PLUS,       -MNU_ALPHAINTL, ITM_ADD      },
 {82,   CHR_0,           -MNU_LOOP,       -MNU_TEST,     CHR_0,          CHR_Z,             CHR_0,           CHR_ZETA,      CHR_0        },
 {83,   CHR_PERIOD,      -MNU_PARTS,      -MNU_INFO,     CHR_PERIOD,     CHR_COMMA,         CHR_PERIOD,     -MNU_ALPHADOT,  CHR_PERIOD   },
 {84,   ITM_RS,           ITM_PR,         -MNU_PFN,      ITM_NULL,       CHR_SPACE,         ITM_NULL,        ITM_NULL,      ITM_NULL     },
 {85,   KEY_EXIT,        -MNU_CATALOG,     ITM_OFF,      KEY_EXIT,       KEY_EXIT,         -MNU_CATALOG,     ITM_OFF,       KEY_EXIT     }
//keyId primary           fShifted         gShifted      keyLblAim       primaryAim         fShiftedAim      gShiftedAim    primaryTam
};



void fnUserMode(uint16_t unusedParamButMandatory) {
  toggleUserMode();
}
