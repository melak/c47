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
 * \file square.c
 ***********************************************/

#include "wp43s.h"



void (* const square[12])(void) = {
// regX ==> 1              2           3           4            5            6            7            8           9           10            11          12
//          Long integer   Real16      Complex16   Angle16      Time         Date         String       Real16 mat  Complex16 m Short integer Real34      Complex34
            squareLonI,    squareRe16, squareCo16, squareError, squareError, squareError, squareError, squareRm16, squareCm16, squareShoI,   squareRe34, squareCo34
};



/********************************************//**
 * \brief Data type error in squaring
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void squareError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot square %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnSquare:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and regX × regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnSquare(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  square[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void squareLonI(void) {
  longInteger_t lgInt;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);
  longIntegerMultiply(lgInt, lgInt, lgInt);
  convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
  longIntegerFree(lgInt);
}



void squareRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function squareRe16:", "cannot use NaN as X input of ^2", NULL, NULL);
    #endif
    return;
  }

  if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function squareRe16:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as ^2 input of exp when flag D is not set", NULL, NULL);
    #endif
    return;
  }

  real16Multiply(REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X), REGISTER_REAL16_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void squareCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(REGISTER_X)) || real16IsNaN(REGISTER_IMAG16_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function squareCo16:", "cannot use NaN as X input of ^2", NULL, NULL);
    #endif
    return;
  }

  real39_t a, b, imagPart;

  real16ToReal(REGISTER_REAL16_DATA(REGISTER_X), &a);
  real16ToReal(REGISTER_IMAG16_DATA(REGISTER_X), &b);

  // (a + bi)² = a² - b² + 2abi
  // Imaginary part
  realMultiply(&a, const_2, &imagPart, &ctxtReal39);
  realMultiply(&b, &imagPart, &imagPart, &ctxtReal39);

  // real part
  realMultiply(&a, &a, &a, &ctxtReal39);
  realMultiply(&b, &b, &b, &ctxtReal39);
  realSubtract(&a, &b, &a, &ctxtReal39);

  realToReal16(&a,        REGISTER_REAL16_DATA(REGISTER_X));
  realToReal16(&imagPart, REGISTER_IMAG16_DATA(REGISTER_X));
}



void squareRm16(void) {
  fnToBeCoded();
}



void squareCm16(void) {
  fnToBeCoded();
}



void squareShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intMultiply(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)), *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



void squareRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function squareRe34:", "cannot use NaN as X input of ^2", NULL, NULL);
    #endif
    return;
  }

  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function squareRe34:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of ^2 when flag D is not set", NULL, NULL);
    #endif
    return;
  }

  real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void squareCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function squareCo34:", "cannot use NaN as X input of ^2", NULL, NULL);
    #endif
    return;
  }

  real39_t a, b, imagPart;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  // (a + bi)² = a² - b² + 2abi
  // Imaginary part
  realMultiply(&a, const_2, &imagPart, &ctxtReal39);
  realMultiply(&b, &imagPart, &imagPart, &ctxtReal39);

  // real part
  realMultiply(&a, &a, &a, &ctxtReal39);
  realMultiply(&b, &b, &b, &ctxtReal39);
  realSubtract(&a, &b, &a, &ctxtReal39);

  realToReal34(&a,        REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&imagPart, REGISTER_IMAG34_DATA(REGISTER_X));
}
