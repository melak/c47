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
 * \file dateTime.h
 ***********************************************/
#ifndef DATETIME_H
#define DATETIME_H

#include "realType.h"
#include "typeDefinitions.h"
#include <stdint.h>

void fnSetDateFormat         (uint16_t dateFormat);
void internalDateToJulianDay (const real34_t *source, real34_t *destination);
void julianDayToInternalDate (const real34_t *source, real34_t *destination);
bool_t checkDateArgument     (calcRegister_t regist, real34_t *jd);
bool_t isLeapYear            (const real34_t *year);
bool_t isValidDay            (const real34_t *year, const real34_t *month, const real34_t *day);
void composeJulianDay        (const real34_t *year, const real34_t *month, const real34_t *day, real34_t *jd);
void composeJulianDay_g      (const real34_t *year, const real34_t *month, const real34_t *day, real34_t *jd);
void composeJulianDay_j      (const real34_t *year, const real34_t *month, const real34_t *day, real34_t *jd);
void decomposeJulianDay      (const real34_t *jd, real34_t *year, real34_t *month, real34_t *day);
uint32_t getDayOfWeek        (calcRegister_t regist);
void checkDateRange          (const real34_t *date34);
void hmmssToSeconds          (const real34_t *src, real34_t *dest);
void hmmssInRegisterToSeconds(calcRegister_t regist);
void checkTimeRange          (const real34_t *time34);
void fnJulianToDate          (uint16_t unusedButMandatoryParameter);
void fnDateToJulian          (uint16_t unusedButMandatoryParameter);
void fnIsLeap                (uint16_t unusedButMandatoryParameter);
void fnSetFirstGregorianDay  (uint16_t unusedButMandatoryParameter);
void fnGetFirstGregorianDay  (uint16_t unusedButMandatoryParameter);
void fnXToDate               (uint16_t unusedButMandatoryParameter);
void fnYear                  (uint16_t unusedButMandatoryParameter);
void fnMonth                 (uint16_t unusedButMandatoryParameter);
void fnDay                   (uint16_t unusedButMandatoryParameter);
void fnWday                  (uint16_t unusedButMandatoryParameter);
void fnDateTo                (uint16_t unusedButMandatoryParameter);
void fnToDate                (uint16_t unusedButMandatoryParameter);
void fnToHr                  (uint16_t unusedButMandatoryParameter);
void fnToHms                 (uint16_t unusedButMandatoryParameter);
void fnDate                  (uint16_t unusedButMandatoryParameter);
void fnTime                  (uint16_t unusedButMandatoryParameter);
void fnSetDate               (uint16_t unusedButMandatoryParameter);
void fnSetTime               (uint16_t unusedButMandatoryParameter);
void getDateString           (char *dateString);
void getTimeString           (char *timeString);
void _gettimeofday           (void);

#endif // DATETIME_H
