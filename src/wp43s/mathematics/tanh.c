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
 * \file tanh.c
 ***********************************************/

#include "wp43s.h"



void (* const Tanh[9])(void) = {
// regX ==> 1            2         3         4          5          6          7          8           9
//          Long integer Real34    complex34 Time       Date       String     Real34 mat Complex34 m Short integer
            tanhLonI,    tanhReal, tanhCplx, tanhError, tanhError, tanhError, tanhRema,  tanhCxma,   tanhError
};



/********************************************//**
 * \brief Data type error in tanh
 *
 * \param void
 * \return void
 ***********************************************/
void tanhError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Tan for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnTanh:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and tanh(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnTanh(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Tanh[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void tanhLonI(void) {
  real_t x;

  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  WP34S_Tanh(&x, &x, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
}



void tanhRema(void) {
  fnToBeCoded();
}



void tanhCxma(void) {
  fnToBeCoded();
}



void tanhReal(void) {
  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X)) && !getSystemFlag(FLAG_SPCRES)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function tanhReal:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of tanh when flag D is not set", NULL, NULL);
    #endif
    return;
  }

  real_t x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  WP34S_Tanh(&x, &x, &ctxtReal39);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void tanhCplx(void) {
  // tanh(a + i b) = (tanh(a) + i tan(b)) / (1 + i tanh(a) tan(b))
  real_t a, b, sina, cosa;
  real_t numerReal, denomReal;
  real_t numerImag, denomImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  if(realIsZero(&b)) {
    WP34S_Tanh(&a, &numerReal, &ctxtReal39);
    realZero(&numerImag);
  }
  else {
    WP34S_Tanh(&a, &numerReal, &ctxtReal39);
    WP34S_Cvt2RadSinCosTan(&b, AM_RADIAN, &sina, &cosa, &numerImag, &ctxtReal39);

    realCopy(const_1, &denomReal);
    realMultiply(&numerReal, &numerImag, &denomImag, &ctxtReal39);

    divComplexComplex(&numerReal, &numerImag, &denomReal, &denomImag, &numerReal, &numerImag, &ctxtReal39);
  }

  realToReal34(&numerReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&numerImag, REGISTER_IMAG34_DATA(REGISTER_X));
}
