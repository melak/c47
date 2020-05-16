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
 * \brief Gets the system date
 *
 * \param[out] dateString char* String receiving the system date.  Must be at least 11 bytes long (yyyy-mm-dd plus terminating 0)
 * \return void
 ***********************************************/
void getDateString(char *dateString) {
  #ifdef PC_BUILD
    time_t rawTime;
    struct tm *timeInfo;

    time(&rawTime);
    timeInfo = localtime(&rawTime);

    // For the format string : man strftime
    if(!getSystemFlag(FLAG_TDM)) { // time format = 12H ==> 2 digit year
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
  #endif

  #ifdef DMCP_BUILD
    tm_t timeInfo;
    dt_t dateInfo;

    rtc_read(&timeInfo, &dateInfo);
    if(!getSystemFlag(FLAG_TDM)) { // 2 digit year
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
  #endif
}



/********************************************//**
 * \brief Gets the system time
 *
 * \param[out] timeString char* String receiving the system time. Must be at least 8 bytes long (03:15pm plus terminating 0)
 * \return void
 ***********************************************/
void getTimeString(char *timeString) {
  #ifdef PC_BUILD
    time_t rawTime;
    struct tm *timeInfo;

    time(&rawTime);
    timeInfo = localtime(&rawTime);

    // For the format string : man strftime
    if(getSystemFlag(FLAG_TDM)) { // time format = 24H
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
  #endif

  #ifdef DMCP_BUILD
    tm_t timeInfo;
    dt_t dateInfo;

    rtc_read(&timeInfo, &dateInfo);
    if(!getSystemFlag(FLAG_TDM)) {
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
  #endif
}
