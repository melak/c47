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
 * \file power.c
 ***********************************************/

#include "wp43s.h"



void (* const power[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX |    regY ==>   1            2            3            4         5         6         7            8            9             10
//      V               Long integer Real34       Complex34    Time      Date      String    Real34 mat   Complex34 m  Short integer Config data
/*  1 Long integer  */ {powLonILonI, powRealLonI, powCplxLonI, powError, powError, powError, powRemaLonI, powCxmaLonI, powShoILonI,  powError},
/*  2 Real34        */ {powLonIReal, powRealReal, powCplxReal, powError, powError, powError, powRemaReal, powCxmaReal, powShoIReal,  powError},
/*  3 Complex34     */ {powLonICplx, powRealCplx, powCplxCplx, powError, powError, powError, powRemaCplx, powCxmaCplx, powShoICplx,  powError},
/*  4 Time          */ {powError,    powError,    powError,    powError, powError, powError, powError,    powError,    powError,     powError},
/*  5 Date          */ {powError,    powError,    powError,    powError, powError, powError, powError,    powError,    powError,     powError},
/*  6 String        */ {powError,    powError,    powError,    powError, powError, powError, powError,    powError,    powError,     powError},
/*  7 Real34 mat    */ {powError,    powError,    powError,    powError, powError, powError, powError,    powError,    powError,     powError},
/*  8 Complex34 mat */ {powError,    powError,    powError,    powError, powError, powError, powError,    powError,    powError,     powError},
/*  9 Short integer */ {powLonIShoI, powRealShoI, powCplxShoI, powError, powError, powError, powRemaShoI, powCxmaShoI, powShoIShoI,  powError},
/* 10 Config data   */ {powError,    powError,    powError,    powError, powError, powError, powError,    powError,    powError,     powError}
};



/********************************************//**
 * \brief Data type error in power
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void powError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot raise %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "to %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnPower:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and regY ^ regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnPower(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  power[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  adjustResult(REGISTER_X, true, true, REGISTER_X, REGISTER_Y, -1);
}



/******************************************************************************************************************************************************************************************/
/* long integer ^ ...                                                                                                                                                                     */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(long integer) ^ X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonILonI(void) {
  int32_t exponentSign, baseSign;
  longInteger_t base, exponent;
  bool_t exponentIsOdd;

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, base);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, exponent);
  exponentIsOdd = longIntegerIsOdd(exponent);

  baseSign = longIntegerSign(base);
  longIntegerSetPositiveSign(base);

  exponentSign = longIntegerSign(exponent);
  longIntegerSetPositiveSign(exponent);

  if(longIntegerIsZero(exponent) && longIntegerIsZero(base)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powLonILonI: Cannot calculate 0^0!", NULL, NULL, NULL);
    #endif

    longIntegerFree(base);
    longIntegerFree(exponent);
    return;
  }

  if(longIntegerIsZero(exponent)) {
    uIntToLongInteger(1, base);
    convertLongIntegerToLongIntegerRegister(base, REGISTER_X);
    longIntegerFree(base);
    longIntegerFree(exponent);
    return;
  }
  else if(longIntegerIsZero(base)) {
    uIntToLongInteger(0, base);
    convertLongIntegerToLongIntegerRegister(base, REGISTER_X);
    longIntegerFree(base);
    longIntegerFree(exponent);
    return;
  }
  else if(exponentSign == -1) {
    uIntToLongInteger(0, base);
    convertLongIntegerToLongIntegerRegister(base, REGISTER_X);
    longIntegerFree(base);
    longIntegerFree(exponent);
    return;
  }

  longInteger_t power;

  longIntegerInit(power);
  uIntToLongInteger(1, power);

  for(uint32_t i=0; !longIntegerIsZero(exponent) && lastErrorCode == 0; i++) {
    if(longIntegerIsOdd(exponent)) {
     longIntegerMultiply(power, base, power);
    }

    longIntegerDivideUInt(exponent, 2, exponent);

    if(!longIntegerIsZero(exponent)) {
      longIntegerSquare(base, base);
    }
  }

  if(baseSign == -1 && exponentIsOdd) {
    longIntegerSetNegativeSign(power);
  }

  convertLongIntegerToLongIntegerRegister(power, REGISTER_X);

  longIntegerFree(power);
  longIntegerFree(base);
  longIntegerFree(exponent);
}



/********************************************//**
 * \brief Y(long integer) ^ X(short integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonIShoI(void) {
  int32_t exponentSign, baseSign;
  longInteger_t base, exponent;
  bool_t exponentIsOdd;

  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_X, REGISTER_X);
  convertLongIntegerRegisterToLongInteger(REGISTER_Y, base);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, exponent);
  exponentIsOdd = longIntegerIsOdd(exponent);

  baseSign = longIntegerSign(base);
  longIntegerSetPositiveSign(base);

  exponentSign = longIntegerSign(exponent);
  longIntegerSetPositiveSign(exponent);

  if(longIntegerIsZero(exponent) && longIntegerIsZero(base)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powLonIShoI: Cannot calculate 0^0!", NULL, NULL, NULL);
    #endif

    longIntegerFree(base);
    longIntegerFree(exponent);
    return;
  }

  if(longIntegerIsZero(exponent)) {
    uIntToLongInteger(1, base);
    convertLongIntegerToLongIntegerRegister(base, REGISTER_X);
    longIntegerFree(base);
    longIntegerFree(exponent);
    return;
  }
  else if(longIntegerIsZero(base)) {
    uIntToLongInteger(0, base);
    convertLongIntegerToLongIntegerRegister(base, REGISTER_X);
    longIntegerFree(base);
    longIntegerFree(exponent);
    return;
  }
  else if(exponentSign == -1) {
    uIntToLongInteger(0, base);
    convertLongIntegerToLongIntegerRegister(base, REGISTER_X);
    longIntegerFree(base);
    longIntegerFree(exponent);
    return;
  }

  longInteger_t power;

  longIntegerInit(power);
  uIntToLongInteger(1, power);

  for(uint32_t i=0; !longIntegerIsZero(exponent) && lastErrorCode == 0; i++) {
    if(longIntegerIsOdd(exponent)) {
     longIntegerMultiply(power, base, power);
    }

    longIntegerDivideUInt(exponent, 2, exponent);

    if(!longIntegerIsZero(exponent)) {
      longIntegerSquare(base, base);
    }
  }

  if(baseSign == -1 && exponentIsOdd) {
    longIntegerSetNegativeSign(power);
  }

  convertLongIntegerToLongIntegerRegister(power, REGISTER_X);

  longIntegerFree(power);
  longIntegerFree(base);
  longIntegerFree(exponent);
}



/********************************************//**
 * \brief Y(short integer) ^ X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoILonI(void) {
  int32_t exponentSign, baseSign;
  longInteger_t base, exponent;
  bool_t exponentIsOdd;

  convertShortIntegerRegisterToLongInteger(REGISTER_Y, base);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, exponent);
  exponentIsOdd = longIntegerIsOdd(exponent);

  baseSign = longIntegerSign(base);
  longIntegerSetPositiveSign(base);

  exponentSign = longIntegerSign(exponent);
  longIntegerSetPositiveSign(exponent);

  if(longIntegerIsZero(exponent) && longIntegerIsZero(base)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function powShoILonI: Cannot calculate 0^0!", NULL, NULL, NULL);
    #endif

    longIntegerFree(base);
    longIntegerFree(exponent);
    return;
  }

  if(longIntegerIsZero(exponent)) {
    uIntToLongInteger(1, base);
    convertLongIntegerToLongIntegerRegister(base, REGISTER_X);
    longIntegerFree(base);
    longIntegerFree(exponent);
    return;
  }
  else if(longIntegerIsZero(base)) {
    uIntToLongInteger(0, base);
    convertLongIntegerToLongIntegerRegister(base, REGISTER_X);
    longIntegerFree(base);
    longIntegerFree(exponent);
    return;
  }
  else if(exponentSign == -1) {
    uIntToLongInteger(0, base);
    convertLongIntegerToLongIntegerRegister(base, REGISTER_X);
    longIntegerFree(base);
    longIntegerFree(exponent);
    return;
  }

  longInteger_t power;

  longIntegerInit(power);
  uIntToLongInteger(1, power);

  for(uint32_t i=0; !longIntegerIsZero(exponent); i++) {
    if(longIntegerIsOdd(exponent)) {
     longIntegerMultiply(power, base, power);
    }

    longIntegerDivideUInt(exponent, 2, exponent);

    if(!longIntegerIsZero(exponent)) {
      longIntegerSquare(base, base);
    }
  }

  if(baseSign == -1 && exponentIsOdd) {
    longIntegerSetNegativeSign(power);
  }

  convertLongIntegerToLongIntegerRegister(power, REGISTER_X);

  longIntegerFree(power);
  longIntegerFree(base);
  longIntegerFree(exponent);
}



/********************************************//**
 * \brief Y(long integer) ^ X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonIReal(void) {
  real_t y, x;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realPower(&y, &x, &x, &ctxtReal39);

  if(getFlag(FLAG_CPXRES) && realIsNaN(&x)) {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));

    reallocateRegister(REGISTER_Y, dtComplex34, COMPLEX34_SIZE, AM_NONE);
    realToReal34(&y, REGISTER_REAL34_DATA(REGISTER_Y));
    real34Zero(REGISTER_IMAG34_DATA(REGISTER_Y));

    powCplxCplx();
    return;
  }

  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real34) ^ X(long integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRealLonI(void) {
  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_Y))) {
    if(longIntegerIsZeroRegister(REGISTER_X)) {
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      if(getRegisterLongIntegerSign(REGISTER_X) == LI_POSITIVE) {
        longInteger_t lgInt;
        convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);
        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
        if(longIntegerIsEven(lgInt)) {
          realToReal34(const_plusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
        }
        else {
          realToReal34(const_minusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
        }
        longIntegerFree(lgInt);
      }
      else {
        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
        realToReal34(const_plusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
      }
    }
    return;
  }

  real_t y, x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  realPower(&y, &x, &x, &ctxtReal39);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(long integer) ^ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powLonICplx(void) {
  real_t y;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);

  reallocateRegister(REGISTER_Y, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(&y, REGISTER_REAL34_DATA(REGISTER_Y));
  real34Zero(REGISTER_IMAG34_DATA(REGISTER_Y));

  powCplxCplx();
}



/********************************************//**
 * \brief Y(complex34) ^ X(long integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCplxLonI(void) {
/*  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsInfinite(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    if(longIntegerIsZeroRegister(REGISTER_X)) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(const_NaN, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    else {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(const_plusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(const_plusInfinity, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    return;
  }*/

  real_t a, b, c, d;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToReal(REGISTER_X, &c, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);

  // ln(a + bi) --> a + bi
  realRectangularToPolar(&a, &b, &a, &b, &ctxtReal39);
  WP34S_Ln(&a, &a, &ctxtReal39);

  // c * ln(a + bi) -- > a + bi
  realMultiply(&c, &a, &a, &ctxtReal39);
  realMultiply(&c, &b, &b, &ctxtReal39);

  // exp(c * ln(a + bi)) -- > c + di
  realExp(&a, &a, &ctxtReal39);
  realPolarToRectangular(const_1, &b, &c, &d, &ctxtReal39);
  realMultiply(&a, &c, &c, &ctxtReal39);
  realMultiply(&a, &d, &d, &ctxtReal39);

  realToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* time ^ ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/******************************************************************************************************************************************************************************************/
/* date ^ ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/******************************************************************************************************************************************************************************************/
/* string ^ ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/******************************************************************************************************************************************************************************************/
/* real16 matrix ^ ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real16 matrix) ^ X(long integer) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRemaLonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) ^ X(short integer) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRemaShoI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) ^ X(real34) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRemaReal(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) ^ X(complex34) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powRemaCplx(void) {
  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* complex16 matrix ^ ...                                                                                                                                                                 */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex16 matrix) ^ X(long integer) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCxmaLonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) ^ X(short integer) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCxmaShoI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) ^ X(real34) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCxmaReal(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) ^ X(complex34) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void powCxmaCplx(void) {
  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* short integer ^ ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(short integer) ^ X(short integer) ==> X(short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoIShoI(void) {
  setRegisterShortIntegerBase(REGISTER_X, getRegisterShortIntegerBase(REGISTER_Y));
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intPower(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)), *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



/********************************************//**
 * \brief Y(short integer) ^ X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoIReal(void) {
  real_t y, x;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realPower(&y, &x, &x, &ctxtReal39);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real34) ^ X(short integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRealShoI(void) {
  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_Y))) {
    if(shortIntegerIsZero(REGISTER_LONG_INTEGER_DATA(REGISTER_X))) {
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      realToReal34(const_plusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
    }
    return;
    setRegisterAngularMode(REGISTER_X, AM_NONE);
  }

  real_t y, x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  realPower(&y, &x, &x, &ctxtReal39);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(short integer) ^ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powShoICplx(void) {
  real_t a, c, d;

  convertShortIntegerRegisterToReal(REGISTER_Y, &a, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &c);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &d);

  // ln(a) --> a
  WP34S_Ln(&a, &a, &ctxtReal39);

  // (c + di) * ln(a) --> c +di
  realMultiply(&a, &c, &c, &ctxtReal39);
  realMultiply(&a, &d, &d, &ctxtReal39);

  // exp((c + di) * ln(a)) --> c +di
  realExp(&c, &a, &ctxtReal39);
  realPolarToRectangular(const_1, &d, &c, &d, &ctxtReal39);
  realMultiply(&a, &c, &c, &ctxtReal39);
  realMultiply(&a, &d, &d, &ctxtReal39);

  realToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) ^ X(short integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCplxShoI(void) {
/*  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsInfinite(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    if(shortIntegerIsZero(REGISTER_LONG_INTEGER_DATA(REGISTER_X))) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(const_NaN, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    else {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(const_plusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(const_plusInfinity, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    return;
  }*/

  real_t a, b, c, d;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  convertShortIntegerRegisterToReal(REGISTER_X, &c, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);

  // ln(a + bi) --> a + bi
  realRectangularToPolar(&a, &b, &a, &b, &ctxtReal39);
  WP34S_Ln(&a, &a, &ctxtReal39);

  // c * ln(a + bi) -- > a + bi
  realMultiply(&c, &a, &a, &ctxtReal39);
  realMultiply(&c, &b, &b, &ctxtReal39);

  // exp(c * ln(a + bi)) -- > c + di
  realExp(&a, &a, &ctxtReal39);
  realPolarToRectangular(const_1, &b, &c, &d, &ctxtReal39);
  realMultiply(&a, &c, &c, &ctxtReal39);
  realMultiply(&a, &d, &d, &ctxtReal39);

  realToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* real34 ^ ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real34) ^ X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRealReal(void) {
  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_Y))) {
    if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      if(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X)) && real34IsAnInteger(REGISTER_REAL34_DATA(REGISTER_X))) {
        longInteger_t lgInt;
        convertReal34ToLongInteger(REGISTER_REAL34_DATA(REGISTER_X), lgInt, DEC_ROUND_DOWN);
        if(longIntegerIsEven(lgInt)) {
          realToReal34(const_plusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
        }
        else {
          realToReal34(const_minusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
        }
        longIntegerFree(lgInt);
      }
      else {
        realToReal34(const_plusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
      }
    }
    setRegisterAngularMode(REGISTER_X, AM_NONE);
    return;
  }

  real_t y, x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realPower(&y, &x, &x, &ctxtReal39);

  if(getFlag(FLAG_CPXRES) && realIsNaN(&x)) {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));

    reallocateRegister(REGISTER_Y, dtComplex34, COMPLEX34_SIZE, AM_NONE);
    realToReal34(&y, REGISTER_REAL34_DATA(REGISTER_Y));
    real34Zero(REGISTER_IMAG34_DATA(REGISTER_Y));

    powCplxCplx();
    return;
  }

  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real34) ^ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powRealCplx(void) {
/*  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_Y))) {
    if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) && real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X))) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(const_NaN, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    else {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(const_plusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(const_plusInfinity, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    return;
  }*/

  real_t y;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  reallocateRegister(REGISTER_Y, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(&y, REGISTER_REAL34_DATA(REGISTER_Y));
  real34Zero(REGISTER_IMAG34_DATA(REGISTER_Y));

  powCplxCplx();
}



/********************************************//**
 * \brief Y(complex34) ^ X(real34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCplxReal(void) {
/*  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsInfinite(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(const_NaN, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    else {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(const_plusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(const_plusInfinity, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    return;
  }*/

  real_t x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));

  powCplxCplx();
}



/******************************************************************************************************************************************************************************************/
/* complex34 + ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex34) ^ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void powCplxCplx(void) {
  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsInfinite(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) && real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X))) {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(const_NaN, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    else {
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(const_plusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(const_plusInfinity, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    return;
  }

  real_t a, b, c, d, theta;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &c);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &d);

  realRectangularToPolar(&a, &b, &a, &theta, &ctxtReal39);
  WP34S_Ln(&a, &a, &ctxtReal39);

  realMultiply(&a, &d, &b, &ctxtReal39);
  realFMA(&theta, &c, &b, &b, &ctxtReal39);
  realChangeSign(&theta);
  realMultiply(&a, &c, &a, &ctxtReal39);
  realFMA(&theta, &d, &a, &a, &ctxtReal39);

  realExp(&a, &c, &ctxtReal39);
  realPolarToRectangular(const_1, &b, &a, &b, &ctxtReal39);
  realMultiply(&c, &b, &d, &ctxtReal39);
  realMultiply(&c, &a, &c, &ctxtReal39);

  realToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&d, REGISTER_IMAG34_DATA(REGISTER_X));
}
