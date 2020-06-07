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
void fnLnP1                     (uint16_t unusedParamButMandatory) {}
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
void fnSwapX                    (uint16_t unusedParamButMandatory) {}
void fnSwapY                    (uint16_t unusedParamButMandatory) {}
void fnSwapZ                    (uint16_t unusedParamButMandatory) {}
void fnSwapT                    (uint16_t unusedParamButMandatory) {}
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
#endif

const item_t indexOfItems[] = {

//            function                     parameter                    item in catalog                                item in softmenu                               TAM min  TAM max  CATALOG   stackLift
/*    0 */  { itemToBeCoded,               NOPARAM,                     "",                                            "0000",                                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/*    1 */  { fnCvtCToF,                   NOPARAM,                     STD_DEGREE "C" STD_RIGHT_ARROW STD_DEGREE "F", STD_DEGREE "C" STD_RIGHT_ARROW STD_DEGREE "F", 0,       0,       CAT_FNCT, SLS_ENABLED  },
/*    2 */  { fnCvtFToC,                   NOPARAM,                     STD_DEGREE "F" STD_RIGHT_ARROW STD_DEGREE "C", STD_DEGREE "F" STD_RIGHT_ARROW STD_DEGREE "C", 0,       0,       CAT_FNCT, SLS_ENABLED  },
/*    3 */  { fn10Pow,                     NOPARAM,                     "10" STD_SUP_x,                                "10" STD_SUP_x,                                0,       0,       CAT_FNCT, SLS_ENABLED  },
/*    4 */  { fnIntegerMode,               SIM_1COMPL,                  "1COMPL",                                      "1COMPL",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*    5 */  { fnScreenDump,                NOPARAM,                     "SNAP",                                        "SNAP",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*    6 */  { fnInvert,                    NOPARAM,                     "1/x",                                         "1/x",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*    7 */  { fnIntegerMode,               SIM_2COMPL,                  "2COMPL",                                      "2COMPL",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*    8 */  { fn2Pow,                      NOPARAM,                     "2" STD_SUP_x,                                 "2" STD_SUP_x,                                 0,       0,       CAT_FNCT, SLS_ENABLED  },
/*    9 */  { fnCubeRoot,                  NOPARAM,                     STD_CUBE_ROOT STD_x_UNDER_ROOT,                STD_CUBE_ROOT STD_x_UNDER_ROOT,                0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   10 */  { itemToBeCoded,               NOPARAM,                     "A",                                           "A",                                           0,       0,       CAT_RVAR, SLS_UNCHANGED},
/*   11 */  { fnConstant,                  0,                           "a",                                           "a",                                           0,       0,       CAT_CNST, SLS_ENABLED  },
/*   12 */  { fnConstant,                  1,                           "a" STD_SUB_0,                                 "a" STD_SUB_0,                                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*   13 */  { fnMagnitude,                 NOPARAM,                     "ABS",                                         "ABS",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   14 */  { itemToBeCoded,               NOPARAM,                     "ACC",                                         "ACC",                                         0,       0,       CAT_RVAR, SLS_UNCHANGED},
/*   15 */  { fnCvtAcreM2,                 multiply,                    "ac" STD_RIGHT_ARROW "m" STD_SUP_2,            "acre",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   16 */  { fnCvtAcreusM2,               multiply,                    "ac" STD_US STD_RIGHT_ARROW "m" STD_SUP_2,     "acre" STD_US,                                 0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   17 */  { itemToBeCoded,               NOPARAM,                     "ADV",                                         "ADV",                                         0,       0,       CAT_MENU, SLS_UNCHANGED},
/*   18 */  { fnAgm,                       NOPARAM,                     "AGM",                                         "AGM",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   19 */  { itemToBeCoded,               NOPARAM,                     "AGRAPH",                                      "AGRAPH",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   20 */  { fnDisplayFormatAll,          TM_VALUE,                    "ALL" ,                                        "ALL",                                         0,      15,       CAT_FNCT, SLS_UNCHANGED},
/*   21 */  { fnConstant,                  2,                           "a" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "a" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   0,       0,       CAT_CNST, SLS_ENABLED  },
/*   22 */  { fnLogicalAnd,                NOPARAM,                     "AND",                                         "AND",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   23 */  { itemToBeCoded,               NOPARAM,                     "ANGLES",                                      "ANGLES",                                      0,       0,       CAT_MENU, SLS_UNCHANGED},
/*   24 */  { fnArccos,                    NOPARAM,                     "arccos",                                      "arccos",                                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   25 */  { fnArccosh,                   NOPARAM,                     "arcosh",                                      "arcosh",                                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   26 */  { fnArcsin,                    NOPARAM,                     "arcsin",                                      "arcsin",                                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   27 */  { fnArctan,                    NOPARAM,                     "arctan",                                      "arctan",                                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   28 */  { fnArcsinh,                   NOPARAM,                     "arsinh",                                      "arsinh",                                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   29 */  { fnArctanh,                   NOPARAM,                     "artanh",                                      "artanh",                                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   30 */  { fnAsr,                       TM_VALUE,                    "ASR",                                         "ASR",                                         0,      63,       CAT_FNCT, SLS_ENABLED  },
/*   31 */  { itemToBeCoded,               NOPARAM,                     "ASSIGN",                                      "ASN",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   32 */  { fnCvtAtmPa,                  multiply,                    "atm" STD_RIGHT_ARROW "Pa",                    "atm" STD_RIGHT_ARROW "Pa",                    0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   33 */  { fnCvtAuM,                    multiply,                    "au" STD_RIGHT_ARROW "m",                      "au" STD_RIGHT_ARROW "m",                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   34 */  { itemToBeCoded,               NOPARAM,                     "PRINT",                                       "PRINT",                                       0,       0,       CAT_MENU, SLS_UNCHANGED},
/*   35 */  { itemToBeCoded,               NOPARAM,                     "A:",                                          "A:",                                          0,       0,       CAT_MENU, SLS_UNCHANGED},
/*   36 */  { fnConstant,                  3,                           "a" STD_SUB_EARTH,                             "a" STD_SUB_EARTH,                             0,       0,       CAT_CNST, SLS_ENABLED  },
/*   37 */  { itemToBeCoded,               NOPARAM,                     "B",                                           "B",                                           0,       0,       CAT_RVAR, SLS_UNCHANGED},
/*   38 */  { itemToBeCoded,               NOPARAM,                     "BACK",                                        "BACK",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   39 */  { fnCvtBarPa,                  multiply,                    "bar" STD_RIGHT_ARROW "Pa",                    "bar" STD_RIGHT_ARROW "Pa",                    0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   40 */  { fnBatteryVoltage,            NOPARAM,                     "BATT?",                                       "BATT?",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   41 */  { fnBc,                        TM_VALUE,                    "BC?",                                         "BC?",                                         1,      64,       CAT_FNCT, SLS_ENABLED  },
/*   42 */  { itemToBeCoded,               NOPARAM,                     "BEEP",                                        "BEEP",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   43 */  { itemToBeCoded,               NOPARAM,                     "BeginP",                                      "Begin",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   44 */  { fnCurveFitting,              CF_BEST_FITTING,             "BestF",                                       "BestF",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   45 */  { itemToBeCoded,               NOPARAM,                     "Binom" STD_SUB_p,                             "Binom" STD_SUB_p,                             0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   46 */  { itemToBeCoded,               NOPARAM,                     "Binom" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Binom" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   47 */  { itemToBeCoded,               NOPARAM,                     "Binom" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Binom" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   48 */  { itemToBeCoded,               NOPARAM,                     "Binom" STD_SUP_MINUS_1,                       "Binom" STD_SUP_MINUS_1,                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   49 */  { itemToBeCoded,               NOPARAM,                     "Binom:",                                      "Binom:",                                      0,       0,       CAT_MENU, SLS_UNCHANGED},
/*   50 */  { itemToBeCoded,               NOPARAM,                     "BITS",                                        "BITS",                                        0,       0,       CAT_MENU, SLS_UNCHANGED},
/*   51 */  { itemToBeCoded,               NOPARAM,                     "B" STD_SUB_n,                                 "B" STD_SUB_n,                                 0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   52 */  { itemToBeCoded,               NOPARAM,                     "B" STD_SUB_n STD_SUP_ASTERISK,                "B" STD_SUB_n STD_SUP_ASTERISK,                0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   53 */  { fnBs,                        TM_VALUE,                    "BS?",                                         "BS?",                                         1,      64,       CAT_FNCT, SLS_ENABLED  },
/*   54 */  { fnCvtBtuJ,                   multiply,                    "Btu" STD_RIGHT_ARROW "J",                     "Btu" STD_RIGHT_ARROW "J",                     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   55 */  { itemToBeCoded,               NOPARAM,                     "C",                                           "C",                                           0,       0,       CAT_RVAR, SLS_UNCHANGED},
/*   56 */  { fnConstant,                  4,                           "c",                                           "c",                                           0,       0,       CAT_CNST, SLS_ENABLED  },
/*   57 */  { fnConstant,                  5,                           "c" STD_SUB_1,                                 "c" STD_SUB_1,                                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*   58 */  { fnConstant,                  6,                           "c" STD_SUB_2,                                 "c" STD_SUB_2,                                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*   59 */  { fnCvtCalJ,                   multiply,                    "cal" STD_RIGHT_ARROW "J",                     "cal" STD_RIGHT_ARROW "J",                     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   60 */  { itemToBeCoded,               NOPARAM,                     "CASE",                                        "CASE",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   61 */  { itemToBeCoded,               NOPARAM,                     "CATALOG",                                     "CATALOG",                                     0,       0,       CAT_MENU, SLS_UNCHANGED},
/*   62 */  { itemToBeCoded,               NOPARAM,                     "Cauch" STD_SUB_p,                             "Cauch" STD_SUB_p,                             0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   63 */  { itemToBeCoded,               NOPARAM,                     "Cauch" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Cauch" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   64 */  { itemToBeCoded,               NOPARAM,                     "Cauch" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Cauch" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   65 */  { itemToBeCoded,               NOPARAM,                     "Cauch" STD_SUP_MINUS_1,                       "Cauch" STD_SUP_MINUS_1,                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   66 */  { itemToBeCoded,               NOPARAM,                     "Cauch:",                                      "Cauch:",                                      0,       0,       CAT_MENU, SLS_UNCHANGED},
/*   67 */  { fnCb,                        TM_VALUE,                    "CB",                                          "CB",                                          1,      64,       CAT_FNCT, SLS_ENABLED  },
/*   68 */  { fnCeil,                      NOPARAM,                     "CEIL",                                        "CEIL",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   69 */  { fnClearFlag,                 TM_FLAGW,                    "CF",                                          "CF",                                          0,      99,       CAT_FNCT, SLS_UNCHANGED},
/*   70 */  { itemToBeCoded,               NOPARAM,                     "CHARS",                                       "CHARS",                                       0,       0,       CAT_MENU, SLS_UNCHANGED},
/*   71 */  { fnClAll,                     NOT_CONFIRMED,               "CLALL",                                       "CLall",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   72 */  { itemToBeCoded,               NOPARAM,                     "CLCVAR",                                      "CLCVAR",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   73 */  { fnClFAll,                    NOPARAM,                     "CLFALL",                                      "CLFall",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   74 */  { itemToBeCoded,               NOPARAM,                     "CLK",                                         "CLK",                                         0,       0,       CAT_MENU, SLS_UNCHANGED},
/*   75 */  { fnFractionType,              NOPARAM,                     "a b/c",                                       "a b/c",                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/*   76 */  { itemToBeCoded,               NOPARAM,                     "REGIST",                                      "REGIST",                                      0,       0,       CAT_MENU, SLS_UNCHANGED},
/*   77 */  { itemToBeCoded,               NOPARAM,                     "CLLCD",                                       "CLLCD",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   78 */  { itemToBeCoded,               NOPARAM,                     "CLMENU",                                      "CLMENU",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   79 */  { itemToBeCoded,               NOPARAM,                     "CLP",                                         "CLP",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   80 */  { fnClPAll,                    NOT_CONFIRMED,               "CLPALL",                                      "CLPall",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   81 */  { itemToBeCoded,               NOPARAM,                     "CLR",                                         "CLR",                                         0,       0,       CAT_MENU, SLS_UNCHANGED},
/*   82 */  { fnClearRegisters,            NOPARAM,                     "CLREGS",                                      "CLREGS",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   83 */  { fnClearStack,                NOPARAM,                     "CLSTK",                                       "CLSTK",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   84 */  { fnClX,                       NOPARAM,                     "CLX",                                         "CLX",                                         0,       0,       CAT_FNCT, SLS_DISABLED },
/*   85 */  { fnClSigma,                   NOPARAM,                     "CL" STD_SIGMA,                                "CL" STD_SIGMA,                                0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   86 */  { itemToBeCoded,               NOPARAM,                     "CONST",                                       "CONST",                                       0,       0,       CAT_MENU, SLS_UNCHANGED},
/*   87 */  { fnCyx,                       NOPARAM,                     "COMB",                                        "C" STD_SUB_y STD_SUB_x,                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   88 */  { fnConjugate,                 NOPARAM,                     "CONJ",                                        "conj",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   89 */  { fnConstant,                  TM_VALUE,                    "CNST",                                        "CNST",                                        0,      99,       CAT_FNCT, SLS_ENABLED  },
/*   90 */  { itemToBeCoded,               NOPARAM,                     "CONVG?",                                      "CONVG?",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   91 */  { itemToBeCoded,               NOPARAM,                     "CORR",                                        "r",                                           0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   92 */  { fnCos,                       NOPARAM,                     "cos",                                         "cos",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   93 */  { fnCosh,                      NOPARAM,                     "cosh",                                        "cosh",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*   94 */  { itemToBeCoded,               NOPARAM,                     "COV",                                         "cov",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*   95 */  { itemToBeCoded,               NOPARAM,                     "CPX",                                         "CPX",                                         0,       0,       CAT_MENU, SLS_UNCHANGED},
/*   96 */  { itemToBeCoded,               NOPARAM,                     "0096",                                        "0096",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*   97 */  { itemToBeCoded,               NOPARAM,                     "0097",                                        "0097",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*   98 */  { itemToBeCoded,               NOPARAM,                     "0098",                                        "0098",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*   99 */  { itemToBeCoded,               NOPARAM,                     "CPXS",                                        "CPXS",                                        0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  100 */  { itemToBeCoded,               NOPARAM,                     "CPX?",                                        "CPX?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  101 */  { fnCross,                     NOPARAM,                     "CROSS",                                       "cross",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  102 */  { fnCvtCwtKg,                  multiply,                    "cwt" STD_RIGHT_ARROW "kg",                    "cwt" STD_RIGHT_ARROW "kg",                    0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  103 */  { fnCxToRe,                    NOPARAM,                     "CX" STD_RIGHT_ARROW "RE",                     "CX" STD_RIGHT_ARROW "RE",                     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  104 */  { itemToBeCoded,               NOPARAM,                     "D",                                           "D",                                           0,       0,       CAT_RVAR, SLS_UNCHANGED},
/*  105 */  { itemToBeCoded,               NOPARAM,                     "DATE",                                        "DATE",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  106 */  { itemToBeCoded,               NOPARAM,                     "DATES",                                       "DATES",                                       0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  107 */  { itemToBeCoded,               NOPARAM,                     "DATE" STD_RIGHT_ARROW,                        "DATE" STD_RIGHT_ARROW,                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  108 */  { itemToBeCoded,               NOPARAM,                     "DAY",                                         "DAY",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  109 */  { itemToBeCoded,               NOPARAM,                     "DBL?",                                        "DBL?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  110 */  { itemToBeCoded,               NOPARAM,                     "DBLR",                                        "DBLR",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  111 */  { itemToBeCoded,               NOPARAM,                     "DBL" STD_CROSS,                               "DBL" STD_CROSS,                               0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  112 */  { itemToBeCoded,               NOPARAM,                     "DBL/",                                        "DBL/",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  113 */  { fnCvtDbRatio,                20,                          "dB" STD_RIGHT_ARROW "fr",                     "dB " STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  114 */  { fnCvtDbRatio,                10,                          "dB" STD_RIGHT_ARROW "pr",                     "dB " STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  115 */  { fnDec,                       TM_REGISTER,                 "DEC",                                         "DEC",                                         0,      99,       CAT_FNCT, SLS_ENABLED  },
/*  116 */  { fnDecomp,                    NOPARAM,                     "DECOMP",                                      "DECOMP",                                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  117 */  { fnAngularMode,               AM_DEGREE,                   "DEG",                                         "DEG",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  118 */  { fnCvtToCurrentAngularMode,   AM_DEGREE,                   "DEG" STD_RIGHT_ARROW,                         "DEG" STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  119 */  { itemToBeCoded,               NOPARAM,                     "0119",                                        "0119",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  120 */  { itemToBeCoded,               NOPARAM,                     "0120",                                        "0120",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  121 */  { itemToBeCoded,               NOPARAM,                     "0121",                                        "0121",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  122 */  { fnDenMax,                    NOPARAM,                     "DENMAX",                                      "DENMAX",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  123 */  { itemToBeCoded,               NOPARAM,                     "DIGITS",                                      "DIGITS",                                      0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  124 */  { itemToBeCoded,               NOPARAM,                     "DISP",                                        "DISP",                                        0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  125 */  { fnDot,                       NOPARAM,                     "DOT",                                         "dot",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  126 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "DP",                          STD_RIGHT_ARROW "DP",                          0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  127 */  { fnDrop,                      NOPARAM,                     "DROP",                                        "DROP" STD_DOWN_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  128 */  { fnDropY,                     NOPARAM,                     "DROPy",                                       "DROPy",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  129 */  { itemToBeCoded,               NOPARAM,                     "DSE",                                         "DSE",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  130 */  { itemToBeCoded,               NOPARAM,                     "DSL",                                         "DSL",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  131 */  { fnConstant,                  59,                          STD_DELTA STD_nu STD_SUB_C STD_SUB_s,          STD_DELTA STD_nu STD_SUB_C STD_SUB_s,          0,       0,       CAT_CNST, SLS_ENABLED  },
/*  132 */  { fnDisplayStack,              TM_VALUE,                    "DSTACK",                                      "DSTACK",                                      1,       4,       CAT_FNCT, SLS_UNCHANGED},
/*  133 */  { itemToBeCoded,               NOPARAM,                     "DSZ",                                         "DSZ",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  134 */  { fnAngularMode,               AM_DMS,                      "D.MS",                                        "d.ms",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  135 */  { fnCvtToCurrentAngularMode,   AM_DMS,                      "D.MS" STD_RIGHT_ARROW,                        "D.MS" STD_RIGHT_ARROW,                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  136 */  { fnSetDateFormat,             ITM_DMY,                     "D.MY",                                        "D.MY",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  137 */  { itemToBeCoded,               NOPARAM,                     "D" STD_RIGHT_ARROW "J",                       "D" STD_RIGHT_ARROW "J",                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  138 */  { fnCvtDegToRad,               NOPARAM,                     "D" STD_RIGHT_ARROW "R",                       "D" STD_RIGHT_ARROW "R",                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  139 */  { fnConstant,                  7,                           "e",                                           "e",                                           0,       0,       CAT_CNST, SLS_ENABLED  },
/*  140 */  { fnConstant,                  8,                           "e" STD_SUB_E,                                 "e" STD_SUB_E,                                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*  141 */  { itemToBeCoded,               NOPARAM,                     "EIGVAL",                                      "EIGVAL",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  142 */  { itemToBeCoded,               NOPARAM,                     "EIGVEC",                                      "EIGVEC",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  143 */  { itemToBeCoded,               NOPARAM,                     "END",                                         "END",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  144 */  { itemToBeCoded,               NOPARAM,                     "ENDP",                                        "End",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  145 */  { fnDisplayFormatEng,          TM_VALUE,                    "ENG",                                         "ENG",                                         0,      15,       CAT_FNCT, SLS_UNCHANGED},
/*  146 */  { itemToBeCoded,               NOPARAM,                     "0146",                                        "0146",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  147 */  { itemToBeCoded,               NOPARAM,                     "ENORM",                                       "ENORM",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  148 */  { fnKeyEnter,                  NOPARAM,                     "ENTER" STD_UP_ARROW,                          "ENTER" STD_UP_ARROW,                          0,       0,       CAT_FNCT, SLS_DISABLED },
/*  149 */  { itemToBeCoded,               NOPARAM,                     "ENTRY?",                                      "ENTRY?",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  150 */  { itemToBeCoded,               NOPARAM,                     "EQN",                                         "EQN",                                         0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  151 */  { itemToBeCoded,               NOPARAM,                     "EQ.DEL",                                      "DELETE",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  152 */  { itemToBeCoded,               NOPARAM,                     "EQ.EDI",                                      "EDIT",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  153 */  { itemToBeCoded,               NOPARAM,                     "EQ.NEW",                                      "NEW",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  154 */  { itemToBeCoded,               NOPARAM,                     "erf",                                         "erf",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  155 */  { itemToBeCoded,               NOPARAM,                     "erfc",                                        "erfc",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  156 */  { itemToBeCoded,               NOPARAM,                     "ERR",                                         "ERR",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  157 */  { itemToBeCoded,               NOPARAM,                     "EVEN?",                                       "EVEN?",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  158 */  { fnExp,                       NOPARAM,                     "e" STD_SUP_x,                                 "e" STD_SUP_x,                                 0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  159 */  { itemToBeCoded,               NOPARAM,                     "EXITALL",                                     "EXITall",                                     0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  160 */  { itemToBeCoded,               NOPARAM,                     "EXP",                                         "EXP",                                         0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  161 */  { fnCurveFitting,              CF_EXPONENTIAL_FITTING,      "ExpF",                                        "ExpF",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  162 */  { itemToBeCoded,               NOPARAM,                     "Expon" STD_SUB_p,                             "Expon" STD_SUB_p,                             0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  163 */  { itemToBeCoded,               NOPARAM,                     "Expon" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Expon" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  164 */  { itemToBeCoded,               NOPARAM,                     "Expon" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Expon" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  165 */  { itemToBeCoded,               NOPARAM,                     "Expon" STD_SUP_MINUS_1,                       "Expon" STD_SUP_MINUS_1,                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  166 */  { itemToBeCoded,               NOPARAM,                     "Expon:",                                      "Expon:",                                      0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  167 */  { fnExpt,                      NOPARAM,                     "EXPT",                                        "EXPT",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  168 */  { fnExpM1,                     NOPARAM,                     "e" STD_SUP_x "-1",                            "e" STD_SUP_x "-1",                            0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  169 */  { itemToBeCoded,               NOPARAM,                     "e/m" STD_SUB_e,                               "e/m" STD_SUB_e,                               0,       0,       CAT_NONE, SLS_ENABLED  },
/*  170 */  { itemToBeCoded,               NOPARAM,                     "E:",                                          "E:",                                          0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  171 */  { fnConstant,                  9,                           "F",                                           "F",                                           0,       0,       CAT_CNST, SLS_ENABLED  },
/*  172 */  { itemToBeCoded,               NOPARAM,                     "FAST",                                        "FAST",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  173 */  { fnFb,                        TM_VALUE,                    "FB",                                          "FB",                                          1,      64,       CAT_FNCT, SLS_ENABLED  },
/*  174 */  { itemToBeCoded,               NOPARAM,                     "FCNS",                                        "FCNS",                                        0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  175 */  { fnIsFlagClear,               TM_FLAGR,                    "FC?",                                         "FC?",                                         0,      99,       CAT_FNCT, SLS_UNCHANGED},
/*  176 */  { fnIsFlagClearClear,          TM_FLAGW,                    "FC?C",                                        "FC?C",                                        0,      99,       CAT_FNCT, SLS_UNCHANGED},
/*  177 */  { fnIsFlagClearFlip,           TM_FLAGW,                    "FC?F",                                        "FC?F",                                        0,      99,       CAT_FNCT, SLS_UNCHANGED},
/*  178 */  { fnIsFlagClearSet,            TM_FLAGW,                    "FC?S",                                        "FC?S",                                        0,      99,       CAT_FNCT, SLS_UNCHANGED},
/*  179 */  { fnCvtFtM,                    multiply,                    "ft." STD_RIGHT_ARROW "m",                     "ft." STD_RIGHT_ARROW "m",                     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  180 */  { fnFlipFlag,                  TM_FLAGW,                    "FF",                                          "FF",                                          0,      99,       CAT_FNCT, SLS_ENABLED  },
/*  181 */  { itemToBeCoded,               NOPARAM,                     "FIB",                                         "FIB",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  182 */  { fnFillStack,                 NOPARAM,                     "FILL",                                        "FILL",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  183 */  { itemToBeCoded,               NOPARAM,                     "FIN",                                         "FIN",                                         0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  184 */  { itemToBeCoded,               NOPARAM,                     "S.INTS",                                      "S.INTS",                                      0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  185 */  { fnDisplayFormatFix,          TM_VALUE,                    "FIX",                                         "FIX",                                         0,      15,       CAT_FNCT, SLS_UNCHANGED},
/*  186 */  { itemToBeCoded,               NOPARAM,                     "FLAGS",                                       "FLAGS",                                       0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  187 */  { itemToBeCoded,               NOPARAM,                     "FLASH",                                       "FLASH",                                       0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  188 */  { fnFreeFlashMemory,           NOPARAM,                     "FLASH?",                                      "FLASH?",                                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  189 */  { fnFloor,                     NOPARAM,                     "FLOOR",                                       "FLOOR",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  190 */  { fnFp,                        NOPARAM,                     "FP",                                          "FP",                                          0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  191 */  { itemToBeCoded,               NOPARAM,                     "FP?",                                         "FP?",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  192 */  { itemToBeCoded,               NOPARAM,                     "F" STD_SUB_p "(x)",                           "F" STD_SUB_p "(x)",                           0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  193 */  { itemToBeCoded,               NOPARAM,                     "F" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", "F" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", 0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  194 */  { itemToBeCoded,               NOPARAM,                     "F" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", "F" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", 0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  195 */  { itemToBeCoded,               NOPARAM,                     "F" STD_SUP_MINUS_1 "(p)",                     "F" STD_SUP_MINUS_1 "(p)",                     0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  196 */  { fnCvtRatioDb,                20,                          "fr" STD_RIGHT_ARROW "dB",                     "field",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  197 */  { fnIsFlagSet,                 TM_FLAGR,                    "FS?",                                         "FS?",                                         0,      99,       CAT_FNCT, SLS_UNCHANGED},
/*  198 */  { fnIsFlagSetClear,            TM_FLAGW,                    "FS?C",                                        "FS?C",                                        0,      99,       CAT_FNCT, SLS_UNCHANGED},
/*  199 */  { fnIsFlagSetFlip,             TM_FLAGW,                    "FS?F",                                        "FS?F",                                        0,      99,       CAT_FNCT, SLS_UNCHANGED},
/*  200 */  { fnIsFlagSetSet,              TM_FLAGW,                    "FS?S",                                        "FS?S",                                        0,      99,       CAT_FNCT, SLS_UNCHANGED},
/*  201 */  { fnCvtSfeetM,                 multiply,                    "ft" STD_US STD_RIGHT_ARROW "m",               "survey",                                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  202 */  { itemToBeCoded,               NOPARAM,                     "FV",                                          "FV",                                          0,       0,       CAT_RVAR, SLS_UNCHANGED},
/*  203 */  { fnCvtFlozukM3,               multiply,                    "fz" STD_UK STD_RIGHT_ARROW "m" STD_SUP_3,     "floz" STD_UK,                                 0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  204 */  { fnCvtFlozusM3,               multiply,                    "fz" STD_US STD_RIGHT_ARROW "m" STD_SUP_3,     "floz" STD_US,                                 0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  205 */  { fnConstant,                  10,                          "F" STD_SUB_alpha,                             "F" STD_SUB_alpha,                             0,       0,       CAT_CNST, SLS_ENABLED  },
/*  206 */  { fnConstant,                  11,                          "F" STD_SUB_delta,                             "F" STD_SUB_delta,                             0,       0,       CAT_CNST, SLS_ENABLED  },
/*  207 */  { itemToBeCoded,               NOPARAM,                     "F:",                                          "F:",                                          0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  208 */  { itemToBeCoded,               NOPARAM,                     "f'",                                          "f'",                                          0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  209 */  { itemToBeCoded,               NOPARAM,                     "f\"",                                         "f\"",                                         0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  210 */  { itemToBeCoded,               NOPARAM,                     "f'(x)",                                       "f'(x)",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  211 */  { itemToBeCoded,               NOPARAM,                     "f\"(x)",                                      "f\"(x)",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  212 */  { itemToBeCoded,               NOPARAM,                     "F&p:",                                        "F&p:",                                        0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  213 */  { fnConstant,                  12,                          "G",                                           "G",                                           0,       0,       CAT_CNST, SLS_ENABLED  },
/*  214 */  { fnConstant,                  13,                          "G" STD_SUB_0,                                 "G" STD_SUB_0,                                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*  215 */  { fnDisplayFormatGap,          TM_VALUE,                    "GAP",                                         "GAP",                                         0,      15,       CAT_FNCT, SLS_UNCHANGED},
/*  216 */  { fnConstant,                  14,                          "G" STD_SUB_C,                                 "G" STD_SUB_C,                                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*  217 */  { fnGcd,                       NOPARAM,                     "GCD",                                         "GCD",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  218 */  { itemToBeCoded,               NOPARAM,                     "g" STD_SUB_d,                                 "g" STD_SUB_d,                                 0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  219 */  { itemToBeCoded,               NOPARAM,                     "g" STD_SUB_d STD_SUP_MINUS_1,                 "g" STD_SUB_d STD_SUP_MINUS_1,                 0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  220 */  { fnConstant,                  15,                          "g" STD_SUB_e,                                 "g" STD_SUB_e,                                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*  221 */  { itemToBeCoded,               NOPARAM,                     "Geom" STD_SUB_p,                              "Geom" STD_SUB_p,                              0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  222 */  { itemToBeCoded,               NOPARAM,                     "Geom" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,    "Geom" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,    0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  223 */  { itemToBeCoded,               NOPARAM,                     "Geom" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,    "Geom" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,    0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  224 */  { itemToBeCoded,               NOPARAM,                     "Geom" STD_SUP_MINUS_1,                        "Geom" STD_SUP_MINUS_1,                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  225 */  { itemToBeCoded,               NOPARAM,                     "Geom:",                                       "Geom:",                                       0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  226 */  { fnCvtGalukM3,                multiply,                    "gl" STD_UK STD_RIGHT_ARROW "m" STD_SUP_3,     "gl" STD_UK STD_RIGHT_ARROW "m" STD_SUP_3,     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  227 */  { fnCvtGalusM3,                multiply,                    "gl" STD_US STD_RIGHT_ARROW "m" STD_SUP_3,     "gl" STD_US STD_RIGHT_ARROW "m" STD_SUP_3,     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  228 */  { fnConstant,                  16,                          "GM" STD_SUB_EARTH,                            "GM" STD_SUB_EARTH,                            0,       0,       CAT_CNST, SLS_ENABLED  },
/*  229 */  { fnAngularMode,               AM_GRAD,                     "GRAD",                                        "GRAD",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  230 */  { fnCvtToCurrentAngularMode,   AM_GRAD,                     "GRAD" STD_RIGHT_ARROW,                        "GRAD" STD_RIGHT_ARROW,                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  231 */  { itemToBeCoded,               NOPARAM,                     "GTO",                                         "GTO",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  232 */  { itemToBeCoded,               NOPARAM,                     "GTO.",                                        "GTO.",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  233 */  { fnConstant,                  17,                          "g" STD_SUB_EARTH,                             "g" STD_SUB_EARTH,                             0,       0,       CAT_CNST, SLS_ENABLED  },
/*  234 */  { fnConstant,                  18,                          STD_PLANCK,                                    STD_PLANCK,                                    0,       0,       CAT_CNST, SLS_ENABLED  },
/*  235 */  { itemToBeCoded,               NOPARAM,                     "H" STD_SUB_n,                                 "H" STD_SUB_n,                                 0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  236 */  { itemToBeCoded,               NOPARAM,                     "H" STD_SUB_n STD_SUB_P,                       "H" STD_SUB_n STD_SUB_P,                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  237 */  { fnCvtHpeW,                   multiply,                    "hp" STD_SUB_E STD_RIGHT_ARROW "W",            "hp" STD_SUB_E STD_RIGHT_ARROW "W",            0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  238 */  { fnCvtHpmW,                   multiply,                    "hp" STD_SUB_M STD_RIGHT_ARROW "W",            "hp" STD_SUB_M STD_RIGHT_ARROW "W",            0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  239 */  { fnCvtHpukW,                  multiply,                    "hp" STD_UK STD_RIGHT_ARROW "W",               "hp" STD_UK STD_RIGHT_ARROW "W",               0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  240 */  { itemToBeCoded,               NOPARAM,                     "Hyper" STD_SUB_p,                             "Hyper" STD_SUB_p,                             0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  241 */  { itemToBeCoded,               NOPARAM,                     "Hyper" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Hyper" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  242 */  { itemToBeCoded,               NOPARAM,                     "Hyper" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Hyper" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  243 */  { itemToBeCoded,               NOPARAM,                     "Hyper" STD_SUP_MINUS_1,                       "Hyper" STD_SUP_MINUS_1,                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  244 */  { itemToBeCoded,               NOPARAM,                     "Hyper:",                                      "Hyper:",                                      0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  245 */  { fnConstant,                  19,                          STD_PLANCK_2PI,                                STD_PLANCK_2PI,                                0,       0,       CAT_CNST, SLS_ENABLED  },
/*  246 */  { addItemToBuffer,             REGISTER_I,                  "I",                                           "I",                                           0,       0,       CAT_RVAR, SLS_UNCHANGED},
/*  247 */  { fnIDiv,                      NOPARAM,                     "IDIV",                                        "IDIV",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  248 */  { fnCvtInhgPa,                 multiply,                    "iHg" STD_RIGHT_ARROW "Pa",                    "in.Hg",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  249 */  { itemToBeCoded,               NOPARAM,                     "L.INTS",                                      "L.INTS",                                      0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  250 */  { fnImaginaryPart,             NOPARAM,                     "Im",                                          "Im",                                          0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  251 */  { itemToBeCoded,               NOPARAM,                     "0251",                                        "0251",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  252 */  { fnInc,                       TM_REGISTER,                 "INC",                                         "INC",                                         0,      99,       CAT_FNCT, SLS_ENABLED  },
/*  253 */  { itemToBeCoded,               NOPARAM,                     "INDEX",                                       "INDEX",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  254 */  { itemToBeCoded,               NOPARAM,                     "INFO",                                        "INFO",                                        0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  255 */  { itemToBeCoded,               NOPARAM,                     "INPUT",                                       "INPUT",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  256 */  { itemToBeCoded,               NOPARAM,                     "INTS",                                        "INTS",                                        0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  257 */  { itemToBeCoded,               NOPARAM,                     "INT?",                                        "INT?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  258 */  { fnCvtInchM,                  multiply,                    "in." STD_RIGHT_ARROW "m",                     "in." STD_RIGHT_ARROW "m",                     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  259 */  { fnIp,                        NOPARAM,                     "IP",                                          "IP",                                          0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  260 */  { itemToBeCoded,               NOPARAM,                     "ISE",                                         "ISE",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  261 */  { itemToBeCoded,               NOPARAM,                     "ISG",                                         "ISG",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  262 */  { itemToBeCoded,               NOPARAM,                     "ISZ",                                         "ISZ",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  263 */  { itemToBeCoded,               NOPARAM,                     "I" STD_SUB_x STD_SUB_y STD_SUB_z,             "I" STD_SUB_x STD_SUB_y STD_SUB_z,             0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  264 */  { itemToBeCoded,               NOPARAM,                     "I" STD_GAMMA STD_SUB_p,                       "I" STD_GAMMA STD_SUB_p,                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  265 */  { itemToBeCoded,               NOPARAM,                     "I" STD_GAMMA STD_SUB_q,                       "I" STD_GAMMA STD_SUB_q,                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  266 */  { itemToBeCoded,               NOPARAM,                     "I+",                                          "I+",                                          0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  267 */  { itemToBeCoded,               NOPARAM,                     "I-",                                          "I-",                                          0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  268 */  { itemToBeCoded,               NOPARAM,                     "I/O",                                         "I/O",                                         0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  269 */  { itemToBeCoded,               NOPARAM,                     "i%/a",                                        "i%/a",                                        0,       0,       CAT_RVAR, SLS_UNCHANGED},
/*  270 */  { addItemToBuffer,             REGISTER_J,                  "J",                                           "J",                                           0,       0,       CAT_RVAR, SLS_UNCHANGED},
/*  271 */  { itemToBeCoded,               NOPARAM,                     "J" STD_SUB_y "(x)",                           "J" STD_SUB_y "(x)",                           0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  272 */  { itemToBeCoded,               NOPARAM,                     "J+",                                          "J+",                                          0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  273 */  { itemToBeCoded,               NOPARAM,                     "J-",                                          "J-",                                          0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  274 */  { itemToBeCoded,               NOPARAM,                     "J/G",                                         "J/G",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  275 */  { fnCvtBtuJ,                   divide,                      "J" STD_RIGHT_ARROW "Btu",                     "J" STD_RIGHT_ARROW "Btu",                     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  276 */  { fnCvtCalJ,                   divide,                      "J" STD_RIGHT_ARROW "cal",                     "J" STD_RIGHT_ARROW "cal",                     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  277 */  { itemToBeCoded,               NOPARAM,                     "J" STD_RIGHT_ARROW "D",                       "J" STD_RIGHT_ARROW "D",                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  278 */  { fnCvtWhJ,                    divide,                      "J" STD_RIGHT_ARROW "Wh",                      "J" STD_RIGHT_ARROW "Wh",                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  279 */  { addItemToBuffer,             REGISTER_K,                  "K",                                           "K",                                           0,       0,       CAT_RVAR, SLS_UNCHANGED},
/*  280 */  { fnConstant,                  20,                          "k",                                           "k",                                           0,       0,       CAT_CNST, SLS_ENABLED  },
/*  281 */  { itemToBeCoded,               NOPARAM,                     "KEY",                                         "KEY",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  282 */  { itemToBeCoded,               NOPARAM,                     "KEYG",                                        "KEYG",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  283 */  { itemToBeCoded,               NOPARAM,                     "KEYX",                                        "KEYX",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  284 */  { itemToBeCoded,               NOPARAM,                     "KEY?",                                        "KEY?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  285 */  { fnCvtCwtKg,                  divide,                      "kg" STD_RIGHT_ARROW "cwt",                    "kg" STD_RIGHT_ARROW "cwt",                    0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  286 */  { fnCvtLbKg,                   divide,                      "kg" STD_RIGHT_ARROW "lb.",                    "kg" STD_RIGHT_ARROW "lb.",                    0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  287 */  { fnCvtOzKg,                   divide,                      "kg" STD_RIGHT_ARROW "oz",                     "kg" STD_RIGHT_ARROW "oz",                     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  288 */  { fnCvtShortcwtKg,             divide,                      "kg" STD_RIGHT_ARROW "scw",                    "kg " STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  289 */  { fnCvtStoneKg,                divide,                      "kg" STD_RIGHT_ARROW "sto",                    "kg " STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  290 */  { fnCvtShorttonKg,             divide,                      "kg" STD_RIGHT_ARROW "s.t",                    "kg " STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  291 */  { fnCvtTonKg,                  divide,                      "kg" STD_RIGHT_ARROW "ton",                    "kg" STD_RIGHT_ARROW "ton",                    0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  292 */  { fnCvtTrozKg,                 divide,                      "kg" STD_RIGHT_ARROW "trz",                    "kg " STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  293 */  { fnConstant,                  21,                          "K" STD_SUB_J,                                 "K" STD_SUB_J,                                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*  294 */  { itemToBeCoded,               NOPARAM,                     "KTYP?",                                       "KTYP?",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  295 */  { addItemToBuffer,             REGISTER_L,                  "L",                                           "L",                                           0,       0,       CAT_RVAR, SLS_UNCHANGED},
/*  296 */  { fnLastX,                     NOPARAM,                     "LASTx",                                       "LASTx",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  297 */  { fnCvtLbfN,                   multiply,                    "lbf" STD_RIGHT_ARROW "N",                     "lbf" STD_RIGHT_ARROW "N",                     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  298 */  { itemToBeCoded,               NOPARAM,                     "LBL",                                         "LBL",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  299 */  { itemToBeCoded,               NOPARAM,                     "LBL?",                                        "LBL?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  300 */  { fnCvtLbKg,                   multiply,                    "lb." STD_RIGHT_ARROW "kg",                    "lb." STD_RIGHT_ARROW "kg",                    0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  301 */  { fnLcm,                       NOPARAM,                     "LCM",                                         "LCM",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  302 */  { itemToBeCoded,               NOPARAM,                     "LEAP?",                                       "LEAP?",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  303 */  { itemToBeCoded,               NOPARAM,                     "LgNrm" STD_SUB_p,                             "LgNrm" STD_SUB_p,                             0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  304 */  { itemToBeCoded,               NOPARAM,                     "LgNrm" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "LgNrm" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  305 */  { itemToBeCoded,               NOPARAM,                     "LgNrm" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "LgNrm" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  306 */  { itemToBeCoded,               NOPARAM,                     "LgNrm" STD_SUP_MINUS_1,                       "LgNrm" STD_SUP_MINUS_1,                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  307 */  { itemToBeCoded,               NOPARAM,                     "LgNrm:",                                      "LgNrm:",                                      0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  308 */  { fnCurveFitting,              CF_LINEAR_FITTING,           "LinF",                                        "LinF",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  309 */  { fnLj,                        NOPARAM,                     "LJ",                                          "LJ",                                          0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  310 */  { fnLn,                        NOPARAM,                     "LN",                                          "ln",                                          0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  311 */  { itemToBeCoded,               NOPARAM,                     "L" STD_SUB_m ,                                "L" STD_SUB_m ,                                0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  312 */  { fnLnP1,                      NOPARAM,                     "LN(1+x)",                                     "ln 1+x",                                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  313 */  { itemToBeCoded,               NOPARAM,                     "L" STD_SUB_m STD_SUB_alpha,                   "L" STD_SUB_m STD_SUB_alpha,                   0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  314 */  { itemToBeCoded,               NOPARAM,                     "LN" STD_beta,                                 "ln" STD_beta,                                 0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  315 */  { fnLnGamma,                   NOPARAM,                     "LN" STD_GAMMA,                                "ln" STD_GAMMA,                                0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  316 */  { itemToBeCoded,               NOPARAM,                     "LOAD",                                        "LOAD",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  317 */  { itemToBeCoded,               NOPARAM,                     "LOADP",                                       "LOADP",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  318 */  { itemToBeCoded,               NOPARAM,                     "LOADR",                                       "LOADR",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  319 */  { itemToBeCoded,               NOPARAM,                     "LOADSS",                                      "LOADSS",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  320 */  { itemToBeCoded,               NOPARAM,                     "LOAD" STD_SIGMA,                              "LOAD" STD_SIGMA,                              0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  321 */  { allocateLocalRegisters,      TM_VALUE,                    "LocR",                                        "LocR",                                        0,      99,       CAT_FNCT, SLS_UNCHANGED},
/*  322 */  { fnGetLocR,                   NOPARAM,                     "LocR?",                                       "LocR?",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  323 */  { fnLog10,                     NOPARAM,                     "LOG" STD_SUB_10,                              "lg",                                          0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  324 */  { fnLog2,                      NOPARAM,                     "LOG" STD_SUB_2,                               "lb x",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  325 */  { fnCurveFitting,              CF_LOGARITHMIC_FITTING,      "LogF",                                        "LogF",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  326 */  { itemToBeCoded,               NOPARAM,                     "Logis" STD_SUB_p,                             "Logis" STD_SUB_p,                             0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  327 */  { itemToBeCoded,               NOPARAM,                     "Logis" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Logis" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  328 */  { itemToBeCoded,               NOPARAM,                     "Logis" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Logis" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  329 */  { itemToBeCoded,               NOPARAM,                     "Logis" STD_SUP_MINUS_1,                       "Logis" STD_SUP_MINUS_1,                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  330 */  { itemToBeCoded,               NOPARAM,                     "Logis:",                                      "Logis:",                                      0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  331 */  { itemToBeCoded,               NOPARAM,                     "LOG" STD_SUB_x "y",                           "log" STD_SUB_x "y",                           0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  332 */  { itemToBeCoded,               NOPARAM,                     "LOOP",                                        "LOOP",                                        0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  333 */  { fnConstant,                  22,                          "l" STD_SUB_P STD_SUB_L,                       "l" STD_SUB_P STD_SUB_L,                       0,       0,       CAT_CNST, SLS_ENABLED  },
/*  334 */  { fnCvtLyM,                    multiply,                    "ly" STD_RIGHT_ARROW "m",                      "ly" STD_RIGHT_ARROW "m",                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  335 */  { itemToBeCoded,               NOPARAM,                     "0335",                                        "0335",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  336 */  { itemToBeCoded,               NOPARAM,                     "0336",                                        "0336",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  337 */  { itemToBeCoded,               NOPARAM,                     "L.R.",                                        "L.R.",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  338 */  { fnCvtAcreM2,                 divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ac",            "m" STD_SUP_2 " " STD_RIGHT_ARROW,             0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  339 */  { fnCvtAcreusM2,               divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ac" STD_US,     "m" STD_SUP_2 " " STD_RIGHT_ARROW,             0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  340 */  { fnCvtFlozukM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "fz" STD_UK,     "m" STD_SUP_3 " " STD_RIGHT_ARROW,             0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  341 */  { fnCvtFlozusM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "fz" STD_US,     "m" STD_SUP_3 " " STD_RIGHT_ARROW,             0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  342 */  { fnCvtGalukM3,                divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_UK,     "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_UK,     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  343 */  { fnCvtGalusM3,                divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_US,     "m" STD_SUP_3 STD_RIGHT_ARROW "gl" STD_US,     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  344 */  { fnMant,                      NOPARAM,                     "MANT",                                        "MANT",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  345 */  { fnMaskl,                     TM_VALUE,                    "MASKL",                                       "MASKL",                                       0,      64,       CAT_FNCT, SLS_ENABLED  },
/*  346 */  { fnMaskr,                     TM_VALUE,                    "MASKR",                                       "MASKR",                                       0,      64,       CAT_FNCT, SLS_ENABLED  },
/*  347 */  { itemToBeCoded,               NOPARAM,                     "MATRS",                                       "MATRS",                                       0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  348 */  { itemToBeCoded,               NOPARAM,                     "MATR?",                                       "MATR?",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  349 */  { itemToBeCoded,               NOPARAM,                     "MATX",                                        "MATX",                                        0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  350 */  { itemToBeCoded,               NOPARAM,                     "Mat_A",                                       "Mat A",                                       0,       0,       CAT_RVAR, SLS_UNCHANGED},
/*  351 */  { itemToBeCoded,               NOPARAM,                     "Mat_B",                                       "Mat B",                                       0,       0,       CAT_RVAR, SLS_UNCHANGED},
/*  352 */  { itemToBeCoded,               NOPARAM,                     "Mat_X",                                       "Mat X",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  353 */  { fnMax,                       NOPARAM,                     "max",                                         "max",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  354 */  { fnConstant,                  23,                          "m" STD_SUB_e,                                 "m" STD_SUB_e,                                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*  355 */  { fnFreeMemory,                NOPARAM,                     "MEM?",                                        "MEM?",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  356 */  { itemToBeCoded,               NOPARAM,                     "MENU",                                        "MENU",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  357 */  { itemToBeCoded,               NOPARAM,                     "MENUS",                                       "MENUS",                                       0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  358 */  { fnMin,                       NOPARAM,                     "min",                                         "min",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  359 */  { fnMirror,                    NOPARAM,                     "MIRROR",                                      "MIRROR",                                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  360 */  { fnCvtMiM,                    multiply,                    "mi." STD_RIGHT_ARROW "m",                     "mi." STD_RIGHT_ARROW "m",                     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  361 */  { fnConstant,                  24,                          "M" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "M" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   0,       0,       CAT_CNST, SLS_ENABLED  },
/*  362 */  { fnConstant,                  25,                          "m" STD_SUB_n,                                 "m" STD_SUB_n,                                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*  363 */  { fnConstant,                  26,                          "m" STD_SUB_n "/m" STD_SUB_p,                  "m" STD_SUB_n "/m" STD_SUB_p,                  0,       0,       CAT_CNST, SLS_ENABLED  },
/*  364 */  { fnMod,                       NOPARAM,                     "MOD",                                         "MOD",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  365 */  { itemToBeCoded,               NOPARAM,                     "MODE",                                        "MODE",                                        0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  366 */  { itemToBeCoded,               NOPARAM,                     "MONTH",                                       "MONTH",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  367 */  { fnConstant,                  27,                          "m" STD_SUB_p,                                 "m" STD_SUB_p,                                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*  368 */  { fnConstant,                  28,                          "m" STD_SUB_P STD_SUB_L,                       "m" STD_SUB_P STD_SUB_L,                       0,       0,       CAT_CNST, SLS_ENABLED  },
/*  369 */  { fnConstant,                  29,                          "m" STD_SUB_p "/m" STD_SUB_e,                  "m" STD_SUB_p "/m" STD_SUB_e,                  0,       0,       CAT_CNST, SLS_ENABLED  },
/*  370 */  { itemToBeCoded,               NOPARAM,                     "MSG",                                         "MSG",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  371 */  { fnConstant,                  30,                          "m" STD_SUB_u,                                 "m" STD_SUB_u,                                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*  372 */  { fnConstant,                  31,                          "m" STD_SUB_u "c" STD_SUP_2,                   "m" STD_SUB_u "c" STD_SUP_2,                   0,       0,       CAT_CNST, SLS_ENABLED  },
/*  373 */  { itemToBeCoded,               NOPARAM,                     "0373",                                        "0373",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  374 */  { itemToBeCoded,               NOPARAM,                     "0374",                                        "0374",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  375 */  { fnAngularMode,               AM_MULTPI,                   "MUL" STD_pi,                                  "MUL" STD_pi,                                  0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  376 */  { itemToBeCoded,               NOPARAM,                     "MVAR",                                        "MVAR",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  377 */  { itemToBeCoded,               NOPARAM,                     "MyMenu",                                      "MyMenu",                                      0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  378 */  { itemToBeCoded,               NOPARAM,                     "My" STD_alpha,                                "My" STD_alpha,                                0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  379 */  { fnConstant,                  32,                          "m" STD_SUB_mu,                                "m" STD_SUB_mu,                                0,       0,       CAT_CNST, SLS_ENABLED  },
/*  380 */  { itemToBeCoded,               NOPARAM,                     "M.DELR",                                      "DELR",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  381 */  { itemToBeCoded,               NOPARAM,                     "M.DIM",                                       "DIM",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  382 */  { itemToBeCoded,               NOPARAM,                     "M.DIM?",                                      "DIM?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  383 */  { fnSetDateFormat,             ITM_MDY,                     "M.DY",                                        "M.DY",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  384 */  { itemToBeCoded,               NOPARAM,                     "M.EDI",                                       "EDIT",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  385 */  { itemToBeCoded,               NOPARAM,                     "M.EDIN",                                      "EDITN",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  386 */  { itemToBeCoded,               NOPARAM,                     "M.EDIT",                                      "EDIT",                                        0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  387 */  { itemToBeCoded,               NOPARAM,                     "M.GET",                                       "GETM",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  388 */  { itemToBeCoded,               NOPARAM,                     "M.GOTO",                                      "GOTO",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  389 */  { itemToBeCoded,               NOPARAM,                     "M.GROW",                                      "GROW",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  390 */  { itemToBeCoded,               NOPARAM,                     "M.INSR",                                      "INSR",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  391 */  { itemToBeCoded,               NOPARAM,                     "M.LU",                                        "M.LU",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  392 */  { itemToBeCoded,               NOPARAM,                     "M.NEW",                                       "NEW",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  393 */  { itemToBeCoded,               NOPARAM,                     "M.OLD",                                       "OLD",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  394 */  { itemToBeCoded,               NOPARAM,                     "M.PUT",                                       "PUTM",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  395 */  { itemToBeCoded,               NOPARAM,                     "M.R" STD_LEFT_RIGHT_ARROWS "R",               "R" STD_LEFT_RIGHT_ARROWS "R",                 0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  396 */  { itemToBeCoded,               NOPARAM,                     "M.SIMQ",                                      "M.SIMQ",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  397 */  { itemToBeCoded,               NOPARAM,                     "M.SQR?",                                      "M.SQR?",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  398 */  { itemToBeCoded,               NOPARAM,                     "M.WRAP",                                      "WRAP",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  399 */  { itemToBeCoded,               NOPARAM,                     "m:",                                          "m:",                                          0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  400 */  { fnCvtAuM,                    divide,                      "m" STD_RIGHT_ARROW "au",                      "m" STD_RIGHT_ARROW "au",                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  401 */  { fnCvtFtM,                    divide,                      "m" STD_RIGHT_ARROW "ft.",                     "m" STD_RIGHT_ARROW "ft.",                     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  402 */  { fnCvtSfeetM,                 divide,                      "m" STD_RIGHT_ARROW "ft" STD_US,               "m " STD_RIGHT_ARROW,                          0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  403 */  { fnCvtInchM,                  divide,                      "m" STD_RIGHT_ARROW "in.",                     "m" STD_RIGHT_ARROW "in.",                     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  404 */  { fnCvtLyM,                    divide,                      "m" STD_RIGHT_ARROW "ly",                      "m" STD_RIGHT_ARROW "ly",                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  405 */  { fnCvtMiM,                    divide,                      "m" STD_RIGHT_ARROW "mi.",                     "m" STD_RIGHT_ARROW "mi.",                     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  406 */  { fnCvtNmiM,                   divide,                      "m" STD_RIGHT_ARROW "nmi.",                    "m" STD_RIGHT_ARROW "nmi.",                    0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  407 */  { fnCvtPcM,                    divide,                      "m" STD_RIGHT_ARROW "pc",                      "m" STD_RIGHT_ARROW "pc",                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  408 */  { fnCvtPointM,                 divide,                      "m" STD_RIGHT_ARROW "pt.",                     "point",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  409 */  { fnCvtYardM,                  divide,                      "m" STD_RIGHT_ARROW "yd.",                     "m" STD_RIGHT_ARROW "yd.",                     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  410 */  { fnConstant,                  33,                          "M" STD_SUB_SUN,                               "M" STD_SUB_SUN,                               0,       0,       CAT_CNST, SLS_ENABLED  },
/*  411 */  { fnConstant,                  34,                          "M" STD_SUB_EARTH,                             "M" STD_SUB_EARTH,                             0,       0,       CAT_CNST, SLS_ENABLED  },
/*  412 */  { fnConstant,                  35,                          "N" STD_SUB_A,                                 "N" STD_SUB_A,                                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*  413 */  { fnConstant,                  36,                          "NaN",                                         "NaN",                                         0,       0,       CAT_CNST, SLS_ENABLED  },
/*  414 */  { fnLogicalNand,               NOPARAM,                     "NAND",                                        "NAND",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  415 */  { itemToBeCoded,               NOPARAM,                     "NaN?",                                        "NaN?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  416 */  { itemToBeCoded,               NOPARAM,                     "NBin" STD_SUB_p,                              "NBin" STD_SUB_p,                              0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  417 */  { itemToBeCoded,               NOPARAM,                     "NBin" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,    "NBin" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,    0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  418 */  { itemToBeCoded,               NOPARAM,                     "NBin" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,    "NBin" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,    0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  419 */  { itemToBeCoded,               NOPARAM,                     "NBin" STD_SUP_MINUS_1,                        "NBin" STD_SUP_MINUS_1,                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  420 */  { itemToBeCoded,               NOPARAM,                     "NBin:",                                       "NBin:",                                       0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  421 */  { fnNeighb,                    NOPARAM,                     "NEIGHB",                                      "NEIGHB",                                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  422 */  { fnNextPrime,                 NOPARAM,                     "NEXTP",                                       "NEXTP",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  423 */  { fnCvtNmiM,                   multiply,                    "nmi." STD_RIGHT_ARROW "m",                    "nmi." STD_RIGHT_ARROW "m",                    0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  424 */  { fnNop,                       NOPARAM,                     "NOP",                                         "NOP",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  425 */  { fnLogicalNor,                NOPARAM,                     "NOR",                                         "NOR",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  426 */  { itemToBeCoded,               NOPARAM,                     "Norml" STD_SUB_p,                             "Norml" STD_SUB_p,                             0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  427 */  { itemToBeCoded,               NOPARAM,                     "Norml" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Norml" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  428 */  { itemToBeCoded,               NOPARAM,                     "Norml" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Norml" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  429 */  { itemToBeCoded,               NOPARAM,                     "Norml" STD_SUP_MINUS_1,                       "Norml" STD_SUP_MINUS_1,                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  430 */  { itemToBeCoded,               NOPARAM,                     "Norml:",                                      "Norml:",                                      0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  431 */  { fnLogicalNot,                NOPARAM,                     "NOT",                                         "NOT",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  432 */  { itemToBeCoded,               NOPARAM,                     "NPER",                                        "n" STD_SUB_P STD_SUB_E STD_SUB_R,             0,       0,       CAT_RVAR, SLS_UNCHANGED},
/*  433 */  { fnStatSum,                   0,                           "n" STD_SIGMA,                                 "n",                                           0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  434 */  { fnCvtLbfN,                   divide,                      "N" STD_RIGHT_ARROW "lbf",                     "N" STD_RIGHT_ARROW "lbf",                     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  435 */  { itemToBeCoded,               NOPARAM,                     "ODD?",                                        "ODD?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  436 */  { fnOff,                       NOPARAM,                     "OFF",                                         "OFF",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  437 */  { fnLogicalOr,                 NOPARAM,                     "OR",                                          "OR",                                          0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  438 */  { itemToBeCoded,               NOPARAM,                     "OrthoF",                                      "OrthoF",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  439 */  { itemToBeCoded,               NOPARAM,                     "ORTHOG",                                      "Orthog",                                      0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  440 */  { fnCvtOzKg,                   multiply,                    "oz" STD_RIGHT_ARROW "kg",                     "oz" STD_RIGHT_ARROW "kg",                     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  441 */  { fnConstant,                  37,                          "p" STD_SUB_0,                                 "p" STD_SUB_0,                                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*  442 */  { itemToBeCoded,               NOPARAM,                     "PAUSE",                                       "PAUSE",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  443 */  { fnCvtAtmPa,                  divide,                      "Pa" STD_RIGHT_ARROW "atm",                    "Pa" STD_RIGHT_ARROW "atm",                    0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  444 */  { fnCvtBarPa,                  divide,                      "Pa" STD_RIGHT_ARROW "bar",                    "Pa" STD_RIGHT_ARROW "bar",                    0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  445 */  { fnCvtInhgPa,                 divide,                      "Pa" STD_RIGHT_ARROW "iHg",                    "Pa" STD_RIGHT_ARROW,                          0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  446 */  { fnCvtPsiPa,                  divide,                      "Pa" STD_RIGHT_ARROW "psi",                    "Pa" STD_RIGHT_ARROW "psi",                    0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  447 */  { fnCvtTorrPa,                 divide,                      "Pa" STD_RIGHT_ARROW "tor",                    "Pa " STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  448 */  { itemToBeCoded,               NOPARAM,                     "PARTS",                                       "PARTS",                                       0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  449 */  { fnCvtPcM,                    multiply,                    "pc" STD_RIGHT_ARROW "m",                      "pc" STD_RIGHT_ARROW "m",                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  450 */  { fnPyx,                       NOPARAM,                     "PERM",                                        "P" STD_SUB_y STD_SUB_x,                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  451 */  { itemToBeCoded,               NOPARAM,                     "PER/a",                                       "per/a",                                       0,       0,       CAT_RVAR, SLS_UNCHANGED},
/*  452 */  { itemToBeCoded,               NOPARAM,                     "PGMINT",                                      "PGMINT",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  453 */  { itemToBeCoded,               NOPARAM,                     "PGMSLV",                                      "PGMSLV",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  454 */  { itemToBeCoded,               NOPARAM,                     "PIXEL",                                       "PIXEL",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  455 */  { itemToBeCoded,               NOPARAM,                     "PLOT",                                        "PLOT",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  456 */  { itemToBeCoded,               NOPARAM,                     "PMT",                                         "PMT",                                         0,       0,       CAT_RVAR, SLS_UNCHANGED},
/*  457 */  { itemToBeCoded,               NOPARAM,                     "P" STD_SUB_n,                                 "P" STD_SUB_n,                                 0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  458 */  { itemToBeCoded,               NOPARAM,                     "POINT",                                       "POINT",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  459 */  { itemToBeCoded,               NOPARAM,                     "Poiss" STD_SUB_p,                             "Poiss" STD_SUB_p,                             0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  460 */  { itemToBeCoded,               NOPARAM,                     "Poiss" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Poiss" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  461 */  { itemToBeCoded,               NOPARAM,                     "Poiss" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Poiss" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  462 */  { itemToBeCoded,               NOPARAM,                     "Poiss" STD_SUP_MINUS_1,                       "Poiss" STD_SUP_MINUS_1,                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  463 */  { itemToBeCoded,               NOPARAM,                     "Poiss:",                                      "Poiss:",                                      0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  464 */  { itemToBeCoded,               NOPARAM,                     "0464",                                        "0464",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  465 */  { itemToBeCoded,               NOPARAM,                     "PopLR",                                       "PopLR",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  466 */  { fnCurveFitting,              CF_POWER_FITTING,            "PowerF",                                      "PowerF",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  467 */  { fnCvtRatioDb,                10,                          "pr" STD_RIGHT_ARROW "dB",                     "power",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  468 */  { itemToBeCoded,               NOPARAM,                     "PRCL",                                        "PRCL",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  469 */  { fnIsPrime,                   NOPARAM,                     "PRIME?",                                      "PRIME?",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  470 */  { itemToBeCoded,               NOPARAM,                     "PROB",                                        "PROB",                                        0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  471 */  { itemToBeCoded,               NOPARAM,                     "0471",                                        "0471",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  472 */  { itemToBeCoded,               NOPARAM,                     "PROGS",                                       "PROGS",                                       0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  473 */  { fnCvtPsiPa,                  multiply,                    "psi" STD_RIGHT_ARROW "Pa",                    "psi" STD_RIGHT_ARROW "Pa",                    0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  474 */  { itemToBeCoded,               NOPARAM,                     "PSTO",                                        "PSTO",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  475 */  { fnCvtPointM,                 multiply,                    "pt." STD_RIGHT_ARROW "m",                     "point",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  476 */  { itemToBeCoded,               NOPARAM,                     "PUTK",                                        "PUTK",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  477 */  { itemToBeCoded,               NOPARAM,                     "PV",                                          "PV",                                          0,       0,       CAT_RVAR, SLS_UNCHANGED},
/*  478 */  { itemToBeCoded,               NOPARAM,                     "P.FN",                                        "P.FN",                                        0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  479 */  { itemToBeCoded,               NOPARAM,                     "P.FN2",                                       "P.FN2",                                       0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  480 */  { itemToBeCoded,               NOPARAM,                     "P:",                                          "P:",                                          0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  481 */  { itemToBeCoded,               NOPARAM,                     "QUIET",                                       "QUIET",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  482 */  { fnConstant,                  38,                          "R",                                           "R",                                           0,       0,       CAT_CNST, SLS_ENABLED  },
/*  483 */  { fnAngularMode,               AM_RADIAN,                   "RAD",                                         "RAD",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  484 */  { fnCvtToCurrentAngularMode,   AM_RADIAN,                   "RAD" STD_RIGHT_ARROW,                         "RAD" STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  485 */  { itemToBeCoded,               NOPARAM,                     "RAM",                                         "RAM",                                         0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  486 */  { fnRandom,                    NOPARAM,                     "RAN#",                                        "RAN#",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  487 */  { registerBrowser,             NOPARAM,                     "RBR",                                         "RBR",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  488 */  { fnRecall,                    TM_STORCL,                   "RCL",                                         "RCL",                                         0,      99,       CAT_FNCT, SLS_ENABLED  },
/*  489 */  { fnRecallConfig,              NOPARAM,                     "RCLCFG",                                      "Config",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  490 */  { fnRecallElement,             NOPARAM,                     "RCLEL",                                       "RCLEL",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  491 */  { fnRecallIJ,                  NOPARAM,                     "RCLIJ",                                       "RCLIJ",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  492 */  { fnRecallStack,               TM_REGISTER,                 "RCLS",                                        "RCLS",                                        0,      99,       CAT_FNCT, SLS_ENABLED  },
/*  493 */  { fnRecallAdd,                 NOPARAM,                     "RCL+",                                        "RCL+",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  494 */  { fnRecallSub,                 NOPARAM,                     "RCL-",                                        "RCL-",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  495 */  { fnRecallMult,                NOPARAM,                     "RCL" STD_CROSS,                               "RCL" STD_CROSS,                               0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  496 */  { fnRecallDiv,                 NOPARAM,                     "RCL/",                                        "RCL/",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  497 */  { fnRecallMax,                 NOPARAM,                     "RCL" STD_UP_ARROW,                            "Max",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  498 */  { fnRecallMin,                 NOPARAM,                     "RCL" STD_DOWN_ARROW,                          "Min",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  499 */  { itemToBeCoded,               NOPARAM,                     "RDP",                                         "RDP",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  500 */  { itemToBeCoded,               NOPARAM,                     "0500",                                        "0500",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  501 */  { itemToBeCoded,               NOPARAM,                     "0501",                                        "0501",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  502 */  { fnConstant,                  39,                          "r" STD_SUB_e,                                 "r" STD_SUB_e,                                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*  503 */  { fnRealPart,                  NOPARAM,                     "Re",                                          "Re",                                          0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  504 */  { itemToBeCoded,               NOPARAM,                     "0504",                                        "0504",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  505 */  { itemToBeCoded,               NOPARAM,                     "REALS",                                       "REALS",                                       0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  506 */  { itemToBeCoded,               NOPARAM,                     "REAL?",                                       "REAL?",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  507 */  { itemToBeCoded,               NOPARAM,                     "0507",                                        "0507",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  508 */  { itemToBeCoded,               NOPARAM,                     "REGS",                                        "REGS",                                        0,       0,       CAT_RVAR, SLS_UNCHANGED},
/*  509 */  { itemToBeCoded,               NOPARAM,                     "RECV",                                        "RECV",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  510 */  { fnReset,                     NOT_CONFIRMED,               "RESET",                                       "RESET",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  511 */  { fnReToCx,                    NOPARAM,                     "RE" STD_RIGHT_ARROW "CX",                     "RE" STD_RIGHT_ARROW "CX",                     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  512 */  { fnSwapRealImaginary,         NOPARAM,                     "Re" STD_LEFT_RIGHT_ARROWS "Im",               "Re" STD_LEFT_RIGHT_ARROWS "Im",               0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  513 */  { fnRj,                        NOPARAM,                     "RJ",                                          "RJ",                                          0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  514 */  { fnConstant,                  40,                          "R" STD_SUB_K,                                 "R" STD_SUB_K,                                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*  515 */  { fnRl,                        TM_VALUE,                    "RL",                                          "RL",                                          0,      63,       CAT_FNCT, SLS_ENABLED  },
/*  516 */  { fnRlc,                       TM_VALUE,                    "RLC",                                         "RLC",                                         0,      63,       CAT_FNCT, SLS_ENABLED  },
/*  517 */  { fnConstant,                  41,                          "R" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "R" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   0,       0,       CAT_CNST, SLS_ENABLED  },
/*  518 */  { itemToBeCoded,               NOPARAM,                     "RM",                                          "RM",                                          0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  519 */  { fnGetRoundingMode,           NOPARAM,                     "RM?",                                         "RM?",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  520 */  { fnRmd,                       NOPARAM,                     "RMD",                                         "RMD",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  521 */  { itemToBeCoded,               NOPARAM,                     "RNORM",                                       "RNORM",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  522 */  { fnRound,                     NOPARAM,                     "ROUND",                                       "ROUND",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  523 */  { fnRoundi,                    NOPARAM,                     "ROUNDI",                                      "ROUNDI",                                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  524 */  { fnRr,                        TM_VALUE,                    "RR",                                          "RR",                                          0,      63,       CAT_FNCT, SLS_ENABLED  },
/*  525 */  { fnRrc,                       TM_VALUE,                    "RRC",                                         "RRC",                                         0,      63,       CAT_FNCT, SLS_ENABLED  },
/*  526 */  { itemToBeCoded,               NOPARAM,                     "RSD",                                         "RSD",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  527 */  { itemToBeCoded,               NOPARAM,                     "RSUM",                                        "RSUM",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  528 */  { itemToBeCoded,               NOPARAM,                     "RTN",                                         "RTN",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  529 */  { itemToBeCoded,               NOPARAM,                     "RTN+1",                                       "RTN+1",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  530 */  { itemToBeCoded,               NOPARAM,                     "R-CLR",                                       "R-CLR",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  531 */  { itemToBeCoded,               NOPARAM,                     "R-COPY",                                      "R-COPY",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  532 */  { itemToBeCoded,               NOPARAM,                     "R-SORT",                                      "R-SORT",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  533 */  { itemToBeCoded,               NOPARAM,                     "R-SWAP",                                      "R-SWAP",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  534 */  { fnCvtRadToDeg,               NOPARAM,                     "R" STD_RIGHT_ARROW "D",                       "R" STD_RIGHT_ARROW "D",                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  535 */  { fnRollUp,                    NOPARAM,                     "R" STD_UP_ARROW,                              "R" STD_UP_ARROW,                              0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  536 */  { fnRollDown,                  NOPARAM,                     "R" STD_DOWN_ARROW,                            "R" STD_DOWN_ARROW,                            0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  537 */  { fnConstant,                  42,                          "R" STD_SUB_INFINITY,                          "R" STD_SUB_INFINITY,                          0,       0,       CAT_CNST, SLS_ENABLED  },
/*  538 */  { fnConstant,                  43,                          "R" STD_SUB_SUN,                               "R" STD_SUB_SUN,                               0,       0,       CAT_CNST, SLS_ENABLED  },
/*  539 */  { fnConstant,                  44,                          "R" STD_SUB_EARTH,                             "R" STD_SUB_EARTH,                             0,       0,       CAT_CNST, SLS_ENABLED  },
/*  540 */  { fnSampleStdDev,              NOPARAM,                     "s",                                           "s",                                           0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  541 */  { fnConstant,                  45,                          "Sa",                                          "Sa",                                          0,       0,       CAT_CNST, SLS_ENABLED  },
/*  542 */  { itemToBeCoded,               NOPARAM,                     "SAVE",                                        "SAVE",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  543 */  { fnSb,                        TM_VALUE,                    "SB",                                          "SB",                                          1,      64,       CAT_FNCT, SLS_ENABLED  },
/*  544 */  { fnConstant,                  46,                          "Sb",                                          "Sb",                                          0,       0,       CAT_CNST, SLS_ENABLED  },
/*  545 */  { fnDisplayFormatSci,          TM_VALUE,                    "SCI",                                         "SCI",                                         0,      15,       CAT_FNCT, SLS_UNCHANGED},
/*  546 */  { fnCvtShortcwtKg,             multiply,                    "scw" STD_RIGHT_ARROW "kg",                    "short",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  547 */  { itemToBeCoded,               NOPARAM,                     "0547",                                        "0457",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  548 */  { fnGetSignificantDigits,      NOPARAM,                     "SDIGS?",                                      "SDIGS?",                                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  549 */  { itemToBeCoded,               NOPARAM,                     "SDL",                                         "SDL",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  550 */  { itemToBeCoded,               NOPARAM,                     "SDR",                                         "SDR",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  551 */  { fnConstant,                  47,                          "Se" STD_SUP_2,                                "Se" STD_SUP_2,                                0,       0,       CAT_CNST, SLS_ENABLED  },
/*  552 */  { fnSeed,                      NOPARAM,                     "SEED",                                        "SEED",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  553 */  { itemToBeCoded,               NOPARAM,                     "SEND",                                        "SEND",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  554 */  { fnConfigChina,               NOPARAM,                     "SETCHN",                                      "CHINA",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  555 */  { itemToBeCoded,               NOPARAM,                     "SETDAT",                                      "SETDAT",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  556 */  { fnConfigEurope,              NOPARAM,                     "SETEUR",                                      "EUROPE",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  557 */  { fnConfigIndia,               NOPARAM,                     "SETIND",                                      "INDIA",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  558 */  { fnConfigJapan,               NOPARAM,                     "SETJPN",                                      "JAPAN",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  559 */  { itemToBeCoded,               NOPARAM,                     "SETSIG",                                      "SETSIG",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  560 */  { itemToBeCoded,               NOPARAM,                     "SETTIM",                                      "SETTIM",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  561 */  { fnConfigUk,                  NOPARAM,                     "SETUK",                                       "UK",                                          0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  562 */  { fnConfigUsa,                 NOPARAM,                     "SETUSA",                                      "USA",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  563 */  { fnConstant,                  48,                          "Se'" STD_SUP_2,                               "Se'" STD_SUP_2,                               0,       0,       CAT_CNST, SLS_ENABLED  },
/*  564 */  { fnSetFlag,                   TM_FLAGW,                    "SF",                                          "SF",                                          0,      99,       CAT_FNCT, SLS_UNCHANGED},
/*  565 */  { fnConstant,                  49,                          "Sf" STD_SUP_MINUS_1,                          "Sf" STD_SUP_MINUS_1,                          0,       0,       CAT_CNST, SLS_ENABLED  },
/*  566 */  { fnSign,                      NOPARAM,                     "SIGN",                                        "sign",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  567 */  { fnIntegerMode,               SIM_SIGNMT,                  "SIGNMT",                                      "SIGNMT",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  568 */  { itemToBeCoded,               NOPARAM,                     "SIM_EQ",                                      "SIM EQ",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  569 */  { fnSin,                       NOPARAM,                     "sin",                                         "sin",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  570 */  { fnSinc,                      NOPARAM,                     "sinc",                                        "sinc",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  571 */  { fnSinh,                      NOPARAM,                     "sinh",                                        "sinh",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  572 */  { itemToBeCoded,               NOPARAM,                     "SKIP",                                        "SKIP",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  573 */  { fnSl,                        TM_VALUE,                    "SL",                                          "SL",                                          0,      63,       CAT_FNCT, SLS_ENABLED  },
/*  574 */  { itemToBeCoded,               NOPARAM,                     "SLOW",                                        "SLOW",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  575 */  { fnSlvq,                      NOPARAM,                     "SLVQ",                                        "SLVQ",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  576 */  { fnStandardError,             NOPARAM,                     "s" STD_SUB_m,                                 "s" STD_SUB_m,                                 0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  577 */  { fnGetIntegerSignMode,        NOPARAM,                     "SMODE?",                                      "SMODE?",                                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  578 */  { fnWeightedStandardError,     NOPARAM,                     "s" STD_SUB_m STD_SUB_w,                       "s" STD_SUB_m STD_SUB_w,                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  579 */  { itemToBeCoded,               NOPARAM,                     "SOLVE",                                       "SOLVE",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  580 */  { itemToBeCoded,               NOPARAM,                     "Solver",                                      "Solver",                                      0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  581 */  { itemToBeCoded,               NOPARAM,                     "SPEC?",                                       "SPEC?",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  582 */  { fnSr,                        TM_VALUE,                    "SR",                                          "SR",                                          0,      63,       CAT_FNCT, SLS_ENABLED  },
/*  583 */  { itemToBeCoded,               NOPARAM,                     "0583",                                        "0583",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  584 */  { itemToBeCoded,               NOPARAM,                     "0584",                                        "0584",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  585 */  { fnGetStackSize,              NOPARAM,                     "SSIZE?",                                      "SSIZE?",                                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  586 */  { itemToBeCoded,               NOPARAM,                     "STAT",                                        "STAT",                                        0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  587 */  { flagBrowser,                 NOPARAM,                     "STATUS",                                      "STATUS",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  588 */  { itemToBeCoded,               NOPARAM,                     "STK",                                         "STK",                                         0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  589 */  { fnStore,                     TM_STORCL,                   "STO",                                         "STO",                                         0,      99,       CAT_FNCT, SLS_UNCHANGED},
/*  590 */  { fnStoreConfig,               NOPARAM,                     "STOCFG",                                      "Config",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  591 */  { fnStoreElement,              NOPARAM,                     "STOEL",                                       "STOEL",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  592 */  { fnStoreIJ,                   NOPARAM,                     "STOIJ",                                       "STOIJ",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  593 */  { itemToBeCoded,               NOPARAM,                     "STOP",                                        "R/S",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  594 */  { fnStoreStack,                TM_REGISTER,                 "STOS",                                        "STOS",                                        0,      99,       CAT_FNCT, SLS_ENABLED  },
/*  595 */  { fnStoreAdd,                  NOPARAM,                     "STO+",                                        "STO+",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  596 */  { fnStoreSub,                  NOPARAM,                     "STO-",                                        "STO-",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  597 */  { fnStoreMult,                 NOPARAM,                     "STO" STD_CROSS,                               "STO" STD_CROSS,                               0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  598 */  { fnStoreDiv,                  NOPARAM,                     "STO/",                                        "STO/",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  599 */  { fnStoreMax,                  NOPARAM,                     "STO" STD_UP_ARROW,                            "Max",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  600 */  { fnStoreMin,                  NOPARAM,                     "STO" STD_DOWN_ARROW,                          "Min",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  601 */  { fnCvtStoneKg,                multiply,                    "sto" STD_RIGHT_ARROW "kg",                    "stone",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  602 */  { itemToBeCoded,               NOPARAM,                     "STRI?",                                       "STRI?",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  603 */  { itemToBeCoded,               NOPARAM,                     "STRING",                                      "STRING",                                      0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  604 */  { addItemToBuffer,             REGISTER_A,                  "ST.A",                                        "ST.A",                                        0,       0,       CAT_REGS, SLS_UNCHANGED},
/*  605 */  { addItemToBuffer,             REGISTER_B,                  "ST.B",                                        "ST.B",                                        0,       0,       CAT_REGS, SLS_UNCHANGED},
/*  606 */  { addItemToBuffer,             REGISTER_C,                  "ST.C",                                        "ST.C",                                        0,       0,       CAT_REGS, SLS_UNCHANGED},
/*  607 */  { addItemToBuffer,             REGISTER_D,                  "ST.D",                                        "ST.D",                                        0,       0,       CAT_REGS, SLS_UNCHANGED},
/*  608 */  { addItemToBuffer,             REGISTER_T,                  "ST.T",                                        "ST.T",                                        0,       0,       CAT_REGS, SLS_UNCHANGED},
/*  609 */  { addItemToBuffer,             REGISTER_X,                  "ST.X",                                        "ST.X",                                        0,       0,       CAT_REGS, SLS_UNCHANGED},
/*  610 */  { addItemToBuffer,             REGISTER_Y,                  "ST.Y",                                        "ST.Y",                                        0,       0,       CAT_REGS, SLS_UNCHANGED},
/*  611 */  { addItemToBuffer,             REGISTER_Z,                  "ST.Z",                                        "ST.Z",                                        0,       0,       CAT_REGS, SLS_UNCHANGED},
/*  612 */  { fnSumXY,                     NOPARAM,                     "SUM",                                         "SUM",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  613 */  { fnWeightedSampleStdDev,      NOPARAM,                     "s" STD_SUB_w,                                 "s" STD_SUB_w,                                 0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  614 */  { itemToBeCoded,               NOPARAM,                     "s" STD_SUB_x STD_SUB_y,                       "s" STD_SUB_x STD_SUB_y,                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  615 */  { fnCvtShorttonKg,             multiply,                    "s.t" STD_RIGHT_ARROW "kg",                    "short",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  616 */  { fnCvtYearS,                  divide,                      "s" STD_RIGHT_ARROW "year",                    "s" STD_RIGHT_ARROW "year",                    0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  617 */  { fnConstant,                  50,                          "T" STD_SUB_0,                                 "T" STD_SUB_0,                                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*  618 */  { fnTan,                       NOPARAM,                     "tan",                                         "tan",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  619 */  { fnTanh,                      NOPARAM,                     "tanh",                                        "tanh",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  620 */  { itemToBeCoded,               NOPARAM,                     "TDISP",                                       "TDISP",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  621 */  { itemToBeCoded,               NOPARAM,                     "TEST",                                        "TEST",                                        0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  622 */  { fnTicks,                     NOPARAM,                     "TICKS",                                       "TICKS",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  623 */  { itemToBeCoded,               NOPARAM,                     "TIME",                                        "TIME",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  624 */  { itemToBeCoded,               NOPARAM,                     "TIMER",                                       "TIMER",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  625 */  { itemToBeCoded,               NOPARAM,                     "TIMES",                                       "TIMES",                                       0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  626 */  { itemToBeCoded,               NOPARAM,                     "T" STD_SUB_n,                                 "T" STD_SUB_n,                                 0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  627 */  { itemToBeCoded,               NOPARAM,                     "TONE",                                        "TONE",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  628 */  { fnCvtTonKg,                  multiply,                    "ton" STD_RIGHT_ARROW "kg",                    "ton" STD_RIGHT_ARROW "kg",                    0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  629 */  { itemToBeCoded,               NOPARAM,                     "TOP?",                                        "TOP?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  630 */  { fnCvtTorrPa,                 multiply,                    "tor" STD_RIGHT_ARROW "Pa",                    "torr",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  631 */  { fnConstant,                  51,                          "T" STD_SUB_p,                                 "T" STD_SUB_P,                                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*  632 */  { fnConstant,                  52,                          "t" STD_SUB_P STD_SUB_L,                       "t" STD_SUB_P STD_SUB_L,                       0,       0,       CAT_CNST, SLS_ENABLED  },
/*  633 */  { itemToBeCoded,               NOPARAM,                     "t" STD_SUB_p "(x)",                           "t" STD_SUB_p "(x)",                           0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  634 */  { itemToBeCoded,               NOPARAM,                     "t" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", "t" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", 0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  635 */  { itemToBeCoded,               NOPARAM,                     "t" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", "t" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", 0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  636 */  { itemToBeCoded,               NOPARAM,                     "t" STD_SUP_MINUS_1 "(p)",                     "t" STD_SUP_MINUS_1 "(p)",                     0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  637 */  { itemToBeCoded,               NOPARAM,                     "TRI",                                         "TRI",                                         0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  638 */  { fnCvtTrozKg,                 multiply,                    "trz" STD_RIGHT_ARROW "kg",                    "tr.oz",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  639 */  { itemToBeCoded,               NOPARAM,                     "TVM",                                         "TVM",                                         0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  640 */  { itemToBeCoded,               NOPARAM,                     "t:",                                          "t:",                                          0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  641 */  { fnSwapT,                     TM_REGISTER,                 "t" STD_LEFT_RIGHT_ARROWS,                     "t" STD_LEFT_RIGHT_ARROWS,                     0,      99,       CAT_FNCT, SLS_ENABLED  },
/*  642 */  { fnUlp,                       NOPARAM,                     "ULP?",                                        "ULP?",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  643 */  { itemToBeCoded,               NOPARAM,                     "U" STD_SUB_n,                                 "U" STD_SUB_n,                                 0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  644 */  { fnUnitVector,                NOPARAM,                     "UNITV",                                       "UNITV",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  645 */  { fnIntegerMode,               SIM_UNSIGN,                  "UNSIGN",                                      "UNSIGN",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  646 */  { itemToBeCoded,               NOPARAM,                     "U" STD_RIGHT_ARROW,                           "U" STD_RIGHT_ARROW,                           0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  647 */  { itemToBeCoded,               NOPARAM,                     "VARMNU",                                      "VARMNU",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  648 */  { itemToBeCoded,               NOPARAM,                     "VARS",                                        "VARS",                                        0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  649 */  { fnVersion,                   NOPARAM,                     "VERS?",                                       "VERS?",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  650 */  { itemToBeCoded,               NOPARAM,                     "VIEW",                                        "VIEW",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  651 */  { fnConstant,                  53,                          "V" STD_SUB_m,                                 "V" STD_SUB_m,                                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*  652 */  { itemToBeCoded,               NOPARAM,                     "V:",                                          "V:",                                          0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  653 */  { itemToBeCoded,               NOPARAM,                     "WDAY",                                        "WDAY",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  654 */  { itemToBeCoded,               NOPARAM,                     "Weibl" STD_SUB_p,                             "Weibl" STD_SUB_p,                             0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  655 */  { itemToBeCoded,               NOPARAM,                     "Weibl" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Weibl" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  656 */  { itemToBeCoded,               NOPARAM,                     "Weibl" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Weibl" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  657 */  { itemToBeCoded,               NOPARAM,                     "Weibl" STD_SUP_MINUS_1,                       "Weibl" STD_SUP_MINUS_1,                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  658 */  { itemToBeCoded,               NOPARAM,                     "Weibl:",                                      "Weibl:",                                      0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  659 */  { fnWho,                       NOPARAM,                     "WHO?",                                        "WHO?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  660 */  { fnCvtWhJ,                    multiply,                    "Wh" STD_RIGHT_ARROW "J",                      "Wh" STD_RIGHT_ARROW "J",                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  661 */  { itemToBeCoded,               NOPARAM,                     "W" STD_SUB_m,                                 "W" STD_SUB_m,                                 0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  662 */  { itemToBeCoded,               NOPARAM,                     "W" STD_SUB_p,                                 "W" STD_SUB_p,                                 0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  663 */  { itemToBeCoded,               NOPARAM,                     "W" STD_SUP_MINUS_1,                           "W" STD_SUP_MINUS_1,                           0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  664 */  { fnSetWordSize,               TM_VALUE,                    "WSIZE",                                       "WSIZE",                                       0,      64,       CAT_FNCT, SLS_UNCHANGED},
/*  665 */  { fnGetWordSize,               NOPARAM,                     "WSIZE?",                                      "WSIZE?",                                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  666 */  { fnCvtHpeW,                   divide,                      "W" STD_RIGHT_ARROW "hp" STD_SUB_E,            "W" STD_RIGHT_ARROW "hp" STD_SUB_E,            0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  667 */  { fnCvtHpmW,                   divide,                      "W" STD_RIGHT_ARROW "hp" STD_SUB_M,            "W" STD_RIGHT_ARROW "hp" STD_SUB_M,            0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  668 */  { fnCvtHpukW,                  divide,                      "W" STD_RIGHT_ARROW "hp" STD_UK,               "W" STD_RIGHT_ARROW "hp" STD_UK,               0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  669 */  { fnSquare,                    NOPARAM,                     "x" STD_SUP_2,                                 "x" STD_SUP_2,                                 0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  670 */  { fnCube,                      NOPARAM,                     "x" STD_SUP_3,                                 "x" STD_SUP_3,                                 0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  671 */  { itemToBeCoded,               NOPARAM,                     "XEQ",                                         "XEQ",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  672 */  { fnLogicalXnor,               NOPARAM,                     "XNOR",                                        "XNOR",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  673 */  { fnLogicalXor,                NOPARAM,                     "XOR",                                         "XOR",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  674 */  { fnMeanXY,                    NOPARAM,                     STD_x_BAR,                                     STD_x_BAR,                                     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  675 */  { fnGeometricMeanXY,           NOPARAM,                     STD_x_BAR STD_SUB_G,                           STD_x_BAR STD_SUB_G,                           0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  676 */  { fnWeightedMeanX,             NOPARAM,                     STD_x_BAR STD_SUB_w,                           STD_x_BAR STD_SUB_w,                           0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  677 */  { itemToBeCoded,               NOPARAM,                     STD_x_CIRC,                                    STD_x_CIRC,                                    0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  678 */  { itemToBeCoded,               NOPARAM,                     "X.FN",                                        "X.FN",                                        0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  679 */  { fnFactorial,                 NOPARAM,                     "x!",                                          "x!",                                          0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  680 */  { itemToBeCoded,               NOPARAM,                     "x:",                                          "x:",                                          0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  681 */  { itemToBeCoded,               NOPARAM,                     "x" STD_RIGHT_ARROW "DATE",                    "x" STD_RIGHT_ARROW "DATE",                    0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  682 */  { fnXToAlpha,                  NOPARAM,                     "x" STD_RIGHT_ARROW STD_alpha,                 "x" STD_RIGHT_ARROW STD_alpha,                 0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  683 */  { fnSwapX,                     TM_REGISTER,                 "x" STD_LEFT_RIGHT_ARROWS,                     "x" STD_LEFT_RIGHT_ARROWS,                     0,      99,       CAT_FNCT, SLS_ENABLED  },
/*  684 */  { fnSwapXY,                    NOPARAM,                     "x" STD_LEFT_RIGHT_ARROWS "y",                 "x" STD_LEFT_RIGHT_ARROWS "y",                 0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  685 */  { itemToBeCoded,               NOPARAM,                     "x= ?",                                        "x= ?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  686 */  { itemToBeCoded,               NOPARAM,                     "x" STD_NOT_EQUAL " ?",                        "x" STD_NOT_EQUAL " ?",                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  687 */  { itemToBeCoded,               NOPARAM,                     "x=+0?",                                       "x=+0?",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  688 */  { itemToBeCoded,               NOPARAM,                     "x=-0?",                                       "x=-0?",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  689 */  { itemToBeCoded,               NOPARAM,                     "x" STD_ALMOST_EQUAL " ?",                     "x" STD_ALMOST_EQUAL " ?",                     0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  690 */  { itemToBeCoded,               NOPARAM,                     "x< ?",                                        "x< ?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  690 *///{ fnXLessThan,                 NOPARAM,                     "x< ?",                                        "x< ?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  691 */  { itemToBeCoded,               NOPARAM,                     "x" STD_LESS_EQUAL " ?",                       "x" STD_LESS_EQUAL " ?",                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  692 */  { itemToBeCoded,               NOPARAM,                     "x" STD_GREATER_EQUAL " ?",                    "x" STD_GREATER_EQUAL " ?",                    0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  693 */  { itemToBeCoded,               NOPARAM,                     "x> ?",                                        "x> ?",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  694 */  { fnXthRoot,                   NOPARAM,                     STD_xTH_ROOT STD_y_UNDER_ROOT,                 STD_xTH_ROOT STD_y_UNDER_ROOT,                 0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  695 */  { fnCvtYardM,                  multiply,                    "yd." STD_RIGHT_ARROW "m",                     "yd." STD_RIGHT_ARROW "m",                     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  696 */  { itemToBeCoded,               NOPARAM,                     "YEAR",                                        "YEAR",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  697 */  { fnCvtYearS,                  multiply,                    "year" STD_RIGHT_ARROW "s",                    "year" STD_RIGHT_ARROW "s",                    0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  698 */  { fnPower,                     NOPARAM,                     "y" STD_SUP_x,                                 "y" STD_SUP_x,                                 0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  699 */  { itemToBeCoded,               NOPARAM,                     STD_y_CIRC,                                    STD_y_CIRC,                                    0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  700 */  { fnSetDateFormat,             ITM_YMD,                     "Y.MD",                                        "Y.MD",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  701 */  { fnSwapY,                     TM_REGISTER,                 "y" STD_LEFT_RIGHT_ARROWS,                     "y" STD_LEFT_RIGHT_ARROWS,                     0,      99,       CAT_FNCT, SLS_ENABLED  },
/*  702 */  { fnConstant,                  54,                          "Z" STD_SUB_0,                                 "Z" STD_SUB_0,                                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*  703 */  { fnSwapZ,                     TM_REGISTER,                 "z" STD_LEFT_RIGHT_ARROWS,                     "z" STD_LEFT_RIGHT_ARROWS,                     0,      99,       CAT_FNCT, SLS_ENABLED  },
/*  704 */  { fnConstant,                  55,                          STD_alpha,                                     STD_alpha,                                     0,       0,       CAT_CNST, SLS_ENABLED  },
/*  705 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "INTL",                              STD_alpha "INTL",                              0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  706 */  { fnAlphaLeng,                 TM_REGISTER,                 STD_alpha "LENG?",                             STD_alpha "LENG?",                             0,      99,       CAT_FNCT, SLS_ENABLED  },
/*  707 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "MATH",                              STD_alpha "MATH",                              0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  708 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "OFF",                               STD_alpha "OFF",                               0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  709 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "ON",                                STD_alpha "ON",                                0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  710 */  { fnAlphaPos,                  TM_REGISTER,                 STD_alpha "POS?",                              STD_alpha "POS?",                              0,      99,       CAT_FNCT, SLS_ENABLED  },
/*  711 */  { fnAlphaRL,                   TM_REGISTER,                 STD_alpha "RL",                                STD_alpha "RL",                                0,      99,       CAT_FNCT, SLS_ENABLED  },
/*  712 */  { fnAlphaRR,                   TM_REGISTER,                 STD_alpha "RR",                                STD_alpha "RR",                                0,      99,       CAT_FNCT, SLS_ENABLED  },
/*  713 */  { fnAlphaSL,                   TM_REGISTER,                 STD_alpha "SL",                                STD_alpha "SL",                                0,      99,       CAT_FNCT, SLS_ENABLED  },
/*  714 */  { itemToBeCoded,               NOPARAM,                     STD_alpha ".FN",                               STD_alpha ".FN",                               0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  715 */  { itemToBeCoded,               NOPARAM,                     STD_ALPHA STD_ELLIPSIS STD_OMEGA,              STD_ALPHA STD_ELLIPSIS STD_OMEGA,              0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  716 */  { itemToBeCoded,               NOPARAM,                     STD_alpha STD_BULLET,                          STD_alpha STD_BULLET,                          0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  717 */  { fnAlphaToX,                  TM_REGISTER,                 STD_alpha STD_RIGHT_ARROW "x",                 STD_alpha STD_RIGHT_ARROW "x",                 0,      99,       CAT_FNCT, SLS_ENABLED  },
/*  718 */  { itemToBeCoded,               NOPARAM,                     STD_beta "(x,y)",                              STD_beta "(x,y)",                              0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  719 */  { fnConstant,                  56,                          STD_gamma,                                     STD_gamma,                                     0,       0,       CAT_CNST, SLS_ENABLED  },
/*  720 */  { fnConstant,                  57,                          STD_gamma STD_SUB_E STD_SUB_M,                 STD_gamma STD_SUB_E STD_SUB_M,                 0,       0,       CAT_CNST, SLS_ENABLED  },
/*  721 */  { fnConstant,                  58,                          STD_gamma STD_SUB_p,                           STD_gamma STD_SUB_p,                           0,       0,       CAT_CNST, SLS_ENABLED  },
/*  722 */  { itemToBeCoded,               NOPARAM,                     STD_gamma STD_SUB_x STD_SUB_y,                 STD_gamma STD_SUB_x STD_SUB_y,                 0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  723 */  { itemToBeCoded,               NOPARAM,                     STD_GAMMA STD_SUB_x STD_SUB_y,                 STD_GAMMA STD_SUB_x STD_SUB_y,                 0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  724 */  { fnGamma,                     NOPARAM,                     STD_GAMMA "(x)",                               STD_GAMMA "(x)",                               0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  725 */  { itemToBeCoded,               NOPARAM,                     STD_delta "x",                                 STD_delta "x",                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  726 */  { fnDeltaPercent,              NOPARAM,                     STD_DELTA "%",                                 STD_DELTA "%",                                 0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  727 */  { fnGeometricSampleStdDev,     NOPARAM,                     STD_epsilon,                                   STD_epsilon,                                   0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  728 */  { fnConstant,                  60,                          STD_epsilon STD_SUB_0,                         STD_epsilon STD_SUB_0,                         0,       0,       CAT_CNST, SLS_ENABLED  },
/*  729 */  { fnGeometricStandardError,    NOPARAM,                     STD_epsilon STD_SUB_m,                         STD_epsilon STD_SUB_m,                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  730 */  { fnGeometricPopulationStdDev, NOPARAM,                     STD_epsilon STD_SUB_p,                         STD_epsilon STD_SUB_p,                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  731 */  { itemToBeCoded,               NOPARAM,                     STD_zeta "(x)",                                STD_zeta "(x)",                                0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  732 */  { fnConstant,                  61,                          STD_lambda STD_SUB_C,                          STD_lambda STD_SUB_C,                          0,       0,       CAT_CNST, SLS_ENABLED  },
/*  733 */  { fnConstant,                  62,                          STD_lambda STD_SUB_C STD_SUB_n,                STD_lambda STD_SUB_C STD_SUB_n,                0,       0,       CAT_CNST, SLS_ENABLED  },
/*  734 */  { fnConstant,                  63,                          STD_lambda STD_SUB_C STD_SUB_p,                STD_lambda STD_SUB_C STD_SUB_p,                0,       0,       CAT_CNST, SLS_ENABLED  },
/*  735 */  { fnConstant,                  64,                          STD_mu STD_SUB_0,                              STD_mu STD_SUB_0,                              0,       0,       CAT_CNST, SLS_ENABLED  },
/*  736 */  { fnConstant,                  65,                          STD_mu STD_SUB_B,                              STD_mu STD_SUB_B,                              0,       0,       CAT_CNST, SLS_ENABLED  },
/*  737 */  { fnConstant,                  66,                          STD_mu STD_SUB_e,                              STD_mu STD_SUB_e,                              0,       0,       CAT_CNST, SLS_ENABLED  },
/*  738 */  { fnConstant,                  67,                          STD_mu STD_SUB_e "/" STD_mu STD_SUB_B,         STD_mu STD_SUB_e "/" STD_mu STD_SUB_B,         0,       0,       CAT_CNST, SLS_ENABLED  },
/*  739 */  { fnConstant,                  68,                          STD_mu STD_SUB_n,                              STD_mu STD_SUB_n,                              0,       0,       CAT_CNST, SLS_ENABLED  },
/*  740 */  { fnConstant,                  69,                          STD_mu STD_SUB_p,                              STD_mu STD_SUB_p,                              0,       0,       CAT_CNST, SLS_ENABLED  },
/*  741 */  { fnConstant,                  70,                          STD_mu STD_SUB_u,                              STD_mu STD_SUB_u,                              0,       0,       CAT_CNST, SLS_ENABLED  },
/*  742 */  { fnConstant,                  71,                          STD_mu STD_SUB_mu,                             STD_mu STD_SUB_mu,                             0,       0,       CAT_CNST, SLS_ENABLED  },
/*  743 */  { itemToBeCoded,               NOPARAM,                     STD_PI STD_SUB_n,                              STD_PI STD_SUB_n,                              0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  744 */  { fnPi,                        NOPARAM,                     STD_pi,                                        STD_pi,                                        0,       0,       CAT_NONE, SLS_ENABLED  },
/*  745 */  { itemToBeCoded,               NOPARAM,                     STD_SIGMA STD_SUB_n,                           STD_SIGMA STD_SUB_n,                           0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  746 */  { fnPopulationStdDev,          NOPARAM,                     STD_sigma,                                     STD_sigma,                                     0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  747 */  { fnConstant,                  72,                          STD_sigma STD_SUB_B,                           STD_sigma STD_SUB_B,                           0,       0,       CAT_CNST, SLS_ENABLED  },
/*  748 */  { fnStatSum,                   9,                           STD_SIGMA "ln" STD_SUP_2 "x",                  STD_SIGMA "ln" STD_SUP_2 "x",                  0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  749 */  { fnStatSum,                   12,                          STD_SIGMA "ln" STD_SUP_2 "y",                  STD_SIGMA "ln" STD_SUP_2 "y",                  0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  750 */  { fnStatSum,                   8,                           STD_SIGMA "lnx",                               STD_SIGMA "lnx",                               0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  751 */  { fnStatSum,                   7,                           STD_SIGMA "lnxy",                              STD_SIGMA "lnxy",                              0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  752 */  { fnStatSum,                   11,                          STD_SIGMA "lny",                               STD_SIGMA "lny",                               0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  753 */  { fnWeightedPopulationStdDev,  NOPARAM,                     STD_sigma STD_SUB_w,                           STD_sigma STD_SUB_w,                           0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  754 */  { fnStatSum,                   1,                           STD_SIGMA "x",                                 STD_SIGMA "x",                                 0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  755 */  { fnStatSum,                   3,                           STD_SIGMA "x" STD_SUP_2,                       STD_SIGMA "x" STD_SUP_2,                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  756 */  { fnStatSum,                   4,                           STD_SIGMA "x" STD_SUP_2 "y",                   STD_SIGMA "x" STD_SUP_2 "y",                   0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  757 */  { fnStatSum,                   13,                          STD_SIGMA "xlny",                              STD_SIGMA "xlny",                              0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  758 */  { fnStatSum,                   6,                           STD_SIGMA "xy",                                STD_SIGMA "xy",                                0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  759 */  { fnStatSum,                   2,                           STD_SIGMA "y",                                 STD_SIGMA "y",                                 0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  760 */  { fnStatSum,                   5,                           STD_SIGMA "y" STD_SUP_2,                       STD_SIGMA "y" STD_SUP_2,                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  761 */  { fnStatSum,                   10,                          STD_SIGMA "ylnx",                              STD_SIGMA "ylnx",                              0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  762 */  { fnSigma,                     1,                           STD_SIGMA "+",                                 STD_SIGMA "+",                                 0,       0,       CAT_FNCT, SLS_DISABLED },
/*  763 */  { fnSigma,                     2,                           STD_SIGMA "-",                                 STD_SIGMA "-",                                 0,       0,       CAT_FNCT, SLS_DISABLED },
/*  764 */  { fnConstant,                  73,                          STD_PHI,                                       STD_PHI,                                       0,       0,       CAT_CNST, SLS_ENABLED  },
/*  765 */  { fnConstant,                  74,                          STD_PHI STD_SUB_0,                             STD_PHI STD_SUB_0,                             0,       0,       CAT_CNST, SLS_ENABLED  },
/*  766 */  { fnRandomI,                   NOPARAM,                     "RANI#",                                       "RANI#",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  767 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "x",                               STD_PRINTER "x",                               0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  768 */  { itemToBeCoded,               NOPARAM,                     "SYS.FL",                                      "SYS.FL",                                      0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  769 */  { fnRange,                     NOPARAM,                     "RANGE",                                       "RANGE",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  770 */  { fnGetRange,                  NOPARAM,                     "RANGE?",                                      "RANGE?",                                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  771 */  { itemToBeCoded,               NOPARAM,                     STD_chi STD_SUP_2 STD_SUB_p "(x)",             STD_chi STD_SUP_2 STD_SUB_p "(x)",             0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  772 */  { itemToBeCoded,               NOPARAM,                     STD_chi STD_SUP_2 STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)",
                                                                                                                       STD_chi STD_SUP_2 STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)",
                                                                                                                                                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  773 */  { itemToBeCoded,               NOPARAM,                     STD_chi STD_SUP_2 STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)",
                                                                                                                       STD_chi STD_SUP_2 STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)",
                                                                                                                                                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  774 */  { itemToBeCoded,               NOPARAM,                     "(" STD_chi STD_SUP_2 ")" STD_SUP_MINUS_1,     "(" STD_chi STD_SUP_2 ")" STD_SUP_MINUS_1,     0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  775 */  { itemToBeCoded,               NOPARAM,                     STD_chi STD_SUP_2 ":",                         STD_chi STD_SUP_2 ":",                         0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  776 */  { fnConstant,                  75,                          STD_omega,                                     STD_omega,                                     0,       0,       CAT_CNST, SLS_ENABLED  },
/*  777 */  { fnM1Pow,                     NOPARAM,                     "(-1)" STD_SUP_x,                              "(-1)" STD_SUP_x,                              0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  778 */  { fnAdd,                       ITM_ADD,                     "+",                                           "+",                                           0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  779 */  { fnChangeSign,                ITM_CHS,                     "+/-",                                         "+/-",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  780 */  { fnSubtract,                  ITM_SUB,                     "-",                                           "-",                                           0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  781 */  { fnConstant,                  76,                          "-" STD_INFINITY,                              "-" STD_INFINITY,                              0,       0,       CAT_CNST, SLS_ENABLED  },
/*  782 */  { fnMultiply,                  ITM_MULT,                    STD_CROSS,                                     STD_CROSS,                                     0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  783 */  { itemToBeCoded,               NOPARAM,                     STD_CROSS "MOD",                               STD_CROSS "MOD",                               0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  784 */  { fnDivide,                    ITM_DIV,                     "/",                                           "/",                                           0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  785 */  { itemToBeCoded,               NOPARAM,                     STD_PLUS_MINUS STD_INFINITY "?",               STD_PLUS_MINUS STD_INFINITY "?",               0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  786 */  { addItemToBuffer,             ITM_INDIRECTION,             STD_RIGHT_ARROW,                               STD_RIGHT_ARROW,                               0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  787 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "DATE",                        STD_RIGHT_ARROW "DATE",                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  788 */  { fnCvtFromCurrentAngularMode, AM_DEGREE,                   STD_RIGHT_ARROW "DEG",                         STD_RIGHT_ARROW "DEG",                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  789 */  { fnCvtFromCurrentAngularMode, AM_DMS,                      STD_RIGHT_ARROW "D.MS",                        STD_RIGHT_ARROW "D.MS",                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  790 */  { fnCvtFromCurrentAngularMode, AM_GRAD,                     STD_RIGHT_ARROW "GRAD",                        STD_RIGHT_ARROW "GRAD",                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  791 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "HR",                          ".d",                                          0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  792 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "H.MS",                        "h.ms",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  793 */  { fnChangeBase,                TM_VALUE_CHB,                STD_RIGHT_ARROW "INT",                         "#",                                           2,      16,       CAT_FNCT, SLS_UNCHANGED},
/*  794 */  { fnCvtFromCurrentAngularMode, AM_MULTPI,                   STD_RIGHT_ARROW "MUL" STD_pi,                  STD_RIGHT_ARROW "MUL" STD_pi,                  0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  795 */  { fnToPolar,                   NOPARAM,                     STD_RIGHT_ARROW "POL",                         STD_RIGHT_ARROW "P",                           0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  796 */  { fnCvtFromCurrentAngularMode, AM_RADIAN,                   STD_RIGHT_ARROW "RAD",                         STD_RIGHT_ARROW "RAD",                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  797 */  { fnToReal,                    NOPARAM,                     STD_RIGHT_ARROW "REAL",                        ".d",                                          0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  798 */  { fnToRect,                    NOPARAM,                     STD_RIGHT_ARROW "REC",                         "R" STD_LEFT_ARROW,                            0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  799 */  { fnCvtDegToDms,               NOPARAM,                     "D" STD_RIGHT_ARROW "D.MS",                    "D" STD_RIGHT_ARROW "D.MS",                    0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  800 */  { itemToBeCoded,               NOPARAM,                     STD_UP_ARROW "Lim",                            STD_UP_ARROW "Lim",                            0,       0,       CAT_RVAR, SLS_UNCHANGED},
/*  801 */  { itemToBeCoded,               NOPARAM,                     STD_DOWN_ARROW "Lim",                          STD_DOWN_ARROW "Lim",                          0,       0,       CAT_RVAR, SLS_UNCHANGED},
/*  802 */  { itemToBeCoded,               NOPARAM,                     STD_LEFT_RIGHT_ARROWS,                         STD_LEFT_RIGHT_ARROWS,                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  803 */  { fnPercent,                   NOPARAM,                     "%",                                           "%",                                           0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  804 */  { fnPercentMRR,                NOPARAM,                     "%MRR",                                        "%MRR",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  805 */  { fnPercentT,                  NOPARAM,                     "%T",                                          "%T",                                          0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  806 */  { fnPercentSigma,              NOPARAM,                     "%" STD_SIGMA,                                 "%" STD_SIGMA,                                 0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  807 */  { fnPercentPlusMG,             NOPARAM,                     "%+MG",                                        "%+MG",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  808 */  { fnSquareRoot,                NOPARAM,                     STD_SQUARE_ROOT STD_x_UNDER_ROOT,              STD_SQUARE_ROOT STD_x_UNDER_ROOT,              0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  809 */  { itemToBeCoded,               NOPARAM,                     STD_INTEGRAL,                                  STD_INTEGRAL,                                  0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  810 */  { itemToBeCoded,               NOPARAM,                     STD_INTEGRAL "f",                              STD_INTEGRAL "f",                              0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  811 */  { itemToBeCoded,               NOPARAM,                     STD_INTEGRAL "fdx",                            STD_INTEGRAL "fdx",                            0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  812 */  { fnConstant,                  77,                          STD_INFINITY,                                  STD_INFINITY,                                  0,       0,       CAT_CNST, SLS_ENABLED  },
/*  813 */  { itemToBeCoded,               NOPARAM,                     "^MOD",                                        "^MOD",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  814 */  { itemToBeCoded,               NOPARAM,                     "|M|",                                         "|M|",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  815 */  { fnMagnitude,                 NOPARAM,                     "|x|",                                         "|x|",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  816 */  { fnParallel,                  NOPARAM,                     "||",                                          "|" STD_SPACE_3_PER_EM "|",                    0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  817 */  { itemToBeCoded,               NOPARAM,                     "[M]" STD_SUP_T,                               "[M]" STD_SUP_T,                               0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  818 */  { itemToBeCoded,               NOPARAM,                     "[M]" STD_SUP_MINUS_1,                         "[M]" STD_SUP_MINUS_1,                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  819 */  { fnArg,                       NOPARAM,                     STD_MEASURED_ANGLE,                            STD_MEASURED_ANGLE,                            0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  820 */  { fnCvtToCurrentAngularMode,   AM_MULTPI,                   "MUL" STD_pi STD_RIGHT_ARROW,                  "MUL" STD_pi STD_RIGHT_ARROW,                  0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  821 */  { itemToBeCoded,               NOPARAM,                     STD_MEASURED_ANGLE STD_RIGHT_ARROW,            STD_MEASURED_ANGLE STD_RIGHT_ARROW,            0,       0,       CAT_MENU, SLS_UNCHANGED},
/*  822 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "ADV",                             STD_PRINTER "ADV",                             0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  823 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "CHAR",                            STD_PRINTER "CHAR",                            0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  824 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "DLAY",                            STD_PRINTER "DLAY",                            0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  825 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "LCD",                             STD_PRINTER "LCD",                             0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  826 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "MODE",                            STD_PRINTER "MODE",                            0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  827 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "PROG",                            STD_PRINTER "PROG",                            0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  828 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "r",                               STD_PRINTER "r",                               0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  829 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "REGS",                            STD_PRINTER "REGS",                            0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  830 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "STK",                             STD_PRINTER "STK",                             0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  831 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "TAB",                             STD_PRINTER "TAB",                             0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  832 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "USER",                            STD_PRINTER "USER",                            0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  833 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "WIDTH",                           STD_PRINTER "WIDTH",                           0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  834 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER STD_SIGMA,                         STD_PRINTER STD_SIGMA,                         0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  835 */  { itemToBeCoded,               NOPARAM,                     STD_PRINTER "#",                               STD_PRINTER "#",                               0,       0,       CAT_FNCT, SLS_UNCHANGED},
/*  836 */  { itemToBeCoded,               NOPARAM,                     "#",                                           "#",                                           0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  837 */  { fnCountBits,                 NOPARAM,                     "#B",                                          "#B",                                          0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  838 */  { fnCvtAcreM2,                 multiply,                    "ac" STD_RIGHT_ARROW "m" STD_SUP_2,            STD_RIGHT_ARROW " m" STD_SUP_2,                0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  839 */  { fnCvtAcreusM2,               multiply,                    "ac" STD_US STD_RIGHT_ARROW "m" STD_SUP_2,     STD_RIGHT_ARROW " m" STD_SUP_2,                0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  840 */  { fnCvtCaratKg,                multiply,                    "ct" STD_RIGHT_ARROW "kg",                     "carat",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  841 */  { fnCvtDbRatio,                20,                          "dB" STD_RIGHT_ARROW "fr",                     "field",                                       0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  842 */  { fnCvtDbRatio,                20,                          "dB" STD_RIGHT_ARROW "fr",                     "ratio",                                       0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  843 */  { fnCvtDbRatio,                10,                          "dB" STD_RIGHT_ARROW "pr",                     "power",                                       0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  844 */  { fnCvtDbRatio,                10,                          "dB" STD_RIGHT_ARROW "pr",                     "ratio",                                       0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  845 */  { fnCvtRatioDb,                20,                          "fr" STD_RIGHT_ARROW "dB",                     "ratio",                                       0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  846 */  { fnCvtRatioDb,                20,                          "fr" STD_RIGHT_ARROW "dB",                     STD_RIGHT_ARROW " dB",                         0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  847 */  { fnCvtSfeetM,                 multiply,                    "ft" STD_US STD_RIGHT_ARROW "m",               "foot" STD_US,                                 0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  848 */  { fnCvtFlozukM3,               multiply,                    "fz" STD_UK STD_RIGHT_ARROW "m" STD_SUP_3,     STD_RIGHT_ARROW " m" STD_SUP_3,                0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  849 */  { fnCvtFlozusM3,               multiply,                    "fz" STD_US STD_RIGHT_ARROW "m" STD_SUP_3,     STD_RIGHT_ARROW " m" STD_SUP_3,                0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  850 */  { fnCvtInhgPa,                 multiply,                    "iHg" STD_RIGHT_ARROW "Pa",                    STD_RIGHT_ARROW " Pa",                         0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  851 */  { fnCvtShortcwtKg,             divide,                      "kg" STD_RIGHT_ARROW "scw",                    "sh.cwt",                                      0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  852 */  { fnCvtStoneKg,                divide,                      "kg" STD_RIGHT_ARROW "sto",                    "stone",                                       0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  853 */  { fnCvtShorttonKg,             divide,                      "kg" STD_RIGHT_ARROW "s.t",                    "short",                                       0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  854 */  { fnCvtShorttonKg,             divide,                      "kg" STD_RIGHT_ARROW "s.t",                    "ton",                                         0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  855 */  { fnCvtCaratKg,                divide,                      "kg" STD_RIGHT_ARROW "ct",                     "kg " STD_RIGHT_ARROW,                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/*  856 */  { fnCvtTrozKg,                 divide,                      "kg" STD_RIGHT_ARROW "trz",                    "tr.oz",                                       0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  857 */  { fnCvtAcreM2,                 divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ac",            "acre",                                        0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  858 */  { fnCvtAcreusM2,               divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ac" STD_US,     "acre" STD_US,                                 0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  859 */  { fnCvtFlozukM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "fz" STD_UK,     "floz" STD_UK,                                 0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  860 */  { fnCvtFlozusM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "fz" STD_US,     "floz" STD_US,                                 0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  861 */  { fnCvtSfeetM,                 divide,                      "m" STD_RIGHT_ARROW "ft" STD_US,               "survey",                                      0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  862 */  { fnCvtCaratKg,                multiply,                    "ct" STD_RIGHT_ARROW "kg",                     STD_RIGHT_ARROW " kg",                         0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  863 */  { fnCvtInhgPa,                 divide,                      "Pa" STD_RIGHT_ARROW "iHg",                    "in.Hg",                                       0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  864 */  { fnCvtTorrPa,                 divide,                      "Pa" STD_RIGHT_ARROW "tor",                    "torr",                                        0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  865 */  { fnCvtRatioDb,                10,                          "pr" STD_RIGHT_ARROW "dB",                     "ratio",                                       0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  866 */  { fnCvtRatioDb,                10,                          "pr" STD_RIGHT_ARROW "dB",                     STD_RIGHT_ARROW " dB",                         0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  867 */  { fnCvtShortcwtKg,             multiply,                    "scw" STD_RIGHT_ARROW "kg",                    "cwt" STD_RIGHT_ARROW "kg",                    0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  868 */  { fnCvtStoneKg,                multiply,                    "sto" STD_RIGHT_ARROW "kg",                    STD_RIGHT_ARROW " kg",                         0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  869 */  { fnCvtShorttonKg,             multiply,                    "s.t" STD_RIGHT_ARROW "kg",                    "ton",                                         0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  870 */  { fnCvtShorttonKg,             multiply,                    "s.t" STD_RIGHT_ARROW "kg",                    STD_RIGHT_ARROW " kg",                         0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  871 */  { fnCvtCaratKg,                divide,                      "kg" STD_RIGHT_ARROW "ct",                     "carat",                                       0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  872 */  { fnCvtTorrPa,                 multiply,                    "tor" STD_RIGHT_ARROW "Pa",                    STD_RIGHT_ARROW " Pa",                         0,       0,       CAT_DUPL, SLS_ENABLED  },
/*  873 */  { fnCvtTrozKg,                 multiply,                    "trz" STD_RIGHT_ARROW "kg",                    STD_RIGHT_ARROW " kg",                         0,       0,       CAT_DUPL, SLS_ENABLED  },

/*  874 */  { fontBrowser,                 NOPARAM,                     "FBR",                                         "FBR",                                         0,       0,       CAT_FNCT, SLS_UNCHANGED}, // Font Browser
/*  875 */  { itemToBeCoded,               NOPARAM,                     "0875",                                        "0875",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  876 */  { itemToBeCoded,               NOPARAM,                     STD_alpha STD_ELLIPSIS STD_omega,              STD_alpha STD_ELLIPSIS STD_omega,              0,       0,       CAT_MENU, SLS_UNCHANGED}, // Small greek letters
/*  877 */  { itemToBeCoded,               NOPARAM,                     STD_alpha "intl",                              STD_alpha "intl",                              0,       0,       CAT_MENU, SLS_UNCHANGED}, // Small intl letters
/*  878 */  { addItemToBuffer,             REGISTER_X,                  "REG_X",                                       "X",                                           0,       0,       CAT_NONE, SLS_UNCHANGED}, // The order
/*  879 */  { addItemToBuffer,             REGISTER_Y,                  "REG_Y",                                       "Y",                                           0,       0,       CAT_NONE, SLS_UNCHANGED}, // of these 8
/*  880 */  { addItemToBuffer,             REGISTER_Z,                  "REG_Z",                                       "Z",                                           0,       0,       CAT_NONE, SLS_UNCHANGED}, // lines MUST
/*  881 */  { addItemToBuffer,             REGISTER_T,                  "REG_T",                                       "T",                                           0,       0,       CAT_NONE, SLS_UNCHANGED}, // be kept as
/*  882 */  { addItemToBuffer,             REGISTER_A,                  "REG_A",                                       "A",                                           0,       0,       CAT_REGS, SLS_UNCHANGED}, // is. Do not
/*  883 */  { addItemToBuffer,             REGISTER_B,                  "REG_B",                                       "B",                                           0,       0,       CAT_REGS, SLS_UNCHANGED}, // put them in
/*  884 */  { addItemToBuffer,             REGISTER_C,                  "REG_C",                                       "C",                                           0,       0,       CAT_REGS, SLS_UNCHANGED}, // alphabetical
/*  885 */  { addItemToBuffer,             REGISTER_D,                  "REG_D",                                       "D",                                           0,       0,       CAT_REGS, SLS_UNCHANGED}, // order!
/*  886 */  { addItemToBuffer,             REGISTER_L,                  "REG_L",                                       "L",                                           0,       0,       CAT_REGS, SLS_UNCHANGED},
/*  887 */  { addItemToBuffer,             REGISTER_I,                  "REG_I",                                       "I",                                           0,       0,       CAT_REGS, SLS_UNCHANGED},
/*  888 */  { addItemToBuffer,             REGISTER_J,                  "REG_J",                                       "J",                                           0,       0,       CAT_REGS, SLS_UNCHANGED},
/*  889 */  { addItemToBuffer,             REGISTER_K,                  "REG_K",                                       "K",                                           0,       0,       CAT_REGS, SLS_UNCHANGED},

/*  890 */  { addItemToBuffer,             CHR_0,                       "",                                            "0",                                           0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  891 */  { addItemToBuffer,             CHR_1,                       "",                                            "1",                                           0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  892 */  { addItemToBuffer,             CHR_2,                       "",                                            "2",                                           0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  893 */  { addItemToBuffer,             CHR_3,                       "",                                            "3",                                           0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  894 */  { addItemToBuffer,             CHR_4,                       "",                                            "4",                                           0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  895 */  { addItemToBuffer,             CHR_5,                       "",                                            "5",                                           0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  896 */  { addItemToBuffer,             CHR_6,                       "",                                            "6",                                           0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  897 */  { addItemToBuffer,             CHR_7,                       "",                                            "7",                                           0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  898 */  { addItemToBuffer,             CHR_8,                       "",                                            "8",                                           0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  899 */  { addItemToBuffer,             CHR_9,                       "",                                            "9",                                           0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  900 */  { addItemToBuffer,             CHR_A,                       "A",                                           "A",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  901 */  { addItemToBuffer,             CHR_B,                       "B",                                           "B",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  902 */  { addItemToBuffer,             CHR_C,                       "C",                                           "C",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  903 */  { addItemToBuffer,             CHR_D,                       "D",                                           "D",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  904 */  { addItemToBuffer,             CHR_E,                       "E",                                           "E",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  905 */  { addItemToBuffer,             CHR_F,                       "F",                                           "F",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  906 */  { addItemToBuffer,             CHR_G,                       "G",                                           "G",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  907 */  { addItemToBuffer,             CHR_H,                       "H",                                           "H",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  908 */  { addItemToBuffer,             CHR_I,                       "I",                                           "I",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  909 */  { addItemToBuffer,             CHR_J,                       "J",                                           "J",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  910 */  { addItemToBuffer,             CHR_K,                       "K",                                           "K",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  911 */  { addItemToBuffer,             CHR_L,                       "L",                                           "L",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  912 */  { addItemToBuffer,             CHR_M,                       "M",                                           "M",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  913 */  { addItemToBuffer,             CHR_N,                       "N",                                           "N",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  914 */  { addItemToBuffer,             CHR_O,                       "O",                                           "O",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  915 */  { addItemToBuffer,             CHR_P,                       "P",                                           "P",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  916 */  { addItemToBuffer,             CHR_Q,                       "Q",                                           "Q",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  917 */  { addItemToBuffer,             CHR_R,                       "R",                                           "R",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  918 */  { addItemToBuffer,             CHR_S,                       "S",                                           "S",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  919 */  { addItemToBuffer,             CHR_T,                       "T",                                           "T",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  920 */  { addItemToBuffer,             CHR_U,                       "U",                                           "U",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  921 */  { addItemToBuffer,             CHR_V,                       "V",                                           "V",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  922 */  { addItemToBuffer,             CHR_W,                       "W",                                           "W",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  923 */  { addItemToBuffer,             CHR_X,                       "X",                                           "X",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  924 */  { addItemToBuffer,             CHR_Y,                       "Y",                                           "Y",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  925 */  { addItemToBuffer,             CHR_Z,                       "Z",                                           "Z",                                           0,       0,       CAT_AINT, SLS_UNCHANGED},
/*  926 */  { addItemToBuffer,             CHR_a,                       "a",                                           "a",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  927 */  { addItemToBuffer,             CHR_b,                       "b",                                           "b",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  928 */  { addItemToBuffer,             CHR_c,                       "c",                                           "c",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  929 */  { addItemToBuffer,             CHR_d,                       "d",                                           "d",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  930 */  { addItemToBuffer,             CHR_e,                       "e",                                           "e",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  931 */  { addItemToBuffer,             CHR_f,                       "f",                                           "f",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  932 */  { addItemToBuffer,             CHR_g,                       "g",                                           "g",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  933 */  { addItemToBuffer,             CHR_h,                       "h",                                           "h",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  934 */  { addItemToBuffer,             CHR_i,                       "i",                                           "i",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  935 */  { addItemToBuffer,             CHR_j,                       "j",                                           "j",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  936 */  { addItemToBuffer,             CHR_k,                       "k",                                           "k",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  937 */  { addItemToBuffer,             CHR_l,                       "l",                                           "l",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  938 */  { addItemToBuffer,             CHR_m,                       "m",                                           "m",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  939 */  { addItemToBuffer,             CHR_n,                       "n",                                           "n",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  940 */  { addItemToBuffer,             CHR_o,                       "o",                                           "o",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  941 */  { addItemToBuffer,             CHR_p,                       "p",                                           "p",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  942 */  { addItemToBuffer,             CHR_q,                       "q",                                           "q",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  943 */  { addItemToBuffer,             CHR_r,                       "r",                                           "r",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  944 */  { addItemToBuffer,             CHR_s,                       "s",                                           "s",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  945 */  { addItemToBuffer,             CHR_t,                       "t",                                           "t",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  946 */  { addItemToBuffer,             CHR_u,                       "u",                                           "u",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  947 */  { addItemToBuffer,             CHR_v,                       "v",                                           "v",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  948 */  { addItemToBuffer,             CHR_w,                       "w",                                           "w",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  949 */  { addItemToBuffer,             CHR_x,                       "x",                                           "x",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  950 */  { addItemToBuffer,             CHR_y,                       "y",                                           "y",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  951 */  { addItemToBuffer,             CHR_z,                       "z",                                           "z",                                           0,       0,       CAT_aint, SLS_UNCHANGED},
/*  952 */  { addItemToBuffer,             CHR_ALPHA,                   "",                                            STD_ALPHA,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  953 */  { itemToBeCoded,               NOPARAM,                     "0953",                                        "0953",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  954 */  { addItemToBuffer,             CHR_BETA,                    "",                                            STD_BETA,                                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  955 */  { addItemToBuffer,             CHR_GAMMA,                   "",                                            STD_GAMMA,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  956 */  { addItemToBuffer,             CHR_DELTA,                   "",                                            STD_DELTA,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  957 */  { addItemToBuffer,             CHR_EPSILON,                 "",                                            STD_EPSILON,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  958 */  { itemToBeCoded,               NOPARAM,                     "0958",                                        "0958",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  959 */  { addItemToBuffer,             CHR_ZETA,                    "",                                            STD_ZETA,                                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  960 */  { addItemToBuffer,             CHR_ETA,                     "",                                            STD_ETA,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  961 */  { itemToBeCoded,               NOPARAM,                     "",                                            "0961",                                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  962 */  { addItemToBuffer,             CHR_THETA,                   "",                                            STD_THETA,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  963 */  { addItemToBuffer,             CHR_IOTA,                    "",                                            STD_IOTA,                                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  964 */  { itemToBeCoded,               NOPARAM,                     "0964",                                        "0964",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  965 */  { itemToBeCoded,               NOPARAM,                     "0965",                                        "0965",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  966 */  { addItemToBuffer,             CHR_IOTA_DIALYTIKA,          "",                                            STD_IOTA_DIALYTIKA,                            0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  967 */  { addItemToBuffer,             CHR_KAPPA,                   "",                                            STD_KAPPA,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  968 */  { addItemToBuffer,             CHR_LAMBDA,                  "",                                            STD_LAMBDA,                                    0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  969 */  { addItemToBuffer,             CHR_MU,                      "",                                            STD_MU,                                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  970 */  { addItemToBuffer,             CHR_NU,                      "",                                            STD_NU,                                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  971 */  { addItemToBuffer,             CHR_XI,                      "",                                            STD_XI,                                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  972 */  { addItemToBuffer,             CHR_OMICRON,                 "",                                            STD_OMICRON,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  973 */  { itemToBeCoded,               NOPARAM,                     "0973",                                        "0973",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  974 */  { addItemToBuffer,             CHR_PI,                      "",                                            STD_PI,                                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  975 */  { addItemToBuffer,             CHR_RHO,                     "",                                            STD_RHO,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  976 */  { addItemToBuffer,             CHR_SIGMA,                   "",                                            STD_SIGMA,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  977 */  { itemToBeCoded,               NOPARAM,                     "0977",                                        "0977",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  978 */  { addItemToBuffer,             CHR_TAU,                     "",                                            STD_TAU,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  979 */  { addItemToBuffer,             CHR_UPSILON,                 "",                                            STD_UPSILON,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  980 */  { itemToBeCoded,               NOPARAM,                     "0980",                                        "0980",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  981 */  { addItemToBuffer,             CHR_UPSILON_DIALYTIKA,       "",                                            STD_UPSILON_DIALYTIKA,                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  982 */  { itemToBeCoded,               NOPARAM,                     "0982",                                        "0982",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  983 */  { addItemToBuffer,             CHR_PHI,                     "",                                            STD_PHI,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  984 */  { addItemToBuffer,             CHR_CHI,                     "",                                            STD_CHI,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  985 */  { addItemToBuffer,             CHR_PSI,                     "",                                            STD_PSI,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  986 */  { addItemToBuffer,             CHR_OMEGA,                   "",                                            STD_OMEGA,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  987 */  { itemToBeCoded,               NOPARAM,                     "0987",                                        "0987",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/*  988 */  { addItemToBuffer,             CHR_alpha,                   "",                                            STD_alpha,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  989 */  { addItemToBuffer,             CHR_alpha_TONOS,             "",                                            STD_alpha_TONOS,                               0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  990 */  { addItemToBuffer,             CHR_beta,                    "",                                            STD_beta,                                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  991 */  { addItemToBuffer,             CHR_gamma,                   "",                                            STD_gamma,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  992 */  { addItemToBuffer,             CHR_delta,                   "",                                            STD_delta,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  993 */  { addItemToBuffer,             CHR_epsilon,                 "",                                            STD_epsilon,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  994 */  { addItemToBuffer,             CHR_epsilon_TONOS,           "",                                            STD_epsilon_TONOS,                             0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  995 */  { addItemToBuffer,             CHR_zeta,                    "",                                            STD_zeta,                                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  996 */  { addItemToBuffer,             CHR_eta,                     "",                                            STD_eta,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  997 */  { addItemToBuffer,             CHR_eta_TONOS,               "",                                            STD_eta_TONOS,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  998 */  { addItemToBuffer,             CHR_theta,                   "",                                            STD_theta,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/*  999 */  { addItemToBuffer,             CHR_iota,                    "",                                            STD_iota,                                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1000 */  { addItemToBuffer,             CHR_iotaTON,                 "",                                            STD_iota_TONOS,                                0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1001 */  { addItemToBuffer,             CHR_iota_DIALYTIKA_TONOS,    "",                                            STD_iota_DIALYTIKA_TONOS,                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1002 */  { addItemToBuffer,             CHR_iota_DIALYTIKA,          "",                                            STD_iota_DIALYTIKA,                            0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1003 */  { addItemToBuffer,             CHR_kappa,                   "",                                            STD_kappa,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1004 */  { addItemToBuffer,             CHR_lambda,                  "",                                            STD_lambda,                                    0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1005 */  { addItemToBuffer,             CHR_mu,                      "",                                            STD_mu,                                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1006 */  { addItemToBuffer,             CHR_nu,                      "",                                            STD_nu,                                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1007 */  { addItemToBuffer,             CHR_xi,                      "",                                            STD_xi,                                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1008 */  { addItemToBuffer,             CHR_omicron,                 "",                                            STD_omicron,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1009 */  { addItemToBuffer,             CHR_omicron_TONOS,           "",                                            STD_omicron_TONOS,                             0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1010 */  { addItemToBuffer,             CHR_pi,                      "",                                            STD_pi,                                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1011 */  { addItemToBuffer,             CHR_rho,                     "",                                            STD_rho,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1012 */  { addItemToBuffer,             CHR_sigma,                   "",                                            STD_sigma,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1013 */  { addItemToBuffer,             CHR_sigma_end,               "",                                            STD_sigma_end,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1014 */  { addItemToBuffer,             CHR_tau,                     "",                                            STD_tau,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1015 */  { addItemToBuffer,             CHR_upsilon,                 "",                                            STD_upsilon,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1016 */  { addItemToBuffer,             CHR_upsilon_TONOS,           "",                                            STD_upsilon_TONOS,                             0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1017 */  { addItemToBuffer,             CHR_upsilon_DIALYTIKA,       "",                                            STD_upsilon_DIALYTIKA,                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1018 */  { addItemToBuffer,             CHR_upsilon_DIALYTIKA_TONOS, "",                                            STD_upsilon_DIALYTIKA_TONOS,                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1019 */  { addItemToBuffer,             CHR_phi,                     "",                                            STD_phi,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1020 */  { addItemToBuffer,             CHR_chi,                     "",                                            STD_chi,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1021 */  { addItemToBuffer,             CHR_psi,                     "",                                            STD_psi,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1022 */  { addItemToBuffer,             CHR_omega,                   "",                                            STD_omega,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1023 */  { addItemToBuffer,             CHR_omega_TONOS,             "",                                            STD_omega_TONOS,                               0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1024 */  { itemToBeCoded,               NOPARAM,                     "1024",                                        "1024",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1025 */  { itemToBeCoded,               NOPARAM,                     "1025",                                        "1025",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1026 */  { itemToBeCoded,               NOPARAM,                     "1026",                                        "1026",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1027 */  { itemToBeCoded,               NOPARAM,                     "1027",                                        "1027",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1028 */  { itemToBeCoded,               NOPARAM,                     "1028",                                        "1028",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1029 */  { itemToBeCoded,               NOPARAM,                     "1029",                                        "1029",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1030 */  { addItemToBuffer,             CHR_A_MACRON,                STD_A_MACRON,                                  STD_A_MACRON,                                  0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1031 */  { addItemToBuffer,             CHR_A_ACUTE,                 STD_A_ACUTE,                                   STD_A_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1032 */  { addItemToBuffer,             CHR_A_BREVE,                 STD_A_BREVE,                                   STD_A_BREVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1033 */  { addItemToBuffer,             CHR_A_GRAVE,                 STD_A_GRAVE,                                   STD_A_GRAVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1034 */  { addItemToBuffer,             CHR_A_DIARESIS,              STD_A_DIARESIS,                                STD_A_DIARESIS,                                0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1035 */  { addItemToBuffer,             CHR_A_TILDE,                 STD_A_TILDE,                                   STD_A_TILDE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1036 */  { addItemToBuffer,             CHR_A_CIRC,                  STD_A_CIRC,                                    STD_A_CIRC,                                    0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1037 */  { addItemToBuffer,             CHR_A_RING,                  STD_A_RING,                                    STD_A_RING,                                    0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1038 */  { addItemToBuffer,             CHR_AE,                      STD_AE,                                        STD_AE,                                        0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1039 */  { addItemToBuffer,             CHR_A_OGONEK,                STD_A_OGONEK,                                  STD_A_OGONEK,                                  0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1040 */  { addItemToBuffer,             CHR_C_ACUTE,                 STD_C_ACUTE,                                   STD_C_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1041 */  { addItemToBuffer,             CHR_C_CARON,                 STD_C_CARON,                                   STD_C_CARON,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1042 */  { addItemToBuffer,             CHR_C_CEDILLA,               STD_C_CEDILLA,                                 STD_C_CEDILLA,                                 0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1043 */  { addItemToBuffer,             CHR_D_STROKE,                STD_D_STROKE,                                  STD_D_STROKE,                                  0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1044 */  { addItemToBuffer,             CHR_D_CARON,                 STD_D_CARON,                                   STD_D_CARON,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1045 */  { addItemToBuffer,             CHR_E_MACRON,                STD_E_MACRON,                                  STD_E_MACRON,                                  0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1046 */  { addItemToBuffer,             CHR_E_ACUTE,                 STD_E_ACUTE,                                   STD_E_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1047 */  { addItemToBuffer,             CHR_E_BREVE,                 STD_E_BREVE,                                   STD_E_BREVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1048 */  { addItemToBuffer,             CHR_E_GRAVE,                 STD_E_GRAVE,                                   STD_E_GRAVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1049 */  { addItemToBuffer,             CHR_E_DIARESIS,              STD_E_DIARESIS,                                STD_E_DIARESIS,                                0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1050 */  { addItemToBuffer,             CHR_E_CIRC,                  STD_E_CIRC,                                    STD_E_CIRC,                                    0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1051 */  { addItemToBuffer,             CHR_E_OGONEK,                STD_E_OGONEK,                                  STD_E_OGONEK,                                  0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1052 */  { addItemToBuffer,             CHR_G_BREVE,                 STD_G_BREVE,                                   STD_G_BREVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1053 */  { itemToBeCoded,               NOPARAM,                     "1053",                                        "1053",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1054 */  { addItemToBuffer,             CHR_I_MACRON,                STD_I_MACRON,                                  STD_I_MACRON,                                  0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1055 */  { addItemToBuffer,             CHR_I_ACUTE,                 STD_I_ACUTE,                                   STD_I_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1056 */  { addItemToBuffer,             CHR_I_BREVE,                 STD_I_BREVE,                                   STD_I_BREVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1057 */  { addItemToBuffer,             CHR_I_GRAVE,                 STD_I_GRAVE,                                   STD_I_GRAVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1058 */  { addItemToBuffer,             CHR_I_DIARESIS,              STD_I_DIARESIS,                                STD_I_DIARESIS,                                0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1059 */  { addItemToBuffer,             CHR_I_CIRC,                  STD_I_CIRC,                                    STD_I_CIRC,                                    0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1060 */  { addItemToBuffer,             CHR_I_OGONEK,                STD_I_OGONEK,                                  STD_I_OGONEK,                                  0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1061 */  { addItemToBuffer,             CHR_I_DOT,                   STD_I_DOT,                                     STD_I_DOT,                                     0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1062 */  { addItemToBuffer,             CHR_I_DOTLESS,               "I",                                           "I",                                           0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1063 */  { addItemToBuffer,             CHR_L_STROKE,                STD_L_STROKE,                                  STD_L_STROKE,                                  0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1064 */  { addItemToBuffer,             CHR_L_ACUTE,                 STD_L_ACUTE,                                   STD_L_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1065 */  { addItemToBuffer,             CHR_L_APOSTROPHE,            STD_L_APOSTROPHE,                              STD_L_APOSTROPHE,                              0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1066 */  { addItemToBuffer,             CHR_N_ACUTE,                 STD_N_ACUTE,                                   STD_N_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1067 */  { addItemToBuffer,             CHR_N_CARON,                 STD_N_CARON,                                   STD_N_CARON,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1068 */  { addItemToBuffer,             CHR_N_TILDE,                 STD_N_TILDE,                                   STD_N_TILDE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1069 */  { addItemToBuffer,             CHR_O_MACRON,                STD_O_MACRON,                                  STD_O_MACRON,                                  0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1070 */  { addItemToBuffer,             CHR_O_ACUTE,                 STD_O_ACUTE,                                   STD_O_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1071 */  { addItemToBuffer,             CHR_O_BREVE,                 STD_O_BREVE,                                   STD_O_BREVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1072 */  { addItemToBuffer,             CHR_O_GRAVE,                 STD_O_GRAVE,                                   STD_O_GRAVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1073 */  { addItemToBuffer,             CHR_O_DIARESIS,              STD_O_DIARESIS,                                STD_O_DIARESIS,                                0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1074 */  { addItemToBuffer,             CHR_O_TILDE,                 STD_O_TILDE,                                   STD_O_TILDE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1075 */  { addItemToBuffer,             CHR_O_CIRC,                  STD_O_CIRC,                                    STD_O_CIRC,                                    0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1076 */  { addItemToBuffer,             CHR_O_STROKE,                STD_O_STROKE,                                  STD_O_STROKE,                                  0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1077 */  { addItemToBuffer,             CHR_OE,                      STD_OE,                                        STD_OE,                                        0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1078 */  { itemToBeCoded,               NOPARAM,                     "1078",                                        "1078",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1079 */  { itemToBeCoded,               NOPARAM,                     "1079",                                        "1079",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1080 */  { addItemToBuffer,             CHR_S_SHARP,                 STD_s_SHARP,                                   STD_s_SHARP,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1081 */  { addItemToBuffer,             CHR_S_ACUTE,                 STD_S_ACUTE,                                   STD_S_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1082 */  { addItemToBuffer,             CHR_S_CARON,                 STD_S_CARON,                                   STD_S_CARON,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1083 */  { addItemToBuffer,             CHR_S_CEDILLA,               STD_S_CEDILLA,                                 STD_S_CEDILLA,                                 0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1084 */  { addItemToBuffer,             CHR_T_CARON,                 STD_T_CARON,                                   STD_T_CARON,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1085 */  { addItemToBuffer,             CHR_T_CEDILLA,               STD_T_CEDILLA,                                 STD_T_CEDILLA,                                 0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1086 */  { addItemToBuffer,             CHR_U_MACRON,                STD_U_MACRON,                                  STD_U_MACRON,                                  0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1087 */  { addItemToBuffer,             CHR_U_ACUTE,                 STD_U_ACUTE,                                   STD_U_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1088 */  { addItemToBuffer,             CHR_U_BREVE,                 STD_U_BREVE,                                   STD_U_BREVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1089 */  { addItemToBuffer,             CHR_U_GRAVE,                 STD_U_GRAVE,                                   STD_U_GRAVE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1090 */  { addItemToBuffer,             CHR_U_DIARESIS,              STD_U_DIARESIS,                                STD_U_DIARESIS,                                0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1091 */  { addItemToBuffer,             CHR_U_TILDE,                 STD_U_TILDE,                                   STD_U_TILDE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1092 */  { addItemToBuffer,             CHR_U_CIRC,                  STD_U_CIRC,                                    STD_U_CIRC,                                    0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1093 */  { addItemToBuffer,             CHR_U_RING,                  STD_U_RING,                                    STD_U_RING,                                    0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1094 */  { addItemToBuffer,             CHR_W_CIRC,                  STD_W_CIRC,                                    STD_W_CIRC,                                    0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1095 */  { itemToBeCoded,               NOPARAM,                     "1095",                                        "1095",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1096 */  { itemToBeCoded,               NOPARAM,                     "1096",                                        "1096",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1097 */  { itemToBeCoded,               NOPARAM,                     "1097",                                        "1097",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1098 */  { addItemToBuffer,             CHR_Y_CIRC,                  STD_Y_CIRC,                                    STD_Y_CIRC,                                    0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1099 */  { addItemToBuffer,             CHR_Y_ACUTE,                 STD_Y_ACUTE,                                   STD_Y_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1100 */  { addItemToBuffer,             CHR_Y_DIARESIS,              STD_Y_DIARESIS,                                STD_Y_DIARESIS,                                0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1101 */  { addItemToBuffer,             CHR_Z_ACUTE,                 STD_Z_ACUTE,                                   STD_Z_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1102 */  { addItemToBuffer,             CHR_Z_CARON,                 STD_Z_CARON,                                   STD_Z_CARON,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1103 */  { addItemToBuffer,             CHR_Z_DOT,                   STD_Z_DOT,                                     STD_Z_DOT,                                     0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1104 */  { itemToBeCoded,               NOPARAM,                     "1104",                                        "1104",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1105 */  { itemToBeCoded,               NOPARAM,                     "1105",                                        "1105",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1106 */  { itemToBeCoded,               NOPARAM,                     "1106",                                        "1106",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1107 */  { itemToBeCoded,               NOPARAM,                     "1107",                                        "1107",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1108 */  { itemToBeCoded,               NOPARAM,                     "1108",                                        "1108",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1109 */  { itemToBeCoded,               NOPARAM,                     "1109",                                        "1109",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1110 */  { addItemToBuffer,             CHR_a_MACRON,                STD_a_MACRON,                                  STD_a_MACRON,                                  0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1111 */  { addItemToBuffer,             CHR_a_ACUTE,                 STD_a_ACUTE,                                   STD_a_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1112 */  { addItemToBuffer,             CHR_a_BREVE,                 STD_a_BREVE,                                   STD_a_BREVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1113 */  { addItemToBuffer,             CHR_a_GRAVE,                 STD_a_GRAVE,                                   STD_a_GRAVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1114 */  { addItemToBuffer,             CHR_a_DIARESIS,              STD_a_DIARESIS,                                STD_a_DIARESIS,                                0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1115 */  { addItemToBuffer,             CHR_a_TILDE,                 STD_a_TILDE,                                   STD_a_TILDE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1116 */  { addItemToBuffer,             CHR_a_CIRC,                  STD_a_CIRC,                                    STD_a_CIRC,                                    0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1117 */  { addItemToBuffer,             CHR_a_RING,                  STD_a_RING,                                    STD_a_RING,                                    0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1118 */  { addItemToBuffer,             CHR_ae,                      STD_ae,                                        STD_ae,                                        0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1119 */  { addItemToBuffer,             CHR_a_OGONEK,                STD_a_OGONEK,                                  STD_a_OGONEK,                                  0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1120 */  { addItemToBuffer,             CHR_c_ACUTE,                 STD_c_ACUTE,                                   STD_c_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1121 */  { addItemToBuffer,             CHR_c_CARON,                 STD_c_CARON,                                   STD_c_CARON,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1122 */  { addItemToBuffer,             CHR_c_CEDILLA,               STD_c_CEDILLA,                                 STD_c_CEDILLA,                                 0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1123 */  { addItemToBuffer,             CHR_d_STROKE,                STD_d_STROKE,                                  STD_d_STROKE,                                  0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1124 */  { addItemToBuffer,             CHR_d_APOSTROPHE,            STD_d_APOSTROPHE,                              STD_d_APOSTROPHE,                              0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1125 */  { addItemToBuffer,             CHR_e_MACRON,                STD_e_MACRON,                                  STD_e_MACRON,                                  0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1126 */  { addItemToBuffer,             CHR_e_ACUTE,                 STD_e_ACUTE,                                   STD_e_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1127 */  { addItemToBuffer,             CHR_e_BREVE,                 STD_e_BREVE,                                   STD_e_BREVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1128 */  { addItemToBuffer,             CHR_e_GRAVE,                 STD_e_GRAVE,                                   STD_e_GRAVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1129 */  { addItemToBuffer,             CHR_e_DIARESIS,              STD_e_DIARESIS,                                STD_e_DIARESIS,                                0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1130 */  { addItemToBuffer,             CHR_e_CIRC,                  STD_e_CIRC,                                    STD_e_CIRC,                                    0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1131 */  { addItemToBuffer,             CHR_e_OGONEK,                STD_e_OGONEK,                                  STD_e_OGONEK,                                  0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1132 */  { addItemToBuffer,             CHR_g_BREVE,                 STD_g_BREVE,                                   STD_g_BREVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1133 */  { addItemToBuffer,             CHR_h_STROKE,                "",                                            STD_h_STROKE,                                  0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1134 */  { addItemToBuffer,             CHR_i_MACRON,                STD_i_MACRON,                                  STD_i_MACRON,                                  0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1135 */  { addItemToBuffer,             CHR_i_ACUTE,                 STD_i_ACUTE,                                   STD_i_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1136 */  { addItemToBuffer,             CHR_i_BREVE,                 STD_i_BREVE,                                   STD_i_BREVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1137 */  { addItemToBuffer,             CHR_i_GRAVE,                 STD_i_GRAVE,                                   STD_i_GRAVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1138 */  { addItemToBuffer,             CHR_i_DIARESIS,              STD_i_DIARESIS,                                STD_i_DIARESIS,                                0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1139 */  { addItemToBuffer,             CHR_i_CIRC,                  STD_i_CIRC,                                    STD_i_CIRC,                                    0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1140 */  { addItemToBuffer,             CHR_i_OGONEK,                STD_i_OGONEK,                                  STD_i_OGONEK,                                  0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1141 */  { addItemToBuffer,             CHR_i_DOT,                   "i",                                           "i",                                           0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1142 */  { addItemToBuffer,             CHR_i_DOTLESS,               STD_i_DOTLESS,                                 STD_i_DOTLESS,                                 0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1143 */  { addItemToBuffer,             CHR_l_STROKE,                STD_l_STROKE,                                  STD_l_STROKE,                                  0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1144 */  { addItemToBuffer,             CHR_l_ACUTE,                 STD_l_ACUTE,                                   STD_l_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1145 */  { addItemToBuffer,             CHR_l_APOSTROPHE,            STD_l_APOSTROPHE,                              STD_l_APOSTROPHE,                              0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1146 */  { addItemToBuffer,             CHR_n_ACUTE,                 STD_n_ACUTE,                                   STD_n_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1147 */  { addItemToBuffer,             CHR_n_CARON,                 STD_n_CARON,                                   STD_n_CARON,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1148 */  { addItemToBuffer,             CHR_n_TILDE,                 STD_n_TILDE,                                   STD_n_TILDE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1149 */  { addItemToBuffer,             CHR_o_MACRON,                STD_o_MACRON,                                  STD_o_MACRON,                                  0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1150 */  { addItemToBuffer,             CHR_o_ACUTE,                 STD_o_ACUTE,                                   STD_o_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1151 */  { addItemToBuffer,             CHR_o_BREVE,                 STD_o_BREVE,                                   STD_o_BREVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1152 */  { addItemToBuffer,             CHR_o_GRAVE,                 STD_o_GRAVE,                                   STD_o_GRAVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1153 */  { addItemToBuffer,             CHR_o_DIARESIS,              STD_o_DIARESIS,                                STD_o_DIARESIS,                                0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1154 */  { addItemToBuffer,             CHR_o_TILDE,                 STD_o_TILDE,                                   STD_o_TILDE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1155 */  { addItemToBuffer,             CHR_o_CIRC,                  STD_o_CIRC,                                    STD_o_CIRC,                                    0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1156 */  { addItemToBuffer,             CHR_o_STROKE,                STD_o_STROKE,                                  STD_o_STROKE,                                  0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1157 */  { addItemToBuffer,             CHR_oe,                      STD_oe,                                        STD_oe,                                        0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1158 */  { addItemToBuffer,             CHR_r_CARON,                 STD_r_CARON,                                   STD_r_CARON,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1159 */  { addItemToBuffer,             CHR_r_ACUTE,                 STD_r_ACUTE,                                   STD_r_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1160 */  { addItemToBuffer,             CHR_s_SHARP,                 STD_s_SHARP,                                   STD_s_SHARP,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1161 */  { addItemToBuffer,             CHR_s_ACUTE,                 STD_s_ACUTE,                                   STD_s_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1162 */  { addItemToBuffer,             CHR_s_CARON,                 STD_s_CARON,                                   STD_s_CARON,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1163 */  { addItemToBuffer,             CHR_s_CEDILLA,               STD_s_CEDILLA,                                 STD_s_CEDILLA,                                 0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1164 */  { addItemToBuffer,             CHR_t_APOSTROPHE,            STD_t_APOSTROPHE,                              STD_t_APOSTROPHE,                              0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1165 */  { addItemToBuffer,             CHR_t_CEDILLA,               STD_t_CEDILLA,                                 STD_t_CEDILLA,                                 0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1166 */  { addItemToBuffer,             CHR_u_MACRON,                STD_u_MACRON,                                  STD_u_MACRON,                                  0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1167 */  { addItemToBuffer,             CHR_u_ACUTE,                 STD_u_ACUTE,                                   STD_u_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1168 */  { addItemToBuffer,             CHR_u_BREVE,                 STD_u_BREVE,                                   STD_u_BREVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1169 */  { addItemToBuffer,             CHR_u_GRAVE,                 STD_u_GRAVE,                                   STD_u_GRAVE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1170 */  { addItemToBuffer,             CHR_u_DIARESIS,              STD_u_DIARESIS,                                STD_u_DIARESIS,                                0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1171 */  { addItemToBuffer,             CHR_u_TILDE,                 STD_u_TILDE,                                   STD_u_TILDE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1172 */  { addItemToBuffer,             CHR_u_CIRC,                  STD_u_CIRC,                                    STD_u_CIRC,                                    0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1173 */  { addItemToBuffer,             CHR_u_RING,                  STD_u_RING,                                    STD_u_RING,                                    0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1174 */  { addItemToBuffer,             CHR_w_CIRC,                  STD_w_CIRC,                                    STD_w_CIRC,                                    0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1175 */  { addItemToBuffer,             CHR_x_BAR,                   "",                                            STD_x_BAR,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1176 */  { addItemToBuffer,             CHR_x_CIRC,                  "",                                            STD_x_CIRC,                                    0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1177 */  { addItemToBuffer,             CHR_y_BAR,                   "",                                            STD_y_BAR,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1178 */  { addItemToBuffer,             CHR_y_CIRC,                  STD_y_CIRC,                                    STD_y_CIRC,                                    0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1179 */  { addItemToBuffer,             CHR_y_ACUTE,                 STD_y_ACUTE,                                   STD_y_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1180 */  { addItemToBuffer,             CHR_y_DIARESIS,              STD_y_DIARESIS,                                STD_y_DIARESIS,                                0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1181 */  { addItemToBuffer,             CHR_z_ACUTE,                 STD_z_ACUTE,                                   STD_z_ACUTE,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1182 */  { addItemToBuffer,             CHR_z_CARON,                 STD_z_CARON,                                   STD_z_CARON,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1183 */  { addItemToBuffer,             CHR_z_DOT,                   STD_z_DOT,                                     STD_z_DOT,                                     0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1184 */  { itemToBeCoded,               NOPARAM,                     "1184",                                        "1184",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1185 */  { itemToBeCoded,               NOPARAM,                     "1185",                                        "1185",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1186 */  { itemToBeCoded,               NOPARAM,                     "1186",                                        "1186",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1187 */  { itemToBeCoded,               NOPARAM,                     "1187",                                        "1187",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1188 */  { itemToBeCoded,               NOPARAM,                     "1188",                                        "1188",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1189 */  { itemToBeCoded,               NOPARAM,                     "1189",                                        "1189",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1190 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_alpha,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1191 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_delta,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1192 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_mu,                                    0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1193 */  { addItemToBuffer,             CHR_SUB_SUN,                 "",                                            STD_SUB_SUN,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1194 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_SUN_b,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1195 */  { addItemToBuffer,             CHR_SUB_EARTH,               "",                                            STD_SUB_EARTH,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1196 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_EARTH_b,                               0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1197 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_PLUS,                                  0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1198 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_MINUS,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1199 */  { addItemToBuffer,             CHR_SUB_INFINITY,            "",                                            STD_SUB_INFINITY,                              0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1200 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_0,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1201 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_1,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1202 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_2,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1203 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_3,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1204 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_4,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1205 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_5,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1206 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_6,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1207 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_7,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1208 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_8,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1209 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_9,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1210 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_10,                                    0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1211 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_A,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1212 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_B,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1213 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_C,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1214 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_D,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1215 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_E,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1216 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_F,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1217 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_G,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1218 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_H,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1219 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_I,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1220 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_J,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1221 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_K,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1222 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_L,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1223 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_M,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1224 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_N,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1225 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_O,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1226 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_P,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1227 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_Q,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1228 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_R,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1229 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_S,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1230 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_T,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1231 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_U,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1232 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_V,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1233 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_W,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1234 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_X,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1235 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_Y,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1236 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_Z,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1237 */  { addItemToBuffer,             CHR_SUB_E_OUTLINE,           "",                                            STD_SUB_E_OUTLINE,                             0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1238 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_a,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1239 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_b,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1240 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_c,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1241 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_d,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1242 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_e,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1243 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_h,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1244 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_i,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1245 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_j,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1246 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_k,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1247 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_l,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1248 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_m,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1249 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_n,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1250 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_o,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1251 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_p,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1252 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_q,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1253 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_s,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1254 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_t,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1255 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_u,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1256 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_v,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1257 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_w,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1258 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_x,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1259 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_y,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1260 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_z,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1261 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_a_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1262 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_e_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1263 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_k_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1264 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_l_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1265 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_m_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1266 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_n_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1267 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_o_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1268 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_p_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1269 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_s_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1270 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_u_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1271 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUB_x_b,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1272 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_PLUS,                                  0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1273 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_MINUS,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1274 */  { addItemToBuffer,             CHR_SUP_MINUS_1,             "",                                            STD_SUP_MINUS_1,                               0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1275 */  { addItemToBuffer,             CHR_SUP_INFINITY,            "",                                            STD_SUP_INFINITY,                              0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1276 */  { addItemToBuffer,             CHR_SUP_ASTERISK,            "",                                            STD_SUP_ASTERISK,                              0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1277 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_0,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1278 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_1,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1279 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_2,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1280 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_3,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1281 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_4,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1282 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_5,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1283 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_6,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1284 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_7,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1285 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_8,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1286 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_9,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1287 */  { itemToBeCoded,               NOPARAM,                     "1287",                                        "1287",                                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1288 */  { addItemToBuffer,             CHR_SUP_T,                   "",                                            STD_SUP_T,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1289 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_a,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1290 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_f,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1291 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_g,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1292 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_h,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1293 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_r,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1294 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SUP_x,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1295 */  { addItemToBuffer,             CHR_SPACE,                   "",                                            STD_SPACE,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1296 */  { addItemToBuffer,             CHR_EXCLAMATION_MARK,        "",                                            STD_EXCLAMATION_MARK,                          0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1297 */  { addItemToBuffer,             CHR_DOUBLE_QUOTE,            "",                                            STD_DOUBLE_QUOTE,                              0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1298 */  { addItemToBuffer,             CHR_NUMBER_SIGN,             "",                                            STD_NUMBER_SIGN,                               0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1299 */  { addItemToBuffer,             CHR_DOLLAR,                  "",                                            STD_DOLLAR,                                    0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1300 */  { addItemToBuffer,             CHR_PERCENT,                 "",                                            STD_PERCENT,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1301 */  { addItemToBuffer,             CHR_AMPERSAND,               "",                                            STD_AMPERSAND,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1302 */  { addItemToBuffer,             CHR_QUOTE,                   "",                                            STD_QUOTE,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1303 */  { addItemToBuffer,             CHR_LEFT_PARENTHESIS,        "",                                            STD_LEFT_PARENTHESIS,                          0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1304 */  { addItemToBuffer,             CHR_RIGHT_PARENTHESIS,       "",                                            STD_RIGHT_PARENTHESIS,                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1305 */  { addItemToBuffer,             CHR_ASTERISK,                "",                                            STD_ASTERISK,                                  0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1306 */  { itemToBeCoded,               NOPARAM,                     "1306",                                        "1306",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1307 */  { addItemToBuffer,             CHR_PLUS,                    "",                                            STD_PLUS,                                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1308 */  { addItemToBuffer,             CHR_COMMA,                   "",                                            STD_COMMA,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1309 */  { addItemToBuffer,             CHR_MINUS,                   "",                                            STD_MINUS,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1310 */  { addItemToBuffer,             CHR_PERIOD,                  "",                                            STD_PERIOD,                                    0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1311 */  { addItemToBuffer,             CHR_SLASH,                   "",                                            STD_SLASH,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1312 */  { addItemToBuffer,             CHR_COLON,                   "",                                            STD_COLON,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1313 */  { addItemToBuffer,             CHR_SEMICOLON,               "",                                            STD_SEMICOLON,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1314 */  { addItemToBuffer,             CHR_LESS_THAN,               "",                                            STD_LESS_THAN,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1315 */  { addItemToBuffer,             CHR_EQUAL,                   "",                                            STD_EQUAL,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1316 */  { addItemToBuffer,             CHR_GREATER_THAN,            "",                                            STD_GREATER_THAN,                              0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1317 */  { addItemToBuffer,             CHR_QUESTION_MARK,           "",                                            STD_QUESTION_MARK,                             0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1318 */  { addItemToBuffer,             CHR_AT,                      "",                                            STD_AT,                                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1319 */  { addItemToBuffer,             CHR_LEFT_SQUARE_BRACKET,     "",                                            STD_LEFT_SQUARE_BRACKET,                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1320 */  { addItemToBuffer,             CHR_BACK_SLASH,              "",                                            STD_BACK_SLASH,                                0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1321 */  { addItemToBuffer,             CHR_RIGHT_SQUARE_BRACKET,    "",                                            STD_RIGHT_SQUARE_BRACKET,                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1322 */  { addItemToBuffer,             CHR_CIRCUMFLEX,              "",                                            STD_CIRCUMFLEX,                                0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1323 */  { addItemToBuffer,             CHR_UNDERSCORE,              "",                                            STD_UNDERSCORE,                                0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1324 */  { addItemToBuffer,             CHR_LEFT_CURLY_BRACKET,      "",                                            STD_LEFT_CURLY_BRACKET,                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1325 */  { addItemToBuffer,             CHR_PIPE,                    "",                                            STD_PIPE,                                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1326 */  { addItemToBuffer,             CHR_RIGHT_CURLY_BRACKET,     "",                                            STD_RIGHT_CURLY_BRACKET,                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1327 */  { addItemToBuffer,             CHR_TILDE,                   "",                                            STD_TILDE,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1328 */  { addItemToBuffer,             CHR_INVERTED_EXCLAMATION_MARK, "",                                          STD_INVERTED_EXCLAMATION_MARK,                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1329 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_CENT,                                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1330 */  { addItemToBuffer,             CHR_POUND,                   "",                                            STD_POUND,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1331 */  { addItemToBuffer,             CHR_YEN,                     "",                                            STD_YEN,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1332 */  { addItemToBuffer,             CHR_SECTION,                 "",                                            STD_SECTION,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1333 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OVERFLOW_CARRY,                            0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1334 */  { addItemToBuffer,             CHR_LEFT_DOUBLE_ANGLE,       "",                                            STD_LEFT_DOUBLE_ANGLE,                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1335 */  { addItemToBuffer,             CHR_NOT,                     "",                                            STD_NOT,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1336 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DEGREE,                                    0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1337 */  { addItemToBuffer,             CHR_PLUS_MINUS,              "",                                            STD_PLUS_MINUS,                                0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1338 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_mu_b,                                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1339 */  { addItemToBuffer,             CHR_DOT,                     "",                                            STD_DOT,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1340 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ORDINAL,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1341 */  { addItemToBuffer,             CHR_RIGHT_DOUBLE_ANGLE,      "",                                            STD_RIGHT_DOUBLE_ANGLE,                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1342 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ONE_HALF,                                  0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1343 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ONE_QUARTER,                               0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1344 */  { addItemToBuffer,             CHR_INVERTED_QUESTION_MARK,  "",                                            STD_INVERTED_QUESTION_MARK,                    0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1345 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ETH,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1346 */  { addItemToBuffer,             CHR_CROSS,                   "",                                            STD_CROSS,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1347 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_eth,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1348 */  { addItemToBuffer,             CHR_DIVIDE,                  "",                                            STD_DIVIDE,                                    0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1349 */  { addItemToBuffer,             CHR_E_DOT,                   STD_E_DOT,                                     STD_E_DOT,                                     0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1350 */  { addItemToBuffer,             CHR_e_DOT,                   STD_e_DOT,                                     STD_e_DOT,                                     0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1351 */  { addItemToBuffer,             NOPARAM,                     STD_E_CARON,                                   STD_E_CARON,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1352 */  { addItemToBuffer,             NOPARAM,                     STD_e_CARON,                                   STD_e_CARON,                                   0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1353 */  { addItemToBuffer,             CHR_R_ACUTE,                 STD_R_ACUTE,                                   STD_R_ACUTE,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1354 */  { addItemToBuffer,             CHR_R_CARON,                 STD_R_CARON,                                   STD_R_CARON,                                   0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1355 */  { addItemToBuffer,             CHR_U_OGONEK,                STD_U_OGONEK,                                  STD_U_OGONEK,                                  0,       0,       CAT_AINT, SLS_UNCHANGED},
/* 1356 */  { addItemToBuffer,             CHR_u_OGONEK,                STD_u_OGONEK,                                  STD_u_OGONEK,                                  0,       0,       CAT_aint, SLS_UNCHANGED},
/* 1357 */  { addItemToBuffer,             CHR_y_UNDER_ROOT,            "",                                            STD_y_UNDER_ROOT,                              0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1358 */  { addItemToBuffer,             CHR_x_UNDER_ROOT,            "",                                            STD_x_UNDER_ROOT,                              0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1359 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_EM,                                  0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1360 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_3_PER_EM,                            0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1361 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_4_PER_EM,                            0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1362 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_6_PER_EM,                            0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1363 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_FIGURE,                              0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1364 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_PUNCTUATION,                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1365 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SPACE_HAIR,                                0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1366 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LEFT_SINGLE_QUOTE,                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1367 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RIGHT_SINGLE_QUOTE,                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1368 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SINGLE_LOW_QUOTE,                          0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1369 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SINGLE_HIGH_QUOTE,                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1370 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LEFT_DOUBLE_QUOTE,                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1371 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RIGHT_DOUBLE_QUOTE,                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1372 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOUBLE_LOW_QUOTE,                          0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1373 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOUBLE_HIGH_QUOTE,                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1374 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ELLIPSIS,                                  0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1375 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ONE,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1376 */  { addItemToBuffer,             CHR_EURO,                    "",                                            STD_EURO,                                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1377 */  { addItemToBuffer,             CHR_COMPLEX_C,               "",                                            STD_COMPLEX_C,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1378 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PLANCK,                                    0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1379 */  { addItemToBuffer,             CHR_PLANCK_2PI,              "",                                            STD_PLANCK_2PI,                                0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1380 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NATURAL_N,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1381 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RATIONAL_Q,                                0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1382 */  { addItemToBuffer,             CHR_REAL_R,                  "",                                            STD_REAL_R,                                    0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1383 */  { addItemToBuffer,             CHR_LEFT_ARROW,              "",                                            STD_LEFT_ARROW,                                0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1384 */  { addItemToBuffer,             CHR_UP_ARROW,                "",                                            STD_UP_ARROW,                                  0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1385 */  { itemToBeCoded,               NOPARAM,                     "1385",                                        "1385",                                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1386 */  { addItemToBuffer,             CHR_RIGHT_ARROW,             "",                                            STD_RIGHT_ARROW,                               0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1387 */  { addItemToBuffer,             CHR_DOWN_ARROW,              "",                                            STD_DOWN_ARROW,                                0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1388 */  { itemToBeCoded,               NOPARAM,                     "1388",                                        "1388",                                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1389 */  { addItemToBuffer,             CHR_SERIAL_IO,               "",                                            STD_SERIAL_IO,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1390 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RIGHT_SHORT_ARROW,                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1391 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_LEFT_RIGHT_ARROWS,                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1392 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_BST,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1393 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SST,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1394 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_HAMBURGER,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1395 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_UNDO,                                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1396 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_FOR_ALL,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1397 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_COMPLEMENT,                                0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1398 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PARTIAL_DIFF,                              0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1399 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_THERE_EXISTS,                              0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1400 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_THERE_DOES_NOT_EXIST,                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1401 */  { addItemToBuffer,             CHR_EMPTY_SET,               "",                                            STD_EMPTY_SET,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1402 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_INCREMENT,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1403 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NABLA,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1404 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ELEMENT_OF,                                0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1405 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NOT_ELEMENT_OF,                            0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1406 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_CONTAINS,                                  0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1407 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOES_NOT_CONTAIN,                          0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1408 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ZERO,                                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1409 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PRODUCT,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1410 */  { itemToBeCoded,               NOPARAM,                     "1410",                                        "1410",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1411 */  { itemToBeCoded,               NOPARAM,                     "1411",                                        "1411",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1412 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MINUS_PLUS,                                0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1413 */  { itemToBeCoded,               NOPARAM,                     "1413",                                        "1413",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1414 */  { itemToBeCoded,               NOPARAM,                     "1414",                                        "1414",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1415 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RING,                                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1416 */  { addItemToBuffer,             CHR_BULLET,                  "",                                            STD_BULLET,                                    0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1417 */  { addItemToBuffer,             CHR_SQUARE_ROOT,             "",                                            STD_SQUARE_ROOT,                               0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1418 */  { addItemToBuffer,             CHR_CUBE_ROOT,               "",                                            STD_CUBE_ROOT,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1419 */  { addItemToBuffer,             CHR_xTH_ROOT,                "",                                            STD_xTH_ROOT,                                  0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1420 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PROPORTIONAL,                              0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1421 */  { addItemToBuffer,             CHR_INFINITY,                "",                                            STD_INFINITY,                                  0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1422 */  { addItemToBuffer,             CHR_RIGHT_ANGLE,             "",                                            STD_RIGHT_ANGLE,                               0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1423 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ANGLE,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1424 */  { addItemToBuffer,             CHR_MEASURED_ANGLE,          "",                                            STD_MEASURED_ANGLE,                            0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1425 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DIVIDES,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1426 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOES_NOT_DIVIDE,                           0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1427 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PARALLEL,                                  0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1428 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NOT_PARALLEL,                              0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1429 */  { addItemToBuffer,             CHR_AND,                     "",                                            STD_AND,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1430 */  { addItemToBuffer,             CHR_OR,                      "",                                            STD_OR,                                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1431 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_INTERSECTION,                              0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1432 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_UNION,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1433 */  { addItemToBuffer,             CHR_INTEGRAL,                "",                                            STD_INTEGRAL,                                  0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1434 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOUBLE_INTEGRAL,                           0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1435 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_CONTOUR_INTEGRAL,                          0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1436 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_SURFACE_INTEGRAL,                          0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1437 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_RATIO,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1438 */  { addItemToBuffer,             CHR_CHECK_MARK,              "",                                            STD_CHECK_MARK,                                0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1439 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_ASYMPOTICALLY_EQUAL,                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1440 */  { addItemToBuffer,             CHR_ALMOST_EQUAL,            "",                                            STD_ALMOST_EQUAL,                              0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1441 */  { addItemToBuffer,             CHR_COLON_EQUALS,            "",                                            STD_COLON_EQUALS,                              0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1442 */  { addItemToBuffer,             CHR_CORRESPONDS_TO,          "",                                            STD_CORRESPONDS_TO,                            0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1443 */  { addItemToBuffer,             CHR_ESTIMATES,               "",                                            STD_ESTIMATES,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1444 */  { addItemToBuffer,             CHR_NOT_EQUAL,               "",                                            STD_NOT_EQUAL,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1445 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_IDENTICAL_TO,                              0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1446 */  { addItemToBuffer,             CHR_LESS_EQUAL,              "",                                            STD_LESS_EQUAL,                                0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1447 */  { addItemToBuffer,             CHR_GREATER_EQUAL,           "",                                            STD_GREATER_EQUAL,                             0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1448 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MUCH_LESS,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1449 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MUCH_GREATER,                              0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1450 */  { addItemToBuffer,             CHR_SUN,                     "",                                            STD_SUN,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1451 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_DOWN_TACK,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1452 */  { addItemToBuffer,             CHR_PERPENDICULAR,           "",                                            STD_PERPENDICULAR,                             0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1453 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_XOR,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1454 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NAND,                                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1455 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_NOR,                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1456 */  { addItemToBuffer,             CHR_WATCH,                   "",                                            STD_WATCH,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1457 */  { addItemToBuffer,             CHR_HOURGLASS,               "",                                            STD_HOURGLASS,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1458 */  { addItemToBuffer,             CHR_PRINTER,                 "",                                            STD_PRINTER,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1459 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_TL,                                    0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1460 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_ML,                                    0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1461 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_BL,                                    0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1462 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_TR,                                    0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1463 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_MR,                                    0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1464 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_MAT_BR,                                    0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1465 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OBLIQUE1,                                  0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1466 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OBLIQUE2,                                  0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1467 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OBLIQUE3,                                  0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1468 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_OBLIQUE4,                                  0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1469 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_CURSOR,                                    0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1470 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PERIOD34,                                  0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1471 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_COMMA34,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1472 */  { addItemToBuffer,             CHR_BATTERY,                 "",                                            STD_BATTERY,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1473 */  { addItemToBuffer,             CHR_PGM_BEGIN,               "",                                            STD_PGM_BEGIN,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1474 */  { addItemToBuffer,             CHR_USER_MODE,               "",                                            STD_USER_MODE,                                 0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1475 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_UK,                                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1476 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_US,                                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1477 */  { addItemToBuffer,             CHR_NEG_EXCLAMATION_MARK,    "",                                            STD_NEG_EXCLAMATION_MARK,                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1478 */  { addItemToBuffer,             CHR_ex,                      "",                                            STD_LEFT_RIGHT_ARROWS,                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1479 */  { addItemToBuffer,             ITM_Max,                     "",                                            "Max",                                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1480 */  { addItemToBuffer,             ITM_Min,                     "",                                            "Min",                                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1481 */  { addItemToBuffer,             ITM_Config,                  "",                                            "Config",                                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1482 */  { addItemToBuffer,             ITM_Stack,                   "",                                            "Stack",                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1483 */  { addItemToBuffer,             ITM_dddEL,                   "",                                            STD_ELLIPSIS "EL",                             0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1484 */  { addItemToBuffer,             ITM_dddIJ,                   "",                                            STD_ELLIPSIS "IJ",                             0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1485 */  { itemToBeCoded,               NOPARAM,                     "",                                            "0.",                                          0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1486 */  { itemToBeCoded,               NOPARAM,                     "",                                            "1.",                                          0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1487 */  { addItemToBuffer,             ITM_EXPONENT,                "",                                            "E",                                           0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1488 */  { addItemToBuffer,             ITM_MA11,                    "",                                            STD_a_DIARESIS,                                0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1489 */  { addItemToBuffer,             ITM_MA12,                    "",                                            STD_o_DIARESIS,                                0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1490 */  { addItemToBuffer,             ITM_MA13,                    "",                                            STD_u_DIARESIS,                                0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1491 */  { addItemToBuffer,             ITM_MA14,                    "",                                            "",                                            0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1492 */  { addItemToBuffer,             ITM_MA15,                    "",                                            "sch",                                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1493 */  { addItemToBuffer,             ITM_MA16,                    "",                                            STD_s_SHARP,                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1494 */  { addItemToBuffer,             ITM_MA21,                    "",                                            "not",                                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1495 */  { addItemToBuffer,             ITM_MA22,                    "",                                            "edi-",                                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1496 */  { addItemToBuffer,             ITM_MA23,                    "",                                            "table",                                       0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1497 */  { addItemToBuffer,             ITM_MA24,                    "",                                            "for",                                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1498 */  { addItemToBuffer,             ITM_MA25,                    "",                                            "now",                                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1499 */  { addItemToBuffer,             ITM_MA26,                    "",                                            "",                                            0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1500 */  { addItemToBuffer,             ITM_MA31,                    "",                                            "",                                            0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1501 */  { addItemToBuffer,             ITM_MA32,                    "",                                            "",                                            0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1502 */  { addItemToBuffer,             ITM_MA33,                    "",                                            "",                                            0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1503 */  { addItemToBuffer,             ITM_MA34,                    "",                                            "",                                            0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1504 */  { addItemToBuffer,             ITM_MA35,                    "",                                            "",                                            0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1505 */  { addItemToBuffer,             ITM_MA36,                    "",                                            "",                                            0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1506 */  { itemToBeCoded,               NOPARAM,                     "UNDO",                                        STD_UNDO,                                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1507 */  { itemToBeCoded,               NOPARAM,                     "P/R",                                         "P/R",                                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1508 */  { itemToBeCoded,               NOPARAM,                     "R/S",                                         "R/S",                                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1509 */  { itemToBeCoded,               NOPARAM,                     "",                                            "Not",                                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1510 */  { itemToBeCoded,               NOPARAM,                     "",                                            "yet",                                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1511 */  { itemToBeCoded,               NOPARAM,                     "",                                            "defined",                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1512 */  { itemToBeCoded,               NOPARAM,                     "",                                            "Tam",                                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1513 */  { itemToBeCoded,               NOPARAM,                     "",                                            "TamCmp",                                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1514 */  { itemToBeCoded,               NOPARAM,                     "",                                            "TamStoRcl",                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1515 */  { fnFlipFlag,                  FLAG_USER,                   "USER",                                        "USER",                                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1516 */  { fnKeyCC,                     NOPARAM,                     "CC",                                          "CC",                                          0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1517 */  { itemToBeCoded,               NOPARAM,                     "",                                            "f",                                           0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1518 */  { itemToBeCoded,               NOPARAM,                     "",                                            "g",                                           0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1519 */  { fnKeyUp,                     NOPARAM,                     "UP",                                          STD_UP_ARROW,                                  0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1520 */  { itemToBeCoded,               NOPARAM,                     "BST",                                         STD_HAMBURGER STD_BST,                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1521 */  { fnKeyDown,                   NOPARAM,                     "DOWN",                                        STD_DOWN_ARROW,                                0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1522 */  { itemToBeCoded,               NOPARAM,                     "SST",                                         STD_HAMBURGER STD_SST,                         0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1523 */  { fnKeyExit,                   NOPARAM,                     "EXIT",                                        "EXIT",                                        0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1524 */  { fnKeyBackspace,              NOPARAM,                     "BKSPC",                                       STD_LEFT_ARROW,                                0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1525 */  { itemToBeCoded,               NOPARAM,                     "",                                            STD_PRINTER "x",                               0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1526 */  { fnAim,                       NOPARAM,                     "AIM",                                         STD_alpha,                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1527 */  { fnKeyDotD,                   NOPARAM,                     ".d",                                          ".d",                                          0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1528 */  { fnCvtQuartM3,                multiply,                    "qt." STD_RIGHT_ARROW "m" STD_SUP_3,           "qt." STD_RIGHT_ARROW "m" STD_SUP_3,           0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1529 */  { fnCvtQuartM3,                divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "qt.",           "m" STD_SUP_3 STD_RIGHT_ARROW "qt.",           0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1530 */  { itemToBeCoded,               NOPARAM,                     STD_RIGHT_ARROW "SP",                          STD_RIGHT_ARROW "SP",                          0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1531 */  { fnShow,                      NOPARAM,                     "SHOW",                                        "SHOW",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/* 1532 */  { backToSystem,                NOPARAM,                     "SYSTEM",                                      "SYSTEM",                                      0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1533 */  { fnCvtDmsToDeg,               NOPARAM,                     "D.MS" STD_RIGHT_ARROW "D",                    "D.MS" STD_RIGHT_ARROW "D",                    0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1534 */  { itemToBeCoded,               NOPARAM,                     "V" STD_MEASURED_ANGLE,                        STD_MEASURED_ANGLE,                            0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1535 */  { fnCvtFathomM,                multiply,                    "fm." STD_RIGHT_ARROW "m",                     "fathom",                                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1536 */  { fnCvtFathomM,                multiply,                    "fm." STD_RIGHT_ARROW "m",                     STD_RIGHT_ARROW " m",                          0,       0,       CAT_DUPL, SLS_ENABLED  },
/* 1537 */  { fnCvtFathomM,                divide,                      "m" STD_RIGHT_ARROW "fm.",                     "m " STD_RIGHT_ARROW,                          0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1538 */  { fnCvtFathomM,                divide,                      "m" STD_RIGHT_ARROW "fm.",                     "fathom",                                      0,       0,       CAT_DUPL, SLS_ENABLED  },
/* 1539 */  { fnCvtSfeetM,                 multiply,                    "ft" STD_US STD_RIGHT_ARROW "m",               STD_RIGHT_ARROW " m",                          0,       0,       CAT_DUPL, SLS_ENABLED  },
/* 1540 */  { fnCvtSfeetM,                 divide,                      "m" STD_RIGHT_ARROW "ft" STD_US,               "foot" STD_US,                                 0,       0,       CAT_DUPL, SLS_ENABLED  },
/* 1541 */  { fnCvtPointM,                 divide,                      "m" STD_RIGHT_ARROW "pt.",                     "m " STD_RIGHT_ARROW,                          0,       0,       CAT_DUPL, SLS_ENABLED  },
/* 1542 */  { fnCvtPointM,                 multiply,                    "pt." STD_RIGHT_ARROW "m",                     STD_RIGHT_ARROW " m",                          0,       0,       CAT_DUPL, SLS_ENABLED  },
/* 1543 */  { fnCvtBarrelM3,               multiply,                    "bbl" STD_RIGHT_ARROW "m" STD_SUP_3,           "barrel",                                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1544 */  { fnCvtBarrelM3,               multiply,                    "bbl" STD_RIGHT_ARROW "m" STD_SUP_3,           STD_RIGHT_ARROW " m" STD_SUP_3,                0,       0,       CAT_DUPL, SLS_ENABLED  },
/* 1545 */  { fnCvtBarrelM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "bbl",           STD_RIGHT_ARROW " m" STD_SUP_3,                0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1546 */  { fnCvtBarrelM3,               divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "bbl",           "barrel",                                      0,       0,       CAT_DUPL, SLS_ENABLED  },
/* 1547 */  { fnCvtTonKg,                  multiply,                    "ton" STD_RIGHT_ARROW "kg",                    STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  },
/* 1548 */  { fnCvtTonKg,                  multiply,                    "ton" STD_RIGHT_ARROW "kg",                    STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  },
/* 1549 */  { fnCvtTonKg,                  divide,                      "kg" STD_RIGHT_ARROW "ton",                    STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  },
/* 1550 */  { fnCvtTonKg,                  divide,                      "kg" STD_RIGHT_ARROW "ton",                    STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  },
/* 1551 */  { fnCvtCaratKg,                multiply,                    "ct" STD_RIGHT_ARROW "kg",                     STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  },
/* 1552 */  { fnCvtCaratKg,                divide,                      "kg" STD_RIGHT_ARROW "ct",                     STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  },
/* 1553 */  { fnCvtAtmPa,                  multiply,                    "atm" STD_RIGHT_ARROW "Pa",                    STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  },
/* 1554 */  { fnCvtAtmPa,                  divide,                      "Pa" STD_RIGHT_ARROW "atm",                    STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  },
/* 1555 */  { fnCvtFathomM,                multiply,                    "fm." STD_RIGHT_ARROW "m",                     STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  },
/* 1556 */  { fnCvtFathomM,                divide,                      "m" STD_RIGHT_ARROW "fm.",                     STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  },
/* 1557 */  { fnCvtPointM,                 multiply,                    "pt." STD_RIGHT_ARROW "m",                     STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  },
/* 1558 */  { fnCvtPointM,                 divide,                      "m" STD_RIGHT_ARROW "pt.",                     STD_SPACE_HAIR,                                0,       0,       CAT_DUPL, SLS_ENABLED  },
/* 1559 */  { fnHarmonicMeanXY,            NOPARAM,                     STD_x_BAR STD_SUB_H,                           STD_x_BAR STD_SUB_H,                           0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1560 */  { fnRMSMeanXY,                 NOPARAM,                     STD_x_BAR STD_SUB_R STD_SUB_M STD_SUB_S,       STD_x_BAR STD_SUB_R STD_SUB_M STD_SUB_S,       0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1561 */  { itemToBeCoded,               NOPARAM,                     STD_SIGMA,                                     STD_SIGMA,                                     0,       0,       CAT_MENU, SLS_UNCHANGED},
/* 1562 */  { itemToBeCoded,               NOPARAM,                     "GaussF",                                      "GaussF",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/* 1563 */  { itemToBeCoded,               NOPARAM,                     "CauchF",                                      "CauchF",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/* 1564 */  { itemToBeCoded,               NOPARAM,                     "ParabF",                                      "ParabF",                                      0,       0,       CAT_FNCT, SLS_UNCHANGED},
/* 1565 */  { itemToBeCoded,               NOPARAM,                     "HypF",                                        "HypF",                                        0,       0,       CAT_FNCT, SLS_UNCHANGED},
/* 1566 */  { itemToBeCoded,               NOPARAM,                     "RootF",                                       "RootF",                                       0,       0,       CAT_FNCT, SLS_UNCHANGED},
/* 1567 */  { fnStatSum,                   14,                          STD_SIGMA "lny/x",                             STD_SIGMA "lny/x",                             0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1568 */  { fnStatSum,                   15,                          STD_SIGMA "x" STD_SUP_2 "/y",                  STD_SIGMA "x" STD_SUP_2 "/y",                  0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1569 */  { fnStatSum,                   16,                          STD_SIGMA STD_SUP_1 "/x",                      STD_SIGMA STD_SUP_1 "/x",                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1570 */  { fnStatSum,                   17,                          STD_SIGMA STD_SUP_1 "/x" STD_SUP_2,            STD_SIGMA STD_SUP_1 "/x" STD_SUP_2,            0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1571 */  { fnStatSum,                   18,                          STD_SIGMA "x/y",                               STD_SIGMA "x/y",                               0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1572 */  { fnStatSum,                   19,                          STD_SIGMA STD_SUP_1 "/y",                      STD_SIGMA STD_SUP_1 "/y",                      0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1573 */  { fnStatSum,                   20,                          STD_SIGMA STD_SUP_1 "/y" STD_SUP_2,            STD_SIGMA STD_SUP_1 "/y" STD_SUP_2,            0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1574 */  { fnStatSum,                   21,                          STD_SIGMA "x" STD_SUP_3,                       STD_SIGMA "x" STD_SUP_3,                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1575 */  { fnStatSum,                   22,                          STD_SIGMA "x" STD_SUP_4,                       STD_SIGMA "x" STD_SUP_4,                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1576 */  { addItemToBuffer,             NOPARAM,                     "HEX",                                         "H",                                           0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1577 */  { fnIDivR,                     NOPARAM,                     "IDIVR",                                       "IDIVR",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1578 */  { fnArccos,                    NOPARAM,                     "ACOS",                                        "ACOS",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1579 */  { fnArcsin,                    NOPARAM,                     "ASIN",                                        "ASIN",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1580 */  { fnArctan,                    NOPARAM,                     "ATAN",                                        "ATAN",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1581 */  { itemToBeCoded,               NOPARAM,                     "DET",                                         "DET",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1582 */  { itemToBeCoded,               NOPARAM,                     "INVRT",                                       "INVRT",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1583 */  { itemToBeCoded,               NOPARAM,                     "TRANS",                                       "TRANS",                                       0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1584 */  { itemToBeCoded,               NOPARAM,                     "xIN",                                         "xIN",                                         0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1585 */  { itemToBeCoded,               NOPARAM,                     "xOUT",                                        "xOUT",                                        0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1586 */  { fnAlphaSR,                   TM_REGISTER,                 STD_alpha "SR",                                STD_alpha "SR",                                0,      99,       CAT_FNCT, SLS_ENABLED  },
/* 1587 */  { fnCvtHectareM2,              multiply,                    "ha" STD_RIGHT_ARROW "m" STD_SUP_2,            "ha" STD_RIGHT_ARROW "m" STD_SUP_2,            0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1588 */  { fnCvtHectareM2,              divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ha",            "m" STD_SUP_2 STD_RIGHT_ARROW "ha",            0,       0,       CAT_FNCT, SLS_ENABLED  },
/* 1589 */  { itemToBeCoded,               NOPARAM,                     "VAR",                                         "VAR",                                         0,       0,       CAT_MENU, SLS_UNCHANGED},
/* 1590 */  { itemToBeCoded,               NOPARAM,                     "",                                            "TamFlag",                                     0,       0,       CAT_NONE, SLS_UNCHANGED},
/* 1591 */  { itemToBeCoded,               NOPARAM,                     "1591",                                        "1591",                                        0,       0,       CAT_FREE, SLS_UNCHANGED},
/* 1592 */  { fnGetSystemFlag,             FLAG_TDM24,                  "TDM24",                                       "TDM24",                                       0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1593 */  { fnGetSystemFlag,             FLAG_YMD,                    "YMD",                                         "YMD",                                         0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1594 */  { fnGetSystemFlag,             FLAG_DMY,                    "DMY",                                         "DMY",                                         0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1595 */  { fnGetSystemFlag,             FLAG_MDY,                    "MDY",                                         "MDY",                                         0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1596 */  { fnGetSystemFlag,             FLAG_CPXRES,                 "CPXRES",                                      "CPXRES",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1597 */  { fnGetSystemFlag,             FLAG_CPXj,                   "CPXj",                                        "CPXj",                                        0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1598 */  { fnGetSystemFlag,             FLAG_POLAR,                  "POLAR",                                       "POLAR",                                       0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1599 */  { fnGetSystemFlag,             FLAG_FRACT,                  "FRACT",                                       "FRACT",                                       0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1600 */  { fnGetSystemFlag,             FLAG_PROPFR,                 "PROPFR",                                      "PROPFR",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1601 */  { fnGetSystemFlag,             FLAG_DENANY,                 "DENANY",                                      "DENANY",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1602 */  { fnGetSystemFlag,             FLAG_DENFIX,                 "DENFIX",                                      "DENFIX",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1603 */  { fnGetSystemFlag,             FLAG_CARRY,                  "CARRY",                                       "CARRY",                                       0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1604 */  { fnGetSystemFlag,             FLAG_OVERFLOW,               "OVERFL",                                      "OVERFL",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1605 */  { fnGetSystemFlag,             FLAG_LEAD0,                  "LEAD.0",                                      "LEAD.0",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1606 */  { fnGetSystemFlag,             FLAG_ALPHA,                  "ALPHA",                                       "ALPHA",                                       0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1607 */  { fnGetSystemFlag,             FLAG_alphaCAP,               STD_alpha "CAP",                               STD_alpha "CAP",                               0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1608 */  { fnGetSystemFlag,             FLAG_RUNTIM,                 "RUNTIM",                                      "RUNTIM",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1609 */  { fnGetSystemFlag,             FLAG_RUNIO,                  "RUNIO",                                       "RUNIO",                                       0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1610 */  { fnGetSystemFlag,             FLAG_PRINT,                  "PRINT",                                       "PRINT",                                       0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1611 */  { fnGetSystemFlag,             FLAG_TRACE,                  "TRACE",                                       "TRACE",                                       0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1612 */  { fnGetSystemFlag,             FLAG_USER,                   "USER",                                        "USER",                                        0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1613 */  { fnGetSystemFlag,             FLAG_LOWBAT,                 "LOWBAT",                                      "LOWBAT",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1614 */  { fnGetSystemFlag,             FLAG_SLOW,                   "SLOW",                                        "SLOW",                                        0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1615 */  { fnGetSystemFlag,             FLAG_SPCRES,                 "SPCRES",                                      "SPCRES",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1616 */  { fnGetSystemFlag,             FLAG_SSIZE8,                 "SSIZE8",                                      "SSIZE8",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1617 */  { fnGetSystemFlag,             FLAG_QUIET,                  "QUIET",                                       "QUIET",                                       0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1618 */  { fnGetSystemFlag,             FLAG_DECIMP,                 "DECIM.",                                      "DECIM.",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1619 */  { fnGetSystemFlag,             FLAG_MULTx,                  "MULT" STD_CROSS,                              "MULT" STD_CROSS,                              0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1620 */  { fnGetSystemFlag,             FLAG_ALLENG,                 "ALLENG",                                      "ALLENG",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1621 */  { fnGetSystemFlag,             FLAG_GROW,                   "GROW",                                        "GROW",                                        0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1622 */  { fnGetSystemFlag,             FLAG_AUTOFF,                 "AUTOFF",                                      "AUTOFF",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1623 */  { fnGetSystemFlag,             FLAG_AUTXEQ,                 "AUTXEQ",                                      "AUTXEQ",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1624 */  { fnGetSystemFlag,             FLAG_PRTACT,                 "PRTACT",                                      "PRTACT",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1625 */  { fnGetSystemFlag,             FLAG_NUMIN,                  "NUM.IN",                                      "NUM.IN",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1626 */  { fnGetSystemFlag,             FLAG_ALPIN,                  "ALP.IN",                                      "ALP.IN",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1627 */  { fnGetSystemFlag,             FLAG_ASLIFT,                 "ASLIFT",                                      "ASLIFT",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1628 */  { fnGetSystemFlag,             FLAG_IGN1ER,                 "IGN1ER",                                      "IGN1ER",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1629 */  { fnGetSystemFlag,             FLAG_INTING,                 "INTING",                                      "INTING",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1630 */  { fnGetSystemFlag,             FLAG_SOLVING,                "SOLVING",                                     "SOLVING",                                     0,       0,       CAT_SYFL, SLS_UNCHANGED},
/* 1631 */  { fnGetSystemFlag,             FLAG_VMDISP,                 "VMDISP",                                      "VMDISP",                                      0,       0,       CAT_SYFL, SLS_UNCHANGED},

/* 1632 */  { itemToBeCoded,               NOPARAM,                     "",                                            "Last item",                                   0,       0,       CAT_NONE, SLS_UNCHANGED},
};
