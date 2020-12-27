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
 * \file store.c
 ***********************************************/

#include "wp43s.h"



/********************************************//**
 * \brief Stores X in an other register
 *
 * \param[in] registerNumber uint16_t
 * \return void
 ***********************************************/
void fnStore(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + currentSubroutineLevelData[1].numberOfLocalRegisters) {
    copySourceRegisterToDestRegister(REGISTER_X, regist);
  }
  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function fnStore:", errorMessage, "is not defined!", NULL);
    }
  #endif // PC_BUILD
}



/********************************************//**
 * \brief Adds X to a register
 *
 * \param[in] registerNumber uint16_t
 * \return void
 ***********************************************/
void fnStoreAdd(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + currentSubroutineLevelData[1].numberOfLocalRegisters) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(regist, REGISTER_Y);
    if(getRegisterDataType(REGISTER_Y) == dtShortInteger) {
      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)) &= shortIntegerMask;
    }

    addition[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);
    copySourceRegisterToDestRegister(REGISTER_X, regist);
    copySourceRegisterToDestRegister(SAVED_REGISTER_X, REGISTER_X);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
  }
  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function fnStorePlus:", errorMessage, "is not defined!", NULL);
    }
  #endif // PC_BUILD
}



/********************************************//**
 * \brief Subtracts X from a register
 *
 * \param[in] registerNumber calcRegister_t
 * \return void
 ***********************************************/
void fnStoreSub(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + currentSubroutineLevelData[1].numberOfLocalRegisters) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(regist, REGISTER_Y);
    if(getRegisterDataType(REGISTER_Y) == dtShortInteger) {
      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)) &= shortIntegerMask;
    }

    subtraction[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);
    copySourceRegisterToDestRegister(REGISTER_X, regist);
    copySourceRegisterToDestRegister(SAVED_REGISTER_X, REGISTER_X);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
  }
  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function fnStoreSub:", errorMessage, "is not defined!", NULL);
    }
  #endif // PC_BUILD
}



/********************************************//**
 * \brief Multiplies a register by X
 *
 * \param[in] registerNumber uint16_t
 * \return void
 ***********************************************/
void fnStoreMult(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + currentSubroutineLevelData[1].numberOfLocalRegisters) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(regist, REGISTER_Y);
    if(getRegisterDataType(REGISTER_Y) == dtShortInteger) {
      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)) &= shortIntegerMask;
    }

    multiplication[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);
    copySourceRegisterToDestRegister(REGISTER_X, regist);
    copySourceRegisterToDestRegister(SAVED_REGISTER_X, REGISTER_X);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
  }
  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function fnStoreMult:", errorMessage, "is not defined!", NULL);
    }
  #endif // PC_BUILD
}



/********************************************//**
 * \brief Divides a register by X
 *
 * \param[in] registerNumber uint16_t
 * \return void
 ***********************************************/
void fnStoreDiv(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + currentSubroutineLevelData[1].numberOfLocalRegisters) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(regist, REGISTER_Y);
    if(getRegisterDataType(REGISTER_Y) == dtShortInteger) {
      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_Y)) &= shortIntegerMask;
    }

    division[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);
    copySourceRegisterToDestRegister(REGISTER_X, regist);
    copySourceRegisterToDestRegister(SAVED_REGISTER_X, REGISTER_X);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
  }
  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function fnStoreDiv:", errorMessage, "is not defined!", NULL);
    }
  #endif // PC_BUILD
}



/********************************************//**
 * \brief Keeps in a register min(X, register)
 *
 * \param[in] registerNumber uint16_t
 * \return void
 ***********************************************/
void fnStoreMin(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + currentSubroutineLevelData[1].numberOfLocalRegisters) {
    registerMin(REGISTER_X, regist, regist);
  }
  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function fnStoreMin:", errorMessage, "is not defined!", NULL);
    }
  #endif // PC_BUILD
}



/********************************************//**
 * \brief Keeps in a register max(X, register)
 *
 * \param[in] registerNumber uint16_t
 * \return void
 ***********************************************/
void fnStoreMax(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + currentSubroutineLevelData[1].numberOfLocalRegisters) {
    registerMax(REGISTER_X, regist, regist);
  }
  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function fnStoreMax:", errorMessage, "is not defined!", NULL);
    }
  #endif // PC_BUILD
}



/********************************************//**
 * \brief Stores the configuration
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnStoreConfig(uint16_t regist) {
  reallocateRegister(regist, dtConfig, CONFIG_SIZE, AM_NONE);
  dtConfigDescriptor_t *configToStore = REGISTER_CONFIG_DATA(regist);

  storeToDtConfigDescriptor(shortIntegerMode);
  storeToDtConfigDescriptor(shortIntegerWordSize);
  storeToDtConfigDescriptor(displayFormat);
  storeToDtConfigDescriptor(displayFormatDigits);
  storeToDtConfigDescriptor(groupingGap);
  storeToDtConfigDescriptor(currentAngularMode);
  storeToDtConfigDescriptor(denMax);
  storeToDtConfigDescriptor(displayStack);
  storeToDtConfigDescriptor(firstGregorianDay);
  storeToDtConfigDescriptor(roundingMode);
  storeToDtConfigDescriptor(systemFlags);
  xcopy(configToStore->kbd_usr, kbd_usr, sizeof(kbd_usr));
}



/********************************************//**
 * \brief Stores the stack
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnStoreStack(uint16_t regist) {
  uint16_t size = getSystemFlag(FLAG_SSIZE8) ? 8 : 4;

  if(regist + size >= REGISTER_X) {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Cannot execute STOS, destination register is out of range: %d", regist);
      moreInfoOnError("In function fnStoreStack:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else {
    for(int i=0; i<size; i++) {
      copySourceRegisterToDestRegister(REGISTER_X + i, regist + i);
    }
  }
}



/********************************************//**
 * \brief Stores X in the element I,J of a matrix
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnStoreElement(uint16_t unusedButMandatoryParameter) {
  #ifdef PC_BUILD
    printf("fnStoreElement\n");
  #endif // PC_BUILD

  displayCalcErrorMessage(ERROR_ITEM_TO_BE_CODED, ERR_REGISTER_LINE, REGISTER_X);
  #ifdef PC_BUILD
    moreInfoOnError("In function fnStoreElement:", "To be coded", NULL, NULL);
  #endif // PC_BUILD
}



/********************************************//**
 * \brief Stores X and Y in the indexes I and J
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnStoreIJ(uint16_t unusedButMandatoryParameter) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_I);
  copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_J);
}
