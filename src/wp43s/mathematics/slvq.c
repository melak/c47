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

/********************************************/ /**
 * \file slvq.c
 ***********************************************/

#include "wp43s.h"

static uint32_t slvqCheckRegisterType(calcRegister_t regist)
{
    uint32_t type = getRegisterDataType(regist);

    if (!(type == dtLongInteger || type == dtShortInteger ||
          type == dtReal16      || type == dtReal34       ||
          type == dtComplex16   || type == dtComplex34))
    {
        displayCalcErrorMessage(INVALID_DATA_INPUT_ERROR, ERR_REGISTER_LINE, regist);

#if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "cannot calculate the slvq of %s", getRegisterDataTypeName(regist, true, false));
        showInfoDialog("In function fnSlqv:", errorMessage, NULL, NULL);
#endif
        return 0;
    }

    return 1;
}

static uint32_t slvqCheckRegisterNaN(calcRegister_t regist)
{
    const uint32_t type = getRegisterDataType(regist);
    const bool_t flag = (type == dtReal34 && real34IsNaN(REGISTER_REAL34_DATA(regist)))
        || (type == dtReal16 && real16IsNaN(REGISTER_REAL16_DATA(regist)))
        || (type == dtComplex34 && (real34IsNaN(REGISTER_REAL34_DATA(regist)) || real34IsNaN(REGISTER_IMAG34_DATA(regist))))
        || (type == dtComplex16 && (real16IsNaN(REGISTER_REAL16_DATA(regist)) || real16IsNaN(REGISTER_IMAG16_DATA(regist))));

    if(flag==true)
    {
        displayCalcErrorMessage(EXCEEDS_FUNCTION_DOMAIN_ERROR, ERR_REGISTER_LINE, regist);
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function slvq:", "cannot use NaN as an input" STD_SUB_2, NULL, NULL);
#endif
        return 0;
    }

    return 1;
}

static void slvqDomainError()
{
    displayCalcErrorMessage(EXCEEDS_FUNCTION_DOMAIN_ERROR, ERR_REGISTER_LINE, REGISTER_X);

#if (EXTRA_INFO_ON_CALC_ERROR == 1)
    showInfoDialog("In function fnSlvq:", "cannot calculate on complex domain.", NULL, NULL);
#endif
}

static void slvqCoefficientError()
{
    displayCalcErrorMessage(INVALID_DATA_INPUT_ERROR, ERR_REGISTER_LINE, REGISTER_X);

#if (EXTRA_INFO_ON_CALC_ERROR == 1)
    showInfoDialog("In function slvq:", "cannot use zero as coefficient.", NULL, NULL);
#endif
}

static void slvqConvertToReal34(calcRegister_t regist)
{
    const uint32_t sourceType = getRegisterDataType(regist);

    if(sourceType==dtReal34)
        return;

    const calcRegister_t tmp = allocateTemporaryRegister();
    reallocateRegister(tmp, dtReal34, REAL34_SIZE, 0);

    switch(sourceType)
    {
        case dtShortInteger:
            convertShortIntegerRegisterToReal34Register(regist, tmp);
            break;

        case dtLongInteger:
            convertLongIntegerRegisterToReal34Register(regist, tmp);
            break;

        case dtReal16:
            reallocateRegister(tmp, dtReal16, REAL16_SIZE, 0);
            real16Copy(REGISTER_REAL16_DATA(regist), REGISTER_REAL16_DATA(tmp));
            convertRegister16To34(tmp);
            break;

        default:
            displayBugScreen("In function slvqConvertRegisterToReal34: "
                             "the source register to convert must be long, short or real16/34!");
            break;
    }


    reallocateRegister(regist, dtReal34, REAL34_SIZE, 0);
    copySourceRegisterToDestRegister(tmp, regist);

    freeTemporaryRegister(tmp);
}

static void slvqCopyToReal34(calcRegister_t source, calcRegister_t destination)
{
    copySourceRegisterToDestRegister(source, destination);
    slvqConvertToReal34(destination);
}

static void slvqConvertToComplex34(calcRegister_t regist)
{
    const uint32_t sourceType = getRegisterDataType(regist);

    if(sourceType==dtComplex34)
        return;

    const calcRegister_t tmp = allocateTemporaryRegister();
    reallocateRegister(tmp, dtComplex34, COMPLEX34_SIZE, 0);

    switch(sourceType)
    {
        case dtShortInteger:
        case dtLongInteger:
        case dtReal16:
        case dtReal34:
            slvqConvertToReal34(regist);
            real34Copy(REGISTER_REAL34_DATA(regist), REGISTER_REAL34_DATA(tmp));
            real34Zero(REGISTER_IMAG34_DATA(tmp));
            break;

        case dtComplex16:
            real16ToReal34(REGISTER_REAL16_DATA(regist), REGISTER_REAL34_DATA(tmp));
            real34Zero(REGISTER_IMAG34_DATA(tmp));
            break;

        default:
            displayBugScreen("In function slvqConvertRegisterToReal34: "
                             "the source register to convert must be long, short, real16/34 or complex16/34!");
            break;
    }

    reallocateRegister(regist, dtComplex34, COMPLEX34_SIZE, 0);
    copySourceRegisterToDestRegister(tmp, regist);

    freeTemporaryRegister(tmp);
}

static void slvqCopyToComplex34(calcRegister_t source, calcRegister_t destination)
{
    copySourceRegisterToDestRegister(source, destination);
    slvqConvertToComplex34(destination);
}

static uint32_t getCoefficientsType()
{
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
static void slvqRe34(void)
{
    if(real34IsZero(rA))
        slvqCoefficientError();
    else
    {
        if (real34IsZero(rB) && real34IsZero(rC))
        {
            // a*x^2 = 0
            // x1 = x2 = 0
            // r = 0;

            slvqConvertToReal34(REGISTER_X);
            slvqConvertToReal34(REGISTER_Y);
            slvqConvertToReal34(REGISTER_Z);

            real34Zero(REGISTER_REAL34_DATA(REGISTER_X));  // x1 = 0
            real34Zero(REGISTER_REAL34_DATA(REGISTER_Y));  // x2 = 0
            real34Zero(REGISTER_REAL34_DATA(REGISTER_Z));  // r = 0
        }
        else if (real34IsZero(rB) && !real34IsZero(rC))
        {
            // a*x^2 + c = 0
            // x1 = sqrt(-c/a)
            // x2 = -sqrt(-c/a)
            // r = -4*a*c

            real34_t x;

            slvqConvertToReal34(REGISTER_Z);
            int32ToReal34(4, REGISTER_REAL34_DATA(REGISTER_Z));                                      // r = 4
            real34Multiply(REGISTER_REAL34_DATA(REGISTER_Z), rA, REGISTER_REAL34_DATA(REGISTER_Z));  // r = 4*a
            real34Multiply(REGISTER_REAL34_DATA(REGISTER_Z), rC, REGISTER_REAL34_DATA(REGISTER_Z));  // r = 4*a*c
            real34ChangeSign(REGISTER_REAL34_DATA(REGISTER_Z));                                      // r = -4*a*c

            real34Divide(rC, rA, &x);           // x = c/a

            if(real34IsNegative(&x))
            {
                real34ChangeSign(&x);                               // x = -c/a
                real34SquareRoot(&x, &x);                           // x = sqrt(-c/a)

                slvqConvertToReal34(REGISTER_X);
                real34Copy(&x, REGISTER_REAL34_DATA(REGISTER_X));   // x1 = sqrt(c/a)

                real34ChangeSign(&x);                               // x = -sqrt(c/a)
                slvqConvertToReal34(REGISTER_Y);
                real34Copy(&x, REGISTER_REAL34_DATA(REGISTER_Y));   // x2 = -sqrt(c/a)
            }
            else if (getFlag(FLAG_CPXRES))
            {
                real34SquareRoot(&x, &x);                           // x = sqrt(c/a)

                reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, 0);
                real34Copy(&x, REGISTER_IMAG34_DATA(REGISTER_X));
                real34Zero(REGISTER_REAL34_DATA(REGISTER_X));

                real34SetNegativeSign(&x);                          // x = -sqrt(c/a)

                reallocateRegister(REGISTER_Y, dtComplex34, COMPLEX34_SIZE, 0);
                real34Copy(&x, REGISTER_IMAG34_DATA(REGISTER_Y));
                real34Zero(REGISTER_REAL34_DATA(REGISTER_Y));
            }
            else
                slvqDomainError();
        }
        else if (!real34IsZero(rB) && real34IsZero(rC))
        {
            // a*x^2 + b*x = 0
            // x1 = 0 
            // x2 = -b/a
            // r = b^2

            slvqConvertToReal34(REGISTER_X);
            slvqConvertToReal34(REGISTER_Y);
            slvqConvertToReal34(REGISTER_Z);

            real34Multiply(rB, rB, REGISTER_REAL34_DATA(REGISTER_Z)); // r = b^2

            real34Zero(REGISTER_REAL34_DATA(REGISTER_X));             // x1 = 0

            real34Divide(rB, rA, REGISTER_REAL34_DATA(REGISTER_Y));   // x2 = b/a
            real34ChangeSign(REGISTER_REAL34_DATA(REGISTER_Y));       // x2 = -b/a
        }
        else // (!real34IsZero(b) && !real34IsZero(c))
        {
            // a^2 + b*x + c = 0

            real34_t r, tmp, x;

            int32ToReal34(4, &tmp);             // tmp = 4
            real34Multiply(&tmp, rA, &tmp);     // tmp = 4*a
            real34Multiply(&tmp, rC, &tmp);     // tmp = 4*a*c

            // TODO overflow if b > sqrt(MAX_REAL34)
            real34Multiply(rB, rB, &r);         // r = b^2
            real34Subtract(&r, &tmp, &r);       // r = b^2 - 4*a*c

            slvqConvertToReal34(REGISTER_Z);
            real34Copy(&r, REGISTER_REAL34_DATA(REGISTER_Z));

            if(real34IsZero(&r))
            {
                // r = 0
                // x1 = x2 = -b/(2*a)

                int32ToReal34(2, &tmp);         // tmp = 2
                real34Divide(rB, &tmp, &x);     // x = b/2
                real34Divide(&x, rA, &x);       // x = b/(2*a)
                real34ChangeSign(&x);           // x = -b/(2*a)

                slvqConvertToReal34(REGISTER_X);
                real34Copy(&x, REGISTER_REAL34_DATA(REGISTER_X));

                slvqConvertToReal34(REGISTER_Y);
                real34Copy(&x, REGISTER_REAL34_DATA(REGISTER_Y));
            }
            else if(! real34IsNegative(&r)) 
            {
#if 0
                // x1 = (-b + sqrt(r))/(2*a)
                // x2 = (-b - sqrt(r))/(2*a)

                int32ToReal34(2, &tmp);         // tmp = 2
                real34Multiply(rA, &tmp, &tmp); // tmp = 2*a

                real34SquareRoot(&r, &r);       // r = sqrt(b^2 - 4*a*c)
                real34ChangeSign(rB);           // b = -b

                real34Add(rB, &r, &x);          // x = -b + sqrt(r)
                real34Divide(&x, &tmp, &x);     // x = (-b + sqrt(r))/(2*a)
                slvqConvertToReal34(REGISTER_X);
                real34Copy(&x, REGISTER_REAL34_DATA(REGISTER_X));

                real34Subtract(rB, &r, &x);     // x = (-b - sqrt(r)
                real34Divide(&x, &tmp, &x);     // x = (-b - sqrt(r))/(2*a)
                slvqConvertToReal34(REGISTER_Y);
                real34Copy(&x, REGISTER_REAL34_DATA(REGISTER_Y));
#else
                // x1 = (-b - sign(b) * sqrt(r)) / (2*a)
                // x2 = c/(a * x1)

                int32ToReal34(2, &tmp);         // tmp = 2
                real34Multiply(rA, &tmp, &tmp); // tmp = 2*a

                real34SquareRoot(&r, &r);       // r = sqrt(b^2 - 4*a*c)
                real34ChangeSign(rB);           // b = -b

                if(real34IsNegative(rB))
                    real34Add(rB, &r, &x);      // x = -b + sqrt(r)
                else
                    real34Subtract(rB, &r, &x); // x = -b - sqrt(r)

                real34Divide(&x, &tmp, &x);     // x = (-b - sign(b) * sqrt(r))/(2*a)
                slvqConvertToReal34(REGISTER_X);
                real34Copy(&x, REGISTER_REAL34_DATA(REGISTER_X));

                real34Multiply(rA, &x, &x);     // x = a * x
                real34Divide(rC, &x, &x);       // x = c / (z*x)
                slvqConvertToReal34(REGISTER_Y);
                real34Copy(&x, REGISTER_REAL34_DATA(REGISTER_Y));
#endif
            }
            else if (getFlag(FLAG_CPXRES))
            {
                real34ChangeSign(&r);            // r = -(b^2 - 4*a*c)
                real34SquareRoot(&r, &r);        // r = sqrt(-(b^2 - 4*a*c))

                int32ToReal34(2, &tmp);         // tmp = 2
                real34Multiply(rA, &tmp, &tmp); // tmp = 2a

                real34Divide(&r, &tmp, &r);     // r = sqrt(-(b^2 - 4ac))/(2*a)

                real34ChangeSign(rB);           // b = -b
                real34Divide(rB, &tmp, &x);     // x = -b/(2*a)

                slvqConvertToComplex34(REGISTER_X);
                real34Copy(&x, REGISTER_REAL34_DATA(REGISTER_X));
                real34Copy(&r, REGISTER_IMAG34_DATA(REGISTER_X));   // x1 = -b/(2*a) + i sqrt(-(b^2 - 4*a*c))/(2*a)

                slvqConvertToComplex34(REGISTER_Y);
                real34Copy(&x, REGISTER_REAL34_DATA(REGISTER_Y));
                real34Copy(&r, REGISTER_IMAG34_DATA(REGISTER_Y));
                real34ChangeSign(REGISTER_IMAG34_DATA(REGISTER_Y)); // x2 = -b/(2*a) - i sqrt(-(b^2 - 4*a*c))/(2*a)
            }
            else
                slvqDomainError();
        }
    }
}

/*
 * SLVQ for Complex coefficients.
 */
static void slvqCo34(void)
{
    if(complex34IsZero(A))
        slvqCoefficientError();
    else
    {
        if (complex34IsZero(B) && complex34IsZero(C))
        {
            // ax^2 = 0
            // x1 = x2 = 0
            // r = 0;

            slvqConvertToComplex34(REGISTER_X);
            real34Zero(REGISTER_REAL34_DATA(REGISTER_X));
            real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));   // x1 = 0+0i

            slvqConvertToComplex34(REGISTER_Y);
            real34Zero(REGISTER_REAL34_DATA(REGISTER_Y));
            real34Zero(REGISTER_IMAG34_DATA(REGISTER_Y));   // x2 = 0+0i

            slvqConvertToReal34(REGISTER_Z);
            real34Zero(REGISTER_REAL34_DATA(REGISTER_Y));   // r = 0
        }
        else if (complex34IsZero(B) && !complex34IsZero(C))
        {
            // ax^2 + c = 0
            // x1 = sqrt(-c/a)
            // x2 = -sqrt(-c/a)
            // r = -4ac

            // TODO
        }
        else if (!complex34IsZero(B) && complex34IsZero(C))
        {
            // ax^2 + bx = 0
            // x1 = 0
            // x2 = -b/a
            // r = b^2

            // TODO
        }
        else // (!complex34IsZero(B) && !complex34IsZero(C))
        {
            // a^2 + bx + c = 0

            // TODO
        }
    }
}


/********************************************//**
 * \brief (c, b, a) ==> (x1, x2, r) c ==> regL
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSlvq(uint16_t unusedParamButMandatory)
{
    /*
     * Check that the input coefficients stored in X, Y and Z have correct types.
     * Allowed types are long, int, real16/34 and complex16/34.
     */
    if(!slvqCheckRegisterType(REGISTER_X) || !slvqCheckRegisterType(REGISTER_Y) || !slvqCheckRegisterType(REGISTER_Z))
        return;
    /*
     * Check that input coefficients stored in X, Y, Z don't are NaN.
     */
    if (!slvqCheckRegisterNaN(REGISTER_X) || !slvqCheckRegisterNaN(REGISTER_Y) || !slvqCheckRegisterNaN(REGISTER_Z))
        return;

    /*
     * Based on the coefficient types the results can be Real34 or Complex34.
     */
    const uint32_t type = getCoefficientsType();

    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    /*
     * Allocates tmp register with the copy of coefficients.
     */
    opX = allocateTemporaryRegister();      // c
    opY = allocateTemporaryRegister();      // b
    opZ = allocateTemporaryRegister();      // a
    /*
     * There are two different implementations for real and complex coefficients.
     */
    if(type == dtComplex34)
    {
        /*
         * Copy input into opX/opY/opZ and covert them to Complex34.
         */
        slvqCopyToComplex34(REGISTER_X, opX);
        slvqCopyToComplex34(REGISTER_Y, opY);
        slvqCopyToComplex34(REGISTER_Z, opZ);

        slvqCo34();
    }
    else
    {
        /*
         * Copy input into opX/opY/opZ and covert them to Real34.
         */
        slvqCopyToReal34(REGISTER_X, opX);
        slvqCopyToReal34(REGISTER_Y, opY);
        slvqCopyToReal34(REGISTER_Z, opZ);

        slvqRe34();
    }

    freeTemporaryRegister(opX);
    freeTemporaryRegister(opY);
    freeTemporaryRegister(opZ);

    if (lastErrorCode != 0)
    {
        restoreStack();
        refreshStack();
    }
    else
    {
        adjustResult(REGISTER_X, false, false, -1, -1, -1);
        adjustResult(REGISTER_Y, false, false, -1, -1, -1);
        adjustResult(REGISTER_Z, false, false, -1, -1, -1);
    }
}
