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
 * \file registers.c
 ***********************************************/

#include "wp43s.h"


/********************************************//**
 * \brief returns the data type of a register
 *
 * \param[in] r calcRegister_t Register number
 * \return uint32_t      Data type
 ***********************************************/
uint32_t getRegisterDataType(calcRegister_t regist) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    return reg[regist].dataType;
  }
  else if(regist < 1000) { // Local register
    if(allLocalRegisterPointer->numberOfLocalRegisters > 0) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < allLocalRegisterPointer->numberOfLocalRegisters) {
        return POINTER_TO_LOCAL_REGISTER(regist)->dataType;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "local register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, allLocalRegisterPointer->numberOfLocalRegisters - 1);
        showInfoDialog("In function getRegisterDataType:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function getRegisterDataType:", "no local registers defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
    #endif
  }
  else if(regist < SAVED_REGISTER_X) { // Named variable
    if(allNamedVariablePointer->numberOfNamedVariables > 0) {
      regist -= 1000;
      if(regist < allNamedVariablePointer->numberOfNamedVariables) {
        return POINTER_TO_NAMED_VARIABLE(regist)->dataType;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named variable %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, allNamedVariablePointer->numberOfNamedVariables - 1);
        showInfoDialog("In function getRegisterDataType:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function getRegisterDataType: no named variables defined!");
    }
  }
  else if(regist < 3000) { // Saved stack register
    if(regist <= LAST_SAVED_REGISTER) {
      return savedStackRegister[regist - SAVED_REGISTER_X].dataType;
    }
    #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "saved stack register %" FMT16S, regist - SAVED_REGISTER_X);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to 7");
      showInfoDialog("In function getRegisterDataType:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
    }
    #endif
  }
  else {
    sprintf(errorMessage, "In function getRegisterDataType: register=%" FMT16S " must be less then 4000!", regist);
    displayBugScreen(errorMessage);
  }
  return 31u;
}



/********************************************//**
 * \brief returns the data pointer of a register
 *
 * \param[in] r calcRegister_t Register number
 * \return dataBlock_t *       Data pointer
 ***********************************************/
dataBlock_t *getRegisterDataPointer(calcRegister_t regist) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    return TO_PCMEMPTR(reg[regist].dataPointer);
  }
  else if(regist < 1000) { // Local register
    if(allLocalRegisterPointer->numberOfLocalRegisters > 0) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < allLocalRegisterPointer->numberOfLocalRegisters) {
        return TO_PCMEMPTR(POINTER_TO_LOCAL_REGISTER(regist)->dataPointer);
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "local register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, allLocalRegisterPointer->numberOfLocalRegisters-1);
        showInfoDialog("In function getRegisterDataPointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function getRegisterDataPointer:", "no local registers defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
    #endif
  }
  else if(regist < SAVED_REGISTER_X) { // Named variable
    if(allNamedVariablePointer->numberOfNamedVariables > 0) {
      regist -= 1000;
      if(regist < allNamedVariablePointer->numberOfNamedVariables) {
        return TO_PCMEMPTR(POINTER_TO_NAMED_VARIABLE(regist)->dataPointer);
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named variable %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, allNamedVariablePointer->numberOfNamedVariables - 1);
        showInfoDialog("In function getRegisterDataPointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function getRegisterDataPointer: no named variables defined!");
    }
  }
  else if(regist < 3000) { // Saved stack register
    if(regist <= LAST_SAVED_REGISTER) {
      return TO_PCMEMPTR(savedStackRegister[regist - SAVED_REGISTER_X].dataPointer);
    }
    #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "saved stack register %" FMT16S, regist - SAVED_REGISTER_X);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to 7");
      showInfoDialog("In function getRegisterDataPointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
    }
    #endif
  }
  else {
    sprintf(errorMessage, "In function getRegisterDataPointer: r=%" FMT16S " must be less then 4000!", regist);
    displayBugScreen(errorMessage);
  }
  return 0;
}



/********************************************//**
 * \brief returns the data information of a register:
 *        Angular mode or base
 *
 * \param[in] r calcRegister_t Register number
 * \return uint32_t      Angular mode
 ***********************************************/
uint32_t getRegisterTag(calcRegister_t regist) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    return reg[regist].tag;
  }
  else if(regist < 1000) { // Local register
    if(allLocalRegisterPointer->numberOfLocalRegisters > 0) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < allLocalRegisterPointer->numberOfLocalRegisters) {
        return POINTER_TO_LOCAL_REGISTER(regist)->tag;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "local register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, allLocalRegisterPointer->numberOfLocalRegisters - 1);
        showInfoDialog("In function getRegisterTag:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    #ifdef PC_BUILD
    else {
     showInfoDialog("In function getRegisterTag:", "no local registers defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
    #endif
  }
  else if(regist < SAVED_REGISTER_X) { // Named variable
    if(allNamedVariablePointer->numberOfNamedVariables > 0) {
      regist -= 1000;
      if(regist < allNamedVariablePointer->numberOfNamedVariables) {
        return POINTER_TO_NAMED_VARIABLE(regist)->tag;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named variable %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, allNamedVariablePointer->numberOfNamedVariables - 1);
        showInfoDialog("In function getRegisterTag:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function getRegisterTag: no named variables defined!");
    }
  }
  else if(regist < 3000) { // Saved stack register
    if(regist <= LAST_SAVED_REGISTER) {
      return savedStackRegister[regist - SAVED_REGISTER_X].tag;
    }
    #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "saved stack register %" FMT16S, regist - SAVED_REGISTER_X);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to 7");
      showInfoDialog("In function getRegisterTag:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
    }
    #endif
  }
  else {
    sprintf(errorMessage, "In function getRegisterTag: r=%" FMT16S " must be less then 4000!", regist);
    displayBugScreen(errorMessage);
  }
  return 0;
}



/********************************************//**
 * \brief returns the length of the memory area of the name of the named variable
 *
 * \param[in] r calcRegister_t Register number
 * \return uint16_t            Length in blocks
 ***********************************************/
uint16_t getVariableNameLength(calcRegister_t regist) {
  if(1000 <= regist && regist <= 1999) { // Named variable
    if(allNamedVariablePointer->numberOfNamedVariables > 0) {
      regist -= 1000;
      if(regist < allNamedVariablePointer->numberOfNamedVariables) {
        return POINTER_TO_POINTER_TO_NAMED_VARIABLE_NAME(regist)->variableNameLen;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named variable %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, allNamedVariablePointer->numberOfNamedVariables - 1);
        showInfoDialog("In function getVariableNameLength:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
     #endif
    }
    else {
      displayBugScreen("In function getVariableNameLength: no named variables defined!");
    }
  }
  else {
    displayBugScreen("In function getVariableNameLength: this function can be called only for a named variable!");
  }
  return 0;
}



/********************************************//**
 * \brief returns the pointer to the memory area of the name of the named variable
 *
 * \param[in] r calcRegister_t Register number
 * \return char*               Pointer to the name
 ***********************************************/
char *getVariableNamePointer(calcRegister_t regist) {
  if(1000 <= regist && regist <= 1999) { // Named variable
    if(allNamedVariablePointer->numberOfNamedVariables > 0) {
      regist -= 1000;
      if(regist < allNamedVariablePointer->numberOfNamedVariables) {
        return TO_PCMEMPTR(POINTER_TO_POINTER_TO_NAMED_VARIABLE_NAME(regist)->ptrToVariableName);
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named variable %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, allNamedVariablePointer->numberOfNamedVariables - 1);
        showInfoDialog("In function getVariableNamePointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function getVariableNamePointer: no named variables defined!");
    }
   }
  else {
    displayBugScreen("In function getVariableNamePointer: this function can be called only for a named variable!");
  }
  return 0;
}



/********************************************//**
 * \brief Sets the data type of a register
 *
 * \param[in] r calcRegister_t        Register number
 * \param[in] dataType uint16_t Data type
 * \param[in] tag      uint16_t Tag
 * \return void
 ***********************************************/
void setRegisterDataType(calcRegister_t regist, uint16_t dataType, uint32_t tag) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    reg[regist].dataType = dataType;
    reg[regist].tag = tag;
  }
  else if(regist < 1000) { // Local register
    if(allLocalRegisterPointer->numberOfLocalRegisters > 0) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < allLocalRegisterPointer->numberOfLocalRegisters) {
        POINTER_TO_LOCAL_REGISTER(regist)->dataType = dataType;
        POINTER_TO_LOCAL_REGISTER(regist)->tag = tag;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "local register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, allLocalRegisterPointer->numberOfLocalRegisters - 1);
        showInfoDialog("In function setRegisterDataType:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    #ifdef PC_BUILD
    else {
     showInfoDialog("In function setRegisterDataType:", "no local registers defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
    #endif
  }
  else if(regist < SAVED_REGISTER_X) { // Named variable
    if(allNamedVariablePointer->numberOfNamedVariables > 0) {
      regist -= 1000;
      if(regist < allNamedVariablePointer->numberOfNamedVariables) {
        POINTER_TO_NAMED_VARIABLE(regist)->dataType = dataType;
        POINTER_TO_NAMED_VARIABLE(regist)->tag = tag;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named variable %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, allNamedVariablePointer->numberOfNamedVariables - 1);
        showInfoDialog("In function setRegisterDataType:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function setRegisterDataType: no named variables defined!");
    }
  }
  else if(regist < 3000) { // Saved stack register
    if(regist <= LAST_SAVED_REGISTER) {
      savedStackRegister[regist - SAVED_REGISTER_X].dataType = dataType;
      savedStackRegister[regist - SAVED_REGISTER_X].tag = tag;
    }
    #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "saved stack register %" FMT16S, regist - SAVED_REGISTER_X);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to 7");
      showInfoDialog("In function setRegisterDataType:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
    }
    #endif
  }
  else {
    sprintf(errorMessage, "In function setRegisterDataType: r=%" FMT16S " must be less then 4000!", regist);
    displayBugScreen(errorMessage);
  }
}



/********************************************//**
 * \brief Sets the data pointer of a register
 *
 * \param[in] r calcRegister_t        Register number
 * \param[in] dataType uint16_t Data pointer
 * \return void
 ***********************************************/
void setRegisterDataPointer(calcRegister_t regist, void *memPtr) {
  uint32_t dataPointer = TO_WP43SMEMPTR(memPtr);

  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    reg[regist].dataPointer = dataPointer;
  }
  else if(regist < 1000) { // Local register
    if(allLocalRegisterPointer->numberOfLocalRegisters > 0) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < allLocalRegisterPointer->numberOfLocalRegisters) {
        POINTER_TO_LOCAL_REGISTER(regist)->dataPointer = dataPointer;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "local register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, allLocalRegisterPointer->numberOfLocalRegisters - 1);
        showInfoDialog("In function setRegisterDataPointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function setRegisterDataPointer:", "no local registers defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
    #endif
  }
  else if(regist < SAVED_REGISTER_X) { // Named variable
    if(allNamedVariablePointer->numberOfNamedVariables > 0) {
      regist -= 1000u;
      if(regist < allNamedVariablePointer->numberOfNamedVariables) {
        POINTER_TO_NAMED_VARIABLE(regist)->dataPointer = dataPointer;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named variable %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, allNamedVariablePointer->numberOfNamedVariables - 1);
        showInfoDialog("In function setRegisterDataPointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function setRegisterDataPointer:", "no local registers defined!", NULL, NULL);
    }
    #endif
  }
  else if(regist < 3000) { // Saved stack register
    if(regist <= LAST_SAVED_REGISTER) {
      savedStackRegister[regist - SAVED_REGISTER_X].dataPointer = dataPointer;
    }
    #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "saved stack register %" FMT16S, regist - SAVED_REGISTER_X);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to 7");
      showInfoDialog("In function setRegisterDataPointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
    }
    #endif
  }
  else {
    sprintf(errorMessage, "In function setRegisterDataPointer: r=%" FMT16S " must be less then 4000!", regist);
    displayBugScreen(errorMessage);
  }
}



/********************************************//**
 * \brief Sets the data information of a register:
 *        angular mode or base
 *
 * \param[in] r calcRegister_t        Register number
 * \param[in] dataType uint16_t Angular mode
 * \return void
 ***********************************************/
void setRegisterTag(calcRegister_t regist, uint32_t tag) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    reg[regist].tag = tag;
  }
  else if(regist < 1000) { // Local register
    if(allLocalRegisterPointer->numberOfLocalRegisters > 0) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < allLocalRegisterPointer->numberOfLocalRegisters) {
        POINTER_TO_LOCAL_REGISTER(regist)->tag = tag;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "local register %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, allLocalRegisterPointer->numberOfLocalRegisters - 1);
        showInfoDialog("In function setRegisterDataInfo:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function setRegisterDataInfo:", "no local registers defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
    #endif
  }
  else if(regist < SAVED_REGISTER_X) { // Named variable
    if(allNamedVariablePointer->numberOfNamedVariables > 0) {
      regist -= 1000u;
      if(regist < allNamedVariablePointer->numberOfNamedVariables) {
        POINTER_TO_NAMED_VARIABLE(regist)->tag = tag;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named variable %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, allNamedVariablePointer->numberOfNamedVariables - 1);
        showInfoDialog("In function setRegisterDataInfo:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function setRegisterDataInfo: no named variables defined!");
    }
   }
  else if(regist < 3000) { // Saved stack register
    if(regist <= LAST_SAVED_REGISTER) {
      savedStackRegister[regist - SAVED_REGISTER_X].tag = tag;
    }
    #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "saved stack register %" FMT16S, regist - SAVED_REGISTER_X);
      sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to 7");
      showInfoDialog("In function setRegisterDataInfo:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
    }
    #endif
  }
  else {
    sprintf(errorMessage, "In function setRegisterDataInfo: r=%" FMT16S " must be less then 4000!", regist);
    displayBugScreen(errorMessage);
  }
}



/********************************************//**
 * \brief Sets the length of the memory area of the name of the named variable
 *
 * \param[in] r calcRegister_t  Register number
 * \param[in] dataType uint16_t Length in blocks
 * \return void
 ***********************************************/
void setVariableNameLength(calcRegister_t regist, uint16_t length) {
  if(1000 <= regist && regist <= 1999) { // Named variable
    if(allNamedVariablePointer->numberOfNamedVariables > 0) {
      regist -= 1000;
      if(regist < allNamedVariablePointer->numberOfNamedVariables) {
        POINTER_TO_POINTER_TO_NAMED_VARIABLE_NAME(regist)->variableNameLen = length;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "named variable %" FMT16S, regist);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, allNamedVariablePointer->numberOfNamedVariables - 1);
        showInfoDialog("In function setVariableNameLength:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
      }
      #endif
    }
    else {
      displayBugScreen("In function setVariableNameLength: no named variables defined!");
    }
  }
  else {
    displayBugScreen("In function setVariableNameLength: this function can be called only for a named variable!");
  }
}



/********************************************//**
 * \brief Sets the pointer to the memory area of the name of the named variable
 *
 * \param[in] r calcRegister_t        Register number
 * \param[in] dataType uint16_t Pointer
 * \return void
 ***********************************************/
void setVariableNamePointer(calcRegister_t regist, void *namePointer) {
  if(1000 <= regist && regist <= 1999) { // Named variable
    if(allNamedVariablePointer->numberOfNamedVariables > 0) {
      regist -= 1000;
      if(regist < allNamedVariablePointer->numberOfNamedVariables) {
        POINTER_TO_POINTER_TO_NAMED_VARIABLE_NAME(regist)->ptrToVariableName = TO_WP43SMEMPTR(namePointer);
      }
      else {
        sprintf(errorMessage, "In function setVariableNamePointer: named variable %" FMT16S " is not defined! Must be from 0 to %" FMT16U, regist, allNamedVariablePointer->numberOfNamedVariables - 1);
        displayBugScreen(errorMessage);
      }
    }
    else {
      displayBugScreen("In function setVariableNamePointer: no named variables defined!");
    }
  }
  else {
    displayBugScreen("In function setVariableNamePointer: this function can be called only for a named variable!");
  }
}



/********************************************//**
 * \brief Allocates local registers. Works when increasing
 * and when decreasing the number of local registers.
 *
 * \param[in] numberOfRegistersToAllocate uint16_t Number of registers to allocate
 * \return void
 ***********************************************/
void allocateLocalRegisters(uint16_t numberOfRegistersToAllocate) {
  if(numberOfRegistersToAllocate > 99) {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "You can allocate up to 99 registers, you requested %" FMT16U, numberOfRegistersToAllocate);
      showInfoDialog("In function allocateLocalRegisters:", errorMessage, NULL, NULL);
    #endif
    return;
  }

  if(numberOfRegistersToAllocate != allLocalRegisterPointer->numberOfLocalRegisters) { // There is something to do
    uint16_t numRegs, r;

    if(numberOfRegistersToAllocate == 0) { // free the allocated memory for the local registers
      numRegs = allLocalRegisterPointer->numberOfLocalRegisters;

      // free memory allocated to the data of the local registers
      for(r=FIRST_LOCAL_REGISTER; r<FIRST_LOCAL_REGISTER+numRegs; r++) {
        freeRegisterData(r);
      }

      allLocalRegisterPointer->numberOfLocalRegisters = 0; // This must be done before the freeMemory call
      numberOfLocalFlags = 0;
      reallocWp43s(allLocalRegisterPointer, TO_BYTES(numRegs + 1), TO_BYTES(1)); // +1 for the fags
      allLocalRegisterPointer->numberOfLocalRegisters = 0;
      allLocalRegisterPointer->localFlags = 0;
    }

    else if(allLocalRegisterPointer->numberOfLocalRegisters == 0) { // Allocate memory
      allLocalRegisterPointer = reallocWp43s(allLocalRegisterPointer, TO_BYTES(1), TO_BYTES(1u + numberOfRegistersToAllocate));
      allLocalRegisterPointer->numberOfLocalRegisters = numberOfRegistersToAllocate;
      numberOfLocalFlags = NUMBER_OF_LOCAL_FLAGS;

      // clear all local flags
      allLocalRegisterPointer->localFlags = 0;

      // All the new local registers are real34s initialized to 0.0
      for(r=FIRST_LOCAL_REGISTER; r<FIRST_LOCAL_REGISTER+numberOfRegistersToAllocate; r++) {
        if(Input_Default == ID_43S || Input_Default == ID_DP) {                 //JM defaults JMZERO
        setRegisterDataType(r, dtReal34, AM_NONE);
        setRegisterDataPointer(r, allocWp43s(TO_BYTES(REAL34_SIZE)));
        real34Zero(REGISTER_REAL34_DATA(r));
        }                                                   //JM defaults ^^
        else if(Input_Default == ID_CPXDP) {                //JM defaults vv
          setRegisterDataType(r, dtComplex34, AM_NONE);
          setRegisterDataPointer(r, allocWp43s(TO_BYTES(COMPLEX34_SIZE)));
          real34Zero(REGISTER_REAL34_DATA(r));
          real34Zero(REGISTER_IMAG34_DATA(r));
        }                                                   //JM defaults ^^
        else if(Input_Default == ID_SI) {                   //JM defaults vv
          longInteger_t lgInt;
          longIntegerInit(lgInt);
          uint16_t val =0;
          uIntToLongInteger(val,lgInt);
          convertLongIntegerToShortIntegerRegister(lgInt, lastIntegerBase, r);
          longIntegerFree(lgInt);
        }                                                   //JM defaults ^^
        else if(Input_Default == ID_LI) {                   //JM defaults vv
          longInteger_t lgInt;
          longIntegerInit(lgInt);
          uint16_t val =0;
          uIntToLongInteger(val,lgInt);
          convertLongIntegerToLongIntegerRegister(lgInt, r);
          longIntegerFree(lgInt);
        }                                                   //JM defaults ^^
      }
    }

    else if(numberOfRegistersToAllocate > allLocalRegisterPointer->numberOfLocalRegisters) { // increase the number of local register
      reallocWp43s(allLocalRegisterPointer,  TO_BYTES(1u + allLocalRegisterPointer->numberOfLocalRegisters), TO_BYTES(1u + numberOfRegistersToAllocate));

      uint16_t oldNumRegs = allLocalRegisterPointer->numberOfLocalRegisters;
      allLocalRegisterPointer->numberOfLocalRegisters = numberOfRegistersToAllocate;

      // All the new local registers are real16s initialized to 0.0
      for(r=FIRST_LOCAL_REGISTER+oldNumRegs; r<FIRST_LOCAL_REGISTER+numberOfRegistersToAllocate; r++) {
        if(Input_Default == ID_43S || Input_Default == ID_DP) {                 //JM defaults JMZERO
        setRegisterDataType(r, dtReal34, AM_NONE);
        setRegisterDataPointer(r, allocWp43s(TO_BYTES(REAL34_SIZE)));
        real34Zero(REGISTER_REAL34_DATA(r));
        }                                                   //JM defaults ^^
        else if(Input_Default == ID_CPXDP) {                //JM defaults vv
          setRegisterDataType(r, dtComplex34, AM_NONE);
          setRegisterDataPointer(r, allocWp43s(TO_BYTES(COMPLEX34_SIZE)));
          real34Zero(REGISTER_REAL34_DATA(r));
          real34Zero(REGISTER_IMAG34_DATA(r));
        }                                                   //JM defaults ^^
        else if(Input_Default == ID_SI) {                   //JM defaults vv
          longInteger_t lgInt;
          longIntegerInit(lgInt);
          uint16_t val =0;
          uIntToLongInteger(val,lgInt);
          convertLongIntegerToShortIntegerRegister(lgInt, lastIntegerBase, r);
          longIntegerFree(lgInt);
        }                                                   //JM defaults ^^
        else if(Input_Default == ID_LI) {                   //JM defaults vv
          longInteger_t lgInt;
          longIntegerInit(lgInt);
          uint16_t val =0;
          uIntToLongInteger(val,lgInt);
          convertLongIntegerToLongIntegerRegister(lgInt, r);
          longIntegerFree(lgInt);
        }                                                   //JM defaults ^^
      }
    }

    else { // reduce the number of local register
      // free memory allocated to the data of the deleted local registers
      for(r=allLocalRegisterPointer->numberOfLocalRegisters - 1; r>=numberOfRegistersToAllocate; r--) {
        freeRegisterData(FIRST_LOCAL_REGISTER + r);
      }

      // free memory allocated to the deleted local registers
      freeWp43s(allLocalRegisterPointer + 1u + numberOfRegistersToAllocate, TO_BYTES(allLocalRegisterPointer->numberOfLocalRegisters - numberOfRegistersToAllocate));
      allLocalRegisterPointer->numberOfLocalRegisters = numberOfRegistersToAllocate; // This must be done after the freeMemory call
    }
  }
}



/********************************************//**
 * \brief Allocates 1 named variable.
 *
 * \param[in] variableName const char* Register name
 * \return void
 ***********************************************/
void allocateNamedVariable(const char *variableName) {
  uint32_t len;
  char *namePtr;
  calcRegister_t regist;

  if(stringGlyphLength(variableName) < 1 || stringGlyphLength(variableName) > 7) {
    #ifdef PC_BUILD
      sprintf(errorMessage, "the name %s", variableName);
      showInfoDialog("In function allocateNamedVariable:", errorMessage, "is incorrect! The length must be", "from 1 to 7 glyphs!");
    #endif
    return;
  }

  if(allNamedVariablePointer->numberOfNamedVariables == 0) { // First named variable
    allNamedVariablePointer = reallocWp43s(allNamedVariablePointer, TO_BYTES(1), TO_BYTES(3)); // 3 blocks: 1 for the number of named variables, 1 for the register descriptor, and 1 for the pointer to the variable name
    allNamedVariablePointer->numberOfNamedVariables = 1;

    regist = 1000;
  }
  else {
    regist = allNamedVariablePointer->numberOfNamedVariables;
    if(regist == 999u) {
      #ifdef PC_BUILD
        showInfoDialog("In function allocateNamedVariable:", "you can allocate up to", "999 named variables!", NULL);
      #endif
      return;
    }

    reallocWp43s(allNamedVariablePointer, TO_BYTES(1u + 2u*allNamedVariablePointer->numberOfNamedVariables), TO_BYTES(1u + 2u*(allNamedVariablePointer->numberOfNamedVariables + 1)));
    (allNamedVariablePointer->numberOfNamedVariables)++;

    regist += 1000;
  }

  // The new named variable is a real34 initialized to 0.0
  setRegisterDataType(regist, dtReal34, AM_NONE);

  if(Input_Default == ID_43S || Input_Default == ID_DP) {                       //JM defaults JMZERO
    setRegisterDataType(regist, dtReal34, AM_NONE);

  len = TO_BYTES(TO_BLOCKS(stringByteLength(variableName) + 1)); // +1 for the trailing zero

  namePtr = allocWp43s(len);
  setVariableNamePointer(regist, namePtr);
  setVariableNameLength(regist, TO_BLOCKS(len));
  xcopy(namePtr, variableName, len);

  setRegisterDataPointer(regist, allocWp43s(TO_BYTES(REAL34_SIZE)));
  real34Zero(REGISTER_REAL34_DATA(regist));
  }                                                         //JM defaults
  else if(Input_Default == ID_CPXDP) {                      //JM defaults vv
    setRegisterDataType(regist, dtComplex34, AM_NONE);
    len = TO_BYTES(TO_BLOCKS(stringByteLength(variableName) + 1)); // +1 for the trailing zero
    namePtr = allocWp43s(len);
    setVariableNamePointer(regist, namePtr);
    setVariableNameLength(regist, TO_BLOCKS(len));
    xcopy(namePtr, variableName, len);
    setRegisterDataPointer(regist, allocWp43s(TO_BYTES(COMPLEX34_SIZE)));
    real34Zero(REGISTER_REAL34_DATA(regist));
    real34Zero(REGISTER_IMAG34_DATA(regist));
  }                                                         //JM defaults ^^
  else if(Input_Default == ID_SI) {                         //JM defaults vv
    setRegisterDataType(regist, dtShortInteger, AM_NONE);
    len = TO_BYTES(TO_BLOCKS(stringByteLength(variableName) + 1)); // +1 for the trailing zero
    namePtr = allocWp43s(len);
    setVariableNamePointer(regist, namePtr);
    setVariableNameLength(regist, TO_BLOCKS(len));
    xcopy(namePtr, variableName, len);
    setRegisterDataPointer(regist, allocWp43s(TO_BYTES(SHORT_INTEGER_SIZE)));
    longInteger_t lgInt;
    longIntegerInit(lgInt);
    uint16_t val =0;
    uIntToLongInteger(val,lgInt);
    convertLongIntegerToShortIntegerRegister(lgInt, lastIntegerBase, regist);
    longIntegerFree(lgInt);
  }                                                         //JM defaults ^^
  else if(Input_Default == ID_LI) {                         //JM defaults vv
    setRegisterDataType(regist, dtShortInteger, AM_NONE);
    len = TO_BYTES(TO_BLOCKS(stringByteLength(variableName) + 1)); // +1 for the trailing zero
    namePtr = allocWp43s(len);
    setVariableNamePointer(regist, namePtr);
    setVariableNameLength(regist, TO_BLOCKS(len));
    xcopy(namePtr, variableName, len);
    setRegisterDataPointer(regist, allocWp43s(TO_BYTES(SHORT_INTEGER_SIZE)));
    longInteger_t lgInt;
    longIntegerInit(lgInt);
    uint16_t val =0;
    uIntToLongInteger(val,lgInt);
    convertLongIntegerToLongIntegerRegister(lgInt, regist);
    longIntegerFree(lgInt);
  }                                                         //JM defaults ^^
}



/********************************************//**
 * \brief Sets the max length of string in blocks
 *
 * \param[in] r calcRegister_t            Register number
 * \param[in] maxStringLen uint16_t       Max length of the string
 * \return void
 ***********************************************/
void setRegisterMaxDataLength(calcRegister_t regist, uint16_t maxDataLen) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    ((dataBlock_t *)TO_PCMEMPTR(reg[regist].dataPointer))->dataMaxLength = maxDataLen;
  }
  else if(regist < 1000) { // Local register
    if(allLocalRegisterPointer->numberOfLocalRegisters > 0) {
      if(regist-FIRST_LOCAL_REGISTER < allLocalRegisterPointer->numberOfLocalRegisters) {
        getRegisterDataPointer(regist)->dataMaxLength = maxDataLen;
      }
      #ifdef PC_BUILD
      else {
        sprintf(errorMessage, "local register %" FMT16S " is not defined!", regist - FIRST_LOCAL_REGISTER);
        sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" FMT16U, allLocalRegisterPointer->numberOfLocalRegisters - 1);
        showInfoDialog("In function setRegisterMaxDataLength:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
      }
      #endif
    }
    #ifdef PC_BUILD
    else {
     showInfoDialog("In function setRegisterMaxDataLength:", "no local registers defined!", NULL, NULL);
    }
    #endif
  }
  else if(regist < SAVED_REGISTER_X) { // Named variable
    if(allNamedVariablePointer->numberOfNamedVariables > 0) {
      if(regist-1000 < allNamedVariablePointer->numberOfNamedVariables) {
        getRegisterDataPointer(regist)->dataMaxLength = maxDataLen;
      }
      else {
        sprintf(errorMessage, "In function setRegisterMaxDataLength: named variable %" FMT16S " is not defined! Must be from 0 to %" FMT16U, regist - 1000, allNamedVariablePointer->numberOfNamedVariables - 1);
        displayBugScreen(errorMessage);
      }
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function setRegisterMaxDataLength:", "no named variables defined!", NULL, NULL);
    }
    #endif
  }
  else if(regist < 3000) { // Saved stack register
    if(regist <= LAST_SAVED_REGISTER) {
      getRegisterDataPointer(regist)->dataMaxLength = maxDataLen;
    }
    else {
      sprintf(errorMessage, "In function setRegisterMaxDataLength: saved stack register %" FMT16S " is not defined! Must be from 0 to 7!", regist - SAVED_REGISTER_X);
      displayBugScreen(errorMessage);
    }
  }
  else {
    sprintf(errorMessage, "In function setRegisterMaxDataLength: regist=%" FMT16S " must be less then 4000!", regist);
    displayBugScreen(errorMessage);
  }
}



/********************************************//**
 * \brief Returns the max length of a string
          including trailing 0, or a
          long integer in blocks
 *
 * \param[in] r calcRegister_t Register number
 * \return uint32_t            Number of blocks
 *
 ***********************************************/
uint16_t getRegisterMaxDataLength(calcRegister_t regist) {
  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    return ((dataBlock_t *)TO_PCMEMPTR(reg[regist].dataPointer))->dataMaxLength;
  }
  else if(regist < 1000) { // Local register
    if(allLocalRegisterPointer->numberOfLocalRegisters > 0) {
      if(regist-FIRST_LOCAL_REGISTER < allLocalRegisterPointer->numberOfLocalRegisters) {
        return ((dataBlock_t *)TO_PCMEMPTR(POINTER_TO_LOCAL_REGISTER(regist)->dataPointer))->dataMaxLength;
      }
      else {
        sprintf(errorMessage, "In function getRegisterMaxDataLength: local register %" FMT16S " is not defined! Must be from 0 to %" FMT16U, regist - FIRST_LOCAL_REGISTER, allLocalRegisterPointer->numberOfLocalRegisters - 1);
        displayBugScreen(errorMessage);
      }
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function getRegisterMaxStringLength:", "no local registers defined!", NULL, NULL);
    }
    #endif
  }
  else if(regist < SAVED_REGISTER_X) { // Named variable
    if(allNamedVariablePointer->numberOfNamedVariables != 0) {
      if(regist-1000 < allNamedVariablePointer->numberOfNamedVariables) {
        return ((dataBlock_t *)TO_PCMEMPTR(POINTER_TO_NAMED_VARIABLE(regist)->dataPointer))->dataMaxLength;
      }
      else {
        sprintf(errorMessage, "In function getRegisterMaxDataLength: named variable %" FMT16S " is not defined! Must be from 0 to %" FMT16U, regist - 1000, allNamedVariablePointer->numberOfNamedVariables - 1);
        displayBugScreen(errorMessage);
      }
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function getRegisterMaxStringLength:", "no named variables defined!", NULL, NULL);
    }
    #endif
  }
  else if(regist < 3000) { // Saved stack register
    if(regist <= LAST_SAVED_REGISTER) {
      return ((dataBlock_t *)TO_PCMEMPTR(savedStackRegister[regist - SAVED_REGISTER_X].dataPointer))->dataMaxLength;
    }
    else {
      sprintf(errorMessage, "In function getRegisterMaxDataLength: saved stack register %" FMT16S " is not defined! Must be from 0 to 7!", regist - SAVED_REGISTER_X);
      displayBugScreen(errorMessage);
    }
  }
  return 0;
}



/********************************************//**
 * \brief Returns the full data size of a register in blocks
 *
 * \param[in] r calcRegister_t Register number
 * \return uint16_t    Number of blocks. For a string this
 *                     is the number of bytes reserved for
 *                     the string (including the ending 0)
 *                     plus 1 block holding the max size
 *                     of the string.
 ***********************************************/
uint16_t getRegisterFullSize(calcRegister_t regist) {
  switch(getRegisterDataType(regist)) {
    case dtLongInteger:  return getRegisterDataPointer(regist)->dataMaxLength + 1;
    //case dtTime:
    //case dtDate:
    case dtString:       return getRegisterDataPointer(regist)->dataMaxLength + 1;
    //case dtReal16Matrix:
    //case dtComplex16Matrix:
    case dtShortInteger: return SHORT_INTEGER_SIZE;
    case dtReal34:       return REAL34_SIZE;
    case dtComplex34:    return COMPLEX34_SIZE;
    case dtConfig:       return CONFIG_SIZE;

    default:
      sprintf(errorMessage, "In function getRegisterFullSize: data type %s is unknown!", getDataTypeName(getRegisterDataType(regist), false, false));
      displayBugScreen(errorMessage);
      return 0;
  }
}



/********************************************//**
 * \brief Clears a register, that is, sets it to 0,0 real16
 *
 * \param[in] r calcRegister_t Register number
 * \return void
 ***********************************************/
void clearRegister(calcRegister_t regist) {
  if(Input_Default == ID_43S || Input_Default == ID_DP) {                       //JM defaults JMZERO
    if(getRegisterDataType(regist) == dtReal34) {
      real34Zero(REGISTER_REAL34_DATA(regist));
      setRegisterTag(regist, AM_NONE);
    }
    else{
      reallocateRegister(regist, dtReal34, REAL34_SIZE, AM_NONE);
      real34Zero(REGISTER_REAL34_DATA(regist));
    }
  }                                                                             //JM defaults ^^
  else if(Input_Default == ID_CPXDP) {                                          //JM defaults vv
    if(getRegisterDataType(regist) == dtComplex34) {
      real34Zero(REGISTER_REAL34_DATA(regist));
      real34Zero(REGISTER_IMAG34_DATA(regist));
      setRegisterTag(regist, AM_NONE);
    }
    else{
      reallocateRegister(regist, dtComplex34, COMPLEX34_SIZE, AM_NONE);
      real34Zero(REGISTER_REAL34_DATA(regist));
      real34Zero(REGISTER_IMAG34_DATA(regist));
    }
  }                                                                             //JM defaults ^^
  else if(Input_Default == ID_SI) {                                             //JM defaults vv
    //JM comment: Not checking if already the correct type, just changing it. Wasting some steps.
    longInteger_t lgInt;
    longIntegerInit(lgInt);
    uint16_t val =0;
    uIntToLongInteger(val,lgInt);
    convertLongIntegerToShortIntegerRegister(lgInt, lastIntegerBase, regist);
    longIntegerFree(lgInt);
  }                                                                             //JM defaults ^^
  else if(Input_Default == ID_LI) {                                             //JM defaults vv
    //JM comment: Not checking if already the correct type, just changing it. Wasting some steps.
    longInteger_t lgInt;
    longIntegerInit(lgInt);
    uint16_t val =0;
    uIntToLongInteger(val,lgInt);
    convertLongIntegerToLongIntegerRegister(lgInt, regist);
    longIntegerFree(lgInt);
  }                                                                             //JM defaults ^^
}



/********************************************//**
 * \brief Clears all the regs (globals and locals),
 * that is sets them to 0,0 real16s
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnClearRegisters(uint16_t unusedParamButMandatory) {
  calcRegister_t regist;

  for(regist=0; regist<REGISTER_X; regist++) {
    clearRegister(regist);
  }

  for(regist=0; regist<allLocalRegisterPointer->numberOfLocalRegisters; regist++) {
    clearRegister(FIRST_LOCAL_REGISTER + regist);
  }

  if(!getSystemFlag(FLAG_SSIZE8)) { // Stack size = 4
    for(regist=REGISTER_A; regist<=REGISTER_D; regist++) {
      clearRegister(regist);
    }
  }

  for(regist=REGISTER_I; regist<=REGISTER_K; regist++) {
    clearRegister(regist);
  }
}



/********************************************//**
 * \brief Sets X to the number of local registers
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGetLocR(uint16_t unusedParamButMandatory) {
  longInteger_t locR;

  liftStack();

  longIntegerInit(locR);
  uIntToLongInteger(allLocalRegisterPointer->numberOfLocalRegisters, locR);
  convertLongIntegerToLongIntegerRegister(locR, REGISTER_X);
  longIntegerFree(locR);

  refreshStack();
}



void adjustResult(calcRegister_t res, bool_t dropY, bool_t setCpxRes, calcRegister_t op1, calcRegister_t op2, calcRegister_t op3) {
  uint32_t resultDataType;
  bool_t oneArgumentIsComplex = false;

  if(op1 >= 0) {
    oneArgumentIsComplex = oneArgumentIsComplex || getRegisterDataType(op1) == dtComplex34;
  }

  if(op2 >= 0) {
    oneArgumentIsComplex = oneArgumentIsComplex || getRegisterDataType(op2) == dtComplex34;
  }

  if(op3 >= 0) {
    oneArgumentIsComplex = oneArgumentIsComplex || getRegisterDataType(op3) == dtComplex34;
  }

  if(lastErrorCode != 0) {
    restoreStack();
    refreshStack();
    return;
  }

  resultDataType = getRegisterDataType(res);
  if(getSystemFlag(FLAG_SPCRES) == false) {
    // D is clear: test infinite values and -0 values
    switch(resultDataType) {
      case dtReal34:
        if(real34IsInfinite(REGISTER_REAL34_DATA(res))) {
          displayCalcErrorMessage(real34IsPositive(REGISTER_REAL34_DATA(res)) ? ERROR_OVERFLOW_PLUS_INF : ERROR_OVERFLOW_MINUS_INF , ERR_REGISTER_LINE, res);
        }
        else if(real34IsZero(REGISTER_REAL34_DATA(res))) {
          real34SetPositiveSign(REGISTER_REAL34_DATA(res));
        }
        break;

      case dtComplex34:
        if(real34IsInfinite(REGISTER_REAL34_DATA(res))) {
          displayCalcErrorMessage(real34IsPositive(REGISTER_REAL34_DATA(res)) ? ERROR_OVERFLOW_PLUS_INF : ERROR_OVERFLOW_MINUS_INF , ERR_REGISTER_LINE, res);
        }
        else if(real34IsZero(REGISTER_REAL34_DATA(res))) {
          real34SetPositiveSign(REGISTER_REAL34_DATA(res));
        }
        else if(real34IsInfinite(REGISTER_IMAG34_DATA(res))) {
          displayCalcErrorMessage(real34IsPositive(REGISTER_IMAG34_DATA(res)) ? ERROR_OVERFLOW_PLUS_INF : ERROR_OVERFLOW_MINUS_INF , ERR_REGISTER_LINE, res);
        }
        else if(real34IsZero(REGISTER_IMAG34_DATA(res))) {
          real34SetPositiveSign(REGISTER_IMAG34_DATA(res));
        }
        break;

      default:
        break;
    }
  }

  if(lastErrorCode != 0) {
    restoreStack();
    refreshStack();
    return;
  }

  if(setCpxRes && oneArgumentIsComplex && resultDataType != dtString) {
    fnSetFlag(FLAG_CPXRES);
    
//  fnRefreshRadioState(RB_BCR, true);                                          //dr
    fnRefreshComboxState(CB_JC, JC_BCR, true);                                  //dr
  }

  // Round the register value
  switch(resultDataType) {
    real_t tmp;

    case dtReal34:
      if(significantDigits == 0 || significantDigits >= 34) {
        break;
      }

      ctxtReal39.digits = significantDigits;
      real34ToReal(REGISTER_REAL34_DATA(res), &tmp);
      ctxtReal39.digits = 39;
      realToReal34(&tmp, REGISTER_REAL34_DATA(res));
      break;

    case dtComplex34:
      if(significantDigits == 0 || significantDigits >= 34) {
        break;
      }

      ctxtReal39.digits = significantDigits;
      real34ToReal(REGISTER_REAL34_DATA(res), &tmp);
      realToReal34(&tmp, REGISTER_REAL34_DATA(res));
      real34ToReal(REGISTER_IMAG34_DATA(res), &tmp);
      realToReal34(&tmp, REGISTER_IMAG34_DATA(res));
      ctxtReal39.digits = 39;
      break;

    default:
      break;
  }

  if(dropY) {
    fnDropY(NOPARAM);
    refreshStack();
  }
  else {
    refreshRegisterLine(res);
  }
}



/********************************************//**
 * \brief Duplicates register source to register destination
 *
 * \param[in] sourceRegister Source register
 * \param[in] destRegister   Destination register
 * \return void
 ***********************************************/
void copySourceRegisterToDestRegister(calcRegister_t sourceRegister, calcRegister_t destRegister) {
    if(   getRegisterDataType(destRegister) != getRegisterDataType(sourceRegister)
      || getRegisterFullSize(destRegister) != getRegisterFullSize(sourceRegister)) {
      uint32_t sizeInBlocks;

      switch(getRegisterDataType(sourceRegister)) {
        case dtLongInteger:  sizeInBlocks = getRegisterDataPointer(sourceRegister)->dataMaxLength; break;
        //case dtTime:
        //case dtDate:
        case dtString:       sizeInBlocks = getRegisterDataPointer(sourceRegister)->dataMaxLength; break;
        //case dtReal16Matrix:
        //case dtComplex16Matrix:
        case dtShortInteger: sizeInBlocks = SHORT_INTEGER_SIZE;                                    break;
        case dtReal34:       sizeInBlocks = REAL34_SIZE;                                           break;
        case dtComplex34:    sizeInBlocks = COMPLEX34_SIZE;                                        break;
        case dtConfig:       sizeInBlocks = CONFIG_SIZE;                                           break;

        default:
          sprintf(errorMessage, "In function copySourceRegisterToDestRegister: data type %s is unknown!", getDataTypeName(getRegisterDataType(sourceRegister), false, false));
          displayBugScreen(errorMessage);
          sizeInBlocks = 0;
      }
      reallocateRegister(destRegister, getRegisterDataType(sourceRegister), sizeInBlocks, AM_NONE);
    }

    xcopy(REGISTER_DATA(destRegister), REGISTER_DATA(sourceRegister), TO_BYTES(getRegisterFullSize(sourceRegister)));
    setRegisterTag(destRegister, getRegisterTag(sourceRegister));
}



/********************************************//**
 * \brief Stores X in an other register
 *
 * \param[in] registerNumber uint16_t
 * \return void
 ***********************************************/
void fnStore(uint16_t r) {
  calcRegister_t regist = r;

  if(regist < FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters) {
    copySourceRegisterToDestRegister(REGISTER_X, regist);
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnStore:", errorMessage, "is not defined!", NULL);
  }
  #endif

  refreshStack();
}



/********************************************//**
 * \brief Adds X to a register
 *
 * \param[in] registerNumber uint16_t
 * \return void
 ***********************************************/
void fnStoreAdd(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(regist, REGISTER_Y);

    addition[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);
    copySourceRegisterToDestRegister(REGISTER_X, regist);
    copySourceRegisterToDestRegister(SAVED_REGISTER_X, REGISTER_X);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnStorePlus:", errorMessage, "is not defined!", NULL);
  }
  #endif
}



/********************************************//**
 * \brief Subtracts X from a register
 *
 * \param[in] registerNumber calcRegister_t
 * \return void
 ***********************************************/
void fnStoreSub(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(regist, REGISTER_Y);

    subtraction[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);
    copySourceRegisterToDestRegister(REGISTER_X, regist);
    copySourceRegisterToDestRegister(SAVED_REGISTER_X, REGISTER_X);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnStoreSub:", errorMessage, "is not defined!", NULL);
  }
  #endif
}



/********************************************//**
 * \brief Multiplies a register by X
 *
 * \param[in] registerNumber uint16_t
 * \return void
 ***********************************************/
void fnStoreMult(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(regist, REGISTER_Y);

    multiplication[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);
    copySourceRegisterToDestRegister(REGISTER_X, regist);
    copySourceRegisterToDestRegister(SAVED_REGISTER_X, REGISTER_X);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnStoreMult:", errorMessage, "is not defined!", NULL);
  }
  #endif
}



/********************************************//**
 * \brief Divides a register by X
 *
 * \param[in] registerNumber uint16_t
 * \return void
 ***********************************************/
void fnStoreDiv(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(regist, REGISTER_Y);

    division[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);
    copySourceRegisterToDestRegister(REGISTER_X, regist);
    copySourceRegisterToDestRegister(SAVED_REGISTER_X, REGISTER_X);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
  }
  #ifdef PC_BUILD
    else {
      sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
      showInfoDialog("In function fnStoreDiv:", errorMessage, "is not defined!", NULL);
    }
  #endif
}



/********************************************//**
 * \brief Keeps in a register min(X, register)
 *
 * \param[in] registerNumber uint16_t
 * \return void
 ***********************************************/
void fnStoreMin(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters) {
    registerMin(REGISTER_X, regist, regist);

    if(REGISTER_X <= regist && regist < REGISTER_X + displayStack) {
      refreshRegisterLine(regist);
    }
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnStoreMin:", errorMessage, "is not defined!", NULL);
  }
  #endif
}



/********************************************//**
 * \brief Keeps in a register max(X, register)
 *
 * \param[in] registerNumber uint16_t
 * \return void
 ***********************************************/
void fnStoreMax(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters) {
    registerMax(REGISTER_X, regist, regist);

    if(REGISTER_X <= regist && regist < REGISTER_X + displayStack) {
      refreshRegisterLine(regist);
    }
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnStoreMax:", errorMessage, "is not defined!", NULL);
  }
  #endif
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
  storeToDtConfigDescriptor(curveFitting);
  storeToDtConfigDescriptor(roundingMode);
  storeToDtConfigDescriptor(systemFlags);
  xcopy(configToStore->kbd_usr, kbd_usr, sizeof(kbd_usr));

//    storeToDtConfigDescriptor(Norm_Key_00_VAR);                          //JMCFG
  storeToDtConfigDescriptor(SigFigMode);
  storeToDtConfigDescriptor(eRPN);
  storeToDtConfigDescriptor(HOME3);
  storeToDtConfigDescriptor(ShiftTimoutMode);
  storeToDtConfigDescriptor(Home3TimerMode);
  storeToDtConfigDescriptor(UNITDisplay);
  storeToDtConfigDescriptor(SH_BASE_HOME);
  storeToDtConfigDescriptor(SH_BASE_AHOME);
  storeToDtConfigDescriptor(Norm_Key_00_VAR);
  storeToDtConfigDescriptor(Input_Default);
  storeToDtConfigDescriptor(jm_FG_LINE);
  storeToDtConfigDescriptor(jm_FG_DOTS);
  storeToDtConfigDescriptor(jm_G_DOUBLETAP);
  storeToDtConfigDescriptor(graph_xmin);
  storeToDtConfigDescriptor(graph_xmax);
  storeToDtConfigDescriptor(graph_ymin);
  storeToDtConfigDescriptor(graph_ymax);
  storeToDtConfigDescriptor(graph_dx);
  storeToDtConfigDescriptor(graph_dy);
  storeToDtConfigDescriptor(jm_VECT);
  storeToDtConfigDescriptor(jm_HOME_SUM);
  storeToDtConfigDescriptor(jm_HOME_MIR);
  storeToDtConfigDescriptor(jm_HOME_FIX);                                    //JMCFG^^


  if(REGISTER_X <= regist && regist < REGISTER_X + displayStack) {
    refreshRegisterLine(regist);
  }
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
      showInfoDialog("In function fnStoreStack:", errorMessage, NULL, NULL);
    #endif
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
void fnStoreElement(uint16_t unusedParamButMandatory) {
#ifdef PC_BUILD
printf("fnStoreElement\n");
#endif
  displayCalcErrorMessage(ERROR_ITEM_TO_BE_CODED, ERR_REGISTER_LINE, REGISTER_X);
  #ifdef PC_BUILD
    showInfoDialog("In function fnStoreElement:", "To be coded", NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Stores X and Y in the indexes I and J
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnStoreIJ(uint16_t unusedParamButMandatory) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_I);
  copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_J);
}



/********************************************//**
 * \brief Recalls a register in X
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecall(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters) {
    if(REGISTER_X <= regist && regist <= getStackTop()) {
      copySourceRegisterToDestRegister(regist, TEMP_REGISTER);
      liftStack();
      copySourceRegisterToDestRegister(TEMP_REGISTER, REGISTER_X);
    }
    else {
      liftStack();
      copySourceRegisterToDestRegister(regist, REGISTER_X);
    }
    refreshStack();
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnRecall:", errorMessage, "is not defined!", NULL);
  }
  #endif
}



/********************************************//**
 * \brief Recalls register L in X
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnLastX(uint16_t unusedParamButMandatory) {
  fnRecall(REGISTER_L);
}



/********************************************//**
 * \brief Adds a register to X
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallAdd(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_Y);
    copySourceRegisterToDestRegister(regist, REGISTER_X);

    addition[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnRecallPlus:", errorMessage, "is not defined!", NULL);
  }
  #endif
}



/********************************************//**
 * \brief Subtracts a register from X
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallSub(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_Y);
    copySourceRegisterToDestRegister(regist, REGISTER_X);

    subtraction[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnRecallSub:", errorMessage, "is not defined!", NULL);
  }
  #endif
}



/********************************************//**
 * \brief Multiplies X by a register
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallMult(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_Y);
    copySourceRegisterToDestRegister(regist, REGISTER_X);

    multiplication[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnRecallMult:", errorMessage, "is not defined!", NULL);
  }
  #endif
}



/********************************************//**
 * \brief Divides X by a register
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallDiv(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters) {
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_Y);
    copySourceRegisterToDestRegister(regist, REGISTER_X);

    division[getRegisterDataType(REGISTER_X)][getRegisterDataType(REGISTER_Y)]();

    copySourceRegisterToDestRegister(SAVED_REGISTER_Y, REGISTER_Y);

    adjustResult(REGISTER_X, false, true, REGISTER_X, regist, -1);
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnRecallDiv:", errorMessage, "is not defined!", NULL);
  }
  #endif
}



/********************************************//**
 * \brief Keeps in X min(X, register)
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallMin(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    registerMin(REGISTER_X, regist, REGISTER_X);
    refreshRegisterLine(REGISTER_X);
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnRecallMin:", errorMessage, "is not defined!", NULL);
  }
  #endif
}



/********************************************//**
 * \brief Keeps in X max(X, register)
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallMax(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters) {
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
    registerMax(REGISTER_X, regist, REGISTER_X);
    refreshRegisterLine(REGISTER_X);
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnReallMax:", errorMessage, "is not defined!", NULL);
  }
  #endif
}



/********************************************//**
 * \brief Recalls a configuration
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallConfig(uint16_t r) {
  if (getRegisterDataType(r) == dtConfig) {
    dtConfigDescriptor_t *configToRecall = REGISTER_CONFIG_DATA(r);

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
    recallFromDtConfigDescriptor(curveFitting);
    recallFromDtConfigDescriptor(roundingMode);

    setSystemFlagToRecalled(FLAG_TDM24);
    setSystemFlagToRecalled(FLAG_DMY);
    setSystemFlagToRecalled(FLAG_MDY);
    setSystemFlagToRecalled(FLAG_YMD);
    setSystemFlagToRecalled(FLAG_CPXRES);
    setSystemFlagToRecalled(FLAG_POLAR);
    setSystemFlagToRecalled(FLAG_FRACT);
    setSystemFlagToRecalled(FLAG_PROPFR);
    setSystemFlagToRecalled(FLAG_DENANY);
    setSystemFlagToRecalled(FLAG_DENFIX);
    setSystemFlagToRecalled(FLAG_LEAD0);
    setSystemFlagToRecalled(FLAG_TRACE);
    setSystemFlagToRecalled(FLAG_USER);
    setSystemFlagToRecalled(FLAG_SLOW);
    setSystemFlagToRecalled(FLAG_SPCRES);
    setSystemFlagToRecalled(FLAG_SSIZE8);
    setSystemFlagToRecalled(FLAG_QUIET);
    setSystemFlagToRecalled(FLAG_DECIMP);
    setSystemFlagToRecalled(FLAG_ALLENG);
    setSystemFlagToRecalled(FLAG_GROW);
    setSystemFlagToRecalled(FLAG_AUTOFF);
    setSystemFlagToRecalled(FLAG_AUTXEQ);
    setSystemFlagToRecalled(FLAG_IGN1ER);

    //    setSystemFlagToRecalled(Norm_Key_00_VAR);                            //JMCFG vv
    recallFromDtConfigDescriptor(SigFigMode);      
    recallFromDtConfigDescriptor(eRPN);             
    recallFromDtConfigDescriptor(HOME3);            
    recallFromDtConfigDescriptor(ShiftTimoutMode);  
    recallFromDtConfigDescriptor(Home3TimerMode);   
    recallFromDtConfigDescriptor(UNITDisplay);      
    recallFromDtConfigDescriptor(SH_BASE_HOME);     
    recallFromDtConfigDescriptor(SH_BASE_AHOME);    
    recallFromDtConfigDescriptor(Norm_Key_00_VAR); 
    recallFromDtConfigDescriptor(Input_Default);   
    recallFromDtConfigDescriptor(jm_FG_LINE);       
    recallFromDtConfigDescriptor(jm_FG_DOTS);       
    recallFromDtConfigDescriptor(jm_G_DOUBLETAP);   
    recallFromDtConfigDescriptor(graph_xmin);        
    recallFromDtConfigDescriptor(graph_xmax);        
    recallFromDtConfigDescriptor(graph_ymin);        
    recallFromDtConfigDescriptor(graph_ymax);        
    recallFromDtConfigDescriptor(graph_dx);          
    recallFromDtConfigDescriptor(graph_dy);          
    recallFromDtConfigDescriptor(jm_VECT);          
    recallFromDtConfigDescriptor(jm_HOME_SUM);      
    recallFromDtConfigDescriptor(jm_HOME_MIR);      
    recallFromDtConfigDescriptor(jm_HOME_FIX);                                 //JMCFG^^


    refreshStack();
    showAngularMode();
    showIntegerMode();
  }

  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "data type %s cannot be used to recall a configuration!", getRegisterDataTypeName(r, false, false));
        showInfoDialog("In function fnRecallConfig:", errorMessage, NULL, NULL);
      #endif
  }
}



/********************************************//**
 * \brief Recalls a stack
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallStack(uint16_t r) {
  uint16_t size = getSystemFlag(FLAG_SSIZE8) ? 8 : 4;

  if(r + size >= REGISTER_X) {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Cannot execute RCLS, destination register is out of range: %d", r);
      showInfoDialog("In function fnRecallStack:", errorMessage, NULL, NULL);
    #endif
  }
  else {
    int i;

    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

    for(i=0; i<size; i++) {
      copySourceRegisterToDestRegister(r + i, REGISTER_X + i);
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
void fnRecallElement(uint16_t unusedParamButMandatory) {
#ifdef PC_BUILD
printf("fnRecallElement\n");
#endif
  displayCalcErrorMessage(ERROR_ITEM_TO_BE_CODED, ERR_REGISTER_LINE, REGISTER_X);
  #ifdef PC_BUILD
    showInfoDialog("In function fnRecallElement:", "To be coded", NULL, NULL);
  #endif
}



/********************************************//**
 * \brief Recalls the indexes I and J in X and Y
 *
 * \param[in] regist uint16_t
 * \return void
 ***********************************************/
void fnRecallIJ(uint16_t unusedParamButMandatory) {
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  copySourceRegisterToDestRegister(REGISTER_I, REGISTER_X);
  copySourceRegisterToDestRegister(REGISTER_J, REGISTER_Y);

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
  adjustResult(REGISTER_Y, false, true, REGISTER_Y, -1, -1);
}



/********************************************//**
 * \brief Tests if X is less than an other register
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnXLessThan(uint16_t unusedParamButMandatory) {
#ifdef PC_BUILD
printf("fnXLessThan\n");
#endif
  displayCalcErrorMessage(ERROR_ITEM_TO_BE_CODED, ERR_REGISTER_LINE, REGISTER_X);
  #ifdef PC_BUILD
    showInfoDialog("In function fnXLessThan:", "To be coded", NULL, NULL);
  #endif
}


/********************************************//**
 * \brief returns the integer part of the value of a register
 *
 * \param regist calcRegister_t Register
 * \return void
 ***********************************************/
int16_t indirectAddressing(calcRegister_t regist, int16_t minValue, int16_t maxValue) {
  int16_t value;

  if(regist >= FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters) {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #ifdef PC_BUILD
      sprintf(errorMessage, "local indirection register .%02d", regist - FIRST_LOCAL_REGISTER);
      showInfoDialog("In function indirectAddressing:", errorMessage, "is not defined!", NULL);
    #endif
    return 9999;
  }

  else if(getRegisterDataType(regist) == dtReal34) {
    real34_t maxValue34;

    int32ToReal34(maxValue, &maxValue34);
    if(real34CompareLessThan(REGISTER_REAL34_DATA(regist), const34_0) || real34CompareGreaterEqual(REGISTER_REAL34_DATA(regist), &maxValue34)) {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      #ifdef PC_BUILD
        real34ToString(REGISTER_REAL34_DATA(regist), errorMessage);
        sprintf(tmpStr3000, "register %" FMT16S " = %s:", regist, errorMessage);
        showInfoDialog("In function indirectAddressing:", tmpStr3000, "this value is negative or too big!", NULL);
      #endif
      return 9999;
    }
    value = real34ToInt32(REGISTER_REAL34_DATA(regist));
  }

  else if(getRegisterDataType(regist) == dtLongInteger) {
    longInteger_t lgInt;

    convertLongIntegerRegisterToLongInteger(regist, lgInt);
    if(longIntegerIsNegative(lgInt) || longIntegerCompareUInt(lgInt, maxValue) >= 0) {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      #ifdef PC_BUILD
        longIntegerToAllocatedString(lgInt, errorMessage, sizeof(errorMessage));
        sprintf(tmpStr3000, "register %" FMT16S " = %s:", regist, errorMessage);
        showInfoDialog("In function indirectAddressing:", tmpStr3000, "this value is negative or too big!", NULL);
      #endif
      longIntegerFree(lgInt);
      return 9999;
    }
    longIntegerToUInt(lgInt, value);
    longIntegerFree(lgInt);
  }

  else if(getRegisterDataType(regist) == dtShortInteger) {
    uint64_t val;
    int16_t sign;

    convertShortIntegerRegisterToUInt64(regist, &sign, &val);
    if(sign == 1 || val > 180) {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      #ifdef PC_BUILD
        shortIntegerToDisplayString(regist, errorMessage, false);
        sprintf(tmpStr3000, "register %" FMT16S " = %s:", regist, errorMessage);
        showInfoDialog("In function indirectAddressing:", tmpStr3000, "this value is negative or too big!", NULL);
      #endif
      return 9999;
    }
    value = val;
  }

  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #ifdef PC_BUILD
      sprintf(errorMessage, "register %" FMT16S " is %s:", regist, getRegisterDataTypeName(regist, true, false));
      showInfoDialog("In function indirectAddressing:", errorMessage, "not suited for indirect addressing!", NULL);
    #endif
    return 9999;
  }

  if(minValue <= value && value <= maxValue) {
    return value;
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #ifdef PC_BUILD
      sprintf(errorMessage, "value = %d! Should be from %d to %d.", value, minValue, maxValue);
      showInfoDialog("In function indirectAddressing:", errorMessage, NULL, NULL);
    #endif
    return 9999;
  }
}



#ifndef DMCP_BUILD
/********************************************//**
 * \brief Prints the content of a register to the console
 *
 * \param r calcRegister_t Register number
 * \return void
 ***********************************************/
void printRegisterToConsole(calcRegister_t regist, const char *before, const char *after) {
  char str[TMP_STR_LENGTH];     //JM updated from 3000

  printf("%s", before);

  if(getRegisterDataType(regist) == dtReal34) {
    real34ToString(REGISTER_REAL34_DATA(regist), str);
    printf("real34 %s %s", str, getAngularModeName(getRegisterAngularMode(regist)));
  }

  else if(getRegisterDataType(regist) == dtComplex34) {
    real34ToString(REGISTER_REAL34_DATA(regist), str);
    printf("complex34 %s ", str);

    real34ToString(REGISTER_IMAG34_DATA(regist), str);
    if(real34IsNegative(REGISTER_IMAG34_DATA(regist))) {
      printf("- ix%s", str + 1);
    }
    else {
      printf("+ ix%s", str);
    }
  }

  else if(getRegisterDataType(regist) == dtString) {
    stringToUtf8(REGISTER_STRING_DATA(regist), (uint8_t *)str);
    printf("string (%" FMTSIZE " + %" FMT32U " bytes) |%s|", sizeof(dataBlock_t), TO_BYTES(getRegisterMaxDataLength(regist)), str);
  }

  else if(getRegisterDataType(regist) == dtShortInteger) {
    uint64_t value = *(REGISTER_SHORT_INTEGER_DATA(regist));

    printf("short integer %08x-%08x (base %" FMT32U ")", (unsigned int)(value>>32), (unsigned int)(value&0xffffffff), getRegisterTag(regist));
  }

  else if(getRegisterDataType(regist) == dtConfig) {
    printf("Configuration data");
  }

  else if(getRegisterDataType(regist) == dtLongInteger) {
    longInteger_t lgInt;

    convertLongIntegerRegisterToLongInteger(regist, lgInt);
    longIntegerToAllocatedString(lgInt, str, sizeof(str));
    longIntegerFree(lgInt);
    printf("long integer (%" FMTSIZE " + %" FMT32U " bytes) %s", sizeof(dataBlock_t), TO_BYTES(getRegisterMaxDataLength(regist)), str);
  }

  else {
    sprintf(errorMessage, "In printRegisterToConsole: data type %s not supported", getRegisterDataTypeName(regist ,false, false));
    displayBugScreen(errorMessage);
  }

  printf("%s", after);
}
#endif



#ifdef TESTSUITE_BUILD
/********************************************//**
 * \brief Prints the content of a register to a string
 *
 * \param r calcRegister_t Register number
 * \return void
 ***********************************************/
void printRegisterToString(calcRegister_t regist, char *registerContent) {
  char str[1000];

  if(getRegisterDataType(regist) == dtReal34) {
    real34ToString(REGISTER_REAL34_DATA(regist), str);
    sprintf(registerContent, "real34 %s %s", str, getAngularModeName(getRegisterAngularMode(regist)));
  }

  else if(getRegisterDataType(regist) == dtComplex34) {
    real34ToString(REGISTER_REAL34_DATA(regist), str);
    sprintf(registerContent, "complex34 %s ", str);

    real34ToString(REGISTER_IMAG34_DATA(regist), str);
    if(real34IsNegative(REGISTER_IMAG34_DATA(regist))) {
      strcat(registerContent, "- ix");
      strcat(registerContent, str + 1);
    }
    else {
      strcat(registerContent, "+ ix");
      strcat(registerContent, str);
    }
  }

  else if(getRegisterDataType(regist) == dtString) {
    stringToUtf8(REGISTER_STRING_DATA(regist), (uint8_t *)str);
    sprintf(registerContent, "string (%" FMT32U " bytes) |%s|", TO_BYTES(getRegisterMaxDataLength(regist)), str);
  }

  else if(getRegisterDataType(regist) == dtShortInteger) {
    uint64_t value = *(REGISTER_SHORT_INTEGER_DATA(regist));

    sprintf(registerContent, "short integer %08x-%08x (base %u)", (unsigned int)(value>>32), (unsigned int)(value&0xffffffff), getRegisterTag(regist));
  }

  else if(getRegisterDataType(regist) == dtConfig) {
    strcpy(registerContent, "Configuration data");
  }

  else if(getRegisterDataType(regist) == dtLongInteger) {
    longInteger_t lgInt;
    char lgIntStr[TMP_STR_LENGTH];                       //JM updated from 3000

    convertLongIntegerRegisterToLongInteger(regist, lgInt);
    longIntegerToAllocatedString(lgInt, lgIntStr, sizeof(lgIntStr));
    longIntegerFree(lgInt);
    sprintf(registerContent, "long integer (%" FMT32U " bytes) %s", TO_BYTES(getRegisterMaxDataLength(regist)), lgIntStr);
  }

  else {
    sprintf(registerContent, "In printRegisterToString: data type %s not supported", getRegisterDataTypeName(regist ,false, false));
  }
}
#endif // TESTSUITE_BUILD



void printReal34ToConsole(const real34_t *value, const char *before, const char *after) {
  char str[100];

  real34ToString(value, str);
  printf("%sreal34 %s%s", before, str, after);
}



void printRealToConsole(const real_t *value, const char *before, const char *after) {
  char str[1000];

  realToString(value, str);
  printf("%sreal%" FMT32S " %s%s", before, value->digits, str, after);

/*  int32_t i, exponent, last;

  if(realIsNaN(value)) {
    printf("NaN");
    return;
  }

  if(realIsNegative(value)) {
    printf("-");
  }

  if(realIsInfinite(value)) {
    printf("infinite");
    return;
  }

  if(realIsZero(value)) {
    printf("0");
    return;
  }

  if(value->digits % DECDPUN) {
    i = value->digits/DECDPUN;
  }
  else {
    i = value->digits/DECDPUN - 1;
  }

  while(value->lsu[i] == 0) i--;
  printf("%" FMT16U, value->lsu[i--]);

  exponent = value->exponent;
  last = 0;
  while(exponent <= -DECDPUN && value->lsu[last] == 0) {
    last++;
    exponent += DECDPUN;
  }

  for(; i>=last; i--) {
    printf(" %03" FMT16U, value->lsu[i]);
  }

  if(exponent != 0) {
    printf(" e %" FMT32S, exponent);
  }*/
}



void printComplex34ToConsole(const complex34_t *value, const char *before, const char *after) {
  char str[100];

  real34ToString((real34_t *)value, str);
  printf("%scomplex34 %s + ", before, str);
  real34ToString((real34_t *)value + 1, str);
  printf("%si%s", str, after);
}



void printRegisterDescriptorToConsole(calcRegister_t regist) {
  registerDescriptor_t descriptor;

  descriptor.descriptor = 0xFFFFFFFF;

  if(regist < FIRST_LOCAL_REGISTER) { // Global register
    descriptor = reg[regist];
  }
  else if(regist < 1000) { // Local register
    if(allLocalRegisterPointer->numberOfLocalRegisters > 0) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < allLocalRegisterPointer->numberOfLocalRegisters) {
        descriptor = *POINTER_TO_LOCAL_REGISTER(regist);
      }
    }
  }
  else if(regist < SAVED_REGISTER_X) { // Named variable
    if(allNamedVariablePointer->numberOfNamedVariables > 0) {
      regist -= 1000u;
      if(regist < allNamedVariablePointer->numberOfNamedVariables) {
        descriptor = *POINTER_TO_NAMED_VARIABLE(regist);
      }
    }
  }
  else if(regist < 3000) { // Saved stack register
    if(regist <= LAST_SAVED_REGISTER) {
      descriptor = savedStackRegister[regist - SAVED_REGISTER_X];
    }
  }

  printf("Header informations of register %d\n", regist);
  printf("    reg ptr   = %u\n", descriptor.dataPointer);
  printf("    data type = %u = %s\n", descriptor.dataType, getDataTypeName(descriptor.dataType, false, false));
  if(descriptor.dataType == dtLongInteger || descriptor.dataType == dtString) {
    printf("    data ptr  = %u\n", descriptor.dataPointer + 1);
    printf("    data size = %" FMT32U "\n", *(uint32_t *)TO_PCMEMPTR(reg[regist].dataPointer));
  }
  printf("    tag       = %u\n", descriptor.tag);
}



/********************************************//**
 * \brief Prints the content of a long integer to the console
 *
 * \param r int16_t Register number
 * \return void
 ***********************************************/
void printLongIntegerToConsole(const longInteger_t value, const char *before, const char *after) {
  char str[TMP_STR_LENGTH];                            //JM updated from 3000

  longIntegerToAllocatedString(value, str, sizeof(str));
  printf("%slong integer (%" FMTSIZE " + %" FMT64U " <%" FMT64U " reserved> bytes) %s%s", before, sizeof(value->_mp_size) + sizeof(value->_mp_d) + sizeof(value->_mp_alloc), (uint64_t)longIntegerSizeInBytes(value), (uint64_t)(value->_mp_alloc * LIMB_SIZE), str, after);
}



void reallocateRegister(calcRegister_t regist, uint32_t dataType, uint16_t dataSizeWithoutDataLenBlocks, uint32_t tag) { // dataSize without data length in blocks, this includes the trailing 0 for strings
  uint16_t dataSizeWithDataLenBlocks = dataSizeWithoutDataLenBlocks;

  //printf("reallocateRegister: %d to %s tag=%u (%u bytes excluding maxSize) begin\n", regist, getDataTypeName(dataType, false, false), tag, dataSizeWithoutDataLenBlocks);
  if(dataType == dtReal34 && dataSizeWithoutDataLenBlocks != REAL34_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" FMT16U " is an unexpected numByte value for a real34 or an angle34! It should be REAL34_SIZE=%" FMT16U "!", dataSizeWithoutDataLenBlocks, (uint16_t)REAL34_SIZE);
    displayBugScreen(errorMessage);
  }
  else if(dataType == dtComplex34 && dataSizeWithoutDataLenBlocks != COMPLEX34_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" FMT16U " is an unexpected numByte value for a complex34! It should be COMPLEX34_SIZE=%" FMT16U "!", dataSizeWithoutDataLenBlocks, (uint16_t)COMPLEX34_SIZE);
    displayBugScreen(errorMessage);
  }
  else if(dataType == dtShortInteger && dataSizeWithoutDataLenBlocks != SHORT_INTEGER_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" FMT16U " is an unexpected numByte value for an integer! It should be SHORT_INTEGER_SIZE=%" FMT16U "!", dataSizeWithoutDataLenBlocks, (uint16_t)SHORT_INTEGER_SIZE);
    displayBugScreen(errorMessage);
  }
  else if(dataType == dtConfig && dataSizeWithoutDataLenBlocks != CONFIG_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" FMT16U " is an unexpected numByte value for a configuration! It should be CONFIG_SIZE=%" FMT16U "!", dataSizeWithoutDataLenBlocks, (uint16_t)CONFIG_SIZE);
    displayBugScreen(errorMessage);
  }
  else if(dataType == dtString) {
    dataSizeWithDataLenBlocks = dataSizeWithoutDataLenBlocks + 1; // +1 for the max length of the string
  }
  else if(dataType == dtLongInteger) {
    if(TO_BYTES(dataSizeWithoutDataLenBlocks) % LIMB_SIZE != 0) {
      dataSizeWithoutDataLenBlocks = ((dataSizeWithoutDataLenBlocks / TO_BLOCKS(LIMB_SIZE)) + 1) * TO_BLOCKS(LIMB_SIZE);
    }
    dataSizeWithDataLenBlocks = dataSizeWithoutDataLenBlocks + 1; // +1 for the max length of the data
  }

  if(getRegisterDataType(regist) != dataType || ((getRegisterDataType(regist) == dtString || getRegisterDataType(regist) == dtLongInteger) && getRegisterMaxDataLength(regist) != dataSizeWithoutDataLenBlocks)) {
    freeRegisterData(regist);
    setRegisterDataPointer(regist, allocWp43s(TO_BYTES(dataSizeWithDataLenBlocks)));
    setRegisterDataType(regist, dataType, tag);
    setRegisterMaxDataLength(regist, dataSizeWithDataLenBlocks - (dataType == dtString || dataType == dtLongInteger ? 1 : 0));
  }
  else {
    setRegisterTag(regist, tag);
  }
//sprintf(tmpStr3000, "reallocateRegister %d to %s tag=%u (%u bytes including dataLen) done", regist, getDataTypeName(dataType, false, false), tag, dataSizeWithDataLenBlocks);
//memoryDump(tmpStr3000);
}



void fnToReal(uint16_t unusedParamButMandatory) {
  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger :
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      break;

    case dtShortInteger :
      convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      break;

    case dtReal34:
      if(getRegisterAngularMode(REGISTER_X) != AM_NONE) {
        if(getRegisterAngularMode(REGISTER_X) == AM_DMS) {
          convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DMS, AM_DEGREE);
        }
        setRegisterAngularMode(REGISTER_X, AM_NONE);
      }
      break;

    default :
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "data type %s cannot be converted to a real34!", getRegisterDataTypeName(REGISTER_X, false, false));
        showInfoDialog("In function fnToReal:", errorMessage, NULL, NULL);
      #endif
      return;
  }

  refreshRegisterLine(REGISTER_X);
}

//=============================================================================
// Inc and Dec functions
//-----------------------------------------------------------------------------

#define INC_FLAG    0
#define DEC_FLAG    1

static void incError(uint16_t regist, uint8_t flag) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, regist);

  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "Cannot increment/decrement, incompatible type.");
    showInfoDialog("In function inc/dec:", errorMessage, NULL, NULL);
  #endif
}

static void incLonI(uint16_t regist, uint8_t flag) {
  longInteger_t r;

  convertLongIntegerRegisterToLongInteger(regist, r);

  (flag==INC_FLAG) ? longIntegerAddUInt(r, 1, r) : longIntegerSubtractUInt(r, 1, r);

  convertLongIntegerToLongIntegerRegister(r, regist);

  longIntegerFree(r);
}

static void incReal(uint16_t regist, uint8_t flag) {
  real_t r;

  real34ToReal(REGISTER_REAL34_DATA(regist), &r);
  (flag==INC_FLAG) ? realAdd(&r, const_1, &r, &ctxtReal39) : realSubtract(&r, const_1, &r, &ctxtReal39);

  realToReal34(&r, REGISTER_REAL34_DATA(regist));
}

static void incCplx(uint16_t regist, uint8_t flag) {
  real_t r_real;

  real34ToReal(REGISTER_REAL34_DATA(regist), &r_real);

  (flag==INC_FLAG) ? realAdd(&r_real, const_1, &r_real, &ctxtReal39) : realSubtract(&r_real, const_1, &r_real, &ctxtReal39);

  realToReal34(&r_real, REGISTER_REAL34_DATA(regist));
}

static void incShoI(uint16_t regist, uint8_t flag) {
  int16_t r_sign;
  uint64_t r_value;

  convertShortIntegerRegisterToUInt64(regist, &r_sign, &r_value);

  if(r_sign)  // regiter regist negative
    (flag!=INC_FLAG) ? r_value++ : r_value--;
  else        // register regist positive
    (flag==INC_FLAG) ? r_value++ : r_value--;

  convertUInt64ToShortIntegerRegister(r_sign, r_value, getRegisterTag(regist), regist);
}

static void (* const increment[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(uint16_t, uint8_t) = {
// reg ==>   1            2        3         4         5         6         7          8            9              10
//           Long integer Real34   Complex34 Time      Date      String    Real34 mat Complex34 m  Short integer  Config data
             incLonI,     incReal, incCplx,  incError, incError, incError, incError,  incError,    incShoI,       incError
};



/********************************************//**
 * \brief Decrement a register
 *
 * \param[in] registerNumber uint16_t
 * \return void
 ***********************************************/
void fnDec(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters) {
    increment[getRegisterDataType(regist)](regist, DEC_FLAG);

    if(REGISTER_X <= regist && regist <= REGISTER_T) {
      refreshRegisterLine(regist);
    }
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnDec:", errorMessage, "is not defined!", NULL);
  }
  #endif
}

/********************************************//**
 * \brief Increment a register
 *
 * \param[in] registerNumber uint16_t
 * \return void
 ***********************************************/
void fnInc(uint16_t regist) {
  if(regist < FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters) {
    increment[getRegisterDataType(regist)](regist, INC_FLAG);

    if(REGISTER_X <= regist && regist <= REGISTER_T) {
      refreshRegisterLine(regist);
    }
  }
  #ifdef PC_BUILD
  else {
    sprintf(errorMessage, "local register .%02u", regist - FIRST_LOCAL_REGISTER);
    showInfoDialog("In function fnInc:", errorMessage, "is not defined!", NULL);
  }
  #endif
}

//=============================================================================
// Register Min/Max functions
//-----------------------------------------------------------------------------

static void registerCmpLonILonI(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Long Integer, regist2 = Long Integer

  longInteger_t r1, r2;

  convertLongIntegerRegisterToLongInteger(regist1, r1);
  convertLongIntegerRegisterToLongInteger(regist2, r2);

  int8_t value = longIntegerCompare(r1, r2);

  longIntegerFree(r1);
  longIntegerFree(r2);

  *result = value;
}

static void registerCmpLonIShoI(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Long Integer, regist2 = Short Integer

  longInteger_t r1, r2;

  convertLongIntegerRegisterToLongInteger(regist1, r1);
  convertShortIntegerRegisterToLongInteger(regist2, r2);

  int8_t value = longIntegerCompare(r1, r2);

  longIntegerFree(r1);
  longIntegerFree(r2);

  *result = value;
}

static void registerCmpLonIReal(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Long Integer, regist2 = Real

  real_t r1, r2;

  convertLongIntegerRegisterToReal(regist1, &r1, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &r2);

  if(realCompareEqual(&r1, &r2))
    *result = 0;
  else
    *result = realCompareGreaterThan(&r1, &r2) ? 1 : -1;
}

static void registerCmpTimeTime(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  fnToBeCoded();
}

static void registerCmpDateDate(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  fnToBeCoded();
}

static void registerCmpStriStri(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
 *result = compareString(REGISTER_STRING_DATA(regist1), REGISTER_STRING_DATA(regist2), CMP_EXTENSIVE);
}

static void registerCmpShoILonI(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Short Integer, regist2 = Long Integer

  longInteger_t r1, r2;

  convertShortIntegerRegisterToLongInteger(regist1, r1);
  convertLongIntegerRegisterToLongInteger(regist2, r2);

  int8_t value = longIntegerCompare(r1, r2);

  longIntegerFree(r1);
  longIntegerFree(r2);

  *result = value;
}

static void registerCmpShoIShoI(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Short Integer, regist2 = Short Integer

  longInteger_t r1, r2;

  convertShortIntegerRegisterToLongInteger(regist1, r1);
  convertShortIntegerRegisterToLongInteger(regist2, r2);

  int8_t value = longIntegerCompare(r1, r2);

  longIntegerFree(r1);
  longIntegerFree(r2);

  *result = value;
}

static void registerCmpShoIReal(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Short Integer, regist2 = Real

  real_t r1, r2;

  convertShortIntegerRegisterToReal(regist1, &r1, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(regist2), &r2);

  if(realCompareEqual(&r1, &r2))
    *result = 0;
  else
    *result = realCompareGreaterThan(&r1, &r2) ? 1 : -1;
}

static void registerCmpRealLonI(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Real, regist2 = Long Integer

  real_t r1, r2;

  real34ToReal(REGISTER_REAL34_DATA(regist1), &r1);
  convertLongIntegerRegisterToReal(regist2, &r2, &ctxtReal39);

  if(realCompareEqual(&r1, &r2))
    *result = 0;
  else
    *result = realCompareGreaterThan(&r1, &r2) ? 1 : -1;
}

static void registerCmpRealShoI(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Real, regist2 = Short Integer

  real_t r1, r2;

  real34ToReal(REGISTER_REAL34_DATA(regist1), &r1);
  convertShortIntegerRegisterToReal(regist2, &r2, &ctxtReal39);

  if(realCompareEqual(&r1, &r2))
    *result = 0;
  else
    *result = realCompareGreaterThan(&r1, &r2) ? 1 : -1;
}

static void registerCmpRealReal(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Real, regist2 = Real

  real_t r1, r2;

  real34ToReal(REGISTER_REAL34_DATA(regist1), &r1);
  real34ToReal(REGISTER_REAL34_DATA(regist2), &r2);

  if(realCompareEqual(&r1, &r2))
    *result = 0;
  else
    *result = realCompareGreaterThan(&r1, &r2) ? 1 : -1;
}

static void (* const cmpFunc[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(calcRegister_t reg1, calcRegister_t reg2, int8_t*) = {
// reg1 |    reg2 ==>    1                    2                    3          4                    5                    6                    7              8               9                    10
//      V                Long integer         Real34               Complex34  Time                 Date                 String               Real34 mat     Complex34 mat   Short integer        Config data
/*  1 Long integer  */ { registerCmpLonILonI, registerCmpLonIReal, NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           registerCmpLonIShoI, NULL},
/*  2 Real34        */ { registerCmpRealLonI, registerCmpRealReal, NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           registerCmpRealShoI, NULL},
/*  3 Complex34     */ { NULL,                NULL,                NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           NULL,                NULL},
/*  4 Time          */ { NULL,                NULL,                NULL,      registerCmpTimeTime, NULL,                NULL,                NULL,          NULL,           NULL,                NULL},
/*  5 Date          */ { NULL,                NULL,                NULL,      NULL,                registerCmpDateDate, NULL,                NULL,          NULL,           NULL,                NULL},
/*  6 String        */ { NULL,                NULL,                NULL,      NULL,                NULL,                registerCmpStriStri, NULL,          NULL,           NULL,                NULL},
/*  7 Real34 mat    */ { NULL,                NULL,                NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           NULL,                NULL},
/*  8 Complex34 mat */ { NULL,                NULL,                NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           NULL,                NULL},
/*  9 Short integer */ { registerCmpShoILonI, registerCmpShoIReal, NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           registerCmpShoIShoI, NULL},
/* 10 Config data   */ { NULL,                NULL,                NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           NULL,                NULL}
};

void registerCmpError(calcRegister_t regist1, calcRegister_t regist2) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot get compare: %s", getRegisterDataTypeName(regist1, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "and %s", getRegisterDataTypeName(regist2, true, false));
    showInfoDialog("In function registerCmp:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif
}

bool_t registerCmp(calcRegister_t reg1, calcRegister_t reg2, int8_t *result) {
  void (*func)(calcRegister_t, calcRegister_t, int8_t*) = cmpFunc[getRegisterDataType(reg1)][getRegisterDataType(reg2)];

  if(func == NULL)
    return false;

  func(reg1, reg2, result);

  return true;
}

void registerMax(calcRegister_t reg1, calcRegister_t reg2, calcRegister_t dest) {
  int8_t result = 0;

  if(!registerCmp(reg1, reg2, &result))
    registerCmpError(reg1, reg2);
  else if(result != 0)
    copySourceRegisterToDestRegister(result>0 ? reg1 : reg2, dest);
}

void registerMin(calcRegister_t reg1, calcRegister_t reg2, calcRegister_t dest) {
  int8_t result = 0;

  if(!registerCmp(reg1, reg2, &result))
    registerCmpError(reg1, reg2);
  else if(result != 0)
    copySourceRegisterToDestRegister(result>0 ? reg2 : reg1, dest);
}
