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
 * \file toRect.c
 ***********************************************/

#include "mathematics/toRect.h"

#include "constantPointers.h"
#include "conversionAngles.h"
#include "debug.h"
#include "error.h"
#include "mathematics/wp34s.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43.h"



void fnToRect2(uint16_t unusedButMandatoryParameter) {
    uint32_t dataTypeX, dataTypeY, dataAtagX;
    if(getRegisterDataType(REGISTER_X) == dtComplex34) {
      setComplexRegisterPolarMode(REGISTER_X, ~amPolar);
      setComplexRegisterAngularMode(REGISTER_X, amNone);
    } else {

    if(getSystemFlag(FLAG_CLASSICRP)) {
      dataTypeX = getRegisterDataType(REGISTER_X); //original
      dataAtagX = getRegisterAngularMode(REGISTER_X);
      dataTypeY = getRegisterDataType(REGISTER_Y);      
    } else {
      dataTypeY = getRegisterDataType(REGISTER_X); //swapped
      dataTypeX = getRegisterDataType(REGISTER_Y);
      dataAtagX  = getRegisterAngularMode(REGISTER_Y);
    }
    // >R needs polar coords, i.e. X=r and Y=angle                                                 //imag not allowed to be an angle if rect entry:
    if((( dataTypeX == dtLongInteger || (dataTypeX == dtReal34 && dataAtagX == amNone   ))         //radius not allowed to be an angle if polar entry
      &&( dataTypeY == dtLongInteger || (dataTypeY == dtReal34 /*can be angle or not */ )) )  ) {  //real not allowed to be an angle if rect entry
    fnToRect(0);
    }
    else {
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "You cannot use >R or >P with %s in X and %s in Y!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false), getDataTypeName(getRegisterDataType(REGISTER_Y), true, false));
      moreInfoOnError("In function fnToRect2:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }
}



void fnToRect(uint16_t unusedButMandatoryParameter) {
  uint32_t dataTypeX, dataTypeY;
  calcRegister_t REG_X, REG_Y;
  real_t x, y;

  if(getSystemFlag(FLAG_CLASSICRP)) {
    REG_X = REGISTER_X;
    REG_Y = REGISTER_Y;
  } else {
    REG_X = REGISTER_Y;
    REG_Y = REGISTER_X;    
  }

  dataTypeX = getRegisterDataType(REG_X);
  dataTypeY = getRegisterDataType(REG_Y);      

  if((dataTypeX == dtReal34 || dataTypeX == dtLongInteger) && (dataTypeY == dtReal34 || dataTypeY == dtLongInteger)) {
    angularMode_t yAngularMode = getRegisterAngularMode(REG_Y);

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
        sprintf(errorMessage, commonBugScreenMessages[bugMsgUnexpectedSValue], "fnToRect", dataTypeX, "dataTypeX");
        displayBugScreen(errorMessage);
      }
    }

    if(yAngularMode == amNone) {
      yAngularMode = currentAngularMode;
    }

    switch(dataTypeY) {
      case dtLongInteger: {
        convertLongIntegerRegisterToReal(REG_Y, &y, &ctxtReal39);
        convertAngleFromTo(&y, currentAngularMode, amRadian, &ctxtReal39);
        break;
      }

      case dtReal34: {
        real34ToReal(REGISTER_REAL34_DATA(REG_Y), &y);
        convertAngleFromTo(&y, yAngularMode, amRadian, &ctxtReal39);
        break;
      }

      default: {
        sprintf(errorMessage, commonBugScreenMessages[bugMsgUnexpectedSValue], "fnToRect", dataTypeY, "dataTypeY");
        displayBugScreen(errorMessage);
      }
    }

    realPolarToRectangular(&x, &y, &x, &y, &ctxtReal39);

    reallocateRegister(REG_X, dtReal34, REAL34_SIZE, amNone);
    reallocateRegister(REG_Y, dtReal34, REAL34_SIZE, amNone);

    convertRealToReal34ResultRegister(&x, REG_X);
    convertRealToReal34ResultRegister(&y, REG_Y);

    if(getSystemFlag(FLAG_CLASSICRP)) {
      temporaryInformation = TI_X_Y;
    } else {
      temporaryInformation = TI_X_Y_SWAPPED;
    }
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REG_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot convert (%s, %s) to rectangular coordinates!", getDataTypeName(getRegisterDataType(REG_X), false, false), getDataTypeName(getRegisterDataType(REG_Y), false, false));
      moreInfoOnError("In function fnToRect:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}


/* never used
void real34PolarToRectangular(const real34_t *magnitude34, const real34_t *theta34, real34_t *real34, real34_t *imag34) {
  real_t real, imag, magnitude, theta;

  real34ToReal(magnitude34, &magnitude);
  real34ToReal(theta34, &theta);

  realPolarToRectangular(&magnitude, &theta, &real, &imag, &ctxtReal39);  // theta in radian

  realToReal34(&real, real34);
  realToReal34(&imag, imag34);
}
*/

void realPolarToRectangular(const real_t *mag, const real_t *the, real_t *real, real_t *imag, realContext_t *realContext) {
  ///////////////////////////////////////////
  //
  //  +----+----+-------------+------+------+
  //  | ρ  | θ  | Condition   | Re   | Im   |
  //  +----+----+-------------+------+------+
  //  |NaN |any |             |NaN   |NaN   |  1
  //  |any |NaN |             |NaN   |NaN   |  2
  //  |-∞  |-∞  |             |NaN   |NaN   |  3
  //  |-∞  |-π  |             |+∞    |0     |  4
  //  |-∞  |-π/2|             |0     |+∞    |  5
  //  |-∞  |0   |             |-∞    |0     |  6
  //  |-∞  |π/2 |             |0     |-∞    |  7
  //  |-∞  |π   |             |+∞    |0     |  8
  //  |-∞  |θ   |0 < θ < π/2  |-∞    |-∞    |  9
  //  |-∞  |θ   |π/2 < θ < π  |+∞    |-∞    | 10
  //  |-∞  |θ   |-π < θ < -π/2|+∞    |+∞    | 11
  //  |-∞  |θ   |-π/2 < θ < 0 |-∞    |+∞    | 12
  //  |-∞  |+∞  |             |NaN   |NaN   | 13
  //  |0   |-∞  |             |NaN   |NaN   | 14
  //  |0   |θ   |             |0     |0     | 15
  //  |0   |+∞  |             |NaN   |NaN   | 16
  //  |r   |-∞  |             |NaN   |NaN   | 17
  //  |r   |θ   |             |r·cosθ|r·sinθ| 18
  //  |r   |+∞  |             |NaN   |NaN   | 19
  //  |+∞  |-∞  |             |NaN   |NaN   | 20
  //  |+∞  |-π  |             |-∞    |0     | 21
  //  |+∞  |-π/2|             |0     |-∞    | 22
  //  |+∞  |0   |             |+∞    |0     | 23
  //  |+∞  |π/2 |             |0     |+∞    | 24
  //  |+∞  |π   |             |-∞    |0     | 25
  //  |+∞  |θ   |0 < θ < π/2  |+∞    |+∞    | 26
  //  |+∞  |θ   |π/2 < θ < π  |-∞    |+∞    | 27
  //  |+∞  |θ   |-π < θ < -π/2|-∞    |-∞    | 28
  //  |+∞  |θ   |-π/2 < θ < 0 |+∞    |-∞    | 29
  //  |+∞  |+∞  |             |NaN   |NaN   | 30

  real_t sin, cos, magnitude, theta;
  bool_t negativeAngle;

  // Testing NaNs and infinities
  if(realIsNaN(mag) || realIsNaN(the) || realIsInfinite(the)) {
    //  +----+----+-------------+------+------+
    //  | ρ  | θ  | Condition   | Re   | Im   |
    //  +----+----+-------------+------+------+
    //  |NaN |any |             |NaN   |NaN   |  1
    //  |any |NaN |             |NaN   |NaN   |  2
    //  |-∞  |-∞  |             |NaN   |NaN   |  3
    //  |-∞  |+∞  |             |NaN   |NaN   | 13
    //  |+∞  |-∞  |             |NaN   |NaN   | 20
    //  |+∞  |+∞  |             |NaN   |NaN   | 30
    //  |0   |-∞  |             |NaN   |NaN   | 14
    //  |0   |+∞  |             |NaN   |NaN   | 16
    //  |r   |-∞  |             |NaN   |NaN   | 17
    //  |r   |+∞  |             |NaN   |NaN   | 19
    realCopy(const_NaN, real);
    realCopy(const_NaN, imag);
    return;
  }

  realCopy(mag, &magnitude);
  WP34S_Mod(the, const1071_2pi, &theta, &ctxtReal75);  // here   0 ≤ theta < 2pi
  negativeAngle = realIsNegative(&theta);

  // Magnitude is infinite
  if(realIsInfinite(&magnitude)) {
    if(negativeAngle) {
      realChangeSign(&theta);
    }

    if(realIsZero(&theta)) { // theta = 0
      //  +----+----+-------------+------+------+
      //  | ρ  | θ  | Condition   | Re   | Im   |
      //  +----+----+-------------+------+------+
      //  |+∞  |0   |             |+∞    |0     | 23
      realCopy(const_plusInfinity, real);
      realZero(imag);
    }
    else {
      realSubtract(&theta, const_piOn2, real, &ctxtReal39);
      if(realIsZero(real)) { // theta = pi/2
        //  +----+----+-------------+------+------+
        //  | ρ  | θ  | Condition   | Re   | Im   |
        //  +----+----+-------------+------+------+
        //  |+∞  |π/2 |             |0     |+∞    | 24
        realZero(real);
        realCopy(const_plusInfinity, imag);
      }
      else {
        realSubtract(&theta, const_3piOn2, real, &ctxtReal39);
        if(realIsZero(real)) { // theta = -pi/2
          //  +----+----+-------------+------+------+
          //  | ρ  | θ  | Condition   | Re   | Im   |
          //  +----+----+-------------+------+------+
          //  |+∞  |-π/2|             |0     |-∞    | 22
          realZero(real);
          realCopy(const_minusInfinity, imag);
        }
        else {
          realSubtract(&theta, const_pi, real, &ctxtReal39);
          if(realIsZero(real)) { // theta = pi
            //  +----+----+-------------+------+------+
            //  | ρ  | θ  | Condition   | Re   | Im   |
            //  +----+----+-------------+------+------+
            //  |+∞  |-π  |             |-∞    |0     | 21
            //  |+∞  |π   |             |-∞    |0     | 25
            realCopy(const_minusInfinity, real);
            realZero(imag);
          }
          else {
            realSubtract(&theta, const_piOn2, &theta, &ctxtReal39);
            if(realIsNegative(&theta)) { //  0 < theta < pi/2
              //  +----+----+-------------+------+------+
              //  | ρ  | θ  | Condition   | Re   | Im   |
              //  +----+----+-------------+------+------+
              //  |+∞  |θ   |0 < θ < π/2  |+∞    |+∞    | 26
              realCopy(const_plusInfinity, real);
              realCopy(const_plusInfinity, imag);
            }
            else {
              realSubtract(&theta, const_piOn2, &theta, &ctxtReal39);
              if(realIsNegative(&theta)) { //  pi/2 < theta < pi
                //  +----+----+-------------+------+------+
                //  | ρ  | θ  | Condition   | Re   | Im   |
                //  +----+----+-------------+------+------+
                //  |+∞  |θ   |π/2 < θ < π  |-∞    |+∞    | 27
                realCopy(const_minusInfinity, real);
                realCopy(const_plusInfinity, imag);
              }
              else {
                realSubtract(&theta, const_piOn2, &theta, &ctxtReal39);
                if(realIsNegative(&theta)) { //  pi < theta < 3pi/2
                  //  +----+----+-------------+------+------+
                  //  | ρ  | θ  | Condition   | Re   | Im   |
                  //  +----+----+-------------+------+------+
                  //  |+∞  |θ   |-π < θ < -π/2|-∞    |-∞    | 28
                  realCopy(const_minusInfinity, real);
                  realCopy(const_minusInfinity, imag);
                }
                else { //  3pi/2 < theta < 2pi
                  //  +----+----+-------------+------+------+
                  //  | ρ  | θ  | Condition   | Re   | Im   |
                  //  +----+----+-------------+------+------+
                  //  |+∞  |θ   |-π/2 < θ < 0 |+∞    |-∞    | 29
                  realCopy(const_plusInfinity, real);
                  realCopy(const_minusInfinity, imag);
                }
              }
            }
          }
        }
      }
    }

    if(realIsNegative(&magnitude)) {
      //  +----+----+-------------+------+------+
      //  | ρ  | θ  | Condition   | Re   | Im   |
      //  +----+----+-------------+------+------+
      //  |-∞  |-π  |             |+∞    |0     |  4
      //  |-∞  |-π/2|             |0     |+∞    |  5
      //  |-∞  |0   |             |-∞    |0     |  6
      //  |-∞  |π/2 |             |0     |-∞    |  7
      //  |-∞  |π   |             |+∞    |0     |  8
      //  |-∞  |θ   |0 < θ < π/2  |-∞    |-∞    |  9
      //  |-∞  |θ   |π/2 < θ < π  |+∞    |-∞    | 10
      //  |-∞  |θ   |-π < θ < -π/2|+∞    |+∞    | 11
      //  |-∞  |θ   |-π/2 < θ < 0 |-∞    |+∞    | 12
      if(!realIsNaN(real) && !realIsZero(real)) {
        realChangeSign(real);
      }
      if(!realIsNaN(imag) && !realIsZero(imag)) {
        realChangeSign(imag);
      }
    }

    if(negativeAngle && !realIsNaN(imag) && !realIsZero(imag)) {
      realChangeSign(imag);
    }

    return;
  }

    //  +----+----+-------------+------+------+
    //  | ρ  | θ  | Condition   | Re   | Im   |
    //  +----+----+-------------+------+------+
    //  |0   |θ   |             |0     |0     | 15
    if(realIsZero(&magnitude)) {
      realZero(real);
      realZero(imag);
      return;
    }

    //  +----+----+-------------+------+------+
    //  | ρ  | θ  | Condition   | Re   | Im   |
    //  +----+----+-------------+------+------+
    //  |r   |θ   |             |r·cosθ|r·sinθ| 18

  WP34S_Cvt2RadSinCosTan(&theta, amRadian, &sin, &cos, NULL, realContext);
  realMultiply(&magnitude, &cos, real, realContext);
  realMultiply(&magnitude, &sin, imag, realContext);
}
