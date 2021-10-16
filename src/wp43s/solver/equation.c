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
 * \file equation.c
 ***********************************************/

#include "solver/equation.h"

#include "charString.h"
#include "defines.h"
#include "error.h"
#include "flags.h"
#include "fonts.h"
#include "gui.h"
#include "items.h"
#include "memory.h"
#include "registers.h"
#include "screen.h"
#include "wp43s.h"



void fnEqNew(uint16_t unusedButMandatoryParameter) {
  if(numberOfFormulae == 0) {
    allFormulae = wp43sAllocate(TO_BLOCKS(sizeof(formulaHeader_t)));
    if(allFormulae) {
      numberOfFormulae = 1;
      currentFormula = 0;
      allFormulae[0].pointerToFormulaData = WP43S_NULL;
      allFormulae[0].sizeInBlocks = 0;
    }
    else {
      displayCalcErrorMessage(ERROR_RAM_FULL, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function fnEqNew:", "there is not enough memory for a new equation!", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }
  }
  else {
    formulaHeader_t *newPtr = wp43sAllocate(TO_BLOCKS(sizeof(formulaHeader_t)) * (numberOfFormulae + 1));
    if(newPtr) {
      for(uint32_t i = 0; i < numberOfFormulae; ++i) {
        newPtr[i + (i > currentFormula ? 1 : 0)] = allFormulae[i];
      }
      ++currentFormula;
      newPtr[currentFormula].pointerToFormulaData = WP43S_NULL;
      newPtr[currentFormula].sizeInBlocks = 0;
      wp43sFree(allFormulae, TO_BLOCKS(sizeof(formulaHeader_t)) * (numberOfFormulae));
      allFormulae = newPtr;
      ++numberOfFormulae;
    }
    else {
      displayCalcErrorMessage(ERROR_RAM_FULL, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function fnEqNew:", "there is not enough memory for a new equation!", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }
  }
  fnEqEdit(NOPARAM);
}

void fnEqEdit(uint16_t unusedButMandatoryParameter) {
  const char *equationString = TO_PCMEMPTR(allFormulae[currentFormula].pointerToFormulaData);
  if(equationString) xcopy(aimBuffer, equationString, stringByteLength(equationString) + 1);
  else               aimBuffer[0] = 0;
  calcMode = CM_EIM;
  alphaCase = AC_UPPER;
  setSystemFlag(FLAG_ALPHA);
  yCursor = 0;
  xCursor = equationString ? stringGlyphLength(equationString) : 0;
  #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
    calcModeAimGui();
  #endif // PC_BUILD && (SCREEN_800X480 == 0)
}

void fnEqDelete(uint16_t unusedButMandatoryParameter) {
  deleteEquation(currentFormula);
}

void fnEqCursorLeft(uint16_t unusedButMandatoryParameter) {
  if(xCursor > 0) --xCursor;
}

void fnEqCursorRight(uint16_t unusedButMandatoryParameter) {
  if(xCursor < (uint32_t)stringGlyphLength(aimBuffer)) ++xCursor;
}



void setEquation(uint16_t equationId, const char *equationString) {
  uint32_t newSizeInBlocks = TO_BLOCKS(stringByteLength(equationString) + 1);
  uint8_t *newPtr;
  if(allFormulae[equationId].sizeInBlocks == 0) {
    newPtr = wp43sAllocate(newSizeInBlocks);
  }
  else {
    newPtr = wp43sReallocate(TO_PCMEMPTR(allFormulae[equationId].pointerToFormulaData), allFormulae[equationId].sizeInBlocks, newSizeInBlocks);
  }
  if(newPtr) {
    allFormulae[equationId].pointerToFormulaData = TO_WP43SMEMPTR(newPtr);
  }
  else {
    displayCalcErrorMessage(ERROR_RAM_FULL, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function setEquation:", "there is not enough memory for a new equation!", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }
  allFormulae[equationId].sizeInBlocks = newSizeInBlocks;
  xcopy(TO_PCMEMPTR(allFormulae[equationId].pointerToFormulaData), equationString, stringByteLength(equationString) + 1);
}

void deleteEquation(uint16_t equationId) {
  if(equationId < numberOfFormulae) {
    if(allFormulae[equationId].sizeInBlocks > 0)
      wp43sFree(TO_PCMEMPTR(allFormulae[equationId].pointerToFormulaData), allFormulae[equationId].sizeInBlocks);
    for(uint16_t i = equationId + 1; i < numberOfFormulae; ++i)
      allFormulae[i - 1] = allFormulae[i];
    wp43sFree(allFormulae + (--numberOfFormulae), TO_BLOCKS(sizeof(formulaHeader_t)));
    if(numberOfFormulae == 0)
      allFormulae = NULL;
    if(numberOfFormulae > 0 && currentFormula >= numberOfFormulae)
      currentFormula = numberOfFormulae - 1;
  }
}



static void _showExponent(char **bufPtr, const char **strPtr) {
  switch(*(++(*strPtr))) {
    case '1':
      **bufPtr         = STD_SUP_1[0];
      *((*bufPtr) + 1) = STD_SUP_1[1];
      break;
    case '2':
      **bufPtr         = STD_SUP_2[0];
      *((*bufPtr) + 1) = STD_SUP_2[1];
      break;
    case '3':
      **bufPtr         = STD_SUP_3[0];
      *((*bufPtr) + 1) = STD_SUP_3[1];
      break;
    default:
      **bufPtr         = STD_SUP_0[0];
      *((*bufPtr) + 1) = STD_SUP_0[1] + ((**strPtr) - '0');
  }
  (*bufPtr) += 2;
}
static uint32_t _checkExponent(const char *strPtr) {
  uint32_t digits = 0;
  while(1) {
    switch(*strPtr) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        ++digits;
        ++strPtr;
        break;
      case '^':
      case '.':
        return 0;
      default:
        return digits;
    }
  }
}

bool_t showEquation(uint16_t equationId, uint16_t startAt, uint16_t cursorAt) {
  if(equationId < numberOfFormulae || equationId == EQUATION_AIM_BUFFER) {
    char *bufPtr = tmpString;
    const char *strPtr = equationId == EQUATION_AIM_BUFFER ? aimBuffer : (char *)TO_PCMEMPTR(allFormulae[equationId].pointerToFormulaData);
    uint16_t strLength = 0;
    int16_t strWidth = 0;
    int16_t glyphWidth = 0;
    uint32_t doubleBytednessHistory = 0;
    uint32_t tmpVal = 0;
    bool_t labelFound = false;
    bool_t cursorShown = false;

    if(startAt > 0) {
      *bufPtr       = STD_ELLIPSIS[0];
      *(bufPtr + 1) = STD_ELLIPSIS[1];
      *(bufPtr + 2) = 0;
      strWidth += stringWidth(bufPtr, &standardFont, true, true);
      bufPtr += 2;
    }
    if(startAt == cursorAt) {
      *bufPtr       = STD_CURSOR[0];
      *(bufPtr + 1) = STD_CURSOR[1];
      *(bufPtr + 2) = 0;
      strWidth += stringWidth(bufPtr, &standardFont, true, true);
      bufPtr += 2;
      cursorShown = true;
    }

    while((*strPtr) != 0) {
      if((++strLength) > startAt) {
        doubleBytednessHistory <<= 1;
        *bufPtr = *strPtr;
        if(((*strPtr) == ':' && !labelFound && strLength <= 8) || (*strPtr) == '(') {
          *(bufPtr + 1) = STD_SPACE_4_PER_EM[0];
          *(bufPtr + 2) = STD_SPACE_4_PER_EM[1];
          *(bufPtr + 3) = 0;
          doubleBytednessHistory <<= 1;
          doubleBytednessHistory |= 1;
          bufPtr += 1;
        }
        else if((*strPtr) == ')') {
          *bufPtr       = STD_SPACE_4_PER_EM[0];
          *(bufPtr + 1) = STD_SPACE_4_PER_EM[1];
          *(bufPtr + 2) = *strPtr;
          *(bufPtr + 3) = 0;
          doubleBytednessHistory <<= 1;
          doubleBytednessHistory |= 2;
          bufPtr += 2;
        }
        else if(cursorAt == EQUATION_NO_CURSOR && (*strPtr) == '^' && (tmpVal = _checkExponent(strPtr + 1))) {
          for(uint32_t i = 0; i < tmpVal; ++i)
            _showExponent(&bufPtr, &strPtr);
          *bufPtr = 0;
          doubleBytednessHistory |= 1;
          for(uint32_t i = 1; i < tmpVal; ++i) {
            doubleBytednessHistory <<= 1;
            doubleBytednessHistory |= 1;
          }
        }
        else if((*strPtr) == '=' || (*strPtr) == '+' || (*strPtr) == '-' || (*strPtr) == '/' || (*strPtr) == '^') {
          *bufPtr       = STD_SPACE_4_PER_EM[0];
          *(bufPtr + 1) = STD_SPACE_4_PER_EM[1];
          *(bufPtr + 2) = *strPtr;
          *(bufPtr + 3) = 0;
          strWidth += stringWidth(bufPtr, &standardFont, true, true);
          *(bufPtr + 3) = STD_SPACE_4_PER_EM[0];
          *(bufPtr + 4) = STD_SPACE_4_PER_EM[1];
          *(bufPtr + 5) = 0;
          doubleBytednessHistory <<= 2;
          doubleBytednessHistory |= 5;
          bufPtr += 3;
        }
        else if(((*strPtr) == STD_CROSS[0] && (*(strPtr + 1)) == STD_CROSS[1]) || ((*strPtr) == STD_DOT[0] && (*(strPtr + 1)) == STD_DOT[1])) {
          *bufPtr       = STD_SPACE_4_PER_EM[0];
          *(bufPtr + 1) = STD_SPACE_4_PER_EM[1];
          if(getSystemFlag(FLAG_MULTx)) {
            *(bufPtr + 2) = STD_CROSS[0];
            *(bufPtr + 3) = STD_CROSS[1];
          }
          else {
            *(bufPtr + 2) = STD_DOT[0];
            *(bufPtr + 3) = STD_DOT[1];
          }
          *(bufPtr + 4) = 0;
          strWidth += stringWidth(bufPtr, &standardFont, true, true);
          *(bufPtr + 4) = STD_SPACE_4_PER_EM[0];
          *(bufPtr + 5) = STD_SPACE_4_PER_EM[1];
          *(bufPtr + 6) = 0;
          doubleBytednessHistory <<= 2;
          doubleBytednessHistory |= 7;
          bufPtr += 4;
        }
        else if((*strPtr) & 0x80) {
          *(bufPtr + 1) = *(strPtr + 1);
          *(bufPtr + 2) = 0;
          doubleBytednessHistory |= 1;
        }
        else {
          *(bufPtr + 1) = 0;
        }

        glyphWidth = stringWidth(bufPtr, &standardFont, true, true);
        strWidth += glyphWidth;
        if(strLength == cursorAt) {
          bufPtr += (doubleBytednessHistory & 0x00000001) ? 2 : 1;
          *bufPtr       = STD_CURSOR[0];
          *(bufPtr + 1) = STD_CURSOR[1];
          *(bufPtr + 2) = 0;
          glyphWidth = stringWidth(bufPtr, &standardFont, true, true);
          strWidth += glyphWidth;
          doubleBytednessHistory <<= 1;
          doubleBytednessHistory |= 1;
          cursorShown = true;
        }
        if(strWidth > (SCREEN_WIDTH - 2)) {
          glyphWidth = stringWidth(STD_ELLIPSIS, &standardFont, true, true);
          while(1) {
            if(*bufPtr == STD_CURSOR[0] && *(bufPtr + 1) == STD_CURSOR[1]) cursorShown = false;
            strWidth -= stringWidth(bufPtr, &standardFont, true, true);
            *bufPtr = 0;
            if((strWidth + glyphWidth) <= (SCREEN_WIDTH - 2)) break;
            doubleBytednessHistory >>= 1;
            bufPtr -= (doubleBytednessHistory & 0x00000001) ? 2 : 1;
          }
          *bufPtr       = STD_ELLIPSIS[0];
          *(bufPtr + 1) = STD_ELLIPSIS[1];
          *(bufPtr + 2) = 0;
          break;
        }
        bufPtr += (doubleBytednessHistory & 0x00000001) ? 2 : 1;
      }
      strPtr += ((*strPtr) & 0x80) ? 2 : 1;
    }

    if(cursorShown || cursorAt == EQUATION_NO_CURSOR)
      showString(tmpString, &standardFont, 1, SCREEN_HEIGHT - SOFTMENU_HEIGHT * 3 + 2 , vmNormal, true, true);
    return cursorShown;
  }
  else return false;
}



static int32_t _compareChar(const char *char1, const char *char2) {
  int16_t code1 = (char1[0] & 0x80) ? ((((uint16_t)(char1[0]) | 0x7f) << 8) | char1[1]) : char1[0];
  int16_t code2 = (char2[0] & 0x80) ? ((((uint16_t)(char2[0]) | 0x7f) << 8) | char2[1]) : char2[0];
  return code2 - code1;
}

static int32_t _compareStr(const char *str1, const char *str2) {
  while(1) {
    int32_t cmpChr = _compareChar(str1, str2);
    if((*str1 == 0) && (*str2 == 0)) {
      return 0;
    }
    else if(cmpChr != 0) {
      return cmpChr;
    }
    else {
      str1 += ((*str1) & 0x80) ? 2 : 1;
      str2 += ((*str2) & 0x80) ? 2 : 1;
    }
  }
}

#define PARSER_HINT_NUMERIC  0
#define PARSER_HINT_OPERATOR 1
#define PARSER_HINT_FUNCTION 2
#define PARSER_HINT_VARIABLE 3
#define PARSER_HINT_REGULAR (stringGlyphLength(buffer) == numericCount ? PARSER_HINT_NUMERIC : PARSER_HINT_VARIABLE)
static void _parseWord(char *strPtr, uint16_t parseMode, uint16_t parserHint, char *mvarBuffer) {
  switch(parseMode) {
    case EQUATION_PARSER_MVAR:
      if(parserHint == PARSER_HINT_VARIABLE) {
        char *bufPtr = mvarBuffer;
        if(_compareStr(STD_pi, strPtr) == 0) { // check for pi
          return;
        }
        while(*bufPtr != 0) { // check for duplicates
          if(_compareStr(bufPtr, strPtr) == 0) {
            return;
          }
          bufPtr += stringByteLength(bufPtr) + 1;
        }
        for(uint32_t i = CST_01; i <= CST_79; ++i) { // check for constants
          if(_compareStr(indexOfItems[i].itemCatalogName, strPtr) == 0) {
            return;
          }
        }
        (void)findOrAllocateNamedVariable(strPtr);
        xcopy(bufPtr, strPtr, stringByteLength(strPtr) + 1);
        bufPtr[stringByteLength(strPtr) + 1] = 0;
      }
      break;
    case EQUATION_PARSER_XEQ:
      printf("***Mockup for formula evaluation!\n");
      fflush(stdout);
      break;
    default:
      displayBugScreen("In function _parseWord: Unknown mode of formula parser!");
  }
}

void parseEquation(uint16_t equationId, uint16_t parseMode, char *buffer, char *mvarBuffer) {
  const char *strPtr = (char *)TO_PCMEMPTR(allFormulae[equationId].pointerToFormulaData);
  char *bufPtr = buffer;
  int16_t numericCount = 0;

  if(parseMode == EQUATION_PARSER_MVAR) {
    mvarBuffer[0] = mvarBuffer[1] = 0;
  }

  while(*strPtr != 0) {
    switch(*strPtr) {
      case '(':
        if(bufPtr != buffer) {
          *(bufPtr++) = 0;
          ++strPtr;
          if(stringGlyphLength(buffer) == numericCount) {
            displayCalcErrorMessage(ERROR_SYNTAX_ERROR_IN_EQUATION, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
              moreInfoOnError("In function parseEquation:", "attempt to call a number as a function!", NULL, NULL);
            #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
            return;
          }
          else {
            _parseWord(buffer, parseMode, PARSER_HINT_FUNCTION, mvarBuffer);
            bufPtr = buffer;
            numericCount = 0;
            break;
          }
        }
        /* fallthrough */
      case '=':
      case '+':
      case '-':
      case '/':
      case ')':
      case '^':
        if(bufPtr != buffer) {
          *(bufPtr++) = 0;
          _parseWord(buffer, parseMode, PARSER_HINT_REGULAR, mvarBuffer);
        }
        buffer[0] = *(strPtr++);
        buffer[1] = 0;
        _parseWord(buffer, parseMode, PARSER_HINT_OPERATOR, mvarBuffer);
        bufPtr = buffer;
        numericCount = 0;
        break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '.':
        ++numericCount;
        /* fallthrough */
      default:
        if(_compareChar(strPtr, STD_CROSS) == 0 || _compareChar(strPtr, STD_DOT) == 0) {
          *(bufPtr++) = 0;
          _parseWord(buffer, parseMode, PARSER_HINT_REGULAR, mvarBuffer);
          buffer[0] = *(strPtr++);
          buffer[1] = *(strPtr++);
          buffer[2] = 0;
          _parseWord(buffer, parseMode, PARSER_HINT_OPERATOR, mvarBuffer);
          bufPtr = buffer;
          numericCount = 0;
        }
        else {
          *(bufPtr++) = *strPtr;
          if((*(strPtr++)) & 0x80) {
            *(bufPtr++) = *(strPtr++);
          }
        }
    }
    fflush(stdout);
  }
  if(bufPtr != buffer) {
    *(bufPtr++) = 0;
    _parseWord(buffer, parseMode, PARSER_HINT_REGULAR, mvarBuffer);
  }
  fflush(stdout);
}
