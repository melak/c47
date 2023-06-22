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
#include "conversionAngles.h"
#include "dateTime.h"
#include "defines.h"
#include "error.h"
#include "flags.h"
#include "fonts.h"
#include "items.h"
#include "keyboard.h"
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
#include "store.h"
#include "timer.h"
#include "ui/tam.h"

#include "wp43.h"



void fnGoto(uint16_t label) {
  if(tam.mode || calcMode != CM_PEM) {
    if(dynamicMenuItem >= 0) {
      goToGlobalStep(label);
      return;
    }

    // Local Label 00 to 99 and A, B, C, D, and E
    if(label <= 104) {
      // Search for local label
      for(uint16_t lbl=0; lbl<numberOfLabels; lbl++) {
        if(labelList[lbl].program == currentProgramNumber && labelList[lbl].step < 0 && *(labelList[lbl].labelPointer) == label) { // Is in the current program and is a local label and is the searched label
          if(programRunStop == PGM_RUNNING) {
            currentLocalStepNumber = (-labelList[lbl].step) - programList[currentProgramNumber - 1].step + 1;
            currentStep = labelList[lbl].labelPointer - 1;
          }
          else {
            goToGlobalStep(-labelList[lbl].step);
          }
          return;
        }
      }

      displayCalcErrorMessage(ERROR_LABEL_NOT_FOUND, ERR_REGISTER_LINE, REGISTER_X);
      #if(EXTRA_INFO_ON_CALC_ERROR == 1)
        if(label < REGISTER_X) {
          sprintf(errorMessage, "there is no local label %02u in current program", label);
        }
        else {
          sprintf(errorMessage, "there is no local label %c in current program", 'A' + (label - 100));
        }
        moreInfoOnError("In function fnGoto:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else if(label >= FIRST_LABEL && label <= LAST_LABEL) { // Global named label
      if((label - FIRST_LABEL) < numberOfLabels) {
        goToGlobalStep((int16_t)labelList[label - FIRST_LABEL].step);
        return;
      }
      else {
        displayCalcErrorMessage(ERROR_LABEL_NOT_FOUND, ERR_REGISTER_LINE, REGISTER_X);
        #if(EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "label ID %u out of range", label - FIRST_LABEL);
          moreInfoOnError("In function fnGoto:", errorMessage, NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      }
    }
    else {
      displayCalcErrorMessage(ERROR_LABEL_NOT_FOUND, ERR_REGISTER_LINE, REGISTER_X);
      #if(EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "invalid parameter %u", label);
        moreInfoOnError("In function fnGoto:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }
  else {
    insertStepInProgram(ITM_GTO);
  }
}



void goToGlobalStep(int32_t step) {
  if(dynamicMenuItem >= 0) {
    int16_t dupNum = 0;
    uint8_t *labelName = (uint8_t *)dynmenuGetLabelWithDup(dynamicMenuItem, &dupNum);

    if(*labelName == 0) {
      return;
    }

    int16_t c, len = stringByteLength((char *)labelName);
    for(uint16_t lbl=0; lbl<numberOfLabels; lbl++) {
      uint8_t *lblPtr;
      lblPtr = labelList[lbl].labelPointer;
      if(labelList[lbl].step > 0 && *lblPtr == len) { // It's a global label and the length is OK
        for(c=0; c<len; c++) {
          if(labelName[c] != lblPtr[c + 1]) {
            break;
          }
        }
        if(c == len) {
          if(dupNum <= 0) {
            step = labelList[lbl].step;
            break;
          }
          else {
            --dupNum;
          }
        }
      }
    }
  }

  defineCurrentProgramFromGlobalStepNumber(step);
  currentLocalStepNumber = abs(step) - abs(programList[currentProgramNumber - 1].step) + 1;

  uint8_t *stepPointer = beginOfCurrentProgram;
  step = 1;
  while(true) {
    if(step == currentLocalStepNumber) {
      currentStep = stepPointer;
      break;
    }

    stepPointer = findNextStep(stepPointer);
    step++;
  }

  if(currentLocalStepNumber >= 3) {
    firstDisplayedLocalStepNumber = currentLocalStepNumber - 3;
    uint16_t numberOfSteps = getNumberOfSteps();
    if(firstDisplayedLocalStepNumber + 6 > numberOfSteps) {
      for(int i=3+currentLocalStepNumber-numberOfSteps; i>0; i--) {
        if(firstDisplayedLocalStepNumber > 0) {
          firstDisplayedLocalStepNumber--;
        }
      }
    }
    defineFirstDisplayedStep();
  }
  else {
    firstDisplayedLocalStepNumber = 0;
    firstDisplayedStep = beginOfCurrentProgram;
  }
}



void goToPgmStep(uint16_t program, uint16_t step) {
  int32_t globalStepNumber = programList[program - 1].step;
  goToGlobalStep(globalStepNumber + step - 1);
}



void fnGotoDot(uint16_t globalStepNumber) {
  goToGlobalStep((int16_t)globalStepNumber);
}




void fnExecute(uint16_t label) {
  if(programRunStop == PGM_RUNNING) {
    dataBlock_t *_currentSubroutineLevelData = currentSubroutineLevelData;
    allSubroutineLevels.numberOfSubroutineLevels += 1;
    currentSubroutineLevelData = allocWp43(3);
    if(currentSubroutineLevelData) {
      _currentSubroutineLevelData[2].ptrToNextLevel = TO_WP43MEMPTR(currentSubroutineLevelData);
      currentReturnProgramNumber = currentProgramNumber;
      currentReturnLocalStep = currentLocalStepNumber;
      currentNumberOfLocalRegisters = 0; // No local register
      currentNumberOfLocalFlags = 0; // No local flags
      currentSubroutineLevel = allSubroutineLevels.numberOfSubroutineLevels - 1;
      currentPtrToNextLevel = WP43_NULL;
      currentPtrToPreviousLevel = TO_WP43MEMPTR(_currentSubroutineLevelData);
      currentLocalFlags = NULL;
      currentLocalRegisters = NULL;

      fnGoto(label);
      dynamicMenuItem = -1;
      if(lastErrorCode != ERROR_NONE) {
        fnReturn(0);
        fnBack(1);
      }
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
    if(lastErrorCode == ERROR_NONE) {
      #if !defined(TESTSUITE_BUILD)
        if(tam.mode) {
          tamLeaveMode();
          refreshScreen();
        }
      #endif // TESTSUITE_BUILD
      runProgram(false, INVALID_VARIABLE);
    }
  }
}



void fnReturn(uint16_t skip) {
  dataBlock_t *_currentSubroutineLevelData = currentSubroutineLevelData;
  uint16_t sizeToBeFreedInBlocks;

  /* Cancel INPUT */
  if(currentInputVariable != INVALID_VARIABLE) {
    currentInputVariable = INVALID_VARIABLE;
    refreshScreen();
    #if defined(DMCP_BUILD)
      lcd_refresh();
    #else // !DMCP_BUILD
      refreshLcd(NULL);
    #endif // DMCP_BUILD
  }

  /* A subroutine is running */
  if(currentSubroutineLevel > 0) {
    if(programRunStop == PGM_RUNNING) {
      currentProgramNumber = currentReturnProgramNumber;
      currentLocalStepNumber = currentReturnLocalStep + 1;
      defineCurrentStep();
    }
    else {
      uint16_t returnGlobalStepNumber = currentReturnLocalStep + programList[currentReturnProgramNumber - 1].step; // the next step
      goToGlobalStep(returnGlobalStepNumber);
    }

    if(skip > 0 && !isAtEndOfProgram(currentStep) && !isAtEndOfPrograms(currentStep)) {
      ++currentLocalStepNumber;
      currentStep = findNextStep(currentStep);
    }
    if(currentNumberOfLocalRegisters > 0) {
      allocateLocalRegisters(0);
      _currentSubroutineLevelData = currentSubroutineLevelData;
    }
    sizeToBeFreedInBlocks = 3 + (currentNumberOfLocalFlags > 0 ? 1 : 0);
    currentSubroutineLevelData = TO_PCMEMPTR(currentPtrToPreviousLevel);
    freeWp43(_currentSubroutineLevelData, sizeToBeFreedInBlocks);
    currentPtrToNextLevel = WP43_NULL;
    allSubroutineLevels.numberOfSubroutineLevels -= 1;

    currentLocalFlags = (currentNumberOfLocalFlags == 0 ? NULL : currentSubroutineLevelData + 3);
    currentLocalRegisters = (registerHeader_t *)(currentNumberOfLocalRegisters == 0 ? NULL : currentSubroutineLevelData + (currentLocalFlags == NULL ? 3 : 4));
  }

  /* Not in a subroutine */
  else {
    goToPgmStep(currentProgramNumber, 1);
    if(currentNumberOfLocalRegisters > 0) {
      allocateLocalRegisters(0);
    }
    if(currentNumberOfLocalFlags > 0) {
      freeWp43(currentSubroutineLevelData + 3, 1);
      currentNumberOfLocalFlags = 0;
    }
    currentLocalFlags = NULL;
    currentLocalRegisters = NULL;
    pemCursorIsZerothStep = true;
  }
}



void fnRunProgram(uint16_t unusedButMandatoryParameter) {
  if(currentInputVariable != INVALID_VARIABLE) {
    if(currentInputVariable & 0x8000) {
      fnDropY(NOPARAM);
    }
    fnStore(currentInputVariable & 0x3fff);
    currentInputVariable = INVALID_VARIABLE;
  }
  dynamicMenuItem = -1;
  runProgram(false, INVALID_VARIABLE);
}



void fnStopProgram(uint16_t unusedButMandatoryParameter) {
  programRunStop = PGM_WAITING;
}



#if !defined(TESTSUITE_BUILD)
static void _getStringLabelOrVariableName(uint8_t *stringAddress) {
  uint8_t stringLength = *(uint8_t *)(stringAddress++);
  xcopy(tmpStringLabelOrVariableName, stringAddress, stringLength);
  tmpStringLabelOrVariableName[stringLength] = 0;
}

static void _executeWithIndirectRegister(uint8_t *paramAddress, uint16_t op) {
  uint8_t opParam = *(uint8_t *)paramAddress;
  if(opParam <= LAST_LOCAL_REGISTER) { // Local register from .00 to .98
      int16_t realParam = indirectAddressing(opParam, (indexOfItems[op].param == TM_FLAGR || indexOfItems[op].param == TM_FLAGW) ? INDPM_FLAG : (indexOfItems[op].param == TM_STORCL || indexOfItems[op].param == TM_M_DIM) ? INDPM_REGISTER : INDPM_PARAM, indexOfItems[op].tamMinMax >> TAM_MAX_BITS, indexOfItems[op].tamMinMax & TAM_MAX_MASK);
      if(realParam < 9999) {
        reallyRunFunction(op, realParam);
      }
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
      int16_t realParam = indirectAddressing(regist, (indexOfItems[op].param == TM_FLAGR || indexOfItems[op].param == TM_FLAGW) ? INDPM_FLAG : (indexOfItems[op].param == TM_STORCL || indexOfItems[op].param == TM_M_DIM) ? INDPM_REGISTER : INDPM_PARAM, indexOfItems[op].tamMinMax >> TAM_MAX_BITS, indexOfItems[op].tamMinMax & TAM_MAX_MASK);
      if(realParam < 9999) {
        reallyRunFunction(op, realParam);
      }
  }
  else {
    displayCalcErrorMessage(ERROR_UNDEF_SOURCE_VAR, ERR_REGISTER_LINE, REGISTER_X);
    #if(EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "string '%s' is not a named variable", tmpStringLabelOrVariableName);
      moreInfoOnError("In function _executeOp:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}

static void _executeOp(uint8_t *paramAddress, uint16_t op, uint16_t paramMode) {
  uint8_t opParam = *(uint8_t *)(paramAddress++);
  bool_t tryAllocate = (op == ITM_STO || op == ITM_M_DIM || op == ITM_MVAR || op == ITM_INPUT);

  switch(paramMode) {
      case PARAM_DECLARE_LABEL: {
      // nothing to do
      break;
      }

      case PARAM_LABEL: {
      if(opParam <= 104) { // Local label from 00 to 99 or from A to E
        reallyRunFunction(op, opParam);
      }
      else if(opParam == STRING_LABEL_VARIABLE) {
        _getStringLabelOrVariableName(paramAddress);
        calcRegister_t label = findNamedLabel(tmpStringLabelOrVariableName);
        if(label != INVALID_VARIABLE || op == ITM_LBLQ) {
          reallyRunFunction(op, label);
        }
        else {
          displayCalcErrorMessage(ERROR_LABEL_NOT_FOUND, ERR_REGISTER_LINE, REGISTER_X);
          #if(EXTRA_INFO_ON_CALC_ERROR == 1)
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
      }

      case PARAM_FLAG: {
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
          case FLAG_alphaCAP:
          case FLAG_RUNTIM:
          case FLAG_RUNIO:
          case FLAG_PRINTS:
          case FLAG_LOWBAT:
          case FLAG_ASLIFT:
          case FLAG_INTING:
          case FLAG_SOLVING:
          case FLAG_VMDISP:
          case FLAG_USB:
            case FLAG_ENDPMT: {
            reallyRunFunction(op, (uint16_t)(*paramAddress) | 0xc000);
            break;
            }
            default: {
            reallyRunFunction(op, (uint16_t)(*paramAddress) | 0x8000);
        }
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
      }

      case PARAM_NUMBER_8: {
      if(opParam <= (indexOfItems[op].tamMinMax & TAM_MAX_MASK)) { // Value from 0 to 99
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
      }

      case PARAM_NUMBER_8_16: {
        if(opParam <= 249) { // Value from 0 to 249
          reallyRunFunction(op, opParam);
        }
        else if(opParam == CNST_BEYOND_250) { // Value from 250 to 499
          reallyRunFunction(op, 250 + *(paramAddress));
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
      }

      case PARAM_NUMBER_16: {
      reallyRunFunction(op, opParam + 256 * *(paramAddress));
      break;
      }

    case PARAM_REGISTER:
      case PARAM_COMPARE: {
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
          #if(EXTRA_INFO_ON_CALC_ERROR == 1)
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
      }

    case PARAM_SKIP_BACK:
      case PARAM_SHUFFLE: {
      reallyRunFunction(op, opParam);
      break;
      }

      default: {
      sprintf(tmpString, "\nIn function _executeOp: paramMode %u is not valid!\n", paramMode);
  }
}
  }

static void _putLiteral(uint8_t *literalAddress) {
  switch(*(uint8_t *)(literalAddress++)) {
      case BINARY_SHORT_INTEGER: {
      liftStack();
      setSystemFlag(FLAG_ASLIFT);
      reallocateRegister(REGISTER_X, dtShortInteger, SHORT_INTEGER_SIZE, *(uint8_t *)(literalAddress++));
      xcopy(REGISTER_DATA(REGISTER_X), literalAddress, TO_BYTES(SHORT_INTEGER_SIZE));
      break;
      }

      //case BINARY_LONG_INTEGER: {
    //  break;
      //}

      case BINARY_REAL34: {
      liftStack();
      setSystemFlag(FLAG_ASLIFT);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      real34Copy((real34_t *)literalAddress, REGISTER_REAL34_DATA(REGISTER_X));
      break;
      }

      case BINARY_COMPLEX34: {
        complex34_t complexLiteral;
        liftStack();
        setSystemFlag(FLAG_ASLIFT);
        reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
        xcopy(VARIABLE_REAL34_DATA(&complexLiteral), literalAddress     , 16);
        xcopy(VARIABLE_IMAG34_DATA(&complexLiteral), literalAddress + 16, 16);
        complex34Copy(&complexLiteral, REGISTER_COMPLEX34_DATA(REGISTER_X));
        break;
      }

      //case BINARY_DATE: {
      //  break;
      //}

      //case BINARY_TIME: {
      //  break;
      //}

      case STRING_SHORT_INTEGER: {
        uint8_t base = *literalAddress;
        longInteger_t val;
        longIntegerInit(val);

        _getStringLabelOrVariableName(literalAddress + 1);
        stringToLongInteger(tmpStringLabelOrVariableName, base, val);
        liftStack();
        setSystemFlag(FLAG_ASLIFT);
        convertLongIntegerToShortIntegerRegister(val, (uint32_t)(*literalAddress), REGISTER_X);

        longIntegerFree(val);
      break;
      }

      case STRING_LONG_INTEGER: {
        longInteger_t val;
        longIntegerInit(val);

        _getStringLabelOrVariableName(literalAddress);
        stringToLongInteger(tmpStringLabelOrVariableName, 10, val);
        liftStack();
        setSystemFlag(FLAG_ASLIFT);
        convertLongIntegerToLongIntegerRegister(val, REGISTER_X);

        longIntegerFree(val);
        break;
      }

      case STRING_REAL34: {
      _getStringLabelOrVariableName(literalAddress);
      liftStack();
      setSystemFlag(FLAG_ASLIFT);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      stringToReal34(tmpStringLabelOrVariableName, REGISTER_REAL34_DATA(REGISTER_X));
      break;
      }

      case STRING_COMPLEX34: {
        char *imag = tmpStringLabelOrVariableName;
        _getStringLabelOrVariableName(literalAddress);
        while(*imag != 'i' && *imag != 0) {
          ++imag;
        }
        if(*imag == 'i') {
          if(imag > tmpStringLabelOrVariableName && *(imag - 1) == '-') {
            *imag = '-'; *(imag - 1) = 0;
          }
          else if(imag > tmpStringLabelOrVariableName && *(imag - 1) == '+') {
            *imag = 0; *(imag - 1) = 0;
            ++imag;
          }
          else {
            *imag = 0;
          }
        }
        liftStack();
        setSystemFlag(FLAG_ASLIFT);
        reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
        stringToReal34(tmpStringLabelOrVariableName, REGISTER_REAL34_DATA(REGISTER_X));
        stringToReal34(imag,                         REGISTER_IMAG34_DATA(REGISTER_X));
        break;
      }

      case STRING_LABEL_VARIABLE: {
      _getStringLabelOrVariableName(literalAddress);
      liftStack();
      setSystemFlag(FLAG_ASLIFT);
      reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(stringByteLength(tmpStringLabelOrVariableName) + 1), amNone);
      xcopy(REGISTER_STRING_DATA(REGISTER_X), tmpStringLabelOrVariableName, stringByteLength(tmpStringLabelOrVariableName) + 1);
      break;
      }

      case STRING_DATE: {
      _getStringLabelOrVariableName(literalAddress);
      liftStack();
      setSystemFlag(FLAG_ASLIFT);
      reallocateRegister(REGISTER_X, dtDate, REAL34_SIZE, amNone);
      stringToReal34(tmpStringLabelOrVariableName, REGISTER_REAL34_DATA(REGISTER_X));
      julianDayToInternalDate(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
      break;
      }

      case STRING_TIME: {
      _getStringLabelOrVariableName(literalAddress);
      liftStack();
      setSystemFlag(FLAG_ASLIFT);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      stringToReal34(tmpStringLabelOrVariableName, REGISTER_REAL34_DATA(REGISTER_X));
      hmmssInRegisterToSeconds(REGISTER_X);
      break;
      }

      case STRING_ANGLE_DMS: {
      _getStringLabelOrVariableName(literalAddress);
      liftStack();
      setSystemFlag(FLAG_ASLIFT);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amDMS);
      stringToReal34(tmpStringLabelOrVariableName, REGISTER_REAL34_DATA(REGISTER_X));
      real34FromDmsToDeg(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(REGISTER_X));
      break;
      }

    default: {
        #if !defined(DMCP_BUILD)
        printf("\nERROR: %u is not an acceptable parameter for ITM_LITERAL!\n", *(uint8_t *)(literalAddress - 1));
      #endif // !DMCP_BUILD
    }
  }
}
#endif // !TESTSUITE_BUILD



int16_t executeOneStep(uint8_t *step) {
  #if defined(TESTSUITE_BUILD)
    return 0;
  #else // TESTSUITE_BUILD
  uint16_t op;

  op = *(step++);
  if(op & 0x80) {
    op &= 0x7f;
    op <<= 8;
    op |= *(step++);
  }

  switch(op) {
    case ITM_GTO:         //     2
    case ITM_XEQ:         //     3
    case ITM_BACK:        //  1412
    case ITM_CASE:        //  1418
    case ITM_SKIP: {      //  1603
      _executeOp(step, op, (indexOfItems[op].status & PTP_STATUS) >> 9);
      return -1;
      }

    case ITM_RTN:         //     4
    case ITM_END:         //  1458
    case ITM_RTNP1: {     //  1579
      runFunction(op);
      return 0;
    }

    case 0x7fff: {        // 32767  .END.
      fnReturn(0);
      return 0;
    }

    case ITM_SOLVE: {     //  1608
      _executeOp(step, op, PARAM_REGISTER);
      if(temporaryInformation == TI_SOLVER_FAILED) {
        lastErrorCode = ERROR_NONE;
        return 2;
      }
      else {
        return 1;
      }
    }

    default: {
      switch(indexOfItems[op].status & PTP_STATUS) {
        case PTP_NONE: {
          runFunction(op);
          break;
        }

        case PTP_DECLARE_LABEL: {
          return 1;
        }

        case PTP_DISABLED: {
          displayCalcErrorMessage(ERROR_NON_PROGRAMMABLE_COMMAND, ERR_REGISTER_LINE, REGISTER_X);
          #if(EXTRA_INFO_ON_CALC_ERROR == 1)
            moreInfoOnError("In function decodeOneStep:", "non-programmable function", indexOfItems[op].itemCatalogName, "appeared in the program!");
          #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
          return 0;
        }

        case PTP_LITERAL: {
          _putLiteral(step);
          return 1;
        }

        case PTP_KEYG_KEYX: {
          _executeOp(step, op, PARAM_NUMBER_8);
          break;
        }

        default: {
          _executeOp(step, op, (indexOfItems[op].status & PTP_STATUS) >> 9);
        }
      }
      return temporaryInformation == TI_FALSE ? 2 : 1;
    }
  }
  #endif // !TESTSUITE_BUILD
}



void runProgram(bool_t singleStep, uint16_t menuLabel) {
  #if !defined(TESTSUITE_BUILD)
  bool_t nestedEngine = (programRunStop == PGM_RUNNING);
  uint16_t startingSubLevel = (nestedEngine && menuLabel == INVALID_VARIABLE) ? currentSubroutineLevel : 0;
  lastErrorCode = ERROR_NONE;
  hourGlassIconEnabled = true;
  programRunStop = PGM_RUNNING;
  if(!getSystemFlag(FLAG_INTING) && !getSystemFlag(FLAG_SOLVING)) {
    showHideHourGlass();
    #if defined(DMCP_BUILD)
      lcd_refresh();
    #else // !DMCP_BUILD
      refreshLcd(NULL);
    #endif // DMCP_BUILD
  }

  if(menuLabel != INVALID_VARIABLE) {
    fnBack(1);
    if(menuLabel & 0x8000) {
      fnExecute(menuLabel & 0x7fff);
    }
    else {
      fnGoto(menuLabel & 0x7fff);
    }
  }

  while(1) {
    int16_t stepsToBeAdvanced;
    uint16_t subLevel = currentSubroutineLevel;
    uint16_t opCode = *currentStep;
    currentInputVariable = INVALID_VARIABLE; // INPUT is already executed
    if(opCode & 0x80) {
      opCode = ((uint16_t)(opCode & 0x7F) << 8) | *(currentStep + 1);
    }
    if(temporaryInformation == TI_TRUE || temporaryInformation == TI_FALSE || temporaryInformation == TI_SOLVER_FAILED || (opCode != ITM_RTN && opCode != ITM_STOP && opCode != ITM_END && opCode != 0x7fff)) {
      temporaryInformation = TI_NO_INFO;
    }
    stepsToBeAdvanced = executeOneStep(currentStep);
    if(lastErrorCode == ERROR_NONE) {
      switch(stepsToBeAdvanced) {
        case -1: { // Already the pointer is set
          break;
        }

        case 0: { // End of the routine
          if(subLevel == startingSubLevel) {
            goto stopProgram;
          }
          break;
        }

        default: { // Find the next step
          fnSkip((uint16_t)(stepsToBeAdvanced - 1));
          break;
        }
      }
    }
    else {
      break;
    }
    #if defined(DMCP_BUILD)
      if(!nestedEngine) {
        int key = key_pop();
        key = convertKeyCode(key);
        if(key == 36 || key == 33) {  //JM
          programRunStop = PGM_WAITING;
          refreshScreen();
          lcd_refresh();
          fnTimerStart(TO_KB_ACTV, TO_KB_ACTV, PROGRAM_KB_ACTV);
          wait_for_key_release(0);
          key_pop();
          break;
        }
        else if(key > 0) {
          setLastKeyCode(key);
        }
      }
    #endif // DMCP_BUILD
    if(programRunStop != PGM_RUNNING) {
      break;
    }
    if(singleStep) {
      break;
    }
      #if defined(PC_BUILD)
      refreshLcd(NULL);
    #endif // PC_BUILD
  }

stopProgram:
  if(programRunStop == PGM_RUNNING && !nestedEngine) {
    programRunStop = PGM_STOPPED;
  }
  if(programRunStop != PGM_RUNNING) {
    entryStatus &= 0xfe;
  }
  if(!getSystemFlag(FLAG_INTING) && !getSystemFlag(FLAG_SOLVING)) {
    showHideHourGlass();
      #if defined(DMCP_BUILD)
      lcd_refresh();
      fnTimerStart(TO_KB_ACTV, TO_KB_ACTV, PROGRAM_STOP);
    #else // !DMCP_BUILD
      refreshLcd(NULL);
    #endif // DMCP_BUILD
  }
  return;
  #endif // !TESTSUITE_BUILD
}



void execProgram(uint16_t label) {
  uint16_t origLocalStepNumber = currentLocalStepNumber;
  uint8_t *origStep = currentStep;
  fnExecute(label);
  if(programRunStop == PGM_RUNNING && (getSystemFlag(FLAG_INTING) || getSystemFlag(FLAG_SOLVING))) {
    runProgram(false, INVALID_VARIABLE);
    currentLocalStepNumber = origLocalStepNumber;
    currentStep = origStep;
  }
}



void fnCheckLabel(uint16_t label) {
  if(dynamicMenuItem >= 0) {
    label = findNamedLabel(dynmenuGetLabel(dynamicMenuItem));
  }

  // Local Label 00 to 99 and A, B, C, D, and E
  if(label <= 104) {
    // Search for local label
    for(uint16_t lbl=0; lbl<numberOfLabels; lbl++) {
      if(labelList[lbl].program == currentProgramNumber && labelList[lbl].step < 0 && *(labelList[lbl].labelPointer) == label) { // Is in the current program and is a local label and is the searched label
        temporaryInformation = TI_TRUE;
        return;
      }
    }
    temporaryInformation = TI_FALSE;
  }
  else if(label >= FIRST_LABEL && label <= LAST_LABEL) { // Global named label
    if((label - FIRST_LABEL) < numberOfLabels) {
      temporaryInformation = TI_TRUE;
      return;
    }
    temporaryInformation = TI_FALSE;
  }
  else {
    temporaryInformation = TI_FALSE;
  }
}



void fnIsTopRoutine(uint16_t unusedButMandatoryParameter) {
  /* A subroutine is running */
  if(currentSubroutineLevel > 0) {
    temporaryInformation = TI_FALSE;
  }

  /* Not in a subroutine */
  else {
    temporaryInformation = TI_TRUE;
  }
}
