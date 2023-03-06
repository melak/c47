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
 * \file linpol.c
 ***********************************************/

#include "mathematics/linpol.h"

#include "constantPointers.h"
#include "debug.h"
#include "error.h"
#include "flags.h"
#include "items.h"
#include "mathematics/comparisonReals.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "stack.h"

#include "wp43.h"

/*
 * Linear interpolation between a and b by an amount p.
 * p = 0 returns a, p = 1 returns b.
 *
 * There are two basic formulae for this but both have issues.
 *     linpol = a + p * (b - a) has cancellation issues but is monotonic
 *     linpol = (1 - p) * a + p * b is only monotonic if a * b < 0
 * we work in high precision and special case both
 */
void linpol(const real_t *a, const real_t *b, const real_t *p, real_t *res) {
  real_t x;

  if (realIsNaN(a) || realIsNaN(b) || realIsNaN(p) || realIsInfinite(p)) {
    realCopy(const_NaN, res);
  } else if (realIsInfinite(a)) {
    if (realIsInfinite(b)) {    // both infinite, either NaN or one of them
      if (realIsNegative(a) == realIsNegative(b))
        realCopy(a, res);
      else
        realCopy(const_NaN, res);
    } else
      realCopy(a, res);         // a only is infinite, return a
  } else if (realIsInfinite(b)) {
    realCopy(b, res);           // b only is infinite, return b
  } else if (realCompareEqual(a, b)) {
    realCopy(a, res);           // both same, return one
  } else if (realIsNegative(a) != realIsNegative(b)) {
    /* a * b < 0, use linpol = (1 - p) * a + p * b */
    realCopy(p, &x);
    realChangeSign(&x);
    realFMA(&x, a, a, &x, &ctxtReal75);   // a - p * a
    realFMA(p, b, &x, res, &ctxtReal75);  // p * b + (a - p * a)
  } else {
    /* a * b > 0, use linpol = a + p * (b - a) */
    realSubtract(b, a, &x, &ctxtReal75);
    realFMA(&x, p, a, res, &ctxtReal75);
  }
}

/********************************************//**
 * \brief (p, b, a) ==> (r) p ==> regL
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnLINPOL(uint16_t unusedButMandatoryParameter) {
  real_t aReal, bReal, aImag, bImag, p, rReal, rImag;
  bool_t realCoefs = true;

  realZero(&aImag);
  realZero(&bImag);

  switch(getRegisterDataType(REGISTER_X)) {
    case dtReal34: {
      real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &p);
      break;
    }
    case dtLongInteger: {
      convertLongIntegerRegisterToReal(REGISTER_X, &p, &ctxtReal75);
      break;
    }

    default: {
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "cannot LINPOL with %s in X", getRegisterDataTypeName(REGISTER_X, true, false));
        moreInfoOnError("In function fnLINPOL:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }
  }

  switch(getRegisterDataType(REGISTER_Y)) {
    case dtLongInteger: {
      convertLongIntegerRegisterToReal(REGISTER_Y, &bReal, &ctxtReal75);
      break;
    }

    case dtShortInteger: {
      convertShortIntegerRegisterToReal(REGISTER_Y, &bReal, &ctxtReal39);
      break;
    }

    case dtReal34: {
      real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &bReal);
      break;
    }

    case dtComplex34: {
      real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &bReal);
      real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &bImag);
      realCoefs = false;
      break;
    }

    default: {
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_Y);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "cannot LINPOL with %s in Y", getRegisterDataTypeName(REGISTER_Y, true, false));
        moreInfoOnError("In function fnLINPOL:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }
  }

  switch(getRegisterDataType(REGISTER_Z)) {
    case dtLongInteger: {
      convertLongIntegerRegisterToReal(REGISTER_Z, &aReal, &ctxtReal75);
      break;
    }

    case dtShortInteger: {
      convertShortIntegerRegisterToReal(REGISTER_Z, &aReal, &ctxtReal39);
      break;
    }

    case dtReal34: {
      real34ToReal(REGISTER_REAL34_DATA(REGISTER_Z), &aReal);
      break;
    }

    case dtComplex34: {
      real34ToReal(REGISTER_REAL34_DATA(REGISTER_Z), &aReal);
      real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Z), &aImag);
      realCoefs = false;
      break;
    }

    default: {
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_Z);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "cannot LINPOL with %s in Z", getRegisterDataTypeName(REGISTER_Z, true, false));
        moreInfoOnError("In function fnLINPOL:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }
  }

  if (!saveLastX())
    return;

  adjustResult(REGISTER_X, false, true, REGISTER_X, REGISTER_Y, REGISTER_Z);
  fnDrop(NOPARAM);
  fnDrop(NOPARAM);

  linpol(&aReal, &bReal, &p, &rReal);
  if (realCoefs) {
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    convertRealToReal34ResultRegister(&rReal, REGISTER_X);
  } else {
    linpol(&aImag, &bImag, &p, &rImag);
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    convertRealToReal34ResultRegister(&rReal, REGISTER_X);
    convertRealToImag34ResultRegister(&rImag, REGISTER_X);
  }
}
