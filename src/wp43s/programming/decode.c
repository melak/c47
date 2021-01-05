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
        else if(*(labelList[i].labelPointer) < 110) {
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


void getStringLabelOrVariableName(uint8_t *stringAddress) {
  uint8_t stringLength = *(uint8_t *)(stringAddress++);
  xcopy(tmpString + 1000, stringAddress, stringLength);
  tmpString[1000 + stringLength] = 0;
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
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wrestrict"
  sprintf(tmpString, "%s " STD_RIGHT_ARROW STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, op, tmpString + 1000);
  #pragma GCC diagnostic pop
}


void decodeOp(uint8_t *paramAddress, const char *op, uint16_t paramMode) {
  uint8_t opParam = *(uint8_t *)(paramAddress++);

  switch(paramMode) {
    case PARAM_DECLARE_LABEL:
      if(opParam <= 99) { // Local label from 00 to 99
        sprintf(tmpString, "%s %02u", op, opParam);
      }
      else if(opParam <= 109) { // Local label from A to J
        sprintf(tmpString, "%s %c", op, 'A' + (opParam - 100));
      }
      else if(opParam == STRING_LABEL_VARIABLE) {
        getStringLabelOrVariableName(paramAddress);
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wrestrict"
        sprintf(tmpString, "%s " STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, op, tmpString + 1000);
        #pragma GCC diagnostic pop
      }
      else {
        sprintf(tmpString, "\nIn function decodeOp case PARAM_DECLARE_LABEL: opParam %u is not a valid label!\n", opParam);
      }
      break;

    case PARAM_LABEL:
      if(opParam <= 99) { // Local label from 00 to 99
        sprintf(tmpString, "%s %02u", op, opParam);
      }
      else if(opParam <= 109) { // Local label from A to J
        sprintf(tmpString, "%s %c", op, 'A' + (opParam - 100));
      }
      else if(opParam == STRING_LABEL_VARIABLE) {
        getStringLabelOrVariableName(paramAddress);
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wrestrict"
        sprintf(tmpString, "%s " STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, op, tmpString + 1000);
        #pragma GCC diagnostic pop
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
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wrestrict"
        sprintf(tmpString, "%s " STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, op, tmpString + 1000);
        #pragma GCC diagnostic pop
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
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wrestrict"
        sprintf(tmpString, "%s " STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, op, tmpString + 1000);
        #pragma GCC diagnostic pop
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
      real34ToDisplayString((real34_t *)literalAddress, AM_NONE, tmpString, &standardFont, 9999, 34, false, STD_SPACE_4_PER_EM);
      break;

    case BINARY_COMPLEX34:
      complex34ToDisplayString((complex34_t *)literalAddress, tmpString, &standardFont, 9999, 34, false, STD_SPACE_4_PER_EM);
      break;

    //case BINARY_DATE:
    //  break;

    //case BINARY_TIME:
    //  break;

    case STRING_SHORT_INTEGER:
      getStringLabelOrVariableName(literalAddress + 1);
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wrestrict"
      sprintf(tmpString, "%s", tmpString + 1000);
      #pragma GCC diagnostic pop
      sprintf(tmpString + strlen(tmpString), "#%u", *literalAddress);
      break;

    case STRING_LONG_INTEGER:
      getStringLabelOrVariableName(literalAddress);
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wrestrict"
      sprintf(tmpString, "%s", tmpString + 1000);
      #pragma GCC diagnostic pop
      break;

    case STRING_REAL34:
      getStringLabelOrVariableName(literalAddress);
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wrestrict"
      sprintf(tmpString, "%s", tmpString + 1000);
      #pragma GCC diagnostic pop
      if(strchr(tmpString, 'e') == NULL && strchr(tmpString, '.') == NULL) {
        strcat(tmpString, RADIX34_MARK_STRING);
      }
      break;

    case STRING_COMPLEX34:
      getStringLabelOrVariableName(literalAddress);
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wrestrict"
      xcopy(tmpString, tmpString + 1000, strlen(tmpString + 1000) + 1);
      #pragma GCC diagnostic pop
      break;

    case STRING_LABEL_VARIABLE:
      getStringLabelOrVariableName(literalAddress);
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wrestrict"
      sprintf(tmpString, STD_LEFT_SINGLE_QUOTE "%s" STD_RIGHT_SINGLE_QUOTE, tmpString + 1000);
      #pragma GCC diagnostic pop
      break;

    //case STRING_DATE:
    //  break;

    //case STRING_TIME:
    //  break;

    default: {
      #ifndef DMCP_BUILD
        printf("\nERROR: %u is not an acceptable parameter for ITM_LITERAL!\n", *(uint8_t *)(literalAddress - 1));
      #endif // !DMCP_BUILD
    }
  }
}


void decodeOneStep(uint8_t *step) {
  uint8_t item8 = *(uint8_t *)(step++);
  uint16_t item16;

  switch(item8) {
    case ITM_LBL:         //   1
      decodeOp(step, indexOfItems[item8].itemCatalogName, PARAM_DECLARE_LABEL);
      break;

    case ITM_GTO:         //   2
    case ITM_XEQ:         //   3
      decodeOp(step, indexOfItems[item8].itemCatalogName, PARAM_LABEL);
      break;

    case ITM_PAUSE:       //  38
      decodeOp(step, indexOfItems[item8].itemCatalogName, PARAM_NUMBER_8);
      break;

    case ITM_ISE:         //   5
    case ITM_ISG:         //   6
    case ITM_ISZ:         //   7
    case ITM_DSE:         //   8
    case ITM_DSL:         //   9
    case ITM_DSZ:         //  10
    case ITM_INPUT:       //  43
    case ITM_STO:         //  44
    case ITM_STOADD:      //  45
    case ITM_STOSUB:      //  46
    case ITM_STOMULT:     //  47
    case ITM_STODIV:      //  48
    case ITM_RCL:         //  51
    case ITM_RCLADD:      //  52
    case ITM_RCLSUB:      //  53
    case ITM_RCLMULT:     //  54
    case ITM_RCLDIV:      //  55
    case ITM_CONVG:       //  56
    case ITM_KEYQ:        //  77
    case ITM_DEC:         //  91
    case ITM_INC:         //  92
    case ITM_VIEW:        // 101
    case ITM_Xex:         // 127
      decodeOp(step, indexOfItems[item8].itemCatalogName, PARAM_REGISTER);
      break;

    case ITM_XEQU:        //  11
    case ITM_XNE:         //  12
    case ITM_XAEQU:       //  15
    case ITM_XLT:         //  16
    case ITM_XLE:         //  17
    case ITM_XGE:         //  18
    case ITM_XGT:         //  19
      decodeOp(step, indexOfItems[item8].itemCatalogName, PARAM_COMPARE);
      break;

    case ITM_FC:          //  20
    case ITM_FS:          //  21
    case ITM_CF:          // 110
    case ITM_SF:          // 111
    case ITM_FF:          // 112
      decodeOp(step, indexOfItems[item8].itemCatalogName, PARAM_FLAG);
      break;

    case ITM_RTN:         //   4
    case ITM_XEQUP0:      //  13
    case ITM_XEQUM0:      //  14
    case ITM_EVEN:        //  22
    case ITM_ODD:         //  23
    case ITM_FPQ:         //  24
    case ITM_INT:         //  25
    case ITM_CPX:         //  26
    case ITM_MATR:        //  27
    case ITM_NAN:         //  28
    case ITM_REAL:        //  29
    case ITM_SPEC:        //  30
    case ITM_STRI:        //  31
    case ITM_PMINFINITY:  //  32
    case ITM_PRIME:       //  33
    case ITM_TOP:         //  34
    case ITM_ENTER:       //  35
    case ITM_XexY:        //  36
    case ITM_DROP:        //  37
    case ITM_Rup:         //  39
    case ITM_Rdown:       //  40
    case ITM_CLX:         //  41
    case ITM_FILL:        //  42
    case ITM_COMB:        //  49
    case ITM_PERM:        //  50
    case ITM_ENTRY:       //  57
    case ITM_SQUARE:      //  58
    case ITM_CUBE:        //  59
    case ITM_YX:          //  60
    case ITM_SQUAREROOTX: //  61
    case ITM_CUBEROOT:    //  62
    case ITM_XTHROOT:     //  63
    case ITM_2X:          //  64
    case ITM_EXP:         //  65
    case ITM_ROUND:       //  66
    case ITM_10x:         //  67
    case ITM_LOG2:        //  68
    case ITM_LN:          //  69
    case ITM_STOP:        //  70
    case ITM_LOG10:       //  71
    case ITM_LOGXY:       //  72
    case ITM_1ONX:        //  73
    case ITM_cos:         //  74
    case ITM_cosh:        //  75
    case ITM_sin:         //  76
    case ITM_sinh:        //  78
    case ITM_tan:         //  79
    case ITM_tanh:        //  80
    case ITM_arccos:      //  81
    case ITM_arcosh:      //  82
    case ITM_arcsin:      //  83
    case ITM_arsinh:      //  84
    case ITM_arctan:      //  85
    case ITM_artanh:      //  86
    case ITM_CEIL:        //  87
    case ITM_FLOOR:       //  88
    case ITM_GCD:         //  89
    case ITM_LCM:         //  90
    case ITM_IP:          //  93
    case ITM_FP:          //  94
    case ITM_ADD:         //  95
    case ITM_SUB:         //  96
    case ITM_CHS:         //  97
    case ITM_MULT:        //  98
    case ITM_DIV:         //  99
    case ITM_IDIV:        // 100
    case ITM_MOD:         // 102
    case ITM_MAX:         // 103
    case ITM_MIN:         // 104
    case ITM_MAGNITUDE:   // 105
    case ITM_NEIGHB:      // 106
    case ITM_NEXTP:       // 107
    case ITM_XFACT:       // 108
    case ITM_CONSTpi:     // 109
    case ITM_M_SQR:       // 113
    case ITM_toDEG:       // 115
    case ITM_toDMS:       // 116
    case ITM_toGRAD:      // 117
    case ITM_toMULpi:     // 118
    case ITM_toRAD:       // 119
    case ITM_DtoR:        // 120
    case ITM_RtoD:        // 121
    case ITM_RMD:         // 122
    case ITM_LOGICALNOT:  // 123
    case ITM_LOGICALAND:  // 124
    case ITM_LOGICALOR:   // 125
    case ITM_LOGICALXOR:  // 126
      sprintf(tmpString, "%s", indexOfItems[item8].itemCatalogName);
      break;

    case ITM_LITERAL:     // 114
       decodeLiteral(step);
       break;

    default:
      if((item8 & 0x80) == 0) {
        #ifndef DMCP_BUILD
          printf("\nERROR in decodeOneStep: single byte instruction %u is unknown!\n", item8);
        #endif // !DMCP_BUILD
        break;
      }

      item16 = ((uint16_t)(item8 & 0x7F) << 8) | *(uint8_t *)(step++);
      switch(item16) {
        case ITM_LBLQ:        //  1493
          decodeOp(step, indexOfItems[item8].itemCatalogName, PARAM_LABEL);
          break;

        case ITM_CNST:        //   207
        case ITM_RL:          //   400
        case ITM_RLC:         //   401
        case ITM_RR:          //   402
        case ITM_RRC:         //   403
        case ITM_SL:          //   404
        case ITM_SR:          //   405
        case ITM_ASR:         //   406
        case ITM_MASKL:       //   409
        case ITM_MASKR:       //   410
        case ITM_SDL:         //   413
        case ITM_SDR:         //   414
        case ITM_AGRAPH:      //  1399
        case ITM_ALL:         //  1400
        case ITM_BACK:        //  1402
        case ITM_DSTACK:      //  1440
        case ITM_ENG:         //  1450
        case ITM_ERR:         //  1458
        case ITM_FIX:         //  1463
        case ITM_GAP:         //  1467
        case ITM_KEY:         //  1487
        case ITM_LocR:        //  1504
        case ITM_SCI:         //  1577
          decodeOp(step, indexOfItems[item16].itemCatalogName, PARAM_NUMBER_8);
          break;

        case ITM_BESTF:       //  1287
          decodeOp(step, indexOfItems[item16].itemCatalogName, PARAM_NUMBER_16);
          step++;
          break;

        case ITM_CASE:        //  1408
        case ITM_STOMAX:      //  1420
        case ITM_RCLMAX:      //  1422
        case ITM_RCLMIN:      //  1452
        case ITM_KTYP:        //  1491
        case ITM_STOMIN:      //  1535
        case ITM_VIEW:        //  1622
        case ITM_Xex:         //  1636
          decodeOp(step, indexOfItems[item16].itemCatalogName, PARAM_REGISTER);
          break;

        case ITM_FCC:         //   386
        case ITM_FCS:         //   387
        case ITM_FCF:         //   388
        case ITM_FSC:         //   389
        case ITM_FSS:         //   390
        case ITM_FSF:         //   391
        case ITM_BS:          //   395
        case ITM_BC:          //   396
        case ITM_CB:          //   397
        case ITM_SB:          //   398
        case ITM_FB:          //   399
          decodeOp(step, indexOfItems[item16].itemCatalogName, PARAM_FLAG);
          break;

        case CST_01:          //   128
        case CST_02:          //   129
        case CST_03:          //   130
        case CST_04:          //   131
        case CST_05:          //   132
        case CST_06:          //   133
        case CST_07:          //   134
        case CST_08:          //   135
        case CST_09:          //   136
        case CST_10:          //   137
        case CST_11:          //   138
        case CST_12:          //   139
        case CST_13:          //   140
        case CST_14:          //   141
        case CST_15:          //   142
        case CST_16:          //   143
        case CST_17:          //   144
        case CST_18:          //   145
        case CST_19:          //   146
        case CST_20:          //   147
        case CST_21:          //   148
        case CST_22:          //   149
        case CST_23:          //   150
        case CST_24:          //   151
        case CST_25:          //   152
        case CST_26:          //   153
        case CST_27:          //   154
        case CST_28:          //   155
        case CST_29:          //   156
        case CST_30:          //   157
        case CST_31:          //   158
        case CST_32:          //   159
        case CST_33:          //   160
        case CST_34:          //   161
        case CST_35:          //   162
        case CST_36:          //   163
        case CST_37:          //   164
        case CST_38:          //   165
        case CST_39:          //   166
        case CST_40:          //   167
        case CST_41:          //   168
        case CST_42:          //   169
        case CST_43:          //   170
        case CST_44:          //   171
        case CST_45:          //   172
        case CST_46:          //   173
        case CST_47:          //   174
        case CST_48:          //   175
        case CST_49:          //   176
        case CST_50:          //   177
        case CST_51:          //   178
        case CST_52:          //   179
        case CST_53:          //   180
        case CST_54:          //   181
        case CST_55:          //   182
        case CST_56:          //   183
        case CST_57:          //   184
        case CST_58:          //   185
        case CST_59:          //   186
        case CST_60:          //   187
        case CST_61:          //   188
        case CST_62:          //   189
        case CST_63:          //   190
        case CST_64:          //   191
        case CST_65:          //   192
        case CST_66:          //   193
        case CST_67:          //   194
        case CST_68:          //   195
        case CST_69:          //   196
        case CST_70:          //   197
        case CST_71:          //   198
        case CST_72:          //   199
        case CST_73:          //   200
        case CST_74:          //   201
        case CST_75:          //   202
        case CST_76:          //   203
        case CST_77:          //   204
        case CST_78:          //   205
        case CST_79:          //   206
        case ITM_CtoF:        //   220
        case ITM_FtoC:        //   221
        case ITM_DBtoPR:      //   222
        case ITM_DBtoPRb:     //   223
        case ITM_DBtoPRc:     //   224
        case ITM_DBtoFR:      //   225
        case ITM_DBtoFRb:     //   226
        case ITM_DBtoFRc:     //   227
        case ITM_PRtoDB:      //   228
        case ITM_PRtoDBb:     //   229
        case ITM_PRtoDBc:     //   230
        case ITM_FRtoDB:      //   231
        case ITM_FRtoDBb:     //   232
        case ITM_FRtoDBc:     //   233
        case ITM_ACtoM2:      //   234
        case ITM_ACtoM2b:     //   235
        case ITM_M2toAC:      //   236
        case ITM_M2toACb:     //   237
        case ITM_ACUStoM2:    //   238
        case ITM_ACUStoM2b:   //   239
        case ITM_M2toACUS:    //   240
        case ITM_M2toACUSb:   //   241
        case ITM_PAtoATM:     //   242
        case ITM_ATMtoPA:     //   243
        case ITM_AUtoM:       //   244
        case ITM_MtoAU:       //   245
        case ITM_BARtoPA:     //   246
        case ITM_PAtoBAR:     //   247
        case ITM_BTUtoJ:      //   248
        case ITM_JtoBTU:      //   249
        case ITM_CALtoJ:      //   250
        case ITM_JtoCAL:      //   251
        case ITM_LBFFTtoNM:   //   252
        case ITM_LBFFTtoNMb:  //   253
        case ITM_NMtoLBFFT:   //   254
        case ITM_NMtoLBFFTb:  //   255
        case ITM_CWTtoKG:     //   256
        case ITM_KGtoCWT:     //   257
        case ITM_FTtoM:       //   258
        case ITM_MtoFT:       //   259
        case ITM_FTUStoM:     //   260
        case ITM_FTUStoMb:    //   261
        case ITM_FTUStoMc:    //   262
        case ITM_MtoFTUS:     //   263
        case ITM_MtoFTUSb:    //   264
        case ITM_MtoFTUSc:    //   265
        case ITM_FZUKtoM3:    //   266
        case ITM_FZUKtoM3b:   //   267
        case ITM_M3toFZUK:    //   268
        case ITM_M3toFZUKb:   //   269
        case ITM_FZUStoM3:    //   270
        case ITM_FZUStoM3b:   //   271
        case ITM_M3toFZUS:    //   272
        case ITM_M3toFZUSb:   //   273
        case ITM_GLUKtoM3:    //   274
        case ITM_M3toGLUK:    //   275
        case ITM_GLUStoM3:    //   276
        case ITM_M3toGLUS:    //   277
        case ITM_HPEtoW:      //   278
        case ITM_WtoHPE:      //   279
        case ITM_HPMtoW:      //   280
        case ITM_WtoHPM:      //   281
        case ITM_HPUKtoW:     //   282
        case ITM_WtoHPUK:     //   283
        case ITM_IHGtoPA:     //   284
        case ITM_IHGtoPAb:    //   285
        case ITM_PAtoIHG:     //   286
        case ITM_PAtoIHGb:    //   287
        case ITM_INtoM:       //   288
        case ITM_MtoINCH:     //   289
        case ITM_WHtoJ:       //   290
        case ITM_JtoWH:       //   291
        case ITM_KGtoLBS:     //   292
        case ITM_LBStoKG:     //   293
        case ITM_KGtoOZ:      //   294
        case ITM_OZtoKG:      //   295
        case ITM_KGtoSCW:     //   296
        case ITM_KGtoSCWb:    //   297
        case ITM_SCWtoKG:     //   298
        case ITM_SCWtoKGb:    //   299
        case ITM_KGtoSTO:     //   300
        case ITM_KGtoSTOb:    //   301
        case ITM_STOtoKG:     //   302
        case ITM_STOtoKGb:    //   303
        case ITM_KGtoST:      //   304
        case ITM_KGtoSTb:     //   305
        case ITM_KGtoSTc:     //   306
        case ITM_STtoKG:      //   307
        case ITM_STtoKGb:     //   308
        case ITM_STtoKGc:     //   309
        case ITM_KGtoTON:     //   310
        case ITM_KGtoTONb:    //   311
        case ITM_KGtoTONc:    //   312
        case ITM_TONtoKG:     //   313
        case ITM_TONtoKGb:    //   314
        case ITM_TONtoKGc:    //   315
        case ITM_KGtoTRZ:     //   316
        case ITM_KGtoTRZb:    //   317
        case ITM_TRZtoKG:     //   318
        case ITM_TRZtoKGb:    //   319
        case ITM_LBFtoN:      //   320
        case ITM_NtoLBF:      //   321
        case ITM_LYtoM:       //   322
        case ITM_MtoLY:       //   323
        case ITM_MMHGtoPA:    //   324
        case ITM_MMHGtoPAb:   //   325
        case ITM_PAtoMMHG:    //   326
        case ITM_PAtoMMHGb:   //   327
        case ITM_MItoM:       //   328
        case ITM_MtoMI:       //   329
        case ITM_MtoNMI:      //   330
        case ITM_NMItoM:      //   331
        case ITM_MtoPC:       //   332
        case ITM_PCtoM:       //   333
        case ITM_MtoPOINTb:   //   334
        case ITM_MtoPOINT:    //   335
        case ITM_MtoPOINTc:   //   336
        case ITM_POINTtoM:    //   337
        case ITM_POINTtoMb:   //   338
        case ITM_POINTtoMc:   //   339
        case ITM_MtoYD:       //   340
        case ITM_YDtoM:       //   341
        case ITM_PSItoPA:     //   342
        case ITM_PAtoPSI:     //   343
        case ITM_PAtoTOR:     //   344
        case ITM_PAtoTORb:    //   345
        case ITM_TORtoPA:     //   346
        case ITM_TORtoPAb:    //   347
        case ITM_StoYEAR:     //   348
        case ITM_YEARtoS:     //   349
        case ITM_CARATtoKG:   //   350
        case ITM_CARATtoKGb:  //   351
        case ITM_CARATtoKGc:  //   352
        case ITM_KGtoCARAT:   //   353
        case ITM_KGtoCARATb:  //   354
        case ITM_KGtoCARATc:  //   355
        case ITM_QTtoM3:      //   356
        case ITM_M3toQT:      //   357
        case ITM_FATHOMtoM:   //   358
        case ITM_FATHOMtoMb:  //   359
        case ITM_FATHOMtoMc:  //   360
        case ITM_MtoFATHOM:   //   361
        case ITM_MtoFATHOMb:  //   362
        case ITM_MtoFATHOMc:  //   363
        case ITM_BARRELtoM3:  //   364
        case ITM_BARRELtoM3b: //   365
        case ITM_M3toBARREL:  //   366
        case ITM_M3toBARRELb: //   367
        case ITM_ATMtoPAb:    //   368
        case ITM_PAtoATMb:    //   369
        case ITM_HECTAREtoM2: //   370
        case ITM_M2toHECTARE: //   371
        case ITM_MUtoM2:      //   372
        case ITM_M2toMU:      //   373
        case ITM_LItoM:       //   374
        case ITM_MtoLI:       //   375
        case ITM_CHItoM:      //   376
        case ITM_MtoCHI:      //   377
        case ITM_YINtoM:      //   378
        case ITM_MtoYIN:      //   379
        case ITM_CUNtoM:      //   380
        case ITM_MtoCUN:      //   381
        case ITM_ZHANGtoM:    //   382
        case ITM_MtoZHANG:    //   383
        case ITM_FENtoM:      //   384
        case ITM_MtoFEN:      //   385
        case ITM_LOGICALNAND: //   392
        case ITM_LOGICALNOR:  //   393
        case ITM_LOGICALXNOR: //   394
        case ITM_LJ:          //   407
        case ITM_RJ:          //   408
        case ITM_MIRROR:      //   411
        case ITM_NUMB:        //   412
        case ITM_SIGMAPLUS:   //   423
        case ITM_SIGMAMINUS:  //   424
        case ITM_NSIGMA:      //   425
        case ITM_SIGMAx:      //   426
        case ITM_SIGMAy:      //   427
        case ITM_SIGMAx2:     //   428
        case ITM_SIGMAx2y:    //   429
        case ITM_SIGMAy2:     //   430
        case ITM_SIGMAxy:     //   431
        case ITM_SIGMAlnxy:   //   432
        case ITM_SIGMAlnx:    //   433
        case ITM_SIGMAln2x:   //   434
        case ITM_SIGMAylnx:   //   435
        case ITM_SIGMAlny:    //   436
        case ITM_SIGMAln2y:   //   437
        case ITM_SIGMAxlny:   //   438
        case ITM_SIGMAlnyonx: //   439
        case ITM_SIGMAx2ony:  //   440
        case ITM_SIGMA1onx:   //   441
        case ITM_SIGMA1onx2:  //   442
        case ITM_SIGMAxony:   //   443
        case ITM_SIGMA1ony:   //   444
        case ITM_SIGMA1ony2:  //   445
        case ITM_SIGMAx3:     //   446
        case ITM_SIGMAx4:     //   447
        case ITM_BINOMP:      //  1198
        case ITM_BINOM:       //  1199
        case ITM_BINOMU:      //  1200
        case ITM_BINOMM1:     //  1201
        case ITM_CAUCHP:      //  1203
        case ITM_CAUCH:       //  1204
        case ITM_CAUCHU:      //  1205
        case ITM_CAUCHM1:     //  1206
        case ITM_EXPONP:      //  1208
        case ITM_EXPON:       //  1209
        case ITM_EXPONU:      //  1210
        case ITM_EXPONM1:     //  1211
        case ITM_FPX:         //  1213
        case ITM_FX:          //  1214
        case ITM_FUX:         //  1215
        case ITM_FM1P:        //  1216
        case ITM_GEOMP:       //  1218
        case ITM_GEOM:        //  1219
        case ITM_GEOMU:       //  1220
        case ITM_GEOMM1:      //  1221
        case ITM_HYPERP:      //  1223
        case ITM_HYPER:       //  1224
        case ITM_HYPERU:      //  1225
        case ITM_HYPERM1:     //  1226
        case ITM_LGNRMP:      //  1228
        case ITM_LGNRM:       //  1229
        case ITM_LGNRMU:      //  1230
        case ITM_LGNRMM1:     //  1231
        case ITM_LOGISP:      //  1233
        case ITM_LOGIS:       //  1234
        case ITM_LOGISU:      //  1235
        case ITM_LOGISM1:     //  1236
        case ITM_NBINP:       //  1238
        case ITM_NBIN:        //  1239
        case ITM_NBINU:       //  1240
        case ITM_NBINM1:      //  1241
        case ITM_NORMLP:      //  1243
        case ITM_NORML:       //  1244
        case ITM_NORMLU:      //  1245
        case ITM_NORMLM1:     //  1246
        case ITM_POISSP:      //  1248
        case ITM_POISS:       //  1249
        case ITM_POISSU:      //  1250
        case ITM_POISSM1:     //  1251
        case ITM_TPX:         //  1253
        case ITM_TX:          //  1254
        case ITM_TUX:         //  1255
        case ITM_TM1P:        //  1256
        case ITM_WEIBLP:      //  1258
        case ITM_WEIBL:       //  1259
        case ITM_WEIBLU:      //  1260
        case ITM_WEIBLM1:     //  1261
        case ITM_chi2Px:      //  1263
        case ITM_chi2x:       //  1264
        case ITM_chi2ux:      //  1265
        case ITM_chi2M1:      //  1266
        case ITM_EXPF:        //  1288
        case ITM_LINF:        //  1289
        case ITM_LOGF:        //  1290
        case ITM_ORTHOF:      //  1291
        case ITM_POWERF:      //  1292
        case ITM_GAUSSF:      //  1293
        case ITM_CAUCHF:      //  1294
        case ITM_PARABF:      //  1295
        case ITM_HYPF:        //  1296
        case ITM_ROOTF:       //  1297
        case ITM_1COMPL:      //  1394
        case ITM_SNAP:        //  1395
        case ITM_2COMPL:      //  1396
        case ITM_ABS:         //  1397
        case ITM_AGM:         //  1398
        case ITM_BATT:        //  1403
        case ITM_BEEP:        //  1404
        case ITM_BEGINP:      //  1405
        case ITM_BN:          //  1406
        case ITM_BNS:         //  1407
        case ITM_CLCVAR:      //  1410
        case ITM_CLFALL:      //  1411
        case ITM_CLLCD:       //  1413
        case ITM_CLMENU:      //  1414
        case ITM_CLREGS:      //  1417
        case ITM_CLSTK:       //  1418
        case ITM_CLSIGMA:     //  1419
        case ITM_CONJ:        //  1421
        case ITM_CORR:        //  1423
        case ITM_COV:         //  1424
        case ITM_BESTFQ:      //  1425
        case ITM_CXtoRE:      //  1427
        case ITM_DATE:        //  1428
        case ITM_DATEto:      //  1429
        case ITM_DAY:         //  1430
        case ITM_DBLR:        //  1431
        case ITM_DBLMULT:     //  1432
        case ITM_DBLDIV:      //  1433
        case ITM_DECOMP:      //  1434
        case ITM_DEG:         //  1435
        case ITM_DEGto:       //  1436
        case ITM_SA:          //  1437
        case ITM_DENMAX:      //  1438
        case ITM_DOT_PROD:    //  1439
        case ITM_DMS:         //  1441
        case ITM_DMSto:       //  1442
        case ITM_DMY:         //  1443
        case ITM_DtoJ:        //  1444
        case ITM_EIGVAL:      //  1446
        case ITM_EIGVEC:      //  1447
        case ITM_END:         //  1448
        case ITM_ENDP:        //  1449
        case ITM_ENORM:       //  1451
        case ITM_EQ_DEL:      //  1453
        case ITM_EQ_EDI:      //  1454
        case ITM_EQ_NEW:      //  1455
        case ITM_ERF:         //  1456
        case ITM_ERFC:        //  1457
        case ITM_EXITALL:     //  1459
        case ITM_EXPT:        //  1460
        case ITM_FIB:         //  1462
        case ITM_GD:          //  1468
        case ITM_GDM1:        //  1469
        case ITM_GRAD:        //  1470
        case ITM_GRADto:      //  1471
        case ITM_HN:          //  1473
        case ITM_HNP:         //  1474
        case ITM_IM:          //  1475
        case ITM_IXYZ:        //  1477
        case ITM_IGAMMAP:     //  1478
        case ITM_IGAMMAQ:     //  1479
        case ITM_JYX:         //  1482
        case ITM_JUL_GREG:    //  1485
        case ITM_JtoD:        //  1486
        case ITM_sinc:        //  1490
        case ITM_LASTX:       //  1492
        case ITM_LEAP:        //  1494
        case ITM_Lm:          //  1495
        case ITM_LmALPHA:     //  1496
        case ITM_LNBETA:      //  1497
        case ITM_LNGAMMA:     //  1498
        case ITM_LOADR:       //  1501
        case ITM_LOADSS:      //  1502
        case ITM_LOADSIGMA:   //  1503
        case ITM_LocRQ:       //  1505
        case ITM_LR:          //  1506
        case ITM_MANT:        //  1507
        case ITM_MEM:         //  1509
        case ITM_MENU:        //  1510
        case ITM_MONTH:       //  1511
        case ITM_MSG:         //  1512
        case ITM_MULPI:       //  1513
        case ITM_M_DELR:      //  1515
        case ITM_M_DIMQ:      //  1517
        case ITM_MDY:         //  1518
        case ITM_M_GET:       //  1521
        case ITM_sincpi:      //  1530
        case ITM_NOP:         //  1532
        case ITM_OFF:         //  1533
        case ITM_DROPY:       //  1534
        case ITM_PIXEL:       //  1538
        case ITM_PLOT:        //  1539
        case ITM_Pn:          //  1540
        case ITM_POINT:       //  1541
        case ITM_LOADV:       //  1542
        case ITM_POPLR:       //  1543

        case ITM_RAN:         //  1549
        case ITM_EX1:         //  1565
        case ITM_SIGN:        //  1590
        case ITM_LN1X:        //  1604
        case ITM_TICKS:       //  1610
        case ITM_IDIVR:       //  1622
        case ITM_MULPIto:     //  1697
          sprintf(tmpString, "%s%s", item16 <= CST_79 ? "# " : "", indexOfItems[item16].itemCatalogName);
          break;

        case 0x7fff:          // 32767
          xcopy(tmpString, ".END.", 6);
          break;

        default: {
          #ifndef DMCP_BUILD
            printf("\nERROR in decodeOneStep: double byte instruction %u is unknown!\n", item16);
          #endif // !DMCP_BUILD
        }
      }
  }
}
