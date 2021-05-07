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

#include "registers.h"

#include "charString.h"
#include "config.h"
#include "constantPointers.h"
#include "dateTime.h"
#include "debug.h"
#include "display.h"
#include "error.h"
#include "flags.h"
#include "items.h"
#include "mathematics/comparisonReals.h"
#include "memory.h"
#include "registerValueConversions.h"
#include "sort.h"
#include "stack.h"
#include <string.h>

#include "wp43s.h"

const reservedVariableHeader_t allReservedVariables[] = { // MUST be in the same order as the reserved variables in item.c item 1165 and upwards
/*  0 */  { .header = {.pointerToRegisterData = WP43S_NULL, .dataType = 0,             .tag = 0,           .readOnly = 0, .notUsed = 0}, .reservedVariableName = {1, 'X',  0,   0,   0,   0,   0,   0} },
/*  1 */  { .header = {.pointerToRegisterData = WP43S_NULL, .dataType = 0,             .tag = 0,           .readOnly = 0, .notUsed = 0}, .reservedVariableName = {1, 'Y',  0,   0,   0,   0,   0,   0} },
/*  2 */  { .header = {.pointerToRegisterData = WP43S_NULL, .dataType = 0,             .tag = 0,           .readOnly = 0, .notUsed = 0}, .reservedVariableName = {1, 'Z',  0,   0,   0,   0,   0,   0} },
/*  3 */  { .header = {.pointerToRegisterData = WP43S_NULL, .dataType = 0,             .tag = 0,           .readOnly = 0, .notUsed = 0}, .reservedVariableName = {1, 'T',  0,   0,   0,   0,   0,   0} },
/*  4 */  { .header = {.pointerToRegisterData = WP43S_NULL, .dataType = 0,             .tag = 0,           .readOnly = 0, .notUsed = 0}, .reservedVariableName = {1, 'A',  0,   0,   0,   0,   0,   0} },
/*  5 */  { .header = {.pointerToRegisterData = WP43S_NULL, .dataType = 0,             .tag = 0,           .readOnly = 0, .notUsed = 0}, .reservedVariableName = {1, 'B',  0,   0,   0,   0,   0,   0} },
/*  6 */  { .header = {.pointerToRegisterData = WP43S_NULL, .dataType = 0,             .tag = 0,           .readOnly = 0, .notUsed = 0}, .reservedVariableName = {1, 'C',  0,   0,   0,   0,   0,   0} },
/*  7 */  { .header = {.pointerToRegisterData = WP43S_NULL, .dataType = 0,             .tag = 0,           .readOnly = 0, .notUsed = 0}, .reservedVariableName = {1, 'D',  0,   0,   0,   0,   0,   0} },
/*  8 */  { .header = {.pointerToRegisterData = WP43S_NULL, .dataType = 0,             .tag = 0,           .readOnly = 0, .notUsed = 0}, .reservedVariableName = {1, 'L',  0,   0,   0,   0,   0,   0} },
/*  9 */  { .header = {.pointerToRegisterData = WP43S_NULL, .dataType = 0,             .tag = 0,           .readOnly = 0, .notUsed = 0}, .reservedVariableName = {1, 'I',  0,   0,   0,   0,   0,   0} },
/* 10 */  { .header = {.pointerToRegisterData = WP43S_NULL, .dataType = 0,             .tag = 0,           .readOnly = 0, .notUsed = 0}, .reservedVariableName = {1, 'J',  0,   0,   0,   0,   0,   0} },
/* 11 */  { .header = {.pointerToRegisterData = WP43S_NULL, .dataType = 0,             .tag = 0,           .readOnly = 0, .notUsed = 0}, .reservedVariableName = {1, 'K',  0,   0,   0,   0,   0,   0} },
/* 12 */  { .header = {.pointerToRegisterData = WP43S_NULL, .dataType = dtLongInteger, .tag = LI_POSITIVE, .readOnly = 1, .notUsed = 0}, .reservedVariableName = {3, 'A', 'D', 'M',  0,   0,   0,   0} },
/* 13 */  { .header = {.pointerToRegisterData = WP43S_NULL, .dataType = dtLongInteger, .tag = LI_POSITIVE, .readOnly = 1, .notUsed = 0}, .reservedVariableName = {6, 'D', 'E', 'N', 'M', 'A', 'X',  0} },
/* 14 */  { .header = {.pointerToRegisterData = WP43S_NULL, .dataType = dtLongInteger, .tag = LI_POSITIVE, .readOnly = 1, .notUsed = 0}, .reservedVariableName = {3, 'I', 'S', 'M',  0,   0,   0,   0} },
/* 15 */  { .header = {.pointerToRegisterData = WP43S_NULL, .dataType = dtLongInteger, .tag = LI_POSITIVE, .readOnly = 1, .notUsed = 0}, .reservedVariableName = {6, 'R', 'E', 'A', 'L', 'D', 'F',  0} },
/* 16 */  { .header = {.pointerToRegisterData = WP43S_NULL, .dataType = dtLongInteger, .tag = LI_POSITIVE, .readOnly = 1, .notUsed = 0}, .reservedVariableName = {4, '#', 'D', 'E', 'C',  0,   0,   0} },
/* 17 */  { .header = {.pointerToRegisterData = 0,          .dataType = dtReal34,      .tag = amNone,     .readOnly = 0, .notUsed = 0}, .reservedVariableName = {3, 'A', 'C', 'C',  0,   0,   0,   0} },
/* 18 */  { .header = {.pointerToRegisterData = 4,          .dataType = dtReal34,      .tag = amNone,     .readOnly = 0, .notUsed = 0}, .reservedVariableName = {6, 161, 145, 'L', 'i', 'm',  0,   0} },
/* 19 */  { .header = {.pointerToRegisterData = 8,          .dataType = dtReal34,      .tag = amNone,     .readOnly = 0, .notUsed = 0}, .reservedVariableName = {6, 161, 147, 'L', 'i', 'm',  0,   0} },
/* 20 */  { .header = {.pointerToRegisterData = 12,         .dataType = dtReal34,      .tag = amNone,     .readOnly = 0, .notUsed = 0}, .reservedVariableName = {2, 'F', 'V',  0,   0,   0,   0,   0} },
/* 21 */  { .header = {.pointerToRegisterData = 16,         .dataType = dtReal34,      .tag = amNone,     .readOnly = 0, .notUsed = 0}, .reservedVariableName = {4, 'i', '%', '/', 'a',  0,   0,   0} },
/* 22 */  { .header = {.pointerToRegisterData = 20,         .dataType = dtReal34,      .tag = amNone,     .readOnly = 0, .notUsed = 0}, .reservedVariableName = {4, 'N', 'P', 'E', 'R',  0,   0,   0} },
/* 23 */  { .header = {.pointerToRegisterData = 24,         .dataType = dtReal34,      .tag = amNone,     .readOnly = 0, .notUsed = 0}, .reservedVariableName = {5, 'P', 'E', 'R', '/', 'a',  0,   0} },
/* 24 */  { .header = {.pointerToRegisterData = 28,         .dataType = dtReal34,      .tag = amNone,     .readOnly = 0, .notUsed = 0}, .reservedVariableName = {3, 'P', 'M', 'T',  0,   0,   0,   0} },
/* 25 */  { .header = {.pointerToRegisterData = 32,         .dataType = dtReal34,      .tag = amNone,     .readOnly = 0, .notUsed = 0}, .reservedVariableName = {2, 'P', 'V',  0,   0,   0,   0,   0} },
/* 26 */  { .header = {.pointerToRegisterData = 36,         .dataType = dtLongInteger, .tag = LI_POSITIVE, .readOnly = 0, .notUsed = 0}, .reservedVariableName = {6, 'G', 'R', 'A', 'M', 'O', 'D',  0} },
};



uint32_t getRegisterDataType(calcRegister_t regist) {
  if(regist <= LAST_GLOBAL_REGISTER) { // Global register
    return globalRegister[regist].dataType;
  }

  else if(regist <= LAST_LOCAL_REGISTER) { // Local register
    if(currentLocalRegisters != NULL) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < currentNumberOfLocalRegisters) {
        return POINTER_TO_LOCAL_REGISTER(regist)->dataType;
      }
      #ifdef PC_BUILD
        else {
          sprintf(errorMessage, "local register %" PRId16, regist);
          sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" PRIu8, (uint8_t)(currentNumberOfLocalRegisters - 1));
          moreInfoOnError("In function getRegisterDataType:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
        }
      #endif // PC_BUILD
    }
    #ifdef PC_BUILD
      else {
        moreInfoOnError("In function getRegisterDataType:", "no local registers defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
      }
    #endif // PC_BUILD
  }

  else if(regist <= LAST_TEMP_REGISTER) { // Saved stack register or temporary register
    return savedStackRegister[regist - FIRST_SAVED_STACK_REGISTER].dataType;
  }

  else if(regist <= LAST_NAMED_VARIABLE) { // Named variable
    if(numberOfNamedVariables > 0) {
      regist -= FIRST_NAMED_VARIABLE;
      if(regist < numberOfNamedVariables) {
        return allNamedVariables[regist].header.dataType;
      }
      #ifdef PC_BUILD
        else {
          sprintf(errorMessage, "named variable %" PRId16, regist);
          sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" PRIu16, (uint16_t)(numberOfNamedVariables - 1));
          moreInfoOnError("In function getRegisterDataType:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
        }
      #endif // PC_BUILD
    }
    else {
      displayBugScreen("In function getRegisterDataType: no named variables defined!");
    }
  }

  else if(regist <= LAST_RESERVED_VARIABLE) { // System named variable
    regist -= FIRST_RESERVED_VARIABLE;
    if(regist < 12) { // Lettered register
      return globalRegister[regist + REGISTER_X].dataType;
    }
    else {
      return allReservedVariables[regist].header.dataType;
    }
  }

  else {
    sprintf(errorMessage, "In function getRegisterDataType: regist=%" PRId16 " must be less than %d!", regist, LAST_RESERVED_VARIABLE + 1);
    displayBugScreen(errorMessage);
  }

  return 31u;
}



dataBlock_t *getRegisterDataPointer(calcRegister_t regist) {
  if(regist <= LAST_GLOBAL_REGISTER) { // Global register
    return TO_PCMEMPTR(globalRegister[regist].pointerToRegisterData);
  }

  else if(regist <= LAST_LOCAL_REGISTER) { // Local register
    if(currentLocalRegisters != NULL) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < currentNumberOfLocalRegisters) {
        return TO_PCMEMPTR(POINTER_TO_LOCAL_REGISTER(regist)->pointerToRegisterData);
      }
      #ifdef PC_BUILD
        else {
          sprintf(errorMessage, "local register %" PRId16, regist);
          sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" PRIu8, (uint8_t)(currentNumberOfLocalRegisters - 1));
          moreInfoOnError("In function getRegisterDataPointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
        }
      #endif // PC_BUILD
    }
    #ifdef PC_BUILD
      else {
        moreInfoOnError("In function getRegisterDataPointer:", "no local registers defined!", "To do so, use LocR", NULL);
      }
    #endif // PC_BUILD
  }

  else if(regist <= LAST_TEMP_REGISTER) { // Saved stack register or temporary register
    return TO_PCMEMPTR(savedStackRegister[regist - FIRST_SAVED_STACK_REGISTER].pointerToRegisterData);
  }

  else if(regist <= LAST_NAMED_VARIABLE) { // Named variable
    if(numberOfNamedVariables > 0) {
      regist -= FIRST_NAMED_VARIABLE;
      if(regist < numberOfNamedVariables) {
        return TO_PCMEMPTR(allNamedVariables[regist].header.pointerToRegisterData);
      }
      #ifdef PC_BUILD
        else {
          sprintf(errorMessage, "named variable %" PRId16, regist);
          sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" PRIu16, (uint16_t)(numberOfNamedVariables - 1));
          moreInfoOnError("In function getRegisterDataPointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
        }
      #endif // PC_BUILD
    }
    else {
      displayBugScreen("In function getRegisterDataPointer: no named variables defined!");
    }
  }

  else if(regist <= LAST_RESERVED_VARIABLE) { // System named variable
    regist -= FIRST_RESERVED_VARIABLE;
    return TO_PCMEMPTR(allReservedVariables[regist].header.pointerToRegisterData);
  }

  else {
    sprintf(errorMessage, "In function getRegisterDataPointer: regist=%" PRId16 " must be less than %d!", regist, LAST_RESERVED_VARIABLE + 1);
    displayBugScreen(errorMessage);
  }
  return 0;
}



uint32_t getRegisterTag(calcRegister_t regist) {
  if(regist <= LAST_GLOBAL_REGISTER) { // Global register
    return globalRegister[regist].tag;
  }

  else if(regist <= LAST_LOCAL_REGISTER) { // Local register
    if(currentLocalRegisters != NULL) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < currentNumberOfLocalRegisters) {
        return POINTER_TO_LOCAL_REGISTER(regist)->tag;
      }
      #ifdef PC_BUILD
        else {
          sprintf(errorMessage, "local register %" PRId16, regist);
          sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" PRIu8, (uint8_t)(currentNumberOfLocalRegisters - 1));
          moreInfoOnError("In function getRegisterTag:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
        }
      #endif // PC_BUILD
    }
    #ifdef PC_BUILD
      else {
       moreInfoOnError("In function getRegisterTag:", "no local registers defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
      }
    #endif // PC_BUILD
  }

  else if(regist <= LAST_TEMP_REGISTER) { // Saved stack register or temporary register
    return savedStackRegister[regist - FIRST_SAVED_STACK_REGISTER].tag;
  }

  else if(regist <= LAST_NAMED_VARIABLE) { // Named variable
    if(numberOfNamedVariables > 0) {
      regist -= FIRST_NAMED_VARIABLE;
      if(regist < numberOfNamedVariables) {
        return allNamedVariables[regist].header.tag;
      }
      #ifdef PC_BUILD
        else {
          sprintf(errorMessage, "named variable %" PRId16, regist);
          sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" PRIu16, (uint16_t)(numberOfNamedVariables - 1));
          moreInfoOnError("In function getRegisterTag:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
        }
      #endif // PC_BUILD
    }
    else {
      displayBugScreen("In function getRegisterTag: no named variables defined!");
    }
  }

  else if(regist <= LAST_RESERVED_VARIABLE) { // System named variable
    regist -= FIRST_RESERVED_VARIABLE;
    return allReservedVariables[regist].header.tag;
  }

  else {
    sprintf(errorMessage, "In function getRegisterTag: regist=%" PRId16 " must be less than %d!", regist, LAST_RESERVED_VARIABLE + 1);
    displayBugScreen(errorMessage);
  }
  return 0;
}



void setRegisterDataType(calcRegister_t regist, uint16_t dataType, uint32_t tag) {
  if(regist <= LAST_GLOBAL_REGISTER) { // Global register
    globalRegister[regist].dataType = dataType;
    globalRegister[regist].tag = tag;
  }

  else if(regist <= LAST_LOCAL_REGISTER) { // Local register
    if(currentLocalRegisters != NULL) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < currentNumberOfLocalRegisters) {
        POINTER_TO_LOCAL_REGISTER(regist)->dataType = dataType;
        POINTER_TO_LOCAL_REGISTER(regist)->tag = tag;
      }
      #ifdef PC_BUILD
        else {
          sprintf(errorMessage, "local register %" PRId16, regist);
          sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" PRIu8, (uint8_t)(currentNumberOfLocalRegisters - 1));
          moreInfoOnError("In function setRegisterDataType:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
        }
      #endif // PC_BUILD
    }
    #ifdef PC_BUILD
      else {
       moreInfoOnError("In function setRegisterDataType:", "no local registers defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
      }
    #endif // PC_BUILD
  }

  else if(regist <= LAST_TEMP_REGISTER) { // Saved stack register or temporary register
    savedStackRegister[regist - FIRST_SAVED_STACK_REGISTER].dataType = dataType;
    savedStackRegister[regist - FIRST_SAVED_STACK_REGISTER].tag = tag;
  }

  else if(regist <= LAST_NAMED_VARIABLE) { // Named variable
    if(numberOfNamedVariables > 0) {
      regist -= FIRST_NAMED_VARIABLE;
      if(regist < numberOfNamedVariables) {
        allNamedVariables[regist].header.dataType = dataType;
        allNamedVariables[regist].header.tag = tag;
      }
      #ifdef PC_BUILD
        else {
          sprintf(errorMessage, "named variable %" PRId16, regist);
          sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" PRIu16, (uint16_t)(numberOfNamedVariables - 1));
          moreInfoOnError("In function setRegisterDataType:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
        }
      #endif // PC_BUILD
    }
    else {
      displayBugScreen("In function setRegisterDataType: no named variables defined!");
    }
  }

  else if(regist <= LAST_RESERVED_VARIABLE) { // System named variable
    regist -= FIRST_RESERVED_VARIABLE;
    if(allReservedVariables[regist].header.pointerToRegisterData != WP43S_NULL && allReservedVariables[regist].header.readOnly == 0) {
      allNamedVariables[regist].header.dataType = dataType;
      allNamedVariables[regist].header.tag = tag;
    }
  }

  else {
    sprintf(errorMessage, "In function setRegisterDataType: regist=%" PRId16 " must be less than %d!", regist, LAST_RESERVED_VARIABLE + 1);
    displayBugScreen(errorMessage);
  }
}



void setRegisterDataPointer(calcRegister_t regist, void *memPtr) {
  uint32_t dataPointer = TO_WP43SMEMPTR(memPtr);

  if(regist <= LAST_GLOBAL_REGISTER) { // Global register
    globalRegister[regist].pointerToRegisterData = dataPointer;
  }

  else if(regist <= LAST_LOCAL_REGISTER) { // Local register
    if(currentLocalRegisters != NULL) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < currentNumberOfLocalRegisters) {
        POINTER_TO_LOCAL_REGISTER(regist)->pointerToRegisterData = dataPointer;
      }
      #ifdef PC_BUILD
        else {
          sprintf(errorMessage, "local register %" PRId16, regist);
          sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" PRIu8, (uint8_t)(currentNumberOfLocalRegisters - 1));
          moreInfoOnError("In function setRegisterDataPointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
        }
      #endif // PC_BUILD
    }
    #ifdef PC_BUILD
      else {
        moreInfoOnError("In function setRegisterDataPointer:", "no local registers defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
      }
    #endif // PC_BUILD
  }

  else if(regist <= LAST_TEMP_REGISTER) { // Saved stack register or temporary register
    savedStackRegister[regist - FIRST_SAVED_STACK_REGISTER].pointerToRegisterData = dataPointer;
  }

  else if(regist <= LAST_NAMED_VARIABLE) { // Named variable
    if(numberOfNamedVariables > 0) {
      regist -= FIRST_NAMED_VARIABLE;
      if(regist < numberOfNamedVariables) {
        allNamedVariables[regist].header.pointerToRegisterData = dataPointer;
      }
      #ifdef PC_BUILD
        else {
          sprintf(errorMessage, "named variable %" PRId16, regist);
          sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" PRIu16, (uint16_t)(numberOfNamedVariables - 1));
          moreInfoOnError("In function setRegisterDataPointer:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
        }
      #endif // PC_BUILD
    }
    #ifdef PC_BUILD
      else {
        moreInfoOnError("In function setRegisterDataPointer:", "no local registers defined!", NULL, NULL);
      }
    #endif // PC_BUILD
  }

  else if(regist <= LAST_RESERVED_VARIABLE) { // System named variable
  }

  else {
    sprintf(errorMessage, "In function setRegisterDataPointer: regist=%" PRId16 " must be less than %d!", regist, LAST_RESERVED_VARIABLE + 1);
    displayBugScreen(errorMessage);
  }
}



void setRegisterTag(calcRegister_t regist, uint32_t tag) {
  if(regist <= LAST_GLOBAL_REGISTER) { // Global register
    globalRegister[regist].tag = tag;
  }

  else if(regist <= LAST_LOCAL_REGISTER) { // Local register
    if(currentLocalRegisters != NULL) {
      regist -= FIRST_LOCAL_REGISTER;
      if(regist < currentNumberOfLocalRegisters) {
        POINTER_TO_LOCAL_REGISTER(regist)->tag = tag;
      }
      #ifdef PC_BUILD
        else {
          sprintf(errorMessage, "local register %" PRId16, regist);
          sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" PRIu8, (uint8_t)(currentNumberOfLocalRegisters - 1));
          moreInfoOnError("In function setRegisterDataInfo:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
        }
      #endif // PC_BUILD
    }
    #ifdef PC_BUILD
      else {
        moreInfoOnError("In function setRegisterDataInfo:", "no local registers defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
      }
    #endif // PC_BUILD
  }

  else if(regist <= LAST_TEMP_REGISTER) { // Saved stack register or temporary register
    savedStackRegister[regist - FIRST_SAVED_STACK_REGISTER].tag = tag;
  }

  else if(regist <= LAST_NAMED_VARIABLE) { // Named variable
    if(numberOfNamedVariables > 0) {
      regist -= FIRST_NAMED_VARIABLE;
      if(regist < numberOfNamedVariables) {
        allNamedVariables[regist].header.tag = tag;
      }
      #ifdef PC_BUILD
        else {
          sprintf(errorMessage, "named variable %" PRId16, regist);
          sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" PRIu16, (uint16_t)(numberOfNamedVariables - 1));
          moreInfoOnError("In function setRegisterDataInfo:", errorMessage, "is not defined!", errorMessage + ERROR_MESSAGE_LENGTH/2);
        }
      #endif // PC_BUILD
    }
    else {
      displayBugScreen("In function setRegisterDataInfo: no named variables defined!");
    }
  }

  else if(regist <= LAST_RESERVED_VARIABLE) { // System named variable
  }

  else {
    sprintf(errorMessage, "In function setRegisterDataInfo: regist=%" PRId16 " must be less than %d!", regist, LAST_RESERVED_VARIABLE + 1);
    displayBugScreen(errorMessage);
  }
}



void allocateLocalRegisters(uint16_t numberOfRegistersToAllocate) {
  if(numberOfRegistersToAllocate > 99) {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "You can allocate up to 99 registers, you requested %" PRIu16, numberOfRegistersToAllocate);
      moreInfoOnError("In function allocateLocalRegisters:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  uint16_t r;
  if(currentLocalFlags == NULL) {
    // 1st allocation of local registers in this level of subroutine
    currentSubroutineLevelData = reallocWp43s(currentSubroutineLevelData, 3, 4 + numberOfRegistersToAllocate);
    currentLocalFlags = currentSubroutineLevelData + 3;
    currentLocalFlags->localFlags = 0;
    currentLocalRegisters = (registerHeader_t *)(currentSubroutineLevelData + 4);
    currentNumberOfLocalRegisters = numberOfRegistersToAllocate;
    currentNumberOfLocalFlags = NUMBER_OF_LOCAL_FLAGS;

    // All the new local registers are real34s initialized to 0.0
    for(r=FIRST_LOCAL_REGISTER; r<FIRST_LOCAL_REGISTER+numberOfRegistersToAllocate; r++) {
      setRegisterDataType(r, dtReal34, amNone);
      setRegisterDataPointer(r, allocWp43s(REAL34_SIZE));
      real34Zero(REGISTER_REAL34_DATA(r));
    }
  }
  else if(numberOfRegistersToAllocate != currentNumberOfLocalRegisters) {
    // The number of allocated local registers changes
    if(numberOfRegistersToAllocate > currentNumberOfLocalRegisters) {
      uint8_t oldNumberOfLocalRegisters = currentNumberOfLocalRegisters;
      currentSubroutineLevelData = reallocWp43s(currentSubroutineLevelData, 4 + currentNumberOfLocalRegisters, 4 + numberOfRegistersToAllocate);
      currentLocalFlags = currentSubroutineLevelData + 3;
      currentLocalRegisters = (registerHeader_t *)(currentSubroutineLevelData + 4);
      currentNumberOfLocalRegisters = numberOfRegistersToAllocate;

      // All the new local registers are real34s initialized to 0.0
      for(r=FIRST_LOCAL_REGISTER+oldNumberOfLocalRegisters; r<FIRST_LOCAL_REGISTER+numberOfRegistersToAllocate; r++) {
        setRegisterDataType(r, dtReal34, amNone);
        setRegisterDataPointer(r, allocWp43s(REAL34_SIZE));
        real34Zero(REGISTER_REAL34_DATA(r));
      }
    }
    else {
      // free memory allocated to the data of the deleted local registers
      for(r=numberOfRegistersToAllocate; r<currentNumberOfLocalRegisters; r++) {
        freeRegisterData(FIRST_LOCAL_REGISTER + r);
      }

      currentSubroutineLevelData = reallocWp43s(currentSubroutineLevelData, 4 + currentNumberOfLocalRegisters, 4 + numberOfRegistersToAllocate);
      currentLocalFlags = currentSubroutineLevelData + 3;
      currentLocalRegisters = (numberOfRegistersToAllocate == 0 ? NULL : (registerHeader_t *)(currentSubroutineLevelData + 4));
      currentNumberOfLocalRegisters = numberOfRegistersToAllocate;
    }
  }
  else {
    return;
  }

  if(currentSubroutineLevel == 0) {
    allSubroutineLevels.ptrToSubroutineLevel0Data = TO_WP43SMEMPTR(currentSubroutineLevelData);
  }
  else {
    ((dataBlock_t *)(TO_PCMEMPTR(currentPtrToPreviousLevel)))[2].ptrToNextLevel = TO_WP43SMEMPTR(currentSubroutineLevelData);
  }
}



void allocateNamedVariable(const char *variableName, dataType_t dataType, uint16_t fullDataSizeInBlocks) {
  calcRegister_t regist;
  uint8_t len;

  if(stringGlyphLength(variableName) < 1 || stringGlyphLength(variableName) > 7) {
    #ifdef PC_BUILD
      sprintf(errorMessage, "the name %s", variableName);
      moreInfoOnError("In function allocateNamedVariable:", errorMessage, "is incorrect! The length must be", "from 1 to 7 glyphs!");
    #endif // PC_BUILD
    return;
  }

  if(numberOfNamedVariables == 0) { // First named variable
    allNamedVariables = allocWp43s(TO_BLOCKS(sizeof(namedVariableHeader_t)));
    numberOfNamedVariables = 1;

    regist = 0;
  }
  else {
    regist = numberOfNamedVariables;
    if(regist == LAST_NAMED_VARIABLE - FIRST_NAMED_VARIABLE + 1) {
      #ifdef PC_BUILD
        sprintf(errorMessage, "%d named variables!", LAST_NAMED_VARIABLE - FIRST_NAMED_VARIABLE + 1);
        moreInfoOnError("In function allocateNamedVariable:", "you can allocate up to", errorMessage, NULL);
      #endif // PC_BUILD
      return;
    }

    allNamedVariables = reallocWp43s(allNamedVariables, TO_BLOCKS(sizeof(namedVariableHeader_t) * numberOfNamedVariables), TO_BLOCKS(sizeof(namedVariableHeader_t) * (numberOfNamedVariables + 1)));
    numberOfNamedVariables++;
  }

  len = stringByteLength(variableName);
  allNamedVariables[regist].variableName[0] = len;
  // Ensure that we terminate with \0 in the string to make in place comparisons easier
  memset(allNamedVariables[regist].variableName + 1, 0, 15);
  xcopy(allNamedVariables[regist].variableName + 1, variableName, len);

  regist += FIRST_NAMED_VARIABLE;
  setRegisterDataType(regist, dataType, amNone);
  setRegisterDataPointer(regist, allocWp43s(fullDataSizeInBlocks));
}



calcRegister_t findNamedVariable(const char *variableName) {
  calcRegister_t regist = INVALID_VARIABLE;
  uint8_t len = stringGlyphLength(variableName);
  if(len < 1 || len > 7) {
    return regist;
  }
  for(int i = 0; i < numberOfNamedVariables; i++) {
    if (compareString((char *)(allNamedVariables[i].variableName + 1), variableName, CMP_EXTENSIVE) == 0) {
      regist = i + FIRST_NAMED_VARIABLE;
      break;
    }
  }
  return regist;
}



calcRegister_t findOrAllocateNamedVariable(const char *variableName) {
  calcRegister_t regist = INVALID_VARIABLE;
  uint8_t len = stringGlyphLength(variableName);
  if(len < 1 || len > 7) {
    return regist;
  }
  regist = findNamedVariable(variableName);
  if(regist == INVALID_VARIABLE && numberOfNamedVariables <= (LAST_NAMED_VARIABLE - FIRST_NAMED_VARIABLE)) {
    allocateNamedVariable(variableName, dtReal34, REAL34_SIZE);
    // New variables are zero by default - although this might be immediately overridden, it might require an
    // initial value, such as when STO+
    regist = FIRST_NAMED_VARIABLE + numberOfNamedVariables - 1;
    real34Zero(REGISTER_REAL34_DATA(regist));
  }
  return regist;
}



void setRegisterMaxDataLength(calcRegister_t regist, uint16_t maxDataLen) {
  if(regist <= LAST_GLOBAL_REGISTER) { // Global register
    ((dataBlock_t *)TO_PCMEMPTR(globalRegister[regist].pointerToRegisterData))->dataMaxLength = maxDataLen;
  }

  else if(regist <= LAST_LOCAL_REGISTER) { // Local register
    if(currentLocalRegisters != NULL) {
      if(regist-FIRST_LOCAL_REGISTER < currentNumberOfLocalRegisters) {
        getRegisterDataPointer(regist)->dataMaxLength = maxDataLen;
      }
      #ifdef PC_BUILD
        else {
          sprintf(errorMessage, "local register %" PRId16 " is not defined!", (uint16_t)(regist - FIRST_LOCAL_REGISTER));
          sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "Must be from 0 to %" PRIu8, (uint8_t)(currentNumberOfLocalRegisters - 1));
          moreInfoOnError("In function setRegisterMaxDataLength:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
        }
      #endif // PC_BUILD
    }
    #ifdef PC_BUILD
      else {
       moreInfoOnError("In function setRegisterMaxDataLength:", "no local registers defined!", NULL, NULL);
      }
    #endif // PC_BUILD
  }

  else if(regist <= LAST_TEMP_REGISTER) { // Saved stack register or temporary register
    getRegisterDataPointer(regist)->dataMaxLength = maxDataLen;
  }

  else if(regist <= LAST_NAMED_VARIABLE) { // Named variable
    if(numberOfNamedVariables > 0) {
      if(regist - FIRST_NAMED_VARIABLE < numberOfNamedVariables) {
        getRegisterDataPointer(regist)->dataMaxLength = maxDataLen;
      }
      else {
        sprintf(errorMessage, "In function setRegisterMaxDataLength: named variable %" PRId16 " is not defined! Must be from 0 to %" PRIu16, (uint16_t)(regist - FIRST_NAMED_VARIABLE), (uint16_t)(numberOfNamedVariables - 1));
        displayBugScreen(errorMessage);
      }
    }
    #ifdef PC_BUILD
      else {
        moreInfoOnError("In function setRegisterMaxDataLength:", "no named variables defined!", NULL, NULL);
      }
    #endif // PC_BUILD
  }

  else if(regist <= LAST_RESERVED_VARIABLE) { // System named variable
    regist -= FIRST_RESERVED_VARIABLE;
    getRegisterDataPointer(regist)->dataMaxLength = maxDataLen;
  }

  else {
    sprintf(errorMessage, "In function setRegisterMaxDataLength: regist=%" PRId16 " must be less than %d!", regist, LAST_RESERVED_VARIABLE + 1);
    displayBugScreen(errorMessage);
  }
}



uint16_t getRegisterMaxDataLength(calcRegister_t regist) {
  dataBlock_t *db = NULL;

  if(regist <= LAST_GLOBAL_REGISTER) { // Global register
      db = (dataBlock_t *)TO_PCMEMPTR(globalRegister[regist].pointerToRegisterData);
  }

  else if(regist <= LAST_LOCAL_REGISTER) { // Local register
    if(currentLocalRegisters != NULL) {
      if(regist-FIRST_LOCAL_REGISTER < currentNumberOfLocalRegisters) {
        db = (dataBlock_t *)TO_PCMEMPTR(POINTER_TO_LOCAL_REGISTER(regist)->pointerToRegisterData);
      }
      else {
        sprintf(errorMessage, "In function getRegisterMaxDataLength: local register %" PRId16 " is not defined! Must be from 0 to %" PRIu8, (uint16_t)(regist - FIRST_LOCAL_REGISTER), (uint8_t)(currentNumberOfLocalRegisters - 1));
        displayBugScreen(errorMessage);
      }
    }
    #ifdef PC_BUILD
      else {
        moreInfoOnError("In function getRegisterMaxStringLength:", "no local registers defined!", NULL, NULL);
      }
    #endif // PC_BUILD
  }

  else if(regist <= LAST_TEMP_REGISTER) { // Saved stack register or temporary register
    db = (dataBlock_t *)TO_PCMEMPTR(savedStackRegister[regist - FIRST_SAVED_STACK_REGISTER].pointerToRegisterData);
  }

  else if(regist <= LAST_NAMED_VARIABLE) { // Named variable
    if(numberOfNamedVariables != 0) {
      regist -= FIRST_NAMED_VARIABLE;
      if(regist < numberOfNamedVariables) {
        db = (dataBlock_t *)TO_PCMEMPTR(allNamedVariables[regist].header.pointerToRegisterData);
      }
      else {
        sprintf(errorMessage, "In function getRegisterMaxDataLength: named variable %" PRId16 " is not defined! Must be from 0 to %" PRIu16, regist, (uint16_t)(numberOfNamedVariables - 1));
        displayBugScreen(errorMessage);
      }
    }
    #ifdef PC_BUILD
      else {
        moreInfoOnError("In function getRegisterMaxStringLength:", "no named variables defined!", NULL, NULL);
      }
    #endif // PC_BUILD
  }

  else if(regist <= LAST_RESERVED_VARIABLE) { // System named variable
    regist -= FIRST_RESERVED_VARIABLE;
    db = (dataBlock_t *)TO_PCMEMPTR(allReservedVariables[regist].header.pointerToRegisterData);
  }

  else {
    sprintf(errorMessage, "In function getRegisterMaxDataLength: regist=%" PRId16 " must be less than %d!", regist, LAST_RESERVED_VARIABLE + 1);
    displayBugScreen(errorMessage);
  }

  if(db) {
    if(getRegisterDataType(regist) == dtReal34Matrix) {
      return db->matrixRows * db->matrixColumns * REAL34_SIZE;
    }
    else if(getRegisterDataType(regist) == dtComplex34Matrix) {
      return db->matrixRows * db->matrixColumns * COMPLEX34_SIZE;
    }
    else {
      return db->dataMaxLength;
    }
  }
  return 0;
}



uint16_t getRegisterFullSize(calcRegister_t regist) {
  switch(getRegisterDataType(regist)) {
    case dtLongInteger:     return getRegisterDataPointer(regist)->dataMaxLength + 1;
    case dtTime:            return REAL34_SIZE;
    case dtDate:            return REAL34_SIZE;
    case dtString:          return getRegisterDataPointer(regist)->dataMaxLength + 1;
    case dtReal34Matrix:    return TO_BLOCKS((getRegisterDataPointer(regist)->matrixRows * getRegisterDataPointer(regist)->matrixColumns) * sizeof(real34_t)) + 1; break;
    case dtComplex34Matrix: return TO_BLOCKS((getRegisterDataPointer(regist)->matrixRows * getRegisterDataPointer(regist)->matrixColumns) * sizeof(complex34_t)) + 1; break;
    case dtShortInteger:    return SHORT_INTEGER_SIZE;
    case dtReal34:          return REAL34_SIZE;
    case dtComplex34:       return COMPLEX34_SIZE;
    case dtConfig:          return CONFIG_SIZE;

    default:
      sprintf(errorMessage, "In function getRegisterFullSize: data type %s is unknown!", getDataTypeName(getRegisterDataType(regist), false, false));
      displayBugScreen(errorMessage);
      return 0;
  }
}



void clearRegister(calcRegister_t regist) {
  if(getRegisterDataType(regist) == dtReal34) {
    real34Zero(REGISTER_REAL34_DATA(regist));
    setRegisterTag(regist, amNone);
  }
  else{
    reallocateRegister(regist, dtReal34, REAL34_SIZE, amNone);
    real34Zero(REGISTER_REAL34_DATA(regist));
  }
}



void fnClearRegisters(uint16_t confirmation) {
  if(confirmation == NOT_CONFIRMED) {
    setConfirmationMode(fnClearRegisters);
  }
  else {
    calcRegister_t regist;

    for(regist=0; regist<REGISTER_X; regist++) {
      clearRegister(regist);
    }

    for(regist=0; regist<currentNumberOfLocalRegisters; regist++) {
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
}



void fnGetLocR(uint16_t unusedButMandatoryParameter) {
  longInteger_t locR;

  liftStack();

  longIntegerInit(locR);
  uIntToLongInteger(currentNumberOfLocalRegisters, locR);
  convertLongIntegerToLongIntegerRegister(locR, REGISTER_X);
  longIntegerFree(locR);
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

  resultDataType = getRegisterDataType(res);
  if(getSystemFlag(FLAG_SPCRES) == false && lastErrorCode == 0) {
    // D is clear: test infinite values and -0 values
    switch(resultDataType) {
      case dtReal34:
      case dtTime:
      case dtDate:
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

        if(real34IsInfinite(REGISTER_IMAG34_DATA(res))) {
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

  if(resultDataType == dtTime) checkTimeRange(REGISTER_REAL34_DATA(res));
  if(resultDataType == dtDate) checkDateRange(REGISTER_REAL34_DATA(res));

  if(lastErrorCode != 0) {
    #ifdef TESTSUITE_BUILD
      undo();
    #endif //TESTSUITE_BUILD
    return;
  }

  if(setCpxRes && oneArgumentIsComplex && resultDataType != dtString) {
    fnSetFlag(FLAG_CPXRES);
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
  }
}



void copySourceRegisterToDestRegister(calcRegister_t sourceRegister, calcRegister_t destRegister) {
  if(   getRegisterDataType(destRegister) != getRegisterDataType(sourceRegister)
    || getRegisterFullSize(destRegister) != getRegisterFullSize(sourceRegister)) {
    uint32_t sizeInBlocks;

    switch(getRegisterDataType(sourceRegister)) {
      case dtLongInteger:     sizeInBlocks = getRegisterDataPointer(sourceRegister)->dataMaxLength; break;
      case dtTime:            sizeInBlocks = REAL34_SIZE;                                           break;
      case dtDate:            sizeInBlocks = REAL34_SIZE;                                           break;
      case dtString:          sizeInBlocks = getRegisterDataPointer(sourceRegister)->dataMaxLength; break;
      case dtReal34Matrix:    sizeInBlocks = TO_BLOCKS((getRegisterDataPointer(sourceRegister)->matrixRows * getRegisterDataPointer(sourceRegister)->matrixColumns) * sizeof(real34_t)); break;
      case dtComplex34Matrix: sizeInBlocks = TO_BLOCKS((getRegisterDataPointer(sourceRegister)->matrixRows * getRegisterDataPointer(sourceRegister)->matrixColumns) * sizeof(complex34_t)); break;
      case dtShortInteger:    sizeInBlocks = SHORT_INTEGER_SIZE;                                    break;
      case dtReal34:          sizeInBlocks = REAL34_SIZE;                                           break;
      case dtComplex34:       sizeInBlocks = COMPLEX34_SIZE;                                        break;
      case dtConfig:          sizeInBlocks = CONFIG_SIZE;                                           break;

      default:
        sprintf(errorMessage, "In function copySourceRegisterToDestRegister: data type %s is unknown!", getDataTypeName(getRegisterDataType(sourceRegister), false, false));
        displayBugScreen(errorMessage);
        sizeInBlocks = 0;
    }
    reallocateRegister(destRegister, getRegisterDataType(sourceRegister), sizeInBlocks, amNone);
  }

  switch(getRegisterDataType(sourceRegister)) {
    case dtReal34Matrix:
      xcopy(REGISTER_REAL34_MATRIX_DBLOCK(destRegister), REGISTER_REAL34_MATRIX_DBLOCK(sourceRegister), sizeof(dataBlock_t));
      xcopy(REGISTER_REAL34_MATRIX_M_ELEMENTS(destRegister), REGISTER_REAL34_MATRIX_M_ELEMENTS(sourceRegister),
        getRegisterDataPointer(sourceRegister)->matrixRows * getRegisterDataPointer(sourceRegister)->matrixColumns * TO_BYTES(REAL34_SIZE));
      break;
    case dtComplex34Matrix:
      xcopy(REGISTER_COMPLEX34_MATRIX_DBLOCK(destRegister), REGISTER_COMPLEX34_MATRIX_DBLOCK(sourceRegister), sizeof(dataBlock_t));
      xcopy(REGISTER_COMPLEX34_MATRIX_M_ELEMENTS(destRegister), REGISTER_COMPLEX34_MATRIX_M_ELEMENTS(sourceRegister),
        getRegisterDataPointer(sourceRegister)->matrixRows * getRegisterDataPointer(sourceRegister)->matrixColumns * TO_BYTES(COMPLEX34_SIZE));
      break;

    default:
      xcopy(REGISTER_DATA(destRegister), REGISTER_DATA(sourceRegister), TO_BYTES(getRegisterFullSize(sourceRegister)));
  }
  setRegisterTag(destRegister, getRegisterTag(sourceRegister));
}



int16_t indirectAddressing(calcRegister_t regist, bool_t valueIsRegister, int16_t minValue, int16_t maxValue) {
  int16_t value;
  bool_t isValidAlpha = false;

  if(valueIsRegister) {
    // Temorarily assign the maximum value to the maximum register
    // We need to do better range checking later
    maxValue = FIRST_NAMED_VARIABLE + numberOfNamedVariables;
  }

  if(regist >= FIRST_LOCAL_REGISTER + currentNumberOfLocalRegisters &&
     (regist < FIRST_NAMED_VARIABLE ||
        regist >= FIRST_NAMED_VARIABLE + numberOfNamedVariables)) {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #ifdef PC_BUILD
      sprintf(errorMessage, "local indirection register .%02d", regist - FIRST_LOCAL_REGISTER);
      moreInfoOnError("In function indirectAddressing:", errorMessage, "is not defined!", NULL);
    #endif // PC_BUILD
    return 9999;
  }

  else if(getRegisterDataType(regist) == dtReal34) {
    real34_t maxValue34;

    int32ToReal34(maxValue, &maxValue34);
    if(real34CompareLessThan(REGISTER_REAL34_DATA(regist), const34_0) || real34CompareGreaterEqual(REGISTER_REAL34_DATA(regist), &maxValue34)) {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      #ifdef PC_BUILD
        real34ToString(REGISTER_REAL34_DATA(regist), errorMessage);
        sprintf(tmpString, "register %" PRId16 " = %s:", regist, errorMessage);
        moreInfoOnError("In function indirectAddressing:", tmpString, "this value is negative or too big!", NULL);
      #endif // PC_BUILD
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
        sprintf(tmpString, "register %" PRId16 " = %s:", regist, errorMessage);
        moreInfoOnError("In function indirectAddressing:", tmpString, "this value is negative or too big!", NULL);
      #endif // PC_BUILD
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
        sprintf(tmpString, "register %" PRId16 " = %s:", regist, errorMessage);
        moreInfoOnError("In function indirectAddressing:", tmpString, "this value is negative or too big!", NULL);
      #endif // PC_BUILD
      return 9999;
    }
    value = val;
  }

  else if(getRegisterDataType(regist) == dtString && valueIsRegister) {
    value = findNamedVariable(REGISTER_STRING_DATA(regist));
    isValidAlpha = true;
    if(value == INVALID_VARIABLE) {
      displayCalcErrorMessage(ERROR_UNDEF_SOURCE_VAR, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "string '%s' is not a named variable", REGISTER_STRING_DATA(regist));
        moreInfoOnError("In function indirectAddressing:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return 9999;
    }
  }

  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #ifdef PC_BUILD
      sprintf(errorMessage, "register %" PRId16 " is %s:", regist, getRegisterDataTypeName(regist, true, false));
      moreInfoOnError("In function indirectAddressing:", errorMessage, "not suited for indirect addressing!", NULL);
    #endif // PC_BUILD
    return 9999;
  }

  if(valueIsRegister && minValue <= value && (value <= 99 || isValidAlpha)) {
    return value;
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #ifdef PC_BUILD
      sprintf(errorMessage, "value = %d! Should be from %d to 99.", value, minValue);
      moreInfoOnError("In function indirectAddressing:", errorMessage, NULL, NULL);
    #endif // PC_BUILD
  }

  if(minValue <= value && value <= maxValue) {
    return value;
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #ifdef PC_BUILD
      sprintf(errorMessage, "value = %d! Should be from %d to %d.", value, minValue, maxValue);
      moreInfoOnError("In function indirectAddressing:", errorMessage, NULL, NULL);
    #endif // PC_BUILD
    return 9999;
  }
}



#ifdef TESTSUITE_BUILD
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
      sprintf(registerContent, "string (%" PRIu32 " bytes) |%s|", TO_BYTES(getRegisterMaxDataLength(regist)), str);
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
      char lgIntStr[3000];

      convertLongIntegerRegisterToLongInteger(regist, lgInt);
      longIntegerToAllocatedString(lgInt, lgIntStr, sizeof(lgIntStr));
      longIntegerFree(lgInt);
      sprintf(registerContent, "long integer (%" PRIu32 " bytes) %s", TO_BYTES(getRegisterMaxDataLength(regist)), lgIntStr);
    }

    else if(getRegisterDataType(regist) == dtTime) {
      real34ToString(REGISTER_REAL34_DATA(regist), str);
      sprintf(registerContent, "time %s", str);
    }

    else if(getRegisterDataType(regist) == dtDate) {
      real34ToString(REGISTER_REAL34_DATA(regist), str);
      sprintf(registerContent, "date %s", str);
    }

    else {
      sprintf(registerContent, "In printRegisterToString: data type %s not supported", getRegisterDataTypeName(regist ,false, false));
    }
  }
#endif // TESTSUITE_BUILD



#ifndef DMCP_BUILD
  void printRegisterToConsole(calcRegister_t regist, const char *before, const char *after) {
    char str[3000];

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
      printf("string (%" PRIu64 " + %" PRIu32 " bytes) |%s|", (uint64_t)sizeof(dataBlock_t), TO_BYTES(getRegisterMaxDataLength(regist)), str);
    }

    else if(getRegisterDataType(regist) == dtShortInteger) {
      uint64_t value = *(REGISTER_SHORT_INTEGER_DATA(regist));

      printf("short integer %08x-%08x (base %" PRIu32 ")", (unsigned int)(value>>32), (unsigned int)(value&0xffffffff), getRegisterTag(regist));
    }

    else if(getRegisterDataType(regist) == dtConfig) {
      printf("Configuration data");
    }

    else if(getRegisterDataType(regist) == dtLongInteger) {
      longInteger_t lgInt;

      convertLongIntegerRegisterToLongInteger(regist, lgInt);
      longIntegerToAllocatedString(lgInt, str, sizeof(str));
      longIntegerFree(lgInt);
      printf("long integer (%" PRIu64 " + %" PRIu32 " bytes) %s", (uint64_t)sizeof(dataBlock_t), TO_BYTES(getRegisterMaxDataLength(regist)), str);
    }

    else if(getRegisterDataType(regist) == dtTime) {
      real34ToString(REGISTER_REAL34_DATA(regist), str);
      printf("time %s", str);
    }

    else if(getRegisterDataType(regist) == dtDate) {
      real34ToString(REGISTER_REAL34_DATA(regist), str);
      printf("date %s", str);
    }

    else {
      sprintf(errorMessage, "In printRegisterToConsole: data type %s not supported", getRegisterDataTypeName(regist ,false, false));
      displayBugScreen(errorMessage);
    }

    printf("%s", after);
  }



  void printReal34ToConsole(const real34_t *value, const char *before, const char *after) {
    char str[100];

    real34ToString(value, str);
    printf("%sreal34 %s%s", before, str, after);
  }



  void printRealToConsole(const real_t *value, const char *before, const char *after) {
    char str[1000];

    realToString(value, str);
    printf("%sreal%" PRId32 " %s%s", before, value->digits, str, after);

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
    printf("%" PRIu16, value->lsu[i--]);

    exponent = value->exponent;
    last = 0;
    while(exponent <= -DECDPUN && value->lsu[last] == 0) {
      last++;
      exponent += DECDPUN;
    }

    for(; i>=last; i--) {
      printf(" %03" PRIu16, value->lsu[i]);
    }

    if(exponent != 0) {
      printf(" e %" PRId32, exponent);
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
    registerHeader_t registerHeader;

    registerHeader.descriptor = 0xFFFFFFFF;

    if(regist <= LAST_GLOBAL_REGISTER) { // Global register
      registerHeader = globalRegister[regist];
    }

    else if(regist <= LAST_LOCAL_REGISTER) { // Local register
      if(currentNumberOfLocalRegisters > 0) {
        regist -= FIRST_LOCAL_REGISTER;
        if(regist < currentNumberOfLocalRegisters) {
          registerHeader = *POINTER_TO_LOCAL_REGISTER(regist);
        }
      }
    }

    else if(regist <= LAST_TEMP_REGISTER) { // Saved stack register or temporary register
      registerHeader = savedStackRegister[regist - FIRST_SAVED_STACK_REGISTER];
    }

    else if(regist <= LAST_NAMED_VARIABLE) { // Named variable
      if(numberOfNamedVariables > 0) {
        regist -= FIRST_NAMED_VARIABLE;
        if(regist < numberOfNamedVariables) {
          registerHeader = allNamedVariables[regist].header;
        }
      }
    }

    printf("Header informations of register %d\n", regist);
    printf("    reg ptr   = %u\n", registerHeader.pointerToRegisterData);
    printf("    data type = %u = %s\n", registerHeader.dataType, getDataTypeName(registerHeader.dataType, false, false));
    if(registerHeader.dataType == dtLongInteger || registerHeader.dataType == dtString) {
      printf("    data ptr  = %u\n", registerHeader.pointerToRegisterData + 1);
      printf("    data size = %" PRIu32 "\n", *(uint32_t *)TO_PCMEMPTR(globalRegister[regist].pointerToRegisterData));
    }
    printf("    tag       = %u\n", registerHeader.tag);
  }



  void printLongIntegerToConsole(const longInteger_t value, const char *before, const char *after) {
    char str[3000];

    longIntegerToAllocatedString(value, str, sizeof(str));
    printf("%slong integer (%" PRIu64 " + %" PRIu64 " <%" PRIu64 " reserved> bytes) %s%s", before, (uint64_t)(sizeof(value->_mp_size) + sizeof(value->_mp_d) + sizeof(value->_mp_alloc)), (uint64_t)longIntegerSizeInBytes(value), (uint64_t)(value->_mp_alloc * LIMB_SIZE), str, after);
  }
#endif // !DMCP_BUILD



void reallocateRegister(calcRegister_t regist, uint32_t dataType, uint16_t dataSizeWithoutDataLenBlocks, uint32_t tag) { // dataSize without data length in blocks, this includes the trailing 0 for strings
  uint16_t dataSizeWithDataLenBlocks = dataSizeWithoutDataLenBlocks;

  //printf("reallocateRegister: %d to %s tag=%u (%u bytes excluding maxSize) begin\n", regist, getDataTypeName(dataType, false, false), tag, dataSizeWithoutDataLenBlocks);
  if(dataType == dtReal34 && dataSizeWithoutDataLenBlocks != REAL34_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" PRIu16 " is an unexpected numByte value for a real34 or an angle34! It should be REAL34_SIZE=%" PRIu16 "!", dataSizeWithoutDataLenBlocks, (uint16_t)REAL34_SIZE);
    displayBugScreen(errorMessage);
  }
  else if(dataType == dtComplex34 && dataSizeWithoutDataLenBlocks != COMPLEX34_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" PRIu16 " is an unexpected numByte value for a complex34! It should be COMPLEX34_SIZE=%" PRIu16 "!", dataSizeWithoutDataLenBlocks, (uint16_t)COMPLEX34_SIZE);
    displayBugScreen(errorMessage);
  }
  else if(dataType == dtShortInteger && dataSizeWithoutDataLenBlocks != SHORT_INTEGER_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" PRIu16 " is an unexpected numByte value for an integer! It should be SHORT_INTEGER_SIZE=%" PRIu16 "!", dataSizeWithoutDataLenBlocks, (uint16_t)SHORT_INTEGER_SIZE);
    displayBugScreen(errorMessage);
  }
  else if(dataType == dtConfig && dataSizeWithoutDataLenBlocks != CONFIG_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" PRIu16 " is an unexpected numByte value for a configuration! It should be CONFIG_SIZE=%" PRIu16 "!", dataSizeWithoutDataLenBlocks, (uint16_t)CONFIG_SIZE);
    displayBugScreen(errorMessage);
  }
  else if(dataType == dtTime && dataSizeWithoutDataLenBlocks != REAL34_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" PRIu16 " is an unexpected numByte value for a time! It should be REAL34_SIZE=%" PRIu16 "!", dataSizeWithoutDataLenBlocks, (uint16_t)REAL34_SIZE);
    displayBugScreen(errorMessage);
  }
  else if(dataType == dtDate && dataSizeWithoutDataLenBlocks != REAL34_SIZE) {
    sprintf(errorMessage, "In function reallocateRegister: %" PRIu16 " is an unexpected numByte value for a date! It should be REAL34_SIZE=%" PRIu16 "!", dataSizeWithoutDataLenBlocks, (uint16_t)REAL34_SIZE);
    displayBugScreen(errorMessage);
  }
  else if(dataType == dtString || dataType == dtReal34Matrix || dataType == dtComplex34Matrix) {
    dataSizeWithDataLenBlocks = dataSizeWithoutDataLenBlocks + 1; // +1 for the max length of the string
  }
  else if(dataType == dtLongInteger) {
    if(TO_BYTES(dataSizeWithoutDataLenBlocks) % LIMB_SIZE != 0) {
      dataSizeWithoutDataLenBlocks = ((dataSizeWithoutDataLenBlocks / TO_BLOCKS(LIMB_SIZE)) + 1) * TO_BLOCKS(LIMB_SIZE);
    }
    dataSizeWithDataLenBlocks = dataSizeWithoutDataLenBlocks + 1; // +1 for the max length of the data
  }

  if(getRegisterDataType(regist) != dataType || ((getRegisterDataType(regist) == dtString || getRegisterDataType(regist) == dtLongInteger || getRegisterDataType(regist) == dtReal34Matrix || getRegisterDataType(regist) == dtComplex34Matrix) && getRegisterMaxDataLength(regist) != dataSizeWithoutDataLenBlocks)) {
    freeRegisterData(regist);
    setRegisterDataPointer(regist, allocWp43s(dataSizeWithDataLenBlocks));
    setRegisterDataType(regist, dataType, tag);
    if(dataType == dtReal34Matrix) {
      REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixRows = 1;
      REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixColumns = dataSizeWithoutDataLenBlocks / REAL34_SIZE;
    }
    else if(dataType == dtComplex34Matrix) {
      REGISTER_COMPLEX34_MATRIX_DBLOCK(regist)->matrixRows = 1;
      REGISTER_COMPLEX34_MATRIX_DBLOCK(regist)->matrixColumns = dataSizeWithoutDataLenBlocks / COMPLEX34_SIZE;
    }
    else {
      setRegisterMaxDataLength(regist, dataSizeWithDataLenBlocks - (dataType == dtString || dataType == dtLongInteger ? 1 : 0));
    }
  }
  else {
    setRegisterTag(regist, tag);
  }
//sprintf(tmpString, "reallocateRegister %d to %s tag=%u (%u bytes including dataLen) done", regist, getDataTypeName(dataType, false, false), tag, dataSizeWithDataLenBlocks);
//memoryDump(tmpString);
}



void fnToReal(uint16_t unusedButMandatoryParameter) {
  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger :
      copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      break;

    case dtShortInteger :
      copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
      convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      break;

    case dtReal34:
      copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
      setRegisterAngularMode(REGISTER_X, amNone);
      break;

    case dtTime:
      copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
      convertTimeRegisterToReal34Register(REGISTER_X, REGISTER_X);
      break;

    case dtDate:
      copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);
      convertDateRegisterToReal34Register(REGISTER_X, REGISTER_X);
      break;

    default :
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "data type %s cannot be converted to a real34!", getRegisterDataTypeName(REGISTER_X, false, false));
        moreInfoOnError("In function fnToReal:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
  }
}
