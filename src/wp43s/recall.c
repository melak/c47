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
 * \file recall.c
 ***********************************************/

#include "wp43s.h"



/********************************************//**
 * \brief Recalls a register in X
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecall(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + currentNumberOfLocalRegisters) {
    if(REGISTER_X <= regist && regist <= getStackTop()) {
      copySourceRegisterToDestRegister(regist, TEMP_REGISTER);
      liftStack();
      copySourceRegisterToDestRegister(TEMP_REGISTER, REGISTER_X);
    }
    else {
      liftStack();
      copySourceRegisterToDestRegister(regist, REGISTER_X);
      if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
        *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) &= shortIntegerMask;
      }
    }
  }
  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function fnRecall:", errorMessage, "is not defined!", NULL);
    }
  #endif // PC_BUILD
}



/********************************************//**
 * \brief Recalls register L in X
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnLastX(uint16_t unusedButMandatoryParameter) {
  fnRecall(REGISTER_L);
}



/********************************************//**
 * \brief Adds a register to X
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallAdd(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + currentNumberOfLocalRegisters) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_Y);
    copySourceRegisterToDestRegister(regist, REGISTER_X);
    if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) &= shortIntegerMask;
    }

    addition[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
  }
  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function fnRecallPlus:", errorMessage, "is not defined!", NULL);
    }
  #endif // PC_BUILD
}



/********************************************//**
 * \brief Subtracts a register from X
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallSub(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + currentNumberOfLocalRegisters) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_Y);
    copySourceRegisterToDestRegister(regist, REGISTER_X);
    if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) &= shortIntegerMask;
    }

    subtraction[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
  }
  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function fnRecallSub:", errorMessage, "is not defined!", NULL);
    }
  #endif // PC_BUILD
}



/********************************************//**
 * \brief Multiplies X by a register
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallMult(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + currentNumberOfLocalRegisters) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_Y);
    copySourceRegisterToDestRegister(regist, REGISTER_X);
    if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) &= shortIntegerMask;
    }

    multiplication[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
  }
  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function fnRecallMult:", errorMessage, "is not defined!", NULL);
    }
  #endif // PC_BUILD
}



/********************************************//**
 * \brief Divides X by a register
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallDiv(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + currentNumberOfLocalRegisters) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_Y);
    copySourceRegisterToDestRegister(regist, REGISTER_X);
    if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) &= shortIntegerMask;
    }

    division[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
  }
  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function fnRecallDiv:", errorMessage, "is not defined!", NULL);
    }
  #endif // PC_BUILD
}



/********************************************//**
 * \brief Keeps in X min(X, register)
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallMin(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + currentNumberOfLocalRegisters) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    registerMin(REGISTER_X, regist, REGISTER_X);
  }
  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function fnRecallMin:", errorMessage, "is not defined!", NULL);
    }
  #endif // PC_BUILD
}



/********************************************//**
 * \brief Keeps in X max(X, register)
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallMax(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + currentNumberOfLocalRegisters) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    registerMax(REGISTER_X, regist, REGISTER_X);
  }
  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function fnReallMax:", errorMessage, "is not defined!", NULL);
    }
  #endif // PC_BUILD
}



/********************************************//**
 * \brief Recalls a configuration
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallConfig(uint16_t regist) {
  if(getRegisterDataType(regist) == dtConfig) {
    dtConfigDescriptor_t *configToRecall = REGISTER_CONFIG_DATA(regist);

    xcopy(kbd_usr, configToRecall->kbd_usr, sizeof(kbd_usr));
    recallFromDtConfigDescriptor(shortIntegerMode);
    recallFromDtConfigDescriptor(shortIntegerWordSize);
    recallFromDtConfigDescriptor(displayFormat);
    recallFromDtConfigDescriptor(displayFormatDigits);
    recallFromDtConfigDescriptor(groupingGap);
    recallFromDtConfigDescriptor(currentAngularMode);
    recallFromDtConfigDescriptor(denMax);
    recallFromDtConfigDescriptor(displayStack);
    recallFromDtConfigDescriptor(firstGregorianDay);
    recallFromDtConfigDescriptor(roundingMode);
    recallFromDtConfigDescriptor(systemFlags);
    synchronizeLetteredFlags();
  }

  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "data type %s cannot be used to recall a configuration!", getRegisterDataTypeName(regist, false, false));
      moreInfoOnError("In function fnRecallConfig:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}



/********************************************//**
 * \brief Recalls a stack
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallStack(uint16_t regist) {
  uint16_t size = getSystemFlag(FLAG_SSIZE8) ? 8 : 4;

  if(regist + size >= REGISTER_X) {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Cannot execute RCLS, destination register is out of range: %d", regist);
      moreInfoOnError("In function fnRecallStack:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else {
    int i;

    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    for(i=0; i<size; i++) {
      copySourceRegisterToDestRegister(regist + i, REGISTER_X + i);
    }

    for(i=0; i<4; i++) {
      adjustResult(REGISTER_X + i, false, true, REGISTER_X + i, -1, -1);
    }
  }
}



/********************************************//**
 * \brief Recalls the matrix element I,J in X
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallElement(uint16_t unusedButMandatoryParameter) {
  #ifdef PC_BUILD
    printf("fnRecallElement\n");
  #endif // PC_BUILD

  displayCalcErrorMessage(ERROR_ITEM_TO_BE_CODED, ERR_REGISTER_LINE, REGISTER_X);
  #ifdef PC_BUILD
    moreInfoOnError("In function fnRecallElement:", "To be coded", NULL, NULL);
  #endif // PC_BUILD
}



/********************************************//**
 * \brief Recalls the indexes I and J in X and Y
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallIJ(uint16_t unusedButMandatoryParameter) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  copySourceRegisterToDestRegister(REGISTER_I, REGISTER_X);
  copySourceRegisterToDestRegister(REGISTER_J, REGISTER_Y);

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
  adjustResult(REGISTER_Y, false, true, REGISTER_Y, -1, -1);
}
