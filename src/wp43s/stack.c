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
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnClX(uint16_t unusedParamButMandatory) {
  saveStack();               //JM
  clearRegister(REGISTER_X);
  refreshRegisterLine(REGISTER_X);
}



/********************************************//**
 * \brief Clears the stack and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 *
 ***********************************************/
void fnClearStack(uint16_t unusedParamButMandatory) {
  saveStack();               //JM
  for(calcRegister_t regist=REGISTER_X; regist<=getStackTop(); regist++) {
    clearRegister(regist);
  }

  refreshStack();
}



/********************************************//**
 * \brief Drops X from the stack and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnDrop(uint16_t unusedParamButMandatory) {
  uint16_t sizeInBytes;
  saveStack();               //JM
  freeRegisterData(REGISTER_X);
  for(calcRegister_t regist=REGISTER_X; regist<getStackTop(); regist++) {
    reg[regist] = reg[regist + 1];
  }

  sizeInBytes = TO_BYTES(getRegisterFullSize(getStackTop()));
  setRegisterDataPointer(getStackTop() - 1, allocWp43s(sizeInBytes));
  xcopy(REGISTER_DATA(getStackTop()-1), REGISTER_DATA(getStackTop()), sizeInBytes);
  refreshStack();
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
      reg[i] = reg[i-1];
    }
  }
  else {
    freeRegisterData(REGISTER_X);
  }

  setRegisterDataPointer(REGISTER_X, allocWp43s(TO_BYTES(REAL34_SIZE)));
  setRegisterDataType(REGISTER_X, dtReal34, AM_NONE);
}



/********************************************//**
 * \brief Drops Y from the stack and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnDropY(uint16_t unusedParamButMandatory) {
  uint16_t sizeInBytes;

  freeRegisterData(REGISTER_Y);
  for(uint16_t i=REGISTER_Y; i<getStackTop(); i++) {
    reg[i] = reg[i+1];
  }

  sizeInBytes = TO_BYTES(getRegisterFullSize(getStackTop()));
  setRegisterDataPointer(getStackTop() - 1, allocWp43s(sizeInBytes));
  xcopy(REGISTER_DATA(getStackTop() - 1), REGISTER_DATA(getStackTop()), sizeInBytes);
  refreshStack();
}



/********************************************//**
 * \brief Rolls the stack up and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnRollUp(uint16_t unusedParamButMandatory) {
  registerDescriptor_t savedRegister = reg[getStackTop()];

  for(uint16_t i=getStackTop(); i>REGISTER_X; i--) {
    reg[i] = reg[i-1];
  }
  reg[REGISTER_X] = savedRegister;
  refreshStack();
}



/********************************************//**
 * \brief Rolls the stack down and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnRollDown(uint16_t unusedParamButMandatory) {
  registerDescriptor_t savedRegister = reg[REGISTER_X];

  for(uint16_t i=REGISTER_X; i<getStackTop(); i++) {
    reg[i] = reg[i+1];
  }
  reg[getStackTop()] = savedRegister;
  refreshStack();
}



/********************************************//**
 * \brief Sets the number of stack registers displayed
 *
 * \param[in] numberOfStackLines uint16_t
 * \return void
 ***********************************************/
void fnDisplayStack(uint16_t numberOfStackLines) {
  displayStack = numberOfStackLines;
  refreshStack();
}



/********************************************//**
 * \brief Swaps X and Y and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSwapXY(uint16_t unusedParamButMandatory) {
  registerDescriptor_t savedRegister = reg[REGISTER_X];

  reg[REGISTER_X] = reg[REGISTER_Y];
  reg[REGISTER_Y] = savedRegister;
  refreshRegisterLine(REGISTER_X);
  refreshRegisterLine(REGISTER_Y);
}



/********************************************//**
 * \brief Fills the stack with the value of X and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnFillStack(uint16_t unusedParamButMandatory) {
  uint16_t dataTypeX = getRegisterDataType(REGISTER_X);
  uint16_t dataSizeXinBytes = TO_BYTES(getRegisterFullSize(REGISTER_X));
  uint16_t tag       = getRegisterTag(REGISTER_X);
  void *newDataPointer;

  for(uint16_t i=REGISTER_Y; i<=getStackTop(); i++) {
    freeRegisterData(i);
    setRegisterDataType(i, dataTypeX, tag);
    newDataPointer = allocWp43s(dataSizeXinBytes);
    setRegisterDataPointer(i, newDataPointer);
    xcopy(newDataPointer, REGISTER_DATA(REGISTER_X), dataSizeXinBytes);
  }

  refreshStack();
}



/********************************************//**
 * \brief Sets X to the stack size and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGetStackSize(uint16_t unusedParamButMandatory) {
  longInteger_t stack;

  liftStack();

  longIntegerInit(stack);
  uIntToLongInteger(getSystemFlag(FLAG_SSIZE8) ? 8 : 4, stack);
  convertLongIntegerToLongIntegerRegister(stack, REGISTER_X);
  longIntegerFree(stack);

  refreshStack();
}



/********************************************//**
 * \brief Refreshes the screen
 *
 * \param void
 * \return void
 ***********************************************/
void refreshStack(void) {
  refreshRegisterLine(REGISTER_X);
  refreshRegisterLine(REGISTER_Y);
  refreshRegisterLine(REGISTER_Z);
  refreshRegisterLine(REGISTER_T);
}



void saveStack(void) {
  savedStackLiftEnabled = getSystemFlag(FLAG_ASLIFT);
  for(calcRegister_t regist=getStackTop(); regist>=REGISTER_X; regist--) {
    //printf("save %" FMT16S " to %" FMT16S " : ", regist, SAVED_REGISTER_X - REGISTER_X + regist); printRegisterToConsole(regist, 0); printf("\n");
    copySourceRegisterToDestRegister(regist, SAVED_REGISTER_X - REGISTER_X + regist);
  }
  //printf("save %" FMT16S " to %" FMT16S " : ", REGISTER_L, SAVED_REGISTER_L); printRegisterToConsole(REGISTER_L, 0); printf("\n");
  copySourceRegisterToDestRegister(REGISTER_L, SAVED_REGISTER_L);
  //printf("\n");
  //refreshDebugPanel();
}



void restoreStack(void) {
  if(savedStackLiftEnabled) {
    setSystemFlag(FLAG_ASLIFT);
  }
  else {
    clearSystemFlag(FLAG_ASLIFT);
  }

  for(calcRegister_t regist=getStackTop(); regist>=REGISTER_X; regist--) {
    copySourceRegisterToDestRegister(SAVED_REGISTER_X - REGISTER_X + regist, regist);
    //printf("restore %" FMT16S " to %" FMT16S " : ", SAVED_REGISTER_X - REGISTER_X + regist, regist); printRegisterToConsole(regist, 0); printf("\n");
  }
  copySourceRegisterToDestRegister(SAVED_REGISTER_L, REGISTER_L);
  //printf("restore %" FMT16S " to %" FMT16S " : ", SAVED_REGISTER_L, REGISTER_L); printRegisterToConsole(REGISTER_L, 0); printf("\n");
  //printf("\n");
  refreshStack();
}



#ifdef PC_BUILD
void stackLiftEnable(void) {
  //printf("Stack lift enabled\n");
  clearSystemFlag(FLAG_LOWBAT);
  setSystemFlag(FLAG_ASLIFT);

  // Draw S
  setPixel(392,  1);
  setPixel(393,  1);
  setPixel(394,  1);
  setPixel(391,  2);
  setPixel(395,  2);
  setPixel(391,  3);
  setPixel(392,  4);
  setPixel(393,  4);
  setPixel(394,  4);
  setPixel(395,  5);
  setPixel(391,  6);
  setPixel(395,  6);
  setPixel(392,  7);
  setPixel(393,  7);
  setPixel(394,  7);

  // Draw L
  setPixel(391, 10);
  setPixel(391, 11);
  setPixel(391, 12);
  setPixel(391, 13);
  setPixel(391, 14);
  setPixel(391, 15);
  setPixel(391, 16);
  setPixel(392, 16);
  setPixel(393, 16);
  setPixel(394, 16);
  setPixel(395, 16);
}



void stackLiftDisable(void) {
  //printf("Stack lift disabled\n");
  clearSystemFlag(FLAG_LOWBAT);
  clearSystemFlag(FLAG_ASLIFT);
}
#endif
