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

/*
// JM Latest WP43S, for reference only, not used.
//keyId primary        fShifted         gShifted         keyLblAim        primaryAim         fShiftedAim      gShiftedAim    primaryTam
 {21,   ITM_1ONX,      ITM_TGLFRT,     -MNU_ALPHAFN,     ITM_NULL,        CHR_A,            -MNU_ALPHAINTL,   CHR_ALPHA,     ITM_ST_A     },
 {22,   ITM_YX,        ITM_toINT,      -MNU_EXP,         CHR_NUMBER_SIGN, CHR_B,             CHR_NUMBER_SIGN, CHR_BETA,      ITM_ST_B     },
 {23,  -MNU_TRI,       ITM_DMS,         ITM_pi,          ITM_NULL,        CHR_C,             ITM_NULL,        CHR_GAMMA,     ITM_ST_C     },
 {24,   ITM_LN,        KEY_dotD,        ITM_LOG10,       ITM_NULL,        CHR_D,             ITM_NULL,        CHR_DELTA,     ITM_ST_D     }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case D for decimal base
 {25,   ITM_EX,        ITM_toHMS,       ITM_10x,         ITM_NULL,        CHR_E,             ITM_NULL,        CHR_EPSILON,   ITM_NULL     },
 {26,   ITM_SQUARE,    ITM_AIM,         ITM_SQUAREROOTX, CHR_CHECK_MARK,  CHR_F,             CHR_CHECK_MARK,  CHR_PHI,       CHR_alpha    },

 {31,   ITM_STO,       ITM_ASSIGN,      ITM_SAVE,        ITM_NULL,        CHR_G,             ITM_ASSIGN,      CHR_GAMMA,     ITM_NULL     },
 {32,   ITM_RCL,       ITM_RBR,         ITM_VIEW,        ITM_NULL,        CHR_H,             ITM_RBR,         CHR_CHI,       ITM_HEX      }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case H for hexadecimal base
 {33,   ITM_Rdown,     ITM_Rup,        -MNU_CPX,         ITM_NULL,        CHR_I,             CHR_DOWN_ARROW,  CHR_IOTA,      ITM_REGI     },
 {34,   KEY_CC,        ITM_MAGNITUDE,   ITM_ANGLE,       ITM_NULL,        CHR_J,             ITM_NULL,        CHR_ETA,       ITM_REGJ     },
 {35,   KEY_f,         ITM_NULL,        ITM_SCRDMP,      KEY_f,           KEY_f,             ITM_NULL,        ITM_SCRDMP,    KEY_f        },
 {36,   KEY_g,         KEY_USERMODE,    ITM_NULL,        KEY_g,           KEY_g,             KEY_USERMODE,    ITM_NULL,      KEY_g        },

 {41,   ITM_ENTER,     ITM_STATUS,      ITM_DROP,        ITM_ENTER,       ITM_ENTER,         ITM_STATUS,      ITM_DROP,      ITM_ENTER    },
 {42,   ITM_XexY,      ITM_FILL,       -MNU_STK,         CHR_ex,          CHR_K,             CHR_ex,          CHR_KAPPA,     ITM_REGK     },
 {43,   ITM_CHS,       ITM_DELTAPC,    -MNU_FIN,         CHR_PLUS_MINUS,  CHR_L,             CHR_PLUS_MINUS,  CHR_LAMBDA,    ITM_REGL     },
 {44,   ITM_EXPONENT,  ITM_SHOW,       -MNU_DSP,         ITM_NULL,        CHR_M,             CHR_UP_ARROW,    CHR_MU,        ITM_NULL     },
 {45,   KEY_BACKSPACE, KEY_UNDO,       -MNU_CLR,         KEY_BACKSPACE,   KEY_BACKSPACE,     KEY_UNDO,       -MNU_CLR,       KEY_BACKSPACE},

 {51,   ITM_DIV,       ITM_RMD,         ITM_MOD,         CHR_SLASH,       CHR_N,             CHR_SLASH,       CHR_NU,        ITM_DIV      },
 {52,   CHR_7,        -MNU_CONST,       ITM_NULL,        CHR_7,           CHR_O,             CHR_7,           CHR_OMEGA,     CHR_7        },
 {53,   CHR_8,         ITM_NULL,        ITM_NULL,        CHR_8,           CHR_P,             CHR_8,           CHR_PI,        CHR_8        },
 {54,   CHR_9,         ITM_NULL,        ITM_RTN,         CHR_9,           CHR_Q,             CHR_9,           ITM_RTN,       CHR_9        },
 {55,   ITM_XEQ,       ITM_GTO,         ITM_LBL,         ITM_NULL,        ITM_NULL,          ITM_NULL,        ITM_NULL,      ITM_NULL     },

 {61,   ITM_MULT,      ITM_XFACT,      -MNU_PROB,        CHR_CROSS,       CHR_R,             CHR_PROD_SIGN,   CHR_RHO,       ITM_MULT     },
 {62,   CHR_4,        -MNU_STAT,       -MNU_SUMS,        CHR_4,           CHR_S,             CHR_4,           CHR_SIGMA,     CHR_4        },
 {63,   CHR_5,         ITM_toREC,       ITM_toPOL,       CHR_5,           CHR_T,             CHR_5,           CHR_TAU,       CHR_5        },
 {64,   CHR_6,        -MNU_UNITCONV,   -MNU_ANGLECONV,   CHR_6,           CHR_U,             CHR_6,           CHR_THETA,     CHR_6        },
 {65,   KEY_UP,        KEY_BST,        -MNU_FLAGS,       KEY_UP,          KEY_UP,            KEY_BST,        -MNU_FLAGS,     KEY_UP       },

 {71,   ITM_SUB,      -MNU_BITS,       -MNU_INTS,        CHR_MINUS,       CHR_V,             CHR_MINUS,      -MNU_ALPHAMATH, ITM_SUB      },
 {72,   CHR_1,        -MNU_ADV,        -MNU_EQN,         CHR_1,           CHR_W,             CHR_1,           CHR_PSI,       CHR_1        },
 {73,   CHR_2,        -MNU_MATX,       -MNU_XFN,         CHR_2,           CHR_X,             CHR_2,           CHR_XI,        CHR_2        },
 {74,   CHR_3,         ITM_TIMER,      -MNU_CLK,         CHR_3,           CHR_Y,             CHR_3,           CHR_UPSILON,   CHR_3        },
 {75,   KEY_DOWN,      KEY_SST,        -MNU_MODE,        KEY_DOWN,        KEY_DOWN,          KEY_SST,        -MNU_MODE,      KEY_DOWN     },

 {81,   ITM_ADD,      -MNU_IO,         -MNU_PRINT,       CHR_PLUS,        CHR_Z,             CHR_PLUS,        CHR_ZETA,      ITM_ADD      },
 {82,   CHR_0,        -MNU_LOOP,       -MNU_TEST,        CHR_0,           CHR_QUESTION_MARK, CHR_0,           CHR_PRINTER,   CHR_0        },
 {83,   CHR_PERIOD,   -MNU_PARTS,      -MNU_INFO,        CHR_PERIOD,      CHR_COMMA,         CHR_PERIOD,     -MNU_ALPHADOT,  CHR_PERIOD   },
 {84,   ITM_RS,        ITM_PR,         -MNU_PFN,         ITM_NULL,        CHR_SPACE,         ITM_NULL,        ITM_NULL,      ITM_NULL     },
 {85,   KEY_EXIT,     -MNU_CATALOG,     ITM_OFF,         KEY_EXIT,        KEY_EXIT,         -MNU_CATALOG,     ITM_OFF,       KEY_EXIT     }
*/


 #ifdef JM_LAYOUT_1A  //JM UPDATED TO LAYOUT 1C. OPTIMAL. SIMULATOR.
//keyId primary           fShifted         gShifted      keyLblAim       primaryAim         fShiftedAim      gShiftedAim    primaryTam
 {21,   ITM_SIGMAPLUS,    ITM_RI/*ITM_RND*/,ITM_TGLFRT,  ITM_NULL,       CHR_A,            -MNU_ALPHAINTL,   CHR_ALPHA,     ITM_ST_A     }, //JM
 {22,   ITM_1ONX,         ITM_YX,          ITM_toINT,    CHR_NUMBER_SIGN,CHR_B,             CHR_NUMBER_SIGN, CHR_BETA,      ITM_ST_B     }, //JM
 {23,   ITM_SQUAREROOTX,  ITM_SQUARE,      ITM_ms,       CHR_CHECK_MARK, CHR_C,             CHR_CHECK_MARK,  CHR_CHI,       ITM_ST_C     }, //JM
 {24,   ITM_LOG10,        ITM_10x,         KEY_dotD,     ITM_NULL,       CHR_D,             ITM_NULL,        CHR_DELTA,     ITM_ST_D     }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case D for decimal base; //JM2 changed top line primary to DM42 keyboard
 {25,   ITM_LN,           ITM_EX,          ITM_toREC2,   ITM_NULL,       CHR_E,             ITM_NULL,        CHR_EPSILON,   ITM_NULL     }, //JM
 {26,   ITM_XEQ,          ITM_XTHROOT,     ITM_toPOL2,   ITM_NULL,       CHR_F,             ITM_NULL,        CHR_DIGAMMA,   CHR_alpha    }, //JM

 {31,   ITM_STO,          ITM_MAGNITUDE,   ITM_ANGLE,    ITM_NULL,       CHR_G,             ITM_NULL,        CHR_GAMMA,     ITM_NULL     }, //JM
 {32,   ITM_RCL,          ITM_PC,          ITM_DELTAPC,  ITM_NULL,       CHR_H,             ITM_NULL,        CHR_ETA,       ITM_HEX      }, //JM
 {33,   ITM_Rdown,        ITM_pi,          ITM_Rup,      ITM_NULL,       CHR_I,             CHR_DOWN_ARROW,  CHR_IOTA,      ITM_REGI     }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case H for hexadecimal base
 {34,   ITM_sin,          ITM_arcsin,      ITM_GTO,      ITM_NULL,       CHR_J,             CHR_case,        CHR_THETA,     ITM_REGJ     }, //JM
 {35,   ITM_cos,          ITM_arccos,      ITM_LBL,      ITM_NULL,       CHR_K,             ITM_NULL,        CHR_KAPPA,     ITM_REGK     }, //JM
 {36,   ITM_tan,          ITM_arctan,      ITM_RTN,      ITM_NULL,       CHR_L,             ITM_NULL,        CHR_LAMBDA,    ITM_REGL     }, //JM
       
 {41,   ITM_ENTER,        KEY_COMPLEX,    -MNU_CPX,      ITM_ENTER,      ITM_ENTER,         ITM_NULL,        ITM_NULL,      ITM_ENTER    }, //JM
 {42,   ITM_XexY,         ITM_LASTX,      -MNU_EXP,      CHR_ex,         CHR_M,             CHR_ex,          CHR_MU,        ITM_NULL     }, //JM
 {43,   ITM_CHS,         -MNU_MODE,       -MNU_BASE,     CHR_PLUS_MINUS, CHR_N,             CHR_PLUS_MINUS,  CHR_NU,        ITM_NULL     }, //JM
 {44,   ITM_EXPONENT,    -MNU_DSP,        -MNU_FIN,      ITM_NULL,       CHR_O,             CHR_UP_ARROW,    CHR_OMICRON,   ITM_NULL     }, //JM
 {45,   KEY_BACKSPACE,    KEY_UNDO,       -MNU_CLR,      KEY_BACKSPACE,  KEY_BACKSPACE,     KEY_UNDO,       -MNU_CLR,       KEY_BACKSPACE}, //JM
                  
 {51,   KEY_UP1,          KEY_BST,         ITM_RBR,      KEY_UP1,        KEY_UP1,           KEY_BST,        -MNU_ALPHA,     KEY_UP1      }, //JM
 {52,   CHR_7,           -MNU_EQN,        -MNU_HOME,     CHR_7,          CHR_P,             CHR_7,           CHR_PI,        CHR_7        }, //JM
 {53,   CHR_8,           -MNU_ADV,        -MNU_CONST,    CHR_8,          CHR_Q,             CHR_8,           CHR_QOPPA,     CHR_8        }, //JM
 {54,   CHR_9,           -MNU_MATX,       -MNU_XFN,      CHR_9,          CHR_R,             CHR_9,           CHR_RHO,       CHR_9        }, //JM
 {55,   ITM_DIV,         -MNU_STAT,       -MNU_SUMS,     CHR_DIVIDE,     CHR_S,             CHR_DIVIDE,      CHR_SIGMA,     ITM_DIV      }, //JM

 {61,   KEY_DOWN1,        KEY_SST,         ITM_FLGSV,    KEY_DOWN1,      KEY_DOWN1,         KEY_SST,         CHR_case,      KEY_DOWN1    }, //JM
 {62,   CHR_4,            ITM_AIM,        -MNU_CLK,      CHR_4,          CHR_T,             CHR_4,           CHR_TAU,       CHR_4        }, //JM
 {63,   CHR_5,           -MNU_ANGLECONV,  -MNU_UNITCONV, CHR_5,          CHR_U,             CHR_5,           CHR_PHI,       CHR_5        }, //JM
 {64,   CHR_6,           -MNU_FLAGS,      -MNU_BITS,     CHR_6,          CHR_V,             CHR_6,           CHR_PSI,       CHR_6        }, //JM
 {65,   ITM_MULT,        -MNU_PROB,       -MNU_INTS,     CHR_CROSS,      CHR_W,             CHR_CROSS,       CHR_OMEGA,     ITM_MULT     }, //JM

 {71,   KEY_fg,           ITM_NULL,        ITM_NULL,     KEY_fg,         KEY_fg,            ITM_NULL,        ITM_NULL,      KEY_fg        }, //JM
 {72,   CHR_1,            ITM_ASSIGN,     -MNU_ASN,      CHR_1,          CHR_X,             CHR_1,           CHR_XI,        CHR_1        }, //JM
 {73,   CHR_2,            KEY_USERMODE,   -MNU_LOOP,     CHR_2,          CHR_Y,             CHR_2,           CHR_UPSILON,   CHR_2        }, //JM
 {74,   CHR_3,           -MNU_PARTS,      -MNU_TEST,     CHR_3,          CHR_Z,             CHR_3,           CHR_ZETA,      CHR_3        }, //JM
 {75,   ITM_SUB,         -MNU_STK,        -MNU_ALPHAFN,  CHR_MINUS,      CHR_UNDERSCORE,    CHR_MINUS,       CHR_SAMPI,     ITM_SUB      }, //JM

 {81,   KEY_EXIT1,        ITM_OFF,        -MNU_PRINT,    KEY_EXIT1,      KEY_EXIT1,         ITM_OFF,         CHR_PRINTER,   KEY_EXIT1    }, //JM
 {82,   CHR_0,            ITM_VIEW,       ITM_TIMER,     CHR_0,          CHR_COLON,         CHR_0,          -MNU_ALPHA,     CHR_0        }, //JM
 {83,   CHR_PERIOD,       ITM_SHOW,       -MNU_INFO,     CHR_PERIOD,     CHR_COMMA,         CHR_PERIOD,     -MNU_ALPHADOT,  CHR_PERIOD   }, //JM
 {84,   ITM_RS,           ITM_PR,         -MNU_PFN,      ITM_NULL,       CHR_QUESTION_MARK, CHR_SLASH,      -MNU_ALPHAMATH, ITM_NULL     }, //JM
 {85,   ITM_ADD,         -MNU_CATALOG,    -MNU_IO,       CHR_PLUS,       CHR_SPACE,         CHR_PLUS,       -MNU_ALPHAINTL, ITM_ADD      }  //JM
//keyId primary           fShifted         gShifted      keyLblAim       primaryAim         fShiftedAim      gShiftedAim    primaryTam
#endif //JM END OF LAYOUT 1a.


#ifdef JM_LAYOUT_2_DM42_STRICT //JM DM42 STRICT. NAME CHANGE FROM LAYOUT 2 TO LAYOUT 42C
//keyId primary           fShifted         gShifted      keyLblAim       primaryAim         fShiftedAim      gShiftedAim    primaryTam
 {21,   ITM_SIGMAPLUS,    ITM_SIGMAMINUS,  ITM_TGLFRT,    ITM_NULL,       CHR_A,           -MNU_ALPHAINTL,   CHR_ALPHA,     ITM_ST_A     }, //JM
 {22,   ITM_1ONX,         ITM_YX,          ITM_toINT,     CHR_NUMBER_SIGN,CHR_B,            CHR_NUMBER_SIGN, CHR_BETA,      ITM_ST_B     }, //JM
 {23,   ITM_SQUAREROOTX,  ITM_SQUARE,      ITM_ms,        CHR_CHECK_MARK, CHR_C,            CHR_CHECK_MARK,  CHR_CHI,       ITM_ST_C     }, //JM
 {24,   ITM_LOG10,        ITM_10x,         KEY_dotD,      ITM_NULL,       CHR_D,            ITM_NULL,        CHR_DELTA,     ITM_ST_D     }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case D for decimal base; //JM2 changed top line primary to DM42 keyboard
 {25,   ITM_LN,           ITM_EX,          ITM_toREC2,    ITM_NULL,       CHR_E,            ITM_NULL,        CHR_EPSILON,   ITM_NULL     }, //JM
 {26,   ITM_XEQ,          ITM_GTO,         ITM_toPOL2,    ITM_NULL,       CHR_F,            ITM_NULL,        CHR_DIGAMMA,   CHR_alpha    }, //JM
 
 {31,   ITM_STO,          KEY_COMPLEX,     ITM_MAGNITUDE, ITM_NULL,       CHR_G,            ITM_NULL,        CHR_GAMMA,     ITM_NULL     }, //JM
 {32,   ITM_RCL,          ITM_PC,          ITM_DELTAPC,   ITM_NULL,       CHR_H,            ITM_NULL,        CHR_ETA,       ITM_HEX      }, //JM
 {33,   ITM_Rdown,        ITM_pi,          ITM_ANGLE,     ITM_NULL,       CHR_I,            CHR_DOWN_ARROW,  CHR_IOTA,      ITM_REGI     }, // if f or g are changed: adapt the function btnClicked section if(calcMode == CM_NIM) in keyboard.c. Case H for hexadecimal base
 {34,   ITM_sin,          ITM_arcsin,      ITM_XTHROOT,   ITM_NULL,       CHR_J,            CHR_case,        CHR_THETA,     ITM_REGJ     }, //JM
 {35,   ITM_cos,          ITM_arccos,      ITM_LBL,       ITM_NULL,       CHR_K,            ITM_NULL,        CHR_KAPPA,     ITM_REGK     }, //JM
 {36,   ITM_tan,          ITM_arctan,      ITM_RTN,       ITM_NULL,       CHR_L,            ITM_NULL,        CHR_LAMBDA,    ITM_REGL     }, //JM
       
 {41,   ITM_ENTER,        ITM_AIM,        -MNU_CPX,      ITM_ENTER,      ITM_ENTER,        ITM_NULL,        ITM_NULL,      ITM_ENTER    }, //JM
 {42,   ITM_XexY,         ITM_LASTX,      -MNU_EXP,      CHR_ex,         CHR_M,            CHR_ex,          CHR_MU,        ITM_NULL     }, //JM
 {43,   ITM_CHS,         -MNU_MODE,        KEY_CC,       CHR_PLUS_MINUS, CHR_N,            CHR_PLUS_MINUS,  CHR_NU,        ITM_NULL     }, //JM
 {44,   ITM_EXPONENT,    -MNU_DSP,        -MNU_FIN,      ITM_NULL,       CHR_O,            CHR_UP_ARROW,    CHR_OMICRON,   ITM_NULL     }, //JM
 {45,   KEY_BACKSPACE,   -MNU_CLR,         KEY_UNDO,     KEY_BACKSPACE,  KEY_BACKSPACE,   -MNU_CLR,         KEY_UNDO,      KEY_BACKSPACE}, //JM
                 
 {51,   KEY_UP1,          KEY_BST,         ITM_RBR,      KEY_UP1,        KEY_UP1,          KEY_BST,        -MNU_ALPHA,     KEY_UP1      }, //JM
 {52,   CHR_7,           -MNU_EQN,        -MNU_INFO,     CHR_7,          CHR_P,            CHR_7,           CHR_PI,        CHR_7        }, //JM
 {53,   CHR_8,           -MNU_ADV,        -MNU_CONST,    CHR_8,          CHR_Q,            CHR_8,           CHR_QOPPA,     CHR_8        }, //JM
 {54,   CHR_9,           -MNU_MATX,       -MNU_XFN,      CHR_9,          CHR_R,            CHR_9,           CHR_RHO,       CHR_9        }, //JM
 {55,   ITM_DIV,         -MNU_STAT,       -MNU_SUMS,     CHR_DIVIDE,     CHR_S,            CHR_DIVIDE,      CHR_SIGMA,     ITM_DIV      }, //JM

 {61,   KEY_DOWN1,        KEY_SST,         ITM_FLGSV,    KEY_DOWN1,      KEY_DOWN1,        KEY_SST,         CHR_case,      KEY_DOWN1    }, //JM
 {62,   CHR_4,           -MNU_BASE,       -MNU_CLK,      CHR_4,          CHR_T,            CHR_4,           CHR_TAU,       CHR_4        }, //JM
 {63,   CHR_5,           -MNU_ANGLECONV,  -MNU_UNITCONV, CHR_5,          CHR_U,            CHR_5,           CHR_PHI,       CHR_5        }, //JM
 {64,   CHR_6,           -MNU_FLAGS,      -MNU_BITS,     CHR_6,          CHR_V,            CHR_6,           CHR_PSI,       CHR_6        }, //JM
 {65,   ITM_MULT,        -MNU_PROB,       -MNU_INTS,     CHR_CROSS,      CHR_W,            CHR_CROSS,       CHR_OMEGA,     ITM_MULT     }, //JM

 {71,   KEY_fg,           ITM_NULL,        ITM_NULL,     KEY_fg,         KEY_fg,           ITM_NULL,        ITM_NULL,      KEY_fg       }, //JM
 {72,   CHR_1,            ITM_ASSIGN,     -MNU_ASN,      CHR_1,          CHR_X,            CHR_1,           CHR_XI,        CHR_1        }, //JM
 {73,   CHR_2,            KEY_USERMODE,   -MNU_LOOP,     CHR_2,          CHR_Y,            CHR_2,           CHR_UPSILON,   CHR_2        }, //JM
 {74,   CHR_3,           -MNU_PFN,        -MNU_TEST,     CHR_3,          CHR_Z,            CHR_3,           CHR_ZETA,      CHR_3        }, //JM
 {75,   ITM_SUB,         -MNU_IO,         -MNU_ALPHAFN,  CHR_MINUS,      CHR_UNDERSCORE,   CHR_MINUS,       CHR_SAMPI,     ITM_SUB      }, //JM

 {81,   KEY_EXIT1,        ITM_OFF,        -MNU_PRINT,    KEY_EXIT1,      KEY_EXIT1,        ITM_OFF,         CHR_PRINTER,   KEY_EXIT1    }, //JM
 {82,   CHR_0,           -MNU_HOME,        ITM_TIMER,    CHR_0,          CHR_COLON,        CHR_0,          -MNU_ALPHA,     CHR_0        }, //JM
 {83,   CHR_PERIOD,       ITM_SHOW,        ITM_VIEW,     CHR_PERIOD,     CHR_COMMA,        CHR_PERIOD,     -MNU_ALPHADOT,  CHR_PERIOD   }, //JM
 {84,   ITM_RS,           ITM_PR,         -MNU_STK,      ITM_NULL,       CHR_QUESTION_MARK,CHR_SLASH,      -MNU_ALPHAMATH, ITM_NULL     }, //JM
 {85,   ITM_ADD,         -MNU_CATALOG,    -MNU_PARTS,    CHR_PLUS,       CHR_SPACE,        CHR_PLUS,       -MNU_ALPHAINTL, ITM_ADD      }  //JM
//keyId primary           fShifted         gShifted      keyLblAim       primaryAim         fShiftedAim      gShiftedAim    primaryTam
#endif //JM END OF LAYOUT 2 DM42 STRICT.


};
