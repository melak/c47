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

#include "registerValueConversions.h"

#include "charString.h"
#include "constantPointers.h"
#include "dateTime.h"
#include "display.h"
#include "error.h"
#include "integers.h"
#include "matrix.h"
#include "registers.h"

#include "wp43s.h"



void convertLongIntegerToLongIntegerRegister(const longInteger_t lgInt, calcRegister_t regist) {
  uint16_t sizeInBytes = longIntegerSizeInBytes(lgInt);

  reallocateRegister(regist, dtLongInteger, TO_BLOCKS(sizeInBytes), longIntegerSignTag(lgInt));
  xcopy(REGISTER_LONG_INTEGER_DATA(regist), lgInt->_mp_d, sizeInBytes);
}



void convertLongIntegerRegisterToLongInteger(calcRegister_t regist, longInteger_t lgInt) {
  uint32_t sizeInBytes = TO_BYTES(getRegisterMaxDataLength(regist));

  longIntegerInitSizeInBits(lgInt, 8 * max(sizeInBytes, LIMB_SIZE));

  xcopy(lgInt->_mp_d, REGISTER_LONG_INTEGER_DATA(regist), sizeInBytes);

  if(getRegisterLongIntegerSign(regist) == LI_NEGATIVE) {
    lgInt->_mp_size = -(sizeInBytes / LIMB_SIZE);
  }
  else {
    lgInt->_mp_size = sizeInBytes / LIMB_SIZE;
  }
}



void convertLongIntegerRegisterToReal34Register(calcRegister_t source, calcRegister_t destination) {
  longInteger_t lgInt;

  convertLongIntegerRegisterToLongInteger(source, lgInt);
  longIntegerToAllocatedString(lgInt, tmpString, TMP_STR_LENGTH);
  longIntegerFree(lgInt);
  reallocateRegister(destination, dtReal34, REAL34_SIZE, amNone);
  stringToReal34(tmpString, REGISTER_REAL34_DATA(destination));
}


void convertLongIntegerRegisterToReal34(calcRegister_t source, real34_t *destination) {
  longInteger_t lgInt;

  convertLongIntegerRegisterToLongInteger(source, lgInt);
  longIntegerToAllocatedString(lgInt, tmpString, TMP_STR_LENGTH);
  longIntegerFree(lgInt);
  stringToReal34(tmpString, destination);
}


void convertLongIntegerRegisterToReal(calcRegister_t source, real_t *destination, realContext_t *ctxt) {
  longInteger_t lgInt;

  convertLongIntegerRegisterToLongInteger(source, lgInt);
  convertLongIntegerToReal(lgInt, destination, ctxt);
  longIntegerFree(lgInt);
}



void convertLongIntegerToReal(longInteger_t source, real_t *destination, realContext_t *ctxt) {
  longIntegerToAllocatedString(source, tmpString, TMP_STR_LENGTH);
  stringToReal(tmpString, destination, ctxt);
}



void convertLongIntegerToShortIntegerRegister(longInteger_t lgInt, uint32_t base, calcRegister_t destination) {
  reallocateRegister(destination, dtShortInteger, SHORT_INTEGER_SIZE, base);
  if(longIntegerIsZero(lgInt)) {
    *(REGISTER_SHORT_INTEGER_DATA(destination)) = 0;
  }
  else {
    #ifdef OS32BIT // 32 bit
      uint64_t u64 = *(uint32_t *)(lgInt->_mp_d);
      if(abs(lgInt->_mp_size) > 1) {
        u64 |= (int64_t)(*(((uint32_t *)(lgInt->_mp_d)) + 1)) << 32;
      }
      *(REGISTER_SHORT_INTEGER_DATA(destination)) = u64 & shortIntegerMask;
    #else // 64 bit
      *(REGISTER_SHORT_INTEGER_DATA(destination)) = *(uint64_t *)(lgInt->_mp_d) & shortIntegerMask;
    #endif // OS32BIT
    if(longIntegerIsNegative(lgInt)) {
      *(REGISTER_SHORT_INTEGER_DATA(destination)) = WP34S_intChs(*(REGISTER_SHORT_INTEGER_DATA(destination)));
    }
  }
}



void convertLongIntegerRegisterToShortIntegerRegister(calcRegister_t source, calcRegister_t destination) {
  longInteger_t lgInt;

  convertLongIntegerRegisterToLongInteger(source, lgInt);
  convertLongIntegerToShortIntegerRegister(lgInt, 10, destination);
  longIntegerFree(lgInt);
}



void convertShortIntegerRegisterToReal34Register(calcRegister_t source, calcRegister_t destination) {
  uint64_t value;
  int16_t sign;
  real34_t lowWord;

  convertShortIntegerRegisterToUInt64(source, &sign, &value);
  reallocateRegister(destination, dtReal34, REAL34_SIZE, amNone);

  uInt32ToReal34(value >> 32, REGISTER_REAL34_DATA(destination));
  uInt32ToReal34(value & 0x00000000ffffffff, &lowWord);
  real34FMA(REGISTER_REAL34_DATA(destination), const34_2p32, &lowWord, REGISTER_REAL34_DATA(destination));

  if(sign) {
    real34SetNegativeSign(REGISTER_REAL34_DATA(destination));
  }
}



void convertShortIntegerRegisterToReal(calcRegister_t source, real_t *destination, realContext_t *ctxt) {
  uint64_t value;
  int16_t sign;
  real_t lowWord;

  convertShortIntegerRegisterToUInt64(source, &sign, &value);

  uInt32ToReal(value >> 32, destination);
  uInt32ToReal(value & 0x00000000ffffffff, &lowWord);
  realFMA(destination, const_2p32, &lowWord, destination, ctxt);

  if(sign) {
    realSetNegativeSign(destination);
  }
}



void convertShortIntegerRegisterToUInt64(calcRegister_t regist, int16_t *sign, uint64_t *value) {
  *value = *(REGISTER_SHORT_INTEGER_DATA(regist)) & shortIntegerMask;

  if(shortIntegerMode == SIM_UNSIGN) {
    *sign = 0;
  }
  else {
    if(*value & shortIntegerSignBit) { // Negative value
      *sign = 1;

      if(shortIntegerMode == SIM_2COMPL) {
        *value = ((~*value) + 1) & shortIntegerMask;
      }
      else if(shortIntegerMode == SIM_1COMPL) {
        *value = (~*value) & shortIntegerMask;
      }
      else if(shortIntegerMode == SIM_SIGNMT) {
       *value -= shortIntegerSignBit;
      }
      else {
        sprintf(errorMessage, "In function convertShortIntegerRegisterToUInt64: %" PRIu8 " is an unexpected value for shortIntegerMode!", shortIntegerMode);
        displayBugScreen(errorMessage);
        *sign = 0;
        *value = 0;
      }
    }
    else { // Positive value
      *sign = 0;
    }
  }
}



void convertShortIntegerRegisterToLongInteger(calcRegister_t source, longInteger_t lgInt) {
  uint64_t value;
  int16_t sign;

  convertShortIntegerRegisterToUInt64(source, &sign, &value);

  longIntegerInit(lgInt);
  uIntToLongInteger(value >> 32, lgInt);
  longIntegerLeftShift(lgInt, 32, lgInt);
  longIntegerAddUInt(lgInt, value & 0x00000000ffffffff, lgInt);

  if(sign) {
    longIntegerChangeSign(lgInt);
  }
}



void convertShortIntegerRegisterToLongIntegerRegister(calcRegister_t source, calcRegister_t destination) {
  longInteger_t lgInt;

  convertShortIntegerRegisterToLongInteger(source, lgInt);

  convertLongIntegerToLongIntegerRegister(lgInt, destination);
  longIntegerFree(lgInt);
}



void convertUInt64ToShortIntegerRegister(int16_t sign, uint64_t value, uint32_t base, calcRegister_t regist) {
  if(sign) { // Negative value
    if(shortIntegerMode == SIM_2COMPL) {
      value = (~value) + 1;
    }
    else if(shortIntegerMode == SIM_1COMPL) {
      value = ~value;
    }
    else if(shortIntegerMode == SIM_SIGNMT) {
      value += shortIntegerSignBit;
    }
    else {
      sprintf(errorMessage, "In function convertUInt64ToShortIntegerRegister: %" PRIu8 " is an unexpected value for shortIntegerMode!", shortIntegerMode);
      displayBugScreen(errorMessage);
      value = 0;
    }
  }

  reallocateRegister(regist, dtShortInteger, SHORT_INTEGER_SIZE, base);
  *(REGISTER_SHORT_INTEGER_DATA(regist)) = value & shortIntegerMask;
}



void convertReal34ToLongInteger(const real34_t *re34, longInteger_t lgInt, enum rounding roundingMode) {
  uint8_t bcd[DECQUAD_Pmax];
  int32_t sign, exponent;
  real34_t real34;

  real34ToIntegralValue(re34, &real34, roundingMode);
  sign = real34GetCoefficient(&real34, bcd);
  exponent = real34GetExponent(&real34);

  longIntegerInit(lgInt);
  uIntToLongInteger(bcd[0], lgInt);

  for(int i=1; i<DECQUAD_Pmax; i++) {
    longIntegerMultiplyUInt(lgInt, 10, lgInt);
    longIntegerAddUInt(lgInt, bcd[i], lgInt);
  }

  while(exponent > 0) {
    longIntegerMultiplyUInt(lgInt, 10, lgInt);
    exponent--;
  }

  if(sign) {
    longIntegerChangeSign(lgInt);
  }
}



void convertReal34ToLongIntegerRegister(const real34_t *real34, calcRegister_t dest, enum rounding roundingMode) {
  longInteger_t lgInt;

  convertReal34ToLongInteger(real34, lgInt, roundingMode);
  convertLongIntegerToLongIntegerRegister(lgInt, dest);

  longIntegerFree(lgInt);
}



void convertRealToLongInteger(const real_t *re, longInteger_t lgInt, enum rounding roundingMode) {
  uint8_t bcd[75];
  int32_t sign, exponent;
  real_t real;

  realToIntegralValue(re, &real, roundingMode, &ctxtReal39);
  realGetCoefficient(&real, bcd);
  sign = (realIsNegative(&real) ? 1 : 0);
  exponent = real.exponent;

  uIntToLongInteger(bcd[0], lgInt);

  for(int i=1; i<real.digits; i++) {
    longIntegerMultiplyUInt(lgInt, 10, lgInt);
    longIntegerAddUInt(lgInt, bcd[i], lgInt);
  }

  while(exponent > 0) {
    longIntegerMultiplyUInt(lgInt, 10, lgInt);
    exponent--;
  }

  if(sign) {
    longIntegerChangeSign(lgInt);
  }
}



void convertRealToLongIntegerRegister(const real_t *real, calcRegister_t dest, enum rounding roundingMode) {
  longInteger_t lgInt;

  longIntegerInit(lgInt);

  convertRealToLongInteger(real, lgInt, roundingMode);
  convertLongIntegerToLongIntegerRegister(lgInt, dest);
  longIntegerFree(lgInt);
}



void realToIntegralValue(const real_t *source, real_t *destination, enum rounding mode, realContext_t *realContext) {
  enum rounding savedRoundingMode;

  savedRoundingMode = realContext->round;
  realContext->round = mode;
  realContext->status = 0;
  decNumberToIntegralValue(destination, source, realContext);
  realContext->round = savedRoundingMode;
}



void realToUInt32(const real_t *re, enum rounding mode, uint32_t *value32, bool_t *overflow) {
  uint8_t bcd[76], sign;
  real_t real;
  longInteger_t lgInt;

  realToIntegralValue(re, &real, mode, &ctxtReal75);
  sign = real.bits & 0x80;
  realGetCoefficient(&real, bcd);

  longIntegerInit(lgInt);
  uIntToLongInteger(bcd[0], lgInt);

  for(int i=1; i<real.digits; i++) {
    longIntegerMultiplyUInt(lgInt, 10, lgInt);
    longIntegerAddUInt(lgInt, bcd[i], lgInt);
  }

  while(real.exponent > 0) {
    longIntegerMultiplyUInt(lgInt, 10, lgInt);
    real.exponent--;
  }

  *overflow = false;

  #ifdef OS32BIT // 32 bit
    *value32 = (lgInt->_mp_size == 0 ? 0 : lgInt->_mp_d[0]);
    if(sign || lgInt->_mp_size > 1) {
      *overflow = true;
    }
  #else // 64 bit
    *value32 = (lgInt->_mp_size == 0 ? 0 : lgInt->_mp_d[0] & 0x00000000ffffffffULL);
    if(sign || lgInt->_mp_size > 1 || lgInt->_mp_d[0] & 0xffffffff00000000ULL) {
      *overflow = true;
    }
  #endif // OS32BIT

  longIntegerFree(lgInt);
}



void convertTimeRegisterToReal34Register(calcRegister_t source, calcRegister_t destination) {
  real34_t real34, value34;
  real34Copy(REGISTER_REAL34_DATA(source), &real34);
  int32ToReal34(3600, &value34);
  reallocateRegister(destination, dtReal34, REAL34_SIZE, amNone);
  real34Divide(&real34, &value34, REGISTER_REAL34_DATA(destination));
}



void convertReal34RegisterToTimeRegister(calcRegister_t source, calcRegister_t destination) {
  real34_t real34, value34;
  real34Copy(REGISTER_REAL34_DATA(source), &real34);
  int32ToReal34(3600, &value34);
  reallocateRegister(destination, dtTime, REAL34_SIZE, amNone);
  real34Multiply(&real34, &value34, REGISTER_REAL34_DATA(destination));
}



void convertLongIntegerRegisterToTimeRegister(calcRegister_t source, calcRegister_t destination) {
  convertLongIntegerRegisterToReal34Register(source, destination);
  convertReal34RegisterToTimeRegister(destination, destination);
}



void convertDateRegisterToReal34Register(calcRegister_t source, calcRegister_t destination) {
  real34_t y, m, d, j, val;
  bool_t isNegative;

  internalDateToJulianDay(REGISTER_REAL34_DATA(source), &j);
  decomposeJulianDay(&j, &y, &m, &d);
  isNegative = real34IsNegative(&y);
  real34SetPositiveSign(&y);

  if(getSystemFlag(FLAG_YMD)) {
    int32ToReal34(100, &val), real34Divide(&m, &val, &m);
    int32ToReal34(10000, &val), real34Divide(&d, &val, &d);
  }
  else if(getSystemFlag(FLAG_MDY)) {
    int32ToReal34(100, &val), real34Divide(&d, &val, &d);
    int32ToReal34(1000000, &val), real34Divide(&y, &val, &y);
  }
  else if(getSystemFlag(FLAG_DMY)) {
    int32ToReal34(100, &val), real34Divide(&m, &val, &m);
    int32ToReal34(1000000, &val), real34Divide(&y, &val, &y);
  }

  reallocateRegister(destination, dtReal34, REAL34_SIZE, amNone);
  real34Add(&y, &m, REGISTER_REAL34_DATA(destination));
  real34Add(REGISTER_REAL34_DATA(destination), &d, REGISTER_REAL34_DATA(destination));
  if(isNegative) real34SetNegativeSign(REGISTER_REAL34_DATA(destination));
}



void convertReal34RegisterToDateRegister(calcRegister_t source, calcRegister_t destination) {
  real34_t part1, part2, part3, val;
  bool_t isNegative;

  isNegative = real34IsNegative(REGISTER_REAL34_DATA(source));
  real34CopyAbs(REGISTER_REAL34_DATA(source), &part2);
  real34ToIntegralValue(&part2, &part1, DEC_ROUND_DOWN);
  real34Subtract(&part2, &part1, &part2);
  int32ToReal34(100, &val), real34Multiply(&part2, &val, &part2);

  real34Copy(&part2, &part3);
  real34ToIntegralValue(&part2, &part2, DEC_ROUND_DOWN);
  real34Subtract(&part3, &part2, &part3);
  int32ToReal34(getSystemFlag(FLAG_YMD) ? 100 : 10000, &val), real34Multiply(&part3, &val, &part3);
  real34ToIntegralValue(&part3, &part3, DEC_ROUND_DOWN);

  if(isNegative) {
    if(getSystemFlag(FLAG_YMD)) real34SetNegativeSign(&part1);
    else real34SetNegativeSign(&part3);
  }

  if((getSystemFlag(FLAG_YMD) && !isValidDay(&part1, &part2, &part3)) ||
    ( getSystemFlag(FLAG_MDY) && !isValidDay(&part3, &part1, &part2)) ||
    ( getSystemFlag(FLAG_DMY) && !isValidDay(&part3, &part2, &part1))) {
      displayCalcErrorMessage(ERROR_BAD_TIME_OR_DATE_INPUT, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function convertReal34RegisterToDateRegister:", "Invalid date input like 30 Feb.", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
  }

  reallocateRegister(destination, dtDate, REAL34_SIZE, amNone);
  if     (getSystemFlag(FLAG_YMD)) composeJulianDay(&part1, &part2, &part3, REGISTER_REAL34_DATA(destination));
  else if(getSystemFlag(FLAG_MDY)) composeJulianDay(&part3, &part1, &part2, REGISTER_REAL34_DATA(destination));
  else if(getSystemFlag(FLAG_DMY)) composeJulianDay(&part3, &part2, &part1, REGISTER_REAL34_DATA(destination));

  int32ToReal34(86400, &val), real34Multiply(REGISTER_REAL34_DATA(destination), &val, REGISTER_REAL34_DATA(destination));
  int32ToReal34(43200, &val), real34Add(REGISTER_REAL34_DATA(destination), &val, REGISTER_REAL34_DATA(destination));
}



#ifndef TESTSUITE_BUILD
void convertReal34MatrixRegisterToReal34Matrix(calcRegister_t regist, real34Matrix_t *matrix) {

  dataBlock_t *dblock           = REGISTER_REAL34_MATRIX_DBLOCK(regist);
  real34_t    *matrixElem     = REGISTER_REAL34_MATRIX_M_ELEMENTS(regist);

  realMatrixInit(matrix, dblock->matrixRows, dblock->matrixColumns);
  xcopy(matrix->matrixElements, REGISTER_REAL34_MATRIX_M_ELEMENTS(regist), (matrix->header.matrixColumns * matrix->header.matrixRows) * sizeof(real34_t));

  for(int i = 0; i < matrix->header.matrixColumns * matrix->header.matrixRows; i++) {
    real34Copy(&matrixElem[i], &matrix->matrixElements[i]);
  }
}

void convertReal34MatrixToReal34MatrixRegister(const real34Matrix_t *matrix, calcRegister_t regist) {
  reallocateRegister(regist, dtReal34Matrix, TO_BLOCKS((matrix->header.matrixColumns * matrix->header.matrixRows) * sizeof(real34_t)), amNone);
  xcopy(REGISTER_REAL34_MATRIX(regist), matrix, sizeof(dataBlock_t));
  xcopy(REGISTER_REAL34_MATRIX_M_ELEMENTS(regist), matrix->matrixElements, (matrix->header.matrixColumns * matrix->header.matrixRows) * sizeof(real34_t));
}

void convertComplex34MatrixRegisterToComplex34Matrix(calcRegister_t regist, complex34Matrix_t *matrix) {

  dataBlock_t *dblock           = REGISTER_COMPLEX34_MATRIX_DBLOCK(regist);
  complex34_t *matrixElem       = REGISTER_COMPLEX34_MATRIX_M_ELEMENTS(regist);

  complexMatrixInit(matrix, dblock->matrixRows, dblock->matrixColumns);
  xcopy(matrix->matrixElements, REGISTER_COMPLEX34_MATRIX_M_ELEMENTS(regist), (matrix->header.matrixColumns * matrix->header.matrixRows) * sizeof(complex34_t));

  for(int i = 0; i < matrix->header.matrixColumns * matrix->header.matrixRows; i++) {
    complex34Copy(&matrixElem[i], &matrix->matrixElements[i]);
  }
}

void convertComplex34MatrixToComplex34MatrixRegister(const complex34Matrix_t *matrix, calcRegister_t regist) {
  reallocateRegister(regist, dtComplex34Matrix, TO_BLOCKS((matrix->header.matrixColumns * matrix->header.matrixRows) * sizeof(complex34_t)), amNone);
  xcopy(REGISTER_COMPLEX34_MATRIX(regist), matrix, sizeof(dataBlock_t));
  xcopy(REGISTER_COMPLEX34_MATRIX_M_ELEMENTS(regist), matrix->matrixElements, (matrix->header.matrixColumns * matrix->header.matrixRows) * sizeof(complex34_t));
}

void convertReal34MatrixToComplex34Matrix(const real34Matrix_t *source, complex34Matrix_t *destination) {
  complexMatrixInit(destination, source->header.matrixRows, source->header.matrixColumns);
  for(uint16_t i = 0; i < source->header.matrixRows * source->header.matrixColumns; ++i) {
    real34Copy(&source->matrixElements[i], VARIABLE_REAL34_DATA(&destination->matrixElements[i]));
    real34Zero(VARIABLE_IMAG34_DATA(&destination->matrixElements[i]));
  }
}

void convertReal34MatrixRegisterToComplex34Matrix(calcRegister_t source, complex34Matrix_t *destination) {
  real34Matrix_t matrix;
  linkToRealMatrixRegister(source, &matrix);
  convertReal34MatrixToComplex34Matrix(&matrix, destination);
}

void convertReal34MatrixRegisterToComplex34MatrixRegister(calcRegister_t source, calcRegister_t destination) {
  complex34Matrix_t matrix;
  convertReal34MatrixRegisterToComplex34Matrix(source, &matrix);
  convertComplex34MatrixToComplex34MatrixRegister(&matrix, destination);
  complexMatrixFree(&matrix);
}
#endif // TESTSUITE_BUILD

