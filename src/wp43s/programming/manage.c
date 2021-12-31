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
 * \file manage.c
 ***********************************************/

#include "programming/manage.h"

#include "bufferize.h"
#include "charString.h"
#include "config.h"
#include "dateTime.h"
#include "defines.h"
#include "flags.h"
#include "fonts.h"
#include "gui.h"
#include "items.h"
#include "memory.h"
#include "programming/decode.h"
#include "programming/lblGtoXeq.h"
#include "programming/nextStep.h"
#include "realType.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "screen.h"
#include "softmenus.h"
#include "sort.h"
#include <stdlib.h>
#include <string.h>

#include "wp43s.h"

// Structure of the program memory.
// In this example the RAM is 16384 blocks (from 0 to 16383) of 4 bytes = 65536 bytes.
// The program memory occupies the end of the RAM area.
//
//  +-----+------+----+----------+
//  |Block| Step |Code|    OP    |
//  |     |loc/gl|    |          |
//  +-----+------+----+----------+
//  |16374| 1/ 1 |   1| LBL 'P1' | <-- beginOfProgramMemory
//  |     |      | 253|          |
//  |     |      |   2|          |
//  |     |      | 'P'|          |
//  |16375|      | '1'|          |  ^
//  |     | 2/ 2 | 114| 1.       |  | 1 block = 4 bytes
//  |     |      |   6|          |  |
//  |     |      |   1|          |  v
//  |16376|      | '1'|          |
//  |     | 3/ 3 |  95| +        |
//  |     | 4/ 4 | 133| END      |
//  |     |      | 168|          |
//  |16377| 1/ 5 |   1| LBL 'P2' | <-- beginOfCurrentProgram  <-- firstDisplayedStep   firstDisplayedStepNumber = 5   firstDisplayedLocalStepNumber = 1
//  |     |      | 253|          |
//  |     |      |   2|          |
//  |     |      | 'P'|          |
//  |16378|      | '2'|          |
//  |     | 2/ 6 | 114| 22.      | <-- currentStep       this inequality is always true: beginOfCurrentProgram ≤ currentStep < endOfCurrentProgram
//  |     |      |   6|          |     currentStepNumber = 6   currentLocalStepNumber = 2
//  |     |      |   2|          |     currentProgramNumber = 2
//  |16379|      | '2'|          |
//  |     |      | '2'|          |
//  |     | 3/ 7 |  95| +        |
//  |     | 4/ 8 | 133| END      |
//  |16380|      | 168|          |
//  |     | 1/ 9 |   2| LBL 'P3' | <-- endOfCurrentProgram
//  |     |      | 253|          |
//  |     |      |   2|          |
//  |16381|      | 'P'|          |
//  |     |      | '3'|          |
//  |     | 2/10 | 114| 3.       |
//  |     |      |   6|          |
//  |16382|      |   1|          |
//  |     |      | '3'|          |
//  |     | 3/11 |  95| +        |
//  |     | 4/12 | 133| END      |
//  |16383| 5/13 | 168|          |
//  |     |      | 255| .END.    | <-- firstFreeProgramByte
//  |     |      | 255|          |
//  |     |      |   ?|          | free byte     This byte is the end of the RAM area
//  +-----+------+----+----------+
//
//  freeProgramBytes = 1



void scanLabelsAndPrograms(void) {
  uint32_t stepNumber = 0;
  uint8_t *nextStep, *step = beginOfProgramMemory;

  freeWp43s(labelList, TO_BLOCKS(sizeof(labelList_t)) * numberOfLabels);
  freeWp43s(programList, TO_BLOCKS(sizeof(programList_t)) * numberOfPrograms);

  numberOfLabels = 0;
  numberOfPrograms = 1;
  while(*step != 255 || *(step + 1) != 255) { // .END.
    if(*step == ITM_LBL) { // LBL
      numberOfLabels++;
    }
    if((*step & 0x7f) == (ITM_END >> 8) && *(step + 1) == (ITM_END & 0xff)) { // END
      numberOfPrograms++;
    }
    step = findNextStep(step);
  }

  labelList = allocWp43s(TO_BLOCKS(sizeof(labelList_t)) * numberOfLabels);
  if(labelList == NULL) {
    // unlikely
    lastErrorCode = ERROR_RAM_FULL;
    return;
  }

  programList = allocWp43s(TO_BLOCKS(sizeof(programList_t)) * numberOfPrograms);
  if(programList == NULL) {
    // unlikely
    lastErrorCode = ERROR_RAM_FULL;
    return;
  }

  numberOfLabels = 0;
  step = beginOfProgramMemory;
  programList[0].instructionPointer = step;
  programList[0].step = (0 + 1);
  numberOfPrograms = 1;
  stepNumber = 1;
  while(*step != 255 || *(step + 1) != 255) { // .END.
    nextStep = findNextStep(step);
    if(*step == 1) { // LBL
      labelList[numberOfLabels].program = numberOfPrograms;
      if(*(step + 1) <= 109) { // Local label
        labelList[numberOfLabels].step = -stepNumber;
        labelList[numberOfLabels].labelPointer = step + 1;
      }
      else { // Global label
        labelList[numberOfLabels].step = stepNumber;
        labelList[numberOfLabels].labelPointer = step + 2;
      }

      labelList[numberOfLabels].instructionPointer = nextStep;
      numberOfLabels++;
    }

    if((*step & 0x7f) == (ITM_END >> 8) && *(step + 1) == (ITM_END & 0xff)) { // END
      programList[numberOfPrograms].instructionPointer = step + 2;
      programList[numberOfPrograms].step = stepNumber + 1;
      numberOfPrograms++;
    }

    step = nextStep;
    stepNumber++;
  }

  defineCurrentProgramFromCurrentStep();
}



void deleteStepsFromTo(uint8_t *from, uint8_t *to) {
  uint16_t opSize = to - from;

  xcopy(from, to, (firstFreeProgramByte - to) + 2);
  firstFreeProgramByte -= opSize;
  freeProgramBytes += opSize;
  scanLabelsAndPrograms();
}



void fnClPAll(uint16_t confirmation) {
  if(confirmation == NOT_CONFIRMED) {
    setConfirmationMode(fnClPAll);
  }
  else {
    resizeProgramMemory(1); // 1 block for an empty program
    *(beginOfProgramMemory + 0)   = (ITM_END >> 8) | 0x80;
    *(beginOfProgramMemory + 1)   =  ITM_END       & 0xff;
    *(beginOfProgramMemory + 2)   = 255; // .END.
    *(beginOfProgramMemory + 3)   = 255; // .END.
    firstFreeProgramByte          = beginOfProgramMemory + 2;
    freeProgramBytes              = 0;
    currentStep                   = beginOfProgramMemory;
    firstDisplayedStep            = beginOfProgramMemory;
    firstDisplayedLocalStepNumber = 0;
    currentLocalStepNumber        = 1;
    temporaryInformation          = TI_NO_INFO;
    programRunStop                = PGM_STOPPED;
    scanLabelsAndPrograms();
  }
}



void fnClP(uint16_t unusedButMandatoryParameter) {
  if(beginOfCurrentProgram == beginOfProgramMemory && *endOfCurrentProgram == 255 && *(endOfCurrentProgram + 1) == 255) { // There is only one program in memory
    fnClPAll(CONFIRMED);
  }
  else {
    uint16_t savedCurrentProgramNumber = currentProgramNumber;

    deleteStepsFromTo(beginOfCurrentProgram, endOfCurrentProgram - ((currentProgramNumber == numberOfPrograms) ? 2 : 0));
    scanLabelsAndPrograms();
    // unlikely fails

    if(savedCurrentProgramNumber >= numberOfPrograms) { // The last program
      fnGotoDot(programList[numberOfPrograms - 2].step);
    }
    else { // Not the last program
      fnGotoDot(programList[savedCurrentProgramNumber - 1].step);
    }
  }
}



static uint32_t _getProgramSize(void) {
  if(currentProgramNumber == numberOfPrograms) {
    uint16_t numberOfSteps = 1;
    uint8_t *step = programList[currentProgramNumber - 1].instructionPointer;
    while(*step != 255 || *(step + 1) != 255) { // .END.
      ++numberOfSteps;
      step = findNextStep(step);
    }
    return (uint32_t)(step - programList[currentProgramNumber - 1].instructionPointer + 2);
  }
  else {
    return (uint32_t)(programList[currentProgramNumber].instructionPointer - programList[currentProgramNumber - 1].instructionPointer);
  }
}



void defineCurrentProgramFromGlobalStepNumber(uint16_t globalStepNumber) {
  currentProgramNumber = 0;
  while(globalStepNumber >= programList[currentProgramNumber].step) {
    currentProgramNumber++;
    if(currentProgramNumber >= numberOfPrograms) break;
  }

  if(currentProgramNumber >= numberOfPrograms) {
    endOfCurrentProgram = programList[currentProgramNumber - 1].instructionPointer + _getProgramSize();
  }
  else {
    endOfCurrentProgram = programList[currentProgramNumber].instructionPointer;
  }
  beginOfCurrentProgram = programList[currentProgramNumber - 1].instructionPointer;
}



void defineCurrentProgramFromCurrentStep(void) {
  currentProgramNumber = 0;
  while(currentStep >= programList[currentProgramNumber].instructionPointer) {
    currentProgramNumber++;
    if(currentProgramNumber >= numberOfPrograms) break;
  }

  if(currentProgramNumber >= numberOfPrograms) {
    endOfCurrentProgram = programList[currentProgramNumber - 1].instructionPointer + _getProgramSize();
  }
  else {
    endOfCurrentProgram = programList[currentProgramNumber].instructionPointer;
  }
  beginOfCurrentProgram = programList[currentProgramNumber - 1].instructionPointer;
}



void fnPem(uint16_t unusedButMandatoryParameter) {
  #ifndef TESTSUITE_BUILD
    ///////////////////////////////////////////////////////////////////////////////////////
    // For this function to work properly we need the following variables set properly:
    //  - currentProgramNumber
    //  - currentLocalStepNumber
    //  - firstDisplayedLocalStepNumber
    //  - firstDisplayedStep
    //
    uint32_t currentStepNumber, firstDisplayedStepNumber;
    uint16_t line, firstLine;
    uint16_t stepsThatWouldBeDisplayed = 7;
    uint8_t *step, *nextStep;
    bool_t lblOrEnd;

    if(calcMode != CM_PEM) {
      calcMode = CM_PEM;
      hourGlassIconEnabled = false;
      aimBuffer[0] = 0;
      currentInputVariable = INVALID_VARIABLE;
      return;
    }

    if(currentLocalStepNumber == 0) {
      currentLocalStepNumber = 1;
    }
    currentStepNumber        = currentLocalStepNumber        + programList[currentProgramNumber - 1].step - 1;
    firstDisplayedStepNumber = firstDisplayedLocalStepNumber + programList[currentProgramNumber - 1].step - 1;
    step                     = firstDisplayedStep;
    programListEnd           = false;
    lastProgramListEnd       = false;

    if(firstDisplayedLocalStepNumber == 0) {
      uint32_t numberOfSteps = (uint32_t)getNumberOfSteps();
      sprintf(tmpString, "{ Prgm #%d: %" PRIu32 " bytes / %" PRIu32 " step%s }", currentProgramNumber, _getProgramSize(),
                                                                               numberOfSteps, numberOfSteps == 1 ? "" : "s");
      showString(tmpString, &standardFont, 2, Y_POSITION_OF_REGISTER_T_LINE, vmNormal,  false, false);
      firstLine = 1;
    }
    else {
      firstLine = 0;
    }

    int lineOffset = 0;
    for(line=firstLine; line<7; line++) {
      nextStep = findNextStep(step);
      //uint16_t stepSize = (uint16_t)(nextStep - step);
      sprintf(tmpString, "%04d:" STD_SPACE_4_PER_EM, firstDisplayedLocalStepNumber + line - lineOffset);
      if(firstDisplayedStepNumber + line - lineOffset == currentStepNumber) {
        tamOverPemYPos = Y_POSITION_OF_REGISTER_T_LINE + 21 * line;
        showString(tmpString, &standardFont, 1, tamOverPemYPos, vmReverse, false, true);
        currentStep = step;
      }
      else {
        showString(tmpString, &standardFont, 1, Y_POSITION_OF_REGISTER_T_LINE + 21 * line, vmNormal,  false, true);
      }
      lblOrEnd = (*step == ITM_LBL) || ((*step == ((ITM_END >> 8) | 0x80)) && (*(step + 1) == (ITM_END & 0xff))) || ((*step == 0xff) && (*(step + 1) == 0xff));
      decodeOneStep(step);
      if(firstDisplayedStepNumber + line - lineOffset == currentStepNumber) {
        if(getSystemFlag(FLAG_ALPHA)) {
          char *tstr = tmpString + stringByteLength(tmpString) - 2;
          *(tstr++) = STD_CURSOR[0];
          *(tstr++) = STD_CURSOR[1];
          *(tstr++) = STD_RIGHT_SINGLE_QUOTE[0];
          *(tstr++) = STD_RIGHT_SINGLE_QUOTE[1];
          *(tstr++) = 0;
        }
        else if(aimBuffer[0] != 0) {
          char *tstr = tmpString + stringByteLength(tmpString);
          *(tstr++) = STD_CURSOR[0];
          *(tstr++) = STD_CURSOR[1];
          *(tstr++) = 0;
        }
      }

      // Split long lines
      int numberOfExtraLines = 0;
      int offset = 0;
      while(offset <= 1500 && stringWidth(tmpString + offset, &standardFont, false, false) > 337) {
        numberOfExtraLines++;
        xcopy(tmpString + 2100, tmpString + offset, stringByteLength(tmpString + offset) + 1);
        while(stringWidth(tmpString + offset, &standardFont, false, false) > 337) {
          tmpString[offset + stringLastGlyph(tmpString + offset)] = 0;
        }
        xcopy(tmpString + offset + 300, tmpString + 2100 + stringByteLength(tmpString + offset), stringByteLength(tmpString + 2100 + stringByteLength(tmpString + offset)) + 1);
        offset += 300;
      }
      stepsThatWouldBeDisplayed -= numberOfExtraLines;

      showString(tmpString, &standardFont, lblOrEnd ? 42 : 62, Y_POSITION_OF_REGISTER_T_LINE + 21 * line, vmNormal,  false, false);
      offset = 300;
      while(numberOfExtraLines && line <= 5) {
        showString(tmpString + offset, &standardFont, 62, Y_POSITION_OF_REGISTER_T_LINE + 21 * (++line), vmNormal,  false, false);
        numberOfExtraLines--;
        offset += 300;
        lineOffset++;
      }

      if((*step == ((ITM_END >> 8) | 0x80)) && (*(step + 1) == (ITM_END & 0xff))) {
        programListEnd = true;
        if(*nextStep == 255 && *(nextStep + 1) == 255) {
          lastProgramListEnd = true;
        }
        break;
      }
      if((*step == 255) && (*(step + 1) == 255)) {
        programListEnd = true;
        lastProgramListEnd = true;
        break;
      }
      step = nextStep;
    }

    if(currentLocalStepNumber >= (firstDisplayedLocalStepNumber + stepsThatWouldBeDisplayed)) {
      firstDisplayedLocalStepNumber = currentLocalStepNumber - stepsThatWouldBeDisplayed + 1;
      firstDisplayedStep = programList[currentProgramNumber - 1].instructionPointer;
      for(uint16_t i = 1; i < firstDisplayedLocalStepNumber; ++i) {
        firstDisplayedStep = findNextStep(firstDisplayedStep);
      }
      clearScreen();
      showSoftmenuCurrentPart();
      fnPem(NOPARAM);
    }
  #endif // TESTSUITE_BUILD
}



static void _insertInProgram(const uint8_t *dat, uint16_t size) {
  int16_t _dynamicMenuItem = dynamicMenuItem;
  uint16_t globalStepNumber;
  if(freeProgramBytes < size) {
    uint8_t *oldBeginOfProgramMemory = beginOfProgramMemory;
    uint32_t programSizeInBlocks = RAM_SIZE - freeMemoryRegions[numberOfFreeMemoryRegions - 1].address - freeMemoryRegions[numberOfFreeMemoryRegions - 1].sizeInBlocks;
    uint32_t newProgramSizeInBlocks = TO_BLOCKS(TO_BYTES(programSizeInBlocks) - freeProgramBytes + size);
    freeProgramBytes      += TO_BYTES(newProgramSizeInBlocks - programSizeInBlocks);
    resizeProgramMemory(newProgramSizeInBlocks);
    fflush(stdout);
    currentStep           = currentStep           - oldBeginOfProgramMemory + beginOfProgramMemory;
    firstDisplayedStep    = firstDisplayedStep    - oldBeginOfProgramMemory + beginOfProgramMemory;
    beginOfCurrentProgram = beginOfCurrentProgram - oldBeginOfProgramMemory + beginOfProgramMemory;
    endOfCurrentProgram   = endOfCurrentProgram   - oldBeginOfProgramMemory + beginOfProgramMemory;
  }
  for(uint8_t *pos = firstFreeProgramByte + 1 + size; pos > currentStep; --pos) {
    *pos = *(pos - size);
  }
  for(uint16_t i = 0; i < size; ++i) {
    *(currentStep++) = *(dat++);
  }
  firstFreeProgramByte   += size;
  freeProgramBytes       -= size;
  currentLocalStepNumber += 1;
  endOfCurrentProgram    += size;
  globalStepNumber = currentLocalStepNumber + programList[currentProgramNumber - 1].step - 1;
  scanLabelsAndPrograms();
  dynamicMenuItem = -1;
  fnGotoDot(globalStepNumber);
  dynamicMenuItem = _dynamicMenuItem;
}

void pemAlpha(int16_t item) {
#ifndef TESTSUITE_BUILD
  if(!getSystemFlag(FLAG_ALPHA)) {
    shiftF = false;
    shiftG = false;
    aimBuffer[0] = 0;
    alphaCase = AC_UPPER;
    nextChar = NC_NORMAL;

    if(softmenuStack[0].softmenuId == 0) { // MyMenu
      softmenuStack[0].softmenuId = 1; // MyAlpha
    }

    setSystemFlag(FLAG_ALPHA);

    #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
      calcModeAimGui();
    #endif // PC_BUILD && (SCREEN_800X480 == 0)

    tmpString[0] = ITM_LITERAL;
    tmpString[1] = (char)STRING_LABEL_VARIABLE;
    tmpString[2] = 0;
    _insertInProgram((uint8_t *)tmpString, 3);
    --currentLocalStepNumber;
    currentStep = findPreviousStep(currentStep);
  }
  if(indexOfItems[item].func == addItemToBuffer) {
    int32_t len = stringByteLength(aimBuffer);
    if(alphaCase == AC_LOWER) {
      if     (ITM_A     <= item && item <= ITM_Z    ) item += 26;
      else if(ITM_ALPHA <= item && item <= ITM_OMEGA) item += 36;
    }
    item = convertItemToSubOrSup(item, nextChar);
    xcopy(aimBuffer + len, indexOfItems[item].itemSoftmenuName, stringByteLength(indexOfItems[item].itemSoftmenuName) + 1);
  }
  else if(item == ITM_DOWN_ARROW) {
    nextChar = NC_SUBSCRIPT;
  }
  else if(item == ITM_UP_ARROW) {
    nextChar = NC_SUPERSCRIPT;
  }
  else if(item == ITM_BACKSPACE) {
    if(aimBuffer[0] == 0) {
      deleteStepsFromTo(currentStep, findNextStep(currentStep));
      clearSystemFlag(FLAG_ALPHA);
      #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
        calcModeNormalGui();
      #endif // PC_BUILD && (SCREEN_800X480 == 0)
      return;
    }
    else {
      aimBuffer[stringLastGlyph(aimBuffer)] = 0;
    }
  }
  else if(item == ITM_ENTER) {
    pemCloseAlphaInput();
    tmpString[0] = ITM_ENTER;
    _insertInProgram((uint8_t *)tmpString, 1);
    return;
  }

  deleteStepsFromTo(currentStep, findNextStep(currentStep));
  tmpString[0] = ITM_LITERAL;
  tmpString[1] = (char)STRING_LABEL_VARIABLE;
  tmpString[2] = stringByteLength(aimBuffer);
  xcopy(tmpString + 3, aimBuffer, stringByteLength(aimBuffer));
  _insertInProgram((uint8_t *)tmpString, stringByteLength(aimBuffer) + 3);
  --currentLocalStepNumber;
  currentStep = findPreviousStep(currentStep);
#endif // TESTSUITE_BUILD
}

void pemCloseAlphaInput(void) {
#ifndef TESTSUITE_BUILD
  aimBuffer[0] = 0;
  clearSystemFlag(FLAG_ALPHA);
  #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
    calcModeNormalGui();
  #endif // PC_BUILD && (SCREEN_800X480 == 0)
  ++currentLocalStepNumber;
  currentStep = findNextStep(currentStep);
#endif // TESTSUITE_BUILD
}

void pemAddNumber(int16_t item) {
#ifndef TESTSUITE_BUILD
  if(aimBuffer[0] == 0) {
    tmpString[0] = ITM_LITERAL;
    tmpString[1] = STRING_LONG_INTEGER;
    tmpString[2] = 0;
    _insertInProgram((uint8_t *)tmpString, 3);
    memset(nimBufferDisplay, 0, NIM_BUFFER_LENGTH);
    --currentLocalStepNumber;
    currentStep = findPreviousStep(currentStep);
    switch(item) {
      case ITM_EXPONENT :
        aimBuffer[0] = '+';
        aimBuffer[1] = '1';
        aimBuffer[2] = '.';
        aimBuffer[3] = 0;
        nimNumberPart = NP_REAL_FLOAT_PART;
        lastIntegerBase = 0;
        break;

      case ITM_PERIOD :
        aimBuffer[0] = '+';
        aimBuffer[1] = '0';
        aimBuffer[2] = 0;
        nimNumberPart = NP_INT_10;
        break;

      case ITM_0 :
      case ITM_1 :
      case ITM_2 :
      case ITM_3 :
      case ITM_4 :
      case ITM_5 :
      case ITM_6 :
      case ITM_7 :
      case ITM_8 :
      case ITM_9 :
      case ITM_A :
      case ITM_B :
      case ITM_C :
      case ITM_D :
      case ITM_E :
      case ITM_F :
        aimBuffer[0] = '+';
        aimBuffer[1] = 0;
        nimNumberPart = NP_EMPTY;
        break;
    }
  }
  if(item == ITM_BACKSPACE && ((aimBuffer[0] == '+' && aimBuffer[1] != 0 && aimBuffer[2] == 0) || aimBuffer[1] == 0)) {
    aimBuffer[0] = 0;
  }
  else {
    addItemToNimBuffer(item);
  }
  clearSystemFlag(FLAG_ALPHA);

  if(aimBuffer[0] != '!') {
    deleteStepsFromTo(currentStep, findNextStep(currentStep));
    if(aimBuffer[0] != 0) {
      const char *numBuffer = aimBuffer[0] == '+' ? aimBuffer + 1 : aimBuffer;
      tmpString[0] = ITM_LITERAL;
      switch(nimNumberPart) {
        //case NP_INT_16:
        //case NP_INT_BASE:
        //  tmpString[1] = STRING_SHORT_INTEGER;
        //  break;
        case NP_REAL_FLOAT_PART:
        case NP_REAL_EXPONENT:
        case NP_FRACTION_DENOMINATOR:
          tmpString[1] = STRING_REAL34;
          break;
        case NP_COMPLEX_INT_PART:
        case NP_COMPLEX_FLOAT_PART:
        case NP_COMPLEX_EXPONENT:
          tmpString[1] = STRING_COMPLEX34;
          break;
        default:
          tmpString[1] = STRING_LONG_INTEGER;
          break;
      }
      tmpString[2] = stringByteLength(numBuffer);
      xcopy(tmpString + 3, numBuffer, stringByteLength(numBuffer));
      _insertInProgram((uint8_t *)tmpString, stringByteLength(numBuffer) + 3);
      --currentLocalStepNumber;
      currentStep = findPreviousStep(currentStep);
    }
    calcMode = CM_PEM;
  }
  else {
    aimBuffer[0] = 0;
  }
#endif // TESTSUITE_BUILD
}

void pemCloseNumberInput(void) {
#ifndef TESTSUITE_BUILD
  deleteStepsFromTo(currentStep, findNextStep(currentStep));
  if(aimBuffer[0] != 0) {
    char *numBuffer = aimBuffer[0] == '+' ? aimBuffer + 1 : aimBuffer;
    char *basePtr = numBuffer;
    char *tmpPtr = tmpString;
    *(tmpPtr++) = ITM_LITERAL;
    switch(nimNumberPart) {
      //case NP_INT_16:
      case NP_INT_BASE:
        *(tmpPtr++) = STRING_SHORT_INTEGER;
        while(*basePtr != '#') ++basePtr;
        *(basePtr++) = 0;
        *(tmpPtr++) = (char)atoi(basePtr);
        fflush(stdout);
        break;
      case NP_REAL_FLOAT_PART:
      case NP_REAL_EXPONENT:
      case NP_FRACTION_DENOMINATOR:
        *(tmpPtr++) = STRING_REAL34;
        break;
      case NP_COMPLEX_INT_PART:
      case NP_COMPLEX_FLOAT_PART:
      case NP_COMPLEX_EXPONENT:
        *(tmpPtr++) = STRING_COMPLEX34;
        break;
      default:
        *(tmpPtr++) = STRING_LONG_INTEGER;
        break;
    }
    *(tmpPtr++) = stringByteLength(numBuffer);
    xcopy(tmpPtr, numBuffer, stringByteLength(numBuffer));
    _insertInProgram((uint8_t *)tmpString, stringByteLength(numBuffer) + (int32_t)(tmpPtr - tmpString));
  }

  aimBuffer[0] = '!';
#endif // TESTSUITE_BUILD
}

static void _pemCloseTimeInput(void) {
#ifndef TESTSUITE_BUILD
  switch(nimNumberPart) {
    case NP_INT_10:
    case NP_REAL_FLOAT_PART:
      deleteStepsFromTo(currentStep, findNextStep(currentStep));
      if(aimBuffer[0] != 0) {
        char *numBuffer = aimBuffer[0] == '+' ? aimBuffer + 1 : aimBuffer;
        char *tmpPtr = tmpString;
        *(tmpPtr++) = ITM_LITERAL;
        *(tmpPtr++) = STRING_TIME;
        *(tmpPtr++) = stringByteLength(numBuffer);
        xcopy(tmpPtr, numBuffer, stringByteLength(numBuffer));
        _insertInProgram((uint8_t *)tmpString, stringByteLength(numBuffer) + (int32_t)(tmpPtr - tmpString));
      }

      aimBuffer[0] = '!';
      break;
  }
#endif // TESTSUITE_BUILD
}

static void _pemCloseDateInput(void) {
#ifndef TESTSUITE_BUILD
  if(nimNumberPart == NP_REAL_FLOAT_PART) {
    deleteStepsFromTo(currentStep, findNextStep(currentStep));
    if(aimBuffer[0] != 0) {
      char *numBuffer = aimBuffer[0] == '+' ? aimBuffer + 1 : aimBuffer;
      char *tmpPtr = tmpString;
      *(tmpPtr++) = ITM_LITERAL;
      *(tmpPtr++) = STRING_DATE;

      reallocateRegister(TEMP_REGISTER_1, dtReal34, REAL34_SIZE, amNone);
      stringToReal34(numBuffer, REGISTER_REAL34_DATA(TEMP_REGISTER_1));
      convertReal34RegisterToDateRegister(TEMP_REGISTER_1, TEMP_REGISTER_1);
      internalDateToJulianDay(REGISTER_REAL34_DATA(TEMP_REGISTER_1), REGISTER_REAL34_DATA(TEMP_REGISTER_1));

      real34ToString(REGISTER_REAL34_DATA(TEMP_REGISTER_1), tmpPtr + 1);
      *tmpPtr = stringByteLength(tmpPtr + 1);
      ++tmpPtr;

      _insertInProgram((uint8_t *)tmpString, stringByteLength(tmpPtr) + (int32_t)(tmpPtr - tmpString));
    }

    aimBuffer[0] = '!';
  }
#endif // TESTSUITE_BUILD
}

void insertStepInProgram(int16_t func) {
  uint32_t opBytes = (func >= 128) ? 2 : 1;

  if(func == ITM_AIM || (!tam.mode && getSystemFlag(FLAG_ALPHA))) {
    pemAlpha(func);
    return;
  }
  if(indexOfItems[func].func == addItemToBuffer || (!tam.mode && aimBuffer[0] != 0 && (func == ITM_CHS || func == ITM_CC || func == ITM_toINT || (nimNumberPart == NP_INT_BASE && (func == ITM_YX || func == ITM_LN || func == ITM_RCL))))) {
    pemAddNumber(func);
    return;
  }
  if(!tam.mode && !tam.alpha && aimBuffer[0] != 0 && func != ITM_toHMS) {
    if(func == ITM_dotD) {
      _pemCloseDateInput();
      if(aimBuffer[0] == '!') {
        aimBuffer[0] = 0;
        return;
      }
    }
    pemCloseNumberInput();
    aimBuffer[0] = 0;
  }

  if(func < 128) {
    tmpString[0] = func;
  }
  else {
    tmpString[0] = (func >> 8) | 0x80;
    tmpString[1] =  func       & 0xff;
  }

  switch(indexOfItems[func].status & PTP_STATUS) {
    case PTP_DISABLED:
      switch(func) {
        case ITM_KEYG:           // 1498
        case ITM_KEYX:           // 1499
          {
            int opLen;
            tmpString[0] = (ITM_KEY >> 8) | 0x80;
            tmpString[1] =  ITM_KEY       & 0xff;
            if(tam.keyAlpha) {
              uint16_t nameLength = stringByteLength(aimBuffer + AIM_BUFFER_LENGTH / 2);
              tmpString[2] = (char)INDIRECT_VARIABLE;
              tmpString[3] = (char)nameLength;
              xcopy(tmpString + 4, aimBuffer + AIM_BUFFER_LENGTH / 2, nameLength);
              opLen = nameLength + 4;
            }
            else if(tam.keyIndirect) {
              tmpString[2] = (char)INDIRECT_REGISTER;
              tmpString[3] = tam.key;
              opLen = 4;
            }
            else {
              tmpString[2] = tam.key;
              opLen = 3;
            }

            tmpString[opLen + 0] = (func == ITM_KEYX ? ITM_XEQ : ITM_GTO);
            if(tam.alpha) {
              uint16_t nameLength = stringByteLength(aimBuffer);
              tmpString[opLen + 1] = (char)(tam.indirect ? INDIRECT_VARIABLE : STRING_LABEL_VARIABLE);
              tmpString[opLen + 2] = nameLength;
              xcopy(tmpString + opLen + 3, aimBuffer, nameLength);
              _insertInProgram((uint8_t *)tmpString, nameLength + opLen + 3);
            }
            else if(tam.indirect) {
              tmpString[opLen + 1] = (char)INDIRECT_REGISTER;
              tmpString[opLen + 2] = tam.value;
              _insertInProgram((uint8_t *)tmpString, opLen + 3);
            }
            else {
              tmpString[opLen + 1] = tam.value;
              _insertInProgram((uint8_t *)tmpString, opLen + 2);
            }
          }
          break;

        case ITM_GTOP:           // 1482
          #ifndef DMCP_BUILD
            stringToUtf8(indexOfItems[func].itemCatalogName, (uint8_t *)tmpString);
            printf("insertStepInProgram: %s\n", tmpString);
          #endif // DMCP_BUILD
          break;

        case ITM_CLP:            // 1425
          fnClP(NOPARAM);
          break;

        case ITM_CLPALL:         // 1426
          fnClPAll(NOT_CONFIRMED);
          break;

        case ITM_BST:            // 1734
          fnBst(NOPARAM);
          break;

        case ITM_SST:            // 1736
          fnSst(NOPARAM);
          break;
      }
      break;

    case PTP_NONE:
      if(func == ITM_toHMS && aimBuffer[0] != 0 && !getSystemFlag(FLAG_ALPHA)) {
        _pemCloseTimeInput();
        if(aimBuffer[0] != '!') {
          pemCloseNumberInput();
          aimBuffer[0] = 0;
          tmpString[0] = (func >> 8) | 0x80;
          tmpString[1] =  func       & 0xff;
          _insertInProgram((uint8_t *)tmpString, 2);
        }
        aimBuffer[0] = 0;
      }
      else {
        _insertInProgram((uint8_t *)tmpString, (func >= 128) ? 2 : 1);
      }
      break;

    case PTP_NUMBER_16:
      tmpString[2] = (char)(tam.value & 0xff); // little endian
      tmpString[3] = (char)(tam.value >> 8);
      _insertInProgram((uint8_t *)tmpString, 4);
      break;

    case PTP_LITERAL:
      // nothing to do here
      break;

    default:
      if(tam.mode == TM_CMP && tam.value == TEMP_REGISTER_1) {
        tmpString[opBytes    ] = (char)(real34IsZero(REGISTER_REAL34_DATA(TEMP_REGISTER_1)) ? VALUE_0 : VALUE_1);
        _insertInProgram((uint8_t *)tmpString, opBytes + 1);
      }
      else if((tam.mode == TM_FLAGR || tam.mode == TM_FLAGW) && tam.alpha && !tam.indirect) {
        tmpString[opBytes    ] = (char)SYSTEM_FLAG_NUMBER;
        tmpString[opBytes + 1] = tam.value;
        _insertInProgram((uint8_t *)tmpString, opBytes + 2);
      }
      else if(tam.alpha) {
        uint16_t nameLength = stringByteLength(aimBuffer);
        tmpString[opBytes    ] = (char)(tam.indirect ? INDIRECT_VARIABLE : STRING_LABEL_VARIABLE);
        tmpString[opBytes + 1] = nameLength;
        xcopy(tmpString + opBytes + 2, aimBuffer, nameLength);
        _insertInProgram((uint8_t *)tmpString, nameLength + opBytes + 2);
      }
      else if(tam.indirect) {
        tmpString[opBytes    ] = (char)INDIRECT_REGISTER;
        tmpString[opBytes + 1] = tam.value;
        _insertInProgram((uint8_t *)tmpString, opBytes + 2);
      }
      else {
        tmpString[opBytes    ] = tam.value;
        _insertInProgram((uint8_t *)tmpString, opBytes + 1);
      }
  }

  aimBuffer[0] = 0;
}



calcRegister_t findNamedLabel(const char *labelName) {
  for(uint16_t lbl = 0; lbl < numberOfLabels; lbl++) {
    if(labelList[lbl].step > 0) {
      xcopy(tmpString, labelList[lbl].labelPointer + 1, *(labelList[lbl].labelPointer));
      tmpString[*(labelList[lbl].labelPointer)] = 0;
      if(compareString(tmpString, labelName, CMP_NAME) == 0) {
        return lbl + FIRST_LABEL;
      }
    }
  }
  return INVALID_VARIABLE;
}



uint16_t getNumberOfSteps(void) {
  if(currentProgramNumber == numberOfPrograms) {
    uint16_t numberOfSteps = 1;
    uint8_t *step = programList[currentProgramNumber - 1].instructionPointer;
    while(*step != 255 || *(step + 1) != 255) { // .END.
      ++numberOfSteps;
      step = findNextStep(step);
    }
    return numberOfSteps;
  }
  else {
    return programList[currentProgramNumber].step - programList[currentProgramNumber - 1].step;
  }
}
