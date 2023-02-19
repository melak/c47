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

/* ADDITIONAL C43 functions and routines */


/********************************************//**
 * \file radioButtonCatalog.c
 ***********************************************/

#include "c43Extensions/radioButtonCatalog.h"

#include "curveFitting.h"
#include "fonts.h"
#include "c43Extensions/inlineTest.h"
#include "items.h"
#include "c43Extensions/jm.h"
#include "plotstat.h"
#include "screen.h"
#include <string.h>

#include "wp43.h"


TO_QSPI const radiocb_t indexOfRadioCbEepromItems[] = {
//  itemNr    item                  parameter                         function
/*  117 */  { ITM_DEG,              amDegree,               RB_AM },  //fnAngularMode
/*  134 */  { ITM_DMS,              amDMS,                  RB_AM },  //fnAngularMode
/*  229 */  { ITM_GRAD,             amGrad,                 RB_AM },  //fnAngularMode
/*  375 */  { ITM_MULPI,            amMultPi,               RB_AM },  //fnAngularMode
/*  483 */  { ITM_RAD,              amRadian,               RB_AM },  //fnAngularMode
/* 1965 */  { ITM_POLAR,            CM_POLAR,               RB_CM },  //fnSetSetJM          /*  464 */ //fnComplexMode
/* 1969 */  { ITM_RECT,             CM_RECTANGULAR,         RB_CM },  //fnSetSetJM          /*  507 */ //fnComplexMode
/* 1960 */  { ITM_CPXI,             CU_I,                   RB_CU },  //fnSetSetJM          /*   96 */ //fnComplexUnit
/* 1961 */  { ITM_CPXJ,             CU_J,                   RB_CU },  //fnSetSetJM          /*   97 */ //fnComplexUnit

/*  136 */  { ITM_DMY,              DF_DMY,                 RB_DF },  //fnSetDateFormat
/*  383 */  { ITM_MDY,              DF_MDY,                 RB_DF },  //fnSetDateFormat
/*  700 */  { ITM_YMD,              DF_YMD,                 RB_DF },  //fnSetDateFormat
/*   20 */  { ITM_ALL,              DF_ALL,                 RB_DI },  //fnDisplayFormatAll
/*  145 */  { ITM_ENG,              DF_ENG,                 RB_DI },  //fnDisplayFormatEng
/*  185 */  { ITM_FIX,              DF_FIX,                 RB_DI },  //fnDisplayFormatFix
/*  545 */  { ITM_SCI,              DF_SCI,                 RB_DI },  //fnDisplayFormatSci
/* 1682 */  { ITM_SIGFIG,           DF_SF,                  RB_DI },  //fnDisplayFormatSigFig
/* 1693 */  { ITM_UNIT,             DF_UN,                  RB_DI },  //fnDisplayFormatUnit
/* 1972 */  { ITM_ENGOVR,           DO_ENG,                 RB_DO },  //fnSetSetJM          /*  146 */ //fnDisplayOvr
/* 1971 */  { ITM_SCIOVR,           DO_SCI,                 RB_DO },  //fnSetSetJM          /*  547 */ //fnDisplayOvr
/*  251 */ // { ITM_IMPFRC,           JC_IMPROPER,            CB_JC },  //  --fnToggleFractionType
/*  471 */ // { ITM_PROFRC,           JC_PROPER,              CB_JC },  //  --fnToggleFractionType
/* 1905 */  { ITM_INP_DEF_43S,      ID_43S,                 RB_ID },  //fnInDefault
/* 1910 */  { ITM_INP_DEF_CPXDP,    ID_CPXDP,               RB_ID },  //fnInDefault
/* 1907 */  { ITM_INP_DEF_DP,       ID_DP,                  RB_ID },  //fnInDefault
/* 1913 */  { ITM_INP_DEF_LI,       ID_LI,                  RB_ID },  //fnInDefault
/* 1912 */  { ITM_INP_DEF_SI,       ID_SI,                  RB_ID },  //fnInDefault
/*    4 */  { ITM_1COMPL,           SIM_1COMPL,             RB_IM },  //fnIntegerMode
/*    7 */  { ITM_2COMPL,           SIM_2COMPL,             RB_IM },  //fnIntegerMode
/*  567 */  { ITM_SIGNMT,           SIM_SIGNMT,             RB_IM },  //fnIntegerMode
/*  645 */  { ITM_UNSIGN,           SIM_UNSIGN,             RB_IM },  //fnIntegerMode
/* 1962 */  { ITM_MULTCR,           PS_CROSS,               RB_PS },  //fnSetSetJM          /*  373 */ //fnProductSign
/* 1963 */  { ITM_MULTDOT,          PS_DOT,                 RB_PS },  //fnSetSetJM          /*  374 */ //fnProductSign
/* 1966 */  { ITM_RDXCOM,           RX_COMMA,               RB_RX },  //fnSetSetJM          /*  500 */ //fnRadixMark
/* 1967 */  { ITM_RDXPER,           RX_PERIOD,              RB_RX },  //fnSetSetJM          /*  501 */ //fnRadixMark
/* 1689 */  { ITM_WS8,              8,                      RB_WS },  //fnSetWordSize
/* 1690 */  { ITM_WS16,             16,                     RB_WS },  //fnSetWordSize
/* 1691 */  { ITM_WS32,             32,                     RB_WS },  //fnSetWordSize
/* 1692 */  { ITM_WS64,             64,                     RB_WS },  //fnSetWordSize
/* 1964 */  { ITM_SSIZE4,           SS_4,                   RB_SS },  //fnSetSetJM          /*  583 */ //fnStackSize
/* 1968 */  { ITM_SSIZE8,           SS_8,                   RB_SS },  //fnSetSetJM          /*  584 */ //fnStackSize
/* 1958 */  { ITM_CLK12,            TF_H12,                 RB_TF },  //fnSetSetJM          /*   75 */ //fnTimeFormat
/* 1959 */  { ITM_CLK24,            TF_H24,                 RB_TF },  //fnSetSetJM          /*   76 */ //fnTimeFormat

/* 1899 */  { ITM_N_KEY_ALPHA,      16384+ITM_AIM,          RB_SA },  //fnSigmaAssign
/* 1891 */  { ITM_N_KEY_CC,         16384+ITM_CC,           RB_SA },  //fnSigmaAssign
/*      */  { ITM_N_KEY_GSH,        16384+ITM_SHIFTg,       RB_SA },  //fnSigmaAssign
/* 1893 */  { ITM_N_KEY_MM,         16384+-MNU_MyMenu,      RB_SA },  //fnSigmaAssign
/*      */  { ITM_N_KEY_DRG,        16384+ITM_DRG,          RB_SA },  //fnSigmaAssign
/* 1897 */  { ITM_N_KEY_PRGM,       16384+ITM_PR,           RB_SA },  //fnSigmaAssign
/* 1889 */  { ITM_N_KEY_USER,       16384+ITM_USERMODE,     RB_SA },  //fnSigmaAssign
/*      */  { ITM_N_KEY_HOME,       16384+-MNU_HOME,        RB_SA },  //fnSigmaAssign
/* 1895 */  { ITM_N_KEY_SIGMA,      16384+ITM_SIGMAPLUS,    RB_SA },  //fnSigmaAssign
/*      */  { ITM_N_KEY_SNAP,       16384+ITM_SNAP,         RB_SA },  //fnSigmaAssign


/*      */  { ITM_F1234,             RB_F1234,              RB_F },  //
/*      */  { ITM_M1234,             RB_M1234,              RB_M },  //
/*      */  { ITM_F14  ,             RB_F14,                RB_F },  //
/*      */  { ITM_M14  ,             RB_M14,                RB_M },  //
/*      */  { ITM_F124 ,             RB_F124,               RB_F },  //


/*      */  { ITM_BCDU,             BCDu,                   RB_BCD },  //
/*      */  { ITM_BCD9,             BCD9c,                  RB_BCD },  //
/*      */  { ITM_BCD10,            BCD10c,                 RB_BCD },  //

/*      */  { ITM_DENANY,           DM_ANY,                 CB_JC },  //  --fnDenMode
/*      */  { ITM_DENFIX,           DM_FIX,                 CB_JC },  //  --fnDenMode

/*  308 */  { ITM_T_LINF,           JC_LINEAR_FITTING       ,CB_JC },  //fnCurveFitting
/*  161 */  { ITM_T_EXPF,           JC_EXPONENTIAL_FITTING  ,CB_JC },  //fnCurveFitting
/*  325 */  { ITM_T_LOGF,           JC_LOGARITHMIC_FITTING  ,CB_JC },  //fnCurveFitting
/*  466 */  { ITM_T_POWERF,         JC_POWER_FITTING        ,CB_JC },  //fnCurveFitting
/* 1566 */  { ITM_T_ROOTF,          JC_ROOT_FITTING         ,CB_JC },  //fnCurveFitting
/* 1565 */  { ITM_T_HYPF,           JC_HYPERBOLIC_FITTING   ,CB_JC },  //fnCurveFitting
/* 1564 */  { ITM_T_PARABF,         JC_PARABOLIC_FITTING    ,CB_JC },  //fnCurveFitting
/* 1563 */  { ITM_T_CAUCHF,         JC_CAUCHY_FITTING       ,CB_JC },  //fnCurveFitting
/* 1562 */  { ITM_T_GAUSSF,         JC_GAUSS_FITTING        ,CB_JC },  //fnCurveFitting
/*  438 */  { ITM_T_ORTHOF,         JC_ORTHOGONAL_FITTING   ,CB_JC },  //fnCurveFitting

/* 1742 */  { ITM_BASE_HOME,        JC_BASE_HOME,           CB_JC },  //fnSetSetJM
/* 1695 */  { ITM_CB_CPXRES,        JC_BCR,                 CB_JC },  //fnSetSetJM
/* 1748 */  { ITM_CB_SPCRES,        JC_BSR,                 CB_JC },  //fnSetSetJM
/* 1696 */  { ITM_CB_LEADING_ZERO,  JC_BLZ,                 CB_JC },  //fnSetSetJM
/* 500  */  { ITM_CB_FRCSRN,        JC_FRC,                 CB_JC },  //fnSetSetJM
/* 1678 */  { ITM_ERPN,             JC_ERPN,                CB_JC },  //fnSetSetJM
/* 1909 */  { ITM_BASE_SCREEN,      JC_BASE_SCREEN,         CB_JC },  //fnSetSetJM
/* 1906 */  { ITM_FG_LINE,          JC_FG_LINE,             CB_JC },  //fnSetSetJM
/* 1911 */  { ITM_G_DOUBLETAP,      JC_G_DOUBLETAP,         CB_JC },  //fnSetSetJM
/* 1679 */  { ITM_HOMEx3,           JC_HOME_TRIPLE,         CB_JC },  //fnSetSetJM
/* 1680 */  { ITM_SHTIM,            JC_SHFT_4s,             CB_JC },  //fnSetSetJM
/* 1943 */  { ITM_VECT,             JC_VECT,                CB_JC },  //fnSetSetJM
/* 1943 */  { ITM_NVECT,            JC_NVECT,               CB_JC },  //fnSetSetJM
/*      */  { ITM_SCALE,            JC_SCALE,               CB_JC },  //fnSetSetJM
/* 1744 */  { ITM_H_SUMRY,          JC_H_SUM,               CB_JC },  //fnSetSetJM
/*      */  { ITM_LARGELI,          JC_LARGELI,             CB_JC },  //fnSetSetJM
/*      */  { ITM_EXFRAC,           JC_EXFRAC,              CB_JC },  //fnSetSetJM
/*      */  { ITM_EXTX,             JC_EXTENTX,             CB_JC },  //fnSetSetJM
/*      */  { ITM_EXTY,             JC_EXTENTY,             CB_JC },  //fnSetSetJM
/* 1892 */  { ITM_TEST,             JC_ITM_TST,             CB_JC },  //fnSetInlineTest
/*      */  { ITM_PLINE,            JC_PLINE,               CB_JC },  //
/*      */  { ITM_PCROS,            JC_PCROS,               CB_JC },  //
/*      */  { ITM_PBOX ,            JC_PBOX,                CB_JC },  //
/*      */  { ITM_INTG,             JC_INTG,                CB_JC },  //
/*      */  { ITM_DIFF,             JC_DIFF,                CB_JC },  //
/*      */  { ITM_RMS ,             JC_RMS ,                CB_JC },  //
/*      */  { ITM_SHADE ,           JC_SHADE ,              CB_JC },  //

/*      */  { CHR_num,              JC_NL,                  CB_JC },  //
/*      */  { CHR_case,             JC_UC,                  CB_JC },  //
/*      */  { ITM_USERMODE,         JC_UU,                  CB_JC },  //

/*      */  { ITM_BCD,              JC_BCD,                 CB_JC },  //
/*      */  { ITM_TOPHEX,           JC_TOPHEX,              CB_JC },  //

/*      */  { ITM_SI_All,           JC_SI_All,              CB_JC },  //


/* 1685 */  { ITM_2BIN,             2,                      RB_HX  }, //fnChangeBaseJM
/* 1686 */  { ITM_2OCT,             8,                      RB_HX  }, //fnChangeBaseJM
/* 1687 */  { ITM_2DEC,             10,                     RB_HX  }, //fnChangeBaseJM
/* 1688 */  { ITM_2HEX,             16,                     RB_HX  }  //fnChangeBaseJM


};



int8_t fnCbIsSet(int16_t item) {
  int8_t result = NOVAL;
  int16_t itemNr = max(item, -item);
  size_t n = nbrOfElements(indexOfRadioCbEepromItems);
  for(uint_fast8_t i = 0; i < n; i++) {
    if(indexOfRadioCbEepromItems[i].itemNr == itemNr) {
      //printf("^^^^**** item found %d\n",itemNr);
      bool_t is_cb = false;
      int32_t rb_param = 0;
      bool_t cb_param = false;

      switch(indexOfRadioCbEepromItems[i].radioButton) {
      case RB_AM: {
        rb_param = currentAngularMode;
      }
      break;

      case RB_CM: {
        if(getSystemFlag(FLAG_POLAR)) { rb_param = CM_POLAR;        }
        else {                          rb_param = CM_RECTANGULAR;  }
      }
      break;

      case RB_CU: {
        if(getSystemFlag(FLAG_CPXj)) {  rb_param = CU_J;  }
        else {                          rb_param = CU_I;  }
      }
      break;

      case RB_DF: {
        if(getSystemFlag(FLAG_YMD)) {       rb_param = DF_YMD;  }
        else if(getSystemFlag(FLAG_DMY)) {  rb_param = DF_DMY;  }
        else {                              rb_param = DF_MDY;  }
      }
      break;

      case RB_DI: {
        rb_param = displayFormat;
      }
      break;

      case RB_DO: {
        if(getSystemFlag(FLAG_ALLENG)) {  rb_param = DO_ENG;  }
        else {                            rb_param = DO_SCI;  }
      }
      break;

      case RB_ID: {
        rb_param = Input_Default;
      }
      break;

      case RB_IM: {
        rb_param = shortIntegerMode;
      }
      break;

      case RB_PS: {
        if(getSystemFlag(FLAG_MULTx)) { rb_param = PS_CROSS;  }
        else {                          rb_param = PS_DOT;    }
      }
      break;

      case RB_RX: {
        if(getSystemFlag(FLAG_DECIMP)) {  rb_param = RX_PERIOD; }
        else {                            rb_param = RX_COMMA;  }
      }
      break;

      case RB_WS: {
        rb_param = shortIntegerWordSize;
      }
      break;

      case RB_SS: {
        if(getSystemFlag(FLAG_SSIZE8)) {  rb_param = SS_8;  }
        else {                            rb_param = SS_4;  }
      }
      break;

      case RB_TF: {
        if(getSystemFlag(FLAG_TDM24)) { rb_param = TF_H24;  }
        else {                          rb_param = TF_H12;  }
      }
      break;

      case RB_F: {
        rb_param = LongPressF;;
      }
      break;

      case RB_M: {
        rb_param = LongPressM;
      }
      break;

      case RB_BCD: {
        rb_param = bcdDisplaySign;
      }
      break;

      case RB_SA: {
        rb_param = 16384+Norm_Key_00_VAR;
        //printf("^^^^*** activated %d\n",rb_param);
      }
      break;

      case RB_HX: {
        if(lastIntegerBase != 0) {  rb_param = lastIntegerBase; }
        else                        return result;
      }
      break;

      case CB_JC: {
        is_cb = true;

        switch(indexOfRadioCbEepromItems[i].param) {



        case JC_LINEAR_FITTING      : {  cb_param = (lrSelection & CF_LINEAR_FITTING     ) == CF_LINEAR_FITTING      ; } break;
        case JC_EXPONENTIAL_FITTING : {  cb_param = (lrSelection & CF_EXPONENTIAL_FITTING) == CF_EXPONENTIAL_FITTING ; } break;
        case JC_LOGARITHMIC_FITTING : {  cb_param = (lrSelection & CF_LOGARITHMIC_FITTING) == CF_LOGARITHMIC_FITTING ; } break;
        case JC_POWER_FITTING       : {  cb_param = (lrSelection & CF_POWER_FITTING      ) == CF_POWER_FITTING       ; } break;
        case JC_ROOT_FITTING        : {  cb_param = (lrSelection & CF_ROOT_FITTING       ) == CF_ROOT_FITTING        ; } break;
        case JC_HYPERBOLIC_FITTING  : {  cb_param = (lrSelection & CF_HYPERBOLIC_FITTING ) == CF_HYPERBOLIC_FITTING  ; } break;
        case JC_PARABOLIC_FITTING   : {  cb_param = (lrSelection & CF_PARABOLIC_FITTING  ) == CF_PARABOLIC_FITTING   ; } break;
        case JC_CAUCHY_FITTING      : {  cb_param = (lrSelection & CF_CAUCHY_FITTING     ) == CF_CAUCHY_FITTING      ; } break;
        case JC_GAUSS_FITTING       : {  cb_param = (lrSelection & CF_GAUSS_FITTING      ) == CF_GAUSS_FITTING       ; } break;
        case JC_ORTHOGONAL_FITTING  : {  cb_param = (lrSelection & CF_ORTHOGONAL_FITTING ) == CF_ORTHOGONAL_FITTING  ; } break;



        case JC_BASE_HOME: {
          cb_param = SH_BASE_HOME;
        }
        break;

        case JC_BCR: {
          cb_param = getSystemFlag(FLAG_CPXRES);
        }
        break;

        case JC_FRC: {
          cb_param = getSystemFlag(FLAG_FRCSRN);
        }
        break;

        case JC_BSR: {                                      //JMvv
          cb_param = getSystemFlag(FLAG_SPCRES);
        }
        break;                                              //JM^^

        case JC_BLZ: {
          cb_param = getSystemFlag(FLAG_LEAD0);
        }
        break;

        case DM_ANY: {
          cb_param = getSystemFlag(FLAG_DENANY);
        }
        break;

        case DM_FIX: {
          cb_param = getSystemFlag(FLAG_DENFIX);
        }
        break;

        case JC_ERPN: {
          cb_param = eRPN;
        }
        break;

        case JC_BASE_SCREEN: {
          cb_param = jm_BASE_SCREEN;
        }
        break;

        case JC_FG_LINE: {
          cb_param = jm_FG_LINE;
        }
        break;

        case JC_G_DOUBLETAP: {
          cb_param = jm_G_DOUBLETAP;
        }
        break;

        case JC_HOME_TRIPLE: {
          cb_param = HOME3;
        }
        break;

        case JC_SHFT_4s: {
          cb_param = ShiftTimoutMode;
        }
        break;

        case JC_VECT: {
          cb_param = PLOT_VECT;
        }
        break;

        case JC_NVECT: {
          cb_param = PLOT_NVECT;
        }
        break;

        case JC_SCALE: {
          cb_param = PLOT_SCALE;
        }
        break;

        case JC_H_SUM: {
          cb_param = jm_HOME_SUM;
        }
        break;

        case JC_LARGELI: {
          cb_param = jm_LARGELI;
        }
        break;

        case JC_EXFRAC: {
          cb_param = constantFractions;
        }
        break;

        case JC_EXTENTX: {
          cb_param = !extentx;
        }
        break;

        case JC_EXTENTY: {
          cb_param = !extenty;
        }
        break;

        case JC_PLINE: {
          cb_param = PLOT_LINE;
        }
        break;

        case JC_PCROS: {
          cb_param = PLOT_CROSS;
        }
        break;

        case JC_PBOX: {
          cb_param = PLOT_BOX;
        }
        break;

        case JC_DIFF: {
          cb_param = PLOT_DIFF;
        }
        break;

        case JC_INTG: {
          cb_param = PLOT_INTG;
        }
        break;

        case JC_RMS: {
          cb_param = PLOT_RMS;
        }
        break;

        case JC_SHADE: {
          cb_param = PLOT_SHADE;
        }
        break;

        case JC_NL: {
          cb_param = numLock;
        }
        break;

        case JC_UC: {
          cb_param = !alphaCase;
        }
        break;

        case JC_UU: {
          cb_param = getSystemFlag(FLAG_USER);
        }
        break;

        case JC_BCD: {
          cb_param = bcdDisplay;
        }
        break;

        case JC_TOPHEX: {
          cb_param = topHex;
        }
        break;

        case JC_SI_All: {
          cb_param = SI_All;
        }
        break;


#ifdef INLINE_TEST
        case JC_ITM_TST: {
          cb_param = testEnabled;
        }
        break;
#endif

        default:
          break;
        }
      }
      break;

      default:
        break;
      }

      if(is_cb) {
        //printf("^^^^*** %d %d\n",indexOfRadioCbEepromItems[i].param, cb_param);
        result = cb_param ? CB_TRUE : CB_FALSE;
      }
      else {
        //printf("^^^^*** %d %d\n",indexOfRadioCbEepromItems[i].param, rb_param);
        result = (indexOfRadioCbEepromItems[i].param == rb_param) ? RB_TRUE : RB_FALSE;
      }
    }
  }

  return result;
}



void fnRefreshState(void) {
#ifndef TESTSUITE_BUILD
  doRefreshSoftMenu = true;
#endif
}



int16_t fnItemShowValue(int16_t item) {
  int16_t result = NOVAL;
  uint16_t itemNr = max(item, -item);

  switch(itemNr) {
  case ITM_DSTACK:  //  132
    result = displayStack;
    break;

  case ITM_TDISP:
    result = timeDisplayFormatDigits;
    break;

  case ITM_SHOIREP:
    result = displayStackSHOIDISP;
    break;

  case ITM_GAP:     //  215
    result = groupingGap;
    break;

  case ITM_FIX:     //  185
    if(displayFormat == DF_FIX) {
      result = displayFormatDigits;
    }
    break;

  case ITM_SIGFIG:  // 1682
    if(displayFormat == DF_SF) {
      result = displayFormatDigits;
    }
    break;

  case ITM_ENG:     //  145
    if(displayFormat == DF_ENG) {
      result = displayFormatDigits;
    }
    break;

  case ITM_UNIT:    // 1693
    if(displayFormat == DF_UN) {
      result = displayFormatDigits;
    }
    break;

  case ITM_SCI:     //  545
    if(displayFormat == DF_SCI) {
      result = displayFormatDigits;
    }
    break;

  case ITM_ALL:     //   20
    if(displayFormat == DF_ALL) {
      result = displayFormatDigits;
    }
    break;

  case ITM_PZOOMX:
    result = PLOT_ZMX;
    break;

  case ITM_PZOOMY:
    result = PLOT_ZMY;
    break;

  case ITM_WSIZE:   //  664
    result = shortIntegerWordSize;
    break;

  case ITM_RNG:     //
    result = exponentLimit;
    break;

  case ITM_DENMAX2:     //
    result = denMax;
    break;

  case ITM_SETSIG2:     //
    result = significantDigits == 0 ? 34 : significantDigits;
    break;

  case ITM_DSPCYCLE:     //
    result = 32700 + displayFormat;
    break;

  case ITM_DSP:     //
    result = displayFormatDigits;
    break;


  case ITM_HIDE:     //
    result = exponentHideLimit;
    break;

//  case ITM_BESTF:     //
//    result = (~lrSelection) & 0x1FF;
//    break;

  case ITM_RMODE:      //
    result = roundingMode;
    break;

  case ITM_HASH_JM: //
    if(lastIntegerBase != 0)
      result = lastIntegerBase;
    else
      result = NOVAL;
    break;

  default:
    if(indexOfItems[itemNr].func == itemToBeCoded) {
      result = ITEM_NOT_CODED;
    }
    break;
  }

  return result;
}



char tmp[12];

void add_digitglyph_to_tmp2(char* tmp2, int16_t xx) {
  tmp2[0] = 0;

  switch(xx) {
    case  0: strcat(tmp2, STD_SUB_0);   break;
    case  1: strcat(tmp2, STD_BASE_1);  break;
    case  2: strcat(tmp2, STD_BASE_2);  break;
    case  3: strcat(tmp2, STD_BASE_3);  break;
    case  4: strcat(tmp2, STD_BASE_4);  break;
    case  5: strcat(tmp2, STD_BASE_5);  break;
    case  6: strcat(tmp2, STD_BASE_6);  break;
    case  7: strcat(tmp2, STD_BASE_7);  break;
    case  8: strcat(tmp2, STD_BASE_8);  break;
    case  9: strcat(tmp2, STD_BASE_9);  break;
    case 10: strcat(tmp2, STD_BASE_10); break;
    case 11: strcat(tmp2, STD_BASE_11); break;
    case 12: strcat(tmp2, STD_BASE_12); break;
    case 13: strcat(tmp2, STD_BASE_13); break;
    case 14: strcat(tmp2, STD_BASE_14); break;
    case 15: strcat(tmp2, STD_BASE_15); break;
    case 16: strcat(tmp2, STD_BASE_16); break;
    default:                            break;
  }
}



void use_base_glyphs(char* tmp1, int16_t xx) {              // Needs non-local variable tmp2
  char tmp2[12];
  tmp1[0] = 0;
  
  if(xx <= 16) {
    add_digitglyph_to_tmp2(tmp2, xx);
    strcat(tmp1, tmp2);
  }
  else if(xx <= 99) {
    add_digitglyph_to_tmp2(tmp2, xx / 10);
    strcat(tmp1, tmp2);
    add_digitglyph_to_tmp2(tmp2, xx % 10);
    strcat(tmp1, tmp2);
  }
  else if(xx ==999) {
    add_digitglyph_to_tmp2(tmp2, 9);   strcat(tmp1, tmp2);
    add_digitglyph_to_tmp2(tmp2, 9);   strcat(tmp1, tmp2);
    add_digitglyph_to_tmp2(tmp2, 9);   strcat(tmp1, tmp2);
  }
  else if(xx ==6145) {
    add_digitglyph_to_tmp2(tmp2, 6);   strcat(tmp1, tmp2);
    add_digitglyph_to_tmp2(tmp2, 1);   strcat(tmp1, tmp2);
    add_digitglyph_to_tmp2(tmp2, 4);   strcat(tmp1, tmp2);
    add_digitglyph_to_tmp2(tmp2, 5);   strcat(tmp1, tmp2);
  }
  else {
    snprintf(tmp1, 12, "%d", xx);
  }
}



char *figlabel(const char *label, const char* showText, int16_t showValue) {      //JM
  char tmp1[12];
  tmp[0] = 0;

  if(strlen(label) <= 12) {
    strcpy(tmp, label);
  }

  if(showValue != NOVAL && showValue != ITEM_NOT_CODED && showValue < 0) {
    strcat(tmp, STD_SUB_MINUS);
  }

  if(showValue != NOVAL && showValue != ITEM_NOT_CODED && strlen(label) <= 8) {
    showValue = max(showValue, -showValue);
    use_base_glyphs(tmp1, showValue);
    strcat(tmp, tmp1);
  }

  if(showText[0] != 0 && strlen(tmp)+strlen(showText) + 1 <= 12) {
    //strcat(tmp, showText);
    uint16_t ii = 0;
    while (showText[ii] != 0) {
       if(showText[ii]>='A' && showText[ii]<='Z') {
         strcat(tmp, STD_SUB_A);
         tmp[strlen(tmp)-1] += showText[ii]-'A';
       }
    ii++;
    }
  }
  return tmp;
}                                                           //JM ^^
