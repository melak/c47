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
 * \file cube.c
 ***********************************************/

#include "wp43s.h"



void (* const cube[9])(void) = {
// regX ==> 1            2         3         4          5          6          7          8           9
//          Long integer Real34    complex34 Time       Date       String     Real34 mat Complex34 m Short integer
            cubeLonI,    cubeReal, cubeCplx, cubeError, cubeError, cubeError, cubeRema,  cubeCxma,   cubeShoI
};



/********************************************//**
 * \brief Data type error in cubing
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void cubeError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot cube %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnCube:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and regX × regX × regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory
 * \return void
 ***********************************************/
void fnCube(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  cube[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
}



void cubeLonI(void) {
  longInteger_t x, cube;

  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);
  longIntegerInit(cube);
  longIntegerMultiply(x, x, cube);
  longIntegerMultiply(cube, x, cube);
  convertLongIntegerToLongIntegerRegister(cube, REGISTER_X);
  longIntegerFree(x);
  longIntegerFree(cube);
}



void cubeRema(void) {
  fnToBeCoded();
}



void cubeCxma(void) {
  fnToBeCoded();
}



void cubeShoI(void) {
  uint64_t square;
  square = WP34S_intMultiply(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)), *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intMultiply(square, *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



void cubeReal(void) {
  if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X)) && !getSystemFlag(FLAG_SPCRES)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function cubeReal:", "cannot use " STD_PLUS_MINUS STD_INFINITY " as X input of ^3 when flag D is not set", NULL, NULL);
    #endif
    return;
  }

  real_t x, xSquared;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

  realMultiply(&x, &x, &xSquared, &ctxtReal39);
  realMultiply(&xSquared, &x, &x, &ctxtReal39);

  realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  setRegisterAngularMode(REGISTER_X, AM_NONE);
}



void cubeCplx(void) {
  real_t a, b, realSquare, imagSquare;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &a);
  real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &b);

  mulComplexComplex(&a, &b, &a, &b, &realSquare, &imagSquare, &ctxtReal39);
  mulComplexComplex(&realSquare, &imagSquare, &a, &b, &a, &b, &ctxtReal39);

  realToReal34(&a, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}
