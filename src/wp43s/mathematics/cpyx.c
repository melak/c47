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
 * \file cpyx.c
 ***********************************************/

#include "wp43s.h"

static void cpyxDataTypeError(uint16_t);

void (* const cpyx[9][9])(uint16_t) = {
// regX |    regY ==>    1                  2                  3                  4                  5                  6                  7                  8                  9
//      V                Long integer       Real34             Complex34          Time               Date               String             Real34 mat         Complex34 mat      Short integer
/*  1 Long integer  */ { cpyxLonILonI,      cpyxRealLonI,      cpyxCplxLonI,      cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxShoILonI      },
/*  2 Real34        */ { cpyxLonIReal,      cpyxRealReal,      cpyxCplxReal,      cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxShoIReal      },
/*  3 Complex34     */ { cpyxLonICplx,      cpyxRealCplx,      cpyxCplxCplx,      cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxShoICplx      },
/*  4 Time          */ { cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError },
/*  5 Date          */ { cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError },
/*  6 String        */ { cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError },
/*  7 Real34 mat    */ { cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError },
/*  8 Complex34 mat */ { cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError },
/*  9 Short integer */ { cpyxLonIShoI,      cpyxRealShoI,      cpyxCplxShoI,      cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxDataTypeError, cpyxShoIShoI      }
};

//=============================================================================
// Error handling
//-----------------------------------------------------------------------------

#define DOMAIN_ERROR            1
#define INVALID_DATA_CMP_ERROR  2
#define DATA_OUT_OF_RANGE_ERROR 3

#if (EXTRA_INFO_ON_CALC_ERROR == 1)

#define EXTRA_INFO_MESSAGE(msg)                                         \
 do {                                                                   \
  sprintf(errorMessage, msg);                                           \
  showInfoDialog("In function fnCyx/fnPyx:", errorMessage, NULL, NULL); \
 } while(0)

#else // EXTRA_INFO_ON_CALC_ERROR != 1

#define EXTRA_INFO_MESSAGE(msg)

#endif // EXTRA_INFO_ON_CALC_ERROR


static void cpyxError(uint16_t error) {
  switch(error) {
    case DOMAIN_ERROR:
        displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        EXTRA_INFO_MESSAGE("cannot calculate Cyx/Pyx, y and x must be greater or equal than zero.");
        break;

    case INVALID_DATA_CMP_ERROR:
        displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        EXTRA_INFO_MESSAGE("cannot calculate Cyx/Pyx, y must be greater or equal than x.");
        break;

    case DATA_OUT_OF_RANGE_ERROR:
        displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
        EXTRA_INFO_MESSAGE("cannot calculate Cyx/Pyx, the limit for Long is 450 and for Short is 20.");
        break;
  }
}

/********************************************//**
 * \brief Data type error in Cyx
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
static void cpyxDataTypeError(uint16_t unused) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);

  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot raise %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "to %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnCyx/fnPyx:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}

//=============================================================================
// Cyx/Pyx calculation function
//-----------------------------------------------------------------------------

static void cyxReal(real_t *y, real_t *x, real_t *result, realContext_t *realContext) {
  //realSubtract(y, x, result, realContext);        // t = y - x
  //WP34S_Factorial(result, result, realContext);   // t = (y - x)!

  //WP34S_Factorial(y, y, realContext);             // y = y!

  //WP34S_Factorial(x, x, realContext);             // x = x!

  //realMultiply(x, result, result, realContext);   // t = x! * (y - x)!

  //realDivide(y, result, result, realContext);     // t = y! / [x! * (y - x)!]

  realSubtract(y, x, result, realContext);
  realAdd(result, const_1, result, realContext);
  WP34S_LnGamma(result, result, realContext);       // r = ln((y-x)!)

  realAdd(x, const_1, x, realContext);
  WP34S_LnGamma(x, x, realContext);                 // x = ln(x!)

  realAdd(y, const_1, y, realContext);
  WP34S_LnGamma(y, y, realContext);                 // y = ln(y!)

  realSubtract(y, result, result, realContext);
  realSubtract(result, x, result, realContext);     // r = ln(y!) - ln((y-x)!) - ln(x!)

  realExp(result, result, realContext);             // r = y! / ((y-x)! × x!)
}

static void cyxLong(longInteger_t y, longInteger_t x, longInteger_t result) {
  uint32_t temp;

  longIntegerSubtract(y, x, result);      // t = y - x

  longIntegerToUInt(result, temp);
  longIntegerFactorial(temp , result);    // t = (y - x)!

  longIntegerToUInt(y, temp);
  longIntegerFactorial(temp, y);          // y = y!

  longIntegerToUInt(x, temp);
  longIntegerFactorial(temp, x);          // x = x!

  longIntegerMultiply(x, result, result); // t = x! * (y - x)!

  longIntegerDivide(y, result, result);   // t = y! / [x! * (y - x)!]
}

static void cyxCplx(real_t *yReal, real_t *yImag, real_t *xReal, real_t *xImag, real_t *rReal, real_t *rImag, realContext_t *realContext) {
  realSubtract(yReal, xReal, rReal, realContext);                // r = y - x
  realSubtract(yImag, xImag, rImag, realContext);

  realAdd(rReal, const_1, rReal, realContext);                   // r = t + 1
  WP34S_ComplexLnGamma(rReal, rImag, rReal, rImag, realContext); // r = lnGamma(t + 1) = ln((y - x)!)

  realAdd(xReal, const_1, xReal, realContext);                   // x = x + 1
  WP34S_ComplexLnGamma(xReal, xImag, xReal, xImag, realContext); // x = lnGamma(x + 1) = ln(x!)

  realAdd(yReal, const_1, yReal, realContext);                   // y = y + 1
  WP34S_ComplexLnGamma(yReal, yImag, yReal, yImag, realContext); // y = lnGamma(y + 1) = ln(y!)

  realSubtract(yReal, rReal, rReal, realContext);                // r = ln(y!) - ln((y - x)!)
  realSubtract(yImag, rImag, rImag, realContext);

  realSubtract(rReal, xReal, rReal, realContext);                // r = ln(y!) - ln((y - x)!) - ln(x!)
  realSubtract(rImag, xImag, rImag, realContext);

  expComplex(rReal, rImag, rReal, rImag, realContext);           // r = y! / ((y-x)! × x!)
}

static void pyxReal(real_t *y, real_t *x, real_t *result, realContext_t *realContext) {
  //realSubtract(y, x, result, realContext);      // t = y - x
  //WP34S_Factorial(result, result, realContext); // t = (y - x)!

  //WP34S_Factorial(y, y, realContext);           // y = y!

  //realDivide(y, result, result, realContext);   // t = y! / (y - x)!

  realSubtract(y, x, result, realContext);
  realAdd(result, const_1, result, realContext);
  WP34S_LnGamma(result, result, realContext);     // r = ln((y-x)!)

  realAdd(y, const_1, y, realContext);
  WP34S_LnGamma(y, y, realContext);               // y = ln(y!)

  realSubtract(y, result, result, realContext);   // r = ln(y!) - ln((y-x)!)

  realExp(result, result, realContext);           // r = y! / (y-x)!
}

static void pyxLong(longInteger_t y, longInteger_t x, longInteger_t result) {
  uint32_t temp;

  longIntegerSubtract(y, x, result);    // t = y-x
  longIntegerToUInt(result, temp);
  longIntegerFactorial(temp, result);   // t = (y-x)!

  longIntegerToUInt(y, temp);
  longIntegerFactorial(temp, y);        // y = y!

  longIntegerDivide(y, result, result); // t = y! / (y -x)!
}

static void pyxCplx(real_t *yReal, real_t *yImag, real_t *xReal, real_t *xImag, real_t *rReal, real_t *rImag, realContext_t *realContext) {
  realSubtract(yReal, xReal, rReal, realContext);                // r = y - x
  realSubtract(yImag, xImag, rImag, realContext);

  realAdd(rReal, const_1, rReal, realContext);                   // r = t + 1
  WP34S_ComplexLnGamma(rReal, rImag, rReal, rImag, realContext); // r = lnGamma(t + 1) = ln((y - x)!)

  realAdd(yReal, const_1, yReal, realContext);                   // y = y + 1
  WP34S_ComplexLnGamma(yReal, yImag, yReal, yImag, realContext); // y = lnGamma(y + 1) = ln(y!)

  realSubtract(yReal, rReal, rReal, realContext);                // r = ln(y!) - ln((y - x)!)
  realSubtract(yImag, rImag, rImag, realContext);

  expComplex(rReal, rImag, rReal, rImag, realContext);           // r = y! / (y-x)!
}

//=============================================================================
// Main function
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief regX ==> regL and Cyx(regX, RegY) ==> regX
 * enables stack lift and refreshes the stack.
 * C(n,k) = n! / [k! * (n-k)!]
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnCyx(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  cpyx[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)](CP_COMBINATION);

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
  adjustResult(REGISTER_Y, true, true, REGISTER_Y, -1, -1);
}

/********************************************//**
 * \brief regX ==> regL and Pyx(regX, RegY) ==> regX
 * enables stack lift and refreshes the stack.
 * P(n,k) = n! / (n-k)!
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnPyx(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  cpyx[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)](CP_PERMUTATION);

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
  adjustResult(REGISTER_Y, true, true, REGISTER_Y, -1, -1);
}

//=============================================================================
// Cyx/Pyx(Y(long integer), *)
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief Cyx/Pyx(Y(long integer), X(long integer)) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void cpyxLonILonI(uint16_t combOrPerm) {
  longInteger_t x, y;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);
  convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);

  if(longIntegerIsNegative(x) || longIntegerIsNegative(y)) {
    cpyxError(DOMAIN_ERROR);
  }
  else if(longIntegerCompareUInt(x, 450) > 0 || longIntegerCompareUInt(y, 450) > 0) {
    cpyxError(DATA_OUT_OF_RANGE_ERROR);
  }
  else if(longIntegerCompare(y, x) < 0) {
    cpyxError(INVALID_DATA_CMP_ERROR);
  }
  else {
    longInteger_t t;
    longIntegerInit(t);

    (combOrPerm == CP_COMBINATION) ? cyxLong(y, x, t) : pyxLong(y, x, t);

    convertLongIntegerToLongIntegerRegister(t, REGISTER_X);
    longIntegerFree(t);
  }

  longIntegerFree(x);
  longIntegerFree(y);
}

/********************************************//**
 * \brief Cyx/Pyx(Y(long integer), X(real34)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void cpyxLonIReal(uint16_t combOrPerm) {
  real_t x, y;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal75);

  if(realIsNegative(&x) || realIsNegative(&y)) {
    cpyxError(DOMAIN_ERROR);
  }
  else {
    real_t t;

    (combOrPerm == CP_COMBINATION) ? cyxReal(&y, &x, &t, &ctxtReal39) : pyxReal(&y, &x, &t, &ctxtReal39);

    realToReal34(&t, REGISTER_REAL34_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, AM_NONE);
  }
}

/********************************************//**
 * \brief Cyx/Pyx(Y(long integer), X(complex34)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void cpyxLonICplx(uint16_t combOrPerm) {
  real_t xReal, xImag, yReal, yImag;
  real_t tReal, tImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

  convertLongIntegerRegisterToReal(REGISTER_Y, &yReal, &ctxtReal39);
  real34ToReal(const34_0, &yImag);

  (combOrPerm == CP_COMBINATION) ? cyxCplx(&yReal, &yImag, &xReal, &xImag, &tReal, &tImag, &ctxtReal39) : pyxCplx(&yReal, &yImag, &xReal, &xImag, &tReal, &tImag, &ctxtReal39);

  realToReal34(&tReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&tImag, REGISTER_IMAG34_DATA(REGISTER_X));
}

/********************************************//**
 * \brief Cyx/Pyx(Y(long integer), X(short integer)) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void cpyxLonIShoI(uint16_t combOrPerm) {
  longInteger_t x, y;

  convertShortIntegerRegisterToLongInteger(REGISTER_X, x);
  convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);

  if(longIntegerIsNegative(x) || longIntegerIsNegative(y)) {
    cpyxError(DOMAIN_ERROR);
  }
  else if(longIntegerCompareUInt(x, 450) > 0 || longIntegerCompareUInt(y, 450) > 0) {
    cpyxError(DATA_OUT_OF_RANGE_ERROR);
  }
  else if(longIntegerCompare(y, x) < 0) {
    cpyxError(INVALID_DATA_CMP_ERROR);
  }
  else {
    longInteger_t t;
    longIntegerInit(t);

    (combOrPerm == CP_COMBINATION) ? cyxLong(y, x, t) : pyxLong(y, x, t);

    convertLongIntegerToLongIntegerRegister(t, REGISTER_X);
    longIntegerFree(t);
  }

  longIntegerFree(x);
  longIntegerFree(y);
}

//=============================================================================
// Cyx/Pyx(Y(real34), *)
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief Cyx/Pyx(Y(real34), X(long integer)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void cpyxRealLonI(uint16_t combOrPerm) {
  real_t x, y;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);

  if(realIsNegative(&x) || realIsNegative(&y)) {
    cpyxError(DOMAIN_ERROR);
  }
  else {
    real_t t;

    (combOrPerm == CP_COMBINATION) ? cyxReal(&y, &x, &t, &ctxtReal39) : pyxReal(&y, &x, &t, &ctxtReal39);

    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
    realToReal34(&t, REGISTER_REAL34_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, AM_NONE);
  }
}

/********************************************//**
 * \brief Cyx/Pyx(Y(real34), X(real34)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void cpyxRealReal(uint16_t combOrPerm) {
  real_t x, y;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  if(realIsNegative(&x) || realIsNegative(&y)) {
    cpyxError(DOMAIN_ERROR);
  }
  else {
    real_t t;

    (combOrPerm == CP_COMBINATION) ? cyxReal(&y, &x, &t, &ctxtReal39) : pyxReal(&y, &x, &t, &ctxtReal39);

    realToReal34(&t, REGISTER_REAL34_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, AM_NONE);
  }
}

/********************************************//**
 * \brief Cyx/Pyx(Y(real34), X(complex34)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void cpyxRealCplx(uint16_t combOrPerm) {
  real_t xReal, xImag, yReal, yImag;
  real_t tReal, tImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &yReal);
  real34ToReal(const34_0, &yImag);

  (combOrPerm == CP_COMBINATION) ? cyxCplx(&yReal, &yImag, &xReal, &xImag, &tReal, &tImag, &ctxtReal39) : pyxCplx(&yReal, &yImag, &xReal, &xImag, &tReal, &tImag, &ctxtReal39);

  realToReal34(&tReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&tImag, REGISTER_IMAG34_DATA(REGISTER_X));
}

/********************************************//**
 * \brief Cyx/Pyx(Y(real34), X(short integer)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void cpyxRealShoI(uint16_t combOrPerm) {
  real_t x, y;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);

  if(realIsNegative(&x) || realIsNegative(&y)) {
    cpyxError(DOMAIN_ERROR);
  }
  else {
    real_t t;

    (combOrPerm == CP_COMBINATION) ? cyxReal(&y, &x, &t, &ctxtReal39) : pyxReal(&y, &x, &t, &ctxtReal39);

    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
    realToReal34(&t, REGISTER_REAL34_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, AM_NONE);
  }
}

//=============================================================================
// Cyx/Pyx(Y(complex34), *)
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief Cyx/Pyx(Y(complex34), X(long integer)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void cpyxCplxLonI(uint16_t combOrPerm) {
  real_t xReal, xImag, yReal, yImag;
  real_t tReal, tImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &yReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &yImag);

  convertLongIntegerRegisterToReal(REGISTER_X, &xReal, &ctxtReal39);
  real34ToReal(const34_0, &xImag);

  (combOrPerm == CP_COMBINATION) ? cyxCplx(&yReal, &yImag, &xReal, &xImag, &tReal, &tImag, &ctxtReal39) : pyxCplx(&yReal, &yImag, &xReal, &xImag, &tReal, &tImag, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(&tReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&tImag, REGISTER_IMAG34_DATA(REGISTER_X));
}

/********************************************//**
 * \brief Cyx/Pyx(Y(complex34), X(real34)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void cpyxCplxReal(uint16_t combOrPerm) {
  real_t xReal, xImag, yReal, yImag;
  real_t tReal, tImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &yReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &yImag);

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
  real34ToReal(const34_0, &xImag);

  (combOrPerm == CP_COMBINATION) ? cyxCplx(&yReal, &yImag, &xReal, &xImag, &tReal, &tImag, &ctxtReal39) : pyxCplx(&yReal, &yImag, &xReal, &xImag, &tReal, &tImag, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(&tReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&tImag, REGISTER_IMAG34_DATA(REGISTER_X));
}

/********************************************//**
 * \brief Cyx/Pyx(Y(complex34), X(complex34)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void cpyxCplxCplx(uint16_t combOrPerm) {
  real_t xReal, xImag, yReal, yImag;
  real_t tReal, tImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &yReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &yImag);

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

  (combOrPerm == CP_COMBINATION) ? cyxCplx(&yReal, &yImag, &xReal, &xImag, &tReal, &tImag, &ctxtReal39) : pyxCplx(&yReal, &yImag, &xReal, &xImag, &tReal, &tImag, &ctxtReal39);

  realToReal34(&tReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&tImag, REGISTER_IMAG34_DATA(REGISTER_X));
}

/********************************************//**
 * \brief Cyx/Pyx(Y(complex34), X(short integer)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void cpyxCplxShoI(uint16_t combOrPerm) {
  real_t xReal, xImag, yReal, yImag;
  real_t tReal, tImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &yReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &yImag);

  convertShortIntegerRegisterToReal(REGISTER_X, &xReal, &ctxtReal39);
  real34ToReal(const34_0, &xImag);

  (combOrPerm == CP_COMBINATION) ? cyxCplx(&yReal, &yImag, &xReal, &xImag, &tReal, &tImag, &ctxtReal39) : pyxCplx(&yReal, &yImag, &xReal, &xImag, &tReal, &tImag, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(&tReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&tImag, REGISTER_IMAG34_DATA(REGISTER_X));
}

//=============================================================================
// Cyx/Pyx(Y(short integer), *)
//-----------------------------------------------------------------------------

/********************************************//**
 * \brief Cyx/Pyx(Y(short integer), X(long integer)) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void cpyxShoILonI(uint16_t combOrPerm) {
  longInteger_t x, y;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);
  convertShortIntegerRegisterToLongInteger(REGISTER_Y, y);

  if(longIntegerIsNegative(x) || longIntegerIsNegative(y)) {
    cpyxError(DOMAIN_ERROR);
  }
  else if(longIntegerCompareUInt(x, 450) > 0 || longIntegerCompareUInt(y, 450) > 0) {
    cpyxError(DATA_OUT_OF_RANGE_ERROR);
  }
  else if(longIntegerCompare(y, x) < 0) {
    cpyxError(INVALID_DATA_CMP_ERROR);
  }
  else {
    longInteger_t t;
    longIntegerInit(t);

    (combOrPerm == CP_COMBINATION) ? cyxLong(y, x, t) : pyxLong(y, x, t);

    convertLongIntegerToLongIntegerRegister(t, REGISTER_X);
    longIntegerFree(t);
  }

  longIntegerFree(x);
  longIntegerFree(y);
}

/********************************************//**
 * \brief Cyx/Pyx(Y(short integer), X(real34)) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void cpyxShoIReal(uint16_t combOrPerm) {
  real_t x, y;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);

  if(realIsNegative(&x) || realIsNegative(&y)) {
    cpyxError(DOMAIN_ERROR);
  }
  else {
    real_t t;

    (combOrPerm == CP_COMBINATION) ? cyxReal(&y, &x, &t, &ctxtReal39) : pyxReal(&y, &x, &t, &ctxtReal39);

    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
    realToReal34(&t, REGISTER_REAL34_DATA(REGISTER_X));
    setRegisterAngularMode(REGISTER_X, AM_NONE);
  }
}

/********************************************//**
 * \brief Cyx/Pyx(Y(short integer), X(complex34)) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void cpyxShoICplx(uint16_t combOrPerm) {
  real_t xReal, xImag, yReal, yImag;
  real_t tReal, tImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &xReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &xImag);

  convertShortIntegerRegisterToReal(REGISTER_Y, &yReal, &ctxtReal39);
  real34ToReal(const34_0, &yImag);

  (combOrPerm == CP_COMBINATION) ? cyxCplx(&yReal, &yImag, &xReal, &xImag, &tReal, &tImag, &ctxtReal39) : pyxCplx(&yReal, &yImag, &xReal, &xImag, &tReal, &tImag, &ctxtReal39);

  realToReal34(&tReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&tImag, REGISTER_IMAG34_DATA(REGISTER_X));
}

/********************************************//**
 * \brief Cyx/Pyx(Y(short integer), X(short integer)) ==> X(short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void cpyxShoIShoI(uint16_t combOrPerm) {
  int16_t x_sign, y_sign;
  uint64_t x_value, y_value;

  convertShortIntegerRegisterToUInt64(REGISTER_X, &x_sign, &x_value);
  convertShortIntegerRegisterToUInt64(REGISTER_Y, &y_sign, &y_value);

  if(x_sign==1 || y_sign==1) {
    cpyxError(DOMAIN_ERROR);
  }
  else if(x_value > 20 || y_value > 20) {
    cpyxError(DATA_OUT_OF_RANGE_ERROR);
  }
  else if(y_value < x_value) {
    cpyxError(INVALID_DATA_CMP_ERROR);
  }
  else {
    uint64_t value = (combOrPerm == CP_COMBINATION)
            ? fact_uint64(y_value) / ( fact_uint64(x_value) * fact_uint64(y_value - x_value))
            : fact_uint64(y_value) / fact_uint64(y_value - x_value);

    if(value > shortIntegerMask) {
      setSystemFlag(FLAG_OVERFLOW);
    }

    convertUInt64ToShortIntegerRegister(0, value, getRegisterTag(REGISTER_X), REGISTER_X);
  }
}

