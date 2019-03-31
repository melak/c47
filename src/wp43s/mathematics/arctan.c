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
 * \file arctan.c
 ***********************************************/

#include "wp43s.h"



void (* const arctan[12])(void) = {
// regX ==> 1            2            3            4             5             6             7             8               9            10             11           12
//          Long integer real16       complex16    angle         Time          Date          String        real16 mat      complex16 m  Short integer  real34       complex34
            arctanLonI,  arctanRe16,  arctanCo16,  arctanError,  arctanError,  arctanError,  arctanError,  arctanRm16,     arctanCm16,  arctanError,   arctanRe34,  arctanCo34
};



/********************************************//**
 * \brief Data type error in arctan
 *
 * \param void
 * \return void
 ***********************************************/
void arctanError(void) {
  displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate arctan for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function fnArctan:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and arctan(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnArctan(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  result = REGISTER_X;
  opX    = allocateTemporaryRegister();
  copySourceRegisterToDestRegister(REGISTER_X, opX);

  arctan[getRegisterDataType(REGISTER_X)]();

  adjustResult(result, false, true, opX, -1, -1);
}



void arctanLonI(void) {
  convertLongIntegerRegisterToReal34Register(opX, opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  WP34S_do_atan(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
  convertAngle34ToInternal(REGISTER_REAL34_DATA(result), AM_RADIAN);
  #if (ANGLE16 == 1)
    convertRegister34To16(result);
  #endif
  setRegisterDataType(result, dtAngle);
  setRegisterAngularMode(result, angularMode);
}



void arctanRe16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arctanRe16:", "cannot use NaN as an input of arctan", NULL, NULL);
    #endif
    return;
  }

  convertRegister16To34(opX);
  reallocateRegister(result, dtReal34, REAL34_SIZE, 0);
  if(real34IsInfinite(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_0_5, REGISTER_REAL34_DATA(result));
      if(real34IsNegative(REGISTER_REAL34_DATA(opX))) {
        real34SetNegativeSign(REGISTER_REAL34_DATA(result));
      }
      convertAngle34ToInternal(REGISTER_REAL34_DATA(result), AM_MULTPI);
      #if (ANGLE16 == 1)
        convertRegister34To16(result);
      #endif
      setRegisterDataType(result, dtAngle);
      setRegisterAngularMode(result, angularMode);
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnArctan:", "X = " STD_PLUS_MINUS STD_INFINITY, NULL, NULL);
      #endif
    }
  }
  else {
    WP34S_do_atan(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertAngle34ToInternal(REGISTER_REAL34_DATA(result), AM_RADIAN);
    #if (ANGLE16 == 1)
      convertRegister34To16(result);
    #endif
    setRegisterDataType(result, dtAngle);
    setRegisterAngularMode(result, angularMode);
  }
}



void arctanCo16(void) {
  if(real16IsNaN(REGISTER_REAL16_DATA(opX)) || real16IsNaN(REGISTER_IMAG16_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arctanCo16:", "cannot use NaN as an input of arctan", NULL, NULL);
    #endif
    return;
  }

  // arctan(z) = i/2 . ln((1 - iz) / (1 + iz))
  complex34_t one_iz;
  calcRegister_t opY;

  convertRegister16To34(opX);
  reallocateRegister(result, dtComplex34, COMPLEX34_SIZE, 0);

  // calculate iz
  opY = allocateTemporaryRegister();
  reallocateRegister(opY, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(REGISTER_COMPLEX34_DATA(opX), REGISTER_COMPLEX34_DATA(opY));
  real34Copy(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX));
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(opX));
  real34ChangeSign(REGISTER_REAL34_DATA(opX));

  // calculate 1 - iz
  real34Copy(const34_1, REGISTER_REAL34_DATA(opY));
  real34Zero(REGISTER_IMAG34_DATA(opY));
  subCo34Co34();
  complex34Copy(REGISTER_COMPLEX34_DATA(result), VARIABLE_COMPLEX34_DATA(&one_iz));

  // calculate 1 + iz
  addCo34Co34();

  // calculate (1 - iz) / (1 + iz)
  complex34Copy(VARIABLE_COMPLEX34_DATA(&one_iz), REGISTER_COMPLEX34_DATA(opY));
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  divCo34Co34();
  freeTemporaryRegister(opY);

  // calculate ln((1 - iz) / (1 + iz))
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  lnCo34();

  // arctan(z) = i/2 . ln((1 - iz) / (1 + iz))
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  real34Copy(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result));
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_REAL34_DATA(result));
  real34ChangeSign(REGISTER_REAL34_DATA(result));
  real34Multiply(REGISTER_REAL34_DATA(result), const34_0_5, REGISTER_REAL34_DATA(result));
  real34Multiply(REGISTER_IMAG34_DATA(result), const34_0_5, REGISTER_IMAG34_DATA(result));

  convertRegister34To16(result);
}



void arctanRm16(void) {
  fnToBeCoded();
}



void arctanCm16(void) {
  fnToBeCoded();
}



void arctanRe34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arctanRe34:", "cannot use NaN as an input of arctan", NULL, NULL);
    #endif
    return;
  }

  if(real34IsInfinite(REGISTER_REAL34_DATA(opX))) {
    if(getFlag(FLAG_DANGER)) {
      real34Copy(const34_0_5, REGISTER_REAL34_DATA(result));
      if(real34IsNegative(REGISTER_REAL34_DATA(opX))) {
        real34SetNegativeSign(REGISTER_REAL34_DATA(result));
      }
      convertAngle34ToInternal(REGISTER_REAL34_DATA(result), AM_MULTPI);
      #if (ANGLE16 == 1)
        convertRegister34To16(result);
      #endif
      setRegisterDataType(result, dtAngle);
      setRegisterAngularMode(result, angularMode);
    }
    else {
      displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function fnArctan:", "X = " STD_PLUS_MINUS STD_INFINITY " and Danger flag not set!", NULL, NULL);
      #endif
    }
  }
  else {
    WP34S_do_atan(REGISTER_REAL34_DATA(opX), REGISTER_REAL34_DATA(result));
    convertAngle34ToInternal(REGISTER_REAL34_DATA(result), AM_RADIAN);
    #if (ANGLE16 == 1)
      convertRegister34To16(result);
    #endif
    setRegisterDataType(result, dtAngle);
    setRegisterAngularMode(result, angularMode);
  }
}



void arctanCo34(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(opX)) || real34IsNaN(REGISTER_IMAG34_DATA(opX))) {
    displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function arctanCo34:", "cannot use NaN as an input of arctan", NULL, NULL);
    #endif
    return;
  }

  // arctan(z) = i/2 . ln((1 - iz) / (1 + iz))
  complex34_t one_iz;
  calcRegister_t opY;

  // calculate iz
  opY = allocateTemporaryRegister();
  reallocateRegister(opY, dtComplex34, COMPLEX34_SIZE, 0);
  complex34Copy(REGISTER_COMPLEX34_DATA(opX), REGISTER_COMPLEX34_DATA(opY));
  real34Copy(REGISTER_REAL34_DATA(opY), REGISTER_IMAG34_DATA(opX));
  real34Copy(REGISTER_IMAG34_DATA(opY), REGISTER_REAL34_DATA(opX));
  real34ChangeSign(REGISTER_REAL34_DATA(opX));

  // calculate 1 - iz
  real34Copy(const34_1, REGISTER_REAL34_DATA(opY));
  real34Zero(REGISTER_IMAG34_DATA(opY));
  subCo34Co34();
  complex34Copy(REGISTER_COMPLEX34_DATA(result), VARIABLE_COMPLEX34_DATA(&one_iz));

  // calculate 1 + iz
  addCo34Co34();

  // calculate (1 - iz) / (1 + iz)
  complex34Copy(VARIABLE_COMPLEX34_DATA(&one_iz), REGISTER_COMPLEX34_DATA(opY));
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  divCo34Co34();
  freeTemporaryRegister(opY);

  // calculate ln((1 - iz) / (1 + iz))
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  lnCo34();

  // arctan(z) = i/2 . ln((1 - iz) / (1 + iz))
  complex34Copy(REGISTER_COMPLEX34_DATA(result), REGISTER_COMPLEX34_DATA(opX));
  real34Copy(REGISTER_REAL34_DATA(opX), REGISTER_IMAG34_DATA(result));
  real34Copy(REGISTER_IMAG34_DATA(opX), REGISTER_REAL34_DATA(result));
  real34ChangeSign(REGISTER_REAL34_DATA(result));
  real34Multiply(REGISTER_REAL34_DATA(result), const34_0_5, REGISTER_REAL34_DATA(result));
  real34Multiply(REGISTER_IMAG34_DATA(result), const34_0_5, REGISTER_IMAG34_DATA(result));
}
