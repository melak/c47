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
 * \file cxToRe.c
 ***********************************************/

#include "wp43s.h"


/********************************************//**
 * \brief regX ==> regL and re(regX) ==> regY, im(regX) ==> regX or magnitude(regX) ==> regY, angle(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnCxToRe(uint16_t unusedParamButMandatory) {
  uint32_t dataTypeX = getRegisterDataType(REGISTER_X);
  uint32_t dataTypeY = getRegisterDataType(REGISTER_Y);
  bool_t xIsAReal;

  if(dataTypeX == dtComplex16) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, AM_NONE);

    STACK_LIFT_ENABLE;
    if(complexMode == CM_RECTANGULAR) {
      real16Copy(REGISTER_REAL16_DATA(REGISTER_L), REGISTER_REAL16_DATA(REGISTER_X));
      liftStack();
      real16Copy(REGISTER_IMAG16_DATA(REGISTER_L), REGISTER_REAL16_DATA(REGISTER_X));
      temporaryInformation = TI_RE_IM;
    }
    else { // CM_POLAR mode
      liftStack();
      real16RectangularToPolar(REGISTER_REAL16_DATA(REGISTER_L), REGISTER_IMAG16_DATA(REGISTER_L), REGISTER_REAL16_DATA(REGISTER_Y), REGISTER_REAL16_DATA(REGISTER_X)); // X in radians
      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_RADIAN, currentAngularMode);
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
      temporaryInformation = TI_RADIUS_THETA;
    }

    refreshStack();
  }

  else if(dataTypeX == dtComplex34) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

    STACK_LIFT_ENABLE;
    if(complexMode == CM_RECTANGULAR) {
      real34Copy(REGISTER_REAL34_DATA(REGISTER_L), REGISTER_REAL34_DATA(REGISTER_X));
      liftStack();
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      real34Copy(REGISTER_IMAG34_DATA(REGISTER_L), REGISTER_REAL34_DATA(REGISTER_X));
      temporaryInformation = TI_RE_IM;
    }
    else { // CM_POLAR mode
      liftStack();
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      real34RectangularToPolar(REGISTER_REAL34_DATA(REGISTER_L), REGISTER_IMAG34_DATA(REGISTER_L), REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X)); // X in radians
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_RADIAN, currentAngularMode);
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
      temporaryInformation = TI_RADIUS_THETA;
    }

    refreshStack();
  }

  else {
    displayBugScreen("fnCxToRe was called with unexpected data type in regX; correct error handling is not implemented yet.");
  }

}
