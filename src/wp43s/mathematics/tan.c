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
    showInfoDialog("In function fnTan:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and tan(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnTan(uint16_t unusedParamButMandatory) {
  saveStack();
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
      showInfoDialog("In function tanLonI:", "X = " STD_PLUS_MINUS "90" STD_DEGREE, NULL, NULL);
    #endif
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
        showInfoDialog("In function tanReal:", "X = " STD_PLUS_MINUS "90" STD_DEGREE, NULL, NULL);
      #endif
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

  real_t real, imag;
  real_t sina, cosa, sinhb, coshb;
  real_t numerReal, denomReal;
  real_t numerImag, denomImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &real);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &imag);

  WP34S_Cvt2RadSinCosTan(&real, AM_RADIAN, &sina, &cosa, NULL, &ctxtReal51);
  WP34S_SinhCosh(&imag, &sinhb, &coshb, &ctxtReal51);

  realMultiply(&sina, &coshb, &numerReal, &ctxtReal51);
  realMultiply(&cosa, &sinhb, &numerImag, &ctxtReal51);

  realMultiply(&cosa, &coshb, &denomReal, &ctxtReal51);
  realMultiply(&sina, &sinhb, &denomImag, &ctxtReal51);
  realChangeSign(&denomImag);

  divComplexComplex(&numerReal, &numerImag, &denomReal, &denomImag, &real, &imag, &ctxtReal51);

  realToReal34(&real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&imag, REGISTER_IMAG34_DATA(REGISTER_X));
}
