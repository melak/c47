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
 * \file deltaPercentXmean.c
 ***********************************************/

#include "mathematics/deltaPercentXmean.h"

#include "constantPointers.h"
#include "debug.h"
#include "error.h"
#include "mathematics/comparisonReals.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43.h"

static void dataTypeError(void);

TO_QSPI void (* const deltaPercentXmean[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX ==>    1                      2                      3              4              5              6              7              8              9              10
//             Long integer           Real34                 Complex34      Time           Date           String         Real34 mat     Complex34 mat  Short integer  Config data
               deltaPercentXmeanLonI, deltaPercentXmeanReal, dataTypeError, dataTypeError, dataTypeError, dataTypeError, dataTypeError, dataTypeError, dataTypeError, dataTypeError
};


//=============================================================================
// Error handling
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief Data type error in %T
 *
 * \param void
 * \return void
 ***********************************************/
static void dataTypeError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);

  #if(EXTRA_INFO_ON_CALC_ERROR == 1)
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    sprintf(errorMessage, "cannot calculate delta percentage for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    moreInfoOnError("In function deltaPercentXmean:", errorMessage, NULL, NULL);
  #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
}

//=============================================================================
// Main function
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief regX ==> regL and deltaPercentXmean(regX) ==> regX
 * enables stack lift and refreshes the stack.
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/


void fnDeltaPercentXmean(uint16_t unusedButMandatoryParameter) {
  if(!saveLastX()) {
    return;
  }

  deltaPercentXmean[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);

  temporaryInformation = TI_PERCD;
}


//=============================================================================
// Delta% calculation functions
//-----------------------------------------------------------------------------

bool_t deltaPercentXmeanReal_(real_t *xReal, real_t *rReal, realContext_t *realContext) {
real_t yReal;

  realDivide(SIGMA_X, SIGMA_N, &yReal, &ctxtReal39);
  /*
   * Check x and y
   */
  if(realIsZero(xReal) && realCompareEqual(xReal, &yReal)) {
      if(getSystemFlag(FLAG_SPCRES)) {
        realCopy(const_NaN, rReal);
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if(EXTRA_INFO_ON_CALC_ERROR == 1)
          moreInfoOnError("In function fndeltaPercentXmean:", "cannot divide 0 by 0", NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        return false;
      }
  }
  else if(realIsZero(&yReal)) {
    if(getSystemFlag(FLAG_SPCRES)) {
      realCopy((realCompareAbsGreaterThan(xReal, &yReal) ? const_plusInfinity : const_minusInfinity),rReal);
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if(EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function fndeltaPercentXmean:", "cannot divide a real by y=0", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return false;
    }
  }
  else {
    realSubtract(xReal, &yReal, rReal, realContext);     // r = x - y
    realDivide(rReal, &yReal, rReal, realContext);       // r = (x - y)/y
    realMultiply(rReal, const_100, rReal, realContext); // r = r * 100.0
  }
  return true;
}

/********************************************//**
 * \brief deltaPercentXmean(Y(long integer), X(long integer)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void deltaPercentXmeanLonI(void) {
  real_t xReal;
  real_t rReal;

  convertLongIntegerRegisterToReal(REGISTER_X, &xReal, &ctxtReal75);

  if(deltaPercentXmeanReal_(&xReal, &rReal, &ctxtReal75)) {
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    convertRealToReal34ResultRegister(&rReal, REGISTER_X);
    setRegisterAngularMode(REGISTER_X, amNone);
  }
}

/********************************************//**
 * \brief deltaPercentXmean(Y(long integer), X(real34)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void deltaPercentXmeanReal(void) {
  real_t xReal;
  real_t rReal;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);

  if(deltaPercentXmeanReal_(&xReal, &rReal, &ctxtReal75)) {
    convertRealToReal34ResultRegister(&rReal, REGISTER_X);
  }
}
