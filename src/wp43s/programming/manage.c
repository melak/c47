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
      lblOrEnd = (*step == ITM_LBL) || ((*step == ((ITM_END >> 8) | 0x80)) && (*(step + 1) == (ITM_END & 0xff)));
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
    tmpString[1] = STRING_LABEL_VARIABLE;
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
  tmpString[1] = STRING_LABEL_VARIABLE;
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

void insertStepInProgram(int16_t func) {
  if(func == ITM_AIM || (!tam.mode && getSystemFlag(FLAG_ALPHA))) {
    pemAlpha(func);
    return;
  }
  if(indexOfItems[func].func == addItemToBuffer || (!tam.mode && aimBuffer[0] != 0 && (func == ITM_CHS || func == ITM_CC || func == ITM_toINT || (nimNumberPart == NP_INT_BASE && (func == ITM_YX || func == ITM_LN || func == ITM_RCL))))) {
    pemAddNumber(func);
    return;
  }
  if(!tam.mode && !tam.alpha && aimBuffer[0] != 0) {
    pemCloseNumberInput();
    aimBuffer[0] = 0;
  }
  switch(func) {
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

    // Not programmable
    case ITM_ASSIGN:         // 1411
    case ITM_CLALL:          // 1419
    case ITM_CLREGS:         // 1427
    case ITM_EQ_DEL:         // 1463
    case ITM_EQ_EDI:         // 1464
    case ITM_EQ_NEW:         // 1465
    case ITM_LOAD:           // 1509
    case ITM_LOADP:          // 1510
    case ITM_LOADR:          // 1511
    case ITM_LOADSS:         // 1512
    case ITM_LOADSIGMA:      // 1513
    case ITM_M_DELR:         // 1525
    case ITM_M_EDI:          // 1529
    case ITM_M_EDIN:         // 1530
    case ITM_M_GOTO:         // 1532
    case ITM_M_GROW:         // 1533
    case ITM_M_INSR:         // 1534
    case ITM_M_OLD:          // 1537
    case ITM_M_WRAP:         // 1541
    case ITM_LOADV:          // 1552
    case ITM_PSTO:           // 1555
    case ITM_RBR:            // 1560
    case ITM_RESET:          // 1568
    case ITM_SAVE:           // 1586
    case ITM_STATUS:         // 1610
    case ITM_TIMER:          // 1622
    case ITM_VERS:           // 1631
    case ITM_WHO:            // 1634
    case ITM_FBR:            // 1722
    case ITM_UNDO:           // 1723
    case ITM_SYSTEM:         // 1743
      break;

    // Single-byte, label parameter
    case ITM_LBL:            //    1
    case ITM_GTO:            //    2
    case ITM_XEQ:            //    3

    // Single-byte, 8-bit integer parameter
    case ITM_PAUSE:          //   38

    // Single-byte, comparison parameter
    case ITM_XEQU:           //   11
    case ITM_XNE:            //   12
    case ITM_XAEQU:          //   15
    case ITM_XLT:            //   16
    case ITM_XLE:            //   17
    case ITM_XGE:            //   18
    case ITM_XGT:            //   19

    // Single-byte, register parameter
    case ITM_ISE:            //    5
    case ITM_ISG:            //    6
    case ITM_ISZ:            //    7
    case ITM_DSE:            //    8
    case ITM_DSL:            //    9
    case ITM_DSZ:            //   10
    case ITM_INPUT:          //   43
    case ITM_STO:            //   44
    case ITM_STOADD:         //   45
    case ITM_STOSUB:         //   46
    case ITM_STOMULT:        //   47
    case ITM_STODIV:         //   48
    case ITM_RCL:            //   51
    case ITM_RCLADD:         //   52
    case ITM_RCLSUB:         //   53
    case ITM_RCLMULT:        //   54
    case ITM_RCLDIV:         //   55
    case ITM_CONVG:          //   56
    case ITM_KEYQ:           //   77
    case ITM_DEC:            //   91
    case ITM_INC:            //   92
    case ITM_VIEW:           //  101
    case ITM_Xex:            //  127

    // Single-byte, flag parameter
    case ITM_FC:             //   20
    case ITM_FS:             //   21
    case ITM_CF:             //  110
    case ITM_SF:             //  111
    case ITM_FF:             //  112

      if(tam.mode == TM_CMP && tam.value == TEMP_REGISTER_1) {
        tmpString[0] = func;
        tmpString[1] = real34IsZero(REGISTER_REAL34_DATA(TEMP_REGISTER_1)) ? VALUE_0 : VALUE_1;
        _insertInProgram((uint8_t *)tmpString, 2);
      }
      else if((tam.mode == TM_FLAGR || tam.mode == TM_FLAGW) && tam.alpha) {
        // not implemented
      }
      else if(tam.alpha) {
        uint16_t nameLength = stringByteLength(aimBuffer);
        tmpString[0] = func;
        tmpString[1] = tam.indirect ? INDIRECT_VARIABLE : STRING_LABEL_VARIABLE;
        tmpString[2] = nameLength;
        xcopy(tmpString + 3, aimBuffer, nameLength);
        _insertInProgram((uint8_t *)tmpString, nameLength + 3);
      }
      else if(tam.indirect) {
        tmpString[0] = func;
        tmpString[1] = INDIRECT_REGISTER;
        tmpString[2] = tam.value;
        _insertInProgram((uint8_t *)tmpString, 3);
      }
      else {
        tmpString[0] = func;
        tmpString[1] = tam.value;
        _insertInProgram((uint8_t *)tmpString, 2);
      }
      break;

    // Single-byte, no parameters
    case ITM_RTN:            //    4
    case ITM_XEQUP0:         //   13
    case ITM_XEQUM0:         //   14
    case ITM_EVEN:           //   22
    case ITM_ODD:            //   23
    case ITM_FPQ:            //   24
    case ITM_INT:            //   25
    case ITM_CPX:            //   26
    case ITM_MATR:           //   27
    case ITM_NAN:            //   28
    case ITM_REAL:           //   29
    case ITM_SPEC:           //   30
    case ITM_STRI:           //   31
    case ITM_PMINFINITY:     //   32
    case ITM_PRIME:          //   33
    case ITM_TOP:            //   34
    case ITM_ENTER:          //   35
    case ITM_XexY:           //   36
    case ITM_DROP:           //   37
    case ITM_Rup:            //   39
    case ITM_Rdown:          //   40
    case ITM_CLX:            //   41
    case ITM_FILL:           //   42
    case ITM_COMB:           //   49
    case ITM_PERM:           //   50
    case ITM_ENTRY:          //   57
    case ITM_SQUARE:         //   58
    case ITM_CUBE:           //   59
    case ITM_YX:             //   60
    case ITM_SQUAREROOTX:    //   61
    case ITM_CUBEROOT:       //   62
    case ITM_XTHROOT:        //   63
    case ITM_2X:             //   64
    case ITM_EXP:            //   65
    case ITM_ROUND:          //   66
    case ITM_10x:            //   67
    case ITM_LOG2:           //   68
    case ITM_LN:             //   69
    case ITM_STOP:           //   70
    case ITM_LOG10:          //   71
    case ITM_LOGXY:          //   72
    case ITM_1ONX:           //   73
    case ITM_cos:            //   74
    case ITM_cosh:           //   75
    case ITM_sin:            //   76
    case ITM_sinh:           //   78
    case ITM_tan:            //   79
    case ITM_tanh:           //   80
    case ITM_arccos:         //   81
    case ITM_arcosh:         //   82
    case ITM_arcsin:         //   83
    case ITM_arsinh:         //   84
    case ITM_arctan:         //   85
    case ITM_artanh:         //   86
    case ITM_CEIL:           //   87
    case ITM_FLOOR:          //   88
    case ITM_GCD:            //   89
    case ITM_LCM:            //   90
    case ITM_IP:             //   93
    case ITM_FP:             //   94
    case ITM_ADD:            //   95
    case ITM_SUB:            //   96
    case ITM_CHS:            //   97
    case ITM_MULT:           //   98
    case ITM_DIV:            //   99
    case ITM_IDIV:           //  100
    case ITM_MOD:            //  102
    case ITM_MAX:            //  103
    case ITM_MIN:            //  104
    case ITM_MAGNITUDE:      //  105
    case ITM_NEIGHB:         //  106
    case ITM_NEXTP:          //  107
    case ITM_XFACT:          //  108
    case ITM_CONSTpi:        //  109
    case ITM_M_SQR:          //  113
    case ITM_toDEG:          //  115
    case ITM_toDMS:          //  116
    case ITM_toGRAD:         //  117
    case ITM_toMULpi:        //  118
    case ITM_toRAD:          //  119
    case ITM_DtoR:           //  120
    case ITM_RtoD:           //  121
    case ITM_RMD:            //  122
    case ITM_LOGICALNOT:     //  123
    case ITM_LOGICALAND:     //  124
    case ITM_LOGICALOR:      //  125
    case ITM_LOGICALXOR:     //  126
      tmpString[0] = func;
      _insertInProgram((uint8_t *)tmpString, 1);
      break;

    // Double-byte, 16-bit integer parameter
    case ITM_BESTF:          // 1297
      tmpString[0] = (func >> 8) | 0x80;
      tmpString[1] =  func       & 0xff;
      tmpString[2] = (char)(tam.value & 0xff); // little endian
      tmpString[3] = (char)(tam.value >> 8);
      _insertInProgram((uint8_t *)tmpString, 4);
      break;

    // Double-byte, 8-bit integer parameter
    case ITM_CNST:           //  207
    case ITM_RL:             //  410
    case ITM_RLC:            //  411
    case ITM_RR:             //  412
    case ITM_RRC:            //  413
    case ITM_SL:             //  414
    case ITM_SR:             //  415
    case ITM_ASR:            //  416
    case ITM_MASKL:          //  419
    case ITM_MASKR:          //  420
    case ITM_SDL:            //  423
    case ITM_SDR:            //  424
    case ITM_AGRAPH:         // 1409
    case ITM_ALL:            // 1410
    case ITM_BACK:           // 1412
    case ITM_DSTACK:         // 1450
    case ITM_ENG:            // 1460
    case ITM_ERR:            // 1468
    case ITM_FIX:            // 1473
    case ITM_GAP:            // 1477
    case ITM_KEY:            // 1497
    case ITM_LocR:           // 1514
    case ITM_RDP:            // 1565
    case ITM_RM:             // 1571
    case ITM_DSP:            // 1573
    case ITM_RSD:            // 1577
    case ITM_SCI:            // 1587
    case ITM_SIM_EQ:         // 1602
    case ITM_SKIP:           // 1603
    case ITM_TDISP:          // 1619
    case ITM_TONE:           // 1624
    case ITM_WSIZE:          // 1638
    case ITM_toINT:          // 1687
    case ITM_SHUFFLE:        // 1694
    case ITM_PRINTERCHAR:    // 1709
    case ITM_PRINTERDLAY:    // 1710
    case ITM_PRINTERMODE:    // 1712
    case ITM_PRINTERTAB:     // 1717

    // Double-byte, label parameter
    case ITM_DELITM:         // 1455
    case ITM_FQX:            // 1475
    case ITM_FDQX:           // 1476
    case ITM_INDEX:          // 1486
    case ITM_LBLQ:           // 1503
    case ITM_MVAR:           // 1524
    case ITM_PGMINT:         // 1546
    case ITM_PGMSLV:         // 1547
    case ITM_SOLVE:          // 1608
    case ITM_VARMNU:         // 1630
    case ITM_PIn:            // 1671
    case ITM_SIGMAn:         // 1672
    case ITM_INTEGRAL:       // 1700

    // Double-byte, register parameter
    case ITM_CASE:           // 1418
    case ITM_STOMAX:         // 1430
    case ITM_RCLMAX:         // 1432
    case ITM_RCLMIN:         // 1462
    case ITM_KTYP:           // 1501
    case ITM_STOMIN:         // 1545
    case ITM_PUTK:           // 1556
    case ITM_RCLCFG:         // 1561
    case ITM_RCLS:           // 1564
    case ITM_STOCFG:         // 1611
    case ITM_STOS:           // 1615
    case ITM_Tex:            // 1625
    case ITM_Yex:            // 1650
    case ITM_Zex:            // 1651
    case ITM_ALPHALENG:      // 1652
    case ITM_ALPHAPOS:       // 1655
    case ITM_ALPHARL:        // 1656
    case ITM_ALPHARR:        // 1657
    case ITM_ALPHASL:        // 1658
    case ITM_ALPHASR:        // 1659
    case ITM_ALPHAtoX:       // 1660
    case ITM_PRINTERR:       // 1714

    // Double-byte, flag parameter
    case ITM_FCC:            //  396
    case ITM_FCS:            //  397
    case ITM_FCF:            //  398
    case ITM_FSC:            //  399
    case ITM_FSS:            //  400
    case ITM_FSF:            //  401
    case ITM_BS:             //  405
    case ITM_BC:             //  406
    case ITM_CB:             //  407
    case ITM_SB:             //  408
    case ITM_FB:             //  409

      if(tam.mode == TM_CMP && tam.value == TEMP_REGISTER_1) {
        tmpString[0] = (func >> 8) | 0x80;
        tmpString[1] =  func       & 0xff;
        tmpString[2] = real34IsZero(REGISTER_REAL34_DATA(TEMP_REGISTER_1)) ? VALUE_0 : VALUE_1;
        _insertInProgram((uint8_t *)tmpString, 3);
      }
      else if((tam.mode == TM_FLAGR || tam.mode == TM_FLAGW) && tam.alpha) {
        // not implemented
      }
      else if(tam.alpha) {
        uint16_t nameLength = stringByteLength(aimBuffer);
        tmpString[0] = (func >> 8) | 0x80;
        tmpString[1] =  func       & 0xff;
        tmpString[2] = tam.indirect ? INDIRECT_VARIABLE : STRING_LABEL_VARIABLE;
        tmpString[3] = nameLength;
        xcopy(tmpString + 4, aimBuffer, nameLength);
        _insertInProgram((uint8_t *)tmpString, nameLength + 4);
      }
      else if(tam.indirect) {
        tmpString[0] = (func >> 8) | 0x80;
        tmpString[1] =  func       & 0xff;
        tmpString[2] = INDIRECT_REGISTER;
        tmpString[3] = tam.value;
        _insertInProgram((uint8_t *)tmpString, 4);
      }
      else {
        tmpString[0] = (func >> 8) | 0x80;
        tmpString[1] =  func       & 0xff;
        tmpString[2] = tam.value;
        _insertInProgram((uint8_t *)tmpString, 3);
      }
      break;

    // Double-byte, no parameters
    case CST_01:             //  128
    case CST_02:             //  129
    case CST_03:             //  130
    case CST_04:             //  131
    case CST_05:             //  132
    case CST_06:             //  133
    case CST_07:             //  134
    case CST_08:             //  135
    case CST_09:             //  136
    case CST_10:             //  137
    case CST_11:             //  138
    case CST_12:             //  139
    case CST_13:             //  140
    case CST_14:             //  141
    case CST_15:             //  142
    case CST_16:             //  143
    case CST_17:             //  144
    case CST_18:             //  145
    case CST_19:             //  146
    case CST_20:             //  147
    case CST_21:             //  148
    case CST_22:             //  149
    case CST_23:             //  150
    case CST_24:             //  151
    case CST_25:             //  152
    case CST_26:             //  153
    case CST_27:             //  154
    case CST_28:             //  155
    case CST_29:             //  156
    case CST_30:             //  157
    case CST_31:             //  158
    case CST_32:             //  159
    case CST_33:             //  160
    case CST_34:             //  161
    case CST_35:             //  162
    case CST_36:             //  163
    case CST_37:             //  164
    case CST_38:             //  165
    case CST_39:             //  166
    case CST_40:             //  167
    case CST_41:             //  168
    case CST_42:             //  169
    case CST_43:             //  170
    case CST_44:             //  171
    case CST_45:             //  172
    case CST_46:             //  173
    case CST_47:             //  174
    case CST_48:             //  175
    case CST_49:             //  176
    case CST_50:             //  177
    case CST_51:             //  178
    case CST_52:             //  179
    case CST_53:             //  180
    case CST_54:             //  181
    case CST_55:             //  182
    case CST_56:             //  183
    case CST_57:             //  184
    case CST_58:             //  185
    case CST_59:             //  186
    case CST_60:             //  187
    case CST_61:             //  188
    case CST_62:             //  189
    case CST_63:             //  190
    case CST_64:             //  191
    case CST_65:             //  192
    case CST_66:             //  193
    case CST_67:             //  194
    case CST_68:             //  195
    case CST_69:             //  196
    case CST_70:             //  197
    case CST_71:             //  198
    case CST_72:             //  199
    case CST_73:             //  200
    case CST_74:             //  201
    case CST_75:             //  202
    case CST_76:             //  203
    case CST_77:             //  204
    case CST_78:             //  205
    case CST_79:             //  206
    case ITM_CtoF:           //  220
    case ITM_FtoC:           //  221
    case ITM_DBtoPR:         //  222
    case ITM_DBtoPRb:        //  223
    case ITM_DBtoPRc:        //  224
    case ITM_DBtoFR:         //  225
    case ITM_DBtoFRb:        //  226
    case ITM_DBtoFRc:        //  227
    case ITM_PRtoDB:         //  228
    case ITM_PRtoDBb:        //  229
    case ITM_PRtoDBc:        //  230
    case ITM_FRtoDB:         //  231
    case ITM_FRtoDBb:        //  232
    case ITM_FRtoDBc:        //  233
    case ITM_ACtoHA:         //  234
    case ITM_ACtoHAb:        //  235
    case ITM_HAtoAC:         //  236
    case ITM_HAtoACb:        //  237
    case ITM_ACUStoHA:       //  238
    case ITM_ACUStoHAb:      //  239
    case ITM_HAtoACUS:       //  240
    case ITM_HAtoACUSb:      //  241
    case ITM_PAtoATM:        //  242
    case ITM_ATMtoPA:        //  243
    case ITM_AUtoM:          //  244
    case ITM_MtoAU:          //  245
    case ITM_BARtoPA:        //  246
    case ITM_PAtoBAR:        //  247
    case ITM_BTUtoJ:         //  248
    case ITM_JtoBTU:         //  249
    case ITM_CALtoJ:         //  250
    case ITM_JtoCAL:         //  251
    case ITM_LBFFTtoNM:      //  252
    case ITM_LBFFTtoNMb:     //  253
    case ITM_NMtoLBFFT:      //  254
    case ITM_NMtoLBFFTb:     //  255
    case ITM_CWTtoKG:        //  256
    case ITM_KGtoCWT:        //  257
    case ITM_FTtoM:          //  258
    case ITM_MtoFT:          //  259
    case ITM_FTUStoM:        //  260
    case ITM_FTUStoMb:       //  261
    case ITM_FTUStoMc:       //  262
    case ITM_MtoFTUS:        //  263
    case ITM_MtoFTUSb:       //  264
    case ITM_MtoFTUSc:       //  265
    case ITM_FZUKtoML:       //  266
    case ITM_FZUKtoMLb:      //  267
    case ITM_MLtoFZUK:       //  268
    case ITM_MLtoFZUKb:      //  269
    case ITM_FZUStoML:       //  270
    case ITM_FZUStoMLb:      //  271
    case ITM_MLtoFZUS:       //  272
    case ITM_MLtoFZUSb:      //  273
    case ITM_GLUKtoL:        //  274
    case ITM_LtoGLUK:        //  275
    case ITM_GLUStoL:        //  276
    case ITM_LtoGLUS:        //  277
    case ITM_HPEtoW:         //  278
    case ITM_WtoHPE:         //  279
    case ITM_HPMtoW:         //  280
    case ITM_WtoHPM:         //  281
    case ITM_HPUKtoW:        //  282
    case ITM_WtoHPUK:        //  283
    case ITM_INCHHGtoPA:     //  284
    case ITM_INCHHGtoPAb:    //  285
    case ITM_PAtoINCHHG:     //  286
    case ITM_PAtoINCHHGb:    //  287
    case ITM_INCHtoMM:       //  288
    case ITM_MMtoINCH:       //  289
    case ITM_WHtoJ:          //  290
    case ITM_JtoWH:          //  291
    case ITM_KGtoLBS:        //  292
    case ITM_LBStoKG:        //  293
    case ITM_GtoOZ:          //  294
    case ITM_OZtoG:          //  295
    case ITM_KGtoSCW:        //  296
    case ITM_KGtoSCWb:       //  297
    case ITM_SCWtoKG:        //  298
    case ITM_SCWtoKGb:       //  299
    case ITM_KGtoSTO:        //  300
    case ITM_KGtoSTOb:       //  301
    case ITM_STOtoKG:        //  302
    case ITM_STOtoKGb:       //  303
    case ITM_KGtoST:         //  304
    case ITM_KGtoSTb:        //  305
    case ITM_KGtoSTc:        //  306
    case ITM_STtoKG:         //  307
    case ITM_STtoKGb:        //  308
    case ITM_STtoKGc:        //  309
    case ITM_KGtoTON:        //  310
    case ITM_KGtoLIANG:      //  311
    case ITM_KGtoLIANGb:     //  312
    case ITM_TONtoKG:        //  313
    case ITM_LIANGtoKG:      //  314
    case ITM_LIANGtoKGb:     //  315
    case ITM_GtoTRZ:         //  316
    case ITM_GtoTRZb:        //  317
    case ITM_TRZtoG:         //  318
    case ITM_TRZtoGb:        //  319
    case ITM_LBFtoN:         //  320
    case ITM_NtoLBF:         //  321
    case ITM_LYtoM:          //  322
    case ITM_MtoLY:          //  323
    case ITM_MMHGtoPA:       //  324
    case ITM_MMHGtoPAb:      //  325
    case ITM_PAtoMMHG:       //  326
    case ITM_PAtoMMHGb:      //  327
    case ITM_MItoKM:         //  328
    case ITM_KMtoMI:         //  329
    case ITM_KMtoNMI:        //  330
    case ITM_NMItoKM:        //  331
    case ITM_MtoPC:          //  332
    case ITM_PCtoM:          //  333
    case ITM_MMtoPOINTb:     //  334
    case ITM_MMtoPOINT:      //  335
    case ITM_MMtoPOINTc:     //  336
    case ITM_POINTtoMM:      //  337
    case ITM_POINTtoMMb:     //  338
    case ITM_POINTtoMMc:     //  339
    case ITM_MtoYD:          //  340
    case ITM_YDtoM:          //  341
    case ITM_PSItoPA:        //  342
    case ITM_PAtoPSI:        //  343
    case ITM_PAtoTOR:        //  344
    case ITM_PAtoTORb:       //  345
    case ITM_TORtoPA:        //  346
    case ITM_TORtoPAb:       //  347
    case ITM_StoYEAR:        //  348
    case ITM_YEARtoS:        //  349
    case ITM_CARATtoG:       //  350
    case ITM_CARATtoGb:      //  351
    case ITM_JINtoKG:        //  352
    case ITM_GtoCARAT:       //  353
    case ITM_GtoCARATb:      //  354
    case ITM_KGtoJIN:        //  355
    case ITM_QTtoL:          //  356
    case ITM_LtoQT:          //  357
    case ITM_FATHOMtoM:      //  358
    case ITM_FATHOMtoMb:     //  359
    case ITM_FATHOMtoMc:     //  360
    case ITM_MtoFATHOM:      //  361
    case ITM_MtoFATHOMb:     //  362
    case ITM_MtoFATHOMc:     //  363
    case ITM_BARRELtoM3:     //  364
    case ITM_BARRELtoM3b:    //  365
    case ITM_M3toBARREL:     //  366
    case ITM_M3toBARRELb:    //  367
    case ITM_ATMtoPAb:       //  368
    case ITM_PAtoATMb:       //  369
    case ITM_HECTAREtoM2:    //  370
    case ITM_M2toHECTARE:    //  371
    case ITM_MUtoM2:         //  372
    case ITM_M2toMU:         //  373
    case ITM_LItoM:          //  374
    case ITM_MtoLI:          //  375
    case ITM_CHItoM:         //  376
    case ITM_MtoCHI:         //  377
    case ITM_YINtoM:         //  378
    case ITM_MtoYIN:         //  379
    case ITM_CUNtoM:         //  380
    case ITM_MtoCUN:         //  381
    case ITM_ZHANGtoM:       //  382
    case ITM_ZHANGtoMb:      //  383
    case ITM_MtoZHANG:       //  384
    case ITM_MtoZHANGb:      //  385
    case ITM_FENtoM:         //  386
    case ITM_MtoFEN:         //  387
    case ITM_LOGICALNAND:    //  402
    case ITM_LOGICALNOR:     //  403
    case ITM_LOGICALXNOR:    //  404
    case ITM_LJ:             //  417
    case ITM_RJ:             //  418
    case ITM_MIRROR:         //  421
    case ITM_NUMB:           //  422
    case ITM_SIGMAPLUS:      //  433
    case ITM_SIGMAMINUS:     //  434
    case ITM_NSIGMA:         //  435
    case ITM_SIGMAx:         //  436
    case ITM_SIGMAy:         //  437
    case ITM_SIGMAx2:        //  438
    case ITM_SIGMAx2y:       //  439
    case ITM_SIGMAy2:        //  440
    case ITM_SIGMAxy:        //  441
    case ITM_SIGMAlnxy:      //  442
    case ITM_SIGMAlnx:       //  443
    case ITM_SIGMAln2x:      //  444
    case ITM_SIGMAylnx:      //  445
    case ITM_SIGMAlny:       //  446
    case ITM_SIGMAln2y:      //  447
    case ITM_SIGMAxlny:      //  448
    case ITM_SIGMAx2lny:     //  449
    case ITM_SIGMAlnyonx:    //  450
    case ITM_SIGMAx2ony:     //  451
    case ITM_SIGMA1onx:      //  452
    case ITM_SIGMA1onx2:     //  453
    case ITM_SIGMAxony:      //  454
    case ITM_SIGMA1ony:      //  455
    case ITM_SIGMA1ony2:     //  456
    case ITM_SIGMAx3:        //  457
    case ITM_SIGMAx4:        //  458
    case ITM_BINOMP:         // 1208
    case ITM_BINOM:          // 1209
    case ITM_BINOMU:         // 1210
    case ITM_BINOMM1:        // 1211
    case ITM_CAUCHP:         // 1213
    case ITM_CAUCH:          // 1214
    case ITM_CAUCHU:         // 1215
    case ITM_CAUCHM1:        // 1216
    case ITM_EXPONP:         // 1218
    case ITM_EXPON:          // 1219
    case ITM_EXPONU:         // 1220
    case ITM_EXPONM1:        // 1221
    case ITM_FPX:            // 1223
    case ITM_FX:             // 1224
    case ITM_FUX:            // 1225
    case ITM_FM1P:           // 1226
    case ITM_GEOMP:          // 1228
    case ITM_GEOM:           // 1229
    case ITM_GEOMU:          // 1230
    case ITM_GEOMM1:         // 1231
    case ITM_HYPERP:         // 1233
    case ITM_HYPER:          // 1234
    case ITM_HYPERU:         // 1235
    case ITM_HYPERM1:        // 1236
    case ITM_LGNRMP:         // 1238
    case ITM_LGNRM:          // 1239
    case ITM_LGNRMU:         // 1240
    case ITM_LGNRMM1:        // 1241
    case ITM_LOGISP:         // 1243
    case ITM_LOGIS:          // 1244
    case ITM_LOGISU:         // 1245
    case ITM_LOGISM1:        // 1246
    case ITM_NBINP:          // 1248
    case ITM_NBIN:           // 1249
    case ITM_NBINU:          // 1250
    case ITM_NBINM1:         // 1251
    case ITM_NORMLP:         // 1253
    case ITM_NORML:          // 1254
    case ITM_NORMLU:         // 1255
    case ITM_NORMLM1:        // 1256
    case ITM_POISSP:         // 1258
    case ITM_POISS:          // 1259
    case ITM_POISSU:         // 1260
    case ITM_POISSM1:        // 1261
    case ITM_TPX:            // 1263
    case ITM_TX:             // 1264
    case ITM_TUX:            // 1265
    case ITM_TM1P:           // 1266
    case ITM_WEIBLP:         // 1268
    case ITM_WEIBL:          // 1269
    case ITM_WEIBLU:         // 1270
    case ITM_WEIBLM1:        // 1271
    case ITM_chi2Px:         // 1273
    case ITM_chi2x:          // 1274
    case ITM_chi2ux:         // 1275
    case ITM_chi2M1:         // 1276
    case ITM_EXPF:           // 1298
    case ITM_LINF:           // 1299
    case ITM_LOGF:           // 1300
    case ITM_ORTHOF:         // 1301
    case ITM_POWERF:         // 1302
    case ITM_GAUSSF:         // 1303
    case ITM_CAUCHF:         // 1304
    case ITM_PARABF:         // 1305
    case ITM_HYPF:           // 1306
    case ITM_ROOTF:          // 1307
    case ITM_1COMPL:         // 1404
    case ITM_SNAP:           // 1405
    case ITM_2COMPL:         // 1406
    case ITM_ABS:            // 1407
    case ITM_AGM:            // 1408
    case ITM_BATT:           // 1413
    case ITM_BEEP:           // 1414
    case ITM_BEGINP:         // 1415
    case ITM_BN:             // 1416
    case ITM_BNS:            // 1417
    case ITM_CLCVAR:         // 1420
    case ITM_CLFALL:         // 1421
    case ITM_TGLFRT:         // 1422
    case ITM_CLLCD:          // 1423
    case ITM_CLMENU:         // 1424
    case ITM_CLSTK:          // 1428
    case ITM_CLSIGMA:        // 1429
    case ITM_CONJ:           // 1431
    case ITM_CORR:           // 1433
    case ITM_COV:            // 1434
    case ITM_BESTFQ:         // 1435
    case ITM_CROSS_PROD:     // 1436
    case ITM_CXtoRE:         // 1437
    case ITM_DATE:           // 1438
    case ITM_DATEto:         // 1439
    case ITM_DAY:            // 1440
    case ITM_DBLR:           // 1441
    case ITM_DBLMULT:        // 1442
    case ITM_DBLDIV:         // 1443
    case ITM_DECOMP:         // 1444
    case ITM_DEG:            // 1445
    case ITM_DEGto:          // 1446
    case ITM_SA:             // 1447
    case ITM_DENMAX:         // 1448
    case ITM_DOT_PROD:       // 1449
    case ITM_DMS:            // 1451
    case ITM_DMSto:          // 1452
    case ITM_DMY:            // 1453
    case ITM_DtoJ:           // 1454
    case ITM_EIGVAL:         // 1456
    case ITM_EIGVEC:         // 1457
    case ITM_END:            // 1458
    case ITM_ENDP:           // 1459
    case ITM_ENORM:          // 1461
    case ITM_ERF:            // 1466
    case ITM_ERFC:           // 1467
    case ITM_EXITALL:        // 1469
    case ITM_EXPT:           // 1470
    case ITM_GET_JUL_GREG:   // 1471
    case ITM_FIB:            // 1472
    case ITM_FLASH:          // 1474
    case ITM_GD:             // 1478
    case ITM_GDM1:           // 1479
    case ITM_GRAD:           // 1480
    case ITM_GRADto:         // 1481
    case ITM_HN:             // 1483
    case ITM_HNP:            // 1484
    case ITM_IM:             // 1485
    case ITM_IXYZ:           // 1487
    case ITM_IGAMMAP:        // 1488
    case ITM_IGAMMAQ:        // 1489
    case ITM_IPLUS:          // 1490
    case ITM_IMINUS:         // 1491
    case ITM_JYX:            // 1492
    case ITM_JPLUS:          // 1493
    case ITM_JMINUS:         // 1494
    case ITM_JUL_GREG:       // 1495
    case ITM_JtoD:           // 1496
    case ITM_sinc:           // 1500
    case ITM_LASTX:          // 1502
    case ITM_LEAP:           // 1504
    case ITM_Lm:             // 1505
    case ITM_LmALPHA:        // 1506
    case ITM_LNBETA:         // 1507
    case ITM_LNGAMMA:        // 1508
    case ITM_LocRQ:          // 1515
    case ITM_LR:             // 1516
    case ITM_MANT:           // 1517
    case ITM_MEM:            // 1519
    case ITM_MENU:           // 1520
    case ITM_MONTH:          // 1521
    case ITM_MSG:            // 1522
    case ITM_MULPI:          // 1523
    case ITM_M_DIM:          // 1526
    case ITM_M_DIMQ:         // 1527
    case ITM_MDY:            // 1528
    case ITM_M_GET:          // 1531
    case ITM_M_LU:           // 1535
    case ITM_M_NEW:          // 1536
    case ITM_M_PUT:          // 1538
    case ITM_M_RR:           // 1539
    case ITM_sincpi:         // 1540
    case ITM_NOP:            // 1542
    case ITM_OFF:            // 1543
    case ITM_DROPY:          // 1544
    case ITM_PIXEL:          // 1548
    case ITM_PLOT:           // 1549
    case ITM_Pn:             // 1550
    case ITM_POINT:          // 1551
    case ITM_POPLR:          // 1553
    case ITM_PRCL:           // 1554
    case ITM_RAD:            // 1557
    case ITM_RADto:          // 1558
    case ITM_RAN:            // 1559
    case ITM_RCLEL:          // 1562
    case ITM_RCLIJ:          // 1563
    case ITM_RE:             // 1566
    case ITM_RECV:           // 1567
    case ITM_REtoCX:         // 1569
    case ITM_REexIM:         // 1570
    case ITM_RMQ:            // 1572
    case ITM_RNORM:          // 1574
    case ITM_EX1:            // 1575
    case ITM_ROUNDI:         // 1576
    case ITM_RSUM:           // 1578
    case ITM_RTNP1:          // 1579
    case ITM_R_CLR:          // 1580
    case ITM_R_COPY:         // 1581
    case ITM_R_SORT:         // 1582
    case ITM_R_SWAP:         // 1583
    case ITM_am:             // 1584
    case ITM_STDDEVWEIGHTED: // 1585
    case ITM_SDIGS:          // 1588
    case ITM_SEED:           // 1589
    case ITM_SEND:           // 1590
    case ITM_SETCHN:         // 1591
    case ITM_SETDAT:         // 1592
    case ITM_SETEUR:         // 1593
    case ITM_SETIND:         // 1594
    case ITM_SETJPN:         // 1595
    case ITM_SETSIG:         // 1596
    case ITM_SETTIM:         // 1597
    case ITM_SETUK:          // 1598
    case ITM_SETUSA:         // 1599
    case ITM_SIGN:           // 1600
    case ITM_SIGNMT:         // 1601
    case ITM_SLVQ:           // 1604
    case ITM_SM:             // 1605
    case ITM_ISM:            // 1606
    case ITM_SMW:            // 1607
    case ITM_SSIZE:          // 1609
    case ITM_STOEL:          // 1612
    case ITM_STOIJ:          // 1613
    case ITM_LN1X:           // 1614
    case ITM_SUM:            // 1616
    case ITM_SW:             // 1617
    case ITM_SXY:            // 1618
    case ITM_TICKS:          // 1620
    case ITM_TIME:           // 1621
    case ITM_Tn:             // 1623
    case ITM_ULP:            // 1626
    case ITM_Un:             // 1627
    case ITM_UNITV:          // 1628
    case ITM_UNSIGN:         // 1629
    case ITM_IDIVR:          // 1632
    case ITM_WDAY:           // 1633
    case ITM_WM:             // 1635
    case ITM_WP:             // 1636
    case ITM_WM1:            // 1637
    case ITM_WSIZEQ:         // 1639
    case ITM_XBAR:           // 1640
    case ITM_XG:             // 1641
    case ITM_XW:             // 1642
    case ITM_XCIRC:          // 1643
    case ITM_XtoDATE:        // 1644
    case ITM_XtoALPHA:       // 1645
    case ITM_YEAR:           // 1647
    case ITM_YCIRC:          // 1648
    case ITM_YMD:            // 1649
    case ITM_XMAX:           // 1653
    case ITM_XMIN:           // 1654
    case ITM_BETAXY:         // 1661
    case ITM_gammaXY:        // 1662
    case ITM_GAMMAXY:        // 1663
    case ITM_GAMMAX:         // 1664
    case ITM_DELTAPC:        // 1666
    case ITM_SCATTFACT:      // 1667
    case ITM_SCATTFACTm:     // 1668
    case ITM_SCATTFACTp:     // 1669
    case ITM_zetaX:          // 1670
    case ITM_STDDEV:         // 1673
    case ITM_STDDEVPOP:      // 1674
    case ITM_RANI:           // 1675
    case ITM_PRINTERX:       // 1676
    case ITM_RANGE:          // 1677
    case ITM_GETRANGE:       // 1678
    case ITM_M1X:            // 1679
    case ITM_XMOD:           // 1680
    case ITM_toDATE:         // 1681
    case ITM_sn:             // 1682
    case ITM_cn:             // 1683
    case ITM_dn:             // 1684
    case ITM_toHR:           // 1685
    case ITM_toHMS:          // 1686
    case ITM_toPOL:          // 1688
    case ITM_MPItoR:         // 1689
    case ITM_RtoMPI:         // 1690
    case ITM_toREAL:         // 1691
    case ITM_toREC:          // 1692
    case ITM_DtoDMS:         // 1693
    case ITM_PC:             // 1695
    case ITM_PCMRR:          // 1696
    case ITM_PCT:            // 1697
    case ITM_PCSIGMA:        // 1698
    case ITM_PCPMG:          // 1699
    case ITM_PMOD:           // 1701
    case ITM_M_DET:          // 1702
    case ITM_PARALLEL:       // 1703
    case ITM_M_TRANSP:       // 1704
    case ITM_M_INV:          // 1705
    case ITM_ARG:            // 1706
    case ITM_MULPIto:        // 1707
    case ITM_PRINTERADV:     // 1708
    case ITM_PRINTERLCD:     // 1711
    case ITM_PRINTERPROG:    // 1713
    case ITM_PRINTERREGS:    // 1715
    case ITM_PRINTERSTK:     // 1716
    case ITM_PRINTERUSER:    // 1718
    case ITM_PRINTERWIDTH:   // 1719
    case ITM_PRINTERSIGMA:   // 1720
    case ITM_PRINTERHASH:    // 1721
    case ITM_Kk:             // 1726
    case ITM_Ek:             // 1727
    case ITM_PInk:           // 1728
    case ITM_ANGLE:          // 1739
    case ITM_SHOW:           // 1742
    case ITM_DMStoD:         // 1744
    case ITM_VANGLE:         // 1745
    case ITM_XH:             // 1746
    case ITM_XRMS:           // 1747
    case ITM_ACOS:           // 1748
    case ITM_ASIN:           // 1749
    case ITM_ATAN:           // 1750
    case ITM_DET:            // 1751
    case ITM_INVRT:          // 1752
    case ITM_TRANS:          // 1753
    case ITM_HIDE:           // 1757
    case ITM_Fphik:          // 1763
    case ITM_Ephik:          // 1764
    case ITM_ZETAphik:       // 1765
    case ITM_GETHIDE:        // 1766
    case ITM_SQRT:           // 1768
    case ITM_atan2:          // 1775
      tmpString[0] = (func >> 8) | 0x80;
      tmpString[1] =  func       & 0xff;
      _insertInProgram((uint8_t *)tmpString, 2);
      break;

    default: {
      #ifndef DMCP_BUILD
        if(stringByteLength(indexOfItems[func].itemCatalogName) != 0) {
          stringToUtf8(indexOfItems[func].itemCatalogName, (uint8_t *)tmpString);
        }
        else {
          stringToUtf8(indexOfItems[func].itemSoftmenuName, (uint8_t *)tmpString);
        }
        printf("\nERROR in function insertStepInProgram: unexpected case %s (item=%" PRId16 ")\n", tmpString, func);
      #endif // DMCP_BUILD
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
