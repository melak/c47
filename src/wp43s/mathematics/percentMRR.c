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
 * \file percentMRR.c
 ***********************************************/

#include "wp43s.h"

static bool_t convertRegisterToReal(calcRegister_t regist, real_t *r) {
  bool_t result = true;

  switch (getRegisterDataType(regist)) {
    case dtLongInteger:
      convertLongIntegerRegisterToReal(regist, r, &ctxtReal34);
      break;

    case dtReal34:
      real34ToReal(REGISTER_REAL34_DATA(regist), r);
      break;

    default:
      result = false;
  }

  return result;
}

static void percentMRR() {
  real_t xReal, yReal, zReal;

  /*
   * Convert register X.
   */
  if (!convertRegisterToReal(REGISTER_X, &xReal)) {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot %%MRR with %s in X", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnPercentMRR:", errorMessage, NULL, NULL);
    #endif
    return;
  }
  /*
   * Convert register Y.
   */
  if (!convertRegisterToReal(REGISTER_Y, &yReal)) {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot %%MRR with %s in Y", getRegisterDataTypeName(REGISTER_Y, true, false));
      showInfoDialog("In function fnPercentMRR:", errorMessage, NULL, NULL);
    #endif
    return;
  }
  /*
   * Convert register Z
   */
  if (!convertRegisterToReal(REGISTER_Z, &zReal)) {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_Z);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot %%MRR with %s in Z", getRegisterDataTypeName(REGISTER_Z, true, false));
      showInfoDialog("In function fnPercentMRR:", errorMessage, NULL, NULL);
    #endif
    return;
  }
  /*
   * Calculate %MRR
   */
  real_t q;

  if (realIsZero(&xReal) && realIsZero(&yReal)) {
    if (getSystemFlag(FLAG_SPCRES)) {
      realCopy(const_NaN, &q);
    } else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnPercentMRTR:", "cannot divide x=0 by y=0", NULL, NULL);
      #endif
      return;
    }
  } else if (realIsZero(&yReal)) {
    if (getSystemFlag(FLAG_SPCRES)) {
      realCopy((realIsPositive(&xReal) ? const_plusInfinity : const_minusInfinity), &q);
    } else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnPercentMRTR:", "cannot divide a real by 0", NULL, NULL);
      #endif
      return;
    }
  }

  realDivide(&xReal, &yReal, &q, &ctxtReal39);        // q = x/y
  realDivide(const_1, &zReal, &zReal, &ctxtReal39);   // z = 1/z
  realPower(&q, &zReal, &q, &ctxtReal39);             // q = pow(x/y, 1/z)
  realSubtract(&q, const_1, &q, &ctxtReal39);         // q = pow(x/y, 1/z) - 1
  realMultiply(&q, const_100, &q, &ctxtReal39);       // q = 100 * ( pow(x/y, 1/z) - 1 )

  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  realToReal34(&q, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}

/********************************************//**
 * \brief regX ==> regL and %MRR(regX, RegY, RegZ) ==> regX
 * enables stack lift and refreshes the stack.
 * Calculate the %MRR.
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnPercentMRR(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  percentMRR();

  adjustResult(REGISTER_X, true, true, REGISTER_X, -1, -1);
  if (lastErrorCode == 0)
    fnDropY(NOPARAM);
}
