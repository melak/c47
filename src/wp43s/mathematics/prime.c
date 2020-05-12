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
 * \file prime.c
 ***********************************************/

#include "wp43s.h"




/* Test if a number is prime or not using a Miller-Rabin test */
const uint8_t smallPrimes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
#define QUICK_CHECK (101*101-1)
#define NUMBER_OF_SMALL_PRIMES (sizeof(smallPrimes) / sizeof(uint8_t))

static bool_t longIntegerIsPrime(longInteger_t primeCandidate) {
  uint32_t i;
  longInteger_t primeCandidateMimus1, s, temp, smallPrime, mod;

  if(longIntegerCompareUInt(primeCandidate, 2) < 0) {
    return false;
  }

  /* Quick check for divisibility by small primes */
  for(i=0; i<NUMBER_OF_SMALL_PRIMES; i++) {
    if(longIntegerCompareUInt(primeCandidate, smallPrimes[i]) == 0) {
      return true;
    }
    else if(longIntegerModuloUInt(primeCandidate, smallPrimes[i]) == 0) {
      return false;
    }
  }

  if(longIntegerCompareUInt(primeCandidate, QUICK_CHECK) < 0) {
    return true;
  }

  #if defined(PC_BUILD) || defined(DMCP_BUILD)
  if(calcMode == CM_NORMAL) {
    showHourGlass();
    #ifdef PC_BUILD
      refreshScreen(NULL);
    #else
      lcd_refresh();
    #endif // PC_BUILD
  }
  #endif

  longIntegerInit(primeCandidateMimus1);
  longIntegerInit(s);
  longIntegerInit(temp);
  longIntegerInit(smallPrime);
  longIntegerInit(mod);
  longIntegerSubtractUInt(primeCandidate, 1, primeCandidateMimus1);
  longIntegerCopy(primeCandidateMimus1, s);

  // Calculate s such as   primeCandidate - 1 = s×2^d and s odd
  while(longIntegerIsEven(s)) {
    longIntegerDivide2Exact(s, s);
  }

  // The loop below should only go from 0 to 12 (primes from 2 to 41) ensuring correct result for candidatePrime < 3 317 044 064 679 887 385 961 981
  // There is a conjecture that when going from 0 to 19 (primes from 2 to 71) the result is correct up to 10^36
  for(i=0; i<NUMBER_OF_SMALL_PRIMES; i++) {
    longIntegerCopy(s, temp);

    uIntToLongInteger(smallPrimes[i], smallPrime);
    longIntegerPowerModulo(smallPrime, temp, primeCandidate, mod);
    while(longIntegerCompare(temp, primeCandidateMimus1) != 0 && longIntegerCompareUInt(mod, 1) != 0 && longIntegerCompare(mod, primeCandidateMimus1) != 0) {
      longIntegerPowerUIntModulo(mod, 2, primeCandidate, mod);
      longIntegerMultiply2(temp, temp);
    }

    if(longIntegerCompare(mod, primeCandidateMimus1) != 0 && longIntegerIsEven(temp)) {
      longIntegerFree(primeCandidateMimus1);
      longIntegerFree(s);
      longIntegerFree(temp);
      longIntegerFree(smallPrime);
      longIntegerFree(mod);
      #if defined(PC_BUILD) || defined(DMCP_BUILD)
        if(calcMode == CM_NORMAL) {
          hideHourGlass();
        }
      #endif
      return false;
    }
  }

  longIntegerFree(primeCandidateMimus1);
  longIntegerFree(s);
  longIntegerFree(temp);
  longIntegerFree(smallPrime);
  longIntegerFree(mod);
  #if defined(PC_BUILD) || defined(DMCP_BUILD)
    if(calcMode == CM_NORMAL) {
      hideHourGlass();
    }
  #endif
  return true;
}



void fnIsPrime(uint16_t unusedButMandatoryParameter) {
  longInteger_t primeCandidate;

  if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
    convertShortIntegerRegisterToLongInteger(REGISTER_X, primeCandidate);
  }

  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_X, primeCandidate);
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    convertReal34ToLongInteger(REGISTER_REAL34_DATA(REGISTER_X), primeCandidate, DEC_ROUND_DOWN);
  }

  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if(EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "the input type %s is not allowed for PRIME?!", getDataTypeName(getRegisterDataType(REGISTER_X), false, false));
      showInfoDialog("In function fnIsPrime:", errorMessage, NULL, NULL);
    #endif
  }

  longIntegerSetPositiveSign(primeCandidate);
  temporaryInformation = (longIntegerIsPrime(primeCandidate) ? TI_TRUE : TI_FALSE);
  longIntegerFree(primeCandidate);
  refreshRegisterLine(TRUE_FALSE_REGISTER_LINE);
}



void fnNextPrime(uint16_t unusedButMandatoryParameter) {
  longInteger_t currentNumber, nextPrime;

  longIntegerInit(currentNumber);
  longIntegerInit(nextPrime);

  if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
    convertShortIntegerRegisterToLongInteger(REGISTER_X, currentNumber);
  }

  else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_X, currentNumber);
  }

  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    convertReal34ToLongInteger(REGISTER_REAL34_DATA(REGISTER_X), currentNumber, DEC_ROUND_DOWN);
  }

  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if(EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "the input type %s is not allowed for PRIME?!", getDataTypeName(getRegisterDataType(REGISTER_X), false, false));
      showInfoDialog("In function fnIsPrime:", errorMessage, NULL, NULL);
    #endif
  }

  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  longIntegerSetPositiveSign(currentNumber);

  longIntegerNextPrime(currentNumber, nextPrime);

  if(getRegisterDataType(REGISTER_L) == dtShortInteger) {
    convertLongIntegerToShortIntegerRegister(nextPrime, getRegisterShortIntegerBase(REGISTER_L), REGISTER_X);
  }
  else {
    convertLongIntegerToLongIntegerRegister(nextPrime, REGISTER_X);
  }

  longIntegerFree(nextPrime);
  longIntegerFree(currentNumber);
  refreshStack();
}
