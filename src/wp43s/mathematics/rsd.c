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
 * \file rsd.c
 ***********************************************/

#include "wp43s.h"



void (* const Rsd[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(uint16_t) = {
// regX ==> 1            2        3         4         5         6         7          8           9             10
//          Long integer Real34   Complex34 Time      Date      String    Real34 mat Complex34 m Short integer Config data
            rsdError,    rsdReal, rsdCplx,  rsdError, rsdError, rsdError, rsdRema,   rsdCxma,    rsdError,     rsdError
};



/********************************************//**
 * \brief calculate RSD
 *
 * \param[in] source real_t*
 * \param[out] destination real_t*
 * \param[in] realContext realContext_t*
 * \param[in] digits uint16_t
 * \return void
 ***********************************************/
void roundToSignificantDigits(real_t *source, real_t *destination, uint16_t digits, realContext_t *realContext) {
  real_t val, tmp;
  int16_t exponent;

  if(realIsZero(source) || realIsSpecial(source)) {
    realCopy(source, destination);
    return;
  }

  realCopy(source, &val);
  exponent = val.digits + val.exponent - 1;
  val.exponent -= exponent + 1;
  while(1) { // in case of subnormal
    realCopy(&val, &tmp);
    if(realCompareAbsLessThan(&tmp, const_1on10)) {
      ++val.exponent;
      --exponent;
    }
    else {
      break;
    }
  }
  val.exponent += digits;
  realToIntegralValue(&val, &val, roundingModeTable[roundingMode], realContext);
  val.exponent -= digits;
  val.exponent += exponent + 1;

  realCopy(&val, destination);
  return;
}



/********************************************//**
 * \brief Data type error in rsd
 *
 * \param unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void rsdError(uint16_t unusedButMandatoryParameter) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate RSD for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    moreInfoOnError("In function rsdError:", errorMessage, NULL, NULL);
  #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
}



/********************************************//**
 * \brief round to given significant digits
 *
 * \param[in] digits uint16_t
 * \return void
 ***********************************************/
void fnRsd(uint16_t digits) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Rsd[getRegisterDataType(REGISTER_X)](digits);

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}



void rsdRema(uint16_t digits) {
  fnToBeCoded();
}



void rsdCxma(uint16_t digits) {
  fnToBeCoded();
}



void rsdReal(uint16_t digits) {
  real_t val;

  updateDisplayValueX = true;
  displayValueX[0] = 0;
  refreshRegisterLine(REGISTER_X);
  updateDisplayValueX = false;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &val);
  roundToSignificantDigits(&val, &val, digits, &ctxtReal39);
  realToReal34(&val, REGISTER_REAL34_DATA(REGISTER_X));
}



void rsdCplx(uint16_t digits) {
  int32_t pos, posI;
  bool_t polar = false;

  updateDisplayValueX = true;
  displayValueX[0] = 0;
  refreshRegisterLine(REGISTER_X);
  updateDisplayValueX = false;

  posI = DISPLAY_VALUE_LEN - 1;
  pos = 0;
  while(displayValueX[pos] != 0) {
    if(displayValueX[pos] == 'i') {
      posI = pos;
      break;
    }
    pos++;
  }

  if(posI == DISPLAY_VALUE_LEN - 1) {
    pos = 0;
    while(displayValueX[pos] != 0) {
      if(displayValueX[pos] == 'j') {
        posI = pos;
        polar = true;
        break;
      }
      pos++;
    }
  }

  displayValueX[posI++] = 0;
  if(polar) {
    real_t magnitude, theta;
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &magnitude);
    real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &theta);
    realRectangularToPolar(&magnitude, &theta, &magnitude, &theta, &ctxtReal39);
    roundToSignificantDigits(&magnitude, &magnitude, digits, &ctxtReal39);
    roundToSignificantDigits(&theta,     &theta,     digits, &ctxtReal39);
    realPolarToRectangular(&magnitude, &theta, &magnitude, &theta, &ctxtReal39);
    realToReal34(&magnitude, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&theta,     REGISTER_IMAG34_DATA(REGISTER_X));
  }
  else {
    real_t real, imaginary;
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &real);
    real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &imaginary);
    roundToSignificantDigits(&real,      &real,      digits, &ctxtReal39);
    roundToSignificantDigits(&imaginary, &imaginary, digits, &ctxtReal39);
    realToReal34(&real,      REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&imaginary, REGISTER_IMAG34_DATA(REGISTER_X));
  }
}