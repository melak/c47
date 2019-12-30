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
 * \file log2.c
 ***********************************************/

#include "wp43s.h"



void (* const logBase2[9])(void) = {
// regX ==> 1            2         3         4          5          6          7          8           9
//          Long integer Real34    complex34 Time       Date       String     Real34 mat Complex34 m Short integer
            log2LonI,    log2Real, log2Cplx, log2Error, log2Error, log2Error, log2Rema,  log2Cxma,   log2ShoI
};



/********************************************//**
 * \brief Data type error in log2
 *
 * \param void
 * \return void
 ***********************************************/
void log2Error(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate log2 for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnLog2:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and log2(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnLog2(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  logBase2[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



/**********************************************************************
 * In all the functions below:
 * if X is a number then X = a + ib
 * The variables a and b are used for intermediate calculations
 ***********************************************************************/

void log2LonI(void) {
  longInteger_t lgInt;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);

  if(longIntegerIsZero(lgInt)) {
    if(getFlag(FLAG_DANGER)) {
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      realToReal34(const_minusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2LonI:", "cannot calculate log2(0)", NULL, NULL);
      #endif
    }
  }
  else {
    real39_t x;

    convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);

    if(longIntegerIsPositive(lgInt)) {
      WP34S_Ln(&x, &x);
      realDivide(&x, const_ln2, &x, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
     }
    else if(getFlag(FLAG_CPXRES)) {
      realSetPositiveSign(&x);
      WP34S_Ln(&x, &x);
      realDivide(&x, const_ln2, &x, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      realDivide(const_pi, const_ln2, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    else if(getFlag(FLAG_DANGER)) {
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2LonI:", "cannot calculate log2 of a negative number when CPXRES is not set!", NULL, NULL);
      #endif
    }
  }

  longIntegerFree(lgInt);
}



void log2Rema(void) {
  fnToBeCoded();
}



void log2Cxma(void) {
  fnToBeCoded();
}



void log2ShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intLog2(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



void log2Real(void) {
  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      realToReal34(const_minusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2Real:", "cannot calculate log2(0)", NULL, NULL);
      #endif
    }
  }

  else if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    if(!getFlag(FLAG_DANGER)) {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2Real:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of log2 when flag D is not set", NULL, NULL);
      #endif
      return;
    }
    else {
      if(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X))) {
        realToReal34(const_plusInfinity, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else {
        realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      }
    }
  }

  else {
    real39_t a;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);
    if(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X))) {
      WP34S_Ln(&a, &a);
      realDivide(&a, const_ln2, &a, &ctxtReal39);
      realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
     }
    else if(getFlag(FLAG_CPXRES)) {
      realSetPositiveSign(&a);
      WP34S_Ln(&a, &a);
      realDivide(&a, const_ln2, &a, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
      realDivide(const_pi, const_ln2, &a, &ctxtReal39);
      realToReal34(&a, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    else if(getFlag(FLAG_DANGER)) {
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2Real:", "cannot calculate log2 of a negative number when CPXRES is not set!", NULL, NULL);
      #endif
    }
  }
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void log2Cplx(void) {
  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) && real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X))) {
    if(getFlag(FLAG_DANGER)) {
      realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X));
      realToReal34(const_NaN, REGISTER_IMAG34_DATA(REGISTER_X));
    }
    else {
      displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function log2Cplx:", "cannot calculate log2(0)", NULL, NULL);
      #endif
    }
  }
  else {
    real39_t a, b;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);
    real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &b);

    real39RectangularToPolar(&a, &b, &a, &b);
    WP34S_Ln(&a, &a);
    realDivide(&a, const_ln2, &a, &ctxtReal39);
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
    realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
    realDivide(&b, const_ln2, &b, &ctxtReal39);
    realToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
  }
}
