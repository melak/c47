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

#include "stack.h"

#include "charString.h"
#include "constantPointers.h"
#include "mathematics/comparisonReals.h"
#include "error.h"
#include "flags.h"
#include "mathematics/matrix.h"
#include "memory.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "stats.h"

#include "wp43.h"

void fnClX(uint16_t unusedButMandatoryParameter) {
  clearRegister(REGISTER_X);
}



void fnClearStack(uint16_t unusedButMandatoryParameter) {
  for(calcRegister_t regist=REGISTER_X; regist<=getStackTop(); regist++) {
    clearRegister(regist);
  }
}



void fnDrop(uint16_t unusedButMandatoryParameter) {
  freeRegisterData(REGISTER_X);
  for(calcRegister_t regist=REGISTER_X; regist<getStackTop(); regist++) {
    globalRegister[regist] = globalRegister[regist + 1];
  }

  uint16_t sizeInBlocks = getRegisterFullSize(getStackTop());
  setRegisterDataPointer(getStackTop() - 1, allocWp43(sizeInBlocks));
  xcopy(REGISTER_DATA(getStackTop() - 1), REGISTER_DATA(getStackTop()), TO_BYTES(sizeInBlocks));
}



void liftStack(void) {
  if(getSystemFlag(FLAG_ASLIFT)) {
    if(currentInputVariable != INVALID_VARIABLE) {
      currentInputVariable |= 0x8000;
    }
    freeRegisterData(getStackTop());
    for(uint16_t i=getStackTop(); i>REGISTER_X; i--) {
      globalRegister[i] = globalRegister[i-1];
    }
  }
  else {
    freeRegisterData(REGISTER_X);
  }

  setRegisterDataPointer(REGISTER_X, allocWp43(REAL34_SIZE));
  setRegisterDataType(REGISTER_X, dtReal34, amNone);
}



void fnDropY(uint16_t unusedButMandatoryParameter) {
  freeRegisterData(REGISTER_Y);
  for(uint16_t i=REGISTER_Y; i<getStackTop(); i++) {
    globalRegister[i] = globalRegister[i+1];
  }

  uint16_t sizeInBlocks = getRegisterFullSize(getStackTop());
  void *dataPtr = allocWp43(sizeInBlocks);
  if(dataPtr) {
    setRegisterDataPointer(getStackTop() - 1, dataPtr);
    xcopy(REGISTER_DATA(getStackTop() - 1), REGISTER_DATA(getStackTop()), TO_BYTES(sizeInBlocks));
  }
  else {
    lastErrorCode = ERROR_RAM_FULL;
  }
}



void fnRollUp(uint16_t unusedButMandatoryParameter) {
  registerHeader_t savedRegisterHeader = globalRegister[getStackTop()];

  for(uint16_t i=getStackTop(); i>REGISTER_X; i--) {
    globalRegister[i] = globalRegister[i-1];
  }
  globalRegister[REGISTER_X] = savedRegisterHeader;
}



void fnRollDown(uint16_t unusedButMandatoryParameter) {
  registerHeader_t savedRegisterHeader = globalRegister[REGISTER_X];

  for(uint16_t i=REGISTER_X; i<getStackTop(); i++) {
    globalRegister[i] = globalRegister[i+1];
  }
  globalRegister[getStackTop()] = savedRegisterHeader;
}



void fnDisplayStack(uint16_t numberOfStackLines) {
  displayStack = numberOfStackLines;
}


static void _swapRegs(uint16_t srcReg, uint16_t regist) {
  registerHeader_t savedRegisterHeader = globalRegister[srcReg];

  if(regist <= LAST_GLOBAL_REGISTER) {
    globalRegister[srcReg] = globalRegister[regist];
    globalRegister[regist] = savedRegisterHeader;
  }

  else if(regist < FIRST_LOCAL_REGISTER + currentNumberOfLocalRegisters) {
    globalRegister[srcReg] = currentLocalRegisters[regist - FIRST_LOCAL_REGISTER];
    currentLocalRegisters[regist - FIRST_LOCAL_REGISTER] = savedRegisterHeader;
  }

  #if defined(PC_BUILD)
    else if(regist <= LAST_LOCAL_REGISTER) {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function _swapRegs:", errorMessage, "is not defined!", NULL);
    }
  #endif // PC_BUILD

  else if(regist <= LAST_TEMP_REGISTER) {
    #if defined(PC_BUILD)
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      sprintf(errorMessage, "register %d", regist);
      moreInfoOnError("In function _swapRegs:", errorMessage, "is unsupported!", NULL);
    #endif // PC_BUILD
  }

  else if(regist < FIRST_NAMED_VARIABLE + numberOfNamedVariables) {
    globalRegister[srcReg] = allNamedVariables[regist - FIRST_NAMED_VARIABLE].header;
    allNamedVariables[regist - FIRST_NAMED_VARIABLE].header = savedRegisterHeader;
  }

  #if defined(PC_BUILD)
    else {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      sprintf(errorMessage, "register %d", regist);
      moreInfoOnError("In function _swapRegs:", errorMessage, "is unsupported!", NULL);
    }
  #endif // PC_BUILD
}


void fnSwapX(uint16_t regist) {
  _swapRegs(REGISTER_X, regist);
}


void fnSwapY(uint16_t regist) {
  _swapRegs(REGISTER_Y, regist);
}


void fnSwapZ(uint16_t regist) {
  _swapRegs(REGISTER_Z, regist);
}


void fnSwapT(uint16_t regist) {
  _swapRegs(REGISTER_T, regist);
}


void fnSwapXY(uint16_t unusedButMandatoryParameter) {
  registerHeader_t savedRegisterHeader = globalRegister[REGISTER_X];

  globalRegister[REGISTER_X] = globalRegister[REGISTER_Y];
  globalRegister[REGISTER_Y] = savedRegisterHeader;
}

void fnShuffle(uint16_t regist_order) {
  for(int i=0; i<4; i++) {
    registerHeader_t savedRegisterHeader = globalRegister[REGISTER_X + i];
    globalRegister[REGISTER_X + i] = savedStackRegister[i];
    savedStackRegister[i] = savedRegisterHeader;
  }
  for(int i=0; i<4; i++) {
    uint16_t regist_offset = (regist_order >> (i*2)) & 3;
    copySourceRegisterToDestRegister(SAVED_REGISTER_X + regist_offset, REGISTER_X + i);
  }
}



void fnFillStack(uint16_t unusedButMandatoryParameter) {
  uint16_t dataTypeX         = getRegisterDataType(REGISTER_X);
  uint16_t dataSizeXinBlocks = getRegisterFullSize(REGISTER_X);
  uint16_t tag               = getRegisterTag(REGISTER_X);

  for(uint16_t i=REGISTER_Y; i<=getStackTop(); i++) {
    freeRegisterData(i);
    setRegisterDataType(i, dataTypeX, tag);
    void *newDataPointer = allocWp43(dataSizeXinBlocks);
    if(newDataPointer) {
      setRegisterDataPointer(i, newDataPointer);
      xcopy(newDataPointer, REGISTER_DATA(REGISTER_X), TO_BYTES(dataSizeXinBlocks));
    }
    else {
      lastErrorCode = ERROR_RAM_FULL;
      return;
    }
  }
}



void fnGetStackSize(uint16_t unusedButMandatoryParameter) {
  longInteger_t stack;

  liftStack();

  longIntegerInit(stack);
  uIntToLongInteger(getSystemFlag(FLAG_SSIZE8) ? 8 : 4, stack);
  convertLongIntegerToLongIntegerRegister(stack, REGISTER_X);
  longIntegerFree(stack);
}



void saveForUndo(void) {
  if(((calcMode == CM_NIM || calcMode == CM_AIM || calcMode == CM_MIM) && thereIsSomethingToUndo) || calcMode == CM_NO_UNDO) {
    #if defined(DEBUGUNDO)
      if(thereIsSomethingToUndo) {
        printf(">>> saveForUndo; calcMode = %i, nothing stored as there is something to undo\n", calcMode);
      }
      if(calcMode == CM_NIM || calcMode == CM_AIM || calcMode == CM_MIM || calcMode == CM_NO_UNDO) {
        printf(">>> saveForUndo; calcMode = %i, nothing stored, wrong mode\n", calcMode);
      }
    #endif // DEBUGUNDO
    return;
  }
  #if defined(DEBUGUNDO)
    printf(">>> in saveForUndo, saving; calcMode = %i pre:thereIsSomethingToUndo = %i ;", calcMode, thereIsSomethingToUndo);
    printf("Clearing TEMP_REGISTER_2_SAVED_STATS\n\n");
  #endif // DEBUGUNDO

  clearRegister(TEMP_REGISTER_2_SAVED_STATS); //clear it here for every saveForUndo call, and explicitly set it in fnEditMatrix() and fnEqSolvGraph() only
  SAVED_SIGMA_LAct = 0;

  savedSystemFlags = systemFlags;

  if(currentInputVariable != INVALID_VARIABLE) {
    if(currentInputVariable & 0x8000) {
      currentInputVariable |= 0x4000;
    }
    else {
      currentInputVariable &= 0xbfff;
    }
  }

  if(entryStatus & 0x01) {
    entryStatus |= 0x02;
  }
  else {
    entryStatus &= 0xfd;
  }

  for(calcRegister_t regist=getStackTop(); regist>=REGISTER_X; regist--) {
    copySourceRegisterToDestRegister(regist, SAVED_REGISTER_X - REGISTER_X + regist);
    if(lastErrorCode == ERROR_RAM_FULL) {
      #if defined(PC_BUILD)
        printf("In function saveForUndo: not enough space for saving register #%" PRId16 "!\n", regist);
        fflush(stdout);
      #endif // PC_BUILD
      goto failed;
    }
  }

  copySourceRegisterToDestRegister(REGISTER_L, SAVED_REGISTER_L);
  if(lastErrorCode == ERROR_RAM_FULL) {
    #if defined(PC_BUILD)
      printf("In function saveForUndo: not enough space for saving register L!\n");
      fflush(stdout);
    #endif // PC_BUILD
    goto failed;
  }

  lrSelectionUndo = lrSelection;
  if(statisticalSumsPointer == NULL) { // There are no statistical sums to save for undo
    if(savedStatisticalSumsPointer != NULL) {
      freeWp43(savedStatisticalSumsPointer, NUMBER_OF_STATISTICAL_SUMS * REAL_SIZE);
      savedStatisticalSumsPointer = NULL;
    }
  }
  else { // There are statistical sums to save for undo
    lrChosenUndo = lrChosen;
    if(savedStatisticalSumsPointer == NULL) {
      savedStatisticalSumsPointer = allocWp43(NUMBER_OF_STATISTICAL_SUMS * REAL_SIZE);
    }
    xcopy(savedStatisticalSumsPointer, statisticalSumsPointer, NUMBER_OF_STATISTICAL_SUMS * TO_BYTES(REAL_SIZE));
  }

  thereIsSomethingToUndo = true;
  return;

failed:
  for(calcRegister_t regist=getStackTop(); regist>=REGISTER_X; regist--) {
    clearRegister(SAVED_REGISTER_X - REGISTER_X + regist);
  }
  clearRegister(SAVED_REGISTER_L);
  thereIsSomethingToUndo = false;
  lastErrorCode = ERROR_RAM_FULL;
  return;
}



void fnUndo(uint16_t unusedButMandatoryParameter) {
  if(thereIsSomethingToUndo) {
    undo();
  }
}



void undo(void) {
  #if defined(DEBUGUNDO)
    printf(">>> Undoing, calcMode = %i ...", calcMode);
  #endif // DEBUGUNDO
  recallStatsMatrix();

  if(currentInputVariable != INVALID_VARIABLE) {
    if(currentInputVariable & 0x4000) {
      currentInputVariable |= 0x8000;
    }
    else {
      currentInputVariable &= 0x7fff;
    }
  }

  if(entryStatus & 0x02) {
    entryStatus |= 0x01;
  }
  else {
    entryStatus &= 0xfe;
  }

  if(SAVED_SIGMA_LAct == +1 && statisticalSumsPointer != NULL) {
    fnSigma(-1);
  }
  else if(SAVED_SIGMA_LAct == -1) {
    clearRegister(REGISTER_X);
    clearRegister(REGISTER_Y);
    convertRealToReal34ResultRegister(&SAVED_SIGMA_LASTX, REGISTER_X);             // Can use stack, as the stack will be undone below
    convertRealToReal34ResultRegister(&SAVED_SIGMA_LASTY, REGISTER_Y);
    fnSigma(+1);
  }

  systemFlags = savedSystemFlags;
  synchronizeLetteredFlags();

  for(calcRegister_t regist=getStackTop(); regist>=REGISTER_X; regist--) {
    copySourceRegisterToDestRegister(SAVED_REGISTER_X - REGISTER_X + regist, regist);
  }

  copySourceRegisterToDestRegister(SAVED_REGISTER_L, REGISTER_L);

  lrSelection = lrSelectionUndo;
  if(savedStatisticalSumsPointer == NULL) { // There are no statistical sums to restore
    if(statisticalSumsPointer != NULL) {
      freeWp43(statisticalSumsPointer, NUMBER_OF_STATISTICAL_SUMS * REAL_SIZE);
      statisticalSumsPointer = NULL;
      lrChosen = 0;
    }
  }
  else { // There are statistical sums to restore
    lrChosen = lrChosenUndo;
    if(statisticalSumsPointer == NULL) {
      statisticalSumsPointer = allocWp43(NUMBER_OF_STATISTICAL_SUMS * REAL_SIZE);
    }
    xcopy(statisticalSumsPointer, savedStatisticalSumsPointer, NUMBER_OF_STATISTICAL_SUMS * TO_BYTES(REAL_SIZE));
  }

  SAVED_SIGMA_LAct = 0;
  thereIsSomethingToUndo = false;
  clearRegister(TEMP_REGISTER_2_SAVED_STATS);
  #if defined(DEBUGUNDO)
    printf(">>> Undone, calcMode = %i\n", calcMode);
  #endif // DEBUGUNDO

}
