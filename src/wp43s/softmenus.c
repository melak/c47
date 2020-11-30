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
const int16_t menu_ADV[]         = { ITM_SOLVE,                     ITM_SLVQ,                   ITM_FQX,                  ITM_PIn,               ITM_SIGMAn,                  -MNU_Sfdx,
                                     ITM_PGMSLV,                    ITM_NULL,                   ITM_FDQX,                 ITM_NULL,              ITM_NULL,                    ITM_PGMINT                    };

const int16_t menu_Sfdx[]        = { ITM_NULL,                      ITM_NULL,                   ITM_ACC,                  ITM_DLIM,              ITM_ULIM,                    ITM_INTEGRAL                  };

const int16_t menu_BITS[]        = { ITM_LOGICALAND,                ITM_LOGICALOR,              ITM_LOGICALXOR,           ITM_LOGICALNOT,        ITM_MASKL,                   ITM_MASKR,
                                     ITM_LOGICALNAND,               ITM_LOGICALNOR,             ITM_LOGICALXNOR,          ITM_MIRROR,            ITM_NULL,                    ITM_ASR,
                                     ITM_SB,                        ITM_BS,                     ITM_NUMB,                 ITM_FB,                ITM_BC,                      ITM_CB,
                                     ITM_SL,                        ITM_RL,                     ITM_RLC,                  ITM_RRC,               ITM_RR,                      ITM_SR,
                                     ITM_LJ,                        ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_RJ,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                     ITM_1COMPL,                    ITM_2COMPL,                 ITM_UNSIGN,               ITM_SIGNMT,            ITM_NULL,                    ITM_WSIZE                     };

const int16_t menu_CLK[]         = { ITM_DATE,                      ITM_toDATE,                 ITM_DATEto,               ITM_WDAY,              ITM_TIME,                    ITM_XtoDATE,
                                     ITM_JtoD,                      ITM_DtoJ,                   ITM_NULL,                 ITM_DAY,               ITM_MONTH,                   ITM_YEAR,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                     ITM_SETTIM,                    ITM_TDISP,                  ITM_SETDAT,               ITM_DMY,               ITM_YMD,                     ITM_MDY,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_JonG                      };

const int16_t menu_CLR[]         = { ITM_CLSIGMA,                   ITM_CLP,                    ITM_CF,                   ITM_CLMENU,            ITM_CLCVAR,                  ITM_CLX,
                                     ITM_CLREGS,                    ITM_CLPALL,                 ITM_CLFALL,               ITM_NULL,              ITM_CLLCD,                   ITM_CLSTK,
                                     ITM_CLALL,                     ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_RESET                     };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_CPX[]         = { ITM_DOT_PROD,                  ITM_CROSS_PROD,             ITM_UNITV,                ITM_RE,                ITM_CONJ,                    ITM_REexIM,
                                     ITM_CXtoRE,                    ITM_REtoCX,                 ITM_SIGN,                 ITM_IM,                ITM_MAGNITUDE,               ITM_ANGLE                     };

const int16_t menu_DISP[]        = { ITM_FIX,                       ITM_SCI,                    ITM_ENG,                  ITM_ALL,               ITM_ROUNDI,                  ITM_ROUND,
                                     ITM_SDL,                       ITM_SDR,                    ITM_NULL,                 ITM_NULL,              ITM_RDP,                     ITM_RSD,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                     ITM_SETCHN,                    ITM_SETEUR,                 ITM_SETIND,               ITM_SETJPN,            ITM_SETUK,                   ITM_SETUSA,
                                     ITM_GAP,                       ITM_NULL,                   ITM_RANGE,                ITM_GETRANGE,          ITM_NULL,                    ITM_DSTACK                    };

const int16_t menu_EQN[]         = { ITM_EQ_NEW,                    ITM_EQ_EDI,                 -MNU_2NDDERIV,            -MNU_1STDERIV,         -MNU_Sf,                     -MNU_Solver,
                                     ITM_EQ_DEL,                    ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

const int16_t menu_EXP[]         = { ITM_CUBE,                      ITM_XTHROOT,                ITM_LOGXY,                ITM_LOG2,              ITM_2X,                      ITM_SQUAREROOTX,
                                     ITM_CUBEROOT,                  ITM_NULL,                   ITM_NULL,                 ITM_LN1X,              ITM_EX1,                     ITM_NULL,
                                     ITM_sinh,                      ITM_arsinh,                 ITM_cosh,                 ITM_arcosh,            ITM_tanh,                    ITM_artanh                    };

const int16_t menu_TRI[]         = { ITM_sin,                       ITM_arcsin,                 ITM_cos,                  ITM_arccos,            ITM_tan,                     ITM_arctan,
                                     ITM_sinc,                      ITM_sincpi,                 ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                     ITM_sinh,                      ITM_arsinh,                 ITM_cosh,                 ITM_arcosh,            ITM_tanh,                    ITM_artanh                    };

const int16_t menu_FIN[]         = { ITM_PC,                        ITM_PCMRR,                  ITM_PCT,                  ITM_PCSIGMA,           ITM_PCPMG,                   -MNU_TVM                      };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_TVM[]         = { ITM_NPER,                      ITM_IPCA,                   ITM_PERonA,               ITM_PV,                ITM_PMT,                     ITM_FV,
                                     ITM_BEGINP,                    ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_ENDP                      };

const int16_t menu_FLAGS[]       = { ITM_SF,                        ITM_FS,                     ITM_FF,                   ITM_STATUS,            ITM_FC,                      ITM_CF,
                                     ITM_FSS,                       ITM_FSC,                    ITM_FSF,                  ITM_FCF,               ITM_FCS,                     ITM_FCC,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_CLFALL                    };

const int16_t menu_INFO[]        = { ITM_SSIZE,                     ITM_MEM,                    ITM_RMQ,                  ITM_ISM,               ITM_WSIZEQ,                  ITM_KTYP,
                                     ITM_LocRQ,                     ITM_FLASH,                  ITM_ULP,                  ITM_NEIGHB,            ITM_SDIGS,                   ITM_BATT,
                                     ITM_WHO,                       ITM_VERS,                   ITM_M_DIMQ,               ITM_PMINFINITY,        ITM_ALPHAPOS,                ITM_ALPHALENG,
                                     ITM_GETRANGE,                  ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_BESTFQ                    };

const int16_t menu_INTS[]        = { ITM_A,                         ITM_B,                      ITM_C,                    ITM_D,                 ITM_E,                       ITM_F,
                                     ITM_IDIV,                      ITM_RMD,                    ITM_MOD,                  ITM_XMOD,              ITM_FLOOR,                   ITM_LCM,
                                     ITM_DBLSLASH,                  ITM_DBLR,                   ITM_DBLCROSS,             ITM_PMOD,              ITM_CEIL,                    ITM_GCD,
                                     ITM_1COMPL,                    ITM_2COMPL,                 ITM_UNSIGN,               ITM_SIGNMT,            ITM_NULL,                    ITM_WSIZE                     };

const int16_t menu_LOOP[]        = { ITM_DSE,                       ITM_DSZ,                    ITM_DSL,                  ITM_ISE,               ITM_ISZ,                     ITM_ISG,
                                     ITM_DEC,                       ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_INC                       };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_MATX[]        = { ITM_M_NEW,                     ITM_M_INV,                  ITM_M_DET,                ITM_M_TRANSP,          ITM_SIM_EQ,                  ITM_M_EDI,
                                     ITM_DOT_PROD,                  ITM_CROSS_PROD,             ITM_UNITV,                ITM_M_DIM,             ITM_INDEX,                   ITM_M_EDIN,
                                     ITM_ENORM,                     ITM_NULL,                   ITM_STOEL,                ITM_RCLEL,             ITM_M_PUT,                   ITM_M_GET,
                                     ITM_IPLUS,                     ITM_IMINUS,                 ITM_STOIJ,                ITM_RCLIJ,             ITM_JMINUS,                  ITM_JPLUS,
                                     ITM_RSUM,                      ITM_RNORM,                  ITM_M_LU,                 ITM_M_DIMQ,            ITM_NULL,                    ITM_M_RR,
                                     ITM_EIGVAL,                    ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_EIGVEC                    };

const int16_t menu_M_SIM_Q[]     = { ITM_MATA,                      ITM_MATB,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_MATX                      };

const int16_t menu_M_EDIT[]      = { ITM_LEFT_ARROW,                ITM_UP_ARROW,               ITM_M_OLD,                ITM_M_GOTO,            ITM_DOWN_ARROW,              ITM_RIGHT_ARROW,
                                     ITM_M_INSR,                    ITM_NULL,                   ITM_M_DELR,               ITM_NULL,              ITM_M_WRAP,                  ITM_M_GROW                    };

const int16_t menu_MODE[]        = { ITM_SF,                        ITM_DEG,                    ITM_RAD,                  ITM_GRAD,              ITM_MULPI,                   ITM_CF,
                                     ITM_NULL,                      ITM_NULL,                   ITM_RM,                   ITM_NULL,              ITM_SETSIG,                  ITM_DENMAX,
#if DMCP_BUILD
                                     ITM_SYSTEM,                    ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
#endif // DMCP_BUILD
                                                                                                                                                                                                            };

const int16_t menu_PARTS[]       = { ITM_IP,                        ITM_FP,                     ITM_MANT,                 ITM_EXPT,              ITM_SIGN,                    ITM_DECOMP,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_MAGNITUDE,               ITM_ANGLE,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_RE,                      ITM_IM                        };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_PROB[]        = { -MNU_NORML,                    -MNU_T,                     ITM_COMB,                 ITM_PERM,              -MNU_F,                      -MNU_CHI2,
                                     -MNU_LGNRM,                    -MNU_CAUCH,                 ITM_NULL,                 -MNU_EXPON,            -MNU_LOGIS,                  -MNU_WEIBL,
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
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_PFN[]         = { ITM_INPUT,                     ITM_END,                    ITM_ERR,                  ITM_TICKS,             ITM_PAUSE,                  -MNU_PFN2,
                                     ITM_PSTO,                      ITM_PRCL,                   ITM_VARMNU,               ITM_MVAR,              ITM_CNST,                    ITM_PUTK,
                                     ITM_R_CLR,                     ITM_R_COPY,                 ITM_R_SORT,               ITM_R_SWAP,            ITM_LocR,                    ITM_POPLR                     };

const int16_t menu_PFN2[]        = { ITM_MENU,                      ITM_KEYG,                   ITM_KEYX,                 ITM_CLMENU,            ITM_EXITALL,                 ITM_RTNP1,
                                     ITM_SDL,                       ITM_SDR,                    ITM_MSG,                  ITM_NOP,               ITM_VARMNU,                  ITM_MVAR,
                                     ITM_BACK,                      ITM_CASE,                   ITM_SKIP,                 ITM_AGRAPH,            ITM_PIXEL,                   ITM_POINT                     };

const int16_t menu_STAT[]        = { ITM_SIGMAPLUS,                 ITM_XBAR,                   ITM_STDDEVWEIGHTED,       ITM_STDDEV,            ITM_SM,                      ITM_SUM,
                                     ITM_SIGMAMINUS,                ITM_XW,                     ITM_SW,                   ITM_STDDEVPOP,         ITM_SMW,                     ITM_NULL,
                                     ITM_CLSIGMA,                   ITM_XG,                     ITM_SCATTFACT,            ITM_SCATTFACTp,        ITM_SCATTFACTm,              ITM_PLOT,
                                     ITM_LR,                        ITM_CORR,                   ITM_SXY,                  ITM_COV,               ITM_XCIRC,                   ITM_YCIRC,
                                     ITM_SA,                        ITM_XH,                     ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                     ITM_NULL,                      ITM_XRMS,                   ITM_XMAX,                 ITM_XMIN,              ITM_NULL,                    ITM_ORTHOF,
                                     ITM_LINF,                      ITM_EXPF,                   ITM_LOGF,                 ITM_POWERF,            ITM_NULL,                    ITM_BESTF,
                                     ITM_GAUSSF,                    ITM_CAUCHF,                 ITM_PARABF,               ITM_HYPF,              ITM_ROOTF,                   ITM_NULL                      };

const int16_t menu_SUMS[]        = { ITM_NSIGMA,                    ITM_SIGMAx,                 ITM_SIGMAx2,              ITM_SIGMAxy,           ITM_SIGMAy2,                 ITM_SIGMAy,
                                     ITM_NULL,                      ITM_SIGMAlnx,               ITM_SIGMAln2x,            ITM_SIGMAlnxy,         ITM_SIGMAln2y,               ITM_SIGMAlny,
                                     ITM_SIGMAx2y,                  ITM_SIGMAxlny,              ITM_NULL,                 ITM_SIGMAlnyonx,       ITM_NULL,                    ITM_SIGMAylnx,
                                     ITM_SIGMAx2ony,                ITM_SIGMA1onx,              ITM_SIGMA1onx2,           ITM_SIGMAxony,         ITM_SIGMA1ony2,              ITM_SIGMA1ony,
                                     ITM_SIGMAx3,                   ITM_SIGMAx4,                ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

const int16_t menu_STK[]         = { ITM_Xex,                       ITM_Yex,                    ITM_Zex,                  ITM_Tex,               ITM_SHUFFLE,                 ITM_DROPY                     };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_TEST[]        = { ITM_XLT,                       ITM_XLE,                    ITM_XEQU,                 ITM_XNE,               ITM_XGE,                     ITM_XGT,
                                     ITM_INT,                       ITM_EVEN,                   ITM_ODD,                  ITM_PRIME,             ITM_LEAP,                    ITM_FPQ,
                                     ITM_ENTRY,                     ITM_KEYQ,                   ITM_LBLQ,                 ITM_STRI,              ITM_CONVG,                   ITM_TOP,
                                     ITM_XEQUP0,                    ITM_XEQUM0,                 ITM_XAEQU,                ITM_MATR,              ITM_CPX,                     ITM_REAL,
                                     ITM_SPEC,                      ITM_NAN,                    ITM_NULL,                 ITM_M_SQR,             ITM_NULL,                    ITM_NULL                      };

const int16_t menu_XFN[]         = { ITM_AGM,                       ITM_BN,                     ITM_BNS,                  ITM_ERF,               ITM_ERFC,                   -MNU_ORTHOG,
                                     ITM_FIB,                       ITM_GD,                     ITM_GDM1,                 ITM_IXYZ,              ITM_IGAMMAP,                 ITM_IGAMMAQ,
                                     ITM_JYX,                       ITM_LNBETA,                 ITM_LNGAMMA,              ITM_MAX,               ITM_MIN,                     ITM_NEXTP,
                                     ITM_WM,                        ITM_WP,                     ITM_WM1,                  ITM_BETAXY,            ITM_gammaXY,                 ITM_GAMMAXY,
                                     ITM_zetaX,                     ITM_M1X,                    ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_PARALLEL                  };

const int16_t menu_Orthog[]      = { ITM_HN,                        ITM_Ln,                     ITM_LNALPHA,              ITM_PN,                ITM_TN,                      ITM_UN,
                                     ITM_HNP,                       ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_CATALOG[]     = { -MNU_FCNS,                     -MNU_SYSFL,                 -MNU_CHARS,               -MNU_PROGS,            -MNU_VARS,                   -MNU_MENUS,
                                     ITM_NULL,                      -MNU_REGIST,                -MNU_DIGITS,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

const int16_t menu_DIGITS[]      = { ITM_0,                         ITM_1,                      ITM_2,                    ITM_3,                 ITM_4,                       ITM_5,
                                     ITM_6,                         ITM_7,                      ITM_8,                    ITM_9,                 ITM_A,                       ITM_B,
                                     ITM_C,                         ITM_D,                      ITM_E,                    ITM_F,                 ITM_i,                       ITM_NULL                      };

const int16_t menu_CHARS[]       = { -MNU_ALPHAINTL,               -MNU_ALPHA_OMEGA,            ITM_NULL,                -MNU_ALPHAMATH,        -MNU_MyAlpha,                -MNU_ALPHADOT                  };

const int16_t menu_PROGS[]       = { -MNU_RAM,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    -MNU_FLASH                    };

const int16_t menu_VARS[]        = { -MNU_LINTS,                    -MNU_SINTS,                 -MNU_REALS,               -MNU_CPXS,             -MNU_STRINGS,                -MNU_MATRS,
                                     -MNU_DATES,                    -MNU_TIMES,                 -MNU_ANGLES,              ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
// Following menu is UPPER CASE for lower case: +36
const int16_t menu_ALPHA_OMEGA[] = { ITM_ALPHA,                     ITM_BETA,                   ITM_GAMMA,                ITM_DELTA,             ITM_EPSILON,                 ITM_ZETA,
                                     ITM_ETA,                       ITM_THETA,                  ITM_IOTA,                 ITM_KAPPA,             ITM_LAMBDA,                  ITM_MU,
                                     ITM_NU,                        ITM_XI,                     ITM_OMICRON,              ITM_PI,                ITM_RHO,                     ITM_SIGMA,
                                     ITM_NULL,                      ITM_TAU,                    ITM_UPSILON,              ITM_PHI,               ITM_CHI,                     ITM_PSI,
                                     ITM_OMEGA,                     ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                     ITM_IOTA_DIALYTIKA,            ITM_NULL,                   ITM_NULL,                 ITM_UPSILON_DIALYTIKA, ITM_NULL,                    ITM_NULL                      };

const int16_t menu_alpha_omega[] = { ITM_alpha,                     ITM_beta,                   ITM_gamma,                ITM_delta,             ITM_epsilon,                 ITM_zeta,
                                     ITM_eta,                       ITM_theta,                  ITM_iota,                 ITM_kappa,             ITM_lambda,                  ITM_mu,
                                     ITM_nu,                        ITM_xi,                     ITM_omicron,              ITM_pi,                ITM_rho,                     ITM_sigma,
                                     ITM_sigma_end,                 ITM_tau,                    ITM_upsilon,              ITM_phi,               ITM_chi,                     ITM_psi,
                                     ITM_omega,                     ITM_alpha_TONOS,            ITM_epsilon_TONOS,        ITM_eta_TONOS,         ITM_iotaTON,                 ITM_iota_DIALYTIKA_TONOS,
                                     ITM_iota_DIALYTIKA,            ITM_omicron_TONOS,          ITM_upsilon_TONOS,        ITM_upsilon_DIALYTIKA, ITM_upsilon_DIALYTIKA_TONOS, ITM_omega_TONOS               };

const int16_t menu_AngleConv[]   = { ITM_toDEG,                     ITM_toRAD,                  ITM_toGRAD,               ITM_NULL,              ITM_toDMS,                   ITM_toMULpi,
                                     ITM_DEGto,                     ITM_RADto,                  ITM_GRADto,               ITM_NULL,              ITM_DMSto,                   ITM_MULPIto,
                                     ITM_DtoR,                      ITM_RtoD,                   ITM_NULL,                 ITM_DtoDMS,            ITM_DMStoD,                  ITM_NULL                      };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_UnitConv[]    = { -MNU_CONVE,                    -MNU_CONVP,                 ITM_YEARtoS,              -MNU_CONVFP,           -MNU_CONVM,                  -MNU_CONVX,
                                     ITM_CtoF,                      ITM_FtoC,                   ITM_StoYEAR,              ITM_NULL,              -MNU_CONVV,                  -MNU_CONVA,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                     10000+ITM_PRtoDBc,             10000+ITM_DBtoPRc,          10000+ITM_NMtoLBFFTb,     10000+ITM_LBFFTtoNMb,  10000+ITM_FRtoDBc,           10000+ITM_DBtoFRc,
                                     30000+ITM_PRtoDBb,             30000+ITM_DBtoPRb,          20000+ITM_NMtoLBFFT,      20000+ITM_LBFFTtoNM,   30000+ITM_FRtoDBb,           30000+ITM_DBtoFRb,
                                     20000+ITM_PRtoDB,              20000+ITM_DBtoPR,           ITM_NULL,                 ITM_NULL,              20000+ITM_FRtoDB,            20000+ITM_DBtoFR              };

const int16_t menu_ConvE[]       = { ITM_CALtoJ,                    ITM_JtoCAL,                 ITM_BTUtoJ,               ITM_JtoBTU,            ITM_WHtoJ,                   ITM_JtoWH                     };

const int16_t menu_ConvP[]       = { ITM_HPEtoW,                    ITM_WtoHPE,                 ITM_HPUKtoW,              ITM_WtoHPUK,           ITM_HPMtoW,                  ITM_WtoHPM                    };

const int16_t menu_ConvFP[]      = { ITM_LBFtoN,                    ITM_NtoLBF,                 ITM_BARtoPA,              ITM_PAtoBAR,           ITM_PSItoPA,                 ITM_PAtoPSI,
                                     10000+ITM_IHGtoPAb,            10000+ITM_PAtoIHGb,         10000+ITM_TORtoPAb,       10000+ITM_PAtoTORb,    10000+ITM_ATMtoPA,           10000+ITM_PAtoATM,
                                     20000+ITM_IHGtoPA,             20000+ITM_PAtoIHG,          20000+ITM_TORtoPA,        20000+ITM_PAtoTOR,     20000+ITM_ATMtoPAb,          20000+ITM_PAtoATMb,
                                     ITM_NULL,                      ITM_NULL,                   10000+ITM_MMHGtoPAb,      10000+ITM_PAtoMMHGb,   ITM_NULL,                    ITM_NULL,
                                     ITM_NULL,                      ITM_NULL,                   20000+ITM_MMHGtoPA,       20000+ITM_PAtoMMHG,    ITM_NULL,                    ITM_NULL                      };

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

const int16_t menu_alphaFN[]     = { ITM_XtoALPHA,                  ITM_ALPHARL,                ITM_ALPHARR,              ITM_ALPHASL,           ITM_ALPHASR,                 ITM_ALPHAtoX,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_ALPHALENG,               ITM_ALPHAPOS,
                                     ITM_FBR,                       ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_alphaMATH[]   = { ITM_LESS_THAN,                 ITM_LESS_EQUAL,             ITM_EQUAL,                ITM_ALMOST_EQUAL,      ITM_GREATER_EQUAL,           ITM_GREATER_THAN,
                                     ITM_LEFT_CURLY_BRACKET,        ITM_LEFT_SQUARE_BRACKET,    ITM_LEFT_PARENTHESIS,     ITM_RIGHT_PARENTHESIS, ITM_RIGHT_SQUARE_BRACKET,    ITM_RIGHT_CURLY_BRACKET,
                                     ITM_PROD_SIGN,                 ITM_OBELUS,                 ITM_INTEGRAL_SIGN,        ITM_SUB_INFINITY,      ITM_SUP_INFINITY,            ITM_INFINITY,
                                     ITM_NOT,                       ITM_AND,                    ITM_OR,                   ITM_NOT_EQUAL,         ITM_PIPE,                    ITM_AMPERSAND,
                                     ITM_MEASURED_ANGLE,            ITM_RIGHT_ANGLE,            ITM_PERPENDICULAR,        ITM_CUBE_ROOT,         ITM_SQUARE_ROOT,             ITM_xTH_ROOT,
                                     ITM_x_BAR,                     ITM_y_BAR,                  ITM_x_CIRC,               ITM_y_CIRC,            ITM_x_UNDER_ROOT,            ITM_y_UNDER_ROOT,
                                     ITM_COLON_EQUALS,              ITM_ESTIMATES,              ITM_CORRESPONDS_TO,       ITM_SUB_E_OUTLINE,     ITM_COMPLEX_C,               ITM_REAL_R,
                                     ITM_SUN,                       ITM_SUB_SUN,                ITM_SUB_EARTH,            ITM_NULL,              ITM_NULL,                    ITM_NULL,
                                     ITM_PLUS_MINUS,                ITM_CIRCUMFLEX,             ITM_SUP_T,                ITM_SUP_MINUS_1,       ITM_PLANCK_2PI,              ITM_NULL                      };

/*      Menu name                  <----------------------------------------------------------------------------- 6 functions ---------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 f shifted functions ------------------------------------------------------------------------->  */
/*                                 <---------------------------------------------------------------------- 6 g shifted functions ------------------------------------------------------------------------->  */
const int16_t menu_alphaDot[]    = { ITM_EXCLAMATION_MARK,          ITM_SEMICOLON,              ITM_UNDERSCORE,           ITM_QUOTE,             ITM_DOUBLE_QUOTE,            ITM_AT,
                                     ITM_INVERTED_EXCLAMATION_MARK, ITM_INVERTED_QUESTION_MARK, ITM_SECTION,              ITM_NULL,              ITM_TILDE,                   ITM_BACK_SLASH,
                                     ITM_DOLLAR,                    ITM_EURO,                   ITM_PERCENT,              ITM_AMPERSAND,         ITM_POUND,                   ITM_YEN,
                                     ITM_LEFT_ARROW,                ITM_UP_ARROW,               ITM_SERIAL_IO,            ITM_DOWN_ARROW,        ITM_RIGHT_ARROW,             ITM_PGM_BEGIN,
                                     ITM_LEFT_DOUBLE_ANGLE,         ITM_RIGHT_DOUBLE_ANGLE,     ITM_EMPTY_SET,            ITM_WATCH,             ITM_BULLET,                  ITM_ASTERISK,
                                     ITM_HOURGLASS,                 ITM_NEG_EXCLAMATION_MARK,   ITM_USER_MODE,            ITM_BATTERY,           ITM_NULL,                    ITM_SUP_ASTERISK              };

const int16_t menu_1stDeriv[]    = { ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_2ndDeriv[]    = { ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_Sf[]          = { ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_Solver[]      = { ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_IO[]          = { ITM_LOAD,                      ITM_LOADP,                  ITM_LOADR,                ITM_LOADSS,            ITM_LOADV,                   ITM_LOADSIGMA,
                                     ITM_BEEP,                      ITM_TONE,                   ITM_NULL,                 ITM_NULL,              ITM_RECV,                    ITM_SEND                      };
const int16_t menu_PRINT[]       = { ITM_PRINTERX,                  ITM_PRINTERR,               ITM_PRINTERSIGMA,         ITM_PRINTERADV,        ITM_PRINTERLCD,              ITM_PRINTERPROG,
                                     ITM_PRINTERSTK,                ITM_PRINTERREGS,            ITM_PRINTERUSER,          ITM_PRINTERTAB,        ITM_PRINTERHASH,             ITM_PRINTERCHAR,
                                     ITM_NULL,                      ITM_NULL,                   ITM_NULL,                 ITM_PRINTERWIDTH,      ITM_PRINTERDLAY,             ITM_PRINTERMODE               };
const int16_t menu_Tam[]         = { ITM_INDIRECTION,               -MNU_VAR,                   ITM_ST_X,                 ITM_ST_Y,              ITM_ST_Z,                    ITM_ST_T                      };
const int16_t menu_TamCmp[]      = { ITM_INDIRECTION,               -MNU_VAR,                   ITM_ST_X,                 ITM_ST_Y,              ITM_ST_Z,                    ITM_ST_T,
                                     ITM_0P,                        ITM_1P,                     ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_TamFlag[]     = { ITM_INDIRECTION,               -MNU_SYSFL,                 ITM_REG_X,                ITM_REG_Y,             ITM_REG_Z,                   ITM_REG_T                     };
const int16_t menu_TamStoRcl[]   = { ITM_INDIRECTION,               -MNU_VAR,                   ITM_ST_X,                 ITM_ST_Y,              ITM_ST_Z,                    ITM_ST_T,
                                     ITM_Config,                    ITM_Stack,                  ITM_NULL,                 ITM_NULL,              ITM_Max,                     ITM_Min,
                                     ITM_dddEL,                     ITM_dddIJ,                  ITM_NULL,                 ITM_NULL,              ITM_NULL,                    ITM_NULL                      };
const int16_t menu_TamShuffle[]  = { ITM_ST_X,                      ITM_ST_Y,                   ITM_ST_Z,                 ITM_ST_T,              ITM_NULL,                    ITM_NULL,                     };
const int16_t menu_TamLabel[]    = { ITM_INDIRECTION,               -MNU_PROG,                  ITM_ST_X,                 ITM_ST_Y,              ITM_ST_Z,                    ITM_ST_T                      };

#include "softmenuCatalogs.h"

const softmenu_t softmenu[] = {
/*   0 */  {.menuItem = -MNU_MyMenu,      .numItems = 0,                                        .softkeyItem = NULL             }, // MyMenu must be the 1st
/*   1 */  {.menuItem = -MNU_MyAlpha,     .numItems = 0,                                        .softkeyItem = NULL             }, // Myalpha must be the 2nd
/*   2 */  {.menuItem = -MNU_RAM,         .numItems = 0,                                        .softkeyItem = NULL             },
/*   3 */  {.menuItem = -MNU_FLASH,       .numItems = 0,                                        .softkeyItem = NULL             }, // The 15 first menus are
/*   4 */  {.menuItem = -MNU_VAR,         .numItems = 0,                                        .softkeyItem = NULL             }, // variable softmenus and
/*   5 */  {.menuItem = -MNU_PROG,        .numItems = 0,                                        .softkeyItem = NULL             }, // MUST be in the same
/*   6 */  {.menuItem = -MNU_MATRS,       .numItems = 0,                                        .softkeyItem = NULL             }, // order as the
/*   7 */  {.menuItem = -MNU_STRINGS,     .numItems = 0,                                        .softkeyItem = NULL             }, // dynamicSoftmenu area.
/*   8 */  {.menuItem = -MNU_DATES,       .numItems = 0,                                        .softkeyItem = NULL             }, //
/*   9 */  {.menuItem = -MNU_TIMES,       .numItems = 0,                                        .softkeyItem = NULL             }, // If you add or remove one:
/*  10 */  {.menuItem = -MNU_ANGLES,      .numItems = 0,                                        .softkeyItem = NULL             }, // don't forget to adjust
/*  11 */  {.menuItem = -MNU_SINTS,       .numItems = 0,                                        .softkeyItem = NULL             }, // NUMBER_OF_DYNAMIC_SOFTMENUS
/*  12 */  {.menuItem = -MNU_LINTS,       .numItems = 0,                                        .softkeyItem = NULL             }, // in defines.h
/*  13 */  {.menuItem = -MNU_REALS,       .numItems = 0,                                        .softkeyItem = NULL             },
/*  14 */  {.menuItem = -MNU_CPXS,        .numItems = 0,                                        .softkeyItem = NULL             },
/*  15 */  {.menuItem = -MNU_TAMFLAG,     .numItems = sizeof(menu_TamFlag    )/sizeof(int16_t), .softkeyItem = menu_TamFlag     },
/*  16 */  {.menuItem = -MNU_SYSFL,       .numItems = sizeof(menu_SYSFL      )/sizeof(int16_t), .softkeyItem = menu_SYSFL       },
/*  17 */  {.menuItem = -MNU_ALPHAINTL,   .numItems = sizeof(menu_alpha_INTL )/sizeof(int16_t), .softkeyItem = menu_alpha_INTL  },
/*  18 */  {.menuItem = -MNU_ALPHAintl,   .numItems = sizeof(menu_alpha_intl )/sizeof(int16_t), .softkeyItem = menu_alpha_intl  },
/*  19 */  {.menuItem = -MNU_ADV,         .numItems = sizeof(menu_ADV        )/sizeof(int16_t), .softkeyItem = menu_ADV         },
/*  20 */  {.menuItem = -MNU_Sfdx,        .numItems = sizeof(menu_Sfdx       )/sizeof(int16_t), .softkeyItem = menu_Sfdx        },
/*  21 */  {.menuItem = -MNU_BITS,        .numItems = sizeof(menu_BITS       )/sizeof(int16_t), .softkeyItem = menu_BITS        },
/*  22 */  {.menuItem = -MNU_CLK,         .numItems = sizeof(menu_CLK        )/sizeof(int16_t), .softkeyItem = menu_CLK         },
/*  23 */  {.menuItem = -MNU_CLR,         .numItems = sizeof(menu_CLR        )/sizeof(int16_t), .softkeyItem = menu_CLR         },
/*  24 */  {.menuItem = -MNU_CPX,         .numItems = sizeof(menu_CPX        )/sizeof(int16_t), .softkeyItem = menu_CPX         },
/*  25 */  {.menuItem = -MNU_DSP,         .numItems = sizeof(menu_DISP       )/sizeof(int16_t), .softkeyItem = menu_DISP        },
/*  26 */  {.menuItem = -MNU_EQN,         .numItems = sizeof(menu_EQN        )/sizeof(int16_t), .softkeyItem = menu_EQN         },
/*  27 */  {.menuItem = -MNU_1STDERIV,    .numItems = sizeof(menu_1stDeriv   )/sizeof(int16_t), .softkeyItem = menu_1stDeriv    },
/*  28 */  {.menuItem = -MNU_2NDDERIV,    .numItems = sizeof(menu_2ndDeriv   )/sizeof(int16_t), .softkeyItem = menu_2ndDeriv    },
/*  29 */  {.menuItem = -MNU_Sf,          .numItems = sizeof(menu_Sf         )/sizeof(int16_t), .softkeyItem = menu_Sf          },
/*  30 */  {.menuItem = -MNU_Solver,      .numItems = sizeof(menu_Solver     )/sizeof(int16_t), .softkeyItem = menu_Solver      },
/*  31 */  {.menuItem = -MNU_EXP,         .numItems = sizeof(menu_EXP        )/sizeof(int16_t), .softkeyItem = menu_EXP         },
/*  32 */  {.menuItem = -MNU_TRI,         .numItems = sizeof(menu_TRI        )/sizeof(int16_t), .softkeyItem = menu_TRI         },
/*  33 */  {.menuItem = -MNU_FIN,         .numItems = sizeof(menu_FIN        )/sizeof(int16_t), .softkeyItem = menu_FIN         },
/*  34 */  {.menuItem = -MNU_TVM,         .numItems = sizeof(menu_TVM        )/sizeof(int16_t), .softkeyItem = menu_TVM         },
/*  35 */  {.menuItem = -MNU_FLAGS,       .numItems = sizeof(menu_FLAGS      )/sizeof(int16_t), .softkeyItem = menu_FLAGS       },
/*  36 */  {.menuItem = -MNU_INFO,        .numItems = sizeof(menu_INFO       )/sizeof(int16_t), .softkeyItem = menu_INFO        },
/*  37 */  {.menuItem = -MNU_INTS,        .numItems = sizeof(menu_INTS       )/sizeof(int16_t), .softkeyItem = menu_INTS        },
/*  38 */  {.menuItem = -MNU_LOOP,        .numItems = sizeof(menu_LOOP       )/sizeof(int16_t), .softkeyItem = menu_LOOP        },
/*  39 */  {.menuItem = -MNU_MATX,        .numItems = sizeof(menu_MATX       )/sizeof(int16_t), .softkeyItem = menu_MATX        },
/*  40 */  {.menuItem = -MNU_SIMQ,        .numItems = sizeof(menu_M_SIM_Q    )/sizeof(int16_t), .softkeyItem = menu_M_SIM_Q     },
/*  41 */  {.menuItem = -MNU_M_EDIT,      .numItems = sizeof(menu_M_EDIT     )/sizeof(int16_t), .softkeyItem = menu_M_EDIT      },
/*  42 */  {.menuItem = -MNU_MODE,        .numItems = sizeof(menu_MODE       )/sizeof(int16_t), .softkeyItem = menu_MODE        },
/*  43 */  {.menuItem = -MNU_PARTS,       .numItems = sizeof(menu_PARTS      )/sizeof(int16_t), .softkeyItem = menu_PARTS       },
/*  44 */  {.menuItem = -MNU_PROB,        .numItems = sizeof(menu_PROB       )/sizeof(int16_t), .softkeyItem = menu_PROB        },
/*  45 */  {.menuItem = -MNU_T,           .numItems = sizeof(menu_t          )/sizeof(int16_t), .softkeyItem = menu_t           },
/*  46 */  {.menuItem = -MNU_F,           .numItems = sizeof(menu_F          )/sizeof(int16_t), .softkeyItem = menu_F           },
/*  47 */  {.menuItem = -MNU_CHI2,        .numItems = sizeof(menu_chi2       )/sizeof(int16_t), .softkeyItem = menu_chi2        },
/*  48 */  {.menuItem = -MNU_NORML,       .numItems = sizeof(menu_Norml      )/sizeof(int16_t), .softkeyItem = menu_Norml       },
/*  49 */  {.menuItem = -MNU_LGNRM,       .numItems = sizeof(menu_LgNrm      )/sizeof(int16_t), .softkeyItem = menu_LgNrm       },
/*  50 */  {.menuItem = -MNU_CAUCH,       .numItems = sizeof(menu_Cauch      )/sizeof(int16_t), .softkeyItem = menu_Cauch       },
/*  51 */  {.menuItem = -MNU_EXPON,       .numItems = sizeof(menu_Expon      )/sizeof(int16_t), .softkeyItem = menu_Expon       },
/*  52 */  {.menuItem = -MNU_LOGIS,       .numItems = sizeof(menu_Logis      )/sizeof(int16_t), .softkeyItem = menu_Logis       },
/*  53 */  {.menuItem = -MNU_WEIBL,       .numItems = sizeof(menu_Weibl      )/sizeof(int16_t), .softkeyItem = menu_Weibl       },
/*  54 */  {.menuItem = -MNU_BINOM,       .numItems = sizeof(menu_Binom      )/sizeof(int16_t), .softkeyItem = menu_Binom       },
/*  55 */  {.menuItem = -MNU_GEOM,        .numItems = sizeof(menu_Geom       )/sizeof(int16_t), .softkeyItem = menu_Geom        },
/*  56 */  {.menuItem = -MNU_HYPER,       .numItems = sizeof(menu_Hyper      )/sizeof(int16_t), .softkeyItem = menu_Hyper       },
/*  57 */  {.menuItem = -MNU_NBIN,        .numItems = sizeof(menu_Nbin       )/sizeof(int16_t), .softkeyItem = menu_Nbin        },
/*  58 */  {.menuItem = -MNU_POISS,       .numItems = sizeof(menu_Poiss      )/sizeof(int16_t), .softkeyItem = menu_Poiss       },
/*  59 */  {.menuItem = -MNU_PFN,         .numItems = sizeof(menu_PFN        )/sizeof(int16_t), .softkeyItem = menu_PFN         },
/*  60 */  {.menuItem = -MNU_PFN2,        .numItems = sizeof(menu_PFN2       )/sizeof(int16_t), .softkeyItem = menu_PFN2        },
/*  61 */  {.menuItem = -MNU_STAT,        .numItems = sizeof(menu_STAT       )/sizeof(int16_t), .softkeyItem = menu_STAT        },
/*  62 */  {.menuItem = -MNU_SUMS,        .numItems = sizeof(menu_SUMS       )/sizeof(int16_t), .softkeyItem = menu_SUMS        },
/*  63 */  {.menuItem = -MNU_STK,         .numItems = sizeof(menu_STK        )/sizeof(int16_t), .softkeyItem = menu_STK         },
/*  64 */  {.menuItem = -MNU_TEST,        .numItems = sizeof(menu_TEST       )/sizeof(int16_t), .softkeyItem = menu_TEST        },
/*  65 */  {.menuItem = -MNU_XFN,         .numItems = sizeof(menu_XFN        )/sizeof(int16_t), .softkeyItem = menu_XFN         },
/*  66 */  {.menuItem = -MNU_ORTHOG,      .numItems = sizeof(menu_Orthog     )/sizeof(int16_t), .softkeyItem = menu_Orthog      },
/*  67 */  {.menuItem = -MNU_REGIST,      .numItems = sizeof(menu_REGIST     )/sizeof(int16_t), .softkeyItem = menu_REGIST      },
/*  68 */  {.menuItem = -MNU_CATALOG,     .numItems = sizeof(menu_CATALOG    )/sizeof(int16_t), .softkeyItem = menu_CATALOG     },
/*  69 */  {.menuItem = -MNU_DIGITS,      .numItems = sizeof(menu_DIGITS     )/sizeof(int16_t), .softkeyItem = menu_DIGITS      },
/*  70 */  {.menuItem = -MNU_CHARS,       .numItems = sizeof(menu_CHARS      )/sizeof(int16_t), .softkeyItem = menu_CHARS       },
/*  71 */  {.menuItem = -MNU_PROGS,       .numItems = sizeof(menu_PROGS      )/sizeof(int16_t), .softkeyItem = menu_PROGS       },
/*  72 */  {.menuItem = -MNU_VARS,        .numItems = sizeof(menu_VARS       )/sizeof(int16_t), .softkeyItem = menu_VARS        },
/*  73 */  {.menuItem = -MNU_ALPHA_OMEGA, .numItems = sizeof(menu_ALPHA_OMEGA)/sizeof(int16_t), .softkeyItem = menu_ALPHA_OMEGA },
/*  74 */  {.menuItem = -MNU_alpha_omega, .numItems = sizeof(menu_alpha_omega)/sizeof(int16_t), .softkeyItem = menu_alpha_omega },
/*  75 */  {.menuItem = -MNU_FCNS,        .numItems = sizeof(menu_FCNS       )/sizeof(int16_t), .softkeyItem = menu_FCNS        },
/*  76 */  {.menuItem = -MNU_MENUS,       .numItems = sizeof(menu_MENUS      )/sizeof(int16_t), .softkeyItem = menu_MENUS       },
/*  77 */  {.menuItem = -MNU_ALPHAMATH,   .numItems = sizeof(menu_alphaMATH  )/sizeof(int16_t), .softkeyItem = menu_alphaMATH   },
/*  78 */  {.menuItem = -MNU_ALPHADOT,    .numItems = sizeof(menu_alphaDot   )/sizeof(int16_t), .softkeyItem = menu_alphaDot    },
/*  79 */  {.menuItem = -MNU_ALPHAFN,     .numItems = sizeof(menu_alphaFN    )/sizeof(int16_t), .softkeyItem = menu_alphaFN     },
/*  80 */  {.menuItem = -MNU_ANGLECONV,   .numItems = sizeof(menu_AngleConv  )/sizeof(int16_t), .softkeyItem = menu_AngleConv   },
/*  81 */  {.menuItem = -MNU_UNITCONV,    .numItems = sizeof(menu_UnitConv   )/sizeof(int16_t), .softkeyItem = menu_UnitConv    },
/*  82 */  {.menuItem = -MNU_CONVE,       .numItems = sizeof(menu_ConvE      )/sizeof(int16_t), .softkeyItem = menu_ConvE       },
/*  83 */  {.menuItem = -MNU_CONVP,       .numItems = sizeof(menu_ConvP      )/sizeof(int16_t), .softkeyItem = menu_ConvP       },
/*  84 */  {.menuItem = -MNU_CONVFP,      .numItems = sizeof(menu_ConvFP     )/sizeof(int16_t), .softkeyItem = menu_ConvFP      },
/*  85 */  {.menuItem = -MNU_CONVM,       .numItems = sizeof(menu_ConvM      )/sizeof(int16_t), .softkeyItem = menu_ConvM       },
/*  86 */  {.menuItem = -MNU_CONVX,       .numItems = sizeof(menu_ConvX      )/sizeof(int16_t), .softkeyItem = menu_ConvX       },
/*  87 */  {.menuItem = -MNU_CONVV,       .numItems = sizeof(menu_ConvV      )/sizeof(int16_t), .softkeyItem = menu_ConvV       },
/*  88 */  {.menuItem = -MNU_CONVA,       .numItems = sizeof(menu_ConvA      )/sizeof(int16_t), .softkeyItem = menu_ConvA       },
/*  89 */  {.menuItem = -MNU_CONST,       .numItems = sizeof(menu_CONST      )/sizeof(int16_t), .softkeyItem = menu_CONST       },
/*  90 */  {.menuItem = -MNU_IO,          .numItems = sizeof(menu_IO         )/sizeof(int16_t), .softkeyItem = menu_IO          },
/*  91 */  {.menuItem = -MNU_PRINT,       .numItems = sizeof(menu_PRINT      )/sizeof(int16_t), .softkeyItem = menu_PRINT       },
/*  92 */  {.menuItem = -MNU_TAM,         .numItems = sizeof(menu_Tam        )/sizeof(int16_t), .softkeyItem = menu_Tam         },
/*  93 */  {.menuItem = -MNU_TAMCMP,      .numItems = sizeof(menu_TamCmp     )/sizeof(int16_t), .softkeyItem = menu_TamCmp      },
/*  94 */  {.menuItem = -MNU_TAMSTORCL,   .numItems = sizeof(menu_TamStoRcl  )/sizeof(int16_t), .softkeyItem = menu_TamStoRcl   },
/*  95 */  {.menuItem = -MNU_TAMSHUFFLE,  .numItems = sizeof(menu_TamShuffle )/sizeof(int16_t), .softkeyItem = menu_TamShuffle  },
/*  96 */  {.menuItem = -MNU_TAMLABEL,    .numItems = sizeof(menu_TamLabel   )/sizeof(int16_t), .softkeyItem = menu_TamLabel    },
/*  97 */  {.menuItem =  0,               .numItems = 0,                                        .softkeyItem = NULL             }
};


dynamicSoftmenu_t dynamicSoftmenu[NUMBER_OF_DYNAMIC_SOFTMENUS] = {
/*   0 */  {.menuItem = -MNU_MyMenu,  .numItems = 0, .menuContent = NULL},
/*   1 */  {.menuItem = -MNU_MyAlpha, .numItems = 0, .menuContent = NULL},
/*   2 */  {.menuItem = -MNU_RAM,     .numItems = 0, .menuContent = NULL},
/*   3 */  {.menuItem = -MNU_FLASH,   .numItems = 0, .menuContent = NULL},
/*   4 */  {.menuItem = -MNU_VAR,     .numItems = 0, .menuContent = NULL},
/*   5 */  {.menuItem = -MNU_PROG,    .numItems = 0, .menuContent = NULL},
/*   6 */  {.menuItem = -MNU_MATRS,   .numItems = 0, .menuContent = NULL},
/*   7 */  {.menuItem = -MNU_STRINGS, .numItems = 0, .menuContent = NULL},
/*   8 */  {.menuItem = -MNU_DATES,   .numItems = 0, .menuContent = NULL},
/*   9 */  {.menuItem = -MNU_TIMES,   .numItems = 0, .menuContent = NULL},
/*  10 */  {.menuItem = -MNU_ANGLES,  .numItems = 0, .menuContent = NULL},
/*  11 */  {.menuItem = -MNU_SINTS,   .numItems = 0, .menuContent = NULL},
/*  12 */  {.menuItem = -MNU_LINTS,   .numItems = 0, .menuContent = NULL},
/*  13 */  {.menuItem = -MNU_REALS,   .numItems = 0, .menuContent = NULL},
/*  14 */  {.menuItem = -MNU_CPXS,    .numItems = 0, .menuContent = NULL},
};



uint8_t *getNthString(uint8_t *ptr, int16_t n) { // Starting with string 0 (the 1st string is returned for n=0)
  while(n) {
    ptr += stringByteLength((char *)ptr) + 1;
    n--;
  }

  return ptr;
}



void fnDynamicMenu(uint16_t unusedButMandatoryParameter) {
  printf("fnDynamicMenu:\n       softmenuId = %d\n  dynamicMenuItem = %d\n", softmenuStack[softmenuStackPointer].softmenuId, dynamicMenuItem);
}



#ifndef TESTSUITE_BUILD
  static int sortMenu(void const *a, void const *b)
   {
    return compareString(a, b, CMP_EXTENSIVE);
   }



  static void initVariableSoftmenu(int16_t menu) {
    int16_t i, numberOfBytes, numberOfGlobalLabels;
    uint8_t *ptr;

    free(dynamicSoftmenu[menu].menuContent);

    switch(-dynamicSoftmenu[menu].menuItem) {
      case MNU_MyAlpha: dynamicSoftmenu[menu].menuContent = malloc(17);
                        //xcopy(dynamicSoftmenu[menu].menuContent, "My" STD_alpha "\000to\000be\000coded", 17);
                        xcopy(dynamicSoftmenu[menu].menuContent, "", 1);
                        dynamicSoftmenu[menu].numItems = 4;
                        break;

      case MNU_FLASH:   dynamicSoftmenu[menu].menuContent = malloc(18);
                        xcopy(dynamicSoftmenu[menu].menuContent, "FLASH\000to\000be\000coded", 18);
                        dynamicSoftmenu[menu].numItems = 4;
                        break;

      case MNU_RAM:     numberOfBytes = 0;
                        numberOfGlobalLabels = 0;
                        memset(tmpString, 0, TMP_STR_LENGTH);
                        for(i=0; i<numberOfLabels; i++) {
                          if(labelList[i].program > 0 && labelList[i].followingStep > 0) { // RAM and Global label
                            xcopy(tmpString + 15 * numberOfGlobalLabels, labelList[i].labelPointer + 1, labelList[i].labelPointer[0]);
                            numberOfGlobalLabels++;
                            numberOfBytes += 1 + labelList[i].labelPointer[0];
                          }
                        }

                        if(numberOfGlobalLabels != 0) {
                          qsort(tmpString, numberOfGlobalLabels, 15, sortMenu);
                        }

                        ptr = malloc(numberOfBytes);
                        dynamicSoftmenu[menu].menuContent = ptr;
                        for(i=0; i<numberOfGlobalLabels; i++) {
                          int16_t len = stringByteLength(tmpString + 15*i) + 1;
                          xcopy(ptr, tmpString + 15*i, len);
                          ptr += len;
                        }

                        dynamicSoftmenu[menu].numItems = numberOfGlobalLabels;
                        break;

      case MNU_MyMenu:  dynamicSoftmenu[menu].menuContent = malloc(19);
                        //xcopy(dynamicSoftmenu[menu].menuContent, "MyMenu\000to\000be\000coded", 19);
                        xcopy(dynamicSoftmenu[menu].menuContent, "", 1);
                        dynamicSoftmenu[menu].numItems = 4;
                        break;

      case MNU_VAR:     dynamicSoftmenu[menu].menuContent = malloc(16);
                        xcopy(dynamicSoftmenu[menu].menuContent, "VAR\000to\000be\000coded", 16);
                        dynamicSoftmenu[menu].numItems = 4;
                        break;

      case MNU_PROG:    numberOfBytes = 1;
                        numberOfGlobalLabels = 0;
                        memset(tmpString, 0, TMP_STR_LENGTH);
                        for(i=0; i<numberOfLabels; i++) {
                          if(labelList[i].program > 0 && labelList[i].followingStep > 0) { // RAM and Global label
                            xcopy(tmpString + 15 * numberOfGlobalLabels, labelList[i].labelPointer + 1, labelList[i].labelPointer[0]);
                            numberOfGlobalLabels++;
                            numberOfBytes += 1 + labelList[i].labelPointer[0];
                          }
                        }

                        if(numberOfGlobalLabels != 0) {
                          qsort(tmpString, numberOfGlobalLabels, 15, sortMenu);
                        }

                        ptr = malloc(numberOfBytes);
                        dynamicSoftmenu[menu].menuContent = ptr;
                        for(i=0; i<numberOfGlobalLabels; i++) {
                          int16_t len = stringByteLength(tmpString + 15*i) + 1;
                          xcopy(ptr, tmpString + 15*i, len);
                          ptr += len;
                        }

                        dynamicSoftmenu[menu].numItems = numberOfGlobalLabels;
                        break;

      case MNU_MATRS:   dynamicSoftmenu[menu].menuContent = malloc(18);
                        xcopy(dynamicSoftmenu[menu].menuContent, "MATRS\000to\000be\000coded", 18);
                        dynamicSoftmenu[menu].numItems = 4;
                        break;

      case MNU_STRINGS: dynamicSoftmenu[menu].menuContent = malloc(20);
                        xcopy(dynamicSoftmenu[menu].menuContent, "STRINGS\000to\000be\000coded", 20);
                        dynamicSoftmenu[menu].numItems = 4;
                        break;

      case MNU_DATES:   dynamicSoftmenu[menu].menuContent = malloc(18);
                        xcopy(dynamicSoftmenu[menu].menuContent, "DATES\000to\000be\000coded", 18);
                        dynamicSoftmenu[menu].numItems = 4;
                        break;

      case MNU_TIMES:   dynamicSoftmenu[menu].menuContent = malloc(18);
                        xcopy(dynamicSoftmenu[menu].menuContent, "TIMES\000to\000be\000coded", 18);
                        dynamicSoftmenu[menu].numItems = 4;
                        break;

      case MNU_ANGLES:  dynamicSoftmenu[menu].menuContent = malloc(19);
                        xcopy(dynamicSoftmenu[menu].menuContent, "ANGLES\000to\000be\000coded", 19);
                        dynamicSoftmenu[menu].numItems = 4;
                        break;

      case MNU_SINTS:   dynamicSoftmenu[menu].menuContent = malloc(18);
                        xcopy(dynamicSoftmenu[menu].menuContent, "SINTS\000to\000be\000coded", 18);
                        dynamicSoftmenu[menu].numItems = 4;
                        break;

      case MNU_LINTS:   dynamicSoftmenu[menu].menuContent = malloc(18);
                        xcopy(dynamicSoftmenu[menu].menuContent, "LINTS\000to\000be\000coded", 18);
                        dynamicSoftmenu[menu].numItems = 4;
                        break;

      case MNU_REALS:   dynamicSoftmenu[menu].menuContent = malloc(18);
                        xcopy(dynamicSoftmenu[menu].menuContent, "REALS\000to\000be\000coded", 18);
                        dynamicSoftmenu[menu].numItems = 4;
                        break;

      case MNU_CPXS:    dynamicSoftmenu[menu].menuContent = malloc(17);
                        xcopy(dynamicSoftmenu[menu].menuContent, "CPXS\000to\000be\000coded", 17);
                        dynamicSoftmenu[menu].numItems = 4;
                        break;

      default:          sprintf(errorMessage, "In function initVariableSoftmenu: unexpected variable softmenu %" PRId16 "!", -dynamicSoftmenu[menu].menuItem);
                        displayBugScreen(errorMessage);
    }
  }



  /********************************************//**
   * \brief Displays one softkey
   *
   * \param[in] l const char*         Text to display
   * \param[in] xSoftkey int16_t      x location of softkey: from 0 (left) to 5 (right)
   * \param[in] ySoftKey int16_t      y location of softkey: from 0 (bottom) to 2 (top)
   * \param[in] videoMode videoMode_t Video mode normal or reverse
   * \param[in] topLineDotted bool_t  Is the item's top line dotted
   * \param[in] topLine bool_t        Draw a top line
   * \param[in] bottomLine bool_t     Draw a bottom line
   * \return void
   ***********************************************/
  void showSoftkey(const char *label, int16_t xSoftkey, int16_t ySoftKey, videoMode_t videoMode, bool_t topLine, bool_t bottomLine) {
    int16_t x1, y1, x2, y2;
    int16_t w;
    char l[15];

    if(0 <= xSoftkey && xSoftkey <= 5) {
      x1 = 67 * xSoftkey - 1;
      x2 = x1 + 67;
    }
    else {
      sprintf(errorMessage, "In function showSoftkey: xSoftkey=%" PRId16 " must be from 0 to 5" , xSoftkey);
      displayBugScreen(errorMessage);
      return;
    }

    if(0 <= ySoftKey && ySoftKey <= 2) {
      y1 = 217 - SOFTMENU_HEIGHT * ySoftKey;
      y2 = y1 + SOFTMENU_HEIGHT;
    }
    else {
      sprintf(errorMessage, "In function showSoftkey: ySoftKey=%" PRId16 " but must be from 0 to 2!" , ySoftKey);
      displayBugScreen(errorMessage);
      return;
    }

    // Draw the frame
    //   Top line
    if(topLine) {
      lcd_fill_rect(max(0, x1), y1, min(x2, SCREEN_WIDTH) - max(0, x1), 1, (videoMode == vmNormal ? LCD_EMPTY_VALUE : LCD_SET_VALUE));
    }

    //   Bottom line
    if(y1 + SOFTMENU_HEIGHT <= min(y2, 239) && bottomLine) {
      lcd_fill_rect(max(0, x1), y1 + SOFTMENU_HEIGHT, min(x2, SCREEN_WIDTH) - max(0, x1), 1, (videoMode == vmNormal ? LCD_EMPTY_VALUE : LCD_SET_VALUE));
    }

    //   Left line
    if(x1 >= 0) {
      lcd_fill_rect(x1, y1, 1, min(y2, SCREEN_HEIGHT - 1) + 1 - y1, (videoMode == vmNormal ? LCD_EMPTY_VALUE : LCD_SET_VALUE));
    }

    //   Right line
    if(x2 < SCREEN_WIDTH) {
      lcd_fill_rect(x2, y1, 1, min(y2, SCREEN_HEIGHT - 1) + 1 - y1, (videoMode == vmNormal ? LCD_EMPTY_VALUE : LCD_SET_VALUE));
    }

    // Clear inside the frame
    lcd_fill_rect(x1 + 1, y1 + 1, min(x2, SCREEN_WIDTH) - x1 - 1, min(y2, SCREEN_HEIGHT) - y1 - 1, (videoMode == vmNormal ? LCD_SET_VALUE : LCD_EMPTY_VALUE));

    xcopy(l, label, stringByteLength(label) + 1);
    w = stringWidth(l, &standardFont, false, false);
    while(w > (xSoftkey == 5 ? 65 : 66)) {
      l[stringLastGlyph(l)] = 0;
      w = stringWidth(l, &standardFont, false, false);
    }

    showString(l, &standardFont, x1 + (xSoftkey == 5 ? 33 : 34) - w/2, y1 + 2, videoMode, false, false);
  }



  /********************************************//**
   * \brief Displays the current part of the displayed softmenu
   *
   * \param void
   * \return void
   ***********************************************/
  void showSoftmenuCurrentPart(void) {
    int16_t x, y, yDotted=0, currentFirstItem, item, numberOfItems, m = softmenuStack[softmenuStackPointer].softmenuId;
    bool_t dottedTopLine;

    if(m < NUMBER_OF_DYNAMIC_SOFTMENUS) { // Dynamic softmenu
      initVariableSoftmenu(m);
      numberOfItems = dynamicSoftmenu[m].numItems;
    }
    else { // Static softmenu
      numberOfItems = softmenu[m].numItems;
    }
    currentFirstItem = softmenuStack[softmenuStackPointer].firstItem;

    if(numberOfItems <= 18) {
      dottedTopLine = false;
    }
    else {
      dottedTopLine = true;
      yDotted = min(3, (numberOfItems + modulo(currentFirstItem - numberOfItems, 6))/6 - currentFirstItem/6) - 1;

      if(m >= NUMBER_OF_DYNAMIC_SOFTMENUS) { // Static softmenu
        item = 6 * (currentFirstItem / 6 + yDotted);
        if(                softmenu[m].softkeyItem[item]==0 && softmenu[m].softkeyItem[item+1]==0 && softmenu[m].softkeyItem[item+2]==0 && softmenu[m].softkeyItem[item+3]==0 && softmenu[m].softkeyItem[item+4]==0 && softmenu[m].softkeyItem[item+5]==0) {
          yDotted--;
        }

        item = 6 * (currentFirstItem / 6 + yDotted);
        if(yDotted >= 0 && softmenu[m].softkeyItem[item]==0 && softmenu[m].softkeyItem[item+1]==0 && softmenu[m].softkeyItem[item+2]==0 && softmenu[m].softkeyItem[item+3]==0 && softmenu[m].softkeyItem[item+4]==0 && softmenu[m].softkeyItem[item+5]==0) {
          yDotted--;
        }

        item = 6 * (currentFirstItem / 6 + yDotted);
        if(yDotted >= 0 && softmenu[m].softkeyItem[item]==0 && softmenu[m].softkeyItem[item+1]==0 && softmenu[m].softkeyItem[item+2]==0 && softmenu[m].softkeyItem[item+3]==0 && softmenu[m].softkeyItem[item+4]==0 && softmenu[m].softkeyItem[item+5]==0) {
          yDotted--;
        }
      }
    }

    if(m < NUMBER_OF_DYNAMIC_SOFTMENUS) { // Dynamic softmenu
      if(numberOfItems == 0) {
        for(x=0; x<6; x++) {
          showSoftkey("", x, 0, vmNormal, true, true);
        }
      }
      else {
        uint8_t *ptr = getNthString(dynamicSoftmenu[m].menuContent, currentFirstItem);
        for(y=0; y<3; y++) {
          for(x=0; x<6; x++) {
            if(x + 6*y + currentFirstItem < numberOfItems) {
              showSoftkey((char *)ptr, x, y, vmNormal, true, true);
              ptr += stringByteLength((char *)ptr) + 1;
            }
          }
        }
      }
    }
    else {
      const int16_t *softkeyItem = softmenu[m].softkeyItem + currentFirstItem;
      for(y=currentFirstItem/6; y<=min(currentFirstItem/6+2, numberOfItems/6); y++, softkeyItem+=6) {
        for(x=0; x<6; x++) {
          if(softkeyItem + x >= softmenu[m].softkeyItem + numberOfItems) {
            item = ITM_NULL;
          }
          else {
            item = softkeyItem[x];
          }
          if(item < 0) { // softmenu
            int16_t menu = 0;
            while(softmenu[menu].menuItem != 0) {
              if(softmenu[menu].menuItem == item) {
                break;
              }
              menu++;
            }

            if(softmenu[menu].menuItem == 0) {
              sprintf(errorMessage, "In function showSoftmenuCurrentPart: softmenu ID %" PRId16 " not found!", item);
              displayBugScreen(errorMessage);
            }
            else {
              showSoftkey(indexOfItems[-softmenu[menu].menuItem].itemSoftmenuName, x, y-currentFirstItem/6, vmReverse, true, true);
            }
          }
          else if(item == 9999) {
            showSoftkey(indexOfItems[getSystemFlag(FLAG_MULTx) ? ITM_DOT : ITM_CROSS].itemSoftmenuName, x, y-currentFirstItem/6, vmNormal, true, true);
          }
          else if(item > 0 && indexOfItems[item%10000].itemSoftmenuName[0] != 0) { // softkey
            // item : +10000 -> no top line
            //        +20000 -> no bottom line
            //        +30000 -> neither top nor bottom line
            if(softmenu[m].menuItem == -MNU_FCNS) {
              showSoftkey(indexOfItems[item%10000].itemCatalogName,  x, y-currentFirstItem/6, vmNormal, (item/10000)==0 || (item/10000)==2, (item/10000)==0 || (item/10000)==1);
            }
            else {
              showSoftkey(indexOfItems[item%10000].itemSoftmenuName, x, y-currentFirstItem/6, vmNormal, (item/10000)==0 || (item/10000)==2, (item/10000)==0 || (item/10000)==1);
            }
            if(indexOfItems[item%10000].func == itemToBeCoded) {
              // Strike out non coded functions
              int16_t yStroke = SCREEN_HEIGHT - (y-currentFirstItem/6)*23 - 3;
              for(int16_t xStroke=x*67 + 10; xStroke<x*67 + 57; xStroke++) {
                if(xStroke%3 == 0) yStroke--;
                setBlackPixel(xStroke, yStroke);
              }
            }
          }
        }
      }
    }

    if(0 <= yDotted && yDotted <= 2) {
      yDotted = 217 - SOFTMENU_HEIGHT * yDotted;

      if(dottedTopLine) {
        for(x=0; x<SCREEN_WIDTH; x++) {
          if(x%8 < 4) {
            setBlackPixel(x, yDotted);
          }
          else {
            setWhitePixel(x, yDotted);
          }
        }
      }
    }
  }



  /********************************************//**
   * \brief Initializes the softmenu stack with a
   * softmenu and displays it's first part
   *
   * \param[in] softmenuId int16_t Softmenu ID
   * \return void
   ***********************************************/
  void initSoftmenuStack(int16_t softmenuId) {
    softmenuStack[0].softmenuId = softmenuId;
    softmenuStack[0].firstItem = (alphaSelectionMenu == ASM_CNST ? lastCnstMenuPos :
                                 (alphaSelectionMenu == ASM_FCNS ? lastFcnsMenuPos :
                                 (alphaSelectionMenu == ASM_MENU ? lastMenuMenuPos :
                                 (alphaSelectionMenu == ASM_SYFL ? lastSyFlMenuPos :
                                 (alphaSelectionMenu == ASM_AINT ? lastAIntMenuPos :
                                 (alphaSelectionMenu == ASM_aint ? lastAIntMenuPos :
                                 (alphaSelectionMenu == ASM_PROG ? lastProgMenuPos :
                                  0)))))));
    softmenuStackPointer = 0;
  }



  /********************************************//**
   * \brief Pushes a new softmenu on the softmenu stack
   * and displays it's first part
   *
   * \param[in] softmenuId int16_t Softmenu ID
   * \return void
   ***********************************************/
  void pushSoftmenu(int16_t softmenuId) {
    softmenuStackPointer++;
    if(softmenuStackPointer < SOFTMENU_STACK_SIZE) {
      softmenuStack[softmenuStackPointer].softmenuId = softmenuId;
      softmenuStack[softmenuStackPointer].firstItem = (alphaSelectionMenu == ASM_CNST ? lastCnstMenuPos :
                                                      (alphaSelectionMenu == ASM_FCNS ? lastFcnsMenuPos :
                                                      (alphaSelectionMenu == ASM_MENU ? lastMenuMenuPos :
                                                      (alphaSelectionMenu == ASM_SYFL ? lastSyFlMenuPos :
                                                      (alphaSelectionMenu == ASM_AINT ? lastAIntMenuPos :
                                                      (alphaSelectionMenu == ASM_aint ? lastAIntMenuPos :
                                                      (alphaSelectionMenu == ASM_PROG ? lastProgMenuPos :
                                                       0)))))));
    }
    else {
      softmenuStackPointer--;
      displayBugScreen("In function pushSoftmenu: the softmenu stack is full! Please increase the value of #define SOFTMENU_STACK_SIZE in defines.h");
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
    if(alphaSelectionMenu != ASM_NONE) {
      alphaSelectionMenu = ASM_NONE;
      if(calcMode != CM_ASM_OVER_AIM && calcMode != CM_ASM_OVER_TAM && calcMode != CM_ASM_OVER_TAM_OVER_PEM && calcMode != CM_PEM) {
        calcModeNormal();
      }
    }
    softmenuStackPointer--;

    if(softmenuStackPointer <= 0) { // There is always a softmenu to display
      softmenuStack[0].softmenuId = (calcMode == CM_AIM ? 1 : 0); // 1=MyAlpha and 0=MyMenu
      softmenuStack[0].firstItem = 0;
      softmenuStackPointer = 0;
    }
  }



  /********************************************//**
   * \brief Displays a softmenu. One of the 2 first
   * parameter must be null and one must not be null
   *
   * \param[in] menu const char* Name of softmenu
   * \param[in] id int16_t       ID of softmenu
   * \param[in] initOrPushOnMenuStack int16_t  * MS_INIT initialize the softmenu stack
   *                                           * MS_PUSH push the softmenu on the stack
   * \return void
   ***********************************************/
  void showSoftmenu(const char *menu, int16_t id, int16_t initOrPushOnMenuStack) {
    int16_t m;

    switch(-id) {
      case MNU_FCNS:
        alphaSelectionMenu = ASM_FCNS;
        if(calcMode == CM_PEM) {
          calcModeAsm();
          calcMode = CM_ASM_OVER_PEM;
          clearSystemFlag(FLAG_ALPHA);
        }
        else {
          calcModeAsm();
        }
        break;

      case MNU_CONST:
        alphaSelectionMenu = ASM_CNST;
        if(calcMode == CM_PEM) {
          calcModeAsm();
          calcMode = CM_ASM_OVER_PEM;
          clearSystemFlag(FLAG_ALPHA);
        }
        else {
          calcModeAsm();
        }
        break;

      case MNU_MENUS:
        alphaSelectionMenu = ASM_MENU;
        calcModeAsm();
        break;

      case MNU_SYSFL:
        alphaSelectionMenu = ASM_SYFL;
        if(calcMode == CM_TAM) {
          calcModeAsm();
          calcMode = CM_ASM_OVER_TAM;
          clearSystemFlag(FLAG_ALPHA);
        }
        else if(calcMode == CM_TAM_OVER_PEM) {
          calcModeAsm();
          calcMode = CM_ASM_OVER_TAM_OVER_PEM;
          clearSystemFlag(FLAG_ALPHA);
        }
        else {
          calcModeAsm();
        }
        break;

      case MNU_ALPHAINTL:
        alphaSelectionMenu = ASM_AINT;
        if(calcMode == CM_AIM) {
          calcModeAsm();
          calcMode = CM_ASM_OVER_AIM;
          clearSystemFlag(FLAG_ALPHA);
        }
        else {
          calcModeAsm();
        }
        break;

      case MNU_ALPHAintl:
        alphaSelectionMenu = ASM_aint;
        if(calcMode == CM_AIM) {
          calcModeAsm();
          calcMode = CM_ASM_OVER_AIM;
          clearSystemFlag(FLAG_ALPHA);
        }
        else {
          calcModeAsm();
        }
        break;

      case MNU_PROG:
        alphaSelectionMenu = ASM_PROG;
        if(calcMode == CM_PEM) {
          calcModeAsm();
          calcMode = CM_ASM_OVER_PEM;
          clearSystemFlag(FLAG_ALPHA);
        }
        else if(calcMode == CM_TAM) {
          calcModeAsm();
          calcMode = CM_ASM_OVER_TAM;
          clearSystemFlag(FLAG_ALPHA);
        }
        else if(calcMode == CM_TAM_OVER_PEM) {
          calcModeAsm();
          calcMode = CM_ASM_OVER_TAM_OVER_PEM;
          clearSystemFlag(FLAG_ALPHA);
        }
        else {
          calcModeAsm();
        }
        break;

      default:
        alphaSelectionMenu = ASM_NONE;
    }

    if((menu != NULL && id != 0) || (menu == NULL && id == 0)) {
      displayBugScreen("In function showSoftmenu: one parameter must be 0 and one parameter must not be 0!");
      return;
    }

    if(id == -MNU_ALPHAINTL && alphaCase == AC_LOWER) { // alphaINTL
      id = -MNU_ALPHAintl;
    }
    else if(id == -MNU_ALPHA_OMEGA && alphaCase == AC_LOWER) { // alpha...omega
      id = -MNU_alpha_omega;
    }

    if((id == 0 && !strcmp(menu, STD_alpha "INTL")) && alphaCase == AC_LOWER) { // alphaINTL
      id = -MNU_ALPHAintl;
    }
    else if((id == 0 && !strcmp(menu, STD_ALPHA STD_ELLIPSIS STD_OMEGA)) && alphaCase == AC_LOWER) { // alpha...omega
      id = -MNU_alpha_omega;
    }

    m = 0;

    if(id != 0) { // Search by ID
      while(softmenu[m].menuItem != 0) {
        if(softmenu[m].menuItem == id) {
         break;
        }
        m++;
      }
    }
    else { // Search by name
      while(softmenu[m].menuItem != 0) {
        if(strcmp(indexOfItems[-softmenu[m].menuItem].itemSoftmenuName, menu) == 0) {
          break;
        }
        m++;
      }
    }
    if(softmenu[m].menuItem == 0) {
      if(menu == NULL) {
        sprintf(errorMessage, "In function showSoftmenu: softmenu %" PRId16 " not found!", id);
        displayBugScreen(errorMessage);
      }
      else {
        sprintf(errorMessage, "In function showSoftmenu: softmenu %s not found!", menu);
        displayBugScreen(errorMessage);
      }
    }
    else {
      if(calcMode == CM_NORMAL || calcMode == CM_NIM || calcMode == CM_ASM || calcMode == CM_ASM_OVER_TAM || calcMode == CM_ASM_OVER_TAM_OVER_PEM || calcMode == CM_ASM_OVER_AIM || calcMode == CM_ASM_OVER_PEM || calcMode == CM_AIM || calcMode == CM_PEM) {
        if(initOrPushOnMenuStack) {
          pushSoftmenu(m);
        }
        else {
          initSoftmenuStack(m);
        }
      }
      else if(calcMode == CM_TAM || calcMode == CM_TAM_OVER_PEM) {
      }
      else {
        sprintf(errorMessage, "In fuction showSoftMenu: %" PRIu8 " is an unexpected value for calcMode!", calcMode);
        displayBugScreen(errorMessage);
      }
    }
  }



  void setCatalogLastPos(void) {
         if(alphaSelectionMenu == ASM_CNST) lastCnstMenuPos = softmenuStack[softmenuStackPointer].firstItem;
    else if(alphaSelectionMenu == ASM_FCNS) lastFcnsMenuPos = softmenuStack[softmenuStackPointer].firstItem;
    else if(alphaSelectionMenu == ASM_MENU) lastMenuMenuPos = softmenuStack[softmenuStackPointer].firstItem;
    else if(alphaSelectionMenu == ASM_SYFL) lastSyFlMenuPos = softmenuStack[softmenuStackPointer].firstItem;
    else if(alphaSelectionMenu == ASM_AINT) lastAIntMenuPos = softmenuStack[softmenuStackPointer].firstItem;
    else if(alphaSelectionMenu == ASM_aint) lastAIntMenuPos = softmenuStack[softmenuStackPointer].firstItem;
    else if(alphaSelectionMenu == ASM_PROG) lastProgMenuPos = softmenuStack[softmenuStackPointer].firstItem;
  }
#endif // !TESTSUITE_BUILD
