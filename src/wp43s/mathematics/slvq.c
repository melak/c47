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

#if 0

static void print(char *name, calcRegister_t regist)
{
    uint32_t type = getRegisterDataType(regist);
    char buffer0[100], buffer1[100];

    switch(type)
    {
        case dtComplex34:
            printf("\n> %s = (%s, %si)\n", name, real34ToString(REGISTER_REAL34_DATA(regist), buffer0),
                    real34ToString(REGISTER_IMAG34_DATA(regist), buffer1));
            break;

        case dtReal34:
            printf("\n> %s = %s\n", name, real34ToString(REGISTER_REAL34_DATA(regist), buffer0));
            break;

        default:
            printf("\n> Unsupported type: %d for %s\n", type, name);
    }
}

#endif //

static uint32_t slvqIsValidRegisterType(calcRegister_t regist)
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

static uint32_t slvqRegisterIsNaN(calcRegister_t regist)
{
    const uint32_t type = getRegisterDataType(regist);
    const bool_t flag =
           (type == dtReal34    && real34IsNaN(REGISTER_REAL34_DATA(regist)))
        || (type == dtReal16    && real16IsNaN(REGISTER_REAL16_DATA(regist)))
        || (type == dtComplex34 && (real34IsNaN(REGISTER_REAL34_DATA(regist)) || real34IsNaN(REGISTER_IMAG34_DATA(regist))))
        || (type == dtComplex16 && (real16IsNaN(REGISTER_REAL16_DATA(regist)) || real16IsNaN(REGISTER_IMAG16_DATA(regist))));

    if(flag==true)
    {
        displayCalcErrorMessage(EXCEEDS_FUNCTION_DOMAIN_ERROR, ERR_REGISTER_LINE, regist);
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function slvq:", "cannot use NaN as an input" STD_SUB_2, NULL, NULL);
#endif
        return 1;
    }

    return 0;
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

        case dtComplex34:
            real34Copy(REGISTER_REAL34_DATA(regist), REGISTER_REAL34_DATA(tmp));
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



typedef void (* func_t)(void);

/*
 * Register Backup
 */
static calcRegister_t backupRegister(calcRegister_t regist)
{
    const calcRegister_t tmp = allocateTemporaryRegister();

    if(regist!=-1)
        copySourceRegisterToDestRegister(regist, tmp);

    return tmp;
}

/*
 * Register restore
 */
static void restoreRegister(calcRegister_t regist, calcRegister_t backup)
{
    if(backup!=-1)
    {
        if(regist!=-1)
            copySourceRegisterToDestRegister(backup, regist);
        freeTemporaryRegister(backup);
    }
}

/*
 * Execute unary function
 */
static void execute_function1(func_t function, calcRegister_t x, calcRegister_t dest)
{
    const calcRegister_t tmp_x = backupRegister(x);
    const calcRegister_t tmp_opX = backupRegister(opX);
    const calcRegister_t tmp_result = backupRegister(result);

    copySourceRegisterToDestRegister(tmp_x, opX);
    copySourceRegisterToDestRegister(opX, result); // Some unary functions use result as opX.

    (*function)();
    const calcRegister_t tmp_execution_result = backupRegister(result);

    restoreRegister(x, tmp_x);
    restoreRegister(opX, tmp_opX);
    restoreRegister(result, tmp_result);

    restoreRegister(dest, tmp_execution_result);
}

/*
 * Execute binary function
 */
static void execute_function2(func_t function, calcRegister_t x, calcRegister_t y, calcRegister_t dest)
{
    const calcRegister_t tmp_x = backupRegister(x);
    const calcRegister_t tmp_y = backupRegister(y);
    const calcRegister_t tmp_opX = backupRegister(opX);
    const calcRegister_t tmp_opY = backupRegister(opY);
    const calcRegister_t tmp_result = backupRegister(result);

    copySourceRegisterToDestRegister(tmp_x, opX);
    copySourceRegisterToDestRegister(tmp_y, opY);

    (*function)();
    const calcRegister_t tmp_execution_result = backupRegister(result);

    restoreRegister(x, tmp_x);
    restoreRegister(y, tmp_y);
    restoreRegister(opX, tmp_opX);
    restoreRegister(opY, tmp_opY);
    restoreRegister(result, tmp_result);

    restoreRegister(dest, tmp_execution_result);
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
void slvqRe51(void)
{
    if(real34IsZero(rA))
        slvqCoefficientError();
    else
    {
        if (real34IsZero(rB) && real34IsZero(rC))
        {
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
        else if (real34IsZero(rB) && !real34IsZero(rC))
        {
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

            if (real51IsNegative(&x))
            {
                real51ChangeSign(&x);                               // x = -c/a
                real51SquareRoot(&x, &x);                           // x = sqrt(-c/a)

                slvqConvertToReal34(result);
                real51ToReal34(&x, REGISTER_REAL34_DATA(result));   // x1 = sqrt(c/a)

                real51ChangeSign(&x);                               // x = -sqrt(c/a)
                slvqConvertToReal34(result1);
                real51ToReal34(&x, REGISTER_REAL34_DATA(result1));  // x2 = -sqrt(c/a)
            }
            else if (getFlag(FLAG_CPXRES))
            {
                real51SquareRoot(&x, &x);                           // x1 = sqrt(c/a)

                reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);
                real51ToReal34(&x, REGISTER_IMAG34_DATA(result));
                real34Zero(REGISTER_REAL34_DATA(result));


                reallocateRegister(result1, dtComplex34, COMPLEX34_SIZE, 0);
                real51ToReal34(&x, REGISTER_IMAG34_DATA(result1));
                real34SetNegativeSign(REGISTER_IMAG34_DATA(result1));// x2 = -sqrt(c/a)
                real34Zero(REGISTER_REAL34_DATA(result1));
            }
            else
                slvqDomainError();
        }
        else // (!real34IsZero(b) && !real34IsZero(c))
        {
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
            // TODO overflow if b > sqrt(MAX_REAL34)
            real51Multiply(&b, &b, &r);         // r = b^2
            real51Subtract(&r, &tmp, &r);       // r = b^2 - 4*a*c
            slvqConvertToReal34(result2);
            real51ToReal34(&r, REGISTER_REAL34_DATA(result2));


            if (real51IsZero(&r))
            {
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
            else if (!real51IsNegative(&r))
            {
                // x1 = (-b - sign(b) * sqrt(r)) / (2*a)
                // x2 = c/(a * x1)

                real51_t x;

                int32ToReal51(2, &tmp);         // tmp = 2
                real51Multiply(&a, &tmp, &tmp); // tmp = 2*a

                real51SquareRoot(&r, &r);       // r = sqrt(b^2 - 4*a*c)
                real51ChangeSign(&b);           // b = -b

                if (real51IsNegative(&b))
                    real51Add(&b, &r, &x);      // x = -b + sqrt(r)
                else
                    real51Subtract(&b, &r, &x); // x = -b - sqrt(r)

                real51Divide(&x, &tmp, &x);     // x = (-b - sign(b) * sqrt(r))/(2*a)
                slvqConvertToReal34(result);
                real51ToReal34(&x, REGISTER_REAL34_DATA(result));

                real51Multiply(&a, &x, &x);     // x = a * x
                real51Divide(&c, &x, &x);       // x = c / (z*x)
                slvqConvertToReal34(result1);
                real51ToReal34(&x, REGISTER_REAL34_DATA(result1));
            }
            else if (getFlag(FLAG_CPXRES))
            {
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
            else
                slvqDomainError();
        }
    }
}


typedef struct {
    real51_t real;
    real51_t img;
} complex51_t;


int complex51IsNaN(complex51_t *op)
{
    return real51IsNaN(&op->real) || real51IsNaN(&op->img);
}

void mulCo51(complex51_t *opX, complex51_t *opY, complex51_t *res)
{
    if(complex51IsNaN(opX) || complex51IsNaN(opY))
    {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function mulCo51:", "cannot use NaN as an input of x", NULL, NULL);
#endif
        return;
    }

    // imaginary part
    real51Multiply(&opY->real, &opX->img, &res->img);
    real51FMA(&opY->img, &opX->real, &res->img, &res->img);

    // real part
    real51_t tmp;
    
    real51Copy(&opY->img, &tmp);
    real51ChangeSign(&tmp);

    real51Multiply(&opY->real, &opX->real, &res->real);
    real51FMA(&tmp, &opX->img, &res->real, &res->real);
}

void divCo51(complex51_t *opX, complex51_t *opY, complex51_t *res)
{
    if(complex51IsNaN(opX) || complex51IsNaN(opY))
    {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divCo51:", "cannot use NaN as an input of /", NULL, NULL);
#endif
        return;
    }

    real51_t tmp, tmp1;

    // denominator
    real51Multiply(&opX->real, &opX->real, &tmp);
    real51FMA(&opX->img, &opX->img, &tmp, &tmp);

    // real part
    real51Multiply(&opY->real, &opX->real, &res->real);
    real51FMA(&opY->img, &opX->img, &res->real, &res->real);
    real51Divide(&res->real, &tmp, &res->real);

    // imaginary part
    real51Multiply(&opY->img, &opX->real, &res->img);
    real51Copy(&opY->real, &tmp1);                          // tmp1 = opY->real
    real51ChangeSign(&tmp1);
    real51FMA(&tmp1, &opX->img, &res->img, &res->img);
    real51Divide(&res->img, &tmp, &res->img);
}

void addCo51(complex51_t *opX, complex51_t *opY, complex51_t *res)
{
    if(complex51IsNaN(opX) || complex51IsNaN(opY))
    {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function divCo51:", "cannot use NaN as an input of /", NULL, NULL);
#endif
        return;
    }

    real51Add(&opX->real, &opY->real, &res->real);
    real51Add(&opX->img, &opY->img, &res->img);
}

void subCo51(complex51_t *opX, complex51_t *opY, complex51_t *res)
{
    if(complex51IsNaN(opX) || complex51IsNaN(opY))
    {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function subCo51:", "cannot use NaN as an input of -", NULL, NULL);
#endif
        return;
    }

    real51Subtract(&opX->real, &opY->real, &res->real);
    real51Subtract(&opX->img, &opY->img, &res->img);
}

void chsCo51(complex51_t *opX, complex51_t *res)
{
    if(complex51IsNaN(opX))
    {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function chsCo51:", "cannot use NaN as an input of chs", NULL, NULL);
#endif
        return;
    }

    if(!getFlag(FLAG_DANGER)) {
        if(real51IsInfinite(&opX->real)) {
            displayCalcErrorMessage(real51IsPositive(&opX->real) ? 5 : 4 , ERR_REGISTER_LINE, REGISTER_X);
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
            showInfoDialog("In function chsCo34:", "cannot change infinity sign of real part while D flag is clear", NULL, NULL);
#endif
            return;
        }

        if(real51IsInfinite(&opX->img)) {
            displayCalcErrorMessage(real51IsPositive(&opX->img) ? 5 : 4 , ERR_REGISTER_LINE, REGISTER_X);
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
            showInfoDialog("In function chsCo51:", "cannot change infinity sign of imaginary part while D flag is clear", NULL, NULL);
#endif
            return;
        }
    }

    real51Copy(&opX->real, &res->real);
    real51ChangeSign(&res->real);

    real51Copy(&opX->img, &res->img);
    real51ChangeSign(&res->img);

    if(real51IsZero(&res->real)) {
        real51SetPositiveSign(&res->real);
    }

    if(real51IsZero(&res->img)) {
        real51SetPositiveSign(&res->img);
    }
}

void sqrtCo51(complex51_t *opX, complex51_t *res)
{
    if(complex51IsNaN(opX))
    {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function sqrtCo51:", "cannot use NaN as an input of sqrt", NULL, NULL);
#endif
        return;
    }

    real34_t real34, img34;
    real34_t magnitude34, theta34;

    real51ToReal34(&opX->real, &real34);
    real51ToReal34(&opX->img, &img34);

    real34RectangularToPolar(&real34, &img34, &magnitude34, &theta34);
    real34SquareRoot(&magnitude34, &magnitude34);
    real34Multiply(&theta34, const34_0_5, &theta34);
    real34PolarToRectangular(&magnitude34, &theta34, &real34, &img34);

    real34ToReal51(&real34, &opX->real);
    real34ToReal51(&img34, &opX->img);
}

void zeroCo51(complex51_t *res)
{
    real51Zero(&res->real);
    real51Zero(&res->img);
}

void int32ToCo51(int real_value, int img_value, complex51_t *res)
{
    int32ToReal51(real_value, &res->real);
    int32ToReal51(img_value, &res->img);
}

void real34ToCo51(real34_t *real_value, real34_t *img_value, complex51_t *res)
{
    real34ToReal51(real_value, &res->real);
    real34ToReal51(img_value, &res->img);
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

            complex51_t a, b, c;

            real34ToCo51(rA, iA, &a);
            real34ToCo51(rB, iB, &b);
            real34ToCo51(rC, iC, &c);

            complex51_t r, x;

            int32ToCo51(4, 0, &r);          // r = 4 + i*0
            mulCo51(&r, &a, &r);            // r = 4*a
            mulCo51(&r, &c, &r);            // r = 4*a*c
            chsCo51(&r, &r);                // r = -4*a*c
            real51ToReal34(&r.real, REGISTER_REAL34_DATA(result2));
            real51ToReal34(&r.img, REGISTER_IMAG34_DATA(result2));

            divCo51(&c, &a, &x);            // x = c/a
            chsCo51(&x, &x);                // x = -c/a
            sqrtCo51(&x, &x);               // x = sqrt(-c/a)
            real51ToReal34(&x.real, REGISTER_REAL34_DATA(result));
            real51ToReal34(&x.img, REGISTER_IMAG34_DATA(result));

            chsCo51(&x, &x);                // x = -sqrt(-c/a)
            real51ToReal34(&x.real, REGISTER_REAL34_DATA(result1));
            real51ToReal34(&x.img, REGISTER_IMAG34_DATA(result1));
        }
        // TODO .....
    }
}

/*
 * SLVQ for Complex coefficients.
 */
void slvqCo34(void)
{
    if(complex34IsZero(A))
        slvqCoefficientError();
    else
    {
        slvqConvertToComplex34(result2);
        slvqConvertToComplex34(result);
        slvqConvertToComplex34(result1);

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

            int32ToReal34(4, REGISTER_REAL34_DATA(result2));        // r = 4 + i*0
            real34Zero(REGISTER_IMAG34_DATA(result2));
            execute_function2(mulCo34Co34, result2, A, result2);    // r = 4*a
            execute_function2(mulCo34Co34, result2, C, result2);    // r = 4*a*c
            execute_function1(chsCo34, result2, result2);           // r = -4*a*c

            execute_function2(divCo34Co34, C, A, result);           // x1 = c/a
            execute_function1(chsCo34, result, result);             // x1 = -c/a
            execute_function1(sqrtCo34, result, result);            // x1 = sqrt(-c/a)

            execute_function1(chsCo34, result, result1);            // x2 = -sqrt(-c/a)
        }
        else if (!complex34IsZero(B) && complex34IsZero(C))
        {
            // ax^2 + bx = 0
            //--------------
            // x1 = 0
            // x2 = -b/a
            // r = b^2

            execute_function2(mulCo34Co34, B, B, result2);          // r = b^2

            real34Zero(REGISTER_REAL34_DATA(result));
            real34Zero(REGISTER_IMAG34_DATA(result));               // x1 = 0+0i

            execute_function2(divCo34Co34, B, A, result1);          // x2 = b/a
            execute_function1(chsCo34, result1, result1);           // x2 = -b/a
        }
        else // (!complex34IsZero(B) && !complex34IsZero(C))
        {
            // a^2 + bx + c = 0
            //-----------------
            // r = b^2 - 4*a*c
            // x1 = (-b - sqrt(r)) / (2*a)
            // x2 = (-b + sqrt(r)) / (2*a)

            int32ToReal34(4, REGISTER_REAL34_DATA(result2));        // r = 4 + i*0
            real34Zero(REGISTER_IMAG34_DATA(result2));
            execute_function2(mulCo34Co34, result2, A, result2);    // r = 4*a
            execute_function2(mulCo34Co34, result2, C, result2);    // r = 4*a*c
            execute_function1(chsCo34, result2, result2);           // r = -4*a*c
            execute_function2(mulCo34Co34, B, B, result1);          // x2 = b^2
            execute_function2(addCo34Co34, result1, result2, result2); // r = b^2 - 4*a*c

            calcRegister_t x = allocateTemporaryRegister();
            calcRegister_t tmp = allocateTemporaryRegister();

            execute_function1(sqrtCo34, result2, x);                // x = sqrt(r)
            execute_function1(chsCo34, B, B);                       // b = -b

            int32ToReal34(2, REGISTER_REAL34_DATA(tmp));            // tmp = 2 + i*0
            real34Zero(REGISTER_IMAG34_DATA(tmp));
            execute_function2(mulCo34Co34, tmp, A, tmp);            // tmp = 2*a

            execute_function2(subCo34Co34, B, x, result);           // x1 = -b - sqrt(r)
            execute_function2(divCo34Co34, result, tmp, result);    // x1 = (-b - sqrt(r))/(2*a)

            execute_function2(addCo34Co34, B, x, result1);          // x2 = -b + sqrt(r)
            execute_function2(divCo34Co34, result1, tmp, result1);  // x2 = (-b - sqrt(r))/(2*a)

            freeTemporaryRegister(x);
            freeTemporaryRegister(tmp);
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
    if(!slvqIsValidRegisterType(REGISTER_X) || !slvqIsValidRegisterType(REGISTER_Y) || !slvqIsValidRegisterType(REGISTER_Z))
        return;
    /*
     * Check that input coefficients stored in X, Y, Z don't are NaN.
     */
    if (slvqRegisterIsNaN(REGISTER_X) || slvqRegisterIsNaN(REGISTER_Y) || slvqRegisterIsNaN(REGISTER_Z))
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
     * Set register results
     */
    result = REGISTER_X;                    // x1
    result1 = REGISTER_Y;                   // x2
    result2 = REGISTER_Z;                   // r

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

        slvqCo51();
    }
    else
    {
        /*
         * Copy input into opX/opY/opZ and covert them to Real34.
         */
        slvqCopyToReal34(REGISTER_X, opX);
        slvqCopyToReal34(REGISTER_Y, opY);
        slvqCopyToReal34(REGISTER_Z, opZ);

        slvqRe51();
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
