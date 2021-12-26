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
#include "mathematics/comparisonReals.h"
#include "mathematics/wp34s.h"
#include "matrix.h"
#include "memory.h"
#include "plotstat.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "stack.h"

#include "wp43s.h"


#ifndef TESTSUITE_BUILD //TESTSUITE_BUILD
  static void calcMax(uint16_t maxOffset);
  static void calcMin(uint16_t maxOffset);
#endif


#ifndef TESTSUITE_BUILD
static bool_t isStatsMatrix(uint16_t *rows) {
  *rows = 0;
  calcRegister_t regStats = findNamedVariable("STATS");
  if(regStats == INVALID_VARIABLE) return false; else {
    if(getRegisterDataType(regStats) != dtReal34Matrix) return false; else {
      real34Matrix_t stats;
      linkToRealMatrixRegister(regStats, &stats);
      *rows = stats.header.matrixRows;
      if (stats.header.matrixColumns != 2) return false;
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
  if(realIsNaN (r1) || realIsNaN (r2) || realIsInfinite (r1) || realIsInfinite (r2) || !realCompareEqual(r1, r2)) {
    calcMax(1);
    return false;
  }
  return true;
}

static bool_t ignoreMinIfValid(real_t *r1, real_t *r2){
  if(realIsNaN (r1) || realIsNaN (r2) || realIsInfinite (r1) || realIsInfinite (r2) || !realCompareEqual(r1, r2)) {
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


#endif //TESTSUITE_|BUILD





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



#ifndef TESTSUITE_BUILD
static void calcMax(uint16_t maxOffset) {

  realCopy(const_minusInfinity, SIGMA_XMAX);
  realCopy(const_minusInfinity, SIGMA_YMAX);

  calcRegister_t regStats = findNamedVariable("STATS");
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

  calcRegister_t regStats = findNamedVariable("STATS");
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
#endif //TESTSUITE_BUILD



void calcSigma(uint16_t maxOffset) {
#ifndef TESTSUITE_BUILD
  clearStatisticalSums();
  if(!statisticalSumsPointer) initStatisticalSums();
  calcRegister_t regStats = findNamedVariable("STATS");
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


#ifndef TESTSUITE_BUILD
static void getLastRowStatsMatrix(real_t *x, real_t *y) {
  uint16_t rows = 0, cols;
  calcRegister_t regStats = findNamedVariable("STATS");

  if(regStats != INVALID_VARIABLE) {
    real34Matrix_t stats;
    linkToRealMatrixRegister(regStats, &stats);
    rows = stats.header.matrixRows;
    cols = stats.header.matrixColumns;
    real34ToReal(&stats.matrixElements[(rows-1) * cols    ], x);
    real34ToReal(&stats.matrixElements[(rows-1) * cols + 1], y);
#ifdef PC_BUILD
printRealToConsole(x,"   x-element:",", ");
printRealToConsole(y,"   y-element:","\n");
#endif
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
  calcRegister_t regStats = findNamedVariable("STATS");
  if(!isStatsMatrix(&rows)) {
    regStats = allocateNamedMatrix("STATS", 1, 2);
  }
  else {
    if(appendRowAtMatrixRegister(regStats)) {
      regStats = findNamedVariable("STATS");
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
      sprintf(errorMessage, "additional matrix line not added");
      moreInfoOnError("In function AddtoStatsMatrix:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}



static void removeLastRowFromStatsMatrix(void) {
  uint16_t rows = 0;
  if(!isStatsMatrix(&rows)) {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "no STATS matrix");
      moreInfoOnError("In function removeLastRowFromStatsMatrix:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  calcRegister_t regStats = findNamedVariable("STATS");
  if(rows <= 1) {
    fnClSigma(0); 
    return;
  }
  else {
    if(!redimMatrixRegister(regStats, rows - 1, 2)) 
      regStats = INVALID_VARIABLE; 
  }
  if(regStats == INVALID_VARIABLE) {
    displayCalcErrorMessage(ERROR_NOT_ENOUGH_MEMORY_FOR_NEW_MATRIX, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "matrix/line not removed");
      moreInfoOnError("In function removeFromStatsMatrix:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}
#endif //TESTSUITE_BUILD


void fnClSigma(uint16_t unusedButMandatoryParameter) {
  calcRegister_t regStats = findNamedVariable("STATS");
  if(regStats == INVALID_VARIABLE) {
    allocateNamedVariable("STATS", dtReal34, REAL34_SIZE);
    regStats = findNamedVariable("STATS");
  }
  clearRegister(regStats);                  // this should change to delete the named variable STATS once the delete function is available. Until then write 0.0 into STATS.
  lrChosen = 0;                             // linear regression selection
  lastPlotMode = PLOT_NOTHING;              // last selected  plotmode
  plotSelection = 0;                        // Currently selected linear regression mode
  PLOT_ZOOM = 0;                            // Currently selected plot zoom level
  if(statisticalSumsPointer != NULL) {
    freeWp43s(statisticalSumsPointer, NUMBER_OF_STATISTICAL_SUMS * REAL_SIZE);
    statisticalSumsPointer = NULL;
  }
}



void fnSigma(uint16_t plusMinus) {
#ifndef TESTSUITE_BUILD
  real_t x, y;
  realContext_t *realContext = &ctxtReal75; // Summation data with 75 digits

  lrChosen = 0;

  if(plusMinus == 1) { // SIGMA+

    if(   (getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtReal34)
       && (getRegisterDataType(REGISTER_Y) == dtLongInteger || getRegisterDataType(REGISTER_Y) == dtReal34)) {

      if(statisticalSumsPointer == NULL) {
        initStatisticalSums();
        if(lastErrorCode != ERROR_NONE) return;
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

      temporaryInformation = TI_STATISTIC_SUMS;
    }
    else if(getRegisterDataType(REGISTER_X) == dtReal34Matrix && plusMinus == 1) {
      real34Matrix_t matrix;
      linkToRealMatrixRegister(REGISTER_X, &matrix);

    if(matrix.header.matrixColumns == 2) {
      if(statisticalSumsPointer == NULL) {
        initStatisticalSums();
        if(lastErrorCode != ERROR_NONE) return;
      }

        if(!saveLastX()) return;
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
  } else 


  { // SIGMA-
    getLastRowStatsMatrix(&x, &y);
    subSigma(&x, &y);
    removeLastRowFromStatsMatrix();

    if(statisticalSumsPointer != NULL) temporaryInformation = TI_STATISTIC_SUMS;

    realCopy(&x,       &SAVED_SIGMA_LASTX);
    realCopy(&y,       &SAVED_SIGMA_LASTY);
    SAVED_SIGMA_LAct = -1;
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
