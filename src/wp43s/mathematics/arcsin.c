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
 * \file arcsin.c
 ***********************************************/

#include "wp43s.h"



void (* const arcsin[9])(void) = {
// regX ==> 1            2           3           4            5            6            7           8           9
//          Long integer Real34      Complex34   Time         Date         String       Real34 mat  Complex34 m Short integer
            arcsinLonI,  arcsinReal, arcsinCplx, arcsinError, arcsinError, arcsinError, arcsinRema, arcsinCxma, arcsinError
};



/********************************************//**
 * \brief Data type error in arcsin
 *
 * \param void
 * \return void
 ***********************************************/
void arcsinError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate arcsin for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnArcsin:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and arcsin(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnArcsin(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  arcsin[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void arcsinLonI(void) {
  real_t x;

  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  if(realCompareAbsGreaterThan(&x, const_1)) {
    if(getFlag(FLAG_CPXRES)) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
      arcsinCplx();
    }
    else if(getSystemFlag(FLAG_SPCRES)) {
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, currentAngularMode);
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arcsinLonI:", "|X| > 1", "and CPXRES is not set!", NULL);
      #endif
    }
    return;
  }

  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, currentAngularMode);

  if(realIsZero(&x)) {
    real34Zero(REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    realToReal34(const_1on2, REGISTER_REAL34_DATA(REGISTER_X));
    if(realIsNegative(&x)) {
      real34ChangeSign(REGISTER_REAL34_DATA(REGISTER_X));
    }
    convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_MULTPI, currentAngularMode);
  }
}



void arcsinRema(void) {
  fnToBeCoded();
}



void arcsinCxma(void) {
  fnToBeCoded();
}



void arcsinReal(void) {
  real_t x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  setRegisterAngularMode(REGISTER_X, currentAngularMode);

  if(realCompareAbsGreaterThan(&x, const_1)) {
    if(getFlag(FLAG_CPXRES)) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
      arcsinCplx();
      return;
    }
    else if(getSystemFlag(FLAG_SPCRES)) {
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function arcsinReal:", "|X| > 1", "and CPXRES is not set!", NULL);
      #endif
      return;
    }
  }

  WP34S_Asin(&x, &x, &ctxtReal39);
  convertAngleFromTo(&x, AM_RADIAN, currentAngularMode, &ctxtReal39);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));

  if(currentAngularMode == AM_DMS) {
    checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
  }
}



void arcsinCplx(void) {
  real_t a, b, real, imag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  // arcsin(z) = -i.ln(iz + sqrt(1 - z²))
  // calculate z²   real part
  realMultiply(&b, &b, &real, &ctxtReal39);
  realChangeSign(&real);
  realFMA(&a, &a, &real, &real, &ctxtReal39);

  // calculate z²   imaginary part
  realMultiply(&a, &b, &imag, &ctxtReal39);
  realMultiply(&imag, const_2, &imag, &ctxtReal39);

  // calculate 1 - z²
  realSubtract(const_1, &real, &real, &ctxtReal39);
  realChangeSign(&imag);

  // calculate sqrt(1 - z²)
  realRectangularToPolar(&real, &imag, &real, &imag, &ctxtReal39);
  realSquareRoot(&real, &real, &ctxtReal39);
  realMultiply(&imag, const_1on2, &imag, &ctxtReal39);
  realPolarToRectangular(&real, &imag, &real, &imag, &ctxtReal39);

  // calculate iz + sqrt(1 - z²)
  realChangeSign(&b);
  realAdd(&real, &b, &real, &ctxtReal39);
  realAdd(&imag, &a, &imag, &ctxtReal39);

  // calculate ln(iz + sqrt(1 - z²))
  realRectangularToPolar(&real, &imag, &a, &b, &ctxtReal39);
  WP34S_Ln(&a, &a, &ctxtReal39);

  // calculate = -i.ln(iz + sqrt(1 - z²))
  realChangeSign(&a);

  realToReal34(&b, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&a, REGISTER_IMAG34_DATA(REGISTER_X));
}
