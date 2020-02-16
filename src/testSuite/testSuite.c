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

#define NUMBER_OF_CORRECT_SIGNIFICANT_DIGITS_EXPECTED 34


extern const int16_t menu_FCNS[];
extern const int16_t menu_CNST[];
extern const int16_t menu_MENUS[];
extern const softmenu_t softmenu[];
char line[100000], lastInParameters[10000], fileName[1000], filePath[1000], filePathName[2000], registerExpectedAndValue[1000], realString[1000];
int32_t lineNumber, numTestsFile, numTestsTotal;
int32_t functionIndex, funcType, correctSignificantDigits;
void (*funcNoParam)(uint16_t);
void (*funcCvt)(uint16_t);

const funcTest_t funcTestNoParam[] = {
  {"fn10Pow",                fn10Pow               },
  {"fn2Pow",                 fn2Pow                },
  {"fnAdd",                  fnAdd                 },
  {"fnAim",                  fnAim                 },
  {"fnAgm",                  fnAgm                 },
  {"fnArccos",               fnArccos              },
  {"fnArccosh",              fnArccosh             },
  {"fnArcsin",               fnArcsin              },
  {"fnArcsinh",              fnArcsinh             },
  {"fnArctan",               fnArctan              },
  {"fnArctanh",              fnArctanh             },
  {"fnBatteryVoltage",       fnBatteryVoltage      },
  {"fnCeil",                 fnCeil                },
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
  {"fnCos",                  fnCos                 },
  {"fnCosh",                 fnCosh                },
  {"fnCube",                 fnCube                },
  {"fnCubeRoot",             fnCubeRoot            },
  {"fnCxToRe",               fnCxToRe              },
  {"fnCvtCToF",              fnCvtCToF             },
  {"fnCvtDegToDms",          fnCvtDegToDms         },
  {"fnCvtDegToRad",          fnCvtDegToRad         },
  {"fnCvtDmsToDeg",          fnCvtDmsToDeg         },
  {"fnCvtFToC",              fnCvtFToC             },
  {"fnCvtRadToDeg",          fnCvtRadToDeg         },
  {"fnCyx",                  fnCyx                 },  
  {"fnDenMax",               fnDenMax              },
  {"fnDivide",               fnDivide              },
  {"fnDrop",                 fnDrop                },
  {"fnDropY",                fnDropY               },
  {"fnExp",                  fnExp                 },
  {"fnExpt",                 fnExpt                },
  {"fnFactorial",            fnFactorial           },
  {"fnFillStack",            fnFillStack           },
  {"fnFloor",                fnFloor               },
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
  {"fnIDiv",                 fnIDiv                },
  {"fnIDivR",                fnIDivR               },
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
  {"fnMin",                  fnMin                 },
  {"fnMax",                  fnMax                 },
  {"fnMant",                 fnMant                },
  {"fnMirror",               fnMirror              },
  {"fnMod",                  fnMod                 },
  {"fnMultiply",             fnMultiply            },
  {"fnNeighb",               fnNeighb              },
  {"fnNop",                  fnNop                 },
  {"fnParallel",             fnParallel            },
  {"fnPi",                   fnPi                  },
  {"fnPower",                fnPower               },
  {"fnPyx",                  fnPyx                 },
  {"fnRealPart",             fnRealPart            },
  {"fnReToCx",               fnReToCx              },
  {"fnRmd",                  fnRmd                 },
  {"fnRollDown",             fnRollDown            },
  {"fnRollUp",               fnRollUp              },
  {"fnRound",                fnRound               },
  {"fnRoundi",               fnRoundi              },
  {"fnSign",                 fnSign                },
  {"fnSin",                  fnSin                 },
  {"fnSinh",                 fnSinh                },
  {"fnSlvq",                 fnSlvq                },
  {"fnSquare",               fnSquare              },
  {"fnSquareRoot",           fnSquareRoot          },
  {"fnSubtract",             fnSubtract            },
  {"fnSwapRealImaginary",    fnSwapRealImaginary   },
  {"fnSwapXY",               fnSwapXY              },
  {"fnTan",                  fnTan                 },
  {"fnTanh",                 fnTanh                },
  {"fnToPolar",              fnToPolar             },
  {"fnToRect",               fnToRect              },
  {"fnUlp",                  fnUlp                 },
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
  // Any change in this function must be reported in the function closeNim from file bufferize.c after the line: else if(nimNumberPart == NP_INT_BASE) {
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

  longIntegerInit(value);
  nimBuffer[posHash] = 0;
  stringToLongInteger(nimBuffer + (nimBuffer[0] == '+' ? 1 : 0), base, value);

  // maxVal = 2^shortIntegerWordSize
  longIntegerInit(maxVal);
  if(shortIntegerWordSize >= 1 && shortIntegerWordSize <= 64) {
    longInteger2Pow(shortIntegerWordSize, maxVal);
  }
  else {
    printf("\nError while initializing a short integer: shortIntegerWordSize must be fom 1 to 64\n");
    abortTest();
  }

  // minVal = -maxVal/2
  longIntegerInit(minVal);
  longIntegerDivideUInt(maxVal, 2, minVal); // minVal = maxVal / 2
  longIntegerSetNegativeSign(minVal); // minVal = -minVal

  if(shortIntegerMode != SIM_UNSIGN) {
    longIntegerDivideUInt(maxVal, 2, maxVal); // maxVal /= 2
  }

  longIntegerSubtractUInt(maxVal, 1, maxVal); // maxVal--

  if(shortIntegerMode == SIM_UNSIGN) {
    longIntegerSetZero(minVal); // minVal = 0
  }

  if(shortIntegerMode == SIM_1COMPL || shortIntegerMode == SIM_SIGNMT) {
    longIntegerAddUInt(minVal, 1, minVal); // minVal++
  }

  if(longIntegerCompare(value, minVal) < 0 || longIntegerCompare(value, maxVal) > 0) {
    char strMin[22], strMax[22];
    longIntegerToAllocatedString(minVal, strMin, sizeof(strMin));
    longIntegerToAllocatedString(maxVal, strMax, sizeof(strMax));
    printf("\nError while initializing a short integer: for a word size of %d bit%s and integer mode %s, the entered number must be from %s to %s!\n", shortIntegerWordSize, shortIntegerWordSize>1 ? "s" : "", getShortIntegerModeName(shortIntegerMode), strMin, strMax);
    abortTest();
  }

  reallocateRegister(regist, dtShortInteger, SHORT_INTEGER_SIZE, base);

  char strValue[22];
  longIntegerToAllocatedString(value, strValue, sizeof(strValue));

  uint64_t val = strtoull(strValue + (longIntegerIsNegative(value) ? 1 : 0), NULL, 10); // when value is negative: discard the minus sign

  if(shortIntegerMode == SIM_UNSIGN) {
  }
  else if(shortIntegerMode == SIM_2COMPL) {
    if(longIntegerIsNegative(value)) {
      val = (~val + 1) & shortIntegerMask;
    }
  }
  else if(shortIntegerMode == SIM_1COMPL) {
    if(longIntegerIsNegative(value)) {
      val = ~val & shortIntegerMask;
    }
  }
  else if(shortIntegerMode == SIM_SIGNMT) {
    if(longIntegerIsNegative(value)) {
      val = (val & shortIntegerMask) | shortIntegerSignBit;
    }
  }
  else {
    printf("\nBad integer mode while initializing a short integer\n");
    abortTest();
  }

  *(REGISTER_SHORT_INTEGER_DATA(regist)) = val;

  longIntegerFree(minVal);
  longIntegerFree(value);
  longIntegerFree(maxVal);
}



char hexToChar(const char *string) {
    // the itialisation to zero prevents a 'variable used is not initialized' warning on Mac:
    char ch=0;

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
  char l[200], r[200], real[200], imag[200], angMod[200]; //, letter;
  int32_t i, am;

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

  //Setting Complex mode
  else if(strcmp(l, "CM") == 0) {
    if(strcmp(r, "RECT") == 0) {
      complexMode = CM_RECTANGULAR;
      //printf("  Set complex mode to RECT\n");
    }
    else if(strcmp(r, "POLAR") == 0) {
      complexMode = CM_POLAR;
      //printf("  Set complex mode to POLAR\n");
    }
    else {
      printf("\nMissformed complex mode setting. The rvalue must be RECT or POLAR.\n");
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

  //Setting a register
  else if(l[0] == 'R') {
    calcRegister_t regist;

    //Lettered register
    if(l[1] >= 'A' && l[2] == 0) {
      if(strstr("XYZTABCDLIJK", l + 1) != NULL) {
        //letter = l[1];
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
      if(regist > 111 || regist < 0) {
        printf("\nMissformed numbered register setting. Th number after R shall be a number from 0 to 111.\n");
        abortTest();
      }
      //letter = 0;
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
      longInteger_t lgInt;

      // remove beginning and ending " and removing leading spaces
      memmove(r, r + 1, strlen(r));
      while(r[0] == ' ') memmove(r, r + 1, strlen(r));
      r[strlen(r) - 1] = 0;

      longIntegerInit(lgInt);
      stringToLongInteger(r, 10, lgInt);
      convertLongIntegerToLongIntegerRegister(lgInt, regist);
      longIntegerFree(lgInt);
    }
    else if(strcmp(l, "REAL") == 0) {
      // find the : separating the real value from the angular mode
      i = 0;
      while(r[i] != ':' && r[i] != 0) i++;
      if(r[i] == 0) {
        strcat(r, ":NONE");
      }

      // separate real value and angular mode
      r[i] = 0;
      strcpy(angMod, r + i + 1);

           if(strcmp(angMod, "DEG"   ) == 0) am = AM_DEGREE;
      else if(strcmp(angMod, "DMS"   ) == 0) am = AM_DMS;
      else if(strcmp(angMod, "GRAD"  ) == 0) am = AM_GRAD;
      else if(strcmp(angMod, "RAD"   ) == 0) am = AM_RADIAN;
      else if(strcmp(angMod, "MULTPI") == 0) am = AM_MULTPI;
      else if(strcmp(angMod, "NONE"  ) == 0) am = AM_NONE;
      else {
        printf("\nMissformed register real%d angular mode. Unknown angular mode after real value.\n", strcmp(l, "RE16") == 0 ? 16 : 34);
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

      reallocateRegister(regist, dtReal34, REAL34_SIZE, am);
      stringToReal34(r, REGISTER_REAL34_DATA(regist));
    }
    else if(strcmp(l, "STRI") == 0) {
      getString(r + 1);
      reallocateRegister(regist, dtString, stringByteLength(r + 1), AM_NONE);
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
    else if(strcmp(l, "CPLX") == 0) {
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

      reallocateRegister(regist, dtComplex34, COMPLEX34_SIZE, AM_NONE);
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

  strReplace(token, "inf", "9e9999");

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
    printRegisterToConsole(regist);
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
      printRegisterToConsole(regist);
      printf("\n");
      abortTest();
    }
    else if(getRegisterDataType(regist) == dtReal34) {
      if(letter == 0) {
        printf("\nRegister %u should be a real tagged %s but it is tagged %s!\n", regist, getAngularModeName(expectedTag), getAngularModeName(getRegisterAngularMode(regist)));
        printf("R%u = ", regist);
      }
      else {
        printf("\nRegister %c should be a real tagged %s but it is tagged %s!\n", letter, getAngularModeName(expectedTag), getAngularModeName(getRegisterAngularMode(regist)));
        printf("R%c = ", letter);
      }
      printRegisterToConsole(regist);
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
      printRegisterToConsole(regist);
      printf("\n");
      abortTest();
    }
  }
}



int relativeErrorReal34(real34_t *expectedValue34, real34_t *value34, char *numberPart, calcRegister_t regist, char letter) {
  real39_t expectedValue, value, relativeError;

  real34ToReal(expectedValue34, &expectedValue);
  real34ToReal(value34, &value);

  realSubtract(&expectedValue, &value, &relativeError, &ctxtReal39);

  if(!realIsZero(&expectedValue)) {
    realDivide(&relativeError, &expectedValue, &relativeError, &ctxtReal39);
  }
  else {
    realCopy(&value, &relativeError);
  }
  realSetPositiveSign(&relativeError);

  correctSignificantDigits = -relativeError.exponent - relativeError.digits;
  ctxtReal39.digits = 2;
  realPlus(&relativeError, &relativeError, &ctxtReal39);
  ctxtReal39.digits = 39;
  if(correctSignificantDigits <= 34) {
    //printf("\nThere are only %d correct significant digits in the %s part of the value: %d are expected!\n", correctSignificantDigits, numberPart, NUMBER_OF_CORRECT_SIGNIFICANT_DIGITS_EXPECTED);
    realToString(&relativeError, realString);
    if(letter == 0) {
      printf("\nThere are only %d correct significant digits in the %s part of register %d! Relative error is %s\n", correctSignificantDigits, numberPart, regist, realString);
      printf("R%d = ", regist);
      printReal34ToConsole(value34);
      printf("\n");
    }
    else {
      printf("\nThere are only %d correct significant digits in the %s part of register %c! Relative error is %s\n", correctSignificantDigits, numberPart, letter, realString);
      printf("%c = ", letter);
      printReal34ToConsole(value34);
      printf("\n");
    }
    printf("%s\n", lastInParameters);
    printf("%s\n", line);
    printf("in file %s line %d\n", fileName, lineNumber);
    if(correctSignificantDigits < 32 && correctSignificantDigits < NUMBER_OF_CORRECT_SIGNIFICANT_DIGITS_EXPECTED) {
      puts(registerExpectedAndValue);
      exit(-1);
    }
  }

  return (correctSignificantDigits < 32 && correctSignificantDigits < NUMBER_OF_CORRECT_SIGNIFICANT_DIGITS_EXPECTED) ? RE_INACCURATE : RE_ACCURATE;
}



void wrongRegisterValue(calcRegister_t regist, char letter, char *expectedValue) {
  if(letter == 0) {
    printf("\nRegister %u value should be ", regist);
  }
  else {
    printf("\nRegister %c value should be ", letter);
  }
  printf("%s\nbut it is ", expectedValue);
  printRegisterToConsole(regist);
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



bool_t real34AreEqual(real34_t *a, real34_t *b) {
  if( real34IsNaN(a) &&  real34IsNaN(b)) return true;
  if( real34IsNaN(a) && !real34IsNaN(b)) return false;
  if(!real34IsNaN(a) &&  real34IsNaN(b)) return false;

  if( real34IsInfinite(a) && !real34IsInfinite(b)) return false;
  if(!real34IsInfinite(a) &&  real34IsInfinite(b)) return false;
  if(real34IsInfinite(a) && real34IsInfinite(b)) {
    if(real34IsPositive(a) && real34IsPositive(b)) return true;
    if(real34IsNegative(a) && real34IsNegative(b)) return true;
    return false;
  }

  return real34CompareEqual(a, b);
}



void checkExpectedOutParameter(char *p) {
  char l[200], r[200], real[200], imag[200], angMod[200], letter;
  int32_t i, am;
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

  //Checking complex mode
  else if(strcmp(l, "CM") == 0) {
    if(strcmp(r, "RECT") == 0) {
      if(complexMode != CM_RECTANGULAR) {
        printf("\ncomplex mode should be RECT but it is not!\n");
        abortTest();
      }
    }
    else if(strcmp(r, "POLAR") == 0) {
      if(complexMode != CM_POLAR) {
        printf("\ncomplex mode should be POLAR but it is not!\n");
        abortTest();
      }
    }
    else {
      printf("\nMissformed complex mode checking. The rvalue must be RECT or POLAR.\n");
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
      if(regist > 111 || regist < 0) {
        printf("\nMissformed numbered register checking. The number after R shall be a number from 0 to 111.\n");
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

      longIntegerInit(expectedLongInteger);
      stringToLongInteger(r, 10, expectedLongInteger);
      checkRegisterType(regist, letter, dtLongInteger, longIntegerSignTag(expectedLongInteger));
      convertLongIntegerRegisterToLongInteger(regist, registerLongInteger);
      if(longIntegerCompare(expectedLongInteger, registerLongInteger) != 0) {
        wrongRegisterValue(regist, letter, r);
      }

      longIntegerFree(expectedLongInteger);
      longIntegerFree(registerLongInteger);
    }
    else if(strcmp(l, "REAL") == 0) {
      // find the : separating the real value from the angular mode
      i = 0;
      while(r[i] != ':' && r[i] != 0) i++;
      if(r[i] == 0) {
        strcat(r, ":NONE");
      }

      // separate real value and angular mode
      r[i] = 0;
      strcpy(angMod, r + i + 1);

           if(strcmp(angMod, "DEG"   ) == 0) am = AM_DEGREE;
      else if(strcmp(angMod, "DMS"   ) == 0) am = AM_DMS;
      else if(strcmp(angMod, "GRAD"  ) == 0) am = AM_GRAD;
      else if(strcmp(angMod, "RAD"   ) == 0) am = AM_RADIAN;
      else if(strcmp(angMod, "MULTPI") == 0) am = AM_MULTPI;
      else if(strcmp(angMod, "NONE"  ) == 0) am = AM_NONE;
      else {
        printf("\nMissformed register real%d angular mode. Unknown angular mode after real value.\n", strcmp(l, "RE16") == 0 ? 16 : 34);
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

      checkRegisterType(regist, letter, dtReal34, am);
      stringToReal34(r, &expectedReal34);
//printf("\nexpectedReal34 = "); printReal34ToConsole(&expectedReal34); printf("\n");
//printf("\nR%d = ", regist); printRegisterToConsole(regist); printf("\n");
//printf("\n1<%s|%s|%s>\n", p, l, r);
      if(!real34AreEqual(REGISTER_REAL34_DATA(regist), &expectedReal34)) {
        expectedAndShouldBeValue(regist, letter, r, registerExpectedAndValue);
        if(relativeErrorReal34(&expectedReal34, REGISTER_REAL34_DATA(regist), "real", regist, letter) == RE_INACCURATE) {
          wrongRegisterValue(regist, letter, r);
        }
//printf("\n2<%s|%s|%s>\n", p, l, r);
      }
    }
    else if(strcmp(l, "STRI") == 0) {
      checkRegisterType(regist, letter, dtString, AM_NONE);
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

      strToShortInteger(r, TEMP_REGISTER);
      checkRegisterType(regist, letter, dtShortInteger, getRegisterTag(TEMP_REGISTER));
      if(*REGISTER_SHORT_INTEGER_DATA(TEMP_REGISTER) != *REGISTER_SHORT_INTEGER_DATA(regist)) {
        wrongRegisterValue(regist, letter, r);
      }
    }
    else if(strcmp(l, "CPLX") == 0) {
      checkRegisterType(regist, letter, dtComplex34, AM_NONE);

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
      if(!real34AreEqual(REGISTER_REAL34_DATA(regist), &expectedReal34)) {
        strcat(r, " +ix ");
        strcat(r, imag);
        expectedAndShouldBeValue(regist, letter, r, registerExpectedAndValue);
        if(relativeErrorReal34(&expectedReal34, REGISTER_REAL34_DATA(regist), "real", regist, letter) == RE_INACCURATE) {
          wrongRegisterValue(regist, letter, r);
        }
      }
      else if(!real34AreEqual(REGISTER_IMAG34_DATA(regist), &expectedImag34)) {
        strcat(r, " +ix ");
        strcat(r, imag);
        expectedAndShouldBeValue(regist, letter, r, registerExpectedAndValue);
        if(relativeErrorReal34(&expectedImag34, REGISTER_IMAG34_DATA(regist), "imaginary", regist, letter) == RE_INACCURATE) {
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

  strReplace(token, "inf", "9e9999");

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

  // 2 spaces ==> 1 space
  for(uint32_t i=0; i<strlen(line); i++) {
    if(line[i] == '"') {
      i = endOfString(line + i) - line;
    }
    if(line[i] == ' ' && line[i + 1] == ' ') {
      memmove(line + i, line + i + 1, strlen(line + i) - 1);
      line[strlen(line) - 1] = 0;
      i--;
    }
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
  strcat(fileName, ".txt");
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

  fgets(line, 9999, testSuite);
  lineNumber = 1;
  while(!feof(testSuite)) {
    standardizeLine();
    while(strlen(line) >= 4 && strncmp(line + strlen(line) - 4, " ...", 4) == 0) {
      line[strlen(line) - 3] = 0;
      if(!feof(testSuite)) {
        fgets(line + strlen(line), 9999, testSuite);
        lineNumber++;
        standardizeLine();
      }
    }
    processLine();
    fgets(line, 9999, testSuite);
    lineNumber++;
  }

  fclose(testSuite);

//  printf(" %d passed successfully\n", numTestsFile);
  printf("\n");
}



void checkOneCatalogSorting(const int16_t *catalog, int16_t catalogId, const char *catalogName) {
  int32_t i, nbElements, cmp;

  for(nbElements=0, i=0; softmenu[i].menuId; i++) {
    if(softmenu[i].menuId == -catalogId) {
      nbElements = softmenu[i].numItems;
      break;
    }
  }
  if(nbElements == 0) {
    printf("MNU_%s (-%d) not found in structure softmenu!\n", catalogName, catalogId);
    exit(1);
  }

  printf("Checking sort order of catalog %s (%d elements)\n", catalogName, nbElements);

  for(i=1; i<nbElements; i++) {
    if((cmp = compareString(indexOfItems[abs(catalog[i - 1])].itemCatalogName, indexOfItems[abs(catalog[i])].itemCatalogName, CMP_EXTENSIVE)) >= 0) {
      printf("In catalog %s, element %d (item %d) should be after element %d (item %d). cmp = %d\n", catalogName, i - 1, catalog[i - 1], i, catalog[i], cmp);
      exit(1);
    }
  }
}



void checkCatalogsSorting(void) {
  //compareString(indexOfItems[234].itemCatalogName, indexOfItems[245].itemCatalogName, CMP_EXTENSIVE);
  checkOneCatalogSorting(menu_FCNS,  MNU_FCNS,  "FCNS");
  checkOneCatalogSorting(menu_CNST,  MNU_CNST,  "CNST");
  checkOneCatalogSorting(menu_MENUS, MNU_MENUS, "MENUS");
}



void processTests(void) {
  FILE *fileList;

  checkCatalogsSorting();

  numTestsTotal = 0;

  strcpy(filePath, "src/testSuite"); // without trailing /

  sprintf(filePathName, "%s/testSuiteList.txt", filePath);
  fileList = fopen(filePathName, "rb");
  if(fileList == NULL) {
    printf("Cannot open file testSuiteList.txt!\n");
    exit(-1);
  }

  fgets(line, 9999, fileList);
  while(!feof(fileList)) {
    standardizeLine();
    if(line[0] != 0) {
      processOneFile();
    }
    fgets(line, 9999, fileList);
  }

  fclose(fileList);

  printf("\n************************************\n");
  printf("* %6d TESTS PASSED SUCCESSFULLY *\n", numTestsTotal);
  printf("************************************\n");
}
