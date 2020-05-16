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
 * \file invert.c
 ***********************************************/

#include "wp43s.h"



void (* const invert[9])(void) = {
// regX ==> 1            2           3            4            5            6            7           8           9
//          Long integer Real34      complex34    Time         Date         String       Real34 mat  Complex34 m Short integer
            invertLonI,  invertReal, invertCplx,  invertError, invertError, invertError, invertRema, invertCxma, invertError
};



/********************************************//**
 * \brief Data type error in invert
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void invertError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot invert %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnInvert:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and 1 ÷ regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnInvert(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  invert[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



/********************************************//**
 * \brief 1 ÷ X(long integer) ==> X(long integer or real16)
 *
 * \param void
 * \return void
 ***********************************************/
void invertLonI(void) {
  longInteger_t a;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, a);

  if(longIntegerIsZero(a)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function invertLonI:", "cannot divide a long integer by 0", NULL, NULL);
    #endif
  }
  else {
    longInteger_t quotient, remainder, one;

    longIntegerInit(one);
    intToLongInteger(1, one);

    longIntegerInit(quotient);
    longIntegerInit(remainder);
    longIntegerDivideQuotientRemainder(one, a, quotient, remainder);

    if(longIntegerIsZero(remainder)) {
      convertLongIntegerToLongIntegerRegister(quotient, REGISTER_X);
    }
    else {
      real_t reX;

      longIntegerToAllocatedString(a, tmpStr3000, TMP_STR_LENGTH);
      stringToReal(tmpStr3000, &reX, &ctxtReal39);

      realDivide(const_1, &reX, &reX, &ctxtReal39);

      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      realToReal34(&reX, REGISTER_REAL34_DATA(REGISTER_X));
    }

    longIntegerFree(quotient);
    longIntegerFree(remainder);
    longIntegerFree(one);
  }

  longIntegerFree(a);
}



void invertRema(void) {
  fnToBeCoded();
}



void invertCxma(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief 1 ÷ X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void invertReal(void) {
  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getSystemFlag(FLAG_SPCRES)) {
      realToReal34(const_plusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function invertReal:", "cannot divide a real34 by 0", NULL, NULL);
      #endif
    }
  }

  else if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X)) && !getSystemFlag(FLAG_SPCRES)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function invertReal:", "cannot divide a real34 by " STD_PLUS_MINUS STD_INFINITY " when flag D is not set", NULL, NULL);
    #endif
  }

  else {
    real34Divide(const34_1, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief 1 ÷ X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void invertCplx(void) {
  real_t a, b;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  divRealComplex(const_1, &a, &b, &a, &b, &ctxtReal39);

  realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}
