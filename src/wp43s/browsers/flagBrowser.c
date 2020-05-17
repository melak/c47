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
 * \file flagBrowser.c
 ***********************************************/

#include "wp43s.h"



#ifndef TESTSUITE_BUILD
#define CHARS_PER_LINE 80
static void oneSystemFlag(uint16_t systemFlag, const char *systemFlagNamename, int16_t *line, bool_t *firstSystemFlag) {
  if(getSystemFlag(systemFlag)) {
    if(stringWidth(tmpStr3000 + CHARS_PER_LINE * *line, &standardFont, true, true) + stringWidth(systemFlagNamename, &standardFont, true, false) <= SCREEN_WIDTH - 1 - 8) { // STD_SPACE_FIGURE is 8 pixel wide
      if(!*firstSystemFlag) {
        strcat(tmpStr3000 + CHARS_PER_LINE * *line, STD_SPACE_FIGURE);
      }
      else {
        *firstSystemFlag = false;
      }
      strcat(tmpStr3000 + CHARS_PER_LINE * *line, systemFlagNamename);
    }
    else {
      xcopy(tmpStr3000 + CHARS_PER_LINE * ++(*line), systemFlagNamename, strlen(systemFlagNamename) + 1);
    }
  }
}



/********************************************//**
 * \brief The flag browser application
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void flagBrowser(uint16_t unusedParamButMandatory) {
  static int16_t line;
  int16_t f;
  char flagNumber[4];
  bool_t firstFlag;

  if(calcMode != CM_FLAG_BROWSER) {
    previousCalcMode = calcMode;
    calcMode = CM_FLAG_BROWSER;
    clearSystemFlag(FLAG_ALPHA);
    currentFlgScr = 0;
  }

  if(currentFlgScr == 0) { // Init
    currentFlgScr = 1;

    line = 0;

    // Free memory
    sprintf(tmpStr3000 + CHARS_PER_LINE * line++, "%" FMT32U " bytes free in RAM, %" FMT32U " in flash.", getFreeRamMemory(), getFreeFlash());

    // Global flags
    sprintf(tmpStr3000 + CHARS_PER_LINE * line++, "Global user flags set:");

    tmpStr3000[CHARS_PER_LINE * line] = 0;
    firstFlag = true;
    for(f=0; f<NUMBER_OF_GLOBAL_FLAGS; f++) {
      if(getFlag(f)) {
        if(f < 10) {
          flagNumber[0] = '0' + f;
          flagNumber[1] = 0;
        }
        else if(f < 100) {
          flagNumber[0] = '0' + f/10;
          flagNumber[1] = '0' + f%10;
          flagNumber[2] = 0;
        }
        else {
          flagNumber[0] = '1';
          flagNumber[1] = '0' + (f-100)/10;
          flagNumber[2] = '0' + (f-100)%10;
          flagNumber[3] = 0;
        }

        if(stringWidth(tmpStr3000 + CHARS_PER_LINE * line, &standardFont, true, true) + stringWidth(flagNumber, &standardFont, true, false) <= SCREEN_WIDTH - 1 - 8) { // STD_SPACE_FIGURE is 8 pixel wide
          if(!firstFlag) {
            strcat(tmpStr3000 + CHARS_PER_LINE * line, STD_SPACE_FIGURE);
          }
          else {
            firstFlag = false;
          }
          strcat(tmpStr3000 + CHARS_PER_LINE * line, flagNumber);
        }
        else {
          xcopy(tmpStr3000 + CHARS_PER_LINE * ++line, flagNumber, 4);
        }
      }
    }

    if(allLocalRegisterPointer->numberOfLocalRegisters > 0) {
      // Local registers
      if(allLocalRegisterPointer->numberOfLocalRegisters == 1) {
        strcpy(tmpStr3000 + CHARS_PER_LINE * ++line, "1 local register is allocated.");
      }
      else {
        sprintf(tmpStr3000 + CHARS_PER_LINE * ++line, "%" FMT16U " local registers are allocated.", allLocalRegisterPointer->numberOfLocalRegisters);
      }

      // Local flags
      tmpStr3000[CHARS_PER_LINE * ++line] = 0;
      firstFlag = true;
      for(f=0; f<NUMBER_OF_LOCAL_FLAGS; f++) {
        if(getFlag(NUMBER_OF_GLOBAL_FLAGS + f)) {
          if(f < 10) {
            flagNumber[0] = '0' + f;
            flagNumber[1] = 0;
          }
          else {
            flagNumber[0] = '0' + f/10;
            flagNumber[1] = '0' + f%10;
            flagNumber[2] = 0;
          }

          if(stringWidth(tmpStr3000 + CHARS_PER_LINE * line, &standardFont, true, true) + stringWidth(flagNumber, &standardFont, true, false) <= SCREEN_WIDTH - 1 - 8) { // STD_SPACE_FIGURE is 8 pixel wide
            if(!firstFlag) {
              strcat(tmpStr3000 + CHARS_PER_LINE * line, STD_SPACE_FIGURE);
            }
            else {
              firstFlag = false;
            }
            strcat(tmpStr3000 + CHARS_PER_LINE * line, flagNumber);
          }
          else {
            xcopy(tmpStr3000 + CHARS_PER_LINE * ++line, flagNumber, 4);
          }
        }
      }
    }
    else {
      sprintf(tmpStr3000 + CHARS_PER_LINE * ++line, "No local registers are allocated.");
    }

    // Empty line
    tmpStr3000[CHARS_PER_LINE * ++line] = 0;

    // Rounding mode
    strcpy(tmpStr3000 + CHARS_PER_LINE * ++line, "RM=");
    switch(roundingMode) {
      case RM_HALF_EVEN: strcat(tmpStr3000 + CHARS_PER_LINE * line, STD_ONE_HALF "E");
                         break;

      case RM_HALF_UP:   strcat(tmpStr3000 + CHARS_PER_LINE * line, STD_ONE_HALF STD_UP_ARROW);
                         break;

      case RM_HALF_DOWN: strcat(tmpStr3000 + CHARS_PER_LINE * line, STD_ONE_HALF STD_DOWN_ARROW);
                         break;

      case RM_UP:        strcat(tmpStr3000 + CHARS_PER_LINE * line, STD_LEFT_ARROW "0" STD_RIGHT_ARROW);
                         break;

      case RM_DOWN:      strcat(tmpStr3000 + CHARS_PER_LINE * line, STD_RIGHT_ARROW "0" STD_LEFT_ARROW);
                         break;

      case RM_CEIL:      strcat(tmpStr3000 + CHARS_PER_LINE * line, STD_LEFT_CEILING "x" STD_RIGHT_CEILING);
                         break;

      case RM_FLOOR:     strcat(tmpStr3000 + CHARS_PER_LINE * line, STD_LEFT_FLOOR "x" STD_RIGHT_FLOOR);
                         break;

      default:           strcat(tmpStr3000 + CHARS_PER_LINE * line, "???");
    }

    // Significant digits
    strcat(tmpStr3000 + CHARS_PER_LINE * line, "  SDIGS=");
    uint8_t sd = (significantDigits == 0 ? 34 : significantDigits);
    if(sd < 10) {
      flagNumber[0] = '0' + sd;
      flagNumber[1] = 0;
    }
    else {
      flagNumber[0] = '0' + sd/10;
      flagNumber[1] = '0' + sd%10;
      flagNumber[2] = 0;
    }
    strcat(tmpStr3000 + CHARS_PER_LINE * line, flagNumber);

    // ULP of X
    switch(getRegisterDataType(REGISTER_X)) {
      case dtLongInteger:
      case dtShortInteger: strcat(tmpStr3000 + CHARS_PER_LINE * line, "  ULP of reg X = 1");
                           break;

      case dtReal34:       if(real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X))) {
                             strcat(tmpStr3000 + CHARS_PER_LINE * line, "  ULP of reg X = " STD_INFINITY);
                           }
                           else {
                             real34_t x34;
                             real34NextPlus(REGISTER_REAL34_DATA(REGISTER_X), &x34);
                             if(real34IsInfinite(&x34)) {
                               real34NextMinus(REGISTER_REAL34_DATA(REGISTER_X), &x34);
                               real34Subtract(REGISTER_REAL34_DATA(REGISTER_X), &x34, &x34);
                             }
                             else {
                               real34Subtract(&x34, REGISTER_REAL34_DATA(REGISTER_X), &x34);
                             }
                             strcat(tmpStr3000 + CHARS_PER_LINE * line, "  ULP of reg X = 10");
                             supNumberToDisplayString(real34GetExponent(&x34), tmpStr3000 + CHARS_PER_LINE * line + strlen(tmpStr3000 + CHARS_PER_LINE * line), NULL, false, NULL);
                           }
                           break;

      default:             {}
    }

    // System flags
    firstFlag = true;
    tmpStr3000[CHARS_PER_LINE * ++line] = 0;
    oneSystemFlag(FLAG_CPXj,    "CPXj",           &line, &firstFlag);
    oneSystemFlag(FLAG_FRACT,   "FRACT",          &line, &firstFlag);
    oneSystemFlag(FLAG_LEAD0,   "LEAD.0",         &line, &firstFlag);
    oneSystemFlag(FLAG_TRACE,   "TRACE",          &line, &firstFlag);
    oneSystemFlag(FLAG_SLOW,    "SLOW",           &line, &firstFlag);
    oneSystemFlag(FLAG_SPCRES,  "SPCRES",         &line, &firstFlag);
    oneSystemFlag(FLAG_SSIZE8,  "SSIZE8",         &line, &firstFlag);
    oneSystemFlag(FLAG_QUIET,   "QUIET",          &line, &firstFlag);
    oneSystemFlag(FLAG_DECIMP,  "DECIM.",         &line, &firstFlag);
    oneSystemFlag(FLAG_MULTx,   "MULT" STD_CROSS, &line, &firstFlag);
    oneSystemFlag(FLAG_ALLSCI,  "ALLSCI",         &line, &firstFlag);
    oneSystemFlag(FLAG_GROW,    "GROW",           &line, &firstFlag);
    oneSystemFlag(FLAG_AUTOFF,  "AUTOFF",         &line, &firstFlag);
    oneSystemFlag(FLAG_AUTXEQ,  "AUTXEQ",         &line, &firstFlag);
    oneSystemFlag(FLAG_PRTACT,  "PRTACT",         &line, &firstFlag);
    oneSystemFlag(FLAG_NUMIN,   "NUM.IN",         &line, &firstFlag);
    oneSystemFlag(FLAG_ALPIN,   "ALP.IN",         &line, &firstFlag);
    oneSystemFlag(FLAG_ASLIFT,  "ASLIFT",         &line, &firstFlag);
    oneSystemFlag(FLAG_IGN1ER,  "IGN1ER",         &line, &firstFlag);
    oneSystemFlag(FLAG_INTING,  "INTING",         &line, &firstFlag);
    oneSystemFlag(FLAG_SOLVING, "SOLVING",        &line, &firstFlag);
    oneSystemFlag(FLAG_VMDISP,  "VMDISP",         &line, &firstFlag);
    line++;
  }


  if(currentFlgScr == 1) {
    clearScreen(false, true, true);

    for(f=0; f<min(9, line); f++) {
      showString(tmpStr3000 + CHARS_PER_LINE * f, &standardFont, 1, 22*f + 43, vmNormal, true, false);
    }
  }

  if(currentFlgScr == 2) {
    if(line > 9) {
      clearScreen(false, true, true);

      for(f=9; f<line; f++) {
        showString(tmpStr3000 + CHARS_PER_LINE * f, &standardFont, 1, 22*(f-9) + 43, vmNormal, true, false);
      }
    }
    else {
      currentFlgScr = 1;
    }
  }
}
#endif
