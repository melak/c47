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
 * \file stack.c Stack managenent
 ***********************************************/

#include "wp43s.h"

/********************************************//**
 * \brief Clears X and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnClX(uint16_t unusedButMandatoryParameter) {
  clearRegister(REGISTER_X);
}



/********************************************//**
 * \brief Clears the stack and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 *
 ***********************************************/
void fnClearStack(uint16_t unusedButMandatoryParameter) {
  for(calcRegister_t regist=REGISTER_X; regist<=getStackTop(); regist++) {
    clearRegister(regist);
  }
}



/********************************************//**
 * \brief Drops X from the stack and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnDrop(uint16_t unusedButMandatoryParameter) {
  freeRegisterData(REGISTER_X);
  for(calcRegister_t regist=REGISTER_X; regist<getStackTop(); regist++) {
    globalRegister[regist] = globalRegister[regist + 1];
  }

  uint16_t sizeInBlocks = getRegisterFullSize(getStackTop());
  setRegisterDataPointer(getStackTop() - 1, allocWp43s(sizeInBlocks));
  xcopy(REGISTER_DATA(getStackTop() - 1), REGISTER_DATA(getStackTop()), TO_BYTES(sizeInBlocks));
}



/********************************************//**
 * \brief Lifts the stack if allowed and reallocates
 * the X register
 *
 * \param[in] dataType uint32_t Data type of the new X register
 * \param[in] numBytes uint32_t Number of bytes allocated to the new X register
 * \return void
 ***********************************************/
void liftStack(void) {
  if(getSystemFlag(FLAG_ASLIFT)) {
    freeRegisterData(getStackTop());
    for(uint16_t i=getStackTop(); i>REGISTER_X; i--) {
      globalRegister[i] = globalRegister[i-1];
    }
  }
  else {
    freeRegisterData(REGISTER_X);
  }

  setRegisterDataPointer(REGISTER_X, allocWp43s(REAL34_SIZE));
  setRegisterDataType(REGISTER_X, dtReal34, AM_NONE);
}



/********************************************//**
 * \brief Drops Y from the stack and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnDropY(uint16_t unusedButMandatoryParameter) {
  freeRegisterData(REGISTER_Y);
  for(uint16_t i=REGISTER_Y; i<getStackTop(); i++) {
    globalRegister[i] = globalRegister[i+1];
  }

  uint16_t sizeInBlocks = getRegisterFullSize(getStackTop());
  setRegisterDataPointer(getStackTop() - 1, allocWp43s(sizeInBlocks));
  xcopy(REGISTER_DATA(getStackTop() - 1), REGISTER_DATA(getStackTop()), TO_BYTES(sizeInBlocks));
}



/********************************************//**
 * \brief Rolls the stack up and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnRollUp(uint16_t unusedButMandatoryParameter) {
  registerHeader_t savedRegisterHeader = globalRegister[getStackTop()];

  for(uint16_t i=getStackTop(); i>REGISTER_X; i--) {
    globalRegister[i] = globalRegister[i-1];
  }
  globalRegister[REGISTER_X] = savedRegisterHeader;
}



/********************************************//**
 * \brief Rolls the stack down and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnRollDown(uint16_t unusedButMandatoryParameter) {
  registerHeader_t savedRegisterHeader = globalRegister[REGISTER_X];

  for(uint16_t i=REGISTER_X; i<getStackTop(); i++) {
    globalRegister[i] = globalRegister[i+1];
  }
  globalRegister[getStackTop()] = savedRegisterHeader;
}



/********************************************//**
 * \brief Sets the number of stack registers displayed
 *
 * \param[in] numberOfStackLines uint16_t
 * \return void
 ***********************************************/
void fnDisplayStack(uint16_t numberOfStackLines) {
  displayStack = numberOfStackLines;
}



/********************************************//**
 * \brief Swaps X with the target register
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnSwapX(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + currentNumberOfLocalRegisters) {
    copySourceRegisterToDestRegister(REGISTER_X, TEMP_REGISTER_1);
    copySourceRegisterToDestRegister(regist, REGISTER_X);
    copySourceRegisterToDestRegister(TEMP_REGISTER_1, regist);
  }

  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function fnSwapX:", errorMessage, "is not defined!", NULL);
    }
  #endif // PC_BUILD
}



/********************************************//**
 * \brief Swaps Y with the target register
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnSwapY(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + currentNumberOfLocalRegisters) {
    copySourceRegisterToDestRegister(REGISTER_Y, TEMP_REGISTER_1);
    copySourceRegisterToDestRegister(regist, REGISTER_Y);
    copySourceRegisterToDestRegister(TEMP_REGISTER_1, regist);
  }

  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function fnSwapY:", errorMessage, "is not defined!", NULL);
    }
  #endif // PC_BUILD
}


/********************************************//**
 * \brief Swaps Z with the target register
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnSwapZ(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + currentNumberOfLocalRegisters) {
    copySourceRegisterToDestRegister(REGISTER_Z, TEMP_REGISTER_1);
    copySourceRegisterToDestRegister(regist, REGISTER_Z);
    copySourceRegisterToDestRegister(TEMP_REGISTER_1, regist);
  }

  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function fnSwapZ:", errorMessage, "is not defined!", NULL);
    }
  #endif // PC_BUILD
}


/********************************************//**
 * \brief Swaps T with the target register
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnSwapT(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + currentNumberOfLocalRegisters) {
    copySourceRegisterToDestRegister(REGISTER_T, TEMP_REGISTER_1);
    copySourceRegisterToDestRegister(regist, REGISTER_T);
    copySourceRegisterToDestRegister(TEMP_REGISTER_1, regist);
  }

  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function fnSwapT:", errorMessage, "is not defined!", NULL);
    }
  #endif // PC_BUILD
}


/********************************************//**
 * \brief Swaps X and Y and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnSwapXY(uint16_t unusedButMandatoryParameter) {
  registerHeader_t savedRegisterHeader = globalRegister[REGISTER_X];

  globalRegister[REGISTER_X] = globalRegister[REGISTER_Y];
  globalRegister[REGISTER_Y] = savedRegisterHeader;
}

/********************************************//**
 * \brief Shuffles the registers and and refreshes the stack.
 * the Shuffle order is in the last four chars of the tamBuffer
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnShuffle(uint16_t unusedButMandatoryParameter) {
  for(int i=0; i<4; i++) {
    if(tamBuffer[strlen(tamBuffer) - 4 + i] == 'x') {
      copySourceRegisterToDestRegister(SAVED_REGISTER_X, REGISTER_X + i);
    }
    else if(tamBuffer[strlen(tamBuffer) - 4  + i] == 'y') {
      copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_X + i);
    }
    else if(tamBuffer[strlen(tamBuffer) - 4 + i] == 'z') {
      copySourceRegisterToDestRegister(SAVED_REGISTER_Z, REGISTER_X + i);
    }
    else if(tamBuffer[strlen(tamBuffer) - 4 + i] == 't') {
      copySourceRegisterToDestRegister(SAVED_REGISTER_T, REGISTER_X + i);
    }
  }
}



/********************************************//**
 * \brief Fills the stack with the value of X and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnFillStack(uint16_t unusedButMandatoryParameter) {
  uint16_t dataTypeX         = getRegisterDataType(REGISTER_X);
  uint16_t dataSizeXinBlocks = getRegisterFullSize(REGISTER_X);
  uint16_t tag               = getRegisterTag(REGISTER_X);

  for(uint16_t i=REGISTER_Y; i<=getStackTop(); i++) {
    freeRegisterData(i);
    setRegisterDataType(i, dataTypeX, tag);
    void *newDataPointer = allocWp43s(dataSizeXinBlocks);
    setRegisterDataPointer(i, newDataPointer);
    xcopy(newDataPointer, REGISTER_DATA(REGISTER_X), TO_BYTES(dataSizeXinBlocks));
  }
}



/********************************************//**
 * \brief Sets X to the stack size and refreshes the stack
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnGetStackSize(uint16_t unusedButMandatoryParameter) {
  longInteger_t stack;

  liftStack();

  longIntegerInit(stack);
  uIntToLongInteger(getSystemFlag(FLAG_SSIZE8) ? 8 : 4, stack);
  convertLongIntegerToLongIntegerRegister(stack, REGISTER_X);
  longIntegerFree(stack);
}



void saveForUndo(void) {
  savedSystemFlags = systemFlags;

  for(calcRegister_t regist=getStackTop(); regist>=REGISTER_X; regist--) {
    copySourceRegisterToDestRegister(regist, SAVED_REGISTER_X - REGISTER_X + regist);
  }

  copySourceRegisterToDestRegister(REGISTER_L, SAVED_REGISTER_L);

  if(statisticalSumsPointer == NULL) { // There are no statistical sums to save for undo
    if(savedStatisticalSumsPointer != NULL) {
      freeWp43s(savedStatisticalSumsPointer, NUMBER_OF_STATISTICAL_SUMS * REAL_SIZE);
      savedStatisticalSumsPointer = NULL;
    }
  }
  else { // There are statistical sums to save for undo
    if(savedStatisticalSumsPointer == NULL) {
      savedStatisticalSumsPointer = allocWp43s(NUMBER_OF_STATISTICAL_SUMS * REAL_SIZE);
    }
    xcopy(savedStatisticalSumsPointer, statisticalSumsPointer, NUMBER_OF_STATISTICAL_SUMS * TO_BYTES(REAL_SIZE));
  }

  thereIsSomethingToUndo = true;
}



void fnUndo(uint16_t unusedButMandatoryParameter) {
  if(thereIsSomethingToUndo) {
    undo();
  }
}



void undo(void) {
  systemFlags = savedSystemFlags;
  synchronizeLetteredFlags();

  for(calcRegister_t regist=getStackTop(); regist>=REGISTER_X; regist--) {
    copySourceRegisterToDestRegister(SAVED_REGISTER_X - REGISTER_X + regist, regist);
  }

  copySourceRegisterToDestRegister(SAVED_REGISTER_L, REGISTER_L);

  if(savedStatisticalSumsPointer == NULL) { // There are no statistical sums to restore
    if(statisticalSumsPointer != NULL) {
      freeWp43s(statisticalSumsPointer, NUMBER_OF_STATISTICAL_SUMS * REAL_SIZE);
      statisticalSumsPointer = NULL;
    }
  }
  else { // There are statistical sums to restore
    if(statisticalSumsPointer == NULL) {
      statisticalSumsPointer = allocWp43s(NUMBER_OF_STATISTICAL_SUMS * REAL_SIZE);
    }
    xcopy(statisticalSumsPointer, savedStatisticalSumsPointer, NUMBER_OF_STATISTICAL_SUMS * TO_BYTES(REAL_SIZE));
  }

  thereIsSomethingToUndo = false;
}
