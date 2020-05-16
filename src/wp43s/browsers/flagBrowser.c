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
/********************************************//**
 * \brief The flag browser application
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void flagBrowser(uint16_t unusedParamButMandatory) {
  int16_t f, x, y;

  if(calcMode != CM_FLAG_BROWSER) {
    previousCalcMode = calcMode;
    calcMode = CM_FLAG_BROWSER;
    clearSystemFlag(FLAG_ALPHA);
  }

  if(currentFlgScr == 0) { // Init
    currentFlgScr = 1;
  }

  if(currentFlgScr == 1) { // Memory and flags from 0 to 79
    clearScreen(false, true, true);

    sprintf(tmpStr3000, "%" FMT32U " words free in RAM, %" FMT32U " in flash.", getFreeRamMemory() / 2, getFreeFlash() / 2);
    showString(tmpStr3000, &standardFont, 1, 22-1, vmNormal, true, true);
    showString("Global flag status:", &standardFont, 1, 44-1, vmNormal, true, true);

    for(f=0; f<=79; f++) {
      if(getFlag(f)) {
        for(x=40*(f%10)+1; x<40*(f%10)+39; x++) {
          for(y=22*(f/10)+66-1; y<22*(f/10)+66+20-1; y++) {
            setPixel(x, y);
          }
        }
      }

      sprintf(tmpStr3000, "%d", f);
      showString(tmpStr3000, &standardFont, 40*(f%10) + 19 - stringWidth(tmpStr3000, &standardFont, false, false)/2, 22*(f/10)+66-1, getFlag(f) ? vmReverse : vmNormal, true, true);
    }
  }

  if(currentFlgScr == 2) { // Flags from 80 to GLOBALFLAGS, local registers and local flags
    clearScreen(false, true, true);

    showString("Global flag status (continued):", &standardFont, 1, 22-1, vmNormal, true, true);

    for(f=80; f<NUMBER_OF_GLOBAL_FLAGS; f++) {
      if(getFlag(f)) {
        for(x=40*(f%10)+1; x<40*(f%10)+39; x++) {
          for(y=22*(f/10)-132-1; y<22*(f/10)-132+20-1; y++) {
           setPixel(x, y);
          }
        }
      }

      if(f == 103) {
        strcpy(tmpStr3000, "103t");
      }
      else if(f == 105) {
        strcpy(tmpStr3000, "105o");
      }
      else if(f == 106) {
        strcpy(tmpStr3000, "106c");
      }
      else if(f == 107) {
        strcpy(tmpStr3000, "107d");
      }
      else if(f == 109) {
        strcpy(tmpStr3000, "109i");
      }
      else {
        sprintf(tmpStr3000, "%d", f);
      }

      showString(tmpStr3000, &standardFont, 40*(f%10) + 19 - stringWidth(tmpStr3000, &standardFont, false, false)/2, 22*(f/10)-132-1, getFlag(f) ? vmReverse : vmNormal, true, true);
    }

    if(allLocalRegisterPointer->numberOfLocalRegisters != 0) {
      sprintf(tmpStr3000, "%" FMT16U " local register%s allocated.", allLocalRegisterPointer->numberOfLocalRegisters, allLocalRegisterPointer->numberOfLocalRegisters==1 ? " is" : "s are");
      showString(tmpStr3000, &standardFont, 1, 132-1, vmNormal, true, true);
      showString("Local flag status:", &standardFont, 1, 154-1, vmNormal, true, true);

      for(f=0; f<16; f++) {
        if(getFlag(NUMBER_OF_GLOBAL_FLAGS+f)) {
          for(x=40*(f%10)+1; x<40*(f%10)+39; x++) {
            for(y=22*(f/10)+176-1; y<22*(f/10)+176+20-1; y++) {
             setPixel(x, y);
            }
          }
        }

        sprintf(tmpStr3000, "%d", f);
        showString(tmpStr3000, &standardFont, f<=9 ? 40*(f%10) + 17 : 40*(f%10) + 12, 22*(f/10)+176-1, getFlag(NUMBER_OF_GLOBAL_FLAGS+f) ? vmReverse : vmNormal, true, true);
      }
    }
  }
}
#endif
