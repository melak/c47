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

#include "items.h"

#include "browsers/browsers.h"
#include "bufferize.h"
#include "config.h"
#include "constants.h"
#include "conversionAngles.h"
#include "conversionUnits.h"
#include "curveFitting.h"
#include "dateTime.h"
#include "display.h"
#include "distributions/distributions.h"
#include "error.h"
#include "flags.h"
#include "fonts.h"
#include "fractions.h"
#include "gui.h"
#include "integers.h"
#include "keyboard.h"
#include "logicalOps/logicalOps.h"
#include "mathematics/mathematics.h"
#include "matrix.h"
#include "plotstat.h"
#include "programming/programming.h"
#include "recall.h"
#include "registers.h"
#include "saveRestoreCalcState.h"
#include "screen.h"
#include "softmenus.h"
#include "stack.h"
#include "stats.h"
#include "store.h"
#include "stringFuncs.h"
#include "timer.h"
#include "ui/tam.h"

#include "wp43s.h"

/********************************************//**
 * \brief Dummy function for an item to be coded
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void itemToBeCoded(uint16_t unusedButMandatoryParameter) {
  funcOK = false;
}



#ifndef GENERATE_CATALOGS
/********************************************//**
 * \brief Dummy function for a function (part of an item) to be coded
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnToBeCoded(void) {
  displayCalcErrorMessage(ERROR_FUNCTION_TO_BE_CODED, ERR_REGISTER_LINE, REGISTER_X);
  #ifdef PC_BUILD
    moreInfoOnError("Function to be coded", "for that data type(s)!", NULL, NULL);
  #endif // PC_BUILD
}
#endif // GENERATE_CATALOGS



/********************************************//**
 * \brief No OPeration
 *
 * \param  unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnNop(uint16_t unusedButMandatoryParameter) {
}


#if !defined(TESTSUITE_BUILD) && !defined(GENERATE_CATALOGS)
  void reallyRunFunction(int16_t func, uint16_t param) {
    if((indexOfItems[func].status & US_STATUS) == US_ENABLED) {
      saveForUndo();
    }
    else if((indexOfItems[func].status & US_STATUS) == US_CANCEL) {
      thereIsSomethingToUndo = false;
    }

    indexOfItems[func].func(param);

    if(lastErrorCode != 0) {
      undo();
    }
    else {
      if((indexOfItems[func].status & SLS_STATUS) == SLS_DISABLED) {
        clearSystemFlag(FLAG_ASLIFT);
      }
      else if((indexOfItems[func].status & SLS_STATUS) == SLS_ENABLED) {
        setSystemFlag(FLAG_ASLIFT);
      }
    }

    #ifdef PC_BUILD
      refreshLcd(NULL);
    #endif // PC_BUILD
  }



  /********************************************//**
   * \brief Runs a function
   *
   * \param[in] fn int16_t Index in the indexOfItems area of the function to run
   * \return void
   ***********************************************/
  void runFunction(int16_t func) {
    funcOK = true;

    #ifdef PC_BUILD
      if(func >= LAST_ITEM) {
        sprintf(errorMessage, "item (%" PRId16 ") must be below LAST_ITEM", func);
        moreInfoOnError("In function runFunction:", errorMessage, NULL, NULL);
      }
    #endif // PC_BUILD

    if(func == ITM_RCL && dynamicMenuItem > -1) {
      char *varCatalogItem = dynmenuGetLabel(dynamicMenuItem);
      calcRegister_t regist = findNamedVariable(varCatalogItem);
      if(regist != INVALID_VARIABLE) {
       reallyRunFunction(func, regist);
      }
      return;
    }
    else if(tam.mode == 0 && TM_VALUE <= indexOfItems[func].param && indexOfItems[func].param <= TM_CMP) {
      tamEnterMode(func);
      return;
    }

    if(calcMode == CM_PEM && !tam.mode) {
      insertStepInProgram(func);
      return;
    }

    reallyRunFunction(func, indexOfItems[func].param);

    if(!funcOK) {
      displayCalcErrorMessage(ERROR_ITEM_TO_BE_CODED, ERR_REGISTER_LINE, REGISTER_X);
      #ifdef PC_BUILD
        sprintf(errorMessage, "%" PRId16 " = %s", func, indexOfItems[func].itemCatalogName);
        moreInfoOnError("In function runFunction:", "Item not implemented", errorMessage, "to be coded");
      #endif // PC_BUILD
    }
  }
#endif // !TESTSUITE_BUILD && !GENERATE_CATALOGS

#ifdef GENERATE_CATALOGS
  void registerBrowser             (uint16_t unusedButMandatoryParameter) {}
  void flagBrowser                 (uint16_t unusedButMandatoryParameter) {}
  void fontBrowser                 (uint16_t unusedButMandatoryParameter) {}
  void fnPow10                     (uint16_t unusedButMandatoryParameter) {}
  void fnIntegerMode               (uint16_t unusedButMandatoryParameter) {}
  void fnConstant                  (uint16_t unusedButMandatoryParameter) {}
  void fnInvert                    (uint16_t unusedButMandatoryParameter) {}
  void fn2Pow                      (uint16_t unusedButMandatoryParameter) {}
  void fn10Pow                     (uint16_t unusedButMandatoryParameter) {}
  void fnCubeRoot                  (uint16_t unusedButMandatoryParameter) {}
  void fnMagnitude                 (uint16_t unusedButMandatoryParameter) {}
  void fnAgm                       (uint16_t unusedButMandatoryParameter) {}
  void fnDisplayFormatAll          (uint16_t unusedButMandatoryParameter) {}
  void fnDisplayFormatFix          (uint16_t unusedButMandatoryParameter) {}
  void fnDisplayFormatSci          (uint16_t unusedButMandatoryParameter) {}
  void fnDisplayFormatEng          (uint16_t unusedButMandatoryParameter) {}
  void fnDisplayFormatGap          (uint16_t unusedButMandatoryParameter) {}
  void fnDisplayFormatTime         (uint16_t unusedButMandatoryParameter) {}
  void fnArccos                    (uint16_t unusedButMandatoryParameter) {}
  void fnArccosh                   (uint16_t unusedButMandatoryParameter) {}
  void fnArcsin                    (uint16_t unusedButMandatoryParameter) {}
  void fnArcsinh                   (uint16_t unusedButMandatoryParameter) {}
  void fnArctan                    (uint16_t unusedButMandatoryParameter) {}
  void fnArctanh                   (uint16_t unusedButMandatoryParameter) {}
  void fnCos                       (uint16_t unusedButMandatoryParameter) {}
  void fnCosh                      (uint16_t unusedButMandatoryParameter) {}
  void fnSin                       (uint16_t unusedButMandatoryParameter) {}
  void fnSinc                      (uint16_t unusedButMandatoryParameter) {}
  void fnSincpi                    (uint16_t unusedButMandatoryParameter) {}
  void fnSinh                      (uint16_t unusedButMandatoryParameter) {}
  void fnTan                       (uint16_t unusedButMandatoryParameter) {}
  void fnTanh                      (uint16_t unusedButMandatoryParameter) {}
  void fnDrop                      (uint16_t unusedButMandatoryParameter) {}
  void fnDropY                     (uint16_t unusedButMandatoryParameter) {}
  void fnBatteryVoltage            (uint16_t unusedButMandatoryParameter) {}
  void fnCurveFitting              (uint16_t unusedButMandatoryParameter) {}
  void fnCeil                      (uint16_t unusedButMandatoryParameter) {}
  void fnFloor                     (uint16_t unusedButMandatoryParameter) {}
  void fnClearFlag                 (uint16_t unusedButMandatoryParameter) {}
  void fnFlipFlag                  (uint16_t unusedButMandatoryParameter) {}
  void fnSetFlag                   (uint16_t unusedButMandatoryParameter) {}
  void fnClAll                     (uint16_t unusedButMandatoryParameter) {}
  void fnClX                       (uint16_t unusedButMandatoryParameter) {}
  void fnClFAll                    (uint16_t unusedButMandatoryParameter) {}
  void fnClPAll                    (uint16_t unusedButMandatoryParameter) {}
  void fnClSigma                   (uint16_t unusedButMandatoryParameter) {}
  void fnClearStack                (uint16_t unusedButMandatoryParameter) {}
  void fnClearRegisters            (uint16_t unusedButMandatoryParameter) {}
  void fnTimeFormat                (uint16_t unusedButMandatoryParameter) {}
  void fnSetDateFormat             (uint16_t unusedButMandatoryParameter) {}
  void fnComplexUnit               (uint16_t unusedButMandatoryParameter) {}
  void fnComplexMode               (uint16_t unusedButMandatoryParameter) {}
  void fnComplexResult             (uint16_t unusedButMandatoryParameter) {}
  void fnConjugate                 (uint16_t unusedButMandatoryParameter) {}
  void fnAngularMode               (uint16_t unusedButMandatoryParameter) {}
  void fnDenMode                   (uint16_t unusedButMandatoryParameter) {}
  void fnDenMax                    (uint16_t unusedButMandatoryParameter) {}
  void fnErf                       (uint16_t unusedButMandatoryParameter) {}
  void fnErfc                      (uint16_t unusedButMandatoryParameter) {}
  void fnExp                       (uint16_t unusedButMandatoryParameter) {}
  void fnExpM1                     (uint16_t unusedButMandatoryParameter) {}
  void fnExpt                      (uint16_t unusedButMandatoryParameter) {}
  void fnMant                      (uint16_t unusedButMandatoryParameter) {}
  void fnCxToRe                    (uint16_t unusedButMandatoryParameter) {}
  void fnReToCx                    (uint16_t unusedButMandatoryParameter) {}
  void fnFillStack                 (uint16_t unusedButMandatoryParameter) {}
  void fnIsFlagClear               (uint16_t unusedButMandatoryParameter) {}
  void fnIsFlagClearClear          (uint16_t unusedButMandatoryParameter) {}
  void fnIsFlagClearFlip           (uint16_t unusedButMandatoryParameter) {}
  void fnIsFlagClearSet            (uint16_t unusedButMandatoryParameter) {}
  void fnIsFlagSet                 (uint16_t unusedButMandatoryParameter) {}
  void fnIsFlagSetClear            (uint16_t unusedButMandatoryParameter) {}
  void fnIsFlagSetFlip             (uint16_t unusedButMandatoryParameter) {}
  void fnIsFlagSetSet              (uint16_t unusedButMandatoryParameter) {}
  void fnKeyEnter                  (uint16_t unusedButMandatoryParameter) {}
  void fnKeyExit                   (uint16_t unusedButMandatoryParameter) {}
  void fnKeyUp                     (uint16_t unusedButMandatoryParameter) {}
  void fnKeyDown                   (uint16_t unusedButMandatoryParameter) {}
  void fnKeyDotD                   (uint16_t unusedButMandatoryParameter) {}
  void fnKeyCC                     (uint16_t unusedButMandatoryParameter) {}
  void fnKeyBackspace              (uint16_t unusedButMandatoryParameter) {}
  void fnDisplayStack              (uint16_t unusedButMandatoryParameter) {}
  void fnFreeFlashMemory           (uint16_t unusedButMandatoryParameter) {}
  void fnFreeMemory                (uint16_t unusedButMandatoryParameter) {}
  void fnFp                        (uint16_t unusedButMandatoryParameter) {}
  void fnIp                        (uint16_t unusedButMandatoryParameter) {}
  void allocateLocalRegisters      (uint16_t unusedButMandatoryParameter) {}
  void fnLeadingZeros              (uint16_t unusedButMandatoryParameter) {}
  void fnNeighb                    (uint16_t unusedButMandatoryParameter) {}
  void fnGcd                       (uint16_t unusedButMandatoryParameter) {}
  void fnMin                       (uint16_t unusedButMandatoryParameter) {}
  void fnMax                       (uint16_t unusedButMandatoryParameter) {}
  void fnStatSum                   (uint16_t unusedButMandatoryParameter) {}
  void fnIsPrime                   (uint16_t unusedButMandatoryParameter) {}
  void fnIsLeap                    (uint16_t unusedButMandatoryParameter) {}
  void fnIsString                  (uint16_t unusedButMandatoryParameter) {}
  void fnCheckValue                (uint16_t unusedButMandatoryParameter) {}
  void fnRandom                    (uint16_t unusedButMandatoryParameter) {}
  void fnRandomI                   (uint16_t unusedButMandatoryParameter) {}
  void fnImaginaryPart             (uint16_t unusedButMandatoryParameter) {}
  void fnRecall                    (uint16_t unusedButMandatoryParameter) {}
  void fnRecallConfig              (uint16_t unusedButMandatoryParameter) {}
  void fnRecallElement             (uint16_t unusedButMandatoryParameter) {}
  void fnRecallIJ                  (uint16_t unusedButMandatoryParameter) {}
  void fnRecallStack               (uint16_t unusedButMandatoryParameter) {}
  void fnRecallAdd                 (uint16_t unusedButMandatoryParameter) {}
  void fnRecallSub                 (uint16_t unusedButMandatoryParameter) {}
  void fnRecallMult                (uint16_t unusedButMandatoryParameter) {}
  void fnRecallDiv                 (uint16_t unusedButMandatoryParameter) {}
  void fnRecallMin                 (uint16_t unusedButMandatoryParameter) {}
  void fnRecallMax                 (uint16_t unusedButMandatoryParameter) {}
  void fnRadixMark                 (uint16_t unusedButMandatoryParameter) {}
  void fnReset                     (uint16_t unusedButMandatoryParameter) {}
  void fnRealPart                  (uint16_t unusedButMandatoryParameter) {}
  void fnRmd                       (uint16_t unusedButMandatoryParameter) {}
  void fnRound                     (uint16_t unusedButMandatoryParameter) {}
  void fnRoundi                    (uint16_t unusedButMandatoryParameter) {}
  void fnRsd                       (uint16_t unusedButMandatoryParameter) {}
  void fnRdp                       (uint16_t unusedButMandatoryParameter) {}
  void fnRollDown                  (uint16_t unusedButMandatoryParameter) {}
  void fnRollUp                    (uint16_t unusedButMandatoryParameter) {}
  void fnSeed                      (uint16_t unusedButMandatoryParameter) {}
  void fnConfigChina               (uint16_t unusedButMandatoryParameter) {}
  void fnSetDate                   (uint16_t unusedButMandatoryParameter) {}
  void fnConfigEurope              (uint16_t unusedButMandatoryParameter) {}
  void fnConfigIndia               (uint16_t unusedButMandatoryParameter) {}
  void fnConfigJapan               (uint16_t unusedButMandatoryParameter) {}
  void fnSetTime                   (uint16_t unusedButMandatoryParameter) {}
  void fnConfigUk                  (uint16_t unusedButMandatoryParameter) {}
  void fnConfigUsa                 (uint16_t unusedButMandatoryParameter) {}
  void fnLcm                       (uint16_t unusedButMandatoryParameter) {}
  void fnSign                      (uint16_t unusedButMandatoryParameter) {}
  void fnSlvq                      (uint16_t unusedButMandatoryParameter) {}
  void fnGetIntegerSignMode        (uint16_t unusedButMandatoryParameter) {}
  void fnLog2                      (uint16_t unusedButMandatoryParameter) {}
  void fnLog10                     (uint16_t unusedButMandatoryParameter) {}
  void fnLn                        (uint16_t unusedButMandatoryParameter) {}
  void fnLogXY                     (uint16_t unusedButMandatoryParameter) {}
  void fnLnP1                      (uint16_t unusedButMandatoryParameter) {}
  void fnLnGamma                   (uint16_t unusedButMandatoryParameter) {}
  void fnLnBeta                    (uint16_t unusedButMandatoryParameter) {}
  void fnBeta                      (uint16_t unusedButMandatoryParameter) {}
  void fnIxyz                      (uint16_t unusedButMandatoryParameter) {}
  void fnGamma                     (uint16_t unusedButMandatoryParameter) {}
  void fnGammaP                    (uint16_t unusedButMandatoryParameter) {}
  void fnGammaQ                    (uint16_t unusedButMandatoryParameter) {}
  void fnGammaXyLower              (uint16_t unusedButMandatoryParameter) {}
  void fnGammaXyUpper              (uint16_t unusedButMandatoryParameter) {}
  void fnBessel                    (uint16_t unusedButMandatoryParameter) {}
  void fnZeta                      (uint16_t unusedButMandatoryParameter) {}
  void fnBn                        (uint16_t unusedButMandatoryParameter) {}
  void fnBnStar                    (uint16_t unusedButMandatoryParameter) {}
  void fnWnegative                 (uint16_t unusedButMandatoryParameter) {}
  void fnWpositive                 (uint16_t unusedButMandatoryParameter) {}
  void fnWinverse                  (uint16_t unusedButMandatoryParameter) {}
  void fnHermite                   (uint16_t unusedButMandatoryParameter) {}
  void fnHermiteP                  (uint16_t unusedButMandatoryParameter) {}
  void fnLaguerre                  (uint16_t unusedButMandatoryParameter) {}
  void fnLaguerreAlpha             (uint16_t unusedButMandatoryParameter) {}
  void fnLegendre                  (uint16_t unusedButMandatoryParameter) {}
  void fnChebyshevT                (uint16_t unusedButMandatoryParameter) {}
  void fnChebyshevU                (uint16_t unusedButMandatoryParameter) {}
  void fnIDiv                      (uint16_t unusedButMandatoryParameter) {}
  void fnIDivR                     (uint16_t unusedButMandatoryParameter) {}
  void fnMirror                    (uint16_t unusedButMandatoryParameter) {}
  void fnMod                       (uint16_t unusedButMandatoryParameter) {}
  void fnMulMod                    (uint16_t unusedButMandatoryParameter) {}
  void fnExpMod                    (uint16_t unusedButMandatoryParameter) {}
  void fnPower                     (uint16_t unusedButMandatoryParameter) {}
  void fnPi                        (uint16_t unusedButMandatoryParameter) {}
  void fnUserMode                  (uint16_t unusedButMandatoryParameter) {}
  void fnParallel                  (uint16_t unusedButMandatoryParameter) {}
  void fnSquareRoot                (uint16_t unusedButMandatoryParameter) {}
  void fnSubtract                  (uint16_t unusedButMandatoryParameter) {}
  void fnChangeSign                (uint16_t unusedButMandatoryParameter) {}
  void fnM1Pow                     (uint16_t unusedButMandatoryParameter) {}
  void backToSystem                (uint16_t unusedButMandatoryParameter) {}
  void fnMultiply                  (uint16_t unusedButMandatoryParameter) {}
  void fnDblDivide                 (uint16_t unusedButMandatoryParameter) {}
  void fnDblDivideRemainder        (uint16_t unusedButMandatoryParameter) {}
  void fnDblMultiply               (uint16_t unusedButMandatoryParameter) {}
  void fnChangeBase                (uint16_t unusedButMandatoryParameter) {}
  void fnToPolar                   (uint16_t unusedButMandatoryParameter) {}
  void fnToRect                    (uint16_t unusedButMandatoryParameter) {}
  void fnDivide                    (uint16_t unusedButMandatoryParameter) {}
  void fnAdd                       (uint16_t unusedButMandatoryParameter) {}
  void fnSigma                     (uint16_t unusedButMandatoryParameter) {}
  void fnXEqualsTo                 (uint16_t unusedButMandatoryParameter) {}
  void fnXNotEqual                 (uint16_t unusedButMandatoryParameter) {}
  void fnXAlmostEqual              (uint16_t unusedButMandatoryParameter) {}
  void fnXLessThan                 (uint16_t unusedButMandatoryParameter) {}
  void fnXLessEqual                (uint16_t unusedButMandatoryParameter) {}
  void fnXGreaterThan              (uint16_t unusedButMandatoryParameter) {}
  void fnXGreaterEqual             (uint16_t unusedButMandatoryParameter) {}
  void fnIsConverged               (uint16_t unusedButMandatoryParameter) {}
  void fnGetLocR                   (uint16_t unusedButMandatoryParameter) {}
  void fnSwapRealImaginary         (uint16_t unusedButMandatoryParameter) {}
  void fnSetRoundingMode           (uint16_t unusedButMandatoryParameter) {}
  void fnGetRoundingMode           (uint16_t unusedButMandatoryParameter) {}
  void fnSetWordSize               (uint16_t unusedButMandatoryParameter) {}
  void fnGetWordSize               (uint16_t unusedButMandatoryParameter) {}
  void fnGetStackSize              (uint16_t unusedButMandatoryParameter) {}
  void fnStackSize                 (uint16_t unusedButMandatoryParameter) {}
  void fnStore                     (uint16_t unusedButMandatoryParameter) {}
  void fnStoreConfig               (uint16_t unusedButMandatoryParameter) {}
  void fnStoreElement              (uint16_t unusedButMandatoryParameter) {}
  void fnStoreIJ                   (uint16_t unusedButMandatoryParameter) {}
  void fnStoreStack                (uint16_t unusedButMandatoryParameter) {}
  void fnStoreAdd                  (uint16_t unusedButMandatoryParameter) {}
  void fnStoreSub                  (uint16_t unusedButMandatoryParameter) {}
  void fnStoreMult                 (uint16_t unusedButMandatoryParameter) {}
  void fnStoreDiv                  (uint16_t unusedButMandatoryParameter) {}
  void fnStoreMax                  (uint16_t unusedButMandatoryParameter) {}
  void fnStoreMin                  (uint16_t unusedButMandatoryParameter) {}
  void fnUlp                       (uint16_t unusedButMandatoryParameter) {}
  void fnUnitVector                (uint16_t unusedButMandatoryParameter) {}
  void fnVersion                   (uint16_t unusedButMandatoryParameter) {}
  void fnSquare                    (uint16_t unusedButMandatoryParameter) {}
  void fnCube                      (uint16_t unusedButMandatoryParameter) {}
  void fnFactorial                 (uint16_t unusedButMandatoryParameter) {}
  void fnSwapX                     (uint16_t unusedButMandatoryParameter) {}
  void fnSwapY                     (uint16_t unusedButMandatoryParameter) {}
  void fnSwapZ                     (uint16_t unusedButMandatoryParameter) {}
  void fnSwapT                     (uint16_t unusedButMandatoryParameter) {}
  void fnSwapXY                    (uint16_t unusedButMandatoryParameter) {}
  void fnShuffle                   (uint16_t unusedButMandatoryParameter) {}
  void fnWho                       (uint16_t unusedButMandatoryParameter) {}
  void fnGetSignificantDigits      (uint16_t unusedButMandatoryParameter) {}
  void fnSdl                       (uint16_t unusedButMandatoryParameter) {}
  void fnSdr                       (uint16_t unusedButMandatoryParameter) {}
  void fnCvtToCurrentAngularMode   (uint16_t unusedButMandatoryParameter) {}
  void fnCvtAcreHa                 (uint16_t unusedButMandatoryParameter) {}
  void fnCvtAcreusHa               (uint16_t unusedButMandatoryParameter) {}
  void fnCvtAtmPa                  (uint16_t unusedButMandatoryParameter) {}
  void fnCvtAuM                    (uint16_t unusedButMandatoryParameter) {}
  void fnCvtBarPa                  (uint16_t unusedButMandatoryParameter) {}
  void fnCvtBtuJ                   (uint16_t unusedButMandatoryParameter) {}
  void fnCvtCalJ                   (uint16_t unusedButMandatoryParameter) {}
  void fnCvtCwtKg                  (uint16_t unusedButMandatoryParameter) {}
  void fnCvtFtM                    (uint16_t unusedButMandatoryParameter) {}
  void fnCvtDegToRad               (uint16_t unusedButMandatoryParameter) {}
  void fnCvtSfeetM                 (uint16_t unusedButMandatoryParameter) {}
  void fnCvtFlozukMl               (uint16_t unusedButMandatoryParameter) {}
  void fnCvtFlozusMl               (uint16_t unusedButMandatoryParameter) {}
  void fnCvtGalukL                 (uint16_t unusedButMandatoryParameter) {}
  void fnCvtGalusL                 (uint16_t unusedButMandatoryParameter) {}
  void fnCvtDbRatio                (uint16_t unusedButMandatoryParameter) {}
  void fnCvtRatioDb                (uint16_t unusedButMandatoryParameter) {}
  void fnCvtHpeW                   (uint16_t unusedButMandatoryParameter) {}
  void fnCvtHpmW                   (uint16_t unusedButMandatoryParameter) {}
  void fnCvtHpukW                  (uint16_t unusedButMandatoryParameter) {}
  void fnCvtInhgPa                 (uint16_t unusedButMandatoryParameter) {}
  void fnCvtMmhgPa                 (uint16_t unusedButMandatoryParameter) {}
  void fnCvtInchMm                 (uint16_t unusedButMandatoryParameter) {}
  void fnCvtWhJ                    (uint16_t unusedButMandatoryParameter) {}
  void fnCvtLbKg                   (uint16_t unusedButMandatoryParameter) {}
  void fnCvtOzG                    (uint16_t unusedButMandatoryParameter) {}
  void fnCvtStoneKg                (uint16_t unusedButMandatoryParameter) {}
  void fnCvtShorttonKg             (uint16_t unusedButMandatoryParameter) {}
  void fnCvtTrozG                  (uint16_t unusedButMandatoryParameter) {}
  void fnCvtLbfN                   (uint16_t unusedButMandatoryParameter) {}
  void fnCvtMiKm                   (uint16_t unusedButMandatoryParameter) {}
  void fnCvtLyM                    (uint16_t unusedButMandatoryParameter) {}
  void fnCvtTonKg                  (uint16_t unusedButMandatoryParameter) {}
  void fnCvtTorrPa                 (uint16_t unusedButMandatoryParameter) {}
  void fnCvtYardM                  (uint16_t unusedButMandatoryParameter) {}
  void fnCvtPcM                    (uint16_t unusedButMandatoryParameter) {}
  void fnCvtPointMm                (uint16_t unusedButMandatoryParameter) {}
  void fnCvtCToF                   (uint16_t unusedButMandatoryParameter) {}
  void fnCvtFToC                   (uint16_t unusedButMandatoryParameter) {}
  void fnCvtNmiKm                  (uint16_t unusedButMandatoryParameter) {}
  void fnCvtPsiPa                  (uint16_t unusedButMandatoryParameter) {}
  void fnCvtShortcwtKg             (uint16_t unusedButMandatoryParameter) {}
  void fnCvtLbfftNm                (uint16_t unusedButMandatoryParameter) {}
  void fnCvtRadToDeg               (uint16_t unusedButMandatoryParameter) {}
  void fnCvtFromCurrentAngularMode (uint16_t unusedButMandatoryParameter) {}
  void fnCvtYearS                  (uint16_t unusedButMandatoryParameter) {}
  void fnCvtCaratG                 (uint16_t unusedButMandatoryParameter) {}
  void fnCvtJinKg                  (uint16_t unusedButMandatoryParameter) {}
  void fnCvtLiangKg                (uint16_t unusedButMandatoryParameter) {}
  void fnCvtQuartL                 (uint16_t unusedButMandatoryParameter) {}
  void fnCvtDmsToDeg               (uint16_t unusedButMandatoryParameter) {}
  void fnCvtDmsToCurrentAngularMode(uint16_t unusedButMandatoryParameter) {}
  void fnCvtFathomM                (uint16_t unusedButMandatoryParameter) {}
  void fnCvtBarrelM3               (uint16_t unusedButMandatoryParameter) {}
  void fnCvtHectareM2              (uint16_t unusedButMandatoryParameter) {}
  void fnCvtDegToDms               (uint16_t unusedButMandatoryParameter) {}
  void fnCvtMuM2                   (uint16_t unusedButMandatoryParameter) {}
  void fnCvtLiM                    (uint16_t unusedButMandatoryParameter) {}
  void fnCvtChiM                   (uint16_t unusedButMandatoryParameter) {}
  void fnCvtYinM                   (uint16_t unusedButMandatoryParameter) {}
  void fnCvtCunM                   (uint16_t unusedButMandatoryParameter) {}
  void fnCvtZhangM                 (uint16_t unusedButMandatoryParameter) {}
  void fnCvtFenM                   (uint16_t unusedButMandatoryParameter) {}
  void addItemToBuffer             (uint16_t unusedButMandatoryParameter) {}
  void fnOff                       (uint16_t unusedButMandatoryParameter) {}
  void fnAim                       (uint16_t unusedButMandatoryParameter) {}
  void fnShow                      (uint16_t unusedButMandatoryParameter) {}
  void fnLastX                     (uint16_t unusedButMandatoryParameter) {}
  void fnCyx                       (uint16_t unusedButMandatoryParameter) {}
  void fnPyx                       (uint16_t unusedButMandatoryParameter) {}
  void fnJulianToDate              (uint16_t unusedButMandatoryParameter) {}
  void fnDateToJulian              (uint16_t unusedButMandatoryParameter) {}
  void fnToDate                    (uint16_t unusedButMandatoryParameter) {}
  void fnDateTo                    (uint16_t unusedButMandatoryParameter) {}
  void fnXToDate                   (uint16_t unusedButMandatoryParameter) {}
  void fnYear                      (uint16_t unusedButMandatoryParameter) {}
  void fnMonth                     (uint16_t unusedButMandatoryParameter) {}
  void fnDay                       (uint16_t unusedButMandatoryParameter) {}
  void fnWday                      (uint16_t unusedButMandatoryParameter) {}
  void fnToHr                      (uint16_t unusedButMandatoryParameter) {}
  void fnToHms                     (uint16_t unusedButMandatoryParameter) {}
  void fnToReal                    (uint16_t unusedButMandatoryParameter) {}
  void fnDec                       (uint16_t unusedButMandatoryParameter) {}
  void fnInc                       (uint16_t unusedButMandatoryParameter) {}
  void fnIse                       (uint16_t unusedButMandatoryParameter) {}
  void fnIsg                       (uint16_t unusedButMandatoryParameter) {}
  void fnIsz                       (uint16_t unusedButMandatoryParameter) {}
  void fnDse                       (uint16_t unusedButMandatoryParameter) {}
  void fnDsl                       (uint16_t unusedButMandatoryParameter) {}
  void fnDsz                       (uint16_t unusedButMandatoryParameter) {}
  void fncountBits                 (uint16_t unusedButMandatoryParameter) {}
  void fnLogicalNot                (uint16_t unusedButMandatoryParameter) {}
  void fnLogicalAnd                (uint16_t unusedButMandatoryParameter) {}
  void fnLogicalNand               (uint16_t unusedButMandatoryParameter) {}
  void fnLogicalOr                 (uint16_t unusedButMandatoryParameter) {}
  void fnLogicalNor                (uint16_t unusedButMandatoryParameter) {}
  void fnLogicalXor                (uint16_t unusedButMandatoryParameter) {}
  void fnLogicalXnor               (uint16_t unusedButMandatoryParameter) {}
  void fnDecomp                    (uint16_t unusedButMandatoryParameter) {}
  void fnPlotStat                  (uint16_t unusedButMandatoryParameter) {}
  void fnSumXY                     (uint16_t unusedButMandatoryParameter) {}
  void fnMeanXY                    (uint16_t unusedButMandatoryParameter) {}
  void fnGeometricMeanXY           (uint16_t unusedButMandatoryParameter) {}
  void fnWeightedMeanX             (uint16_t unusedButMandatoryParameter) {}
  void fnHarmonicMeanXY            (uint16_t unusedButMandatoryParameter) {}
  void fnRMSMeanXY                 (uint16_t unusedButMandatoryParameter) {}
  void fnWeightedSampleStdDev      (uint16_t unusedButMandatoryParameter) {}
  void fnWeightedPopulationStdDev  (uint16_t unusedButMandatoryParameter) {}
  void fnWeightedStandardError     (uint16_t unusedButMandatoryParameter) {}
  void fnSampleStdDev              (uint16_t unusedButMandatoryParameter) {}
  void fnPopulationStdDev          (uint16_t unusedButMandatoryParameter) {}
  void fnStandardError             (uint16_t unusedButMandatoryParameter) {}
  void fnGeometricSampleStdDev     (uint16_t unusedButMandatoryParameter) {}
  void fnGeometricPopulationStdDev (uint16_t unusedButMandatoryParameter) {}
  void fnGeometricStandardError    (uint16_t unusedButMandatoryParameter) {}
  void fnPopulationCovariance      (uint16_t unusedButMandatoryParameter) {}
  void fnSampleCovariance          (uint16_t unusedButMandatoryParameter) {}
  void fnCoefficientDetermination  (uint16_t unusedButMandatoryParameter) {}
  void fnMinExpStdDev              (uint16_t unusedButMandatoryParameter) {}
  void fnPlotCloseSmi              (uint16_t unusedButMandatoryParameter) {}
  void fnMaskl                     (uint16_t unusedButMandatoryParameter) {}
  void fnMaskr                     (uint16_t unusedButMandatoryParameter) {}
  void fnAsr                       (uint16_t unusedButMandatoryParameter) {}
  void fnCb                        (uint16_t unusedButMandatoryParameter) {}
  void fnSb                        (uint16_t unusedButMandatoryParameter) {}
  void fnFb                        (uint16_t unusedButMandatoryParameter) {}
  void fnBs                        (uint16_t unusedButMandatoryParameter) {}
  void fnBc                        (uint16_t unusedButMandatoryParameter) {}
  void fnSl                        (uint16_t unusedButMandatoryParameter) {}
  void fnRl                        (uint16_t unusedButMandatoryParameter) {}
  void fnRlc                       (uint16_t unusedButMandatoryParameter) {}
  void fnSr                        (uint16_t unusedButMandatoryParameter) {}
  void fnRr                        (uint16_t unusedButMandatoryParameter) {}
  void fnRrc                       (uint16_t unusedButMandatoryParameter) {}
  void fnLj                        (uint16_t unusedButMandatoryParameter) {}
  void fnRj                        (uint16_t unusedButMandatoryParameter) {}
  void fnCountBits                 (uint16_t unusedButMandatoryParameter) {}
  void fnNextPrime                 (uint16_t unusedButMandatoryParameter) {}
  void fnScreenDump                (uint16_t unusedButMandatoryParameter) {}
  void fnArg                       (uint16_t unusedButMandatoryParameter) {}
  void fnRange                     (uint16_t unusedButMandatoryParameter) {}
  void fnGetRange                  (uint16_t unusedButMandatoryParameter) {}
  void fnDot                       (uint16_t unusedButMandatoryParameter) {}
  void fnCross                     (uint16_t unusedButMandatoryParameter) {}
  void fnPercent                   (uint16_t unusedButMandatoryParameter) {}
  void fnPercentMRR                (uint16_t unusedButMandatoryParameter) {}
  void fnPercentT                  (uint16_t unusedButMandatoryParameter) {}
  void fnPercentSigma              (uint16_t unusedButMandatoryParameter) {}
  void fnPercentPlusMG             (uint16_t unusedButMandatoryParameter) {}
  void fnDeltaPercent              (uint16_t unusedButMandatoryParameter) {}
  void fnXthRoot                   (uint16_t unusedButMandatoryParameter) {}
  void fnGetSystemFlag             (uint16_t unusedButMandatoryParameter) {}
  void fnFractionType              (uint16_t unusedButMandatoryParameter) {}
  void fnAlphaLeng                 (uint16_t unusedButMandatoryParameter) {}
  void fnAlphaSR                   (uint16_t unusedButMandatoryParameter) {}
  void fnAlphaSL                   (uint16_t unusedButMandatoryParameter) {}
  void fnAlphaRR                   (uint16_t unusedButMandatoryParameter) {}
  void fnAlphaRL                   (uint16_t unusedButMandatoryParameter) {}
  void fnAlphaPos                  (uint16_t unusedButMandatoryParameter) {}
  void fnXToAlpha                  (uint16_t unusedButMandatoryParameter) {}
  void fnAlphaToX                  (uint16_t unusedButMandatoryParameter) {}
  void fnTicks                     (uint16_t unusedButMandatoryParameter) {}
  void fnSetFirstGregorianDay      (uint16_t unusedButMandatoryParameter) {}
  void fnGetFirstGregorianDay      (uint16_t unusedButMandatoryParameter) {}
  void fnDate                      (uint16_t unusedButMandatoryParameter) {}
  void fnTime                      (uint16_t unusedButMandatoryParameter) {}
  void fnSave                      (uint16_t unusedButMandatoryParameter) {}
  void fnLoad                      (uint16_t unusedButMandatoryParameter) {}
  void fnUndo                      (uint16_t unusedButMandatoryParameter) {}
  void fnXmax                      (uint16_t unusedButMandatoryParameter) {}
  void fnXmin                      (uint16_t unusedButMandatoryParameter) {}
  void fnFib                       (uint16_t unusedButMandatoryParameter) {}
  void fnGd                        (uint16_t unusedButMandatoryParameter) {}
  void fnInvGd                     (uint16_t unusedButMandatoryParameter) {}
  void fnClP                       (uint16_t unusedButMandatoryParameter) {}
  void fnPem                       (uint16_t unusedButMandatoryParameter) {}
  void fnGoto                      (uint16_t unusedButMandatoryParameter) {}
  void fnGotoDot                   (uint16_t unusedButMandatoryParameter) {}
  void fnDynamicMenu               (uint16_t unusedButMandatoryParameter) {}
  void fnCheckInteger              (uint16_t unusedButMandatoryParameter) {}
  void fnNormalP                   (uint16_t unusedButMandatoryParameter) {}
  void fnNormalL                   (uint16_t unusedButMandatoryParameter) {}
  void fnNormalR                   (uint16_t unusedButMandatoryParameter) {}
  void fnNormalI                   (uint16_t unusedButMandatoryParameter) {}
  void fnT_P                       (uint16_t unusedButMandatoryParameter) {}
  void fnT_L                       (uint16_t unusedButMandatoryParameter) {}
  void fnT_R                       (uint16_t unusedButMandatoryParameter) {}
  void fnT_I                       (uint16_t unusedButMandatoryParameter) {}
  void fnChi2P                     (uint16_t unusedButMandatoryParameter) {}
  void fnChi2L                     (uint16_t unusedButMandatoryParameter) {}
  void fnChi2R                     (uint16_t unusedButMandatoryParameter) {}
  void fnChi2I                     (uint16_t unusedButMandatoryParameter) {}
  void fnF_P                       (uint16_t unusedButMandatoryParameter) {}
  void fnF_L                       (uint16_t unusedButMandatoryParameter) {}
  void fnF_R                       (uint16_t unusedButMandatoryParameter) {}
  void fnF_I                       (uint16_t unusedButMandatoryParameter) {}
  void fnProcessLR                 (uint16_t unusedButMandatoryParameter) {}
  void fnPlotZoom                  (uint16_t unusedButMandatoryParameter) {}
  void fnYIsFnx                    (uint16_t unusedButMandatoryParameter) {}
  void fnXIsFny                    (uint16_t unusedButMandatoryParameter) {}
  void fnStatSa                    (uint16_t unusedButMandatoryParameter) {}
  void fnCurveFittingLR            (uint16_t unusedButMandatoryParameter) {}
  void fnLogNormalP                (uint16_t unusedButMandatoryParameter) {}
  void fnLogNormalL                (uint16_t unusedButMandatoryParameter) {}
  void fnLogNormalR                (uint16_t unusedButMandatoryParameter) {}
  void fnLogNormalI                (uint16_t unusedButMandatoryParameter) {}
  void fnCauchyP                   (uint16_t unusedButMandatoryParameter) {}
  void fnCauchyL                   (uint16_t unusedButMandatoryParameter) {}
  void fnCauchyR                   (uint16_t unusedButMandatoryParameter) {}
  void fnCauchyI                   (uint16_t unusedButMandatoryParameter) {}
  void fnExponentialP              (uint16_t unusedButMandatoryParameter) {}
  void fnExponentialL              (uint16_t unusedButMandatoryParameter) {}
  void fnExponentialR              (uint16_t unusedButMandatoryParameter) {}
  void fnExponentialI              (uint16_t unusedButMandatoryParameter) {}
  void fnLogisticP                 (uint16_t unusedButMandatoryParameter) {}
  void fnLogisticL                 (uint16_t unusedButMandatoryParameter) {}
  void fnLogisticR                 (uint16_t unusedButMandatoryParameter) {}
  void fnLogisticI                 (uint16_t unusedButMandatoryParameter) {}
  void fnWeibullP                  (uint16_t unusedButMandatoryParameter) {}
  void fnWeibullL                  (uint16_t unusedButMandatoryParameter) {}
  void fnWeibullR                  (uint16_t unusedButMandatoryParameter) {}
  void fnWeibullI                  (uint16_t unusedButMandatoryParameter) {}
  void fnNegBinomialP              (uint16_t unusedButMandatoryParameter) {}
  void fnNegBinomialL              (uint16_t unusedButMandatoryParameter) {}
  void fnNegBinomialR              (uint16_t unusedButMandatoryParameter) {}
  void fnNegBinomialI              (uint16_t unusedButMandatoryParameter) {}
  void fnGeometricP                (uint16_t unusedButMandatoryParameter) {}
  void fnGeometricL                (uint16_t unusedButMandatoryParameter) {}
  void fnGeometricR                (uint16_t unusedButMandatoryParameter) {}
  void fnGeometricI                (uint16_t unusedButMandatoryParameter) {}
  void fnHypergeometricP           (uint16_t unusedButMandatoryParameter) {}
  void fnHypergeometricL           (uint16_t unusedButMandatoryParameter) {}
  void fnHypergeometricR           (uint16_t unusedButMandatoryParameter) {}
  void fnHypergeometricI           (uint16_t unusedButMandatoryParameter) {}
  void fnBinomialP                 (uint16_t unusedButMandatoryParameter) {}
  void fnBinomialL                 (uint16_t unusedButMandatoryParameter) {}
  void fnBinomialR                 (uint16_t unusedButMandatoryParameter) {}
  void fnBinomialI                 (uint16_t unusedButMandatoryParameter) {}
  void fnPoissonP                  (uint16_t unusedButMandatoryParameter) {}
  void fnPoissonL                  (uint16_t unusedButMandatoryParameter) {}
  void fnPoissonR                  (uint16_t unusedButMandatoryParameter) {}
  void fnPoissonI                  (uint16_t unusedButMandatoryParameter) {}
  void fnNewMatrix                 (uint16_t unusedButMandatoryParameter) {}
  void fnEditMatrix                (uint16_t unusedButMandatoryParameter) {}
  void fnOldMatrix                 (uint16_t unusedButMandatoryParameter) {}
  void fnGoToElement               (uint16_t unusedButMandatoryParameter) {}
  void fnGoToRow                   (uint16_t unusedButMandatoryParameter) {}
  void fnGoToColumn                (uint16_t unusedButMandatoryParameter) {}
  void fnSetGrowMode               (uint16_t unusedButMandatoryParameter) {}
  void fnIncDecI                   (uint16_t unusedButMandatoryParameter) {}
  void fnIncDecJ                   (uint16_t unusedButMandatoryParameter) {}
  void fnInsRow                    (uint16_t unusedButMandatoryParameter) {}
  void fnDelRow                    (uint16_t unusedButMandatoryParameter) {}
  void fnSetMatrixDimensions       (uint16_t unusedButMandatoryParameter) {}
  void fnGetMatrixDimensions       (uint16_t unusedButMandatoryParameter) {}
  void fnTranspose                 (uint16_t unusedButMandatoryParameter) {}
  void fnLuDecomposition           (uint16_t unusedButMandatoryParameter) {}
  void fnDeterminant               (uint16_t unusedButMandatoryParameter) {}
  void fnInvertMatrix              (uint16_t unusedButMandatoryParameter) {}
  void fnEuclideanNorm             (uint16_t unusedButMandatoryParameter) {}
  void fnRowSum                    (uint16_t unusedButMandatoryParameter) {}
  void fnRowNorm                   (uint16_t unusedButMandatoryParameter) {}
  void fnIndexMatrix               (uint16_t unusedButMandatoryParameter) {}
  void fnGetMatrix                 (uint16_t unusedButMandatoryParameter) {}
  void fnPutMatrix                 (uint16_t unusedButMandatoryParameter) {}
  void fnSwapRows                  (uint16_t unusedButMandatoryParameter) {}
  void fnSimultaneousLinearEquation(uint16_t unusedButMandatoryParameter) {}
  void fnEditLinearEquationMatrixA (uint16_t unusedButMandatoryParameter) {}
  void fnEditLinearEquationMatrixB (uint16_t unusedButMandatoryParameter) {}
  void fnEditLinearEquationMatrixX (uint16_t unusedButMandatoryParameter) {}
#endif // GENERATE_CATALOGS

TO_QSPI const item_t indexOfItems[] = {

//            function                      parameter                    item in catalog                                item in softmenu                               TAM min                 max  CATALOG    stackLift       UNDO status
/*    0 */  { itemToBeCoded,                NOPARAM,                     "",                                            "0000",                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED}, // ITM_NULL

// Items from 1 to 127 are 1 byte OP codes
/*    1 */  { itemToBeCoded,                TM_LABEL,                    "LBL",                                         "LBL",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*    2 */  { fnGoto,                       TM_LABEL,                    "GTO",                                         "GTO",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*    3 */  { itemToBeCoded,                TM_LABEL,                    "XEQ",                                         "XEQ",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*    4 */  { itemToBeCoded,                NOPARAM,                     "RTN",                                         "RTN",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*    5 */  { fnIse,                        TM_REGISTER,                 "ISE",                                         "ISE",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*    6 */  { fnIsg,                        TM_REGISTER,                 "ISG",                                         "ISG",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*    7 */  { fnIsz,                        TM_REGISTER,                 "ISZ",                                         "ISZ",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*    8 */  { fnDse,                        TM_REGISTER,                 "DSE",                                         "DSE",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*    9 */  { fnDsl,                        TM_REGISTER,                 "DSL",                                         "DSL",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   10 */  { fnDsz,                        TM_REGISTER,                 "DSZ",                                         "DSZ",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   11 */  { fnXEqualsTo,                  TM_CMP,                      "x= ?",                                        "x= ?",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   12 */  { fnXNotEqual,                  TM_CMP,                      "x" STD_NOT_EQUAL " ?",                        "x" STD_NOT_EQUAL " ?",                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   13 */  { fnCheckValue,                 CHECK_VALUE_POSITIVE_ZERO,   "x=+0?",                                       "x=+0?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   14 */  { fnCheckValue,                 CHECK_VALUE_NEGATIVE_ZERO,   "x=-0?",                                       "x=-0?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   15 */  { fnXAlmostEqual,               TM_CMP,                      "x" STD_ALMOST_EQUAL " ?",                     "x" STD_ALMOST_EQUAL " ?",                     (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   16 */  { fnXLessThan,                  TM_CMP,                      "x< ?",                                        "x< ?",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   17 */  { fnXLessEqual,                 TM_CMP,                      "x" STD_LESS_EQUAL " ?",                       "x" STD_LESS_EQUAL " ?",                       (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   18 */  { fnXGreaterEqual,              TM_CMP,                      "x" STD_GREATER_EQUAL " ?",                    "x" STD_GREATER_EQUAL " ?",                    (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   19 */  { fnXGreaterThan,               TM_CMP,                      "x> ?",                                        "x> ?",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   20 */  { fnIsFlagClear,                TM_FLAGR,                    "FC?",                                         "FC?",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   21 */  { fnIsFlagSet,                  TM_FLAGR,                    "FS?",                                         "FS?",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   22 */  { fnCheckInteger,               CHECK_INTEGER_EVEN,          "EVEN?",                                       "EVEN?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   23 */  { fnCheckInteger,               CHECK_INTEGER_ODD,           "ODD?",                                        "ODD?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   24 */  { fnCheckInteger,               CHECK_INTEGER_FP,            "FP?",                                         "FP?",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   25 */  { fnCheckInteger,               CHECK_INTEGER,               "INT?",                                        "INT?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   26 */  { fnCheckValue,                 CHECK_VALUE_COMPLEX,         "CPX?",                                        "CPX?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   27 */  { fnCheckValue,                 CHECK_VALUE_MATRIX,          "MATR?",                                       "MATR?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   28 */  { fnCheckValue,                 CHECK_VALUE_NAN,             "NaN?",                                        "NaN?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   29 */  { fnCheckValue,                 CHECK_VALUE_REAL,            "REAL?",                                       "REAL?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   30 */  { fnCheckValue,                 CHECK_VALUE_SPECIAL,         "SPEC?",                                       "SPEC?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   31 */  { fnIsString,                   NOPARAM,                     "STRI?",                                       "STRI?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   32 */  { fnCheckValue,                 CHECK_VALUE_INFINITY,        STD_PLUS_MINUS STD_INFINITY "?",               STD_PLUS_MINUS STD_INFINITY "?",               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   33 */  { fnIsPrime,                    NOPARAM,                     "PRIME?",                                      "PRIME?",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   34 */  { itemToBeCoded,                NOPARAM,                     "TOP?",                                        "TOP?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   35 */  { fnKeyEnter,                   NOPARAM,                     "ENTER" STD_UP_ARROW,                          "ENTER" STD_UP_ARROW,                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_DISABLED  | US_ENABLED  },
/*   36 */  { fnSwapXY,                     NOPARAM,                     "x" STD_LEFT_RIGHT_ARROWS "y",                 "x" STD_LEFT_RIGHT_ARROWS "y",                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   37 */  { fnDrop,                       NOPARAM,                     "DROP",                                        "DROP" STD_DOWN_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   38 */  { itemToBeCoded,                NOPARAM,                     "PAUSE",                                       "PAUSE",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   39 */  { fnRollUp,                     NOPARAM,                     "R" STD_UP_ARROW,                              "R" STD_UP_ARROW,                              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   40 */  { fnRollDown,                   NOPARAM,                     "R" STD_DOWN_ARROW,                            "R" STD_DOWN_ARROW,                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   41 */  { fnClX,                        NOPARAM,                     "CLX",                                         "CLX",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_DISABLED  | US_ENABLED  },
/*   42 */  { fnFillStack,                  NOPARAM,                     "FILL",                                        "FILL",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   43 */  { itemToBeCoded,                NOPARAM,                     "INPUT",                                       "INPUT",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   44 */  { fnStore,                      TM_STORCL,                   "STO",                                         "STO",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   45 */  { fnStoreAdd,                   NOPARAM,                     "STO+",                                        "STO+",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   46 */  { fnStoreSub,                   NOPARAM,                     "STO-",                                        "STO-",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   47 */  { fnStoreMult,                  NOPARAM,                     "STO" STD_CROSS,                               "STO" STD_CROSS,                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   48 */  { fnStoreDiv,                   NOPARAM,                     "STO/",                                        "STO/",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   49 */  { fnCyx,                        NOPARAM,                     "COMB",                                        "C" STD_SUB_y STD_SUB_x,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   50 */  { fnPyx,                        NOPARAM,                     "PERM",                                        "P" STD_SUB_y STD_SUB_x,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   51 */  { fnRecall,                     TM_STORCL,                   "RCL",                                         "RCL",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   52 */  { fnRecallAdd,                  NOPARAM,                     "RCL+",                                        "RCL+",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   53 */  { fnRecallSub,                  NOPARAM,                     "RCL-",                                        "RCL-",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   54 */  { fnRecallMult,                 NOPARAM,                     "RCL" STD_CROSS,                               "RCL" STD_CROSS,                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   55 */  { fnRecallDiv,                  NOPARAM,                     "RCL/",                                        "RCL/",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   56 */  { fnIsConverged,                TM_VALUE,                    "CONVG?",                                      "CONVG?",                                      (0 << TAM_MAX_BITS) |    31, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   57 */  { itemToBeCoded,                NOPARAM,                     "ENTRY?",                                      "ENTRY?",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   58 */  { fnSquare,                     NOPARAM,                     "x" STD_SUP_2,                                 "x" STD_SUP_2,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   59 */  { fnCube,                       NOPARAM,                     "x" STD_SUP_3,                                 "x" STD_SUP_3,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   60 */  { fnPower,                      NOPARAM,                     "y" STD_SUP_x,                                 "y" STD_SUP_x,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   61 */  { fnSquareRoot,                 NOPARAM,                     STD_SQUARE_ROOT STD_x_UNDER_ROOT,              STD_SQUARE_ROOT STD_x_UNDER_ROOT,              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   62 */  { fnCubeRoot,                   NOPARAM,                     STD_CUBE_ROOT STD_x_UNDER_ROOT,                STD_CUBE_ROOT STD_x_UNDER_ROOT,                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   63 */  { fnXthRoot,                    NOPARAM,                     STD_xTH_ROOT STD_y_UNDER_ROOT,                 STD_xTH_ROOT STD_y_UNDER_ROOT,                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   64 */  { fn2Pow,                       NOPARAM,                     "2" STD_SUP_x,                                 "2" STD_SUP_x,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   65 */  { fnExp,                        NOPARAM,                     "e" STD_SUP_x,                                 "e" STD_SUP_x,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   66 */  { fnRound,                      NOPARAM,                     "ROUND",                                       "ROUND",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   67 */  { fn10Pow,                      NOPARAM,                     "10" STD_SUP_x,                                "10" STD_SUP_x,                                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   68 */  { fnLog2,                       NOPARAM,                     "LOG" STD_SUB_2,                               "lb x",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   69 */  { fnLn,                         NOPARAM,                     "LN",                                          "ln",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   70 */  { itemToBeCoded,                NOPARAM,                     "STOP",                                        "R/S",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   71 */  { fnLog10,                      NOPARAM,                     "LOG" STD_SUB_1 STD_SUB_0,                     "lg",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   72 */  { fnLogXY,                      NOPARAM,                     "LOG" STD_SUB_x "y",                           "log" STD_SUB_x "y",                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   73 */  { fnInvert,                     NOPARAM,                     "1/x",                                         "1/x",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   74 */  { fnCos,                        NOPARAM,                     "cos",                                         "cos",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   75 */  { fnCosh,                       NOPARAM,                     "cosh",                                        "cosh",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   76 */  { fnSin,                        NOPARAM,                     "sin",                                         "sin",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   77 */  { itemToBeCoded,                NOPARAM,                     "KEY?",                                        "KEY?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   78 */  { fnSinh,                       NOPARAM,                     "sinh",                                        "sinh",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   79 */  { fnTan,                        NOPARAM,                     "tan",                                         "tan",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   80 */  { fnTanh,                       NOPARAM,                     "tanh",                                        "tanh",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   81 */  { fnArccos,                     NOPARAM,                     "arccos",                                      "arccos",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   82 */  { fnArccosh,                    NOPARAM,                     "arcosh",                                      "arcosh",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   83 */  { fnArcsin,                     NOPARAM,                     "arcsin",                                      "arcsin",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   84 */  { fnArcsinh,                    NOPARAM,                     "arsinh",                                      "arsinh",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   85 */  { fnArctan,                     NOPARAM,                     "arctan",                                      "arctan",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   86 */  { fnArctanh,                    NOPARAM,                     "artanh",                                      "artanh",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   87 */  { fnCeil,                       NOPARAM,                     "CEIL",                                        "CEIL",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   88 */  { fnFloor,                      NOPARAM,                     "FLOOR",                                       "FLOOR",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   89 */  { fnGcd,                        NOPARAM,                     "GCD",                                         "GCD",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   90 */  { fnLcm,                        NOPARAM,                     "LCM",                                         "LCM",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   91 */  { fnDec,                        TM_REGISTER,                 "DEC",                                         "DEC",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   92 */  { fnInc,                        TM_REGISTER,                 "INC",                                         "INC",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   93 */  { fnIp,                         NOPARAM,                     "IP",                                          "IP",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   94 */  { fnFp,                         NOPARAM,                     "FP",                                          "FP",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   95 */  { fnAdd,                        ITM_ADD,                     "+",                                           "+",                                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   96 */  { fnSubtract,                   ITM_SUB,                     "-",                                           "-",                                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   97 */  { fnChangeSign,                 ITM_CHS,                     "+/-",                                         "+/-",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   98 */  { fnMultiply,                   ITM_MULT,                    STD_CROSS,                                     STD_CROSS,                                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*   99 */  { fnDivide,                     ITM_DIV,                     "/",                                           "/",                                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  100 */  { fnIDiv,                       NOPARAM,                     "IDIV",                                        "IDIV",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  101 */  { itemToBeCoded,                NOPARAM,                     "VIEW",                                        "VIEW",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  102 */  { fnMod,                        NOPARAM,                     "MOD",                                         "MOD",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  103 */  { fnMax,                        NOPARAM,                     "max",                                         "max",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  104 */  { fnMin,                        NOPARAM,                     "min",                                         "min",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  105 */  { fnMagnitude,                  NOPARAM,                     "|x|",                                         "|x|",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  106 */  { fnNeighb,                     NOPARAM,                     "NEIGHB",                                      "NEIGHB",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  107 */  { fnNextPrime,                  NOPARAM,                     "NEXTP",                                       "NEXTP",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  108 */  { fnFactorial,                  NOPARAM,                     "x!",                                          "x!",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  109 */  { fnPi,                         NOPARAM,                     STD_pi,                                        STD_pi,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED  },
/*  110 */  { fnClearFlag,                  TM_FLAGW,                    "CF",                                          "CF",                                          (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  111 */  { fnSetFlag,                    TM_FLAGW,                    "SF",                                          "SF",                                          (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  112 */  { fnFlipFlag,                   TM_FLAGW,                    "FF",                                          "FF",                                          (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  113 */  { fnCheckValue,                 CHECK_VALUE_MATRIX_SQUARE,   "M.SQR?",                                      "M.SQR?",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  114 */  { itemToBeCoded,                NOPARAM,                     "LITE",                                        "LITE",                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED}, // Literal in a PGM
/*  115 */  { fnCvtFromCurrentAngularMode,  amDegree,                    STD_RIGHT_ARROW "DEG",                         STD_RIGHT_ARROW "DEG",                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  116 */  { fnCvtFromCurrentAngularMode,  amDMS,                       STD_RIGHT_ARROW "D.MS",                        STD_RIGHT_ARROW "D.MS",                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  117 */  { fnCvtFromCurrentAngularMode,  amGrad,                      STD_RIGHT_ARROW "GRAD",                        STD_RIGHT_ARROW "GRAD",                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  118 */  { fnCvtFromCurrentAngularMode,  amMultPi,                    STD_RIGHT_ARROW "MUL" STD_pi,                  STD_RIGHT_ARROW "MUL" STD_pi,                  (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  119 */  { fnCvtFromCurrentAngularMode,  amRadian,                    STD_RIGHT_ARROW "RAD",                         STD_RIGHT_ARROW "RAD",                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  120 */  { fnCvtDegToRad,                NOPARAM,                     "D" STD_RIGHT_ARROW "R",                       "D" STD_RIGHT_ARROW "R",                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  121 */  { fnCvtRadToDeg,                NOPARAM,                     "R" STD_RIGHT_ARROW "D",                       "R" STD_RIGHT_ARROW "D",                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  122 */  { fnRmd,                        NOPARAM,                     "RMD",                                         "RMD",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  123 */  { fnLogicalNot,                 NOPARAM,                     "NOT",                                         "NOT",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  124 */  { fnLogicalAnd,                 NOPARAM,                     "AND",                                         "AND",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  125 */  { fnLogicalOr,                  NOPARAM,                     "OR",                                          "OR",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  126 */  { fnLogicalXor,                 NOPARAM,                     "XOR",                                         "XOR",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  127 */  { fnSwapX,                      TM_REGISTER,                 "x" STD_LEFT_RIGHT_ARROWS,                     "x" STD_LEFT_RIGHT_ARROWS,                     (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },


// Items from 128 to ... are 2 byte OP codes
// Constants
/*  128 */  { fnConstant,                   0,                           "a",                                           "a",                                           (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  129 */  { fnConstant,                   1,                           "a" STD_SUB_0,                                 "a" STD_SUB_0,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  130 */  { fnConstant,                   2,                           "a" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "a" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  131 */  { fnConstant,                   3,                           "a" STD_SUB_EARTH,                             "a" STD_SUB_EARTH,                             (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  132 */  { fnConstant,                   4,                           "c",                                           "c",                                           (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  133 */  { fnConstant,                   5,                           "c" STD_SUB_1,                                 "c" STD_SUB_1,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  134 */  { fnConstant,                   6,                           "c" STD_SUB_2,                                 "c" STD_SUB_2,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  135 */  { fnConstant,                   7,                           "e",                                           "e",                                           (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  136 */  { fnConstant,                   8,                           "e" STD_SUB_E,                                 "e" STD_SUB_E,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  137 */  { fnConstant,                   9,                           "F",                                           "F",                                           (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  138 */  { fnConstant,                   10,                          "F" STD_SUB_alpha,                             "F" STD_SUB_alpha,                             (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  139 */  { fnConstant,                   11,                          "F" STD_SUB_delta,                             "F" STD_SUB_delta,                             (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  140 */  { fnConstant,                   12,                          "G",                                           "G",                                           (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  141 */  { fnConstant,                   13,                          "G" STD_SUB_0,                                 "G" STD_SUB_0,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  142 */  { fnConstant,                   14,                          "G" STD_SUB_C,                                 "G" STD_SUB_C,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  143 */  { fnConstant,                   15,                          "g" STD_SUB_e,                                 "g" STD_SUB_e,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  144 */  { fnConstant,                   16,                          "GM" STD_SUB_EARTH,                            "GM" STD_SUB_EARTH,                            (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  145 */  { fnConstant,                   17,                          "g" STD_SUB_EARTH,                             "g" STD_SUB_EARTH,                             (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  146 */  { fnConstant,                   18,                          STD_PLANCK,                                    STD_PLANCK,                                    (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  147 */  { fnConstant,                   19,                          STD_PLANCK_2PI,                                STD_PLANCK_2PI,                                (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  148 */  { fnConstant,                   20,                          "k",                                           "k",                                           (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  149 */  { fnConstant,                   21,                          "K" STD_SUB_J,                                 "K" STD_SUB_J,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  150 */  { fnConstant,                   22,                          "l" STD_SUB_P STD_SUB_L,                       "l" STD_SUB_P STD_SUB_L,                       (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  151 */  { fnConstant,                   23,                          "m" STD_SUB_e,                                 "m" STD_SUB_e,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  152 */  { fnConstant,                   24,                          "M" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "M" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  153 */  { fnConstant,                   25,                          "m" STD_SUB_n,                                 "m" STD_SUB_n,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  154 */  { fnConstant,                   26,                          "m" STD_SUB_n "/m" STD_SUB_p,                  "m" STD_SUB_n "/m" STD_SUB_p,                  (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  155 */  { fnConstant,                   27,                          "m" STD_SUB_p,                                 "m" STD_SUB_p,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  156 */  { fnConstant,                   28,                          "m" STD_SUB_P STD_SUB_L,                       "m" STD_SUB_P STD_SUB_L,                       (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  157 */  { fnConstant,                   29,                          "m" STD_SUB_p "/m" STD_SUB_e,                  "m" STD_SUB_p "/m" STD_SUB_e,                  (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  158 */  { fnConstant,                   30,                          "m" STD_SUB_u,                                 "m" STD_SUB_u,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  159 */  { fnConstant,                   31,                          "m" STD_SUB_u "c" STD_SUP_2,                   "m" STD_SUB_u "c" STD_SUP_2,                   (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  160 */  { fnConstant,                   32,                          "m" STD_SUB_mu,                                "m" STD_SUB_mu,                                (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  161 */  { fnConstant,                   33,                          "M" STD_SUB_SUN,                               "M" STD_SUB_SUN,                               (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  162 */  { fnConstant,                   34,                          "M" STD_SUB_EARTH,                             "M" STD_SUB_EARTH,                             (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  163 */  { fnConstant,                   35,                          "N" STD_SUB_A,                                 "N" STD_SUB_A,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  164 */  { fnConstant,                   36,                          "NaN",                                         "NaN",                                         (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  165 */  { fnConstant,                   37,                          "p" STD_SUB_0,                                 "p" STD_SUB_0,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  166 */  { fnConstant,                   38,                          "R",                                           "R",                                           (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  167 */  { fnConstant,                   39,                          "r" STD_SUB_e,                                 "r" STD_SUB_e,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  168 */  { fnConstant,                   40,                          "R" STD_SUB_K,                                 "R" STD_SUB_K,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  169 */  { fnConstant,                   41,                          "R" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "R" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  170 */  { fnConstant,                   42,                          "R" STD_SUB_INFINITY,                          "R" STD_SUB_INFINITY,                          (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  171 */  { fnConstant,                   43,                          "R" STD_SUB_SUN,                               "R" STD_SUB_SUN,                               (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  172 */  { fnConstant,                   44,                          "R" STD_SUB_EARTH,                             "R" STD_SUB_EARTH,                             (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  173 */  { fnConstant,                   45,                          "Sa",                                          "Sa",                                          (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  174 */  { fnConstant,                   46,                          "Sb",                                          "Sb",                                          (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  175 */  { fnConstant,                   47,                          "Se" STD_SUP_2,                                "Se" STD_SUP_2,                                (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  176 */  { fnConstant,                   48,                          "Se'" STD_SUP_2,                               "Se'" STD_SUP_2,                               (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  177 */  { fnConstant,                   49,                          "Sf" STD_SUP_MINUS_1,                          "Sf" STD_SUP_MINUS_1,                          (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  178 */  { fnConstant,                   50,                          "T" STD_SUB_0,                                 "T" STD_SUB_0,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  179 */  { fnConstant,                   51,                          "T" STD_SUB_p,                                 "T" STD_SUB_P,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  180 */  { fnConstant,                   52,                          "t" STD_SUB_P STD_SUB_L,                       "t" STD_SUB_P STD_SUB_L,                       (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  181 */  { fnConstant,                   53,                          "V" STD_SUB_m,                                 "V" STD_SUB_m,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  182 */  { fnConstant,                   54,                          "Z" STD_SUB_0,                                 "Z" STD_SUB_0,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  183 */  { fnConstant,                   55,                          STD_alpha,                                     STD_alpha,                                     (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  184 */  { fnConstant,                   56,                          STD_gamma,                                     STD_gamma,                                     (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  185 */  { fnConstant,                   57,                          STD_gamma STD_SUB_E STD_SUB_M,                 STD_gamma STD_SUB_E STD_SUB_M,                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  186 */  { fnConstant,                   58,                          STD_gamma STD_SUB_p,                           STD_gamma STD_SUB_p,                           (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  187 */  { fnConstant,                   59,                          STD_DELTA STD_nu STD_SUB_C STD_SUB_s,          STD_DELTA STD_nu STD_SUB_C STD_SUB_s,          (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  188 */  { fnConstant,                   60,                          STD_epsilon STD_SUB_0,                         STD_epsilon STD_SUB_0,                         (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  189 */  { fnConstant,                   61,                          STD_lambda STD_SUB_C,                          STD_lambda STD_SUB_C,                          (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  190 */  { fnConstant,                   62,                          STD_lambda STD_SUB_C STD_SUB_n,                STD_lambda STD_SUB_C STD_SUB_n,                (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  191 */  { fnConstant,                   63,                          STD_lambda STD_SUB_C STD_SUB_p,                STD_lambda STD_SUB_C STD_SUB_p,                (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  192 */  { fnConstant,                   64,                          STD_mu STD_SUB_0,                              STD_mu STD_SUB_0,                              (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  193 */  { fnConstant,                   65,                          STD_mu STD_SUB_B,                              STD_mu STD_SUB_B,                              (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  194 */  { fnConstant,                   66,                          STD_mu STD_SUB_e,                              STD_mu STD_SUB_e,                              (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  195 */  { fnConstant,                   67,                          STD_mu STD_SUB_e "/" STD_mu STD_SUB_B,         STD_mu STD_SUB_e "/" STD_mu STD_SUB_B,         (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  196 */  { fnConstant,                   68,                          STD_mu STD_SUB_n,                              STD_mu STD_SUB_n,                              (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  197 */  { fnConstant,                   69,                          STD_mu STD_SUB_p,                              STD_mu STD_SUB_p,                              (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  198 */  { fnConstant,                   70,                          STD_mu STD_SUB_u,                              STD_mu STD_SUB_u,                              (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  199 */  { fnConstant,                   71,                          STD_mu STD_SUB_mu,                             STD_mu STD_SUB_mu,                             (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  200 */  { fnConstant,                   72,                          STD_sigma STD_SUB_B,                           STD_sigma STD_SUB_B,                           (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  201 */  { fnConstant,                   73,                          STD_PHI,                                       STD_PHI,                                       (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  202 */  { fnConstant,                   74,                          STD_PHI STD_SUB_0,                             STD_PHI STD_SUB_0,                             (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  203 */  { fnConstant,                   75,                          STD_omega,                                     STD_omega,                                     (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  204 */  { fnConstant,                   76,                          "-" STD_INFINITY,                              "-" STD_INFINITY,                              (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  205 */  { fnConstant,                   77,                          STD_INFINITY,                                  STD_INFINITY,                                  (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED  },
/*  206 */  { itemToBeCoded,                78,                          "#",                                           "#",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED},
/*  207 */  { fnConstant,                   TM_VALUE,                    "CNST",                                        "CNST",                                        (0 << TAM_MAX_BITS) |   215, CAT_FNCT | SLS_ENABLED   | US_ENABLED  }, // 215 is replaced at run time by NUMBER_OF_CONSTANTS_39 + NUMBER_OF_CONSTANTS_51 + NUMBER_OF_CONSTANTS_1071 + NUMBER_OF_CONSTANTS_34 - 1
/*  208 */  { itemToBeCoded,                NOPARAM,                     "0208",                                        "0208",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  209 */  { itemToBeCoded,                NOPARAM,                     "0209",                                        "0209",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  210 */  { itemToBeCoded,                NOPARAM,                     "0210",                                        "0210",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  211 */  { itemToBeCoded,                NOPARAM,                     "0211",                                        "0211",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  212 */  { itemToBeCoded,                NOPARAM,                     "0212",                                        "0212",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  213 */  { itemToBeCoded,                NOPARAM,                     "0213",                                        "0213",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  214 */  { itemToBeCoded,                NOPARAM,                     "0214",                                        "0214",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  215 */  { itemToBeCoded,                NOPARAM,                     "0215",                                        "0215",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  216 */  { itemToBeCoded,                NOPARAM,                     "0216",                                        "0216",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  217 */  { itemToBeCoded,                NOPARAM,                     "0217",                                        "0217",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  218 */  { itemToBeCoded,                NOPARAM,                     "0218",                                        "0218",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  219 */  { itemToBeCoded,                NOPARAM,                     "0219",                                        "0219",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},


// Conversions
/*  220 */  { fnCvtCToF,                    NOPARAM,                     STD_DEGREE "C" STD_RIGHT_ARROW STD_DEGREE "F", STD_DEGREE "C" STD_RIGHT_ARROW STD_DEGREE "F", (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  221 */  { fnCvtFToC,                    NOPARAM,                     STD_DEGREE "F" STD_RIGHT_ARROW STD_DEGREE "C", STD_DEGREE "F" STD_RIGHT_ARROW STD_DEGREE "C", (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  222 */  { fnCvtDbRatio,                 10,                          "dB" STD_RIGHT_ARROW "pr",                     "dB " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  223 */  { fnCvtDbRatio,                 10,                          "dB" STD_RIGHT_ARROW "pr",                     "power",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  224 */  { fnCvtDbRatio,                 10,                          "dB" STD_RIGHT_ARROW "pr",                     "ratio",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  225 */  { fnCvtDbRatio,                 20,                          "dB" STD_RIGHT_ARROW "fr",                     "dB " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  226 */  { fnCvtDbRatio,                 20,                          "dB" STD_RIGHT_ARROW "fr",                     "field",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  227 */  { fnCvtDbRatio,                 20,                          "dB" STD_RIGHT_ARROW "fr",                     "ratio",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  228 */  { fnCvtRatioDb,                 10,                          "pr" STD_RIGHT_ARROW "dB",                     "power",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  229 */  { fnCvtRatioDb,                 10,                          "pr" STD_RIGHT_ARROW "dB",                     "ratio",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  230 */  { fnCvtRatioDb,                 10,                          "pr" STD_RIGHT_ARROW "dB",                     STD_RIGHT_ARROW " dB",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  231 */  { fnCvtRatioDb,                 20,                          "fr" STD_RIGHT_ARROW "dB",                     "field",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  232 */  { fnCvtRatioDb,                 20,                          "fr" STD_RIGHT_ARROW "dB",                     "ratio",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  233 */  { fnCvtRatioDb,                 20,                          "fr" STD_RIGHT_ARROW "dB",                     STD_RIGHT_ARROW " dB",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  234 */  { fnCvtAcreHa,                  multiply,                    "ac" STD_RIGHT_ARROW "ha",                     "acre",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  235 */  { fnCvtAcreHa,                  multiply,                    "ac" STD_RIGHT_ARROW "ha",                     STD_RIGHT_ARROW " ha",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  236 */  { fnCvtAcreHa,                  divide,                      "ha" STD_RIGHT_ARROW "ac",                     "ha " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  237 */  { fnCvtAcreHa,                  divide,                      "ha" STD_RIGHT_ARROW "ac",                     "acre",                                        (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  238 */  { fnCvtAcreusHa,                multiply,                    "ac" STD_US STD_RIGHT_ARROW "ha",              "acre" STD_US,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  239 */  { fnCvtAcreusHa,                multiply,                    "ac" STD_US STD_RIGHT_ARROW "ha",              STD_RIGHT_ARROW " ha",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  240 */  { fnCvtAcreusHa,                divide,                      "ha" STD_RIGHT_ARROW "ac" STD_US,              "ha " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  241 */  { fnCvtAcreusHa,                divide,                      "ha" STD_RIGHT_ARROW "ac" STD_US,              "acre" STD_US,                                 (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  242 */  { fnCvtAtmPa,                   divide,                      "Pa" STD_RIGHT_ARROW "atm",                    "Pa" STD_RIGHT_ARROW "atm",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  243 */  { fnCvtAtmPa,                   multiply,                    "atm" STD_RIGHT_ARROW "Pa",                    "atm" STD_RIGHT_ARROW "Pa",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  244 */  { fnCvtAuM,                     multiply,                    "au" STD_RIGHT_ARROW "m",                      "au" STD_RIGHT_ARROW "m",                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  245 */  { fnCvtAuM,                     divide,                      "m" STD_RIGHT_ARROW "au",                      "m" STD_RIGHT_ARROW "au",                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  246 */  { fnCvtBarPa,                   divide,                      "Pa" STD_RIGHT_ARROW "bar",                    "Pa" STD_RIGHT_ARROW "bar",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  247 */  { fnCvtBarPa,                   multiply,                    "bar" STD_RIGHT_ARROW "Pa",                    "bar" STD_RIGHT_ARROW "Pa",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  248 */  { fnCvtBtuJ,                    multiply,                    "Btu" STD_RIGHT_ARROW "J",                     "Btu" STD_RIGHT_ARROW "J",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  249 */  { fnCvtBtuJ,                    divide,                      "J" STD_RIGHT_ARROW "Btu",                     "J" STD_RIGHT_ARROW "Btu",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  250 */  { fnCvtCalJ,                    multiply,                    "cal" STD_RIGHT_ARROW "J",                     "cal" STD_RIGHT_ARROW "J",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  251 */  { fnCvtCalJ,                    divide,                      "J" STD_RIGHT_ARROW "cal",                     "J" STD_RIGHT_ARROW "cal",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  252 */  { fnCvtLbfftNm,                 multiply,                    "lbft" STD_RIGHT_SHORT_ARROW "Nm",             "lbf" STD_DOT "ft",                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  253 */  { fnCvtLbfftNm,                 multiply,                    "lbft" STD_RIGHT_SHORT_ARROW "Nm",             STD_RIGHT_ARROW " Nm",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  254 */  { fnCvtLbfftNm,                 divide,                      "Nm" STD_RIGHT_SHORT_ARROW "lbft",             "Nm " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  255 */  { fnCvtLbfftNm,                 divide,                      "Nm" STD_RIGHT_SHORT_ARROW "lbft",             "lbf" STD_DOT "ft",                            (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  256 */  { fnCvtCwtKg,                   multiply,                    "cwt" STD_RIGHT_ARROW "kg",                    "cwt" STD_RIGHT_ARROW "kg",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  257 */  { fnCvtCwtKg,                   divide,                      "kg" STD_RIGHT_ARROW "cwt",                    "kg" STD_RIGHT_ARROW "cwt",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  258 */  { fnCvtFtM,                     multiply,                    "ft." STD_RIGHT_ARROW "m",                     "ft." STD_RIGHT_ARROW "m",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  259 */  { fnCvtFtM,                     divide,                      "m" STD_RIGHT_ARROW "ft.",                     "m" STD_RIGHT_ARROW "ft.",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  260 */  { fnCvtSfeetM,                  multiply,                    "ft" STD_US STD_RIGHT_ARROW "m",               "survey",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  261 */  { fnCvtSfeetM,                  multiply,                    "ft" STD_US STD_RIGHT_ARROW "m",               "foot" STD_US,                                 (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  262 */  { fnCvtSfeetM,                  multiply,                    "ft" STD_US STD_RIGHT_ARROW "m",               STD_RIGHT_ARROW " m",                          (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  263 */  { fnCvtSfeetM,                  divide,                      "m" STD_RIGHT_ARROW "ft" STD_US,               "m " STD_RIGHT_ARROW,                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  264 */  { fnCvtSfeetM,                  divide,                      "m" STD_RIGHT_ARROW "ft" STD_US,               "survey",                                      (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  265 */  { fnCvtSfeetM,                  divide,                      "m" STD_RIGHT_ARROW "ft" STD_US,               "foot" STD_US,                                 (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  266 */  { fnCvtFlozukMl,                multiply,                    "fz" STD_UK STD_RIGHT_ARROW "ml",              "floz" STD_UK,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  267 */  { fnCvtFlozukMl,                multiply,                    "fz" STD_UK STD_RIGHT_ARROW "ml",              STD_RIGHT_ARROW " ml",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  268 */  { fnCvtFlozukMl,                divide,                      "ml" STD_RIGHT_ARROW "fz" STD_UK,              "ml " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  269 */  { fnCvtFlozukMl,                divide,                      "ml" STD_RIGHT_ARROW "fz" STD_UK,              "floz" STD_UK,                                 (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  270 */  { fnCvtFlozusMl,                multiply,                    "fz" STD_US STD_RIGHT_ARROW "ml",              "floz" STD_US,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  271 */  { fnCvtFlozusMl,                multiply,                    "fz" STD_US STD_RIGHT_ARROW "ml",              STD_RIGHT_ARROW " ml",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  272 */  { fnCvtFlozusMl,                divide,                      "ml" STD_RIGHT_ARROW "fz" STD_US,              "ml " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  273 */  { fnCvtFlozusMl,                divide,                      "ml" STD_RIGHT_ARROW "fz" STD_US,              "floz" STD_US,                                 (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  274 */  { fnCvtGalukL,                  multiply,                    "gal" STD_UK STD_RIGHT_ARROW "l",              "gal" STD_UK STD_RIGHT_ARROW "l",              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  275 */  { fnCvtGalukL,                  divide,                      "l" STD_RIGHT_ARROW "gal" STD_UK,              "l" STD_RIGHT_ARROW "gal" STD_UK,              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  276 */  { fnCvtGalusL,                  multiply,                    "gal" STD_US STD_RIGHT_ARROW "l",              "gal" STD_US STD_RIGHT_ARROW "l",              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  277 */  { fnCvtGalusL,                  divide,                      "l" STD_RIGHT_ARROW "gal" STD_US,              "l" STD_RIGHT_ARROW "gal" STD_US,              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  278 */  { fnCvtHpeW,                    multiply,                    "hp" STD_SUB_E STD_RIGHT_ARROW "W",            "hp" STD_SUB_E STD_RIGHT_ARROW "W",            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  279 */  { fnCvtHpeW,                    divide,                      "W" STD_RIGHT_ARROW "hp" STD_SUB_E,            "W" STD_RIGHT_ARROW "hp" STD_SUB_E,            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  280 */  { fnCvtHpmW,                    multiply,                    "hp" STD_SUB_M STD_RIGHT_ARROW "W",            "hp" STD_SUB_M STD_RIGHT_ARROW "W",            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  281 */  { fnCvtHpmW,                    divide,                      "W" STD_RIGHT_ARROW "hp" STD_SUB_M,            "W" STD_RIGHT_ARROW "hp" STD_SUB_M,            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  282 */  { fnCvtHpukW,                   multiply,                    "hp" STD_UK STD_RIGHT_ARROW "W",               "hp" STD_UK STD_RIGHT_ARROW "W",               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  283 */  { fnCvtHpukW,                   divide,                      "W" STD_RIGHT_ARROW "hp" STD_UK,               "W" STD_RIGHT_ARROW "hp" STD_UK,               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  284 */  { fnCvtInhgPa,                  multiply,                    "iHg" STD_RIGHT_ARROW "Pa",                    "in.Hg",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  285 */  { fnCvtInhgPa,                  multiply,                    "iHg" STD_RIGHT_ARROW "Pa",                    STD_RIGHT_ARROW " Pa",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  286 */  { fnCvtInhgPa,                  divide,                      "Pa" STD_RIGHT_ARROW "iHg",                    "Pa " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  287 */  { fnCvtInhgPa,                  divide,                      "Pa" STD_RIGHT_ARROW "iHg",                    "in.Hg",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  288 */  { fnCvtInchMm,                  multiply,                    "in." STD_RIGHT_ARROW "mm",                    "in." STD_RIGHT_ARROW "mm",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  289 */  { fnCvtInchMm,                  divide,                      "mm" STD_RIGHT_ARROW "in.",                    "mm" STD_RIGHT_ARROW "in.",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  290 */  { fnCvtWhJ,                     multiply,                    "Wh" STD_RIGHT_ARROW "J",                      "Wh" STD_RIGHT_ARROW "J",                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  291 */  { fnCvtWhJ,                     divide,                      "J" STD_RIGHT_ARROW "Wh",                      "J" STD_RIGHT_ARROW "Wh",                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  292 */  { fnCvtLbKg,                    divide,                      "kg" STD_RIGHT_ARROW "lb.",                    "kg" STD_RIGHT_ARROW "lb.",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  293 */  { fnCvtLbKg,                    multiply,                    "lb." STD_RIGHT_ARROW "kg",                    "lb." STD_RIGHT_ARROW "kg",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  294 */  { fnCvtOzG,                     divide,                      "g" STD_RIGHT_ARROW "oz",                      "g" STD_RIGHT_ARROW "oz",                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  295 */  { fnCvtOzG,                     multiply,                    "oz" STD_RIGHT_ARROW "g",                      "oz" STD_RIGHT_ARROW "g",                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  296 */  { fnCvtShortcwtKg,              divide,                      "kg" STD_RIGHT_ARROW "scw",                    "kg " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  297 */  { fnCvtShortcwtKg,              divide,                      "kg" STD_RIGHT_ARROW "scw",                    "sh.cwt",                                      (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  298 */  { fnCvtShortcwtKg,              multiply,                    "scw" STD_RIGHT_ARROW "kg",                    "short",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  299 */  { fnCvtShortcwtKg,              multiply,                    "scw" STD_RIGHT_ARROW "kg",                    "cwt" STD_RIGHT_ARROW "kg",                    (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  300 */  { fnCvtStoneKg,                 divide,                      "kg" STD_RIGHT_ARROW "sto",                    "kg " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  301 */  { fnCvtStoneKg,                 divide,                      "kg" STD_RIGHT_ARROW "sto",                    "stone",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  302 */  { fnCvtStoneKg,                 multiply,                    "sto" STD_RIGHT_ARROW "kg",                    "stone",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  303 */  { fnCvtStoneKg,                 multiply,                    "sto" STD_RIGHT_ARROW "kg",                    STD_RIGHT_ARROW " kg",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  304 */  { fnCvtShorttonKg,              divide,                      "kg" STD_RIGHT_ARROW "s.t",                    "kg " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  305 */  { fnCvtShorttonKg,              divide,                      "kg" STD_RIGHT_ARROW "s.t",                    "short",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  306 */  { fnCvtShorttonKg,              divide,                      "kg" STD_RIGHT_ARROW "s.t",                    "ton",                                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  307 */  { fnCvtShorttonKg,              multiply,                    "s.t" STD_RIGHT_ARROW "kg",                    "short",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  308 */  { fnCvtShorttonKg,              multiply,                    "s.t" STD_RIGHT_ARROW "kg",                    "ton",                                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  309 */  { fnCvtShorttonKg,              multiply,                    "s.t" STD_RIGHT_ARROW "kg",                    STD_RIGHT_ARROW " kg",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  310 */  { fnCvtTonKg,                   divide,                      "kg" STD_RIGHT_ARROW "ton",                    "kg" STD_RIGHT_ARROW "ton",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  311 */  { fnCvtLiangKg,                 multiply,                    "kg" STD_RIGHT_ARROW "lian",                   "kg " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  312 */  { fnCvtLiangKg,                 multiply,                    "kg" STD_RIGHT_ARROW "lian",                   "li" STD_a_BREVE "ng",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  313 */  { fnCvtTonKg,                   multiply,                    "ton" STD_RIGHT_ARROW "kg",                    "ton" STD_RIGHT_ARROW "kg",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  314 */  { fnCvtLiangKg,                 divide,                      "lian" STD_RIGHT_ARROW "kg",                   "li" STD_a_BREVE "ng",                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  315 */  { fnCvtLiangKg,                 divide,                      "lian" STD_RIGHT_ARROW "kg",                   STD_RIGHT_ARROW " kg",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  316 */  { fnCvtTrozG,                   divide,                      "g" STD_RIGHT_ARROW "trz",                     "g " STD_RIGHT_ARROW,                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  317 */  { fnCvtTrozG,                   divide,                      "g" STD_RIGHT_ARROW "trz",                     "tr.oz",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  318 */  { fnCvtTrozG,                   multiply,                    "trz" STD_RIGHT_ARROW "g",                     "tr.oz",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  319 */  { fnCvtTrozG,                   multiply,                    "trz" STD_RIGHT_ARROW "g",                     STD_RIGHT_ARROW " g",                          (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  320 */  { fnCvtLbfN,                    multiply,                    "lbf" STD_RIGHT_ARROW "N",                     "lbf" STD_RIGHT_ARROW "N",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  321 */  { fnCvtLbfN,                    divide,                      "N" STD_RIGHT_ARROW "lbf",                     "N" STD_RIGHT_ARROW "lbf",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  322 */  { fnCvtLyM,                     multiply,                    "l.y." STD_RIGHT_ARROW "m",                    "l.y." STD_RIGHT_ARROW "m",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  323 */  { fnCvtLyM,                     divide,                      "m" STD_RIGHT_ARROW "l.y.",                    "m" STD_RIGHT_ARROW "l.y.",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  324 */  { fnCvtMmhgPa,                  multiply,                    "mmH" STD_RIGHT_SHORT_ARROW "Pa",              "mm.Hg",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  325 */  { fnCvtMmhgPa,                  multiply,                    "mmH" STD_RIGHT_SHORT_ARROW "Pa",              STD_RIGHT_ARROW " Pa",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  326 */  { fnCvtMmhgPa,                  divide,                      "Pa" STD_RIGHT_SHORT_ARROW "mmH",              "Pa " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  327 */  { fnCvtMmhgPa,                  divide,                      "Pa" STD_RIGHT_SHORT_ARROW "mmH",              "mm.Hg",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  328 */  { fnCvtMiKm,                    multiply,                    "mi." STD_RIGHT_ARROW "km",                    "mi." STD_RIGHT_ARROW "km",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  329 */  { fnCvtMiKm,                    divide,                      "km" STD_RIGHT_ARROW "mi.",                    "km" STD_RIGHT_ARROW "mi.",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  330 */  { fnCvtNmiKm,                   divide,                      "km" STD_RIGHT_ARROW "nmi",                    "km" STD_RIGHT_ARROW "nmi",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  331 */  { fnCvtNmiKm,                   multiply,                    "nmi" STD_RIGHT_ARROW "km",                    "nmi" STD_RIGHT_ARROW "km",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  332 */  { fnCvtPcM,                     divide,                      "m" STD_RIGHT_ARROW "pc",                      "m" STD_RIGHT_ARROW "pc",                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  333 */  { fnCvtPcM,                     multiply,                    "pc" STD_RIGHT_ARROW "m",                      "pc" STD_RIGHT_ARROW "m",                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  334 */  { fnCvtPointMm,                 divide,                      "mm" STD_RIGHT_ARROW "pt.",                    "mm " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  335 */  { fnCvtPointMm,                 divide,                      "mm" STD_RIGHT_ARROW "pt.",                    "point",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  336 */  { fnCvtPointMm,                 divide,                      "mm" STD_RIGHT_ARROW "pt.",                    STD_SPACE_HAIR,                                (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  337 */  { fnCvtPointMm,                 multiply,                    "pt." STD_RIGHT_ARROW "mm",                    "point",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  338 */  { fnCvtPointMm,                 multiply,                    "pt." STD_RIGHT_ARROW "mm",                    STD_RIGHT_ARROW " mm",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  339 */  { fnCvtPointMm,                 multiply,                    "pt." STD_RIGHT_ARROW "mm",                    STD_SPACE_HAIR,                                (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  340 */  { fnCvtYardM,                   divide,                      "m" STD_RIGHT_ARROW "yd.",                     "m" STD_RIGHT_ARROW "yd.",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  341 */  { fnCvtYardM,                   multiply,                    "yd." STD_RIGHT_ARROW "m",                     "yd." STD_RIGHT_ARROW "m",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  342 */  { fnCvtPsiPa,                   multiply,                    "psi" STD_RIGHT_ARROW "Pa",                    "psi" STD_RIGHT_ARROW "Pa",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  343 */  { fnCvtPsiPa,                   divide,                      "Pa" STD_RIGHT_ARROW "psi",                    "Pa" STD_RIGHT_ARROW "psi",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  344 */  { fnCvtTorrPa,                  divide,                      "Pa" STD_RIGHT_ARROW "tor",                    "Pa " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  345 */  { fnCvtTorrPa,                  divide,                      "Pa" STD_RIGHT_ARROW "tor",                    "torr",                                        (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  346 */  { fnCvtTorrPa,                  multiply,                    "tor" STD_RIGHT_ARROW "Pa",                    "torr",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  347 */  { fnCvtTorrPa,                  multiply,                    "tor" STD_RIGHT_ARROW "Pa",                    STD_RIGHT_ARROW " Pa",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  348 */  { fnCvtYearS,                   divide,                      "s" STD_RIGHT_ARROW "year",                    "s" STD_RIGHT_ARROW "year",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  349 */  { fnCvtYearS,                   multiply,                    "year" STD_RIGHT_ARROW "s",                    "year" STD_RIGHT_ARROW "s",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  350 */  { fnCvtCaratG,                  multiply,                    "ct" STD_RIGHT_ARROW "g",                      "carat",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  351 */  { fnCvtCaratG,                  multiply,                    "ct" STD_RIGHT_ARROW "g",                      STD_RIGHT_ARROW " g",                          (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  352 */  { fnCvtJinKg,                   divide,                      "j" STD_i_MACRON "n" STD_RIGHT_ARROW "kg",     "j" STD_i_MACRON "n" STD_RIGHT_ARROW "kg",     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  353 */  { fnCvtCaratG,                  divide,                      "g" STD_RIGHT_ARROW "ct",                      "g " STD_RIGHT_ARROW,                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  354 */  { fnCvtCaratG,                  divide,                      "g" STD_RIGHT_ARROW "ct",                      "carat",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  355 */  { fnCvtJinKg,                   multiply,                    "kg" STD_RIGHT_ARROW "j" STD_i_MACRON "n",     "kg" STD_RIGHT_ARROW "j" STD_i_MACRON "n",     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  356 */  { fnCvtQuartL,                  multiply,                    "qt." STD_RIGHT_ARROW "l",                     "qt." STD_RIGHT_ARROW "l",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  357 */  { fnCvtQuartL,                  divide,                      "l" STD_RIGHT_ARROW "qt.",                     "l" STD_RIGHT_ARROW "qt.",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  358 */  { fnCvtFathomM,                 multiply,                    "fm." STD_RIGHT_ARROW "m",                     "fathom",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  359 */  { fnCvtFathomM,                 multiply,                    "fm." STD_RIGHT_ARROW "m",                     STD_RIGHT_ARROW " m",                          (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  360 */  { fnCvtFathomM,                 multiply,                    "fm." STD_RIGHT_ARROW "m",                     STD_SPACE_HAIR,                                (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  361 */  { fnCvtFathomM,                 divide,                      "m" STD_RIGHT_ARROW "fm.",                     "m " STD_RIGHT_ARROW,                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  362 */  { fnCvtFathomM,                 divide,                      "m" STD_RIGHT_ARROW "fm.",                     "fathom",                                      (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  363 */  { fnCvtFathomM,                 divide,                      "m" STD_RIGHT_ARROW "fm.",                     STD_SPACE_HAIR,                                (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  364 */  { fnCvtBarrelM3,                multiply,                    "bbl" STD_RIGHT_ARROW "m" STD_SUP_3,           "barrel",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  365 */  { fnCvtBarrelM3,                multiply,                    "bbl" STD_RIGHT_ARROW "m" STD_SUP_3,           STD_RIGHT_ARROW " m" STD_SUP_3,                (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  366 */  { fnCvtBarrelM3,                divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "bbl",           STD_RIGHT_ARROW " m" STD_SUP_3,                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  367 */  { fnCvtBarrelM3,                divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "bbl",           "barrel",                                      (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  368 */  { fnCvtAtmPa,                   multiply,                    "atm" STD_RIGHT_ARROW "Pa",                    STD_SPACE_HAIR,                                (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  369 */  { fnCvtAtmPa,                   divide,                      "Pa" STD_RIGHT_ARROW "atm",                    STD_SPACE_HAIR,                                (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  370 */  { fnCvtHectareM2,               multiply,                    "ha" STD_RIGHT_ARROW "m" STD_SUP_2,            "ha" STD_RIGHT_ARROW "m" STD_SUP_2,            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  371 */  { fnCvtHectareM2,               divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ha",            "m" STD_SUP_2 STD_RIGHT_ARROW "ha",            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  372 */  { fnCvtMuM2,                    divide,                      "m" STD_u_BREVE STD_RIGHT_ARROW "m" STD_SUP_2, "m" STD_u_BREVE STD_RIGHT_ARROW "m" STD_SUP_2, (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  373 */  { fnCvtMuM2,                    multiply,                    "m" STD_SUP_2 STD_RIGHT_ARROW "m" STD_u_BREVE, "m" STD_SUP_2 STD_RIGHT_ARROW "m" STD_u_BREVE, (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  374 */  { fnCvtLiM,                     multiply,                    "l" STD_i_BREVE STD_RIGHT_ARROW "m",           "l" STD_i_BREVE STD_RIGHT_ARROW "m",           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  375 */  { fnCvtLiM,                     divide,                      "m" STD_RIGHT_ARROW "l" STD_i_BREVE,           "m" STD_RIGHT_ARROW "l" STD_i_BREVE,           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  376 */  { fnCvtChiM,                    divide,                      "ch" STD_i_BREVE STD_RIGHT_ARROW "m",          "ch" STD_i_BREVE STD_RIGHT_ARROW "m",          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  377 */  { fnCvtChiM,                    multiply,                    "m" STD_RIGHT_ARROW "ch" STD_i_BREVE,          "m" STD_RIGHT_ARROW "ch" STD_i_BREVE,          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  378 */  { fnCvtYinM,                    divide,                      "y" STD_i_BREVE "n" STD_RIGHT_ARROW "m",       "y" STD_i_BREVE "n" STD_RIGHT_ARROW "m",       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  379 */  { fnCvtYinM,                    multiply,                    "m" STD_RIGHT_ARROW "y" STD_i_BREVE "n",       "m" STD_RIGHT_ARROW "y" STD_i_BREVE "n",       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  380 */  { fnCvtCunM,                    divide,                      "c" STD_u_GRAVE "n" STD_RIGHT_ARROW "m",       "c" STD_u_GRAVE "n" STD_RIGHT_ARROW "m",       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  381 */  { fnCvtCunM,                    multiply,                    "m" STD_RIGHT_ARROW "c" STD_u_GRAVE "n",       "m" STD_RIGHT_ARROW "c" STD_u_GRAVE "n",       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  382 */  { fnCvtZhangM,                  divide,                      "zh" STD_a_GRAVE "n" STD_RIGHT_ARROW "m",      "zh" STD_a_GRAVE "ng",                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  383 */  { fnCvtZhangM,                  divide,                      "zh" STD_a_GRAVE "n" STD_RIGHT_ARROW "m",      STD_RIGHT_ARROW " m",                          (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  384 */  { fnCvtZhangM,                  multiply,                    "m" STD_RIGHT_ARROW "zh" STD_a_GRAVE "n",      "m " STD_RIGHT_ARROW,                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  385 */  { fnCvtZhangM,                  multiply,                    "m" STD_RIGHT_ARROW "zh" STD_a_GRAVE "n",      "zh" STD_a_GRAVE "ng",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED  },
/*  386 */  { fnCvtFenM,                    divide,                      "f" STD_e_MACRON "n" STD_RIGHT_ARROW "m",      "f" STD_e_MACRON "n" STD_RIGHT_ARROW "m",      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  387 */  { fnCvtFenM,                    multiply,                    "m" STD_RIGHT_ARROW "f" STD_e_MACRON "n",      "m" STD_RIGHT_ARROW "f" STD_e_MACRON "n",      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  388 */  { itemToBeCoded,                NOPARAM,                     "0388",                                        "0388",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  389 */  { itemToBeCoded,                NOPARAM,                     "0389",                                        "0389",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  390 */  { itemToBeCoded,                NOPARAM,                     "0390",                                        "0390",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  391 */  { itemToBeCoded,                NOPARAM,                     "0391",                                        "0391",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  392 */  { itemToBeCoded,                NOPARAM,                     "0392",                                        "0392",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  393 */  { itemToBeCoded,                NOPARAM,                     "0393",                                        "0393",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  394 */  { itemToBeCoded,                NOPARAM,                     "0394",                                        "0394",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  395 */  { itemToBeCoded,                NOPARAM,                     "0395",                                        "0395",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},


// Flag, bit, rotation, and logical OPs
/*  396 */  { fnIsFlagClearClear,           TM_FLAGW,                    "FC?C",                                        "FC?C",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  397 */  { fnIsFlagClearSet,             TM_FLAGW,                    "FC?S",                                        "FC?S",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  398 */  { fnIsFlagClearFlip,            TM_FLAGW,                    "FC?F",                                        "FC?F",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  399 */  { fnIsFlagSetClear,             TM_FLAGW,                    "FS?C",                                        "FS?C",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  400 */  { fnIsFlagSetSet,               TM_FLAGW,                    "FS?S",                                        "FS?S",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  401 */  { fnIsFlagSetFlip,              TM_FLAGW,                    "FS?F",                                        "FS?F",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  402 */  { fnLogicalNand,                NOPARAM,                     "NAND",                                        "NAND",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  403 */  { fnLogicalNor,                 NOPARAM,                     "NOR",                                         "NOR",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  404 */  { fnLogicalXnor,                NOPARAM,                     "XNOR",                                        "XNOR",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  405 */  { fnBs,                         TM_VALUE,                    "BS?",                                         "BS?",                                         (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  406 */  { fnBc,                         TM_VALUE,                    "BC?",                                         "BC?",                                         (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  407 */  { fnCb,                         TM_VALUE,                    "CB",                                          "CB",                                          (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  408 */  { fnSb,                         TM_VALUE,                    "SB",                                          "SB",                                          (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  409 */  { fnFb,                         TM_VALUE,                    "FB",                                          "FB",                                          (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  410 */  { fnRl,                         TM_VALUE,                    "RL",                                          "RL",                                          (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  411 */  { fnRlc,                        TM_VALUE,                    "RLC",                                         "RLC",                                         (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  412 */  { fnRr,                         TM_VALUE,                    "RR",                                          "RR",                                          (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  413 */  { fnRrc,                        TM_VALUE,                    "RRC",                                         "RRC",                                         (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  414 */  { fnSl,                         TM_VALUE,                    "SL",                                          "SL",                                          (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  415 */  { fnSr,                         TM_VALUE,                    "SR",                                          "SR",                                          (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  416 */  { fnAsr,                        TM_VALUE,                    "ASR",                                         "ASR",                                         (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  417 */  { fnLj,                         NOPARAM,                     "LJ",                                          "LJ",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  418 */  { fnRj,                         NOPARAM,                     "RJ",                                          "RJ",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  419 */  { fnMaskl,                      TM_VALUE,                    "MASKL",                                       "MASKL",                                       (0 << TAM_MAX_BITS) |    64, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  420 */  { fnMaskr,                      TM_VALUE,                    "MASKR",                                       "MASKR",                                       (0 << TAM_MAX_BITS) |    64, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  421 */  { fnMirror,                     NOPARAM,                     "MIRROR",                                      "MIRROR",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  422 */  { fnCountBits,                  NOPARAM,                     "#B",                                          "#B",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  423 */  { fnSdl,                        TM_VALUE,                    "SDL",                                         "SDL",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  424 */  { fnSdr,                        TM_VALUE,                    "SDR",                                         "SDR",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  425 */  { itemToBeCoded,                NOPARAM,                     "0425",                                        "0425",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  426 */  { itemToBeCoded,                NOPARAM,                     "0426",                                        "0426",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  427 */  { itemToBeCoded,                NOPARAM,                     "0427",                                        "0427",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  428 */  { itemToBeCoded,                NOPARAM,                     "0428",                                        "0428",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  429 */  { itemToBeCoded,                NOPARAM,                     "0429",                                        "0429",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  430 */  { itemToBeCoded,                NOPARAM,                     "0430",                                        "0430",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  431 */  { itemToBeCoded,                NOPARAM,                     "0431",                                        "0431",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  432 */  { itemToBeCoded,                NOPARAM,                     "0432",                                        "0432",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},


// Statistical sums
/*  433 */  { fnSigma,                      1,                           STD_SIGMA "+",                                 STD_SIGMA "+",                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_DISABLED  | US_ENABLED  },
/*  434 */  { fnSigma,                      2,                           STD_SIGMA "-",                                 STD_SIGMA "-",                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_DISABLED  | US_ENABLED  },
/*  435 */  { fnStatSum,                    0,                           "n" STD_SIGMA,                                 "n",                                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  436 */  { fnStatSum,                    SUM_X,                       STD_SIGMA "x",                                 STD_SIGMA "x",                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  437 */  { fnStatSum,                    SUM_Y,                       STD_SIGMA "y",                                 STD_SIGMA "y",                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  438 */  { fnStatSum,                    SUM_X2,                      STD_SIGMA "x" STD_SUP_2,                       STD_SIGMA "x" STD_SUP_2,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  439 */  { fnStatSum,                    SUM_X2Y,                     STD_SIGMA "x" STD_SUP_2 "y",                   STD_SIGMA "x" STD_SUP_2 "y",                   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  440 */  { fnStatSum,                    SUM_Y2,                      STD_SIGMA "y" STD_SUP_2,                       STD_SIGMA "y" STD_SUP_2,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  441 */  { fnStatSum,                    SUM_XY,                      STD_SIGMA "xy",                                STD_SIGMA "xy",                                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  442 */  { fnStatSum,                    SUM_lnXlnY,                  STD_SIGMA "lnxy",                              STD_SIGMA "lnxy",                              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  443 */  { fnStatSum,                    SUM_lnX,                     STD_SIGMA "lnx",                               STD_SIGMA "lnx",                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  444 */  { fnStatSum,                    SUM_ln2X,                    STD_SIGMA "ln" STD_SUP_2 "x",                  STD_SIGMA "ln" STD_SUP_2 "x",                  (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  445 */  { fnStatSum,                    SUM_YlnX,                    STD_SIGMA "ylnx",                              STD_SIGMA "ylnx",                              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  446 */  { fnStatSum,                    SUM_lnY,                     STD_SIGMA "lny",                               STD_SIGMA "lny",                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  447 */  { fnStatSum,                    SUM_ln2Y,                    STD_SIGMA "ln" STD_SUP_2 "y",                  STD_SIGMA "ln" STD_SUP_2 "y",                  (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  448 */  { fnStatSum,                    SUM_XlnY,                    STD_SIGMA "xlny",                              STD_SIGMA "xlny",                              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  449 */  { fnStatSum,                    SUM_X2lnY,                   STD_SIGMA "x" STD_SUP_2 "lny",                 STD_SIGMA "x" STD_SUP_2 "lny",                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  450 */  { fnStatSum,                    SUM_lnYonX,                  STD_SIGMA "lny/x",                             STD_SIGMA "lny/x",                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  451 */  { fnStatSum,                    SUM_X2onY,                   STD_SIGMA "x" STD_SUP_2 "/y",                  STD_SIGMA "x" STD_SUP_2 "/y",                  (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  452 */  { fnStatSum,                    SUM_1onX,                    STD_SIGMA STD_SUP_1 "/x",                      STD_SIGMA STD_SUP_1 "/x",                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  453 */  { fnStatSum,                    SUM_1onX2,                   STD_SIGMA STD_SUP_1 "/x" STD_SUP_2,            STD_SIGMA STD_SUP_1 "/x" STD_SUP_2,            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  454 */  { fnStatSum,                    SUM_XonY,                    STD_SIGMA "x/y",                               STD_SIGMA "x/y",                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  455 */  { fnStatSum,                    SUM_1onY,                    STD_SIGMA STD_SUP_1 "/y",                      STD_SIGMA STD_SUP_1 "/y",                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  456 */  { fnStatSum,                    SUM_1onY2,                   STD_SIGMA STD_SUP_1 "/y" STD_SUP_2,            STD_SIGMA STD_SUP_1 "/y" STD_SUP_2,            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  457 */  { fnStatSum,                    SUM_X3,                      STD_SIGMA "x" STD_SUP_3,                       STD_SIGMA "x" STD_SUP_3,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  458 */  { fnStatSum,                    SUM_X4,                      STD_SIGMA "x" STD_SUP_4,                       STD_SIGMA "x" STD_SUP_4,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/*  459 */  { itemToBeCoded,                NOPARAM,                     "0458",                                        "0458",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  460 */  { itemToBeCoded,                NOPARAM,                     "0459",                                        "0459",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  461 */  { itemToBeCoded,                NOPARAM,                     "0460",                                        "0460",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  462 */  { itemToBeCoded,                NOPARAM,                     "0461",                                        "0461",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},


// System flags
/*  463 */  { fnGetSystemFlag,              FLAG_TDM24,                  "TDM24",                                       "TDM24",                                       (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED}, // The system flags,
/*  464 */  { fnGetSystemFlag,              FLAG_YMD,                    "YMD",                                         "YMD",                                         (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED}, // items from 453 to 493,
/*  465 */  { fnGetSystemFlag,              FLAG_DMY,                    "DMY",                                         "DMY",                                         (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED}, // MUST be in the same
/*  466 */  { fnGetSystemFlag,              FLAG_MDY,                    "MDY",                                         "MDY",                                         (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED}, // order as the flag
/*  467 */  { fnGetSystemFlag,              FLAG_CPXRES,                 "CPXRES",                                      "CPXRES",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED}, // number (8 LSB) defined
/*  468 */  { fnGetSystemFlag,              FLAG_CPXj,                   "CPXj",                                        "CPXj",                                        (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED}, // in defines.h
/*  469 */  { fnGetSystemFlag,              FLAG_POLAR,                  "POLAR",                                       "POLAR",                                       (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  470 */  { fnGetSystemFlag,              FLAG_FRACT,                  "FRACT",                                       "FRACT",                                       (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED}, // And TDM24 MUST be
/*  471 */  { fnGetSystemFlag,              FLAG_PROPFR,                 "PROPFR",                                      "PROPFR",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED}, // the first.
/*  472 */  { fnGetSystemFlag,              FLAG_DENANY,                 "DENANY",                                      "DENANY",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  473 */  { fnGetSystemFlag,              FLAG_DENFIX,                 "DENFIX",                                      "DENFIX",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  474 */  { fnGetSystemFlag,              FLAG_CARRY,                  "CARRY",                                       "CARRY",                                       (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  475 */  { fnGetSystemFlag,              FLAG_OVERFLOW,               "OVERFL",                                      "OVERFL",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  476 */  { fnGetSystemFlag,              FLAG_LEAD0,                  "LEAD.0",                                      "LEAD.0",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  477 */  { fnGetSystemFlag,              FLAG_ALPHA,                  "ALPHA",                                       "ALPHA",                                       (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  478 */  { fnGetSystemFlag,              FLAG_alphaCAP,               STD_alpha "CAP",                               STD_alpha "CAP",                               (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  479 */  { fnGetSystemFlag,              FLAG_RUNTIM,                 "RUNTIM",                                      "RUNTIM",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  480 */  { fnGetSystemFlag,              FLAG_RUNIO,                  "RUNIO",                                       "RUNIO",                                       (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  481 */  { fnGetSystemFlag,              FLAG_PRINT,                  "PRINT",                                       "PRINT",                                       (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  482 */  { fnGetSystemFlag,              FLAG_TRACE,                  "TRACE",                                       "TRACE",                                       (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  483 */  { fnGetSystemFlag,              FLAG_USER,                   "USER",                                        "USER",                                        (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  484 */  { fnGetSystemFlag,              FLAG_LOWBAT,                 "LOWBAT",                                      "LOWBAT",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  485 */  { fnGetSystemFlag,              FLAG_SLOW,                   "SLOW",                                        "SLOW",                                        (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  486 */  { fnGetSystemFlag,              FLAG_SPCRES,                 "SPCRES",                                      "SPCRES",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  487 */  { fnGetSystemFlag,              FLAG_SSIZE8,                 "SSIZE8",                                      "SSIZE8",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  488 */  { fnGetSystemFlag,              FLAG_QUIET,                  "QUIET",                                       "QUIET",                                       (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  489 */  { fnGetSystemFlag,              FLAG_DECIMP,                 "DECIM.",                                      "DECIM.",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  490 */  { fnGetSystemFlag,              FLAG_MULTx,                  "MULT" STD_CROSS,                              "MULT" STD_CROSS,                              (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  491 */  { fnGetSystemFlag,              FLAG_ALLENG,                 "ALLENG",                                      "ALLENG",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  492 */  { fnGetSystemFlag,              FLAG_GROW,                   "GROW",                                        "GROW",                                        (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  493 */  { fnGetSystemFlag,              FLAG_AUTOFF,                 "AUTOFF",                                      "AUTOFF",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  494 */  { fnGetSystemFlag,              FLAG_AUTXEQ,                 "AUTXEQ",                                      "AUTXEQ",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  495 */  { fnGetSystemFlag,              FLAG_PRTACT,                 "PRTACT",                                      "PRTACT",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  496 */  { fnGetSystemFlag,              FLAG_NUMIN,                  "NUM.IN",                                      "NUM.IN",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  497 */  { fnGetSystemFlag,              FLAG_ALPIN,                  "ALP.IN",                                      "ALP.IN",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  498 */  { fnGetSystemFlag,              FLAG_ASLIFT,                 "ASLIFT",                                      "ASLIFT",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  499 */  { fnGetSystemFlag,              FLAG_IGN1ER,                 "IGN1ER",                                      "IGN1ER",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  500 */  { fnGetSystemFlag,              FLAG_INTING,                 "INTING",                                      "INTING",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  501 */  { fnGetSystemFlag,              FLAG_SOLVING,                "SOLVING",                                     "SOLVING",                                     (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  502 */  { fnGetSystemFlag,              FLAG_VMDISP,                 "VMDISP",                                      "VMDISP",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  503 */  { fnGetSystemFlag,              FLAG_USB,                    "USB",                                         "USB",                                         (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED},
/*  504 */  { itemToBeCoded,                NOPARAM,                     "0504",                                        "0504",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  505 */  { itemToBeCoded,                NOPARAM,                     "0505",                                        "0505",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  506 */  { itemToBeCoded,                NOPARAM,                     "0506",                                        "0506",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  507 */  { itemToBeCoded,                NOPARAM,                     "0507",                                        "0507",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  508 */  { itemToBeCoded,                NOPARAM,                     "0508",                                        "0508",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  509 */  { itemToBeCoded,                NOPARAM,                     "0509",                                        "0509",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  510 */  { itemToBeCoded,                NOPARAM,                     "0510",                                        "0510",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  511 */  { itemToBeCoded,                NOPARAM,                     "0511",                                        "0511",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  512 */  { itemToBeCoded,                NOPARAM,                     "0512",                                        "0512",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  513 */  { itemToBeCoded,                NOPARAM,                     "0513",                                        "0513",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  514 */  { itemToBeCoded,                NOPARAM,                     "0514",                                        "0514",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  515 */  { itemToBeCoded,                NOPARAM,                     "0515",                                        "0515",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  516 */  { itemToBeCoded,                NOPARAM,                     "0516",                                        "0516",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  517 */  { itemToBeCoded,                NOPARAM,                     "0517",                                        "0517",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  518 */  { itemToBeCoded,                NOPARAM,                     "0518",                                        "0518",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  519 */  { itemToBeCoded,                NOPARAM,                     "0519",                                        "0519",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  520 */  { itemToBeCoded,                NOPARAM,                     "0520",                                        "0520",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  521 */  { itemToBeCoded,                NOPARAM,                     "0521",                                        "0521",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  522 */  { itemToBeCoded,                NOPARAM,                     "0522",                                        "0522",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  523 */  { itemToBeCoded,                NOPARAM,                     "0523",                                        "0523",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  524 */  { itemToBeCoded,                NOPARAM,                     "0524",                                        "0524",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  525 */  { itemToBeCoded,                NOPARAM,                     "0525",                                        "0525",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/*  526 */  { itemToBeCoded,                NOPARAM,                     "0526",                                        "0526",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},


// Bufferized items
/*  527 */  { addItemToBuffer,              REGISTER_X,                  "X",                                           "X",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED}, // The
/*  528 */  { addItemToBuffer,              REGISTER_Y,                  "Y",                                           "Y",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED}, // order
/*  529 */  { addItemToBuffer,              REGISTER_Z,                  "Z",                                           "Z",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED}, // of these
/*  530 */  { addItemToBuffer,              REGISTER_T,                  "T",                                           "T",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED}, // 12 lines
/*  531 */  { addItemToBuffer,              REGISTER_A,                  "A",                                           "A",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED}, // Must be
/*  532 */  { addItemToBuffer,              REGISTER_B,                  "B",                                           "B",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED}, // kept as
/*  533 */  { addItemToBuffer,              REGISTER_C,                  "C",                                           "C",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED}, // is.
/*  534 */  { addItemToBuffer,              REGISTER_D,                  "D",                                           "D",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED}, // Do not
/*  535 */  { addItemToBuffer,              REGISTER_L,                  "L",                                           "L",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED}, // put them
/*  536 */  { addItemToBuffer,              REGISTER_I,                  "I",                                           "I",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED}, // in a
/*  537 */  { addItemToBuffer,              REGISTER_J,                  "J",                                           "J",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED}, // different
/*  538 */  { addItemToBuffer,              REGISTER_K,                  "K",                                           "K",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED}, // order!
/*  539 */  { addItemToBuffer,              ITM_INDIRECTION,             STD_RIGHT_ARROW,                               STD_RIGHT_ARROW,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  540 */  { itemToBeCoded,                NOPARAM,                     "0540",                                        "0540",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  541 */  { itemToBeCoded,                NOPARAM,                     "0541",                                        "0541",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  542 */  { itemToBeCoded,                NOPARAM,                     "0542",                                        "0542",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  543 */  { itemToBeCoded,                NOPARAM,                     "0543",                                        "0543",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  544 */  { itemToBeCoded,                NOPARAM,                     "0544",                                        "0544",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  545 */  { itemToBeCoded,                NOPARAM,                     "0545",                                        "0545",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  546 */  { itemToBeCoded,                NOPARAM,                     "0546",                                        "0546",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  547 */  { itemToBeCoded,                NOPARAM,                     "0547",                                        "0547",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  548 */  { addItemToBuffer,              ITM_0,                       "",                                            "0",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  549 */  { addItemToBuffer,              ITM_1,                       "",                                            "1",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  550 */  { addItemToBuffer,              ITM_2,                       "",                                            "2",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  551 */  { addItemToBuffer,              ITM_3,                       "",                                            "3",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  552 */  { addItemToBuffer,              ITM_4,                       "",                                            "4",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  553 */  { addItemToBuffer,              ITM_5,                       "",                                            "5",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  554 */  { addItemToBuffer,              ITM_6,                       "",                                            "6",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  555 */  { addItemToBuffer,              ITM_7,                       "",                                            "7",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  556 */  { addItemToBuffer,              ITM_8,                       "",                                            "8",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  557 */  { addItemToBuffer,              ITM_9,                       "",                                            "9",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  558 */  { addItemToBuffer,              ITM_A,                       "A",                                           "A",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  559 */  { addItemToBuffer,              ITM_B,                       "B",                                           "B",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  560 */  { addItemToBuffer,              ITM_C,                       "C",                                           "C",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  561 */  { addItemToBuffer,              ITM_D,                       "D",                                           "D",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  562 */  { addItemToBuffer,              ITM_E,                       "E",                                           "E",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  563 */  { addItemToBuffer,              ITM_F,                       "F",                                           "F",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  564 */  { addItemToBuffer,              ITM_G,                       "G",                                           "G",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  565 */  { addItemToBuffer,              ITM_H,                       "H",                                           "H",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  566 */  { addItemToBuffer,              ITM_I,                       "I",                                           "I",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  567 */  { addItemToBuffer,              ITM_J,                       "J",                                           "J",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  568 */  { addItemToBuffer,              ITM_K,                       "K",                                           "K",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  569 */  { addItemToBuffer,              ITM_L,                       "L",                                           "L",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  570 */  { addItemToBuffer,              ITM_M,                       "M",                                           "M",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  571 */  { addItemToBuffer,              ITM_N,                       "N",                                           "N",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  572 */  { addItemToBuffer,              ITM_O,                       "O",                                           "O",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  573 */  { addItemToBuffer,              ITM_P,                       "P",                                           "P",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  574 */  { addItemToBuffer,              ITM_Q,                       "Q",                                           "Q",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  575 */  { addItemToBuffer,              ITM_R,                       "R",                                           "R",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  576 */  { addItemToBuffer,              ITM_S,                       "S",                                           "S",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  577 */  { addItemToBuffer,              ITM_T,                       "T",                                           "T",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  578 */  { addItemToBuffer,              ITM_U,                       "U",                                           "U",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  579 */  { addItemToBuffer,              ITM_V,                       "V",                                           "V",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  580 */  { addItemToBuffer,              ITM_W,                       "W",                                           "W",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  581 */  { addItemToBuffer,              ITM_X,                       "X",                                           "X",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  582 */  { addItemToBuffer,              ITM_Y,                       "Y",                                           "Y",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  583 */  { addItemToBuffer,              ITM_Z,                       "Z",                                           "Z",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  584 */  { addItemToBuffer,              ITM_a,                       "a",                                           "a",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  585 */  { addItemToBuffer,              ITM_b,                       "b",                                           "b",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  586 */  { addItemToBuffer,              ITM_c,                       "c",                                           "c",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  587 */  { addItemToBuffer,              ITM_d,                       "d",                                           "d",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  588 */  { addItemToBuffer,              ITM_e,                       "e",                                           "e",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  589 */  { addItemToBuffer,              ITM_f,                       "f",                                           "f",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  590 */  { addItemToBuffer,              ITM_g,                       "g",                                           "g",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  591 */  { addItemToBuffer,              ITM_h,                       "h",                                           "h",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  592 */  { addItemToBuffer,              ITM_i,                       "i",                                           "i",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  593 */  { addItemToBuffer,              ITM_j,                       "j",                                           "j",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  594 */  { addItemToBuffer,              ITM_k,                       "k",                                           "k",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  595 */  { addItemToBuffer,              ITM_l,                       "l",                                           "l",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  596 */  { addItemToBuffer,              ITM_m,                       "m",                                           "m",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  597 */  { addItemToBuffer,              ITM_n,                       "n",                                           "n",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  598 */  { addItemToBuffer,              ITM_o,                       "o",                                           "o",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  599 */  { addItemToBuffer,              ITM_p,                       "p",                                           "p",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  600 */  { addItemToBuffer,              ITM_q,                       "q",                                           "q",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  601 */  { addItemToBuffer,              ITM_r,                       "r",                                           "r",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  602 */  { addItemToBuffer,              ITM_s,                       "s",                                           "s",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  603 */  { addItemToBuffer,              ITM_t,                       "t",                                           "t",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  604 */  { addItemToBuffer,              ITM_u,                       "u",                                           "u",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  605 */  { addItemToBuffer,              ITM_v,                       "v",                                           "v",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  606 */  { addItemToBuffer,              ITM_w,                       "w",                                           "w",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  607 */  { addItemToBuffer,              ITM_x,                       "x",                                           "x",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  608 */  { addItemToBuffer,              ITM_y,                       "y",                                           "y",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  609 */  { addItemToBuffer,              ITM_z,                       "z",                                           "z",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  610 */  { addItemToBuffer,              ITM_ALPHA,                   "",                                            STD_ALPHA,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  611 */  { itemToBeCoded,                NOPARAM,                     "0611",                                        "0611",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  612 */  { addItemToBuffer,              ITM_BETA,                    "",                                            STD_BETA,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  613 */  { addItemToBuffer,              ITM_GAMMA,                   "",                                            STD_GAMMA,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  614 */  { addItemToBuffer,              ITM_DELTA,                   "",                                            STD_DELTA,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  615 */  { addItemToBuffer,              ITM_EPSILON,                 "",                                            STD_EPSILON,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  616 */  { itemToBeCoded,                NOPARAM,                     "0616",                                        "0616",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  617 */  { addItemToBuffer,              ITM_ZETA,                    "",                                            STD_ZETA,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  618 */  { addItemToBuffer,              ITM_ETA,                     "",                                            STD_ETA,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  619 */  { itemToBeCoded,                NOPARAM,                     "0619",                                        "0619",                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  620 */  { addItemToBuffer,              ITM_THETA,                   "",                                            STD_THETA,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  621 */  { addItemToBuffer,              ITM_IOTA,                    "",                                            STD_IOTA,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  622 */  { itemToBeCoded,                NOPARAM,                     "0622",                                        "0622",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  623 */  { itemToBeCoded,                NOPARAM,                     "0623",                                        "0623",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  624 */  { addItemToBuffer,              ITM_IOTA_DIALYTIKA,          "",                                            STD_IOTA_DIALYTIKA,                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  625 */  { addItemToBuffer,              ITM_KAPPA,                   "",                                            STD_KAPPA,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  626 */  { addItemToBuffer,              ITM_LAMBDA,                  "",                                            STD_LAMBDA,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  627 */  { addItemToBuffer,              ITM_MU,                      "",                                            STD_MU,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  628 */  { addItemToBuffer,              ITM_NU,                      "",                                            STD_NU,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  629 */  { addItemToBuffer,              ITM_XI,                      "",                                            STD_XI,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  630 */  { addItemToBuffer,              ITM_OMICRON,                 "",                                            STD_OMICRON,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  631 */  { itemToBeCoded,                NOPARAM,                     "0631",                                        "0631",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  632 */  { addItemToBuffer,              ITM_PI,                      "",                                            STD_PI,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  633 */  { addItemToBuffer,              ITM_RHO,                     "",                                            STD_RHO,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  634 */  { addItemToBuffer,              ITM_SIGMA,                   "",                                            STD_SIGMA,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  635 */  { itemToBeCoded,                NOPARAM,                     "0635",                                        "0635",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  636 */  { addItemToBuffer,              ITM_TAU,                     "",                                            STD_TAU,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  637 */  { addItemToBuffer,              ITM_UPSILON,                 "",                                            STD_UPSILON,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  638 */  { itemToBeCoded,                NOPARAM,                     "0638",                                        "0638",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  639 */  { addItemToBuffer,              ITM_UPSILON_DIALYTIKA,       "",                                            STD_UPSILON_DIALYTIKA,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  640 */  { itemToBeCoded,                NOPARAM,                     "0640",                                        "0640",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  641 */  { addItemToBuffer,              ITM_PHI,                     "",                                            STD_PHI,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  642 */  { addItemToBuffer,              ITM_CHI,                     "",                                            STD_CHI,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  643 */  { addItemToBuffer,              ITM_PSI,                     "",                                            STD_PSI,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  644 */  { addItemToBuffer,              ITM_OMEGA,                   "",                                            STD_OMEGA,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  645 */  { itemToBeCoded,                NOPARAM,                     "0645",                                        "0645",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  646 */  { addItemToBuffer,              ITM_alpha,                   "",                                            STD_alpha,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  647 */  { addItemToBuffer,              ITM_alpha_TONOS,             "",                                            STD_alpha_TONOS,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  648 */  { addItemToBuffer,              ITM_beta,                    "",                                            STD_beta,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  649 */  { addItemToBuffer,              ITM_gamma,                   "",                                            STD_gamma,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  650 */  { addItemToBuffer,              ITM_delta,                   "",                                            STD_delta,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  651 */  { addItemToBuffer,              ITM_epsilon,                 "",                                            STD_epsilon,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  652 */  { addItemToBuffer,              ITM_epsilon_TONOS,           "",                                            STD_epsilon_TONOS,                             (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  653 */  { addItemToBuffer,              ITM_zeta,                    "",                                            STD_zeta,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  654 */  { addItemToBuffer,              ITM_eta,                     "",                                            STD_eta,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  655 */  { addItemToBuffer,              ITM_eta_TONOS,               "",                                            STD_eta_TONOS,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  656 */  { addItemToBuffer,              ITM_theta,                   "",                                            STD_theta,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  657 */  { addItemToBuffer,              ITM_iota,                    "",                                            STD_iota,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  658 */  { addItemToBuffer,              ITM_iotaTON,                 "",                                            STD_iota_TONOS,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  659 */  { addItemToBuffer,              ITM_iota_DIALYTIKA_TONOS,    "",                                            STD_iota_DIALYTIKA_TONOS,                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  660 */  { addItemToBuffer,              ITM_iota_DIALYTIKA,          "",                                            STD_iota_DIALYTIKA,                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  661 */  { addItemToBuffer,              ITM_kappa,                   "",                                            STD_kappa,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  662 */  { addItemToBuffer,              ITM_lambda,                  "",                                            STD_lambda,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  663 */  { addItemToBuffer,              ITM_mu,                      "",                                            STD_mu,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  664 */  { addItemToBuffer,              ITM_nu,                      "",                                            STD_nu,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  665 */  { addItemToBuffer,              ITM_xi,                      "",                                            STD_xi,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  666 */  { addItemToBuffer,              ITM_omicron,                 "",                                            STD_omicron,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  667 */  { addItemToBuffer,              ITM_omicron_TONOS,           "",                                            STD_omicron_TONOS,                             (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  668 */  { addItemToBuffer,              ITM_pi,                      "",                                            STD_pi,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  669 */  { addItemToBuffer,              ITM_rho,                     "",                                            STD_rho,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  670 */  { addItemToBuffer,              ITM_sigma,                   "",                                            STD_sigma,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  671 */  { addItemToBuffer,              ITM_sigma_end,               "",                                            STD_sigma_end,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  672 */  { addItemToBuffer,              ITM_tau,                     "",                                            STD_tau,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  673 */  { addItemToBuffer,              ITM_upsilon,                 "",                                            STD_upsilon,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  674 */  { addItemToBuffer,              ITM_upsilon_TONOS,           "",                                            STD_upsilon_TONOS,                             (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  675 */  { addItemToBuffer,              ITM_upsilon_DIALYTIKA,       "",                                            STD_upsilon_DIALYTIKA,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  676 */  { addItemToBuffer,              ITM_upsilon_DIALYTIKA_TONOS, "",                                            STD_upsilon_DIALYTIKA_TONOS,                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  677 */  { addItemToBuffer,              ITM_phi,                     "",                                            STD_phi,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  678 */  { addItemToBuffer,              ITM_chi,                     "",                                            STD_chi,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  679 */  { addItemToBuffer,              ITM_psi,                     "",                                            STD_psi,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  680 */  { addItemToBuffer,              ITM_omega,                   "",                                            STD_omega,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  681 */  { addItemToBuffer,              ITM_omega_TONOS,             "",                                            STD_omega_TONOS,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  682 */  { itemToBeCoded,                NOPARAM,                     "0682",                                        "0682",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  683 */  { itemToBeCoded,                NOPARAM,                     "0683",                                        "0683",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  684 */  { itemToBeCoded,                NOPARAM,                     "0684",                                        "0684",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  685 */  { itemToBeCoded,                NOPARAM,                     "0685",                                        "0685",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  686 */  { itemToBeCoded,                NOPARAM,                     "0686",                                        "0686",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  687 */  { itemToBeCoded,                NOPARAM,                     "0687",                                        "0687",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  688 */  { addItemToBuffer,              ITM_A_MACRON,                STD_A_MACRON,                                  STD_A_MACRON,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  689 */  { addItemToBuffer,              ITM_A_ACUTE,                 STD_A_ACUTE,                                   STD_A_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  690 */  { addItemToBuffer,              ITM_A_BREVE,                 STD_A_BREVE,                                   STD_A_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  691 */  { addItemToBuffer,              ITM_A_GRAVE,                 STD_A_GRAVE,                                   STD_A_GRAVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  692 */  { addItemToBuffer,              ITM_A_DIARESIS,              STD_A_DIARESIS,                                STD_A_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  693 */  { addItemToBuffer,              ITM_A_TILDE,                 STD_A_TILDE,                                   STD_A_TILDE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  694 */  { addItemToBuffer,              ITM_A_CIRC,                  STD_A_CIRC,                                    STD_A_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  695 */  { addItemToBuffer,              ITM_A_RING,                  STD_A_RING,                                    STD_A_RING,                                    (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  696 */  { addItemToBuffer,              ITM_AE,                      STD_AE,                                        STD_AE,                                        (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  697 */  { addItemToBuffer,              ITM_A_OGONEK,                STD_A_OGONEK,                                  STD_A_OGONEK,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  698 */  { addItemToBuffer,              ITM_C_ACUTE,                 STD_C_ACUTE,                                   STD_C_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  699 */  { addItemToBuffer,              ITM_C_CARON,                 STD_C_CARON,                                   STD_C_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  700 */  { addItemToBuffer,              ITM_C_CEDILLA,               STD_C_CEDILLA,                                 STD_C_CEDILLA,                                 (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  701 */  { addItemToBuffer,              ITM_D_STROKE,                STD_D_STROKE,                                  STD_D_STROKE,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  702 */  { addItemToBuffer,              ITM_D_CARON,                 STD_D_CARON,                                   STD_D_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  703 */  { addItemToBuffer,              ITM_E_MACRON,                STD_E_MACRON,                                  STD_E_MACRON,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  704 */  { addItemToBuffer,              ITM_E_ACUTE,                 STD_E_ACUTE,                                   STD_E_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  705 */  { addItemToBuffer,              ITM_E_BREVE,                 STD_E_BREVE,                                   STD_E_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  706 */  { addItemToBuffer,              ITM_E_GRAVE,                 STD_E_GRAVE,                                   STD_E_GRAVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  707 */  { addItemToBuffer,              ITM_E_DIARESIS,              STD_E_DIARESIS,                                STD_E_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  708 */  { addItemToBuffer,              ITM_E_CIRC,                  STD_E_CIRC,                                    STD_E_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  709 */  { addItemToBuffer,              ITM_E_OGONEK,                STD_E_OGONEK,                                  STD_E_OGONEK,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  710 */  { addItemToBuffer,              ITM_G_BREVE,                 STD_G_BREVE,                                   STD_G_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  711 */  { itemToBeCoded,                NOPARAM,                     "0711",                                        "0711",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  712 */  { addItemToBuffer,              ITM_I_MACRON,                STD_I_MACRON,                                  STD_I_MACRON,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  713 */  { addItemToBuffer,              ITM_I_ACUTE,                 STD_I_ACUTE,                                   STD_I_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  714 */  { addItemToBuffer,              ITM_I_BREVE,                 STD_I_BREVE,                                   STD_I_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  715 */  { addItemToBuffer,              ITM_I_GRAVE,                 STD_I_GRAVE,                                   STD_I_GRAVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  716 */  { addItemToBuffer,              ITM_I_DIARESIS,              STD_I_DIARESIS,                                STD_I_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  717 */  { addItemToBuffer,              ITM_I_CIRC,                  STD_I_CIRC,                                    STD_I_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  718 */  { addItemToBuffer,              ITM_I_OGONEK,                STD_I_OGONEK,                                  STD_I_OGONEK,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  719 */  { addItemToBuffer,              ITM_I_DOT,                   STD_I_DOT,                                     STD_I_DOT,                                     (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  720 */  { addItemToBuffer,              ITM_I_DOTLESS,               "I",                                           "I",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  721 */  { addItemToBuffer,              ITM_L_STROKE,                STD_L_STROKE,                                  STD_L_STROKE,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  722 */  { addItemToBuffer,              ITM_L_ACUTE,                 STD_L_ACUTE,                                   STD_L_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  723 */  { addItemToBuffer,              ITM_L_APOSTROPHE,            STD_L_APOSTROPHE,                              STD_L_APOSTROPHE,                              (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  724 */  { addItemToBuffer,              ITM_N_ACUTE,                 STD_N_ACUTE,                                   STD_N_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  725 */  { addItemToBuffer,              ITM_N_CARON,                 STD_N_CARON,                                   STD_N_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  726 */  { addItemToBuffer,              ITM_N_TILDE,                 STD_N_TILDE,                                   STD_N_TILDE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  727 */  { addItemToBuffer,              ITM_O_MACRON,                STD_O_MACRON,                                  STD_O_MACRON,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  728 */  { addItemToBuffer,              ITM_O_ACUTE,                 STD_O_ACUTE,                                   STD_O_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  729 */  { addItemToBuffer,              ITM_O_BREVE,                 STD_O_BREVE,                                   STD_O_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  730 */  { addItemToBuffer,              ITM_O_GRAVE,                 STD_O_GRAVE,                                   STD_O_GRAVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  731 */  { addItemToBuffer,              ITM_O_DIARESIS,              STD_O_DIARESIS,                                STD_O_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  732 */  { addItemToBuffer,              ITM_O_TILDE,                 STD_O_TILDE,                                   STD_O_TILDE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  733 */  { addItemToBuffer,              ITM_O_CIRC,                  STD_O_CIRC,                                    STD_O_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  734 */  { addItemToBuffer,              ITM_O_STROKE,                STD_O_STROKE,                                  STD_O_STROKE,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  735 */  { addItemToBuffer,              ITM_OE,                      STD_OE,                                        STD_OE,                                        (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  736 */  { itemToBeCoded,                NOPARAM,                     "0736",                                        "0736",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  737 */  { itemToBeCoded,                NOPARAM,                     "0737",                                        "0737",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  738 */  { addItemToBuffer,              ITM_S_SHARP,                 STD_s_SHARP,                                   STD_s_SHARP,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  739 */  { addItemToBuffer,              ITM_S_ACUTE,                 STD_S_ACUTE,                                   STD_S_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  740 */  { addItemToBuffer,              ITM_S_CARON,                 STD_S_CARON,                                   STD_S_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  741 */  { addItemToBuffer,              ITM_S_CEDILLA,               STD_S_CEDILLA,                                 STD_S_CEDILLA,                                 (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  742 */  { addItemToBuffer,              ITM_T_CARON,                 STD_T_CARON,                                   STD_T_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  743 */  { addItemToBuffer,              ITM_T_CEDILLA,               STD_T_CEDILLA,                                 STD_T_CEDILLA,                                 (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  744 */  { addItemToBuffer,              ITM_U_MACRON,                STD_U_MACRON,                                  STD_U_MACRON,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  745 */  { addItemToBuffer,              ITM_U_ACUTE,                 STD_U_ACUTE,                                   STD_U_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  746 */  { addItemToBuffer,              ITM_U_BREVE,                 STD_U_BREVE,                                   STD_U_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  747 */  { addItemToBuffer,              ITM_U_GRAVE,                 STD_U_GRAVE,                                   STD_U_GRAVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  748 */  { addItemToBuffer,              ITM_U_DIARESIS,              STD_U_DIARESIS,                                STD_U_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  749 */  { addItemToBuffer,              ITM_U_TILDE,                 STD_U_TILDE,                                   STD_U_TILDE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  750 */  { addItemToBuffer,              ITM_U_CIRC,                  STD_U_CIRC,                                    STD_U_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  751 */  { addItemToBuffer,              ITM_U_RING,                  STD_U_RING,                                    STD_U_RING,                                    (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  752 */  { addItemToBuffer,              ITM_W_CIRC,                  STD_W_CIRC,                                    STD_W_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  753 */  { itemToBeCoded,                NOPARAM,                     "0753",                                        "0753",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  754 */  { itemToBeCoded,                NOPARAM,                     "0754",                                        "0754",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  755 */  { itemToBeCoded,                NOPARAM,                     "0755",                                        "0755",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  756 */  { addItemToBuffer,              ITM_Y_CIRC,                  STD_Y_CIRC,                                    STD_Y_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  757 */  { addItemToBuffer,              ITM_Y_ACUTE,                 STD_Y_ACUTE,                                   STD_Y_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  758 */  { addItemToBuffer,              ITM_Y_DIARESIS,              STD_Y_DIARESIS,                                STD_Y_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  759 */  { addItemToBuffer,              ITM_Z_ACUTE,                 STD_Z_ACUTE,                                   STD_Z_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  760 */  { addItemToBuffer,              ITM_Z_CARON,                 STD_Z_CARON,                                   STD_Z_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  761 */  { addItemToBuffer,              ITM_Z_DOT,                   STD_Z_DOT,                                     STD_Z_DOT,                                     (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/*  762 */  { itemToBeCoded,                NOPARAM,                     "0762",                                        "0762",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  763 */  { itemToBeCoded,                NOPARAM,                     "0763",                                        "0763",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  764 */  { itemToBeCoded,                NOPARAM,                     "0764",                                        "0764",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  765 */  { itemToBeCoded,                NOPARAM,                     "0765",                                        "0765",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  766 */  { itemToBeCoded,                NOPARAM,                     "0766",                                        "0766",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  767 */  { itemToBeCoded,                NOPARAM,                     "0767",                                        "0767",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  768 */  { addItemToBuffer,              ITM_a_MACRON,                STD_a_MACRON,                                  STD_a_MACRON,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  769 */  { addItemToBuffer,              ITM_a_ACUTE,                 STD_a_ACUTE,                                   STD_a_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  770 */  { addItemToBuffer,              ITM_a_BREVE,                 STD_a_BREVE,                                   STD_a_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  771 */  { addItemToBuffer,              ITM_a_GRAVE,                 STD_a_GRAVE,                                   STD_a_GRAVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  772 */  { addItemToBuffer,              ITM_a_DIARESIS,              STD_a_DIARESIS,                                STD_a_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  773 */  { addItemToBuffer,              ITM_a_TILDE,                 STD_a_TILDE,                                   STD_a_TILDE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  774 */  { addItemToBuffer,              ITM_a_CIRC,                  STD_a_CIRC,                                    STD_a_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  775 */  { addItemToBuffer,              ITM_a_RING,                  STD_a_RING,                                    STD_a_RING,                                    (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  776 */  { addItemToBuffer,              ITM_ae,                      STD_ae,                                        STD_ae,                                        (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  777 */  { addItemToBuffer,              ITM_a_OGONEK,                STD_a_OGONEK,                                  STD_a_OGONEK,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  778 */  { addItemToBuffer,              ITM_c_ACUTE,                 STD_c_ACUTE,                                   STD_c_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  779 */  { addItemToBuffer,              ITM_c_CARON,                 STD_c_CARON,                                   STD_c_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  780 */  { addItemToBuffer,              ITM_c_CEDILLA,               STD_c_CEDILLA,                                 STD_c_CEDILLA,                                 (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  781 */  { addItemToBuffer,              ITM_d_STROKE,                STD_d_STROKE,                                  STD_d_STROKE,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  782 */  { addItemToBuffer,              ITM_d_APOSTROPHE,            STD_d_APOSTROPHE,                              STD_d_APOSTROPHE,                              (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  783 */  { addItemToBuffer,              ITM_e_MACRON,                STD_e_MACRON,                                  STD_e_MACRON,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  784 */  { addItemToBuffer,              ITM_e_ACUTE,                 STD_e_ACUTE,                                   STD_e_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  785 */  { addItemToBuffer,              ITM_e_BREVE,                 STD_e_BREVE,                                   STD_e_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  786 */  { addItemToBuffer,              ITM_e_GRAVE,                 STD_e_GRAVE,                                   STD_e_GRAVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  787 */  { addItemToBuffer,              ITM_e_DIARESIS,              STD_e_DIARESIS,                                STD_e_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  788 */  { addItemToBuffer,              ITM_e_CIRC,                  STD_e_CIRC,                                    STD_e_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  789 */  { addItemToBuffer,              ITM_e_OGONEK,                STD_e_OGONEK,                                  STD_e_OGONEK,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  790 */  { addItemToBuffer,              ITM_g_BREVE,                 STD_g_BREVE,                                   STD_g_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  791 */  { addItemToBuffer,              ITM_h_STROKE,                "",                                            STD_h_STROKE,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  792 */  { addItemToBuffer,              ITM_i_MACRON,                STD_i_MACRON,                                  STD_i_MACRON,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  793 */  { addItemToBuffer,              ITM_i_ACUTE,                 STD_i_ACUTE,                                   STD_i_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  794 */  { addItemToBuffer,              ITM_i_BREVE,                 STD_i_BREVE,                                   STD_i_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  795 */  { addItemToBuffer,              ITM_i_GRAVE,                 STD_i_GRAVE,                                   STD_i_GRAVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  796 */  { addItemToBuffer,              ITM_i_DIARESIS,              STD_i_DIARESIS,                                STD_i_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  797 */  { addItemToBuffer,              ITM_i_CIRC,                  STD_i_CIRC,                                    STD_i_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  798 */  { addItemToBuffer,              ITM_i_OGONEK,                STD_i_OGONEK,                                  STD_i_OGONEK,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  799 */  { addItemToBuffer,              ITM_i_DOT,                   "i",                                           "i",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  800 */  { addItemToBuffer,              ITM_i_DOTLESS,               STD_i_DOTLESS,                                 STD_i_DOTLESS,                                 (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  801 */  { addItemToBuffer,              ITM_l_STROKE,                STD_l_STROKE,                                  STD_l_STROKE,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  802 */  { addItemToBuffer,              ITM_l_ACUTE,                 STD_l_ACUTE,                                   STD_l_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  803 */  { addItemToBuffer,              ITM_l_APOSTROPHE,            STD_l_APOSTROPHE,                              STD_l_APOSTROPHE,                              (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  804 */  { addItemToBuffer,              ITM_n_ACUTE,                 STD_n_ACUTE,                                   STD_n_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  805 */  { addItemToBuffer,              ITM_n_CARON,                 STD_n_CARON,                                   STD_n_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  806 */  { addItemToBuffer,              ITM_n_TILDE,                 STD_n_TILDE,                                   STD_n_TILDE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  807 */  { addItemToBuffer,              ITM_o_MACRON,                STD_o_MACRON,                                  STD_o_MACRON,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  808 */  { addItemToBuffer,              ITM_o_ACUTE,                 STD_o_ACUTE,                                   STD_o_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  809 */  { addItemToBuffer,              ITM_o_BREVE,                 STD_o_BREVE,                                   STD_o_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  810 */  { addItemToBuffer,              ITM_o_GRAVE,                 STD_o_GRAVE,                                   STD_o_GRAVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  811 */  { addItemToBuffer,              ITM_o_DIARESIS,              STD_o_DIARESIS,                                STD_o_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  812 */  { addItemToBuffer,              ITM_o_TILDE,                 STD_o_TILDE,                                   STD_o_TILDE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  813 */  { addItemToBuffer,              ITM_o_CIRC,                  STD_o_CIRC,                                    STD_o_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  814 */  { addItemToBuffer,              ITM_o_STROKE,                STD_o_STROKE,                                  STD_o_STROKE,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  815 */  { addItemToBuffer,              ITM_oe,                      STD_oe,                                        STD_oe,                                        (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  816 */  { addItemToBuffer,              ITM_r_CARON,                 STD_r_CARON,                                   STD_r_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  817 */  { addItemToBuffer,              ITM_r_ACUTE,                 STD_r_ACUTE,                                   STD_r_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  818 */  { addItemToBuffer,              ITM_s_SHARP,                 STD_s_SHARP,                                   STD_s_SHARP,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  819 */  { addItemToBuffer,              ITM_s_ACUTE,                 STD_s_ACUTE,                                   STD_s_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  820 */  { addItemToBuffer,              ITM_s_CARON,                 STD_s_CARON,                                   STD_s_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  821 */  { addItemToBuffer,              ITM_s_CEDILLA,               STD_s_CEDILLA,                                 STD_s_CEDILLA,                                 (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  822 */  { addItemToBuffer,              ITM_t_APOSTROPHE,            STD_t_APOSTROPHE,                              STD_t_APOSTROPHE,                              (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  823 */  { addItemToBuffer,              ITM_t_CEDILLA,               STD_t_CEDILLA,                                 STD_t_CEDILLA,                                 (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  824 */  { addItemToBuffer,              ITM_u_MACRON,                STD_u_MACRON,                                  STD_u_MACRON,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  825 */  { addItemToBuffer,              ITM_u_ACUTE,                 STD_u_ACUTE,                                   STD_u_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  826 */  { addItemToBuffer,              ITM_u_BREVE,                 STD_u_BREVE,                                   STD_u_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  827 */  { addItemToBuffer,              ITM_u_GRAVE,                 STD_u_GRAVE,                                   STD_u_GRAVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  828 */  { addItemToBuffer,              ITM_u_DIARESIS,              STD_u_DIARESIS,                                STD_u_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  829 */  { addItemToBuffer,              ITM_u_TILDE,                 STD_u_TILDE,                                   STD_u_TILDE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  830 */  { addItemToBuffer,              ITM_u_CIRC,                  STD_u_CIRC,                                    STD_u_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  831 */  { addItemToBuffer,              ITM_u_RING,                  STD_u_RING,                                    STD_u_RING,                                    (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  832 */  { addItemToBuffer,              ITM_w_CIRC,                  STD_w_CIRC,                                    STD_w_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  833 */  { addItemToBuffer,              ITM_x_BAR,                   "",                                            STD_x_BAR,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  834 */  { addItemToBuffer,              ITM_x_CIRC,                  "",                                            STD_x_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  835 */  { addItemToBuffer,              ITM_y_BAR,                   "",                                            STD_y_BAR,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  836 */  { addItemToBuffer,              ITM_y_CIRC,                  STD_y_CIRC,                                    STD_y_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  837 */  { addItemToBuffer,              ITM_y_ACUTE,                 STD_y_ACUTE,                                   STD_y_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  838 */  { addItemToBuffer,              ITM_y_DIARESIS,              STD_y_DIARESIS,                                STD_y_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  839 */  { addItemToBuffer,              ITM_z_ACUTE,                 STD_z_ACUTE,                                   STD_z_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  840 */  { addItemToBuffer,              ITM_z_CARON,                 STD_z_CARON,                                   STD_z_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  841 */  { addItemToBuffer,              ITM_z_DOT,                   STD_z_DOT,                                     STD_z_DOT,                                     (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/*  842 */  { itemToBeCoded,                NOPARAM,                     "0842",                                        "0842",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  843 */  { itemToBeCoded,                NOPARAM,                     "0843",                                        "0843",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  844 */  { itemToBeCoded,                NOPARAM,                     "0844",                                        "0844",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  845 */  { itemToBeCoded,                NOPARAM,                     "0845",                                        "0845",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  846 */  { itemToBeCoded,                NOPARAM,                     "0846",                                        "0846",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  847 */  { itemToBeCoded,                NOPARAM,                     "0847",                                        "0847",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  848 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_alpha,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  849 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_delta,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  850 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_mu,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  851 */  { addItemToBuffer,              ITM_SUB_SUN,                 "",                                            STD_SUB_SUN,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  852 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_SUN_b,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  853 */  { addItemToBuffer,              ITM_SUB_EARTH,               "",                                            STD_SUB_EARTH,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  854 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_EARTH_b,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  855 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_PLUS,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  856 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_MINUS,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  857 */  { addItemToBuffer,              ITM_SUB_INFINITY,            "",                                            STD_SUB_INFINITY,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  858 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_0,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  859 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_1,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  860 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_2,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  861 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_3,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  862 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_4,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  863 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_5,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  864 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_6,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  865 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_7,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  866 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_8,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  867 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_9,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  868 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_10,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  869 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_A,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  870 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_B,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  871 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_C,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  872 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_D,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  873 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_E,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  874 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_F,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  875 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_G,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  876 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_H,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  877 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_I,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  878 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_J,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  879 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_K,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  880 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_L,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  881 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_M,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  882 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_N,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  883 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_O,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  884 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_P,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  885 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_Q,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  886 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_R,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  887 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_S,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  888 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_T,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  889 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_U,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  890 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_V,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  891 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_W,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  892 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_X,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  893 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_Y,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  894 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_Z,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  895 */  { addItemToBuffer,              ITM_SUB_E_OUTLINE,           "",                                            STD_SUB_E_OUTLINE,                             (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  896 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_a,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  897 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_b,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  898 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_c,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  899 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_d,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  900 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_e,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  901 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_h,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  902 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_i,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  903 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_j,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  904 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_k,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  905 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_l,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  906 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_m,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  907 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_n,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  908 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_o,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  909 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_p,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  910 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_q,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  911 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_s,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  912 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_t,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  913 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_u,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  914 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_v,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  915 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_w,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  916 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_x,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  917 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_y,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  918 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_z,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  919 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_a_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  920 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_e_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  921 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_k_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  922 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_l_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  923 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_m_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  924 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_n_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  925 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_o_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  926 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_p_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  927 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_s_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  928 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_u_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  929 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_x_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  930 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_PLUS,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  931 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_MINUS,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  932 */  { addItemToBuffer,              ITM_SUP_MINUS_1,             "",                                            STD_SUP_MINUS_1,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  933 */  { addItemToBuffer,              ITM_SUP_INFINITY,            "",                                            STD_SUP_INFINITY,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  934 */  { addItemToBuffer,              ITM_SUP_ASTERISK,            "",                                            STD_SUP_ASTERISK,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  935 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_0,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  936 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_1,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  937 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_2,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  938 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_3,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  939 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_4,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  940 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_5,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  941 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_6,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  942 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_7,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  943 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_8,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  944 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_9,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  945 */  { itemToBeCoded,                NOPARAM,                     "0945",                                        "0945",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  946 */  { addItemToBuffer,              ITM_SUP_T,                   "",                                            STD_SUP_T,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  947 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_a,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  948 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_f,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  949 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_g,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  950 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_h,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  951 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_r,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  952 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_x,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  953 */  { addItemToBuffer,              ITM_SPACE,                   "",                                            STD_SPACE,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  954 */  { addItemToBuffer,              ITM_EXCLAMATION_MARK,        "",                                            STD_EXCLAMATION_MARK,                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  955 */  { addItemToBuffer,              ITM_DOUBLE_QUOTE,            "",                                            STD_DOUBLE_QUOTE,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  956 */  { addItemToBuffer,              ITM_NUMBER_SIGN,             "",                                            STD_NUMBER_SIGN,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  957 */  { addItemToBuffer,              ITM_DOLLAR,                  "",                                            STD_DOLLAR,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  958 */  { addItemToBuffer,              ITM_PERCENT,                 "",                                            STD_PERCENT,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  959 */  { addItemToBuffer,              ITM_AMPERSAND,               "",                                            STD_AMPERSAND,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  960 */  { addItemToBuffer,              ITM_QUOTE,                   "",                                            STD_QUOTE,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  961 */  { addItemToBuffer,              ITM_LEFT_PARENTHESIS,        "",                                            STD_LEFT_PARENTHESIS,                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  962 */  { addItemToBuffer,              ITM_RIGHT_PARENTHESIS,       "",                                            STD_RIGHT_PARENTHESIS,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  963 */  { addItemToBuffer,              ITM_ASTERISK,                "",                                            STD_ASTERISK,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  964 */  { itemToBeCoded,                NOPARAM,                     "0964",                                        "0964",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/*  965 */  { addItemToBuffer,              ITM_PLUS,                    "",                                            STD_PLUS,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  966 */  { addItemToBuffer,              ITM_COMMA,                   "",                                            STD_COMMA,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  967 */  { addItemToBuffer,              ITM_MINUS,                   "",                                            STD_MINUS,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  968 */  { addItemToBuffer,              ITM_PERIOD,                  "",                                            STD_PERIOD,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  969 */  { addItemToBuffer,              ITM_SLASH,                   "",                                            STD_SLASH,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  970 */  { addItemToBuffer,              ITM_COLON,                   "",                                            STD_COLON,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  971 */  { addItemToBuffer,              ITM_SEMICOLON,               "",                                            STD_SEMICOLON,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  972 */  { addItemToBuffer,              ITM_LESS_THAN,               "",                                            STD_LESS_THAN,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  973 */  { addItemToBuffer,              ITM_EQUAL,                   "",                                            STD_EQUAL,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  974 */  { addItemToBuffer,              ITM_GREATER_THAN,            "",                                            STD_GREATER_THAN,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  975 */  { addItemToBuffer,              ITM_QUESTION_MARK,           "",                                            STD_QUESTION_MARK,                             (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  976 */  { addItemToBuffer,              ITM_AT,                      "",                                            STD_AT,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  977 */  { addItemToBuffer,              ITM_LEFT_SQUARE_BRACKET,     "",                                            STD_LEFT_SQUARE_BRACKET,                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  978 */  { addItemToBuffer,              ITM_BACK_SLASH,              "",                                            STD_BACK_SLASH,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  979 */  { addItemToBuffer,              ITM_RIGHT_SQUARE_BRACKET,    "",                                            STD_RIGHT_SQUARE_BRACKET,                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  980 */  { addItemToBuffer,              ITM_CIRCUMFLEX,              "",                                            STD_CIRCUMFLEX,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  981 */  { addItemToBuffer,              ITM_UNDERSCORE,              "",                                            STD_UNDERSCORE,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  982 */  { addItemToBuffer,              ITM_LEFT_CURLY_BRACKET,      "",                                            STD_LEFT_CURLY_BRACKET,                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  983 */  { addItemToBuffer,              ITM_PIPE,                    "",                                            STD_PIPE,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  984 */  { addItemToBuffer,              ITM_RIGHT_CURLY_BRACKET,     "",                                            STD_RIGHT_CURLY_BRACKET,                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  985 */  { addItemToBuffer,              ITM_TILDE,                   "",                                            STD_TILDE,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  986 */  { addItemToBuffer,              ITM_INVERTED_EXCLAMATION_MARK, "",                                          STD_INVERTED_EXCLAMATION_MARK,                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  987 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_CENT,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  988 */  { addItemToBuffer,              ITM_POUND,                   "",                                            STD_POUND,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  989 */  { addItemToBuffer,              ITM_YEN,                     "",                                            STD_YEN,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  990 */  { addItemToBuffer,              ITM_SECTION,                 "",                                            STD_SECTION,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  991 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_OVERFLOW_CARRY,                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  992 */  { addItemToBuffer,              ITM_LEFT_DOUBLE_ANGLE,       "",                                            STD_LEFT_DOUBLE_ANGLE,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  993 */  { addItemToBuffer,              ITM_NOT,                     "",                                            STD_NOT,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  994 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_DEGREE,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  995 */  { addItemToBuffer,              ITM_PLUS_MINUS,              "",                                            STD_PLUS_MINUS,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  996 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_mu_b,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  997 */  { addItemToBuffer,              ITM_DOT,                     "",                                            STD_DOT,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  998 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_ORDINAL,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/*  999 */  { addItemToBuffer,              ITM_RIGHT_DOUBLE_ANGLE,      "",                                            STD_RIGHT_DOUBLE_ANGLE,                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1000 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_ONE_HALF,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1001 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_ONE_QUARTER,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1002 */  { addItemToBuffer,              ITM_INVERTED_QUESTION_MARK,  "",                                            STD_INVERTED_QUESTION_MARK,                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1003 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_ETH,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1004 */  { addItemToBuffer,              ITM_CROSS,                   "",                                            STD_CROSS,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1005 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_eth,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1006 */  { addItemToBuffer,              ITM_OBELUS,                  "",                                            STD_DIVIDE,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1007 */  { addItemToBuffer,              ITM_E_DOT,                   STD_E_DOT,                                     STD_E_DOT,                                     (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/* 1008 */  { addItemToBuffer,              ITM_e_DOT,                   STD_e_DOT,                                     STD_e_DOT,                                     (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/* 1009 */  { addItemToBuffer,              ITM_E_CARON,                 STD_E_CARON,                                   STD_E_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/* 1010 */  { addItemToBuffer,              ITM_e_CARON,                 STD_e_CARON,                                   STD_e_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/* 1011 */  { addItemToBuffer,              ITM_R_ACUTE,                 STD_R_ACUTE,                                   STD_R_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/* 1012 */  { addItemToBuffer,              ITM_R_CARON,                 STD_R_CARON,                                   STD_R_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/* 1013 */  { addItemToBuffer,              ITM_U_OGONEK,                STD_U_OGONEK,                                  STD_U_OGONEK,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED},
/* 1014 */  { addItemToBuffer,              ITM_u_OGONEK,                STD_u_OGONEK,                                  STD_u_OGONEK,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED},
/* 1015 */  { addItemToBuffer,              ITM_y_UNDER_ROOT,            "",                                            STD_y_UNDER_ROOT,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1016 */  { addItemToBuffer,              ITM_x_UNDER_ROOT,            "",                                            STD_x_UNDER_ROOT,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1017 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SPACE_EM,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1018 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SPACE_3_PER_EM,                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1019 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SPACE_4_PER_EM,                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1020 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SPACE_6_PER_EM,                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1021 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SPACE_FIGURE,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1022 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SPACE_PUNCTUATION,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1023 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SPACE_HAIR,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1024 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_LEFT_SINGLE_QUOTE,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1025 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_RIGHT_SINGLE_QUOTE,                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1026 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SINGLE_LOW_QUOTE,                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1027 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SINGLE_HIGH_QUOTE,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1028 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_LEFT_DOUBLE_QUOTE,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1029 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_RIGHT_DOUBLE_QUOTE,                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1030 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_DOUBLE_LOW_QUOTE,                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1031 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_DOUBLE_HIGH_QUOTE,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1032 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_ELLIPSIS,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1033 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_ONE,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1034 */  { addItemToBuffer,              ITM_EURO,                    "",                                            STD_EURO,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1035 */  { addItemToBuffer,              ITM_COMPLEX_C,               "",                                            STD_COMPLEX_C,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1036 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_PLANCK,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1037 */  { addItemToBuffer,              ITM_PLANCK_2PI,              "",                                            STD_PLANCK_2PI,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1038 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_NATURAL_N,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1039 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_RATIONAL_Q,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1040 */  { addItemToBuffer,              ITM_REAL_R,                  "",                                            STD_REAL_R,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1041 */  { addItemToBuffer,              ITM_LEFT_ARROW,              "",                                            STD_LEFT_ARROW,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1042 */  { addItemToBuffer,              ITM_UP_ARROW,                "",                                            STD_UP_ARROW,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1043 */  { itemToBeCoded,                NOPARAM,                     "1043",                                        "1043",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1044 */  { addItemToBuffer,              ITM_RIGHT_ARROW,             "",                                            STD_RIGHT_ARROW,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1045 */  { addItemToBuffer,              ITM_DOWN_ARROW,              "",                                            STD_DOWN_ARROW,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1046 */  { itemToBeCoded,                NOPARAM,                     "1046",                                        "1046",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1047 */  { addItemToBuffer,              ITM_SERIAL_IO,               "",                                            STD_SERIAL_IO,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1048 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_RIGHT_SHORT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1049 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_LEFT_RIGHT_ARROWS,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1050 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_BST,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1051 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SST,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1052 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_HAMBURGER,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1053 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_UNDO,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1054 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_FOR_ALL,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1055 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_COMPLEMENT,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1056 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_PARTIAL_DIFF,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1057 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_THERE_EXISTS,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1058 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_THERE_DOES_NOT_EXIST,                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1059 */  { addItemToBuffer,              ITM_EMPTY_SET,               "",                                            STD_EMPTY_SET,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1060 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_INCREMENT,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1061 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_NABLA,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1062 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_ELEMENT_OF,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1063 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_NOT_ELEMENT_OF,                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1064 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_CONTAINS,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1065 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_DOES_NOT_CONTAIN,                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1066 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_ZERO,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1067 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_PRODUCT,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1068 */  { itemToBeCoded,                NOPARAM,                     "1068",                                        "1068",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1069 */  { itemToBeCoded,                NOPARAM,                     "1069",                                        "1069",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1070 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_MINUS_PLUS,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1071 */  { itemToBeCoded,                NOPARAM,                     "1071",                                        "1071",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1072 */  { itemToBeCoded,                NOPARAM,                     "1072",                                        "1072",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1073 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_RING,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1074 */  { addItemToBuffer,              ITM_BULLET,                  "",                                            STD_BULLET,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1075 */  { addItemToBuffer,              ITM_SQUARE_ROOT,             "",                                            STD_SQUARE_ROOT,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1076 */  { addItemToBuffer,              ITM_CUBE_ROOT,               "",                                            STD_CUBE_ROOT,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1077 */  { addItemToBuffer,              ITM_xTH_ROOT,                "",                                            STD_xTH_ROOT,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1078 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_PROPORTIONAL,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1079 */  { addItemToBuffer,              ITM_INFINITY,                "",                                            STD_INFINITY,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1080 */  { addItemToBuffer,              ITM_RIGHT_ANGLE,             "",                                            STD_RIGHT_ANGLE,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1081 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_ANGLE,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1082 */  { addItemToBuffer,              ITM_MEASURED_ANGLE,          "",                                            STD_MEASURED_ANGLE,                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1083 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_DIVIDES,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1084 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_DOES_NOT_DIVIDE,                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1085 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_PARALLEL,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1086 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_NOT_PARALLEL,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1087 */  { addItemToBuffer,              ITM_AND,                     "",                                            STD_AND,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1088 */  { addItemToBuffer,              ITM_OR,                      "",                                            STD_OR,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1089 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_INTERSECTION,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1090 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_UNION,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1091 */  { addItemToBuffer,              ITM_INTEGRAL_SIGN,           "",                                            STD_INTEGRAL,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1092 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_DOUBLE_INTEGRAL,                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1093 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_CONTOUR_INTEGRAL,                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1094 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SURFACE_INTEGRAL,                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1095 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_RATIO,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1096 */  { addItemToBuffer,              ITM_CHECK_MARK,              "",                                            STD_CHECK_MARK,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1097 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_ASYMPOTICALLY_EQUAL,                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1098 */  { addItemToBuffer,              ITM_ALMOST_EQUAL,            "",                                            STD_ALMOST_EQUAL,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1099 */  { addItemToBuffer,              ITM_COLON_EQUALS,            "",                                            STD_COLON_EQUALS,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1100 */  { addItemToBuffer,              ITM_CORRESPONDS_TO,          "",                                            STD_CORRESPONDS_TO,                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1101 */  { addItemToBuffer,              ITM_ESTIMATES,               "",                                            STD_ESTIMATES,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1102 */  { addItemToBuffer,              ITM_NOT_EQUAL,               "",                                            STD_NOT_EQUAL,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1103 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_IDENTICAL_TO,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1104 */  { addItemToBuffer,              ITM_LESS_EQUAL,              "",                                            STD_LESS_EQUAL,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1105 */  { addItemToBuffer,              ITM_GREATER_EQUAL,           "",                                            STD_GREATER_EQUAL,                             (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1106 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_MUCH_LESS,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1107 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_MUCH_GREATER,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1108 */  { addItemToBuffer,              ITM_SUN,                     "",                                            STD_SUN,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1109 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_DOWN_TACK,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1110 */  { addItemToBuffer,              ITM_PERPENDICULAR,           "",                                            STD_PERPENDICULAR,                             (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1111 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_XOR,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1112 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_NAND,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1113 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_NOR,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1114 */  { addItemToBuffer,              ITM_WATCH,                   "",                                            STD_WATCH,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1115 */  { addItemToBuffer,              ITM_HOURGLASS,               "",                                            STD_HOURGLASS,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1116 */  { addItemToBuffer,              ITM_PRINTER,                 "",                                            STD_PRINTER,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1117 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_MAT_TL,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1118 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_MAT_ML,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1119 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_MAT_BL,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1120 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_MAT_TR,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1121 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_MAT_MR,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1122 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_MAT_BR,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1123 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_OBLIQUE1,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1124 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_OBLIQUE2,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1125 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_OBLIQUE3,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1126 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_OBLIQUE4,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1127 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_CURSOR,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1128 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_PERIOD34,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1129 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_COMMA34,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1130 */  { addItemToBuffer,              ITM_BATTERY,                 "",                                            STD_BATTERY,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1131 */  { addItemToBuffer,              ITM_PGM_BEGIN,               "",                                            STD_PGM_BEGIN,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1132 */  { addItemToBuffer,              ITM_USER_MODE,               "",                                            STD_USER_MODE,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1133 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_UK,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1134 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_US,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1135 */  { addItemToBuffer,              ITM_NEG_EXCLAMATION_MARK,    "",                                            STD_NEG_EXCLAMATION_MARK,                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1136 */  { addItemToBuffer,              ITM_ex,                      "",                                            STD_LEFT_RIGHT_ARROWS,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1137 */  { addItemToBuffer,              ITM_Max,                     "",                                            "Max",                                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1138 */  { addItemToBuffer,              ITM_Min,                     "",                                            "Min",                                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1139 */  { addItemToBuffer,              ITM_Config,                  "",                                            "Config",                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1140 */  { addItemToBuffer,              ITM_Stack,                   "",                                            "Stack",                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1141 */  { addItemToBuffer,              ITM_dddEL,                   "",                                            STD_ELLIPSIS "EL",                             (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1142 */  { addItemToBuffer,              ITM_dddIJ,                   "",                                            STD_ELLIPSIS "IJ",                             (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1143 */  { addItemToBuffer,              ITM_0P,                      "",                                            "0.",                                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1144 */  { addItemToBuffer,              ITM_1P,                      "",                                            "1.",                                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1145 */  { addItemToBuffer,              ITM_EXPONENT,                "",                                            "E",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1146 */  { addItemToBuffer,              NOPARAM,                     "HEX",                                         "H",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1147 */  { fnGoToRow,                    TM_VALUE,                    "GOTO Row",                                    "GOTO",                                        (0 << TAM_MAX_BITS) |  9999, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1148 */  { fnGoToColumn,                 TM_VALUE,                    "GOTO Column",                                 "GOTO",                                        (0 << TAM_MAX_BITS) |  9999, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1149 */  { itemToBeCoded,                NOPARAM,                     "1149",                                        "1149",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1150 */  { itemToBeCoded,                NOPARAM,                     "1150",                                        "1150",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1151 */  { itemToBeCoded,                NOPARAM,                     "1151",                                        "1151",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1152 */  { itemToBeCoded,                NOPARAM,                     "1152",                                        "1152",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1153 */  { itemToBeCoded,                NOPARAM,                     "1153",                                        "1153",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1154 */  { itemToBeCoded,                NOPARAM,                     "1154",                                        "1154",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1155 */  { itemToBeCoded,                NOPARAM,                     "1155",                                        "1155",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1156 */  { itemToBeCoded,                NOPARAM,                     "1156",                                        "1156",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1157 */  { itemToBeCoded,                NOPARAM,                     "1157",                                        "1157",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1158 */  { itemToBeCoded,                NOPARAM,                     "1158",                                        "1158",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1159 */  { itemToBeCoded,                NOPARAM,                     "1159",                                        "1159",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1160 */  { itemToBeCoded,                NOPARAM,                     "1160",                                        "1160",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1161 */  { itemToBeCoded,                NOPARAM,                     "1161",                                        "1161",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1162 */  { itemToBeCoded,                NOPARAM,                     "1162",                                        "1162",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1163 */  { itemToBeCoded,                NOPARAM,                     "1163",                                        "1163",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1164 */  { itemToBeCoded,                NOPARAM,                     "1164",                                        "1164",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1165 */  { itemToBeCoded,                NOPARAM,                     "1165",                                        "1165",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1166 */  { itemToBeCoded,                NOPARAM,                     "1166",                                        "1166",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1167 */  { itemToBeCoded,                NOPARAM,                     "1167",                                        "1167",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1168 */  { itemToBeCoded,                NOPARAM,                     "1168",                                        "1168",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1169 */  { itemToBeCoded,                NOPARAM,                     "1169",                                        "1169",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1170 */  { itemToBeCoded,                NOPARAM,                     "1170",                                        "1170",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1171 */  { itemToBeCoded,                NOPARAM,                     "1171",                                        "1171",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1172 */  { itemToBeCoded,                NOPARAM,                     "1172",                                        "1172",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1173 */  { itemToBeCoded,                NOPARAM,                     "1173",                                        "1173",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1174 */  { itemToBeCoded,                NOPARAM,                     "1174",                                        "1174",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},


// Reserved variables
/* 1175 */  { addItemToBuffer,              REGISTER_X,                  "X",                                           "X",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED}, // Do
/* 1176 */  { addItemToBuffer,              REGISTER_Y,                  "Y",                                           "Y",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED}, // not
/* 1177 */  { addItemToBuffer,              REGISTER_Z,                  "Z",                                           "Z",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED}, // change
/* 1178 */  { addItemToBuffer,              REGISTER_T,                  "T",                                           "T",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED}, // the
/* 1179 */  { addItemToBuffer,              REGISTER_A,                  "A",                                           "A",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED}, // order
/* 1180 */  { addItemToBuffer,              REGISTER_B,                  "B",                                           "B",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED}, // of
/* 1181 */  { addItemToBuffer,              REGISTER_C,                  "C",                                           "C",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED}, // this
/* 1182 */  { addItemToBuffer,              REGISTER_D,                  "D",                                           "D",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED}, // 3
/* 1183 */  { addItemToBuffer,              REGISTER_L,                  "L",                                           "L",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED}, // 0
/* 1184 */  { addItemToBuffer,              REGISTER_I,                  "I",                                           "I",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED}, // reserved
/* 1185 */  { addItemToBuffer,              REGISTER_J,                  "J",                                           "J",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED}, // variables
/* 1186 */  { addItemToBuffer,              REGISTER_K,                  "K",                                           "K",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED}, // !
/* 1187 */  { itemToBeCoded,                NOPARAM,                     "ADM",                                         "ADM",                                         (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED},
/* 1188 */  { itemToBeCoded,                NOPARAM,                     "DENMAX",                                      "DENMAX",                                      (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED},
/* 1189 */  { itemToBeCoded,                NOPARAM,                     "ISM",                                         "ISM",                                         (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED},
/* 1190 */  { itemToBeCoded,                NOPARAM,                     "REALDF",                                      "REALDF",                                      (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED},
/* 1191 */  { itemToBeCoded,                NOPARAM,                     "#DEC",                                        "#DEC",                                        (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED},
/* 1192 */  { itemToBeCoded,                NOPARAM,                     "ACC",                                         "ACC",                                         (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED},
/* 1193 */  { itemToBeCoded,                NOPARAM,                     STD_UP_ARROW "Lim",                            STD_UP_ARROW "Lim",                            (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED},
/* 1194 */  { itemToBeCoded,                NOPARAM,                     STD_DOWN_ARROW "Lim",                          STD_DOWN_ARROW "Lim",                          (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED},
/* 1195 */  { itemToBeCoded,                NOPARAM,                     "FV",                                          "FV",                                          (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED},
/* 1196 */  { itemToBeCoded,                NOPARAM,                     "i%/a",                                        "i%/a",                                        (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED},
/* 1197 */  { itemToBeCoded,                NOPARAM,                     "NPER",                                        "n" STD_SUB_P STD_SUB_E STD_SUB_R,             (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED},
/* 1198 */  { itemToBeCoded,                NOPARAM,                     "PER/a",                                       "per/a",                                       (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED},
/* 1199 */  { itemToBeCoded,                NOPARAM,                     "PMT",                                         "PMT",                                         (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED},
/* 1200 */  { itemToBeCoded,                NOPARAM,                     "PV",                                          "PV",                                          (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED},
/* 1201 */  { itemToBeCoded,                NOPARAM,                     "GRAMOD",                                      "GRAMOD",                                      (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED},
/* 1202 */  { fnEditLinearEquationMatrixA,  NOPARAM,                     "Mat_A",                                       "Mat A",                                       (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED},
/* 1203 */  { fnEditLinearEquationMatrixB,  NOPARAM,                     "Mat_B",                                       "Mat B",                                       (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED},
/* 1204 */  { itemToBeCoded,                NOPARAM,                     "Mat_X",                                       "Mat X",                                       (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED},
/* 1205 */  { itemToBeCoded,                NOPARAM,                     "1205",                                        "1205",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1206 */  { itemToBeCoded,                NOPARAM,                     "1206",                                        "1206",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},


// Probability distributions
/* 1207 */  { itemToBeCoded,                NOPARAM,                     "Binom:",                                      "Binom:",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1208 */  { fnBinomialP,                  NOPARAM,                     "Binom" STD_SUB_p,                             "Binom" STD_SUB_p,                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1209 */  { fnBinomialL,                  NOPARAM,                     "Binom" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Binom" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1210 */  { fnBinomialR,                  NOPARAM,                     "Binom" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Binom" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1211 */  { fnBinomialI,                  NOPARAM,                     "Binom" STD_SUP_MINUS_1,                       "Binom" STD_SUP_MINUS_1,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1212 */  { itemToBeCoded,                NOPARAM,                     "Cauch:",                                      "Cauch:",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1213 */  { fnCauchyP,                    NOPARAM,                     "Cauch" STD_SUB_p,                             "Cauch" STD_SUB_p,                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1214 */  { fnCauchyL,                    NOPARAM,                     "Cauch" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Cauch" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1215 */  { fnCauchyR,                    NOPARAM,                     "Cauch" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Cauch" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1216 */  { fnCauchyI,                    NOPARAM,                     "Cauch" STD_SUP_MINUS_1,                       "Cauch" STD_SUP_MINUS_1,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1217 */  { itemToBeCoded,                NOPARAM,                     "Expon:",                                      "Expon:",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1218 */  { fnExponentialP,               NOPARAM,                     "Expon" STD_SUB_p,                             "Expon" STD_SUB_p,                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1219 */  { fnExponentialL,               NOPARAM,                     "Expon" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Expon" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1220 */  { fnExponentialR,               NOPARAM,                     "Expon" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Expon" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1221 */  { fnExponentialI,               NOPARAM,                     "Expon" STD_SUP_MINUS_1,                       "Expon" STD_SUP_MINUS_1,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1222 */  { itemToBeCoded,                NOPARAM,                     "F:",                                          "F:",                                          (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1223 */  { fnF_P,                        NOPARAM,                     "F" STD_SUB_p "(x)",                           "F" STD_SUB_p "(x)",                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1224 */  { fnF_L,                        NOPARAM,                     "F" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", "F" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1225 */  { fnF_R,                        NOPARAM,                     "F" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", "F" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1226 */  { fnF_I,                        NOPARAM,                     "F" STD_SUP_MINUS_1 "(p)",                     "F" STD_SUP_MINUS_1 "(p)",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1227 */  { itemToBeCoded,                NOPARAM,                     "Geom:",                                       "Geom:",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1228 */  { fnGeometricP,                 NOPARAM,                     "Geom" STD_SUB_p,                              "Geom" STD_SUB_p,                              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1229 */  { fnGeometricL,                 NOPARAM,                     "Geom" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,    "Geom" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1230 */  { fnGeometricR,                 NOPARAM,                     "Geom" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,    "Geom" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1231 */  { fnGeometricI,                 NOPARAM,                     "Geom" STD_SUP_MINUS_1,                        "Geom" STD_SUP_MINUS_1,                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1232 */  { itemToBeCoded,                NOPARAM,                     "Hyper:",                                      "Hyper:",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1233 */  { fnHypergeometricP,            NOPARAM,                     "Hyper" STD_SUB_p,                             "Hyper" STD_SUB_p,                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1234 */  { fnHypergeometricL,            NOPARAM,                     "Hyper" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Hyper" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1235 */  { fnHypergeometricR,            NOPARAM,                     "Hyper" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Hyper" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1236 */  { fnHypergeometricI,            NOPARAM,                     "Hyper" STD_SUP_MINUS_1,                       "Hyper" STD_SUP_MINUS_1,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1237 */  { itemToBeCoded,                NOPARAM,                     "LgNrm:",                                      "LgNrm:",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1238 */  { fnLogNormalP,                 NOPARAM,                     "LgNrm" STD_SUB_p,                             "LgNrm" STD_SUB_p,                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1239 */  { fnLogNormalL,                 NOPARAM,                     "LgNrm" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "LgNrm" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1240 */  { fnLogNormalR,                 NOPARAM,                     "LgNrm" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "LgNrm" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1241 */  { fnLogNormalI,                 NOPARAM,                     "LgNrm" STD_SUP_MINUS_1,                       "LgNrm" STD_SUP_MINUS_1,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1242 */  { itemToBeCoded,                NOPARAM,                     "Logis:",                                      "Logis:",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1243 */  { fnLogisticP,                  NOPARAM,                     "Logis" STD_SUB_p,                             "Logis" STD_SUB_p,                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1244 */  { fnLogisticL,                  NOPARAM,                     "Logis" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Logis" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1245 */  { fnLogisticR,                  NOPARAM,                     "Logis" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Logis" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1246 */  { fnLogisticI,                  NOPARAM,                     "Logis" STD_SUP_MINUS_1,                       "Logis" STD_SUP_MINUS_1,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1247 */  { itemToBeCoded,                NOPARAM,                     "NBin:",                                       "NBin:",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1248 */  { fnNegBinomialP,               NOPARAM,                     "NBin" STD_SUB_p,                              "NBin" STD_SUB_p,                              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1249 */  { fnNegBinomialL,               NOPARAM,                     "NBin" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,    "NBin" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1250 */  { fnNegBinomialR,               NOPARAM,                     "NBin" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,    "NBin" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1251 */  { fnNegBinomialI,               NOPARAM,                     "NBin" STD_SUP_MINUS_1,                        "NBin" STD_SUP_MINUS_1,                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1252 */  { itemToBeCoded,                NOPARAM,                     "Norml:",                                      "Norml:",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1253 */  { fnNormalP,                    NOPARAM,                     "Norml" STD_SUB_p,                             "Norml" STD_SUB_p,                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1254 */  { fnNormalL,                    NOPARAM,                     "Norml" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Norml" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1255 */  { fnNormalR,                    NOPARAM,                     "Norml" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Norml" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1256 */  { fnNormalI,                    NOPARAM,                     "Norml" STD_SUP_MINUS_1,                       "Norml" STD_SUP_MINUS_1,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1257 */  { itemToBeCoded,                NOPARAM,                     "Poiss:",                                      "Poiss:",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1258 */  { fnPoissonP,                   NOPARAM,                     "Poiss" STD_SUB_p,                             "Poiss" STD_SUB_p,                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1259 */  { fnPoissonL,                   NOPARAM,                     "Poiss" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Poiss" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1260 */  { fnPoissonR,                   NOPARAM,                     "Poiss" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Poiss" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1261 */  { fnPoissonI,                   NOPARAM,                     "Poiss" STD_SUP_MINUS_1,                       "Poiss" STD_SUP_MINUS_1,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1262 */  { itemToBeCoded,                NOPARAM,                     "t:",                                          "t:",                                          (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1263 */  { fnT_P,                        NOPARAM,                     "t" STD_SUB_p "(x)",                           "t" STD_SUB_p "(x)",                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1264 */  { fnT_L,                        NOPARAM,                     "t" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", "t" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1265 */  { fnT_R,                        NOPARAM,                     "t" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", "t" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1266 */  { fnT_I,                        NOPARAM,                     "t" STD_SUP_MINUS_1 "(p)",                     "t" STD_SUP_MINUS_1 "(p)",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1267 */  { itemToBeCoded,                NOPARAM,                     "Weibl:",                                      "Weibl:",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1268 */  { fnWeibullP,                   NOPARAM,                     "Weibl" STD_SUB_p,                             "Weibl" STD_SUB_p,                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1269 */  { fnWeibullL,                   NOPARAM,                     "Weibl" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Weibl" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1270 */  { fnWeibullR,                   NOPARAM,                     "Weibl" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Weibl" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1271 */  { fnWeibullI,                   NOPARAM,                     "Weibl" STD_SUP_MINUS_1,                       "Weibl" STD_SUP_MINUS_1,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1272 */  { itemToBeCoded,                NOPARAM,                     STD_chi STD_SUP_2 ":",                         STD_chi STD_SUP_2 ":",                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1273 */  { fnChi2P,                      NOPARAM,                     STD_chi STD_SUP_2 STD_SUB_p "(x)",             STD_chi STD_SUP_2 STD_SUB_p "(x)",             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1274 */  { fnChi2L,                      NOPARAM,                     STD_chi STD_SUP_2 STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)",
                                                                                                                        STD_chi STD_SUP_2 STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)",
                                                                                                                                                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1275 */  { fnChi2R,                      NOPARAM,                     STD_chi STD_SUP_2 STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)",
                                                                                                                        STD_chi STD_SUP_2 STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)",
                                                                                                                                                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1276 */  { fnChi2I,                      NOPARAM,                     "(" STD_chi STD_SUP_2 ")" STD_SUP_MINUS_1,     "(" STD_chi STD_SUP_2 ")" STD_SUP_MINUS_1,     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1277 */  { itemToBeCoded,                NOPARAM,                     "1277",                                        "1277",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1278 */  { itemToBeCoded,                NOPARAM,                     "1278",                                        "1278",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1279 */  { itemToBeCoded,                NOPARAM,                     "1279",                                        "1279",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1280 */  { itemToBeCoded,                NOPARAM,                     "1280",                                        "1280",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1281 */  { itemToBeCoded,                NOPARAM,                     "1281",                                        "1281",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1282 */  { itemToBeCoded,                NOPARAM,                     "1282",                                        "1282",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1283 */  { itemToBeCoded,                NOPARAM,                     "1283",                                        "1283",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1284 */  { itemToBeCoded,                NOPARAM,                     "1284",                                        "1284",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1285 */  { itemToBeCoded,                NOPARAM,                     "1285",                                        "1285",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1286 */  { itemToBeCoded,                NOPARAM,                     "1286",                                        "1286",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1287 */  { itemToBeCoded,                NOPARAM,                     "1287",                                        "1287",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1288 */  { itemToBeCoded,                NOPARAM,                     "1288",                                        "1288",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1289 */  { itemToBeCoded,                NOPARAM,                     "1289",                                        "1289",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1290 */  { itemToBeCoded,                NOPARAM,                     "1290",                                        "1290",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1291 */  { itemToBeCoded,                NOPARAM,                     "1291",                                        "1291",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1292 */  { itemToBeCoded,                NOPARAM,                     "1292",                                        "1292",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1293 */  { itemToBeCoded,                NOPARAM,                     "1293",                                        "1293",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1294 */  { itemToBeCoded,                NOPARAM,                     "1294",                                        "1294",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1295 */  { itemToBeCoded,                NOPARAM,                     "1295",                                        "1295",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1296 */  { itemToBeCoded,                NOPARAM,                     "1296",                                        "1296",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},


// Curve fitting
/* 1297 */  { fnCurveFitting,               TM_VALUE,                    "BestF",                                       "BestF",                                       (0 << TAM_MAX_BITS) |   510, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1298 */  { fnCurveFitting,               CF_EXPONENTIAL_FITTING_EX,   "ExpF",                                        "ExpF",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1299 */  { fnCurveFitting,               CF_LINEAR_FITTING_EX,        "LinF",                                        "LinF",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1300 */  { fnCurveFitting,               CF_LOGARITHMIC_FITTING_EX,   "LogF",                                        "LogF",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1301 */  { fnCurveFitting,               CF_ORTHOGONAL_FITTING_EX,    "OrthoF",                                      "OrthoF",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1302 */  { fnCurveFitting,               CF_POWER_FITTING_EX,         "PowerF",                                      "PowerF",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1303 */  { fnCurveFitting,               CF_GAUSS_FITTING_EX,         "GaussF",                                      "GaussF",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1304 */  { fnCurveFitting,               CF_CAUCHY_FITTING_EX,        "CauchF",                                      "CauchF",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1305 */  { fnCurveFitting,               CF_PARABOLIC_FITTING_EX,     "ParabF",                                      "ParabF",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1306 */  { fnCurveFitting,               CF_HYPERBOLIC_FITTING_EX,    "HypF",                                        "HypF",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1307 */  { fnCurveFitting,               CF_ROOT_FITTING_EX,          "RootF",                                       "RootF",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1308 */  { itemToBeCoded,                NOPARAM,                     "1308",                                        "1308",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/* 1309 */  { itemToBeCoded,                NOPARAM,                     "1309",                                        "1309",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/* 1310 */  { itemToBeCoded,                NOPARAM,                     "1310",                                        "1310",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/* 1311 */  { itemToBeCoded,                NOPARAM,                     "1311",                                        "1311",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/* 1312 */  { itemToBeCoded,                NOPARAM,                     "1312",                                        "1312",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},


// Menus
/* 1313 */  { itemToBeCoded,                NOPARAM,                     "ADV",                                         "ADV",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1314 */  { itemToBeCoded,                NOPARAM,                     "ANGLES",                                      "ANGLES",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1315 */  { itemToBeCoded,                NOPARAM,                     "PRINT",                                       "PRINT",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1316 */  { itemToBeCoded,                NOPARAM,                     "A:",                                          "A:",                                          (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1317 */  { itemToBeCoded,                NOPARAM,                     "BITS",                                        "BITS",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1318 */  { itemToBeCoded,                NOPARAM,                     "CATALOG",                                     "CATALOG",                                     (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1319 */  { itemToBeCoded,                NOPARAM,                     "CHARS",                                       "CHARS",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1320 */  { itemToBeCoded,                NOPARAM,                     "CLK",                                         "CLK",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1321 */  { itemToBeCoded,                NOPARAM,                     "CLR",                                         "CLR",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1322 */  { itemToBeCoded,                NOPARAM,                     "CONST",                                       "CONST",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1323 */  { itemToBeCoded,                NOPARAM,                     "CPX",                                         "CPX",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1324 */  { itemToBeCoded,                NOPARAM,                     "CPXS",                                        "CPXS",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1325 */  { itemToBeCoded,                NOPARAM,                     "DATES",                                       "DATES",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1326 */  { itemToBeCoded,                NOPARAM,                     "DISP",                                        "DISP",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1327 */  { itemToBeCoded,                NOPARAM,                     "EQN",                                         "EQN",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1328 */  { itemToBeCoded,                NOPARAM,                     "EXP",                                         "EXP",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1329 */  { itemToBeCoded,                NOPARAM,                     "E:",                                          "E:",                                          (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1330 */  { itemToBeCoded,                NOPARAM,                     "FCNS",                                        "FCNS",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1331 */  { itemToBeCoded,                NOPARAM,                     "FIN",                                         "FIN",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1332 */  { itemToBeCoded,                NOPARAM,                     "S.INTS",                                      "S.INTS",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1333 */  { itemToBeCoded,                NOPARAM,                     "FLAGS",                                       "FLAGS",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1334 */  { itemToBeCoded,                NOPARAM,                     "FLASH",                                       "FLASH",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1335 */  { itemToBeCoded,                NOPARAM,                     "f'",                                          "f'",                                          (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1336 */  { itemToBeCoded,                NOPARAM,                     "f\"",                                         "f\"",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1337 */  { itemToBeCoded,                NOPARAM,                     "F&p:",                                        "F&p:",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1338 */  { itemToBeCoded,                NOPARAM,                     "L.INTS",                                      "L.INTS",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1339 */  { itemToBeCoded,                NOPARAM,                     "INFO",                                        "INFO",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1340 */  { itemToBeCoded,                NOPARAM,                     "INTS",                                        "INTS",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1341 */  { itemToBeCoded,                NOPARAM,                     "I/O",                                         "I/O",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1342 */  { itemToBeCoded,                NOPARAM,                     "LOOP",                                        "LOOP",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1343 */  { itemToBeCoded,                NOPARAM,                     "MATRS",                                       "MATRS",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1344 */  { itemToBeCoded,                NOPARAM,                     "MATX",                                        "MATX",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1345 */  { itemToBeCoded,                NOPARAM,                     "MENUS",                                       "MENUS",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1346 */  { itemToBeCoded,                NOPARAM,                     "MODE",                                        "MODE",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1347 */  { itemToBeCoded,                NOPARAM,                     "M.SIMQ",                                      "M.SIMQ",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1348 */  { itemToBeCoded,                NOPARAM,                     "M.EDIT",                                      "M.EDIT",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1349 */  { itemToBeCoded,                NOPARAM,                     "MyMenu",                                      "MyMenu",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1350 */  { itemToBeCoded,                NOPARAM,                     "My" STD_alpha,                                "My" STD_alpha,                                (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1351 */  { itemToBeCoded,                NOPARAM,                     "m:",                                          "m:",                                          (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1352 */  { itemToBeCoded,                NOPARAM,                     "ORTHOG",                                      "Orthog",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1353 */  { itemToBeCoded,                NOPARAM,                     "PARTS",                                       "PARTS",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1354 */  { itemToBeCoded,                NOPARAM,                     "PROB",                                        "PROB",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1355 */  { itemToBeCoded,                NOPARAM,                     "PROGS",                                       "PROGS",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1356 */  { itemToBeCoded,                NOPARAM,                     "P.FN",                                        "P.FN",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1357 */  { itemToBeCoded,                NOPARAM,                     "P.FN2",                                       "P.FN2",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1358 */  { itemToBeCoded,                NOPARAM,                     "P:",                                          "P:",                                          (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1359 */  { itemToBeCoded,                NOPARAM,                     "RAM",                                         "RAM",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1360 */  { itemToBeCoded,                NOPARAM,                     "REALS",                                       "REALS",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1361 */  { itemToBeCoded,                NOPARAM,                     "Solver",                                      "Solver",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1362 */  { itemToBeCoded,                NOPARAM,                     "STAT",                                        "STAT",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1363 */  { itemToBeCoded,                NOPARAM,                     "STK",                                         "STK",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1364 */  { itemToBeCoded,                NOPARAM,                     "STRING",                                      "STRING",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1365 */  { itemToBeCoded,                NOPARAM,                     "TEST",                                        "TEST",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1366 */  { itemToBeCoded,                NOPARAM,                     "TIMES",                                       "TIMES",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1367 */  { itemToBeCoded,                NOPARAM,                     "TRI",                                         "TRI",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1368 */  { itemToBeCoded,                NOPARAM,                     "TVM",                                         "TVM",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1369 */  { itemToBeCoded,                NOPARAM,                     "U" STD_RIGHT_ARROW,                           "U" STD_RIGHT_ARROW,                           (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1370 */  { itemToBeCoded,                NOPARAM,                     "VARS",                                        "VARS",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1371 */  { itemToBeCoded,                NOPARAM,                     "V:",                                          "V:",                                          (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1372 */  { itemToBeCoded,                NOPARAM,                     "X.FN",                                        "X.FN",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1373 */  { itemToBeCoded,                NOPARAM,                     "x:",                                          "x:",                                          (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1374 */  { itemToBeCoded,                NOPARAM,                     STD_alpha "INTL",                              STD_alpha "INTL",                              (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1375 */  { itemToBeCoded,                NOPARAM,                     STD_alpha "MATH",                              STD_alpha "MATH",                              (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1376 */  { itemToBeCoded,                NOPARAM,                     STD_alpha ".FN",                               STD_alpha ".FN",                               (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1377 */  { itemToBeCoded,                NOPARAM,                     STD_ALPHA STD_ELLIPSIS STD_OMEGA,              STD_ALPHA STD_ELLIPSIS STD_OMEGA,              (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED}, // Upper case greek letters
/* 1378 */  { itemToBeCoded,                NOPARAM,                     STD_alpha STD_BULLET,                          STD_alpha STD_BULLET,                          (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED}, // Upper case intl letters
/* 1379 */  { itemToBeCoded,                NOPARAM,                     "SYS.FL",                                      "SYS.FL",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1380 */  { itemToBeCoded,                NOPARAM,                     STD_INTEGRAL "f",                              STD_INTEGRAL "f",                              (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1381 */  { itemToBeCoded,                NOPARAM,                     STD_INTEGRAL "fdx",                            STD_INTEGRAL "fdx",                            (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1382 */  { itemToBeCoded,                NOPARAM,                     STD_MEASURED_ANGLE STD_RIGHT_ARROW,            STD_MEASURED_ANGLE STD_RIGHT_ARROW,            (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1383 */  { itemToBeCoded,                NOPARAM,                     STD_alpha STD_ELLIPSIS STD_omega,              STD_alpha STD_ELLIPSIS STD_omega,              (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED}, // Lower case greek letters
/* 1384 */  { itemToBeCoded,                NOPARAM,                     STD_alpha "intl",                              STD_alpha "intl",                              (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED}, // lower case intl letters
/* 1385 */  { itemToBeCoded,                NOPARAM,                     "",                                            "Tam",                                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1386 */  { itemToBeCoded,                NOPARAM,                     "",                                            "TamCmp",                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1387 */  { itemToBeCoded,                NOPARAM,                     "",                                            "TamStoRcl",                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1388 */  { itemToBeCoded,                NOPARAM,                     STD_SIGMA,                                     STD_SIGMA,                                     (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1389 */  { itemToBeCoded,                NOPARAM,                     "VAR",                                         "VAR",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1390 */  { itemToBeCoded,                NOPARAM,                     "",                                            "TamFlag",                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1391 */  { itemToBeCoded,                NOPARAM,                     "",                                            "TamShuffle",                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1392 */  { itemToBeCoded,                NOPARAM,                     "PROG",                                        "PROG",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED},
/* 1393 */  { itemToBeCoded,                NOPARAM,                     "",                                            "TamLabel",                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1394 */  { fnDynamicMenu,                NOPARAM,                     "",                                            "DYNMNU",                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1395 */  { itemToBeCoded,                NOPARAM,                     "PLOT.ST",                                     "PLOT.ST",                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1396 */  { itemToBeCoded,                NOPARAM,                     "PLOT.LR",                                     "PLOT.LR",                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED},
/* 1397 */  { itemToBeCoded,                NOPARAM,                     "1397",                                        "1397",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1398 */  { itemToBeCoded,                NOPARAM,                     "1398",                                        "1398",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1399 */  { itemToBeCoded,                NOPARAM,                     "1399",                                        "1399",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1400 */  { itemToBeCoded,                NOPARAM,                     "1400",                                        "1400",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1401 */  { itemToBeCoded,                NOPARAM,                     "1401",                                        "1401",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1402 */  { itemToBeCoded,                NOPARAM,                     "1402",                                        "1402",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},
/* 1403 */  { itemToBeCoded,                NOPARAM,                     "1403",                                        "1403",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED},


/* 1404 */  { fnIntegerMode,                SIM_1COMPL,                  "1COMPL",                                      "1COMPL",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1405 */  { fnScreenDump,                 NOPARAM,                     "SNAP",                                        "SNAP",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1406 */  { fnIntegerMode,                SIM_2COMPL,                  "2COMPL",                                      "2COMPL",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1407 */  { fnMagnitude,                  NOPARAM,                     "ABS",                                         "ABS",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1408 */  { fnAgm,                        NOPARAM,                     "AGM",                                         "AGM",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1409 */  { itemToBeCoded,                NOPARAM,                     "AGRAPH",                                      "AGRAPH",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1410 */  { fnDisplayFormatAll,           TM_VALUE,                    "ALL" ,                                        "ALL",                                         (0 << TAM_MAX_BITS) |    15, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1411 */  { itemToBeCoded,                NOPARAM,                     "ASSIGN",                                      "ASN",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1412 */  { itemToBeCoded,                NOPARAM,                     "BACK",                                        "BACK",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1413 */  { fnBatteryVoltage,             NOPARAM,                     "BATT?",                                       "BATT?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1414 */  { itemToBeCoded,                NOPARAM,                     "BEEP",                                        "BEEP",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1415 */  { itemToBeCoded,                NOPARAM,                     "BeginP",                                      "Begin",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1416 */  { fnBn,                         NOPARAM,                     "B" STD_SUB_n,                                 "B" STD_SUB_n,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1417 */  { fnBnStar,                     NOPARAM,                     "B" STD_SUB_n STD_SUP_ASTERISK,                "B" STD_SUB_n STD_SUP_ASTERISK,                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1418 */  { itemToBeCoded,                NOPARAM,                     "CASE",                                        "CASE",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1419 */  { fnClAll,                      NOT_CONFIRMED,               "CLALL",                                       "CLall",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1420 */  { itemToBeCoded,                NOPARAM,                     "CLCVAR",                                      "CLCVAR",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1421 */  { fnClFAll,                     NOT_CONFIRMED,               "CLFALL",                                      "CLFall",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1422 */  { fnFractionType,               NOPARAM,                     "a b/c",                                       "a b/c",                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED},
/* 1423 */  { itemToBeCoded,                NOPARAM,                     "CLLCD",                                       "CLLCD",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1424 */  { itemToBeCoded,                NOPARAM,                     "CLMENU",                                      "CLMENU",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1425 */  { fnClP,                        NOPARAM,                     "CLP",                                         "CLP",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_CANCEL   },
/* 1426 */  { fnClPAll,                     NOT_CONFIRMED,               "CLPALL",                                      "CLPall",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_CANCEL   },
/* 1427 */  { fnClearRegisters,             NOT_CONFIRMED,               "CLREGS",                                      "CLREGS",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_CANCEL   },
/* 1428 */  { fnClearStack,                 NOPARAM,                     "CLSTK",                                       "CLSTK",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_CANCEL   },
/* 1429 */  { fnClSigma,                    NOPARAM,                     "CL" STD_SIGMA,                                "CL" STD_SIGMA,                                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1430 */  { fnStoreMax,                   NOPARAM,                     "STO" STD_UP_ARROW,                            "Max",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1431 */  { fnConjugate,                  NOPARAM,                     "CONJ",                                        "conj",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1432 */  { fnRecallMax,                  NOPARAM,                     "RCL" STD_UP_ARROW,                            "Max",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1433 */  { fnCoefficientDetermination,   NOPARAM,                     "CORR",                                        "r",                                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1434 */  { fnPopulationCovariance,       NOPARAM,                     "COV",                                         "cov",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1435 */  { fnCurveFittingLR,             NOPARAM,                     "BestF?",                                      "BestF?",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1436 */  { fnCross,                      NOPARAM,                     "CROSS",                                       "cross",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1437 */  { fnCxToRe,                     NOPARAM,                     "CX" STD_RIGHT_ARROW "RE",                     "CX" STD_RIGHT_ARROW "RE",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1438 */  { fnDate,                       NOPARAM,                     "DATE",                                        "DATE",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1439 */  { fnDateTo,                     NOPARAM,                     "DATE" STD_RIGHT_ARROW,                        "DATE" STD_RIGHT_ARROW,                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1440 */  { fnDay,                        NOPARAM,                     "DAY",                                         "DAY",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1441 */  { fnDblDivideRemainder,         NOPARAM,                     "DBLR",                                        "DBLR",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1442 */  { fnDblMultiply,                NOPARAM,                     "DBL" STD_CROSS,                               "DBL" STD_CROSS,                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1443 */  { fnDblDivide,                  NOPARAM,                     "DBL/",                                        "DBL/",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1444 */  { fnDecomp,                     NOPARAM,                     "DECOMP",                                      "DECOMP",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1445 */  { fnAngularMode,                amDegree,                    "DEG",                                         "DEG",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1446 */  { fnCvtToCurrentAngularMode,    amDegree,                    "DEG" STD_RIGHT_ARROW,                         "DEG" STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1447 */  { fnStatSa,                     NOPARAM,                     "s(a)",                                        "s(a)",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1448 */  { fnDenMax,                     NOPARAM,                     "DENMAX",                                      "DENMAX",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1449 */  { fnDot,                        NOPARAM,                     "DOT",                                         "dot",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1450 */  { fnDisplayStack,               TM_VALUE,                    "DSTACK",                                      "DSTACK",                                      (1 << TAM_MAX_BITS) |     4, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1451 */  { fnAngularMode,                amDMS,                       "D.MS",                                        "d.ms",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1452 */  { fnCvtDmsToCurrentAngularMode, NOPARAM,                     "D.MS" STD_RIGHT_ARROW,                        "D.MS" STD_RIGHT_ARROW,                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1453 */  { fnSetDateFormat,              ITM_DMY,                     "D.MY",                                        "D.MY",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1454 */  { fnDateToJulian,               NOPARAM,                     "D" STD_RIGHT_ARROW "J",                       "D" STD_RIGHT_ARROW "J",                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1455 */  { itemToBeCoded,                NOPARAM,                     "DELITM",                                      "DELITM",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1456 */  { itemToBeCoded,                NOPARAM,                     "EIGVAL",                                      "EIGVAL",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1457 */  { itemToBeCoded,                NOPARAM,                     "EIGVEC",                                      "EIGVEC",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1458 */  { itemToBeCoded,                NOPARAM,                     "END",                                         "END",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1459 */  { itemToBeCoded,                NOPARAM,                     "ENDP",                                        "End",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1460 */  { fnDisplayFormatEng,           TM_VALUE,                    "ENG",                                         "ENG",                                         (0 << TAM_MAX_BITS) |    15, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1461 */  { fnEuclideanNorm,              NOPARAM,                     "ENORM",                                       "ENORM",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1462 */  { fnRecallMin,                  NOPARAM,                     "RCL" STD_DOWN_ARROW,                          "Min",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1463 */  { itemToBeCoded,                NOPARAM,                     "EQ.DEL",                                      "DELETE",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1464 */  { itemToBeCoded,                NOPARAM,                     "EQ.EDI",                                      "EDIT",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1465 */  { itemToBeCoded,                NOPARAM,                     "EQ.NEW",                                      "NEW",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1466 */  { fnErf,                        NOPARAM,                     "erf",                                         "erf",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1467 */  { fnErfc,                       NOPARAM,                     "erfc",                                        "erfc",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1468 */  { itemToBeCoded,                NOPARAM,                     "ERR",                                         "ERR",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1469 */  { itemToBeCoded,                NOPARAM,                     "EXITALL",                                     "EXITall",                                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1470 */  { fnExpt,                       NOPARAM,                     "EXPT",                                        "EXPT",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1471 */  { fnGetFirstGregorianDay,       NOPARAM,                     "J/G?",                                        "J/G?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1472 */  { fnFib,                        NOPARAM,                     "FIB",                                         "FIB",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1473 */  { fnDisplayFormatFix,           TM_VALUE,                    "FIX",                                         "FIX",                                         (0 << TAM_MAX_BITS) |    15, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1474 */  { fnFreeFlashMemory,            NOPARAM,                     "FLASH?",                                      "FLASH?",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1475 */  { itemToBeCoded,                NOPARAM,                     "f'(x)",                                       "f'(x)",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1476 */  { itemToBeCoded,                NOPARAM,                     "f\"(x)",                                      "f\"(x)",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1477 */  { fnDisplayFormatGap,           TM_VALUE,                    "GAP",                                         "GAP",                                         (0 << TAM_MAX_BITS) |    15, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1478 */  { fnGd,                         NOPARAM,                     "g" STD_SUB_d,                                 "g" STD_SUB_d,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1479 */  { fnInvGd,                      NOPARAM,                     "g" STD_SUB_d STD_SUP_MINUS_1,                 "g" STD_SUB_d STD_SUP_MINUS_1,                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1480 */  { fnAngularMode,                amGrad,                      "GRAD",                                        "GRAD",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1481 */  { fnCvtToCurrentAngularMode,    amGrad,                      "GRAD" STD_RIGHT_ARROW,                        "GRAD" STD_RIGHT_ARROW,                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1482 */  { fnGotoDot,                    NOPARAM,                     "GTO.",                                        "GTO.",                                        (0 << TAM_MAX_BITS) | 16383, CAT_FNCT | SLS_ENABLED   | US_CANCEL   },
/* 1483 */  { fnHermite,                    NOPARAM,                     "H" STD_SUB_n,                                 "H" STD_SUB_n,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1484 */  { fnHermiteP,                   NOPARAM,                     "H" STD_SUB_n STD_SUB_P,                       "H" STD_SUB_n STD_SUB_P,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1485 */  { fnImaginaryPart,              NOPARAM,                     "Im",                                          "Im",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1486 */  { fnIndexMatrix,                TM_REGISTER,                 "INDEX",                                       "INDEX",                                       (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_UNCHANGED | US_ENABLED  },
/* 1487 */  { fnIxyz,                       NOPARAM,                     "I" STD_SUB_x STD_SUB_y STD_SUB_z,             "I" STD_SUB_x STD_SUB_y STD_SUB_z,             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1488 */  { fnGammaP,                     NOPARAM,                     "I" STD_GAMMA STD_SUB_p,                       "I" STD_GAMMA STD_SUB_p,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1489 */  { fnGammaQ,                     NOPARAM,                     "I" STD_GAMMA STD_SUB_q,                       "I" STD_GAMMA STD_SUB_q,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1490 */  { fnIncDecI,                    INC_FLAG,                    "I+",                                          "I+",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1491 */  { fnIncDecI,                    DEC_FLAG,                    "I-",                                          "I-",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1492 */  { fnBessel,                     NOPARAM,                     "J" STD_SUB_y "(x)",                           "J" STD_SUB_y "(x)",                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1493 */  { fnIncDecJ,                    INC_FLAG,                    "J+",                                          "J+",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1494 */  { fnIncDecJ,                    DEC_FLAG,                    "J-",                                          "J-",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1495 */  { fnSetFirstGregorianDay,       NOPARAM,                     "J/G",                                         "J/G",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1496 */  { fnJulianToDate,               NOPARAM,                     "J" STD_RIGHT_ARROW "D",                       "J" STD_RIGHT_ARROW "D",                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1497 */  { itemToBeCoded,                NOPARAM,                     "KEY",                                         "KEY",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1498 */  { itemToBeCoded,                NOPARAM,                     "KEYG",                                        "KEYG",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1499 */  { itemToBeCoded,                NOPARAM,                     "KEYX",                                        "KEYX",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1500 */  { fnSinc,                       NOPARAM,                     "sinc",                                        "sinc",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1501 */  { itemToBeCoded,                NOPARAM,                     "KTYP?",                                       "KTYP?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1502 */  { fnLastX,                      NOPARAM,                     "LASTx",                                       "LASTx",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1503 */  { itemToBeCoded,                NOPARAM,                     "LBL?",                                        "LBL?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1504 */  { fnIsLeap,                     NOPARAM,                     "LEAP?",                                       "LEAP?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1505 */  { fnLaguerre,                   NOPARAM,                     "L" STD_SUB_m ,                                "L" STD_SUB_m ,                                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1506 */  { fnLaguerreAlpha,              NOPARAM,                     "L" STD_SUB_m STD_SUB_alpha,                   "L" STD_SUB_m STD_SUB_alpha,                   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1507 */  { fnLnBeta,                     NOPARAM,                     "LN" STD_beta,                                 "ln" STD_beta,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1508 */  { fnLnGamma,                    NOPARAM,                     "LN" STD_GAMMA,                                "ln" STD_GAMMA,                                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1509 */  { fnLoad,                       LM_ALL,                      "LOAD",                                        "LOAD",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_CANCEL   },
/* 1510 */  { fnLoad,                       LM_PROGRAMS,                 "LOADP",                                       "LOADP",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1511 */  { fnLoad,                       LM_REGISTERS,                "LOADR",                                       "LOADR",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1512 */  { fnLoad,                       LM_SYSTEM_STATE,             "LOADSS",                                      "LOADSS",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1513 */  { fnLoad,                       LM_SUMS,                     "LOAD" STD_SIGMA,                              "LOAD" STD_SIGMA,                              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1514 */  { allocateLocalRegisters,       TM_VALUE,                    "LocR",                                        "LocR",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1515 */  { fnGetLocR,                    NOPARAM,                     "LocR?",                                       "LocR?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1516 */  { fnProcessLR,                  NOPARAM,                     "L.R.",                                        "L.R.",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1517 */  { fnMant,                       NOPARAM,                     "MANT",                                        "MANT",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1518 */  { fnEditLinearEquationMatrixX,  NOPARAM,                     "Mat_X",                                       "Mat X",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1519 */  { fnFreeMemory,                 NOPARAM,                     "MEM?",                                        "MEM?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1520 */  { itemToBeCoded,                NOPARAM,                     "MENU",                                        "MENU",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1521 */  { fnMonth,                      NOPARAM,                     "MONTH",                                       "MONTH",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1522 */  { itemToBeCoded,                NOPARAM,                     "MSG",                                         "MSG",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1523 */  { fnAngularMode,                amMultPi,                    "MUL" STD_pi,                                  "MUL" STD_pi,                                  (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1524 */  { itemToBeCoded,                NOPARAM,                     "MVAR",                                        "MVAR",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1525 */  { fnDelRow,                     NOPARAM,                     "M.DELR",                                      "DELR",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1526 */  { fnSetMatrixDimensions,        TM_M_DIM,                    "M.DIM",                                       "DIM",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_UNCHANGED | US_ENABLED  },
/* 1527 */  { fnGetMatrixDimensions,        NOPARAM,                     "M.DIM?",                                      "DIM?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1528 */  { fnSetDateFormat,              ITM_MDY,                     "M.DY",                                        "M.DY",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1529 */  { fnEditMatrix,                 NOPARAM,                     "M.EDI",                                       "EDIT",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_UNCHANGED | US_ENABLED  },
/* 1530 */  { fnEditMatrix,                 TM_REGISTER,                 "M.EDIN",                                      "EDITN",                                       (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_UNCHANGED | US_ENABLED  },
/* 1531 */  { fnGetMatrix,                  NOPARAM,                     "M.GET",                                       "GETM",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1532 */  { fnGoToElement,                NOPARAM,                     "M.GOTO",                                      "GOTO",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1533 */  { fnSetGrowMode,                ON,                          "M.GROW",                                      "GROW",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_UNCHANGED | US_UNCHANGED},
/* 1534 */  { fnInsRow,                     NOPARAM,                     "M.INSR",                                      "INSR",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1535 */  { fnLuDecomposition,            NOPARAM,                     "M.LU",                                        "M.LU",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1536 */  { fnNewMatrix,                  NOPARAM,                     "M.NEW",                                       "NEW",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1537 */  { fnOldMatrix,                  NOPARAM,                     "M.OLD",                                       "OLD",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1538 */  { fnPutMatrix,                  NOPARAM,                     "M.PUT",                                       "PUTM",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1539 */  { fnSwapRows,                   NOPARAM,                     "M.R" STD_LEFT_RIGHT_ARROWS "R",               "R" STD_LEFT_RIGHT_ARROWS "R",                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1540 */  { fnSincpi,                     NOPARAM,                     "sinc" STD_pi,                                 "sinc" STD_pi,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1541 */  { fnSetGrowMode,                OFF,                         "M.WRAP",                                      "WRAP",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_UNCHANGED | US_UNCHANGED},
/* 1542 */  { fnNop,                        NOPARAM,                     "NOP",                                         "NOP",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1543 */  { fnOff,                        NOPARAM,                     "OFF",                                         "OFF",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1544 */  { fnDropY,                      NOPARAM,                     "DROPy",                                       "DROPy",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1545 */  { fnStoreMin,                   NOPARAM,                     "STO" STD_DOWN_ARROW,                          "Min",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1546 */  { itemToBeCoded,                NOPARAM,                     "PGMINT",                                      "PGMINT",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1547 */  { itemToBeCoded,                NOPARAM,                     "PGMSLV",                                      "PGMSLV",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1548 */  { itemToBeCoded,                NOPARAM,                     "PIXEL",                                       "PIXEL",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1549 */  { fnPlotStat,                   PLOT_START,                  "PLOT",                                        "PLOT",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1550 */  { fnLegendre,                   NOPARAM,                     "P" STD_SUB_n,                                 "P" STD_SUB_n,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1551 */  { itemToBeCoded,                NOPARAM,                     "POINT",                                       "POINT",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1552 */  { fnLoad,                       LM_NAMED_VARIABLES,          "LOADV",                                       "LOADV",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1553 */  { itemToBeCoded,                NOPARAM,                     "PopLR",                                       "PopLR",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1554 */  { itemToBeCoded,                NOPARAM,                     "PRCL",                                        "PRCL",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1555 */  { itemToBeCoded,                NOPARAM,                     "PSTO",                                        "PSTO",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1556 */  { itemToBeCoded,                NOPARAM,                     "PUTK",                                        "PUTK",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1557 */  { fnAngularMode,                amRadian,                    "RAD",                                         "RAD",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1558 */  { fnCvtToCurrentAngularMode,    amRadian,                    "RAD" STD_RIGHT_ARROW,                         "RAD" STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1559 */  { fnRandom,                     NOPARAM,                     "RAN#",                                        "RAN#",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1560 */  { registerBrowser,              NOPARAM,                     "RBR",                                         "RBR",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1561 */  { fnRecallConfig,               NOPARAM,                     "RCLCFG",                                      "Config",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1562 */  { fnRecallElement,              NOPARAM,                     "RCLEL",                                       "RCLEL",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1563 */  { fnRecallIJ,                   NOPARAM,                     "RCLIJ",                                       "RCLIJ",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1564 */  { fnRecallStack,                TM_REGISTER,                 "RCLS",                                        "RCLS",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1565 */  { fnRdp,                        TM_VALUE,                    "RDP",                                         "RDP",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1566 */  { fnRealPart,                   NOPARAM,                     "Re",                                          "Re",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1567 */  { itemToBeCoded,                NOPARAM,                     "RECV",                                        "RECV",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1568 */  { fnReset,                      NOT_CONFIRMED,               "RESET",                                       "RESET",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1569 */  { fnReToCx,                     NOPARAM,                     "RE" STD_RIGHT_ARROW "CX",                     "RE" STD_RIGHT_ARROW "CX",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1570 */  { fnSwapRealImaginary,          NOPARAM,                     "Re" STD_LEFT_RIGHT_ARROWS "Im",               "Re" STD_LEFT_RIGHT_ARROWS "Im",               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1571 */  { fnSetRoundingMode,            TM_VALUE,                    "RM",                                          "RM",                                          (0 << TAM_MAX_BITS) |     6, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1572 */  { fnGetRoundingMode,            NOPARAM,                     "RM?",                                         "RM?",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1573 */  { itemToBeCoded,                NOPARAM,                     "1573",                                        "1573",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_ENABLED  },
/* 1574 */  { fnRowNorm,                    NOPARAM,                     "RNORM",                                       "RNORM",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1575 */  { fnExpM1,                      NOPARAM,                     "e" STD_SUP_x "-1",                            "e" STD_SUP_x "-1",                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1576 */  { fnRoundi,                     NOPARAM,                     "ROUNDI",                                      "ROUNDI",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1577 */  { fnRsd,                        TM_VALUE,                    "RSD",                                         "RSD",                                         (1 << TAM_MAX_BITS) |    34, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1578 */  { fnRowSum,                     NOPARAM,                     "RSUM",                                        "RSUM",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1579 */  { itemToBeCoded,                NOPARAM,                     "RTN+1",                                       "RTN+1",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1580 */  { itemToBeCoded,                NOPARAM,                     "R-CLR",                                       "R-CLR",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1581 */  { itemToBeCoded,                NOPARAM,                     "R-COPY",                                      "R-COPY",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1582 */  { itemToBeCoded,                NOPARAM,                     "R-SORT",                                      "R-SORT",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1583 */  { itemToBeCoded,                NOPARAM,                     "R-SWAP",                                      "R-SWAP",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1584 */  { itemToBeCoded,                NOPARAM,                     "1584",                                        "1584",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_ENABLED  },
/* 1585 */  { fnSampleStdDev,               NOPARAM,                     "s",                                           "s",                                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1586 */  { fnSave,                       NOPARAM,                     "SAVE",                                        "SAVE",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1587 */  { fnDisplayFormatSci,           TM_VALUE,                    "SCI",                                         "SCI",                                         (0 << TAM_MAX_BITS) |    15, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1588 */  { fnGetSignificantDigits,       NOPARAM,                     "SDIGS?",                                      "SDIGS?",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1589 */  { fnSeed,                       NOPARAM,                     "SEED",                                        "SEED",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1590 */  { itemToBeCoded,                NOPARAM,                     "SEND",                                        "SEND",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1591 */  { fnConfigChina,                NOPARAM,                     "SETCHN",                                      "CHINA",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1592 */  { fnSetDate,                    NOPARAM,                     "SETDAT",                                      "SETDAT",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1593 */  { fnConfigEurope,               NOPARAM,                     "SETEUR",                                      "EUROPE",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1594 */  { fnConfigIndia,                NOPARAM,                     "SETIND",                                      "INDIA",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1595 */  { fnConfigJapan,                NOPARAM,                     "SETJPN",                                      "JAPAN",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1596 */  { itemToBeCoded,                NOPARAM,                     "SETSIG",                                      "SETSIG",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1597 */  { fnSetTime,                    NOPARAM,                     "SETTIM",                                      "SETTIM",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1598 */  { fnConfigUk,                   NOPARAM,                     "SETUK",                                       "UK",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1599 */  { fnConfigUsa,                  NOPARAM,                     "SETUSA",                                      "USA",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1600 */  { fnSign,                       NOPARAM,                     "SIGN",                                        "sign",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1601 */  { fnIntegerMode,                SIM_SIGNMT,                  "SIGNMT",                                      "SIGNMT",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1602 */  { fnSimultaneousLinearEquation, TM_VALUE,                    "SIM_EQ",                                      "SIM EQ",                                      (1 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1603 */  { itemToBeCoded,                NOPARAM,                     "SKIP",                                        "SKIP",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1604 */  { fnSlvq,                       NOPARAM,                     "SLVQ",                                        "SLVQ",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1605 */  { fnStandardError,              NOPARAM,                     "s" STD_SUB_m,                                 "s" STD_SUB_m,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1606 */  { fnGetIntegerSignMode,         NOPARAM,                     "ISM?",                                        "ISM?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1607 */  { fnWeightedStandardError,      NOPARAM,                     "s" STD_SUB_m STD_SUB_w,                       "s" STD_SUB_m STD_SUB_w,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1608 */  { itemToBeCoded,                NOPARAM,                     "SOLVE",                                       "SOLVE",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1609 */  { fnGetStackSize,               NOPARAM,                     "SSIZE?",                                      "SSIZE?",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1610 */  { flagBrowser,                  NOPARAM,                     "STATUS",                                      "STATUS",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1611 */  { fnStoreConfig,                NOPARAM,                     "STOCFG",                                      "Config",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1612 */  { fnStoreElement,               NOPARAM,                     "STOEL",                                       "STOEL",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1613 */  { fnStoreIJ,                    NOPARAM,                     "STOIJ",                                       "STOIJ",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1614 */  { fnLnP1,                       NOPARAM,                     "LN(1+x)",                                     "ln 1+x",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1615 */  { fnStoreStack,                 TM_REGISTER,                 "STOS",                                        "STOS",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1616 */  { fnSumXY,                      NOPARAM,                     "SUM",                                         "SUM",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1617 */  { fnWeightedSampleStdDev,       NOPARAM,                     "s" STD_SUB_w,                                 "s" STD_SUB_w,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1618 */  { fnSampleCovariance,           NOPARAM,                     "s" STD_SUB_x STD_SUB_y,                       "s" STD_SUB_x STD_SUB_y,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1619 */  { fnDisplayFormatTime,          TM_VALUE,                    "TDISP",                                       "TDISP",                                       (0 << TAM_MAX_BITS) |     6, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1620 */  { fnTicks,                      NOPARAM,                     "TICKS",                                       "TICKS",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1621 */  { fnTime,                       NOPARAM,                     "TIME",                                        "TIME",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1622 */  { itemToBeCoded,                NOPARAM,                     "TIMER",                                       "TIMER",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1623 */  { fnChebyshevT,                 NOPARAM,                     "T" STD_SUB_n,                                 "T" STD_SUB_n,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1624 */  { itemToBeCoded,                NOPARAM,                     "TONE",                                        "TONE",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1625 */  { fnSwapT,                      TM_REGISTER,                 "t" STD_LEFT_RIGHT_ARROWS,                     "t" STD_LEFT_RIGHT_ARROWS,                     (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1626 */  { fnUlp,                        NOPARAM,                     "ULP?",                                        "ULP?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1627 */  { fnChebyshevU,                 NOPARAM,                     "U" STD_SUB_n,                                 "U" STD_SUB_n,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1628 */  { fnUnitVector,                 NOPARAM,                     "UNITV",                                       "UNITV",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1629 */  { fnIntegerMode,                SIM_UNSIGN,                  "UNSIGN",                                      "UNSIGN",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1630 */  { itemToBeCoded,                NOPARAM,                     "VARMNU",                                      "VARMNU",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1631 */  { fnVersion,                    NOPARAM,                     "VERS?",                                       "VERS?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1632 */  { fnIDivR,                      NOPARAM,                     "IDIVR",                                       "IDIVR",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1633 */  { fnWday,                       NOPARAM,                     "WDAY",                                        "WDAY",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1634 */  { fnWho,                        NOPARAM,                     "WHO?",                                        "WHO?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1635 */  { fnWnegative,                  NOPARAM,                     "W" STD_SUB_m,                                 "W" STD_SUB_m,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1636 */  { fnWpositive,                  NOPARAM,                     "W" STD_SUB_p,                                 "W" STD_SUB_p,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1637 */  { fnWinverse,                   NOPARAM,                     "W" STD_SUP_MINUS_1,                           "W" STD_SUP_MINUS_1,                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1638 */  { fnSetWordSize,                TM_VALUE,                    "WSIZE",                                       "WSIZE",                                       (0 << TAM_MAX_BITS) |    64, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1639 */  { fnGetWordSize,                NOPARAM,                     "WSIZE?",                                      "WSIZE?",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1640 */  { fnMeanXY,                     NOPARAM,                     STD_x_BAR,                                     STD_x_BAR,                                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1641 */  { fnGeometricMeanXY,            NOPARAM,                     STD_x_BAR STD_SUB_G,                           STD_x_BAR STD_SUB_G,                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1642 */  { fnWeightedMeanX,              NOPARAM,                     STD_x_BAR STD_SUB_w,                           STD_x_BAR STD_SUB_w,                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1643 */  { fnXIsFny,                     NOPARAM,                     STD_x_CIRC,                                    STD_x_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1644 */  { fnXToDate,                    NOPARAM,                     "x" STD_RIGHT_ARROW "DATE",                    "x" STD_RIGHT_ARROW "DATE",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1645 */  { fnXToAlpha,                   NOPARAM,                     "x" STD_RIGHT_ARROW STD_alpha,                 "x" STD_RIGHT_ARROW STD_alpha,                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1646 */  { itemToBeCoded,                NOPARAM,                     "1646",                                        "1646",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_ENABLED  },
/* 1647 */  { fnYear,                       NOPARAM,                     "YEAR",                                        "YEAR",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1648 */  { fnYIsFnx,                     NOPARAM,                     STD_y_CIRC,                                    STD_y_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1649 */  { fnSetDateFormat,              ITM_YMD,                     "Y.MD",                                        "Y.MD",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1650 */  { fnSwapY,                      TM_REGISTER,                 "y" STD_LEFT_RIGHT_ARROWS,                     "y" STD_LEFT_RIGHT_ARROWS,                     (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1651 */  { fnSwapZ,                      TM_REGISTER,                 "z" STD_LEFT_RIGHT_ARROWS,                     "z" STD_LEFT_RIGHT_ARROWS,                     (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1652 */  { fnAlphaLeng,                  TM_REGISTER,                 STD_alpha "LENG?",                             STD_alpha "LENG?",                             (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1653 */  { fnXmax,                       NOPARAM,                     "x" STD_SUB_m STD_SUB_a STD_SUB_x,             "x" STD_SUB_m STD_SUB_a STD_SUB_x,             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1654 */  { fnXmin,                       NOPARAM,                     "x" STD_SUB_m STD_SUB_i STD_SUB_n,             "x" STD_SUB_m STD_SUB_i STD_SUB_n,             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1655 */  { fnAlphaPos,                   TM_REGISTER,                 STD_alpha "POS?",                              STD_alpha "POS?",                              (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1656 */  { fnAlphaRL,                    TM_REGISTER,                 STD_alpha "RL",                                STD_alpha "RL",                                (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1657 */  { fnAlphaRR,                    TM_REGISTER,                 STD_alpha "RR",                                STD_alpha "RR",                                (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1658 */  { fnAlphaSL,                    TM_REGISTER,                 STD_alpha "SL",                                STD_alpha "SL",                                (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1659 */  { fnAlphaSR,                    TM_REGISTER,                 STD_alpha "SR",                                STD_alpha "SR",                                (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1660 */  { fnAlphaToX,                   TM_REGISTER,                 STD_alpha STD_RIGHT_ARROW "x",                 STD_alpha STD_RIGHT_ARROW "x",                 (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1661 */  { fnBeta,                       NOPARAM,                     STD_beta "(x,y)",                              STD_beta "(x,y)",                              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1662 */  { fnGammaXyLower,               NOPARAM,                     STD_gamma STD_SUB_x STD_SUB_y,                 STD_gamma STD_SUB_x STD_SUB_y,                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1663 */  { fnGammaXyUpper,               NOPARAM,                     STD_GAMMA STD_SUB_x STD_SUB_y,                 STD_GAMMA STD_SUB_x STD_SUB_y,                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1664 */  { fnGamma,                      NOPARAM,                     STD_GAMMA "(x)",                               STD_GAMMA "(x)",                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1665 */  { itemToBeCoded,                NOPARAM,                     STD_delta "x",                                 STD_delta "x",                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED},
/* 1666 */  { fnDeltaPercent,               NOPARAM,                     STD_DELTA "%",                                 STD_DELTA "%",                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1667 */  { fnGeometricSampleStdDev,      NOPARAM,                     STD_epsilon,                                   STD_epsilon,                                   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1668 */  { fnGeometricStandardError,     NOPARAM,                     STD_epsilon STD_SUB_m,                         STD_epsilon STD_SUB_m,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1669 */  { fnGeometricPopulationStdDev,  NOPARAM,                     STD_epsilon STD_SUB_p,                         STD_epsilon STD_SUB_p,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1670 */  { fnZeta,                       NOPARAM,                     STD_zeta "(x)",                                STD_zeta "(x)",                                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1671 */  { itemToBeCoded,                NOPARAM,                     STD_PI STD_SUB_n,                              STD_PI STD_SUB_n,                              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1672 */  { itemToBeCoded,                NOPARAM,                     STD_SIGMA STD_SUB_n,                           STD_SIGMA STD_SUB_n,                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1673 */  { fnPopulationStdDev,           NOPARAM,                     STD_sigma,                                     STD_sigma,                                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1674 */  { fnWeightedPopulationStdDev,   NOPARAM,                     STD_sigma STD_SUB_w,                           STD_sigma STD_SUB_w,                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1675 */  { fnRandomI,                    NOPARAM,                     "RANI#",                                       "RANI#",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1676 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "x",                               STD_PRINTER "x",                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1677 */  { fnRange,                      NOPARAM,                     "RANGE",                                       "RANGE",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1678 */  { fnGetRange,                   NOPARAM,                     "RANGE?",                                      "RANGE?",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1679 */  { fnM1Pow,                      NOPARAM,                     "(-1)" STD_SUP_x,                              "(-1)" STD_SUP_x,                              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1680 */  { fnMulMod,                     NOPARAM,                     STD_CROSS "MOD",                               STD_CROSS "MOD",                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1681 */  { fnToDate,                     NOPARAM,                     STD_RIGHT_ARROW "DATE",                        STD_RIGHT_ARROW "DATE",                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1682 */  { itemToBeCoded,                NOPARAM,                     "1682",                                        "1682",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_ENABLED  },
/* 1683 */  { itemToBeCoded,                NOPARAM,                     "1683",                                        "1683",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_ENABLED  },
/* 1684 */  { itemToBeCoded,                NOPARAM,                     "1684",                                        "1684",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_ENABLED  },
/* 1685 */  { fnToHr,                       NOPARAM,                     STD_RIGHT_ARROW "HR",                          ".d",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1686 */  { fnToHms,                      NOPARAM,                     STD_RIGHT_ARROW "H.MS",                        "h.ms",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1687 */  { fnChangeBase,                 TM_VALUE_CHB,                STD_RIGHT_ARROW "INT",                         "#",                                           (2 << TAM_MAX_BITS) |    16, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1688 */  { itemToBeCoded,                NOPARAM,                     "1688",                                        "1688",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_ENABLED  },
/* 1689 */  { fnToPolar,                    NOPARAM,                     STD_RIGHT_ARROW "POL",                         STD_RIGHT_ARROW "P",                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1690 */  { itemToBeCoded,                NOPARAM,                     "1690",                                        "1690",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_ENABLED  },
/* 1691 */  { fnToReal,                     NOPARAM,                     STD_RIGHT_ARROW "REAL",                        ".d",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1692 */  { fnToRect,                     NOPARAM,                     STD_RIGHT_ARROW "REC",                         "R" STD_LEFT_ARROW,                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1693 */  { fnCvtDegToDms,                NOPARAM,                     "D" STD_RIGHT_ARROW "D.MS",                    "D" STD_RIGHT_ARROW "D.MS",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1694 */  { fnShuffle,                    TM_SHUFFLE,                  STD_LEFT_RIGHT_ARROWS,                         STD_LEFT_RIGHT_ARROWS,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1695 */  { fnPercent,                    NOPARAM,                     "%",                                           "%",                                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1696 */  { fnPercentMRR,                 NOPARAM,                     "%MRR",                                        "%MRR",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1697 */  { fnPercentT,                   NOPARAM,                     "%T",                                          "%T",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1698 */  { fnPercentSigma,               NOPARAM,                     "%" STD_SIGMA,                                 "%" STD_SIGMA,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1699 */  { fnPercentPlusMG,              NOPARAM,                     "%+MG",                                        "%+MG",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1700 */  { itemToBeCoded,                NOPARAM,                     STD_INTEGRAL,                                  STD_INTEGRAL,                                  (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1701 */  { fnExpMod,                     NOPARAM,                     "^MOD",                                        "^MOD",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1702 */  { fnDeterminant,                NOPARAM,                     "|M|",                                         "|M|",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1703 */  { fnParallel,                   NOPARAM,                     "||",                                          "|" STD_SPACE_3_PER_EM "|",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1704 */  { fnTranspose,                  NOPARAM,                     "[M]" STD_SUP_T,                               "[M]" STD_SUP_T,                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1705 */  { fnInvertMatrix,               NOPARAM,                     "[M]" STD_SUP_MINUS_1,                         "[M]" STD_SUP_MINUS_1,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1706 */  { fnArg,                        NOPARAM,                     STD_MEASURED_ANGLE,                            STD_MEASURED_ANGLE,                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1707 */  { fnCvtToCurrentAngularMode,    amMultPi,                    "MUL" STD_pi STD_RIGHT_ARROW,                  "MUL" STD_pi STD_RIGHT_ARROW,                  (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1708 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "ADV",                             STD_PRINTER "ADV",                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1709 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "CHAR",                            STD_PRINTER "CHAR",                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1710 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "DLAY",                            STD_PRINTER "DLAY",                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1711 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "LCD",                             STD_PRINTER "LCD",                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1712 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "MODE",                            STD_PRINTER "MODE",                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1713 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "PROG",                            STD_PRINTER "PROG",                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1714 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "r",                               STD_PRINTER "r",                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1715 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "REGS",                            STD_PRINTER "REGS",                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1716 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "STK",                             STD_PRINTER "STK",                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1717 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "TAB",                             STD_PRINTER "TAB",                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1718 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "USER",                            STD_PRINTER "USER",                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1719 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "WIDTH",                           STD_PRINTER "WIDTH",                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1720 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER STD_SIGMA,                         STD_PRINTER STD_SIGMA,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1721 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "#",                               STD_PRINTER "#",                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },

/* 1722 */  { fontBrowser,                  NOPARAM,                     "FBR",                                         "FBR",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED}, // Font browser

/* 1723 */  { fnUndo,                       NOPARAM,                     "UNDO",                                        STD_UNDO,                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_UNCHANGED | US_UNCHANGED},
/* 1724 */  { fnPem,                        NOPARAM,                     "P/R",                                         "P/R",                                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_CANCEL   },
/* 1725 */  { itemToBeCoded,                NOPARAM,                     "R/S",                                         "R/S",                                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED},
/* 1726 */  { itemToBeCoded,                NOPARAM,                     "1726",                                        "1726",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/* 1727 */  { itemToBeCoded,                NOPARAM,                     "1727",                                        "1727",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/* 1728 */  { itemToBeCoded,                NOPARAM,                     "1728",                                        "1728",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED},
/* 1729 */  { fnFlipFlag,                   FLAG_USER,                   "USER",                                        "USER",                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED},
/* 1730 */  { fnKeyCC,                      NOPARAM,                     "CC",                                          "CC",                                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED},
/* 1731 */  { itemToBeCoded,                NOPARAM,                     "",                                            "f",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED},
/* 1732 */  { itemToBeCoded,                NOPARAM,                     "",                                            "g",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED},
/* 1733 */  { fnKeyUp,                      NOPARAM,                     "UP",                                          STD_UP_ARROW,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED},
/* 1734 */  { itemToBeCoded,                NOPARAM,                     "BST",                                         STD_HAMBURGER STD_BST,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED},
/* 1735 */  { fnKeyDown,                    NOPARAM,                     "DOWN",                                        STD_DOWN_ARROW,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED},
/* 1736 */  { itemToBeCoded,                NOPARAM,                     "SST",                                         STD_HAMBURGER STD_SST,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED},
/* 1737 */  { fnKeyExit,                    NOPARAM,                     "EXIT",                                        "EXIT",                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED},
/* 1738 */  { fnKeyBackspace,               NOPARAM,                     "BKSPC",                                       STD_LEFT_ARROW,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED},
/* 1739 */  { itemToBeCoded,                NOPARAM,                     "1739",                                        "1739",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_ENABLED  },
/* 1740 */  { fnAim,                        NOPARAM,                     "AIM",                                         STD_alpha,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED  },
/* 1741 */  { fnKeyDotD,                    NOPARAM,                     ".d",                                          ".d",                                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED},
/* 1742 */  { fnShow,                       NOPARAM,                     "SHOW",                                        "SHOW",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1743 */  { backToSystem,                 NOPARAM,                     "SYSTEM",                                      "SYSTEM",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED},
/* 1744 */  { fnCvtDmsToDeg,                NOPARAM,                     "D.MS" STD_RIGHT_ARROW "D",                    "D.MS" STD_RIGHT_ARROW "D",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1745 */  { itemToBeCoded,                NOPARAM,                     "V" STD_MEASURED_ANGLE,                        STD_MEASURED_ANGLE,                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1746 */  { fnHarmonicMeanXY,             NOPARAM,                     STD_x_BAR STD_SUB_H,                           STD_x_BAR STD_SUB_H,                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1747 */  { fnRMSMeanXY,                  NOPARAM,                     STD_x_BAR STD_SUB_R STD_SUB_M STD_SUB_S,       STD_x_BAR STD_SUB_R STD_SUB_M STD_SUB_S,       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1748 */  { fnArccos,                     NOPARAM,                     "ACOS",                                        "ACOS",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1749 */  { fnArcsin,                     NOPARAM,                     "ASIN",                                        "ASIN",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1750 */  { fnArctan,                     NOPARAM,                     "ATAN",                                        "ATAN",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1751 */  { itemToBeCoded,                NOPARAM,                     "DET",                                         "DET",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1752 */  { itemToBeCoded,                NOPARAM,                     "INVRT",                                       "INVRT",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1753 */  { itemToBeCoded,                NOPARAM,                     "TRANS",                                       "TRANS",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1754 */  { itemToBeCoded,                NOPARAM,                     "xIN",                                         "xIN",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1755 */  { itemToBeCoded,                NOPARAM,                     "xOUT",                                        "xOUT",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED  },
/* 1756 */  { fnPlotStat,                   PLOT_ORTHOF,                 "",                                            "CENTRL",                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED  },
/* 1757 */  { itemToBeCoded,                NOPARAM,                     "1757",                                        "1757",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_ENABLED  },
/* 1758 */  { fnPlotCloseSmi,               NOPARAM,                     "s" STD_SUB_m STD_SUB_i,                       "s" STD_SUB_m STD_SUB_i,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_UNCHANGED | US_ENABLED  },
/* 1759 */  { fnPlotStat,                   PLOT_LR,                     "ASSESS",                                      "ASSESS",                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED  },
/* 1760 */  { fnPlotStat,                   PLOT_NXT,                    "",                                            "NXTFIT",                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED  },
/* 1761 */  { fnPlotStat,                   PLOT_REV,                    "",                                            "",                                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED  },
/* 1762 */  { fnPlotZoom,                   NOPARAM,                     "",                                            "ZOOM",                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED  },

/* 1763 */  { itemToBeCoded,                NOPARAM,                    "",                                            "Last item",                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED},
};
