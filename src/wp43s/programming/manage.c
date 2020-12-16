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

  free(labelList);
  free(programList);

  labelList = malloc(sizeof(labelList_t) * numberOfLabels);
  if(labelList == NULL) {
  }

  programList = malloc(sizeof(programList_t) * numberOfPrograms);
  if(programList == NULL) {
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
    scanLabelsAndPrograms();
  }
}



void fnClP(uint16_t unusedButMandatoryParameter) {
  if(beginOfCurrentProgram == beginOfProgramMemory && *endOfCurrentProgram == 255 && *(endOfCurrentProgram + 1) == 255) { // There is only one program in memory
    fnClPAll(CONFIRMED);
  }
  else {
    uint16_t savedCurrentProgramNumber = currentProgramNumber;

    deleteStepsFromTo(beginOfCurrentProgram, endOfCurrentProgram);
    scanLabelsAndPrograms();

    if(savedCurrentProgramNumber >= numberOfPrograms) { // The last program
      fnGotoDot(programList[numberOfPrograms - 2].step);
    }
    else { // Not the last program
      fnGotoDot(programList[savedCurrentProgramNumber - 1].step);
    }
  }
}



void defineCurrentProgramFromGlobalStepNumber(uint16_t globalStepNumber) {
  currentProgramNumber = 0;
  while(globalStepNumber >= programList[currentProgramNumber].step) {
    currentProgramNumber++;
  }

  endOfCurrentProgram = programList[currentProgramNumber].instructionPointer;
  beginOfCurrentProgram = programList[currentProgramNumber - 1].instructionPointer;
}



void defineCurrentProgramFromCurrentStep(void) {
  currentProgramNumber = 0;
  while(currentStep >= programList[currentProgramNumber].instructionPointer) {
    currentProgramNumber++;
  }

  endOfCurrentProgram = programList[currentProgramNumber].instructionPointer;
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
    uint8_t *step, *nextStep;
    bool_t lblOrEnd;

    if(calcMode != CM_PEM) {
      calcMode = CM_PEM;
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
      uint32_t numberOfSteps = (uint32_t)(programList[currentProgramNumber].step - programList[currentProgramNumber - 1].step);
      sprintf(tmpString, "{ Prgm #%d: %" PRIu32 " bytes / %" PRIu32 " step%s }", currentProgramNumber, (uint32_t)(programList[currentProgramNumber].instructionPointer - programList[currentProgramNumber - 1].instructionPointer),
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
      step = nextStep;
    }
  #endif // TESTSUITE_BUILD
}



void insertStepInProgram(int16_t func) {
  switch(func) {
    case ITM_GTO:          //    2
    case ITM_GTOP:         // 1472
      #ifndef DMCP_BUILD
        stringToUtf8(indexOfItems[func].itemCatalogName, (uint8_t *)tmpString);
        printf("insertStepInProgram: %s\n", tmpString);
      #endif // DMCP_BUILD
      break;

    case ITM_CLP:          // 1415
      fnClP(NOPARAM);
      break;

    case ITM_CLPALL:       // 1416
      fnClPAll(NOT_CONFIRMED);
      break;

    case ITM_BST:          // 1724
      fnBst(NOPARAM);
      break;

    case ITM_SST:          // 1726
      fnSst(NOPARAM);
      break;

    // Not programmable
    case ITM_ASSIGN:       // 1401
    case ITM_CLALL:        // 1409
    case ITM_CLREGS:       // 1417
    case ITM_EQ_DEL:       // 1453
    case ITM_EQ_EDI:       // 1454
    case ITM_EQ_NEW:       // 1455
    case ITM_LOAD:         // 1499
    case ITM_LOADP:        // 1500
    case ITM_LOADR:        // 1501
    case ITM_LOADSS:       // 1502
    case ITM_LOADSIGMA:    // 1503
    case ITM_LOADV:        // 1542
    case ITM_M_DELR:       // 1515
    case ITM_M_EDI:        // 1519
    case ITM_M_EDIN:       // 1520
    case ITM_M_GOTO:       // 1522
    case ITM_M_GROW:       // 1523
    case ITM_M_INSR:       // 1524
    case ITM_M_OLD:        // 1527
    case ITM_M_WRAP:       // 1531
    case ITM_PSTO:         // 1545
    case ITM_RBR:          // 1550
    case ITM_RESET:        // 1558
    case ITM_SAVE:         // 1576
    case ITM_STATUS:       // 1600
    case ITM_TIMER:        // 1612
    case ITM_FBR:          // 1712
    case ITM_UNDO:         // 1713
    case ITM_SYSTEM:       // 1733
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
}
