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
 * \file realPart.c
 ***********************************************/

#include "wp43s.h"



void (* const realPart[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX ==> 1              2              3             4              5              6              7              8             9              10
//          Long integer   Real34         Complex34     Time           Date           String         Real34 mat     Complex34 m   Short integer  Config data
            realPartError, realPartReal,  realPartCplx, realPartError, realPartError, realPartError, realPartError, realPartCxma, realPartError, realPartError
};



/********************************************//**
 * \brief Data type error in Re
 *
 * \param void
 * \return void
 ***********************************************/
void realPartError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate Re for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    moreInfoOnError("In function fnRealPart:", errorMessage, NULL, NULL);
  #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
}



/********************************************//**
 * \brief regX ==> regL and Re(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnRealPart(uint16_t unusedButMandatoryParameter) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  realPart[getRegisterDataType(REGISTER_X)]();

  if(lastErrorCode != 0) {
    undo();
  }
}



void realPartCxma(void) {
  fnToBeCoded();
}


void realPartCplx(void) {
  real34_t rp;

  real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &rp);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  real34Copy(&rp, REGISTER_REAL34_DATA(REGISTER_X));
}


void realPartReal(void) {
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}

