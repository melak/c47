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
 * \file log2.c
 ***********************************************/

#include "wp43s.h"



void (* const logBase2[12])(void) = {
//  regX ==> 1            2          3          4           5           6           7           8           9            10             11         12
//           Big integer  real16     complex16              Time        Date        String      real16 mat  complex16 m  Small integer  real34     complex34
             log2BigI,    log2Re16,  log2Co16,  errorLog2,  errorLog2,  errorLog2,  errorLog2,  log2Rm16,   log2Cm16,    log2SmaI,      log2Re34,  log2Co34
};



/********************************************//**
 * \brief Data type error in log2
 *
 * \param void
 * \return void
 ***********************************************/
void errorLog2(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("errorLog2");
  #endif

  displayCalcErrorMessage(24, REGISTER_T, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate log2 for %s", getRegisterDataTypeName(op1, true, false));
    showInfoDialog("In function fnLog2:", errorMessage, NULL, NULL);
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("errorLog2");
  #endif
}



/********************************************//**
 * \brief Error message for a valid operation to be coded
 *
 * \param void
 * \return void
 ***********************************************/
void log2ToBeCoded(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("log2ToBeCoded");
  #endif

  #ifdef PC_BUILD
    sprintf(errorMessage, "log2(%s)", getRegisterDataTypeName(op1, false, false));
    showInfoDialog("Operation to be coded:", errorMessage, NULL, NULL);
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("log2ToBeCoded");
  #endif
}



/********************************************//**
 * \brief regX ==> regL and log2(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnLog2(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnLog2");
  #endif

  if(logBase2[getRegisterDataType(REGISTER_X)] != errorLog2) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    result = REGISTER_X;
    op1    = allocateTemporaryRegister();
    copySourceRegisterToDestRegister(REGISTER_X, op1);

    logBase2[getRegisterDataType(REGISTER_X)]();
    freeTemporaryRegister(op1);

    refreshStack();
  }
  else {
    errorLog2();
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnLog2");
  #endif
}



void log2BigI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("log2BigI");
  #endif

  bigInteger_t value;

  convertBigIntegerRegisterToBigInteger(op1, &value);

  int32_t signX = value.sign;
  bigIntegerSetPositiveSign(&value);

  if(bigIntegerIsZero(&value) || signX) {
    displayCalcErrorMessage(1, REGISTER_T, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function log2BigI: Cannot calculate the log" STD_SUB_2 " of a number " STD_LESS_EQUAL " 0!", NULL, NULL, NULL);
    #endif

    bigIntegerSetZero(&value);
    convertBigIntegerToBigIntegerRegister(&value, result);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("log2BigI");
    #endif

    return;
  }

  uint32_t log2 = 0;

  if(!bigIntegerIsZero(&value)) {
    bigIntegerDivide2(&value, &value);
    while(!bigIntegerIsZero(&value)) {
      log2++;
      bigIntegerDivide2(&value, &value);
    }
  }

  uIntToBigInteger(log2, &value);
  convertBigIntegerToBigIntegerRegister(&value, result);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("log2BigI");
  #endif
}



void log2Re16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("log2Re16");
  #endif

  real51_t real51;

  real16ToReal51(POINTER_TO_REGISTER_DATA(op1), &real51);
  WP34S_real51Ln(&real51, &real51);
  real51Divide(&real51, const51_ln2, &real51);
  real51ToReal16(&real51, POINTER_TO_REGISTER_DATA(result));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("log2Re16");
  #endif
}



void log2Co16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("log2Co16");
  #endif

  log2ToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("log2Co16");
  #endif
}



void log2Rm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("log2Rm16");
  #endif

  log2ToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("log2Rm16");
  #endif
}



void log2Cm16(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("log2Cm16");
  #endif

  log2ToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("log2Cm16");
  #endif
}



void log2SmaI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("log2SmaI");
  #endif

  *(uint64_t *)(POINTER_TO_REGISTER_DATA(result)) = WP34S_intLog2(*(uint64_t *)(POINTER_TO_REGISTER_DATA(op1)));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("log2SmaI");
  #endif
}



void log2Re34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("log2Re34");
  #endif

  real51_t real51;

  real34ToReal51(POINTER_TO_REGISTER_DATA(op1), &real51);
  WP34S_real51Ln(&real51, &real51);
  real51Divide(&real51, const51_ln2, &real51);
  real51ToReal34(&real51, POINTER_TO_REGISTER_DATA(result));

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("log2Re34");
  #endif
}



void log2Co34(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("log2Co34");
  #endif

  log2ToBeCoded();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("log2Co34");
  #endif
}
