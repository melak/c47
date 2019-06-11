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
 * \file conversionAngles.c
 ***********************************************/

#include "wp43s.h"



void fnCvtToCurrentAngularMode(uint16_t fromAngularMode) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), fromAngularMode, currentAngularMode);
      setRegisterDataType(REGISTER_X, dtAngle16, currentAngularMode);
      break;

    case dtReal16:
      if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtToCurrentAngularMode:", "cannot use NaN as an input of fnCvtToCurrentAngularMode", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), fromAngularMode, currentAngularMode);
      setRegisterDataType(REGISTER_X, dtAngle16, currentAngularMode);
      break;

    case dtReal34:
      if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtToCurrentAngularMode:", "cannot use NaN as an input of fnCvtToCurrentAngularMode", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), fromAngularMode, currentAngularMode);
      setRegisterDataType(REGISTER_X, dtAngle34, currentAngularMode);
      break;

    case dtAngle16:
      if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtToCurrentAngularMode:", "cannot use NaN as an input of fnCvtToCurrentAngularMode", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      if(getRegisterAngularMode(REGISTER_X) == fromAngularMode) {
        convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), fromAngularMode, currentAngularMode);
        setRegisterDataType(REGISTER_X, dtAngle16, currentAngularMode);
      }
      else {
        displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "input angle16 must be tagged %s!", getAngularModeName(fromAngularMode));
          showInfoDialog("In function fnCvtToCurrentAngularMode:", errorMessage, NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    case dtAngle34:
      if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtToCurrentAngularMode:", "cannot use NaN as an input of fnCvtToCurrentAngularMode", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      if(getRegisterAngularMode(REGISTER_X) == fromAngularMode) {
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), fromAngularMode, currentAngularMode);
        setRegisterDataType(REGISTER_X, dtAngle34, currentAngularMode);
      }
      else {
        displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "input angle34 must be tagged %s!", getAngularModeName(fromAngularMode));
          showInfoDialog("In function fnCvtToCurrentAngularMode:", errorMessage, NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    default:
      displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "%s cannot be converted to an angle!", getRegisterDataTypeName(REGISTER_X, true, false));
        showInfoDialog("In function fnCvtToCurrentAngularMode:", "the input value must be a long integer, a real16, a real34, an angle16 or an angle34", errorMessage, NULL);
      #endif
      restoreStack();
      refreshStack();
      return;
  }

  refreshRegisterLine(REGISTER_X);
}



void fnCvtFromCurrentAngularMode(uint16_t toAngularMode) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), currentAngularMode, toAngularMode);
      setRegisterDataType(REGISTER_X, dtAngle16, toAngularMode);
      break;

    case dtReal16:
      if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtFromCurrentAngularMode:", "cannot use NaN as an input of fnCvtFromCurrentAngularMode", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), currentAngularMode, toAngularMode);
      setRegisterDataType(REGISTER_X, dtAngle16, toAngularMode);
      break;

    case dtReal34:
      if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtFromCurrentAngularMode:", "cannot use NaN as an input of fnCvtFromCurrentAngularMode", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), currentAngularMode, toAngularMode);
      setRegisterDataType(REGISTER_X, dtAngle34, toAngularMode);
      break;

    case dtAngle16:
      if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtFromCurrentAngularMode:", "cannot use NaN as an input of fnCvtFromCurrentAngularMode", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X), toAngularMode);
      setRegisterDataType(REGISTER_X, dtAngle16, toAngularMode);
      break;

    case dtAngle34:
      if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtFromCurrentAngularMode:", "cannot use NaN as an input of fnCvtFromCurrentAngularMode", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X), toAngularMode);
      setRegisterDataType(REGISTER_X, dtAngle34, toAngularMode);
      break;

    default:
      displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "%s cannot be converted to an angle!", getRegisterDataTypeName(REGISTER_X, true, false));
        showInfoDialog("In function fnCvtFromCurrentAngularMode:", "the input value must be a long integer, a real16, a real34, an angle16 or an angle34", errorMessage, NULL);
      #endif
      restoreStack();
      refreshStack();
      return;
  }

  refreshRegisterLine(REGISTER_X);
}



void fnCvtDegToRad(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_DEGREE, AM_RADIAN);
      setRegisterDataType(REGISTER_X, dtAngle16, AM_RADIAN);
      break;

    case dtReal16:
      if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToRad:", "cannot use NaN as an input of fnCvtDegToRad", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_DEGREE, AM_RADIAN);
      setRegisterDataType(REGISTER_X, dtAngle16, AM_RADIAN);
      break;

    case dtReal34:
      if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToRad:", "cannot use NaN as an input of fnCvtDegToRad", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE, AM_RADIAN);
      setRegisterDataType(REGISTER_X, dtAngle34, AM_RADIAN);
      break;

    case dtAngle16:
      if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToRad:", "cannot use NaN as an input of fnCvtDegToRad", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      if(getRegisterAngularMode(REGISTER_X) == AM_DEGREE) {
        convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_DEGREE, AM_RADIAN);
        setRegisterDataType(REGISTER_X, dtAngle16, AM_RADIAN);
      }
      else {
        displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToRad:", "cannot use an angle16 not tagged degree as an input of fnCvtDegToRad", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    case dtAngle34:
      if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToRad:", "cannot use NaN as an input of fnCvtDegToRad", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      if(getRegisterAngularMode(REGISTER_X) == AM_DEGREE) {
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE, AM_RADIAN);
        setRegisterDataType(REGISTER_X, dtAngle34, AM_RADIAN);
      }
      else {
        displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToRad:", "cannot use an angle34 not tagged degree as an input of fnCvtDegToRad", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    default:
      displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "%s cannot be converted to an angle!", getRegisterDataTypeName(REGISTER_X, true, false));
        showInfoDialog("In function fnCvtDegToRad:", "the input value must be a real16, a real34, a long integer", errorMessage, NULL);
      #endif
      restoreStack();
      refreshStack();
      return;
  }

  refreshRegisterLine(REGISTER_X);
}



void fnCvtRadToDeg(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_RADIAN, AM_DEGREE);
      setRegisterDataType(REGISTER_X, dtAngle16, AM_DEGREE);
      break;

    case dtReal16:
      if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use NaN as an input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_RADIAN, AM_DEGREE);
      setRegisterDataType(REGISTER_X, dtAngle16, AM_DEGREE);
      break;

    case dtReal34:
      if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use NaN as an input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_RADIAN, AM_DEGREE);
      setRegisterDataType(REGISTER_X, dtAngle34, AM_DEGREE);
      break;

    case dtAngle16:
      if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use NaN as an input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      if(getRegisterAngularMode(REGISTER_X) == AM_RADIAN) {
        convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_RADIAN, AM_DEGREE);
        setRegisterDataType(REGISTER_X, dtAngle16, AM_DEGREE);
      }
      else {
        displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use an angle16 not tagged degree as an input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    case dtAngle34:
      if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use NaN as an input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      if(getRegisterAngularMode(REGISTER_X) == AM_RADIAN) {
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_RADIAN, AM_DEGREE);
        setRegisterDataType(REGISTER_X, dtAngle34, AM_DEGREE);
      }
      else {
        displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use an angle34 not tagged degree as an input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    default:
      displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "%s cannot be converted to an angle!", getRegisterDataTypeName(REGISTER_X, true, false));
        showInfoDialog("In function fnCvtRadToDeg:", "the input value must be a real16, a real34, a long integer", errorMessage, NULL);
      #endif
      restoreStack();
      refreshStack();
      return;
  }

  refreshRegisterLine(REGISTER_X);
}



void fnCvtDegToDms(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_DEGREE, AM_DMS);
      setRegisterDataType(REGISTER_X, dtAngle16, AM_DMS);
      break;

    case dtReal16:
      if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToDms:", "cannot use NaN as an input of fnCvtDegToDms", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_DEGREE, AM_DMS);
      setRegisterDataType(REGISTER_X, dtAngle16, AM_DMS);
      break;

    case dtReal34:
      if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToDms:", "cannot use NaN as an input of fnCvtDegToDms", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE, AM_DMS);
      setRegisterDataType(REGISTER_X, dtAngle34, AM_DMS);
      break;

    case dtAngle16:
      if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToDms:", "cannot use NaN as an input of fnCvtDegToDms", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      if(getRegisterAngularMode(REGISTER_X) == AM_DEGREE) {
        convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_DEGREE, AM_DMS);
        setRegisterDataType(REGISTER_X, dtAngle16, AM_DMS);
      }
      else {
        displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToDms:", "cannot use an angle16 not tagged degree as an input of fnCvtDegToDms", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    case dtAngle34:
      if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToDms:", "cannot use NaN as an input of fnCvtDegToDms", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      if(getRegisterAngularMode(REGISTER_X) == AM_DEGREE) {
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE, AM_DMS);
        setRegisterDataType(REGISTER_X, dtAngle34, AM_DMS);
      }
      else {
        displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtDegToDms:", "cannot use an angle34 not tagged degree as an input of fnCvtDegToDms", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    default:
      displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "%s cannot be converted to an angle!", getRegisterDataTypeName(REGISTER_X, true, false));
        showInfoDialog("In function fnCvtDegToDms:", "the input value must be a real16, a real34, a long integer", errorMessage, NULL);
      #endif
      restoreStack();
      refreshStack();
      return;
  }

  refreshRegisterLine(REGISTER_X);
}



void fnCvtDmsToDeg(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_DMS, AM_DEGREE);
      setRegisterDataType(REGISTER_X, dtAngle16, AM_DEGREE);
      break;

    case dtReal16:
      if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use NaN as an input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_DMS, AM_DEGREE);
      setRegisterDataType(REGISTER_X, dtAngle16, AM_DEGREE);
      break;

    case dtReal34:
      if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use NaN as an input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DMS, AM_DEGREE);
      setRegisterDataType(REGISTER_X, dtAngle34, AM_DEGREE);
      break;

    case dtAngle16:
      if(real16IsNaN(REGISTER_REAL16_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use NaN as an input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      if(getRegisterAngularMode(REGISTER_X) == AM_DMS) {
        convertAngle16FromTo(REGISTER_REAL16_DATA(REGISTER_X), AM_DMS, AM_DEGREE);
        setRegisterDataType(REGISTER_X, dtAngle16, AM_DEGREE);
      }
      else {
        displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use an angle16 not tagged degree as an input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    case dtAngle34:
      if(real34IsNaN(REGISTER_REAL34_DATA(opX))) {
        displayCalcErrorMessage(1, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use NaN as an input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }

      if(getRegisterAngularMode(REGISTER_X) == AM_DMS) {
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DMS, AM_DEGREE);
        setRegisterDataType(REGISTER_X, dtAngle34, AM_DEGREE);
      }
      else {
        displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          showInfoDialog("In function fnCvtRadToDeg:", "cannot use an angle34 not tagged degree as an input of fnCvtRadToDeg", NULL, NULL);
        #endif
        restoreStack();
        refreshStack();
        return;
      }
      break;

    default:
      displayCalcErrorMessage(24, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "%s cannot be converted to an angle!", getRegisterDataTypeName(REGISTER_X, true, false));
        showInfoDialog("In function fnCvtRadToDeg:", "the input value must be a real16, a real34, a long integer", errorMessage, NULL);
      #endif
      restoreStack();
      refreshStack();
      return;
  }

  refreshRegisterLine(REGISTER_X);
}



void convertAngle16FromTo(real16_t *angle, uint32_t fromAngularMode, uint32_t toAngularMode) {
  real34_t angle34;

  real16ToReal34(angle, &angle34);
  convertAngle34FromTo(&angle34, fromAngularMode, toAngularMode);
  real34ToReal16(&angle34, angle);
}



void convertAngle34FromTo(real34_t *angle, uint32_t fromAngularMode, uint32_t toAngularMode) {
  int16_t  sign;
  bool_t toDms;
  real34_t degrees, minutes, seconds;
  real51_t angle51, real51;

  if(fromAngularMode == AM_DMS) {
    sign = real34IsNegative(angle) ? -1 : 1;
    real34SetPositiveSign(angle);

    real34ToIntegral(angle, &degrees);
    real34Subtract(angle, &degrees, angle);

    real34Multiply(angle, const34_100, angle);
    real34ToIntegral(angle, &minutes);
    real34Subtract(angle, &minutes, angle);

    real34Multiply(angle, const34_100, &seconds);

    if(real34CompareGreaterEqual(&seconds, const34_60)) {
      real34Subtract(&seconds, const34_60, &seconds);
      real34Add(&minutes, const34_1, &minutes);
    }

    if(real34CompareGreaterEqual(&minutes, const34_60)) {
      real34Subtract(&minutes, const34_60, &minutes);
      real34Add(&degrees, const34_1, &degrees);
    }

    real34ToReal51(&degrees, &angle51);

    real34ToReal51(&minutes, &real51);
    real51Divide(&real51, const51_60, &real51);
    real51Add(&angle51, &real51, &angle51);

    real34ToReal51(&seconds, &real51);
    real51Divide(&real51, const51_3600, &real51);
    real51Add(&angle51, &real51, &angle51);

    real51ToReal34(&angle51, angle);

    if(sign == -1) {
      real34SetNegativeSign(angle);
    }

    fromAngularMode = AM_DEGREE;
  }

  if(toAngularMode == AM_DMS) {
    toAngularMode = AM_DEGREE;
    toDms = true;
  }
  else {
    toDms = false;
  }

  real34ToReal51(angle, &angle51);

  switch(fromAngularMode) {
    case AM_DEGREE:
      switch(toAngularMode) {
        case AM_GRAD:   real51Divide(&angle51, const51_9on10,     &angle51); break;
        case AM_RADIAN: real51Divide(&angle51, const51_180onPi,   &angle51); break;
        case AM_MULTPI: real51Divide(&angle51, const51_180,       &angle51); break;
        default: {}
      }
      break;

    case AM_GRAD:
      switch(toAngularMode) {
        case AM_DEGREE: real51Multiply(&angle51, const51_9on10,   &angle51); break;
        case AM_RADIAN: real51Divide(  &angle51, const51_200onPi, &angle51); break;
        case AM_MULTPI: real51Divide(  &angle51, const51_200,     &angle51); break;
        default: {}
      }
      break;

    case AM_RADIAN:
      switch(toAngularMode) {
        case AM_DEGREE: real51Multiply(&angle51, const51_180onPi, &angle51); break;
        case AM_GRAD:   real51Multiply(&angle51, const51_200onPi, &angle51); break;
        case AM_MULTPI: real51Divide(  &angle51, const51_pi,      &angle51); break;
        default: {}
      }
      break;

    case AM_MULTPI:
      switch(toAngularMode) {
        case AM_DEGREE: real51Multiply(&angle51, const51_180,     &angle51); break;
        case AM_GRAD:   real51Multiply(&angle51, const51_200,     &angle51); break;
        case AM_RADIAN: real51Multiply(&angle51, const51_pi,      &angle51); break;
        default: {}
      }
      break;

    default: {}
  }

  real51ToReal34(&angle51, angle);

  if(toDms) {
    sign = real34IsNegative(angle) ? -1 : 1;
    real34SetPositiveSign(angle);

    real34ToIntegral(angle, &degrees);
    real34Subtract(angle, &degrees, angle);

    real34Multiply(angle, const34_60, angle);
    real34ToIntegral(angle, &minutes);
    real34Subtract(angle, &minutes, angle);

    real34Multiply(angle, const34_60, &seconds);

    real34ToReal51(&degrees, &angle51);

    real34Divide(&minutes, const34_100, &minutes);
    real34ToReal51(&minutes, &real51);
    real51Add(&angle51, &real51, &angle51);

    real34Divide(&seconds, const34_10000, &seconds);
    real34ToReal51(&seconds, &real51);
    real51Add(&angle51, &real51, &angle51);

    real51ToReal34(&angle51, angle);

    if(sign == -1) {
      real34SetNegativeSign(angle);
    }
  }
}



//void convertRegisterToAngularMode(calcRegister_t regist, uint16_t toAngularMode) {
//  uint32_t dataType = getRegisterDataType(regist);
//
//  if(dataType == dtReal16 || dataType == dtReal34) {
//    if(getRegisterAngularMode(regist) == toAngularMode) {
//      return;
//    }
//    else if(getRegisterTag(regist) != RT_REAL) {
//      if(dataType == dtReal16) {
//        convertAngle16FromTo(REGISTER_REAL16_DATA(regist), getRegisterTag(regist), toAngularMode);
//      }
//      else {
//        convertAngle34FromTo(REGISTER_REAL34_DATA(regist), getRegisterTag(regist), toAngularMode);
//      }
//    }
//
//    setRegisterTag(regist, toAngularMode);
//  }
//  else {
//    sprintf(errorMessage, "In function convertRegisterToAngularMode: cannot convert angular modes for %s!", getDataTypeName(dataType, true, false));
//    displayBugScreen(errorMessage);
//  }
//}



void checkDms16(real16_t *angleDms) {
  int16_t  sign;
  real16_t degrees, minutes, seconds;

  sign = real16IsNegative(angleDms) ? -1 : 1;
  real16SetPositiveSign(angleDms);

  real16ToIntegral(angleDms, &degrees);
  real16Subtract(angleDms, &degrees, angleDms);

  real16Multiply(angleDms, const16_100, angleDms);
  real16ToIntegral(angleDms, &minutes);
  real16Subtract(angleDms, &minutes, angleDms);

  real16Multiply(angleDms, const16_100, &seconds);

  if(real16CompareGreaterEqual(&seconds, const16_60)) {
    real16Subtract(&seconds, const16_60, &seconds);
    real16Add(&minutes, const16_1, &minutes);
  }

  if(real16CompareGreaterEqual(&minutes, const16_60)) {
    real16Subtract(&minutes, const16_60, &minutes);
    real16Add(&degrees, const16_1, &degrees);
  }

  real16Divide(&minutes, const16_100, &minutes);
  real16Add(&degrees, &minutes, angleDms);
  real16Divide(&seconds, const16_10000, &seconds);
  real16Add(angleDms, &seconds, angleDms);

  if(sign == -1) {
    real16SetNegativeSign(angleDms);
  }
}



void checkDms34(real34_t *angleDms) {
  int16_t  sign;
  real34_t degrees, minutes, seconds;

  sign = real34IsNegative(angleDms) ? -1 : 1;
  real34SetPositiveSign(angleDms);

  real34ToIntegral(angleDms, &degrees);
  real34Subtract(angleDms, &degrees, angleDms);

  real34Multiply(angleDms, const34_100, angleDms);
  real34ToIntegral(angleDms, &minutes);
  real34Subtract(angleDms, &minutes, angleDms);

  real34Multiply(angleDms, const34_100, &seconds);

  if(real34CompareGreaterEqual(&seconds, const34_60)) {
    real34Subtract(&seconds, const34_60, &seconds);
    real34Add(&minutes, const34_1, &minutes);
  }

  if(real34CompareGreaterEqual(&minutes, const34_60)) {
    real34Subtract(&minutes, const34_60, &minutes);
    real34Add(&degrees, const34_1, &degrees);
  }

  real34Divide(&minutes, const34_100, &minutes);
  real34Add(&degrees, &minutes, angleDms);
  real34Divide(&seconds, const34_10000, &seconds);
  real34Add(angleDms, &seconds, angleDms);

  if(sign == -1) {
    real34SetNegativeSign(angleDms);
  }
}
