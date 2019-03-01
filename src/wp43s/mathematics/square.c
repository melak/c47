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
 * \file square.c
 ***********************************************/

#include "wp43s.h"



void (* const square[12])(void) = {
// regX ==> 1             2             3             4             5             6             7             8             9             10             11            12
//          Long integer  real16        complex16     Angle         Time          Angle         String        real16 mat    complex16 m   Short integer  real34        complex34
            mulLonILonI,  mulRe16Re16,  mulCo16Co16,  errorSquare,  errorSquare,  errorSquare,  errorSquare,  mulRm16Rm16,  mulCm16Cm16,  mulShoIShoI,   mulRe34Re34,  mulCo34Co34
};



/********************************************//**
 * \brief Data type error in squaring
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void errorSquare(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot square %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnSquare:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and regX × regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnSquare(uint16_t unusedParamButMandatory) {
  if(square[getRegisterDataType(REGISTER_X)] != errorSquare) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    opY    = allocateTemporaryRegister();
    opX    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, opY);
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    square[getRegisterDataType(REGISTER_X)]();
    freeTemporaryRegister(opY);
    freeTemporaryRegister(opX);

    if(lastErrorCode != 0) {
      restoreStack();
      refreshStack();
    }
    else {
      refreshRegisterLine(REGISTER_X);
    }
  }
  else {
    errorSquare();
  }
}
