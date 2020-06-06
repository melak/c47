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
    oneSystemFlag(FLAG_ALLENG,  "ALLENG",         &line, &firstFlag);
    oneSystemFlag(FLAG_ALPIN,   "ALP.IN",         &line, &firstFlag);
    oneSystemFlag(FLAG_AUTOFF,  "AUTOFF",         &line, &firstFlag);
    oneSystemFlag(FLAG_AUTXEQ,  "AUTXEQ",         &line, &firstFlag);
    oneSystemFlag(FLAG_CPXj,    "CPXj",           &line, &firstFlag);
    oneSystemFlag(FLAG_FRACT,   "FRACT",          &line, &firstFlag);
    oneSystemFlag(FLAG_GROW,    "GROW",           &line, &firstFlag);
    oneSystemFlag(FLAG_IGN1ER,  "IGN1ER",         &line, &firstFlag);
    oneSystemFlag(FLAG_INTING,  "INTING",         &line, &firstFlag);
    oneSystemFlag(FLAG_LEAD0,   "LEAD.0",         &line, &firstFlag);
    oneSystemFlag(FLAG_NUMIN,   "NUM.IN",         &line, &firstFlag);
    oneSystemFlag(FLAG_PRTACT,  "PRTACT",         &line, &firstFlag);
    oneSystemFlag(FLAG_QUIET,   "QUIET",          &line, &firstFlag);
    oneSystemFlag(FLAG_SLOW,    "SLOW",           &line, &firstFlag);
    oneSystemFlag(FLAG_SOLVING, "SOLVING",        &line, &firstFlag);
    oneSystemFlag(FLAG_SPCRES,  "SPCRES",         &line, &firstFlag);
    oneSystemFlag(FLAG_SSIZE8,  "SSIZE8",         &line, &firstFlag);
    oneSystemFlag(FLAG_TRACE,   "TRACE",          &line, &firstFlag);
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
      currentFlgScr = 2;
    }
  }

  if(currentFlgScr == 3) {        //JMvv
    currentFlgScr = 0;
    calcMode = previousCalcMode;  //JM needed to preserve the previous state, only when jumping from the old to new STATUS, and vice versa
    flagBrowser_old(0);           //JM^^
  }
}
#endif




#ifndef TESTSUITE_BUILD
/********************************************//**
 * \brief The flag browser application
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void flagBrowser_old(uint16_t unusedParamButMandatory) {           //Returned from last old version JM
  int16_t f, x, y, i;

  if(calcMode != CM_FLAG_BROWSER_OLD) {
    previousCalcMode = calcMode;
    calcMode = CM_FLAG_BROWSER_OLD;
    clearSystemFlag(FLAG_ALPHA);          //JM
    currentFlgScr = 0;                    //JM
  }

  if(currentFlgScr == 0) { // Init
    currentFlgScr = 1;
  }

  if(currentFlgScr == 1) { // Memory and flags from 0 to 79
    clearScreen(false, true, true);

//JM    sprintf(tmpStr3000, "%" FMT32U " words free in RAM, %" FMT32U " in flash.", getFreeRamMemory() / 2, getFreeFlash() / 2);
//JM    showString(tmpStr3000, &standardFont, 1, 22-1, vmNormal, true, true);
//JM    showString("Global flag status:", &standardFont, 1, 44-1, vmNormal, true, true);

    for(f=0; f<=99/*79*/; f++) {                                          //JM 99
      if(getFlag(f)) {
        for(x=40*(f%10)+1; x<40*(f%10)+39; x++) {
          for(y=22*(f/10)+66-1-44; y<22*(f/10)+66+20-1-44; y++) {         //JM-44
            setPixel(x, y);
          }
        }
      }

      sprintf(tmpStr3000, "%d", f);
      showString(tmpStr3000, &standardFont, 40*(f%10) + 19 - stringWidth(tmpStr3000, &standardFont, false, false)/2, 22*(f/10)+66-1-44, getFlag(f) ? vmReverse : vmNormal, true, true); //JM-44
    }
  }

  if(currentFlgScr == 2) { // Flags from 100 to GLOBALFLAGS, local registers and local flags
    clearScreen(false, true, true);

    showString("Global flag status (continued):", &standardFont, 1, 22-1, vmNormal, true, true);

    for(f=100/*80*/; f<NUMBER_OF_GLOBAL_FLAGS; f++) {                     //JM100
      if(getFlag(f)) {
        for(x=80*(f%5); x<80*(f%5)+74; x++) {
          for(y=22*(f/5)-132-1-44-220; y<22*(f/5)-132+20-1-44-220; y++) {       //JM-44
           setPixel(x, y);
          }
        }
      }

      switch(f) {
      	case 100: strcpy(tmpStr3000, " POLAR.X"); break;
      	case 101: strcpy(tmpStr3000, "   101.Y"); break;
      	case 102: strcpy(tmpStr3000, "   102.Z"); break;
      	case 103: strcpy(tmpStr3000, " TRACE.T"); break;
      	case 104: strcpy(tmpStr3000, "ALLENG.A"); break;
      	case 105: strcpy(tmpStr3000, " OVRFL.B"); break;
      	case 106: strcpy(tmpStr3000, " CARRY.C"); break;
      	case 107: strcpy(tmpStr3000, "SPCRES.D"); break;
      	case 108: strcpy(tmpStr3000, " LEAD0.L"); break;
      	case 109: strcpy(tmpStr3000, "CPXRES.I"); break;
      	case 110: strcpy(tmpStr3000, "   110.J"); break;
      	case 111: strcpy(tmpStr3000, "   111.K"); break;
      	default:  sprintf(tmpStr3000,"   %d ", f);break;
      }

      char ss[2];
      i=0;
      ss[1]=0;
      while(tmpStr3000[i]!=0){
        ss[0]=tmpStr3000[i];
        showString(ss, &standardFont, i*9-32+1+max(0,16-1+2*40*(f%5) + 19 - 16/8), 22*(f/5)-132-1-44-220, getFlag(f) ? vmReverse : vmNormal, true, true);  //JM-44
        i++;
      }
//      showString(tmpStr3000, &standardFont, max(0,16-1+2*40*(f%5) + 19 - stringWidth(tmpStr3000, &standardFont, false, false)/2), 22*(f/5)-132-1-44-220, getFlag(f) ? vmReverse : vmNormal, true, true);  //JM-44
    }

    if(allLocalRegisterPointer->numberOfLocalRegisters != 0) {
      sprintf(tmpStr3000, "%" FMT16U " local register%s allocated.", allLocalRegisterPointer->numberOfLocalRegisters, allLocalRegisterPointer->numberOfLocalRegisters==1 ? " is" : "s are");
      showString(tmpStr3000, &standardFont, 1, 132-1, vmNormal, true, true);
      showString("Local flag status:", &standardFont, 1, 154-1, vmNormal, true, true);

      for(f=0; f<16; f++) {
        if(getFlag(NUMBER_OF_GLOBAL_FLAGS+f)) {
          for(x=40*(f%10)+1; x<40*(f%10)+39; x++) {
            for(y=22*(f/10)+176-1-44; y<22*(f/10)+176+20-1-44; y++) {                 //JM-44
             setPixel(x, y);
            }
          }
        }

        sprintf(tmpStr3000, "%d", f);
        showString(tmpStr3000, &standardFont, f<=9 ? 40*(f%10) + 17 : 40*(f%10) + 12, 22*(f/10)+176-1-44, getFlag(NUMBER_OF_GLOBAL_FLAGS+f) ? vmReverse : vmNormal, true, true);     //JM-44
      }
    }
  }

  if(currentFlgScr == 3) { // Change over to STATUS
  currentFlgScr = 0;
  calcMode = previousCalcMode;  //JM needed to preserve the previous state, only when jumping from the old to new STATUS, and vice versa
  flagBrowser(0);
  }
}
#endif

