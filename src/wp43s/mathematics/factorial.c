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



void (* const fact[9])(void) = {
// regX ==> 1            2         3         4          5          6          7          8           9
//          Long integer Real34    Complex34 Time       Date       String     Real34 mat Complex34 m Short integer
            factLonI,    factReal, factCplx, factError, factError, factError, factError, factError,  factShoI
};



/********************************************//**
 * \brief Data type error in fact
 *
 * \param void
 * \return void
 ***********************************************/
void factError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate x! for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnFactorial:", errorMessage, NULL, NULL);
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
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  fact[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void factLonI(void) {
  longInteger_t x, fact;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  if(longIntegerIsNegative(x)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      longIntegerRegisterToDisplayString(REGISTER_X, errorMessage, sizeof(errorMessage), SCREEN_WIDTH, 50, STD_SPACE_PUNCTUATION);
      sprintf(tmpStr3000, "cannot calculate factorial(%s)", errorMessage);
      showInfoDialog("In function factLonI:", tmpStr3000, NULL, NULL);
    #endif
    longIntegerFree(x);
    return;
  }

  if(longIntegerCompareUInt(x, MAX_FACTORIAL) > 0) {                            //JM
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      longIntegerRegisterToDisplayString(REGISTER_X, errorMessage, sizeof(errorMessage), SCREEN_WIDTH, 50, STD_SPACE_PUNCTUATION);
      sprintf(tmpStr3000, "cannot calculate factorial(%s), the limit is 450, it's to ensure that the 3328 bits limit is not exceeded", errorMessage);
      showInfoDialog("In function factLonI:", tmpStr3000, NULL, NULL);
    #endif
    longIntegerFree(x);
    return;
  }


  uint32_t n;
  longIntegerToUInt(x, n);
  #if (__linux__ == 1)
    //The more precise formula below is: (n*ln(n) - n + (ln(8n³ + 4n² + n + 1/30))/6 + ln(pi)/2) / ln(2)
    longIntegerInitSizeInBits(fact, 1 + (uint32_t)((n * log(n) - n) / log(2)));
    uIntToLongInteger(1, fact);
    for(uint32_t i=2; i<=n; i++) {
      longIntegerMultiplyUInt(fact, i, fact);
    }
  #else
    longIntegerInit(fact);
    longIntegerFactorial(n, fact); //TODO why this line fails?
  #endif


  convertLongIntegerToLongIntegerRegister(fact, REGISTER_X);

  longIntegerFree(fact);
  longIntegerFree(x);
}

uint64_t fact_uint64(uint64_t value)
{
  uint64_t result = value;

  if(value <= 1)
    result = 1;
  else
    for(uint64_t i=value-1 ; i > 1 ; result *= i--)
      ;

  return result;
}


void factShoI(void) {
  int16_t sign;
  uint64_t value;

  convertShortIntegerRegisterToUInt64(REGISTER_X, &sign, &value);

  if(sign == 1) { // Negative value
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      longIntegerRegisterToDisplayString(REGISTER_X, errorMessage, sizeof(errorMessage), SCREEN_WIDTH, 50, STD_SPACE_PUNCTUATION);
      sprintf(tmpStr3000, "cannot calculate factorial(%s)", errorMessage);
      showInfoDialog("In function factShoI:", tmpStr3000, NULL, NULL);
    #endif
    return;
  }

  if(value > 20) {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      longIntegerRegisterToDisplayString(REGISTER_X, errorMessage, sizeof(errorMessage), SCREEN_WIDTH, 50, STD_SPACE_PUNCTUATION);
      sprintf(tmpStr3000, "cannot calculate factorial(%s)", errorMessage);
      showInfoDialog("In function factShoI:", tmpStr3000, NULL, NULL);
    #endif
    return;
  }

  uint64_t fact = fact_uint64(value);

  if(fact > shortIntegerMask) {
    setSystemFlag(FLAG_OVERFLOW);
  }

  convertUInt64ToShortIntegerRegister(0, fact, getRegisterTag(REGISTER_X), REGISTER_X);
}



void factReal(void) {
  real_t x;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  WP34S_Factorial(&x, &x, &ctxtReal39);
  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void factCplx(void) {
  real_t zReal, zImag;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &zReal);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &zImag);

  realAdd(&zReal, const_1, &zReal, &ctxtReal39);
  WP34S_ComplexGamma(&zReal, &zImag, &zReal, &zImag, &ctxtReal39);

  realToReal34(&zReal, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&zImag, REGISTER_IMAG34_DATA(REGISTER_X));
}
