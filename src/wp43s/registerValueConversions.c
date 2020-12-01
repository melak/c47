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
 * \file registerValueConversions.c
 ***********************************************/

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
  reallocateRegister(destination, dtReal34, REAL34_SIZE, AM_NONE);
  stringToReal34(tmpString, REGISTER_REAL34_DATA(destination));
}


/* never used
void convertLongIntegerRegisterToReal34(calcRegister_t source, real34_t *destination) {
  longInteger_t lgInt;

  convertLongIntegerRegisterToLongInteger(source, lgInt);
  longIntegerToAllocatedString(lgInt, tmpString, TMP_STR_LENGTH);
  longIntegerFree(lgInt);
  stringToReal34(tmpString, destination);
}
*/


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
  reallocateRegister(destination, dtReal34, REAL34_SIZE, AM_NONE);

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
  exponent = realGetExponent(&real) - real.digits;

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
