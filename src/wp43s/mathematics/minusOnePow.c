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
 * \file minusOnePow.c
 ***********************************************/

#include "wp43s.h"



void (* const m1Pow[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX ==> 1            2          3          4           5           6           7          8           9             10
//          Long integer Real34     complex34  Time        Date        String      Real34 mat Complex34 m Short integer Config data
            m1PowLonI,   m1PowReal, m1PowCplx, m1PowError, m1PowError, m1PowError, m1PowRema, m1PowCxma,  m1PowShoI,    m1PowError
};



/********************************************//**
 * \brief Data type error in exp
 *
 * \param void
 * \return void
 ***********************************************/
void m1PowError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate (-1)" STD_SUP_x " for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    moreInfoOnError("In function fnM1Pow:", errorMessage, NULL, NULL);
  #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
}



/********************************************//**
 * \brief regX ==> regL and 10^regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnM1Pow(uint16_t unusedButMandatoryParameter) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  m1Pow[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



/**********************************************************************
 * In all the functions below:
 * if X is a number then X = a + ib
 * The variables a and b are used for intermediate calculations
 ***********************************************************************/

void m1PowLonI(void) {
  longInteger_t lgInt, exponent;

  longIntegerInit(lgInt);
  uIntToLongInteger(1, lgInt);

  convertLongIntegerRegisterToLongInteger(REGISTER_X, exponent);
  if(longIntegerIsOdd(exponent)) {
    longIntegerChangeSign(lgInt);
  }

  convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);

  longIntegerFree(lgInt);
  longIntegerFree(exponent);
}



void m1PowRema(void) {
  fnToBeCoded();
}



void m1PowCxma(void) {
  fnToBeCoded();
}



void m1PowShoI(void) {
  int32_t signExponent;
  uint64_t valueExponent = WP34S_extract_value(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)), &signExponent);
  int32_t odd = valueExponent & 1;

  if(shortIntegerMode == SIM_UNSIGN && odd) {
    setSystemFlag(FLAG_OVERFLOW);
  }
  else {
    clearSystemFlag(FLAG_OVERFLOW);
  }

  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_build_value((uint64_t)1, odd);
}



void m1PowReal(void) {
  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, AM_NONE);
    return;
  }

  real_t x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realMultiply(const_pi, &x, &x, &ctxtReal39);
  WP34S_Cvt2RadSinCosTan(&x, AM_RADIAN, NULL, &x, NULL, &ctxtReal39);

  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void m1PowCplx(void) {
  real_t real, imag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &real);
  realMultiply(const_pi, &real, &real, &ctxtReal39);

  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &imag);
  realMultiply(const_pi, &imag, &imag, &ctxtReal39);

  uint8_t savedAngularMode = currentAngularMode;
  currentAngularMode = AM_RADIAN;

  cosComplex(&real, &imag, &real, &imag, &ctxtReal39);
  realToReal34(&real, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&imag, REGISTER_IMAG34_DATA(REGISTER_X));

  currentAngularMode = savedAngularMode;
}
