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
 * \file percentSigma.c
 ***********************************************/

#include "wp43s.h"

static void dataTypeError(void);

static void (* const functionMatrix[9])(void) = {
// regX ==> 1                 2                 3              4              5              6              7              8              9
//          Long integer      Real34            complex34      Time           Date           String         Real34 mat     Complex34 mat  Short integer
            percentSigmaLonI, percentSigmaReal, dataTypeError, dataTypeError, dataTypeError, dataTypeError, dataTypeError, dataTypeError, dataTypeError
};

//=============================================================================
// Error handling
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief Data type error in %Sigma
 *
 * \param void
 * \return void
 ***********************************************/
static void dataTypeError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);

  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot use %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnPercentSigma:", errorMessage, NULL, NULL);
  #endif
}

//=============================================================================
// Main function
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief regX ==> regL and PercentSigma(regX) ==> regX
 * enables stack lift and refreshes the stack.
 * Calculate %Sigma
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnPercentSigma(uint16_t unusedParamButMandatory) {
  if(statisticalSumsPointer == NULL) {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "There is no statistical data available!");
      showInfoDialog("In function fnPercentSigma:", errorMessage, NULL, NULL);
    #endif
  }
  else {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    functionMatrix[getRegisterDataType(REGISTER_X)]();

    adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
  }
}

//=============================================================================
// PercentSigma calculation functions
//-----------------------------------------------------------------------------

static bool_t percentSigma(real_t *xReal, real_t *rReal, realContext_t *realContext) {
  real34ToReal(SIGMA_X, rReal);    // r = Sum(x)

  if(realIsZero(rReal)) {
    if(getFlag(FLAG_DANGER)) {
      realCopy((realIsPositive(rReal) ? const_plusInfinity : const_minusInfinity), rReal);
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnPercentSigma:", "cannot divide a real by 0", NULL, NULL);
      #endif
      return false;
    }
  }

  realDivide(xReal, rReal, rReal, realContext);       // r = x/Sum(x)
  realMultiply(rReal, const_100, rReal, realContext); // r = 100*x/Sum(x)

  return true;
}

/********************************************//**
 * \brief Percent(X(long integer)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void percentSigmaLonI(void) {
  real_t xReal, rReal;

  convertLongIntegerRegisterToReal(REGISTER_X, &xReal, &ctxtReal39);

  if(percentSigma(&xReal, &rReal, &ctxtReal39)) {
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
    realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, AM_NONE);
  }
}

/********************************************//**
 * \brief Percent(X(real34)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void percentSigmaReal(void) {
  real_t xReal, rReal;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);

  if(percentSigma(&xReal, &rReal, &ctxtReal39)) {
    realToReal34(&rReal, REGISTER_REAL34_DATA(REGISTER_X));
  }
}
