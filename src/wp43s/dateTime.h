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

void fnSetDateFormat         (uint16_t dateFormat);
void hmmssToSeconds          (const real34_t *src, real34_t *dest);
void hmmssInRegisterToSeconds(calcRegister_t regist);
void fnToHr                  (uint16_t unusedButMandatoryParameter);
void fnToHms                 (uint16_t unusedButMandatoryParameter);
void fnTime                  (uint16_t unusedButMandatoryParameter);
void fnSetTime               (uint16_t unusedButMandatoryParameter);
void getDateString           (char *dateString);
void getTimeString           (char *timeString);
void _gettimeofday           (void);
