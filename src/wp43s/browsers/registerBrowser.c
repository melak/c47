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

  if(currentRegisterBrowserScreen == 9999) { // Init
    currentRegisterBrowserScreen = REGISTER_X;
    rbrMode = RBR_GLOBAL;
    showContent = true;
    rbr1stDigit = true;
    calcMode = CM_REGISTER_BROWSER;
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
        registerNameWidth = showString(tmpStr3000, &standardFont, 1, 219-22*row, vmNormal, false, true);

        if(   (regist <  REGISTER_X && regist % 5 == 4)
           || (regist >= REGISTER_X && regist % 4 == 3)) {
          for(int16_t x=0; x<SCREEN_WIDTH; x++) {
            setPixel(x, 218-22*row);
          }
        }

        if(getRegisterDataType(regist) == dtReal16) {
          if(showContent) {
            realToDisplayString(REGISTER_REAL16_DATA(regist), false, TAG_NONE, tmpStr3000, &standardFont, SCREEN_WIDTH - 1 - registerNameWidth);
          }
          else {
            sprintf(tmpStr3000, "%d bytes", (int16_t)REAL16_SIZE);
          }
        }
        else if(getRegisterDataType(regist) == dtAngle16) {
          if(showContent) {
            angle16ToDisplayString(REGISTER_REAL16_DATA(regist), getRegisterAngularMode(regist), tmpStr3000, &standardFont, SCREEN_WIDTH - 1 - registerNameWidth);
          }
          else {
            sprintf(tmpStr3000, "%d bytes", (int16_t)REAL16_SIZE);
          }
        }
        else if(getRegisterDataType(regist) == dtReal34) {
          if(showContent) {
            realToDisplayString(REGISTER_REAL34_DATA(regist), true, TAG_NONE, tmpStr3000, &standardFont, SCREEN_WIDTH - 1 - registerNameWidth);
          }
          else {
            sprintf(tmpStr3000, "%d bytes", (int16_t)REAL34_SIZE);
          }
        }
        else if(getRegisterDataType(regist) == dtAngle34) {
          if(showContent) {
            angle34ToDisplayString(REGISTER_REAL34_DATA(regist), getRegisterAngularMode(regist), tmpStr3000, &standardFont, SCREEN_WIDTH - 1 - registerNameWidth);
          }
          else {
            sprintf(tmpStr3000, "%d bytes", (int16_t)REAL34_SIZE);
          }
        }
        else if(getRegisterDataType(regist) == dtComplex16) {
          if(showContent) {
            complexToDisplayString(REGISTER_COMPLEX16_DATA(regist), false, tmpStr3000, &standardFont, SCREEN_WIDTH - 1 - registerNameWidth);
          }
          else {
            sprintf(tmpStr3000, "%d bytes", (int16_t)COMPLEX16_SIZE);
          }
        }
        else if(getRegisterDataType(regist) == dtComplex34) {
          if(showContent) {
            complexToDisplayString(REGISTER_COMPLEX34_DATA(regist), true, tmpStr3000, &standardFont, SCREEN_WIDTH - 1 - registerNameWidth);
          }
          else {
            sprintf(tmpStr3000, "%d bytes", (int16_t)COMPLEX34_SIZE);
          }
        }
        else if(getRegisterDataType(regist) == dtLongInteger) {
          if(showContent) {
            longIntegerToDisplayString(regist, tmpStr3000, SCREEN_WIDTH - 1 - registerNameWidth);
          }
          else {
            sprintf(tmpStr3000, "%" FMT32U " bits := 2+%" FMT32U " bytes", (uint32_t)*(REGISTER_DATA_MAX_LEN(regist)) * 8, (uint32_t)*(REGISTER_DATA_MAX_LEN(regist)));
          }
        }
        else if(getRegisterDataType(regist) == dtShortInteger) {
          if(showContent) {
            const font_t *font;

            font = &standardFont;
            shortIntegerToDisplayString(regist, tmpStr3000, &font);
          }
          else {
            strcpy(tmpStr3000, "64 bits := 8 bytes");
          }
        }
        else if(getRegisterDataType(regist) == dtString) {
          if(showContent) {
            strcpy(tmpStr3000, "'");
            strncat(tmpStr3000, REGISTER_STRING_DATA(regist), stringByteLength(REGISTER_STRING_DATA(regist)) + 1);
            strcat(tmpStr3000, "'");
            if(stringWidth(tmpStr3000, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
              tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
              while(stringWidth(tmpStr3000, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
                tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
              }
             strcat(tmpStr3000 + stringByteLength(tmpStr3000), STD_ELLIPSIS);
            }
          }
          else {
            sprintf(tmpStr3000, "%" FMT32S " character%s := 2+%" FMT32U " bytes", stringGlyphLength(REGISTER_STRING_DATA(regist)), stringGlyphLength(REGISTER_STRING_DATA(regist))==1 ? "" : "s", (uint32_t)*(REGISTER_DATA_MAX_LEN(regist)));
          }
        }
        else {
          sprintf(tmpStr3000, "Data type %s: to be coded", getDataTypeName(getRegisterDataType(regist), false, true));
        }

        showString(tmpStr3000, &standardFont, SCREEN_WIDTH - stringWidth(tmpStr3000, &standardFont, false, true) - 1, 219-22*row, vmNormal, false, true);
      }
    }

    else if(rbrMode == RBR_LOCAL) { // Local registers
      if(numberOfLocalRegisters != 0) { // Local registers are allocated
        clearScreen(false, true, true);

        calcRegister_t regist;
        for(int16_t row=0; row<10; row++) {
          regist = currentRegisterBrowserScreen + row;
          if(regist - FIRST_LOCAL_REGISTER < numberOfLocalRegisters) {
            sprintf(tmpStr3000, "R.%02d:", regist);

            // register number
            registerNameWidth = showString(tmpStr3000, &standardFont, 1, 219-22*row, vmNormal, true, true);

            if(   (regist <  REGISTER_X && regist % 5 == 4)
               || (regist >= REGISTER_X && regist % 4 == 3)) {
              for(int16_t x=0; x<SCREEN_WIDTH; x++) {
                setPixel(x, 218-22*row);
              }
            }

            if(getRegisterDataType(regist) == dtReal16) {
              if(showContent) {
                realToDisplayString(REGISTER_REAL16_DATA(regist), false, TAG_NONE, tmpStr3000, &standardFont, SCREEN_WIDTH - 1 - registerNameWidth);
              }
              else {
                sprintf(tmpStr3000, "%d bytes", (int16_t)REAL16_SIZE);
              }
            }
            else if(getRegisterDataType(regist) == dtAngle16) {
              if(showContent) {
                angle16ToDisplayString(REGISTER_REAL16_DATA(regist), getRegisterAngularMode(regist), tmpStr3000, &standardFont, SCREEN_WIDTH - 1 - registerNameWidth);
              }
              else {
                sprintf(tmpStr3000, "%d bytes", (int16_t)REAL16_SIZE);
              }
            }
            else if(getRegisterDataType(regist) == dtReal34) {
              if(showContent) {
                realToDisplayString(REGISTER_REAL34_DATA(regist), true, TAG_NONE, tmpStr3000, &standardFont, SCREEN_WIDTH - 1 - registerNameWidth);
              }
              else {
                sprintf(tmpStr3000, "%d bytes", (int16_t)REAL34_SIZE);
              }
            }
            else if(getRegisterDataType(regist) == dtAngle34) {
              if(showContent) {
                angle34ToDisplayString(REGISTER_REAL34_DATA(regist), getRegisterAngularMode(regist), tmpStr3000, &standardFont, SCREEN_WIDTH - 1 - registerNameWidth);
              }
              else {
                sprintf(tmpStr3000, "%d bytes", (int16_t)REAL34_SIZE);
              }
            }
            else if(getRegisterDataType(regist) == dtComplex16) {
              if(showContent) {
                complexToDisplayString(REGISTER_COMPLEX16_DATA(regist), false, tmpStr3000, &standardFont, SCREEN_WIDTH - 1 - registerNameWidth);
              }
              else {
                sprintf(tmpStr3000, "4+%d bytes", (int16_t)COMPLEX16_SIZE);
              }
            }
            else if(getRegisterDataType(regist) == dtComplex34) {
              if(showContent) {
                complexToDisplayString(REGISTER_COMPLEX34_DATA(regist), true, tmpStr3000, &standardFont, SCREEN_WIDTH - 1 - registerNameWidth);
              }
              else {
                sprintf(tmpStr3000, "4+%d bytes", (int16_t)COMPLEX34_SIZE);
              }
            }
            else if(getRegisterDataType(regist) == dtLongInteger) {
              if(showContent) {
                longIntegerToDisplayString(regist, tmpStr3000, SCREEN_WIDTH - 1 - registerNameWidth);
              }
              else {
                sprintf(tmpStr3000, "%" FMT32U " bits := 4+2+%" FMT32U " bytes", (uint32_t)*(REGISTER_DATA_MAX_LEN(regist)) * 8, (uint32_t)*(REGISTER_DATA_MAX_LEN(regist)));
              }
            }
            else if(getRegisterDataType(regist) == dtShortInteger) {
              if(showContent) {
                const font_t *font;

                font = &standardFont;
                shortIntegerToDisplayString(regist, tmpStr3000, &font);
              }
              else {
                strcpy(tmpStr3000, "64 bits := 4+8 bytes");
              }
            }
            else if(getRegisterDataType(regist) == dtString) {
              if(showContent) {
                strcpy(tmpStr3000, "'");
                strncat(tmpStr3000, REGISTER_STRING_DATA(regist), stringByteLength(REGISTER_STRING_DATA(regist)) + 1);
                strcat(tmpStr3000, "'");
                if(stringWidth(tmpStr3000, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
                  tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
                  while(stringWidth(tmpStr3000, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
                    tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
                  }
                 strcat(tmpStr3000 + stringByteLength(tmpStr3000), STD_ELLIPSIS);
                }
              }
              else {
                sprintf(tmpStr3000, "%" FMT32S " character%s := 4+2+%" FMT32U " bytes", stringGlyphLength(REGISTER_STRING_DATA(regist)), stringGlyphLength(REGISTER_STRING_DATA(regist))==1 ? "" : "s", *(REGISTER_DATA_MAX_LEN(regist)));
              }
            }
            else {
              sprintf(tmpStr3000, "Data type %s: to be coded", getDataTypeName(getRegisterDataType(regist), false, true));
            }

            showString(tmpStr3000, &standardFont, SCREEN_WIDTH - stringWidth(tmpStr3000, &standardFont, false, true), 219-22*row, vmNormal, false, true);
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
