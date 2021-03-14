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

/****************************************************//**
 * \file checkValue.c
 *******************************************************/

#include "wp43s.h"


TO_QSPI void (* const CheckValue[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(uint16_t) = {
// regX ==> 1               2               3               4                5                6                7               8               9               10
//          Long integer    Real34          Complex34       Time             Date             String           Real34 matrix   Complex34 mat   Short integer   Config data
            checkValueLonI, checkValueReal, checkValueCplx, checkValueError, checkValueError, checkValueError, checkValueRema, checkValueCxma, checkValueShoI, checkValueError
};


void checkValueError(uint16_t unusedButMandatoryParameter) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot do this for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    moreInfoOnError("In function fnCheckValue:", errorMessage, NULL, NULL);
  #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  temporaryInformation = TI_FALSE;
}

static void checkReal(uint16_t mode) {
  switch(mode) {
    case CHECK_VALUE_COMPLEX:
      temporaryInformation = TI_FALSE;
      return;
    case CHECK_VALUE_REAL:
      temporaryInformation = TI_TRUE;
      return;
    default:
      checkValueError(mode);
  }
}



void fnCheckValue(uint16_t mode) {
  CheckValue[getRegisterDataType(REGISTER_X)](mode);
}



void checkValueLonI(uint16_t mode) {
  longInteger_t val;

  if(mode == CHECK_VALUE_POSITIVE_ZERO || mode == CHECK_VALUE_NEGATIVE_ZERO) { // unlikely true
    convertLongIntegerRegisterToLongInteger(REGISTER_X, val);
    if(mode == CHECK_VALUE_POSITIVE_ZERO)
      temporaryInformation = (getSystemFlag(FLAG_SPCRES) && longIntegerIsZero(val) && longIntegerIsPositive(val)) ? TI_TRUE : TI_FALSE;
    else // mode == CHECK_VALUE_NEGATIVE_ZERO
      temporaryInformation = (getSystemFlag(FLAG_SPCRES) && longIntegerIsZero(val) && longIntegerIsNegative(val)) ? TI_TRUE : TI_FALSE;
    longIntegerFree(val);
    return;
  }
  checkReal(mode);
}



void checkValueRema(uint16_t mode) {
  fnToBeCoded();
}



void checkValueCxma(uint16_t mode) {
  fnToBeCoded();
}



void checkValueShoI(uint16_t mode) {
  if(mode == CHECK_VALUE_POSITIVE_ZERO || mode == CHECK_VALUE_NEGATIVE_ZERO) {
    if(shortIntegerMode == SIM_1COMPL || shortIntegerMode == SIM_SIGNMT) {
      if(mode == CHECK_VALUE_POSITIVE_ZERO && (*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) & shortIntegerMask) == 0) {
          temporaryInformation = TI_TRUE;
          return;
      }
      if(mode == CHECK_VALUE_NEGATIVE_ZERO && (*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) & shortIntegerMask) == (shortIntegerMode == SIM_1COMPL ? shortIntegerMask : shortIntegerSignBit)) {
          temporaryInformation = TI_TRUE;
          return;
      }
    }
    temporaryInformation = TI_FALSE;
    return;
  }
  checkReal(mode);
}



void checkValueReal(uint16_t mode) {
  longInteger_t val;

  switch(mode) {
    case CHECK_VALUE_SPECIAL:
      temporaryInformation = (getSystemFlag(FLAG_SPCRES) && real34IsSpecial(REGISTER_REAL34_DATA(REGISTER_X))) ? TI_TRUE : TI_FALSE;
      return;
    case CHECK_VALUE_NAN:
      temporaryInformation = (getSystemFlag(FLAG_SPCRES) && real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) ? TI_TRUE : TI_FALSE;
      return;
  }
  if(getRegisterAngularMode(REGISTER_X) == amNone) {
    switch(mode) {
      case CHECK_VALUE_POSITIVE_ZERO:
        temporaryInformation = (getSystemFlag(FLAG_SPCRES) && real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) && real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X))) ? TI_TRUE : TI_FALSE;
        return;
      case CHECK_VALUE_NEGATIVE_ZERO:
        temporaryInformation = (getSystemFlag(FLAG_SPCRES) && real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) && real34IsNegative(REGISTER_REAL34_DATA(REGISTER_X))) ? TI_TRUE : TI_FALSE;
        return;
      case CHECK_VALUE_INFINITY:
        if(getSystemFlag(FLAG_SPCRES) && real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
          temporaryInformation = TI_TRUE;
          longIntegerInit(val);
          intToLongInteger(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X)) ? 1 : -1, val);
          convertLongIntegerToLongIntegerRegister(val, REGISTER_X);
          longIntegerFree(val);
        }
        else {
          temporaryInformation = TI_FALSE;
          longIntegerInit(val);
          intToLongInteger(0, val);
          convertLongIntegerToLongIntegerRegister(val, REGISTER_X);
          longIntegerFree(val);
        }
        return;
    }
  }
  checkReal(mode);
}



void checkValueCplx(uint16_t mode) {
  switch(mode) {
    case CHECK_VALUE_COMPLEX:
      temporaryInformation = real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X)) ? TI_FALSE : TI_TRUE;
      return;
    case CHECK_VALUE_REAL:
      temporaryInformation = real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X)) ? TI_TRUE : TI_FALSE;
      return;
    case CHECK_VALUE_SPECIAL:
      temporaryInformation = (getSystemFlag(FLAG_SPCRES) && (real34IsSpecial(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsSpecial(REGISTER_IMAG34_DATA(REGISTER_X)))) ? TI_TRUE : TI_FALSE;
      return;
    case CHECK_VALUE_NAN:
      temporaryInformation = (getSystemFlag(FLAG_SPCRES) && (real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X)))) ? TI_TRUE : TI_FALSE;
      return;
    default:
      checkValueError(mode);
  }
}
