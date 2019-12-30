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
 * \file changeSign.c
 ***********************************************/

#include "wp43s.h"



void (* const chs[9])(void) = {
// regX ==> 1            2        3         4         5         6         7          8           9
//          Long integer Real34   Complex34 Time      Date      String    Real34 mat Complex34 m Short integer
            chsLonI,     chsReal, chsCplx,  chsError, chsError, chsError, chsRema,   chsCxma,    chsShoI
};



/********************************************//**
 * \brief Data type error in +/-
 *
 * \param void
 * \return void
 ***********************************************/
void chsError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot change the sign of %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnChangeSign:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief rexX ==> regL and -regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnChangeSign(uint16_t unusedParamButMandatory) {
  saveStack();
  //copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  chs[getRegisterDataType(REGISTER_X)]();

  if(lastErrorCode == 0) {
    refreshRegisterLine(REGISTER_X);
  }
  else {
    restoreStack();
    refreshStack();
  }
}



void chsLonI(void) {
  switch(getRegisterLongIntegerSign(REGISTER_X)) {
    case LONG_INTEGER_POSITIVE : setRegisterLongIntegerSign(REGISTER_X, LONG_INTEGER_NEGATIVE); break;
    case LONG_INTEGER_NEGATIVE : setRegisterLongIntegerSign(REGISTER_X, LONG_INTEGER_POSITIVE); break;
    default : {}
  }
}



void chsRema(void) {
  fnToBeCoded();
}



void chsCxma(void) {
  fnToBeCoded();
}



void chsShoI(void) {
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intChs(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



void chsReal(void) {
  if(!getFlag(FLAG_DANGER) && real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X)) ? ERROR_OVERFLOW_MINUS_INF : ERROR_OVERFLOW_PLUS_INF , ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function chsReal:", "cannot change infinity sign while D flag is clear", NULL, NULL);
    #endif
    return;
  }

  real34ChangeSign(REGISTER_REAL34_DATA(REGISTER_X));

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    real34SetPositiveSign(REGISTER_REAL34_DATA(REGISTER_X));
  }
}



void chsCplx(void) {
  if(!getFlag(FLAG_DANGER)) {
    if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
      displayCalcErrorMessage(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X)) ? ERROR_OVERFLOW_MINUS_INF : ERROR_OVERFLOW_PLUS_INF , ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function chsCplx:", "cannot change infinity sign of real part while D flag is clear", NULL, NULL);
      #endif
      return;
    }

    if(real34IsInfinite(REGISTER_IMAG34_DATA(REGISTER_X))) {
      displayCalcErrorMessage(real34IsPositive(REGISTER_IMAG34_DATA(REGISTER_X)) ? ERROR_OVERFLOW_MINUS_INF : ERROR_OVERFLOW_PLUS_INF , ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function chsCplx:", "cannot change infinity sign of imaginary part while D flag is clear", NULL, NULL);
      #endif
      return;
    }
  }

  complex34ChangeSign(REGISTER_COMPLEX34_DATA(REGISTER_X));

  if(real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    real34SetPositiveSign(REGISTER_REAL34_DATA(REGISTER_X));
  }

  if(real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
    real34SetPositiveSign(REGISTER_IMAG34_DATA(REGISTER_X));
  }
}
