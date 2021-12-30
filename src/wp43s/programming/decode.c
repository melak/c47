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
 * \file decode.c
 ***********************************************/

#include "programming/decode.h"

#include "charString.h"
#include "dateTime.h"
#include "display.h"
#include "fonts.h"
#include "items.h"
#include "programming/nextStep.h"
#include "registers.h"
#include <string.h>

#include "wp43s.h"


#ifndef DMCP_BUILD
  void listPrograms(void) {
    uint16_t i, numberOfBytesInStep, stepNumber = 0, programNumber = 0;
    uint8_t *nextStep, *step;

    printf("\nProgram listing");
    step = beginOfProgramMemory;
    while(step) {
      if(step == programList[programNumber].instructionPointer) {
        programNumber++;
        if(programNumber != 1) {
          printf("\n------------------------------------------------------------");
        }
        printf("\nPgm Step   Bytes         OP");
      }

      nextStep = findNextStep(step);
      if(nextStep) {
        numberOfBytesInStep = (uint16_t)(nextStep - step);
        printf("\n%02d  %4d  ", programNumber, ++stepNumber - programList[programNumber - 1].step + 1); fflush(stdout);

        for(i=0; i<numberOfBytesInStep; i++) {
          printf(" %02x", *(step + i)); fflush(stdout);
          if(i == 3 && numberOfBytesInStep > 4) {
            decodeOneStep(step);
            stringToUtf8(tmpString, (uint8_t *)(tmpString + 2000));

            if(*step != ITM_LBL && (*step != ((ITM_END >> 8) | 0x80) || *(step + 1) != (ITM_END & 0xff))) { // Not LBL and not END
              printf("   "); fflush(stdout);
            }

            printf("   %s", tmpString + 2000); fflush(stdout);
          }

          if(i%4 == 3 && i != numberOfBytesInStep - 1) {
            printf("\n          "); fflush(stdout);
          }
        }

        if(numberOfBytesInStep <= 4) {
          for(i=1; i<=4 - ((numberOfBytesInStep - 1) % 4); i++) {
            printf("   "); fflush(stdout);
          }
          decodeOneStep(step);
          stringToUtf8(tmpString, (uint8_t *)(tmpString + 2000));

          if(*step != ITM_LBL && (*step != ((ITM_END >> 8) | 0x80) || *(step + 1) != (ITM_END & 0xff))) { // Not LBL and not END
            printf("   "); fflush(stdout);
          }

          printf("%s", tmpString + 2000); fflush(stdout);
        }
      }

      step = nextStep;
    }
    printf("\n");
  }


  void listLabelsAndPrograms(void) {
    printf("\nContent of labelList\n");
    printf("num program  step label\n");
    for(int i=0; i<numberOfLabels; i++) {
      printf("%3d%8d%6d ", i, labelList[i].program, labelList[i].step);
      if(labelList[i].step < 0) { // Local label
        if(*(labelList[i].labelPointer) < 100) {
          printf("%02d\n", *(labelList[i].labelPointer));
        }
        else if(*(labelList[i].labelPointer) < 105) {
          printf("%c\n", *(labelList[i].labelPointer) - 100 + 'A');
        }
      }
      else { // Global label
        xcopy(tmpString + 100, labelList[i].labelPointer + 1, *(labelList[i].labelPointer));
        tmpString[100 + *(labelList[i].labelPointer)] = 0;
        stringToUtf8(tmpString + 100, (uint8_t *)tmpString);
        printf("'%s'\n", tmpString);
      }
    }

    printf("\nContent of programList\n");
    printf("program  step OP\n");
    for(int i=0; i<numberOfPrograms; i++) {
      decodeOneStep(programList[i].instructionPointer);
      stringToUtf8(tmpString, (uint8_t *)(tmpString + 2000));
      printf("%7d %5d %s\n", i, programList[i].step, tmpString);
    }
  }
#endif // !DMCP_BUILD


static void getStringLabelOrVariableName(uint8_t *stringAddress) {
  uint8_t stringLength = *(uint8_t *)(stringAddress++);
  xcopy(tmpStringLabelOrVariableName, stringAddress, stringLength);
  tmpStringLabelOrVariableName[stringLength] = 0;
}


void getIndirectRegister(uint8_t *paramAddress, const char *op) {
  uint8_t opParam = *(uint8_t *)paramAddress;
  if(opParam < REGISTER_X) { // Global register from 00 to 99
    sprintf(tmpString, "%s " STD_RIGHT_ARROW "%02u", op, opParam);
  }
  else if(opParam <= REGISTER_K) { // Lettered register from X to K
    sprintf(tmpString, "%s " STD_RIGHT_ARROW "%s", op, indexOfItems[ITM_REG_X + opParam - REGISTER_X].itemSoftmenuName);
  }
  else if(opParam <= LAST_LOCAL_REGISTER) { // Local register from .00 to .98
    sprintf(tmpString, "%s " STD_RIGHT_ARROW ".%02d", op, opParam - FIRST_LOCAL_REGISTER);
  }
  else {
    sprintf(tmpString, "\nIn function getIndirectRegister: %s " STD_RIGHT_ARROW " %u is not a valid parameter!", op, opParam);
  }
}


void getIndirectVariable(uint8_t *stringAddress, const char *op) {
  getStringLabelOrVariableName(stringAddress);
  sprintf(tmpString, "%s " STD_RIGHT_ARROW STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, op, tmpStringLabelOrVariableName);
}


void decodeOp(uint8_t *paramAddress, const char *op, uint16_t paramMode) {
  uint8_t opParam = *(uint8_t *)(paramAddress++);

  switch(paramMode) {
    case PARAM_DECLARE_LABEL:
      if(opParam <= 99) { // Local label from 00 to 99
        sprintf(tmpString, "%s %02u", op, opParam);
      }
      else if(opParam <= 104) { // Local label from A to E
        sprintf(tmpString, "%s %c", op, 'A' + (opParam - 100));
      }
      else if(opParam == STRING_LABEL_VARIABLE) {
        getStringLabelOrVariableName(paramAddress);
        sprintf(tmpString, "%s " STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, op, tmpStringLabelOrVariableName);
      }
      else {
        sprintf(tmpString, "\nIn function decodeOp case PARAM_DECLARE_LABEL: opParam %u is not a valid label!\n", opParam);
      }
      break;

    case PARAM_LABEL:
      if(opParam <= 99) { // Local label from 00 to 99
        sprintf(tmpString, "%s %02u", op, opParam);
      }
      else if(opParam <= 104) { // Local label from A to E
        sprintf(tmpString, "%s %c", op, 'A' + (opParam - 100));
      }
      else if(opParam == STRING_LABEL_VARIABLE) {
        getStringLabelOrVariableName(paramAddress);
        sprintf(tmpString, "%s " STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, op, tmpStringLabelOrVariableName);
      }
      else if(opParam == INDIRECT_REGISTER) {
        getIndirectRegister(paramAddress, op);
      }
      else if(opParam == INDIRECT_VARIABLE) {
        getIndirectVariable(paramAddress, op);
      }
      else {
        sprintf(tmpString, "\nIn function decodeOp: case PARAM_LABEL, %s  %u is not a valid parameter!", op, opParam);
      }
      break;

    case PARAM_REGISTER:
      if(opParam < REGISTER_X) { // Global register from 00 to 99
        sprintf(tmpString, "%s %02u", op, opParam);
      }
      else if(opParam <= REGISTER_K) { // Lettered register from X to K
        sprintf(tmpString, "%s %s", op, indexOfItems[ITM_REG_X + opParam - REGISTER_X].itemSoftmenuName);
      }
      else if(opParam <= LAST_LOCAL_REGISTER) { // Local register from .00 to .98
        sprintf(tmpString, "%s .%02d", op, opParam - FIRST_LOCAL_REGISTER);
      }
      else if(opParam == STRING_LABEL_VARIABLE) {
        getStringLabelOrVariableName(paramAddress);
        sprintf(tmpString, "%s " STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, op, tmpStringLabelOrVariableName);
      }
      else if(opParam == INDIRECT_REGISTER) {
        getIndirectRegister(paramAddress, op);
      }
      else if(opParam == INDIRECT_VARIABLE) {
        getIndirectVariable(paramAddress, op);
      }
      else {
        sprintf(tmpString, "\nIn function decodeOp: case PARAM_REGISTER, %s  %u is not a valid parameter!", op, opParam);
      }
      break;

    case PARAM_FLAG:
      if(opParam < REGISTER_X) { // Global flag from 00 to 99
        sprintf(tmpString, "%s %02u", op, opParam);
      }
      else if(opParam <= REGISTER_K) { // Lettered flag from X to K
        sprintf(tmpString, "%s %s", op, indexOfItems[ITM_REG_X + opParam - REGISTER_X].itemSoftmenuName);
      }
      else if(opParam <= LAST_LOCAL_FLAG) { // Local flag from .00 to .15 (or .31)
        sprintf(tmpString, "%s .%02d", op, opParam - FIRST_LOCAL_FLAG);
      }
      else if(FIRST_LOCAL_FLAG + NUMBER_OF_LOCAL_FLAGS <= opParam && opParam < FIRST_LOCAL_FLAG + NUMBER_OF_LOCAL_FLAGS + NUMBER_OF_SYSTEM_FLAGS) { // Local register from .00 to .15 (or .31)
        sprintf(tmpString, "%s .%02d", op, opParam - FIRST_LOCAL_FLAG);
      }
      else if(opParam == SYSTEM_FLAG_NUMBER) {
        sprintf(tmpString, "%s " STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, op, indexOfItems[*paramAddress + SFL_TDM24].itemSoftmenuName);
      }
      else if(opParam == INDIRECT_REGISTER) {
        getIndirectRegister(paramAddress, op);
      }
      else if(opParam == INDIRECT_VARIABLE) {
        getIndirectVariable(paramAddress, op);
      }
      else {
        sprintf(tmpString, "\nIn function decodeOp: case PARAM_FLAG, %s  %u is not a valid parameter!", op, opParam);
      }
      break;

    case PARAM_NUMBER_8:
      if(opParam <= 99) { // Value from 0 to 99
        sprintf(tmpString, "%s %02u", op, opParam);
      }
      else if(opParam == INDIRECT_REGISTER) {
        getIndirectRegister(paramAddress, op);
      }
      else if(opParam == INDIRECT_VARIABLE) {
        getIndirectVariable(paramAddress, op);
      }
      else {
        sprintf(tmpString, "\nIn function decodeOp: case PARAM_NUMBER, %s  %u is not a valid parameter!", op, opParam);
      }
      break;

    case PARAM_NUMBER_16:
      sprintf(tmpString, "%s %u", op, opParam + 256 * *(paramAddress));
      break;

    case PARAM_COMPARE:
      if(opParam < REGISTER_X) { // Global register from 00 to 99
        sprintf(tmpString, "%s %02u", op, opParam);
      }
      else if(opParam <= REGISTER_K) { // Lettered register from X to K
        sprintf(tmpString, "%s %s", op, indexOfItems[ITM_REG_X + opParam - REGISTER_X].itemSoftmenuName);
      }
      else if(opParam <= LAST_LOCAL_REGISTER) { // Local register from .00 to .98
        sprintf(tmpString, "%s .%02d", op, opParam - FIRST_LOCAL_REGISTER);
      }
      else if(opParam == STRING_LABEL_VARIABLE) {
        getStringLabelOrVariableName(paramAddress);
        sprintf(tmpString, "%s " STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, op, tmpStringLabelOrVariableName);
      }
      else if(opParam == VALUE_0) {
        sprintf(tmpString, "%s 0.", op);
      }
      else if(opParam == VALUE_1) {
        sprintf(tmpString, "%s 1.", op);
      }
      else if(opParam == INDIRECT_REGISTER) {
        getIndirectRegister(paramAddress, op);
      }
      else if(opParam == INDIRECT_VARIABLE) {
        getIndirectVariable(paramAddress, op);
      }
      else {
        sprintf(tmpString, "\nIn function decodeOp: case PARAM_COMPARE, %s  %u is not a valid parameter!", op, opParam);
      }
      break;

    case PARAM_KEYG_KEYX:
      {
        uint8_t *secondParam = findKey2ndParam(paramAddress - 3);
        decodeOp(secondParam + 1, indexOfItems[*secondParam].itemCatalogName, PARAM_LABEL);
        xcopy(tmpString + TMP_STR_LENGTH / 2, tmpString, stringByteLength(tmpString) + 1);
        decodeOp(paramAddress - 1, op, PARAM_NUMBER_8);
        tmpString[stringByteLength(tmpString) + 1] = 0;
        tmpString[stringByteLength(tmpString)    ] = ' ';
        xcopy(tmpString + stringByteLength(tmpString), tmpString + TMP_STR_LENGTH / 2, stringByteLength(tmpString + TMP_STR_LENGTH / 2) + 1);
      }
      break;

    default:
      sprintf(tmpString, "\nIn function decodeOp: paramMode %u is not valid!\n", paramMode);
  }
}


static void decodeLiteral(uint8_t *literalAddress) {
  switch(*(uint8_t *)(literalAddress++)) {
    case BINARY_SHORT_INTEGER:
      reallocateRegister(TEMP_REGISTER_1, dtShortInteger, SHORT_INTEGER_SIZE, *(uint8_t *)(literalAddress++));
      xcopy(REGISTER_DATA(TEMP_REGISTER_1), literalAddress, TO_BYTES(SHORT_INTEGER_SIZE));
      shortIntegerToDisplayString(TEMP_REGISTER_1, tmpString, false);
      break;

    //case BINARY_LONG_INTEGER:
    //  break;

    case BINARY_REAL34:
      real34ToDisplayString((real34_t *)literalAddress, amNone, tmpString, &standardFont, 9999, 34, false, STD_SPACE_4_PER_EM, false);
      break;

    case BINARY_COMPLEX34:
      complex34ToDisplayString((complex34_t *)literalAddress, tmpString, &standardFont, 9999, 34, false, STD_SPACE_4_PER_EM, false);
      break;

    //case BINARY_DATE:
    //  break;

    //case BINARY_TIME:
    //  break;

    case STRING_SHORT_INTEGER:
      getStringLabelOrVariableName(literalAddress + 1);
      sprintf(tmpString, "%s", tmpStringLabelOrVariableName);
      sprintf(tmpString + strlen(tmpString), "#%u", *literalAddress);
      break;

    case STRING_LONG_INTEGER:
      getStringLabelOrVariableName(literalAddress);
      sprintf(tmpString, "%s", tmpStringLabelOrVariableName);
      break;

    case STRING_REAL34:
      getStringLabelOrVariableName(literalAddress);
      sprintf(tmpString, "%s", tmpStringLabelOrVariableName);
      if(strchr(tmpString, 'e') == NULL && strchr(tmpString, '.') == NULL) {
        strcat(tmpString, RADIX34_MARK_STRING);
      }
      break;

    case STRING_COMPLEX34:
      getStringLabelOrVariableName(literalAddress);
      xcopy(tmpString, tmpStringLabelOrVariableName, strlen(tmpStringLabelOrVariableName) + 1);
      break;

    case STRING_LABEL_VARIABLE:
      getStringLabelOrVariableName(literalAddress);
      sprintf(tmpString, STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, tmpStringLabelOrVariableName);
      break;

    case STRING_DATE:
      getStringLabelOrVariableName(literalAddress);
      reallocateRegister(TEMP_REGISTER_1, dtDate, REAL34_SIZE, amNone);
      stringToReal34(tmpStringLabelOrVariableName, REGISTER_REAL34_DATA(TEMP_REGISTER_1));
      julianDayToInternalDate(REGISTER_REAL34_DATA(TEMP_REGISTER_1), REGISTER_REAL34_DATA(TEMP_REGISTER_1));
      dateToDisplayString(TEMP_REGISTER_1, tmpString);
      break;

    case STRING_TIME:
      {
        char *timeStringPtr = tmpString;
        char *sourceStringPtr = tmpStringLabelOrVariableName;
        getStringLabelOrVariableName(literalAddress);
        for(; *sourceStringPtr != '.' && *sourceStringPtr != 0; ++sourceStringPtr) {
          *(timeStringPtr++) = *sourceStringPtr;
        }
        if(*sourceStringPtr == '.') ++sourceStringPtr;
        *(timeStringPtr++) = ':';
        if(*sourceStringPtr != 0) {*(timeStringPtr++) = *(sourceStringPtr++);} else {*(timeStringPtr++) = '0';}
        if(*sourceStringPtr != 0) {*(timeStringPtr++) = *(sourceStringPtr++);} else {*(timeStringPtr++) = '0';}
        *(timeStringPtr++) = ':';
        if(*sourceStringPtr != 0) {*(timeStringPtr++) = *(sourceStringPtr++);} else {*(timeStringPtr++) = '0';}
        if(*sourceStringPtr != 0) {*(timeStringPtr++) = *(sourceStringPtr++);} else {*(timeStringPtr++) = '0';}
        if(*sourceStringPtr != 0) {*(timeStringPtr++) = '.';}
        for(; *sourceStringPtr != 0; ++sourceStringPtr) {
          *(timeStringPtr++) = *sourceStringPtr;
        }
        *(timeStringPtr++) = 0;
      }
      break;

    default: {
      #ifndef DMCP_BUILD
        printf("\nERROR: %u is not an acceptable parameter for ITM_LITERAL!\n", *(uint8_t *)(literalAddress - 1));
      #endif // !DMCP_BUILD
    }
  }
}


void decodeOneStep(uint8_t *step) {
  uint16_t op = *(step++);
  if(op & 0x80) {
    op &= 0x7f;
    op <<= 8;
    op |= *(step++);
  }

  if(op == 0x7fff) { // .END.
    xcopy(tmpString, ".END.", 6);
  }
  else {
    switch(indexOfItems[op].status & PTP_STATUS) {
      case PTP_NONE:
        sprintf(tmpString, "%s%s", (CST_01 <= op && op <= CST_79) ? "# " : "", indexOfItems[op].itemCatalogName);
        break;

      case PTP_DISABLED:
        printf("\nERROR in decodeOneStep: instruction %u is not programmable!\n", op);
        break;

      case PTP_LITERAL:
        decodeLiteral(step);
        break;

      default:
        decodeOp(step, indexOfItems[op].itemCatalogName, (indexOfItems[op].status & PTP_STATUS) >> 9);
    }
  }
}
