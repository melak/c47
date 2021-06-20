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
 * \file dateTime.c
 ***********************************************/

#include "dateTime.h"

#include "debug.h"
#include "error.h"
#include "flags.h"
#include "items.h"
#include "mathematics/comparisonReals.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "stack.h"
#include <string.h>
#include <time.h>

#include "wp43s.h"

void fnSetDateFormat(uint16_t dateFormat) {
  switch(dateFormat) {
    case ITM_DMY:
      clearSystemFlag(FLAG_MDY);
      clearSystemFlag(FLAG_YMD);
      setSystemFlag(FLAG_DMY);
      break;

    case ITM_MDY:
      clearSystemFlag(FLAG_DMY);
      clearSystemFlag(FLAG_YMD);
      setSystemFlag(FLAG_MDY);
      break;

    case ITM_YMD:
      clearSystemFlag(FLAG_MDY);
      clearSystemFlag(FLAG_DMY);
      setSystemFlag(FLAG_YMD);
      break;

    default: {}
  }
}

/********************************************//**
 * \brief Convert internal date representation to Julian day number (floored)
 *
 * \param[in] source real34_t*
 * \param[out] destination real34_t*
 * \return void
 ***********************************************/
void internalDateToJulianDay(const real34_t *source, real34_t *destination) {
  real34_t val;

  int32ToReal34(43200, &val), real34Subtract(source, &val, destination);
  int32ToReal34(86400, &val), real34Divide(destination, &val, destination), real34ToIntegralValue(destination, destination, DEC_ROUND_FLOOR);
}

/********************************************//**
 * \brief Convert Julian day number (floored) to internal date representation
 *
 * \param[in] source real34_t*
 * \param[out] destination real34_t*
 * \return void
 ***********************************************/
void julianDayToInternalDate(const real34_t *source, real34_t *destination) {
  real34_t val;

  real34ToIntegralValue(source, destination, DEC_ROUND_FLOOR);
  int32ToReal34(86400, &val), real34Multiply(destination, &val, destination);
  int32ToReal34(43200, &val), real34Add(destination, &val, destination);
}

/********************************************//**
 * \brief Check date argument and get Julian day
 *
 * \param[in] regist calcRegister_t register
 * \param[out] jd real34_t* Julian day (floored)
 * \return bool_t true if valid
 ***********************************************/
bool_t checkDateArgument(calcRegister_t regist, real34_t *jd) {
  switch(getRegisterDataType(regist)) {
    case dtDate:
      internalDateToJulianDay(REGISTER_REAL34_DATA(regist), jd);
      return true;

    case dtReal34:
      if(getRegisterAngularMode(regist) == amNone) {
        reallocateRegister(TEMP_REGISTER_1, dtReal34, REAL34_SIZE, amNone); // make sure TEMP_REGISTER_1 is not of dtDate type here
        convertReal34RegisterToDateRegister(regist, TEMP_REGISTER_1);
        if(getRegisterDataType(TEMP_REGISTER_1) != dtDate) return false; // invalid date
        internalDateToJulianDay(REGISTER_REAL34_DATA(TEMP_REGISTER_1), jd);
        return true;
      }
      /* fallthrough */

    default :
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "data type %s cannot be converted to date!", getRegisterDataTypeName(REGISTER_X, false, false));
        moreInfoOnError("In function checkDateArgument:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return false;
  }
}

/********************************************//**
 * \brief Check for leap year
 *
 * \param[in] year real34_t*
 * \return bool_t true if leap year
 ***********************************************/
bool_t isLeapYear(const real34_t *year) {
  real34_t val, four_hundred, val2;
  int32_t y400; // year mod 400
  bool_t isGregorian;
  int32ToReal34(400, &four_hundred);

  int32ToReal34(2, &val), int32ToReal34(28, &val2), composeJulianDay(year, &val, &val2, &val);
  uInt32ToReal34(firstGregorianDay, &val2);
  isGregorian = real34CompareGreaterEqual(&val, &val2);

  real34Divide(year, &four_hundred, &val), real34ToIntegralValue(&val, &val, DEC_ROUND_FLOOR);
  real34Multiply(&val, &four_hundred, &val);
  real34Subtract(year, &val, &val);
  y400 = real34ToInt32(&val);

  if(y400 == 0) return true;
  else if(isGregorian && (y400 % 100 == 0)) return false;

  return (y400 % 4 == 0);
}


/********************************************//**
 * \brief Check if date is valid (e.g. 30 Feb is invalid)
 *
 * \param[in] year real34_t*
 * \param[in] month real34_t*
 * \param[in] day real34_t*
 * \return bool_t true if valid
 ***********************************************/
bool_t isValidDay(const real34_t *year, const real34_t *month, const real34_t *day) {
  real34_t val;

  // Year (this rejects year -4713 and earlier)
  real34ToIntegralValue(year, &val, DEC_ROUND_FLOOR), real34Subtract(year, &val, &val);
  if(!real34IsZero(&val)) return false;
  int32ToReal34(-4712, &val), real34Compare(year, &val, &val);
  if(real34ToInt32(&val) < 0) return false;

  // Day
  real34ToIntegralValue(day, &val, DEC_ROUND_FLOOR), real34Subtract(day, &val, &val);
  if(!real34IsZero(&val)) return false;
  int32ToReal34(1, &val), real34Compare(day, &val, &val);
  if(real34ToInt32(&val) < 0) return false;
  int32ToReal34(31, &val), real34Compare(day, &val, &val);
  if(real34ToInt32(&val) > 0) return false;

  // Month
  real34ToIntegralValue(month, &val, DEC_ROUND_FLOOR), real34Subtract(month, &val, &val);
  if(!real34IsZero(&val)) return false;
  int32ToReal34(1, &val), real34Compare(month, &val, &val);
  if(real34ToInt32(&val) < 0) return false;
  int32ToReal34(12, &val), real34Compare(month, &val, &val);
  if(real34ToInt32(&val) > 0) return false;

  // Thirty days hath September...
  if(real34ToInt32(day) == 31) {
    switch(real34ToInt32(month)) {
      case 2: case 4: case 6: case 9: case 11:
        return false;
      default:
        return true;
    }
  }

  // February
  if(real34ToInt32(month) == 2) {
    if(real34ToInt32(day) == 30) return false;
    else if((real34ToInt32(day) == 29) && (!isLeapYear(year))) return false;
  }

  // Check for Julian-Gregorian gap
  if(firstGregorianDay > 0u) { // Gregorian
    real34_t y, m, d;
    composeJulianDay(year, month, day, &val);
    decomposeJulianDay(&val, &y, &m, &d);
    if(!real34CompareEqual(year, &y) || !real34CompareEqual(month, &m) || !real34CompareEqual(day, &d)) return false;
  }

  // Valid date
  return true;
}


#define AddInt(p, q, r) int32ToReal34(q, &tmp), real34Add(p, &tmp, r)
#define SubInt(p, q, r) int32ToReal34(q, &tmp), real34Subtract(p, &tmp, r)
#define MulInt(p, q, r) int32ToReal34(q, &tmp), real34Multiply(p, &tmp, r)
#define DivInt(p, q, r) int32ToReal34(q, &tmp), real34Divide(p, &tmp, &tmp), real34ToIntegralValue(&tmp, r, DEC_ROUND_DOWN)

/********************************************//**
 * \brief Convert date into Julian day number floored
 *
 * \param[in] year real34_t*
 * \param[in] month real34_t*
 * \param[in] day real34_t*
 * \param[out] jd real34_t*
 * \return void
 ***********************************************/
void composeJulianDay(const real34_t *year, const real34_t *month, const real34_t *day, real34_t *jd) {
  real34_t fg, y, m, d;

  uInt32ToReal34(firstGregorianDay, &fg);
  real34Copy(year, &y); real34Copy(month, &m); real34Copy(day, &d);
  composeJulianDay_g(&y, &m, &d, jd);
  if((firstGregorianDay > 0u) && real34CompareLessThan(jd, &fg)) {
    composeJulianDay_j(&y, &m, &d, jd);
  }
}

// Gregorian
void composeJulianDay_g(const real34_t *year, const real34_t *month, const real34_t *day, real34_t *jd) {
  real34_t m_14_12; // round_down((month - 14) / 12)
  real34_t a, tmp;

  // round_down((month - 14) / 12)
  SubInt(month, 14, &m_14_12);
  DivInt(&m_14_12, 12, &m_14_12);

  AddInt(year, 4800, &a);
  real34Add(&a, &m_14_12, &a);
  MulInt(&a, 1461, &a);
  DivInt(&a, 4, jd);

  MulInt(&m_14_12, 12, &a);
  real34Subtract(month, &a, &a);
  SubInt(&a, 2, &a);
  MulInt(&a, 367, &a);
  DivInt(&a, 12, &a);
  real34Add(jd, &a, jd);

  AddInt(year, 4900, &a);
  real34Add(&a, &m_14_12, &a);
  DivInt(&a, 100, &a);
  MulInt(&a, 3, &a);
  DivInt(&a, 4, &a);
  real34Subtract(jd, &a, jd);

  real34Add(jd, day, jd);
  SubInt(jd, 32075, jd);
}

// Julian
void composeJulianDay_j(const real34_t *year, const real34_t *month, const real34_t *day, real34_t *jd) {
  real34_t a, tmp;

  MulInt(year, 367, jd);

  SubInt(month, 9, &a);
  DivInt(&a, 7, &a);
  real34Add(&a, year, &a);
  AddInt(&a, 5001, &a);
  MulInt(&a, 7, &a);
  DivInt(&a, 4, &a);
  real34Subtract(jd, &a, jd);

  MulInt(month, 275, &a);
  DivInt(&a, 9, &a);
  real34Add(jd, &a, jd);

  real34Add(jd, day, jd);
  AddInt(jd, 1729777, jd);
}

#undef DivInt
#define DivInt(p, q, r) int32ToReal34(q, &tmp), real34Divide(p, &tmp, &tmp), real34ToIntegralValue(&tmp, r, DEC_ROUND_FLOOR)
#define ModInt(p, q, r) DivInt(p, q, &modtmp), MulInt(&modtmp, q, &modtmp), real34Subtract(p, &modtmp, r)

/********************************************//**
 * \brief Convert Julian day number (floored) into date
 *
 * \param[in] jd real34_t*
 * \param[out] year real34_t*
 * \param[out] month real34_t*
 * \param[out] day real34_t*
 * \return void
 ***********************************************/
void decomposeJulianDay(const real34_t *jd, real34_t *year, real34_t *month, real34_t *day) {
  real34_t e, h, tmp1, tmp, modtmp;

  AddInt(jd, 1401, &e);
  uInt32ToReal34(firstGregorianDay, &tmp);
  // proleptic Gregorian calendar is used if firstGregorianDay == 0: for special purpose only!
  if((firstGregorianDay == 0u) || real34CompareGreaterEqual(jd, &tmp)) { // Gregorian
    MulInt(jd, 4, &tmp1);
    AddInt(&tmp1, 274277, &tmp1);
    DivInt(&tmp1, 146097, &tmp1);
    MulInt(&tmp1, 3, &tmp1);
    DivInt(&tmp1, 4, &tmp1);
    SubInt(&tmp1, 38, &tmp1);
    real34Add(&e, &tmp1, &e);
  }

  MulInt(&e, 4, &e);
  AddInt(&e, 3, &e);

  ModInt(&e, 1461, &h);
  DivInt(&h, 4, &h);
  MulInt(&h, 5, &h);
  AddInt(&h, 2, &h);

  ModInt(&h, 153, day);
  DivInt(day, 5, day);
  AddInt(day, 1, day);

  DivInt(&h, 153, month);
  AddInt(month, 2, month);
  ModInt(month, 12, month);
  AddInt(month, 1, month);

  DivInt(&e, 1461, year);
  SubInt(year, 4716, year);
  int32ToReal34(14, &tmp1), real34Subtract(&tmp1, month, &tmp1);
  DivInt(&tmp1, 12, &tmp1);
  real34Add(year, &tmp1, year);
}

/********************************************//**
 * \brief Get day of week from date in given register
 *
 * \param[in] regist calcRegister_t register
 * \return uint32_t day of week (1 = Monday, 7 = Sunday), 0 if invalid
 ***********************************************/
uint32_t getDayOfWeek(calcRegister_t regist) {
  real34_t date34, tmp, modtmp;
  if(checkDateArgument(regist, &date34)) {
    ModInt(&date34, 7, &date34);
    AddInt(&date34, 1, &date34);
    return real34ToUInt32(&date34);
  }
  else {
    return 0u;
  }
}
#undef ModInt
#undef AddInt
#undef SubInt
#undef MulInt
#undef DivInt

/********************************************//**
 * \brief Check date range
 *
 * \param[in] time34 real34_t*
 * \return void
 ***********************************************/
void checkDateRange(const real34_t *date34) {
  real34_t hundredgigayear;
  stringToReal34("3155695348699627200.000000000000000", &hundredgigayear);
  if(real34CompareGreaterEqual(date34, &hundredgigayear) || real34IsNegative(date34)) {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "value of date type is too large");
      moreInfoOnError("In function checkDateRange:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }
}


/********************************************//**
 * \brief Convert H.MMSS into seconds
 *
 * \param[in] src real34_t* H.MMSS-formatted time value (for input)
 * \param[out] dest real34_t* time value in seconds (internal representation)
 * \return void
 ***********************************************/
void hmmssToSeconds(const real34_t *src, real34_t *dest) {
  real34_t time34, real34, value34;
  int32_t sign;

  sign = real34IsNegative(src);

  // Hours
  real34CopyAbs(src, &time34);
  real34ToIntegralValue(&time34, &real34, DEC_ROUND_DOWN);
  int32ToReal34(3600, &value34);
  real34Multiply(&real34, &value34, dest);

  // Minutes
  real34Subtract(&time34, &real34, &time34);
  int32ToReal34(100, &value34);
  real34Multiply(&time34, &value34, &time34);
  real34ToIntegralValue(&time34, &real34, DEC_ROUND_DOWN);
  int32ToReal34(60, &value34);
  real34Multiply(&value34, &real34, &value34);
  real34Add(dest, &value34, dest);

  // Seconds
  real34Subtract(&time34, &real34, &time34);
  int32ToReal34(100, &value34);
  real34Multiply(&time34, &value34, &time34);
  real34Add(dest, &time34, dest);

  // Sign
  if(sign) {
    real34SetNegativeSign(dest);
  }
}

/********************************************//**
 * \brief Convert H.MMSS in given register into time
 *
 * \param[in] regist calcRegister_t register
 * \return void
 ***********************************************/
void hmmssInRegisterToSeconds(calcRegister_t regist) {
  real34_t real34;
  real34Copy(REGISTER_REAL34_DATA(regist), &real34);
  reallocateRegister(regist, dtTime, REAL34_SIZE, amNone);
  hmmssToSeconds(&real34, REGISTER_REAL34_DATA(regist));
  checkTimeRange(REGISTER_REAL34_DATA(regist));
}

/********************************************//**
 * \brief Check time range
 *
 * \param[in] time34 real34_t*
 * \return void
 ***********************************************/
void checkTimeRange(const real34_t *time34) {
  real34_t t, petahour;
  real34CopyAbs(time34, &t);
  stringToReal34("36000000000000000000.00000000000000", &petahour);
  if(real34CompareGreaterEqual(&t, &petahour)) {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "value of time type is too large");
      moreInfoOnError("In function checkTimeRange:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }
}



void fnJulianToDate(uint16_t unusedButMandatoryParameter) {
  real34_t date;

  if(!saveLastX()) return;

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      julianDayToInternalDate(REGISTER_REAL34_DATA(REGISTER_X), &date);
      reallocateRegister(REGISTER_X, dtDate, REAL34_SIZE, amNone);
      real34Copy(&date, REGISTER_REAL34_DATA(REGISTER_X));
      break;

    default :
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "data type %s cannot be converted to date!", getRegisterDataTypeName(REGISTER_X, false, false));
        moreInfoOnError("In function fnDateToJulian:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
  }

  // check range
  checkDateRange(REGISTER_REAL34_DATA(REGISTER_X));
  if(lastErrorCode != 0) undo();
}

void fnDateToJulian(uint16_t unusedButMandatoryParameter) {
  real34_t jd34;

  if(!saveLastX()) return;

  if(checkDateArgument(REGISTER_X, &jd34)) {
    convertReal34ToLongIntegerRegister(&jd34, REGISTER_X, DEC_ROUND_FLOOR);
  }
}

void fnIsLeap(uint16_t unusedButMandatoryParameter) {
  real34_t y, m, d, j;

  if(checkDateArgument(REGISTER_X, &j)) {
    decomposeJulianDay(&j, &y, &m, &d);
    temporaryInformation = (isLeapYear(&y) ? TI_TRUE : TI_FALSE);
  }
}

void fnSetFirstGregorianDay(uint16_t unusedButMandatoryParameter) {
  real34_t jd34;
  const uint32_t fgd = firstGregorianDay;

  if((getRegisterDataType(REGISTER_X) == dtReal34) && (getRegisterAngularMode(REGISTER_X) == amNone)) {
    firstGregorianDay = 0u; // proleptic Gregorian mode
    if(checkDateArgument(REGISTER_X, &jd34)) {
      firstGregorianDay = real34ToUInt32(&jd34);
    }
    else {
      firstGregorianDay = fgd;
    }
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      if(getRegisterDataType(REGISTER_X) == dtDate)
        sprintf(errorMessage, "data type %s is disabled as input because of complicated Julian-Gregorian issue!", getRegisterDataTypeName(REGISTER_X, false, false));
      else
        sprintf(errorMessage, "data type %s cannot be interpreted as a date!", getRegisterDataTypeName(REGISTER_X, false, false));
      moreInfoOnError("In function fnSetFirstGregorianDay:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}

void fnGetFirstGregorianDay(uint16_t unusedButMandatoryParameter) {
  real34_t j;

  uInt32ToReal34(firstGregorianDay, &j);
  liftStack();
  reallocateRegister(REGISTER_X, dtDate, REAL34_SIZE, amNone);
  julianDayToInternalDate(&j, REGISTER_REAL34_DATA(REGISTER_X));
}

void fnXToDate(uint16_t unusedButMandatoryParameter) {
  if(!saveLastX()) return;

  switch(getRegisterDataType(REGISTER_X)) {
    case dtDate:
      /* already in date: do nothing */
      break;

    case dtReal34:
      if(getRegisterAngularMode(REGISTER_X) == amNone) {
        convertReal34RegisterToDateRegister(REGISTER_X, REGISTER_X);
        checkDateRange(REGISTER_REAL34_DATA(REGISTER_X));
        if(lastErrorCode != 0) undo();
        break;
      }
      /* fallthrough */

    default :
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "data type %s cannot be converted to date!", getRegisterDataTypeName(REGISTER_X, false, false));
        moreInfoOnError("In function fnXToDate:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
  }
}


void fnYear(uint16_t unusedButMandatoryParameter) {
  real34_t y, m, d, j;

  if(!saveLastX()) return;

  if(checkDateArgument(REGISTER_X, &j)) {
    decomposeJulianDay(&j, &y, &m, &d);
    convertReal34ToLongIntegerRegister(&y, REGISTER_X, DEC_ROUND_FLOOR);
  }
}

void fnMonth(uint16_t unusedButMandatoryParameter) {
  real34_t y, m, d, j;

  if(!saveLastX()) return;

  if(checkDateArgument(REGISTER_X, &j)) {
    decomposeJulianDay(&j, &y, &m, &d);
    convertReal34ToLongIntegerRegister(&m, REGISTER_X, DEC_ROUND_FLOOR);
  }
}

void fnDay(uint16_t unusedButMandatoryParameter) {
  real34_t y, m, d, j;

  if(!saveLastX()) return;

  if(checkDateArgument(REGISTER_X, &j)) {
    decomposeJulianDay(&j, &y, &m, &d);
    convertReal34ToLongIntegerRegister(&d, REGISTER_X, DEC_ROUND_FLOOR);
  }
}

void fnWday(uint16_t unusedButMandatoryParameter) {
  const uint32_t dayOfWeek = getDayOfWeek(REGISTER_X);
  longInteger_t result;

  if(!saveLastX()) return;

  if(dayOfWeek != 0) {
    longIntegerInit(result);
    uIntToLongInteger(dayOfWeek, result);
    convertLongIntegerToLongIntegerRegister(result, REGISTER_X);
    longIntegerFree(result);
    temporaryInformation = TI_DAY_OF_WEEK;
  }
}

void fnDateTo(uint16_t unusedButMandatoryParameter) {
  real34_t y, m, d, j;

  if(!saveLastX()) return;

  if(checkDateArgument(REGISTER_X, &j)) {
    liftStack();
    liftStack();
    decomposeJulianDay(&j, &y, &m, &d);
    convertReal34ToLongIntegerRegister(&y, getSystemFlag(FLAG_YMD) ? REGISTER_Z :                                        REGISTER_X, DEC_ROUND_FLOOR);
    convertReal34ToLongIntegerRegister(&m, getSystemFlag(FLAG_MDY) ? REGISTER_Z :                                        REGISTER_Y, DEC_ROUND_FLOOR);
    convertReal34ToLongIntegerRegister(&d, getSystemFlag(FLAG_DMY) ? REGISTER_Z : getSystemFlag(FLAG_MDY) ? REGISTER_Y : REGISTER_X, DEC_ROUND_FLOOR);
  }
}

void fnToDate(uint16_t unusedButMandatoryParameter) {
  real34_t y, m, d, j;
  real34_t *part[3];
  calcRegister_t r[3] = {REGISTER_Z, REGISTER_Y, REGISTER_X};
  int32_t i;

  if(!saveLastX()) return;

  if(getSystemFlag(FLAG_DMY)) {
    part[0] = &d;
    part[1] = &m;
    part[2] = &y;
  }
  else if(getSystemFlag(FLAG_MDY)) {
    part[0] = &m;
    part[1] = &d;
    part[2] = &y;
  }
  else {
    part[0] = &y;
    part[1] = &m;
    part[2] = &d;
  }

  for(i = 0; i < 3; ++i) {
    switch(getRegisterDataType(r[i])) {
      case dtLongInteger:
        convertLongIntegerRegisterToReal34(r[i], part[i]);
        break;

      case dtReal34:
        if(getRegisterAngularMode(r[i]) == amNone) {
          real34ToIntegralValue(REGISTER_REAL34_DATA(r[i]), part[i], DEC_ROUND_DOWN);
          break;
        }
        /* fallthrough */

      default :
        displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "data type %s cannot be converted to a real34!", getRegisterDataTypeName(r[i], false, false));
          moreInfoOnError("In function fnToReal:", errorMessage, NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        return;
    }
  }

  if(!isValidDay(&y, &m, &d)) {
      displayCalcErrorMessage(ERROR_BAD_TIME_OR_DATE_INPUT, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function fnToDate:", "Invalid date input like 30 Feb.", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
  }

  // valid date
  fnDropY(NOPARAM);
  if(lastErrorCode == ERROR_NONE) {
    fnDropY(NOPARAM);
    if(lastErrorCode == ERROR_NONE) {
      composeJulianDay(&y, &m, &d, &j);
      reallocateRegister(REGISTER_X, dtDate, REAL34_SIZE, amNone);
      julianDayToInternalDate(&j, REGISTER_REAL34_DATA(REGISTER_X));

      // check range
      checkDateRange(REGISTER_REAL34_DATA(REGISTER_X));
    }
  }
  if(lastErrorCode != 0) undo();
}


void fnToHr(uint16_t unusedButMandatoryParameter) {
  if(!saveLastX()) return;

  switch(getRegisterDataType(REGISTER_X)) {
    case dtTime:
      convertTimeRegisterToReal34Register(REGISTER_X, REGISTER_X);
      break;

    default :
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "data type %s cannot be converted to a real34!", getRegisterDataTypeName(REGISTER_X, false, false));
        moreInfoOnError("In function fnToReal:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
  }
}

void fnToHms(uint16_t unusedButMandatoryParameter) {
  if(!saveLastX()) return;

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger :
      convertLongIntegerRegisterToTimeRegister(REGISTER_X, REGISTER_X);
      break;

    case dtTime:
      /* already in hours: do nothing */
      break;

    case dtReal34:
      if(getRegisterAngularMode(REGISTER_X) == amNone) {
        convertReal34RegisterToTimeRegister(REGISTER_X, REGISTER_X);
        break;
      }
      /* fallthrough */

    default :
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "data type %s cannot be converted to time!", getRegisterDataTypeName(REGISTER_X, false, false));
        moreInfoOnError("In function fnToReal:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
  }
  checkTimeRange(REGISTER_REAL34_DATA(REGISTER_X));
  if(lastErrorCode != 0) undo();
}


void fnDate(uint16_t unusedButMandatoryParameter) {
  real34_t y, m, d, j;

  #ifdef DMCP_BUILD
    tm_t timeInfo;
    dt_t dateInfo;

    rtc_read(&timeInfo, &dateInfo);
    uInt32ToReal34(dateInfo.year,  &y);
    uInt32ToReal34(dateInfo.month, &m);
    uInt32ToReal34(dateInfo.day,   &d);
  #else // !DMCP_BUILD
    time_t epoch = time(NULL);
    struct tm *timeInfo = localtime(&epoch);

    int32ToReal34(timeInfo->tm_year + 1900, &y);
    int32ToReal34(timeInfo->tm_mon  + 1,    &m);
    int32ToReal34(timeInfo->tm_mday,        &d);
  #endif // DMCP_BUILD

  composeJulianDay(&y, &m, &d, &j);
  liftStack();
  reallocateRegister(REGISTER_X, dtDate, REAL34_SIZE, amNone);
  julianDayToInternalDate(&j, REGISTER_REAL34_DATA(REGISTER_X));
  temporaryInformation = TI_DAY_OF_WEEK;
}

void fnTime(uint16_t unusedButMandatoryParameter) {
  real34_t time34;

  #ifdef DMCP_BUILD
    tm_t timeInfo;
    dt_t dateInfo;

    rtc_read(&timeInfo, &dateInfo);
    uInt32ToReal34((uint32_t)timeInfo.hour * 3600u + (uint32_t)timeInfo.min * 60u + (uint32_t)timeInfo.sec, &time34);
  #else // !DMCP_BUILD
    time_t epoch = time(NULL);
    struct tm *timeInfo = localtime(&epoch);

    uInt32ToReal34((uint32_t)timeInfo->tm_hour * 3600u + (uint32_t)timeInfo->tm_min * 60u + (uint32_t)timeInfo->tm_sec, &time34);
  #endif // DMCP_BUILD

  liftStack();
  reallocateRegister(REGISTER_X, dtTime, REAL34_SIZE, amNone);
  real34Copy(&time34, REGISTER_REAL34_DATA(REGISTER_X));
}


void fnSetDate(uint16_t unusedButMandatoryParameter) {
  #ifdef DMCP_BUILD
    tm_t timeInfo;
    dt_t dateInfo;
    real34_t j, y, m, d;

    if(checkDateArgument(REGISTER_X, &j)) {
      rtc_read(&timeInfo, &dateInfo);
      decomposeJulianDay(&j, &y, &m, &d);
      dateInfo.year  = real34ToUInt32(&y);
      dateInfo.month = real34ToUInt32(&m);
      dateInfo.day   = real34ToUInt32(&d);
      rtc_write(&timeInfo, &dateInfo);
    }
  #else // !DMCP_BUILD
    displayCalcErrorMessage(ERROR_FUNCTION_NOT_FOUND, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "real calculator only!");
      moreInfoOnError("In function fnSetDate:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  #endif // DMCP_BUILD
}

void fnSetTime(uint16_t unusedButMandatoryParameter) {
  #ifdef DMCP_BUILD
    tm_t timeInfo;
    dt_t dateInfo;
    real34_t time34, value34;
    int32_t timeVal;

    if(getRegisterDataType(REGISTER_X) == dtTime) {
      int32ToReal34(86400, &value34);
      if(real34IsNegative(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || real34CompareGreaterEqual(REGISTER_REAL34_DATA(REGISTER_X), &value34)) {
        displayCalcErrorMessage(ERROR_BAD_TIME_OR_DATE_INPUT, ERR_REGISTER_LINE, REGISTER_X);
      }
      else {
        rtc_read(&timeInfo, &dateInfo);
        int32ToReal34(100, &value34);
        real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), &value34, &time34);
        real34ToIntegralValue(&time34, &time34, DEC_ROUND_DOWN);
        timeVal = real34ToInt32(&time34);
        timeInfo.csec =  timeVal         % 100;
        timeInfo.sec  = (timeVal /= 100) %  60;
        timeInfo.min  = (timeVal /=  60) %  60;
        timeInfo.hour = (timeVal /=  60);
        rtc_write(&timeInfo, &dateInfo);
      }
    }
    else {
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    }
  #else // !DMCP_BUILD
    displayCalcErrorMessage(ERROR_FUNCTION_NOT_FOUND, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "real calculator only!");
      moreInfoOnError("In function fnSetTime:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  #endif // DMCP_BUILD
}


/********************************************//**
 * \brief Gets the system date
 *
 * \param[out] dateString char* String receiving the system date.  Must be at least 11 bytes long (yyyy-mm-dd plus terminating 0)
 * \return void
 ***********************************************/
void getDateString(char *dateString) {
  #ifdef DMCP_BUILD
    tm_t timeInfo;
    dt_t dateInfo;

    rtc_read(&timeInfo, &dateInfo);
    if(!getSystemFlag(FLAG_TDM24)) { // 2 digit year
      if(getSystemFlag(FLAG_DMY)) {
        sprintf(dateString, "%02d.%02d.%02d", dateInfo.day, dateInfo.month, dateInfo.year % 100);
      }
      else if(getSystemFlag(FLAG_YMD)) {
        sprintf(dateString, "%02d-%02d-%02d", dateInfo.year % 100, dateInfo.month, dateInfo.day);
      }
      else if(getSystemFlag(FLAG_MDY)) {
        sprintf(dateString, "%02d/%02d/%02d", dateInfo.month, dateInfo.day, dateInfo.year % 100);
      }
      else {
        strcpy(dateString, "?? ?? ????");
      }
    }
    else {// 4 digit year
      if(getSystemFlag(FLAG_DMY)) {
        sprintf(dateString, "%02d.%02d.%04d", dateInfo.day, dateInfo.month, dateInfo.year);
      }
      else if(getSystemFlag(FLAG_YMD)) {
        sprintf(dateString, "%04d-%02d-%02d", dateInfo.year, dateInfo.month, dateInfo.day);
      }
      else if(getSystemFlag(FLAG_MDY)) {
        sprintf(dateString, "%02d/%02d/%04d", dateInfo.month, dateInfo.day, dateInfo.year);
      }
      else {
        strcpy(dateString, "?? ?? ????");
      }
    }
  #else // !DMCP_BUILD
    time_t epoch = time(NULL);
    struct tm *timeInfo = localtime(&epoch);

    // For the format string : man strftime
    if(!getSystemFlag(FLAG_TDM24)) { // time format = 12H ==> 2 digit year
      if(getSystemFlag(FLAG_DMY)) {
        strftime(dateString, 11, "%d.%m.%y", timeInfo);
      }
      else if(getSystemFlag(FLAG_YMD)) {
        strftime(dateString, 11, "%y-%m-%d", timeInfo);
      }
      else if(getSystemFlag(FLAG_MDY)) {
        strftime(dateString, 11, "%m/%d/%y", timeInfo);
      }
      else {
        strcpy(dateString, "?? ?? ????");
      }
    }
    else {// 4 digit year
      if(getSystemFlag(FLAG_DMY)) {
        strftime(dateString, 11, "%d.%m.%Y", timeInfo);
      }
      else if(getSystemFlag(FLAG_YMD)) {
        strftime(dateString, 11, "%Y-%m-%d", timeInfo);
      }
      else if(getSystemFlag(FLAG_MDY)) {
        strftime(dateString, 11, "%m/%d/%Y", timeInfo);
      }
      else {
        strcpy(dateString, "?? ?? ????");
      }
    }
  #endif // DMCP_BUILD
}



/********************************************//**
 * \brief Gets the system time
 *
 * \param[out] timeString char* String receiving the system time. Must be at least 8 bytes long (03:15pm plus terminating 0)
 * \return void
 ***********************************************/
void getTimeString(char *timeString) {
  #ifdef DMCP_BUILD
    tm_t timeInfo;
    dt_t dateInfo;

    rtc_read(&timeInfo, &dateInfo);
    if(!getSystemFlag(FLAG_TDM24)) {
      sprintf(timeString, "%02d:%02d", (timeInfo.hour % 12) == 0 ? 12 : timeInfo.hour % 12, timeInfo.min);
      if(timeInfo.hour >= 12) {
        strcat(timeString, "pm");
      }
      else {
        strcat(timeString, "am");
      }
    }
    else {
      sprintf(timeString, "%02d:%02d", timeInfo.hour, timeInfo.min);
    }
  #else // !DMCP_BUILD
    time_t epoch = time(NULL);
    struct tm *timeInfo = localtime(&epoch);

    // For the format string : man strftime
    if(getSystemFlag(FLAG_TDM24)) { // time format = 24H
      strftime(timeString, 8, "%H:%M", timeInfo); // %R don't work on Windows
    }
    else {
      strftime(timeString, 8, "%I:%M", timeInfo); // I could use %p but in many locals the AM and PM string are empty
      if(timeInfo->tm_hour >= 12) {
        strcat(timeString, "pm");
      }
      else {
        strcat(timeString, "am");
      }
    }
  #endif // DMCP_BUILD
}
