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
 * \file sincpi.c
 ***********************************************/
// Coded by JM, based on sin.c

#include "mathematics/sincpi.h"

#include "constantPointers.h"
#include "debug.h"
#include "error.h"
#include "conversionAngles.h"
#include "fonts.h"
#include "items.h"
#include "mathematics/division.h"
#include "mathematics/wp34s.h"
#include "matrix.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43s.h"



TO_QSPI void (* const Sincpi[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX ==> 1            2           3           4            5            6            7            8             9               10
//          Long integer Real34      Complex34   Time         Date         String       Real34 mat   Complex34 m   Short integer   ConfigData
            sincpiLonI,  sincpiReal, sincpiCplx, sincpiError, sincpiError, sincpiError, sincpiRema,  sincpiCxma,   sincpiError,    sincpiError
};



/********************************************//**
 * \brief Data type error in sincpi
 *
 * \param void
 * \return void
 ***********************************************/
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
void sincpiError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    sprintf(errorMessage, "cannot calculate Sincpi for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    moreInfoOnError("In function fnSincpi:", errorMessage, NULL, NULL);
}
#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)



/********************************************//**
 * \brief regX ==> regL and sincpi(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnSincpi(uint16_t unusedButMandatoryParameter) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Sincpi[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}


void sincpiComplex(const real_t *real, const real_t *imag, real_t *resReal, real_t *resImag, realContext_t *realContext) {
  // sin(a + ib) = sin(a)*cosh(b) + i*cos(a)*sinh(b)
  // sinc(a + ib) = sin(a + ib) / (a + ib), for the allowable conditions
  real_t rr, ii, rmdr;

  realCopy(real, &rr);
  realCopy(imag, &ii);
  realDivideRemainder(&rr, const_1, &rmdr, realContext);

  if(realIsZero(&rr) && realIsZero(&ii)) {
    realCopy(const_1, resReal);
    realCopy(const_0, resImag);
  }
  else if(realIsZero(&rmdr) && realIsZero(&ii)) {
    realCopy(const_0, resReal);
    realCopy(const_0, resImag);
  }
  else {
    real_t sina, cosa, sinhb, coshb, sinR, sinImag;

    realMultiply(&rr, const_pi, &rr, realContext);
    realMultiply(&ii, const_pi, &ii, realContext);

    WP34S_Cvt2RadSinCosTan(&rr, amRadian, &sina, &cosa, NULL, realContext);
    WP34S_SinhCosh(&ii, &sinhb, &coshb, realContext);

    realMultiply(&sina, &coshb, resReal, realContext);
    realMultiply(&cosa, &sinhb, resImag, realContext);

    realCopy(resReal, &sinR);
    realCopy(resImag, &sinImag);
    divComplexComplex(&sinR, &sinImag, &rr, &ii, resReal, resImag, realContext);
  }
}



void sincpiLonI(void) {

  if(longIntegerIsZeroRegister(REGISTER_X)) {
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(const_1, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X));
  }
}



void sincpiRema(void) {
  elementwiseRema(sincpiReal);
}



void sincpiCxma(void) {
  fnToBeCoded();
}







void sincpiReal(void) {
  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getSystemFlag(FLAG_SPCRES)) {
      realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function sincpiReal:", "cannot divide a real34 by " STD_PLUS_MINUS STD_INFINITY " when flag D is not set", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }
  }

  else {
    real_t x, xx, sine;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    realToIntegralValue(&x, &xx, DEC_ROUND_DOWN, &ctxtReal39);
    realSubtract(&x, &xx , &xx, &ctxtReal39);

    if(realIsZero(&x)) {
      realCopy(const_1, &x);
    }
    else if(realIsZero(&xx)) {
      realCopy(const_0, &x);
    }
    else {
      if(getRegisterAngularMode(REGISTER_X) == amNone) {
        setRegisterAngularMode(REGISTER_X, amMultPi);
      }
      convertAngleFromTo(&x, getRegisterAngularMode(REGISTER_X), amRadian, &ctxtReal39);
      WP34S_Cvt2RadSinCosTan(&x, amRadian, &sine, NULL, NULL, &ctxtReal39);
      realDivide(&sine, &x, &x, &ctxtReal39);
    }
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }

  setRegisterAngularMode(REGISTER_X, amNone);
}



void sincpiCplx(void) {
  real_t zReal, zImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &zReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &zImag);

  sincpiComplex(&zReal, &zImag, &zReal, &zImag, &ctxtReal39);

  realToReal34(&zReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&zImag, REGISTER_IMAG34_DATA(REGISTER_X));
}
