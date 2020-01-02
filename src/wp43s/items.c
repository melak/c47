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



#ifndef GENERATE_CATALOGS
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
#endif



/********************************************//**
 * \brief No OPeration
 *
 * \param  unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnNop(uint16_t unusedParamButMandatory) {
}


#if !defined(TESTSUITE_BUILD) && !defined(GENERATE_CATALOGS)
//#error runFunction
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
    strcpy(tamBuffer, indexOfItems[func].itemSoftmenuName);
    tamNumberMin = 0;
    tamNumberMax = 99;

    if(func == ITM_FIX || func == ITM_SCI || func == ITM_ENG || func == ITM_ALL || func == ITM_GAP || func == ITM_SIGFIG || func == ITM_UNIT) {      //JM SIGFIG. Added ITM_SIGFIG to max 15. Added Item ITM_UNIT
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

    calcModeTam();
    return;
  }

  indexOfItems[func].func(indexOfItems[func].param);

  if(indexOfItems[func].stackLiftStatus == SLS_DISABLED) {
    STACK_LIFT_DISABLE;
  }
  else if(indexOfItems[func].stackLiftStatus == SLS_ENABLED) {
    STACK_LIFT_ENABLE;
  }

  if((func == ITM_ENTER) && (eRPN)) {      //JM NEWERPN 
    STACK_LIFT_ENABLE;                     //JM NEWERPN OVERRIDE SLS, AS ERPN ENTER ALWAYS HAS SLS SET
  }                                        //JM NEWERPN 


  if(!funcOK) {
    displayCalcErrorMessage(ERROR_ITEM_TO_BE_CODED, ERR_REGISTER_LINE, REGISTER_X);
    #ifdef PC_BUILD
      sprintf(errorMessage, "%" FMT16S " = %s", func, indexOfItems[func].itemCatalogName);
      showInfoDialog("In function runFunction:", "Item not implemented", errorMessage, "to be coded");
    #endif
  }
}
#endif

#ifdef GENERATE_CATALOGS
void fnPow10                    (uint16_t unusedParamButMandatory) {}
void fnIntegerMode              (uint16_t unusedParamButMandatory) {}
void fnConstant                 (uint16_t unusedParamButMandatory) {}
void fnInvert                   (uint16_t unusedParamButMandatory) {}
void fn2Pow                     (uint16_t unusedParamButMandatory) {}
void fn10Pow                    (uint16_t unusedParamButMandatory) {}
void fnCubeRoot                 (uint16_t unusedParamButMandatory) {}
void fnMagnitude                (uint16_t unusedParamButMandatory) {}
void fnAgm                      (uint16_t unusedParamButMandatory) {}
void fnDisplayFormatAll         (uint16_t unusedParamButMandatory) {}
void fnDisplayFormatFix         (uint16_t unusedParamButMandatory) {}
void fnDisplayFormatSci         (uint16_t unusedParamButMandatory) {}
void fnDisplayFormatEng         (uint16_t unusedParamButMandatory) {}
void fnDisplayFormatGap         (uint16_t unusedParamButMandatory) {}
void fnArccos                   (uint16_t unusedParamButMandatory) {}
void fnArccosh                  (uint16_t unusedParamButMandatory) {}
void fnArcsin                   (uint16_t unusedParamButMandatory) {}
void fnArcsinh                  (uint16_t unusedParamButMandatory) {}
void fnArctan                   (uint16_t unusedParamButMandatory) {}
void fnArctanh                  (uint16_t unusedParamButMandatory) {}
void fnCos                      (uint16_t unusedParamButMandatory) {}
void fnCosh                     (uint16_t unusedParamButMandatory) {}
void fnSin                      (uint16_t unusedParamButMandatory) {}
void fnSinh                     (uint16_t unusedParamButMandatory) {}
void fnTan                      (uint16_t unusedParamButMandatory) {}
void fnTanh                     (uint16_t unusedParamButMandatory) {}
void fnDrop                     (uint16_t unusedParamButMandatory) {}
void fnDropY                    (uint16_t unusedParamButMandatory) {}
void fnBatteryVoltage           (uint16_t unusedParamButMandatory) {}
void fnCurveFitting             (uint16_t unusedParamButMandatory) {}
void fnCeil                     (uint16_t unusedParamButMandatory) {}
void fnFloor                    (uint16_t unusedParamButMandatory) {}
void fnClearFlag                (uint16_t unusedParamButMandatory) {}
void fnFlipFlag                 (uint16_t unusedParamButMandatory) {}
void fnSetFlag                  (uint16_t unusedParamButMandatory) {}
void fnClAll                    (uint16_t unusedParamButMandatory) {}
void fnClX                      (uint16_t unusedParamButMandatory) {}
void fnClFAll                   (uint16_t unusedParamButMandatory) {}
void fnClPAll                   (uint16_t unusedParamButMandatory) {}
void fnClSigma                  (uint16_t unusedParamButMandatory) {}
void fnClearStack               (uint16_t unusedParamButMandatory) {}
void fnClearRegisters           (uint16_t unusedParamButMandatory) {}
void fnTimeFormat               (uint16_t unusedParamButMandatory) {}
void fnDateFormat               (uint16_t unusedParamButMandatory) {}
void fnComplexUnit              (uint16_t unusedParamButMandatory) {}
void fnComplexMode              (uint16_t unusedParamButMandatory) {}
void fnComplexResult            (uint16_t unusedParamButMandatory) {}
void fnConjugate                (uint16_t unusedParamButMandatory) {}
void fnAngularMode              (uint16_t unusedParamButMandatory) {}
void fnDenMode                  (uint16_t unusedParamButMandatory) {}
void fnDenMax                   (uint16_t unusedParamButMandatory) {}
void fnExp                      (uint16_t unusedParamButMandatory) {}
void fnExpt                     (uint16_t unusedParamButMandatory) {}
void fnMant                     (uint16_t unusedParamButMandatory) {}
void fnCxToRe                   (uint16_t unusedParamButMandatory) {}
void fnReToCx                   (uint16_t unusedParamButMandatory) {}
void fnFillStack                (uint16_t unusedParamButMandatory) {}
void fnIsFlagClear              (uint16_t unusedParamButMandatory) {}
void fnIsFlagClearClear         (uint16_t unusedParamButMandatory) {}
void fnIsFlagClearFlip          (uint16_t unusedParamButMandatory) {}
void fnIsFlagClearSet           (uint16_t unusedParamButMandatory) {}
void fnIsFlagSet                (uint16_t unusedParamButMandatory) {}
void fnIsFlagSetClear           (uint16_t unusedParamButMandatory) {}
void fnIsFlagSetFlip            (uint16_t unusedParamButMandatory) {}
void fnIsFlagSetSet             (uint16_t unusedParamButMandatory) {}
void fnDisplayOvr               (uint16_t unusedParamButMandatory) {}
void fnEnter                    (uint16_t unusedParamButMandatory) {}
void fnDisplayStack             (uint16_t unusedParamButMandatory) {}
void fnFreeFlashMemory          (uint16_t unusedParamButMandatory) {}
void fnFreeMemory               (uint16_t unusedParamButMandatory) {}
void fnFp                       (uint16_t unusedParamButMandatory) {}
void fnIp                       (uint16_t unusedParamButMandatory) {}
void allocateLocalRegisters     (uint16_t unusedParamButMandatory) {}
void fnLeadingZeros             (uint16_t unusedParamButMandatory) {}
void fnNeighb                   (uint16_t unusedParamButMandatory) {}
void fnGcd                      (uint16_t unusedParamButMandatory) {}
void fnLogicalNot               (uint16_t unusedParamButMandatory) {}
void fnMin                      (uint16_t unusedParamButMandatory) {}
void fnMax                      (uint16_t unusedParamButMandatory) {}
void fnStatSum                  (uint16_t unusedParamButMandatory) {}
void fnIsPrime                  (uint16_t unusedParamButMandatory) {}
void fnRandom                   (uint16_t unusedParamButMandatory) {}
void fnImaginaryPart            (uint16_t unusedParamButMandatory) {}
void fnRecall                   (uint16_t unusedParamButMandatory) {}
void fnRecallConfig             (uint16_t unusedParamButMandatory) {}
void fnRecallElement            (uint16_t unusedParamButMandatory) {}
void fnRecallIJ                 (uint16_t unusedParamButMandatory) {}
void fnRecallStack              (uint16_t unusedParamButMandatory) {}
void fnRecallAdd                (uint16_t unusedParamButMandatory) {}
void fnRecallSub                (uint16_t unusedParamButMandatory) {}
void fnRecallMult               (uint16_t unusedParamButMandatory) {}
void fnRecallDiv                (uint16_t unusedParamButMandatory) {}
void fnRecallMin                (uint16_t unusedParamButMandatory) {}
void fnRecallMax                (uint16_t unusedParamButMandatory) {}
void fnRadixMark                (uint16_t unusedParamButMandatory) {}
void fnReset                    (uint16_t unusedParamButMandatory) {}
void fnRealPart                 (uint16_t unusedParamButMandatory) {}
void fnRmd                      (uint16_t unusedParamButMandatory) {}
void fnRound                    (uint16_t unusedParamButMandatory) {}
void fnRoundi                   (uint16_t unusedParamButMandatory) {}
void fnRollDown                 (uint16_t unusedParamButMandatory) {}
void fnRollUp                   (uint16_t unusedParamButMandatory) {}
void fnSeed                     (uint16_t unusedParamButMandatory) {}
void fnConfigChina              (uint16_t unusedParamButMandatory) {}
void fnConfigEurope             (uint16_t unusedParamButMandatory) {}
void fnConfigIndia              (uint16_t unusedParamButMandatory) {}
void fnConfigJapan              (uint16_t unusedParamButMandatory) {}
void fnConfigUk                 (uint16_t unusedParamButMandatory) {}
void fnConfigUsa                (uint16_t unusedParamButMandatory) {}
void fnFractionType             (uint16_t unusedParamButMandatory) {}
void fnLcm                      (uint16_t unusedParamButMandatory) {}
void fnSign                     (uint16_t unusedParamButMandatory) {}
void fnSlvq                     (uint16_t unusedParamButMandatory) {}
void fnGetIntegerSignMode       (uint16_t unusedParamButMandatory) {}
void fnLog2                     (uint16_t unusedParamButMandatory) {}
void fnLog10                    (uint16_t unusedParamButMandatory) {}
void fnLn                       (uint16_t unusedParamButMandatory) {}
void fnLnGamma                  (uint16_t unusedParamButMandatory) {}
void fnGamma                    (uint16_t unusedParamButMandatory) {}
void fnIDiv                     (uint16_t unusedParamButMandatory) {}
void fnIDivR                    (uint16_t unusedParamButMandatory) {}
void fnProductSign              (uint16_t unusedParamButMandatory) {}
void fnMirror                   (uint16_t unusedParamButMandatory) {}
void fnMod                      (uint16_t unusedParamButMandatory) {}
void fnPower                    (uint16_t unusedParamButMandatory) {}
void fnPi                       (uint16_t unusedParamButMandatory) {}
void fnUserMode                 (uint16_t unusedParamButMandatory) {}
void fnParallel                 (uint16_t unusedParamButMandatory) {}
void fnSquareRoot               (uint16_t unusedParamButMandatory) {}
void fnSubtract                 (uint16_t unusedParamButMandatory) {}
void fnChangeSign               (uint16_t unusedParamButMandatory) {}
void fnM1Pow                    (uint16_t unusedParamButMandatory) {}
void backToSystem               (uint16_t unusedParamButMandatory) {}
void fnMultiply                 (uint16_t unusedParamButMandatory) {}
void fnChangeBase               (uint16_t unusedParamButMandatory) {}
void fnToPolar                  (uint16_t unusedParamButMandatory) {}
void fnToRect                   (uint16_t unusedParamButMandatory) {}
void fnDivide                   (uint16_t unusedParamButMandatory) {}
void fnAdd                      (uint16_t unusedParamButMandatory) {}
void fnSigma                    (uint16_t unusedParamButMandatory) {}
void fnXLessThan                (uint16_t unusedParamButMandatory) {}
void fnGetLocR                  (uint16_t unusedParamButMandatory) {}
void fnSwapRealImaginary        (uint16_t unusedParamButMandatory) {}
void fnGetRoundingMode          (uint16_t unusedParamButMandatory) {}
void fnSetWordSize              (uint16_t unusedParamButMandatory) {}
void fnGetWordSize              (uint16_t unusedParamButMandatory) {}
void fnGetStackSize             (uint16_t unusedParamButMandatory) {}
void fnStackSize                (uint16_t unusedParamButMandatory) {}
void fnStore                    (uint16_t unusedParamButMandatory) {}
void fnStoreConfig              (uint16_t unusedParamButMandatory) {}
void fnStoreElement             (uint16_t unusedParamButMandatory) {}
void fnStoreIJ                  (uint16_t unusedParamButMandatory) {}
void fnStoreStack               (uint16_t unusedParamButMandatory) {}
void fnStoreAdd                 (uint16_t unusedParamButMandatory) {}
void fnStoreSub                 (uint16_t unusedParamButMandatory) {}
void fnStoreMult                (uint16_t unusedParamButMandatory) {}
void fnStoreDiv                 (uint16_t unusedParamButMandatory) {}
void fnStoreMax                 (uint16_t unusedParamButMandatory) {}
void fnStoreMin                 (uint16_t unusedParamButMandatory) {}
void fnUlp                      (uint16_t unusedParamButMandatory) {}
void fnUnitVector               (uint16_t unusedParamButMandatory) {}
void fnVersion                  (uint16_t unusedParamButMandatory) {}
void fnSquare                   (uint16_t unusedParamButMandatory) {}
void fnCube                     (uint16_t unusedParamButMandatory) {}
void fnFactorial                (uint16_t unusedParamButMandatory) {}
void fnSwapXY                   (uint16_t unusedParamButMandatory) {}
void fnWho                      (uint16_t unusedParamButMandatory) {}
void fnGetSignificantDigits     (uint16_t unusedParamButMandatory) {}
void fnCvtToCurrentAngularMode  (uint16_t unusedParamButMandatory) {}
void fnCvtAcreM2                (uint16_t unusedParamButMandatory) {}
void fnCvtAcreusM2              (uint16_t unusedParamButMandatory) {}
void fnCvtAtmPa                 (uint16_t unusedParamButMandatory) {}
void fnCvtAuM                   (uint16_t unusedParamButMandatory) {}
void fnCvtBarPa                 (uint16_t unusedParamButMandatory) {}
void fnCvtBtuJ                  (uint16_t unusedParamButMandatory) {}
void fnCvtCalJ                  (uint16_t unusedParamButMandatory) {}
void fnCvtCwtKg                 (uint16_t unusedParamButMandatory) {}
void fnCvtFtM                   (uint16_t unusedParamButMandatory) {}
void fnCvtDegToRad              (uint16_t unusedParamButMandatory) {}
void fnCvtSfeetM                (uint16_t unusedParamButMandatory) {}
void fnCvtFlozukM3              (uint16_t unusedParamButMandatory) {}
void fnCvtFlozusM3              (uint16_t unusedParamButMandatory) {}
void fnCvtGalukM3               (uint16_t unusedParamButMandatory) {}
void fnCvtGalusM3               (uint16_t unusedParamButMandatory) {}
void fnCvtDbRatio               (uint16_t unusedParamButMandatory) {}
void fnCvtRatioDb               (uint16_t unusedParamButMandatory) {}
void fnCvtHpeW                  (uint16_t unusedParamButMandatory) {}
void fnCvtHpmW                  (uint16_t unusedParamButMandatory) {}
void fnCvtHpukW                 (uint16_t unusedParamButMandatory) {}
void fnCvtInhgPa                (uint16_t unusedParamButMandatory) {}
void fnCvtInchM                 (uint16_t unusedParamButMandatory) {}
void fnCvtWhJ                   (uint16_t unusedParamButMandatory) {}
void fnCvtLbKg                  (uint16_t unusedParamButMandatory) {}
void fnCvtOzKg                  (uint16_t unusedParamButMandatory) {}
void fnCvtStoneKg               (uint16_t unusedParamButMandatory) {}
void fnCvtShorttonKg            (uint16_t unusedParamButMandatory) {}
void fnCvtTrozKg                (uint16_t unusedParamButMandatory) {}
void fnCvtLbfN                  (uint16_t unusedParamButMandatory) {}
void fnCvtMiM                   (uint16_t unusedParamButMandatory) {}
void fnCvtLyM                   (uint16_t unusedParamButMandatory) {}
void fnCvtTonKg                 (uint16_t unusedParamButMandatory) {}
void fnCvtTorrPa                (uint16_t unusedParamButMandatory) {}
void fnCvtYardM                 (uint16_t unusedParamButMandatory) {}
void fnCvtPcM                   (uint16_t unusedParamButMandatory) {}
void fnCvtPointM                (uint16_t unusedParamButMandatory) {}
void fnCvtCToF                  (uint16_t unusedParamButMandatory) {}
void fnCvtFToC                  (uint16_t unusedParamButMandatory) {}
void fnCvtNmiM                  (uint16_t unusedParamButMandatory) {}
void fnCvtPsiPa                 (uint16_t unusedParamButMandatory) {}
void fnCvtShortcwtKg            (uint16_t unusedParamButMandatory) {}
void fnCvtRadToDeg              (uint16_t unusedParamButMandatory) {}
void fnCvtFromCurrentAngularMode(uint16_t unusedParamButMandatory) {}
void fnCvtYearS                 (uint16_t unusedParamButMandatory) {}
void fnCvtCaratKg               (uint16_t unusedParamButMandatory) {}
void fnCvtQuartM3               (uint16_t unusedParamButMandatory) {}
void fnCvtDmsToDeg              (uint16_t unusedParamButMandatory) {}
void fnCvtFathomM               (uint16_t unusedParamButMandatory) {}
void fnCvtBarrelM3              (uint16_t unusedParamButMandatory) {}
void fnCvtHectareM2             (uint16_t unusedParamButMandatory) {}
void fnCvtDegToDms              (uint16_t unusedParamButMandatory) {}
void addItemToBuffer            (uint16_t unusedParamButMandatory) {}
void fnOff                      (uint16_t unusedParamButMandatory) {}
void fnAim                      (uint16_t unusedParamButMandatory) {}
void fnComplexCCCC              (uint16_t unusedParamButMandatory) {}
void registerBrowser            (uint16_t unusedParamButMandatory) {}
void flagBrowser                (uint16_t unusedParamButMandatory) {}
void fontBrowser                (uint16_t unusedParamButMandatory) {}
void fnLastX                    (uint16_t unusedParamButMandatory) {}           //vv JM
void fnJM                       (uint16_t unusedParamButMandatory) {}
void fnComplexCCCC_CPX          (uint16_t unusedParamButMandatory) {}
void fnComplexCCCC_CC           (uint16_t unusedParamButMandatory) {}
void fnComplexCCCC_CC1          (uint16_t unusedParamButMandatory) {}
void fnCvtmmhgPa                (uint16_t unusedParamButMandatory) {}
void fnSetSetJM                 (uint16_t unusedParamButMandatory) {}
void fnDisplayFormatSigFig      (uint16_t unusedParamButMandatory) {}
void fnDisplayFormatUnit        (uint16_t unusedParamButMandatory) {}
void fnShowJM                   (uint16_t unusedParamButMandatory) {}
void fnJMup                     (uint16_t unusedParamButMandatory) {}
void fnJMdown                   (uint16_t unusedParamButMandatory) {}
void fnUserJM                   (uint16_t unusedParamButMandatory) {}
void fnSigmaAssign              (uint16_t unusedParamButMandatory) {}
void fnGetSigmaAssignToX        (uint16_t unusedParamButMandatory) {}
void fnJM_GetXToNORMmode        (uint16_t unusedParamButMandatory) {}
void fnInDefault                (uint16_t unusedParamButMandatory) {}
void fnJMUSERmode               (uint16_t unusedParamButMandatory) {}
void fnJMUSERmode_f             (uint16_t unusedParamButMandatory) {}
void fnJMUSERmode_g             (uint16_t unusedParamButMandatory) {}
void fnBASE_Hash                (uint16_t unusedParamButMandatory) {}
void graph                      (uint16_t unusedParamButMandatory) {}           //^^
#endif

const item_t indexOfItems[] = {
//            *func                        param                        itemCatalogName                                itemSoftmenuName                               catalog.  stackLiftStatus      //JM
//            function                     parameter                    item in catalog                                item in softmenu                               CATALOG   stackLift
/*    0 */  { itemToBeCoded,               NOPARAM,                     "",                                            "0000",                                        CAT_NONE, SLS_UNCHANGED},
/*    1 */  { fnCvtCToF,                   NOPARAM,                     STD_DEGREE "C" STD_RIGHT_ARROW STD_DEGREE "F", STD_DEGREE "C" STD_RIGHT_ARROW STD_DEGREE "F", CAT_FNCT, SLS_ENABLED  },
/*    2 */  { fnCvtFToC,                   NOPARAM,                     STD_DEGREE "F" STD_RIGHT_ARROW STD_DEGREE "C", STD_DEGREE "F" STD_RIGHT_ARROW STD_DEGREE "C", CAT_FNCT, SLS_ENABLED  },
/*    3 */  { fn10Pow,                     NOPARAM,                     "10" STD_SUP_x,                                "10" STD_SUP_x,                                CAT_FNCT, SLS_ENABLED  },
/*    4 */  { fnIntegerMode,               SIM_1COMPL,                  "1COMPL",                                      "1COMPL",                                      CAT_FNCT, SLS_UNCHANGED},
/*    5 */  { fnConstant,                  0,                           STD_ONE_HALF,                                  STD_ONE_HALF,                                  CAT_CNST, SLS_ENABLED  },
/*    6 */  { fnInvert,                    NOPARAM,                     "1/x",                                         "1/x",                                         CAT_FNCT, SLS_ENABLED  },
/*    7 */  { fnIntegerMode,               SIM_2COMPL,                  "2COMPL",                                      "2COMPL",                                      CAT_FNCT, SLS_UNCHANGED},
/*    8 */  { fn2Pow,                      NOPARAM,                     "2" STD_SUP_x,                                 "2" STD_SUP_x,                                 CAT_FNCT, SLS_ENABLED  },
/*    9 */  { fnCubeRoot,                  NOPARAM,                     STD_CUBE_ROOT STD_x_UNDER_ROOT,                STD_CUBE_ROOT STD_x_UNDER_ROOT,                CAT_FNCT, SLS_ENABLED  },
/*   10 */  { itemToBeCoded,               NOPARAM,                     "A",                                           "A",                                           CAT_RVAR, SLS_UNCHANGED},
/*   11 */  { fnConstant,                  1,                           "a",                                           "a",                                           CAT_CNST, SLS_ENABLED  },
/*   12 */  { fnConstant,                  2,                           "a" STD_SUB_0,                                 "a" STD_SUB_0,                                 CAT_CNST, SLS_ENABLED  },
/*   13 */  { fnMagnitude,                 NOPARAM,                     "ABS",                                         "ABS",                                         CAT_FNCT, SLS_ENABLED  },
/*   14 */  { itemToBeCoded,               NOPARAM,                     "ACC",                                         "ACC",                                         CAT_RVAR, SLS_UNCHANGED},
/*   15 */  { fnCvtAcreM2,                 multiply,                    "ac" STD_RIGHT_ARROW "m" STD_SUP_2,            "acre",                                        CAT_FNCT, SLS_ENABLED  },
/*   16 */  { fnCvtAcreusM2,               multiply,                    "ac" STD_US STD_RIGHT_ARROW "m" STD_SUP_2,     "acre" STD_US,                                 CAT_FNCT, SLS_ENABLED  },
/*   17 */  { itemToBeCoded,               NOPARAM,                     "ADV",                                         "ADV",                                         CAT_MENU, SLS_UNCHANGED},
/*   18 */  { fnAgm,                       NOPARAM,                     "AGM",                                         "AGM",                                         CAT_FNCT, SLS_ENABLED  },
/*   19 */  { itemToBeCoded,               NOPARAM,                     "AGRAPH",                                      "AGRAPH",                                      CAT_FNCT, SLS_UNCHANGED},
/*   20 */  { fnDisplayFormatAll,          TM_VALUE,                    "ALL" ,                                        "ALL",                                         CAT_FNCT, SLS_UNCHANGED},
/*   21 */  { fnConstant,                  3,                           "a" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "a" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   CAT_CNST, SLS_ENABLED  },
/*   22 */  { itemToBeCoded,               NOPARAM,                     "AND",                                         "AND",                                         CAT_FNCT, SLS_UNCHANGED},
/*   23 */  { itemToBeCoded,               NOPARAM,                     "ANGLES",                                      "ANGLES",                                      CAT_MENU, SLS_UNCHANGED},
/*   24 */  { fnArccos,                    NOPARAM,                     "ARCCOS",                                      "ACOS",                                        CAT_FNCT, SLS_ENABLED  },      //JM
/*   25 */  { fnArccosh,                   NOPARAM,                     "arcosh",                                      "arcosh",                                      CAT_FNCT, SLS_ENABLED  },
/*   26 */  { fnArcsin,                    NOPARAM,                     "ARCSIN",                                      "ASIN",                                        CAT_FNCT, SLS_ENABLED  },      //JM
/*   27 */  { fnArctan,                    NOPARAM,                     "ARCTAN",                                      "ATAN",                                        CAT_FNCT, SLS_ENABLED  },      //JM
/*   28 */  { fnArcsinh,                   NOPARAM,                     "arsinh",                                      "arsinh",                                      CAT_FNCT, SLS_ENABLED  },
/*   29 */  { fnArctanh,                   NOPARAM,                     "artanh",                                      "artanh",                                      CAT_FNCT, SLS_ENABLED  },
/*   30 */  { itemToBeCoded,               NOPARAM,                     "ASR",                                         "ASR",                                         CAT_FNCT, SLS_UNCHANGED},
/*   31 */  { itemToBeCoded,               NOPARAM,                     "ASSIGN",                                      "ASN",                                         CAT_FNCT, SLS_UNCHANGED},
/*   32 */  { fnCvtAtmPa,                  multiply,                    "atm" STD_RIGHT_ARROW "Pa",                    "atm" STD_RIGHT_ARROW "Pa",                    CAT_FNCT, SLS_ENABLED  },
/*   33 */  { fnCvtAuM,                    multiply,                    "au" STD_RIGHT_ARROW "m",                      "au" STD_RIGHT_ARROW "m",                      CAT_FNCT, SLS_ENABLED  },
/*   34 */  { itemToBeCoded,               NOPARAM,                     "0034",                                        "0034",                                        CAT_FREE, SLS_UNCHANGED},
/*   35 */  { itemToBeCoded,               NOPARAM,                     "A:",                                          "A:",                                          CAT_MENU, SLS_UNCHANGED},
/*   36 */  { fnConstant,                  4,                           "a" STD_SUB_EARTH,                             "a" STD_SUB_EARTH,                             CAT_CNST, SLS_ENABLED  },
/*   37 */  { itemToBeCoded,               NOPARAM,                     "B",                                           "B",                                           CAT_RVAR, SLS_UNCHANGED},
/*   38 */  { itemToBeCoded,               NOPARAM,                     "BACK",                                        "BACK",                                        CAT_FNCT, SLS_UNCHANGED},
/*   39 */  { fnCvtBarPa,                  multiply,                    "bar" STD_RIGHT_ARROW "Pa",                    "bar" STD_RIGHT_ARROW "Pa",                    CAT_FNCT, SLS_ENABLED  },
/*   40 */  { fnBatteryVoltage,            NOPARAM,                     "BATT?",                                       "BATT?",                                       CAT_FNCT, SLS_ENABLED  },
/*   41 */  { itemToBeCoded,               NOPARAM,                     "BC?",                                         "BC?",                                         CAT_FNCT, SLS_UNCHANGED},
/*   42 */  { itemToBeCoded,               NOPARAM,                     "BEEP",                                        "BEEP",                                        CAT_FNCT, SLS_UNCHANGED},
/*   43 */  { itemToBeCoded,               NOPARAM,                     "BeginP",                                      "Begin",                                       CAT_FNCT, SLS_UNCHANGED},
/*   44 */  { fnCurveFitting,              CF_BEST_FITTING,             "BestF",                                       "BestF",                                       CAT_FNCT, SLS_UNCHANGED},
/*   45 */  { itemToBeCoded,               NOPARAM,                     "Binom",                                       "Binom",                                       CAT_FNCT, SLS_UNCHANGED},
/*   46 */  { itemToBeCoded,               NOPARAM,                     "Binom" STD_SUB_p,                             "Binom" STD_SUB_p,                             CAT_FNCT, SLS_UNCHANGED},
/*   47 */  { itemToBeCoded,               NOPARAM,                     "Binom" STD_SUB_e,                             "Binom" STD_SUB_e,                             CAT_FNCT, SLS_UNCHANGED},
/*   48 */  { itemToBeCoded,               NOPARAM,                     "Binom" STD_SUP_MINUS_1,                       "Binom" STD_SUP_MINUS_1,                       CAT_FNCT, SLS_UNCHANGED},
/*   49 */  { itemToBeCoded,               NOPARAM,                     "Binom:",                                      "Binom:",                                      CAT_MENU, SLS_UNCHANGED},
/*   50 */  { itemToBeCoded,               NOPARAM,                     "BITS",                                        "BITS",                                        CAT_MENU, SLS_UNCHANGED},
/*   51 */  { itemToBeCoded,               NOPARAM,                     "B" STD_SUB_n,                                 "B" STD_SUB_n,                                 CAT_FNCT, SLS_UNCHANGED},
/*   52 */  { itemToBeCoded,               NOPARAM,                     "B" STD_SUB_n STD_SUP_ASTERISK,                "B" STD_SUB_n STD_SUP_ASTERISK,                CAT_FNCT, SLS_UNCHANGED},
/*   53 */  { itemToBeCoded,               NOPARAM,                     "BS?",                                         "BS?",                                         CAT_FNCT, SLS_UNCHANGED},
/*   54 */  { fnCvtBtuJ,                   multiply,                    "Btu" STD_RIGHT_ARROW "J",                     "Btu" STD_RIGHT_ARROW "J",                     CAT_FNCT, SLS_ENABLED  },
/*   55 */  { itemToBeCoded,               NOPARAM,                     "C",                                           "C",                                           CAT_RVAR, SLS_UNCHANGED},
/*   56 */  { fnConstant,                  5,                           "c",                                           "c",                                           CAT_CNST, SLS_ENABLED  },
/*   57 */  { fnConstant,                  6,                           "c" STD_SUB_1,                                 "c" STD_SUB_1,                                 CAT_CNST, SLS_ENABLED  },
/*   58 */  { fnConstant,                  7,                           "c" STD_SUB_2,                                 "c" STD_SUB_2,                                 CAT_CNST, SLS_ENABLED  },
/*   59 */  { fnCvtCalJ,                   multiply,                    "cal" STD_RIGHT_ARROW "J",                     "cal" STD_RIGHT_ARROW "J",                     CAT_FNCT, SLS_ENABLED  },
/*   60 */  { itemToBeCoded,               NOPARAM,                     "CASE",                                        "CASE",                                        CAT_FNCT, SLS_UNCHANGED},
/*   61 */  { itemToBeCoded,               NOPARAM,                     "CATALOG",                                     "CAT",                                         CAT_MENU, SLS_UNCHANGED},
/*   62 */  { itemToBeCoded,               NOPARAM,                     "Cauch",                                       "Cauch",                                       CAT_FNCT, SLS_UNCHANGED},
/*   63 */  { itemToBeCoded,               NOPARAM,                     "Cauch" STD_SUB_p,                             "Cauch" STD_SUB_p,                             CAT_FNCT, SLS_UNCHANGED},
/*   64 */  { itemToBeCoded,               NOPARAM,                     "Cauch" STD_SUB_e,                             "Cauch" STD_SUB_e,                             CAT_FNCT, SLS_UNCHANGED},
/*   65 */  { itemToBeCoded,               NOPARAM,                     "Cauch" STD_SUP_MINUS_1,                       "Cauch" STD_SUP_MINUS_1,                       CAT_FNCT, SLS_UNCHANGED},
/*   66 */  { itemToBeCoded,               NOPARAM,                     "Cauch:",                                      "Cauch:",                                      CAT_MENU, SLS_UNCHANGED},
/*   67 */  { itemToBeCoded,               NOPARAM,                     "CB",                                          "CB",                                          CAT_FNCT, SLS_UNCHANGED},
/*   68 */  { fnCeil,                      NOPARAM,                     "CEIL",                                        "CEIL",                                        CAT_FNCT, SLS_ENABLED  },
/*   69 */  { fnClearFlag,                 TM_FLAG,                     "CF",                                          "CF",                                          CAT_FNCT, SLS_UNCHANGED},
/*   70 */  { itemToBeCoded,               NOPARAM,                     "CHARS",                                       "CHARS",                                       CAT_MENU, SLS_UNCHANGED},
/*   71 */  { fnClAll,                     NOT_CONFIRMED,               "CLALL",                                       "CLall",                                       CAT_FNCT, SLS_UNCHANGED},
/*   72 */  { itemToBeCoded,               NOPARAM,                     "CLCVAR",                                      "CLCVAR",                                      CAT_FNCT, SLS_UNCHANGED},
/*   73 */  { fnClFAll,                    NOPARAM,                     "CLFALL",                                      "CLFall",                                      CAT_FNCT, SLS_UNCHANGED},
/*   74 */  { itemToBeCoded,               NOPARAM,                     "CLK",                                         "CLK",                                         CAT_MENU, SLS_UNCHANGED},
/*   75 */  { fnTimeFormat,                TF_H12,                      "CLK12",                                       "CLK12",                                       CAT_FNCT, SLS_UNCHANGED},
/*   76 */  { fnTimeFormat,                TF_H24,                      "CLK24",                                       "CLK24",                                       CAT_FNCT, SLS_UNCHANGED},
/*   77 */  { itemToBeCoded,               NOPARAM,                     "CLLCD",                                       "CLLCD",                                       CAT_FNCT, SLS_UNCHANGED},
/*   78 */  { itemToBeCoded,               NOPARAM,                     "CLMENU",                                      "CLMENU",                                      CAT_FNCT, SLS_UNCHANGED},
/*   79 */  { itemToBeCoded,               NOPARAM,                     "CLP",                                         "CLP",                                         CAT_FNCT, SLS_UNCHANGED},
/*   80 */  { fnClPAll,                    NOT_CONFIRMED,               "CLPALL",                                      "CLPall",                                      CAT_FNCT, SLS_UNCHANGED},
/*   81 */  { itemToBeCoded,               NOPARAM,                     "CLR",                                         "CLR",                                         CAT_MENU, SLS_UNCHANGED},
/*   82 */  { fnClearRegisters,            NOPARAM,                     "CLREGS",                                      "CLREGS",                                      CAT_FNCT, SLS_UNCHANGED},
/*   83 */  { fnClearStack,                NOPARAM,                     "CLSTK",                                       "CLSTK",                                       CAT_FNCT, SLS_UNCHANGED},
/*   84 */  { fnClX,                       NOPARAM,                     "CLX",                                         "CLX",                                         CAT_FNCT, SLS_DISABLED },
/*   85 */  { fnClSigma,                   NOPARAM,                     "CL" STD_SIGMA,                                "CL" STD_SIGMA,                                CAT_FNCT, SLS_UNCHANGED},
/*   86 */  { itemToBeCoded,               NOPARAM,                     "CNST",                                        "CNST",                                        CAT_MENU, SLS_UNCHANGED},
/*   87 */  { itemToBeCoded,               NOPARAM,                     "COMB",                                        "C" STD_SUB_y STD_SUB_x,                       CAT_FNCT, SLS_UNCHANGED},
/*   88 */  { fnConjugate,                 NOPARAM,                     "CONJ",                                        "conj",                                        CAT_FNCT, SLS_ENABLED  },
/*   89 */  { itemToBeCoded,               NOPARAM,                     "CONST",                                       "CONST",                                       CAT_FNCT, SLS_UNCHANGED},
/*   90 */  { itemToBeCoded,               NOPARAM,                     "CONVG?",                                      "CONVG?",                                      CAT_FNCT, SLS_UNCHANGED},
/*   91 */  { itemToBeCoded,               NOPARAM,                     "CORR",                                        "r",                                           CAT_FNCT, SLS_UNCHANGED},
/*   92 */  { fnCos,                       NOPARAM,                     "COS",                                         "COS",                                         CAT_FNCT, SLS_ENABLED  },      //JM
/*   93 */  { fnCosh,                      NOPARAM,                     "cosh",                                        "cosh",                                        CAT_FNCT, SLS_ENABLED  },
/*   94 */  { itemToBeCoded,               NOPARAM,                     "COV",                                         "cov",                                         CAT_FNCT, SLS_UNCHANGED},
/*   95 */  { itemToBeCoded,               NOPARAM,                     "CPX",                                         "CPX",                                         CAT_MENU, SLS_UNCHANGED},
/*   96 */  { fnComplexUnit,               CU_I,                        "CPXi",                                        "CPXi",                                        CAT_FNCT, SLS_UNCHANGED},
/*   97 */  { fnComplexUnit,               CU_J,                        "CPXj",                                        "CPXj",                                        CAT_FNCT, SLS_UNCHANGED},
/*   98 */  { fnComplexResult,             true,                        "CPXRES",                                      "CPXRES",                                      CAT_FNCT, SLS_UNCHANGED},
/*   99 */  { itemToBeCoded,               NOPARAM,                     "CPXS",                                        "CPXS",                                        CAT_MENU, SLS_UNCHANGED},
/*  100 */  { itemToBeCoded,               NOPARAM,                     "CPX?",                                        "CPX?",                                        CAT_FNCT, SLS_UNCHANGED},
/*  101 */  { itemToBeCoded,               NOPARAM,                     "CROSS",                                       "cross",                                       CAT_FNCT, SLS_UNCHANGED},
/*  102 */  { fnCvtCwtKg,                  multiply,                    "cwt" STD_RIGHT_ARROW "kg",                    "cwt" STD_RIGHT_ARROW "kg",                    CAT_FNCT, SLS_ENABLED  },
/*  103 */  { fnCxToRe,                    NOPARAM,                     "CX" STD_RIGHT_ARROW "RE",                     "CX" STD_RIGHT_ARROW "RE",                     CAT_FNCT, SLS_ENABLED  },
/*  104 */  { itemToBeCoded,               NOPARAM,                     "D",                                           "D",                                           CAT_RVAR, SLS_UNCHANGED},
/*  105 */  { itemToBeCoded,               NOPARAM,                     "DATE",                                        "DATE",                                        CAT_FNCT, SLS_UNCHANGED},
/*  106 */  { itemToBeCoded,               NOPARAM,                     "DATES",                                       "DATES",                                       CAT_MENU, SLS_UNCHANGED},
/*  107 */  { itemToBeCoded,               NOPARAM,                     "DATE" STD_RIGHT_ARROW,                        "DATE" STD_RIGHT_ARROW,                        CAT_FNCT, SLS_UNCHANGED},
/*  108 */  { itemToBeCoded,               NOPARAM,                     "DAY",                                         "DAY",                                         CAT_FNCT, SLS_UNCHANGED},
/*  109 */  { itemToBeCoded,               NOPARAM,                     "DBL?",                                        "DBL?",                                        CAT_FNCT, SLS_UNCHANGED},
/*  110 */  { itemToBeCoded,               NOPARAM,                     "DBLR",                                        "DBLR",                                        CAT_FNCT, SLS_UNCHANGED},
/*  111 */  { itemToBeCoded,               NOPARAM,                     "DBL" STD_CROSS,                               "DBL" STD_CROSS,                               CAT_FNCT, SLS_UNCHANGED},
/*  112 */  { itemToBeCoded,               NOPARAM,                     "DBL/",                                        "DBL/",                                        CAT_FNCT, SLS_UNCHANGED},
/*  113 */  { fnCvtDbRatio,                20,                          "dB" STD_RIGHT_ARROW "fr",                     "dB " STD_RIGHT_ARROW,                         CAT_FNCT, SLS_ENABLED  },
/*  114 */  { fnCvtDbRatio,                10,                          "dB" STD_RIGHT_ARROW "pr",                     "dB " STD_RIGHT_ARROW,                         CAT_FNCT, SLS_ENABLED  },
/*  115 */  { itemToBeCoded,               NOPARAM,                     "DEC",                                         "DEC",                                         CAT_FNCT, SLS_UNCHANGED},
/*  116 */  { itemToBeCoded,               NOPARAM,                     "DECOMP",                                      "DECOMP",                                      CAT_FNCT, SLS_UNCHANGED},
/*  117 */  { fnAngularMode,               AM_DEGREE,                   "DEG",                                         "DEG",                                         CAT_FNCT, SLS_UNCHANGED},
/*  118 */  { fnCvtToCurrentAngularMode,   AM_DEGREE,                   "DEG" STD_RIGHT_ARROW,                         "DEG" STD_RIGHT_ARROW,                         CAT_FNCT, SLS_ENABLED  },
/*  119 */  { fnDenMode,                   DM_ANY,                      "DENANY",                                      "DENANY",                                      CAT_FNCT, SLS_UNCHANGED},
/*  120 */  { fnDenMode,                   DM_FAC,                      "DENFAC",                                      "DENFAC",                                      CAT_FNCT, SLS_UNCHANGED},
/*  121 */  { fnDenMode,                   DM_FIX,                      "DENFIX",                                      "DENFIX",                                      CAT_FNCT, SLS_UNCHANGED},
/*  122 */  { fnDenMax,                    NOPARAM,                     "DENMAX",                                      "DENMAX",                                      CAT_FNCT, SLS_UNCHANGED},
/*  123 */  { itemToBeCoded,               NOPARAM,                     "DIGITS",                                      "DIGITS",                                      CAT_MENU, SLS_UNCHANGED},
/*  124 */  { itemToBeCoded,               NOPARAM,                     "DISP",                                        "DISP",                                        CAT_MENU, SLS_UNCHANGED},
/*  125 */  { itemToBeCoded,               NOPARAM,                     "DOT",                                         "dot",                                         CAT_FNCT, SLS_UNCHANGED},
/*  126 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "DP",                          STD_RIGHT_ARROW "DP",                          CAT_FNCT, SLS_ENABLED  },
/*  127 */  { fnDrop,                      NOPARAM,                     "DROP",                                        "DROP" STD_DOWN_ARROW,                         CAT_FNCT, SLS_ENABLED  },
/*  128 */  { fnDropY,                     NOPARAM,                     "DROPy",                                       "DROPy",                                       CAT_FNCT, SLS_ENABLED  },
/*  129 */  { itemToBeCoded,               NOPARAM,                     "DSE",                                         "DSE",                                         CAT_FNCT, SLS_UNCHANGED},
/*  130 */  { itemToBeCoded,               NOPARAM,                     "DSL",                                         "DSL",                                         CAT_FNCT, SLS_UNCHANGED},
/*  131 */  { fnConstant,                  60,                          STD_DELTA STD_nu STD_SUB_C STD_SUB_s,          STD_DELTA STD_nu STD_SUB_C STD_SUB_s,          CAT_CNST, SLS_ENABLED  },
/*  132 */  { fnDisplayStack,              TM_VALUE,                    "DSTACK",                                      "DSTACK",                                      CAT_FNCT, SLS_UNCHANGED},
/*  133 */  { itemToBeCoded,               NOPARAM,                     "DSZ",                                         "DSZ",                                         CAT_FNCT, SLS_UNCHANGED},
/*  134 */  { fnAngularMode,               AM_DMS,                      "D.MS",                                        "d.ms",                                        CAT_FNCT, SLS_UNCHANGED},
/*  135 */  { fnCvtToCurrentAngularMode,   AM_DMS,                      "D.MS" STD_RIGHT_ARROW,                        "D.MS" STD_RIGHT_ARROW,                        CAT_FNCT, SLS_ENABLED  },
/*  136 */  { fnDateFormat,                DF_DMY,                      "D.MY",                                        "D.MY",                                        CAT_FNCT, SLS_UNCHANGED},
/*  137 */  { itemToBeCoded,               NOPARAM,                     "D" STD_RIGHT_ARROW "J",                       "D" STD_RIGHT_ARROW "J",                       CAT_FNCT, SLS_UNCHANGED},
/*  138 */  { fnCvtDegToRad,               NOPARAM,                     "D" STD_RIGHT_ARROW "R",                       "D" STD_RIGHT_ARROW "R",                       CAT_FNCT, SLS_ENABLED  },
/*  139 */  { fnConstant,                  8,                           "e",                                           "e",                                           CAT_CNST, SLS_ENABLED  },
/*  140 */  { fnConstant,                  9,                           "e" STD_SUB_E,                                 "e" STD_SUB_E,                                 CAT_CNST, SLS_ENABLED  },
/*  141 */  { itemToBeCoded,               NOPARAM,                     "EIGVAL",                                      "EIGVAL",                                      CAT_FNCT, SLS_UNCHANGED},
/*  142 */  { itemToBeCoded,               NOPARAM,                     "EIGVEC",                                      "EIGVEC",                                      CAT_FNCT, SLS_UNCHANGED},
/*  143 */  { itemToBeCoded,               NOPARAM,                     "END",                                         "END",                                         CAT_FNCT, SLS_UNCHANGED},
/*  144 */  { itemToBeCoded,               NOPARAM,                     "ENDP",                                        "End",                                         CAT_FNCT, SLS_UNCHANGED},
/*  145 */  { fnDisplayFormatEng,          TM_VALUE,                    "ENG",                                         "ENG",                                         CAT_FNCT, SLS_UNCHANGED},
/*  146 */  { fnDisplayOvr,                DO_ENG,                      "ENGOVR",                                      "ENGOVR",                                      CAT_FNCT, SLS_UNCHANGED},
/*  147 */  { itemToBeCoded,               NOPARAM,                     "ENORM",                                       "ENORM",                                       CAT_FNCT, SLS_UNCHANGED},
/*  148 */  { fnEnter,                     NOPARAM,                     "ENTER" STD_UP_ARROW,                          "ENTER" STD_UP_ARROW,                          CAT_FNCT, SLS_DISABLED },
/*  149 */  { itemToBeCoded,               NOPARAM,                     "ENTRY?",                                      "ENTRY?",                                      CAT_FNCT, SLS_UNCHANGED},
/*  150 */  { itemToBeCoded,               NOPARAM,                     "EQN",                                         "EQN",                                         CAT_MENU, SLS_UNCHANGED},
/*  151 */  { itemToBeCoded,               NOPARAM,                     "EQ.DEL",                                      "DELETE",                                      CAT_FNCT, SLS_UNCHANGED},
/*  152 */  { itemToBeCoded,               NOPARAM,                     "EQ.EDI",                                      "EDIT",                                        CAT_FNCT, SLS_UNCHANGED},
/*  153 */  { itemToBeCoded,               NOPARAM,                     "EQ.NEW",                                      "NEW",                                         CAT_FNCT, SLS_UNCHANGED},
/*  154 */  { itemToBeCoded,               NOPARAM,                     "erf",                                         "erf",                                         CAT_FNCT, SLS_UNCHANGED},
/*  155 */  { itemToBeCoded,               NOPARAM,                     "erfc",                                        "erfc",                                        CAT_FNCT, SLS_UNCHANGED},
/*  156 */  { itemToBeCoded,               NOPARAM,                     "ERR",                                         "ERR",                                         CAT_FNCT, SLS_UNCHANGED},
/*  157 */  { itemToBeCoded,               NOPARAM,                     "EVEN?",                                       "EVEN?",                                       CAT_FNCT, SLS_UNCHANGED},
/*  158 */  { fnExp,                       NOPARAM,                     "e" STD_SUP_x,                                 "e" STD_SUP_x,                                 CAT_FNCT, SLS_ENABLED  },
/*  159 */  { itemToBeCoded,               NOPARAM,                     "EXITALL",                                     "EXITall",                                     CAT_FNCT, SLS_UNCHANGED},
/*  160 */  { itemToBeCoded,               NOPARAM,                     "EXP",                                         "EXP",                                         CAT_MENU, SLS_UNCHANGED},
/*  161 */  { fnCurveFitting,              CF_EXPONENTIAL_FITTING,      "ExpF",                                        "ExpF",                                        CAT_FNCT, SLS_UNCHANGED},
/*  162 */  { itemToBeCoded,               NOPARAM,                     "Expon",                                       "Expon",                                       CAT_FNCT, SLS_UNCHANGED},
/*  163 */  { itemToBeCoded,               NOPARAM,                     "Expon" STD_SUB_p,                             "Expon" STD_SUB_p,                             CAT_FNCT, SLS_UNCHANGED},
/*  164 */  { itemToBeCoded,               NOPARAM,                     "Expon" STD_SUB_e,                             "Expon" STD_SUB_e,                             CAT_FNCT, SLS_UNCHANGED},
/*  165 */  { itemToBeCoded,               NOPARAM,                     "Expon" STD_SUP_MINUS_1,                       "Expon" STD_SUP_MINUS_1,                       CAT_FNCT, SLS_UNCHANGED},
/*  166 */  { itemToBeCoded,               NOPARAM,                     "Expon:",                                      "Expon:",                                      CAT_MENU, SLS_UNCHANGED},
/*  167 */  { fnExpt,                      NOPARAM,                     "EXPT",                                        "EXPT",                                        CAT_FNCT, SLS_ENABLED  },
/*  168 */  { itemToBeCoded,               NOPARAM,                     "e" STD_SUP_x "-1",                            "e" STD_SUP_x "-1",                            CAT_FNCT, SLS_ENABLED  },
/*  169 */  { itemToBeCoded,               NOPARAM,                     "e/m" STD_SUB_e,                               "e/m" STD_SUB_e,                               CAT_NONE, SLS_ENABLED  },
/*  170 */  { itemToBeCoded,               NOPARAM,                     "E:",                                          "E:",                                          CAT_MENU, SLS_UNCHANGED},
/*  171 */  { fnConstant,                  10,                          "F",                                           "F",                                           CAT_CNST, SLS_ENABLED  },
/*  172 */  { itemToBeCoded,               NOPARAM,                     "FAST",                                        "FAST",                                        CAT_FNCT, SLS_UNCHANGED},
/*  173 */  { itemToBeCoded,               NOPARAM,                     "FB",                                          "FB",                                          CAT_FNCT, SLS_UNCHANGED},
/*  174 */  { itemToBeCoded,               NOPARAM,                     "FCNS",                                        "FCNS",                                        CAT_MENU, SLS_UNCHANGED},
/*  175 */  { fnIsFlagClear,               TM_FLAG,                     "FC?",                                         "FC?",                                         CAT_FNCT, SLS_UNCHANGED},
/*  176 */  { fnIsFlagClearClear,          TM_FLAG,                     "FC?C",                                        "FC?C",                                        CAT_FNCT, SLS_UNCHANGED},
/*  177 */  { fnIsFlagClearFlip,           TM_FLAG,                     "FC?F",                                        "FC?F",                                        CAT_FNCT, SLS_UNCHANGED},
/*  178 */  { fnIsFlagClearSet,            TM_FLAG,                     "FC?S",                                        "FC?S",                                        CAT_FNCT, SLS_UNCHANGED},
/*  179 */  { fnCvtFtM,                    multiply,                    "ft." STD_RIGHT_ARROW "m",                     "ft." STD_RIGHT_ARROW "m",                     CAT_FNCT, SLS_ENABLED  },
/*  180 */  { fnFlipFlag,                  TM_FLAG,                     "FF",                                          "FF",                                          CAT_FNCT, SLS_ENABLED  },
/*  181 */  { itemToBeCoded,               NOPARAM,                     "FIB",                                         "FIB",                                         CAT_FNCT, SLS_UNCHANGED},
/*  182 */  { fnFillStack,                 NOPARAM,                     "FILL",                                        "FILL",                                        CAT_FNCT, SLS_ENABLED  },
/*  183 */  { itemToBeCoded,               NOPARAM,                     "FIN",                                         "FIN",                                         CAT_MENU, SLS_UNCHANGED},
/*  184 */  { itemToBeCoded,               NOPARAM,                     "S.INTS",                                      "S.INTS",                                      CAT_MENU, SLS_UNCHANGED},
/*  185 */  { fnDisplayFormatFix,          TM_VALUE,                    "FIX",                                         "FIX",                                         CAT_FNCT, SLS_UNCHANGED},
/*  186 */  { itemToBeCoded,               NOPARAM,                     "FLAGS",                                       "FLAGS",                                       CAT_MENU, SLS_UNCHANGED},
/*  187 */  { itemToBeCoded,               NOPARAM,                     "FLASH",                                       "FLASH",                                       CAT_MENU, SLS_UNCHANGED},
/*  188 */  { fnFreeFlashMemory,           NOPARAM,                     "FLASH?",                                      "FLASH?",                                      CAT_FNCT, SLS_ENABLED  },
/*  189 */  { fnFloor,                     NOPARAM,                     "FLOOR",                                       "FLOOR",                                       CAT_FNCT, SLS_ENABLED  },
/*  190 */  { fnFp,                        NOPARAM,                     "FP",                                          "FP",                                          CAT_FNCT, SLS_ENABLED  },
/*  191 */  { itemToBeCoded,               NOPARAM,                     "FP?",                                         "FP?",                                         CAT_FNCT, SLS_UNCHANGED},
/*  192 */  { itemToBeCoded,               NOPARAM,                     "F" STD_SUB_p "(x)",                           "F" STD_SUB_p "(x)",                           CAT_FNCT, SLS_UNCHANGED},
/*  193 */  { itemToBeCoded,               NOPARAM,                     "F" STD_SUB_e "(x)",                           "F" STD_SUB_e "(x)",                           CAT_FNCT, SLS_UNCHANGED},
/*  194 */  { itemToBeCoded,               NOPARAM,                     "F(x)",                                        "F(x)",                                        CAT_FNCT, SLS_UNCHANGED},
/*  195 */  { itemToBeCoded,               NOPARAM,                     "F" STD_SUP_MINUS_1 "(p)",                     "F" STD_SUP_MINUS_1 "(p)",                     CAT_FNCT, SLS_UNCHANGED},
/*  196 */  { fnCvtRatioDb,                20,                          "fr" STD_RIGHT_ARROW "dB",                     "field",                                       CAT_FNCT, SLS_ENABLED  },
/*  197 */  { fnIsFlagSet,                 TM_FLAG,                     "FS?",                                         "FS?",                                         CAT_FNCT, SLS_UNCHANGED},
/*  198 */  { fnIsFlagSetClear,            TM_FLAG,                     "FS?C",                                        "FS?C",                                        CAT_FNCT, SLS_UNCHANGED},
/*  199 */  { fnIsFlagSetFlip,             TM_FLAG,                     "FS?F",                                        "FS?F",                                        CAT_FNCT, SLS_UNCHANGED},
/*  200 */  { fnIsFlagSetSet,              TM_FLAG,                     "FS?S",                                        "FS?S",                                        CAT_FNCT, SLS_UNCHANGED},
/*  201 */  { fnCvtSfeetM,                 multiply,                    "ft" STD_US STD_RIGHT_ARROW "m",               "survey",                                      CAT_FNCT, SLS_ENABLED  },
/*  202 */  { itemToBeCoded,               NOPARAM,                     "FV",                                          "FV",                                          CAT_RVAR, SLS_UNCHANGED},
/*  203 */  { fnCvtFlozukM3,               multiply,                    "fz" STD_UK STD_RIGHT_ARROW "m" STD_SUP_3,     "floz" STD_UK,                                 CAT_FNCT, SLS_ENABLED  },
/*  204 */  { fnCvtFlozusM3,               multiply,                    "fz" STD_US STD_RIGHT_ARROW "m" STD_SUP_3,     "floz" STD_US,                                 CAT_FNCT, SLS_ENABLED  },
/*  205 */  { fnConstant,                  11,                          "F" STD_SUB_alpha,                             "F" STD_SUB_alpha,                             CAT_CNST, SLS_ENABLED  },
/*  206 */  { fnConstant,                  12,                          "F" STD_SUB_delta,                             "F" STD_SUB_delta,                             CAT_CNST, SLS_ENABLED  },
/*  207 */  { itemToBeCoded,               NOPARAM,                     "F:",                                          "F:",                                          CAT_MENU, SLS_UNCHANGED},
/*  208 */  { itemToBeCoded,               NOPARAM,                     "f'",                                          "f'",                                          CAT_MENU, SLS_UNCHANGED},
/*  209 */  { itemToBeCoded,               NOPARAM,                     "f\"",                                         "f\"",                                         CAT_MENU, SLS_UNCHANGED},
/*  210 */  { itemToBeCoded,               NOPARAM,                     "f'(x)",                                       "f'(x)",                                       CAT_FNCT, SLS_UNCHANGED},
/*  211 */  { itemToBeCoded,               NOPARAM,                     "f\"(x)",                                      "f\"(x)",                                      CAT_FNCT, SLS_UNCHANGED},
/*  212 */  { itemToBeCoded,               NOPARAM,                     "F&p:",                                        "F&p:",                                        CAT_MENU, SLS_UNCHANGED},
/*  213 */  { fnConstant,                  13,                          "G",                                           "G",                                           CAT_CNST, SLS_ENABLED  },
/*  214 */  { fnConstant,                  14,                          "G" STD_SUB_0,                                 "G" STD_SUB_0,                                 CAT_CNST, SLS_ENABLED  },
/*  215 */  { fnDisplayFormatGap,          TM_VALUE,                    "GAP",                                         "GAP",                                         CAT_FNCT, SLS_UNCHANGED},
/*  216 */  { fnConstant,                  15,                          "G" STD_SUB_C,                                 "G" STD_SUB_C,                                 CAT_CNST, SLS_ENABLED  },
/*  217 */  { fnGcd,                       NOPARAM,                     "GCD",                                         "GCD",                                         CAT_FNCT, SLS_ENABLED  },
/*  218 */  { itemToBeCoded,               NOPARAM,                     "g" STD_SUB_d,                                 "g" STD_SUB_d,                                 CAT_FNCT, SLS_UNCHANGED},
/*  219 */  { itemToBeCoded,               NOPARAM,                     "g" STD_SUB_d STD_SUP_MINUS_1,                 "g" STD_SUB_d STD_SUP_MINUS_1,                 CAT_FNCT, SLS_UNCHANGED},
/*  220 */  { fnConstant,                  16,                          "g" STD_SUB_e,                                 "g" STD_SUB_e,                                 CAT_CNST, SLS_ENABLED  },
/*  221 */  { itemToBeCoded,               NOPARAM,                     "Geom",                                        "Geom",                                        CAT_FNCT, SLS_UNCHANGED},
/*  222 */  { itemToBeCoded,               NOPARAM,                     "Geom" STD_SUB_p,                              "Geom" STD_SUB_p,                              CAT_FNCT, SLS_UNCHANGED},
/*  223 */  { itemToBeCoded,               NOPARAM,                     "Geom" STD_SUB_e,                              "Geom" STD_SUB_e,                              CAT_FNCT, SLS_UNCHANGED},
/*  224 */  { itemToBeCoded,               NOPARAM,                     "Geom" STD_SUP_MINUS_1,                        "Geom" STD_SUP_MINUS_1,                        CAT_FNCT, SLS_UNCHANGED},
/*  225 */  { itemToBeCoded,               NOPARAM,                     "Geom:",                                       "Geom:",                                       CAT_MENU, SLS_UNCHANGED},
/*  226 */  { fnCvtGalukM3,                multiply,                    "gl" STD_UK STD_RIGHT_ARROW "m" STD_SUP_3,     "gl" STD_UK STD_RIGHT_ARROW "m" STD_SUP_3,     CAT_FNCT, SLS_ENABLED  },
/*  227 */  { fnCvtGalusM3,                multiply,                    "gl" STD_US STD_RIGHT_ARROW "m" STD_SUP_3,     "gl" STD_US STD_RIGHT_ARROW "m" STD_SUP_3,     CAT_FNCT, SLS_ENABLED  },
/*  228 */  { fnConstant,                  17,                          "GM" STD_SUB_EARTH,                            "GM" STD_SUB_EARTH,                            CAT_CNST, SLS_ENABLED  },
/*  229 */  { fnAngularMode,               AM_GRAD,                     "GRAD",                                        "GRAD",                                        CAT_FNCT, SLS_UNCHANGED},
/*  230 */  { fnCvtToCurrentAngularMode,   AM_GRAD,                     "GRAD" STD_RIGHT_ARROW,                        "GRAD" STD_RIGHT_ARROW,                        CAT_FNCT, SLS_ENABLED  },
/*  231 */  { itemToBeCoded,               NOPARAM,                     "GTO",                                         "GTO",                                         CAT_FNCT, SLS_UNCHANGED},
/*  232 */  { itemToBeCoded,               NOPARAM,                     "GTO.",                                        "GTO.",                                        CAT_FNCT, SLS_UNCHANGED},
/*  233 */  { fnConstant,                  18,                          "g" STD_SUB_EARTH,                             "g" STD_SUB_EARTH,                             CAT_CNST, SLS_ENABLED  },
/*  234 */  { fnConstant,                  19,                          STD_PLANCK,                                    STD_PLANCK,                                    CAT_CNST, SLS_ENABLED  },
/*  235 */  { itemToBeCoded,               NOPARAM,                     "H" STD_SUB_n,                                 "H" STD_SUB_n,                                 CAT_FNCT, SLS_UNCHANGED},
/*  236 */  { itemToBeCoded,               NOPARAM,                     "H" STD_SUB_n STD_SUB_P,                       "H" STD_SUB_n STD_SUB_P,                       CAT_FNCT, SLS_UNCHANGED},
/*  237 */  { fnCvtHpeW,                   multiply,                    "hp" STD_SUB_E STD_RIGHT_ARROW "W",            "hp" STD_SUB_E STD_RIGHT_ARROW "W",            CAT_FNCT, SLS_ENABLED  },
/*  238 */  { fnCvtHpmW,                   multiply,                    "hp" STD_SUB_M STD_RIGHT_ARROW "W",            "hp" STD_SUB_M STD_RIGHT_ARROW "W",            CAT_FNCT, SLS_ENABLED  },
/*  239 */  { fnCvtHpukW,                  multiply,                    "hp" STD_UK STD_RIGHT_ARROW "W",               "hp" STD_UK STD_RIGHT_ARROW "W",               CAT_FNCT, SLS_ENABLED  },
/*  240 */  { itemToBeCoded,               NOPARAM,                     "Hyper",                                       "Hyper",                                       CAT_FNCT, SLS_UNCHANGED},
/*  241 */  { itemToBeCoded,               NOPARAM,                     "Hyper" STD_SUB_p,                             "Hyper" STD_SUB_p,                             CAT_FNCT, SLS_UNCHANGED},
/*  242 */  { itemToBeCoded,               NOPARAM,                     "Hyper" STD_SUB_e,                             "Hyper" STD_SUB_e,                             CAT_FNCT, SLS_UNCHANGED},
/*  243 */  { itemToBeCoded,               NOPARAM,                     "Hyper" STD_SUP_MINUS_1,                       "Hyper" STD_SUP_MINUS_1,                       CAT_FNCT, SLS_UNCHANGED},
/*  244 */  { itemToBeCoded,               NOPARAM,                     "Hyper:",                                      "Hyper:",                                      CAT_MENU, SLS_UNCHANGED},
/*  245 */  { fnConstant,                  20,                          STD_PLANCK_2PI,                                STD_PLANCK_2PI,                                CAT_CNST, SLS_ENABLED  },
/*  246 */  { addItemToBuffer,             REGISTER_I,                  "I",                                           "I",                                           CAT_RVAR, SLS_UNCHANGED},
/*  247 */  { fnIDiv,                      NOPARAM,                     "IDIV",                                        "IDIV",                                        CAT_FNCT, SLS_ENABLED  },
/*  248 */  { fnCvtInhgPa,                 multiply,                    "iHg" STD_RIGHT_ARROW "Pa",                    "in.Hg",                                       CAT_FNCT, SLS_ENABLED  },
/*  249 */  { itemToBeCoded,               NOPARAM,                     "L.INTS",                                      "L.INTS",                                      CAT_MENU, SLS_UNCHANGED},
/*  250 */  { fnImaginaryPart,             NOPARAM,                     "Im",                                          "Im",                                          CAT_FNCT, SLS_ENABLED  },
/*  251 */  { fnFractionType,              FT_IMPROPER,                 "IMPFRC",                                      "d/c",                                         CAT_FNCT, SLS_UNCHANGED},
/*  252 */  { itemToBeCoded,               NOPARAM,                     "INC",                                         "INC",                                         CAT_FNCT, SLS_UNCHANGED},
/*  253 */  { itemToBeCoded,               NOPARAM,                     "INDEX",                                       "INDEX",                                       CAT_FNCT, SLS_UNCHANGED},
/*  254 */  { itemToBeCoded,               NOPARAM,                     "INFO",                                        "INFO",                                        CAT_MENU, SLS_UNCHANGED},
/*  255 */  { itemToBeCoded,               NOPARAM,                     "INPUT",                                       "INPUT",                                       CAT_FNCT, SLS_UNCHANGED},
/*  256 */  { itemToBeCoded,               NOPARAM,                     "INTS",                                        "INTS",                                        CAT_MENU, SLS_UNCHANGED},
/*  257 */  { itemToBeCoded,               NOPARAM,                     "INT?",                                        "INT?",                                        CAT_FNCT, SLS_UNCHANGED},
/*  258 */  { fnCvtInchM,                  multiply,                    "in." STD_RIGHT_ARROW "m",                     "in." STD_RIGHT_ARROW "m",                     CAT_FNCT, SLS_ENABLED  },
/*  259 */  { fnIp,                        NOPARAM,                     "IP",                                          "IP",                                          CAT_FNCT, SLS_ENABLED  },
/*  260 */  { itemToBeCoded,               NOPARAM,                     "ISE",                                         "ISE",                                         CAT_FNCT, SLS_UNCHANGED},
/*  261 */  { itemToBeCoded,               NOPARAM,                     "ISG",                                         "ISG",                                         CAT_FNCT, SLS_UNCHANGED},
/*  262 */  { itemToBeCoded,               NOPARAM,                     "ISZ",                                         "ISZ",                                         CAT_FNCT, SLS_UNCHANGED},
/*  263 */  { itemToBeCoded,               NOPARAM,                     "I" STD_SUB_x STD_SUB_y STD_SUB_z,             "I" STD_SUB_x STD_SUB_y STD_SUB_z,             CAT_FNCT, SLS_UNCHANGED},
/*  264 */  { itemToBeCoded,               NOPARAM,                     "I" STD_GAMMA STD_SUB_p,                       "I" STD_GAMMA STD_SUB_p,                       CAT_FNCT, SLS_UNCHANGED},
/*  265 */  { itemToBeCoded,               NOPARAM,                     "I" STD_GAMMA STD_SUB_q,                       "I" STD_GAMMA STD_SUB_q,                       CAT_FNCT, SLS_UNCHANGED},
/*  266 */  { itemToBeCoded,               NOPARAM,                     "I+",                                          "I+",                                          CAT_FNCT, SLS_UNCHANGED},
/*  267 */  { itemToBeCoded,               NOPARAM,                     "I-",                                          "I-",                                          CAT_FNCT, SLS_UNCHANGED},
/*  268 */  { itemToBeCoded,               NOPARAM,                     "I/O",                                         "I/O",                                         CAT_MENU, SLS_UNCHANGED},
/*  269 */  { itemToBeCoded,               NOPARAM,                     "i%/a",                                        "i%/a",                                        CAT_RVAR, SLS_UNCHANGED},
/*  270 */  { addItemToBuffer,             REGISTER_J,                  "J",                                           "J",                                           CAT_RVAR, SLS_UNCHANGED},
/*  271 */  { itemToBeCoded,               NOPARAM,                     "J" STD_SUB_y "(x)",                           "J" STD_SUB_y "(x)",                           CAT_FNCT, SLS_UNCHANGED},
/*  272 */  { itemToBeCoded,               NOPARAM,                     "J+",                                          "J+",                                          CAT_FNCT, SLS_UNCHANGED},
/*  273 */  { itemToBeCoded,               NOPARAM,                     "J-",                                          "J-",                                          CAT_FNCT, SLS_UNCHANGED},
/*  274 */  { itemToBeCoded,               NOPARAM,                     "J/G",                                         "J/G",                                         CAT_FNCT, SLS_UNCHANGED},
/*  275 */  { fnCvtBtuJ,                   divide,                      "J" STD_RIGHT_ARROW "Btu",                     "J" STD_RIGHT_ARROW "Btu",                     CAT_FNCT, SLS_ENABLED  },
/*  276 */  { fnCvtCalJ,                   divide,                      "J" STD_RIGHT_ARROW "cal",                     "J" STD_RIGHT_ARROW "cal",                     CAT_FNCT, SLS_ENABLED  },
/*  277 */  { itemToBeCoded,               NOPARAM,                     "J" STD_RIGHT_ARROW "D",                       "J" STD_RIGHT_ARROW "D",                       CAT_FNCT, SLS_UNCHANGED},
/*  278 */  { fnCvtWhJ,                    divide,                      "J" STD_RIGHT_ARROW "Wh",                      "J" STD_RIGHT_ARROW "Wh",                      CAT_FNCT, SLS_ENABLED  },
/*  279 */  { addItemToBuffer,             REGISTER_K,                  "K",                                           "K",                                           CAT_RVAR, SLS_UNCHANGED},
/*  280 */  { fnConstant,                  21,                          "k",                                           "k",                                           CAT_CNST, SLS_ENABLED  },
/*  281 */  { itemToBeCoded,               NOPARAM,                     "KEY",                                         "KEY",                                         CAT_FNCT, SLS_UNCHANGED},
/*  282 */  { itemToBeCoded,               NOPARAM,                     "KEYG",                                        "KEYG",                                        CAT_FNCT, SLS_UNCHANGED},
/*  283 */  { itemToBeCoded,               NOPARAM,                     "KEYX",                                        "KEYX",                                        CAT_FNCT, SLS_UNCHANGED},
/*  284 */  { itemToBeCoded,               NOPARAM,                     "KEY?",                                        "KEY?",                                        CAT_FNCT, SLS_UNCHANGED},
/*  285 */  { fnCvtCwtKg,                  divide,                      "kg" STD_RIGHT_ARROW "cwt",                    "kg" STD_RIGHT_ARROW "cwt",                    CAT_FNCT, SLS_ENABLED  },
/*  286 */  { fnCvtLbKg,                   divide,                      "kg" STD_RIGHT_ARROW "lb.",                    "kg" STD_RIGHT_ARROW "lb.",                    CAT_FNCT, SLS_ENABLED  },
/*  287 */  { fnCvtOzKg,                   divide,                      "kg" STD_RIGHT_ARROW "oz",                     "kg" STD_RIGHT_ARROW "oz",                     CAT_FNCT, SLS_ENABLED  },
/*  288 */  { fnCvtShortcwtKg,             divide,                      "kg" STD_RIGHT_ARROW "scw",                    "kg " STD_RIGHT_ARROW,                         CAT_FNCT, SLS_ENABLED  },
/*  289 */  { fnCvtStoneKg,                divide,                      "kg" STD_RIGHT_ARROW "sto",                    "kg " STD_RIGHT_ARROW,                         CAT_FNCT, SLS_ENABLED  },
/*  290 */  { fnCvtShorttonKg,             divide,                      "kg" STD_RIGHT_ARROW "s.t",                    "kg " STD_RIGHT_ARROW,                         CAT_FNCT, SLS_ENABLED  },
/*  291 */  { fnCvtTonKg,                  divide,                      "kg" STD_RIGHT_ARROW "ton",                    "kg" STD_RIGHT_ARROW "ton",                    CAT_FNCT, SLS_ENABLED  },
/*  292 */  { fnCvtTrozKg,                 divide,                      "kg" STD_RIGHT_ARROW "trz",                    "kg " STD_RIGHT_ARROW,                         CAT_FNCT, SLS_ENABLED  },
/*  293 */  { fnConstant,                  22,                          "K" STD_SUB_J,                                 "K" STD_SUB_J,                                 CAT_CNST, SLS_ENABLED  },
/*  294 */  { itemToBeCoded,               NOPARAM,                     "KTYP?",                                       "KTYP?",                                       CAT_FNCT, SLS_UNCHANGED},
/*  295 */  { addItemToBuffer,             REGISTER_L,                  "L",                                           "L",                                           CAT_RVAR, SLS_UNCHANGED},
/*  296 */  { fnLastX,                     NOPARAM,                     "LASTx",                                       "LASTx",                                       CAT_FNCT, SLS_UNCHANGED},      //JM LastX Jaco added the Function in stack.c
/*  297 */  { fnCvtLbfN,                   multiply,                    "lbf" STD_RIGHT_ARROW "N",                     "lbf" STD_RIGHT_ARROW "N",                     CAT_FNCT, SLS_ENABLED  },
/*  298 */  { itemToBeCoded,               NOPARAM,                     "LBL",                                         "LBL",                                         CAT_FNCT, SLS_UNCHANGED},
/*  299 */  { itemToBeCoded,               NOPARAM,                     "LBL?",                                        "LBL?",                                        CAT_FNCT, SLS_UNCHANGED},
/*  300 */  { fnCvtLbKg,                   multiply,                    "lb." STD_RIGHT_ARROW "kg",                    "lb." STD_RIGHT_ARROW "kg",                    CAT_FNCT, SLS_ENABLED  },
/*  301 */  { fnLcm,                       NOPARAM,                     "LCM",                                         "LCM",                                         CAT_FNCT, SLS_ENABLED  },
/*  302 */  { itemToBeCoded,               NOPARAM,                     "LEAP?",                                       "LEAP?",                                       CAT_FNCT, SLS_UNCHANGED},
/*  303 */  { itemToBeCoded,               NOPARAM,                     "LgNrm",                                       "LgNrm",                                       CAT_FNCT, SLS_UNCHANGED},
/*  304 */  { itemToBeCoded,               NOPARAM,                     "LgNrm" STD_SUB_p,                             "LgNrm" STD_SUB_p,                             CAT_FNCT, SLS_UNCHANGED},
/*  305 */  { itemToBeCoded,               NOPARAM,                     "LgNrm" STD_SUB_e,                             "LgNrm" STD_SUB_e,                             CAT_FNCT, SLS_UNCHANGED},
/*  306 */  { itemToBeCoded,               NOPARAM,                     "LgNrm" STD_SUP_MINUS_1,                       "LgNrm" STD_SUP_MINUS_1,                       CAT_FNCT, SLS_UNCHANGED},
/*  307 */  { itemToBeCoded,               NOPARAM,                     "LgNrm:",                                      "LgNrm:",                                      CAT_MENU, SLS_UNCHANGED},
/*  308 */  { fnCurveFitting,              CF_LINEAR_FITTING,           "LinF",                                        "LinF",                                        CAT_FNCT, SLS_UNCHANGED},
/*  309 */  { itemToBeCoded,               NOPARAM,                     "LJ",                                          "LJ",                                          CAT_FNCT, SLS_UNCHANGED},
/*  310 */  { fnLn,                        NOPARAM,                     "LN",                                          "LN",                                          CAT_FNCT, SLS_ENABLED  },      //JM3 change ln to LN
/*  311 */  { itemToBeCoded,               NOPARAM,                     "L" STD_SUB_m ,                                "L" STD_SUB_m ,                                CAT_FNCT, SLS_UNCHANGED},
/*  312 */  { itemToBeCoded,               NOPARAM,                     "LN(1+x)",                                     "ln 1+x",                                      CAT_FNCT, SLS_ENABLED  },
/*  313 */  { itemToBeCoded,               NOPARAM,                     "L" STD_SUB_m STD_SUB_alpha,                   "L" STD_SUB_m STD_SUB_alpha,                   CAT_FNCT, SLS_UNCHANGED},
/*  314 */  { itemToBeCoded,               NOPARAM,                     "LN" STD_beta,                                 "ln" STD_beta,                                 CAT_FNCT, SLS_ENABLED  },
/*  315 */  { fnLnGamma,                   NOPARAM,                     "LN" STD_GAMMA,                                "ln" STD_GAMMA,                                CAT_FNCT, SLS_ENABLED  },
/*  316 */  { itemToBeCoded,               NOPARAM,                     "LOAD",                                        "LOAD",                                        CAT_FNCT, SLS_UNCHANGED},
/*  317 */  { itemToBeCoded,               NOPARAM,                     "LOADP",                                       "LOADP",                                       CAT_FNCT, SLS_UNCHANGED},
/*  318 */  { itemToBeCoded,               NOPARAM,                     "LOADR",                                       "LOADR",                                       CAT_FNCT, SLS_UNCHANGED},
/*  319 */  { itemToBeCoded,               NOPARAM,                     "LOADSS",                                      "LOADSS",                                      CAT_FNCT, SLS_UNCHANGED},
/*  320 */  { itemToBeCoded,               NOPARAM,                     "LOAD" STD_SIGMA,                              "LOAD" STD_SIGMA,                              CAT_FNCT, SLS_UNCHANGED},
/*  321 */  { allocateLocalRegisters,      TM_VALUE,                    "LocR",                                        "LocR",                                        CAT_FNCT, SLS_UNCHANGED},
/*  322 */  { fnGetLocR,                   NOPARAM,                     "LocR?",                                       "LocR?",                                       CAT_FNCT, SLS_ENABLED  },
/*  323 */  { fnLog10,                     NOPARAM,                     "LOG" STD_SUB_10,                              "LOG",                                         CAT_FNCT, SLS_ENABLED  },      //JM Change lg to LOG
/*  324 */  { fnLog2,                      NOPARAM,                     "LOG" STD_SUB_2,                               "lb x",                                        CAT_FNCT, SLS_ENABLED  },
/*  325 */  { fnCurveFitting,              CF_LOGARITHMIC_FITTING,      "LogF",                                        "LogF",                                        CAT_FNCT, SLS_UNCHANGED},
/*  326 */  { itemToBeCoded,               NOPARAM,                     "Logis",                                       "Logis",                                       CAT_FNCT, SLS_UNCHANGED},
/*  327 */  { itemToBeCoded,               NOPARAM,                     "Logis" STD_SUB_p,                             "Logis" STD_SUB_p,                             CAT_FNCT, SLS_UNCHANGED},
/*  328 */  { itemToBeCoded,               NOPARAM,                     "Logis" STD_SUB_e,                             "Logis" STD_SUB_e,                             CAT_FNCT, SLS_UNCHANGED},
/*  329 */  { itemToBeCoded,               NOPARAM,                     "Logis" STD_SUP_MINUS_1,                       "Logis" STD_SUP_MINUS_1,                       CAT_FNCT, SLS_UNCHANGED},
/*  330 */  { itemToBeCoded,               NOPARAM,                     "Logis:",                                      "Logis:",                                      CAT_MENU, SLS_UNCHANGED},
/*  331 */  { itemToBeCoded,               NOPARAM,                     "LOG" STD_SUB_x "y",                           "log" STD_SUB_x "y",                           CAT_FNCT, SLS_UNCHANGED},
/*  332 */  { itemToBeCoded,               NOPARAM,                     "LOOP",                                        "LOOP",                                        CAT_MENU, SLS_UNCHANGED},
/*  333 */  { fnConstant,                  23,                          "l" STD_SUB_P STD_SUB_L,                       "l" STD_SUB_P STD_SUB_L,                       CAT_CNST, SLS_ENABLED  },
/*  334 */  { fnCvtLyM,                    multiply,                    "ly" STD_RIGHT_ARROW "m",                      "ly" STD_RIGHT_ARROW "m",                      CAT_FNCT, SLS_ENABLED  },
/*  335 */  { fnLeadingZeros,              false,                       "LZOFF",                                       "LZOFF",                                       CAT_FNCT, SLS_UNCHANGED},
/*  336 */  { fnLeadingZeros,              true,                        "LZON",                                        "LZON",                                        CAT_FNCT, SLS_UNCHANGED},
/*  337 */  { itemToBeCoded,               NOPARAM,                     "L.R.",                                        "L.R.",                                        CAT_FNCT, SLS_UNCHANGED},
/*  338 */  { fnCvtAcreM2,                 divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ac",            "m" STD_SUP_2 " " STD_RIGHT_ARROW,             CAT_FNCT, SLS_ENABLED  },
/*  339 */  { fnCvtAcreusM2,               divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ac" STD_US,     "m" STD_SUP_2 " " STD_RIGHT_ARROW,             CAT_FNCT, SLS_ENABLED  },
/*  340 */  { fnCvtFlozukM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "fz" STD_UK,     "m" STD_SUP_3 " " STD_RIGHT_ARROW,             CAT_FNCT, SLS_ENABLED  },
/*  341 */  { fnCvtFlozusM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "fz" STD_US,     "m" STD_SUP_3 " " STD_RIGHT_ARROW,             CAT_FNCT, SLS_ENABLED  },
/*  342 */  { fnCvtGalukM3,                divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_UK,     "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_UK,     CAT_FNCT, SLS_ENABLED  },
/*  343 */  { fnCvtGalusM3,                divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_US,     "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_US,     CAT_FNCT, SLS_ENABLED  },
/*  344 */  { fnMant,                      NOPARAM,                     "MANT",                                        "MANT",                                        CAT_FNCT, SLS_ENABLED  },
/*  345 */  { itemToBeCoded,               NOPARAM,                     "MASKL",                                       "MASKL",                                       CAT_FNCT, SLS_UNCHANGED},
/*  346 */  { itemToBeCoded,               NOPARAM,                     "MASKR",                                       "MASKR",                                       CAT_FNCT, SLS_UNCHANGED},
/*  347 */  { itemToBeCoded,               NOPARAM,                     "MATRS",                                       "MATRS",                                       CAT_MENU, SLS_UNCHANGED},
/*  348 */  { itemToBeCoded,               NOPARAM,                     "MATR?",                                       "MATR?",                                       CAT_FNCT, SLS_UNCHANGED},
/*  349 */  { itemToBeCoded,               NOPARAM,                     "MATX",                                        "MATX",                                        CAT_MENU, SLS_UNCHANGED},
/*  350 */  { itemToBeCoded,               NOPARAM,                     "Mat_A",                                       "Mat A",                                       CAT_RVAR, SLS_UNCHANGED},
/*  351 */  { itemToBeCoded,               NOPARAM,                     "Mat_B",                                       "Mat B",                                       CAT_RVAR, SLS_UNCHANGED},
/*  352 */  { itemToBeCoded,               NOPARAM,                     "Mat_X",                                       "Mat X",                                       CAT_FNCT, SLS_UNCHANGED},
/*  353 */  { fnMax,                       NOPARAM,                     "max",                                         "max",                                         CAT_FNCT, SLS_ENABLED  },
/*  354 */  { fnConstant,                  24,                          "m" STD_SUB_e,                                 "m" STD_SUB_e,                                 CAT_CNST, SLS_ENABLED  },
/*  355 */  { fnFreeMemory,                NOPARAM,                     "MEM?",                                        "MEM?",                                        CAT_FNCT, SLS_ENABLED  },
/*  356 */  { itemToBeCoded,               NOPARAM,                     "MENU",                                        "MENU",                                        CAT_FNCT, SLS_UNCHANGED},
/*  357 */  { itemToBeCoded,               NOPARAM,                     "MENUS",                                       "MENUS",                                       CAT_MENU, SLS_UNCHANGED},
/*  358 */  { fnMin,                       NOPARAM,                     "min",                                         "min",                                         CAT_FNCT, SLS_ENABLED  },
/*  359 */  { fnMirror,                    NOPARAM,                     "MIRROR",                                      "MIRROR",                                      CAT_FNCT, SLS_ENABLED  },
/*  360 */  { fnCvtMiM,                    multiply,                    "mi." STD_RIGHT_ARROW "m",                     "mi." STD_RIGHT_ARROW "m",                     CAT_FNCT, SLS_ENABLED  },
/*  361 */  { fnConstant,                  25,                          "M" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "M" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   CAT_CNST, SLS_ENABLED  },
/*  362 */  { fnConstant,                  26,                          "m" STD_SUB_n,                                 "m" STD_SUB_n,                                 CAT_CNST, SLS_ENABLED  },
/*  363 */  { fnConstant,                  27,                          "m" STD_SUB_n "/m" STD_SUB_p,                  "m" STD_SUB_n "/m" STD_SUB_p,                  CAT_CNST, SLS_ENABLED  },
/*  364 */  { fnMod,                       NOPARAM,                     "MOD",                                         "MOD",                                         CAT_FNCT, SLS_ENABLED  },
/*  365 */  { itemToBeCoded,               NOPARAM,                     "MODE",                                        "MODE",                                        CAT_MENU, SLS_UNCHANGED},
/*  366 */  { itemToBeCoded,               NOPARAM,                     "MONTH",                                       "MONTH",                                       CAT_FNCT, SLS_UNCHANGED},
/*  367 */  { fnConstant,                  28,                          "m" STD_SUB_p,                                 "m" STD_SUB_p,                                 CAT_CNST, SLS_ENABLED  },
/*  368 */  { fnConstant,                  29,                          "m" STD_SUB_P STD_SUB_L,                       "m" STD_SUB_P STD_SUB_L,                       CAT_CNST, SLS_ENABLED  },
/*  369 */  { fnConstant,                  30,                          "m" STD_SUB_p "/m" STD_SUB_e,                  "m" STD_SUB_p "/m" STD_SUB_e,                  CAT_CNST, SLS_ENABLED  },
/*  370 */  { itemToBeCoded,               NOPARAM,                     "MSG",                                         "MSG",                                         CAT_FNCT, SLS_UNCHANGED},
/*  371 */  { fnConstant,                  31,                          "m" STD_SUB_u,                                 "m" STD_SUB_u,                                 CAT_CNST, SLS_ENABLED  },
/*  372 */  { fnConstant,                  32,                          "m" STD_SUB_u "c" STD_SUP_2,                   "m" STD_SUB_u "c" STD_SUP_2,                   CAT_CNST, SLS_ENABLED  },
/*  373 */  { fnProductSign,               PS_CROSS,                    "MULT" STD_CROSS,                              "MULT" STD_CROSS,                              CAT_FNCT, SLS_UNCHANGED},
/*  374 */  { fnProductSign,               PS_DOT,                      "MULT" STD_DOT,                                "MULT" STD_DOT,                                CAT_FNCT, SLS_UNCHANGED},
/*  375 */  { fnAngularMode,               AM_MULTPI,                   "MUL" STD_pi,                                  "MUL" STD_pi,                                  CAT_FNCT, SLS_UNCHANGED},
/*  376 */  { itemToBeCoded,               NOPARAM,                     "MVAR",                                        "MVAR",                                        CAT_FNCT, SLS_UNCHANGED},
/*  377 */  { itemToBeCoded,               NOPARAM,                     "MyMenu",                                      "MyMenu",                                      CAT_MENU, SLS_UNCHANGED},
/*  378 */  { itemToBeCoded,               NOPARAM,                     "My" STD_alpha,                                "My" STD_alpha,                                CAT_MENU, SLS_UNCHANGED},
/*  379 */  { fnConstant,                  33,                          "m" STD_SUB_mu,                                "m" STD_SUB_mu,                                CAT_CNST, SLS_ENABLED  },
/*  380 */  { itemToBeCoded,               NOPARAM,                     "M.DELR",                                      "DELR",                                        CAT_FNCT, SLS_UNCHANGED},
/*  381 */  { itemToBeCoded,               NOPARAM,                     "M.DIM",                                       "DIM",                                         CAT_FNCT, SLS_UNCHANGED},
/*  382 */  { itemToBeCoded,               NOPARAM,                     "M.DIM?",                                      "DIM?",                                        CAT_FNCT, SLS_UNCHANGED},
/*  383 */  { fnDateFormat,                DF_MDY,                      "M.DY",                                        "M.DY",                                        CAT_FNCT, SLS_UNCHANGED},
/*  384 */  { itemToBeCoded,               NOPARAM,                     "M.EDI",                                       "EDIT",                                        CAT_FNCT, SLS_UNCHANGED},
/*  385 */  { itemToBeCoded,               NOPARAM,                     "M.EDIN",                                      "EDITN",                                       CAT_FNCT, SLS_UNCHANGED},
/*  386 */  { itemToBeCoded,               NOPARAM,                     "M.EDIT",                                      "EDIT",                                        CAT_MENU, SLS_UNCHANGED},
/*  387 */  { itemToBeCoded,               NOPARAM,                     "M.GET",                                       "GETM",                                        CAT_FNCT, SLS_UNCHANGED},
/*  388 */  { itemToBeCoded,               NOPARAM,                     "M.GOTO",                                      "GOTO",                                        CAT_FNCT, SLS_UNCHANGED},
/*  389 */  { itemToBeCoded,               NOPARAM,                     "M.GROW",                                      "GROW",                                        CAT_FNCT, SLS_UNCHANGED},
/*  390 */  { itemToBeCoded,               NOPARAM,                     "M.INSR",                                      "INSR",                                        CAT_FNCT, SLS_UNCHANGED},
/*  391 */  { itemToBeCoded,               NOPARAM,                     "M.LU",                                        "M.LU",                                        CAT_FNCT, SLS_UNCHANGED},
/*  392 */  { itemToBeCoded,               NOPARAM,                     "M.NEW",                                       "NEW",                                         CAT_FNCT, SLS_UNCHANGED},
/*  393 */  { itemToBeCoded,               NOPARAM,                     "M.OLD",                                       "OLD",                                         CAT_FNCT, SLS_UNCHANGED},
/*  394 */  { itemToBeCoded,               NOPARAM,                     "M.PUT",                                       "PUTM",                                        CAT_FNCT, SLS_UNCHANGED},
/*  395 */  { itemToBeCoded,               NOPARAM,                     "M.R" STD_LEFT_RIGHT_ARROWS "R",               "R" STD_LEFT_RIGHT_ARROWS "R",                 CAT_FNCT, SLS_UNCHANGED},
/*  396 */  { itemToBeCoded,               NOPARAM,                     "M.SIMQ",                                      "M.SIMQ",                                      CAT_FNCT, SLS_UNCHANGED},
/*  397 */  { itemToBeCoded,               NOPARAM,                     "M.SQR?",                                      "M.SQR?",                                      CAT_FNCT, SLS_UNCHANGED},
/*  398 */  { itemToBeCoded,               NOPARAM,                     "M.WRAP",                                      "WRAP",                                        CAT_FNCT, SLS_UNCHANGED},
/*  399 */  { itemToBeCoded,               NOPARAM,                     "m:",                                          "m:",                                          CAT_MENU, SLS_UNCHANGED},
/*  400 */  { fnCvtAuM,                    divide,                      "m" STD_RIGHT_ARROW "au",                      "m" STD_RIGHT_ARROW "au",                      CAT_FNCT, SLS_ENABLED  },
/*  401 */  { fnCvtFtM,                    divide,                      "m" STD_RIGHT_ARROW "ft.",                     "m" STD_RIGHT_ARROW "ft.",                     CAT_FNCT, SLS_ENABLED  },
/*  402 */  { fnCvtSfeetM,                 divide,                      "m" STD_RIGHT_ARROW "ft" STD_US,               "m " STD_RIGHT_ARROW,                          CAT_FNCT, SLS_ENABLED  },
/*  403 */  { fnCvtInchM,                  divide,                      "m" STD_RIGHT_ARROW "in.",                     "m" STD_RIGHT_ARROW "in.",                     CAT_FNCT, SLS_ENABLED  },
/*  404 */  { fnCvtLyM,                    divide,                      "m" STD_RIGHT_ARROW "ly",                      "m" STD_RIGHT_ARROW "ly",                      CAT_FNCT, SLS_ENABLED  },
/*  405 */  { fnCvtMiM,                    divide,                      "m" STD_RIGHT_ARROW "mi.",                     "m" STD_RIGHT_ARROW "mi.",                     CAT_FNCT, SLS_ENABLED  },
/*  406 */  { fnCvtNmiM,                   divide,                      "m" STD_RIGHT_ARROW "nmi.",                    "m" STD_RIGHT_ARROW "nmi.",                    CAT_FNCT, SLS_ENABLED  },
/*  407 */  { fnCvtPcM,                    divide,                      "m" STD_RIGHT_ARROW "pc",                      "m" STD_RIGHT_ARROW "pc",                      CAT_FNCT, SLS_ENABLED  },
/*  408 */  { fnCvtPointM,                 divide,                      "m" STD_RIGHT_ARROW "pt.",                     "point",                                       CAT_FNCT, SLS_ENABLED  },
/*  409 */  { fnCvtYardM,                  divide,                      "m" STD_RIGHT_ARROW "yd.",                     "m" STD_RIGHT_ARROW "yd.",                     CAT_FNCT, SLS_ENABLED  },
/*  410 */  { fnConstant,                  34,                          "M" STD_SUB_SUN,                               "M" STD_SUB_SUN,                               CAT_CNST, SLS_ENABLED  },
/*  411 */  { fnConstant,                  35,                          "M" STD_SUB_EARTH,                             "M" STD_SUB_EARTH,                             CAT_CNST, SLS_ENABLED  },
/*  412 */  { fnConstant,                  36,                          "N" STD_SUB_A,                                 "N" STD_SUB_A,                                 CAT_CNST, SLS_ENABLED  },
/*  413 */  { fnConstant,                  37,                          "NaN",                                         "NaN",                                         CAT_CNST, SLS_ENABLED  },
/*  414 */  { itemToBeCoded,               NOPARAM,                     "NAND",                                        "NAND",                                        CAT_FNCT, SLS_UNCHANGED},
/*  415 */  { itemToBeCoded,               NOPARAM,                     "NaN?",                                        "NaN?",                                        CAT_FNCT, SLS_UNCHANGED},
/*  416 */  { itemToBeCoded,               NOPARAM,                     "NBin",                                        "NBin",                                        CAT_FNCT, SLS_UNCHANGED},
/*  417 */  { itemToBeCoded,               NOPARAM,                     "NBin" STD_SUB_p,                              "NBin" STD_SUB_p,                              CAT_FNCT, SLS_UNCHANGED},
/*  418 */  { itemToBeCoded,               NOPARAM,                     "NBin" STD_SUB_e,                              "NBin" STD_SUB_e,                              CAT_FNCT, SLS_UNCHANGED},
/*  419 */  { itemToBeCoded,               NOPARAM,                     "NBin" STD_SUP_MINUS_1,                        "NBin" STD_SUP_MINUS_1,                        CAT_FNCT, SLS_UNCHANGED},
/*  420 */  { itemToBeCoded,               NOPARAM,                     "NBin:",                                       "NBin:",                                       CAT_MENU, SLS_UNCHANGED},
/*  421 */  { fnNeighb,                    NOPARAM,                     "NEIGHB",                                      "NEIGHB",                                      CAT_FNCT, SLS_ENABLED  },
/*  422 */  { itemToBeCoded,               NOPARAM,                     "NEXTP",                                       "NEXTP",                                       CAT_FNCT, SLS_UNCHANGED},
/*  423 */  { fnCvtNmiM,                   multiply,                    "nmi." STD_RIGHT_ARROW "m",                    "nmi." STD_RIGHT_ARROW "m",                    CAT_FNCT, SLS_ENABLED  },
/*  424 */  { fnNop,                       NOPARAM,                     "NOP",                                         "NOP",                                         CAT_FNCT, SLS_UNCHANGED},
/*  425 */  { itemToBeCoded,               NOPARAM,                     "NOR",                                         "NOR",                                         CAT_FNCT, SLS_UNCHANGED},
/*  426 */  { itemToBeCoded,               NOPARAM,                     "Norml",                                       "Norml",                                       CAT_FNCT, SLS_UNCHANGED},
/*  427 */  { itemToBeCoded,               NOPARAM,                     "Norml" STD_SUB_p,                             "Norml" STD_SUB_p,                             CAT_FNCT, SLS_UNCHANGED},
/*  428 */  { itemToBeCoded,               NOPARAM,                     "Norml" STD_SUB_e,                             "Norml" STD_SUB_e,                             CAT_FNCT, SLS_UNCHANGED},
/*  429 */  { itemToBeCoded,               NOPARAM,                     "Norml" STD_SUP_MINUS_1,                       "Norml" STD_SUP_MINUS_1,                       CAT_FNCT, SLS_UNCHANGED},
/*  430 */  { itemToBeCoded,               NOPARAM,                     "Norml:",                                      "Norml:",                                      CAT_MENU, SLS_UNCHANGED},
/*  431 */  { fnLogicalNot,                NOPARAM,                     "NOT",                                         "NOT",                                         CAT_FNCT, SLS_ENABLED  },
/*  432 */  { itemToBeCoded,               NOPARAM,                     "NPER",                                        "n" STD_SUB_P STD_SUB_E STD_SUB_R,             CAT_RVAR, SLS_UNCHANGED},
/*  433 */  { fnStatSum,                   0,                           "n" STD_SIGMA,                                 "n",                                           CAT_FNCT, SLS_ENABLED  },
/*  434 */  { fnCvtLbfN,                   divide,                      "N" STD_RIGHT_ARROW "lbf",                     "N" STD_RIGHT_ARROW "lbf",                     CAT_FNCT, SLS_ENABLED  },
/*  435 */  { itemToBeCoded,               NOPARAM,                     "ODD?",                                        "ODD?",                                        CAT_FNCT, SLS_UNCHANGED},
/*  436 */  { fnOff,                       NOPARAM,                     "OFF",                                         "OFF",                                         CAT_FNCT, SLS_UNCHANGED},
/*  437 */  { itemToBeCoded,               NOPARAM,                     "OR",                                          "OR",                                          CAT_FNCT, SLS_UNCHANGED},
/*  438 */  { itemToBeCoded,               NOPARAM,                     "OrthoF",                                      "OrthoF",                                      CAT_FNCT, SLS_UNCHANGED},
/*  439 */  { itemToBeCoded,               NOPARAM,                     "ORTHOG",                                      "Orthog",                                      CAT_FNCT, SLS_UNCHANGED},
/*  440 */  { fnCvtOzKg,                   multiply,                    "oz" STD_RIGHT_ARROW "kg",                     "oz" STD_RIGHT_ARROW "kg",                     CAT_FNCT, SLS_ENABLED  },
/*  441 */  { fnConstant,                  38,                          "p" STD_SUB_0,                                 "p" STD_SUB_0,                                 CAT_CNST, SLS_ENABLED  },
/*  442 */  { itemToBeCoded,               NOPARAM,                     "PAUSE",                                       "PAUSE",                                       CAT_FNCT, SLS_UNCHANGED},
/*  443 */  { fnCvtAtmPa,                  divide,                      "Pa" STD_RIGHT_ARROW "atm",                    "Pa" STD_RIGHT_ARROW "atm",                    CAT_FNCT, SLS_ENABLED  },
/*  444 */  { fnCvtBarPa,                  divide,                      "Pa" STD_RIGHT_ARROW "bar",                    "Pa" STD_RIGHT_ARROW "bar",                    CAT_FNCT, SLS_ENABLED  },
/*  445 */  { fnCvtInhgPa,                 divide,                      "Pa" STD_RIGHT_ARROW "iHg",                    "Pa" STD_RIGHT_ARROW,                          CAT_FNCT, SLS_ENABLED  },
/*  446 */  { fnCvtPsiPa,                  divide,                      "Pa" STD_RIGHT_ARROW "psi",                    "Pa" STD_RIGHT_ARROW "psi",                    CAT_FNCT, SLS_ENABLED  },
/*  447 */  { fnCvtTorrPa,                 divide,                      "Pa" STD_RIGHT_ARROW "tor",                    "Pa " STD_RIGHT_ARROW,                         CAT_FNCT, SLS_ENABLED  },
/*  448 */  { itemToBeCoded,               NOPARAM,                     "PARTS",                                       "PARTS",                                       CAT_MENU, SLS_UNCHANGED},
/*  449 */  { fnCvtPcM,                    multiply,                    "pc" STD_RIGHT_ARROW "m",                      "pc" STD_RIGHT_ARROW "m",                      CAT_FNCT, SLS_ENABLED  },
/*  450 */  { itemToBeCoded,               NOPARAM,                     "PERM",                                        "P" STD_SUB_y STD_SUB_x,                       CAT_FNCT, SLS_UNCHANGED},
/*  451 */  { itemToBeCoded,               NOPARAM,                     "PER/a",                                       "per/a",                                       CAT_RVAR, SLS_UNCHANGED},
/*  452 */  { itemToBeCoded,               NOPARAM,                     "PGMINT",                                      "PGMINT",                                      CAT_FNCT, SLS_UNCHANGED},
/*  453 */  { itemToBeCoded,               NOPARAM,                     "PGMSLV",                                      "PGMSLV",                                      CAT_FNCT, SLS_UNCHANGED},
/*  454 */  { itemToBeCoded,               NOPARAM,                     "PIXEL",                                       "PIXEL",                                       CAT_FNCT, SLS_UNCHANGED},
/*  455 */  { itemToBeCoded,               NOPARAM,                     "PLOT",                                        "PLOT",                                        CAT_FNCT, SLS_UNCHANGED},
/*  456 */  { itemToBeCoded,               NOPARAM,                     "PMT",                                         "PMT",                                         CAT_RVAR, SLS_UNCHANGED},
/*  457 */  { itemToBeCoded,               NOPARAM,                     "P" STD_SUB_n,                                 "P" STD_SUB_n,                                 CAT_FNCT, SLS_UNCHANGED},
/*  458 */  { itemToBeCoded,               NOPARAM,                     "POINT",                                       "POINT",                                       CAT_FNCT, SLS_UNCHANGED},
/*  459 */  { itemToBeCoded,               NOPARAM,                     "Poiss",                                       "Poiss",                                       CAT_FNCT, SLS_UNCHANGED},
/*  460 */  { itemToBeCoded,               NOPARAM,                     "Poiss" STD_SUB_p,                             "Poiss" STD_SUB_p,                             CAT_FNCT, SLS_UNCHANGED},
/*  461 */  { itemToBeCoded,               NOPARAM,                     "Poiss" STD_SUB_e,                             "Poiss" STD_SUB_e,                             CAT_FNCT, SLS_UNCHANGED},
/*  462 */  { itemToBeCoded,               NOPARAM,                     "Poiss" STD_SUP_MINUS_1,                       "Poiss" STD_SUP_MINUS_1,                       CAT_FNCT, SLS_UNCHANGED},
/*  463 */  { itemToBeCoded,               NOPARAM,                     "Poiss:",                                      "Poiss:",                                      CAT_MENU, SLS_UNCHANGED},
/*  464 */  { fnComplexMode,               CM_POLAR,                    "POLAR",                                       "POLAR",                                       CAT_FNCT, SLS_UNCHANGED},
/*  465 */  { itemToBeCoded,               NOPARAM,                     "PopLR",                                       "PopLR",                                       CAT_FNCT, SLS_UNCHANGED},
/*  466 */  { fnCurveFitting,              CF_POWER_FITTING,            "PowerF",                                      "PowerF",                                      CAT_FNCT, SLS_UNCHANGED},
/*  467 */  { fnCvtRatioDb,                10,                          "pr" STD_RIGHT_ARROW "dB",                     "power",                                       CAT_FNCT, SLS_ENABLED  },
/*  468 */  { itemToBeCoded,               NOPARAM,                     "PRCL",                                        "PRCL",                                        CAT_FNCT, SLS_UNCHANGED},
/*  469 */  { fnIsPrime,                   NOPARAM,                     "PRIME?",                                      "PRIME?",                                      CAT_FNCT, SLS_UNCHANGED},
/*  470 */  { itemToBeCoded,               NOPARAM,                     "PROB",                                        "PROB",                                        CAT_MENU, SLS_UNCHANGED},
/*  471 */  { fnFractionType,              FT_PROPER,                   "PROFRC",                                      "a" STD_SPACE_3_PER_EM "b/c",                  CAT_FNCT, SLS_UNCHANGED},
/*  472 */  { itemToBeCoded,               NOPARAM,                     "PROGS",                                       "PROGS",                                       CAT_MENU, SLS_UNCHANGED},
/*  473 */  { fnCvtPsiPa,                  multiply,                    "psi" STD_RIGHT_ARROW "Pa",                    "psi" STD_RIGHT_ARROW "Pa",                    CAT_FNCT, SLS_ENABLED  },
/*  474 */  { itemToBeCoded,               NOPARAM,                     "PSTO",                                        "PSTO",                                        CAT_FNCT, SLS_UNCHANGED},
/*  475 */  { fnCvtPointM,                 multiply,                    "pt." STD_RIGHT_ARROW "m",                     "point",                                       CAT_FNCT, SLS_ENABLED  },
/*  476 */  { itemToBeCoded,               NOPARAM,                     "PUTK",                                        "PUTK",                                        CAT_FNCT, SLS_UNCHANGED},
/*  477 */  { itemToBeCoded,               NOPARAM,                     "PV",                                          "PV",                                          CAT_RVAR, SLS_UNCHANGED},
/*  478 */  { itemToBeCoded,               NOPARAM,                     "P.FN",                                        "P.FN",                                        CAT_MENU, SLS_UNCHANGED},
/*  479 */  { itemToBeCoded,               NOPARAM,                     "P.FN2",                                       "P.FN2",                                       CAT_MENU, SLS_UNCHANGED},
/*  480 */  { itemToBeCoded,               NOPARAM,                     "P:",                                          "P:",                                          CAT_MENU, SLS_UNCHANGED},
/*  481 */  { itemToBeCoded,               NOPARAM,                     "QUIET",                                       "QUIET",                                       CAT_FNCT, SLS_UNCHANGED},
/*  482 */  { fnConstant,                  39,                          "R",                                           "R",                                           CAT_CNST, SLS_ENABLED  },
/*  483 */  { fnAngularMode,               AM_RADIAN,                   "RAD",                                         "RAD",                                         CAT_FNCT, SLS_UNCHANGED},
/*  484 */  { fnCvtToCurrentAngularMode,   AM_RADIAN,                   "RAD" STD_RIGHT_ARROW,                         "RAD" STD_RIGHT_ARROW,                         CAT_FNCT, SLS_ENABLED  },
/*  485 */  { itemToBeCoded,               NOPARAM,                     "RAM",                                         "RAM",                                         CAT_MENU, SLS_UNCHANGED},
/*  486 */  { fnRandom,                    NOPARAM,                     "RAN#",                                        "RAN#",                                        CAT_FNCT, SLS_ENABLED  },
/*  487 */  { registerBrowser,             NOPARAM,                     "REGS.V",                                      "REGS.V",                                      CAT_FNCT, SLS_UNCHANGED},      //JM Changed RBR to REGS
/*  488 */  { fnRecall,                    TM_STORCL,                   "RCL",                                         "RCL",                                         CAT_FNCT, SLS_ENABLED  },
/*  489 */  { fnRecallConfig,              NOPARAM,                     "RCLCFG",                                      "Config",                                      CAT_FNCT, SLS_UNCHANGED},
/*  490 */  { fnRecallElement,             NOPARAM,                     "RCLEL",                                       "RCLEL",                                       CAT_FNCT, SLS_ENABLED  },
/*  491 */  { fnRecallIJ,                  NOPARAM,                     "RCLIJ",                                       "RCLIJ",                                       CAT_FNCT, SLS_ENABLED  },
/*  492 */  { fnRecallStack,               NOPARAM,                     "RCLS",                                        "Stack",                                       CAT_FNCT, SLS_ENABLED  },
/*  493 */  { fnRecallAdd,                 NOPARAM,                     "RCL+",                                        "RCL+",                                        CAT_FNCT, SLS_ENABLED  },
/*  494 */  { fnRecallSub,                 NOPARAM,                     "RCL-",                                        "RCL-",                                        CAT_FNCT, SLS_ENABLED  },
/*  495 */  { fnRecallMult,                NOPARAM,                     "RCL" STD_CROSS,                               "RCL" STD_CROSS,                               CAT_FNCT, SLS_ENABLED  },
/*  496 */  { fnRecallDiv,                 NOPARAM,                     "RCL/",                                        "RCL/",                                        CAT_FNCT, SLS_ENABLED  },
/*  497 */  { fnRecallMax,                 NOPARAM,                     "RCL" STD_UP_ARROW,                            "Max",                                         CAT_FNCT, SLS_ENABLED  },
/*  498 */  { fnRecallMin,                 NOPARAM,                     "RCL" STD_DOWN_ARROW,                          "Min",                                         CAT_FNCT, SLS_ENABLED  },
/*  499 */  { itemToBeCoded,               NOPARAM,                     "RDP",                                         "RDP",                                         CAT_FNCT, SLS_UNCHANGED},
/*  500 */  { fnRadixMark,                 RM_COMMA,                    "RDX,",                                        "RDX,",                                        CAT_FNCT, SLS_UNCHANGED},
/*  501 */  { fnRadixMark,                 RM_PERIOD,                   "RDX.",                                        "RDX.",                                        CAT_FNCT, SLS_UNCHANGED},
/*  502 */  { fnConstant,                  40,                          "r" STD_SUB_e,                                 "r" STD_SUB_e,                                 CAT_CNST, SLS_ENABLED  },
/*  503 */  { fnRealPart,                  NOPARAM,                     "Re",                                          "Re",                                          CAT_FNCT, SLS_ENABLED  },
/*  504 */  { fnComplexResult,             false,                       "REALRE",                                      "REALRE",                                      CAT_FNCT, SLS_UNCHANGED},
/*  505 */  { itemToBeCoded,               NOPARAM,                     "REALS",                                       "REALS",                                       CAT_MENU, SLS_UNCHANGED},
/*  506 */  { itemToBeCoded,               NOPARAM,                     "REAL?",                                       "REAL?",                                       CAT_FNCT, SLS_UNCHANGED},
/*  507 */  { fnComplexMode,               CM_RECTANGULAR,              "RECT",                                        "RECT",                                        CAT_FNCT, SLS_UNCHANGED},
/*  508 */  { itemToBeCoded,               NOPARAM,                     "REGS",                                        "REGS",                                        CAT_RVAR, SLS_UNCHANGED},
/*  509 */  { itemToBeCoded,               NOPARAM,                     "RECV",                                        "RECV",                                        CAT_FNCT, SLS_UNCHANGED},
/*  510 */  { fnReset,                     NOT_CONFIRMED,               "RESET",                                       "RESET",                                       CAT_FNCT, SLS_UNCHANGED},
/*  511 */  { fnReToCx,                    NOPARAM,                     "RE" STD_RIGHT_ARROW "CX",                     "RE" STD_RIGHT_ARROW "CX",                     CAT_FNCT, SLS_ENABLED  },
/*  512 */  { fnSwapRealImaginary,         NOPARAM,                     "Re" STD_LEFT_RIGHT_ARROWS "Im",               "Re" STD_LEFT_RIGHT_ARROWS "Im",               CAT_FNCT, SLS_ENABLED  },
/*  513 */  { itemToBeCoded,               NOPARAM,                     "RJ",                                          "RJ",                                          CAT_FNCT, SLS_UNCHANGED},
/*  514 */  { fnConstant,                  41,                          "R" STD_SUB_K,                                 "R" STD_SUB_K,                                 CAT_CNST, SLS_ENABLED  },
/*  515 */  { itemToBeCoded,               NOPARAM,                     "RL",                                          "RL",                                          CAT_FNCT, SLS_UNCHANGED},
/*  516 */  { itemToBeCoded,               NOPARAM,                     "RLC",                                         "RLC",                                         CAT_FNCT, SLS_UNCHANGED},
/*  517 */  { fnConstant,                  42,                          "R" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "R" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   CAT_CNST, SLS_ENABLED  },
/*  518 */  { itemToBeCoded,               NOPARAM,                     "RM",                                          "RM",                                          CAT_FNCT, SLS_UNCHANGED},
/*  519 */  { fnGetRoundingMode,           NOPARAM,                     "RM?",                                         "RM?",                                         CAT_FNCT, SLS_ENABLED  },
/*  520 */  { fnRmd,                       NOPARAM,                     "RMD",                                         "RMD",                                         CAT_FNCT, SLS_ENABLED  },
/*  521 */  { itemToBeCoded,               NOPARAM,                     "RNORM",                                       "RNORM",                                       CAT_FNCT, SLS_UNCHANGED},
/*  522 */  { fnRound,                     NOPARAM,                     "ROUND",                                       "ROUND",                                       CAT_FNCT, SLS_ENABLED  },
/*  523 */  { fnRoundi,                    NOPARAM,                     "ROUNDI",                                      "ROUNDI",                                      CAT_FNCT, SLS_ENABLED  },
/*  524 */  { itemToBeCoded,               NOPARAM,                     "RR",                                          "RR",                                          CAT_FNCT, SLS_UNCHANGED},
/*  525 */  { itemToBeCoded,               NOPARAM,                     "RRC",                                         "RRC",                                         CAT_FNCT, SLS_UNCHANGED},
/*  526 */  { itemToBeCoded,               NOPARAM,                     "RSD",                                         "RSD",                                         CAT_FNCT, SLS_UNCHANGED},
/*  527 */  { itemToBeCoded,               NOPARAM,                     "RSUM",                                        "RSUM",                                        CAT_FNCT, SLS_UNCHANGED},
/*  528 */  { itemToBeCoded,               NOPARAM,                     "RTN",                                         "RTN",                                         CAT_FNCT, SLS_UNCHANGED},
/*  529 */  { itemToBeCoded,               NOPARAM,                     "RTN+1",                                       "RTN+1",                                       CAT_FNCT, SLS_UNCHANGED},
/*  530 */  { itemToBeCoded,               NOPARAM,                     "R-CLR",                                       "R-CLR",                                       CAT_FNCT, SLS_UNCHANGED},
/*  531 */  { itemToBeCoded,               NOPARAM,                     "R-COPY",                                      "R-COPY",                                      CAT_FNCT, SLS_UNCHANGED},
/*  532 */  { itemToBeCoded,               NOPARAM,                     "R-SORT",                                      "R-SORT",                                      CAT_FNCT, SLS_UNCHANGED},
/*  533 */  { itemToBeCoded,               NOPARAM,                     "R-SWAP",                                      "R-SWAP",                                      CAT_FNCT, SLS_UNCHANGED},
/*  534 */  { fnCvtRadToDeg,               NOPARAM,                     "R" STD_RIGHT_ARROW "D",                       "R" STD_RIGHT_ARROW "D",                       CAT_FNCT, SLS_ENABLED  },
/*  535 */  { fnRollUp,                    NOPARAM,                     "R" STD_UP_ARROW,                              "R" STD_UP_ARROW,                              CAT_FNCT, SLS_UNCHANGED},
/*  536 */  { fnRollDown,                  NOPARAM,                     "R" STD_DOWN_ARROW,                            "R" STD_DOWN_ARROW,                            CAT_FNCT, SLS_UNCHANGED},
/*  537 */  { fnConstant,                  43,                          "R" STD_SUB_INFINITY,                          "R" STD_SUB_INFINITY,                          CAT_CNST, SLS_ENABLED  },
/*  538 */  { fnConstant,                  44,                          "R" STD_SUB_SUN,                               "R" STD_SUB_SUN,                               CAT_CNST, SLS_ENABLED  },
/*  539 */  { fnConstant,                  45,                          "R" STD_SUB_EARTH,                             "R" STD_SUB_EARTH,                             CAT_CNST, SLS_ENABLED  },
/*  540 */  { itemToBeCoded,               NOPARAM,                     "s",                                           "s",                                           CAT_FNCT, SLS_UNCHANGED},
/*  541 */  { fnConstant,                  46,                          "Sa",                                          "Sa",                                          CAT_CNST, SLS_ENABLED  },
/*  542 */  { itemToBeCoded,               NOPARAM,                     "SAVE",                                        "SAVE",                                        CAT_FNCT, SLS_UNCHANGED},
/*  543 */  { itemToBeCoded,               NOPARAM,                     "SB",                                          "SB",                                          CAT_FNCT, SLS_UNCHANGED},
/*  544 */  { fnConstant,                  47,                          "Sb",                                          "Sb",                                          CAT_CNST, SLS_ENABLED  },
/*  545 */  { fnDisplayFormatSci,          TM_VALUE,                    "SCI",                                         "SCI",                                         CAT_FNCT, SLS_UNCHANGED},
/*  546 */  { fnCvtShortcwtKg,             multiply,                    "scw" STD_RIGHT_ARROW "kg",                    "short",                                       CAT_FNCT, SLS_ENABLED  },
/*  547 */  { fnDisplayOvr,                DO_SCI,                      "SCIOVR",                                      "SCIOVR",                                      CAT_FNCT, SLS_UNCHANGED},
/*  548 */  { fnGetSignificantDigits,      NOPARAM,                     "SDIGS?",                                      "SDIGS?",                                      CAT_FNCT, SLS_ENABLED  },
/*  549 */  { itemToBeCoded,               NOPARAM,                     "SDL",                                         "SDL",                                         CAT_FNCT, SLS_UNCHANGED},
/*  550 */  { itemToBeCoded,               NOPARAM,                     "SDR",                                         "SDR",                                         CAT_FNCT, SLS_UNCHANGED},
/*  551 */  { fnConstant,                  48,                          "Se" STD_SUP_2,                                "Se" STD_SUP_2,                                CAT_CNST, SLS_ENABLED  },
/*  552 */  { fnSeed,                      NOPARAM,                     "SEED",                                        "SEED",                                        CAT_FNCT, SLS_ENABLED  },
/*  553 */  { itemToBeCoded,               NOPARAM,                     "SEND",                                        "SEND",                                        CAT_FNCT, SLS_UNCHANGED},
/*  554 */  { fnConfigChina,               NOPARAM,                     "SETCHN",                                      "CHINA",                                       CAT_FNCT, SLS_UNCHANGED},
/*  555 */  { itemToBeCoded,               NOPARAM,                     "SETDAT",                                      "SETDAT",                                      CAT_FNCT, SLS_UNCHANGED},
/*  556 */  { fnConfigEurope,              NOPARAM,                     "SETEUR",                                      "EUROPE",                                      CAT_FNCT, SLS_UNCHANGED},
/*  557 */  { fnConfigIndia,               NOPARAM,                     "SETIND",                                      "INDIA",                                       CAT_FNCT, SLS_UNCHANGED},
/*  558 */  { fnConfigJapan,               NOPARAM,                     "SETJPN",                                      "JAPAN",                                       CAT_FNCT, SLS_UNCHANGED},
/*  559 */  { itemToBeCoded,               NOPARAM,                     "SETSIG",                                      "SETSIG",                                      CAT_FNCT, SLS_UNCHANGED},
/*  560 */  { itemToBeCoded,               NOPARAM,                     "SETTIM",                                      "SETTIM",                                      CAT_FNCT, SLS_UNCHANGED},
/*  561 */  { fnConfigUk,                  NOPARAM,                     "SETUK",                                       "UK",                                          CAT_FNCT, SLS_UNCHANGED},
/*  562 */  { fnConfigUsa,                 NOPARAM,                     "SETUSA",                                      "USA",                                         CAT_FNCT, SLS_UNCHANGED},
/*  563 */  { fnConstant,                  49,                          "Se'" STD_SUP_2,                               "Se'" STD_SUP_2,                               CAT_CNST, SLS_ENABLED  },
/*  564 */  { fnSetFlag,                   TM_FLAG,                     "SF",                                          "SF",                                          CAT_FNCT, SLS_UNCHANGED},
/*  565 */  { fnConstant,                  50,                          "Sf" STD_SUP_MINUS_1,                          "Sf" STD_SUP_MINUS_1,                          CAT_CNST, SLS_ENABLED  },
/*  566 */  { fnSign,                      NOPARAM,                     "SIGN",                                        "sign",                                        CAT_FNCT, SLS_ENABLED  },
/*  567 */  { fnIntegerMode,               SIM_SIGNMT,                  "SIGNMT",                                      "SIGNMT",                                      CAT_FNCT, SLS_UNCHANGED},
/*  568 */  { itemToBeCoded,               NOPARAM,                     "SIM_EQ",                                      "SIM EQ",                                      CAT_FNCT, SLS_UNCHANGED},
/*  569 */  { fnSin,                       NOPARAM,                     "SIN",                                         "SIN",                                         CAT_FNCT, SLS_ENABLED  },      //JM3
/*  570 */  { itemToBeCoded,               NOPARAM,                     "sinc",                                        "sinc",                                        CAT_FNCT, SLS_ENABLED  },
/*  571 */  { fnSinh,                      NOPARAM,                     "sinh",                                        "sinh",                                        CAT_FNCT, SLS_ENABLED  },
/*  572 */  { itemToBeCoded,               NOPARAM,                     "SKIP",                                        "SKIP",                                        CAT_FNCT, SLS_UNCHANGED},
/*  573 */  { itemToBeCoded,               NOPARAM,                     "SL",                                          "SL",                                          CAT_FNCT, SLS_UNCHANGED},
/*  574 */  { itemToBeCoded,               NOPARAM,                     "SLOW",                                        "SLOW",                                        CAT_FNCT, SLS_UNCHANGED},
/*  575 */  { fnSlvq,                      NOPARAM,                     "SLVQ",                                        "SLVQ",                                        CAT_FNCT, SLS_ENABLED  },
/*  576 */  { itemToBeCoded,               NOPARAM,                     "s" STD_SUB_m,                                 "s" STD_SUB_m,                                 CAT_FNCT, SLS_UNCHANGED},
/*  577 */  { fnGetIntegerSignMode,        NOPARAM,                     "SMODE?",                                      "SMODE?",                                      CAT_FNCT, SLS_ENABLED  },
/*  578 */  { itemToBeCoded,               NOPARAM,                     "s" STD_SUB_m STD_SUB_w,                       "s" STD_SUB_m STD_SUB_w,                       CAT_FNCT, SLS_UNCHANGED},
/*  579 */  { itemToBeCoded,               NOPARAM,                     "SOLVE",                                       "SOLVE",                                       CAT_FNCT, SLS_UNCHANGED},
/*  580 */  { itemToBeCoded,               NOPARAM,                     "Solver",                                      "Solver",                                      CAT_MENU, SLS_UNCHANGED},
/*  581 */  { itemToBeCoded,               NOPARAM,                     "SPEC?",                                       "SPEC?",                                       CAT_FNCT, SLS_UNCHANGED},
/*  582 */  { itemToBeCoded,               NOPARAM,                     "SR",                                          "SR",                                          CAT_FNCT, SLS_UNCHANGED},
/*  583 */  { fnStackSize,                 SS_4,                        "SSIZE4",                                      "SSIZE4",                                      CAT_FNCT, SLS_UNCHANGED},
/*  584 */  { fnStackSize,                 SS_8,                        "SSIZE8",                                      "SSIZE8",                                      CAT_FNCT, SLS_UNCHANGED},
/*  585 */  { fnGetStackSize,              NOPARAM,                     "SSIZE?",                                      "SSIZE?",                                      CAT_FNCT, SLS_ENABLED  },
/*  586 */  { itemToBeCoded,               NOPARAM,                     "STAT",                                        "STAT",                                        CAT_MENU, SLS_UNCHANGED},
/*  587 */  { flagBrowser,                 NOPARAM,                     "FLAGS.V",                                     "FLAGS.V",                                     CAT_FNCT, SLS_UNCHANGED},      //JM Changed STATUS
/*  588 */  { itemToBeCoded,               NOPARAM,                     "STK",                                         "STK",                                         CAT_MENU, SLS_UNCHANGED},
/*  589 */  { fnStore,                     TM_STORCL,                   "STO",                                         "STO",                                         CAT_FNCT, SLS_UNCHANGED},
/*  590 */  { fnStoreConfig,               NOPARAM,                     "STOCFG",                                      "Config",                                      CAT_FNCT, SLS_UNCHANGED},
/*  591 */  { fnStoreElement,              NOPARAM,                     "STOEL",                                       "STOEL",                                       CAT_FNCT, SLS_UNCHANGED},
/*  592 */  { fnStoreIJ,                   NOPARAM,                     "STOIJ",                                       "STOIJ",                                       CAT_FNCT, SLS_UNCHANGED},
/*  593 */  { itemToBeCoded,               NOPARAM,                     "STOP",                                        "R/S",                                         CAT_FNCT, SLS_UNCHANGED},
/*  594 */  { fnStoreStack,                NOPARAM,                     "STOS",                                        "Stack",                                       CAT_FNCT, SLS_UNCHANGED},
/*  595 */  { fnStoreAdd,                  NOPARAM,                     "STO+",                                        "STO+",                                        CAT_FNCT, SLS_UNCHANGED},
/*  596 */  { fnStoreSub,                  NOPARAM,                     "STO-",                                        "STO-",                                        CAT_FNCT, SLS_UNCHANGED},
/*  597 */  { fnStoreMult,                 NOPARAM,                     "STO" STD_CROSS,                               "STO" STD_CROSS,                               CAT_FNCT, SLS_UNCHANGED},
/*  598 */  { fnStoreDiv,                  NOPARAM,                     "STO/",                                        "STO/",                                        CAT_FNCT, SLS_UNCHANGED},
/*  599 */  { fnStoreMax,                  NOPARAM,                     "STO" STD_UP_ARROW,                            "Max",                                         CAT_FNCT, SLS_UNCHANGED},
/*  600 */  { fnStoreMin,                  NOPARAM,                     "STO" STD_DOWN_ARROW,                          "Min",                                         CAT_FNCT, SLS_UNCHANGED},
/*  601 */  { fnCvtStoneKg,                multiply,                    "sto" STD_RIGHT_ARROW "kg",                    "stone",                                       CAT_FNCT, SLS_ENABLED  },
/*  602 */  { itemToBeCoded,               NOPARAM,                     "STRI?",                                       "STRI?",                                       CAT_FNCT, SLS_UNCHANGED},
/*  603 */  { itemToBeCoded,               NOPARAM,                     "STRING",                                      "STRING",                                      CAT_MENU, SLS_UNCHANGED},
/*  604 */  { addItemToBuffer,             REGISTER_A,                  "ST.A",                                        "A",                                           CAT_RVAR, SLS_UNCHANGED},
/*  605 */  { addItemToBuffer,             REGISTER_B,                  "ST.B",                                        "B",                                           CAT_RVAR, SLS_UNCHANGED},
/*  606 */  { addItemToBuffer,             REGISTER_C,                  "ST.C",                                        "C",                                           CAT_RVAR, SLS_UNCHANGED},
/*  607 */  { addItemToBuffer,             REGISTER_D,                  "ST.D",                                        "D",                                           CAT_RVAR, SLS_UNCHANGED},
/*  608 */  { addItemToBuffer,             REGISTER_T,                  "ST.T",                                        "ST.T",                                        CAT_RVAR, SLS_UNCHANGED},
/*  609 */  { addItemToBuffer,             REGISTER_X,                  "ST.X",                                        "ST.X",                                        CAT_RVAR, SLS_UNCHANGED},
/*  610 */  { addItemToBuffer,             REGISTER_Y,                  "ST.Y",                                        "ST.Y",                                        CAT_RVAR, SLS_UNCHANGED},
/*  611 */  { addItemToBuffer,             REGISTER_Z,                  "ST.Z",                                        "ST.Z",                                        CAT_RVAR, SLS_UNCHANGED},
/*  612 */  { itemToBeCoded,               NOPARAM,                     "SUM",                                         "SUM",                                         CAT_FNCT, SLS_UNCHANGED},
/*  613 */  { itemToBeCoded,               NOPARAM,                     "s" STD_SUB_w,                                 "s" STD_SUB_w,                                 CAT_FNCT, SLS_UNCHANGED},
/*  614 */  { itemToBeCoded,               NOPARAM,                     "s" STD_SUB_x STD_SUB_y,                       "s" STD_SUB_x STD_SUB_y,                       CAT_FNCT, SLS_UNCHANGED},
/*  615 */  { fnCvtShorttonKg,             multiply,                    "s.t" STD_RIGHT_ARROW "kg",                    "short",                                       CAT_FNCT, SLS_ENABLED  },
/*  616 */  { fnCvtYearS,                  divide,                      "s" STD_RIGHT_ARROW "year",                    "s" STD_RIGHT_ARROW "year",                    CAT_FNCT, SLS_ENABLED  },
/*  617 */  { fnConstant,                  51,                          "T" STD_SUB_0,                                 "T" STD_SUB_0,                                 CAT_CNST, SLS_ENABLED  },
/*  618 */  { fnTan,                       NOPARAM,                     "TAN",                                         "TAN",                                         CAT_FNCT, SLS_ENABLED  },      //JM3
/*  619 */  { fnTanh,                      NOPARAM,                     "tanh",                                        "tanh",                                        CAT_FNCT, SLS_ENABLED  },
/*  620 */  { itemToBeCoded,               NOPARAM,                     "TDISP",                                       "TDISP",                                       CAT_FNCT, SLS_UNCHANGED},
/*  621 */  { itemToBeCoded,               NOPARAM,                     "TEST",                                        "TEST",                                        CAT_MENU, SLS_UNCHANGED},
/*  622 */  { itemToBeCoded,               NOPARAM,                     "TICKS",                                       "TICKS",                                       CAT_FNCT, SLS_UNCHANGED},
/*  623 */  { itemToBeCoded,               NOPARAM,                     "TIME",                                        "TIME",                                        CAT_FNCT, SLS_UNCHANGED},
/*  624 */  { itemToBeCoded,               NOPARAM,                     "TIMER",                                       "TIMER",                                       CAT_FNCT, SLS_UNCHANGED},
/*  625 */  { itemToBeCoded,               NOPARAM,                     "TIMES",                                       "TIMES",                                       CAT_MENU, SLS_UNCHANGED},
/*  626 */  { itemToBeCoded,               NOPARAM,                     "T" STD_SUB_n,                                 "T" STD_SUB_n,                                 CAT_FNCT, SLS_UNCHANGED},
/*  627 */  { itemToBeCoded,               NOPARAM,                     "TONE",                                        "TONE",                                        CAT_FNCT, SLS_UNCHANGED},
/*  628 */  { fnCvtTonKg,                  multiply,                    "ton" STD_RIGHT_ARROW "kg",                    "ton" STD_RIGHT_ARROW "kg",                    CAT_FNCT, SLS_ENABLED  },
/*  629 */  { itemToBeCoded,               NOPARAM,                     "TOP?",                                        "TOP?",                                        CAT_FNCT, SLS_UNCHANGED},
/*  630 */  { fnCvtTorrPa,                 multiply,                    "tor" STD_RIGHT_ARROW "Pa",                    "torr",                                        CAT_FNCT, SLS_ENABLED  },
/*  631 */  { fnConstant,                  52,                          "T" STD_SUB_p,                                 "T" STD_SUB_P,                                 CAT_CNST, SLS_ENABLED  },
/*  632 */  { fnConstant,                  53,                          "t" STD_SUB_P STD_SUB_L,                       "t" STD_SUB_P STD_SUB_L,                       CAT_CNST, SLS_ENABLED  },
/*  633 */  { itemToBeCoded,               NOPARAM,                     "t" STD_SUB_p "(x)",                           "t" STD_SUB_p "(x)",                           CAT_FNCT, SLS_UNCHANGED},
/*  634 */  { itemToBeCoded,               NOPARAM,                     "t" STD_SUB_e "(x)",                           "t" STD_SUB_e "(x)",                           CAT_FNCT, SLS_UNCHANGED},
/*  635 */  { itemToBeCoded,               NOPARAM,                     "t(x)",                                        "t(x)",                                        CAT_FNCT, SLS_UNCHANGED},
/*  636 */  { itemToBeCoded,               NOPARAM,                     "t" STD_SUP_MINUS_1 "(p)",                     "t" STD_SUP_MINUS_1 "(p)",                     CAT_FNCT, SLS_UNCHANGED},
/*  637 */  { itemToBeCoded,               NOPARAM,                     "TRI",                                         "TRIG",                                        CAT_MENU, SLS_UNCHANGED},      //JM
/*  638 */  { fnCvtTrozKg,                 multiply,                    "trz" STD_RIGHT_ARROW "kg",                    "tr.oz",                                       CAT_FNCT, SLS_ENABLED  },
/*  639 */  { itemToBeCoded,               NOPARAM,                     "TVM",                                         "TVM",                                         CAT_MENU, SLS_UNCHANGED},
/*  640 */  { itemToBeCoded,               NOPARAM,                     "t:",                                          "t:",                                          CAT_MENU, SLS_UNCHANGED},
/*  641 */  { itemToBeCoded,               NOPARAM,                     "t" STD_LEFT_RIGHT_ARROWS,                     "t" STD_LEFT_RIGHT_ARROWS,                     CAT_FNCT, SLS_UNCHANGED},
/*  642 */  { fnUlp,                       NOPARAM,                     "ULP?",                                        "ULP?",                                        CAT_FNCT, SLS_ENABLED  },
/*  643 */  { itemToBeCoded,               NOPARAM,                     "U" STD_SUB_n,                                 "U" STD_SUB_n,                                 CAT_FNCT, SLS_UNCHANGED},
/*  644 */  { fnUnitVector,                NOPARAM,                     "UNITV",                                       "UNITV",                                       CAT_FNCT, SLS_ENABLED  },
/*  645 */  { fnIntegerMode,               SIM_UNSIGN,                  "UNSIGN",                                      "UNSIGN",                                      CAT_FNCT, SLS_UNCHANGED},
/*  646 */  { itemToBeCoded,               NOPARAM,                     "UNIT",                                        "UNIT",                                        CAT_MENU, SLS_UNCHANGED},      //JM Change U> arrow to CONV. Changed again to UNIT
/*  647 */  { itemToBeCoded,               NOPARAM,                     "VARMNU",                                      "VARMNU",                                      CAT_FNCT, SLS_UNCHANGED},
/*  648 */  { itemToBeCoded,               NOPARAM,                     "VARS",                                        "VARS",                                        CAT_MENU, SLS_UNCHANGED},
/*  649 */  { fnVersion,                   NOPARAM,                     "VERS?",                                       "VERS?",                                       CAT_FNCT, SLS_UNCHANGED},
/*  650 */  { itemToBeCoded,               NOPARAM,                     "VIEW",                                        "VIEW",                                        CAT_FNCT, SLS_UNCHANGED},
/*  651 */  { fnConstant,                  54,                          "V" STD_SUB_m,                                 "V" STD_SUB_m,                                 CAT_CNST, SLS_ENABLED  },
/*  652 */  { itemToBeCoded,               NOPARAM,                     "V:",                                          "V:",                                          CAT_MENU, SLS_UNCHANGED},
/*  653 */  { itemToBeCoded,               NOPARAM,                     "WDAY",                                        "WDAY",                                        CAT_FNCT, SLS_UNCHANGED},
/*  654 */  { itemToBeCoded,               NOPARAM,                     "Weibl",                                       "Weibl",                                       CAT_FNCT, SLS_UNCHANGED},
/*  655 */  { itemToBeCoded,               NOPARAM,                     "Weibl" STD_SUB_p,                             "Weibl" STD_SUB_p,                             CAT_FNCT, SLS_UNCHANGED},
/*  656 */  { itemToBeCoded,               NOPARAM,                     "Weibl" STD_SUB_e,                             "Weibl" STD_SUB_e,                             CAT_FNCT, SLS_UNCHANGED},
/*  657 */  { itemToBeCoded,               NOPARAM,                     "Weibl" STD_SUP_MINUS_1,                       "Weibl" STD_SUP_MINUS_1,                       CAT_FNCT, SLS_UNCHANGED},
/*  658 */  { itemToBeCoded,               NOPARAM,                     "Weibl:",                                      "Weibl:",                                      CAT_MENU, SLS_UNCHANGED},
/*  659 */  { fnWho,                       NOPARAM,                     "WHO?",                                        "WHO?",                                        CAT_FNCT, SLS_UNCHANGED},
/*  660 */  { fnCvtWhJ,                    multiply,                    "Wh" STD_RIGHT_ARROW "J",                      "Wh" STD_RIGHT_ARROW "J",                      CAT_FNCT, SLS_ENABLED  },
/*  661 */  { itemToBeCoded,               NOPARAM,                     "W" STD_SUB_m,                                 "W" STD_SUB_m,                                 CAT_FNCT, SLS_UNCHANGED},
/*  662 */  { itemToBeCoded,               NOPARAM,                     "W" STD_SUB_p,                                 "W" STD_SUB_p,                                 CAT_FNCT, SLS_UNCHANGED},
/*  663 */  { itemToBeCoded,               NOPARAM,                     "W" STD_SUP_MINUS_1,                           "W" STD_SUP_MINUS_1,                           CAT_FNCT, SLS_UNCHANGED},
/*  664 */  { fnSetWordSize,               TM_VALUE,                    "WSIZE",                                       "WSIZE",                                       CAT_FNCT, SLS_UNCHANGED},
/*  665 */  { fnGetWordSize,               NOPARAM,                     "WSIZE?",                                      "WSIZE?",                                      CAT_FNCT, SLS_ENABLED  },
/*  666 */  { fnCvtHpeW,                   divide,                      "W" STD_RIGHT_ARROW "hp" STD_SUB_E,            "W" STD_RIGHT_ARROW "hp" STD_SUB_E,            CAT_FNCT, SLS_ENABLED  },
/*  667 */  { fnCvtHpmW,                   divide,                      "W" STD_RIGHT_ARROW "hp" STD_SUB_M,            "W" STD_RIGHT_ARROW "hp" STD_SUB_M,            CAT_FNCT, SLS_ENABLED  },
/*  668 */  { fnCvtHpukW,                  divide,                      "W" STD_RIGHT_ARROW "hp" STD_UK,               "W" STD_RIGHT_ARROW "hp" STD_UK,               CAT_FNCT, SLS_ENABLED  },
/*  669 */  { fnSquare,                    NOPARAM,                     "x" STD_SUP_2,                                 "x" STD_SUP_2,                                 CAT_FNCT, SLS_ENABLED  },
/*  670 */  { fnCube,                      NOPARAM,                     "x" STD_SUP_3,                                 "x" STD_SUP_3,                                 CAT_FNCT, SLS_ENABLED  },
/*  671 */  { itemToBeCoded,               NOPARAM,                     "XEQ",                                         "XEQ",                                         CAT_FNCT, SLS_UNCHANGED},
/*  672 */  { itemToBeCoded,               NOPARAM,                     "XNOR",                                        "XNOR",                                        CAT_FNCT, SLS_UNCHANGED},
/*  673 */  { itemToBeCoded,               NOPARAM,                     "XOR",                                         "XOR",                                         CAT_FNCT, SLS_UNCHANGED},
/*  674 */  { itemToBeCoded,               NOPARAM,                     STD_x_BAR,                                     STD_x_BAR,                                     CAT_FNCT, SLS_UNCHANGED},
/*  675 */  { itemToBeCoded,               NOPARAM,                     STD_x_BAR STD_SUB_G,                           STD_x_BAR STD_SUB_G,                           CAT_FNCT, SLS_UNCHANGED},
/*  676 */  { itemToBeCoded,               NOPARAM,                     STD_x_BAR STD_SUB_w,                           STD_x_BAR STD_SUB_w,                           CAT_FNCT, SLS_UNCHANGED},
/*  677 */  { itemToBeCoded,               NOPARAM,                     STD_x_CIRC,                                    STD_x_CIRC,                                    CAT_FNCT, SLS_UNCHANGED},
/*  678 */  { itemToBeCoded,               NOPARAM,                     "X.FN",                                        "X.FN",                                        CAT_MENU, SLS_UNCHANGED},
/*  679 */  { fnFactorial,                 NOPARAM,                     "x!",                                          "x!",                                          CAT_FNCT, SLS_ENABLED  },
/*  680 */  { itemToBeCoded,               NOPARAM,                     "x:",                                          "x:",                                          CAT_MENU, SLS_UNCHANGED},
/*  681 */  { itemToBeCoded,               NOPARAM,                     "x" STD_RIGHT_ARROW "DATE",                    "x" STD_RIGHT_ARROW "DATE",                    CAT_FNCT, SLS_UNCHANGED},
/*  682 */  { itemToBeCoded,               NOPARAM,                     "x" STD_RIGHT_ARROW STD_alpha,                 "x" STD_RIGHT_ARROW STD_alpha,                 CAT_FNCT, SLS_UNCHANGED},
/*  683 */  { itemToBeCoded,               NOPARAM,                     "x" STD_LEFT_RIGHT_ARROWS,                     "x" STD_LEFT_RIGHT_ARROWS,                     CAT_FNCT, SLS_UNCHANGED},
/*  684 */  { fnSwapXY,                    NOPARAM,                     "x" STD_LEFT_RIGHT_ARROWS "y",                 "x" STD_LEFT_RIGHT_ARROWS "y",                 CAT_FNCT, SLS_ENABLED  },
/*  685 */  { itemToBeCoded,               NOPARAM,                     "x= ?",                                        "x= ?",                                        CAT_FNCT, SLS_UNCHANGED},
/*  686 */  { itemToBeCoded,               NOPARAM,                     "x" STD_NOT_EQUAL " ?",                        "x" STD_NOT_EQUAL " ?",                        CAT_FNCT, SLS_UNCHANGED},
/*  687 */  { itemToBeCoded,               NOPARAM,                     "x=+0?",                                       "x=+0?",                                       CAT_FNCT, SLS_UNCHANGED},
/*  688 */  { itemToBeCoded,               NOPARAM,                     "x=-0?",                                       "x=-0?",                                       CAT_FNCT, SLS_UNCHANGED},
/*  689 */  { itemToBeCoded,               NOPARAM,                     "x" STD_ALMOST_EQUAL " ?",                     "x" STD_ALMOST_EQUAL " ?",                     CAT_FNCT, SLS_UNCHANGED},
/*  690 */  { fnXLessThan,                 NOPARAM,                     "x< ?",                                        "x< ?",                                        CAT_FNCT, SLS_UNCHANGED},
/*  691 */  { itemToBeCoded,               NOPARAM,                     "x" STD_LESS_EQUAL " ?",                       "x" STD_LESS_EQUAL " ?",                       CAT_FNCT, SLS_UNCHANGED},
/*  692 */  { itemToBeCoded,               NOPARAM,                     "x" STD_GREATER_EQUAL " ?",                    "x" STD_GREATER_EQUAL " ?",                    CAT_FNCT, SLS_UNCHANGED},
/*  693 */  { itemToBeCoded,               NOPARAM,                     "x> ?",                                        "x> ?",                                        CAT_FNCT, SLS_UNCHANGED},
/*  694 */  { itemToBeCoded,               NOPARAM,                     STD_xTH_ROOT STD_y_UNDER_ROOT,                 STD_xTH_ROOT STD_y_UNDER_ROOT,                 CAT_FNCT, SLS_ENABLED  },
/*  695 */  { fnCvtYardM,                  multiply,                    "yd." STD_RIGHT_ARROW "m",                     "yd." STD_RIGHT_ARROW "m",                     CAT_FNCT, SLS_ENABLED  },
/*  696 */  { itemToBeCoded,               NOPARAM,                     "YEAR",                                        "YEAR",                                        CAT_FNCT, SLS_UNCHANGED},
/*  697 */  { fnCvtYearS,                  multiply,                    "year" STD_RIGHT_ARROW "s",                    "year" STD_RIGHT_ARROW "s",                    CAT_FNCT, SLS_ENABLED  },
/*  698 */  { fnPower,                     NOPARAM,                     "y" STD_SUP_x,                                 "y" STD_SUP_x,                                 CAT_FNCT, SLS_ENABLED  },
/*  699 */  { itemToBeCoded,               NOPARAM,                     STD_y_CIRC,                                    STD_y_CIRC,                                    CAT_FNCT, SLS_UNCHANGED},
/*  700 */  { fnDateFormat,                DF_YMD,                      "Y.MD",                                        "Y.MD",                                        CAT_FNCT, SLS_UNCHANGED},
/*  701 */  { itemToBeCoded,               NOPARAM,                     "y" STD_LEFT_RIGHT_ARROWS,                     "y" STD_LEFT_RIGHT_ARROWS,                     CAT_FNCT, SLS_UNCHANGED},
/*  702 */  { fnConstant,                  55,                          "Z" STD_SUB_0,                                 "Z" STD_SUB_0,                                 CAT_CNST, SLS_ENABLED  },
/*  703 */  { itemToBeCoded,               NOPARAM,                     "z" STD_LEFT_RIGHT_ARROWS,                     "z" STD_LEFT_RIGHT_ARROWS,                     CAT_FNCT, SLS_UNCHANGED},
/*  704 */  { fnConstant,                  56,                          STD_alpha,                                     STD_alpha,                                     CAT_CNST, SLS_ENABLED  },
/*  705 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "INTL",                              STD_alpha "INTL",                              CAT_MENU, SLS_UNCHANGED},
/*  706 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "LENG?",                             STD_alpha "LENG?",                             CAT_FNCT, SLS_UNCHANGED},
/*  707 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "MATH",                              STD_alpha "MATH",                              CAT_MENU, SLS_UNCHANGED},
/*  708 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "OFF",                               STD_alpha "OFF",                               CAT_FNCT, SLS_UNCHANGED},
/*  709 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "ON",                                STD_alpha "ON",                                CAT_FNCT, SLS_UNCHANGED},
/*  710 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "POS?",                              STD_alpha "POS?",                              CAT_FNCT, SLS_UNCHANGED},
/*  711 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "RL",                                STD_alpha "RL",                                CAT_FNCT, SLS_UNCHANGED},
/*  712 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "RR",                                STD_alpha "RR",                                CAT_FNCT, SLS_UNCHANGED},
/*  713 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "SL",                                STD_alpha "SL",                                CAT_FNCT, SLS_UNCHANGED},
/*  714 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "STR",                               STD_alpha "STR",                               CAT_MENU, SLS_UNCHANGED},      //JM Changed a.FN to STRNG
/*  715 */  { itemToBeCoded,               NOPARAM,                     STD_ALPHA STD_ELLIPSIS STD_OMEGA,              STD_ALPHA STD_ELLIPSIS STD_OMEGA,              CAT_MENU, SLS_UNCHANGED},
/*  716 */  { itemToBeCoded,               NOPARAM,                     STD_alpha STD_DOT,                             STD_alpha STD_DOT,                             CAT_MENU, SLS_UNCHANGED},
/*  717 */  { itemToBeCoded,               NOPARAM,                     STD_alpha STD_RIGHT_ARROW "x",                 STD_alpha STD_RIGHT_ARROW "x",                 CAT_FNCT, SLS_UNCHANGED},
/*  718 */  { itemToBeCoded,               NOPARAM,                     STD_beta "(x,y)",                              STD_beta "(x,y)",                              CAT_FNCT, SLS_UNCHANGED},
/*  719 */  { fnConstant,                  57,                          STD_gamma,                                     STD_gamma,                                     CAT_CNST, SLS_ENABLED  },
/*  720 */  { fnConstant,                  58,                          STD_gamma STD_SUB_E STD_SUB_M,                 STD_gamma STD_SUB_E STD_SUB_M,                 CAT_CNST, SLS_ENABLED  },
/*  721 */  { fnConstant,                  59,                          STD_gamma STD_SUB_p,                           STD_gamma STD_SUB_p,                           CAT_CNST, SLS_ENABLED  },
/*  722 */  { itemToBeCoded,               NOPARAM,                     STD_gamma STD_SUB_x STD_SUB_y,                 STD_gamma STD_SUB_x STD_SUB_y,                 CAT_FNCT, SLS_UNCHANGED},
/*  723 */  { itemToBeCoded,               NOPARAM,                     STD_GAMMA STD_SUB_x STD_SUB_y,                 STD_GAMMA STD_SUB_x STD_SUB_y,                 CAT_FNCT, SLS_UNCHANGED},
/*  724 */  { fnGamma,                     NOPARAM,                     STD_GAMMA "(x)",                               STD_GAMMA "(x)",                               CAT_FNCT, SLS_ENABLED  },
/*  725 */  { itemToBeCoded,               NOPARAM,                     STD_delta "x",                                 STD_delta "x",                                 CAT_FNCT, SLS_UNCHANGED},
/*  726 */  { itemToBeCoded,               NOPARAM,                     STD_DELTA "%",                                 STD_DELTA "%",                                 CAT_FNCT, SLS_UNCHANGED},
/*  727 */  { itemToBeCoded,               NOPARAM,                     STD_epsilon,                                   STD_epsilon,                                   CAT_FNCT, SLS_UNCHANGED},
/*  728 */  { fnConstant,                  61,                          STD_epsilon STD_SUB_0,                         STD_epsilon STD_SUB_0,                         CAT_CNST, SLS_ENABLED  },
/*  729 */  { itemToBeCoded,               NOPARAM,                     STD_epsilon STD_SUB_m,                         STD_epsilon STD_SUB_m,                         CAT_FNCT, SLS_UNCHANGED},
/*  730 */  { itemToBeCoded,               NOPARAM,                     STD_epsilon STD_SUB_p,                         STD_epsilon STD_SUB_p,                         CAT_FNCT, SLS_UNCHANGED},
/*  731 */  { itemToBeCoded,               NOPARAM,                     STD_zeta "(x)",                                STD_zeta "(x)",                                CAT_FNCT, SLS_UNCHANGED},
/*  732 */  { fnConstant,                  62,                          STD_lambda STD_SUB_C,                          STD_lambda STD_SUB_C,                          CAT_CNST, SLS_ENABLED  },
/*  733 */  { fnConstant,                  63,                          STD_lambda STD_SUB_C STD_SUB_n,                STD_lambda STD_SUB_C STD_SUB_n,                CAT_CNST, SLS_ENABLED  },
/*  734 */  { fnConstant,                  64,                          STD_lambda STD_SUB_C STD_SUB_p,                STD_lambda STD_SUB_C STD_SUB_p,                CAT_CNST, SLS_ENABLED  },
/*  735 */  { fnConstant,                  65,                          STD_mu STD_SUB_0,                              STD_mu STD_SUB_0,                              CAT_CNST, SLS_ENABLED  },
/*  736 */  { fnConstant,                  66,                          STD_mu STD_SUB_B,                              STD_mu STD_SUB_B,                              CAT_CNST, SLS_ENABLED  },
/*  737 */  { fnConstant,                  67,                          STD_mu STD_SUB_e,                              STD_mu STD_SUB_e,                              CAT_CNST, SLS_ENABLED  },
/*  738 */  { fnConstant,                  68,                          STD_mu STD_SUB_e "/" STD_mu STD_SUB_B,         STD_mu STD_SUB_e "/" STD_mu STD_SUB_B,         CAT_CNST, SLS_ENABLED  },
/*  739 */  { fnConstant,                  69,                          STD_mu STD_SUB_n,                              STD_mu STD_SUB_n,                              CAT_CNST, SLS_ENABLED  },
/*  740 */  { fnConstant,                  70,                          STD_mu STD_SUB_p,                              STD_mu STD_SUB_p,                              CAT_CNST, SLS_ENABLED  },
/*  741 */  { fnConstant,                  71,                          STD_mu STD_SUB_u,                              STD_mu STD_SUB_u,                              CAT_CNST, SLS_ENABLED  },
/*  742 */  { fnConstant,                  72,                          STD_mu STD_SUB_mu,                             STD_mu STD_SUB_mu,                             CAT_CNST, SLS_ENABLED  },
/*  743 */  { itemToBeCoded,               NOPARAM,                     STD_PI STD_SUB_n,                              STD_PI STD_SUB_n,                              CAT_FNCT, SLS_UNCHANGED},
/*  744 */  { fnPi,                        NOPARAM,                     STD_pi,                                        STD_pi,                                        CAT_NONE, SLS_ENABLED  },
/*  745 */  { itemToBeCoded,               NOPARAM,                     STD_SIGMA STD_SUB_n,                           STD_SIGMA STD_SUB_n,                           CAT_FNCT, SLS_UNCHANGED},
/*  746 */  { itemToBeCoded,               NOPARAM,                     STD_sigma,                                     STD_sigma,                                     CAT_FNCT, SLS_UNCHANGED},
/*  747 */  { fnConstant,                  73,                          STD_sigma STD_SUB_B,                           STD_sigma STD_SUB_B,                           CAT_CNST, SLS_ENABLED  },
/*  748 */  { fnStatSum,                   9,                           STD_SIGMA "ln" STD_SUP_2 "x",                  STD_SIGMA "ln" STD_SUP_2 "x",                  CAT_FNCT, SLS_ENABLED  },
/*  749 */  { fnStatSum,                   12,                          STD_SIGMA "ln" STD_SUP_2 "y",                  STD_SIGMA "ln" STD_SUP_2 "y",                  CAT_FNCT, SLS_ENABLED  },
/*  750 */  { fnStatSum,                   8,                           STD_SIGMA "lnx",                               STD_SIGMA "lnx",                               CAT_FNCT, SLS_ENABLED  },
/*  751 */  { fnStatSum,                   7,                           STD_SIGMA "lnxy",                              STD_SIGMA "lnxy",                              CAT_FNCT, SLS_ENABLED  },
/*  752 */  { fnStatSum,                   11,                          STD_SIGMA "lny",                               STD_SIGMA "lny",                               CAT_FNCT, SLS_ENABLED  },
/*  753 */  { itemToBeCoded,               NOPARAM,                     STD_sigma STD_SUB_w,                           STD_sigma STD_SUB_w,                           CAT_FNCT, SLS_UNCHANGED},
/*  754 */  { fnStatSum,                   1,                           STD_SIGMA "x",                                 STD_SIGMA "x",                                 CAT_FNCT, SLS_ENABLED  },
/*  755 */  { fnStatSum,                   3,                           STD_SIGMA "x" STD_SUP_2,                       STD_SIGMA "x" STD_SUP_2,                       CAT_FNCT, SLS_ENABLED  },
/*  756 */  { fnStatSum,                   4,                           STD_SIGMA "x" STD_SUP_2 "y",                   STD_SIGMA "x" STD_SUP_2 "y",                   CAT_FNCT, SLS_ENABLED  },
/*  757 */  { fnStatSum,                   13,                          STD_SIGMA "xlny",                              STD_SIGMA "xlny",                              CAT_FNCT, SLS_ENABLED  },
/*  758 */  { fnStatSum,                   6,                           STD_SIGMA "xy",                                STD_SIGMA "xy",                                CAT_FNCT, SLS_ENABLED  },
/*  759 */  { fnStatSum,                   2,                           STD_SIGMA "y",                                 STD_SIGMA "y",                                 CAT_FNCT, SLS_ENABLED  },
/*  760 */  { fnStatSum,                   5,                           STD_SIGMA "y" STD_SUP_2,                       STD_SIGMA "y" STD_SUP_2,                       CAT_FNCT, SLS_ENABLED  },
/*  761 */  { fnStatSum,                   10,                          STD_SIGMA "ylnx",                              STD_SIGMA "ylnx",                              CAT_FNCT, SLS_ENABLED  },
/*  762 */  { fnSigma,                     1,                           STD_SIGMA "+",                                 STD_SIGMA "+",                                 CAT_FNCT, SLS_DISABLED },
/*  763 */  { fnSigma,                     2,                           STD_SIGMA "-",                                 STD_SIGMA "-",                                 CAT_FNCT, SLS_DISABLED },
/*  764 */  { fnConstant,                  74,                          STD_PHI,                                       STD_PHI,                                       CAT_CNST, SLS_ENABLED  },
/*  765 */  { fnConstant,                  75,                          STD_PHI STD_SUB_0,                             STD_PHI STD_SUB_0,                             CAT_CNST, SLS_ENABLED  },
/*  766 */  { itemToBeCoded,               NOPARAM,                     "0766",                                        "0766",                                        CAT_FREE, SLS_UNCHANGED},
/*  767 */  { itemToBeCoded,               NOPARAM,                     "0767",                                        "0767",                                        CAT_FREE, SLS_UNCHANGED},
/*  768 */  { itemToBeCoded,               NOPARAM,                     "0768",                                        "0768",                                        CAT_FREE, SLS_UNCHANGED},
/*  769 */  { itemToBeCoded,               NOPARAM,                     "0769",                                        "0769",                                        CAT_FREE, SLS_UNCHANGED},
/*  770 */  { itemToBeCoded,               NOPARAM,                     "0770",                                        "0770",                                        CAT_FREE, SLS_UNCHANGED},
/*  771 */  { itemToBeCoded,               NOPARAM,                     STD_chi STD_SUP_2 "(x)",                       STD_chi STD_SUP_2 "(x)",                       CAT_FNCT, SLS_UNCHANGED},
/*  772 */  { itemToBeCoded,               NOPARAM,                     STD_chi STD_SUP_2 STD_SUB_p "(x)",             STD_chi STD_SUP_2 STD_SUB_p "(x)",             CAT_FNCT, SLS_UNCHANGED},
/*  773 */  { itemToBeCoded,               NOPARAM,                     STD_chi STD_SUP_2 STD_SUB_e "(x)",             STD_chi STD_SUP_2 STD_SUB_e "(x)",             CAT_FNCT, SLS_UNCHANGED},
/*  774 */  { itemToBeCoded,               NOPARAM,                     "(" STD_chi STD_SUP_2 ")" STD_SUP_MINUS_1,     "(" STD_chi STD_SUP_2 ")" STD_SUP_MINUS_1,     CAT_FNCT, SLS_UNCHANGED},
/*  775 */  { itemToBeCoded,               NOPARAM,                     STD_chi STD_SUP_2 ":",                         STD_chi STD_SUP_2 ":",                         CAT_MENU, SLS_UNCHANGED},
/*  776 */  { fnConstant,                  76,                          STD_omega,                                     STD_omega,                                     CAT_CNST, SLS_ENABLED  },
/*  777 */  { fnM1Pow,                     NOPARAM,                     "(-1)" STD_SUP_x,                              "(-1)" STD_SUP_x,                              CAT_FNCT, SLS_ENABLED  },
/*  778 */  { fnAdd,                       ITM_ADD,                     "+",                                           "+",                                           CAT_FNCT, SLS_ENABLED  },
/*  779 */  { fnChangeSign,                ITM_CHS,                     "CHS",                                         "CHS",                                         CAT_FNCT, SLS_ENABLED  },      //JM Change +/- to CHS
/*  780 */  { fnSubtract,                  ITM_SUB,                     "-",                                           "-",                                           CAT_FNCT, SLS_ENABLED  },
/*  781 */  { fnConstant,                  77,                          "-" STD_INFINITY,                              "-" STD_INFINITY,                              CAT_CNST, SLS_ENABLED  },
/*  782 */  { fnMultiply,                  ITM_MULT,                    STD_CROSS,                                     STD_CROSS,                                     CAT_FNCT, SLS_ENABLED  },
/*  783 */  { itemToBeCoded,               NOPARAM,                     STD_CROSS "MOD",                               STD_CROSS "MOD",                               CAT_FNCT, SLS_UNCHANGED},
/*  784 */  { fnDivide,                    ITM_DIV,                     "/",                                           "/",                                           CAT_FNCT, SLS_ENABLED  },
/*  785 */  { itemToBeCoded,               NOPARAM,                     STD_PLUS_MINUS STD_INFINITY "?",               STD_PLUS_MINUS STD_INFINITY "?",               CAT_FNCT, SLS_UNCHANGED},
/*  786 */  { addItemToBuffer,             ITM_INDIRECTION,             STD_RIGHT_ARROW,                               STD_RIGHT_ARROW,                               CAT_NONE, SLS_UNCHANGED},
/*  787 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "DATE",                        STD_RIGHT_ARROW "DATE",                        CAT_FNCT, SLS_UNCHANGED},
/*  788 */  { fnCvtFromCurrentAngularMode, AM_DEGREE,                   STD_RIGHT_ARROW "DEG",                         STD_RIGHT_ARROW "DEG",                         CAT_FNCT, SLS_ENABLED  },
/*  789 */  { fnCvtFromCurrentAngularMode, AM_DMS,                      STD_RIGHT_ARROW "D.MS",                        STD_RIGHT_ARROW "D.MS",                        CAT_FNCT, SLS_ENABLED  },
/*  790 */  { fnCvtFromCurrentAngularMode, AM_GRAD,                     STD_RIGHT_ARROW "GRAD",                        STD_RIGHT_ARROW "GRAD",                        CAT_FNCT, SLS_ENABLED  },
/*  791 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "HR",                          ".d",                                          CAT_FNCT, SLS_ENABLED  },
/*  792 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "H.MS",                        STD_RIGHT_ARROW "h.ms",                        CAT_FNCT, SLS_UNCHANGED},     //JM mod
/*  793 */  { fnChangeBase,                TM_VALUE_CHB,                STD_RIGHT_ARROW "INT",                         "#",                                           CAT_FNCT, SLS_UNCHANGED},
/*  794 */  { fnCvtFromCurrentAngularMode, AM_MULTPI,                   STD_RIGHT_ARROW "MUL" STD_pi,                  STD_RIGHT_ARROW "MUL" STD_pi,                  CAT_FNCT, SLS_ENABLED  },
/*  795 */  { fnToPolar,                   NOPARAM,                     STD_RIGHT_ARROW "POL",                         STD_RIGHT_ARROW "P",                           CAT_FNCT, SLS_ENABLED  },
/*  796 */  { fnCvtFromCurrentAngularMode, AM_RADIAN,                   STD_RIGHT_ARROW "RAD",                         STD_RIGHT_ARROW "RAD",                         CAT_FNCT, SLS_ENABLED  },
/*  797 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "REAL",                        ".d",                                          CAT_FNCT, SLS_ENABLED  },
/*  798 */  { fnToRect,                    NOPARAM,                     STD_RIGHT_ARROW "REC",                         STD_RIGHT_ARROW "R",                           CAT_FNCT, SLS_ENABLED  },      //SWAPPED THE ARROW DIRECTION AND POSITION
/*  799 */  { fnCvtDegToDms,               NOPARAM,                     "D" STD_RIGHT_ARROW "D.MS",                    "D" STD_RIGHT_ARROW "D.MS",                    CAT_FNCT, SLS_ENABLED  },
/*  800 */  { itemToBeCoded,               NOPARAM,                     STD_UP_ARROW "Lim",                            STD_UP_ARROW "Lim",                            CAT_RVAR, SLS_UNCHANGED},
/*  801 */  { itemToBeCoded,               NOPARAM,                     STD_DOWN_ARROW "Lim",                          STD_DOWN_ARROW "Lim",                          CAT_RVAR, SLS_UNCHANGED},
/*  802 */  { itemToBeCoded,               NOPARAM,                     STD_LEFT_RIGHT_ARROWS,                         STD_LEFT_RIGHT_ARROWS,                         CAT_FNCT, SLS_UNCHANGED},
/*  803 */  { itemToBeCoded,               NOPARAM,                     "%",                                           "%",                                           CAT_FNCT, SLS_UNCHANGED},
/*  804 */  { itemToBeCoded,               NOPARAM,                     "%MRR",                                        "%MRR",                                        CAT_FNCT, SLS_UNCHANGED},
/*  805 */  { itemToBeCoded,               NOPARAM,                     "%T",                                          "%T",                                          CAT_FNCT, SLS_UNCHANGED},
/*  806 */  { itemToBeCoded,               NOPARAM,                     "%" STD_SIGMA,                                 "%" STD_SIGMA,                                 CAT_FNCT, SLS_UNCHANGED},
/*  807 */  { itemToBeCoded,               NOPARAM,                     "%+MG",                                        "%+MG",                                        CAT_FNCT, SLS_UNCHANGED},
/*  808 */  { fnSquareRoot,                NOPARAM,                     STD_SQUARE_ROOT STD_x_UNDER_ROOT,              STD_SQUARE_ROOT STD_x_UNDER_ROOT,              CAT_FNCT, SLS_ENABLED  },
/*  809 */  { itemToBeCoded,               NOPARAM,                     STD_INTEGRAL,                                  STD_INTEGRAL,                                  CAT_FNCT, SLS_UNCHANGED},
/*  810 */  { itemToBeCoded,               NOPARAM,                     STD_INTEGRAL "f",                              STD_INTEGRAL "f",                              CAT_MENU, SLS_UNCHANGED},
/*  811 */  { itemToBeCoded,               NOPARAM,                     STD_INTEGRAL "fdx",                            STD_INTEGRAL "fdx",                            CAT_MENU, SLS_UNCHANGED},
/*  812 */  { fnConstant,                  78,                          STD_INFINITY,                                  STD_INFINITY,                                  CAT_CNST, SLS_ENABLED  },
/*  813 */  { itemToBeCoded,               NOPARAM,                     "^MOD",                                        "^MOD",                                        CAT_FNCT, SLS_UNCHANGED},
/*  814 */  { itemToBeCoded,               NOPARAM,                     "|M|",                                         "|M|",                                         CAT_FNCT, SLS_UNCHANGED},
/*  815 */  { fnMagnitude,                 NOPARAM,                     "|x|",                                         "|x|",                                         CAT_FNCT, SLS_ENABLED  },
/*  816 */  { fnParallel,                  NOPARAM,                     "||",                                          "|" STD_SPACE_3_PER_EM "|",                    CAT_FNCT, SLS_ENABLED  },
/*  817 */  { itemToBeCoded,               NOPARAM,                     "[M]" STD_SUP_T,                               "[M]" STD_SUP_T,                               CAT_FNCT, SLS_UNCHANGED},
/*  818 */  { itemToBeCoded,               NOPARAM,                     "[M]" STD_SUP_MINUS_1,                         "[M]" STD_SUP_MINUS_1,                         CAT_FNCT, SLS_UNCHANGED},
/*  819 */  { fnJM,                        002,       /*JMCALC*/        STD_MEASURED_ANGLE,                            STD_MEASURED_ANGLE,                            CAT_FNCT, SLS_UNCHANGED},      //JM CALC
/*  820 */  { fnCvtToCurrentAngularMode,   AM_MULTPI,                   "MUL" STD_pi STD_RIGHT_ARROW,                  "MUL" STD_pi STD_RIGHT_ARROW,                  CAT_FNCT, SLS_ENABLED  },
/*  821 */  { itemToBeCoded,               NOPARAM,                     "CONV",                                        "CONV",                                        CAT_MENU, SLS_UNCHANGED},      //JM Change to text DRG and change again to CONV
/*  822 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "ADV",                             STD_PRINTER "ADV",                             CAT_FNCT, SLS_UNCHANGED},
/*  823 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "CHAR",                            STD_PRINTER "CHAR",                            CAT_FNCT, SLS_UNCHANGED},
/*  824 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "DLAY",                            STD_PRINTER "DLAY",                            CAT_FNCT, SLS_UNCHANGED},
/*  825 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "LCD",                             STD_PRINTER "LCD",                             CAT_FNCT, SLS_UNCHANGED},
/*  826 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "MODE",                            STD_PRINTER "MODE",                            CAT_FNCT, SLS_UNCHANGED},
/*  827 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "PROG",                            STD_PRINTER "PROG",                            CAT_FNCT, SLS_UNCHANGED},
/*  828 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "r",                               STD_PRINTER "r",                               CAT_FNCT, SLS_UNCHANGED},
/*  829 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "REGS",                            STD_PRINTER "REGS",                            CAT_FNCT, SLS_UNCHANGED},
/*  830 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "STK",                             STD_PRINTER "STK",                             CAT_FNCT, SLS_UNCHANGED},
/*  831 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "TAB",                             STD_PRINTER "TAB",                             CAT_FNCT, SLS_UNCHANGED},
/*  832 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "USER",                            STD_PRINTER "USER",                            CAT_FNCT, SLS_UNCHANGED},
/*  833 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "WIDTH",                           STD_PRINTER "WIDTH",                           CAT_FNCT, SLS_UNCHANGED},
/*  834 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER STD_SIGMA,                         STD_PRINTER STD_SIGMA,                         CAT_FNCT, SLS_UNCHANGED},
/*  835 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "#",                               STD_PRINTER "#",                               CAT_FNCT, SLS_UNCHANGED},
/*  836 */  { itemToBeCoded,               NOPARAM,                     "#",                                           "#",                                           CAT_NONE, SLS_UNCHANGED},
/*  837 */  { itemToBeCoded,               NOPARAM,                     "#B",                                          "#B",                                          CAT_FNCT, SLS_UNCHANGED},
/*  838 */  { fnCvtAcreM2,                 multiply,                    "ac" STD_RIGHT_ARROW "m" STD_SUP_2,            STD_RIGHT_ARROW " m" STD_SUP_2,                CAT_DUPL, SLS_ENABLED  },
/*  839 */  { fnCvtAcreusM2,               multiply,                    "ac" STD_US STD_RIGHT_ARROW "m" STD_SUP_2,     STD_RIGHT_ARROW " m" STD_SUP_2,                CAT_DUPL, SLS_ENABLED  },
/*  840 */  { fnCvtCaratKg,                multiply,                    "ct" STD_RIGHT_ARROW "kg",                     "carat",                                       CAT_FNCT, SLS_ENABLED  },
/*  841 */  { fnCvtDbRatio,                20,                          "dB" STD_RIGHT_ARROW "fr",                     "field",                                       CAT_DUPL, SLS_ENABLED  },
/*  842 */  { fnCvtDbRatio,                20,                          "dB" STD_RIGHT_ARROW "fr",                     "ratio",                                       CAT_DUPL, SLS_ENABLED  },
/*  843 */  { fnCvtDbRatio,                10,                          "dB" STD_RIGHT_ARROW "pr",                     "power",                                       CAT_DUPL, SLS_ENABLED  },
/*  844 */  { fnCvtDbRatio,                10,                          "dB" STD_RIGHT_ARROW "pr",                     "ratio",                                       CAT_DUPL, SLS_ENABLED  },
/*  845 */  { fnCvtRatioDb,                20,                          "fr" STD_RIGHT_ARROW "dB",                     "ratio",                                       CAT_DUPL, SLS_ENABLED  },
/*  846 */  { fnCvtRatioDb,                20,                          "fr" STD_RIGHT_ARROW "dB",                     STD_RIGHT_ARROW " dB",                         CAT_DUPL, SLS_ENABLED  },
/*  847 */  { fnCvtSfeetM,                 multiply,                    "ft" STD_US STD_RIGHT_ARROW "m",               "foot" STD_US,                                 CAT_DUPL, SLS_ENABLED  },
/*  848 */  { fnCvtFlozukM3,               multiply,                    "fz" STD_UK STD_RIGHT_ARROW "m" STD_SUP_3,     STD_RIGHT_ARROW " m" STD_SUP_3,                CAT_DUPL, SLS_ENABLED  },
/*  849 */  { fnCvtFlozusM3,               multiply,                    "fz" STD_US STD_RIGHT_ARROW "m" STD_SUP_3,     STD_RIGHT_ARROW " m" STD_SUP_3,                CAT_DUPL, SLS_ENABLED  },
/*  850 */  { fnCvtInhgPa,                 multiply,                    "iHg" STD_RIGHT_ARROW "Pa",                    STD_RIGHT_ARROW " Pa",                         CAT_DUPL, SLS_ENABLED  },
/*  851 */  { fnCvtShortcwtKg,             divide,                      "kg" STD_RIGHT_ARROW "scw",                    "sh.cwt",                                      CAT_DUPL, SLS_ENABLED  },
/*  852 */  { fnCvtStoneKg,                divide,                      "kg" STD_RIGHT_ARROW "sto",                    "stone",                                       CAT_DUPL, SLS_ENABLED  },
/*  853 */  { fnCvtShorttonKg,             divide,                      "kg" STD_RIGHT_ARROW "s.t",                    "short",                                       CAT_DUPL, SLS_ENABLED  },
/*  854 */  { fnCvtShorttonKg,             divide,                      "kg" STD_RIGHT_ARROW "s.t",                    "ton",                                         CAT_DUPL, SLS_ENABLED  },
/*  855 */  { fnCvtCaratKg,                divide,                      "kg" STD_RIGHT_ARROW "ct",                     "kg " STD_RIGHT_ARROW,                         CAT_FNCT, SLS_ENABLED  },
/*  856 */  { fnCvtTrozKg,                 divide,                      "kg" STD_RIGHT_ARROW "trz",                    "tr.oz",                                       CAT_DUPL, SLS_ENABLED  },
/*  857 */  { fnCvtAcreM2,                 divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ac",            "acre",                                        CAT_DUPL, SLS_ENABLED  },
/*  858 */  { fnCvtAcreusM2,               divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ac" STD_US,     "acre" STD_US,                                 CAT_DUPL, SLS_ENABLED  },
/*  859 */  { fnCvtFlozukM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "fz" STD_UK,     "floz" STD_UK,                                 CAT_DUPL, SLS_ENABLED  },
/*  860 */  { fnCvtFlozusM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "fz" STD_US,     "floz" STD_US,                                 CAT_DUPL, SLS_ENABLED  },
/*  861 */  { fnCvtSfeetM,                 divide,                      "m" STD_RIGHT_ARROW "ft" STD_US,               "survey",                                      CAT_DUPL, SLS_ENABLED  },
/*  862 */  { fnCvtCaratKg,                multiply,                    "ct" STD_RIGHT_ARROW "kg",                     STD_RIGHT_ARROW " kg",                         CAT_DUPL, SLS_ENABLED  },
/*  863 */  { fnCvtInhgPa,                 divide,                      "Pa" STD_RIGHT_ARROW "iHg",                    "in.Hg",                                       CAT_DUPL, SLS_ENABLED  },
/*  864 */  { fnCvtTorrPa,                 divide,                      "Pa" STD_RIGHT_ARROW "tor",                    "torr",                                        CAT_DUPL, SLS_ENABLED  },
/*  865 */  { fnCvtRatioDb,                10,                          "pr" STD_RIGHT_ARROW "dB",                     "ratio",                                       CAT_DUPL, SLS_ENABLED  },
/*  866 */  { fnCvtRatioDb,                10,                          "pr" STD_RIGHT_ARROW "dB",                     STD_RIGHT_ARROW " dB",                         CAT_DUPL, SLS_ENABLED  },
/*  867 */  { fnCvtShortcwtKg,             multiply,                    "scw" STD_RIGHT_ARROW "kg",                    "cwt" STD_RIGHT_ARROW "kg",                    CAT_DUPL, SLS_ENABLED  },
/*  868 */  { fnCvtStoneKg,                multiply,                    "sto" STD_RIGHT_ARROW "kg",                    STD_RIGHT_ARROW " kg",                         CAT_DUPL, SLS_ENABLED  },
/*  869 */  { fnCvtShorttonKg,             multiply,                    "s.t" STD_RIGHT_ARROW "kg",                    "ton",                                         CAT_DUPL, SLS_ENABLED  },
/*  870 */  { fnCvtShorttonKg,             multiply,                    "s.t" STD_RIGHT_ARROW "kg",                    STD_RIGHT_ARROW " kg",                         CAT_DUPL, SLS_ENABLED  },
/*  871 */  { fnCvtCaratKg,                divide,                      "kg" STD_RIGHT_ARROW "ct",                     "carat",                                       CAT_DUPL, SLS_ENABLED  },
/*  872 */  { fnCvtTorrPa,                 multiply,                    "tor" STD_RIGHT_ARROW "Pa",                    STD_RIGHT_ARROW " Pa",                         CAT_DUPL, SLS_ENABLED  },
/*  873 */  { fnCvtTrozKg,                 multiply,                    "trz" STD_RIGHT_ARROW "kg",                    STD_RIGHT_ARROW " kg",                         CAT_DUPL, SLS_ENABLED  },

/*  874 */  { fontBrowser,                 NOPARAM,                     "FBR",                                         "FBR",                                         CAT_FNCT, SLS_UNCHANGED}, // Font Browser
/*  875 */  { itemToBeCoded,               NOPARAM,                     "CFG",                                         "CFG",                                         CAT_NONE, SLS_UNCHANGED}, // JM Menu not defined 
/*  876 */  { itemToBeCoded,               NOPARAM,                     STD_alpha STD_ELLIPSIS STD_omega,              STD_alpha STD_ELLIPSIS STD_omega,              CAT_MENU, SLS_UNCHANGED}, // Small greek letters
/*  877 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "intl",                              STD_alpha "intl",                              CAT_MENU, SLS_UNCHANGED}, // Small intl letters
/*  878 */  { addItemToBuffer,             REGISTER_X,                  "REG_X",                                       "X",                                           CAT_NONE, SLS_UNCHANGED}, // The order
/*  879 */  { addItemToBuffer,             REGISTER_Y,                  "REG_Y",                                       "Y",                                           CAT_NONE, SLS_UNCHANGED}, // of these 8
/*  880 */  { addItemToBuffer,             REGISTER_Z,                  "REG_Z",                                       "Z",                                           CAT_NONE, SLS_UNCHANGED}, // lines MUST
/*  881 */  { addItemToBuffer,             REGISTER_T,                  "REG_T",                                       "T",                                           CAT_NONE, SLS_UNCHANGED}, // be kept as
/*  882 */  { addItemToBuffer,             REGISTER_A,                  "REG_A",                                       "A",                                           CAT_NONE, SLS_UNCHANGED}, // is. Do not
/*  883 */  { addItemToBuffer,             REGISTER_B,                  "REG_B",                                       "B",                                           CAT_NONE, SLS_UNCHANGED}, // put them in
/*  884 */  { addItemToBuffer,             REGISTER_C,                  "REG_C",                                       "C",                                           CAT_NONE, SLS_UNCHANGED}, // alphabetical
/*  885 */  { addItemToBuffer,             REGISTER_D,                  "REG_D",                                       "D",                                           CAT_NONE, SLS_UNCHANGED}, // order!
/*  886 */  { addItemToBuffer,             REGISTER_L,                  "REG_L",                                       "L",                                           CAT_NONE, SLS_UNCHANGED},
/*  887 */  { addItemToBuffer,             REGISTER_I,                  "REG_I",                                       "I",                                           CAT_NONE, SLS_UNCHANGED},
/*  888 */  { addItemToBuffer,             REGISTER_J,                  "REG_J",                                       "J",                                           CAT_NONE, SLS_UNCHANGED},
/*  889 */  { addItemToBuffer,             REGISTER_K,                  "REG_K",                                       "K",                                           CAT_NONE, SLS_UNCHANGED},

/*  890 */  { addItemToBuffer,             CHR_0,                       "",                                            "0",                                           CAT_NONE, SLS_UNCHANGED},
/*  891 */  { addItemToBuffer,             CHR_1,                       "",                                            "1",                                           CAT_NONE, SLS_UNCHANGED},
/*  892 */  { addItemToBuffer,             CHR_2,                       "",                                            "2",                                           CAT_NONE, SLS_UNCHANGED},
/*  893 */  { addItemToBuffer,             CHR_3,                       "",                                            "3",                                           CAT_NONE, SLS_UNCHANGED},
/*  894 */  { addItemToBuffer,             CHR_4,                       "",                                            "4",                                           CAT_NONE, SLS_UNCHANGED},
/*  895 */  { addItemToBuffer,             CHR_5,                       "",                                            "5",                                           CAT_NONE, SLS_UNCHANGED},
/*  896 */  { addItemToBuffer,             CHR_6,                       "",                                            "6",                                           CAT_NONE, SLS_UNCHANGED},
/*  897 */  { addItemToBuffer,             CHR_7,                       "",                                            "7",                                           CAT_NONE, SLS_UNCHANGED},
/*  898 */  { addItemToBuffer,             CHR_8,                       "",                                            "8",                                           CAT_NONE, SLS_UNCHANGED},
/*  899 */  { addItemToBuffer,             CHR_9,                       "",                                            "9",                                           CAT_NONE, SLS_UNCHANGED},
/*  900 */  { addItemToBuffer,             CHR_A,                       "",                                            "A",                                           CAT_NONE, SLS_UNCHANGED},
/*  901 */  { addItemToBuffer,             CHR_B,                       "",                                            "B",                                           CAT_NONE, SLS_UNCHANGED},
/*  902 */  { addItemToBuffer,             CHR_C,                       "",                                            "C",                                           CAT_NONE, SLS_UNCHANGED},
/*  903 */  { addItemToBuffer,             CHR_D,                       "",                                            "D",                                           CAT_NONE, SLS_UNCHANGED},
/*  904 */  { addItemToBuffer,             CHR_E,                       "",                                            "E",                                           CAT_NONE, SLS_UNCHANGED},
/*  905 */  { addItemToBuffer,             CHR_F,                       "",                                            "F",                                           CAT_NONE, SLS_UNCHANGED},
/*  906 */  { addItemToBuffer,             CHR_G,                       "",                                            "G",                                           CAT_NONE, SLS_UNCHANGED},
/*  907 */  { addItemToBuffer,             CHR_H,                       "",                                            "H",                                           CAT_NONE, SLS_UNCHANGED},
/*  908 */  { addItemToBuffer,             CHR_I,                       "",                                            "I",                                           CAT_NONE, SLS_UNCHANGED},
/*  909 */  { addItemToBuffer,             CHR_J,                       "",                                            "J",                                           CAT_NONE, SLS_UNCHANGED},
/*  910 */  { addItemToBuffer,             CHR_K,                       "",                                            "K",                                           CAT_NONE, SLS_UNCHANGED},
/*  911 */  { addItemToBuffer,             CHR_L,                       "",                                            "L",                                           CAT_NONE, SLS_UNCHANGED},
/*  912 */  { addItemToBuffer,             CHR_M,                       "",                                            "M",                                           CAT_NONE, SLS_UNCHANGED},
/*  913 */  { addItemToBuffer,             CHR_N,                       "",                                            "N",                                           CAT_NONE, SLS_UNCHANGED},
/*  914 */  { addItemToBuffer,             CHR_O,                       "",                                            "O",                                           CAT_NONE, SLS_UNCHANGED},
/*  915 */  { addItemToBuffer,             CHR_P,                       "",                                            "P",                                           CAT_NONE, SLS_UNCHANGED},
/*  916 */  { addItemToBuffer,             CHR_Q,                       "",                                            "Q",                                           CAT_NONE, SLS_UNCHANGED},
/*  917 */  { addItemToBuffer,             CHR_R,                       "",                                            "R",                                           CAT_NONE, SLS_UNCHANGED},
/*  918 */  { addItemToBuffer,             CHR_S,                       "",                                            "S",                                           CAT_NONE, SLS_UNCHANGED},
/*  919 */  { addItemToBuffer,             CHR_T,                       "",                                            "T",                                           CAT_NONE, SLS_UNCHANGED},
/*  920 */  { addItemToBuffer,             CHR_U,                       "",                                            "U",                                           CAT_NONE, SLS_UNCHANGED},
/*  921 */  { addItemToBuffer,             CHR_V,                       "",                                            "V",                                           CAT_NONE, SLS_UNCHANGED},
/*  922 */  { addItemToBuffer,             CHR_W,                       "",                                            "W",                                           CAT_NONE, SLS_UNCHANGED},
/*  923 */  { addItemToBuffer,             CHR_X,                       "",                                            "X",                                           CAT_NONE, SLS_UNCHANGED},
/*  924 */  { addItemToBuffer,             CHR_Y,                       "",                                            "Y",                                           CAT_NONE, SLS_UNCHANGED},
/*  925 */  { addItemToBuffer,             CHR_Z,                       "",                                            "Z",                                           CAT_NONE, SLS_UNCHANGED},
/*  926 */  { addItemToBuffer,             CHR_a,                       "",                                            "a",                                           CAT_NONE, SLS_UNCHANGED},
/*  927 */  { addItemToBuffer,             CHR_b,                       "",                                            "b",                                           CAT_NONE, SLS_UNCHANGED},
/*  928 */  { addItemToBuffer,             CHR_c,                       "",                                            "c",                                           CAT_NONE, SLS_UNCHANGED},
/*  929 */  { addItemToBuffer,             CHR_d,                       "",                                            "d",                                           CAT_NONE, SLS_UNCHANGED},
/*  930 */  { addItemToBuffer,             CHR_e,                       "",                                            "e",                                           CAT_NONE, SLS_UNCHANGED},
/*  931 */  { addItemToBuffer,             CHR_f,                       "",                                            "f",                                           CAT_NONE, SLS_UNCHANGED},
/*  932 */  { addItemToBuffer,             CHR_g,                       "",                                            "g",                                           CAT_NONE, SLS_UNCHANGED},
/*  933 */  { addItemToBuffer,             CHR_h,                       "",                                            "h",                                           CAT_NONE, SLS_UNCHANGED},
/*  934 */  { addItemToBuffer,             CHR_i,                       "",                                            "i",                                           CAT_NONE, SLS_UNCHANGED},
/*  935 */  { addItemToBuffer,             CHR_j,                       "",                                            "j",                                           CAT_NONE, SLS_UNCHANGED},
/*  936 */  { addItemToBuffer,             CHR_k,                       "",                                            "k",                                           CAT_NONE, SLS_UNCHANGED},
/*  937 */  { addItemToBuffer,             CHR_l,                       "",                                            "l",                                           CAT_NONE, SLS_UNCHANGED},
/*  938 */  { addItemToBuffer,             CHR_m,                       "",                                            "m",                                           CAT_NONE, SLS_UNCHANGED},
/*  939 */  { addItemToBuffer,             CHR_n,                       "",                                            "n",                                           CAT_NONE, SLS_UNCHANGED},
/*  940 */  { addItemToBuffer,             CHR_o,                       "",                                            "o",                                           CAT_NONE, SLS_UNCHANGED},
/*  941 */  { addItemToBuffer,             CHR_p,                       "",                                            "p",                                           CAT_NONE, SLS_UNCHANGED},
/*  942 */  { addItemToBuffer,             CHR_q,                       "",                                            "q",                                           CAT_NONE, SLS_UNCHANGED},
/*  943 */  { addItemToBuffer,             CHR_r,                       "",                                            "r",                                           CAT_NONE, SLS_UNCHANGED},
/*  944 */  { addItemToBuffer,             CHR_s,                       "",                                            "s",                                           CAT_NONE, SLS_UNCHANGED},
/*  945 */  { addItemToBuffer,             CHR_t,                       "",                                            "t",                                           CAT_NONE, SLS_UNCHANGED},
/*  946 */  { addItemToBuffer,             CHR_u,                       "",                                            "u",                                           CAT_NONE, SLS_UNCHANGED},
/*  947 */  { addItemToBuffer,             CHR_v,                       "",                                            "v",                                           CAT_NONE, SLS_UNCHANGED},
/*  948 */  { addItemToBuffer,             CHR_w,                       "",                                            "w",                                           CAT_NONE, SLS_UNCHANGED},
/*  949 */  { addItemToBuffer,             CHR_x,                       "",                                            "x",                                           CAT_NONE, SLS_UNCHANGED},
/*  950 */  { addItemToBuffer,             CHR_y,                       "",                                            "y",                                           CAT_NONE, SLS_UNCHANGED},
/*  951 */  { addItemToBuffer,             CHR_z,                       "",                                            "z",                                           CAT_NONE, SLS_UNCHANGED},
/*  952 */  { addItemToBuffer,             CHR_ALPHA,                   "",                                            STD_ALPHA,                                     CAT_NONE, SLS_UNCHANGED},
/*  953 */  { itemToBeCoded,               NOPARAM,                     "0953",                                        "0953",                                        CAT_FREE, SLS_UNCHANGED},
/*  954 */  { addItemToBuffer,             CHR_BETA,                    "",                                            STD_BETA,                                      CAT_NONE, SLS_UNCHANGED},
/*  955 */  { addItemToBuffer,             CHR_GAMMA,                   "",                                            STD_GAMMA,                                     CAT_NONE, SLS_UNCHANGED},
/*  956 */  { addItemToBuffer,             CHR_DELTA,                   "",                                            STD_DELTA,                                     CAT_NONE, SLS_UNCHANGED},
/*  957 */  { addItemToBuffer,             CHR_EPSILON,                 "",                                            STD_EPSILON,                                   CAT_NONE, SLS_UNCHANGED},
/*  958 */  { itemToBeCoded,               NOPARAM,                     "0958",                                        "0958",                                        CAT_FREE, SLS_UNCHANGED},
/*  959 */  { addItemToBuffer,             CHR_ZETA,                    "",                                            STD_ZETA,                                      CAT_NONE, SLS_UNCHANGED},
/*  960 */  { addItemToBuffer,             CHR_ETA,                     "",                                            STD_ETA,                                       CAT_NONE, SLS_UNCHANGED},
/*  961 */  { itemToBeCoded,               NOPARAM,                     "0961",                                        "0961",                                        CAT_FREE, SLS_UNCHANGED},
/*  962 */  { addItemToBuffer,             CHR_THETA,                   "",                                            STD_THETA,                                     CAT_NONE, SLS_UNCHANGED},
/*  963 */  { addItemToBuffer,             CHR_IOTA,                    "",                                            STD_IOTA,                                      CAT_NONE, SLS_UNCHANGED},
/*  964 */  { itemToBeCoded,               NOPARAM,                     "0964",                                        "0964",                                        CAT_FREE, SLS_UNCHANGED},
/*  965 */  { itemToBeCoded,               NOPARAM,                     "0965",                                        "0965",                                        CAT_FREE, SLS_UNCHANGED},
/*  966 */  { addItemToBuffer,             CHR_IOTA_DIALYTIKA,          "",                                            STD_IOTA_DIALYTIKA,                            CAT_NONE, SLS_UNCHANGED},
/*  967 */  { addItemToBuffer,             CHR_KAPPA,                   "",                                            STD_KAPPA,                                     CAT_NONE, SLS_UNCHANGED},
/*  968 */  { addItemToBuffer,             CHR_LAMBDA,                  "",                                            STD_LAMBDA,                                    CAT_NONE, SLS_UNCHANGED},
/*  969 */  { addItemToBuffer,             CHR_MU,                      "",                                            STD_MU,                                        CAT_NONE, SLS_UNCHANGED},
/*  970 */  { addItemToBuffer,             CHR_NU,                      "",                                            STD_NU,                                        CAT_NONE, SLS_UNCHANGED},
/*  971 */  { addItemToBuffer,             CHR_XI,                      "",                                            STD_XI,                                        CAT_NONE, SLS_UNCHANGED},
/*  972 */  { addItemToBuffer,             CHR_OMICRON,                 "",                                            STD_OMICRON,                                   CAT_NONE, SLS_UNCHANGED},
/*  973 */  { itemToBeCoded,               NOPARAM,                     "0973",                                        "0973",                                        CAT_FREE, SLS_UNCHANGED},
/*  974 */  { addItemToBuffer,             CHR_PI,                      "",                                            STD_PI,                                        CAT_NONE, SLS_UNCHANGED},
/*  975 */  { addItemToBuffer,             CHR_RHO,                     "",                                            STD_RHO,                                       CAT_NONE, SLS_UNCHANGED},
/*  976 */  { addItemToBuffer,             CHR_SIGMA,                   "",                                            STD_SIGMA,                                     CAT_NONE, SLS_UNCHANGED},
/*  977 */  { itemToBeCoded,               NOPARAM,                     "0977",                                        "0977",                                        CAT_FREE, SLS_UNCHANGED},
/*  978 */  { addItemToBuffer,             CHR_TAU,                     "",                                            STD_TAU,                                       CAT_NONE, SLS_UNCHANGED},
/*  979 */  { addItemToBuffer,             CHR_UPSILON,                 "",                                            STD_UPSILON,                                   CAT_NONE, SLS_UNCHANGED},
/*  980 */  { itemToBeCoded,               NOPARAM,                     "0980",                                        "0980",                                        CAT_FREE, SLS_UNCHANGED},
/*  981 */  { addItemToBuffer,             CHR_UPSILON_DIALYTIKA,       "",                                            STD_UPSILON_DIALYTIKA,                         CAT_NONE, SLS_UNCHANGED},
/*  982 */  { itemToBeCoded,               NOPARAM,                     "0982",                                        "0982",                                        CAT_FREE, SLS_UNCHANGED},
/*  983 */  { addItemToBuffer,             CHR_PHI,                     "",                                            STD_PHI,                                       CAT_NONE, SLS_UNCHANGED},
/*  984 */  { addItemToBuffer,             CHR_CHI,                     "",                                            STD_CHI,                                       CAT_NONE, SLS_UNCHANGED},
/*  985 */  { addItemToBuffer,             CHR_PSI,                     "",                                            STD_PSI,                                       CAT_NONE, SLS_UNCHANGED},
/*  986 */  { addItemToBuffer,             CHR_OMEGA,                   "",                                            STD_OMEGA,                                     CAT_NONE, SLS_UNCHANGED},
/*  987 */  { itemToBeCoded,               NOPARAM,                     "0987",                                        "0987",                                        CAT_FREE, SLS_UNCHANGED},
/*  988 */  { addItemToBuffer,             CHR_alpha,                   "",                                            STD_alpha,                                     CAT_NONE, SLS_UNCHANGED},
/*  989 */  { addItemToBuffer,             CHR_alpha_TONOS,             "",                                            STD_alpha_TONOS,                               CAT_NONE, SLS_UNCHANGED},
/*  990 */  { addItemToBuffer,             CHR_beta,                    "",                                            STD_beta,                                      CAT_NONE, SLS_UNCHANGED},
/*  991 */  { addItemToBuffer,             CHR_gamma,                   "",                                            STD_gamma,                                     CAT_NONE, SLS_UNCHANGED},
/*  992 */  { addItemToBuffer,             CHR_delta,                   "",                                            STD_delta,                                     CAT_NONE, SLS_UNCHANGED},
/*  993 */  { addItemToBuffer,             CHR_epsilon,                 "",                                            STD_epsilon,                                   CAT_NONE, SLS_UNCHANGED},
/*  994 */  { addItemToBuffer,             CHR_epsilon_TONOS,           "",                                            STD_epsilon_TONOS,                             CAT_NONE, SLS_UNCHANGED},
/*  995 */  { addItemToBuffer,             CHR_zeta,                    "",                                            STD_zeta,                                      CAT_NONE, SLS_UNCHANGED},
/*  996 */  { addItemToBuffer,             CHR_eta,                     "",                                            STD_eta,                                       CAT_NONE, SLS_UNCHANGED},
/*  997 */  { addItemToBuffer,             CHR_eta_TONOS,               "",                                            STD_eta_TONOS,                                 CAT_NONE, SLS_UNCHANGED},
/*  998 */  { addItemToBuffer,             CHR_theta,                   "",                                            STD_theta,                                     CAT_NONE, SLS_UNCHANGED},
/*  999 */  { addItemToBuffer,             CHR_iota,                    "",                                            STD_iota,                                      CAT_NONE, SLS_UNCHANGED},
/* 1000 */  { addItemToBuffer,             CHR_iotaTON,                 "",                                            STD_iota_TONOS,                                CAT_NONE, SLS_UNCHANGED},
/* 1001 */  { addItemToBuffer,             CHR_iota_DIALYTIKA_TONOS,    "",                                            STD_iota_DIALYTIKA_TONOS,                      CAT_NONE, SLS_UNCHANGED},
/* 1002 */  { addItemToBuffer,             CHR_iota_DIALYTIKA,          "",                                            STD_iota_DIALYTIKA,                            CAT_NONE, SLS_UNCHANGED},
/* 1003 */  { addItemToBuffer,             CHR_kappa,                   "",                                            STD_kappa,                                     CAT_NONE, SLS_UNCHANGED},
/* 1004 */  { addItemToBuffer,             CHR_lambda,                  "",                                            STD_lambda,                                    CAT_NONE, SLS_UNCHANGED},
/* 1005 */  { addItemToBuffer,             CHR_mu,                      "",                                            STD_mu,                                        CAT_NONE, SLS_UNCHANGED},
/* 1006 */  { addItemToBuffer,             CHR_nu,                      "",                                            STD_nu,                                        CAT_NONE, SLS_UNCHANGED},
/* 1007 */  { addItemToBuffer,             CHR_xi,                      "",                                            STD_xi,                                        CAT_NONE, SLS_UNCHANGED},
/* 1008 */  { addItemToBuffer,             CHR_omicron,                 "",                                            STD_omicron,                                   CAT_NONE, SLS_UNCHANGED},
/* 1009 */  { addItemToBuffer,             CHR_omicron_TONOS,           "",                                            STD_omicron_TONOS,                             CAT_NONE, SLS_UNCHANGED},
/* 1010 */  { addItemToBuffer,             CHR_pi,                      "",                                            STD_pi,                                        CAT_NONE, SLS_UNCHANGED},
/* 1011 */  { addItemToBuffer,             CHR_rho,                     "",                                            STD_rho,                                       CAT_NONE, SLS_UNCHANGED},
/* 1012 */  { addItemToBuffer,             CHR_sigma,                   "",                                            STD_sigma,                                     CAT_NONE, SLS_UNCHANGED},
/* 1013 */  { addItemToBuffer,             CHR_sigma_end,               "",                                            STD_sigma_end,                                 CAT_NONE, SLS_UNCHANGED},
/* 1014 */  { addItemToBuffer,             CHR_tau,                     "",                                            STD_tau,                                       CAT_NONE, SLS_UNCHANGED},
/* 1015 */  { addItemToBuffer,             CHR_upsilon,                 "",                                            STD_upsilon,                                   CAT_NONE, SLS_UNCHANGED},
/* 1016 */  { addItemToBuffer,             CHR_upsilon_TONOS,           "",                                            STD_upsilon_TONOS,                             CAT_NONE, SLS_UNCHANGED},
/* 1017 */  { addItemToBuffer,             CHR_upsilon_DIALYTIKA,       "",                                            STD_upsilon_DIALYTIKA,                         CAT_NONE, SLS_UNCHANGED},
/* 1018 */  { addItemToBuffer,             CHR_upsilon_DIALYTIKA_TONOS, "",                                            STD_upsilon_DIALYTIKA_TONOS,                   CAT_NONE, SLS_UNCHANGED},
/* 1019 */  { addItemToBuffer,             CHR_phi,                     "",                                            STD_phi,                                       CAT_NONE, SLS_UNCHANGED},
/* 1020 */  { addItemToBuffer,             CHR_chi,                     "",                                            STD_chi,                                       CAT_NONE, SLS_UNCHANGED},
/* 1021 */  { addItemToBuffer,             CHR_psi,                     "",                                            STD_psi,                                       CAT_NONE, SLS_UNCHANGED},
/* 1022 */  { addItemToBuffer,             CHR_omega,                   "",                                            STD_omega,                                     CAT_NONE, SLS_UNCHANGED},
/* 1023 */  { addItemToBuffer,             CHR_omega_TONOS,             "",                                            STD_omega_TONOS,                               CAT_NONE, SLS_UNCHANGED},
/* 1024 */  { itemToBeCoded,               NOPARAM,                     "1024",                                        "1024",                                        CAT_FREE, SLS_UNCHANGED},
/* 1025 */  { itemToBeCoded,               NOPARAM,                     "1025",                                        "1025",                                        CAT_FREE, SLS_UNCHANGED},
/* 1026 */  { itemToBeCoded,               NOPARAM,                     "1026",                                        "1026",                                        CAT_FREE, SLS_UNCHANGED},
/* 1027 */  { itemToBeCoded,               NOPARAM,                     "1027",                                        "1027",                                        CAT_FREE, SLS_UNCHANGED},
/* 1028 */  { itemToBeCoded,               NOPARAM,                     "1028",                                        "1028",                                        CAT_FREE, SLS_UNCHANGED},
/* 1029 */  { itemToBeCoded,               NOPARAM,                     "1029",                                        "1029",                                        CAT_FREE, SLS_UNCHANGED},
/* 1030 */  { addItemToBuffer,             CHR_A_MACRON,                "",                                            STD_A_MACRON,                                  CAT_NONE, SLS_UNCHANGED},
/* 1031 */  { addItemToBuffer,             CHR_A_ACUTE,                 "",                                            STD_A_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1032 */  { addItemToBuffer,             CHR_A_BREVE,                 "",                                            STD_A_BREVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1033 */  { addItemToBuffer,             CHR_A_GRAVE,                 "",                                            STD_A_GRAVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1034 */  { addItemToBuffer,             CHR_A_DIARESIS,              "",                                            STD_A_DIARESIS,                                CAT_NONE, SLS_UNCHANGED},
/* 1035 */  { addItemToBuffer,             CHR_A_TILDE,                 "",                                            STD_A_TILDE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1036 */  { addItemToBuffer,             CHR_A_CIRC,                  "",                                            STD_A_CIRC,                                    CAT_NONE, SLS_UNCHANGED},
/* 1037 */  { addItemToBuffer,             CHR_A_RING,                  "",                                            STD_A_RING,                                    CAT_NONE, SLS_UNCHANGED},
/* 1038 */  { addItemToBuffer,             CHR_AE,                      "",                                            STD_AE,                                        CAT_NONE, SLS_UNCHANGED},
/* 1039 */  { addItemToBuffer,             CHR_A_OGONEK,                "",                                            STD_A_OGONEK,                                  CAT_NONE, SLS_UNCHANGED},
/* 1040 */  { addItemToBuffer,             CHR_C_ACUTE,                 "",                                            STD_C_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1041 */  { addItemToBuffer,             CHR_C_CARON,                 "",                                            STD_C_CARON,                                   CAT_NONE, SLS_UNCHANGED},
/* 1042 */  { addItemToBuffer,             CHR_C_CEDILLA,               "",                                            STD_C_CEDILLA,                                 CAT_NONE, SLS_UNCHANGED},
/* 1043 */  { addItemToBuffer,             CHR_D_STROKE,                "",                                            STD_D_STROKE,                                  CAT_NONE, SLS_UNCHANGED},
/* 1044 */  { addItemToBuffer,             CHR_D_CARON,                 "",                                            STD_D_CARON,                                   CAT_NONE, SLS_UNCHANGED},
/* 1045 */  { addItemToBuffer,             CHR_E_MACRON,                "",                                            STD_E_MACRON,                                  CAT_NONE, SLS_UNCHANGED},
/* 1046 */  { addItemToBuffer,             CHR_E_ACUTE,                 "",                                            STD_E_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1047 */  { addItemToBuffer,             CHR_E_BREVE,                 "",                                            STD_E_BREVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1048 */  { addItemToBuffer,             CHR_E_GRAVE,                 "",                                            STD_E_GRAVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1049 */  { addItemToBuffer,             CHR_E_DIARESIS,              "",                                            STD_E_DIARESIS,                                CAT_NONE, SLS_UNCHANGED},
/* 1050 */  { addItemToBuffer,             CHR_E_CIRC,                  "",                                            STD_E_CIRC,                                    CAT_NONE, SLS_UNCHANGED},
/* 1051 */  { addItemToBuffer,             CHR_E_OGONEK,                "",                                            STD_E_OGONEK,                                  CAT_NONE, SLS_UNCHANGED},
/* 1052 */  { addItemToBuffer,             CHR_G_BREVE,                 "",                                            STD_G_BREVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1053 */  { itemToBeCoded,               NOPARAM,                     "1053",                                        "1053",                                        CAT_FREE, SLS_UNCHANGED},
/* 1054 */  { addItemToBuffer,             CHR_I_MACRON,                "",                                            STD_I_MACRON,                                  CAT_NONE, SLS_UNCHANGED},
/* 1055 */  { addItemToBuffer,             CHR_I_ACUTE,                 "",                                            STD_I_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1056 */  { addItemToBuffer,             CHR_I_BREVE,                 "",                                            STD_I_BREVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1057 */  { addItemToBuffer,             CHR_I_GRAVE,                 "",                                            STD_I_GRAVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1058 */  { addItemToBuffer,             CHR_I_DIARESIS,              "",                                            STD_I_DIARESIS,                                CAT_NONE, SLS_UNCHANGED},
/* 1059 */  { addItemToBuffer,             CHR_I_CIRC,                  "",                                            STD_I_CIRC,                                    CAT_NONE, SLS_UNCHANGED},
/* 1060 */  { addItemToBuffer,             CHR_I_OGONEK,                "",                                            STD_I_OGONEK,                                  CAT_NONE, SLS_UNCHANGED},
/* 1061 */  { addItemToBuffer,             CHR_I_DOT,                   "",                                            STD_I_DOT,                                     CAT_NONE, SLS_UNCHANGED},
/* 1062 */  { addItemToBuffer,             CHR_I_DOTLESS,               "",                                            "I",                                           CAT_NONE, SLS_UNCHANGED},
/* 1063 */  { addItemToBuffer,             CHR_L_STROKE,                "",                                            STD_L_STROKE,                                  CAT_NONE, SLS_UNCHANGED},
/* 1064 */  { addItemToBuffer,             CHR_L_ACUTE,                 "",                                            STD_L_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1065 */  { addItemToBuffer,             CHR_L_APOSTROPHE,            "",                                            STD_L_APOSTROPHE,                              CAT_NONE, SLS_UNCHANGED},
/* 1066 */  { addItemToBuffer,             CHR_N_ACUTE,                 "",                                            STD_N_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1067 */  { addItemToBuffer,             CHR_N_CARON,                 "",                                            STD_N_CARON,                                   CAT_NONE, SLS_UNCHANGED},
/* 1068 */  { addItemToBuffer,             CHR_N_TILDE,                 "",                                            STD_N_TILDE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1069 */  { addItemToBuffer,             CHR_O_MACRON,                "",                                            STD_O_MACRON,                                  CAT_NONE, SLS_UNCHANGED},
/* 1070 */  { addItemToBuffer,             CHR_O_ACUTE,                 "",                                            STD_O_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1071 */  { addItemToBuffer,             CHR_O_BREVE,                 "",                                            STD_O_BREVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1072 */  { addItemToBuffer,             CHR_O_GRAVE,                 "",                                            STD_O_GRAVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1073 */  { addItemToBuffer,             CHR_O_DIARESIS,              "",                                            STD_O_DIARESIS,                                CAT_NONE, SLS_UNCHANGED},
/* 1074 */  { addItemToBuffer,             CHR_O_TILDE,                 "",                                            STD_O_TILDE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1075 */  { addItemToBuffer,             CHR_O_CIRC,                  "",                                            STD_O_CIRC,                                    CAT_NONE, SLS_UNCHANGED},
/* 1076 */  { addItemToBuffer,             CHR_O_STROKE,                "",                                            STD_O_STROKE,                                  CAT_NONE, SLS_UNCHANGED},
/* 1077 */  { addItemToBuffer,             CHR_OE,                      "",                                            STD_OE,                                        CAT_NONE, SLS_UNCHANGED},
/* 1078 */  { itemToBeCoded,               NOPARAM,                     "1078",                                        "1078",                                        CAT_FREE, SLS_UNCHANGED},
/* 1079 */  { itemToBeCoded,               NOPARAM,                     "1079",                                        "1079",                                        CAT_FREE, SLS_UNCHANGED},
/* 1080 */  { itemToBeCoded,               NOPARAM,                     "1080",                                        "1080",                                        CAT_FREE, SLS_UNCHANGED},
/* 1081 */  { addItemToBuffer,             CHR_S_ACUTE,                 "",                                            STD_S_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1082 */  { addItemToBuffer,             CHR_S_CARON,                 "",                                            STD_S_CARON,                                   CAT_NONE, SLS_UNCHANGED},
/* 1083 */  { addItemToBuffer,             CHR_S_CEDILLA,               "",                                            STD_S_CEDILLA,                                 CAT_NONE, SLS_UNCHANGED},
/* 1084 */  { addItemToBuffer,             CHR_T_CARON,                 "",                                            STD_T_CARON,                                   CAT_NONE, SLS_UNCHANGED},
/* 1085 */  { addItemToBuffer,             CHR_T_CEDILLA,               "",                                            STD_T_CEDILLA,                                 CAT_NONE, SLS_UNCHANGED},
/* 1086 */  { addItemToBuffer,             CHR_U_MACRON,                "",                                            STD_U_MACRON,                                  CAT_NONE, SLS_UNCHANGED},
/* 1087 */  { addItemToBuffer,             CHR_U_ACUTE,                 "",                                            STD_U_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1088 */  { addItemToBuffer,             CHR_U_BREVE,                 "",                                            STD_U_BREVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1089 */  { addItemToBuffer,             CHR_U_GRAVE,                 "",                                            STD_U_GRAVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1090 */  { addItemToBuffer,             CHR_U_DIARESIS,              "",                                            STD_U_DIARESIS,                                CAT_NONE, SLS_UNCHANGED},
/* 1091 */  { addItemToBuffer,             CHR_U_TILDE,                 "",                                            STD_U_TILDE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1092 */  { addItemToBuffer,             CHR_U_CIRC,                  "",                                            STD_U_CIRC,                                    CAT_NONE, SLS_UNCHANGED},
/* 1093 */  { addItemToBuffer,             CHR_U_RING,                  "",                                            STD_U_RING,                                    CAT_NONE, SLS_UNCHANGED},
/* 1094 */  { addItemToBuffer,             CHR_W_CIRC,                  "",                                            STD_W_CIRC,                                    CAT_NONE, SLS_UNCHANGED},
/* 1095 */  { itemToBeCoded,               NOPARAM,                     "1095",                                        "1095",                                        CAT_FREE, SLS_UNCHANGED},
/* 1096 */  { itemToBeCoded,               NOPARAM,                     "1096",                                        "1096",                                        CAT_FREE, SLS_UNCHANGED},
/* 1097 */  { itemToBeCoded,               NOPARAM,                     "1097",                                        "1097",                                        CAT_FREE, SLS_UNCHANGED},
/* 1098 */  { addItemToBuffer,             CHR_Y_CIRC,                  "",                                            STD_Y_CIRC,                                    CAT_NONE, SLS_UNCHANGED},
/* 1099 */  { addItemToBuffer,             CHR_Y_ACUTE,                 "",                                            STD_Y_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1100 */  { addItemToBuffer,             CHR_Y_DIARESIS,              "",                                            STD_Y_DIARESIS,                                CAT_NONE, SLS_UNCHANGED},
/* 1101 */  { addItemToBuffer,             CHR_Z_ACUTE,                 "",                                            STD_Z_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1102 */  { addItemToBuffer,             CHR_Z_CARON,                 "",                                            STD_Z_CARON,                                   CAT_NONE, SLS_UNCHANGED},
/* 1103 */  { addItemToBuffer,             CHR_Z_DOT,                   "",                                            STD_Z_DOT,                                     CAT_NONE, SLS_UNCHANGED},
/* 1104 */  { itemToBeCoded,               NOPARAM,                     "1104",                                        "1104",                                        CAT_FREE, SLS_UNCHANGED},
/* 1105 */  { itemToBeCoded,               NOPARAM,                     "1105",                                        "1105",                                        CAT_FREE, SLS_UNCHANGED},
/* 1106 */  { itemToBeCoded,               NOPARAM,                     "1106",                                        "1106",                                        CAT_FREE, SLS_UNCHANGED},
/* 1107 */  { itemToBeCoded,               NOPARAM,                     "1107",                                        "1107",                                        CAT_FREE, SLS_UNCHANGED},
/* 1108 */  { itemToBeCoded,               NOPARAM,                     "1108",                                        "1108",                                        CAT_FREE, SLS_UNCHANGED},
/* 1109 */  { itemToBeCoded,               NOPARAM,                     "1109",                                        "1109",                                        CAT_FREE, SLS_UNCHANGED},
/* 1110 */  { addItemToBuffer,             CHR_a_MACRON,                "",                                            STD_a_MACRON,                                  CAT_NONE, SLS_UNCHANGED},
/* 1111 */  { addItemToBuffer,             CHR_a_ACUTE,                 "",                                            STD_a_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1112 */  { addItemToBuffer,             CHR_a_BREVE,                 "",                                            STD_a_BREVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1113 */  { addItemToBuffer,             CHR_a_GRAVE,                 "",                                            STD_a_GRAVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1114 */  { addItemToBuffer,             CHR_a_DIARESIS,              "",                                            STD_a_DIARESIS,                                CAT_NONE, SLS_UNCHANGED},
/* 1115 */  { addItemToBuffer,             CHR_a_TILDE,                 "",                                            STD_a_TILDE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1116 */  { addItemToBuffer,             CHR_a_CIRC,                  "",                                            STD_a_CIRC,                                    CAT_NONE, SLS_UNCHANGED},
/* 1117 */  { addItemToBuffer,             CHR_a_RING,                  "",                                            STD_a_RING,                                    CAT_NONE, SLS_UNCHANGED},
/* 1118 */  { addItemToBuffer,             CHR_ae,                      "",                                            STD_ae,                                        CAT_NONE, SLS_UNCHANGED},
/* 1119 */  { addItemToBuffer,             CHR_a_OGONEK,                "",                                            STD_a_OGONEK,                                  CAT_NONE, SLS_UNCHANGED},
/* 1120 */  { addItemToBuffer,             CHR_c_ACUTE,                 "",                                            STD_c_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1121 */  { addItemToBuffer,             CHR_c_CARON,                 "",                                            STD_c_CARON,                                   CAT_NONE, SLS_UNCHANGED},
/* 1122 */  { addItemToBuffer,             CHR_c_CEDILLA,               "",                                            STD_c_CEDILLA,                                 CAT_NONE, SLS_UNCHANGED},
/* 1123 */  { addItemToBuffer,             CHR_d_STROKE,                "",                                            STD_d_STROKE,                                  CAT_NONE, SLS_UNCHANGED},
/* 1124 */  { addItemToBuffer,             CHR_d_APOSTROPHE,            "",                                            STD_d_APOSTROPHE,                              CAT_NONE, SLS_UNCHANGED},
/* 1125 */  { addItemToBuffer,             CHR_e_MACRON,                "",                                            STD_e_MACRON,                                  CAT_NONE, SLS_UNCHANGED},
/* 1126 */  { addItemToBuffer,             CHR_e_ACUTE,                 "",                                            STD_e_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1127 */  { addItemToBuffer,             CHR_e_BREVE,                 "",                                            STD_e_BREVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1128 */  { addItemToBuffer,             CHR_e_GRAVE,                 "",                                            STD_e_GRAVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1129 */  { addItemToBuffer,             CHR_e_DIARESIS,              "",                                            STD_e_DIARESIS,                                CAT_NONE, SLS_UNCHANGED},
/* 1130 */  { addItemToBuffer,             CHR_e_CIRC,                  "",                                            STD_e_CIRC,                                    CAT_NONE, SLS_UNCHANGED},
/* 1131 */  { addItemToBuffer,             CHR_e_OGONEK,                "",                                            STD_e_OGONEK,                                  CAT_NONE, SLS_UNCHANGED},
/* 1132 */  { addItemToBuffer,             CHR_g_BREVE,                 "",                                            STD_g_BREVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1133 */  { addItemToBuffer,             CHR_h_STROKE,                "",                                            STD_h_STROKE,                                  CAT_NONE, SLS_UNCHANGED},
/* 1134 */  { addItemToBuffer,             CHR_i_MACRON,                "",                                            STD_i_MACRON,                                  CAT_NONE, SLS_UNCHANGED},
/* 1135 */  { addItemToBuffer,             CHR_i_ACUTE,                 "",                                            STD_i_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1136 */  { addItemToBuffer,             CHR_i_BREVE,                 "",                                            STD_i_BREVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1137 */  { addItemToBuffer,             CHR_i_GRAVE,                 "",                                            STD_i_GRAVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1138 */  { addItemToBuffer,             CHR_i_DIARESIS,              "",                                            STD_i_DIARESIS,                                CAT_NONE, SLS_UNCHANGED},
/* 1139 */  { addItemToBuffer,             CHR_i_CIRC,                  "",                                            STD_i_CIRC,                                    CAT_NONE, SLS_UNCHANGED},
/* 1140 */  { addItemToBuffer,             CHR_i_OGONEK,                "",                                            STD_i_OGONEK,                                  CAT_NONE, SLS_UNCHANGED},
/* 1141 */  { addItemToBuffer,             CHR_i_DOT,                   "",                                            "i",                                           CAT_NONE, SLS_UNCHANGED},
/* 1142 */  { addItemToBuffer,             CHR_i_DOTLESS,               "",                                            STD_i_DOTLESS,                                 CAT_NONE, SLS_UNCHANGED},
/* 1143 */  { addItemToBuffer,             CHR_l_STROKE,                "",                                            STD_l_STROKE,                                  CAT_NONE, SLS_UNCHANGED},
/* 1144 */  { addItemToBuffer,             CHR_l_ACUTE,                 "",                                            STD_l_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1145 */  { addItemToBuffer,             CHR_l_APOSTROPHE,            "",                                            STD_l_APOSTROPHE,                              CAT_NONE, SLS_UNCHANGED},
/* 1146 */  { addItemToBuffer,             CHR_n_ACUTE,                 "",                                            STD_n_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1147 */  { addItemToBuffer,             CHR_n_CARON,                 "",                                            STD_n_CARON,                                   CAT_NONE, SLS_UNCHANGED},
/* 1148 */  { addItemToBuffer,             CHR_n_TILDE,                 "",                                            STD_n_TILDE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1149 */  { addItemToBuffer,             CHR_o_MACRON,                "",                                            STD_o_MACRON,                                  CAT_NONE, SLS_UNCHANGED},
/* 1150 */  { addItemToBuffer,             CHR_o_ACUTE,                 "",                                            STD_o_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1151 */  { addItemToBuffer,             CHR_o_BREVE,                 "",                                            STD_o_BREVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1152 */  { addItemToBuffer,             CHR_o_GRAVE,                 "",                                            STD_o_GRAVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1153 */  { addItemToBuffer,             CHR_o_DIARESIS,              "",                                            STD_o_DIARESIS,                                CAT_NONE, SLS_UNCHANGED},
/* 1154 */  { addItemToBuffer,             CHR_o_TILDE,                 "",                                            STD_o_TILDE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1155 */  { addItemToBuffer,             CHR_o_CIRC,                  "",                                            STD_o_CIRC,                                    CAT_NONE, SLS_UNCHANGED},
/* 1156 */  { addItemToBuffer,             CHR_o_STROKE,                "",                                            STD_o_STROKE,                                  CAT_NONE, SLS_UNCHANGED},
/* 1157 */  { addItemToBuffer,             CHR_oe,                      "",                                            STD_oe,                                        CAT_NONE, SLS_UNCHANGED},
/* 1158 */  { addItemToBuffer,             CHR_r_CARON,                 "",                                            STD_r_CARON,                                   CAT_NONE, SLS_UNCHANGED},
/* 1159 */  { addItemToBuffer,             CHR_r_ACUTE,                 "",                                            STD_r_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1160 */  { addItemToBuffer,             CHR_s_SHARP,                 "",                                            STD_s_SHARP,                                   CAT_NONE, SLS_UNCHANGED},
/* 1161 */  { addItemToBuffer,             CHR_s_ACUTE,                 "",                                            STD_s_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1162 */  { addItemToBuffer,             CHR_s_CARON,                 "",                                            STD_s_CARON,                                   CAT_NONE, SLS_UNCHANGED},
/* 1163 */  { addItemToBuffer,             CHR_s_CEDILLA,               "",                                            STD_s_CEDILLA,                                 CAT_NONE, SLS_UNCHANGED},
/* 1164 */  { addItemToBuffer,             CHR_t_APOSTROPHE,            "",                                            STD_t_APOSTROPHE,                              CAT_NONE, SLS_UNCHANGED},
/* 1165 */  { addItemToBuffer,             CHR_t_CEDILLA,               "",                                            STD_t_CEDILLA,                                 CAT_NONE, SLS_UNCHANGED},
/* 1166 */  { addItemToBuffer,             CHR_u_MACRON,                "",                                            STD_u_MACRON,                                  CAT_NONE, SLS_UNCHANGED},
/* 1167 */  { addItemToBuffer,             CHR_u_ACUTE,                 "",                                            STD_u_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1168 */  { addItemToBuffer,             CHR_u_BREVE,                 "",                                            STD_u_BREVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1169 */  { addItemToBuffer,             CHR_u_GRAVE,                 "",                                            STD_u_GRAVE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1170 */  { addItemToBuffer,             CHR_u_DIARESIS,              "",                                            STD_u_DIARESIS,                                CAT_NONE, SLS_UNCHANGED},
/* 1171 */  { addItemToBuffer,             CHR_u_TILDE,                 "",                                            STD_u_TILDE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1172 */  { addItemToBuffer,             CHR_u_CIRC,                  "",                                            STD_u_CIRC,                                    CAT_NONE, SLS_UNCHANGED},
/* 1173 */  { addItemToBuffer,             CHR_u_RING,                  "",                                            STD_u_RING,                                    CAT_NONE, SLS_UNCHANGED},
/* 1174 */  { addItemToBuffer,             CHR_w_CIRC,                  "",                                            STD_w_CIRC,                                    CAT_NONE, SLS_UNCHANGED},
/* 1175 */  { addItemToBuffer,             CHR_x_BAR,                   "",                                            STD_x_BAR,                                     CAT_NONE, SLS_UNCHANGED},
/* 1176 */  { addItemToBuffer,             CHR_x_CIRC,                  "",                                            STD_x_CIRC,                                    CAT_NONE, SLS_UNCHANGED},
/* 1177 */  { addItemToBuffer,             CHR_y_BAR,                   "",                                            STD_y_BAR,                                     CAT_NONE, SLS_UNCHANGED},
/* 1178 */  { addItemToBuffer,             CHR_y_CIRC,                  "",                                            STD_y_CIRC,                                    CAT_NONE, SLS_UNCHANGED},
/* 1179 */  { addItemToBuffer,             CHR_y_ACUTE,                 "",                                            STD_y_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1180 */  { addItemToBuffer,             CHR_y_DIARESIS,              "",                                            STD_y_DIARESIS,                                CAT_NONE, SLS_UNCHANGED},
/* 1181 */  { addItemToBuffer,             CHR_z_ACUTE,                 "",                                            STD_z_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1182 */  { addItemToBuffer,             CHR_z_CARON,                 "",                                            STD_z_CARON,                                   CAT_NONE, SLS_UNCHANGED},
/* 1183 */  { addItemToBuffer,             CHR_z_DOT,                   "",                                            STD_z_DOT,                                     CAT_NONE, SLS_UNCHANGED},
/* 1184 */  { itemToBeCoded,               NOPARAM,                     "1184",                                        "1184",                                        CAT_FREE, SLS_UNCHANGED},
/* 1185 */  { itemToBeCoded,               NOPARAM,                     "1185",                                        "1185",                                        CAT_FREE, SLS_UNCHANGED},
/* 1186 */  { itemToBeCoded,               NOPARAM,                     "1186",                                        "1186",                                        CAT_FREE, SLS_UNCHANGED},
/* 1187 */  { itemToBeCoded,               NOPARAM,                     "1187",                                        "1187",                                        CAT_FREE, SLS_UNCHANGED},
/* 1188 */  { itemToBeCoded,               NOPARAM,                     "1188",                                        "1188",                                        CAT_FREE, SLS_UNCHANGED},
/* 1189 */  { itemToBeCoded,               NOPARAM,                     "1189",                                        "1189",                                        CAT_FREE, SLS_UNCHANGED},
/* 1190 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_alpha,                                 CAT_NONE, SLS_UNCHANGED},
/* 1191 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_delta,                                 CAT_NONE, SLS_UNCHANGED},
/* 1192 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_mu,                                    CAT_NONE, SLS_UNCHANGED},
/* 1193 */  { addItemToBuffer,             CHR_SUB_SUN,                 "",                                            STD_SUB_SUN,                                   CAT_NONE, SLS_UNCHANGED},
/* 1194 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_SUN_b,                                 CAT_NONE, SLS_UNCHANGED},
/* 1195 */  { addItemToBuffer,             CHR_SUB_EARTH,               "",                                            STD_SUB_EARTH,                                 CAT_NONE, SLS_UNCHANGED},
/* 1196 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_EARTH_b,                               CAT_NONE, SLS_UNCHANGED},
/* 1197 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_PLUS,                                  CAT_NONE, SLS_UNCHANGED},
/* 1198 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_MINUS,                                 CAT_NONE, SLS_UNCHANGED},
/* 1199 */  { addItemToBuffer,             CHR_SUB_INFINITY,            "",                                            STD_SUB_INFINITY,                              CAT_NONE, SLS_UNCHANGED},
/* 1200 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_0,                                     CAT_NONE, SLS_UNCHANGED},
/* 1201 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_1,                                     CAT_NONE, SLS_UNCHANGED},
/* 1202 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_2,                                     CAT_NONE, SLS_UNCHANGED},
/* 1203 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_3,                                     CAT_NONE, SLS_UNCHANGED},
/* 1204 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_4,                                     CAT_NONE, SLS_UNCHANGED},
/* 1205 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_5,                                     CAT_NONE, SLS_UNCHANGED},
/* 1206 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_6,                                     CAT_NONE, SLS_UNCHANGED},
/* 1207 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_7,                                     CAT_NONE, SLS_UNCHANGED},
/* 1208 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_8,                                     CAT_NONE, SLS_UNCHANGED},
/* 1209 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_9,                                     CAT_NONE, SLS_UNCHANGED},
/* 1210 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_10,                                    CAT_NONE, SLS_UNCHANGED},
/* 1211 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_A,                                     CAT_NONE, SLS_UNCHANGED},
/* 1212 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_B,                                     CAT_NONE, SLS_UNCHANGED},
/* 1213 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_C,                                     CAT_NONE, SLS_UNCHANGED},
/* 1214 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_D,                                     CAT_NONE, SLS_UNCHANGED},
/* 1215 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_E,                                     CAT_NONE, SLS_UNCHANGED},
/* 1216 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_F,                                     CAT_NONE, SLS_UNCHANGED},
/* 1217 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_G,                                     CAT_NONE, SLS_UNCHANGED},
/* 1218 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_H,                                     CAT_NONE, SLS_UNCHANGED},
/* 1219 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_I,                                     CAT_NONE, SLS_UNCHANGED},
/* 1220 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_J,                                     CAT_NONE, SLS_UNCHANGED},
/* 1221 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_K,                                     CAT_NONE, SLS_UNCHANGED},
/* 1222 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_L,                                     CAT_NONE, SLS_UNCHANGED},
/* 1223 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_M,                                     CAT_NONE, SLS_UNCHANGED},
/* 1224 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_N,                                     CAT_NONE, SLS_UNCHANGED},
/* 1225 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_O,                                     CAT_NONE, SLS_UNCHANGED},
/* 1226 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_P,                                     CAT_NONE, SLS_UNCHANGED},
/* 1227 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_Q,                                     CAT_NONE, SLS_UNCHANGED},
/* 1228 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_R,                                     CAT_NONE, SLS_UNCHANGED},
/* 1229 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_S,                                     CAT_NONE, SLS_UNCHANGED},
/* 1230 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_T,                                     CAT_NONE, SLS_UNCHANGED},
/* 1231 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_U,                                     CAT_NONE, SLS_UNCHANGED},
/* 1232 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_V,                                     CAT_NONE, SLS_UNCHANGED},
/* 1233 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_W,                                     CAT_NONE, SLS_UNCHANGED},
/* 1234 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_X,                                     CAT_NONE, SLS_UNCHANGED},
/* 1235 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_Y,                                     CAT_NONE, SLS_UNCHANGED},
/* 1236 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_Z,                                     CAT_NONE, SLS_UNCHANGED},
/* 1237 */  { addItemToBuffer,             CHR_SUB_E_OUTLINE,           "",                                            STD_SUB_E_OUTLINE,                             CAT_NONE, SLS_UNCHANGED},
/* 1238 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_a,                                     CAT_NONE, SLS_UNCHANGED},
/* 1239 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_b,                                     CAT_NONE, SLS_UNCHANGED},
/* 1240 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_c,                                     CAT_NONE, SLS_UNCHANGED},
/* 1241 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_d,                                     CAT_NONE, SLS_UNCHANGED},
/* 1242 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_e,                                     CAT_NONE, SLS_UNCHANGED},
/* 1243 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_h,                                     CAT_NONE, SLS_UNCHANGED},
/* 1244 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_i,                                     CAT_NONE, SLS_UNCHANGED},
/* 1245 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_j,                                     CAT_NONE, SLS_UNCHANGED},
/* 1246 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_k,                                     CAT_NONE, SLS_UNCHANGED},
/* 1247 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_l,                                     CAT_NONE, SLS_UNCHANGED},
/* 1248 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_m,                                     CAT_NONE, SLS_UNCHANGED},
/* 1249 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_n,                                     CAT_NONE, SLS_UNCHANGED},
/* 1250 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_o,                                     CAT_NONE, SLS_UNCHANGED},
/* 1251 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_p,                                     CAT_NONE, SLS_UNCHANGED},
/* 1252 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_q,                                     CAT_NONE, SLS_UNCHANGED},
/* 1253 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_s,                                     CAT_NONE, SLS_UNCHANGED},
/* 1254 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_t,                                     CAT_NONE, SLS_UNCHANGED},
/* 1255 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_u,                                     CAT_NONE, SLS_UNCHANGED},
/* 1256 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_v,                                     CAT_NONE, SLS_UNCHANGED},
/* 1257 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_w,                                     CAT_NONE, SLS_UNCHANGED},
/* 1258 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_x,                                     CAT_NONE, SLS_UNCHANGED},
/* 1259 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_y,                                     CAT_NONE, SLS_UNCHANGED},
/* 1260 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_z,                                     CAT_NONE, SLS_UNCHANGED},
/* 1261 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_a_b,                                   CAT_NONE, SLS_UNCHANGED},
/* 1262 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_e_b,                                   CAT_NONE, SLS_UNCHANGED},
/* 1263 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_k_b,                                   CAT_NONE, SLS_UNCHANGED},
/* 1264 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_l_b,                                   CAT_NONE, SLS_UNCHANGED},
/* 1265 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_m_b,                                   CAT_NONE, SLS_UNCHANGED},
/* 1266 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_n_b,                                   CAT_NONE, SLS_UNCHANGED},
/* 1267 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_o_b,                                   CAT_NONE, SLS_UNCHANGED},
/* 1268 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_p_b,                                   CAT_NONE, SLS_UNCHANGED},
/* 1269 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_s_b,                                   CAT_NONE, SLS_UNCHANGED},
/* 1270 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_u_b,                                   CAT_NONE, SLS_UNCHANGED},
/* 1271 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_x_b,                                   CAT_NONE, SLS_UNCHANGED},
/* 1272 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_PLUS,                                  CAT_NONE, SLS_UNCHANGED},
/* 1273 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_MINUS,                                 CAT_NONE, SLS_UNCHANGED},
/* 1274 */  { addItemToBuffer,             CHR_SUP_MINUS_1,             "",                                            STD_SUP_MINUS_1,                               CAT_NONE, SLS_UNCHANGED},
/* 1275 */  { addItemToBuffer,             CHR_SUP_INFINITY,            "",                                            STD_SUP_INFINITY,                              CAT_NONE, SLS_UNCHANGED},
/* 1276 */  { addItemToBuffer,             CHR_SUP_ASTERISK,            "",                                            STD_SUP_ASTERISK,                              CAT_NONE, SLS_UNCHANGED},
/* 1277 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_0,                                     CAT_NONE, SLS_UNCHANGED},
/* 1278 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_1,                                     CAT_NONE, SLS_UNCHANGED},
/* 1279 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_2,                                     CAT_NONE, SLS_UNCHANGED},
/* 1280 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_3,                                     CAT_NONE, SLS_UNCHANGED},
/* 1281 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_4,                                     CAT_NONE, SLS_UNCHANGED},
/* 1282 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_5,                                     CAT_NONE, SLS_UNCHANGED},
/* 1283 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_6,                                     CAT_NONE, SLS_UNCHANGED},
/* 1284 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_7,                                     CAT_NONE, SLS_UNCHANGED},
/* 1285 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_8,                                     CAT_NONE, SLS_UNCHANGED},
/* 1286 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_9,                                     CAT_NONE, SLS_UNCHANGED},
/* 1287 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_1_b,                                   CAT_NONE, SLS_UNCHANGED},
/* 1288 */  { addItemToBuffer,             CHR_SUP_T,                   "",                                            STD_SUP_T,                                     CAT_NONE, SLS_UNCHANGED},
/* 1289 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_a,                                     CAT_NONE, SLS_UNCHANGED},
/* 1290 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_f,                                     CAT_NONE, SLS_UNCHANGED},
/* 1291 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_g,                                     CAT_NONE, SLS_UNCHANGED},
/* 1292 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_h,                                     CAT_NONE, SLS_UNCHANGED},
/* 1293 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_r,                                     CAT_NONE, SLS_UNCHANGED},
/* 1294 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_x,                                     CAT_NONE, SLS_UNCHANGED},
/* 1295 */  { addItemToBuffer,             CHR_SPACE,                   "",                                            STD_SPACE,                                     CAT_NONE, SLS_UNCHANGED},
/* 1296 */  { addItemToBuffer,             CHR_EXCLAMATION_MARK,        "",                                            STD_EXCLAMATION_MARK,                          CAT_NONE, SLS_UNCHANGED},
/* 1297 */  { addItemToBuffer,             CHR_DOUBLE_QUOTE,            "",                                            STD_DOUBLE_QUOTE,                              CAT_NONE, SLS_UNCHANGED},
/* 1298 */  { addItemToBuffer,             CHR_NUMBER_SIGN,             "",                                            STD_NUMBER_SIGN,                               CAT_NONE, SLS_UNCHANGED},
/* 1299 */  { addItemToBuffer,             CHR_DOLLAR,                  "",                                            STD_DOLLAR,                                    CAT_NONE, SLS_UNCHANGED},
/* 1300 */  { addItemToBuffer,             CHR_PERCENT,                 "",                                            STD_PERCENT,                                   CAT_NONE, SLS_UNCHANGED},
/* 1301 */  { addItemToBuffer,             CHR_AMPERSAND,               "",                                            STD_AMPERSAND,                                 CAT_NONE, SLS_UNCHANGED},
/* 1302 */  { addItemToBuffer,             CHR_QUOTE,                   "",                                            STD_QUOTE,                                     CAT_NONE, SLS_UNCHANGED},
/* 1303 */  { addItemToBuffer,             CHR_LEFT_PARENTHESIS,        "",                                            STD_LEFT_PARENTHESIS,                          CAT_NONE, SLS_UNCHANGED},
/* 1304 */  { addItemToBuffer,             CHR_RIGHT_PARENTHESIS,       "",                                            STD_RIGHT_PARENTHESIS,                         CAT_NONE, SLS_UNCHANGED},
/* 1305 */  { addItemToBuffer,             CHR_ASTERISK,                "",                                            STD_ASTERISK,                                  CAT_NONE, SLS_UNCHANGED},
/* 1306 */  { itemToBeCoded,               NOPARAM,                     "1306",                                        "1306",                                        CAT_FREE, SLS_UNCHANGED},
/* 1307 */  { addItemToBuffer,             CHR_PLUS,                    "",                                            STD_PLUS,                                      CAT_NONE, SLS_UNCHANGED},
/* 1308 */  { addItemToBuffer,             CHR_COMMA,                   "",                                            STD_COMMA,                                     CAT_NONE, SLS_UNCHANGED},
/* 1309 */  { addItemToBuffer,             CHR_MINUS,                   "",                                            STD_MINUS,                                     CAT_NONE, SLS_UNCHANGED},
/* 1310 */  { addItemToBuffer,             CHR_PERIOD,                  "",                                            STD_PERIOD,                                    CAT_NONE, SLS_UNCHANGED},
/* 1311 */  { addItemToBuffer,             CHR_SLASH,                   "",                                            STD_SLASH,                                     CAT_NONE, SLS_UNCHANGED},
/* 1312 */  { addItemToBuffer,             CHR_COLON,                   "",                                            STD_COLON,                                     CAT_NONE, SLS_UNCHANGED},
/* 1313 */  { addItemToBuffer,             CHR_SEMICOLON,               "",                                            STD_SEMICOLON,                                 CAT_NONE, SLS_UNCHANGED},
/* 1314 */  { addItemToBuffer,             CHR_LESS_THAN,               "",                                            STD_LESS_THAN,                                 CAT_NONE, SLS_UNCHANGED},
/* 1315 */  { addItemToBuffer,             CHR_EQUAL,                   "",                                            STD_EQUAL,                                     CAT_NONE, SLS_UNCHANGED},
/* 1316 */  { addItemToBuffer,             CHR_GREATER_THAN,            "",                                            STD_GREATER_THAN,                              CAT_NONE, SLS_UNCHANGED},
/* 1317 */  { addItemToBuffer,             CHR_QUESTION_MARK,           "",                                            STD_QUESTION_MARK,                             CAT_NONE, SLS_UNCHANGED},
/* 1318 */  { addItemToBuffer,             CHR_AT,                      "",                                            STD_AT,                                        CAT_NONE, SLS_UNCHANGED},
/* 1319 */  { addItemToBuffer,             CHR_LEFT_SQUARE_BRACKET,     "",                                            STD_LEFT_SQUARE_BRACKET,                       CAT_NONE, SLS_UNCHANGED},
/* 1320 */  { addItemToBuffer,             CHR_BACK_SLASH,              "",                                            STD_BACK_SLASH,                                CAT_NONE, SLS_UNCHANGED},
/* 1321 */  { addItemToBuffer,             CHR_RIGHT_SQUARE_BRACKET,    "",                                            STD_RIGHT_SQUARE_BRACKET,                      CAT_NONE, SLS_UNCHANGED},
/* 1322 */  { addItemToBuffer,             CHR_CIRCUMFLEX,              "",                                            STD_CIRCUMFLEX,                                CAT_NONE, SLS_UNCHANGED},
/* 1323 */  { addItemToBuffer,             CHR_UNDERSCORE,              "",                                            STD_UNDERSCORE,                                CAT_NONE, SLS_UNCHANGED},
/* 1324 */  { addItemToBuffer,             CHR_LEFT_CURLY_BRACKET,      "",                                            STD_LEFT_CURLY_BRACKET,                        CAT_NONE, SLS_UNCHANGED},
/* 1325 */  { addItemToBuffer,             CHR_PIPE,                    "",                                            STD_PIPE,                                      CAT_NONE, SLS_UNCHANGED},
/* 1326 */  { addItemToBuffer,             CHR_RIGHT_CURLY_BRACKET,     "",                                            STD_RIGHT_CURLY_BRACKET,                       CAT_NONE, SLS_UNCHANGED},
/* 1327 */  { addItemToBuffer,             CHR_TILDE,                   "",                                            STD_TILDE,                                     CAT_NONE, SLS_UNCHANGED},
/* 1328 */  { addItemToBuffer,             CHR_INVERTED_EXCLAMATION_MARK, "",                                          STD_INVERTED_EXCLAMATION_MARK,                 CAT_NONE, SLS_UNCHANGED},
/* 1329 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_CENT,                                      CAT_NONE, SLS_UNCHANGED},
/* 1330 */  { addItemToBuffer,             CHR_POUND,                   "",                                            STD_POUND,                                     CAT_NONE, SLS_UNCHANGED},
/* 1331 */  { addItemToBuffer,             CHR_YEN,                     "",                                            STD_YEN,                                       CAT_NONE, SLS_UNCHANGED},
/* 1332 */  { addItemToBuffer,             CHR_SECTION,                 "",                                            STD_SECTION,                                   CAT_NONE, SLS_UNCHANGED},
/* 1333 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OVERFLOW_CARRY,                            CAT_NONE, SLS_UNCHANGED},
/* 1334 */  { addItemToBuffer,             CHR_LEFT_DOUBLE_ANGLE,       "",                                            STD_LEFT_DOUBLE_ANGLE,                         CAT_NONE, SLS_UNCHANGED},
/* 1335 */  { addItemToBuffer,             CHR_NOT,                     "",                                            STD_NOT,                                       CAT_NONE, SLS_UNCHANGED},
/* 1336 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DEGREE,                                    CAT_NONE, SLS_UNCHANGED},
/* 1337 */  { addItemToBuffer,             CHR_PLUS_MINUS,              "",                                            STD_PLUS_MINUS,                                CAT_NONE, SLS_UNCHANGED},
/* 1338 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_mu_b,                                      CAT_NONE, SLS_UNCHANGED},
/* 1339 */  { addItemToBuffer,             CHR_DOT,                     "",                                            STD_DOT,                                       CAT_NONE, SLS_UNCHANGED},
/* 1340 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ORDINAL,                                   CAT_NONE, SLS_UNCHANGED},
/* 1341 */  { addItemToBuffer,             CHR_RIGHT_DOUBLE_ANGLE,      "",                                            STD_RIGHT_DOUBLE_ANGLE,                        CAT_NONE, SLS_UNCHANGED},
/* 1342 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ONE_HALF,                                  CAT_NONE, SLS_UNCHANGED},
/* 1343 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ONE_QUARTER,                               CAT_NONE, SLS_UNCHANGED},
/* 1344 */  { addItemToBuffer,             CHR_INVERTED_QUESTION_MARK,  "",                                            STD_INVERTED_QUESTION_MARK,                    CAT_NONE, SLS_UNCHANGED},
/* 1345 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ETH,                                       CAT_NONE, SLS_UNCHANGED},
/* 1346 */  { addItemToBuffer,             CHR_CROSS,                   "",                                            STD_CROSS,                                     CAT_NONE, SLS_UNCHANGED},
/* 1347 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_eth,                                       CAT_NONE, SLS_UNCHANGED},
/* 1348 */  { addItemToBuffer,             CHR_DIVIDE,                  "",                                            STD_DIVIDE,                                    CAT_NONE, SLS_UNCHANGED},
/* 1349 */  { addItemToBuffer,             CHR_E_DOT,                   "",                                            STD_E_DOT,                                     CAT_NONE, SLS_UNCHANGED},
/* 1350 */  { addItemToBuffer,             CHR_e_DOT,                   "",                                            STD_e_DOT,                                     CAT_NONE, SLS_UNCHANGED},
/* 1351 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_E_CARON,                                   CAT_NONE, SLS_UNCHANGED},
/* 1352 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_e_CARON,                                   CAT_NONE, SLS_UNCHANGED},
/* 1353 */  { addItemToBuffer,             CHR_R_ACUTE,                 "",                                            STD_R_ACUTE,                                   CAT_NONE, SLS_UNCHANGED},
/* 1354 */  { addItemToBuffer,             CHR_R_CARON,                 "",                                            STD_R_CARON,                                   CAT_NONE, SLS_UNCHANGED},
/* 1355 */  { addItemToBuffer,             CHR_U_OGONEK,                "",                                            STD_U_OGONEK,                                  CAT_NONE, SLS_UNCHANGED},
/* 1356 */  { addItemToBuffer,             CHR_u_OGONEK,                "",                                            STD_u_OGONEK,                                  CAT_NONE, SLS_UNCHANGED},
/* 1357 */  { addItemToBuffer,             CHR_y_UNDER_ROOT,            "",                                            STD_y_UNDER_ROOT,                              CAT_NONE, SLS_UNCHANGED},
/* 1358 */  { addItemToBuffer,             CHR_x_UNDER_ROOT,            "",                                            STD_x_UNDER_ROOT,                              CAT_NONE, SLS_UNCHANGED},
/* 1359 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_EM,                                  CAT_NONE, SLS_UNCHANGED},
/* 1360 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_3_PER_EM,                            CAT_NONE, SLS_UNCHANGED},
/* 1361 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_4_PER_EM,                            CAT_NONE, SLS_UNCHANGED},
/* 1362 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_6_PER_EM,                            CAT_NONE, SLS_UNCHANGED},
/* 1363 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_FIGURE,                              CAT_NONE, SLS_UNCHANGED},
/* 1364 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_PUNCTUATION,                         CAT_NONE, SLS_UNCHANGED},
/* 1365 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_HAIR,                                CAT_NONE, SLS_UNCHANGED},
/* 1366 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LEFT_SINGLE_QUOTE,                         CAT_NONE, SLS_UNCHANGED},
/* 1367 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RIGHT_SINGLE_QUOTE,                        CAT_NONE, SLS_UNCHANGED},
/* 1368 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SINGLE_LOW_QUOTE,                          CAT_NONE, SLS_UNCHANGED},
/* 1369 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SINGLE_HIGH_QUOTE,                         CAT_NONE, SLS_UNCHANGED},
/* 1370 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LEFT_DOUBLE_QUOTE,                         CAT_NONE, SLS_UNCHANGED},
/* 1371 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RIGHT_DOUBLE_QUOTE,                        CAT_NONE, SLS_UNCHANGED},
/* 1372 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOUBLE_LOW_QUOTE,                          CAT_NONE, SLS_UNCHANGED},
/* 1373 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOUBLE_HIGH_QUOTE,                         CAT_NONE, SLS_UNCHANGED},
/* 1374 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ELLIPSIS,                                  CAT_NONE, SLS_UNCHANGED},
/* 1375 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ONE,                                       CAT_NONE, SLS_UNCHANGED},
/* 1376 */  { addItemToBuffer,             CHR_EURO,                    "",                                            STD_EURO,                                      CAT_NONE, SLS_UNCHANGED},
/* 1377 */  { addItemToBuffer,             CHR_COMPLEX_C,               "",                                            STD_COMPLEX_C,                                 CAT_NONE, SLS_UNCHANGED},
/* 1378 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PLANCK,                                    CAT_NONE, SLS_UNCHANGED},
/* 1379 */  { addItemToBuffer,             CHR_PLANCK_2PI,              "",                                            STD_PLANCK_2PI,                                CAT_NONE, SLS_UNCHANGED},
/* 1380 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NATURAL_N,                                 CAT_NONE, SLS_UNCHANGED},
/* 1381 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RATIONAL_Q,                                CAT_NONE, SLS_UNCHANGED},
/* 1382 */  { addItemToBuffer,             CHR_REAL_R,                  "",                                            STD_REAL_R,                                    CAT_NONE, SLS_UNCHANGED},
/* 1383 */  { addItemToBuffer,             CHR_LEFT_ARROW,              "",                                            STD_LEFT_ARROW,                                CAT_NONE, SLS_UNCHANGED},
/* 1384 */  { addItemToBuffer,             CHR_UP_ARROW,                "",                                            STD_UP_ARROW,                                  CAT_NONE, SLS_UNCHANGED},
/* 1385 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_UP_ARROW_b,                                CAT_NONE, SLS_UNCHANGED},
/* 1386 */  { addItemToBuffer,             CHR_RIGHT_ARROW,             "",                                            STD_RIGHT_ARROW,                               CAT_NONE, SLS_UNCHANGED},
/* 1387 */  { addItemToBuffer,             CHR_DOWN_ARROW,              "",                                            STD_DOWN_ARROW,                                CAT_NONE, SLS_UNCHANGED},
/* 1388 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOWN_ARROW_b,                              CAT_NONE, SLS_UNCHANGED},
/* 1389 */  { addItemToBuffer,             CHR_SERIAL_IO,               "",                                            STD_SERIAL_IO,                                 CAT_NONE, SLS_UNCHANGED},
/* 1390 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RIGHT_SHORT_ARROW,                         CAT_NONE, SLS_UNCHANGED},
/* 1391 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LEFT_RIGHT_ARROWS,                         CAT_NONE, SLS_UNCHANGED},
/* 1392 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_BST,                                       CAT_NONE, SLS_UNCHANGED},
/* 1393 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SST,                                       CAT_NONE, SLS_UNCHANGED},
/* 1394 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_HAMBURGER,                                 CAT_NONE, SLS_UNCHANGED},
/* 1395 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_UNDO,                                      CAT_NONE, SLS_UNCHANGED},
/* 1396 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_FOR_ALL,                                   CAT_NONE, SLS_UNCHANGED},
/* 1397 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_COMPLEMENT,                                CAT_NONE, SLS_UNCHANGED},
/* 1398 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PARTIAL_DIFF,                              CAT_NONE, SLS_UNCHANGED},
/* 1399 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_THERE_EXISTS,                              CAT_NONE, SLS_UNCHANGED},
/* 1400 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_THERE_DOES_NOT_EXIST,                      CAT_NONE, SLS_UNCHANGED},
/* 1401 */  { addItemToBuffer,             CHR_EMPTY_SET,               "",                                            STD_EMPTY_SET,                                 CAT_NONE, SLS_UNCHANGED},
/* 1402 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_INCREMENT,                                 CAT_NONE, SLS_UNCHANGED},
/* 1403 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NABLA,                                     CAT_NONE, SLS_UNCHANGED},
/* 1404 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ELEMENT_OF,                                CAT_NONE, SLS_UNCHANGED},
/* 1405 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NOT_ELEMENT_OF,                            CAT_NONE, SLS_UNCHANGED},
/* 1406 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_CONTAINS,                                  CAT_NONE, SLS_UNCHANGED},
/* 1407 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOES_NOT_CONTAIN,                          CAT_NONE, SLS_UNCHANGED},
/* 1408 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ZERO,                                      CAT_NONE, SLS_UNCHANGED},
/* 1409 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PRODUCT,                                   CAT_NONE, SLS_UNCHANGED},
/* 1410 */  { itemToBeCoded,               NOPARAM,                     "1410",                                        "1410",                                        CAT_FREE, SLS_UNCHANGED},
/* 1411 */  { itemToBeCoded,               NOPARAM,                     "1411",                                        "1411",                                        CAT_FREE, SLS_UNCHANGED},
/* 1412 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MINUS_PLUS,                                CAT_NONE, SLS_UNCHANGED},
/* 1413 */  { itemToBeCoded,               NOPARAM,                     "1413",                                        "1413",                                        CAT_FREE, SLS_UNCHANGED},
/* 1414 */  { itemToBeCoded,               NOPARAM,                     "1414",                                        "1414",                                        CAT_FREE, SLS_UNCHANGED},
/* 1415 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RING,                                      CAT_NONE, SLS_UNCHANGED},
/* 1416 */  { addItemToBuffer,             CHR_BULLET,                  "",                                            STD_BULLET,                                    CAT_NONE, SLS_UNCHANGED},
/* 1417 */  { addItemToBuffer,             CHR_SQUARE_ROOT,             "",                                            STD_SQUARE_ROOT,                               CAT_NONE, SLS_UNCHANGED},
/* 1418 */  { addItemToBuffer,             CHR_CUBE_ROOT,               "",                                            STD_CUBE_ROOT,                                 CAT_NONE, SLS_UNCHANGED},
/* 1419 */  { addItemToBuffer,             CHR_xTH_ROOT,                "",                                            STD_xTH_ROOT,                                  CAT_NONE, SLS_UNCHANGED},
/* 1420 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PROPORTIONAL,                              CAT_NONE, SLS_UNCHANGED},
/* 1421 */  { addItemToBuffer,             CHR_INFINITY,                "",                                            STD_INFINITY,                                  CAT_NONE, SLS_UNCHANGED},
/* 1422 */  { addItemToBuffer,             CHR_RIGHT_ANGLE,             "",                                            STD_RIGHT_ANGLE,                               CAT_NONE, SLS_UNCHANGED},
/* 1423 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ANGLE,                                     CAT_NONE, SLS_UNCHANGED},
/* 1424 */  { addItemToBuffer,             CHR_MEASURED_ANGLE,          "",                                            STD_MEASURED_ANGLE,                            CAT_NONE, SLS_UNCHANGED},
/* 1425 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DIVIDES,                                   CAT_NONE, SLS_UNCHANGED},
/* 1426 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOES_NOT_DIVIDE,                           CAT_NONE, SLS_UNCHANGED},
/* 1427 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PARALLEL,                                  CAT_NONE, SLS_UNCHANGED},
/* 1428 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NOT_PARALLEL,                              CAT_NONE, SLS_UNCHANGED},
/* 1429 */  { addItemToBuffer,             CHR_AND,                     "",                                            STD_AND,                                       CAT_NONE, SLS_UNCHANGED},
/* 1430 */  { addItemToBuffer,             CHR_OR,                      "",                                            STD_OR,                                        CAT_NONE, SLS_UNCHANGED},
/* 1431 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_INTERSECTION,                              CAT_NONE, SLS_UNCHANGED},
/* 1432 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_UNION,                                     CAT_NONE, SLS_UNCHANGED},
/* 1433 */  { addItemToBuffer,             CHR_INTEGRAL,                "",                                            STD_INTEGRAL,                                  CAT_NONE, SLS_UNCHANGED},
/* 1434 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOUBLE_INTEGRAL,                           CAT_NONE, SLS_UNCHANGED},
/* 1435 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_CONTOUR_INTEGRAL,                          CAT_NONE, SLS_UNCHANGED},
/* 1436 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SURFACE_INTEGRAL,                          CAT_NONE, SLS_UNCHANGED},
/* 1437 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RATIO,                                     CAT_NONE, SLS_UNCHANGED},
/* 1438 */  { addItemToBuffer,             CHR_CHECK_MARK,              "",                                            STD_CHECK_MARK,                                CAT_NONE, SLS_UNCHANGED},
/* 1439 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ASYMPOTICALLY_EQUAL,                       CAT_NONE, SLS_UNCHANGED},
/* 1440 */  { addItemToBuffer,             CHR_ALMOST_EQUAL,            "",                                            STD_ALMOST_EQUAL,                              CAT_NONE, SLS_UNCHANGED},
/* 1441 */  { addItemToBuffer,             CHR_COLON_EQUALS,            "",                                            STD_COLON_EQUALS,                              CAT_NONE, SLS_UNCHANGED},
/* 1442 */  { addItemToBuffer,             CHR_CORRESPONDS_TO,          "",                                            STD_CORRESPONDS_TO,                            CAT_NONE, SLS_UNCHANGED},
/* 1443 */  { addItemToBuffer,             CHR_ESTIMATES,               "",                                            STD_ESTIMATES,                                 CAT_NONE, SLS_UNCHANGED},
/* 1444 */  { addItemToBuffer,             CHR_NOT_EQUAL,               "",                                            STD_NOT_EQUAL,                                 CAT_NONE, SLS_UNCHANGED},
/* 1445 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_IDENTICAL_TO,                              CAT_NONE, SLS_UNCHANGED},
/* 1446 */  { addItemToBuffer,             CHR_LESS_EQUAL,              "",                                            STD_LESS_EQUAL,                                CAT_NONE, SLS_UNCHANGED},
/* 1447 */  { addItemToBuffer,             CHR_GREATER_EQUAL,           "",                                            STD_GREATER_EQUAL,                             CAT_NONE, SLS_UNCHANGED},
/* 1448 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MUCH_LESS,                                 CAT_NONE, SLS_UNCHANGED},
/* 1449 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MUCH_GREATER,                              CAT_NONE, SLS_UNCHANGED},
/* 1450 */  { addItemToBuffer,             CHR_SUN,                     "",                                            STD_SUN,                                       CAT_NONE, SLS_UNCHANGED},
/* 1451 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOWN_TACK,                                 CAT_NONE, SLS_UNCHANGED},
/* 1452 */  { addItemToBuffer,             CHR_PERPENDICULAR,           "",                                            STD_PERPENDICULAR,                             CAT_NONE, SLS_UNCHANGED},
/* 1453 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_XOR,                                       CAT_NONE, SLS_UNCHANGED},
/* 1454 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NAND,                                      CAT_NONE, SLS_UNCHANGED},
/* 1455 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NOR,                                       CAT_NONE, SLS_UNCHANGED},
/* 1456 */  { addItemToBuffer,             CHR_WATCH,                   "",                                            STD_WATCH,                                     CAT_NONE, SLS_UNCHANGED},
/* 1457 */  { addItemToBuffer,             CHR_HOURGLASS,               "",                                            STD_HOURGLASS,                                 CAT_NONE, SLS_UNCHANGED},
/* 1458 */  { addItemToBuffer,             CHR_PRINTER,                 "",                                            STD_PRINTER,                                   CAT_NONE, SLS_UNCHANGED},
/* 1459 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_TL,                                    CAT_NONE, SLS_UNCHANGED},
/* 1460 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_ML,                                    CAT_NONE, SLS_UNCHANGED},
/* 1461 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_BL,                                    CAT_NONE, SLS_UNCHANGED},
/* 1462 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_TR,                                    CAT_NONE, SLS_UNCHANGED},
/* 1463 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_MR,                                    CAT_NONE, SLS_UNCHANGED},
/* 1464 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_BR,                                    CAT_NONE, SLS_UNCHANGED},
/* 1465 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OBLIQUE1,                                  CAT_NONE, SLS_UNCHANGED},
/* 1466 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OBLIQUE2,                                  CAT_NONE, SLS_UNCHANGED},
/* 1467 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OBLIQUE3,                                  CAT_NONE, SLS_UNCHANGED},
/* 1468 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OBLIQUE4,                                  CAT_NONE, SLS_UNCHANGED},
/* 1469 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_CURSOR,                                    CAT_NONE, SLS_UNCHANGED},
/* 1470 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PERIOD34,                                  CAT_NONE, SLS_UNCHANGED},
/* 1471 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_COMMA34,                                   CAT_NONE, SLS_UNCHANGED},
/* 1472 */  { addItemToBuffer,             CHR_BATTERY,                 "",                                            STD_BATTERY,                                   CAT_NONE, SLS_UNCHANGED},
/* 1473 */  { addItemToBuffer,             CHR_PGM_BEGIN,               "",                                            STD_PGM_BEGIN,                                 CAT_NONE, SLS_UNCHANGED},
/* 1474 */  { addItemToBuffer,             CHR_USER_MODE,               "",                                            STD_USER_MODE,                                 CAT_NONE, SLS_UNCHANGED},
/* 1475 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_UK,                                        CAT_NONE, SLS_UNCHANGED},
/* 1476 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_US,                                        CAT_NONE, SLS_UNCHANGED},
/* 1477 */  { addItemToBuffer,             CHR_NEG_EXCLAMATION_MARK,    "",                                            STD_NEG_EXCLAMATION_MARK,                      CAT_NONE, SLS_UNCHANGED},
/* 1478 */  { addItemToBuffer,             CHR_ex,                      "",                                            STD_LEFT_RIGHT_ARROWS,                         CAT_NONE, SLS_UNCHANGED},
/* 1479 */  { addItemToBuffer,             ITM_Max,                     "",                                            "Max",                                         CAT_NONE, SLS_UNCHANGED},
/* 1480 */  { addItemToBuffer,             ITM_Min,                     "",                                            "Min",                                         CAT_NONE, SLS_UNCHANGED},
/* 1481 */  { addItemToBuffer,             ITM_Config,                  "",                                            "Config",                                      CAT_NONE, SLS_UNCHANGED},
/* 1482 */  { addItemToBuffer,             ITM_Stack,                   "",                                            "Stack",                                       CAT_NONE, SLS_UNCHANGED},
/* 1483 */  { addItemToBuffer,             ITM_dddEL,                   "",                                            STD_ELLIPSIS "EL",                             CAT_NONE, SLS_UNCHANGED},
/* 1484 */  { addItemToBuffer,             ITM_dddIJ,                   "",                                            STD_ELLIPSIS "IJ",                             CAT_NONE, SLS_UNCHANGED},
/* 1485 */  { itemToBeCoded,               NOPARAM,                     "",                                            "0.",                                          CAT_NONE, SLS_UNCHANGED},
/* 1486 */  { itemToBeCoded,               NOPARAM,                     "",                                            "1.",                                          CAT_NONE, SLS_UNCHANGED},
/* 1487 */  { addItemToBuffer,             ITM_EXPONENT,                "",                                            "EEX",                                         CAT_NONE, SLS_UNCHANGED},      //JM Change E to EEX
/* 1488 */  { addItemToBuffer,             ITM_MA11,                    "",                                            STD_a_DIARESIS,                                CAT_NONE, SLS_UNCHANGED},
/* 1489 */  { addItemToBuffer,             ITM_MA12,                    "",                                            STD_o_DIARESIS,                                CAT_NONE, SLS_UNCHANGED},
/* 1490 */  { addItemToBuffer,             ITM_MA13,                    "",                                            STD_u_DIARESIS,                                CAT_NONE, SLS_UNCHANGED},
/* 1491 */  { addItemToBuffer,             ITM_MA14,                    "",                                            "",                                            CAT_NONE, SLS_UNCHANGED},
/* 1492 */  { addItemToBuffer,             ITM_MA15,                    "",                                            "sch",                                         CAT_NONE, SLS_UNCHANGED},
/* 1493 */  { addItemToBuffer,             ITM_MA16,                    "",                                            STD_s_SHARP,                                   CAT_NONE, SLS_UNCHANGED},
/* 1494 */  { addItemToBuffer,             ITM_MA21,                    "",                                            "not",                                         CAT_NONE, SLS_UNCHANGED},
/* 1495 */  { addItemToBuffer,             ITM_MA22,                    "",                                            "edi-",                                        CAT_NONE, SLS_UNCHANGED},
/* 1496 */  { addItemToBuffer,             ITM_MA23,                    "",                                            "table",                                       CAT_NONE, SLS_UNCHANGED},
/* 1497 */  { addItemToBuffer,             ITM_MA24,                    "",                                            "for",                                         CAT_NONE, SLS_UNCHANGED},
/* 1498 */  { addItemToBuffer,             ITM_MA25,                    "",                                            "now",                                         CAT_NONE, SLS_UNCHANGED},
/* 1499 */  { addItemToBuffer,             ITM_MA26,                    "",                                            "",                                            CAT_NONE, SLS_UNCHANGED},
/* 1500 */  { addItemToBuffer,             ITM_MA31,                    "",                                            "",                                            CAT_NONE, SLS_UNCHANGED},
/* 1501 */  { addItemToBuffer,             ITM_MA32,                    "",                                            "",                                            CAT_NONE, SLS_UNCHANGED},
/* 1502 */  { addItemToBuffer,             ITM_MA33,                    "",                                            "",                                            CAT_NONE, SLS_UNCHANGED},
/* 1503 */  { addItemToBuffer,             ITM_MA34,                    "",                                            "",                                            CAT_NONE, SLS_UNCHANGED},
/* 1504 */  { addItemToBuffer,             ITM_MA35,                    "",                                            "",                                            CAT_NONE, SLS_UNCHANGED},
/* 1505 */  { addItemToBuffer,             ITM_MA36,                    "",                                            "",                                            CAT_NONE, SLS_UNCHANGED},
/* 1506 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_UNDO,                                      CAT_NONE, SLS_UNCHANGED},
/* 1507 */  { itemToBeCoded,               NOPARAM,                     "",                                            "PRGM",                                        CAT_NONE, SLS_UNCHANGED},      //JM Change P/R to PRGM
/* 1508 */  { itemToBeCoded,               NOPARAM,                     "",                                            "R/S",                                         CAT_NONE, SLS_UNCHANGED},
/* 1509 */  { itemToBeCoded,               NOPARAM,                     "",                                            "Not",                                         CAT_NONE, SLS_UNCHANGED},
/* 1510 */  { itemToBeCoded,               NOPARAM,                     "",                                            "yet",                                         CAT_NONE, SLS_UNCHANGED},
/* 1511 */  { itemToBeCoded,               NOPARAM,                     "",                                            "defined",                                     CAT_NONE, SLS_UNCHANGED},
/* 1512 */  { itemToBeCoded,               NOPARAM,                     "",                                            "Tam",                                         CAT_NONE, SLS_UNCHANGED},
/* 1513 */  { itemToBeCoded,               NOPARAM,                     "",                                            "TamCmp",                                      CAT_NONE, SLS_UNCHANGED},
/* 1514 */  { itemToBeCoded,               NOPARAM,                     "",                                            "TamStoRcl",                                   CAT_NONE, SLS_UNCHANGED},
/* 1515 */  { fnUserMode,                  NOPARAM,                     "",                                            "USER",                                        CAT_NONE, SLS_UNCHANGED},
/* 1516 */  { fnComplexCCCC_CC,            NOPARAM,                     "CC",                                          "CC",                                          CAT_FNCT, SLS_UNCHANGED},      //JM Change CC to COMPLEX
/* 1517 */  { itemToBeCoded,               NOPARAM,                     "",                                            "f",                                           CAT_NONE, SLS_UNCHANGED},
/* 1518 */  { itemToBeCoded,               NOPARAM,                     "",                                            "g",                                           CAT_NONE, SLS_UNCHANGED},
/* 1519 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_UP_ARROW,                                  CAT_NONE, SLS_UNCHANGED},
/* 1520 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_HAMBURGER STD_BST,                         CAT_NONE, SLS_UNCHANGED},
/* 1521 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOWN_ARROW,                                CAT_NONE, SLS_UNCHANGED},
/* 1522 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_HAMBURGER STD_SST,                         CAT_NONE, SLS_UNCHANGED},
/* 1523 */  { itemToBeCoded,               NOPARAM,                     "",                                            "EXIT",                                        CAT_NONE, SLS_UNCHANGED},
/* 1524 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LEFT_ARROW,                                CAT_NONE, SLS_UNCHANGED},
/* 1525 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PRINTER "x",                               CAT_NONE, SLS_UNCHANGED},
#ifdef JM_LAYOUT_2_DM42_STRICT //JM LAYOUT 2. DM42 STRICT.
/* 1526 */  { fnAim,                       NOPARAM,                     "",                                            "ALPHA", /*STD_alpha,*/                        CAT_NONE, SLS_UNCHANGED},      //JM
#endif //JM END OF LAYOUT 2 DM42 STRICT.
#ifdef JM_LAYOUT_1A
/* 1526 */  { fnAim,                       NOPARAM,                     "",                                            STD_alpha,                                     CAT_NONE, SLS_UNCHANGED},      //JM
#endif //JM END OF LAYOUT 1
/* 1527 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RIGHT_ARROW ".d",                          CAT_NONE, SLS_UNCHANGED},      //JM mod
/* 1528 */  { fnCvtQuartM3,                multiply,                    "qt." STD_RIGHT_ARROW "m" STD_SUP_3,           "qt." STD_RIGHT_ARROW "m" STD_SUP_3,           CAT_FNCT, SLS_ENABLED  },
/* 1529 */  { fnCvtQuartM3,                divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "qt.",           "m" STD_SUP_3 STD_RIGHT_ARROW "qt.",           CAT_FNCT, SLS_ENABLED  },
/* 1530 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "SP",                          STD_RIGHT_ARROW "SP",                          CAT_FNCT, SLS_ENABLED  },
/* 1531 */  { itemToBeCoded,               NOPARAM,                     "SHOW",                                        "SHOW",                                        CAT_FNCT, SLS_UNCHANGED},
/* 1532 */  { backToSystem,                NOPARAM,                     "SYSTEM",                                      "SYSTEM",                                      CAT_NONE, SLS_UNCHANGED},
/* 1533 */  { fnCvtDmsToDeg,               NOPARAM,                     "D.MS" STD_RIGHT_ARROW "D",                    "D.MS" STD_RIGHT_ARROW "D",                    CAT_FNCT, SLS_ENABLED  },
/* 1534 */  { itemToBeCoded,               NOPARAM,                     "V" STD_MEASURED_ANGLE,                        STD_MEASURED_ANGLE,                            CAT_FNCT, SLS_ENABLED  },
/* 1535 */  { fnCvtFathomM,                multiply,                    "fm." STD_RIGHT_ARROW "m",                     "fathom",                                      CAT_FNCT, SLS_ENABLED  },
/* 1536 */  { fnCvtFathomM,                multiply,                    "fm." STD_RIGHT_ARROW "m",                     STD_RIGHT_ARROW " m",                          CAT_DUPL, SLS_ENABLED  },
/* 1537 */  { fnCvtFathomM,                divide,                      "m" STD_RIGHT_ARROW "fm.",                     "m " STD_RIGHT_ARROW,                          CAT_FNCT, SLS_ENABLED  },
/* 1538 */  { fnCvtFathomM,                divide,                      "m" STD_RIGHT_ARROW "fm.",                     "fathom",                                      CAT_DUPL, SLS_ENABLED  },
/* 1539 */  { fnCvtSfeetM,                 multiply,                    "ft" STD_US STD_RIGHT_ARROW "m",               STD_RIGHT_ARROW " m",                          CAT_DUPL, SLS_ENABLED  },
/* 1540 */  { fnCvtSfeetM,                 divide,                      "m" STD_RIGHT_ARROW "ft" STD_US,               "foot" STD_US,                                 CAT_DUPL, SLS_ENABLED  },
/* 1541 */  { fnCvtPointM,                 divide,                      "m" STD_RIGHT_ARROW "pt.",                     "m " STD_RIGHT_ARROW,                          CAT_DUPL, SLS_ENABLED  },
/* 1542 */  { fnCvtPointM,                 multiply,                    "pt." STD_RIGHT_ARROW "m",                     STD_RIGHT_ARROW " m",                          CAT_DUPL, SLS_ENABLED  },
/* 1543 */  { fnCvtBarrelM3,               multiply,                    "bbl" STD_RIGHT_ARROW "m" STD_SUP_3,           "barrel",                                      CAT_FNCT, SLS_ENABLED  },
/* 1544 */  { fnCvtBarrelM3,               multiply,                    "bbl" STD_RIGHT_ARROW "m" STD_SUP_3,           STD_RIGHT_ARROW " m" STD_SUP_3,                CAT_DUPL, SLS_ENABLED  },
/* 1545 */  { fnCvtBarrelM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "bbl",           STD_RIGHT_ARROW " m" STD_SUP_3,                CAT_FNCT, SLS_ENABLED  },
/* 1546 */  { fnCvtBarrelM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "bbl",           "barrel",                                      CAT_DUPL, SLS_ENABLED  },
/* 1547 */  { fnCvtTonKg,                  multiply,                    "ton" STD_RIGHT_ARROW "kg",                    STD_SPACE_HAIR,                                CAT_DUPL, SLS_ENABLED  },
/* 1548 */  { fnCvtTonKg,                  multiply,                    "ton" STD_RIGHT_ARROW "kg",                    STD_SPACE_HAIR,                                CAT_DUPL, SLS_ENABLED  },
/* 1549 */  { fnCvtTonKg,                  divide,                      "kg" STD_RIGHT_ARROW "ton",                    STD_SPACE_HAIR,                                CAT_DUPL, SLS_ENABLED  },
/* 1550 */  { fnCvtTonKg,                  divide,                      "kg" STD_RIGHT_ARROW "ton",                    STD_SPACE_HAIR,                                CAT_DUPL, SLS_ENABLED  },
/* 1551 */  { fnCvtCaratKg,                multiply,                    "ct" STD_RIGHT_ARROW "kg",                     STD_SPACE_HAIR,                                CAT_DUPL, SLS_ENABLED  },
/* 1552 */  { fnCvtCaratKg,                divide,                      "kg" STD_RIGHT_ARROW "ct",                     STD_SPACE_HAIR,                                CAT_DUPL, SLS_ENABLED  },
/* 1553 */  { fnCvtAtmPa,                  multiply,                    "atm" STD_RIGHT_ARROW "Pa",                    STD_SPACE_HAIR,                                CAT_DUPL, SLS_ENABLED  },
/* 1554 */  { fnCvtAtmPa,                  divide,                      "Pa" STD_RIGHT_ARROW "atm",                    STD_SPACE_HAIR,                                CAT_DUPL, SLS_ENABLED  },
/* 1555 */  { fnCvtFathomM,                multiply,                    "fm." STD_RIGHT_ARROW "m",                     STD_SPACE_HAIR,                                CAT_DUPL, SLS_ENABLED  },
/* 1556 */  { fnCvtFathomM,                divide,                      "m" STD_RIGHT_ARROW "fm.",                     STD_SPACE_HAIR,                                CAT_DUPL, SLS_ENABLED  },
/* 1557 */  { fnCvtPointM,                 multiply,                    "pt." STD_RIGHT_ARROW "m",                     STD_SPACE_HAIR,                                CAT_DUPL, SLS_ENABLED  },
/* 1558 */  { fnCvtPointM,                 divide,                      "m" STD_RIGHT_ARROW "pt.",                     STD_SPACE_HAIR,                                CAT_DUPL, SLS_ENABLED  },
/* 1559 */  { itemToBeCoded,               NOPARAM,                     STD_x_BAR STD_SUB_H,                           STD_x_BAR STD_SUB_H,                           CAT_FNCT, SLS_UNCHANGED},
/* 1560 */  { itemToBeCoded,               NOPARAM,                     STD_x_BAR STD_SUB_R STD_SUB_M STD_SUB_S,       STD_x_BAR STD_SUB_R STD_SUB_M STD_SUB_S,       CAT_FNCT, SLS_UNCHANGED},
/* 1561 */  { itemToBeCoded,               NOPARAM,                     "SUMS",                                        "SUMS",                                        CAT_MENU, SLS_UNCHANGED},
/* 1562 */  { itemToBeCoded,               NOPARAM,                     "GaussF",                                      "GaussF",                                      CAT_FNCT, SLS_UNCHANGED},
/* 1563 */  { itemToBeCoded,               NOPARAM,                     "CauchF",                                      "CauchF",                                      CAT_FNCT, SLS_UNCHANGED},
/* 1564 */  { itemToBeCoded,               NOPARAM,                     "ParabF",                                      "ParabF",                                      CAT_FNCT, SLS_UNCHANGED},
/* 1565 */  { itemToBeCoded,               NOPARAM,                     "HypF",                                        "HypF",                                        CAT_FNCT, SLS_UNCHANGED},
/* 1566 */  { itemToBeCoded,               NOPARAM,                     "RootF",                                       "RootF",                                       CAT_FNCT, SLS_UNCHANGED},
/* 1567 */  { fnStatSum,                   14,                          STD_SIGMA "lny/x",                             STD_SIGMA "lny/x",                             CAT_FNCT, SLS_ENABLED  },
/* 1568 */  { fnStatSum,                   15,                          STD_SIGMA "x" STD_SUP_2 "/y",                  STD_SIGMA "x" STD_SUP_2 "/y",                  CAT_FNCT, SLS_ENABLED  },
/* 1569 */  { fnStatSum,                   16,                          STD_SIGMA STD_SUP_1 "/x",                      STD_SIGMA STD_SUP_1 "/x",                      CAT_FNCT, SLS_ENABLED  },
/* 1570 */  { fnStatSum,                   17,                          STD_SIGMA STD_SUP_1 "/x" STD_SUP_2,            STD_SIGMA STD_SUP_1 "/x" STD_SUP_2,            CAT_FNCT, SLS_ENABLED  },
/* 1571 */  { fnStatSum,                   18,                          STD_SIGMA "x/y",                               STD_SIGMA "x/y",                               CAT_FNCT, SLS_ENABLED  },
/* 1572 */  { fnStatSum,                   19,                          STD_SIGMA STD_SUP_1 "/y",                      STD_SIGMA STD_SUP_1 "/y",                      CAT_FNCT, SLS_ENABLED  },
/* 1573 */  { fnStatSum,                   20,                          STD_SIGMA STD_SUP_1 "/y" STD_SUP_2,            STD_SIGMA STD_SUP_1 "/y" STD_SUP_2,            CAT_FNCT, SLS_ENABLED  },
/* 1574 */  { fnStatSum,                   21,                          STD_SIGMA "x" STD_SUP_3,                       STD_SIGMA "x" STD_SUP_3,                       CAT_FNCT, SLS_ENABLED  },
/* 1575 */  { fnStatSum,                   22,                          STD_SIGMA "x" STD_SUP_4,                       STD_SIGMA "x" STD_SUP_4,                       CAT_FNCT, SLS_ENABLED  },
/* 1576 */  { addItemToBuffer,             NOPARAM,                     "HEX",                                         "H",                                           CAT_NONE, SLS_UNCHANGED},
/* 1577 */  { fnIDivR,                     NOPARAM,                     "IDIVR",                                       "IDIVR",                                       CAT_FNCT, SLS_ENABLED  },
/* 1578 */  { fnArccos,                    NOPARAM,                     "ACOS",                                        "ACOS",                                        CAT_FNCT, SLS_ENABLED  },
/* 1579 */  { fnArcsin,                    NOPARAM,                     "ASIN",                                        "ASIN",                                        CAT_FNCT, SLS_ENABLED  },
/* 1580 */  { fnArctan,                    NOPARAM,                     "ATAN",                                        "ATAN",                                        CAT_FNCT, SLS_ENABLED  },
/* 1581 */  { itemToBeCoded,               NOPARAM,                     "DET",                                         "DET",                                         CAT_FNCT, SLS_ENABLED  },
/* 1582 */  { itemToBeCoded,               NOPARAM,                     "INVRT",                                       "INVRT",                                       CAT_FNCT, SLS_ENABLED  },
/* 1583 */  { itemToBeCoded,               NOPARAM,                     "TRANS",                                       "TRANS",                                       CAT_FNCT, SLS_ENABLED  },
/* 1584 */  { itemToBeCoded,               NOPARAM,                     "xIN",                                         "xIN",                                         CAT_FNCT, SLS_ENABLED  },
/* 1585 */  { itemToBeCoded,               NOPARAM,                     "xOUT",                                        "xOUT",                                        CAT_FNCT, SLS_ENABLED  },
/* 1586 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "SR",                                STD_alpha "SR",                                CAT_FNCT, SLS_UNCHANGED},
/* 1587 */  { fnCvtHectareM2,              multiply,                    "ha" STD_RIGHT_ARROW "m" STD_SUP_2,            "ha" STD_RIGHT_ARROW "m" STD_SUP_2,            CAT_FNCT, SLS_ENABLED  },
/* 1588 */  { fnCvtHectareM2,              divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ha",            "m" STD_SUP_2 STD_RIGHT_ARROW "ha",            CAT_FNCT, SLS_ENABLED  },

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/* 1589 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1590 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1591 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1592 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1593 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1594 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1595 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1596 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1597 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1598 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1599 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1600 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1601 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1602 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1603 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1604 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1605 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1606 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1607 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1608 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1609 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1610 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1611 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1612 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1613 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1615 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1615 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1616 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1617 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1618 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1619 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1620 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1621 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1622 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1623 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1624 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1625 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1626 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1627 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1628 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1629 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1630 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1631 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1632 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1633 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1634 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1635 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1636 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1637 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1638 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1639 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1640 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1641 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1642 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1643 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1644 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1645 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1646 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1647 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1648 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1649 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1650 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1651 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1652 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1653 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1654 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1655 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1656 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1657 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1658 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1659 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1660 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1661 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1662 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1663 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1664 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1665 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1666 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1667 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1668 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1669 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1670 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1671 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1672 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1673 */  { itemToBeCoded,               NOPARAM,                     "",                                            "",                                            CAT_FREE, SLS_UNCHANGED},
/* 1674 */  { fnCvtmmhgPa,                 multiply,                    "mmHg" STD_RIGHT_ARROW "Pa",                   "mm.Hg",                                       CAT_FNCT, SLS_ENABLED  },
/* 1675 */  { fnCvtmmhgPa,                 divide,                      "Pa" STD_RIGHT_ARROW "mmHg",                   "Pa" STD_RIGHT_ARROW,                          CAT_FNCT, SLS_ENABLED  },
/* 1676 */  { fnCvtmmhgPa,                 multiply,                    "mmHg" STD_RIGHT_ARROW "Pa",                   STD_RIGHT_ARROW " Pa",                         CAT_DUPL, SLS_ENABLED  },
/* 1677 */  { fnCvtmmhgPa,                 divide,                      "Pa" STD_RIGHT_ARROW "mmHg",                   "mm.Hg",                                       CAT_DUPL, SLS_ENABLED  },

/* 1678 */  { fnSetSetJM,                  JC_ERPN,                     "SET_ERPN",                                    "eRPN",                                        CAT_FNCT, SLS_ENABLED  },      //JM eRPN
/* 1679 */  { fnSetSetJM,                  JC_HOME_TRIPLE,              "SET_HOME_TRIPLE",                             "HOME.3",                                      CAT_FNCT, SLS_ENABLED  },      //JM HOME.3
/* 1680 */  { fnSetSetJM,                  JC_SHFT_4s,                  "SET_SHFT_4s",                                 "SH_4s",                                       CAT_FNCT, SLS_ENABLED  },      //JM SHIFT CANCEL
/* 1681 */  { itemToBeCoded,               NOPARAM,                     "HOME",                                        "HOME",                                        CAT_FNCT, SLS_UNCHANGED},      //JM HOME
/* 1682 */  { fnDisplayFormatSigFig,       TM_VALUE,                    "SIGFIG",                                      "SIGFIG",                                      CAT_FNCT, SLS_UNCHANGED},      //JM SIGFIG
/* 1683 */  { itemToBeCoded,               NOPARAM,                     "ALPHA",                                       "ALPHA",                                       CAT_FNCT, SLS_UNCHANGED},      //JM ALPHA
/* 1684 */  { itemToBeCoded,               NOPARAM,                     "BASE",                                        "BASE",                                        CAT_FNCT, SLS_UNCHANGED},      //JM BASE
/* 1685 */  { fnChangeBase,                2,                           STD_RIGHT_ARROW "BIN",                         STD_RIGHT_ARROW "BIN",                         CAT_FNCT, SLS_UNCHANGED},      //JM HEX
/* 1686 */  { fnChangeBase,                8,                           STD_RIGHT_ARROW "OCT",                         STD_RIGHT_ARROW "OCT",                         CAT_FNCT, SLS_UNCHANGED},      //JM HEX
/* 1687 */  { fnChangeBase,                10,                          STD_RIGHT_ARROW "DEC",                         STD_RIGHT_ARROW "DEC",                         CAT_FNCT, SLS_UNCHANGED},      //JM HEX
/* 1688 */  { fnChangeBase,                16,                          STD_RIGHT_ARROW "HEX",                         STD_RIGHT_ARROW "HEX",                         CAT_FNCT, SLS_UNCHANGED},      //JM HEX
/* 1689 */  { fnSetWordSize,               8,                           "8-BIT",                                       "8-BIT",                                       CAT_FNCT, SLS_UNCHANGED},      //JM HEX
/* 1690 */  { fnSetWordSize,               16,                          "16-BIT",                                      "16-BIT",                                      CAT_FNCT, SLS_UNCHANGED},      //JM HEX
/* 1691 */  { fnSetWordSize,               32,                          "32-BIT",                                      "32-BIT",                                      CAT_FNCT, SLS_UNCHANGED},      //JM HEX
/* 1692 */  { fnSetWordSize,               64,                          "64-BIT",                                      "64-BIT",                                      CAT_FNCT, SLS_UNCHANGED},      //JM HEX
/* 1693 */  { fnDisplayFormatUnit,         TM_VALUE,                    "SETUNIT",                                     "UNITS",                                       CAT_FNCT, SLS_UNCHANGED},      //JM UNIT
/* 1694 */  { fnShowJM,                    JC_ERPN,                     "eRPN?",                                       "eRPN?",                                       CAT_FNCT, SLS_ENABLED  },      //JM SHOW
/* 1695 */  { itemToBeCoded,               NOPARAM,                     "1695",                                        "1695",                                        CAT_FREE, SLS_UNCHANGED},
/* 1696 */  { itemToBeCoded,               NOPARAM,                     "1696",                                        "1696",                                        CAT_FREE, SLS_UNCHANGED},
/* 1697 */  { addItemToBuffer,             CHR_QOPPA,                   "",                                            STD_QOPPA,                                     CAT_NONE, SLS_UNCHANGED},      //JM GREEK
/* 1698 */  { addItemToBuffer,             CHR_DIGAMMA,                 "",                                            STD_DIGAMMA,                                   CAT_NONE, SLS_UNCHANGED},      //JM GREEK
/* 1699 */  { addItemToBuffer,             CHR_SAMPI,                   "",                                            STD_SAMPI,                                     CAT_NONE, SLS_UNCHANGED},      //JM GREEK

/* 1700 */  { itemToBeCoded,               NOPARAM,                     "1599",                                        "1599",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1701 */  { itemToBeCoded,               NOPARAM,                     "1600",                                        "1600",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1702 */  { itemToBeCoded,               NOPARAM,                     "1601",                                        "1601",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1703 */  { itemToBeCoded,               NOPARAM,                     "1602",                                        "1602",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1704 */  { itemToBeCoded,               NOPARAM,                     "1603",                                        "1603",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1705 */  { itemToBeCoded,               NOPARAM,                     "1604",                                        "1604",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1706 */  { itemToBeCoded,               NOPARAM,                     "1605",                                        "1605",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1707 */  { itemToBeCoded,               NOPARAM,                     "1606",                                        "1606",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1708 */  { itemToBeCoded,               NOPARAM,                     "1607",                                        "1607",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1709 */  { itemToBeCoded,               NOPARAM,                     "1608",                                        "1608",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1710 */  { itemToBeCoded,               NOPARAM,                     "1609",                                        "1609",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1711 */  { itemToBeCoded,               NOPARAM,                     "1610",                                        "1610",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1712 */  { itemToBeCoded,               NOPARAM,                     "1611",                                        "1611",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1713 */  { itemToBeCoded,               NOPARAM,                     "1612",                                        "1612",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1714 */  { itemToBeCoded,               NOPARAM,                     "1613",                                        "1613",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1715 */  { itemToBeCoded,               NOPARAM,                     "1614",                                        "1614",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1716 */  { itemToBeCoded,               NOPARAM,                     "1615",                                        "1615",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1717 */  { itemToBeCoded,               NOPARAM,                     "1616",                                        "1616",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1718 */  { itemToBeCoded,               NOPARAM,                     "1617",                                        "1617",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1719 */  { itemToBeCoded,               NOPARAM,                     "1618",                                        "1618",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1720 */  { itemToBeCoded,               NOPARAM,                     "1619",                                        "1619",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1721 */  { itemToBeCoded,               NOPARAM,                     "1620",                                        "1620",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1722 */  { itemToBeCoded,               NOPARAM,                     "1621",                                        "1621",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1723 */  { itemToBeCoded,               NOPARAM,                     "1622",                                        "1622",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1724 */  { itemToBeCoded,               NOPARAM,                     "1623",                                        "1623",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1725 */  { itemToBeCoded,               NOPARAM,                     "1624",                                        "1624",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1726 */  { itemToBeCoded,               NOPARAM,                     "1625",                                        "1625",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1727 */  { itemToBeCoded,               NOPARAM,                     "1626",                                        "1626",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1728 */  { itemToBeCoded,               NOPARAM,                     "1627",                                        "1627",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1729 */  { itemToBeCoded,               NOPARAM,                     "1628",                                        "1628",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1730 */  { itemToBeCoded,               NOPARAM,                     "1629",                                        "1629",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1731 */  { itemToBeCoded,               NOPARAM,                     "1630",                                        "1630",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1732 */  { itemToBeCoded,               NOPARAM,                     "1631",                                        "1631",                                        CAT_FREE, SLS_UNCHANGED},      //JM GREEK
/* 1733 */  { addItemToBuffer,             CHR_qoppa,                   "",                                            STD_qoppa,                                     CAT_NONE, SLS_UNCHANGED},      //JM GREEK
/* 1734 */  { addItemToBuffer,             CHR_digamma,                 "",                                            STD_digamma,                                   CAT_NONE, SLS_UNCHANGED},      //JM GREEK
/* 1735 */  { addItemToBuffer,             CHR_sampi,                   "",                                            STD_sampi,                                     CAT_NONE, SLS_UNCHANGED},      //JM GREEK
/* 1736 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_case,                                      CAT_NONE, SLS_UNCHANGED},      //JM CASE
/* 1737 */  { fnBASE_Hash,      /*TEST!*/  NOPARAM,                     "",                                            "##",                                          CAT_NONE, SLS_UNCHANGED},      //JM ##
/* 1738 */  { itemToBeCoded,               NOPARAM,                     "1738",                                        "1738",                                        CAT_FREE, SLS_UNCHANGED},

/* 1739 */  { fnJM,                        3,                           "op_a",                                        "a",                                           CAT_FNCT, SLS_ENABLED  },      //JM Operator a
/* 1740 */  { fnJM,                        4,                           "op_a" STD_SUP_2,                              "a" STD_SUP_2,                                 CAT_FNCT, SLS_ENABLED  },      //JM Operator a
/* 1741 */  { fnJM,                        5,                           "op_j",                                        "j",                                           CAT_FNCT, SLS_ENABLED  },      //JM Operator a
/* 1742 */  { fnSetSetJM,                  JC_BASE_HOME,                "SET_BASE_HOME",                               "HOME",                                        CAT_FNCT, SLS_ENABLED  },      //JM eRPN
/* 1743 */  { fnSetSetJM,                  JC_BASE_MYMENU,              "SET_BASE_MYMENU",                             "MYMNU",                                       CAT_FNCT, SLS_ENABLED  },      //JM eRPN
/* 1744 */  { fnSetSetJM,                  JC_BASE_AHOME,               "SET_BASE_AHOME",                              STD_alpha "HOME",                              CAT_FNCT, SLS_ENABLED  },      //JM eRPN
/* 1745 */  { fnSetSetJM,                  JC_BASE_MYA,                 "SET_BASE_MYA",                                "MY" STD_alpha,                                CAT_FNCT, SLS_ENABLED  },      //JM eRPN
/* 1746 */  { fnShowJM,                    JC_BASE_HOME,                "BASE_HOME?",                                  "HOME?",                                       CAT_FNCT, SLS_ENABLED  },      //JM SHOW
/* 1747 */  { fnShowJM,                    JC_BASE_MYMENU,              "BASE_MYMENU?",                                "MYMNU?",                                      CAT_FNCT, SLS_ENABLED  },      //JM SHOW
/* 1748 */  { fnShowJM,                    JC_BASE_AHOME,               "BASE_AHOME?",                                 STD_alpha "HOME?",                             CAT_FNCT, SLS_ENABLED  },      //JM SHOW
/* 1749 */  { fnShowJM,                    JC_BASE_MYA,                 "BASE_MYA?",                                   "MY" STD_alpha "?",                            CAT_FNCT, SLS_ENABLED  },      //JM SHOW
/* 1750 */  { fnJM,                        7,                           "YtoD",                                        "Y" STD_SPACE_3_PER_EM STD_RIGHT_ARROW STD_SPACE_3_PER_EM STD_DELTA,          CAT_FNCT, SLS_ENABLED  },      //JM EE
/* 1751 */  { fnJM,                        6,                           "DtoY",                                        STD_DELTA STD_SPACE_3_PER_EM STD_RIGHT_ARROW STD_SPACE_3_PER_EM "Y",          CAT_FNCT, SLS_ENABLED  },      //JM EE
/* 1752 */  { fnJM,                        9,                           "AtoSYM",                                      STD_RIGHT_ARROW STD_SPACE_3_PER_EM "012",      CAT_FNCT, SLS_ENABLED  },      //JM EE
/* 1753 */  { fnJM,                        8,                           "SYMtoA",                                      STD_RIGHT_ARROW STD_SPACE_3_PER_EM "abc",      CAT_FNCT, SLS_ENABLED  },      //JM EE
/* 1754 */  { itemToBeCoded,               NOPARAM,                     "ELEC.ENG",                                    "ELEC",                                        CAT_MENU, SLS_UNCHANGED},      //JM EE
/* 1755 */  { fnJM,                        10,                          "e^theta_j",                                   "e^" STD_THETA "j",                            CAT_FNCT, SLS_ENABLED  },      //JM EE6
/* 1756 */  { fnJM,                        11,                          "",                                            "STO" STD_SPACE_3_PER_EM "Z",                  CAT_FNCT, SLS_ENABLED  },      //JM EE7
/* 1757 */  { fnJM,                        12,                          "",                                            "RCL" STD_SPACE_3_PER_EM "Z",                  CAT_FNCT, SLS_ENABLED  },      //JM EE8
/* 1758 */  { fnJM,                        13,                          "",                                            "STO" STD_SPACE_3_PER_EM "V",                  CAT_FNCT, SLS_ENABLED  },      //JM EE9
/* 1759 */  { fnJM,                        14,                          "",                                            "RCL" STD_SPACE_3_PER_EM "V",                  CAT_FNCT, SLS_ENABLED  },      //JM EE
/* 1760 */  { fnJM,                        15,                          "",                                            "STO" STD_SPACE_3_PER_EM "I",                  CAT_FNCT, SLS_ENABLED  },      //JM EE
/* 1761 */  { fnJM,                        16,                          "",                                            "RCL" STD_SPACE_3_PER_EM "I",                  CAT_FNCT, SLS_ENABLED  },      //JM EE
/* 1762 */  { fnJM,                        17,                          "",                                            "V" STD_DIVIDE "I",                            CAT_FNCT, SLS_ENABLED  },      //JM EE
/* 1763 */  { fnJM,                        18,                          "",                                            "I" STD_CROSS "Z",                             CAT_FNCT, SLS_ENABLED  },      //JM EE
/* 1764 */  { fnJM,                        19,                          "",                                            "V" STD_DIVIDE "Z",                            CAT_FNCT, SLS_ENABLED  },      //JM EE
/* 1765 */  { fnJM,                        20,                          "",                                            "X" STD_SPACE_3_PER_EM STD_RIGHT_ARROW STD_SPACE_3_PER_EM "BAL",              CAT_FNCT, SLS_ENABLED  },      //JM EE
/* 1766 */  { fnComplexCCCC_CPX,           NOPARAM,                     "COMPLEX",                                     "COMPLEX",                                     CAT_FNCT, SLS_ENABLED  },      //JM Change CC to COMPLEX
/* 1767 */  { fnComplexCCCC_CC1,           NOPARAM,                     "CC1",                                         "CC1",                                         CAT_FNCT, SLS_ENABLED  },      //JM Change CC to CC1
/* 1768 */  { fnJMup,                      NOPARAM,                     "convert Data Types up",                       ">>",                                          CAT_FNCT, SLS_ENABLED  },      //JM TYPE CONVERT
/* 1769 */  { fnJMdown,                    NOPARAM,                     "convert Data Types down",                     "<<",                                          CAT_FNCT, SLS_ENABLED  },      //JM TYPE CONVERT
/* 1770 */  { fnSetSetJM,                  JC_SH_3T,                    "SH.3T",                                       "SH.3T",                                       CAT_FNCT, SLS_ENABLED  },      //JM HOME.3T
/* 1771 */  { itemToBeCoded,               NOPARAM,                     "1771",                                        "1771",                                        CAT_FREE, SLS_UNCHANGED},

/* 1772 */  { itemToBeCoded,               NOPARAM,                     "MENU ASSIGN",                                 "ASN",                                         CAT_MENU, SLS_UNCHANGED},      //JM USER

/* 1773 */  { fnJMUSERmode,                256+  0,                     "KEY  00U",                                    "K_00U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1774 */  { fnJMUSERmode_f,              256+  0,                     "KEY f00U",                                    "Kf00U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1775 */  { fnJMUSERmode_g,              256+  0,                     "KEY g00U",                                    "Kg00U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1776 */  { fnJMUSERmode,                256+  1,                     "KEY  01U",                                    "K_01U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1777 */  { fnJMUSERmode_f,              256+  1,                     "KEY f01U",                                    "Kf01U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1778 */  { fnJMUSERmode_g,              256+  1,                     "KEY g01U",                                    "Kg01U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1779 */  { fnJMUSERmode,                256+  2,                     "KEY  02U",                                    "K_02U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1780 */  { fnJMUSERmode_f,              256+  2,                     "KEY f02U",                                    "Kf02U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1781 */  { fnJMUSERmode_g,              256+  2,                     "KEY g02U",                                    "Kg02U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1782 */  { fnJMUSERmode,                256+  3,                     "KEY  03U",                                    "K_03U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1783 */  { fnJMUSERmode_f,              256+  3,                     "KEY f03U",                                    "Kf03U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1784 */  { fnJMUSERmode_g,              256+  3,                     "KEY g03U",                                    "Kg03U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1785 */  { fnJMUSERmode,                256+  4,                     "KEY  04U",                                    "K_04U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1786 */  { fnJMUSERmode_f,              256+  4,                     "KEY f04U",                                    "Kf04U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1787 */  { fnJMUSERmode_g,              256+  4,                     "KEY g04U",                                    "Kg04U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1788 */  { fnJMUSERmode,                256+  5,                     "KEY  05U",                                    "K_05U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1789 */  { fnJMUSERmode_f,              256+  5,                     "KEY f05U",                                    "Kf05U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1790 */  { fnJMUSERmode_g,              256+  5,                     "KEY g05U",                                    "Kg05U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1791 */  { fnJMUSERmode,                256+  6,                     "KEY  06U",                                    "K_06U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1792 */  { fnJMUSERmode_f,              256+  6,                     "KEY f06U",                                    "Kf06U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1793 */  { fnJMUSERmode_g,              256+  6,                     "KEY g06U",                                    "Kg06U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1794 */  { fnJMUSERmode,                256+  7,                     "KEY  07U",                                    "K_07U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1795 */  { fnJMUSERmode_f,              256+  7,                     "KEY f07U",                                    "Kf07U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1796 */  { fnJMUSERmode_g,              256+  7,                     "KEY g07U",                                    "Kg07U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1797 */  { fnJMUSERmode,                256+  8,                     "KEY  08U",                                    "K_08U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1798 */  { fnJMUSERmode_f,              256+  8,                     "KEY f08U",                                    "Kf08U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1799 */  { fnJMUSERmode_g,              256+  8,                     "KEY g08U",                                    "Kg08U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1800 */  { fnJMUSERmode,                256+  9,                     "KEY  09U",                                    "K_09U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1801 */  { fnJMUSERmode_f,              256+  9,                     "KEY f09U",                                    "Kf09U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1802 */  { fnJMUSERmode_g,              256+  9,                     "KEY g09U",                                    "Kg09U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1803 */  { fnJMUSERmode,                256+ 10,                     "KEY  10U",                                    "K_10U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1804 */  { fnJMUSERmode_f,              256+ 10,                     "KEY f10U",                                    "Kf10U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1805 */  { fnJMUSERmode_g,              256+ 10,                     "KEY g10U",                                    "Kg10U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1806 */  { fnJMUSERmode,                256+ 11,                     "KEY  11U",                                    "K_11U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1807 */  { fnJMUSERmode_f,              256+ 11,                     "KEY f11U",                                    "Kf11U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1808 */  { fnJMUSERmode_g,              256+ 11,                     "KEY g11U",                                    "Kg11U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1809 */  { fnJMUSERmode,                256+ 12,                     "KEY  12U",                                    "K_12U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1810 */  { fnJMUSERmode_f,              256+ 12,                     "KEY f12U",                                    "Kf12U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1811 */  { fnJMUSERmode_g,              256+ 12,                     "KEY g12U",                                    "Kg12U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1812 */  { fnJMUSERmode,                256+ 13,                     "KEY  13U",                                    "K_13U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1813 */  { fnJMUSERmode_f,              256+ 13,                     "KEY f13U",                                    "Kf13U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1814 */  { fnJMUSERmode_g,              256+ 13,                     "KEY g13U",                                    "Kg13U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1815 */  { fnJMUSERmode,                256+ 14,                     "KEY  14U",                                    "K_14U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1816 */  { fnJMUSERmode_f,              256+ 14,                     "KEY f14U",                                    "Kf14U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1817 */  { fnJMUSERmode_g,              256+ 14,                     "KEY g14U",                                    "Kg14U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1818 */  { fnJMUSERmode,                256+ 15,                     "KEY  15U",                                    "K_15U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1819 */  { fnJMUSERmode_f,              256+ 15,                     "KEY f15U",                                    "Kf15U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1820 */  { fnJMUSERmode_g,              256+ 15,                     "KEY g15U",                                    "Kg15U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1821 */  { fnJMUSERmode,                256+ 16,                     "KEY  16U",                                    "K_16U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1822 */  { fnJMUSERmode_f,              256+ 16,                     "KEY f16U",                                    "Kf16U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1823 */  { fnJMUSERmode_g,              256+ 16,                     "KEY g16U",                                    "Kg16U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1824 */  { fnJMUSERmode,                256+ 17,                     "KEY  17U",                                    "K_17U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1825 */  { fnJMUSERmode_f,              256+ 17,                     "KEY f17U",                                    "Kf17U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1826 */  { fnJMUSERmode_g,              256+ 17,                     "KEY g17U",                                    "Kg17U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1827 */  { fnJMUSERmode,                256+ 18,                     "KEY  18U",                                    "K_18U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1828 */  { fnJMUSERmode_f,              256+ 18,                     "KEY f18U",                                    "Kf18U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1829 */  { fnJMUSERmode_g,              256+ 18,                     "KEY g18U",                                    "Kg18U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1830 */  { fnJMUSERmode,                256+ 19,                     "KEY  19U",                                    "K_19U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1831 */  { fnJMUSERmode_f,              256+ 19,                     "KEY f19U",                                    "Kf19U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1832 */  { fnJMUSERmode_g,              256+ 19,                     "KEY g19U",                                    "Kg19U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1833 */  { fnJMUSERmode,                256+ 20,                     "KEY  20U",                                    "K_20U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1834 */  { fnJMUSERmode_f,              256+ 20,                     "KEY f20U",                                    "Kf20U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1835 */  { fnJMUSERmode_g,              256+ 20,                     "KEY g20U",                                    "Kg20U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1836 */  { fnJMUSERmode,                256+ 21,                     "KEY  21U",                                    "K_21U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1837 */  { fnJMUSERmode_f,              256+ 21,                     "KEY f21U",                                    "Kf21U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1838 */  { fnJMUSERmode_g,              256+ 21,                     "KEY g21U",                                    "Kg21U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1839 */  { fnJMUSERmode,                256+ 22,                     "KEY  22U",                                    "K_22U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1840 */  { fnJMUSERmode_f,              256+ 22,                     "KEY f22U",                                    "Kf22U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1841 */  { fnJMUSERmode_g,              256+ 22,                     "KEY g22U",                                    "Kg22U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1842 */  { fnJMUSERmode,                256+ 23,                     "KEY  23U",                                    "K_23U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1843 */  { fnJMUSERmode_f,              256+ 23,                     "KEY f23U",                                    "Kf23U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1844 */  { fnJMUSERmode_g,              256+ 23,                     "KEY g23U",                                    "Kg23U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1845 */  { fnJMUSERmode,                256+ 24,                     "KEY  24U",                                    "K_24U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1846 */  { fnJMUSERmode_f,              256+ 24,                     "KEY f24U",                                    "Kf24U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1847 */  { fnJMUSERmode_g,              256+ 24,                     "KEY g24U",                                    "Kg24U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1848 */  { fnJMUSERmode,                256+ 25,                     "KEY  25U",                                    "K_25U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1849 */  { fnJMUSERmode_f,              256+ 25,                     "KEY f25U",                                    "Kf25U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1850 */  { fnJMUSERmode_g,              256+ 25,                     "KEY g25U",                                    "Kg25U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1851 */  { fnJMUSERmode,                256+ 26,                     "KEY  26U",                                    "K_26U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1852 */  { fnJMUSERmode_f,              256+ 26,                     "KEY f26U",                                    "Kf26U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1853 */  { fnJMUSERmode_g,              256+ 26,                     "KEY g26U",                                    "Kg26U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1854 */  { fnJMUSERmode,                256+ 27,                     "KEY  27U",                                    "K_27U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1855 */  { fnJMUSERmode_f,              256+ 27,                     "KEY f27U",                                    "Kf27U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1856 */  { fnJMUSERmode_g,              256+ 27,                     "KEY g27U",                                    "Kg27U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1857 */  { fnJMUSERmode,                256+ 28,                     "KEY  28U",                                    "K_28U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1858 */  { fnJMUSERmode_f,              256+ 28,                     "KEY f28U",                                    "Kf28U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1859 */  { fnJMUSERmode_g,              256+ 28,                     "KEY g28U",                                    "Kg28U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1860 */  { fnJMUSERmode,                256+ 29,                     "KEY  29U",                                    "K_29U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1861 */  { fnJMUSERmode_f,              256+ 29,                     "KEY f29U",                                    "Kf29U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1862 */  { fnJMUSERmode_g,              256+ 29,                     "KEY g29U",                                    "Kg29U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1863 */  { fnJMUSERmode,                256+ 30,                     "KEY  30U",                                    "K_30U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1864 */  { fnJMUSERmode_f,              256+ 30,                     "KEY f30U",                                    "Kf30U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1865 */  { fnJMUSERmode_g,              256+ 30,                     "KEY g30U",                                    "Kg30U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1866 */  { fnJMUSERmode,                256+ 31,                     "KEY  31U",                                    "K_31U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1867 */  { fnJMUSERmode_f,              256+ 31,                     "KEY f31U",                                    "Kf31U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1868 */  { fnJMUSERmode_g,              256+ 31,                     "KEY g31U",                                    "Kg31U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1869 */  { fnJMUSERmode,                256+ 32,                     "KEY  32U",                                    "K_32U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1870 */  { fnJMUSERmode_f,              256+ 32,                     "KEY f32U",                                    "Kf32U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1871 */  { fnJMUSERmode_g,              256+ 32,                     "KEY g32U",                                    "Kg32U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1872 */  { fnJMUSERmode,                256+ 33,                     "KEY  33U",                                    "K_33U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1873 */  { fnJMUSERmode_f,              256+ 33,                     "KEY f33U",                                    "Kf33U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1874 */  { fnJMUSERmode_g,              256+ 33,                     "KEY g33U",                                    "Kg33U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1875 */  { fnJMUSERmode,                256+ 34,                     "KEY  34U",                                    "K_34U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1876 */  { fnJMUSERmode_f,              256+ 34,                     "KEY f34U",                                    "Kf34U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1877 */  { fnJMUSERmode_g,              256+ 34,                     "KEY g34U",                                    "Kg34U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1878 */  { fnJMUSERmode,                256+ 35,                     "KEY  35U",                                    "K_35U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1879 */  { fnJMUSERmode_f,              256+ 35,                     "KEY f35U",                                    "Kf35U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1880 */  { fnJMUSERmode_g,              256+ 35,                     "KEY g35U",                                    "Kg35U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1881 */  { fnJMUSERmode,                256+ 36,                     "KEY  36U",                                    "K_36U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1882 */  { fnJMUSERmode_f,              256+ 36,                     "KEY f36U",                                    "Kf36U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST
/* 1883 */  { fnJMUSERmode_g,              256+ 36,                     "KEY g36U",                                    "Kg36U",                                       CAT_NONE, SLS_UNCHANGED},  //JM USER MODE TEST

/* 1884 */  { itemToBeCoded,               NOPARAM      ,               "",                                            "f/g",                                         CAT_NONE, SLS_UNCHANGED},      //JM3 Changed f to f/g for DM42 keyboard
/* 1885 */  { fnUserJM,                    USER_DEFAULTS,               "",                                            "U" STD_SIGMA    " CC",                        CAT_NONE, SLS_UNCHANGED},      //JM USER
/* 1886 */  { fnUserJM,                    USER_COMPLEX ,               "",                                            "U" STD_UP_ARROW " CC",                        CAT_NONE, SLS_UNCHANGED},      //JM USER
/* 1887 */  { fnUserJM,                    USER_SHIFTS  ,               "",                                            "U" " SHFT",                                   CAT_NONE, SLS_UNCHANGED},      //JM USER
/* 1888 */  { fnUserJM,                    USER_RESET   ,               "",                                            "U" " RSET",                                   CAT_NONE, SLS_UNCHANGED},      //JM USER
/* 1889 */  { fnSigmaAssign,               KEY_USERMODE ,               "",                                            STD_SIGMA "+USR",                              CAT_NONE, SLS_UNCHANGED},      //JM USER
/* 1890 */  { itemToBeCoded,               NOPARAM      ,               "1890",                                        "1890",                                        CAT_FREE, SLS_UNCHANGED},
/* 1891 */  { fnSigmaAssign,               KEY_CC       ,               "",                                            STD_SIGMA "+CC",                               CAT_NONE, SLS_UNCHANGED},      //JM USER
/* 1892 */  { itemToBeCoded,               NOPARAM      ,               "1892",                                        "1892",                                        CAT_FREE, SLS_UNCHANGED},
/* 1893 */  { fnSigmaAssign,               -MNU_MyMenu  ,               "",                                            STD_SIGMA "+MyM",                              CAT_NONE, SLS_UNCHANGED},      //JM USER
/* 1894 */  { itemToBeCoded,               NOPARAM      ,               "1894",                                        "1894",                                        CAT_FREE, SLS_UNCHANGED},
/* 1895 */  { fnSigmaAssign,               ITM_SIGMAPLUS,               "",                                            STD_SIGMA "+",                                 CAT_NONE, SLS_UNCHANGED},      //JM USER
/* 1896 */  { itemToBeCoded,               NOPARAM      ,               "1896",                                        "1896",                                        CAT_FREE, SLS_UNCHANGED},
/* 1897 */  { fnSigmaAssign,               ITM_PR       ,               "",                                            STD_SIGMA "+PGM",                              CAT_NONE, SLS_UNCHANGED},      //JM USER
/* 1898 */  { itemToBeCoded,               NOPARAM      ,               "1898",                                        "1898",                                        CAT_FREE, SLS_UNCHANGED},
/* 1899 */  { fnSigmaAssign,               ITM_AIM      ,               "",                                            STD_SIGMA "+" STD_alpha,                       CAT_NONE, SLS_UNCHANGED},      //JM USER
/* 1900 */  { fnGetSigmaAssignToX,         NOPARAM      ,               "",                                            STD_SIGMA "+ toX",                             CAT_NONE, SLS_ENABLED  },      //JM USER NORMAL
/* 1901 */  { fnUserJM,                    JM_ASSIGN    ,               "",                                            "ASSIGN",                                      CAT_NONE, SLS_UNCHANGED},      //JM ASSIGN
/* 1902 */  { fnUserJM,                    JM_SEEK_FN   ,               "",                                            "FN" STD_DOT "ASN",                            CAT_NONE, SLS_UNCHANGED},      //JM ASSIGN SEEK FUNCTION
/* 1903 */  { itemToBeCoded,               NOPARAM      ,               "",                                            STD_SIGMA "+ ASN",                             CAT_NONE, SLS_UNCHANGED},      //JM USER NORMAL
/* 1904 */  { fnJM_GetXToNORMmode,         NOPARAM      ,               "",                                            "X to"STD_SIGMA "+",                           CAT_NONE, SLS_ENABLED  },      //JM USER NORMAL
/* 1905 */  { fnInDefault,                 ID_43S       ,               "",                                            "i" STD_SPACE_3_PER_EM "LI/RL",                CAT_NONE, SLS_UNCHANGED},      //JM INPUT DEFAULT
/* 1906 */  { fnSetSetJM,                  JC_FG_LINE   ,               "",                                            "FG" STD_SPACE_3_PER_EM "LINE",                CAT_NONE, SLS_UNCHANGED},      //JM INPUT DEFAULT
/* 1907 */  { fnInDefault,                 ID_DP        ,               "",                                            "i" STD_SPACE_3_PER_EM "REAL",                 CAT_NONE, SLS_UNCHANGED},      //JM INPUT DEFAULT
/* 1908 */  { fnShowJM,                    JM_INP_DFLT  ,               "",                                            "i" STD_SPACE_3_PER_EM "Dflt?",                CAT_NONE, SLS_UNCHANGED},      //JM INPUT DEFAULT
/* 1909 */  { fnSetSetJM,                  JC_FG_DOTS   ,               "",                                            "FG" STD_SPACE_3_PER_EM "DOTS",                CAT_NONE, SLS_UNCHANGED},      //JM INPUT DEFAULT
/* 1910 */  { fnInDefault,                 ID_CPXDP     ,               "",                                            "i" STD_SPACE_3_PER_EM "CPX",                  CAT_NONE, SLS_UNCHANGED},      //JM INPUT DEFAULT
/* 1911 */  { fnSetSetJM,                  JC_G_DOUBLETAP,              "",                                            "G" STD_SPACE_3_PER_EM "2TAP",                 CAT_NONE, SLS_UNCHANGED},      //JM INPUT DEFAULT
/* 1912 */  { fnInDefault,                 ID_SI        ,               "",                                            "i" STD_SPACE_3_PER_EM "SI",                   CAT_NONE, SLS_UNCHANGED},      //JM INPUT DEFAULT
/* 1913 */  { fnInDefault,                 ID_LI        ,               "",                                            "i" STD_SPACE_3_PER_EM "LI",                   CAT_NONE, SLS_UNCHANGED},      //JM INPUT DEFAULT
/* 1914 */  { graph,                       NOPARAM      ,               "",                                            "GRAPH",                                       CAT_FNCT, SLS_UNCHANGED},      //JM GRAPHING
/* 1915 */  { itemToBeCoded,               NOPARAM,                     "A" STD_ELLIPSIS "Z",                          "A" STD_ELLIPSIS "Z",                          CAT_MENU, SLS_UNCHANGED},      // Ex 34
/* 1916 */  { itemToBeCoded,               NOPARAM,                     "a" STD_ELLIPSIS "z",                          "a" STD_ELLIPSIS "z",                          CAT_MENU, SLS_UNCHANGED},      // Ex 875 Small latin letters

/* 1917 */  { itemToBeCoded,               NOPARAM,                     "",                                            "Last item",                                   CAT_NONE, SLS_UNCHANGED}       //JM
};
