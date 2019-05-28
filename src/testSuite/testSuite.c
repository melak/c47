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
 * \file testSuite.c
 ***********************************************/

#include "wp43s.h"
#include "testSuite.h"



char line[10000], lastInParameters[10000], fileName[1000], filePath[1000], filePathName[2000], registerExpectedAndValue[1000];
int32_t lineNumber, numTestsFile, numTestsTotal;
int32_t functionIndex, funcType, correctSignificantDigits, numberOfCorrectSignificantDigitsExpected;
void (*funcNoParam)(uint16_t);
void (*funcCvt)(uint16_t);

const funcTest_t funcTestNoParam[] = {
  {"fn10Pow",                fn10Pow               },
  {"fn2Pow",                 fn2Pow                },
  {"fnAdd",                  fnAdd                 },
  {"fnAim",                  fnAim                 },
  {"fnArccos",               fnArccos              },
  {"fnArcsin",               fnArcsin              },
  {"fnArctan",               fnArctan              },
  {"fnBatteryVoltage",       fnBatteryVoltage      },
  {"fnChangeSign",           fnChangeSign          },
  {"fnClearRegisters",       fnClearRegisters      },
  {"fnClearStack",           fnClearStack          },
  {"fnClFAll",               fnClFAll              },
  {"fnClSigma",              fnClSigma             },
  {"fnClX",                  fnClX                 },
  {"fnComplexCCCC",          fnComplexCCCC         },
  {"fnConfigChina",          fnConfigChina         },
  {"fnConfigEurope",         fnConfigEurope        },
  {"fnConfigIndia",          fnConfigIndia         },
  {"fnConfigJapan",          fnConfigJapan         },
  {"fnConfigUk",             fnConfigUk            },
  {"fnConfigUsa",            fnConfigUsa           },
  {"fnConjugate",            fnConjugate           },
  {"fnConvertXToReal16",     fnConvertXToReal16    },
  {"fnConvertXToReal34",     fnConvertXToReal34    },
  {"fnCos",                  fnCos                 },
  {"fnCube",                 fnCube                },
  {"fnCvtCToF",              fnCvtCToF             },
  {"fnCvtDegToDms",          fnCvtDegToDms         },
  {"fnCvtDegToRad",          fnCvtDegToRad         },
  {"fnCvtDmsToDeg",          fnCvtDmsToDeg         },
  {"fnCvtFToC",              fnCvtFToC             },
  {"fnCvtRadToDeg",          fnCvtRadToDeg         },
  {"fnDenMax",               fnDenMax              },
  {"fnDivide",               fnDivide              },
  {"fnDrop",                 fnDrop                },
  {"fnDropY",                fnDropY               },
  {"fnExp",                  fnExp                 },
  {"fnFactorial",            fnFactorial           },
  {"fnFillStack",            fnFillStack           },
  {"fnFp",                   fnFp                  },
  {"fnFreeFlashMemory",      fnFreeFlashMemory     },
  {"fnFreeMemory",           fnFreeMemory          },
  {"fnGamma",                fnGamma               },
  {"fnGcd",                  fnGcd                 },
  {"fnGetIntegerSignMode",   fnGetIntegerSignMode  },
  {"fnGetLocR",              fnGetLocR             },
  {"fnGetRoundingMode",      fnGetRoundingMode     },
  {"fnGetSignificantDigits", fnGetSignificantDigits},
  {"fnGetStackSize",         fnGetStackSize        },
  {"fnGetWordSize",          fnGetWordSize         },
  {"fnImaginaryPart",        fnImaginaryPart       },
  {"fnInvert",               fnInvert              },
  {"fnIp",                   fnIp                  },
  {"fnIsPrime",              fnIsPrime             },
  {"fnLcm",                  fnLcm                 },
  {"fnLn",                   fnLn                  },
  {"fnLnGamma",              fnLnGamma             },
  {"fnLog10",                fnLog10               },
  {"fnLog2",                 fnLog2                },
  {"fnLogicalNot",           fnLogicalNot          },
  {"fnM1Pow",                fnM1Pow               },
  {"fnMagnitude",            fnMagnitude           },
  {"fnMirror",               fnMirror              },
  {"fnMultiply",             fnMultiply            },
  {"fnNop",                  fnNop                 },
  {"fnPi",                   fnPi                  },
  {"fnPower",                fnPower               },
  {"fnRealPart",             fnRealPart            },
  {"fnRollDown",             fnRollDown            },
  {"fnRollUp",               fnRollUp              },
  {"fnSign",                 fnSign                },
  {"fnSin",                  fnSin                 },
  {"fnSlvq",                 fnSlvq                },
  {"fnSquare",               fnSquare              },
  {"fnSquareRoot",           fnSquareRoot          },
  {"fnSubtract",             fnSubtract            },
  {"fnSwapRealImaginary",    fnSwapRealImaginary   },
  {"fnSwapXY",               fnSwapXY              },
  {"fnTan",                  fnTan                 },
  {"fnToPolar",              fnToPolar             },
  {"fnToRect",               fnToRect              },
  {"fnUnitVector",           fnUnitVector          },
  {"",                       NULL                  }
};

const funcTest_t funcTestCvt[] = {
  {"fnCvtAcreM2",     fnCvtAcreM2    },
  {"fnCvtAcreusM2",   fnCvtAcreusM2  },
  {"fnCvtAtmPa",      fnCvtAtmPa     },
  {"fnCvtAuM",        fnCvtAuM       },
  {"fnCvtBarPa",      fnCvtBarPa     },
  {"fnCvtBtuJ",       fnCvtBtuJ      },
  {"fnCvtCalJ",       fnCvtCalJ      },
  {"fnCvtCwtKg",      fnCvtCwtKg     },
  {"fnCvtFlozukM3",   fnCvtFlozukM3  },
  {"fnCvtFlozusM3",   fnCvtFlozusM3  },
  {"fnCvtFtM",        fnCvtFtM       },
  {"fnCvtGalukM3",    fnCvtGalukM3   },
  {"fnCvtGalusM3",    fnCvtGalusM3   },
  {"fnCvtHpeW",       fnCvtHpeW      },
  {"fnCvtHpmW",       fnCvtHpmW      },
  {"fnCvtHpukW",      fnCvtHpukW     },
  {"fnCvtInchM",      fnCvtInchM     },
  {"fnCvtInhgPa",     fnCvtInhgPa    },
  {"fnCvtLbfN",       fnCvtLbfN      },
  {"fnCvtLbKg",       fnCvtLbKg      },
  {"fnCvtLyM",        fnCvtLyM       },
  {"fnCvtMiM",        fnCvtMiM       },
  {"fnCvtNmiM",       fnCvtNmiM      },
  {"fnCvtOzKg",       fnCvtOzKg      },
  {"fnCvtPcM",        fnCvtPcM       },
  {"fnCvtPointM",     fnCvtPointM    },
  {"fnCvtPsiPa",      fnCvtPsiPa     },
  {"fnCvtQuartM3",    fnCvtQuartM3   },
  {"fnCvtSfeetM",     fnCvtSfeetM    },
  {"fnCvtShortcwtKg", fnCvtShortcwtKg},
  {"fnCvtShorttonKg", fnCvtShorttonKg},
  {"fnCvtStoneKg",    fnCvtStoneKg   },
  {"fnCvtTonKg",      fnCvtTonKg     },
  {"fnCvtTorrPa",     fnCvtTorrPa    },
  {"fnCvtTrozKg",     fnCvtTrozKg    },
  {"fnCvtWhJ",        fnCvtWhJ       },
  {"fnCvtYardM",      fnCvtYardM     },
  {"fnCvtYearS",      fnCvtYearS     },
  {"",                NULL           }
};



char *endOfString(char *string) { // string must point on the 1st "
  string++;
  while(*string != '"' && *string != 0) {
    if(*string == '\\' && *(string + 1) == 'x') {
      string += 3;
    }
    else if(*string == '\\') {
      string++;
    }

    string++;
  }

  if(*string == '"') {
    string++;
  }
  else {
    printf("Unterminated string\n");
    abortTest();
  }

  return string; // pointer to the 1st char after the ending "
}



void strToShortInteger(char *nimBuffer, calcRegister_t regist) {
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Any change in this function is to be reported in the function closeNim from file bufferize.c after the line: else if(nimNumberPart == NP_INT_BASE) {
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  longInteger_t minVal, value, maxVal;
  int16_t posHash, i, lg;
  int32_t base;

  lg = strlen(nimBuffer);
  posHash = 0;
  for(i=1; i<lg; i++) {
    if(nimBuffer[i] == '#') {
      posHash = i;
      break;
    }
  }

  for(i=posHash+1; i<lg; i++) {
    if(nimBuffer[i]<'0' || nimBuffer[i]>'9') {
      printf("\nError while initializing a short integer: there is a non numeric character in the base of the integer!\n");
      abortTest();
    }
  }

  base = atoi(nimBuffer + posHash + 1);
  if(base < 2 || base > 16) {
    printf("\nError while initializing a short integer: the base of the integer must be from 2 to 16!\n");
    abortTest();
  }

  for(i=nimBuffer[0] == '-' ? 1 : 0; i<posHash; i++) {
    if((nimBuffer[i] > '9' ? nimBuffer[i] - 'A' + 10 : nimBuffer[i] - '0') >= base) {
      printf("\nError while initializing a short integer: digit %c is not allowed in base %d!\n", nimBuffer[i], base);
      abortTest();
    }
  }

  stringToLongInteger(nimBuffer + (nimBuffer[0] == '+' ? 1 : 0), base, &value);

  // maxVal = 2^shortIntegerWordSize
  if(shortIntegerWordSize >= 1 && shortIntegerWordSize <= 64) {
    longInteger2Exp(shortIntegerWordSize, &maxVal);
  }
  else {
    printf("\nError while initializing a short integer: shortIntegerWordSize must be fom 1 to 64\n");
    abortTest();
  }

  // minVal = -maxVal/2
  longIntegerSetZero(&minVal); // Mandatory! Else segmentation fault at next instruction
  longIntegerDivideUInt(&maxVal, 2, &minVal); // minVal = maxVal / 2
  longIntegerSetNegativeSign(&minVal); // minVal = -minVal

  if(shortIntegerMode != SIM_UNSIGN) {
    longIntegerDivideUInt(&maxVal, 2, &maxVal); // maxVal /= 2
  }

  longIntegerSubtractUInt(&maxVal, 1, &maxVal); // maxVal--

  if(shortIntegerMode == SIM_UNSIGN) {
    longIntegerSetZero(&minVal); // minVal = 0
  }

  if(shortIntegerMode == SIM_1COMPL || shortIntegerMode == SIM_SIGNMT) {
    longIntegerAddUInt(&minVal, 1, &minVal); // minVal++
  }

  if(longIntegerCompare(&value, &minVal) == LONG_INTEGER_LESS_THAN || longIntegerCompare(&value, &maxVal) == LONG_INTEGER_GREATER_THAN) {
    char strMin[22], strMax[22];
    longIntegerToString(&minVal, strMin, 10);
    longIntegerToString(&maxVal, strMax, 10);
    printf("\nError while initializing a short integer: for a word size of %d bit%s and integer mode %s, the entered number must be from %s to %s!\n", shortIntegerWordSize, shortIntegerWordSize>1 ? "s" : "", getShortIntegerModeName(shortIntegerMode), strMin, strMax);
    abortTest();
  }

  reallocateRegister(regist, dtShortInteger, SHORT_INTEGER_SIZE, base);

  char strValue[22];
  longIntegerToString(&value, strValue, 10);

  uint64_t val;
  if(value.sign) {
    val = atoll(strValue + 1); // value is negative: discard the minus sign
  }
  else {
    val = atoll(strValue); // value is positive
  }

  if(shortIntegerMode == SIM_UNSIGN) {
  }
  else if(shortIntegerMode == SIM_2COMPL) {
    if(value.sign) {
      val = (~val + 1) & shortIntegerMask;
    }
  }
  else if(shortIntegerMode == SIM_1COMPL) {
    if(value.sign) {
      val = ~val & shortIntegerMask;
    }
  }
  else if(shortIntegerMode == SIM_SIGNMT) {
    if(value.sign) {
      val = (val & shortIntegerMask) | shortIntegerSignBit;
    }
  }
  else {
    printf("\nBad integer mode while initializing a short integer\n");
    abortTest();
  }

  *(REGISTER_SHORT_INTEGER_DATA(regist)) = val;
}



char hexToChar(const char *string) {
  char ch;

  if(   (('0' <= string[0] && string[0] <= '9') || ('A' <= string[0] && string[0] <= 'F') || ('a' <= string[0] && string[0] <= 'f'))
     && (('0' <= string[1] && string[1] <= '9') || ('A' <= string[1] && string[1] <= 'F') || ('a' <= string[1] && string[1] <= 'f'))) {
    if('0' <= string[0] && string[0] <= '9') {
      ch = string[0] - '0';
    }
    else if('a' <= string[0] && string[0] <= 'f') {
      ch = string[0] - 'a' + 10;
    }
    else {
      ch = string[0] - 'A' + 10;
    }

    if('0' <= string[1] && string[1] <= '9') {
      ch = ch*16 + string[1] - '0';
    }
    else if('a' <= string[1] && string[1] <= 'f') {
      ch = ch*16 + string[1] - 'a' + 10;
    }
    else {
      ch = ch*16 + string[1] - 'A' + 10;
    }
  }
  else {
    printf("\nMissformed parameter setting. The hexadecimal char \\x%c%c is erroneous.\n", string[0], string[1]);
    abortTest();
  }

  return ch;
}



void getString(char *str) {
  int32_t i, j, lg;

  lg = stringByteLength(str);

  str[lg - 1] = 0; // The ending "
  lg--;

  for(i=0; i<lg; i++) {
    if(str[i] == '\\' && (str[i + 1] == '\\' || str[i + 1] == '"')) {
      for(j=i+1; j<=lg; j++) {
        str[j - 1] = str[j];
      }
      lg--;
    }

    else if(str[i] == '\\' && str[i + 1] == 'x') {
      str[i] = hexToChar(str + i + 2);
      for(j=i+4; j<=lg; j++) {
        str[j - 3] = str[j];
      }
      lg -= 3;
    }
  }
}



void setParameter(char *p) {
  char l[200], r[200], real[200], imag[200], angMod[200], letter;
  int32_t i, am;
  longInteger_t temp;

  //printf("  setting %s\n", p);

  i = 0;
  while(p[i] != '=' && p[i] != 0) i++;
  if(p[i] == 0) {
    printf("\nMissformed parameter setting. Missing equal sign, remember that no space is allowed around the equal sign.\n");
    abortTest();
  }

  p[i] = 0;
  strcpy(l, p);
  strcpy(r, p + i + 1);

  if(r[0] == 0) {
    printf("\nMissformed parameter setting. Missing value after equal sign, remember that no space is allowed around the equal sign.\n");
    abortTest();
  }

  //Setting a flag
  if(l[0] == 'F') {
    if(r[0] != '0' && r[0] != '1' && r[1] != 0) {
      printf("\nMissformed flag setting. The rvalue must be 0 or 1\n");
      abortTest();
    }

    //Lettered flag
    if(l[1] >= 'A' && l[2] == 0) {
      if(strstr("XYZTABCDLIJK", l + 1) != NULL) {
        uint16_t flg;

        flg = l[1] == 'T' ? 103 :
              l[1] == 'L' ? 108 :
              l[1] <= 'D' ? l[1] + 39 :
              l[1] <= 'K' ? l[1] + 36 :
                            l[1] + 12;

        if(r[0] == '1') {
          fnSetFlag(flg);
          //printf("  Flag %c set\n", l[1]);
        }
        else {
          fnClearFlag(flg);
          //printf("  Flag %c cleared\n", l[1]);
        }
      }
      else {
        printf("\nMissformed flag setting. After F there shall be a number from 0 to 111 or a lettered flag\n");
        abortTest();
      }
    }

    //Numbered flag
    else if(   (l[1] >= '0' && l[1] <= '9' && l[2] == 0)
            || (l[1] >= '0' && l[1] <= '9' && l[2] >= '0' && l[2] <= '9' && l[3] == 0)
            || (l[1] >= '0' && l[1] <= '9' && l[2] >= '0' && l[2] <= '9' && l[3] >= '0' && l[3] <= '9' && l[4] == 0)) {
      uint16_t flg = atoi(l + 1);
      if(flg <= 111) {
        if(r[0] == '1') {
          fnSetFlag(flg);
          //printf("  Flag %d set\n", flg);
        }
        else {
          fnClearFlag(flg);
          //printf("  Flag %d cleared\n", flg);
        }
      }
      else {
        printf("\nMissformed flag setting. After the after F shall be a number from 0 to 111\n");
        abortTest();
      }
    }

    else {
      printf("\nMissformed numbered flag setting. After F there shall be a number from 0 to 111\n");
      abortTest();
    }
  }

  //Setting Stack Lift
  else if(strcmp(l, "SL") == 0) {
    if(r[0] != '0' && r[0] != '1' && r[1] != 0) {
      printf("\nMissformed stack lift setting. The rvalue must be 0 or 1");
      abortTest();
    }

    if(r[0] == '1') {
      STACK_LIFT_ENABLE;
      //printf("  Stack lift enabled\n");
    }
    else {
      STACK_LIFT_DISABLE;
      //printf("  Stack lift disabled\n");
    }
  }

  //Setting integer mode
  else if(strcmp(l, "IM") == 0) {
    if(strcmp(r, "1COMPL") == 0) {
      shortIntegerMode = SIM_1COMPL;
      //printf("  Set integer mode to 1COMPL\n");
    }
    else if(strcmp(r, "2COMPL") == 0) {
      shortIntegerMode = SIM_2COMPL;
      //printf("  Set integer mode to 2COMPL\n");
    }
    else if(strcmp(r, "UNSIGN") == 0) {
      shortIntegerMode = SIM_UNSIGN;
      //printf("  Set integer mode to UNSIGN\n");
    }
    else if(strcmp(r, "SIGNMT") == 0) {
      shortIntegerMode = SIM_SIGNMT;
      //printf("  Set integer mode to SIGNMT\n");
    }
    else {
      printf("\nMissformed integer mode setting. The rvalue must be 1COMPL, 2COMPL, UNSIGN or SIGNMT.\n");
      abortTest();
    }
  }

  //Setting angular mode
  else if(strcmp(l, "AM") == 0) {
    if(strcmp(r, "DEG") == 0) {
      currentAngularMode = AM_DEGREE;
      //printf("  Set angular mode to DEG\n");
    }
    else if(strcmp(r, "DMS") == 0) {
      currentAngularMode = AM_DMS;
      //printf("  Set angular mode to DMS\n");
    }
    else if(strcmp(r, "GRAD") == 0) {
      currentAngularMode = AM_GRAD;
      //printf("  Set angular mode to GRAD\n");
    }
    else if(strcmp(r, "RAD") == 0) {
      currentAngularMode = AM_RADIAN;
      //printf("  Set angular mode to RAD\n");
    }
    else if(strcmp(r, "MULTPI") == 0) {
      currentAngularMode = AM_MULTPI;
      //printf("  Set angular mode to MULTPI\n");
    }
    else {
      printf("\nMissformed angular mode setting. The rvalue must be DEG, DMS, GRAD, RAD or MULTPI.\n");
      abortTest();
    }
  }

  //Setting stack size
  else if(strcmp(l, "SS") == 0) {
    if(strcmp(r, "4") == 0) {
      stackSize = SS_4;
      //printf("  Set stack size to 4\n");
    }
    else if(strcmp(r, "8") == 0) {
      stackSize = SS_8;
      //printf("  Set stack size to 8\n");
    }
    else {
      printf("\nMissformed stack size setting. The rvalue must be 4 or 8.\n");
      abortTest();
    }
  }

  //Setting word size
  else if(strcmp(l, "WS") == 0) {
    if(   (r[0] >= '0' && r[0] <= '9' && r[1] == 0)
       || (r[0] >= '0' && r[0] <= '9' && r[1] >= '0' && r[1] <= '9' && r[2] == 0)) {
      uint16_t ws = atoi(r);

      if(ws == 0) {
        ws = 64;
      }
      if(ws <= 64) {
        fnSetWordSize(ws);
        //printf("  Set word size to %d bit\n", ws);
      }
      else {
        printf("\nMissformed word size setting. The rvalue must be from 0 to 64 (0 is the same as 64).\n");
        abortTest();
      }
    }
    else {
      printf("\nMissformed word size setting. The rvalue must be a number from 0 to 64 (0 is the same as 64).\n");
      abortTest();
    }
  }

  //Setting gap
  else if(strcmp(l, "GAP") == 0) {
    if(   (r[0] >= '0' && r[0] <= '9' && r[1] == 0)
       || (r[0] >= '0' && r[0] <= '9' && r[1] >= '0' && r[1] <= '9' && r[2] == 0)) {
      uint16_t gap = atoi(r);

      if(gap <= 15) {
        fnDisplayFormatGap(gap);
        //printf("  Set grouping gap to %d\n", gap);
      }
      else {
        printf("\nMissformed grouping gap setting. The rvalue must be from 0 to 15.\n");
        abortTest();
      }
    }
    else {
      printf("\nMissformed grouping gap setting. The rvalue must be a number from 0 to 15.\n");
      abortTest();
    }
  }

  //Setting significant digits
  else if(strcmp(l, "SD") == 0) {
    if(   (r[0] >= '0' && r[0] <= '9' && r[1] == 0)
       || (r[0] >= '0' && r[0] <= '9' && r[1] >= '0' && r[1] <= '9' && r[2] == 0)) {
      uint16_t sd = atoi(r);

      if(sd <= 34) {
        significantDigits = sd;
        //printf("  Set significant digits to %d\n", sd);
      }
      else {
        printf("\nMissformed significant digits setting. The rvalue must be from 0 to 34 (0 is the same as 34).\n");
        abortTest();
      }
    }
    else {
      printf("\nMissformed significant digits setting. The rvalue must be a number from 0 to 34 (0 is the same as 34).\n");
      abortTest();
    }
  }

  //Setting rounding mode
  else if(strcmp(l, "RM") == 0) {
    if(r[0] >= '0' && r[0] <= '9' && r[1] == 0) {
      uint16_t rm = atoi(r);

      if(rm <= 6) {
        fnRoundingMode(rm);
        //printf("  Set rounding mode to %d (%s)\n", rm, getRoundingModeName(rm));
        //printf("  Set rounding mode to %d\n", rm);
      }
      else {
        printf("\nMissformed rounding mode setting. The rvalue must be a number from 0 to 6.\n");
        abortTest();
      }
    }
    else {
      printf("\nMissformed rounding mode setting. The rvalue must be a number from 0 to 6.\n");
      abortTest();
    }
  }

  //Setting number of correct significant digits expected
  else if(strcmp(l, "NCSD") == 0) {
    if(   (r[0] >= '0' && r[0] <= '9' && r[1] == 0)
       || (r[0] >= '0' && r[0] <= '9' && r[1] >= '0' && r[1] <= '9' && r[2] == 0)) {
      uint16_t ncsd = atoi(r);

      if(1 <= ncsd && ncsd <= 34) {
        numberOfCorrectSignificantDigitsExpected = ncsd;
        //printf("  Set significant digits to %d\n", ncsd);
      }
      else {
        printf("\nMissformed number of correct significant digits expected setting. The rvalue must be from 1 to 34.\n");
        abortTest();
      }
    }
    else {
      printf("\nMissformed number of correct significant digits expected setting. The rvalue must be a number from 1 to 34.\n");
      abortTest();
    }
  }

  //Setting a register
  else if(l[0] == 'R') {
    calcRegister_t regist;

    //Lettered register
    if(l[1] >= 'A' && l[2] == 0) {
      if(strstr("XYZTABCDLIJK", l + 1) != NULL) {
        letter = l[1];
        regist = l[1] == 'T' ? 103 :
                 l[1] == 'L' ? 108 :
                 l[1] <= 'D' ? l[1] + 39 :
                 l[1] <= 'K' ? l[1] + 36 :
                               l[1] + 12;
      }
      else {
        printf("\nMissformed lettered register setting. The letter after R is not a lettered register.\n");
        abortTest();
      }
    }

    //Numbered register
    else if(   (l[1] >= '0' && l[1] <= '9' && l[2] == 0)
            || (l[1] >= '0' && l[1] <= '9' && l[2] >= '0' && l[2] <= '9' && l[3] == 0)
            || (l[1] >= '0' && l[1] <= '9' && l[2] >= '0' && l[2] <= '9' && l[3] >= '0' && l[3] <= '9' && l[4] == 0)) {
      regist = atoi(l + 1);
      if(regist > 111) {
        printf("\nMissformed numbered register setting. Th number after R shall be a number from 0 to 111.\n");
        abortTest();
      }
      letter = 0;
    }

    else {
      printf("\nMissformed register setting. After R there shall be a number from 0 to 111 or a lettered register.\n");
      abortTest();
    }

    // find the : separating the data type and the value
    i = 0;
    while(r[i] != ':' && r[i] != 0) i++;
    if(r[i] == 0) {
      printf("\nMissformed register value. Missing colon between data type and value.\n");
      abortTest();
    }

    // separating the data type and the value
    r[i] = 0;
    strcpy(l, r);
    memmove(r, r + i + 1, strlen(r + i + 1) + 1);

    if(strcmp(l, "LONI") == 0) {
      // remove beginning and ending " and removing leading spaces
      memmove(r, r + 1, strlen(r));
      while(r[0] == ' ') memmove(r, r + 1, strlen(r));
      r[strlen(r) - 1] = 0;

      stringToLongInteger(r, 10, &temp);
      convertLongIntegerToLongIntegerRegister(&temp, regist);
    }
    else if(strcmp(l, "RE16") == 0) {
      // remove beginning and ending " and removing leading spaces
      memmove(r, r + 1, strlen(r));
      while(r[0] == ' ') memmove(r, r + 1, strlen(r));
      r[strlen(r) - 1] = 0;

      // removing trailing spaces
      while(r[strlen(r) - 1] == ' ') r[strlen(r) - 1] = 0;

      // replace , with .
      for(int i=0; i<(int)strlen(r); i++) {
        if(r[i] == ',') r[i] = '.';
      }

      reallocateRegister(regist, dtReal16, REAL16_SIZE, TAG_NONE);
      stringToReal16(r, REGISTER_REAL16_DATA(regist));
    }
    else if(strcmp(l, "CO16") == 0) {
      // remove beginning and ending " and removing leading spaces
      memmove(r, r + 1, strlen(r));
      while(r[0] == ' ') memmove(r, r + 1, strlen(r));
      r[strlen(r) - 1] = 0;

      // find the i separating the real and imagynary part
      i = 0;
      while(r[i] != 'i' && r[i] != 0) i++;
      if(r[i] == 0) {
        printf("\nMissformed register complex16 value. Missing i between real and imaginary part.\n");
        abortTest();
      }

      // separate real and imaginary part
      r[i] = 0;
      strcpy(real, r);
      strcpy(imag, r + i + 1);

      // remove leading spaces
      while(imag[0] == ' ') memmove(imag, imag + 1, strlen(imag));

      // removing trailing spaces from real part
      while(real[strlen(real) - 1] == ' ') real[strlen(real) - 1] = 0;

      // removing trailing spaces from imaginary part
      while(imag[strlen(imag) - 1] == ' ') imag[strlen(imag) - 1] = 0;

      // replace , with . in the real part
      for(int i=0; i<(int)strlen(real); i++) {
        if(real[i] == ',') real[i] = '.';
      }

      // replace , with . in the imaginary part
      for(int i=0; i<(int)strlen(imag); i++) {
        if(imag[i] == ',') imag[i] = '.';
      }

      reallocateRegister(regist, dtComplex16, COMPLEX16_SIZE, TAG_NONE);
      stringToReal16(real, REGISTER_REAL16_DATA(regist));
      stringToReal16(imag, REGISTER_IMAG16_DATA(regist));
    }
    else if(strcmp(l, "AN16") == 0 || strcmp(l, "AN34") == 0) {
      // find the : separating the angle value from the angular mode
      i = 0;
      while(r[i] != ':' && r[i] != 0) i++;
      if(r[i] == 0) {
        printf("\nMissformed register angle%d value. Missing colon between angle value and angular mode.\n", strcmp(l, "AN16") == 0 ? 16 : 34);
        abortTest();
      }

      // separate angle value and angular mode
      r[i] = 0;
      strcpy(angMod, r + i + 1);

           if(strcmp(angMod, "DEG"   ) == 0) am = AM_DEGREE;
      else if(strcmp(angMod, "DMS"   ) == 0) am = AM_DMS;
      else if(strcmp(angMod, "GRAD"  ) == 0) am = AM_GRAD;
      else if(strcmp(angMod, "RAD"   ) == 0) am = AM_RADIAN;
      else if(strcmp(angMod, "MULTPI") == 0) am = AM_MULTPI;
      else {
        printf("\nMissformed register angle%d unit. Unknown unit after angle value.\n", strcmp(l, "AN16") == 0 ? 16 : 34);
        abortTest();
      }

      // remove beginning and ending " and removing leading spaces
      memmove(r, r + 1, strlen(r));
      while(r[0] == ' ') memmove(r, r + 1, strlen(r));
      r[strlen(r) - 1] = 0;

      // replace , with .
      for(int i=0; i<(int)strlen(r); i++) {
        if(r[i] == ',') r[i] = '.';
      }

      if(strcmp(l, "AN16") == 0) {
        reallocateRegister(regist, dtAngle16, REAL16_SIZE, am);
        stringToReal16(r, REGISTER_REAL16_DATA(regist));
      }
      else {
        reallocateRegister(regist, dtAngle34, REAL34_SIZE, am);
        stringToReal34(r, REGISTER_REAL34_DATA(regist));
      }
    }
    else if(strcmp(l, "STRI") == 0) {
      getString(r + 1);
      reallocateRegister(regist, dtString, stringByteLength(r + 1), 0);
      strcpy(REGISTER_STRING_DATA(regist), r + 1);
    }
    else if(strcmp(l, "SHOI") == 0) {
      // find the # separating the value from the base
      i = 0;
      while(r[i] != '#' && r[i] != 0) i++;
      if(r[i] == 0) {
        printf("\nMissformed register short integer value. Missing # between value and base.\n");
        abortTest();
      }

      // remove beginning and ending " and removing leading spaces
      memmove(r, r + 1, strlen(r));
      while(r[0] == ' ') memmove(r, r + 1, strlen(r));
      r[strlen(r) - 1] = 0;

      // Convert string to upper case
      for(i=0; r[i]!=0; i++) {
        if('a' <= r[i] && r[i] <= 'z') {
          r[i] -= 32;
        }
      }

      strToShortInteger(r, regist);
    }
    else if(strcmp(l, "RE34") == 0) {
      memmove(r, r + 1, strlen(r));
      while(r[0] == ' ') memmove(r, r + 1, strlen(r));
      r[strlen(r) - 1] = 0;

      while(r[strlen(r) - 1] == ' ') r[strlen(r) - 1] = 0;
      for(int i=0; i<(int)strlen(r); i++) {
        if(r[i] == ',') r[i] = '.';
      }

      reallocateRegister(regist, dtReal34, REAL34_SIZE, TAG_NONE);
      stringToReal34(r, REGISTER_REAL34_DATA(regist));
    }
    else if(strcmp(l, "CO34") == 0) {
      // remove beginning and ending " and removing leading spaces
      memmove(r, r + 1, strlen(r));
      while(r[0] == ' ') memmove(r, r + 1, strlen(r));
      r[strlen(r) - 1] = 0;

      // find the i separating the real and imagynary part
      i = 0;
      while(r[i] != 'i' && r[i] != 0) i++;
      if(r[i] == 0) {
        printf("\nMissformed register complex34 value. Missing i between real and imaginary part.\n");
        abortTest();
      }

      // separate real and imaginary part
      r[i] = 0;
      strcpy(real, r);
      strcpy(imag, r + i + 1);

      // remove leading spaces
      while(imag[0] == ' ') memmove(imag, imag + 1, strlen(imag));

      // removing trailing spaces from real part
      while(real[strlen(real) - 1] == ' ') real[strlen(real) - 1] = 0;

      // removing trailing spaces from imaginary part
      while(imag[strlen(imag) - 1] == ' ') imag[strlen(imag) - 1] = 0;

      // replace , with . in the real part
      for(int i=0; i<(int)strlen(real); i++) {
        if(real[i] == ',') real[i] = '.';
      }

      // replace , with . in the imaginary part
      for(int i=0; i<(int)strlen(imag); i++) {
        if(imag[i] == ',') imag[i] = '.';
      }

      reallocateRegister(regist, dtComplex34, COMPLEX34_SIZE, TAG_NONE);
      stringToReal34(real, REGISTER_REAL34_DATA(regist));
      stringToReal34(imag, REGISTER_IMAG34_DATA(regist));
    }
    else {
      printf("\nMissformed register value. Unknown data type %s for register %s\n", l, p+1);
      abortTest();
    }

    //if(letter == 0) {
    //  printf("  R%d = ", regist);
    //}
    //else {
    //  printf("  R%c = ", letter);
    //}

    //printRegisterToConsole(regist, 0);
    //printf("\n");
  }

  else {
    printf("\nUnknown setting %s.\n", l);
    abortTest();
  }
}



void inParameters(char *token) {
  char parameter[300];
  int32_t index, lg;

  while(*token == ' ') token++;
  while(*token != 0) {
    index = 0;
    while(*token != ' ' && *token != 0) {
      if(*token == '"') { // Inside a string
        lg = endOfString(token) - token;
        strncpy(parameter + index, token, lg--);
        index += lg;
        token += lg;
      }
      parameter[index++] = *(token++);
    }
    parameter[index] = 0;

    setParameter(parameter);

    while(*token == ' ') token++;
  }
}



void checkRegisterType(calcRegister_t regist, char letter, uint32_t expectedDataType, uint32_t expectedTag) {
  if(getRegisterDataType(regist) != expectedDataType) {
    if(letter == 0) {
      printf("\nRegister %u should be %s but it is %s!\n", regist, getDataTypeName(expectedDataType, true, false), getDataTypeName(getRegisterDataType(regist), true, false));
      printf("R%u = ", regist);
    }
    else {
      printf("\nRegister %c should be %s but it is %s!\n", letter, getDataTypeName(expectedDataType, true, false), getDataTypeName(getRegisterDataType(regist), true, false));
      printf("R%c = ", letter);
    }
    printRegisterToConsole(regist, 0);
    printf("\n");
    abortTest();
  }

  if(getRegisterTag(regist) != expectedTag) {
    if(getRegisterDataType(regist) == dtShortInteger) {
      if(letter == 0) {
        printf("\nRegister %u is a short integer base %u but it should be base %u!\n", regist, expectedTag, getRegisterShortIntegerBase(regist));
        printf("R%u = ", regist);
      }
      else {
        printf("\nRegister %c is a short integer base %u but it should be base %u!\n", letter, expectedTag, getRegisterShortIntegerBase(regist));
        printf("R%c = ", letter);
      }
      printRegisterToConsole(regist, 0);
      printf("\n");
      abortTest();
    }
    else if(getRegisterDataType(regist) == dtReal16 || getRegisterDataType(regist) == dtReal34) {
      if(letter == 0) {
        printf("\nRegister %u should be a real tagged %s but it is tagged %s!\n", regist, getAngularModeName(expectedTag), getAngularModeName(getRegisterAngularMode(regist)));
        printf("R%u = ", regist);
      }
      else {
        printf("\nRegister %c should be a real tagged %s but it is tagged %s!\n", letter, getAngularModeName(expectedTag), getAngularModeName(getRegisterAngularMode(regist)));
        printf("R%c = ", letter);
      }
      printRegisterToConsole(regist, 0);
      printf("\n");
      abortTest();
    }
    else if(getRegisterDataType(regist) == dtLongInteger) {
      if(letter == 0) {
        printf("\nRegister %u should be a long integer tagged %d but it is tagged %d!\n", regist, expectedTag, getRegisterLongIntegerSign(regist));
        printf("R%u = ", regist);
      }
      else {
        printf("\nRegister %c should be a long integer tagged %d but it is tagged %d!\n", letter, expectedTag, getRegisterLongIntegerSign(regist));
        printf("R%c = ", letter);
      }
      printRegisterToConsole(regist, 0);
      printf("\n");
      abortTest();
    }
  }
}



int relativeErrorReal16(real16_t *expectedValue, real16_t *value, char *numberPart) {
  real51_t expectedValue51, value51, relativeError51, numSignificantDigits51;
  real16_t integer;

  real16ToReal51(expectedValue, &expectedValue51);
  real16ToReal51(value, &value51);

  real51Subtract(&expectedValue51, &value51, &relativeError51);

  if(!real51IsZero(&expectedValue51)) {
    real51Divide(&relativeError51, &expectedValue51, &relativeError51);
    real51Divide(const51_1, &relativeError51, &numSignificantDigits51);
  }

  real51SetPositiveSign(&numSignificantDigits51);
  real51Log10(&numSignificantDigits51, &numSignificantDigits51);

  real51ToReal16(&numSignificantDigits51, &integer);
  correctSignificantDigits = real16ToInt32(&integer);
  if(correctSignificantDigits <= 15) {
    //printf("\nThere are only %d correct significant digits in the %s part of the value: %d are expected!\n", correctSignificantDigits, numberPart, numberOfCorrectSignificantDigitsExpected);
    printf("\nThere are only %d correct significant digits in the %s part of the value!\n", correctSignificantDigits, numberPart);
    printf("%s\n", lastInParameters);
    printf("%s\n", line);
    printf("in file %s line %d\n", fileName, lineNumber);
    if(correctSignificantDigits < numberOfCorrectSignificantDigitsExpected) {
      puts(registerExpectedAndValue);
      exit(-1);
    }
  }

  return correctSignificantDigits < numberOfCorrectSignificantDigitsExpected ? RE_INACCURATE : RE_ACCURATE;
}



int relativeErrorReal34(real34_t *expectedValue, real34_t *value, char *numberPart) {
  real51_t expectedValue51, value51, relativeError51, numSignificantDigits51;
  real16_t integer;

  real34ToReal51(expectedValue, &expectedValue51);
  real34ToReal51(value, &value51);

  real51Subtract(&expectedValue51, &value51, &relativeError51);

  if(!real51IsZero(&expectedValue51)) {
    real51Divide(&relativeError51, &expectedValue51, &relativeError51);
    real51Divide(const51_1, &relativeError51, &numSignificantDigits51);
  }

  real51SetPositiveSign(&numSignificantDigits51);
  real51Log10(&numSignificantDigits51, &numSignificantDigits51);

  real51ToReal16(&numSignificantDigits51, &integer);
  correctSignificantDigits = real16ToInt32(&integer);
  if(correctSignificantDigits <= 33) {
    //printf("\nThere are only %d correct significant digits in the %s part of the value: %d are expected!\n", correctSignificantDigits, numberPart, numberOfCorrectSignificantDigitsExpected);
    printf("\nThere are only %d correct significant digits in the %s part of the value!\n", correctSignificantDigits, numberPart);
    printf("%s\n", lastInParameters);
    printf("%s\n", line);
    printf("in file %s line %d\n", fileName, lineNumber);
    if(correctSignificantDigits < 32 && correctSignificantDigits < numberOfCorrectSignificantDigitsExpected) {
      puts(registerExpectedAndValue);
      exit(-1);
    }
  }

  return (correctSignificantDigits < 32 && correctSignificantDigits < numberOfCorrectSignificantDigitsExpected) ? RE_INACCURATE : RE_ACCURATE;
}



void wrongRegisterValue(calcRegister_t regist, char letter, char *expectedValue) {
  if(letter == 0) {
    printf("\nRegister %u value should be ", regist);
  }
  else {
    printf("\nRegister %c value should be ", letter);
  }
  printf("%s\nbut it is ", expectedValue);
  printRegisterToConsole(regist, 0);
  printf("\n");
  abortTest();
}



void expectedAndShouldBeValue(calcRegister_t regist, char letter, char *expectedValue, char *expectedAndValue) {
  char str[300];

  if(letter == 0) {
    sprintf(expectedAndValue, "\nRegister %u value should be ", regist);
  }
  else {
    sprintf(expectedAndValue, "\nRegister %c value should be ", letter);
  }
  strcat(expectedAndValue, expectedValue);
  strcat(expectedAndValue, "\nbut it is ");
  printRegisterToString(regist, str);
  strcat(expectedAndValue, str);
  strcat(expectedAndValue, "\n");
}



void checkExpectedOutParameter(char *p) {
  char l[200], r[200], real[200], imag[200], angMod[200], letter;
  int32_t i, am;
  real16_t expectedReal16, expectedImag16;
  real34_t expectedReal34, expectedImag34;

  //printf("  Checking %s\n", p);

  i = 0;
  while(p[i] != '=' && p[i] != 0) i++;
  if(p[i] == 0) {
    printf("\nMissformed out parameter. Missing equal sign, remember that no space is allowed around the equal sign.\n");
    abortTest();
  }

  p[i] = 0;
  strcpy(l, p);
  strcpy(r, p + i + 1);

  if(r[0] == 0) {
    printf("\nMissformed out parameter. Missing value after equal sign, remember that no space is allowed around the equal sign.\n");
    abortTest();
  }

  //Checking a flag
  if(l[0] == 'F') {
    if(r[0] != '0' && r[0] != '1' && r[1] != 0) {
      printf("\nMissformed flag checking. The rvalue must be 0 or 1.\n");
      abortTest();
    }

    //Lettered flag
    if(l[1] >= 'A' && l[2] == 0) {
      if(strstr("XYZTABCDLIJK", l + 1) != NULL) {
        uint16_t flg;

        flg = l[1] == 'T' ? 103 :
              l[1] == 'L' ? 108 :
              l[1] <= 'D' ? l[1] + 39 :
              l[1] <= 'K' ? l[1] + 36 :
                            l[1] + 12;

        if(r[0] == '1') {
          if(!getFlag(flg)) {
            printf("\nFlag %c should be set but it is clear!\n", l[1]);
            abortTest();
          }
        }
        else {
          if(getFlag(flg)) {
            printf("\nFlag %c should be clear but it is set!\n", l[1]);
            abortTest();
          }
        }
      }
      else {
        printf("\nMissformed flag checking. After F there shall be a number from 0 to 111 or a lettered flag.\n");
        abortTest();
      }
    }

    //Numbered flag
    else if(   (l[1] >= '0' && l[1] <= '9' && l[2] == 0)
            || (l[1] >= '0' && l[1] <= '9' && l[2] >= '0' && l[2] <= '9' && l[3] == 0)
            || (l[1] >= '0' && l[1] <= '9' && l[2] >= '0' && l[2] <= '9' && l[3] >= '0' && l[3] <= '9' && l[4] == 0)) {
      uint16_t flg = atoi(l + 1);
      if(flg <= 111) {
        if(r[0] == '1') {
          printf("\nFlag %d should be set but it is clear!\n", flg);
          abortTest();
        }
        else {
          printf("\nFlag %d should be clear but it is set!\n", flg);
          abortTest();
        }
      }
      else {
        printf("\nMissformed flag checking in line. After the after F shall be a number from 0 to 111.\n");
        abortTest();
      }
    }

    else {
      printf("\nMissformed numbered flag checking. After F there shall be a number from 0 to 111.\n");
      abortTest();
    }
  }

  //Checking Stack Lift
  else if(strcmp(l, "SL") == 0) {
    if(r[0] != '0' && r[0] != '1' && r[1] != 0) {
      printf("\nMissformed stack lift checking. The rvalue must be 0 or 1.\n");
      abortTest();
    }

    if(r[0] == '1') {
      if(!stackLiftEnabled) {
        printf("\nStack lift should be enabled but it is disabled!\n");
        abortTest();
      }
    }
    else {
      if(stackLiftEnabled) {
        printf("\nStack lift should be disabled but it is enabled!\n");
        abortTest();
      }
    }
  }

  //Checking integer mode
  else if(strcmp(l, "IM") == 0) {
    if(strcmp(r, "1COMPL") == 0) {
      if(shortIntegerMode != SIM_1COMPL) {
        printf("\nInteger mode should be 1COMPL but it is not!\n");
        abortTest();
      }
    }
    else if(strcmp(r, "2COMPL") == 0) {
      if(shortIntegerMode != SIM_2COMPL) {
        printf("\nInteger mode should be 2COMPL but it is not!\n");
        abortTest();
      }
    }
    else if(strcmp(r, "UNSIGN") == 0) {
      if(shortIntegerMode != SIM_UNSIGN) {
        printf("\nInteger mode should be UNSIGN but it is not!\n");
        abortTest();
      }
    }
    else if(strcmp(r, "SIGNMT") == 0) {
      if(shortIntegerMode != SIM_SIGNMT) {
        printf("\nInteger mode should be SIGNMT but it is not!\n");
        abortTest();
      }
    }
    else {
      printf("\nMissformed integer mode checking. The rvalue must be 1COMPL, 2COMPL, UNSIGN or SIGNMT.\n");
      abortTest();
    }
  }

  //Checking angular mode
  else if(strcmp(l, "AM") == 0) {
    if(strcmp(r, "DEG") == 0) {
      if(currentAngularMode != AM_DEGREE) {
        printf("\nAngular mode should be DEGREE but it is not!\n");
        abortTest();
      }
    }
    else if(strcmp(r, "DMS") == 0) {
      if(currentAngularMode != AM_DMS) {
        printf("\nAngular mode should be DMS but it is not!\n");
        abortTest();
      }
    }
    else if(strcmp(r, "GRAD") == 0) {
      if(currentAngularMode != AM_GRAD) {
        printf("\nAngular mode should be GRAD but it is not!\n");
        abortTest();
      }
    }
    else if(strcmp(r, "RAD") == 0) {
      if(currentAngularMode != AM_RADIAN) {
        printf("\nAngular mode should be RAD but it is not!\n");
        abortTest();
      }
    }
    else if(strcmp(r, "MULTPI") == 0) {
      if(currentAngularMode != AM_MULTPI) {
        printf("\nAngular mode should be MULTPI but it is not!\n");
        abortTest();
      }
    }
    else {
      printf("\nMissformed angular mode checking. The rvalue must be DEG, DMS, GRAD, RAD or MULTPI.\n");
      abortTest();
    }
  }

  //Checking stack size
  else if(strcmp(l, "SS") == 0) {
    if(strcmp(r, "4") == 0) {
      if(stackSize != SS_4) {
        printf("\nStack size should be 4 but it is not!\n");
        abortTest();
      }
    }
    else if(strcmp(r, "8") == 0) {
      if(stackSize != SS_8) {
        printf("\nStack size should be 8 but it is not!\n");
        abortTest();
      }
    }
    else {
      printf("\nMissformed stack size checking. The rvalue must be 4 or 8.\n");
      abortTest();
    }
  }

  //Checking word size
  else if(strcmp(l, "WS") == 0) {
    if(   (r[0] >= '0' && r[0] <= '9' && r[1] == 0)
       || (r[0] >= '0' && r[0] <= '9' && r[1] >= '0' && r[1] <= '9' && r[2] == 0)) {
      uint16_t ws = atoi(r);

      if(ws == 0) {
        ws = 64;
      }
      if(ws <= 64) {
        if(shortIntegerWordSize != ws) {
          printf("\nShort integer word size should be %u but it is %u!\n", ws, shortIntegerWordSize);
          abortTest();
        }
      }
      else {
        printf("\nMissformed word size checking. The rvalue must be from 0 to 64 (0 is the same as 64).\n");
        abortTest();
      }
    }
    else {
      printf("\nMissformed word size checking. The rvalue must be a number from 0 to 64 (0 is the same as 64).\n");
      abortTest();
    }
  }

  //Checking significant digits
  else if(strcmp(l, "SD") == 0) {
    if(   (r[0] >= '0' && r[0] <= '9' && r[1] == 0)
       || (r[0] >= '0' && r[0] <= '9' && r[1] >= '0' && r[1] <= '9' && r[2] == 0)) {
      uint16_t sd = atoi(r);

      if(sd <= 34) {
        if(significantDigits != sd) {
          printf("\nNumber of significant digits should be %u but it is %u!\n", sd, significantDigits);
          abortTest();
        }
      }
      else {
        printf("\nMissformed significant digits checking. The rvalue must be from 0 to 34 (0 is the same as 34).\n");
        abortTest();
      }
    }
    else {
      printf("\nMissformed significant digits checking. The rvalue must be a number from 0 to 34 (0 is the same as 34).\n");
      abortTest();
    }
  }

  //Checking rounding mode
  else if(strcmp(l, "RM") == 0) {
    if(r[0] >= '0' && r[0] <= '9' && r[1] == 0) {
      uint16_t rm = atoi(r);

      if(rm <= 6) {
        if(roundingMode != rm) {
          printf("\nRounding mode should be %u but it is %u!\n", rm, roundingMode);
          abortTest();
        }
      }
      else {
        printf("\nMissformed rounding mode checking. The rvalue must be a number from 0 to 6.\n");
        abortTest();
      }
    }
    else {
      printf("\nMissformed rounding mode checking. The rvalue must be a number from 0 to 6.\n");
      abortTest();
    }
  }

  //Checking error code
  else if(strcmp(l, "EC") == 0) {
    if(   (r[0] >= '0' && r[0] <= '9' && r[1] == 0)
       || (r[0] >= '0' && r[0] <= '9' && r[1] >= '0' && r[1] <= '9' && r[2] == 0)) {
      uint16_t ec = atoi(r);

      if(ec <= 28) {
        if(lastErrorCode != ec) {
          printf("\nLast error code should be %u (%s) but it is %u (%s)!\n", ec, errorMessages[ec], lastErrorCode, errorMessages[lastErrorCode]);
          abortTest();
        }
      }
      else {
        printf("\nMissformed error code checking. The rvalue must be a number from 0 to 28.\n");
        abortTest();
      }
    }
    else {
      printf("\nMissformed error code checking. The rvalue must be a number from 0 to 28.\n");
      abortTest();
    }
  }

  //Checking a register
  else if(l[0] == 'R') {
    calcRegister_t regist;

    //Lettered register
    if(l[1] >= 'A' && l[2] == 0) {
      if(strstr("XYZTABCDLIJK", l + 1) != NULL) {
        letter = l[1];
        regist = l[1] == 'T' ? 103 :
                 l[1] == 'L' ? 108 :
                 l[1] <= 'D' ? l[1] + 39 :
                 l[1] <= 'K' ? l[1] + 36 :
                               l[1] + 12;
      }
      else {
        printf("\nMissformed lettered register checking. The letter after R is not a lettered register.\n");
        abortTest();
      }
    }

    //Numbered register
    else if(   (l[1] >= '0' && l[1] <= '9' && l[2] == 0)
            || (l[1] >= '0' && l[1] <= '9' && l[2] >= '0' && l[2] <= '9' && l[3] == 0)
            || (l[1] >= '0' && l[1] <= '9' && l[2] >= '0' && l[2] <= '9' && l[3] >= '0' && l[3] <= '9' && l[4] == 0)) {
      regist = atoi(l + 1);
      if(regist > 111) {
        printf("\nMissformed numbered register checking. Th number after R shall be a number from 0 to 111.\n");
        abortTest();
      }
      letter = 0;
    }

    else {
      printf("\nMissformed register checking. After R there shall be a number from 0 to 111 or a lettered register.\n");
      abortTest();
    }

    // find the : separating the data type and the value
    i = 0;
    while(r[i] != ':' && r[i] != 0) i++;
    if(r[i] == 0) {
      printf("\nMissformed register value. Missing colon between data type and value.\n");
      abortTest();
    }

    // separating the data type and the value
    r[i] = 0;
    strcpy(l, r);
    memmove(r, r + i + 1, strlen(r + i + 1) + 1);

    if(strcmp(l, "LONI") == 0) {
      longInteger_t expectedLongInteger, registerLongInteger;

      // remove beginning and ending " and removing leading spaces
      memmove(r, r + 1, strlen(r));
      while(r[0] == ' ') memmove(r, r + 1, strlen(r));
      r[strlen(r) - 1] = 0;

      stringToLongInteger(r, 10, &expectedLongInteger);
      checkRegisterType(regist, letter, dtLongInteger, expectedLongInteger.sign);
      convertLongIntegerRegisterToLongInteger(regist, &registerLongInteger);
      if(longIntegerCompare(&expectedLongInteger, &registerLongInteger) != LONG_INTEGER_EQUAL) {
        wrongRegisterValue(regist, letter, r);
      }
    }
    else if(strcmp(l, "RE16") == 0) {
      checkRegisterType(regist, letter, dtReal16, TAG_NONE);

      // remove beginning and ending " and removing leading spaces
      memmove(r, r + 1, strlen(r));
      while(r[0] == ' ') memmove(r, r + 1, strlen(r));
      r[strlen(r) - 1] = 0;

      // removing trailing spaces
      while(r[strlen(r) - 1] == ' ') r[strlen(r) - 1] = 0;

      // replace , with .
      for(int i=0; i<(int)strlen(r); i++) {
        if(r[i] == ',') r[i] = '.';
      }

      stringToReal16(r, &expectedReal16);
      if(!real16CompareEqual(&expectedReal16, REGISTER_REAL16_DATA(regist))) {
        expectedAndShouldBeValue(regist, letter, r, registerExpectedAndValue);
        if(relativeErrorReal16(&expectedReal16, REGISTER_REAL16_DATA(regist), "real") == RE_INACCURATE) {
          wrongRegisterValue(regist, letter, r);
        }
      }
    }
    else if(strcmp(l, "CO16") == 0) {
      checkRegisterType(regist, letter, dtComplex16, TAG_NONE);

      // remove beginning and ending " and removing leading spaces
      memmove(r, r + 1, strlen(r));
      while(r[0] == ' ') memmove(r, r + 1, strlen(r));
      r[strlen(r) - 1] = 0;

      // find the i separating the real and imagynary part
      i = 0;
      while(r[i] != 'i' && r[i] != 0) i++;
      if(r[i] == 0) {
        printf("\nMissformed register complex16 value. Missing i between real and imaginary part.\n");
        abortTest();
      }

      // separate real and imaginary part
      r[i] = 0;
      strcpy(real, r);
      strcpy(imag, r + i + 1);

      // remove leading spaces
      while(imag[0] == ' ') memmove(imag, imag + 1, strlen(imag));

      // removing trailing spaces from real part
      while(real[strlen(real) - 1] == ' ') real[strlen(real) - 1] = 0;

      // removing trailing spaces from imaginary part
      while(imag[strlen(imag) - 1] == ' ') imag[strlen(imag) - 1] = 0;

      // replace , with . in the real part
      for(int i=0; i<(int)strlen(real); i++) {
        if(real[i] == ',') real[i] = '.';
      }

      // replace , with . in the imaginary part
      for(int i=0; i<(int)strlen(imag); i++) {
        if(imag[i] == ',') imag[i] = '.';
      }

      stringToReal16(real, &expectedReal16);
      stringToReal16(imag, &expectedImag16);
      if(!real16CompareEqual(&expectedReal16, REGISTER_REAL16_DATA(regist))) {
        strcat(r, " +ix ");
        strcat(r, imag);
        expectedAndShouldBeValue(regist, letter, r, registerExpectedAndValue);
        if(relativeErrorReal16(&expectedReal16, REGISTER_REAL16_DATA(regist), "real") == RE_INACCURATE) {
          wrongRegisterValue(regist, letter, r);
        }
      }
      else if(!real16CompareEqual(&expectedImag16, REGISTER_IMAG16_DATA(regist))) {
        strcat(r, " +ix ");
        strcat(r, imag);
        expectedAndShouldBeValue(regist, letter, r, registerExpectedAndValue);
        if(relativeErrorReal16(&expectedImag16, REGISTER_IMAG16_DATA(regist), "imaginary") == RE_INACCURATE) {
          wrongRegisterValue(regist, letter, r);
        }
      }
    }
    else if(strcmp(l, "AN16") == 0 || strcmp(l, "AN34") == 0) {
      // find the : separating the angle value from the angular mode
      i = 0;
      while(r[i] != ':' && r[i] != 0) i++;
      if(r[i] != 0) {
        // separate angle value and angular mode
        r[i] = 0;
        strcpy(angMod, r + i + 1);

             if(strcmp(angMod, "DEG"   ) == 0) am = AM_DEGREE;
        else if(strcmp(angMod, "DMS"   ) == 0) am = AM_DMS;
        else if(strcmp(angMod, "GRAD"  ) == 0) am = AM_GRAD;
        else if(strcmp(angMod, "RAD"   ) == 0) am = AM_RADIAN;
        else if(strcmp(angMod, "MULTPI") == 0) am = AM_MULTPI;
        else {
          printf("\nMissformed register angle%d unit. Unknown unit after angle value.\n", strcmp(l, "AN16") == 0 ? 16 : 34);
          abortTest();
        }
      }
      else {
          printf("\nMissformed register angle%d value. Missing colon between angle value and angular mode.\n", strcmp(l, "AN16") == 0 ? 16 : 34);
          abortTest();
      }


      // remove beginning and ending " and removing leading spaces
      memmove(r, r + 1, strlen(r));
      while(r[0] == ' ') memmove(r, r + 1, strlen(r));
      r[strlen(r) - 1] = 0;

      // replace , with .
      for(int i=0; i<(int)strlen(r); i++) {
        if(r[i] == ',') r[i] = '.';
      }

      if(strcmp(l, "AN16") == 0) {
        checkRegisterType(regist, letter, dtAngle16, am);
        stringToReal16(r, &expectedReal16);
        if(!real16CompareEqual(&expectedReal16, REGISTER_REAL16_DATA(regist))) {
          expectedAndShouldBeValue(regist, letter, r, registerExpectedAndValue);
          if(relativeErrorReal16(&expectedReal16, REGISTER_REAL16_DATA(regist), "real") == RE_INACCURATE) {
            wrongRegisterValue(regist, letter, r);
          }
        }
      }
      else {
        checkRegisterType(regist, letter, dtAngle34, am);
        stringToReal34(r, &expectedReal34);
        if(!real34CompareEqual(&expectedReal34, REGISTER_REAL34_DATA(regist))) {
          expectedAndShouldBeValue(regist, letter, r, registerExpectedAndValue);
          if(relativeErrorReal34(&expectedReal34, REGISTER_REAL34_DATA(regist), "real") == RE_INACCURATE) {
            wrongRegisterValue(regist, letter, r);
          }
        }
      }
    }
    else if(strcmp(l, "STRI") == 0) {
      checkRegisterType(regist, letter, dtString, TAG_NONE);
      getString(r + 1);

      char *expected, *is, stringUtf8[200];
      if(stringByteLength(r + 1) != stringByteLength(REGISTER_STRING_DATA(regist))) {
        stringToUtf8(REGISTER_STRING_DATA(regist), (uint8_t *)stringUtf8);
        printf("\nThe 2 strings are not of the same size.\nRegister string: %s\n", stringUtf8);
        for(i=0, is=REGISTER_STRING_DATA(regist); i<=stringByteLength(REGISTER_STRING_DATA(regist)); i++, is++) {
          printf("%02x ", (unsigned char)*is);
        }
        stringToUtf8(r+1, (uint8_t *)stringUtf8);
        printf("\nExpected string: %s\n", stringUtf8);
        for(i=1; i<=stringByteLength(r); i++) {
          printf("%02x ", (unsigned char)r[i]);
        }
        printf("\n");
        abortTest();
      }

      for(i=stringByteLength(r + 1), expected=r + 1, is=REGISTER_STRING_DATA(regist);
          i>0;
          i--, expected++, is++) {
        //printf("%c %02x   %c %02x\n", *expected, (unsigned char)*expected, *is, (unsigned char)*is);
        if(*expected != *is) {
          printf("\nThe 2 strings are different.\nRegister string: ");
          for(i=0, is=REGISTER_STRING_DATA(regist); i<=stringByteLength(REGISTER_STRING_DATA(regist)); i++, is++) {
            printf("%02x ", (unsigned char)*is);
          }
          printf("\nExpected string: ");
          for(i=1; i<=stringByteLength(r); i++) {
            printf("%02x ", (unsigned char)r[i]);
          }
          printf("\n");
          abortTest();
        }
      }
    }
    else if(strcmp(l, "SHOI") == 0) {
      // find the # separating the value from the base
      i = 0;
      while(r[i] != '#' && r[i] != 0) i++;
      if(r[i] == 0) {
        printf("\nMissformed register short integer value. Missing # between value and base.\n");
        abortTest();
      }

      // remove beginning and ending " and removing leading spaces
      memmove(r, r + 1, strlen(r));
      while(r[0] == ' ') memmove(r, r + 1, strlen(r));
      r[strlen(r) - 1] = 0;

      // Convert string to upper case
      for(i=0; r[i]!=0; i++) {
        if('a' <= r[i] && r[i] <= 'z') {
          r[i] -= 32;
        }
      }

      opX = allocateTemporaryRegister();
      strToShortInteger(r, opX);
      checkRegisterType(regist, letter, dtShortInteger, getRegisterTag(opX));
      if(*REGISTER_SHORT_INTEGER_DATA(opX) != *REGISTER_SHORT_INTEGER_DATA(regist)) {
        wrongRegisterValue(regist, letter, r);
      }
      freeTemporaryRegister(opX);
    }
    else if(strcmp(l, "RE34") == 0) {
      checkRegisterType(regist, letter, dtReal34, TAG_NONE);

      memmove(r, r + 1, strlen(r));
      while(r[0] == ' ') memmove(r, r + 1, strlen(r));
      r[strlen(r) - 1] = 0;

      while(r[strlen(r) - 1] == ' ') r[strlen(r) - 1] = 0;
      for(int i=0; i<(int)strlen(r); i++) {
        if(r[i] == ',') r[i] = '.';
      }

      stringToReal34(r, &expectedReal34);
      if(!real34CompareEqual(&expectedReal34, REGISTER_REAL34_DATA(regist))) {
        expectedAndShouldBeValue(regist, letter, r, registerExpectedAndValue);
        if(relativeErrorReal34(&expectedReal34, REGISTER_REAL34_DATA(regist), "real") == RE_INACCURATE) {
          wrongRegisterValue(regist, letter, r);
        }
      }
    }
    else if(strcmp(l, "CO34") == 0) {
      checkRegisterType(regist, letter, dtComplex34, TAG_NONE);

      // remove beginning and ending " and removing leading spaces
      memmove(r, r + 1, strlen(r));
      while(r[0] == ' ') memmove(r, r + 1, strlen(r));
      r[strlen(r) - 1] = 0;

      // find the i separating the real and imagynary part
      i = 0;
      while(r[i] != 'i' && r[i] != 0) i++;
      if(r[i] == 0) {
        printf("\nMissformed register complex34 value. Missing i between real and imaginary part.\n");
        abortTest();
      }

      // separate real and imaginary part
      r[i] = 0;
      strcpy(real, r);
      strcpy(imag, r + i + 1);

      // remove leading spaces
      while(imag[0] == ' ') memmove(imag, imag + 1, strlen(imag));

      // removing trailing spaces from real part
      while(real[strlen(real) - 1] == ' ') real[strlen(real) - 1] = 0;

      // removing trailing spaces from imaginary part
      while(imag[strlen(imag) - 1] == ' ') imag[strlen(imag) - 1] = 0;

      // replace , with . in the real part
      for(int i=0; i<(int)strlen(real); i++) {
        if(real[i] == ',') real[i] = '.';
      }

      // replace , with . in the imaginary part
      for(int i=0; i<(int)strlen(imag); i++) {
        if(imag[i] == ',') imag[i] = '.';
      }

      stringToReal34(real, &expectedReal34);
      stringToReal34(imag, &expectedImag34);
      if(!real34CompareEqual(&expectedReal34, REGISTER_REAL34_DATA(regist))) {
        strcat(r, " +ix ");
        strcat(r, imag);
        expectedAndShouldBeValue(regist, letter, r, registerExpectedAndValue);
        if(relativeErrorReal34(&expectedReal34, REGISTER_REAL34_DATA(regist), "real") == RE_INACCURATE) {
          wrongRegisterValue(regist, letter, r);
        }
      }
      else if(!real34CompareEqual(&expectedImag34, REGISTER_IMAG34_DATA(regist))) {
        strcat(r, " +ix ");
        strcat(r, imag);
        expectedAndShouldBeValue(regist, letter, r, registerExpectedAndValue);
        if(relativeErrorReal34(&expectedImag34, REGISTER_IMAG34_DATA(regist), "imaginary") == RE_INACCURATE) {
          wrongRegisterValue(regist, letter, r);
        }
      }
    }
    else {
      printf("\nMissformed register value. Unknown data type %s for register %s\n", l, p+1);
      abortTest();
    }
  }

  else {
    printf("\nUnknown checking %s\n", l);
    abortTest();
  }
}



void outParameters(char *token) {
  char parameter[300];
  int32_t index, lg;

  while(*token == ' ') token++;
  while(*token != 0) {
    index = 0;
    while(*token != ' ' && *token != 0) {
      if(*token == '"') { // Inside a string
        lg = endOfString(token) - token;
        strncpy(parameter + index, token, lg--);
        index += lg;
        token += lg;
      }
      parameter[index++] = *(token++);
    }
    parameter[index] = 0;

    //printf("  Check %s\n", parameter);
    checkExpectedOutParameter(parameter);

    while(*token == ' ') token++;
  }
}



void callFunction(void) {
  lastErrorCode = 0;

  switch(funcType) {
    case FUNC_NOPARAM:
      funcNoParam(NOPARAM);
      break;

    case FUNC_CVT:
      funcCvt(NOPARAM);
      break;

    default: {}
  }

  if(lastErrorCode == 0) {
    if(functionIndex < LAST_ITEM) {
      if(indexOfItems[functionIndex].stackLiftStatus == SLS_DISABLED) {
        stackLiftEnabled = false;
      }
      else if(indexOfItems[functionIndex].stackLiftStatus == SLS_ENABLED) {
        stackLiftEnabled = true;
      }
    }
  }
}



void functionToCall(char *functionName) {
  int32_t function;

  function = 0;
  while(funcTestNoParam[function].name[0] != 0 && strcmp(funcTestNoParam[function].name, functionName) != 0) {
    function++;
  }
  if(funcTestNoParam[function].name[0] != 0) {
    funcNoParam = funcTestNoParam[function].func;
    funcType = FUNC_NOPARAM;

    for(functionIndex=1; functionIndex<=LAST_ITEM; functionIndex++) {
      if(indexOfItems[functionIndex].func == funcNoParam) {
        break;
      }
    }

    if(functionIndex >= LAST_ITEM) {
      printf("\nThe function %s must be somewhere in the indexOfItems array!\n", functionName);
      abortTest();
    }

    //printf("%s=%d\n", functionName, functionIndex);
    return;
  }

  else {
    function = 0;
    while(funcTestCvt[function].name[0] != 0 && strcmp(funcTestCvt[function].name, functionName) != 0) {
      function++;
    }
    if(funcTestCvt[function].name[0] != 0) {
      //ici il faut tester la présence de fromUnitToISO ou fromISOtoUnit

      funcCvt = funcTestCvt[function].func;
      funcType = FUNC_CVT;

      for(functionIndex=1; functionIndex<=LAST_ITEM; functionIndex++) {
        if(indexOfItems[functionIndex].func == funcCvt) {
          break;
        }
      }

      if(functionIndex >= LAST_ITEM) {
        printf("\nThe function %s must be somewhere in the indexOfItems array!\n", functionName);
        abortTest();
      }

      return;
    }

    else {
    }
  }

  printf("\nCannot find the function to test: check spelling of the function name and remember the name is case sensitive\n");
  abortTest();
}



void abortTest(void) {
  printf("\n%s\n", lastInParameters);
  printf("%s\n", line);
  printf("in file %s line %d\n", fileName, lineNumber);
  exit(-1);
}



void standardizeLine(void) {
  char *location;

  // trim comments
  location = strstr(line, ";");
  if(location != NULL) {
    *location = 0;
  }

  // trim ending LF
  location = strstr(line, "\n");
  if(location != NULL) {
    *location = 0;
  }

  // trim ending CR
  location = strstr(line, "\r");
  if(location != NULL) {
    *location = 0;
  }

  // trim ending LF
  location = strstr(line, "\n");
  if(location != NULL) {
    *location = 0;
  }

  // Change tabs in spaces
  for(int i=strlen(line)-1; i>0; i--) {
    if(line[i] == '\t') {
      line[i] = ' ';
    }
  }

  // Trim ending spaces
  for(int i=strlen(line)-1; i>0; i--) {
    if(line[i] == ' ') {
      line[i] = 0;
    }
    else {
      break;
    }
  }

  // Trim beginning spaces
  while(line[0] == ' ') {
    memmove(line, line + 1, strlen(line));
  }
}



void processLine(void) {
  // convert to upper case
  int32_t lg = strlen(line);
  for(int i=0; i<lg; i++) {
    if(line[i] == '"') {
      i = endOfString(line + i) - line;
    }

    if('a' <= line[i] && line[i] <= 'z') {
      line[i] -= 32;
    }
    if(i >= 5 && strncmp(line, "FUNC: ", 6) == 0) {
      break;
    }
  }

  if(strncmp(line, "IN: ", 4) == 0) {
    //printf("%s\n", line);
    strcpy(lastInParameters, line);
    inParameters(line + 4);
  }

  else if(strncmp(line, "FUNC: ", 6) == 0) {
    //printf("%s\n", line);
    functionToCall(line + 6);
  }

  else if(strncmp(line, "OUT: ", 5) == 0) {
    //printf("%s\n", line);
    callFunction();

    if((numTestsFile++ % 10) == 0) {
      printf(".");
    }

    numTestsTotal++;
    outParameters(line + 5);
  }

  else if(line[0] != 0) {
    printf("\nLine cannot be processed\n%s\n", line);
    abortTest();
  }
}



void processOneFile(void) {
  FILE *testSuite;

  numTestsFile = 0;

  strcpy(fileName, line);
  sprintf(filePathName, "%s/%s", filePath, fileName);

  printf("Performing tests from file %s ", filePathName);

  testSuite = fopen(filePathName, "rb");
  if(testSuite == NULL) {
    printf("Cannot open file %s!\n", fileName);
    exit(-1);
  }

  // Default function to call
  functionIndex = ITM_NOP;
  funcNoParam = fnNop;
  funcType = FUNC_NOPARAM;

  fgets(line, 999, testSuite);
  lineNumber = 1;
  while(!feof(testSuite)) {
    standardizeLine();
    processLine();
    fgets(line, 999, testSuite);
    lineNumber++;
  }

  fclose(testSuite);

//  printf(" %d passed successfully\n", numTestsFile);
  printf("\n");
}



void processTests(void) {
  FILE *fileList;

  numTestsTotal = 0;

  strcpy(filePath, "src/testSuite"); // without trailing /

  sprintf(filePathName, "%s/testSuiteList.txt", filePath);
  fileList = fopen(filePathName, "rb");
  if(fileList == NULL) {
    printf("Cannot open file fileList.txt!\n");
    exit(-1);
  }

  fgets(line, 999, fileList);
  while(!feof(fileList)) {
    standardizeLine();
    if(line[0] != 0) {
      processOneFile();
    }
    fgets(line, 999, fileList);
  }

  fclose(fileList);

  printf("\n************************************\n");
  printf("* %6d TESTS PASSED SUCCESSFULLY *\n", numTestsTotal);
  printf("************************************\n");
}
