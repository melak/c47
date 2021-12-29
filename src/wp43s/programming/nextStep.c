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
 * \file nextStep.c
 ***********************************************/

#include "programming/manage.h"
#include "programming/nextStep.h"

#include "charString.h"
#include "constantPointers.h"
#include "debug.h"
#include "defines.h"
#include "error.h"
#include "fonts.h"
#include "items.h"
#include "longIntegerType.h"
#include "mathematics/comparisonReals.h"
#include "programming/decode.h"
#include "programming/lblGtoXeq.h"
#include "realType.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "screen.h"
#include <stdint.h>

#include "wp43s.h"


uint8_t *countOpBytes(uint8_t *step, uint16_t paramMode) {
  uint8_t opParam = *(uint8_t *)(step++);

  switch(paramMode) {
    case PARAM_DECLARE_LABEL:
      if(opParam <= 109) { // Local labels from 00 to 99 and from A to J
        return step;
      }
      else if(opParam == STRING_LABEL_VARIABLE) {
        return step + *step + 1;
      }
      else {
        #ifndef DMCP_BUILD
          printf("\nIn function countOpBytes case PARAM_DECLARE_LABEL: opParam %u is not a valid label!\n", opParam);
        #endif // !DMCP_BUILD
        return NULL;
      }

    case PARAM_LABEL:
      if(opParam <= 109) { // Local labels from 00 to 99 and from A to J
        return step;
      }
      else if(opParam == STRING_LABEL_VARIABLE || opParam == INDIRECT_VARIABLE) {
        return step + *step + 1;
      }
      else if(opParam == INDIRECT_REGISTER) {
        return step + 1;
      }
      else {
        #ifndef DMCP_BUILD
          printf("\nIn function countOpBytes: case PARAM_LABEL, %u is not a valid parameter!", opParam);
        #endif // !DMCP_BUILD
        return NULL;
      }

    case PARAM_REGISTER:
      if(opParam <= LAST_LOCAL_REGISTER) { // Global registers from 00 to 99, lettered registers from X to K, and local registers from .00 to .98
        return step;
      }
      else if(opParam == STRING_LABEL_VARIABLE || opParam == INDIRECT_VARIABLE) {
        return step + *step + 1;
      }
      else if(opParam == INDIRECT_REGISTER) {
        return step + 1;
      }
      else {
        #ifndef DMCP_BUILD
          printf("\nIn function countOpBytes: case PARAM_REGISTER, %u is not a valid parameter!", opParam);
        #endif // !DMCP_BUILD
        return NULL;
      }

    case PARAM_FLAG:
      if(opParam <= LAST_LOCAL_FLAG) { // Global flags from 00 to 99, lettered flags from X to K, and local flags from .00 to .15 (or .31)
        return step;
      }
      else if(opParam == INDIRECT_REGISTER || opParam == SYSTEM_FLAG_NUMBER) {
        return step + 1;
      }
      else if(opParam == INDIRECT_VARIABLE) {
        return step + *step + 1;
      }
      else {
        #ifndef DMCP_BUILD
          printf("\nIn function countOpBytes: case PARAM_FLAG, %u is not a valid parameter!", opParam);
        #endif // !DMCP_BUILD
        return NULL;
      }

    case PARAM_NUMBER_8:
      if(opParam <= 99) { // Value from 0 to 99
        return step;
      }
      else if(opParam == INDIRECT_REGISTER) {
        return step + 1;
      }
      else if(opParam == INDIRECT_VARIABLE) {
        return step + *step + 1;
      }
      else {
        #ifndef DMCP_BUILD
          printf("\nIn function countOpBytes: case PARAM_NUMBER, %u is not a valid parameter!", opParam);
        #endif // !DMCP_BUILD
        return NULL;
      }

    case PARAM_NUMBER_16:
      return step + 1;

    case PARAM_COMPARE:
      if(opParam <= LAST_LOCAL_REGISTER || opParam == VALUE_0 || opParam == VALUE_1) { // Global registers from 00 to 99, lettered registers from X to K, and local registers from .00 to .98 OR value 0 OR value 1
        return step;
      }
      else if(opParam == STRING_LABEL_VARIABLE || opParam == INDIRECT_VARIABLE) {
        return step + *step + 1;
      }
      else if(opParam == INDIRECT_REGISTER) {
        return step + 1;
      }
      else {
        #ifndef DMCP_BUILD
          printf("\nIn function countOpBytes: case PARAM_COMPARE, %u is not a valid parameter!", opParam);
        #endif // !DMCP_BUILD
        return NULL;
      }

    default:
      #ifndef DMCP_BUILD
        printf("\nIn function countOpBytes: paramMode %u is not valid!\n", paramMode);
      #endif // !DMCP_BUILD
      return NULL;
  }
}


uint8_t *countLiteralBytes(uint8_t *step) {
  switch(*(uint8_t *)(step++)) {
    case BINARY_SHORT_INTEGER:
      return step + 9;

    //case BINARY_LONG_INTEGER:
    //  break;

    case BINARY_REAL34:
      return step + TO_BYTES(REAL34_SIZE);

    case BINARY_COMPLEX34:
      return step + TO_BYTES(COMPLEX34_SIZE);

    //case BINARY_DATE:
    //  break;

    //case BINARY_TIME:
    //  break;

    case STRING_SHORT_INTEGER:
      return step + *(step + 1) + 2;

    case STRING_LONG_INTEGER:
    case STRING_REAL34:
    case STRING_LABEL_VARIABLE:
    case STRING_COMPLEX34:
    case STRING_DATE:
    case STRING_TIME:
      return step + *step + 1;

    default:
      #ifndef DMCP_BUILD
        printf("\nERROR: %u is not an acceptable parameter for ITM_LITERAL!\n", *(uint8_t *)(step - 1));
      #endif // !DMCP_BUILD
      return NULL;
  }
}


uint8_t *findNextStep(uint8_t *step) {
  uint16_t op = *(step++);
  if(op & 0x80) {
    op &= 0x7f;
    op <<= 8;
    op |= *(step++);
  }

  if(op == 0x7fff) { // .END.
    return NULL;
  }
  else {
    switch(indexOfItems[op].status & PTP_STATUS) {
      case PTP_NONE:
      case PTP_DISABLED:
        return step;

      case PTP_LITERAL:
        return countLiteralBytes(step);

      default:
        return countOpBytes(step, (indexOfItems[op].status & PTP_STATUS) >> 9);
    }
  }
}



uint8_t *findPreviousStep(uint8_t *step) {
  uint8_t *searchFromStep = NULL, *nextStep;

  if(step == beginOfProgramMemory) {
    return step;
  }

  if(numberOfLabels == 0 || step <= labelList[0].instructionPointer) {
    searchFromStep = beginOfProgramMemory;
  }
  else {
    for(int16_t label=numberOfLabels - 1; label >= 0; label--) {
      if(labelList[label].instructionPointer < step) {
        searchFromStep = labelList[label].instructionPointer;
        break;
      }
    }
  }

  nextStep = findNextStep(searchFromStep);
  while(nextStep != step) {
    searchFromStep = nextStep;
    nextStep = findNextStep(searchFromStep);
  }

  return searchFromStep;
}



static void _showStep(void) {
#ifndef TESTSUITE_BUILD
  bool_t lblOrEnd = (*currentStep == ITM_LBL) || ((*currentStep == ((ITM_END >> 8) | 0x80)) && (*(currentStep + 1) == (ITM_END & 0xff))) || ((*currentStep == 0xff) && (*(currentStep + 1) == 0xff));
  int16_t xPos = (lblOrEnd ? 42 : 62);
  int16_t maxWidth = SCREEN_WIDTH - xPos;

  sprintf(tmpString, "%04" PRIu16 ":" STD_SPACE_4_PER_EM, currentLocalStepNumber);
  showString(tmpString, &standardFont, 1, Y_POSITION_OF_REGISTER_T_LINE + 6, vmNormal, true, true);

  decodeOneStep(currentStep);
  if(stringWidth(tmpString, &standardFont, true, true) >= maxWidth) {
    char *xstr = tmpString;
    char *xstrOrig = tmpString;
    char *glyph = tmpString + TMP_STR_LENGTH - 4;
    maxWidth -= stringWidth(STD_ELLIPSIS, &standardFont, true, true);
    while(maxWidth > 0) {
      xstrOrig = xstr;
      glyph[0] = *(xstr++);
      if(glyph[0] & 0x80) {
        glyph[1] = *(xstr++);
        glyph[2] = 0;
      }
      else {
        glyph[1] = 0;
      }
      maxWidth -= stringWidth(glyph, &standardFont, true, true);
    }
    xstrOrig[0] = STD_ELLIPSIS[0];
    xstrOrig[1] = STD_ELLIPSIS[1];
    xstrOrig[2] = 0;
  }
  showString(tmpString, &standardFont, xPos, Y_POSITION_OF_REGISTER_T_LINE + 6, vmNormal, true, true);
#endif // TESTSUITE_BUILD
}



static void _bstInPem(void) {
  //  - currentProgramNumber
  //  - currentLocalStepNumber
  //  - firstDisplayedLocalStepNumber
  //  - firstDisplayedStep
  if(currentLocalStepNumber > 1) {
    if(firstDisplayedLocalStepNumber > 0 && currentLocalStepNumber <= firstDisplayedLocalStepNumber + 3) {
      if(--firstDisplayedLocalStepNumber != 0) {
        firstDisplayedStep = findPreviousStep(firstDisplayedStep);
      }
    }

    if(currentLocalStepNumber > 1) {
      currentLocalStepNumber--;
    }
  }
  else {
    uint16_t numberOfSteps = getNumberOfSteps();
    currentLocalStepNumber = numberOfSteps;
    if(numberOfSteps <= 6) {
      firstDisplayedLocalStepNumber = 0;
      firstDisplayedStep = programList[currentProgramNumber - 1].instructionPointer;
    }
    else if(currentProgramNumber == numberOfPrograms) {
      firstDisplayedLocalStepNumber = numberOfSteps - 6;
      firstDisplayedStep = programList[currentProgramNumber - 1].instructionPointer;
      for(uint16_t i = 1; i < firstDisplayedLocalStepNumber; ++i) {
        firstDisplayedStep = findNextStep(firstDisplayedStep);
      }
    }
    else {
      firstDisplayedLocalStepNumber = numberOfSteps - 6;
      firstDisplayedStep = findPreviousStep(programList[currentProgramNumber].instructionPointer);
      firstDisplayedStep = findPreviousStep(firstDisplayedStep);
      firstDisplayedStep = findPreviousStep(firstDisplayedStep);
      firstDisplayedStep = findPreviousStep(firstDisplayedStep);
      firstDisplayedStep = findPreviousStep(firstDisplayedStep);
      firstDisplayedStep = findPreviousStep(firstDisplayedStep);
      firstDisplayedStep = findPreviousStep(firstDisplayedStep);
    }
  }
}

void fnBst(uint16_t unusedButMandatoryParameter) {
  currentInputVariable = INVALID_VARIABLE;
  _bstInPem();
  if(calcMode != CM_PEM) {
    currentStep = programList[currentProgramNumber - 1].instructionPointer;
    for(int i = 1; i < currentLocalStepNumber; ++i) {
      currentStep = findNextStep(currentStep);
    }
    _showStep();
  }
}



static void _sstInPem(void) {
  uint16_t numberOfSteps = getNumberOfSteps();

  if(currentLocalStepNumber < numberOfSteps) {
    if(currentLocalStepNumber++ >= 3) {
      if(!programListEnd) {
        if(++firstDisplayedLocalStepNumber > 1) {
          firstDisplayedStep = findNextStep(firstDisplayedStep);
        }
      }
    }

    if(firstDisplayedLocalStepNumber + 7 > numberOfSteps) {
      if(numberOfSteps <= 6) {
        firstDisplayedLocalStepNumber = 0;
        firstDisplayedStep = programList[currentProgramNumber - 1].instructionPointer;
      }
      else {
        firstDisplayedLocalStepNumber = numberOfSteps - 6;
        firstDisplayedStep = programList[currentProgramNumber - 1].instructionPointer;
        for(uint16_t i = 1; i < firstDisplayedLocalStepNumber; ++i)
          firstDisplayedStep = findNextStep(firstDisplayedStep);
      }
    }
  }
  else {
    currentLocalStepNumber = 1;
    firstDisplayedLocalStepNumber = 0;
    firstDisplayedStep = programList[currentProgramNumber - 1].instructionPointer;
  }
}

void fnSst(uint16_t unusedButMandatoryParameter) {
  currentInputVariable = INVALID_VARIABLE;
  if(calcMode == CM_PEM) {
    _sstInPem();
  }
  else {
    _showStep();
    runProgram(true);
  }
}



void fnBack(uint16_t numberOfSteps) {
  if(numberOfSteps >= (currentLocalStepNumber - 1)) {
    currentLocalStepNumber = 1;
    currentStep = programList[currentProgramNumber - 1].instructionPointer;
  }
  else {
    currentLocalStepNumber -= numberOfSteps;
    currentStep = programList[currentProgramNumber - 1].instructionPointer;
    for(uint16_t i = 1; i < numberOfSteps; ++i) {
      currentStep = findNextStep(currentStep);
    }
  }
}



void fnSkip(uint16_t numberOfSteps) {
  for(uint16_t i = 0; i <= numberOfSteps; ++i) { // '<=' is intended here because the pointer must be moved at least by 1 step
    if((*currentStep != ((ITM_END >> 8) | 0x80) || *(currentStep + 1) != (ITM_END & 0xff)) && (*currentStep != 255 || *(currentStep + 1) != 255)) {
      ++currentLocalStepNumber;
      currentStep = findNextStep(currentStep);
    }
    else {
      break;
    }
  }
}



void fnCase(uint16_t regist) {
  real34_t arg;
  switch(getRegisterDataType(regist)) {
    case dtLongInteger:
      convertLongIntegerRegisterToReal34(regist, &arg);
      break;
    case dtReal34:
      if(getRegisterAngularMode(regist) == amNone) {
        real34ToIntegralValue(REGISTER_REAL34_DATA(regist), &arg, DEC_ROUND_DOWN);
        break;
      }
      /* fallthrough */
    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "cannot use %s for the parameter of CASE", getRegisterDataTypeName(REGISTER_X, true, false));
        moreInfoOnError("In function fnCase:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
  }

  if(real34CompareLessThan(&arg, const34_1)) {
    fnSkip(0);
  }
  else if(real34CompareGreaterEqual(&arg, const34_1e6)) {
    fnSkip(65534u);
  }
  else {
    uint32_t x = real34ToUInt32(&arg);
    if(x < 65535u) {
      fnSkip(x - 1);
    }
    else {
      fnSkip(65534u);
    }
  }
}
