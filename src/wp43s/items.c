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
 * \file items.c Item list and function to run them
 ***********************************************/

#include "wp43s.h"

/********************************************//**
 * \brief Dummy function for an item to be coded
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void itemToBeCoded(uint16_t unusedParamButMandatory) {
  funcOK = false;
}



/********************************************//**
 * \brief Dummy function for a function (part of an item) to be coded
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnToBeCoded(void) {
  displayCalcErrorMessage(ERROR_FUNCTION_TO_BE_CODED, ERR_REGISTER_LINE, REGISTER_X);
  #ifdef PC_BUILD
    showInfoDialog("Function to be coded", "for that data type(s)!", NULL, NULL);
  #endif
}



/********************************************//**
 * \brief No OPeration
 *
 * \param  unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnNop(uint16_t unusedParamButMandatory) {
}



#ifndef TESTSUITE_BUILD
/********************************************//**
 * \brief Runs a function
 *
 * \param[in] fn int16_t Index in the indexOfItems area of the function to run
 * \return void
 ***********************************************/
void runFunction(int16_t func) {
  funcOK = true;

  if(func > LAST_ITEM) {
  }

  tamMode = indexOfItems[func].param;
  if(calcMode != CM_TAM && TM_VALUE <= tamMode && tamMode <= TM_CMP) {
    tamFunction = func;
    strcpy(tamBuffer, indexOfItems[func].itemPrinted);
    tamNumberMin = 0;
    tamNumberMax = 99;

    if(func == ITM_FIX || func == ITM_SCI || func == ITM_ENG || func == ITM_ALL || func == ITM_DSP || func == ITM_GAP || func == ITM_SIGFIG || func == ITM_UNIT) {      //JM SIGFIG. Added ITM_SIGFIG to max 15. Added Item ITM_UNIT
      tamNumberMax = 15;
    }
    else if(func == ITM_toINT) {
      tamNumberMin = 2;
      tamNumberMax = 16;
    }
    else if(func == ITM_DSTACK) {
      tamNumberMin = 1;
      tamNumberMax = 4;
    }
    else if(func == ITM_WSIZE) {
      tamNumberMax = 64;
    }

    calcModeTAM();
    return;
  }

  indexOfItems[func].func(indexOfItems[func].param);

  if(indexOfItems[func].stackLiftStatus == SLS_DISABLED) {
    STACK_LIFT_DISABLE;
  }
  else if(indexOfItems[func].stackLiftStatus == SLS_ENABLED) {
    STACK_LIFT_ENABLE;
  }

  if(!funcOK) {
    displayCalcErrorMessage(ERROR_ITEM_TO_BE_CODED, ERR_REGISTER_LINE, REGISTER_X);
    #ifdef PC_BUILD
      sprintf(errorMessage, "%" FMT16S " = %s", func, indexOfItems[func].itemPrinted);
      showInfoDialog("In function runFunction:", "Item not implemented", errorMessage, "to be coded");
    #endif
  }
}
#endif



const item_t indexOfItems[] = {

//            function                     parameter                    item name                                      item retésentation in menu                                                    stackLift
/*    0 */  { itemToBeCoded,               NOPARAM,                     "",                                            "0000",                                                                       SLS_UNCHANGED},
/*    1 */  { fnCvtCToF,                   NOPARAM,                     STD_DEGREE "C" STD_RIGHT_ARROW STD_DEGREE "F", STD_DEGREE "C" STD_RIGHT_ARROW STD_DEGREE "F",                                SLS_ENABLED  },
/*    2 */  { fnCvtFToC,                   NOPARAM,                     STD_DEGREE "F" STD_RIGHT_ARROW STD_DEGREE "C", STD_DEGREE "F" STD_RIGHT_ARROW STD_DEGREE "C",                                SLS_ENABLED  },
/*    3 */  { fn10Pow,                     NOPARAM,                     "10" STD_SUP_x,                                "10" STD_SUP_x,                                                               SLS_ENABLED  },
/*    4 */  { fnIntegerMode,               SIM_1COMPL,                  "1COMPL",                                      "1COMPL",                                                                     SLS_UNCHANGED},
/*    5 */  { fnConstant,                  0,                           STD_ONE_HALF,                                  STD_ONE_HALF,                                                                 SLS_ENABLED  },
/*    6 */  { fnInvert,                    NOPARAM,                     "1/x",                                         "1/x",                                                                        SLS_ENABLED  },
/*    7 */  { fnIntegerMode,               SIM_2COMPL,                  "2COMPL",                                      "2COMPL",                                                                     SLS_UNCHANGED},
/*    8 */  { fn2Pow,                      NOPARAM,                     "2" STD_SUP_x,                                 "2" STD_SUP_x,                                                                SLS_ENABLED  },
/*    9 */  { fnCubeRoot,                  NOPARAM,                     STD_CUBE_ROOT STD_x_UNDER_ROOT,                STD_CUBE_ROOT STD_x_UNDER_ROOT,                                               SLS_ENABLED  },
/*   10 */  { itemToBeCoded,               NOPARAM,                     "A",                                           "A",                                                                          SLS_UNCHANGED},
/*   11 */  { fnConstant,                  1,                           "a",                                           "a",                                                                          SLS_ENABLED  },
/*   12 */  { fnConstant,                  2,                           "a" STD_SUB_0,                                 "a" STD_SUB_0,                                                                SLS_ENABLED  },
/*   13 */  { itemToBeCoded,               NOPARAM,                     "ABS",                                         "ABS",                                                                        SLS_UNCHANGED},
/*   14 */  { itemToBeCoded,               NOPARAM,                     "ACC",                                         "ACC",                                                                        SLS_UNCHANGED},
/*   15 */  { fnCvtAcreM2,                 multiply,                    "ac" STD_RIGHT_ARROW "m" STD_SUP_2,            "acre",                                                                       SLS_ENABLED  },
/*   16 */  { fnCvtAcreusM2,               multiply,                    "ac" STD_US STD_RIGHT_ARROW "m" STD_SUP_2,     "acre" STD_US,                                                                SLS_ENABLED  },
/*   17 */  { itemToBeCoded,               NOPARAM,                     "ADV",                                         "ADV",                                                                        SLS_UNCHANGED},
/*   18 */  { itemToBeCoded,               NOPARAM,                     "AGM",                                         "AGM",                                                                        SLS_UNCHANGED},
/*   19 */  { itemToBeCoded,               NOPARAM,                     "AGRAPH",                                      "AGRAPH",                                                                     SLS_UNCHANGED},
/*   20 */  { fnDisplayFormatAll,          TM_VALUE,                    "ALL" ,                                        "ALL",                                                                        SLS_UNCHANGED},
/*   21 */  { fnConstant,                  3,                           "a" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "a" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,                                  SLS_ENABLED  },
/*   22 */  { itemToBeCoded,               NOPARAM,                     "AND",                                         "AND",                                                                        SLS_UNCHANGED},
/*   23 */  { itemToBeCoded,               NOPARAM,                     "ANGLES",                                      "ANGLES",                                                                     SLS_UNCHANGED},
/*   24 */  { fnArccos,                    NOPARAM,                     "arccos",                                      "ACOS",                                                                     SLS_ENABLED  },          //JM3
/*   25 */  { fnArccosh,                   NOPARAM,                     "arcosh",                                      "arcosh",                                                                     SLS_ENABLED  },
/*   26 */  { fnArcsin,                    NOPARAM,                     "arcsin",                                      "ASIN",                                                                     SLS_ENABLED  },          //JM3
/*   27 */  { fnArctan,                    NOPARAM,                     "arctan",                                      "ATAN",                                                                     SLS_ENABLED  },          //JM3
/*   28 */  { fnArcsinh,                   NOPARAM,                     "arsinh",                                      "arsinh",                                                                     SLS_ENABLED  },
/*   29 */  { fnArctanh,                   NOPARAM,                     "artanh",                                      "artanh",                                                                     SLS_ENABLED  },
/*   30 */  { itemToBeCoded,               NOPARAM,                     "ASR",                                         "ASR",                                                                        SLS_UNCHANGED},
/*   31 */  { itemToBeCoded,               NOPARAM,                     "ASSIGN",                                      "ASN",                                                                        SLS_UNCHANGED},
/*   32 */  { fnCvtAtmPa,                  multiply,                    "atm" STD_RIGHT_ARROW "Pa",                    "atm" STD_RIGHT_ARROW "Pa",                                                   SLS_ENABLED  },
/*   33 */  { fnCvtAuM,                    multiply,                    "au" STD_RIGHT_ARROW "m",                      "au" STD_RIGHT_ARROW "m",                                                     SLS_ENABLED  },
/*   34 */  { itemToBeCoded,               NOPARAM,                     "A" STD_ELLIPSIS "Z",                          "A" STD_ELLIPSIS "Z",                                                         SLS_UNCHANGED},
/*   35 */  { itemToBeCoded,               NOPARAM,                     "A:",                                          "A:",                                                                         SLS_UNCHANGED},
/*   36 */  { fnConstant,                  4,                           "a" STD_SUB_EARTH,                             "a" STD_SUB_EARTH,                                                            SLS_ENABLED  },
/*   37 */  { itemToBeCoded,               NOPARAM,                     "B",                                           "B",                                                                          SLS_UNCHANGED},
/*   38 */  { itemToBeCoded,               NOPARAM,                     "BACK",                                        "BACK",                                                                       SLS_UNCHANGED},
/*   39 */  { fnCvtBarPa,                  multiply,                    "bar" STD_RIGHT_ARROW "Pa",                    "bar" STD_RIGHT_ARROW "Pa",                                                   SLS_ENABLED  },
/*   40 */  { fnBatteryVoltage,            NOPARAM,                     "BATT?",                                       "BATT?",                                                                      SLS_ENABLED  },
/*   41 */  { itemToBeCoded,               NOPARAM,                     "BC?",                                         "BC?",                                                                        SLS_UNCHANGED},
/*   42 */  { itemToBeCoded,               NOPARAM,                     "BEEP",                                        "BEEP",                                                                       SLS_UNCHANGED},
/*   43 */  { itemToBeCoded,               NOPARAM,                     "BeginP",                                      "Begin",                                                                      SLS_UNCHANGED},
/*   44 */  { fnCurveFitting,              CF_BEST_FITTING,             "BestF",                                       "BestF",                                                                      SLS_UNCHANGED},
/*   45 */  { itemToBeCoded,               NOPARAM,                     "Binom",                                       "Binom",                                                                      SLS_UNCHANGED},
/*   46 */  { itemToBeCoded,               NOPARAM,                     "Binom" STD_SUB_p,                             "Binom" STD_SUB_p,                                                            SLS_UNCHANGED},
/*   47 */  { itemToBeCoded,               NOPARAM,                     "Binom" STD_SUB_e,                             "Binom" STD_SUB_e,                                                            SLS_UNCHANGED},
/*   48 */  { itemToBeCoded,               NOPARAM,                     "Binom" STD_SUP_MINUS_1,                       "Binom" STD_SUP_MINUS_1,                                                      SLS_UNCHANGED},
/*   49 */  { itemToBeCoded,               NOPARAM,                     "Binom:",                                      "Binom:",                                                                     SLS_UNCHANGED},
/*   50 */  { itemToBeCoded,               NOPARAM,                     "BITS",                                        "BITS",                                                                       SLS_UNCHANGED},
/*   51 */  { itemToBeCoded,               NOPARAM,                     "B" STD_SUB_n,                                 "B" STD_SUB_n,                                                                SLS_UNCHANGED},
/*   52 */  { itemToBeCoded,               NOPARAM,                     "B" STD_SUB_n STD_SUP_ASTERISK,                "B" STD_SUB_n STD_SUP_ASTERISK,                                               SLS_UNCHANGED},
/*   53 */  { itemToBeCoded,               NOPARAM,                     "BS?",                                         "BS?",                                                                        SLS_UNCHANGED},
/*   54 */  { fnCvtBtuJ,                   multiply,                    "Btu" STD_RIGHT_ARROW "J",                     "Btu" STD_RIGHT_ARROW "J",                                                    SLS_ENABLED  },
/*   55 */  { itemToBeCoded,               NOPARAM,                     "C",                                           "C",                                                                          SLS_UNCHANGED},
/*   56 */  { fnConstant,                  5,                           "c",                                           "c",                                                                          SLS_ENABLED  },
/*   57 */  { fnConstant,                  6,                           "c" STD_SUB_1,                                 "c" STD_SUB_1,                                                                SLS_ENABLED  },
/*   58 */  { fnConstant,                  7,                           "c" STD_SUB_2,                                 "c" STD_SUB_2,                                                                SLS_ENABLED  },
/*   59 */  { fnCvtCalJ,                   multiply,                    "cal" STD_RIGHT_ARROW "J",                     "cal" STD_RIGHT_ARROW "J",                                                    SLS_ENABLED  },
/*   60 */  { itemToBeCoded,               NOPARAM,                     "CASE",                                        "CASE",                                                                       SLS_UNCHANGED},
/*   61 */  { itemToBeCoded,               NOPARAM,                     "CATALOG",                                     "CAT",                                                                        SLS_UNCHANGED},      //JM HOME and also DM42 shift does not allow full text
/*   62 */  { itemToBeCoded,               NOPARAM,                     "Cauch",                                       "Cauch",                                                                      SLS_UNCHANGED},
/*   63 */  { itemToBeCoded,               NOPARAM,                     "Cauch" STD_SUB_p,                             "Cauch" STD_SUB_p,                                                            SLS_UNCHANGED},
/*   64 */  { itemToBeCoded,               NOPARAM,                     "Cauch" STD_SUB_e,                             "Cauch" STD_SUB_e,                                                            SLS_UNCHANGED},
/*   65 */  { itemToBeCoded,               NOPARAM,                     "Cauch" STD_SUP_MINUS_1,                       "Cauch" STD_SUP_MINUS_1,                                                      SLS_UNCHANGED},
/*   66 */  { itemToBeCoded,               NOPARAM,                     "Cauch:",                                      "Cauch:",                                                                     SLS_UNCHANGED},
/*   67 */  { itemToBeCoded,               NOPARAM,                     "CB",                                          "CB",                                                                         SLS_UNCHANGED},
/*   68 */  { fnCeil,                      NOPARAM,                     "CEIL",                                        "CEIL",                                                                       SLS_ENABLED  },
/*   69 */  { fnClearFlag,                 TM_FLAG,                     "CF",                                          "CF",                                                                         SLS_UNCHANGED},
/*   70 */  { itemToBeCoded,               NOPARAM,                     "CHARS",                                       "CHARS",                                                                      SLS_UNCHANGED},
/*   71 */  { fnClAll,                     NOT_CONFIRMED,               "CLALL",                                       "CLall",                                                                      SLS_UNCHANGED},
/*   72 */  { itemToBeCoded,               NOPARAM,                     "CLCVAR",                                      "CLCVAR",                                                                     SLS_UNCHANGED},
/*   73 */  { fnClFAll,                    NOPARAM,                     "CLFALL",                                      "CLFall",                                                                     SLS_UNCHANGED},
/*   74 */  { itemToBeCoded,               NOPARAM,                     "CLK",                                         "CLK",                                                                        SLS_UNCHANGED},
/*   75 */  { fnTimeFormat,                TF_H12,                      "CLK12",                                       "CLK12",                                                                      SLS_UNCHANGED},
/*   76 */  { fnTimeFormat,                TF_H24,                      "CLK24",                                       "CLK24",                                                                      SLS_UNCHANGED},
/*   77 */  { itemToBeCoded,               NOPARAM,                     "CLLCD",                                       "CLLCD",                                                                      SLS_UNCHANGED},
/*   78 */  { itemToBeCoded,               NOPARAM,                     "CLMENU",                                      "CLMENU",                                                                     SLS_UNCHANGED},
/*   79 */  { itemToBeCoded,               NOPARAM,                     "CLP",                                         "CLP",                                                                        SLS_UNCHANGED},
/*   80 */  { fnClPAll,                    NOT_CONFIRMED,               "CLPALL",                                      "CLPall",                                                                     SLS_UNCHANGED},
/*   81 */  { itemToBeCoded,               NOPARAM,                     "CLR",                                         "CLR",                                                                        SLS_UNCHANGED},
/*   82 */  { fnClearRegisters,            NOPARAM,                     "CLREGS",                                      "CLREGS",                                                                     SLS_UNCHANGED},
/*   83 */  { fnClearStack,                NOPARAM,                     "CLSTK",                                       "CLSTK",                                                                      SLS_UNCHANGED},
/*   84 */  { fnClX,                       NOPARAM,                     "CLX",                                         "CLX",                                                                        SLS_DISABLED },
/*   85 */  { fnClSigma,                   NOPARAM,                     "CL" STD_SIGMA,                                "CL" STD_SIGMA,                                                               SLS_UNCHANGED},
/*   86 */  { itemToBeCoded,               NOPARAM,                     "CNST",                                        "CNST",                                                                       SLS_UNCHANGED},
/*   87 */  { itemToBeCoded,               NOPARAM,                     "COMB",                                        "C" STD_SUB_y STD_SUB_x,                                                      SLS_UNCHANGED},
/*   88 */  { fnConjugate,                 NOPARAM,                     "CONJ",                                        "conj",                                                                       SLS_ENABLED  },
/*   89 */  { itemToBeCoded,               NOPARAM,                     "CONST",                                       "CONST",                                                                      SLS_UNCHANGED},
/*   90 */  { itemToBeCoded,               NOPARAM,                     "CONVG?",                                      "CONVG?",                                                                     SLS_UNCHANGED},
/*   91 */  { itemToBeCoded,               NOPARAM,                     "CORR",                                        "r",                                                                          SLS_UNCHANGED},
/*   92 */  { fnCos,                       NOPARAM,                     "cos",                                         "COS",                                                                        SLS_ENABLED  },     //JM3
/*   93 */  { fnCosh,                      NOPARAM,                     "cosh",                                        "cosh",                                                                       SLS_ENABLED  },
/*   94 */  { itemToBeCoded,               NOPARAM,                     "COV",                                         "cov",                                                                        SLS_UNCHANGED},
/*   95 */  { itemToBeCoded,               NOPARAM,                     "CPX",                                         "CPX",                                                                        SLS_UNCHANGED},
/*   96 */  { fnComplexUnit,               CU_I,                        "CPXi",                                        "CPXi",                                                                       SLS_UNCHANGED},
/*   97 */  { fnComplexUnit,               CU_J,                        "CPXj",                                        "CPXj",                                                                       SLS_UNCHANGED},
/*   98 */  { fnComplexResult,             true,                        "CPXRES",                                      "CPXRES",                                                                     SLS_UNCHANGED},
/*   99 */  { itemToBeCoded,               NOPARAM,                     "CPXS",                                        "CPXS",                                                                       SLS_UNCHANGED},
/*  100 */  { itemToBeCoded,               NOPARAM,                     "CPX?",                                        "CPX?",                                                                       SLS_UNCHANGED},
/*  101 */  { itemToBeCoded,               NOPARAM,                     "CROSS",                                       "cross",                                                                      SLS_UNCHANGED},
/*  102 */  { fnCvtCwtKg,                  multiply,                    "cwt" STD_RIGHT_ARROW "kg",                    "cwt" STD_RIGHT_ARROW "kg",                                                   SLS_ENABLED  },
/*  103 */  { itemToBeCoded,               NOPARAM,                     "CX" STD_RIGHT_ARROW "RE",                     "CX" STD_RIGHT_ARROW "RE",                                                    SLS_UNCHANGED},
/*  104 */  { itemToBeCoded,               NOPARAM,                     "D",                                           "D",                                                                          SLS_UNCHANGED},
/*  105 */  { itemToBeCoded,               NOPARAM,                     "DATE",                                        "DATE",                                                                       SLS_UNCHANGED},
/*  106 */  { itemToBeCoded,               NOPARAM,                     "DATES",                                       "DATES",                                                                      SLS_UNCHANGED},
/*  107 */  { itemToBeCoded,               NOPARAM,                     "DATE" STD_RIGHT_ARROW,                        "DATE" STD_RIGHT_ARROW,                                                       SLS_UNCHANGED},
/*  108 */  { itemToBeCoded,               NOPARAM,                     "DAY",                                         "DAY",                                                                        SLS_UNCHANGED},
/*  109 */  { itemToBeCoded,               NOPARAM,                     "DBL?",                                        "DBL?",                                                                       SLS_UNCHANGED},
/*  110 */  { itemToBeCoded,               NOPARAM,                     "DBLR",                                        "DBLR",                                                                       SLS_UNCHANGED},
/*  111 */  { itemToBeCoded,               NOPARAM,                     "DBL" STD_CROSS,                               "DBL" STD_CROSS,                                                              SLS_UNCHANGED},
/*  112 */  { itemToBeCoded,               NOPARAM,                     "DBL/",                                        "DBL/",                                                                       SLS_UNCHANGED},
/*  113 */  { fnCvtDbRatio,                20,                          "dB" STD_RIGHT_ARROW "fr",                     "dB " STD_RIGHT_ARROW,                                                        SLS_ENABLED  },
/*  114 */  { fnCvtDbRatio,                10,                          "dB" STD_RIGHT_ARROW "pr",                     "dB " STD_RIGHT_ARROW,                                                        SLS_ENABLED  },
/*  115 */  { itemToBeCoded,               NOPARAM,                     "DEC",                                         "DEC",                                                                        SLS_UNCHANGED},
/*  116 */  { itemToBeCoded,               NOPARAM,                     "DECOMP",                                      "DECOMP",                                                                     SLS_UNCHANGED},
/*  117 */  { fnAngularMode,               AM_DEGREE,                   "DEG",                                         "DEG",                                                                        SLS_UNCHANGED},
/*  118 */  { fnCvtToCurrentAngularMode,   AM_DEGREE,                   "DEG" STD_RIGHT_ARROW,                         "DEG" STD_RIGHT_ARROW,                                                        SLS_ENABLED  },
/*  119 */  { fnDenMode,                   DM_ANY,                      "DENANY",                                      "DENANY",                                                                     SLS_UNCHANGED},
/*  120 */  { fnDenMode,                   DM_FAC,                      "DENFAC",                                      "DENFAC",                                                                     SLS_UNCHANGED},
/*  121 */  { fnDenMode,                   DM_FIX,                      "DENFIX",                                      "DENFIX",                                                                     SLS_UNCHANGED},
/*  122 */  { fnDenMax,                    NOPARAM,                     "DENMAX",                                      "DENMAX",                                                                     SLS_UNCHANGED},
/*  123 */  { itemToBeCoded,               NOPARAM,                     "DIGITS",                                      "DIGITS",                                                                     SLS_UNCHANGED},
/*  124 */  { itemToBeCoded,               NOPARAM,                     "DISP",                                        "DISP",                                                                       SLS_UNCHANGED},
/*  125 */  { itemToBeCoded,               NOPARAM,                     "DOT",                                         "dot",                                                                        SLS_UNCHANGED},
/*  126 */  { fnConvertXToReal34,          NOPARAM,                     STD_RIGHT_ARROW "DP",                          STD_RIGHT_ARROW "DP",                                                         SLS_ENABLED  },
/*  127 */  { fnDrop,                      NOPARAM,                     "DROP" STD_DOWN_ARROW,                         "DROP" STD_DOWN_ARROW,                                                        SLS_ENABLED  },
/*  128 */  { fnDropY,                     NOPARAM,                     "DROPy",                                       "DROPy",                                                                      SLS_ENABLED  },
/*  129 */  { itemToBeCoded,               NOPARAM,                     "DSE",                                         "DSE",                                                                        SLS_UNCHANGED},
/*  130 */  { itemToBeCoded,               NOPARAM,                     "DSL",                                         "DSL",                                                                        SLS_UNCHANGED},
/*  131 */  { fnDisplayFormatDsp,          TM_VALUE,                    "DSP",                                         "DSP",                                                                        SLS_UNCHANGED},
/*  132 */  { fnDisplayStack,              TM_VALUE,                    "DSTACK",                                      "DSTACK",                                                                     SLS_UNCHANGED},
/*  133 */  { itemToBeCoded,               NOPARAM,                     "DSZ",                                         "DSZ",                                                                        SLS_UNCHANGED},
/*  134 */  { fnAngularMode,               AM_DMS,                      "D.MS",                                        "d.ms",                                                                       SLS_UNCHANGED},
/*  135 */  { fnCvtToCurrentAngularMode,   AM_DMS,                      "D.MS" STD_RIGHT_ARROW,                        "D.MS" STD_RIGHT_ARROW,                                                       SLS_ENABLED  },
/*  136 */  { fnDateFormat,                DF_DMY,                      "D.MY",                                        "D.MY",                                                                       SLS_UNCHANGED},
/*  137 */  { itemToBeCoded,               NOPARAM,                     "D" STD_RIGHT_ARROW "J",                       "D" STD_RIGHT_ARROW "J",                                                      SLS_UNCHANGED},
/*  138 */  { fnCvtDegToRad,               NOPARAM,                     "D" STD_RIGHT_ARROW "R",                       "D" STD_RIGHT_ARROW "R",                                                      SLS_ENABLED  },
/*  139 */  { fnConstant,                  8,                           "e",                                           "e",                                                                          SLS_ENABLED  },
/*  140 */  { fnConstant,                  9,                           "e" STD_SUB_E,                                 "e" STD_SUB_E,                                                                SLS_ENABLED  },
/*  141 */  { itemToBeCoded,               NOPARAM,                     "EIGVAL",                                      "EIGVAL",                                                                     SLS_UNCHANGED},
/*  142 */  { itemToBeCoded,               NOPARAM,                     "EIGVEC",                                      "EIGVEC",                                                                     SLS_UNCHANGED},
/*  143 */  { itemToBeCoded,               NOPARAM,                     "END",                                         "END",                                                                        SLS_UNCHANGED},
/*  144 */  { itemToBeCoded,               NOPARAM,                     "ENDP",                                        "End",                                                                        SLS_UNCHANGED},
/*  145 */  { fnDisplayFormatEng,          TM_VALUE,                    "ENG",                                         "ENG",                                                                        SLS_UNCHANGED},
/*  146 */  { fnDisplayOvr,                DO_ENG,                      "ENGOVR",                                      "ENGOVR",                                                                     SLS_UNCHANGED},
/*  147 */  { itemToBeCoded,               NOPARAM,                     "ENORM",                                       "ENORM",                                                                      SLS_UNCHANGED},
/*  148 */  { addItemToBuffer,             NOPARAM,                     "ENTER" STD_UP_ARROW,                          "ENTER" STD_UP_ARROW,                                                         SLS_DISABLED },
/*  149 */  { itemToBeCoded,               NOPARAM,                     "ENTRY?",                                      "ENTRY?",                                                                     SLS_UNCHANGED},
/*  150 */  { itemToBeCoded,               NOPARAM,                     "EQN",                                         "EQN",                                                                        SLS_UNCHANGED},
/*  151 */  { itemToBeCoded,               NOPARAM,                     "EQ.DEL",                                      "DELETE",                                                                     SLS_UNCHANGED},
/*  152 */  { itemToBeCoded,               NOPARAM,                     "EQ.EDI",                                      "EDIT",                                                                       SLS_UNCHANGED},
/*  153 */  { itemToBeCoded,               NOPARAM,                     "EQ.NEW",                                      "NEW",                                                                        SLS_UNCHANGED},
/*  154 */  { itemToBeCoded,               NOPARAM,                     "erf",                                         "erf",                                                                        SLS_UNCHANGED},
/*  155 */  { itemToBeCoded,               NOPARAM,                     "erfc",                                        "erfc",                                                                       SLS_UNCHANGED},
/*  156 */  { itemToBeCoded,               NOPARAM,                     "ERR",                                         "ERR",                                                                        SLS_UNCHANGED},
/*  157 */  { itemToBeCoded,               NOPARAM,                     "EVEN?",                                       "EVEN?",                                                                      SLS_UNCHANGED},
/*  158 */  { fnExp,                       NOPARAM,                     "e" STD_SUP_x,                                 "e" STD_SUP_x,                                                                SLS_ENABLED  },
/*  159 */  { itemToBeCoded,               NOPARAM,                     "EXITALL",                                     "EXITall",                                                                    SLS_UNCHANGED},
/*  160 */  { itemToBeCoded,               NOPARAM,                     "EXP",                                         "EXP",                                                                        SLS_UNCHANGED},
/*  161 */  { fnCurveFitting,              CF_EXPONENTIAL_FITTING,      "ExpF",                                        "ExpF",                                                                       SLS_UNCHANGED},
/*  162 */  { itemToBeCoded,               NOPARAM,                     "Expon",                                       "Expon",                                                                      SLS_UNCHANGED},
/*  163 */  { itemToBeCoded,               NOPARAM,                     "Expon" STD_SUB_p,                             "Expon" STD_SUB_p,                                                            SLS_UNCHANGED},
/*  164 */  { itemToBeCoded,               NOPARAM,                     "Expon" STD_SUB_e,                             "Expon" STD_SUB_e,                                                            SLS_UNCHANGED},
/*  165 */  { itemToBeCoded,               NOPARAM,                     "Expon" STD_SUP_MINUS_1,                       "Expon" STD_SUP_MINUS_1,                                                      SLS_UNCHANGED},
/*  166 */  { itemToBeCoded,               NOPARAM,                     "Expon:",                                      "Expon:",                                                                     SLS_UNCHANGED},
/*  167 */  { itemToBeCoded,               NOPARAM,                     "EXPT",                                        "EXPT",                                                                       SLS_UNCHANGED},
/*  168 */  { itemToBeCoded,               NOPARAM,                     "e" STD_SUP_x "-1",                            "e" STD_SUP_x "-1",                                                           SLS_ENABLED  },
/*  169 */  { fnConstant,                  10,                          "e/m" STD_SUB_e,                               "e/m" STD_SUB_e,                                                              SLS_ENABLED  },
/*  170 */  { itemToBeCoded,               NOPARAM,                     "E:",                                          "E:",                                                                         SLS_UNCHANGED},
/*  171 */  { fnConstant,                  11,                          "F",                                           "F",                                                                          SLS_ENABLED  },
/*  172 */  { itemToBeCoded,               NOPARAM,                     "FAST",                                        "FAST",                                                                       SLS_UNCHANGED},
/*  173 */  { itemToBeCoded,               NOPARAM,                     "FB",                                          "FB",                                                                         SLS_UNCHANGED},
/*  174 */  { itemToBeCoded,               NOPARAM,                     "FCNS",                                        "FCNS",                                                                       SLS_UNCHANGED},
/*  175 */  { fnIsFlagClear,               TM_FLAG,                     "FC?",                                         "FC?",                                                                        SLS_UNCHANGED},
/*  176 */  { fnIsFlagClearClear,          TM_FLAG,                     "FC?C",                                        "FC?C",                                                                       SLS_UNCHANGED},
/*  177 */  { fnIsFlagClearFlip,           TM_FLAG,                     "FC?F",                                        "FC?F",                                                                       SLS_UNCHANGED},
/*  178 */  { fnIsFlagClearSet,            TM_FLAG,                     "FC?S",                                        "FC?S",                                                                       SLS_UNCHANGED},
/*  179 */  { fnCvtFtM,                    multiply,                    "ft." STD_RIGHT_ARROW "m",                     "ft." STD_RIGHT_ARROW "m",                                                    SLS_ENABLED  },
/*  180 */  { fnFlipFlag,                  TM_FLAG,                     "FF",                                          "FF",                                                                         SLS_ENABLED  },
/*  181 */  { itemToBeCoded,               NOPARAM,                     "FIB",                                         "FIB",                                                                        SLS_UNCHANGED},
/*  182 */  { fnFillStack,                 NOPARAM,                     "FILL",                                        "FILL",                                                                       SLS_ENABLED  },
/*  183 */  { itemToBeCoded,               NOPARAM,                     "FIN",                                         "FIN",                                                                        SLS_UNCHANGED},
/*  184 */  { itemToBeCoded,               NOPARAM,                     "FINTS",                                       "FINTS",                                                                      SLS_UNCHANGED},
/*  185 */  { fnDisplayFormatFix,          TM_VALUE,                    "FIX",                                         "FIX",                                                                        SLS_UNCHANGED},
/*  186 */  { itemToBeCoded,               NOPARAM,                     "FLAGS",                                       "FLAGS",                                                                      SLS_UNCHANGED},
/*  187 */  { itemToBeCoded,               NOPARAM,                     "FLASH",                                       "FLASH",                                                                      SLS_UNCHANGED},
/*  188 */  { fnFreeFlashMemory,           NOPARAM,                     "FLASH?",                                      "FLASH?",                                                                     SLS_ENABLED  },
/*  189 */  { fnFloor,                     NOPARAM,                     "FLOOR",                                       "FLOOR",                                                                      SLS_ENABLED  },
/*  190 */  { fnFp,                        NOPARAM,                     "FP",                                          "FP",                                                                         SLS_ENABLED  },
/*  191 */  { itemToBeCoded,               NOPARAM,                     "FP?",                                         "FP?",                                                                        SLS_UNCHANGED},
/*  192 */  { itemToBeCoded,               NOPARAM,                     "F" STD_SUB_p "(x)",                           "F" STD_SUB_p "(x)",                                                          SLS_UNCHANGED},
/*  193 */  { itemToBeCoded,               NOPARAM,                     "F" STD_SUB_e "(x)",                           "F" STD_SUB_e "(x)",                                                          SLS_UNCHANGED},
/*  194 */  { itemToBeCoded,               NOPARAM,                     "F(x)",                                        "F(x)",                                                                       SLS_UNCHANGED},
/*  195 */  { itemToBeCoded,               NOPARAM,                     "F" STD_SUP_MINUS_1 "(p)",                     "F" STD_SUP_MINUS_1 "(p)",                                                    SLS_UNCHANGED},
/*  196 */  { fnCvtRatioDb,                20,                          "fr" STD_RIGHT_ARROW "dB",                     "field",                                                                      SLS_ENABLED  },
/*  197 */  { fnIsFlagSet,                 TM_FLAG,                     "FS?",                                         "FS?",                                                                        SLS_UNCHANGED},
/*  198 */  { fnIsFlagSetClear,            TM_FLAG,                     "FS?C",                                        "FS?C",                                                                       SLS_UNCHANGED},
/*  199 */  { fnIsFlagSetFlip,             TM_FLAG,                     "FS?F",                                        "FS?F",                                                                       SLS_UNCHANGED},
/*  200 */  { fnIsFlagSetSet,              TM_FLAG,                     "FS?S",                                        "FS?S",                                                                       SLS_UNCHANGED},
/*  201 */  { fnCvtSfeetM,                 multiply,                    "ft" STD_US STD_RIGHT_ARROW "m",               "survey",                                                                     SLS_ENABLED  },
/*  202 */  { itemToBeCoded,               NOPARAM,                     "FV",                                          "FV",                                                                         SLS_UNCHANGED},
/*  203 */  { fnCvtFlozukM3,               multiply,                    "fz" STD_UK STD_RIGHT_ARROW "m" STD_SUP_3,     "floz" STD_UK,                                                                SLS_ENABLED  },
/*  204 */  { fnCvtFlozusM3,               multiply,                    "fz" STD_US STD_RIGHT_ARROW "m" STD_SUP_3,     "floz" STD_US,                                                                SLS_ENABLED  },
/*  205 */  { fnConstant,                  12,                          "F" STD_SUB_alpha,                             "F" STD_SUB_alpha,                                                            SLS_ENABLED  },
/*  206 */  { fnConstant,                  13,                          "F" STD_SUB_delta,                             "F" STD_SUB_delta,                                                            SLS_ENABLED  },
/*  207 */  { itemToBeCoded,               NOPARAM,                     "F:",                                          "F:",                                                                         SLS_UNCHANGED},
/*  208 */  { itemToBeCoded,               NOPARAM,                     "f" STD_RIGHT_SINGLE_QUOTE,                    "f" STD_RIGHT_SINGLE_QUOTE,                                                   SLS_UNCHANGED},
/*  209 */  { itemToBeCoded,               NOPARAM,                     "f" STD_RIGHT_DOUBLE_QUOTE,                    "f" STD_RIGHT_DOUBLE_QUOTE,                                                   SLS_UNCHANGED},
/*  210 */  { itemToBeCoded,               NOPARAM,                     "f" STD_RIGHT_SINGLE_QUOTE "(x)",              "f" STD_RIGHT_SINGLE_QUOTE "(x)",                                             SLS_UNCHANGED},
/*  211 */  { itemToBeCoded,               NOPARAM,                     "f" STD_RIGHT_DOUBLE_QUOTE "(x)",              "f" STD_RIGHT_DOUBLE_QUOTE "(x)",                                             SLS_UNCHANGED},
/*  212 */  { itemToBeCoded,               NOPARAM,                     "F&p:",                                        "F&p:",                                                                       SLS_UNCHANGED},
/*  213 */  { fnConstant,                  14,                          "G",                                           "G",                                                                          SLS_ENABLED  },
/*  214 */  { fnConstant,                  15,                          "G" STD_SUB_0,                                 "G" STD_SUB_0,                                                                SLS_ENABLED  },
/*  215 */  { fnDisplayFormatGap,          TM_VALUE,                    "GAP",                                         "GAP",                                                                        SLS_UNCHANGED},
/*  216 */  { fnConstant,                  16,                          "G" STD_SUB_C,                                 "G" STD_SUB_C,                                                                SLS_ENABLED  },
/*  217 */  { fnGcd,                       NOPARAM,                     "GCD",                                         "GCD",                                                                        SLS_ENABLED  },
/*  218 */  { itemToBeCoded,               NOPARAM,                     "g" STD_SUB_d,                                 "g" STD_SUB_d,                                                                SLS_UNCHANGED},
/*  219 */  { itemToBeCoded,               NOPARAM,                     "g" STD_SUB_d STD_SUP_MINUS_1,                 "g" STD_SUB_d STD_SUP_MINUS_1,                                                SLS_UNCHANGED},
/*  220 */  { fnConstant,                  17,                          "g" STD_SUB_e,                                 "g" STD_SUB_e,                                                                SLS_ENABLED  },
/*  221 */  { itemToBeCoded,               NOPARAM,                     "Geom",                                        "Geom",                                                                       SLS_UNCHANGED},
/*  222 */  { itemToBeCoded,               NOPARAM,                     "Geom" STD_SUB_p,                              "Geom" STD_SUB_p,                                                             SLS_UNCHANGED},
/*  223 */  { itemToBeCoded,               NOPARAM,                     "Geom" STD_SUB_e,                              "Geom" STD_SUB_e,                                                             SLS_UNCHANGED},
/*  224 */  { itemToBeCoded,               NOPARAM,                     "Geom" STD_SUP_MINUS_1,                        "Geom" STD_SUP_MINUS_1,                                                       SLS_UNCHANGED},
/*  225 */  { itemToBeCoded,               NOPARAM,                     "Geom:",                                       "Geom:",                                                                      SLS_UNCHANGED},
/*  226 */  { fnCvtGalukM3,                multiply,                    "gl" STD_UK STD_RIGHT_ARROW "m" STD_SUP_3,     "gl" STD_UK STD_RIGHT_ARROW "m" STD_SUP_3,                                    SLS_ENABLED  },
/*  227 */  { fnCvtGalusM3,                multiply,                    "gl" STD_US STD_RIGHT_ARROW "m" STD_SUP_3,     "gl" STD_US STD_RIGHT_ARROW "m" STD_SUP_3,                                    SLS_ENABLED  },
/*  228 */  { fnConstant,                  18,                          "GM" STD_SUB_EARTH,                            "GM" STD_SUB_EARTH,                                                           SLS_ENABLED  },
/*  229 */  { fnAngularMode,               AM_GRAD,                     "GRAD",                                        "GRAD",                                                                       SLS_UNCHANGED},
/*  230 */  { fnCvtToCurrentAngularMode,   AM_GRAD,                     "GRAD" STD_RIGHT_ARROW,                        "GRAD" STD_RIGHT_ARROW,                                                       SLS_ENABLED  },
/*  231 */  { itemToBeCoded,               NOPARAM,                     "GTO",                                         "GTO",                                                                        SLS_UNCHANGED},
/*  232 */  { itemToBeCoded,               NOPARAM,                     "GTO.",                                        "GTO.",                                                                       SLS_UNCHANGED},
/*  233 */  { fnConstant,                  19,                          "g" STD_SUB_EARTH,                             "g" STD_SUB_EARTH,                                                            SLS_ENABLED  },
/*  234 */  { fnConstant,                  20,                          STD_PLANCK,                                    STD_PLANCK,                                                                   SLS_ENABLED  },
/*  235 */  { itemToBeCoded,               NOPARAM,                     "H" STD_SUB_n,                                 "H" STD_SUB_n,                                                                SLS_UNCHANGED},
/*  236 */  { itemToBeCoded,               NOPARAM,                     "H" STD_SUB_n STD_SUB_p,                       "H" STD_SUB_n STD_SUB_p,                                                      SLS_UNCHANGED},
/*  237 */  { fnCvtHpeW,                   multiply,                    "hp" STD_SUB_E STD_RIGHT_ARROW "W",            "hp" STD_SUB_E STD_RIGHT_ARROW "W",                                           SLS_ENABLED  },
/*  238 */  { fnCvtHpmW,                   multiply,                    "hp" STD_SUB_M STD_RIGHT_ARROW "W",            "hp" STD_SUB_M STD_RIGHT_ARROW "W",                                           SLS_ENABLED  },
/*  239 */  { fnCvtHpukW,                  multiply,                    "hp" STD_UK STD_RIGHT_ARROW "W",               "hp" STD_UK STD_RIGHT_ARROW "W",                                              SLS_ENABLED  },
/*  240 */  { itemToBeCoded,               NOPARAM,                     "Hyper",                                       "Hyper",                                                                      SLS_UNCHANGED},
/*  241 */  { itemToBeCoded,               NOPARAM,                     "Hyper" STD_SUB_p,                             "Hyper" STD_SUB_p,                                                            SLS_UNCHANGED},
/*  242 */  { itemToBeCoded,               NOPARAM,                     "Hyper" STD_SUB_e,                             "Hyper" STD_SUB_e,                                                            SLS_UNCHANGED},
/*  243 */  { itemToBeCoded,               NOPARAM,                     "Hyper" STD_SUP_MINUS_1,                       "Hyper" STD_SUP_MINUS_1,                                                      SLS_UNCHANGED},
/*  244 */  { itemToBeCoded,               NOPARAM,                     "Hyper:",                                      "Hyper:",                                                                     SLS_UNCHANGED},
/*  245 */  { fnConstant,                  21,                          STD_PLANCK_2PI,                                STD_PLANCK_2PI,                                                               SLS_ENABLED  },
/*  246 */  { addItemToBuffer,             REGISTER_I,                  "I",                                           "I",                                                                          SLS_UNCHANGED},
/*  247 */  { itemToBeCoded,               NOPARAM,                     "IDIV",                                        "IDIV",                                                                       SLS_UNCHANGED},
/*  248 */  { fnCvtInhgPa,                 multiply,                    "iHg" STD_RIGHT_ARROW "Pa",                    "in.Hg",                                                                      SLS_ENABLED  },
/*  249 */  { itemToBeCoded,               NOPARAM,                     "IINTS",                                       "IINTS",                                                                      SLS_UNCHANGED},
/*  250 */  { fnImaginaryPart,             NOPARAM,                     "IM",                                          "Im",                                                                         SLS_ENABLED  },
/*  251 */  { fnFractionType,              FT_IMPROPER,                 "IMPFRC",                                      "d/c",                                                                        SLS_UNCHANGED},
/*  252 */  { itemToBeCoded,               NOPARAM,                     "INC",                                         "INC",                                                                        SLS_UNCHANGED},
/*  253 */  { itemToBeCoded,               NOPARAM,                     "INDEX",                                       "INDEX",                                                                      SLS_UNCHANGED},
/*  254 */  { itemToBeCoded,               NOPARAM,                     "INFO",                                        "INFO",                                                                       SLS_UNCHANGED},
/*  255 */  { itemToBeCoded,               NOPARAM,                     "INPUT",                                       "INPUT",                                                                      SLS_UNCHANGED},
/*  256 */  { itemToBeCoded,               NOPARAM,                     "INTS",                                        "INTS",                                                                       SLS_UNCHANGED},
/*  257 */  { itemToBeCoded,               NOPARAM,                     "INT?",                                        "INT?",                                                                       SLS_UNCHANGED},
/*  258 */  { fnCvtInchM,                  multiply,                    "in." STD_RIGHT_ARROW "m",                     "in." STD_RIGHT_ARROW "m",                                                    SLS_ENABLED  },
/*  259 */  { fnIp,                        NOPARAM,                     "IP",                                          "IP",                                                                         SLS_ENABLED  },
/*  260 */  { itemToBeCoded,               NOPARAM,                     "ISE",                                         "ISE",                                                                        SLS_UNCHANGED},
/*  261 */  { itemToBeCoded,               NOPARAM,                     "ISG",                                         "ISG",                                                                        SLS_UNCHANGED},
/*  262 */  { itemToBeCoded,               NOPARAM,                     "ISZ",                                         "ISZ",                                                                        SLS_UNCHANGED},
/*  263 */  { itemToBeCoded,               NOPARAM,                     "I" STD_SUB_x STD_SUB_y STD_SUB_z,             "I" STD_SUB_x STD_SUB_y STD_SUB_z,                                            SLS_UNCHANGED},
/*  264 */  { itemToBeCoded,               NOPARAM,                     "I" STD_GAMMA STD_SUB_p,                       "I" STD_GAMMA STD_SUB_p,                                                      SLS_UNCHANGED},
/*  265 */  { itemToBeCoded,               NOPARAM,                     "I" STD_GAMMA STD_SUB_q,                       "I" STD_GAMMA STD_SUB_q,                                                      SLS_UNCHANGED},
/*  266 */  { itemToBeCoded,               NOPARAM,                     "I+",                                          "I+",                                                                         SLS_UNCHANGED},
/*  267 */  { itemToBeCoded,               NOPARAM,                     "I-",                                          "I-",                                                                         SLS_UNCHANGED},
/*  268 */  { itemToBeCoded,               NOPARAM,                     "I/O",                                         "I/O",                                                                        SLS_UNCHANGED},
/*  269 */  { itemToBeCoded,               NOPARAM,                     "i%/a",                                        "i%/a",                                                                       SLS_UNCHANGED},
/*  270 */  { addItemToBuffer,             REGISTER_J,                  "J",                                           "J",                                                                          SLS_UNCHANGED},
/*  271 */  { itemToBeCoded,               NOPARAM,                     "J" STD_SUB_y "(x)",                           "J" STD_SUB_y "(x)",                                                          SLS_UNCHANGED},
/*  272 */  { itemToBeCoded,               NOPARAM,                     "J+",                                          "J+",                                                                         SLS_UNCHANGED},
/*  273 */  { itemToBeCoded,               NOPARAM,                     "J-",                                          "J-",                                                                         SLS_UNCHANGED},
/*  274 */  { itemToBeCoded,               NOPARAM,                     "J/G",                                         "J/G",                                                                        SLS_UNCHANGED},
/*  275 */  { fnCvtBtuJ,                   divide,                      "J" STD_RIGHT_ARROW "Btu",                     "J" STD_RIGHT_ARROW "Btu",                                                    SLS_ENABLED  },
/*  276 */  { fnCvtCalJ,                   divide,                      "J" STD_RIGHT_ARROW "cal",                     "J" STD_RIGHT_ARROW "cal",                                                    SLS_ENABLED  },
/*  277 */  { itemToBeCoded,               NOPARAM,                     "J" STD_RIGHT_ARROW "D",                       "J" STD_RIGHT_ARROW "D",                                                      SLS_UNCHANGED},
/*  278 */  { fnCvtWhJ,                    divide,                      "J" STD_RIGHT_ARROW "Wh",                      "J" STD_RIGHT_ARROW "Wh",                                                     SLS_ENABLED  },
/*  279 */  { addItemToBuffer,             REGISTER_K,                  "K",                                           "K",                                                                          SLS_UNCHANGED},
/*  280 */  { fnConstant,                  22,                          "k",                                           "k",                                                                          SLS_ENABLED  },
/*  281 */  { itemToBeCoded,               NOPARAM,                     "KEY",                                         "KEY",                                                                        SLS_UNCHANGED},
/*  282 */  { itemToBeCoded,               NOPARAM,                     "KEYG",                                        "KEYG",                                                                       SLS_UNCHANGED},
/*  283 */  { itemToBeCoded,               NOPARAM,                     "KEYX",                                        "KEYX",                                                                       SLS_UNCHANGED},
/*  284 */  { itemToBeCoded,               NOPARAM,                     "KEY?",                                        "KEY?",                                                                       SLS_UNCHANGED},
/*  285 */  { fnCvtCwtKg,                  divide,                      "kg" STD_RIGHT_ARROW "cwt",                    "kg" STD_RIGHT_ARROW "cwt",                                                   SLS_ENABLED  },
/*  286 */  { fnCvtLbKg,                   divide,                      "kg" STD_RIGHT_ARROW "lb.",                    "kg" STD_RIGHT_ARROW "lb.",                                                   SLS_ENABLED  },
/*  287 */  { fnCvtOzKg,                   divide,                      "kg" STD_RIGHT_ARROW "oz",                     "kg" STD_RIGHT_ARROW "oz",                                                    SLS_ENABLED  },
/*  288 */  { fnCvtShortcwtKg,             divide,                      "kg" STD_RIGHT_ARROW "scw",                    "kg " STD_RIGHT_ARROW,                                                        SLS_ENABLED  },
/*  289 */  { fnCvtStoneKg,                divide,                      "kg" STD_RIGHT_ARROW "sto",                    "kg " STD_RIGHT_ARROW,                                                        SLS_ENABLED  },
/*  290 */  { fnCvtShorttonKg,             divide,                      "kg" STD_RIGHT_ARROW "s.t",                    "kg " STD_RIGHT_ARROW,                                                        SLS_ENABLED  },
/*  291 */  { fnCvtTonKg,                  divide,                      "kg" STD_RIGHT_ARROW "ton",                    "kg" STD_RIGHT_ARROW "ton",                                                   SLS_ENABLED  },
/*  292 */  { fnCvtTrozKg,                 divide,                      "kg" STD_RIGHT_ARROW "tr.oz",                  "kg " STD_RIGHT_ARROW,                                                        SLS_ENABLED  },
/*  293 */  { fnConstant,                  23,                          "K" STD_SUB_J,                                 "K" STD_SUB_J,                                                                SLS_ENABLED  },
/*  294 */  { itemToBeCoded,               NOPARAM,                     "KTYP?",                                       "KTYP?",                                                                      SLS_UNCHANGED},
/*  295 */  { addItemToBuffer,             REGISTER_L,                  "L",                                           "L",                                                                          SLS_UNCHANGED},
/*  296 */  { fnLastX,                     NOPARAM,                     "LASTx",                                       "LASTx",                                                                      SLS_UNCHANGED},     //JM LastX Jaco added the Function in stack.c
/*  297 */  { fnCvtLbfN,                   multiply,                    "lbf" STD_RIGHT_ARROW "N",                     "lbf" STD_RIGHT_ARROW "N",                                                    SLS_ENABLED  },
/*  298 */  { itemToBeCoded,               NOPARAM,                     "LBL",                                         "LBL",                                                                        SLS_UNCHANGED},
/*  299 */  { itemToBeCoded,               NOPARAM,                     "LBL?",                                        "LBL?",                                                                       SLS_UNCHANGED},
/*  300 */  { fnCvtLbKg,                   multiply,                    "lb." STD_RIGHT_ARROW "kg",                    "lb." STD_RIGHT_ARROW "kg",                                                   SLS_ENABLED  },
/*  301 */  { fnLcm,                       NOPARAM,                     "LCM",                                         "LCM",                                                                        SLS_ENABLED  },
/*  302 */  { itemToBeCoded,               NOPARAM,                     "LEAP?",                                       "LEAP?",                                                                      SLS_UNCHANGED},
/*  303 */  { itemToBeCoded,               NOPARAM,                     "LgNrm",                                       "LgNrm",                                                                      SLS_UNCHANGED},
/*  304 */  { itemToBeCoded,               NOPARAM,                     "LgNrm" STD_SUB_p,                             "LgNrm" STD_SUB_p,                                                            SLS_UNCHANGED},
/*  305 */  { itemToBeCoded,               NOPARAM,                     "LgNrm" STD_SUB_e,                             "LgNrm" STD_SUB_e,                                                            SLS_UNCHANGED},
/*  306 */  { itemToBeCoded,               NOPARAM,                     "LgNrm" STD_SUP_MINUS_1,                       "LgNrm" STD_SUP_MINUS_1,                                                      SLS_UNCHANGED},
/*  307 */  { itemToBeCoded,               NOPARAM,                     "LgNrm:",                                      "LgNrm:",                                                                     SLS_UNCHANGED},
/*  308 */  { fnCurveFitting,              CF_LINEAR_FITTING,           "LinF",                                        "LinF",                                                                       SLS_UNCHANGED},
/*  309 */  { itemToBeCoded,               NOPARAM,                     "LJ",                                          "LJ",                                                                         SLS_UNCHANGED},
/*  310 */  { fnLn,                        NOPARAM,                     "LN",                                          "LN",                                                                         SLS_ENABLED  },  //JM3 change ln to LN
/*  311 */  { itemToBeCoded,               NOPARAM,                     "L" STD_SUB_m ,                                "L" STD_SUB_m ,                                                               SLS_UNCHANGED},
/*  312 */  { itemToBeCoded,               NOPARAM,                     "LN1+x",                                       "ln 1+x",                                                                     SLS_ENABLED  },
/*  313 */  { itemToBeCoded,               NOPARAM,                     "L" STD_SUB_m STD_SUB_alpha,                   "L" STD_SUB_m STD_SUB_alpha,                                                  SLS_UNCHANGED},
/*  314 */  { itemToBeCoded,               NOPARAM,                     "LN" STD_beta,                                 "ln" STD_beta,                                                                SLS_ENABLED  },
/*  315 */  { fnLnGamma,                   NOPARAM,                     "LN" STD_GAMMA,                                "ln" STD_GAMMA,                                                               SLS_ENABLED  },
/*  316 */  { itemToBeCoded,               NOPARAM,                     "LOAD",                                        "LOAD",                                                                       SLS_UNCHANGED},
/*  317 */  { itemToBeCoded,               NOPARAM,                     "LOADP",                                       "LOADP",                                                                      SLS_UNCHANGED},
/*  318 */  { itemToBeCoded,               NOPARAM,                     "LOADR",                                       "LOADR",                                                                      SLS_UNCHANGED},
/*  319 */  { itemToBeCoded,               NOPARAM,                     "LOADSS",                                      "LOADSS",                                                                     SLS_UNCHANGED},
/*  320 */  { itemToBeCoded,               NOPARAM,                     "LOAD" STD_SIGMA,                              "LOAD" STD_SIGMA,                                                             SLS_UNCHANGED},
/*  321 */  { allocateLocalRegisters,      TM_VALUE,                    "LocR",                                        "LocR",                                                                       SLS_UNCHANGED},
/*  322 */  { fnGetLocR,                   NOPARAM,                     "LocR?",                                       "LocR?",                                                                      SLS_ENABLED  },
/*  JM 323 */  { fnLog10,                     NOPARAM,                     "LOG" STD_SUB_10,                              "LOG",                                                                         SLS_ENABLED  },  //JM Change lg to LOG
/*  324 */  { fnLog2,                      NOPARAM,                     "LOG" STD_SUB_2,                               "lb x",                                                                       SLS_ENABLED  },
/*  325 */  { fnCurveFitting,              CF_LOGARITHMIC_FITTING,      "LogF",                                        "LogF",                                                                       SLS_UNCHANGED},
/*  326 */  { itemToBeCoded,               NOPARAM,                     "Logis",                                       "Logis",                                                                      SLS_UNCHANGED},
/*  327 */  { itemToBeCoded,               NOPARAM,                     "Logis" STD_SUB_p,                             "Logis" STD_SUB_p,                                                            SLS_UNCHANGED},
/*  328 */  { itemToBeCoded,               NOPARAM,                     "Logis" STD_SUB_e,                             "Logis" STD_SUB_e,                                                            SLS_UNCHANGED},
/*  329 */  { itemToBeCoded,               NOPARAM,                     "Logis" STD_SUP_MINUS_1,                       "Logis" STD_SUP_MINUS_1,                                                      SLS_UNCHANGED},
/*  330 */  { itemToBeCoded,               NOPARAM,                     "Logis:",                                      "Logis:",                                                                     SLS_UNCHANGED},
/*  331 */  { itemToBeCoded,               NOPARAM,                     "LOG" STD_SUB_x "y",                           "log" STD_SUB_x "y",                                                          SLS_UNCHANGED},
/*  332 */  { itemToBeCoded,               NOPARAM,                     "LOOP",                                        "LOOP",                                                                       SLS_UNCHANGED},
/*  333 */  { fnConstant,                  24,                          "l" STD_SUB_P STD_SUB_L,                       "l" STD_SUB_P STD_SUB_L,                                                      SLS_ENABLED  },
/*  334 */  { fnCvtLyM,                    multiply,                    "ly" STD_RIGHT_ARROW "m",                      "ly" STD_RIGHT_ARROW "m",                                                     SLS_ENABLED  },
/*  335 */  { fnLeadingZeros,              false,                       "LZOFF",                                       "LZOFF",                                                                      SLS_UNCHANGED},
/*  336 */  { fnLeadingZeros,              true,                        "LZON",                                        "LZON",                                                                       SLS_UNCHANGED},
/*  337 */  { itemToBeCoded,               NOPARAM,                     "L.R.",                                        "L.R.",                                                                       SLS_UNCHANGED},
/*  338 */  { fnCvtAcreM2,                 divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ac",            "m" STD_SUP_2 " " STD_RIGHT_ARROW,                                            SLS_ENABLED  },
/*  339 */  { fnCvtAcreusM2,               divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ac" STD_US,     "m" STD_SUP_2 " " STD_RIGHT_ARROW,                                            SLS_ENABLED  },
/*  340 */  { fnCvtFlozukM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "fz" STD_UK,     "m" STD_SUP_3 " " STD_RIGHT_ARROW,                                            SLS_ENABLED  },
/*  341 */  { fnCvtFlozusM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "fz" STD_US,     "m" STD_SUP_3 " " STD_RIGHT_ARROW,                                            SLS_ENABLED  },
/*  342 */  { fnCvtGalukM3,                divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_UK,     "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_UK,                                    SLS_ENABLED  },
/*  343 */  { fnCvtGalusM3,                divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_US,     "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_US,                                    SLS_ENABLED  },
/*  344 */  { itemToBeCoded,               NOPARAM,                     "MANT",                                        "MANT",                                                                       SLS_UNCHANGED},
/*  345 */  { itemToBeCoded,               NOPARAM,                     "MASKL",                                       "MASKL",                                                                      SLS_UNCHANGED},
/*  346 */  { itemToBeCoded,               NOPARAM,                     "MASKR",                                       "MASKR",                                                                      SLS_UNCHANGED},
/*  347 */  { itemToBeCoded,               NOPARAM,                     "MATRS",                                       "MATRS",                                                                      SLS_UNCHANGED},
/*  348 */  { itemToBeCoded,               NOPARAM,                     "MATR?",                                       "MATR?",                                                                      SLS_UNCHANGED},
/*  349 */  { itemToBeCoded,               NOPARAM,                     "MATX",                                        "MATX",                                                                       SLS_UNCHANGED},
/*  350 */  { itemToBeCoded,               NOPARAM,                     "Mat_A",                                       "Mat A",                                                                      SLS_UNCHANGED},
/*  351 */  { itemToBeCoded,               NOPARAM,                     "Mat_B",                                       "Mat B",                                                                      SLS_UNCHANGED},
/*  352 */  { itemToBeCoded,               NOPARAM,                     "Mat_X",                                       "Mat X",                                                                      SLS_UNCHANGED},
/*  353 */  { itemToBeCoded,               NOPARAM,                     "MAX",                                         "max",                                                                        SLS_UNCHANGED},
/*  354 */  { fnConstant,                  25,                          "m" STD_SUB_e,                                 "m" STD_SUB_e,                                                                SLS_ENABLED  },
/*  355 */  { fnFreeMemory,                NOPARAM,                     "MEM?",                                        "MEM?",                                                                       SLS_ENABLED  },
/*  356 */  { itemToBeCoded,               NOPARAM,                     "MENU",                                        "MENU",                                                                       SLS_UNCHANGED},
/*  357 */  { itemToBeCoded,               NOPARAM,                     "MENUS",                                       "MENUS",                                                                      SLS_UNCHANGED},
/*  358 */  { itemToBeCoded,               NOPARAM,                     "MIN",                                         "min",                                                                        SLS_UNCHANGED},
/*  359 */  { fnMirror,                    NOPARAM,                     "MIRROR",                                      "MIRROR",                                                                     SLS_ENABLED  },
/*  360 */  { fnCvtMiM,                    multiply,                    "mi." STD_RIGHT_ARROW "m",                     "mi." STD_RIGHT_ARROW "m",                                                    SLS_ENABLED  },
/*  361 */  { fnConstant,                  26,                          "M" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "M" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,                                  SLS_ENABLED  },
/*  362 */  { fnConstant,                  27,                          "m" STD_SUB_n,                                 "m" STD_SUB_n,                                                                SLS_ENABLED  },
/*  363 */  { fnConstant,                  28,                          "m" STD_SUB_n "/m" STD_SUB_p,                  "m" STD_SUB_n "/m" STD_SUB_p,                                                 SLS_ENABLED  },
/*  364 */  { itemToBeCoded,               NOPARAM,                     "MOD",                                         "MOD",                                                                        SLS_UNCHANGED},
/*  365 */  { itemToBeCoded,               NOPARAM,                     "MODE",                                        "MODE",                                                                       SLS_UNCHANGED},
/*  366 */  { itemToBeCoded,               NOPARAM,                     "MONTH",                                       "MONTH",                                                                      SLS_UNCHANGED},
/*  367 */  { fnConstant,                  29,                          "m" STD_SUB_p,                                 "m" STD_SUB_p,                                                                SLS_ENABLED  },
/*  368 */  { fnConstant,                  30,                          "m" STD_SUB_P STD_SUB_L,                       "m" STD_SUB_P STD_SUB_L,                                                      SLS_ENABLED  },
/*  369 */  { fnConstant,                  31,                          "m" STD_SUB_p "/m" STD_SUB_e,                  "m" STD_SUB_p "/m" STD_SUB_e,                                                 SLS_ENABLED  },
/*  370 */  { itemToBeCoded,               NOPARAM,                     "MSG",                                         "MSG",                                                                        SLS_UNCHANGED},
/*  371 */  { fnConstant,                  32,                          "m" STD_SUB_u,                                 "m" STD_SUB_u,                                                                SLS_ENABLED  },
/*  372 */  { fnConstant,                  33,                          "m" STD_SUB_u "c" STD_SUP_2,                   "m" STD_SUB_u "c" STD_SUP_2,                                                  SLS_ENABLED  },
/*  373 */  { fnProductSign,               PS_CROSS,                    "MULT" STD_CROSS,                              "MULT" STD_CROSS,                                                             SLS_UNCHANGED},
/*  374 */  { fnProductSign,               PS_DOT,                      "MULT" STD_DOT,                                "MULT" STD_DOT,                                                               SLS_UNCHANGED},
/*  375 */  { fnAngularMode,               AM_MULTPI,                   "MUL" STD_pi,                                  "MUL" STD_pi,                                                                 SLS_UNCHANGED},
/*  376 */  { itemToBeCoded,               NOPARAM,                     "MVAR",                                        "MVAR",                                                                       SLS_UNCHANGED},
/*  377 */  { itemToBeCoded,               NOPARAM,                     "MyMenu",                                      "MyMenu",                                                                     SLS_UNCHANGED},
/*  378 */  { itemToBeCoded,               NOPARAM,                     "My" STD_alpha,                                "My" STD_alpha,                                                               SLS_UNCHANGED},
/*  379 */  { fnConstant,                  34,                          "m" STD_SUB_mu,                                "m" STD_SUB_mu,                                                               SLS_ENABLED  },
/*  380 */  { itemToBeCoded,               NOPARAM,                     "M.DELR",                                      "DELR",                                                                       SLS_UNCHANGED},
/*  381 */  { itemToBeCoded,               NOPARAM,                     "M.DIM",                                       "DIM",                                                                        SLS_UNCHANGED},
/*  382 */  { itemToBeCoded,               NOPARAM,                     "M.DIM?",                                      "DIM?",                                                                       SLS_UNCHANGED},
/*  383 */  { fnDateFormat,                DF_MDY,                      "M.DY",                                        "M.DY",                                                                       SLS_UNCHANGED},
/*  384 */  { itemToBeCoded,               NOPARAM,                     "M.EDI",                                       "EDIT",                                                                       SLS_UNCHANGED},
/*  385 */  { itemToBeCoded,               NOPARAM,                     "M.EDITN",                                     "EDITN",                                                                      SLS_UNCHANGED},
/*  386 */  { itemToBeCoded,               NOPARAM,                     "M.EDIT",                                      "EDIT",                                                                       SLS_UNCHANGED},
/*  387 */  { itemToBeCoded,               NOPARAM,                     "M.GET",                                       "GETM",                                                                       SLS_UNCHANGED},
/*  388 */  { itemToBeCoded,               NOPARAM,                     "M.GOTO",                                      "GOTO",                                                                       SLS_UNCHANGED},
/*  389 */  { itemToBeCoded,               NOPARAM,                     "M.GROW",                                      "GROW",                                                                       SLS_UNCHANGED},
/*  390 */  { itemToBeCoded,               NOPARAM,                     "M.INSR",                                      "INSR",                                                                       SLS_UNCHANGED},
/*  391 */  { itemToBeCoded,               NOPARAM,                     "M.LU",                                        "M.LU",                                                                       SLS_UNCHANGED},
/*  392 */  { itemToBeCoded,               NOPARAM,                     "M.NEW",                                       "NEW",                                                                        SLS_UNCHANGED},
/*  393 */  { itemToBeCoded,               NOPARAM,                     "M.OLD",                                       "OLD",                                                                        SLS_UNCHANGED},
/*  394 */  { itemToBeCoded,               NOPARAM,                     "M.PUT",                                       "PUTM",                                                                       SLS_UNCHANGED},
/*  395 */  { itemToBeCoded,               NOPARAM,                     "M.R" STD_LEFT_RIGHT_ARROWS "R",               "R" STD_LEFT_RIGHT_ARROWS "R",                                                SLS_UNCHANGED},
/*  396 */  { itemToBeCoded,               NOPARAM,                     "M.SIMQ",                                      "M.SIMQ",                                                                     SLS_UNCHANGED},
/*  397 */  { itemToBeCoded,               NOPARAM,                     "M.SQR?",                                      "M.SQR?",                                                                     SLS_UNCHANGED},
/*  398 */  { itemToBeCoded,               NOPARAM,                     "M.WRAP",                                      "WRAP",                                                                       SLS_UNCHANGED},
/*  399 */  { itemToBeCoded,               NOPARAM,                     "m:",                                          "m:",                                                                         SLS_UNCHANGED},
/*  400 */  { fnCvtAuM,                    divide,                      "m" STD_RIGHT_ARROW "au",                      "m" STD_RIGHT_ARROW "au",                                                     SLS_ENABLED  },
/*  401 */  { fnCvtFtM,                    divide,                      "m" STD_RIGHT_ARROW "ft.",                     "m" STD_RIGHT_ARROW "ft.",                                                    SLS_ENABLED  },
/*  402 */  { fnCvtSfeetM,                 divide,                      "m" STD_RIGHT_ARROW "ft" STD_US,               "m " STD_RIGHT_ARROW,                                                         SLS_ENABLED  },
/*  403 */  { fnCvtInchM,                  divide,                      "m" STD_RIGHT_ARROW "in.",                     "m" STD_RIGHT_ARROW "in.",                                                    SLS_ENABLED  },
/*  404 */  { fnCvtLyM,                    divide,                      "m" STD_RIGHT_ARROW "ly",                      "m" STD_RIGHT_ARROW "ly",                                                     SLS_ENABLED  },
/*  405 */  { fnCvtMiM,                    divide,                      "m" STD_RIGHT_ARROW "mi.",                     "m" STD_RIGHT_ARROW "mi.",                                                    SLS_ENABLED  },
/*  406 */  { fnCvtNmiM,                   divide,                      "m" STD_RIGHT_ARROW "nmi.",                    "m" STD_RIGHT_ARROW "nmi.",                                                   SLS_ENABLED  },
/*  407 */  { fnCvtPcM,                    divide,                      "m" STD_RIGHT_ARROW "pc",                      "m" STD_RIGHT_ARROW "pc",                                                     SLS_ENABLED  },
/*  408 */  { fnCvtPointM,                 divide,                      "m" STD_RIGHT_ARROW "pt.",                     "point",                                                                      SLS_ENABLED  },
/*  409 */  { fnCvtYardM,                  divide,                      "m" STD_RIGHT_ARROW "yd.",                     "m" STD_RIGHT_ARROW "yd.",                                                    SLS_ENABLED  },
/*  410 */  { fnConstant,                  35,                          "M" STD_SUB_SUN,                               "M" STD_SUB_SUN,                                                              SLS_ENABLED  },
/*  411 */  { fnConstant,                  36,                          "M" STD_SUB_EARTH,                             "M" STD_SUB_EARTH,                                                            SLS_ENABLED  },
/*  412 */  { fnConstant,                  37,                          "N" STD_SUB_A,                                 "N" STD_SUB_A,                                                                SLS_ENABLED  },
/*  413 */  { fnConstant,                  38,                          "NaN",                                         "NaN",                                                                        SLS_ENABLED  },
/*  414 */  { itemToBeCoded,               NOPARAM,                     "NAND",                                        "NAND",                                                                       SLS_UNCHANGED},
/*  415 */  { itemToBeCoded,               NOPARAM,                     "NaN?",                                        "NaN?",                                                                       SLS_UNCHANGED},
/*  416 */  { itemToBeCoded,               NOPARAM,                     "NBin",                                        "NBin",                                                                       SLS_UNCHANGED},
/*  417 */  { itemToBeCoded,               NOPARAM,                     "NBin" STD_SUB_p,                              "NBin" STD_SUB_p,                                                             SLS_UNCHANGED},
/*  418 */  { itemToBeCoded,               NOPARAM,                     "NBin" STD_SUB_e,                              "NBin" STD_SUB_e,                                                             SLS_UNCHANGED},
/*  419 */  { itemToBeCoded,               NOPARAM,                     "NBin" STD_SUP_MINUS_1,                        "NBin" STD_SUP_MINUS_1,                                                       SLS_UNCHANGED},
/*  420 */  { itemToBeCoded,               NOPARAM,                     "NBin:",                                       "NBin:",                                                                      SLS_UNCHANGED},
/*  421 */  { itemToBeCoded,               NOPARAM,                     "NEIGHB",                                      "NEIGHB",                                                                     SLS_UNCHANGED},
/*  422 */  { itemToBeCoded,               NOPARAM,                     "NEXTP",                                       "NEXTP",                                                                      SLS_UNCHANGED},
/*  423 */  { fnCvtNmiM,                   multiply,                    "nmi." STD_RIGHT_ARROW "m",                    "nmi." STD_RIGHT_ARROW "m",                                                   SLS_ENABLED  },
/*  424 */  { fnNop,                       NOPARAM,                     "NOP",                                         "NOP",                                                                        SLS_UNCHANGED},
/*  425 */  { itemToBeCoded,               NOPARAM,                     "NOR",                                         "NOR",                                                                        SLS_UNCHANGED},
/*  426 */  { itemToBeCoded,               NOPARAM,                     "Norml",                                       "Norml",                                                                      SLS_UNCHANGED},
/*  427 */  { itemToBeCoded,               NOPARAM,                     "Norml" STD_SUB_p,                             "Norml" STD_SUB_p,                                                            SLS_UNCHANGED},
/*  428 */  { itemToBeCoded,               NOPARAM,                     "Norml" STD_SUB_e,                             "Norml" STD_SUB_e,                                                            SLS_UNCHANGED},
/*  429 */  { itemToBeCoded,               NOPARAM,                     "Norml" STD_SUP_MINUS_1,                       "Norml" STD_SUP_MINUS_1,                                                      SLS_UNCHANGED},
/*  430 */  { itemToBeCoded,               NOPARAM,                     "Norml:",                                      "Norml:",                                                                     SLS_UNCHANGED},
/*  431 */  { fnLogicalNot,                NOPARAM,                     "NOT",                                         "NOT",                                                                        SLS_ENABLED  },
/*  432 */  { itemToBeCoded,               NOPARAM,                     "NPER",                                        "n" STD_SUB_P STD_SUB_E STD_SUB_R,                                            SLS_UNCHANGED},
/*  433 */  { fnStatSum,                   0,                           "n" STD_SIGMA,                                 "n",                                                                          SLS_ENABLED  },
/*  434 */  { fnCvtLbfN,                   divide,                      "N" STD_RIGHT_ARROW "lbf",                     "N" STD_RIGHT_ARROW "lbf",                                                    SLS_ENABLED  },
/*  435 */  { itemToBeCoded,               NOPARAM,                     "ODD?",                                        "ODD?",                                                                       SLS_UNCHANGED},
/*  436 */  { fnOff,                       NOPARAM,                     "OFF",                                         "OFF",                                                                        SLS_UNCHANGED},
/*  437 */  { itemToBeCoded,               NOPARAM,                     "OR",                                          "OR",                                                                         SLS_UNCHANGED},
/*  438 */  { itemToBeCoded,               NOPARAM,                     "OrthoF",                                      "OrthoF",                                                                     SLS_UNCHANGED},
/*  439 */  { itemToBeCoded,               NOPARAM,                     "ORTHOG",                                      "Orthog",                                                                     SLS_UNCHANGED},
/*  440 */  { fnCvtOzKg,                   multiply,                    "oz" STD_RIGHT_ARROW "kg",                     "oz" STD_RIGHT_ARROW "kg",                                                    SLS_ENABLED  },
/*  441 */  { fnConstant,                  39,                          "P" STD_SUB_0,                                 "P" STD_SUB_0,                                                                SLS_ENABLED  },
/*  442 */  { itemToBeCoded,               NOPARAM,                     "PAUSE",                                       "PAUSE",                                                                      SLS_UNCHANGED},
/*  443 */  { fnCvtAtmPa,                  divide,                      "Pa" STD_RIGHT_ARROW "atm",                    "Pa" STD_RIGHT_ARROW "atm",                                                   SLS_ENABLED  },
/*  444 */  { fnCvtBarPa,                  divide,                      "Pa" STD_RIGHT_ARROW "bar",                    "Pa" STD_RIGHT_ARROW "bar",                                                   SLS_ENABLED  },
/*  445 */  { fnCvtInhgPa,                 divide,                      "Pa" STD_RIGHT_ARROW "iHg",                    "Pa" STD_RIGHT_ARROW,                                                         SLS_ENABLED  },
/*  446 */  { fnCvtPsiPa,                  divide,                      "Pa" STD_RIGHT_ARROW "psi",                    "Pa" STD_RIGHT_ARROW "psi",                                                   SLS_ENABLED  },
/*  447 */  { fnCvtTorrPa,                 divide,                      "Pa" STD_RIGHT_ARROW "tor",                    "Pa " STD_RIGHT_ARROW,                                                        SLS_ENABLED  },
/*  448 */  { itemToBeCoded,               NOPARAM,                     "PARTS",                                       "PARTS",                                                                      SLS_UNCHANGED},
/*  449 */  { fnCvtPcM,                    multiply,                    "pc" STD_RIGHT_ARROW "m",                      "pc" STD_RIGHT_ARROW "m",                                                     SLS_ENABLED  },
/*  450 */  { itemToBeCoded,               NOPARAM,                     "PERM",                                        "P" STD_SUB_y STD_SUB_x,                                                      SLS_UNCHANGED},
/*  451 */  { itemToBeCoded,               NOPARAM,                     "PER/a",                                       "per/a",                                                                      SLS_UNCHANGED},
/*  452 */  { itemToBeCoded,               NOPARAM,                     "PGMINT",                                      "PGMINT",                                                                     SLS_UNCHANGED},
/*  453 */  { itemToBeCoded,               NOPARAM,                     "PGMSLV",                                      "PGMSLV",                                                                     SLS_UNCHANGED},
/*  454 */  { itemToBeCoded,               NOPARAM,                     "PIXEL",                                       "PIXEL",                                                                      SLS_UNCHANGED},
/*  455 */  { itemToBeCoded,               NOPARAM,                     "PLOT",                                        "PLOT",                                                                       SLS_UNCHANGED},
/*  456 */  { itemToBeCoded,               NOPARAM,                     "PMT",                                         "PMT",                                                                        SLS_UNCHANGED},
/*  457 */  { itemToBeCoded,               NOPARAM,                     "P" STD_SUB_n,                                 "P" STD_SUB_n,                                                                SLS_UNCHANGED},
/*  458 */  { itemToBeCoded,               NOPARAM,                     "POINT",                                       "POINT",                                                                      SLS_UNCHANGED},
/*  459 */  { itemToBeCoded,               NOPARAM,                     "Poiss",                                       "Poiss",                                                                      SLS_UNCHANGED},
/*  460 */  { itemToBeCoded,               NOPARAM,                     "Poiss" STD_SUB_p,                             "Poiss" STD_SUB_p,                                                            SLS_UNCHANGED},
/*  461 */  { itemToBeCoded,               NOPARAM,                     "Poiss" STD_SUB_e,                             "Poiss" STD_SUB_e,                                                            SLS_UNCHANGED},
/*  462 */  { itemToBeCoded,               NOPARAM,                     "Poiss" STD_SUP_MINUS_1,                       "Poiss" STD_SUP_MINUS_1,                                                      SLS_UNCHANGED},
/*  463 */  { itemToBeCoded,               NOPARAM,                     "Poiss:",                                      "Poiss:",                                                                     SLS_UNCHANGED},
/*  464 */  { fnComplexMode,               CM_POLAR,                    "POLAR",                                       "POLAR",                                                                      SLS_UNCHANGED},
/*  465 */  { itemToBeCoded,               NOPARAM,                     "PopLR",                                       "PopLR",                                                                      SLS_UNCHANGED},
/*  466 */  { fnCurveFitting,              CF_POWER_FITTING,            "PowerF",                                      "PowerF",                                                                     SLS_UNCHANGED},
/*  467 */  { fnCvtRatioDb,                10,                          "pr" STD_RIGHT_ARROW "dB",                     "power",                                                                      SLS_ENABLED  },
/*  468 */  { itemToBeCoded,               NOPARAM,                     "PRCL",                                        "PRCL",                                                                       SLS_UNCHANGED},
/*  469 */  { fnIsPrime,                   NOPARAM,                     "PRIME?",                                      "PRIME?",                                                                     SLS_UNCHANGED},
/*  470 */  { itemToBeCoded,               NOPARAM,                     "PROB",                                        "PROB",                                                                       SLS_UNCHANGED},
/*  471 */  { fnFractionType,              FT_PROPER,                   "PROFRC",                                      "a" STD_SPACE_3_PER_EM "b/c",                                                 SLS_UNCHANGED},
/*  472 */  { itemToBeCoded,               NOPARAM,                     "PROGS",                                       "PROGS",                                                                      SLS_UNCHANGED},
/*  473 */  { fnCvtPsiPa,                  multiply,                    "psi" STD_RIGHT_ARROW "Pa",                    "psi" STD_RIGHT_ARROW "Pa",                                                   SLS_ENABLED  },
/*  474 */  { itemToBeCoded,               NOPARAM,                     "PSTO",                                        "PSTO",                                                                       SLS_UNCHANGED},
/*  475 */  { fnCvtPointM,                 multiply,                    "pt." STD_RIGHT_ARROW "m",                     "point",                                                                      SLS_ENABLED  },
/*  476 */  { itemToBeCoded,               NOPARAM,                     "PUTK",                                        "PUTK",                                                                       SLS_UNCHANGED},
/*  477 */  { itemToBeCoded,               NOPARAM,                     "PV",                                          "PV",                                                                         SLS_UNCHANGED},
/*  478 */  { itemToBeCoded,               NOPARAM,                     "P.FN",                                        "P.FN",                                                                       SLS_UNCHANGED},
/*  479 */  { itemToBeCoded,               NOPARAM,                     "P.FN2",                                       "P.FN2",                                                                      SLS_UNCHANGED},
/*  480 */  { itemToBeCoded,               NOPARAM,                     "P:",                                          "P:",                                                                         SLS_UNCHANGED},
/*  481 */  { itemToBeCoded,               NOPARAM,                     "QUIET",                                       "QUIET",                                                                      SLS_UNCHANGED},
/*  482 */  { fnConstant,                  40,                          "R",                                           "R",                                                                          SLS_ENABLED  },
/*  483 */  { fnAngularMode,               AM_RADIAN,                   "RAD",                                         "RAD",                                                                        SLS_UNCHANGED},
/*  484 */  { fnCvtToCurrentAngularMode,   AM_RADIAN,                   "RAD" STD_RIGHT_ARROW,                         "RAD" STD_RIGHT_ARROW,                                                        SLS_ENABLED  },
/*  485 */  { itemToBeCoded,               NOPARAM,                     "RAM",                                         "RAM",                                                                        SLS_UNCHANGED},
/*  486 */  { itemToBeCoded,               NOPARAM,                     "RAN#",                                        "RAN#",                                                                       SLS_UNCHANGED},
/*  487 */  { registerBrowser,             NOPARAM,                     "RBR",                                         "REGS.V",                                                                         SLS_UNCHANGED},   //Changed RBR to REGS
/*  488 */  { fnRecall,                    TM_STORCL,                   "RCL",                                         "RCL",                                                                        SLS_ENABLED  },
/*  489 */  { fnRecallConfig,              NOPARAM,                     "RCLCFG",                                      "Config",                                                                     SLS_UNCHANGED},
/*  490 */  { fnRecallElement,             NOPARAM,                     "RCLEL",                                       "RCLEL",                                                                      SLS_ENABLED  },
/*  491 */  { fnRecallIJ,                  NOPARAM,                     "RCLIJ",                                       "RCLIJ",                                                                      SLS_ENABLED  },
/*  492 */  { fnRecallStack,               NOPARAM,                     "RCLS",                                        "Stack",                                                                      SLS_ENABLED  },
/*  493 */  { fnRecallAdd,                 NOPARAM,                     "RCL+",                                        "RCL+",                                                                       SLS_ENABLED  },
/*  494 */  { fnRecallSub,                 NOPARAM,                     "RCL-",                                        "RCL-",                                                                       SLS_ENABLED  },
/*  495 */  { fnRecallMult,                NOPARAM,                     "RCL" STD_CROSS,                               "RCL" STD_CROSS,                                                              SLS_ENABLED  },
/*  496 */  { fnRecallDiv,                 NOPARAM,                     "RCL/",                                        "RCL/",                                                                       SLS_ENABLED  },
/*  497 */  { fnRecallMax,                 NOPARAM,                     "RCL" STD_UP_ARROW,                            "Max",                                                                        SLS_ENABLED  },
/*  498 */  { fnRecallMin,                 NOPARAM,                     "RCL" STD_DOWN_ARROW,                          "Min",                                                                        SLS_ENABLED  },
/*  499 */  { itemToBeCoded,               NOPARAM,                     "RDP",                                         "RDP",                                                                        SLS_UNCHANGED},
/*  500 */  { fnRadixMark,                 RM_COMMA,                    "RDX,",                                        "RDX,",                                                                       SLS_UNCHANGED},
/*  501 */  { fnRadixMark,                 RM_PERIOD,                   "RDX.",                                        "RDX.",                                                                       SLS_UNCHANGED},
/*  502 */  { fnConstant,                  41,                          "r" STD_SUB_e,                                 "r" STD_SUB_e,                                                                SLS_ENABLED  },
/*  503 */  { fnRealPart,                  NOPARAM,                     "RE",                                          "Re",                                                                         SLS_ENABLED  },
/*  504 */  { fnComplexResult,             false,                       "REALRES",                                     "REALRE",                                                                     SLS_UNCHANGED},
/*  505 */  { itemToBeCoded,               NOPARAM,                     "REALS",                                       "REALS",                                                                      SLS_UNCHANGED},
/*  506 */  { itemToBeCoded,               NOPARAM,                     "REAL?",                                       "REAL?",                                                                      SLS_UNCHANGED},
/*  507 */  { fnComplexMode,               CM_RECTANGULAR,              "RECT",                                        "RECT",                                                                       SLS_UNCHANGED},
/*  508 */  { itemToBeCoded,               NOPARAM,                     "REGS",                                        "REGS",                                                                       SLS_UNCHANGED},
/*  509 */  { itemToBeCoded,               NOPARAM,                     "RECV",                                        "RECV",                                                                       SLS_UNCHANGED},
/*  510 */  { fnReset,                     NOT_CONFIRMED,               "RESET",                                       "RESET",                                                                      SLS_UNCHANGED},
/*  511 */  { itemToBeCoded,               NOPARAM,                     "RE" STD_RIGHT_ARROW "CX",                     "CC",                                                                         SLS_UNCHANGED},
/*  512 */  { fnSwapRealImaginary,         NOPARAM,                     "Re" STD_LEFT_RIGHT_ARROWS "Im",               "Re" STD_LEFT_RIGHT_ARROWS "Im",                                              SLS_ENABLED  },
/*  513 */  { itemToBeCoded,               NOPARAM,                     "RJ",                                          "RJ",                                                                         SLS_UNCHANGED},
/*  514 */  { fnConstant,                  42,                          "R" STD_SUB_K,                                 "R" STD_SUB_K,                                                                SLS_ENABLED  },
/*  515 */  { itemToBeCoded,               NOPARAM,                     "RL",                                          "RL",                                                                         SLS_UNCHANGED},
/*  516 */  { itemToBeCoded,               NOPARAM,                     "RLC",                                         "RLC",                                                                        SLS_UNCHANGED},
/*  517 */  { fnConstant,                  43,                          "R" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "R" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,                                  SLS_ENABLED  },
/*  518 */  { itemToBeCoded,               NOPARAM,                     "RM",                                          "RM",                                                                         SLS_UNCHANGED},
/*  519 */  { fnGetRoundingMode,           NOPARAM,                     "RM?",                                         "RM?",                                                                        SLS_ENABLED  },
/*  520 */  { itemToBeCoded,               NOPARAM,                     "RMDR",                                        "RMDR",                                                                       SLS_UNCHANGED},
/*  521 */  { itemToBeCoded,               NOPARAM,                     "RNORM",                                       "RNORM",                                                                      SLS_UNCHANGED},
/*  522 */  { itemToBeCoded,               NOPARAM,                     "ROUND",                                       "ROUND",                                                                      SLS_UNCHANGED},
/*  523 */  { itemToBeCoded,               NOPARAM,                     "ROUNDI",                                      "ROUNDI",                                                                     SLS_UNCHANGED},
/*  524 */  { itemToBeCoded,               NOPARAM,                     "RR",                                          "RR",                                                                         SLS_UNCHANGED},
/*  525 */  { itemToBeCoded,               NOPARAM,                     "RRC",                                         "RRC",                                                                        SLS_UNCHANGED},
/*  526 */  { itemToBeCoded,               NOPARAM,                     "RSD",                                         "RSD",                                                                        SLS_UNCHANGED},
/*  527 */  { itemToBeCoded,               NOPARAM,                     "RSUM",                                        "RSUM",                                                                       SLS_UNCHANGED},
/*  528 */  { itemToBeCoded,               NOPARAM,                     "RTN",                                         "RTN",                                                                        SLS_UNCHANGED},
/*  529 */  { itemToBeCoded,               NOPARAM,                     "RTN+1",                                       "RTN+1",                                                                      SLS_UNCHANGED},
/*  530 */  { itemToBeCoded,               NOPARAM,                     "R-CLR",                                       "R-CLR",                                                                      SLS_UNCHANGED},
/*  531 */  { itemToBeCoded,               NOPARAM,                     "R-COPY",                                      "R-COPY",                                                                     SLS_UNCHANGED},
/*  532 */  { itemToBeCoded,               NOPARAM,                     "R-SORT",                                      "R-SORT",                                                                     SLS_UNCHANGED},
/*  533 */  { itemToBeCoded,               NOPARAM,                     "R-SWAP",                                      "R-SWAP",                                                                     SLS_UNCHANGED},
/*  534 */  { fnCvtRadToDeg,               NOPARAM,                     "R" STD_RIGHT_ARROW "D",                       "R" STD_RIGHT_ARROW "D",                                                      SLS_ENABLED  },
/*  535 */  { fnRollUp,                    NOPARAM,                     "R" STD_UP_ARROW,                              "R" STD_UP_ARROW,                                                             SLS_UNCHANGED},
/*  536 */  { fnRollDown,                  NOPARAM,                     "R" STD_DOWN_ARROW,                            "R" STD_DOWN_ARROW,                                                           SLS_UNCHANGED},
/*  537 */  { fnConstant,                  44,                          "R" STD_SUB_INFINITY,                          "R" STD_SUB_INFINITY,                                                         SLS_ENABLED  },
/*  538 */  { fnConstant,                  45,                          "R" STD_SUB_SUN,                               "R" STD_SUB_SUN,                                                              SLS_ENABLED  },
/*  539 */  { fnConstant,                  46,                          "R" STD_SUB_EARTH,                             "R" STD_SUB_EARTH,                                                            SLS_ENABLED  },
/*  540 */  { itemToBeCoded,               NOPARAM,                     "s",                                           "s",                                                                          SLS_UNCHANGED},
/*  541 */  { fnConstant,                  47,                          "Sa",                                          "Sa",                                                                         SLS_ENABLED  },
/*  542 */  { itemToBeCoded,               NOPARAM,                     "SAVE",                                        "SAVE",                                                                       SLS_UNCHANGED},
/*  543 */  { itemToBeCoded,               NOPARAM,                     "SB",                                          "SB",                                                                         SLS_UNCHANGED},
/*  544 */  { fnConstant,                  48,                          "Sb",                                          "Sb",                                                                         SLS_ENABLED  },
/*  545 */  { fnDisplayFormatSci,          TM_VALUE,                    "SCI",                                         "SCI",                                                                        SLS_UNCHANGED},
/*  546 */  { fnCvtShortcwtKg,             multiply,                    "scw" STD_RIGHT_ARROW "kg",                    "short",                                                                      SLS_ENABLED  },
/*  547 */  { fnDisplayOvr,                DO_SCI,                      "SCIOVR",                                      "SCIOVR",                                                                     SLS_UNCHANGED},
/*  548 */  { fnGetSignificantDigits,      NOPARAM,                     "SDIGS?",                                      "SDIGS?",                                                                     SLS_ENABLED  },
/*  549 */  { itemToBeCoded,               NOPARAM,                     "SDL",                                         "SDL",                                                                        SLS_UNCHANGED},
/*  550 */  { itemToBeCoded,               NOPARAM,                     "SDR",                                         "SDR",                                                                        SLS_UNCHANGED},
/*  551 */  { fnConstant,                  49,                          "Se" STD_SUP_2,                                "Se" STD_SUP_2,                                                               SLS_ENABLED  },
/*  552 */  { itemToBeCoded,               NOPARAM,                     "SEED",                                        "SEED",                                                                       SLS_UNCHANGED},
/*  553 */  { itemToBeCoded,               NOPARAM,                     "SEND",                                        "SEND",                                                                       SLS_UNCHANGED},
/*  554 */  { fnConfigChina,               NOPARAM,                     "SETCHN",                                      "CHINA",                                                                      SLS_UNCHANGED},
/*  555 */  { itemToBeCoded,               NOPARAM,                     "SETDAT",                                      "SETDAT",                                                                     SLS_UNCHANGED},
/*  556 */  { fnConfigEurope,              NOPARAM,                     "SETEUR",                                      "EUROPE",                                                                     SLS_UNCHANGED},
/*  557 */  { fnConfigIndia,               NOPARAM,                     "SETIND",                                      "INDIA",                                                                      SLS_UNCHANGED},
/*  558 */  { fnConfigJapan,               NOPARAM,                     "SETJPN",                                      "JAPAN",                                                                      SLS_UNCHANGED},
/*  559 */  { itemToBeCoded,               NOPARAM,                     "SETSIG",                                      "SETSIG",                                                                     SLS_UNCHANGED},
/*  560 */  { itemToBeCoded,               NOPARAM,                     "SETTIM",                                      "SETTIM",                                                                     SLS_UNCHANGED},
/*  561 */  { fnConfigUk,                  NOPARAM,                     "SETUK",                                       "UK",                                                                         SLS_UNCHANGED},
/*  562 */  { fnConfigUsa,                 NOPARAM,                     "SETUSA",                                      "USA",                                                                        SLS_UNCHANGED},
/*  563 */  { fnConstant,                  50,                          "Se'" STD_SUP_2,                               "Se'" STD_SUP_2,                                                              SLS_ENABLED  },
/*  564 */  { fnSetFlag,                   TM_FLAG,                     "SF",                                          "SF",                                                                         SLS_UNCHANGED},
/*  565 */  { fnConstant,                  51,                          "Sf" STD_SUP_MINUS_1,                          "Sf" STD_SUP_MINUS_1,                                                         SLS_ENABLED  },
/*  566 */  { fnSign,                      NOPARAM,                     "SIGN",                                        "sign",                                                                       SLS_ENABLED  },
/*  567 */  { fnIntegerMode,               SIM_SIGNMT,                  "SIGNMT",                                      "SIGNMT",                                                                     SLS_UNCHANGED},
/*  568 */  { itemToBeCoded,               NOPARAM,                     "SIM_EQ",                                      "SIM EQ",                                                                     SLS_UNCHANGED},
/*  569 */  { fnSin,                       NOPARAM,                     "sin",                                         "SIN",                                                                        SLS_ENABLED  },    //JM3
/*  570 */  { itemToBeCoded,               NOPARAM,                     "sinc",                                        "sinc",                                                                       SLS_ENABLED  },
/*  571 */  { fnSinh,                      NOPARAM,                     "sinh",                                        "sinh",                                                                       SLS_ENABLED  },
/*  572 */  { itemToBeCoded,               NOPARAM,                     "SKIP",                                        "SKIP",                                                                       SLS_UNCHANGED},
/*  573 */  { itemToBeCoded,               NOPARAM,                     "SL",                                          "SL",                                                                         SLS_UNCHANGED},
/*  574 */  { itemToBeCoded,               NOPARAM,                     "SLOW",                                        "SLOW",                                                                       SLS_UNCHANGED},
/*  575 */  { itemToBeCoded,               NOPARAM,                     "SLVQ",                                        "SLVQ",                                                                       SLS_UNCHANGED},
/*  576 */  { itemToBeCoded,               NOPARAM,                     "s" STD_SUB_m,                                 "s" STD_SUB_m,                                                                SLS_UNCHANGED},
/*  577 */  { fnGetIntegerSignMode,        NOPARAM,                     "SMODE?",                                      "SMODE?",                                                                     SLS_ENABLED  },
/*  578 */  { itemToBeCoded,               NOPARAM,                     "s" STD_SUB_m STD_SUB_w,                       "s" STD_SUB_m STD_SUB_w,                                                      SLS_UNCHANGED},
/*  579 */  { itemToBeCoded,               NOPARAM,                     "SOLVE",                                       "SOLVE",                                                                      SLS_UNCHANGED},
/*  580 */  { itemToBeCoded,               NOPARAM,                     "Solver",                                      "Solver",                                                                     SLS_UNCHANGED},
/*  581 */  { itemToBeCoded,               NOPARAM,                     "SPEC?",                                       "SPEC?",                                                                      SLS_UNCHANGED},
/*  582 */  { itemToBeCoded,               NOPARAM,                     "SR",                                          "SR",                                                                         SLS_UNCHANGED},
/*  583 */  { fnStackSize,                 SS_4,                        "SSIZE4",                                      "SSIZE4",                                                                     SLS_UNCHANGED},
/*  584 */  { fnStackSize,                 SS_8,                        "SSIZE8",                                      "SSIZE8",                                                                     SLS_UNCHANGED},
/*  585 */  { fnGetStackSize,              NOPARAM,                     "SSIZE?",                                      "SSIZE?",                                                                     SLS_ENABLED  },
/*  586 */  { itemToBeCoded,               NOPARAM,                     "STAT",                                        "STAT",                                                                       SLS_UNCHANGED},
/*  587 */  { flagBrowser,                 NOPARAM,                     "STATUS",                                      "FLAGS.V",                                                                     SLS_UNCHANGED},         //JM Changed STATUS
/*  588 */  { itemToBeCoded,               NOPARAM,                     "STK",                                         "STK",                                                                        SLS_UNCHANGED},
/*  589 */  { fnStore,                     TM_STORCL,                   "STO",                                         "STO",                                                                        SLS_UNCHANGED},
/*  590 */  { fnStoreConfig,               NOPARAM,                     "STOCFG",                                      "Config",                                                                     SLS_UNCHANGED},
/*  591 */  { fnStoreElement,              NOPARAM,                     "STOEL",                                       "STOEL",                                                                      SLS_UNCHANGED},
/*  592 */  { fnStoreIJ,                   NOPARAM,                     "STOIJ",                                       "STOIJ",                                                                      SLS_UNCHANGED},
/*  593 */  { itemToBeCoded,               NOPARAM,                     "STOP",                                        "R/S",                                                                        SLS_UNCHANGED},
/*  594 */  { fnStoreStack,                NOPARAM,                     "STOS",                                        "Stack",                                                                      SLS_UNCHANGED},
/*  595 */  { fnStoreAdd,                  NOPARAM,                     "STO+",                                        "STO+",                                                                       SLS_UNCHANGED},
/*  596 */  { fnStoreSub,                  NOPARAM,                     "STO-",                                        "STO-",                                                                       SLS_UNCHANGED},
/*  597 */  { fnStoreMult,                 NOPARAM,                     "STO" STD_CROSS,                               "STO" STD_CROSS,                                                              SLS_UNCHANGED},
/*  598 */  { fnStoreDiv,                  NOPARAM,                     "STO/",                                        "STO/",                                                                       SLS_UNCHANGED},
/*  599 */  { fnStoreMax,                  NOPARAM,                     "STO" STD_UP_ARROW,                            "Max",                                                                        SLS_UNCHANGED},
/*  600 */  { fnStoreMin,                  NOPARAM,                     "STO" STD_DOWN_ARROW,                          "Min",                                                                        SLS_UNCHANGED},
/*  601 */  { fnCvtStoneKg,                multiply,                    "sto" STD_RIGHT_ARROW "kg",                    "stone",                                                                      SLS_ENABLED  },
/*  602 */  { itemToBeCoded,               NOPARAM,                     "STRI?",                                       "STRI?",                                                                      SLS_UNCHANGED},
/*  603 */  { itemToBeCoded,               NOPARAM,                     "STRING",                                      "STRING",                                                                     SLS_UNCHANGED},
/*  604 */  { addItemToBuffer,             REGISTER_A,                  "ST.A",                                        "ST.A",                                                                       SLS_UNCHANGED},
/*  605 */  { addItemToBuffer,             REGISTER_B,                  "ST.B",                                        "ST.B",                                                                       SLS_UNCHANGED},
/*  606 */  { addItemToBuffer,             REGISTER_C,                  "ST.C",                                        "ST.C",                                                                       SLS_UNCHANGED},
/*  607 */  { addItemToBuffer,             REGISTER_D,                  "ST.D",                                        "ST.D",                                                                       SLS_UNCHANGED},
/*  608 */  { addItemToBuffer,             REGISTER_T,                  "ST.T",                                        "ST.T",                                                                       SLS_UNCHANGED},
/*  609 */  { addItemToBuffer,             REGISTER_X,                  "ST.X",                                        "ST.X",                                                                       SLS_UNCHANGED},
/*  610 */  { addItemToBuffer,             REGISTER_Y,                  "ST.Y",                                        "ST.Y",                                                                       SLS_UNCHANGED},
/*  611 */  { addItemToBuffer,             REGISTER_Z,                  "ST.Z",                                        "ST.Z",                                                                       SLS_UNCHANGED},
/*  612 */  { itemToBeCoded,               NOPARAM,                     "SUM",                                         "SUM",                                                                        SLS_UNCHANGED},
/*  613 */  { itemToBeCoded,               NOPARAM,                     "s" STD_SUB_w,                                 "s" STD_SUB_w,                                                                SLS_UNCHANGED},
/*  614 */  { itemToBeCoded,               NOPARAM,                     "s" STD_SUB_x STD_SUB_y,                       "s" STD_SUB_x STD_SUB_y,                                                      SLS_UNCHANGED},
/*  615 */  { fnCvtShorttonKg,             multiply,                    "s.t" STD_RIGHT_ARROW "kg",                    "short",                                                                      SLS_ENABLED  },
/*  616 */  { fnCvtYearS,                  divide,                      "s" STD_RIGHT_ARROW "year",                    "s" STD_RIGHT_ARROW "year",                                                   SLS_ENABLED  },
/*  617 */  { fnConstant,                  52,                          "T" STD_SUB_0,                                 "T" STD_SUB_0,                                                                SLS_ENABLED  },
/*  618 */  { fnTan,                       NOPARAM,                     "tan",                                         "TAN",                                                                        SLS_ENABLED  },     //JM3
/*  619 */  { fnTanh,                      NOPARAM,                     "tanh",                                        "tanh",                                                                       SLS_ENABLED  },
/*  620 */  { itemToBeCoded,               NOPARAM,                     "TDISP",                                       "TDISP",                                                                      SLS_UNCHANGED},
/*  621 */  { itemToBeCoded,               NOPARAM,                     "TEST",                                        "TEST",                                                                       SLS_UNCHANGED},
/*  622 */  { itemToBeCoded,               NOPARAM,                     "TICKS",                                       "TICKS",                                                                      SLS_UNCHANGED},
/*  623 */  { itemToBeCoded,               NOPARAM,                     "TIME",                                        "TIME",                                                                       SLS_UNCHANGED},
/*  624 */  { itemToBeCoded,               NOPARAM,                     "TIMER",                                       "TIMER",                                                                      SLS_UNCHANGED},
/*  625 */  { itemToBeCoded,               NOPARAM,                     "TIMES",                                       "TIMES",                                                                      SLS_UNCHANGED},
/*  626 */  { itemToBeCoded,               NOPARAM,                     "T" STD_SUB_n,                                 "T" STD_SUB_n,                                                                SLS_UNCHANGED},
/*  627 */  { itemToBeCoded,               NOPARAM,                     "TONE",                                        "TONE",                                                                       SLS_UNCHANGED},
/*  628 */  { fnCvtTonKg,                  multiply,                    "ton" STD_RIGHT_ARROW "kg",                    "ton" STD_RIGHT_ARROW "kg",                                                   SLS_ENABLED  },
/*  629 */  { itemToBeCoded,               NOPARAM,                     "TOP?",                                        "TOP?",                                                                       SLS_UNCHANGED},
/*  630 */  { fnCvtTorrPa,                 multiply,                    "tor" STD_RIGHT_ARROW "Pa",                    "torr",                                                                       SLS_ENABLED  },
/*  631 */  { fnConstant,                  53,                          "T" STD_SUB_P,                                 "T" STD_SUB_P,                                                                SLS_ENABLED  },
/*  632 */  { fnConstant,                  54,                          "t" STD_SUB_P STD_SUB_L,                       "t" STD_SUB_P STD_SUB_L,                                                      SLS_ENABLED  },
/*  633 */  { itemToBeCoded,               NOPARAM,                     "t" STD_SUB_p "(x)",                           "t" STD_SUB_p "(x)",                                                          SLS_UNCHANGED},
/*  634 */  { itemToBeCoded,               NOPARAM,                     "t" STD_SUB_e "(x)",                           "t" STD_SUB_e "(x)",                                                          SLS_UNCHANGED},
/*  635 */  { itemToBeCoded,               NOPARAM,                     "t(x)",                                        "t(x)",                                                                       SLS_UNCHANGED},
/*  636 */  { itemToBeCoded,               NOPARAM,                     "t" STD_SUP_MINUS_1 "(p)",                     "t" STD_SUP_MINUS_1 "(p)",                                                    SLS_UNCHANGED},
/*  637 */  { itemToBeCoded,               NOPARAM,                     "TRI",                                         "TRIG",                                                                        SLS_UNCHANGED},
/*  638 */  { fnCvtTrozKg,                 multiply,                    "trz" STD_RIGHT_ARROW "kg",                    "tr.oz",                                                                      SLS_ENABLED  },
/*  639 */  { itemToBeCoded,               NOPARAM,                     "TVM",                                         "TVM",                                                                        SLS_UNCHANGED},
/*  640 */  { itemToBeCoded,               NOPARAM,                     "t:",                                          "t:",                                                                         SLS_UNCHANGED},
/*  641 */  { itemToBeCoded,               NOPARAM,                     "t" STD_LEFT_RIGHT_ARROWS,                     "t" STD_LEFT_RIGHT_ARROWS,                                                    SLS_UNCHANGED},
/*  642 */  { itemToBeCoded,               NOPARAM,                     "ULP?",                                        "ULP?",                                                                       SLS_UNCHANGED},
/*  643 */  { itemToBeCoded,               NOPARAM,                     "U" STD_SUB_n,                                 "U" STD_SUB_n,                                                                SLS_UNCHANGED},
/*  644 */  { fnUnitVector,                NOPARAM,                     "UNITV",                                       "UNITV",                                                                      SLS_ENABLED  },
/*  645 */  { fnIntegerMode,               SIM_UNSIGN,                  "UNSIGN",                                      "UNSIGN",                                                                     SLS_UNCHANGED},
/* JM  646 */  { itemToBeCoded,               NOPARAM,                     "UNIT",                                        "UNIT",                                                          SLS_UNCHANGED},   //JM Change U> arrow to CONV. Changed again to UNIT
/*  647 */  { itemToBeCoded,               NOPARAM,                     "VARMNU",                                      "VARMNU",                                                                     SLS_UNCHANGED},
/*  648 */  { itemToBeCoded,               NOPARAM,                     "VARS",                                        "VARS",                                                                       SLS_UNCHANGED},
/*  649 */  { fnVersion,                   NOPARAM,                     "VERS?",                                       "VERS?",                                                                      SLS_UNCHANGED},
/*  650 */  { itemToBeCoded,               NOPARAM,                     "VIEW",                                        "VIEW",                                                                       SLS_UNCHANGED},
/*  651 */  { fnConstant,                  55,                          "V" STD_SUB_m,                                 "V" STD_SUB_m,                                                                SLS_ENABLED  },
/*  652 */  { itemToBeCoded,               NOPARAM,                     "V:",                                          "V:",                                                                         SLS_UNCHANGED},
/*  653 */  { itemToBeCoded,               NOPARAM,                     "WDAY",                                        "WDAY",                                                                       SLS_UNCHANGED},
/*  654 */  { itemToBeCoded,               NOPARAM,                     "Weibl",                                       "Weibl",                                                                      SLS_UNCHANGED},
/*  655 */  { itemToBeCoded,               NOPARAM,                     "Weibl" STD_SUB_p,                             "Weibl" STD_SUB_p,                                                            SLS_UNCHANGED},
/*  656 */  { itemToBeCoded,               NOPARAM,                     "Weibl" STD_SUB_e,                             "Weibl" STD_SUB_e,                                                            SLS_UNCHANGED},
/*  657 */  { itemToBeCoded,               NOPARAM,                     "Weibl" STD_SUP_MINUS_1,                       "Weibl" STD_SUP_MINUS_1,                                                      SLS_UNCHANGED},
/*  658 */  { itemToBeCoded,               NOPARAM,                     "Weibl:",                                      "Weibl:",                                                                     SLS_UNCHANGED},
/*  659 */  { fnWho,                       NOPARAM,                     "WHO?",                                        "WHO?",                                                                       SLS_UNCHANGED},
/*  660 */  { fnCvtWhJ,                    multiply,                    "Wh" STD_RIGHT_ARROW "J",                      "Wh" STD_RIGHT_ARROW "J",                                                     SLS_ENABLED  },
/*  661 */  { itemToBeCoded,               NOPARAM,                     "W" STD_SUB_m,                                 "W" STD_SUB_m,                                                                SLS_UNCHANGED},
/*  662 */  { itemToBeCoded,               NOPARAM,                     "W" STD_SUB_p,                                 "W" STD_SUB_p,                                                                SLS_UNCHANGED},
/*  663 */  { itemToBeCoded,               NOPARAM,                     "W" STD_SUP_MINUS_1,                           "W" STD_SUP_MINUS_1,                                                          SLS_UNCHANGED},
/*  664 */  { fnSetWordSize,               TM_VALUE,                    "WSIZE",                                       "WSIZE",                                                                      SLS_UNCHANGED},
/*  665 */  { fnGetWordSize,               NOPARAM,                     "WSIZE?",                                      "WSIZE?",                                                                     SLS_ENABLED  },
/*  666 */  { fnCvtHpeW,                   divide,                      "W" STD_RIGHT_ARROW "hp" STD_SUB_E,            "W" STD_RIGHT_ARROW "hp" STD_SUB_E,                                           SLS_ENABLED  },
/*  667 */  { fnCvtHpmW,                   divide,                      "W" STD_RIGHT_ARROW "hp" STD_SUB_M,            "W" STD_RIGHT_ARROW "hp" STD_SUB_M,                                           SLS_ENABLED  },
/*  668 */  { fnCvtHpukW,                  divide,                      "W" STD_RIGHT_ARROW "hp" STD_UK,               "W" STD_RIGHT_ARROW "hp" STD_UK,                                              SLS_ENABLED  },
/*  669 */  { fnSquare,                    NOPARAM,                     "x" STD_SUP_2,                                 "x" STD_SUP_2,                                                                SLS_ENABLED  },
/*  670 */  { fnCube,                      NOPARAM,                     "x" STD_SUP_3,                                 "x" STD_SUP_3,                                                                SLS_ENABLED  },
/*  671 */  { itemToBeCoded,               NOPARAM,                     "XEQ",                                         "XEQ",                                                                        SLS_UNCHANGED},
/*  672 */  { itemToBeCoded,               NOPARAM,                     "XNOR",                                        "XNOR",                                                                       SLS_UNCHANGED},
/*  673 */  { itemToBeCoded,               NOPARAM,                     "XOR",                                         "XOR",                                                                        SLS_UNCHANGED},
/*  674 */  { itemToBeCoded,               NOPARAM,                     STD_x_BAR,                                     STD_x_BAR,                                                                    SLS_UNCHANGED},
/*  675 */  { itemToBeCoded,               NOPARAM,                     STD_x_BAR STD_SUB_G,                           STD_x_BAR STD_SUB_G,                                                          SLS_UNCHANGED},
/*  676 */  { itemToBeCoded,               NOPARAM,                     STD_x_BAR STD_SUB_w,                           STD_x_BAR STD_SUB_w,                                                          SLS_UNCHANGED},
/*  677 */  { itemToBeCoded,               NOPARAM,                     STD_x_CIRC,                                    STD_x_CIRC,                                                                   SLS_UNCHANGED},
/*  678 */  { itemToBeCoded,               NOPARAM,                     "X.FN",                                        "X.FN",                                                                       SLS_UNCHANGED},
/*  679 */  { fnFactorial,                 NOPARAM,                     "x!",                                          "x!",                                                                         SLS_ENABLED  },
/*  680 */  { itemToBeCoded,               NOPARAM,                     "x:",                                          "x:",                                                                         SLS_UNCHANGED},
/*  681 */  { itemToBeCoded,               NOPARAM,                     "x" STD_RIGHT_ARROW "DATE",                    "x" STD_RIGHT_ARROW "DATE",                                                   SLS_UNCHANGED},
/*  682 */  { itemToBeCoded,               NOPARAM,                     "x" STD_RIGHT_ARROW STD_alpha,                 "x" STD_RIGHT_ARROW STD_alpha,                                                SLS_UNCHANGED},
/*  683 */  { itemToBeCoded,               NOPARAM,                     "x" STD_LEFT_RIGHT_ARROWS,                     "x" STD_LEFT_RIGHT_ARROWS,                                                    SLS_UNCHANGED},
/*  684 */  { fnSwapXY,                    NOPARAM,                     "x" STD_LEFT_RIGHT_ARROWS "y",                 "x" STD_LEFT_RIGHT_ARROWS "y",                                                SLS_ENABLED  },
/*  685 */  { itemToBeCoded,               NOPARAM,                     "x= ?",                                        "x= ?",                                                                       SLS_UNCHANGED},
/*  686 */  { itemToBeCoded,               NOPARAM,                     "x" STD_NOT_EQUAL " ?",                        "x" STD_NOT_EQUAL " ?",                                                       SLS_UNCHANGED},
/*  687 */  { itemToBeCoded,               NOPARAM,                     "x=+0?",                                       "x=+0?",                                                                      SLS_UNCHANGED},
/*  688 */  { itemToBeCoded,               NOPARAM,                     "x=-0?",                                       "x=-0?",                                                                      SLS_UNCHANGED},
/*  689 */  { itemToBeCoded,               NOPARAM,                     "x" STD_ALMOST_EQUAL " ?",                     "x" STD_ALMOST_EQUAL " ?",                                                    SLS_UNCHANGED},
/*  690 */  { fnXLessThan,                 NOPARAM,                     "x< ?",                                        "x< ?",                                                                       SLS_UNCHANGED},
/*  691 */  { itemToBeCoded,               NOPARAM,                     "x" STD_LESS_EQUAL " ?",                       "x" STD_LESS_EQUAL " ?",                                                      SLS_UNCHANGED},
/*  692 */  { itemToBeCoded,               NOPARAM,                     "x" STD_GREATER_EQUAL " ?",                    "x" STD_GREATER_EQUAL " ?",                                                   SLS_UNCHANGED},
/*  693 */  { itemToBeCoded,               NOPARAM,                     "x> ?",                                        "x> ?",                                                                       SLS_UNCHANGED},
/*  694 */  { itemToBeCoded,               NOPARAM,                     STD_xTH_ROOT STD_y_UNDER_ROOT,                 STD_xTH_ROOT STD_y_UNDER_ROOT,                                                SLS_ENABLED  },
/*  695 */  { fnCvtYardM,                  multiply,                    "yd." STD_RIGHT_ARROW "m",                     "yd." STD_RIGHT_ARROW "m",                                                    SLS_ENABLED  },
/*  696 */  { itemToBeCoded,               NOPARAM,                     "YEAR",                                        "YEAR",                                                                       SLS_UNCHANGED},
/*  697 */  { fnCvtYearS,                  multiply,                    "year" STD_RIGHT_ARROW "s",                    "year" STD_RIGHT_ARROW "s",                                                   SLS_ENABLED  },
/*  698 */  { fnPower,                     NOPARAM,                     "y" STD_SUP_x,                                 "y" STD_SUP_x,                                                                SLS_ENABLED  },
/*  699 */  { itemToBeCoded,               NOPARAM,                     STD_y_CIRC,                                    STD_y_CIRC,                                                                   SLS_UNCHANGED},
/*  700 */  { fnDateFormat,                DF_YMD,                      "Y.MD",                                        "Y.MD",                                                                       SLS_UNCHANGED},
/*  701 */  { itemToBeCoded,               NOPARAM,                     "y" STD_LEFT_RIGHT_ARROWS,                     "y" STD_LEFT_RIGHT_ARROWS,                                                    SLS_UNCHANGED},
/*  702 */  { fnConstant,                  56,                          "Z" STD_SUB_0,                                 "Z" STD_SUB_0,                                                                SLS_ENABLED  },
/*  703 */  { itemToBeCoded,               NOPARAM,                     "z" STD_LEFT_RIGHT_ARROWS,                     "z" STD_LEFT_RIGHT_ARROWS,                                                    SLS_UNCHANGED},
/*  704 */  { fnConstant,                  57,                          STD_alpha,                                     STD_alpha,                                                                    SLS_ENABLED  },
/*  705 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "INTL",                              STD_alpha "INTL",                                                             SLS_UNCHANGED},
/*  706 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "LENG?",                             STD_alpha "LENG?",                                                            SLS_UNCHANGED},
/*  707 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "MATH",                              STD_alpha "MATH",                                                             SLS_UNCHANGED},
/*  708 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "OFF",                               STD_alpha "OFF",                                                              SLS_UNCHANGED},
/*  709 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "ON",                                STD_alpha "ON",                                                               SLS_UNCHANGED},
/*  710 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "POS?",                              STD_alpha "POS?",                                                             SLS_UNCHANGED},
/*  711 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "RL",                                STD_alpha "RL",                                                               SLS_UNCHANGED},
/*  712 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "RR",                                STD_alpha "RR",                                                               SLS_UNCHANGED},
/*  713 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "SL",                                STD_alpha "SL",                                                               SLS_UNCHANGED},
/*  714 */  { itemToBeCoded,               NOPARAM,                     /* STD_alpha ".FN",                               STD_alpha ".FN", */  STD_alpha "STR", STD_alpha "STR",                                                             SLS_UNCHANGED},    //JM Changed a.FN to STRNG
/*  715 */  { itemToBeCoded,               NOPARAM,                     STD_ALPHA STD_ELLIPSIS STD_OMEGA,              STD_ALPHA STD_ELLIPSIS STD_OMEGA,                                             SLS_UNCHANGED},
/*  716 */  { itemToBeCoded,               NOPARAM,                     STD_alpha STD_DOT,                             STD_alpha STD_DOT,                                                            SLS_UNCHANGED},
/*  717 */  { itemToBeCoded,               NOPARAM,                     STD_alpha STD_RIGHT_ARROW "x",                 STD_alpha STD_RIGHT_ARROW "x",                                                SLS_UNCHANGED},
/*  718 */  { itemToBeCoded,               NOPARAM,                     STD_beta "(x,y)",                              STD_beta "(x,y)",                                                             SLS_UNCHANGED},
/*  719 */  { fnConstant,                  58,                          STD_gamma,                                     STD_gamma,                                                                    SLS_ENABLED  },
/*  720 */  { fnConstant,                  59,                          STD_gamma STD_SUB_E STD_SUB_M,                 STD_gamma STD_SUB_E STD_SUB_M,                                                SLS_ENABLED  },
/*  721 */  { fnConstant,                  60,                          STD_gamma STD_SUB_p,                           STD_gamma STD_SUB_p,                                                          SLS_ENABLED  },
/*  722 */  { itemToBeCoded,               NOPARAM,                     STD_gamma STD_SUB_x STD_SUB_y,                 STD_gamma STD_SUB_x STD_SUB_y,                                                SLS_UNCHANGED},
/*  723 */  { itemToBeCoded,               NOPARAM,                     STD_GAMMA STD_SUB_x STD_SUB_y,                 STD_GAMMA STD_SUB_x STD_SUB_y,                                                SLS_UNCHANGED},
/*  724 */  { fnGamma,                     NOPARAM,                     STD_GAMMA "(x)",                               STD_GAMMA "(x)",                                                              SLS_ENABLED  },
/*  725 */  { itemToBeCoded,               NOPARAM,                     STD_delta "x",                                 STD_delta "x",                                                                SLS_UNCHANGED},
/*  726 */  { itemToBeCoded,               NOPARAM,                     STD_DELTA "%",                                 STD_DELTA "%",                                                                SLS_UNCHANGED},
/*  727 */  { itemToBeCoded,               NOPARAM,                     STD_epsilon,                                   STD_epsilon,                                                                  SLS_UNCHANGED},
/*  728 */  { fnConstant,                  61,                          STD_epsilon STD_SUB_0,                         STD_epsilon STD_SUB_0,                                                        SLS_ENABLED  },
/*  729 */  { itemToBeCoded,               NOPARAM,                     STD_epsilon STD_SUB_m,                         STD_epsilon STD_SUB_m,                                                        SLS_UNCHANGED},
/*  730 */  { itemToBeCoded,               NOPARAM,                     STD_epsilon STD_SUB_p,                         STD_epsilon STD_SUB_p,                                                        SLS_UNCHANGED},
/*  731 */  { itemToBeCoded,               NOPARAM,                     STD_zeta "(x)",                                STD_zeta "(x)",                                                               SLS_UNCHANGED},
/*  732 */  { fnConstant,                  62,                          STD_lambda STD_SUB_C,                          STD_lambda STD_SUB_C,                                                         SLS_ENABLED  },
/*  733 */  { fnConstant,                  63,                          STD_lambda STD_SUB_C STD_SUB_n,                STD_lambda STD_SUB_C STD_SUB_n,                                               SLS_ENABLED  },
/*  734 */  { fnConstant,                  64,                          STD_lambda STD_SUB_C STD_SUB_p,                STD_lambda STD_SUB_C STD_SUB_p,                                               SLS_ENABLED  },
/*  735 */  { fnConstant,                  65,                          STD_mu STD_SUB_0,                              STD_mu STD_SUB_0,                                                             SLS_ENABLED  },
/*  736 */  { fnConstant,                  66,                          STD_mu STD_SUB_B,                              STD_mu STD_SUB_B,                                                             SLS_ENABLED  },
/*  737 */  { fnConstant,                  67,                          STD_mu STD_SUB_e,                              STD_mu STD_SUB_e,                                                             SLS_ENABLED  },
/*  738 */  { fnConstant,                  68,                          STD_mu STD_SUB_e "/" STD_mu STD_SUB_B,         STD_mu STD_SUB_e "/" STD_mu STD_SUB_B,                                        SLS_ENABLED  },
/*  739 */  { fnConstant,                  69,                          STD_mu STD_SUB_n,                              STD_mu STD_SUB_n,                                                             SLS_ENABLED  },
/*  740 */  { fnConstant,                  70,                          STD_mu STD_SUB_p,                              STD_mu STD_SUB_p,                                                             SLS_ENABLED  },
/*  741 */  { fnConstant,                  71,                          STD_mu STD_SUB_u,                              STD_mu STD_SUB_u,                                                             SLS_ENABLED  },
/*  742 */  { fnConstant,                  72,                          STD_mu STD_SUB_mu,                             STD_mu STD_SUB_mu,                                                            SLS_ENABLED  },
/*  743 */  { itemToBeCoded,               NOPARAM,                     STD_PI STD_SUB_n,                              STD_PI STD_SUB_n,                                                             SLS_UNCHANGED},
/*  744 */  { fnPi,                        NOPARAM,                     STD_pi,                                        STD_pi,                                                                       SLS_ENABLED  },
/*  745 */  { itemToBeCoded,               NOPARAM,                     STD_SIGMA STD_SUB_n,                           STD_SIGMA STD_SUB_n,                                                          SLS_UNCHANGED},
/*  746 */  { itemToBeCoded,               NOPARAM,                     STD_sigma,                                     STD_sigma,                                                                    SLS_UNCHANGED},
/*  747 */  { fnConstant,                  73,                          STD_sigma STD_SUB_B,                           STD_sigma STD_SUB_B,                                                          SLS_ENABLED  },
/*  748 */  { fnStatSum,                   9,                           STD_SIGMA "ln" STD_SUP_2 "x",                  STD_SIGMA "ln" STD_SUP_2 "x",                                                 SLS_ENABLED  },
/*  749 */  { fnStatSum,                   12,                          STD_SIGMA "ln" STD_SUP_2 "y",                  STD_SIGMA "ln" STD_SUP_2 "y",                                                 SLS_ENABLED  },
/*  750 */  { fnStatSum,                   8,                           STD_SIGMA "lnx",                               STD_SIGMA "lnx",                                                              SLS_ENABLED  },
/*  751 */  { fnStatSum,                   7,                           STD_SIGMA "lnxy",                              STD_SIGMA "lnxy",                                                             SLS_ENABLED  },
/*  752 */  { fnStatSum,                   11,                          STD_SIGMA "lny",                               STD_SIGMA "lny",                                                              SLS_ENABLED  },
/*  753 */  { itemToBeCoded,               NOPARAM,                     STD_sigma STD_SUB_w,                           STD_sigma STD_SUB_w,                                                          SLS_UNCHANGED},
/*  754 */  { fnStatSum,                   1,                           STD_SIGMA "x",                                 STD_SIGMA "x",                                                                SLS_ENABLED  },
/*  755 */  { fnStatSum,                   3,                           STD_SIGMA "x" STD_SUP_2,                       STD_SIGMA "x" STD_SUP_2,                                                      SLS_ENABLED  },
/*  756 */  { fnStatSum,                   4,                           STD_SIGMA "x" STD_SUP_2 "y",                   STD_SIGMA "x" STD_SUP_2 "y",                                                  SLS_ENABLED  },
/*  757 */  { fnStatSum,                   13,                          STD_SIGMA "xlny",                              STD_SIGMA "xlny",                                                             SLS_ENABLED  },
/*  758 */  { fnStatSum,                   6,                           STD_SIGMA "xy",                                STD_SIGMA "xy",                                                               SLS_ENABLED  },
/*  759 */  { fnStatSum,                   2,                           STD_SIGMA "y",                                 STD_SIGMA "y",                                                                SLS_ENABLED  },
/*  760 */  { fnStatSum,                   5,                           STD_SIGMA "y" STD_SUP_2,                       STD_SIGMA "y" STD_SUP_2,                                                      SLS_ENABLED  },
/*  761 */  { fnStatSum,                   10,                          STD_SIGMA "ylnx",                              STD_SIGMA "ylnx",                                                             SLS_ENABLED  },
/*  762 */  { fnSigma,                     1,                           STD_SIGMA "+",                                 STD_SIGMA "+",                                                                SLS_DISABLED },
/*  763 */  { fnSigma,                     2,                           STD_SIGMA "-",                                 STD_SIGMA "-",                                                                SLS_DISABLED },
/*  764 */  { fnConstant,                  74,                          STD_PHI,                                       STD_PHI,                                                                      SLS_ENABLED  },
/*  765 */  { fnConstant,                  75,                          STD_PHI STD_SUB_0,                             STD_PHI STD_SUB_0,                                                            SLS_ENABLED  },
/*  766 */  { itemToBeCoded,               NOPARAM,                     STD_PHI STD_SUB_e "(x)",                       STD_PHI STD_SUB_e "(x)",                                                      SLS_UNCHANGED},
/*  767 */  { itemToBeCoded,               NOPARAM,                     STD_PHI "(x)",                                 STD_PHI "(x)",                                                                SLS_UNCHANGED},
/*  768 */  { itemToBeCoded,               NOPARAM,                     STD_phi "(x)",                                 STD_phi "(x)",                                                                SLS_UNCHANGED},
/*  769 */  { itemToBeCoded,               NOPARAM,                     STD_PHI STD_SUP_MINUS_1 "(p)",                 STD_PHI STD_SUP_MINUS_1 "(p)",                                                SLS_UNCHANGED},
/*  770 */  { itemToBeCoded,               NOPARAM,                     STD_PHI ":",                                   STD_PHI ":",                                                                  SLS_UNCHANGED},
/*  771 */  { itemToBeCoded,               NOPARAM,                     STD_chi STD_SUP_2 "(x)",                       STD_chi STD_SUP_2 "(x)",                                                      SLS_UNCHANGED},
/*  772 */  { itemToBeCoded,               NOPARAM,                     STD_chi STD_SUP_2 STD_SUB_P "(x)",             STD_chi STD_SUP_2 STD_SUB_P "(x)",                                            SLS_UNCHANGED},
/*  773 */  { itemToBeCoded,               NOPARAM,                     STD_chi STD_SUP_2 STD_SUB_e "(x)",             STD_chi STD_SUP_2 STD_SUB_e "(x)",                                            SLS_UNCHANGED},
/*  774 */  { itemToBeCoded,               NOPARAM,                     "(" STD_chi STD_SUP_2 ")" STD_SUP_MINUS_1,     "(" STD_chi STD_SUP_2 ")" STD_SUP_MINUS_1,                                    SLS_UNCHANGED},
/*  775 */  { itemToBeCoded,               NOPARAM,                     STD_chi STD_SUP_2 ":",                         STD_chi STD_SUP_2 ":",                                                        SLS_UNCHANGED},
/*  776 */  { fnConstant,                  76,                          STD_omega,                                     STD_omega,                                                                    SLS_ENABLED  },
/*  777 */  { fnM1Pow,                     NOPARAM,                     "(-1)" STD_SUP_x,                              "(-1)" STD_SUP_x,                                                             SLS_ENABLED  },
/*  778 */  { fnAdd,                       ITM_ADD,                     "+",                                           "+",                                                                          SLS_ENABLED  },
/*  JM 779 */  { fnChangeSign,                ITM_CHS,                     "CHS",                                         "CHS",                                                                        SLS_ENABLED  },   //JM Change +/- to CHS
/*  780 */  { fnSubtract,                  ITM_SUB,                     "-",                                           "-",                                                                          SLS_ENABLED  },
/*  781 */  { fnConstant,                  77,                          "-" STD_INFINITY,                              "-" STD_INFINITY,                                                             SLS_ENABLED  },
/*  782 */  { fnMultiply,                  ITM_MULT,                    STD_CROSS,                                     STD_CROSS,                                                                    SLS_ENABLED  },
/*  783 */  { itemToBeCoded,               NOPARAM,                     STD_CROSS "MOD",                               STD_CROSS "MOD",                                                              SLS_UNCHANGED},
/*  784 */  { fnDivide,                    ITM_DIV,                     "/",                                           "/",                                                                          SLS_ENABLED  },
/*  785 */  { itemToBeCoded,               NOPARAM,                     STD_PLUS_MINUS STD_INFINITY "?",               STD_PLUS_MINUS STD_INFINITY "?",                                              SLS_UNCHANGED},
/*  786 */  { addItemToBuffer,             ITM_INDIRECTION,             STD_RIGHT_ARROW,                               STD_RIGHT_ARROW,                                                              SLS_UNCHANGED},
/*  787 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "DATE",                        STD_RIGHT_ARROW "DATE",                                                       SLS_UNCHANGED},
/*  788 */  { fnCvtFromCurrentAngularMode, AM_DEGREE,                   STD_RIGHT_ARROW "DEG",                         STD_RIGHT_ARROW "DEG",                                                        SLS_ENABLED  },
/*  789 */  { fnCvtFromCurrentAngularMode, AM_DMS,                      STD_RIGHT_ARROW "D.MS",                        STD_RIGHT_ARROW "D.MS",                                                       SLS_ENABLED  },
/*  790 */  { fnCvtFromCurrentAngularMode, AM_GRAD,                     STD_RIGHT_ARROW "GRAD",                        STD_RIGHT_ARROW "GRAD",                                                       SLS_ENABLED  },
/*  791 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "HR",                          ".d",                                                                         SLS_ENABLED  },
/*  792 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "H.MS",                        "h.ms",                                                                       SLS_UNCHANGED},
/*  793 */  { fnChangeBase,                TM_VALUE_CHB,                STD_RIGHT_ARROW "INT",                         "#",                                                                          SLS_UNCHANGED},
/*  794 */  { fnCvtFromCurrentAngularMode, AM_MULTPI,                   STD_RIGHT_ARROW "MUL" STD_pi,                  STD_RIGHT_ARROW "MUL" STD_pi,                                                 SLS_ENABLED  },
/*  795 */  { fnToPolar,                   NOPARAM,                     STD_RIGHT_ARROW "POL",                         STD_RIGHT_ARROW "P",                                                          SLS_ENABLED  },
/*  796 */  { fnCvtFromCurrentAngularMode, AM_RADIAN,                   STD_RIGHT_ARROW "RAD",                         STD_RIGHT_ARROW "RAD",                                                        SLS_ENABLED  },
/*  797 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "REAL",                        ".d",                                                                         SLS_ENABLED  },
/*  798 */  { fnToRect,                    NOPARAM,                     STD_RIGHT_ARROW "REC",                         STD_RIGHT_ARROW "R",                                                                     SLS_ENABLED  },   //SWAPPED THE ARROW DIRECTION AND POSITION
/*  799 */  { fnCvtDegToDms,               NOPARAM,                     "D" STD_RIGHT_ARROW "D.MS",                    "D" STD_RIGHT_ARROW "D.MS",                                                   SLS_ENABLED  },
/*  800 */  { itemToBeCoded,               NOPARAM,                     STD_UP_ARROW "Lim",                            STD_UP_ARROW "Lim",                                                           SLS_UNCHANGED},
/*  801 */  { itemToBeCoded,               NOPARAM,                     STD_DOWN_ARROW "Lim",                          STD_DOWN_ARROW "Lim",                                                         SLS_UNCHANGED},
/*  802 */  { itemToBeCoded,               NOPARAM,                     STD_LEFT_RIGHT_ARROWS,                         STD_LEFT_RIGHT_ARROWS,                                                        SLS_UNCHANGED},
/*  803 */  { itemToBeCoded,               NOPARAM,                     "%",                                           "%",                                                                          SLS_UNCHANGED},
/*  804 */  { itemToBeCoded,               NOPARAM,                     "%MRR",                                        "%MRR",                                                                       SLS_UNCHANGED},
/*  805 */  { itemToBeCoded,               NOPARAM,                     "%T",                                          "%T",                                                                         SLS_UNCHANGED},
/*  806 */  { itemToBeCoded,               NOPARAM,                     "%" STD_SIGMA,                                 "%" STD_SIGMA,                                                                SLS_UNCHANGED},
/*  807 */  { itemToBeCoded,               NOPARAM,                     "%+MG",                                        "%+MG",                                                                       SLS_UNCHANGED},
/*  808 */  { fnSquareRoot,                NOPARAM,                     STD_SQUARE_ROOT STD_x_UNDER_ROOT,              STD_SQUARE_ROOT STD_x_UNDER_ROOT,                                             SLS_ENABLED  },
/*  809 */  { itemToBeCoded,               NOPARAM,                     STD_INTEGRAL,                                  STD_INTEGRAL,                                                                 SLS_UNCHANGED},
/*  810 */  { itemToBeCoded,               NOPARAM,                     STD_INTEGRAL "f",                              STD_INTEGRAL "f",                                                             SLS_UNCHANGED},
/*  811 */  { itemToBeCoded,               NOPARAM,                     STD_INTEGRAL "fdx",                            STD_INTEGRAL "fdx",                                                           SLS_UNCHANGED},
/*  812 */  { fnConstant,                  78,                          STD_INFINITY,                                  STD_INFINITY,                                                                 SLS_ENABLED  },
/*  813 */  { itemToBeCoded,               NOPARAM,                     "^MOD",                                        "^MOD",                                                                       SLS_UNCHANGED},
/*  814 */  { itemToBeCoded,               NOPARAM,                     "|M|",                                         "|M|",                                                                        SLS_UNCHANGED},
/*  815 */  { fnMagnitude,                 NOPARAM,                     "|x|",                                         "|x|",                                                                        SLS_ENABLED  },
/*  816 */  { fnParallel,                  NOPARAM,                     "|" STD_SPACE_3_PER_EM "|",                    "|" STD_SPACE_3_PER_EM "|",                                                   SLS_ENABLED  },
/*  817 */  { itemToBeCoded,               NOPARAM,                     "[M]" STD_SUP_T,                               "[M]" STD_SUP_T,                                                              SLS_UNCHANGED},
/*  818 */  { itemToBeCoded,               NOPARAM,                     "[M]" STD_SUP_MINUS_1,                         "[M]" STD_SUP_MINUS_1,                                                        SLS_UNCHANGED},
/*  819 */  { fnJM,                        002,       /*JMCALC*/        STD_MEASURED_ANGLE,                            STD_MEASURED_ANGLE,                                                           SLS_UNCHANGED},    //JM CALC
/*  820 */  { fnCvtToCurrentAngularMode,   AM_MULTPI,                   "MUL" STD_pi STD_RIGHT_ARROW,                  "MUL" STD_pi STD_RIGHT_ARROW,                                                 SLS_ENABLED  },
/* JM  821 */  { itemToBeCoded,               NOPARAM,                  "CONV",                                        "CONV",                                                                   SLS_UNCHANGED},    //JM Change to text DRG and change again to CONV
/*  822 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "ADV",                             STD_PRINTER "ADV",                                                            SLS_UNCHANGED},
/*  823 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "CHAR",                            STD_PRINTER "CHAR",                                                           SLS_UNCHANGED},
/*  824 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "DLAY",                            STD_PRINTER "DLAY",                                                           SLS_UNCHANGED},
/*  825 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "LCD",                             STD_PRINTER "LCD",                                                            SLS_UNCHANGED},
/*  826 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "MODE",                            STD_PRINTER "MODE",                                                           SLS_UNCHANGED},
/*  827 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "PROG",                            STD_PRINTER "PROG",                                                           SLS_UNCHANGED},
/*  828 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "r",                               STD_PRINTER "r",                                                              SLS_UNCHANGED},
/*  829 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "REGS",                            STD_PRINTER "REGS",                                                           SLS_UNCHANGED},
/*  830 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "STK",                             STD_PRINTER "STK",                                                            SLS_UNCHANGED},
/*  831 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "TAB",                             STD_PRINTER "TAB",                                                            SLS_UNCHANGED},
/*  832 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "USER",                            STD_PRINTER "USER",                                                           SLS_UNCHANGED},
/*  833 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "WIDTH",                           STD_PRINTER "WIDTH",                                                          SLS_UNCHANGED},
/*  834 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER STD_SIGMA,                         STD_PRINTER STD_SIGMA,                                                        SLS_UNCHANGED},
/*  835 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "#",                               STD_PRINTER "#",                                                              SLS_UNCHANGED},
/*  836 */  { itemToBeCoded,               79,                          "#",                                           "#",                                                                          SLS_UNCHANGED},
/*  837 */  { itemToBeCoded,               NOPARAM,                     "#B",                                          "#B",                                                                         SLS_UNCHANGED},

/*  838 */  { fnCvtAcreM2,                 multiply,                    "ac" STD_RIGHT_ARROW "m" STD_SUP_2,            STD_RIGHT_ARROW " m" STD_SUP_2,                                               SLS_ENABLED  },
/*  839 */  { fnCvtAcreusM2,               multiply,                    "ac" STD_US STD_RIGHT_ARROW "m" STD_SUP_2,     STD_RIGHT_ARROW " m" STD_SUP_2,                                               SLS_ENABLED  },
/*  840 */  { fnCvtCaratKg,                multiply,                    "ct" STD_RIGHT_ARROW "kg",                     "carat",                                                                      SLS_ENABLED  },
/*  841 */  { fnCvtDbRatio,                20,                          "dB" STD_RIGHT_ARROW "fr",                     "field",                                                                      SLS_ENABLED  },
/*  842 */  { fnCvtDbRatio,                20,                          "dB" STD_RIGHT_ARROW "fr",                     "ratio",                                                                      SLS_ENABLED  },
/*  843 */  { fnCvtDbRatio,                10,                          "dB" STD_RIGHT_ARROW "pr",                     "power",                                                                      SLS_ENABLED  },
/*  844 */  { fnCvtDbRatio,                10,                          "dB" STD_RIGHT_ARROW "pr",                     "ratio",                                                                      SLS_ENABLED  },
/*  845 */  { fnCvtRatioDb,                20,                          "fr" STD_RIGHT_ARROW "dB",                     "ratio",                                                                      SLS_ENABLED  },
/*  846 */  { fnCvtRatioDb,                20,                          "fr" STD_RIGHT_ARROW "dB",                     STD_RIGHT_ARROW " dB",                                                        SLS_ENABLED  },
/*  847 */  { fnCvtSfeetM,                 multiply,                    "ft" STD_US STD_RIGHT_ARROW "m",               "foot" STD_US,                                                                SLS_ENABLED  },
/*  848 */  { fnCvtFlozukM3,               multiply,                    "fz" STD_UK STD_RIGHT_ARROW "m" STD_SUP_3,     STD_RIGHT_ARROW " m" STD_SUP_3,                                               SLS_ENABLED  },
/*  849 */  { fnCvtFlozusM3,               multiply,                    "fz" STD_US STD_RIGHT_ARROW "m" STD_SUP_3,     STD_RIGHT_ARROW " m" STD_SUP_3,                                               SLS_ENABLED  },
/*  850 */  { fnCvtInhgPa,                 multiply,                    "iHg" STD_RIGHT_ARROW "Pa",                    STD_RIGHT_ARROW " Pa",                                                        SLS_ENABLED  },
/*  851 */  { fnCvtShortcwtKg,             divide,                      "kg" STD_RIGHT_ARROW "scw",                    "sh.cwt",                                                                     SLS_ENABLED  },
/*  852 */  { fnCvtStoneKg,                divide,                      "kg" STD_RIGHT_ARROW "sto",                    "stone",                                                                      SLS_ENABLED  },
/*  853 */  { fnCvtShorttonKg,             divide,                      "kg" STD_RIGHT_ARROW "s.t",                    "short",                                                                      SLS_ENABLED  },
/*  854 */  { fnCvtShorttonKg,             divide,                      "kg" STD_RIGHT_ARROW "s.t",                    "ton",                                                                        SLS_ENABLED  },
/*  855 */  { fnCvtCaratKg,                divide,                      "kg" STD_RIGHT_ARROW "ct",                     "kg " STD_RIGHT_ARROW,                                                        SLS_ENABLED  },
/*  856 */  { fnCvtTrozKg,                 divide,                      "kg" STD_RIGHT_ARROW "trz",                    "tr.oz",                                                                      SLS_ENABLED  },
/*  857 */  { fnCvtAcreM2,                 divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ac",            "acre",                                                                       SLS_ENABLED  },
/*  858 */  { fnCvtAcreusM2,               divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ac" STD_US,     "acre" STD_US,                                                                SLS_ENABLED  },
/*  859 */  { fnCvtFlozukM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "fz" STD_UK,     "floz" STD_UK,                                                                SLS_ENABLED  },
/*  860 */  { fnCvtFlozusM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "fz" STD_US,     "floz" STD_US,                                                                SLS_ENABLED  },
/*  861 */  { fnCvtSfeetM,                 divide,                      "m" STD_RIGHT_ARROW "ft" STD_US,               "survey",                                                                     SLS_ENABLED  },
/*  862 */  { fnCvtCaratKg,                multiply,                    "ct" STD_RIGHT_ARROW "kg",                     STD_RIGHT_ARROW " kg",                                                        SLS_ENABLED  },
/*  863 */  { fnCvtInhgPa,                 divide,                      "Pa" STD_RIGHT_ARROW "iHg",                    "in.Hg",                                                                      SLS_ENABLED  },
/*  864 */  { fnCvtTorrPa,                 divide,                      "Pa" STD_RIGHT_ARROW "tor",                    "torr",                                                                       SLS_ENABLED  },
/*  865 */  { fnCvtRatioDb,                10,                          "pr" STD_RIGHT_ARROW "dB",                     "ratio",                                                                      SLS_ENABLED  },
/*  866 */  { fnCvtRatioDb,                10,                          "pr" STD_RIGHT_ARROW "dB",                     STD_RIGHT_ARROW " dB",                                                        SLS_ENABLED  },
/*  867 */  { fnCvtShortcwtKg,             multiply,                    "scw" STD_RIGHT_ARROW "kg",                    "cwt" STD_RIGHT_ARROW "kg",                                                   SLS_ENABLED  },
/*  868 */  { fnCvtStoneKg,                multiply,                    "sto" STD_RIGHT_ARROW "kg",                    STD_RIGHT_ARROW " kg",                                                        SLS_ENABLED  },
/*  869 */  { fnCvtShorttonKg,             multiply,                    "s.t" STD_RIGHT_ARROW "kg",                    "ton",                                                                        SLS_ENABLED  },
/*  870 */  { fnCvtShorttonKg,             multiply,                    "s.t" STD_RIGHT_ARROW "kg",                    STD_RIGHT_ARROW " kg",                                                        SLS_ENABLED  },
/*  871 */  { fnCvtCaratKg,                divide,                      "kg" STD_RIGHT_ARROW "ct",                     "carat",                                                                      SLS_ENABLED  },
/*  872 */  { fnCvtTorrPa,                 multiply,                    "tor" STD_RIGHT_ARROW "Pa",                    STD_RIGHT_ARROW " Pa",                                                        SLS_ENABLED  },
/*  873 */  { fnCvtTrozKg,                 multiply,                    "trz" STD_RIGHT_ARROW "kg",                    STD_RIGHT_ARROW " kg",                                                        SLS_ENABLED  },

/*  874 */  { fontBrowser,                 NOPARAM,                     "FBR",                                         "FBR",                                                                        SLS_UNCHANGED}, // Font Browser
/*  875 */  { itemToBeCoded,               NOPARAM,                     "a" STD_ELLIPSIS "z",                          "a" STD_ELLIPSIS "z",                                                         SLS_UNCHANGED}, // Small latin letters
/*  876 */  { itemToBeCoded,               NOPARAM,                     STD_alpha STD_ELLIPSIS STD_omega,              STD_alpha STD_ELLIPSIS STD_omega,                                             SLS_UNCHANGED}, // Small greek letters
/*  877 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "intl",                              STD_alpha "intl",                                                             SLS_UNCHANGED}, // Small intl letters
/*  878 */  { addItemToBuffer,             REGISTER_X,                  "REG_X",                                       "X",                                                                          SLS_UNCHANGED}, // The order
/*  879 */  { addItemToBuffer,             REGISTER_Y,                  "REG_Y",                                       "Y",                                                                          SLS_UNCHANGED}, // of these 8
/*  880 */  { addItemToBuffer,             REGISTER_Z,                  "REG_Z",                                       "Z",                                                                          SLS_UNCHANGED}, // lines MUST
/*  881 */  { addItemToBuffer,             REGISTER_T,                  "REG_T",                                       "T",                                                                          SLS_UNCHANGED}, // be kept as
/*  882 */  { addItemToBuffer,             REGISTER_A,                  "REG_A",                                       "A",                                                                          SLS_UNCHANGED}, // is. Do not
/*  883 */  { addItemToBuffer,             REGISTER_B,                  "REG_B",                                       "B",                                                                          SLS_UNCHANGED}, // put them in
/*  884 */  { addItemToBuffer,             REGISTER_C,                  "REG_C",                                       "C",                                                                          SLS_UNCHANGED}, // alphabetical
/*  885 */  { addItemToBuffer,             REGISTER_D,                  "REG_D",                                       "D",                                                                          SLS_UNCHANGED}, // order!
/*  886 */  { addItemToBuffer,             REGISTER_L,                  "REG_L",                                       "L",                                                                          SLS_UNCHANGED},
/*  887 */  { addItemToBuffer,             REGISTER_I,                  "REG_I",                                       "I",                                                                          SLS_UNCHANGED},
/*  888 */  { addItemToBuffer,             REGISTER_J,                  "REG_J",                                       "J",                                                                          SLS_UNCHANGED},
/*  889 */  { addItemToBuffer,             REGISTER_K,                  "REG_K",                                       "K",                                                                          SLS_UNCHANGED},

/*  890 */  { addItemToBuffer,             CHR_0,                       "",                                            "0",                                                                          SLS_UNCHANGED},
/*  891 */  { addItemToBuffer,             CHR_1,                       "",                                            "1",                                                                          SLS_UNCHANGED},
/*  892 */  { addItemToBuffer,             CHR_2,                       "",                                            "2",                                                                          SLS_UNCHANGED},
/*  893 */  { addItemToBuffer,             CHR_3,                       "",                                            "3",                                                                          SLS_UNCHANGED},
/*  894 */  { addItemToBuffer,             CHR_4,                       "",                                            "4",                                                                          SLS_UNCHANGED},
/*  895 */  { addItemToBuffer,             CHR_5,                       "",                                            "5",                                                                          SLS_UNCHANGED},
/*  896 */  { addItemToBuffer,             CHR_6,                       "",                                            "6",                                                                          SLS_UNCHANGED},
/*  897 */  { addItemToBuffer,             CHR_7,                       "",                                            "7",                                                                          SLS_UNCHANGED},
/*  898 */  { addItemToBuffer,             CHR_8,                       "",                                            "8",                                                                          SLS_UNCHANGED},
/*  899 */  { addItemToBuffer,             CHR_9,                       "",                                            "9",                                                                          SLS_UNCHANGED},
/*  900 */  { addItemToBuffer,             CHR_A,                       "",                                            "A",                                                                          SLS_UNCHANGED},
/*  901 */  { addItemToBuffer,             CHR_B,                       "",                                            "B",                                                                          SLS_UNCHANGED},
/*  902 */  { addItemToBuffer,             CHR_C,                       "",                                            "C",                                                                          SLS_UNCHANGED},
/*  903 */  { addItemToBuffer,             CHR_D,                       "",                                            "D",                                                                          SLS_UNCHANGED},
/*  904 */  { addItemToBuffer,             CHR_E,                       "",                                            "E",                                                                          SLS_UNCHANGED},
/*  905 */  { addItemToBuffer,             CHR_F,                       "",                                            "F",                                                                          SLS_UNCHANGED},
/*  906 */  { addItemToBuffer,             CHR_G,                       "",                                            "G",                                                                          SLS_UNCHANGED},
/*  907 */  { addItemToBuffer,             CHR_H,                       "",                                            "H",                                                                          SLS_UNCHANGED},
/*  908 */  { addItemToBuffer,             CHR_I,                       "",                                            "I",                                                                          SLS_UNCHANGED},
/*  909 */  { addItemToBuffer,             CHR_J,                       "",                                            "J",                                                                          SLS_UNCHANGED},
/*  910 */  { addItemToBuffer,             CHR_K,                       "",                                            "K",                                                                          SLS_UNCHANGED},
/*  911 */  { addItemToBuffer,             CHR_L,                       "",                                            "L",                                                                          SLS_UNCHANGED},
/*  912 */  { addItemToBuffer,             CHR_M,                       "",                                            "M",                                                                          SLS_UNCHANGED},
/*  913 */  { addItemToBuffer,             CHR_N,                       "",                                            "N",                                                                          SLS_UNCHANGED},
/*  914 */  { addItemToBuffer,             CHR_O,                       "",                                            "O",                                                                          SLS_UNCHANGED},
/*  915 */  { addItemToBuffer,             CHR_P,                       "",                                            "P",                                                                          SLS_UNCHANGED},
/*  916 */  { addItemToBuffer,             CHR_Q,                       "",                                            "Q",                                                                          SLS_UNCHANGED},
/*  917 */  { addItemToBuffer,             CHR_R,                       "",                                            "R",                                                                          SLS_UNCHANGED},
/*  918 */  { addItemToBuffer,             CHR_S,                       "",                                            "S",                                                                          SLS_UNCHANGED},
/*  919 */  { addItemToBuffer,             CHR_T,                       "",                                            "T",                                                                          SLS_UNCHANGED},
/*  920 */  { addItemToBuffer,             CHR_U,                       "",                                            "U",                                                                          SLS_UNCHANGED},
/*  921 */  { addItemToBuffer,             CHR_V,                       "",                                            "V",                                                                          SLS_UNCHANGED},
/*  922 */  { addItemToBuffer,             CHR_W,                       "",                                            "W",                                                                          SLS_UNCHANGED},
/*  923 */  { addItemToBuffer,             CHR_X,                       "",                                            "X",                                                                          SLS_UNCHANGED},
/*  924 */  { addItemToBuffer,             CHR_Y,                       "",                                            "Y",                                                                          SLS_UNCHANGED},
/*  925 */  { addItemToBuffer,             CHR_Z,                       "",                                            "Z",                                                                          SLS_UNCHANGED},
/*  926 */  { addItemToBuffer,             CHR_a,                       "",                                            "a",                                                                          SLS_UNCHANGED},
/*  927 */  { addItemToBuffer,             CHR_b,                       "",                                            "b",                                                                          SLS_UNCHANGED},
/*  928 */  { addItemToBuffer,             CHR_c,                       "",                                            "c",                                                                          SLS_UNCHANGED},
/*  929 */  { addItemToBuffer,             CHR_d,                       "",                                            "d",                                                                          SLS_UNCHANGED},
/*  930 */  { addItemToBuffer,             CHR_e,                       "",                                            "e",                                                                          SLS_UNCHANGED},
/*  931 */  { addItemToBuffer,             CHR_f,                       "",                                            "f",                                                                          SLS_UNCHANGED},
/*  932 */  { addItemToBuffer,             CHR_g,                       "",                                            "g",                                                                          SLS_UNCHANGED},
/*  933 */  { addItemToBuffer,             CHR_h,                       "",                                            "h",                                                                          SLS_UNCHANGED},
/*  934 */  { addItemToBuffer,             CHR_i,                       "",                                            "i",                                                                          SLS_UNCHANGED},
/*  935 */  { addItemToBuffer,             CHR_j,                       "",                                            "j",                                                                          SLS_UNCHANGED},
/*  936 */  { addItemToBuffer,             CHR_k,                       "",                                            "k",                                                                          SLS_UNCHANGED},
/*  937 */  { addItemToBuffer,             CHR_l,                       "",                                            "l",                                                                          SLS_UNCHANGED},
/*  938 */  { addItemToBuffer,             CHR_m,                       "",                                            "m",                                                                          SLS_UNCHANGED},
/*  939 */  { addItemToBuffer,             CHR_n,                       "",                                            "n",                                                                          SLS_UNCHANGED},
/*  940 */  { addItemToBuffer,             CHR_o,                       "",                                            "o",                                                                          SLS_UNCHANGED},
/*  941 */  { addItemToBuffer,             CHR_p,                       "",                                            "p",                                                                          SLS_UNCHANGED},
/*  942 */  { addItemToBuffer,             CHR_q,                       "",                                            "q",                                                                          SLS_UNCHANGED},
/*  943 */  { addItemToBuffer,             CHR_r,                       "",                                            "r",                                                                          SLS_UNCHANGED},
/*  944 */  { addItemToBuffer,             CHR_s,                       "",                                            "s",                                                                          SLS_UNCHANGED},
/*  945 */  { addItemToBuffer,             CHR_t,                       "",                                            "t",                                                                          SLS_UNCHANGED},
/*  946 */  { addItemToBuffer,             CHR_u,                       "",                                            "u",                                                                          SLS_UNCHANGED},
/*  947 */  { addItemToBuffer,             CHR_v,                       "",                                            "v",                                                                          SLS_UNCHANGED},
/*  948 */  { addItemToBuffer,             CHR_w,                       "",                                            "w",                                                                          SLS_UNCHANGED},
/*  949 */  { addItemToBuffer,             CHR_x,                       "",                                            "x",                                                                          SLS_UNCHANGED},
/*  950 */  { addItemToBuffer,             CHR_y,                       "",                                            "y",                                                                          SLS_UNCHANGED},
/*  951 */  { addItemToBuffer,             CHR_z,                       "",                                            "z",                                                                          SLS_UNCHANGED},
/*  952 */  { addItemToBuffer,             CHR_ALPHA,                   "",                                            STD_ALPHA,                                                                    SLS_UNCHANGED},
/*  953 */  { itemToBeCoded,               NOPARAM,                     "0953",                                        "0953",                                                                       SLS_UNCHANGED},
/*  954 */  { addItemToBuffer,             CHR_BETA,                    "",                                            STD_BETA,                                                                     SLS_UNCHANGED},
/*  955 */  { addItemToBuffer,             CHR_GAMMA,                   "",                                            STD_GAMMA,                                                                    SLS_UNCHANGED},
/*  956 */  { addItemToBuffer,             CHR_DELTA,                   "",                                            STD_DELTA,                                                                    SLS_UNCHANGED},
/*  957 */  { addItemToBuffer,             CHR_EPSILON,                 "",                                            STD_EPSILON,                                                                  SLS_UNCHANGED},
/*  958 */  { itemToBeCoded,               NOPARAM,                     "0958",                                        "0958",                                                                       SLS_UNCHANGED},
/*  959 */  { addItemToBuffer,             CHR_ZETA,                    "",                                            STD_ZETA,                                                                     SLS_UNCHANGED},
/*  960 */  { addItemToBuffer,             CHR_ETA,                     "",                                            STD_ETA,                                                                      SLS_UNCHANGED},
/*  961 */  { itemToBeCoded,               NOPARAM,                     "",                                            "0961",                                                                       SLS_UNCHANGED},
/*  962 */  { addItemToBuffer,             CHR_THETA,                   "0961",                                        STD_THETA,                                                                    SLS_UNCHANGED},
/*  963 */  { addItemToBuffer,             CHR_IOTA,                    "",                                            STD_IOTA,                                                                     SLS_UNCHANGED},
/*  964 */  { itemToBeCoded,               NOPARAM,                     "0964",                                        "0964",                                                                       SLS_UNCHANGED},
/*  965 */  { itemToBeCoded,               NOPARAM,                     "0965",                                        "0965",                                                                       SLS_UNCHANGED},
/*  966 */  { addItemToBuffer,             CHR_IOTA_DIALYTIKA,          "",                                            STD_IOTA_DIALYTIKA,                                                           SLS_UNCHANGED},
/*  967 */  { addItemToBuffer,             CHR_KAPPA,                   "",                                            STD_KAPPA,                                                                    SLS_UNCHANGED},
/*  968 */  { addItemToBuffer,             CHR_LAMBDA,                  "",                                            STD_LAMBDA,                                                                   SLS_UNCHANGED},
/*  969 */  { addItemToBuffer,             CHR_MU,                      "",                                            STD_MU,                                                                       SLS_UNCHANGED},
/*  970 */  { addItemToBuffer,             CHR_NU,                      "",                                            STD_NU,                                                                       SLS_UNCHANGED},
/*  971 */  { addItemToBuffer,             CHR_XI,                      "",                                            STD_XI,                                                                       SLS_UNCHANGED},
/*  972 */  { addItemToBuffer,             CHR_OMICRON,                 "",                                            STD_OMICRON,                                                                  SLS_UNCHANGED},
/*  973 */  { itemToBeCoded,               NOPARAM,                     "0973",                                        "0973",                                                                       SLS_UNCHANGED},
/*  974 */  { addItemToBuffer,             CHR_PI,                      "",                                            STD_PI,                                                                       SLS_UNCHANGED},
/*  975 */  { addItemToBuffer,             CHR_RHO,                     "",                                            STD_RHO,                                                                      SLS_UNCHANGED},
/*  976 */  { addItemToBuffer,             CHR_SIGMA,                   "",                                            STD_SIGMA,                                                                    SLS_UNCHANGED},
/*  977 */  { itemToBeCoded,               NOPARAM,                     "0977",                                        "0977",                                                                       SLS_UNCHANGED},
/*  978 */  { addItemToBuffer,             CHR_TAU,                     "",                                            STD_TAU,                                                                      SLS_UNCHANGED},
/*  979 */  { addItemToBuffer,             CHR_UPSILON,                 "",                                            STD_UPSILON,                                                                  SLS_UNCHANGED},
/*  980 */  { itemToBeCoded,               NOPARAM,                     "0980",                                        "0980",                                                                       SLS_UNCHANGED},
/*  981 */  { addItemToBuffer,             CHR_UPSILON_DIALYTIKA,       "",                                            STD_UPSILON_DIALYTIKA,                                                        SLS_UNCHANGED},
/*  982 */  { itemToBeCoded,               NOPARAM,                     "0982",                                        "0982",                                                                       SLS_UNCHANGED},
/*  983 */  { addItemToBuffer,             CHR_PHI,                     "",                                            STD_PHI,                                                                      SLS_UNCHANGED},
/*  984 */  { addItemToBuffer,             CHR_CHI,                     "",                                            STD_CHI,                                                                      SLS_UNCHANGED},
/*  985 */  { addItemToBuffer,             CHR_PSI,                     "",                                            STD_PSI,                                                                      SLS_UNCHANGED},
/*  986 */  { addItemToBuffer,             CHR_OMEGA,                   "",                                            STD_OMEGA,                                                                    SLS_UNCHANGED},
/*  987 */  { itemToBeCoded,               NOPARAM,                     "0987",                                        "0987",                                                                       SLS_UNCHANGED},
/*  988 */  { addItemToBuffer,             CHR_alpha,                   "",                                            STD_alpha,                                                                    SLS_UNCHANGED},
/*  989 */  { addItemToBuffer,             CHR_alpha_TONOS,             "",                                            STD_alpha_TONOS,                                                              SLS_UNCHANGED},
/*  990 */  { addItemToBuffer,             CHR_beta,                    "",                                            STD_beta,                                                                     SLS_UNCHANGED},
/*  991 */  { addItemToBuffer,             CHR_gamma,                   "",                                            STD_gamma,                                                                    SLS_UNCHANGED},
/*  992 */  { addItemToBuffer,             CHR_delta,                   "",                                            STD_delta,                                                                    SLS_UNCHANGED},
/*  993 */  { addItemToBuffer,             CHR_epsilon,                 "",                                            STD_epsilon,                                                                  SLS_UNCHANGED},
/*  994 */  { addItemToBuffer,             CHR_epsilon_TONOS,           "",                                            STD_epsilon_TONOS,                                                            SLS_UNCHANGED},
/*  995 */  { addItemToBuffer,             CHR_zeta,                    "",                                            STD_zeta,                                                                     SLS_UNCHANGED},
/*  996 */  { addItemToBuffer,             CHR_eta,                     "",                                            STD_eta,                                                                      SLS_UNCHANGED},
/*  997 */  { addItemToBuffer,             CHR_eta_TONOS,               "",                                            STD_eta_TONOS,                                                                SLS_UNCHANGED},
/*  998 */  { addItemToBuffer,             CHR_theta,                   "",                                            STD_theta,                                                                    SLS_UNCHANGED},
/*  999 */  { addItemToBuffer,             CHR_iota,                    "",                                            STD_iota,                                                                     SLS_UNCHANGED},
/* 1000 */  { addItemToBuffer,             CHR_iotaTON,                 "",                                            STD_iota_TONOS,                                                               SLS_UNCHANGED},
/* 1001 */  { addItemToBuffer,             CHR_iota_DIALYTIKA_TONOS,    "",                                            STD_iota_DIALYTIKA_TONOS,                                                     SLS_UNCHANGED},
/* 1002 */  { addItemToBuffer,             CHR_iota_DIALYTIKA,          "",                                            STD_iota_DIALYTIKA,                                                           SLS_UNCHANGED},
/* 1003 */  { addItemToBuffer,             CHR_kappa,                   "",                                            STD_kappa,                                                                    SLS_UNCHANGED},
/* 1004 */  { addItemToBuffer,             CHR_lambda,                  "",                                            STD_lambda,                                                                   SLS_UNCHANGED},
/* 1005 */  { addItemToBuffer,             CHR_mu,                      "",                                            STD_mu,                                                                       SLS_UNCHANGED},
/* 1006 */  { addItemToBuffer,             CHR_nu,                      "",                                            STD_nu,                                                                       SLS_UNCHANGED},
/* 1007 */  { addItemToBuffer,             CHR_xi,                      "",                                            STD_xi,                                                                       SLS_UNCHANGED},
/* 1008 */  { addItemToBuffer,             CHR_omicron,                 "",                                            STD_omicron,                                                                  SLS_UNCHANGED},
/* 1009 */  { addItemToBuffer,             CHR_omicron_TONOS,           "",                                            STD_omicron_TONOS,                                                            SLS_UNCHANGED},
/* 1010 */  { addItemToBuffer,             CHR_pi,                      "",                                            STD_pi,                                                                       SLS_UNCHANGED},
/* 1011 */  { addItemToBuffer,             CHR_rho,                     "",                                            STD_rho,                                                                      SLS_UNCHANGED},
/* 1012 */  { addItemToBuffer,             CHR_sigma,                   "",                                            STD_sigma,                                                                    SLS_UNCHANGED},
/* 1013 */  { addItemToBuffer,             CHR_sigma_end,               "",                                            STD_sigma_end,                                                                SLS_UNCHANGED},
/* 1014 */  { addItemToBuffer,             CHR_tau,                     "",                                            STD_tau,                                                                      SLS_UNCHANGED},
/* 1015 */  { addItemToBuffer,             CHR_upsilon,                 "",                                            STD_upsilon,                                                                  SLS_UNCHANGED},
/* 1016 */  { addItemToBuffer,             CHR_upsilon_TONOS,           "",                                            STD_upsilon_TONOS,                                                            SLS_UNCHANGED},
/* 1017 */  { addItemToBuffer,             CHR_upsilon_DIALYTIKA,       "",                                            STD_upsilon_DIALYTIKA,                                                        SLS_UNCHANGED},
/* 1018 */  { addItemToBuffer,             CHR_upsilon_DIALYTIKA_TONOS, "",                                            STD_upsilon_DIALYTIKA_TONOS,                                                  SLS_UNCHANGED},
/* 1019 */  { addItemToBuffer,             CHR_phi,                     "",                                            STD_phi,                                                                      SLS_UNCHANGED},
/* 1020 */  { addItemToBuffer,             CHR_chi,                     "",                                            STD_chi,                                                                      SLS_UNCHANGED},
/* 1021 */  { addItemToBuffer,             CHR_psi,                     "",                                            STD_psi,                                                                      SLS_UNCHANGED},
/* 1022 */  { addItemToBuffer,             CHR_omega,                   "",                                            STD_omega,                                                                    SLS_UNCHANGED},
/* 1023 */  { addItemToBuffer,             CHR_omega_TONOS,             "",                                            STD_omega_TONOS,                                                              SLS_UNCHANGED},
/* 1024 */  { itemToBeCoded,               NOPARAM,                     "1024",                                        "1024",                                                                       SLS_UNCHANGED},
/* 1025 */  { itemToBeCoded,               NOPARAM,                     "1025",                                        "1025",                                                                       SLS_UNCHANGED},
/* 1026 */  { itemToBeCoded,               NOPARAM,                     "1026",                                        "1026",                                                                       SLS_UNCHANGED},
/* 1027 */  { itemToBeCoded,               NOPARAM,                     "1027",                                        "1027",                                                                       SLS_UNCHANGED},
/* 1028 */  { itemToBeCoded,               NOPARAM,                     "1028",                                        "1028",                                                                       SLS_UNCHANGED},
/* 1029 */  { itemToBeCoded,               NOPARAM,                     "1029",                                        "1029",                                                                       SLS_UNCHANGED},
/* 1030 */  { addItemToBuffer,             CHR_A_MACRON,                "",                                            STD_A_MACRON,                                                                 SLS_UNCHANGED},
/* 1031 */  { addItemToBuffer,             CHR_A_ACUTE,                 "",                                            STD_A_ACUTE,                                                                  SLS_UNCHANGED},
/* 1032 */  { addItemToBuffer,             CHR_A_BREVE,                 "",                                            STD_A_BREVE,                                                                  SLS_UNCHANGED},
/* 1033 */  { addItemToBuffer,             CHR_A_GRAVE,                 "",                                            STD_A_GRAVE,                                                                  SLS_UNCHANGED},
/* 1034 */  { addItemToBuffer,             CHR_A_DIARESIS,              "",                                            STD_A_DIARESIS,                                                               SLS_UNCHANGED},
/* 1035 */  { addItemToBuffer,             CHR_A_TILDE,                 "",                                            STD_A_TILDE,                                                                  SLS_UNCHANGED},
/* 1036 */  { addItemToBuffer,             CHR_A_CIRC,                  "",                                            STD_A_CIRC,                                                                   SLS_UNCHANGED},
/* 1037 */  { addItemToBuffer,             CHR_A_RING,                  "",                                            STD_A_RING,                                                                   SLS_UNCHANGED},
/* 1038 */  { addItemToBuffer,             CHR_AE,                      "",                                            STD_AE,                                                                       SLS_UNCHANGED},
/* 1039 */  { addItemToBuffer,             CHR_A_OGONEK,                "",                                            STD_A_OGONEK,                                                                 SLS_UNCHANGED},
/* 1040 */  { addItemToBuffer,             CHR_C_ACUTE,                 "",                                            STD_C_ACUTE,                                                                  SLS_UNCHANGED},
/* 1041 */  { addItemToBuffer,             CHR_C_CARON,                 "",                                            STD_C_CARON,                                                                  SLS_UNCHANGED},
/* 1042 */  { addItemToBuffer,             CHR_C_CEDILLA,               "",                                            STD_C_CEDILLA,                                                                SLS_UNCHANGED},
/* 1043 */  { addItemToBuffer,             CHR_D_STROKE,                "",                                            STD_D_STROKE,                                                                 SLS_UNCHANGED},
/* 1044 */  { addItemToBuffer,             CHR_D_CARON,                 "",                                            STD_D_CARON,                                                                  SLS_UNCHANGED},
/* 1045 */  { addItemToBuffer,             CHR_E_MACRON,                "",                                            STD_E_MACRON,                                                                 SLS_UNCHANGED},
/* 1046 */  { addItemToBuffer,             CHR_E_ACUTE,                 "",                                            STD_E_ACUTE,                                                                  SLS_UNCHANGED},
/* 1047 */  { addItemToBuffer,             CHR_E_BREVE,                 "",                                            STD_E_BREVE,                                                                  SLS_UNCHANGED},
/* 1048 */  { addItemToBuffer,             CHR_E_GRAVE,                 "",                                            STD_E_GRAVE,                                                                  SLS_UNCHANGED},
/* 1049 */  { addItemToBuffer,             CHR_E_DIARESIS,              "",                                            STD_E_DIARESIS,                                                               SLS_UNCHANGED},
/* 1050 */  { addItemToBuffer,             CHR_E_CIRC,                  "",                                            STD_E_CIRC,                                                                   SLS_UNCHANGED},
/* 1051 */  { addItemToBuffer,             CHR_E_OGONEK,                "",                                            STD_E_OGONEK,                                                                 SLS_UNCHANGED},
/* 1052 */  { addItemToBuffer,             CHR_G_BREVE,                 "",                                            STD_G_BREVE,                                                                  SLS_UNCHANGED},
/* 1053 */  { itemToBeCoded,               NOPARAM,                     "1053",                                        "1053",                                                                       SLS_UNCHANGED},
/* 1054 */  { addItemToBuffer,             CHR_I_MACRON,                "",                                            STD_I_MACRON,                                                                 SLS_UNCHANGED},
/* 1055 */  { addItemToBuffer,             CHR_I_ACUTE,                 "",                                            STD_I_ACUTE,                                                                  SLS_UNCHANGED},
/* 1056 */  { addItemToBuffer,             CHR_I_BREVE,                 "",                                            STD_I_BREVE,                                                                  SLS_UNCHANGED},
/* 1057 */  { addItemToBuffer,             CHR_I_GRAVE,                 "",                                            STD_I_GRAVE,                                                                  SLS_UNCHANGED},
/* 1058 */  { addItemToBuffer,             CHR_I_DIARESIS,              "",                                            STD_I_DIARESIS,                                                               SLS_UNCHANGED},
/* 1059 */  { addItemToBuffer,             CHR_I_CIRC,                  "",                                            STD_I_CIRC,                                                                   SLS_UNCHANGED},
/* 1060 */  { addItemToBuffer,             CHR_I_OGONEK,                "",                                            STD_I_OGONEK,                                                                 SLS_UNCHANGED},
/* 1061 */  { addItemToBuffer,             CHR_I_DOT,                   "",                                            STD_I_DOT,                                                                    SLS_UNCHANGED},
/* 1062 */  { addItemToBuffer,             CHR_I_DOTLESS,               "",                                            "I",                                                                          SLS_UNCHANGED},
/* 1063 */  { addItemToBuffer,             CHR_L_STROKE,                "",                                            STD_L_STROKE,                                                                 SLS_UNCHANGED},
/* 1064 */  { addItemToBuffer,             CHR_L_ACUTE,                 "",                                            STD_L_ACUTE,                                                                  SLS_UNCHANGED},
/* 1065 */  { addItemToBuffer,             CHR_L_APOSTROPHE,            "",                                            STD_L_APOSTROPHE,                                                             SLS_UNCHANGED},
/* 1066 */  { addItemToBuffer,             CHR_N_ACUTE,                 "",                                            STD_N_ACUTE,                                                                  SLS_UNCHANGED},
/* 1067 */  { addItemToBuffer,             CHR_N_CARON,                 "",                                            STD_N_CARON,                                                                  SLS_UNCHANGED},
/* 1068 */  { addItemToBuffer,             CHR_N_TILDE,                 "",                                            STD_N_TILDE,                                                                  SLS_UNCHANGED},
/* 1069 */  { addItemToBuffer,             CHR_O_MACRON,                "",                                            STD_O_MACRON,                                                                 SLS_UNCHANGED},
/* 1070 */  { addItemToBuffer,             CHR_O_ACUTE,                 "",                                            STD_O_ACUTE,                                                                  SLS_UNCHANGED},
/* 1071 */  { addItemToBuffer,             CHR_O_BREVE,                 "",                                            STD_O_BREVE,                                                                  SLS_UNCHANGED},
/* 1072 */  { addItemToBuffer,             CHR_O_GRAVE,                 "",                                            STD_O_GRAVE,                                                                  SLS_UNCHANGED},
/* 1073 */  { addItemToBuffer,             CHR_O_DIARESIS,              "",                                            STD_O_DIARESIS,                                                               SLS_UNCHANGED},
/* 1074 */  { addItemToBuffer,             CHR_O_TILDE,                 "",                                            STD_O_TILDE,                                                                  SLS_UNCHANGED},
/* 1075 */  { addItemToBuffer,             CHR_O_CIRC,                  "",                                            STD_O_CIRC,                                                                   SLS_UNCHANGED},
/* 1076 */  { addItemToBuffer,             CHR_O_STROKE,                "",                                            STD_O_STROKE,                                                                 SLS_UNCHANGED},
/* 1077 */  { addItemToBuffer,             CHR_OE,                      "",                                            STD_OE,                                                                       SLS_UNCHANGED},
/* 1078 */  { itemToBeCoded,               NOPARAM,                     "1078",                                        "1078",                                                                       SLS_UNCHANGED},
/* 1079 */  { itemToBeCoded,               NOPARAM,                     "1079",                                        "1079",                                                                       SLS_UNCHANGED},
/* 1080 */  { itemToBeCoded,               NOPARAM,                     "1080",                                        "1080",                                                                       SLS_UNCHANGED},
/* 1081 */  { addItemToBuffer,             CHR_S_ACUTE,                 "",                                            STD_S_ACUTE,                                                                  SLS_UNCHANGED},
/* 1082 */  { addItemToBuffer,             CHR_S_CARON,                 "",                                            STD_S_CARON,                                                                  SLS_UNCHANGED},
/* 1083 */  { addItemToBuffer,             CHR_S_CEDILLA,               "",                                            STD_S_CEDILLA,                                                                SLS_UNCHANGED},
/* 1084 */  { addItemToBuffer,             CHR_T_CARON,                 "",                                            STD_T_CARON,                                                                  SLS_UNCHANGED},
/* 1085 */  { addItemToBuffer,             CHR_T_CEDILLA,               "",                                            STD_T_CEDILLA,                                                                SLS_UNCHANGED},
/* 1086 */  { addItemToBuffer,             CHR_U_MACRON,                "",                                            STD_U_MACRON,                                                                 SLS_UNCHANGED},
/* 1087 */  { addItemToBuffer,             CHR_U_ACUTE,                 "",                                            STD_U_ACUTE,                                                                  SLS_UNCHANGED},
/* 1088 */  { addItemToBuffer,             CHR_U_BREVE,                 "",                                            STD_U_BREVE,                                                                  SLS_UNCHANGED},
/* 1089 */  { addItemToBuffer,             CHR_U_GRAVE,                 "",                                            STD_U_GRAVE,                                                                  SLS_UNCHANGED},
/* 1090 */  { addItemToBuffer,             CHR_U_DIARESIS,              "",                                            STD_U_DIARESIS,                                                               SLS_UNCHANGED},
/* 1091 */  { addItemToBuffer,             CHR_U_TILDE,                 "",                                            STD_U_TILDE,                                                                  SLS_UNCHANGED},
/* 1092 */  { addItemToBuffer,             CHR_U_CIRC,                  "",                                            STD_U_CIRC,                                                                   SLS_UNCHANGED},
/* 1093 */  { addItemToBuffer,             CHR_U_RING,                  "",                                            STD_U_RING,                                                                   SLS_UNCHANGED},
/* 1094 */  { addItemToBuffer,             CHR_W_CIRC,                  "",                                            STD_W_CIRC,                                                                   SLS_UNCHANGED},
/* 1095 */  { itemToBeCoded,               NOPARAM,                     "1095",                                        "1095",                                                                       SLS_UNCHANGED},
/* 1096 */  { itemToBeCoded,               NOPARAM,                     "1096",                                        "1096",                                                                       SLS_UNCHANGED},
/* 1097 */  { itemToBeCoded,               NOPARAM,                     "1097",                                        "1097",                                                                       SLS_UNCHANGED},
/* 1098 */  { addItemToBuffer,             CHR_Y_CIRC,                  "",                                            STD_Y_CIRC,                                                                   SLS_UNCHANGED},
/* 1099 */  { addItemToBuffer,             CHR_Y_ACUTE,                 "",                                            STD_Y_ACUTE,                                                                  SLS_UNCHANGED},
/* 1100 */  { addItemToBuffer,             CHR_Y_DIARESIS,              "",                                            STD_Y_DIARESIS,                                                               SLS_UNCHANGED},
/* 1101 */  { addItemToBuffer,             CHR_Z_ACUTE,                 "",                                            STD_Z_ACUTE,                                                                  SLS_UNCHANGED},
/* 1102 */  { addItemToBuffer,             CHR_Z_CARON,                 "",                                            STD_Z_CARON,                                                                  SLS_UNCHANGED},
/* 1103 */  { addItemToBuffer,             CHR_Z_DOT,                   "",                                            STD_Z_DOT,                                                                    SLS_UNCHANGED},
/* 1104 */  { itemToBeCoded,               NOPARAM,                     "1104",                                        "1104",                                                                       SLS_UNCHANGED},
/* 1105 */  { itemToBeCoded,               NOPARAM,                     "1105",                                        "1105",                                                                       SLS_UNCHANGED},
/* 1106 */  { itemToBeCoded,               NOPARAM,                     "1106",                                        "1106",                                                                       SLS_UNCHANGED},
/* 1107 */  { itemToBeCoded,               NOPARAM,                     "1107",                                        "1107",                                                                       SLS_UNCHANGED},
/* 1108 */  { itemToBeCoded,               NOPARAM,                     "1108",                                        "1108",                                                                       SLS_UNCHANGED},
/* 1109 */  { itemToBeCoded,               NOPARAM,                     "1109",                                        "1109",                                                                       SLS_UNCHANGED},
/* 1110 */  { addItemToBuffer,             CHR_a_MACRON,                "",                                            STD_a_MACRON,                                                                 SLS_UNCHANGED},
/* 1111 */  { addItemToBuffer,             CHR_a_ACUTE,                 "",                                            STD_a_ACUTE,                                                                  SLS_UNCHANGED},
/* 1112 */  { addItemToBuffer,             CHR_a_BREVE,                 "",                                            STD_a_BREVE,                                                                  SLS_UNCHANGED},
/* 1113 */  { addItemToBuffer,             CHR_a_GRAVE,                 "",                                            STD_a_GRAVE,                                                                  SLS_UNCHANGED},
/* 1114 */  { addItemToBuffer,             CHR_a_DIARESIS,              "",                                            STD_a_DIARESIS,                                                               SLS_UNCHANGED},
/* 1115 */  { addItemToBuffer,             CHR_a_TILDE,                 "",                                            STD_a_TILDE,                                                                  SLS_UNCHANGED},
/* 1116 */  { addItemToBuffer,             CHR_a_CIRC,                  "",                                            STD_a_CIRC,                                                                   SLS_UNCHANGED},
/* 1117 */  { addItemToBuffer,             CHR_a_RING,                  "",                                            STD_a_RING,                                                                   SLS_UNCHANGED},
/* 1118 */  { addItemToBuffer,             CHR_ae,                      "",                                            STD_ae,                                                                       SLS_UNCHANGED},
/* 1119 */  { addItemToBuffer,             CHR_a_OGONEK,                "",                                            STD_a_OGONEK,                                                                 SLS_UNCHANGED},
/* 1120 */  { addItemToBuffer,             CHR_c_ACUTE,                 "",                                            STD_c_ACUTE,                                                                  SLS_UNCHANGED},
/* 1121 */  { addItemToBuffer,             CHR_c_CARON,                 "",                                            STD_c_CARON,                                                                  SLS_UNCHANGED},
/* 1122 */  { addItemToBuffer,             CHR_c_CEDILLA,               "",                                            STD_c_CEDILLA,                                                                SLS_UNCHANGED},
/* 1123 */  { addItemToBuffer,             CHR_d_STROKE,                "",                                            STD_d_STROKE,                                                                 SLS_UNCHANGED},
/* 1124 */  { addItemToBuffer,             CHR_d_APOSTROPHE,            "",                                            STD_d_APOSTROPHE,                                                             SLS_UNCHANGED},
/* 1125 */  { addItemToBuffer,             CHR_e_MACRON,                "",                                            STD_e_MACRON,                                                                 SLS_UNCHANGED},
/* 1126 */  { addItemToBuffer,             CHR_e_ACUTE,                 "",                                            STD_e_ACUTE,                                                                  SLS_UNCHANGED},
/* 1127 */  { addItemToBuffer,             CHR_e_BREVE,                 "",                                            STD_e_BREVE,                                                                  SLS_UNCHANGED},
/* 1128 */  { addItemToBuffer,             CHR_e_GRAVE,                 "",                                            STD_e_GRAVE,                                                                  SLS_UNCHANGED},
/* 1129 */  { addItemToBuffer,             CHR_e_DIARESIS,              "",                                            STD_e_DIARESIS,                                                               SLS_UNCHANGED},
/* 1130 */  { addItemToBuffer,             CHR_e_CIRC,                  "",                                            STD_e_CIRC,                                                                   SLS_UNCHANGED},
/* 1131 */  { addItemToBuffer,             CHR_e_OGONEK,                "",                                            STD_e_OGONEK,                                                                 SLS_UNCHANGED},
/* 1132 */  { addItemToBuffer,             CHR_g_BREVE,                 "",                                            STD_g_BREVE,                                                                  SLS_UNCHANGED},
/* 1133 */  { addItemToBuffer,             CHR_h_STROKE,                "",                                            STD_h_STROKE,                                                                 SLS_UNCHANGED},
/* 1134 */  { addItemToBuffer,             CHR_i_MACRON,                "",                                            STD_i_MACRON,                                                                 SLS_UNCHANGED},
/* 1135 */  { addItemToBuffer,             CHR_i_ACUTE,                 "",                                            STD_i_ACUTE,                                                                  SLS_UNCHANGED},
/* 1136 */  { addItemToBuffer,             CHR_i_BREVE,                 "",                                            STD_i_BREVE,                                                                  SLS_UNCHANGED},
/* 1137 */  { addItemToBuffer,             CHR_i_GRAVE,                 "",                                            STD_i_GRAVE,                                                                  SLS_UNCHANGED},
/* 1138 */  { addItemToBuffer,             CHR_i_DIARESIS,              "",                                            STD_i_DIARESIS,                                                               SLS_UNCHANGED},
/* 1139 */  { addItemToBuffer,             CHR_i_CIRC,                  "",                                            STD_i_CIRC,                                                                   SLS_UNCHANGED},
/* 1140 */  { addItemToBuffer,             CHR_i_OGONEK,                "",                                            STD_i_OGONEK,                                                                 SLS_UNCHANGED},
/* 1141 */  { addItemToBuffer,             CHR_i_DOT,                   "",                                            "i",                                                                          SLS_UNCHANGED},
/* 1142 */  { addItemToBuffer,             CHR_i_DOTLESS,               "",                                            STD_i_DOTLESS,                                                                SLS_UNCHANGED},
/* 1143 */  { addItemToBuffer,             CHR_l_STROKE,                "",                                            STD_l_STROKE,                                                                 SLS_UNCHANGED},
/* 1144 */  { addItemToBuffer,             CHR_l_ACUTE,                 "",                                            STD_l_ACUTE,                                                                  SLS_UNCHANGED},
/* 1145 */  { addItemToBuffer,             CHR_l_APOSTROPHE,            "",                                            STD_l_APOSTROPHE,                                                             SLS_UNCHANGED},
/* 1146 */  { addItemToBuffer,             CHR_n_ACUTE,                 "",                                            STD_n_ACUTE,                                                                  SLS_UNCHANGED},
/* 1147 */  { addItemToBuffer,             CHR_n_CARON,                 "",                                            STD_n_CARON,                                                                  SLS_UNCHANGED},
/* 1148 */  { addItemToBuffer,             CHR_n_TILDE,                 "",                                            STD_n_TILDE,                                                                  SLS_UNCHANGED},
/* 1149 */  { addItemToBuffer,             CHR_o_MACRON,                "",                                            STD_o_MACRON,                                                                 SLS_UNCHANGED},
/* 1150 */  { addItemToBuffer,             CHR_o_ACUTE,                 "",                                            STD_o_ACUTE,                                                                  SLS_UNCHANGED},
/* 1151 */  { addItemToBuffer,             CHR_o_BREVE,                 "",                                            STD_o_BREVE,                                                                  SLS_UNCHANGED},
/* 1152 */  { addItemToBuffer,             CHR_o_GRAVE,                 "",                                            STD_o_GRAVE,                                                                  SLS_UNCHANGED},
/* 1153 */  { addItemToBuffer,             CHR_o_DIARESIS,              "",                                            STD_o_DIARESIS,                                                               SLS_UNCHANGED},
/* 1154 */  { addItemToBuffer,             CHR_o_TILDE,                 "",                                            STD_o_TILDE,                                                                  SLS_UNCHANGED},
/* 1155 */  { addItemToBuffer,             CHR_o_CIRC,                  "",                                            STD_o_CIRC,                                                                   SLS_UNCHANGED},
/* 1156 */  { addItemToBuffer,             CHR_o_STROKE,                "",                                            STD_o_STROKE,                                                                 SLS_UNCHANGED},
/* 1157 */  { addItemToBuffer,             CHR_oe,                      "",                                            STD_oe,                                                                       SLS_UNCHANGED},
/* 1158 */  { addItemToBuffer,             CHR_r_CARON,                 "",                                            STD_r_CARON,                                                                  SLS_UNCHANGED},
/* 1159 */  { addItemToBuffer,             CHR_r_ACUTE,                 "",                                            STD_r_ACUTE,                                                                  SLS_UNCHANGED},
/* 1160 */  { addItemToBuffer,             CHR_s_SHARP,                 "",                                            STD_s_SHARP,                                                                  SLS_UNCHANGED},
/* 1161 */  { addItemToBuffer,             CHR_s_ACUTE,                 "",                                            STD_s_ACUTE,                                                                  SLS_UNCHANGED},
/* 1162 */  { addItemToBuffer,             CHR_s_CARON,                 "",                                            STD_s_CARON,                                                                  SLS_UNCHANGED},
/* 1163 */  { addItemToBuffer,             CHR_s_CEDILLA,               "",                                            STD_s_CEDILLA,                                                                SLS_UNCHANGED},
/* 1164 */  { addItemToBuffer,             CHR_t_APOSTROPHE,            "",                                            STD_t_APOSTROPHE,                                                             SLS_UNCHANGED},
/* 1165 */  { addItemToBuffer,             CHR_t_CEDILLA,               "",                                            STD_t_CEDILLA,                                                                SLS_UNCHANGED},
/* 1166 */  { addItemToBuffer,             CHR_u_MACRON,                "",                                            STD_u_MACRON,                                                                 SLS_UNCHANGED},
/* 1167 */  { addItemToBuffer,             CHR_u_ACUTE,                 "",                                            STD_u_ACUTE,                                                                  SLS_UNCHANGED},
/* 1168 */  { addItemToBuffer,             CHR_u_BREVE,                 "",                                            STD_u_BREVE,                                                                  SLS_UNCHANGED},
/* 1169 */  { addItemToBuffer,             CHR_u_GRAVE,                 "",                                            STD_u_GRAVE,                                                                  SLS_UNCHANGED},
/* 1170 */  { addItemToBuffer,             CHR_u_DIARESIS,              "",                                            STD_u_DIARESIS,                                                               SLS_UNCHANGED},
/* 1171 */  { addItemToBuffer,             CHR_u_TILDE,                 "",                                            STD_u_TILDE,                                                                  SLS_UNCHANGED},
/* 1172 */  { addItemToBuffer,             CHR_u_CIRC,                  "",                                            STD_u_CIRC,                                                                   SLS_UNCHANGED},
/* 1173 */  { addItemToBuffer,             CHR_u_RING,                  "",                                            STD_u_RING,                                                                   SLS_UNCHANGED},
/* 1174 */  { addItemToBuffer,             CHR_w_CIRC,                  "",                                            STD_w_CIRC,                                                                   SLS_UNCHANGED},
/* 1175 */  { addItemToBuffer,             CHR_x_BAR,                   "",                                            STD_x_BAR,                                                                    SLS_UNCHANGED},
/* 1176 */  { addItemToBuffer,             CHR_x_CIRC,                  "",                                            STD_x_CIRC,                                                                   SLS_UNCHANGED},
/* 1177 */  { addItemToBuffer,             CHR_y_BAR,                   "",                                            STD_y_BAR,                                                                    SLS_UNCHANGED},
/* 1178 */  { addItemToBuffer,             CHR_y_CIRC,                  "",                                            STD_y_CIRC,                                                                   SLS_UNCHANGED},
/* 1179 */  { addItemToBuffer,             CHR_y_ACUTE,                 "",                                            STD_y_ACUTE,                                                                  SLS_UNCHANGED},
/* 1180 */  { addItemToBuffer,             CHR_y_DIARESIS,              "",                                            STD_y_DIARESIS,                                                               SLS_UNCHANGED},
/* 1181 */  { addItemToBuffer,             CHR_z_ACUTE,                 "",                                            STD_z_ACUTE,                                                                  SLS_UNCHANGED},
/* 1182 */  { addItemToBuffer,             CHR_z_CARON,                 "",                                            STD_z_CARON,                                                                  SLS_UNCHANGED},
/* 1183 */  { addItemToBuffer,             CHR_z_DOT,                   "",                                            STD_z_DOT,                                                                    SLS_UNCHANGED},
/* 1184 */  { itemToBeCoded,               NOPARAM,                     "1184",                                        "1184",                                                                       SLS_UNCHANGED},
/* 1185 */  { itemToBeCoded,               NOPARAM,                     "1185",                                        "1185",                                                                       SLS_UNCHANGED},
/* 1186 */  { itemToBeCoded,               NOPARAM,                     "1186",                                        "1186",                                                                       SLS_UNCHANGED},
/* 1187 */  { itemToBeCoded,               NOPARAM,                     "1187",                                        "1187",                                                                       SLS_UNCHANGED},
/* 1188 */  { itemToBeCoded,               NOPARAM,                     "1188",                                        "1188",                                                                       SLS_UNCHANGED},
/* 1189 */  { itemToBeCoded,               NOPARAM,                     "1189",                                        "1189",                                                                       SLS_UNCHANGED},
/* 1190 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_alpha,                                                                SLS_UNCHANGED},
/* 1191 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_delta,                                                                SLS_UNCHANGED},
/* 1192 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_mu,                                                                   SLS_UNCHANGED},
/* 1193 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_SUN,                                                                  SLS_UNCHANGED},
/* 1194 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_SUN_b,                                                                SLS_UNCHANGED},
/* 1195 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_EARTH,                                                                SLS_UNCHANGED},
/* 1196 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_EARTH_b,                                                              SLS_UNCHANGED},
/* 1197 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_PLUS,                                                                 SLS_UNCHANGED},
/* 1198 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_MINUS,                                                                SLS_UNCHANGED},
/* 1199 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_INFINITY,                                                             SLS_UNCHANGED},
/* 1200 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_0,                                                                    SLS_UNCHANGED},
/* 1201 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_1,                                                                    SLS_UNCHANGED},
/* 1202 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_2,                                                                    SLS_UNCHANGED},
/* 1203 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_3,                                                                    SLS_UNCHANGED},
/* 1204 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_4,                                                                    SLS_UNCHANGED},
/* 1205 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_5,                                                                    SLS_UNCHANGED},
/* 1206 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_6,                                                                    SLS_UNCHANGED},
/* 1207 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_7,                                                                    SLS_UNCHANGED},
/* 1208 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_8,                                                                    SLS_UNCHANGED},
/* 1209 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_9,                                                                    SLS_UNCHANGED},
/* 1210 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_10,                                                                   SLS_UNCHANGED},
/* 1211 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_A,                                                                    SLS_UNCHANGED},
/* 1212 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_B,                                                                    SLS_UNCHANGED},
/* 1213 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_C,                                                                    SLS_UNCHANGED},
/* 1214 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_D,                                                                    SLS_UNCHANGED},
/* 1215 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_E,                                                                    SLS_UNCHANGED},
/* 1216 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_F,                                                                    SLS_UNCHANGED},
/* 1217 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_G,                                                                    SLS_UNCHANGED},
/* 1218 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_H,                                                                    SLS_UNCHANGED},
/* 1219 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_I,                                                                    SLS_UNCHANGED},
/* 1220 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_J,                                                                    SLS_UNCHANGED},
/* 1221 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_K,                                                                    SLS_UNCHANGED},
/* 1222 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_L,                                                                    SLS_UNCHANGED},
/* 1223 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_M,                                                                    SLS_UNCHANGED},
/* 1224 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_N,                                                                    SLS_UNCHANGED},
/* 1225 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_O,                                                                    SLS_UNCHANGED},
/* 1226 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_P,                                                                    SLS_UNCHANGED},
/* 1227 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_Q,                                                                    SLS_UNCHANGED},
/* 1228 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_R,                                                                    SLS_UNCHANGED},
/* 1229 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_S,                                                                    SLS_UNCHANGED},
/* 1230 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_T,                                                                    SLS_UNCHANGED},
/* 1231 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_U,                                                                    SLS_UNCHANGED},
/* 1232 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_V,                                                                    SLS_UNCHANGED},
/* 1233 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_W,                                                                    SLS_UNCHANGED},
/* 1234 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_X,                                                                    SLS_UNCHANGED},
/* 1235 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_Y,                                                                    SLS_UNCHANGED},
/* 1236 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_Z,                                                                    SLS_UNCHANGED},
/* 1237 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_E_OUTLINE,                                                            SLS_UNCHANGED},
/* 1238 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_a,                                                                    SLS_UNCHANGED},
/* 1239 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_b,                                                                    SLS_UNCHANGED},
/* 1240 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_c,                                                                    SLS_UNCHANGED},
/* 1241 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_d,                                                                    SLS_UNCHANGED},
/* 1242 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_e,                                                                    SLS_UNCHANGED},
/* 1243 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_h,                                                                    SLS_UNCHANGED},
/* 1244 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_i,                                                                    SLS_UNCHANGED},
/* 1245 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_j,                                                                    SLS_UNCHANGED},
/* 1246 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_k,                                                                    SLS_UNCHANGED},
/* 1247 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_l,                                                                    SLS_UNCHANGED},
/* 1248 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_m,                                                                    SLS_UNCHANGED},
/* 1249 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_n,                                                                    SLS_UNCHANGED},
/* 1250 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_o,                                                                    SLS_UNCHANGED},
/* 1251 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_p,                                                                    SLS_UNCHANGED},
/* 1252 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_q,                                                                    SLS_UNCHANGED},
/* 1253 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_s,                                                                    SLS_UNCHANGED},
/* 1254 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_t,                                                                    SLS_UNCHANGED},
/* 1255 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_u,                                                                    SLS_UNCHANGED},
/* 1256 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_v,                                                                    SLS_UNCHANGED},
/* 1257 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_w,                                                                    SLS_UNCHANGED},
/* 1258 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_x,                                                                    SLS_UNCHANGED},
/* 1259 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_y,                                                                    SLS_UNCHANGED},
/* 1260 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_z,                                                                    SLS_UNCHANGED},
/* 1261 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_a_b,                                                                  SLS_UNCHANGED},
/* 1262 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_e_b,                                                                  SLS_UNCHANGED},
/* 1263 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_k_b,                                                                  SLS_UNCHANGED},
/* 1264 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_l_b,                                                                  SLS_UNCHANGED},
/* 1265 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_m_b,                                                                  SLS_UNCHANGED},
/* 1266 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_n_b,                                                                  SLS_UNCHANGED},
/* 1267 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_o_b,                                                                  SLS_UNCHANGED},
/* 1268 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_p_b,                                                                  SLS_UNCHANGED},
/* 1269 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_s_b,                                                                  SLS_UNCHANGED},
/* 1270 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_u_b,                                                                  SLS_UNCHANGED},
/* 1271 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_x_b,                                                                  SLS_UNCHANGED},
/* 1272 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_PLUS,                                                                 SLS_UNCHANGED},
/* 1273 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_MINUS,                                                                SLS_UNCHANGED},
/* 1274 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_MINUS_1,                                                              SLS_UNCHANGED},
/* 1275 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_INFINITY,                                                             SLS_UNCHANGED},
/* 1276 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_ASTERISK,                                                             SLS_UNCHANGED},
/* 1277 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_0,                                                                    SLS_UNCHANGED},
/* 1278 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_1,                                                                    SLS_UNCHANGED},
/* 1279 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_2,                                                                    SLS_UNCHANGED},
/* 1280 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_3,                                                                    SLS_UNCHANGED},
/* 1281 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_4,                                                                    SLS_UNCHANGED},
/* 1282 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_5,                                                                    SLS_UNCHANGED},
/* 1283 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_6,                                                                    SLS_UNCHANGED},
/* 1284 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_7,                                                                    SLS_UNCHANGED},
/* 1285 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_8,                                                                    SLS_UNCHANGED},
/* 1286 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_9,                                                                    SLS_UNCHANGED},
/* 1287 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_1_b,                                                                  SLS_UNCHANGED},
/* 1288 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_T,                                                                    SLS_UNCHANGED},
/* 1289 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_a,                                                                    SLS_UNCHANGED},
/* 1290 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_f,                                                                    SLS_UNCHANGED},
/* 1291 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_g,                                                                    SLS_UNCHANGED},
/* 1292 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_h,                                                                    SLS_UNCHANGED},
/* 1293 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_r,                                                                    SLS_UNCHANGED},
/* 1294 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_x,                                                                    SLS_UNCHANGED},
/* 1295 */  { addItemToBuffer,             CHR_SPACE,                   "",                                            STD_SPACE,                                                                    SLS_UNCHANGED},
/* 1296 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_EXCLAMATION_MARK,                                                         SLS_UNCHANGED},
/* 1297 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOUBLE_QUOTE,                                                             SLS_UNCHANGED},
/* 1298 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NUMBER_SIGN,                                                              SLS_UNCHANGED},
/* 1299 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOLLAR,                                                                   SLS_UNCHANGED},
/* 1300 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PERCENT,                                                                  SLS_UNCHANGED},
/* 1301 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_AMPERSAND,                                                                SLS_UNCHANGED},
/* 1302 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_QUOTE,                                                                    SLS_UNCHANGED},
/* 1303 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LEFT_PARENTHESIS,                                                         SLS_UNCHANGED},
/* 1304 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RIGHT_PARENTHESIS,                                                        SLS_UNCHANGED},
/* 1305 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ASTERISK,                                                                 SLS_UNCHANGED},
/* 1306 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ASTERISK_b,                                                               SLS_UNCHANGED},
/* 1307 */  { addItemToBuffer,             CHR_PLUS,                    "",                                            STD_PLUS,                                                                     SLS_UNCHANGED},
/* 1308 */  { addItemToBuffer,             CHR_COMMA,                   "",                                            STD_COMMA,                                                                    SLS_UNCHANGED},
/* 1309 */  { addItemToBuffer,             CHR_MINUS,                   "",                                            STD_MINUS,                                                                    SLS_UNCHANGED},
/* 1310 */  { addItemToBuffer,             CHR_PERIOD,                  "",                                            STD_PERIOD,                                                                   SLS_UNCHANGED},
/* 1311 */  { addItemToBuffer,             CHR_SLASH,                   "",                                            STD_SLASH,                                                                    SLS_UNCHANGED},
/* 1312 */  { addItemToBuffer,             CHR_COLON,                   "",                                            STD_COLON,                                                                    SLS_UNCHANGED},  //JM ADDED TEXT
/* 1313 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SEMICOLON,                                                                SLS_UNCHANGED},
/* 1314 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LESS_THAN,                                                                SLS_UNCHANGED},
/* 1315 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_EQUAL,                                                                    SLS_UNCHANGED},
/* 1316 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_GREATER_THAN,                                                             SLS_UNCHANGED},
/* 1317 */  { addItemToBuffer,             CHR_QUESTION_MARK,           "",                                            STD_QUESTION_MARK,                                                            SLS_UNCHANGED},
/* 1318 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_AT,                                                                       SLS_UNCHANGED},
/* 1319 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LEFT_SQUARE_BRACKET,                                                      SLS_UNCHANGED},
/* 1320 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_BACK_SLASH,                                                               SLS_UNCHANGED},
/* 1321 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RIGHT_SQUARE_BRACKET,                                                     SLS_UNCHANGED},
/* 1322 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_CIRCUMFLEX,                                                               SLS_UNCHANGED},
/* 1323 */  { addItemToBuffer,               CHR_UNDERSCORE,                     "",                                            STD_UNDERSCORE,                                                               SLS_UNCHANGED},  //JM TEXT
/* 1324 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LEFT_CURLY_BRACKET,                                                       SLS_UNCHANGED},
/* 1325 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PIPE,                                                                     SLS_UNCHANGED},
/* 1326 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RIGHT_CURLY_BRACKET,                                                      SLS_UNCHANGED},
/* 1327 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_TILDE,                                                                    SLS_UNCHANGED},
/* 1328 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_INVERTED_EXCLAMATION_MARK,                                                SLS_UNCHANGED},
/* 1329 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_CENT,                                                                     SLS_UNCHANGED},
/* 1330 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_POUND,                                                                    SLS_UNCHANGED},
/* 1331 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_YEN,                                                                      SLS_UNCHANGED},
/* 1332 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SECTION,                                                                  SLS_UNCHANGED},
/* 1333 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OVERFLOW_CARRY,                                                           SLS_UNCHANGED},
/* 1334 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LEFT_DOUBLE_ANGLE,                                                        SLS_UNCHANGED},
/* 1335 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NOT,                                                                      SLS_UNCHANGED},
/* 1336 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DEGREE,                                                                   SLS_UNCHANGED},
/* 1337 */  { addItemToBuffer,             CHR_PLUS_MINUS,              "",                                            STD_PLUS_MINUS,                                                               SLS_UNCHANGED},
/* 1338 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_mu_b,                                                                     SLS_UNCHANGED},
/* 1339 */  { addItemToBuffer,             CHR_DOT,                     "",                                            STD_DOT,                                                                      SLS_UNCHANGED},
/* 1340 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ORDINAL,                                                                  SLS_UNCHANGED},
/* 1341 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RIGHT_DOUBLE_ANGLE,                                                       SLS_UNCHANGED},
/* 1342 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ONE_HALF,                                                                 SLS_UNCHANGED},
/* 1343 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ONE_QUARTER,                                                              SLS_UNCHANGED},
/* 1344 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_INVERTED_QUESTION_MARK,                                                   SLS_UNCHANGED},
/* 1345 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ETH,                                                                      SLS_UNCHANGED},
/* 1346 */  { addItemToBuffer,             CHR_CROSS,                   "",                                            STD_CROSS,                                                                    SLS_UNCHANGED},
/* 1347 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_eth,                                                                      SLS_UNCHANGED},
/* 1348 */  { addItemToBuffer,               CHR_DIVIDE,                     "",                                            STD_DIVIDE,                                                                   SLS_UNCHANGED},   //JM DIV
/* 1349 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_E_DOT,                                                                    SLS_UNCHANGED},
/* 1350 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_e_DOT,                                                                    SLS_UNCHANGED},
/* 1351 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_E_CARON,                                                                  SLS_UNCHANGED},
/* 1352 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_e_CARON,                                                                  SLS_UNCHANGED},
/* 1353 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_R_ACUTE,                                                                  SLS_UNCHANGED},
/* 1354 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_R_CARON,                                                                  SLS_UNCHANGED},
/* 1355 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_U_OGONEK,                                                                 SLS_UNCHANGED},
/* 1356 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_u_OGONEK,                                                                 SLS_UNCHANGED},
/* 1357 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_y_UNDER_ROOT,                                                             SLS_UNCHANGED},
/* 1358 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_x_UNDER_ROOT,                                                             SLS_UNCHANGED},
/* 1359 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_EM,                                                                 SLS_UNCHANGED},
/* 1360 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_3_PER_EM,                                                           SLS_UNCHANGED},
/* 1361 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_4_PER_EM,                                                           SLS_UNCHANGED},
/* 1362 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_6_PER_EM,                                                           SLS_UNCHANGED},
/* 1363 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_FIGURE,                                                             SLS_UNCHANGED},
/* 1364 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_PUNCTUATION,                                                        SLS_UNCHANGED},
/* 1365 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_HAIR,                                                               SLS_UNCHANGED},
/* 1366 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LEFT_SINGLE_QUOTE,                                                        SLS_UNCHANGED},
/* 1367 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RIGHT_SINGLE_QUOTE,                                                       SLS_UNCHANGED},
/* 1368 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SINGLE_LOW_QUOTE,                                                         SLS_UNCHANGED},
/* 1369 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SINGLE_HIGH_QUOTE,                                                        SLS_UNCHANGED},
/* 1370 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LEFT_DOUBLE_QUOTE,                                                        SLS_UNCHANGED},
/* 1371 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RIGHT_DOUBLE_QUOTE,                                                       SLS_UNCHANGED},
/* 1372 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOUBLE_LOW_QUOTE,                                                         SLS_UNCHANGED},
/* 1373 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOUBLE_HIGH_QUOTE,                                                        SLS_UNCHANGED},
/* 1374 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ELLIPSIS,                                                                 SLS_UNCHANGED},
/* 1375 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ONE,                                                                      SLS_UNCHANGED},
/* 1376 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_EURO,                                                                     SLS_UNCHANGED},
/* 1377 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_COMPLEX_C,                                                                SLS_UNCHANGED},
/* 1378 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PLANCK,                                                                   SLS_UNCHANGED},
/* 1379 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PLANCK_2PI,                                                               SLS_UNCHANGED},
/* 1380 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NATURAL_N,                                                                SLS_UNCHANGED},
/* 1381 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RATIONAL_Q,                                                               SLS_UNCHANGED},
/* 1382 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_REAL_R,                                                                   SLS_UNCHANGED},
/* 1383 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LEFT_ARROW,                                                               SLS_UNCHANGED},
/* 1384 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_UP_ARROW,                                                                 SLS_UNCHANGED},
/* 1385 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_UP_ARROW_b,                                                               SLS_UNCHANGED},
/* 1386 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RIGHT_ARROW,                                                              SLS_UNCHANGED},
/* 1387 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOWN_ARROW,                                                               SLS_UNCHANGED},
/* 1388 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOWN_ARROW_b,                                                             SLS_UNCHANGED},
/* 1389 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SERIAL_IO,                                                                SLS_UNCHANGED},
/* 1390 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RIGHT_SHORT_ARROW,                                                        SLS_UNCHANGED},
/* 1391 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LEFT_RIGHT_ARROWS,                                                        SLS_UNCHANGED},
/* 1392 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_BST,                                                                      SLS_UNCHANGED},
/* 1393 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SST,                                                                      SLS_UNCHANGED},
/* 1394 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_HAMBURGER,                                                                SLS_UNCHANGED},
/* 1395 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_UNDO,                                                                     SLS_UNCHANGED},
/* 1396 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_FOR_ALL,                                                                  SLS_UNCHANGED},
/* 1397 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_COMPLEMENT,                                                               SLS_UNCHANGED},
/* 1398 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PARTIAL_DIFF,                                                             SLS_UNCHANGED},
/* 1399 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_THERE_EXISTS,                                                             SLS_UNCHANGED},
/* 1400 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_THERE_DOES_NOT_EXIST,                                                     SLS_UNCHANGED},
/* 1401 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_EMPTY_SET,                                                                SLS_UNCHANGED},
/* 1402 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_INCREMENT,                                                                SLS_UNCHANGED},
/* 1403 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NABLA,                                                                    SLS_UNCHANGED},
/* 1404 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ELEMENT_OF,                                                               SLS_UNCHANGED},
/* 1405 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NOT_ELEMENT_OF,                                                           SLS_UNCHANGED},
/* 1406 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_CONTAINS,                                                                 SLS_UNCHANGED},
/* 1407 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOES_NOT_CONTAIN,                                                         SLS_UNCHANGED},
/* 1408 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ZERO,                                                                     SLS_UNCHANGED},
/* 1409 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PRODUCT,                                                                  SLS_UNCHANGED},
/* 1410 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUM,                                                                      SLS_UNCHANGED},
/* 1411 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MINUS_SIGN,                                                               SLS_UNCHANGED},
/* 1412 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MINUS_PLUS,                                                               SLS_UNCHANGED},
/* 1413 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DIVISION,                                                                 SLS_UNCHANGED},
/* 1414 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SET_MINUS,                                                                SLS_UNCHANGED},
/* 1415 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RING,                                                                     SLS_UNCHANGED},
/* 1416 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_BULLET,                                                                   SLS_UNCHANGED},
/* 1417 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SQUARE_ROOT,                                                              SLS_UNCHANGED},
/* 1418 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_CUBE_ROOT,                                                                SLS_UNCHANGED},
/* 1419 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_xTH_ROOT,                                                                 SLS_UNCHANGED},
/* 1420 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PROPORTIONAL,                                                             SLS_UNCHANGED},
/* 1421 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_INFINITY,                                                                 SLS_UNCHANGED},
/* 1422 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RIGHT_ANGLE,                                                              SLS_UNCHANGED},
/* 1423 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ANGLE,                                                                    SLS_UNCHANGED},
/* 1424 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MEASURED_ANGLE,                                                           SLS_UNCHANGED},
/* 1425 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DIVIDES,                                                                  SLS_UNCHANGED},
/* 1426 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOES_NOT_DIVIDE,                                                          SLS_UNCHANGED},
/* 1427 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PARALLEL,                                                                 SLS_UNCHANGED},
/* 1428 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NOT_PARALLEL,                                                             SLS_UNCHANGED},
/* 1429 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_AND,                                                                      SLS_UNCHANGED},
/* 1430 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OR,                                                                       SLS_UNCHANGED},
/* 1431 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_INTERSECTION,                                                             SLS_UNCHANGED},
/* 1432 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_UNION,                                                                    SLS_UNCHANGED},
/* 1433 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_INTEGRAL,                                                                 SLS_UNCHANGED},
/* 1434 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOUBLE_INTEGRAL,                                                          SLS_UNCHANGED},
/* 1435 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_CONTOUR_INTEGRAL,                                                         SLS_UNCHANGED},
/* 1436 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SURFACE_INTEGRAL,                                                         SLS_UNCHANGED},
/* 1437 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RATIO,                                                                    SLS_UNCHANGED},
/* 1438 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_TILDE_b,                                                                  SLS_UNCHANGED},
/* 1439 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ASYMPOTICALLY_EQUAL,                                                      SLS_UNCHANGED},
/* 1440 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ALMOST_EQUAL,                                                             SLS_UNCHANGED},
/* 1441 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_COLON_EQUALS,                                                             SLS_UNCHANGED},
/* 1442 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_CORRESPONDS_TO,                                                           SLS_UNCHANGED},
/* 1443 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ESTIMATES,                                                                SLS_UNCHANGED},
/* 1444 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NOT_EQUAL,                                                                SLS_UNCHANGED},
/* 1445 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_IDENTICAL_TO,                                                             SLS_UNCHANGED},
/* 1446 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LESS_EQUAL,                                                               SLS_UNCHANGED},
/* 1447 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_GREATER_EQUAL,                                                            SLS_UNCHANGED},
/* 1448 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MUCH_LESS,                                                                SLS_UNCHANGED},
/* 1449 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MUCH_GREATER,                                                             SLS_UNCHANGED},
/* 1450 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUN,                                                                      SLS_UNCHANGED},
/* 1451 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOWN_TACK,                                                                SLS_UNCHANGED},
/* 1452 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PERPENDICULAR,                                                            SLS_UNCHANGED},
/* 1453 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_XOR,                                                                      SLS_UNCHANGED},
/* 1454 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NAND,                                                                     SLS_UNCHANGED},
/* 1455 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NOR,                                                                      SLS_UNCHANGED},
/* 1456 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_WATCH,                                                                    SLS_UNCHANGED},
/* 1457 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_HOURGLASS,                                                                SLS_UNCHANGED},
/* 1458 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PRINTER,                                                                  SLS_UNCHANGED},
/* 1459 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_TL,                                                                   SLS_UNCHANGED},
/* 1460 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_ML,                                                                   SLS_UNCHANGED},
/* 1461 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_BL,                                                                   SLS_UNCHANGED},
/* 1462 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_TR,                                                                   SLS_UNCHANGED},
/* 1463 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_MR,                                                                   SLS_UNCHANGED},
/* 1464 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_BR,                                                                   SLS_UNCHANGED},
/* 1465 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OBLIQUE1,                                                                 SLS_UNCHANGED},
/* 1466 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OBLIQUE2,                                                                 SLS_UNCHANGED},
/* 1467 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OBLIQUE3,                                                                 SLS_UNCHANGED},
/* 1468 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OBLIQUE4,                                                                 SLS_UNCHANGED},
/* 1469 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_CURSOR,                                                                   SLS_UNCHANGED},
/* 1470 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PERIOD34,                                                                 SLS_UNCHANGED},
/* 1471 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_COMMA34,                                                                  SLS_UNCHANGED},
/* 1472 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_BATTERY,                                                                  SLS_UNCHANGED},
/* 1473 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PGM_BEGIN,                                                                SLS_UNCHANGED},
/* 1474 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_USER_MODE,                                                                SLS_UNCHANGED},
/* 1475 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_UK,                                                                       SLS_UNCHANGED},
/* 1476 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_US,                                                                       SLS_UNCHANGED},
/* 1477 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NEG_EXCLAMATION_MARK,                                                     SLS_UNCHANGED},
/* 1478 */  { addItemToBuffer,             CHR_ex,                      "",                                            STD_LEFT_RIGHT_ARROWS,                                                        SLS_UNCHANGED},
/* 1479 */  { addItemToBuffer,             ITM_Max,                     "",                                            "Max",                                                                        SLS_UNCHANGED},
/* 1480 */  { addItemToBuffer,             ITM_Min,                     "",                                            "Min",                                                                        SLS_UNCHANGED},
/* 1481 */  { addItemToBuffer,             ITM_Config,                  "",                                            "Config",                                                                     SLS_UNCHANGED},
/* 1482 */  { addItemToBuffer,             ITM_Stack,                   "",                                            "Stack",                                                                      SLS_UNCHANGED},
/* 1483 */  { addItemToBuffer,             ITM_dddEL,                   "",                                            STD_ELLIPSIS "EL",                                                            SLS_UNCHANGED},
/* 1484 */  { addItemToBuffer,             ITM_dddIJ,                   "",                                            STD_ELLIPSIS "IJ",                                                            SLS_UNCHANGED},
/* 1485 */  { itemToBeCoded,               NOPARAM,                     "",                                            "0.",                                                                         SLS_UNCHANGED},
/* 1486 */  { itemToBeCoded,               NOPARAM,                     "",                                            "1.",                                                                         SLS_UNCHANGED},
/* JM 1487 */  { addItemToBuffer,             ITM_EXPONENT,                "",                                            "EEX",                                                                          SLS_UNCHANGED}, //JM Change E to EEX
/* 1488 */  { addItemToBuffer,             ITM_MA11,                    "",                                            STD_a_DIARESIS,                                                               SLS_UNCHANGED},
/* 1489 */  { addItemToBuffer,             ITM_MA12,                    "",                                            STD_o_DIARESIS,                                                               SLS_UNCHANGED},
/* 1490 */  { addItemToBuffer,             ITM_MA13,                    "",                                            STD_u_DIARESIS,                                                               SLS_UNCHANGED},
/* 1491 */  { addItemToBuffer,             ITM_MA14,                    "",                                            "",                                                                           SLS_UNCHANGED},
/* 1492 */  { addItemToBuffer,             ITM_MA15,                    "",                                            "sch",                                                                        SLS_UNCHANGED},
/* 1493 */  { addItemToBuffer,             ITM_MA16,                    "",                                            STD_s_SHARP,                                                                  SLS_UNCHANGED},
/* 1494 */  { addItemToBuffer,             ITM_MA21,                    "",                                            "not",                                                                        SLS_UNCHANGED},
/* 1495 */  { addItemToBuffer,             ITM_MA22,                    "",                                            "edi-",                                                                       SLS_UNCHANGED},
/* 1496 */  { addItemToBuffer,             ITM_MA23,                    "",                                            "table",                                                                      SLS_UNCHANGED},
/* 1497 */  { addItemToBuffer,             ITM_MA24,                    "",                                            "for",                                                                        SLS_UNCHANGED},
/* 1498 */  { addItemToBuffer,             ITM_MA25,                    "",                                            "now",                                                                        SLS_UNCHANGED},
/* 1499 */  { addItemToBuffer,             ITM_MA26,                    "",                                            "",                                                                           SLS_UNCHANGED},
/* 1500 */  { addItemToBuffer,             ITM_MA31,                    "",                                            "",                                                                           SLS_UNCHANGED},
/* 1501 */  { addItemToBuffer,             ITM_MA32,                    "",                                            "",                                                                           SLS_UNCHANGED},
/* 1502 */  { addItemToBuffer,             ITM_MA33,                    "",                                            "",                                                                           SLS_UNCHANGED},
/* 1503 */  { addItemToBuffer,             ITM_MA34,                    "",                                            "",                                                                           SLS_UNCHANGED},
/* 1504 */  { addItemToBuffer,             ITM_MA35,                    "",                                            "",                                                                           SLS_UNCHANGED},
/* 1505 */  { addItemToBuffer,             ITM_MA36,                    "",                                            "",                                                                           SLS_UNCHANGED},
/* 1506 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_UNDO,                                                                     SLS_UNCHANGED},
/* JM 1507 */  { itemToBeCoded,               NOPARAM,                     "",                                            "PRGM",                                                                        SLS_UNCHANGED},  //JM Change P/R to PRGM
/* 1508 */  { itemToBeCoded,               NOPARAM,                     "",                                            "R/S",                                                                        SLS_UNCHANGED},
/* 1509 */  { itemToBeCoded,               NOPARAM,                     "",                                            "Not",                                                                        SLS_UNCHANGED},
/* 1510 */  { itemToBeCoded,               NOPARAM,                     "",                                            "yet",                                                                        SLS_UNCHANGED},
/* 1511 */  { itemToBeCoded,               NOPARAM,                     "",                                            "defined",                                                                    SLS_UNCHANGED},
/* 1512 */  { itemToBeCoded,               NOPARAM,                     "",                                            "Tam",                                                                        SLS_UNCHANGED},
/* 1513 */  { itemToBeCoded,               NOPARAM,                     "",                                            "TamCmp",                                                                     SLS_UNCHANGED},
/* 1514 */  { itemToBeCoded,               NOPARAM,                     "",                                            "TamStoRcl",                                                                  SLS_UNCHANGED},
/* 1515 */  { fnUserMode,                  NOPARAM,                     "",                                            "USER",                                                                       SLS_UNCHANGED},
/* 1516 */  { fnComplexCCCC_CC,               NOPARAM,                     "CC",                                            "CC",                                                                         SLS_UNCHANGED},   //JM Change CC to COMPLEX
/* 1517 */  { itemToBeCoded,               NOPARAM,                     "",                                            "f/g",                                                                          SLS_UNCHANGED},  //JM3 Changed f to f/g for DM42 keyboard
/* 1518 */  { itemToBeCoded,               NOPARAM,                     "",                                            "g",                                                                          SLS_UNCHANGED},
/* 1519 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_UP_ARROW,                                                                 SLS_UNCHANGED},
/* 1520 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_HAMBURGER STD_BST,                                                        SLS_UNCHANGED},
/* 1521 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOWN_ARROW,                                                               SLS_UNCHANGED},
/* 1522 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_HAMBURGER STD_SST,                                                        SLS_UNCHANGED},
/* 1523 */  { itemToBeCoded,               NOPARAM,                     "",                                            "EXIT",                                                                       SLS_UNCHANGED},
/* 1524 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LEFT_ARROW,                                                               SLS_UNCHANGED},
/* 1525 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PRINTER "x",                                                              SLS_UNCHANGED},
#ifdef JM_LAYOUT_2_DM42_STRICT //JM LAYOUT 2. DM42 STRICT. 
/* 1526 */  { fnAim,                       NOPARAM,                     "",                                            "ALPHA", /*STD_alpha,*/                                                                    SLS_UNCHANGED},             //JM
#endif //JM END OF LAYOUT 2 DM42 STRICT.
#ifdef JM_LAYOUT_1A
/* 1526 */  { fnAim,                       NOPARAM,                     "",                                            STD_alpha,                                                                    SLS_UNCHANGED},                             //JM
#endif //JM END OF LAYOUT 1
/* 1527 */  { itemToBeCoded,               NOPARAM,                     "",                                            ".d",                                                                         SLS_UNCHANGED},
/* 1528 */  { fnCvtQuartM3,                multiply,                    "qt." STD_RIGHT_ARROW "m" STD_SUP_3,           "qt." STD_RIGHT_ARROW "m" STD_SUP_3,                                          SLS_ENABLED  },
/* 1529 */  { fnCvtQuartM3,                divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "qt.",           "m" STD_SUP_3 STD_RIGHT_ARROW "qt.",                                          SLS_ENABLED  },
/* 1530 */  { fnConvertXToReal16,          NOPARAM,                     STD_RIGHT_ARROW "SP",                          STD_RIGHT_ARROW "SP",                                                         SLS_ENABLED  },
/* 1531 */  { itemToBeCoded,               NOPARAM,                     "SHOW",                                        "SHOW",                                                                       SLS_UNCHANGED},
/* 1532 */  { backToSystem,                NOPARAM,                     "SYSTEM",                                      "SYSTEM",                                                                     SLS_UNCHANGED},
/* 1533 */  { fnCvtDmsToDeg,               NOPARAM,                     "D.MS" STD_RIGHT_ARROW "D",                    "D.MS" STD_RIGHT_ARROW "D",                                                   SLS_ENABLED  },
/* 1534 */  { itemToBeCoded,               NOPARAM,                     "VANGLE",                                      "V" STD_MEASURED_ANGLE,                                                       SLS_ENABLED  },
/* 1535 */  { fnCvtFathomM,                multiply,                    "fm." STD_RIGHT_ARROW "m",                     "fathom",                                                                     SLS_ENABLED  },
/* 1536 */  { fnCvtFathomM,                multiply,                    "fm." STD_RIGHT_ARROW "m",                     STD_RIGHT_ARROW " m",                                                         SLS_ENABLED  },
/* 1537 */  { fnCvtFathomM,                divide,                      "m" STD_RIGHT_ARROW "fm.",                     "m " STD_RIGHT_ARROW,                                                         SLS_ENABLED  },
/* 1538 */  { fnCvtFathomM,                divide,                      "m" STD_RIGHT_ARROW "fm.",                     "fathom",                                                                     SLS_ENABLED  },
/* 1539 */  { fnCvtSfeetM,                 multiply,                    "ft" STD_US STD_RIGHT_ARROW "m",               STD_RIGHT_ARROW " m",                                                         SLS_ENABLED  },
/* 1540 */  { fnCvtSfeetM,                 divide,                      "m" STD_RIGHT_ARROW "ft" STD_US,               "foot" STD_US,                                                                SLS_ENABLED  },
/* 1541 */  { fnCvtPointM,                 divide,                      "m" STD_RIGHT_ARROW "pt.",                     "m " STD_RIGHT_ARROW,                                                         SLS_ENABLED  },
/* 1542 */  { fnCvtPointM,                 multiply,                    "pt." STD_RIGHT_ARROW "m",                     STD_RIGHT_ARROW " m",                                                         SLS_ENABLED  },
/* 1543 */  { fnCvtBarrelM3,               multiply,                    "bbl" STD_RIGHT_ARROW "m" STD_SUP_3,           "barrel",                                                                     SLS_ENABLED  },
/* 1544 */  { fnCvtBarrelM3,               multiply,                    "bbl" STD_RIGHT_ARROW "m" STD_SUP_3,           STD_RIGHT_ARROW " m" STD_SUP_3,                                               SLS_ENABLED  },
/* 1545 */  { fnCvtBarrelM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "bbl",           STD_RIGHT_ARROW " m" STD_SUP_3,                                               SLS_ENABLED  },
/* 1546 */  { fnCvtBarrelM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "bbl",           "barrel",                                                                     SLS_ENABLED  },
/* 1547 */  { fnCvtTonKg,                  multiply,                    "ton" STD_RIGHT_ARROW "kg",                    STD_SPACE_HAIR,                                                               SLS_ENABLED  },
/* 1548 */  { fnCvtTonKg,                  multiply,                    "ton" STD_RIGHT_ARROW "kg",                    STD_SPACE_HAIR,                                                               SLS_ENABLED  },
/* 1549 */  { fnCvtTonKg,                  divide,                      "kg" STD_RIGHT_ARROW "ton",                    STD_SPACE_HAIR,                                                               SLS_ENABLED  },
/* 1550 */  { fnCvtTonKg,                  divide,                      "kg" STD_RIGHT_ARROW "ton",                    STD_SPACE_HAIR,                                                               SLS_ENABLED  },
/* 1551 */  { fnCvtCaratKg,                multiply,                    "ct" STD_RIGHT_ARROW "kg",                     STD_SPACE_HAIR,                                                               SLS_ENABLED  },
/* 1552 */  { fnCvtCaratKg,                divide,                      "kg" STD_RIGHT_ARROW "ct",                     STD_SPACE_HAIR,                                                               SLS_ENABLED  },
/* 1553 */  { fnCvtAtmPa,                  multiply,                    "atm" STD_RIGHT_ARROW "Pa",                    STD_SPACE_HAIR,                                                               SLS_ENABLED  },
/* 1554 */  { fnCvtAtmPa,                  divide,                      "Pa" STD_RIGHT_ARROW "atm",                    STD_SPACE_HAIR,                                                               SLS_ENABLED  },
/* 1555 */  { fnCvtFathomM,                multiply,                    "fm." STD_RIGHT_ARROW "m",                     STD_SPACE_HAIR,                                                               SLS_ENABLED  },
/* 1556 */  { fnCvtFathomM,                divide,                      "m" STD_RIGHT_ARROW "fm.",                     STD_SPACE_HAIR,                                                               SLS_ENABLED  },
/* 1557 */  { fnCvtPointM,                 multiply,                    "pt." STD_RIGHT_ARROW "m",                     STD_SPACE_HAIR,                                                               SLS_ENABLED  },
/* 1558 */  { fnCvtPointM,                 divide,                      "m" STD_RIGHT_ARROW "pt.",                     STD_SPACE_HAIR,                                                               SLS_ENABLED  },
/* 1559 */  { itemToBeCoded,               NOPARAM,                     STD_x_BAR STD_SUB_H,                           STD_x_BAR STD_SUB_H,                                                          SLS_UNCHANGED},
/* 1560 */  { itemToBeCoded,               NOPARAM,                     STD_x_BAR STD_SUB_R STD_SUB_M STD_SUB_S,       STD_x_BAR STD_SUB_R STD_SUB_M STD_SUB_S,                                      SLS_UNCHANGED},
/* 1561 */  { itemToBeCoded,               NOPARAM,                     "SUMS",                                        "SUMS",                                                                       SLS_UNCHANGED},
/* 1562 */  { itemToBeCoded,               NOPARAM,                     "GaussF",                                      "GaussF",                                                                     SLS_UNCHANGED},
/* 1563 */  { itemToBeCoded,               NOPARAM,                     "CauchF",                                      "CauchF",                                                                     SLS_UNCHANGED},
/* 1564 */  { itemToBeCoded,               NOPARAM,                     "ParabF",                                      "ParabF",                                                                     SLS_UNCHANGED},
/* 1565 */  { itemToBeCoded,               NOPARAM,                     "HypF",                                        "HypF",                                                                       SLS_UNCHANGED},
/* 1566 */  { itemToBeCoded,               NOPARAM,                     "RootF",                                       "RootF",                                                                      SLS_UNCHANGED},
/* 1567 */  { fnStatSum,                   14,                          STD_SIGMA "lny/x",                             STD_SIGMA "lny/x",                                                            SLS_ENABLED  },
/* 1568 */  { fnStatSum,                   15,                          STD_SIGMA "x" STD_SUP_2 "/y",                  STD_SIGMA "x" STD_SUP_2 "/y",                                                 SLS_ENABLED  },
/* 1569 */  { fnStatSum,                   16,                          STD_SIGMA STD_SUP_1 "/x",                      STD_SIGMA STD_SUP_1 "/x",                                                     SLS_ENABLED  },
/* 1570 */  { fnStatSum,                   17,                          STD_SIGMA STD_SUP_1 "/x" STD_SUP_2,            STD_SIGMA STD_SUP_1 "/x" STD_SUP_2,                                           SLS_ENABLED  },
/* 1571 */  { fnStatSum,                   18,                          STD_SIGMA "x/y",                               STD_SIGMA "x/y",                                                              SLS_ENABLED  },
/* 1572 */  { fnStatSum,                   19,                          STD_SIGMA STD_SUP_1 "/y",                      STD_SIGMA STD_SUP_1 "/y",                                                     SLS_ENABLED  },
/* 1573 */  { fnStatSum,                   20,                          STD_SIGMA STD_SUP_1 "/y" STD_SUP_2,            STD_SIGMA STD_SUP_1 "/y" STD_SUP_2,                                           SLS_ENABLED  },
/* 1574 */  { fnStatSum,                   21,                          STD_SIGMA "x" STD_SUP_3,                       STD_SIGMA "x" STD_SUP_3,                                                      SLS_ENABLED  },
/* 1575 */  { fnStatSum,                   22,                          STD_SIGMA "x" STD_SUP_4,                       STD_SIGMA "x" STD_SUP_4,                                                      SLS_ENABLED  },
/* 1576 */  { addItemToBuffer,             NOPARAM,                     "HEX",                                         "H",                                                                          SLS_UNCHANGED},
/* 1577 */  { fnSetSetJM,                  1,                           "SET_ERPN",                                    "eRPN",                                                                       SLS_ENABLED},      //JM eRPN
/* 1578 */  { fnSetSetJM,                  2,                           "SET_HOME_TRIPLE",                             "HOME.3",                                                                     SLS_ENABLED},      //JM HOME.3
/* 1579 */  { fnSetSetJM,                  3,                           "SET_SHFT_4s",                                 "SH_4s",                                                                     SLS_ENABLED},      //JM SHIFT CANCEL
/* 1580 */  { itemToBeCoded,               NOPARAM,                     "HOME",                                        "HOME",                                                                       SLS_UNCHANGED},      //JM HOME
/* 1581 */  { fnDisplayFormatSigFig,       TM_VALUE,                    "SIGFIG",                                      "SIGFIG",                                                                     SLS_UNCHANGED},      //JM SIGFIG
/* 1582 */  { itemToBeCoded,               NOPARAM,                     "ALPHA",                                       "ALPHA",                                                                      SLS_UNCHANGED},      //JM ALPHA
/* 1583 */  { itemToBeCoded,               NOPARAM,                     "BASE",                                        "BASE",                                                                       SLS_UNCHANGED},      //JM BASE
/* 1584 */  { fnChangeBase,                2,                           STD_RIGHT_ARROW "BIN",                         STD_RIGHT_ARROW "BIN",                                                        SLS_UNCHANGED},      //JM HEX
/* 1585 */  { fnChangeBase,                8,                           STD_RIGHT_ARROW "OCT",                         STD_RIGHT_ARROW "OCT",                                                        SLS_UNCHANGED},      //JM HEX
/* 1586 */  { fnChangeBase,                10,                          STD_RIGHT_ARROW "DEC",                         STD_RIGHT_ARROW "DEC",                                                        SLS_UNCHANGED},      //JM HEX
/* 1587 */  { fnChangeBase,                16,                          STD_RIGHT_ARROW "HEX",                         STD_RIGHT_ARROW "HEX",                                                        SLS_UNCHANGED},      //JM HEX
/* 1588 */  { fnSetWordSize,               8,                           "8-BIT",                                       "8-BIT",                                                                      SLS_UNCHANGED},      //JM HEX
/* 1589 */  { fnSetWordSize,               16,                          "16-BIT",                                      "16-BIT",                                                                      SLS_UNCHANGED},      //JM HEX
/* 1590 */  { fnSetWordSize,               32,                          "32-BIT",                                      "32-BIT",                                                                      SLS_UNCHANGED},      //JM HEX
/* 1591 */  { fnSetWordSize,               64,                          "64-BIT",                                      "64-BIT",                                                                      SLS_UNCHANGED},      //JM HEX
/* 1592 */  { fnDisplayFormatUnit,         TM_VALUE,                    "SETUNIT",                                     "UNITS",                                                                       SLS_UNCHANGED},      //JM UNIT
/* 1593 */  { fnShowJM,                    1,                           "eRPN?",                                       "eRPN?",                                                                       SLS_ENABLED  },     //JM SHOW
/* 1594 */  { fnShowJM,                    2,                           "HOME.3?",                                     "HOME.3?",                                                                     SLS_ENABLED  },     //JM SHOW
/* 1595 */  { fnShowJM,                    3,                           "SH_TIM_4s?",                                  "SH_4s?",                                                                     SLS_ENABLED  },     //JM SHOW
/* 1596 */  { addItemToBuffer,             CHR_QOPPA,                    "",                                            STD_QOPPA,                                                                   SLS_UNCHANGED},  //JM GREEK
/* 1597 */  { addItemToBuffer,             CHR_DIGAMMA,                  "",                                            STD_DIGAMMA,                                                                   SLS_UNCHANGED},  //JM GREEK
/* 1598 */  { addItemToBuffer,             CHR_SAMPI,                    "",                                            STD_SAMPI,                                                                   SLS_UNCHANGED},  //JM GREEK
/* 1599 */  { itemToBeCoded,               NOPARAM,                     "1599",                                        "1599",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1600 */  { itemToBeCoded,               NOPARAM,                     "1600",                                        "1600",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1601 */  { itemToBeCoded,               NOPARAM,                     "1601",                                        "1601",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1602 */  { itemToBeCoded,               NOPARAM,                     "1602",                                        "1602",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1603 */  { itemToBeCoded,               NOPARAM,                     "1603",                                        "1603",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1604 */  { itemToBeCoded,               NOPARAM,                     "1604",                                        "1604",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1605 */  { itemToBeCoded,               NOPARAM,                     "1605",                                        "1605",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1606 */  { itemToBeCoded,               NOPARAM,                     "1606",                                        "1606",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1607 */  { itemToBeCoded,               NOPARAM,                     "1607",                                        "1607",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1608 */  { itemToBeCoded,               NOPARAM,                     "1608",                                        "1608",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1609 */  { itemToBeCoded,               NOPARAM,                     "1609",                                        "1609",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1610 */  { itemToBeCoded,               NOPARAM,                     "1610",                                        "1610",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1611 */  { itemToBeCoded,               NOPARAM,                     "1611",                                        "1611",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1612 */  { itemToBeCoded,               NOPARAM,                     "1612",                                        "1612",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1613 */  { itemToBeCoded,               NOPARAM,                     "1613",                                        "1613",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1614 */  { itemToBeCoded,               NOPARAM,                     "1614",                                        "1614",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1615 */  { itemToBeCoded,               NOPARAM,                     "1615",                                        "1615",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1616 */  { itemToBeCoded,               NOPARAM,                     "1616",                                        "1616",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1617 */  { itemToBeCoded,               NOPARAM,                     "1617",                                        "1617",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1618 */  { itemToBeCoded,               NOPARAM,                     "1618",                                        "1618",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1619 */  { itemToBeCoded,               NOPARAM,                     "1619",                                        "1619",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1620 */  { itemToBeCoded,               NOPARAM,                     "1620",                                        "1620",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1621 */  { itemToBeCoded,               NOPARAM,                     "1621",                                        "1621",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1622 */  { itemToBeCoded,               NOPARAM,                     "1622",                                        "1622",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1623 */  { itemToBeCoded,               NOPARAM,                     "1623",                                        "1623",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1624 */  { itemToBeCoded,               NOPARAM,                     "1624",                                        "1624",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1625 */  { itemToBeCoded,               NOPARAM,                     "1625",                                        "1625",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1626 */  { itemToBeCoded,               NOPARAM,                     "1626",                                        "1626",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1627 */  { itemToBeCoded,               NOPARAM,                     "1627",                                        "1627",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1628 */  { itemToBeCoded,               NOPARAM,                     "1628",                                        "1628",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1629 */  { itemToBeCoded,               NOPARAM,                     "1629",                                        "1629",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1630 */  { itemToBeCoded,               NOPARAM,                     "1630",                                        "1630",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1631 */  { itemToBeCoded,               NOPARAM,                     "1631",                                        "1631",                                                                       SLS_UNCHANGED},  //JM GREEK
/* 1632 */  { addItemToBuffer,             CHR_qoppa,                   "",                                            STD_qoppa,                                                                   SLS_UNCHANGED},  //JM GREEK
/* 1633 */  { addItemToBuffer,             CHR_digamma,                 "",                                            STD_digamma,                                                                 SLS_UNCHANGED},  //JM GREEK
/* 1634 */  { addItemToBuffer,             CHR_sampi,                   "",                                            STD_sampi,                                                                   SLS_UNCHANGED},  //JM GREEK
/* 1635 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_case,                                                                    SLS_UNCHANGED},   //JM CASE
/* 1636 */  { fnBASE_Hash,      /*TEST!*/  NOPARAM,                     "##",                                          "##",                                                                        SLS_UNCHANGED},   //JM ##
/* 1637 */  { addItemToBuffer,  /*TEST!*/  CHR_NUMBER_SIGN,             "###",                                         "###",                                                                       SLS_UNCHANGED},   //JM ##
/* 1638 */  { fnJM,                        3,                          "op_a",                                         "a",                                                                       SLS_ENABLED  },  //JM Operator a
/* 1639 */  { fnJM,                        4,                          "op_a" STD_SUP_2,                               "a" STD_SUP_2,                                                             SLS_ENABLED  },  //JM Operator a
/* 1640 */  { fnJM,                        5,                          "op_j",                                         "j",                                                                       SLS_ENABLED  },  //JM Operator a
/* 1641 */  { fnSetSetJM,                  4,                           "SET_BASE_HOME",                               "HOME",                                                                     SLS_ENABLED},      //JM eRPN
/* 1642 */  { fnSetSetJM,                  5,                           "SET_BASE_MYMENU",                             "MYMNU",                                                                    SLS_ENABLED},      //JM eRPN
/* 1643 */  { fnSetSetJM,                  6,                           "SET_BASE_AHOME",                              STD_alpha "HOME",                                                           SLS_ENABLED},      //JM eRPN
/* 1644 */  { fnSetSetJM,                  7,                           "SET_BASE_MYA",                                "MY" STD_alpha,                                                             SLS_ENABLED},      //JM eRPN
/* 1645 */  { fnShowJM,                    4,                           "BASE_HOME?",                                  "HOME?",                                                                    SLS_ENABLED  },     //JM SHOW
/* 1646 */  { fnShowJM,                    5,                           "BASE_MYMENU?",                                "MYMNU?",                                                                   SLS_ENABLED  },     //JM SHOW
/* 1647 */  { fnShowJM,                    6,                           "BASE_AHOME?",                                 STD_alpha "HOME?",                                                             SLS_ENABLED  },     //JM SHOW
/* 1648 */  { fnShowJM,                    7,                           "BASE_MYA?",                                   "MY" STD_alpha "?",                                                            SLS_ENABLED  },     //JM SHOW
/* 1649 */  { fnJM,                        7,                          "YtoD",                                         "Y" STD_SPACE_3_PER_EM STD_RIGHT_ARROW STD_SPACE_3_PER_EM STD_DELTA,           SLS_ENABLED  },  //JM EE 
/* 1650 */  { fnJM,                        6,                          "DtoY",                                         STD_DELTA STD_SPACE_3_PER_EM STD_RIGHT_ARROW STD_SPACE_3_PER_EM "Y",           SLS_ENABLED  },  //JM EE 
/* 1651 */  { fnJM,                        9,                          "AtoSYM",                                       STD_RIGHT_ARROW STD_SPACE_3_PER_EM "012",                                      SLS_ENABLED  },  //JM EE 
/* 1652 */  { fnJM,                        8,                          "SYMtoA",                                       STD_RIGHT_ARROW STD_SPACE_3_PER_EM "abc",                                      SLS_ENABLED  },  //JM EE 
/* 1653 */  { itemToBeCoded,               NOPARAM,                     "ELEC.ENG",                                    "ELEC",                                                                       SLS_UNCHANGED},   //JM EE
/* 1654 */  { fnJM,                        10,                          "e^theta_j",                                   "e^" STD_THETA "j",                                                            SLS_ENABLED  },  //JM EE 

/* 1655 */  { fnJM,                        11,                          "",                                             "STO" STD_SPACE_3_PER_EM "Z",                      SLS_ENABLED  },  //JM EE 
/* 1656 */  { fnJM,                        12,                          "",                                             "RCL" STD_SPACE_3_PER_EM "Z",                      SLS_ENABLED  },  //JM EE 
/* 1657 */  { fnJM,                        13,                          "",                                             "STO" STD_SPACE_3_PER_EM "V",                      SLS_ENABLED  },  //JM EE 
/* 1658 */  { fnJM,                        14,                          "",                                             "RCL" STD_SPACE_3_PER_EM "V",                      SLS_ENABLED  },  //JM EE 
/* 1659 */  { fnJM,                        15,                          "",                                             "STO" STD_SPACE_3_PER_EM "I",                      SLS_ENABLED  },  //JM EE 
/* 1660 */  { fnJM,                        16,                          "",                                             "RCL" STD_SPACE_3_PER_EM "I",                      SLS_ENABLED  },  //JM EE 
/* 1661 */  { fnJM,                        17,                          "",                                             "V" STD_DIVIDE "I",             SLS_ENABLED  },  //JM EE 
/* 1662 */  { fnJM,                        18,                          "",                                             "I" STD_CROSS "R",              SLS_ENABLED  },  //JM EE 
/* 1663 */  { fnJM,                        19,                          "",                                             "V" STD_DIVIDE "Z",             SLS_ENABLED  },  //JM EE 
/* 1664 */  { fnJM,                        20,                          "",                                             "X" STD_SPACE_3_PER_EM STD_RIGHT_ARROW STD_SPACE_3_PER_EM "BAL",             SLS_ENABLED  },  //JM EE 
/* 1665 */  { fnComplexCCCC_CPX,           NOPARAM,                     "COMPLEX",                                      "COMPLEX",                                                                     SLS_UNCHANGED},   //JM Change CC to COMPLEX
/* 1666 */  { fnComplexCCCC_CC1,           NOPARAM,                     "CC1",                                          "CC1",                                                                         SLS_UNCHANGED},   //JM Change CC to CC1
/* 1667 */  { fnJM,                        21,     /*up*/               "KEY_TYPCON_UP",                                ">>",                                                                         SLS_ENABLED  },  //JM TYPE CONVERT 
/* 1668 */  { fnJM,                        22,     /*dn*/               "KEY_TYPCON_DN",                                "<<",                                                                         SLS_ENABLED  },  //JM TYPE CONVERT 
/* 1669 */  { fnSetSetJM,                  8,                           "SH.3T",                                        "SH.3T",                                                                     SLS_ENABLED},      //JM HOME.3T
/* 1670 */  { fnShowJM,                    8,                           "SH.3T?",                                       "SH.3T?",                                                                     SLS_ENABLED  },     //JM SHOW HOME.3T

/* 1671 */  { itemToBeCoded,               NOPARAM,                     "MNUASM",                                       "M_ASM",                                                                        SLS_UNCHANGED}, //JM USER

/*1672*/  { fnJMUSERmode, 256+0,         "KEY 00U",               "K00U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1673*/  { fnJMUSERmode, 256+1,         "KEY f00U",               "Kf00U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1674*/  { fnJMUSERmode, 256+2,         "KEY g00U",               "Kg00U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1675*/  { fnJMUSERmode, 256+3,         "KEY 01U",               "K01U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1676*/  { fnJMUSERmode, 256+4,         "KEY f01U",               "Kf01U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1677*/  { fnJMUSERmode, 256+5,         "KEY g01U",               "Kg01U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1678*/  { fnJMUSERmode, 256+6,         "KEY 02U",               "K02U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1679*/  { fnJMUSERmode, 256+7,         "KEY f02U",               "Kf02U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1680*/  { fnJMUSERmode, 256+8,         "KEY g02U",               "Kg02U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1681*/  { fnJMUSERmode, 256+9,         "KEY 03U",               "K03U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1682*/  { fnJMUSERmode, 256+10,         "KEY f03U",               "Kf03U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1683*/  { fnJMUSERmode, 256+11,         "KEY g03U",               "Kg03U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1684*/  { fnJMUSERmode, 256+12,         "KEY 04U",               "K04U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1685*/  { fnJMUSERmode, 256+13,         "KEY f04U",               "Kf04U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1686*/  { fnJMUSERmode, 256+14,         "KEY g04U",               "Kg04U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1687*/  { fnJMUSERmode, 256+15,         "KEY 05U",               "K05U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1688*/  { fnJMUSERmode, 256+16,         "KEY f05U",               "Kf05U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1689*/  { fnJMUSERmode, 256+17,         "KEY g05U",               "Kg05U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1690*/  { fnJMUSERmode, 256+18,         "KEY 06U",               "K06U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1691*/  { fnJMUSERmode, 256+19,         "KEY f06U",               "Kf06U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1692*/  { fnJMUSERmode, 256+20,         "KEY g06U",               "Kg06U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1693*/  { fnJMUSERmode, 256+21,         "KEY 07U",               "K07U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1694*/  { fnJMUSERmode, 256+22,         "KEY f07U",               "Kf07U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1695*/  { fnJMUSERmode, 256+23,         "KEY g07U",               "Kg07U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1696*/  { fnJMUSERmode, 256+24,         "KEY 08U",               "K08U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1697*/  { fnJMUSERmode, 256+25,         "KEY f08U",               "Kf08U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1698*/  { fnJMUSERmode, 256+26,         "KEY g08U",               "Kg08U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1699*/  { fnJMUSERmode, 256+27,         "KEY 09U",               "K09U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1700*/  { fnJMUSERmode, 256+28,         "KEY f09U",               "Kf09U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1701*/  { fnJMUSERmode, 256+29,         "KEY g09U",               "Kg09U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1702*/  { fnJMUSERmode, 256+30,         "KEY 10U",               "K10U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1703*/  { fnJMUSERmode, 256+31,         "KEY f10U",               "Kf10U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1704*/  { fnJMUSERmode, 256+32,         "KEY g10U",               "Kg10U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1705*/  { fnJMUSERmode, 256+33,         "KEY 11U",               "K11U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1706*/  { fnJMUSERmode, 256+34,         "KEY f11U",               "Kf11U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1707*/  { fnJMUSERmode, 256+35,         "KEY g11U",               "Kg11U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1708*/  { fnJMUSERmode, 256+36,         "KEY 12U",               "K12U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1709*/  { fnJMUSERmode, 256+37,         "KEY f12U",               "Kf12U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1710*/  { fnJMUSERmode, 256+38,         "KEY g12U",               "Kg12U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1711*/  { fnJMUSERmode, 256+39,         "KEY 13U",               "K13U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1712*/  { fnJMUSERmode, 256+40,         "KEY f13U",               "Kf13U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1713*/  { fnJMUSERmode, 256+41,         "KEY g13U",               "Kg13U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1714*/  { fnJMUSERmode, 256+42,         "KEY 14U",               "K14U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1715*/  { fnJMUSERmode, 256+43,         "KEY f14U",               "Kf14U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1716*/  { fnJMUSERmode, 256+44,         "KEY g14U",               "Kg14U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1717*/  { fnJMUSERmode, 256+45,         "KEY 15U",               "K15U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1718*/  { fnJMUSERmode, 256+46,         "KEY f15U",               "Kf15U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1719*/  { fnJMUSERmode, 256+47,         "KEY g15U",               "Kg15U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1720*/  { fnJMUSERmode, 256+48,         "KEY 16U",               "K16U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1721*/  { fnJMUSERmode, 256+49,         "KEY f16U",               "Kf16U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1722*/  { fnJMUSERmode, 256+50,         "KEY g16U",               "Kg16U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1723*/  { fnJMUSERmode, 256+51,         "KEY 17U",               "K17U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1724*/  { fnJMUSERmode, 256+52,         "KEY f17U",               "Kf17U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1725*/  { fnJMUSERmode, 256+53,         "KEY g17U",               "Kg17U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1726*/  { fnJMUSERmode, 256+54,         "KEY 18U",               "K18U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1727*/  { fnJMUSERmode, 256+55,         "KEY f18U",               "Kf18U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1728*/  { fnJMUSERmode, 256+56,         "KEY g18U",               "Kg18U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1729*/  { fnJMUSERmode, 256+57,         "KEY 19U",               "K19U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1730*/  { fnJMUSERmode, 256+58,         "KEY f19U",               "Kf19U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1731*/  { fnJMUSERmode, 256+59,         "KEY g19U",               "Kg19U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1732*/  { fnJMUSERmode, 256+60,         "KEY 20U",               "K20U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1733*/  { fnJMUSERmode, 256+61,         "KEY f20U",               "Kf20U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1734*/  { fnJMUSERmode, 256+62,         "KEY g20U",               "Kg20U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1735*/  { fnJMUSERmode, 256+63,         "KEY 21U",               "K21U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1736*/  { fnJMUSERmode, 256+64,         "KEY f21U",               "Kf21U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1737*/  { fnJMUSERmode, 256+65,         "KEY g21U",               "Kg21U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1738*/  { fnJMUSERmode, 256+66,         "KEY 22U",               "K22U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1739*/  { fnJMUSERmode, 256+67,         "KEY f22U",               "Kf22U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1740*/  { fnJMUSERmode, 256+68,         "KEY g22U",               "Kg22U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1741*/  { fnJMUSERmode, 256+69,         "KEY 23U",               "K23U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1742*/  { fnJMUSERmode, 256+70,         "KEY f23U",               "Kf23U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1743*/  { fnJMUSERmode, 256+71,         "KEY g23U",               "Kg23U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1744*/  { fnJMUSERmode, 256+72,         "KEY 24U",               "K24U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1745*/  { fnJMUSERmode, 256+73,         "KEY f24U",               "Kf24U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1746*/  { fnJMUSERmode, 256+74,         "KEY g24U",               "Kg24U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1747*/  { fnJMUSERmode, 256+75,         "KEY 25U",               "K25U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1748*/  { fnJMUSERmode, 256+76,         "KEY f25U",               "Kf25U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1749*/  { fnJMUSERmode, 256+77,         "KEY g25U",               "Kg25U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1750*/  { fnJMUSERmode, 256+78,         "KEY 26U",               "K26U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1751*/  { fnJMUSERmode, 256+79,         "KEY f26U",               "Kf26U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1752*/  { fnJMUSERmode, 256+80,         "KEY g26U",               "Kg26U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1753*/  { fnJMUSERmode, 256+81,         "KEY 27U",               "K27U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1754*/  { fnJMUSERmode, 256+82,         "KEY f27U",               "Kf27U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1755*/  { fnJMUSERmode, 256+83,         "KEY g27U",               "Kg27U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1756*/  { fnJMUSERmode, 256+84,         "KEY 28U",               "K28U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1757*/  { fnJMUSERmode, 256+85,         "KEY f28U",               "Kf28U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1758*/  { fnJMUSERmode, 256+86,         "KEY g28U",               "Kg28U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1759*/  { fnJMUSERmode, 256+87,         "KEY 29U",               "K29U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1760*/  { fnJMUSERmode, 256+88,         "KEY f29U",               "Kf29U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1761*/  { fnJMUSERmode, 256+89,         "KEY g29U",               "Kg29U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1762*/  { fnJMUSERmode, 256+90,         "KEY 30U",               "K30U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1763*/  { fnJMUSERmode, 256+91,         "KEY f30U",               "Kf30U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1764*/  { fnJMUSERmode, 256+92,         "KEY g30U",               "Kg30U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1765*/  { fnJMUSERmode, 256+93,         "KEY 31U",               "K31U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1766*/  { fnJMUSERmode, 256+94,         "KEY f31U",               "Kf31U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1767*/  { fnJMUSERmode, 256+95,         "KEY g31U",               "Kg31U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1768*/  { fnJMUSERmode, 256+96,         "KEY 32U",               "K32U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1769*/  { fnJMUSERmode, 256+97,         "KEY f32U",               "Kf32U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1770*/  { fnJMUSERmode, 256+98,         "KEY g32U",               "Kg32U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1771*/  { fnJMUSERmode, 256+99,         "KEY 33U",               "K33U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1772*/  { fnJMUSERmode, 256+100,         "KEY f33U",               "Kf33U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1773*/  { fnJMUSERmode, 256+101,         "KEY g33U",               "Kg33U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1774*/  { fnJMUSERmode, 256+102,         "KEY 34U",               "K34U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1775*/  { fnJMUSERmode, 256+103,         "KEY f34U",               "Kf34U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1776*/  { fnJMUSERmode, 256+104,         "KEY g34U",               "Kg34U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1777*/  { fnJMUSERmode, 256+105,         "KEY 35U",               "K35U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1778*/  { fnJMUSERmode, 256+106,         "KEY f35U",               "Kf35U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1779*/  { fnJMUSERmode, 256+107,         "KEY g35U",               "Kg35U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1780*/  { fnJMUSERmode, 256+108,         "KEY 36U",               "K36U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1781*/  { fnJMUSERmode, 256+109,         "KEY f36U",               "Kf36U",                   SLS_UNCHANGED  },  //JM USER MODE TEST
/*1782*/  { fnJMUSERmode, 256+110,         "KEY g36U",               "Kg36U",                   SLS_UNCHANGED  },  //JM USER MODE TEST


/*1783*/  { fnJMUSERmode, 0,         "KEY 00N",               "K00N",                   SLS_UNCHANGED  },  //JM USER MODE TEST: NORMAL KEY
/*1784*/  { fnJMUSERmode, 1,         "KEY f00N",               "Kf00N",                   SLS_UNCHANGED  },  //JM USER MODE TEST: NORMAL KEY
/*1785*/  { fnJMUSERmode, 2,         "KEY g00N",               "Kg00N",                   SLS_UNCHANGED  },  //JM USER MODE TEST: NORMAL KEY
/*1786*/  { fnJMUSERmode, 27,         "KEY 09N",               "K09N",                   SLS_UNCHANGED  },  //JM USER MODE TEST: NORMAL KEY
/*1787*/  { fnJMUSERmode, 28,         "KEY f09N",               "Kf09N",                   SLS_UNCHANGED  },  //JM USER MODE TEST: NORMAL KEY
/*1788*/  { fnJMUSERmode, 29,         "KEY g09N",               "Kg09N",                   SLS_UNCHANGED  },  //JM USER MODE TEST: NORMAL KEY
/*1789*/  { fnJMUSERmode, 30,         "KEY 10N",               "K10N",                   SLS_UNCHANGED  },  //JM USER MODE TEST: NORMAL KEY
/*1790*/  { fnJMUSERmode, 31,         "KEY f10N",               "Kf10N",                   SLS_UNCHANGED  },  //JM USER MODE TEST: NORMAL KEY
/*1791*/  { fnJMUSERmode, 32,         "KEY g10N",               "Kg10N",                   SLS_UNCHANGED  },  //JM USER MODE TEST: NORMAL KEY
/*1792*/  { fnJMUSERmode, 33,         "KEY 11N",               "K11N",                   SLS_UNCHANGED  },  //JM USER MODE TEST: NORMAL KEY
/*1793*/  { fnJMUSERmode, 34,         "KEY f11N",               "Kf11N",                   SLS_UNCHANGED  },  //JM USER MODE TEST: NORMAL KEY
/*1794*/  { fnJMUSERmode, 35,         "KEY g11N",               "Kg11N",                   SLS_UNCHANGED  },  //JM USER MODE TEST: NORMAL KEY
/*1795*/  { fnJMUSERmode, 36,         "KEY 12N",               "K12N",                   SLS_UNCHANGED  },  //JM USER MODE TEST: NORMAL KEY
/*1796*/  { fnJMUSERmode, 37,         "KEY f12N",               "Kf12N",                   SLS_UNCHANGED  },  //JM USER MODE TEST: NORMAL KEY
/*1797*/  { fnJMUSERmode, 38,         "KEY g12N",               "Kg12N",                   SLS_UNCHANGED  },  //JM USER MODE TEST: NORMAL KEY
/*1798*/  { fnJMUSERmode, 81,         "KEY 27N",               "K27N",                   SLS_UNCHANGED  },  //JM USER MODE TEST: NORMAL KEY
/*1799*/  { fnJMUSERmode, 82,         "KEY f27N",               "Kf27N",                   SLS_UNCHANGED  },  //JM USER MODE TEST: NORMAL KEY
/*1800*/  { fnJMUSERmode, 83,         "KEY g27N",               "Kg27N",                   SLS_UNCHANGED  },  //JM USER MODE TEST: NORMAL KEY
  
/* 1801 */  { itemToBeCoded,               NOPARAM,                     "",                                            "Last item",                                                                  SLS_UNCHANGED}       //JM eRPN 
};
