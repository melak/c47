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

#include "stats.h"

#include "constantPointers.h"
#include "debug.h"
#include "error.h"
#include "flags.h"
#include "fonts.h"
#include "items.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/wp34s.h"
#include "matrix.h"
#include "memory.h"
#include "plotstat.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "stack.h"

#include "wp43s.h"


#if !defined(TESTSUITE_BUILD) //TESTSUITE_BUILD
  static void calcMax(uint16_t maxOffset);
  static void calcMin(uint16_t maxOffset);
#endif


#if !defined(TESTSUITE_BUILD)
  bool_t isStatsMatrix(uint16_t *rows, char *mx) {
    *rows = 0;
    calcRegister_t regStats = findNamedVariable(mx);
    if(regStats == INVALID_VARIABLE) {
      return false;
    }
    else {
      if(getRegisterDataType(regStats) != dtReal34Matrix) {
        return false;
      }
      else {
        real34Matrix_t stats;
        linkToRealMatrixRegister(regStats, &stats);
        *rows = stats.header.matrixRows;
        if(stats.header.matrixColumns != 2) {
          return false;
        }
      }
    }
    return true;
  }



  static void addMax(real_t *x, real_t *y) {
    // xmax
    if(realCompareGreaterThan(x, SIGMA_XMAX)) {
      realCopy(x, SIGMA_XMAX);
    }

    // ymax
    if(realCompareGreaterThan(y, SIGMA_YMAX)) {
      realCopy(y, SIGMA_YMAX);
    }
  }


  static void addMin(real_t *x, real_t *y) {
    // xmin
    if(realCompareLessThan(x, SIGMA_XMIN)) {
      realCopy(x, SIGMA_XMIN);
    }

    // ymin
    if(realCompareLessThan(y, SIGMA_YMIN)) {
      realCopy(y, SIGMA_YMIN);
    }
  }


  static void addSigma(real_t *x, real_t *y) {
    real_t tmpReal1, tmpReal2, tmpReal3;
    realContext_t *realContext = &ctxtReal75; // Summation data with 75 digits

    addMax(x, y);
    addMin(x, y);

    // n
    realAdd(SIGMA_N, const_1, SIGMA_N, realContext);

    // sigma x
    realAdd(SIGMA_X, x, SIGMA_X, realContext);

    // sigma y
    realAdd(SIGMA_Y, y, SIGMA_Y, realContext);

    // sigma x²
    realMultiply(x, x, &tmpReal1, realContext);
    realAdd(SIGMA_X2, &tmpReal1, SIGMA_X2, realContext);

    // sigma x³
    realMultiply(&tmpReal1, x, &tmpReal2, realContext);
    realAdd(SIGMA_X3, &tmpReal2, SIGMA_X3, realContext);

    // sigma x⁴
    realMultiply(&tmpReal2, x, &tmpReal2, realContext);
    realAdd(SIGMA_X4, &tmpReal2, SIGMA_X4, realContext);

    // sigma x²y
    realMultiply(&tmpReal1, y, &tmpReal2, realContext);
    realAdd(SIGMA_X2Y, &tmpReal2, SIGMA_X2Y, realContext);

    // sigma x²/y
    realDivide(&tmpReal1, y, &tmpReal2, realContext);
    realAdd(SIGMA_X2onY, &tmpReal2, SIGMA_X2onY, realContext);

    // sigma 1/x²
    realDivide(const_1, &tmpReal1, &tmpReal2, realContext);
    realAdd(SIGMA_1onX2, &tmpReal2, SIGMA_1onX2, realContext);

    // sigma y²
    realMultiply(y, y, &tmpReal1, realContext);
    realAdd(SIGMA_Y2, &tmpReal1, SIGMA_Y2, realContext);

    // sigma 1/y²
    realDivide(const_1, &tmpReal1, &tmpReal2, realContext);
    realAdd(SIGMA_1onY2, &tmpReal2, SIGMA_1onY2, realContext);

    // sigma xy
    realMultiply(x, y, &tmpReal1, realContext);
    realAdd(SIGMA_XY, &tmpReal1, SIGMA_XY, realContext);

    // sigma ln(x)
    WP34S_Ln(x, &tmpReal1, realContext);
    realCopy(&tmpReal1 ,&tmpReal3);
    realAdd(SIGMA_lnX, &tmpReal1, SIGMA_lnX, realContext);

    // sigma ln²(x)
    realMultiply(&tmpReal1, &tmpReal1, &tmpReal2, realContext);
    realAdd(SIGMA_ln2X, &tmpReal2, SIGMA_ln2X, realContext);

    // sigma yln(x)
    realMultiply(&tmpReal1, y, &tmpReal1, realContext);
    realAdd(SIGMA_YlnX, &tmpReal1, SIGMA_YlnX, realContext);

    // sigma ln(y)
    WP34S_Ln(y, &tmpReal1, realContext);
    realAdd(SIGMA_lnY, &tmpReal1, SIGMA_lnY, realContext);

    // sigma ln(x)×ln(y)
    realMultiply(&tmpReal3, &tmpReal1, &tmpReal3, realContext);
    realAdd(SIGMA_lnXlnY, &tmpReal3, SIGMA_lnXlnY, realContext);

    // sigma ln(y)/x
    realDivide(&tmpReal1, x, &tmpReal2, realContext);
    realAdd(SIGMA_lnYonX, &tmpReal2, SIGMA_lnYonX, realContext);

    // sigma ln²(y)
    realMultiply(&tmpReal1, &tmpReal1, &tmpReal2, realContext);
    realAdd(SIGMA_ln2Y, &tmpReal2, SIGMA_ln2Y, realContext);

    // sigma xln(y)
    realMultiply(&tmpReal1, x, &tmpReal1, realContext);
    realAdd(SIGMA_XlnY, &tmpReal1, SIGMA_XlnY, realContext);

    // sigma x²ln(y)
    realMultiply(&tmpReal1, x, &tmpReal1, realContext);
    realAdd(SIGMA_X2lnY, &tmpReal1, SIGMA_X2lnY, realContext);

    // sigma 1/x
    realDivide(const_1, x, &tmpReal1, realContext);
    realAdd(SIGMA_1onX, &tmpReal1, SIGMA_1onX, realContext);

    // sigma x/y
    realDivide(x, y, &tmpReal1, realContext);
    realAdd(SIGMA_XonY, &tmpReal1, SIGMA_XonY, realContext);

    // sigma 1/y
    realDivide(const_1, y, &tmpReal1, realContext);
    realAdd(SIGMA_1onY, &tmpReal1, SIGMA_1onY, realContext);
  }


  static bool_t ignoreMaxIfValid(real_t *r1, real_t *r2){
    if(realIsNaN (r1) || realIsNaN (r2) || realIsInfinite (r1) || realIsInfinite (r2) || realCompareEqual(r1, r2)) {
      calcMax(1);
      return false;
    }
    return true;
  }

  static bool_t ignoreMinIfValid(real_t *r1, real_t *r2){
    if(realIsNaN (r1) || realIsNaN (r2) || realIsInfinite (r1) || realIsInfinite (r2) || realCompareEqual(r1, r2)) {
      calcMin(1);
      return false;
    }
    return true;
  }


  static bool_t realSubtractIfValid(real_t *r1, real_t *r2, real_t *r3, realContext_t *ct){
    if(realIsNaN (r1) || realIsNaN (r2) || realIsInfinite (r1) || realIsInfinite (r2)) {
      calcSigma(1);
      return false;
    }
    realSubtract(r1, r2, r3, ct);
    return true;
  }


  static void subSigma(real_t *x, real_t *y) {
    real_t tmpReal1, tmpReal2, tmpReal3;
    realContext_t *realContext = &ctxtReal75; // Summation data with 75 digits
   // SIGMA-

    // xmax
    if(!ignoreMaxIfValid(x, SIGMA_XMAX)) goto endMax;

    // ymax
    if(!ignoreMaxIfValid(y, SIGMA_YMAX)) goto endMax;

    endMax:

    // xmin
    if(!ignoreMinIfValid(x, SIGMA_XMIN)) goto endMin;

    // ymin
    if(!ignoreMinIfValid(y, SIGMA_YMIN)) goto endMin;

    endMin:

    // n
    realCopy(const_1,&tmpReal1);
    if(!realSubtractIfValid(SIGMA_N, &tmpReal1, SIGMA_N, realContext)) goto toReturn;

    // sigma x
    if(!realSubtractIfValid(SIGMA_X, x, SIGMA_X, realContext)) goto toReturn;

    // sigma y
    if(!realSubtractIfValid(SIGMA_Y, y, SIGMA_Y, realContext)) goto toReturn;

    // sigma x²
    realMultiply(x, x, &tmpReal1, realContext);
    if(!realSubtractIfValid(SIGMA_X2, &tmpReal1, SIGMA_X2, realContext)) goto toReturn;

    // sigma x³
    realMultiply(&tmpReal1, x, &tmpReal2, realContext);
    if(!realSubtractIfValid(SIGMA_X3, &tmpReal2, SIGMA_X3, realContext)) goto toReturn;

    // sigma x⁴
    realMultiply(&tmpReal2, x, &tmpReal2, realContext);
    if(!realSubtractIfValid(SIGMA_X4, &tmpReal2, SIGMA_X4, realContext)) goto toReturn;

    // sigma x²y
    realMultiply(&tmpReal1, y, &tmpReal2, realContext);
    if(!realSubtractIfValid(SIGMA_X2Y, &tmpReal2, SIGMA_X2Y, realContext)) goto toReturn;

    // sigma x²/y
    realDivide(&tmpReal1, y, &tmpReal2, realContext);
    if(!realSubtractIfValid(SIGMA_X2onY, &tmpReal2, SIGMA_X2onY, realContext)) goto toReturn;

    // sigma 1/x²
    realDivide(const_1, &tmpReal1, &tmpReal2, realContext);
    if(!realSubtractIfValid(SIGMA_1onX2, &tmpReal2, SIGMA_1onX2, realContext)) goto toReturn;

    // sigma y²
    realMultiply(y, y, &tmpReal1, realContext);
    if(!realSubtractIfValid(SIGMA_Y2, &tmpReal1, SIGMA_Y2, realContext)) goto toReturn;

    // sigma 1/y²
    realDivide(const_1, &tmpReal1, &tmpReal2, realContext);
    if(!realSubtractIfValid(SIGMA_1onY2, &tmpReal2, SIGMA_1onY2, realContext)) goto toReturn;

    // sigma xy
    realMultiply(x, y, &tmpReal1, realContext);
    if(!realSubtractIfValid(SIGMA_XY, &tmpReal1, SIGMA_XY, realContext)) goto toReturn;

    // sigma ln(x)
    WP34S_Ln(x, &tmpReal1, realContext);
    realCopy(&tmpReal1 ,&tmpReal3);
    if(!realSubtractIfValid(SIGMA_lnX, &tmpReal1, SIGMA_lnX, realContext)) goto toReturn;

    // sigma ln²(x)
    realMultiply(&tmpReal1, &tmpReal1, &tmpReal2, realContext);
    if(!realSubtractIfValid(SIGMA_ln2X, &tmpReal2, SIGMA_ln2X, realContext)) goto toReturn;

    // sigma yln(x)
    realMultiply(&tmpReal1, y, &tmpReal1, realContext);
    if(!realSubtractIfValid(SIGMA_YlnX, &tmpReal1, SIGMA_YlnX, realContext)) goto toReturn;

    // sigma ln(y)
    WP34S_Ln(y, &tmpReal1, realContext);
    if(!realSubtractIfValid(SIGMA_lnY, &tmpReal1, SIGMA_lnY, realContext)) goto toReturn;

    // sigma ln(x)×ln(y)
    realMultiply(&tmpReal3, &tmpReal1, &tmpReal3, realContext);
    if(!realSubtractIfValid(SIGMA_lnXlnY, &tmpReal3, SIGMA_lnXlnY, realContext)) goto toReturn;

    // sigma ln(y)/x
    realDivide(&tmpReal1, x, &tmpReal2, realContext);
    if(!realSubtractIfValid(SIGMA_lnYonX, &tmpReal2, SIGMA_lnYonX, realContext)) goto toReturn;

    // sigma ln²(y)
    realMultiply(&tmpReal1, &tmpReal1, &tmpReal2, realContext);
    if(!realSubtractIfValid(SIGMA_ln2Y, &tmpReal2, SIGMA_ln2Y, realContext)) goto toReturn;

    // sigma xln(y)
    realMultiply(&tmpReal1, x, &tmpReal1, realContext);
    if(!realSubtractIfValid(SIGMA_XlnY, &tmpReal1, SIGMA_XlnY, realContext)) goto toReturn;

    // sigma x²ln(y)
    realMultiply(&tmpReal1, x, &tmpReal1, realContext);
    if(!realSubtractIfValid(SIGMA_X2lnY, &tmpReal1, SIGMA_X2lnY, realContext)) goto toReturn;

    // sigma 1/x
    realDivide(const_1, x, &tmpReal1, realContext);
    if(!realSubtractIfValid(SIGMA_1onX, &tmpReal1, SIGMA_1onX, realContext)) goto toReturn;

    // sigma x/y
    realDivide(x, y, &tmpReal1, realContext);
    if(!realSubtractIfValid(SIGMA_XonY, &tmpReal1, SIGMA_XonY, realContext)) goto toReturn;

    // sigma 1/y
    realDivide(const_1, y, &tmpReal1, realContext);
    if(!realSubtractIfValid(SIGMA_1onY, &tmpReal1, SIGMA_1onY, realContext)) goto toReturn;

    toReturn:

    return;
  }
#endif // !TESTSUITE_|BUILD



bool_t checkMinimumDataPoints(const real_t *n) {
  if(statisticalSumsPointer == NULL) {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function checkMinimumDataPoints:", "There is no statistical data available!", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }

  if(realCompareLessThan(SIGMA_N, n)) {
    displayCalcErrorMessage(ERROR_TOO_FEW_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function checkMinimumDataPoints:", "There is insufficient statistical data available!", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  return true;
}


static void clearStatisticalSums(void) {
  if(statisticalSumsPointer) {
    for(int32_t sum=0; sum<NUMBER_OF_STATISTICAL_SUMS - 4; sum++) {
      realZero((real_t *)(statisticalSumsPointer + REAL_SIZE * sum));
    }
    realCopy(const_plusInfinity,  SIGMA_XMIN);
    realCopy(const_plusInfinity,  SIGMA_YMIN);
    realCopy(const_minusInfinity, SIGMA_XMAX);
    realCopy(const_minusInfinity, SIGMA_YMAX);
  }
}


void initStatisticalSums(void) {
  if(statisticalSumsPointer == NULL) {
    statisticalSumsPointer = allocWp43s(NUMBER_OF_STATISTICAL_SUMS * REAL_SIZE);
    clearStatisticalSums();
    }
  else {
      displayCalcErrorMessage(ERROR_RAM_FULL, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    }
  }



#if !defined(TESTSUITE_BUILD)
  static void calcMax(uint16_t maxOffset) {
    realCopy(const_minusInfinity, SIGMA_XMAX);
    realCopy(const_minusInfinity, SIGMA_YMAX);

    calcRegister_t regStats = findNamedVariable(statMx);
    if(regStats != INVALID_VARIABLE) {
      real34Matrix_t stats;
      linkToRealMatrixRegister(regStats, &stats);
      const uint16_t rows = stats.header.matrixRows, cols = stats.header.matrixColumns;

      real_t x, y;
      for(uint16_t i = 0; i < rows - maxOffset; i++) {
        real34ToReal(&stats.matrixElements[i * cols    ], &x);
        real34ToReal(&stats.matrixElements[i * cols + 1], &y);
        addMax(&x, &y);
      }
    }
  }


  static void calcMin(uint16_t maxOffset) {
    realCopy(const_plusInfinity,  SIGMA_XMIN);
    realCopy(const_plusInfinity,  SIGMA_YMIN);

    calcRegister_t regStats = findNamedVariable(statMx);
    if(regStats != INVALID_VARIABLE) {
      real34Matrix_t stats;
      linkToRealMatrixRegister(regStats, &stats);
      const uint16_t rows = stats.header.matrixRows, cols = stats.header.matrixColumns;
      real_t x, y;
      for(uint16_t i = 0; i < rows - maxOffset; i++) {
        real34ToReal(&stats.matrixElements[i * cols    ], &x);
        real34ToReal(&stats.matrixElements[i * cols + 1], &y);
        addMin(&x, &y);
      }
    }
  }
#endif // !TESTSUITE_BUILD



void calcSigma(uint16_t maxOffset) {
  #if !defined(TESTSUITE_BUILD)
    clearStatisticalSums();
    if(!statisticalSumsPointer) {
      initStatisticalSums();
    }
    calcRegister_t regStats = findNamedVariable(statMx);
    if(regStats != INVALID_VARIABLE) {
      real34Matrix_t stats;
      linkToRealMatrixRegister(regStats, &stats);
      const uint16_t rows = stats.header.matrixRows, cols = stats.header.matrixColumns;
      real_t x, y;
      for(uint16_t i = 0; i < rows - maxOffset; i++) {
        real34ToReal(&stats.matrixElements[i * cols    ], &x);
        real34ToReal(&stats.matrixElements[i * cols + 1], &y);
        addSigma(&x, &y);
      }
    }
  #endif //TESTSUITE_BUILD
}


#if !defined(TESTSUITE_BUILD)
static void getLastRowStatsMatrix(real_t *x, real_t *y) {
  uint16_t rows = 0, cols;
  calcRegister_t regStats = findNamedVariable(statMx);

  if(regStats != INVALID_VARIABLE) {
    real34Matrix_t stats;
    linkToRealMatrixRegister(regStats, &stats);
    rows = stats.header.matrixRows;
    cols = stats.header.matrixColumns;
    real34ToReal(&stats.matrixElements[(rows-1) * cols    ], x);
    real34ToReal(&stats.matrixElements[(rows-1) * cols + 1], y);
  }
  else {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "STATS matrix not found");
      moreInfoOnError("In function getLastRowStatsMatrix:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}



  static void AddtoStatsMatrix(real_t *x, real_t *y) {
    uint16_t rows = 0, cols;
    calcRegister_t regStats = findNamedVariable(statMx);
    if(!isStatsMatrix(&rows,"STATS")) {
      regStats = allocateNamedMatrix("STATS", 1, 2);
      real34Matrix_t stats;
      linkToRealMatrixRegister(regStats, &stats);
      realMatrixInit(&stats,1,2);
    }
    else {
      if(appendRowAtMatrixRegister(regStats)) {
      }
      else {
        regStats = INVALID_VARIABLE;
      }
    }
    if(regStats != INVALID_VARIABLE) {
      real34Matrix_t stats;
      linkToRealMatrixRegister(regStats, &stats);
      rows = stats.header.matrixRows;
      cols = stats.header.matrixColumns;
      realToReal34(x, &stats.matrixElements[(rows-1) * cols    ]);
      realToReal34(y, &stats.matrixElements[(rows-1) * cols + 1]);
    }
    else {
      displayCalcErrorMessage(ERROR_NOT_ENOUGH_MEMORY_FOR_NEW_MATRIX, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "additional matrix line not added; rows = %i",rows);
        moreInfoOnError("In function AddtoStatsMatrix:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }



  static void removeLastRowFromStatsMatrix(void) {
    uint16_t rows = 0;
    if(!isStatsMatrix(&rows,"STATS")) {
      displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "no STATS matrix");
        moreInfoOnError("In function removeLastRowFromStatsMatrix:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }

    calcRegister_t regStats = findNamedVariable(statMx);
    if(rows <= 1) {
      fnClSigma(0);
      return;
    }
    else {
      if(!redimMatrixRegister(regStats, rows - 1, 2)) {
        regStats = INVALID_VARIABLE;
      }
    }
    if(regStats == INVALID_VARIABLE) {
      displayCalcErrorMessage(ERROR_NOT_ENOUGH_MEMORY_FOR_NEW_MATRIX, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "matrix/line not removed");
        moreInfoOnError("In function removeFromStatsMatrix:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }
#endif // !TESTSUITE_BUILD


void fnClSigma(uint16_t unusedButMandatoryParameter) {
  calcRegister_t regStats = findNamedVariable("HISTO");
  if(regStats == INVALID_VARIABLE) {
    allocateNamedVariable("HISTO", dtReal34, REAL34_SIZE);
    regStats = findNamedVariable("HISTO");
  }

  if(regStats == INVALID_VARIABLE) {
    displayCalcErrorMessage(ERROR_NO_MATRIX_INDEXED, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "HISTO matrix not created");
      moreInfoOnError("In function fnClSigma:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }

  clearRegister(regStats);                  // this should change to delete the named variable HISTO once the delete function is available. Until then write 0.0 into HISTO.

  regStats = findNamedVariable(statMx);
  if(regStats == INVALID_VARIABLE) {
    allocateNamedVariable("STATS", dtReal34, REAL34_SIZE);
    regStats = findNamedVariable(statMx);
  }

  if(regStats == INVALID_VARIABLE) {
    displayCalcErrorMessage(ERROR_NO_MATRIX_INDEXED, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "STATS matrix not created");
      moreInfoOnError("In function fnClSigma:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }


  clearRegister(regStats);                  // this should change to delete the named variable STATS once the delete function is available. Until then write 0.0 into STATS.
  lrChosen = 0;                             // linear regression selection
  lastPlotMode = PLOT_NOTHING;              // last selected  plotmode
  plotSelection = 0;                        // Currently selected linear regression mode
  PLOT_ZOOM = 0;                            // Currently selected plot zoom level
  drawHistogram = 0;
 
  if(statisticalSumsPointer != NULL) {
    freeWp43s(statisticalSumsPointer, NUMBER_OF_STATISTICAL_SUMS * REAL_SIZE);
    statisticalSumsPointer = NULL;
  }
}



void fnSigma(uint16_t plusMinus) {
  #if !defined(TESTSUITE_BUILD)
    real_t x, y;
    realContext_t *realContext = &ctxtReal75; // Summation data with 75 digits

  lrChosen = 0;

  if(plusMinus == 1) { // SIGMA+
    if(   (getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtReal34)
       && (getRegisterDataType(REGISTER_Y) == dtLongInteger || getRegisterDataType(REGISTER_Y) == dtReal34)) {

        if(statisticalSumsPointer == NULL) {
          initStatisticalSums();
          if(lastErrorCode != ERROR_NONE) {
            return;
          }
        }

      if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
        convertLongIntegerRegisterToReal(REGISTER_X, &x, realContext);
        convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      }
      else {
        real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    }

      if(getRegisterDataType(REGISTER_Y) == dtLongInteger) {
        convertLongIntegerRegisterToReal(REGISTER_Y, &y, realContext);
        convertLongIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
      }
      else {
        real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
      }

        addSigma(&x, &y);
        AddtoStatsMatrix(&x, &y);
        realCopy(&x,      &SAVED_SIGMA_LASTX);
        realCopy(&y,      &SAVED_SIGMA_LASTY);
        SAVED_SIGMA_LAct = +1;

        #if defined(DEBUGUNDO)
          calcRegister_t regStats = findNamedVariable(statMx);
          printRegisterToConsole(regStats,"From: AddtoStatsMatrix STATS:\n","\n");
        #endif //DEBUGUNDO

        temporaryInformation = TI_STATISTIC_SUMS;
      }
      else if(getRegisterDataType(REGISTER_X) == dtReal34Matrix && plusMinus == 1) {
        real34Matrix_t matrix;
        linkToRealMatrixRegister(REGISTER_X, &matrix);

        if(matrix.header.matrixColumns == 2) {
          if(statisticalSumsPointer == NULL) {
            initStatisticalSums();
            if(lastErrorCode != ERROR_NONE) {
              return;
            }
          }

          if(!saveLastX()) {
            return;
          }
          for(uint16_t i = 0; i < matrix.header.matrixRows; ++i) {
            real34ToReal(&matrix.matrixElements[i * 2    ], &y);
            real34ToReal(&matrix.matrixElements[i * 2 + 1], &x);
            addSigma(&x, &y);
            AddtoStatsMatrix(&x, &y);
          }

          liftStack();
          reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);
          convertRealToReal34ResultRegister(&y, REGISTER_Y);
          reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
          convertRealToReal34ResultRegister(&x, REGISTER_X);
          temporaryInformation = TI_STATISTIC_SUMS;
        }
        else {
          displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(errorMessage, "cannot use %" PRIu16 STD_CROSS "%" PRId16 "-matrix as statistical data!", matrix.header.matrixRows, matrix.header.matrixColumns);
            moreInfoOnError("In function fnSigma:", errorMessage, NULL, NULL);
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        }
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "cannot use (%s, %s) as statistical data!", getDataTypeName(getRegisterDataType(REGISTER_X), false, false), getDataTypeName(getRegisterDataType(REGISTER_Y), false, false));
          moreInfoOnError("In function fnSigma:", errorMessage, NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      }
    }
    else { // SIGMA-
      if(checkMinimumDataPoints(const_1)) {
        getLastRowStatsMatrix(&x, &y);
        subSigma(&x, &y);
        removeLastRowFromStatsMatrix();

        if(statisticalSumsPointer != NULL) {
          temporaryInformation = TI_STATISTIC_SUMS;
        }
        liftStack();
        setSystemFlag(FLAG_ASLIFT);
        liftStack();
        convertRealToReal34ResultRegister(&x, REGISTER_X);
        convertRealToReal34ResultRegister(&y, REGISTER_Y);

        realCopy(&x,       &SAVED_SIGMA_LASTX);
        realCopy(&y,       &SAVED_SIGMA_LASTY);
        SAVED_SIGMA_LAct = -1;

        #if defined(DEBUGUNDO)
          if(statisticalSumsPointer != NULL) {
            calcRegister_t regStats = findNamedVariable(statMx);
            //printRealToConsole(SIGMA_N,"   >>> After\n   >>>   SIGMA_N:","\n");
            //printRealToConsole(SIGMA_XMAX,"   >>>   SIGMA_MaxX:","\n");
            //printRegisterToConsole(regStats,"From Sigma-: STATS\n","\n");
          }
        #endif //DEBUGUNDO
      }
    }
  #endif // TESTSUITE_BUILD
}



void fnStatSum(uint16_t sum) {
  if(checkMinimumDataPoints(const_1)) {
    liftStack();
    realToReal34((real_t *)(statisticalSumsPointer + REAL_SIZE * sum), REGISTER_REAL34_DATA(REGISTER_X));
  }
}



void fnSumXY(uint16_t unusedButMandatoryParameter) {
  if(checkMinimumDataPoints(const_1)) {
    liftStack();
    setSystemFlag(FLAG_ASLIFT);
    liftStack();

    convertRealToReal34ResultRegister(SIGMA_X, REGISTER_X);
    convertRealToReal34ResultRegister(SIGMA_Y, REGISTER_Y);

    temporaryInformation = TI_SUMX_SUMY;
  }
}



void fnXmin(uint16_t unusedButMandatoryParameter) {
  if(checkMinimumDataPoints(const_1)) {
    liftStack();
    setSystemFlag(FLAG_ASLIFT);
    liftStack();

    convertRealToReal34ResultRegister(SIGMA_XMIN, REGISTER_X);
    convertRealToReal34ResultRegister(SIGMA_YMIN, REGISTER_Y);

    temporaryInformation = TI_XMIN_YMIN;
  }
}



void fnXmax(uint16_t unusedButMandatoryParameter) {
  if(checkMinimumDataPoints(const_1)) {
    liftStack();
    setSystemFlag(FLAG_ASLIFT);
    liftStack();

    convertRealToReal34ResultRegister(SIGMA_XMAX, REGISTER_X);
    convertRealToReal34ResultRegister(SIGMA_YMAX, REGISTER_Y);

    temporaryInformation = TI_XMAX_YMAX;
  }
}


//----------- Histogram Section -----------------


#if !defined(TESTSUITE_BUILD)
  static bool_t isHistoMatrix(uint16_t *rows, char *mx) {
    *rows = 0;
    calcRegister_t regHisto = findNamedVariable(mx);
    if(regHisto == INVALID_VARIABLE) {
      return false;
    }
    else {   
      if(getRegisterDataType(regHisto) != dtReal34Matrix) {
        return false;} 
      else
      {
        real34Matrix_t histo;
        linkToRealMatrixRegister(regHisto, &histo);
        *rows = histo.header.matrixRows;
        if(histo.header.matrixColumns != 2) {
          return false;
        }
      }
    }
    return true;
  }


  static void initHistoMatrix(real_t *s) {
    uint16_t rows = 0, cols;
    calcRegister_t regHisto = findNamedVariable("HISTO");
    if(!isHistoMatrix(&rows,"HISTO")) {
      regHisto = allocateNamedMatrix("HISTO", 1, 2);
      real34Matrix_t histo;
      linkToRealMatrixRegister(regHisto, &histo);
      realMatrixInit(&histo, 1, 2);
    }
    else {
      if(appendRowAtMatrixRegister(regHisto)) {
      }
      else {
        regHisto = INVALID_VARIABLE;
      }
    }
    if(regHisto != INVALID_VARIABLE) {
      real34Matrix_t histo;
      linkToRealMatrixRegister(regHisto, &histo);
      rows = histo.header.matrixRows;
      cols = histo.header.matrixColumns;
      realToReal34(s,       &histo.matrixElements[(rows-1) * cols    ]);
      realToReal34(const_0, &histo.matrixElements[(rows-1) * cols + 1]);
      //printf(">>>>> rows=%d  cols=%d  ",rows, cols);
      //printReal34ToConsole(&histo.matrixElements[(rows-1) * cols    ],"X34:","  ");
      //printReal34ToConsole(&histo.matrixElements[(rows-1) * cols +1 ],"Y34:","  \n");

    }
    else {
      displayCalcErrorMessage(ERROR_NOT_ENOUGH_MEMORY_FOR_NEW_MATRIX, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "additional matrix line not added; rows = %i",rows);
        moreInfoOnError("In function initHistoMatrix:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }
#endif //!defined(TESTSUITE_BUILD)


void fnSetLoBin(uint16_t unusedButMandatoryParameter) {
  #if !defined(TESTSUITE_BUILD)
    if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
      convertLongIntegerRegisterToReal34(REGISTER_X, &loBinR);
    } else {
      if(getRegisterDataType(REGISTER_X) == dtReal34) {
        real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &loBinR);
      } else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
        #ifdef PC_BUILD
        sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
        moreInfoOnError("In function fnSetLoBin:", errorMessage, "is not a long integer or real.", "");
        #endif
      }
    }
  #endif //!defined(TESTSUITE_BUILD)
}


void fnSetHiBin(uint16_t unusedButMandatoryParameter) {
  #if !defined(TESTSUITE_BUILD)
    if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
      convertLongIntegerRegisterToReal34(REGISTER_X, &hiBinR);
    } 
    else {
      if(getRegisterDataType(REGISTER_X) == dtReal34) {
        real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &hiBinR);
      } else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
        #ifdef PC_BUILD
        sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
        moreInfoOnError("In function fnSetHiBin:", errorMessage, "is not a long integer or real.", "");
        #endif
      }
    }
  #endif //!defined(TESTSUITE_BUILD)
}


void fnSetNBins(uint16_t unusedButMandatoryParameter) {
  #if !defined(TESTSUITE_BUILD)
    if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
      convertLongIntegerRegisterToReal34(REGISTER_X, &nBins);
    } else {
      if(getRegisterDataType(REGISTER_X) == dtReal34) {
        real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &nBins);
      } else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
        #ifdef PC_BUILD
        sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
        moreInfoOnError("In function fnSetNBins:", errorMessage, "is not a long integer or real.", "");
        #endif
      }
    }
  #endif //!defined(TESTSUITE_BUILD)
}


#if !defined(TESTSUITE_BUILD)
  static calcRegister_t clHisto(void){
    calcRegister_t regHisto = findNamedVariable("HISTO");
    if(regHisto == INVALID_VARIABLE) {
      allocateNamedVariable("HISTO", dtReal34, REAL34_SIZE);
      regHisto = findNamedVariable("HISTO");
    }

    if(regHisto == INVALID_VARIABLE) {
      displayCalcErrorMessage(ERROR_NO_MATRIX_INDEXED, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "HISTO matrix not created");
        moreInfoOnError("In function fnConvertStatsToHisto:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return INVALID_VARIABLE;
    }
    clearRegister(regHisto);                  // this should change to delete the named variable HISTO once the delete function is available. Until then write 0.0 into STATS.
    return regHisto;
  }
#endif //TESTSUITE_BUILD



//Temporary
//TODO JM
//TODO check if clearRegister can work if invalid? !!!
//TODO use above clHisto function from clSigma



void fnConvertStatsToHisto(uint16_t statsVariableToHistogram) {
  #if !defined(TESTSUITE_BUILD)
    real_t ii, lb, hb, nb, nn, bw, bwon2;

    if (!checkMinimumDataPoints(const_3)) return;


    if(statsVariableToHistogram == ITM_Y) {
      realCopy(SIGMA_YMIN, &lb);
      realCopy(SIGMA_YMAX, &hb);
    } else if(statsVariableToHistogram == ITM_X) {
      realCopy(SIGMA_XMIN, &lb);
      realCopy(SIGMA_XMAX, &hb);
    } else return;


    realCopy(SIGMA_N, &nn);
    realSquareRoot(&nn,&nb,&ctxtReal39);
    realToIntegralValue(&nb, &nb, DEC_ROUND_CEILING, &ctxtReal39);

    realToReal34(&lb, &loBinR);             //set up the user variables
    realToReal34(&hb, &hiBinR);             //set up the user variables
    realToReal34(&nb, &nBins);              //set up the user variables

    int32_t n = real34ToInt32(&nBins);
    realSubtract(&hb, &lb, &bw, &ctxtReal39); //calculate bin width
    realDivide(&bw, &nb, &bw, &ctxtReal39);
    realDivide(&bw, const_2, &bwon2, &ctxtReal39);

    calcRegister_t regStats = findNamedVariable("STATS");
    calcRegister_t regHisto =   clHisto();

    if(regStats != INVALID_VARIABLE && regHisto != INVALID_VARIABLE) {
      real34Matrix_t stats;
      real34Matrix_t histo;
      linkToRealMatrixRegister(regStats, &stats);
      linkToRealMatrixRegister(regHisto, &histo);
      const uint16_t rows = stats.header.matrixRows, cols = stats.header.matrixColumns;
      if(cols == 2) {

        for(uint16_t i = 0; i < n; i++) {
          int32ToReal(i, &ii);
          realAdd(&ii, const_1on2, &ii, &ctxtReal39);
          realMultiply(&ii, &bw, &ii, &ctxtReal39);
          realAdd(&ii, &lb, &ii, &ctxtReal39);           //midpoint
          //printRealToConsole(&ii,"midpoint \n","");
          initHistoMatrix(&ii);                            // Set up all x-mid-points of the bins in HISTO, with 0 in y
          //printReal34ToConsole(&histo.matrixElements[(i) * cols    ],"XX34:","  ");
          //printReal34ToConsole(&histo.matrixElements[(i) * cols +1 ],"YY34:","  \n");
        }

        uint16_t i = 0;
        if(isStatsMatrix(&i, "STATS") && isHistoMatrix(&i, "HISTO")) {
          for(uint16_t i = 0; i < rows - 1; i++) {
            //printf("n=%d ^^^^ i=%d ",n,i);
            for(uint16_t j = 0; j < n; j++) {
              //printf("^ j=%d\n",j);
              real_t t, tl, th;
              real34ToReal(&stats.matrixElements[i * cols + 1], &t);  //from Y
              real34ToReal(&histo.matrixElements[j * cols    ], &tl); //get the bin mid x

  //temporary
  //TODO JM
  #if defined (PC_BUILD)
  printReal34ToConsole(&histo.matrixElements[j * cols    ],"Xrcl34:","  \n");
  printRealToConsole(&tl,"xRCL:","  ");
  #endif // PC_BUILD

              realSubtract(&tl, &bwon2, &tl, &ctxtReal39);            //get the bin x low
              realAdd     (&tl, &bw   , &th, &ctxtReal39);            //get the bin x hi

  //temporary
  //TODO JM
  #if defined (PC_BUILD)
  printRealToConsole(&tl,"low:","  ");
  printRealToConsole(&t,"t:","  ");
  printRealToConsole(&th,"hi:","\n");
  #endif // PC_BUILD

              if(realCompareLessThan(&t, &th) && realCompareGreaterEqual(&t, &tl)) {
                //printf("Add\n");
                real34Add(&histo.matrixElements[j * cols + 1], const34_1, &histo.matrixElements[j * cols + 1]);
                break;
              }
            }
          }
        }
      }
      else {
        #ifdef PC_BUILD
          #ifdef VERBOSE_SOLVER00
          printf("ERROR in execute_rpn_function; STATS Matrix columns not right: %u\n",lastErrorCode);
          #endif //VERBOSE_SOLVER1
          lastErrorCode = 0;
          return;
        #endif //PC_BUILD
      }
    }
    else {
      #ifdef PC_BUILD
        #ifdef VERBOSE_SOLVER00
        printf("ERROR in execute_rpn_function; invalid variable: %u\n",lastErrorCode);
        #endif //VERBOSE_SOLVER1
        lastErrorCode = 0;
        return;
      #endif //PC_BUILD
    }
    liftStack();
    liftStack();
    liftStack();
    reallocateRegister(REGISTER_Z, dtReal34, REAL34_SIZE, amNone);
    convertRealToReal34ResultRegister(&nb, REGISTER_Z);
    reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);
    convertRealToReal34ResultRegister(&lb, REGISTER_Y);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    convertRealToReal34ResultRegister(&hb, REGISTER_X);
    temporaryInformation = TI_STATISTIC_HISTO;
  #endif //TESTSUITE_BUILD
}
