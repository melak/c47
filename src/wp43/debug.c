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
 * \file debug.c
 ***********************************************/

#include "debug.h"

#include "charString.h"
#include "defines.h"
#include "display.h"
#include "flags.h"
#include "fonts.h"
#include "items.h"
#include "registers.h"
#include <string.h>

#include "wp43.h"



TO_QSPI const char typeName[4][10 + 1][24 /* 21 characters + 1 sentinel + 2 padding */] = {
  {
    "???",
    "long integer",
    "real34",
    "complex34",
    "time",
    "date",
    "string",
    "real34 matrix",
    "complex34 matrix",
    "short integer",
    "config data",
    //"label",
    //"system integer",
    //"flags",
    //"pgm step",
    //"directory",
  },
  {
    "???                  ",
    "long integer         ",
    "real34               ",
    "complex34            ",
    "time                 ",
    "date                 ",
    "string               ",
    "real34 matrix        ",
    "complex34 matrix     ",
    "short integer        ",
    "config data          ",
    //"label                ",
    //"system integer       ",
    //"flags                ",
    //"pgm step             ",
    //"directory            ",
  },
  {
    "a ???",
    "a long integer",
    "a real34",
    "a complex34",
    "a time",
    "a date",
    "a string",
    "a real34 matrix",
    "a complex34 matrix",
    "a short integer",
    "a config data",
    //"a label",
    //"a system integer",
    //"a flags",
    //"a pgm step",
    //"a directory",
  },
  {
    "a ???                ",
    "a long integer       ",
    "a real34             ",
    "a complex34          ",
    "a time               ",
    "a date               ",
    "a string             ",
    "a real34 matrix      ",
    "a complex34 matrix   ",
    "a short integer      ",
    "a config data        ",
    //"a label              ",
    //"a system integer     ",
    //"a flags              ",
    //"a pgm step           ",
    //"a directory          ",
  },
};



/********************************************//**
 * \brief Returns the name of a data type
 *
 * \param[in] dt uint16_t Data type
 * \return char*          Name of the data type
 ***********************************************/
const char * getDataTypeName(uint16_t dt, bool_t article, bool_t padWithBlanks) {
  if(article && padWithBlanks) {
    switch(dt) {
      case dtLongInteger:
      case dtTime:
      case dtDate:
      case dtString:
      case dtReal34Matrix:
      case dtComplex34Matrix:
      case dtShortInteger:
      case dtReal34:
      case dtComplex34:
      case dtConfig:
      //case dtLabel:
      //case dtSystemInteger:
      //case dtFlags:
      //case dtPgmStep:
      //case dtDirectory:
      {
        return typeName[3][dt + 1];
      }
      default: {
        return typeName[3][0];
      }
    }
  }
  else if(article && !padWithBlanks) {
    switch(dt) {
      case dtLongInteger:
      case dtTime:
      case dtDate:
      case dtString:
      case dtReal34Matrix:
      case dtComplex34Matrix:
      case dtShortInteger:
      case dtReal34:
      case dtComplex34:
      case dtConfig:
      //case dtLabel:
      //case dtSystemInteger:
      //case dtFlags:
      //case dtPgmStep:
      //case dtDirectory:
      {
        return typeName[2][dt + 1];
      }
      default: {
        return typeName[2][0];
      }
    }
  }
  else if(!article && padWithBlanks) {
    switch(dt) {
      case dtLongInteger:
      case dtTime:
      case dtDate:
      case dtString:
      case dtReal34Matrix:
      case dtComplex34Matrix:
      case dtShortInteger:
      case dtReal34:
      case dtComplex34:
      case dtConfig:
      //case dtLabel:
      //case dtSystemInteger:
      //case dtFlags:
      //case dtPgmStep:
      //case dtDirectory:
      {
        return typeName[1][dt + 1];
      }
      default: {
        return typeName[1][0];
      }
    }
  }
  else if(!article && !padWithBlanks) {
    switch(dt) {
      case dtLongInteger:
      case dtTime:
      case dtDate:
      case dtString:
      case dtReal34Matrix:
      case dtComplex34Matrix:
      case dtShortInteger:
      case dtReal34:
      case dtComplex34:
      case dtConfig:
      //case dtLabel:
      //case dtSystemInteger:
      //case dtFlags:
      //case dtPgmStep:
      //case dtDirectory:
      {
        return typeName[0][dt + 1];
      }
      default: {
        return typeName[0][0];
      }
    }
  }
  else {
    return typeName[0][0];
  }
}



/********************************************//**
 * \brief Returns the name of a data type of a register
 *
 * \param[in] dt calcRegister_t register
 * \return char* Name of the data type
 ***********************************************/
const char * getRegisterDataTypeName(calcRegister_t regist, bool_t article, bool_t padWithBlanks) {
  return getDataTypeName(getRegisterDataType(regist), article, padWithBlanks);
}



const char * getRegisterTagName(calcRegister_t regist, bool_t padWithBlanks) {
  static char base[9];

  switch(getRegisterDataType(regist)) {
    case dtLongInteger: {
      switch(getRegisterTag(regist)) {
        case LI_ZERO: {
          return "zero    ";
        }
        case LI_NEGATIVE: {
          return "negative";
        }
        case LI_POSITIVE: {
          return "positive";
        }
        default: {
          return "???     ";
        }
      }
      }

    case dtReal34: {
      switch(getRegisterTag(regist)) {
        case amRadian: {
          return "radian  ";
        }
        case amMultPi: {
          return "multPi  ";
        }
        case amGrad: {
          return "grad    ";
        }
        case amDegree: {
          return "degree  ";
        }
        case amDMS: {
          return "dms     ";
        }
        case amNone: {
          return "none    ";
        }
        default: {
         return "???     ";
        }
      }
      }

    case dtComplex34: {
      if(getComplexRegisterPolarMode(regist)) {
        return "Polar   ";
      }
      else {
        return "Rect    ";
      }
    }

    case dtString:
    case dtReal34Matrix:
    case dtComplex34Matrix:
    case dtDate:
    case dtTime:
    case dtConfig: {
      switch(getRegisterTag(regist)) {
        case amNone: {
          return "        ";
        }
        default: {
          return "???     ";
        }
      }
      }

    case dtShortInteger: {
      sprintf(base, "base %2" PRIu32 " ", getRegisterTag(regist));
                                    return base;
    }

    default: {
      return "???     ";
    }
  }
}


/********************************************//**
 * \brief Returns the single name of a curvefitting mode, or ??? if multiple names are defined in bits
 * \param[in] am uint16_t curvefitting mode
 * \return char*          Name of the curvefitting mode
 ***********************************************/
const char * getCurveFitModeName(uint16_t selection) {          //Can be only one bit. ??? if invalid.
    switch( selection & 0x03FF ){
      case CF_LINEAR_FITTING: {
        return "Linear     ";
      }
      case CF_EXPONENTIAL_FITTING: {
        return "Exponential";
      }
      case CF_LOGARITHMIC_FITTING: {
        return "Logarithmic";
      }
      case CF_POWER_FITTING: {
        return "Power      ";
      }
      case CF_ROOT_FITTING: {
        return "Root       ";
      }
      case CF_HYPERBOLIC_FITTING: {
        return "Hyperbolic ";
      }
      case CF_PARABOLIC_FITTING: {
        return "Parabolic  ";
      }
      case CF_CAUCHY_FITTING: {
        return "Cauchy peak";
      }
      case CF_GAUSS_FITTING: {
        return "Gauss peak ";
      }
      case CF_ORTHOGONAL_FITTING: {
        return "Orthogonal ";
      }
      default: {
        return "???        ";
        break;
      }
    }
  }


/********************************************//**
 * \brief Remove trailing spaces from the curvefitting mode name
 *
 ***********************************************/
char * eatSpacesEnd(const char * ss) {
  static char tmp_names[20];
  int8_t ix;

  strcpy(tmp_names, ss);
  ix = stringByteLength(ss)-1;
  while(ix > 0) {
    if(ss[ix] == ' ') {
      tmp_names[ix] = 0;
    }
    else {
      break;
    }
    ix--;
  }
  return tmp_names;
}


/********************************************//**
 * \brief Remove spaces from the curvefitting mode name
 *
 ***********************************************/
char * eatSpacesMid(const char * ss) {
  static char tmp_names[20];
  char tt[50];
  int8_t ix = 0, iy = 0;

  strcpy(tt, ss);
  tmp_names[0] = 0;
  while(tt[ix] != 0 && ix < 50) {
    if(tt[ix] != ' ') {
      tmp_names[iy++] = tt[ix];
    }
    ix++;
  }
  tmp_names[iy] = 0;
  tmp_names[iy++] = 0;
  return tmp_names;
}


/********************************************//**
 * \brief Returns all selected names of the curve fit types
 * \note that a single bit EXCLUDES a method
 *
 * \param[in] dt uint16_t Data type
 * \return char*          Name of the curvefit type
 ***********************************************/
const char * getCurveFitModeNames(uint16_t selection) {
#if defined(PC_BUILD)
  uint16_t ix;

  errorMessage[0] = 0;
  for(ix = 0; ix < 10; ix++) {
    if(selection & (1 << ix)) {
      strcat(errorMessage, errorMessage[0] == 0 ? "" : " ");
      strcat(errorMessage, eatSpacesEnd(getCurveFitModeName(1 << ix)));
    }
  }
  if(errorMessage[0] == 0) {
    return "???        ";
  }
  return errorMessage;
#else // defined(PC_BUILD)
  return "";
#endif // defined(PC_BUILD)
}


/********************************************//**
 * \brief Returns the formula of a curvefitting mode
 *
 * \param[in] am uint16_t curvefitting mode
 * \return char*          Formula of the curvefitting mode
 ***********************************************/
const char * getCurveFitModeFormula(uint16_t selection) {          //Can be only one bit. ??? if invalid.
  switch( selection & 0x03FF ){
    case CF_LINEAR_FITTING: {
      return "a" STD_SUB_0 STD_SPACE_3_PER_EM "+" STD_SPACE_3_PER_EM "a" STD_SUB_1 "x";
    }
    case CF_EXPONENTIAL_FITTING: {
      return "a" STD_SUB_0 STD_SPACE_3_PER_EM "e^(a" STD_SUB_1 "x)";
    }
    case CF_LOGARITHMIC_FITTING: {
      return "a" STD_SUB_0 STD_SPACE_3_PER_EM "+" STD_SPACE_3_PER_EM "a" STD_SUB_1 "ln(x)";
    }
    case CF_POWER_FITTING: {
      return "a" STD_SUB_0 STD_SPACE_3_PER_EM "x^a" STD_SUB_1 ;
    }
    case CF_ROOT_FITTING: {
      return "a" STD_SUB_0 STD_SPACE_3_PER_EM "a" STD_SUB_1 "^(1/x)";
    }
    case CF_HYPERBOLIC_FITTING: {
      return "(a" STD_SUB_0 STD_SPACE_3_PER_EM "+" STD_SPACE_3_PER_EM "a" STD_SUB_1 "x)" STD_SUP_MINUS_1;
    }
    case CF_PARABOLIC_FITTING: {
      return "a" STD_SUB_0 STD_SPACE_3_PER_EM "+" STD_SPACE_3_PER_EM "a" STD_SUB_1 "x" STD_SPACE_3_PER_EM "+" STD_SPACE_3_PER_EM "a" STD_SUB_2 "x" STD_SUP_2;
    }
    case CF_CAUCHY_FITTING: {
      return STD_LEFT_SQUARE_BRACKET "a" STD_SUB_0 "(x+a" STD_SUB_1 ")" STD_SUP_2 "+a" STD_SUB_2 STD_RIGHT_SQUARE_BRACKET STD_SUP_MINUS_1;
    }
    case CF_GAUSS_FITTING: {
      return "a" STD_SUB_0 "e^" STD_LEFT_SQUARE_BRACKET  "(x-a" STD_SUB_1 ")" STD_SUP_2 "/a" STD_SUB_2 STD_RIGHT_SQUARE_BRACKET;
    }
    case CF_ORTHOGONAL_FITTING: {
      return "a" STD_SUB_0 STD_SPACE_3_PER_EM "+" STD_SPACE_3_PER_EM "a" STD_SUB_1 "x";
    }
    default: {
      return "???        "; break;
    }
  }
}



/********************************************//**
 * \brief Returns the name of a angular mode
 *
 * \param[in] am uint16_t Angular mode
 * \return char*          Name of the angular mode
 ***********************************************/
const char * getAngularModeName(angularMode_t angularMode) {
  switch(angularMode) {
    case amRadian: {
      return "radian";
    }
    case amMultPi: {
      return "multPi";
    }
    case amGrad: {
      return "grad  ";
    }
    case amDegree: {
      return "degree";
    }
    case amDMS: {
      return "d.ms  ";
    }
    case amNone: {
      return "none  ";
    }
    default: {
      return "???   ";
    }
  }
}


/********************************************//**
 * \brief Returns the name of an integer mode
 *
 * \param[in] im uint16_t Integer mode
 * \return char*          Name of the integer mode
 ***********************************************/
const char * getShortIntegerModeName(uint16_t im) {
  switch(im) {
    case SIM_1COMPL: {
      return "1compl";
    }
    case SIM_2COMPL: {
      return "2compl";
    }
    case SIM_SIGNMT: {
      return "signmt";
    }
    case SIM_UNSIGN: {
      return "unsign";
    }
    default: {
      return "???   ";
    }
  }
}


/* Never used
void debugNIM(void) {
  switch(nimNumberPart) {
    case NP_EMPTY: {
      printf("nimNumberPart = NP_EMPTY               \n");
    }
    case NP_INT_10: {
      printf("nimNumberPart = NP_INT_10              \n");
    }
    case NP_INT_16: {
      printf("nimNumberPart = NP_INT_16              \n");
    }
    case NP_INT_BASE: {
      printf("nimNumberPart = NP_INT_BASE            \n");
    }
    case NP_REAL_FLOAT_PART: {
      printf("nimNumberPart = NP_REAL_FLOAT_PART     \n");
    }
    case NP_REAL_EXPONENT: {
      printf("nimNumberPart = NP_REAL_EXPONENT       \n");
    }
    case NP_FRACTION_DENOMINATOR: {
      printf("nimNumberPart = NP_FRACTION_DENOMINATOR\n");
    }
    case NP_COMPLEX_INT_PART: {
      printf("nimNumberPart = NP_COMPLEX_INT_PART    \n");
    }
    case NP_COMPLEX_FLOAT_PART: {
      printf("nimNumberPart = NP_COMPLEX_FLOAT_PART  \n");
    }
    case NP_COMPLEX_EXPONENT: {
      printf("nimNumberPart = NP_COMPLEX_EXPONENT    \n");
    }
    default: {
      printf("nimNumberPart = NP_???                 \n");
    }
  }
}
*/


#if(DEBUG_PANEL == 1)
  /********************************************//**
   * \brief Returns the name of a display format
   *
   * \param[in] df uint16_t Display format
   * \return char*          Name of the display format
   ***********************************************/
  char * getDisplayFormatName(uint16_t df) {
    switch(df) {
      case DF_ALL: {
        return "ALL";
      }
      case DF_FIX: {
        return "FIX";
      }
      case DF_SCI: {
        return "SCI";
      }
      case DF_ENG: {
        return "ENG";
      }
      case DF_SF: {
        return "SIG";
      }
      case DF_UN: {
        return "UNT";
        }
      default: {
    return "???";
  }
    }
  }


  /********************************************//**
   * \brief Returns the name of a time format
   *
   * \param[in] tf bool_t Time format
   * \return char*        Name of the time format
   ***********************************************/
  char * getTimeFormatName(bool_t tf) {
    return (tf ? "H24" : "H12");
  }


  /********************************************//**
   * \brief Returns the name of a date format
   *
   * \param[in] df uint16_t Date format
   * \return char*          Name of the date format
   *
   ***********************************************/
  char * getDateFormatName(uint16_t df) {
    switch(df) {
      case DF_DMY: {
        return "D.MY";
      }
      case DF_MDY: {
        return "M.DY";
      }
      case DF_YMD: {
        return "Y.MD";
      }
      default: {
    return "??? ";
  }
    }
  }


  /********************************************//**
   * \brief Returns the name of a boolean
   *
   * \param[in] b bool_t Boolean
   * \return char*       Name of the boolean
   ***********************************************/
  char * getBooleanName(bool_t b) {
    return (b ? "true" : "false");
  }


  /********************************************//**
   * \brief Returns the name of a RBR mode
   *
   * \param[in] mode uint16_t
   * \return char*         Name of the mode
   ***********************************************/
  char * getRbrModeName(uint16_t mode) {
    switch(mode) {
      case RBR_GLOBAL:  {
      return "RBR_GLOBAL";
    }
      case RBR_LOCAL: {
      return "RBR_LOCAL";
    }
      case RBR_NAMED: {
      return "RBR_NAMED";
    }
      default: {
     return "RBR_???";
   }
  }
  }


  /********************************************//**
   * \brief Returns the name of a rounding mode
   *
   * \param[in] rm uint16_t Rounding mode
   * \return char*          Name of the rounding mode
   ***********************************************/
  char * getRoundingModeName(uint16_t rm) {
    switch(rm) {
      case RM_CEIL: {
        return "ceil     ";
      }
      case RM_DOWN: {
        return "down     ";
      }
      case RM_FLOOR: {
        return "floor    ";
      }
      case RM_HALF_DOWN: {
        return "half down";
      }
      case RM_HALF_EVEN: {
        return "half even";
      }
      case RM_HALF_UP: {
        return "half up  ";
      }
      case RM_UP: {
        return "up       ";
      }
      default: {
    return "???      ";
  }
    }
  }


  /********************************************//**
   * \brief Returns the name of a calc mode
   *
   * \param[in] cm uint16_t Calc mode
   * \return char*          Name of the calc mode
   ***********************************************/
  char * getCalcModeName(uint16_t cm) {
    switch(cm) {
      case CM_NORMAL: {
        return "normal ";
      }
      case CM_AIM: {
        return "aim    ";
      }
      case CM_EIM: {
        return "eim    ";
      }
      case CM_PEM: {
        return "pem    ";
      }
      case CM_NIM: {
        return "nim    ";
      }
      case CM_ASSIGN: {
        return "assign ";
      }
      case CM_REGISTER_BROWSER: {
        return "reg.bro";
      }
      case CM_ASN_BROWSER: {
        return "asn.bro";
      }
      case CM_FLAG_BROWSER: {
        return "flg.bro";
      }
      case CM_FONT_BROWSER: {
        return "fnt.bro";
      }
      case CM_PLOT_STAT: {
        return "plot.st";
      }
      case CM_GRAPH: {
        return "plot.gr";
      }
      case CM_ERROR_MESSAGE: {
        return "err.msg";
      }
      case CM_BUG_ON_SCREEN: {
        return "bug.scr";
      }
      case CM_TIMER: {
        return "timer  ";
      }
      case CM_LISTXY: {
          return "listxy ";
      }
      default: {
    return "???    ";
  }
    }
  }


  /********************************************//**
   * \brief Returns the name of a TAM mode
   *
   * \param[in] tmm uint16_t TAM mode
   * \return char*          Name of the TAM mode
   ***********************************************/
  char * getTamModeName(uint16_t tmm) {
    switch(tmm) {
      case TM_VALUE: {
        return "Value   ";
      }
      case TM_VALUE_CHB: {
        return "Valuechb"; // change base (->#)
      }
      case TM_REGISTER: {
        return "Register";
      }
      case TM_FLAGR: {
        return "FlagR   ";
      }
      case TM_FLAGW: {
        return "FlagW   ";
      }
      case TM_STORCL: {
        return "STORCL  ";
      }
      case TM_M_DIM: {
        return "M_DIM   ";
      }
      case TM_SHUFFLE: {
        return "Shuffle ";
      }
      case TM_LABEL: {
        return "Label   ";
      }
      case TM_SOLVE: {
        return "Solve   ";
      }
      case TM_NEWMENU: {
        return "NewMenu ";
      }
      case TM_KEY: {
        return "Key     ";
      }
      case TM_INTEGRATE: {
        return "Integrat";
      }
      case TM_DELITM: {
        return "DelItem ";
      }
      case TM_CMP: {
        return "Compare ";
      }
      default: {
        return "???     ";
      }
    }
  }


  /********************************************//**
   * \brief Returns the name of a "next char indicator"
   *
   * \param[in] nc uint16_t Next char indicator
   * \return char*          Name of the "next char indicator"
   ***********************************************/
  char * getNextCharName(uint16_t nc) {
    switch(nc) {
      case NC_NORMAL: {
        return "normal     ";
      }
      case NC_SUBSCRIPT: {
        return "subscript  ";
      }
      case NC_SUPERSCRIPT: {
        return "superscript";
      }
      default: {
    return "???        ";
  }
    }
  }


  /********************************************//**
   * \brief Returns the name of a system flag
   *
   * \param[in] sf uint16_t System flag
   * \return char*          Name of the system flag
   ***********************************************/
  char * getSystemFlagName(uint16_t sf) {
    return (char *) indexOfItems[SFL_TDM24 + (sf & 0x3fff)].itemCatalogName;
  }


  /********************************************//**
   * \brief Returns the name of a complex unit
   *
   * \param[in] cu bool_t Complex unit
   * \return char*        Name of the complex unit
   ***********************************************/
  char * getComplexUnitName(bool_t cu) {
    return (cu ? "j" : "i");
  }


  /********************************************//**
   * \brief Returns the name of a Product sign
   *
   * \param[in] ps bool_t Product sign
   * \return char*        Name of the product sign
   ***********************************************/
  char * getProductSignName(bool_t ps) {
    return (ps ? "cross" : "dot  ");
  }


  /********************************************//**
   * \brief returns the name of a fraction type
   *
   * \param[in] ft bool_t Fraction type
   * \return char*        Name of the fraction type
   ***********************************************/
  char * getFractionTypeName(bool_t ft) {
    return (ft ? "proper a b/c" : "improper d/c");
  }


  /********************************************//**
   * \brief returns the name of denominator mode 1 type
   *
   * \param[in] ft bool_t Fraction type
   * \return char*        Name of the fraction type
   ***********************************************/
  char * getFractionDenom1ModeName(bool_t ft) {
    return (ft ? "up to D.MAX   " : "D.MAX or fact");
  }


  /********************************************//**
   * \brief returns the name of denominator mode 2 type
   *
   * \param[in] ft bool_t Fraction type
   * \return char*        Name of the fraction type
   ***********************************************/
  char * getFractionDenom2ModeName(bool_t ft) {
    return (ft ? "D.MAX         " : "fact of D.MAX");
  }


  /********************************************//**
   * \brief returns the name of a radix mark
   *
   * \param[in] rm bool_t Radix mark
   * \return char*        Name of the radix mark
   ***********************************************/
  char * getRadixMarkName(bool_t rm) {
    return (rm ? "period" :"comma ");
  }


  /********************************************//**
   * \brief returns the name of a display override mode
   *
   * \param[in] dio bool_t Display override mode
   * \return char*         Name of the override mode
   ***********************************************/
  char * getDisplayOvrName(bool_t dio) {
    return (dio ? "sciOvr" : "engOvr");
  }


  /********************************************//**
   * \brief returns the name of a stack size
   *
   * \param[in] ss bool_t Stack size
   * \return char*        Name of the stack size
   ***********************************************/
  char * getStackSizeName(bool_t ss) {
    return (ss ? "8 levels" : "4 levels");
  }


  /********************************************//**
   * \brief returns the name of a complex mode
   *
   * \param[in] cm bool_t Complex mode
   * \return char*        Name of the complex mode
   ***********************************************/
  char * getComplexModeName(bool_t cm) {
    return (cm ? "rectangular" : "polar      ");
  }


  /********************************************//**
   * \brief returns the name of an alpha case mode
   *
   * \param[in] ac uint16_t Alphacase mode
   * \return char*          Name of the alphacase mode
   ***********************************************/
  char * getAlphaCaseName(uint16_t ac) {
    if(ac == AC_LOWER) {
      return "lower";
    }
    if(ac == AC_UPPER) {
      return "upper";
    }

    return "???  ";
  }


  /********************************************//**
   * \brief returns the name of an alpha selection menu
   *
   * \param[in] asm uint16_t Alpha selection menu
   * \return char*          Name of the alpha selection menu
   ***********************************************/
  char * getAlphaSelectionMenuName(uint16_t alsm) {
    switch(alsm) {
      case CATALOG_NONE: {
        return "CATALOG_NONE";
      }
      case CATALOG_FCNS: {
        return "CATALOG_FCNS";
      }
      case CATALOG_CNST: {
        return "CATALOG_CNST";
      }
      case CATALOG_MENU: {
        return "CATALOG_MENU";
      }
      case CATALOG_SYFL: {
        return "CATALOG_SYFL";
      }
      case CATALOG_AINT: {
        return "CATALOG_AINT";
      }
      case CATALOG_aint: {
        return "CATALOG_aint";
      }
      case CATALOG_PROG: {
        return "CATALOG_PROG";
      }
      case CATALOG_VAR: {
        return "CATALOG_VAR";
      }
      case CATALOG_MATRS: {
        return "CATALOG_MATRS";
      }
      case CATALOG_STRINGS: {
        return "CATALOG_STRINGS";
      }
      case CATALOG_DATES: {
        return "CATALOG_DATES";
      }
      case CATALOG_TIMES: {
        return "CATALOG_TIMES";
      }
      case CATALOG_ANGLES: {
        return "CATALOG_ANGLES";
      }
      case CATALOG_SINTS: {
        return "CATALOG_SINTS";
      }
      case CATALOG_LINTS: {
        return "CATALOG_LINTS";
      }
      case CATALOG_REALS: {
        return "CATALOG_REALS";
      }
      case CATALOG_CPXS: {
        return "CATALOG_CPXS";
      }
      case CATALOG_MVAR: {
        return "CATALOG_MVAR";
      }
      default: {
    return "CATALOG_????";
  }
    }
  }



  /********************************************//**
   * \brief Fills the row th line of the debug window
   * with the value of register r
   *
   * \param[in] r int   Register number
   * \param[in] row int Row number
   * \return void
   *
   ***********************************************/
  void debugRegisterValue(calcRegister_t regist, int row) {
    char     string[3000], *p, tmpStr[1000];
    uint16_t i, k, n=0;

    if(FIRST_NAMED_VARIABLE <= regist && regist <= FIRST_NAMED_VARIABLE + numberOfNamedVariables) { // Named variable
      n = allNamedVariables[regist - FIRST_NAMED_VARIABLE].variableName[0];
      for(i=0, p=(char *)(allNamedVariables[regist - FIRST_NAMED_VARIABLE].variableName) + 1; i<n; i++, p++) {
        string[i] = *p;
      }
      string[i] = 0;

      strcat(string, STD_SPACE_3_PER_EM "=" STD_SPACE_3_PER_EM);
      n = stringByteLength(string);
    }

    if(getRegisterDataType(regist) == dtReal34) {
      formatReal34Debug(string + n, (real34_t *)getRegisterDataPointer(regist));
      if(getRegisterAngularMode(regist) != amNone) {
        n = stringByteLength(string);
        strcpy(string + n++, " ");
        strcpy(string + n, getAngularModeName(getRegisterAngularMode(regist)));
      }
    }

    else if(getRegisterDataType(regist) == dtComplex34) {
      formatComplex34Debug(string + n, (void *)getRegisterDataPointer(regist));
    }

    else if(getRegisterDataType(regist) == dtString) {
      if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chkHexaString))) {
        k = stringByteLength(REGISTER_STRING_DATA(regist));
        for(i=0, p=REGISTER_STRING_DATA(regist); i<=k; i++, p++) {
          sprintf(string + n + 4*i, STD_SPACE_3_PER_EM "%02x", *p);
        }
      }
      else {
        for(i=0, p=REGISTER_STRING_DATA(regist); i<=stringByteLength(REGISTER_STRING_DATA(regist)); i++, p++) {
          string[n + i] = *p;
        }
      }
    }

    else if(getRegisterDataType(regist) == dtShortInteger) {
      shortIntegerToDisplayString(regist, string + n, false);
      strcat(string + n, STD_SPACE_3_PER_EM);
      strcat(string + n, getShortIntegerModeName(shortIntegerMode));
    }

    else if(getRegisterDataType(regist) == dtLongInteger) {
      longIntegerRegisterToDisplayString(regist, string + n, sizeof(string) - n, SCREEN_WIDTH, 50, false);   //JM added last parameter: Allow LARGELI
    }

    else if(getRegisterDataType(regist) == dtTime) {
      formatReal34Debug(string + n, (real34_t *)getRegisterDataPointer(regist));
    }

    else if(getRegisterDataType(regist) == dtDate) {
      formatReal34Debug(string + n, (real34_t *)getRegisterDataPointer(regist));
    }

    else if(getRegisterDataType(regist) == dtConfig) {
      sprintf(string + n, "Configuration data");
    }

    else if(getRegisterDataType(regist) == dtReal34Matrix) {
      dataBlock_t* dblock = REGISTER_REAL34_MATRIX_DBLOCK(regist);
      sprintf(string + n, "Real Matrix of Size [%" PRIu16" x %" PRIu16" Matrix]", dblock->matrixRows, dblock->matrixColumns);
    }

    else if(getRegisterDataType(regist) == dtComplex34Matrix) {
      dataBlock_t* dblock = REGISTER_COMPLEX34_MATRIX_DBLOCK(regist);
      sprintf(string + n, "Complex Matrix of Size [%" PRIu16" x %" PRIu16" Matrix]", dblock->matrixRows, dblock->matrixColumns);
    }

    else {
      sprintf(string + n, "data type %s not supported for now!", getRegisterDataTypeName(regist, false, false));
    }

    *(stringAfterPixels(string, &standardFont, 961, true, true)) = 0;

    stringToUtf8(string, (uint8_t *)tmpStr);

    gtk_label_set_label(GTK_LABEL(lbl2[row]), tmpStr);
    gtk_widget_show(lbl2[row]);
  }



  /********************************************//**
   * \brief refreshes the debug window
   *
   * \param void
   * \return void
   ***********************************************/
  void refreshDebugPanel(void) {
    char string[200];
    int row;

    if(debugWindow == DBG_BIT_FIELDS) {
      for(int i=0; i<DEBUG_LINES; i++) {
        gtk_widget_hide(lbl1[i]);
        gtk_widget_hide(lbl2[i]);
      }

      row = 0;

      if(row < DEBUG_LINES) {
        sprintf(string, "TO_WP43MEMPTR(statisticalSumsPointer)    = %6d",         TO_WP43MEMPTR(statisticalSumsPointer));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "numberOfNamedVariables                    = %6u",         numberOfNamedVariables);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "FLAG_SSIZE8                               = %s",          getBooleanName(getSystemFlag(FLAG_SSIZE8)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "FLAG_FRACT                                = %s",          getBooleanName(getSystemFlag(FLAG_FRACT)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "FLAG_PROPFR                               = %s = %s",     getBooleanName(getSystemFlag(FLAG_PROPFR)), getFractionTypeName(getSystemFlag(FLAG_PROPFR)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "FLAG_HPRP                                 = %s = %s",     getBooleanName(getSystemFlag(FLAG_HPRP)), getFractionDenom1ModeName(getSystemFlag(FLAG_HPRP)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "FLAG_HPBASE                               = %s = %s",     getBooleanName(getSystemFlag(FLAG_HPBASE)), getFractionDenom1ModeName(getSystemFlag(FLAG_HPBASE)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "FLAG_DENANY                               = %s = %s",     getBooleanName(getSystemFlag(FLAG_DENANY)), getFractionDenom1ModeName(getSystemFlag(FLAG_DENANY)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "FLAG_DENFIX                               = %s = %s",     getBooleanName(getSystemFlag(FLAG_DENFIX)), getFractionDenom2ModeName(getSystemFlag(FLAG_DENFIX)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "denMax                                    = %6u\n",       denMax);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "shortIntegerMode                          = %6u = %s",    shortIntegerMode,     getShortIntegerModeName(shortIntegerMode));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "shortIntegerWordSize                      = %6u",         shortIntegerWordSize);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "displatFormat                             = %6u = %s",    displayFormat,        getDisplayFormatName(displayFormat));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "displatFormatDigits                       = %6u",         displayFormatDigits);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "timeDisplayFormatDigits                   = %6u",         timeDisplayFormatDigits);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "gapItemLeft                               = %6u",         gapItemLeft);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "gapItemRight                               = %6u",         gapItemRight);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "gapItemRadix                               = %6u",         gapItemRadix);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }
      if(row < DEBUG_LINES) {
        sprintf(string, "grpGroupingLeft                               = %6u",         grpGroupingLeft);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }
      if(row < DEBUG_LINES) {
        sprintf(string, "grpGroupingGr1LeftOverflow                               = %6u",         grpGroupingGr1LeftOverflow);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }
      if(row < DEBUG_LINES) {
        sprintf(string, "grpGroupingGr1Left                               = %6u",         grpGroupingGr1Left);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }
      if(row < DEBUG_LINES) {
        sprintf(string, "grpGroupingRight                               = %6u",         grpGroupingRight);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }


      if(row < DEBUG_LINES) {
        sprintf(string, "significantDigits                         = %6u",         significantDigits);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "roundingMode                              = %6u = %s",    roundingMode,         getRoundingModeName(roundingMode));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "currentAngularMode                        = %6u = %s",    currentAngularMode,   getAngularModeName(currentAngularMode));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "FLAG_TDM24 (time format)                  = %s = %s",     getBooleanName(getSystemFlag(FLAG_TDM24)), getTimeFormatName(getSystemFlag(FLAG_TDM24)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "FLAG_YMD (date format)                    = %s",          getBooleanName(getSystemFlag(FLAG_YMD)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "FLAG_DMY (date format)                    = %s",          getBooleanName(getSystemFlag(FLAG_DMY)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "FLAG_MDY (date format)                    = %s",          getBooleanName(getSystemFlag(FLAG_MDY)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "firstGregorianDay                         = %6u",         firstGregorianDay);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "FLAG_CPXj                                 = %s = %s",     getBooleanName(getSystemFlag(FLAG_CPXj)), getTimeFormatName(getSystemFlag(FLAG_CPXj)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "FLAG_LEAD.0                               = %s",          getBooleanName(getSystemFlag(FLAG_LEAD0)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "FLAG_POLAR (complex mode)                 = %s = %s",     getBooleanName(getSystemFlag(FLAG_POLAR)), getTimeFormatName(getSystemFlag(FLAG_POLAR)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "alphaCase                                 = %6u = %s",    alphaCase,            getAlphaCaseName(alphaCase));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "hourGlassIconEnabled                      = %6d = %s",    hourGlassIconEnabled, getBooleanName(hourGlassIconEnabled));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "watchIconEnabled                          = %6d = %s",    watchIconEnabled,     getBooleanName(watchIconEnabled));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "FLAG_USER                                 = %s",          getBooleanName(getSystemFlag(FLAG_USER)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "serialIOIconEnabled                       = %6d = %s",    serialIOIconEnabled,  getBooleanName(serialIOIconEnabled));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "printerIconEnabled                        = %6d = %s",    printerIconEnabled,   getBooleanName(printerIconEnabled));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "FLAG_LOWBAT                               = %s",          getBooleanName(getSystemFlag(FLAG_LOWBAT)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "FLAG_USB                                  = %s",          getBooleanName(getSystemFlag(FLAG_USB)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "FLAG_ASLIFT                               = %s",          getBooleanName(getSystemFlag(FLAG_ASLIFT)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "FLAG_ENDPMT                               = %s",          getBooleanName(getSystemFlag(FLAG_ENDPMT)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "calcMode                                  = %6u = %s",    calcMode,             getCalcModeName(calcMode));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "nextChar                                  = %6u = %s",    nextChar,             getNextCharName(nextChar));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "TAM mode                                  = %6u = %s",    tam.mode,             getTamModeName(tam.mode));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "shiftF                                    = %6d = %s",    shiftF,               getBooleanName(shiftF));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "shiftG                                    = %6d = %s\n",  shiftG,               getBooleanName(shiftG));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "xCursor                                   = %6u",         xCursor);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "yCursor                                   = %6u",         yCursor);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "cursorEnabled                             = %6u = %s",    cursorEnabled,        getBooleanName(cursorEnabled));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "currentFntScr                             = %6u",         currentFntScr);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "currentAsnScr                             = %6u",         currentAsnScr);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "currentRegisterBrowserScreen              = %6d",         currentRegisterBrowserScreen);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "currentFlgScr                             = %6u",         currentFlgScr);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "lastFlgScr                             = %6u",         lastFlgScr);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "rbrMode                                   = %6u = %s",    rbrMode,              getRbrModeName(rbrMode));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      if(row < DEBUG_LINES) {
        sprintf(string, "showContent                               = %6d = %s",    showContent,          getBooleanName(showContent));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }
    }

    else if(debugWindow == DBG_FLAGS) {
      for(int i=0; i<DEBUG_LINES; i++) {
        gtk_widget_hide(lbl1[i]);
        gtk_widget_hide(lbl2[i]);
      }

      row = 0;

      gtk_label_set_label(GTK_LABEL(lbl1[row]), "Flags:");
      gtk_widget_show(lbl1[row++]);

      gtk_label_set_label(GTK_LABEL(lbl1[row]), "      0 1 2 3 4 5 6 7 8 9");
      gtk_widget_show(lbl1[row++]);

      for(int i=0; i<=10; i++) {
        if(i == 10) {
          row++;
          gtk_label_set_label(GTK_LABEL(lbl1[row]), "      X Y Z T A B C D L I");
          gtk_widget_show(lbl1[row++]);
        }

        sprintf(string, "%4d ", 10*i);

        for(int col=0; col<10; col++) {
          strcat(string, getFlag(10*i+col) ? " 1" : " .");
        }

        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }

      gtk_label_set_label(GTK_LABEL(lbl1[row]), "      J K");
      gtk_widget_show(lbl1[row++]);

      sprintf(string, " 110 ");
      strcat(string, getFlag(110) ? " 1" : " .");
      strcat(string, getFlag(111) ? " 1" : " .");
      gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
      gtk_widget_show(lbl1[row++]);

      row++;
      if(currentLocalRegisters != NULL) {
        gtk_label_set_label(GTK_LABEL(lbl1[row]), "Local flags:");
        gtk_widget_show(lbl1[row++]);

        gtk_label_set_label(GTK_LABEL(lbl1[row]), "      0 1 2 3 4 5 6 7 8 9");
        gtk_widget_show(lbl1[row++]);

        sprintf(string, "   0 ");
        for(int col=0; col<10; col++) {
          strcat(string, getFlag(NUMBER_OF_GLOBAL_FLAGS + col) ? " 1" : " .");
        }
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);

        sprintf(string, "  10 ");
        for(int col=10; col<16; col++) {
         strcat(string, getFlag(NUMBER_OF_GLOBAL_FLAGS + col) ? " 1" : " .");
        }
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row++]);
      }
    }

    else if(debugWindow == DBG_REGISTERS) {
      for(int i=0; i<DEBUG_LINES; i++) {
        gtk_widget_hide(lbl1[i]);
        gtk_widget_hide(lbl2[i]);
      }

      row = 0;
      gtk_label_set_label(GTK_LABEL(lbl1[row]), "Regis Type                  Address    Size");
      gtk_label_set_label(GTK_LABEL(lbl2[row]), "Content");
      gtk_widget_show(lbl1[row]);
      gtk_widget_show(lbl2[row++]);

      for(int i=REGISTER_K; i>=REGISTER_I; i--) {
        sprintf(string, "%3d %c %s %7d %7d", i, i-REGISTER_I+'I', getRegisterDataTypeName(i, false, true), TO_WP43MEMPTR(getRegisterDataPointer(i)), TO_BYTES(getRegisterFullSize(i)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row]);
        debugRegisterValue(i, row++);
      }

      for(int i=REGISTER_D; i>=REGISTER_A; i--) {
        sprintf(string, "%3d %c %s %7d %7d", i, i-REGISTER_A+'A', getRegisterDataTypeName(i, false, true), TO_WP43MEMPTR(getRegisterDataPointer(i)), TO_BYTES(getRegisterFullSize(i)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row]);
        debugRegisterValue(i, row++);
      }

      sprintf(string, "103 T %s %7d %7d", getRegisterDataTypeName(REGISTER_T, false, true), TO_WP43MEMPTR(getRegisterDataPointer(REGISTER_T)), TO_BYTES(getRegisterFullSize(REGISTER_T)));
      gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
      gtk_widget_show(lbl1[row]);
      debugRegisterValue(REGISTER_T, row++);

      for(int i=REGISTER_Z; i>=REGISTER_X; i--) {
        sprintf(string, "%3d %c %s %7d %7d", i, i-REGISTER_X+'X', getRegisterDataTypeName(i, false, true), TO_WP43MEMPTR(getRegisterDataPointer(i)), TO_BYTES(getRegisterFullSize(i)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row]);
        debugRegisterValue(i, row++);
      }

      sprintf(string, "108 L %s %7d %7d", getRegisterDataTypeName(REGISTER_L, false, true), TO_WP43MEMPTR(getRegisterDataPointer(REGISTER_L)), TO_BYTES(getRegisterFullSize(REGISTER_L)));
      gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
      gtk_widget_show(lbl1[row]);
      debugRegisterValue(REGISTER_L, row++);

      row++;
      for(int i=0; i<100; i++) {
        if(row < DEBUG_LINES) {
          sprintf(string, "  %02d  %s %7d %7d", i, getRegisterDataTypeName(i, false, true), TO_WP43MEMPTR(getRegisterDataPointer(i)), TO_BYTES(getRegisterFullSize(i)));
          gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
          gtk_widget_show(lbl1[row]);
          debugRegisterValue(i, row);
          row++;
        }
      }
    }

    else if(debugWindow == DBG_LOCAL_REGISTERS) {
      for(int i=0; i<DEBUG_LINES; i++) {
        gtk_widget_hide(lbl1[i]);
        gtk_widget_hide(lbl2[i]);
      }

      row = 0;
      gtk_label_set_label(GTK_LABEL(lbl1[row]), "Regis Type                  Address    Size");
      sprintf(string, "Content of the %" PRIu8 " local registers", currentNumberOfLocalRegisters);
      gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
      gtk_widget_show(lbl1[row]);
      gtk_widget_show(lbl2[row++]);

      for(uint16_t i=FIRST_LOCAL_REGISTER; i<FIRST_LOCAL_REGISTER + currentNumberOfLocalRegisters; i++) {
        if(row < DEBUG_LINES) {
          sprintf(string, ".%02d   %s %7d %7d", i-FIRST_LOCAL_REGISTER, getRegisterDataTypeName(i, false, true), TO_WP43MEMPTR(getRegisterDataPointer(i)), TO_BYTES(getRegisterFullSize(i)));
          gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
          gtk_widget_show(lbl1[row]);
          debugRegisterValue(i, row++);
        }
      }
    }

    else if(debugWindow == DBG_STATISTICAL_SUMS) {
      for(int i=0; i<DEBUG_LINES; i++) {
        gtk_widget_hide(lbl1[i]);
        gtk_widget_hide(lbl2[i]);
      }

      row = 0;
      gtk_label_set_label(GTK_LABEL(lbl1[row]), "Sum");
      if(statisticalSumsPointer == NULL) {
        strcpy(string, "Content of the statistical sums (NULL)");
      }
      else {
        sprintf(string, "Content of the statistical sums (%d)", TO_WP43MEMPTR(statisticalSumsPointer));
      }
      gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
      gtk_widget_show(lbl1[row]);
      gtk_widget_show(lbl2[row++]);

      if(statisticalSumsPointer != NULL) {
        gtk_label_set_label(GTK_LABEL(lbl1[row]), "n");
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_N);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(x)");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_X);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(y)");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_Y);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(x" STD_SUP_2 ")");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_X2);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(x" STD_SUP_2 "y)");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_X2Y);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(y" STD_SUP_2 ")");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_Y2);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(xy)");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_XY);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(ln(x) ln(y))");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_lnXlnY);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(ln(x))");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_lnX);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(ln" STD_SUP_2 "(x))");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_ln2X);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(y ln(x))");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_YlnX);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(ln(y))");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_lnY);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(ln" STD_SUP_2 "(y))");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_ln2Y);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(x ln(y))");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_XlnY);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(x" STD_SUP_2 "ln(y))");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_X2lnY);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(ln(y)/x)");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_lnYonX);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(x" STD_SUP_2 "/y)");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_X2onY);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(1/x)");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_1onX);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(1/x" STD_SUP_2 ")");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_1onX2);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(x/y)");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_XonY);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(1/y)");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_1onY);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(1/y" STD_SUP_2 ")");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_1onY2);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(x" STD_SUP_3 ")");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_X3);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, STD_SIGMA "(x" STD_SUP_4 ")");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_X4);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, "Xmin");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_XMIN);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, "Xmax");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_XMAX);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, "Ymin");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_YMIN);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, "Ymax");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, SIGMA_YMAX);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        row++;
        sprintf(string, "STATS %s %7d %7d", getRegisterDataTypeName(findNamedVariable(statMx), false, true), TO_WP43MEMPTR(getRegisterDataPointer(findNamedVariable(statMx))), TO_BYTES(getRegisterFullSize(findNamedVariable(statMx))));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row]);
        debugRegisterValue(findNamedVariable(statMx), row++);

        sprintf(string, "SAVED STATS %s %7d %7d", getRegisterDataTypeName(TEMP_REGISTER_2_SAVED_STATS, false, true), TO_WP43MEMPTR(getRegisterDataPointer(TEMP_REGISTER_2_SAVED_STATS)), TO_BYTES(getRegisterFullSize(TEMP_REGISTER_2_SAVED_STATS)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row]);
        debugRegisterValue(TEMP_REGISTER_2_SAVED_STATS, row++);

        sprintf(string, "SAVED LastX");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, &SAVED_SIGMA_LASTX);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, "SAVED LastY");
        stringToUtf8(string, (uint8_t *)(string + 50));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string + 50);
        gtk_widget_show(lbl1[row]);
        formatRealDebug(string, &SAVED_SIGMA_LASTY);
        gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
        gtk_widget_show(lbl2[row++]);

        sprintf(string, "SAVED Last Action %65i", SAVED_SIGMA_LAct);
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row]);
      }
    }

    else if(debugWindow == DBG_NAMED_VARIABLES) {
      for(int i=0; i<DEBUG_LINES; i++) {
        gtk_widget_hide(lbl1[i]);
        gtk_widget_hide(lbl2[i]);
      }

      row = 0;
      gtk_label_set_label(GTK_LABEL(lbl1[row]), "Regis Type                  Address    Size");
      sprintf(string, "Content of the %" PRIu16 " named variables", numberOfNamedVariables);
      gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
      gtk_widget_show(lbl1[row]);
      gtk_widget_show(lbl2[row++]);

      for(uint16_t i=1000; i<1000 + numberOfNamedVariables; i++) {
        if(row < DEBUG_LINES) {
          sprintf(string, "%03d   %s %7d %7d", i-FIRST_NAMED_VARIABLE, getRegisterDataTypeName(i, false, true), TO_WP43MEMPTR(getRegisterDataPointer(i)), TO_BYTES(getRegisterFullSize(i)));
          gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
          gtk_widget_show(lbl1[row]);
          debugRegisterValue(i, row++);
        }
      }
    }

    else if(debugWindow == DBG_TMP_SAVED_STACK_REGISTERS) {
      for(int i=0; i<DEBUG_LINES; i++) {
        gtk_widget_hide(lbl1[i]);
        gtk_widget_hide(lbl2[i]);
      }

      row = 0;
      gtk_label_set_label(GTK_LABEL(lbl1[row]), "Regis Type                  Address    Size");
      sprintf(string, "Content of the 8 saved stack registers");
      gtk_label_set_label(GTK_LABEL(lbl2[row]), string);
      gtk_widget_show(lbl1[row]);
      gtk_widget_show(lbl2[row++]);

      for(uint16_t i=SAVED_REGISTER_X; i<=SAVED_REGISTER_L; i++) {
        sprintf(string, "%3d   %s %7d %7d", i - SAVED_REGISTER_X, getRegisterDataTypeName(i, false, true), TO_WP43MEMPTR(getRegisterDataPointer(i)), TO_BYTES(getRegisterFullSize(i)));
        gtk_label_set_label(GTK_LABEL(lbl1[row]), string);
        gtk_widget_show(lbl1[row]);
        debugRegisterValue(i, row++);
      }
    }
  }

  void btnBitFieldsClicked(GtkWidget* w, gpointer data) {
    debugWindow = DBG_BIT_FIELDS;
    refreshDebugPanel();
  }

  void btnFlagsClicked(GtkWidget* w, gpointer data) {
    debugWindow = DBG_FLAGS;
    //gtk_label_set_label(GTK_LABEL(lbl1[0]), "Flags:");
    //gtk_widget_show(frmCalc);
    refreshDebugPanel();
  }

  void btnRegistersClicked(GtkWidget* w, gpointer data) {
    debugWindow = DBG_REGISTERS;
    //gtk_label_set_label(GTK_LABEL(lbl1[0]), "Regis Addres   Type  Size Content");
    refreshDebugPanel();
  }

  void btnLocalRegistersClicked(GtkWidget* w, gpointer data) {
    debugWindow = DBG_LOCAL_REGISTERS;
    //gtk_label_set_label(GTK_LABEL(lbl1[0]), "Regis Addres   Type  Size Content");
    refreshDebugPanel();
  }

  void btnStatisticalSumsClicked(GtkWidget* w, gpointer data) {
    debugWindow = DBG_STATISTICAL_SUMS;
    //gtk_label_set_label(GTK_LABEL(lbl1[0]), "Regis Addres   Type  Size Content");
    refreshDebugPanel();
  }

  void btnNamedVariablesClicked(GtkWidget* w, gpointer data) {
    debugWindow = DBG_NAMED_VARIABLES;
    //gtk_label_set_label(GTK_LABEL(lbl1[0]), "Regis Addres   Type  Size Content");
    refreshDebugPanel();
  }

  void btnSavedStackRegistersClicked(GtkWidget* w, gpointer data) {
    debugWindow = DBG_TMP_SAVED_STACK_REGISTERS;
    //gtk_label_set_label(GTK_LABEL(lbl1[0]), "Regis Addres   Type  Size Content");
    refreshDebugPanel();
  }

  void chkHexaStringClicked(GtkWidget* w, gpointer data) {
    refreshDebugPanel();
  }
#endif // (DEBUG_PANEL == 1)



#if(DEBUG_PANEL == 1) || (DEBUG_REGISTER_L == 1)
  /********************************************//**
   * \brief Formats a real34 for the debug window
   *
   * \param[in] str char*  String receiving the value
   * \param[in] real34 real34_t* Value
   * \return void
   ***********************************************/
  void formatReal34Debug(char *str, real34_t *real34) {
    uint8_t ch, coef, digit;
    uint8_t bcd[DECQUAD_Pmax];
    int32_t sign, exponent;

    if(real34IsInfinite(real34) || real34IsNaN(real34)) {
      real34ToString(real34, str);
      return;
    }

    if(real34IsZero(real34)) {
      if(real34IsPositive(real34)) {
        strcpy(str, "+0.000000000000000000000000000000000e+0");
      }
      else {
        strcpy(str, "-0.000000000000000000000000000000000e+0");
      }
      return;
    }

    sign = real34GetCoefficient(real34, bcd);
    exponent = real34GetExponent(real34);
    if(sign) {
      str[0] = '-';
    }
    else {
      str[0] = '+';
    }

    coef = 0;
    while(coef<34 && bcd[coef] == 0) {
      coef++;
    }

    str[1] = '0' + bcd[coef++];
    digit = 1;
    str[2] = '.';

    ch = 3;
    while(coef<34) {
      str[ch++] = '0' + bcd[coef++];
      digit++;
      exponent++;
    }

    while(digit<34) {
      str[ch++] = '0';
      digit++;
    }

    sprintf(str+ch, "e%+d", exponent);
  }



  /********************************************//**
   * \brief Formats a real for the debug window
   *
   * \param[in] str char*  String receiving the value
   * \param[in] real real_t* Value
   * \return void
   ***********************************************/
  void formatRealDebug(char *str, real_t *real) {
    if(realIsZero(real)) {
      xcopy(str, "0.", 3);
    }
    else {
      realToString(real, str);
    }
  }



  /********************************************//**
   * \brief Formats a complex34 for the debug window
   *
   * \param[in] str char*  String receiving the value
   * \param[in] x real34_t* Value
   * \return void
   ***********************************************/
  void formatComplex34Debug(char *str, void *addr) {
    formatReal34Debug(str     , addr             );
    formatReal34Debug(str + 64, (real34_t *)((dataBlock_t *)addr + REAL34_SIZE));

    strcat(str, " ");
    xcopy(strchr(str, '\0'), str + 64, strlen(str + 64) + 1);
    strcat(str, "i");
  }
#endif // DEBUG_PANEL == 1 || DEBUG_REGISTER_L == 1

#if defined(PC_BUILD)
  void dumpScreenToConsole(void) {
    int x, y;

    for(y = 0; y < SCREEN_HEIGHT; y++) {
      for(x = 0; x < SCREEN_WIDTH; x++) {
        if(*(screenData + y*screenStride + x) == ON_PIXEL) {
          printf("#");
        }
        else {
          printf(" ");
        }
      }
      printf("\n");
    }
  }
#endif // PC_BUILD



#if defined(PC_BUILD ) || defined(TESTSUITE_BUILD)
  void testRegisters(const char *text) {
    calcRegister_t i;
    bool_t situationIsBad;

    situationIsBad = false;
    for(i=0; i<FIRST_LOCAL_REGISTER; i++) {
      if(TO_WP43MEMPTR(getRegisterDataPointer(i)) >= RAM_SIZE) {
        situationIsBad = true;
        break;
      }

      if(getRegisterDataType(i) == dtString || getRegisterDataType(i) == dtLongInteger) {
        if(getRegisterMaxDataLength(i) >= RAM_SIZE) {
          situationIsBad = true;
          break;
        }
      }
    }

    if(situationIsBad) {
      printf("\nsituation is bad %s\n", text);
      for(i=0; i<FIRST_LOCAL_REGISTER; i++) {
        if(TO_WP43MEMPTR(getRegisterDataPointer(i)) >= RAM_SIZE) {
          printf("register %d    ptr=%d\n", i, TO_WP43MEMPTR(getRegisterDataPointer(i)));
        }

        if(getRegisterDataType(i) == dtString || getRegisterDataType(i) == dtLongInteger) {
          if(getRegisterMaxDataLength(i) >= RAM_SIZE) {
            printf("register %d    ptr=%d     dataLen=%d (%d %d)\n", i, TO_WP43MEMPTR(getRegisterDataPointer(i)), TO_BYTES(getRegisterMaxDataLength(i)), TO_BYTES(getRegisterMaxDataLength(i)), TO_BYTES(getRegisterMaxDataLength(i)+1));
          }
        }
      }
      exit(0);
    }
  }



  void memoryDump2(const char *text) {
    int32_t i;
    uint32_t dataType;
    calcRegister_t regist;

    //if(debug) {
    //  debugCounter++;
      printf("\n\n%s\nTotal memory = %d bytes = %d blocks\n", text, TO_BYTES(RAM_SIZE), RAM_SIZE);
      printf("Free blocks (%" PRId32 "):\n", numberOfFreeMemoryRegions);

      for(i=0; i<numberOfFreeMemoryRegions; i++) {
        printf("  %2" PRId32 " starting at %5" PRIu16 ": %5" PRIu16 " blocks = %6" PRIu32 " bytes\n", i, freeMemoryRegions[i].address, freeMemoryRegions[i].sizeInBlocks, TO_BYTES((uint32_t)freeMemoryRegions[i].sizeInBlocks));
      }

      printf("Reg  Num DescrAddr DataType                    DataInfo    DataPtr FullDataLen Content\n");

      regist = REGISTER_X;
      dataType = globalRegister[regist].dataType;
      printf(" X  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), globalRegister[regist].tag, getRegisterTagName(regist, true), globalRegister[regist].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");

      regist = REGISTER_Y;
      dataType = globalRegister[regist].dataType;
      printf(" Y  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), globalRegister[regist].tag, getRegisterTagName(regist, true), globalRegister[regist].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");

      regist = REGISTER_Z;
      dataType = globalRegister[regist].dataType;
      printf(" Z  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), globalRegister[regist].tag, getRegisterTagName(regist, true), globalRegister[regist].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");

      regist = REGISTER_T;
      dataType = globalRegister[regist].dataType;
      printf(" T  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), globalRegister[regist].tag, getRegisterTagName(regist, true), globalRegister[regist].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");

      printf("----------------------------------------------------------------------------------------------------------------------------------------\n");

      regist = REGISTER_A;
      dataType = globalRegister[regist].dataType;
      printf(" A  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), globalRegister[regist].tag, getRegisterTagName(regist, true), globalRegister[regist].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");

      regist = REGISTER_B;
      dataType = globalRegister[regist].dataType;
      printf(" B  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), globalRegister[regist].tag, getRegisterTagName(regist, true), globalRegister[regist].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");

      regist = REGISTER_C;
      dataType = globalRegister[regist].dataType;
      printf(" C  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), globalRegister[regist].tag, getRegisterTagName(regist, true), globalRegister[regist].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");

      regist = REGISTER_D;
      dataType = globalRegister[regist].dataType;
      printf(" D  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), globalRegister[regist].tag, getRegisterTagName(regist, true), globalRegister[regist].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");

      printf("----------------------------------------------------------------------------------------------------------------------------------------\n");

      regist = REGISTER_L;
      dataType = globalRegister[regist].dataType;
      printf(" L  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), globalRegister[regist].tag, getRegisterTagName(regist, true), globalRegister[regist].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");

      regist = REGISTER_I;
      dataType = globalRegister[regist].dataType;
      printf(" I  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), globalRegister[regist].tag, getRegisterTagName(regist, true), globalRegister[regist].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");

      regist = REGISTER_J;
      dataType = globalRegister[regist].dataType;
      printf(" J  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), globalRegister[regist].tag, getRegisterTagName(regist, true), globalRegister[regist].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");

      regist = REGISTER_K;
      dataType = globalRegister[regist].dataType;
      printf(" K  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), globalRegister[regist].tag, getRegisterTagName(regist, true), globalRegister[regist].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");

      printf("----------------------------------------------------------------------------------------------------------------------------------------\n");

      regist = SAVED_REGISTER_X;
      dataType = savedStackRegister[regist - SAVED_REGISTER_X].dataType;
      printf("SX  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), savedStackRegister[regist - SAVED_REGISTER_X].tag, getRegisterTagName(regist, true), savedStackRegister[regist - SAVED_REGISTER_X].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");

      regist = SAVED_REGISTER_Y;
      dataType = savedStackRegister[regist - SAVED_REGISTER_X].dataType;
      printf("SY  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), savedStackRegister[regist - SAVED_REGISTER_X].tag, getRegisterTagName(regist, true), savedStackRegister[regist - SAVED_REGISTER_X].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");

      regist = SAVED_REGISTER_Z;
      dataType = savedStackRegister[regist - SAVED_REGISTER_X].dataType;
      printf("SZ  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), savedStackRegister[regist - SAVED_REGISTER_X].tag, getRegisterTagName(regist, true), savedStackRegister[regist - SAVED_REGISTER_X].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");

      regist = SAVED_REGISTER_T;
      dataType = savedStackRegister[regist - SAVED_REGISTER_X].dataType;
      printf("ST  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), savedStackRegister[regist - SAVED_REGISTER_X].tag, getRegisterTagName(regist, true), savedStackRegister[regist - SAVED_REGISTER_X].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");

      printf("----------------------------------------------------------------------------------------------------------------------------------------\n");

      regist = SAVED_REGISTER_A;
      dataType = savedStackRegister[regist - SAVED_REGISTER_X].dataType;
      printf("SA  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), savedStackRegister[regist - SAVED_REGISTER_X].tag, getRegisterTagName(regist, true), savedStackRegister[regist - SAVED_REGISTER_X].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");

      regist = SAVED_REGISTER_B;
      dataType = savedStackRegister[regist - SAVED_REGISTER_X].dataType;
      printf("SB  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), savedStackRegister[regist - SAVED_REGISTER_X].tag, getRegisterTagName(regist, true), savedStackRegister[regist - SAVED_REGISTER_X].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");

      regist = SAVED_REGISTER_C;
      dataType = savedStackRegister[regist - SAVED_REGISTER_X].dataType;
      printf("SC  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), savedStackRegister[regist - SAVED_REGISTER_X].tag, getRegisterTagName(regist, true), savedStackRegister[regist - SAVED_REGISTER_X].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");

      regist = SAVED_REGISTER_D;
      dataType = savedStackRegister[regist - SAVED_REGISTER_X].dataType;
      printf("SD  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), savedStackRegister[regist - SAVED_REGISTER_X].tag, getRegisterTagName(regist, true), savedStackRegister[regist - SAVED_REGISTER_X].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");

      regist = SAVED_REGISTER_L;
      dataType = savedStackRegister[regist - SAVED_REGISTER_X].dataType;
      printf("SL  %4d           %2u=%s %5u=%s %5u   %5d       ", regist, dataType, getDataTypeName(dataType, false, true), savedStackRegister[regist - SAVED_REGISTER_X].tag, getRegisterTagName(regist, true), savedStackRegister[regist - SAVED_REGISTER_X].pointerToRegisterData, TO_BYTES(getRegisterFullSize(regist)));
      printRegisterToConsole(regist, "", "\n");
  //  }
  }

  ///////////////////////////////
  // Stack smashing detection
  void stackCheck(const unsigned char *begin, const unsigned char *end, int size, const char *where) {
     int i, corrupted = 0;

     for(i=0; i<size; i++) {
       if(*(begin + i) != 0xaa) {
         printf("Stack begin corrupted: begin[%d]=0x%02x at %s\n", i, begin[i], where);
         corrupted = 1;
       }
     }

     for(i=0; i<size; i++) {
       if(*(end + i) != 0xaa) {
         printf("Stack end corrupted: end[%d]=0x%02x at %s\n", i, end[i], where);
         corrupted = 1;
       }
     }

     if(corrupted) {
       exit(0xBAD);
     }
  }


  void initStackCheck(unsigned char *begin, unsigned char *end, int size) {
    memset(begin, 0xaa, size);
    memset(end,   0xaa, size);
  }

  //////////////////////////////////////////////////
  // Example of stack smashing tests in a function
  void stackSmashingTest(void) {
                                                unsigned char stackEnd[10000]; // First declaration
    int v1, v2, v3;
                                                unsigned char stackBegin[10000]; // Last declaration

                                                initStackCheck(stackBegin, stackEnd, 10000);

    v1 = 1;
                                                stackCheck(stackBegin, stackEnd, 10000, "after v1 = ...");
    v2 = v1 + 1;
                                                stackCheck(stackBegin, stackEnd, 10000, "after v2 = ...");
    v3 = v2 + 2;
                                                stackCheck(stackBegin, stackEnd, 10000, "after v3 = ...");
    printf("v1=%d v2=%d v3=%d\n", v1, v2, v3);
                                                stackCheck(stackBegin, stackEnd, 10000, "after printf(...");
  }
#endif // PC_BUILD || TESTSUITE_BUILD
