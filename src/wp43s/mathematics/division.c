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
 * \file division.c
 ***********************************************/

#include "wp43s.h"



void (* const division[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX |    regY ==>   1            2            3            4            5         6         7            8            9             10
//      V               Long integer Real34       Complex34    Time         Date      String    Real34 mat   Complex34 m  Short integer Config data
/*  1 Long integer  */ {divLonILonI, divRealLonI, divCplxLonI, divTimeLonI, divError, divError, divRemaLonI, divCxmaLonI, divShoILonI,  divError},
/*  2 Real34        */ {divLonIReal, divRealReal, divCplxReal, divTimeReal, divError, divError, divRemaReal, divCxmaReal, divShoIReal,  divError},
/*  3 Complex34     */ {divLonICplx, divRealCplx, divCplxCplx, divError,    divError, divError, divRemaCplx, divCxmaCplx, divShoICplx,  divError},
/*  4 Time          */ {divError,    divError,    divError,    divError,    divError, divError, divError,    divError,    divError,     divError},
/*  5 Date          */ {divError,    divError,    divError,    divError,    divError, divError, divError,    divError,    divError,     divError},
/*  6 String        */ {divError,    divError,    divError,    divError,    divError, divError, divError,    divError,    divError,     divError},
/*  7 Real34 mat    */ {divError,    divError,    divError,    divError,    divError, divError, divError,    divError,    divError,     divError},
/*  8 Complex34 mat */ {divError,    divError,    divError,    divError,    divError, divError, divError,    divError,    divError,     divError},
/*  9 Short integer */ {divLonIShoI, divRealShoI, divCplxShoI, divTimeShoI, divError, divError, divRemaShoI, divCxmaShoI, divShoIShoI,  divError},
/* 10 Config data   */ {divError,    divError,    divError,    divError,    divError, divError, divError,    divError,    divError,     divError}
};



/********************************************//**
 * \brief Data type error in division
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void divError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot divide %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnDivide:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and regY ÷ regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnDivide(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  division[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  adjustResult(REGISTER_X, true, true, REGISTER_X, REGISTER_Y, -1);
}



void divComplexComplex(const real_t *numerReal, const real_t *numerImag, const real_t *denomReal, const real_t *denomImag, real_t *quotientReal, real_t *quotientImag, realContext_t *realContext) {
  real_t realNumer, realDenom, a, b, c, d;

  realCopy(numerReal, &a);
  realCopy(numerImag, &b);
  realCopy(denomReal, &c);
  realCopy(denomImag, &d);

  if(realIsNaN(&a) || realIsNaN(&b) || realIsNaN(&c) || realIsNaN(&d)) {
    realCopy(const_NaN, quotientReal);
    realCopy(const_NaN, quotientImag);
    return;
  }

  if(realIsInfinite(&c) || realIsInfinite(&d)) {
    if(realIsInfinite(&a) || realIsInfinite(&b)) {
      realCopy(const_NaN, quotientReal);
      realCopy(const_NaN, quotientImag);
    }
    else {
      realZero(quotientReal);
      realZero(quotientImag);
    }
    return;
  }

  if(realIsInfinite(&a) && !realIsInfinite(&b)) {
    realZero(&b);
  }

  if(realIsInfinite(&b) && !realIsInfinite(&a)) {
    realZero(&a);
  }


  // Denominator
  realMultiply(&c, &c, &realDenom, realContext);                 // realDenom = c²
  realFMA(&d, &d, &realDenom, &realDenom, realContext);          // realDenom = c² + d²

  // real part
  realMultiply(&a, &c, &realNumer, realContext);                 // realNumer = a*c
  realFMA(&b, &d, &realNumer, &realNumer, realContext);          // realNumer = a*c + b*d
  realDivide(&realNumer, &realDenom, quotientReal, realContext); // realPart = (a*c + b*d) / (c² + d²) = realNumer / realDenom

  // imaginary part
  realMultiply(&b, &c, &realNumer, realContext);                 // realNumer = b*c
  realChangeSign(&a);                                            // a = -a
  realFMA(&a, &d, &realNumer, &realNumer, realContext);          // realNumer = b*c - a*d
  realDivide(&realNumer, &realDenom, quotientImag, realContext); // imagPart = (b*c - a*d) / (c² + d²) = realNumer / realDenom

}



void divRealComplex(const real_t *numerReal, const real_t *denomReal, const real_t *denomImag, real_t *quotientReal, real_t *quotientImag, realContext_t *realContext) {
  real_t a, c, d, denom;

  realCopy(numerReal, &a);
  realCopy(denomReal, &c);
  realCopy(denomImag, &d);

  if(realIsNaN(&a) || realIsNaN(&c) || realIsNaN(&d)) {
    realCopy(const_NaN, quotientReal);
    realCopy(const_NaN, quotientImag);
    return;
  }

  if(realIsInfinite(&c) || realIsInfinite(&d)) {
    realZero(quotientReal);
    realZero(quotientImag);
    return;
  }

  realMultiply(&c, &c, &denom, realContext);    // c²
  realFMA(&d, &d, &denom, &denom, realContext); // c² + d²

  // real part
  realMultiply(&a, &c, quotientReal, realContext);             // numer = a*c
  realDivide(quotientReal, &denom, quotientReal, realContext); // realPart  = (a*c) / (c² + d²) = numer / denom

  // imaginary part
  realChangeSign(&a);                                          // a = -a
  realMultiply(&a, &d, quotientImag, realContext);             // numer = -a*d
  realDivide(quotientImag, &denom, quotientImag, realContext); // imagPart  = -(a*d) / (c² + d²) = numer / denom
}



/******************************************************************************************************************************************************************************************/
/* long integer / ...                                                                                                                                                                     */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(long integer) ÷ X(long integer) ==> X(long integer or real16)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonILonI(void) {
  longInteger_t x;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  if(longIntegerIsZero(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divLonILonI:", "cannot divide a long integer by 0", NULL, NULL);
    #endif
  }
  else {
    longInteger_t y, quotient, remainder;

    convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);
    longIntegerInit(quotient);
    longIntegerInit(remainder);
    longIntegerDivideQuotientRemainder(y, x, quotient, remainder);

    if(longIntegerIsZero(remainder)) {
      convertLongIntegerToLongIntegerRegister(quotient, REGISTER_X);
    }
    else {
      real_t xIc, yIc;

      convertLongIntegerRegisterToReal(REGISTER_Y, &yIc, &ctxtReal39);
      convertLongIntegerRegisterToReal(REGISTER_X, &xIc, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

      realDivide(&yIc, &xIc, &xIc, &ctxtReal39);
      realToReal34(&xIc, REGISTER_REAL34_DATA(REGISTER_X));
    }

    longIntegerFree(quotient);
    longIntegerFree(remainder);
    longIntegerFree(y);
  }

  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) ÷ X(short integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonIShoI(void) {
  longInteger_t a, c;

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, a);
  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_X, REGISTER_X);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, c);

  if(longIntegerIsZero(c)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divLonIShoI:", "cannot divide a long integer by 0", NULL, NULL);
    #endif
  }
  else {
    longIntegerDivideQuotientRemainder(a, c, a, c);
    convertLongIntegerToLongIntegerRegister(a, REGISTER_X);
  }

  longIntegerFree(a);
  longIntegerFree(c);
}



/********************************************//**
 * \brief Y(short integer) ÷ X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoILonI(void) {
  longInteger_t a, c;

  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_Y, REGISTER_Y);
  convertLongIntegerRegisterToLongInteger(REGISTER_Y, a);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, c);

  if(longIntegerIsZero(c)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divShoILonI:", "cannot divide a short integer by 0", NULL, NULL);
    #endif
  }
  else {
    longIntegerDivideQuotientRemainder(a, c, a, c);
    convertLongIntegerToLongIntegerRegister(a, REGISTER_X);
  }

  longIntegerFree(a);
  longIntegerFree(c);
}



/********************************************//**
 * \brief Y(long integer) ÷ X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonIReal(void) {
  real_t y, x;

  setRegisterAngularMode(REGISTER_X, AM_NONE);
  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(realIsZero(&y)) {
      if(getSystemFlag(FLAG_SPCRES)) {
        realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divLonIReal:", "cannot divide 0 by 0", NULL, NULL);
        #endif
      }
    }
    else {
      if(getSystemFlag(FLAG_SPCRES)) {
        realToReal34((realIsPositive(&y) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divLonIReal:", "cannot divide a long integer by 0", NULL, NULL);
        #endif
      }
    }
  }

  else {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    realDivide(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(real34) ÷ X(long integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRealLonI(void) {
  real_t y, x;
  uint32_t yAngularMode;

  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  if(realIsZero(&x)) {
    if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y))) {
      if(getSystemFlag(FLAG_SPCRES)) {
        realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divLonIReal:", "cannot divide 0 by 0", NULL, NULL);
        #endif
      }
    }
    else {
      if(getSystemFlag(FLAG_SPCRES)) {
        realToReal34((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divLonIReal:", "cannot divide a real34 by 0", NULL, NULL);
        #endif
      }
    }
  }

  else {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    yAngularMode = getRegisterAngularMode(REGISTER_Y);

    if(yAngularMode == AM_NONE) {
      realDivide(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      if(currentAngularMode == AM_DMS) {
        convertAngleFromTo(&y, yAngularMode, AM_DEGREE, &ctxtReal39);
        realDivide(&y, &x, &x, &ctxtReal39);
        convertAngleFromTo(&x, AM_DEGREE, AM_DMS, &ctxtReal39);
        realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
        checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        convertAngleFromTo(&y, yAngularMode, currentAngularMode, &ctxtReal39);
        realDivide(&y, &x, &x, &ctxtReal39);
        realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      }
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
    }
  }
}



/********************************************//**
 * \brief Y(long integer) ÷ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divLonICplx(void) {
  real_t y, xReal, xImag;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

  divRealComplex(&y, &xReal, &xImag, &xReal, &xImag, &ctxtReal39);

  realToReal34(&xReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&xImag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) ÷ X(long integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCplxLonI(void) {
  real_t a, b, c;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToReal(REGISTER_X, &c, &ctxtReal39);

  realDivide(&a, &c, &a, &ctxtReal39);
  realDivide(&b, &c, &b, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* time / ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(time) ÷ X(long integer) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeLonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) ÷ X(short integer) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeShoI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) ÷ X(real34) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void divTimeReal(void) {
  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* date / ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/******************************************************************************************************************************************************************************************/
/* string / ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/******************************************************************************************************************************************************************************************/
/* real16 matrix / ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real16 matrix) ÷ X(long integer) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRemaLonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) ÷ X(short integer) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRemaShoI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) ÷ X(real34) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRemaReal(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) ÷ X(complex34) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divRemaCplx(void) {
  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* complex16 matrix / ...                                                                                                                                                                 */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex16 matrix) ÷ X(long integer) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCxmaLonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) ÷ X(short integer) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCxmaShoI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) ÷ X(real34) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCxmaReal(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) ÷ X(complex34) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void divCxmaCplx(void) {
  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* short integer / ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(short integer) ÷ X(short integer) ==> X(short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoIShoI(void) {
  int16_t sign;
  uint64_t value;

  convertShortIntegerRegisterToUInt64(REGISTER_X, &sign, &value);
  if(value == 0) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function divShoIShoI:", "cannot divide a short integer by 0", NULL, NULL);
    #endif
  }
  else {
    *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intDivide(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)), *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
    setRegisterShortIntegerBase(REGISTER_X, getRegisterShortIntegerBase(REGISTER_Y));
  }
}



/********************************************//**
 * \brief Y(short integer) ÷ X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoIReal(void) {
  real_t y, x;

  setRegisterAngularMode(REGISTER_X, AM_NONE);
  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(realIsZero(&y)) {
      if(getSystemFlag(FLAG_SPCRES)) {
        realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divShoIReal:", "cannot divide 0 by 0", NULL, NULL);
        #endif
      }
    }
    else {
      if(getSystemFlag(FLAG_SPCRES)) {
        realToReal34((realIsPositive(&y) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divShoIReal:", "cannot divide a short integer by 0", NULL, NULL);
        #endif
      }
    }
  }

  else {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    realDivide(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
}



/********************************************//**
 * \brief Y(real34) ÷ X(short integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRealShoI(void) {
  real_t y, x;
  uint32_t yAngularMode;

  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  if(realIsZero(&x)) {
    if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y))) {
      if(getSystemFlag(FLAG_SPCRES)) {
        realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divRealShoI:", "cannot divide 0 by 0", NULL, NULL);
        #endif
      }
    }
    else {
      if(getSystemFlag(FLAG_SPCRES)) {
        realToReal34((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function divRealShoI:", "cannot divide a real34 by 0", NULL, NULL);
        #endif
      }
    }
  }

  else {
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    yAngularMode = getRegisterAngularMode(REGISTER_Y);

    if(yAngularMode == AM_NONE) {
      realDivide(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      if(currentAngularMode == AM_DMS) {
        convertAngleFromTo(&y, yAngularMode, AM_DEGREE, &ctxtReal39);
        realDivide(&y, &x, &x, &ctxtReal39);
        convertAngleFromTo(&x, AM_DEGREE, AM_DMS, &ctxtReal39);
        realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
        checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        convertAngleFromTo(&y, yAngularMode, currentAngularMode, &ctxtReal39);
        realDivide(&y, &x, &x, &ctxtReal39);
        realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      }
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
    }
  }
}



/********************************************//**
 * \brief Y(short integer) ÷ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divShoICplx(void) {
  real_t y, xReal, xImag;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

  divRealComplex(&y, &xReal, &xImag, &xReal, &xImag, &ctxtReal39);

  realToReal34(&xReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&xImag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) ÷ X(short integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCplxShoI(void) {
  convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_Y)); // imaginary part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* real34 / ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real34) ÷ X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRealReal(void) {
  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getSystemFlag(FLAG_SPCRES)) {
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRealReal:", "cannot divide 0 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getSystemFlag(FLAG_SPCRES)) {
      realToReal34((real34IsPositive(REGISTER_REAL34_DATA(REGISTER_Y)) ? const_plusInfinity : const_minusInfinity), REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divRealReal:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else {
    real_t y, x;
    uint32_t yAngularMode, xAngularMode;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    yAngularMode = getRegisterAngularMode(REGISTER_Y);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    xAngularMode = getRegisterAngularMode(REGISTER_X);

    if(yAngularMode != AM_NONE && xAngularMode != AM_NONE) { // angle / angle
      if(yAngularMode == AM_DMS) {
        convertAngleFromTo(&y, AM_DMS, AM_DEGREE, &ctxtReal39);
        yAngularMode = AM_DEGREE;
      }
      if(xAngularMode == AM_DMS) {
        convertAngleFromTo(&x, AM_DMS, AM_DEGREE, &ctxtReal39);
        xAngularMode = AM_DEGREE;
      }
      convertAngleFromTo(&x, xAngularMode, yAngularMode, &ctxtReal39);
      realDivide(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      setRegisterAngularMode(REGISTER_X, AM_NONE);
    }
    else if(yAngularMode == AM_NONE) { // real / (real or angle)
      real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
      setRegisterAngularMode(REGISTER_X, AM_NONE);
    }
    else { // angle / real
      if(yAngularMode == AM_DMS) {
        convertAngleFromTo(&y, yAngularMode, AM_DEGREE, &ctxtReal39);
        yAngularMode = AM_DEGREE;
      }

      realDivide(&y, &x, &x, &ctxtReal39);

      convertAngleFromTo(&x, yAngularMode, currentAngularMode, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
      if(currentAngularMode == AM_DMS) {
        checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
      }
    }
  }
}



/********************************************//**
 * \brief Y(real34) ÷ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divRealCplx(void) {
  real_t y, xReal, xImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

  divRealComplex(&y, &xReal, &xImag, &xReal, &xImag, &ctxtReal39);

  realToReal34(&xReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&xImag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) ÷ X(real34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCplxReal(void) {
  real34Divide(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  real34Divide(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_Y)); // imaginary part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* complex34 / ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex34) ÷ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void divCplxCplx(void) {
  real_t yReal, yImag, xReal, xImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &yReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &yImag);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

  divComplexComplex(&yReal, &yImag, &xReal, &xImag, &xReal, &xImag, &ctxtReal39);

  realToReal34(&xReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&xImag, REGISTER_IMAG34_DATA(REGISTER_X));
}
