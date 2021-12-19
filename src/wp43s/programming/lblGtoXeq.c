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
 * \file lblGtoXeq.c
 ***********************************************/

#include "programming/lblGtoXeq.h"

#include "charString.h"
#include "constantPointers.h"
#include "dateTime.h"
#include "defines.h"
#include "error.h"
#include "flags.h"
#include "fonts.h"
#include "items.h"
#include "longIntegerType.h"
#include "memory.h"
#include "programming/manage.h"
#include "programming/nextStep.h"
#include "realType.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "screen.h"
#include "softmenus.h"
#include "statusBar.h"
#include "stack.h"
#include "timer.h"
#include "ui/tam.h"

#include "wp43s.h"



void fnGoto(uint16_t label) {
  if(tam.mode || calcMode != CM_PEM) {
    if(dynamicMenuItem >= 0) {
      fnGotoDot(label);
      return;
    }

    // Local Label 00 to 99 and A, B, C, D, I, and J
    if(label < REGISTER_X || (label != REGISTER_L && REGISTER_A <= label && label <= REGISTER_J)) {
      switch(label) {
        case REGISTER_A: label = 100 - 'A' + 'A'; break;
        case REGISTER_B: label = 100 - 'A' + 'B'; break;
        case REGISTER_C: label = 100 - 'A' + 'C'; break;
        case REGISTER_D: label = 100 - 'A' + 'D'; break;
        case REGISTER_I: label = 100 - 'A' + 'I'; break;
        case REGISTER_J: label = 100 - 'A' + 'J'; break;
        default: {}
      }

      // Search for local label
      for(uint16_t lbl=0; lbl<numberOfLabels; lbl++) {
        if(labelList[lbl].program == currentProgramNumber && labelList[lbl].step < 0 && *(labelList[lbl].labelPointer) == label) { // Is in the current program and is a local label and is the searched label
          fnGotoDot(-labelList[lbl].step);
          return;
        }
      }

      #ifndef DMCP_BUILD
        if(label < REGISTER_X) {
          printf("Error in function fnGoto: there is no local label %02u in current program\n", label);
        }
        else {
          printf("Error in function fnGoto: there is no local label %c in current program\n", 'A' + (label - 100));
        }
      #endif // DMCP_BUILD
    }
    else if(label >= FIRST_LABEL && label <= LAST_LABEL) { // Global named label
      if((label - FIRST_LABEL) < numberOfLabels) {
        fnGotoDot(labelList[label - FIRST_LABEL].step);
        return;
      }
      #ifndef DMCP_BUILD
      else {
        printf("Error in function fnGoto: label ID %u out of range\n", label - FIRST_LABEL);
      }
      #endif // DMCP_BUILD
    }
    else { // Global label X, Y, Z, T, K, or L
      switch(label) {
        case REGISTER_X: label = 'X'; break;
        case REGISTER_Y: label = 'Y'; break;
        case REGISTER_Z: label = 'Z'; break;
        case REGISTER_T: label = 'T'; break;
        case REGISTER_K: label = 'K'; break;
        case REGISTER_L: label = 'L'; break;
        default: {}
      }

      for(uint16_t lbl=0; lbl<numberOfLabels; lbl++) {
        if(labelList[lbl].step > 0 && *(labelList[lbl].labelPointer) == 1 && *(labelList[lbl].labelPointer + 1) == label) { // Is a global label and is the searched label
          fnGotoDot(labelList[lbl].step);
          return;
        }
      }

      #ifndef DMCP_BUILD
        printf("Error in function fnGoto: there is no global label %c\n", label);
      #endif // DMCP_BUILD
    }
  }
  else {
    insertStepInProgram(ITM_GTO);
  }
}



void fnGotoDot(uint16_t globalStepNumber) {
  if(dynamicMenuItem >= 0) {
    uint8_t *labelName = (uint8_t *)dynmenuGetLabel(dynamicMenuItem);

    if(*labelName == 0) {
      return;
    }

    int16_t c, len = stringByteLength((char *)labelName);
    for(uint16_t lbl=0; lbl<numberOfLabels; lbl++) {
      if(labelList[lbl].step > 0 && *labelList[lbl].labelPointer == len) { // It's a global label and the length is OK
        for(c=0; c<len; c++) {
          if(labelName[c] != labelList[lbl].labelPointer[c + 1]) {
            break;
          }
        }
        if(c == len) {
          globalStepNumber = labelList[lbl].step;
          break;
        }
      }
    }
  }

  defineCurrentProgramFromGlobalStepNumber(globalStepNumber);
  currentLocalStepNumber = globalStepNumber - programList[currentProgramNumber - 1].step + 1;

  uint8_t *stepPointer = beginOfCurrentProgram;
  globalStepNumber = 1;
  while(true) {
    if(globalStepNumber == currentLocalStepNumber) {
      currentStep = stepPointer;
      break;
    }

    stepPointer = findNextStep(stepPointer);
    globalStepNumber++;
  }

  if(currentLocalStepNumber >= 3) {
    firstDisplayedLocalStepNumber = currentLocalStepNumber - 3;
    firstDisplayedStep = findPreviousStep(findPreviousStep(currentStep));
    if(firstDisplayedLocalStepNumber != 0) {
      firstDisplayedStep = findPreviousStep(firstDisplayedStep);
    }
    uint16_t numberOfSteps = getNumberOfSteps();
    if(firstDisplayedLocalStepNumber + 6 > numberOfSteps) {
      for(int i=3+currentLocalStepNumber-numberOfSteps; i>0; i--) {
        if(firstDisplayedLocalStepNumber > 0) {
          firstDisplayedLocalStepNumber--;
        }
        if(firstDisplayedStep > programList[currentProgramNumber - 1].instructionPointer) {
          firstDisplayedStep = findPreviousStep(firstDisplayedStep);
        }
      }
    }
  }
  else {
    firstDisplayedLocalStepNumber = 0;
    firstDisplayedStep = beginOfCurrentProgram;
  }
}



void fnExecute(uint16_t label) {
  if(programRunStop == PGM_RUNNING) {
    dataBlock_t *_currentSubroutineLevelData = currentSubroutineLevelData;
    allSubroutineLevels.numberOfSubroutineLevels += 1;
    currentSubroutineLevelData = allocWp43s(3);
    if(currentSubroutineLevelData) {
      _currentSubroutineLevelData[2].ptrToNextLevel = TO_WP43SMEMPTR(currentSubroutineLevelData);
      currentReturnProgramNumber = currentProgramNumber;
      currentReturnLocalStep = currentLocalStepNumber;
      currentNumberOfLocalRegisters = 0; // No local register
      currentNumberOfLocalFlags = 0; // No local flags
      currentSubroutineLevel = allSubroutineLevels.numberOfSubroutineLevels - 1;
      currentPtrToNextLevel = WP43S_NULL;
      currentPtrToPreviousLevel = TO_WP43SMEMPTR(_currentSubroutineLevelData);
      currentLocalFlags = NULL;
      currentLocalRegisters = NULL;

      fnGoto(label);
      dynamicMenuItem = -1;
    }
    else {
      // OUT OF MEMORY
      // May occur if nested too deeply: we don't have tail recursion optimization
      currentSubroutineLevelData = _currentSubroutineLevelData;
      displayCalcErrorMessage(ERROR_RAM_FULL, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    }
  }
  else {
    fnGoto(label);
    dynamicMenuItem = -1;
#ifndef TESTSUITE_BUILD
    if(tam.mode) {
      tamLeaveMode();
      refreshScreen();
    }
#endif // TESTSUITE_BUILD
    runProgram(false);
  }
}



void fnReturn(uint16_t skip) {
  dataBlock_t *_currentSubroutineLevelData = currentSubroutineLevelData;
  uint16_t sizeToBeFreedInBlocks;

  /* A subroutine is running */
  if(currentSubroutineLevel > 0) {
    uint16_t returnGlobalStepNumber = currentReturnLocalStep + programList[currentReturnProgramNumber - 1].step; // the next step
    fnGotoDot(returnGlobalStepNumber);
    if(skip > 0 && (*currentStep != ((ITM_END >> 8) | 0x80) || *(currentStep + 1) != (ITM_END & 0xff)) && (*currentStep != 255 || *(currentStep + 1) != 255)) {
      ++currentLocalStepNumber;
      currentStep = findNextStep(currentStep);
    }
    if(currentNumberOfLocalRegisters > 0) {
      allocateLocalRegisters(0);
      _currentSubroutineLevelData = currentSubroutineLevelData;
    }
    sizeToBeFreedInBlocks = 3 + (currentNumberOfLocalFlags > 0 ? 1 : 0);
    currentSubroutineLevelData = TO_PCMEMPTR(currentPtrToPreviousLevel);
    freeWp43s(_currentSubroutineLevelData, sizeToBeFreedInBlocks);
    currentPtrToNextLevel = WP43S_NULL;
    allSubroutineLevels.numberOfSubroutineLevels -= 1;

    currentLocalFlags = (currentNumberOfLocalFlags == 0 ? NULL : currentSubroutineLevelData + 3);
    currentLocalRegisters = (registerHeader_t *)(currentNumberOfLocalRegisters == 0 ? NULL : currentSubroutineLevelData + (currentLocalFlags == NULL ? 3 : 4));
  }

  /* Not in a subroutine */
  else {
    fnGotoDot(programList[currentProgramNumber - 1].step);
  }
}



void fnRunProgram(uint16_t unusedButMandatoryParameter) {
  dynamicMenuItem = -1;
  runProgram(false);
}



void fnStopProgram(uint16_t unusedButMandatoryParameter) {
  programRunStop = PGM_WAITING;
}



#ifndef TESTSUITE_BUILD
static void _getStringLabelOrVariableName(uint8_t *stringAddress) {
  uint8_t stringLength = *(uint8_t *)(stringAddress++);
  xcopy(tmpStringLabelOrVariableName, stringAddress, stringLength);
  tmpStringLabelOrVariableName[stringLength] = 0;
}

static void _executeWithIndirectRegister(uint8_t *paramAddress, uint16_t op) {
  uint8_t opParam = *(uint8_t *)paramAddress;
  if(opParam <= LAST_LOCAL_REGISTER) { // Local register from .00 to .98
    int16_t realParam = indirectAddressing(opParam, (indexOfItems[op].param == TM_STORCL || indexOfItems[op].param == TM_M_DIM), indexOfItems[op].tamMinMax >> TAM_MAX_BITS, indexOfItems[op].tamMinMax & TAM_MAX_MASK);
    if(realParam < 9999) reallyRunFunction(op, realParam);
  }
  else {
    sprintf(tmpString, "\nIn function _executeWithIndirectRegister: %s " STD_RIGHT_ARROW " %u is not a valid parameter!", indexOfItems[op].itemCatalogName, opParam);
  }
}

static void _executeWithIndirectVariable(uint8_t *stringAddress, uint16_t op) {
  calcRegister_t regist;
  _getStringLabelOrVariableName(stringAddress);
  regist = findNamedVariable(tmpStringLabelOrVariableName);
  if(regist != INVALID_VARIABLE) {
    int16_t realParam = indirectAddressing(regist, (indexOfItems[op].param == TM_STORCL || indexOfItems[op].param == TM_M_DIM), indexOfItems[op].tamMinMax >> TAM_MAX_BITS, indexOfItems[op].tamMinMax & TAM_MAX_MASK);
    if(realParam < 9999) reallyRunFunction(op, realParam);
  }
  else {
    displayCalcErrorMessage(ERROR_UNDEF_SOURCE_VAR, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "string '%s' is not a named variable", tmpStringLabelOrVariableName);
      moreInfoOnError("In function _executeOp:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}

static void _executeOp(uint8_t *paramAddress, uint16_t op, uint16_t paramMode) {
  uint8_t opParam = *(uint8_t *)(paramAddress++);
  bool_t tryAllocate = (op == ITM_STO || op == ITM_M_DIM || op == ITM_MVAR);

  switch(paramMode) {
    case PARAM_DECLARE_LABEL:
      // nothing to do
      break;

    case PARAM_LABEL:
      if(opParam <= 109) { // Local label from 00 to 99 or from A to J
        reallyRunFunction(op, opParam);
      }
      else if(opParam == STRING_LABEL_VARIABLE) {
        _getStringLabelOrVariableName(paramAddress);
        calcRegister_t label = findNamedLabel(tmpStringLabelOrVariableName);
        if(label != INVALID_VARIABLE) {
          reallyRunFunction(op, label);
        }
        else {
          displayCalcErrorMessage(ERROR_LABEL_NOT_FOUND, ERR_REGISTER_LINE, REGISTER_X);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(errorMessage, "string '%s' is not a named label", tmpStringLabelOrVariableName);
            moreInfoOnError("In function _executeOp:", errorMessage, NULL, NULL);
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        }
      }
      else if(opParam == INDIRECT_REGISTER) {
        _executeWithIndirectRegister(paramAddress, op);
      }
      else if(opParam == INDIRECT_VARIABLE) {
        _executeWithIndirectVariable(paramAddress, op);
      }
      else {
        sprintf(tmpString, "\nIn function _executeOp: case PARAM_LABEL, %s  %u is not a valid parameter!", indexOfItems[op].itemCatalogName, opParam);
      }
      break;

    case PARAM_FLAG:
      if(opParam <= LAST_LOCAL_FLAG) { // Global flag from 00 to 99, Lettered flag from X to K, or Local flag from .00 to .15 (or .31)
        reallyRunFunction(op, opParam);
      }
      else if(FIRST_LOCAL_FLAG + NUMBER_OF_LOCAL_FLAGS <= opParam && opParam < FIRST_LOCAL_FLAG + NUMBER_OF_LOCAL_FLAGS + NUMBER_OF_SYSTEM_FLAGS) { // Local register from .00 to .15 (or .31)
        reallyRunFunction(op, opParam);
      }
      else if(opParam == SYSTEM_FLAG_NUMBER) {
        switch((uint16_t)(*paramAddress) | 0xc000) { 
          case FLAG_YMD:
          case FLAG_DMY:
          case FLAG_MDY:
          case FLAG_ALPHA:
          case FLAG_alphaCAP:
          case FLAG_RUNTIM:
          case FLAG_RUNIO:
          case FLAG_PRINT:
          case FLAG_LOWBAT:
          case FLAG_NUMIN:
          case FLAG_ALPIN:
          case FLAG_ASLIFT:
          case FLAG_INTING:
          case FLAG_SOLVING:
          case FLAG_VMDISP:
          case FLAG_USB:
          case FLAG_ENDPMT:
            reallyRunFunction(op, (uint16_t)(*paramAddress) | 0xc000);
            break;
          default:
            reallyRunFunction(op, (uint16_t)(*paramAddress) | 0x8000);
        }
      }
      else if(opParam == INDIRECT_REGISTER) {
        _executeWithIndirectRegister(paramAddress, op);
      }
      else if(opParam == INDIRECT_VARIABLE) {
        _executeWithIndirectVariable(paramAddress, op);
      }
      else {
        sprintf(tmpString, "\nIn function _executeOp: case PARAM_FLAG, %s  %u is not a valid parameter!", indexOfItems[op].itemCatalogName, opParam);
      }
      break;

    case PARAM_NUMBER_8:
      if(opParam <= 99) { // Value from 0 to 99
        reallyRunFunction(op, opParam);
      }
      else if(opParam == INDIRECT_REGISTER) {
        _executeWithIndirectRegister(paramAddress, op);
      }
      else if(opParam == INDIRECT_VARIABLE) {
        _executeWithIndirectVariable(paramAddress, op);
      }
      else {
        sprintf(tmpString, "\nIn function _executeOp: case PARAM_NUMBER, %s  %u is not a valid parameter!", indexOfItems[op].itemCatalogName, opParam);
      }
      break;

    case PARAM_NUMBER_16:
      reallyRunFunction(op, opParam + 256 * *(paramAddress));
      break;

    case PARAM_REGISTER:
    case PARAM_COMPARE:
      if(opParam <= LAST_LOCAL_REGISTER) { // Global register from 00 to 99, Lettered register from X to K, or Local register from .00 to .98
        reallyRunFunction(op, opParam);
      }
      else if(opParam == STRING_LABEL_VARIABLE) {
        _getStringLabelOrVariableName(paramAddress);
        calcRegister_t regist = findNamedVariable(tmpStringLabelOrVariableName);
        if(tryAllocate) {
          reallyRunFunction(op, findOrAllocateNamedVariable(tmpStringLabelOrVariableName));
        }
        else if(regist != INVALID_VARIABLE) {
          reallyRunFunction(op, regist);
        }
        else {
          displayCalcErrorMessage(ERROR_UNDEF_SOURCE_VAR, ERR_REGISTER_LINE, REGISTER_X);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(errorMessage, "string '%s' is not a named variable", tmpStringLabelOrVariableName);
            moreInfoOnError("In function _executeOp:", errorMessage, NULL, NULL);
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        }
      }
      else if(paramMode == PARAM_COMPARE && opParam == VALUE_0) {
        reallocateRegister(TEMP_REGISTER_1, dtReal34, REAL34_SIZE, amNone);
        real34Copy(const34_0, REGISTER_REAL34_DATA(TEMP_REGISTER_1));
        reallyRunFunction(op, TEMP_REGISTER_1);
      }
      else if(paramMode == PARAM_COMPARE && opParam == VALUE_1) {
        reallocateRegister(TEMP_REGISTER_1, dtReal34, REAL34_SIZE, amNone);
        real34Copy(const34_1, REGISTER_REAL34_DATA(TEMP_REGISTER_1));
        reallyRunFunction(op, TEMP_REGISTER_1);
      }
      else if(opParam == INDIRECT_REGISTER) {
        _executeWithIndirectRegister(paramAddress, op);
      }
      else if(opParam == INDIRECT_VARIABLE) {
        _executeWithIndirectVariable(paramAddress, op);
      }
      else {
        sprintf(tmpString, "\nIn function _executeOp: case PARAM_REGISTER / PARAM_COMPARE, %s  %u is not a valid parameter!", indexOfItems[op].itemCatalogName, opParam);
      }
      break;

    default:
      sprintf(tmpString, "\nIn function _executeOp: paramMode %u is not valid!\n", paramMode);
  }
}

static void _putLiteral(uint8_t *literalAddress) {
  switch(*(uint8_t *)(literalAddress++)) {
    case BINARY_SHORT_INTEGER:
      reallocateRegister(REGISTER_X, dtShortInteger, SHORT_INTEGER_SIZE, *(uint8_t *)(literalAddress++));
      xcopy(REGISTER_DATA(REGISTER_X), literalAddress, TO_BYTES(SHORT_INTEGER_SIZE));
      break;

    //case BINARY_LONG_INTEGER:
    //  break;

    case BINARY_REAL34:
      liftStack();
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      real34Copy((real34_t *)literalAddress, REGISTER_REAL34_DATA(REGISTER_X));
      break;

    case BINARY_COMPLEX34:
      liftStack();
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
      complex34Copy((complex34_t *)literalAddress, REGISTER_COMPLEX34_DATA(REGISTER_X));
      break;

    //case BINARY_DATE:
    //  break;

    //case BINARY_TIME:
    //  break;

    case STRING_SHORT_INTEGER:
      {
        longInteger_t val;
        longIntegerInit(val);

        _getStringLabelOrVariableName(literalAddress + 1);
        stringToLongInteger(tmpStringLabelOrVariableName, 10, val);
        liftStack();
        convertLongIntegerToShortIntegerRegister(val, (uint32_t)(*literalAddress), REGISTER_X);

        longIntegerFree(val);
      }
      break;

    case STRING_LONG_INTEGER:
      {
        longInteger_t val;
        longIntegerInit(val);

        _getStringLabelOrVariableName(literalAddress);
        stringToLongInteger(tmpStringLabelOrVariableName, 10, val);
        liftStack();
        convertLongIntegerToLongIntegerRegister(val, REGISTER_X);

        longIntegerFree(val);
      }
      break;

    case STRING_REAL34:
      _getStringLabelOrVariableName(literalAddress);
      liftStack();
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      stringToReal34(tmpStringLabelOrVariableName, REGISTER_REAL34_DATA(REGISTER_X));
      break;

    case STRING_COMPLEX34:
      {
        char *imag = tmpStringLabelOrVariableName;
        _getStringLabelOrVariableName(literalAddress);
        while(*imag != 'i' || *imag != 0) ++imag;
        if(*imag == 'i') {
          if(imag > tmpStringLabelOrVariableName && *(imag - 1) == '-') {
            *imag = '-'; *(imag - 1) = 0;
          }
          else if(imag > tmpStringLabelOrVariableName && *(imag - 1) == '+') {
            *imag = 0; *(imag - 1) = 0;
          }
          else {
            *imag = 0;
          }
        }
        liftStack();
        reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
        stringToReal34(tmpStringLabelOrVariableName, REGISTER_REAL34_DATA(REGISTER_X));
        stringToReal34(imag,                         REGISTER_IMAG34_DATA(REGISTER_X));
      }
      break;

    case STRING_LABEL_VARIABLE:
      _getStringLabelOrVariableName(literalAddress);
      liftStack();
      reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(stringByteLength(tmpStringLabelOrVariableName) + 1), amNone);
      xcopy(REGISTER_STRING_DATA(REGISTER_X), tmpStringLabelOrVariableName, stringByteLength(tmpStringLabelOrVariableName) + 1);
      break;

    case STRING_DATE:
      _getStringLabelOrVariableName(literalAddress);
      liftStack();
      reallocateRegister(REGISTER_X, dtDate, REAL34_SIZE, amNone);
      stringToReal34(tmpStringLabelOrVariableName, REGISTER_REAL34_DATA(REGISTER_X));
      julianDayToInternalDate(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
      break;

    case STRING_TIME:
      _getStringLabelOrVariableName(literalAddress);
      liftStack();
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      stringToReal34(tmpStringLabelOrVariableName, REGISTER_REAL34_DATA(REGISTER_X));
      hmmssInRegisterToSeconds(REGISTER_X);
      break;

    default: {
      #ifndef DMCP_BUILD
        printf("\nERROR: %u is not an acceptable parameter for ITM_LITERAL!\n", *(uint8_t *)(literalAddress - 1));
      #endif // !DMCP_BUILD
    }
  }
}
#endif // TESTSUITE_BUILD

int16_t executeOneStep(uint8_t *step) {
#ifdef TESTSUITE_BUILD
  return 0;
#else // TESTSUITE_BUILD
  uint8_t item8 = *(uint8_t *)(step++);
  uint16_t item16;

  switch(item8) {
    case ITM_LBL:         //   1
      return 1;

    case ITM_GTO:         //   2
    case ITM_XEQ:         //   3
      _executeOp(step, (uint16_t)item8, PARAM_LABEL);
      return -1;

    case ITM_PAUSE:       //  38
      _executeOp(step, (uint16_t)item8, PARAM_NUMBER_8);
      return temporaryInformation == TI_FALSE ? 2 : 1;

    case ITM_ISE:         //   5
    case ITM_ISG:         //   6
    case ITM_ISZ:         //   7
    case ITM_DSE:         //   8
    case ITM_DSL:         //   9
    case ITM_DSZ:         //  10
    case ITM_INPUT:       //  43
    case ITM_STO:         //  44
    case ITM_STOADD:      //  45
    case ITM_STOSUB:      //  46
    case ITM_STOMULT:     //  47
    case ITM_STODIV:      //  48
    case ITM_RCL:         //  51
    case ITM_RCLADD:      //  52
    case ITM_RCLSUB:      //  53
    case ITM_RCLMULT:     //  54
    case ITM_RCLDIV:      //  55
    case ITM_CONVG:       //  56
    case ITM_KEYQ:        //  77
    case ITM_DEC:         //  91
    case ITM_INC:         //  92
    case ITM_VIEW:        // 101
    case ITM_Xex:         // 127
      _executeOp(step, (uint16_t)item8, PARAM_REGISTER);
      return temporaryInformation == TI_FALSE ? 2 : 1;

    case ITM_XEQU:        //  11
    case ITM_XNE:         //  12
    case ITM_XAEQU:       //  15
    case ITM_XLT:         //  16
    case ITM_XLE:         //  17
    case ITM_XGE:         //  18
    case ITM_XGT:         //  19
      _executeOp(step, (uint16_t)item8, PARAM_COMPARE);
      return temporaryInformation == TI_FALSE ? 2 : 1;

    case ITM_FC:          //  20
    case ITM_FS:          //  21
    case ITM_CF:          // 110
    case ITM_SF:          // 111
    case ITM_FF:          // 112
      _executeOp(step, (uint16_t)item8, PARAM_FLAG);
      return temporaryInformation == TI_FALSE ? 2 : 1;

    case ITM_RTN:         //   4
      runFunction(item8);
      return 0;

    case ITM_XEQUP0:      //  13
    case ITM_XEQUM0:      //  14
    case ITM_EVEN:        //  22
    case ITM_ODD:         //  23
    case ITM_FPQ:         //  24
    case ITM_INT:         //  25
    case ITM_CPX:         //  26
    case ITM_MATR:        //  27
    case ITM_NAN:         //  28
    case ITM_REAL:        //  29
    case ITM_SPEC:        //  30
    case ITM_STRI:        //  31
    case ITM_PMINFINITY:  //  32
    case ITM_PRIME:       //  33
    case ITM_TOP:         //  34
    case ITM_ENTER:       //  35
    case ITM_XexY:        //  36
    case ITM_DROP:        //  37
    case ITM_Rup:         //  39
    case ITM_Rdown:       //  40
    case ITM_CLX:         //  41
    case ITM_FILL:        //  42
    case ITM_COMB:        //  49
    case ITM_PERM:        //  50
    case ITM_ENTRY:       //  57
    case ITM_SQUARE:      //  58
    case ITM_CUBE:        //  59
    case ITM_YX:          //  60
    case ITM_SQUAREROOTX: //  61
    case ITM_CUBEROOT:    //  62
    case ITM_XTHROOT:     //  63
    case ITM_2X:          //  64
    case ITM_EXP:         //  65
    case ITM_ROUND:       //  66
    case ITM_10x:         //  67
    case ITM_LOG2:        //  68
    case ITM_LN:          //  69
    case ITM_STOP:        //  70
    case ITM_LOG10:       //  71
    case ITM_LOGXY:       //  72
    case ITM_1ONX:        //  73
    case ITM_cos:         //  74
    case ITM_cosh:        //  75
    case ITM_sin:         //  76
    case ITM_sinh:        //  78
    case ITM_tan:         //  79
    case ITM_tanh:        //  80
    case ITM_arccos:      //  81
    case ITM_arcosh:      //  82
    case ITM_arcsin:      //  83
    case ITM_arsinh:      //  84
    case ITM_arctan:      //  85
    case ITM_artanh:      //  86
    case ITM_CEIL:        //  87
    case ITM_FLOOR:       //  88
    case ITM_GCD:         //  89
    case ITM_LCM:         //  90
    case ITM_IP:          //  93
    case ITM_FP:          //  94
    case ITM_ADD:         //  95
    case ITM_SUB:         //  96
    case ITM_CHS:         //  97
    case ITM_MULT:        //  98
    case ITM_DIV:         //  99
    case ITM_IDIV:        // 100
    case ITM_MOD:         // 102
    case ITM_MAX:         // 103
    case ITM_MIN:         // 104
    case ITM_MAGNITUDE:   // 105
    case ITM_NEIGHB:      // 106
    case ITM_NEXTP:       // 107
    case ITM_XFACT:       // 108
    case ITM_CONSTpi:     // 109
    case ITM_M_SQR:       // 113
    case ITM_toDEG:       // 115
    case ITM_toDMS:       // 116
    case ITM_toGRAD:      // 117
    case ITM_toMULpi:     // 118
    case ITM_toRAD:       // 119
    case ITM_DtoR:        // 120
    case ITM_RtoD:        // 121
    case ITM_RMD:         // 122
    case ITM_LOGICALNOT:  // 123
    case ITM_LOGICALAND:  // 124
    case ITM_LOGICALOR:   // 125
    case ITM_LOGICALXOR:  // 126
      runFunction(item8);
      return temporaryInformation == TI_FALSE ? 2 : 1;

    case ITM_LITERAL:     // 114
      _putLiteral(step);
      return 1;

    default:
      if((item8 & 0x80) == 0) {
        #ifndef DMCP_BUILD
          printf("\nERROR in decodeOneStep: single byte instruction %u is unknown!\n", item8);
        #endif // !DMCP_BUILD
        return 0;
      }

      item16 = ((uint16_t)(item8 & 0x7F) << 8) | *(uint8_t *)(step++);
      switch(item16) {
        case ITM_ASSIGN:         //  1411
        case ITM_CLALL:          //  1419
        case ITM_CLREGS:         //  1427
        case ITM_EQ_DEL:         //  1463
        case ITM_EQ_EDI:         //  1464
        case ITM_EQ_NEW:         //  1465
        case ITM_LOAD:           //  1509
        case ITM_LOADP:          //  1510
        case ITM_LOADR:          //  1511
        case ITM_LOADSS:         //  1512
        case ITM_LOADSIGMA:      //  1513
        case ITM_M_DELR:         //  1525
        case ITM_M_EDI:          //  1529
        case ITM_M_EDIN:         //  1530
        case ITM_M_GOTO:         //  1532
        case ITM_M_GROW:         //  1533
        case ITM_M_INSR:         //  1534
        case ITM_M_OLD:          //  1537
        case ITM_M_WRAP:         //  1541
        case ITM_LOADV:          //  1552
        case ITM_PSTO:           //  1555
        case ITM_RBR:            //  1560
        case ITM_RESET:          //  1568
        case ITM_SAVE:           //  1586
        case ITM_STATUS:         //  1610
        case ITM_TIMER:          //  1622
        case ITM_VERS:           //  1631
        case ITM_WHO:            //  1634
        case ITM_FBR:            //  1722
        case ITM_UNDO:           //  1723
        case ITM_SYSTEM:         //  1743
        case ITM_CLBUP:          //  1780
          displayCalcErrorMessage(ERROR_NON_PROGRAMMABLE_COMMAND, ERR_REGISTER_LINE, REGISTER_X);
          #if (EXTRA_INFO_ON_CALC_ERROR == 1)
            moreInfoOnError("In function decodeOneStep:", "non-programmable function", indexOfItems[item16].itemCatalogName, "appeared in the program!");
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
          return 0;

        case ITM_DELITM:         //  1455
        case ITM_FQX:            //  1475
        case ITM_FDQX:           //  1476
        case ITM_INDEX:          //  1486
        case ITM_LBLQ:           //  1503
        case ITM_PGMINT:         //  1546
        case ITM_PGMSLV:         //  1547
        case ITM_VARMNU:         //  1630
        case ITM_PIn:            //  1671
        case ITM_SIGMAn:         //  1672
        case ITM_INTEGRAL:       //  1700
          _executeOp(step, item16, PARAM_LABEL);
          return temporaryInformation == TI_FALSE ? 2 : 1;

        case ITM_BACK:           //  1412
        case ITM_SKIP:           //  1603
          _executeOp(step, item16, PARAM_NUMBER_8);
          return -1;

        case ITM_CNST:           //   207
        case ITM_BS:             //   405
        case ITM_BC:             //   406
        case ITM_CB:             //   407
        case ITM_SB:             //   408
        case ITM_FB:             //   409
        case ITM_RL:             //   410
        case ITM_RLC:            //   411
        case ITM_RR:             //   412
        case ITM_RRC:            //   413
        case ITM_SL:             //   414
        case ITM_SR:             //   415
        case ITM_ASR:            //   416
        case ITM_MASKL:          //   419
        case ITM_MASKR:          //   420
        case ITM_SDL:            //   423
        case ITM_SDR:            //   424
        case ITM_AGRAPH:         //  1409
        case ITM_ALL:            //  1410
        case ITM_DSTACK:         //  1450
        case ITM_ENG:            //  1460
        case ITM_ERR:            //  1468
        case ITM_FIX:            //  1473
        case ITM_GAP:            //  1477
        case ITM_KEY:            //  1497
        case ITM_LocR:           //  1514
        case ITM_RDP:            //  1565
        case ITM_RM:             //  1571
        case ITM_DSP:            //  1573
        case ITM_RSD:            //  1577
        case ITM_SCI:            //  1587
        case ITM_SIM_EQ:         //  1602
        case ITM_TDISP:          //  1619
        case ITM_TONE:           //  1624
        case ITM_WSIZE:          //  1638
        case ITM_toINT:          //  1687
        case ITM_SHUFFLE:        //  1694
        case ITM_PRINTERCHAR:    //  1709
        case ITM_PRINTERDLAY:    //  1710
        case ITM_PRINTERMODE:    //  1712
        case ITM_PRINTERTAB:     //  1717
          _executeOp(step, item16, PARAM_NUMBER_8);
          return temporaryInformation == TI_FALSE ? 2 : 1;

        case ITM_BESTF:          //  1297
          _executeOp(step, item16, PARAM_NUMBER_16);
          step++;
          return temporaryInformation == TI_FALSE ? 2 : 1;

        case ITM_CASE:           //  1418
          _executeOp(step, item16, PARAM_REGISTER);
          return -1;

        case ITM_SOLVE:          //  1608
          _executeOp(step, item16, PARAM_REGISTER);
          if(temporaryInformation == TI_SOLVER_FAILED) {
            lastErrorCode = ERROR_NONE;
            return 2;
          }
          else {
            return 1;
          }

        case ITM_STOMAX:         //  1430
        case ITM_RCLMAX:         //  1432
        case ITM_RCLMIN:         //  1462
        case ITM_KTYP:           //  1501
        case ITM_MVAR:           //  1524
        case ITM_STOMIN:         //  1545
        case ITM_PUTK:           //  1556
        case ITM_RCLCFG:         //  1561
        case ITM_RCLS:           //  1564
        case ITM_STOCFG:         //  1611
        case ITM_STOS:           //  1615
        case ITM_Tex:            //  1625
        case ITM_Yex:            //  1650
        case ITM_Zex:            //  1651
        case ITM_ALPHALENG:      //  1652
        case ITM_ALPHAPOS:       //  1655
        case ITM_ALPHARL:        //  1656
        case ITM_ALPHARR:        //  1657
        case ITM_ALPHASL:        //  1658
        case ITM_ALPHASR:        //  1659
        case ITM_ALPHAtoX:       //  1660
        case ITM_PRINTERR:       //  1714
          _executeOp(step, item16, PARAM_REGISTER);
          return temporaryInformation == TI_FALSE ? 2 : 1;

        case ITM_FCC:            //   396
        case ITM_FCS:            //   397
        case ITM_FCF:            //   398
        case ITM_FSC:            //   399
        case ITM_FSS:            //   400
        case ITM_FSF:            //   401
          _executeOp(step, item16, PARAM_FLAG);
          return temporaryInformation == TI_FALSE ? 2 : 1;

        case CST_01:             //   128
        case CST_02:             //   129
        case CST_03:             //   130
        case CST_04:             //   131
        case CST_05:             //   132
        case CST_06:             //   133
        case CST_07:             //   134
        case CST_08:             //   135
        case CST_09:             //   136
        case CST_10:             //   137
        case CST_11:             //   138
        case CST_12:             //   139
        case CST_13:             //   140
        case CST_14:             //   141
        case CST_15:             //   142
        case CST_16:             //   143
        case CST_17:             //   144
        case CST_18:             //   145
        case CST_19:             //   146
        case CST_20:             //   147
        case CST_21:             //   148
        case CST_22:             //   149
        case CST_23:             //   150
        case CST_24:             //   151
        case CST_25:             //   152
        case CST_26:             //   153
        case CST_27:             //   154
        case CST_28:             //   155
        case CST_29:             //   156
        case CST_30:             //   157
        case CST_31:             //   158
        case CST_32:             //   159
        case CST_33:             //   160
        case CST_34:             //   161
        case CST_35:             //   162
        case CST_36:             //   163
        case CST_37:             //   164
        case CST_38:             //   165
        case CST_39:             //   166
        case CST_40:             //   167
        case CST_41:             //   168
        case CST_42:             //   169
        case CST_43:             //   170
        case CST_44:             //   171
        case CST_45:             //   172
        case CST_46:             //   173
        case CST_47:             //   174
        case CST_48:             //   175
        case CST_49:             //   176
        case CST_50:             //   177
        case CST_51:             //   178
        case CST_52:             //   179
        case CST_53:             //   180
        case CST_54:             //   181
        case CST_55:             //   182
        case CST_56:             //   183
        case CST_57:             //   184
        case CST_58:             //   185
        case CST_59:             //   186
        case CST_60:             //   187
        case CST_61:             //   188
        case CST_62:             //   189
        case CST_63:             //   190
        case CST_64:             //   191
        case CST_65:             //   192
        case CST_66:             //   193
        case CST_67:             //   194
        case CST_68:             //   195
        case CST_69:             //   196
        case CST_70:             //   197
        case CST_71:             //   198
        case CST_72:             //   199
        case CST_73:             //   200
        case CST_74:             //   201
        case CST_75:             //   202
        case CST_76:             //   203
        case CST_77:             //   204
        case CST_78:             //   205
        case CST_79:             //   206
        case ITM_CtoF:           //   220
        case ITM_FtoC:           //   221
        case ITM_DBtoPR:         //   222
        case ITM_DBtoPRb:        //   223
        case ITM_DBtoPRc:        //   224
        case ITM_DBtoFR:         //   225
        case ITM_DBtoFRb:        //   226
        case ITM_DBtoFRc:        //   227
        case ITM_PRtoDB:         //   228
        case ITM_PRtoDBb:        //   229
        case ITM_PRtoDBc:        //   230
        case ITM_FRtoDB:         //   231
        case ITM_FRtoDBb:        //   232
        case ITM_FRtoDBc:        //   233
        case ITM_ACtoHA:         //   234
        case ITM_ACtoHAb:        //   235
        case ITM_HAtoAC:         //   236
        case ITM_HAtoACb:        //   237
        case ITM_ACUStoHA:       //   238
        case ITM_ACUStoHAb:      //   239
        case ITM_HAtoACUS:       //   240
        case ITM_HAtoACUSb:      //   241
        case ITM_PAtoATM:        //   242
        case ITM_ATMtoPA:        //   243
        case ITM_AUtoM:          //   244
        case ITM_MtoAU:          //   245
        case ITM_BARtoPA:        //   246
        case ITM_PAtoBAR:        //   247
        case ITM_BTUtoJ:         //   248
        case ITM_JtoBTU:         //   249
        case ITM_CALtoJ:         //   250
        case ITM_JtoCAL:         //   251
        case ITM_LBFFTtoNM:      //   252
        case ITM_LBFFTtoNMb:     //   253
        case ITM_NMtoLBFFT:      //   254
        case ITM_NMtoLBFFTb:     //   255
        case ITM_CWTtoKG:        //   256
        case ITM_KGtoCWT:        //   257
        case ITM_FTtoM:          //   258
        case ITM_MtoFT:          //   259
        case ITM_FTUStoM:        //   260
        case ITM_FTUStoMb:       //   261
        case ITM_FTUStoMc:       //   262
        case ITM_MtoFTUS:        //   263
        case ITM_MtoFTUSb:       //   264
        case ITM_MtoFTUSc:       //   265
        case ITM_FZUKtoML:       //   266
        case ITM_FZUKtoMLb:      //   267
        case ITM_MLtoFZUK:       //   268
        case ITM_MLtoFZUKb:      //   269
        case ITM_FZUStoML:       //   270
        case ITM_FZUStoMLb:      //   271
        case ITM_MLtoFZUS:       //   272
        case ITM_MLtoFZUSb:      //   273
        case ITM_GLUKtoL:        //   274
        case ITM_LtoGLUK:        //   275
        case ITM_GLUStoL:        //   276
        case ITM_LtoGLUS:        //   277
        case ITM_HPEtoW:         //   278
        case ITM_WtoHPE:         //   279
        case ITM_HPMtoW:         //   280
        case ITM_WtoHPM:         //   281
        case ITM_HPUKtoW:        //   282
        case ITM_WtoHPUK:        //   283
        case ITM_INCHHGtoPA:     //   284
        case ITM_INCHHGtoPAb:    //   285
        case ITM_PAtoINCHHG:     //   286
        case ITM_PAtoINCHHGb:    //   287
        case ITM_INCHtoMM:       //   288
        case ITM_MMtoINCH:       //   289
        case ITM_WHtoJ:          //   290
        case ITM_JtoWH:          //   291
        case ITM_KGtoLBS:        //   292
        case ITM_LBStoKG:        //   293
        case ITM_GtoOZ:          //   294
        case ITM_OZtoG:          //   295
        case ITM_KGtoSCW:        //   296
        case ITM_KGtoSCWb:       //   297
        case ITM_SCWtoKG:        //   298
        case ITM_SCWtoKGb:       //   299
        case ITM_KGtoSTO:        //   300
        case ITM_KGtoSTOb:       //   301
        case ITM_STOtoKG:        //   302
        case ITM_STOtoKGb:       //   303
        case ITM_KGtoST:         //   304
        case ITM_KGtoSTb:        //   305
        case ITM_KGtoSTc:        //   306
        case ITM_STtoKG:         //   307
        case ITM_STtoKGb:        //   308
        case ITM_STtoKGc:        //   309
        case ITM_KGtoTON:        //   310
        case ITM_KGtoLIANG:      //   311
        case ITM_KGtoLIANGb:     //   312
        case ITM_TONtoKG:        //   313
        case ITM_LIANGtoKG:      //   314
        case ITM_LIANGtoKGb:     //   315
        case ITM_GtoTRZ:         //   316
        case ITM_GtoTRZb:        //   317
        case ITM_TRZtoG:         //   318
        case ITM_TRZtoGb:        //   319
        case ITM_LBFtoN:         //   320
        case ITM_NtoLBF:         //   321
        case ITM_LYtoM:          //   322
        case ITM_MtoLY:          //   323
        case ITM_MMHGtoPA:       //   324
        case ITM_MMHGtoPAb:      //   325
        case ITM_PAtoMMHG:       //   326
        case ITM_PAtoMMHGb:      //   327
        case ITM_MItoKM:         //   328
        case ITM_KMtoMI:         //   329
        case ITM_KMtoNMI:        //   330
        case ITM_NMItoKM:        //   331
        case ITM_MtoPC:          //   332
        case ITM_PCtoM:          //   333
        case ITM_MMtoPOINTb:     //   334
        case ITM_MMtoPOINT:      //   335
        case ITM_MMtoPOINTc:     //   336
        case ITM_POINTtoMM:      //   337
        case ITM_POINTtoMMb:     //   338
        case ITM_POINTtoMMc:     //   339
        case ITM_MtoYD:          //   340
        case ITM_YDtoM:          //   341
        case ITM_PSItoPA:        //   342
        case ITM_PAtoPSI:        //   343
        case ITM_PAtoTOR:        //   344
        case ITM_PAtoTORb:       //   345
        case ITM_TORtoPA:        //   346
        case ITM_TORtoPAb:       //   347
        case ITM_StoYEAR:        //   348
        case ITM_YEARtoS:        //   349
        case ITM_CARATtoG:       //   350
        case ITM_CARATtoGb:      //   351
        case ITM_JINtoKG:        //   352
        case ITM_GtoCARAT:       //   353
        case ITM_GtoCARATb:      //   354
        case ITM_KGtoJIN:        //   355
        case ITM_QTtoL:          //   356
        case ITM_LtoQT:          //   357
        case ITM_FATHOMtoM:      //   358
        case ITM_FATHOMtoMb:     //   359
        case ITM_FATHOMtoMc:     //   360
        case ITM_MtoFATHOM:      //   361
        case ITM_MtoFATHOMb:     //   362
        case ITM_MtoFATHOMc:     //   363
        case ITM_BARRELtoM3:     //   364
        case ITM_BARRELtoM3b:    //   365
        case ITM_M3toBARREL:     //   366
        case ITM_M3toBARRELb:    //   367
        case ITM_ATMtoPAb:       //   368
        case ITM_PAtoATMb:       //   369
        case ITM_HECTAREtoM2:    //   370
        case ITM_M2toHECTARE:    //   371
        case ITM_MUtoM2:         //   372
        case ITM_M2toMU:         //   373
        case ITM_LItoM:          //   374
        case ITM_MtoLI:          //   375
        case ITM_CHItoM:         //   376
        case ITM_MtoCHI:         //   377
        case ITM_YINtoM:         //   378
        case ITM_MtoYIN:         //   379
        case ITM_CUNtoM:         //   380
        case ITM_MtoCUN:         //   381
        case ITM_ZHANGtoM:       //   382
        case ITM_ZHANGtoMb:      //   383
        case ITM_MtoZHANG:       //   384
        case ITM_MtoZHANGb:      //   385
        case ITM_FENtoM:         //   386
        case ITM_MtoFEN:         //   387
        case ITM_LOGICALNAND:    //   402
        case ITM_LOGICALNOR:     //   403
        case ITM_LOGICALXNOR:    //   404
        case ITM_LJ:             //   417
        case ITM_RJ:             //   418
        case ITM_MIRROR:         //   421
        case ITM_NUMB:           //   422
        case ITM_SIGMAPLUS:      //   433
        case ITM_SIGMAMINUS:     //   434
        case ITM_NSIGMA:         //   435
        case ITM_SIGMAx:         //   436
        case ITM_SIGMAy:         //   437
        case ITM_SIGMAx2:        //   438
        case ITM_SIGMAx2y:       //   439
        case ITM_SIGMAy2:        //   440
        case ITM_SIGMAxy:        //   441
        case ITM_SIGMAlnxy:      //   442
        case ITM_SIGMAlnx:       //   443
        case ITM_SIGMAln2x:      //   444
        case ITM_SIGMAylnx:      //   445
        case ITM_SIGMAlny:       //   446
        case ITM_SIGMAln2y:      //   447
        case ITM_SIGMAxlny:      //   448
        case ITM_SIGMAlnyonx:    //   449
        case ITM_SIGMAx2ony:     //   450
        case ITM_SIGMA1onx:      //   451
        case ITM_SIGMA1onx2:     //   452
        case ITM_SIGMAxony:      //   453
        case ITM_SIGMA1ony:      //   454
        case ITM_SIGMA1ony2:     //   455
        case ITM_SIGMAx3:        //   456
        case ITM_SIGMAx4:        //   457
        case ITM_BINOMP:         //  1208
        case ITM_BINOM:          //  1209
        case ITM_BINOMU:         //  1210
        case ITM_BINOMM1:        //  1211
        case ITM_CAUCHP:         //  1213
        case ITM_CAUCH:          //  1214
        case ITM_CAUCHU:         //  1215
        case ITM_CAUCHM1:        //  1216
        case ITM_EXPONP:         //  1218
        case ITM_EXPON:          //  1219
        case ITM_EXPONU:         //  1220
        case ITM_EXPONM1:        //  1221
        case ITM_FPX:            //  1223
        case ITM_FX:             //  1224
        case ITM_FUX:            //  1225
        case ITM_FM1P:           //  1226
        case ITM_GEOMP:          //  1228
        case ITM_GEOM:           //  1229
        case ITM_GEOMU:          //  1230
        case ITM_GEOMM1:         //  1231
        case ITM_HYPERP:         //  1233
        case ITM_HYPER:          //  1234
        case ITM_HYPERU:         //  1235
        case ITM_HYPERM1:        //  1236
        case ITM_LGNRMP:         //  1238
        case ITM_LGNRM:          //  1239
        case ITM_LGNRMU:         //  1240
        case ITM_LGNRMM1:        //  1241
        case ITM_LOGISP:         //  1243
        case ITM_LOGIS:          //  1244
        case ITM_LOGISU:         //  1245
        case ITM_LOGISM1:        //  1246
        case ITM_NBINP:          //  1248
        case ITM_NBIN:           //  1249
        case ITM_NBINU:          //  1250
        case ITM_NBINM1:         //  1251
        case ITM_NORMLP:         //  1253
        case ITM_NORML:          //  1254
        case ITM_NORMLU:         //  1255
        case ITM_NORMLM1:        //  1256
        case ITM_POISSP:         //  1258
        case ITM_POISS:          //  1259
        case ITM_POISSU:         //  1260
        case ITM_POISSM1:        //  1261
        case ITM_TPX:            //  1263
        case ITM_TX:             //  1264
        case ITM_TUX:            //  1265
        case ITM_TM1P:           //  1266
        case ITM_WEIBLP:         //  1268
        case ITM_WEIBL:          //  1269
        case ITM_WEIBLU:         //  1270
        case ITM_WEIBLM1:        //  1271
        case ITM_chi2Px:         //  1273
        case ITM_chi2x:          //  1274
        case ITM_chi2ux:         //  1275
        case ITM_chi2M1:         //  1276
        case ITM_EXPF:           //  1298
        case ITM_LINF:           //  1299
        case ITM_LOGF:           //  1300
        case ITM_ORTHOF:         //  1301
        case ITM_POWERF:         //  1302
        case ITM_GAUSSF:         //  1303
        case ITM_CAUCHF:         //  1304
        case ITM_PARABF:         //  1305
        case ITM_HYPF:           //  1306
        case ITM_ROOTF:          //  1307
        case ITM_1COMPL:         //  1404
        case ITM_SNAP:           //  1405
        case ITM_2COMPL:         //  1406
        case ITM_ABS:            //  1407
        case ITM_AGM:            //  1408
        case ITM_BATT:           //  1413
        case ITM_BEEP:           //  1414
        case ITM_BEGINP:         //  1415
        case ITM_BN:             //  1416
        case ITM_BNS:            //  1417
        case ITM_CLCVAR:         //  1420
        case ITM_CLFALL:         //  1421
        case ITM_TGLFRT:         //  1422
        case ITM_CLLCD:          //  1423
        case ITM_CLMENU:         //  1424
        case ITM_CLSTK:          //  1428
        case ITM_CLSIGMA:        //  1429
        case ITM_CONJ:           //  1431
        case ITM_CORR:           //  1433
        case ITM_COV:            //  1434
        case ITM_BESTFQ:         //  1435
        case ITM_CROSS_PROD:     //  1436
        case ITM_CXtoRE:         //  1437
        case ITM_DATE:           //  1438
        case ITM_DATEto:         //  1439
        case ITM_DAY:            //  1440
        case ITM_DBLR:           //  1441
        case ITM_DBLMULT:        //  1442
        case ITM_DBLDIV:         //  1443
        case ITM_DECOMP:         //  1444
        case ITM_DEG:            //  1445
        case ITM_DEGto:          //  1446
        case ITM_SA:             //  1447
        case ITM_DENMAX:         //  1448
        case ITM_DOT_PROD:       //  1449
        case ITM_DMS:            //  1451
        case ITM_DMSto:          //  1452
        case ITM_DMY:            //  1453
        case ITM_DtoJ:           //  1454
        case ITM_EIGVAL:         //  1456
        case ITM_EIGVEC:         //  1457
        case ITM_ENDP:           //  1459
        case ITM_ENORM:          //  1461
        case ITM_ERF:            //  1466
        case ITM_ERFC:           //  1467
        case ITM_EXITALL:        //  1469
        case ITM_EXPT:           //  1470
        case ITM_GET_JUL_GREG:   //  1471
        case ITM_FIB:            //  1472
        case ITM_FLASH:          //  1474
        case ITM_GD:             //  1478
        case ITM_GDM1:           //  1479
        case ITM_GRAD:           //  1480
        case ITM_GRADto:         //  1481
        case ITM_HN:             //  1483
        case ITM_HNP:            //  1484
        case ITM_IM:             //  1485
        case ITM_IXYZ:           //  1487
        case ITM_IGAMMAP:        //  1488
        case ITM_IGAMMAQ:        //  1489
        case ITM_IPLUS:          //  1490
        case ITM_IMINUS:         //  1491
        case ITM_JYX:            //  1492
        case ITM_JPLUS:          //  1493
        case ITM_JMINUS:         //  1494
        case ITM_JUL_GREG:       //  1495
        case ITM_JtoD:           //  1496
        case ITM_sinc:           //  1500
        case ITM_LASTX:          //  1502
        case ITM_LEAP:           //  1504
        case ITM_Lm:             //  1505
        case ITM_LmALPHA:        //  1506
        case ITM_LNBETA:         //  1507
        case ITM_LNGAMMA:        //  1508
        case ITM_LocRQ:          //  1515
        case ITM_LR:             //  1516
        case ITM_MANT:           //  1517
        case ITM_MEM:            //  1519
        case ITM_MENU:           //  1520
        case ITM_MONTH:          //  1521
        case ITM_MSG:            //  1522
        case ITM_MULPI:          //  1523
        case ITM_M_DIM:          //  1526
        case ITM_M_DIMQ:         //  1527
        case ITM_MDY:            //  1528
        case ITM_M_GET:          //  1531
        case ITM_M_LU:           //  1535
        case ITM_M_NEW:          //  1536
        case ITM_M_PUT:          //  1538
        case ITM_M_RR:           //  1539
        case ITM_sincpi:         //  1540
        case ITM_NOP:            //  1542
        case ITM_OFF:            //  1543
        case ITM_DROPY:          //  1544
        case ITM_PIXEL:          //  1548
        case ITM_PLOT:           //  1549
        case ITM_Pn:             //  1550
        case ITM_POINT:          //  1551
        case ITM_POPLR:          //  1553
        case ITM_PRCL:           //  1554
        case ITM_RAD:            //  1557
        case ITM_RADto:          //  1558
        case ITM_RAN:            //  1559
        case ITM_RCLEL:          //  1562
        case ITM_RCLIJ:          //  1563
        case ITM_RE:             //  1566
        case ITM_RECV:           //  1567
        case ITM_REtoCX:         //  1569
        case ITM_REexIM:         //  1570
        case ITM_RMQ:            //  1572
        case ITM_RNORM:          //  1574
        case ITM_EX1:            //  1575
        case ITM_ROUNDI:         //  1576
        case ITM_RSUM:           //  1578
        case ITM_R_CLR:          //  1580
        case ITM_R_COPY:         //  1581
        case ITM_R_SORT:         //  1582
        case ITM_R_SWAP:         //  1583
        case ITM_am:             //  1584
        case ITM_STDDEVWEIGHTED: //  1585
        case ITM_SDIGS:          //  1588
        case ITM_SEED:           //  1589
        case ITM_SEND:           //  1590
        case ITM_SETCHN:         //  1591
        case ITM_SETDAT:         //  1592
        case ITM_SETEUR:         //  1593
        case ITM_SETIND:         //  1594
        case ITM_SETJPN:         //  1595
        case ITM_SETSIG:         //  1596
        case ITM_SETTIM:         //  1597
        case ITM_SETUK:          //  1598
        case ITM_SETUSA:         //  1599
        case ITM_SIGN:           //  1600
        case ITM_SIGNMT:         //  1601
        case ITM_SLVQ:           //  1604
        case ITM_SM:             //  1605
        case ITM_ISM:            //  1606
        case ITM_SMW:            //  1607
        case ITM_SSIZE:          //  1609
        case ITM_STOEL:          //  1612
        case ITM_STOIJ:          //  1613
        case ITM_LN1X:           //  1614
        case ITM_SUM:            //  1616
        case ITM_SW:             //  1617
        case ITM_SXY:            //  1618
        case ITM_TICKS:          //  1620
        case ITM_TIME:           //  1621
        case ITM_Tn:             //  1623
        case ITM_ULP:            //  1626
        case ITM_Un:             //  1627
        case ITM_UNITV:          //  1628
        case ITM_UNSIGN:         //  1629
        case ITM_IDIVR:          //  1632
        case ITM_WDAY:           //  1633
        case ITM_WM:             //  1635
        case ITM_WP:             //  1636
        case ITM_WM1:            //  1637
        case ITM_WSIZEQ:         //  1639
        case ITM_XBAR:           //  1640
        case ITM_XG:             //  1641
        case ITM_XW:             //  1642
        case ITM_XCIRC:          //  1643
        case ITM_XtoDATE:        //  1644
        case ITM_XtoALPHA:       //  1645
        case ITM_YEAR:           //  1647
        case ITM_YCIRC:          //  1648
        case ITM_YMD:            //  1649
        case ITM_XMAX:           //  1653
        case ITM_XMIN:           //  1654
        case ITM_BETAXY:         //  1661
        case ITM_gammaXY:        //  1662
        case ITM_GAMMAXY:        //  1663
        case ITM_GAMMAX:         //  1664
        case ITM_DELTAPC:        //  1666
        case ITM_SCATTFACT:      //  1667
        case ITM_SCATTFACTm:     //  1668
        case ITM_SCATTFACTp:     //  1669
        case ITM_zetaX:          //  1670
        case ITM_STDDEV:         //  1673
        case ITM_STDDEVPOP:      //  1674
        case ITM_RANI:           //  1675
        case ITM_PRINTERX:       //  1676
        case ITM_RANGE:          //  1677
        case ITM_GETRANGE:       //  1678
        case ITM_M1X:            //  1679
        case ITM_XMOD:           //  1680
        case ITM_toDATE:         //  1681
        case ITM_sn:             //  1682
        case ITM_cn:             //  1683
        case ITM_dn:             //  1684
        case ITM_toHR:           //  1685
        case ITM_toHMS:          //  1686
        case ITM_toPOL:          //  1688
        case ITM_MPItoR:         //  1689
        case ITM_RtoMPI:         //  1690
        case ITM_toREAL:         //  1691
        case ITM_toREC:          //  1692
        case ITM_DtoDMS:         //  1693
        case ITM_PC:             //  1695
        case ITM_PCMRR:          //  1696
        case ITM_PCT:            //  1697
        case ITM_PCSIGMA:        //  1698
        case ITM_PCPMG:          //  1699
        case ITM_PMOD:           //  1701
        case ITM_M_DET:          //  1702
        case ITM_PARALLEL:       //  1703
        case ITM_M_TRANSP:       //  1704
        case ITM_M_INV:          //  1705
        case ITM_ARG:            //  1706
        case ITM_MULPIto:        //  1707
        case ITM_PRINTERADV:     //  1708
        case ITM_PRINTERLCD:     //  1711
        case ITM_PRINTERPROG:    //  1713
        case ITM_PRINTERREGS:    //  1715
        case ITM_PRINTERSTK:     //  1716
        case ITM_PRINTERUSER:    //  1718
        case ITM_PRINTERWIDTH:   //  1719
        case ITM_PRINTERSIGMA:   //  1720
        case ITM_PRINTERHASH:    //  1721
        case ITM_Kk:             //  1726
        case ITM_Ek:             //  1727
        case ITM_PInk:           //  1728
        case ITM_ANGLE:          //  1739
        case ITM_SHOW:           //  1742
        case ITM_DMStoD:         //  1744
        case ITM_VANGLE:         //  1745
        case ITM_XH:             //  1746
        case ITM_XRMS:           //  1747
        case ITM_ACOS:           //  1748
        case ITM_ASIN:           //  1749
        case ITM_ATAN:           //  1750
        case ITM_DET:            //  1751
        case ITM_INVRT:          //  1752
        case ITM_TRANS:          //  1753
        case ITM_HIDE:           //  1757
        case ITM_Fphik:          //  1763
        case ITM_Ephik:          //  1764
        case ITM_ZETAphik:       //  1765
        case ITM_GETHIDE:        //  1766
        case ITM_SQRT:           //  1768
        case ITM_atan2:          //  1775
          runFunction(item16);
          return temporaryInformation == TI_FALSE ? 2 : 1;

        case ITM_END:            //  1458
        case ITM_RTNP1:          //  1579
          runFunction(item16);
          return 0;

        case 0x7fff:             // 32767  .END.
          fnReturn(0);
          return 0;

        default: {
          #ifndef DMCP_BUILD
            printf("\nERROR in decodeOneStep: double byte instruction %u is unknown!\n", item16);
          #endif // !DMCP_BUILD
          return 0;
        }
      }
  }
#endif // TESTSUITE_BUILD
}



void runProgram(bool_t singleStep) {
#ifndef TESTSUITE_BUILD
  bool_t nestedEngine = (programRunStop == PGM_RUNNING);
  uint16_t startingSubLevel = currentSubroutineLevel;
  lastErrorCode = ERROR_NONE;
  hourGlassIconEnabled = true;
  programRunStop = PGM_RUNNING;
  showHideHourGlass();
  #ifdef DMCP_BUILD
    lcd_refresh();
  #else // !DMCP_BUILD
    refreshLcd(NULL);
  #endif // DMCP_BUILD

  while(1) {
    int16_t stepsToBeAdvanced;
    uint16_t subLevel = currentSubroutineLevel;
    if(temporaryInformation == TI_TRUE || temporaryInformation == TI_FALSE || temporaryInformation == TI_SOLVER_FAILED) {
      temporaryInformation = TI_NO_INFO;
    }
    stepsToBeAdvanced = executeOneStep(currentStep);
    switch(stepsToBeAdvanced) {
      case -1: // Already the pointer is set
        break;

      case 0: // End of the routine
        if(subLevel == startingSubLevel) {
          goto stopProgram;
        }
        break;

      default: // Find the next step
        fnSkip((uint16_t)(stepsToBeAdvanced - 1));
        break;
    }

    if(lastErrorCode != ERROR_NONE) {
      if(getSystemFlag(FLAG_IGN1ER)) {
        lastErrorCode = ERROR_NONE;
        clearSystemFlag(FLAG_IGN1ER);
      }
      else {
        break;
      }
    }
    #ifdef DMCP_BUILD
      if(!nestedEngine) {
        int key = key_pop();
        key = convertKeyCode(key);
        if(key == 36 || key == 37) {
          programRunStop = PGM_WAITING;
          refreshScreen();
          lcd_refresh();
          fnTimerStart(TO_KB_ACTV, TO_KB_ACTV, 60000);
          do {
            key = key_pop();
            sys_sleep();
          } while(key == -1);
          break;
        }
      }
    #endif // DMCP_BUILD
    if(programRunStop != PGM_RUNNING) {
      break;
    }
    if(singleStep) {
      break;
    }
    #ifdef PC_BUILD
      refreshLcd(NULL);
    #endif // PC_BUILD
  }

stopProgram:
  if(programRunStop == PGM_RUNNING && !nestedEngine) {
    programRunStop = PGM_STOPPED;
  }
  showHideHourGlass();
  #ifdef DMCP_BUILD
    lcd_refresh();
    fnTimerStart(TO_KB_ACTV, TO_KB_ACTV, FAST_SCREEN_REFRESH_PERIOD+50);
  #else // !DMCP_BUILD
    refreshLcd(NULL);
  #endif // DMCP_BUILD
  return;
#endif // TESTSUITE_BUILD
}



void execProgram(uint16_t label) {
  uint16_t origLocalStepNumber = currentLocalStepNumber;
  uint8_t *origStep = currentStep;
  fnExecute(label);
  if(programRunStop == PGM_RUNNING && (getSystemFlag(FLAG_INTING) || getSystemFlag(FLAG_SOLVING))) {
    runProgram(false);
    currentLocalStepNumber = origLocalStepNumber;
    currentStep = origStep;
  }
}
