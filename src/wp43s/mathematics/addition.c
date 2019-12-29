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



void (* const addition[9][9])(void) = {
// regX |    regY ==>   1            2            3            4            5            6            7            8            9
//      V               Long integer Real34       Complex34    Time         Date         String       Real34 mat   Complex34 m  Short integer
/*  1 Long integer  */ {addLonILonI, addRealLonI, addCplxLonI, addTimeLonI, addDateLonI, addStriLonI, addError,    addError,    addShoILonI},
/*  2 Real34        */ {addLonIReal, addRealReal, addCplxReal, addTimeReal, addDateReal, addStriReal, addError,    addError,    addShoIReal},
/*  3 Complex34     */ {addLonICplx, addRealCplx, addCplxCplx, addError,    addError,    addStriCplx, addError,    addError,    addShoICplx},
/*  4 Time          */ {addLonITime, addRealTime, addError,    addTimeTime, addError,    addStriTime, addError,    addError,    addError   },
/*  5 Date          */ {addLonIDate, addRealDate, addError,    addError,    addError,    addStriDate, addError,    addError,    addError   },
/*  6 String        */ {addError,    addError,    addError,    addError,    addError,    addStriStri, addError,    addError,    addError   },
/*  7 Real34 mat    */ {addError,    addError,    addError,    addError,    addError,    addStriRema, addRemaRema, addCxmaRema, addError   },
/*  8 Complex34 mat */ {addError,    addError,    addError,    addError,    addError,    addStriCxma, addRemaCxma, addCxmaCxma, addError   },
/*  9 Short integer */ {addLonIShoI, addRealShoI, addCplxShoI, addError,    addError,    addStriShoI, addError,    addError,    addShoIShoI}
};



/********************************************//**
 * \brief Data type error in addition
 *
 * \param void
 * \return void
 ***********************************************/
void addError(void) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_INPUT_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot add %s", getRegisterDataTypeName(REGISTER_X, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "to %s", getRegisterDataTypeName(REGISTER_Y, true, false));
    showInfoDialog("In function fnAdd:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}



/********************************************//**
 * \brief regX ==> regL and regY + regX ==> regX
 * Drops Y, enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnAdd(uint16_t unusedParamButMandatory) {
  saveStack();
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
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addLonIReal:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;
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
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);
      realAdd(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);
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
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRealLonI:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;
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
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      realAdd(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
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
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addLonICplx:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  real39_t a, c;

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
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCplxLonI:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  real39_t a, c;
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
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addTimeReal:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real34) + X(time) ==> X(time)
 *
 * \param void
 * \return void
 ***********************************************/
void addRealTime(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRealTime:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

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
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addDateReal:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  fnToBeCoded();
}



/********************************************//**
 * \brief Y(real34) + X(date) ==> X(date)
 *
 * \param void
 * \return void
 ***********************************************/
void addRealDate(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRealDate:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

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

  len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
  longIntegerToDisplayString(REGISTER_X, tmpStr3000, TMP_STR_LENGTH, SCREEN_WIDTH);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(REGISTER_X, dtString, len1 + len2, AM_NONE);

  memcpy(REGISTER_STRING_DATA(REGISTER_X),        REGISTER_STRING_DATA(REGISTER_Y), len1    );
  memcpy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpStr3000,                       len2 + 1);
}



/********************************************//**
 * \brief Y(string) + X(time) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriTime(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
  timeToDisplayString(REGISTER_X, tmpStr3000);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(REGISTER_X, dtString, len1 + len2, AM_NONE);

  memcpy(REGISTER_STRING_DATA(REGISTER_X)       , REGISTER_STRING_DATA(REGISTER_Y), len1    );
  memcpy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpStr3000,                       len2 + 1);
}



/********************************************//**
 * \brief Y(string) + X(date) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriDate(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
  dateToDisplayString(REGISTER_X, tmpStr3000);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(REGISTER_X, dtString, len1 + len2, AM_NONE);

  memcpy(REGISTER_STRING_DATA(REGISTER_X)       , REGISTER_STRING_DATA(REGISTER_Y), len1    );
  memcpy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpStr3000,                       len2 + 1);
}



/********************************************//**
 * \brief Y(string) + X(string) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriStri(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
  len2 = stringByteLength(REGISTER_STRING_DATA(REGISTER_X));

  memcpy(tmpStr3000, REGISTER_STRING_DATA(REGISTER_X), min(TMP_STR_LENGTH, len2 + 1));
  reallocateRegister(REGISTER_X, dtString, len1 + len2, AM_NONE);

  memcpy(REGISTER_STRING_DATA(REGISTER_X)       , REGISTER_STRING_DATA(REGISTER_Y), len1    );
  memcpy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpStr3000,                       len2 + 1);
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
  const font_t *font;

  len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
  font = &standardFont;
  shortIntegerToDisplayString(REGISTER_X, errorMessage, &font); // We use errorMessage here because this string can become very long
  len2 = stringByteLength(errorMessage) + 1; // +1 for the trailing 0

  reallocateRegister(REGISTER_X, dtString, len1 + len2, AM_NONE);

  memcpy(REGISTER_STRING_DATA(REGISTER_X)       , REGISTER_STRING_DATA(REGISTER_Y), len1    );
  memcpy(REGISTER_STRING_DATA(REGISTER_X) + len1, errorMessage,                     len2 + 1);
}



/********************************************//**
 * \brief Y(string) + X(real34) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriReal(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
  real34ToDisplayString(REGISTER_REAL34_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X), tmpStr3000, &standardFont, SCREEN_WIDTH, NUMBER_OF_DISPLAY_DIGITS);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(REGISTER_X, dtString, len1 + len2, AM_NONE);

  memcpy(REGISTER_STRING_DATA(REGISTER_X),        REGISTER_STRING_DATA(REGISTER_Y), len1    );
  memcpy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpStr3000,                       len2 + 1);
}



/********************************************//**
 * \brief Y(string) + X(complex34) ==> X(string)
 *
 * \param void
 * \return void
 ***********************************************/
void addStriCplx(void) {
  int16_t len1, len2;

  len1 = stringByteLength(REGISTER_STRING_DATA(REGISTER_Y));
  complex34ToDisplayString(REGISTER_COMPLEX34_DATA(REGISTER_X), tmpStr3000, &numericFont, SCREEN_WIDTH, NUMBER_OF_DISPLAY_DIGITS);
  len2 = stringByteLength(tmpStr3000);

  reallocateRegister(REGISTER_X, dtString, len1 + len2, AM_NONE);

  memcpy(REGISTER_STRING_DATA(REGISTER_X)       , REGISTER_STRING_DATA(REGISTER_Y), len1    );
  memcpy(REGISTER_STRING_DATA(REGISTER_X) + len1, tmpStr3000,                       len2 + 1);
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
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addShoIReal:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;
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
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);
      realAdd(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);
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
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRealShoI:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  real39_t y, x;
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
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      realAdd(&y, &x, &x, &ctxtReal39);
      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else {
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
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
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addShoICplx:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

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
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCplxShoI:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

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
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRealReal:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRealReal:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  uint32_t yAngularMode, xAngularMode;

  yAngularMode = getRegisterAngularMode(REGISTER_Y);
  xAngularMode = getRegisterAngularMode(REGISTER_X);

  if(yAngularMode == AM_NONE && xAngularMode == AM_NONE) {
    real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    real39_t y, x;

    if(yAngularMode == AM_NONE) {
      yAngularMode = currentAngularMode;
    }
    else if(xAngularMode == AM_NONE) {
      xAngularMode = currentAngularMode;
    }

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);

    if(currentAngularMode == AM_DMS) {
      convertAngle39FromTo(&y, yAngularMode, AM_DEGREE);
      convertAngle39FromTo(&x, xAngularMode, AM_DEGREE);

      realAdd(&y, &x, &x, &ctxtReal39);

      convertAngle39FromTo(&x, AM_DEGREE, AM_DMS);
      realToReal34(&x, REGISTER_REAL34_DATA(REGISTER_X));
      checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
    }
    else { //current angular mode is not DMS
      convertAngle39FromTo(&y, yAngularMode, currentAngularMode);
      convertAngle39FromTo(&x, xAngularMode, currentAngularMode);

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
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRealCplx:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addRealCplx:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
}



/********************************************//**
 * \brief Y(complex34) + X(real34) ==> X(complex34)
 *
 * \param void
 * \return void
 ***********************************************/
void addCplxReal(void) {
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCplxReal:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCplxReal:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

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
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCplxCplx:", "cannot use NaN as Y input of +", NULL, NULL);
    #endif
    return;
  }

  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X))) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function addCplxCplx:", "cannot use NaN as X input of +", NULL, NULL);
    #endif
    return;
  }

  real34Add(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X)); // real part
  real34Add(REGISTER_IMAG34_DATA(REGISTER_Y), REGISTER_IMAG34_DATA(REGISTER_X), REGISTER_IMAG34_DATA(REGISTER_X)); // imaginary part
}
