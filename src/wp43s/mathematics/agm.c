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
 * \file agm.c
 ***********************************************/

#include "wp43s.h"


void fnAgm(uint16_t unusedButMandatoryParameter) {
  bool_t realInput=true;
  real_t aReal, bReal, cReal;
  real_t aImag, bImag, cImag;

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_X, &aReal, &ctxtReal39);
                        realZero(&aImag);
                        break;

    case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &aReal);
                        realZero(&aImag);
                        break;

    case dtComplex34:   real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &aReal);
                        real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &aImag);
                        realInput = false;
                        break;

    default:            displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
                        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                          sprintf(errorMessage, "cannot calculate AGM with %s in X", getRegisterDataTypeName(REGISTER_X, true, false));
                          moreInfoOnError("In function fnAgm:", errorMessage, NULL, NULL);
                        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
                        return;
  }

  switch(getRegisterDataType(REGISTER_Y)) {
    case dtLongInteger: convertLongIntegerRegisterToReal(REGISTER_Y, &bReal, &ctxtReal39);
                        realZero(&bImag);
                        break;

    case dtReal34:      real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &bReal);
                        realZero(&bImag);
                        break;

    case dtComplex34:   real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &bReal);
                        real34ToReal(REGISTER_IMAG34_DATA(REGISTER_Y), &bImag);
                        realInput = false;
                        break;

    default:            displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_Y);
                        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                          sprintf(errorMessage, "cannot calculate AGM with %s in Y", getRegisterDataTypeName(REGISTER_Y, true, false));
                          moreInfoOnError("In function fnAgm:", errorMessage, NULL, NULL);
                        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
                        return;
  }

  if(realInput && (realIsNegative(&aReal) || realIsNegative(&bReal))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function fnAgm:", "cannot use negative X and Y as input of AGM", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(realInput) {
    while(realIdenticalDigits(&aReal, &bReal) <= 34) {
      realAdd(&aReal, &bReal, &cReal, &ctxtReal39);          // c = a + b
      realMultiply(&aReal, &bReal, &bReal, &ctxtReal39);     // b = a * b
      realSquareRoot(&bReal, &bReal, &ctxtReal39);           // b = sqrt(a * b)
      realMultiply(&cReal, const_1on2, &aReal, &ctxtReal39); // a = (a + b) / 2
    }

    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
    realToReal34(&aReal, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else { // Complex input
    while(realIdenticalDigits(&aReal, &bReal) <= 34 || realIdenticalDigits(&aImag, &bImag) <= 34) {
      realAdd(&aReal, &bReal, &cReal, &ctxtReal39);                                   // c = a + b real part
      realAdd(&aImag, &bImag, &cImag, &ctxtReal39);                                   // c = a + b imag part

      mulComplexComplex(&aReal, &aImag, &bReal, &bImag, &bReal, &bImag, &ctxtReal39); // b = a * b

      // b = sqrt(a * b)
      realRectangularToPolar(&bReal, &bImag, &bReal, &bImag, &ctxtReal39);
      realSquareRoot(&bReal, &bReal, &ctxtReal39);
      realMultiply(&bImag, const_1on2, &bImag, &ctxtReal39);
      realPolarToRectangular(&bReal, &bImag, &bReal, &bImag, &ctxtReal39);

      realMultiply(&cReal, const_1on2, &aReal, &ctxtReal39); // a = (a + b) / 2 real part
      realMultiply(&cImag, const_1on2, &aImag, &ctxtReal39); // a = (a + b) / 2 imag part
    }

    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
    realToReal34(&aReal, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&aImag, REGISTER_IMAG34_DATA(REGISTER_X));
  }

  adjustResult(REGISTER_X, true, true, REGISTER_X, -1, -1);
}
