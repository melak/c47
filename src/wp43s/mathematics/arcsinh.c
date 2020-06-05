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
 * \file arcsinh.c
 ***********************************************/

#include "wp43s.h"



void (* const arcsinh[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX ==> 1             2            3            4             5             6             7            8            9             10
//          Long integer  Real34       Complex34    Time          Date          String        Real34 mat   Complex34 m  Short integer Config data
            arcsinhLonI,  arcsinhReal, arcsinhCplx, arcsinhError, arcsinhError, arcsinhError, arcsinhRema, arcsinhCxma, arcsinhError, arcsinhError
};



/********************************************//**
 * \brief Data type error in arcsinh
 *
 * \param void
 * \return void
 ***********************************************/
void arcsinhError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate arcsinh for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnArcsinh:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and arcsinh(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnArcsinh(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  arcsinh[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void arcsinhLonI(void) {
  real_t x, xSquared;

  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  // arcsinh(x) = ln(x + sqrt(x² + 1))
  realMultiply(&x, &x, &xSquared, &ctxtReal39);
  realAdd(&xSquared, const_1, &xSquared, &ctxtReal39);
  realSquareRoot(&xSquared, &xSquared, &ctxtReal39);
  realAdd(&xSquared, &x, &x, &ctxtReal39);
  WP34S_Ln(&x, &x, &ctxtReal39);

  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
}



void arcsinhRema(void) {
  fnToBeCoded();
}



void arcsinhCxma(void) {
  fnToBeCoded();
}



void arcsinhReal(void) {
  real_t x, xSquared;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  if(realIsInfinite(&x) && realIsNegative(&x)) {
    realCopy(const_minusInfinity, &x);
  }
  else {
    // arcsinh(x) = ln(x + sqrt(x² + 1))
    realMultiply(&x, &x, &xSquared, &ctxtReal51);
    realAdd(&xSquared, const_1, &xSquared, &ctxtReal51);
    realSquareRoot(&xSquared, &xSquared, &ctxtReal51);
    realAdd(&xSquared, &x, &x, &ctxtReal51);
    WP34S_Ln(&x, &x, &ctxtReal51);
  }

  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void arcsinhCplx(void) {
  real_t a, b, real, imag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  // arcsinh(z) = ln(z + sqrt(z² + 1))
  // calculate z²   real part
  realMultiply(&b, &b, &real, &ctxtReal39);
  realChangeSign(&real);
  realFMA(&a, &a, &real, &real, &ctxtReal39);

  // calculate z²   imaginary part
  realMultiply(&a, &b, &imag, &ctxtReal39);
  realMultiply(&imag, const_2, &imag, &ctxtReal39);

  // calculate z² + 1
  realAdd(&real, const_1, &real, &ctxtReal39);

  // calculate sqrt(z² + 1)
  realRectangularToPolar(&real, &imag, &real, &imag, &ctxtReal39);
  realSquareRoot(&real, &real, &ctxtReal39);
  realMultiply(&imag, const_1on2, &imag, &ctxtReal39);
  realPolarToRectangular(&real, &imag, &real, &imag, &ctxtReal39);

  // calculate z + sqrt(z² + 1)
  realAdd(&a, &real, &real, &ctxtReal39);
  realAdd(&b, &imag, &imag, &ctxtReal39);

  // calculate ln(z + sqtr(z² + 1))
  realRectangularToPolar(&real, &imag, &a, &b, &ctxtReal39);
  WP34S_Ln(&a, &a, &ctxtReal39);

  realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}
