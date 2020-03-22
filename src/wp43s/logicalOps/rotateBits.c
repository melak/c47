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
 * \file rotateBits.c
 ***********************************************/

#include "wp43s.h"



/********************************************//**
 * \brief regX ==> regL and ASR(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnAsr(uint16_t numberOfShifts) {
  int32_t i;
  uint64_t sign;

  if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    sign = *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) & shortIntegerSignBit;
    for(i=1; i<=numberOfShifts; i++) {
      if(i == numberOfShifts) {
        if(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) & 1) {
          fnSetFlag(FLAG_CARRY);
        }
        else {
          fnClearFlag(FLAG_CARRY);
        }
      }

      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = (*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) >> 1) | sign;
    }

    refreshRegisterLine(REGISTER_X);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot ASR %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnAsr:", errorMessage, NULL, NULL);
    #endif
  }
}



/********************************************//**
 * \brief regX ==> regL and SL(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSl(uint16_t numberOfShifts) {
  int32_t i;

  if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    for(i=1; i<=numberOfShifts; i++) {
      if(i == numberOfShifts) {
        if(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) & shortIntegerSignBit) {
          fnSetFlag(FLAG_CARRY);
        }
        else {
          fnClearFlag(FLAG_CARRY);
        }
      }

      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) <<= 1;
    }

    refreshRegisterLine(REGISTER_X);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot SL %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnSl:", errorMessage, NULL, NULL);
    #endif
  }
}



/********************************************//**
 * \brief regX ==> regL and SR(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSr(uint16_t numberOfShifts) {
  int32_t i;

  if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    for(i=1; i<=numberOfShifts; i++) {
      if(i == numberOfShifts) {
        if(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) & 1) {
          fnSetFlag(FLAG_CARRY);
        }
        else {
          fnClearFlag(FLAG_CARRY);
        }
      }

      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) >>= 1;
    }

    refreshRegisterLine(REGISTER_X);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot SR %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnSr:", errorMessage, NULL, NULL);
    #endif
  }
}



/********************************************//**
 * \brief regX ==> regL and RL(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnRl(uint16_t numberOfShifts) {
  int32_t i;
  uint64_t sign;

  if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    for(i=1; i<=numberOfShifts; i++) {
      sign = (*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) & shortIntegerSignBit) >> (shortIntegerWordSize - 1);
      if(i == numberOfShifts) {
        if(sign) {
          fnSetFlag(FLAG_CARRY);
        }
        else {
          fnClearFlag(FLAG_CARRY);
        }
      }

      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = (*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) << 1) | sign;
    }

    refreshRegisterLine(REGISTER_X);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot RL %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnRl:", errorMessage, NULL, NULL);
    #endif
  }
}



/********************************************//**
 * \brief regX ==> regL and RR(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnRr(uint16_t numberOfShifts) {
  int32_t i;

  if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    for(i=1; i<=numberOfShifts; i++) {
      if(i == numberOfShifts) {
        if(*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) & 1) {
          fnSetFlag(FLAG_CARRY);
        }
        else {
          fnClearFlag(FLAG_CARRY);
        }
      }

      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = (*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) >> 1) | ((*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) & 1) << (shortIntegerWordSize - 1));
    }

    refreshRegisterLine(REGISTER_X);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot RR %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnRr:", errorMessage, NULL, NULL);
    #endif
  }
}



/********************************************//**
 * \brief regX ==> regL and RLC(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnRlc(uint16_t numberOfShifts) {
  int32_t i;
  uint64_t sign, carry;

  if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    carry = getFlag(FLAG_CARRY);
    for(i=1; i<=numberOfShifts; i++) {
      sign = (*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) & shortIntegerSignBit) >> (shortIntegerWordSize - 1);
      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = (*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) << 1) | carry;
      carry = sign;
    }

    if(carry) {
      fnSetFlag(FLAG_CARRY);
    }
    else {
      fnClearFlag(FLAG_CARRY);
    }

    refreshRegisterLine(REGISTER_X);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot RLC %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnRlc:", errorMessage, NULL, NULL);
    #endif
  }
}



/********************************************//**
 * \brief regX ==> regL and RRC(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnRrc(uint16_t numberOfShifts) {
  int32_t i;
  uint64_t lsb, carry;

  if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    carry = getFlag(FLAG_CARRY);
    for(i=1; i<=numberOfShifts; i++) {
      lsb = *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) & 1;
      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = (*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) >> 1) | (carry << (shortIntegerWordSize - 1));
      carry = lsb;
    }

    if(carry) {
      fnSetFlag(FLAG_CARRY);
    }
    else {
      fnClearFlag(FLAG_CARRY);
    }

    refreshRegisterLine(REGISTER_X);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot RRC %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnRrc:", errorMessage, NULL, NULL);
    #endif
  }
}



/********************************************//**
 * \brief regX ==> regL and LJ(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnLj(uint16_t numberOfShifts) {
  int32_t count;
  longInteger_t regX;

  if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    count = 0;
    while((*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) & shortIntegerSignBit) == 0) {
      count++;
      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) <<= 1;
    }

    longIntegerInit(regX);
    intToLongInteger(count, regX);

    STACK_LIFT_ENABLE;
    liftStack();
    convertLongIntegerToLongIntegerRegister(regX, REGISTER_X);
    longIntegerFree(regX);

    refreshStack();
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot LJ %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnLj:", errorMessage, NULL, NULL);
    #endif
  }
}



/********************************************//**
 * \brief regX ==> regL and RJ(regX) ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnRj(uint16_t numberOfShifts) {
  int32_t count;
  longInteger_t regX;

  if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    count = 0;
    while((*(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) & 1) == 0) {
      count++;
      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) >>= 1;
    }

    longIntegerInit(regX);
    intToLongInteger(count, regX);

    STACK_LIFT_ENABLE;
    liftStack();
    convertLongIntegerToLongIntegerRegister(regX, REGISTER_X);
    longIntegerFree(regX);

    refreshStack();
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot RJ %s", getRegisterDataTypeName(REGISTER_X, true, false));
      showInfoDialog("In function fnRj:", errorMessage, NULL, NULL);
    #endif
  }
}



void fnMirror(uint16_t unusedButMandatoryParameter) {
  if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
    uint64_t x, r=0;
    uint32_t i;

    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    x = *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X));
    if(x != 0) {
      for(i=0; i<shortIntegerWordSize; i++) {
        if(x & (1LL << i)) {
          r |= 1LL << (shortIntegerWordSize-i-1);
        }
      }
    }

    *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = r;
  }

  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if(EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "the input type %s is not allowed for MIRROR!", getDataTypeName(getRegisterDataType(REGISTER_X), false, false));
      showInfoDialog("In function fnMirror:", errorMessage, NULL, NULL);
    #endif
  }

  refreshRegisterLine(NIM_REGISTER_LINE);
}
