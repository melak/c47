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

#include "wp43s.h"



void (* const unitVector[9])(void) = {
// regX ==> 1                2                3               4                5                6                7                8                9
//          Long integer     Real34           complex34       Time             Date             String           Real16 mat       Complex16 m      Short integer
            unitVectorError, unitVectorError, unitVectorCplx, unitVectorError, unitVectorError, unitVectorError, unitVectorError, unitVectorError, unitVectorError
};



/********************************************//**
 * \brief Data type error in unitVector
 *
 * \param void
 * \return void
 ***********************************************/
void unitVectorError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate the unit vector of %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnUnitVector:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and unitVector(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnUnitVector(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  unitVector[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void unitVectorCplx(void) {
  real39_t a, b, norm;

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
