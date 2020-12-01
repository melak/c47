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
 * \file addition.c
 ***********************************************/

#include "wp43s.h"



void (* const addition[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(void) = {
// regX |    regY ==>   1            2            3            4            5            6            7            8            9             10
//      V               Long integer Real34       Complex34    Time         Date         String       Real34 mat   Complex34 m  Short integer Config data
/*  1 Long integer  */ {addLonILonI, addRealLonI, addCplxLonI, addTimeLonI, addDateLonI, addStriLonI, addError,    addError,    addShoILonI,  addError},
/*  2 Real34        */ {addLonIReal, addRealReal, addCplxReal, addTimeReal, addDateReal, addStriReal, addError,    addError,    addShoIReal,  addError},
/*  3 Complex34     */ {addLonICplx, addRealCplx, addCplxCplx, addError,    addError,    addStriCplx, addError,    addError,    addShoICplx,  addError},
/*  4 Time          */ {addLonITime, addRealTime, addError,    addTimeTime, addError,    addStriTime, addError,    addError,    addError,     addError},
/*  5 Date          */ {addLonIDate, addRealDate, addError,    addError,    addError,    addStriDate, addError,    addError,    addError,     addError},
/*  6 String        */ {addError,    addError,    addError,    addError,    addError,    addStriStri, addError,    addError,    addError,     addError},
/*  7 Real34 mat    */ {addError,    addError,    addError,    addError,    addError,    addStriRema, addRemaRema, addCxmaRema, addError,     addError},
/*  8 Complex34 mat */ {addError,    addError,    addError,    addError,    addError,    addStriCxma, addRemaCxma, addCxmaCxma, addError,     addError},
/*  9 Short integer */ {addLonIShoI, addRealShoI, addCplxShoI, addError,    addError,    addStriShoI, addError,    addError,    addShoIShoI,  addError},
/* 10 Config data   */ {addError,    addError,    addError,    addError,    addError,    addError,    addError,    addError,    addError,     addError}
};



/********************************************//**
 * \brief Data type error in addition
 *
 * \param void
 * \return void
 ***********************************************/
void addError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot add %s", getRegisterDataTypeName(REGISTER_X, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "to %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    moreInfoOnError("In function fnAdd:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
}



/********************************************//**
 * \brief regX ==> regL and regY + regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnAdd(uint16_t unusedButMandatoryParameter) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  addition[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

  adjustResult(REGISTER_X, true, true, REGISTER_X, REGISTER_Y, -1);
}



/******************************************************************************************************************************************************************************************/
/* long integer + ...                                                                                                                                                                     */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(long integer) + X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonILonI(void) {
  longInteger_t y, x;

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  longIntegerAdd(y, x, x);

  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

  longIntegerFree(y);
  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) + X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonITime(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) + X(long integer) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeLonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(long integer) + X(date) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonIDate(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(date) + X(long integer) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addDateLonI(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(long integer) + X(short integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonIShoI(void) {
  longInteger_t y, x;

  convertLongIntegerRegisterToLongInteger(REGISTER_Y, y);
  convertShortIntegerRegisterToLongInteger(REGISTER_X, x);

  longIntegerAdd(y, x, x);

  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

  longIntegerFree(y);
  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(short integer) + X(long integer) ==> X(long integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoILonI(void) {
  longInteger_t y, x;

  convertShortIntegerRegisterToLongInteger(REGISTER_Y, y);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  longIntegerAdd(y, x, x);

  convertLongIntegerToLongIntegerRegister(x, REGISTER_X);

  longIntegerFree(y);
  longIntegerFree(x);
}



/********************************************//**
 * \brief Y(long integer) + X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonIReal(void) {
  real_t y, x;
  uint32_t xAngularMode;

  convertLongIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == AM_NONE) {
    realAdd(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngleFromTo(&x, xAngularMode, AM_DEGREE, &ctxtReal39);
      realAdd(&y, &x, &x, &ctxtReal39);
      convertAngleFromTo(&x, AM_DEGREE, AM_DMS, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngleFromTo(&x, xAngularMode, currentAngularMode, &ctxtReal39);
      realAdd(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
   setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) + X(long integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRealLonI(void) {
  real_t y, x;
  uint32_t yAngularMode;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  convertLongIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  if(yAngularMode == AM_NONE) {
    realAdd(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngleFromTo(&y, yAngularMode, AM_DEGREE, &ctxtReal39);
      realAdd(&y, &x, &x, &ctxtReal39);
      convertAngleFromTo(&x, AM_DEGREE, AM_DMS, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngleFromTo(&y, yAngularMode, currentAngularMode, &ctxtReal39);
      realAdd(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(long integer) + X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addLonICplx(void) {
  real_t a, c;

  convertLongIntegerRegisterToReal(REGISTER_Y, &a, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &c);

  realAdd(&a, &c, &c, &ctxtReal39);

  realToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Y(complex34) + X(long integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCplxLonI(void) {
  real_t a, c;
  real34_t b;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &a);
  real34Copy(REGISTER_IMAG34_DATA(REGISTER_Y), &b);
  convertLongIntegerRegisterToReal(REGISTER_X, &c, &ctxtReal39);

  realAdd(&a, &c, &c, &ctxtReal39);

  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(&c, REGISTER_REAL34_DATA(REGISTER_X));
  real34Copy(&b, REGISTER_IMAG34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* time + ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(time) + X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeTime(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(time) + X(real34) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addTimeReal(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real34) + X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addRealTime(void) {
  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* date + ...                                                                                                                                                                             */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(date) + X(real34) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addDateReal(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real34) + X(date) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addRealDate(void) {
  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* string + ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(string) + X(long integer) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriLonI(void) {
  int16_t len1, len2;

  longIntegerRegisterToDisplayString(REGISTER_X, tmpString, TMP_STR_LENGTH, SCREEN_WIDTH, 50, STD_SPACE_PUNCTUATION);

  if(stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)) + stringGlyphLength(tmpString) > MAX_NUMBER_OF_GLYPHS_IN_STRING) {
    displayCalcErrorMessage(ERROR_STRING_WOULD_BE_TOO_LONG, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "the resulting string would be %d (Y %d + X %d) characters long. Maximum is %d",
                                                           stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)) + stringGlyphLength(tmpString),
                                                                 stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)),
                                                                        stringGlyphLength(tmpString),  MAX_NUMBER_OF_GLYPHS_IN_STRING);
      moreInfoOnError("In function addStriLonI:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else {
    len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
    len2 = stringByteLength(tmpString) + 1;

    reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len1 + len2), AM_NONE);

    xcopy(REGISTER_STRING_DATA(REGISTER_X),        REGISTER_STRING_DATA(REGISTER_Y), len1);
    xcopy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpString,                       len2);
  }
}



/********************************************//**
 * \brief Y(string) + X(time) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriTime(void) {
  int16_t len1, len2;

  timeToDisplayString(REGISTER_X, tmpString);

  if(stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)) + stringGlyphLength(tmpString) > MAX_NUMBER_OF_GLYPHS_IN_STRING) {
    displayCalcErrorMessage(ERROR_STRING_WOULD_BE_TOO_LONG, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "the resulting string would be %d (Y %d + X %d) characters long. Maximum is %d",
                                                           stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)) + stringGlyphLength(tmpString),
                                                                 stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)),
                                                                        stringGlyphLength(tmpString),  MAX_NUMBER_OF_GLYPHS_IN_STRING);
      moreInfoOnError("In function addStriTime:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else {
    len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
    len2 = stringByteLength(tmpString) + 1;

    reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len1 + len2), AM_NONE);

    xcopy(REGISTER_STRING_DATA(REGISTER_X)       , REGISTER_STRING_DATA(REGISTER_Y), len1);
    xcopy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpString,                       len2);
  }
}



/********************************************//**
 * \brief Y(string) + X(date) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriDate(void) {
  int16_t len1, len2;

  dateToDisplayString(REGISTER_X, tmpString);

  if(stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)) + stringGlyphLength(tmpString) > MAX_NUMBER_OF_GLYPHS_IN_STRING) {
    displayCalcErrorMessage(ERROR_STRING_WOULD_BE_TOO_LONG, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "the resulting string would be %d (Y %d + X %d) characters long. Maximum is %d",
                                                           stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)) + stringGlyphLength(tmpString),
                                                                 stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)),
                                                                        stringGlyphLength(tmpString),  MAX_NUMBER_OF_GLYPHS_IN_STRING);
      moreInfoOnError("In function addStriDate:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else {
    len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
    len2 = stringByteLength(tmpString) + 1;

    reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len1 + len2), AM_NONE);

    xcopy(REGISTER_STRING_DATA(REGISTER_X)       , REGISTER_STRING_DATA(REGISTER_Y), len1);
    xcopy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpString,                       len2);
  }
}



/********************************************//**
 * \brief Y(string) + X(string) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriStri(void) {
  int16_t len1, len2;

  if(stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)) + stringGlyphLength(REGISTER_STRING_DATA(REGISTER_X)) > MAX_NUMBER_OF_GLYPHS_IN_STRING) {
    displayCalcErrorMessage(ERROR_STRING_WOULD_BE_TOO_LONG, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "the resulting string would be %d (Y %d + X %d) characters long. Maximum is %d",
                                                           stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)) + stringGlyphLength(REGISTER_STRING_DATA(REGISTER_X)),
                                                                 stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)),
                                                                        stringGlyphLength(REGISTER_STRING_DATA(REGISTER_X)),
                                                                                                        MAX_NUMBER_OF_GLYPHS_IN_STRING);
      moreInfoOnError("In function addStriStri:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else {
    len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
    len2 = stringByteLength(REGISTER_STRING_DATA(REGISTER_X)) + 1;

    xcopy(tmpString, REGISTER_STRING_DATA(REGISTER_X), min(TMP_STR_LENGTH, len2));
    reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len1 + len2), AM_NONE);

    xcopy(REGISTER_STRING_DATA(REGISTER_X)       , REGISTER_STRING_DATA(REGISTER_Y), len1);
    xcopy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpString,                       len2);
  }
}



/********************************************//**
 * \brief Y(string) + X(real16 matrix) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriRema(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(string) + X(complex16 matrix) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriCxma(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(string) + X(short integer) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriShoI(void) {
  int16_t len1, len2;

  shortIntegerToDisplayString(REGISTER_X, tmpString, false);

  if(stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)) + stringGlyphLength(tmpString) > MAX_NUMBER_OF_GLYPHS_IN_STRING) {
    displayCalcErrorMessage(ERROR_STRING_WOULD_BE_TOO_LONG, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "the resulting string would be %d (Y %d + X %d) characters long. Maximum is %d",
                                                           stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)) + stringGlyphLength(tmpString),
                                                                 stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)),
                                                                        stringGlyphLength(tmpString),  MAX_NUMBER_OF_GLYPHS_IN_STRING);
      moreInfoOnError("In function addStriShoI:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else {
    len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
    len2 = stringByteLength(tmpString) + 1;

    reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len1 + len2), AM_NONE);

    xcopy(REGISTER_STRING_DATA(REGISTER_X)       , REGISTER_STRING_DATA(REGISTER_Y), len1);
    xcopy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpString,                        len2);
  }
}



/********************************************//**
 * \brief Y(string) + X(real34) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriReal(void) {
  int16_t len1, len2;

  real34ToDisplayString(REGISTER_REAL34_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X), tmpString, &standardFont, SCREEN_WIDTH, NUMBER_OF_DISPLAY_DIGITS, false, STD_SPACE_PUNCTUATION);

  if(stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)) + stringGlyphLength(tmpString) > MAX_NUMBER_OF_GLYPHS_IN_STRING) {
    displayCalcErrorMessage(ERROR_STRING_WOULD_BE_TOO_LONG, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "the resulting string would be %d (Y %d + X %d) characters long. Maximum is %d",
                                                           stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)) + stringGlyphLength(tmpString),
                                                                 stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)),
                                                                        stringGlyphLength(tmpString),  MAX_NUMBER_OF_GLYPHS_IN_STRING);
      moreInfoOnError("In function addStriReal:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else {
    len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
    len2 = stringByteLength(tmpString) + 1;

    reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len1 + len2), AM_NONE);

    xcopy(REGISTER_STRING_DATA(REGISTER_X),        REGISTER_STRING_DATA(REGISTER_Y), len1);
    xcopy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpString,                        len2);
  }
}



/********************************************//**
 * \brief Y(string) + X(complex34) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriCplx(void) {
  int16_t len1, len2;

  complex34ToDisplayString(REGISTER_COMPLEX34_DATA(REGISTER_X), tmpString, &numericFont, SCREEN_WIDTH, NUMBER_OF_DISPLAY_DIGITS, false, STD_SPACE_PUNCTUATION);

  if(stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)) + stringGlyphLength(tmpString) > MAX_NUMBER_OF_GLYPHS_IN_STRING) {
    displayCalcErrorMessage(ERROR_STRING_WOULD_BE_TOO_LONG, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "the resulting string would be %d (Y %d + X %d) characters long. Maximum is %d",
                                                           stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)) + stringGlyphLength(tmpString),
                                                                 stringGlyphLength(REGISTER_STRING_DATA(REGISTER_Y)),
                                                                        stringGlyphLength(tmpString),  MAX_NUMBER_OF_GLYPHS_IN_STRING);
      moreInfoOnError("In function addStriCplx:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else {
    len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
    len2 = stringByteLength(tmpString) + 1;

    reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len1 + len2), AM_NONE);

    xcopy(REGISTER_STRING_DATA(REGISTER_X)       , REGISTER_STRING_DATA(REGISTER_Y), len1);
    xcopy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpString,                        len2);
  }
}



/******************************************************************************************************************************************************************************************/
/* real16 matrix + ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real16 matrix) + X(real16 matrix) ==> X(real16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void addRemaRema(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real16 matrix) + X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void addRemaCxma(void) {
  fnToBeCoded();
}



/********************************************//**
 * \brief Y(complex16 matrix) + X(real16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void addCxmaRema(void) {
  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* complex16 matrix + ...                                                                                                                                                                 */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex16 matrix) + X(complex16 matrix) ==> X(complex16 matrix)
 *
 * \param void
 * \return void
 ***********************************************/
void addCxmaCxma(void) {
  fnToBeCoded();
}



/******************************************************************************************************************************************************************************************/
/* short integer + ...                                                                                                                                                                    */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(short integer) + X(short integer) ==> X(short integer)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoIShoI(void) {
  setRegisterTag(REGISTER_X, getRegisterTag(REGISTER_Y));
  *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = WP34S_intAdd(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)), *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)));
}



/********************************************//**
 * \brief Y(short integer) + X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoIReal(void) {
  real_t y, x;
  uint32_t xAngularMode;

  convertShortIntegerRegisterToReal(REGISTER_Y, &y, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(xAngularMode == AM_NONE) {
    realAdd(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngleFromTo(&x, xAngularMode, AM_DEGREE, &ctxtReal39);
      realAdd(&y, &x, &x, &ctxtReal39);
      convertAngleFromTo(&x, AM_DEGREE, AM_DMS, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngleFromTo(&x, xAngularMode, currentAngularMode, &ctxtReal39);
      realAdd(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
   setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) + X(short integer) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRealShoI(void) {
  real_t y, x;
  uint32_t yAngularMode;

  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  convertShortIntegerRegisterToReal(REGISTER_X, &x, &ctxtReal39);
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);

  if(yAngularMode == AM_NONE) {
    realAdd(&y, &x, &x, &ctxtReal39);
    realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    if(currentAngularMode == AM_DMS) {
      convertAngleFromTo(&y, yAngularMode, AM_DEGREE, &ctxtReal39);
      realAdd(&y, &x, &x, &ctxtReal39);
      convertAngleFromTo(&x, AM_DEGREE, AM_DMS, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngleFromTo(&y, yAngularMode, currentAngularMode, &ctxtReal39);
      realAdd(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(short integer) + X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addShoICplx(void) {
  convertShortIntegerRegisterToReal34Register(REGISTER_Y, REGISTER_Y);
  real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
}



/********************************************//**
 * \brief Y(complex34) + X(short integer) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCplxShoI(void) {
  convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X));
}



/******************************************************************************************************************************************************************************************/
/* real34 + ...                                                                                                                                                                           */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(real34) + X(real34) ==> X(real34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRealReal(void) {
  uint32_t yAngularMode, xAngularMode;

  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(yAngularMode == AM_NONE && xAngularMode == AM_NONE) {
    real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    real_t y, x;

    if(yAngularMode == AM_NONE) {
      yAngularMode = currentAngularMode;
    }
    else if(xAngularMode == AM_NONE) {
      xAngularMode = currentAngularMode;
    }

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

    if(currentAngularMode == AM_DMS) {
      convertAngleFromTo(&y, yAngularMode, AM_DEGREE, &ctxtReal39);
      convertAngleFromTo(&x, xAngularMode, AM_DEGREE, &ctxtReal39);

      realAdd(&y, &x, &x, &ctxtReal39);

      convertAngleFromTo(&x, AM_DEGREE, AM_DMS, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else { //current angular mode is not DMS
      convertAngleFromTo(&y, yAngularMode, currentAngularMode, &ctxtReal39);
      convertAngleFromTo(&x, xAngularMode, currentAngularMode, &ctxtReal39);

      realAdd(&y, &x, &x, &ctxtReal39);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
    }
    setRegisterAngularMode(REGISTER_X, currentAngularMode);
  }
}



/********************************************//**
 * \brief Y(real34) + X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addRealCplx(void) {
  real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
}



/********************************************//**
 * \brief Y(complex34) + X(real34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCplxReal(void) {
  real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_Y)); // real part
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_Y), REGISTER_COMPLEX34_DATA(REGISTER_X)); // imaginary part
}



/******************************************************************************************************************************************************************************************/
/* complex34 + ...                                                                                                                                                                        */
/******************************************************************************************************************************************************************************************/

/********************************************//**
 * \brief Y(complex34) + X(complex34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCplxCplx(void) {
  real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34Add(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // imaginary part
}
