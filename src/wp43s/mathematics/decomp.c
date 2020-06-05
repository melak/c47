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
 * \file decomp.c
 ***********************************************/

#include "wp43s.h"

static void (*const Decomp[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX ==> 1            2           3            4            5            6            7            8            9             10
//          Long integer Real34      Complex34    Time         Date         String       Real34 mat   Complex34 m  Short integer Config data
            decompError, decompReal, decompError, decompError, decompError, decompError, decompError, decompError, decompError,  decompError
};



/********************************************//**
 * \brief regX ==> regL and DECOMP(regX) ==> regX, regY
 * enables stack lift and refreshes the stack.
 * Decomposes x (after converting it to an improper fraction, if applicable), returning a stack with
 * [denominator(x), numerator(x)]
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnDecomp(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Decomp[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  adjustResult(REGISTER_Y, false, false, REGISTER_Y, -1, -1);
}



void decompError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);

  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
  sprintf(errorMessage, "cannot calculate Decomp for %s", getRegisterDataTypeName(REGISTER_X, true, false));
  showInfoDialog("In function fnDecomp:", errorMessage, NULL, NULL);
  #endif
}



void decompReal(void) {
  STACK_LIFT_ENABLE;
  liftStack();

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_X)); // Denominator = NaN
    realToReal34(const_NaN, REGISTER_REAL34_DATA(REGISTER_Y)); // Numerator = NaN
  }
  else if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X)); // Denominator = 0
    realToReal34(real34IsNegative(REGISTER_REAL34_DATA(REGISTER_X)) ? const__1 : const_1, REGISTER_REAL34_DATA(REGISTER_Y)); // Numerator = +/- 1
  }
  else {
    uint32_t savedDenMax = denMax;
    uint64_t savedSystemFlags = systemFlags;
    int16_t sign, lessEqualGreater;
    uint64_t intPart, numer, denom;
    longInteger_t lgInt;

    denMax = MAX_DENMAX;
    clearSystemFlag(FLAG_PROPFR); // set improper fraction mode

    fraction(REGISTER_Y, &sign, &intPart, &numer, &denom, &lessEqualGreater);

    denMax = savedDenMax;
    systemFlags = savedSystemFlags;

    longIntegerInit(lgInt);

    uIntToLongInteger(numer, lgInt);
    if(sign == -1) {
      longIntegerSetNegativeSign(lgInt);
    }
    convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_Y);

    uIntToLongInteger(denom, lgInt);
    convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);

    longIntegerFree(lgInt);
  }

  refreshStack();
}
