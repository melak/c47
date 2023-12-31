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
 * \file multiplication.c
 ***********************************************/

#include "mathematics/multiplication.h"

#include "constantPointers.h"
#include "conversionAngles.h"
#include "debug.h"
#include "error.h"
#include "fonts.h"
#include "integers.h"
#include "items.h"
#include "mathematics/matrix.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43.h"



TO_QSPI void (* const multiplication[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX |    regY ==>   1            2            3            4            5         6         7            8            9             10
//      V               Long integer Real34       Complex34    Time         Date      String    Real34 mat   Complex34 m  Short integer Config data
/*  1 Long integer  */ {mulLonILonI, mulRealLonI, mulCplxLonI, mulTimeLonI, mulError, mulError, mulRemaLonI, mulCxmaLonI, mulShoILonI,  mulError},
/*  2 Real34        */ {mulLonIReal, mulRealReal, mulCplxReal, mulTimeReal, mulError, mulError, mulRemaReal, mulCxmaReal, mulShoIReal,  mulError},
/*  3 Complex34     */ {mulLonICplx, mulRealCplx, mulCplxCplx, mulError,    mulError, mulError, mulRemaCplx, mulCxmaCplx, mulShoICplx,  mulError},
/*  4 Time          */ {mulLonITime, mulRealTime, mulError,    mulError,    mulError, mulError, mulError,    mulError,    mulShoITime,  mulError},
/*  5 Date          */ {mulError,    mulError,    mulError,    mulError,    mulError, mulError, mulError,    mulError,    mulError,     mulError},
/*  6 String        */ {mulError,    mulError,    mulError,    mulError,    mulError, mulError, mulError,    mulError,    mulError,     mulError},
/*  7 Real34 mat    */ {mulLonIRema, mulRealRema, mulCplxRema, mulError,    mulError, mulError, mulRemaRema, mulCxmaRema, mulShoIRema,  mulError},
/*  8 Complex34 mat */ {mulLonICxma, mulRealCxma, mulCplxCxma, mulError,    mulError, mulError, mulRemaCxma, mulCxmaCxma, mulShoICxma,  mulError},
/*  9 Short integer */ {mulLonIShoI, mulRealShoI, mulCplxShoI, mulTimeShoI, mulError, mulError, mulRemaShoI, mulCxmaShoI, mulShoIShoI,  mulError},
/* 10 Config data   */ {mulError,    mulError,    mulError,    mulError,    mulError, mulError, mulError,    mulError,    mulError,     mulError}
};



/********************************************//**
 * \brief Data type error in multiplication
 *
 * \param[in] unusedButMandatoryParameter
 * \return void
 ***********************************************/
#if(EXTRA_INFO_ON_CALC_ERROR == 1)
  void mulError(void) {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    sprintf(errorMessage, "cannot multiply %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "by %s", getRegisterDataTypeName(REGISTER_X, true, false));
    moreInfoOnError("In function fnMultiply:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  }
#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)



/********************************************//**
 * \brief regX ==> regL and regY × regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter
 * \return void
 ***********************************************/
void fnMultiply(uint16_t unusedButMandatoryParameter) {
  if(!saveLastX()) {
    return;
  }

  multiplication[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  adjustResult(REGISTER_X, true, true, REGISTER_X, REGISTER_Y, -1);
}



void mulComplexi(const real_t *inReal, const real_t *inImag, real_t *productReal, real_t *productImag) {
  real_t tmpI;

  realCopy(inImag, &tmpI);
  realChangeSign(&tmpI);
  realCopy(inReal, productImag);
  realCopy(&tmpI, productReal);
}






void mulComplexComplex(const real_t *factor1Real, const real_t *factor1Imag, const real_t *factor2Real, const real_t *factor2Imag, real_t *productReal, real_t *productImag, realContext_t *realContext) {
  real_t a, b, c, d, p, t;
  bool_t aIsZero, bIsZero, cIsZero, dIsZero, aIsInfinite, bIsInfinite, cIsInfinite, dIsInfinite;

  realCopy(factor1Real, &a);
  aIsZero = realIsZero(&a);
  aIsInfinite = realIsInfinite(&a);

  realCopy(factor1Imag, &b);
  bIsZero = realIsZero(&b);
  bIsInfinite = realIsInfinite(&b);

  realCopy(factor2Real, &c);
  cIsZero = realIsZero(&c);
  cIsInfinite = realIsInfinite(&c);

  realCopy(factor2Imag, &d);
  dIsZero = realIsZero(&d);
  dIsInfinite = realIsInfinite(&d);

  if(   realIsNaN(&a) || realIsNaN(&b) || realIsNaN(&c) || realIsNaN(&d)
     || (aIsZero && bIsZero && (cIsInfinite || dIsInfinite))
     || (cIsZero && dIsZero && (aIsInfinite || bIsInfinite))) {
    realCopy(const_NaN, productReal);
    realCopy(const_NaN, productImag);
    return;
  }

  if((aIsInfinite || bIsInfinite) && (cIsInfinite || dIsInfinite)) { // perform multiplication in polar form
    setInfiniteComplexAngle((getInfiniteComplexAngle(&a, &b) + getInfiniteComplexAngle(&c, &d)) % 8, productReal, productImag);
  }
  // Not sure the following lines are needed
  //else if(aIsInfinite || bIsInfinite || cIsInfinite || dIsInfinite) {
  //  realRectangularToPolar(&a, &b, &a, &b, realContext);
  //  realRectangularToPolar(&c, &d, &c, &d, realContext);
  //  realMultiply(&a, &c, &a, realContext);
  //  realAdd(&b, &d, &b, realContext);
  //  realPolarToRectangular(&a, &b, productReal, productImag, realContext);
  //}
  else { // perform multiplication in rectangular form using numerically stable approach
    // real part
    realMultiply(&a, &c, &p, realContext);                  // RN(ac)
    realChangeSign(&b);
    realFMA(&b, &d, &p, &t, realContext);                   // RN(p - bd)
    realChangeSign(&b);
    realChangeSign(&p);
    realFMA(&a, &c, &p, productReal, realContext);          // RN(ac - p)
    realAdd(productReal, &t, productReal, realContext);     // RN(RN(p - bd) + RN(ac - p))

    // imaginary part
    realMultiply(&a, &d, &p, realContext);                  // RN(ad)
    realFMA(&b, &c, &p, &t, realContext);                   // RN(bc + p)
    realChangeSign(&p);
    realFMA(&a, &d, &p, productImag, realContext);          // RN(ad - p)
    realAdd(productImag, &t, productImag, realContext);     // RN(RN(bc + p) + RN(ad - p))
  }
}

/******************************************************************************************************************************************************************************************/
/* long integer × ...                                                                                                                                                                     */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(long integer) × X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonILonI(void) {
  longInteger_t y, x;

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  longIntegerMultiply(y, x, x);

  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

  longIntegerFree(y);
  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) × X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonITime(void) {
  real_t y, x;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realMultiply(&y, &x, &x, &ctxtReal39);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(time) × X(long integer) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulTimeLonI(void) {
  real_t y, x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtTime, REAL34_SIZE, amNone);

  realMultiply(&y, &x, &x, &ctxtReal39);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(long integer) × X(real34 matrix) ==> X(real34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonIRema(void) {
  real34Matrix_t matrix, res;
  real_t y;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  linkToRealMatrixRegister(REGISTER_X, &matrix);
  _multiplyRealMatrix(&matrix, &y, &res, &ctxtReal39);
  convertReal34MatrixToReal34MatrixRegister(&res, REGISTER_X);
  realMatrixFree(&res);
}



/********************************************//**
 * \brief Y(real34 matrix) × X(long integer) ==> X(real34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRemaLonI(void) {
  real34Matrix_t matrix, res;
  real_t x;

  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  linkToRealMatrixRegister(REGISTER_Y, &matrix);
  _multiplyRealMatrix(&matrix, &x, &res, &ctxtReal39);
  convertReal34MatrixToReal34MatrixRegister(&res, REGISTER_X);
  realMatrixFree(&res);
}



/********************************************//**
 * \brief Y(long integer) × X(complex34 matrix) ==> X(complex34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonICxma(void) {
  complex34Matrix_t matrix, res;
  real_t y;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  linkToComplexMatrixRegister(REGISTER_X, &matrix);
  _multiplyComplexMatrix(&matrix, &y, const_0, &res, &ctxtReal39);
  convertComplex34MatrixToComplex34MatrixRegister(&res, REGISTER_X);
  complexMatrixFree(&res);
}



/********************************************//**
 * \brief Y(complex34 matrix) × X(long integer) ==> X(complex34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCxmaLonI(void) {
  complex34Matrix_t matrix, res;
  real_t y;

  convertLongIntegerRegisterToReal(REGISTER_X, &y, &ctxtReal39);
  linkToComplexMatrixRegister(REGISTER_Y, &matrix);
  _multiplyComplexMatrix(&matrix, &y, const_0, &res, &ctxtReal39);
  convertComplex34MatrixToComplex34MatrixRegister(&res, REGISTER_X);
  complexMatrixFree(&res);
}



/********************************************//**
 * \brief Y(long integer) × X(short integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonIShoI(void) {
  longInteger_t y, x;

  convertShortIntegerRegisterToLongIntegerRegister(REGISTER_X, REGISTER_X);

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  longIntegerMultiply(y, x, x);

  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

  longIntegerFree(y);
  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(short integer) × X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoILonI(void) {
  longInteger_t y, x;

  convertShortIntegerRegisterToLongInteger(REGISTER_Y, y);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  longIntegerMultiply(y, x, x);

  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

  longIntegerFree(y);
  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) × X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonIReal(void) {
  real_t y, x;
  angularMode_t xAngularMode;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == amNone) {
    realMultiply(&y, &x, &x, &ctxtReal39);
    convertRealToReal34ResultRegister(&x, REGISTER_X);
  }
  else {
    convertAngleFromTo(&x, xAngularMode, currentAngularMode, &ctxtReal39);
    realMultiply(&y, &x, &x, &ctxtReal39);
    convertRealToReal34ResultRegister(&x, REGISTER_X);
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) × X(long integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRealLonI(void) {
  real_t y, x;
  angularMode_t yAngularMode;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);

  if(yAngularMode == amNone) {
    realMultiply(&y, &x, &x, &ctxtReal39);
    convertRealToReal34ResultRegister(&x, REGISTER_X);
  }
  else {
    convertAngleFromTo(&y, yAngularMode, currentAngularMode, &ctxtReal39);
    realMultiply(&y, &x, &x, &ctxtReal39);
    convertRealToReal34ResultRegister(&x, REGISTER_X);
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(long integer) × X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulLonICplx(void) {
  real_t a, c, d;

  convertLongIntegerRegisterToReal(REGISTER_Y, &a, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &c);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &d);

  realMultiply(&c, &a, &c, &ctxtReal39);
  realMultiply(&d, &a, &d, &ctxtReal39);

  convertRealToReal34ResultRegister(&c, REGISTER_X);
  convertRealToImag34ResultRegister(&d, REGISTER_X);
}



/********************************************//**
 * \brief Y(complex34) × X(long integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCplxLonI(void) {
  real_t a, b, c;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToReal(REGISTER_X, &c, &ctxtReal39);

  realMultiply(&a, &c, &a, &ctxtReal39);
  realMultiply(&b, &c, &b, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
  convertRealToReal34ResultRegister(&a, REGISTER_X);
  convertRealToImag34ResultRegister(&b, REGISTER_X);
}



/******************************************************************************************************************************************************************************************/
/* time × ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(time) × X(short integer) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulTimeShoI(void) {
  real_t y, x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtTime, REAL34_SIZE, amNone);

  realMultiply(&y, &x, &x, &ctxtReal39);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(short integer) × X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoITime(void) {
  real_t y, x;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realMultiply(&y, &x, &x, &ctxtReal39);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(time) × X(real34) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulTimeReal(void) {
  real34_t b;
  angularMode_t xAngularMode;

  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == amNone) {
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &b);
    reallocateRegister(REGISTER_X, dtTime, REAL34_SIZE, amNone);
    real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), &b, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    mulError();
  }
}



/********************************************//**
 * \brief Y(real34) × X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRealTime(void) {
  angularMode_t yAngularMode;

  yAngularMode = getRegisterAngularMode(REGISTER_Y);

  if(yAngularMode == amNone) {
    real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    mulError();
  }
}



/******************************************************************************************************************************************************************************************/
/* time × ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/



/******************************************************************************************************************************************************************************************/
/* string × ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/



/******************************************************************************************************************************************************************************************/
/* real34 matrix × ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real34 matrix) × X(real34 matrix) ==> X(real34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRemaRema(void) {
  real34Matrix_t y, x, res;

  linkToRealMatrixRegister(REGISTER_Y, &y);
  linkToRealMatrixRegister(REGISTER_X, &x);

  multiplyRealMatrices(&y, &x, &res);
  if(res.matrixElements) {
    convertReal34MatrixToReal34MatrixRegister(&res, REGISTER_X);
    realMatrixFree(&res);
  }
  else {
    displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
    #if(EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot multiply %d" STD_CROSS "%d-matrix and %d" STD_CROSS "%d-matrix",
              y.header.matrixRows, y.header.matrixColumns,
              x.header.matrixRows, x.header.matrixColumns);
      moreInfoOnError("In function mulRemaRema:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}



/********************************************//**
 * \brief Y(real34 matrix) × X(complex34 matrix) ==> X(complex34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRemaCxma(void) {
  convertReal34MatrixRegisterToComplex34MatrixRegister(REGISTER_Y, REGISTER_Y);
  mulCxmaCxma();
}



/********************************************//**
 * \brief Y(complex34 matrix) × X(real34 matrix) ==> X(complex34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCxmaRema(void) {
  convertReal34MatrixRegisterToComplex34MatrixRegister(REGISTER_X, REGISTER_X);
  mulCxmaCxma();
}



/********************************************//**
 * \brief Y(real34 matrix) × X(short integer) ==> X(real34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRemaShoI(void) {
  real34Matrix_t matrix, res;
  real_t x;

  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  linkToRealMatrixRegister(REGISTER_Y, &matrix);
  _multiplyRealMatrix(&matrix, &x, &res, &ctxtReal39);
  convertReal34MatrixToReal34MatrixRegister(&res, REGISTER_X);
  realMatrixFree(&res);
}



/********************************************//**
 * \brief Y(short integer) × X(real34 matrix) ==> X(real34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoIRema(void) {
  real34Matrix_t matrix, res;
  real_t y;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);

  linkToRealMatrixRegister(REGISTER_X, &matrix);
  _multiplyRealMatrix(&matrix, &y, &res, &ctxtReal39);
  convertReal34MatrixToReal34MatrixRegister(&res, REGISTER_X);
  realMatrixFree(&res);
}



/********************************************//**
 * \brief Y(real34 matrix) × X(real34) ==> X(real34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRemaReal(void) {
  real34Matrix_t matrix;
  if(getRegisterAngularMode(REGISTER_X) == amNone) {
    linkToRealMatrixRegister(REGISTER_Y, &matrix);
    multiplyRealMatrix(&matrix, REGISTER_REAL34_DATA(REGISTER_X), &matrix);
    convertReal34MatrixToReal34MatrixRegister(&matrix, REGISTER_X);
  }
  else {
    elementwiseRemaReal(mulRealReal);
  }
}



/********************************************//**
 * \brief Y(real34) × X(real34 matrix) ==> X(real34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRealRema(void) {
  real34Matrix_t matrix;
  if(getRegisterAngularMode(REGISTER_Y) == amNone) {
    linkToRealMatrixRegister(REGISTER_X, &matrix);
    multiplyRealMatrix(&matrix, REGISTER_REAL34_DATA(REGISTER_Y), &matrix);
  }
  else {
    elementwiseRealRema(mulRealReal);
  }
}



/********************************************//**
 * \brief Y(real34 matrix) × X(complex34) ==> X(complex34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRemaCplx(void) {
  convertReal34MatrixRegisterToComplex34MatrixRegister(REGISTER_Y, REGISTER_Y);
  mulCxmaCplx();
}



/********************************************//**
 * \brief Y(complex34) × X(real34 matrix) ==> X(complex34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCplxRema(void) {
  convertReal34MatrixRegisterToComplex34MatrixRegister(REGISTER_X, REGISTER_X);
  mulCplxCxma();
}



/******************************************************************************************************************************************************************************************/
/* complex34 matrix × ...                                                                                                                                                                 */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex34 matrix) × X(complex34 matrix) ==> X(complex34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCxmaCxma(void) {
  complex34Matrix_t y, x, res;

  linkToComplexMatrixRegister(REGISTER_Y, &y);
  linkToComplexMatrixRegister(REGISTER_X, &x);

  multiplyComplexMatrices(&y, &x, &res);
  if(res.matrixElements) {
    convertComplex34MatrixToComplex34MatrixRegister(&res, REGISTER_X);
    complexMatrixFree(&res);
  }
  else {
    displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
    #if(EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot multiply %d" STD_CROSS "%d-matrix and %d" STD_CROSS "%d-matrix",
              y.header.matrixRows, y.header.matrixColumns,
              x.header.matrixRows, x.header.matrixColumns);
      moreInfoOnError("In function mulCxmaCxma:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}



/********************************************//**
 * \brief Y(complex34 matrix) × X(short integer) ==> X(complex34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCxmaShoI(void) {
  convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  mulCxmaReal();
}



/********************************************//**
 * \brief Y(short integer) × X(complex34 matrix) ==> X(complex34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoICxma(void) {
  convertShortIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
  mulRealCxma();
}



/********************************************//**
 * \brief Y(complex34 matrix) × X(real34) ==> X(complex34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCxmaReal(void) {
  complex34Matrix_t matrix;
  if(getRegisterAngularMode(REGISTER_X) == amNone) {
    linkToComplexMatrixRegister(REGISTER_Y, &matrix);
    multiplyComplexMatrix(&matrix, REGISTER_REAL34_DATA(REGISTER_X), const34_0, &matrix);
    convertComplex34MatrixToComplex34MatrixRegister(&matrix, REGISTER_X);
  }
  else {
    elementwiseCxmaReal(mulCplxReal);
  }
}



/********************************************//**
 * \brief Y(real34) × X(complex34 matrix) ==> X(complex34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRealCxma(void) {
  complex34Matrix_t matrix;
  if(getRegisterAngularMode(REGISTER_Y) == amNone) {
    linkToComplexMatrixRegister(REGISTER_X, &matrix);
    multiplyComplexMatrix(&matrix, REGISTER_REAL34_DATA(REGISTER_Y), const34_0, &matrix);
  }
  else {
    elementwiseRealCxma(mulRealCplx);
  }
}



/********************************************//**
 * \brief Y(complex34 matrix) × X(complex34) ==> X(complex34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCxmaCplx(void) {
  complex34Matrix_t matrix;
  linkToComplexMatrixRegister(REGISTER_Y, &matrix);
  multiplyComplexMatrix(&matrix, REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X), &matrix);
  convertComplex34MatrixToComplex34MatrixRegister(&matrix, REGISTER_X);
}



/********************************************//**
 * \brief Y(complex34) × X(complex34 matrix) ==> X(complex34 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCplxCxma(void) {
  complex34Matrix_t matrix;
  linkToComplexMatrixRegister(REGISTER_X, &matrix);
  multiplyComplexMatrix(&matrix, REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_Y), &matrix);
}



/******************************************************************************************************************************************************************************************/
/* short integer × ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(short integer) × X(short integer) ==> X(short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoIShoI(void) {
  setRegisterTag(REGISTER_X, getRegisterTag(REGISTER_Y));
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intMultiply(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)), *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



/********************************************//**
 * \brief Y(short integer) × X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoIReal(void) {
  real_t y, x;
  angularMode_t xAngularMode;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == amNone) {
    realMultiply(&y, &x, &x, &ctxtReal39);
    convertRealToReal34ResultRegister(&x, REGISTER_X);
  }
  else {
    convertAngleFromTo(&x, xAngularMode, currentAngularMode, &ctxtReal39);
    realMultiply(&y, &x, &x, &ctxtReal39);
    convertRealToReal34ResultRegister(&x, REGISTER_X);
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) × X(short integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRealShoI(void) {
  real_t y, x;
  angularMode_t yAngularMode;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);

  if(yAngularMode == amNone) {
    realMultiply(&y, &x, &x, &ctxtReal39);
    convertRealToReal34ResultRegister(&x, REGISTER_X);
  }
  else {
    convertAngleFromTo(&y, yAngularMode, currentAngularMode, &ctxtReal39);
    realMultiply(&y, &x, &x, &ctxtReal39);
    convertRealToReal34ResultRegister(&x, REGISTER_X);
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(short integer) × X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulShoICplx(void) {
  convertShortIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // imaginary part
}



/********************************************//**
 * \brief Y(complex34) × X(short integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCplxShoI(void) {
  convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_Y)); // imaginary part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* real34 × ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real34) × X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRealReal(void) {
  angularMode_t yAngularMode, xAngularMode;

  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(yAngularMode == amNone && xAngularMode == amNone) { // Neither is an angle
    real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
  else if(yAngularMode != amNone && xAngularMode != amNone) { // Both are angles
    real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, amNone);
  }
  else { // One and only one is an angle
    real_t y, x;

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    realMultiply(&y, &x, &x, &ctxtReal39);

    convertAngleFromTo(&x, yAngularMode != amNone ? yAngularMode : xAngularMode, currentAngularMode, &ctxtReal39);

    convertRealToReal34ResultRegister(&x, REGISTER_X);
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) × X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulRealCplx(void) {
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // imaginary part
}



/********************************************//**
 * \brief Y(complex34) × X(real34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCplxReal(void) {
  real34Multiply(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  real34Multiply(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_Y)); // imaginary part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X)); // imaginary part
}



/******************************************************************************************************************************************************************************************/
/* complex34 × ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex34) × X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void mulCplxCplx(void) {
  real_t yReal, yImag, xReal, xImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &yReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &yImag);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

  mulComplexComplex(&yReal, &yImag, &xReal, &xImag, &xReal, &xImag, &ctxtReal39);

  convertRealToReal34ResultRegister(&xReal, REGISTER_X);
  convertRealToImag34ResultRegister(&xImag, REGISTER_X);
}
