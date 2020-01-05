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
 * \file reToCx.c
 ***********************************************/

#include "wp43s.h"


/********************************************//**
 * \brief regX ==> regL and regY+i*regX ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnReToCx(uint16_t unusedParamButMandatory) {
  uint32_t dataTypeX = getRegisterDataType(REGISTER_X);
  uint32_t dataTypeY = getRegisterDataType(REGISTER_Y);
  bool_t xIsAReal;

  if(    (dataTypeX == dtReal34 || dataTypeX == dtLongInteger)
      && (dataTypeY == dtReal34 || dataTypeY == dtLongInteger)) {

    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    fnSetFlag(FLAG_CPXRES);

    fnRefreshRadioState(RB_BCR, true);                                    //dr

    xIsAReal = true;
    if(complexMode == CM_POLAR) {
      if(dataTypeX == dtReal34 && getRegisterAngularMode(REGISTER_X) != AM_NONE) {
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X), AM_RADIAN);
        setRegisterAngularMode(REGISTER_X, AM_NONE);
        dataTypeX = dtReal34;
        xIsAReal = false;
      }
    }

    if(dataTypeX == dtLongInteger) {
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      dataTypeX = dtReal34;
    }

    if(dataTypeY == dtLongInteger) {
      convertLongIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
      dataTypeY = dtReal34;
    }

    complex34_t temp;

    real34Copy(REGISTER_REAL34_DATA(REGISTER_Y), &temp);
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), VARIABLE_IMAG34_DATA(&temp));
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);

    if(complexMode == CM_POLAR) {
      if(real34CompareEqual(VARIABLE_REAL34_DATA(&temp), const34_0)) {
        real34Zero(VARIABLE_IMAG34_DATA(&temp));
      }
      else {
        real39_t magnitude, theta;

        real34ToReal(VARIABLE_REAL34_DATA(&temp), &magnitude);
        real34ToReal(VARIABLE_IMAG34_DATA(&temp), &theta);
        if(xIsAReal) {
          convertAngle39FromTo(&theta, currentAngularMode, AM_RADIAN);
        }
        if(realCompareLessThan(&magnitude, const_0)) {
          realSetPositiveSign(&magnitude);
          realAdd(&theta, const_pi, &theta, &ctxtReal39);
          WP34S_Mod(&theta, const1071_2pi, &theta);
        }
        real39PolarToRectangular(&magnitude, &theta, &magnitude, &theta); // theta in radian
        realToReal34(&magnitude, REGISTER_REAL34_DATA(REGISTER_X));
        realToReal34(&theta,     REGISTER_IMAG34_DATA(REGISTER_X));
      }
    }
    else {
      complex34Copy(&temp, REGISTER_COMPLEX34_DATA(REGISTER_X));
    }

    fnDropY(NOPARAM);
    STACK_LIFT_ENABLE;
    refreshStack();
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "You cannot use Re->Cx with %s in X and %s in Y!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false), getDataTypeName(getRegisterDataType(REGISTER_Y), true, false));
      showInfoDialog("In function fnReToCx:", errorMessage, NULL, NULL);
    #endif
  }
}
