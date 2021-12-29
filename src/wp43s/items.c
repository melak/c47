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

#include "items.h"

#include "assign.h"
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
#include "solver/solver.h"
#include "stack.h"
#include "stats.h"
#include "statusBar.h"
#include "store.h"
#include "stringFuncs.h"
#include "timer.h"
#include "tone.h"
#include "ui/tam.h"

#include "wp43s.h"

void itemToBeCoded(uint16_t unusedButMandatoryParameter) {
  funcOK = false;
}



//#ifndef GENERATE_CATALOGS
//void fnToBeCoded(void) {
//  displayCalcErrorMessage(ERROR_FUNCTION_TO_BE_CODED, ERR_REGISTER_LINE, REGISTER_X);
//  #ifdef PC_BUILD
//    moreInfoOnError("Function to be coded", "for that data type(s)!", NULL, NULL);
//  #endif // PC_BUILD
//}
//#endif // GENERATE_CATALOGS



void fnNop(uint16_t unusedButMandatoryParameter) {
}


#if !defined(TESTSUITE_BUILD) && !defined(GENERATE_CATALOGS)
  void reallyRunFunction(int16_t func, uint16_t param) {
    if((indexOfItems[func].status & US_STATUS) == US_ENABLED || (indexOfItems[func].status & US_STATUS) == US_ENABL_XEQ) {
      if(programRunStop != PGM_RUNNING) {
        saveForUndo();
      }
      if(lastErrorCode == ERROR_RAM_FULL) {
        if((indexOfItems[func].status & US_STATUS) == US_ENABLED || calcMode == CM_CONFIRMATION) {
          displayCalcErrorMessage(ERROR_RAM_FULL, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            moreInfoOnError("In function reallyRunFunction:", "there is not enough memory to save for undo!", NULL, NULL);
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
          return;
        }
        else {
          lastErrorCode = ERROR_NONE;
          temporaryInformation = TI_UNDO_DISABLED;
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            moreInfoOnError("In function reallyRunFunction:", "there is not enough memory to save for undo!", NULL, NULL);
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        }
      }
    }
    else if((indexOfItems[func].status & US_STATUS) == US_CANCEL) {
      thereIsSomethingToUndo = false;
    }

    if(programRunStop != PGM_RUNNING) {
      hourGlassIconEnabled = true;
      showHideHourGlass();

      #ifdef DMCP_BUILD
        lcd_refresh();
      #else // !DMCP_BUILD
        refreshLcd(NULL);
      #endif // DMCP_BUILD
    }

    indexOfItems[func].func(param);

    if(lastErrorCode != 0) {
      if(getSystemFlag(FLAG_IGN1ER)) {
        if(thereIsSomethingToUndo) undo();
        lastErrorCode = ERROR_NONE;
      }
      else {
        if(thereIsSomethingToUndo) undo();
      }
      clearSystemFlag(FLAG_IGN1ER);
    }
    else {
      if((indexOfItems[func].status & SLS_STATUS) == SLS_DISABLED) {
        clearSystemFlag(FLAG_ASLIFT);
      }
      else if((indexOfItems[func].status & SLS_STATUS) == SLS_ENABLED) {
        setSystemFlag(FLAG_ASLIFT);
      }
    }

    if(programRunStop != PGM_RUNNING) {
      updateMatrixHeightCache();
      cachedDynamicMenu = 0;
      #ifdef PC_BUILD
        refreshLcd(NULL);
      #endif // PC_BUILD
    }
  }



  void runFunction(int16_t func) {
    funcOK = true;

    #ifdef PC_BUILD
      if(func >= LAST_ITEM) {
        sprintf(errorMessage, "item (%" PRId16 ") must be below LAST_ITEM", func);
        moreInfoOnError("In function runFunction:", errorMessage, NULL, NULL);
      }
    #endif // PC_BUILD

    if(programRunStop != PGM_RUNNING) {
      if(func == ITM_RCL && dynamicMenuItem > -1) {
        char *varCatalogItem = dynmenuGetLabel(dynamicMenuItem);
        calcRegister_t regist = findNamedVariable(varCatalogItem);
        if(regist != INVALID_VARIABLE) {
          reallyRunFunction(func, regist);
        }
        else {
          displayCalcErrorMessage(ERROR_UNDEF_SOURCE_VAR, ERR_REGISTER_LINE, REGISTER_X);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(errorMessage, "string '%s' is not a named variable", varCatalogItem);
            moreInfoOnError("In function runFunction:", errorMessage, NULL, NULL);
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        }
        return;
      }
      else if(func == ITM_XEQ && dynamicMenuItem > -1) {
        char *varCatalogItem = dynmenuGetLabel(dynamicMenuItem);
        calcRegister_t regist = findNamedLabel(varCatalogItem);
        if(regist != INVALID_VARIABLE) {
          reallyRunFunction(func, regist);
        }
        else {
          displayCalcErrorMessage(ERROR_LABEL_NOT_FOUND, ERR_REGISTER_LINE, REGISTER_X);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(errorMessage, "string '%s' is not a named label", varCatalogItem);
            moreInfoOnError("In function runFunction:", errorMessage, NULL, NULL);
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
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
  void fnDisplayFormatDsp          (uint16_t unusedButMandatoryParameter) {}
  void fnDisplayFormatGap          (uint16_t unusedButMandatoryParameter) {}
  void fnDisplayFormatTime         (uint16_t unusedButMandatoryParameter) {}
  void fnArccos                    (uint16_t unusedButMandatoryParameter) {}
  void fnArccosh                   (uint16_t unusedButMandatoryParameter) {}
  void fnArcsin                    (uint16_t unusedButMandatoryParameter) {}
  void fnArcsinh                   (uint16_t unusedButMandatoryParameter) {}
  void fnArctan                    (uint16_t unusedButMandatoryParameter) {}
  void fnArctanh                   (uint16_t unusedButMandatoryParameter) {}
  void fnAtan2                     (uint16_t unusedButMandatoryParameter) {}
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
  void fnKeyAngle                  (uint16_t unusedButMandatoryParameter) {}
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
  void fnBesselJ                   (uint16_t unusedButMandatoryParameter) {}
  void fnBesselY                   (uint16_t unusedButMandatoryParameter) {}
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
  void fnSetSignificantDigits      (uint16_t unusedButMandatoryParameter) {}
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
  void fnView                      (uint16_t unusedButMandatoryParameter) {}
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
  void fnHide                      (uint16_t unusedButMandatoryParameter) {}
  void fnGetHide                   (uint16_t unusedButMandatoryParameter) {}
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
  void fnTone                      (uint16_t unusedButMandatoryParameter) {}
  void fnBeep                      (uint16_t unusedButMandatoryParameter) {}
  void fnSave                      (uint16_t unusedButMandatoryParameter) {}
  void fnLoad                      (uint16_t unusedButMandatoryParameter) {}
  void fnDeleteBackup              (uint16_t unusedButMandatoryParameter) {}
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
  void fnExecute                   (uint16_t unusedButMandatoryParameter) {}
  void fnReturn                    (uint16_t unusedButMandatoryParameter) {}
  void fnRunProgram                (uint16_t unusedButMandatoryParameter) {}
  void fnStopProgram               (uint16_t unusedButMandatoryParameter) {}
  void fnBst                       (uint16_t unusedButMandatoryParameter) {}
  void fnSst                       (uint16_t unusedButMandatoryParameter) {}
  void fnBack                      (uint16_t unusedButMandatoryParameter) {}
  void fnSkip                      (uint16_t unusedButMandatoryParameter) {}
  void fnCase                      (uint16_t unusedButMandatoryParameter) {}
  void fnRaiseError                (uint16_t unusedButMandatoryParameter) {}
  void fnRegClr                    (uint16_t unusedButMandatoryParameter) {}
  void fnRegCopy                   (uint16_t unusedButMandatoryParameter) {}
  void fnRegSort                   (uint16_t unusedButMandatoryParameter) {}
  void fnRegSwap                   (uint16_t unusedButMandatoryParameter) {}
  void fnInput                     (uint16_t unusedButMandatoryParameter) {}
  void fnClCVar                    (uint16_t unusedButMandatoryParameter) {}
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
  void fnVectorAngle               (uint16_t unusedButMandatoryParameter) {}
  void fnIndexMatrix               (uint16_t unusedButMandatoryParameter) {}
  void fnGetMatrix                 (uint16_t unusedButMandatoryParameter) {}
  void fnPutMatrix                 (uint16_t unusedButMandatoryParameter) {}
  void fnSwapRows                  (uint16_t unusedButMandatoryParameter) {}
  void fnSimultaneousLinearEquation(uint16_t unusedButMandatoryParameter) {}
  void fnEditLinearEquationMatrixA (uint16_t unusedButMandatoryParameter) {}
  void fnEditLinearEquationMatrixB (uint16_t unusedButMandatoryParameter) {}
  void fnEditLinearEquationMatrixX (uint16_t unusedButMandatoryParameter) {}
  void fnQrDecomposition           (uint16_t unusedButMandatoryParameter) {}
  void fnEigenvalues               (uint16_t unusedButMandatoryParameter) {}
  void fnEigenvectors              (uint16_t unusedButMandatoryParameter) {}
  void fnCvtMultPiToRad            (uint16_t unusedButMandatoryParameter) {}
  void fnCvtRadToMultPi            (uint16_t unusedButMandatoryParameter) {}
  void fnJacobiSn                  (uint16_t unusedButMandatoryParameter) {}
  void fnJacobiCn                  (uint16_t unusedButMandatoryParameter) {}
  void fnJacobiDn                  (uint16_t unusedButMandatoryParameter) {}
  void fnJacobiAmplitude           (uint16_t unusedButMandatoryParameter) {}
  void fnEllipticK                 (uint16_t unusedButMandatoryParameter) {}
  void fnEllipticE                 (uint16_t unusedButMandatoryParameter) {}
  void fnEllipticPi                (uint16_t unusedButMandatoryParameter) {}
  void fnEllipticFphi              (uint16_t unusedButMandatoryParameter) {}
  void fnEllipticEphi              (uint16_t unusedButMandatoryParameter) {}
  void fnJacobiZeta                (uint16_t unusedButMandatoryParameter) {}
  void fnPgmSlv                    (uint16_t unusedButMandatoryParameter) {}
  void fnSolve                     (uint16_t unusedButMandatoryParameter) {}
  void fnSolveVar                  (uint16_t unusedButMandatoryParameter) {}
  void fnEqDelete                  (uint16_t unusedButMandatoryParameter) {}
  void fnEqEdit                    (uint16_t unusedButMandatoryParameter) {}
  void fnEqNew                     (uint16_t unusedButMandatoryParameter) {}
  void fnEqCursorLeft              (uint16_t unusedButMandatoryParameter) {}
  void fnEqCursorRight             (uint16_t unusedButMandatoryParameter) {}
  void fnEqCalc                    (uint16_t unusedButMandatoryParameter) {}
  void fnProgrammableSum           (uint16_t unusedButMandatoryParameter) {}
  void fnProgrammableProduct       (uint16_t unusedButMandatoryParameter) {}
  void fnTvmVar                    (uint16_t unusedButMandatoryParameter) {}
  void fnTvmBeginMode              (uint16_t unusedButMandatoryParameter) {}
  void fnTvmEndMode                (uint16_t unusedButMandatoryParameter) {}
  void fnAssign                    (uint16_t unusedButMandatoryParameter) {}
  void fnTimer                     (uint16_t unusedButMandatoryParameter) {}
  void fnAddTimerApp               (uint16_t unusedButMandatoryParameter) {}
  void fnDecisecondTimerApp        (uint16_t unusedButMandatoryParameter) {}
  void fnResetTimerApp             (uint16_t unusedButMandatoryParameter) {}
  void fnRecallTimerApp            (uint16_t unusedButMandatoryParameter) {}
#endif // GENERATE_CATALOGS

TO_QSPI const item_t indexOfItems[] = {

//            function                      parameter                    item in catalog                                item in softmenu                               TAM min                 max  CATALOG    stackLift       UNDO status
/*    0 */  { itemToBeCoded,                NOPARAM,                     "",                                            "0000",                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // ITM_NULL

// Items from 1 to 127 are 1 byte OP codes
/*    1 */  { fnNop,                        TM_LABEL,                    "LBL",                                         "LBL",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*    2 */  { fnGoto,                       TM_LABEL,                    "GTO",                                         "GTO",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*    3 */  { fnExecute,                    TM_LABEL,                    "XEQ",                                         "XEQ",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*    4 */  { fnReturn,                     0,                           "RTN",                                         "RTN",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*    5 */  { fnIse,                        TM_REGISTER,                 "ISE",                                         "ISE",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*    6 */  { fnIsg,                        TM_REGISTER,                 "ISG",                                         "ISG",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*    7 */  { fnIsz,                        TM_REGISTER,                 "ISZ",                                         "ISZ",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*    8 */  { fnDse,                        TM_REGISTER,                 "DSE",                                         "DSE",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*    9 */  { fnDsl,                        TM_REGISTER,                 "DSL",                                         "DSL",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   10 */  { fnDsz,                        TM_REGISTER,                 "DSZ",                                         "DSZ",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   11 */  { fnXEqualsTo,                  TM_CMP,                      "x= ?",                                        "x= ?",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   12 */  { fnXNotEqual,                  TM_CMP,                      "x" STD_NOT_EQUAL " ?",                        "x" STD_NOT_EQUAL " ?",                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   13 */  { fnCheckValue,                 CHECK_VALUE_POSITIVE_ZERO,   "x=+0?",                                       "x=+0?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   14 */  { fnCheckValue,                 CHECK_VALUE_NEGATIVE_ZERO,   "x=-0?",                                       "x=-0?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   15 */  { fnXAlmostEqual,               TM_CMP,                      "x" STD_ALMOST_EQUAL " ?",                     "x" STD_ALMOST_EQUAL " ?",                     (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   16 */  { fnXLessThan,                  TM_CMP,                      "x< ?",                                        "x< ?",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   17 */  { fnXLessEqual,                 TM_CMP,                      "x" STD_LESS_EQUAL " ?",                       "x" STD_LESS_EQUAL " ?",                       (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   18 */  { fnXGreaterEqual,              TM_CMP,                      "x" STD_GREATER_EQUAL " ?",                    "x" STD_GREATER_EQUAL " ?",                    (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   19 */  { fnXGreaterThan,               TM_CMP,                      "x> ?",                                        "x> ?",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   20 */  { fnIsFlagClear,                TM_FLAGR,                    "FC?",                                         "FC?",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   21 */  { fnIsFlagSet,                  TM_FLAGR,                    "FS?",                                         "FS?",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   22 */  { fnCheckInteger,               CHECK_INTEGER_EVEN,          "EVEN?",                                       "EVEN?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   23 */  { fnCheckInteger,               CHECK_INTEGER_ODD,           "ODD?",                                        "ODD?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   24 */  { fnCheckInteger,               CHECK_INTEGER_FP,            "FP?",                                         "FP?",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   25 */  { fnCheckInteger,               CHECK_INTEGER,               "INT?",                                        "INT?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   26 */  { fnCheckValue,                 CHECK_VALUE_COMPLEX,         "CPX?",                                        "CPX?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   27 */  { fnCheckValue,                 CHECK_VALUE_MATRIX,          "MATR?",                                       "MATR?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   28 */  { fnCheckValue,                 CHECK_VALUE_NAN,             "NaN?",                                        "NaN?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   29 */  { fnCheckValue,                 CHECK_VALUE_REAL,            "REAL?",                                       "REAL?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   30 */  { fnCheckValue,                 CHECK_VALUE_SPECIAL,         "SPEC?",                                       "SPEC?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   31 */  { fnIsString,                   NOPARAM,                     "STRI?",                                       "STRI?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   32 */  { fnCheckValue,                 CHECK_VALUE_INFINITY,        STD_PLUS_MINUS STD_INFINITY "?",               STD_PLUS_MINUS STD_INFINITY "?",               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   33 */  { fnIsPrime,                    NOPARAM,                     "PRIME?",                                      "PRIME?",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   34 */  { itemToBeCoded,                NOPARAM,                     "TOP?",                                        "TOP?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   35 */  { fnKeyEnter,                   NOPARAM,                     "ENTER" STD_UP_ARROW,                          "ENTER" STD_UP_ARROW,                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_DISABLED  | US_ENABLED   | EIM_DISABLED},
/*   36 */  { fnSwapXY,                     NOPARAM,                     "x" STD_LEFT_RIGHT_ARROWS "y",                 "x" STD_LEFT_RIGHT_ARROWS "y",                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABL_XEQ | EIM_DISABLED},
/*   37 */  { fnDrop,                       NOPARAM,                     "DROP",                                        "DROP" STD_DOWN_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   38 */  { itemToBeCoded,                NOPARAM,                     "PAUSE",                                       "PAUSE",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   39 */  { fnRollUp,                     NOPARAM,                     "R" STD_UP_ARROW,                              "R" STD_UP_ARROW,                              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABL_XEQ | EIM_DISABLED},
/*   40 */  { fnRollDown,                   NOPARAM,                     "R" STD_DOWN_ARROW,                            "R" STD_DOWN_ARROW,                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABL_XEQ | EIM_DISABLED},
/*   41 */  { fnClX,                        NOPARAM,                     "CLX",                                         "CLX",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_DISABLED  | US_ENABL_XEQ | EIM_DISABLED},
/*   42 */  { fnFillStack,                  NOPARAM,                     "FILL",                                        "FILL",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   43 */  { fnInput,                      TM_REGISTER,                 "INPUT",                                       "INPUT",                                       (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_CANCEL    | EIM_DISABLED},
/*   44 */  { fnStore,                      TM_STORCL,                   "STO",                                         "STO",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   45 */  { fnStoreAdd,                   NOPARAM,                     "STO+",                                        "STO+",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   46 */  { fnStoreSub,                   NOPARAM,                     "STO-",                                        "STO-",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   47 */  { fnStoreMult,                  NOPARAM,                     "STO" STD_CROSS,                               "STO" STD_CROSS,                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   48 */  { fnStoreDiv,                   NOPARAM,                     "STO/",                                        "STO/",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   49 */  { fnCyx,                        NOPARAM,                     "COMB",                                        "C" STD_SUB_y STD_SUB_x,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   50 */  { fnPyx,                        NOPARAM,                     "PERM",                                        "P" STD_SUB_y STD_SUB_x,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   51 */  { fnRecall,                     TM_STORCL,                   "RCL",                                         "RCL",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   52 */  { fnRecallAdd,                  NOPARAM,                     "RCL+",                                        "RCL+",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   53 */  { fnRecallSub,                  NOPARAM,                     "RCL-",                                        "RCL-",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   54 */  { fnRecallMult,                 NOPARAM,                     "RCL" STD_CROSS,                               "RCL" STD_CROSS,                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   55 */  { fnRecallDiv,                  NOPARAM,                     "RCL/",                                        "RCL/",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   56 */  { fnIsConverged,                TM_VALUE,                    "CONVG?",                                      "CONVG?",                                      (0 << TAM_MAX_BITS) |    31, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*   57 */  { itemToBeCoded,                NOPARAM,                     "ENTRY?",                                      "ENTRY?",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   58 */  { fnSquare,                     NOPARAM,                     "x" STD_SUP_2,                                 "x" STD_SUP_2,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   59 */  { fnCube,                       NOPARAM,                     "x" STD_SUP_3,                                 "x" STD_SUP_3,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   60 */  { fnPower,                      NOPARAM,                     "y" STD_SUP_x,                                 "y" STD_SUP_x,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   61 */  { fnSquareRoot,                 NOPARAM,                     STD_SQUARE_ROOT STD_x_UNDER_ROOT,              STD_SQUARE_ROOT STD_x_UNDER_ROOT,              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   62 */  { fnCubeRoot,                   NOPARAM,                     STD_CUBE_ROOT STD_x_UNDER_ROOT,                STD_CUBE_ROOT STD_x_UNDER_ROOT,                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   63 */  { fnXthRoot,                    NOPARAM,                     STD_xTH_ROOT STD_y_UNDER_ROOT,                 STD_xTH_ROOT STD_y_UNDER_ROOT,                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   64 */  { fn2Pow,                       NOPARAM,                     "2" STD_SUP_x,                                 "2" STD_SUP_x,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   65 */  { fnExp,                        NOPARAM,                     "e" STD_SUP_x,                                 "e" STD_SUP_x,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   66 */  { fnRound,                      NOPARAM,                     "ROUND",                                       "ROUND",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   67 */  { fn10Pow,                      NOPARAM,                     "10" STD_SUP_x,                                "10" STD_SUP_x,                                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   68 */  { fnLog2,                       NOPARAM,                     "lb",                                          "lb x",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   69 */  { fnLn,                         NOPARAM,                     "ln",                                          "ln",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   70 */  { fnStopProgram,                NOPARAM,                     "STOP",                                        "R/S",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*   71 */  { fnLog10,                      NOPARAM,                     "lg",                                          "lg",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   72 */  { fnLogXY,                      NOPARAM,                     "log" STD_SUB_x "y",                           "log" STD_SUB_x "y",                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   73 */  { fnInvert,                     NOPARAM,                     "1/x",                                         "1/x",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   74 */  { fnCos,                        NOPARAM,                     "cos",                                         "cos",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   75 */  { fnCosh,                       NOPARAM,                     "cosh",                                        "cosh",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   76 */  { fnSin,                        NOPARAM,                     "sin",                                         "sin",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   77 */  { itemToBeCoded,                NOPARAM,                     "KEY?",                                        "KEY?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   78 */  { fnSinh,                       NOPARAM,                     "sinh",                                        "sinh",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   79 */  { fnTan,                        NOPARAM,                     "tan",                                         "tan",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   80 */  { fnTanh,                       NOPARAM,                     "tanh",                                        "tanh",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   81 */  { fnArccos,                     NOPARAM,                     "arccos",                                      "arccos",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   82 */  { fnArccosh,                    NOPARAM,                     "arcosh",                                      "arcosh",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   83 */  { fnArcsin,                     NOPARAM,                     "arcsin",                                      "arcsin",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   84 */  { fnArcsinh,                    NOPARAM,                     "arsinh",                                      "arsinh",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   85 */  { fnArctan,                     NOPARAM,                     "arctan",                                      "arctan",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   86 */  { fnArctanh,                    NOPARAM,                     "artanh",                                      "artanh",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   87 */  { fnCeil,                       NOPARAM,                     "ceil",                                        "ceil",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   88 */  { fnFloor,                      NOPARAM,                     "floor",                                       "floor",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*   89 */  { fnGcd,                        NOPARAM,                     "GCD",                                         "GCD",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   90 */  { fnLcm,                        NOPARAM,                     "LCM",                                         "LCM",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   91 */  { fnDec,                        TM_REGISTER,                 "DEC",                                         "DEC",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   92 */  { fnInc,                        TM_REGISTER,                 "INC",                                         "INC",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   93 */  { fnIp,                         NOPARAM,                     "IP",                                          "IP",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   94 */  { fnFp,                         NOPARAM,                     "FP",                                          "FP",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   95 */  { fnAdd,                        ITM_ADD,                     "+",                                           "+",                                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   96 */  { fnSubtract,                   ITM_SUB,                     "-",                                           "-",                                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   97 */  { fnChangeSign,                 ITM_CHS,                     "+/-",                                         "+/-",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   98 */  { fnMultiply,                   ITM_MULT,                    STD_CROSS,                                     STD_CROSS,                                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*   99 */  { fnDivide,                     ITM_DIV,                     "/",                                           "/",                                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  100 */  { fnIDiv,                       NOPARAM,                     "IDIV",                                        "IDIV",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*  101 */  { fnView,                       TM_M_DIM,                    "VIEW",                                        "VIEW",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  102 */  { fnMod,                        NOPARAM,                     "MOD",                                         "MOD",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*  103 */  { fnMax,                        NOPARAM,                     "max",                                         "max",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*  104 */  { fnMin,                        NOPARAM,                     "min",                                         "min",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*  105 */  { fnMagnitude,                  NOPARAM,                     "|x|",                                         "|x|",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*  106 */  { fnNeighb,                     NOPARAM,                     "NEIGHB",                                      "NEIGHB",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  107 */  { fnNextPrime,                  NOPARAM,                     "NEXTP",                                       "NEXTP",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  108 */  { fnFactorial,                  NOPARAM,                     "x!",                                          "x!",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*  109 */  { fnPi,                         NOPARAM,                     STD_pi,                                        STD_pi,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  110 */  { fnClearFlag,                  TM_FLAGW,                    "CF",                                          "CF",                                          (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  111 */  { fnSetFlag,                    TM_FLAGW,                    "SF",                                          "SF",                                          (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  112 */  { fnFlipFlag,                   TM_FLAGW,                    "FF",                                          "FF",                                          (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  113 */  { fnCheckValue,                 CHECK_VALUE_MATRIX_SQUARE,   "M.SQR?",                                      "M.SQR?",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  114 */  { itemToBeCoded,                NOPARAM,                     "LITE",                                        "LITE",                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED}, // Literal in a PGM
/*  115 */  { fnCvtFromCurrentAngularMode,  amDegree,                    STD_RIGHT_ARROW "DEG",                         STD_RIGHT_ARROW "DEG",                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  116 */  { fnCvtFromCurrentAngularMode,  amDMS,                       STD_RIGHT_ARROW "D.MS",                        STD_RIGHT_ARROW "D.MS",                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  117 */  { fnCvtFromCurrentAngularMode,  amGrad,                      STD_RIGHT_ARROW "GRAD",                        STD_RIGHT_ARROW "GRAD",                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  118 */  { fnCvtFromCurrentAngularMode,  amMultPi,                    STD_RIGHT_ARROW "MUL" STD_pi,                  STD_RIGHT_ARROW "MUL" STD_pi,                  (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  119 */  { fnCvtFromCurrentAngularMode,  amRadian,                    STD_RIGHT_ARROW "RAD",                         STD_RIGHT_ARROW "RAD",                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  120 */  { fnCvtDegToRad,                NOPARAM,                     "D" STD_RIGHT_ARROW "R",                       "D" STD_RIGHT_ARROW "R",                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  121 */  { fnCvtRadToDeg,                NOPARAM,                     "R" STD_RIGHT_ARROW "D",                       "R" STD_RIGHT_ARROW "D",                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  122 */  { fnRmd,                        NOPARAM,                     "RMD",                                         "RMD",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/*  123 */  { fnLogicalNot,                 NOPARAM,                     "NOT",                                         "NOT",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  124 */  { fnLogicalAnd,                 NOPARAM,                     "AND",                                         "AND",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  125 */  { fnLogicalOr,                  NOPARAM,                     "OR",                                          "OR",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  126 */  { fnLogicalXor,                 NOPARAM,                     "XOR",                                         "XOR",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  127 */  { fnSwapX,                      TM_REGISTER,                 "x" STD_LEFT_RIGHT_ARROWS,                     "x" STD_LEFT_RIGHT_ARROWS,                     (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABL_XEQ | EIM_DISABLED},


// Items from 128 to ... are 2 byte OP codes
// Constants
/*  128 */  { fnConstant,                   0,                           "a",                                           "a",                                           (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  129 */  { fnConstant,                   1,                           "a" STD_SUB_0,                                 "a" STD_SUB_0,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  130 */  { fnConstant,                   2,                           "a" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "a" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  131 */  { fnConstant,                   3,                           "a" STD_SUB_EARTH,                             "a" STD_SUB_EARTH,                             (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  132 */  { fnConstant,                   4,                           "c",                                           "c",                                           (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  133 */  { fnConstant,                   5,                           "c" STD_SUB_1,                                 "c" STD_SUB_1,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  134 */  { fnConstant,                   6,                           "c" STD_SUB_2,                                 "c" STD_SUB_2,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  135 */  { fnConstant,                   7,                           "e",                                           "e",                                           (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  136 */  { fnConstant,                   8,                           "e" STD_SUB_E,                                 "e" STD_SUB_E,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  137 */  { fnConstant,                   9,                           "F",                                           "F",                                           (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  138 */  { fnConstant,                   10,                          "F" STD_SUB_alpha,                             "F" STD_SUB_alpha,                             (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  139 */  { fnConstant,                   11,                          "F" STD_SUB_delta,                             "F" STD_SUB_delta,                             (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  140 */  { fnConstant,                   12,                          "G",                                           "G",                                           (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  141 */  { fnConstant,                   13,                          "G" STD_SUB_0,                                 "G" STD_SUB_0,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  142 */  { fnConstant,                   14,                          "G" STD_SUB_C,                                 "G" STD_SUB_C,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  143 */  { fnConstant,                   15,                          "g" STD_SUB_e,                                 "g" STD_SUB_e,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  144 */  { fnConstant,                   16,                          "GM" STD_SUB_EARTH,                            "GM" STD_SUB_EARTH,                            (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  145 */  { fnConstant,                   17,                          "g" STD_SUB_EARTH,                             "g" STD_SUB_EARTH,                             (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  146 */  { fnConstant,                   18,                          STD_PLANCK,                                    STD_PLANCK,                                    (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  147 */  { fnConstant,                   19,                          STD_PLANCK_2PI,                                STD_PLANCK_2PI,                                (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  148 */  { fnConstant,                   20,                          "k",                                           "k",                                           (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  149 */  { fnConstant,                   21,                          "K" STD_SUB_J,                                 "K" STD_SUB_J,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  150 */  { fnConstant,                   22,                          "l" STD_SUB_P STD_SUB_L,                       "l" STD_SUB_P STD_SUB_L,                       (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  151 */  { fnConstant,                   23,                          "m" STD_SUB_e,                                 "m" STD_SUB_e,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  152 */  { fnConstant,                   24,                          "M" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "M" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  153 */  { fnConstant,                   25,                          "m" STD_SUB_n,                                 "m" STD_SUB_n,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  154 */  { fnConstant,                   26,                          "m" STD_SUB_n "/m" STD_SUB_p,                  "m" STD_SUB_n "/m" STD_SUB_p,                  (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  155 */  { fnConstant,                   27,                          "m" STD_SUB_p,                                 "m" STD_SUB_p,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  156 */  { fnConstant,                   28,                          "m" STD_SUB_P STD_SUB_L,                       "m" STD_SUB_P STD_SUB_L,                       (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  157 */  { fnConstant,                   29,                          "m" STD_SUB_p "/m" STD_SUB_e,                  "m" STD_SUB_p "/m" STD_SUB_e,                  (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  158 */  { fnConstant,                   30,                          "m" STD_SUB_u,                                 "m" STD_SUB_u,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  159 */  { fnConstant,                   31,                          "m" STD_SUB_u "c" STD_SUP_2,                   "m" STD_SUB_u "c" STD_SUP_2,                   (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  160 */  { fnConstant,                   32,                          "m" STD_SUB_mu,                                "m" STD_SUB_mu,                                (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  161 */  { fnConstant,                   33,                          "M" STD_SUB_SUN,                               "M" STD_SUB_SUN,                               (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  162 */  { fnConstant,                   34,                          "M" STD_SUB_EARTH,                             "M" STD_SUB_EARTH,                             (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  163 */  { fnConstant,                   35,                          "N" STD_SUB_A,                                 "N" STD_SUB_A,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  164 */  { fnConstant,                   36,                          "NaN",                                         "NaN",                                         (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  165 */  { fnConstant,                   37,                          "p" STD_SUB_0,                                 "p" STD_SUB_0,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  166 */  { fnConstant,                   38,                          "R",                                           "R",                                           (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  167 */  { fnConstant,                   39,                          "r" STD_SUB_e,                                 "r" STD_SUB_e,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  168 */  { fnConstant,                   40,                          "R" STD_SUB_K,                                 "R" STD_SUB_K,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  169 */  { fnConstant,                   41,                          "R" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   "R" STD_SUB_M STD_SUB_o STD_SUB_o STD_SUB_n,   (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  170 */  { fnConstant,                   42,                          "R" STD_SUB_INFINITY,                          "R" STD_SUB_INFINITY,                          (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  171 */  { fnConstant,                   43,                          "R" STD_SUB_SUN,                               "R" STD_SUB_SUN,                               (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  172 */  { fnConstant,                   44,                          "R" STD_SUB_EARTH,                             "R" STD_SUB_EARTH,                             (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  173 */  { fnConstant,                   45,                          "Sa",                                          "Sa",                                          (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  174 */  { fnConstant,                   46,                          "Sb",                                          "Sb",                                          (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  175 */  { fnConstant,                   47,                          "Se" STD_SUP_2,                                "Se" STD_SUP_2,                                (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  176 */  { fnConstant,                   48,                          "Se'" STD_SUP_2,                               "Se'" STD_SUP_2,                               (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  177 */  { fnConstant,                   49,                          "Sf" STD_SUP_MINUS_1,                          "Sf" STD_SUP_MINUS_1,                          (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  178 */  { fnConstant,                   50,                          "T" STD_SUB_0,                                 "T" STD_SUB_0,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  179 */  { fnConstant,                   51,                          "T" STD_SUB_p,                                 "T" STD_SUB_P,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  180 */  { fnConstant,                   52,                          "t" STD_SUB_P STD_SUB_L,                       "t" STD_SUB_P STD_SUB_L,                       (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  181 */  { fnConstant,                   53,                          "V" STD_SUB_m,                                 "V" STD_SUB_m,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  182 */  { fnConstant,                   54,                          "Z" STD_SUB_0,                                 "Z" STD_SUB_0,                                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  183 */  { fnConstant,                   55,                          STD_alpha,                                     STD_alpha,                                     (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  184 */  { fnConstant,                   56,                          STD_gamma,                                     STD_gamma,                                     (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  185 */  { fnConstant,                   57,                          STD_gamma STD_SUB_E STD_SUB_M,                 STD_gamma STD_SUB_E STD_SUB_M,                 (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  186 */  { fnConstant,                   58,                          STD_gamma STD_SUB_p,                           STD_gamma STD_SUB_p,                           (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  187 */  { fnConstant,                   59,                          STD_DELTA STD_nu STD_SUB_C STD_SUB_s,          STD_DELTA STD_nu STD_SUB_C STD_SUB_s,          (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  188 */  { fnConstant,                   60,                          STD_epsilon STD_SUB_0,                         STD_epsilon STD_SUB_0,                         (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  189 */  { fnConstant,                   61,                          STD_lambda STD_SUB_C,                          STD_lambda STD_SUB_C,                          (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  190 */  { fnConstant,                   62,                          STD_lambda STD_SUB_C STD_SUB_n,                STD_lambda STD_SUB_C STD_SUB_n,                (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  191 */  { fnConstant,                   63,                          STD_lambda STD_SUB_C STD_SUB_p,                STD_lambda STD_SUB_C STD_SUB_p,                (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  192 */  { fnConstant,                   64,                          STD_mu STD_SUB_0,                              STD_mu STD_SUB_0,                              (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  193 */  { fnConstant,                   65,                          STD_mu STD_SUB_B,                              STD_mu STD_SUB_B,                              (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  194 */  { fnConstant,                   66,                          STD_mu STD_SUB_e,                              STD_mu STD_SUB_e,                              (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  195 */  { fnConstant,                   67,                          STD_mu STD_SUB_e "/" STD_mu STD_SUB_B,         STD_mu STD_SUB_e "/" STD_mu STD_SUB_B,         (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  196 */  { fnConstant,                   68,                          STD_mu STD_SUB_n,                              STD_mu STD_SUB_n,                              (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  197 */  { fnConstant,                   69,                          STD_mu STD_SUB_p,                              STD_mu STD_SUB_p,                              (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  198 */  { fnConstant,                   70,                          STD_mu STD_SUB_u,                              STD_mu STD_SUB_u,                              (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  199 */  { fnConstant,                   71,                          STD_mu STD_SUB_mu,                             STD_mu STD_SUB_mu,                             (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  200 */  { fnConstant,                   72,                          STD_sigma STD_SUB_B,                           STD_sigma STD_SUB_B,                           (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  201 */  { fnConstant,                   73,                          STD_PHI,                                       STD_PHI,                                       (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  202 */  { fnConstant,                   74,                          STD_PHI STD_SUB_0,                             STD_PHI STD_SUB_0,                             (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  203 */  { fnConstant,                   75,                          STD_omega,                                     STD_omega,                                     (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  204 */  { fnConstant,                   76,                          "-" STD_INFINITY,                              "-" STD_INFINITY,                              (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  205 */  { fnConstant,                   77,                          STD_INFINITY,                                  STD_INFINITY,                                  (0 << TAM_MAX_BITS) |     0, CAT_CNST | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  206 */  { itemToBeCoded,                78,                          "#",                                           "#",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  207 */  { fnConstant,                   TM_VALUE,                    "CNST",                                        "CNST",                                        (0 << TAM_MAX_BITS) |   215, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED}, // 215 is replaced at run time by NUMBER_OF_CONSTANTS_39 + NUMBER_OF_CONSTANTS_51 + NUMBER_OF_CONSTANTS_1071 + NUMBER_OF_CONSTANTS_34 - 1
/*  208 */  { itemToBeCoded,                NOPARAM,                     "0208",                                        "0208",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  209 */  { itemToBeCoded,                NOPARAM,                     "0209",                                        "0209",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  210 */  { itemToBeCoded,                NOPARAM,                     "0210",                                        "0210",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  211 */  { itemToBeCoded,                NOPARAM,                     "0211",                                        "0211",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  212 */  { itemToBeCoded,                NOPARAM,                     "0212",                                        "0212",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  213 */  { itemToBeCoded,                NOPARAM,                     "0213",                                        "0213",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  214 */  { itemToBeCoded,                NOPARAM,                     "0214",                                        "0214",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  215 */  { itemToBeCoded,                NOPARAM,                     "0215",                                        "0215",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  216 */  { itemToBeCoded,                NOPARAM,                     "0216",                                        "0216",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  217 */  { itemToBeCoded,                NOPARAM,                     "0217",                                        "0217",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  218 */  { itemToBeCoded,                NOPARAM,                     "0218",                                        "0218",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  219 */  { itemToBeCoded,                NOPARAM,                     "0219",                                        "0219",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},


// Conversions
/*  220 */  { fnCvtCToF,                    NOPARAM,                     STD_DEGREE "C" STD_RIGHT_ARROW STD_DEGREE "F", STD_DEGREE "C" STD_RIGHT_ARROW STD_DEGREE "F", (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  221 */  { fnCvtFToC,                    NOPARAM,                     STD_DEGREE "F" STD_RIGHT_ARROW STD_DEGREE "C", STD_DEGREE "F" STD_RIGHT_ARROW STD_DEGREE "C", (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  222 */  { fnCvtDbRatio,                 10,                          "dB" STD_RIGHT_ARROW "pr",                     "dB " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  223 */  { fnCvtDbRatio,                 10,                          "dB" STD_RIGHT_ARROW "pr",                     "power",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  224 */  { fnCvtDbRatio,                 10,                          "dB" STD_RIGHT_ARROW "pr",                     "ratio",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  225 */  { fnCvtDbRatio,                 20,                          "dB" STD_RIGHT_ARROW "fr",                     "dB " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  226 */  { fnCvtDbRatio,                 20,                          "dB" STD_RIGHT_ARROW "fr",                     "field",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  227 */  { fnCvtDbRatio,                 20,                          "dB" STD_RIGHT_ARROW "fr",                     "ratio",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  228 */  { fnCvtRatioDb,                 10,                          "pr" STD_RIGHT_ARROW "dB",                     "power",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  229 */  { fnCvtRatioDb,                 10,                          "pr" STD_RIGHT_ARROW "dB",                     "ratio",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  230 */  { fnCvtRatioDb,                 10,                          "pr" STD_RIGHT_ARROW "dB",                     STD_RIGHT_ARROW " dB",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  231 */  { fnCvtRatioDb,                 20,                          "fr" STD_RIGHT_ARROW "dB",                     "field",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  232 */  { fnCvtRatioDb,                 20,                          "fr" STD_RIGHT_ARROW "dB",                     "ratio",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  233 */  { fnCvtRatioDb,                 20,                          "fr" STD_RIGHT_ARROW "dB",                     STD_RIGHT_ARROW " dB",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  234 */  { fnCvtAcreHa,                  multiply,                    "ac" STD_RIGHT_ARROW "ha",                     "acre",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  235 */  { fnCvtAcreHa,                  multiply,                    "ac" STD_RIGHT_ARROW "ha",                     STD_RIGHT_ARROW " ha",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  236 */  { fnCvtAcreHa,                  divide,                      "ha" STD_RIGHT_ARROW "ac",                     "ha " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  237 */  { fnCvtAcreHa,                  divide,                      "ha" STD_RIGHT_ARROW "ac",                     "acre",                                        (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  238 */  { fnCvtAcreusHa,                multiply,                    "ac" STD_US STD_RIGHT_ARROW "ha",              "acre" STD_US,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  239 */  { fnCvtAcreusHa,                multiply,                    "ac" STD_US STD_RIGHT_ARROW "ha",              STD_RIGHT_ARROW " ha",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  240 */  { fnCvtAcreusHa,                divide,                      "ha" STD_RIGHT_ARROW "ac" STD_US,              "ha " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  241 */  { fnCvtAcreusHa,                divide,                      "ha" STD_RIGHT_ARROW "ac" STD_US,              "acre" STD_US,                                 (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  242 */  { fnCvtAtmPa,                   divide,                      "Pa" STD_RIGHT_ARROW "atm",                    "Pa" STD_RIGHT_ARROW "atm",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  243 */  { fnCvtAtmPa,                   multiply,                    "atm" STD_RIGHT_ARROW "Pa",                    "atm" STD_RIGHT_ARROW "Pa",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  244 */  { fnCvtAuM,                     multiply,                    "au" STD_RIGHT_ARROW "m",                      "au" STD_RIGHT_ARROW "m",                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  245 */  { fnCvtAuM,                     divide,                      "m" STD_RIGHT_ARROW "au",                      "m" STD_RIGHT_ARROW "au",                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  246 */  { fnCvtBarPa,                   divide,                      "Pa" STD_RIGHT_ARROW "bar",                    "Pa" STD_RIGHT_ARROW "bar",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  247 */  { fnCvtBarPa,                   multiply,                    "bar" STD_RIGHT_ARROW "Pa",                    "bar" STD_RIGHT_ARROW "Pa",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  248 */  { fnCvtBtuJ,                    multiply,                    "Btu" STD_RIGHT_ARROW "J",                     "Btu" STD_RIGHT_ARROW "J",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  249 */  { fnCvtBtuJ,                    divide,                      "J" STD_RIGHT_ARROW "Btu",                     "J" STD_RIGHT_ARROW "Btu",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  250 */  { fnCvtCalJ,                    multiply,                    "cal" STD_RIGHT_ARROW "J",                     "cal" STD_RIGHT_ARROW "J",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  251 */  { fnCvtCalJ,                    divide,                      "J" STD_RIGHT_ARROW "cal",                     "J" STD_RIGHT_ARROW "cal",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  252 */  { fnCvtLbfftNm,                 multiply,                    "lbft" STD_RIGHT_SHORT_ARROW "Nm",             "lbf" STD_DOT "ft",                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  253 */  { fnCvtLbfftNm,                 multiply,                    "lbft" STD_RIGHT_SHORT_ARROW "Nm",             STD_RIGHT_ARROW " Nm",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  254 */  { fnCvtLbfftNm,                 divide,                      "Nm" STD_RIGHT_SHORT_ARROW "lbft",             "Nm " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  255 */  { fnCvtLbfftNm,                 divide,                      "Nm" STD_RIGHT_SHORT_ARROW "lbft",             "lbf" STD_DOT "ft",                            (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  256 */  { fnCvtCwtKg,                   multiply,                    "cwt" STD_RIGHT_ARROW "kg",                    "cwt" STD_RIGHT_ARROW "kg",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  257 */  { fnCvtCwtKg,                   divide,                      "kg" STD_RIGHT_ARROW "cwt",                    "kg" STD_RIGHT_ARROW "cwt",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  258 */  { fnCvtFtM,                     multiply,                    "ft." STD_RIGHT_ARROW "m",                     "ft." STD_RIGHT_ARROW "m",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  259 */  { fnCvtFtM,                     divide,                      "m" STD_RIGHT_ARROW "ft.",                     "m" STD_RIGHT_ARROW "ft.",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  260 */  { fnCvtSfeetM,                  multiply,                    "ft" STD_US STD_RIGHT_ARROW "m",               "survey",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  261 */  { fnCvtSfeetM,                  multiply,                    "ft" STD_US STD_RIGHT_ARROW "m",               "foot" STD_US,                                 (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  262 */  { fnCvtSfeetM,                  multiply,                    "ft" STD_US STD_RIGHT_ARROW "m",               STD_RIGHT_ARROW " m",                          (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  263 */  { fnCvtSfeetM,                  divide,                      "m" STD_RIGHT_ARROW "ft" STD_US,               "m " STD_RIGHT_ARROW,                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  264 */  { fnCvtSfeetM,                  divide,                      "m" STD_RIGHT_ARROW "ft" STD_US,               "survey",                                      (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  265 */  { fnCvtSfeetM,                  divide,                      "m" STD_RIGHT_ARROW "ft" STD_US,               "foot" STD_US,                                 (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  266 */  { fnCvtFlozukMl,                multiply,                    "fz" STD_UK STD_RIGHT_ARROW "ml",              "floz" STD_UK,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  267 */  { fnCvtFlozukMl,                multiply,                    "fz" STD_UK STD_RIGHT_ARROW "ml",              STD_RIGHT_ARROW " ml",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  268 */  { fnCvtFlozukMl,                divide,                      "ml" STD_RIGHT_ARROW "fz" STD_UK,              "ml " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  269 */  { fnCvtFlozukMl,                divide,                      "ml" STD_RIGHT_ARROW "fz" STD_UK,              "floz" STD_UK,                                 (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  270 */  { fnCvtFlozusMl,                multiply,                    "fz" STD_US STD_RIGHT_ARROW "ml",              "floz" STD_US,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  271 */  { fnCvtFlozusMl,                multiply,                    "fz" STD_US STD_RIGHT_ARROW "ml",              STD_RIGHT_ARROW " ml",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  272 */  { fnCvtFlozusMl,                divide,                      "ml" STD_RIGHT_ARROW "fz" STD_US,              "ml " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  273 */  { fnCvtFlozusMl,                divide,                      "ml" STD_RIGHT_ARROW "fz" STD_US,              "floz" STD_US,                                 (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  274 */  { fnCvtGalukL,                  multiply,                    "gal" STD_UK STD_RIGHT_ARROW "l",              "gal" STD_UK STD_RIGHT_ARROW "l",              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  275 */  { fnCvtGalukL,                  divide,                      "l" STD_RIGHT_ARROW "gal" STD_UK,              "l" STD_RIGHT_ARROW "gal" STD_UK,              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  276 */  { fnCvtGalusL,                  multiply,                    "gal" STD_US STD_RIGHT_ARROW "l",              "gal" STD_US STD_RIGHT_ARROW "l",              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  277 */  { fnCvtGalusL,                  divide,                      "l" STD_RIGHT_ARROW "gal" STD_US,              "l" STD_RIGHT_ARROW "gal" STD_US,              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  278 */  { fnCvtHpeW,                    multiply,                    "hp" STD_SUB_E STD_RIGHT_ARROW "W",            "hp" STD_SUB_E STD_RIGHT_ARROW "W",            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  279 */  { fnCvtHpeW,                    divide,                      "W" STD_RIGHT_ARROW "hp" STD_SUB_E,            "W" STD_RIGHT_ARROW "hp" STD_SUB_E,            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  280 */  { fnCvtHpmW,                    multiply,                    "hp" STD_SUB_M STD_RIGHT_ARROW "W",            "hp" STD_SUB_M STD_RIGHT_ARROW "W",            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  281 */  { fnCvtHpmW,                    divide,                      "W" STD_RIGHT_ARROW "hp" STD_SUB_M,            "W" STD_RIGHT_ARROW "hp" STD_SUB_M,            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  282 */  { fnCvtHpukW,                   multiply,                    "hp" STD_UK STD_RIGHT_ARROW "W",               "hp" STD_UK STD_RIGHT_ARROW "W",               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  283 */  { fnCvtHpukW,                   divide,                      "W" STD_RIGHT_ARROW "hp" STD_UK,               "W" STD_RIGHT_ARROW "hp" STD_UK,               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  284 */  { fnCvtInhgPa,                  multiply,                    "iHg" STD_RIGHT_ARROW "Pa",                    "in.Hg",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  285 */  { fnCvtInhgPa,                  multiply,                    "iHg" STD_RIGHT_ARROW "Pa",                    STD_RIGHT_ARROW " Pa",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  286 */  { fnCvtInhgPa,                  divide,                      "Pa" STD_RIGHT_ARROW "iHg",                    "Pa " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  287 */  { fnCvtInhgPa,                  divide,                      "Pa" STD_RIGHT_ARROW "iHg",                    "in.Hg",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  288 */  { fnCvtInchMm,                  multiply,                    "in." STD_RIGHT_ARROW "mm",                    "in." STD_RIGHT_ARROW "mm",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  289 */  { fnCvtInchMm,                  divide,                      "mm" STD_RIGHT_ARROW "in.",                    "mm" STD_RIGHT_ARROW "in.",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  290 */  { fnCvtWhJ,                     multiply,                    "Wh" STD_RIGHT_ARROW "J",                      "Wh" STD_RIGHT_ARROW "J",                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  291 */  { fnCvtWhJ,                     divide,                      "J" STD_RIGHT_ARROW "Wh",                      "J" STD_RIGHT_ARROW "Wh",                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  292 */  { fnCvtLbKg,                    divide,                      "kg" STD_RIGHT_ARROW "lb.",                    "kg" STD_RIGHT_ARROW "lb.",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  293 */  { fnCvtLbKg,                    multiply,                    "lb." STD_RIGHT_ARROW "kg",                    "lb." STD_RIGHT_ARROW "kg",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  294 */  { fnCvtOzG,                     divide,                      "g" STD_RIGHT_ARROW "oz",                      "g" STD_RIGHT_ARROW "oz",                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  295 */  { fnCvtOzG,                     multiply,                    "oz" STD_RIGHT_ARROW "g",                      "oz" STD_RIGHT_ARROW "g",                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  296 */  { fnCvtShortcwtKg,              divide,                      "kg" STD_RIGHT_ARROW "scw",                    "kg " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  297 */  { fnCvtShortcwtKg,              divide,                      "kg" STD_RIGHT_ARROW "scw",                    "sh.cwt",                                      (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  298 */  { fnCvtShortcwtKg,              multiply,                    "scw" STD_RIGHT_ARROW "kg",                    "short",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  299 */  { fnCvtShortcwtKg,              multiply,                    "scw" STD_RIGHT_ARROW "kg",                    "cwt" STD_RIGHT_ARROW "kg",                    (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  300 */  { fnCvtStoneKg,                 divide,                      "kg" STD_RIGHT_ARROW "sto",                    "kg " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  301 */  { fnCvtStoneKg,                 divide,                      "kg" STD_RIGHT_ARROW "sto",                    "stone",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  302 */  { fnCvtStoneKg,                 multiply,                    "sto" STD_RIGHT_ARROW "kg",                    "stone",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  303 */  { fnCvtStoneKg,                 multiply,                    "sto" STD_RIGHT_ARROW "kg",                    STD_RIGHT_ARROW " kg",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  304 */  { fnCvtShorttonKg,              divide,                      "kg" STD_RIGHT_ARROW "s.t",                    "kg " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  305 */  { fnCvtShorttonKg,              divide,                      "kg" STD_RIGHT_ARROW "s.t",                    "short",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  306 */  { fnCvtShorttonKg,              divide,                      "kg" STD_RIGHT_ARROW "s.t",                    "ton",                                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  307 */  { fnCvtShorttonKg,              multiply,                    "s.t" STD_RIGHT_ARROW "kg",                    "short",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  308 */  { fnCvtShorttonKg,              multiply,                    "s.t" STD_RIGHT_ARROW "kg",                    "ton",                                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  309 */  { fnCvtShorttonKg,              multiply,                    "s.t" STD_RIGHT_ARROW "kg",                    STD_RIGHT_ARROW " kg",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  310 */  { fnCvtTonKg,                   divide,                      "kg" STD_RIGHT_ARROW "ton",                    "kg" STD_RIGHT_ARROW "ton",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  311 */  { fnCvtLiangKg,                 multiply,                    "kg" STD_RIGHT_ARROW "lian",                   "kg " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  312 */  { fnCvtLiangKg,                 multiply,                    "kg" STD_RIGHT_ARROW "lian",                   "li" STD_a_BREVE "ng",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  313 */  { fnCvtTonKg,                   multiply,                    "ton" STD_RIGHT_ARROW "kg",                    "ton" STD_RIGHT_ARROW "kg",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  314 */  { fnCvtLiangKg,                 divide,                      "lian" STD_RIGHT_ARROW "kg",                   "li" STD_a_BREVE "ng",                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  315 */  { fnCvtLiangKg,                 divide,                      "lian" STD_RIGHT_ARROW "kg",                   STD_RIGHT_ARROW " kg",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  316 */  { fnCvtTrozG,                   divide,                      "g" STD_RIGHT_ARROW "trz",                     "g " STD_RIGHT_ARROW,                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  317 */  { fnCvtTrozG,                   divide,                      "g" STD_RIGHT_ARROW "trz",                     "tr.oz",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  318 */  { fnCvtTrozG,                   multiply,                    "trz" STD_RIGHT_ARROW "g",                     "tr.oz",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  319 */  { fnCvtTrozG,                   multiply,                    "trz" STD_RIGHT_ARROW "g",                     STD_RIGHT_ARROW " g",                          (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  320 */  { fnCvtLbfN,                    multiply,                    "lbf" STD_RIGHT_ARROW "N",                     "lbf" STD_RIGHT_ARROW "N",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  321 */  { fnCvtLbfN,                    divide,                      "N" STD_RIGHT_ARROW "lbf",                     "N" STD_RIGHT_ARROW "lbf",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  322 */  { fnCvtLyM,                     multiply,                    "l.y." STD_RIGHT_ARROW "m",                    "l.y." STD_RIGHT_ARROW "m",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  323 */  { fnCvtLyM,                     divide,                      "m" STD_RIGHT_ARROW "l.y.",                    "m" STD_RIGHT_ARROW "l.y.",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  324 */  { fnCvtMmhgPa,                  multiply,                    "mmH" STD_RIGHT_SHORT_ARROW "Pa",              "mm.Hg",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  325 */  { fnCvtMmhgPa,                  multiply,                    "mmH" STD_RIGHT_SHORT_ARROW "Pa",              STD_RIGHT_ARROW " Pa",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  326 */  { fnCvtMmhgPa,                  divide,                      "Pa" STD_RIGHT_SHORT_ARROW "mmH",              "Pa " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  327 */  { fnCvtMmhgPa,                  divide,                      "Pa" STD_RIGHT_SHORT_ARROW "mmH",              "mm.Hg",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  328 */  { fnCvtMiKm,                    multiply,                    "mi." STD_RIGHT_ARROW "km",                    "mi." STD_RIGHT_ARROW "km",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  329 */  { fnCvtMiKm,                    divide,                      "km" STD_RIGHT_ARROW "mi.",                    "km" STD_RIGHT_ARROW "mi.",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  330 */  { fnCvtNmiKm,                   divide,                      "km" STD_RIGHT_ARROW "nmi",                    "km" STD_RIGHT_ARROW "nmi",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  331 */  { fnCvtNmiKm,                   multiply,                    "nmi" STD_RIGHT_ARROW "km",                    "nmi" STD_RIGHT_ARROW "km",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  332 */  { fnCvtPcM,                     divide,                      "m" STD_RIGHT_ARROW "pc",                      "m" STD_RIGHT_ARROW "pc",                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  333 */  { fnCvtPcM,                     multiply,                    "pc" STD_RIGHT_ARROW "m",                      "pc" STD_RIGHT_ARROW "m",                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  334 */  { fnCvtPointMm,                 divide,                      "mm" STD_RIGHT_ARROW "pt.",                    "mm " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  335 */  { fnCvtPointMm,                 divide,                      "mm" STD_RIGHT_ARROW "pt.",                    "point",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  336 */  { fnCvtPointMm,                 divide,                      "mm" STD_RIGHT_ARROW "pt.",                    STD_SPACE_HAIR,                                (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  337 */  { fnCvtPointMm,                 multiply,                    "pt." STD_RIGHT_ARROW "mm",                    "point",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  338 */  { fnCvtPointMm,                 multiply,                    "pt." STD_RIGHT_ARROW "mm",                    STD_RIGHT_ARROW " mm",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  339 */  { fnCvtPointMm,                 multiply,                    "pt." STD_RIGHT_ARROW "mm",                    STD_SPACE_HAIR,                                (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  340 */  { fnCvtYardM,                   divide,                      "m" STD_RIGHT_ARROW "yd.",                     "m" STD_RIGHT_ARROW "yd.",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  341 */  { fnCvtYardM,                   multiply,                    "yd." STD_RIGHT_ARROW "m",                     "yd." STD_RIGHT_ARROW "m",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  342 */  { fnCvtPsiPa,                   multiply,                    "psi" STD_RIGHT_ARROW "Pa",                    "psi" STD_RIGHT_ARROW "Pa",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  343 */  { fnCvtPsiPa,                   divide,                      "Pa" STD_RIGHT_ARROW "psi",                    "Pa" STD_RIGHT_ARROW "psi",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  344 */  { fnCvtTorrPa,                  divide,                      "Pa" STD_RIGHT_ARROW "tor",                    "Pa " STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  345 */  { fnCvtTorrPa,                  divide,                      "Pa" STD_RIGHT_ARROW "tor",                    "torr",                                        (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  346 */  { fnCvtTorrPa,                  multiply,                    "tor" STD_RIGHT_ARROW "Pa",                    "torr",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  347 */  { fnCvtTorrPa,                  multiply,                    "tor" STD_RIGHT_ARROW "Pa",                    STD_RIGHT_ARROW " Pa",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  348 */  { fnCvtYearS,                   divide,                      "s" STD_RIGHT_ARROW "year",                    "s" STD_RIGHT_ARROW "year",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  349 */  { fnCvtYearS,                   multiply,                    "year" STD_RIGHT_ARROW "s",                    "year" STD_RIGHT_ARROW "s",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  350 */  { fnCvtCaratG,                  multiply,                    "ct" STD_RIGHT_ARROW "g",                      "carat",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  351 */  { fnCvtCaratG,                  multiply,                    "ct" STD_RIGHT_ARROW "g",                      STD_RIGHT_ARROW " g",                          (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  352 */  { fnCvtJinKg,                   divide,                      "j" STD_i_MACRON "n" STD_RIGHT_ARROW "kg",     "j" STD_i_MACRON "n" STD_RIGHT_ARROW "kg",     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  353 */  { fnCvtCaratG,                  divide,                      "g" STD_RIGHT_ARROW "ct",                      "g " STD_RIGHT_ARROW,                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  354 */  { fnCvtCaratG,                  divide,                      "g" STD_RIGHT_ARROW "ct",                      "carat",                                       (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  355 */  { fnCvtJinKg,                   multiply,                    "kg" STD_RIGHT_ARROW "j" STD_i_MACRON "n",     "kg" STD_RIGHT_ARROW "j" STD_i_MACRON "n",     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  356 */  { fnCvtQuartL,                  multiply,                    "qt." STD_RIGHT_ARROW "l",                     "qt." STD_RIGHT_ARROW "l",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  357 */  { fnCvtQuartL,                  divide,                      "l" STD_RIGHT_ARROW "qt.",                     "l" STD_RIGHT_ARROW "qt.",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  358 */  { fnCvtFathomM,                 multiply,                    "fm." STD_RIGHT_ARROW "m",                     "fathom",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  359 */  { fnCvtFathomM,                 multiply,                    "fm." STD_RIGHT_ARROW "m",                     STD_RIGHT_ARROW " m",                          (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  360 */  { fnCvtFathomM,                 multiply,                    "fm." STD_RIGHT_ARROW "m",                     STD_SPACE_HAIR,                                (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  361 */  { fnCvtFathomM,                 divide,                      "m" STD_RIGHT_ARROW "fm.",                     "m " STD_RIGHT_ARROW,                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  362 */  { fnCvtFathomM,                 divide,                      "m" STD_RIGHT_ARROW "fm.",                     "fathom",                                      (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  363 */  { fnCvtFathomM,                 divide,                      "m" STD_RIGHT_ARROW "fm.",                     STD_SPACE_HAIR,                                (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  364 */  { fnCvtBarrelM3,                multiply,                    "bbl" STD_RIGHT_ARROW "m" STD_SUP_3,           "barrel",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  365 */  { fnCvtBarrelM3,                multiply,                    "bbl" STD_RIGHT_ARROW "m" STD_SUP_3,           STD_RIGHT_ARROW " m" STD_SUP_3,                (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  366 */  { fnCvtBarrelM3,                divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "bbl",           STD_RIGHT_ARROW " m" STD_SUP_3,                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  367 */  { fnCvtBarrelM3,                divide,                      "m" STD_SUP_3 STD_RIGHT_ARROW "bbl",           "barrel",                                      (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  368 */  { fnCvtAtmPa,                   multiply,                    "atm" STD_RIGHT_ARROW "Pa",                    STD_SPACE_HAIR,                                (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  369 */  { fnCvtAtmPa,                   divide,                      "Pa" STD_RIGHT_ARROW "atm",                    STD_SPACE_HAIR,                                (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  370 */  { fnCvtHectareM2,               multiply,                    "ha" STD_RIGHT_ARROW "m" STD_SUP_2,            "ha" STD_RIGHT_ARROW "m" STD_SUP_2,            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  371 */  { fnCvtHectareM2,               divide,                      "m" STD_SUP_2 STD_RIGHT_ARROW "ha",            "m" STD_SUP_2 STD_RIGHT_ARROW "ha",            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  372 */  { fnCvtMuM2,                    divide,                      "m" STD_u_BREVE STD_RIGHT_ARROW "m" STD_SUP_2, "m" STD_u_BREVE STD_RIGHT_ARROW "m" STD_SUP_2, (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  373 */  { fnCvtMuM2,                    multiply,                    "m" STD_SUP_2 STD_RIGHT_ARROW "m" STD_u_BREVE, "m" STD_SUP_2 STD_RIGHT_ARROW "m" STD_u_BREVE, (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  374 */  { fnCvtLiM,                     multiply,                    "l" STD_i_BREVE STD_RIGHT_ARROW "m",           "l" STD_i_BREVE STD_RIGHT_ARROW "m",           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  375 */  { fnCvtLiM,                     divide,                      "m" STD_RIGHT_ARROW "l" STD_i_BREVE,           "m" STD_RIGHT_ARROW "l" STD_i_BREVE,           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  376 */  { fnCvtChiM,                    divide,                      "ch" STD_i_BREVE STD_RIGHT_ARROW "m",          "ch" STD_i_BREVE STD_RIGHT_ARROW "m",          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  377 */  { fnCvtChiM,                    multiply,                    "m" STD_RIGHT_ARROW "ch" STD_i_BREVE,          "m" STD_RIGHT_ARROW "ch" STD_i_BREVE,          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  378 */  { fnCvtYinM,                    divide,                      "y" STD_i_BREVE "n" STD_RIGHT_ARROW "m",       "y" STD_i_BREVE "n" STD_RIGHT_ARROW "m",       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  379 */  { fnCvtYinM,                    multiply,                    "m" STD_RIGHT_ARROW "y" STD_i_BREVE "n",       "m" STD_RIGHT_ARROW "y" STD_i_BREVE "n",       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  380 */  { fnCvtCunM,                    divide,                      "c" STD_u_GRAVE "n" STD_RIGHT_ARROW "m",       "c" STD_u_GRAVE "n" STD_RIGHT_ARROW "m",       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  381 */  { fnCvtCunM,                    multiply,                    "m" STD_RIGHT_ARROW "c" STD_u_GRAVE "n",       "m" STD_RIGHT_ARROW "c" STD_u_GRAVE "n",       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  382 */  { fnCvtZhangM,                  divide,                      "zh" STD_a_GRAVE "n" STD_RIGHT_ARROW "m",      "zh" STD_a_GRAVE "ng",                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  383 */  { fnCvtZhangM,                  divide,                      "zh" STD_a_GRAVE "n" STD_RIGHT_ARROW "m",      STD_RIGHT_ARROW " m",                          (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  384 */  { fnCvtZhangM,                  multiply,                    "m" STD_RIGHT_ARROW "zh" STD_a_GRAVE "n",      "m " STD_RIGHT_ARROW,                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  385 */  { fnCvtZhangM,                  multiply,                    "m" STD_RIGHT_ARROW "zh" STD_a_GRAVE "n",      "zh" STD_a_GRAVE "ng",                         (0 << TAM_MAX_BITS) |     0, CAT_DUPL | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  386 */  { fnCvtFenM,                    divide,                      "f" STD_e_MACRON "n" STD_RIGHT_ARROW "m",      "f" STD_e_MACRON "n" STD_RIGHT_ARROW "m",      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  387 */  { fnCvtFenM,                    multiply,                    "m" STD_RIGHT_ARROW "f" STD_e_MACRON "n",      "m" STD_RIGHT_ARROW "f" STD_e_MACRON "n",      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  388 */  { itemToBeCoded,                NOPARAM,                     "0388",                                        "0388",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  389 */  { itemToBeCoded,                NOPARAM,                     "0389",                                        "0389",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  390 */  { itemToBeCoded,                NOPARAM,                     "0390",                                        "0390",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  391 */  { itemToBeCoded,                NOPARAM,                     "0391",                                        "0391",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  392 */  { itemToBeCoded,                NOPARAM,                     "0392",                                        "0392",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  393 */  { itemToBeCoded,                NOPARAM,                     "0393",                                        "0393",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  394 */  { itemToBeCoded,                NOPARAM,                     "0394",                                        "0394",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  395 */  { itemToBeCoded,                NOPARAM,                     "0395",                                        "0395",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},


// Flag, bit, rotation, and logical OPs
/*  396 */  { fnIsFlagClearClear,           TM_FLAGW,                    "FC?C",                                        "FC?C",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  397 */  { fnIsFlagClearSet,             TM_FLAGW,                    "FC?S",                                        "FC?S",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  398 */  { fnIsFlagClearFlip,            TM_FLAGW,                    "FC?F",                                        "FC?F",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  399 */  { fnIsFlagSetClear,             TM_FLAGW,                    "FS?C",                                        "FS?C",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  400 */  { fnIsFlagSetSet,               TM_FLAGW,                    "FS?S",                                        "FS?S",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  401 */  { fnIsFlagSetFlip,              TM_FLAGW,                    "FS?F",                                        "FS?F",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  402 */  { fnLogicalNand,                NOPARAM,                     "NAND",                                        "NAND",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  403 */  { fnLogicalNor,                 NOPARAM,                     "NOR",                                         "NOR",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  404 */  { fnLogicalXnor,                NOPARAM,                     "XNOR",                                        "XNOR",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  405 */  { fnBs,                         TM_VALUE,                    "BS?",                                         "BS?",                                         (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  406 */  { fnBc,                         TM_VALUE,                    "BC?",                                         "BC?",                                         (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  407 */  { fnCb,                         TM_VALUE,                    "CB",                                          "CB",                                          (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  408 */  { fnSb,                         TM_VALUE,                    "SB",                                          "SB",                                          (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  409 */  { fnFb,                         TM_VALUE,                    "FB",                                          "FB",                                          (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  410 */  { fnRl,                         TM_VALUE,                    "RL",                                          "RL",                                          (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  411 */  { fnRlc,                        TM_VALUE,                    "RLC",                                         "RLC",                                         (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  412 */  { fnRr,                         TM_VALUE,                    "RR",                                          "RR",                                          (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  413 */  { fnRrc,                        TM_VALUE,                    "RRC",                                         "RRC",                                         (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  414 */  { fnSl,                         TM_VALUE,                    "SL",                                          "SL",                                          (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  415 */  { fnSr,                         TM_VALUE,                    "SR",                                          "SR",                                          (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  416 */  { fnAsr,                        TM_VALUE,                    "ASR",                                         "ASR",                                         (0 << TAM_MAX_BITS) |    63, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  417 */  { fnLj,                         NOPARAM,                     "LJ",                                          "LJ",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  418 */  { fnRj,                         NOPARAM,                     "RJ",                                          "RJ",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  419 */  { fnMaskl,                      TM_VALUE,                    "MASKL",                                       "MASKL",                                       (0 << TAM_MAX_BITS) |    64, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  420 */  { fnMaskr,                      TM_VALUE,                    "MASKR",                                       "MASKR",                                       (0 << TAM_MAX_BITS) |    64, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  421 */  { fnMirror,                     NOPARAM,                     "MIRROR",                                      "MIRROR",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  422 */  { fnCountBits,                  NOPARAM,                     "#B",                                          "#B",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  423 */  { fnSdl,                        TM_VALUE,                    "SDL",                                         "SDL",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  424 */  { fnSdr,                        TM_VALUE,                    "SDR",                                         "SDR",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  425 */  { itemToBeCoded,                NOPARAM,                     "0425",                                        "0425",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  426 */  { itemToBeCoded,                NOPARAM,                     "0426",                                        "0426",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  427 */  { itemToBeCoded,                NOPARAM,                     "0427",                                        "0427",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  428 */  { itemToBeCoded,                NOPARAM,                     "0428",                                        "0428",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  429 */  { itemToBeCoded,                NOPARAM,                     "0429",                                        "0429",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  430 */  { itemToBeCoded,                NOPARAM,                     "0430",                                        "0430",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  431 */  { itemToBeCoded,                NOPARAM,                     "0431",                                        "0431",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  432 */  { itemToBeCoded,                NOPARAM,                     "0432",                                        "0432",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},


// Statistical sums
/*  433 */  { fnSigma,                      1,                           STD_SIGMA "+",                                 STD_SIGMA "+",                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_DISABLED  | US_ENABLED   | EIM_DISABLED},
/*  434 */  { fnSigma,                      2,                           STD_SIGMA "-",                                 STD_SIGMA "-",                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_DISABLED  | US_ENABLED   | EIM_DISABLED},
/*  435 */  { fnStatSum,                    0,                           "n" STD_SIGMA,                                 "n",                                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  436 */  { fnStatSum,                    SUM_X,                       STD_SIGMA "x",                                 STD_SIGMA "x",                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  437 */  { fnStatSum,                    SUM_Y,                       STD_SIGMA "y",                                 STD_SIGMA "y",                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  438 */  { fnStatSum,                    SUM_X2,                      STD_SIGMA "x" STD_SUP_2,                       STD_SIGMA "x" STD_SUP_2,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  439 */  { fnStatSum,                    SUM_X2Y,                     STD_SIGMA "x" STD_SUP_2 "y",                   STD_SIGMA "x" STD_SUP_2 "y",                   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  440 */  { fnStatSum,                    SUM_Y2,                      STD_SIGMA "y" STD_SUP_2,                       STD_SIGMA "y" STD_SUP_2,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  441 */  { fnStatSum,                    SUM_XY,                      STD_SIGMA "xy",                                STD_SIGMA "xy",                                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  442 */  { fnStatSum,                    SUM_lnXlnY,                  STD_SIGMA "lnxy",                              STD_SIGMA "lnxy",                              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  443 */  { fnStatSum,                    SUM_lnX,                     STD_SIGMA "lnx",                               STD_SIGMA "lnx",                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  444 */  { fnStatSum,                    SUM_ln2X,                    STD_SIGMA "ln" STD_SUP_2 "x",                  STD_SIGMA "ln" STD_SUP_2 "x",                  (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  445 */  { fnStatSum,                    SUM_YlnX,                    STD_SIGMA "ylnx",                              STD_SIGMA "ylnx",                              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  446 */  { fnStatSum,                    SUM_lnY,                     STD_SIGMA "lny",                               STD_SIGMA "lny",                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  447 */  { fnStatSum,                    SUM_ln2Y,                    STD_SIGMA "ln" STD_SUP_2 "y",                  STD_SIGMA "ln" STD_SUP_2 "y",                  (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  448 */  { fnStatSum,                    SUM_XlnY,                    STD_SIGMA "xlny",                              STD_SIGMA "xlny",                              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  449 */  { fnStatSum,                    SUM_X2lnY,                   STD_SIGMA "x" STD_SUP_2 "lny",                 STD_SIGMA "x" STD_SUP_2 "lny",                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  450 */  { fnStatSum,                    SUM_lnYonX,                  STD_SIGMA "lny/x",                             STD_SIGMA "lny/x",                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  451 */  { fnStatSum,                    SUM_X2onY,                   STD_SIGMA "x" STD_SUP_2 "/y",                  STD_SIGMA "x" STD_SUP_2 "/y",                  (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  452 */  { fnStatSum,                    SUM_1onX,                    STD_SIGMA STD_SUP_1 "/x",                      STD_SIGMA STD_SUP_1 "/x",                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  453 */  { fnStatSum,                    SUM_1onX2,                   STD_SIGMA STD_SUP_1 "/x" STD_SUP_2,            STD_SIGMA STD_SUP_1 "/x" STD_SUP_2,            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  454 */  { fnStatSum,                    SUM_XonY,                    STD_SIGMA "x/y",                               STD_SIGMA "x/y",                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  455 */  { fnStatSum,                    SUM_1onY,                    STD_SIGMA STD_SUP_1 "/y",                      STD_SIGMA STD_SUP_1 "/y",                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  456 */  { fnStatSum,                    SUM_1onY2,                   STD_SIGMA STD_SUP_1 "/y" STD_SUP_2,            STD_SIGMA STD_SUP_1 "/y" STD_SUP_2,            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  457 */  { fnStatSum,                    SUM_X3,                      STD_SIGMA "x" STD_SUP_3,                       STD_SIGMA "x" STD_SUP_3,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  458 */  { fnStatSum,                    SUM_X4,                      STD_SIGMA "x" STD_SUP_4,                       STD_SIGMA "x" STD_SUP_4,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/*  459 */  { itemToBeCoded,                NOPARAM,                     "0458",                                        "0458",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  460 */  { itemToBeCoded,                NOPARAM,                     "0459",                                        "0459",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  461 */  { itemToBeCoded,                NOPARAM,                     "0460",                                        "0460",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  462 */  { itemToBeCoded,                NOPARAM,                     "0461",                                        "0461",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},


// System flags
/*  463 */  { fnGetSystemFlag,              FLAG_TDM24,                  "TDM24",                                       "TDM24",                                       (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED}, // The system flags,
/*  464 */  { fnGetSystemFlag,              FLAG_YMD,                    "YMD",                                         "YMD",                                         (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED}, // items from 453 to 493,
/*  465 */  { fnGetSystemFlag,              FLAG_DMY,                    "DMY",                                         "DMY",                                         (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED}, // MUST be in the same
/*  466 */  { fnGetSystemFlag,              FLAG_MDY,                    "MDY",                                         "MDY",                                         (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED}, // order as the flag
/*  467 */  { fnGetSystemFlag,              FLAG_CPXRES,                 "CPXRES",                                      "CPXRES",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED}, // number (8 LSB) defined
/*  468 */  { fnGetSystemFlag,              FLAG_CPXj,                   "CPXj",                                        "CPXj",                                        (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED}, // in defines.h
/*  469 */  { fnGetSystemFlag,              FLAG_POLAR,                  "POLAR",                                       "POLAR",                                       (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  470 */  { fnGetSystemFlag,              FLAG_FRACT,                  "FRACT",                                       "FRACT",                                       (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED}, // And TDM24 MUST be
/*  471 */  { fnGetSystemFlag,              FLAG_PROPFR,                 "PROPFR",                                      "PROPFR",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED}, // the first.
/*  472 */  { fnGetSystemFlag,              FLAG_DENANY,                 "DENANY",                                      "DENANY",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  473 */  { fnGetSystemFlag,              FLAG_DENFIX,                 "DENFIX",                                      "DENFIX",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  474 */  { fnGetSystemFlag,              FLAG_CARRY,                  "CARRY",                                       "CARRY",                                       (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  475 */  { fnGetSystemFlag,              FLAG_OVERFLOW,               "OVERFL",                                      "OVERFL",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  476 */  { fnGetSystemFlag,              FLAG_LEAD0,                  "LEAD.0",                                      "LEAD.0",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  477 */  { fnGetSystemFlag,              FLAG_ALPHA,                  "ALPHA",                                       "ALPHA",                                       (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  478 */  { fnGetSystemFlag,              FLAG_alphaCAP,               STD_alpha "CAP",                               STD_alpha "CAP",                               (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  479 */  { fnGetSystemFlag,              FLAG_RUNTIM,                 "RUNTIM",                                      "RUNTIM",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  480 */  { fnGetSystemFlag,              FLAG_RUNIO,                  "RUNIO",                                       "RUNIO",                                       (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  481 */  { fnGetSystemFlag,              FLAG_PRINT,                  "PRINT",                                       "PRINT",                                       (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  482 */  { fnGetSystemFlag,              FLAG_TRACE,                  "TRACE",                                       "TRACE",                                       (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  483 */  { fnGetSystemFlag,              FLAG_USER,                   "USER",                                        "USER",                                        (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  484 */  { fnGetSystemFlag,              FLAG_LOWBAT,                 "LOWBAT",                                      "LOWBAT",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  485 */  { fnGetSystemFlag,              FLAG_SLOW,                   "SLOW",                                        "SLOW",                                        (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  486 */  { fnGetSystemFlag,              FLAG_SPCRES,                 "SPCRES",                                      "SPCRES",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  487 */  { fnGetSystemFlag,              FLAG_SSIZE8,                 "SSIZE8",                                      "SSIZE8",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  488 */  { fnGetSystemFlag,              FLAG_QUIET,                  "QUIET",                                       "QUIET",                                       (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  489 */  { fnGetSystemFlag,              FLAG_DECIMP,                 "DECIM.",                                      "DECIM.",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  490 */  { fnGetSystemFlag,              FLAG_MULTx,                  "MULT" STD_CROSS,                              "MULT" STD_CROSS,                              (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  491 */  { fnGetSystemFlag,              FLAG_ALLENG,                 "ALLENG",                                      "ALLENG",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  492 */  { fnGetSystemFlag,              FLAG_GROW,                   "GROW",                                        "GROW",                                        (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  493 */  { fnGetSystemFlag,              FLAG_AUTOFF,                 "AUTOFF",                                      "AUTOFF",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  494 */  { fnGetSystemFlag,              FLAG_AUTXEQ,                 "AUTXEQ",                                      "AUTXEQ",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  495 */  { fnGetSystemFlag,              FLAG_PRTACT,                 "PRTACT",                                      "PRTACT",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  496 */  { fnGetSystemFlag,              FLAG_NUMIN,                  "NUM.IN",                                      "NUM.IN",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  497 */  { fnGetSystemFlag,              FLAG_ALPIN,                  "ALP.IN",                                      "ALP.IN",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  498 */  { fnGetSystemFlag,              FLAG_ASLIFT,                 "ASLIFT",                                      "ASLIFT",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  499 */  { fnGetSystemFlag,              FLAG_IGN1ER,                 "IGN1ER",                                      "IGN1ER",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  500 */  { fnGetSystemFlag,              FLAG_INTING,                 "INTING",                                      "INTING",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  501 */  { fnGetSystemFlag,              FLAG_SOLVING,                "SOLVING",                                     "SOLVING",                                     (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  502 */  { fnGetSystemFlag,              FLAG_VMDISP,                 "VMDISP",                                      "VMDISP",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  503 */  { fnGetSystemFlag,              FLAG_USB,                    "USB",                                         "USB",                                         (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  504 */  { fnGetSystemFlag,              FLAG_ENDPMT,                 "ENDPMT",                                      "ENDPMT",                                      (0 << TAM_MAX_BITS) |     0, CAT_SYFL | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  505 */  { itemToBeCoded,                NOPARAM,                     "0505",                                        "0505",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  506 */  { itemToBeCoded,                NOPARAM,                     "0506",                                        "0506",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  507 */  { itemToBeCoded,                NOPARAM,                     "0507",                                        "0507",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  508 */  { itemToBeCoded,                NOPARAM,                     "0508",                                        "0508",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  509 */  { itemToBeCoded,                NOPARAM,                     "0509",                                        "0509",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  510 */  { itemToBeCoded,                NOPARAM,                     "0510",                                        "0510",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  511 */  { itemToBeCoded,                NOPARAM,                     "0511",                                        "0511",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  512 */  { itemToBeCoded,                NOPARAM,                     "0512",                                        "0512",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  513 */  { itemToBeCoded,                NOPARAM,                     "0513",                                        "0513",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  514 */  { itemToBeCoded,                NOPARAM,                     "0514",                                        "0514",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  515 */  { itemToBeCoded,                NOPARAM,                     "0515",                                        "0515",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  516 */  { itemToBeCoded,                NOPARAM,                     "0516",                                        "0516",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  517 */  { itemToBeCoded,                NOPARAM,                     "0517",                                        "0517",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  518 */  { itemToBeCoded,                NOPARAM,                     "0518",                                        "0518",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  519 */  { itemToBeCoded,                NOPARAM,                     "0519",                                        "0519",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  520 */  { itemToBeCoded,                NOPARAM,                     "0520",                                        "0520",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  521 */  { itemToBeCoded,                NOPARAM,                     "0521",                                        "0521",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  522 */  { itemToBeCoded,                NOPARAM,                     "0522",                                        "0522",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  523 */  { itemToBeCoded,                NOPARAM,                     "0523",                                        "0523",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  524 */  { itemToBeCoded,                NOPARAM,                     "0524",                                        "0524",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  525 */  { itemToBeCoded,                NOPARAM,                     "0525",                                        "0525",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/*  526 */  { itemToBeCoded,                NOPARAM,                     "0526",                                        "0526",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},


// Bufferized items
/*  527 */  { addItemToBuffer,              REGISTER_X,                  "X",                                           "X",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // The
/*  528 */  { addItemToBuffer,              REGISTER_Y,                  "Y",                                           "Y",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // order
/*  529 */  { addItemToBuffer,              REGISTER_Z,                  "Z",                                           "Z",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // of these
/*  530 */  { addItemToBuffer,              REGISTER_T,                  "T",                                           "T",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // 12 lines
/*  531 */  { addItemToBuffer,              REGISTER_A,                  "A",                                           "A",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // Must be
/*  532 */  { addItemToBuffer,              REGISTER_B,                  "B",                                           "B",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // kept as
/*  533 */  { addItemToBuffer,              REGISTER_C,                  "C",                                           "C",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // is.
/*  534 */  { addItemToBuffer,              REGISTER_D,                  "D",                                           "D",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // Do not
/*  535 */  { addItemToBuffer,              REGISTER_L,                  "L",                                           "L",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // put them
/*  536 */  { addItemToBuffer,              REGISTER_I,                  "I",                                           "I",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // in a
/*  537 */  { addItemToBuffer,              REGISTER_J,                  "J",                                           "J",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // different
/*  538 */  { addItemToBuffer,              REGISTER_K,                  "K",                                           "K",                                           (0 << TAM_MAX_BITS) |     0, CAT_REGS | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // order!
/*  539 */  { addItemToBuffer,              ITM_INDIRECTION,             STD_RIGHT_ARROW,                               STD_RIGHT_ARROW,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  540 */  { itemToBeCoded,                NOPARAM,                     "0540",                                        "0540",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  541 */  { itemToBeCoded,                NOPARAM,                     "0541",                                        "0541",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  542 */  { itemToBeCoded,                NOPARAM,                     "0542",                                        "0542",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  543 */  { itemToBeCoded,                NOPARAM,                     "0543",                                        "0543",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  544 */  { itemToBeCoded,                NOPARAM,                     "0544",                                        "0544",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  545 */  { itemToBeCoded,                NOPARAM,                     "0545",                                        "0545",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  546 */  { itemToBeCoded,                NOPARAM,                     "0546",                                        "0546",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  547 */  { itemToBeCoded,                NOPARAM,                     "0547",                                        "0547",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  548 */  { addItemToBuffer,              ITM_0,                       "",                                            "0",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  549 */  { addItemToBuffer,              ITM_1,                       "",                                            "1",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  550 */  { addItemToBuffer,              ITM_2,                       "",                                            "2",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  551 */  { addItemToBuffer,              ITM_3,                       "",                                            "3",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  552 */  { addItemToBuffer,              ITM_4,                       "",                                            "4",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  553 */  { addItemToBuffer,              ITM_5,                       "",                                            "5",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  554 */  { addItemToBuffer,              ITM_6,                       "",                                            "6",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  555 */  { addItemToBuffer,              ITM_7,                       "",                                            "7",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  556 */  { addItemToBuffer,              ITM_8,                       "",                                            "8",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  557 */  { addItemToBuffer,              ITM_9,                       "",                                            "9",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  558 */  { addItemToBuffer,              ITM_A,                       "A",                                           "A",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  559 */  { addItemToBuffer,              ITM_B,                       "B",                                           "B",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  560 */  { addItemToBuffer,              ITM_C,                       "C",                                           "C",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  561 */  { addItemToBuffer,              ITM_D,                       "D",                                           "D",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  562 */  { addItemToBuffer,              ITM_E,                       "E",                                           "E",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  563 */  { addItemToBuffer,              ITM_F,                       "F",                                           "F",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  564 */  { addItemToBuffer,              ITM_G,                       "G",                                           "G",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  565 */  { addItemToBuffer,              ITM_H,                       "H",                                           "H",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  566 */  { addItemToBuffer,              ITM_I,                       "I",                                           "I",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  567 */  { addItemToBuffer,              ITM_J,                       "J",                                           "J",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  568 */  { addItemToBuffer,              ITM_K,                       "K",                                           "K",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  569 */  { addItemToBuffer,              ITM_L,                       "L",                                           "L",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  570 */  { addItemToBuffer,              ITM_M,                       "M",                                           "M",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  571 */  { addItemToBuffer,              ITM_N,                       "N",                                           "N",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  572 */  { addItemToBuffer,              ITM_O,                       "O",                                           "O",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  573 */  { addItemToBuffer,              ITM_P,                       "P",                                           "P",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  574 */  { addItemToBuffer,              ITM_Q,                       "Q",                                           "Q",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  575 */  { addItemToBuffer,              ITM_R,                       "R",                                           "R",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  576 */  { addItemToBuffer,              ITM_S,                       "S",                                           "S",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  577 */  { addItemToBuffer,              ITM_T,                       "T",                                           "T",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  578 */  { addItemToBuffer,              ITM_U,                       "U",                                           "U",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  579 */  { addItemToBuffer,              ITM_V,                       "V",                                           "V",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  580 */  { addItemToBuffer,              ITM_W,                       "W",                                           "W",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  581 */  { addItemToBuffer,              ITM_X,                       "X",                                           "X",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  582 */  { addItemToBuffer,              ITM_Y,                       "Y",                                           "Y",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  583 */  { addItemToBuffer,              ITM_Z,                       "Z",                                           "Z",                                           (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  584 */  { addItemToBuffer,              ITM_a,                       "a",                                           "a",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  585 */  { addItemToBuffer,              ITM_b,                       "b",                                           "b",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  586 */  { addItemToBuffer,              ITM_c,                       "c",                                           "c",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  587 */  { addItemToBuffer,              ITM_d,                       "d",                                           "d",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  588 */  { addItemToBuffer,              ITM_e,                       "e",                                           "e",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  589 */  { addItemToBuffer,              ITM_f,                       "f",                                           "f",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  590 */  { addItemToBuffer,              ITM_g,                       "g",                                           "g",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  591 */  { addItemToBuffer,              ITM_h,                       "h",                                           "h",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  592 */  { addItemToBuffer,              ITM_i,                       "i",                                           "i",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  593 */  { addItemToBuffer,              ITM_j,                       "j",                                           "j",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  594 */  { addItemToBuffer,              ITM_k,                       "k",                                           "k",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  595 */  { addItemToBuffer,              ITM_l,                       "l",                                           "l",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  596 */  { addItemToBuffer,              ITM_m,                       "m",                                           "m",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  597 */  { addItemToBuffer,              ITM_n,                       "n",                                           "n",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  598 */  { addItemToBuffer,              ITM_o,                       "o",                                           "o",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  599 */  { addItemToBuffer,              ITM_p,                       "p",                                           "p",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  600 */  { addItemToBuffer,              ITM_q,                       "q",                                           "q",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  601 */  { addItemToBuffer,              ITM_r,                       "r",                                           "r",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  602 */  { addItemToBuffer,              ITM_s,                       "s",                                           "s",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  603 */  { addItemToBuffer,              ITM_t,                       "t",                                           "t",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  604 */  { addItemToBuffer,              ITM_u,                       "u",                                           "u",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  605 */  { addItemToBuffer,              ITM_v,                       "v",                                           "v",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  606 */  { addItemToBuffer,              ITM_w,                       "w",                                           "w",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  607 */  { addItemToBuffer,              ITM_x,                       "x",                                           "x",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  608 */  { addItemToBuffer,              ITM_y,                       "y",                                           "y",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  609 */  { addItemToBuffer,              ITM_z,                       "z",                                           "z",                                           (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  610 */  { addItemToBuffer,              ITM_ALPHA,                   "",                                            STD_ALPHA,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  611 */  { itemToBeCoded,                NOPARAM,                     "0611",                                        "0611",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  612 */  { addItemToBuffer,              ITM_BETA,                    "",                                            STD_BETA,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  613 */  { addItemToBuffer,              ITM_GAMMA,                   "",                                            STD_GAMMA,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  614 */  { addItemToBuffer,              ITM_DELTA,                   "",                                            STD_DELTA,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  615 */  { addItemToBuffer,              ITM_EPSILON,                 "",                                            STD_EPSILON,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  616 */  { itemToBeCoded,                NOPARAM,                     "0616",                                        "0616",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  617 */  { addItemToBuffer,              ITM_ZETA,                    "",                                            STD_ZETA,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  618 */  { addItemToBuffer,              ITM_ETA,                     "",                                            STD_ETA,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  619 */  { itemToBeCoded,                NOPARAM,                     "0619",                                        "0619",                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  620 */  { addItemToBuffer,              ITM_THETA,                   "",                                            STD_THETA,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  621 */  { addItemToBuffer,              ITM_IOTA,                    "",                                            STD_IOTA,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  622 */  { itemToBeCoded,                NOPARAM,                     "0622",                                        "0622",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  623 */  { itemToBeCoded,                NOPARAM,                     "0623",                                        "0623",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  624 */  { addItemToBuffer,              ITM_IOTA_DIALYTIKA,          "",                                            STD_IOTA_DIALYTIKA,                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  625 */  { addItemToBuffer,              ITM_KAPPA,                   "",                                            STD_KAPPA,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  626 */  { addItemToBuffer,              ITM_LAMBDA,                  "",                                            STD_LAMBDA,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  627 */  { addItemToBuffer,              ITM_MU,                      "",                                            STD_MU,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  628 */  { addItemToBuffer,              ITM_NU,                      "",                                            STD_NU,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  629 */  { addItemToBuffer,              ITM_XI,                      "",                                            STD_XI,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  630 */  { addItemToBuffer,              ITM_OMICRON,                 "",                                            STD_OMICRON,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  631 */  { itemToBeCoded,                NOPARAM,                     "0631",                                        "0631",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  632 */  { addItemToBuffer,              ITM_PI,                      "",                                            STD_PI,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  633 */  { addItemToBuffer,              ITM_RHO,                     "",                                            STD_RHO,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  634 */  { addItemToBuffer,              ITM_SIGMA,                   "",                                            STD_SIGMA,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  635 */  { itemToBeCoded,                NOPARAM,                     "0635",                                        "0635",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  636 */  { addItemToBuffer,              ITM_TAU,                     "",                                            STD_TAU,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  637 */  { addItemToBuffer,              ITM_UPSILON,                 "",                                            STD_UPSILON,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  638 */  { itemToBeCoded,                NOPARAM,                     "0638",                                        "0638",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  639 */  { addItemToBuffer,              ITM_UPSILON_DIALYTIKA,       "",                                            STD_UPSILON_DIALYTIKA,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  640 */  { itemToBeCoded,                NOPARAM,                     "0640",                                        "0640",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  641 */  { addItemToBuffer,              ITM_PHI,                     "",                                            STD_PHI,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  642 */  { addItemToBuffer,              ITM_CHI,                     "",                                            STD_CHI,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  643 */  { addItemToBuffer,              ITM_PSI,                     "",                                            STD_PSI,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  644 */  { addItemToBuffer,              ITM_OMEGA,                   "",                                            STD_OMEGA,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  645 */  { itemToBeCoded,                NOPARAM,                     "0645",                                        "0645",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  646 */  { addItemToBuffer,              ITM_alpha,                   "",                                            STD_alpha,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  647 */  { addItemToBuffer,              ITM_alpha_TONOS,             "",                                            STD_alpha_TONOS,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  648 */  { addItemToBuffer,              ITM_beta,                    "",                                            STD_beta,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  649 */  { addItemToBuffer,              ITM_gamma,                   "",                                            STD_gamma,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  650 */  { addItemToBuffer,              ITM_delta,                   "",                                            STD_delta,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  651 */  { addItemToBuffer,              ITM_epsilon,                 "",                                            STD_epsilon,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  652 */  { addItemToBuffer,              ITM_epsilon_TONOS,           "",                                            STD_epsilon_TONOS,                             (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  653 */  { addItemToBuffer,              ITM_zeta,                    "",                                            STD_zeta,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  654 */  { addItemToBuffer,              ITM_eta,                     "",                                            STD_eta,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  655 */  { addItemToBuffer,              ITM_eta_TONOS,               "",                                            STD_eta_TONOS,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  656 */  { addItemToBuffer,              ITM_theta,                   "",                                            STD_theta,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  657 */  { addItemToBuffer,              ITM_iota,                    "",                                            STD_iota,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  658 */  { addItemToBuffer,              ITM_iotaTON,                 "",                                            STD_iota_TONOS,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  659 */  { addItemToBuffer,              ITM_iota_DIALYTIKA_TONOS,    "",                                            STD_iota_DIALYTIKA_TONOS,                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  660 */  { addItemToBuffer,              ITM_iota_DIALYTIKA,          "",                                            STD_iota_DIALYTIKA,                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  661 */  { addItemToBuffer,              ITM_kappa,                   "",                                            STD_kappa,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  662 */  { addItemToBuffer,              ITM_lambda,                  "",                                            STD_lambda,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  663 */  { addItemToBuffer,              ITM_mu,                      "",                                            STD_mu,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  664 */  { addItemToBuffer,              ITM_nu,                      "",                                            STD_nu,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  665 */  { addItemToBuffer,              ITM_xi,                      "",                                            STD_xi,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  666 */  { addItemToBuffer,              ITM_omicron,                 "",                                            STD_omicron,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  667 */  { addItemToBuffer,              ITM_omicron_TONOS,           "",                                            STD_omicron_TONOS,                             (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  668 */  { addItemToBuffer,              ITM_pi,                      "",                                            STD_pi,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  669 */  { addItemToBuffer,              ITM_rho,                     "",                                            STD_rho,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  670 */  { addItemToBuffer,              ITM_sigma,                   "",                                            STD_sigma,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  671 */  { addItemToBuffer,              ITM_sigma_end,               "",                                            STD_sigma_end,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  672 */  { addItemToBuffer,              ITM_tau,                     "",                                            STD_tau,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  673 */  { addItemToBuffer,              ITM_upsilon,                 "",                                            STD_upsilon,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  674 */  { addItemToBuffer,              ITM_upsilon_TONOS,           "",                                            STD_upsilon_TONOS,                             (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  675 */  { addItemToBuffer,              ITM_upsilon_DIALYTIKA,       "",                                            STD_upsilon_DIALYTIKA,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  676 */  { addItemToBuffer,              ITM_upsilon_DIALYTIKA_TONOS, "",                                            STD_upsilon_DIALYTIKA_TONOS,                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  677 */  { addItemToBuffer,              ITM_phi,                     "",                                            STD_phi,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  678 */  { addItemToBuffer,              ITM_chi,                     "",                                            STD_chi,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  679 */  { addItemToBuffer,              ITM_psi,                     "",                                            STD_psi,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  680 */  { addItemToBuffer,              ITM_omega,                   "",                                            STD_omega,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  681 */  { addItemToBuffer,              ITM_omega_TONOS,             "",                                            STD_omega_TONOS,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  682 */  { itemToBeCoded,                NOPARAM,                     "0682",                                        "0682",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  683 */  { itemToBeCoded,                NOPARAM,                     "0683",                                        "0683",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  684 */  { itemToBeCoded,                NOPARAM,                     "0684",                                        "0684",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  685 */  { itemToBeCoded,                NOPARAM,                     "0685",                                        "0685",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  686 */  { itemToBeCoded,                NOPARAM,                     "0686",                                        "0686",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  687 */  { itemToBeCoded,                NOPARAM,                     "0687",                                        "0687",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  688 */  { addItemToBuffer,              ITM_A_MACRON,                STD_A_MACRON,                                  STD_A_MACRON,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  689 */  { addItemToBuffer,              ITM_A_ACUTE,                 STD_A_ACUTE,                                   STD_A_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  690 */  { addItemToBuffer,              ITM_A_BREVE,                 STD_A_BREVE,                                   STD_A_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  691 */  { addItemToBuffer,              ITM_A_GRAVE,                 STD_A_GRAVE,                                   STD_A_GRAVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  692 */  { addItemToBuffer,              ITM_A_DIARESIS,              STD_A_DIARESIS,                                STD_A_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  693 */  { addItemToBuffer,              ITM_A_TILDE,                 STD_A_TILDE,                                   STD_A_TILDE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  694 */  { addItemToBuffer,              ITM_A_CIRC,                  STD_A_CIRC,                                    STD_A_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  695 */  { addItemToBuffer,              ITM_A_RING,                  STD_A_RING,                                    STD_A_RING,                                    (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  696 */  { addItemToBuffer,              ITM_AE,                      STD_AE,                                        STD_AE,                                        (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  697 */  { addItemToBuffer,              ITM_A_OGONEK,                STD_A_OGONEK,                                  STD_A_OGONEK,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  698 */  { addItemToBuffer,              ITM_C_ACUTE,                 STD_C_ACUTE,                                   STD_C_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  699 */  { addItemToBuffer,              ITM_C_CARON,                 STD_C_CARON,                                   STD_C_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  700 */  { addItemToBuffer,              ITM_C_CEDILLA,               STD_C_CEDILLA,                                 STD_C_CEDILLA,                                 (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  701 */  { addItemToBuffer,              ITM_D_STROKE,                STD_D_STROKE,                                  STD_D_STROKE,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  702 */  { addItemToBuffer,              ITM_D_CARON,                 STD_D_CARON,                                   STD_D_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  703 */  { addItemToBuffer,              ITM_E_MACRON,                STD_E_MACRON,                                  STD_E_MACRON,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  704 */  { addItemToBuffer,              ITM_E_ACUTE,                 STD_E_ACUTE,                                   STD_E_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  705 */  { addItemToBuffer,              ITM_E_BREVE,                 STD_E_BREVE,                                   STD_E_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  706 */  { addItemToBuffer,              ITM_E_GRAVE,                 STD_E_GRAVE,                                   STD_E_GRAVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  707 */  { addItemToBuffer,              ITM_E_DIARESIS,              STD_E_DIARESIS,                                STD_E_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  708 */  { addItemToBuffer,              ITM_E_CIRC,                  STD_E_CIRC,                                    STD_E_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  709 */  { addItemToBuffer,              ITM_E_OGONEK,                STD_E_OGONEK,                                  STD_E_OGONEK,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  710 */  { addItemToBuffer,              ITM_G_BREVE,                 STD_G_BREVE,                                   STD_G_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  711 */  { itemToBeCoded,                NOPARAM,                     "0711",                                        "0711",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  712 */  { addItemToBuffer,              ITM_I_MACRON,                STD_I_MACRON,                                  STD_I_MACRON,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  713 */  { addItemToBuffer,              ITM_I_ACUTE,                 STD_I_ACUTE,                                   STD_I_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  714 */  { addItemToBuffer,              ITM_I_BREVE,                 STD_I_BREVE,                                   STD_I_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  715 */  { addItemToBuffer,              ITM_I_GRAVE,                 STD_I_GRAVE,                                   STD_I_GRAVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  716 */  { addItemToBuffer,              ITM_I_DIARESIS,              STD_I_DIARESIS,                                STD_I_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  717 */  { addItemToBuffer,              ITM_I_CIRC,                  STD_I_CIRC,                                    STD_I_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  718 */  { addItemToBuffer,              ITM_I_OGONEK,                STD_I_OGONEK,                                  STD_I_OGONEK,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  719 */  { addItemToBuffer,              ITM_I_DOT,                   STD_I_DOT,                                     STD_I_DOT,                                     (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  720 */  { addItemToBuffer,              ITM_I_DOTLESS,               "I",                                           "I",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  721 */  { addItemToBuffer,              ITM_L_STROKE,                STD_L_STROKE,                                  STD_L_STROKE,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  722 */  { addItemToBuffer,              ITM_L_ACUTE,                 STD_L_ACUTE,                                   STD_L_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  723 */  { addItemToBuffer,              ITM_L_APOSTROPHE,            STD_L_APOSTROPHE,                              STD_L_APOSTROPHE,                              (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  724 */  { addItemToBuffer,              ITM_N_ACUTE,                 STD_N_ACUTE,                                   STD_N_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  725 */  { addItemToBuffer,              ITM_N_CARON,                 STD_N_CARON,                                   STD_N_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  726 */  { addItemToBuffer,              ITM_N_TILDE,                 STD_N_TILDE,                                   STD_N_TILDE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  727 */  { addItemToBuffer,              ITM_O_MACRON,                STD_O_MACRON,                                  STD_O_MACRON,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  728 */  { addItemToBuffer,              ITM_O_ACUTE,                 STD_O_ACUTE,                                   STD_O_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  729 */  { addItemToBuffer,              ITM_O_BREVE,                 STD_O_BREVE,                                   STD_O_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  730 */  { addItemToBuffer,              ITM_O_GRAVE,                 STD_O_GRAVE,                                   STD_O_GRAVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  731 */  { addItemToBuffer,              ITM_O_DIARESIS,              STD_O_DIARESIS,                                STD_O_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  732 */  { addItemToBuffer,              ITM_O_TILDE,                 STD_O_TILDE,                                   STD_O_TILDE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  733 */  { addItemToBuffer,              ITM_O_CIRC,                  STD_O_CIRC,                                    STD_O_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  734 */  { addItemToBuffer,              ITM_O_STROKE,                STD_O_STROKE,                                  STD_O_STROKE,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  735 */  { addItemToBuffer,              ITM_OE,                      STD_OE,                                        STD_OE,                                        (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  736 */  { itemToBeCoded,                NOPARAM,                     "0736",                                        "0736",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  737 */  { itemToBeCoded,                NOPARAM,                     "0737",                                        "0737",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  738 */  { addItemToBuffer,              ITM_S_SHARP,                 STD_s_SHARP,                                   STD_s_SHARP,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  739 */  { addItemToBuffer,              ITM_S_ACUTE,                 STD_S_ACUTE,                                   STD_S_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  740 */  { addItemToBuffer,              ITM_S_CARON,                 STD_S_CARON,                                   STD_S_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  741 */  { addItemToBuffer,              ITM_S_CEDILLA,               STD_S_CEDILLA,                                 STD_S_CEDILLA,                                 (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  742 */  { addItemToBuffer,              ITM_T_CARON,                 STD_T_CARON,                                   STD_T_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  743 */  { addItemToBuffer,              ITM_T_CEDILLA,               STD_T_CEDILLA,                                 STD_T_CEDILLA,                                 (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  744 */  { addItemToBuffer,              ITM_U_MACRON,                STD_U_MACRON,                                  STD_U_MACRON,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  745 */  { addItemToBuffer,              ITM_U_ACUTE,                 STD_U_ACUTE,                                   STD_U_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  746 */  { addItemToBuffer,              ITM_U_BREVE,                 STD_U_BREVE,                                   STD_U_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  747 */  { addItemToBuffer,              ITM_U_GRAVE,                 STD_U_GRAVE,                                   STD_U_GRAVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  748 */  { addItemToBuffer,              ITM_U_DIARESIS,              STD_U_DIARESIS,                                STD_U_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  749 */  { addItemToBuffer,              ITM_U_TILDE,                 STD_U_TILDE,                                   STD_U_TILDE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  750 */  { addItemToBuffer,              ITM_U_CIRC,                  STD_U_CIRC,                                    STD_U_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  751 */  { addItemToBuffer,              ITM_U_RING,                  STD_U_RING,                                    STD_U_RING,                                    (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  752 */  { addItemToBuffer,              ITM_W_CIRC,                  STD_W_CIRC,                                    STD_W_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  753 */  { itemToBeCoded,                NOPARAM,                     "0753",                                        "0753",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  754 */  { itemToBeCoded,                NOPARAM,                     "0754",                                        "0754",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  755 */  { itemToBeCoded,                NOPARAM,                     "0755",                                        "0755",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  756 */  { addItemToBuffer,              ITM_Y_CIRC,                  STD_Y_CIRC,                                    STD_Y_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  757 */  { addItemToBuffer,              ITM_Y_ACUTE,                 STD_Y_ACUTE,                                   STD_Y_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  758 */  { addItemToBuffer,              ITM_Y_DIARESIS,              STD_Y_DIARESIS,                                STD_Y_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  759 */  { addItemToBuffer,              ITM_Z_ACUTE,                 STD_Z_ACUTE,                                   STD_Z_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  760 */  { addItemToBuffer,              ITM_Z_CARON,                 STD_Z_CARON,                                   STD_Z_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  761 */  { addItemToBuffer,              ITM_Z_DOT,                   STD_Z_DOT,                                     STD_Z_DOT,                                     (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  762 */  { itemToBeCoded,                NOPARAM,                     "0762",                                        "0762",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  763 */  { itemToBeCoded,                NOPARAM,                     "0763",                                        "0763",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  764 */  { itemToBeCoded,                NOPARAM,                     "0764",                                        "0764",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  765 */  { itemToBeCoded,                NOPARAM,                     "0765",                                        "0765",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  766 */  { itemToBeCoded,                NOPARAM,                     "0766",                                        "0766",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  767 */  { itemToBeCoded,                NOPARAM,                     "0767",                                        "0767",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  768 */  { addItemToBuffer,              ITM_a_MACRON,                STD_a_MACRON,                                  STD_a_MACRON,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  769 */  { addItemToBuffer,              ITM_a_ACUTE,                 STD_a_ACUTE,                                   STD_a_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  770 */  { addItemToBuffer,              ITM_a_BREVE,                 STD_a_BREVE,                                   STD_a_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  771 */  { addItemToBuffer,              ITM_a_GRAVE,                 STD_a_GRAVE,                                   STD_a_GRAVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  772 */  { addItemToBuffer,              ITM_a_DIARESIS,              STD_a_DIARESIS,                                STD_a_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  773 */  { addItemToBuffer,              ITM_a_TILDE,                 STD_a_TILDE,                                   STD_a_TILDE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  774 */  { addItemToBuffer,              ITM_a_CIRC,                  STD_a_CIRC,                                    STD_a_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  775 */  { addItemToBuffer,              ITM_a_RING,                  STD_a_RING,                                    STD_a_RING,                                    (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  776 */  { addItemToBuffer,              ITM_ae,                      STD_ae,                                        STD_ae,                                        (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  777 */  { addItemToBuffer,              ITM_a_OGONEK,                STD_a_OGONEK,                                  STD_a_OGONEK,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  778 */  { addItemToBuffer,              ITM_c_ACUTE,                 STD_c_ACUTE,                                   STD_c_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  779 */  { addItemToBuffer,              ITM_c_CARON,                 STD_c_CARON,                                   STD_c_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  780 */  { addItemToBuffer,              ITM_c_CEDILLA,               STD_c_CEDILLA,                                 STD_c_CEDILLA,                                 (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  781 */  { addItemToBuffer,              ITM_d_STROKE,                STD_d_STROKE,                                  STD_d_STROKE,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  782 */  { addItemToBuffer,              ITM_d_APOSTROPHE,            STD_d_APOSTROPHE,                              STD_d_APOSTROPHE,                              (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  783 */  { addItemToBuffer,              ITM_e_MACRON,                STD_e_MACRON,                                  STD_e_MACRON,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  784 */  { addItemToBuffer,              ITM_e_ACUTE,                 STD_e_ACUTE,                                   STD_e_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  785 */  { addItemToBuffer,              ITM_e_BREVE,                 STD_e_BREVE,                                   STD_e_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  786 */  { addItemToBuffer,              ITM_e_GRAVE,                 STD_e_GRAVE,                                   STD_e_GRAVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  787 */  { addItemToBuffer,              ITM_e_DIARESIS,              STD_e_DIARESIS,                                STD_e_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  788 */  { addItemToBuffer,              ITM_e_CIRC,                  STD_e_CIRC,                                    STD_e_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  789 */  { addItemToBuffer,              ITM_e_OGONEK,                STD_e_OGONEK,                                  STD_e_OGONEK,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  790 */  { addItemToBuffer,              ITM_g_BREVE,                 STD_g_BREVE,                                   STD_g_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  791 */  { addItemToBuffer,              ITM_h_STROKE,                "",                                            STD_h_STROKE,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  792 */  { addItemToBuffer,              ITM_i_MACRON,                STD_i_MACRON,                                  STD_i_MACRON,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  793 */  { addItemToBuffer,              ITM_i_ACUTE,                 STD_i_ACUTE,                                   STD_i_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  794 */  { addItemToBuffer,              ITM_i_BREVE,                 STD_i_BREVE,                                   STD_i_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  795 */  { addItemToBuffer,              ITM_i_GRAVE,                 STD_i_GRAVE,                                   STD_i_GRAVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  796 */  { addItemToBuffer,              ITM_i_DIARESIS,              STD_i_DIARESIS,                                STD_i_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  797 */  { addItemToBuffer,              ITM_i_CIRC,                  STD_i_CIRC,                                    STD_i_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  798 */  { addItemToBuffer,              ITM_i_OGONEK,                STD_i_OGONEK,                                  STD_i_OGONEK,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  799 */  { addItemToBuffer,              ITM_i_DOT,                   "i",                                           "i",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  800 */  { addItemToBuffer,              ITM_i_DOTLESS,               STD_i_DOTLESS,                                 STD_i_DOTLESS,                                 (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  801 */  { addItemToBuffer,              ITM_l_STROKE,                STD_l_STROKE,                                  STD_l_STROKE,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  802 */  { addItemToBuffer,              ITM_l_ACUTE,                 STD_l_ACUTE,                                   STD_l_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  803 */  { addItemToBuffer,              ITM_l_APOSTROPHE,            STD_l_APOSTROPHE,                              STD_l_APOSTROPHE,                              (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  804 */  { addItemToBuffer,              ITM_n_ACUTE,                 STD_n_ACUTE,                                   STD_n_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  805 */  { addItemToBuffer,              ITM_n_CARON,                 STD_n_CARON,                                   STD_n_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  806 */  { addItemToBuffer,              ITM_n_TILDE,                 STD_n_TILDE,                                   STD_n_TILDE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  807 */  { addItemToBuffer,              ITM_o_MACRON,                STD_o_MACRON,                                  STD_o_MACRON,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  808 */  { addItemToBuffer,              ITM_o_ACUTE,                 STD_o_ACUTE,                                   STD_o_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  809 */  { addItemToBuffer,              ITM_o_BREVE,                 STD_o_BREVE,                                   STD_o_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  810 */  { addItemToBuffer,              ITM_o_GRAVE,                 STD_o_GRAVE,                                   STD_o_GRAVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  811 */  { addItemToBuffer,              ITM_o_DIARESIS,              STD_o_DIARESIS,                                STD_o_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  812 */  { addItemToBuffer,              ITM_o_TILDE,                 STD_o_TILDE,                                   STD_o_TILDE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  813 */  { addItemToBuffer,              ITM_o_CIRC,                  STD_o_CIRC,                                    STD_o_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  814 */  { addItemToBuffer,              ITM_o_STROKE,                STD_o_STROKE,                                  STD_o_STROKE,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  815 */  { addItemToBuffer,              ITM_oe,                      STD_oe,                                        STD_oe,                                        (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  816 */  { addItemToBuffer,              ITM_r_CARON,                 STD_r_CARON,                                   STD_r_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  817 */  { addItemToBuffer,              ITM_r_ACUTE,                 STD_r_ACUTE,                                   STD_r_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  818 */  { addItemToBuffer,              ITM_s_SHARP,                 STD_s_SHARP,                                   STD_s_SHARP,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  819 */  { addItemToBuffer,              ITM_s_ACUTE,                 STD_s_ACUTE,                                   STD_s_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  820 */  { addItemToBuffer,              ITM_s_CARON,                 STD_s_CARON,                                   STD_s_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  821 */  { addItemToBuffer,              ITM_s_CEDILLA,               STD_s_CEDILLA,                                 STD_s_CEDILLA,                                 (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  822 */  { addItemToBuffer,              ITM_t_APOSTROPHE,            STD_t_APOSTROPHE,                              STD_t_APOSTROPHE,                              (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  823 */  { addItemToBuffer,              ITM_t_CEDILLA,               STD_t_CEDILLA,                                 STD_t_CEDILLA,                                 (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  824 */  { addItemToBuffer,              ITM_u_MACRON,                STD_u_MACRON,                                  STD_u_MACRON,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  825 */  { addItemToBuffer,              ITM_u_ACUTE,                 STD_u_ACUTE,                                   STD_u_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  826 */  { addItemToBuffer,              ITM_u_BREVE,                 STD_u_BREVE,                                   STD_u_BREVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  827 */  { addItemToBuffer,              ITM_u_GRAVE,                 STD_u_GRAVE,                                   STD_u_GRAVE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  828 */  { addItemToBuffer,              ITM_u_DIARESIS,              STD_u_DIARESIS,                                STD_u_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  829 */  { addItemToBuffer,              ITM_u_TILDE,                 STD_u_TILDE,                                   STD_u_TILDE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  830 */  { addItemToBuffer,              ITM_u_CIRC,                  STD_u_CIRC,                                    STD_u_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  831 */  { addItemToBuffer,              ITM_u_RING,                  STD_u_RING,                                    STD_u_RING,                                    (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  832 */  { addItemToBuffer,              ITM_w_CIRC,                  STD_w_CIRC,                                    STD_w_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  833 */  { addItemToBuffer,              ITM_x_BAR,                   "",                                            STD_x_BAR,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  834 */  { addItemToBuffer,              ITM_x_CIRC,                  "",                                            STD_x_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  835 */  { addItemToBuffer,              ITM_y_BAR,                   "",                                            STD_y_BAR,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  836 */  { addItemToBuffer,              ITM_y_CIRC,                  STD_y_CIRC,                                    STD_y_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  837 */  { addItemToBuffer,              ITM_y_ACUTE,                 STD_y_ACUTE,                                   STD_y_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  838 */  { addItemToBuffer,              ITM_y_DIARESIS,              STD_y_DIARESIS,                                STD_y_DIARESIS,                                (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  839 */  { addItemToBuffer,              ITM_z_ACUTE,                 STD_z_ACUTE,                                   STD_z_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  840 */  { addItemToBuffer,              ITM_z_CARON,                 STD_z_CARON,                                   STD_z_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  841 */  { addItemToBuffer,              ITM_z_DOT,                   STD_z_DOT,                                     STD_z_DOT,                                     (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  842 */  { itemToBeCoded,                NOPARAM,                     "0842",                                        "0842",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  843 */  { itemToBeCoded,                NOPARAM,                     "0843",                                        "0843",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  844 */  { itemToBeCoded,                NOPARAM,                     "0844",                                        "0844",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  845 */  { itemToBeCoded,                NOPARAM,                     "0845",                                        "0845",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  846 */  { itemToBeCoded,                NOPARAM,                     "0846",                                        "0846",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  847 */  { itemToBeCoded,                NOPARAM,                     "0847",                                        "0847",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  848 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_alpha,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  849 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_delta,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  850 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_mu,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  851 */  { addItemToBuffer,              ITM_SUB_SUN,                 "",                                            STD_SUB_SUN,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  852 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_SUN_b,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  853 */  { addItemToBuffer,              ITM_SUB_EARTH,               "",                                            STD_SUB_EARTH,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  854 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_EARTH_b,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  855 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_PLUS,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  856 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_MINUS,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  857 */  { addItemToBuffer,              ITM_SUB_INFINITY,            "",                                            STD_SUB_INFINITY,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  858 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_0,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  859 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_1,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  860 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_2,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  861 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_3,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  862 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_4,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  863 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_5,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  864 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_6,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  865 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_7,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  866 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_8,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  867 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_9,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  868 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_10,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  869 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_A,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  870 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_B,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  871 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_C,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  872 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_D,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  873 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_E,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  874 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_F,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  875 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_G,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  876 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_H,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  877 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_I,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  878 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_J,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  879 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_K,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  880 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_L,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  881 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_M,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  882 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_N,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  883 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_O,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  884 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_P,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  885 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_Q,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  886 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_R,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  887 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_S,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  888 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_T,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  889 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_U,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  890 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_V,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  891 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_W,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  892 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_X,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  893 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_Y,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  894 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_Z,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  895 */  { addItemToBuffer,              ITM_SUB_E_OUTLINE,           "",                                            STD_SUB_E_OUTLINE,                             (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  896 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_a,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  897 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_b,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  898 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_c,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  899 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_d,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  900 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_e,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  901 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_h,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  902 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_i,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  903 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_j,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  904 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_k,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  905 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_l,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  906 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_m,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  907 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_n,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  908 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_o,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  909 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_p,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  910 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_q,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  911 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_s,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  912 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_t,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  913 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_u,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  914 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_v,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  915 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_w,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  916 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_x,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  917 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_y,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  918 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_z,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  919 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_a_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  920 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_e_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  921 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_k_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  922 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_l_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  923 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_m_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  924 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_n_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  925 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_o_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  926 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_p_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  927 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_s_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  928 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_u_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  929 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUB_x_b,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  930 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_PLUS,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  931 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_MINUS,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  932 */  { addItemToBuffer,              ITM_SUP_MINUS_1,             "",                                            STD_SUP_MINUS_1,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  933 */  { addItemToBuffer,              ITM_SUP_INFINITY,            "",                                            STD_SUP_INFINITY,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  934 */  { addItemToBuffer,              ITM_SUP_ASTERISK,            "",                                            STD_SUP_ASTERISK,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  935 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_0,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  936 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_1,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  937 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_2,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  938 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_3,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  939 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_4,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  940 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_5,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  941 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_6,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  942 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_7,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  943 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_8,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  944 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_9,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  945 */  { itemToBeCoded,                NOPARAM,                     "0945",                                        "0945",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  946 */  { addItemToBuffer,              ITM_SUP_T,                   "",                                            STD_SUP_T,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  947 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_a,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  948 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_f,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  949 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_g,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  950 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_h,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  951 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_r,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  952 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SUP_x,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  953 */  { addItemToBuffer,              ITM_SPACE,                   "",                                            STD_SPACE,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  954 */  { addItemToBuffer,              ITM_EXCLAMATION_MARK,        "",                                            STD_EXCLAMATION_MARK,                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  955 */  { addItemToBuffer,              ITM_DOUBLE_QUOTE,            "",                                            STD_DOUBLE_QUOTE,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  956 */  { addItemToBuffer,              ITM_NUMBER_SIGN,             "",                                            STD_NUMBER_SIGN,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  957 */  { addItemToBuffer,              ITM_DOLLAR,                  "",                                            STD_DOLLAR,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  958 */  { addItemToBuffer,              ITM_PERCENT,                 "",                                            STD_PERCENT,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  959 */  { addItemToBuffer,              ITM_AMPERSAND,               "",                                            STD_AMPERSAND,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  960 */  { addItemToBuffer,              ITM_QUOTE,                   "",                                            STD_QUOTE,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  961 */  { addItemToBuffer,              ITM_LEFT_PARENTHESIS,        "",                                            STD_LEFT_PARENTHESIS,                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  962 */  { addItemToBuffer,              ITM_RIGHT_PARENTHESIS,       "",                                            STD_RIGHT_PARENTHESIS,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  963 */  { addItemToBuffer,              ITM_ASTERISK,                "",                                            STD_ASTERISK,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  964 */  { itemToBeCoded,                NOPARAM,                     "0964",                                        "0964",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  965 */  { addItemToBuffer,              ITM_PLUS,                    "",                                            STD_PLUS,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  966 */  { addItemToBuffer,              ITM_COMMA,                   "",                                            STD_COMMA,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  967 */  { addItemToBuffer,              ITM_MINUS,                   "",                                            STD_MINUS,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  968 */  { addItemToBuffer,              ITM_PERIOD,                  "",                                            STD_PERIOD,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  969 */  { addItemToBuffer,              ITM_SLASH,                   "",                                            STD_SLASH,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  970 */  { addItemToBuffer,              ITM_COLON,                   "",                                            STD_COLON,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  971 */  { addItemToBuffer,              ITM_SEMICOLON,               "",                                            STD_SEMICOLON,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  972 */  { addItemToBuffer,              ITM_LESS_THAN,               "",                                            STD_LESS_THAN,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  973 */  { addItemToBuffer,              ITM_EQUAL,                   "",                                            STD_EQUAL,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  974 */  { addItemToBuffer,              ITM_GREATER_THAN,            "",                                            STD_GREATER_THAN,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  975 */  { addItemToBuffer,              ITM_QUESTION_MARK,           "",                                            STD_QUESTION_MARK,                             (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  976 */  { addItemToBuffer,              ITM_AT,                      "",                                            STD_AT,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  977 */  { addItemToBuffer,              ITM_LEFT_SQUARE_BRACKET,     "",                                            STD_LEFT_SQUARE_BRACKET,                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  978 */  { addItemToBuffer,              ITM_BACK_SLASH,              "",                                            STD_BACK_SLASH,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  979 */  { addItemToBuffer,              ITM_RIGHT_SQUARE_BRACKET,    "",                                            STD_RIGHT_SQUARE_BRACKET,                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  980 */  { addItemToBuffer,              ITM_CIRCUMFLEX,              "",                                            STD_CIRCUMFLEX,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  981 */  { addItemToBuffer,              ITM_UNDERSCORE,              "",                                            STD_UNDERSCORE,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  982 */  { addItemToBuffer,              ITM_LEFT_CURLY_BRACKET,      "",                                            STD_LEFT_CURLY_BRACKET,                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  983 */  { addItemToBuffer,              ITM_PIPE,                    "",                                            STD_PIPE,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  984 */  { addItemToBuffer,              ITM_RIGHT_CURLY_BRACKET,     "",                                            STD_RIGHT_CURLY_BRACKET,                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  985 */  { addItemToBuffer,              ITM_TILDE,                   "",                                            STD_TILDE,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  986 */  { addItemToBuffer,              ITM_INVERTED_EXCLAMATION_MARK, "",                                          STD_INVERTED_EXCLAMATION_MARK,                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  987 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_CENT,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  988 */  { addItemToBuffer,              ITM_POUND,                   "",                                            STD_POUND,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  989 */  { addItemToBuffer,              ITM_YEN,                     "",                                            STD_YEN,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  990 */  { addItemToBuffer,              ITM_SECTION,                 "",                                            STD_SECTION,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  991 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_OVERFLOW_CARRY,                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  992 */  { addItemToBuffer,              ITM_LEFT_DOUBLE_ANGLE,       "",                                            STD_LEFT_DOUBLE_ANGLE,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  993 */  { addItemToBuffer,              ITM_NOT,                     "",                                            STD_NOT,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  994 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_DEGREE,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  995 */  { addItemToBuffer,              ITM_PLUS_MINUS,              "",                                            STD_PLUS_MINUS,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  996 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_mu_b,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  997 */  { addItemToBuffer,              ITM_DOT,                     "",                                            STD_DOT,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  998 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_ORDINAL,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/*  999 */  { addItemToBuffer,              ITM_RIGHT_DOUBLE_ANGLE,      "",                                            STD_RIGHT_DOUBLE_ANGLE,                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1000 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_ONE_HALF,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1001 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_ONE_QUARTER,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1002 */  { addItemToBuffer,              ITM_INVERTED_QUESTION_MARK,  "",                                            STD_INVERTED_QUESTION_MARK,                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1003 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_ETH,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1004 */  { addItemToBuffer,              ITM_CROSS,                   "",                                            STD_CROSS,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1005 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_eth,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1006 */  { addItemToBuffer,              ITM_OBELUS,                  "",                                            STD_DIVIDE,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1007 */  { addItemToBuffer,              ITM_E_DOT,                   STD_E_DOT,                                     STD_E_DOT,                                     (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1008 */  { addItemToBuffer,              ITM_e_DOT,                   STD_e_DOT,                                     STD_e_DOT,                                     (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1009 */  { addItemToBuffer,              ITM_E_CARON,                 STD_E_CARON,                                   STD_E_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1010 */  { addItemToBuffer,              ITM_e_CARON,                 STD_e_CARON,                                   STD_e_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1011 */  { addItemToBuffer,              ITM_R_ACUTE,                 STD_R_ACUTE,                                   STD_R_ACUTE,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1012 */  { addItemToBuffer,              ITM_R_CARON,                 STD_R_CARON,                                   STD_R_CARON,                                   (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1013 */  { addItemToBuffer,              ITM_U_OGONEK,                STD_U_OGONEK,                                  STD_U_OGONEK,                                  (0 << TAM_MAX_BITS) |     0, CAT_AINT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1014 */  { addItemToBuffer,              ITM_u_OGONEK,                STD_u_OGONEK,                                  STD_u_OGONEK,                                  (0 << TAM_MAX_BITS) |     0, CAT_aint | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1015 */  { addItemToBuffer,              ITM_y_UNDER_ROOT,            "",                                            STD_y_UNDER_ROOT,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1016 */  { addItemToBuffer,              ITM_x_UNDER_ROOT,            "",                                            STD_x_UNDER_ROOT,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1017 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SPACE_EM,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1018 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SPACE_3_PER_EM,                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1019 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SPACE_4_PER_EM,                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1020 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SPACE_6_PER_EM,                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1021 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SPACE_FIGURE,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1022 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SPACE_PUNCTUATION,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1023 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SPACE_HAIR,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1024 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_LEFT_SINGLE_QUOTE,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1025 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_RIGHT_SINGLE_QUOTE,                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1026 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SINGLE_LOW_QUOTE,                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1027 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SINGLE_HIGH_QUOTE,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1028 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_LEFT_DOUBLE_QUOTE,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1029 */  { addItemToBuffer,              ITM_RIGHT_DOUBLE_QUOTE,      "",                                            STD_RIGHT_DOUBLE_QUOTE,                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1030 */  { addItemToBuffer,              ITM_DOUBLE_LOW_QUOTE,        "",                                            STD_DOUBLE_LOW_QUOTE,                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1031 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_DOUBLE_HIGH_QUOTE,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1032 */  { addItemToBuffer,              ITM_ELLIPSIS,                "",                                            STD_ELLIPSIS,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1033 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_ONE,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1034 */  { addItemToBuffer,              ITM_EURO,                    "",                                            STD_EURO,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1035 */  { addItemToBuffer,              ITM_COMPLEX_C,               "",                                            STD_COMPLEX_C,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1036 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_PLANCK,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1037 */  { addItemToBuffer,              ITM_PLANCK_2PI,              "",                                            STD_PLANCK_2PI,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1038 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_NATURAL_N,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1039 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_RATIONAL_Q,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1040 */  { addItemToBuffer,              ITM_REAL_R,                  "",                                            STD_REAL_R,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1041 */  { addItemToBuffer,              ITM_LEFT_ARROW,              "",                                            STD_LEFT_ARROW,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1042 */  { addItemToBuffer,              ITM_UP_ARROW,                "",                                            STD_UP_ARROW,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1043 */  { itemToBeCoded,                NOPARAM,                     "1043",                                        "1043",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1044 */  { addItemToBuffer,              ITM_RIGHT_ARROW,             "",                                            STD_RIGHT_ARROW,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1045 */  { addItemToBuffer,              ITM_DOWN_ARROW,              "",                                            STD_DOWN_ARROW,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1046 */  { itemToBeCoded,                NOPARAM,                     "1046",                                        "1046",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1047 */  { addItemToBuffer,              ITM_SERIAL_IO,               "",                                            STD_SERIAL_IO,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1048 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_RIGHT_SHORT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1049 */  { addItemToBuffer,              ITM_LEFT_RIGHT_ARROWS,       "",                                            STD_LEFT_RIGHT_ARROWS,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1050 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_BST,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1051 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SST,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1052 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_HAMBURGER,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1053 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_UNDO,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1054 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_FOR_ALL,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1055 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_COMPLEMENT,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1056 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_PARTIAL_DIFF,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1057 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_THERE_EXISTS,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1058 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_THERE_DOES_NOT_EXIST,                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1059 */  { addItemToBuffer,              ITM_EMPTY_SET,               "",                                            STD_EMPTY_SET,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1060 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_INCREMENT,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1061 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_NABLA,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1062 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_ELEMENT_OF,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1063 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_NOT_ELEMENT_OF,                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1064 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_CONTAINS,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1065 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_DOES_NOT_CONTAIN,                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1066 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_ZERO,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1067 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_PRODUCT,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1068 */  { itemToBeCoded,                NOPARAM,                     "1068",                                        "1068",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1069 */  { itemToBeCoded,                NOPARAM,                     "1069",                                        "1069",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1070 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_MINUS_PLUS,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1071 */  { itemToBeCoded,                NOPARAM,                     "1071",                                        "1071",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1072 */  { itemToBeCoded,                NOPARAM,                     "1072",                                        "1072",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1073 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_RING,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1074 */  { addItemToBuffer,              ITM_BULLET,                  "",                                            STD_BULLET,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1075 */  { addItemToBuffer,              ITM_SQUARE_ROOT,             "",                                            STD_SQUARE_ROOT,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1076 */  { addItemToBuffer,              ITM_CUBE_ROOT,               "",                                            STD_CUBE_ROOT,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1077 */  { addItemToBuffer,              ITM_xTH_ROOT,                "",                                            STD_xTH_ROOT,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1078 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_PROPORTIONAL,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1079 */  { addItemToBuffer,              ITM_INFINITY,                "",                                            STD_INFINITY,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1080 */  { addItemToBuffer,              ITM_RIGHT_ANGLE,             "",                                            STD_RIGHT_ANGLE,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1081 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_ANGLE,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1082 */  { addItemToBuffer,              ITM_MEASURED_ANGLE,          "",                                            STD_MEASURED_ANGLE,                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1083 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_DIVIDES,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1084 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_DOES_NOT_DIVIDE,                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1085 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_PARALLEL,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1086 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_NOT_PARALLEL,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1087 */  { addItemToBuffer,              ITM_AND,                     "",                                            STD_AND,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1088 */  { addItemToBuffer,              ITM_OR,                      "",                                            STD_OR,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1089 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_INTERSECTION,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1090 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_UNION,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1091 */  { addItemToBuffer,              ITM_INTEGRAL_SIGN,           "",                                            STD_INTEGRAL,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1092 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_DOUBLE_INTEGRAL,                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1093 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_CONTOUR_INTEGRAL,                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1094 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_SURFACE_INTEGRAL,                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1095 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_RATIO,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1096 */  { addItemToBuffer,              ITM_CHECK_MARK,              "",                                            STD_CHECK_MARK,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1097 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_ASYMPOTICALLY_EQUAL,                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1098 */  { addItemToBuffer,              ITM_ALMOST_EQUAL,            "",                                            STD_ALMOST_EQUAL,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1099 */  { addItemToBuffer,              ITM_COLON_EQUALS,            "",                                            STD_COLON_EQUALS,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1100 */  { addItemToBuffer,              ITM_CORRESPONDS_TO,          "",                                            STD_CORRESPONDS_TO,                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1101 */  { addItemToBuffer,              ITM_ESTIMATES,               "",                                            STD_ESTIMATES,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1102 */  { addItemToBuffer,              ITM_NOT_EQUAL,               "",                                            STD_NOT_EQUAL,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1103 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_IDENTICAL_TO,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1104 */  { addItemToBuffer,              ITM_LESS_EQUAL,              "",                                            STD_LESS_EQUAL,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1105 */  { addItemToBuffer,              ITM_GREATER_EQUAL,           "",                                            STD_GREATER_EQUAL,                             (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1106 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_MUCH_LESS,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1107 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_MUCH_GREATER,                              (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1108 */  { addItemToBuffer,              ITM_SUN,                     "",                                            STD_SUN,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1109 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_DOWN_TACK,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1110 */  { addItemToBuffer,              ITM_PERPENDICULAR,           "",                                            STD_PERPENDICULAR,                             (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1111 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_XOR,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1112 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_NAND,                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1113 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_NOR,                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1114 */  { addItemToBuffer,              ITM_WATCH,                   "",                                            STD_WATCH,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1115 */  { addItemToBuffer,              ITM_HOURGLASS,               "",                                            STD_HOURGLASS,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1116 */  { addItemToBuffer,              ITM_PRINTER,                 "",                                            STD_PRINTER,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1117 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_MAT_TL,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1118 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_MAT_ML,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1119 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_MAT_BL,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1120 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_MAT_TR,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1121 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_MAT_MR,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1122 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_MAT_BR,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1123 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_OBLIQUE1,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1124 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_OBLIQUE2,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1125 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_OBLIQUE3,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1126 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_OBLIQUE4,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1127 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_CURSOR,                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1128 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_PERIOD34,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1129 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_COMMA34,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1130 */  { addItemToBuffer,              ITM_BATTERY,                 "",                                            STD_BATTERY,                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1131 */  { addItemToBuffer,              ITM_PGM_BEGIN,               "",                                            STD_PGM_BEGIN,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1132 */  { addItemToBuffer,              ITM_USER_MODE,               "",                                            STD_USER_MODE,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1133 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_UK,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1134 */  { itemToBeCoded,                NOPARAM,                     "",                                            STD_US,                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1135 */  { addItemToBuffer,              ITM_NEG_EXCLAMATION_MARK,    "",                                            STD_NEG_EXCLAMATION_MARK,                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1136 */  { addItemToBuffer,              ITM_ex,                      "",                                            STD_LEFT_RIGHT_ARROWS,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1137 */  { addItemToBuffer,              ITM_Max,                     "",                                            "Max",                                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1138 */  { addItemToBuffer,              ITM_Min,                     "",                                            "Min",                                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1139 */  { addItemToBuffer,              ITM_Config,                  "",                                            "Config",                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1140 */  { addItemToBuffer,              ITM_Stack,                   "",                                            "Stack",                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1141 */  { addItemToBuffer,              ITM_dddEL,                   "",                                            STD_ELLIPSIS "EL",                             (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1142 */  { addItemToBuffer,              ITM_dddIJ,                   "",                                            STD_ELLIPSIS "IJ",                             (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1143 */  { addItemToBuffer,              ITM_0P,                      "",                                            "0.",                                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1144 */  { addItemToBuffer,              ITM_1P,                      "",                                            "1.",                                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1145 */  { addItemToBuffer,              ITM_EXPONENT,                "",                                            "E",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1146 */  { addItemToBuffer,              NOPARAM,                     "HEX",                                         "H",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1147 */  { fnGoToRow,                    TM_VALUE,                    "GOTO Row",                                    "GOTO",                                        (0 << TAM_MAX_BITS) |  9999, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1148 */  { fnGoToColumn,                 TM_VALUE,                    "GOTO Column",                                 "GOTO",                                        (0 << TAM_MAX_BITS) |  9999, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1149 */  { fnSolveVar,                   NOPARAM,                     "SOLVE Var",                                   "SOLVE",                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1150 */  { fnEqCursorLeft,               NOPARAM,                     "",                                            STD_LEFT_ARROW,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1151 */  { fnEqCursorRight,              NOPARAM,                     "",                                            STD_RIGHT_ARROW,                               (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1152 */  { addItemToBuffer,              ITM_PAIR_OF_PARENTHESES,     "",                                            "( )",                                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1153 */  { addItemToBuffer,              ITM_VERTICAL_BAR,            "",                                            "|",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1154 */  { addItemToBuffer,              ITM_ALOG_SYMBOL,             "",                                            "e" STD_SUB_E,                                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1155 */  { addItemToBuffer,              ITM_ROOT_SIGN,               "",                                            STD_SQUARE_ROOT STD_SUP_MINUS,                 (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1156 */  { addItemToBuffer,              ITM_TIMER_SYMBOL,            "",                                            STD_TIMER,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1157 */  { itemToBeCoded,                NOPARAM,                     "1157",                                        "1157",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1158 */  { itemToBeCoded,                NOPARAM,                     "1158",                                        "1158",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1159 */  { itemToBeCoded,                NOPARAM,                     "1159",                                        "1159",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1160 */  { itemToBeCoded,                NOPARAM,                     "1160",                                        "1160",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1161 */  { itemToBeCoded,                NOPARAM,                     "1161",                                        "1161",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1162 */  { itemToBeCoded,                NOPARAM,                     "1162",                                        "1162",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1163 */  { itemToBeCoded,                NOPARAM,                     "1163",                                        "1163",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1164 */  { itemToBeCoded,                NOPARAM,                     "1164",                                        "1164",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1165 */  { itemToBeCoded,                NOPARAM,                     "1165",                                        "1165",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1166 */  { itemToBeCoded,                NOPARAM,                     "1166",                                        "1166",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1167 */  { itemToBeCoded,                NOPARAM,                     "1167",                                        "1167",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1168 */  { itemToBeCoded,                NOPARAM,                     "1168",                                        "1168",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1169 */  { itemToBeCoded,                NOPARAM,                     "1169",                                        "1169",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1170 */  { itemToBeCoded,                NOPARAM,                     "1170",                                        "1170",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1171 */  { itemToBeCoded,                NOPARAM,                     "1171",                                        "1171",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1172 */  { itemToBeCoded,                NOPARAM,                     "1172",                                        "1172",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1173 */  { itemToBeCoded,                NOPARAM,                     "1173",                                        "1173",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1174 */  { itemToBeCoded,                NOPARAM,                     "1174",                                        "1174",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},


// Reserved variables
/* 1175 */  { addItemToBuffer,              REGISTER_X,                  "X",                                           "X",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // Do
/* 1176 */  { addItemToBuffer,              REGISTER_Y,                  "Y",                                           "Y",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // not
/* 1177 */  { addItemToBuffer,              REGISTER_Z,                  "Z",                                           "Z",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // change
/* 1178 */  { addItemToBuffer,              REGISTER_T,                  "T",                                           "T",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // the
/* 1179 */  { addItemToBuffer,              REGISTER_A,                  "A",                                           "A",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // order
/* 1180 */  { addItemToBuffer,              REGISTER_B,                  "B",                                           "B",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // of
/* 1181 */  { addItemToBuffer,              REGISTER_C,                  "C",                                           "C",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // this
/* 1182 */  { addItemToBuffer,              REGISTER_D,                  "D",                                           "D",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // 3
/* 1183 */  { addItemToBuffer,              REGISTER_L,                  "L",                                           "L",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // 0
/* 1184 */  { addItemToBuffer,              REGISTER_I,                  "I",                                           "I",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // reserved
/* 1185 */  { addItemToBuffer,              REGISTER_J,                  "J",                                           "J",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // variables
/* 1186 */  { addItemToBuffer,              REGISTER_K,                  "K",                                           "K",                                           (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // !
/* 1187 */  { itemToBeCoded,                NOPARAM,                     "ADM",                                         "ADM",                                         (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1188 */  { itemToBeCoded,                NOPARAM,                     "DENMAX",                                      "DENMAX",                                      (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1189 */  { itemToBeCoded,                NOPARAM,                     "ISM",                                         "ISM",                                         (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1190 */  { itemToBeCoded,                NOPARAM,                     "REALDF",                                      "REALDF",                                      (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1191 */  { itemToBeCoded,                NOPARAM,                     "#DEC",                                        "#DEC",                                        (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1192 */  { itemToBeCoded,                NOPARAM,                     "ACC",                                         "ACC",                                         (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1193 */  { itemToBeCoded,                NOPARAM,                     STD_UP_ARROW "Lim",                            STD_UP_ARROW "Lim",                            (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1194 */  { itemToBeCoded,                NOPARAM,                     STD_DOWN_ARROW "Lim",                          STD_DOWN_ARROW "Lim",                          (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1195 */  { fnTvmVar,                     RESERVED_VARIABLE_FV,        "FV",                                          "FV",                                          (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1196 */  { fnTvmVar,                     RESERVED_VARIABLE_IPONA,     "i%/a",                                        "i%/a",                                        (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1197 */  { fnTvmVar,                     RESERVED_VARIABLE_NPER,      "NPER",                                        "n" STD_SUB_P STD_SUB_E STD_SUB_R,             (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1198 */  { fnTvmVar,                     RESERVED_VARIABLE_PERONA,    "PER/a",                                       "per/a",                                       (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1199 */  { fnTvmVar,                     RESERVED_VARIABLE_PMT,       "PMT",                                         "PMT",                                         (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1200 */  { fnTvmVar,                     RESERVED_VARIABLE_PV,        "PV",                                          "PV",                                          (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1201 */  { itemToBeCoded,                NOPARAM,                     "GRAMOD",                                      "GRAMOD",                                      (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1202 */  { fnEditLinearEquationMatrixA,  NOPARAM,                     "Mat_A",                                       "Mat A",                                       (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1203 */  { fnEditLinearEquationMatrixB,  NOPARAM,                     "Mat_B",                                       "Mat B",                                       (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1204 */  { itemToBeCoded,                NOPARAM,                     "Mat_X",                                       "Mat X",                                       (0 << TAM_MAX_BITS) |     0, CAT_RVAR | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1205 */  { itemToBeCoded,                NOPARAM,                     "1205",                                        "1205",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1206 */  { itemToBeCoded,                NOPARAM,                     "1206",                                        "1206",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},


// Probability distributions
/* 1207 */  { itemToBeCoded,                NOPARAM,                     "Binom:",                                      "Binom:",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1208 */  { fnBinomialP,                  NOPARAM,                     "Binom" STD_SUB_p,                             "Binom" STD_SUB_p,                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1209 */  { fnBinomialL,                  NOPARAM,                     "Binom" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Binom" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1210 */  { fnBinomialR,                  NOPARAM,                     "Binom" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Binom" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1211 */  { fnBinomialI,                  NOPARAM,                     "Binom" STD_SUP_MINUS_1,                       "Binom" STD_SUP_MINUS_1,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1212 */  { itemToBeCoded,                NOPARAM,                     "Cauch:",                                      "Cauch:",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1213 */  { fnCauchyP,                    NOPARAM,                     "Cauch" STD_SUB_p,                             "Cauch" STD_SUB_p,                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1214 */  { fnCauchyL,                    NOPARAM,                     "Cauch" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Cauch" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1215 */  { fnCauchyR,                    NOPARAM,                     "Cauch" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Cauch" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1216 */  { fnCauchyI,                    NOPARAM,                     "Cauch" STD_SUP_MINUS_1,                       "Cauch" STD_SUP_MINUS_1,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1217 */  { itemToBeCoded,                NOPARAM,                     "Expon:",                                      "Expon:",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1218 */  { fnExponentialP,               NOPARAM,                     "Expon" STD_SUB_p,                             "Expon" STD_SUB_p,                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1219 */  { fnExponentialL,               NOPARAM,                     "Expon" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Expon" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1220 */  { fnExponentialR,               NOPARAM,                     "Expon" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Expon" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1221 */  { fnExponentialI,               NOPARAM,                     "Expon" STD_SUP_MINUS_1,                       "Expon" STD_SUP_MINUS_1,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1222 */  { itemToBeCoded,                NOPARAM,                     "F:",                                          "F:",                                          (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1223 */  { fnF_P,                        NOPARAM,                     "F" STD_SUB_p "(x)",                           "F" STD_SUB_p "(x)",                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1224 */  { fnF_L,                        NOPARAM,                     "F" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", "F" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1225 */  { fnF_R,                        NOPARAM,                     "F" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", "F" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1226 */  { fnF_I,                        NOPARAM,                     "F" STD_SUP_MINUS_1 "(p)",                     "F" STD_SUP_MINUS_1 "(p)",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1227 */  { itemToBeCoded,                NOPARAM,                     "Geom:",                                       "Geom:",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1228 */  { fnGeometricP,                 NOPARAM,                     "Geom" STD_SUB_p,                              "Geom" STD_SUB_p,                              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1229 */  { fnGeometricL,                 NOPARAM,                     "Geom" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,    "Geom" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1230 */  { fnGeometricR,                 NOPARAM,                     "Geom" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,    "Geom" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1231 */  { fnGeometricI,                 NOPARAM,                     "Geom" STD_SUP_MINUS_1,                        "Geom" STD_SUP_MINUS_1,                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1232 */  { itemToBeCoded,                NOPARAM,                     "Hyper:",                                      "Hyper:",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1233 */  { fnHypergeometricP,            NOPARAM,                     "Hyper" STD_SUB_p,                             "Hyper" STD_SUB_p,                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1234 */  { fnHypergeometricL,            NOPARAM,                     "Hyper" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Hyper" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1235 */  { fnHypergeometricR,            NOPARAM,                     "Hyper" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Hyper" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1236 */  { fnHypergeometricI,            NOPARAM,                     "Hyper" STD_SUP_MINUS_1,                       "Hyper" STD_SUP_MINUS_1,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1237 */  { itemToBeCoded,                NOPARAM,                     "LgNrm:",                                      "LgNrm:",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1238 */  { fnLogNormalP,                 NOPARAM,                     "LgNrm" STD_SUB_p,                             "LgNrm" STD_SUB_p,                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1239 */  { fnLogNormalL,                 NOPARAM,                     "LgNrm" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "LgNrm" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1240 */  { fnLogNormalR,                 NOPARAM,                     "LgNrm" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "LgNrm" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1241 */  { fnLogNormalI,                 NOPARAM,                     "LgNrm" STD_SUP_MINUS_1,                       "LgNrm" STD_SUP_MINUS_1,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1242 */  { itemToBeCoded,                NOPARAM,                     "Logis:",                                      "Logis:",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1243 */  { fnLogisticP,                  NOPARAM,                     "Logis" STD_SUB_p,                             "Logis" STD_SUB_p,                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1244 */  { fnLogisticL,                  NOPARAM,                     "Logis" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Logis" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1245 */  { fnLogisticR,                  NOPARAM,                     "Logis" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Logis" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1246 */  { fnLogisticI,                  NOPARAM,                     "Logis" STD_SUP_MINUS_1,                       "Logis" STD_SUP_MINUS_1,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1247 */  { itemToBeCoded,                NOPARAM,                     "NBin:",                                       "NBin:",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1248 */  { fnNegBinomialP,               NOPARAM,                     "NBin" STD_SUB_p,                              "NBin" STD_SUB_p,                              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1249 */  { fnNegBinomialL,               NOPARAM,                     "NBin" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,    "NBin" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1250 */  { fnNegBinomialR,               NOPARAM,                     "NBin" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,    "NBin" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1251 */  { fnNegBinomialI,               NOPARAM,                     "NBin" STD_SUP_MINUS_1,                        "NBin" STD_SUP_MINUS_1,                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1252 */  { itemToBeCoded,                NOPARAM,                     "Norml:",                                      "Norml:",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1253 */  { fnNormalP,                    NOPARAM,                     "Norml" STD_SUB_p,                             "Norml" STD_SUB_p,                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1254 */  { fnNormalL,                    NOPARAM,                     "Norml" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Norml" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1255 */  { fnNormalR,                    NOPARAM,                     "Norml" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Norml" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1256 */  { fnNormalI,                    NOPARAM,                     "Norml" STD_SUP_MINUS_1,                       "Norml" STD_SUP_MINUS_1,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1257 */  { itemToBeCoded,                NOPARAM,                     "Poiss:",                                      "Poiss:",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1258 */  { fnPoissonP,                   NOPARAM,                     "Poiss" STD_SUB_p,                             "Poiss" STD_SUB_p,                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1259 */  { fnPoissonL,                   NOPARAM,                     "Poiss" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Poiss" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1260 */  { fnPoissonR,                   NOPARAM,                     "Poiss" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Poiss" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1261 */  { fnPoissonI,                   NOPARAM,                     "Poiss" STD_SUP_MINUS_1,                       "Poiss" STD_SUP_MINUS_1,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1262 */  { itemToBeCoded,                NOPARAM,                     "t:",                                          "t:",                                          (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1263 */  { fnT_P,                        NOPARAM,                     "t" STD_SUB_p "(x)",                           "t" STD_SUB_p "(x)",                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1264 */  { fnT_L,                        NOPARAM,                     "t" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", "t" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)", (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1265 */  { fnT_R,                        NOPARAM,                     "t" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", "t" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)", (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1266 */  { fnT_I,                        NOPARAM,                     "t" STD_SUP_MINUS_1 "(p)",                     "t" STD_SUP_MINUS_1 "(p)",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1267 */  { itemToBeCoded,                NOPARAM,                     "Weibl:",                                      "Weibl:",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1268 */  { fnWeibullP,                   NOPARAM,                     "Weibl" STD_SUB_p,                             "Weibl" STD_SUB_p,                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1269 */  { fnWeibullL,                   NOPARAM,                     "Weibl" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   "Weibl" STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1270 */  { fnWeibullR,                   NOPARAM,                     "Weibl" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   "Weibl" STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R,   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1271 */  { fnWeibullI,                   NOPARAM,                     "Weibl" STD_SUP_MINUS_1,                       "Weibl" STD_SUP_MINUS_1,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1272 */  { itemToBeCoded,                NOPARAM,                     STD_chi STD_SUP_2 ":",                         STD_chi STD_SUP_2 ":",                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1273 */  { fnChi2P,                      NOPARAM,                     STD_chi STD_SUP_2 STD_SUB_p "(x)",             STD_chi STD_SUP_2 STD_SUB_p "(x)",             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1274 */  { fnChi2L,                      NOPARAM,                     STD_chi STD_SUP_2 STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)",
                                                                                                                        STD_chi STD_SUP_2 STD_GAUSS_BLACK_L STD_GAUSS_WHITE_R "(x)",
                                                                                                                                                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1275 */  { fnChi2R,                      NOPARAM,                     STD_chi STD_SUP_2 STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)",
                                                                                                                        STD_chi STD_SUP_2 STD_GAUSS_WHITE_L STD_GAUSS_BLACK_R "(x)",
                                                                                                                                                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1276 */  { fnChi2I,                      NOPARAM,                     "(" STD_chi STD_SUP_2 ")" STD_SUP_MINUS_1,     "(" STD_chi STD_SUP_2 ")" STD_SUP_MINUS_1,     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1277 */  { itemToBeCoded,                NOPARAM,                     "1277",                                        "1277",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1278 */  { itemToBeCoded,                NOPARAM,                     "1278",                                        "1278",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1279 */  { itemToBeCoded,                NOPARAM,                     "1279",                                        "1279",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1280 */  { itemToBeCoded,                NOPARAM,                     "1280",                                        "1280",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1281 */  { itemToBeCoded,                NOPARAM,                     "1281",                                        "1281",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1282 */  { itemToBeCoded,                NOPARAM,                     "1282",                                        "1282",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1283 */  { itemToBeCoded,                NOPARAM,                     "1283",                                        "1283",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1284 */  { itemToBeCoded,                NOPARAM,                     "1284",                                        "1284",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1285 */  { itemToBeCoded,                NOPARAM,                     "1285",                                        "1285",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1286 */  { itemToBeCoded,                NOPARAM,                     "1286",                                        "1286",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1287 */  { itemToBeCoded,                NOPARAM,                     "1287",                                        "1287",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1288 */  { itemToBeCoded,                NOPARAM,                     "1288",                                        "1288",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1289 */  { itemToBeCoded,                NOPARAM,                     "1289",                                        "1289",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1290 */  { itemToBeCoded,                NOPARAM,                     "1290",                                        "1290",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1291 */  { itemToBeCoded,                NOPARAM,                     "1291",                                        "1291",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1292 */  { itemToBeCoded,                NOPARAM,                     "1292",                                        "1292",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1293 */  { itemToBeCoded,                NOPARAM,                     "1293",                                        "1293",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1294 */  { itemToBeCoded,                NOPARAM,                     "1294",                                        "1294",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1295 */  { itemToBeCoded,                NOPARAM,                     "1295",                                        "1295",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1296 */  { itemToBeCoded,                NOPARAM,                     "1296",                                        "1296",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},


// Curve fitting
/* 1297 */  { fnCurveFitting,               TM_VALUE,                    "BestF",                                       "BestF",                                       (0 << TAM_MAX_BITS) |   510, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1298 */  { fnCurveFitting,               CF_EXPONENTIAL_FITTING_EX,   "ExpF",                                        "ExpF",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1299 */  { fnCurveFitting,               CF_LINEAR_FITTING_EX,        "LinF",                                        "LinF",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1300 */  { fnCurveFitting,               CF_LOGARITHMIC_FITTING_EX,   "LogF",                                        "LogF",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1301 */  { fnCurveFitting,               CF_ORTHOGONAL_FITTING_EX,    "OrthoF",                                      "OrthoF",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1302 */  { fnCurveFitting,               CF_POWER_FITTING_EX,         "PowerF",                                      "PowerF",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1303 */  { fnCurveFitting,               CF_GAUSS_FITTING_EX,         "GaussF",                                      "GaussF",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1304 */  { fnCurveFitting,               CF_CAUCHY_FITTING_EX,        "CauchF",                                      "CauchF",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1305 */  { fnCurveFitting,               CF_PARABOLIC_FITTING_EX,     "ParabF",                                      "ParabF",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1306 */  { fnCurveFitting,               CF_HYPERBOLIC_FITTING_EX,    "HypF",                                        "HypF",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1307 */  { fnCurveFitting,               CF_ROOT_FITTING_EX,          "RootF",                                       "RootF",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1308 */  { itemToBeCoded,                NOPARAM,                     "1308",                                        "1308",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1309 */  { itemToBeCoded,                NOPARAM,                     "1309",                                        "1309",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1310 */  { itemToBeCoded,                NOPARAM,                     "1310",                                        "1310",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1311 */  { itemToBeCoded,                NOPARAM,                     "1311",                                        "1311",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1312 */  { itemToBeCoded,                NOPARAM,                     "1312",                                        "1312",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},


// Menus
/* 1313 */  { itemToBeCoded,                NOPARAM,                     "ADV",                                         "ADV",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1314 */  { itemToBeCoded,                NOPARAM,                     "ANGLES",                                      "ANGLES",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1315 */  { itemToBeCoded,                NOPARAM,                     "PRINT",                                       "PRINT",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1316 */  { itemToBeCoded,                NOPARAM,                     "A:",                                          "A:",                                          (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1317 */  { itemToBeCoded,                NOPARAM,                     "BITS",                                        "BITS",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1318 */  { itemToBeCoded,                NOPARAM,                     "CATALOG",                                     "CATALOG",                                     (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1319 */  { itemToBeCoded,                NOPARAM,                     "CHARS",                                       "CHARS",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1320 */  { itemToBeCoded,                NOPARAM,                     "CLK",                                         "CLK",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1321 */  { itemToBeCoded,                NOPARAM,                     "CLR",                                         "CLR",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1322 */  { itemToBeCoded,                NOPARAM,                     "CONST",                                       "CONST",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1323 */  { itemToBeCoded,                NOPARAM,                     "CPX",                                         "CPX",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1324 */  { itemToBeCoded,                NOPARAM,                     "CPXS",                                        "CPXS",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1325 */  { itemToBeCoded,                NOPARAM,                     "DATES",                                       "DATES",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1326 */  { itemToBeCoded,                NOPARAM,                     "DISP",                                        "DISP",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1327 */  { itemToBeCoded,                NOPARAM,                     "EQN",                                         "EQN",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1328 */  { itemToBeCoded,                NOPARAM,                     "EXP",                                         "EXP",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1329 */  { itemToBeCoded,                NOPARAM,                     "E:",                                          "E:",                                          (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1330 */  { itemToBeCoded,                NOPARAM,                     "FCNS",                                        "FCNS",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1331 */  { itemToBeCoded,                NOPARAM,                     "FIN",                                         "FIN",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1332 */  { itemToBeCoded,                NOPARAM,                     "S.INTS",                                      "S.INTS",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1333 */  { itemToBeCoded,                NOPARAM,                     "FLAGS",                                       "FLAGS",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1334 */  { itemToBeCoded,                NOPARAM,                     "FLASH",                                       "FLASH",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1335 */  { itemToBeCoded,                NOPARAM,                     "f'",                                          "f'",                                          (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1336 */  { itemToBeCoded,                NOPARAM,                     "f\"",                                         "f\"",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1337 */  { itemToBeCoded,                NOPARAM,                     "F&p:",                                        "F&p:",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1338 */  { itemToBeCoded,                NOPARAM,                     "L.INTS",                                      "L.INTS",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1339 */  { itemToBeCoded,                NOPARAM,                     "INFO",                                        "INFO",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1340 */  { itemToBeCoded,                NOPARAM,                     "INTS",                                        "INTS",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1341 */  { itemToBeCoded,                NOPARAM,                     "I/O",                                         "I/O",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1342 */  { itemToBeCoded,                NOPARAM,                     "LOOP",                                        "LOOP",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1343 */  { itemToBeCoded,                NOPARAM,                     "MATRS",                                       "MATRS",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1344 */  { itemToBeCoded,                NOPARAM,                     "MATX",                                        "MATX",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1345 */  { itemToBeCoded,                NOPARAM,                     "MENUS",                                       "MENUS",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1346 */  { itemToBeCoded,                NOPARAM,                     "MODE",                                        "MODE",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1347 */  { itemToBeCoded,                NOPARAM,                     "M.SIMQ",                                      "M.SIMQ",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1348 */  { itemToBeCoded,                NOPARAM,                     "M.EDIT",                                      "M.EDIT",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1349 */  { itemToBeCoded,                NOPARAM,                     "MyMenu",                                      "MyMenu",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1350 */  { itemToBeCoded,                NOPARAM,                     "My" STD_alpha,                                "My" STD_alpha,                                (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1351 */  { itemToBeCoded,                NOPARAM,                     "m:",                                          "m:",                                          (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1352 */  { itemToBeCoded,                NOPARAM,                     "ORTHOG",                                      "Orthog",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1353 */  { itemToBeCoded,                NOPARAM,                     "PARTS",                                       "PARTS",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1354 */  { itemToBeCoded,                NOPARAM,                     "PROB",                                        "PROB",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1355 */  { itemToBeCoded,                NOPARAM,                     "PROGS",                                       "PROGS",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1356 */  { itemToBeCoded,                NOPARAM,                     "P.FN",                                        "P.FN",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1357 */  { itemToBeCoded,                NOPARAM,                     "P.FN2",                                       "P.FN2",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1358 */  { itemToBeCoded,                NOPARAM,                     "P:",                                          "P:",                                          (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1359 */  { itemToBeCoded,                NOPARAM,                     "RAM",                                         "RAM",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1360 */  { itemToBeCoded,                NOPARAM,                     "REALS",                                       "REALS",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1361 */  { itemToBeCoded,                NOPARAM,                     "Solver",                                      "Solver",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1362 */  { itemToBeCoded,                NOPARAM,                     "STAT",                                        "STAT",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1363 */  { itemToBeCoded,                NOPARAM,                     "STK",                                         "STK",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1364 */  { itemToBeCoded,                NOPARAM,                     "STRING",                                      "STRING",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1365 */  { itemToBeCoded,                NOPARAM,                     "TEST",                                        "TEST",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1366 */  { itemToBeCoded,                NOPARAM,                     "TIMES",                                       "TIMES",                                       (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1367 */  { itemToBeCoded,                NOPARAM,                     "TRI",                                         "TRI",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1368 */  { itemToBeCoded,                NOPARAM,                     "TVM",                                         "TVM",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1369 */  { itemToBeCoded,                NOPARAM,                     "U" STD_RIGHT_ARROW,                           "U" STD_RIGHT_ARROW,                           (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1370 */  { itemToBeCoded,                NOPARAM,                     "VARS",                                        "VARS",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1371 */  { itemToBeCoded,                NOPARAM,                     "V:",                                          "V:",                                          (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1372 */  { itemToBeCoded,                NOPARAM,                     "X.FN",                                        "X.FN",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1373 */  { itemToBeCoded,                NOPARAM,                     "x:",                                          "x:",                                          (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1374 */  { itemToBeCoded,                NOPARAM,                     STD_alpha "INTL",                              STD_alpha "INTL",                              (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1375 */  { itemToBeCoded,                NOPARAM,                     STD_alpha "MATH",                              STD_alpha "MATH",                              (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1376 */  { itemToBeCoded,                NOPARAM,                     STD_alpha ".FN",                               STD_alpha ".FN",                               (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1377 */  { itemToBeCoded,                NOPARAM,                     STD_ALPHA STD_ELLIPSIS STD_OMEGA,              STD_ALPHA STD_ELLIPSIS STD_OMEGA,              (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // Upper case greek letters
/* 1378 */  { itemToBeCoded,                NOPARAM,                     STD_alpha STD_BULLET,                          STD_alpha STD_BULLET,                          (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // Upper case intl letters
/* 1379 */  { itemToBeCoded,                NOPARAM,                     "SYS.FL",                                      "SYS.FL",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1380 */  { itemToBeCoded,                NOPARAM,                     STD_INTEGRAL "f",                              STD_INTEGRAL "f",                              (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1381 */  { itemToBeCoded,                NOPARAM,                     STD_INTEGRAL "fdx",                            STD_INTEGRAL "fdx",                            (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1382 */  { itemToBeCoded,                NOPARAM,                     STD_MEASURED_ANGLE STD_RIGHT_ARROW,            STD_MEASURED_ANGLE STD_RIGHT_ARROW,            (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1383 */  { itemToBeCoded,                NOPARAM,                     STD_alpha STD_ELLIPSIS STD_omega,              STD_alpha STD_ELLIPSIS STD_omega,              (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // Lower case greek letters
/* 1384 */  { itemToBeCoded,                NOPARAM,                     STD_alpha "intl",                              STD_alpha "intl",                              (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // lower case intl letters
/* 1385 */  { itemToBeCoded,                NOPARAM,                     "",                                            "Tam",                                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1386 */  { itemToBeCoded,                NOPARAM,                     "",                                            "TamCmp",                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1387 */  { itemToBeCoded,                NOPARAM,                     "",                                            "TamStoRcl",                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1388 */  { itemToBeCoded,                NOPARAM,                     STD_SIGMA,                                     STD_SIGMA,                                     (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1389 */  { itemToBeCoded,                NOPARAM,                     "VAR",                                         "VAR",                                         (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1390 */  { itemToBeCoded,                NOPARAM,                     "",                                            "TamFlag",                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1391 */  { itemToBeCoded,                NOPARAM,                     "",                                            "TamShuffle",                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1392 */  { itemToBeCoded,                NOPARAM,                     "PROG",                                        "PROG",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1393 */  { itemToBeCoded,                NOPARAM,                     "",                                            "TamLabel",                                    (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1394 */  { fnDynamicMenu,                NOPARAM,                     "",                                            "DYNMNU",                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1395 */  { itemToBeCoded,                NOPARAM,                     "PLOT.ST",                                     "PLOT.ST",                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1396 */  { itemToBeCoded,                NOPARAM,                     "PLOT.LR",                                     "PLOT.LR",                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1397 */  { itemToBeCoded,                NOPARAM,                     "ELLIPT",                                      "Ellipt",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1398 */  { fnSolveVar,                   NOPARAM,                     "MVAR",                                        "MVAR",                                        (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED}, // solver variables
/* 1399 */  { itemToBeCoded,                NOPARAM,                     "EQ.EDIT",                                     "EQ.EDIT",                                     (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1400 */  { itemToBeCoded,                NOPARAM,                     "TIMERF",                                      "TIMERF",                                      (0 << TAM_MAX_BITS) |     0, CAT_MENU | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1401 */  { itemToBeCoded,                NOPARAM,                     "1401",                                        "1401",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1402 */  { itemToBeCoded,                NOPARAM,                     "1402",                                        "1402",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1403 */  { itemToBeCoded,                NOPARAM,                     "1403",                                        "1403",                                        (0 << TAM_MAX_BITS) |     0, CAT_FREE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},


/* 1404 */  { fnIntegerMode,                SIM_1COMPL,                  "1COMPL",                                      "1COMPL",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1405 */  { fnScreenDump,                 NOPARAM,                     "SNAP",                                        "SNAP",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1406 */  { fnIntegerMode,                SIM_2COMPL,                  "2COMPL",                                      "2COMPL",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1407 */  { fnMagnitude,                  NOPARAM,                     "ABS",                                         "ABS",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1408 */  { fnAgm,                        NOPARAM,                     "AGM",                                         "AGM",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1409 */  { itemToBeCoded,                NOPARAM,                     "AGRAPH",                                      "AGRAPH",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1410 */  { fnDisplayFormatAll,           TM_VALUE,                    "ALL" ,                                        "ALL",                                         (0 << TAM_MAX_BITS) |    15, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1411 */  { fnAssign,                     0,                           "ASSIGN",                                      "ASN",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1412 */  { fnBack,                       TM_VALUE,                    "BACK",                                        "BACK",                                        (0 << TAM_MAX_BITS) |   255, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1413 */  { fnBatteryVoltage,             NOPARAM,                     "BATT?",                                       "BATT?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1414 */  { fnBeep,                       NOPARAM,                     "BEEP",                                        "BEEP",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1415 */  { fnTvmBeginMode,               NOPARAM,                     "BeginP",                                      "Begin",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1416 */  { fnBn,                         NOPARAM,                     "B" STD_SUB_n,                                 "B" STD_SUB_n,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1417 */  { fnBnStar,                     NOPARAM,                     "B" STD_SUB_n STD_SUP_ASTERISK,                "B" STD_SUB_n STD_SUP_ASTERISK,                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1418 */  { fnCase,                       TM_REGISTER,                 "CASE",                                        "CASE",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1419 */  { fnClAll,                      NOT_CONFIRMED,               "CLALL",                                       "CLall",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABL_XEQ | EIM_DISABLED},
/* 1420 */  { fnClCVar,                     NOPARAM,                     "CLCVAR",                                      "CLCVAR",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABL_XEQ | EIM_DISABLED},
/* 1421 */  { fnClFAll,                     NOT_CONFIRMED,               "CLFALL",                                      "CLFall",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABL_XEQ | EIM_DISABLED},
/* 1422 */  { fnFractionType,               NOPARAM,                     "a b/c",                                       "a b/c",                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1423 */  { itemToBeCoded,                NOPARAM,                     "CLLCD",                                       "CLLCD",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABL_XEQ | EIM_DISABLED},
/* 1424 */  { itemToBeCoded,                NOPARAM,                     "CLMENU",                                      "CLMENU",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABL_XEQ | EIM_DISABLED},
/* 1425 */  { fnClP,                        NOPARAM,                     "CLP",                                         "CLP",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_CANCEL    | EIM_DISABLED},
/* 1426 */  { fnClPAll,                     NOT_CONFIRMED,               "CLPALL",                                      "CLPall",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_CANCEL    | EIM_DISABLED},
/* 1427 */  { fnClearRegisters,             NOT_CONFIRMED,               "CLREGS",                                      "CLREGS",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_CANCEL    | EIM_DISABLED},
/* 1428 */  { fnClearStack,                 NOPARAM,                     "CLSTK",                                       "CLSTK",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_CANCEL    | EIM_DISABLED},
/* 1429 */  { fnClSigma,                    NOPARAM,                     "CL" STD_SIGMA,                                "CL" STD_SIGMA,                                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABL_XEQ | EIM_DISABLED},
/* 1430 */  { fnStoreMax,                   NOPARAM,                     "STO" STD_UP_ARROW,                            "Max",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1431 */  { fnConjugate,                  NOPARAM,                     "CONJ",                                        "conj",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1432 */  { fnRecallMax,                  NOPARAM,                     "RCL" STD_UP_ARROW,                            "Max",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1433 */  { fnCoefficientDetermination,   NOPARAM,                     "CORR",                                        "r",                                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1434 */  { fnPopulationCovariance,       NOPARAM,                     "COV",                                         "cov",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1435 */  { fnCurveFittingLR,             NOPARAM,                     "BestF?",                                      "BestF?",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1436 */  { fnCross,                      NOPARAM,                     "CROSS",                                       "cross",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1437 */  { fnCxToRe,                     NOPARAM,                     "CX" STD_RIGHT_ARROW "RE",                     "CX" STD_RIGHT_ARROW "RE",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1438 */  { fnDate,                       NOPARAM,                     "DATE",                                        "DATE",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1439 */  { fnDateTo,                     NOPARAM,                     "DATE" STD_RIGHT_ARROW,                        "DATE" STD_RIGHT_ARROW,                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1440 */  { fnDay,                        NOPARAM,                     "DAY",                                         "DAY",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1441 */  { fnDblDivideRemainder,         NOPARAM,                     "DBLR",                                        "DBLR",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1442 */  { fnDblMultiply,                NOPARAM,                     "DBL" STD_CROSS,                               "DBL" STD_CROSS,                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1443 */  { fnDblDivide,                  NOPARAM,                     "DBL/",                                        "DBL/",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1444 */  { fnDecomp,                     NOPARAM,                     "DECOMP",                                      "DECOMP",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1445 */  { fnAngularMode,                amDegree,                    "DEG",                                         "DEG",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1446 */  { fnCvtToCurrentAngularMode,    amDegree,                    "DEG" STD_RIGHT_ARROW,                         "DEG" STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1447 */  { fnStatSa,                     NOPARAM,                     "s(a)",                                        "s(a)",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1448 */  { fnDenMax,                     NOPARAM,                     "DENMAX",                                      "DENMAX",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1449 */  { fnDot,                        NOPARAM,                     "DOT",                                         "dot",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1450 */  { fnDisplayStack,               TM_VALUE,                    "DSTACK",                                      "DSTACK",                                      (1 << TAM_MAX_BITS) |     4, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1451 */  { fnAngularMode,                amDMS,                       "D.MS",                                        "d.ms",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1452 */  { fnCvtDmsToCurrentAngularMode, NOPARAM,                     "D.MS" STD_RIGHT_ARROW,                        "D.MS" STD_RIGHT_ARROW,                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1453 */  { fnSetDateFormat,              ITM_DMY,                     "D.MY",                                        "D.MY",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1454 */  { fnDateToJulian,               NOPARAM,                     "D" STD_RIGHT_ARROW "J",                       "D" STD_RIGHT_ARROW "J",                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1455 */  { itemToBeCoded,                NOPARAM,                     "DELITM",                                      "DELITM",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1456 */  { fnEigenvalues,                NOPARAM,                     "EIGVAL",                                      "EIGVAL",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1457 */  { fnEigenvectors,               NOPARAM,                     "EIGVEC",                                      "EIGVEC",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1458 */  { fnReturn,                     0,                           "END",                                         "END",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1459 */  { fnTvmEndMode,                 NOPARAM,                     "ENDP",                                        "End",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1460 */  { fnDisplayFormatEng,           TM_VALUE,                    "ENG",                                         "ENG",                                         (0 << TAM_MAX_BITS) |    15, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1461 */  { fnEuclideanNorm,              NOPARAM,                     "ENORM",                                       "ENORM",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1462 */  { fnRecallMin,                  NOPARAM,                     "RCL" STD_DOWN_ARROW,                          "Min",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1463 */  { fnEqDelete,                   NOPARAM,                     "EQ.DEL",                                      "DELETE",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1464 */  { fnEqEdit,                     NOPARAM,                     "EQ.EDI",                                      "EDIT",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1465 */  { fnEqNew,                      NOPARAM,                     "EQ.NEW",                                      "NEW",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1466 */  { fnErf,                        NOPARAM,                     "erf",                                         "erf",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1467 */  { fnErfc,                       NOPARAM,                     "erfc",                                        "erfc",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1468 */  { fnRaiseError,                 TM_VALUE,                    "ERR",                                         "ERR",                                         (1 << TAM_MAX_BITS) | (NUMBER_OF_ERROR_CODES - 1),
                                                                                                                                                                                                    CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1469 */  { itemToBeCoded,                NOPARAM,                     "EXITALL",                                     "EXITall",                                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1470 */  { fnExpt,                       NOPARAM,                     "EXPT",                                        "EXPT",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1471 */  { fnGetFirstGregorianDay,       NOPARAM,                     "J/G?",                                        "J/G?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1472 */  { fnFib,                        NOPARAM,                     "FIB",                                         "FIB",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1473 */  { fnDisplayFormatFix,           TM_VALUE,                    "FIX",                                         "FIX",                                         (0 << TAM_MAX_BITS) |    15, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1474 */  { fnFreeFlashMemory,            NOPARAM,                     "FLASH?",                                      "FLASH?",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1475 */  { itemToBeCoded,                NOPARAM,                     "f'(x)",                                       "f'(x)",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1476 */  { itemToBeCoded,                NOPARAM,                     "f\"(x)",                                      "f\"(x)",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1477 */  { fnDisplayFormatGap,           TM_VALUE,                    "GAP",                                         "GAP",                                         (0 << TAM_MAX_BITS) |    15, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1478 */  { fnGd,                         NOPARAM,                     "g" STD_SUB_d,                                 "g" STD_SUB_d,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1479 */  { fnInvGd,                      NOPARAM,                     "g" STD_SUB_d STD_SUP_MINUS_1,                 "g" STD_SUB_d STD_SUP_MINUS_1,                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1480 */  { fnAngularMode,                amGrad,                      "GRAD",                                        "GRAD",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1481 */  { fnCvtToCurrentAngularMode,    amGrad,                      "GRAD" STD_RIGHT_ARROW,                        "GRAD" STD_RIGHT_ARROW,                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1482 */  { fnGotoDot,                    NOPARAM,                     "GTO.",                                        "GTO.",                                        (0 << TAM_MAX_BITS) | 16383, CAT_FNCT | SLS_ENABLED   | US_CANCEL    | EIM_DISABLED},
/* 1483 */  { fnHermite,                    NOPARAM,                     "H" STD_SUB_n,                                 "H" STD_SUB_n,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1484 */  { fnHermiteP,                   NOPARAM,                     "H" STD_SUB_n STD_SUB_P,                       "H" STD_SUB_n STD_SUB_P,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1485 */  { fnImaginaryPart,              NOPARAM,                     "Im",                                          "Im",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1486 */  { fnIndexMatrix,                TM_REGISTER,                 "INDEX",                                       "INDEX",                                       (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_UNCHANGED | US_ENABLED   | EIM_DISABLED},
/* 1487 */  { fnIxyz,                       NOPARAM,                     "I" STD_SUB_x STD_SUB_y STD_SUB_z,             "I" STD_SUB_x STD_SUB_y STD_SUB_z,             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1488 */  { fnGammaP,                     NOPARAM,                     "I" STD_GAMMA STD_SUB_p,                       "I" STD_GAMMA STD_SUB_p,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1489 */  { fnGammaQ,                     NOPARAM,                     "I" STD_GAMMA STD_SUB_q,                       "I" STD_GAMMA STD_SUB_q,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1490 */  { fnIncDecI,                    INC_FLAG,                    "I+",                                          "I+",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1491 */  { fnIncDecI,                    DEC_FLAG,                    "I-",                                          "I-",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1492 */  { fnBesselJ,                    NOPARAM,                     "J" STD_SUB_y "(x)",                           "J" STD_SUB_y "(x)",                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1493 */  { fnIncDecJ,                    INC_FLAG,                    "J+",                                          "J+",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1494 */  { fnIncDecJ,                    DEC_FLAG,                    "J-",                                          "J-",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1495 */  { fnSetFirstGregorianDay,       NOPARAM,                     "J/G",                                         "J/G",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1496 */  { fnJulianToDate,               NOPARAM,                     "J" STD_RIGHT_ARROW "D",                       "J" STD_RIGHT_ARROW "D",                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1497 */  { itemToBeCoded,                NOPARAM,                     "KEY",                                         "KEY",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1498 */  { itemToBeCoded,                NOPARAM,                     "KEYG",                                        "KEYG",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1499 */  { itemToBeCoded,                NOPARAM,                     "KEYX",                                        "KEYX",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1500 */  { fnSinc,                       NOPARAM,                     "sinc",                                        "sinc",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1501 */  { itemToBeCoded,                NOPARAM,                     "KTYP?",                                       "KTYP?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1502 */  { fnLastX,                      NOPARAM,                     "LASTx",                                       "LASTx",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1503 */  { itemToBeCoded,                NOPARAM,                     "LBL?",                                        "LBL?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1504 */  { fnIsLeap,                     NOPARAM,                     "LEAP?",                                       "LEAP?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1505 */  { fnLaguerre,                   NOPARAM,                     "L" STD_SUB_m ,                                "L" STD_SUB_m ,                                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1506 */  { fnLaguerreAlpha,              NOPARAM,                     "L" STD_SUB_m STD_SUB_alpha,                   "L" STD_SUB_m STD_SUB_alpha,                   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1507 */  { fnLnBeta,                     NOPARAM,                     "ln" STD_beta,                                 "ln" STD_beta,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1508 */  { fnLnGamma,                    NOPARAM,                     "ln" STD_GAMMA,                                "ln" STD_GAMMA,                                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1509 */  { fnLoad,                       LM_ALL,                      "LOAD",                                        "LOAD",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_CANCEL    | EIM_DISABLED},
/* 1510 */  { fnLoad,                       LM_PROGRAMS,                 "LOADP",                                       "LOADP",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1511 */  { fnLoad,                       LM_REGISTERS,                "LOADR",                                       "LOADR",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1512 */  { fnLoad,                       LM_SYSTEM_STATE,             "LOADSS",                                      "LOADSS",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1513 */  { fnLoad,                       LM_SUMS,                     "LOAD" STD_SIGMA,                              "LOAD" STD_SIGMA,                              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1514 */  { allocateLocalRegisters,       TM_VALUE,                    "LocR",                                        "LocR",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1515 */  { fnGetLocR,                    NOPARAM,                     "LocR?",                                       "LocR?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1516 */  { fnProcessLR,                  NOPARAM,                     "L.R.",                                        "L.R.",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1517 */  { fnMant,                       NOPARAM,                     "MANT",                                        "MANT",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1518 */  { fnEditLinearEquationMatrixX,  NOPARAM,                     "Mat_X",                                       "Mat X",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1519 */  { fnFreeMemory,                 NOPARAM,                     "MEM?",                                        "MEM?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1520 */  { itemToBeCoded,                NOPARAM,                     "MENU",                                        "MENU",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1521 */  { fnMonth,                      NOPARAM,                     "MONTH",                                       "MONTH",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1522 */  { itemToBeCoded,                NOPARAM,                     "MSG",                                         "MSG",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1523 */  { fnAngularMode,                amMultPi,                    "MUL" STD_pi,                                  "MUL" STD_pi,                                  (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1524 */  { fnNop,                        TM_REGISTER,                 "MVAR",                                        "MVAR",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1525 */  { fnDelRow,                     NOPARAM,                     "M.DELR",                                      "DELR",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1526 */  { fnSetMatrixDimensions,        TM_M_DIM,                    "M.DIM",                                       "DIM",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_UNCHANGED | US_ENABLED   | EIM_DISABLED},
/* 1527 */  { fnGetMatrixDimensions,        NOPARAM,                     "M.DIM?",                                      "DIM?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1528 */  { fnSetDateFormat,              ITM_MDY,                     "M.DY",                                        "M.DY",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1529 */  { fnEditMatrix,                 NOPARAM,                     "M.EDI",                                       "EDIT",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_UNCHANGED | US_ENABLED   | EIM_DISABLED},
/* 1530 */  { fnEditMatrix,                 TM_REGISTER,                 "M.EDIN",                                      "EDITN",                                       (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_UNCHANGED | US_ENABLED   | EIM_DISABLED},
/* 1531 */  { fnGetMatrix,                  NOPARAM,                     "M.GET",                                       "GETM",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1532 */  { fnGoToElement,                NOPARAM,                     "M.GOTO",                                      "GOTO",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1533 */  { fnSetGrowMode,                ON,                          "M.GROW",                                      "GROW",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1534 */  { fnInsRow,                     NOPARAM,                     "M.INSR",                                      "INSR",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1535 */  { fnLuDecomposition,            NOPARAM,                     "M.LU",                                        "M.LU",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1536 */  { fnNewMatrix,                  NOPARAM,                     "M.NEW",                                       "NEW",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1537 */  { fnOldMatrix,                  NOPARAM,                     "M.OLD",                                       "OLD",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1538 */  { fnPutMatrix,                  NOPARAM,                     "M.PUT",                                       "PUTM",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1539 */  { fnSwapRows,                   NOPARAM,                     "M.R" STD_LEFT_RIGHT_ARROWS "R",               "R" STD_LEFT_RIGHT_ARROWS "R",                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1540 */  { fnSincpi,                     NOPARAM,                     "sinc" STD_pi,                                 "sinc" STD_pi,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1541 */  { fnSetGrowMode,                OFF,                         "M.WRAP",                                      "WRAP",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1542 */  { fnNop,                        NOPARAM,                     "NOP",                                         "NOP",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1543 */  { fnOff,                        NOPARAM,                     "OFF",                                         "OFF",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1544 */  { fnDropY,                      NOPARAM,                     "DROPy",                                       "DROPy",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1545 */  { fnStoreMin,                   NOPARAM,                     "STO" STD_DOWN_ARROW,                          "Min",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1546 */  { itemToBeCoded,                NOPARAM,                     "PGMINT",                                      "PGMINT",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1547 */  { fnPgmSlv,                     TM_SOLVE,                    "PGMSLV",                                      "PGMSLV",                                      (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1548 */  { itemToBeCoded,                NOPARAM,                     "PIXEL",                                       "PIXEL",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1549 */  { fnPlotStat,                   PLOT_START,                  "PLOT",                                        "PLOT",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1550 */  { fnLegendre,                   NOPARAM,                     "P" STD_SUB_n,                                 "P" STD_SUB_n,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1551 */  { itemToBeCoded,                NOPARAM,                     "POINT",                                       "POINT",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1552 */  { fnLoad,                       LM_NAMED_VARIABLES,          "LOADV",                                       "LOADV",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1553 */  { allocateLocalRegisters,       0,                           "PopLR",                                       "PopLR",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1554 */  { itemToBeCoded,                NOPARAM,                     "PRCL",                                        "PRCL",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1555 */  { itemToBeCoded,                NOPARAM,                     "PSTO",                                        "PSTO",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1556 */  { itemToBeCoded,                NOPARAM,                     "PUTK",                                        "PUTK",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1557 */  { fnAngularMode,                amRadian,                    "RAD",                                         "RAD",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1558 */  { fnCvtToCurrentAngularMode,    amRadian,                    "RAD" STD_RIGHT_ARROW,                         "RAD" STD_RIGHT_ARROW,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1559 */  { fnRandom,                     NOPARAM,                     "RAN#",                                        "RAN#",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1560 */  { registerBrowser,              NOPARAM,                     "RBR",                                         "RBR",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1561 */  { fnRecallConfig,               NOPARAM,                     "RCLCFG",                                      "Config",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1562 */  { fnRecallElement,              NOPARAM,                     "RCLEL",                                       "RCLEL",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1563 */  { fnRecallIJ,                   NOPARAM,                     "RCLIJ",                                       "RCLIJ",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1564 */  { fnRecallStack,                TM_REGISTER,                 "RCLS",                                        "RCLS",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1565 */  { fnRdp,                        TM_VALUE,                    "RDP",                                         "RDP",                                         (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1566 */  { fnRealPart,                   NOPARAM,                     "Re",                                          "Re",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1567 */  { itemToBeCoded,                NOPARAM,                     "RECV",                                        "RECV",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1568 */  { fnReset,                      NOT_CONFIRMED,               "RESET",                                       "RESET",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABL_XEQ | EIM_DISABLED},
/* 1569 */  { fnReToCx,                     NOPARAM,                     "RE" STD_RIGHT_ARROW "CX",                     "RE" STD_RIGHT_ARROW "CX",                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1570 */  { fnSwapRealImaginary,          NOPARAM,                     "Re" STD_LEFT_RIGHT_ARROWS "Im",               "Re" STD_LEFT_RIGHT_ARROWS "Im",               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1571 */  { fnSetRoundingMode,            TM_VALUE,                    "RM",                                          "RM",                                          (0 << TAM_MAX_BITS) |     6, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1572 */  { fnGetRoundingMode,            NOPARAM,                     "RM?",                                         "RM?",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1573 */  { fnDisplayFormatDsp,           TM_VALUE,                    "DSP",                                         "DSP",                                         (0 << TAM_MAX_BITS) |    15, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1574 */  { fnRowNorm,                    NOPARAM,                     "RNORM",                                       "RNORM",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1575 */  { fnExpM1,                      NOPARAM,                     "e" STD_SUP_x "-1",                            "e" STD_SUP_x "-1",                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1576 */  { fnRoundi,                     NOPARAM,                     "ROUNDI",                                      "ROUNDI",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1577 */  { fnRsd,                        TM_VALUE,                    "RSD",                                         "RSD",                                         (1 << TAM_MAX_BITS) |    34, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1578 */  { fnRowSum,                     NOPARAM,                     "RSUM",                                        "RSUM",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1579 */  { fnReturn,                     1,                           "RTN+1",                                       "RTN+1",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1580 */  { fnRegClr,                     NOPARAM,                     "R-CLR",                                       "R-CLR",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1581 */  { fnRegCopy,                    NOPARAM,                     "R-COPY",                                      "R-COPY",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1582 */  { fnRegSort,                    NOPARAM,                     "R-SORT",                                      "R-SORT",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1583 */  { fnRegSwap,                    NOPARAM,                     "R-SWAP",                                      "R-SWAP",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1584 */  { fnJacobiAmplitude,            NOPARAM,                     STD_psi "(u,m)",                               STD_psi "(u,m)",                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1585 */  { fnSampleStdDev,               NOPARAM,                     "s",                                           "s",                                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1586 */  { fnSave,                       NOPARAM,                     "SAVE",                                        "SAVE",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1587 */  { fnDisplayFormatSci,           TM_VALUE,                    "SCI",                                         "SCI",                                         (0 << TAM_MAX_BITS) |    15, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1588 */  { fnGetSignificantDigits,       NOPARAM,                     "SDIGS?",                                      "SDIGS?",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1589 */  { fnSeed,                       NOPARAM,                     "SEED",                                        "SEED",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1590 */  { itemToBeCoded,                NOPARAM,                     "SEND",                                        "SEND",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1591 */  { fnConfigChina,                NOPARAM,                     "SETCHN",                                      "CHINA",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1592 */  { fnSetDate,                    NOPARAM,                     "SETDAT",                                      "SETDAT",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1593 */  { fnConfigEurope,               NOPARAM,                     "SETEUR",                                      "EUROPE",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1594 */  { fnConfigIndia,                NOPARAM,                     "SETIND",                                      "INDIA",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1595 */  { fnConfigJapan,                NOPARAM,                     "SETJPN",                                      "JAPAN",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1596 */  { fnSetSignificantDigits,       NOPARAM,                     "SETSIG",                                      "SETSIG",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1597 */  { fnSetTime,                    NOPARAM,                     "SETTIM",                                      "SETTIM",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1598 */  { fnConfigUk,                   NOPARAM,                     "SETUK",                                       "UK",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1599 */  { fnConfigUsa,                  NOPARAM,                     "SETUSA",                                      "USA",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1600 */  { fnSign,                       NOPARAM,                     "SIGN",                                        "sign",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1601 */  { fnIntegerMode,                SIM_SIGNMT,                  "SIGNMT",                                      "SIGNMT",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1602 */  { fnSimultaneousLinearEquation, TM_VALUE,                    "SIM_EQ",                                      "SIM EQ",                                      (1 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1603 */  { fnSkip,                       TM_VALUE,                    "SKIP",                                        "SKIP",                                        (0 << TAM_MAX_BITS) |   255, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1604 */  { fnSlvq,                       NOPARAM,                     "SLVQ",                                        "SLVQ",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1605 */  { fnStandardError,              NOPARAM,                     "s" STD_SUB_m,                                 "s" STD_SUB_m,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1606 */  { fnGetIntegerSignMode,         NOPARAM,                     "ISM?",                                        "ISM?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1607 */  { fnWeightedStandardError,      NOPARAM,                     "s" STD_SUB_m STD_SUB_w,                       "s" STD_SUB_m STD_SUB_w,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1608 */  { fnSolve,                      TM_SOLVE,                    "SOLVE",                                       "SOLVE",                                       (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1609 */  { fnGetStackSize,               NOPARAM,                     "SSIZE?",                                      "SSIZE?",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1610 */  { flagBrowser,                  NOPARAM,                     "STATUS",                                      "STATUS",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1611 */  { fnStoreConfig,                NOPARAM,                     "STOCFG",                                      "Config",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1612 */  { fnStoreElement,               NOPARAM,                     "STOEL",                                       "STOEL",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1613 */  { fnStoreIJ,                    NOPARAM,                     "STOIJ",                                       "STOIJ",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1614 */  { fnLnP1,                       NOPARAM,                     "ln(1+x)",                                     "ln 1+x",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1615 */  { fnStoreStack,                 TM_REGISTER,                 "STOS",                                        "STOS",                                        (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1616 */  { fnSumXY,                      NOPARAM,                     "SUM",                                         "SUM",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1617 */  { fnWeightedSampleStdDev,       NOPARAM,                     "s" STD_SUB_w,                                 "s" STD_SUB_w,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1618 */  { fnSampleCovariance,           NOPARAM,                     "s" STD_SUB_x STD_SUB_y,                       "s" STD_SUB_x STD_SUB_y,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1619 */  { fnDisplayFormatTime,          TM_VALUE,                    "TDISP",                                       "TDISP",                                       (0 << TAM_MAX_BITS) |     6, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1620 */  { fnTicks,                      NOPARAM,                     "TICKS",                                       "TICKS",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1621 */  { fnTime,                       NOPARAM,                     "TIME",                                        "TIME",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1622 */  { fnTimer,                      NOPARAM,                     "TIMER",                                       "TIMER",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1623 */  { fnChebyshevT,                 NOPARAM,                     "T" STD_SUB_n,                                 "T" STD_SUB_n,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1624 */  { fnTone,                       TM_VALUE,                    "TONE",                                        "TONE",                                        (0 << TAM_MAX_BITS) |     9, CAT_FNCT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1625 */  { fnSwapT,                      TM_REGISTER,                 "t" STD_LEFT_RIGHT_ARROWS,                     "t" STD_LEFT_RIGHT_ARROWS,                     (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1626 */  { fnUlp,                        NOPARAM,                     "ULP?",                                        "ULP?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1627 */  { fnChebyshevU,                 NOPARAM,                     "U" STD_SUB_n,                                 "U" STD_SUB_n,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1628 */  { fnUnitVector,                 NOPARAM,                     "UNITV",                                       "UNITV",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1629 */  { fnIntegerMode,                SIM_UNSIGN,                  "UNSIGN",                                      "UNSIGN",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1630 */  { itemToBeCoded,                NOPARAM,                     "VARMNU",                                      "VARMNU",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1631 */  { fnVersion,                    NOPARAM,                     "VERS?",                                       "VERS?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1632 */  { fnIDivR,                      NOPARAM,                     "IDIVR",                                       "IDIVR",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1633 */  { fnWday,                       NOPARAM,                     "WDAY",                                        "WDAY",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1634 */  { fnWho,                        NOPARAM,                     "WHO?",                                        "WHO?",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1635 */  { fnWnegative,                  NOPARAM,                     "W" STD_SUB_m,                                 "W" STD_SUB_m,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1636 */  { fnWpositive,                  NOPARAM,                     "W" STD_SUB_p,                                 "W" STD_SUB_p,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1637 */  { fnWinverse,                   NOPARAM,                     "W" STD_SUP_MINUS_1,                           "W" STD_SUP_MINUS_1,                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1638 */  { fnSetWordSize,                TM_VALUE,                    "WSIZE",                                       "WSIZE",                                       (0 << TAM_MAX_BITS) |    64, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1639 */  { fnGetWordSize,                NOPARAM,                     "WSIZE?",                                      "WSIZE?",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1640 */  { fnMeanXY,                     NOPARAM,                     STD_x_BAR,                                     STD_x_BAR,                                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1641 */  { fnGeometricMeanXY,            NOPARAM,                     STD_x_BAR STD_SUB_G,                           STD_x_BAR STD_SUB_G,                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1642 */  { fnWeightedMeanX,              NOPARAM,                     STD_x_BAR STD_SUB_w,                           STD_x_BAR STD_SUB_w,                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1643 */  { fnXIsFny,                     NOPARAM,                     STD_x_CIRC,                                    STD_x_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1644 */  { fnXToDate,                    NOPARAM,                     "x" STD_RIGHT_ARROW "DATE",                    "x" STD_RIGHT_ARROW "DATE",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1645 */  { fnXToAlpha,                   NOPARAM,                     "x" STD_RIGHT_ARROW STD_alpha,                 "x" STD_RIGHT_ARROW STD_alpha,                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1646 */  { fnQrDecomposition,            NOPARAM,                     "M.QR",                                        "M.QR",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1647 */  { fnYear,                       NOPARAM,                     "YEAR",                                        "YEAR",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1648 */  { fnYIsFnx,                     NOPARAM,                     STD_y_CIRC,                                    STD_y_CIRC,                                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1649 */  { fnSetDateFormat,              ITM_YMD,                     "Y.MD",                                        "Y.MD",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1650 */  { fnSwapY,                      TM_REGISTER,                 "y" STD_LEFT_RIGHT_ARROWS,                     "y" STD_LEFT_RIGHT_ARROWS,                     (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1651 */  { fnSwapZ,                      TM_REGISTER,                 "z" STD_LEFT_RIGHT_ARROWS,                     "z" STD_LEFT_RIGHT_ARROWS,                     (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1652 */  { fnAlphaLeng,                  TM_REGISTER,                 STD_alpha "LENG?",                             STD_alpha "LENG?",                             (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1653 */  { fnXmax,                       NOPARAM,                     "x" STD_SUB_m STD_SUB_a STD_SUB_x,             "x" STD_SUB_m STD_SUB_a STD_SUB_x,             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1654 */  { fnXmin,                       NOPARAM,                     "x" STD_SUB_m STD_SUB_i STD_SUB_n,             "x" STD_SUB_m STD_SUB_i STD_SUB_n,             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1655 */  { fnAlphaPos,                   TM_REGISTER,                 STD_alpha "POS?",                              STD_alpha "POS?",                              (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1656 */  { fnAlphaRL,                    TM_REGISTER,                 STD_alpha "RL",                                STD_alpha "RL",                                (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1657 */  { fnAlphaRR,                    TM_REGISTER,                 STD_alpha "RR",                                STD_alpha "RR",                                (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1658 */  { fnAlphaSL,                    TM_REGISTER,                 STD_alpha "SL",                                STD_alpha "SL",                                (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1659 */  { fnAlphaSR,                    TM_REGISTER,                 STD_alpha "SR",                                STD_alpha "SR",                                (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1660 */  { fnAlphaToX,                   TM_REGISTER,                 STD_alpha STD_RIGHT_ARROW "x",                 STD_alpha STD_RIGHT_ARROW "x",                 (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1661 */  { fnBeta,                       NOPARAM,                     STD_beta "(x,y)",                              STD_beta "(x,y)",                              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1662 */  { fnGammaXyLower,               NOPARAM,                     STD_gamma STD_SUB_x STD_SUB_y,                 STD_gamma STD_SUB_x STD_SUB_y,                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1663 */  { fnGammaXyUpper,               NOPARAM,                     STD_GAMMA STD_SUB_x STD_SUB_y,                 STD_GAMMA STD_SUB_x STD_SUB_y,                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1664 */  { fnGamma,                      NOPARAM,                     STD_GAMMA "(x)",                               STD_GAMMA "(x)",                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1665 */  { fnBesselY,                    NOPARAM,                     "Y" STD_SUB_y "(x)",                           "Y" STD_SUB_y "(x)",                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1666 */  { fnDeltaPercent,               NOPARAM,                     STD_DELTA "%",                                 STD_DELTA "%",                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1667 */  { fnGeometricSampleStdDev,      NOPARAM,                     STD_epsilon,                                   STD_epsilon,                                   (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1668 */  { fnGeometricStandardError,     NOPARAM,                     STD_epsilon STD_SUB_m,                         STD_epsilon STD_SUB_m,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1669 */  { fnGeometricPopulationStdDev,  NOPARAM,                     STD_epsilon STD_SUB_p,                         STD_epsilon STD_SUB_p,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1670 */  { fnZeta,                       NOPARAM,                     STD_zeta "(x)",                                STD_zeta "(x)",                                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1671 */  { fnProgrammableProduct,        TM_LABEL,                    STD_PI STD_SUB_n,                              STD_PI STD_SUB_n,                              (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1672 */  { fnProgrammableSum,            TM_LABEL,                    STD_SIGMA STD_SUB_n,                           STD_SIGMA STD_SUB_n,                           (0 << TAM_MAX_BITS) |    99, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1673 */  { fnPopulationStdDev,           NOPARAM,                     STD_sigma,                                     STD_sigma,                                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1674 */  { fnWeightedPopulationStdDev,   NOPARAM,                     STD_sigma STD_SUB_w,                           STD_sigma STD_SUB_w,                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1675 */  { fnRandomI,                    NOPARAM,                     "RANI#",                                       "RANI#",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1676 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "x",                               STD_PRINTER "x",                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1677 */  { fnRange,                      NOPARAM,                     "RANGE",                                       "RANGE",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1678 */  { fnGetRange,                   NOPARAM,                     "RANGE?",                                      "RANGE?",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1679 */  { fnM1Pow,                      NOPARAM,                     "(-1)" STD_SUP_x,                              "(-1)" STD_SUP_x,                              (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1680 */  { fnMulMod,                     NOPARAM,                     STD_CROSS "MOD",                               STD_CROSS "MOD",                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1681 */  { fnToDate,                     NOPARAM,                     STD_RIGHT_ARROW "DATE",                        STD_RIGHT_ARROW "DATE",                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1682 */  { fnJacobiSn,                   NOPARAM,                     "sn(u,m)",                                     "sn(u,m)",                                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1683 */  { fnJacobiCn,                   NOPARAM,                     "cn(u,m)",                                     "cn(u,m)",                                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1684 */  { fnJacobiDn,                   NOPARAM,                     "dn(u,m)",                                     "dn(u,m)",                                     (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1685 */  { fnToHr,                       NOPARAM,                     STD_RIGHT_ARROW "HR",                          ".d",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1686 */  { fnToHms,                      NOPARAM,                     STD_RIGHT_ARROW "H.MS",                        "h.ms",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1687 */  { fnChangeBase,                 TM_VALUE_CHB,                STD_RIGHT_ARROW "INT",                         "#",                                           (2 << TAM_MAX_BITS) |    16, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1688 */  { fnToPolar,                    NOPARAM,                     STD_RIGHT_ARROW "POL",                         STD_RIGHT_ARROW "P",                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1689 */  { fnCvtMultPiToRad,             NOPARAM,                     "M" STD_pi STD_RIGHT_ARROW "R",                "M" STD_pi STD_RIGHT_ARROW "R",                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1690 */  { fnCvtRadToMultPi,             NOPARAM,                     "R" STD_RIGHT_ARROW "M" STD_pi,                "R" STD_RIGHT_ARROW "M" STD_pi,                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1691 */  { fnToReal,                     NOPARAM,                     STD_RIGHT_ARROW "REAL",                        ".d",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1692 */  { fnToRect,                     NOPARAM,                     STD_RIGHT_ARROW "REC",                         "R" STD_LEFT_ARROW,                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1693 */  { fnCvtDegToDms,                NOPARAM,                     "D" STD_RIGHT_ARROW "D.MS",                    "D" STD_RIGHT_ARROW "D.MS",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1694 */  { fnShuffle,                    TM_SHUFFLE,                  STD_LEFT_RIGHT_ARROWS,                         STD_LEFT_RIGHT_ARROWS,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1695 */  { fnPercent,                    NOPARAM,                     "%",                                           "%",                                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1696 */  { fnPercentMRR,                 NOPARAM,                     "%MRR",                                        "%MRR",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1697 */  { fnPercentT,                   NOPARAM,                     "%T",                                          "%T",                                          (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1698 */  { fnPercentSigma,               NOPARAM,                     "%" STD_SIGMA,                                 "%" STD_SIGMA,                                 (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1699 */  { fnPercentPlusMG,              NOPARAM,                     "%+MG",                                        "%+MG",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1700 */  { itemToBeCoded,                NOPARAM,                     STD_INTEGRAL,                                  STD_INTEGRAL,                                  (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1701 */  { fnExpMod,                     NOPARAM,                     "^MOD",                                        "^MOD",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1702 */  { fnDeterminant,                NOPARAM,                     "|M|",                                         "|M|",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1703 */  { fnParallel,                   NOPARAM,                     "||",                                          "|" STD_SPACE_3_PER_EM "|",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1704 */  { fnTranspose,                  NOPARAM,                     "[M]" STD_SUP_T,                               "[M]" STD_SUP_T,                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1705 */  { fnInvertMatrix,               NOPARAM,                     "[M]" STD_SUP_MINUS_1,                         "[M]" STD_SUP_MINUS_1,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1706 */  { fnArg,                        NOPARAM,                     STD_MEASURED_ANGLE,                            STD_MEASURED_ANGLE,                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1707 */  { fnCvtToCurrentAngularMode,    amMultPi,                    "MUL" STD_pi STD_RIGHT_ARROW,                  "MUL" STD_pi STD_RIGHT_ARROW,                  (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1708 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "ADV",                             STD_PRINTER "ADV",                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1709 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "CHAR",                            STD_PRINTER "CHAR",                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1710 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "DLAY",                            STD_PRINTER "DLAY",                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1711 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "LCD",                             STD_PRINTER "LCD",                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1712 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "MODE",                            STD_PRINTER "MODE",                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1713 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "PROG",                            STD_PRINTER "PROG",                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1714 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "r",                               STD_PRINTER "r",                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1715 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "REGS",                            STD_PRINTER "REGS",                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1716 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "STK",                             STD_PRINTER "STK",                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1717 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "TAB",                             STD_PRINTER "TAB",                             (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1718 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "USER",                            STD_PRINTER "USER",                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1719 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "WIDTH",                           STD_PRINTER "WIDTH",                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1720 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER STD_SIGMA,                         STD_PRINTER STD_SIGMA,                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1721 */  { itemToBeCoded,                NOPARAM,                     STD_PRINTER "#",                               STD_PRINTER "#",                               (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},

/* 1722 */  { fontBrowser,                  NOPARAM,                     "FBR",                                         "FBR",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED}, // Font browser

/* 1723 */  { fnUndo,                       NOPARAM,                     "UNDO",                                        STD_UNDO,                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1724 */  { fnPem,                        NOPARAM,                     "P/R",                                         "P/R",                                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_CANCEL    | EIM_DISABLED},
/* 1725 */  { fnRunProgram,                 NOPARAM,                     "R/S",                                         "R/S",                                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1726 */  { fnEllipticK,                  NOPARAM,                     "K(m)",                                        "K(m)",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1727 */  { fnEllipticE,                  NOPARAM,                     "E(m)",                                        "E(m)",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1728 */  { fnEllipticPi,                 NOPARAM,                     STD_PI "(n,m)",                                STD_PI "(n,m)",                                (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1729 */  { fnFlipFlag,                   FLAG_USER,                   "USER",                                        "USER",                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1730 */  { fnKeyCC,                      NOPARAM,                     "CC",                                          "CC",                                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1731 */  { itemToBeCoded,                NOPARAM,                     "",                                            "f",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1732 */  { itemToBeCoded,                NOPARAM,                     "",                                            "g",                                           (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1733 */  { fnKeyUp,                      NOPARAM,                     "UP",                                          STD_UP_ARROW,                                  (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1734 */  { fnBst,                        NOPARAM,                     "BST",                                         STD_HAMBURGER STD_BST,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1735 */  { fnKeyDown,                    NOPARAM,                     "DOWN",                                        STD_DOWN_ARROW,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1736 */  { fnSst,                        NOPARAM,                     "SST",                                         STD_HAMBURGER STD_SST,                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1737 */  { fnKeyExit,                    NOPARAM,                     "EXIT",                                        "EXIT",                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1738 */  { fnKeyBackspace,               NOPARAM,                     "BKSPC",                                       STD_LEFT_ARROW,                                (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1739 */  { fnKeyAngle,                   NOPARAM,                     STD_MEASURED_ANGLE,                            STD_MEASURED_ANGLE,                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1740 */  { fnAim,                        NOPARAM,                     "AIM",                                         STD_alpha,                                     (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1741 */  { fnKeyDotD,                    NOPARAM,                     ".d",                                          ".d",                                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1742 */  { fnShow,                       NOPARAM,                     "SHOW",                                        "SHOW",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1743 */  { backToSystem,                 NOPARAM,                     "SYSTEM",                                      "SYSTEM",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
/* 1744 */  { fnCvtDmsToDeg,                NOPARAM,                     "D.MS" STD_RIGHT_ARROW "D",                    "D.MS" STD_RIGHT_ARROW "D",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1745 */  { fnVectorAngle,                NOPARAM,                     "V" STD_MEASURED_ANGLE,                        STD_MEASURED_ANGLE,                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1746 */  { fnHarmonicMeanXY,             NOPARAM,                     STD_x_BAR STD_SUB_H,                           STD_x_BAR STD_SUB_H,                           (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1747 */  { fnRMSMeanXY,                  NOPARAM,                     STD_x_BAR STD_SUB_R STD_SUB_M STD_SUB_S,       STD_x_BAR STD_SUB_R STD_SUB_M STD_SUB_S,       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1748 */  { fnArccos,                     NOPARAM,                     "ACOS",                                        "ACOS",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1749 */  { fnArcsin,                     NOPARAM,                     "ASIN",                                        "ASIN",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1750 */  { fnArctan,                     NOPARAM,                     "ATAN",                                        "ATAN",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },
/* 1751 */  { fnDeterminant,                NOPARAM,                     "DET",                                         "DET",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1752 */  { fnInvertMatrix,               NOPARAM,                     "INVRT",                                       "INVRT",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1753 */  { fnTranspose,                  NOPARAM,                     "TRANS",                                       "TRANS",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1754 */  { itemToBeCoded,                NOPARAM,                     "xIN",                                         "xIN",                                         (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1755 */  { itemToBeCoded,                NOPARAM,                     "xOUT",                                        "xOUT",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1756 */  { fnPlotStat,                   PLOT_ORTHOF,                 "",                                            "CENTRL",                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1757 */  { fnHide,                       NOPARAM,                     "HIDE",                                        "HIDE",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1758 */  { fnPlotCloseSmi,               NOPARAM,                     "s" STD_SUB_m STD_SUB_i,                       "s" STD_SUB_m STD_SUB_i,                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_UNCHANGED | US_ENABLED   | EIM_DISABLED},
/* 1759 */  { fnPlotStat,                   PLOT_LR,                     "ASSESS",                                      "ASSESS",                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1760 */  { fnPlotStat,                   PLOT_NXT,                    "",                                            "NXTFIT",                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1761 */  { fnPlotStat,                   PLOT_REV,                    "",                                            "",                                            (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1762 */  { fnPlotZoom,                   NOPARAM,                     "",                                            "ZOOM",                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1763 */  { fnEllipticFphi,               NOPARAM,                     "F(" STD_phi ",m)",                            "F(" STD_phi ",m)",                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1764 */  { fnEllipticEphi,               NOPARAM,                     "E(" STD_phi ",m)",                            "E(" STD_phi ",m)",                            (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1765 */  { fnJacobiZeta,                 NOPARAM,                     STD_ZETA "(" STD_phi ",m)",                    STD_ZETA "(" STD_phi ",m)",                    (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1766 */  { fnGetHide,                    NOPARAM,                     "HIDE?",                                       "HIDE?",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1767 */  { fnEqCalc,                     NOPARAM,                     "Calc",                                        "Calc",                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1768 */  { fnSquareRoot,                 NOPARAM,                     "SQRT",                                        "SQRT",                                        (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },

/* 1769 */  { fnRecall,                     RESERVED_VARIABLE_FV,        "",                                            "RCL FV",                                      (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1770 */  { fnRecall,                     RESERVED_VARIABLE_IPONA,     "",                                            "i%/a",                                        (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1771 */  { fnRecall,                     RESERVED_VARIABLE_NPER,      "",                                            "RCL n" STD_SUB_P,                             (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1772 */  { fnRecall,                     RESERVED_VARIABLE_PERONA,    "",                                            "per/a",                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1773 */  { fnRecall,                     RESERVED_VARIABLE_PMT,       "",                                            "PMT",                                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},
/* 1774 */  { fnRecall,                     RESERVED_VARIABLE_PV,        "",                                            "PV",                                          (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_ENABLED   | EIM_DISABLED},

/* 1775 */  { fnAtan2,                      NOPARAM,                     "atan2",                                       "atan2",                                       (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_ENABLED   | US_ENABLED   | EIM_ENABLED },

/* 1776 */  { fnAddTimerApp,                NOPARAM,                     "",                                            "ADD",                                         (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1777 */  { fnDecisecondTimerApp,         NOPARAM,                     "",                                            "0.1s?",                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1778 */  { fnResetTimerApp,              NOPARAM,                     "",                                            "RESET",                                       (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1779 */  { fnRecallTimerApp,             TM_REGISTER,                 "RCL",                                         "",                                            (0 << TAM_MAX_BITS) |    99, CAT_NONE | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},
/* 1780 */  { fnDeleteBackup,               NOT_CONFIRMED,               "CLBKUP",                                      "CLBKUP",                                      (0 << TAM_MAX_BITS) |     0, CAT_FNCT | SLS_UNCHANGED | US_UNCHANGED | EIM_DISABLED},

/* 1781 */  { itemToBeCoded,                NOPARAM,                     "",                                            "Last item",                                   (0 << TAM_MAX_BITS) |     0, CAT_NONE | SLS_ENABLED   | US_UNCHANGED | EIM_DISABLED},
};
