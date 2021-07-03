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
 * \file agm.c
 ***********************************************/

#include "mathematics/agm.h"

#include "constantPointers.h"
#include "debug.h"
#include "error.h"
#include "mathematics/arctan.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/division.h"
#include "mathematics/multiplication.h"
#include "mathematics/sin.h"
#include "mathematics/tan.h"
#include "mathematics/toRect.h"
#include "mathematics/toPolar.h"
#include "mathematics/wp34s.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43s.h"


#define AGM_MODE_NORMAL 0
#define AGM_MODE_E      1
#define AGM_MODE_STEP   2
#define AGM_MODE_F      3
#define AGM_MODE_ZETA   4

void fnAgm(uint16_t unusedButMandatoryParameter) {
  bool_t realInput=true;
  real_t aReal, bReal;
  real_t aImag, bImag;

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_X, &aReal, &ctxtReal39);
                        realZero(&aImag);
                        break;

    case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &aReal);
                        realZero(&aImag);
                        break;

    case dtComplex34:   real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &aReal);
                        real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &aImag);
                        realInput = false;
                        break;

    default:            displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
                        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                          sprintf(errorMessage, "cannot calculate AGM with %s in X", getRegisterDataTypeName(REGISTER_X, true, false));
                          moreInfoOnError("In function fnAgm:", errorMessage, NULL, NULL);
                        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
                        return;
  }

  switch(getRegisterDataType(REGISTER_Y)) {
    case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_Y, &bReal, &ctxtReal39);
                        realZero(&bImag);
                        break;

    case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &bReal);
                        realZero(&bImag);
                        break;

    case dtComplex34:   real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &bReal);
                        real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &bImag);
                        realInput = false;
                        break;

    default:            displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_Y);
                        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                          sprintf(errorMessage, "cannot calculate AGM with %s in Y", getRegisterDataTypeName(REGISTER_Y, true, false));
                          moreInfoOnError("In function fnAgm:", errorMessage, NULL, NULL);
                        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
                        return;
  }

  if(realInput && (realIsNegative(&aReal) || realIsNegative(&bReal))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function fnAgm:", "cannot use negative X and Y as input of AGM", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  if(!saveLastX()) return;

  if(realInput) {
    realAgm(&aReal, &bReal, &aReal, &ctxtReal39);

    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&aReal, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else { // Complex input
    complexAgm(&aReal, &aImag, &bReal, &bImag, &aReal, &aImag, &ctxtReal39);

    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    realToReal34(&aReal, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&aImag, REGISTER_IMAG34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, true, true, REGISTER_X, -1, -1);
}

static int _realAgm(int mode, const real_t *a, const real_t *b, real_t *c, real_t *res, real_t *_a, real_t *_b, size_t _sz, realContext_t *realContext) {
  real_t aReal, bReal, cReal;
  real_t cCoeff, prevDelta, z;
  int n = 0;

  realCopy(a, &aReal);
  realCopy(b, &bReal);
  if(mode==AGM_MODE_E) {
    realCopy(const_1, &cCoeff);
  }
  if(mode==AGM_MODE_STEP) {
    realCopy(&aReal, _a);
    realCopy(&bReal, _b);
  }
  if(mode==AGM_MODE_F||mode==AGM_MODE_ZETA) {
    realCopy(const_plusInfinity, &prevDelta);
    realCopy(const_0, &z);
    realDivide(c, const_pi, &cCoeff, realContext);
    realToIntegralValue(&cCoeff, &cCoeff, DEC_ROUND_DOWN, realContext);
    realDivideRemainder(c, const_pi, c, realContext);
  }

  while(!realCompareEqual(&aReal, &bReal) && realIdenticalDigits(&aReal, &bReal) <= 34) {
    if(mode==AGM_MODE_E) {
      realMultiply(&cCoeff, const_2, &cCoeff, realContext);
      realSubtract(&aReal, &bReal, &cReal, realContext);     // c = a - b
      realMultiply(&cReal, const_1on2, &cReal, realContext); // c = (a - b) / 2
      realMultiply(&cReal, &cReal, &cReal, realContext);     // c^2
      realFMA(&cReal, &cCoeff, c, c, realContext);
    }
    if(mode==AGM_MODE_F||mode==AGM_MODE_ZETA) {
      real_t d, e, tanphi, ba;
      WP34S_Cvt2RadSinCosTan(c, amRadian, &d, &e, &tanphi, realContext);
      realDivide(&bReal, &aReal, &ba, realContext);
      realDivide(const_1, &tanphi, &d, realContext);
      realFMA(&ba, &tanphi, &d, &d, realContext);
      realSubtract(&ba, const_1, &e, realContext);
      WP34S_Atan2(&e, &d, &d, realContext);
      realAdd(&cCoeff, &cCoeff, &cCoeff, realContext);
      if(realCompareAbsLessThan(&prevDelta, &d)) {
        realAdd(&cCoeff, const_1, &cCoeff, realContext);
      }
      realCopy(&d, &prevDelta);
      realAdd(&d, c, &d, realContext);
      realAdd(&d, c, c, realContext);
      if(mode==AGM_MODE_ZETA) {
        realCopy(c, &d);
        realDivideRemainder(&cCoeff, const_2, &e, realContext);
        if(!realIsZero(&e)) {
          realSubtract(const_2pi, &d, &d, realContext);
        }
        WP34S_Cvt2RadSinCosTan(&d, amRadian, &d, NULL, NULL, realContext);
        realSubtract(&aReal, &bReal, &e, realContext);
        realMultiply(&e, const_1on2, &e, realContext);
        realFMA(&e, &d, &z, &z, realContext);
      }
    }
    realAdd(&aReal, &bReal, &cReal, realContext);          // c = a + b
    realMultiply(&aReal, &bReal, &bReal, realContext);     // b = a * b
    realSquareRoot(&bReal, &bReal, realContext);           // b = sqrt(a * b)
    realMultiply(&cReal, const_1on2, &aReal, realContext); // a = (a + b) / 2
    ++n;
    if(mode==AGM_MODE_STEP) {
      realCopy(&aReal, _a + n);
      realCopy(&bReal, _b + n);
      if(n >= (int)_sz - 1) break;
    }
  }

  if(mode==AGM_MODE_E) {
    realMultiply(c, const_1on2, c, realContext);
  }
  if(mode==AGM_MODE_F) {
    realFMA(&cCoeff, const_pi, c, c, realContext);
  }
  if(mode==AGM_MODE_ZETA) {
    realCopy(&z, c);
  }

  realCopy(&aReal, res);
  return n;
}

static void _complexAgm(int mode, const real_t *ar, const real_t *ai, const real_t *br, const real_t *bi, real_t *cr, real_t *ci, real_t *resr, real_t *resi, realContext_t *realContext) {
  real_t aReal, bReal, cReal;
  real_t aImag, bImag, cImag;
  real_t cCoeff, prevDelta, zr, zi;

  realCopy(ar, &aReal); realCopy(ai, &aImag);
  realCopy(br, &bReal); realCopy(bi, &bImag);
  if(mode==AGM_MODE_E) {
    realCopy(const_1, &cCoeff);
  }
  if(mode==AGM_MODE_ZETA) {
    realCopy(const_plusInfinity, &prevDelta);
    realCopy(const_0, &zr); realCopy(const_0, &zi);
    realDivide(cr, const_pi, &cCoeff, realContext);
    realToIntegralValue(&cCoeff, &cCoeff, DEC_ROUND_DOWN, realContext);
    realDivideRemainder(cr, const_pi, cr, realContext);
  }

  while((!realCompareEqual(&aReal, &bReal) && realIdenticalDigits(&aReal, &bReal) <= 34) || (!realCompareEqual(&aImag, &bImag) && realIdenticalDigits(&aImag, &bImag) <= 34)) {
    if(mode==AGM_MODE_E) {
      realMultiply(&cCoeff, const_2, &cCoeff, realContext);
      realSubtract(&aReal, &bReal, &cReal, realContext); realSubtract(&aImag, &bImag, &cImag, realContext);     // c = a - b
      realMultiply(&cReal, const_1on2, &cReal, realContext); realMultiply(&cImag, const_1on2, &cImag, realContext); // c = (a - b) / 2
      mulComplexComplex(&cReal, &cImag, &cReal, &cImag, &cReal, &cImag, realContext);     // c^2
      realFMA(&cReal, &cCoeff, cr, cr, realContext);
      realFMA(&cImag, &cCoeff, ci, ci, realContext);
    }
    if(mode==AGM_MODE_ZETA) {
      real_t d, di, e, ei, tanphi, tanphii, ba, bai;
      TanComplex(cr, ci, &tanphi, &tanphii, realContext);
      divComplexComplex(&bReal, &bImag, &aReal, &aImag, &ba, &bai, realContext);
      divRealComplex(const_1, &tanphi, &tanphii, &d, &di, realContext);
      mulComplexComplex(&ba, &bai, &tanphi, &tanphii, &e, &ei, realContext);
      realAdd(&e, &d, &d, realContext); realAdd(&ei, &di, &di, realContext);
      realSubtract(&ba, const_1, &e, realContext); realCopy(&bai, &ei);
      divComplexComplex(&e, &ei, &d, &di, &d, &di, realContext);
      ArctanComplex(&d, &di, &d, &di, realContext);
      realAdd(&cCoeff, &cCoeff, &cCoeff, realContext);
      if(realCompareAbsLessThan(&prevDelta, &d)) {
        realAdd(&cCoeff, const_1, &cCoeff, realContext);
      }
      realCopy(&d, &prevDelta);
      realAdd(&d, cr, &d, realContext); realAdd(&di, ci, &di, realContext);
      realAdd(&d, cr, cr, realContext); realAdd(&di, ci, ci, realContext);

      realCopy(cr, &d); realCopy(ci, &di);
      realDivideRemainder(&cCoeff, const_2, &e, realContext);
      if(!realIsZero(&e)) {
        realSubtract(const_2pi, &d, &d, realContext);
      }
      sinComplex(&d, &di, &d, &di, realContext);
      realSubtract(&aReal, &bReal, &e, realContext); realSubtract(&aImag, &bImag, &ei, realContext);
      realMultiply(&e, const_1on2, &e, realContext); realMultiply(&ei, const_1on2, &ei, realContext);
      mulComplexComplex(&e, &ei, &d, &di, &d, &di, realContext);
      realAdd(&d, &zr, &zr, realContext); realAdd(&di, &zi, &zi, realContext);
    }

    realAdd(&aReal, &bReal, &cReal, realContext);                                   // c = a + b real part
    realAdd(&aImag, &bImag, &cImag, realContext);                                   // c = a + b imag part

    mulComplexComplex(&aReal, &aImag, &bReal, &bImag, &bReal, &bImag, realContext); // b = a * b

    // b = sqrt(a * b)
    realRectangularToPolar(&bReal, &bImag, &bReal, &bImag, realContext);
    realSquareRoot(&bReal, &bReal, realContext);
    realMultiply(&bImag, const_1on2, &bImag, realContext);
    realPolarToRectangular(&bReal, &bImag, &bReal, &bImag, realContext);

    realMultiply(&cReal, const_1on2, &aReal, realContext); // a = (a + b) / 2 real part
    realMultiply(&cImag, const_1on2, &aImag, realContext); // a = (a + b) / 2 imag part
  }

  if(mode==AGM_MODE_E) {
    realMultiply(cr, const_1on2, cr, realContext); realMultiply(ci, const_1on2, ci, realContext);
  }
  if(mode==AGM_MODE_ZETA) {
    realCopy(&zr, cr); realCopy(&zi, ci);
  }

  realCopy(&aReal, resr); realCopy(&aImag, resi);
}

size_t realAgm(const real_t *a, const real_t *b, real_t *res, realContext_t *realContext) {
  return _realAgm(AGM_MODE_NORMAL, a, b, NULL, res, NULL, NULL, 0, realContext);
}

void complexAgm(const real_t *ar, const real_t *ai, const real_t *br, const real_t *bi, real_t *resr, real_t *resi, realContext_t *realContext) {
  _complexAgm(AGM_MODE_NORMAL, ar, ai, br, bi, NULL, NULL, resr, resi, realContext);
}

size_t realAgmForE(const real_t *a, const real_t *b, real_t *c, real_t *res, realContext_t *realContext) {
  return _realAgm(AGM_MODE_E, a, b, c, res, NULL, NULL, 0, realContext);
}

void complexAgmForE(const real_t *ar, const real_t *ai, const real_t *br, const real_t *bi, real_t *cr, real_t *ci, real_t *resr, real_t *resi, realContext_t *realContext) {
  _complexAgm(AGM_MODE_E, ar, ai, br, bi, cr, ci, resr, resi, realContext);
}

size_t realAgmForF(const real_t *a, const real_t *b, real_t *c, real_t *res, realContext_t *realContext) {
  return _realAgm(AGM_MODE_F, a, b, c, res, NULL, NULL, 0, realContext);
}

size_t realAgmForZ(const real_t *a, const real_t *b, real_t *c, real_t *res, realContext_t *realContext) {
  return _realAgm(AGM_MODE_ZETA, a, b, c, res, NULL, NULL, 0, realContext);
}

void complexAgmForZ(const real_t *ar, const real_t *ai, const real_t *br, const real_t *bi, real_t *cr, real_t *ci, real_t *resr, real_t *resi, realContext_t *realContext) {
  _complexAgm(AGM_MODE_ZETA, ar, ai, br, bi, cr, ci, resr, resi, realContext);
}

size_t realAgmStep(const real_t *a, const real_t *b, real_t *res, real_t *aStep, real_t *bStep, size_t bufSize, realContext_t *realContext) {
  return _realAgm(AGM_MODE_STEP, a, b, NULL, res, aStep, bStep, bufSize, realContext);
}
