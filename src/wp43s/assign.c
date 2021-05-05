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
//keyId primary        fShifted         gShifted         keyLblAim        primaryAim         fShiftedAim      gShiftedAim    primaryTam
 {21,   ITM_1ONX,      ITM_TGLFRT,     -MNU_ALPHAFN,     ITM_NULL,        ITM_A,            -MNU_ALPHAINTL,   ITM_ALPHA,     ITM_REG_A    },
 {22,   ITM_YX,        ITM_toINT,      -MNU_EXP,         ITM_NUMBER_SIGN, ITM_B,             ITM_NUMBER_SIGN, ITM_BETA,      ITM_REG_B    },
 {23,  -MNU_TRI,       ITM_DMS,         ITM_CONSTpi,     ITM_NULL,        ITM_C,             ITM_NULL,        ITM_GAMMA,     ITM_REG_C    },
 {24,   ITM_LN,        ITM_dotD,        ITM_LOG10,       ITM_NULL,        ITM_D,             ITM_NULL,        ITM_DELTA,     ITM_REG_D    }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case D for decimal base
 {25,   ITM_EXP,       ITM_toHMS,       ITM_10x,         ITM_NULL,        ITM_E,             ITM_NULL,        ITM_EPSILON,   ITM_NULL     },
 {26,   ITM_SQUARE,    ITM_AIM,         ITM_SQUAREROOTX, ITM_CHECK_MARK,  ITM_F,             ITM_CHECK_MARK,  ITM_PHI,       ITM_alpha    },

 {31,   ITM_STO,       ITM_ASSIGN,      ITM_SAVE,        ITM_NULL,        ITM_G,             ITM_ASSIGN,      ITM_GAMMA,     ITM_NULL     },
 {32,   ITM_RCL,       ITM_RBR,         ITM_VIEW,        ITM_NULL,        ITM_H,             ITM_RBR,         ITM_CHI,       ITM_HEX      }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case H for hexadecimal base
 {33,   ITM_Rdown,     ITM_Rup,        -MNU_CPX,         ITM_NULL,        ITM_I,             ITM_DOWN_ARROW,  ITM_IOTA,      ITM_REG_I    },
 {34,   ITM_CC,        ITM_MAGNITUDE,   ITM_ANGLE,       ITM_NULL,        ITM_J,             ITM_NULL,        ITM_ETA,       ITM_REG_J    },
 {35,   ITM_SHIFTf,    ITM_NULL,        ITM_SNAP,        ITM_SHIFTf,      ITM_SHIFTf,        ITM_NULL,        ITM_SNAP,      ITM_SHIFTf   },
 {36,   ITM_SHIFTg,    ITM_USERMODE,    ITM_NULL,        ITM_SHIFTg,      ITM_SHIFTg,        ITM_USERMODE,    ITM_NULL,      ITM_SHIFTg   },

 {41,   ITM_ENTER,     ITM_STATUS,      ITM_DROP,        ITM_ENTER,       ITM_ENTER,         ITM_STATUS,      ITM_DROP,      ITM_ENTER    },
 {42,   ITM_XexY,      ITM_FILL,       -MNU_STK,         ITM_ex,          ITM_K,             ITM_ex,          ITM_KAPPA,     ITM_REG_K    },
 {43,   ITM_CHS,       ITM_DELTAPC,    -MNU_FIN,         ITM_PLUS_MINUS,  ITM_L,             ITM_PLUS_MINUS,  ITM_LAMBDA,    ITM_REG_L    },
 {44,   ITM_EXPONENT,  ITM_SHOW,       -MNU_DSP,         ITM_NULL,        ITM_M,             ITM_UP_ARROW,    ITM_MU,        ITM_NULL     },
 {45,   ITM_BACKSPACE, ITM_UNDO,       -MNU_CLR,         ITM_BACKSPACE,   ITM_BACKSPACE,     ITM_UNDO,       -MNU_CLR,       ITM_BACKSPACE},

 {51,   ITM_DIV,       ITM_RMD,         ITM_MOD,         ITM_SLASH,       ITM_N,             ITM_SLASH,       ITM_NU,        ITM_DIV      },
 {52,   ITM_7,        -MNU_CONST,       ITM_NULL,        ITM_7,           ITM_O,             ITM_7,           ITM_OMEGA,     ITM_7        },
 {53,   ITM_8,         ITM_NULL,        ITM_NULL,        ITM_8,           ITM_P,             ITM_8,           ITM_PI,        ITM_8        },
 {54,   ITM_9,         ITM_NULL,        ITM_RTN,         ITM_9,           ITM_Q,             ITM_9,           ITM_RTN,       ITM_9        },
 {55,   ITM_XEQ,       ITM_GTO,         ITM_LBL,         ITM_NULL,        ITM_NULL,          ITM_NULL,        ITM_NULL,      ITM_NULL     },

 {61,   ITM_MULT,      ITM_XFACT,      -MNU_PROB,        ITM_CROSS,       ITM_R,             ITM_PROD_SIGN,   ITM_RHO,       ITM_MULT     },
 {62,   ITM_4,        -MNU_STAT,       -MNU_SUMS,        ITM_4,           ITM_S,             ITM_4,           ITM_SIGMA,     ITM_4        },
 {63,   ITM_5,         ITM_toREC,       ITM_toPOL,       ITM_5,           ITM_T,             ITM_5,           ITM_TAU,       ITM_5        },
 {64,   ITM_6,        -MNU_UNITCONV,   -MNU_ANGLECONV,   ITM_6,           ITM_U,             ITM_6,           ITM_THETA,     ITM_6        },
 {65,   ITM_UP,        ITM_BST,        -MNU_FLAGS,       ITM_UP,          ITM_UP,            ITM_BST,        -MNU_FLAGS,     ITM_UP       },

 {71,   ITM_SUB,      -MNU_BITS,       -MNU_INTS,        ITM_MINUS,       ITM_V,             ITM_MINUS,      -MNU_ALPHAMATH, ITM_SUB      },
 {72,   ITM_1,        -MNU_ADV,        -MNU_EQN,         ITM_1,           ITM_W,             ITM_1,           ITM_PSI,       ITM_1        },
 {73,   ITM_2,        -MNU_MATX,       -MNU_XFN,         ITM_2,           ITM_X,             ITM_2,           ITM_XI,        ITM_2        },
 {74,   ITM_3,         ITM_TIMER,      -MNU_CLK,         ITM_3,           ITM_Y,             ITM_3,           ITM_UPSILON,   ITM_3        },
 {75,   ITM_DOWN,      ITM_SST,        -MNU_MODE,        ITM_DOWN,        ITM_DOWN,          ITM_SST,        -MNU_MODE,      ITM_DOWN     },

 {81,   ITM_ADD,      -MNU_IO,         -MNU_PRINT,       ITM_PLUS,        ITM_Z,             ITM_PLUS,        ITM_ZETA,      ITM_ADD      },
 {82,   ITM_0,        -MNU_LOOP,       -MNU_TEST,        ITM_0,           ITM_QUESTION_MARK, ITM_0,           ITM_PRINTER,   ITM_0        },
 {83,   ITM_PERIOD,   -MNU_PARTS,      -MNU_INFO,        ITM_PERIOD,      ITM_COMMA,         ITM_PERIOD,     -MNU_ALPHADOT,  ITM_PERIOD   },
 {84,   ITM_RS,        ITM_PR,         -MNU_PFN,         ITM_NULL,        ITM_SPACE,         ITM_NULL,        ITM_NULL,      ITM_NULL     },
 {85,   ITM_EXIT,     -MNU_CATALOG,     ITM_OFF,         ITM_EXIT,        ITM_EXIT,         -MNU_CATALOG,     ITM_OFF,       ITM_EXIT     }
//keyId primary        fShifted         gShifted         keyLblAim        primaryAim         fShiftedAim      gShiftedAim    primaryTam
};
