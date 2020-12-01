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
 * \file shiftDigits.c
 ***********************************************/

#include "wp43s.h"


/********************************************//**
 * \brief regX ==> regL and SDL(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnSdl(uint16_t numberOfShifts) {
  if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real_t real;

    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &real);
    real.exponent += numberOfShifts;
    realToReal34(&real, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot SDL %s", getRegisterDataTypeName(REGISTER_X, true, false));
      moreInfoOnError("In function fnSdl:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}



/********************************************//**
 * \brief regX ==> regL and SDR(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnSdr(uint16_t numberOfShifts) {
  if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real_t real;

    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &real);
    real.exponent -= numberOfShifts;
    realToReal34(&real, REGISTER_REAL34_DATA(REGISTER_X));
  }

  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot SDR %s", getRegisterDataTypeName(REGISTER_X, true, false));
      moreInfoOnError("In function fnSdr:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}



