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
 * \file stats.c Statistical functions
 ***********************************************/

#include "stats.h"

#include "constantPointers.h"
#include "debug.h"
#include "error.h"
#include "flags.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/wp34s.h"
#include "memory.h"
#include "plotstat.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "stack.h"

#include "wp43s.h"


/********************************************//**
 * \brief Verifies that the statistical registers are allocated and that there are enough data
 * An appropriate error message is displayed if either condition fails
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return bool_t
 ***********************************************/
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


void initStatisticalSums(void) {
  if(statisticalSumsPointer == NULL) {
    statisticalSumsPointer = allocWp43s(NUMBER_OF_STATISTICAL_SUMS * REAL_SIZE);
    for(int32_t sum=0; sum<NUMBER_OF_STATISTICAL_SUMS - 4; sum++) {
      realZero((real_t *)(statisticalSumsPointer + REAL_SIZE * sum));
    }

    realCopy(const_plusInfinity,  SIGMA_XMIN);
    realCopy(const_plusInfinity,  SIGMA_YMIN);
    realCopy(const_minusInfinity, SIGMA_XMAX);
    realCopy(const_minusInfinity, SIGMA_YMAX);
  }
  if(telltale != MEM_INITIALIZED) {
    graph_setupmemory();
  }
}



void fnClSigma(uint16_t unusedButMandatoryParameter) {
  if(statisticalSumsPointer != NULL) {
    freeWp43s(statisticalSumsPointer, NUMBER_OF_STATISTICAL_SUMS * REAL_SIZE);
    statisticalSumsPointer = NULL;
    graph_end();                            // release plot memory
    lrSelection = CF_LINEAR_FITTING;        // linear regression selection
    lrChosen = 0;                           // linear regression selection
    lastPlotMode = PLOT_NOTHING;            // last selected  plotmode
    plotSelection = 0;                      // Currently selected linear regression mode
    PLOT_ZOOM = 0;                          // Currently selected plot zoom level
  }
}



/********************************************//**
 * \brief Adds a value to the statistic registers
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnSigma(uint16_t plusMinus) {
  real_t tmpReal1, tmpReal2, tmpReal3, x, y;
  realContext_t *realContext = &ctxtReal75; // Summation data with 75 digits

  if(   (getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtReal34)
     && (getRegisterDataType(REGISTER_Y) == dtLongInteger || getRegisterDataType(REGISTER_Y) == dtReal34)) {
    if(statisticalSumsPointer == NULL) {
      initStatisticalSums();
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

    if(plusMinus == 1) { // SIGMA+
      // xmin
      if(realCompareLessThan(&x, SIGMA_XMIN)) {
        realCopy(&x, SIGMA_XMIN);
      }

      // xmax
      if(realCompareGreaterThan(&x, SIGMA_XMAX)) {
        realCopy(&x, SIGMA_XMAX);
      }

      // ymin
      if(realCompareLessThan(&y, SIGMA_YMIN)) {
        realCopy(&y, SIGMA_YMIN);
      }

      // ymax
      if(realCompareGreaterThan(&y, SIGMA_YMAX)) {
        realCopy(&y, SIGMA_YMAX);
      }

      // n
      realAdd(SIGMA_N, const_1, SIGMA_N, realContext);

      // sigma x
      realAdd(SIGMA_X, &x, SIGMA_X, realContext);

      // sigma y
      realAdd(SIGMA_Y, &y, SIGMA_Y, realContext);

      // sigma x²
      realMultiply(&x, &x, &tmpReal1, realContext);
      realAdd(SIGMA_X2, &tmpReal1, SIGMA_X2, realContext);

      // sigma x³
      realMultiply(&tmpReal1, &x, &tmpReal2, realContext);
      realAdd(SIGMA_X3, &tmpReal2, SIGMA_X3, realContext);

      // sigma x⁴
      realMultiply(&tmpReal2, &x, &tmpReal2, realContext);
      realAdd(SIGMA_X4, &tmpReal2, SIGMA_X4, realContext);

      // sigma x²y
      realMultiply(&tmpReal1, &y, &tmpReal2, realContext);
      realAdd(SIGMA_X2Y, &tmpReal2, SIGMA_X2Y, realContext);

      // sigma x²/y
      realDivide(&tmpReal1, &y, &tmpReal2, realContext);
      realAdd(SIGMA_X2onY, &tmpReal2, SIGMA_X2onY, realContext);

      // sigma 1/x²
      realDivide(const_1, &tmpReal1, &tmpReal2, realContext);
      realAdd(SIGMA_1onX2, &tmpReal2, SIGMA_1onX2, realContext);

      // sigma y²
      realMultiply(&y, &y, &tmpReal1, realContext);
      realAdd(SIGMA_Y2, &tmpReal1, SIGMA_Y2, realContext);

      // sigma 1/y²
      realDivide(const_1, &tmpReal1, &tmpReal2, realContext);
      realAdd(SIGMA_1onY2, &tmpReal2, SIGMA_1onY2, realContext);

      // sigma xy
      realMultiply(&x, &y, &tmpReal1, realContext);
      realAdd(SIGMA_XY, &tmpReal1, SIGMA_XY, realContext);

      // sigma ln(x)
      WP34S_Ln(&x, &tmpReal1, realContext);
      realCopy(&tmpReal1 ,&tmpReal3);
      realAdd(SIGMA_lnX, &tmpReal1, SIGMA_lnX, realContext);

      // sigma ln²(x)
      realMultiply(&tmpReal1, &tmpReal1, &tmpReal2, realContext);
      realAdd(SIGMA_ln2X, &tmpReal2, SIGMA_ln2X, realContext);

      // sigma yln(x)
      realMultiply(&tmpReal1, &y, &tmpReal1, realContext);
      realAdd(SIGMA_YlnX, &tmpReal1, SIGMA_YlnX, realContext);

      // sigma ln(y)
      WP34S_Ln(&y, &tmpReal1, realContext);
      realAdd(SIGMA_lnY, &tmpReal1, SIGMA_lnY, realContext);

      // sigma ln(x)×ln(y)
      realMultiply(&tmpReal3, &tmpReal1, &tmpReal3, realContext);
      realAdd(SIGMA_lnXlnY, &tmpReal3, SIGMA_lnXlnY, realContext);

      // sigma ln(y)/x
      realDivide(&tmpReal1, &x, &tmpReal2, realContext);
      realAdd(SIGMA_lnYonX, &tmpReal2, SIGMA_lnYonX, realContext);

      // sigma ln²(y)
      realMultiply(&tmpReal1, &tmpReal1, &tmpReal2, realContext);
      realAdd(SIGMA_ln2Y, &tmpReal2, SIGMA_ln2Y, realContext);

      // sigma xln(y)
      realMultiply(&tmpReal1, &x, &tmpReal1, realContext);
      realAdd(SIGMA_XlnY, &tmpReal1, SIGMA_XlnY, realContext);

      // sigma x²ln(y)
      realMultiply(&tmpReal1, &x, &tmpReal1, realContext);
      realAdd(SIGMA_X2lnY, &tmpReal1, SIGMA_X2lnY, realContext);

      // sigma 1/x
      realDivide(const_1, &x, &tmpReal1, realContext);
      realAdd(SIGMA_1onX, &tmpReal1, SIGMA_1onX, realContext);

      // sigma x/y
      realDivide(&x, &y, &tmpReal1, realContext);
      realAdd(SIGMA_XonY, &tmpReal1, SIGMA_XonY, realContext);

      // sigma 1/y
      realDivide(const_1, &y, &tmpReal1, realContext);
      realAdd(SIGMA_1onY, &tmpReal1, SIGMA_1onY, realContext);

      graph_sigmaplus(+1, &x, &y);
    }
    else { // SIGMA-
      // n
      realSubtract(SIGMA_N, const_1, SIGMA_N, realContext);

      // sigma x
      realSubtract(SIGMA_X, &x, SIGMA_X, realContext);

      // sigma y
      realSubtract(SIGMA_Y, &y, SIGMA_Y, realContext);

      // sigma x²
      realMultiply(&x, &x, &tmpReal1, realContext);
      realSubtract(SIGMA_X2, &tmpReal1, SIGMA_X2, realContext);

      // sigma x³
      realMultiply(&tmpReal1, &x, &tmpReal2, realContext);
      realSubtract(SIGMA_X3, &tmpReal2, SIGMA_X3, realContext);

      // sigma x⁴
      realMultiply(&tmpReal2, &x, &tmpReal2, realContext);
      realSubtract(SIGMA_X4, &tmpReal2, SIGMA_X4, realContext);

      // sigma x²y
      realMultiply(&tmpReal1, &y, &tmpReal2, realContext);
      realSubtract(SIGMA_X2Y, &tmpReal2, SIGMA_X2Y, realContext);

      // sigma x²/y
      realDivide(&tmpReal1, &y, &tmpReal2, realContext);
      realSubtract(SIGMA_X2onY, &tmpReal2, SIGMA_X2onY, realContext);

      // sigma 1/x²
      realDivide(const_1, &tmpReal1, &tmpReal2, realContext);
      realSubtract(SIGMA_1onX2, &tmpReal2, SIGMA_1onX2, realContext);

      // sigma y²
      realMultiply(&y, &y, &tmpReal1, realContext);
      realSubtract(SIGMA_Y2, &tmpReal1, SIGMA_Y2, realContext);

      // sigma 1/y²
      realDivide(const_1, &tmpReal1, &tmpReal2, realContext);
      realSubtract(SIGMA_1onY2, &tmpReal2, SIGMA_1onY2, realContext);

      // sigma xy
      realMultiply(&x, &y, &tmpReal1, realContext);
      realSubtract(SIGMA_XY, &tmpReal1, SIGMA_XY, realContext);

      // sigma ln(x)
      WP34S_Ln(&x, &tmpReal1, realContext);
      realCopy(&tmpReal1 ,&tmpReal3);
      realSubtract(SIGMA_lnX, &tmpReal1, SIGMA_lnX, realContext);

      // sigma ln²(x)
      realMultiply(&tmpReal1, &tmpReal1, &tmpReal2, realContext);
      realSubtract(SIGMA_ln2X, &tmpReal2, SIGMA_ln2X, realContext);

      // sigma yln(x)
      realMultiply(&tmpReal1, &y, &tmpReal1, realContext);
      realSubtract(SIGMA_YlnX, &tmpReal1, SIGMA_YlnX, realContext);

      // sigma ln(y)
      WP34S_Ln(&y, &tmpReal1, realContext);
      realSubtract(SIGMA_lnY, &tmpReal1, SIGMA_lnY, realContext);

      // sigma ln(x)×ln(y)
      realMultiply(&tmpReal3, &tmpReal1, &tmpReal3, realContext);
      realSubtract(SIGMA_lnXlnY, &tmpReal3, SIGMA_lnXlnY, realContext);

      // sigma ln(y)/x
      realDivide(&tmpReal1, &x, &tmpReal2, realContext);
      realSubtract(SIGMA_lnYonX, &tmpReal2, SIGMA_lnYonX, realContext);

      // sigma ln²(y)
      realMultiply(&tmpReal1, &tmpReal1, &tmpReal2, realContext);
      realSubtract(SIGMA_ln2Y, &tmpReal2, SIGMA_ln2Y, realContext);

      // sigma xln(y)
      realMultiply(&tmpReal1, &x, &tmpReal1, realContext);
      realSubtract(SIGMA_XlnY, &tmpReal1, SIGMA_XlnY, realContext);

      // sigma x²ln(y)
      realMultiply(&tmpReal1, &x, &tmpReal1, realContext);
      realAdd(SIGMA_X2lnY, &tmpReal1, SIGMA_X2lnY, realContext);

      // sigma 1/x
      realDivide(const_1, &x, &tmpReal1, realContext);
      realSubtract(SIGMA_1onX, &tmpReal1, SIGMA_1onX, realContext);

      // sigma x/y
      realDivide(&x, &y, &tmpReal1, realContext);
      realSubtract(SIGMA_XonY, &tmpReal1, SIGMA_XonY, realContext);

      // sigma 1/y
      realDivide(const_1, &y, &tmpReal1, realContext);
      realSubtract(SIGMA_1onY, &tmpReal1, SIGMA_1onY, realContext);

      graph_sigmaplus(-1, &x, &y);
    }

    temporaryInformation = TI_STATISTIC_SUMS;
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot use (%s, %s) as statistical data!", getDataTypeName(getRegisterDataType(REGISTER_X), false, false), getDataTypeName(getRegisterDataType(REGISTER_Y), false, false));
      moreInfoOnError("In function fnSigma:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}



void fnStatSum(uint16_t sum) {
  if(checkMinimumDataPoints(const_1)) {
    liftStack();
    realToReal34((real_t *)(statisticalSumsPointer + REAL_SIZE * sum), REGISTER_REAL34_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief SUM ==> regX, regY
 * regX = SUM x, regY = SUM y
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnSumXY(uint16_t unusedButMandatoryParameter) {
  if(checkMinimumDataPoints(const_1)) {
    liftStack();
    setSystemFlag(FLAG_ASLIFT);
    liftStack();

    realToReal34(SIGMA_X, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(SIGMA_Y, REGISTER_REAL34_DATA(REGISTER_Y));

    temporaryInformation = TI_SUMX_SUMY;
  }
}



/********************************************//**
 * \brief Xmin ==> regX, regY
 * regX = min x, regY = min y
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnXmin(uint16_t unusedButMandatoryParameter) {
  if(checkMinimumDataPoints(const_1)) {
    liftStack();
    setSystemFlag(FLAG_ASLIFT);
    liftStack();

    realToReal34(SIGMA_XMIN, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(SIGMA_YMIN, REGISTER_REAL34_DATA(REGISTER_Y));

    temporaryInformation = TI_XMIN_YMIN;
  }
}



/********************************************//**
 * \brief Xmax ==> regX, regY
 * regX = max x, regY = max y
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnXmax(uint16_t unusedButMandatoryParameter) {
  if(checkMinimumDataPoints(const_1)) {
    liftStack();
    setSystemFlag(FLAG_ASLIFT);
    liftStack();

    realToReal34(SIGMA_XMAX, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(SIGMA_YMAX, REGISTER_REAL34_DATA(REGISTER_Y));

    temporaryInformation = TI_XMAX_YMAX;
  }
}
