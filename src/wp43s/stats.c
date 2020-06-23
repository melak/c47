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

#include "wp43s.h"


/********************************************//**
 * \brief Verifies that the statistical registers are allocated and that there are enough data
 * An appropriate error message is displayed if either condition fails
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return int
 ***********************************************/
int checkMinimumDataPoints(const real_t *n) {
  if(statisticalSumsPointer == NULL) {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "There is no statistical data available!");
      showInfoDialog("In function checkMinimumDataPoints:", errorMessage, NULL, NULL);
    #endif
    return 0;
  }
  if (realCompareLessThan(SIGMA_N, n)) {
    displayCalcErrorMessage(ERROR_TOO_FEW_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "There is insufficient statistical data available!");
      showInfoDialog("In function checkMinimumDataPoints:", errorMessage, NULL, NULL);
    #endif
    return 0;
  }
  return 1;
}


void initStatisticalSums(void) {
  if(statisticalSumsPointer == NULL) {
    statisticalSumsPointer = allocWp43s(NUMBER_OF_STATISTICAL_SUMS * TO_BYTES(REAL_SIZE));
    for(int32_t sum=0; sum<NUMBER_OF_STATISTICAL_SUMS; sum++) {
      realZero((real_t *)(statisticalSumsPointer + REAL_SIZE * sum));
    }
  realCopy(const_minusInfinity, MAX_X);
  realCopy(const_plusInfinity , MIN_X);
  realCopy(const_minusInfinity, MAX_Y);
  realCopy(const_plusInfinity , MIN_Y);
  }
}



/********************************************//**
 * \brief Adds a value to the statistic registers
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSigma(uint16_t plusMinus) {
  real_t tmpReal1, tmpReal2, tmpReal3, x, y;
  bool_t refreshLineX;
  realContext_t *realContext = &ctxtReal75; // Summation data with 75 digits

  if(   (getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtReal34)
     && (getRegisterDataType(REGISTER_Y) == dtLongInteger || getRegisterDataType(REGISTER_Y) == dtReal34)) {

    if(statisticalSumsPointer == NULL) {
      initStatisticalSums();
    }

    if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
      convertLongIntegerRegisterToReal(REGISTER_X, &x, realContext);
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      refreshLineX = true;
    }
    else {
      real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
      refreshLineX = false;
    }

    if(getRegisterDataType(REGISTER_Y) == dtLongInteger) {
      convertLongIntegerRegisterToReal(REGISTER_Y, &y, realContext);
      convertLongIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
    }
    else {
      real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    }

    if(plusMinus == 1) { // SIGMA+
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

      // sigma 1/x
      realDivide(const_1, &x, &tmpReal1, realContext);
      realAdd(SIGMA_1onX, &tmpReal1, SIGMA_1onX, realContext);

      // sigma x/y
      realDivide(&x, &y, &tmpReal1, realContext);
      realAdd(SIGMA_XonY, &tmpReal1, SIGMA_XonY, realContext);

      // sigma 1/y
      realDivide(const_1, &y, &tmpReal1, realContext);
      realAdd(SIGMA_1onY, &tmpReal1, SIGMA_1onY, realContext);

      // max x
      if(realCompareGreaterThan(&x, MAX_X))
        realCopy(&x, MAX_X);

      // max y
      if(realCompareGreaterThan(&y, MAX_Y))
        realCopy(&y, MAX_Y);

      // min x
      if(realCompareLessThan(&x, MIN_X))
        realCopy(&x, MIN_X);

      // min y
      if(realCompareLessThan(&y, MIN_Y))
        realCopy(&y, MIN_Y);
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

      // sigma 1/x
      realDivide(const_1, &x, &tmpReal1, realContext);
      realSubtract(SIGMA_1onX, &tmpReal1, SIGMA_1onX, realContext);

      // sigma x/y
      realDivide(&x, &y, &tmpReal1, realContext);
      realSubtract(SIGMA_XonY, &tmpReal1, SIGMA_XonY, realContext);

      // sigma 1/y
      realDivide(const_1, &y, &tmpReal1, realContext);
      realSubtract(SIGMA_1onY, &tmpReal1, SIGMA_1onY, realContext);
    }

    temporaryInformation = TI_STATISTIC_SUMS;
    refreshRegisterLine(REGISTER_Y);
    if(refreshLineX) {
      refreshRegisterLine(REGISTER_X);
    }
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot use (%s, %s) as statistical data!", getDataTypeName(getRegisterDataType(REGISTER_X), false, false), getDataTypeName(getRegisterDataType(REGISTER_Y), false, false));
      showInfoDialog("In function fnSigma:", errorMessage, NULL, NULL);
    #endif
  }
//for(int i=0; i<NUMBER_OF_STATISTICAL_SUMS; i++) {
//printf("sum %02d ", i); printRealToConsole(((real_t *)statisticalSumsPointer) + i); printf("\n");
//}
}



void fnStatSum(uint16_t sum) {
  if(statisticalSumsPointer == NULL) {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "There is no statistical data available!");
      showInfoDialog("In function fnStatSum:", errorMessage, NULL, NULL);
    #endif
  }
  else {
   liftStack();
   realToReal34((real_t *)(statisticalSumsPointer + REAL_SIZE * sum), REGISTER_REAL34_DATA(REGISTER_X));
   refreshStack();
  }
}

/********************************************//**
 * \brief SUM ==> regX, regY
 * enables stack lift and refreshes the stack.
 * regX = SUM x, regY = SUM y
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSumXY(uint16_t unusedParamButMandatory) {
  if(statisticalSumsPointer == NULL) {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "There is no statistical data available!");
      showInfoDialog("In function fnSumXY:", errorMessage, NULL, NULL);
    #endif
  }
  else {
    saveStack();

    liftStack();
    STACK_LIFT_ENABLE;
    liftStack();

    realToReal34(SIGMA_X, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(SIGMA_Y, REGISTER_REAL34_DATA(REGISTER_Y));

    temporaryInformation = TI_SUMX_SUMY;
    refreshStack();
  }
}
