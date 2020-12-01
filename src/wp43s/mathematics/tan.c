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
 * \file tan.c
 ***********************************************/

#include "wp43s.h"



void (* const Tan[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX ==> 1            2        3         4         5         6         7          8           9             10
//          Long integer Real34   complex34 Time      Date      String    Real34 mat Complex34 m Short integer Config data
            tanLonI,     tanReal, tanCplx,  tanError, tanError, tanError, tanRema,   tanCxma,    tanError,     tanError
};



void longIntegerAngleReduction(calcRegister_t regist, uint8_t angularMode, real_t *reducedAngle) {
  uint32_t oneTurn;

  switch(angularMode) {
    case AM_DEGREE:
    case AM_DMS:    oneTurn = 360; break;
    case AM_GRAD:   oneTurn = 400; break;
    case AM_MULTPI: oneTurn =   2; break;
    default:        oneTurn =   0;
  }

  if(oneTurn == 0) {
    convertLongIntegerRegisterToReal(regist, reducedAngle, &ctxtReal39);
  }
  else {
    longInteger_t angle;

    convertLongIntegerRegisterToLongInteger(regist, angle);
    uInt32ToReal(longIntegerModuloUInt(angle, oneTurn), reducedAngle);
    longIntegerFree(angle);
  }
}



/********************************************//**
 * \brief Data type error in tan
 *
 * \param void
 * \return void
 ***********************************************/
void tanError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Tan for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    moreInfoOnError("In function fnTan:", errorMessage, NULL, NULL);
  #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
}



/********************************************//**
 * \brief regX ==> regL and tan(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnTan(uint16_t unusedButMandatoryParameter) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Tan[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void tanLonI(void) {
  real_t sin, cos, tan;

  longIntegerAngleReduction(REGISTER_X, currentAngularMode, &tan);
  WP34S_Cvt2RadSinCosTan(&tan, currentAngularMode, &sin, &cos, &tan, &ctxtReal39);

  if(realIsZero(&cos) && !getSystemFlag(FLAG_SPCRES)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function tanLonI:", "X = " STD_PLUS_MINUS "90" STD_DEGREE, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  if(realIsZero(&cos)) {
    realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    realToReal34(&tan, REGISTER_REAL34_DATA(REGISTER_X));
  }
}



void tanRema(void) {
  fnToBeCoded();
}



void tanCxma(void) {
  fnToBeCoded();
}



void tanReal(void) {
  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    real_t sin, cos, tan;
    uint32_t xAngularMode;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &tan);
    xAngularMode = getRegisterAngularMode(REGISTER_X);
    WP34S_Cvt2RadSinCosTan(&tan, (xAngularMode == AM_NONE ? currentAngularMode : xAngularMode), &sin, &cos, &tan, &ctxtReal39);

    if(realIsZero(&cos) && !getSystemFlag(FLAG_SPCRES)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function tanReal:", "X = " STD_PLUS_MINUS "90" STD_DEGREE, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }
    else {
      if(realIsZero(&cos)) {
        realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        realToReal34(&tan, REGISTER_REAL34_DATA(REGISTER_X));
      }
    }
  }
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void tanCplx(void) {
  //                sin(a)*cosh(b) + i*cos(a)*sinh(b)
  // tan(a + ib) = -----------------------------------
  //                cos(a)*cosh(b) - i*sin(a)*sinh(b)

    real_t xReal, xImag;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
    real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

    TanComplex(&xReal, &xImag, &xReal, &xImag, &ctxtReal51);

    realToReal34(&xReal, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&xImag, REGISTER_IMAG34_DATA(REGISTER_X));
}

uint8_t TanComplex(const real_t *xReal, const real_t *xImag, real_t *rReal, real_t *rImag, realContext_t *realContext) {
    real_t sina, cosa, sinhb, coshb;
    real_t numerReal, denomReal;
    real_t numerImag, denomImag;

    WP34S_Cvt2RadSinCosTan(xReal, AM_RADIAN, &sina, &cosa, NULL, realContext);
    WP34S_SinhCosh(xImag, &sinhb, &coshb, realContext);

    realMultiply(&sina, &coshb, &numerReal, realContext);
    realMultiply(&cosa, &sinhb, &numerImag, realContext);

    realMultiply(&cosa, &coshb, &denomReal, realContext);
    realMultiply(&sina, &sinhb, &denomImag, realContext);
    realChangeSign(&denomImag);

    divComplexComplex(&numerReal, &numerImag, &denomReal, &denomImag, rReal, rImag, realContext);

    return ERROR_NONE;
}
