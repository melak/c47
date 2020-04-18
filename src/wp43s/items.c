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

  if(func >= LAST_ITEM) {
    #ifdef PC_BUILD
      sprintf(errorMessage, "item (%" FMT16S ") must be below LAST_ITEM", func);
      showInfoDialog("In function runFunction:", errorMessage, NULL, NULL);
    #endif
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
    else if(func == ITM_CNST) {
      tamNumberMax = NUMBER_OF_CONSTANTS_39 + NUMBER_OF_CONSTANTS_51 + NUMBER_OF_CONSTANTS_1071 + NUMBER_OF_CONSTANTS_34;
    }
    else if(func == ITM_ASR || func == ITM_SL || func == ITM_SR || func == ITM_RL || func == ITM_RR || func == ITM_RLC || func == ITM_RRC) {
      tamNumberMax = 63; // limit to (shortIntegerWordSize - 1)?
    }
    else if(func == ITM_MASKL || func == ITM_MASKR) {
      tamNumberMax = 64; // limit to shortIntegerWordSize?
    }
    else if(func == ITM_CB || func == ITM_SB || func == ITM_FB || func == ITM_BC || func == ITM_BS) {
      tamNumberMin = 1;
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
    //printf("Forced ERPN items.c\n");
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
void registerBrowser            (uint16_t unusedParamButMandatory) {}
void flagBrowser                (uint16_t unusedParamButMandatory) {}
void fontBrowser                (uint16_t unusedParamButMandatory) {}
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
void fnSinc                     (uint16_t unusedParamButMandatory) {}
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
void fnKeyEnter                 (uint16_t unusedParamButMandatory) {}
void fnKeyExit                  (uint16_t unusedParamButMandatory) {}
void fnKeyUp                    (uint16_t unusedParamButMandatory) {}
void fnKeyDown                  (uint16_t unusedParamButMandatory) {}
void fnKeyDotD                  (uint16_t unusedParamButMandatory) {}
void fnKeyCC                    (uint16_t unusedParamButMandatory) {}
void fnKeyBackspace             (uint16_t unusedParamButMandatory) {}
void fnDisplayStack             (uint16_t unusedParamButMandatory) {}
void fnFreeFlashMemory          (uint16_t unusedParamButMandatory) {}
void fnFreeMemory               (uint16_t unusedParamButMandatory) {}
void fnFp                       (uint16_t unusedParamButMandatory) {}
void fnIp                       (uint16_t unusedParamButMandatory) {}
void allocateLocalRegisters     (uint16_t unusedParamButMandatory) {}
void fnLeadingZeros             (uint16_t unusedParamButMandatory) {}
void fnNeighb                   (uint16_t unusedParamButMandatory) {}
void fnGcd                      (uint16_t unusedParamButMandatory) {}
void fnMin                      (uint16_t unusedParamButMandatory) {}
void fnMax                      (uint16_t unusedParamButMandatory) {}
void fnStatSum                  (uint16_t unusedParamButMandatory) {}
void fnIsPrime                  (uint16_t unusedParamButMandatory) {}
void fnRandom                   (uint16_t unusedParamButMandatory) {}
void fnRandomI                  (uint16_t unusedParamButMandatory) {}
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
void fnToggleFractionType       (uint16_t unusedParamButMandatory) {}
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
void fnShow                     (uint16_t unusedParamButMandatory) {}
void fnLastX                    (uint16_t unusedParamButMandatory) {}
void fnCyx                      (uint16_t unusedParamButMandatory) {}
void fnPyx                      (uint16_t unusedParamButMandatory) {}
void fnToReal                   (uint16_t unusedParamButMandatory) {}
void fnDec                      (uint16_t unusedParamButMandatory) {}
void fnInc                      (uint16_t unusedParamButMandatory) {}
void fncountBits                (uint16_t unusedParamButMandatory) {}
void fnLogicalNot               (uint16_t unusedParamButMandatory) {}
void fnLogicalAnd               (uint16_t unusedParamButMandatory) {}
void fnLogicalNand              (uint16_t unusedParamButMandatory) {}
void fnLogicalOr                (uint16_t unusedParamButMandatory) {}
void fnLogicalNor               (uint16_t unusedParamButMandatory) {}
void fnLogicalXor               (uint16_t unusedParamButMandatory) {}
void fnLogicalXnor              (uint16_t unusedParamButMandatory) {}
void fnDecomp                   (uint16_t unusedParamButMandatory) {}
void fnSumXY                    (uint16_t unusedParamButMandatory) {}
void fnMeanXY                   (uint16_t unusedParamButMandatory) {}
void fnGeometricMeanXY          (uint16_t unusedParamButMandatory) {}
void fnWeightedMeanX            (uint16_t unusedParamButMandatory) {}
void fnHarmonicMeanXY           (uint16_t unusedParamButMandatory) {}
void fnRMSMeanXY                (uint16_t unusedParamButMandatory) {}
void fnMaskl                    (uint16_t unusedParamButMandatory) {}
void fnMaskr                    (uint16_t unusedParamButMandatory) {}
void fnAsr                      (uint16_t unusedParamButMandatory) {}
void fnCb                       (uint16_t unusedParamButMandatory) {}
void fnSb                       (uint16_t unusedParamButMandatory) {}
void fnFb                       (uint16_t unusedParamButMandatory) {}
void fnBs                       (uint16_t unusedParamButMandatory) {}
void fnBc                       (uint16_t unusedParamButMandatory) {}
void fnSl                       (uint16_t unusedParamButMandatory) {}
void fnRl                       (uint16_t unusedParamButMandatory) {}
void fnRlc                      (uint16_t unusedParamButMandatory) {}
void fnSr                       (uint16_t unusedParamButMandatory) {}
void fnRr                       (uint16_t unusedParamButMandatory) {}
void fnRrc                      (uint16_t unusedParamButMandatory) {}
void fnLj                       (uint16_t unusedParamButMandatory) {}
void fnRj                       (uint16_t unusedParamButMandatory) {}
void fnCountBits                (uint16_t unusedParamButMandatory) {}
void fnNextPrime                (uint16_t unusedParamButMandatory) {}
void fnScreenDump               (uint16_t unusedParamButMandatory) {}
void fnArg                      (uint16_t unusedParamButMandatory) {}
void fnRange                    (uint16_t unusedParamButMandatory) {}
void fnGetRange                 (uint16_t unusedParamButMandatory) {}
void fnDot                      (uint16_t unusedParamButMandatory) {}
void fnCross                    (uint16_t unusedParamButMandatory) {}
void fnPercent                  (uint16_t unusedParamButMandatory) {}
void fnPercentMRR               (uint16_t unusedParamButMandatory) {}
void fnPercentT                 (uint16_t unusedParamButMandatory) {}
void fnPercentSigma             (uint16_t unusedParamButMandatory) {}
void fnPercentPlusMG            (uint16_t unusedParamButMandatory) {}
void fnDeltaPercent             (uint16_t unusedParamButMandatory) {}
void fnJM                       (uint16_t unusedParamButMandatory) {}           //vv JM
//void fnComplexCCCC_CPX          (uint16_t unusedParamButMandatory) {}
//void fnComplexCCCC_CC           (uint16_t unusedParamButMandatory) {}
//void fnComplexCCCC_CC1          (uint16_t unusedParamButMandatory) {}
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
void fnGraph                    (uint16_t unusedParamButMandatory) {}
void fnJM_2SI                   (uint16_t unusedParamButMandatory) {}
void fnJM_ms                    (uint16_t unusedParamButMandatory) {}
void fnShow_SCROLL              (uint16_t unusedParamButMandatory) {}
void fnKeyDotDjm                (uint16_t unusedParamButMandatory) {}
void fnP_All_Regs               (uint16_t unusedParamButMandatory) {}
void fnJM_fnToPolar             (uint16_t unusedParamButMandatory) {}
void fnJM_fnToRect              (uint16_t unusedParamButMandatory) {}          //^^
#endif

const item_t indexOfItems[] = {
//            *func                        param                        itemCatalogName (also FN DISPLAY (NOP))        itemSoftmenuName                               catalog.  stackLiftStatus   //JM
//            function                     parameter                    item in catalog                                item in softmenu                               CATALOG   stackLift
/*    0 */  { itemToBeCoded,               NOPARAM,                     "",                                            "0000",                                        CAT_NONE, SLS_UNCHANGED},
/*    1 */  { fnCvtCToF,                   NOPARAM,                     STD_DEGREE "C" STD_RIGHT_ARROW STD_DEGREE "F", STD_DEGREE "C" STD_RIGHT_ARROW STD_DEGREE "F", CAT_FNCT, SLS_ENABLED  },
/*    2 */  { fnCvtFToC,                   NOPARAM,                     STD_DEGREE "F" STD_RIGHT_ARROW STD_DEGREE "C", STD_DEGREE "F" STD_RIGHT_ARROW STD_DEGREE "C", CAT_FNCT, SLS_ENABLED  },
/*    3 */  { fn10Pow,                     NOPARAM,                     "10" STD_SUP_x,                                "10" STD_SUP_x,                                CAT_FNCT, SLS_ENABLED  },
/*    4 */  { fnIntegerMode,               SIM_1COMPL,                  "1COMPL",                                      "1COMPL",                                      CAT_FNCT, SLS_UNCHANGED},
/*    5 */  { fnScreenDump,                NOPARAM,                     "SNAP",                                        "SNAP",                                        CAT_FNCT, SLS_UNCHANGED},
/*    6 */  { fnInvert,                    NOPARAM,                     "1/x",                                         "1/x",                                         CAT_FNCT, SLS_ENABLED  },
/*    7 */  { fnIntegerMode,               SIM_2COMPL,                  "2COMPL",                                      "2COMPL",                                      CAT_FNCT, SLS_UNCHANGED},
/*    8 */  { fn2Pow,                      NOPARAM,                     "2" STD_SUP_x,                                 "2" STD_SUP_x,                                 CAT_FNCT, SLS_ENABLED  },
/*    9 */  { fnCubeRoot,                  NOPARAM,                     STD_CUBE_ROOT STD_x_UNDER_ROOT,                STD_CUBE_ROOT STD_x_UNDER_ROOT,                CAT_FNCT, SLS_ENABLED  },
/*   10 */  { itemToBeCoded,               NOPARAM,                     "A",                                           "A",                                           CAT_RVAR, SLS_UNCHANGED},
/*   11 */  { fnConstant,                  0,                           "a",                                           "a",                                           CAT_CNST, SLS_ENABLED  },
/*   12 */  { fnConstant,                  1,                           "a" STD_SUB_0,                                 "a" STD_SUB_0,                                 CAT_CNST, SLS_ENABLED  },
/*   13 */  { fnMagnitude,                 NOPARAM,                     "ABS",                                         "ABS",                                         CAT_FNCT, SLS_ENABLED  },
/*   14 */  { itemToBeCoded,               NOPARAM,                     "ACC",                                         "ACC",                                         CAT_RVAR, SLS_UNCHANGED},
/*   15 */  { fnCvtAcreM2,                 multiply,                    "ac" STD_RIGHT_ARROW "m" STD_SUP_2,            "acre",                                        CAT_FNCT, SLS_ENABLED  },
/*   16 */  { fnCvtAcreusM2,               multiply,                    "ac" STD_US STD_RIGHT_ARROW "m" STD_SUP_2,     "acre" STD_US,                                 CAT_FNCT, SLS_ENABLED  },
/*   17 */  { itemToBeCoded,               NOPARAM,                     "ADV",                                         "ADV",                                         CAT_MENU, SLS_UNCHANGED},
/*   18 */  { fnAgm,                       NOPARAM,                     "AGM",                                         "AGM",                                         CAT_FNCT, SLS_ENABLED  },
/*   19 */  { itemToBeCoded,               NOPARAM,                     "AGRAPH",                                      "AGRAPH",                                      CAT_FNCT, SLS_UNCHANGED},
/*   20 */  { fnDisplayFormatAll,          TM_VALUE,                    "ALL" ,                                        "ALL",                                         CAT_FNCT, SLS_UNCHANGED},
/*   21 */  { fnConstant,                  2,                           "a" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "a" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   CAT_CNST, SLS_ENABLED  },
/*   22 */  { fnLogicalAnd,                NOPARAM,                     "AND",                                         "AND",                                         CAT_FNCT, SLS_ENABLED  },
/*   23 */  { itemToBeCoded,               NOPARAM,                     "ANGLES",                                      "ANGLES",                                      CAT_MENU, SLS_UNCHANGED},
/*   24 */  { fnArccos,                    NOPARAM,                     "ARCCOS",                                      "ACOS",                                        CAT_FNCT, SLS_ENABLED  },   //JM
/*   25 */  { fnArccosh,                   NOPARAM,                     "arcosh",                                      "arcosh",                                      CAT_FNCT, SLS_ENABLED  },
/*   26 */  { fnArcsin,                    NOPARAM,                     "ARCSIN",                                      "ASIN",                                        CAT_FNCT, SLS_ENABLED  },   //JM
/*   27 */  { fnArctan,                    NOPARAM,                     "ARCTAN",                                      "ATAN",                                        CAT_FNCT, SLS_ENABLED  },   //JM
/*   28 */  { fnArcsinh,                   NOPARAM,                     "arsinh",                                      "arsinh",                                      CAT_FNCT, SLS_ENABLED  },
/*   29 */  { fnArctanh,                   NOPARAM,                     "artanh",                                      "artanh",                                      CAT_FNCT, SLS_ENABLED  },
/*   30 */  { fnAsr,                       TM_VALUE,                    "ASR",                                         "ASR",                                         CAT_FNCT, SLS_ENABLED  },
/*   31 */  { itemToBeCoded,               NOPARAM,                     "ASN",                                         "ASN",                                         CAT_FNCT, SLS_UNCHANGED},
/*   32 */  { fnCvtAtmPa,                  multiply,                    "atm" STD_RIGHT_ARROW "Pa",                    "atm" STD_RIGHT_ARROW "Pa",                    CAT_FNCT, SLS_ENABLED  },
/*   33 */  { fnCvtAuM,                    multiply,                    "au" STD_RIGHT_ARROW "m",                      "au" STD_RIGHT_ARROW "m",                      CAT_FNCT, SLS_ENABLED  },
/*   34 */  { itemToBeCoded,               NOPARAM,                     "PRINT",                                       STD_PRINTER,                                   CAT_MENU, SLS_UNCHANGED},
/*   35 */  { itemToBeCoded,               NOPARAM,                     "Area:",                                       "Area:",                                       CAT_MENU, SLS_UNCHANGED},
/*   36 */  { fnConstant,                  3,                           "a" STD_SUB_EARTH,                             "a" STD_SUB_EARTH,                             CAT_CNST, SLS_ENABLED  },
/*   37 */  { itemToBeCoded,               NOPARAM,                     "B",                                           "B",                                           CAT_RVAR, SLS_UNCHANGED},
/*   38 */  { itemToBeCoded,               NOPARAM,                     "BACK",                                        "BACK",                                        CAT_FNCT, SLS_UNCHANGED},
/*   39 */  { fnCvtBarPa,                  multiply,                    "bar" STD_RIGHT_ARROW "Pa",                    "bar" STD_RIGHT_ARROW "Pa",                    CAT_FNCT, SLS_ENABLED  },
/*   40 */  { fnBatteryVoltage,            NOPARAM,                     "BATT?",                                       "BATT?",                                       CAT_FNCT, SLS_ENABLED  },
/*   41 */  { fnBc,                        TM_VALUE,                    "BC?",                                         "BC?",                                         CAT_FNCT, SLS_ENABLED  },
/*   42 */  { itemToBeCoded,               NOPARAM,                     "BEEP",                                        "BEEP",                                        CAT_FNCT, SLS_UNCHANGED},
/*   43 */  { itemToBeCoded,               NOPARAM,                     "BeginP",                                      "Begin",                                       CAT_FNCT, SLS_UNCHANGED},
/*   44 */  { fnCurveFitting,              CF_BEST_FITTING,             "BestF",                                       "BestF",                                       CAT_FNCT, SLS_UNCHANGED},
/*   45 */  { itemToBeCoded,               NOPARAM,                     "Binom" STD_SUB_p,                             "Binom" STD_SUB_p,                             CAT_FNCT, SLS_UNCHANGED},
/*   46 */  { itemToBeCoded,               NOPARAM,                     "Binom" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Binom" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   CAT_FNCT, SLS_UNCHANGED},
/*   47 */  { itemToBeCoded,               NOPARAM,                     "Binom" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Binom" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   CAT_FNCT, SLS_UNCHANGED},
/*   48 */  { itemToBeCoded,               NOPARAM,                     "Binom" STD_SUP_MINUS_1,                       "Binom" STD_SUP_MINUS_1,                       CAT_FNCT, SLS_UNCHANGED},
/*   49 */  { itemToBeCoded,               NOPARAM,                     "Binom:",                                      "Binom:",                                      CAT_MENU, SLS_UNCHANGED},
/*   50 */  { itemToBeCoded,               NOPARAM,                     "BITS",                                        "BITS",                                        CAT_MENU, SLS_UNCHANGED},
/*   51 */  { itemToBeCoded,               NOPARAM,                     "B" STD_SUB_n,                                 "B" STD_SUB_n,                                 CAT_FNCT, SLS_UNCHANGED},
/*   52 */  { itemToBeCoded,               NOPARAM,                     "B" STD_SUB_n STD_SUP_ASTERISK,                "B" STD_SUB_n STD_SUP_ASTERISK,                CAT_FNCT, SLS_UNCHANGED},
/*   53 */  { fnBs,                        TM_VALUE,                    "BS?",                                         "BS?",                                         CAT_FNCT, SLS_ENABLED  },
/*   54 */  { fnCvtBtuJ,                   multiply,                    "Btu" STD_RIGHT_ARROW "J",                     "Btu" STD_RIGHT_ARROW "J",                     CAT_FNCT, SLS_ENABLED  },
/*   55 */  { itemToBeCoded,               NOPARAM,                     "C",                                           "C",                                           CAT_RVAR, SLS_UNCHANGED},
/*   56 */  { fnConstant,                  4,                           "c",                                           "c",                                           CAT_CNST, SLS_ENABLED  },
/*   57 */  { fnConstant,                  5,                           "c" STD_SUB_1,                                 "c" STD_SUB_1,                                 CAT_CNST, SLS_ENABLED  },
/*   58 */  { fnConstant,                  6,                           "c" STD_SUB_2,                                 "c" STD_SUB_2,                                 CAT_CNST, SLS_ENABLED  },
/*   59 */  { fnCvtCalJ,                   multiply,                    "cal" STD_RIGHT_ARROW "J",                     "cal" STD_RIGHT_ARROW "J",                     CAT_FNCT, SLS_ENABLED  },
/*   60 */  { itemToBeCoded,               NOPARAM,                     "CASE",                                        "CASE",                                        CAT_FNCT, SLS_UNCHANGED},
/*   61 */  { itemToBeCoded,               NOPARAM,                     "CATALOG",                                     "CAT",                                         CAT_MENU, SLS_UNCHANGED},   // JM
/*   62 */  { itemToBeCoded,               NOPARAM,                     "Cauch" STD_SUB_p,                             "Cauch" STD_SUB_p,                             CAT_FNCT, SLS_UNCHANGED},
/*   63 */  { itemToBeCoded,               NOPARAM,                     "Cauch" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Cauch" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   CAT_FNCT, SLS_UNCHANGED},
/*   64 */  { itemToBeCoded,               NOPARAM,                     "Cauch" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Cauch" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   CAT_FNCT, SLS_UNCHANGED},
/*   65 */  { itemToBeCoded,               NOPARAM,                     "Cauch" STD_SUP_MINUS_1,                       "Cauch" STD_SUP_MINUS_1,                       CAT_FNCT, SLS_UNCHANGED},
/*   66 */  { itemToBeCoded,               NOPARAM,                     "Cauch:",                                      "Cauch:",                                      CAT_MENU, SLS_UNCHANGED},
/*   67 */  { fnCb,                        TM_VALUE,                    "CB",                                          "CB",                                          CAT_FNCT, SLS_ENABLED  },
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
/*   86 */  { itemToBeCoded,               NOPARAM,                     "CNST",                                        "CNST",                                        CAT_MENU, SLS_UNCHANGED},   //JM Keeps the same. Don't havce space for more on kjeyplate
/*   87 */  { fnCyx,                       NOPARAM,                     "COMB",                                        "C" STD_SUB_y STD_SUB_x,                       CAT_FNCT, SLS_ENABLED  },
/*   88 */  { fnConjugate,                 NOPARAM,                     "CONJ",                                        "conj",                                        CAT_FNCT, SLS_ENABLED  },
/*   89 */  { fnConstant,                  TM_VALUE,                    "CNST",                                        "CNST",                                        CAT_FNCT, SLS_ENABLED  },   //JM RENAMED
/*   90 */  { itemToBeCoded,               NOPARAM,                     "CONVG?",                                      "CONVG?",                                      CAT_FNCT, SLS_UNCHANGED},
/*   91 */  { itemToBeCoded,               NOPARAM,                     "CORR",                                        "r",                                           CAT_FNCT, SLS_UNCHANGED},
/*   92 */  { fnCos,                       NOPARAM,                     "COS",                                         "COS",                                         CAT_FNCT, SLS_ENABLED  },   //JM
/*   93 */  { fnCosh,                      NOPARAM,                     "cosh",                                        "cosh",                                        CAT_FNCT, SLS_ENABLED  },
/*   94 */  { itemToBeCoded,               NOPARAM,                     "COV",                                         "cov",                                         CAT_FNCT, SLS_UNCHANGED},
/*   95 */  { itemToBeCoded,               NOPARAM,                     "CPX",                                         "CPX",                                         CAT_MENU, SLS_UNCHANGED},
/*   96 */  { fnComplexUnit,               CU_I,                        "CPXi",                                        "CPXi",                                        CAT_FNCT, SLS_UNCHANGED},
/*   97 */  { fnComplexUnit,               CU_J,                        "CPXj",                                        "CPXj",                                        CAT_FNCT, SLS_UNCHANGED},
/*   98 */  { fnComplexResult,             true,                        "CPXRES",                                      "CPXRES",                                      CAT_NONE, SLS_UNCHANGED},   //dr
/*   99 */  { itemToBeCoded,               NOPARAM,                     "CPXS",                                        "CPXS",                                        CAT_MENU, SLS_UNCHANGED},
/*  100 */  { itemToBeCoded,               NOPARAM,                     "CPX?",                                        "CPX?",                                        CAT_FNCT, SLS_UNCHANGED},
/*  101 */  { fnCross,                     NOPARAM,                     "CROSS",                                       "cross",                                       CAT_FNCT, SLS_ENABLED  },
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
/*  115 */  { fnDec,                       TM_REGISTER,                 "DEC",                                         "DEC",                                         CAT_FNCT, SLS_ENABLED  },
/*  116 */  { fnDecomp,                    NOPARAM,                     "DECOMP",                                      "DECOMP",                                      CAT_FNCT, SLS_ENABLED  },
/*  117 */  { fnAngularMode,               AM_DEGREE,                   "DEG",                                         "DEG",                                         CAT_FNCT, SLS_UNCHANGED},
/*  118 */  { fnCvtToCurrentAngularMode,   AM_DEGREE,                   "DEG" STD_RIGHT_ARROW,                         "DEG" STD_RIGHT_ARROW,                         CAT_FNCT, SLS_ENABLED  },
/*  119 */  { fnDenMode,                   DM_ANY,                      "DENANY",                                      "DENANY",                                      CAT_FNCT, SLS_UNCHANGED},
/*  120 */  { fnDenMode,                   DM_FAC,                      "DENFAC",                                      "DENFAC",                                      CAT_FNCT, SLS_UNCHANGED},
/*  121 */  { fnDenMode,                   DM_FIX,                      "DENFIX",                                      "DENFIX",                                      CAT_FNCT, SLS_UNCHANGED},
/*  122 */  { fnDenMax,                    NOPARAM,                     "DENMAX",                                      "DENMAX",                                      CAT_FNCT, SLS_UNCHANGED},
/*  123 */  { itemToBeCoded,               NOPARAM,                     "DIGITS",                                      "DIGITS",                                      CAT_MENU, SLS_UNCHANGED},
/*  124 */  { itemToBeCoded,               NOPARAM,                     "DISP",                                        "DISP",                                        CAT_MENU, SLS_UNCHANGED},
/*  125 */  { fnDot,                       NOPARAM,                     "DOT",                                         "dot",                                         CAT_FNCT, SLS_ENABLED  },
/*  126 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "DP",                          STD_RIGHT_ARROW "DP",                          CAT_FNCT, SLS_ENABLED  },
/*  127 */  { fnDrop,                      NOPARAM,                     "DROP",                                        "DROP" STD_DOWN_ARROW,                         CAT_FNCT, SLS_ENABLED  },
/*  128 */  { fnDropY,                     NOPARAM,                     "DROPy",                                       "DROPy",                                       CAT_FNCT, SLS_ENABLED  },
/*  129 */  { itemToBeCoded,               NOPARAM,                     "DSE",                                         "DSE",                                         CAT_FNCT, SLS_UNCHANGED},
/*  130 */  { itemToBeCoded,               NOPARAM,                     "DSL",                                         "DSL",                                         CAT_FNCT, SLS_UNCHANGED},
/*  131 */  { fnConstant,                  59,                          STD_DELTA STD_nu STD_SUB_C STD_SUB_s,          STD_DELTA STD_nu STD_SUB_C STD_SUB_s,          CAT_CNST, SLS_ENABLED  },
/*  132 */  { fnDisplayStack,              TM_VALUE,                    "DSTACK",                                      "DSTACK",                                      CAT_FNCT, SLS_UNCHANGED},
/*  133 */  { itemToBeCoded,               NOPARAM,                     "DSZ",                                         "DSZ",                                         CAT_FNCT, SLS_UNCHANGED},
/*  134 */  { fnAngularMode,               AM_DMS,                      "D.MS",                                        "d.ms",                                        CAT_FNCT, SLS_UNCHANGED},
/*  135 */  { fnCvtToCurrentAngularMode,   AM_DMS,                      "D.MS" STD_RIGHT_ARROW,                        "D.MS" STD_RIGHT_ARROW,                        CAT_FNCT, SLS_ENABLED  },
/*  136 */  { fnDateFormat,                DF_DMY,                      "D.MY",                                        "D.MY",                                        CAT_FNCT, SLS_UNCHANGED},
/*  137 */  { itemToBeCoded,               NOPARAM,                     "D" STD_RIGHT_ARROW "J",                       "D" STD_RIGHT_ARROW "J",                       CAT_FNCT, SLS_UNCHANGED},
/*  138 */  { fnCvtDegToRad,               NOPARAM,                     "D" STD_RIGHT_ARROW "R",                       "D" STD_RIGHT_ARROW "R",                       CAT_FNCT, SLS_ENABLED  },
/*  139 */  { fnConstant,                  7,                           "e",                                           "e",                                           CAT_CNST, SLS_ENABLED  },
/*  140 */  { fnConstant,                  8,                           "e" STD_SUB_E,                                 "e" STD_SUB_E,                                 CAT_CNST, SLS_ENABLED  },
/*  141 */  { itemToBeCoded,               NOPARAM,                     "EIGVAL",                                      "EIGVAL",                                      CAT_FNCT, SLS_UNCHANGED},
/*  142 */  { itemToBeCoded,               NOPARAM,                     "EIGVEC",                                      "EIGVEC",                                      CAT_FNCT, SLS_UNCHANGED},
/*  143 */  { itemToBeCoded,               NOPARAM,                     "END",                                         "END",                                         CAT_FNCT, SLS_UNCHANGED},
/*  144 */  { itemToBeCoded,               NOPARAM,                     "ENDP",                                        "End",                                         CAT_FNCT, SLS_UNCHANGED},
/*  145 */  { fnDisplayFormatEng,          TM_VALUE,                    "ENG",                                         "ENG",                                         CAT_FNCT, SLS_UNCHANGED},
/*  146 */  { fnDisplayOvr,                DO_ENG,                      "ENGOVR",                                      "ENGOVR",                                      CAT_FNCT, SLS_UNCHANGED},
/*  147 */  { itemToBeCoded,               NOPARAM,                     "ENORM",                                       "ENORM",                                       CAT_FNCT, SLS_UNCHANGED},
/*  148 */  { fnKeyEnter,                  NOPARAM,                     "ENTER" STD_UP_ARROW,                          "ENTER" STD_UP_ARROW,                          CAT_FNCT, SLS_DISABLED },
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
/*  162 */  { itemToBeCoded,               NOPARAM,                     "Expon" STD_SUB_p,                             "Expon" STD_SUB_p,                             CAT_FNCT, SLS_UNCHANGED},
/*  163 */  { itemToBeCoded,               NOPARAM,                     "Expon" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Expon" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   CAT_FNCT, SLS_UNCHANGED},
/*  164 */  { itemToBeCoded,               NOPARAM,                     "Expon" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Expon" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   CAT_FNCT, SLS_UNCHANGED},
/*  165 */  { itemToBeCoded,               NOPARAM,                     "Expon" STD_SUP_MINUS_1,                       "Expon" STD_SUP_MINUS_1,                       CAT_FNCT, SLS_UNCHANGED},
/*  166 */  { itemToBeCoded,               NOPARAM,                     "Expon:",                                      "Expon:",                                      CAT_MENU, SLS_UNCHANGED},
/*  167 */  { fnExpt,                      NOPARAM,                     "EXPT",                                        "EXPT",                                        CAT_FNCT, SLS_ENABLED  },
/*  168 */  { itemToBeCoded,               NOPARAM,                     "e" STD_SUP_x "-1",                            "e" STD_SUP_x "-1",                            CAT_FNCT, SLS_ENABLED  },
/*  169 */  { itemToBeCoded,               NOPARAM,                     "e/m" STD_SUB_e,                               "e/m" STD_SUB_e,                               CAT_NONE, SLS_ENABLED  },
/*  170 */  { itemToBeCoded,               NOPARAM,                     "Energy:",                                     "Energy:",                                     CAT_MENU, SLS_UNCHANGED},
/*  171 */  { fnConstant,                  9,                           "F",                                           "F",                                           CAT_CNST, SLS_ENABLED  },
/*  172 */  { itemToBeCoded,               NOPARAM,                     "FAST",                                        "FAST",                                        CAT_FNCT, SLS_UNCHANGED},
/*  173 */  { fnFb,                        TM_VALUE,                    "FB",                                          "FB",                                          CAT_FNCT, SLS_ENABLED  },
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
/*  193 */  { itemToBeCoded,               NOPARAM,                     "F" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", "F" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", CAT_FNCT, SLS_UNCHANGED},
/*  194 */  { itemToBeCoded,               NOPARAM,                     "F" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", "F" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", CAT_FNCT, SLS_UNCHANGED},
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
/*  205 */  { fnConstant,                  10,                          "F" STD_SUB_alpha,                             "F" STD_SUB_alpha,                             CAT_CNST, SLS_ENABLED  },
/*  206 */  { fnConstant,                  11,                          "F" STD_SUB_delta,                             "F" STD_SUB_delta,                             CAT_CNST, SLS_ENABLED  },
/*  207 */  { itemToBeCoded,               NOPARAM,                     "F:",                                          "F:",                                          CAT_MENU, SLS_UNCHANGED},
/*  208 */  { itemToBeCoded,               NOPARAM,                     "f'",                                          "f'",                                          CAT_MENU, SLS_UNCHANGED},
/*  209 */  { itemToBeCoded,               NOPARAM,                     "f\"",                                         "f\"",                                         CAT_MENU, SLS_UNCHANGED},
/*  210 */  { itemToBeCoded,               NOPARAM,                     "f'(x)",                                       "f'(x)",                                       CAT_FNCT, SLS_UNCHANGED},
/*  211 */  { itemToBeCoded,               NOPARAM,                     "f\"(x)",                                      "f\"(x)",                                      CAT_FNCT, SLS_UNCHANGED},
/*  212 */  { itemToBeCoded,               NOPARAM,                     "F&p:",                                        "F&p:",                                        CAT_MENU, SLS_UNCHANGED},
/*  213 */  { fnConstant,                  12,                          "G",                                           "G",                                           CAT_CNST, SLS_ENABLED  },
/*  214 */  { fnConstant,                  13,                          "G" STD_SUB_0,                                 "G" STD_SUB_0,                                 CAT_CNST, SLS_ENABLED  },
/*  215 */  { fnDisplayFormatGap,          TM_VALUE,                    "GAP",                                         "GAP",                                         CAT_FNCT, SLS_UNCHANGED},
/*  216 */  { fnConstant,                  14,                          "G" STD_SUB_C,                                 "G" STD_SUB_C,                                 CAT_CNST, SLS_ENABLED  },
/*  217 */  { fnGcd,                       NOPARAM,                     "GCD",                                         "GCD",                                         CAT_FNCT, SLS_ENABLED  },
/*  218 */  { itemToBeCoded,               NOPARAM,                     "g" STD_SUB_d,                                 "g" STD_SUB_d,                                 CAT_FNCT, SLS_UNCHANGED},
/*  219 */  { itemToBeCoded,               NOPARAM,                     "g" STD_SUB_d STD_SUP_MINUS_1,                 "g" STD_SUB_d STD_SUP_MINUS_1,                 CAT_FNCT, SLS_UNCHANGED},
/*  220 */  { fnConstant,                  15,                          "g" STD_SUB_e,                                 "g" STD_SUB_e,                                 CAT_CNST, SLS_ENABLED  },
/*  221 */  { itemToBeCoded,               NOPARAM,                     "Geom" STD_SUB_p,                              "Geom" STD_SUB_p,                              CAT_FNCT, SLS_UNCHANGED},
/*  222 */  { itemToBeCoded,               NOPARAM,                     "Geom" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,    "Geom" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,    CAT_FNCT, SLS_UNCHANGED},
/*  223 */  { itemToBeCoded,               NOPARAM,                     "Geom" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,    "Geom" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,    CAT_FNCT, SLS_UNCHANGED},
/*  224 */  { itemToBeCoded,               NOPARAM,                     "Geom" STD_SUP_MINUS_1,                        "Geom" STD_SUP_MINUS_1,                        CAT_FNCT, SLS_UNCHANGED},
/*  225 */  { itemToBeCoded,               NOPARAM,                     "Geom:",                                       "Geom:",                                       CAT_MENU, SLS_UNCHANGED},
/*  226 */  { fnCvtGalukM3,                multiply,                    "gl" STD_UK STD_RIGHT_ARROW "m" STD_SUP_3,     "gl" STD_UK STD_RIGHT_ARROW "m" STD_SUP_3,     CAT_FNCT, SLS_ENABLED  },
/*  227 */  { fnCvtGalusM3,                multiply,                    "gl" STD_US STD_RIGHT_ARROW "m" STD_SUP_3,     "gl" STD_US STD_RIGHT_ARROW "m" STD_SUP_3,     CAT_FNCT, SLS_ENABLED  },
/*  228 */  { fnConstant,                  16,                          "GM" STD_SUB_EARTH,                            "GM" STD_SUB_EARTH,                            CAT_CNST, SLS_ENABLED  },
/*  229 */  { fnAngularMode,               AM_GRAD,                     "GRAD",                                        "GRAD",                                        CAT_FNCT, SLS_UNCHANGED},
/*  230 */  { fnCvtToCurrentAngularMode,   AM_GRAD,                     "GRAD" STD_RIGHT_ARROW,                        "GRAD" STD_RIGHT_ARROW,                        CAT_FNCT, SLS_ENABLED  },
/*  231 */  { itemToBeCoded,               NOPARAM,                     "GTO",                                         "GTO",                                         CAT_FNCT, SLS_UNCHANGED},
/*  232 */  { itemToBeCoded,               NOPARAM,                     "GTO.",                                        "GTO.",                                        CAT_FNCT, SLS_UNCHANGED},
/*  233 */  { fnConstant,                  17,                          "g" STD_SUB_EARTH,                             "g" STD_SUB_EARTH,                             CAT_CNST, SLS_ENABLED  },
/*  234 */  { fnConstant,                  18,                          STD_PLANCK,                                    STD_PLANCK,                                    CAT_CNST, SLS_ENABLED  },
/*  235 */  { itemToBeCoded,               NOPARAM,                     "H" STD_SUB_n,                                 "H" STD_SUB_n,                                 CAT_FNCT, SLS_UNCHANGED},
/*  236 */  { itemToBeCoded,               NOPARAM,                     "H" STD_SUB_n STD_SUB_P,                       "H" STD_SUB_n STD_SUB_P,                       CAT_FNCT, SLS_UNCHANGED},
/*  237 */  { fnCvtHpeW,                   multiply,                    "hp" STD_SUB_E STD_RIGHT_ARROW "W",            "hp" STD_SUB_E STD_RIGHT_ARROW "W",            CAT_FNCT, SLS_ENABLED  },
/*  238 */  { fnCvtHpmW,                   multiply,                    "hp" STD_SUB_M STD_RIGHT_ARROW "W",            "hp" STD_SUB_M STD_RIGHT_ARROW "W",            CAT_FNCT, SLS_ENABLED  },
/*  239 */  { fnCvtHpukW,                  multiply,                    "hp" STD_UK STD_RIGHT_ARROW "W",               "hp" STD_UK STD_RIGHT_ARROW "W",               CAT_FNCT, SLS_ENABLED  },
/*  240 */  { itemToBeCoded,               NOPARAM,                     "Hyper" STD_SUB_p,                             "Hyper" STD_SUB_p,                             CAT_FNCT, SLS_UNCHANGED},
/*  241 */  { itemToBeCoded,               NOPARAM,                     "Hyper" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Hyper" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   CAT_FNCT, SLS_UNCHANGED},
/*  242 */  { itemToBeCoded,               NOPARAM,                     "Hyper" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Hyper" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   CAT_FNCT, SLS_UNCHANGED},
/*  243 */  { itemToBeCoded,               NOPARAM,                     "Hyper" STD_SUP_MINUS_1,                       "Hyper" STD_SUP_MINUS_1,                       CAT_FNCT, SLS_UNCHANGED},
/*  244 */  { itemToBeCoded,               NOPARAM,                     "Hyper:",                                      "Hyper:",                                      CAT_MENU, SLS_UNCHANGED},
/*  245 */  { fnConstant,                  19,                          STD_PLANCK_2PI,                                STD_PLANCK_2PI,                                CAT_CNST, SLS_ENABLED  },
/*  246 */  { addItemToBuffer,             REGISTER_I,                  "I",                                           "I",                                           CAT_RVAR, SLS_UNCHANGED},
/*  247 */  { fnIDiv,                      NOPARAM,                     "IDIV",                                        "IDIV",                                        CAT_FNCT, SLS_ENABLED  },
/*  248 */  { fnCvtInhgPa,                 multiply,                    "iHg" STD_RIGHT_ARROW "Pa",                    "in.Hg",                                       CAT_FNCT, SLS_ENABLED  },
/*  249 */  { itemToBeCoded,               NOPARAM,                     "L.INTS",                                      "L.INTS",                                      CAT_MENU, SLS_UNCHANGED},
/*  250 */  { fnImaginaryPart,             NOPARAM,                     "Im",                                          "Im",                                          CAT_FNCT, SLS_ENABLED  },
/*  251 */  { fnToggleFractionType,        NOPARAM,                     "IMPFRC",                                      "d/c",                                         CAT_FNCT, SLS_UNCHANGED},
/*  252 */  { fnInc,                       TM_REGISTER,                 "INC",                                         "INC",                                         CAT_FNCT, SLS_ENABLED  },
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
/*  280 */  { fnConstant,                  20,                          "k",                                           "k",                                           CAT_CNST, SLS_ENABLED  },
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
/*  293 */  { fnConstant,                  21,                          "K" STD_SUB_J,                                 "K" STD_SUB_J,                                 CAT_CNST, SLS_ENABLED  },
/*  294 */  { itemToBeCoded,               NOPARAM,                     "KTYP?",                                       "KTYP?",                                       CAT_FNCT, SLS_UNCHANGED},
/*  295 */  { addItemToBuffer,             REGISTER_L,                  "L",                                           "L",                                           CAT_RVAR, SLS_UNCHANGED},
/*  296 */  { fnLastX,                     NOPARAM,                     "LASTx",                                       "LSTx",                                        CAT_FNCT, SLS_ENABLED  },
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
/*  309 */  { fnLj,                        NOPARAM,                     "LJ",                                          "LJ",                                          CAT_FNCT, SLS_ENABLED  },
/*  310 */  { fnLn,                        NOPARAM,                     "LN",                                          "LN",                                          CAT_FNCT, SLS_ENABLED  },   //JM3 change ln to LN
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
/*  323 */  { fnLog10,                     NOPARAM,                     "LOG" STD_SUB_10,                              "LOG",                                         CAT_FNCT, SLS_ENABLED  },   //JM Change lg to LOG
/*  324 */  { fnLog2,                      NOPARAM,                     "LOG" STD_SUB_2,                               "lb x",                                        CAT_FNCT, SLS_ENABLED  },
/*  325 */  { fnCurveFitting,              CF_LOGARITHMIC_FITTING,      "LogF",                                        "LogF",                                        CAT_FNCT, SLS_UNCHANGED},
/*  326 */  { itemToBeCoded,               NOPARAM,                     "Logis" STD_SUB_p,                             "Logis" STD_SUB_p,                             CAT_FNCT, SLS_UNCHANGED},
/*  327 */  { itemToBeCoded,               NOPARAM,                     "Logis" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Logis" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   CAT_FNCT, SLS_UNCHANGED},
/*  328 */  { itemToBeCoded,               NOPARAM,                     "Logis" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Logis" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   CAT_FNCT, SLS_UNCHANGED},
/*  329 */  { itemToBeCoded,               NOPARAM,                     "Logis" STD_SUP_MINUS_1,                       "Logis" STD_SUP_MINUS_1,                       CAT_FNCT, SLS_UNCHANGED},
/*  330 */  { itemToBeCoded,               NOPARAM,                     "Logis:",                                      "Logis:",                                      CAT_MENU, SLS_UNCHANGED},
/*  331 */  { itemToBeCoded,               NOPARAM,                     "LOG" STD_SUB_x "y",                           "log" STD_SUB_x "y",                           CAT_FNCT, SLS_UNCHANGED},
/*  332 */  { itemToBeCoded,               NOPARAM,                     "LOOP",                                        "LOOP",                                        CAT_MENU, SLS_UNCHANGED},
/*  333 */  { fnConstant,                  22,                          "l" STD_SUB_P STD_SUB_L,                       "l" STD_SUB_P STD_SUB_L,                       CAT_CNST, SLS_ENABLED  },
/*  334 */  { fnCvtLyM,                    multiply,                    "ly" STD_RIGHT_ARROW "m",                      "ly" STD_RIGHT_ARROW "m",                      CAT_FNCT, SLS_ENABLED  },
/*  335 */  { fnLeadingZeros,              false,                       "LZOFF",                                       "LZOFF",                                       CAT_NONE, SLS_UNCHANGED},   //dr
/*  336 */  { fnLeadingZeros,              true,                        "LZON",                                        "LZON",                                        CAT_NONE, SLS_UNCHANGED},   //dr
/*  337 */  { itemToBeCoded,               NOPARAM,                     "L.R.",                                        "L.R.",                                        CAT_FNCT, SLS_UNCHANGED},
/*  338 */  { fnCvtAcreM2,                 divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ac",            "m" STD_SUP_2 " " STD_RIGHT_ARROW,             CAT_FNCT, SLS_ENABLED  },
/*  339 */  { fnCvtAcreusM2,               divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ac" STD_US,     "m" STD_SUP_2 " " STD_RIGHT_ARROW,             CAT_FNCT, SLS_ENABLED  },
/*  340 */  { fnCvtFlozukM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "fz" STD_UK,     "m" STD_SUP_3 " " STD_RIGHT_ARROW,             CAT_FNCT, SLS_ENABLED  },
/*  341 */  { fnCvtFlozusM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "fz" STD_US,     "m" STD_SUP_3 " " STD_RIGHT_ARROW,             CAT_FNCT, SLS_ENABLED  },
/*  342 */  { fnCvtGalukM3,                divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_UK,     "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_UK,     CAT_FNCT, SLS_ENABLED  },
/*  343 */  { fnCvtGalusM3,                divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_US,     "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_US,     CAT_FNCT, SLS_ENABLED  },
/*  344 */  { fnMant,                      NOPARAM,                     "MANT",                                        "MANT",                                        CAT_FNCT, SLS_ENABLED  },
/*  345 */  { fnMaskl,                     TM_VALUE,                    "MASKL",                                       "MASKL",                                       CAT_FNCT, SLS_ENABLED  },
/*  346 */  { fnMaskr,                     TM_VALUE,                    "MASKR",                                       "MASKR",                                       CAT_FNCT, SLS_ENABLED  },
/*  347 */  { itemToBeCoded,               NOPARAM,                     "MATRS",                                       "MATRS",                                       CAT_MENU, SLS_UNCHANGED},
/*  348 */  { itemToBeCoded,               NOPARAM,                     "MATR?",                                       "MATR?",                                       CAT_FNCT, SLS_UNCHANGED},
/*  349 */  { itemToBeCoded,               NOPARAM,                     "MATX",                                        "MATX",                                        CAT_MENU, SLS_UNCHANGED},
/*  350 */  { itemToBeCoded,               NOPARAM,                     "Mat_A",                                       "Mat A",                                       CAT_RVAR, SLS_UNCHANGED},
/*  351 */  { itemToBeCoded,               NOPARAM,                     "Mat_B",                                       "Mat B",                                       CAT_RVAR, SLS_UNCHANGED},
/*  352 */  { itemToBeCoded,               NOPARAM,                     "Mat_X",                                       "Mat X",                                       CAT_FNCT, SLS_UNCHANGED},
/*  353 */  { fnMax,                       NOPARAM,                     "max",                                         "max",                                         CAT_FNCT, SLS_ENABLED  },
/*  354 */  { fnConstant,                  23,                          "m" STD_SUB_e,                                 "m" STD_SUB_e,                                 CAT_CNST, SLS_ENABLED  },
/*  355 */  { fnFreeMemory,                NOPARAM,                     "MEM?",                                        "MEM?",                                        CAT_FNCT, SLS_ENABLED  },
/*  356 */  { itemToBeCoded,               NOPARAM,                     "MENU",                                        "MENU",                                        CAT_FNCT, SLS_UNCHANGED},
/*  357 */  { itemToBeCoded,               NOPARAM,                     "MENUS",                                       "MENUS",                                       CAT_MENU, SLS_UNCHANGED},
/*  358 */  { fnMin,                       NOPARAM,                     "min",                                         "min",                                         CAT_FNCT, SLS_ENABLED  },
/*  359 */  { fnMirror,                    NOPARAM,                     "MIRROR",                                      "MIRROR",                                      CAT_FNCT, SLS_ENABLED  },
/*  360 */  { fnCvtMiM,                    multiply,                    "mi." STD_RIGHT_ARROW "m",                     "mi." STD_RIGHT_ARROW "m",                     CAT_FNCT, SLS_ENABLED  },
/*  361 */  { fnConstant,                  24,                          "M" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "M" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   CAT_CNST, SLS_ENABLED  },
/*  362 */  { fnConstant,                  25,                          "m" STD_SUB_n,                                 "m" STD_SUB_n,                                 CAT_CNST, SLS_ENABLED  },
/*  363 */  { fnConstant,                  26,                          "m" STD_SUB_n "/m" STD_SUB_p,                  "m" STD_SUB_n "/m" STD_SUB_p,                  CAT_CNST, SLS_ENABLED  },
/*  364 */  { fnMod,                       NOPARAM,                     "MOD",                                         "MOD",                                         CAT_FNCT, SLS_ENABLED  },
/*  365 */  { itemToBeCoded,               NOPARAM,                     "MODE",                                        "MODE",                                        CAT_MENU, SLS_UNCHANGED},
/*  366 */  { itemToBeCoded,               NOPARAM,                     "MONTH",                                       "MONTH",                                       CAT_FNCT, SLS_UNCHANGED},
/*  367 */  { fnConstant,                  27,                          "m" STD_SUB_p,                                 "m" STD_SUB_p,                                 CAT_CNST, SLS_ENABLED  },
/*  368 */  { fnConstant,                  28,                          "m" STD_SUB_P STD_SUB_L,                       "m" STD_SUB_P STD_SUB_L,                       CAT_CNST, SLS_ENABLED  },
/*  369 */  { fnConstant,                  29,                          "m" STD_SUB_p "/m" STD_SUB_e,                  "m" STD_SUB_p "/m" STD_SUB_e,                  CAT_CNST, SLS_ENABLED  },
/*  370 */  { itemToBeCoded,               NOPARAM,                     "MSG",                                         "MSG",                                         CAT_FNCT, SLS_UNCHANGED},
/*  371 */  { fnConstant,                  30,                          "m" STD_SUB_u,                                 "m" STD_SUB_u,                                 CAT_CNST, SLS_ENABLED  },
/*  372 */  { fnConstant,                  31,                          "m" STD_SUB_u "c" STD_SUP_2,                   "m" STD_SUB_u "c" STD_SUP_2,                   CAT_CNST, SLS_ENABLED  },
/*  373 */  { fnProductSign,               PS_CROSS,                    "MULT" STD_CROSS,                              "MULT" STD_CROSS,                              CAT_FNCT, SLS_UNCHANGED},
/*  374 */  { fnProductSign,               PS_DOT,                      "MULT" STD_DOT,                                "MULT" STD_DOT,                                CAT_FNCT, SLS_UNCHANGED},
/*  375 */  { fnAngularMode,               AM_MULTPI,                   "MUL" STD_pi,                                  "MUL" STD_pi,                                  CAT_FNCT, SLS_UNCHANGED},
/*  376 */  { itemToBeCoded,               NOPARAM,                     "MVAR",                                        "MVAR",                                        CAT_FNCT, SLS_UNCHANGED},
/*  377 */  { itemToBeCoded,               NOPARAM,                     "MyMenu",                                      "MyM",                                         CAT_MENU, SLS_UNCHANGED},
/*  378 */  { itemToBeCoded,               NOPARAM,                     "My" STD_alpha,                                "My" STD_alpha,                                CAT_MENU, SLS_UNCHANGED},
/*  379 */  { fnConstant,                  32,                          "m" STD_SUB_mu,                                "m" STD_SUB_mu,                                CAT_CNST, SLS_ENABLED  },
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
/*  399 */  { itemToBeCoded,               NOPARAM,                     "Mass:",                                       "Mass:",                                       CAT_MENU, SLS_UNCHANGED},
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
/*  410 */  { fnConstant,                  33,                          "M" STD_SUB_SUN,                               "M" STD_SUB_SUN,                               CAT_CNST, SLS_ENABLED  },
/*  411 */  { fnConstant,                  34,                          "M" STD_SUB_EARTH,                             "M" STD_SUB_EARTH,                             CAT_CNST, SLS_ENABLED  },
/*  412 */  { fnConstant,                  35,                          "N" STD_SUB_A,                                 "N" STD_SUB_A,                                 CAT_CNST, SLS_ENABLED  },
/*  413 */  { fnConstant,                  36,                          "NaN",                                         "NaN",                                         CAT_CNST, SLS_ENABLED  },
/*  414 */  { fnLogicalNand,               NOPARAM,                     "NAND",                                        "NAND",                                        CAT_FNCT, SLS_ENABLED  },
/*  415 */  { itemToBeCoded,               NOPARAM,                     "NaN?",                                        "NaN?",                                        CAT_FNCT, SLS_UNCHANGED},
/*  416 */  { itemToBeCoded,               NOPARAM,                     "NBin" STD_SUB_p,                              "NBin" STD_SUB_p,                              CAT_FNCT, SLS_UNCHANGED},
/*  417 */  { itemToBeCoded,               NOPARAM,                     "NBin" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,    "NBin" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,    CAT_FNCT, SLS_UNCHANGED},
/*  418 */  { itemToBeCoded,               NOPARAM,                     "NBin" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,    "NBin" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,    CAT_FNCT, SLS_UNCHANGED},
/*  419 */  { itemToBeCoded,               NOPARAM,                     "NBin" STD_SUP_MINUS_1,                        "NBin" STD_SUP_MINUS_1,                        CAT_FNCT, SLS_UNCHANGED},
/*  420 */  { itemToBeCoded,               NOPARAM,                     "NBin:",                                       "NBin:",                                       CAT_MENU, SLS_UNCHANGED},
/*  421 */  { fnNeighb,                    NOPARAM,                     "NEIGHB",                                      "NEIGHB",                                      CAT_FNCT, SLS_ENABLED  },
/*  422 */  { fnNextPrime,                 NOPARAM,                     "NEXTP",                                       "NEXTP",                                       CAT_FNCT, SLS_ENABLED  },
/*  423 */  { fnCvtNmiM,                   multiply,                    "nmi." STD_RIGHT_ARROW "m",                    "nmi." STD_RIGHT_ARROW "m",                    CAT_FNCT, SLS_ENABLED  },
/*  424 */  { fnNop,                       NOPARAM,                     "NOP",                                         "NOP",                                         CAT_FNCT, SLS_UNCHANGED},
/*  425 */  { fnLogicalNor,                NOPARAM,                     "NOR",                                         "NOR",                                         CAT_FNCT, SLS_ENABLED  },
/*  426 */  { itemToBeCoded,               NOPARAM,                     "Norml" STD_SUB_p,                             "Norml" STD_SUB_p,                             CAT_FNCT, SLS_UNCHANGED},
/*  427 */  { itemToBeCoded,               NOPARAM,                     "Norml" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Norml" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   CAT_FNCT, SLS_UNCHANGED},
/*  428 */  { itemToBeCoded,               NOPARAM,                     "Norml" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Norml" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   CAT_FNCT, SLS_UNCHANGED},
/*  429 */  { itemToBeCoded,               NOPARAM,                     "Norml" STD_SUP_MINUS_1,                       "Norml" STD_SUP_MINUS_1,                       CAT_FNCT, SLS_UNCHANGED},
/*  430 */  { itemToBeCoded,               NOPARAM,                     "Norml:",                                      "Norml:",                                      CAT_MENU, SLS_UNCHANGED},
/*  431 */  { fnLogicalNot,                NOPARAM,                     "NOT",                                         "NOT",                                         CAT_FNCT, SLS_ENABLED  },
/*  432 */  { itemToBeCoded,               NOPARAM,                     "NPER",                                        "n" STD_SUB_P STD_SUB_E STD_SUB_R,             CAT_RVAR, SLS_UNCHANGED},
/*  433 */  { fnStatSum,                   0,                           "n" STD_SIGMA,                                 "n",                                           CAT_FNCT, SLS_ENABLED  },
/*  434 */  { fnCvtLbfN,                   divide,                      "N" STD_RIGHT_ARROW "lbf",                     "N" STD_RIGHT_ARROW "lbf",                     CAT_FNCT, SLS_ENABLED  },
/*  435 */  { itemToBeCoded,               NOPARAM,                     "ODD?",                                        "ODD?",                                        CAT_FNCT, SLS_UNCHANGED},
/*  436 */  { fnOff,                       NOPARAM,                     "OFF",                                         "OFF",                                         CAT_FNCT, SLS_UNCHANGED},
/*  437 */  { fnLogicalOr,                 NOPARAM,                     "OR",                                          "OR",                                          CAT_FNCT, SLS_ENABLED  },
/*  438 */  { itemToBeCoded,               NOPARAM,                     "OrthoF",                                      "OrthoF",                                      CAT_FNCT, SLS_UNCHANGED},
/*  439 */  { itemToBeCoded,               NOPARAM,                     "ORTHOG",                                      "Orthog",                                      CAT_MENU, SLS_UNCHANGED},
/*  440 */  { fnCvtOzKg,                   multiply,                    "oz" STD_RIGHT_ARROW "kg",                     "oz" STD_RIGHT_ARROW "kg",                     CAT_FNCT, SLS_ENABLED  },
/*  441 */  { fnConstant,                  37,                          "p" STD_SUB_0,                                 "p" STD_SUB_0,                                 CAT_CNST, SLS_ENABLED  },
/*  442 */  { itemToBeCoded,               NOPARAM,                     "PAUSE",                                       "PAUSE",                                       CAT_FNCT, SLS_UNCHANGED},
/*  443 */  { fnCvtAtmPa,                  divide,                      "Pa" STD_RIGHT_ARROW "atm",                    "Pa" STD_RIGHT_ARROW "atm",                    CAT_FNCT, SLS_ENABLED  },
/*  444 */  { fnCvtBarPa,                  divide,                      "Pa" STD_RIGHT_ARROW "bar",                    "Pa" STD_RIGHT_ARROW "bar",                    CAT_FNCT, SLS_ENABLED  },
/*  445 */  { fnCvtInhgPa,                 divide,                      "Pa" STD_RIGHT_ARROW "iHg",                    "Pa" STD_RIGHT_ARROW,                          CAT_FNCT, SLS_ENABLED  },
/*  446 */  { fnCvtPsiPa,                  divide,                      "Pa" STD_RIGHT_ARROW "psi",                    "Pa" STD_RIGHT_ARROW "psi",                    CAT_FNCT, SLS_ENABLED  },
/*  447 */  { fnCvtTorrPa,                 divide,                      "Pa" STD_RIGHT_ARROW "tor",                    "Pa " STD_RIGHT_ARROW,                         CAT_FNCT, SLS_ENABLED  },
/*  448 */  { itemToBeCoded,               NOPARAM,                     "PARTS",                                       "PARTS",                                       CAT_MENU, SLS_UNCHANGED},
/*  449 */  { fnCvtPcM,                    multiply,                    "pc" STD_RIGHT_ARROW "m",                      "pc" STD_RIGHT_ARROW "m",                      CAT_FNCT, SLS_ENABLED  },
/*  450 */  { fnPyx,                       NOPARAM,                     "PERM",                                        "P" STD_SUB_y STD_SUB_x,                       CAT_FNCT, SLS_ENABLED  },
/*  451 */  { itemToBeCoded,               NOPARAM,                     "PER/a",                                       "per/a",                                       CAT_RVAR, SLS_UNCHANGED},
/*  452 */  { itemToBeCoded,               NOPARAM,                     "PGMINT",                                      "PGMINT",                                      CAT_FNCT, SLS_UNCHANGED},
/*  453 */  { itemToBeCoded,               NOPARAM,                     "PGMSLV",                                      "PGMSLV",                                      CAT_FNCT, SLS_UNCHANGED},
/*  454 */  { itemToBeCoded,               NOPARAM,                     "PIXEL",                                       "PIXEL",                                       CAT_FNCT, SLS_UNCHANGED},
/*  455 */  { fnGraph,                     4,                           "PLOT",                                        "PLOT",                                        CAT_FNCT, SLS_UNCHANGED},
/*  456 */  { itemToBeCoded,               NOPARAM,                     "PMT",                                         "PMT",                                         CAT_RVAR, SLS_UNCHANGED},
/*  457 */  { itemToBeCoded,               NOPARAM,                     "P" STD_SUB_n,                                 "P" STD_SUB_n,                                 CAT_FNCT, SLS_UNCHANGED},
/*  458 */  { itemToBeCoded,               NOPARAM,                     "POINT",                                       "POINT",                                       CAT_FNCT, SLS_UNCHANGED},
/*  459 */  { itemToBeCoded,               NOPARAM,                     "Poiss" STD_SUB_p,                             "Poiss" STD_SUB_p,                             CAT_FNCT, SLS_UNCHANGED},
/*  460 */  { itemToBeCoded,               NOPARAM,                     "Poiss" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Poiss" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   CAT_FNCT, SLS_UNCHANGED},
/*  461 */  { itemToBeCoded,               NOPARAM,                     "Poiss" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Poiss" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   CAT_FNCT, SLS_UNCHANGED},
/*  462 */  { itemToBeCoded,               NOPARAM,                     "Poiss" STD_SUP_MINUS_1,                       "Poiss" STD_SUP_MINUS_1,                       CAT_FNCT, SLS_UNCHANGED},
/*  463 */  { itemToBeCoded,               NOPARAM,                     "Poiss:",                                      "Poiss:",                                      CAT_MENU, SLS_UNCHANGED},
/*  464 */  { fnComplexMode,               CM_POLAR,                    "POLAR",                                       "POLAR",                                       CAT_FNCT, SLS_UNCHANGED},
/*  465 */  { itemToBeCoded,               NOPARAM,                     "PopLR",                                       "PopLR",                                       CAT_FNCT, SLS_UNCHANGED},
/*  466 */  { fnCurveFitting,              CF_POWER_FITTING,            "PowerF",                                      "PowerF",                                      CAT_FNCT, SLS_UNCHANGED},
/*  467 */  { fnCvtRatioDb,                10,                          "pr" STD_RIGHT_ARROW "dB",                     "power",                                       CAT_FNCT, SLS_ENABLED  },
/*  468 */  { itemToBeCoded,               NOPARAM,                     "PRCL",                                        "PRCL",                                        CAT_FNCT, SLS_UNCHANGED},
/*  469 */  { fnIsPrime,                   NOPARAM,                     "PRIME?",                                      "PRIME?",                                      CAT_FNCT, SLS_UNCHANGED},
/*  470 */  { itemToBeCoded,               NOPARAM,                     "PROB",                                        "PROB",                                        CAT_MENU, SLS_UNCHANGED},
/*  471 */  { fnToggleFractionType,        NOPARAM,                     "PROFRC",                                      "a" STD_SPACE_3_PER_EM "b/c",                  CAT_FNCT, SLS_UNCHANGED},
/*  472 */  { itemToBeCoded,               NOPARAM,                     "PROGS",                                       "PROGS",                                       CAT_MENU, SLS_UNCHANGED},
/*  473 */  { fnCvtPsiPa,                  multiply,                    "psi" STD_RIGHT_ARROW "Pa",                    "psi" STD_RIGHT_ARROW "Pa",                    CAT_FNCT, SLS_ENABLED  },
/*  474 */  { itemToBeCoded,               NOPARAM,                     "PSTO",                                        "PSTO",                                        CAT_FNCT, SLS_UNCHANGED},
/*  475 */  { fnCvtPointM,                 multiply,                    "pt." STD_RIGHT_ARROW "m",                     "point",                                       CAT_FNCT, SLS_ENABLED  },
/*  476 */  { itemToBeCoded,               NOPARAM,                     "PUTK",                                        "PUTK",                                        CAT_FNCT, SLS_UNCHANGED},
/*  477 */  { itemToBeCoded,               NOPARAM,                     "PV",                                          "PV",                                          CAT_RVAR, SLS_UNCHANGED},
/*  478 */  { itemToBeCoded,               NOPARAM,                     "P.FN",                                        "P.FN",                                        CAT_MENU, SLS_UNCHANGED},
/*  479 */  { itemToBeCoded,               NOPARAM,                     "P.FN2",                                       "P.FN2",                                       CAT_MENU, SLS_UNCHANGED},
/*  480 */  { itemToBeCoded,               NOPARAM,                     "Power:",                                      "Power:",                                      CAT_MENU, SLS_UNCHANGED},
/*  481 */  { itemToBeCoded,               NOPARAM,                     "QUIET",                                       "QUIET",                                       CAT_FNCT, SLS_UNCHANGED},
/*  482 */  { fnConstant,                  38,                          "R",                                           "R",                                           CAT_CNST, SLS_ENABLED  },
/*  483 */  { fnAngularMode,               AM_RADIAN,                   "RAD",                                         "RAD",                                         CAT_FNCT, SLS_UNCHANGED},
/*  484 */  { fnCvtToCurrentAngularMode,   AM_RADIAN,                   "RAD" STD_RIGHT_ARROW,                         "RAD" STD_RIGHT_ARROW,                         CAT_FNCT, SLS_ENABLED  },
/*  485 */  { itemToBeCoded,               NOPARAM,                     "RAM",                                         "RAM",                                         CAT_MENU, SLS_UNCHANGED},
/*  486 */  { fnRandom,                    NOPARAM,                     "RAN#",                                        "RAN#",                                        CAT_FNCT, SLS_ENABLED  },
/*  487 */  { registerBrowser,             NOPARAM,                     "REGS.V",                                      "REGS",                                        CAT_FNCT, SLS_UNCHANGED},   //JM Changed RBR to REGS
/*  488 */  { fnRecall,                    TM_STORCL,                   "RCL",                                         "RCL",                                         CAT_FNCT, SLS_ENABLED  },
/*  489 */  { fnRecallConfig,              NOPARAM,                     "RCLCFG",                                      "Config",                                      CAT_FNCT, SLS_UNCHANGED},
/*  490 */  { fnRecallElement,             NOPARAM,                     "RCLEL",                                       "RCLEL",                                       CAT_FNCT, SLS_ENABLED  },
/*  491 */  { fnRecallIJ,                  NOPARAM,                     "RCLIJ",                                       "RCLIJ",                                       CAT_FNCT, SLS_ENABLED  },
/*  492 */  { fnRecallStack,               TM_REGISTER,                 "RCLS",                                        "RCLS",                                        CAT_FNCT, SLS_ENABLED  },
/*  493 */  { fnRecallAdd,                 NOPARAM,                     "RCL+",                                        "RCL+",                                        CAT_FNCT, SLS_ENABLED  },
/*  494 */  { fnRecallSub,                 NOPARAM,                     "RCL-",                                        "RCL-",                                        CAT_FNCT, SLS_ENABLED  },
/*  495 */  { fnRecallMult,                NOPARAM,                     "RCL" STD_CROSS,                               "RCL" STD_CROSS,                               CAT_FNCT, SLS_ENABLED  },
/*  496 */  { fnRecallDiv,                 NOPARAM,                     "RCL/",                                        "RCL/",                                        CAT_FNCT, SLS_ENABLED  },
/*  497 */  { fnRecallMax,                 NOPARAM,                     "RCL" STD_UP_ARROW,                            "Max",                                         CAT_FNCT, SLS_ENABLED  },
/*  498 */  { fnRecallMin,                 NOPARAM,                     "RCL" STD_DOWN_ARROW,                          "Min",                                         CAT_FNCT, SLS_ENABLED  },
/*  499 */  { itemToBeCoded,               NOPARAM,                     "RDP",                                         "RDP",                                         CAT_FNCT, SLS_UNCHANGED},
/*  500 */  { fnRadixMark,                 RM_COMMA,                    "RDX,",                                        "RDX,",                                        CAT_FNCT, SLS_UNCHANGED},
/*  501 */  { fnRadixMark,                 RM_PERIOD,                   "RDX.",                                        "RDX.",                                        CAT_FNCT, SLS_UNCHANGED},
/*  502 */  { fnConstant,                  39,                          "r" STD_SUB_e,                                 "r" STD_SUB_e,                                 CAT_CNST, SLS_ENABLED  },
/*  503 */  { fnRealPart,                  NOPARAM,                     "Re",                                          "Re",                                          CAT_FNCT, SLS_ENABLED  },
/*  504 */  { fnComplexResult,             false,                       "REALRE",                                      "REALRE",                                      CAT_FNCT, SLS_UNCHANGED},   //dr
/*  505 */  { itemToBeCoded,               NOPARAM,                     "REALS",                                       "REALS",                                       CAT_MENU, SLS_UNCHANGED},
/*  506 */  { itemToBeCoded,               NOPARAM,                     "REAL?",                                       "REAL?",                                       CAT_FNCT, SLS_UNCHANGED},
/*  507 */  { fnComplexMode,               CM_RECTANGULAR,              "RECT",                                        "RECT",                                        CAT_FNCT, SLS_UNCHANGED},
/*  508 */  { itemToBeCoded,               NOPARAM,                     "REGS",                                        "REGS",                                        CAT_RVAR, SLS_UNCHANGED},
/*  509 */  { itemToBeCoded,               NOPARAM,                     "RECV",                                        "RECV",                                        CAT_FNCT, SLS_UNCHANGED},
/*  510 */  { fnReset,                     NOT_CONFIRMED,               "RESET",                                       "RESET",                                       CAT_FNCT, SLS_UNCHANGED},
/*  511 */  { fnReToCx,                    NOPARAM,                     "RE" STD_RIGHT_ARROW "CX",                     "RE" STD_RIGHT_ARROW "CX",                     CAT_FNCT, SLS_ENABLED  },
/*  512 */  { fnSwapRealImaginary,         NOPARAM,                     "Re" STD_LEFT_RIGHT_ARROWS "Im",               "Re" STD_LEFT_RIGHT_ARROWS "Im",               CAT_FNCT, SLS_ENABLED  },
/*  513 */  { fnRj,                        NOPARAM,                     "RJ",                                          "RJ",                                          CAT_FNCT, SLS_ENABLED  },
/*  514 */  { fnConstant,                  40,                          "R" STD_SUB_K,                                 "R" STD_SUB_K,                                 CAT_CNST, SLS_ENABLED  },
/*  515 */  { fnRl,                        TM_VALUE,                    "RL",                                          "RL",                                          CAT_FNCT, SLS_ENABLED  },
/*  516 */  { fnRlc,                       TM_VALUE,                    "RLC",                                         "RLC",                                         CAT_FNCT, SLS_ENABLED  },
/*  517 */  { fnConstant,                  41,                          "R" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "R" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   CAT_CNST, SLS_ENABLED  },
/*  518 */  { itemToBeCoded,               NOPARAM,                     "RMODE",                                       "RMODE",                                       CAT_FNCT, SLS_UNCHANGED},
/*  519 */  { fnGetRoundingMode,           NOPARAM,                     "RMODE?",                                      "RMODE?",                                      CAT_FNCT, SLS_ENABLED  },
/*  520 */  { fnRmd,                       NOPARAM,                     "RMD",                                         "RMD",                                         CAT_FNCT, SLS_ENABLED  },
/*  521 */  { itemToBeCoded,               NOPARAM,                     "RNORM",                                       "RNORM",                                       CAT_FNCT, SLS_UNCHANGED},
/*  522 */  { fnRound,                     NOPARAM,                     "ROUND",                                       "ROUND",                                       CAT_FNCT, SLS_ENABLED  },
/*  523 */  { fnRoundi,                    NOPARAM,                     "ROUNDI",                                      "ROUNDI",                                      CAT_FNCT, SLS_ENABLED  },
/*  524 */  { fnRr,                        TM_VALUE,                    "RR",                                          "RR",                                          CAT_FNCT, SLS_ENABLED  },
/*  525 */  { fnRrc,                       TM_VALUE,                    "RRC",                                         "RRC",                                         CAT_FNCT, SLS_ENABLED  },
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
/*  537 */  { fnConstant,                  42,                          "R" STD_SUB_INFINITY,                          "R" STD_SUB_INFINITY,                          CAT_CNST, SLS_ENABLED  },
/*  538 */  { fnConstant,                  43,                          "R" STD_SUB_SUN,                               "R" STD_SUB_SUN,                               CAT_CNST, SLS_ENABLED  },
/*  539 */  { fnConstant,                  44,                          "R" STD_SUB_EARTH,                             "R" STD_SUB_EARTH,                             CAT_CNST, SLS_ENABLED  },
/*  540 */  { itemToBeCoded,               NOPARAM,                     "s",                                           "s",                                           CAT_FNCT, SLS_UNCHANGED},
/*  541 */  { fnConstant,                  45,                          "Sa",                                          "Sa",                                          CAT_CNST, SLS_ENABLED  },
/*  542 */  { itemToBeCoded,               NOPARAM,                     "SAVE",                                        "SAVE",                                        CAT_FNCT, SLS_UNCHANGED},
/*  543 */  { fnSb,                        TM_VALUE,                    "SB",                                          "SB",                                          CAT_FNCT, SLS_ENABLED  },
/*  544 */  { fnConstant,                  46,                          "Sb",                                          "Sb",                                          CAT_CNST, SLS_ENABLED  },
/*  545 */  { fnDisplayFormatSci,          TM_VALUE,                    "SCI",                                         "SCI",                                         CAT_FNCT, SLS_UNCHANGED},
/*  546 */  { fnCvtShortcwtKg,             multiply,                    "scw" STD_RIGHT_ARROW "kg",                    "short",                                       CAT_FNCT, SLS_ENABLED  },
/*  547 */  { fnDisplayOvr,                DO_SCI,                      "SCIOVR",                                      "SCIOVR",                                      CAT_FNCT, SLS_UNCHANGED},
/*  548 */  { fnGetSignificantDigits,      NOPARAM,                     "SDIGS?",                                      "SDIGS?",                                      CAT_FNCT, SLS_ENABLED  },
/*  549 */  { itemToBeCoded,               NOPARAM,                     "SDL",                                         "SDL",                                         CAT_FNCT, SLS_UNCHANGED},
/*  550 */  { itemToBeCoded,               NOPARAM,                     "SDR",                                         "SDR",                                         CAT_FNCT, SLS_UNCHANGED},
/*  551 */  { fnConstant,                  47,                          "Se" STD_SUP_2,                                "Se" STD_SUP_2,                                CAT_CNST, SLS_ENABLED  },
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
/*  563 */  { fnConstant,                  48,                          "Se'" STD_SUP_2,                               "Se'" STD_SUP_2,                               CAT_CNST, SLS_ENABLED  },
/*  564 */  { fnSetFlag,                   TM_FLAG,                     "SF",                                          "SF",                                          CAT_FNCT, SLS_UNCHANGED},
/*  565 */  { fnConstant,                  49,                          "Sf" STD_SUP_MINUS_1,                          "Sf" STD_SUP_MINUS_1,                          CAT_CNST, SLS_ENABLED  },
/*  566 */  { fnSign,                      NOPARAM,                     "SIGN",                                        "sign",                                        CAT_FNCT, SLS_ENABLED  },
/*  567 */  { fnIntegerMode,               SIM_SIGNMT,                  "SIGNMT",                                      "SIGNMT",                                      CAT_FNCT, SLS_UNCHANGED},
/*  568 */  { itemToBeCoded,               NOPARAM,                     "SIM_EQ",                                      "SIM EQ",                                      CAT_FNCT, SLS_UNCHANGED},
/*  569 */  { fnSin,                       NOPARAM,                     "SIN",                                         "SIN",                                         CAT_FNCT, SLS_ENABLED  },   //JM3
/*  570 */  { fnSinc,                      NOPARAM,                     "sinc",                                        "sinc",                                        CAT_FNCT, SLS_ENABLED  },
/*  571 */  { fnSinh,                      NOPARAM,                     "sinh",                                        "sinh",                                        CAT_FNCT, SLS_ENABLED  },
/*  572 */  { itemToBeCoded,               NOPARAM,                     "SKIP",                                        "SKIP",                                        CAT_FNCT, SLS_UNCHANGED},
/*  573 */  { fnSl,                        TM_VALUE,                    "SL",                                          "SL",                                          CAT_FNCT, SLS_ENABLED  },
/*  574 */  { itemToBeCoded,               NOPARAM,                     "SLOW",                                        "SLOW",                                        CAT_FNCT, SLS_UNCHANGED},
/*  575 */  { fnSlvq,                      NOPARAM,                     "SLVQ",                                        "SLVQ",                                        CAT_FNCT, SLS_ENABLED  },
/*  576 */  { itemToBeCoded,               NOPARAM,                     "s" STD_SUB_m,                                 "s" STD_SUB_m,                                 CAT_FNCT, SLS_UNCHANGED},
/*  577 */  { fnGetIntegerSignMode,        NOPARAM,                     "SMODE?",                                      "SMODE?",                                      CAT_FNCT, SLS_ENABLED  },
/*  578 */  { itemToBeCoded,               NOPARAM,                     "s" STD_SUB_m STD_SUB_w,                       "s" STD_SUB_m STD_SUB_w,                       CAT_FNCT, SLS_UNCHANGED},
/*  579 */  { itemToBeCoded,               NOPARAM,                     "SOLVE",                                       "SOLVE",                                       CAT_FNCT, SLS_UNCHANGED},
/*  580 */  { itemToBeCoded,               NOPARAM,                     "Solver",                                      "Solver",                                      CAT_MENU, SLS_UNCHANGED},
/*  581 */  { itemToBeCoded,               NOPARAM,                     "SPEC?",                                       "SPEC?",                                       CAT_FNCT, SLS_UNCHANGED},
/*  582 */  { fnSr,                        TM_VALUE,                    "SR",                                          "SR",                                          CAT_FNCT, SLS_ENABLED  },
/*  583 */  { fnStackSize,                 SS_4,                        "SSIZE4",                                      "SSIZE4",                                      CAT_FNCT, SLS_UNCHANGED},
/*  584 */  { fnStackSize,                 SS_8,                        "SSIZE8",                                      "SSIZE8",                                      CAT_FNCT, SLS_UNCHANGED},
/*  585 */  { fnGetStackSize,              NOPARAM,                     "SSIZE?",                                      "SSIZE?",                                      CAT_FNCT, SLS_ENABLED  },
/*  586 */  { itemToBeCoded,               NOPARAM,                     "STAT",                                        "STAT",                                        CAT_MENU, SLS_UNCHANGED},
/*  587 */  { flagBrowser,                 NOPARAM,                     "FLAGS.V",                                     "FLGS",                                        CAT_FNCT, SLS_UNCHANGED},   //JM Changed STATUS
/*  588 */  { itemToBeCoded,               NOPARAM,                     "STK",                                         "STK",                                         CAT_MENU, SLS_UNCHANGED},
/*  589 */  { fnStore,                     TM_STORCL,                   "STO",                                         "STO",                                         CAT_FNCT, SLS_UNCHANGED},
/*  590 */  { fnStoreConfig,               NOPARAM,                     "STOCFG",                                      "Config",                                      CAT_FNCT, SLS_UNCHANGED},
/*  591 */  { fnStoreElement,              NOPARAM,                     "STOEL",                                       "STOEL",                                       CAT_FNCT, SLS_ENABLED  },
/*  592 */  { fnStoreIJ,                   NOPARAM,                     "STOIJ",                                       "STOIJ",                                       CAT_FNCT, SLS_ENABLED  },
/*  593 */  { itemToBeCoded,               NOPARAM,                     "STOP",                                        "R/S",                                         CAT_FNCT, SLS_UNCHANGED},
/*  594 */  { fnStoreStack,                TM_REGISTER,                 "STOS",                                        "STOS",                                        CAT_FNCT, SLS_ENABLED  },
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
/*  612 */  { fnSumXY,                     NOPARAM,                     "SUM",                                         "SUM",                                         CAT_FNCT, SLS_ENABLED  },
/*  613 */  { itemToBeCoded,               NOPARAM,                     "s" STD_SUB_w,                                 "s" STD_SUB_w,                                 CAT_FNCT, SLS_UNCHANGED},
/*  614 */  { itemToBeCoded,               NOPARAM,                     "s" STD_SUB_x STD_SUB_y,                       "s" STD_SUB_x STD_SUB_y,                       CAT_FNCT, SLS_UNCHANGED},
/*  615 */  { fnCvtShorttonKg,             multiply,                    "s.t" STD_RIGHT_ARROW "kg",                    "short",                                       CAT_FNCT, SLS_ENABLED  },
/*  616 */  { fnCvtYearS,                  divide,                      "s" STD_RIGHT_ARROW "year",                    "s" STD_RIGHT_ARROW "year",                    CAT_FNCT, SLS_ENABLED  },
/*  617 */  { fnConstant,                  50,                          "T" STD_SUB_0,                                 "T" STD_SUB_0,                                 CAT_CNST, SLS_ENABLED  },
/*  618 */  { fnTan,                       NOPARAM,                     "TAN",                                         "TAN",                                         CAT_FNCT, SLS_ENABLED  },   //JM3
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
/*  631 */  { fnConstant,                  51,                          "T" STD_SUB_p,                                 "T" STD_SUB_P,                                 CAT_CNST, SLS_ENABLED  },
/*  632 */  { fnConstant,                  52,                          "t" STD_SUB_P STD_SUB_L,                       "t" STD_SUB_P STD_SUB_L,                       CAT_CNST, SLS_ENABLED  },
/*  633 */  { itemToBeCoded,               NOPARAM,                     "t" STD_SUB_p "(x)",                           "t" STD_SUB_p "(x)",                           CAT_FNCT, SLS_UNCHANGED},
/*  634 */  { itemToBeCoded,               NOPARAM,                     "t" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", "t" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", CAT_FNCT, SLS_UNCHANGED},
/*  635 */  { itemToBeCoded,               NOPARAM,                     "t" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", "t" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", CAT_FNCT, SLS_UNCHANGED},
/*  636 */  { itemToBeCoded,               NOPARAM,                     "t" STD_SUP_MINUS_1 "(p)",                     "t" STD_SUP_MINUS_1 "(p)",                     CAT_FNCT, SLS_UNCHANGED},
/*  637 */  { itemToBeCoded,               NOPARAM,                     "TRI",                                         "TRIG",                                        CAT_MENU, SLS_UNCHANGED},   //JM
/*  638 */  { fnCvtTrozKg,                 multiply,                    "trz" STD_RIGHT_ARROW "kg",                    "tr.oz",                                       CAT_FNCT, SLS_ENABLED  },
/*  639 */  { itemToBeCoded,               NOPARAM,                     "TVM",                                         "TVM",                                         CAT_MENU, SLS_UNCHANGED},
/*  640 */  { itemToBeCoded,               NOPARAM,                     "t:",                                          "t:",                                          CAT_MENU, SLS_UNCHANGED},
/*  641 */  { itemToBeCoded,               NOPARAM,                     "t" STD_LEFT_RIGHT_ARROWS,                     "t" STD_LEFT_RIGHT_ARROWS,                     CAT_FNCT, SLS_UNCHANGED},
/*  642 */  { fnUlp,                       NOPARAM,                     "ULP?",                                        "ULP?",                                        CAT_FNCT, SLS_ENABLED  },
/*  643 */  { itemToBeCoded,               NOPARAM,                     "U" STD_SUB_n,                                 "U" STD_SUB_n,                                 CAT_FNCT, SLS_UNCHANGED},
/*  644 */  { fnUnitVector,                NOPARAM,                     "UNITV",                                       "UNITV",                                       CAT_FNCT, SLS_ENABLED  },
/*  645 */  { fnIntegerMode,               SIM_UNSIGN,                  "UNSIGN",                                      "UNSIGN",                                      CAT_FNCT, SLS_UNCHANGED},
/*  646 */  { itemToBeCoded,               NOPARAM,                     "UNIT",                                        "UNIT",                                        CAT_MENU, SLS_UNCHANGED},   //JM Change U> arrow to CONV. Changed again to UNIT
/*  647 */  { itemToBeCoded,               NOPARAM,                     "VARMNU",                                      "VARMNU",                                      CAT_FNCT, SLS_UNCHANGED},
/*  648 */  { itemToBeCoded,               NOPARAM,                     "VARS",                                        "VARS",                                        CAT_MENU, SLS_UNCHANGED},
/*  649 */  { fnVersion,                   NOPARAM,                     "VERS?",                                       "VERS?",                                       CAT_FNCT, SLS_UNCHANGED},
/*  650 */  { itemToBeCoded,               NOPARAM,                     "VIEW",                                        "VIEW",                                        CAT_FNCT, SLS_UNCHANGED},
/*  651 */  { fnConstant,                  54,                          "V" STD_SUB_m,                                 "V" STD_SUB_m,                                 CAT_CNST, SLS_ENABLED  },
/*  652 */  { itemToBeCoded,               NOPARAM,                     "Volume:",                                     "Volume:",                                     CAT_MENU, SLS_UNCHANGED},
/*  653 */  { itemToBeCoded,               NOPARAM,                     "WDAY",                                        "WDAY",                                        CAT_FNCT, SLS_UNCHANGED},
/*  654 */  { itemToBeCoded,               NOPARAM,                     "Weibl" STD_SUB_p,                             "Weibl" STD_SUB_p,                             CAT_FNCT, SLS_UNCHANGED},
/*  655 */  { itemToBeCoded,               NOPARAM,                     "Weibl" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Weibl" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   CAT_FNCT, SLS_UNCHANGED},
/*  656 */  { itemToBeCoded,               NOPARAM,                     "Weibl" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Weibl" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   CAT_FNCT, SLS_UNCHANGED},
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
/*  672 */  { fnLogicalXnor,               NOPARAM,                     "XNOR",                                        "XNOR",                                        CAT_FNCT, SLS_ENABLED  },
/*  673 */  { fnLogicalXor,                NOPARAM,                     "XOR",                                         "XOR",                                         CAT_FNCT, SLS_ENABLED  },
/*  674 */  { fnMeanXY,                    NOPARAM,                     STD_x_BAR,                                     STD_x_BAR,                                     CAT_FNCT, SLS_ENABLED  },
/*  675 */  { fnGeometricMeanXY,           NOPARAM,                     STD_x_BAR STD_SUB_G,                           STD_x_BAR STD_SUB_G,                           CAT_FNCT, SLS_UNCHANGED},
/*  676 */  { fnWeightedMeanX,             NOPARAM,                     STD_x_BAR STD_SUB_w,                           STD_x_BAR STD_SUB_w,                           CAT_FNCT, SLS_UNCHANGED},
/*  677 */  { itemToBeCoded,               NOPARAM,                     STD_x_CIRC,                                    STD_x_CIRC,                                    CAT_FNCT, SLS_UNCHANGED},
/*  678 */  { itemToBeCoded,               NOPARAM,                     "X.FN",                                        "X.FN",                                        CAT_MENU, SLS_UNCHANGED},
/*  679 */  { fnFactorial,                 NOPARAM,                     "x!",                                          "x!",                                          CAT_FNCT, SLS_ENABLED  },
/*  680 */  { itemToBeCoded,               NOPARAM,                     "Dist:",                                       "Dist:",                                       CAT_MENU, SLS_UNCHANGED},
/*  681 */  { itemToBeCoded,               NOPARAM,                     "x" STD_RIGHT_ARROW "DATE",                    "x" STD_RIGHT_ARROW "DATE",                    CAT_FNCT, SLS_UNCHANGED},
/*  682 */  { itemToBeCoded,               NOPARAM,                     "x" STD_RIGHT_ARROW STD_alpha,                 "x" STD_RIGHT_ARROW STD_alpha,                 CAT_FNCT, SLS_UNCHANGED},
/*  683 */  { itemToBeCoded,               NOPARAM,                     "x" STD_LEFT_RIGHT_ARROWS,                     "x" STD_LEFT_RIGHT_ARROWS,                     CAT_FNCT, SLS_UNCHANGED},
/*  684 */  { fnSwapXY,                    NOPARAM,                     "x" STD_LEFT_RIGHT_ARROWS "y",                 "x" STD_LEFT_RIGHT_ARROWS "y",                 CAT_FNCT, SLS_ENABLED  },
/*  685 */  { itemToBeCoded,               NOPARAM,                     "x= ?",                                        "x= ?",                                        CAT_FNCT, SLS_UNCHANGED},
/*  686 */  { itemToBeCoded,               NOPARAM,                     "x" STD_NOT_EQUAL " ?",                        "x" STD_NOT_EQUAL " ?",                        CAT_FNCT, SLS_UNCHANGED},
/*  687 */  { itemToBeCoded,               NOPARAM,                     "x=+0?",                                       "x=+0?",                                       CAT_FNCT, SLS_UNCHANGED},
/*  688 */  { itemToBeCoded,               NOPARAM,                     "x=-0?",                                       "x=-0?",                                       CAT_FNCT, SLS_UNCHANGED},
/*  689 */  { itemToBeCoded,               NOPARAM,                     "x" STD_ALMOST_EQUAL " ?",                     "x" STD_ALMOST_EQUAL " ?",                     CAT_FNCT, SLS_UNCHANGED},
/*  690 */  { itemToBeCoded,               NOPARAM,                     "x< ?",                                        "x< ?",                                        CAT_FNCT, SLS_UNCHANGED},
/*  691 */  { itemToBeCoded,               NOPARAM,                     "x" STD_LESS_EQUAL " ?",                       "x" STD_LESS_EQUAL " ?",                       CAT_FNCT, SLS_UNCHANGED},
/*  692 */  { itemToBeCoded,               NOPARAM,                     "x" STD_GREATER_EQUAL " ?",                    "x" STD_GREATER_EQUAL " ?",                    CAT_FNCT, SLS_UNCHANGED},
/*  693 */  { itemToBeCoded,               NOPARAM,                     "x> ?",                                        "x> ?",                                        CAT_FNCT, SLS_UNCHANGED},
/*  694 */  { fnJM,                        1,                           STD_xTH_ROOT STD_y_UNDER_ROOT,                 STD_xTH_ROOT STD_y_UNDER_ROOT,                 CAT_FNCT, SLS_ENABLED  },
/*  695 */  { fnCvtYardM,                  multiply,                    "yd." STD_RIGHT_ARROW "m",                     "yd." STD_RIGHT_ARROW "m",                     CAT_FNCT, SLS_ENABLED  },
/*  696 */  { itemToBeCoded,               NOPARAM,                     "YEAR",                                        "YEAR",                                        CAT_FNCT, SLS_UNCHANGED},
/*  697 */  { fnCvtYearS,                  multiply,                    "year" STD_RIGHT_ARROW "s",                    "year" STD_RIGHT_ARROW "s",                    CAT_FNCT, SLS_ENABLED  },
/*  698 */  { fnPower,                     NOPARAM,                     "y" STD_SUP_x,                                 "y" STD_SUP_x,                                 CAT_FNCT, SLS_ENABLED  },
/*  699 */  { itemToBeCoded,               NOPARAM,                     STD_y_CIRC,                                    STD_y_CIRC,                                    CAT_FNCT, SLS_UNCHANGED},
/*  700 */  { fnDateFormat,                DF_YMD,                      "Y.MD",                                        "Y.MD",                                        CAT_FNCT, SLS_UNCHANGED},
/*  701 */  { itemToBeCoded,               NOPARAM,                     "y" STD_LEFT_RIGHT_ARROWS,                     "y" STD_LEFT_RIGHT_ARROWS,                     CAT_FNCT, SLS_UNCHANGED},
/*  702 */  { fnConstant,                  54,                          "Z" STD_SUB_0,                                 "Z" STD_SUB_0,                                 CAT_CNST, SLS_ENABLED  },
/*  703 */  { itemToBeCoded,               NOPARAM,                     "z" STD_LEFT_RIGHT_ARROWS,                     "z" STD_LEFT_RIGHT_ARROWS,                     CAT_FNCT, SLS_UNCHANGED},
/*  704 */  { fnConstant,                  55,                          STD_alpha,                                     STD_alpha,                                     CAT_CNST, SLS_ENABLED  },
/*  705 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "INTL",                              STD_alpha "INTL",                              CAT_MENU, SLS_UNCHANGED},
/*  706 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "LENG?",                             STD_alpha "LENG?",                             CAT_FNCT, SLS_UNCHANGED},
/*  707 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "MATH",                              STD_alpha "MATH",                              CAT_MENU, SLS_UNCHANGED},
/*  708 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "OFF",                               STD_alpha "OFF",                               CAT_FNCT, SLS_UNCHANGED},
/*  709 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "ON",                                STD_alpha "ON",                                CAT_FNCT, SLS_UNCHANGED},
/*  710 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "POS?",                              STD_alpha "POS?",                              CAT_FNCT, SLS_UNCHANGED},
/*  711 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "RL",                                STD_alpha "RL",                                CAT_FNCT, SLS_UNCHANGED},
/*  712 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "RR",                                STD_alpha "RR",                                CAT_FNCT, SLS_UNCHANGED},
/*  713 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "SL",                                STD_alpha "SL",                                CAT_FNCT, SLS_UNCHANGED},
/*  714 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "STR",                               STD_alpha "STR",                               CAT_MENU, SLS_UNCHANGED},   //JM Changed a.FN to STRNG
/*  715 */  { itemToBeCoded,               NOPARAM,                     STD_ALPHA STD_ELLIPSIS STD_OMEGA,              STD_ALPHA STD_ELLIPSIS STD_OMEGA,              CAT_MENU, SLS_UNCHANGED},
/*  716 */  { itemToBeCoded,               NOPARAM,                     STD_alpha STD_BULLET,                          STD_alpha STD_BULLET,                          CAT_MENU, SLS_UNCHANGED},
/*  717 */  { itemToBeCoded,               NOPARAM,                     STD_alpha STD_RIGHT_ARROW "x",                 STD_alpha STD_RIGHT_ARROW "x",                 CAT_FNCT, SLS_UNCHANGED},
/*  718 */  { itemToBeCoded,               NOPARAM,                     STD_beta "(x,y)",                              STD_beta "(x,y)",                              CAT_FNCT, SLS_UNCHANGED},
/*  719 */  { fnConstant,                  56,                          STD_gamma,                                     STD_gamma,                                     CAT_CNST, SLS_ENABLED  },
/*  720 */  { fnConstant,                  57,                          STD_gamma STD_SUB_E STD_SUB_M,                 STD_gamma STD_SUB_E STD_SUB_M,                 CAT_CNST, SLS_ENABLED  },
/*  721 */  { fnConstant,                  58,                          STD_gamma STD_SUB_p,                           STD_gamma STD_SUB_p,                           CAT_CNST, SLS_ENABLED  },
/*  722 */  { itemToBeCoded,               NOPARAM,                     STD_gamma STD_SUB_x STD_SUB_y,                 STD_gamma STD_SUB_x STD_SUB_y,                 CAT_FNCT, SLS_UNCHANGED},
/*  723 */  { itemToBeCoded,               NOPARAM,                     STD_GAMMA STD_SUB_x STD_SUB_y,                 STD_GAMMA STD_SUB_x STD_SUB_y,                 CAT_FNCT, SLS_UNCHANGED},
/*  724 */  { fnGamma,                     NOPARAM,                     STD_GAMMA "(x)",                               STD_GAMMA "(x)",                               CAT_FNCT, SLS_ENABLED  },
/*  725 */  { itemToBeCoded,               NOPARAM,                     STD_delta "x",                                 STD_delta "x",                                 CAT_NONE, SLS_UNCHANGED},
/*  726 */  { fnDeltaPercent,              NOPARAM,                     STD_DELTA "%",                                 STD_DELTA "%",                                 CAT_FNCT, SLS_ENABLED  },
/*  727 */  { itemToBeCoded,               NOPARAM,                     STD_epsilon,                                   STD_epsilon,                                   CAT_FNCT, SLS_UNCHANGED},
/*  728 */  { fnConstant,                  60,                          STD_epsilon STD_SUB_0,                         STD_epsilon STD_SUB_0,                         CAT_CNST, SLS_ENABLED  },
/*  729 */  { itemToBeCoded,               NOPARAM,                     STD_epsilon STD_SUB_m,                         STD_epsilon STD_SUB_m,                         CAT_FNCT, SLS_UNCHANGED},
/*  730 */  { itemToBeCoded,               NOPARAM,                     STD_epsilon STD_SUB_p,                         STD_epsilon STD_SUB_p,                         CAT_FNCT, SLS_UNCHANGED},
/*  731 */  { itemToBeCoded,               NOPARAM,                     STD_zeta "(x)",                                STD_zeta "(x)",                                CAT_FNCT, SLS_UNCHANGED},
/*  732 */  { fnConstant,                  61,                          STD_lambda STD_SUB_C,                          STD_lambda STD_SUB_C,                          CAT_CNST, SLS_ENABLED  },
/*  733 */  { fnConstant,                  62,                          STD_lambda STD_SUB_C STD_SUB_n,                STD_lambda STD_SUB_C STD_SUB_n,                CAT_CNST, SLS_ENABLED  },
/*  734 */  { fnConstant,                  63,                          STD_lambda STD_SUB_C STD_SUB_p,                STD_lambda STD_SUB_C STD_SUB_p,                CAT_CNST, SLS_ENABLED  },
/*  735 */  { fnConstant,                  64,                          STD_mu STD_SUB_0,                              STD_mu STD_SUB_0,                              CAT_CNST, SLS_ENABLED  },
/*  736 */  { fnConstant,                  65,                          STD_mu STD_SUB_B,                              STD_mu STD_SUB_B,                              CAT_CNST, SLS_ENABLED  },
/*  737 */  { fnConstant,                  66,                          STD_mu STD_SUB_e,                              STD_mu STD_SUB_e,                              CAT_CNST, SLS_ENABLED  },
/*  738 */  { fnConstant,                  67,                          STD_mu STD_SUB_e "/" STD_mu STD_SUB_B,         STD_mu STD_SUB_e "/" STD_mu STD_SUB_B,         CAT_CNST, SLS_ENABLED  },
/*  739 */  { fnConstant,                  68,                          STD_mu STD_SUB_n,                              STD_mu STD_SUB_n,                              CAT_CNST, SLS_ENABLED  },
/*  740 */  { fnConstant,                  69,                          STD_mu STD_SUB_p,                              STD_mu STD_SUB_p,                              CAT_CNST, SLS_ENABLED  },
/*  741 */  { fnConstant,                  70,                          STD_mu STD_SUB_u,                              STD_mu STD_SUB_u,                              CAT_CNST, SLS_ENABLED  },
/*  742 */  { fnConstant,                  71,                          STD_mu STD_SUB_mu,                             STD_mu STD_SUB_mu,                             CAT_CNST, SLS_ENABLED  },
/*  743 */  { itemToBeCoded,               NOPARAM,                     STD_PI STD_SUB_n,                              STD_PI STD_SUB_n,                              CAT_FNCT, SLS_UNCHANGED},
/*  744 */  { fnPi,                        NOPARAM,                     STD_pi,                                        STD_pi,                                        CAT_NONE, SLS_ENABLED  },
/*  745 */  { itemToBeCoded,               NOPARAM,                     STD_SIGMA STD_SUB_n,                           STD_SIGMA STD_SUB_n,                           CAT_FNCT, SLS_UNCHANGED},
/*  746 */  { itemToBeCoded,               NOPARAM,                     STD_sigma,                                     STD_sigma,                                     CAT_FNCT, SLS_UNCHANGED},
/*  747 */  { fnConstant,                  72,                          STD_sigma STD_SUB_B,                           STD_sigma STD_SUB_B,                           CAT_CNST, SLS_ENABLED  },
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
/*  764 */  { fnConstant,                  73,                          STD_PHI,                                       STD_PHI,                                       CAT_CNST, SLS_ENABLED  },
/*  765 */  { fnConstant,                  74,                          STD_PHI STD_SUB_0,                             STD_PHI STD_SUB_0,                             CAT_CNST, SLS_ENABLED  },
/*  766 */  { fnRandomI,                   NOPARAM,                     "RANI#",                                       "RANI#",                                       CAT_FNCT, SLS_ENABLED  },
/*  767 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "x",                               STD_PRINTER "x",                               CAT_FNCT, SLS_UNCHANGED},
/*  768 */  { itemToBeCoded,               NOPARAM,                     "SYS.FL",                                      "SYS.FL",                                      CAT_MENU, SLS_UNCHANGED},
/*  769 */  { fnRange,                     NOPARAM,                     "RANGE",                                       "RANGE",                                       CAT_FNCT, SLS_ENABLED  },
/*  770 */  { fnGetRange,                  NOPARAM,                     "RANGE?",                                      "RANGE?",                                      CAT_FNCT, SLS_ENABLED  },
/*  771 */  { itemToBeCoded,               NOPARAM,                     STD_chi STD_SUP_2  STD_SUB_p "(x)",            STD_chi STD_SUP_2  STD_SUB_p "(x)",            CAT_FNCT, SLS_UNCHANGED},
/*  772 */  { itemToBeCoded,               NOPARAM,                     STD_chi STD_SUP_2 STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", STD_chi STD_SUP_2 STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", CAT_FNCT, SLS_UNCHANGED},
/*  773 */  { itemToBeCoded,               NOPARAM,                     STD_chi STD_SUP_2 STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", STD_chi STD_SUP_2 STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", CAT_FNCT, SLS_UNCHANGED},
/*  774 */  { itemToBeCoded,               NOPARAM,                     "(" STD_chi STD_SUP_2 ")" STD_SUP_MINUS_1,     "(" STD_chi STD_SUP_2 ")" STD_SUP_MINUS_1,     CAT_FNCT, SLS_UNCHANGED},
/*  775 */  { itemToBeCoded,               NOPARAM,                     STD_chi STD_SUP_2 ":",                         STD_chi STD_SUP_2 ":",                         CAT_MENU, SLS_UNCHANGED},
/*  776 */  { fnConstant,                  75,                          STD_omega,                                     STD_omega,                                     CAT_CNST, SLS_ENABLED  },
/*  777 */  { fnM1Pow,                     NOPARAM,                     "(-1)" STD_SUP_x,                              "(-1)" STD_SUP_x,                              CAT_FNCT, SLS_ENABLED  },
/*  778 */  { fnAdd,                       ITM_ADD,                     "+",                                           "+",                                           CAT_FNCT, SLS_ENABLED  },
/*  779 */  { fnChangeSign,                ITM_CHS,                     "CHS",                                         "CHS",                                         CAT_FNCT, SLS_ENABLED  },   //JM Change +/- to CHS
/*  780 */  { fnSubtract,                  ITM_SUB,                     "-",                                           "-",                                           CAT_FNCT, SLS_ENABLED  },
/*  781 */  { fnConstant,                  76,                          "-" STD_INFINITY,                              "-" STD_INFINITY,                              CAT_CNST, SLS_ENABLED  },
/*  782 */  { fnMultiply,                  ITM_MULT,                    STD_CROSS,                                     STD_CROSS,                                     CAT_FNCT, SLS_ENABLED  },
/*  783 */  { itemToBeCoded,               NOPARAM,                     STD_CROSS "MOD",                               STD_CROSS "MOD",                               CAT_FNCT, SLS_UNCHANGED},
/*  784 */  { fnDivide,                    ITM_DIV,                     STD_DIVIDE,                                    STD_DIVIDE,                                    CAT_FNCT, SLS_ENABLED  },
/*  785 */  { itemToBeCoded,               NOPARAM,                     STD_PLUS_MINUS STD_INFINITY "?",               STD_PLUS_MINUS STD_INFINITY "?",               CAT_FNCT, SLS_UNCHANGED},
/*  786 */  { addItemToBuffer,             ITM_INDIRECTION,             STD_RIGHT_ARROW,                               STD_RIGHT_ARROW,                               CAT_NONE, SLS_UNCHANGED},
/*  787 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "DATE",                        STD_RIGHT_ARROW "DATE",                        CAT_FNCT, SLS_UNCHANGED},
/*  788 */  { fnCvtFromCurrentAngularMode, AM_DEGREE,                   STD_RIGHT_ARROW "DEG",                         STD_RIGHT_ARROW "DEG",                         CAT_FNCT, SLS_ENABLED  },
/*  789 */  { fnCvtFromCurrentAngularMode, AM_DMS,                      STD_RIGHT_ARROW "D.MS",                        STD_RIGHT_ARROW "D.MS",                        CAT_FNCT, SLS_ENABLED  },
/*  790 */  { fnCvtFromCurrentAngularMode, AM_GRAD,                     STD_RIGHT_ARROW "GRAD",                        STD_RIGHT_ARROW "GRAD",                        CAT_FNCT, SLS_ENABLED  },
/*  791 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "HR",                          ".d",                                          CAT_FNCT, SLS_ENABLED  },
/*  792 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "H.MS",                        STD_RIGHT_ARROW "h.ms",                        CAT_FNCT, SLS_UNCHANGED},   //JM mod
/*  793 */  { fnChangeBase,                TM_VALUE_CHB,                STD_RIGHT_ARROW "INT",                         "#",                                           CAT_FNCT, SLS_UNCHANGED},
/*  794 */  { fnCvtFromCurrentAngularMode, AM_MULTPI,                   STD_RIGHT_ARROW "MUL" STD_pi,                  STD_RIGHT_ARROW "MUL" STD_pi,                  CAT_FNCT, SLS_ENABLED  },
/*  795 */  { fnJM_fnToPolar,              NOPARAM,                     STD_RIGHT_ARROW "POLAR",                       STD_RIGHT_ARROW "P",                           CAT_FNCT, SLS_ENABLED  },   //JM TEXT & point to function to add POLAR/RECT
/*  796 */  { fnCvtFromCurrentAngularMode, AM_RADIAN,                   STD_RIGHT_ARROW "RAD",                         STD_RIGHT_ARROW "RAD",                         CAT_FNCT, SLS_ENABLED  },
/*  797 */  { fnToReal,                    NOPARAM,                     STD_RIGHT_ARROW "REAL",                        ".d",                                          CAT_FNCT, SLS_ENABLED  },
/*  798 */  { fnJM_fnToRect,               NOPARAM,                     STD_RIGHT_ARROW "RECT",                        STD_RIGHT_ARROW "R",                           CAT_FNCT, SLS_ENABLED  },   //SWAPPED ARROW DIRECTION & JM TEXT & point to function to add POLAR/RECT
/*  799 */  { fnCvtDegToDms,               NOPARAM,                     "D" STD_RIGHT_ARROW "D.MS",                    "D" STD_RIGHT_ARROW "D.MS",                    CAT_FNCT, SLS_ENABLED  },
/*  800 */  { itemToBeCoded,               NOPARAM,                     STD_UP_ARROW "Lim",                            STD_UP_ARROW "Lim",                            CAT_RVAR, SLS_UNCHANGED},
/*  801 */  { itemToBeCoded,               NOPARAM,                     STD_DOWN_ARROW "Lim",                          STD_DOWN_ARROW "Lim",                          CAT_RVAR, SLS_UNCHANGED},
/*  802 */  { itemToBeCoded,               NOPARAM,                     STD_LEFT_RIGHT_ARROWS,                         STD_LEFT_RIGHT_ARROWS,                         CAT_FNCT, SLS_UNCHANGED},
/*  803 */  { fnPercent,                   NOPARAM,                     "%",                                           "%",                                           CAT_FNCT, SLS_ENABLED  },
/*  804 */  { fnPercentMRR,                NOPARAM,                     "%MRR",                                        "%MRR",                                        CAT_FNCT, SLS_ENABLED  },
/*  805 */  { fnPercentT,                  NOPARAM,                     "%T",                                          "%T",                                          CAT_FNCT, SLS_ENABLED  },
/*  806 */  { fnPercentSigma,              NOPARAM,                     "%" STD_SIGMA,                                 "%" STD_SIGMA,                                 CAT_FNCT, SLS_ENABLED  },
/*  807 */  { fnPercentPlusMG,             NOPARAM,                     "%+MG",                                        "%+MG",                                        CAT_FNCT, SLS_ENABLED  },
/*  808 */  { fnSquareRoot,                NOPARAM,                     STD_SQUARE_ROOT STD_x_UNDER_ROOT,              STD_SQUARE_ROOT STD_x_UNDER_ROOT,              CAT_FNCT, SLS_ENABLED  },
/*  809 */  { itemToBeCoded,               NOPARAM,                     STD_INTEGRAL,                                  STD_INTEGRAL,                                  CAT_FNCT, SLS_UNCHANGED},
/*  810 */  { itemToBeCoded,               NOPARAM,                     STD_INTEGRAL "f",                              STD_INTEGRAL "f",                              CAT_MENU, SLS_UNCHANGED},
/*  811 */  { itemToBeCoded,               NOPARAM,                     STD_INTEGRAL "fdx",                            STD_INTEGRAL "fdx",                            CAT_MENU, SLS_UNCHANGED},
/*  812 */  { fnConstant,                  77,                          STD_INFINITY,                                  STD_INFINITY,                                  CAT_CNST, SLS_ENABLED  },
/*  813 */  { itemToBeCoded,               NOPARAM,                     "^MOD",                                        "^MOD",                                        CAT_FNCT, SLS_UNCHANGED},
/*  814 */  { itemToBeCoded,               NOPARAM,                     "|M|",                                         "|M|",                                         CAT_FNCT, SLS_UNCHANGED},
/*  815 */  { fnMagnitude,                 NOPARAM,                     "|x|",                                         "|x|",                                         CAT_FNCT, SLS_ENABLED  },
/*  816 */  { fnParallel,                  NOPARAM,                     "|" STD_SPACE_3_PER_EM "|",                    "|" STD_SPACE_3_PER_EM "|",                    CAT_FNCT, SLS_ENABLED  },   //JM
/*  817 */  { itemToBeCoded,               NOPARAM,                     "[M]" STD_SUP_T,                               "[M]" STD_SUP_T,                               CAT_FNCT, SLS_UNCHANGED},
/*  818 */  { itemToBeCoded,               NOPARAM,                     "[M]" STD_SUP_MINUS_1,                         "[M]" STD_SUP_MINUS_1,                         CAT_FNCT, SLS_UNCHANGED},
/*  819 */  { fnArg,                       NOPARAM,                     STD_MEASURED_ANGLE,                            STD_MEASURED_ANGLE,                            CAT_FNCT, SLS_ENABLED  },   //JM CALC
/*  820 */  { fnCvtToCurrentAngularMode,   AM_MULTPI,                   "MUL" STD_pi STD_RIGHT_ARROW,                  "MUL" STD_pi STD_RIGHT_ARROW,                  CAT_FNCT, SLS_ENABLED  },
/*  821 */  { itemToBeCoded,               NOPARAM,                     "CONV",                                        "CONV",                                        CAT_MENU, SLS_UNCHANGED},   //JM Change to text DRG and change again to CONV
/*  822 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "ADV",                             STD_PRINTER "ADV",                             CAT_FNCT, SLS_UNCHANGED},
/*  823 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "CHAR",                            STD_PRINTER "CHAR",                            CAT_FNCT, SLS_UNCHANGED},
/*  824 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "DLAY",                            STD_PRINTER "DLAY",                            CAT_FNCT, SLS_UNCHANGED},
/*  825 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "LCD",                             STD_PRINTER "LCD",                             CAT_FNCT, SLS_UNCHANGED},
/*  826 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "MODE",                            STD_PRINTER "MODE",                            CAT_FNCT, SLS_UNCHANGED},
/*  827 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "PROG",                            STD_PRINTER "PROG",                            CAT_FNCT, SLS_UNCHANGED},
/*  828 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "r",                               STD_PRINTER "r",                               CAT_FNCT, SLS_UNCHANGED},
/*  829 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "REGS",                            STD_PRINTER "REGS",                            CAT_FNCT, SLS_UNCHANGED},
/*  830 */  { fnP_All_Regs,                1,                           STD_PRINTER "STK",                             STD_PRINTER "STK",                             CAT_FNCT, SLS_UNCHANGED},
/*  831 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "TAB",                             STD_PRINTER "TAB",                             CAT_FNCT, SLS_UNCHANGED},
/*  832 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "USER",                            STD_PRINTER "USER",                            CAT_FNCT, SLS_UNCHANGED},
/*  833 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "WIDTH",                           STD_PRINTER "WIDTH",                           CAT_FNCT, SLS_UNCHANGED},
/*  834 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER STD_SIGMA,                         STD_PRINTER STD_SIGMA,                         CAT_FNCT, SLS_UNCHANGED},
/*  835 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "#",                               STD_PRINTER "#",                               CAT_FNCT, SLS_UNCHANGED},
/*  836 */  { itemToBeCoded,               NOPARAM,                     "#",                                           "#",                                           CAT_NONE, SLS_UNCHANGED},
/*  837 */  { fnCountBits,                 NOPARAM,                     "#B",                                          "#B",                                          CAT_FNCT, SLS_ENABLED  },
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

/*  874 */  { fontBrowser,                 NOPARAM,                     "FBR",                                         "FBR",                                         CAT_FNCT, SLS_UNCHANGED},   // Font Browser
/*  875 */  { itemToBeCoded,               NOPARAM,                     "CFG",                                         "CFG",                                         CAT_MENU, SLS_UNCHANGED},   // JM Menu not defined 
/*  876 */  { itemToBeCoded,               NOPARAM,                     STD_alpha STD_ELLIPSIS STD_omega,              STD_alpha STD_ELLIPSIS STD_omega,              CAT_MENU, SLS_UNCHANGED},   // Small greek letters
/*  877 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "intl",                              STD_alpha "intl",                              CAT_MENU, SLS_UNCHANGED},   // Small intl letters
/*  878 */  { addItemToBuffer,             REGISTER_X,                  "REG_X",                                       "X",                                           CAT_NONE, SLS_UNCHANGED},   // The order
/*  879 */  { addItemToBuffer,             REGISTER_Y,                  "REG_Y",                                       "Y",                                           CAT_NONE, SLS_UNCHANGED},   // of these 8
/*  880 */  { addItemToBuffer,             REGISTER_Z,                  "REG_Z",                                       "Z",                                           CAT_NONE, SLS_UNCHANGED},   // lines MUST
/*  881 */  { addItemToBuffer,             REGISTER_T,                  "REG_T",                                       "T",                                           CAT_NONE, SLS_UNCHANGED},   // be kept as
/*  882 */  { addItemToBuffer,             REGISTER_A,                  "REG_A",                                       "A",                                           CAT_NONE, SLS_UNCHANGED},   // is. Do not
/*  883 */  { addItemToBuffer,             REGISTER_B,                  "REG_B",                                       "B",                                           CAT_NONE, SLS_UNCHANGED},   // put them in
/*  884 */  { addItemToBuffer,             REGISTER_C,                  "REG_C",                                       "C",                                           CAT_NONE, SLS_UNCHANGED},   // alphabetical
/*  885 */  { addItemToBuffer,             REGISTER_D,                  "REG_D",                                       "D",                                           CAT_NONE, SLS_UNCHANGED},   // order!
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
/*  961 */  { itemToBeCoded,               NOPARAM,                     "0961",                                        "0961",                                        CAT_NONE, SLS_UNCHANGED},
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
/* 1328 */  { addItemToBuffer,             CHR_INVERTED_EXCLAMATION_MARK, "",                                            STD_INVERTED_EXCLAMATION_MARK,                 CAT_NONE, SLS_UNCHANGED},
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
/* 1487 */  { addItemToBuffer,             ITM_EXPONENT,                "",                                            "EEX",                                         CAT_NONE, SLS_UNCHANGED},   //JM Change E to EEX
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
/* 1506 */  { fnJM,                        31,                          "",                                            STD_UNDO,                                      CAT_NONE, SLS_UNCHANGED},   //JM added temporary routine for undo
/* 1507 */  { itemToBeCoded,               NOPARAM,                     "PRGM",                                        "PRGM",                                        CAT_NONE, SLS_UNCHANGED},   //JM Change P/R to PRGM
/* 1508 */  { itemToBeCoded,               NOPARAM,                     "R/S",                                         "R/S",                                         CAT_NONE, SLS_UNCHANGED},
/* 1509 */  { itemToBeCoded,               NOPARAM,                     "",                                            "Not",                                         CAT_NONE, SLS_UNCHANGED},
/* 1510 */  { itemToBeCoded,               NOPARAM,                     "",                                            "yet",                                         CAT_NONE, SLS_UNCHANGED},
/* 1511 */  { itemToBeCoded,               NOPARAM,                     "",                                            "defined",                                     CAT_NONE, SLS_UNCHANGED},
/* 1512 */  { itemToBeCoded,               NOPARAM,                     "",                                            "Tam",                                         CAT_NONE, SLS_UNCHANGED},
/* 1513 */  { itemToBeCoded,               NOPARAM,                     "",                                            "TamCmp",                                      CAT_NONE, SLS_UNCHANGED},
/* 1514 */  { itemToBeCoded,               NOPARAM,                     "",                                            "TamStoRcl",                                   CAT_NONE, SLS_UNCHANGED},
/* 1515 */  { fnUserMode,                  NOPARAM,                     "USER",                                        "USER",                                        CAT_NONE, SLS_UNCHANGED},
/* 1516 */  { fnKeyCC,                     NOPARAM,                     "CC",                                          "CC",                                          CAT_FNCT, SLS_UNCHANGED},   //JM Change CC to COMPLEX
/* 1517 */  { itemToBeCoded,               NOPARAM,                     "",                                            "f",                                           CAT_NONE, SLS_UNCHANGED},
/* 1518 */  { itemToBeCoded,               NOPARAM,                     "",                                            "g",                                           CAT_NONE, SLS_UNCHANGED},
/* 1519 */  { fnKeyUp,                     NOPARAM,                     "UP",                                          STD_UP_ARROW,                                  CAT_NONE, SLS_UNCHANGED},
/* 1520 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_HAMBURGER STD_BST,                         CAT_NONE, SLS_UNCHANGED},
/* 1521 */  { fnKeyDown,                   NOPARAM,                     "DOWN",                                        STD_DOWN_ARROW,                                CAT_NONE, SLS_UNCHANGED},
/* 1522 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_HAMBURGER STD_SST,                         CAT_NONE, SLS_UNCHANGED},
/* 1523 */  { fnKeyExit,                   NOPARAM,                     "EXIT",                                        "EXIT",                                        CAT_NONE, SLS_UNCHANGED},
/* 1524 */  { fnKeyBackspace,              NOPARAM,                     "BKSPC",                                       STD_LEFT_ARROW,                                CAT_NONE, SLS_UNCHANGED},
/* 1525 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PRINTER "x",                               CAT_NONE, SLS_UNCHANGED},
#ifdef JM_LAYOUT_2_DM42_STRICT //JM LAYOUT 2. DM42 STRICT.
/* 1526 */  { fnAim,                       NOPARAM,                     "",                                            "ALPHA",                                       CAT_NONE, SLS_UNCHANGED},   //JM
#endif //JM END OF LAYOUT 2 DM42 STRICT.
#ifdef JM_LAYOUT_1A
/* 1526 */  { fnAim,                       NOPARAM,                     "",                                            STD_alpha,                                     CAT_NONE, SLS_UNCHANGED},   //JM
#endif //JM END OF LAYOUT 1
/* 1527 */  { fnKeyDotD/*fnJM 255*/,       NOPARAM,                     ".d",                                          ".d",                                          CAT_NONE, SLS_UNCHANGED},   //JM mod
/* 1528 */  { fnCvtQuartM3,                multiply,                    "qt." STD_RIGHT_ARROW "m" STD_SUP_3,           "qt." STD_RIGHT_ARROW "m" STD_SUP_3,           CAT_FNCT, SLS_ENABLED  },
/* 1529 */  { fnCvtQuartM3,                divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "qt.",           "m" STD_SUP_3 STD_RIGHT_ARROW "qt.",           CAT_FNCT, SLS_ENABLED  },
/* 1530 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "SP",                          STD_RIGHT_ARROW "SP",                          CAT_FNCT, SLS_ENABLED  },
/* 1531 */  { fnShow_SCROLL,               NOPARAM,                     "SHOW",                                        "SHOW",                                        CAT_FNCT, SLS_UNCHANGED},
/* 1532 */  { backToSystem,                NOPARAM,                     "SYSTEM",                                      "SYSTEM",                                      CAT_NONE, SLS_UNCHANGED},
/* 1533 */  { fnCvtDmsToDeg,               NOPARAM,                     "D.MS" STD_RIGHT_ARROW "D",                    "D.MS" STD_RIGHT_ARROW "D",                    CAT_FNCT, SLS_ENABLED  },
/* 1534 */  { fnArg,                       NOPARAM,                     "V" STD_MEASURED_ANGLE,                        STD_MEASURED_ANGLE,                            CAT_FNCT, SLS_ENABLED  },
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
/* 1559 */  { fnHarmonicMeanXY,            NOPARAM,                     STD_x_BAR STD_SUB_H,                           STD_x_BAR STD_SUB_H,                           CAT_FNCT, SLS_UNCHANGED},
/* 1560 */  { fnRMSMeanXY,                 NOPARAM,                     STD_x_BAR STD_SUB_R STD_SUB_M STD_SUB_S,       STD_x_BAR STD_SUB_R STD_SUB_M STD_SUB_S,       CAT_FNCT, SLS_UNCHANGED},
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



/* 1589 */  { itemToBeCoded,               NOPARAM,                     "VAR",                                         "VAR",                                         CAT_MENU, SLS_UNCHANGED},
/* 1590 */  { itemToBeCoded,               NOPARAM,                     "",                                            "TamFlag",                                     CAT_NONE, SLS_UNCHANGED},
/* 1591 */  { itemToBeCoded,               NOPARAM,                     "1591",                                        "1591",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1592 */  { itemToBeCoded,               NOPARAM,                     "1592",                                        "1592",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1593 */  { itemToBeCoded,               NOPARAM,                     "1593",                                        "1593",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1594 */  { itemToBeCoded,               NOPARAM,                     "1594",                                        "1594",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1595 */  { itemToBeCoded,               NOPARAM,                     "1595",                                        "1595",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1596 */  { itemToBeCoded,               NOPARAM,                     "1596",                                        "1596",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1597 */  { itemToBeCoded,               NOPARAM,                     "1597",                                        "1597",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1598 */  { itemToBeCoded,               NOPARAM,                     "1598",                                        "1598",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1599 */  { itemToBeCoded,               NOPARAM,                     "1599",                                        "1599",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1600 */  { itemToBeCoded,               NOPARAM,                     "1600",                                        "1600",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1601 */  { itemToBeCoded,               NOPARAM,                     "1601",                                        "1601",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1602 */  { itemToBeCoded,               NOPARAM,                     "1602",                                        "1602",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1603 */  { itemToBeCoded,               NOPARAM,                     "1603",                                        "1603",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1604 */  { itemToBeCoded,               NOPARAM,                     "1604",                                        "1604",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1605 */  { itemToBeCoded,               NOPARAM,                     "1605",                                        "1605",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1606 */  { itemToBeCoded,               NOPARAM,                     "1606",                                        "1606",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1607 */  { itemToBeCoded,               NOPARAM,                     "1607",                                        "1607",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1608 */  { itemToBeCoded,               NOPARAM,                     "1608",                                        "1608",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1609 */  { itemToBeCoded,               NOPARAM,                     "1609",                                        "1609",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1610 */  { itemToBeCoded,               NOPARAM,                     "1610",                                        "1610",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1611 */  { itemToBeCoded,               NOPARAM,                     "1611",                                        "1611",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1612 */  { itemToBeCoded,               NOPARAM,                     "1612",                                        "1612",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1613 */  { itemToBeCoded,               NOPARAM,                     "1613",                                        "1613",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1615 */  { itemToBeCoded,               NOPARAM,                     "1615",                                        "1615",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1615 */  { itemToBeCoded,               NOPARAM,                     "1615",                                        "1615",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1616 */  { itemToBeCoded,               NOPARAM,                     "1616",                                        "1616",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1617 */  { itemToBeCoded,               NOPARAM,                     "1617",                                        "1617",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1618 */  { itemToBeCoded,               NOPARAM,                     "1618",                                        "1618",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1619 */  { itemToBeCoded,               NOPARAM,                     "1619",                                        "1619",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1620 */  { itemToBeCoded,               NOPARAM,                     "1620",                                        "1620",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1621 */  { itemToBeCoded,               NOPARAM,                     "1621",                                        "1621",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1622 */  { itemToBeCoded,               NOPARAM,                     "1622",                                        "1622",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1623 */  { itemToBeCoded,               NOPARAM,                     "1623",                                        "1623",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1624 */  { itemToBeCoded,               NOPARAM,                     "1624",                                        "1624",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1625 */  { itemToBeCoded,               NOPARAM,                     "1625",                                        "1625",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1626 */  { itemToBeCoded,               NOPARAM,                     "1626",                                        "1626",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1627 */  { itemToBeCoded,               NOPARAM,                     "1627",                                        "1627",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1628 */  { itemToBeCoded,               NOPARAM,                     "1628",                                        "1628",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1629 */  { itemToBeCoded,               NOPARAM,                     "1629",                                        "1629",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1630 */  { itemToBeCoded,               NOPARAM,                     "1630",                                        "1630",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1631 */  { itemToBeCoded,               NOPARAM,                     "1631",                                        "1631",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1632 */  { itemToBeCoded,               NOPARAM,                     "1632",                                        "1632",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1633 */  { itemToBeCoded,               NOPARAM,                     "1633",                                        "1633",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1634 */  { itemToBeCoded,               NOPARAM,                     "1634",                                        "1634",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1635 */  { itemToBeCoded,               NOPARAM,                     "1635",                                        "1635",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1636 */  { itemToBeCoded,               NOPARAM,                     "1636",                                        "1636",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1637 */  { itemToBeCoded,               NOPARAM,                     "1637",                                        "1637",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1638 */  { itemToBeCoded,               NOPARAM,                     "1638",                                        "1638",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1639 */  { itemToBeCoded,               NOPARAM,                     "1639",                                        "1639",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1640 */  { itemToBeCoded,               NOPARAM,                     "1640",                                        "1640",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1641 */  { itemToBeCoded,               NOPARAM,                     "1641",                                        "1641",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1642 */  { itemToBeCoded,               NOPARAM,                     "1642",                                        "1642",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1643 */  { itemToBeCoded,               NOPARAM,                     "1643",                                        "1643",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1644 */  { itemToBeCoded,               NOPARAM,                     "1644",                                        "1644",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1645 */  { itemToBeCoded,               NOPARAM,                     "1645",                                        "1645",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1646 */  { itemToBeCoded,               NOPARAM,                     "1646",                                        "1646",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1647 */  { itemToBeCoded,               NOPARAM,                     "1647",                                        "1647",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1648 */  { itemToBeCoded,               NOPARAM,                     "1648",                                        "1648",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1649 */  { itemToBeCoded,               NOPARAM,                     "1649",                                        "1649",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1650 */  { itemToBeCoded,               NOPARAM,                     "1650",                                        "1650",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1651 */  { itemToBeCoded,               NOPARAM,                     "1651",                                        "1651",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1652 */  { itemToBeCoded,               NOPARAM,                     "1652",                                        "1652",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1653 */  { itemToBeCoded,               NOPARAM,                     "1653",                                        "1653",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1654 */  { itemToBeCoded,               NOPARAM,                     "1654",                                        "1654",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1655 */  { itemToBeCoded,               NOPARAM,                     "1655",                                        "1655",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1656 */  { itemToBeCoded,               NOPARAM,                     "1656",                                        "1656",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1657 */  { itemToBeCoded,               NOPARAM,                     "1657",                                        "1657",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1658 */  { itemToBeCoded,               NOPARAM,                     "1658",                                        "1658",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1659 */  { itemToBeCoded,               NOPARAM,                     "1659",                                        "1659",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1660 */  { itemToBeCoded,               NOPARAM,                     "1660",                                        "1660",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1661 */  { itemToBeCoded,               NOPARAM,                     "1661",                                        "1661",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1662 */  { itemToBeCoded,               NOPARAM,                     "1662",                                        "1662",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1663 */  { itemToBeCoded,               NOPARAM,                     "1663",                                        "1663",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1664 */  { itemToBeCoded,               NOPARAM,                     "1664",                                        "1664",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1665 */  { itemToBeCoded,               NOPARAM,                     "1665",                                        "1665",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1666 */  { itemToBeCoded,               NOPARAM,                     "1666",                                        "1666",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1667 */  { itemToBeCoded,               NOPARAM,                     "1667",                                        "1667",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1668 */  { itemToBeCoded,               NOPARAM,                     "1668",                                        "1668",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1669 */  { itemToBeCoded,               NOPARAM,                     "1669",                                        "1669",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1670 */  { itemToBeCoded,               NOPARAM,                     "1670",                                        "1670",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1671 */  { itemToBeCoded,               NOPARAM,                     "1671",                                        "1671",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1672 */  { itemToBeCoded,               NOPARAM,                     "1672",                                        "1672",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1673 */  { itemToBeCoded,               NOPARAM,                     "1673",                                        "1673",                                        CAT_FREE, SLS_UNCHANGED},   //Spare numbers allowed for Martin
/* 1674 */  { fnCvtmmhgPa,                 multiply,                    "mmHg" STD_GREATER_THAN "Pa",                  "mm.Hg",                                       CAT_FNCT, SLS_ENABLED  },   //JM UNIT conversion
/* 1675 */  { fnCvtmmhgPa,                 divide,                      "Pa" STD_GREATER_THAN "mmHg",                  "Pa" STD_RIGHT_ARROW,                          CAT_FNCT, SLS_ENABLED  },   //JM UNIT conversion
/* 1676 */  { fnCvtmmhgPa,                 multiply,                    "mmHg" STD_GREATER_THAN "Pa",                  STD_RIGHT_ARROW " Pa",                         CAT_DUPL, SLS_ENABLED  },   //JM UNIT conversion
/* 1677 */  { fnCvtmmhgPa,                 divide,                      "Pa" STD_GREATER_THAN "mmHg",                  "mm.Hg",                                       CAT_DUPL, SLS_ENABLED  },   //JM UNIT conversion

/* 1678 */  { fnSetSetJM,                  JC_ERPN,                     "eRPN",                                        "eRPN",                                        CAT_FNCT, SLS_UNCHANGED},   //JM eRPN
/* 1679 */  { fnSetSetJM,                  JC_HOME_TRIPLE,              "HOME.3",                                      "HOME.3",                                      CAT_FNCT, SLS_UNCHANGED},   //JM HOME.3
/* 1680 */  { fnSetSetJM,                  JC_SHFT_4s,                  "SH_4s",                                       "SH_4s",                                       CAT_NONE, SLS_UNCHANGED},   //JM SHIFT CANCEL
/* 1681 */  { itemToBeCoded,               NOPARAM,                     "HOME",                                        "HOME",                                        CAT_MENU, SLS_UNCHANGED},   //JM HOME
/* 1682 */  { fnDisplayFormatSigFig,       TM_VALUE,                    "SIG",                                         "SIG",                                         CAT_FNCT, SLS_UNCHANGED},   //JM SIGFIG
/* 1683 */  { itemToBeCoded,               NOPARAM,                     "ALPHA",                                       "ALPHA",                                       CAT_FNCT, SLS_UNCHANGED},   //JM ALPHA
/* 1684 */  { itemToBeCoded,               NOPARAM,                     "BASE",                                        "BASE",                                        CAT_MENU, SLS_UNCHANGED},   //JM BASE
/* 1685 */  { fnChangeBase,                2,                           STD_RIGHT_ARROW "BIN",                         STD_RIGHT_ARROW "BIN",                         CAT_FNCT, SLS_UNCHANGED},   //JM HEX
/* 1686 */  { fnChangeBase,                8,                           STD_RIGHT_ARROW "OCT",                         STD_RIGHT_ARROW "OCT",                         CAT_FNCT, SLS_UNCHANGED},   //JM HEX
/* 1687 */  { fnChangeBase,                10,                          STD_RIGHT_ARROW "DEC",                         STD_RIGHT_ARROW "DEC",                         CAT_FNCT, SLS_UNCHANGED},   //JM HEX
/* 1688 */  { fnChangeBase,                16,                          STD_RIGHT_ARROW "HEX",                         STD_RIGHT_ARROW "HEX",                         CAT_FNCT, SLS_UNCHANGED},   //JM HEX
/* 1689 */  { fnSetWordSize,               8,                           "8-BIT",                                       "8-BIT",                                       CAT_FNCT, SLS_UNCHANGED},   //JM HEX
/* 1690 */  { fnSetWordSize,               16,                          "16-BIT",                                      "16-BIT",                                      CAT_FNCT, SLS_UNCHANGED},   //JM HEX
/* 1691 */  { fnSetWordSize,               32,                          "32-BIT",                                      "32-BIT",                                      CAT_FNCT, SLS_UNCHANGED},   //JM HEX
/* 1692 */  { fnSetWordSize,               64,                          "64-BIT",                                      "64-BIT",                                      CAT_FNCT, SLS_UNCHANGED},   //JM HEX
/* 1693 */  { fnDisplayFormatUnit,         TM_VALUE,                    "UNIT",                                        "UNIT",                                        CAT_FNCT, SLS_UNCHANGED},   //JM UNIT
/* 1694 */  { fnShowJM,                    JC_ERPN,                     "eRPN?",                                       "eRPN?",                                       CAT_FNCT, SLS_ENABLED  },   //JM SHOW
/* 1695 */  { fnSetSetJM,                  JC_BCR,                      "CPXRES",                                      "CPXRES",                                      CAT_FNCT, SLS_UNCHANGED},   //dr
/* 1696 */  { fnSetSetJM,                  JC_BLZ,                      "LEAD0",                                       "LEAD0",                                       CAT_FNCT, SLS_UNCHANGED},   //dr
/* 1697 */  { addItemToBuffer,             CHR_QOPPA,                   "",                                            STD_QOPPA,                                     CAT_NONE, SLS_UNCHANGED},   //JM GREEK
/* 1698 */  { addItemToBuffer,             CHR_DIGAMMA,                 "",                                            STD_DIGAMMA,                                   CAT_NONE, SLS_UNCHANGED},   //JM GREEK
/* 1699 */  { addItemToBuffer,             CHR_SAMPI,                   "",                                            STD_SAMPI,                                     CAT_NONE, SLS_UNCHANGED},   //JM GREEK

/* 1700 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1701 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1702 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1703 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1704 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1705 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1706 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1707 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1708 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1709 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1710 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1711 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1712 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1713 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1714 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1715 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1716 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1717 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1718 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1719 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1720 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1721 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1722 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1723 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1724 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1725 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1726 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1727 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1728 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1729 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1730 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1731 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1732 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    CAT_FREE, SLS_UNCHANGED},   //JM GREEK (Reserved - do not use)
/* 1733 */  { addItemToBuffer,             CHR_qoppa,                   "",                                            STD_qoppa,                                     CAT_NONE, SLS_UNCHANGED},   //JM GREEK
/* 1734 */  { addItemToBuffer,             CHR_digamma,                 "",                                            STD_digamma,                                   CAT_NONE, SLS_UNCHANGED},   //JM GREEK
/* 1735 */  { addItemToBuffer,             CHR_sampi,                   "",                                            STD_sampi,                                     CAT_NONE, SLS_UNCHANGED},   //JM GREEK
/* 1736 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_case,                                      CAT_NONE, SLS_UNCHANGED},   //JM CASE
/* 1737 */  { fnBASE_Hash,                 NOPARAM,                     "##" STD_RIGHT_ARROW "INT",                    "##" STD_RIGHT_ARROW "INT",                    CAT_NONE, SLS_UNCHANGED},   //JM ##
/* 1738 */  { itemToBeCoded,               NOPARAM,                     "1738",                                        "1738",                                        CAT_FREE, SLS_UNCHANGED},

/* 1739 */  { fnJM,                        3,                           "op_a",                                        "a",                                           CAT_FNCT, SLS_ENABLED  },   //JM Operator a
/* 1740 */  { fnJM,                        4,                           "op_a" STD_SUP_2,                              "a" STD_SUP_2,                                 CAT_FNCT, SLS_ENABLED  },   //JM Operator a.a
/* 1741 */  { fnJM,                        5,                           "op_j",                                        "j",                                           CAT_FNCT, SLS_ENABLED  },   //JM Operator j
/* 1742 */  { fnSetSetJM,                  JC_BASE_HOME,                "_HOME",                                       "HOME",                                        CAT_FNCT, SLS_UNCHANGED},   //JM eRPN
/* 1743 */  { itemToBeCoded,               NOPARAM,                     "1743",                                        "1743",                                        CAT_NONE, SLS_UNCHANGED},   // JM SCREEN SHOT
/* 1744 */  { fnSetSetJM,                  JC_BASE_AHOME,               "_" STD_alpha "HOME",                          STD_alpha "HOME",                              CAT_FNCT, SLS_UNCHANGED},   //JM eRPN
/* 1745 */  { fnSetSetJM,                  JC_H_SUM,                    "H-SUMMARY",                                   "SUMRY",                                       CAT_NONE, SLS_UNCHANGED},   //JMHOME
/* 1746 */  { fnSetSetJM,                  JC_H_MIR,                    "H-MIRROR",                                    "REPLCA",                                      CAT_NONE, SLS_UNCHANGED},   //JMHOME
/* 1747 */  { fnSetSetJM,                  JC_H_FIX,                    "H-FIXED",                                     "FIXED",                                       CAT_NONE, SLS_UNCHANGED},   //JMHOME
/* 1748 */  { itemToBeCoded,               NOPARAM,                     "1748",                                        "1748",                                        CAT_FREE, SLS_UNCHANGED},
/* 1749 */  { itemToBeCoded,               NOPARAM,                     "1749",                                        "1749",                                        CAT_FREE, SLS_UNCHANGED},
/* 1750 */  { fnJM,                        7,                           "Y" STD_SPACE_3_PER_EM STD_RIGHT_ARROW STD_SPACE_3_PER_EM STD_DELTA, "Y" STD_SPACE_3_PER_EM STD_RIGHT_ARROW STD_SPACE_3_PER_EM STD_DELTA, CAT_FNCT, SLS_ENABLED  },   //JM EE
/* 1751 */  { fnJM,                        6,                           STD_DELTA STD_SPACE_3_PER_EM STD_RIGHT_ARROW STD_SPACE_3_PER_EM "Y", STD_DELTA STD_SPACE_3_PER_EM STD_RIGHT_ARROW STD_SPACE_3_PER_EM "Y", CAT_FNCT, SLS_ENABLED  },   //JM EE
/* 1752 */  { fnJM,                        9,                           "AtoSYM",                                      STD_RIGHT_ARROW STD_SPACE_3_PER_EM "012",      CAT_FNCT, SLS_ENABLED  },   //JM EE
/* 1753 */  { fnJM,                        8,                           "SYMtoA",                                      STD_RIGHT_ARROW STD_SPACE_3_PER_EM "abc",      CAT_FNCT, SLS_ENABLED  },   //JM EE
/* 1754 */  { itemToBeCoded,               NOPARAM,                     "ELEC.ENG",                                    "ELEC",                                        CAT_MENU, SLS_UNCHANGED},   //JM EE
/* 1755 */  { fnJM,                        10,                          "e^" STD_THETA "j",                            "e^" STD_THETA "j",                            CAT_FNCT, SLS_ENABLED  },   //JM EE
/* 1756 */  { fnJM,                        11,                          "STO" STD_SPACE_3_PER_EM "3Z",                 "STO" STD_SPACE_3_PER_EM "3Z",                 CAT_FNCT, SLS_ENABLED  },   //JM EE
/* 1757 */  { fnJM,                        12,                          "RCL" STD_SPACE_3_PER_EM "3Z",                 "RCL" STD_SPACE_3_PER_EM "3Z",                 CAT_FNCT, SLS_ENABLED  },   //JM EE
/* 1758 */  { fnJM,                        13,                          "STO" STD_SPACE_3_PER_EM "3V",                 "STO" STD_SPACE_3_PER_EM "3V",                 CAT_FNCT, SLS_ENABLED  },   //JM EE
/* 1759 */  { fnJM,                        14,                          "RCL" STD_SPACE_3_PER_EM "3V",                 "RCL" STD_SPACE_3_PER_EM "3V",                 CAT_FNCT, SLS_ENABLED  },   //JM EE
/* 1760 */  { fnJM,                        15,                          "STO" STD_SPACE_3_PER_EM "3I",                 "STO" STD_SPACE_3_PER_EM "3I",                 CAT_FNCT, SLS_ENABLED  },   //JM EE
/* 1761 */  { fnJM,                        16,                          "RCL" STD_SPACE_3_PER_EM "3I",                 "RCL" STD_SPACE_3_PER_EM "3I",                 CAT_FNCT, SLS_ENABLED  },   //JM EE
/* 1762 */  { fnJM,                        17,                          "3V" STD_DIVIDE "3I",                          "V" STD_DIVIDE "I",                            CAT_FNCT, SLS_ENABLED  },   //JM EE
/* 1763 */  { fnJM,                        18,                          "3I" STD_CROSS "3Z",                           "I" STD_CROSS "Z",                             CAT_FNCT, SLS_ENABLED  },   //JM EE
/* 1764 */  { fnJM,                        19,                          "3V" STD_DIVIDE "3Z",                          "V" STD_DIVIDE "Z",                            CAT_FNCT, SLS_ENABLED  },   //JM EE
/* 1765 */  { fnJM,                        20,                          "X" STD_SPACE_3_PER_EM STD_RIGHT_ARROW STD_SPACE_3_PER_EM "BAL", "X" STD_SPACE_3_PER_EM STD_RIGHT_ARROW STD_SPACE_3_PER_EM "BAL", CAT_FNCT, SLS_ENABLED  },   //JM EE
/* 1766 */  { fnKeyCC,                     KEY_COMPLEX,                 "COMPLEX",                                     "COMPLEX",                                     CAT_FNCT, SLS_UNCHANGED},   //JM Change CC to COMPLEX
/* 1767 */  { itemToBeCoded,               NOPARAM,                     "1767",                                        "1767",                                        CAT_FREE, SLS_UNCHANGED},
/* 1768 */  { fnJMup,                      NOPARAM,                     "CONV UP",                                     STD_RIGHT_ARROW STD_RIGHT_ARROW "LI",          CAT_FNCT, SLS_ENABLED  },   //JM TYPE CONVERT
/* 1769 */  { fnJMdown,                    NOPARAM,                     "CONV DN",                                     "SI" STD_LEFT_ARROW STD_LEFT_ARROW,            CAT_FNCT, SLS_ENABLED  },   //JM TYPE CONVERT
/* 1770 */  { fnSetSetJM,                  JC_SH_3T,                    "SH.3T",                                       "SH.3T",                                       CAT_NONE, SLS_UNCHANGED},
/* 1771 */  { fnGraph,                     3,                           "DEMO",                                        "DEMO",                                        CAT_FNCT, SLS_ENABLED  },

/* 1772 */  { itemToBeCoded,               NOPARAM,                     "KEYS",                                        "KEYS",                                        CAT_MENU, SLS_UNCHANGED},

/* 1773 */  { fnJMUSERmode,                256+0,                       "",                                            "K_00U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1774 */  { fnJMUSERmode_f,              256+0,                       "",                                            "Kf00U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1775 */  { fnJMUSERmode_g,              256+0,                       "",                                            "Kg00U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1776 */  { fnJMUSERmode,                256+1,                       "",                                            "K_01U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1777 */  { fnJMUSERmode_f,              256+1,                       "",                                            "Kf01U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1778 */  { fnJMUSERmode_g,              256+1,                       "",                                            "Kg01U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1779 */  { fnJMUSERmode,                256+2,                       "",                                            "K_02U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1780 */  { fnJMUSERmode_f,              256+2,                       "",                                            "Kf02U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1781 */  { fnJMUSERmode_g,              256+2,                       "",                                            "Kg02U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1782 */  { fnJMUSERmode,                256+3,                       "",                                            "K_03U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1783 */  { fnJMUSERmode_f,              256+3,                       "",                                            "Kf03U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1784 */  { fnJMUSERmode_g,              256+3,                       "",                                            "Kg03U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1785 */  { fnJMUSERmode,                256+4,                       "",                                            "K_04U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1786 */  { fnJMUSERmode_f,              256+4,                       "",                                            "Kf04U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1787 */  { fnJMUSERmode_g,              256+4,                       "",                                            "Kg04U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1788 */  { fnJMUSERmode,                256+5,                       "",                                            "K_05U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1789 */  { fnJMUSERmode_f,              256+5,                       "",                                            "Kf05U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1790 */  { fnJMUSERmode_g,              256+5,                       "",                                            "Kg05U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1791 */  { fnJMUSERmode,                256+6,                       "",                                            "K_06U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1792 */  { fnJMUSERmode_f,              256+6,                       "",                                            "Kf06U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1793 */  { fnJMUSERmode_g,              256+6,                       "",                                            "Kg06U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1794 */  { fnJMUSERmode,                256+7,                       "",                                            "K_07U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1795 */  { fnJMUSERmode_f,              256+7,                       "",                                            "Kf07U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1796 */  { fnJMUSERmode_g,              256+7,                       "",                                            "Kg07U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1797 */  { fnJMUSERmode,                256+8,                       "",                                            "K_08U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1798 */  { fnJMUSERmode_f,              256+8,                       "",                                            "Kf08U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1799 */  { fnJMUSERmode_g,              256+8,                       "",                                            "Kg08U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1800 */  { fnJMUSERmode,                256+9,                       "",                                            "K_09U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1801 */  { fnJMUSERmode_f,              256+9,                       "",                                            "Kf09U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1802 */  { fnJMUSERmode_g,              256+9,                       "",                                            "Kg09U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1803 */  { fnJMUSERmode,                256+10,                      "",                                            "K_10U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1804 */  { fnJMUSERmode_f,              256+10,                      "",                                            "Kf10U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1805 */  { fnJMUSERmode_g,              256+10,                      "",                                            "Kg10U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1806 */  { fnJMUSERmode,                256+11,                      "",                                            "K_11U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1807 */  { fnJMUSERmode_f,              256+11,                      "",                                            "Kf11U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1808 */  { fnJMUSERmode_g,              256+11,                      "",                                            "Kg11U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1809 */  { fnJMUSERmode,                256+12,                      "",                                            "K_12U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1810 */  { fnJMUSERmode_f,              256+12,                      "",                                            "Kf12U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1811 */  { fnJMUSERmode_g,              256+12,                      "",                                            "Kg12U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1812 */  { fnJMUSERmode,                256+13,                      "",                                            "K_13U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1813 */  { fnJMUSERmode_f,              256+13,                      "",                                            "Kf13U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1814 */  { fnJMUSERmode_g,              256+13,                      "",                                            "Kg13U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1815 */  { fnJMUSERmode,                256+14,                      "",                                            "K_14U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1816 */  { fnJMUSERmode_f,              256+14,                      "",                                            "Kf14U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1817 */  { fnJMUSERmode_g,              256+14,                      "",                                            "Kg14U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1818 */  { fnJMUSERmode,                256+15,                      "",                                            "K_15U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1819 */  { fnJMUSERmode_f,              256+15,                      "",                                            "Kf15U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1820 */  { fnJMUSERmode_g,              256+15,                      "",                                            "Kg15U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1821 */  { fnJMUSERmode,                256+16,                      "",                                            "K_16U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1822 */  { fnJMUSERmode_f,              256+16,                      "",                                            "Kf16U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1823 */  { fnJMUSERmode_g,              256+16,                      "",                                            "Kg16U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1824 */  { fnJMUSERmode,                256+17,                      "",                                            "K_17U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1825 */  { fnJMUSERmode_f,              256+17,                      "",                                            "Kf17U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1826 */  { fnJMUSERmode_g,              256+17,                      "",                                            "Kg17U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1827 */  { fnJMUSERmode,                256+18,                      "",                                            "K_18U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1828 */  { fnJMUSERmode_f,              256+18,                      "",                                            "Kf18U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1829 */  { fnJMUSERmode_g,              256+18,                      "",                                            "Kg18U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1830 */  { fnJMUSERmode,                256+19,                      "",                                            "K_19U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1831 */  { fnJMUSERmode_f,              256+19,                      "",                                            "Kf19U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1832 */  { fnJMUSERmode_g,              256+19,                      "",                                            "Kg19U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1833 */  { fnJMUSERmode,                256+20,                      "",                                            "K_20U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1834 */  { fnJMUSERmode_f,              256+20,                      "",                                            "Kf20U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1835 */  { fnJMUSERmode_g,              256+20,                      "",                                            "Kg20U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1836 */  { fnJMUSERmode,                256+21,                      "",                                            "K_21U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1837 */  { fnJMUSERmode_f,              256+21,                      "",                                            "Kf21U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1838 */  { fnJMUSERmode_g,              256+21,                      "",                                            "Kg21U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1839 */  { fnJMUSERmode,                256+22,                      "",                                            "K_22U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1840 */  { fnJMUSERmode_f,              256+22,                      "",                                            "Kf22U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1841 */  { fnJMUSERmode_g,              256+22,                      "",                                            "Kg22U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1842 */  { fnJMUSERmode,                256+23,                      "",                                            "K_23U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1843 */  { fnJMUSERmode_f,              256+23,                      "",                                            "Kf23U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1844 */  { fnJMUSERmode_g,              256+23,                      "",                                            "Kg23U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1845 */  { fnJMUSERmode,                256+24,                      "",                                            "K_24U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1846 */  { fnJMUSERmode_f,              256+24,                      "",                                            "Kf24U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1847 */  { fnJMUSERmode_g,              256+24,                      "",                                            "Kg24U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1848 */  { fnJMUSERmode,                256+25,                      "",                                            "K_25U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1849 */  { fnJMUSERmode_f,              256+25,                      "",                                            "Kf25U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1850 */  { fnJMUSERmode_g,              256+25,                      "",                                            "Kg25U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1851 */  { fnJMUSERmode,                256+26,                      "",                                            "K_26U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1852 */  { fnJMUSERmode_f,              256+26,                      "",                                            "Kf26U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1853 */  { fnJMUSERmode_g,              256+26,                      "",                                            "Kg26U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1854 */  { fnJMUSERmode,                256+27,                      "",                                            "K_27U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1855 */  { fnJMUSERmode_f,              256+27,                      "",                                            "Kf27U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1856 */  { fnJMUSERmode_g,              256+27,                      "",                                            "Kg27U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1857 */  { fnJMUSERmode,                256+28,                      "",                                            "K_28U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1858 */  { fnJMUSERmode_f,              256+28,                      "",                                            "Kf28U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1859 */  { fnJMUSERmode_g,              256+28,                      "",                                            "Kg28U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1860 */  { fnJMUSERmode,                256+29,                      "",                                            "K_29U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1861 */  { fnJMUSERmode_f,              256+29,                      "",                                            "Kf29U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1862 */  { fnJMUSERmode_g,              256+29,                      "",                                            "Kg29U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1863 */  { fnJMUSERmode,                256+30,                      "",                                            "K_30U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1864 */  { fnJMUSERmode_f,              256+30,                      "",                                            "Kf30U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1865 */  { fnJMUSERmode_g,              256+30,                      "",                                            "Kg30U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1866 */  { fnJMUSERmode,                256+31,                      "",                                            "K_31U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1867 */  { fnJMUSERmode_f,              256+31,                      "",                                            "Kf31U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1868 */  { fnJMUSERmode_g,              256+31,                      "",                                            "Kg31U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1869 */  { fnJMUSERmode,                256+32,                      "",                                            "K_32U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1870 */  { fnJMUSERmode_f,              256+32,                      "",                                            "Kf32U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1871 */  { fnJMUSERmode_g,              256+32,                      "",                                            "Kg32U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1872 */  { fnJMUSERmode,                256+33,                      "",                                            "K_33U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1873 */  { fnJMUSERmode_f,              256+33,                      "",                                            "Kf33U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1874 */  { fnJMUSERmode_g,              256+33,                      "",                                            "Kg33U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1875 */  { fnJMUSERmode,                256+34,                      "",                                            "K_34U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1876 */  { fnJMUSERmode_f,              256+34,                      "",                                            "Kf34U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1877 */  { fnJMUSERmode_g,              256+34,                      "",                                            "Kg34U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1878 */  { fnJMUSERmode,                256+35,                      "",                                            "K_35U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1879 */  { fnJMUSERmode_f,              256+35,                      "",                                            "Kf35U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1880 */  { fnJMUSERmode_g,              256+35,                      "",                                            "Kg35U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1881 */  { fnJMUSERmode,                256+36,                      "",                                            "K_36U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1882 */  { fnJMUSERmode_f,              256+36,                      "",                                            "Kf36U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)
/* 1883 */  { fnJMUSERmode_g,              256+36,                      "",                                            "Kg36U",                                       CAT_NONE, SLS_UNCHANGED},   //JM User mode (Will remove later - reserved)

/* 1884 */  { fnJM,                        255,                         "f/g",                                         "f/g",                                         CAT_NONE, SLS_UNCHANGED},   //JM Shift replacement
/* 1885 */  { fnUserJM,                    USER_DEFAULTS,               "U" STD_SIGMA " CC",                           "U" STD_SIGMA " CC",                           CAT_NONE, SLS_UNCHANGED},
/* 1886 */  { fnUserJM,                    USER_COMPLEX,                "U" STD_UP_ARROW " CC",                        "U" STD_UP_ARROW " CC",                        CAT_NONE, SLS_UNCHANGED},
/* 1887 */  { fnUserJM,                    USER_SHIFTS,                 "U" " SHFT",                                   "U" " SHFT",                                   CAT_NONE, SLS_UNCHANGED},   //JM USER
/* 1888 */  { fnUserJM,                    USER_RESET,                  "U" " RSET",                                   "U" " RSET",                                   CAT_NONE, SLS_UNCHANGED},
/* 1889 */  { fnSigmaAssign,               KEY_USERMODE,                STD_SIGMA "+USR",                              STD_SIGMA "+USR",                              CAT_NONE, SLS_UNCHANGED},
/* 1890 */  { itemToBeCoded,               NOPARAM,                     "",                                            "Inl. Tst",                                    CAT_NONE, SLS_UNCHANGED},   //INLINE_TEST
/* 1891 */  { fnSigmaAssign,               KEY_CC,                      STD_SIGMA "+CC",                               STD_SIGMA "+CC",                               CAT_NONE, SLS_UNCHANGED},
/* 1892 */  { itemToBeCoded,               NOPARAM,                     "",                                            "Test",                                        CAT_NONE, SLS_UNCHANGED},   //INLINE_TEST
/* 1893 */  { fnSigmaAssign,               -MNU_MyMenu,                 STD_SIGMA "+MyM",                              STD_SIGMA "+MyM",                              CAT_NONE, SLS_UNCHANGED},
/* 1894 */  { itemToBeCoded,               NOPARAM,                     "",                                            "Get BS",                                      CAT_NONE, SLS_ENABLED  },   //INLINE_TEST
/* 1895 */  { fnSigmaAssign,               ITM_SIGMAPLUS,               STD_SIGMA "+",                                 STD_SIGMA "+",                                 CAT_NONE, SLS_UNCHANGED},   //JM USER
/* 1896 */  { itemToBeCoded,               NOPARAM,                     "",                                            "Set BS",                                      CAT_NONE, SLS_ENABLED  },   //INLINE_TEST
/* 1897 */  { fnSigmaAssign,               ITM_PR,                      STD_SIGMA "+PGM",                              STD_SIGMA "+PGM",                              CAT_NONE, SLS_UNCHANGED},   //JM USER
/* 1898 */  { fnRound,                     NOPARAM,                     "ROUND",                                       "RND",                                         CAT_NONE, SLS_ENABLED  },
/* 1899 */  { fnSigmaAssign,               ITM_AIM,                     STD_SIGMA "+" STD_alpha,                       STD_SIGMA "+" STD_alpha,                       CAT_NONE, SLS_UNCHANGED},
/* 1900 */  { fnGetSigmaAssignToX,         NOPARAM,                     STD_SIGMA "+ toX",                             STD_SIGMA "+ toX",                             CAT_NONE, SLS_UNCHANGED},
/* 1901 */  { fnUserJM,                    JM_ASSIGN,                   "ASSIGN",                                      "ASSIGN",                                      CAT_NONE, SLS_UNCHANGED},
/* 1902 */  { fnUserJM,                    JM_SEEK_FN,                  "FN" STD_DOT "ASN",                            "FN" STD_DOT "ASN",                            CAT_FNCT, SLS_UNCHANGED},
/* 1903 */  { itemToBeCoded,               NOPARAM,                     STD_SIGMA "+ ASN",                             STD_SIGMA "+ ASN",                             CAT_NONE, SLS_UNCHANGED},
/* 1904 */  { fnJM_GetXToNORMmode,         NOPARAM,                     "X to" STD_SIGMA "+",                          "X to" STD_SIGMA "+",                          CAT_NONE, SLS_UNCHANGED},   //JM USER NORMAL
/* 1905 */  { fnInDefault,                 ID_43S,                      "i" STD_SPACE_3_PER_EM "LI/RL",                "i" STD_SPACE_3_PER_EM "LI/RL",                CAT_FNCT, SLS_UNCHANGED},   //JM INPUT DEFAULT
/* 1906 */  { fnSetSetJM,                  JC_FG_LINE,                  "fg" STD_SPACE_3_PER_EM "LINE",                "fg" STD_SPACE_3_PER_EM "LINE",                CAT_FNCT, SLS_UNCHANGED},   //JM UNDERLINING
/* 1907 */  { fnInDefault,                 ID_DP,                       "i" STD_SPACE_3_PER_EM "REAL",                 "i" STD_SPACE_3_PER_EM "REAL",                 CAT_FNCT, SLS_UNCHANGED},   //JM INPUT DEFAULT
/* 1908 */  { fnShowJM,                    JM_INP_DFLT,                 "i" STD_SPACE_3_PER_EM "Dflt?",                "i" STD_SPACE_3_PER_EM "Dflt?",                CAT_NONE, SLS_UNCHANGED},   //JM INPUT DEFAULT
/* 1909 */  { fnSetSetJM,                  JC_FG_DOTS,                  "fg" STD_SPACE_3_PER_EM "DOTS",                "fg" STD_SPACE_3_PER_EM "DOTS",                CAT_FNCT, SLS_UNCHANGED},   //JM UNDERLINING
/* 1910 */  { fnInDefault,                 ID_CPXDP,                    "i" STD_SPACE_3_PER_EM "CPX",                  "i" STD_SPACE_3_PER_EM "CPX",                  CAT_FNCT, SLS_UNCHANGED},   //JM INPUT DEFAULT
/* 1911 */  { fnSetSetJM,                  JC_G_DOUBLETAP,              "G" STD_SPACE_3_PER_EM "2TAP",                 "G" STD_SPACE_3_PER_EM "2TAP",                 CAT_FNCT, SLS_UNCHANGED},   //JM KEY TAP DOUBLE SETTING
/* 1912 */  { fnInDefault,                 ID_SI,                       "i" STD_SPACE_3_PER_EM "SI",                   "i" STD_SPACE_3_PER_EM "SI",                   CAT_FNCT, SLS_UNCHANGED},   //JM INPUT DEFAULT
/* 1913 */  { fnInDefault,                 ID_LI,                       "i" STD_SPACE_3_PER_EM "LI",                   "i" STD_SPACE_3_PER_EM "LI",                   CAT_FNCT, SLS_UNCHANGED},   //JM INPUT DEFAULT
/* 1914 */  { fnGraph,                     1,                           "PLTGRF",                                      "PLTGRF",                                      CAT_FNCT, SLS_UNCHANGED},   //JM GRAPHING
/* 1915 */  { itemToBeCoded,               NOPARAM,                     "A..Z",                                        "A..Z",                                        CAT_MENU, SLS_UNCHANGED},   // Ex 34
/* 1916 */  { itemToBeCoded,               NOPARAM,                     "a..z",                                        "a..z",                                        CAT_MENU, SLS_UNCHANGED},   // Ex 875 Small latin letters
/* 1917 */  { itemToBeCoded,               NOPARAM,                     "GRAPH",                                       "GRAPH",                                       CAT_MENU, SLS_UNCHANGED},   //JM GRAPH
/* 1918 */  { fnJM,                        21,                          "GRF.X1",                                      "Xmin",                                        CAT_NONE, SLS_UNCHANGED},   //JM GRAPH
/* 1919 */  { fnJM,                        22,                          "GRF.X2",                                      "Xmax",                                        CAT_NONE, SLS_UNCHANGED},   //JM GRAPH
/* 1920 */  { fnJM,                        23,                          "GRF.Y1",                                      "Ymin",                                        CAT_NONE, SLS_UNCHANGED},   //JM GRAPH
/* 1921 */  { fnJM,                        24,                          "GRF.Y2",                                      "Ymax",                                        CAT_NONE, SLS_UNCHANGED},   //JM GRAPH
/* 1922 */  { fnJM,                        25,                          "GRF.DX",                                      "TICK DX",                                     CAT_NONE, SLS_UNCHANGED},   //JM GRAPH
/* 1923 */  { fnJM,                        26,                          "GRF.DY",                                      "TICK DY",                                     CAT_NONE, SLS_UNCHANGED},   //JM GRAPH
/* 1924 */  { fnJM,                        27,                          "GRF.HLP",                                     "GRF.HLP",                                     CAT_NONE, SLS_UNCHANGED},   //JM GRAPH
/* 1925 */  { fnJM_2SI,                    NOPARAM,                     STD_RIGHT_ARROW "LI" STD_LEFT_RIGHT_ARROWS "SI", STD_RIGHT_ARROW "LI" STD_LEFT_RIGHT_ARROWS "SI", CAT_FNCT, SLS_ENABLED  },   //JM SHORTINT
/* 1926 */  { fnJM,                        30,                          ".ms",                                         ".ms",                                         CAT_NONE, SLS_UNCHANGED},   //JM DMS HMS
/* 1927 */  { fnJM,                        34,                          STD_DOT "p",                                   STD_DOT "p",                                   CAT_NONE, SLS_ENABLED  },   //JM PRE UNIT
/* 1928 */  { fnJM,                        35,                          STD_DOT "n",                                   STD_DOT "n",                                   CAT_NONE, SLS_ENABLED  },   //JM PRE UNIT
/* 1929 */  { fnJM,                        36,                          STD_DOT STD_mu,                                STD_DOT STD_mu,                                CAT_NONE, SLS_ENABLED  },   //JM PRE UNIT
/* 1930 */  { fnJM,                        37,                          STD_DOT "m",                                   STD_DOT "m",                                   CAT_NONE, SLS_ENABLED  },   //JM PRE UNIT
/* 1931 */  { fnJM,                        38,                          STD_DOT "k",                                   STD_DOT "k",                                   CAT_NONE, SLS_ENABLED  },   //JM PRE UNIT
/* 1932 */  { fnJM,                        39,                          STD_DOT "M",                                   STD_DOT "M",                                   CAT_NONE, SLS_ENABLED  },   //JM PRE UNIT
/* 1933 */  { fnUserJM,                    USER_ALPHA,                  "U" STD_SIGMA STD_DOT STD_alpha,               "U" STD_SIGMA STD_DOT STD_alpha,               CAT_NONE, SLS_UNCHANGED},
/* 1934 */  { fnUserJM,                    USER_GSHFT,                  "U" STD_SIGMA STD_DOT "G" STD_DOT "SH",        "U" STD_SIGMA STD_DOT "G" STD_DOT "SH",        CAT_NONE, SLS_UNCHANGED},
/* 1935 */  { fnKeyDotDjm,                 NOPARAM,                     "Dot.d",                                       "Dot.d",                                       CAT_NONE, SLS_ENABLED  },
/* 1936 */  { fnGraph,                     11,                          "DEMO1",                                       "DEMO1",                                       CAT_FNCT, SLS_ENABLED  },
/* 1937 */  { fnGraph,                     12,                          "DEMO2",                                       "DEMO2",                                       CAT_FNCT, SLS_ENABLED  },
/* 1938 */  { fnGraph,                     13,                          "DEMO3",                                       "DEMO3",                                       CAT_FNCT, SLS_ENABLED  },
/* 1939 */  { fnGraph,                     14,                          "DEMO4",                                       "DEMO4",                                       CAT_FNCT, SLS_ENABLED  },
/* 1940 */  { fnGraph,                     15,                          "DEMO5",                                       "DEMO5",                                       CAT_FNCT, SLS_ENABLED  },
/* 1941 */  { fnGraph,                     16,                          "DEMO6",                                       "DEMO6",                                       CAT_FNCT, SLS_ENABLED  },
/* 1942 */  { itemToBeCoded,               NOPARAM,                     "ST_GRF",                                      "ST_GRF",                                      CAT_MENU, SLS_ENABLED  },
/* 1943 */  { fnSetSetJM,                  JC_VECT,                     "VECTOR",                                      "VECTOR",                                      CAT_FNCT, SLS_UNCHANGED},   //JM VECTOR MODE
/* 1944 */  { fnSetSetJM,                  NOPARAM,                     "TOFILE",                                      "TOFILE",                                      CAT_FNCT, SLS_UNCHANGED},   //JM CSV FILE MODE
/* 1945 */  { fnP_All_Regs,                0,                           STD_PRINTER "ALLr",                            STD_PRINTER "ALLr",                            CAT_FNCT, SLS_UNCHANGED},
/* 1946 */  { fnJM,                        40,                          STD_DOT "f",                                   STD_DOT "f",                                   CAT_NONE, SLS_ENABLED  },   //JM PRE UNIT
/* 1947 */  { fnJM,                        41,                          STD_DOT "G",                                   STD_DOT "G",                                   CAT_NONE, SLS_ENABLED  },   //JM PRE UNIT
/* 1948 */  { fnJM,                        42,                          STD_DOT "T",                                   STD_DOT "T",                                   CAT_NONE, SLS_ENABLED  },   //JM PRE UNIT
/* 1949 */  { fnUserJM,                    USER_CC,                     "U" STD_SIGMA STD_DOT "CC",                    "U" STD_SIGMA STD_DOT "CC",                    CAT_NONE, SLS_UNCHANGED},
/* 1950 */  { fnUserJM,                    USER_MYM,                    "U" STD_SIGMA STD_DOT "MYM",                   "U" STD_SIGMA STD_DOT "MYM",                   CAT_NONE, SLS_UNCHANGED},
/* 1951 */  { fnUserJM,                    USER_PRGM,                   "U" STD_SIGMA STD_DOT "PRGM",                  "U" STD_SIGMA STD_DOT "PRGM",                  CAT_NONE, SLS_UNCHANGED},
/* 1952 */  { fnUserJM,                    USER_USER,                   "U" STD_SIGMA STD_DOT "USER",                  "U" STD_SIGMA STD_DOT "USER",                  CAT_NONE, SLS_UNCHANGED},
/* 1953 */  { fnUserJM,                    USER_SIGMAPLUS,              "U" STD_SIGMA STD_DOT STD_SIGMA "+",           "U" STD_SIGMA STD_DOT STD_SIGMA "+",           CAT_NONE, SLS_UNCHANGED},
/* 1954 */  { fnJM,                        46,                          "LISTXY",                                      "LISTXY",                                      CAT_FNCT, SLS_UNCHANGED},

/* 1955 */  { itemToBeCoded,               NOPARAM,                     "",                                            "Last item",                                   CAT_NONE, SLS_UNCHANGED}       //JM
};
