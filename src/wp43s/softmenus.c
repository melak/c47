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
 * \file softmenus.c List of softmenus and related functions.
 * The numbers refer to the index of items in items.c
 * *         item <     0  ==>  sub menu
 * *     0 < item <  9999  ==>  item with top and bottom line
 * * 10000 < item < 19999  ==>  item without top line
 * * 20000 < item < 29999  ==>  item without bottom line
 * * 30000 < item < 39999  ==>  item without top and bottom line
 ***********************************************/

#include "wp43s.h"

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_MyAlpha[]     = { ITM_MA11,                      ITM_MA12,                   ITM_MA13,                 ITM_MA14,              ITM_MA15,                    ITM_MA16,
                                     ITM_MA21,                      ITM_MA22,                   ITM_MA23,                 ITM_MA24,              ITM_MA25,                    ITM_MA26,
                                     ITM_MA31,                      ITM_MA32,                   ITM_MA33,                 ITM_MA34,              ITM_MA35,                    ITM_MA36                      };

const int16_t menu_ADV[]         = { ITM_SOLVE,                     ITM_SLVQ,                   ITM_FQX,                  ITM_PI,                ITM_SIGMA,                   -MNU_Sfdx,
                                     ITM_PGMSLV,                    ITM_NULL,                   ITM_FDQX,                 ITM_NULL,              ITM_NULL,                    ITM_PGMINT                    };

const int16_t menu_Sfdx[]        = { ITM_NULL,                      ITM_NULL,                   ITM_ACC,                  ITM_DLIM,              ITM_ULIM,                    ITM_INTEGRAL                  };

const int16_t menu_BITS[]        = { ITM_AND,                       ITM_OR,                     ITM_XOR,                  ITM_NOT,               ITM_MASKL,                   ITM_MASKR,
                                     ITM_NAND,                      ITM_NOR,                    ITM_XNOR,                 ITM_MIRROR,            ITM_NULL,                    ITM_ASR,
                                     ITM_SB,                        ITM_BS,                     ITM_NUMB,                 ITM_FB,                ITM_BC,                      ITM_CB,
                                     ITM_SL,                        ITM_RL,                     ITM_RLC,                  ITM_RRC,               ITM_RR,                      ITM_SR,
                                     ITM_LJ,                        ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_RJ,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                     ITM_1COMPL,                    ITM_2COMPL,                 ITM_UNSIGN,               ITM_SIGNMT,            ITM_NULL,                    ITM_WSIZE                     };

const int16_t menu_CLK[]         = { ITM_DATE,                      ITM_toDATE,                 ITM_DATEto,               ITM_WDAY,              ITM_TIME,                    ITM_XtoDATE,
                                     ITM_JtoD,                      ITM_DtoJ,                   ITM_NULL,                 ITM_DAY,               ITM_MONTH,                   ITM_YEAR,
                                     ITM_YEARtoS,                   ITM_StoYEAR,                ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                     ITM_SETTIM,                    ITM_TDISP,                  ITM_SETDAT,               ITM_DMY,               ITM_YMD,                     ITM_MDY,
                                     ITM_CLK12,                     ITM_CLK24,                  ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_JonG                      };

const int16_t menu_CLR[]         = { ITM_CLSIGMA,                   ITM_CLP,                    ITM_CF,                   ITM_CLMENU,            ITM_CLSTK,                   ITM_CLX,
                                     ITM_CLREGS,                    ITM_CLPALL,                 ITM_CLFALL,               ITM_CLCVAR,            ITM_CLLCD,                   ITM_CLALL,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_RESET                     };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_CPX[]         = { ITM_RE,                        ITM_IM,                     ITM_MAGNITUDE,            ITM_ANGLE,             ITM_CONJ,                    ITM_REexIM,                         //JM re-arranged menu. CPX menu
                                     KEY_COMPLEX,                   KEY_CC,                     ITM_DOT,                  ITM_CROSS,             ITM_SIGN,                    ITM_UNITV,                                          //JM re-arranged menu. CPX menu
                                     ITM_CPXI,                      ITM_CPXJ,                   ITM_CXtoRE,               ITM_REtoCX,            ITM_RECT,                    ITM_POLAR                     };    //JM re-arranged menu
                        /*KEY_CC1 removed this one*/

const int16_t menu_DISP[]        = { ITM_FIX,                       ITM_SCI,                    ITM_ENG,                  ITM_ALL,               ITM_SIGFIG,                  ITM_UNIT,
                                     ITM_ROUND,                     ITM_ROUNDI,                 ITM_RDP,                  ITM_RSD,               ITM_NULL,                    -MNU_CFG,                           //JM added CFG
                                     ITM_DSTACK,                    ITM_GAP,                    ITM_RANGE,                ITM_GETRANGE,              ITM_RECT,                    ITM_POLAR                     };    //JM added SIGFIG & UNIT

const int16_t menu_EQN[]         = { ITM_EQ_EDI,                    ITM_EQ_NEW,                 -MNU_2NDDERIV,            -MNU_1STDERIV,         -MNU_Sf,                     -MNU_Solver,
                                     ITM_EQ_DEL,                    ITM_NULL,                   ITM_NULL,                 ITM_NULL,              -MNU_GRAPH,                  ITM_NULL                      };

const int16_t menu_EXP[]         = { ITM_CUBE,                      ITM_YX,                     ITM_SQUARE,               ITM_LOG2,              ITM_EX,                      ITM_LOGXY,                          //JM re-arranged menu. logxy and square to follow DM42 keyboard. Re-aligned with 42S keys.
                                     ITM_CUBEROOT,                  ITM_XTHROOT,                ITM_SQUAREROOTX,          ITM_2X,                ITM_EX1,                     ITM_LN1X,                           //JM re-arranged menu. Added YˆX to follow DM42 keyboard. Swapped YˆX and Yˆ(1/X). Re-aligned with 42S keys.
                                     ITM_sinh,                      ITM_cosh,                   ITM_tanh,                 ITM_arsinh,            ITM_arcosh,                  ITM_artanh                    };

const int16_t menu_TRI[]         = { ITM_sin,                       ITM_cos,                    ITM_tan,                  ITM_arcsin,            ITM_arccos,                  ITM_arctan,                         //JM re-arranged menu TRIG menu
                                     ITM_DEG,                       ITM_RAD,                    ITM_GRAD,                 ITM_DMS,               ITM_toREC2,                  ITM_toPOL2,                          //JM re-arranged menu TRIG menu
                                     ITM_sinh,                      ITM_cosh,                   ITM_tanh,                 ITM_arsinh,            ITM_arcosh,                  ITM_artanh                    };    //JM re-arranged menu TRIG menu

const int16_t menu_FIN[]         = { ITM_PC,                        ITM_PCMRR,                  ITM_PCT,                  ITM_PCSIGMA,           ITM_PCPMG,                   -MNU_TVM,                     
                                     ITM_DELTAPC,                   ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };    //JM Added D%
/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_TVM[]         = { ITM_NPER,                      ITM_IPCA,                   ITM_PERonA,               ITM_PV,                ITM_PMT,                     ITM_FV,
                                     ITM_BEGINP,                    ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_ENDP                      };

const int16_t menu_FLAGS[]       = { ITM_SF,                        ITM_FS,                     ITM_FF,                   ITM_STATUS,            ITM_FC,                      ITM_CF,
                                     ITM_FSS,                       ITM_FSC,                    ITM_FSF,                  ITM_FCF,               ITM_FCS,                     ITM_FCC,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_CLFALL                    };

const int16_t menu_INFO[]        = { ITM_SSIZE,                     ITM_MEM,                    ITM_RMQ,                  ITM_SMODE,             ITM_WSIZEQ,                  ITM_KTYP,
                                     ITM_LocRQ,                     ITM_FLASH,                  ITM_ULP,                  ITM_NEIGHB,            ITM_SDIGS,                   ITM_BATT,
                                     ITM_WHO,                       ITM_VERS,                   ITM_M_DIMQ,               ITM_PMINFINITY,        ITM_ALPHAPOS,                ITM_ALPHALENG,                 

                                     ITM_GETRANGE,                  ITM_SH_INP_DEF,             ITM_SH_ERPN,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };    //JM INFO
              

const int16_t menu_INTS[]        = { CHR_A,                         CHR_B,                      CHR_C,                    CHR_D,                 CHR_E,                       CHR_F,
                                     ITM_IDIV,                      ITM_RMD,                    ITM_MOD,                  ITM_XMOD,              ITM_FLOOR,                   ITM_LCM,
                                     ITM_DBLSLASH,                  ITM_DBLR,                   ITM_DBLCROSS,             ITM_PMOD,              ITM_CEIL,                    ITM_GCD,
                                     ITM_1COMPL,                    ITM_2COMPL,                 ITM_UNSIGN,               ITM_SIGNMT,            ITM_NULL,                    ITM_WSIZE,
                                     KEY_TYPCON_DN,                 ITM_DOTDEMU,                KEY_TYPCON_UP,            ITM_toSI,              ITM_NULL,                    ITM_NULL                      };    //JM

const int16_t menu_LOOP[]        = { ITM_DSE,                       ITM_DSZ,                    ITM_DSL,                  ITM_ISE,               ITM_ISZ,                     ITM_ISG,
                                     ITM_DEC,                       ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_INC                       };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_MATX[]        = { ITM_M_NEW,                     ITM_M_INV,                  ITM_M_DET,                ITM_M_TRANSP,          -MNU_SIM_EQ,                 -MNU_M_EDIT,
                                     ITM_DOT,                       ITM_CROSS,                  ITM_UNITV,                ITM_M_DIM,             ITM_INDEX,                   -MNU_M_EDITN,
                                     ITM_ENORM,                     ITM_NULL,                   ITM_STOEL,                ITM_RCLEL,             ITM_M_PUT,                   ITM_M_GET,
                                     ITM_IPLUS,                     ITM_IMINUS,                 ITM_STOIJ,                ITM_RCLIJ,             ITM_JMINUS,                  ITM_JPLUS,
                                     ITM_RSUM,                      ITM_RNORM,                  ITM_M_LU,                 ITM_M_DIMQ,            ITM_NULL,                    ITM_M_R,
                                     ITM_EIGVAL,                    ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_EIGVEC                    };

const int16_t menu_M_SIM_Q[]     = { ITM_MATA,                      ITM_MATB,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_MATX                      };

const int16_t menu_M_EDIT[]      = { CHR_LEFT_ARROW,                CHR_UP_ARROW,               ITM_M_OLD,                ITM_M_GOTO,            CHR_DOWN_ARROW,              CHR_RIGHT_ARROW,
                                     ITM_M_INSR,                    ITM_NULL,                   ITM_M_DELR,               ITM_NULL,              ITM_M_WRAP,                  ITM_M_GROW                    };

const int16_t menu_M_EDITN[]     = { CHR_LEFT_ARROW,                CHR_UP_ARROW,               ITM_M_OLD,                ITM_M_GOTO,            CHR_DOWN_ARROW,              CHR_RIGHT_ARROW,
                                     ITM_M_INSR,                    ITM_NULL,                   ITM_M_DELR,               ITM_NULL,              ITM_M_WRAP,                  ITM_M_GROW                    };

const int16_t menu_MODE[]        = { ITM_DEG,                       ITM_RAD,                    ITM_GRAD,                 ITM_MULPI,             ITM_DMS,                     ITM_NULL,
                                     ITM_INP_DEF_43S,               ITM_INP_DEF_DP,             ITM_INP_DEF_CPXDP,        ITM_INP_DEF_LI,        ITM_INP_DEF_SI,              -MNU_CFG,                           //JM
#ifndef DMCP_BUILD
                                     ITM_NULL,                      ITM_ERPN,                   ITM_SETSIG,               ITM_NULL,              ITM_RECT,                    ITM_POLAR                     };    //JM modifoed
#endif
#ifdef DMCP_BUILD
                                     ITM_SYSTEM,                    ITM_ERPN,                   ITM_SETSIG,               ITM_NULL,              ITM_RECT,                    ITM_POLAR                     };
#endif

const int16_t menu_PARTS[]       = { ITM_IP,                        ITM_FP,                     ITM_MANT,                 ITM_EXPT,              ITM_SIGN,                    ITM_MAGNITUDE,
                                     ITM_ROUNDI,                    ITM_ROUND,                  ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_DECOMP,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_RE,                ITM_IM,                      ITM_ANGLE                     };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_PROB[]        = { -MNU_NORML,                    -MNU_T,                     ITM_COMB,                 ITM_PERM,              -MNU_F,                      -MNU_CHI2,
                                     -MNU_LGNRM,                    -MNU_CAUCH,                 ITM_XFACT,                -MNU_EXPON,            -MNU_LOGIS,                  -MNU_WEIBL,                         //JM Added x! where it belongs, directly above Cyx
                                     ITM_NULL,                      -MNU_NBIN,                  -MNU_GEOM,                -MNU_HYPER,            -MNU_BINOM,                  -MNU_POISS,
                                     ITM_RAN,                       ITM_SEED,                   ITM_RANI,                 ITM_NULL,              ITM_LNGAMMA,                 ITM_GAMMAX                    };

const int16_t menu_t[]           = { ITM_TPX,                       ITM_NULL,                   ITM_TX,                   ITM_TUX,               ITM_NULL,                    ITM_TM1P                      };
const int16_t menu_F[]           = { ITM_FPX,                       ITM_NULL,                   ITM_FX,                   ITM_FUX,               ITM_NULL,                    ITM_FM1P                      };
const int16_t menu_chi2[]        = { ITM_chi2Px,                    ITM_NULL,                   ITM_chi2x,                ITM_chi2ux,            ITM_NULL,                    ITM_chi2M1                    };
const int16_t menu_Norml[]       = { ITM_NORMLP,                    ITM_NULL,                   ITM_NORML,                ITM_NORMLU,            ITM_NULL,                    ITM_NORMLM1                   };
const int16_t menu_LgNrm[]       = { ITM_LGNRMP,                    ITM_NULL,                   ITM_LGNRM,                ITM_LGNRMU,            ITM_NULL,                    ITM_LGNRMM1                   };
const int16_t menu_Cauch[]       = { ITM_CAUCHP,                    ITM_NULL,                   ITM_CAUCH,                ITM_CAUCHU,            ITM_NULL,                    ITM_CAUCHM1                   };
const int16_t menu_Expon[]       = { ITM_EXPONP,                    ITM_NULL,                   ITM_EXPON,                ITM_EXPONU,            ITM_NULL,                    ITM_EXPONM1                   };
const int16_t menu_Logis[]       = { ITM_LOGISP,                    ITM_NULL,                   ITM_LOGIS,                ITM_LOGISU,            ITM_NULL,                    ITM_LOGISM1                   };
const int16_t menu_Weibl[]       = { ITM_WEIBLP,                    ITM_NULL,                   ITM_WEIBL,                ITM_WEIBLU,            ITM_NULL,                    ITM_WEIBLM1                   };
const int16_t menu_Binom[]       = { ITM_BINOMP,                    ITM_NULL,                   ITM_BINOM,                ITM_BINOMU,            ITM_NULL,                    ITM_BINOMM1                   };
const int16_t menu_Geom[]        = { ITM_GEOMP,                     ITM_NULL,                   ITM_GEOM,                 ITM_GEOMU,             ITM_NULL,                    ITM_GEOMM1                    };
const int16_t menu_Hyper[]       = { ITM_HYPERP,                    ITM_NULL,                   ITM_HYPER,                ITM_HYPERU,            ITM_NULL,                    ITM_HYPERM1                   };
const int16_t menu_Nbin[]        = { ITM_NBINP,                     ITM_NULL,                   ITM_NBIN,                 ITM_NBINU,             ITM_NULL,                    ITM_NBINM1                    };
const int16_t menu_Poiss[]       = { ITM_POISSP,                    ITM_NULL,                   ITM_POISS,                ITM_POISSU,            ITM_NULL,                    ITM_POISSM1                   };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                  <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_PFN[]         = { ITM_LBL,                       ITM_RTN,                    ITM_END,                  ITM_GTO,               ITM_XEQ,                     -MNU_PFN2,          //JM

                                     ITM_INPUT,                     ITM_END,                    ITM_ERR,                  ITM_TICKS,             ITM_PAUSE,                   -MNU_PFN2,
                                     ITM_PSTO,                      ITM_PRCL,                   ITM_ALPHAOFF,             ITM_ALPHAON,           ITM_CNST,                    ITM_PUTK,
                                     ITM_R_CLR,                     ITM_R_COPY,                 ITM_R_SORT,               ITM_R_SWAP,            ITM_LocR,                    ITM_POPLR                     };

const int16_t menu_PFN2[]        = { ITM_MENU,                      ITM_KEYG,                   ITM_KEYX,                 ITM_CLMENU,            ITM_EXITALL,                 ITM_RTNP1,
                                     ITM_SDL,                       ITM_SDR,                    ITM_MSG,                  ITM_NOP,               ITM_VARMNU,                  ITM_MVAR,
                                     ITM_BACK,                      ITM_CASE,                   ITM_SKIP,                 ITM_AGRAPH,            ITM_PIXEL,                   ITM_POINT                     };

const int16_t menu_STAT[]        = { ITM_SIGMAMINUS,                ITM_XBAR,                   ITM_S,                    ITM_sigma,             ITM_SM,                      ITM_SUM,
                                     ITM_SIGMAPLUS,                 ITM_XW,                     ITM_SW,                   ITM_sigmaw,            ITM_SMW,                     -MNU_ST_GRAPH/*ITM_NSIGMA*/,                         //JM added ITM_NSIGMA
                                     ITM_CLSIGMA,                   ITM_XG,                     ITM_epsilon,              ITM_epsilonP,          ITM_epsilonM,                ITM_PLOT,
                                     ITM_LR,                        ITM_CORR,                   ITM_SXY,                  ITM_COV,               ITM_XCIRC,                   ITM_YCIRC,
                                     ITM_NULL,                      ITM_XH,                     ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                     ITM_NULL,                      ITM_XRMS,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                     ITM_LINF,                      ITM_EXPF,                   ITM_LOGF,                 ITM_POWERF,            ITM_NULL,                    ITM_BESTF,
                                     ITM_ORTHOF,                    ITM_GAUSSF,                 ITM_CAUCHF,               ITM_PARABF,            ITM_HYPF,                    ITM_ROOTF                     };

const int16_t menu_SUMS[]        = { ITM_NSIGMA,                    ITM_SIGMAx,                 ITM_SIGMAx2,              ITM_SIGMAxy,           ITM_SIGMAy2,                 ITM_SIGMAy,
                                     ITM_NULL,                      ITM_SIGMAlnx,               ITM_SIGMAln2x,            ITM_SIGMAlnxy,         ITM_SIGMAln2y,               ITM_SIGMAlny,
                                     ITM_SIGMAx2y,                  ITM_SIGMAxlny,              ITM_NULL,                 ITM_SIGMAlnyonx,       ITM_NULL,                    ITM_SIGMAylnx,
                                     ITM_SIGMAx2ony,                ITM_SIGMA1onx,              ITM_SIGMA1onx2,           ITM_SIGMAxony,         ITM_SIGMA1ony2,              ITM_SIGMA1ony,
                                     ITM_SIGMAx3,                   ITM_SIGMAx4,                ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

const int16_t menu_STK[]         = { ITM_Xex,                       ITM_Yex,                    ITM_Zex,                  ITM_Tex,               ITM_ex,                      ITM_CLSTK,                          //JM Re-arranged
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_DROPY,                          //JM Added CLRSTK
                                     ITM_FILL,                      ITM_LASTX,                  ITM_NULL,                 ITM_Rdown,             ITM_Rup,                     ITM_DROP                      };    //JM Added missing STACK items

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_TEST[]        = { ITM_XLT,                       ITM_XLE,                    ITM_XEQU,                 ITM_XNE,               ITM_XGE,                     ITM_XGT,
                                     ITM_INT,                       ITM_EVEN,                   ITM_ODD,                  ITM_PRIME,             ITM_LEAP,                    ITM_FPQ,
                                     ITM_ENTRY,                     ITM_KEYQ,                   ITM_LBLQ,                 ITM_STRI,              ITM_CONVG,                   ITM_TOP,
                                     ITM_XEQUP0,                    ITM_XEQUM0,                 ITM_XAEQU,                ITM_MATR,              ITM_CPX,                     ITM_REAL,
                                     ITM_SPEC,                      ITM_NAN,                    ITM_NULL,                 ITM_M_SQR,             ITM_NULL,                    ITM_DBL                       };

const int16_t menu_XFN[]         = { ITM_AGM,                       ITM_BN,                     ITM_BNS,                  ITM_ERF,               ITM_ERFC,                    -MNU_ORTHOG,
                                     ITM_FIB,                       ITM_GD,                     ITM_GDM1,                 ITM_IXYZ,              ITM_IGAMMAP,                 ITM_IGAMMAQ,
                                     ITM_JYX,                       ITM_LNBETA,                 ITM_LNGAMMA,              ITM_MAX,               ITM_MIN,                     ITM_NEXTP,
                                     ITM_sinc,                      ITM_WM,                     ITM_WP,                   ITM_WM1,               ITM_BETAXY,                  ITM_gammaXY,
                                     ITM_GAMMAXY,                   ITM_zetaX,                  ITM_M1X,                  ITM_NULL,              ITM_NULL,                    ITM_PARALLEL,                 
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_PGMTST,            -MNU_GRAPH,                  -MNU_EE                       };    //JM Added 

const int16_t menu_Orthog[]      = { ITM_HN,                        ITM_Ln,                     ITM_LNALPHA,              ITM_PN,                ITM_TN,                      ITM_UN,
                                     ITM_HNP,                       ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

// Following menu is UPPER CASE for lower case: +80
const int16_t menu_alpha_INTL[]  = { CHR_A,                         CHR_A_MACRON,               CHR_A_ACUTE,              CHR_A_BREVE,           CHR_A_GRAVE,                 CHR_A_DIARESIS,
                                     CHR_A_TILDE,                   CHR_A_CIRC,                 CHR_A_RING,               CHR_AE,                CHR_A_OGONEK,                CHR_B,
                                     CHR_C,                         CHR_C_ACUTE,                CHR_C_CARON,              CHR_C_CEDILLA,         CHR_D,                       CHR_D_STROKE,
                                     CHR_D_CARON,                   CHR_E,                      CHR_E_MACRON,             CHR_E_ACUTE,           CHR_E_BREVE,                 CHR_E_GRAVE,
                                     CHR_E_DIARESIS,                CHR_E_CIRC,                 CHR_E_DOT,                CHR_E_BREVE,           CHR_E_OGONEK,                CHR_F,
                                     CHR_G,                         CHR_G_BREVE,                CHR_H,                    CHR_I,                 CHR_I_MACRON,                CHR_I_ACUTE,
                                     CHR_I_BREVE,                   CHR_I_GRAVE,                CHR_I_DIARESIS,           CHR_I_CIRC,            CHR_I_OGONEK,                CHR_I_DOT,
                                     CHR_I_DOTLESS,                 CHR_J,                      CHR_K,                    CHR_L,                 CHR_L_STROKE,                CHR_L_ACUTE,
                                     CHR_L_APOSTROPHE,              CHR_M,                      CHR_N,                    CHR_N_ACUTE,           CHR_N_CARON,                 CHR_N_TILDE,
                                     CHR_O,                         CHR_O_MACRON,               CHR_O_ACUTE,              CHR_O_BREVE,           CHR_O_GRAVE,                 CHR_O_DIARESIS,
                                     CHR_O_TILDE,                   CHR_O_CIRC,                 CHR_O_STROKE,             CHR_OE,                CHR_P,                       CHR_Q,
                                     CHR_R,                         CHR_R_CARON,                CHR_R_ACUTE,              CHR_S,                 ITM_NULL,                    CHR_S_ACUTE,
                                     CHR_S_CARON,                   CHR_S_CEDILLA,              CHR_T,                    CHR_T_CARON,           CHR_T_CEDILLA,               CHR_U,
                                     CHR_U_MACRON,                  CHR_U_ACUTE,                CHR_U_BREVE,              CHR_U_GRAVE,           CHR_U_DIARESIS,              CHR_U_TILDE,
                                     CHR_U_CIRC,                    CHR_U_RING,                 CHR_U_OGONEK,             CHR_V,                 CHR_W,                       CHR_W_CIRC,
                                     CHR_X,                         CHR_Y,                      CHR_Y_CIRC,               CHR_Y_ACUTE,           CHR_Y_DIARESIS,              CHR_Z,
                                     CHR_Z_ACUTE,                   CHR_Z_CARON,                CHR_Z_DOT,                ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_alpha_intl[]  = { CHR_a,                         CHR_a_MACRON,               CHR_a_ACUTE,              CHR_a_BREVE,           CHR_a_GRAVE,                 CHR_a_DIARESIS,
                                     CHR_a_TILDE,                   CHR_a_CIRC,                 CHR_a_RING,               CHR_ae,                CHR_a_OGONEK,                CHR_b,
                                     CHR_c,                         CHR_c_ACUTE,                CHR_c_CARON,              CHR_c_CEDILLA,         CHR_d,                       CHR_d_STROKE,
                                     CHR_d_APOSTROPHE,              CHR_e,                      CHR_e_MACRON,             CHR_e_ACUTE,           CHR_e_BREVE,                 CHR_e_GRAVE,
                                     CHR_e_DIARESIS,                CHR_e_CIRC,                 CHR_e_DOT,                CHR_e_BREVE,           CHR_e_OGONEK,                CHR_f,
                                     CHR_g,                         CHR_g_BREVE,                CHR_h,                    CHR_i,                 CHR_i_MACRON,                CHR_i_ACUTE,
                                     CHR_i_BREVE,                   CHR_i_GRAVE,                CHR_i_DIARESIS,           CHR_i_CIRC,            CHR_i_OGONEK,                CHR_i_DOT,
                                     CHR_i_DOTLESS,                 CHR_j,                      CHR_k,                    CHR_l,                 CHR_l_STROKE,                CHR_l_ACUTE,
                                     CHR_l_APOSTROPHE,              CHR_m,                      CHR_n,                    CHR_n_ACUTE,           CHR_n_CARON,                 CHR_n_TILDE,
                                     CHR_o,                         CHR_o_MACRON,               CHR_o_ACUTE,              CHR_o_BREVE,           CHR_o_GRAVE,                 CHR_o_DIARESIS,
                                     CHR_o_TILDE,                   CHR_o_CIRC,                 CHR_o_STROKE,             CHR_oe,                CHR_p,                       CHR_q,
                                     CHR_r,                         CHR_r_CARON,                CHR_r_ACUTE,              CHR_s,                 CHR_s_SHARP,                 CHR_s_ACUTE,
                                     CHR_s_CARON,                   CHR_s_CEDILLA,              CHR_t,                    CHR_t_APOSTROPHE,      CHR_t_CEDILLA,               CHR_u,
                                     CHR_u_MACRON,                  CHR_u_ACUTE,                CHR_u_BREVE,              CHR_u_GRAVE,           CHR_u_DIARESIS,              CHR_u_TILDE,
                                     CHR_u_CIRC,                    CHR_u_RING,                 CHR_u_OGONEK,             CHR_v,                 CHR_w,                       CHR_w_CIRC,
                                     CHR_x,                         CHR_y,                      CHR_y_CIRC,               CHR_y_ACUTE,           CHR_y_DIARESIS,              CHR_z,
                                     CHR_z_ACUTE,                   CHR_z_CARON,                CHR_z_DOT,                ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

const int16_t menu_CATALOG[]     = { -MNU_FCNS,                     -MNU_SYSFL,                 -MNU_CHARS,               -MNU_PROGS,            -MNU_VARS,                   -MNU_MENUS,
                                     ITM_NULL,                      ITM_NULL,                   -MNU_DIGITS,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

const int16_t menu_DIGITS[]      = { CHR_A,                         CHR_B,                      CHR_C,                    CHR_D,                 CHR_E,                       CHR_F,
                                     CHR_5,                         CHR_6,                      CHR_7,                    CHR_8,                 CHR_9,                       CHR_i,
                                     CHR_0,                         CHR_1,                      CHR_2,                    CHR_3,                 CHR_4,                       ITM_NULL                      };

const int16_t menu_CHARS[]       = { -MNU_A_Z,                      -MNU_ALPHA_OMEGA,           -MNU_ALPHAINTL,           -MNU_ALPHAMATH,        -MNU_MyAlpha,                -MNU_ALPHADOT,
                                     -MNU_a_z,                      -MNU_alpha_omega,           ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };    //JM

const int16_t menu_PROGS[]       = { -MNU_RAM,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    -MNU_FLASH                    };

const int16_t menu_VARS[]        = { -MNU_LINTS,                    -MNU_SINTS,                 -MNU_REALS,               -MNU_CPXS,             -MNU_STRING,                 -MNU_MATRS,
                                     -MNU_DATES,                    -MNU_TIMES,                 -MNU_ANGLES,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

const int16_t menu_VAR[]         = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
// Following menu is UPPER CASE for lower case: +26
const int16_t menu_A_Z[]         = { CHR_A,                         CHR_B,                      CHR_C,                    CHR_D,                 CHR_E,                       CHR_F,
                                     CHR_G,                         CHR_H,                      CHR_I,                    CHR_J,                 CHR_K,                       CHR_L,
                                     CHR_M,                         CHR_N,                      CHR_O,                    CHR_P,                 CHR_Q,                       CHR_R,
                                     CHR_S,                         CHR_T,                      CHR_U,                    CHR_V,                 CHR_W,                       CHR_X,
                                     CHR_Y,                         CHR_Z,                      ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

const int16_t menu_a_z[]         = { CHR_a,                         CHR_b,                      CHR_c,                    CHR_d,                 CHR_e,                       CHR_f,
                                     CHR_g,                         CHR_h,                      CHR_i,                    CHR_j,                 CHR_k,                       CHR_l,
                                     CHR_m,                         CHR_n,                      CHR_o,                    CHR_p,                 CHR_q,                       CHR_r,
                                     CHR_s,                         CHR_t,                      CHR_u,                    CHR_v,                 CHR_w,                       CHR_x,
                                     CHR_y,                         CHR_z,                      ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

// Following menu is UPPER CASE for lower case: +36
const int16_t menu_ALPHA_OMEGA[] = { CHR_ALPHA,                     CHR_BETA,                   CHR_GAMMA,                CHR_DELTA,             CHR_EPSILON,                 CHR_DIGAMMA,
                                     CHR_ZETA,                      CHR_ETA,                    CHR_THETA,                CHR_IOTA,              CHR_KAPPA,                   CHR_LAMBDA,
                                     CHR_MU,                        CHR_NU,                     CHR_XI,                   CHR_OMICRON,           CHR_PI,                      CHR_QOPPA,
                                     CHR_RHO,                       CHR_SIGMA,                  CHR_TAU,                  CHR_UPSILON,           CHR_PHI,                     CHR_CHI,
                                     CHR_PSI,                       CHR_OMEGA,                  CHR_SAMPI,                ITM_NULL,              ITM_NULL,                    ITM_NULL,                           //JM modified greek sequence
                                     CHR_IOTA_DIALYTIKA,            ITM_NULL,                   ITM_NULL,                 CHR_UPSILON_DIALYTIKA, ITM_NULL,                    ITM_NULL                      };

const int16_t menu_alpha_omega[] = { CHR_alpha,                     CHR_beta,                   CHR_gamma,                CHR_delta,             CHR_epsilon,                 CHR_digamma,
                                     CHR_zeta,                      CHR_eta,                    CHR_theta,                CHR_iota,              CHR_kappa,                   CHR_lambda,
                                     CHR_mu,                        CHR_nu,                     CHR_xi,                   CHR_omicron,           CHR_pi,                      CHR_qoppa,
                                     CHR_rho,                       CHR_sigma,                  CHR_tau,                  CHR_upsilon,           CHR_phi,                     CHR_chi,
                                     CHR_psi,                       CHR_omega,                  CHR_sampi,                ITM_NULL,              ITM_NULL,                    ITM_NULL,                        
                                     CHR_alpha_TONOS,               CHR_epsilon_TONOS,          CHR_eta_TONOS,            CHR_iotaTON,           CHR_iota_DIALYTIKA_TONOS,    ITM_NULL,                           //JM modified greek sequence
                                     CHR_iota_DIALYTIKA,            CHR_omicron_TONOS,          CHR_upsilon_TONOS,        CHR_upsilon_DIALYTIKA, CHR_upsilon_DIALYTIKA_TONOS, CHR_omega_TONOS               };

const int16_t menu_AngleConv[]   = { ITM_toDEG,                     ITM_toRAD,                  ITM_toGRAD,               ITM_toMULpi,           ITM_toDMS,                   -MNU_UNITCONV,                      //JM mod
                                     ITM_DEGto,                     ITM_RADto,                  ITM_GRADto,               ITM_MULPIto,           ITM_DMSto,                   ITM_NULL,
                                     ITM_DtoR,                      ITM_RtoD,                   ITM_toHMS,                ITM_ms,                ITM_DtoDMS,                  ITM_DMStoD                    };    //JM Added H.MS

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_UnitConv[]    = { -MNU_CONVE,                    -MNU_CONVP,                 ITM_NULL,                 -MNU_CONVFP,           -MNU_CONVM,                  -MNU_CONVX,
                                     ITM_CtoF,                      ITM_FtoC,                   ITM_NULL,                 ITM_NULL,              -MNU_CONVV,                  -MNU_CONVA,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                     10000+ITM_PRtoDBc,             10000+ITM_DBtoPRc,          ITM_NULL,                 ITM_NULL,              10000+ITM_FRtoDBc,           10000+ITM_DBtoFRc,
                                     30000+ITM_PRtoDBb,             30000+ITM_DBtoPRb,          ITM_NULL,                 ITM_NULL,              30000+ITM_FRtoDBb,           30000+ITM_DBtoFRb,
                                     20000+ITM_PRtoDB,              20000+ITM_DBtoPR,           ITM_NULL,                 ITM_NULL,              20000+ITM_FRtoDB,            20000+ITM_DBtoFR              };

const int16_t menu_ConvE[]       = { ITM_CALtoJ,                    ITM_JtoCAL,                 ITM_BTUtoJ,               ITM_JtoBTU,            ITM_WHtoJ,                   ITM_JtoWH                     };

const int16_t menu_ConvP[]       = { ITM_HPEtoW,                    ITM_WtoHPE,                 ITM_HPUKtoW,              ITM_WtoHPUK,           ITM_HPMtoW,                  ITM_WtoHPM                    };

const int16_t menu_ConvFP[]      = { ITM_LBFtoN,                    ITM_NtoLBF,                 ITM_BARtoPA,              ITM_PAtoBAR,           ITM_PSItoPA,                 ITM_PAtoPSI,
                                     10000+ITM_IHGtoPAb,            10000+ITM_PAtoIHGb,         10000+ITM_TORtoPAb,       10000+ITM_PAtoTORb,    10000+ITM_ATMtoPA,           10000+ITM_PAtoATM,
                                     20000+ITM_IHGtoPA,             20000+ITM_PAtoIHG,          20000+ITM_TORtoPA,        20000+ITM_PAtoTOR,     20000+ITM_ATMtoPAb,          20000+ITM_PAtoATMb,           
                                     10000+ITM_mmHGtoPAb,           10000+ITM_PAtommHGb,        ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,                           //JM
                                     30000+ITM_mmHGtoPA,            30000+ITM_PAtommHG,         ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };    //JM

const int16_t menu_ConvM[]       = { ITM_LBStoKG,                   ITM_KGtoLBS,                ITM_CWTtoKG,              ITM_KGtoCWT,           ITM_OZtoKG,                  ITM_KGtoOZ,
                                     10000+ITM_STOtoKGb,            10000+ITM_KGtoSTOb,         10000+ITM_SCWtoKGb,       10000+ITM_KGtoSCWb,    10000+ITM_TRZtoKGb,          10000+ITM_KGtoTRZb,
                                     20000+ITM_STOtoKG,             20000+ITM_KGtoSTO,          20000+ITM_SCWtoKG,        20000+ITM_KGtoSCW,     20000+ITM_TRZtoKG,           20000+ITM_KGtoTRZ,
                                     10000+ITM_TONtoKG,             10000+ITM_KGtoTON,          10000+ITM_STtoKGc,        10000+ITM_KGtoSTc,     10000+ITM_CARATtoKGb,        10000+ITM_KGtoCARATb,
                                     30000+ITM_TONtoKGb,            30000+ITM_KGtoTONb,         30000+ITM_STtoKGb,        30000+ITM_KGtoSTb,     30000+ITM_CARATtoKG,         30000+ITM_KGtoCARAT,
                                     20000+ITM_TONtoKGc,            20000+ITM_KGtoTONc,         20000+ITM_STtoKG,         20000+ITM_KGtoST,      20000+ITM_CARATtoKGc,        20000+ITM_KGtoCARATc          };

const int16_t menu_ConvX[]       = { ITM_AUtoM,                     ITM_MtoAU,                  ITM_LYtoM,                ITM_MtoLY,             ITM_PCtoM,                   ITM_MtoPC,
                                     ITM_MItoM,                     ITM_MtoMI,                  ITM_NMItoM,               ITM_MtoNMI,            ITM_FTtoM,                   ITM_MtoFT,
                                     ITM_INtoM,                     ITM_MtoINCH,                ITM_NULL,                 ITM_NULL,              ITM_YDtoM,                   ITM_MtoYD,
                                     10000+ITM_FATHOMtoMb,          10000+ITM_MtoFATHOMb,       10000+ITM_POINTtoMb,      10000+ITM_MtoPOINT,    10000+ITM_FTUStoMc,          10000+ITM_MtoFTUSc,
                                     30000+ITM_FATHOMtoM,           30000+ITM_MtoFATHOM,        30000+ITM_POINTtoM,       30000+ITM_MtoPOINTb,   30000+ITM_FTUStoMb,          30000+ITM_MtoFTUSb,
                                     20000+ITM_FATHOMtoMc,          20000+ITM_MtoFATHOMc,       20000+ITM_POINTtoMc,      20000+ITM_MtoPOINTc,   20000+ITM_FTUStoM,           20000+ITM_MtoFTUS             };

const int16_t menu_ConvA[]       = { 10000+ITM_ACtoM2b,             10000+ITM_M2toACb,          ITM_HECTAREtoM2,          ITM_M2toHECTARE,       10000+ITM_ACUStoM2b,         10000+ITM_M2toACUSb,
                                     20000+ITM_ACtoM2,              20000+ITM_M2toAC,           ITM_NULL,                 ITM_NULL,              20000+ITM_ACUStoM2,          20000+ITM_M2toACUS            };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_ConvV[]       = { ITM_GLUKtoM3,                  ITM_M3toGLUK,               ITM_QTtoM3,               ITM_M3toQT,            ITM_GLUStoM3,                ITM_M3toGLUS,
                                     10000+ITM_FZUKtoM3b,           10000+ITM_M3toFZUKb,        10000+ITM_BARRELtoM3b,    10000+ITM_M3toBARRELb, 10000+ITM_FZUStoM3b,         10000+ITM_M3toFZUSb,
                                     20000+ITM_FZUKtoM3,            20000+ITM_M3toFZUK,         20000+ITM_BARRELtoM3,     20000+ITM_M3toBARREL,  20000+ITM_FZUStoM3,          20000+ITM_M3toFZUS            };

const int16_t menu_alphaFN[]     = { ITM_XtoALPHA,                  ITM_ALPHARL,                ITM_ALPHARR,              ITM_ALPHASL,           ITM_ALPHAPOS,                ITM_ALPHAtoX,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_ALPHALENG,               ITM_NULL,
                                     ITM_FBR,                       ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_MATRS[]       = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_STRING[]      = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_CPXS[]        = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_RAM[]         = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_FLASH[]       = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_DATES[]       = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_TIMES[]       = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_ANGLES[]      = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_SINTS[]       = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_LINTS[]       = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_SYSFL[]       = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

const int16_t menu_alphaMATH[]   = { CHR_LESS_THAN,                 CHR_LESS_EQUAL,             CHR_EQUAL,                CHR_ALMOST_EQUAL,      CHR_GREATER_EQUAL,           CHR_GREATER_THAN,
                                     CHR_LEFT_CURLY_BRACKET,        CHR_LEFT_SQUARE_BRACKET,    CHR_LEFT_PARENTHESIS,     CHR_RIGHT_PARENTHESIS, CHR_RIGHT_SQUARE_BRACKET,    CHR_RIGHT_CURLY_BRACKET,
                                     CHR_PROD_SIGN,                 CHR_DIVIDE,                 CHR_INTEGRAL,             CHR_SUB_INFINITY,      CHR_SUP_INFINITY,            CHR_INFINITY,
                                     CHR_NOT,                       CHR_AND,                    CHR_OR,                   CHR_NOT_EQUAL,         CHR_PIPE,                    CHR_AMPERSAND,
                                     CHR_MEASURED_ANGLE,            CHR_RIGHT_ANGLE,            CHR_PERPENDICULAR,        CHR_CUBE_ROOT,         CHR_SQUARE_ROOT,             CHR_xTH_ROOT,
                                     CHR_x_BAR,                     CHR_y_BAR,                  CHR_x_CIRC,               CHR_y_CIRC,            CHR_x_UNDER_ROOT,            CHR_y_UNDER_ROOT,
                                     CHR_COLON_EQUALS,              CHR_ESTIMATES,              CHR_CORRESPONDS_TO,       CHR_SUB_E_OUTLINE,     CHR_COMPLEX_C,               CHR_REAL_R,
                                     CHR_SUN,                       CHR_SUB_SUN,                CHR_SUB_EARTH,            ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                     CHR_PLUS_MINUS,                CHR_CIRCUMFLEX,             CHR_SUP_T,                CHR_SUP_MINUS_1,       CHR_PLANCK_2PI,              ITM_NULL                      };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_alphaDot[]    = { CHR_EXCLAMATION_MARK,          CHR_SEMICOLON,              CHR_UNDERSCORE,           CHR_QUOTE,             CHR_DOUBLE_QUOTE,            CHR_AT,
                                     CHR_INVERTED_EXCLAMATION_MARK, CHR_INVERTED_QUESTION_MARK, CHR_SECTION,              ITM_NULL,              CHR_TILDE,                   CHR_BACK_SLASH,
                                     CHR_DOLLAR,                    CHR_EURO,                   CHR_PERCENT,              CHR_AMPERSAND,         CHR_POUND,                   CHR_YEN,
                                     CHR_LEFT_ARROW,                CHR_UP_ARROW,               CHR_SERIAL_IO,            CHR_DOWN_ARROW,        CHR_RIGHT_ARROW,             CHR_PGM_BEGIN,
                                     CHR_LEFT_DOUBLE_ANGLE,         CHR_RIGHT_DOUBLE_ANGLE,     CHR_EMPTY_SET,            CHR_WATCH,             CHR_BULLET,                  CHR_ASTERISK,
                                     CHR_HOURGLASS,                 CHR_NEG_EXCLAMATION_MARK,   CHR_USER_MODE,            CHR_BATTERY,           ITM_NULL,                    CHR_SUP_ASTERISK              };

const int16_t menu_REALS[]       = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_1stDeriv[]    = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_2ndDeriv[]    = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_Sf[]          = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_Solver[]      = { ITM_Not,                       ITM_yet,                    ITM_defined,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_IO[]          = { ITM_SAVE,                      ITM_LOAD,                   ITM_LOADP,                ITM_LOADR,             ITM_LOADSS,                  ITM_LOADSIGMA,                      //JM ADDED SAVE
                                     ITM_QUIET,                     ITM_TONE,                   ITM_BEEP,                 -MNU_PRINT,            ITM_RECV,                    ITM_SEND,                           //JM RELOCATED BEEP
                                     -MNU_PRINT,                    ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };    //JM ADDED PRINT 
const int16_t menu_PRINT[]       = { ITM_PRINTERX,                  ITM_PRINTERR,               ITM_PRINTERSIGMA,         ITM_PRINTERADV,        ITM_PRINTERLCD,              ITM_PRINTERPROG,
                                     ITM_PRINTERSTK,                ITM_PRINTERREGS,            ITM_PRINTERUSER,          ITM_PRINTERTAB,        ITM_PRINTERHASH,             ITM_PRINTERCHAR,
                                     ITM_NULL,                      ITM_P_ALLREGS,              ITM_NULL,                 ITM_PRINTERWIDTH,      ITM_PRINTERDLAY,             ITM_PRINTERMODE               };
const int16_t menu_Tam[]         = { ITM_INDIRECTION,               -MNU_VAR,                   ITM_ST_X,                 ITM_ST_Y,              ITM_ST_Z,                    ITM_ST_T                      };
const int16_t menu_TamCmp[]      = { ITM_INDIRECTION,               -MNU_VAR,                   ITM_ST_X,                 ITM_ST_Y,              ITM_ST_Z,                    ITM_ST_T,
                                     ITM_0P,                        ITM_1P,                     ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_TamFlag[]     = { ITM_INDIRECTION,               -MNU_SYSFL,                 ITM_ST_X,                 ITM_ST_Y,              ITM_ST_Z,                    ITM_ST_T                      };
const int16_t menu_TamStoRcl[]   = { ITM_INDIRECTION,               -MNU_VAR,                   ITM_ST_X,                 ITM_ST_Y,              ITM_ST_Z,                    ITM_ST_T,
                                     ITM_Config,                    ITM_Stack,                  ITM_NULL,                 ITM_NULL,              ITM_Max,                     ITM_Min,
                                     ITM_dddEL,                     ITM_dddIJ,                  ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_MyMenu[]      = { ITM_cos                                                                                                                                                                };


const int16_t menu_CFG[]         = { ITM_SLOW,                      ITM_FAST,                   ITM_SSIZE4,               ITM_SSIZE8,            ITM_CB_CPXRES,               ITM_CB_LEADING_ZERO,
                                     ITM_CPXI,                      ITM_CPXJ,                   ITM_DENANY,               ITM_DENFAC,            ITM_DENFIX,                  ITM_DENMAX,                         //JM sequence change
                                     ITM_MULTCR,                    ITM_MULTDOT,                ITM_RM,                   ITM_QUIET,             ITM_SCIOVR,                  ITM_ENGOVR,                         //JM sequence change

                                     ITM_SETCHN,                    ITM_SETEUR,                 ITM_SETIND,               ITM_SETJPN,            ITM_SETUK,                   ITM_SETUSA,                    
                                     ITM_CLK12,                     ITM_CLK24,                  ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                     ITM_RDXPER,                    ITM_RDXCOM,                 ITM_GAP,                  ITM_DMY,               ITM_YMD,                     ITM_MDY,

#if !defined(DMCP_BUILD) && defined (INLINE_TEST)                               //vv dr
                                     ITM_BASE_HOME,                 ITM_BASE_AHOME,             -MNU_INL_TST,             ITM_FG_LINE,           ITM_FG_DOTS,                 ITM_G_DOUBLETAP,                    //JM
#else                                                                           //^^
                                     ITM_BASE_HOME,                 ITM_BASE_AHOME,             ITM_NULL,                 ITM_FG_LINE,           ITM_FG_DOTS,                 ITM_G_DOUBLETAP,                    //JM
#endif                                                                          //dr
                                     -MNU_ASN,                      ITM_NULL,                   ITM_NULL,                 ITM_HOMEx3,            ITM_HOMEx3T,                 ITM_SHTIM                      };   //JM

const int16_t menu_BASE[]        = { CHR_A,                         CHR_B,                      CHR_C,                    CHR_D,                 CHR_E,                       CHR_F, 
                                     ITM_toSI,                      KEY_HASH,                   ITM_2HEX,                 ITM_2DEC,              ITM_2OCT,             	      ITM_2BIN,                           //JM BASE MENU ADDED
                                     ITM_CB_LEADING_ZERO,           ITM_WSIZE,                  ITM_WS64,                 ITM_WS32,              ITM_WS16,             	      ITM_WS8,                      };    //JM BASE MENU ADDED

const int16_t menu_EE[]          = { ITM_pi,                        ITM_op_j,                   ITM_SQUARE,               ITM_op_a,              ITM_op_a2,                   ITM_CLSTK,                          //JM EE
                                     ITM_EE_D2Y,                    ITM_EE_Y2D,                 ITM_EE_A2S,               ITM_EE_S2A,            ITM_PARALLEL,                -MNU_CPX,                           //JM EE
                                     ITM_DEG,                       ITM_RAD,                    ITM_DtoR,                 ITM_RtoD,              ITM_RECT,                    ITM_POLAR,                          //JM EE

                                     ITM_EE_D2Y,                    ITM_EE_Y2D,                 ITM_EE_A2S,               ITM_EE_S2A,            ITM_EE_EXP_TH,                ITM_CLSTK,                         //JM EE
                                     ITM_EE_STO_V_I,                ITM_EE_STO_IR,              ITM_EE_STO_V_Z,           ITM_EE_X2BAL,          ITM_PARALLEL,                -MNU_CPX,                           //JM EE
                                     ITM_EE_STO_Z,                  ITM_EE_RCL_Z,               ITM_EE_STO_V,             ITM_EE_RCL_V,          ITM_EE_STO_I,                ITM_EE_RCL_I                  };    //JM EE

#ifdef INLINE_TEST
const int16_t menu_Inl_Tst[]     = { ITM_TEST,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_GET_TEST_BS,             ITM_SET_TEST_BS               };    //dr
#endif


const int16_t menu_ASN_N[]       = { 
                                     ITM_U_KEY_USER,                ITM_U_KEY_SIGMA,            ITM_U_KEY_CC,             ITM_U_KEY_PRGM,        ITM_U_KEY_MM,                ITM_U_KEY_ALPHA,                    //JM USER NAORMAL MODE
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,                           //JM USER NAORMAL MODE
                                     -MNU_ASN,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_SH_NORM_E,               ITM_GET_NORM_E                };    //JM USER NAORMAL MODE

const int16_t menu_ASN[]         = { 
                                     ITM_USER_DEFAULTS,             ITM_USER_COMPLEX,           ITM_USER_SHIFTS,          ITM_USER_RESET,        ITM_NULL,                    KEY_USERMODE,         /*ITM_JM_ASN*/
                                     ITM_USER_ALPHA,                ITM_USER_CC,                ITM_USER_GSHFT,           ITM_USER_MYM,          ITM_NULL,                    ITM_JM_SEEK,
                                     ITM_USER_PRGM,                 ITM_USER_USER,              ITM_USER_43D,             ITM_USER_SIGMAPLUS,    ITM_NULL,                    -MNU_ASN_N,
                                                                          
                                     K_00U,                         K_01U,                      K_02U,                    K_03U,                 K_04U,                       K_05U,  //JM USER
                                     Kf00U,                         Kf01U,                      Kf02U,                    Kf03U,                 Kf04U,                       Kf05U,  //JM USER
                                     Kg00U,                         Kg01U,                      Kg02U,                    Kg03U,                 Kg04U,                       Kg05U,  //JM USER
                                                                                                                           
                                     K_06U,                         K_07U,                      K_08U,                    K_09U,                 K_10U,                       K_11U,  //JM USER
                                     Kf06U,                         Kf07U,                      Kf08U,                    Kf09U,                 Kf10U,                       Kf11U,  //JM USER
                                     Kg06U,                         Kg07U,                      Kg08U,                    Kg09U,                 Kg10U,                       Kg11U,  //JM USER
                                                                                                                           
                                     K_12U,                         K_13U,                      K_14U,                    K_15U,                 K_16U,                       ITM_NULL,  //JM USER
                                     Kf12U,                         Kf13U,                      Kf14U,                    Kf15U,                 Kf16U,                       ITM_NULL,  //JM USER
                                     Kg12U,                         Kg13U,                      Kg14U,                    Kg15U,                 Kg16U,                       ITM_NULL,  //JM USER
/*                                   K_17U,                         K_18U,                      K_19U,                    K_20U,                 K_21U,                       ITM_NULL,  //JM USER
                                     Kf17U,                         Kf18U,                      Kf19U,                    Kf20U,                 Kf21U,                       ITM_NULL,  //JM USER
                                     Kg17U,                         Kg18U,                      Kg19U,                    Kg20U,                 Kg21U,                       ITM_NULL,  //JM USER
                                     K_22U,                         K_23U,                      K_24U,                    K_25U,                 K_26U,                       ITM_NULL,  //JM USER
                                     Kf22U,                         Kf23U,                      Kf24U,                    Kf25U,                 Kf26U,                       ITM_NULL,  //JM USER
                                     Kg22U,                         Kg23U,                      Kg24U,                    Kg25U,                 Kg26U,                       ITM_NULL,  //JM USER
                                     K_27U,                         K_28U,                      K_29U,                    K_30U,                 K_31U,                       ITM_NULL,  //JM USER
                                     Kf27U,                         Kf28U,                      Kf29U,                    Kf30U,                 Kf31U,                       ITM_NULL,  //JM USER
                                     Kg27U,                         Kg28U,                      Kg29U,                    Kg30U,                 Kg31U,                       ITM_NULL,  //JM USER
                                     K_32U,                         K_33U,                      K_34U,                    K_35U,                 K_36U,                       ITM_NULL,  //JM USER
                                     Kf32U,                         Kf33U,                      Kf34U,                    Kf35U,                 Kf36U,                       ITM_NULL,  //JM USER
                                     Kg32U,                         Kg33U,                      Kg34U,                    Kg35U,                 Kg36U,                       ITM_NULL   //JM USER
 */
      };

//##################################################################################################################################################################################################################################
const int16_t menu_A_HOME[360]   = { -1,-1,-1,-1,-1,-1,        -1,-1,-1,-1,-1,-1,        -1,-1,-1,-1,-1,-1,               //HOME  0
                                     -1,-1,-1,-1,-1,-1,        -1,-1,-1,-1,-1,-1,        -1,-1,-1,-1,-1,-1,               //HOME -1  (Dn = -)
                                     -1,-1,-1,-1,-1,-1,        -1,-1,-1,-1,-1,-1,        -1,-1,-1,-1,-1,-1,               //HOME -2

                                     -1,32,33,34,35,36,        -1 ,132,133,134,135,136,  -1 ,232,233,234,235,236,         //REPLICA ROW 7
                                     -1,27,28,29,30,31,        -1 ,127,128,129,130,131,  -1 ,227,228,229,230,231,         //REPLICA ROW 6
                                     -1,22,23,24,25,26,        -1 ,122,123,124,125,126,  -1 ,222,223,224,225,226,         //REPLICA ROW 5
                                     -1,17,18,19,20,21,        -1 ,117,118,119,120,121,  -1 ,217,218,219,220,221,         //REPLICA ROW 4
                                     -1,12,13,14,15,16,        -1 ,112,113,114,115,116,  -1 ,212,213,214,215,216,         //REPLICA ROW 3
                                     6 , 7, 8, 9,10,11,        106,107,108,109,110,111,  206,207,208,209,210,211,         //REPLICA ROW 2
                                     0 , 1, 2, 3, 4, 5,        100,101,102,103,104,105,  200,201,202,203,204,205,         //REPLICA ROW 1

                                     233,217,218,219,220,221,  234,222,223,224,225,226,  235,236,228,229,230,231,         //SUMMARY 2    Change all to -1 to have the fixed system
                                     200,201,202,203,204,205,  206,207,208,209,210,211,  232,212,213,214,215,216,         //SUMMARY 1    Change all to -1 to have the fixed system

                                     -1,-1,-1,-1,-1,-1,        -1 ,132,133,134,135,136,  -1 ,232,233,234,235,136,         //FIXED ROW 7
                                     -1,-1,-1,-1,-1,-1,        -1 ,127,128,129,130,131,  -1 ,227,228,229,230,231,         //FIXED ROW 6
                                     -1,-1,-1,-1,-1,-1,        -1 ,122,123,124,125,126,  -1 ,222,223,224,225,226,         //FIXED ROW 5
                                     -1,-1,-1,-1,-1,-1,        -1 ,117,118,119,120,121,  -1 ,217,218,219,220,221,         //FIXED ROW 4
                                     -1,-1,-1,-1,-1,-1,        -1 ,112,113,114,115,116,  -1 ,212,213,214,215,216,         //FIXED ROW 3
                                     -1,-1,-1,-1,-1,-1,        106,107,108,109,110,111,  206,207,208,209,210,211,         //FIXED ROW 2
                                     -1,-1,-1,-1,-1,-1,        100,101,102,103,104,105,  200,201,202,203,204,205,         //FIXED ROW 1

                                     -1,-1,-1,-1,-1,-1,        -1,-1,-1,-1,-1,-1,        -1,-1,-1,-1,-1,-1        };      //HOME  +1  (Up = +)


const int16_t menu_HOME[360]        = { //JMHOMEDEMO: NOTE REMOVE CONST TO MAKE JMHOME DEMO WORK
/*HOME0 */
/*0x18*/                             ITM_SIGMAMINUS,      ITM_YX,          ITM_SQUARE,        ITM_10x,          ITM_EX,           ITM_XFACT,                        //JM HOME
                                     ITM_toREC2,          ITM_toPOL2,      ITM_PARALLEL,      KEY_CC,           ITM_MAGNITUDE,    ITM_ANGLE,                          //JM HOME
                                     ITM_DEG,             ITM_RAD,         ITM_DMS,           ITM_op_j,         ITM_RECT,         ITM_POLAR,                          //JM HOME
/*HOME-1*/                                                                                                                                                             //JM HOME
/*1*18*/                             ITM_SIGMAMINUS,      ITM_YX,          ITM_SQUARE,        ITM_10x,          ITM_EX,           ITM_XFACT,                        //JM HOME
                                     ITM_RI,              KEY_TYPCON_DN,   KEY_TYPCON_UP,     ITM_toSI,         -MNU_BASE,        -MNU_INTS,                          //JM HOME
                                     ITM_SF,              ITM_CF,          ITM_RL,            ITM_RR,           ITM_RMD,          ITM_IDIV,                           //JM HOME
/*HOME-2*/                                                                                                                                                             //JM HOME
/*2*18*/                             ITM_SIGMAMINUS,      ITM_YX,          ITM_SQUARE,        ITM_10x,          ITM_EX,           ITM_XFACT,                        //JM HOME
                                     -MNU_TRI,            -MNU_EXP,        -MNU_CPX,          -MNU_XFN,         -MNU_MyMenu,      -MNU_EE,                            //JM HOME
                                     -MNU_CONST,          ITM_RBR,         ITM_STATUS,        ITM_H_SUMRY,      ITM_H_REPLCA,     ITM_H_FIXED,                        //JM HOME
                                       

// 03 -- 09 HAAKON SUGGESTIONS OF FULL REPLICA
/*ROW 7    */
                                    ITM_NULL,             KEY_EXIT1,        CHR_0,            CHR_PERIOD,       ITM_RS,           ITM_ADD,
/* f       */                       ITM_NULL,             ITM_OFF,          -MNU_HOME,        ITM_SHOW,         ITM_PR,           -MNU_CATALOG,    
/* g       */                       ITM_NULL,             -MNU_PRINT,       ITM_TIMER,        ITM_VIEW,         -MNU_STK,         -MNU_PARTS,   

/*ROW 6    */
                                    ITM_NULL,             KEY_fg,           CHR_1,            CHR_2,            CHR_3,            ITM_SUB,
/* f       */                       ITM_NULL,             ITM_NULL,         ITM_ASSIGN,       KEY_USERMODE,     -MNU_PFN,         -MNU_IO,         
/* g       */                       ITM_NULL,             ITM_NULL,         -MNU_ASN,         -MNU_LOOP,        -MNU_TEST,        -MNU_ALPHAFN,  
 
/*ROW 5    */
                                    ITM_NULL,             KEY_DOWN1,        CHR_4,            CHR_5,            CHR_6,            ITM_MULT,
/* f       */                       ITM_NULL,             KEY_SST,          -MNU_BASE,        -MNU_ANGLECONV,   -MNU_FLAGS,       -MNU_PROB,       
/* g       */                       ITM_NULL,             ITM_STATUS,       -MNU_CLK,         -MNU_UNITCONV,    -MNU_BITS,        -MNU_INTS,    
 
/*ROW 4    */
                                    ITM_NULL,             KEY_UP1,          CHR_7,            CHR_8,            CHR_9,            ITM_DIV,
/* f       */                       ITM_NULL,             KEY_BST,          -MNU_EQN,         -MNU_ADV,         -MNU_MATX,        -MNU_STAT,      
/* g       */                       ITM_NULL,             ITM_RBR,          -MNU_INFO,        -MNU_CONST,       -MNU_XFN,         -MNU_SUMS,     

/*ROW 3    */
                                    ITM_NULL,             ITM_ENTER,        ITM_XexY,         ITM_CHS,          ITM_EXPONENT,     KEY_BACKSPACE,
/* f       */                       ITM_NULL,             ITM_AIM,          ITM_LASTX,        -MNU_MODE,        -MNU_DSP,         -MNU_CLR,       
/* g       */                       ITM_NULL,             -MNU_CPX,         -MNU_EXP,         KEY_CC,           -MNU_FIN,         KEY_UNDO,     

/*ROW 2    */
                                    ITM_STO,              ITM_RCL,          ITM_Rdown,        ITM_sin,          ITM_cos,         ITM_tan,
/* f       */                       KEY_COMPLEX,          ITM_PC,           ITM_pi,           ITM_arcsin,       ITM_arccos,      ITM_arctan,     
/* g       */                       ITM_MAGNITUDE,        ITM_DELTAPC,      ITM_ANGLE,        ITM_XTHROOT,      ITM_LBL,         ITM_RTN,      

 /*ROW 1    */
                                    ITM_SIGMAPLUS,        ITM_1ONX,         ITM_SQUAREROOTX,  ITM_LOG10,        ITM_LN,           ITM_XEQ,
 /* f       */                      ITM_SIGMAMINUS,       ITM_YX,           ITM_SQUARE,       ITM_10x,          ITM_EX,           ITM_GTO,
/* g       */                       ITM_PROFRC,           KEY_HASH,         ITM_DOTDEMU,      ITM_ms,           ITM_toREC2,       ITM_toPOL2,    



// 10 - 11 NIGEL SUGGESTED 36 CELLS OF ALL THE BLUE ONES                                                                                                                                                                         //JM HOME
/* g       */                       ITM_TIMER,            ITM_RBR,          -MNU_INFO,        -MNU_CONST,       -MNU_XFN,         -MNU_SUMS,     
/* g       */                       ITM_VIEW,             ITM_STATUS,       -MNU_CLK,         -MNU_UNITCONV,    -MNU_BITS,        -MNU_INTS,    
/* g       */                       -MNU_STK,             -MNU_PARTS,       -MNU_ASN,         -MNU_LOOP,        -MNU_TEST,        -MNU_ALPHAFN,  

/* g       */                       ITM_PROFRC,           KEY_HASH,         ITM_DOTDEMU,      ITM_ms,           ITM_toREC2,       ITM_toPOL2,    
/* g       */                       ITM_MAGNITUDE,        ITM_DELTAPC,      ITM_ANGLE,        ITM_XTHROOT,      ITM_LBL,          ITM_RTN,      
/* g       */                       -MNU_PRINT,           -MNU_CPX,         -MNU_EXP,         KEY_CC,           -MNU_FIN,         KEY_UNDO,     


// 12 - 18 JACO PROPOSAL WITH FIXED BUTTONS RUNG #1, ALL F-functions on RUNG #2, ALL G-functions on RUNG #3
//
//################################################################################################
/*ROW 7    */                       ITM_SIGMAMINUS,         ITM_YX,           ITM_SQUARE,       ITM_10x,          ITM_EX,           ITM_XFACT,                        //JM HOME
/* f       */                       KEY_EXIT1,              ITM_OFF,          -MNU_HOME,        ITM_SHOW,         ITM_PR,           -MNU_CATALOG,    
/* g       */                       KEY_EXIT1,              -MNU_PRINT,       ITM_TIMER,        ITM_VIEW,         -MNU_STK,         -MNU_PARTS,   

/*ROW 6    */                       ITM_SIGMAMINUS,         ITM_YX,           ITM_SQUARE,       ITM_10x,          ITM_EX,           ITM_XFACT,                        //JM HOME
/* f       */                       KEY_fg,                 ITM_NULL,         ITM_ASSIGN,       KEY_USERMODE,     -MNU_PFN,         -MNU_IO,         
/* g       */                       KEY_fg,                 ITM_NULL,         -MNU_ASN,         -MNU_LOOP,        -MNU_TEST,        -MNU_ALPHAFN,  
 
/*ROW 5    */                       ITM_SIGMAMINUS,         ITM_YX,           ITM_SQUARE,       ITM_10x,          ITM_EX,           ITM_XFACT,                        //JM HOME
/* f       */                       KEY_DOWN1,              KEY_SST,          -MNU_BASE,        -MNU_ANGLECONV,   -MNU_FLAGS,       -MNU_PROB,       
/* g       */                       KEY_DOWN1,              ITM_STATUS,       -MNU_CLK,         -MNU_UNITCONV,    -MNU_BITS,        -MNU_INTS,    
 
/*ROW 4    */                       ITM_SIGMAMINUS,         ITM_YX,           ITM_SQUARE,       ITM_10x,          ITM_EX,           ITM_XFACT,                        //JM HOME
/* f       */                       KEY_UP1,                KEY_BST,          -MNU_EQN,         -MNU_ADV,         -MNU_MATX,        -MNU_STAT,      
/* g       */                       KEY_UP1,                ITM_RBR,          -MNU_INFO,        -MNU_CONST,       -MNU_XFN,         -MNU_SUMS,     

/*ROW 3    */                       ITM_SIGMAMINUS,         ITM_YX,           ITM_SQUARE,       ITM_10x,          ITM_EX,           ITM_XFACT,                        //JM HOME
/* f       */                       ITM_ENTER,              ITM_AIM,          ITM_LASTX,        -MNU_MODE,        -MNU_DSP,         -MNU_CLR,       
/* g       */                       ITM_ENTER,              -MNU_CPX,         -MNU_EXP,         KEY_CC,           -MNU_FIN,         KEY_UNDO,     

/*ROW 2    */                       ITM_SIGMAMINUS,         ITM_YX,           ITM_SQUARE,       ITM_10x,          ITM_EX,           ITM_XFACT,                        //JM HOME
/* f       */                       KEY_COMPLEX,            ITM_PC,           ITM_pi,           ITM_arcsin,       ITM_arccos,      ITM_arctan,     
/* g       */                       ITM_MAGNITUDE,          ITM_DELTAPC,      ITM_ANGLE,        ITM_XTHROOT,      ITM_LBL,         ITM_RTN,      

 /*ROW 1    */                      ITM_SIGMAMINUS,        ITM_YX,           ITM_SQUARE,       ITM_10x,          ITM_EX,           ITM_XFACT,                        //JM HOME
 /* f       */                  //  ITM_SIGMAMINUS,        ITM_YX,           ITM_SQUARE,       ITM_10x,          ITM_EX,           ITM_GTO,
                                    ITM_NULL,              ITM_NULL,         ITM_NULL,         ITM_NULL,         ITM_NULL,         ITM_GTO,    
/* g       */                       ITM_PROFRC,            KEY_HASH,         ITM_DOTDEMU,      ITM_ms,           ITM_toREC2,       ITM_toPOL2,    


#ifdef XXXX
//OLD FOUR HOME MENUS WITH FIXED BUTTONS RUNG #1
//
//################################################################################################
                                                                   ITM_SIGMAMINUS,                ITM_YX,                     ITM_SQUARE,               ITM_10x,               ITM_EX,                      ITM_XFACT,                          //JM HOME
                                                                   KEY_EXIT1,                     -MNU_PRINT,                 ITM_TIMER,                ITM_VIEW,              -MNU_STK,                    -MNU_PARTS,                         //JM HOME
                                                                   KEY_fg,                        ITM_NULL,                   -MNU_ASN,                 -MNU_LOOP,             -MNU_TEST,                   -MNU_ALPHAFN,                       //JM HOME
                                                                                                                                                                                                                                                //JM HOME
                                                                   ITM_SIGMAMINUS,                ITM_YX,                     ITM_SQUARE,               ITM_10x,               ITM_EX,                      ITM_XFACT,                          //JM HOME
                              /* 6 Bottom Prim */  /* 6 */         KEY_DOWN1,                     ITM_STATUS,                 -MNU_CLK,                 -MNU_UNITCONV,         -MNU_BITS,                   -MNU_INTS,                          //JM HOME
                              /* 5 Middle f    */  /* 5 */         KEY_UP1,                       ITM_RBR,                    -MNU_INFO,                -MNU_CONST,            -MNU_XFN,                    -MNU_SUMS,                          //JM HOME



                                                                   ITM_SIGMAMINUS,                ITM_YX,                     ITM_SQUARE,               ITM_10x,               ITM_EX,                      ITM_XFACT,                          //JM HOME
                              #ifdef INLINE_TEST                                                              //vv dr
                              /* 4 Top    g    */  /* 4 */        -MNU_INL_TST,                   -MNU_CPX,                   -MNU_EXP,                 KEY_CC,                -MNU_FIN,                    KEY_UNDO,                           //JM HOME
                              #else                                                                           //^^
                              /* 4 Top    g    */  /* 4 */         ITM_ENTER,                     -MNU_CPX,                   -MNU_EXP,                 KEY_CC,                -MNU_FIN,                    KEY_UNDO,                           //JM HOME
                              #endif                                                                          //dr
                                                                   ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,


                                                                                                                                                                                                                                                //JM HOME
                                                                   ITM_SIGMAMINUS,                ITM_YX,                     ITM_SQUARE,               ITM_10x,               ITM_EX,                      ITM_XFACT,                          //JM HOME
                                                                   ITM_MAGNITUDE,                 ITM_DELTAPC,                ITM_ANGLE,                ITM_GTO,               ITM_LBL,                     ITM_RTN,                            //JM HOME
                                                                   ITM_PROFRC,                    KEY_HASH,                   ITM_DOTDEMU,              ITM_ms,                ITM_toREC2,                  ITM_toPOL2,                          //JM HOME
//#endif //JM END OF LAYOUT 2 DM42 STRICT.
#endif //XXXX


/*HOME1*/
                                     ITM_SIGMAMINUS,                ITM_YX,                     ITM_SQUARE,               ITM_10x,               ITM_EX,                      ITM_XFACT,                        //JM HOME
                                     ITM_SI_f,                      ITM_SI_p,                   ITM_PARALLEL,             KEY_CC,                ITM_MAGNITUDE,               ITM_SI_T,                           //JM HOME                                    
                                     ITM_SI_n,                      ITM_SI_u,                   ITM_SI_m,                 ITM_SI_k,              ITM_SI_M,                    ITM_SI_G                            //JM UNITS SI

//##################################################################################################################################################################################################################################

                                     };                                                                                                                                                                           //JM HOME

const int16_t menu_GRAPH[]        = {
          /*-1------*/                                                                                                                                                                                            //JM ALPHA
                                     ITM_GRF_X0,                    ITM_GRF_Y0,                 ITM_GRF_X1,               ITM_SCRDMP,            ITM_GRAPH,                   ITM_GRF_DX,                         //JM GRAPH
                                     ITM_NULL,                      ITM_GRF_Y1,                 ITM_NULL,                 ITM_NULL,              ITM_GRF_HLP,                 ITM_GRF_DY,                         //JM GRAPH
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };    //JM GRAPH

const int16_t menu_ST_GRAPH[]        = {
          /*-1------*/                                                                                                                                                                                            //JM ALPHA
                                     ITM_SIGMAMINUS,                ITM_SUM,                    ITM_LISTXY,               ITM_P_ALLREGS,         ITM_SCRDMP,                  ITM_PLOT,                         //JM GRAPH
                                     ITM_SIGMAPLUS,                 ITM_NSIGMA,                 ITM_NULL,                 ITM_GRF_X0,            ITM_GRF_X1,                  ITM_GRF_HLP,                         //JM GRAPH
                                     ITM_CLSIGMA,                   ITM_VECT,                   ITM_NULL,                 ITM_DEMO4,             ITM_DEMO5,                   ITM_DEMO6                      };    //JM GRAPH




const int16_t menu_ALPHA[]        = {
          /*-1------*/                                                                                                                                                                                            //JM ALPHA
/* 03 */                             -MNU_MyAlpha,                  -MNU_ALPHA_OMEGA,           -MNU_alpha_omega,         -MNU_ALPHADOT,        -MNU_ALPHAMATH,               -MNU_ALPHAINTL,                     //JM
                                     ITM_ASSIGN,                    KEY_USERMODE,               -MNU_ASN,                 -MNU_CATALOG,          -MNU_MODE,                   ITM_NULL,                           //JM 
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };    //JM

#include "softmenuCatalogs.h"

const softmenu_t softmenu[] = {
  {.menuId = -MNU_MyAlpha,     .numItems = sizeof(menu_MyAlpha    )/sizeof(int16_t), .softkeyItem = menu_MyAlpha     }, // This menu MUST stay the 1st in this list or change #define MY_ALPHA_MENU 0
  {.menuId = -MNU_MyMenu,      .numItems = sizeof(menu_MyMenu     )/sizeof(int16_t), .softkeyItem = menu_MyMenu      },
  {.menuId = -MNU_ADV,         .numItems = sizeof(menu_ADV        )/sizeof(int16_t), .softkeyItem = menu_ADV         },
  {.menuId = -MNU_Sfdx,        .numItems = sizeof(menu_Sfdx       )/sizeof(int16_t), .softkeyItem = menu_Sfdx        },
  {.menuId = -MNU_BITS,        .numItems = sizeof(menu_BITS       )/sizeof(int16_t), .softkeyItem = menu_BITS        },
  {.menuId = -MNU_CLK,         .numItems = sizeof(menu_CLK        )/sizeof(int16_t), .softkeyItem = menu_CLK         },
  {.menuId = -MNU_CLR,         .numItems = sizeof(menu_CLR        )/sizeof(int16_t), .softkeyItem = menu_CLR         },
  {.menuId = -MNU_CPX,         .numItems = sizeof(menu_CPX        )/sizeof(int16_t), .softkeyItem = menu_CPX         },
  {.menuId = -MNU_DSP,         .numItems = sizeof(menu_DISP       )/sizeof(int16_t), .softkeyItem = menu_DISP        },
  {.menuId = -MNU_EQN,         .numItems = sizeof(menu_EQN        )/sizeof(int16_t), .softkeyItem = menu_EQN         },
  {.menuId = -MNU_1STDERIV,    .numItems = sizeof(menu_1stDeriv   )/sizeof(int16_t), .softkeyItem = menu_1stDeriv    },
  {.menuId = -MNU_2NDDERIV,    .numItems = sizeof(menu_2ndDeriv   )/sizeof(int16_t), .softkeyItem = menu_2ndDeriv    },
  {.menuId = -MNU_Sf,          .numItems = sizeof(menu_Sf         )/sizeof(int16_t), .softkeyItem = menu_Sf          },
  {.menuId = -MNU_Solver,      .numItems = sizeof(menu_Solver     )/sizeof(int16_t), .softkeyItem = menu_Solver      },
  {.menuId = -MNU_EXP,         .numItems = sizeof(menu_EXP        )/sizeof(int16_t), .softkeyItem = menu_EXP         },
  {.menuId = -MNU_TRI,         .numItems = sizeof(menu_TRI        )/sizeof(int16_t), .softkeyItem = menu_TRI         },
  {.menuId = -MNU_FIN,         .numItems = sizeof(menu_FIN        )/sizeof(int16_t), .softkeyItem = menu_FIN         },
  {.menuId = -MNU_TVM,         .numItems = sizeof(menu_TVM        )/sizeof(int16_t), .softkeyItem = menu_TVM         },
  {.menuId = -MNU_FLAGS,       .numItems = sizeof(menu_FLAGS      )/sizeof(int16_t), .softkeyItem = menu_FLAGS       },
  {.menuId = -MNU_INFO,        .numItems = sizeof(menu_INFO       )/sizeof(int16_t), .softkeyItem = menu_INFO        },
  {.menuId = -MNU_INTS,        .numItems = sizeof(menu_INTS       )/sizeof(int16_t), .softkeyItem = menu_INTS        },
  {.menuId = -MNU_LOOP,        .numItems = sizeof(menu_LOOP       )/sizeof(int16_t), .softkeyItem = menu_LOOP        },
  {.menuId = -MNU_MATX,        .numItems = sizeof(menu_MATX       )/sizeof(int16_t), .softkeyItem = menu_MATX        },
  {.menuId = -MNU_SIM_EQ,      .numItems = sizeof(menu_M_SIM_Q    )/sizeof(int16_t), .softkeyItem = menu_M_SIM_Q     },
  {.menuId = -MNU_M_EDIT,      .numItems = sizeof(menu_M_EDIT     )/sizeof(int16_t), .softkeyItem = menu_M_EDIT      },
  {.menuId = -MNU_M_EDITN,     .numItems = sizeof(menu_M_EDITN    )/sizeof(int16_t), .softkeyItem = menu_M_EDITN     },
  {.menuId = -MNU_MODE,        .numItems = sizeof(menu_MODE       )/sizeof(int16_t), .softkeyItem = menu_MODE        },
  {.menuId = -MNU_CFG,         .numItems = sizeof(menu_CFG        )/sizeof(int16_t), .softkeyItem = menu_CFG         },
  {.menuId = -MNU_PARTS,       .numItems = sizeof(menu_PARTS      )/sizeof(int16_t), .softkeyItem = menu_PARTS       },
  {.menuId = -MNU_PROB,        .numItems = sizeof(menu_PROB       )/sizeof(int16_t), .softkeyItem = menu_PROB        },
  {.menuId = -MNU_T,           .numItems = sizeof(menu_t          )/sizeof(int16_t), .softkeyItem = menu_t           },
  {.menuId = -MNU_F,           .numItems = sizeof(menu_F          )/sizeof(int16_t), .softkeyItem = menu_F           },
  {.menuId = -MNU_CHI2,        .numItems = sizeof(menu_chi2       )/sizeof(int16_t), .softkeyItem = menu_chi2        },
  {.menuId = -MNU_NORML,       .numItems = sizeof(menu_Norml      )/sizeof(int16_t), .softkeyItem = menu_Norml       },
  {.menuId = -MNU_LGNRM,       .numItems = sizeof(menu_LgNrm      )/sizeof(int16_t), .softkeyItem = menu_LgNrm       },
  {.menuId = -MNU_CAUCH,       .numItems = sizeof(menu_Cauch      )/sizeof(int16_t), .softkeyItem = menu_Cauch       },
  {.menuId = -MNU_EXPON,       .numItems = sizeof(menu_Expon      )/sizeof(int16_t), .softkeyItem = menu_Expon       },
  {.menuId = -MNU_LOGIS,       .numItems = sizeof(menu_Logis      )/sizeof(int16_t), .softkeyItem = menu_Logis       },
  {.menuId = -MNU_WEIBL,       .numItems = sizeof(menu_Weibl      )/sizeof(int16_t), .softkeyItem = menu_Weibl       },
  {.menuId = -MNU_BINOM,       .numItems = sizeof(menu_Binom      )/sizeof(int16_t), .softkeyItem = menu_Binom       },
  {.menuId = -MNU_GEOM,        .numItems = sizeof(menu_Geom       )/sizeof(int16_t), .softkeyItem = menu_Geom        },
  {.menuId = -MNU_HYPER,       .numItems = sizeof(menu_Hyper      )/sizeof(int16_t), .softkeyItem = menu_Hyper       },
  {.menuId = -MNU_NBIN,        .numItems = sizeof(menu_Nbin       )/sizeof(int16_t), .softkeyItem = menu_Nbin        },
  {.menuId = -MNU_POISS,       .numItems = sizeof(menu_Poiss      )/sizeof(int16_t), .softkeyItem = menu_Poiss       },
  {.menuId = -MNU_PFN,         .numItems = sizeof(menu_PFN        )/sizeof(int16_t), .softkeyItem = menu_PFN         },
  {.menuId = -MNU_PFN2,        .numItems = sizeof(menu_PFN2       )/sizeof(int16_t), .softkeyItem = menu_PFN2        },
  {.menuId = -MNU_STAT,        .numItems = sizeof(menu_STAT       )/sizeof(int16_t), .softkeyItem = menu_STAT        },
  {.menuId = -MNU_SUMS,        .numItems = sizeof(menu_SUMS       )/sizeof(int16_t), .softkeyItem = menu_SUMS        },
  {.menuId = -MNU_STK,         .numItems = sizeof(menu_STK        )/sizeof(int16_t), .softkeyItem = menu_STK         },
  {.menuId = -MNU_TEST,        .numItems = sizeof(menu_TEST       )/sizeof(int16_t), .softkeyItem = menu_TEST        },
  {.menuId = -MNU_XFN,         .numItems = sizeof(menu_XFN        )/sizeof(int16_t), .softkeyItem = menu_XFN         },
  {.menuId = -MNU_ORTHOG,      .numItems = sizeof(menu_Orthog     )/sizeof(int16_t), .softkeyItem = menu_Orthog      },
  {.menuId = -MNU_ALPHAINTL,   .numItems = sizeof(menu_alpha_INTL )/sizeof(int16_t), .softkeyItem = menu_alpha_INTL  },
  {.menuId = -MNU_ALPHAintl,   .numItems = sizeof(menu_alpha_intl )/sizeof(int16_t), .softkeyItem = menu_alpha_intl  },
  {.menuId = -MNU_CATALOG,     .numItems = sizeof(menu_CATALOG    )/sizeof(int16_t), .softkeyItem = menu_CATALOG     },
  {.menuId = -MNU_DIGITS,      .numItems = sizeof(menu_DIGITS     )/sizeof(int16_t), .softkeyItem = menu_DIGITS      },
  {.menuId = -MNU_CHARS,       .numItems = sizeof(menu_CHARS      )/sizeof(int16_t), .softkeyItem = menu_CHARS       },
  {.menuId = -MNU_PROGS,       .numItems = sizeof(menu_PROGS      )/sizeof(int16_t), .softkeyItem = menu_PROGS       },
  {.menuId = -MNU_VARS,        .numItems = sizeof(menu_VARS       )/sizeof(int16_t), .softkeyItem = menu_VARS        },
  {.menuId = -MNU_VAR,         .numItems = sizeof(menu_VAR        )/sizeof(int16_t), .softkeyItem = menu_VAR         },
  {.menuId = -MNU_A_Z,         .numItems = sizeof(menu_A_Z        )/sizeof(int16_t), .softkeyItem = menu_A_Z         },
  {.menuId = -MNU_a_z,         .numItems = sizeof(menu_a_z        )/sizeof(int16_t), .softkeyItem = menu_a_z         },
  {.menuId = -MNU_ALPHA_OMEGA, .numItems = sizeof(menu_ALPHA_OMEGA)/sizeof(int16_t), .softkeyItem = menu_ALPHA_OMEGA },
  {.menuId = -MNU_alpha_omega, .numItems = sizeof(menu_alpha_omega)/sizeof(int16_t), .softkeyItem = menu_alpha_omega },
  {.menuId = -MNU_FCNS,        .numItems = sizeof(menu_FCNS       )/sizeof(int16_t), .softkeyItem = menu_FCNS        },
  {.menuId = -MNU_MENUS,       .numItems = sizeof(menu_MENUS      )/sizeof(int16_t), .softkeyItem = menu_MENUS       },
  {.menuId = -MNU_MATRS,       .numItems = sizeof(menu_MATRS      )/sizeof(int16_t), .softkeyItem = menu_MATRS       },
  {.menuId = -MNU_STRING,      .numItems = sizeof(menu_STRING     )/sizeof(int16_t), .softkeyItem = menu_STRING      },
  {.menuId = -MNU_RAM,         .numItems = sizeof(menu_RAM        )/sizeof(int16_t), .softkeyItem = menu_RAM         },
  {.menuId = -MNU_FLASH,       .numItems = sizeof(menu_FLASH      )/sizeof(int16_t), .softkeyItem = menu_FLASH       },
  {.menuId = -MNU_DATES,       .numItems = sizeof(menu_DATES      )/sizeof(int16_t), .softkeyItem = menu_DATES       },
  {.menuId = -MNU_TIMES,       .numItems = sizeof(menu_TIMES      )/sizeof(int16_t), .softkeyItem = menu_TIMES       },
  {.menuId = -MNU_ANGLES,      .numItems = sizeof(menu_ANGLES     )/sizeof(int16_t), .softkeyItem = menu_ANGLES      },
  {.menuId = -MNU_SINTS,       .numItems = sizeof(menu_SINTS      )/sizeof(int16_t), .softkeyItem = menu_SINTS       },
  {.menuId = -MNU_LINTS,       .numItems = sizeof(menu_LINTS      )/sizeof(int16_t), .softkeyItem = menu_LINTS       },
  {.menuId = -MNU_SYSFL,       .numItems = sizeof(menu_SYSFL      )/sizeof(int16_t), .softkeyItem = menu_SYSFL       },
  {.menuId = -MNU_ALPHAMATH,   .numItems = sizeof(menu_alphaMATH  )/sizeof(int16_t), .softkeyItem = menu_alphaMATH   },
  {.menuId = -MNU_ALPHADOT,    .numItems = sizeof(menu_alphaDot   )/sizeof(int16_t), .softkeyItem = menu_alphaDot    },
  {.menuId = -MNU_REALS,       .numItems = sizeof(menu_REALS      )/sizeof(int16_t), .softkeyItem = menu_REALS       },
  {.menuId = -MNU_ALPHAFN,     .numItems = sizeof(menu_alphaFN    )/sizeof(int16_t), .softkeyItem = menu_alphaFN     },
  {.menuId = -MNU_CPXS,        .numItems = sizeof(menu_CPXS       )/sizeof(int16_t), .softkeyItem = menu_CPXS        },
  {.menuId = -MNU_ANGLECONV,   .numItems = sizeof(menu_AngleConv  )/sizeof(int16_t), .softkeyItem = menu_AngleConv   },
  {.menuId = -MNU_UNITCONV,    .numItems = sizeof(menu_UnitConv   )/sizeof(int16_t), .softkeyItem = menu_UnitConv    },
  {.menuId = -MNU_CONVE,       .numItems = sizeof(menu_ConvE      )/sizeof(int16_t), .softkeyItem = menu_ConvE       },
  {.menuId = -MNU_CONVP,       .numItems = sizeof(menu_ConvP      )/sizeof(int16_t), .softkeyItem = menu_ConvP       },
  {.menuId = -MNU_CONVFP,      .numItems = sizeof(menu_ConvFP     )/sizeof(int16_t), .softkeyItem = menu_ConvFP      },
  {.menuId = -MNU_CONVM,       .numItems = sizeof(menu_ConvM      )/sizeof(int16_t), .softkeyItem = menu_ConvM       },
  {.menuId = -MNU_CONVX,       .numItems = sizeof(menu_ConvX      )/sizeof(int16_t), .softkeyItem = menu_ConvX       },
  {.menuId = -MNU_CONVV,       .numItems = sizeof(menu_ConvV      )/sizeof(int16_t), .softkeyItem = menu_ConvV       },
  {.menuId = -MNU_CONVA,       .numItems = sizeof(menu_ConvA      )/sizeof(int16_t), .softkeyItem = menu_ConvA       },
  {.menuId = -MNU_CONST,       .numItems = sizeof(menu_CONST      )/sizeof(int16_t), .softkeyItem = menu_CONST       },
  {.menuId = -MNU_IO,          .numItems = sizeof(menu_IO         )/sizeof(int16_t), .softkeyItem = menu_IO          },
  {.menuId = -MNU_PRINT,       .numItems = sizeof(menu_PRINT      )/sizeof(int16_t), .softkeyItem = menu_PRINT       },
  {.menuId = -MNU_TAM,         .numItems = sizeof(menu_Tam        )/sizeof(int16_t), .softkeyItem = menu_Tam         },
  {.menuId = -MNU_TAMCMP,      .numItems = sizeof(menu_TamCmp     )/sizeof(int16_t), .softkeyItem = menu_TamCmp      },
  {.menuId = -MNU_TAMFLAG,     .numItems = sizeof(menu_TamFlag    )/sizeof(int16_t), .softkeyItem = menu_TamFlag     },
  {.menuId = -MNU_TAMSTORCL,   .numItems = sizeof(menu_TamStoRcl  )/sizeof(int16_t), .softkeyItem = menu_TamStoRcl   },
  {.menuId = -MNU_ASN_N,       .numItems = sizeof(menu_ASN_N      )/sizeof(int16_t), .softkeyItem = menu_ASN_N       },  //JM USER NORMAL
  {.menuId = -MNU_ASN,         .numItems = sizeof(menu_ASN        )/sizeof(int16_t), .softkeyItem = menu_ASN         },  //JM USER
  {.menuId = -MNU_HOME,        .numItems = sizeof(menu_HOME       )/sizeof(int16_t), .softkeyItem = menu_HOME        },  //JM HOME
  {.menuId = -MNU_GRAPH,       .numItems = sizeof(menu_GRAPH      )/sizeof(int16_t), .softkeyItem = menu_GRAPH       },  //JM GRAPH
  {.menuId = -MNU_ST_GRAPH,    .numItems = sizeof(menu_ST_GRAPH   )/sizeof(int16_t), .softkeyItem = menu_ST_GRAPH    },  //JM GRAPH
  {.menuId = -MNU_ALPHA,       .numItems = sizeof(menu_ALPHA      )/sizeof(int16_t), .softkeyItem = menu_ALPHA       },  //JM ALPHA
  {.menuId = -MNU_BASE,        .numItems = sizeof(menu_BASE       )/sizeof(int16_t), .softkeyItem = menu_BASE        },  //JM BASE
  {.menuId = -MNU_EE,          .numItems = sizeof(menu_EE         )/sizeof(int16_t), .softkeyItem = menu_EE          },  //JM EE
#ifdef INLINE_TEST                                                              //vv dr
  {.menuId = -MNU_INL_TST,     .numItems = sizeof(menu_Inl_Tst    )/sizeof(int16_t), .softkeyItem = menu_Inl_Tst     },
#endif                                                                          //^^
  {.menuId =  0,               .numItems = 0,                                        .softkeyItem = NULL             }
};



#ifndef TESTSUITE_BUILD
//vv EXTRA DRAWINGS FOR RADIO_BUTTON AND CHECK_BOX
#define JM_LINE2_DRAW
#undef JM_LINE2_DRAW
#ifdef JM_LINE2_DRAW
void JM_LINE2(int16_t xx, int16_t yy) {                          // To draw the lines for radiobutton on screen
  int16_t x, y;
  y = yy-3-1;
  for(x=xx-66+1; x<min(xx-1,SCREEN_WIDTH); x++) {
    if(mod(x, 2) == 0) {
      setPixel(x, y);
      setPixel(x, y+2);
    }
    else {
      setPixel(x, y+1);
    }
  }
}
#endif


/*
void JM_DOT2(int16_t xx, int16_t yy) {                          // To draw the dots for radiobutton on screen

  setPixel (xx+5,yy+9);
  setPixel (xx+6,yy+9);
  setPixel (xx+7,yy+9);
  setPixel (xx+8,yy+8);
  setPixel (xx+9,yy+7);
  setPixel (xx+9,yy+6);
  setPixel (xx+9,yy+5);
  setPixel (xx+9,yy+4);
  setPixel (xx+9,yy+3);
  setPixel (xx+8,yy+2);
  setPixel (xx+7,yy+1);
  setPixel (xx+6,yy+1);
  setPixel (xx+5,yy+1);
  setPixel (xx+4,yy+1);
  setPixel (xx+3,yy+1);
  setPixel (xx+2,yy+2);
  setPixel (xx+1,yy+3);
  setPixel (xx+1,yy+4);
  setPixel (xx+1,yy+5);
  setPixel (xx+1,yy+6);
  setPixel (xx+1,yy+7);
  setPixel (xx+2,yy+8);
  setPixel (xx+3,yy+9);
  setPixel (xx+4,yy+9);
  setPixel (xx+5,yy+8);
  setPixel (xx+6,yy+8);
  setPixel (xx+7,yy+8);
  setPixel (xx+7,yy+7);
  setPixel (xx+8,yy+7);
  setPixel (xx+8,yy+6);
  setPixel (xx+8,yy+5);
  setPixel (xx+8,yy+4);
  setPixel (xx+8,yy+3);
  setPixel (xx+7,yy+3);
  setPixel (xx+7,yy+2);
  setPixel (xx+6,yy+2);
  setPixel (xx+5,yy+2);
  setPixel (xx+4,yy+2);
  setPixel (xx+3,yy+2);
  setPixel (xx+3,yy+3);
  setPixel (xx+2,yy+3);
  setPixel (xx+2,yy+4);
  setPixel (xx+2,yy+5);
  setPixel (xx+2,yy+6);
  setPixel (xx+2,yy+7);
  setPixel (xx+3,yy+7);
  setPixel (xx+3,yy+8);
  setPixel (xx+4,yy+8);
  clearPixel (xx+5,yy+7);
  clearPixel (xx+6,yy+7);
//clearPixel (xx+6,yy+6);
  clearPixel (xx+7,yy+6);
  clearPixel (xx+7,yy+5);
  clearPixel (xx+7,yy+4);
//clearPixel (xx+6,yy+4);
  clearPixel (xx+6,yy+3);
  clearPixel (xx+5,yy+3);
  clearPixel (xx+4,yy+3);
//clearPixel (xx+4,yy+4);
  clearPixel (xx+3,yy+4);
  clearPixel (xx+3,yy+5);
  clearPixel (xx+3,yy+6);
//clearPixel (xx+4,yy+6);
  clearPixel (xx+4,yy+7);
//clearPixel (xx+5,yy+6);
//clearPixel (xx+6,yy+5);
//clearPixel (xx+5,yy+4);
//clearPixel (xx+4,yy+5);
  setPixel (xx+5,yy+5);
  setPixel (xx+5,yy+6);
  setPixel (xx+6,yy+6);
  setPixel (xx+6,yy+5);
  setPixel (xx+6,yy+4);
  setPixel (xx+5,yy+4);
  setPixel (xx+4,yy+4);
  setPixel (xx+4,yy+5);
  setPixel (xx+4,yy+6);
}
*/


#define RB_EXTRA_BORDER
//#undef RB_EXTRA_BORDER
#define RB_CLEAR_CENTER
#undef RB_CLEAR_CENTER
#ifdef RB_EXTRA_BORDER
void rbColumnCcccccc(int16_t xx, int16_t yy) {
  clearPixel (xx,yy+8);
  clearPixel (xx,yy+7);
  clearPixel (xx,yy+6);
  clearPixel (xx,yy+5);
  clearPixel (xx,yy+4);
  clearPixel (xx,yy+3);
  clearPixel (xx,yy+2);
}
#endif



void rbColumnCcSssssCc(int16_t xx, int16_t yy) {
#ifdef RB_EXTRA_BORDER
  clearPixel (xx,yy+9);
  clearPixel (xx,yy+8);
#endif
  setPixel (xx,yy+7);
  setPixel (xx,yy+6);
  setPixel (xx,yy+5);
  setPixel (xx,yy+4);
  setPixel (xx,yy+3);
#ifdef RB_EXTRA_BORDER
  clearPixel (xx,yy+2);
  clearPixel (xx,yy+1);
#endif
}



void rbColumnCcSssssssCc(int16_t xx, int16_t yy) {
#ifdef RB_EXTRA_BORDER
  clearPixel (xx,yy+10);
  clearPixel (xx,yy+9);
#endif
  setPixel (xx,yy+8);
  setPixel (xx,yy+7);
  setPixel (xx,yy+6);
  setPixel (xx,yy+5);
  setPixel (xx,yy+4);
  setPixel (xx,yy+3);
  setPixel (xx,yy+2);
#ifdef RB_EXTRA_BORDER
  clearPixel (xx,yy+1);
  clearPixel (xx,yy+0);
#endif
}



void rbColumnCSssCccSssC(int16_t xx, int16_t yy) {
#ifdef RB_EXTRA_BORDER
  clearPixel (xx,yy+10);
#endif
  setPixel (xx,yy+9);
  setPixel (xx,yy+8);
  setPixel (xx,yy+7);
  clearPixel (xx,yy+6);
  clearPixel (xx,yy+5);
  clearPixel (xx,yy+4);
  setPixel (xx,yy+3);
  setPixel (xx,yy+2);
  setPixel (xx,yy+1);
#ifdef RB_EXTRA_BORDER
  clearPixel (xx,yy+0);
#endif
}



void rbColumnCSsCSssCSsC(int16_t xx, int16_t yy) {
#ifdef RB_EXTRA_BORDER
  clearPixel (xx,yy+10);
#endif
  setPixel (xx,yy+9);
  setPixel (xx,yy+8);
  clearPixel (xx,yy+7);
  setPixel (xx,yy+6);
  setPixel (xx,yy+5);
  setPixel (xx,yy+4);
  clearPixel (xx,yy+3);
  setPixel (xx,yy+2);
  setPixel (xx,yy+1);
#ifdef RB_EXTRA_BORDER
  clearPixel (xx,yy+0);
#endif
}


void rbColumnCcSsNnnSsCc(int16_t xx, int16_t yy) {
#ifdef RB_EXTRA_BORDER
  clearPixel (xx,yy+10);
  clearPixel (xx,yy+9);
#endif
  setPixel (xx,yy+8);
  setPixel (xx,yy+7);
#ifdef RB_CLEAR_CENTER
  clearPixel (xx,yy+6);
  clearPixel (xx,yy+5);
  clearPixel (xx,yy+4);
#endif
  setPixel (xx,yy+3);
  setPixel (xx,yy+2);
#ifdef RB_EXTRA_BORDER
  clearPixel (xx,yy+1);
  clearPixel (xx,yy+0);
#endif
}



void rbColumnCSsNnnnnSsC(int16_t xx, int16_t yy) {
#ifdef RB_EXTRA_BORDER
  clearPixel (xx,yy+10);
#endif
  setPixel (xx,yy+9);
  setPixel (xx,yy+8);
#ifdef RB_CLEAR_CENTER
  clearPixel (xx,yy+7);
  clearPixel (xx,yy+6);
  clearPixel (xx,yy+5);
  clearPixel (xx,yy+4);
  clearPixel (xx,yy+3);
#endif
  setPixel (xx,yy+2);
  setPixel (xx,yy+1);
#ifdef RB_EXTRA_BORDER
  clearPixel (xx,yy+0);
#endif
}



void rbColumnCSNnnnnnnSC(int16_t xx, int16_t yy) {
#ifdef RB_EXTRA_BORDER
  clearPixel (xx,yy+10);
#endif
  setPixel (xx,yy+9);
#ifdef RB_CLEAR_CENTER
  clearPixel (xx,yy+8);
  clearPixel (xx,yy+7);
  clearPixel (xx,yy+6);
  clearPixel (xx,yy+5);
  clearPixel (xx,yy+4);
  clearPixel (xx,yy+3);
  clearPixel (xx,yy+2);
#endif
  setPixel (xx,yy+1);
#ifdef RB_EXTRA_BORDER
  clearPixel (xx,yy+0);
#endif
}



#ifdef RB_EXTRA_BORDER
void cbColumnCcccccccccc(int16_t xx, int16_t yy) {
  clearPixel (xx,yy+10);
  clearPixel (xx,yy+9);
  clearPixel (xx,yy+8);
  clearPixel (xx,yy+7);
  clearPixel (xx,yy+6);
  clearPixel (xx,yy+5);
  clearPixel (xx,yy+4);
  clearPixel (xx,yy+3);
  clearPixel (xx,yy+2);
  clearPixel (xx,yy+1);
  clearPixel (xx,yy+0);
}
#endif



void cbColumnCSssssssssC(int16_t xx, int16_t yy) {
#ifdef RB_EXTRA_BORDER
  clearPixel (xx,yy+10);
#endif
  setPixel (xx,yy+9);
  setPixel (xx,yy+8);
  setPixel (xx,yy+7);
  setPixel (xx,yy+6);
  setPixel (xx,yy+5);
  setPixel (xx,yy+4);
  setPixel (xx,yy+3);
  setPixel (xx,yy+2);
  setPixel (xx,yy+1);
#ifdef RB_EXTRA_BORDER
  clearPixel (xx,yy+0);
#endif
}



void cbColumnCSsCccccSsC(int16_t xx, int16_t yy) {
#ifdef RB_EXTRA_BORDER
  clearPixel (xx,yy+10);
#endif
  setPixel (xx,yy+9);
  setPixel (xx,yy+8);
  clearPixel (xx,yy+7);
  clearPixel (xx,yy+6);
  clearPixel (xx,yy+5);
  clearPixel (xx,yy+4);
  clearPixel (xx,yy+3);
  setPixel (xx,yy+2);
  setPixel (xx,yy+1);
#ifdef RB_EXTRA_BORDER
  clearPixel (xx,yy+0);
#endif
}



void cbColumnCSNnnnnnnSC(int16_t xx, int16_t yy) {
#ifdef RB_EXTRA_BORDER
  clearPixel (xx,yy+10);
#endif
  setPixel (xx,yy+9);
#ifdef RB_CLEAR_CENTER
  clearPixel (xx,yy+8);
  clearPixel (xx,yy+7);
  clearPixel (xx,yy+6);
  clearPixel (xx,yy+5);
  clearPixel (xx,yy+4);
  clearPixel (xx,yy+3);
  clearPixel (xx,yy+2);
#endif
  setPixel (xx,yy+1);
#ifdef RB_EXTRA_BORDER
  clearPixel (xx,yy+0);
#endif
}



void RB_CHECKED(int16_t xx, int16_t yy) {
#ifdef RB_EXTRA_BORDER
  rbColumnCcccccc(xx+0, yy);
#endif
  rbColumnCcSssssCc(xx+1, yy);
  rbColumnCcSssssssCc(xx+2, yy);
  rbColumnCSssCccSssC(xx+3, yy);
  rbColumnCSsCSssCSsC(xx+4, yy);
  rbColumnCSsCSssCSsC(xx+5, yy);
  rbColumnCSsCSssCSsC(xx+6, yy);
  rbColumnCSssCccSssC(xx+7, yy);
  rbColumnCcSssssssCc(xx+8, yy);
  rbColumnCcSssssCc(xx+9, yy);
//#ifdef RB_EXTRA_BORDER
//  rbColumnCcccccc(xx+10, yy);
//#endif
}



void RB_UNCHECKED(int16_t xx, int16_t yy) {
#ifdef RB_EXTRA_BORDER
  rbColumnCcccccc(xx+0, yy);
#endif
  rbColumnCcSssssCc(xx+1, yy);
  rbColumnCcSsNnnSsCc(xx+2, yy);
  rbColumnCSsNnnnnSsC(xx+3, yy);
  rbColumnCSNnnnnnnSC(xx+4, yy);
  rbColumnCSNnnnnnnSC(xx+5, yy);
  rbColumnCSNnnnnnnSC(xx+6, yy);
  rbColumnCSsNnnnnSsC(xx+7, yy);
  rbColumnCcSsNnnSsCc(xx+8, yy);
  rbColumnCcSssssCc(xx+9, yy);
//#ifdef RB_EXTRA_BORDER
//  rbColumnCcccccc(xx+10, yy);
//#endif
}



void CB_CHECKED(int16_t xx, int16_t yy) {
#ifdef RB_EXTRA_BORDER
  cbColumnCcccccccccc(xx+0, yy);
#endif
  cbColumnCSssssssssC(xx+1, yy);
  cbColumnCSssssssssC(xx+2, yy);
  cbColumnCSsCccccSsC(xx+3, yy);
  rbColumnCSsCSssCSsC(xx+4, yy);
  rbColumnCSsCSssCSsC(xx+5, yy);
  rbColumnCSsCSssCSsC(xx+6, yy);
  cbColumnCSsCccccSsC(xx+7, yy);
  cbColumnCSssssssssC(xx+8, yy);
  cbColumnCSssssssssC(xx+9, yy);
//#ifdef RB_EXTRA_BORDER
//  cbColumnCcccccccccc(xx+10, yy);
//#endif
}



void CB_UNCHECKED(int16_t xx, int16_t yy) {
#ifdef RB_EXTRA_BORDER
  cbColumnCcccccccccc(xx+0, yy);
#endif
  cbColumnCSssssssssC(xx+1, yy);
  cbColumnCSNnnnnnnSC(xx+2, yy);
  cbColumnCSNnnnnnnSC(xx+3, yy);
  cbColumnCSNnnnnnnSC(xx+4, yy);
  cbColumnCSNnnnnnnSC(xx+5, yy);
  cbColumnCSNnnnnnnSC(xx+6, yy);
  cbColumnCSNnnnnnnSC(xx+7, yy);
  cbColumnCSNnnnnnnSC(xx+8, yy);
  cbColumnCSssssssssC(xx+9, yy);
//#ifdef RB_EXTRA_BORDER
//  cbColumnCcccccccccc(xx+10, yy);
//#endif
}
//^^


/********************************************//**
 * \brief Displays one softkey
 *
 * \param[in] label const char*     Text to display
 * \param[in] xSoftkey int16_t      x location of softkey: from 0 (left) to 5 (right)
 * \param[in] ySoftKey int16_t      y location of softkey: from 0 (bottom) to 2 (top)
 * \param[in] videoMode videoMode_t Video mode normal or reverse
 * \param[in] topLineDotted bool_t  Is the item's top line dotted
 * \param[in] topLine bool_t        Draw a top line
 * \param[in] bottomLine bool_t     Draw a bottom line
 * \return void
 ***********************************************/
void showSoftkey(const char *label, int16_t xSoftkey, int16_t ySoftKey, videoMode_t videoMode, bool_t topLine, bool_t bottomLine, int8_t showCb, int16_t showValue) {     //dr
  int16_t x, y, x1, y1, x2, y2;
  int16_t w;

  if(0 <= xSoftkey && xSoftkey <= 5) {
    x1 = 67 * xSoftkey - 1;
    x2 = x1 + 67;
  }
  else {
    sprintf(errorMessage, "In function showSoftkey: xSoftkey=%" FMT16S " must be from 0 to 5" , xSoftkey);
    displayBugScreen(errorMessage);
    return;
  }

  if(0 <= ySoftKey && ySoftKey <= 2) {
    y1 = 217 - SOFTMENU_HEIGHT * ySoftKey;
    y2 = y1 + SOFTMENU_HEIGHT;
  }
  else {
    sprintf(errorMessage, "In function showSoftkey: ySoftKey=%" FMT16S " but must be from 0 to 2!" , ySoftKey);
    displayBugScreen(errorMessage);
    return;
  }

  // Draw the frame
  //   Top line
  for(x=max(0,x1); x<min(x2,SCREEN_WIDTH); x++) {
    if(videoMode == vmNormal) {
      if(topLine) {
        setPixel(x, y1);
      }
    }
    else {
      clearPixel(x, y1);
    }
  }

  //   Bottom line
  if(y1 + SOFTMENU_HEIGHT <= min(y2, 239)) {
    y = y1 + SOFTMENU_HEIGHT;
    for(x=max(0,x1); x<min(x2,SCREEN_WIDTH); x++) {
      if(videoMode == vmNormal) {
        if(bottomLine) {
          setPixel(x, y);
        }
      }
      else {
        clearPixel(x, y);
      }
    }
  }

  //   Left line
  if(x1 >= 0) {
    for(y=y1; y<=min(y2,SCREEN_HEIGHT-1); y++) {
      if(videoMode == vmNormal) {
        setPixel(x1, y);
      }
      else {
        clearPixel(x1, y);
      }
    }
  }

  //   Right line
  if(x2 < SCREEN_WIDTH) {
    for(y=y1; y<=min(y2,SCREEN_HEIGHT-1); y++) {
      if(videoMode == vmNormal) {
        setPixel(x2, y);
      }
      else {
        clearPixel(x2, y);
      }
    }
  }

  // Clear inside the frame
  for(y=y1+1; y<min(y2,SCREEN_HEIGHT); y++) {
    for(x=x1+1; x<min(x2,SCREEN_WIDTH); x++) {
      if(videoMode == vmNormal) {
        clearPixel(x, y);
      }
      else {
        setPixel(x, y);
      }
    }
  }

  w = stringWidth(figlabel(label, showValue), &standardFont, false, false);                      //JM & dr vv
  if((showCb >= 0) || (w >= 50)) {
    compressWidth = 1;         //JM compressWidth
    w = stringWidth(figlabel(label, showValue), &standardFont, false, false);
    compressWidth = 0;         //JM compressWidth
    if(showCb >= 0) { w = w + 8; }
    compressString = 1;       //JM compressString
    showString(figlabel(label, showValue), &standardFont, x1 + (xSoftkey == 5 ? 33 : 34) - w/2, y1 + 2, videoMode, false, false);
    compressString = 0;       //JM compressString
  }
  else {
//  w = stringWidth(label, &standardFont, false, false);
     showString(figlabel(label, showValue), &standardFont, x1 + (xSoftkey == 5 ? 33 : 34) - w/2, y1 + 2, videoMode, false, false);
  }                                                                                              //JM & dr ^^

//  w = stringWidth(label, &standardFont, false, false);
//  if(showCb >= 0) { compressString = 1; w = w +2; }         //JM compressString
//  showString(label, &standardFont, x1 + 33 - w/2, y1 + 2, videoMode, false, false);
//  if(showCb >= 0) { compressString = 0; }                   //JM unCompressString

#ifdef JM_LINE2_DRAW
  if(showCb >= 0) {
    if(videoMode == vmNormal) {
      JM_LINE2(x2, y2);
    }
  }
#endif

  //vv EXTRA DRAWINGS FOR RADIO_BUTTON AND CHECK_BOX
  if(showCb >= 0) {
    if(videoMode == vmNormal) {
      if(showCb == RB_FALSE) {
        RB_UNCHECKED(x2-11, y2-16);
      }
      else if(showCb == RB_TRUE) {
        RB_CHECKED(x2-11, y2-16);
      }
      else if(showCb == CB_TRUE) {
        CB_CHECKED(x2-11, y2-16);
      }
      else {
        CB_UNCHECKED(x2-11, y2-16);
      }
    }
  }
  //^^
}


/********************************************//**
 * \brief Displays the current part of a softmenu
 *
 * \param void
 * \return void
 ***********************************************/
void showSoftmenuCurrentPart(void) {
  int16_t m, x, y, menu, yDotted=0, currentFirstItem, item;
  bool_t dottedTopLine;

  if(softmenuStackPointer > 0) {
    clearScreen(false, false, true);

    m                = softmenuStack[softmenuStackPointer-1].softmenu;
    currentFirstItem = softmenuStack[softmenuStackPointer-1].firstItem;

    if(softmenu[m].numItems <= 18) {
      dottedTopLine = false;
    }
    else {
      dottedTopLine = true;
      yDotted = min(3, (softmenu[m].numItems + modulo(currentFirstItem - softmenu[m].numItems, 6))/6 - currentFirstItem/6) - 1;

      if(   softmenu[m].softkeyItem[6*(currentFirstItem/6+yDotted)  ]==0 && softmenu[m].softkeyItem[6*(currentFirstItem/6+yDotted)+1]==0 && softmenu[m].softkeyItem[6*(currentFirstItem/6+yDotted)+2]==0
         && softmenu[m].softkeyItem[6*(currentFirstItem/6+yDotted)+3]==0 && softmenu[m].softkeyItem[6*(currentFirstItem/6+yDotted)+4]==0 && softmenu[m].softkeyItem[6*(currentFirstItem/6+yDotted)+5]==0) {
        yDotted--;
      }

      if(   yDotted >= 0
         && softmenu[m].softkeyItem[6*(currentFirstItem/6+yDotted)  ]==0 && softmenu[m].softkeyItem[6*(currentFirstItem/6+yDotted)+1]==0 && softmenu[m].softkeyItem[6*(currentFirstItem/6+yDotted)+2]==0
         && softmenu[m].softkeyItem[6*(currentFirstItem/6+yDotted)+3]==0 && softmenu[m].softkeyItem[6*(currentFirstItem/6+yDotted)+4]==0 && softmenu[m].softkeyItem[6*(currentFirstItem/6+yDotted)+5]==0) {
        yDotted--;
      }

      if(   yDotted >= 0
         && softmenu[m].softkeyItem[6*(currentFirstItem/6+yDotted)  ]==0 && softmenu[m].softkeyItem[6*(currentFirstItem/6+yDotted)+1]==0 && softmenu[m].softkeyItem[6*(currentFirstItem/6+yDotted)+2]==0
         && softmenu[m].softkeyItem[6*(currentFirstItem/6+yDotted)+3]==0 && softmenu[m].softkeyItem[6*(currentFirstItem/6+yDotted)+4]==0 && softmenu[m].softkeyItem[6*(currentFirstItem/6+yDotted)+5]==0) {
       yDotted--;
      }
    }

    ULFL = false;                                   //JM Underline
    ULGL = false;                                   //JM Underline

    const int16_t *softkeyItem = softmenu[m].softkeyItem + currentFirstItem;
    for(y=currentFirstItem/6; y<=min(currentFirstItem/6+2, softmenu[m].numItems/6); y++, softkeyItem+=6) {
      for(x=0; x<6; x++) {
        if(softkeyItem + x >= softmenu[m].softkeyItem + softmenu[m].numItems) {
          item = ITM_NULL;
        }
        else {
          int16_t xx = x + y*6;
          //printf("x:%d y:%d 6y:%d xx:%d menu_A_HOME[xx]=%d menuId=%d currentFirstItem=%d/18=%d --> ",x,y,6*y,xx,menu_A_HOME[xx],softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId,currentFirstItem,currentFirstItem/18);  //JMHOME
          if(  menu_A_HOME[xx] >= 0  &&  softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId == -MNU_HOME) 
          {                                          //JMHOME
            if(menu_A_HOME[xx] < 100) {item = !userModeEnabled ? (kbd_std[menu_A_HOME[xx]    ].primary ) : (kbd_usr[menu_A_HOME[xx]    ].primary );}             else           
            if(menu_A_HOME[xx] < 200) {item = !userModeEnabled ? (kbd_std[menu_A_HOME[xx]-100].fShifted) : (kbd_usr[menu_A_HOME[xx]-100].fShifted);}             else
            if(menu_A_HOME[xx]>= 200) {item = !userModeEnabled ? (kbd_std[menu_A_HOME[xx]-200].gShifted) : (kbd_usr[menu_A_HOME[xx]-200].gShifted);}
            //printf("item (std/usr)=%d \n",item);                              //JMHOME

            if(!userModeEnabled && menu_A_HOME[xx] == 0 && (calcMode == CM_NORMAL || calcMode == CM_NIM) && (Norm_Key_00_VAR != kbd_std[0].primary)){
              item = Norm_Key_00_VAR;
            }

          }                                                                     //JMHOME vv
          else {
            item = softkeyItem[x]; 
            //printf("item (-1)=%d \n",item);                                  //JMHOME
          }                                                                    //JMHOME

        }
        int8_t showCb = fnCbIsSet(item%10000);                                  //dr
        int16_t showValue = fnItemShowValue(item%10000);                        //dr
        if(item < 0) { // softmenu
          menu = 0;
          while(softmenu[menu].menuId != 0) {
            if(softmenu[menu].menuId == item) {
              break;
            }
            menu++;
          }

          if(softmenu[menu].menuId == 0) {
            sprintf(errorMessage, "In function showSoftmenuCurrentPart: softmenu ID %" FMT16S " not found!", item);
            displayBugScreen(errorMessage);
          }
          else {
            if(softmenu[menu].softkeyItem == NULL) {
              sprintf(errorMessage, "In function showSoftmenuCurrentPart: Softmenu ID %" FMT16S " must be generated!", item);
              displayBugScreen(errorMessage);
            }
            else {
#ifdef INLINE_TEST                                                              //vv dr
              if(softmenu[menu].menuId == -MNU_INL_TST) {
                showSoftkey(/*STD_omicron*/STD_SPACE_3_PER_EM, x, y-currentFirstItem/6, vmNormal, false, false, -1, -1);
              }
              else {
#endif                                                                          //^^
              showSoftkey(indexOfItems[-softmenu[menu].menuId].itemSoftmenuName, x, y-currentFirstItem/6, vmReverse, true, true, -1, -1);
#ifdef INLINE_TEST                                                              //vv dr
              }
#endif                                                                          //^^
            }
          }
        }
        else if(item == 9999) {
          showSoftkey(indexOfItems[productSign == PS_DOT ? CHR_CROSS : CHR_DOT].itemSoftmenuName, x, y-currentFirstItem/6, vmNormal, true, true, showCb, showValue);
        }
        else if(item > 0 && indexOfItems[item%10000].itemSoftmenuName[0] != 0) { // softkey
          // item : +10000 -> no top line
          //        +20000 -> no bottom line
          //        +30000 -> neither top nor bottom line
          if(softmenu[m].menuId == -MNU_FCNS) {
            showSoftkey(indexOfItems[item%10000].itemCatalogName,  x, y-currentFirstItem/6, vmNormal, (item/10000)==0 || (item/10000)==2, (item/10000)==0 || (item/10000)==1, showCb, showValue);
          }
          else   //JM vv display i or j properly on display
          if(item == ITM_op_j && complexUnit == CU_J) {
            showSoftkey(STD_j, x, y-currentFirstItem/6, vmNormal, (item/10000)==0 || (item/10000)==2, (item/10000)==0 || (item/10000)==1, showCb, showValue);
          }
          else if(item == ITM_op_j && complexUnit == CU_I) {
            showSoftkey(STD_i, x, y-currentFirstItem/6, vmNormal, (item/10000)==0 || (item/10000)==2, (item/10000)==0 || (item/10000)==1, showCb, showValue);
          }     //JM ^^
          else {
            showSoftkey(indexOfItems[item%10000].itemSoftmenuName, x, y-currentFirstItem/6, vmNormal, (item/10000)==0 || (item/10000)==2, (item/10000)==0 || (item/10000)==1, showCb, showValue);
          }
          if(showValue == ITEM_NOT_CODED) {
            int16_t yStroke = SCREEN_HEIGHT - (y-currentFirstItem/6)*23 - 1;
            for(int16_t xStroke=x*67 + 1 +9 ; xStroke<x*67 + 66 -10; xStroke++) {     //JM mod stroke slash cross out
              if(xStroke%3 == 0) yStroke--;
              invertPixel(xStroke, yStroke -3);                                       //JM mod
            }
          }
        }
      }
    // printf("\n"); //JMHOME
    }
    // printf("\n"); //JMHOME

    if(0 <= yDotted && yDotted <= 2) {
      yDotted = 217 - SOFTMENU_HEIGHT * yDotted;

      if(dottedTopLine) {
        for(x=0; x<SCREEN_WIDTH; x++) {
          if(x%8 < 4) {
            setPixel(x, yDotted);
          }
          else {
            clearPixel(x, yDotted);
          }
        }
      }
    }
  }
}


//JMHOMEDEMO: NOTE REMOVE comments TO MAKE JMHOME DEMO WORK ^^
//        Here a HOME menu config can be loaded from disk
/*void Load_HOME(void) {
  menu_HOME[0] = 744; //pi=744
  menu_HOME[1] = 744; //pi=744
  menu_HOME[2] = 744; //pi=744
}
*/
//JMHOMEDEMO: NOTE REMOVE comments TO MAKE JMHOME DEMO WORK ^^

/********************************************//**
 * \brief Initializes the softmenu stack with a
 * softmenu and displays it's first part
 *
 * \param[in] softmenu int16_t Softmenu number
 * \return void
 ***********************************************/
void initSoftmenuStack(int16_t softmenu) {
  softmenuStack[0].softmenu = softmenu;
  softmenuStack[0].firstItem = alphaSelectionMenu == ASM_CNST ? lastCnstMenuPos : (alphaSelectionMenu == ASM_FCNS ? lastFcnsMenuPos : (alphaSelectionMenu == ASM_MENU ? lastMenuMenuPos : 0));
  softmenuStackPointer = 1;
  showSoftmenuCurrentPart();
}



/********************************************//**
 * \brief Pushes a new softmenu on the softmenu stack
 * and displays it's first part
 *
 * \param[in] softmenu int16_t Softmenu number
 * \return void
 ***********************************************/
void pushSoftmenu(int16_t softmenu) {
  if(softmenuStackPointer < SOFTMENU_STACK_SIZE) {
    softmenuStack[softmenuStackPointer].softmenu = softmenu;
    softmenuStack[softmenuStackPointer].firstItem = alphaSelectionMenu == ASM_CNST ? lastCnstMenuPos : (alphaSelectionMenu == ASM_FCNS ? lastFcnsMenuPos : (alphaSelectionMenu == ASM_MENU ? lastMenuMenuPos : 0));
    softmenuStackPointer++;
    showSoftmenuCurrentPart();
  }
  else {
    displayBugScreen("In function pushSoftmenu: the softmenu stack is full! Please increase the value of #define SOFTMENU_STACK_SIZE in wp43s.h");
  }
}



/********************************************//**
 * \brief Pops a softmenu from the softmenu stack
 * and display it's current part or clear the
 * softmenu area of the screen if there's nothing
 * to pop.
 *
 * \param[in] softmenu int16_t Softmenu number
 * \return void
 ***********************************************/
void popSoftmenu(void) {
  if(softmenuStackPointer > 0) {
    if(alphaSelectionMenu != ASM_NONE) {
      alphaSelectionMenu = ASM_NONE;
      calcModeNormal();
    }
    softmenuStackPointer--;
    if(softmenuStackPointer > 0) {
      showSoftmenuCurrentPart();
    }
    else {
      clearScreen(false, false, true);
    }
  }
  else {
    displayBugScreen("In function popSoftmenu: the softmenu stack is empty, there is no softmenu to pop!");
  }
}



/********************************************//**
 * \brief Displays a softmenu. One of the 2 first
 * parameter must be null and one must not be null
 *
 * \param[in] menu const char* Name of softmenu
 * \param[in] id int16_t       ID of softmenu
 * \param[in] push bool_t      * false: initialize the softmenu stack
 *                             * true: push the softmenu on the stack
 * \return void
 ***********************************************/
void showSoftmenu(const char *menu, int16_t id, bool_t push) {
  int16_t m;

  if(id == -MNU_FCNS) {
    alphaSelectionMenu = ASM_FCNS;
    calcModeAsm();
  }
  else if(id == -MNU_CONST) {
    alphaSelectionMenu = ASM_CNST;
    calcModeAsm();
  }
  else if(id == -MNU_MENUS) {
    alphaSelectionMenu = ASM_MENU;
    calcModeAsm();
  }
  else {
    alphaSelectionMenu = ASM_NONE;
  }

  if((menu != NULL && id != 0) || (menu == NULL && id == 0)) {
    clearScreen(false, false, true);
    displayBugScreen("In function showSoftmenu: one parameter must be 0 and one parameter must not be 0!");
    return;
  }

  m = 0;

  if(id==-MNU_ALPHAINTL && alphaCase==AC_LOWER) { // alphaINTL
    id = -MNU_ALPHAintl;
  }
  else if(id==-MNU_A_Z && alphaCase==AC_LOWER) { // A...Z                                      //JM vv retaining the old menu
    id = -MNU_a_z;
  } //JM^^
  else if(id==-MNU_ALPHA_OMEGA && alphaCase==AC_LOWER) { // alpha...omega
    id = -MNU_alpha_omega;
  }

  if((id==0 && !strcmp(menu, STD_alpha "INTL")) && alphaCase==AC_LOWER) { // alphaINTL
    id = -MNU_ALPHAintl;
  }
  else if((id==0 && !strcmp(menu, "A" STD_ELLIPSIS "Z")) && alphaCase==AC_LOWER) { // A...Z    //JM vv retaining the old menu
    id = -MNU_a_z;
  } //JM^^
  else if((id==0 && !strcmp(menu, STD_ALPHA STD_ELLIPSIS STD_OMEGA)) && alphaCase==AC_LOWER) { // alpha...omega
    id = -MNU_alpha_omega;
  }

  if(id != 0) { // Search by ID
    while(softmenu[m].menuId != 0) {
      if(softmenu[m].menuId == id) {
       break;
      }
      m++;
    }
  }
  else { // Search by name
    while(softmenu[m].menuId != 0) {
      if(strcmp(indexOfItems[-softmenu[m].menuId].itemSoftmenuName, menu) == 0) {
        break;
      }
      m++;
    }
  }
  if(softmenu[m].menuId == 0) {
    if(menu == NULL) {
      sprintf(errorMessage, "In function showSoftmenu: softmenu %" FMT16S " not found!", id);
      displayBugScreen(errorMessage);
    }
    else {
      sprintf(errorMessage, "In function showSoftmenu: softmenu %s not found!", menu);
      displayBugScreen(errorMessage);
    }
  }
  else {
    if(calcMode == CM_NORMAL || calcMode == CM_NIM || calcMode == CM_ASM) {
      if(push) {
        pushSoftmenu(m);
      }
      else {
        initSoftmenuStack(m);
      }
    }
    else if(calcMode == CM_AIM) {
      if(push) {
        pushSoftmenu(m);
      }
      else {
        while(softmenuStack[softmenuStackPointer - 1].softmenu != MY_ALPHA_MENU) {
          softmenuStackPointer--;
        }
        pushSoftmenu(m);
      }
    }
    else if(calcMode == CM_TAM) {
    }
    else {
      sprintf(errorMessage, "In fuction showSoftMenu: %" FMT8U " is an unexpected value for calcMode!", calcMode);
      displayBugScreen(errorMessage);
    }
  }
}



void setCatalogLastPos(void) {
       if(alphaSelectionMenu == ASM_CNST) lastCnstMenuPos = softmenuStack[softmenuStackPointer - 1].firstItem;
  else if(alphaSelectionMenu == ASM_FCNS) lastFcnsMenuPos = softmenuStack[softmenuStackPointer - 1].firstItem;
  else if(alphaSelectionMenu == ASM_MENU) lastMenuMenuPos = softmenuStack[softmenuStackPointer - 1].firstItem;
}
#endif
