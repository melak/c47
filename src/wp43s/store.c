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

#include "store.h"

#include "charString.h"
#include "debug.h"
#include "error.h"
#include "mathematics/compare.h"
#include "matrix.h"
#include "registerValueConversions.h"
#include "registers.h"

#include "wp43s.h"



bool_t regInRange(uint16_t regist) {
  bool_t inRange = (
    (regist < FIRST_LOCAL_REGISTER + currentNumberOfLocalRegisters) ||
    (regist >= FIRST_NAMED_VARIABLE && regist - FIRST_NAMED_VARIABLE < numberOfNamedVariables));
#ifdef PC_BUILD
  if(!inRange) {
    if(regist >= FIRST_LOCAL_REGISTER && regist <= LAST_LOCAL_REGISTER) {
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
    }
    else if(regist >= FIRST_NAMED_VARIABLE && regist <= LAST_NAMED_VARIABLE) {
      // This error message is not massively useful because it doesn't have the original name
      // But it shouldn't have even got this far if the name doesn't exist
      sprintf(errorMessage, "named register .%02d", regist - FIRST_NAMED_VARIABLE);
    }
    moreInfoOnError("In function regInRange:", errorMessage, " is not defined!", NULL);
  }
#endif
  return inRange;
}



#ifndef TESTSUITE_BUILD
static bool_t storeElementReal(real34Matrix_t *matrix) {
  const int16_t i = getIRegisterAsInt(true);
  const int16_t j = getJRegisterAsInt(true);

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToReal34(REGISTER_X, &matrix->matrixElements[i * matrix->header.matrixColumns + j]);
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &matrix->matrixElements[i * matrix->header.matrixColumns + j]);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Cannot store %s in a matrix", getRegisterDataTypeName(REGISTER_X, true, false));
      moreInfoOnError("In function storeElementReal:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  return true;
}

static bool_t storeElementComplex(complex34Matrix_t *matrix) {
  const int16_t i = getIRegisterAsInt(true);
  const int16_t j = getJRegisterAsInt(true);

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToReal34(REGISTER_X, VARIABLE_REAL34_DATA(&matrix->matrixElements[i * matrix->header.matrixColumns + j]));
    real34Zero(VARIABLE_IMAG34_DATA(&matrix->matrixElements[i * matrix->header.matrixColumns + j]));
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), VARIABLE_REAL34_DATA(&matrix->matrixElements[i * matrix->header.matrixColumns + j]));
    real34Zero(VARIABLE_IMAG34_DATA(&matrix->matrixElements[i * matrix->header.matrixColumns + j]));
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), VARIABLE_REAL34_DATA(&matrix->matrixElements[i * matrix->header.matrixColumns + j]));
    real34Copy(REGISTER_IMAG34_DATA(REGISTER_X), VARIABLE_IMAG34_DATA(&matrix->matrixElements[i * matrix->header.matrixColumns + j]));
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Cannot store %s in a matrix", getRegisterDataTypeName(REGISTER_X, true, false));
      moreInfoOnError("In function storeElementReal:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  return true;
}



static bool_t storeIjReal(real34Matrix_t *matrix) {
  if(getRegisterDataType(REGISTER_X) == dtLongInteger && getRegisterDataType(REGISTER_Y) == dtLongInteger) {
    longInteger_t i, j;
    convertLongIntegerRegisterToLongInteger(REGISTER_Y, i);
    convertLongIntegerRegisterToLongInteger(REGISTER_X, j);
    if(longIntegerCompareInt(i, 0) > 0 && longIntegerCompareUInt(i, matrix->header.matrixRows) <= 0 && longIntegerCompareInt(j, 0) > 0 && longIntegerCompareUInt(j, matrix->header.matrixColumns) <= 0) {
      copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_I);
      copySourceRegisterToDestRegister(REGISTER_X, REGISTER_J);
    }
    else {
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        uint16_t row, col;
        longIntegerToUInt(i, row);
        longIntegerToUInt(j, col);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "(%" PRIu16 ", %" PRIu16 ") out of range", row, col);
        moreInfoOnError("In function storeIJReal:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    longIntegerFree(i);
    longIntegerFree(j);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Cannot store %s in a matrix", getRegisterDataTypeName(REGISTER_X, true, false));
      moreInfoOnError("In function storeIJReal:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  return false;
}

static bool_t storeIjComplex(complex34Matrix_t *matrix) {
  return storeIjReal((real34Matrix_t *)matrix);
}
#endif // TESTSUITE_BUILD



void fnStore(uint16_t regist) {
  if(regInRange(regist)) {
    copySourceRegisterToDestRegister(REGISTER_X, regist);
  }
}



void fnStoreAdd(uint16_t regist) {
  if(regInRange(regist)) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(regist, REGISTER_Y);
    if(getRegisterDataType(REGISTER_Y) == dtShortInteger) {
      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)) &= shortIntegerMask;
    }

    addition[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);
    copySourceRegisterToDestRegister(REGISTER_X, regist);
    copySourceRegisterToDestRegister(SAVED_REGISTER_X, REGISTER_X);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
  }
}



void fnStoreSub(uint16_t regist) {
  if(regInRange(regist)) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(regist, REGISTER_Y);
    if(getRegisterDataType(REGISTER_Y) == dtShortInteger) {
      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)) &= shortIntegerMask;
    }

    subtraction[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);
    copySourceRegisterToDestRegister(REGISTER_X, regist);
    copySourceRegisterToDestRegister(SAVED_REGISTER_X, REGISTER_X);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
  }
}



void fnStoreMult(uint16_t regist) {
  if(regInRange(regist)) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(regist, REGISTER_Y);
    if(getRegisterDataType(REGISTER_Y) == dtShortInteger) {
      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)) &= shortIntegerMask;
    }

    multiplication[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);
    copySourceRegisterToDestRegister(REGISTER_X, regist);
    copySourceRegisterToDestRegister(SAVED_REGISTER_X, REGISTER_X);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
  }
}



void fnStoreDiv(uint16_t regist) {
  if(regInRange(regist)) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(regist, REGISTER_Y);
    if(getRegisterDataType(REGISTER_Y) == dtShortInteger) {
      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)) &= shortIntegerMask;
    }

    division[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);
    copySourceRegisterToDestRegister(REGISTER_X, regist);
    copySourceRegisterToDestRegister(SAVED_REGISTER_X, REGISTER_X);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
  }
}



void fnStoreMin(uint16_t regist) {
  if(regInRange(regist)) {
    registerMin(REGISTER_X, regist, regist);
  }
}



void fnStoreMax(uint16_t regist) {
  if(regInRange(regist)) {
    registerMax(REGISTER_X, regist, regist);
  }
}



void fnStoreConfig(uint16_t regist) {
  reallocateRegister(regist, dtConfig, CONFIG_SIZE, amNone);
  dtConfigDescriptor_t *configToStore = REGISTER_CONFIG_DATA(regist);

  storeToDtConfigDescriptor(shortIntegerMode);
  storeToDtConfigDescriptor(shortIntegerWordSize);
  storeToDtConfigDescriptor(displayFormat);
  storeToDtConfigDescriptor(displayFormatDigits);
  storeToDtConfigDescriptor(groupingGap);
  storeToDtConfigDescriptor(currentAngularMode);
  storeToDtConfigDescriptor(denMax);
  storeToDtConfigDescriptor(displayStack);
  storeToDtConfigDescriptor(firstGregorianDay);
  storeToDtConfigDescriptor(roundingMode);
  storeToDtConfigDescriptor(systemFlags);
  xcopy(configToStore->kbd_usr, kbd_usr, sizeof(kbd_usr));
}



void fnStoreStack(uint16_t regist) {
  uint16_t size = getSystemFlag(FLAG_SSIZE8) ? 8 : 4;

  if(regist + size >= REGISTER_X) {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Cannot execute STOS, destination register is out of range: %d", regist);
      moreInfoOnError("In function fnStoreStack:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else {
    for(int i=0; i<size; i++) {
      copySourceRegisterToDestRegister(REGISTER_X + i, regist + i);
    }
  }
}



void fnStoreElement(uint16_t unusedButMandatoryParameter) {
#ifndef TESTSUITE_BUILD
  callByIndexedMatrix(storeElementReal, storeElementComplex);
#endif // TESTSUITE_BUILD
}



void fnStoreIJ(uint16_t unusedButMandatoryParameter) {
#ifndef TESTSUITE_BUILD
  callByIndexedMatrix(storeIjReal, storeIjComplex);
#endif // TESTSUITE_BUILD
}
