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
#include "realType.h"
#include "registers.h"
#include "screen.h"
#include "stack.h"
#include "wp43s.h"



#ifndef TESTSUITE_BUILD
  typedef struct {
    char     name[16];
    uint16_t opCode;
    uint16_t unused;
  } functionAlias_t;
  TO_QSPI static const functionAlias_t functionAlias[] = {
    //name             opCode           padding
    { STD_CUBE_ROOT,   ITM_CUBEROOT,    0}, // Cube root
    { "cn",            ITM_cn,          0}, // Cosinus amplitudinis
    { "dn",            ITM_dn,          0}, // Delta amplitudinis
    { "E",             ITM_Ek,          0}, // Complete or incomplete elliptic integral of 2nd kind
    { "F",             ITM_Fphik,       0}, // Incomplete elliptic integral of 1st kind
    { "J" STD_SUB_y,   ITM_JYX,         0}, // Bessel function
    { "K",             ITM_Kk,          0}, // Complete elliptic integral of 1st kind
    { "sn",            ITM_sn,          0}, // Sinus amplitudinis
    { "Y" STD_SUB_y,   ITM_YYX,         0}, // Bessel function
    { STD_beta,        ITM_BETAXY,      0}, // Beta function
    { STD_GAMMA,       ITM_GAMMAX,      0}, // Gamma function
    { STD_zeta,        ITM_zetaX,       0}, // Riemann zeta function
    { STD_ZETA,        ITM_ZETAphik,    0}, // Jacobi zeta function
    { STD_PI,          ITM_PInk,        0}, // Complete elliptic integral of 3rd kind
    { STD_psi,         ITM_am,          0}, // Jacobi amplitude
    { STD_SQUARE_ROOT, ITM_SQUAREROOTX, 0}, // Square root (available through f SQRT in EIM)
    { "",              0,               0}  // Sentinel
  };
#endif /* TESTSUITE_BUILD */


void fnEqNew(uint16_t unusedButMandatoryParameter) {
  if(numberOfFormulae == 0) {
    allFormulae = allocWp43s(TO_BLOCKS(sizeof(formulaHeader_t)));
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
    formulaHeader_t *newPtr = allocWp43s(TO_BLOCKS(sizeof(formulaHeader_t)) * (numberOfFormulae + 1));
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
#ifndef TESTSUITE_BUILD
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
#endif /* TESTSUITE_BUILD */
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

void fnEqCalc(uint16_t unusedButMandatoryParameter) {
  parseEquation(currentFormula, EQUATION_PARSER_XEQ, tmpString, tmpString + AIM_BUFFER_LENGTH);
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}



void setEquation(uint16_t equationId, const char *equationString) {
  uint32_t newSizeInBlocks = TO_BLOCKS(stringByteLength(equationString) + 1);
  uint8_t *newPtr;
  if(allFormulae[equationId].sizeInBlocks == 0) {
    newPtr = allocWp43s(newSizeInBlocks);
  }
  else {
    newPtr = reallocWp43s(TO_PCMEMPTR(allFormulae[equationId].pointerToFormulaData), allFormulae[equationId].sizeInBlocks, newSizeInBlocks);
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



#ifndef TESTSUITE_BUILD
static void _showExponent(char **bufPtr, const char **strPtr, int16_t *strWidth) {
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
  *((*bufPtr) + 2) = 0;
  (*strWidth) += stringWidth(*bufPtr, &standardFont, true, true);
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

static int32_t _compareChar(const char *char1, const char *char2) {
  int16_t code1 = (char1[0] & 0x80) ? ((((uint16_t)(char1[0]) | 0x7f) << 8) | char1[1]) : char1[0];
  int16_t code2 = (char2[0] & 0x80) ? ((((uint16_t)(char2[0]) | 0x7f) << 8) | char2[1]) : char2[0];
  return code2 - code1;
}

static void _addSpace(char **bufPtr, int16_t *strWidth, uint32_t *doubleBytednessHistory) { // space between an operand and an operator
  bool_t spaceShallBeAdded = true;
  if(((*bufPtr) >= (tmpString + 2)) && (_compareChar((*bufPtr) - 2, STD_SPACE_PUNCTUATION) == 0)) spaceShallBeAdded = false;
  if(((*bufPtr) >= (tmpString + 1)) && (((*doubleBytednessHistory) & 1) == 0 && *((*bufPtr) - 1) == ' ')) spaceShallBeAdded = false;
  if(spaceShallBeAdded) {
    **bufPtr         = STD_SPACE_PUNCTUATION[0];
    *((*bufPtr) + 1) = STD_SPACE_PUNCTUATION[1];
    *((*bufPtr) + 2) = 0;
    *strWidth += stringWidth(*bufPtr, &standardFont, true, true);
    *bufPtr += 2;
    *doubleBytednessHistory |= 1;
    *doubleBytednessHistory <<= 1;
  }
}
#endif /* TESTSUITE_BUILD */

void showEquation(uint16_t equationId, uint16_t startAt, uint16_t cursorAt, bool_t dryRun, bool_t *cursorShown, bool_t *rightEllipsis) {
#ifndef TESTSUITE_BUILD
  if(equationId < numberOfFormulae || equationId == EQUATION_AIM_BUFFER) {
    char *bufPtr = tmpString;
    const char *strPtr = equationId == EQUATION_AIM_BUFFER ? aimBuffer : (char *)TO_PCMEMPTR(allFormulae[equationId].pointerToFormulaData);
    uint16_t strLength = 0;
    int16_t strWidth = 0;
    int16_t glyphWidth = 0;
    uint32_t doubleBytednessHistory = 0;
    uint32_t tmpVal = 0;
    bool_t inLabel = false;
    bool_t unaryMinus = true;
    const char *tmpPtr = strPtr;

    bool_t _cursorShown, _rightEllipsis;
    if(cursorShown == NULL)   cursorShown   = &_cursorShown;
    if(rightEllipsis == NULL) rightEllipsis = &_rightEllipsis;
    *cursorShown = false;
    *rightEllipsis = false;

    for(uint32_t i = 0; i < 7; ++i) {
      tmpPtr += ((*tmpPtr) & 0x80) ? 2 : 1;
      if(*tmpPtr == ':') {
        inLabel = true;
        tmpVal = i;
        break;
      }
    }
    bufPtr = tmpString;

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
      *cursorShown = true;
    }

    while((*strPtr) != 0) {
      if((++strLength) > startAt) {
        doubleBytednessHistory <<= 1;
        *bufPtr = *strPtr;

        /* End of label */
        if((*strPtr) == ':') {
          *(bufPtr + 1) = 0;
          strWidth += stringWidth(bufPtr, &standardFont, true, true);
          *(bufPtr + 1) = ' ';
          *(bufPtr + 2) = 0;
          doubleBytednessHistory <<= 1;
          bufPtr += 1;
          inLabel = false;
        }

        /* Argument separator */
        else if((!inLabel) && (*strPtr) == ' ') {
          *bufPtr       = *strPtr;
          *(bufPtr + 1) = 0;
          unaryMinus = true;
        }

        /* Unary minus */
        else if((!inLabel) && unaryMinus && (*strPtr) == '-') {
          if(strLength > 1) _addSpace(&bufPtr, &strWidth, &doubleBytednessHistory);
          *bufPtr       = *strPtr;
          *(bufPtr + 1) = 0;
          unaryMinus = false;
        }

        /* Opening parenthesis */
        else if((!inLabel) && (*strPtr) == '(') {
          *(bufPtr + 1) = 0;
          strWidth += stringWidth(bufPtr, &standardFont, true, true);
          *(bufPtr + 1) = STD_SPACE_PUNCTUATION[0];
          *(bufPtr + 2) = STD_SPACE_PUNCTUATION[1];
          *(bufPtr + 3) = 0;
          doubleBytednessHistory <<= 1;
          doubleBytednessHistory |= 1;
          bufPtr += 1;
          unaryMinus = true;
        }

        /* Power (if not editing) */
        else if((!inLabel) && (cursorAt == EQUATION_NO_CURSOR && (*strPtr) == '^' && (tmpVal = _checkExponent(strPtr + 1)))) {
          for(uint32_t i = 0; i < tmpVal; ++i)
            _showExponent(&bufPtr, &strPtr, &strWidth);
          *bufPtr = 0;
          bufPtr -= 2;
          strWidth -= stringWidth(bufPtr, &standardFont, true, true);
          doubleBytednessHistory |= 1;
          for(uint32_t i = 1; i < tmpVal; ++i) {
            doubleBytednessHistory <<= 1;
            doubleBytednessHistory |= 1;
          }
          unaryMinus = false;
        }

        /* Closing parenthesis or power (when editing) */
        else if((!inLabel) && ((*strPtr) == ')' || (*strPtr) == '^')) {
          _addSpace(&bufPtr, &strWidth, &doubleBytednessHistory);
          *bufPtr       = *strPtr;
          *(bufPtr + 1) = 0;
          unaryMinus = false;
        }

        /* Operators */
        else if((!inLabel) && ((*strPtr) == '=' || (*strPtr) == '+' || (*strPtr) == '-' || (*strPtr) == '/' || (*strPtr) == '!' || (*strPtr) == '|')) {
          if((*strPtr) != '|' || (strLength > (startAt + 1)))
            _addSpace(&bufPtr, &strWidth, &doubleBytednessHistory);
          *bufPtr       = *strPtr;
          *(bufPtr + 1) = 0;
          strWidth += stringWidth(bufPtr, &standardFont, true, true);
          *(bufPtr + 1) = STD_SPACE_PUNCTUATION[0];
          *(bufPtr + 2) = STD_SPACE_PUNCTUATION[1];
          *(bufPtr + 3) = 0;
          doubleBytednessHistory <<= 1;
          doubleBytednessHistory |= 1;
          bufPtr += 1;
          unaryMinus = false;
        }

        /* Multiply */
        else if((!inLabel) && (((*strPtr) == STD_CROSS[0] && (*(strPtr + 1)) == STD_CROSS[1]) || ((*strPtr) == STD_DOT[0] && (*(strPtr + 1)) == STD_DOT[1]))) {
          _addSpace(&bufPtr, &strWidth, &doubleBytednessHistory);
          //if(getSystemFlag(FLAG_MULTx)) {
          //  *bufPtr       = STD_CROSS[0];
          //  *(bufPtr + 1) = STD_CROSS[1];
          //}
          //else {
            *bufPtr       = STD_DOT[0];
            *(bufPtr + 1) = STD_DOT[1];
          //}
          *(bufPtr + 2) = 0;
          strWidth += stringWidth(bufPtr, &standardFont, true, true);
          *(bufPtr + 2) = STD_SPACE_PUNCTUATION[0];
          *(bufPtr + 3) = STD_SPACE_PUNCTUATION[1];
          *(bufPtr + 4) = 0;
          doubleBytednessHistory <<= 1;
          doubleBytednessHistory |= 3;
          bufPtr += 2;
          unaryMinus = false;
        }

        /* Other double-byte characters */
        else if((*strPtr) & 0x80) {
          *(bufPtr + 1) = *(strPtr + 1);
          *(bufPtr + 2) = 0;
          doubleBytednessHistory |= 1;
          unaryMinus = false;
        }

        /* Other single-byte characters */
        else {
          *(bufPtr + 1) = 0;
          unaryMinus = false;
        }

        /* Add the character */
        glyphWidth = stringWidth(bufPtr, &standardFont, true, true);
        strWidth += glyphWidth;

        /* Cursor */
        if(strLength == cursorAt) {
          bufPtr += (doubleBytednessHistory & 0x00000001) ? 2 : 1;
          *bufPtr       = STD_CURSOR[0];
          *(bufPtr + 1) = STD_CURSOR[1];
          *(bufPtr + 2) = 0;
          glyphWidth = stringWidth(bufPtr, &standardFont, true, true);
          strWidth += glyphWidth;
          doubleBytednessHistory <<= 1;
          doubleBytednessHistory |= 1;
          *cursorShown = true;
        }

        /* Trailing ellipsis */
        if(strWidth > (SCREEN_WIDTH - 2)) {
          glyphWidth = stringWidth(STD_ELLIPSIS, &standardFont, true, true);
          while(1) {
            if(*bufPtr == STD_CURSOR[0] && *(bufPtr + 1) == STD_CURSOR[1]) *cursorShown = false;
            strWidth -= stringWidth(bufPtr, &standardFont, true, true);
            *bufPtr = 0;
            if((strWidth + glyphWidth) <= (SCREEN_WIDTH - 2)) break;
            doubleBytednessHistory >>= 1;
            bufPtr -= (doubleBytednessHistory & 0x00000001) ? 2 : 1;
          }
          *bufPtr       = STD_ELLIPSIS[0];
          *(bufPtr + 1) = STD_ELLIPSIS[1];
          *(bufPtr + 2) = 0;
          *rightEllipsis = true;
          break;
        }

        /* Increment bufPtr */
        bufPtr += (doubleBytednessHistory & 0x00000001) ? 2 : 1;
      }
      strPtr += ((*strPtr) & 0x80) ? 2 : 1;
    }

    if((!dryRun) && (*cursorShown || cursorAt == EQUATION_NO_CURSOR))
      showString(tmpString, &standardFont, 1, SCREEN_HEIGHT - SOFTMENU_HEIGHT * 3 + 2 , vmNormal, true, true);
  }
#endif /* TESTSUITE_BUILD */
}



#ifndef TESTSUITE_BUILD
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

static void _menuF6(char *bufPtr) {
  xcopy(bufPtr, "Calc", 5);
  bufPtr[5] = 0;
}

#define PARSER_HINT_NUMERIC  0
#define PARSER_HINT_OPERATOR 1
#define PARSER_HINT_FUNCTION 2
#define PARSER_HINT_VARIABLE 3
#define PARSER_HINT_REGULAR  (stringGlyphLength(buffer) == numericCount ? PARSER_HINT_NUMERIC : PARSER_HINT_VARIABLE)

//#define PARSER_OPERATOR_STACK_SIZE (getSystemFlag(FLAG_SSIZE8) ? 8 : 4)
#define PARSER_OPERATOR_STACK_SIZE (getSystemFlag(FLAG_SSIZE8) ? 8 : 4)

#define PARSER_OPERATOR_ITM_PARENTHESIS_LEFT   5000
#define PARSER_OPERATOR_ITM_PARENTHESIS_RIGHT  5001
#define PARSER_OPERATOR_ITM_VERTICAL_BAR_LEFT  5002
#define PARSER_OPERATOR_ITM_VERTICAL_BAR_RIGHT 5003
#define PARSER_OPERATOR_ITM_EQUAL              5004
#define PARSER_OPERATOR_ITM_YX                 5005
#define PARSER_OPERATOR_ITM_END_OF_FORMULA     5006

static uint32_t _operatorPriority(uint16_t func) {
  // priority of operator: smaller number represents higher priority 
  // associative property: even number represents left-associativity, odd number represents right-associativity.
  switch(func) {
    case ITM_MULT:
    case ITM_DIV:
      return 8;
    case ITM_ADD:
    case ITM_SUB:
      return 10;
    case PARSER_OPERATOR_ITM_YX:
      return 7;
    case ITM_XFACT:
      return 5;
    case PARSER_OPERATOR_ITM_PARENTHESIS_LEFT:
    case PARSER_OPERATOR_ITM_PARENTHESIS_RIGHT:
    case PARSER_OPERATOR_ITM_VERTICAL_BAR_LEFT:
    case PARSER_OPERATOR_ITM_VERTICAL_BAR_RIGHT:
    case PARSER_OPERATOR_ITM_EQUAL:
      return 1;
    default:
      return 3;
  }
}
static void _processOperator(uint16_t func, char *mvarBuffer) {
  uint32_t opStackTop = 0xffffffffu;
  for(uint32_t i = 0; i <= PARSER_OPERATOR_STACK_SIZE; ++i) {
    if((i == PARSER_OPERATOR_STACK_SIZE) || (((uint16_t *)mvarBuffer)[i] == 0)) {
      opStackTop = i;
      break;
    }
  }
  if(opStackTop != 0xffffffffu) {
    /* flush operator stack */
    /* closing parenthesis, equal, or end of formula */
    if(func == PARSER_OPERATOR_ITM_PARENTHESIS_RIGHT || func == PARSER_OPERATOR_ITM_VERTICAL_BAR_RIGHT || func == PARSER_OPERATOR_ITM_EQUAL || func == PARSER_OPERATOR_ITM_END_OF_FORMULA) {
      for(int32_t i = (int32_t)opStackTop - 1; i >= 0; --i) {
        switch(((uint16_t *)mvarBuffer)[i]) {
          case PARSER_OPERATOR_ITM_PARENTHESIS_LEFT:
            if(func == PARSER_OPERATOR_ITM_VERTICAL_BAR_RIGHT) {
              displayCalcErrorMessage(ERROR_SYNTAX_ERROR_IN_EQUATION, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                moreInfoOnError("In function parseEquation:", "parentheses mismatch!", "parenthesis not closed", NULL);
              #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
            }
            break;
          case PARSER_OPERATOR_ITM_VERTICAL_BAR_LEFT:
            runFunction(ITM_ABS);
            if(func == PARSER_OPERATOR_ITM_PARENTHESIS_RIGHT) {
              displayCalcErrorMessage(ERROR_SYNTAX_ERROR_IN_EQUATION, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                moreInfoOnError("In function parseEquation:", "parentheses mismatch!", "unpaired vertical bar within parentheses", NULL);
              #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
            }
            break;
          case PARSER_OPERATOR_ITM_YX:
            runFunction(ITM_YX);
            break;
          default:
            runFunction(((uint16_t *)mvarBuffer)[i]);
        }
        switch(((uint16_t *)mvarBuffer)[i]) {
          case ITM_ADD:
          case ITM_SUB:
          case ITM_MULT:
          case ITM_DIV:
          case PARSER_OPERATOR_ITM_YX:
            ((uint16_t *)mvarBuffer)[i] = 0;
            break;
          default:
            ((uint16_t *)mvarBuffer)[i] = 0;
            if(func == PARSER_OPERATOR_ITM_PARENTHESIS_RIGHT || func == PARSER_OPERATOR_ITM_VERTICAL_BAR_RIGHT) return;
            displayCalcErrorMessage(ERROR_SYNTAX_ERROR_IN_EQUATION, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
              moreInfoOnError("In function parseEquation:", "parentheses mismatch!", "parenthesis not closed", NULL);
            #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
            break;
        }
      }
      switch(func) {
        case PARSER_OPERATOR_ITM_PARENTHESIS_RIGHT:
          displayCalcErrorMessage(ERROR_SYNTAX_ERROR_IN_EQUATION, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            moreInfoOnError("In function parseEquation:", "parentheses mismatch!", "no corresponding opening parenthesis", NULL);
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
          break;
        case PARSER_OPERATOR_ITM_VERTICAL_BAR_RIGHT:
          displayCalcErrorMessage(ERROR_SYNTAX_ERROR_IN_EQUATION, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            moreInfoOnError("In function parseEquation:", "parentheses mismatch!", "no corresponding opening vertical bar", NULL);
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
          break;
        case PARSER_OPERATOR_ITM_EQUAL:
          fnToReal(NOPARAM);
          real34Copy(REGISTER_REAL34_DATA(REGISTER_X), (real34_t *)(mvarBuffer + PARSER_OPERATOR_STACK_SIZE * 2));
          break;
        default:
          setSystemFlag(FLAG_ASLIFT);
          liftStack();
          reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
          real34Copy((real34_t *)(mvarBuffer + PARSER_OPERATOR_STACK_SIZE * 2), REGISTER_REAL34_DATA(REGISTER_X));
          runFunction(ITM_SUB);
      }
      return;
    }

    /* stack is empty */
    if(opStackTop == 0) {
      ((uint16_t *)mvarBuffer)[0] = func;
      return;
    }

    /* other cases */
    for(uint32_t i = opStackTop; i > 0; --i) {

      /* factorial */
      if(func == ITM_XFACT) {
        runFunction(ITM_XFACT);
        return;
      }

      /* push an operator */
      else if(
        (_operatorPriority(((uint16_t *)mvarBuffer)[i - 1]) < 4) || /* parenthesis */
        (_operatorPriority(((uint16_t *)mvarBuffer)[i - 1]) & (~1u)) > (_operatorPriority(func) & (~1u)) || /* higher priority */
        ((_operatorPriority(((uint16_t *)mvarBuffer)[i - 1]) & (~1u)) == (_operatorPriority(func) & (~1u)) && (_operatorPriority(func) & 1) /* same priority and right-associative */ )
      ) {
        if(i < PARSER_OPERATOR_STACK_SIZE) {
          ((uint16_t *)mvarBuffer)[i] = func;
        }
        else {
          displayCalcErrorMessage(ERROR_EQUATION_TOO_COMPLEX, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            moreInfoOnError("In function parseEquation:", "operator stack overflow!", NULL, NULL);
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        }
        return;
      }

      /* pop an operator */
      else {
        runFunction(((uint16_t *)mvarBuffer)[i - 1] == PARSER_OPERATOR_ITM_YX ? ITM_YX :
                    ((uint16_t *)mvarBuffer)[i - 1] == PARSER_OPERATOR_ITM_VERTICAL_BAR_LEFT ? ITM_ABS :
                    ((uint16_t *)mvarBuffer)[i - 1]);
        if(i == 1) {
          ((uint16_t *)mvarBuffer)[i - 1] = func;
          return;
        }
        else {
          ((uint16_t *)mvarBuffer)[i - 1] = 0;
        }
      }

    }
  }
  else {
    displayCalcErrorMessage(ERROR_EQUATION_TOO_COMPLEX, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function parseEquation:", "operator stack overflow!", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}
static void _parseWord(char *strPtr, uint16_t parseMode, uint16_t parserHint, char *mvarBuffer) {
  uint32_t tmpVal = 0;
  if(parserHint != PARSER_HINT_NUMERIC && stringGlyphLength(strPtr) > 7) {
    displayCalcErrorMessage(ERROR_SYNTAX_ERROR_IN_EQUATION, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function parseEquation:", "token too long!", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }
  if(strPtr[0] == 0) {
    return;
  }

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
          ++tmpVal;
        }
        for(uint32_t i = CST_01; i <= CST_79; ++i) { // check for constants
          if(_compareStr(indexOfItems[i].itemCatalogName, strPtr) == 0) {
            return;
          }
        }
        (void)findOrAllocateNamedVariable(strPtr);
        xcopy(bufPtr, strPtr, stringByteLength(strPtr) + 1);
        bufPtr += stringByteLength(strPtr) + 1;
        bufPtr[0] = 0;
        if(tmpVal == 4) {
          _menuF6(bufPtr);
        }
      }
      break;

    case EQUATION_PARSER_XEQ:
      if(parserHint == PARSER_HINT_VARIABLE) {
        if(_compareStr(STD_pi, strPtr) == 0) { // check for pi
          runFunction(ITM_CONSTpi);
          return;
        }
        for(uint32_t i = CST_01; i <= CST_79; ++i) { // check for constants
          if(_compareStr(indexOfItems[i].itemCatalogName, strPtr) == 0) {
            runFunction(i);
            return;
          }
        }
        reallyRunFunction(ITM_RCL, findNamedVariable(strPtr));
      }
      else if(parserHint == PARSER_HINT_NUMERIC) {
        liftStack();
        setSystemFlag(FLAG_ASLIFT);
        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
        stringToReal34(strPtr, REGISTER_REAL34_DATA(REGISTER_X));
      }
      else if(parserHint == PARSER_HINT_OPERATOR) {
        if(_compareStr("+", strPtr) == 0) {
          _processOperator(ITM_ADD, mvarBuffer);
        }
        else if(_compareStr("-", strPtr) == 0) {
          _processOperator(ITM_SUB, mvarBuffer);
        }
        else if(_compareStr(STD_CROSS, strPtr) == 0 || _compareStr(STD_DOT, strPtr) == 0) {
          _processOperator(ITM_MULT, mvarBuffer);
        }
        else if(_compareStr("/", strPtr) == 0) {
          _processOperator(ITM_DIV, mvarBuffer);
        }
        else if(_compareStr("^", strPtr) == 0) {
          _processOperator(PARSER_OPERATOR_ITM_YX, mvarBuffer);
        }
        else if(_compareStr("!", strPtr) == 0) {
          _processOperator(ITM_XFACT, mvarBuffer);
        }
        else if(_compareStr("(", strPtr) == 0) {
          _processOperator(PARSER_OPERATOR_ITM_PARENTHESIS_LEFT, mvarBuffer);
        }
        else if(_compareStr(")", strPtr) == 0) {
          _processOperator(PARSER_OPERATOR_ITM_PARENTHESIS_RIGHT, mvarBuffer);
        }
        else if(_compareStr("|", strPtr) == 0) {
          _processOperator(PARSER_OPERATOR_ITM_VERTICAL_BAR_LEFT, mvarBuffer);
        }
        else if(_compareStr("|)", strPtr) == 0) {
          _processOperator(PARSER_OPERATOR_ITM_VERTICAL_BAR_RIGHT, mvarBuffer);
        }
        else if(_compareStr("=", strPtr) == 0) {
          _processOperator(PARSER_OPERATOR_ITM_EQUAL, mvarBuffer);
        }
        else if(_compareStr(":", strPtr) == 0) {
          // label will be skipped
        }
        else if(_compareStr(" ", strPtr) == 0) {
          uint32_t opStackTop = 0xffffffffu;
          for(uint32_t i = 0; i <= PARSER_OPERATOR_STACK_SIZE; ++i) {
            if((i == PARSER_OPERATOR_STACK_SIZE) || (((uint16_t *)mvarBuffer)[i] == 0)) {
              opStackTop = i;
              break;
            }
          }
          if((opStackTop != 0) && (((uint16_t *)mvarBuffer)[opStackTop - 1] == ITM_Ek)) {
            ((uint16_t *)mvarBuffer)[opStackTop - 1] = ITM_Ephik;
          }
        }
        else {
          displayBugScreen("In function _parseWord: Unknown operator appeared!");
        }
      }
      else if(parserHint == PARSER_HINT_FUNCTION) {
        for(uint32_t i = 0; functionAlias[i].name[0] != 0; ++i) {
          if(_compareStr(functionAlias[i].name, strPtr) == 0) {
            _processOperator(functionAlias[i].opCode, mvarBuffer);
            return;
          }
        }
        for(uint32_t i = 1; i < LAST_ITEM; ++i) {
          if(((indexOfItems[i].status & CAT_STATUS) == CAT_FNCT) && (indexOfItems[i].param <= NOPARAM) && (_compareStr(indexOfItems[i].itemCatalogName, strPtr) == 0)) {
            _processOperator(i, mvarBuffer);
            return;
          }
        }
        for(uint32_t i = 1; i < LAST_ITEM; ++i) {
          if(((indexOfItems[i].status & CAT_STATUS) == CAT_FNCT) && (indexOfItems[i].param <= NOPARAM) && (_compareStr(indexOfItems[i].itemSoftmenuName, strPtr) == 0)) {
            _processOperator(i, mvarBuffer);
            return;
          }
        }
        displayCalcErrorMessage(ERROR_FUNCTION_NOT_FOUND, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          stringToUtf8(strPtr, (uint8_t *)errorMessage);
          moreInfoOnError("In function parseEquation:", errorMessage, "is not recognized as a function", NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      }
      break;

    default:
      displayBugScreen("In function _parseWord: Unknown mode of formula parser!");
  }
}
#endif /* TESTSUITE_BUILD */

void parseEquation(uint16_t equationId, uint16_t parseMode, char *buffer, char *mvarBuffer) {
#ifndef TESTSUITE_BUILD
  const char *strPtr = (char *)TO_PCMEMPTR(allFormulae[equationId].pointerToFormulaData);
  char *bufPtr = buffer;
  int16_t numericCount = 0;
  bool_t equalAppeared = false, labeled = false, afterClosingParenthesis = false, unaryMinusCanOccur = true;

  for(uint32_t i = 0; i < (PARSER_OPERATOR_STACK_SIZE * 2); ++i) mvarBuffer[i] = 0;
  real34Zero((real34_t *)(mvarBuffer + PARSER_OPERATOR_STACK_SIZE * 2));
  if(parseMode == EQUATION_PARSER_XEQ) {
    fnClearStack(NOPARAM);
  }

  for(uint32_t i = 0; i < 7; ++i) {
    strPtr += ((*strPtr) & 0x80) ? 2 : 1;
    if(*strPtr == ':') {
      labeled = true;
      ++strPtr;
      break;
    }
  }
  if(!labeled) {
    strPtr = (char *)TO_PCMEMPTR(allFormulae[equationId].pointerToFormulaData);
  }

  while(*strPtr != 0) {
    switch(*strPtr) {
      case ':':
        displayCalcErrorMessage(ERROR_SYNTAX_ERROR_IN_EQUATION, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          moreInfoOnError("In function parseEquation:", "unexpected \":\"", "label too long or \":\" appeared more than once", NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        return;

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
            buffer[0] = 0;
            numericCount = 0;
            break;
          }
          unaryMinusCanOccur = true;
        }
        /* fallthrough */
      case '=':
      case '+':
      case '-':
      case '/':
      case ')':
      case '^':
      case '!':
      case ' ':
      case '|':
        if(equalAppeared && (*strPtr == '=')) {
          displayCalcErrorMessage(ERROR_SYNTAX_ERROR_IN_EQUATION, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            moreInfoOnError("In function parseEquation:", "= appears more than once", NULL, NULL);
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
          return;
        }
        else if((bufPtr != buffer) && (*strPtr == '|')) {
          *(bufPtr++) = 0;
          _parseWord(buffer, parseMode, PARSER_HINT_REGULAR, mvarBuffer);
          bufPtr = buffer;
          numericCount = 0;
          *(bufPtr++) = '|';
          *(bufPtr++) = ')';
          *(bufPtr++) = 0;
          afterClosingParenthesis = true;
          unaryMinusCanOccur = false;
        }
        else if(bufPtr != buffer) {
          *(bufPtr++) = 0;
          if(*strPtr != ':')
            _parseWord(buffer, parseMode, PARSER_HINT_REGULAR, mvarBuffer);
          afterClosingParenthesis = (*strPtr == ')');
          unaryMinusCanOccur = false;
        }
        else if(unaryMinusCanOccur && *strPtr == '-') {
          /* unary minus */
          buffer[0] = '-';
          buffer[1] = '1';
          buffer[2] = 0;
          _parseWord(buffer, parseMode, PARSER_HINT_NUMERIC, mvarBuffer);
          buffer[0] = STD_DOT[0];
          buffer[1] = STD_DOT[1];
          buffer[2] = 0;
          _parseWord(buffer, parseMode, PARSER_HINT_OPERATOR, mvarBuffer);
          bufPtr = buffer;
          buffer[0] = 0;
          numericCount = 0;
          afterClosingParenthesis = false;
          unaryMinusCanOccur = false;
          ++strPtr;
          break;
        }
        else if((*strPtr == '(') || (*strPtr == '|')) {
          afterClosingParenthesis = false;
          unaryMinusCanOccur = true;
        }
        else if(*strPtr == ')') {
          afterClosingParenthesis = true;
          unaryMinusCanOccur = false;
        }
        else if(afterClosingParenthesis && *strPtr != ' ') {
          afterClosingParenthesis = false;
          unaryMinusCanOccur = false;
        }
        else {
          displayCalcErrorMessage(ERROR_SYNTAX_ERROR_IN_EQUATION, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            moreInfoOnError("In function parseEquation:", "unexpected operator", NULL, NULL);
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
          return;
        }
        if(*strPtr == '=') {
          equalAppeared = true;
          unaryMinusCanOccur = true;
        }

        if(_compareStr("|)", buffer) != 0) {
          buffer[0] = *(strPtr++);
          buffer[1] = 0;
        }
        else {
          ++strPtr;
        }
        _parseWord(buffer, parseMode, PARSER_HINT_OPERATOR, mvarBuffer);
        bufPtr = buffer;
        buffer[0] = 0;
        numericCount = 0;
        break;
        /* fallthrough */
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
          buffer[0] = 0;
          numericCount = 0;
        }
        else {
          *(bufPtr++) = *strPtr;
          if((*(strPtr++)) & 0x80) {
            *(bufPtr++) = *(strPtr++);
          }
        }
        afterClosingParenthesis = false;
        unaryMinusCanOccur = false;
    }
    if(lastErrorCode != ERROR_NONE) return;
  }
  if(bufPtr != buffer) {
    *(bufPtr++) = 0;
    _parseWord(buffer, parseMode, PARSER_HINT_REGULAR, mvarBuffer);
  }

  if(stringGlyphLength(buffer) > 7) {
    displayCalcErrorMessage(ERROR_SYNTAX_ERROR_IN_EQUATION, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      moreInfoOnError("In function parseEquation:", "token too long!", NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  if(parseMode == EQUATION_PARSER_MVAR) {
    uint32_t tmpVal = 0;
    bufPtr = mvarBuffer;
    while(*bufPtr != 0) {
      bufPtr += stringByteLength(bufPtr) + 1;
      ++tmpVal;
    }
    for(; tmpVal < 5; ++tmpVal) {
      *(bufPtr++) = 0;
    }
    if(tmpVal == 5) {
      _menuF6(bufPtr);
    }
  }
  if(parseMode == EQUATION_PARSER_XEQ) {
    _processOperator(PARSER_OPERATOR_ITM_END_OF_FORMULA, mvarBuffer);
  }
#endif /* TESTSUITE_BUILD */
}
