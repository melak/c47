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
 * \file round.c
 ***********************************************/

#include "wp43s.h"



void (* const Round[12])(void) = {
// regX ==> 1            2          3          4           5          6          7           8          9           10            11         12
//          Long integer Real16     Complex16  Angle16     Time       Date       String      Real16 mat Complex16 m Short integer Real34     Complex34
            roundLonI,   roundRe16, roundCo16, roundError, roundTime, roundDate, roundError, roundRm16, roundCm16,  roundError,   roundRe34, roundCo34
};



/********************************************//**
 * \brief Data type error in round
 *
 * \param void
 * \return void
 ***********************************************/
void roundError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot calculate ROUND for %s", getRegisterDataTypeName(REGISTER_X, true, false));
    showInfoDialog("In function roundError:", errorMessage, NULL, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and round(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnRound(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  Round[getRegisterDataType(REGISTER_X)]();

  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}



void roundLonI(void) {
  int32_t pos, posE;
  longInteger_t lgInt;

  updateDisplayValueX = true;
  displayValueX[0] = 0;
  refreshRegisterLine(REGISTER_X);
  updateDisplayValueX = false;

  pos = 0;
  posE = -1;
  while(displayValueX[pos] != 0) {
    if(displayValueX[pos] == 'e') {
      posE = pos;
      break;
    }
    pos++;
  }

  longIntegerInit(lgInt);
  if(posE == -1) { // There is no exponent
    stringToLongInteger(displayValueX, 10, lgInt);
  }
  else { // There is an exponent
    displayValueX[posE] = 0;
    stringToLongInteger(displayValueX, 10, lgInt);

    pos = atoi(displayValueX + posE + 1);
    while(pos >= 9) {
      longIntegerMultiplyUInt(lgInt, 1000000000, lgInt);
      pos -= 9;
    }
    while(pos >= 4) {
      longIntegerMultiplyUInt(lgInt, 10000, lgInt);
      pos -= 4;
    }
    while(pos >= 1) {
      longIntegerMultiplyUInt(lgInt, 10, lgInt);
      pos -= 1;
    }
  }

  convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
  longIntegerFree(lgInt);
}



void roundRe16(void) {
  updateDisplayValueX = true;
  displayValueX[0] = 0;
  refreshRegisterLine(REGISTER_X);
  updateDisplayValueX = false;

  if(!displayRealAsFraction) {
    stringToReal16(displayValueX, REGISTER_REAL16_DATA(REGISTER_X));
  }
  else {
    int16_t endOfIntegerPart, slashPos;
    real16_t numerator, denominator;

    endOfIntegerPart = -1;
    if(fractionType == FT_PROPER) { // a b/c
      while(displayValueX[++endOfIntegerPart] != ' '); // The ending ; is OK here
      displayValueX[endOfIntegerPart] = 0;
      stringToReal16(displayValueX, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else { // FT_IMPROPER d/c
      real16Zero(REGISTER_REAL16_DATA(REGISTER_X));
    }

    slashPos = endOfIntegerPart++;
    while(displayValueX[++slashPos] != '/'); // The ending ; is OK here
    displayValueX[slashPos++] = 0;
    int32ToReal16(atoi(displayValueX + endOfIntegerPart), &numerator);
    int32ToReal16(atoi(displayValueX + slashPos), &denominator);
    real16Divide(&numerator, &denominator, &numerator);
    if(displayValueX[0] == '-') {
      real16Subtract(REGISTER_REAL16_DATA(REGISTER_X), &numerator, REGISTER_REAL16_DATA(REGISTER_X));
    }
    else {
      real16Add(REGISTER_REAL16_DATA(REGISTER_X), &numerator, REGISTER_REAL16_DATA(REGISTER_X));
    }
  }
}



void roundCo16(void) {
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
    real39_t magnitude, theta;

    stringToReal(displayValueX,        &magnitude, &ctxtReal39);
    stringToReal(displayValueX + posI, &theta,     &ctxtReal39);
    real39PolarToRectangular(&magnitude, &theta, &magnitude, &theta);
    realToReal16(&magnitude, REGISTER_REAL16_DATA(REGISTER_X));
    realToReal16(&theta,     REGISTER_IMAG16_DATA(REGISTER_X));
  }
  else {
    stringToReal16(displayValueX,        REGISTER_REAL16_DATA(REGISTER_X));
    stringToReal16(displayValueX + posI, REGISTER_IMAG16_DATA(REGISTER_X));
  }
}



void roundTime(void) {
  fnToBeCoded();
}



void roundDate(void) {
  fnToBeCoded();
}



void roundRm16(void) {
  fnToBeCoded();
}



void roundCm16(void) {
  fnToBeCoded();
}



void roundRe34(void) {
  updateDisplayValueX = true;
  displayValueX[0] = 0;
  refreshRegisterLine(REGISTER_X);
  updateDisplayValueX = false;

  if(!displayRealAsFraction) {
    stringToReal34(displayValueX, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    int16_t endOfIntegerPart, slashPos;
    real34_t numerator, denominator;

    endOfIntegerPart = -1;
    if(fractionType == FT_PROPER) { // a b/c
      while(displayValueX[++endOfIntegerPart] != ' '); // The ending ; is OK here
      displayValueX[endOfIntegerPart] = 0;
      stringToReal34(displayValueX, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else { // FT_IMPROPER d/c
      real34Zero(REGISTER_REAL34_DATA(REGISTER_X));
    }

    slashPos = endOfIntegerPart++;
    while(displayValueX[++slashPos] != '/'); // The ending ; is OK here
    displayValueX[slashPos++] = 0;
    int32ToReal34(atoi(displayValueX + endOfIntegerPart), &numerator);
    int32ToReal34(atoi(displayValueX + slashPos), &denominator);
    real34Divide(&numerator, &denominator, &numerator);
    if(displayValueX[0] == '-') {
      real34Subtract(REGISTER_REAL34_DATA(REGISTER_X), &numerator, REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      real34Add(REGISTER_REAL34_DATA(REGISTER_X), &numerator, REGISTER_REAL34_DATA(REGISTER_X));
    }
  }
}



void roundCo34(void) {
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
    real39_t magnitude, theta;

    stringToReal(displayValueX,        &magnitude, &ctxtReal39);
    stringToReal(displayValueX + posI, &theta,     &ctxtReal39);
    real39PolarToRectangular(&magnitude, &theta, &magnitude, &theta);
    realToReal34(&magnitude, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(&theta,     REGISTER_IMAG34_DATA(REGISTER_X));
  }
  else {
    stringToReal34(displayValueX,        REGISTER_REAL34_DATA(REGISTER_X));
    stringToReal34(displayValueX + posI, REGISTER_IMAG34_DATA(REGISTER_X));
  }
}
