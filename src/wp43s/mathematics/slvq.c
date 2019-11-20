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
 * \file slvq.c
 ***********************************************/

#include "wp43s.h"


// see https://en.wikipedia.org/wiki/Quadratic_formula#Method_2
// r1+r2 = -b/a  &  r1r2 = c/a
/********************************************//**
 * \brief (c, b, a) ==> (x1, x2, r) c ==> regL
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSlvq(uint16_t unusedParamButMandatory) {
  // Check that the input coefficients stored in X, Y and Z have correct types.
  // Allowed types are long, int, real16/34 and complex16/34.
  if(!slvqIsValidRegisterType(REGISTER_X) || !slvqIsValidRegisterType(REGISTER_Y) || !slvqIsValidRegisterType(REGISTER_Z)) {
    return;
  }

  // Check that input coefficients stored in X, Y, Z don't are NaN.
  if(slvqRegisterIsNaN(REGISTER_X) || slvqRegisterIsNaN(REGISTER_Y) || slvqRegisterIsNaN(REGISTER_Z)) {
    return;
  }

  // Based on the coefficient types the results can be Real34 or Complex34.
  const uint32_t type = getCoefficientsType();

  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  opX = allocateTemporaryRegister();      // c
  opY = allocateTemporaryRegister();      // b
  opZ = allocateTemporaryRegister();      // a
  result = REGISTER_X;                    // x1
  result1 = REGISTER_Y;                   // x2
  result2 = REGISTER_Z;                   // r

  // There are two different implementations for real and complex coefficients.
  if(type == dtComplex34) {
    // Copy input into opX/opY/opZ and covert them to Complex34.
    slvqCopyToComplex34(REGISTER_X, opX);
    slvqCopyToComplex34(REGISTER_Y, opY);
    slvqCopyToComplex34(REGISTER_Z, opZ);

    slvqCo51();
  }
  else {
    // Copy input into opX/opY/opZ and covert them to Real34.
    slvqCopyToReal34(REGISTER_X, opX);
    slvqCopyToReal34(REGISTER_Y, opY);
    slvqCopyToReal34(REGISTER_Z, opZ);

    slvqRe51();
  }

  freeTemporaryRegister(opX);
  freeTemporaryRegister(opY);
  freeTemporaryRegister(opZ);

  if(lastErrorCode != 0) {
    restoreStack();
    refreshStack();
  }
  else {
    adjustResult(REGISTER_X, false, false, -1, -1, -1);
    adjustResult(REGISTER_Y, false, false, -1, -1, -1);
    adjustResult(REGISTER_Z, false, false, -1, -1, -1);
  }
}


static uint32_t slvqIsValidRegisterType(calcRegister_t regist)
 {
  uint32_t type = getRegisterDataType(regist);

  if (!(type == dtLongInteger || type == dtShortInteger ||
        type == dtReal16      || type == dtReal34       ||
        type == dtComplex16   || type == dtComplex34))     {
    displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, regist);

    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot calculate the slvq of %s", getRegisterDataTypeName(regist, true, false));
      showInfoDialog("In function fnSlqv:", errorMessage, NULL, NULL);
    #endif
    return 0;
  }

  return 1;
}



static uint32_t slvqRegisterIsNaN(calcRegister_t regist) {
  const uint32_t type = getRegisterDataType(regist);
  const bool_t flag =    (type == dtReal34    && real34IsNaN(REGISTER_REAL34_DATA(regist)))
                      || (type == dtReal16    && real16IsNaN(REGISTER_REAL16_DATA(regist)))
                      || (type == dtComplex34 && (real34IsNaN(REGISTER_REAL34_DATA(regist)) || real34IsNaN(REGISTER_IMAG34_DATA(regist))))
                      || (type == dtComplex16 && (real16IsNaN(REGISTER_REAL16_DATA(regist)) || real16IsNaN(REGISTER_IMAG16_DATA(regist))));

  if(flag) {
    displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, regist);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function slvq:", "cannot use NaN as an input" STD_SUB_2, NULL, NULL);
    #endif
    return 1;
  }

  return 0;
}

static void slvqDomainError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);

  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    showInfoDialog("In function fnSlvq:", "cannot calculate on complex domain.", NULL, NULL);
  #endif
}

static void slvqCoefficientError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);

  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    showInfoDialog("In function slvq:", "cannot use zero as coefficient.", NULL, NULL);
  #endif
}

static void slvqConvertToReal34(calcRegister_t regist) {
  const uint32_t sourceType = getRegisterDataType(regist);

  if(sourceType==dtReal34) {
    return;
  }

  const calcRegister_t tmp = allocateTemporaryRegister();
  reallocateRegister(tmp, dtReal34, REAL34_SIZE, 0);

  switch(sourceType) {
    case dtShortInteger: convertShortIntegerRegisterToReal34Register(regist, tmp);
                         break;

    case dtLongInteger:  convertLongIntegerRegisterToReal34Register(regist, tmp);
                         break;

    case dtReal16:       reallocateRegister(tmp, dtReal16, REAL16_SIZE, 0);
                         real16Copy(REGISTER_REAL16_DATA(regist), REGISTER_REAL16_DATA(tmp));
                         convertRegister16To34(tmp);
                         break;

    case dtComplex34:    real34Copy(REGISTER_REAL34_DATA(regist), REGISTER_REAL34_DATA(tmp));
                         break;

    default:             displayBugScreen("In function slvqConvertRegisterToReal34: the source register to convert must be long, short or real16/34!");
                         break;
  }

  reallocateRegister(regist, dtReal34, REAL34_SIZE, 0);
  copySourceRegisterToDestRegister(tmp, regist);

  freeTemporaryRegister(tmp);
}

static void slvqCopyToReal34(calcRegister_t source, calcRegister_t destination) {
  copySourceRegisterToDestRegister(source, destination);
  slvqConvertToReal34(destination);
}

static void slvqConvertToComplex34(calcRegister_t regist) {
  const uint32_t sourceType = getRegisterDataType(regist);

  if(sourceType==dtComplex34) {
    return;
  }

  const calcRegister_t tmp = allocateTemporaryRegister();
  reallocateRegister(tmp, dtComplex34, COMPLEX34_SIZE, 0);

  switch(sourceType) {
    case dtShortInteger:
    case dtLongInteger:
    case dtReal16:
    case dtReal34:       slvqConvertToReal34(regist);
                         real34Copy(REGISTER_REAL34_DATA(regist), REGISTER_REAL34_DATA(tmp));
                         real34Zero(REGISTER_IMAG34_DATA(tmp));
                         break;

    case dtComplex16:    real16ToReal34(REGISTER_REAL16_DATA(regist), REGISTER_REAL34_DATA(tmp));
                         real34Zero(REGISTER_IMAG34_DATA(tmp));
                         break;

    default:             displayBugScreen("In function slvqConvertRegisterToReal34: the source register to convert must be long, short, real16/34 or complex16/34!");
                         break;
  }

  reallocateRegister(regist, dtComplex34, COMPLEX34_SIZE, 0);
  copySourceRegisterToDestRegister(tmp, regist);

  freeTemporaryRegister(tmp);
}

static void slvqCopyToComplex34(calcRegister_t source, calcRegister_t destination) {
  copySourceRegisterToDestRegister(source, destination);
  slvqConvertToComplex34(destination);
}

static uint32_t getCoefficientsType() {
  const uint32_t xType = getRegisterDataType(REGISTER_X);
  const uint32_t yType = getRegisterDataType(REGISTER_Y);
  const uint32_t zType = getRegisterDataType(REGISTER_Z);

  return (xType == dtComplex16 || xType == dtComplex34 ||
          yType == dtComplex16 || yType == dtComplex34 ||
          zType == dtComplex16 || zType == dtComplex34)
         ? dtComplex34
         : dtReal34;
}

#define A   opZ
#define B   opY
#define C   opX

#define rA   REGISTER_REAL34_DATA(A)
#define rB   REGISTER_REAL34_DATA(B)
#define rC   REGISTER_REAL34_DATA(C)

#define iA   REGISTER_IMAG34_DATA(A)
#define iB   REGISTER_IMAG34_DATA(B)
#define iC   REGISTER_IMAG34_DATA(C)

#define complex34IsZero(regist)    (real34IsZero(REGISTER_REAL34_DATA(regist)) && real34IsZero(REGISTER_IMAG34_DATA(regist)))

/*
 * SLVQ for Real coefficients.
 */
void slvqRe51(void) {
  if(real34IsZero(rA)) {
    slvqCoefficientError();
  }
  else {
    if(real34IsZero(rB) && real34IsZero(rC)) {
      // a*x^2 = 0
      //----------
      // x1 = x2 = 0
      // r = 0;

      slvqConvertToReal34(result);
      slvqConvertToReal34(result1);
      slvqConvertToReal34(result2);

      real34Zero(REGISTER_REAL34_DATA(result));   // x1 = 0
      real34Zero(REGISTER_REAL34_DATA(result1));  // x2 = 0
      real34Zero(REGISTER_REAL34_DATA(result2));  // r = 0
    }
    else if(real34IsZero(rB) && !real34IsZero(rC)) {
      // a*x^2 + c = 0
      //--------------
      // x1 = sqrt(-c/a)
      // x2 = -sqrt(-c/a)
      // r = -4*a*c

      real51_t a, b, c;
      real51_t x, r;

      real34ToReal51(rA, &a);
      real34ToReal51(rB, &b);
      real34ToReal51(rC, &c);

      int32ToReal51(4, &r);       // r = 4
      real51Multiply(&r, &a, &r); // r = 4*a
      real51Multiply(&r, &c, &r); // r = 4*a*c
      real51ChangeSign(&r);       // r = -4*a*c

      slvqConvertToReal34(result2);
      real51ToReal34(&r, REGISTER_REAL34_DATA(result2));

      real51Divide(&c, &a, &x);   // x = c/a

      if(real51IsNegative(&x)) {
        real51ChangeSign(&x);                               // x = -c/a
        real51SquareRoot(&x, &x);                           // x = sqrt(-c/a)

        slvqConvertToReal34(result);
        real51ToReal34(&x, REGISTER_REAL34_DATA(result));   // x1 = sqrt(c/a)

        real51ChangeSign(&x);                               // x = -sqrt(c/a)
        slvqConvertToReal34(result1);
        real51ToReal34(&x, REGISTER_REAL34_DATA(result1));  // x2 = -sqrt(c/a)
      }
      else if(getFlag(FLAG_CPXRES)) {
        real51SquareRoot(&x, &x);                           // x1 = sqrt(c/a)

        reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
        real51ToReal34(&x, REGISTER_IMAG34_DATA(result));
        real34Zero(REGISTER_REAL34_DATA(result));

        reallocateRegister(result1, dtComplex34, COMPLEX34_SIZE, 0);
        real51ToReal34(&x, REGISTER_IMAG34_DATA(result1));
        real34SetNegativeSign(REGISTER_IMAG34_DATA(result1)); // x2 = -sqrt(c/a)
        real34Zero(REGISTER_REAL34_DATA(result1));
      }
      else {
        slvqDomainError();
      }
    }
    else { // (!real34IsZero(b) && !real34IsZero(c))
      // a^2 + b*x + c = 0
      //------------------

      real51_t a, b, c;
      real51_t r, tmp;

      real34ToReal51(rA, &a);
      real34ToReal51(rB, &b);
      real34ToReal51(rC, &c);

      int32ToReal51(4, &tmp);             // tmp = 4
      real51Multiply(&tmp, &a, &tmp);     // tmp = 4*a
      real51Multiply(&tmp, &c, &tmp);     // tmp = 4*a*c
      real51Multiply(&b, &b, &r);         // r = b^2
      real51Subtract(&r, &tmp, &r);       // r = b^2 - 4*a*c
      slvqConvertToReal34(result2);
      real51ToReal34(&r, REGISTER_REAL34_DATA(result2));

      if(real51IsZero(&r)) {
        // r = 0
        // x1 = x2 = -b/(2*a)

        real51_t x;

        int32ToReal51(2, &x);           // x = 2
        real51Divide(&b, &x, &x);       // x = b/2
        real51Divide(&x, &a, &x);       // x = b/(2*a)
        real51ChangeSign(&x);           // x = -b/(2*a)

        slvqConvertToReal34(result);
        real51ToReal34(&x, REGISTER_REAL34_DATA(result));

        slvqConvertToReal34(result1);
        real51ToReal34(&x, REGISTER_REAL34_DATA(result1));
      }
      else if(!real51IsNegative(&r)) {
        // x1 = (-b - sign(b) * sqrt(r)) / (2*a)
        // x2 = c/(a * x1)

        real51_t x;

        int32ToReal51(2, &tmp);         // tmp = 2
        real51Multiply(&a, &tmp, &tmp); // tmp = 2*a

        real51SquareRoot(&r, &r);       // r = sqrt(b^2 - 4*a*c)
        real51ChangeSign(&b);           // b = -b

        if(real51IsNegative(&b)) {
          real51Add(&b, &r, &x);      // x = -b + sqrt(r)
        }
        else {
          real51Subtract(&b, &r, &x); // x = -b - sqrt(r)
        }

        real51Divide(&x, &tmp, &x);     // x = (-b - sign(b) * sqrt(r))/(2*a)
        slvqConvertToReal34(result);
        real51ToReal34(&x, REGISTER_REAL34_DATA(result));

        real51Multiply(&a, &x, &x);     // x = a * x
        real51Divide(&c, &x, &x);       // x = c / (z*x)
        slvqConvertToReal34(result1);
        real51ToReal34(&x, REGISTER_REAL34_DATA(result1));
      }
      else if(getFlag(FLAG_CPXRES)) {
        real51_t x;

        real51ChangeSign(&r);           // r = -(b^2 - 4*a*c)
        real51SquareRoot(&r, &r);       // r = sqrt(-(b^2 - 4*a*c))

        int32ToReal51(2, &tmp);         // tmp = 2
        real51Multiply(&a, &tmp, &tmp); // tmp = 2a

        real51Divide(&r, &tmp, &r);     // r = sqrt(-(b^2 - 4ac))/(2*a)

        real51ChangeSign(&b);           // b = -b
        real51Divide(&b, &tmp, &x);     // x = -b/(2*a)

        slvqConvertToComplex34(result);
        real51ToReal34(&x, REGISTER_REAL34_DATA(result));
        real51ToReal34(&r, REGISTER_IMAG34_DATA(result));  // x1 = -b/(2*a) + i sqrt(-(b^2 - 4*a*c))/(2*a)

        slvqConvertToComplex34(result1);
        real51ToReal34(&x, REGISTER_REAL34_DATA(result1));
        real51ToReal34(&r, REGISTER_IMAG34_DATA(result1));
        real34ChangeSign(REGISTER_IMAG34_DATA(result1));   // x2 = -b/(2*a) - i sqrt(-(b^2 - 4*a*c))/(2*a)
      }
      else {
        slvqDomainError();
      }
    }
  }
}



void slvqCo51(void)
{
    if (complex34IsZero(A))
        slvqCoefficientError();
    else
    {
        slvqConvertToComplex34(result);
        slvqConvertToComplex34(result1);
        slvqConvertToComplex34(result2);

        if (complex34IsZero(B) && complex34IsZero(C))
        {
            // ax^2 = 0
            //---------
            // x1 = x2 = 0
            // r = 0;

            real34Zero(REGISTER_REAL34_DATA(result));
            real34Zero(REGISTER_IMAG34_DATA(result));               // x1 = 0+0i

            real34Zero(REGISTER_REAL34_DATA(result1));
            real34Zero(REGISTER_IMAG34_DATA(result1));              // x2 = 0+0i

            real34Zero(REGISTER_REAL34_DATA(result2));
            real34Zero(REGISTER_IMAG34_DATA(result1));              // r = 0+0i
        }
        else if (complex34IsZero(B) && !complex34IsZero(C))
        {
            // ax^2 + c = 0
            //-------------
            // x1 = sqrt(-c/a)
            // x2 = -sqrt(-c/a)
            // r = -4ac

            complex51_t a, c, tmp;

            real34ToComplex51(rA, iA, &a);
            real34ToComplex51(rC, iC, &c);

            int32ToComplex51(4, 0, &tmp);        // r = 4 + i*0
            complex51Multiply(&tmp, &a, &tmp);        // r = 4*a
            complex51Multiply(&tmp, &c, &tmp);        // r = 4*a*c
            complex51ChangeSign(&tmp, &tmp);            // r = -4*a*c
            real51ToReal34(&tmp.real, REGISTER_REAL34_DATA(result2));
            real51ToReal34(&tmp.img, REGISTER_IMAG34_DATA(result2));

            complex51Divide(&c, &a, &tmp);          // x = c/a
            complex51ChangeSign(&tmp, &tmp);            // x = -c/a
            complex51SquareRoot(&tmp, &tmp);           // x = sqrt(-c/a)
            real51ToReal34(&tmp.real, REGISTER_REAL34_DATA(result));
            real51ToReal34(&tmp.img, REGISTER_IMAG34_DATA(result));

            complex51ChangeSign(&tmp, &tmp);            // x = -sqrt(-c/a)
            real51ToReal34(&tmp.real, REGISTER_REAL34_DATA(result1));
            real51ToReal34(&tmp.img, REGISTER_IMAG34_DATA(result1));
        }
        else if (!complex34IsZero(B) && complex34IsZero(C))
        {
            // ax^2 + bx = 0
            //--------------
            // x1 = 0
            // x2 = -b/a
            // r = b^2

            complex51_t a, b, tmp;

            real34ToComplex51(rA, iA, &a);
            real34ToComplex51(rB, iB, &b);

            complex51Multiply(&b, &b, &tmp);            // r = b*b
            real51ToReal34(&tmp.real, REGISTER_REAL34_DATA(result2));
            real51ToReal34(&tmp.img, REGISTER_IMAG34_DATA(result2));

            real34Zero(REGISTER_REAL34_DATA(result));
            real34Zero(REGISTER_IMAG34_DATA(result));   // x1 = 0+0i

            complex51Divide(&b, &a, &tmp);              // x2 = b/a
            complex51ChangeSign(&tmp, &tmp);            // x2 = -b/a
            real51ToReal34(&tmp.real, REGISTER_REAL34_DATA(result1));
            real51ToReal34(&tmp.img, REGISTER_IMAG34_DATA(result1));
        }
        else // (!complex34IsZero(B) && !complex34IsZero(C))
        {
            // a^2 + bx + c = 0
            //-----------------
            // r = b^2 - 4*a*c
            // x1 = (-b - sqrt(r)) / (2*a)
            // x2 = (-b + sqrt(r)) / (2*a)

            complex51_t a, b, c;

            real34ToComplex51(rA, iA, &a);
            real34ToComplex51(rB, iB, &b);
            real34ToComplex51(rC, iC, &c);

            complex51_t x, r, tmp;

            int32ToComplex51(4, 0, &r);                 // r = 4 + i 0
            complex51Multiply(&a, &r, &r);              // r = 4*a
            complex51Multiply(&c, &r, &r);              // r = 4*a*c
            complex51Multiply(&b, &b, &x);              // x = b^2
            complex51Subtract(&x, &r, &r);              // r = b^2 - 4*a*c
            real51ToReal34(&r.real, REGISTER_REAL34_DATA(result2));
            real51ToReal34(&r.img, REGISTER_IMAG34_DATA(result2));

            complex51SquareRoot(&r, &r);                // r = sqrt(r)
            complex51ChangeSign(&b, &b);                // b = -b
            int32ToComplex51(2, 0, &tmp);               // tmp = 2 + i 0
            complex51Multiply(&tmp, &a, &tmp);          // tmp = 2*a

            complex51Subtract(&b, &r, &x);              // x = -b - sqrt(b^2 - 4*a*c)
            complex51Divide(&x, &tmp, &x);              // x = (-b - sqrt(b^2 - 4*a*c))/(2*a)
            real51ToReal34(&x.real, REGISTER_REAL34_DATA(result));
            real51ToReal34(&x.img, REGISTER_IMAG34_DATA(result));

            complex51Add(&b, &r, &x);                   // x = -b + sqrt(b^2 - 4*a*c)
            complex51Divide(&x, &tmp, &x);              // x = (-b + sqrt(b^2 - 4*a*c))/(2*a)
            real51ToReal34(&x.real, REGISTER_REAL34_DATA(result1));
            real51ToReal34(&x.img, REGISTER_IMAG34_DATA(result1));
        }
        /*
         * If img part is zero then convert to real34.
         */
        if(real34IsZero(REGISTER_IMAG34_DATA(result)))
            slvqConvertToReal34(result);
        if(real34IsZero(REGISTER_IMAG34_DATA(result1)))
            slvqConvertToReal34(result1);
        if(real34IsZero(REGISTER_IMAG34_DATA(result2)))
            slvqConvertToReal34(result2);
    }
}
