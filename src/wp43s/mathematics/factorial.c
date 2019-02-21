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
 * \file factorial.c
 ***********************************************/

#include "wp43s.h"



void (* const fact[12])(void) = {
// regX ==> 1            2          3          4           5           6           7           8           9            10             11         12
//          Big integer  real16     complex16  Date        Time        Date        String      real16 mat  complex16 m  Small integer  real34     complex34
            factBigI,    factRe16,  factCo16,  factError,  factError,  factError,  factError,  factError,  factError,   factSmaI,      factRe34,  factCo34
};



/********************************************//**
 * \brief Data type error in fact
 *
 * \param void
 * \return void
 ***********************************************/
void factError(void) {
  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate x! for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnFactorial:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void factToBeCoded(void) {
  #ifdef PC_BUILD
    sprintf(errorMessage, "fact(%s)", getRegisterDataTypeName(REGISTER_X, false, false));
    showInfoDialog("Operation to be coded:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and fact(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnFactorial(uint16_t unusedParamButMandatory) {
  if(fact[getRegisterDataType(REGISTER_X)] != factError) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    opX    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, opX);

    fact[getRegisterDataType(REGISTER_X)]();
    freeTemporaryRegister(opX);

    refreshStack();
  }
  else {
    factError();
  }
}



void factBigI(void) {
  bigInteger_t temp;

  convertBigIntegerRegisterToBigInteger(opX, &temp);

  if(bigIntegerIsNegative(&temp)) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      bigIntegerToDisplayString(opX, errorMessage + 100);
      sprintf(errorMessage, "cannot calculate factorial(%s)", errorMessage + 100);
      showInfoDialog("In function factBigI:", errorMessage, NULL, NULL);
    #endif
  }
  else if(bigIntegerCompareUInt(&temp, 294) == BIG_INTEGER_GREATER_THAN) {
    displayCalcErrorMessage(8, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      bigIntegerToDisplayString(opX, errorMessage + 100);
      sprintf(errorMessage, "cannot calculate factorial(%s)", errorMessage + 100);
      showInfoDialog("In function factBigI:", errorMessage, NULL, NULL);
    #endif
  }
  else {
    bigInteger_t fact;
    uint32_t counter;

    bigIntegerCopy(&temp, &fact);
    counter = bigIntegerToUInt(&temp) - 1;
    while(counter > 1) {
      bigIntegerMultiplyUInt(&fact, counter--, &fact);
    }
    convertBigIntegerToBigIntegerRegister(&fact, result);
  }
}



void factRe16(void) {
  convertRegister16To34(opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  WP34S_real34Factorial(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  convertRegister34To16(result);
}



void factCo16(void) {
  factToBeCoded();
}



void factSmaI(void) {
  int16_t sign;
  uint64_t value;

  convertSmallIntegerRegisterToUInt64(opX, &sign, &value);

  if(sign == 1) { // Negative value
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      bigIntegerToDisplayString(opX, errorMessage + 100);
      sprintf(errorMessage, "cannot calculate factorial(%s)", errorMessage + 100);
      showInfoDialog("In function factSmaI:", errorMessage, NULL, NULL);
    #endif
  }
  else if(value > 20) {
    displayCalcErrorMessage(8, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      bigIntegerToDisplayString(opX, errorMessage + 100);
      sprintf(errorMessage, "cannot calculate factorial(%s)", errorMessage + 100);
      showInfoDialog("In function factSmaI:", errorMessage, NULL, NULL);
    #endif
  }
  else {
    uint64_t fact;
    uint32_t counter;

    fact = value;
    counter = value - 1;
    while(counter > 1) {
      fact *= counter--;
    }

    if(fact > smallIntegerMask) {
      fnSetFlag(FLAG_OVERFLOW);
    }

    convertUInt64ToSmallIntegerRegister(0, fact, getRegisterBase(opX), result);
  }
}



void factRe34(void) {
  WP34S_real34Factorial(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
}



void factCo34(void) {
  factToBeCoded();
}
