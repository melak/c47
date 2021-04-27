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
 * \file unitVector.c
 ***********************************************/

#include "mathematics/unitVector.h"

#include "debug.h"
#include "error.h"
#include "items.h"
#include "matrix.h"
#include "registers.h"

#include "wp43s.h"



TO_QSPI void (* const unitVector[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX ==> 1                2                3               4                5                6                7               8               9                10
//          Long integer     Real34           complex34       Time             Date             String           Real16 mat      Complex16 m     Short integer    Config data
            unitVectorError, unitVectorError, unitVectorCplx, unitVectorError, unitVectorError, unitVectorError, unitVectorRema, unitVectorCxma, unitVectorError, unitVectorError
};



/********************************************//**
 * \brief Data type error in unitVector
 *
 * \param void
 * \return void
 ***********************************************/
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
void unitVectorError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    sprintf(errorMessage, "cannot calculate the unit vector of %s", getRegisterDataTypeName(REGISTER_X, true, false));
    moreInfoOnError("In function fnUnitVector:", errorMessage, NULL, NULL);
}
#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)



/********************************************//**
 * \brief regX ==> regL and unitVector(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnUnitVector(uint16_t unusedButMandatoryParameter) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  unitVector[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void unitVectorCplx(void) {
  real_t a, b, norm;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  realMultiply(&a, &a, &norm, &ctxtReal39);
  realFMA(&b, &b, &norm, &norm, &ctxtReal39);
  realSquareRoot(&norm, &norm, &ctxtReal39);
  realDivide(&a, &norm, &a, &ctxtReal39);
  realDivide(&b, &norm, &b, &ctxtReal39);

  realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}



void unitVectorRema(void) {
#ifndef TESTSUITE_BUILD
  real34Matrix_t matrix;
  real_t elem, sum;

  linkToRealMatrixRegister(REGISTER_X, &matrix);

  realZero(&sum);
  for(int i = 0; i < matrix.header.matrixRows * matrix.header.matrixColumns; ++i) {
    real34ToReal(&matrix.matrixElements[i], &elem);
    realMultiply(&elem, &elem, &elem, &ctxtReal39);
    realAdd(&sum, &elem, &sum, &ctxtReal39);
  }
  realSquareRoot(&sum, &sum, &ctxtReal39);
  for(int i = 0; i < matrix.header.matrixRows * matrix.header.matrixColumns; ++i) {
    real34ToReal(&matrix.matrixElements[i], &elem);
    realDivide(&elem, &sum, &elem, &ctxtReal39);
    realToReal34(&elem, &matrix.matrixElements[i]);
  }
#endif // TESTSUITE_BUILD
}



void unitVectorCxma(void) {
  fnToBeCoded();
}
