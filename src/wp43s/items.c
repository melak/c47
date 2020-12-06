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
    moreInfoOnError("Function to be coded", "for that data type(s)!", NULL, NULL);
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



#if !defined(GENERATE_CATALOGS) && !defined(INLINE_TEST)    //vv dr
  void fnSetInlineTest      (uint16_t drConfig)                {};
  void fnGetInlineTestBsToX (uint16_t unusedParamButMandatory) {};
  void fnSetInlineTestXToBs (uint16_t unusedParamButMandatory) {};
  void fnSysFreeMem         (uint16_t unusedParamButMandatory) {};
#endif                                                      //^^


#if !defined(TESTSUITE_BUILD) && !defined(GENERATE_CATALOGS)
void reallyRunFunction(int16_t func, uint16_t param) {
  if(indexOfItems[func].undoStatus == US_ENABLED) {
    saveForUndo();
  }
  else if(indexOfItems[func].undoStatus == US_CANCEL) {
    thereIsSomethingToUndo = false;
  }

  indexOfItems[func].func(param);

  if(indexOfItems[func].stackLiftStatus == SLS_DISABLED) {
    clearSystemFlag(FLAG_ASLIFT);
  }
  else if(indexOfItems[func].stackLiftStatus == SLS_ENABLED) {
    setSystemFlag(FLAG_ASLIFT);
  }
  if((func == ITM_ENTER) && (eRPN)) {      //JM NEWERPN
    setSystemFlag(FLAG_ASLIFT);            //JM NEWERPN OVERRIDE SLS, AS ERPN ENTER ALWAYS HAS SLS SET
    //printf("Forced ERPN items.c\n");
  }                                        //JM NEWERPN

  #ifdef PC_BUILD
    refreshLcd(NULL);
  #endif
}



/********************************************//**
 * \brief Runs a function
 *
 * \param[in] fn int16_t Index in the indexOfItems area of the function to run
 * \return void
 ***********************************************/
void runFunction(int16_t func) {
  funcOK = true;
  #ifdef RECORDLOG
  capture_sequence("runFunction:", func);        //JMEXEC
  #endif

  if(func >= LAST_ITEM) {
    #ifdef PC_BUILD
      sprintf(errorMessage, "item (%" PRId16 ") must be below LAST_ITEM", func);
      moreInfoOnError("In function runFunction:", errorMessage, NULL, NULL);
    #endif
  }

  if(calcMode != CM_ASM_OVER_TAM) {
    tamMode = indexOfItems[func].param;
  }

  if(calcMode != CM_TAM && calcMode != CM_ASM_OVER_TAM && TM_VALUE <= tamMode && tamMode <= TM_CMP) {
    tamFunction = func;
    strcpy(tamBuffer, indexOfItems[func].itemSoftmenuName);
    tamNumberMin = indexOfItems[func].tamMin;
    tamNumberMax = indexOfItems[func].tamMax;

    if(func == ITM_CNST) {
      tamNumberMax = NUMBER_OF_CONSTANTS_39 + NUMBER_OF_CONSTANTS_51 + NUMBER_OF_CONSTANTS_1071 + NUMBER_OF_CONSTANTS_34;
    }

    calcModeTam();
    return;
  }

  reallyRunFunction(func, indexOfItems[func].param);

  if(!funcOK) {
    displayCalcErrorMessage(ERROR_ITEM_TO_BE_CODED, ERR_REGISTER_LINE, REGISTER_X);
    #ifdef PC_BUILD
      sprintf(errorMessage, "%" PRId16 " = %s", func, indexOfItems[func].itemCatalogName);
      moreInfoOnError("In function runFunction:", "Item not implemented", errorMessage, "to be coded");
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
void fnSincpi                   (uint16_t unusedParamButMandatory) {}
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
void fnSetDateFormat            (uint16_t unusedParamButMandatory) {}
void fnComplexUnit              (uint16_t unusedParamButMandatory) {}
void fnComplexMode              (uint16_t unusedParamButMandatory) {}
void fnComplexResult            (uint16_t unusedParamButMandatory) {}
void fnConjugate                (uint16_t unusedParamButMandatory) {}
void fnAngularMode              (uint16_t unusedParamButMandatory) {}
void fnDenMode                  (uint16_t unusedParamButMandatory) {}
void fnDenMax                   (uint16_t unusedParamButMandatory) {}
void fnExp                      (uint16_t unusedParamButMandatory) {}
void fnExpM1                    (uint16_t unusedParamButMandatory) {}
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
void fnLogXY                    (uint16_t unusedParamButMandatory) {}
void fnLnP1                     (uint16_t unusedParamButMandatory) {}
void fnLnGamma                  (uint16_t unusedParamButMandatory) {}
void fnLnBeta                   (uint16_t unusedParamButMandatory) {}
void fnBeta                     (uint16_t unusedParamButMandatory) {}
void fnGamma                    (uint16_t unusedParamButMandatory) {}
void fnIDiv                     (uint16_t unusedParamButMandatory) {}
void fnIDivR                    (uint16_t unusedParamButMandatory) {}
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
void fnSwapX                    (uint16_t unusedParamButMandatory) {}
void fnSwapY                    (uint16_t unusedParamButMandatory) {}
void fnSwapZ                    (uint16_t unusedParamButMandatory) {}
void fnSwapT                    (uint16_t unusedParamButMandatory) {}
void fnSwapXY                   (uint16_t unusedParamButMandatory) {}
void fnShuffle                  (uint16_t unusedParamButMandatory) {}
void fnWho                      (uint16_t unusedParamButMandatory) {}
void fnGetSignificantDigits     (uint16_t unusedParamButMandatory) {}
void fnSdl                      (uint16_t unusedParamButMandatory) {}
void fnSdr                      (uint16_t unusedParamButMandatory) {}
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
void fnCvtMmhgPa                (uint16_t unusedParamButMandatory) {}
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
void fnCvtLbfftNm               (uint16_t unusedParamButMandatory) {}
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
void fnWeightedSampleStdDev     (uint16_t unusedParamButMandatory) {}
void fnWeightedPopulationStdDev (uint16_t unusedParamButMandatory) {}
void fnWeightedStandardError    (uint16_t unusedParamButMandatory) {}
void fnSampleStdDev             (uint16_t unusedParamButMandatory) {}
void fnPopulationStdDev         (uint16_t unusedParamButMandatory) {}
void fnStandardError            (uint16_t unusedParamButMandatory) {}
void fnGeometricSampleStdDev    (uint16_t unusedParamButMandatory) {}
void fnGeometricPopulationStdDev(uint16_t unusedParamButMandatory) {}
void fnGeometricStandardError   (uint16_t unusedParamButMandatory) {}
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
//void fnArg                      (uint16_t unusedParamButMandatory) {}
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
void fnXthRoot                  (uint16_t unusedParamButMandatory) {}
void fnGetSystemFlag            (uint16_t unusedParamButMandatory) {}
void fnFractionType             (uint16_t unusedParamButMandatory) {}
void fnAlphaLeng                (uint16_t unusedParamButMandatory) {}
void fnAlphaSR                  (uint16_t unusedParamButMandatory) {}
void fnAlphaSL                  (uint16_t unusedParamButMandatory) {}
void fnAlphaRR                  (uint16_t unusedParamButMandatory) {}
void fnAlphaRL                  (uint16_t unusedParamButMandatory) {}
void fnAlphaPos                 (uint16_t unusedParamButMandatory) {}
void fnXToAlpha                 (uint16_t unusedParamButMandatory) {}
void fnAlphaToX                 (uint16_t unusedParamButMandatory) {}
void fnTicks                    (uint16_t unusedParamButMandatory) {}
void fnSave                     (uint16_t unusedParamButMandatory) {}
void fnLoad                     (uint16_t unusedParamButMandatory) {}
void fnUndo                     (uint16_t unusedParamButMandatory) {}
void fnXmax                     (uint16_t unusedParamButMandatory) {}
void fnXmin                     (uint16_t unusedParamButMandatory) {}
void fnFib                      (uint16_t unusedParamButMandatory) {}
void fnGd                       (uint16_t unusedParamButMandatory) {}
void fnInvGd                    (uint16_t unusedParamButMandatory) {}
void fnJM                       (uint16_t unusedParamButMandatory) {}           //vv JM
void fnSetSetJM                 (uint16_t unusedParamButMandatory) {}
void fnDisplayFormatSigFig      (uint16_t unusedParamButMandatory) {}
void fnDisplayFormatUnit        (uint16_t unusedParamButMandatory) {}
void fnShowJM                   (uint16_t unusedParamButMandatory) {}
void fnUserJM                   (uint16_t unusedParamButMandatory) {}
void fnSigmaAssign              (uint16_t unusedParamButMandatory) {}
void fnGetSigmaAssignToX        (uint16_t unusedParamButMandatory) {}
void fnJM_GetXToNORMmode        (uint16_t unusedParamButMandatory) {}
void fnInDefault                (uint16_t unusedParamButMandatory) {}
void fnJMUSERmode               (uint16_t unusedParamButMandatory) {}
void fnJMUSERmode_f             (uint16_t unusedParamButMandatory) {}
void fnJMUSERmode_g             (uint16_t unusedParamButMandatory) {}
void fnGraph                    (uint16_t unusedParamButMandatory) {}
void fnJM_2SI                   (uint16_t unusedParamButMandatory) {}
void fnTo_ms                    (uint16_t unusedParamButMandatory) {}
void fnShow_SCROLL              (uint16_t unusedParamButMandatory) {}
void fnP_All_Regs               (uint16_t unusedParamButMandatory) {}
void fnToPolar2                 (uint16_t unusedParamButMandatory) {}
void fnToRect2                  (uint16_t unusedParamButMandatory) {}
void fnArg_all                  (uint16_t unusedParamButMandatory) {}
void fnMultiplySI               (uint16_t unusedParamButMandatory) {}
void fn_cnst_op_j               (uint16_t unusedParamButMandatory) {}
void fn_cnst_op_aa              (uint16_t unusedParamButMandatory) {}
void fn_cnst_op_a               (uint16_t unusedParamButMandatory) {}
void fn_cnst_0_cpx              (uint16_t unusedParamButMandatory) {}
void fn_cnst_1_cpx              (uint16_t unusedParamButMandatory) {}
void fnListXY                   (uint16_t unusedParamButMandatory) {}
void fnXEQMENU                  (uint16_t unusedParamButMandatory) {}
void flagBrowser_old            (uint16_t unusedParamButMandatory) {}
void fnRound2                   (uint16_t unusedParamButMandatory) {}
void fnRoundi2                  (uint16_t unusedParamButMandatory) {}
void fnSetInlineTest            (uint16_t unusedParamButMandatory) {}           //vv dr
void fnSetInlineTestXToBs       (uint16_t unusedParamButMandatory) {}
void fnGetInlineTestBsToX       (uint16_t unusedParamButMandatory) {}
void fnSysFreeMem               (uint16_t unusedParamButMandatory) {}           //^^
void fneRPN                     (uint16_t unusedParamButMandatory) {}
void fnT_ARROW                  (uint16_t unusedParamButMandatory) {}
void fnXEQNEW                   (uint16_t unusedParamButMandatory) {}
void fnXEQMSAVE                 (uint16_t unusedParamButMandatory) {}
void fnXEQMLOAD                 (uint16_t unusedParamButMandatory) {}
void fnXEQMEDIT                 (uint16_t unusedParamButMandatory) {}
void fnXEQMXXEQ                 (uint16_t unusedParamButMandatory) {}
void fnCase                     (uint16_t unusedParamButMandatory) {}
void fnAngularModeJM            (uint16_t unusedParamButMandatory) {}
void fnChangeBaseJM             (uint16_t unusedParamButMandatory) {}
void fnChangeBaseMNU            (uint16_t unusedParamButMandatory) {}
void fnLongInt                  (uint16_t unusedParamButMandatory) {}
void fnByteShortcutsS           (uint16_t unusedParamButMandatory) {}  //JM POC BASE2
void fnByteShortcutsU           (uint16_t unusedParamButMandatory) {}  //JM POC BASE2
void fnByte                     (uint16_t unusedParamButMandatory) {}  //JM POC BASE2
void fnClAIM                    (uint16_t unusedParamButMandatory) {}  //JM POC BASE2
void fnShoiXRepeats             (uint16_t unusedParamButMandatory) {}  //JM SHOIDISP
void fnDumpMenus                (uint16_t unusedParamButMandatory) {}  //JM
void fnCFGsettings              (uint16_t unusedParamButMandatory) {}
void fnPlotLS                   (uint16_t unusedParamButMandatory) {}
void fnPlot                     (uint16_t unusedParamButMandatory) {}
void fnPline                    (uint16_t unusedParamButMandatory) {}
void fnPcros                    (uint16_t unusedParamButMandatory) {}
void fnPbox                     (uint16_t unusedParamButMandatory) {}
void fnPintg                    (uint16_t unusedParamButMandatory) {}
void fnPdiff                    (uint16_t unusedParamButMandatory) {}
void fnPrms                     (uint16_t unusedParamButMandatory) {}
void fnPvect                    (uint16_t unusedParamButMandatory) {}
void fnPNvect                   (uint16_t unusedParamButMandatory) {}
void fnScale                    (uint16_t unusedParamButMandatory) {}
void fnPx                       (uint16_t unusedParamButMandatory) {}
void fnPy                       (uint16_t unusedParamButMandatory) {}
void fnPshade                   (uint16_t unusedParamButMandatory) {}
void fnCla                      (uint16_t unusedParamButMandatory) {}
void fnCln                      (uint16_t unusedParamButMandatory) {}


  
                                                                                //JM ^^

#endif

const item_t indexOfItems[] = {

//            function                     parameter                    item in catalog                                item in softmenu                               TAM min  TAM max  CATALOG   stackLift      UNDO status

/*    0 */  { itemToBeCoded,               NOPARAM,                     "",                                            "0000",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED}, // ITM_NULL

// Items from 1 to 127 are 1 byte OP codes
/*    1 */  { itemToBeCoded,               NOPARAM,                     "LBL",                                         "LBL",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*    2 */  { itemToBeCoded,               NOPARAM,                     "GTO",                                         "GTO",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*    3 */  { fnRecall,                    TM_STORCL    /*# JM #*/,     "XEQ",                                         "XEQ",                                         0,      99,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },//TEMPORARY PLACEHOLDER FOR XEQ
/*    4 */  { itemToBeCoded,               NOPARAM,                     "RTN",                                         "RTN",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*    5 */  { itemToBeCoded,               NOPARAM,                     "ISE",                                         "ISE",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*    6 */  { itemToBeCoded,               NOPARAM,                     "ISG",                                         "ISG",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*    7 */  { itemToBeCoded,               NOPARAM,                     "ISZ",                                         "ISZ",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*    8 */  { itemToBeCoded,               NOPARAM,                     "DSE",                                         "DSE",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*    9 */  { itemToBeCoded,               NOPARAM,                     "DSL",                                         "DSL",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   10 */  { itemToBeCoded,               NOPARAM,                     "DSZ",                                         "DSZ",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   11 */  { itemToBeCoded,               NOPARAM,                     "x= ?",                                        "x= ?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   12 */  { itemToBeCoded,               NOPARAM,                     "x" STD_NOT_EQUAL " ?",                        "x" STD_NOT_EQUAL " ?",                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   13 */  { itemToBeCoded,               NOPARAM,                     "x=+0?",                                       "x=+0?",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   14 */  { itemToBeCoded,               NOPARAM,                     "x=-0?",                                       "x=-0?",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   15 */  { itemToBeCoded,               NOPARAM,                     "x" STD_ALMOST_EQUAL " ?",                     "x" STD_ALMOST_EQUAL " ?",                     0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   16 */  { itemToBeCoded,               NOPARAM,                     "x< ?",                                        "x< ?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   17 */  { itemToBeCoded,               NOPARAM,                     "x" STD_LESS_EQUAL " ?",                       "x" STD_LESS_EQUAL " ?",                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   18 */  { itemToBeCoded,               NOPARAM,                     "x" STD_GREATER_EQUAL " ?",                    "x" STD_GREATER_EQUAL " ?",                    0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   19 */  { itemToBeCoded,               NOPARAM,                     "x> ?",                                        "x> ?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   20 */  { fnIsFlagClear,               TM_FLAGR,                    "FC?",                                         "FC?",                                         0,      99,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   21 */  { fnIsFlagSet,                 TM_FLAGR,                    "FS?",                                         "FS?",                                         0,      99,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   22 */  { itemToBeCoded,               NOPARAM,                     "EVEN?",                                       "EVEN?",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   23 */  { itemToBeCoded,               NOPARAM,                     "ODD?",                                        "ODD?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   24 */  { itemToBeCoded,               NOPARAM,                     "FP?",                                         "FP?",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   25 */  { itemToBeCoded,               NOPARAM,                     "INT?",                                        "INT?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   26 */  { itemToBeCoded,               NOPARAM,                     "CPX?",                                        "CPX?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   27 */  { itemToBeCoded,               NOPARAM,                     "MATR?",                                       "MATR?",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   28 */  { itemToBeCoded,               NOPARAM,                     "NaN?",                                        "NaN?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   29 */  { itemToBeCoded,               NOPARAM,                     "REAL?",                                       "REAL?",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   30 */  { itemToBeCoded,               NOPARAM,                     "SPEC?",                                       "SPEC?",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   31 */  { itemToBeCoded,               NOPARAM,                     "STRI?",                                       "STRI?",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   32 */  { itemToBeCoded,               NOPARAM,                     STD_PLUS_MINUS STD_INFINITY "?",               STD_PLUS_MINUS STD_INFINITY "?",               0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   33 */  { fnIsPrime,                   NOPARAM,                     "PRIME?",                                      "PRIME?",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   34 */  { itemToBeCoded,               NOPARAM,                     "TOP?",                                        "TOP?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   35 */  { fnKeyEnter,                  NOPARAM,                     "ENTER" STD_UP_ARROW,                          "ENTER" STD_UP_ARROW,                          0,       0,       CAT_FNCT, SLS_DISABLED , US_ENABLED  },
/*   36 */  { fnSwapXY,                    NOPARAM,                     "x" STD_LEFT_RIGHT_ARROWS "y",                 "x" STD_LEFT_RIGHT_ARROWS "y",                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   37 */  { fnDrop,                      NOPARAM,                     "DROP",                                        "DROP" STD_DOWN_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   38 */  { fnDropY,                     NOPARAM,                     "DROPy",                                       "DROPy",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   39 */  { fnRollUp,                    NOPARAM,                     "R" STD_UP_ARROW,                              "R" STD_UP_ARROW,                              0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   40 */  { fnRollDown,                  NOPARAM,                     "R" STD_DOWN_ARROW,                            "R" STD_DOWN_ARROW,                            0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   41 */  { fnClX,                       NOPARAM,                     "CLX",                                         "CLX",                                         0,       0,       CAT_FNCT, SLS_DISABLED , US_ENABLED  },
/*   42 */  { fnFillStack,                 NOPARAM,                     "FILL",                                        "FILL",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   43 */  { itemToBeCoded,               NOPARAM,                     "INPUT",                                       "INPUT",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   44 */  { fnStore,                     TM_STORCL,                   "STO",                                         "STO",                                         0,      99,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   45 */  { fnStoreAdd,                  NOPARAM,                     "STO+",                                        "STO+",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   46 */  { fnStoreSub,                  NOPARAM,                     "STO-",                                        "STO-",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   47 */  { fnStoreMult,                 NOPARAM,                     "STO" STD_CROSS,                               "STO" STD_CROSS,                               0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   48 */  { fnStoreDiv,                  NOPARAM,                     "STO/",                                        "STO/",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   49 */  { fnStoreMax,                  NOPARAM,                     "STO" STD_UP_ARROW,                            "Max",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   50 */  { fnStoreMin,                  NOPARAM,                     "STO" STD_DOWN_ARROW,                          "Min",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*   51 */  { fnRecall,                    TM_STORCL,                   "RCL",                                         "RCL",                                         0,      99,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   52 */  { fnRecallAdd,                 NOPARAM,                     "RCL+",                                        "RCL+",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   53 */  { fnRecallSub,                 NOPARAM,                     "RCL-",                                        "RCL-",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   54 */  { fnRecallMult,                NOPARAM,                     "RCL" STD_CROSS,                               "RCL" STD_CROSS,                               0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   55 */  { fnRecallDiv,                 NOPARAM,                     "RCL/",                                        "RCL/",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   56 */  { fnRecallMax,                 NOPARAM,                     "RCL" STD_UP_ARROW,                            "Max",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   57 */  { fnRecallMin,                 NOPARAM,                     "RCL" STD_DOWN_ARROW,                          "Min",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   58 */  { fnSquare,                    NOPARAM,                     "x" STD_SUP_2,                                 "x" STD_SUP_2,                                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   59 */  { fnCube,                      NOPARAM,                     "x" STD_SUP_3,                                 "x" STD_SUP_3,                                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   60 */  { fnPower,                     NOPARAM,                     "y" STD_SUP_x,                                 "y" STD_SUP_x,                                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   61 */  { fnSquareRoot,                NOPARAM,                     STD_SQUARE_ROOT STD_x_UNDER_ROOT,              STD_SQUARE_ROOT STD_x_UNDER_ROOT,              0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   62 */  { fnCubeRoot,                  NOPARAM,                     STD_CUBE_ROOT STD_x_UNDER_ROOT,                STD_CUBE_ROOT STD_x_UNDER_ROOT,                0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   63 */  { fnXthRoot,                   NOPARAM,                     STD_xTH_ROOT STD_y_UNDER_ROOT,                 STD_xTH_ROOT STD_y_UNDER_ROOT,                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   64 */  { fn2Pow,                      NOPARAM,                     "2" STD_SUP_x,                                 "2" STD_SUP_x,                                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   65 */  { fnExp,                       NOPARAM,                     "e" STD_SUP_x,                                 "e" STD_SUP_x,                                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   66 */  { fnExpM1,                     NOPARAM,                     "e" STD_SUP_x "-1",                            "e" STD_SUP_x "-1",                            0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   67 */  { fn10Pow,                     NOPARAM,                     "10" STD_SUP_x,                                "10" STD_SUP_x,                                0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   68 */  { fnLog2,                      NOPARAM,                     "LOG" STD_SUB_2,                               "lb x",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   69 */  { fnLn,                        NOPARAM     /*# JM #*/,      "LN",                                          "LN",                                          0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM3 change ln to LN
/*   70 */  { fnLnP1,                      NOPARAM,                     "LN(1+x)",                                     "ln 1+x",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   71 */  { fnLog10,                     NOPARAM     /*# JM #*/,      "LOG" STD_SUB_1 STD_SUB_0,                     "LOG",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM Change lg to LOG
/*   72 */  { fnLogXY,                     NOPARAM,                     "LOG" STD_SUB_x "y",                           "log" STD_SUB_x "y",                           0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   73 */  { fnInvert,                    NOPARAM,                     "1/x",                                         "1/x",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   74 */  { fnCos,                       NOPARAM     /*# JM #*/,      "COS",                                         "COS",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM
/*   75 */  { fnCosh,                      NOPARAM,                     "cosh",                                        "cosh",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   76 */  { fnSin,                       NOPARAM     /*# JM #*/,      "SIN",                                         "SIN",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM3
/*   77 */  { fnSinc,                      NOPARAM,                     "sinc",                                        "sinc",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   78 */  { fnSinh,                      NOPARAM,                     "sinh",                                        "sinh",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   79 */  { fnTan,                       NOPARAM     /*# JM #*/,      "TAN",                                         "TAN",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM3
/*   80 */  { fnTanh,                      NOPARAM,                     "tanh",                                        "tanh",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   81 */  { fnArccos,                    NOPARAM     /*# JM #*/,      "ARCCOS",                                      "ACOS",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM
/*   82 */  { fnArccosh,                   NOPARAM,                     "arcosh",                                      "arcosh",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   83 */  { fnArcsin,                    NOPARAM     /*# JM #*/,      "ARCSIN",                                      "ASIN",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM
/*   84 */  { fnArcsinh,                   NOPARAM,                     "arsinh",                                      "arsinh",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   85 */  { fnArctan,                    NOPARAM     /*# JM #*/,      "ARCTAN",                                      "ATAN",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM
/*   86 */  { fnArctanh,                   NOPARAM,                     "artanh",                                      "artanh",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   87 */  { fnCeil,                      NOPARAM,                     "CEIL",                                        "CEIL",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   88 */  { fnFloor,                     NOPARAM,                     "FLOOR",                                       "FLOOR",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   89 */  { fnGcd,                       NOPARAM,                     "GCD",                                         "GCD",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   90 */  { fnLcm,                       NOPARAM,                     "LCM",                                         "LCM",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   91 */  { fnDec,                       TM_REGISTER,                 "DECR",                                        "DECR",                                        0,      99,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   92 */  { fnInc,                       TM_REGISTER,                 "INCR",                                        "INCR",                                        0,      99,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   93 */  { fnIp,                        NOPARAM,                     "IP",                                          "IP",                                          0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   94 */  { fnFp,                        NOPARAM,                     "FP",                                          "FP",                                          0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   95 */  { fnAdd,                       ITM_ADD,                     "+",                                           "+",                                           0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   96 */  { fnSubtract,                  ITM_SUB,                     "-",                                           "-",                                           0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   97 */  { fnChangeSign,                ITM_CHS     /*# JM #*/,      "CHS",                                         "CHS",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM Change +/- to CHS
/*   98 */  { fnMultiply,                  ITM_MULT,                    STD_CROSS,                                     STD_CROSS,                                     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*   99 */  { fnDivide,                    ITM_DIV     /*# JM #*/,      STD_DIVIDE,                                    STD_DIVIDE,                                    0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  100 */  { fnIDiv,                      NOPARAM,                     "IDIV",                                        "IDIV",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  101 */  { fnIDivR,                     NOPARAM,                     "IDIVR",                                       "IDIVR",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  102 */  { fnMod,                       NOPARAM,                     "MOD",                                         "MOD",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  103 */  { fnMax,                       NOPARAM,                     "max",                                         "max",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  104 */  { fnMin,                       NOPARAM,                     "min",                                         "min",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  105 */  { fnMagnitude,                 NOPARAM,                     "|x|",                                         "|x|",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  106 */  { fnNeighb,                    NOPARAM,                     "NEIGHB",                                      "NEIGHB",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  107 */  { fnNextPrime,                 NOPARAM,                     "NEXTP",                                       "NEXTP",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  108 */  { fnFactorial,                 NOPARAM,                     "x!",                                          "x!",                                          0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  109 */  { fnPi,                        NOPARAM,                     STD_pi,                                        STD_pi,                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },
/*  110 */  { fnClearFlag,                 TM_FLAGW,                    "CF",                                          "CF",                                          0,      99,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*  111 */  { fnSetFlag,                   TM_FLAGW,                    "SF",                                          "SF",                                          0,      99,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*  112 */  { fnFlipFlag,                  TM_FLAGW,                    "FF",                                          "FF",                                          0,      99,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  113 */  { fnSincpi,                    NOPARAM,                     "sinc" STD_pi,                                 "sinc" STD_pi,                                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  114 */  { itemToBeCoded,               NOPARAM,                     "0114",                                        "0114",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  115 */  { itemToBeCoded,               NOPARAM,                     "0115",                                        "0115",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  116 */  { itemToBeCoded,               NOPARAM,                     "0116",                                        "0116",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  117 */  { itemToBeCoded,               NOPARAM,                     "0117",                                        "0117",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  118 */  { itemToBeCoded,               NOPARAM,                     "0118",                                        "0118",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  119 */  { itemToBeCoded,               NOPARAM,                     "0119",                                        "0119",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  120 */  { itemToBeCoded,               NOPARAM,                     "0120",                                        "0120",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  121 */  { itemToBeCoded,               NOPARAM,                     "0121",                                        "0121",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  122 */  { itemToBeCoded,               NOPARAM,                     "0122",                                        "0122",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  123 */  { itemToBeCoded,               NOPARAM,                     "0123",                                        "0123",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  124 */  { itemToBeCoded,               NOPARAM,                     "0124",                                        "0124",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  125 */  { itemToBeCoded,               NOPARAM,                     "0125",                                        "0125",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  126 */  { itemToBeCoded,               NOPARAM,                     "0126",                                        "0126",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  127 */  { itemToBeCoded,               NOPARAM,                     "0127",                                        "0127",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},


// Items from 128 to ... are 2 byte OP codes
// Constants
/*  128 */  { fnConstant,                  0,                           "a",                                           "a",                                           0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  129 */  { fnConstant,                  1,                           "a" STD_SUB_0,                                 "a" STD_SUB_0,                                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  130 */  { fnConstant,                  2,                           "a" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "a" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  131 */  { fnConstant,                  3,                           "a" STD_SUB_EARTH,                             "a" STD_SUB_EARTH,                             0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  132 */  { fnConstant,                  4,                           "c",                                           "c",                                           0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  133 */  { fnConstant,                  5,                           "c" STD_SUB_1,                                 "c" STD_SUB_1,                                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  134 */  { fnConstant,                  6,                           "c" STD_SUB_2,                                 "c" STD_SUB_2,                                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  135 */  { fnConstant,                  7,                           "e",                                           "e",                                           0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  136 */  { fnConstant,                  8,                           "e" STD_SUB_E,                                 "e" STD_SUB_E,                                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  137 */  { fnConstant,                  9,                           "F",                                           "F",                                           0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  138 */  { fnConstant,                  10,                          "F" STD_SUB_alpha,                             "F" STD_SUB_alpha,                             0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  139 */  { fnConstant,                  11,                          "F" STD_SUB_delta,                             "F" STD_SUB_delta,                             0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  140 */  { fnConstant,                  12,                          "G",                                           "G",                                           0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  141 */  { fnConstant,                  13,                          "G" STD_SUB_0,                                 "G" STD_SUB_0,                                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  142 */  { fnConstant,                  14,                          "G" STD_SUB_C,                                 "G" STD_SUB_C,                                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  143 */  { fnConstant,                  15,                          "g" STD_SUB_e,                                 "g" STD_SUB_e,                                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  144 */  { fnConstant,                  16,                          "GM" STD_SUB_EARTH,                            "GM" STD_SUB_EARTH,                            0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  145 */  { fnConstant,                  17,                          "g" STD_SUB_EARTH,                             "g" STD_SUB_EARTH,                             0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  146 */  { fnConstant,                  18,                          STD_PLANCK,                                    STD_PLANCK,                                    0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  147 */  { fnConstant,                  19,                          STD_PLANCK_2PI,                                STD_PLANCK_2PI,                                0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  148 */  { fnConstant,                  20,                          "k",                                           "k",                                           0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  149 */  { fnConstant,                  21,                          "K" STD_SUB_J,                                 "K" STD_SUB_J,                                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  150 */  { fnConstant,                  22,                          "l" STD_SUB_P STD_SUB_L,                       "l" STD_SUB_P STD_SUB_L,                       0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  151 */  { fnConstant,                  23,                          "m" STD_SUB_e,                                 "m" STD_SUB_e,                                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  152 */  { fnConstant,                  24,                          "M" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "M" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  153 */  { fnConstant,                  25,                          "m" STD_SUB_n,                                 "m" STD_SUB_n,                                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  154 */  { fnConstant,                  26,                          "m" STD_SUB_n "/m" STD_SUB_p,                  "m" STD_SUB_n "/m" STD_SUB_p,                  0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  155 */  { fnConstant,                  27,                          "m" STD_SUB_p,                                 "m" STD_SUB_p,                                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  156 */  { fnConstant,                  28,                          "m" STD_SUB_P STD_SUB_L,                       "m" STD_SUB_P STD_SUB_L,                       0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  157 */  { fnConstant,                  29,                          "m" STD_SUB_p "/m" STD_SUB_e,                  "m" STD_SUB_p "/m" STD_SUB_e,                  0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  158 */  { fnConstant,                  30,                          "m" STD_SUB_u,                                 "m" STD_SUB_u,                                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  159 */  { fnConstant,                  31,                          "m" STD_SUB_u "c" STD_SUP_2,                   "m" STD_SUB_u "c" STD_SUP_2,                   0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  160 */  { fnConstant,                  32,                          "m" STD_SUB_mu,                                "m" STD_SUB_mu,                                0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  161 */  { fnConstant,                  33,                          "M" STD_SUB_SUN,                               "M" STD_SUB_SUN,                               0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  162 */  { fnConstant,                  34,                          "M" STD_SUB_EARTH,                             "M" STD_SUB_EARTH,                             0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  163 */  { fnConstant,                  35,                          "N" STD_SUB_A,                                 "N" STD_SUB_A,                                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  164 */  { fnConstant,                  36,                          "NaN",                                         "NaN",                                         0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  165 */  { fnConstant,                  37,                          "p" STD_SUB_0,                                 "p" STD_SUB_0,                                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  166 */  { fnConstant,                  38,                          "R",                                           "R",                                           0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  167 */  { fnConstant,                  39,                          "r" STD_SUB_e,                                 "r" STD_SUB_e,                                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  168 */  { fnConstant,                  40,                          "R" STD_SUB_K,                                 "R" STD_SUB_K,                                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  169 */  { fnConstant,                  41,                          "R" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "R" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  170 */  { fnConstant,                  42,                          "R" STD_SUB_INFINITY,                          "R" STD_SUB_INFINITY,                          0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  171 */  { fnConstant,                  43,                          "R" STD_SUB_SUN,                               "R" STD_SUB_SUN,                               0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  172 */  { fnConstant,                  44,                          "R" STD_SUB_EARTH,                             "R" STD_SUB_EARTH,                             0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  173 */  { fnConstant,                  45,                          "Sa",                                          "Sa",                                          0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  174 */  { fnConstant,                  46,                          "Sb",                                          "Sb",                                          0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  175 */  { fnConstant,                  47,                          "Se" STD_SUP_2,                                "Se" STD_SUP_2,                                0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  176 */  { fnConstant,                  48,                          "Se'" STD_SUP_2,                               "Se'" STD_SUP_2,                               0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  177 */  { fnConstant,                  49,                          "Sf" STD_SUP_MINUS_1,                          "Sf" STD_SUP_MINUS_1,                          0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  178 */  { fnConstant,                  50,                          "T" STD_SUB_0,                                 "T" STD_SUB_0,                                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  179 */  { fnConstant,                  51,                          "T" STD_SUB_p,                                 "T" STD_SUB_P,                                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  180 */  { fnConstant,                  52,                          "t" STD_SUB_P STD_SUB_L,                       "t" STD_SUB_P STD_SUB_L,                       0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  181 */  { fnConstant,                  53,                          "V" STD_SUB_m,                                 "V" STD_SUB_m,                                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  182 */  { fnConstant,                  54,                          "Z" STD_SUB_0,                                 "Z" STD_SUB_0,                                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  183 */  { fnConstant,                  55,                          STD_alpha,                                     STD_alpha,                                     0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  184 */  { fnConstant,                  56,                          STD_gamma,                                     STD_gamma,                                     0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  185 */  { fnConstant,                  57,                          STD_gamma STD_SUB_E STD_SUB_M,                 STD_gamma STD_SUB_E STD_SUB_M,                 0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  186 */  { fnConstant,                  58,                          STD_gamma STD_SUB_p,                           STD_gamma STD_SUB_p,                           0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  187 */  { fnConstant,                  59,                          STD_DELTA STD_nu STD_SUB_C STD_SUB_s,          STD_DELTA STD_nu STD_SUB_C STD_SUB_s,          0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  188 */  { fnConstant,                  60,                          STD_epsilon STD_SUB_0,                         STD_epsilon STD_SUB_0,                         0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  189 */  { fnConstant,                  61,                          STD_lambda STD_SUB_C,                          STD_lambda STD_SUB_C,                          0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  190 */  { fnConstant,                  62,                          STD_lambda STD_SUB_C STD_SUB_n,                STD_lambda STD_SUB_C STD_SUB_n,                0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  191 */  { fnConstant,                  63,                          STD_lambda STD_SUB_C STD_SUB_p,                STD_lambda STD_SUB_C STD_SUB_p,                0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  192 */  { fnConstant,                  64,                          STD_mu STD_SUB_0,                              STD_mu STD_SUB_0,                              0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  193 */  { fnConstant,                  65,                          STD_mu STD_SUB_B,                              STD_mu STD_SUB_B,                              0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  194 */  { fnConstant,                  66,                          STD_mu STD_SUB_e,                              STD_mu STD_SUB_e,                              0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  195 */  { fnConstant,                  67,                          STD_mu STD_SUB_e "/" STD_mu STD_SUB_B,         STD_mu STD_SUB_e "/" STD_mu STD_SUB_B,         0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  196 */  { fnConstant,                  68,                          STD_mu STD_SUB_n,                              STD_mu STD_SUB_n,                              0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  197 */  { fnConstant,                  69,                          STD_mu STD_SUB_p,                              STD_mu STD_SUB_p,                              0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  198 */  { fnConstant,                  70,                          STD_mu STD_SUB_u,                              STD_mu STD_SUB_u,                              0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  199 */  { fnConstant,                  71,                          STD_mu STD_SUB_mu,                             STD_mu STD_SUB_mu,                             0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  200 */  { fnConstant,                  72,                          STD_sigma STD_SUB_B,                           STD_sigma STD_SUB_B,                           0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  201 */  { fnConstant,                  73,                          STD_PHI,                                       STD_PHI,                                       0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  202 */  { fnConstant,                  74,                          STD_PHI STD_SUB_0,                             STD_PHI STD_SUB_0,                             0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  203 */  { fnConstant,                  75,                          STD_omega,                                     STD_omega,                                     0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  204 */  { fnConstant,                  76,                          "-" STD_INFINITY,                              "-" STD_INFINITY,                              0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  205 */  { fnConstant,                  77,                          STD_INFINITY,                                  STD_INFINITY,                                  0,       0,       CAT_CNST, SLS_ENABLED  , US_ENABLED  },
/*  206 */  { itemToBeCoded,               NOPARAM,                     "#",                                           "#",                                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  207 */  { fnConstant,                  TM_VALUE,                    "CNST",                                        "CNST",                                        0,      99,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  208 */  { itemToBeCoded,               NOPARAM,                     "0208",                                        "0208",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  209 */  { itemToBeCoded,               NOPARAM,                     "0209",                                        "0209",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  210 */  { itemToBeCoded,               NOPARAM,                     "0210",                                        "0210",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  211 */  { itemToBeCoded,               NOPARAM,                     "0211",                                        "0211",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  212 */  { itemToBeCoded,               NOPARAM,                     "0212",                                        "0212",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  213 */  { itemToBeCoded,               NOPARAM,                     "0213",                                        "0213",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  214 */  { itemToBeCoded,               NOPARAM,                     "0214",                                        "0214",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  215 */  { itemToBeCoded,               NOPARAM,                     "0215",                                        "0215",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  216 */  { itemToBeCoded,               NOPARAM,                     "0216",                                        "0216",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  217 */  { itemToBeCoded,               NOPARAM,                     "0217",                                        "0217",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  218 */  { itemToBeCoded,               NOPARAM,                     "0218",                                        "0218",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  219 */  { itemToBeCoded,               NOPARAM,                     "0219",                                        "0219",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},


// Conversions
/*  220 */  { fnCvtCToF,                   NOPARAM,                     STD_DEGREE "C" STD_RIGHT_ARROW STD_DEGREE "F", STD_DEGREE "C" STD_RIGHT_ARROW STD_DEGREE "F",   0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  221 */  { fnCvtFToC,                   NOPARAM,                     STD_DEGREE "F" STD_RIGHT_ARROW STD_DEGREE "C", STD_DEGREE "F" STD_RIGHT_ARROW STD_DEGREE "C",   0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  222 */  { fnCvtDbRatio,                10,                          "dB" STD_RIGHT_ARROW "pr",                     "dB " STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  223 */  { fnCvtDbRatio,                10,                          "dB" STD_RIGHT_ARROW "pr",                     "power",                                       0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  224 */  { fnCvtDbRatio,                10,                          "dB" STD_RIGHT_ARROW "pr",                     "ratio",                                       0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  225 */  { fnCvtDbRatio,                20,                          "dB" STD_RIGHT_ARROW "fr",                     "dB " STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  226 */  { fnCvtDbRatio,                20,                          "dB" STD_RIGHT_ARROW "fr",                     "field",                                       0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  227 */  { fnCvtDbRatio,                20,                          "dB" STD_RIGHT_ARROW "fr",                     "ratio",                                       0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  228 */  { fnCvtRatioDb,                10,                          "pr" STD_RIGHT_ARROW "dB",                     "power",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  229 */  { fnCvtRatioDb,                10,                          "pr" STD_RIGHT_ARROW "dB",                     "ratio",                                       0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  230 */  { fnCvtRatioDb,                10,                          "pr" STD_RIGHT_ARROW "dB",                     STD_RIGHT_ARROW " dB",                         0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  231 */  { fnCvtRatioDb,                20,                          "fr" STD_RIGHT_ARROW "dB",                     "field",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  232 */  { fnCvtRatioDb,                20,                          "fr" STD_RIGHT_ARROW "dB",                     "ratio",                                       0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  233 */  { fnCvtRatioDb,                20,                          "fr" STD_RIGHT_ARROW "dB",                     STD_RIGHT_ARROW " dB",                         0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  234 */  { fnCvtAcreM2,                 multiply,                    "ac" STD_RIGHT_ARROW "m" STD_SUP_2,            "acre",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  235 */  { fnCvtAcreM2,                 multiply,                    "ac" STD_RIGHT_ARROW "m" STD_SUP_2,            STD_RIGHT_ARROW " m" STD_SUP_2,                0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  236 */  { fnCvtAcreM2,                 divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ac",            "m" STD_SUP_2 " " STD_RIGHT_ARROW,             0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  237 */  { fnCvtAcreM2,                 divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ac",            "acre",                                        0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  238 */  { fnCvtAcreusM2,               multiply,                    "ac" STD_US STD_RIGHT_ARROW "m" STD_SUP_2,     "acre" STD_US,                                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  239 */  { fnCvtAcreusM2,               multiply,                    "ac" STD_US STD_RIGHT_ARROW "m" STD_SUP_2,     STD_RIGHT_ARROW " m" STD_SUP_2,                0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  240 */  { fnCvtAcreusM2,               divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ac" STD_US,     "m" STD_SUP_2 " " STD_RIGHT_ARROW,             0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  241 */  { fnCvtAcreusM2,               divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ac" STD_US,     "acre" STD_US,                                 0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  242 */  { fnCvtAtmPa,                  divide,                      "Pa" STD_RIGHT_ARROW "atm",                    "Pa" STD_RIGHT_ARROW "atm",                    0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  243 */  { fnCvtAtmPa,                  multiply,                    "atm" STD_RIGHT_ARROW "Pa",                    "atm" STD_RIGHT_ARROW "Pa",                    0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  244 */  { fnCvtAuM,                    multiply,                    "au" STD_RIGHT_ARROW "m",                      "au" STD_RIGHT_ARROW "m",                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  245 */  { fnCvtAuM,                    divide,                      "m" STD_RIGHT_ARROW "au",                      "m" STD_RIGHT_ARROW "au",                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  246 */  { fnCvtBarPa,                  divide,                      "Pa" STD_RIGHT_ARROW "bar",                    "Pa" STD_RIGHT_ARROW "bar",                    0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  247 */  { fnCvtBarPa,                  multiply,                    "bar" STD_RIGHT_ARROW "Pa",                    "bar" STD_RIGHT_ARROW "Pa",                    0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  248 */  { fnCvtBtuJ,                   multiply,                    "Btu" STD_RIGHT_ARROW "J",                     "Btu" STD_RIGHT_ARROW "J",                     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  249 */  { fnCvtBtuJ,                   divide,                      "J" STD_RIGHT_ARROW "Btu",                     "J" STD_RIGHT_ARROW "Btu",                     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  250 */  { fnCvtCalJ,                   multiply,                    "cal" STD_RIGHT_ARROW "J",                     "cal" STD_RIGHT_ARROW "J",                     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  251 */  { fnCvtCalJ,                   divide,                      "J" STD_RIGHT_ARROW "cal",                     "J" STD_RIGHT_ARROW "cal",                     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  252 */  { fnCvtLbfftNm,                multiply,                    "lbft" STD_RIGHT_SHORT_ARROW "Nm",             "lbf" STD_DOT "ft",                            0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  253 */  { fnCvtLbfftNm,                multiply,                    "lbft" STD_RIGHT_SHORT_ARROW "Nm",             STD_RIGHT_ARROW " Nm",                         0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  254 */  { fnCvtLbfftNm,                divide,                      "Nm" STD_RIGHT_SHORT_ARROW "lbft",             "Nm " STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  255 */  { fnCvtLbfftNm,                divide,                      "Nm" STD_RIGHT_SHORT_ARROW "lbft",             "lbf" STD_DOT "ft",                            0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  256 */  { fnCvtCwtKg,                  multiply,                    "cwt" STD_RIGHT_ARROW "kg",                    "cwt" STD_RIGHT_ARROW "kg",                    0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  257 */  { fnCvtCwtKg,                  divide,                      "kg" STD_RIGHT_ARROW "cwt",                    "kg" STD_RIGHT_ARROW "cwt",                    0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  258 */  { fnCvtFtM,                    multiply,                    "ft." STD_RIGHT_ARROW "m",                     "ft." STD_RIGHT_ARROW "m",                     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  259 */  { fnCvtFtM,                    divide,                      "m" STD_RIGHT_ARROW "ft.",                     "m" STD_RIGHT_ARROW "ft.",                     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  260 */  { fnCvtSfeetM,                 multiply,                    "ft" STD_US STD_RIGHT_ARROW "m",               "survey",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  261 */  { fnCvtSfeetM,                 multiply,                    "ft" STD_US STD_RIGHT_ARROW "m",               "foot" STD_US,                                 0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  262 */  { fnCvtSfeetM,                 multiply,                    "ft" STD_US STD_RIGHT_ARROW "m",               STD_RIGHT_ARROW " m",                          0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  263 */  { fnCvtSfeetM,                 divide,                      "m" STD_RIGHT_ARROW "ft" STD_US,               "m " STD_RIGHT_ARROW,                          0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  264 */  { fnCvtSfeetM,                 divide,                      "m" STD_RIGHT_ARROW "ft" STD_US,               "survey",                                      0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  265 */  { fnCvtSfeetM,                 divide,                      "m" STD_RIGHT_ARROW "ft" STD_US,               "foot" STD_US,                                 0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  266 */  { fnCvtFlozukM3,               multiply,                    "fz" STD_UK STD_RIGHT_ARROW "m" STD_SUP_3,     "floz" STD_UK,                                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  267 */  { fnCvtFlozukM3,               multiply,                    "fz" STD_UK STD_RIGHT_ARROW "m" STD_SUP_3,     STD_RIGHT_ARROW " m" STD_SUP_3,                0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  268 */  { fnCvtFlozukM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "fz" STD_UK,     "m" STD_SUP_3 " " STD_RIGHT_ARROW,             0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  269 */  { fnCvtFlozukM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "fz" STD_UK,     "floz" STD_UK,                                 0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  270 */  { fnCvtFlozusM3,               multiply,                    "fz" STD_US STD_RIGHT_ARROW "m" STD_SUP_3,     "floz" STD_US,                                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  271 */  { fnCvtFlozusM3,               multiply,                    "fz" STD_US STD_RIGHT_ARROW "m" STD_SUP_3,     STD_RIGHT_ARROW " m" STD_SUP_3,                0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  272 */  { fnCvtFlozusM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "fz" STD_US,     "m" STD_SUP_3 " " STD_RIGHT_ARROW,             0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  273 */  { fnCvtFlozusM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "fz" STD_US,     "floz" STD_US,                                 0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  274 */  { fnCvtGalukM3,                multiply,                    "gl" STD_UK STD_RIGHT_ARROW "m" STD_SUP_3,     "gl" STD_UK STD_RIGHT_ARROW "m" STD_SUP_3,     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  275 */  { fnCvtGalukM3,                divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_UK,     "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_UK,     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  276 */  { fnCvtGalusM3,                multiply,                    "gl" STD_US STD_RIGHT_ARROW "m" STD_SUP_3,     "gl" STD_US STD_RIGHT_ARROW "m" STD_SUP_3,     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  277 */  { fnCvtGalusM3,                divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_US,     "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_US,     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  278 */  { fnCvtHpeW,                   multiply,                    "hp" STD_SUB_E STD_RIGHT_ARROW "W",            "hp" STD_SUB_E STD_RIGHT_ARROW "W",            0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  279 */  { fnCvtHpeW,                   divide,                      "W" STD_RIGHT_ARROW "hp" STD_SUB_E,            "W" STD_RIGHT_ARROW "hp" STD_SUB_E,            0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  280 */  { fnCvtHpmW,                   multiply,                    "hp" STD_SUB_M STD_RIGHT_ARROW "W",            "hp" STD_SUB_M STD_RIGHT_ARROW "W",            0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  281 */  { fnCvtHpmW,                   divide,                      "W" STD_RIGHT_ARROW "hp" STD_SUB_M,            "W" STD_RIGHT_ARROW "hp" STD_SUB_M,            0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  282 */  { fnCvtHpukW,                  multiply,                    "hp" STD_UK STD_RIGHT_ARROW "W",               "hp" STD_UK STD_RIGHT_ARROW "W",               0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  283 */  { fnCvtHpukW,                  divide,                      "W" STD_RIGHT_ARROW "hp" STD_UK,               "W" STD_RIGHT_ARROW "hp" STD_UK,               0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  284 */  { fnCvtInhgPa,                 multiply,                    "iHg" STD_RIGHT_ARROW "Pa",                    "in.Hg",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  285 */  { fnCvtInhgPa,                 multiply,                    "iHg" STD_RIGHT_ARROW "Pa",                    STD_RIGHT_ARROW " Pa",                         0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  286 */  { fnCvtInhgPa,                 divide,                      "Pa" STD_RIGHT_ARROW "iHg",                    "Pa" STD_RIGHT_ARROW,                          0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  287 */  { fnCvtInhgPa,                 divide,                      "Pa" STD_RIGHT_ARROW "iHg",                    "in.Hg",                                       0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  288 */  { fnCvtInchM,                  multiply,                    "in." STD_RIGHT_ARROW "m",                     "in." STD_RIGHT_ARROW "m",                     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  289 */  { fnCvtInchM,                  divide,                      "m" STD_RIGHT_ARROW "in.",                     "m" STD_RIGHT_ARROW "in.",                     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  290 */  { fnCvtWhJ,                    multiply,                    "Wh" STD_RIGHT_ARROW "J",                      "Wh" STD_RIGHT_ARROW "J",                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  291 */  { fnCvtWhJ,                    divide,                      "J" STD_RIGHT_ARROW "Wh",                      "J" STD_RIGHT_ARROW "Wh",                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  292 */  { fnCvtLbKg,                   divide,                      "kg" STD_RIGHT_ARROW "lb.",                    "kg" STD_RIGHT_ARROW "lb.",                    0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  293 */  { fnCvtLbKg,                   multiply,                    "lb." STD_RIGHT_ARROW "kg",                    "lb." STD_RIGHT_ARROW "kg",                    0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  294 */  { fnCvtOzKg,                   divide,                      "kg" STD_RIGHT_ARROW "oz",                     "kg" STD_RIGHT_ARROW "oz",                     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  295 */  { fnCvtOzKg,                   multiply,                    "oz" STD_RIGHT_ARROW "kg",                     "oz" STD_RIGHT_ARROW "kg",                     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  296 */  { fnCvtShortcwtKg,             divide,                      "kg" STD_RIGHT_ARROW "scw",                    "kg " STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  297 */  { fnCvtShortcwtKg,             divide,                      "kg" STD_RIGHT_ARROW "scw",                    "sh.cwt",                                      0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  298 */  { fnCvtShortcwtKg,             multiply,                    "scw" STD_RIGHT_ARROW "kg",                    "short",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  299 */  { fnCvtShortcwtKg,             multiply,                    "scw" STD_RIGHT_ARROW "kg",                    "cwt" STD_RIGHT_ARROW "kg",                    0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  300 */  { fnCvtStoneKg,                divide,                      "kg" STD_RIGHT_ARROW "sto",                    "kg " STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  301 */  { fnCvtStoneKg,                divide,                      "kg" STD_RIGHT_ARROW "sto",                    "stone",                                       0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  302 */  { fnCvtStoneKg,                multiply,                    "sto" STD_RIGHT_ARROW "kg",                    "stone",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  303 */  { fnCvtStoneKg,                multiply,                    "sto" STD_RIGHT_ARROW "kg",                    STD_RIGHT_ARROW " kg",                         0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  304 */  { fnCvtShorttonKg,             divide,                      "kg" STD_RIGHT_ARROW "s.t",                    "kg " STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  305 */  { fnCvtShorttonKg,             divide,                      "kg" STD_RIGHT_ARROW "s.t",                    "short",                                       0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  306 */  { fnCvtShorttonKg,             divide,                      "kg" STD_RIGHT_ARROW "s.t",                    "ton",                                         0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  307 */  { fnCvtShorttonKg,             multiply,                    "s.t" STD_RIGHT_ARROW "kg",                    "short",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  308 */  { fnCvtShorttonKg,             multiply,                    "s.t" STD_RIGHT_ARROW "kg",                    "ton",                                         0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  309 */  { fnCvtShorttonKg,             multiply,                    "s.t" STD_RIGHT_ARROW "kg",                    STD_RIGHT_ARROW " kg",                         0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  310 */  { fnCvtTonKg,                  divide,                      "kg" STD_RIGHT_ARROW "ton",                    "kg" STD_RIGHT_ARROW "ton",                    0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  311 */  { fnCvtTonKg,                  divide,                      "kg" STD_RIGHT_ARROW "ton",                    STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  312 */  { fnCvtTonKg,                  divide,                      "kg" STD_RIGHT_ARROW "ton",                    STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  313 */  { fnCvtTonKg,                  multiply,                    "ton" STD_RIGHT_ARROW "kg",                    "ton" STD_RIGHT_ARROW "kg",                    0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  314 */  { fnCvtTonKg,                  multiply,                    "ton" STD_RIGHT_ARROW "kg",                    STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  315 */  { fnCvtTonKg,                  multiply,                    "ton" STD_RIGHT_ARROW "kg",                    STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  316 */  { fnCvtTrozKg,                 divide,                      "kg" STD_RIGHT_ARROW "trz",                    "kg " STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  317 */  { fnCvtTrozKg,                 divide,                      "kg" STD_RIGHT_ARROW "trz",                    "tr.oz",                                       0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  318 */  { fnCvtTrozKg,                 multiply,                    "trz" STD_RIGHT_ARROW "kg",                    "tr.oz",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  319 */  { fnCvtTrozKg,                 multiply,                    "trz" STD_RIGHT_ARROW "kg",                    STD_RIGHT_ARROW " kg",                         0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  320 */  { fnCvtLbfN,                   multiply,                    "lbf" STD_RIGHT_ARROW "N",                     "lbf" STD_RIGHT_ARROW "N",                     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  321 */  { fnCvtLbfN,                   divide,                      "N" STD_RIGHT_ARROW "lbf",                     "N" STD_RIGHT_ARROW "lbf",                     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  322 */  { fnCvtLyM,                    multiply,                    "ly" STD_RIGHT_ARROW "m",                      "ly" STD_RIGHT_ARROW "m",                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  323 */  { fnCvtLyM,                    divide,                      "m" STD_RIGHT_ARROW "ly",                      "m" STD_RIGHT_ARROW "ly",                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  324 */  { fnCvtMmhgPa,                 multiply,                    "mmH" STD_RIGHT_SHORT_ARROW "Pa",              "mm.Hg",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  325 */  { fnCvtMmhgPa,                 multiply,                    "mmH" STD_RIGHT_SHORT_ARROW "Pa",              STD_RIGHT_ARROW " Pa",                         0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  326 */  { fnCvtMmhgPa,                 divide,                      "Pa" STD_RIGHT_SHORT_ARROW "mmH",              "Pa " STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  327 */  { fnCvtMmhgPa,                 divide,                      "Pa" STD_RIGHT_SHORT_ARROW "mmH",              "mm.Hg",                                       0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  328 */  { fnCvtMiM,                    multiply,                    "mi." STD_RIGHT_ARROW "m",                     "mi." STD_RIGHT_ARROW "m",                     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  329 */  { fnCvtMiM,                    divide,                      "m" STD_RIGHT_ARROW "mi.",                     "m" STD_RIGHT_ARROW "mi.",                     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  330 */  { fnCvtNmiM,                   divide,                      "m" STD_RIGHT_ARROW "nmi.",                    "m" STD_RIGHT_ARROW "nmi.",                    0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  331 */  { fnCvtNmiM,                   multiply,                    "nmi." STD_RIGHT_ARROW "m",                    "nmi." STD_RIGHT_ARROW "m",                    0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  332 */  { fnCvtPcM,                    divide,                      "m" STD_RIGHT_ARROW "pc",                      "m" STD_RIGHT_ARROW "pc",                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  333 */  { fnCvtPcM,                    multiply,                    "pc" STD_RIGHT_ARROW "m",                      "pc" STD_RIGHT_ARROW "m",                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  334 */  { fnCvtPointM,                 divide,                      "m" STD_RIGHT_ARROW "pt.",                     "m " STD_RIGHT_ARROW,                          0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  335 */  { fnCvtPointM,                 divide,                      "m" STD_RIGHT_ARROW "pt.",                     "point",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  336 */  { fnCvtPointM,                 divide,                      "m" STD_RIGHT_ARROW "pt.",                     STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  337 */  { fnCvtPointM,                 multiply,                    "pt." STD_RIGHT_ARROW "m",                     "point",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  338 */  { fnCvtPointM,                 multiply,                    "pt." STD_RIGHT_ARROW "m",                     STD_RIGHT_ARROW " m",                          0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  339 */  { fnCvtPointM,                 multiply,                    "pt." STD_RIGHT_ARROW "m",                     STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  340 */  { fnCvtYardM,                  divide,                      "m" STD_RIGHT_ARROW "yd.",                     "m" STD_RIGHT_ARROW "yd.",                     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  341 */  { fnCvtYardM,                  multiply,                    "yd." STD_RIGHT_ARROW "m",                     "yd." STD_RIGHT_ARROW "m",                     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  342 */  { fnCvtPsiPa,                  multiply,                    "psi" STD_RIGHT_ARROW "Pa",                    "psi" STD_RIGHT_ARROW "Pa",                    0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  343 */  { fnCvtPsiPa,                  divide,                      "Pa" STD_RIGHT_ARROW "psi",                    "Pa" STD_RIGHT_ARROW "psi",                    0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  344 */  { fnCvtTorrPa,                 divide,                      "Pa" STD_RIGHT_ARROW "tor",                    "Pa " STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  345 */  { fnCvtTorrPa,                 divide,                      "Pa" STD_RIGHT_ARROW "tor",                    "torr",                                        0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  346 */  { fnCvtTorrPa,                 multiply,                    "tor" STD_RIGHT_ARROW "Pa",                    "torr",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  347 */  { fnCvtTorrPa,                 multiply,                    "tor" STD_RIGHT_ARROW "Pa",                    STD_RIGHT_ARROW " Pa",                         0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  348 */  { fnCvtYearS,                  divide,                      "s" STD_RIGHT_ARROW "year",                    "s" STD_RIGHT_ARROW "year",                    0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  349 */  { fnCvtYearS,                  multiply,                    "year" STD_RIGHT_ARROW "s",                    "year" STD_RIGHT_ARROW "s",                    0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  350 */  { fnCvtCaratKg,                multiply,                    "ct" STD_RIGHT_ARROW "kg",                     "carat",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  351 */  { fnCvtCaratKg,                multiply,                    "ct" STD_RIGHT_ARROW "kg",                     STD_RIGHT_ARROW " kg",                         0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  352 */  { fnCvtCaratKg,                multiply,                    "ct" STD_RIGHT_ARROW "kg",                     STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  353 */  { fnCvtCaratKg,                divide,                      "kg" STD_RIGHT_ARROW "ct",                     "kg " STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  354 */  { fnCvtCaratKg,                divide,                      "kg" STD_RIGHT_ARROW "ct",                     "carat",                                       0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  355 */  { fnCvtCaratKg,                divide,                      "kg" STD_RIGHT_ARROW "ct",                     STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  356 */  { fnCvtQuartM3,                multiply,                    "qt." STD_RIGHT_ARROW "m" STD_SUP_3,           "qt." STD_RIGHT_ARROW "m" STD_SUP_3,           0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  357 */  { fnCvtQuartM3,                divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "qt.",           "m" STD_SUP_3 STD_RIGHT_ARROW "qt.",           0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  358 */  { fnCvtFathomM,                multiply,                    "fm." STD_RIGHT_ARROW "m",                     "fathom",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  359 */  { fnCvtFathomM,                multiply,                    "fm." STD_RIGHT_ARROW "m",                     STD_RIGHT_ARROW " m",                          0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  360 */  { fnCvtFathomM,                multiply,                    "fm." STD_RIGHT_ARROW "m",                     STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  361 */  { fnCvtFathomM,                divide,                      "m" STD_RIGHT_ARROW "fm.",                     "m " STD_RIGHT_ARROW,                          0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  362 */  { fnCvtFathomM,                divide,                      "m" STD_RIGHT_ARROW "fm.",                     "fathom",                                      0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  363 */  { fnCvtFathomM,                divide,                      "m" STD_RIGHT_ARROW "fm.",                     STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  364 */  { fnCvtBarrelM3,               multiply,                    "bbl" STD_RIGHT_ARROW "m" STD_SUP_3,           "barrel",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  365 */  { fnCvtBarrelM3,               multiply,                    "bbl" STD_RIGHT_ARROW "m" STD_SUP_3,           STD_RIGHT_ARROW " m" STD_SUP_3,                0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  366 */  { fnCvtBarrelM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "bbl",           STD_RIGHT_ARROW " m" STD_SUP_3,                0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  367 */  { fnCvtBarrelM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "bbl",           "barrel",                                      0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  368 */  { fnCvtAtmPa,                  multiply,                    "atm" STD_RIGHT_ARROW "Pa",                    STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  369 */  { fnCvtAtmPa,                  divide,                      "Pa" STD_RIGHT_ARROW "atm",                    STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  , US_ENABLED  },
/*  370 */  { fnCvtHectareM2,              multiply,                    "ha" STD_RIGHT_ARROW "m" STD_SUP_2,            "ha" STD_RIGHT_ARROW "m" STD_SUP_2,            0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  371 */  { fnCvtHectareM2,              divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ha",            "m" STD_SUP_2 STD_RIGHT_ARROW "ha",            0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  372 */  { itemToBeCoded,               NOPARAM,                     "0372",                                        "0372",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  373 */  { itemToBeCoded,               NOPARAM,                     "0373",                                        "0373",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  374 */  { itemToBeCoded,               NOPARAM,                     "0374",                                        "0374",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  375 */  { itemToBeCoded,               NOPARAM,                     "0375",                                        "0375",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  376 */  { itemToBeCoded,               NOPARAM,                     "0376",                                        "0376",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  377 */  { itemToBeCoded,               NOPARAM,                     "0377",                                        "0377",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  378 */  { itemToBeCoded,               NOPARAM,                     "0378",                                        "0378",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  379 */  { itemToBeCoded,               NOPARAM,                     "0379",                                        "0379",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  380 */  { itemToBeCoded,               NOPARAM,                     "0380",                                        "0380",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  381 */  { itemToBeCoded,               NOPARAM,                     "0381",                                        "0381",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  382 */  { itemToBeCoded,               NOPARAM,                     "0382",                                        "0382",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  383 */  { itemToBeCoded,               NOPARAM,                     "0383",                                        "0383",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  384 */  { itemToBeCoded,               NOPARAM,                     "0384",                                        "0384",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  385 */  { itemToBeCoded,               NOPARAM,                     "0385",                                        "0385",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},


// Flag, bit, rotation, and logical OPs
/*  386 */  { fnIsFlagClearClear,          TM_FLAGW,                    "FC?C",                                        "FC?C",                                        0,      99,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*  387 */  { fnIsFlagClearSet,            TM_FLAGW,                    "FC?S",                                        "FC?S",                                        0,      99,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*  388 */  { fnIsFlagClearFlip,           TM_FLAGW,                    "FC?F",                                        "FC?F",                                        0,      99,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*  389 */  { fnIsFlagSetClear,            TM_FLAGW,                    "FS?C",                                        "FS?C",                                        0,      99,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*  390 */  { fnIsFlagSetSet,              TM_FLAGW,                    "FS?S",                                        "FS?S",                                        0,      99,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*  391 */  { fnIsFlagSetFlip,             TM_FLAGW,                    "FS?F",                                        "FS?F",                                        0,      99,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/*  392 */  { fnLogicalNot,                NOPARAM,                     "NOT",                                         "NOT",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  393 */  { fnLogicalAnd,                NOPARAM,                     "AND",                                         "AND",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  394 */  { fnLogicalOr,                 NOPARAM,                     "OR",                                          "OR",                                          0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  395 */  { fnLogicalXor,                NOPARAM,                     "XOR",                                         "XOR",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  396 */  { fnLogicalNand,               NOPARAM,                     "NAND",                                        "NAND",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  397 */  { fnLogicalNor,                NOPARAM,                     "NOR",                                         "NOR",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  398 */  { fnLogicalXnor,               NOPARAM,                     "XNOR",                                        "XNOR",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  399 */  { fnBs,                        TM_VALUE,                    "BS?",                                         "BS?",                                         0,      63,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  400 */  { fnBc,                        TM_VALUE,                    "BC?",                                         "BC?",                                         0,      63,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  401 */  { fnCb,                        TM_VALUE,                    "CB",                                          "CB",                                          0,      63,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  402 */  { fnSb,                        TM_VALUE,                    "SB",                                          "SB",                                          0,      63,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  403 */  { fnFb,                        TM_VALUE,                    "FB",                                          "FB",                                          0,      63,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  404 */  { fnRl,                        TM_VALUE,                    "RL",                                          "RL",                                          0,      63,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  405 */  { fnRlc,                       TM_VALUE,                    "RLC",                                         "RLC",                                         0,      63,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  406 */  { fnRr,                        TM_VALUE,                    "RR",                                          "RR",                                          0,      63,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  407 */  { fnRrc,                       TM_VALUE,                    "RRC",                                         "RRC",                                         0,      63,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  408 */  { fnSl,                        TM_VALUE,                    "SL",                                          "SL",                                          0,      63,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  409 */  { fnSr,                        TM_VALUE,                    "SR",                                          "SR",                                          0,      63,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  410 */  { fnAsr,                       TM_VALUE,                    "ASR",                                         "ASR",                                         0,      63,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  411 */  { fnLj,                        NOPARAM,                     "LJ",                                          "LJ",                                          0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  412 */  { fnRj,                        NOPARAM,                     "RJ",                                          "RJ",                                          0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  413 */  { fnMaskl,                     TM_VALUE,                    "MASKL",                                       "MASKL",                                       0,      64,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  414 */  { fnMaskr,                     TM_VALUE,                    "MASKR",                                       "MASKR",                                       0,      64,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  415 */  { fnMirror,                    NOPARAM,                     "MIRROR",                                      "MIRROR",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  416 */  { fnCountBits,                 NOPARAM,                     "#B",                                          "#B",                                          0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  417 */  { fnSdl,                       TM_VALUE,                    "SDL",                                         "SDL",                                         0,      99,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  418 */  { fnSdr,                       TM_VALUE,                    "SDR",                                         "SDR",                                         0,      99,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  419 */  { itemToBeCoded,               NOPARAM,                     "0419",                                        "0419",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  420 */  { itemToBeCoded,               NOPARAM,                     "0420",                                        "0420",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  421 */  { itemToBeCoded,               NOPARAM,                     "0421",                                        "0421",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  422 */  { itemToBeCoded,               NOPARAM,                     "0422",                                        "0422",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},


// Statistical sums
/*  423 */  { fnSigma,                     1,                           STD_SIGMA "+",                                 STD_SIGMA "+",                                 0,       0,       CAT_FNCT, SLS_DISABLED , US_ENABLED  },
/*  424 */  { fnSigma,                     2,                           STD_SIGMA "-",                                 STD_SIGMA "-",                                 0,       0,       CAT_FNCT, SLS_DISABLED , US_ENABLED  },
/*  425 */  { fnStatSum,                   0,                           "n" STD_SIGMA,                                 "n",                                           0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  426 */  { fnStatSum,                   1,                           STD_SIGMA "x",                                 STD_SIGMA "x",                                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  427 */  { fnStatSum,                   2,                           STD_SIGMA "y",                                 STD_SIGMA "y",                                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  428 */  { fnStatSum,                   3,                           STD_SIGMA "x" STD_SUP_2,                       STD_SIGMA "x" STD_SUP_2,                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  429 */  { fnStatSum,                   4,                           STD_SIGMA "x" STD_SUP_2 "y",                   STD_SIGMA "x" STD_SUP_2 "y",                   0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  430 */  { fnStatSum,                   5,                           STD_SIGMA "y" STD_SUP_2,                       STD_SIGMA "y" STD_SUP_2,                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  431 */  { fnStatSum,                   6,                           STD_SIGMA "xy",                                STD_SIGMA "xy",                                0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  432 */  { fnStatSum,                   7,                           STD_SIGMA "lnxy",                              STD_SIGMA "lnxy",                              0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  433 */  { fnStatSum,                   8,                           STD_SIGMA "lnx",                               STD_SIGMA "lnx",                               0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  434 */  { fnStatSum,                   9,                           STD_SIGMA "ln" STD_SUP_2 "x",                  STD_SIGMA "ln" STD_SUP_2 "x",                  0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  435 */  { fnStatSum,                   10,                          STD_SIGMA "ylnx",                              STD_SIGMA "ylnx",                              0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  436 */  { fnStatSum,                   11,                          STD_SIGMA "lny",                               STD_SIGMA "lny",                               0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  437 */  { fnStatSum,                   12,                          STD_SIGMA "ln" STD_SUP_2 "y",                  STD_SIGMA "ln" STD_SUP_2 "y",                  0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  438 */  { fnStatSum,                   13,                          STD_SIGMA "xlny",                              STD_SIGMA "xlny",                              0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  439 */  { fnStatSum,                   14,                          STD_SIGMA "lny/x",                             STD_SIGMA "lny/x",                             0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  440 */  { fnStatSum,                   15,                          STD_SIGMA "x" STD_SUP_2 "/y",                  STD_SIGMA "x" STD_SUP_2 "/y",                  0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  441 */  { fnStatSum,                   16,                          STD_SIGMA STD_SUP_1 "/x",                      STD_SIGMA STD_SUP_1 "/x",                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  442 */  { fnStatSum,                   17,                          STD_SIGMA STD_SUP_1 "/x" STD_SUP_2,            STD_SIGMA STD_SUP_1 "/x" STD_SUP_2,            0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  443 */  { fnStatSum,                   18,                          STD_SIGMA "x/y",                               STD_SIGMA "x/y",                               0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  444 */  { fnStatSum,                   19,                          STD_SIGMA STD_SUP_1 "/y",                      STD_SIGMA STD_SUP_1 "/y",                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  445 */  { fnStatSum,                   20,                          STD_SIGMA STD_SUP_1 "/y" STD_SUP_2,            STD_SIGMA STD_SUP_1 "/y" STD_SUP_2,            0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  446 */  { fnStatSum,                   21,                          STD_SIGMA "x" STD_SUP_3,                       STD_SIGMA "x" STD_SUP_3,                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  447 */  { fnStatSum,                   22,                          STD_SIGMA "x" STD_SUP_4,                       STD_SIGMA "x" STD_SUP_4,                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/*  448 */  { itemToBeCoded,               NOPARAM,                     "0448",                                        "0448",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  449 */  { itemToBeCoded,               NOPARAM,                     "0449",                                        "0449",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  450 */  { itemToBeCoded,               NOPARAM,                     "0450",                                        "0450",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  451 */  { itemToBeCoded,               NOPARAM,                     "0451",                                        "0451",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  452 */  { itemToBeCoded,               NOPARAM,                     "0452",                                        "0452",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},


// System flags
/*  453 */  { fnGetSystemFlag,             FLAG_USB,                    "USB",                                         "USB",                                         0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  454 */  { fnGetSystemFlag,             FLAG_TDM24,                  "TDM24",                                       "TDM24",                                       0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  455 */  { fnGetSystemFlag,             FLAG_YMD,                    "YMD",                                         "YMD",                                         0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  456 */  { fnGetSystemFlag,             FLAG_DMY,                    "DMY",                                         "DMY",                                         0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  457 */  { fnGetSystemFlag,             FLAG_MDY,                    "MDY",                                         "MDY",                                         0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  458 */  { fnGetSystemFlag,             FLAG_CPXRES,                 "CPXRES",                                      "CPXRES",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  459 */  { fnGetSystemFlag,             FLAG_CPXj,                   "CPXj",                                        "CPXj",                                        0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  460 */  { fnGetSystemFlag,             FLAG_POLAR,                  "POLAR",                                       "POLAR",                                       0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  461 */  { fnGetSystemFlag,             FLAG_FRACT,                  "FRACT",                                       "FRACT",                                       0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  462 */  { fnGetSystemFlag,             FLAG_PROPFR,                 "PROPFR",                                      "PROPFR",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  463 */  { fnGetSystemFlag,             FLAG_DENANY,                 "DENANY",                                      "DENANY",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  464 */  { fnGetSystemFlag,             FLAG_DENFIX,                 "DENFIX",                                      "DENFIX",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  465 */  { fnGetSystemFlag,             FLAG_CARRY,                  "CARRY",                                       "CARRY",                                       0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  466 */  { fnGetSystemFlag,             FLAG_OVERFLOW,               "OVERFL",                                      "OVERFL",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  467 */  { fnGetSystemFlag,             FLAG_LEAD0,                  "LEAD.0",                                      "LEAD.0",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  468 */  { fnGetSystemFlag,             FLAG_ALPHA,                  "ALPHA",                                       "ALPHA",                                       0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  469 */  { fnGetSystemFlag,             FLAG_alphaCAP,               STD_alpha "CAP",                               STD_alpha "CAP",                               0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  470 */  { fnGetSystemFlag,             FLAG_RUNTIM,                 "RUNTIM",                                      "RUNTIM",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  471 */  { fnGetSystemFlag,             FLAG_RUNIO,                  "RUNIO",                                       "RUNIO",                                       0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  472 */  { fnGetSystemFlag,             FLAG_PRINT,                  "PRINT",                                       "PRINT",                                       0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  473 */  { fnGetSystemFlag,             FLAG_TRACE,                  "TRACE",                                       "TRACE",                                       0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  474 */  { fnGetSystemFlag,             FLAG_USER,                   "USER",                                        "USER",                                        0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  475 */  { fnGetSystemFlag,             FLAG_LOWBAT,                 "LOWBAT",                                      "LOWBAT",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  476 */  { fnGetSystemFlag,             FLAG_SLOW,                   "SLOW",                                        "SLOW",                                        0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  477 */  { fnGetSystemFlag,             FLAG_SPCRES,                 "SPCRES",                                      "SPCRES",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  478 */  { fnGetSystemFlag,             FLAG_SSIZE8,                 "SSIZE8",                                      "SSIZE8",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  479 */  { fnGetSystemFlag,             FLAG_QUIET,                  "QUIET",                                       "QUIET",                                       0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  480 */  { fnGetSystemFlag,             FLAG_DECIMP,                 "DECIM.",                                      "DECIM.",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  481 */  { fnGetSystemFlag,             FLAG_MULTx,                  "MULT" STD_CROSS,                              "MULT" STD_CROSS,                              0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  482 */  { fnGetSystemFlag,             FLAG_ALLENG,                 "ALLENG",                                      "ALLENG",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  483 */  { fnGetSystemFlag,             FLAG_GROW,                   "GROW",                                        "GROW",                                        0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  484 */  { fnGetSystemFlag,             FLAG_AUTOFF,                 "AUTOFF",                                      "AUTOFF",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  485 */  { fnGetSystemFlag,             FLAG_AUTXEQ,                 "AUTXEQ",                                      "AUTXEQ",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  486 */  { fnGetSystemFlag,             FLAG_PRTACT,                 "PRTACT",                                      "PRTACT",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  487 */  { fnGetSystemFlag,             FLAG_NUMIN,                  "NUM.IN",                                      "NUM.IN",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  488 */  { fnGetSystemFlag,             FLAG_ALPIN,                  "ALP.IN",                                      "ALP.IN",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  489 */  { fnGetSystemFlag,             FLAG_ASLIFT,                 "ASLIFT",                                      "ASLIFT",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  490 */  { fnGetSystemFlag,             FLAG_IGN1ER,                 "IGN1ER",                                      "IGN1ER",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  491 */  { fnGetSystemFlag,             FLAG_INTING,                 "INTING",                                      "INTING",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  492 */  { fnGetSystemFlag,             FLAG_SOLVING,                "SOLVING",                                     "SOLVING",                                     0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  493 */  { fnGetSystemFlag,             FLAG_VMDISP,                 "VMDISP",                                      "VMDISP",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED, US_UNCHANGED},
/*  494 */  { itemToBeCoded,               NOPARAM,                     "0494",                                        "0494",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  495 */  { itemToBeCoded,               NOPARAM,                     "0495",                                        "0495",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  496 */  { itemToBeCoded,               NOPARAM,                     "0496",                                        "0496",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  497 */  { itemToBeCoded,               NOPARAM,                     "0497",                                        "0497",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  498 */  { itemToBeCoded,               NOPARAM,                     "0498",                                        "0498",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  499 */  { itemToBeCoded,               NOPARAM,                     "0499",                                        "0499",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  500 */  { itemToBeCoded,               NOPARAM,                     "0500",                                        "0500",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  501 */  { itemToBeCoded,               NOPARAM,                     "0501",                                        "0501",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  502 */  { itemToBeCoded,               NOPARAM,                     "0502",                                        "0502",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  503 */  { itemToBeCoded,               NOPARAM,                     "0503",                                        "0503",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  504 */  { itemToBeCoded,               NOPARAM,                     "0504",                                        "0504",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  505 */  { itemToBeCoded,               NOPARAM,                     "0505",                                        "0505",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  506 */  { itemToBeCoded,               NOPARAM,                     "0506",                                        "0506",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  507 */  { itemToBeCoded,               NOPARAM,                     "0507",                                        "0507",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  508 */  { itemToBeCoded,               NOPARAM,                     "0508",                                        "0508",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  509 */  { itemToBeCoded,               NOPARAM,                     "0509",                                        "0509",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  510 */  { itemToBeCoded,               NOPARAM,                     "0510",                                        "0510",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  511 */  { itemToBeCoded,               NOPARAM,                     "0511",                                        "0511",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  512 */  { itemToBeCoded,               NOPARAM,                     "0512",                                        "0512",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  513 */  { itemToBeCoded,               NOPARAM,                     "0513",                                        "0513",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  514 */  { itemToBeCoded,               NOPARAM,                     "0514",                                        "0514",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  515 */  { itemToBeCoded,               NOPARAM,                     "0515",                                        "0515",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  516 */  { itemToBeCoded,               NOPARAM,                     "0516",                                        "0516",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},


// Bufferized items
/*  517 */  { addItemToBuffer,             REGISTER_A,                  "ST.A",                                        "ST.A",                                        0,       0,       CAT_REGS, SLS_UNCHANGED, US_UNCHANGED},
/*  518 */  { addItemToBuffer,             REGISTER_B,                  "ST.B",                                        "ST.B",                                        0,       0,       CAT_REGS, SLS_UNCHANGED, US_UNCHANGED},
/*  519 */  { addItemToBuffer,             REGISTER_C,                  "ST.C",                                        "ST.C",                                        0,       0,       CAT_REGS, SLS_UNCHANGED, US_UNCHANGED},
/*  520 */  { addItemToBuffer,             REGISTER_D,                  "ST.D",                                        "ST.D",                                        0,       0,       CAT_REGS, SLS_UNCHANGED, US_UNCHANGED},
/*  521 */  { addItemToBuffer,             REGISTER_T,                  "ST.T",                                        "ST.T",                                        0,       0,       CAT_REGS, SLS_UNCHANGED, US_UNCHANGED},
/*  522 */  { addItemToBuffer,             REGISTER_X,                  "ST.X",                                        "ST.X",                                        0,       0,       CAT_REGS, SLS_UNCHANGED, US_UNCHANGED},
/*  523 */  { addItemToBuffer,             REGISTER_Y,                  "ST.Y",                                        "ST.Y",                                        0,       0,       CAT_REGS, SLS_UNCHANGED, US_UNCHANGED},
/*  524 */  { addItemToBuffer,             REGISTER_Z,                  "ST.Z",                                        "ST.Z",                                        0,       0,       CAT_REGS, SLS_UNCHANGED, US_UNCHANGED},
/*  525 */  { addItemToBuffer,             ITM_INDIRECTION,             STD_RIGHT_ARROW,                               STD_RIGHT_ARROW,                               0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  526 */  { addItemToBuffer,             REGISTER_X,                  "REG_X",                                       "X",                                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED}, // The order
/*  527 */  { addItemToBuffer,             REGISTER_Y,                  "REG_Y",                                       "Y",                                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED}, // of these 8
/*  528 */  { addItemToBuffer,             REGISTER_Z,                  "REG_Z",                                       "Z",                                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED}, // lines MUST
/*  529 */  { addItemToBuffer,             REGISTER_T,                  "REG_T",                                       "T",                                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED}, // be kept as
/*  530 */  { addItemToBuffer,             REGISTER_A,                  "REG_A",                                       "A",                                           0,       0,       CAT_REGS, SLS_UNCHANGED, US_UNCHANGED}, // is. Do not
/*  531 */  { addItemToBuffer,             REGISTER_B,                  "REG_B",                                       "B",                                           0,       0,       CAT_REGS, SLS_UNCHANGED, US_UNCHANGED}, // put them in
/*  532 */  { addItemToBuffer,             REGISTER_C,                  "REG_C",                                       "C",                                           0,       0,       CAT_REGS, SLS_UNCHANGED, US_UNCHANGED}, // alphabetical
/*  533 */  { addItemToBuffer,             REGISTER_D,                  "REG_D",                                       "D",                                           0,       0,       CAT_REGS, SLS_UNCHANGED, US_UNCHANGED}, // order!
/*  534 */  { addItemToBuffer,             REGISTER_L,                  "REG_L",                                       "L",                                           0,       0,       CAT_REGS, SLS_UNCHANGED, US_UNCHANGED},
/*  535 */  { addItemToBuffer,             REGISTER_I,                  "REG_I",                                       "I",                                           0,       0,       CAT_REGS, SLS_UNCHANGED, US_UNCHANGED},
/*  536 */  { addItemToBuffer,             REGISTER_J,                  "REG_J",                                       "J",                                           0,       0,       CAT_REGS, SLS_UNCHANGED, US_UNCHANGED},
/*  537 */  { addItemToBuffer,             REGISTER_K,                  "REG_K",                                       "K",                                           0,       0,       CAT_REGS, SLS_UNCHANGED, US_UNCHANGED},
/*  538 */  { addItemToBuffer,             ITM_0,                       "",                                            "0",                                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  539 */  { addItemToBuffer,             ITM_1,                       "",                                            "1",                                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  540 */  { addItemToBuffer,             ITM_2,                       "",                                            "2",                                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  541 */  { addItemToBuffer,             ITM_3,                       "",                                            "3",                                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  542 */  { addItemToBuffer,             ITM_4,                       "",                                            "4",                                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  543 */  { addItemToBuffer,             ITM_5,                       "",                                            "5",                                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  544 */  { addItemToBuffer,             ITM_6,                       "",                                            "6",                                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  545 */  { addItemToBuffer,             ITM_7,                       "",                                            "7",                                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  546 */  { addItemToBuffer,             ITM_8,                       "",                                            "8",                                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  547 */  { addItemToBuffer,             ITM_9,                       "",                                            "9",                                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  548 */  { addItemToBuffer,             ITM_A,                       "A",                                           "A",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  549 */  { addItemToBuffer,             ITM_B,                       "B",                                           "B",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  550 */  { addItemToBuffer,             ITM_C,                       "C",                                           "C",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  551 */  { addItemToBuffer,             ITM_D,                       "D",                                           "D",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  552 */  { addItemToBuffer,             ITM_E,                       "E",                                           "E",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  553 */  { addItemToBuffer,             ITM_F,                       "F",                                           "F",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  554 */  { addItemToBuffer,             ITM_G,                       "G",                                           "G",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  555 */  { addItemToBuffer,             ITM_H,                       "H",                                           "H",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  556 */  { addItemToBuffer,             ITM_I,                       "I",                                           "I",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  557 */  { addItemToBuffer,             ITM_J,                       "J",                                           "J",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  558 */  { addItemToBuffer,             ITM_K,                       "K",                                           "K",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  559 */  { addItemToBuffer,             ITM_L,                       "L",                                           "L",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  560 */  { addItemToBuffer,             ITM_M,                       "M",                                           "M",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  561 */  { addItemToBuffer,             ITM_N,                       "N",                                           "N",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  562 */  { addItemToBuffer,             ITM_O,                       "O",                                           "O",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  563 */  { addItemToBuffer,             ITM_P,                       "P",                                           "P",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  564 */  { addItemToBuffer,             ITM_Q,                       "Q",                                           "Q",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  565 */  { addItemToBuffer,             ITM_R,                       "R",                                           "R",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  566 */  { addItemToBuffer,             ITM_S,                       "S",                                           "S",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  567 */  { addItemToBuffer,             ITM_T,                       "T",                                           "T",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  568 */  { addItemToBuffer,             ITM_U,                       "U",                                           "U",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  569 */  { addItemToBuffer,             ITM_V,                       "V",                                           "V",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  570 */  { addItemToBuffer,             ITM_W,                       "W",                                           "W",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  571 */  { addItemToBuffer,             ITM_X,                       "X",                                           "X",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  572 */  { addItemToBuffer,             ITM_Y,                       "Y",                                           "Y",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  573 */  { addItemToBuffer,             ITM_Z,                       "Z",                                           "Z",                                           0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  574 */  { addItemToBuffer,             ITM_a,                       "a",                                           "a",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  575 */  { addItemToBuffer,             ITM_b,                       "b",                                           "b",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  576 */  { addItemToBuffer,             ITM_c,                       "c",                                           "c",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  577 */  { addItemToBuffer,             ITM_d,                       "d",                                           "d",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  578 */  { addItemToBuffer,             ITM_e,                       "e",                                           "e",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  579 */  { addItemToBuffer,             ITM_f,                       "f",                                           "f",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  580 */  { addItemToBuffer,             ITM_g,                       "g",                                           "g",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  581 */  { addItemToBuffer,             ITM_h,                       "h",                                           "h",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  582 */  { addItemToBuffer,             ITM_i,                       "i",                                           "i",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  583 */  { addItemToBuffer,             ITM_j,                       "j",                                           "j",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  584 */  { addItemToBuffer,             ITM_k,                       "k",                                           "k",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  585 */  { addItemToBuffer,             ITM_l,                       "l",                                           "l",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  586 */  { addItemToBuffer,             ITM_m,                       "m",                                           "m",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  587 */  { addItemToBuffer,             ITM_n,                       "n",                                           "n",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  588 */  { addItemToBuffer,             ITM_o,                       "o",                                           "o",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  589 */  { addItemToBuffer,             ITM_p,                       "p",                                           "p",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  590 */  { addItemToBuffer,             ITM_q,                       "q",                                           "q",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  591 */  { addItemToBuffer,             ITM_r,                       "r",                                           "r",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  592 */  { addItemToBuffer,             ITM_s,                       "s",                                           "s",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  593 */  { addItemToBuffer,             ITM_t,                       "t",                                           "t",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  594 */  { addItemToBuffer,             ITM_u,                       "u",                                           "u",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  595 */  { addItemToBuffer,             ITM_v,                       "v",                                           "v",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  596 */  { addItemToBuffer,             ITM_w,                       "w",                                           "w",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  597 */  { addItemToBuffer,             ITM_x,                       "x",                                           "x",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  598 */  { addItemToBuffer,             ITM_y,                       "y",                                           "y",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  599 */  { addItemToBuffer,             ITM_z,                       "z",                                           "z",                                           0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  600 */  { addItemToBuffer,             ITM_ALPHA,                   "",                                            STD_ALPHA,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  601 */  { itemToBeCoded,               NOPARAM,                     "0601",                                        "0601",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  602 */  { addItemToBuffer,             ITM_BETA,                    "",                                            STD_BETA,                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  603 */  { addItemToBuffer,             ITM_GAMMA,                   "",                                            STD_GAMMA,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  604 */  { addItemToBuffer,             ITM_DELTA,                   "",                                            STD_DELTA,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  605 */  { addItemToBuffer,             ITM_EPSILON,                 "",                                            STD_EPSILON,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  606 */  { itemToBeCoded,               NOPARAM,                     "0606",                                        "0606",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  607 */  { addItemToBuffer,             ITM_ZETA,                    "",                                            STD_ZETA,                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  608 */  { addItemToBuffer,             ITM_ETA,                     "",                                            STD_ETA,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  609 */  { itemToBeCoded,               NOPARAM,                     "0609",                                        "0609",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  610 */  { addItemToBuffer,             ITM_THETA,                   "",                                            STD_THETA,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  611 */  { addItemToBuffer,             ITM_IOTA,                    "",                                            STD_IOTA,                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  612 */  { itemToBeCoded,               NOPARAM,                     "0612",                                        "0612",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  613 */  { itemToBeCoded,               NOPARAM,                     "0613",                                        "0613",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  614 */  { addItemToBuffer,             ITM_IOTA_DIALYTIKA,          "",                                            STD_IOTA_DIALYTIKA,                            0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  615 */  { addItemToBuffer,             ITM_KAPPA,                   "",                                            STD_KAPPA,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  616 */  { addItemToBuffer,             ITM_LAMBDA,                  "",                                            STD_LAMBDA,                                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  617 */  { addItemToBuffer,             ITM_MU,                      "",                                            STD_MU,                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  618 */  { addItemToBuffer,             ITM_NU,                      "",                                            STD_NU,                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  619 */  { addItemToBuffer,             ITM_XI,                      "",                                            STD_XI,                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  620 */  { addItemToBuffer,             ITM_OMICRON,                 "",                                            STD_OMICRON,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  621 */  { itemToBeCoded,               NOPARAM,                     "0621",                                        "0621",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  622 */  { addItemToBuffer,             ITM_PI,                      "",                                            STD_PI,                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  623 */  { addItemToBuffer,             ITM_RHO,                     "",                                            STD_RHO,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  624 */  { addItemToBuffer,             ITM_SIGMA,                   "",                                            STD_SIGMA,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  625 */  { itemToBeCoded,               NOPARAM,                     "0625",                                        "0625",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  626 */  { addItemToBuffer,             ITM_TAU,                     "",                                            STD_TAU,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  627 */  { addItemToBuffer,             ITM_UPSILON,                 "",                                            STD_UPSILON,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  628 */  { itemToBeCoded,               NOPARAM,                     "0628",                                        "0628",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  629 */  { addItemToBuffer,             ITM_UPSILON_DIALYTIKA,       "",                                            STD_UPSILON_DIALYTIKA,                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  630 */  { itemToBeCoded,               NOPARAM,                     "0630",                                        "0630",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  631 */  { addItemToBuffer,             ITM_PHI,                     "",                                            STD_PHI,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  632 */  { addItemToBuffer,             ITM_CHI,                     "",                                            STD_CHI,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  633 */  { addItemToBuffer,             ITM_PSI,                     "",                                            STD_PSI,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  634 */  { addItemToBuffer,             ITM_OMEGA,                   "",                                            STD_OMEGA,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  635 */  { itemToBeCoded,               NOPARAM,                     "0635",                                        "0635",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  636 */  { addItemToBuffer,             ITM_alpha,                   "",                                            STD_alpha,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  637 */  { addItemToBuffer,             ITM_alpha_TONOS,             "",                                            STD_alpha_TONOS,                               0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  638 */  { addItemToBuffer,             ITM_beta,                    "",                                            STD_beta,                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  639 */  { addItemToBuffer,             ITM_gamma,                   "",                                            STD_gamma,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  640 */  { addItemToBuffer,             ITM_delta,                   "",                                            STD_delta,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  641 */  { addItemToBuffer,             ITM_epsilon,                 "",                                            STD_epsilon,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  642 */  { addItemToBuffer,             ITM_epsilon_TONOS,           "",                                            STD_epsilon_TONOS,                             0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  643 */  { addItemToBuffer,             ITM_zeta,                    "",                                            STD_zeta,                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  644 */  { addItemToBuffer,             ITM_eta,                     "",                                            STD_eta,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  645 */  { addItemToBuffer,             ITM_eta_TONOS,               "",                                            STD_eta_TONOS,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  646 */  { addItemToBuffer,             ITM_theta,                   "",                                            STD_theta,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  647 */  { addItemToBuffer,             ITM_iota,                    "",                                            STD_iota,                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  648 */  { addItemToBuffer,             ITM_iotaTON,                 "",                                            STD_iota_TONOS,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  649 */  { addItemToBuffer,             ITM_iota_DIALYTIKA_TONOS,    "",                                            STD_iota_DIALYTIKA_TONOS,                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  650 */  { addItemToBuffer,             ITM_iota_DIALYTIKA,          "",                                            STD_iota_DIALYTIKA,                            0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  651 */  { addItemToBuffer,             ITM_kappa,                   "",                                            STD_kappa,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  652 */  { addItemToBuffer,             ITM_lambda,                  "",                                            STD_lambda,                                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  653 */  { addItemToBuffer,             ITM_mu,                      "",                                            STD_mu,                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  654 */  { addItemToBuffer,             ITM_nu,                      "",                                            STD_nu,                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  655 */  { addItemToBuffer,             ITM_xi,                      "",                                            STD_xi,                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  656 */  { addItemToBuffer,             ITM_omicron,                 "",                                            STD_omicron,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  657 */  { addItemToBuffer,             ITM_omicron_TONOS,           "",                                            STD_omicron_TONOS,                             0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  658 */  { addItemToBuffer,             ITM_pi,                      "",                                            STD_pi,                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  659 */  { addItemToBuffer,             ITM_rho,                     "",                                            STD_rho,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  660 */  { addItemToBuffer,             ITM_sigma,                   "",                                            STD_sigma,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  661 */  { addItemToBuffer,             ITM_sigma_end,               "",                                            STD_sigma_end,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  662 */  { addItemToBuffer,             ITM_tau,                     "",                                            STD_tau,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  663 */  { addItemToBuffer,             ITM_upsilon,                 "",                                            STD_upsilon,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  664 */  { addItemToBuffer,             ITM_upsilon_TONOS,           "",                                            STD_upsilon_TONOS,                             0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  665 */  { addItemToBuffer,             ITM_upsilon_DIALYTIKA,       "",                                            STD_upsilon_DIALYTIKA,                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  666 */  { addItemToBuffer,             ITM_upsilon_DIALYTIKA_TONOS, "",                                            STD_upsilon_DIALYTIKA_TONOS,                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  667 */  { addItemToBuffer,             ITM_phi,                     "",                                            STD_phi,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  668 */  { addItemToBuffer,             ITM_chi,                     "",                                            STD_chi,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  669 */  { addItemToBuffer,             ITM_psi,                     "",                                            STD_psi,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  670 */  { addItemToBuffer,             ITM_omega,                   "",                                            STD_omega,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  671 */  { addItemToBuffer,             ITM_omega_TONOS,             "",                                            STD_omega_TONOS,                               0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  672 */  { itemToBeCoded,               NOPARAM,                     "0672",                                        "0672",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  673 */  { itemToBeCoded,               NOPARAM,                     "0673",                                        "0673",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  674 */  { itemToBeCoded,               NOPARAM,                     "0674",                                        "0674",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  675 */  { itemToBeCoded,               NOPARAM,                     "0675",                                        "0675",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  676 */  { itemToBeCoded,               NOPARAM,                     "0676",                                        "0676",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  677 */  { itemToBeCoded,               NOPARAM,                     "0677",                                        "0677",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  678 */  { addItemToBuffer,             ITM_A_MACRON,                STD_A_MACRON,                                  STD_A_MACRON,                                  0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  679 */  { addItemToBuffer,             ITM_A_ACUTE,                 STD_A_ACUTE,                                   STD_A_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  680 */  { addItemToBuffer,             ITM_A_BREVE,                 STD_A_BREVE,                                   STD_A_BREVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  681 */  { addItemToBuffer,             ITM_A_GRAVE,                 STD_A_GRAVE,                                   STD_A_GRAVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  682 */  { addItemToBuffer,             ITM_A_DIARESIS,              STD_A_DIARESIS,                                STD_A_DIARESIS,                                0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  683 */  { addItemToBuffer,             ITM_A_TILDE,                 STD_A_TILDE,                                   STD_A_TILDE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  684 */  { addItemToBuffer,             ITM_A_CIRC,                  STD_A_CIRC,                                    STD_A_CIRC,                                    0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  685 */  { addItemToBuffer,             ITM_A_RING,                  STD_A_RING,                                    STD_A_RING,                                    0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  686 */  { addItemToBuffer,             ITM_AE,                      STD_AE,                                        STD_AE,                                        0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  687 */  { addItemToBuffer,             ITM_A_OGONEK,                STD_A_OGONEK,                                  STD_A_OGONEK,                                  0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  688 */  { addItemToBuffer,             ITM_C_ACUTE,                 STD_C_ACUTE,                                   STD_C_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  689 */  { addItemToBuffer,             ITM_C_CARON,                 STD_C_CARON,                                   STD_C_CARON,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  690 */  { addItemToBuffer,             ITM_C_CEDILLA,               STD_C_CEDILLA,                                 STD_C_CEDILLA,                                 0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  691 */  { addItemToBuffer,             ITM_D_STROKE,                STD_D_STROKE,                                  STD_D_STROKE,                                  0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  692 */  { addItemToBuffer,             ITM_D_CARON,                 STD_D_CARON,                                   STD_D_CARON,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  693 */  { addItemToBuffer,             ITM_E_MACRON,                STD_E_MACRON,                                  STD_E_MACRON,                                  0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  694 */  { addItemToBuffer,             ITM_E_ACUTE,                 STD_E_ACUTE,                                   STD_E_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  695 */  { addItemToBuffer,             ITM_E_BREVE,                 STD_E_BREVE,                                   STD_E_BREVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  696 */  { addItemToBuffer,             ITM_E_GRAVE,                 STD_E_GRAVE,                                   STD_E_GRAVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  697 */  { addItemToBuffer,             ITM_E_DIARESIS,              STD_E_DIARESIS,                                STD_E_DIARESIS,                                0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  698 */  { addItemToBuffer,             ITM_E_CIRC,                  STD_E_CIRC,                                    STD_E_CIRC,                                    0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  699 */  { addItemToBuffer,             ITM_E_OGONEK,                STD_E_OGONEK,                                  STD_E_OGONEK,                                  0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  700 */  { addItemToBuffer,             ITM_G_BREVE,                 STD_G_BREVE,                                   STD_G_BREVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  701 */  { itemToBeCoded,               NOPARAM,                     "0701",                                        "0701",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  702 */  { addItemToBuffer,             ITM_I_MACRON,                STD_I_MACRON,                                  STD_I_MACRON,                                  0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  703 */  { addItemToBuffer,             ITM_I_ACUTE,                 STD_I_ACUTE,                                   STD_I_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  704 */  { addItemToBuffer,             ITM_I_BREVE,                 STD_I_BREVE,                                   STD_I_BREVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  705 */  { addItemToBuffer,             ITM_I_GRAVE,                 STD_I_GRAVE,                                   STD_I_GRAVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  706 */  { addItemToBuffer,             ITM_I_DIARESIS,              STD_I_DIARESIS,                                STD_I_DIARESIS,                                0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  707 */  { addItemToBuffer,             ITM_I_CIRC,                  STD_I_CIRC,                                    STD_I_CIRC,                                    0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  708 */  { addItemToBuffer,             ITM_I_OGONEK,                STD_I_OGONEK,                                  STD_I_OGONEK,                                  0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  709 */  { addItemToBuffer,             ITM_I_DOT,                   STD_I_DOT,                                     STD_I_DOT,                                     0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  710 */  { addItemToBuffer,             ITM_I_DOTLESS,               "I",                                           "I",                                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  711 */  { addItemToBuffer,             ITM_L_STROKE,                STD_L_STROKE,                                  STD_L_STROKE,                                  0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  712 */  { addItemToBuffer,             ITM_L_ACUTE,                 STD_L_ACUTE,                                   STD_L_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  713 */  { addItemToBuffer,             ITM_L_APOSTROPHE,            STD_L_APOSTROPHE,                              STD_L_APOSTROPHE,                              0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  714 */  { addItemToBuffer,             ITM_N_ACUTE,                 STD_N_ACUTE,                                   STD_N_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  715 */  { addItemToBuffer,             ITM_N_CARON,                 STD_N_CARON,                                   STD_N_CARON,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  716 */  { addItemToBuffer,             ITM_N_TILDE,                 STD_N_TILDE,                                   STD_N_TILDE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  717 */  { addItemToBuffer,             ITM_O_MACRON,                STD_O_MACRON,                                  STD_O_MACRON,                                  0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  718 */  { addItemToBuffer,             ITM_O_ACUTE,                 STD_O_ACUTE,                                   STD_O_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  719 */  { addItemToBuffer,             ITM_O_BREVE,                 STD_O_BREVE,                                   STD_O_BREVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  720 */  { addItemToBuffer,             ITM_O_GRAVE,                 STD_O_GRAVE,                                   STD_O_GRAVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  721 */  { addItemToBuffer,             ITM_O_DIARESIS,              STD_O_DIARESIS,                                STD_O_DIARESIS,                                0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  722 */  { addItemToBuffer,             ITM_O_TILDE,                 STD_O_TILDE,                                   STD_O_TILDE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  723 */  { addItemToBuffer,             ITM_O_CIRC,                  STD_O_CIRC,                                    STD_O_CIRC,                                    0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  724 */  { addItemToBuffer,             ITM_O_STROKE,                STD_O_STROKE,                                  STD_O_STROKE,                                  0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  725 */  { addItemToBuffer,             ITM_OE,                      STD_OE,                                        STD_OE,                                        0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  726 */  { itemToBeCoded,               NOPARAM,                     "0726",                                        "0726",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  727 */  { itemToBeCoded,               NOPARAM,                     "0727",                                        "0727",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  728 */  { addItemToBuffer,             ITM_S_SHARP,                 STD_s_SHARP,                                   STD_s_SHARP,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  729 */  { addItemToBuffer,             ITM_S_ACUTE,                 STD_S_ACUTE,                                   STD_S_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  730 */  { addItemToBuffer,             ITM_S_CARON,                 STD_S_CARON,                                   STD_S_CARON,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  731 */  { addItemToBuffer,             ITM_S_CEDILLA,               STD_S_CEDILLA,                                 STD_S_CEDILLA,                                 0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  732 */  { addItemToBuffer,             ITM_T_CARON,                 STD_T_CARON,                                   STD_T_CARON,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  733 */  { addItemToBuffer,             ITM_T_CEDILLA,               STD_T_CEDILLA,                                 STD_T_CEDILLA,                                 0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  734 */  { addItemToBuffer,             ITM_U_MACRON,                STD_U_MACRON,                                  STD_U_MACRON,                                  0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  735 */  { addItemToBuffer,             ITM_U_ACUTE,                 STD_U_ACUTE,                                   STD_U_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  736 */  { addItemToBuffer,             ITM_U_BREVE,                 STD_U_BREVE,                                   STD_U_BREVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  737 */  { addItemToBuffer,             ITM_U_GRAVE,                 STD_U_GRAVE,                                   STD_U_GRAVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  738 */  { addItemToBuffer,             ITM_U_DIARESIS,              STD_U_DIARESIS,                                STD_U_DIARESIS,                                0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  739 */  { addItemToBuffer,             ITM_U_TILDE,                 STD_U_TILDE,                                   STD_U_TILDE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  740 */  { addItemToBuffer,             ITM_U_CIRC,                  STD_U_CIRC,                                    STD_U_CIRC,                                    0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  741 */  { addItemToBuffer,             ITM_U_RING,                  STD_U_RING,                                    STD_U_RING,                                    0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  742 */  { addItemToBuffer,             ITM_W_CIRC,                  STD_W_CIRC,                                    STD_W_CIRC,                                    0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  743 */  { itemToBeCoded,               NOPARAM,                     "0743",                                        "0743",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  744 */  { itemToBeCoded,               NOPARAM,                     "0744",                                        "0744",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  745 */  { itemToBeCoded,               NOPARAM,                     "0745",                                        "0745",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  746 */  { addItemToBuffer,             ITM_Y_CIRC,                  STD_Y_CIRC,                                    STD_Y_CIRC,                                    0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  747 */  { addItemToBuffer,             ITM_Y_ACUTE,                 STD_Y_ACUTE,                                   STD_Y_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  748 */  { addItemToBuffer,             ITM_Y_DIARESIS,              STD_Y_DIARESIS,                                STD_Y_DIARESIS,                                0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  749 */  { addItemToBuffer,             ITM_Z_ACUTE,                 STD_Z_ACUTE,                                   STD_Z_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  750 */  { addItemToBuffer,             ITM_Z_CARON,                 STD_Z_CARON,                                   STD_Z_CARON,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  751 */  { addItemToBuffer,             ITM_Z_DOT,                   STD_Z_DOT,                                     STD_Z_DOT,                                     0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  752 */  { itemToBeCoded,               NOPARAM,                     "0752",                                        "0752",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  753 */  { itemToBeCoded,               NOPARAM,                     "0753",                                        "0753",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  754 */  { itemToBeCoded,               NOPARAM,                     "0754",                                        "0754",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  755 */  { itemToBeCoded,               NOPARAM,                     "0755",                                        "0755",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  756 */  { itemToBeCoded,               NOPARAM,                     "0756",                                        "0756",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  757 */  { itemToBeCoded,               NOPARAM,                     "0757",                                        "0757",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  758 */  { addItemToBuffer,             ITM_a_MACRON,                STD_a_MACRON,                                  STD_a_MACRON,                                  0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  759 */  { addItemToBuffer,             ITM_a_ACUTE,                 STD_a_ACUTE,                                   STD_a_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  760 */  { addItemToBuffer,             ITM_a_BREVE,                 STD_a_BREVE,                                   STD_a_BREVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  761 */  { addItemToBuffer,             ITM_a_GRAVE,                 STD_a_GRAVE,                                   STD_a_GRAVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  762 */  { addItemToBuffer,             ITM_a_DIARESIS,              STD_a_DIARESIS,                                STD_a_DIARESIS,                                0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  763 */  { addItemToBuffer,             ITM_a_TILDE,                 STD_a_TILDE,                                   STD_a_TILDE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  764 */  { addItemToBuffer,             ITM_a_CIRC,                  STD_a_CIRC,                                    STD_a_CIRC,                                    0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  765 */  { addItemToBuffer,             ITM_a_RING,                  STD_a_RING,                                    STD_a_RING,                                    0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  766 */  { addItemToBuffer,             ITM_ae,                      STD_ae,                                        STD_ae,                                        0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  767 */  { addItemToBuffer,             ITM_a_OGONEK,                STD_a_OGONEK,                                  STD_a_OGONEK,                                  0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  768 */  { addItemToBuffer,             ITM_c_ACUTE,                 STD_c_ACUTE,                                   STD_c_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  769 */  { addItemToBuffer,             ITM_c_CARON,                 STD_c_CARON,                                   STD_c_CARON,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  770 */  { addItemToBuffer,             ITM_c_CEDILLA,               STD_c_CEDILLA,                                 STD_c_CEDILLA,                                 0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  771 */  { addItemToBuffer,             ITM_d_STROKE,                STD_d_STROKE,                                  STD_d_STROKE,                                  0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  772 */  { addItemToBuffer,             ITM_d_APOSTROPHE,            STD_d_APOSTROPHE,                              STD_d_APOSTROPHE,                              0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  773 */  { addItemToBuffer,             ITM_e_MACRON,                STD_e_MACRON,                                  STD_e_MACRON,                                  0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  774 */  { addItemToBuffer,             ITM_e_ACUTE,                 STD_e_ACUTE,                                   STD_e_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  775 */  { addItemToBuffer,             ITM_e_BREVE,                 STD_e_BREVE,                                   STD_e_BREVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  776 */  { addItemToBuffer,             ITM_e_GRAVE,                 STD_e_GRAVE,                                   STD_e_GRAVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  777 */  { addItemToBuffer,             ITM_e_DIARESIS,              STD_e_DIARESIS,                                STD_e_DIARESIS,                                0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  778 */  { addItemToBuffer,             ITM_e_CIRC,                  STD_e_CIRC,                                    STD_e_CIRC,                                    0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  779 */  { addItemToBuffer,             ITM_e_OGONEK,                STD_e_OGONEK,                                  STD_e_OGONEK,                                  0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  780 */  { addItemToBuffer,             ITM_g_BREVE,                 STD_g_BREVE,                                   STD_g_BREVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  781 */  { addItemToBuffer,             ITM_h_STROKE,                "",                                            STD_h_STROKE,                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  782 */  { addItemToBuffer,             ITM_i_MACRON,                STD_i_MACRON,                                  STD_i_MACRON,                                  0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  783 */  { addItemToBuffer,             ITM_i_ACUTE,                 STD_i_ACUTE,                                   STD_i_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  784 */  { addItemToBuffer,             ITM_i_BREVE,                 STD_i_BREVE,                                   STD_i_BREVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  785 */  { addItemToBuffer,             ITM_i_GRAVE,                 STD_i_GRAVE,                                   STD_i_GRAVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  786 */  { addItemToBuffer,             ITM_i_DIARESIS,              STD_i_DIARESIS,                                STD_i_DIARESIS,                                0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  787 */  { addItemToBuffer,             ITM_i_CIRC,                  STD_i_CIRC,                                    STD_i_CIRC,                                    0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  788 */  { addItemToBuffer,             ITM_i_OGONEK,                STD_i_OGONEK,                                  STD_i_OGONEK,                                  0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  789 */  { addItemToBuffer,             ITM_i_DOT,                   "i",                                           "i",                                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  790 */  { addItemToBuffer,             ITM_i_DOTLESS,               STD_i_DOTLESS,                                 STD_i_DOTLESS,                                 0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  791 */  { addItemToBuffer,             ITM_l_STROKE,                STD_l_STROKE,                                  STD_l_STROKE,                                  0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  792 */  { addItemToBuffer,             ITM_l_ACUTE,                 STD_l_ACUTE,                                   STD_l_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  793 */  { addItemToBuffer,             ITM_l_APOSTROPHE,            STD_l_APOSTROPHE,                              STD_l_APOSTROPHE,                              0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  794 */  { addItemToBuffer,             ITM_n_ACUTE,                 STD_n_ACUTE,                                   STD_n_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  795 */  { addItemToBuffer,             ITM_n_CARON,                 STD_n_CARON,                                   STD_n_CARON,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  796 */  { addItemToBuffer,             ITM_n_TILDE,                 STD_n_TILDE,                                   STD_n_TILDE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  797 */  { addItemToBuffer,             ITM_o_MACRON,                STD_o_MACRON,                                  STD_o_MACRON,                                  0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  798 */  { addItemToBuffer,             ITM_o_ACUTE,                 STD_o_ACUTE,                                   STD_o_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  799 */  { addItemToBuffer,             ITM_o_BREVE,                 STD_o_BREVE,                                   STD_o_BREVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  800 */  { addItemToBuffer,             ITM_o_GRAVE,                 STD_o_GRAVE,                                   STD_o_GRAVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  801 */  { addItemToBuffer,             ITM_o_DIARESIS,              STD_o_DIARESIS,                                STD_o_DIARESIS,                                0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  802 */  { addItemToBuffer,             ITM_o_TILDE,                 STD_o_TILDE,                                   STD_o_TILDE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  803 */  { addItemToBuffer,             ITM_o_CIRC,                  STD_o_CIRC,                                    STD_o_CIRC,                                    0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  804 */  { addItemToBuffer,             ITM_o_STROKE,                STD_o_STROKE,                                  STD_o_STROKE,                                  0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  805 */  { addItemToBuffer,             ITM_oe,                      STD_oe,                                        STD_oe,                                        0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  806 */  { addItemToBuffer,             ITM_r_CARON,                 STD_r_CARON,                                   STD_r_CARON,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  807 */  { addItemToBuffer,             ITM_r_ACUTE,                 STD_r_ACUTE,                                   STD_r_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  808 */  { addItemToBuffer,             ITM_s_SHARP,                 STD_s_SHARP,                                   STD_s_SHARP,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  809 */  { addItemToBuffer,             ITM_s_ACUTE,                 STD_s_ACUTE,                                   STD_s_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  810 */  { addItemToBuffer,             ITM_s_CARON,                 STD_s_CARON,                                   STD_s_CARON,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  811 */  { addItemToBuffer,             ITM_s_CEDILLA,               STD_s_CEDILLA,                                 STD_s_CEDILLA,                                 0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  812 */  { addItemToBuffer,             ITM_t_APOSTROPHE,            STD_t_APOSTROPHE,                              STD_t_APOSTROPHE,                              0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  813 */  { addItemToBuffer,             ITM_t_CEDILLA,               STD_t_CEDILLA,                                 STD_t_CEDILLA,                                 0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  814 */  { addItemToBuffer,             ITM_u_MACRON,                STD_u_MACRON,                                  STD_u_MACRON,                                  0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  815 */  { addItemToBuffer,             ITM_u_ACUTE,                 STD_u_ACUTE,                                   STD_u_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  816 */  { addItemToBuffer,             ITM_u_BREVE,                 STD_u_BREVE,                                   STD_u_BREVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  817 */  { addItemToBuffer,             ITM_u_GRAVE,                 STD_u_GRAVE,                                   STD_u_GRAVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  818 */  { addItemToBuffer,             ITM_u_DIARESIS,              STD_u_DIARESIS,                                STD_u_DIARESIS,                                0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  819 */  { addItemToBuffer,             ITM_u_TILDE,                 STD_u_TILDE,                                   STD_u_TILDE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  820 */  { addItemToBuffer,             ITM_u_CIRC,                  STD_u_CIRC,                                    STD_u_CIRC,                                    0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  821 */  { addItemToBuffer,             ITM_u_RING,                  STD_u_RING,                                    STD_u_RING,                                    0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  822 */  { addItemToBuffer,             ITM_w_CIRC,                  STD_w_CIRC,                                    STD_w_CIRC,                                    0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  823 */  { addItemToBuffer,             ITM_x_BAR,                   "",                                            STD_x_BAR,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  824 */  { addItemToBuffer,             ITM_x_CIRC,                  "",                                            STD_x_CIRC,                                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  825 */  { addItemToBuffer,             ITM_y_BAR,                   "",                                            STD_y_BAR,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  826 */  { addItemToBuffer,             ITM_y_CIRC,                  STD_y_CIRC,                                    STD_y_CIRC,                                    0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  827 */  { addItemToBuffer,             ITM_y_ACUTE,                 STD_y_ACUTE,                                   STD_y_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  828 */  { addItemToBuffer,             ITM_y_DIARESIS,              STD_y_DIARESIS,                                STD_y_DIARESIS,                                0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  829 */  { addItemToBuffer,             ITM_z_ACUTE,                 STD_z_ACUTE,                                   STD_z_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  830 */  { addItemToBuffer,             ITM_z_CARON,                 STD_z_CARON,                                   STD_z_CARON,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  831 */  { addItemToBuffer,             ITM_z_DOT,                   STD_z_DOT,                                     STD_z_DOT,                                     0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  832 */  { itemToBeCoded,               NOPARAM,                     "0832",                                        "0832",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  833 */  { itemToBeCoded,               NOPARAM,                     "0833",                                        "0833",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  834 */  { itemToBeCoded,               NOPARAM,                     "0834",                                        "0834",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  835 */  { itemToBeCoded,               NOPARAM,                     "0835",                                        "0835",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  836 */  { itemToBeCoded,               NOPARAM,                     "0836",                                        "0836",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  837 */  { itemToBeCoded,               NOPARAM,                     "0837",                                        "0837",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  838 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_alpha,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  839 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_delta,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  840 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_mu,                                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  841 */  { addItemToBuffer,             ITM_SUB_SUN,                 "",                                            STD_SUB_SUN,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  842 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_SUN_b,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  843 */  { addItemToBuffer,             ITM_SUB_EARTH,               "",                                            STD_SUB_EARTH,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  844 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_EARTH_b,                               0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  845 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_PLUS,                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  846 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_MINUS,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  847 */  { addItemToBuffer,             ITM_SUB_INFINITY,            "",                                            STD_SUB_INFINITY,                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  848 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_0,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  849 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_1,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  850 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_2,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  851 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_3,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  852 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_4,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  853 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_5,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  854 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_6,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  855 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_7,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  856 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_8,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  857 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_9,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  858 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_10,                                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  859 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_A,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  860 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_B,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  861 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_C,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  862 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_D,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  863 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_E,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  864 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_F,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  865 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_G,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  866 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_H,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  867 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_I,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  868 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_J,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  869 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_K,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  870 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_L,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  871 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_M,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  872 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_N,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  873 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_O,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  874 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_P,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  875 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_Q,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  876 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_R,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  877 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_S,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  878 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_T,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  879 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_U,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  880 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_V,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  881 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_W,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  882 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_X,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  883 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_Y,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  884 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_Z,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  885 */  { addItemToBuffer,             ITM_SUB_E_OUTLINE,           "",                                            STD_SUB_E_OUTLINE,                             0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  886 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_a,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  887 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_b,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  888 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_c,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  889 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_d,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  890 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_e,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  891 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_h,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  892 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_i,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  893 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_j,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  894 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_k,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  895 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_l,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  896 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_m,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  897 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_n,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  898 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_o,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  899 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_p,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  900 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_q,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  901 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_s,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  902 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_t,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  903 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_u,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  904 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_v,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  905 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_w,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  906 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_x,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  907 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_y,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  908 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_z,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  909 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_a_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  910 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_e_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  911 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_k_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  912 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_l_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  913 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_m_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  914 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_n_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  915 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_o_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  916 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_p_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  917 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_s_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  918 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_u_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  919 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_x_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  920 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_PLUS,                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  921 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_MINUS,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  922 */  { addItemToBuffer,             ITM_SUP_MINUS_1,             "",                                            STD_SUP_MINUS_1,                               0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  923 */  { addItemToBuffer,             ITM_SUP_INFINITY,            "",                                            STD_SUP_INFINITY,                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  924 */  { addItemToBuffer,             ITM_SUP_ASTERISK,            "",                                            STD_SUP_ASTERISK,                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  925 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_0,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  926 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_1,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  927 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_2,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  928 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_3,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  929 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_4,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  930 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_5,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  931 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_6,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  932 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_7,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  933 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_8,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  934 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_9,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  935 */  { itemToBeCoded,               NOPARAM,                     "0935",                                        "0935",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  936 */  { addItemToBuffer,             ITM_SUP_T,                   "",                                            STD_SUP_T,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  937 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_a,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  938 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_f,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  939 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_g,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  940 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_h,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  941 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_r,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  942 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_x,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  943 */  { addItemToBuffer,             ITM_SPACE,                   "",                                            STD_SPACE,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  944 */  { addItemToBuffer,             ITM_EXCLAMATION_MARK,        "",                                            STD_EXCLAMATION_MARK,                          0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  945 */  { addItemToBuffer,             ITM_DOUBLE_QUOTE,            "",                                            STD_DOUBLE_QUOTE,                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  946 */  { addItemToBuffer,             ITM_NUMBER_SIGN,             "",                                            STD_NUMBER_SIGN,                               0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  947 */  { addItemToBuffer,             ITM_DOLLAR,                  "",                                            STD_DOLLAR,                                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  948 */  { addItemToBuffer,             ITM_PERCENT,                 "",                                            STD_PERCENT,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  949 */  { addItemToBuffer,             ITM_AMPERSAND,               "",                                            STD_AMPERSAND,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  950 */  { addItemToBuffer,             ITM_QUOTE,                   "",                                            STD_QUOTE,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  951 */  { addItemToBuffer,             ITM_LEFT_PARENTHESIS,        "",                                            STD_LEFT_PARENTHESIS,                          0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  952 */  { addItemToBuffer,             ITM_RIGHT_PARENTHESIS,       "",                                            STD_RIGHT_PARENTHESIS,                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  953 */  { addItemToBuffer,             ITM_ASTERISK,                "",                                            STD_ASTERISK,                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  954 */  { itemToBeCoded,               NOPARAM,                     "0954",                                        "0954",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/*  955 */  { addItemToBuffer,             ITM_PLUS,                    "",                                            STD_PLUS,                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  956 */  { addItemToBuffer,             ITM_COMMA,                   "",                                            STD_COMMA,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  957 */  { addItemToBuffer,             ITM_MINUS,                   "",                                            STD_MINUS,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  958 */  { addItemToBuffer,             ITM_PERIOD,                  "",                                            STD_PERIOD,                                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  959 */  { addItemToBuffer,             ITM_SLASH,                   "",                                            STD_SLASH,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  960 */  { addItemToBuffer,             ITM_COLON,                   "",                                            STD_COLON,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  961 */  { addItemToBuffer,             ITM_SEMICOLON,               "",                                            STD_SEMICOLON,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  962 */  { addItemToBuffer,             ITM_LESS_THAN,               "",                                            STD_LESS_THAN,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  963 */  { addItemToBuffer,             ITM_EQUAL,                   "",                                            STD_EQUAL,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  964 */  { addItemToBuffer,             ITM_GREATER_THAN,            "",                                            STD_GREATER_THAN,                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  965 */  { addItemToBuffer,             ITM_QUESTION_MARK,           "",                                            STD_QUESTION_MARK,                             0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  966 */  { addItemToBuffer,             ITM_AT,                      "",                                            STD_AT,                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  967 */  { addItemToBuffer,             ITM_LEFT_SQUARE_BRACKET,     "",                                            STD_LEFT_SQUARE_BRACKET,                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  968 */  { addItemToBuffer,             ITM_BACK_SLASH,              "",                                            STD_BACK_SLASH,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  969 */  { addItemToBuffer,             ITM_RIGHT_SQUARE_BRACKET,    "",                                            STD_RIGHT_SQUARE_BRACKET,                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  970 */  { addItemToBuffer,             ITM_CIRCUMFLEX,              "",                                            STD_CIRCUMFLEX,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  971 */  { addItemToBuffer,             ITM_UNDERSCORE,              "",                                            STD_UNDERSCORE,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  972 */  { addItemToBuffer,             ITM_LEFT_CURLY_BRACKET,      "",                                            STD_LEFT_CURLY_BRACKET,                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  973 */  { addItemToBuffer,             ITM_PIPE,                    "",                                            STD_PIPE,                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  974 */  { addItemToBuffer,             ITM_RIGHT_CURLY_BRACKET,     "",                                            STD_RIGHT_CURLY_BRACKET,                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  975 */  { addItemToBuffer,             ITM_TILDE,                   "",                                            STD_TILDE,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  976 */  { addItemToBuffer,             ITM_INVERTED_EXCLAMATION_MARK, "",                                            STD_INVERTED_EXCLAMATION_MARK,                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  977 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_CENT,                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  978 */  { addItemToBuffer,             ITM_POUND,                   "",                                            STD_POUND,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  979 */  { addItemToBuffer,             ITM_YEN,                     "",                                            STD_YEN,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  980 */  { addItemToBuffer,             ITM_SECTION,                 "",                                            STD_SECTION,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  981 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OVERFLOW_CARRY,                            0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  982 */  { addItemToBuffer,             ITM_LEFT_DOUBLE_ANGLE,       "",                                            STD_LEFT_DOUBLE_ANGLE,                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  983 */  { addItemToBuffer,             ITM_NOT,                     "",                                            STD_NOT,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  984 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DEGREE,                                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  985 */  { addItemToBuffer,             ITM_PLUS_MINUS,              "",                                            STD_PLUS_MINUS,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  986 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_mu_b,                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  987 */  { addItemToBuffer,             ITM_DOT,                     "",                                            STD_DOT,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  988 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ORDINAL,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  989 */  { addItemToBuffer,             ITM_RIGHT_DOUBLE_ANGLE,      "",                                            STD_RIGHT_DOUBLE_ANGLE,                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  990 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ONE_HALF,                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  991 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ONE_QUARTER,                               0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  992 */  { addItemToBuffer,             ITM_INVERTED_QUESTION_MARK,  "",                                            STD_INVERTED_QUESTION_MARK,                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  993 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ETH,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  994 */  { addItemToBuffer,             ITM_CROSS,                   "",                                            STD_CROSS,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  995 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_eth,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  996 */  { addItemToBuffer,             ITM_OBELUS,                  "",                                            STD_DIVIDE,                                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/*  997 */  { addItemToBuffer,             ITM_E_DOT,                   STD_E_DOT,                                     STD_E_DOT,                                     0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/*  998 */  { addItemToBuffer,             ITM_e_DOT,                   STD_e_DOT,                                     STD_e_DOT,                                     0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/*  999 */  { addItemToBuffer,             NOPARAM,                     STD_E_CARON,                                   STD_E_CARON,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/* 1000 */  { addItemToBuffer,             NOPARAM,                     STD_e_CARON,                                   STD_e_CARON,                                   0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/* 1001 */  { addItemToBuffer,             ITM_R_ACUTE,                 STD_R_ACUTE,                                   STD_R_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/* 1002 */  { addItemToBuffer,             ITM_R_CARON,                 STD_R_CARON,                                   STD_R_CARON,                                   0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/* 1003 */  { addItemToBuffer,             ITM_U_OGONEK,                STD_U_OGONEK,                                  STD_U_OGONEK,                                  0,       0,       CAT_AINT, SLS_UNCHANGED, US_UNCHANGED},
/* 1004 */  { addItemToBuffer,             ITM_u_OGONEK,                STD_u_OGONEK,                                  STD_u_OGONEK,                                  0,       0,       CAT_aint, SLS_UNCHANGED, US_UNCHANGED},
/* 1005 */  { addItemToBuffer,             ITM_y_UNDER_ROOT,            "",                                            STD_y_UNDER_ROOT,                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1006 */  { addItemToBuffer,             ITM_x_UNDER_ROOT,            "",                                            STD_x_UNDER_ROOT,                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1007 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_EM,                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1008 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_3_PER_EM,                            0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1009 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_4_PER_EM,                            0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1010 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_6_PER_EM,                            0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1011 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_FIGURE,                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1012 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_PUNCTUATION,                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1013 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_HAIR,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1014 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LEFT_SINGLE_QUOTE,                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1015 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RIGHT_SINGLE_QUOTE,                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1016 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SINGLE_LOW_QUOTE,                          0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1017 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SINGLE_HIGH_QUOTE,                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1018 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LEFT_DOUBLE_QUOTE,                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1019 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RIGHT_DOUBLE_QUOTE,                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1020 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOUBLE_LOW_QUOTE,                          0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1021 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOUBLE_HIGH_QUOTE,                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1022 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ELLIPSIS,                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1023 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ONE,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1024 */  { addItemToBuffer,             ITM_EURO,                    "",                                            STD_EURO,                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1025 */  { addItemToBuffer,             ITM_COMPLEX_C,               "",                                            STD_COMPLEX_C,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1026 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PLANCK,                                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1027 */  { addItemToBuffer,             ITM_PLANCK_2PI,              "",                                            STD_PLANCK_2PI,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1028 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NATURAL_N,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1029 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RATIONAL_Q,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1030 */  { addItemToBuffer,             ITM_REAL_R,                  "",                                            STD_REAL_R,                                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1031 */  { addItemToBuffer,             ITM_LEFT_ARROW,              "",                                            STD_LEFT_ARROW,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1032 */  { addItemToBuffer,             ITM_UP_ARROW,                "",                                            STD_UP_ARROW,                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1033 */  { itemToBeCoded,               NOPARAM,                     "1033",                                        "1033",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1034 */  { addItemToBuffer,             ITM_RIGHT_ARROW,             "",                                            STD_RIGHT_ARROW,                               0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1035 */  { addItemToBuffer,             ITM_DOWN_ARROW,              "",                                            STD_DOWN_ARROW,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1036 */  { itemToBeCoded,               NOPARAM,                     "1036",                                        "1036",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1037 */  { addItemToBuffer,             ITM_SERIAL_IO,               "",                                            STD_SERIAL_IO,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1038 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RIGHT_SHORT_ARROW,                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1039 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LEFT_RIGHT_ARROWS,                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1040 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_BST,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1041 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SST,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1042 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_HAMBURGER,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1043 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_UNDO,                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1044 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_FOR_ALL,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1045 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_COMPLEMENT,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1046 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PARTIAL_DIFF,                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1047 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_THERE_EXISTS,                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1048 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_THERE_DOES_NOT_EXIST,                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1049 */  { addItemToBuffer,             ITM_EMPTY_SET,               "",                                            STD_EMPTY_SET,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1050 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_INCREMENT,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1051 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NABLA,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1052 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ELEMENT_OF,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1053 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NOT_ELEMENT_OF,                            0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1054 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_CONTAINS,                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1055 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOES_NOT_CONTAIN,                          0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1056 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ZERO,                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1057 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PRODUCT,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1058 */  { itemToBeCoded,               NOPARAM,                     "1058",                                        "1058",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1059 */  { itemToBeCoded,               NOPARAM,                     "1059",                                        "1059",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1060 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MINUS_PLUS,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1061 */  { itemToBeCoded,               NOPARAM,                     "1061",                                        "1061",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1062 */  { itemToBeCoded,               NOPARAM,                     "1062",                                        "1062",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1063 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RING,                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1064 */  { addItemToBuffer,             ITM_BULLET,                  "",                                            STD_BULLET,                                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1065 */  { addItemToBuffer,             ITM_SQUARE_ROOT,             "",                                            STD_SQUARE_ROOT,                               0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1066 */  { addItemToBuffer,             ITM_CUBE_ROOT,               "",                                            STD_CUBE_ROOT,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1067 */  { addItemToBuffer,             ITM_xTH_ROOT,                "",                                            STD_xTH_ROOT,                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1068 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PROPORTIONAL,                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1069 */  { addItemToBuffer,             ITM_INFINITY,                "",                                            STD_INFINITY,                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1070 */  { addItemToBuffer,             ITM_RIGHT_ANGLE,             "",                                            STD_RIGHT_ANGLE,                               0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1071 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ANGLE,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1072 */  { addItemToBuffer,             ITM_MEASURED_ANGLE,          "",                                            STD_MEASURED_ANGLE,                            0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1073 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DIVIDES,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1074 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOES_NOT_DIVIDE,                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1075 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PARALLEL,                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1076 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NOT_PARALLEL,                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1077 */  { addItemToBuffer,             ITM_AND,                     "",                                            STD_AND,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1078 */  { addItemToBuffer,             ITM_OR,                      "",                                            STD_OR,                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1079 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_INTERSECTION,                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1080 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_UNION,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1081 */  { addItemToBuffer,             ITM_INTEGRAL_SIGN,                "",                                            STD_INTEGRAL,                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1082 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOUBLE_INTEGRAL,                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1083 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_CONTOUR_INTEGRAL,                          0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1084 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SURFACE_INTEGRAL,                          0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1085 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RATIO,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1086 */  { addItemToBuffer,             ITM_CHECK_MARK,              "",                                            STD_CHECK_MARK,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1087 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ASYMPOTICALLY_EQUAL,                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1088 */  { addItemToBuffer,             ITM_ALMOST_EQUAL,            "",                                            STD_ALMOST_EQUAL,                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1089 */  { addItemToBuffer,             ITM_COLON_EQUALS,            "",                                            STD_COLON_EQUALS,                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1090 */  { addItemToBuffer,             ITM_CORRESPONDS_TO,          "",                                            STD_CORRESPONDS_TO,                            0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1091 */  { addItemToBuffer,             ITM_ESTIMATES,               "",                                            STD_ESTIMATES,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1092 */  { addItemToBuffer,             ITM_NOT_EQUAL,               "",                                            STD_NOT_EQUAL,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1093 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_IDENTICAL_TO,                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1094 */  { addItemToBuffer,             ITM_LESS_EQUAL,              "",                                            STD_LESS_EQUAL,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1095 */  { addItemToBuffer,             ITM_GREATER_EQUAL,           "",                                            STD_GREATER_EQUAL,                             0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1096 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MUCH_LESS,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1097 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MUCH_GREATER,                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1098 */  { addItemToBuffer,             ITM_SUN,                     "",                                            STD_SUN,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1099 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOWN_TACK,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1100 */  { addItemToBuffer,             ITM_PERPENDICULAR,           "",                                            STD_PERPENDICULAR,                             0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1101 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_XOR,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1102 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NAND,                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1103 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NOR,                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1104 */  { addItemToBuffer,             ITM_WATCH,                   "",                                            STD_WATCH,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1105 */  { addItemToBuffer,             ITM_HOURGLASS,               "",                                            STD_HOURGLASS,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1106 */  { addItemToBuffer,             ITM_PRINTER,                 "",                                            STD_PRINTER,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1107 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_TL,                                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1108 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_ML,                                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1109 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_BL,                                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1110 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_TR,                                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1111 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_MR,                                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1112 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_BR,                                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1113 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OBLIQUE1,                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1114 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OBLIQUE2,                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1115 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OBLIQUE3,                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1116 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OBLIQUE4,                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1117 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_CURSOR,                                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1118 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PERIOD34,                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1119 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_COMMA34,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1120 */  { addItemToBuffer,             ITM_BATTERY,                 "",                                            STD_BATTERY,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1121 */  { addItemToBuffer,             ITM_PGM_BEGIN,               "",                                            STD_PGM_BEGIN,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1122 */  { addItemToBuffer,             ITM_USER_MODE,               "",                                            STD_USER_MODE,                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1123 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_UK,                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1124 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_US,                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1125 */  { addItemToBuffer,             ITM_NEG_EXCLAMATION_MARK,    "",                                            STD_NEG_EXCLAMATION_MARK,                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1126 */  { addItemToBuffer,             ITM_ex,                      "",                                            STD_LEFT_RIGHT_ARROWS,                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1127 */  { addItemToBuffer,             ITM_Max,                     "",                                            "Max",                                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1128 */  { addItemToBuffer,             ITM_Min,                     "",                                            "Min",                                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1129 */  { addItemToBuffer,             ITM_Config,                  "",                                            "Config",                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1130 */  { addItemToBuffer,             ITM_Stack,                   "",                                            "Stack",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1131 */  { addItemToBuffer,             ITM_dddEL,                   "",                                            STD_ELLIPSIS "EL",                             0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1132 */  { addItemToBuffer,             ITM_dddIJ,                   "",                                            STD_ELLIPSIS "IJ",                             0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1133 */  { itemToBeCoded,               NOPARAM,                     "",                                            "0.",                                          0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1134 */  { itemToBeCoded,               NOPARAM,                     "",                                            "1.",                                          0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1135 */  { addItemToBuffer,             ITM_EXPONENT     /*# JM #*/, "",                                            "EEX",                                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM Change E to EEX
/* 1136 */  { addItemToBuffer,             ITM_MA11,                    "",                                            STD_a_DIARESIS,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1137 */  { addItemToBuffer,             ITM_MA12,                    "",                                            STD_o_DIARESIS,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1138 */  { addItemToBuffer,             ITM_MA13,                    "",                                            STD_u_DIARESIS,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1139 */  { addItemToBuffer,             ITM_MA14,                    "",                                            "",                                            0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1140 */  { addItemToBuffer,             ITM_MA15,                    "",                                            "sch",                                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1141 */  { addItemToBuffer,             ITM_MA16,                    "",                                            STD_s_SHARP,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1142 */  { addItemToBuffer,             ITM_MA21,                    "",                                            "not",                                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1143 */  { addItemToBuffer,             ITM_MA22,                    "",                                            "edi-",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1144 */  { addItemToBuffer,             ITM_MA23,                    "",                                            "table",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1145 */  { addItemToBuffer,             ITM_MA24,                    "",                                            "for",                                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1146 */  { addItemToBuffer,             ITM_MA25,                    "",                                            "now",                                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1147 */  { addItemToBuffer,             ITM_MA26,                    "",                                            "",                                            0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1148 */  { addItemToBuffer,             ITM_MA31,                    "",                                            "",                                            0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1149 */  { addItemToBuffer,             ITM_MA32,                    "",                                            "",                                            0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1150 */  { addItemToBuffer,             ITM_MA33,                    "",                                            "",                                            0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1151 */  { addItemToBuffer,             ITM_MA34,                    "",                                            "",                                            0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1152 */  { addItemToBuffer,             ITM_MA35,                    "",                                            "",                                            0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1153 */  { addItemToBuffer,             ITM_MA36,                    "",                                            "",                                            0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1154 */  { addItemToBuffer,             NOPARAM,                     "HEX",                                         "H",                                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1155 */  { itemToBeCoded,               NOPARAM,                     "1155",                                        "1155",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1156 */  { itemToBeCoded,               NOPARAM,                     "1156",                                        "1156",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1157 */  { itemToBeCoded,               NOPARAM,                     "1157",                                        "1157",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1158 */  { itemToBeCoded,               NOPARAM,                     "1158",                                        "1158",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1159 */  { itemToBeCoded,               NOPARAM,                     "1159",                                        "1159",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1160 */  { itemToBeCoded,               NOPARAM,                     "1160",                                        "1160",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1161 */  { itemToBeCoded,               NOPARAM,                     "1161",                                        "1161",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1162 */  { itemToBeCoded,               NOPARAM,                     "1162",                                        "1162",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1163 */  { itemToBeCoded,               NOPARAM,                     "1163",                                        "1163",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1164 */  { itemToBeCoded,               NOPARAM,                     "1164",                                        "1164",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},


// Reserved variables
/* 1165 */  { addItemToBuffer,             REGISTER_I,                  "I",                                           "I",                                           0,       0,       CAT_RVAR, SLS_UNCHANGED, US_UNCHANGED},
/* 1166 */  { addItemToBuffer,             REGISTER_J,                  "J",                                           "J",                                           0,       0,       CAT_RVAR, SLS_UNCHANGED, US_UNCHANGED},
/* 1167 */  { addItemToBuffer,             REGISTER_K,                  "K",                                           "K",                                           0,       0,       CAT_RVAR, SLS_UNCHANGED, US_UNCHANGED},
/* 1168 */  { addItemToBuffer,             REGISTER_L,                  "L",                                           "L",                                           0,       0,       CAT_RVAR, SLS_UNCHANGED, US_UNCHANGED},
/* 1169 */  { itemToBeCoded,               NOPARAM,                     "A",                                           "A",                                           0,       0,       CAT_RVAR, SLS_UNCHANGED, US_UNCHANGED},
/* 1170 */  { itemToBeCoded,               NOPARAM,                     "ACC",                                         "ACC",                                         0,       0,       CAT_RVAR, SLS_UNCHANGED, US_UNCHANGED},
/* 1171 */  { itemToBeCoded,               NOPARAM,                     "B",                                           "B",                                           0,       0,       CAT_RVAR, SLS_UNCHANGED, US_UNCHANGED},
/* 1172 */  { itemToBeCoded,               NOPARAM,                     "C",                                           "C",                                           0,       0,       CAT_RVAR, SLS_UNCHANGED, US_UNCHANGED},
/* 1173 */  { itemToBeCoded,               NOPARAM,                     "D",                                           "D",                                           0,       0,       CAT_RVAR, SLS_UNCHANGED, US_UNCHANGED},
/* 1174 */  { itemToBeCoded,               NOPARAM,                     "FV",                                          "FV",                                          0,       0,       CAT_RVAR, SLS_UNCHANGED, US_UNCHANGED},
/* 1175 */  { itemToBeCoded,               NOPARAM,                     "i%/a",                                        "i%/a",                                        0,       0,       CAT_RVAR, SLS_UNCHANGED, US_UNCHANGED},
/* 1176 */  { itemToBeCoded,               NOPARAM,                     "Mat_A",                                       "Mat A",                                       0,       0,       CAT_RVAR, SLS_UNCHANGED, US_UNCHANGED},
/* 1177 */  { itemToBeCoded,               NOPARAM,                     "Mat_B",                                       "Mat B",                                       0,       0,       CAT_RVAR, SLS_UNCHANGED, US_UNCHANGED},
/* 1178 */  { itemToBeCoded,               NOPARAM,                     "NPER",                                        "n" STD_SUB_P STD_SUB_E STD_SUB_R,             0,       0,       CAT_RVAR, SLS_UNCHANGED, US_UNCHANGED},
/* 1179 */  { itemToBeCoded,               NOPARAM,                     "PER/a",                                       "per/a",                                       0,       0,       CAT_RVAR, SLS_UNCHANGED, US_UNCHANGED},
/* 1180 */  { itemToBeCoded,               NOPARAM,                     "PMT",                                         "PMT",                                         0,       0,       CAT_RVAR, SLS_UNCHANGED, US_UNCHANGED},
/* 1181 */  { itemToBeCoded,               NOPARAM,                     "PV",                                          "PV",                                          0,       0,       CAT_RVAR, SLS_UNCHANGED, US_UNCHANGED},
/* 1182 */  { itemToBeCoded,               NOPARAM,                     "REGS",                                        "REGS",                                        0,       0,       CAT_RVAR, SLS_UNCHANGED, US_UNCHANGED},
/* 1183 */  { itemToBeCoded,               NOPARAM,                     STD_UP_ARROW "Lim",                            STD_UP_ARROW "Lim",                            0,       0,       CAT_RVAR, SLS_UNCHANGED, US_UNCHANGED},
/* 1184 */  { itemToBeCoded,               NOPARAM,                     STD_DOWN_ARROW "Lim",                          STD_DOWN_ARROW "Lim",                          0,       0,       CAT_RVAR, SLS_UNCHANGED, US_UNCHANGED},
/* 1185 */  { itemToBeCoded,               NOPARAM,                     "1185",                                        "1185",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1186 */  { itemToBeCoded,               NOPARAM,                     "1186",                                        "1186",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1187 */  { itemToBeCoded,               NOPARAM,                     "1187",                                        "1187",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1188 */  { itemToBeCoded,               NOPARAM,                     "1188",                                        "1188",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1189 */  { itemToBeCoded,               NOPARAM,                     "1189",                                        "1189",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1190 */  { itemToBeCoded,               NOPARAM,                     "1190",                                        "1190",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1191 */  { itemToBeCoded,               NOPARAM,                     "1191",                                        "1191",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1192 */  { itemToBeCoded,               NOPARAM,                     "1192",                                        "1192",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1193 */  { itemToBeCoded,               NOPARAM,                     "1193",                                        "1193",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1194 */  { itemToBeCoded,               NOPARAM,                     "1194",                                        "1194",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1195 */  { itemToBeCoded,               NOPARAM,                     "1195",                                        "1195",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1196 */  { itemToBeCoded,               NOPARAM,                     "1196",                                        "1196",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},


// Probability distributions
/* 1197 */  { itemToBeCoded,               NOPARAM,                     "Binom:",                                      "Binom:",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1198 */  { itemToBeCoded,               NOPARAM,                     "Binom" STD_SUB_p,                             "Binom" STD_SUB_p,                             0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1199 */  { itemToBeCoded,               NOPARAM,                     "Binom" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Binom" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1200 */  { itemToBeCoded,               NOPARAM,                     "Binom" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Binom" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1201 */  { itemToBeCoded,               NOPARAM,                     "Binom" STD_SUP_MINUS_1,                       "Binom" STD_SUP_MINUS_1,                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1202 */  { itemToBeCoded,               NOPARAM,                     "Cauch:",                                      "Cauch:",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1203 */  { itemToBeCoded,               NOPARAM,                     "Cauch" STD_SUB_p,                             "Cauch" STD_SUB_p,                             0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1204 */  { itemToBeCoded,               NOPARAM,                     "Cauch" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Cauch" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1205 */  { itemToBeCoded,               NOPARAM,                     "Cauch" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Cauch" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1206 */  { itemToBeCoded,               NOPARAM,                     "Cauch" STD_SUP_MINUS_1,                       "Cauch" STD_SUP_MINUS_1,                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1207 */  { itemToBeCoded,               NOPARAM,                     "Expon:",                                      "Expon:",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1208 */  { itemToBeCoded,               NOPARAM,                     "Expon" STD_SUB_p,                             "Expon" STD_SUB_p,                             0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1209 */  { itemToBeCoded,               NOPARAM,                     "Expon" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Expon" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1210 */  { itemToBeCoded,               NOPARAM,                     "Expon" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Expon" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1211 */  { itemToBeCoded,               NOPARAM,                     "Expon" STD_SUP_MINUS_1,                       "Expon" STD_SUP_MINUS_1,                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1212 */  { itemToBeCoded,               NOPARAM,                     "F:",                                          "F:",                                          0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1213 */  { itemToBeCoded,               NOPARAM,                     "F" STD_SUB_p "(x)",                           "F" STD_SUB_p "(x)",                           0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1214 */  { itemToBeCoded,               NOPARAM,                     "F" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", "F" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)",   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1215 */  { itemToBeCoded,               NOPARAM,                     "F" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", "F" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)",   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1216 */  { itemToBeCoded,               NOPARAM,                     "F" STD_SUP_MINUS_1 "(p)",                     "F" STD_SUP_MINUS_1 "(p)",                     0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1217 */  { itemToBeCoded,               NOPARAM,                     "Geom:",                                       "Geom:",                                       0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1218 */  { itemToBeCoded,               NOPARAM,                     "Geom" STD_SUB_p,                              "Geom" STD_SUB_p,                              0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1219 */  { itemToBeCoded,               NOPARAM,                     "Geom" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,    "Geom" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,    0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1220 */  { itemToBeCoded,               NOPARAM,                     "Geom" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,    "Geom" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,    0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1221 */  { itemToBeCoded,               NOPARAM,                     "Geom" STD_SUP_MINUS_1,                        "Geom" STD_SUP_MINUS_1,                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1222 */  { itemToBeCoded,               NOPARAM,                     "Hyper:",                                      "Hyper:",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1223 */  { itemToBeCoded,               NOPARAM,                     "Hyper" STD_SUB_p,                             "Hyper" STD_SUB_p,                             0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1224 */  { itemToBeCoded,               NOPARAM,                     "Hyper" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Hyper" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1225 */  { itemToBeCoded,               NOPARAM,                     "Hyper" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Hyper" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1226 */  { itemToBeCoded,               NOPARAM,                     "Hyper" STD_SUP_MINUS_1,                       "Hyper" STD_SUP_MINUS_1,                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1227 */  { itemToBeCoded,               NOPARAM,                     "LgNrm:",                                      "LgNrm:",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1228 */  { itemToBeCoded,               NOPARAM,                     "LgNrm" STD_SUB_p,                             "LgNrm" STD_SUB_p,                             0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1229 */  { itemToBeCoded,               NOPARAM,                     "LgNrm" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "LgNrm" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1230 */  { itemToBeCoded,               NOPARAM,                     "LgNrm" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "LgNrm" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1231 */  { itemToBeCoded,               NOPARAM,                     "LgNrm" STD_SUP_MINUS_1,                       "LgNrm" STD_SUP_MINUS_1,                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1232 */  { itemToBeCoded,               NOPARAM,                     "Logis:",                                      "Logis:",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1233 */  { itemToBeCoded,               NOPARAM,                     "Logis" STD_SUB_p,                             "Logis" STD_SUB_p,                             0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1234 */  { itemToBeCoded,               NOPARAM,                     "Logis" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Logis" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1235 */  { itemToBeCoded,               NOPARAM,                     "Logis" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Logis" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1236 */  { itemToBeCoded,               NOPARAM,                     "Logis" STD_SUP_MINUS_1,                       "Logis" STD_SUP_MINUS_1,                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1237 */  { itemToBeCoded,               NOPARAM,                     "NBin:",                                       "NBin:",                                       0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1238 */  { itemToBeCoded,               NOPARAM,                     "NBin" STD_SUB_p,                              "NBin" STD_SUB_p,                              0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1239 */  { itemToBeCoded,               NOPARAM,                     "NBin" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,    "NBin" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,    0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1240 */  { itemToBeCoded,               NOPARAM,                     "NBin" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,    "NBin" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,    0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1241 */  { itemToBeCoded,               NOPARAM,                     "NBin" STD_SUP_MINUS_1,                        "NBin" STD_SUP_MINUS_1,                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1242 */  { itemToBeCoded,               NOPARAM,                     "Norml:",                                      "Norml:",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1243 */  { itemToBeCoded,               NOPARAM,                     "Norml" STD_SUB_p,                             "Norml" STD_SUB_p,                             0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1244 */  { itemToBeCoded,               NOPARAM,                     "Norml" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Norml" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1245 */  { itemToBeCoded,               NOPARAM,                     "Norml" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Norml" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1246 */  { itemToBeCoded,               NOPARAM,                     "Norml" STD_SUP_MINUS_1,                       "Norml" STD_SUP_MINUS_1,                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1247 */  { itemToBeCoded,               NOPARAM,                     "Poiss:",                                      "Poiss:",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1248 */  { itemToBeCoded,               NOPARAM,                     "Poiss" STD_SUB_p,                             "Poiss" STD_SUB_p,                             0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1249 */  { itemToBeCoded,               NOPARAM,                     "Poiss" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Poiss" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1250 */  { itemToBeCoded,               NOPARAM,                     "Poiss" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Poiss" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1251 */  { itemToBeCoded,               NOPARAM,                     "Poiss" STD_SUP_MINUS_1,                       "Poiss" STD_SUP_MINUS_1,                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1252 */  { itemToBeCoded,               NOPARAM,                     "t:",                                          "t:",                                          0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1253 */  { itemToBeCoded,               NOPARAM,                     "t" STD_SUB_p "(x)",                           "t" STD_SUB_p "(x)",                           0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1254 */  { itemToBeCoded,               NOPARAM,                     "t" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", "t" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)",   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1255 */  { itemToBeCoded,               NOPARAM,                     "t" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", "t" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)",   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1256 */  { itemToBeCoded,               NOPARAM,                     "t" STD_SUP_MINUS_1 "(p)",                     "t" STD_SUP_MINUS_1 "(p)",                     0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1257 */  { itemToBeCoded,               NOPARAM,                     "Weibl:",                                      "Weibl:",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1258 */  { itemToBeCoded,               NOPARAM,                     "Weibl" STD_SUB_p,                             "Weibl" STD_SUB_p,                             0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1259 */  { itemToBeCoded,               NOPARAM,                     "Weibl" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Weibl" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1260 */  { itemToBeCoded,               NOPARAM,                     "Weibl" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Weibl" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1261 */  { itemToBeCoded,               NOPARAM,                     "Weibl" STD_SUP_MINUS_1,                       "Weibl" STD_SUP_MINUS_1,                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1262 */  { itemToBeCoded,               NOPARAM,                     STD_chi STD_SUP_2 ":",                         STD_chi STD_SUP_2 ":",                         0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1263 */  { itemToBeCoded,               NOPARAM,                     STD_chi STD_SUP_2  STD_SUB_p "(x)",            STD_chi STD_SUP_2 STD_SUB_p "(x)",             0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1264 */  { itemToBeCoded,               NOPARAM,                     STD_chi STD_SUP_2 STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", STD_chi STD_SUP_2 STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)",   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1265 */  { itemToBeCoded,               NOPARAM,                     STD_chi STD_SUP_2 STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", STD_chi STD_SUP_2 STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)",   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1266 */  { itemToBeCoded,               NOPARAM,                     "(" STD_chi STD_SUP_2 ")" STD_SUP_MINUS_1,     "(" STD_chi STD_SUP_2 ")" STD_SUP_MINUS_1,     0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1267 */  { itemToBeCoded,               NOPARAM,                     "1267",                                        "1267",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1268 */  { itemToBeCoded,               NOPARAM,                     "1268",                                        "1268",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1269 */  { itemToBeCoded,               NOPARAM,                     "1269",                                        "1269",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1270 */  { itemToBeCoded,               NOPARAM,                     "1270",                                        "1270",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1271 */  { itemToBeCoded,               NOPARAM,                     "1271",                                        "1271",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1272 */  { itemToBeCoded,               NOPARAM,                     "1272",                                        "1272",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1273 */  { itemToBeCoded,               NOPARAM,                     "1273",                                        "1273",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1274 */  { itemToBeCoded,               NOPARAM,                     "1274",                                        "1274",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1275 */  { itemToBeCoded,               NOPARAM,                     "1275",                                        "1275",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1276 */  { itemToBeCoded,               NOPARAM,                     "1276",                                        "1276",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1277 */  { itemToBeCoded,               NOPARAM,                     "1277",                                        "1277",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1278 */  { itemToBeCoded,               NOPARAM,                     "1278",                                        "1278",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1279 */  { itemToBeCoded,               NOPARAM,                     "1279",                                        "1279",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1280 */  { itemToBeCoded,               NOPARAM,                     "1280",                                        "1280",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1281 */  { itemToBeCoded,               NOPARAM,                     "1281",                                        "1281",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1282 */  { itemToBeCoded,               NOPARAM,                     "1282",                                        "1282",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1283 */  { itemToBeCoded,               NOPARAM,                     "1283",                                        "1283",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1284 */  { itemToBeCoded,               NOPARAM,                     "1284",                                        "1284",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1285 */  { itemToBeCoded,               NOPARAM,                     "1285",                                        "1285",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1286 */  { itemToBeCoded,               NOPARAM,                     "1286",                                        "1286",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},


// Curve fitting
/* 1287 */  { fnCurveFitting,              TM_VALUE,                    "BestF",                                       "BestF",                                       1,     1023,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1288 */  { fnCurveFitting,              CF_EXPONENTIAL_FITTING,      "ExpF",                                        "ExpF",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1289 */  { fnCurveFitting,              CF_LINEAR_FITTING,           "LinF",                                        "LinF",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1290 */  { fnCurveFitting,              CF_LOGARITHMIC_FITTING,      "LogF",                                        "LogF",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1291 */  { fnCurveFitting,              CF_ORTHOGONAL_FITTING,       "OrthoF",                                      "OrthoF",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1292 */  { fnCurveFitting,              CF_POWER_FITTING,            "PowerF",                                      "PowerF",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1293 */  { fnCurveFitting,              CF_GAUSS_FITTING,            "GaussF",                                      "GaussF",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1294 */  { fnCurveFitting,              CF_CAUCHY_FITTING,           "CauchF",                                      "CauchF",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1295 */  { fnCurveFitting,              CF_PARABOLIC_FITTING,        "ParabF",                                      "ParabF",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1296 */  { fnCurveFitting,              CF_HYPERBOLIC_FITTING,       "HypF",                                        "HypF",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1297 */  { fnCurveFitting,              CF_ROOT_FITTING,             "RootF",                                       "RootF",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1298 */  { itemToBeCoded,               NOPARAM,                     "1298",                                        "1298",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1299 */  { itemToBeCoded,               NOPARAM,                     "1299",                                        "1299",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1300 */  { itemToBeCoded,               NOPARAM,                     "1300",                                        "1300",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1301 */  { itemToBeCoded,               NOPARAM,                     "1301",                                        "1301",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1302 */  { itemToBeCoded,               NOPARAM,                     "1302",                                        "1302",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},


// Menus
/* 1303 */  { itemToBeCoded,               NOPARAM,                     "ADV",                                         "ADV",                                         0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1304 */  { itemToBeCoded,               NOPARAM,                     "ANGLES",                                      "ANGLES",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1305 */  { itemToBeCoded,               NOPARAM     /*# JM #*/,      "PRINT",                                       STD_PRINTER,                                   0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1306 */  { itemToBeCoded,               NOPARAM     /*# JM #*/,      "Area:",                                       "Area:",                                       0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1307 */  { itemToBeCoded,               NOPARAM,                     "BITS",                                        "BITS",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1308 */  { itemToBeCoded,               NOPARAM     /*# JM #*/,      "CATALOG",                                     "CAT",                                         0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},// JM
/* 1309 */  { itemToBeCoded,               NOPARAM,                     "CHARS",                                       "CHARS",                                       0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1310 */  { itemToBeCoded,               NOPARAM,                     "CLK",                                         "CLK",                                         0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1311 */  { itemToBeCoded,               NOPARAM,                     "REGIST",                                      "REGIST",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1312 */  { itemToBeCoded,               NOPARAM,                     "CLR",                                         "CLR",                                         0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1313 */  { itemToBeCoded,               NOPARAM     /*# JM #*/,      "CNST",                                        "CNST",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},//JM Keeps the same. Don't havce space for more on kjeyplate
/* 1314 */  { itemToBeCoded,               NOPARAM,                     "CPX",                                         "CPX",                                         0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1315 */  { itemToBeCoded,               NOPARAM,                     "CPXS",                                        "CPXS",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1316 */  { itemToBeCoded,               NOPARAM,                     "DATES",                                       "DATES",                                       0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1317 */  { itemToBeCoded,               NOPARAM,                     "DIGITS",                                      "DIGITS",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1318 */  { itemToBeCoded,               NOPARAM,                     "DISP",                                        "DISP",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1319 */  { itemToBeCoded,               NOPARAM,                     "EQN",                                         "EQN",                                         0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1320 */  { itemToBeCoded,               NOPARAM,                     "EXP",                                         "EXP",                                         0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1321 */  { itemToBeCoded,               NOPARAM     /*# JM #*/,      "Energy:",                                     "Energy:",                                     0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1322 */  { itemToBeCoded,               NOPARAM,                     "FCNS",                                        "FCNS",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1323 */  { itemToBeCoded,               NOPARAM,                     "FIN",                                         "FIN",                                         0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1324 */  { itemToBeCoded,               NOPARAM,                     "S.INTS",                                      "S.INTS",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1325 */  { itemToBeCoded,               NOPARAM,                     "FLAGS",                                       "FLAGS",                                       0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1326 */  { itemToBeCoded,               NOPARAM,                     "FLASH",                                       "FLASH",                                       0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1327 */  { itemToBeCoded,               NOPARAM,                     "f'",                                          "f'",                                          0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1328 */  { itemToBeCoded,               NOPARAM,                     "f\"",                                         "f\"",                                         0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1329 */  { itemToBeCoded,               NOPARAM,                     "F&p:",                                        "F&p:",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1330 */  { itemToBeCoded,               NOPARAM,                     "L.INTS",                                      "L.INTS",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1331 */  { itemToBeCoded,               NOPARAM,                     "INFO",                                        "INFO",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1332 */  { itemToBeCoded,               NOPARAM,                     "INTS",                                        "INTS",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1333 */  { itemToBeCoded,               NOPARAM,                     "I/O",                                         "I/O",                                         0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1334 */  { itemToBeCoded,               NOPARAM,                     "LOOP",                                        "LOOP",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1335 */  { itemToBeCoded,               NOPARAM,                     "MATRS",                                       "MATRS",                                       0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1336 */  { itemToBeCoded,               NOPARAM,                     "MATX",                                        "MATX",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1337 */  { itemToBeCoded,               NOPARAM,                     "MENUS",                                       "MENUS",                                       0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1338 */  { itemToBeCoded,               NOPARAM,                     "MODE",                                        "MODE",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1339 */  { itemToBeCoded,               NOPARAM,                     "M.SIMQ",                                      "M.SIMQ",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1340 */  { itemToBeCoded,               NOPARAM,                     "M.EDIT",                                      "M.EDIT",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1341 */  { itemToBeCoded,               NOPARAM     /*# JM #*/,      "MyMenu",                                      "MyM",                                         0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1342 */  { itemToBeCoded,               NOPARAM,                     "My" STD_alpha,                                "My" STD_alpha,                                0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1343 */  { itemToBeCoded,               NOPARAM     /*# JM #*/,      "Mass:",                                       "Mass:",                                       0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1344 */  { itemToBeCoded,               NOPARAM,                     "ORTHOG",                                      "Orthog",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1345 */  { itemToBeCoded,               NOPARAM,                     "PARTS",                                       "PARTS",                                       0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1346 */  { itemToBeCoded,               NOPARAM,                     "PROB",                                        "PROB",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1347 */  { itemToBeCoded,               NOPARAM,                     "PROGS",                                       "PROGS",                                       0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1348 */  { itemToBeCoded,               NOPARAM,                     "P.FN",                                        "P.FN",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1349 */  { itemToBeCoded,               NOPARAM,                     "P.FN2",                                       "P.FN2",                                       0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1350 */  { itemToBeCoded,               NOPARAM     /*# JM #*/,      "Power:",                                      "Power:",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1351 */  { itemToBeCoded,               NOPARAM,                     "RAM",                                         "RAM",                                         0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1352 */  { itemToBeCoded,               NOPARAM,                     "REALS",                                       "REALS",                                       0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1353 */  { itemToBeCoded,               NOPARAM,                     "Solver",                                      "Solver",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1354 */  { itemToBeCoded,               NOPARAM,                     "STAT",                                        "STAT",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1355 */  { itemToBeCoded,               NOPARAM,                     "STK",                                         "STK",                                         0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1356 */  { itemToBeCoded,               NOPARAM,                     "STRING",                                      "STRING",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1357 */  { itemToBeCoded,               NOPARAM,                     "TEST",                                        "TEST",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1358 */  { itemToBeCoded,               NOPARAM,                     "TIMES",                                       "TIMES",                                       0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1359 */  { itemToBeCoded,               NOPARAM     /*# JM #*/,      "TRI",                                         "TRIG",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},//JM
/* 1360 */  { itemToBeCoded,               NOPARAM,                     "TVM",                                         "TVM",                                         0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1361 */  { itemToBeCoded,               NOPARAM     /*# JM #*/,      "UNIT",                                        "UNIT",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},//JM Change U> arrow to CONV. Changed again to UNIT
/* 1362 */  { itemToBeCoded,               NOPARAM,                     "VARS",                                        "VARS",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1363 */  { itemToBeCoded,               NOPARAM     /*# JM #*/,      "Volume:",                                     "Volume:",                                     0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1364 */  { itemToBeCoded,               NOPARAM,                     "X.FN",                                        "X.FN",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1365 */  { itemToBeCoded,               NOPARAM     /*# JM #*/,      "Dist:",                                       "Dist:",                                       0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1366 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "INTL",                              STD_alpha "INTL",                              0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1367 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "MATH",                              STD_alpha "MATH",                              0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1368 */  { itemToBeCoded,               NOPARAM     /*# JM #*/,      STD_alpha "STR",                               STD_alpha "STR",                               0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},//JM Changed a.FN to STRNG
/* 1369 */  { itemToBeCoded,               NOPARAM,                     STD_ALPHA STD_ELLIPSIS STD_OMEGA,              STD_ALPHA STD_ELLIPSIS STD_OMEGA,              0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1370 */  { itemToBeCoded,               NOPARAM,                     STD_alpha STD_BULLET,                          STD_alpha STD_BULLET,                          0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1371 */  { itemToBeCoded,               NOPARAM,                     "SYS.FL",                                      "SYS.FL",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1372 */  { itemToBeCoded,               NOPARAM,                     STD_INTEGRAL "f",                              STD_INTEGRAL "f",                              0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1373 */  { itemToBeCoded,               NOPARAM,                     STD_INTEGRAL "fdx",                            STD_INTEGRAL "fdx",                            0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1374 */  { itemToBeCoded,               NOPARAM     /*# JM #*/,      "CONV",                                        "CONV",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},//JM Change to text DRG and change again to CONV
/* 1375 */  { itemToBeCoded,               NOPARAM,                     STD_alpha STD_ELLIPSIS STD_omega,              STD_alpha STD_ELLIPSIS STD_omega,              0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED}, // Small greek letters
/* 1376 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "intl",                              STD_alpha "intl",                              0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED}, // Small intl letters
/* 1377 */  { itemToBeCoded,               NOPARAM,                     "",                                            "Tam",                                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1378 */  { itemToBeCoded,               NOPARAM,                     "",                                            "TamCmp",                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1379 */  { itemToBeCoded,               NOPARAM,                     "",                                            "TamStoRcl",                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1380 */  { itemToBeCoded,               NOPARAM     /*# JM #*/,      "SUMS",                                        "SUMS",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1381 */  { itemToBeCoded,               NOPARAM,                     "VAR",                                         "VAR",                                         0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1382 */  { itemToBeCoded,               NOPARAM,                     "",                                            "TamFlag",                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1383 */  { itemToBeCoded,               NOPARAM,                     "",                                            "TamShuffle",                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1384 */  { itemToBeCoded,               NOPARAM,                     "1384",                                        "1384",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1385 */  { itemToBeCoded,               NOPARAM,                     "1385",                                        "1385",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1386 */  { itemToBeCoded,               NOPARAM,                     "1386",                                        "1386",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1387 */  { itemToBeCoded,               NOPARAM,                     "1387",                                        "1387",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1388 */  { itemToBeCoded,               NOPARAM,                     "1388",                                        "1388",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1389 */  { itemToBeCoded,               NOPARAM,                     "1389",                                        "1389",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1390 */  { itemToBeCoded,               NOPARAM,                     "1390",                                        "1390",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1391 */  { itemToBeCoded,               NOPARAM,                     "1391",                                        "1391",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1392 */  { itemToBeCoded,               NOPARAM,                     "1392",                                        "1392",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1393 */  { itemToBeCoded,               NOPARAM,                     "1393",                                        "1393",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},


// ????
/* 1394 */  { fnIntegerMode,               SIM_1COMPL,                  "1COMPL",                                      "1COMPL",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1395 */  { fnScreenDump,                NOPARAM,                     "SNAP",                                        "SNAP",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1396 */  { fnIntegerMode,               SIM_2COMPL,                  "2COMPL",                                      "2COMPL",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1397 */  { fnMagnitude,                 NOPARAM,                     "ABS",                                         "ABS",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1398 */  { fnAgm,                       NOPARAM,                     "AGM",                                         "AGM",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1399 */  { itemToBeCoded,               NOPARAM,                     "AGRAPH",                                      "AGRAPH",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1400 */  { fnDisplayFormatAll,          TM_VALUE,                    "ALL" ,                                        "ALL",                                         0,      15,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1401 */  { itemToBeCoded,               NOPARAM     /*# JM #*/,      "ASN",                                         "ASN",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1402 */  { itemToBeCoded,               NOPARAM,                     "BACK",                                        "BACK",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1403 */  { fnBatteryVoltage,            NOPARAM,                     "BATT?",                                       "BATT?",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1404 */  { itemToBeCoded,               NOPARAM,                     "BEEP",                                        "BEEP",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1405 */  { itemToBeCoded,               NOPARAM,                     "BeginP",                                      "Begin",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1406 */  { itemToBeCoded,               NOPARAM,                     "B" STD_SUB_n,                                 "B" STD_SUB_n,                                 0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1407 */  { itemToBeCoded,               NOPARAM,                     "B" STD_SUB_n STD_SUP_ASTERISK,                "B" STD_SUB_n STD_SUP_ASTERISK,                0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1408 */  { itemToBeCoded,               NOPARAM,                     "CASE",                                        "CASE",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1409 */  { fnClAll,                     NOT_CONFIRMED,               "CLALL",                                       "CLall",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1410 */  { itemToBeCoded,               NOPARAM,                     "CLCVAR",                                      "CLCVAR",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1411 */  { fnClFAll,                    NOPARAM,                     "CLFALL",                                      "CLFall",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1412 */  { fnFractionType,              NOPARAM,                     "a b/c",                                       "a b/c",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1413 */  { itemToBeCoded,               NOPARAM,                     "CLLCD",                                       "CLLCD",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1414 */  { itemToBeCoded,               NOPARAM,                     "CLMENU",                                      "CLMENU",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1415 */  { itemToBeCoded,               NOPARAM,                     "CLP",                                         "CLP",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1416 */  { fnClPAll,                    NOT_CONFIRMED,               "CLPALL",                                      "CLPall",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1417 */  { fnClearRegisters,            NOPARAM,                     "CLREGS",                                      "CLREGS",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1418 */  { fnClearStack,                NOPARAM,                     "CLSTK",                                       "CLSTK",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1419 */  { fnClSigma,                   NOPARAM,                     "CL" STD_SIGMA,                                "CL" STD_SIGMA,                                0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1420 */  { fnCyx,                       NOPARAM,                     "COMB",                                        "C" STD_SUB_y STD_SUB_x,                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1421 */  { fnConjugate,                 NOPARAM,                     "CONJ",                                        "conj",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1422 */  { itemToBeCoded,               NOPARAM,                     "CONVG?",                                      "CONVG?",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1423 */  { itemToBeCoded,               NOPARAM,                     "CORR",                                        "r",                                           0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1424 */  { itemToBeCoded,               NOPARAM,                     "COV",                                         "cov",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1425 */  { itemToBeCoded,               NOPARAM,                     "BestF?",                                      "BestF?",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1426 */  { fnCross,                     NOPARAM,                     "CROSS",                                       "cross",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1427 */  { fnCxToRe,                    NOPARAM,                     "CX" STD_RIGHT_ARROW "RE",                     "CX" STD_RIGHT_ARROW "RE",                     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1428 */  { itemToBeCoded,               NOPARAM,                     "DATE",                                        "DATE",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1429 */  { itemToBeCoded,               NOPARAM,                     "DATE" STD_RIGHT_ARROW,                        "DATE" STD_RIGHT_ARROW,                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1430 */  { itemToBeCoded,               NOPARAM,                     "DAY",                                         "DAY",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1431 */  { itemToBeCoded,               NOPARAM,                     "DBLR",                                        "DBLR",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1432 */  { itemToBeCoded,               NOPARAM,                     "DBL" STD_CROSS,                               "DBL" STD_CROSS,                               0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1433 */  { itemToBeCoded,               NOPARAM,                     "DBL/",                                        "DBL/",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1434 */  { fnDecomp,                    NOPARAM,                     "DECOMP",                                      "DECOMP",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1435 */  { fnAngularMode,               AM_DEGREE,                   "DEG",                                         "DEG",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1436 */  { fnCvtToCurrentAngularMode,   AM_DEGREE,                   "DEG" STD_RIGHT_ARROW,                         "DEG" STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1437 */  { itemToBeCoded,               NOPARAM,                     "s(a)",                                        "s(a)",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1438 */  { fnDenMax,                    NOPARAM,                     "DENMAX",                                      "DENMAX",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1439 */  { fnDot,                       NOPARAM,                     "DOT",                                         "dot",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1440 */  { fnDisplayStack,              TM_VALUE,                    "DSTACK",                                      "DSTACK",                                      1,       4,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1441 */  { fnAngularMode,               AM_DMS,                      "D.MS",                                        "d.ms",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1442 */  { fnCvtToCurrentAngularMode,   AM_DMS,                      "D.MS" STD_RIGHT_ARROW,                        "D.MS" STD_RIGHT_ARROW,                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1443 */  { fnSetDateFormat,             ITM_DMY,                     "D.MY",                                        "D.MY",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1444 */  { itemToBeCoded,               NOPARAM,                     "D" STD_RIGHT_ARROW "J",                       "D" STD_RIGHT_ARROW "J",                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1445 */  { fnCvtDegToRad,               NOPARAM,                     "D" STD_RIGHT_ARROW "R",                       "D" STD_RIGHT_ARROW "R",                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1446 */  { itemToBeCoded,               NOPARAM,                     "EIGVAL",                                      "EIGVAL",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1447 */  { itemToBeCoded,               NOPARAM,                     "EIGVEC",                                      "EIGVEC",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1448 */  { itemToBeCoded,               NOPARAM,                     "END",                                         "END",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1449 */  { itemToBeCoded,               NOPARAM,                     "ENDP",                                        "End",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1450 */  { fnDisplayFormatEng,          TM_VALUE,                    "ENG",                                         "ENG",                                         0,      15,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1451 */  { itemToBeCoded,               NOPARAM,                     "ENORM",                                       "ENORM",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1452 */  { itemToBeCoded,               NOPARAM,                     "ENTRY?",                                      "ENTRY?",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1453 */  { itemToBeCoded,               NOPARAM,                     "EQ.DEL",                                      "DELETE",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1454 */  { itemToBeCoded,               NOPARAM,                     "EQ.EDI",                                      "EDIT",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1455 */  { itemToBeCoded,               NOPARAM,                     "EQ.NEW",                                      "NEW",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1456 */  { itemToBeCoded,               NOPARAM,                     "erf",                                         "erf",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1457 */  { itemToBeCoded,               NOPARAM,                     "erfc",                                        "erfc",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1458 */  { itemToBeCoded,               NOPARAM,                     "ERR",                                         "ERR",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1459 */  { itemToBeCoded,               NOPARAM,                     "EXITALL",                                     "EXITall",                                     0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1460 */  { fnExpt,                      NOPARAM,                     "EXPT",                                        "EXPT",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1461 */  { itemToBeCoded,               NOPARAM,                     "PLOTMNU",                                     "PLOTMNU",                                     0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1462 */  { fnFib,                       NOPARAM,                     "FIB",                                         "FIB",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1463 */  { fnDisplayFormatFix,          TM_VALUE,                    "FIX",                                         "FIX",                                         0,      15,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1464 */  { fnFreeFlashMemory,           NOPARAM,                     "FLASH?",                                      "FLASH?",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1465 */  { itemToBeCoded,               NOPARAM,                     "f'(x)",                                       "f'(x)",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1466 */  { itemToBeCoded,               NOPARAM,                     "f\"(x)",                                      "f\"(x)",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1467 */  { fnDisplayFormatGap,          TM_VALUE,                    "GAP",                                         "GAP",                                         0,      15,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1468 */  { fnGd,                        NOPARAM,                     "g" STD_SUB_d,                                 "g" STD_SUB_d,                                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1469 */  { fnInvGd,                     NOPARAM,                     "g" STD_SUB_d STD_SUP_MINUS_1,                 "g" STD_SUB_d STD_SUP_MINUS_1,                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1470 */  { fnAngularMode,               AM_GRAD,                     "GRAD",                                        "GRAD",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1471 */  { fnCvtToCurrentAngularMode,   AM_GRAD,                     "GRAD" STD_RIGHT_ARROW,                        "GRAD" STD_RIGHT_ARROW,                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1472 */  { itemToBeCoded,               NOPARAM,                     "GTO.",                                        "GTO.",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1473 */  { itemToBeCoded,               NOPARAM,                     "H" STD_SUB_n,                                 "H" STD_SUB_n,                                 0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1474 */  { itemToBeCoded,               NOPARAM,                     "H" STD_SUB_n STD_SUB_P,                       "H" STD_SUB_n STD_SUB_P,                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1475 */  { fnImaginaryPart,             NOPARAM,                     "Im",                                          "Im",                                          0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1476 */  { itemToBeCoded,               NOPARAM,                     "INDEX",                                       "INDEX",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1477 */  { itemToBeCoded,               NOPARAM,                     "I" STD_SUB_x STD_SUB_y STD_SUB_z,             "I" STD_SUB_x STD_SUB_y STD_SUB_z,             0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1478 */  { itemToBeCoded,               NOPARAM,                     "I" STD_GAMMA STD_SUB_p,                       "I" STD_GAMMA STD_SUB_p,                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1479 */  { itemToBeCoded,               NOPARAM,                     "I" STD_GAMMA STD_SUB_q,                       "I" STD_GAMMA STD_SUB_q,                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1480 */  { itemToBeCoded,               NOPARAM,                     "I+",                                          "I+",                                          0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1481 */  { itemToBeCoded,               NOPARAM,                     "I-",                                          "I-",                                          0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1482 */  { itemToBeCoded,               NOPARAM,                     "J" STD_SUB_y "(x)",                           "J" STD_SUB_y "(x)",                           0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1483 */  { itemToBeCoded,               NOPARAM,                     "J+",                                          "J+",                                          0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1484 */  { itemToBeCoded,               NOPARAM,                     "J-",                                          "J-",                                          0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1485 */  { itemToBeCoded,               NOPARAM,                     "J/G",                                         "J/G",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1486 */  { itemToBeCoded,               NOPARAM,                     "J" STD_RIGHT_ARROW "D",                       "J" STD_RIGHT_ARROW "D",                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1487 */  { itemToBeCoded,               NOPARAM,                     "KEY",                                         "KEY",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1488 */  { itemToBeCoded,               NOPARAM,                     "KEYG",                                        "KEYG",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1489 */  { itemToBeCoded,               NOPARAM,                     "KEYX",                                        "KEYX",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1490 */  { itemToBeCoded,               NOPARAM,                     "KEY?",                                        "KEY?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1491 */  { itemToBeCoded,               NOPARAM,                     "KTYP?",                                       "KTYP?",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1492 */  { fnLastX,                     NOPARAM     /*# JM #*/,      "LASTx",                                       "LSTx",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1493 */  { itemToBeCoded,               NOPARAM,                     "LBL?",                                        "LBL?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1494 */  { itemToBeCoded,               NOPARAM,                     "LEAP?",                                       "LEAP?",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1495 */  { itemToBeCoded,               NOPARAM,                     "L" STD_SUB_m ,                                "L" STD_SUB_m ,                                0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1496 */  { itemToBeCoded,               NOPARAM,                     "L" STD_SUB_m STD_SUB_alpha,                   "L" STD_SUB_m STD_SUB_alpha,                   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1497 */  { fnLnBeta,                    NOPARAM,                     "LN" STD_beta,                                 "ln" STD_beta,                                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1498 */  { fnLnGamma,                   NOPARAM,                     "LN" STD_GAMMA,                                "ln" STD_GAMMA,                                0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1499 */  { fnLoad,                      LM_ALL,                      "LOAD",                                        "LOAD",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_CANCEL   },
/* 1500 */  { fnLoad,                      LM_PROGRAMS,                 "LOADP",                                       "LOADP",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1501 */  { fnLoad,                      LM_REGISTERS,                "LOADR",                                       "LOADR",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1502 */  { fnLoad,                      LM_SYSTEM_STATE,             "LOADSS",                                      "LOADSS",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1503 */  { fnLoad,                      LM_SUMS,                     "LOAD" STD_SIGMA,                              "LOAD" STD_SIGMA,                              0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1504 */  { allocateLocalRegisters,      TM_VALUE,                    "LocR",                                        "LocR",                                        0,      99,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1505 */  { fnGetLocR,                   NOPARAM,                     "LocR?",                                       "LocR?",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1506 */  { itemToBeCoded,               NOPARAM,                     "L.R.",                                        "L.R.",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1507 */  { fnMant,                      NOPARAM,                     "MANT",                                        "MANT",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1508 */  { itemToBeCoded,               NOPARAM,                     "Mat_X",                                       "Mat X",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1509 */  { fnFreeMemory,                NOPARAM,                     "MEM?",                                        "MEM?",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1510 */  { itemToBeCoded,               NOPARAM,                     "MENU",                                        "MENU",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1511 */  { itemToBeCoded,               NOPARAM,                     "MONTH",                                       "MONTH",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1512 */  { itemToBeCoded,               NOPARAM,                     "MSG",                                         "MSG",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1513 */  { fnAngularMode,               AM_MULTPI,                   "MUL" STD_pi,                                  "MUL" STD_pi,                                  0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1514 */  { itemToBeCoded,               NOPARAM,                     "MVAR",                                        "MVAR",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1515 */  { itemToBeCoded,               NOPARAM,                     "M.DELR",                                      "DELR",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1516 */  { itemToBeCoded,               NOPARAM,                     "M.DIM",                                       "DIM",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1517 */  { itemToBeCoded,               NOPARAM,                     "M.DIM?",                                      "DIM?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1518 */  { fnSetDateFormat,             ITM_MDY,                     "M.DY",                                        "M.DY",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1519 */  { itemToBeCoded,               NOPARAM,                     "M.EDI",                                       "EDIT",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1520 */  { itemToBeCoded,               NOPARAM,                     "M.EDIN",                                      "EDITN",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1521 */  { itemToBeCoded,               NOPARAM,                     "M.GET",                                       "GETM",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1522 */  { itemToBeCoded,               NOPARAM,                     "M.GOTO",                                      "GOTO",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1523 */  { itemToBeCoded,               NOPARAM,                     "M.GROW",                                      "GROW",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1524 */  { itemToBeCoded,               NOPARAM,                     "M.INSR",                                      "INSR",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1525 */  { itemToBeCoded,               NOPARAM,                     "M.LU",                                        "M.LU",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1526 */  { itemToBeCoded,               NOPARAM,                     "M.NEW",                                       "NEW",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1527 */  { itemToBeCoded,               NOPARAM,                     "M.OLD",                                       "OLD",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1528 */  { itemToBeCoded,               NOPARAM,                     "M.PUT",                                       "PUTM",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1529 */  { itemToBeCoded,               NOPARAM,                     "M.R" STD_LEFT_RIGHT_ARROWS "R",               "R" STD_LEFT_RIGHT_ARROWS "R",                 0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1530 */  { itemToBeCoded,               NOPARAM,                     "M.SQR?",                                      "M.SQR?",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1531 */  { itemToBeCoded,               NOPARAM,                     "M.WRAP",                                      "WRAP",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1532 */  { fnNop,                       NOPARAM,                     "NOP",                                         "NOP",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1533 */  { fnOff,                       NOPARAM,                     "OFF",                                         "OFF",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1534 */  { itemToBeCoded,               NOPARAM,                     "PAUSE",                                       "PAUSE",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1535 */  { fnPyx,                       NOPARAM,                     "PERM",                                        "P" STD_SUB_y STD_SUB_x,                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1536 */  { itemToBeCoded,               NOPARAM,                     "PGMINT",                                      "PGMINT",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1537 */  { itemToBeCoded,               NOPARAM,                     "PGMSLV",                                      "PGMSLV",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1538 */  { itemToBeCoded,               NOPARAM,                     "PIXEL",                                       "PIXEL",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1539 */  { fnPlot,                      NOPARAM,                     "PLOT",                                        "PLOT",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1540 */  { itemToBeCoded,               NOPARAM,                     "P" STD_SUB_n,                                 "P" STD_SUB_n,                                 0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1541 */  { itemToBeCoded,               NOPARAM,                     "POINT",                                       "POINT",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1542 */  { fnLoad,                      LM_NAMED_VARIABLES,          "LOADV",                                       "LOADV",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1543 */  { itemToBeCoded,               NOPARAM,                     "PopLR",                                       "PopLR",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1544 */  { itemToBeCoded,               NOPARAM,                     "PRCL",                                        "PRCL",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1545 */  { itemToBeCoded,               NOPARAM,                     "PSTO",                                        "PSTO",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1546 */  { itemToBeCoded,               NOPARAM,                     "PUTK",                                        "PUTK",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1547 */  { fnAngularMode,               AM_RADIAN,                   "RAD",                                         "RAD",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1548 */  { fnCvtToCurrentAngularMode,   AM_RADIAN,                   "RAD" STD_RIGHT_ARROW,                         "RAD" STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1549 */  { fnRandom,                    NOPARAM,                     "RAN#",                                        "RAN#",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1550 */  { registerBrowser,             NOPARAM     /*# JM #*/,      "REGS.V",                                      "REGS",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },//JM Changed RBR to REGS
/* 1551 */  { fnRecallConfig,              NOPARAM,                     "RCLCFG",                                      "Config",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1552 */  { fnRecallElement,             NOPARAM,                     "RCLEL",                                       "RCLEL",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1553 */  { fnRecallIJ,                  NOPARAM,                     "RCLIJ",                                       "RCLIJ",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1554 */  { fnRecallStack,               TM_REGISTER,                 "RCLS",                                        "RCLS",                                        0,      99,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1555 */  { itemToBeCoded,               NOPARAM,                     "RDP",                                         "RDP",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1556 */  { fnRealPart,                  NOPARAM,                     "Re",                                          "Re",                                          0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1557 */  { itemToBeCoded,               NOPARAM,                     "RECV",                                        "RECV",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1558 */  { fnReset,                     NOT_CONFIRMED,               "RESET",                                       "RESET",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1559 */  { fnReToCx,                    NOPARAM,                     "RE" STD_RIGHT_ARROW "CX",                     "RE" STD_RIGHT_ARROW "CX",                     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1560 */  { fnSwapRealImaginary,         NOPARAM,                     "Re" STD_LEFT_RIGHT_ARROWS "Im",               "Re" STD_LEFT_RIGHT_ARROWS "Im",               0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1561 */  { itemToBeCoded,               NOPARAM     /*# JM #*/,      "RMODE",                                       "RMODE",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1562 */  { fnGetRoundingMode,           NOPARAM     /*# JM #*/,      "RMODE?",                                      "RMODE?",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1563 */  { fnRmd,                       NOPARAM,                     "RMD",                                         "RMD",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1564 */  { itemToBeCoded,               NOPARAM,                     "RNORM",                                       "RNORM",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1565 */  { fnRound,                     NOPARAM,                     "ROUND",                                       "ROUND",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1566 */  { fnRoundi,                    NOPARAM,                     "ROUNDI",                                      "ROUNDI",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1567 */  { itemToBeCoded,               NOPARAM,                     "RSD",                                         "RSD",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1568 */  { itemToBeCoded,               NOPARAM,                     "RSUM",                                        "RSUM",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1569 */  { itemToBeCoded,               NOPARAM,                     "RTN+1",                                       "RTN+1",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1570 */  { itemToBeCoded,               NOPARAM,                     "R-CLR",                                       "R-CLR",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1571 */  { itemToBeCoded,               NOPARAM,                     "R-COPY",                                      "R-COPY",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1572 */  { itemToBeCoded,               NOPARAM,                     "R-SORT",                                      "R-SORT",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1573 */  { itemToBeCoded,               NOPARAM,                     "R-SWAP",                                      "R-SWAP",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1574 */  { fnCvtRadToDeg,               NOPARAM,                     "R" STD_RIGHT_ARROW "D",                       "R" STD_RIGHT_ARROW "D",                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1575 */  { fnSampleStdDev,              NOPARAM,                     "s",                                           "s",                                           0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1576 */  { fnSave,                      NOPARAM,                     "SAVE",                                        "SAVE",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1577 */  { fnDisplayFormatSci,          TM_VALUE,                    "SCI",                                         "SCI",                                         0,      15,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1578 */  { fnGetSignificantDigits,      NOPARAM,                     "SDIGS?",                                      "SDIGS?",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1579 */  { fnSeed,                      NOPARAM,                     "SEED",                                        "SEED",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1580 */  { itemToBeCoded,               NOPARAM,                     "SEND",                                        "SEND",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1581 */  { fnConfigChina,               NOPARAM,                     "SETCHN",                                      "CHINA",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1582 */  { itemToBeCoded,               NOPARAM,                     "SETDAT",                                      "SETDAT",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1583 */  { fnConfigEurope,              NOPARAM,                     "SETEUR",                                      "EUROPE",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1584 */  { fnConfigIndia,               NOPARAM,                     "SETIND",                                      "INDIA",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1585 */  { fnConfigJapan,               NOPARAM,                     "SETJPN",                                      "JAPAN",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1586 */  { itemToBeCoded,               NOPARAM,                     "SETSIG",                                      "SETSIG",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1587 */  { itemToBeCoded,               NOPARAM,                     "SETTIM",                                      "SETTIM",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1588 */  { fnConfigUk,                  NOPARAM,                     "SETUK",                                       "UK",                                          0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1589 */  { fnConfigUsa,                 NOPARAM,                     "SETUSA",                                      "USA",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1590 */  { fnSign,                      NOPARAM,                     "SIGN",                                        "sign",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1591 */  { fnIntegerMode,               SIM_SIGNMT,                  "SIGNMT",                                      "SIGNMT",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1592 */  { itemToBeCoded,               NOPARAM,                     "SIM_EQ",                                      "SIM EQ",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1593 */  { itemToBeCoded,               NOPARAM,                     "SKIP",                                        "SKIP",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1594 */  { fnSlvq,                      NOPARAM,                     "SLVQ",                                        "SLVQ",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1595 */  { fnStandardError,             NOPARAM,                     "s" STD_SUB_m,                                 "s" STD_SUB_m,                                 0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1596 */  { fnGetIntegerSignMode,        NOPARAM,                     "ISM?",                                        "ISM?",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1597 */  { fnWeightedStandardError,     NOPARAM,                     "s" STD_SUB_m STD_SUB_w,                       "s" STD_SUB_m STD_SUB_w,                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1598 */  { itemToBeCoded,               NOPARAM,                     "SOLVE",                                       "SOLVE",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1599 */  { fnGetStackSize,              NOPARAM,                     "SSIZE?",                                      "SSIZE?",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1600 */  { flagBrowser,                 NOPARAM,                     "STATUS",                                      "STATUS",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1601 */  { fnStoreConfig,               NOPARAM,                     "STOCFG",                                      "Config",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1602 */  { fnStoreElement,              NOPARAM,                     "STOEL",                                       "STOEL",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1603 */  { fnStoreIJ,                   NOPARAM,                     "STOIJ",                                       "STOIJ",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1604 */  { itemToBeCoded,               NOPARAM,                     "STOP",                                        "R/S",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1605 */  { fnStoreStack,                TM_REGISTER,                 "STOS",                                        "STOS",                                        0,      99,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1606 */  { fnSumXY,                     NOPARAM,                     "SUM",                                         "SUM",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1607 */  { fnWeightedSampleStdDev,      NOPARAM,                     "s" STD_SUB_w,                                 "s" STD_SUB_w,                                 0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1608 */  { itemToBeCoded,               NOPARAM,                     "s" STD_SUB_x STD_SUB_y,                       "s" STD_SUB_x STD_SUB_y,                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1609 */  { itemToBeCoded,               NOPARAM,                     "TDISP",                                       "TDISP",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1610 */  { fnTicks,                     NOPARAM,                     "TICKS",                                       "TICKS",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1611 */  { itemToBeCoded,               NOPARAM,                     "TIME",                                        "TIME",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1612 */  { itemToBeCoded,               NOPARAM,                     "TIMER",                                       "TIMER",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1613 */  { itemToBeCoded,               NOPARAM,                     "T" STD_SUB_n,                                 "T" STD_SUB_n,                                 0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1614 */  { itemToBeCoded,               NOPARAM,                     "TONE",                                        "TONE",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1615 */  { fnSwapT,                     TM_REGISTER,                 "t" STD_LEFT_RIGHT_ARROWS,                     "t" STD_LEFT_RIGHT_ARROWS,                     0,      99,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1616 */  { fnUlp,                       NOPARAM,                     "ULP?",                                        "ULP?",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1617 */  { itemToBeCoded,               NOPARAM,                     "U" STD_SUB_n,                                 "U" STD_SUB_n,                                 0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1618 */  { fnUnitVector,                NOPARAM,                     "UNITV",                                       "UNITV",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1619 */  { fnIntegerMode,               SIM_UNSIGN,                  "UNSIGN",                                      "UNSIGN",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1620 */  { itemToBeCoded,               NOPARAM,                     "VARMNU",                                      "VARMNU",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1621 */  { fnVersion,                   NOPARAM,                     "VERS?",                                       "VERS?",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1622 */  { fnShow,                      NOPARAM     /*# JM #*/,      "VIEW",                                        "VIEW",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },//TEMPORARY SHOW OLD
/* 1623 */  { itemToBeCoded,               NOPARAM,                     "WDAY",                                        "WDAY",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1624 */  { fnWho,                       NOPARAM,                     "WHO?",                                        "WHO?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1625 */  { itemToBeCoded,               NOPARAM,                     "W" STD_SUB_m,                                 "W" STD_SUB_m,                                 0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1626 */  { itemToBeCoded,               NOPARAM,                     "W" STD_SUB_p,                                 "W" STD_SUB_p,                                 0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1627 */  { itemToBeCoded,               NOPARAM,                     "W" STD_SUP_MINUS_1,                           "W" STD_SUP_MINUS_1,                           0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1628 */  { fnSetWordSize,               TM_VALUE,                    "WSIZE",                                       "WSIZE",                                       0,      64,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1629 */  { fnGetWordSize,               NOPARAM,                     "WSIZE?",                                      "WSIZE?",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1630 */  { fnMeanXY,                    NOPARAM,                     STD_x_BAR,                                     STD_x_BAR,                                     0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1631 */  { fnGeometricMeanXY,           NOPARAM,                     STD_x_BAR STD_SUB_G,                           STD_x_BAR STD_SUB_G,                           0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1632 */  { fnWeightedMeanX,             NOPARAM,                     STD_x_BAR STD_SUB_w,                           STD_x_BAR STD_SUB_w,                           0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1633 */  { itemToBeCoded,               NOPARAM,                     STD_x_CIRC,                                    STD_x_CIRC,                                    0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1634 */  { itemToBeCoded,               NOPARAM,                     "x" STD_RIGHT_ARROW "DATE",                    "x" STD_RIGHT_ARROW "DATE",                    0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1635 */  { fnXToAlpha,                  NOPARAM,                     "x" STD_RIGHT_ARROW STD_alpha,                 "x" STD_RIGHT_ARROW STD_alpha,                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1636 */  { fnSwapX,                     TM_REGISTER,                 "x" STD_LEFT_RIGHT_ARROWS,                     "x" STD_LEFT_RIGHT_ARROWS,                     0,      99,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1637 */  { itemToBeCoded,               NOPARAM,                     "YEAR",                                        "YEAR",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1638 */  { itemToBeCoded,               NOPARAM,                     STD_y_CIRC,                                    STD_y_CIRC,                                    0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1639 */  { fnSetDateFormat,             ITM_YMD,                     "Y.MD",                                        "Y.MD",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1640 */  { fnSwapY,                     TM_REGISTER,                 "y" STD_LEFT_RIGHT_ARROWS,                     "y" STD_LEFT_RIGHT_ARROWS,                     0,      99,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1641 */  { fnSwapZ,                     TM_REGISTER,                 "z" STD_LEFT_RIGHT_ARROWS,                     "z" STD_LEFT_RIGHT_ARROWS,                     0,      99,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1642 */  { fnAlphaLeng,                 TM_REGISTER,                 STD_alpha "LENG?",                             STD_alpha "LENG?",                             0,      99,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1643 */  { fnXmax,                      NOPARAM,                     "x" STD_SUB_m STD_SUB_a STD_SUB_x,             "x" STD_SUB_m STD_SUB_a STD_SUB_x,             0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1644 */  { fnXmin,                      NOPARAM,                     "x" STD_SUB_m STD_SUB_i STD_SUB_n,             "x" STD_SUB_m STD_SUB_i STD_SUB_n,             0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1645 */  { fnAlphaPos,                  TM_REGISTER,                 STD_alpha "POS?",                              STD_alpha "POS?",                              0,      99,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1646 */  { fnAlphaRL,                   TM_REGISTER,                 STD_alpha "RL",                                STD_alpha "RL",                                0,      99,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1647 */  { fnAlphaRR,                   TM_REGISTER,                 STD_alpha "RR",                                STD_alpha "RR",                                0,      99,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1648 */  { fnAlphaSL,                   TM_REGISTER,                 STD_alpha "SL",                                STD_alpha "SL",                                0,      99,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1649 */  { fnAlphaSR,                   TM_REGISTER,                 STD_alpha "SR",                                STD_alpha "SR",                                0,      99,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1650 */  { fnAlphaToX,                  TM_REGISTER,                 STD_alpha STD_RIGHT_ARROW "x",                 STD_alpha STD_RIGHT_ARROW "x",                 0,      99,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1651 */  { fnBeta,                      NOPARAM,                     STD_beta "(x,y)",                              STD_beta "(x,y)",                              0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1652 */  { itemToBeCoded,               NOPARAM,                     STD_gamma STD_SUB_x STD_SUB_y,                 STD_gamma STD_SUB_x STD_SUB_y,                 0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1653 */  { itemToBeCoded,               NOPARAM,                     STD_GAMMA STD_SUB_x STD_SUB_y,                 STD_GAMMA STD_SUB_x STD_SUB_y,                 0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1654 */  { fnGamma,                     NOPARAM,                     STD_GAMMA "(x)",                               STD_GAMMA "(x)",                               0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1655 */  { itemToBeCoded,               NOPARAM,                     STD_delta "x",                                 STD_delta "x",                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1656 */  { fnDeltaPercent,              NOPARAM,                     STD_DELTA "%",                                 STD_DELTA "%",                                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1657 */  { fnGeometricSampleStdDev,     NOPARAM,                     STD_epsilon,                                   STD_epsilon,                                   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1658 */  { fnGeometricStandardError,    NOPARAM,                     STD_epsilon STD_SUB_m,                         STD_epsilon STD_SUB_m,                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1659 */  { fnGeometricPopulationStdDev, NOPARAM,                     STD_epsilon STD_SUB_p,                         STD_epsilon STD_SUB_p,                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1660 */  { itemToBeCoded,               NOPARAM,                     STD_zeta "(x)",                                STD_zeta "(x)",                                0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1661 */  { itemToBeCoded,               NOPARAM,                     STD_PI STD_SUB_n,                              STD_PI STD_SUB_n,                              0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1662 */  { itemToBeCoded,               NOPARAM,                     STD_SIGMA STD_SUB_n,                           STD_SIGMA STD_SUB_n,                           0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1663 */  { fnPopulationStdDev,          NOPARAM,                     STD_sigma,                                     STD_sigma,                                     0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1664 */  { fnWeightedPopulationStdDev,  NOPARAM,                     STD_sigma STD_SUB_w,                           STD_sigma STD_SUB_w,                           0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1665 */  { fnRandomI,                   NOPARAM,                     "RANI#",                                       "RANI#",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1666 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "x",                               STD_PRINTER "x",                               0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1667 */  { fnRange,                     NOPARAM,                     "RANGE",                                       "RANGE",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1668 */  { fnGetRange,                  NOPARAM,                     "RANGE?",                                      "RANGE?",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1669 */  { fnM1Pow,                     NOPARAM,                     "(-1)" STD_SUP_x,                              "(-1)" STD_SUP_x,                              0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1670 */  { itemToBeCoded,               NOPARAM,                     STD_CROSS "MOD",                               STD_CROSS "MOD",                               0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1671 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "DATE",                        STD_RIGHT_ARROW "DATE",                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1672 */  { fnCvtFromCurrentAngularMode, AM_DEGREE,                   STD_RIGHT_ARROW "DEG",                         STD_RIGHT_ARROW "DEG",                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1673 */  { fnCvtFromCurrentAngularMode, AM_DMS,                      STD_RIGHT_ARROW "D.MS",                        STD_RIGHT_ARROW "D.MS",                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1674 */  { fnCvtFromCurrentAngularMode, AM_GRAD,                     STD_RIGHT_ARROW "GRAD",                        STD_RIGHT_ARROW "GRAD",                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1675 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "HR",                          ".d",                                          0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1676 */  { itemToBeCoded,               NOPARAM     /*# JM #*/,      STD_RIGHT_ARROW "H.MS",                        STD_RIGHT_ARROW "h.ms",                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },//JM mod
/* 1677 */  { fnChangeBase,                TM_VALUE_CHB,                STD_RIGHT_ARROW "INT",                         "#",                                           2,      16,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1678 */  { fnCvtFromCurrentAngularMode, AM_MULTPI,                   STD_RIGHT_ARROW "MUL" STD_pi,                  STD_RIGHT_ARROW "MUL" STD_pi,                  0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1679 */  { fnToPolar,                   NOPARAM     /*# JM #*/,      STD_RIGHT_ARROW "POL" STD_SUB_o,               STD_RIGHT_ARROW "P" STD_SUB_o,                 0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM TEXT & point to function to add POLAR/RECT
/* 1680 */  { fnCvtFromCurrentAngularMode, AM_RADIAN,                   STD_RIGHT_ARROW "RAD",                         STD_RIGHT_ARROW "RAD",                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1681 */  { fnToReal,                    NOPARAM,                     STD_RIGHT_ARROW "REAL",                        ".d",                                          0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1682 */  { fnToRect,                    NOPARAM     /*# JM #*/,      STD_RIGHT_ARROW "REC" STD_SUB_o,               "R" STD_LEFT_ARROW STD_SUB_o,                  0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//SWAPPED ARROW DIRECTION & JM TEXT & point to function to add POLAR/RECT
/* 1683 */  { fnCvtDegToDms,               NOPARAM,                     "D" STD_RIGHT_ARROW "D.MS",                    "D" STD_RIGHT_ARROW "D.MS",                    0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1684 */  { fnShuffle,                   TM_SHUFFLE,                  STD_LEFT_RIGHT_ARROWS,                         STD_LEFT_RIGHT_ARROWS,                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1685 */  { fnPercent,                   NOPARAM,                     "%",                                           "%",                                           0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1686 */  { fnPercentMRR,                NOPARAM,                     "%MRR",                                        "%MRR",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1687 */  { fnPercentT,                  NOPARAM,                     "%T",                                          "%T",                                          0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1688 */  { fnPercentSigma,              NOPARAM,                     "%" STD_SIGMA,                                 "%" STD_SIGMA,                                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1689 */  { fnPercentPlusMG,             NOPARAM,                     "%+MG",                                        "%+MG",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1690 */  { itemToBeCoded,               NOPARAM,                     STD_INTEGRAL,                                  STD_INTEGRAL,                                  0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1691 */  { itemToBeCoded,               NOPARAM,                     "^MOD",                                        "^MOD",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1692 */  { itemToBeCoded,               NOPARAM,                     "|M|",                                         "|M|",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1693 */  { fnParallel,                  NOPARAM     /*# JM #*/,      "|" STD_SPACE_3_PER_EM "|",                    "|" STD_SPACE_3_PER_EM "|",                    0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM
/* 1694 */  { itemToBeCoded,               NOPARAM,                     "[M]" STD_SUP_T,                               "[M]" STD_SUP_T,                               0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1695 */  { itemToBeCoded,               NOPARAM,                     "[M]" STD_SUP_MINUS_1,                         "[M]" STD_SUP_MINUS_1,                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1696 */  { fnArg_all,                   NOPARAM     /*# JM #*/,      STD_MEASURED_ANGLE,                            STD_MEASURED_ANGLE,                            0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1697 */  { fnCvtToCurrentAngularMode,   AM_MULTPI,                   "MUL" STD_pi STD_RIGHT_ARROW,                  "MUL" STD_pi STD_RIGHT_ARROW,                  0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1698 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "ADV",                             STD_PRINTER "ADV",                             0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1699 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "CHAR",                            STD_PRINTER "CHAR",                            0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1700 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "DLAY",                            STD_PRINTER "DLAY",                            0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1701 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "LCD",                             STD_PRINTER "LCD",                             0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1702 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "MODE",                            STD_PRINTER "MODE",                            0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1703 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "PROG",                            STD_PRINTER "PROG",                            0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1704 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "r",                               STD_PRINTER "r",                               0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1705 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "REGS",                            STD_PRINTER "REGS",                            0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1706 */  { fnP_All_Regs,                1     /*# JM #*/,            STD_PRINTER "STK",                             STD_PRINTER "STK",                             0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1707 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "TAB",                             STD_PRINTER "TAB",                             0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1708 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "USER",                            STD_PRINTER "USER",                            0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1709 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "WIDTH",                           STD_PRINTER "WIDTH",                           0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1710 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER STD_SIGMA,                         STD_PRINTER STD_SIGMA,                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1711 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "#",                               STD_PRINTER "#",                               0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },

/* 1712 */  { fontBrowser,                 NOPARAM,                     "FBR",                                         "FBR",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},// Font Browser

/* 1713 */  { fnUndo,                      NOPARAM,                     "UNDO",                                        STD_UNDO,                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},
/* 1714 */  { itemToBeCoded,               NOPARAM     /*# JM #*/,      "PRGM",                                        "PRGM",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM Change P/R to PRGM
/* 1715 */  { itemToBeCoded,               NOPARAM,                     "R/S",                                         "R/S",                                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1716 */  { itemToBeCoded,               NOPARAM,                     "",                                            "Not",                                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1717 */  { itemToBeCoded,               NOPARAM,                     "",                                            "yet",                                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1718 */  { itemToBeCoded,               NOPARAM,                     "",                                            "defined",                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1719 */  { fnFlipFlag,                  FLAG_USER,                   "USER",                                        "USER",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1720 */  { fnKeyCC,                     NOPARAM,                     "CC",                                          "CC",                                          0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1721 */  { itemToBeCoded,               NOPARAM,                     "",                                            "f",                                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1722 */  { itemToBeCoded,               NOPARAM,                     "",                                            "g",                                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1723 */  { fnKeyUp,                     NOPARAM,                     "UP",                                          STD_UP_ARROW,                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1724 */  { itemToBeCoded,               NOPARAM,                     "BST",                                         STD_HAMBURGER STD_BST,                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1725 */  { fnKeyDown,                   NOPARAM,                     "DOWN",                                        STD_DOWN_ARROW,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1726 */  { itemToBeCoded,               NOPARAM,                     "SST",                                         STD_HAMBURGER STD_SST,                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1727 */  { fnKeyExit,                   NOPARAM,                     "EXIT",                                        "EXIT",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1728 */  { fnKeyBackspace,              NOPARAM,                     "BKSPC",                                       STD_LEFT_ARROW,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1729 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PRINTER "x",                               0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1730 */  { fnAim,                       NOPARAM     /*# JM #*/,      "AIM",                                         "ALPHA",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_ENABLED  },//JM
/* 1731 */  { fnKeyDotD,                   NOPARAM,                     ".d",                                          ".d",                                          0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1732 */  { fnShow_SCROLL,               NOPARAM     /*# JM #*/,      "SHOW",                                        "SHOW",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1733 */  { backToSystem,                NOPARAM,                     "SYSTEM",                                      "SYSTEM",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},
/* 1734 */  { fnCvtDmsToDeg,               NOPARAM,                     "D.MS" STD_RIGHT_ARROW "D",                    "D.MS" STD_RIGHT_ARROW "D",                    0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1735 */  { itemToBeCoded,               NOPARAM,                     "V" STD_MEASURED_ANGLE,                        STD_MEASURED_ANGLE,                            0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1736 */  { fnHarmonicMeanXY,            NOPARAM,                     STD_x_BAR STD_SUB_H,                           STD_x_BAR STD_SUB_H,                           0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1737 */  { fnRMSMeanXY,                 NOPARAM,                     STD_x_BAR STD_SUB_R STD_SUB_M STD_SUB_S,       STD_x_BAR STD_SUB_R STD_SUB_M STD_SUB_S,       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1738 */  { fnArccos,                    NOPARAM,                     "ACOS",                                        "ACOS",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1739 */  { fnArcsin,                    NOPARAM,                     "ASIN",                                        "ASIN",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1740 */  { fnArctan,                    NOPARAM,                     "ATAN",                                        "ATAN",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1741 */  { itemToBeCoded,               NOPARAM,                     "DET",                                         "DET",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1742 */  { itemToBeCoded,               NOPARAM,                     "INVRT",                                       "INVRT",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1743 */  { itemToBeCoded,               NOPARAM,                     "TRANS",                                       "TRANS",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1744 */  { itemToBeCoded,               NOPARAM,                     "xIN",                                         "xIN",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1745 */  { itemToBeCoded,               NOPARAM,                     "xOUT",                                        "xOUT",                                        0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },






/* 1746 */  { fnSetSetJM,                  JC_FG_LINE,                  "fg" STD_SPACE_3_PER_EM "LINE",                "fg" STD_SPACE_3_PER_EM "LINE",                0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM UNDERLINING
/* 1747 */  { fnSetSetJM,                  JC_FG_DOTS,                  "fg" STD_SPACE_3_PER_EM "DOTS",                "fg" STD_SPACE_3_PER_EM "DOTS",                0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM UNDERLINING
/* 1748 */  { fnSetSetJM,                  JC_G_DOUBLETAP,              "G" STD_SPACE_3_PER_EM "2TAP",                 "G" STD_SPACE_3_PER_EM "2TAP",                 0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM KEY TAP DOUBLE SETTING
/* 1749 */  { itemToBeCoded,               NOPARAM,                     "1749",                                        "1749",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1750 */  { fnP_All_Regs,                0,                           STD_PRINTER "ALLr",                            STD_PRINTER "ALLr",                            0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},
/* 1751 */  { fnMultiplySI,                85,                          STD_DOT "f",                                   STD_DOT "f",                                   0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM PRE UNIT
/* 1752 */  { fnMultiplySI,                88,                          STD_DOT "p",                                   STD_DOT "p",                                   0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM PRE UNIT
/* 1753 */  { fnMultiplySI,                91,                          STD_DOT "n",                                   STD_DOT "n",                                   0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM PRE UNIT
/* 1754 */  { fnMultiplySI,                94,                          STD_DOT STD_mu,                                STD_DOT STD_mu,                                0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM PRE UNIT
/* 1755 */  { fnMultiplySI,                97,                          STD_DOT "m",                                   STD_DOT "m",                                   0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM PRE UNIT
/* 1756 */  { fnMultiplySI,                103,                         STD_DOT "k",                                   STD_DOT "k",                                   0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM PRE UNIT
/* 1757 */  { fnMultiplySI,                106,                         STD_DOT "M",                                   STD_DOT "M",                                   0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM PRE UNIT
/* 1758 */  { fnMultiplySI,                109,                         STD_DOT "G",                                   STD_DOT "G",                                   0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM PRE UNIT
/* 1759 */  { fnMultiplySI,                112,                         STD_DOT "T",                                   STD_DOT "T",                                   0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM PRE UNIT
/* 1760 */  { addItemToBuffer,             ITM_QOPPA,                   "",                                            STD_QOPPA,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK
/* 1761 */  { addItemToBuffer,             ITM_DIGAMMA,                 "",                                            STD_DIGAMMA,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK
/* 1762 */  { addItemToBuffer,             ITM_SAMPI,                   "",                                            STD_SAMPI,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK
/* 1763 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1764 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1765 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1766 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1767 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1768 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1769 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1770 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1771 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1772 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1773 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1774 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1775 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1776 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1777 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1778 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1779 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1780 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1781 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1782 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1783 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1784 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1785 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1786 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1787 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1788 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1789 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1790 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1791 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1792 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1793 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1794 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1795 */  { itemToBeCoded,               NOPARAM,                     "reserved",                                    "reserved",                                    0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK (Reserved - do not use)
/* 1796 */  { addItemToBuffer,             ITM_qoppa,                   "",                                            STD_qoppa,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK
/* 1797 */  { addItemToBuffer,             ITM_digamma,                 "",                                            STD_digamma,                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK
/* 1798 */  { addItemToBuffer,             ITM_sampi,                   "",                                            STD_sampi,                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM GREEK
/* 1799 */  { fnKeyCC,                     KEY_COMPLEX,                 "COMPLEX",                                     "COMPLEX",                                     0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },//JM Change CC to COMPLEX
/* 1800 */  { fnToPolar2,                  NOPARAM,                     STD_RIGHT_ARROW "POLAR",                       STD_RIGHT_ARROW "P",                           0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM TEXT & point to function to add POLAR/RECT
/* 1801 */  { fnToRect2,                   NOPARAM,                     STD_RIGHT_ARROW "RECT",                        STD_RIGHT_ARROW "R",                           0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//SWAPPED ARROW DIRECTION & JM TEXT & point to function to add POLAR/RECT
/* 1802 */  { fneRPN,                      1,                           "eRPN",                                        "eRPN",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1803 */  { fneRPN,                      0,                           "RPN",                                         "RPN",                                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1804 */  { fnSetSetJM,                  JC_ERPN,                     "eRPN",                                        "eRPN",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM eRPN
/* 1805 */  { fnSetSetJM,                  JC_HOME_TRIPLE,              "HOME.3",                                      "HOME.3",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM HOME.3
/* 1806 */  { fnSetSetJM,                  JC_SHFT_4s,                  "SH_4s",                                       "SH_4s",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM SHIFT CANCEL
/* 1807 */  { fnSetSetJM,                  JC_BCR,                      "CPXRES",                                      "CPXRES",                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//dr
/* 1808 */  { fnSetSetJM,                  JC_BLZ,                      "LEAD0",                                       "LEAD0",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//dr
/* 1809 */  { fnCase,                      0,                           "",                                            STD_case,                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM CASE
/* 1810 */  { fnSetSetJM,                  JC_BASE_HOME,                "_HOME",                                       "HOME",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM eRPN
/* 1811 */  { fnSetSetJM,                  JC_BASE_AHOME,               "_" STD_alpha "HOME",                          STD_alpha "HOME",                              0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM eRPN
/* 1812 */  { fnSetSetJM,                  JC_H_SUM,                    "H-SUMMARY",                                   "SUMRY",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JMHOME
/* 1813 */  { fnSetSetJM,                  JC_H_MIR,                    "H-MIRROR",                                    "REPLCA",                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JMHOME
/* 1814 */  { fnSetSetJM,                  JC_H_FIX,                    "H-FIXED",                                     "FIXED",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JMHOME
/* 1815 */  { fnSetSetJM,                  JC_SH_3T,                    "SH.3T",                                       "SH.3T",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_ENABLED  },

/* 1816 */  { fnSetSetJM,                  JC_LARGELI,                  "LARGELI",                                     "LRG_LI",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},
/* 1817 */  { fnJM,                        45,                          "TEST_45",                                     "TEST_45",                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1818 */  { fnDisplayFormatSigFig,       TM_VALUE,                    "SIG",                                         "SIG",                                         0,      15,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM SIGFIG
/* 1819 */  { fnDisplayFormatUnit,         TM_VALUE,                    "UNIT",                                        "UNIT",                                        0,      15,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM UNIT
/* 1820 */  { fnRound2,                    NOPARAM,                     "ROUND",                                       "ROUND",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1821 */  { fnRoundi2,                   NOPARAM,                     "ROUNDI",                                      "ROUNDI",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1822 */  { fn_cnst_op_a,                NOPARAM,                     "op_a",                                        "a",                                           0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM Operator a
/* 1823 */  { fn_cnst_op_aa,               NOPARAM,                     "op_a" STD_SUP_2,                              "a" STD_SUP_2,                                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM Operator a.a
/* 1824 */  { fn_cnst_op_j,                NOPARAM,                     "op_j",                                        "j",                                           0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM Operator j
/* 1825 */  { fnJM,                        7,                           "Y" STD_SPACE_3_PER_EM STD_RIGHT_ARROW STD_SPACE_3_PER_EM STD_DELTA, "Y" STD_SPACE_3_PER_EM STD_RIGHT_ARROW STD_SPACE_3_PER_EM STD_DELTA,   0,       0,       CAT_FNCT, SLS_ENABLED  , US_UNCHANGED},//JM EE
/* 1826 */  { fnJM,                        6,                           STD_DELTA STD_SPACE_3_PER_EM STD_RIGHT_ARROW STD_SPACE_3_PER_EM "Y", STD_DELTA STD_SPACE_3_PER_EM STD_RIGHT_ARROW STD_SPACE_3_PER_EM "Y",   0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EE
/* 1827 */  { fnJM,                        9,                           "AtoSYM",                                      STD_RIGHT_ARROW STD_SPACE_3_PER_EM "012",      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EE
/* 1828 */  { fnJM,                        8,                           "SYMtoA",                                      STD_RIGHT_ARROW STD_SPACE_3_PER_EM "abc",      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EE
/* 1829 */  { fnJM,                        10,                          "e^" STD_THETA "j",                            "e^" STD_THETA "j",                            0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EE
/* 1830 */  { fnJM,                        11,                          "STO" STD_SPACE_3_PER_EM "3Z",                 "STO" STD_SPACE_3_PER_EM "3Z",                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EE
/* 1831 */  { fnJM,                        12,                          "RCL" STD_SPACE_3_PER_EM "3Z",                 "RCL" STD_SPACE_3_PER_EM "3Z",                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EE
/* 1832 */  { fnJM,                        13,                          "STO" STD_SPACE_3_PER_EM "3V",                 "STO" STD_SPACE_3_PER_EM "3V",                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EE
/* 1833 */  { fnJM,                        14,                          "RCL" STD_SPACE_3_PER_EM "3V",                 "RCL" STD_SPACE_3_PER_EM "3V",                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EE
/* 1834 */  { fnJM,                        15,                          "STO" STD_SPACE_3_PER_EM "3I",                 "STO" STD_SPACE_3_PER_EM "3I",                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EE
/* 1835 */  { fnJM,                        16,                          "RCL" STD_SPACE_3_PER_EM "3I",                 "RCL" STD_SPACE_3_PER_EM "3I",                 0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EE
/* 1836 */  { fnJM,                        17,                          "3V" STD_DIVIDE "3I",                          "V" STD_DIVIDE "I",                            0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EE
/* 1837 */  { fnJM,                        18,                          "3I" STD_CROSS "3Z",                           "I" STD_CROSS "Z",                             0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EE
/* 1838 */  { fnJM,                        19,                          "3V" STD_DIVIDE "3Z",                          "V" STD_DIVIDE "Z",                            0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EE
/* 1839 */  { fnJM,                        20,                          "X" STD_SPACE_3_PER_EM STD_RIGHT_ARROW STD_SPACE_3_PER_EM "BAL", "X" STD_SPACE_3_PER_EM STD_RIGHT_ARROW STD_SPACE_3_PER_EM "BAL",   0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EE
/* 1840 */  { fnDumpMenus,                 NOPARAM,                     "DUMPMNU",                                     "DUMPMNU",                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1841 */  { fnLongInt,                   NOPARAM,                     "LNGINT",                                      "LNGINT",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },
/* 1842 */  { fnChangeBaseJM,              2,                           "BIN",                                         "BIN",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM HEX
/* 1843 */  { fnChangeBaseJM,              8,                           "OCT",                                         "OCT",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM HEX
/* 1844 */  { fnChangeBaseJM,              10,                          "DEC",                                         "DEC",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM HEX
/* 1845 */  { fnChangeBaseJM,              16,                          "HEX",                                         "HEX",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM HEX
/* 1846 */  { fnSetWordSize,               8,                           "8-BIT",                                       "8-BIT",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM HEX
/* 1847 */  { fnSetWordSize,               16,                          "16-BIT",                                      "16-BIT",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM HEX
/* 1848 */  { fnSetWordSize,               32,                          "32-BIT",                                      "32-BIT",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM HEX
/* 1849 */  { fnSetWordSize,               64,                          "64-BIT",                                      "64-BIT",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM HEX
/* 1850 */  { fnJM_2SI,                    NOPARAM,                     STD_RIGHT_ARROW "I",                           STD_RIGHT_ARROW "I",                           0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM Copy of 1925, fnJM_2SI, -> "LI" -> "SI"
/* 1851 */  { fnChangeBaseMNU,             NOPARAM,                     "MODE#",                                       "MODE#",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_ENABLED  },
/* 1852 */  { fnJM,                        21,                          "GRF.X1",                                      "Xmin",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM GRAPH
/* 1853 */  { fnJM,                        22,                          "GRF.X2",                                      "Xmax",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM GRAPH
/* 1854 */  { fnJM,                        23,                          "GRF.Y1",                                      "Ymin",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM GRAPH
/* 1855 */  { fnJM,                        24,                          "GRF.Y2",                                      "Ymax",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM GRAPH
/* 1856 */  { fnJM,                        25,                          "GRF.DX",                                      "TICK DX",                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM GRAPH
/* 1857 */  { fnJM,                        26,                          "GRF.DY",                                      "TICK DY",                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM GRAPH
/* 1858 */  { fnJM,                        27,                          "GRF.HLP",                                     "GRF.HLP",                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM GRAPH
/* 1859 */  { fnCla,                       NOPARAM,                     "CLA",                                         "CLA",                                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//GRAPH
/* 1860 */  { fnCln,                       NOPARAM,                     "CLN",                                         "CLN",                                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//GRAPH
/* 1861 */  { itemToBeCoded,               NOPARAM,                     "1861",                                        "1861",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1862 */  { itemToBeCoded,               NOPARAM,                     "1862",                                        "1862",                                        0,       0,       CAT_FREE, SLS_UNCHANGED, US_UNCHANGED},
/* 1863 */  { fnCase,                      1,                           "CASE UP",                                     "^^",                                          0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM CASE
/* 1864 */  { fnCase,                      2,                           "CASE DN",                                     "vv",                                          0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM CASE
/* 1865 */  { fnListXY,                    NOPARAM,                     "LISTXY",                                      "LISTXY",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},


/* 1866 */  { fnShowJM,                    JC_ERPN,                     "eRPN?",                                       "eRPN?",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM SHOW
/* 1867 */  { fnSysFreeMem,                NOPARAM,                     "",                                            "HEAP",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_UNCHANGED},
/* 1868 */  { itemToBeCoded,               NOPARAM,                     "",                                            "Inl. Tst",                                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//INLINE_TEST
/* 1869 */  { fnSetInlineTest,             JC_ITM_TST,                  "",                                            "Test",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//INLINE_TEST
/* 1870 */  { fnGetInlineTestBsToX,        NOPARAM,                     "",                                            "Get BS",                                      0,       0,       CAT_NONE, SLS_ENABLED  , US_UNCHANGED},//INLINE_TEST
/* 1871 */  { fnSetInlineTestXToBs,        NOPARAM,                     "",                                            "Set BS",                                      0,       0,       CAT_NONE, SLS_ENABLED  , US_UNCHANGED},//INLINE_TEST
/* 1872 */  { fnInDefault,                 ID_DP,                       "i" STD_SPACE_3_PER_EM "REAL",                 "i" STD_SPACE_3_PER_EM "REAL",                 0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM INPUT DEFAULT
/* 1873 */  { fnShowJM,                    JM_INP_DFLT,                 "i" STD_SPACE_3_PER_EM "Dflt?",                "i" STD_SPACE_3_PER_EM "Dflt?",                0,       0,       CAT_NONE, SLS_UNCHANGED, US_ENABLED  },//JM INPUT DEFAULT
/* 1874 */  { fnInDefault,                 ID_CPXDP,                    "i" STD_SPACE_3_PER_EM "CPX",                  "i" STD_SPACE_3_PER_EM "CPX",                  0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM INPUT DEFAULT
/* 1875 */  { fnInDefault,                 ID_SI,                       "i" STD_SPACE_3_PER_EM "SI",                   "i" STD_SPACE_3_PER_EM "SI",                   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM INPUT DEFAULT
/* 1876 */  { fnInDefault,                 ID_LI,                       "i" STD_SPACE_3_PER_EM "LI",                   "i" STD_SPACE_3_PER_EM "LI",                   0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM INPUT DEFAULT
/* 1877 */  { fnUserJM,                    USER_V43,                    "V43 RT",                                      "V43 RT",                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//J=V43
/* 1878 */  { fnJM,                        255,                         "f/g",                                         "f/g",                                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM Shift replacement
/* 1879 */  { fnUserJM,                    USER_DEFAULTS,               "U" STD_SIGMA " CC",                           "U" STD_SIGMA " CC",                           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1880 */  { fnUserJM,                    USER_COMPLEX,                "U" STD_UP_ARROW " CC",                        "U" STD_UP_ARROW " CC",                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1881 */  { fnUserJM,                    USER_SHIFTS,                 "C43 ALT",                                     "C43 ALT",                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM USER
/* 1882 */  { fnUserJM,                    USER_RESET,                  "RESET",                                       "RESET",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1883 */  { fnSigmaAssign,               ITM_USERMODE,                STD_SIGMA "+USR",                              STD_SIGMA "+USR",                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1884 */  { fnSigmaAssign,               ITM_CC,                      STD_SIGMA "+CC",                               STD_SIGMA "+CC",                               0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1885 */  { fnSigmaAssign,               -MNU_MyMenu,                 STD_SIGMA "+MyM",                              STD_SIGMA "+MyM",                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1886 */  { fnSigmaAssign,               ITM_SIGMAPLUS,               STD_SIGMA "+",                                 STD_SIGMA "+",                                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM USER
/* 1887 */  { fnSigmaAssign,               ITM_PR,                      STD_SIGMA "+PGM",                              STD_SIGMA "+PGM",                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM USER
/* 1888 */  { fnSigmaAssign,               ITM_AIM,                     STD_SIGMA "+" STD_alpha,                       STD_SIGMA "+" STD_alpha,                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1889 */  { fnGetSigmaAssignToX,         NOPARAM,                     STD_SIGMA "+ toX",                             STD_SIGMA "+ toX",                             0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1890 */  { fnUserJM,                    JM_ASSIGN,                   "ASSIGN",                                      "ASSIGN",                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1891 */  { fnUserJM,                    JM_SEEK_FN,                  "FN" STD_DOT "ASN",                            "FN" STD_DOT "ASN",                            0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},
/* 1892 */  { fnInDefault,                 ID_43S,                      "i" STD_SPACE_3_PER_EM "LI/RL",                "i" STD_SPACE_3_PER_EM "LI/RL",                0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM INPUT DEFAULT
/* 1893 */  { fnXEQMXXEQ,                  NOPARAM,                     "X.XEQ",                                       "X.XEQ",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1894 */  { fnUserJM,                    USER_ALPHA,                  "U" STD_SIGMA STD_DOT STD_alpha,               "U" STD_SIGMA STD_DOT STD_alpha,               0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1895 */  { fnUserJM,                    USER_GSHFT,                  "U" STD_SIGMA STD_DOT "G" STD_DOT "SH",        "U" STD_SIGMA STD_DOT "G" STD_DOT "SH",        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1896 */  { fnUserJM,                    USER_CC,                     "U" STD_SIGMA STD_DOT "CC",                    "U" STD_SIGMA STD_DOT "CC",                    0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1897 */  { fnUserJM,                    USER_MYM,                    "U" STD_SIGMA STD_DOT "MYM",                   "U" STD_SIGMA STD_DOT "MYM",                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1898 */  { fnUserJM,                    USER_PRGM,                   "U" STD_SIGMA STD_DOT "PRGM",                  "U" STD_SIGMA STD_DOT "PRGM",                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1899 */  { fnUserJM,                    USER_USER,                   "U" STD_SIGMA STD_DOT "USER",                  "U" STD_SIGMA STD_DOT "USER",                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1900 */  { fnUserJM,                    USER_SIGMAPLUS,              "U" STD_SIGMA STD_DOT STD_SIGMA "+",           "U" STD_SIGMA STD_DOT STD_SIGMA "+",           0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1901 */  { fnUserJM,                    USER_V43LT,                  "V43 LT",                                      "V43 LT",                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//J=V43
/* 1902 */  { fnUserJM,                    USER_SHIFTS2,                "LT SHFT",                                     "LT SHFT",                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//J=V43
/* 1903 */  { fnUserJM,                    USER_HOME,                   "U" STD_SIGMA STD_DOT "HOME",                  "U" STD_SIGMA STD_DOT "HOME",                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1904 */  { fnUserJM,                    USER_43S,                    "WP43S",                                       "WP43S",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//J=V43
/* 1905 */  { fnUserJM,                    USER_DM42,                   "DM42",                                        "DM42",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//J=V43
/* 1906 */  { fnUserJM,                    USER_C43,                    "C43",                                         "C43",                                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//J=C43
/* 1907 */  { fnJM_GetXToNORMmode,         NOPARAM,                     "X to" STD_SIGMA "+",                          "X to" STD_SIGMA "+",                          0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM USER NORMAL
/* 1908 */  { itemToBeCoded,               NOPARAM,                     "GRAPH",                                       "GRAPH",                                       0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},//JM GRAPH
/* 1909 */  { itemToBeCoded,               NOPARAM,                     "I",                                           STD_SIGMA "+NRM",                              0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1910 */  { itemToBeCoded,               NOPARAM,                     "HOME",                                        "HOME",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},//JM HOME
/* 1911 */  { itemToBeCoded,               NOPARAM,                     "ALPHA",                                       "ALPHA",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM ALPHA
/* 1912 */  { itemToBeCoded,               NOPARAM,                     "BASE",                                        "BASE",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},//JM BASE
/* 1913 */  { itemToBeCoded,               NOPARAM,                     "XEQM",                                        "XEQM",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1914 */  { itemToBeCoded,               NOPARAM,                     "ELEC.ENG",                                    "ELEC",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_ENABLED  },//JM EE
/* 1915 */  { fnT_ARROW,                   ITM_T_UP_ARROW,              STD_UP_ARROW,                                  STD_UP_ARROW,                                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1916 */  { itemToBeCoded,               NOPARAM,                     "KEYS",                                        "KEYS",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1917 */  { fnT_ARROW,                   ITM_T_DOWN_ARROW,            STD_DOWN_ARROW,                                STD_DOWN_ARROW,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1918 */  { fnT_ARROW,                   ITM_T_HOME,                  "HOME",                                        "HOME",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1919 */  { fnT_ARROW,                   ITM_T_END,                   "END",                                         "END",                                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1920 */  { itemToBeCoded,               NOPARAM,                     "I",                                           STD_SIGMA "+U&N",                              0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},//J=V43
/* 1921 */  { itemToBeCoded,               NOPARAM,                     "T.EDIT",                                      "T.EDIT",                                      0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1922 */  { itemToBeCoded,               NOPARAM,                     "XXEQ",                                        "XXEQ",                                        0,       0,       CAT_MENU, SLS_UNCHANGED, US_UNCHANGED},
/* 1923 */  { fnRange,                     NOPARAM,                     "RNG",                                         "RNG",                                         0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },
/* 1924 */  { flagBrowser_old,             NOPARAM,                     "FLAGS.V",                                     "FLGS",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM Changed STATUS
/* 1925 */  { fnSetSetJM,                  CU_I,                        "CPXi",                                        "CPXi",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1926 */  { fnSetSetJM,                  CU_J,                        "CPXj",                                        "CPXj",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1927 */  { fnSetSetJM,                  SS_4,                        "SSIZE4",                                      "SSIZE4",                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1928 */  { fnSetSetJM,                  SS_8,                        "SSIZE8",                                      "SSIZE8",                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1929 */  { fnSetSetJM,                  JC_BSR,                      "SPCRES",                                      "SPCRES",                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1930 */  { fnCFGsettings,               NOPARAM,                     "CFG",                                         "CFG",                                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM Replacements
/* 1931 */  { fnSetSetJM,                  TF_H12,                      "CLK12",                                       "CLK12",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM Replacements
/* 1932 */  { fnSetSetJM,                  TF_H24,                      "CLK24",                                       "CLK24",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM Replacements
/* 1933 */  { fnSetSetJM,                  PS_CROSS,                    "MULT" STD_CROSS,                              "MULT" STD_CROSS,                              0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1934 */  { fnSetSetJM,                  PS_DOT,                      "MULT" STD_DOT,                                "MULT" STD_DOT,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1935 */  { fnSetSetJM,                  CM_POLAR,                    "POLAR",                                       "POLAR",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM Replacements
/* 1936 */  { fnSetSetJM,                  RX_COMMA,                    "RDX,",                                        "RDX,",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM Replacements
/* 1937 */  { fnSetSetJM,                  RX_PERIOD,                   "RDX.",                                        "RDX.",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM Replacements
/* 1938 */  { fnSetSetJM,                  CM_RECTANGULAR,              "RECT",                                        "RECT",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM Replacements
/* 1939 */  { fnSetSetJM,                  DO_SCI,                      "SCIOVR",                                      "SCIOVR",                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM Replacements
/* 1940 */  { fnSetSetJM,                  DO_ENG,                      "ENGOVR",                                      "ENGOVR",                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM Replacements
/* 1941 */  { fnT_ARROW,                   ITM_T_LEFT_ARROW  ,          STD_LEFT_ARROW,                                STD_LEFT_ARROW,                                0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1942 */  { fnT_ARROW,                   ITM_T_RIGHT_ARROW ,          STD_RIGHT_ARROW,                               STD_RIGHT_ARROW,                               0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1943 */  { fnT_ARROW,                   ITM_T_LLEFT_ARROW ,          STD_LEFT_ARROW STD_LEFT_ARROW,                 STD_LEFT_ARROW STD_LEFT_ARROW,                 0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1944 */  { fnT_ARROW,                   ITM_T_RRIGHT_ARROW,          STD_RIGHT_ARROW STD_RIGHT_ARROW,               STD_RIGHT_ARROW STD_RIGHT_ARROW,               0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1945 */  { fnXEQNEW,                    NOPARAM,                     "X.NEW",                                       "X.NEW",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1946 */  { fnXEQMEDIT,                  NOPARAM,                     "X.EDIT",                                      "X.EDIT",                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 1947 */  { fnTo_ms,                     NOPARAM,                     ".ms",                                         ".ms",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM DMS HMS
/* 1948 */  { fnAngularModeJM,             AM_DEGREE,                   STD_RIGHT_DOUBLE_ANGLE "DEG",                  STD_RIGHT_DOUBLE_ANGLE "DEG",                  0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1949 */  { fnAngularModeJM,             AM_DMS,                      STD_RIGHT_DOUBLE_ANGLE "D.MS",                 STD_RIGHT_DOUBLE_ANGLE "d.ms",                 0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1950 */  { fnAngularModeJM,             AM_GRAD,                     STD_RIGHT_DOUBLE_ANGLE "GRAD",                 STD_RIGHT_DOUBLE_ANGLE "GRAD",                 0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1951 */  { fnAngularModeJM,             AM_MULTPI,                   STD_RIGHT_DOUBLE_ANGLE "MUL" STD_pi,           STD_RIGHT_DOUBLE_ANGLE "MUL" STD_pi,           0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1952 */  { fnAngularModeJM,             AM_RADIAN,                   STD_RIGHT_DOUBLE_ANGLE "RAD",                  STD_RIGHT_DOUBLE_ANGLE "RAD",                  0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1953 */  { fnAngularModeJM,             AM_HMS,                      STD_RIGHT_DOUBLE_ANGLE "H.MS",                 STD_RIGHT_DOUBLE_ANGLE "h.ms",                 0,       0,       CAT_FNCT, SLS_UNCHANGED, US_ENABLED  },
/* 1954 */  { fnJMUSERmode,                256+0,                       "",                                            "USER_PRIM00U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1955 */  { fnJMUSERmode_f,              256+0,                       "",                                            "USER_SFTf00U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1956 */  { fnJMUSERmode_g,              256+0,                       "",                                            "Kg00U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1957 */  { fnJMUSERmode,                256+1,                       "",                                            "USER_PRIM01U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1958 */  { fnJMUSERmode_f,              256+1,                       "",                                            "USER_SFTf01U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1959 */  { fnJMUSERmode_g,              256+1,                       "",                                            "Kg01U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1960 */  { fnJMUSERmode,                256+2,                       "",                                            "USER_PRIM02U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1961 */  { fnJMUSERmode_f,              256+2,                       "",                                            "USER_SFTf02U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1962 */  { fnJMUSERmode_g,              256+2,                       "",                                            "Kg02U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1963 */  { fnJMUSERmode,                256+3,                       "",                                            "USER_PRIM03U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1964 */  { fnJMUSERmode_f,              256+3,                       "",                                            "USER_SFTf03U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1965 */  { fnJMUSERmode_g,              256+3,                       "",                                            "Kg03U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1966 */  { fnJMUSERmode,                256+4,                       "",                                            "USER_PRIM04U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1967 */  { fnJMUSERmode_f,              256+4,                       "",                                            "USER_SFTf04U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1968 */  { fnJMUSERmode_g,              256+4,                       "",                                            "Kg04U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1969 */  { fnJMUSERmode,                256+5,                       "",                                            "USER_PRIM05U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1970 */  { fnJMUSERmode_f,              256+5,                       "",                                            "USER_SFTf05U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1971 */  { fnJMUSERmode_g,              256+5,                       "",                                            "Kg05U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1972 */  { fnJMUSERmode,                256+6,                       "",                                            "USER_PRIM06U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1973 */  { fnJMUSERmode_f,              256+6,                       "",                                            "USER_SFTf06U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1974 */  { fnJMUSERmode_g,              256+6,                       "",                                            "Kg06U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1975 */  { fnJMUSERmode,                256+7,                       "",                                            "USER_PRIM07U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1976 */  { fnJMUSERmode_f,              256+7,                       "",                                            "USER_SFTf07U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1977 */  { fnJMUSERmode_g,              256+7,                       "",                                            "Kg07U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1978 */  { fnJMUSERmode,                256+8,                       "",                                            "USER_PRIM08U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1979 */  { fnJMUSERmode_f,              256+8,                       "",                                            "USER_SFTf08U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1980 */  { fnJMUSERmode_g,              256+8,                       "",                                            "Kg08U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1981 */  { fnJMUSERmode,                256+9,                       "",                                            "USER_PRIM09U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1982 */  { fnJMUSERmode_f,              256+9,                       "",                                            "USER_SFTf09U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1983 */  { fnJMUSERmode_g,              256+9,                       "",                                            "Kg09U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1984 */  { fnJMUSERmode,                256+10,                      "",                                            "USER_PRIM10U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1985 */  { fnJMUSERmode_f,              256+10,                      "",                                            "USER_SFTf10U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1986 */  { fnJMUSERmode_g,              256+10,                      "",                                            "Kg10U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1987 */  { fnJMUSERmode,                256+11,                      "",                                            "USER_PRIM11U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1988 */  { fnJMUSERmode_f,              256+11,                      "",                                            "USER_SFTf11U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1989 */  { fnJMUSERmode_g,              256+11,                      "",                                            "Kg11U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1990 */  { fnJMUSERmode,                256+12,                      "",                                            "USER_PRIM12U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1991 */  { fnJMUSERmode_f,              256+12,                      "",                                            "USER_SFTf12U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1992 */  { fnJMUSERmode_g,              256+12,                      "",                                            "Kg12U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1993 */  { fnJMUSERmode,                256+13,                      "",                                            "USER_PRIM13U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1994 */  { fnJMUSERmode_f,              256+13,                      "",                                            "USER_SFTf13U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1995 */  { fnJMUSERmode_g,              256+13,                      "",                                            "Kg13U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1996 */  { fnJMUSERmode,                256+14,                      "",                                            "USER_PRIM14U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1997 */  { fnJMUSERmode_f,              256+14,                      "",                                            "USER_SFTf14U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1998 */  { fnJMUSERmode_g,              256+14,                      "",                                            "Kg14U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 1999 */  { fnJMUSERmode,                256+15,                      "",                                            "USER_PRIM15U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2000 */  { fnJMUSERmode_f,              256+15,                      "",                                            "USER_SFTf15U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2001 */  { fnJMUSERmode_g,              256+15,                      "",                                            "Kg15U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2002 */  { fnJMUSERmode,                256+16,                      "",                                            "USER_PRIM16U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2003 */  { fnJMUSERmode_f,              256+16,                      "",                                            "USER_SFTf16U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2004 */  { fnJMUSERmode_g,              256+16,                      "",                                            "Kg16U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2005 */  { fnJMUSERmode,                256+17,                      "",                                            "USER_PRIM17U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2006 */  { fnJMUSERmode_f,              256+17,                      "",                                            "USER_SFTf17U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2007 */  { fnJMUSERmode_g,              256+17,                      "",                                            "Kg17U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2008 */  { fnJMUSERmode,                256+18,                      "",                                            "USER_PRIM18U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2009 */  { fnJMUSERmode_f,              256+18,                      "",                                            "USER_SFTf18U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2010 */  { fnJMUSERmode_g,              256+18,                      "",                                            "Kg18U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2011 */  { fnJMUSERmode,                256+19,                      "",                                            "USER_PRIM19U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2012 */  { fnJMUSERmode_f,              256+19,                      "",                                            "USER_SFTf19U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2013 */  { fnJMUSERmode_g,              256+19,                      "",                                            "Kg19U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2014 */  { fnJMUSERmode,                256+20,                      "",                                            "USER_PRIM20U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2015 */  { fnJMUSERmode_f,              256+20,                      "",                                            "USER_SFTf20U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2016 */  { fnJMUSERmode_g,              256+20,                      "",                                            "Kg20U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2017 */  { fnJMUSERmode,                256+21,                      "",                                            "USER_PRIM21U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2018 */  { fnJMUSERmode_f,              256+21,                      "",                                            "USER_SFTf21U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2019 */  { fnJMUSERmode_g,              256+21,                      "",                                            "Kg21U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2020 */  { fnJMUSERmode,                256+22,                      "",                                            "USER_PRIM22U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2021 */  { fnJMUSERmode_f,              256+22,                      "",                                            "USER_SFTf22U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2022 */  { fnJMUSERmode_g,              256+22,                      "",                                            "Kg22U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2023 */  { fnJMUSERmode,                256+23,                      "",                                            "USER_PRIM23U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2024 */  { fnJMUSERmode_f,              256+23,                      "",                                            "USER_SFTf23U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2025 */  { fnJMUSERmode_g,              256+23,                      "",                                            "Kg23U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2026 */  { fnJMUSERmode,                256+24,                      "",                                            "USER_PRIM24U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2027 */  { fnJMUSERmode_f,              256+24,                      "",                                            "USER_SFTf24U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2028 */  { fnJMUSERmode_g,              256+24,                      "",                                            "Kg24U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2029 */  { fnJMUSERmode,                256+25,                      "",                                            "USER_PRIM25U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2030 */  { fnJMUSERmode_f,              256+25,                      "",                                            "USER_SFTf25U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2031 */  { fnJMUSERmode_g,              256+25,                      "",                                            "Kg25U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2032 */  { fnJMUSERmode,                256+26,                      "",                                            "USER_PRIM26U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2033 */  { fnJMUSERmode_f,              256+26,                      "",                                            "USER_SFTf26U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2034 */  { fnJMUSERmode_g,              256+26,                      "",                                            "Kg26U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2035 */  { fnJMUSERmode,                256+27,                      "",                                            "USER_PRIM27U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2036 */  { fnJMUSERmode_f,              256+27,                      "",                                            "USER_SFTf27U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2037 */  { fnJMUSERmode_g,              256+27,                      "",                                            "Kg27U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2038 */  { fnJMUSERmode,                256+28,                      "",                                            "USER_PRIM28U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2039 */  { fnJMUSERmode_f,              256+28,                      "",                                            "USER_SFTf28U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2040 */  { fnJMUSERmode_g,              256+28,                      "",                                            "Kg28U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2041 */  { fnJMUSERmode,                256+29,                      "",                                            "USER_PRIM29U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2042 */  { fnJMUSERmode_f,              256+29,                      "",                                            "USER_SFTf29U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2043 */  { fnJMUSERmode_g,              256+29,                      "",                                            "Kg29U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2044 */  { fnJMUSERmode,                256+30,                      "",                                            "USER_PRIM30U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2045 */  { fnJMUSERmode_f,              256+30,                      "",                                            "USER_SFTf30U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2046 */  { fnJMUSERmode_g,              256+30,                      "",                                            "Kg30U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2047 */  { fnJMUSERmode,                256+31,                      "",                                            "USER_PRIM31U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2048 */  { fnJMUSERmode_f,              256+31,                      "",                                            "USER_SFTf31U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2049 */  { fnJMUSERmode_g,              256+31,                      "",                                            "Kg31U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2050 */  { fnJMUSERmode,                256+32,                      "",                                            "USER_PRIM32U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2051 */  { fnJMUSERmode_f,              256+32,                      "",                                            "USER_SFTf32U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2052 */  { fnJMUSERmode_g,              256+32,                      "",                                            "Kg32U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2053 */  { fnJMUSERmode,                256+33,                      "",                                            "USER_PRIM33U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2054 */  { fnJMUSERmode_f,              256+33,                      "",                                            "USER_SFTf33U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2055 */  { fnJMUSERmode_g,              256+33,                      "",                                            "Kg33U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2056 */  { fnJMUSERmode,                256+34,                      "",                                            "USER_PRIM34U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2057 */  { fnJMUSERmode_f,              256+34,                      "",                                            "USER_SFTf34U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2058 */  { fnJMUSERmode_g,              256+34,                      "",                                            "Kg34U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2059 */  { fnJMUSERmode,                256+35,                      "",                                            "USER_PRIM35U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2060 */  { fnJMUSERmode_f,              256+35,                      "",                                            "USER_SFTf35U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2061 */  { fnJMUSERmode_g,              256+35,                      "",                                            "Kg35U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2062 */  { fnJMUSERmode,                256+36,                      "",                                            "USER_PRIM36U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2063 */  { fnJMUSERmode_f,              256+36,                      "",                                            "USER_SFTf36U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2064 */  { fnJMUSERmode_g,              256+36,                      "",                                            "Kg36U",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM User mode (Will remove later - reserved)
/* 2065 */  { fnXEQMENU,                   1,                           "XEQM01",                                      "XEQM01",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EXEC
/* 2066 */  { fnXEQMENU,                   2,                           "XEQM02",                                      "XEQM02",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EXEC
/* 2067 */  { fnXEQMENU,                   3,                           "XEQM03",                                      "XEQM03",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EXEC
/* 2068 */  { fnXEQMENU,                   4,                           "XEQM04",                                      "XEQM04",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EXEC
/* 2069 */  { fnXEQMENU,                   5,                           "XEQM05",                                      "XEQM05",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EXEC
/* 2070 */  { fnXEQMENU,                   6,                           "XEQM06",                                      "XEQM06",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EXEC
/* 2071 */  { fnXEQMENU,                   7,                           "XEQM07",                                      "XEQM07",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EXEC
/* 2072 */  { fnXEQMENU,                   8,                           "XEQM08",                                      "XEQM08",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EXEC
/* 2073 */  { fnXEQMENU,                   9,                           "XEQM09",                                      "XEQM09",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EXEC
/* 2074 */  { fnXEQMENU,                   10,                          "XEQM10",                                      "XEQM10",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EXEC
/* 2075 */  { fnXEQMENU,                   11,                          "XEQM11",                                      "XEQM11",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EXEC
/* 2076 */  { fnXEQMENU,                   12,                          "XEQM12",                                      "XEQM12",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EXEC
/* 2077 */  { fnXEQMENU,                   13,                          "XEQM13",                                      "XEQM13",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EXEC
/* 2078 */  { fnXEQMENU,                   14,                          "XEQM14",                                      "XEQM14",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EXEC
/* 2079 */  { fnXEQMENU,                   15,                          "XEQM15",                                      "XEQM15",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EXEC
/* 2080 */  { fnXEQMENU,                   16,                          "XEQM16",                                      "XEQM16",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EXEC
/* 2081 */  { fnXEQMENU,                   17,                          "XEQM17",                                      "XEQM17",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EXEC
/* 2082 */  { fnXEQMENU,                   18,                          "XEQM18",                                      "XEQM18",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM EXEC
/* 2083 */  { fnXEQMSAVE,                  TM_VALUE,                    "X.SAVE",                                      "X.SAVE",                                      1,      18,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 2084 */  { fnXEQMLOAD,                  TM_VALUE,                    "X.LOAD",                                      "X.LOAD",                                      1,      18,       CAT_NONE, SLS_UNCHANGED, US_ENABLED  },
/* 2085 */  { fnFb,                        0,                           "FB00",                                        "FB00",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2086 */  { fnFb,                        1,                           "FB01",                                        "FB01",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2087 */  { fnFb,                        2,                           "FB02",                                        "FB02",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2088 */  { fnFb,                        3,                           "FB03",                                        "FB03",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2089 */  { fnFb,                        4,                           "FB04",                                        "FB04",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2090 */  { fnFb,                        5,                           "FB05",                                        "FB05",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2091 */  { fnFb,                        6,                           "FB06",                                        "FB06",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2092 */  { fnFb,                        7,                           "FB07",                                        "FB07",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2093 */  { fnFb,                        8,                           "FB08",                                        "FB08",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2094 */  { fnFb,                        9,                           "FB09",                                        "FB09",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2095 */  { fnFb,                        10,                          "FB10",                                        "FB10",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2096 */  { fnFb,                        11,                          "FB11",                                        "FB11",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2097 */  { fnFb,                        12,                          "FB12",                                        "FB12",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2098 */  { fnFb,                        13,                          "FB13",                                        "FB13",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2099 */  { fnFb,                        14,                          "FB14",                                        "FB14",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2100 */  { fnFb,                        15,                          "FB15",                                        "FB15",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2101 */  { fnFb,                        16,                          "FB16",                                        "FB16",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2102 */  { fnFb,                        17,                          "FB17",                                        "FB17",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2103 */  { fnFb,                        18,                          "FB18",                                        "FB18",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2104 */  { fnFb,                        19,                          "FB19",                                        "FB19",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2105 */  { fnFb,                        20,                          "FB20",                                        "FB20",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2106 */  { fnFb,                        21,                          "FB21",                                        "FB21",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2107 */  { fnFb,                        22,                          "FB22",                                        "FB22",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2108 */  { fnFb,                        23,                          "FB23",                                        "FB23",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2109 */  { fnFb,                        24,                          "FB24",                                        "FB24",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2110 */  { fnFb,                        25,                          "FB25",                                        "FB25",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2111 */  { fnFb,                        26,                          "FB26",                                        "FB26",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2112 */  { fnFb,                        27,                          "FB27",                                        "FB27",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2113 */  { fnFb,                        28,                          "FB28",                                        "FB28",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2114 */  { fnFb,                        29,                          "FB29",                                        "FB29",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2115 */  { fnFb,                        30,                          "FB30",                                        "FB30",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2116 */  { fnFb,                        31,                          "FB31",                                        "FB31",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2117 */  { fnFb,                        32,                          "FB32",                                        "FB32",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2118 */  { fnFb,                        33,                          "FB33",                                        "FB33",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2119 */  { fnFb,                        34,                          "FB34",                                        "FB34",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2120 */  { fnFb,                        35,                          "FB35",                                        "FB35",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2121 */  { fnFb,                        36,                          "FB36",                                        "FB36",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2122 */  { fnFb,                        37,                          "FB37",                                        "FB37",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2123 */  { fnFb,                        38,                          "FB38",                                        "FB38",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2124 */  { fnFb,                        39,                          "FB39",                                        "FB39",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2125 */  { fnFb,                        40,                          "FB40",                                        "FB40",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2126 */  { fnFb,                        41,                          "FB41",                                        "FB41",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2127 */  { fnFb,                        42,                          "FB42",                                        "FB42",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2128 */  { fnFb,                        43,                          "FB43",                                        "FB43",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2129 */  { fnFb,                        44,                          "FB44",                                        "FB44",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2130 */  { fnFb,                        45,                          "FB45",                                        "FB45",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2131 */  { fnFb,                        46,                          "FB46",                                        "FB46",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2132 */  { fnFb,                        47,                          "FB47",                                        "FB47",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2133 */  { fnFb,                        48,                          "FB48",                                        "FB48",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2134 */  { fnFb,                        49,                          "FB49",                                        "FB49",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2135 */  { fnFb,                        50,                          "FB50",                                        "FB50",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2136 */  { fnFb,                        51,                          "FB51",                                        "FB51",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2137 */  { fnFb,                        52,                          "FB52",                                        "FB52",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2138 */  { fnFb,                        53,                          "FB53",                                        "FB53",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2139 */  { fnFb,                        54,                          "FB54",                                        "FB54",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2140 */  { fnFb,                        55,                          "FB55",                                        "FB55",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2141 */  { fnFb,                        56,                          "FB56",                                        "FB56",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2142 */  { fnFb,                        57,                          "FB57",                                        "FB57",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2143 */  { fnFb,                        58,                          "FB58",                                        "FB58",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2144 */  { fnFb,                        59,                          "FB59",                                        "FB59",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2145 */  { fnFb,                        60,                          "FB60",                                        "FB60",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2146 */  { fnFb,                        61,                          "FB61",                                        "FB61",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2147 */  { fnFb,                        62,                          "FB62",                                        "FB62",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2148 */  { fnFb,                        63,                          "FB63",                                        "FB63",                                        0,       0,       CAT_NONE, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2149 */  { fnByteShortcutsS,            6,                           "S06",                                         "S06",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2150 */  { fnByteShortcutsS,            8,                           "S08",                                         "S08",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2151 */  { fnByteShortcutsS,            16,                          "S16",                                         "S16",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2152 */  { fnByteShortcutsS,            32,                          "S32",                                         "S32",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2153 */  { fnByteShortcutsS,            64,                          "S64",                                         "S64",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2154 */  { fnByteShortcutsU,            6,                           "U06",                                         "U06",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2155 */  { fnByteShortcutsU,            8,                           "U08",                                         "U08",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2156 */  { fnByteShortcutsU,            16,                          "U16",                                         "U16",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2157 */  { fnByteShortcutsU,            32,                          "U32",                                         "U32",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2158 */  { fnByteShortcutsU,            64,                          "U64",                                         "U64",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2159 */  { fnByte,                      1,                           "SL1",                                         "SL1",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2160 */  { fnByte,                      2,                           "SR1",                                         "SR1",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2161 */  { fnByte,                      3,                           "RL1",                                         "RL1",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2162 */  { fnByte,                      4,                           "RR1",                                         "RR1",                                         0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2163 */  { fnByte,                      5,                           "SWAP.W",                                      "SWAP.W",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2164 */  { fnByte,                      6,                           "SWAP.B",                                      "SWAP.B",                                      0,       0,       CAT_FNCT, SLS_ENABLED  , US_ENABLED  },//JM SHOI
/* 2165 */  { fnClAIM,                     NOPARAM,                     "EXITCLR",                                     "EXITCLR",                                     0,       0,       CAT_NONE, SLS_UNCHANGED, US_ENABLED  },//clear HEX mode
/* 2166 */  { fnShoiXRepeats,              TM_VALUE,                    "DISP_SI",                                     "DISP_SI",                                     0,       3,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM SHOI
/* 2167 */  { fnScale,                     NOPARAM,                     "TICKS",                                       "TICKS",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM GRAPHING
/* 2168 */  { fnPlotLS,                    NOPARAM,                     "PLOTLS",                                      "PLOTLS",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},
/* 2169 */  { fnPline,                     NOPARAM,                     "LINE",                                        "LINE",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//GRAPH
/* 2170 */  { fnPcros,                     NOPARAM,                     "CROSS",                                       "CROSS",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//GRAPH
/* 2171 */  { fnPbox,                      NOPARAM,                     "BOX",                                         "BOX",                                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//GRAPH
/* 2172 */  { fnPvect,                     NOPARAM,                     "VECT",                                        "VECT",                                        0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM VECTOR MODE
/* 2173 */  { fnPNvect,                    NOPARAM,                     "N.VECT",                                      "N.VECT",                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM VECTOR MODE
/* 2174 */  { fnPx,                        NOPARAM,                     "MAGN.X",                                      "MAGN.X",                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 2175 */  { fnPy,                        NOPARAM,                     "MAGN.Y",                                      "MAGN.Y",                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
/* 2176 */  { fnGraph,                     4,                           "GRF",                                         "GRF",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},
/* 2177 */  { fnGraph,                     1,                           "PLTGRF",                                      "PLTGRF",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED, US_UNCHANGED},//JM GRAPHING
/* 2178 */  { fnGraph,                     11,                          "DEMO1",                                       "DEMO1",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_UNCHANGED},
/* 2179 */  { fnGraph,                     12,                          "DEMO2",                                       "DEMO2",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_UNCHANGED},
/* 2180 */  { fnGraph,                     13,                          "DEMO3",                                       "DEMO3",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_UNCHANGED},
/* 2181 */  { fnGraph,                     14,                          "DEMO4",                                       "DEMO4",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_UNCHANGED},
/* 2182 */  { fnGraph,                     15,                          "DEMO5",                                       "DEMO5",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_UNCHANGED},
/* 2183 */  { fnGraph,                     16,                          "DEMO6",                                       "DEMO6",                                       0,       0,       CAT_FNCT, SLS_ENABLED  , US_UNCHANGED},
/* 2184 */  { fnPintg,                     NOPARAM,                     STD_SIGMA STD_y_BAR STD_DELTA "x",             STD_SIGMA STD_y_BAR STD_DELTA "x",             0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//GRAPH
/* 2185 */  { fnPdiff,                     NOPARAM,                     STD_DELTA "y/" STD_DELTA "x",                  STD_DELTA "y/" STD_DELTA "x",                  0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//GRAPH
/* 2186 */  { fnPrms,                      NOPARAM,                     "RMS",                                         "RMS",                                         0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//GRAPH
/* 2187 */  { fnPshade,                    NOPARAM,                     "SHADE",                                       "SHADE",                                       0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//GRAPH
/* 2188 */  { fnCase,                      3,                           "",                                            STD_num,                                      0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},//JM CASE

/* 2189 */  { itemToBeCoded,               NOPARAM,                     "",                                            "Last item",                                   0,       0,       CAT_NONE, SLS_UNCHANGED, US_UNCHANGED},
};
