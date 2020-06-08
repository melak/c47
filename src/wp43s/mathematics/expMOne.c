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
 * \file expMOne.c
 ***********************************************/
// Coded by JM, based on exp.c

#include "wp43s.h"



void (* const ExpM1[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX ==> 1            2          3           4           5           6           7            8             9               10
//          Long integer Real34     Complex34   Time        Date        String      Real34 mat   Complex34 m   Short integer   Config data
            expM1LonI,   expM1Real, expM1Cplx,  expM1Error, expM1Error, expM1Error, expM1Rema,   expM1Cxma,    expM1ShoI,      expM1Error
};



/********************************************//**
 * \brief Data type error in expM1
 *
 * \param void
 * \return void
 ***********************************************/
void expM1Error(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Exp(x)-1 for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnExpM1:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and expM1(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnExpM1(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  ExpM1[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}





void expM1Complex(const real_t *real, const real_t *imag, real_t *resReal, real_t *resImag, realContext_t *realContext) {
  real_t expa, sin, cos;

  if(realIsZero(imag)) {
    if(realIsInfinite(real) && realIsNegative(real)) {
      realSubtract(const_0,const_1,resReal,realContext);
      realZero(resImag);
      return;
    }
    if(realCompareAbsGreaterThan(real, const_10000)){
      realExp(real, resReal, realContext);
      realSubtract(resReal, const_1, resReal, realContext);
    } else {
      WP34S_ExpM1(real, resReal, realContext);
    }
    realZero(resImag);
    return;
  }

  if(realIsSpecial(real) || realIsSpecial(imag)) {
    realCopy(const_NaN, resReal);
    realCopy(const_NaN, resImag);
    return;
  }

// if(realCompareAbsGreaterThan(real, const_10000) || realIsInfinite(real) ){
   realExp(real, &expa, realContext);
// } else {
//   WP34S_ExpM1(real, &expa, realContext);
  // realAdd(&expa, const_1, &expa, realContext);
// }
 WP34S_Cvt2RadSinCosTan(imag, AM_RADIAN, &sin, &cos, NULL, realContext);
 realMultiply(&expa, &cos, resReal, realContext);
 realMultiply(&expa, &sin, resImag, realContext);
 realSubtract(resReal, const_1, resReal, realContext);

}



/**********************************************************************
 * In all the functions below:
 * if X is a number then X = a + ib
 * The variables a and b are used for intermediate calculations
 * 1 is subtracted at the end
 ***********************************************************************/

void expM1LonI(void) {
  real_t a;

  convertLongIntegerRegisterToReal(REGISTER_X, &a, &ctxtReal39);
  if(realCompareAbsGreaterThan(&a, const_10000)|| realIsInfinite(&a) ){
    realExp(&a, &a, &ctxtReal39);
    realSubtract(&a, const_1, &a, &ctxtReal39);
  } else {
    WP34S_ExpM1(&a, &a, &ctxtReal39);
  }
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
}



void expM1Rema(void) {
  fnToBeCoded();
}



void expM1Cxma(void) {
  fnToBeCoded();
}



void expM1ShoI(void) {
  real_t x;

  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  if(realCompareAbsGreaterThan(&x, const_10000)){
    realExp(&x, &x, &ctxtReal39);
    realSubtract(&x, const_1, &x, &ctxtReal39);
  } else {
    WP34S_ExpM1(&x, &x, &ctxtReal39);
  }
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
}



void expM1Real(void) {
  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X)) && !getSystemFlag(FLAG_SPCRES)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function expM1Real:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of exp when flag D is not set", NULL, NULL);
    #endif
    return;
  }

  real_t x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  if(realIsInfinite(&x) && realIsNegative(&x)) {
    realSubtract(const_0,const_1,&x,&ctxtReal39);
  } else
    if(realIsInfinite(&x) && realIsPositive(&x)) {
        realCopy(const_plusInfinity, &x);
    } else
      if(realIsSpecial(&x)) {
        realCopy(const_NaN, &x);
      } else
        if(realCompareAbsGreaterThan(&x, const_10000) || realIsInfinite(&x) ) {
          realExp(&x, &x, &ctxtReal39);
          realSubtract(&x, const_1, &x, &ctxtReal39);
        } else {
          WP34S_ExpM1(&x, &x, &ctxtReal39);
        }
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void expM1Cplx(void) {
  real_t zReal, zImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &zReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &zImag);

  expM1Complex(&zReal, &zImag, &zReal, &zImag, &ctxtReal51);

  realToReal34(&zReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&zImag, REGISTER_IMAG34_DATA(REGISTER_X));
}




#ifdef RRR
//**********************
  real_t x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  WP34S_Tanh(&x, &x, &ctxtReal39);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}


void   WP34S_Ln1P            (const real_t *x, real_t *res, realContext_t *realContext);
void   WP34S_ExpM1           (const real_t *x, real_t *res, realContext_t *realContext);



void WP34S_Tanh(const real_t *x, real_t *res, realContext_t *realContext) {
  if(realIsNaN(x)) {
    realCopy(const_NaN, res);
  }
  else if(realCompareAbsGreaterThan(x, const_47)) { // equals 1 to 39 digits
    realCopy((realIsPositive(x) ? const_1 : const__1), res);
  }
  else {
    real_t a, b;

    realAdd(x, x, &a, realContext);        // a = 2x
    WP34S_ExpM1(&a, &b, realContext);      // b = exp(2x) - 1
    realAdd(&b, const_2, &a, realContext); // a = exp(2x) - 1 + 2 = exp(2x) + 1
    realDivide(&b, &a, res, realContext);  // res = (exp(2x) - 1) / (exp(2x) + 1)
  }
}




/* ln(1+x) */
void WP34S_Ln1P(const real_t *x, real_t *res, realContext_t *realContext) {
  real_t u, v, w;

  if(realIsSpecial(x) || realIsZero(x)) {
    realCopy(x, res);
  }
  else {
    realAdd(x, const_1, &u, realContext);       // u = x+1
    realSubtract(&u, const_1, &v, realContext); // v = x
    if(realIsZero(&v)) {
      realCopy(x, res);
    }
    else {
      realDivide(x, &v, &w, realContext);
      WP34S_Ln(&u, &v, realContext);
      realMultiply(&v, &w, res, realContext);
    }
  }
}



/* exp(x)-1 */
void WP34S_ExpM1(const real_t *x, real_t *res, realContext_t *realContext) {
  real_t u, v, w;

  if(realIsInfinite(x)) {
    if(realIsPositive(x)) {
      realCopy(const_plusInfinity, res);
    }
    else {
      realCopy(const__1, res);
    }
  }

  realExp(x, &u, realContext);
  realSubtract(&u, const_1, &v, realContext);
  if(realIsZero(&v)) { // |x| is very little
    realCopy(x, res);
  }
  else {
    if(realCompareEqual(&v, const__1)) {
      realCopy(const__1, res);
    }
    else {
      realMultiply(&v, x, &w, realContext);
      WP34S_Ln(&u, &v, realContext);
      realDivide(&w, &v, res, realContext);
    }
  }
}


void arcsinhReal(void) {
  real_t x, xSquared;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  if(realIsInfinite(&x) && realIsNegative(&x)) {
    realCopy(const_minusInfinity, &x);
  }
  else {
    // arcsinh(x) = ln(x + sqrt(x² + 1))
    realMultiply(&x, &x, &xSquared, &ctxtReal51);
    realAdd(&xSquared, const_1, &xSquared, &ctxtReal51);
    realSquareRoot(&xSquared, &xSquared, &ctxtReal51);
    realAdd(&xSquared, &x, &x, &ctxtReal51);
    WP34S_Ln(&x, &x, &ctxtReal51);
  }

  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



#endif

