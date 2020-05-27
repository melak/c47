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
 * \file registerBrowser.c The register browser application
 ***********************************************/

#include "wp43s.h"



#ifndef TESTSUITE_BUILD
/********************************************//**
 * \brief The register browser
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void registerBrowser(uint16_t unusedParamButMandatory) {
  int16_t registerNameWidth;

  if(calcMode != CM_REGISTER_BROWSER) {
    if(calcMode == CM_AIM) {
      cursorEnabled = false;
      hideCursor();
    }

    previousCalcMode = calcMode;
    calcMode = CM_REGISTER_BROWSER;
    clearSystemFlag(FLAG_ALPHA);
  }

  if(currentRegisterBrowserScreen == 9999) { // Init
    currentRegisterBrowserScreen = REGISTER_X;
    rbrMode = RBR_GLOBAL;
    showContent = true;
    rbr1stDigit = true;
  }

  if(currentRegisterBrowserScreen < 9999) {
    if(rbrMode == RBR_GLOBAL) { // Global registers
      clearScreen(false, true, true);

      calcRegister_t regist;
      for(int16_t row=0; row<10; row++) {
        regist = (currentRegisterBrowserScreen + row) % FIRST_LOCAL_REGISTER;
        switch(regist) {
          case REGISTER_X: strcpy(tmpStr3000, "X:"); break;
          case REGISTER_Y: strcpy(tmpStr3000, "Y:"); break;
          case REGISTER_Z: strcpy(tmpStr3000, "Z:"); break;
          case REGISTER_T: strcpy(tmpStr3000, "T:"); break;
          case REGISTER_A: strcpy(tmpStr3000, "A:"); break;
          case REGISTER_B: strcpy(tmpStr3000, "B:"); break;
          case REGISTER_C: strcpy(tmpStr3000, "C:"); break;
          case REGISTER_D: strcpy(tmpStr3000, "D:"); break;
          case REGISTER_L: strcpy(tmpStr3000, "L:"); break;
          case REGISTER_I: strcpy(tmpStr3000, "I:"); break;
          case REGISTER_J: strcpy(tmpStr3000, "J:"); break;
          case REGISTER_K: strcpy(tmpStr3000, "K:"); break;
          default: sprintf(tmpStr3000, "R%02d:", regist);
        }

        // register name or number
        registerNameWidth = showString(tmpStr3000, &standardFont, 1, 219 - 22 * row, vmNormal, false, true);

        if(   (regist <  REGISTER_X && regist % 5 == 4)
           || (regist >= REGISTER_X && regist % 4 == 3)) {
          for(int16_t x=0; x<SCREEN_WIDTH; x++) {
            setPixel(x, 218 - 22 * row);
          }
        }

        if(getRegisterDataType(regist) == dtReal34) {
          if(showContent) {
            real34ToDisplayString(REGISTER_REAL34_DATA(regist), getRegisterAngularMode(regist), tmpStr3000, &standardFont, SCREEN_WIDTH - 1 - registerNameWidth, 34, false, STD_SPACE_4_PER_EM);
          }
          else {
            sprintf(tmpStr3000, "%d bytes", (int16_t)TO_BYTES(REAL34_SIZE));
          }
        }
        else if(getRegisterDataType(regist) == dtComplex34) {
          if(showContent) {
            complex34ToDisplayString(REGISTER_COMPLEX34_DATA(regist), tmpStr3000, &standardFont, SCREEN_WIDTH - 1 - registerNameWidth, 34, false, STD_SPACE_4_PER_EM);
          }
          else {
            sprintf(tmpStr3000, "%d bytes", (int16_t)TO_BYTES(COMPLEX34_SIZE));
          }
        }
        else if(getRegisterDataType(regist) == dtLongInteger) {
          if(showContent) {
            if(getRegisterLongIntegerSign(regist) == LI_NEGATIVE) {
              longIntegerRegisterToDisplayString(regist, tmpStr3000, TMP_STR_LENGTH, SCREEN_WIDTH - 1 - registerNameWidth, 50, STD_SPACE_4_PER_EM);
            }
            else {
              longIntegerRegisterToDisplayString(regist, tmpStr3000, TMP_STR_LENGTH, SCREEN_WIDTH - 9 - registerNameWidth, 50, STD_SPACE_4_PER_EM);
            }
          }
          else {
            sprintf(tmpStr3000, "%" FMT32U " bits := 4+%" FMT32U " bytes", (uint32_t)TO_BYTES(getRegisterMaxDataLength(regist)) * 8, (uint32_t)TO_BYTES(getRegisterMaxDataLength(regist)));
          }
        }
        else if(getRegisterDataType(regist) == dtShortInteger) {
          if(showContent) {
            shortIntegerToDisplayString(regist, tmpStr3000, false);
          }
          else {
            strcpy(tmpStr3000, "64 bits := 8 bytes");
          }
        }
        else if(getRegisterDataType(regist) == dtString) {
          if(showContent) {
            strcpy(tmpStr3000, STD_LEFT_SINGLE_QUOTE);
            strncat(tmpStr3000, REGISTER_STRING_DATA(regist), stringByteLength(REGISTER_STRING_DATA(regist)) + 1);
            strcat(tmpStr3000, STD_RIGHT_SINGLE_QUOTE);
            if(stringWidth(tmpStr3000, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
              tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
              while(stringWidth(tmpStr3000, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
                tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
              }
             strcat(tmpStr3000 + stringByteLength(tmpStr3000), STD_ELLIPSIS);
            }
          }
          else {
            sprintf(tmpStr3000, "%" FMT32U " character%s := 4+%" FMT32U " bytes", (uint32_t)stringGlyphLength(REGISTER_STRING_DATA(regist)), stringGlyphLength(REGISTER_STRING_DATA(regist))==1 ? "" : "s", (uint32_t)TO_BYTES(getRegisterMaxDataLength(regist)));
          }
        }
        else {
          sprintf(tmpStr3000, "Data type %s: to be coded", getDataTypeName(getRegisterDataType(regist), false, true));
        }

        showString(tmpStr3000, &standardFont, SCREEN_WIDTH - stringWidth(tmpStr3000, &standardFont, false, true) - 1, 219-22*row, vmNormal, false, true);
      }
    }

    else if(rbrMode == RBR_LOCAL) { // Local registers
      if(allLocalRegisterPointer->numberOfLocalRegisters != 0) { // Local registers are allocated
        clearScreen(false, true, true);

        calcRegister_t regist;
        for(int16_t row=0; row<10; row++) {
          regist = currentRegisterBrowserScreen + row;
          if(regist - FIRST_LOCAL_REGISTER < allLocalRegisterPointer->numberOfLocalRegisters) {
            sprintf(tmpStr3000, "R.%02d:", regist);

            // register number
            registerNameWidth = showString(tmpStr3000, &standardFont, 1, 219 - 22 * row, vmNormal, true, true);

            if(   (regist <  REGISTER_X && regist % 5 == 4)
               || (regist >= REGISTER_X && regist % 4 == 3)) {
              for(int16_t x=0; x<SCREEN_WIDTH; x++) {
                setPixel(x, 218 - 22 * row);
              }
            }

            if(getRegisterDataType(regist) == dtReal34) {
              if(showContent) {
                real34ToDisplayString(REGISTER_REAL34_DATA(regist), getRegisterAngularMode(regist), tmpStr3000, &standardFont, SCREEN_WIDTH - 1 - registerNameWidth, 34, false, STD_SPACE_4_PER_EM);
              }
              else {
                sprintf(tmpStr3000, "%d bytes", (int16_t)TO_BYTES(REAL34_SIZE));
              }
            }
            else if(getRegisterDataType(regist) == dtComplex34) {
              if(showContent) {
                complex34ToDisplayString(REGISTER_COMPLEX34_DATA(regist), tmpStr3000, &standardFont, SCREEN_WIDTH - 1 - registerNameWidth, 34, false, STD_SPACE_4_PER_EM);
              }
              else {
                sprintf(tmpStr3000, "4+%d bytes", (int16_t)TO_BYTES(COMPLEX34_SIZE));
              }
            }
            else if(getRegisterDataType(regist) == dtLongInteger) {
              if(showContent) {
                longIntegerRegisterToDisplayString(regist, tmpStr3000, TMP_STR_LENGTH, SCREEN_WIDTH - 1 - registerNameWidth, 50, STD_SPACE_4_PER_EM);
              }
              else {
                sprintf(tmpStr3000, "%" FMT32U " bits := 4+4+%" FMT32U " bytes", (uint32_t)TO_BYTES(getRegisterMaxDataLength(regist)) * 8, (uint32_t)TO_BYTES(getRegisterMaxDataLength(regist)));
              }
            }
            else if(getRegisterDataType(regist) == dtShortInteger) {
              if(showContent) {
                shortIntegerToDisplayString(regist, tmpStr3000, false);
              }
              else {
                strcpy(tmpStr3000, "64 bits := 4+8 bytes");
              }
            }
            else if(getRegisterDataType(regist) == dtString) {
              if(showContent) {
                strcpy(tmpStr3000, STD_LEFT_SINGLE_QUOTE);
                strncat(tmpStr3000, REGISTER_STRING_DATA(regist), stringByteLength(REGISTER_STRING_DATA(regist)) + 1);
                strcat(tmpStr3000, STD_RIGHT_SINGLE_QUOTE);
                if(stringWidth(tmpStr3000, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
                  tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
                  while(stringWidth(tmpStr3000, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
                    tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
                  }
                 strcat(tmpStr3000 + stringByteLength(tmpStr3000), STD_ELLIPSIS);
                }
              }
              else {
                sprintf(tmpStr3000, "%" FMT32U " character%s := 4+4+%" FMT32U " bytes", (uint32_t)stringGlyphLength(REGISTER_STRING_DATA(regist)), stringGlyphLength(REGISTER_STRING_DATA(regist))==1 ? "" : "s", (uint32_t)TO_BYTES(getRegisterMaxDataLength(regist)));
              }
            }
            else {
              sprintf(tmpStr3000, "Data type %s: to be coded", getDataTypeName(getRegisterDataType(regist), false, true));
            }

            showString(tmpStr3000, &standardFont, SCREEN_WIDTH - stringWidth(tmpStr3000, &standardFont, false, true), 219 - 22 * row, vmNormal, false, true);
          }
        }
      }
      else { // no local register allocated
        rbrMode = RBR_GLOBAL;
      }
    }
  }
}
#endif
