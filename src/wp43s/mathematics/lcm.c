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
 * \file lcm.c
 ***********************************************/

#include "wp43s.h"



void (* const lcm[12][12])(void) = {
// regX |    regY ==>    1            2         3          4         5         6         7         8           9            10            11        12
//      V                Long integer real16    complex16  angle34   Time      Date      String    real16 mat  complex16 m  Short integer real34    complex34
/*  1 Long integer   */ {lcmLonILonI, lcmError, lcmError,  lcmError, lcmError, lcmError, lcmError, lcmError,   lcmError,    lcmShoILonI,  lcmError, lcmError},
/*  2 real16         */ {lcmError,    lcmError, lcmError,  lcmError, lcmError, lcmError, lcmError, lcmError,   lcmError,    lcmError,     lcmError, lcmError},
/*  3 complex16      */ {lcmError,    lcmError, lcmError,  lcmError, lcmError, lcmError, lcmError, lcmError,   lcmError,    lcmError,     lcmError, lcmError},
/*  4 angle34        */ {lcmError,    lcmError, lcmError,  lcmError, lcmError, lcmError, lcmError, lcmError,   lcmError,    lcmError,     lcmError, lcmError},
/*  5 Time           */ {lcmError,    lcmError, lcmError,  lcmError, lcmError, lcmError, lcmError, lcmError,   lcmError,    lcmError,     lcmError, lcmError},
/*  6 Date           */ {lcmError,    lcmError, lcmError,  lcmError, lcmError, lcmError, lcmError, lcmError,   lcmError,    lcmError,     lcmError, lcmError},
/*  7 String         */ {lcmError,    lcmError, lcmError,  lcmError, lcmError, lcmError, lcmError, lcmError,   lcmError,    lcmError,     lcmError, lcmError},
/*  8 real16 mat     */ {lcmError,    lcmError, lcmError,  lcmError, lcmError, lcmError, lcmError, lcmError,   lcmError,    lcmError,     lcmError, lcmError},
/*  9 complex16 mat  */ {lcmError,    lcmError, lcmError,  lcmError, lcmError, lcmError, lcmError, lcmError,   lcmError,    lcmError,     lcmError, lcmError},
/* 10 Short integer  */ {lcmLonIShoI, lcmError, lcmError,  lcmError, lcmError, lcmError, lcmError, lcmError,   lcmError,    lcmShoIShoI,  lcmError, lcmError},
/* 11 real34         */ {lcmError,    lcmError, lcmError,  lcmError, lcmError, lcmError, lcmError, lcmError,   lcmError,    lcmError,     lcmError, lcmError},
/* 12 complex34      */ {lcmError,    lcmError, lcmError,  lcmError, lcmError, lcmError, lcmError, lcmError,   lcmError,    lcmError,     lcmError, lcmError}
};



/********************************************//**
 * \brief Data type error in lcm
 *
 * \param void
 * \return void
 ***********************************************/
void lcmError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate lcm (%s, %s)", getRegisterDataTypeName(REGISTER_Y, true, false), getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnLcm:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and LCM(regY, regX) ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnLcm(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  result = REGISTER_X;
  opY    = allocateTemporaryRegister();
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_Y, opY);
  copySourceRegisterToDestRegister(REGISTER_X, opX);

  lcm[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();
  freeTemporaryRegister(opY);
  freeTemporaryRegister(opX);

  if(lastErrorCode == 0) {
    fnDropY(NOPARAM);
  }
  else {
    restoreStack();
  }

  refreshStack();
}



void lcmLonILonI(void) {
  longInteger_t iOp1, iOp2;

  convertLongIntegerRegisterToLongInteger(opY, &iOp1);
  iOp1.sign = 0;
  convertLongIntegerRegisterToLongInteger(opX, &iOp2);
  iOp2.sign = 0;

  longIntegerLcm(&iOp1, &iOp2, &iOp1);

  convertLongIntegerToLongIntegerRegister(&iOp1, result);
}



void lcmLonIShoI(void) {
  longInteger_t iOp1, iOp2;

  convertLongIntegerRegisterToLongInteger(opY, &iOp1);
  iOp1.sign = 0;
  convertShortIntegerRegisterLongIntegerRegister(opX, opX);
  convertLongIntegerRegisterToLongInteger(opX, &iOp2);
  iOp2.sign = 0;

  longIntegerLcm(&iOp1, &iOp2, &iOp1);

  convertLongIntegerToLongIntegerRegister(&iOp1, result);
}



void lcmShoILonI(void) {
  longInteger_t iOp1, iOp2;

  convertLongIntegerRegisterToLongInteger(opX, &iOp1);
  iOp1.sign = 0;
  convertShortIntegerRegisterLongIntegerRegister(opY, opY);
  convertLongIntegerRegisterToLongInteger(opY, &iOp2);
  iOp2.sign = 0;

  longIntegerLcm(&iOp1, &iOp2, &iOp1);

  convertLongIntegerToLongIntegerRegister(&iOp1, result);
}



void lcmShoIShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(result)) = WP34S_intLCM(*(REGISTER_SHORT_INTEGER_DATA(opY)), *(REGISTER_SHORT_INTEGER_DATA(opX)));
}
