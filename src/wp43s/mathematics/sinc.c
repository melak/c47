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
 * \file sinc.c
 ***********************************************/
// Coded by JM, based on sin.c

#include "wp43s.h"



void (* const Sinc[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX ==> 1            2         3         4          5          6          7          8           9             10
//          Long integer Real34    Complex34 Time       Date       String     Real34 mat Complex34 m Short integer ConfigData
            sincLonI,    sincReal, sincCplx, sincError, sincError, sincError, sincRema,  sincCxma,   sincError,    sincError
};



/********************************************//**
 * \brief Data type error in sinc
 *
 * \param void
 * \return void
 ***********************************************/
void sincError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Sinc for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnSinc:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and sinc(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSinc(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Sinc[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void sincComplex(const real_t *real, const real_t *imag, real_t *resReal, real_t *resImag, realContext_t *realContext) {
  // sin(a + ib) = sin(a)*cosh(b) + i*cos(a)*sinh(b)
  // sinc (a + ib) = sin(a + ib) / (a + ib), for the allowable conditions
  real_t sina, cosa, sinhb, coshb;
  real_t rr,sinReal;
  real_t ii,sinImag;


//  fnCvtFromCurrentAngularMode(AM_RADIAN);

  realCopy(real, &rr);
  realCopy(imag, &ii);

  if(realIsZero(&rr) && realIsZero(&ii)) {
    realCopy(const_1, resReal);
    realCopy(const_0, resImag);
  }
  else {
    WP34S_Cvt2RadSinCosTan(real, AM_RADIAN, &sina, &cosa, NULL, realContext);
    WP34S_SinhCosh(imag, &sinhb, &coshb, realContext);

    realMultiply(&sina, &coshb, resReal, realContext);
    realMultiply(&cosa, &sinhb, resImag, realContext);

    realCopy(resReal, &sinReal);
    realCopy(resImag, &sinImag);
    divComplexComplex(&sinReal, &sinImag, &rr, &ii, resReal, resImag, realContext);
  }
}



void sincLonI(void) {
  real_t x, rr;

  convertLongIntegerRegisterToReal(REGISTER_X, &rr, &ctxtReal39);

  if(realIsZero(&rr)) {
    realCopy(const_1, &x);
  }
  else {
    longIntegerAngleReduction(REGISTER_X, currentAngularMode, &x);
    WP34S_Cvt2RadSinCosTan(&x, currentAngularMode, &x, NULL, NULL, &ctxtReal39);
    realDivide(&x, &rr, &x, &ctxtReal39);
  }

  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
}



void sincRema(void) {
  fnToBeCoded();
}



void sincCxma(void) {
  fnToBeCoded();
}



void sincReal(void) {
  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getSystemFlag(FLAG_SPCRES)) {
      realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function sincReal:", "cannot divide a real34 by " STD_PLUS_MINUS STD_INFINITY " when flag D is not set", NULL, NULL);
      #endif
    }
  }


  else {
    real_t x, rr;
    uint32_t xAngularMode;
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

    if(realIsZero(&x)) {
      realCopy(const_1, &x);
    }
    else {
      realCopy(&x, &rr);
      xAngularMode = getRegisterAngularMode(REGISTER_X);
      WP34S_Cvt2RadSinCosTan(&x, (xAngularMode == AM_NONE ? currentAngularMode : xAngularMode), &x, NULL, NULL, &ctxtReal39);
      realDivide(&x, &rr, &x, &ctxtReal39);
    }
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void sincCplx(void) {
  real_t zReal, zImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &zReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &zImag);

  sincComplex(&zReal, &zImag, &zReal, &zImag, &ctxtReal39);

  realToReal34(&zReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&zImag, REGISTER_IMAG34_DATA(REGISTER_X));
}
