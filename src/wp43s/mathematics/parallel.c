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
 * \file parallel.c
 ***********************************************/

#include "wp43s.h"



void (* const parallel[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX |    regY ==>   1                 2                 3                 4              5              6              7              8              9              10
//      V               Long integer      Real34            Complex34         Time           Date           String         Real34 mat     Complex34 m    Short integer  Config data
/*  1 Long integer  */ {parallelLonILonI, parallelRealLonI, parallelCplxLonI, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError},
/*  2 Real34        */ {parallelLonIReal, parallelRealReal, parallelCplxReal, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError},
/*  3 Complex34     */ {parallelLonICplx, parallelRealCplx, parallelCplxCplx, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError},
/*  4 Time          */ {parallelError,    parallelError,    parallelError,    parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError},
/*  5 Date          */ {parallelError,    parallelError,    parallelError,    parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError},
/*  6 String        */ {parallelError,    parallelError,    parallelError,    parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError},
/*  7 Real34 mat    */ {parallelError,    parallelError,    parallelError,    parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError},
/*  8 Complex34 mat */ {parallelError,    parallelError,    parallelError,    parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError},
/*  9 Short integer */ {parallelError,    parallelError,    parallelError,    parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError},
/* 10 Config data   */ {parallelError,    parallelError,    parallelError,    parallelError, parallelError, parallelError, parallelError, parallelError, parallelError, parallelError}
};



/********************************************//**
 * \brief Data type error in parallel function
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void parallelError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot multiply %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnMultiply:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and regY || regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnParallel(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  parallel[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  adjustResult(REGISTER_X, true, true, REGISTER_X, REGISTER_Y, -1);
}



/******************************************************************************************************************************************************************************************/
/* long integer || ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(long integer) || X(long integer) ==> X(Real16)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelLonILonI(void) {
  real_t y, x, product;

  // y || x = xy / (x+y)
  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  if(!realIsZero(&x)) {
    realMultiply(&y, &x, &product, &ctxtReal39);
    realAdd(&y, &x, &y, &ctxtReal39);
    realDivide(&product, &y, &x, &ctxtReal39);
  }

  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(long integer) || X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelLonIReal(void) {
  real_t y, x, product;

  // y || x = xy / (x+y)
  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  if(!realIsZero(&x)) {
    realMultiply(&y, &x, &product, &ctxtReal39);
    realAdd(&y, &x, &y, &ctxtReal39);
    realDivide(&product, &y, &x, &ctxtReal39);
  }

  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real34) || X(long integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelRealLonI(void) {
  real_t y, x, product;

  // y || x = xy / (x+y)
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  if(!realIsZero(&x)) {
    realMultiply(&y, &x, &product, &ctxtReal39);
    realAdd(&y, &x, &y, &ctxtReal39);
    realDivide(&product, &y, &x, &ctxtReal39);
  }

  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(long integer) || X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelLonICplx(void) {
  real_t y;
  real_t xReal, productReal, sumReal;
  real_t xImag, productImag, sumImag;

  // y || x = xy / (x + y)
  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

  if(!realIsZero(&xReal) || !realIsZero(&xImag)) {
    realMultiply(&y, &xReal, &productReal, &ctxtReal39);
    realMultiply(&y, &xImag, &productImag, &ctxtReal39);
    realAdd(&y, &xReal, &sumReal, &ctxtReal39);
    realCopy(&xImag, &sumImag);
    divComplexComplex(&productReal, &productImag, &sumReal, &sumImag, &xReal, &xImag, &ctxtReal39);
  }

  realToReal34(&xReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&xImag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) || X(long integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelCplxLonI(void) {
  real_t x;
  real_t yReal, productReal, sumReal;
  real_t yImag, productImag, sumImag;

  // y || x = xy / (x + y)
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &yReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &yImag);
  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);

  if(!realIsZero(&yReal) || !realIsZero(&yImag)) {
    realMultiply(&x, &yReal, &productReal, &ctxtReal39);
    realMultiply(&x, &yImag, &productImag, &ctxtReal39);
    realAdd(&x, &yReal, &sumReal, &ctxtReal39);
    realCopy(&yImag, &sumImag);
    divComplexComplex(&productReal, &productImag, &sumReal, &sumImag, &yReal, &yImag, &ctxtReal39);
  }

  realToReal34(&yReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&yImag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* real34 || ...                                                                                                                                                                          */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real34) || X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelRealReal(void) {
  real_t y, x, product;

  // y || x = xy / (x+y)
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  if(!realIsZero(&x)) {
    realMultiply(&y, &x, &product, &ctxtReal39);
    realAdd(&y, &x, &y, &ctxtReal39);
    realDivide(&product, &y, &x, &ctxtReal39);
  }

  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



/********************************************//**
 * \brief Y(real34) || X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelRealCplx(void) {
  real_t y;
  real_t xReal, productReal, sumReal;
  real_t xImag, productImag, sumImag;

  // y || x = xy / (x + y)
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

  if(!realIsZero(&xReal) || !realIsZero(&xImag)) {
    realMultiply(&y, &xReal, &productReal, &ctxtReal39);
    realMultiply(&y, &xImag, &productImag, &ctxtReal39);
    realAdd(&y, &xReal, &sumReal, &ctxtReal39);
    realCopy(&xImag, &sumImag);
    divComplexComplex(&productReal, &productImag, &sumReal, &sumImag, &xReal, &xImag, &ctxtReal39);
  }

  realToReal34(&xReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&xImag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) || X(real34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelCplxReal(void) {
  real_t x;
  real_t yReal, productReal, sumReal;
  real_t yImag, productImag, sumImag;

  // y || x = xy / (x + y)
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &yReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &yImag);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);

  if(!realIsZero(&yReal) || !realIsZero(&yImag)) {
    realMultiply(&x, &yReal, &productReal, &ctxtReal39);
    realMultiply(&x, &yImag, &productImag, &ctxtReal39);
    realAdd(&x, &yReal, &sumReal, &ctxtReal39);
    realCopy(&yImag, &sumImag);
    divComplexComplex(&productReal, &productImag, &sumReal, &sumImag, &yReal, &yImag, &ctxtReal39);
  }

  realToReal34(&yReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&yImag, REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* complex34 || ...                                                                                                                                                                       */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex34) || X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void parallelCplxCplx(void) {
  real_t yReal, xReal, productReal, sumReal;
  real_t yImag, xImag, productImag, sumImag;

  // y || x = xy / (x + y)
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &yReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &yImag);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

  if(!realIsZero(&xReal) || !realIsZero(&xImag)) {
    mulComplexComplex(&yReal, &yImag, &xReal, &xImag, &productReal, &productImag, &ctxtReal39);
    realAdd(&yReal, &xReal, &sumReal, &ctxtReal39);
    realAdd(&yImag, &xImag, &sumImag, &ctxtReal39);
    divComplexComplex(&productReal, &productImag, &sumReal, &sumImag, &xReal, &xImag, &ctxtReal39);
  }

  realToReal34(&xReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&xImag, REGISTER_IMAG34_DATA(REGISTER_X));
}
