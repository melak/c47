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
 * \file integerPart.c
 ***********************************************/

#include "wp43s.h"



void (* const ip[9])(void) = {
// regX ==> 1            2       3         4        5        6        7          8           9
//          Long integer Real34  complex34 Time     Date     String   Real34 mat Complex34 m Short integer
            ipLonI,      ipReal, ipError,  ipError, ipError, ipError, ipRema,    ipError,    ipShoI
};



/********************************************//**
 * \brief Data type error in IP
 *
 * \param void
 * \return void
 ***********************************************/
void ipError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate IP for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnIp:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and IP(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnIp(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  ip[getRegisterDataType(REGISTER_X)]();

  if(lastErrorCode == 0) {
    refreshRegisterLine(REGISTER_X);
  }
  else {
    restoreStack();
    refreshStack();
  }
}



void ipLonI(void) {
}



void ipRema(void) {
  fnToBeCoded();
}



void ipShoI(void) {
}



void ipReal(void) {
  convertReal34ToLongIntegerRegister(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_X, DEC_ROUND_DOWN);
}
