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
 * \file toPolar.c
 ***********************************************/

#include "mathematics/toPolar.h"

#include "constantPointers.h"
#include "conversionAngles.h"
#include "debug.h"
#include "error.h"
#include "mathematics/wp34s.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43.h"


void fnToPolar2(uint16_t unusedButMandatoryParameter) {
  uint32_t dataTypeX, dataTypeY, dataAtagX, dataAtagY;
  if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    setComplexRegisterPolarMode(REGISTER_X, amPolar);
    if(getComplexRegisterAngularMode(REGISTER_X) == amNone) {
      setComplexRegisterAngularMode(REGISTER_X, currentAngularMode);
    }
    return;
  }
  //X and Y are both only checked for REAL - symmetrical. Therefore clasRP does not play a role in the type checking even when swapped
  dataTypeX = getRegisterDataType(REGISTER_X);
  dataAtagX  = getRegisterAngularMode(REGISTER_X);
  dataTypeY = getRegisterDataType(REGISTER_Y);
  dataAtagY  = getRegisterAngularMode(REGISTER_Y);

  // >P needs rectangular coords, i.e. X=real and Y=real
  if(  (( dataTypeX == dtLongInteger || (dataTypeX == dtReal34 && dataAtagX == amNone  ))  //radius not allowed to be an angle if polar entry
     && ( dataTypeY == dtLongInteger || (dataTypeY == dtReal34 && dataAtagY == amNone  ))) //real not allowed to be an angle if rect entry
    ) {  //imag not allowed to be an angle if rect entry
    fnToPolar(0);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
  #if(EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "You cannot use >R or >P with %s in X and %s in Y!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false), getDataTypeName(getRegisterDataType(REGISTER_Y), true, false));
    moreInfoOnError("In function fnToPolar2:", errorMessage, NULL, NULL);
  #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}


void fnToPolar(uint16_t unusedButMandatoryParameter) {
  uint32_t dataTypeX, dataTypeY;
  calcRegister_t REG_X, REG_Y;
  real_t x, y;

  if(getSystemFlag(FLAG_HPRP)) {
    REG_X = REGISTER_X;
    REG_Y = REGISTER_Y;
  }
  else {
    REG_X = REGISTER_Y;
    REG_Y = REGISTER_X;
  }

  dataTypeX = getRegisterDataType(REG_X);
  dataTypeY = getRegisterDataType(REG_Y);

  if((dataTypeX == dtReal34 || dataTypeX == dtLongInteger) && (dataTypeY == dtReal34 || dataTypeY == dtLongInteger)) {
    if(!saveLastX()) {
    return;
  }

    switch(dataTypeX) {
      case dtLongInteger: {
        convertLongIntegerRegisterToReal(REG_X, &x, &ctxtReal39);
        break;
      }
      case dtReal34: {
        real34ToReal(REGISTER_REAL34_DATA(REG_X), &x);
        break;
      }
      default: {
        sprintf(errorMessage, commonBugScreenMessages[bugMsgUnexpectedSValue], "fnToPolar", dataTypeX, "dataTypeX");
        displayBugScreen(errorMessage);
      }
    }

    switch(dataTypeY) {
      case dtLongInteger: {
        convertLongIntegerRegisterToReal(REG_Y, &y, &ctxtReal39);
        break;
      }
      case dtReal34: {
        real34ToReal(REGISTER_REAL34_DATA(REG_Y), &y);
        break;
      }
      default: {
        sprintf(errorMessage, commonBugScreenMessages[bugMsgUnexpectedSValue], "fnToPolar", dataTypeY, "dataTypeY");
        displayBugScreen(errorMessage);
      }
    }

    realRectangularToPolar(&x, &y, &x, &y, &ctxtReal39);
    convertAngleFromTo(&y, amRadian, currentAngularMode, &ctxtReal39);

    reallocateRegister(REG_X, dtReal34, REAL34_SIZE, amNone);              //original
    reallocateRegister(REG_Y, dtReal34, REAL34_SIZE, currentAngularMode);
    convertRealToReal34ResultRegister(&x, REG_X);
    convertRealToReal34ResultRegister(&y, REG_Y);

    if(getSystemFlag(FLAG_HPRP)) {
      temporaryInformation = TI_RADIUS_THETA;
    }
    else {
      temporaryInformation = TI_RADIUS_THETA_SWAPPED;
    }
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REG_X);
    #if(EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot convert (%s, %s) to polar coordinates!", getDataTypeName(getRegisterDataType(REG_X), false, false), getDataTypeName(getRegisterDataType(REG_Y), false, false));
      moreInfoOnError("In function fnToPolar:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}



// The theta34 output angle is in radian
void real34RectangularToPolar(const real34_t *real34, const real34_t *imag34, real34_t *magnitude34, real34_t *theta34) {
  real_t real, imag, magnitude, theta;

  real34ToReal(real34, &real);
  real34ToReal(imag34, &imag);

  realRectangularToPolar(&real, &imag, &magnitude, &theta, &ctxtReal39); // theta in radian

  realToReal34(&magnitude, magnitude34);
  realToReal34(&theta, theta34);
}



void realRectangularToPolar(const real_t *real, const real_t *imag, real_t *magnitude, real_t *theta, realContext_t *realContext) { // theta is in ]-pi, pi]
  ///////////////////////////////////////////
  //
  //    a > 0  and  b > 0
  //
  //  +----+----+--------+------------+
  //  | Re | Im | ρ      | θ          |
  //  +----+----+--------+------------+
  //  |NaN |any |NaN     |NaN         |   1
  //  |any |NaN |NaN     |NaN         |   2
  //  |-∞  |-∞  |∞       |-3π/4       |   3
  //  |-∞  |-b  |∞       |π           |   4
  //  |-∞  |0   |∞       |π           |   5
  //  |-∞  |+b  |∞       |π           |   6
  //  |-∞  |+∞  |∞       |3π/4        |   7
  //  |-a  |-∞  |∞       |-π/2        |   8
  //  |-a  |-b  |√(a²+b²)|atan2(-b,-a)|   9
  //  |-a  |0   |a       |π           |  10
  //  |-a  |+b  |√(a²+b²)|atan2(+b,-a)|  11
  //  |-a  |+∞  |∞       |π/2         |  12
  //  |0   |-∞  |∞       |-π/2        |  13
  //  |0   |-b  |b       |-π/2        |  14
  //  |0   |0   |0       |0           |  15
  //  |0   |+b  |b       |π/2         |  16
  //  |0   |+∞  |∞       |π/2         |  17
  //  |+a  |-∞  |∞       |-π/2        |  18
  //  |+a  |-b  |√(a²+b²)|atan2(-b,+a)|  19
  //  |+a  |0   |a       |0           |  20
  //  |+a  |+b  |√(a²+b²)|atan2(+b,+a)|  21
  //  |+a  |+∞  |∞       |π/2         |  22
  //  |+∞  |-∞  |∞       |-π/4        |  23
  //  |+∞  |-b  |∞       |0           |  24
  //  |+∞  |0   |∞       |0           |  25
  //  |+∞  |+b  |∞       |0           |  26
  //  |+∞  |+∞  |∞       |π/4         |  27

  real_t re, im;

  realCopy(real, &re);
  realCopy(imag, &im);

  // Testing NaNs
  if(realIsNaN(&re) || realIsNaN(&im)) {
    //  +----+----+--------+------------+
    //  | Re | Im | ρ      | θ          |
    //  +----+----+--------+------------+
    //  |NaN |any |NaN     |NaN         |   1
    //  |any |NaN |NaN     |NaN         |   2
    realCopy(const_NaN, magnitude);
    realCopy(const_NaN, theta);
    return;
  }

  // Real part is infinite
  if(realIsInfinite(&re)) {
    realCopy(const_plusInfinity, magnitude);

    if(realIsPositive(&re)) { // re = +inf
      //  +----+----+--------+------------+
      //  | Re | Im | ρ      | θ          |
      //  +----+----+--------+------------+
      //  |+∞  |-∞  |∞       |-π/4        |  23
      //  |+∞  |-b  |∞       |0           |  24
      //  |+∞  |0   |∞       |0           |  25
      //  |+∞  |+b  |∞       |0           |  26
      //  |+∞  |+∞  |∞       |π/4         |  27
      if(realIsInfinite(&im)) { // re = +inf  im = ±inf
        realCopy(const_piOn4, theta);

        if(realIsNegative(&im)) { // re = +inf  im = -inf
          realSetNegativeSign(theta);
        }
      }
      else { // re = +inf  im ≠ infinite
        realZero(theta);
      }
    }
    else { // re = -inf
      //  +----+----+--------+------------+
      //  | Re | Im | ρ      | θ          |
      //  +----+----+--------+------------+
      //  |-∞  |-∞  |∞       |-3π/4       |   3
      //  |-∞  |-b  |∞       |π           |   4
      //  |-∞  |0   |∞       |π           |   5
      //  |-∞  |+b  |∞       |π           |   6
      //  |-∞  |+∞  |∞       |3π/4        |   7
      if(realIsInfinite(&im)) { // re = -inf  im = ±inf
        realCopy(const_3piOn4, theta);

        if(realIsNegative(&im)) { // re = -inf  im = -inf
          realSetNegativeSign(theta);
        }
      }
      else { // re = -inf  im ≠ infinite
        realCopy(const_pi, theta);
      }
    }

    return;
  }

  // Imaginary part is infinite
  if(realIsInfinite(&im)) {
    //  +----+----+--------+------------+
    //  | Re | Im | ρ      | θ          |
    //  +----+----+--------+------------+
    //  |-a  |+∞  |∞       |π/2         |  12
    //  |0   |+∞  |∞       |π/2         |  17
    //  |+a  |+∞  |∞       |π/2         |  22
    //  |-a  |-∞  |∞       |-π/2        |   8
    //  |0   |-∞  |∞       |-π/2        |  13
    //  |+a  |-∞  |∞       |-π/2        |  18
    realCopy(const_plusInfinity, magnitude);
    realCopy(const_piOn2, theta);

    if(realIsNegative(&im)) { // im = -inf
      realSetNegativeSign(theta);
    }

    return;
  }

  // Real part = 0
  if(realIsZero(&re)) { // re = 0
    //  +----+----+--------+------------+
    //  | Re | Im | ρ      | θ          |
    //  +----+----+--------+------------+
    //  |0   |0   |0       |0           |  15
    //  |0   |-b  |b       |-π/2        |  14
    //  |0   |+b  |b       |π/2         |  16
    if(realIsZero(&im)) { // re = 0  im = 0
      realZero(magnitude);
      realZero(theta);
    }
    else { // re = 0  im ≠ 0
      realCopyAbs(&im, magnitude);
      realCopy(const_piOn2, theta); // 90°

      if(realIsNegative(&im)) { // re = 0  im < 0
        realSetNegativeSign(theta); // -90°
      }
    }

    return;
  }

  // Imaginary part = 0
  if(realIsZero(&im)) { // im = 0
    //  +----+----+--------+------------+
    //  | Re | Im | ρ      | θ          |
    //  +----+----+--------+------------+
    //  |-a  |0   |a       |π           |  10
    //  |+a  |0   |a       |0           |  20
    realCopyAbs(&re, magnitude);

    if(realIsNegative(&re)) { // re < 0  im = 0
      realCopy(const_pi, theta); // 180°
    }
    else { // re > 0  im = 0
      realZero(theta); // 0°
    }

    return;
  }

  // Real and imagynary part not special and not zero
  //  +----+----+--------+------------+
  //  | Re | Im | ρ      | θ          |
  //  +----+----+--------+------------+
  //  |-a  |-b  |√(a²+b²)|atan2(-b,-a)|   9
  //  |-a  |+b  |√(a²+b²)|atan2(+b,-a)|  11
  //  |+a  |-b  |√(a²+b²)|atan2(-b,+a)|  19
  //  |+a  |+b  |√(a²+b²)|atan2(+b,+a)|  21

  if(realContext->digits > 75) {
    sprintf(errorMessage, "In function realRectangularToPolar: The number of digits is > 75");
    displayBugScreen(errorMessage);
  }
  else {
    // Magnitude
    realMultiply(&re, &re, magnitude, realContext);
    realFMA(&im, &im, magnitude, magnitude, realContext);
    realSquareRoot(magnitude, magnitude, realContext);

    // Angle
    WP34S_Atan2(&im, &re, theta, realContext);
  }
}
