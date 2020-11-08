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
 * \file config.c
 ***********************************************/

#include "wp43s.h"



/********************************************//**
 * \brief Sets the calc to meet Chinese standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigChina(uint16_t unusedParamButMandatory) {
  setSystemFlag(FLAG_DECIMP);
  groupingGap = 4;
  setSystemFlag(FLAG_TDM24); // time format = 24H
  clearSystemFlag(FLAG_DMY); // date format
  clearSystemFlag(FLAG_MDY); // date format
  setSystemFlag(FLAG_YMD);   // date format
  firstGregorianDay = 1949;  // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet European standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigEurope(uint16_t unusedParamButMandatory) {
  clearSystemFlag(FLAG_DECIMP);
  groupingGap = 3;
  setSystemFlag(FLAG_TDM24); // time format = 24H
  clearSystemFlag(FLAG_MDY); // date format
  clearSystemFlag(FLAG_YMD); // date format
  setSystemFlag(FLAG_DMY);   // date format
  firstGregorianDay = 1582;  // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet Indian standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigIndia(uint16_t unusedParamButMandatory) {
  setSystemFlag(FLAG_DECIMP);
  groupingGap = 3;
  setSystemFlag(FLAG_TDM24); // time format = 24H
  clearSystemFlag(FLAG_MDY); // date format
  clearSystemFlag(FLAG_YMD); // date format
  setSystemFlag(FLAG_DMY);   // date format
  firstGregorianDay = 1752;  // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet Japanese standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigJapan(uint16_t unusedParamButMandatory) {
  setSystemFlag(FLAG_DECIMP);
  groupingGap = 3;
  setSystemFlag(FLAG_TDM24); // time format = 24H
  clearSystemFlag(FLAG_MDY); // date format
  clearSystemFlag(FLAG_DMY); // date format
  setSystemFlag(FLAG_YMD);   // date format
  firstGregorianDay = 1873;  // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet English standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigUk(uint16_t unusedParamButMandatory) {
  setSystemFlag(FLAG_DECIMP);
  groupingGap = 3;
  clearSystemFlag(FLAG_TDM24); // time format = 12H
  clearSystemFlag(FLAG_MDY);   // date format
  clearSystemFlag(FLAG_YMD);   // date format
  setSystemFlag(FLAG_DMY);     // date format
  firstGregorianDay = 1752;    // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet American standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigUsa(uint16_t unusedParamButMandatory) {
  setSystemFlag(FLAG_DECIMP);
  groupingGap = 3;
  clearSystemFlag(FLAG_TDM24); // time format = 12H
  clearSystemFlag(FLAG_YMD);   // date format
  clearSystemFlag(FLAG_DMY);   // date format
  setSystemFlag(FLAG_MDY);     // date format
  firstGregorianDay = 1752;    // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the integer mode
 *
 * \param[in] mode uint16_t Integer mode
 * \return void
 ***********************************************/
void fnIntegerMode(uint16_t mode) {
  shortIntegerMode = mode;
}



/********************************************//**
 * \brief Displays credits to the brave men who
 * made this project work.
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnWho(uint16_t unusedParamButMandatory) {
  temporaryInformation = TI_WHO;
 }



/********************************************//**
 * \brief Displays the version of this software
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnVersion(uint16_t unusedParamButMandatory) {
  temporaryInformation = TI_VERSION;
}



/********************************************//**
 * \brief Sets X to the amount of free RAM
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnFreeMemory(uint16_t unusedParamButMandatory) {
  longInteger_t mem;

  liftStack();

  longIntegerInit(mem);
  uIntToLongInteger(getFreeRamMemory(), mem);
  convertLongIntegerToLongIntegerRegister(mem, REGISTER_X);
  longIntegerFree(mem);
}



/********************************************//**
 * \brief Sets X to the value of the rounding mode
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGetRoundingMode(uint16_t unusedParamButMandatory) {
  longInteger_t rounding;

  liftStack();

  longIntegerInit(rounding);
  uIntToLongInteger(roundingMode, rounding);
  convertLongIntegerToLongIntegerRegister(rounding, REGISTER_X);
  longIntegerFree(rounding);
}



/********************************************//**
 * \brief Sets X to the value of the integer mode
 *
 * \return void
 ***********************************************/
void fnGetIntegerSignMode(uint16_t unusedParamButMandatory) {
  longInteger_t ism;

  liftStack();

  longIntegerInit(ism);
  uIntToLongInteger((shortIntegerMode==SIM_2COMPL ? 2 : (shortIntegerMode==SIM_1COMPL ? 1 : (shortIntegerMode==SIM_UNSIGN ? 0 : -1))), ism);
  convertLongIntegerToLongIntegerRegister(ism, REGISTER_X);
  longIntegerFree(ism);
}



/********************************************//**
 * \brief Gets the word size
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGetWordSize(uint16_t unusedParamButMandatory) {
  longInteger_t wordSize;

  liftStack();

  longIntegerInit(wordSize);
  uIntToLongInteger(shortIntegerWordSize, wordSize);
  convertLongIntegerToLongIntegerRegister(wordSize, REGISTER_X);
  longIntegerFree(wordSize);
}



/********************************************//**
 * \brief Sets the word size
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSetWordSize(uint16_t WS) {
  bool_t reduceWordSize;
  if(WS == 0) {
    WS = 64;
  }

  reduceWordSize = (WS < shortIntegerWordSize);

  shortIntegerWordSize = WS;

  if(shortIntegerWordSize == 64) {
    shortIntegerMask    = -1;
  }
  else {
    shortIntegerMask    = ((uint64_t)1 << shortIntegerWordSize) - 1;
  }

  shortIntegerSignBit = (uint64_t)1 << (shortIntegerWordSize - 1);
  //printf("shortIntegerMask  =   %08x-%08x\n", (unsigned int)(shortIntegerMask>>32), (unsigned int)(shortIntegerMask&0xffffffff));
  //printf("shortIntegerSignBit = %08x-%08x\n", (unsigned int)(shortIntegerSignBit>>32), (unsigned int)(shortIntegerSignBit&0xffffffff));

  if(reduceWordSize) {
    // reduce the word size of integers on the stack
    for(calcRegister_t regist=REGISTER_X; regist<=getStackTop(); regist++) {
      if(getRegisterDataType(regist) == dtShortInteger) {
        *(REGISTER_SHORT_INTEGER_DATA(regist)) &= shortIntegerMask;
      }
    }

    // reduce the word size of integers in the L register
    if(getRegisterDataType(REGISTER_L) == dtShortInteger) {
      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_L)) &= shortIntegerMask;
    }
  }
}



/********************************************//**
 * \brief Sets X to the amount of free flash memory
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnFreeFlashMemory(uint16_t unusedParamButMandatory) {
  longInteger_t flashMem;

  liftStack();

  longIntegerInit(flashMem);
  uIntToLongInteger(getFreeFlash(), flashMem);
  convertLongIntegerToLongIntegerRegister(flashMem, REGISTER_X);
  longIntegerFree(flashMem);
}



/********************************************//**
 * \brief Sets X to the battery voltage
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnBatteryVoltage(uint16_t unusedParamButMandatory) {
  real_t value;

  liftStack();

  #ifdef PC_BUILD
    int32ToReal(3100, &value);
  #endif

  #ifdef DMCP_BUILD
    int32ToReal(get_vbat(), &value);
  #endif

  realDivide(&value, const_1000, &value, &ctxtReal39);
  realToReal34(&value, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Retrives the amount of free flash memory
 *
 * \param void
 * \return uint32_t Number of bytes free
 ***********************************************/
uint32_t getFreeFlash(void) {
  return 1234567u;
}



/********************************************//**
 * \brief Sets X to the number of signficant digits
 * rounding after each operation
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGetSignificantDigits(uint16_t unusedParamButMandatory) {
  longInteger_t sigDigits;

  liftStack();

  longIntegerInit(sigDigits);
  uIntToLongInteger(significantDigits, sigDigits);
  convertLongIntegerToLongIntegerRegister(sigDigits, REGISTER_X);
  longIntegerFree(sigDigits);
}



/********************************************//**
 * \brief Sets the rounding mode
 *
 * \param[in] RM uint16_t Rounding mode
 * \return void
 ***********************************************/
void fnRoundingMode(uint16_t RM) {
  roundingMode = RM;

  if(RM == 0) ctxtReal34.round = DEC_ROUND_HALF_EVEN;
  else if(RM == 1) ctxtReal34.round = DEC_ROUND_HALF_UP;
  else if(RM == 2) ctxtReal34.round = DEC_ROUND_HALF_DOWN;
  else if(RM == 3) ctxtReal34.round = DEC_ROUND_UP;
  else if(RM == 4) ctxtReal34.round = DEC_ROUND_DOWN;
  else if(RM == 5) ctxtReal34.round = DEC_ROUND_CEILING;
  else if(RM == 6) ctxtReal34.round = DEC_ROUND_FLOOR;
  else {
    sprintf(errorMessage, "In function fnRoundingMode: %d is an unexpected value for RM! Must be from 0 to 6", RM);
    displayBugScreen(errorMessage);
  }
}



/********************************************//**
 * \brief Sets the angular mode
 *
 * \param[in] am uint16_t Angular mode
 * \return void
 ***********************************************/
void fnAngularMode(uint16_t am) {
  currentAngularMode = am;
}



void fnFractionType(uint16_t unusedParamButMandatory) {
  if(getSystemFlag(FLAG_FRACT)) {
    flipSystemFlag(FLAG_PROPFR);
  }
  else {
    setSystemFlag(FLAG_FRACT);
  }
}



void setConfirmationMode(void (*func)(uint16_t)) {
  previousCalcMode = calcMode;
  cursorEnabled = false;
  calcMode = CM_CONFIRMATION;
  clearSystemFlag(FLAG_ALPHA);
  confirmedFunction = func;
  temporaryInformation = TI_ARE_YOU_SURE;
}



void fnRange(uint16_t unusedParamButMandatory) {
  longInteger_t longInt;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_X, longInt);
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    convertReal34ToLongInteger(REGISTER_REAL34_DATA(REGISTER_X), longInt, DEC_ROUND_DOWN);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot use %s for setting RANGE", getRegisterDataTypeName(REGISTER_X, true, true));
      moreInfoOnError("In function fnRange:", errorMessage, NULL, NULL);
    #endif
    return;
  }

  longIntegerSetPositiveSign(longInt);

  if(longIntegerCompareInt(longInt, 6145) > 0) {
    exponentLimit = 6145;
  }
  else if(longIntegerCompareInt(longInt, 99) < 0) {
    exponentLimit = 99;
  }
  else {
    exponentLimit = (int16_t)(longInt->_mp_d[0]); // OK for 32 and 64 bit limbs
  }

  longIntegerFree(longInt);
}



void fnGetRange(uint16_t unusedParamButMandatory) {
  longInteger_t range;

  liftStack();

  longIntegerInit(range);
  uIntToLongInteger(exponentLimit, range);
  convertLongIntegerToLongIntegerRegister(range, REGISTER_X);
  longIntegerFree(range);
}



void fnClAll(uint16_t confirmation) {
  if(confirmation == NOT_CONFIRMED) {
    setConfirmationMode(fnClAll);
  }
  else {
    calcRegister_t regist;

    fnClPAll(CONFIRMED);  // Clears all the programs
    fnClSigma(CONFIRMED); // Clears and releases the memory of all statistical sums
    if(savedStatisticalSumsPointer != NULL) {
      freeWp43s(savedStatisticalSumsPointer, NUMBER_OF_STATISTICAL_SUMS * TO_BYTES(REAL_SIZE));
    }

    // Clear local registers
    allocateLocalRegisters(0);

    // Clear registers including stack, I, J, K and L
    for(regist=0; regist<FIRST_LOCAL_REGISTER; regist++) {
      clearRegister(regist);
    }

    // Clear saved stack registers
    for(regist=FIRST_SAVED_STACK_REGISTER; regist<=TEMP_REGISTER; regist++) {
      clearRegister(regist);
    }
    thereIsSomethingToUndo = false;

    // TODO: clear (or delete) named variables

    // Clear global flags
    for(int32_t sixteenFlags=0; sixteenFlags<7; sixteenFlags++) { // 7 times uint16_t = 112 flags
      globalFlags[sixteenFlags] = 0;
    }

    temporaryInformation = TI_NO_INFO;
  }
}



void addTestPrograms(void) {
  uint32_t numberOfBytesForTheTestPrograms = 2048 * 4; // Multiple of 4

  resizeProgramMemory(TO_BLOCKS(numberOfBytesForTheTestPrograms));
  firstDisplayedStep        = beginOfProgramMemory;
  currentStep               = beginOfProgramMemory;
  currentStepNumber         = 0;
  firstDisplayedStepNumber  = 0;

  { // Prime number checker
    // 0
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'P';
    *(currentStep++) = 'r';
    *(currentStep++) = 'i';
    *(currentStep++) = 'm';
    *(currentStep++) = 'e';

    *(currentStep++) = ITM_MAGNITUDE;

    *(currentStep++) = ITM_IP;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_SQUAREROOTX;

    *(currentStep++) = ITM_IP;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 1;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    // 10
    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    // 20
    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    // 30
    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6'; // THE string

    // 40
    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    // 50
    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '8'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4'; // THE string

    // 60
    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '8'; // THE string

    // 70
    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4'; // THE string

    // 80
    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4'; // THE string

    // 90
    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '6'; // THE string

    // 100
    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '4'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 2; // String length
    *(currentStep++) = '1'; // THE string
    *(currentStep++) = '0'; // THE string

    // 110
    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 2; // String length
    *(currentStep++) = '1'; // THE string
    *(currentStep++) = '0'; // THE string

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 1;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = REGISTER_Y;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 0;

    // 120
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_STOP >> 8) | 0x80;
    *(currentStep++) =  ITM_STOP       & 0xff;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 9;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 1;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 1;

    *(currentStep++) = ITM_MOD;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = VALUE_0;

    *(currentStep++) = ITM_RTN;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 0;

    // 130
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 1;

    // 131
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime : %u bytes\n", (uint32_t)(currentStep - beginOfProgramMemory));
    #endif
  }

  { // Len1
    // 132
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 : %u bytes\n", (uint32_t)(currentStep - beginOfProgramMemory));
    #endif
  }

  { // Len2
    // 133
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'e';
    *(currentStep++) = 'n';
    *(currentStep++) = '2';

    // 134
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 : %u bytes\n", (uint32_t)(currentStep - beginOfProgramMemory));
    #endif
  }

  { // Len3
    // 135
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'e';
    *(currentStep++) = 'n';
    *(currentStep++) = '3';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 2;

    // 137
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 : %u bytes\n", (uint32_t)(currentStep - beginOfProgramMemory));
    #endif
  }

  { // Len4
    // 138
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'e';
    *(currentStep++) = 'n';
    *(currentStep++) = '4';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 2;

    // 140
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 3;

    // 131
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 + Len4 : %u bytes\n", (uint32_t)(currentStep - beginOfProgramMemory));
    #endif
  }

  { // Len5
    // 142
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'e';
    *(currentStep++) = 'n';
    *(currentStep++) = '5';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 3;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 4;

    // 146
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 + Len4 + Len5 : %u bytes\n", (uint32_t)(currentStep - beginOfProgramMemory));
    #endif
  }

  { // Len6
    // 147
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'e';
    *(currentStep++) = 'n';
    *(currentStep++) = '6';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 3;

    // 150
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 4;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 5;

    // 152
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 + Len4 + Len5 + Len6 : %u bytes\n", (uint32_t)(currentStep - beginOfProgramMemory));
    #endif
  }

  { // Len7
    // 153
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'e';
    *(currentStep++) = 'n';
    *(currentStep++) = '7';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 3;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 4;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 5;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 6;

    // 159
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 + Len4 + Len5 + Len6 + Len7 : %u bytes\n", (uint32_t)(currentStep - beginOfProgramMemory));
    #endif
  }

  { // Len8
    // 160
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'L';
    *(currentStep++) = 'e';
    *(currentStep++) = 'n';
    *(currentStep++) = '8';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 3;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 4;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 5;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 6;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 7;

    // 167
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 + Len4 + Len5 + Len6 + Len7 + Len8 : %u bytes\n", (uint32_t)(currentStep - beginOfProgramMemory));
    #endif
  }

  { // Bairstow polynomial root finder
    // 168
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 7; // String length
    *(currentStep++) = 'B';
    *(currentStep++) = 'a';
    *(currentStep++) = 'i';
    *(currentStep++) = 'r';
    *(currentStep++) = 's';
    *(currentStep++) = 't';
    *(currentStep++) = 'o';

    *(currentStep++) = (ITM_ALL >> 8) | 0x80;
    *(currentStep++) =  ITM_ALL       & 0xff;
    *(currentStep++) = 0;

    // 170
    *(currentStep++) = (ITM_CLREGS >> 8) | 0x80;
    *(currentStep++) =  ITM_CLREGS       & 0xff;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_LABEL_VARIABLE;
    *(currentStep++) = 19; // String length
    *(currentStep++) = 'P'; // THE string
    *(currentStep++) = 'o';
    *(currentStep++) = 'l';
    *(currentStep++) = 'y';
    *(currentStep++) = 'n';
    *(currentStep++) = 'o';
    *(currentStep++) = 'm';
    *(currentStep++) = 'i';
    *(currentStep++) = 'a';
    *(currentStep++) = 'l';
    *(currentStep++) = ' ';
    *(currentStep++) = 'd';
    *(currentStep++) = 'e';
    *(currentStep++) = 'g';
    *(currentStep++) = 'r';
    *(currentStep++) = 'e';
    *(currentStep++) = 'e';
    *(currentStep++) = ' ';
    *(currentStep++) = '?';

    *(currentStep++) = (ITM_STOP >> 8) | 0x80;
    *(currentStep++) =  ITM_STOP       & 0xff;

    *(currentStep++) = ITM_MAGNITUDE;

    *(currentStep++) = ITM_IP;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 90;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 91;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 91;

    // 180
    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 3; // String length
    *(currentStep++) = '1'; // THE string
    *(currentStep++) = 'e';
    *(currentStep++) = '3';

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'A' - 'A'; // A

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_LABEL_VARIABLE;
    *(currentStep++) = 2; // String length
    *(currentStep++) = 'x'; // THE string
    *(currentStep++) = '^';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_ADD;

    *(currentStep++) = (ITM_STOP >> 8) | 0x80;
    *(currentStep++) =  ITM_STOP       & 0xff;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 93;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    // 190
    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'A' - 'A'; // A

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = REGISTER_Y;

    // 200
    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 100 + 'B' - 'A'; // B

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_LABEL_VARIABLE;
    *(currentStep++) = 10; // String length
    *(currentStep++) = 'T'; // THE string
    *(currentStep++) = 'o';
    *(currentStep++) = 'l';
    *(currentStep++) = 'e';
    *(currentStep++) = 'r';
    *(currentStep++) = 'a';
    *(currentStep++) = 'n';
    *(currentStep++) = 'c';
    *(currentStep++) = 'e';
    *(currentStep++) = '?';

    *(currentStep++) = (ITM_STOP >> 8) | 0x80;
    *(currentStep++) =  ITM_STOP       & 0xff;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 10;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '3'; // THE string

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 90;

    // 210
    *(currentStep++) = ITM_XLT;
    *(currentStep++) = REGISTER_Y;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '0'; // THE string

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 95;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 96;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 100 + 'D' - 'A'; // D

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 100 + 'J' - 'A'; // J

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 11;

    *(currentStep++) = (ITM_STOP >> 8) | 0x80;
    *(currentStep++) =  ITM_STOP       & 0xff;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 10;

    // 220
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 90;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = REGISTER_Y;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 1;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 1;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 90;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 1;

    // 230
    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 13;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 90;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_LABEL_VARIABLE;
    *(currentStep++) = 9; // String length
    *(currentStep++) = 'L'; // THE string
    *(currentStep++) = 'a';
    *(currentStep++) = 's';
    *(currentStep++) = 't';
    *(currentStep++) = ' ';
    *(currentStep++) = 'r';
    *(currentStep++) = 'o';
    *(currentStep++) = 'o';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_RTN;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 100 + 'B' - 'A'; // B

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 90;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 3; // String length
    *(currentStep++) = '1'; // THE string
    *(currentStep++) = 'e';
    *(currentStep++) = '3';

    *(currentStep++) = ITM_DIV;

    // 240
    *(currentStep++) = ITM_STO;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 100 + 'C' - 'A'; // C

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'C' - 'A'; // C

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    // 250
    *(currentStep++) = ITM_STO;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RTN;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'D' - 'A'; // D

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 97;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '0'; // THE string

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 98;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'I' - 'A'; // I

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 91;

    // 260
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 90;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 3; // String length
    *(currentStep++) = '1'; // THE string
    *(currentStep++) = 'e';
    *(currentStep++) = '3';

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_ADD;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 89;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 2; // String length
    *(currentStep++) = '3'; // THE string
    *(currentStep++) = '0'; // THE string

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 94;

    // 270
    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '0'; // THE string

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'E' - 'A'; // E

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 91;

    // 280
    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 95;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_CHS;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 96;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_CHS;

    // 290
    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 89;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = 89;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'E' - 'A'; // E

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 92;

    // 300
    *(currentStep++) = ITM_STO;
    *(currentStep++) = 84;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 85;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 2; // String length
    *(currentStep++) = '3'; // THE string
    *(currentStep++) = '0'; // THE string

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 90;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 3; // String length
    *(currentStep++) = '1'; // THE string
    *(currentStep++) = 'e';
    *(currentStep++) = '3';

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_ADD;

    // 310
    *(currentStep++) = ITM_STO;
    *(currentStep++) = 89;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 2; // String length
    *(currentStep++) = '6'; // THE string
    *(currentStep++) = '0'; // THE string

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '0'; // THE string

    // 320
    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '0'; // THE string

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'F' - 'A'; // F

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 95;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_CHS;

    // 330
    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 96;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_CHS;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 93;

    // 340
    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 89;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = 89;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'F' - 'A'; // F

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 93;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 95;

    // 350
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 96;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_ADD;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 93;

    // 360
    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_ADD;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 88;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 84;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 85;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_SUB;

    // 370
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 88;

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 87;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 95;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 96;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_ADD;

    // 380
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 84;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 85;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_ADD;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 88;

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 86;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 87;

    // 390
    *(currentStep++) = ITM_MAGNITUDE;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 86;

    *(currentStep++) = ITM_MAGNITUDE;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = REGISTER_Y;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = REGISTER_Y;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'G' - 'A'; // G

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 87;

    *(currentStep++) = ITM_MAGNITUDE;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 97;

    // 400
    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'H' - 'A'; // H

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'G' - 'A'; // G

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 86;

    *(currentStep++) = ITM_MAGNITUDE;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 97;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'H' - 'A'; // H

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 87;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 95;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 86;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 96;

    // 410
    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 98;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 3; // String length
    *(currentStep++) = '1'; // THE string
    *(currentStep++) = 'e';
    *(currentStep++) = '4';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 98;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = REGISTER_Y;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 4;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 97;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = REGISTER_Y;

    // 420
    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 100 + 'I' - 'A';

    *(currentStep++) = ITM_RTN;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 100 + 'J' - 'A'; // J

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '0'; // THE string

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 2; // String length
    *(currentStep++) = '3'; // THE string
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 90;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_SUB;

    // 430
    *(currentStep++) = ITM_STO;
    *(currentStep++) = 93;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 3; // String length
    *(currentStep++) = '1'; // THE string
    *(currentStep++) = 'e';
    *(currentStep++) = '3';

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 88;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 92;

    // 440
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = 94;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 88;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 90;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    *(currentStep++) = ITM_SUB;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 90;

    *(currentStep++) = ITM_RTN;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 11;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    // 450
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 95;

    *(currentStep++) = ITM_CHS;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_SQUARE;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 96;

    *(currentStep++) = ITM_SUB;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 92;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    // 460
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_SQUAREROOTX;

    *(currentStep++) = ITM_ADD;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_SQUAREROOTX;

    *(currentStep++) = ITM_SUB;

    *(currentStep++) = ITM_RTN;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 12;

    // 470
    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_LABEL_VARIABLE;
    *(currentStep++) = 12; // String length
    *(currentStep++) = '.'; // THE string
    *(currentStep++) = '.';
    *(currentStep++) = '.';
    *(currentStep++) = ' ';
    *(currentStep++) = 'c';
    *(currentStep++) = 'o';
    *(currentStep++) = 'n';
    *(currentStep++) = 't';
    *(currentStep++) = 'i';
    *(currentStep++) = 'n';
    *(currentStep++) = 'u';
    *(currentStep++) = 'e';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 1;

    *(currentStep++) = ITM_CHS;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '2'; // THE string

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_SQUARE;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 2;

    *(currentStep++) = ITM_SUB;

    // 480
    *(currentStep++) = ITM_STO;
    *(currentStep++) = 92;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_SQUAREROOTX;

    *(currentStep++) = ITM_ADD;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 92;

    *(currentStep++) = ITM_SQUAREROOTX;

    *(currentStep++) = ITM_SUB;

    // 490
    *(currentStep++) = ITM_RTN;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 13;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_LABEL_VARIABLE;
    *(currentStep++) = 12; // String length
    *(currentStep++) = '.'; // THE string
    *(currentStep++) = '.';
    *(currentStep++) = '.';
    *(currentStep++) = ' ';
    *(currentStep++) = 'c';
    *(currentStep++) = 'o';
    *(currentStep++) = 'n';
    *(currentStep++) = 't';
    *(currentStep++) = 'i';
    *(currentStep++) = 'n';
    *(currentStep++) = 'u';
    *(currentStep++) = 'e';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 1;

    *(currentStep++) = ITM_CHS;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 91;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 91;

    *(currentStep++) = ITM_RTN;

    // 500
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 4;

    *(currentStep++) = (ITM_CLSTK >> 8) | 0x80;
    *(currentStep++) =  ITM_CLSTK       & 0xff;

    *(currentStep++) = ITM_LITT;
    *(currentStep++)= STRING_LABEL_VARIABLE;
    *(currentStep++) = 11; // String length
    *(currentStep++) = 'E'; // THE string
    *(currentStep++) = 'r';
    *(currentStep++) = 'r';
    *(currentStep++) = 'o';
    *(currentStep++) = 'r';
    *(currentStep++) = ' ';
    *(currentStep++) = 'm';
    *(currentStep++) = '>';
    *(currentStep++) = '8';
    *(currentStep++) = '0';
    *(currentStep++) = '!';

    *(currentStep++) = ITM_RTN;

    // 504
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 + Len4 + Len5 + Len6 + Len7 + Len8 + Bairstow : %u bytes\n", (uint32_t)(currentStep - beginOfProgramMemory));
    #endif
  }

  { // Speed test. See: https://forum.swissmicros.com/viewtopic.php?p=17308
    // 505
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5; // String length
    *(currentStep++) = 'S';
    *(currentStep++) = 'p';
    *(currentStep++) = 'e';
    *(currentStep++) = 'e';
    *(currentStep++) = 'd';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 10;

    *(currentStep++) = (ITM_TICKS >> 8) | 0x80;
    *(currentStep++) =  ITM_TICKS       & 0xff;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '0'; // THE string

    // 510
    *(currentStep++) = ITM_STO;
    *(currentStep++) = 11;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 12;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 1;

    *(currentStep++) = ITM_arctan;

    *(currentStep++) = ITM_sin;

    *(currentStep++) = ITM_EXP;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '3'; // THE string

    *(currentStep++) = ITM_1ONX;

    *(currentStep++) = ITM_YX;

    // 520
    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 11;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 12;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_ADD;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 12;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = 10;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 1;

    *(currentStep++) = (ITM_TICKS >> 8) | 0x80;
    *(currentStep++) =  ITM_TICKS       & 0xff;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_SUB;

    // 530
    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 2; // String length
    *(currentStep++) = '1'; // THE string
    *(currentStep++) = '0';

    *(currentStep++) = ITM_DIV;

    // 532
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 + Len4 + Len5 + Len6 + Len7 + Len8 + Bairstow + Speed : %u bytes\n", (uint32_t)(currentStep - beginOfProgramMemory));
    #endif
  }

  { // Factorial: the recursive way
    // 533
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'F';
    *(currentStep++) = 'a';
    *(currentStep++) = 'c';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_IP;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = VALUE_1;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_REAL34;
    *(currentStep++) = 1; // String length
    *(currentStep++) = '1'; // THE string

    *(currentStep++) = ITM_RTN;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 0;

    // 540
    *(currentStep++) = (ITM_LocR >> 8) | 0x80;
    *(currentStep++) =  ITM_LocR       & 0xff;
    *(currentStep++) = 1;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 112; // .00

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 4; // String length
    *(currentStep++) = 'F';
    *(currentStep++) = 'a';
    *(currentStep++) = 'c';
    *(currentStep++) = 't';

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = 112; // .00

    *(currentStep++) = ITM_RTN;

    // 546
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 + Len4 + Len5 + Len6 + Len7 + Len8 + Bairstow + Speed + Fact : %u bytes\n", (uint32_t)(currentStep - beginOfProgramMemory));
    #endif
  }

  { // OM page 203
    // 547
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 6; // String length
    *(currentStep++) = 'O';
    *(currentStep++) = 'M';
    *(currentStep++) = 'p';
    *(currentStep++) = '2';
    *(currentStep++) = '0';
    *(currentStep++) = '3';

    *(currentStep++) = ITM_SQUARE;

    *(currentStep++) = ITM_pi;

    // 550
    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_RTN;
    // 552
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 + Len4 + Len5 + Len6 + Len7 + Len8 + Bairstow + Speed + Fact + OMp203 : %u bytes\n", (uint32_t)(currentStep - beginOfProgramMemory));
    #endif
  }

  { // All OP's
    // 553
    *(currentStep++) = ITM_LBL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 7; // String length
    *(currentStep++) = 'A';
    *(currentStep++) = 'l';
    *(currentStep++) = 'l';
    *(currentStep++) = 'O';
    *(currentStep++) = 'p';
    *(currentStep++) = '\'';
    *(currentStep++) = 's';

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 'A' - 'A' + 100; // A

    *(currentStep++) = ITM_LBL;
    *(currentStep++) = 'J' - 'A' + 100; // J

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 99;

    // 560
    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 'A' - 'A' + 100; // A

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = 'J' - 'A' + 100; // J

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5;
    *(currentStep++) = 'L';
    *(currentStep++) = 'a';
    *(currentStep++) = 'b';
    *(currentStep++) = 'e';
    *(currentStep++) = 'l';

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_GTO;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    // 570
    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 'A' - 'A' + 100; // A

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = 'J' - 'A' + 100; // J

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 5;
    *(currentStep++) = 'L';
    *(currentStep++) = 'a';
    *(currentStep++) = 'b';
    *(currentStep++) = 'e';
    *(currentStep++) = 'l';

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    // 580
    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XEQ;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RTN;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    // 590
    *(currentStep++) = ITM_ISE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    // 590
    *(currentStep++) = ITM_ISE;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    // 600
    *(currentStep++) = ITM_ISG;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISG;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    // 610
    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_ISZ;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = 0;

    // 620
    *(currentStep++) = ITM_DSE;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    // 630
    *(currentStep++) = ITM_DSE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSE;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    // 640
    *(currentStep++) = ITM_DSL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSL;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = REGISTER_X;

    // 650
    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    // 660
    *(currentStep++) = ITM_DSZ;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    // 670
    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XEQU;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    // 680
    *(currentStep++) = ITM_XNE;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XNE;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XEQUP0;

    // 690
    *(currentStep++) = ITM_XEQUM0;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    // 700
    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XAEQU;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    // 710
    *(currentStep++) = ITM_XLT;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XLT;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = 0;

    // 720
    *(currentStep++) = ITM_XLE;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    // 730
    *(currentStep++) = ITM_XLE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XLE;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    // 740
    *(currentStep++) = ITM_XGE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XGE;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = REGISTER_X;

    // 750
    *(currentStep++) = ITM_XGT;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_XGT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    // 760
    *(currentStep++) = ITM_XGT;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_FC;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    // 770
    *(currentStep++) = ITM_FC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = ITM_FC;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_FS;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = LAST_LOCAL_FLAG;

    // 780
    *(currentStep++) = ITM_FS;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = ITM_FS;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_EVEN;

    *(currentStep++) = ITM_ODD;

    *(currentStep++) = ITM_FPQ;

    // 790
    *(currentStep++) = ITM_INT;

    *(currentStep++) = ITM_CPX;

    *(currentStep++) = ITM_MATR;

    *(currentStep++) = ITM_NAN;

    *(currentStep++) = ITM_REAL;

    *(currentStep++) = ITM_SPEC;

    *(currentStep++) = ITM_STRI;

    *(currentStep++) = ITM_PMINFINITY;

    *(currentStep++) = ITM_PRIME;

    *(currentStep++) = ITM_TOP;

    // 800
    *(currentStep++) = ITM_ENTER;

    *(currentStep++) = ITM_XexY;

    *(currentStep++) = ITM_DROP;

    *(currentStep++) = ITM_DROPY;

    *(currentStep++) = ITM_Rup;

    *(currentStep++) = ITM_Rdown;

    *(currentStep++) = ITM_CLX;

    *(currentStep++) = ITM_FILL;

    *(currentStep++) = ITM_INPUT;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = 0;

    // 810
    *(currentStep++) = ITM_STO;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    // 820
    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STO;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    // 830
    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOADD;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = REGISTER_X;

    // 840
    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    // 850
    *(currentStep++) = ITM_STOSUB;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    // 860
    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOMULT;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    // 870
    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STODIV;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STOMAX;
    *(currentStep++) = 0;

    // 880
    *(currentStep++) = ITM_STOMAX;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STOMAX;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STOMAX;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STOMAX;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOMAX;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOMAX;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STOMAX;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STOMAX;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STOMAX;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STOMAX;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    // 890
    *(currentStep++) = ITM_STOMAX;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOMAX;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOMAX;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_STOMIN;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STOMIN;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STOMIN;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STOMIN;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STOMIN;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOMIN;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOMIN;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    // 900
    *(currentStep++) = ITM_STOMIN;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_STOMIN;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_STOMIN;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_STOMIN;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_STOMIN;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOMIN;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_STOMIN;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = REGISTER_X;

    // 910
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    // 920
    *(currentStep++) = ITM_RCL;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    // 930
    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLADD;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    // 940
    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLSUB;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = 0;

    // 950
    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    // 960
    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLMULT;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    // 970
    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLDIV;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCLMAX;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCLMAX;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCLMAX;
    *(currentStep++) = REGISTER_X;

    // 980
    *(currentStep++) = ITM_RCLMAX;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCLMAX;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLMAX;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLMAX;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCLMAX;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCLMAX;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCLMAX;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCLMAX;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCLMAX;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLMAX;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    // 990
    *(currentStep++) = ITM_RCLMAX;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCLMIN;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCLMIN;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_RCLMIN;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCLMIN;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCLMIN;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLMIN;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLMIN;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_RCLMIN;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_RCLMIN;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    // 1000
    *(currentStep++) = ITM_RCLMIN;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_RCLMIN;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_RCLMIN;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLMIN;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_RCLMIN;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_SQUARE;

    *(currentStep++) = ITM_CUBE;

    *(currentStep++) = ITM_YX;

    *(currentStep++) = ITM_SQUAREROOTX;

    *(currentStep++) = ITM_CUBEROOT;

    // 1010
    *(currentStep++) = ITM_XTHROOT;

    *(currentStep++) = ITM_2X;

    *(currentStep++) = ITM_EXP;

    *(currentStep++) = ITM_EX1;

    *(currentStep++) = ITM_10x;

    *(currentStep++) = ITM_LOG2;

    *(currentStep++) = ITM_LN;

    *(currentStep++) = ITM_LN1X;

    *(currentStep++) = ITM_LOG10;

    *(currentStep++) = ITM_LOGXY;

    // 1020
    *(currentStep++) = ITM_1ONX;

    *(currentStep++) = ITM_cos;

    *(currentStep++) = ITM_cosh;

    *(currentStep++) = ITM_sin;

    *(currentStep++) = ITM_sinc;

    *(currentStep++) = ITM_sinh;

    *(currentStep++) = ITM_tan;

    *(currentStep++) = ITM_tanh;

    *(currentStep++) = ITM_arccos;

    *(currentStep++) = ITM_arcosh;

    // 1030
    *(currentStep++) = ITM_arcsin;

    *(currentStep++) = ITM_arsinh;

    *(currentStep++) = ITM_arctan;

    *(currentStep++) = ITM_artanh;

    *(currentStep++) = ITM_CEIL;

    *(currentStep++) = ITM_FLOOR;

    *(currentStep++) = ITM_GCD;

    *(currentStep++) = ITM_LCM;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = 99;

    // 1040
    *(currentStep++) = ITM_DEC;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    // 1050
    *(currentStep++) = ITM_DEC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_DEC;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_INC;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = STRING_LABEL_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_INC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    // 1060
    *(currentStep++) = ITM_INC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = ITM_INC;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_IP;

    *(currentStep++) = ITM_FP;

    *(currentStep++) = ITM_ADD;

    *(currentStep++) = ITM_SUB;

    // 1070
    *(currentStep++) = ITM_CHS;

    *(currentStep++) = ITM_MULT;

    *(currentStep++) = ITM_DIV;

    *(currentStep++) = ITM_IDIV;

    *(currentStep++) = ITM_IDIVR;

    *(currentStep++) = ITM_MOD;

    *(currentStep++) = ITM_MAX;

    *(currentStep++) = ITM_MIN;

    *(currentStep++) = ITM_MAGNITUDE;

    *(currentStep++) = ITM_NEIGHB;

    // 1080
    *(currentStep++) = ITM_NEXTP;

    *(currentStep++) = ITM_XFACT;

    *(currentStep++) = ITM_pi;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    // 1090
    *(currentStep++) = ITM_CF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = ITM_CF;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_SF;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = REGISTER_K;

    // 1100
    *(currentStep++) = ITM_SF;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = ITM_SF;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_FF;
    *(currentStep++) = 0;

    // 1110
    *(currentStep++) = ITM_FF;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_FLAG;

    // 1120
    *(currentStep++) = ITM_FF;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_FLAG;

    *(currentStep++) = ITM_FF;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    *(currentStep++) = ITM_sincpi;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = BINARY_SHORT_INTEGER;
    *(currentStep++) = 16; // Base
    *(currentStep++) = 1;
    *(currentStep++) = 2;
    *(currentStep++) = 3;
    *(currentStep++) = 4;
    *(currentStep++) = 5;
    *(currentStep++) = 6;
    *(currentStep++) = 7;
    *(currentStep++) = 8;

    *(currentStep++) = (ITM_NOP >> 8) | 0x80; // TODO: implement ITM_LITT BINARY_LONG_INTEGER
    *(currentStep++) =  ITM_NOP       & 0xff;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = BINARY_REAL34;
    *(currentStep++) =  1;
    *(currentStep++) =  2;
    *(currentStep++) =  3;
    *(currentStep++) =  4;
    *(currentStep++) =  5;
    *(currentStep++) =  6;
    *(currentStep++) =  7;
    *(currentStep++) =  8;
    *(currentStep++) =  9;
    *(currentStep++) = 10;
    *(currentStep++) = 11;
    *(currentStep++) = 12;
    *(currentStep++) = 13;
    *(currentStep++) = 14;
    *(currentStep++) = 15;
    *(currentStep++) = 16;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = BINARY_ANGLE34;
    *(currentStep++) = AM_MULTPI;
    *(currentStep++) = 16;
    *(currentStep++) = 15;
    *(currentStep++) = 14;
    *(currentStep++) = 13;
    *(currentStep++) = 12;
    *(currentStep++) = 11;
    *(currentStep++) = 10;
    *(currentStep++) =  9;
    *(currentStep++) =  8;
    *(currentStep++) =  7;
    *(currentStep++) =  6;
    *(currentStep++) =  5;
    *(currentStep++) =  4;
    *(currentStep++) =  3;
    *(currentStep++) =  2;
    *(currentStep++) =  1;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = BINARY_COMPLEX34;
    *(currentStep++) =  1;
    *(currentStep++) =  2;
    *(currentStep++) =  3;
    *(currentStep++) =  4;
    *(currentStep++) =  5;
    *(currentStep++) =  6;
    *(currentStep++) =  7;
    *(currentStep++) =  8;
    *(currentStep++) =  9;
    *(currentStep++) = 10;
    *(currentStep++) = 11;
    *(currentStep++) = 12;
    *(currentStep++) = 13;
    *(currentStep++) = 14;
    *(currentStep++) = 15;
    *(currentStep++) = 16;
    *(currentStep++) = 17;
    *(currentStep++) = 18;
    *(currentStep++) = 19;
    *(currentStep++) = 20;
    *(currentStep++) = 21;
    *(currentStep++) = 22;
    *(currentStep++) = 23;
    *(currentStep++) = 24;
    *(currentStep++) = 25;
    *(currentStep++) = 26;
    *(currentStep++) = 27;
    *(currentStep++) = 28;
    *(currentStep++) = 29;
    *(currentStep++) = 30;
    *(currentStep++) = 31;
    *(currentStep++) = 32;

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_SHORT_INTEGER;
    *(currentStep++) =  16; // Base
    *(currentStep++) =  7;  // String length
    *(currentStep++) =  '-';
    *(currentStep++) =  'A';
    *(currentStep++) =  'B';
    *(currentStep++) =  'C';
    *(currentStep++) =  'D';
    *(currentStep++) =  'E';
    *(currentStep++) =  'F';

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_LONG_INTEGER;
    *(currentStep++) =  7;  // String length
    *(currentStep++) =  '-';
    *(currentStep++) =  '1';
    *(currentStep++) =  '2';
    *(currentStep++) =  '3';
    *(currentStep++) =  '4';
    *(currentStep++) =  '5';
    *(currentStep++) =  '6';

    // 1130
    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_ANGLE34;
    *(currentStep++) = AM_DEGREE;
    *(currentStep++) = 2; // String length
    *(currentStep++) = '4';
    *(currentStep++) = '5';

    *(currentStep++) = ITM_LITT;
    *(currentStep++) = STRING_COMPLEX34;
    *(currentStep++) = 4; // String length
    *(currentStep++) = '1';
    *(currentStep++) = '-';
    *(currentStep++) = 'i';
    *(currentStep++) = '2';

    *(currentStep++) = (CST_01 >> 8) | 0x80;
    *(currentStep++) =  CST_01       & 0xff;

    *(currentStep++) = (CST_02 >> 8) | 0x80;
    *(currentStep++) =  CST_02       & 0xff;

    *(currentStep++) = (CST_03 >> 8) | 0x80;
    *(currentStep++) =  CST_03       & 0xff;

    *(currentStep++) = (CST_04 >> 8) | 0x80;
    *(currentStep++) =  CST_04       & 0xff;

    *(currentStep++) = (CST_05 >> 8) | 0x80;
    *(currentStep++) =  CST_05       & 0xff;

    *(currentStep++) = (CST_06 >> 8) | 0x80;
    *(currentStep++) =  CST_06       & 0xff;

    *(currentStep++) = (CST_07 >> 8) | 0x80;
    *(currentStep++) =  CST_07       & 0xff;

    *(currentStep++) = (CST_08 >> 8) | 0x80;
    *(currentStep++) =  CST_08       & 0xff;

    // 1140
    *(currentStep++) = (CST_09 >> 8) | 0x80;
    *(currentStep++) =  CST_09       & 0xff;

    *(currentStep++) = (CST_10 >> 8) | 0x80;
    *(currentStep++) =  CST_10       & 0xff;

    *(currentStep++) = (CST_11 >> 8) | 0x80;
    *(currentStep++) =  CST_11       & 0xff;

    *(currentStep++) = (CST_12 >> 8) | 0x80;
    *(currentStep++) =  CST_12       & 0xff;

    *(currentStep++) = (CST_13 >> 8) | 0x80;
    *(currentStep++) =  CST_13       & 0xff;

    *(currentStep++) = (CST_14 >> 8) | 0x80;
    *(currentStep++) =  CST_14       & 0xff;

    *(currentStep++) = (CST_15 >> 8) | 0x80;
    *(currentStep++) =  CST_15       & 0xff;

    *(currentStep++) = (CST_16 >> 8) | 0x80;
    *(currentStep++) =  CST_16       & 0xff;

    *(currentStep++) = (CST_17 >> 8) | 0x80;
    *(currentStep++) =  CST_17       & 0xff;

    *(currentStep++) = (CST_18 >> 8) | 0x80;
    *(currentStep++) =  CST_18       & 0xff;

    // 1150
    *(currentStep++) = (CST_19 >> 8) | 0x80;
    *(currentStep++) =  CST_19       & 0xff;

    *(currentStep++) = (CST_20 >> 8) | 0x80;
    *(currentStep++) =  CST_20       & 0xff;

    *(currentStep++) = (CST_21 >> 8) | 0x80;
    *(currentStep++) =  CST_21       & 0xff;

    *(currentStep++) = (CST_22 >> 8) | 0x80;
    *(currentStep++) =  CST_22       & 0xff;

    *(currentStep++) = (CST_23 >> 8) | 0x80;
    *(currentStep++) =  CST_23       & 0xff;

    *(currentStep++) = (CST_24 >> 8) | 0x80;
    *(currentStep++) =  CST_24       & 0xff;

    *(currentStep++) = (CST_25 >> 8) | 0x80;
    *(currentStep++) =  CST_25       & 0xff;

    *(currentStep++) = (CST_26 >> 8) | 0x80;
    *(currentStep++) =  CST_26       & 0xff;

    *(currentStep++) = (CST_27 >> 8) | 0x80;
    *(currentStep++) =  CST_27       & 0xff;

    *(currentStep++) = (CST_28 >> 8) | 0x80;
    *(currentStep++) =  CST_28       & 0xff;

    // 1160
    *(currentStep++) = (CST_29 >> 8) | 0x80;
    *(currentStep++) =  CST_29       & 0xff;

    *(currentStep++) = (CST_30 >> 8) | 0x80;
    *(currentStep++) =  CST_30       & 0xff;

    *(currentStep++) = (CST_31 >> 8) | 0x80;
    *(currentStep++) =  CST_31       & 0xff;

    *(currentStep++) = (CST_32 >> 8) | 0x80;
    *(currentStep++) =  CST_32       & 0xff;

    *(currentStep++) = (CST_33 >> 8) | 0x80;
    *(currentStep++) =  CST_33       & 0xff;

    *(currentStep++) = (CST_34 >> 8) | 0x80;
    *(currentStep++) =  CST_34       & 0xff;

    *(currentStep++) = (CST_35 >> 8) | 0x80;
    *(currentStep++) =  CST_35       & 0xff;

    *(currentStep++) = (CST_36 >> 8) | 0x80;
    *(currentStep++) =  CST_36       & 0xff;

    *(currentStep++) = (CST_37 >> 8) | 0x80;
    *(currentStep++) =  CST_37       & 0xff;

    *(currentStep++) = (CST_38 >> 8) | 0x80;
    *(currentStep++) =  CST_38       & 0xff;

    // 1170
    *(currentStep++) = (CST_39 >> 8) | 0x80;
    *(currentStep++) =  CST_39       & 0xff;

    *(currentStep++) = (CST_40 >> 8) | 0x80;
    *(currentStep++) =  CST_40       & 0xff;

    *(currentStep++) = (CST_41 >> 8) | 0x80;
    *(currentStep++) =  CST_41       & 0xff;

    *(currentStep++) = (CST_42 >> 8) | 0x80;
    *(currentStep++) =  CST_42       & 0xff;

    *(currentStep++) = (CST_43 >> 8) | 0x80;
    *(currentStep++) =  CST_43       & 0xff;

    *(currentStep++) = (CST_44 >> 8) | 0x80;
    *(currentStep++) =  CST_44       & 0xff;

    *(currentStep++) = (CST_45 >> 8) | 0x80;
    *(currentStep++) =  CST_45       & 0xff;

    *(currentStep++) = (CST_46 >> 8) | 0x80;
    *(currentStep++) =  CST_46       & 0xff;

    *(currentStep++) = (CST_47 >> 8) | 0x80;
    *(currentStep++) =  CST_47       & 0xff;

    *(currentStep++) = (CST_48 >> 8) | 0x80;
    *(currentStep++) =  CST_48       & 0xff;

    // 1180
    *(currentStep++) = (CST_49 >> 8) | 0x80;
    *(currentStep++) =  CST_49       & 0xff;

    *(currentStep++) = (CST_50 >> 8) | 0x80;
    *(currentStep++) =  CST_50       & 0xff;

    *(currentStep++) = (CST_51 >> 8) | 0x80;
    *(currentStep++) =  CST_51       & 0xff;

    *(currentStep++) = (CST_52 >> 8) | 0x80;
    *(currentStep++) =  CST_52       & 0xff;

    *(currentStep++) = (CST_53 >> 8) | 0x80;
    *(currentStep++) =  CST_53       & 0xff;

    *(currentStep++) = (CST_54 >> 8) | 0x80;
    *(currentStep++) =  CST_54       & 0xff;

    *(currentStep++) = (CST_55 >> 8) | 0x80;
    *(currentStep++) =  CST_55       & 0xff;

    *(currentStep++) = (CST_56 >> 8) | 0x80;
    *(currentStep++) =  CST_56       & 0xff;

    *(currentStep++) = (CST_57 >> 8) | 0x80;
    *(currentStep++) =  CST_57       & 0xff;

    *(currentStep++) = (CST_58 >> 8) | 0x80;
    *(currentStep++) =  CST_58       & 0xff;

    // 1190
    *(currentStep++) = (CST_59 >> 8) | 0x80;
    *(currentStep++) =  CST_59       & 0xff;

    *(currentStep++) = (CST_60 >> 8) | 0x80;
    *(currentStep++) =  CST_60       & 0xff;

    *(currentStep++) = (CST_61 >> 8) | 0x80;
    *(currentStep++) =  CST_61       & 0xff;

    *(currentStep++) = (CST_62 >> 8) | 0x80;
    *(currentStep++) =  CST_62       & 0xff;

    *(currentStep++) = (CST_63 >> 8) | 0x80;
    *(currentStep++) =  CST_63       & 0xff;

    *(currentStep++) = (CST_64 >> 8) | 0x80;
    *(currentStep++) =  CST_64       & 0xff;

    *(currentStep++) = (CST_65 >> 8) | 0x80;
    *(currentStep++) =  CST_65       & 0xff;

    *(currentStep++) = (CST_66 >> 8) | 0x80;
    *(currentStep++) =  CST_66       & 0xff;

    *(currentStep++) = (CST_67 >> 8) | 0x80;
    *(currentStep++) =  CST_67       & 0xff;

    *(currentStep++) = (CST_68 >> 8) | 0x80;
    *(currentStep++) =  CST_68       & 0xff;

    // 1200
    *(currentStep++) = (CST_69 >> 8) | 0x80;
    *(currentStep++) =  CST_69       & 0xff;

    *(currentStep++) = (CST_70 >> 8) | 0x80;
    *(currentStep++) =  CST_70       & 0xff;

    *(currentStep++) = (CST_71 >> 8) | 0x80;
    *(currentStep++) =  CST_71       & 0xff;

    *(currentStep++) = (CST_72 >> 8) | 0x80;
    *(currentStep++) =  CST_72       & 0xff;

    *(currentStep++) = (CST_73 >> 8) | 0x80;
    *(currentStep++) =  CST_73       & 0xff;

    *(currentStep++) = (CST_74 >> 8) | 0x80;
    *(currentStep++) =  CST_74       & 0xff;

    *(currentStep++) = (CST_75 >> 8) | 0x80;
    *(currentStep++) =  CST_75       & 0xff;

    *(currentStep++) = (CST_76 >> 8) | 0x80;
    *(currentStep++) =  CST_76       & 0xff;

    *(currentStep++) = (CST_77 >> 8) | 0x80;
    *(currentStep++) =  CST_77       & 0xff;

    *(currentStep++) = (CST_78 >> 8) | 0x80;
    *(currentStep++) =  CST_78       & 0xff;

    // 1210
    *(currentStep++) = (CST_79 >> 8) | 0x80;
    *(currentStep++) =  CST_79       & 0xff;

    *(currentStep++) = (ITM_CNST >> 8) | 0x80;
    *(currentStep++) =  ITM_CNST       & 0xff;
    *(currentStep++) =  1;

    *(currentStep++) = (ITM_CNST >> 8) | 0x80;
    *(currentStep++) =  ITM_CNST       & 0xff;
    *(currentStep++) =  79;

    *(currentStep++) = (ITM_CNST >> 8) | 0x80;
    *(currentStep++) =  ITM_CNST       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 0;

    *(currentStep++) = (ITM_CNST >> 8) | 0x80;
    *(currentStep++) =  ITM_CNST       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = 99;

    *(currentStep++) = (ITM_CNST >> 8) | 0x80;
    *(currentStep++) =  ITM_CNST       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_X;

    *(currentStep++) = (ITM_CNST >> 8) | 0x80;
    *(currentStep++) =  ITM_CNST       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = REGISTER_K;

    *(currentStep++) = (ITM_CNST >> 8) | 0x80;
    *(currentStep++) =  ITM_CNST       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = FIRST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_CNST >> 8) | 0x80;
    *(currentStep++) =  ITM_CNST       & 0xff;
    *(currentStep++) = INDIRECT_REGISTER;
    *(currentStep++) = LAST_LOCAL_REGISTER;

    *(currentStep++) = (ITM_CNST >> 8) | 0x80;
    *(currentStep++) =  ITM_CNST       & 0xff;
    *(currentStep++) = INDIRECT_VARIABLE;
    *(currentStep++) = 3;
    *(currentStep++) = 'V';
    *(currentStep++) = 'a';
    *(currentStep++) = 'r';

    // 1220
    *(currentStep++) = (ITM_CtoF >> 8) | 0x80;
    *(currentStep++) =  ITM_CtoF       & 0xff;

    *(currentStep++) = (ITM_FtoC >> 8) | 0x80;
    *(currentStep++) =  ITM_FtoC       & 0xff;

    *(currentStep++) = (ITM_DBtoPR >> 8) | 0x80;
    *(currentStep++) =  ITM_DBtoPR       & 0xff;

    *(currentStep++) = (ITM_DBtoPRb >> 8) | 0x80;
    *(currentStep++) =  ITM_DBtoPRb       & 0xff;

    *(currentStep++) = (ITM_DBtoPRc >> 8) | 0x80;
    *(currentStep++) =  ITM_DBtoPRc       & 0xff;

    *(currentStep++) = (ITM_DBtoFR >> 8) | 0x80;
    *(currentStep++) =  ITM_DBtoFR       & 0xff;

    *(currentStep++) = (ITM_DBtoFRb >> 8) | 0x80;
    *(currentStep++) =  ITM_DBtoFRb       & 0xff;

    *(currentStep++) = (ITM_DBtoFRc >> 8) | 0x80;
    *(currentStep++) =  ITM_DBtoFRc       & 0xff;

    *(currentStep++) = (ITM_PRtoDB >> 8) | 0x80;
    *(currentStep++) =  ITM_PRtoDB       & 0xff;

    *(currentStep++) = (ITM_PRtoDBb >> 8) | 0x80;
    *(currentStep++) =  ITM_PRtoDBb       & 0xff;

    // 1230
    *(currentStep++) = (ITM_PRtoDBc >> 8) | 0x80;
    *(currentStep++) =  ITM_PRtoDBc       & 0xff;

    *(currentStep++) = (ITM_FRtoDB >> 8) | 0x80;
    *(currentStep++) =  ITM_FRtoDB       & 0xff;

    *(currentStep++) = (ITM_FRtoDBb >> 8) | 0x80;
    *(currentStep++) =  ITM_FRtoDBb       & 0xff;

    *(currentStep++) = (ITM_FRtoDBc >> 8) | 0x80;
    *(currentStep++) =  ITM_FRtoDBc       & 0xff;

    *(currentStep++) = (ITM_ACtoM2 >> 8) | 0x80;
    *(currentStep++) =  ITM_ACtoM2       & 0xff;

    *(currentStep++) = (ITM_ACtoM2b >> 8) | 0x80;
    *(currentStep++) =  ITM_ACtoM2b       & 0xff;

    *(currentStep++) = (ITM_M2toAC >> 8) | 0x80;
    *(currentStep++) =  ITM_M2toAC       & 0xff;

    *(currentStep++) = (ITM_M2toACb >> 8) | 0x80;
    *(currentStep++) =  ITM_M2toACb       & 0xff;

    *(currentStep++) = (ITM_ACUStoM2 >> 8) | 0x80;
    *(currentStep++) =  ITM_ACUStoM2       & 0xff;

    *(currentStep++) = (ITM_ACUStoM2b >> 8) | 0x80;
    *(currentStep++) =  ITM_ACUStoM2b       & 0xff;

    // 1240
    *(currentStep++) = (ITM_M2toACUS >> 8) | 0x80;
    *(currentStep++) =  ITM_M2toACUS       & 0xff;

    *(currentStep++) = (ITM_M2toACUSb >> 8) | 0x80;
    *(currentStep++) =  ITM_M2toACUSb       & 0xff;

    *(currentStep++) = (ITM_PAtoATM >> 8) | 0x80;
    *(currentStep++) =  ITM_PAtoATM       & 0xff;

    *(currentStep++) = (ITM_ATMtoPA >> 8) | 0x80;
    *(currentStep++) =  ITM_ATMtoPA       & 0xff;

    *(currentStep++) = (ITM_AUtoM >> 8) | 0x80;
    *(currentStep++) =  ITM_AUtoM       & 0xff;

    *(currentStep++) = (ITM_MtoAU >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoAU       & 0xff;

    *(currentStep++) = (ITM_BARtoPA >> 8) | 0x80;
    *(currentStep++) =  ITM_BARtoPA       & 0xff;

    *(currentStep++) = (ITM_PAtoBAR >> 8) | 0x80;
    *(currentStep++) =  ITM_PAtoBAR       & 0xff;

    *(currentStep++) = (ITM_BTUtoJ >> 8) | 0x80;
    *(currentStep++) =  ITM_BTUtoJ       & 0xff;

    *(currentStep++) = (ITM_JtoBTU >> 8) | 0x80;
    *(currentStep++) =  ITM_JtoBTU       & 0xff;

    // 1250
    *(currentStep++) = (ITM_CALtoJ >> 8) | 0x80;
    *(currentStep++) =  ITM_CALtoJ       & 0xff;

    *(currentStep++) = (ITM_JtoCAL >> 8) | 0x80;
    *(currentStep++) =  ITM_JtoCAL       & 0xff;

    *(currentStep++) = (ITM_LBFFTtoNM >> 8) | 0x80;
    *(currentStep++) =  ITM_LBFFTtoNM       & 0xff;

    *(currentStep++) = (ITM_LBFFTtoNMb >> 8) | 0x80;
    *(currentStep++) =  ITM_LBFFTtoNMb       & 0xff;

    *(currentStep++) = (ITM_NMtoLBFFT >> 8) | 0x80;
    *(currentStep++) =  ITM_NMtoLBFFT       & 0xff;

    *(currentStep++) = (ITM_NMtoLBFFTb >> 8) | 0x80;
    *(currentStep++) =  ITM_NMtoLBFFTb       & 0xff;

    *(currentStep++) = (ITM_CWTtoKG >> 8) | 0x80;
    *(currentStep++) =  ITM_CWTtoKG       & 0xff;

    *(currentStep++) = (ITM_KGtoCWT >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoCWT       & 0xff;

    *(currentStep++) = (ITM_FTtoM >> 8) | 0x80;
    *(currentStep++) =  ITM_FTtoM       & 0xff;

    *(currentStep++) = (ITM_MtoFT >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoFT       & 0xff;

    // 1260
    *(currentStep++) = (ITM_FTUStoM >> 8) | 0x80;
    *(currentStep++) =  ITM_FTUStoM       & 0xff;

    *(currentStep++) = (ITM_FTUStoMb >> 8) | 0x80;
    *(currentStep++) =  ITM_FTUStoMb       & 0xff;

    *(currentStep++) = (ITM_FTUStoMc >> 8) | 0x80;
    *(currentStep++) =  ITM_FTUStoMc       & 0xff;

    *(currentStep++) = (ITM_MtoFTUS >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoFTUS       & 0xff;

    *(currentStep++) = (ITM_MtoFTUSb >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoFTUSb       & 0xff;

    *(currentStep++) = (ITM_MtoFTUSc >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoFTUSc       & 0xff;

    *(currentStep++) = (ITM_FZUKtoM3 >> 8) | 0x80;
    *(currentStep++) =  ITM_FZUKtoM3       & 0xff;

    *(currentStep++) = (ITM_FZUKtoM3b >> 8) | 0x80;
    *(currentStep++) =  ITM_FZUKtoM3b       & 0xff;

    *(currentStep++) = (ITM_M3toFZUK >> 8) | 0x80;
    *(currentStep++) =  ITM_M3toFZUK       & 0xff;

    *(currentStep++) = (ITM_M3toFZUKb >> 8) | 0x80;
    *(currentStep++) =  ITM_M3toFZUKb       & 0xff;

    // 1270
    *(currentStep++) = (ITM_FZUStoM3 >> 8) | 0x80;
    *(currentStep++) =  ITM_FZUStoM3       & 0xff;

    *(currentStep++) = (ITM_FZUStoM3b >> 8) | 0x80;
    *(currentStep++) =  ITM_FZUStoM3b       & 0xff;

    *(currentStep++) = (ITM_M3toFZUS >> 8) | 0x80;
    *(currentStep++) =  ITM_M3toFZUS       & 0xff;

    *(currentStep++) = (ITM_M3toFZUSb >> 8) | 0x80;
    *(currentStep++) =  ITM_M3toFZUSb       & 0xff;

    *(currentStep++) = (ITM_GLUKtoM3 >> 8) | 0x80;
    *(currentStep++) =  ITM_GLUKtoM3       & 0xff;

    *(currentStep++) = (ITM_M3toGLUK >> 8) | 0x80;
    *(currentStep++) =  ITM_M3toGLUK       & 0xff;

    *(currentStep++) = (ITM_GLUStoM3 >> 8) | 0x80;
    *(currentStep++) =  ITM_GLUStoM3       & 0xff;

    *(currentStep++) = (ITM_M3toGLUS >> 8) | 0x80;
    *(currentStep++) =  ITM_M3toGLUS       & 0xff;

    *(currentStep++) = (ITM_HPEtoW >> 8) | 0x80;
    *(currentStep++) =  ITM_HPEtoW       & 0xff;

    *(currentStep++) = (ITM_WtoHPE >> 8) | 0x80;
    *(currentStep++) =  ITM_WtoHPE       & 0xff;

    // 1280
    *(currentStep++) = (ITM_HPMtoW >> 8) | 0x80;
    *(currentStep++) =  ITM_HPMtoW       & 0xff;

    *(currentStep++) = (ITM_WtoHPM >> 8) | 0x80;
    *(currentStep++) =  ITM_WtoHPM       & 0xff;

    *(currentStep++) = (ITM_HPUKtoW >> 8) | 0x80;
    *(currentStep++) =  ITM_HPUKtoW       & 0xff;

    *(currentStep++) = (ITM_WtoHPUK >> 8) | 0x80;
    *(currentStep++) =  ITM_WtoHPUK       & 0xff;

    *(currentStep++) = (ITM_IHGtoPA >> 8) | 0x80;
    *(currentStep++) =  ITM_IHGtoPA       & 0xff;

    *(currentStep++) = (ITM_IHGtoPA >> 8) | 0x80;
    *(currentStep++) =  ITM_IHGtoPA       & 0xff;

    *(currentStep++) = (ITM_IHGtoPAb >> 8) | 0x80;
    *(currentStep++) =  ITM_IHGtoPAb       & 0xff;

    *(currentStep++) = (ITM_PAtoIHG >> 8) | 0x80;
    *(currentStep++) =  ITM_PAtoIHG       & 0xff;

    *(currentStep++) = (ITM_PAtoIHGb >> 8) | 0x80;
    *(currentStep++) =  ITM_PAtoIHGb       & 0xff;

    *(currentStep++) = (ITM_INtoM >> 8) | 0x80;
    *(currentStep++) =  ITM_INtoM       & 0xff;

    // 1290
    *(currentStep++) = (ITM_MtoINCH >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoINCH       & 0xff;

    *(currentStep++) = (ITM_WHtoJ >> 8) | 0x80;
    *(currentStep++) =  ITM_WHtoJ       & 0xff;

    *(currentStep++) = (ITM_JtoWH >> 8) | 0x80;
    *(currentStep++) =  ITM_JtoWH       & 0xff;

    *(currentStep++) = (ITM_KGtoLBS >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoLBS       & 0xff;

    *(currentStep++) = (ITM_LBStoKG >> 8) | 0x80;
    *(currentStep++) =  ITM_LBStoKG       & 0xff;

    *(currentStep++) = (ITM_KGtoOZ >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoOZ       & 0xff;

    *(currentStep++) = (ITM_OZtoKG >> 8) | 0x80;
    *(currentStep++) =  ITM_OZtoKG       & 0xff;

    *(currentStep++) = (ITM_KGtoSCW >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoSCW       & 0xff;

    *(currentStep++) = (ITM_KGtoSCWb >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoSCWb       & 0xff;

    *(currentStep++) = (ITM_SCWtoKG >> 8) | 0x80;
    *(currentStep++) =  ITM_SCWtoKG       & 0xff;

    // 1300
    *(currentStep++) = (ITM_SCWtoKGb >> 8) | 0x80;
    *(currentStep++) =  ITM_SCWtoKGb       & 0xff;

    *(currentStep++) = (ITM_KGtoSTO >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoSTO       & 0xff;

    *(currentStep++) = (ITM_KGtoSTOb >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoSTOb       & 0xff;

    *(currentStep++) = (ITM_STOtoKG >> 8) | 0x80;
    *(currentStep++) =  ITM_STOtoKG       & 0xff;

    *(currentStep++) = (ITM_STOtoKGb >> 8) | 0x80;
    *(currentStep++) =  ITM_STOtoKGb       & 0xff;

    *(currentStep++) = (ITM_KGtoST >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoST       & 0xff;

    *(currentStep++) = (ITM_KGtoSTb >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoSTb       & 0xff;

    *(currentStep++) = (ITM_KGtoSTc >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoSTc       & 0xff;

    *(currentStep++) = (ITM_STtoKG >> 8) | 0x80;
    *(currentStep++) =  ITM_STtoKG       & 0xff;

    *(currentStep++) = (ITM_STtoKGb >> 8) | 0x80;
    *(currentStep++) =  ITM_STtoKGb       & 0xff;

    // 1310
    *(currentStep++) = (ITM_STtoKGc >> 8) | 0x80;
    *(currentStep++) =  ITM_STtoKGc       & 0xff;

    *(currentStep++) = (ITM_KGtoTON >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoTON       & 0xff;

    *(currentStep++) = (ITM_KGtoTONb >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoTONb       & 0xff;

    *(currentStep++) = (ITM_KGtoTONc >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoTONc       & 0xff;

    *(currentStep++) = (ITM_TONtoKG >> 8) | 0x80;
    *(currentStep++) =  ITM_TONtoKG       & 0xff;

    *(currentStep++) = (ITM_TONtoKGb >> 8) | 0x80;
    *(currentStep++) =  ITM_TONtoKGb       & 0xff;

    *(currentStep++) = (ITM_TONtoKGc >> 8) | 0x80;
    *(currentStep++) =  ITM_TONtoKGc       & 0xff;

    *(currentStep++) = (ITM_KGtoTRZ >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoTRZ       & 0xff;

    *(currentStep++) = (ITM_KGtoTRZb >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoTRZb       & 0xff;

    *(currentStep++) = (ITM_TRZtoKG >> 8) | 0x80;
    *(currentStep++) =  ITM_TRZtoKG       & 0xff;

    // 1320
    *(currentStep++) = (ITM_TRZtoKGb >> 8) | 0x80;
    *(currentStep++) =  ITM_TRZtoKGb       & 0xff;

    *(currentStep++) = (ITM_LBFtoN >> 8) | 0x80;
    *(currentStep++) =  ITM_LBFtoN       & 0xff;

    *(currentStep++) = (ITM_NtoLBF >> 8) | 0x80;
    *(currentStep++) =  ITM_NtoLBF       & 0xff;

    *(currentStep++) = (ITM_LYtoM >> 8) | 0x80;
    *(currentStep++) =  ITM_LYtoM       & 0xff;

    *(currentStep++) = (ITM_MtoLY >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoLY       & 0xff;

    *(currentStep++) = (ITM_MMHGtoPA >> 8) | 0x80;
    *(currentStep++) =  ITM_MMHGtoPA       & 0xff;

    *(currentStep++) = (ITM_MMHGtoPAb >> 8) | 0x80;
    *(currentStep++) =  ITM_MMHGtoPAb       & 0xff;

    *(currentStep++) = (ITM_PAtoMMHG >> 8) | 0x80;
    *(currentStep++) =  ITM_PAtoMMHG       & 0xff;

    *(currentStep++) = (ITM_PAtoMMHGb >> 8) | 0x80;
    *(currentStep++) =  ITM_PAtoMMHGb       & 0xff;

    *(currentStep++) = (ITM_MItoM >> 8) | 0x80;
    *(currentStep++) =  ITM_MItoM       & 0xff;

    // 1330
    *(currentStep++) = (ITM_MtoMI >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoMI       & 0xff;

    *(currentStep++) = (ITM_MtoNMI >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoNMI       & 0xff;

    *(currentStep++) = (ITM_NMItoM >> 8) | 0x80;
    *(currentStep++) =  ITM_NMItoM       & 0xff;

    *(currentStep++) = (ITM_MtoPC >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoPC       & 0xff;

    *(currentStep++) = (ITM_PCtoM >> 8) | 0x80;
    *(currentStep++) =  ITM_PCtoM       & 0xff;

    *(currentStep++) = (ITM_MtoPOINTb >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoPOINTb       & 0xff;

    *(currentStep++) = (ITM_MtoPOINT >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoPOINT       & 0xff;

    *(currentStep++) = (ITM_POINTtoM >> 8) | 0x80;
    *(currentStep++) =  ITM_POINTtoM       & 0xff;

    *(currentStep++) = (ITM_POINTtoMb >> 8) | 0x80;
    *(currentStep++) =  ITM_POINTtoMb       & 0xff;

    *(currentStep++) = (ITM_POINTtoMc >> 8) | 0x80;
    *(currentStep++) =  ITM_POINTtoMc       & 0xff;

    // 1340
    *(currentStep++) = (ITM_MtoYD >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoYD       & 0xff;

    *(currentStep++) = (ITM_YDtoM >> 8) | 0x80;
    *(currentStep++) =  ITM_YDtoM       & 0xff;

    *(currentStep++) = (ITM_PSItoPA >> 8) | 0x80;
    *(currentStep++) =  ITM_PSItoPA       & 0xff;

    *(currentStep++) = (ITM_PAtoPSI >> 8) | 0x80;
    *(currentStep++) =  ITM_PAtoPSI       & 0xff;

    *(currentStep++) = (ITM_PAtoTOR >> 8) | 0x80;
    *(currentStep++) =  ITM_PAtoTOR       & 0xff;

    *(currentStep++) = (ITM_PAtoTORb >> 8) | 0x80;
    *(currentStep++) =  ITM_PAtoTORb       & 0xff;

    *(currentStep++) = (ITM_TORtoPA >> 8) | 0x80;
    *(currentStep++) =  ITM_TORtoPA       & 0xff;

    *(currentStep++) = (ITM_TORtoPAb >> 8) | 0x80;
    *(currentStep++) =  ITM_TORtoPAb       & 0xff;

    *(currentStep++) = (ITM_StoYEAR >> 8) | 0x80;
    *(currentStep++) =  ITM_StoYEAR       & 0xff;

    *(currentStep++) = (ITM_YEARtoS >> 8) | 0x80;
    *(currentStep++) =  ITM_YEARtoS       & 0xff;

    // 1350
    *(currentStep++) = (ITM_CARATtoKG >> 8) | 0x80;
    *(currentStep++) =  ITM_CARATtoKG       & 0xff;

    *(currentStep++) = (ITM_CARATtoKGb >> 8) | 0x80;
    *(currentStep++) =  ITM_CARATtoKGb       & 0xff;

    *(currentStep++) = (ITM_CARATtoKGc >> 8) | 0x80;
    *(currentStep++) =  ITM_CARATtoKGc       & 0xff;

    *(currentStep++) = (ITM_KGtoCARAT >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoCARAT       & 0xff;

    *(currentStep++) = (ITM_KGtoCARATb >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoCARATb       & 0xff;

    *(currentStep++) = (ITM_KGtoCARATc >> 8) | 0x80;
    *(currentStep++) =  ITM_KGtoCARATc       & 0xff;

    *(currentStep++) = (ITM_QTtoM3 >> 8) | 0x80;
    *(currentStep++) =  ITM_QTtoM3       & 0xff;

    *(currentStep++) = (ITM_M3toQT >> 8) | 0x80;
    *(currentStep++) =  ITM_M3toQT       & 0xff;

    *(currentStep++) = (ITM_FATHOMtoM >> 8) | 0x80;
    *(currentStep++) =  ITM_FATHOMtoM       & 0xff;

    *(currentStep++) = (ITM_FATHOMtoMb >> 8) | 0x80;
    *(currentStep++) =  ITM_FATHOMtoMb       & 0xff;

    // 1360
    *(currentStep++) = (ITM_FATHOMtoMc >> 8) | 0x80;
    *(currentStep++) =  ITM_FATHOMtoMc       & 0xff;

    *(currentStep++) = (ITM_MtoFATHOM >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoFATHOM       & 0xff;

    *(currentStep++) = (ITM_MtoFATHOMb >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoFATHOMb       & 0xff;

    *(currentStep++) = (ITM_MtoFATHOMc >> 8) | 0x80;
    *(currentStep++) =  ITM_MtoFATHOMc       & 0xff;

    *(currentStep++) = (ITM_BARRELtoM3 >> 8) | 0x80;
    *(currentStep++) =  ITM_BARRELtoM3       & 0xff;

    *(currentStep++) = (ITM_BARRELtoM3b >> 8) | 0x80;
    *(currentStep++) =  ITM_BARRELtoM3b       & 0xff;

    *(currentStep++) = (ITM_M3toBARREL >> 8) | 0x80;
    *(currentStep++) =  ITM_M3toBARREL       & 0xff;

    *(currentStep++) = (ITM_M3toBARRELb >> 8) | 0x80;
    *(currentStep++) =  ITM_M3toBARRELb       & 0xff;

    *(currentStep++) = (ITM_ATMtoPAb >> 8) | 0x80;
    *(currentStep++) =  ITM_ATMtoPAb       & 0xff;

    *(currentStep++) = (ITM_PAtoATMb >> 8) | 0x80;
    *(currentStep++) =  ITM_PAtoATMb       & 0xff;

    // 1370
    *(currentStep++) = (ITM_HECTAREtoM2 >> 8) | 0x80;
    *(currentStep++) =  ITM_HECTAREtoM2       & 0xff;

    *(currentStep++) = (ITM_M2toHECTARE >> 8) | 0x80;
    *(currentStep++) =  ITM_M2toHECTARE       & 0xff;



    // 1372
    *(currentStep++) = (ITM_END >> 8) | 0x80;
    *(currentStep++) =  ITM_END       & 0xff;

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      printf("Prime + Len1 + Len2 + Len3 + Len4 + Len5 + Len6 + Len7 + Len8 + Bairstow + Speed + Fact + OMp203 + AllOp's : %u bytes\n", (uint32_t)(currentStep - beginOfProgramMemory));
    #endif
  }

  firstFreeProgramByte = currentStep;

  // 1373
  *(currentStep++) = 255; // .END.
  *(currentStep++) = 255; // .END.

  freeProgramBytes = (uint8_t *)(ram + RAM_SIZE) - currentStep;
  currentStep = beginOfProgramMemory;
  scanLabelsAndPrograms();

  #ifndef DMCP_BUILD
    printf("Prime + Bairstow + Speed + Fact + AllOp's + 2 : %u bytes\n", (uint32_t)(currentStep - beginOfProgramMemory));
    if((uint32_t)(currentStep - beginOfProgramMemory) + 2 > numberOfBytesForTheTestPrograms) {
      printf("Increase allocated memory for programs!\n");
      exit(0);
    }

    printf("freeProgramBytes = %u\n", freeProgramBytes);

    //listPrograms();
    listLabelsAndPrograms();
  #endif // !DMCP_BUILD
}



void fnReset(uint16_t confirmation) {
  if(confirmation == NOT_CONFIRMED) {
    setConfirmationMode(fnReset);
  }
  else {
    void *memPtr;

    if(ram == NULL) {
      ram = (dataBlock_t *)malloc(TO_BYTES(RAM_SIZE));
    }
    memset(ram, 0, TO_BYTES(RAM_SIZE));
    numberOfFreeMemoryRegions = 1;
    freeMemoryRegions[0].address = 0;
    freeMemoryRegions[0].sizeInBlocks = RAM_SIZE - 1; // - 1: one block for an empty program

    if(tmpString == NULL) {
      #ifdef DMCP_BUILD
         tmpString        = aux_buf_ptr();   // 2560 byte buffer provided by DMCP
         errorMessage     = write_buf_ptr(); // 4096 byte buffer provided by DMCP
         aimBuffer        = errorMessage + ERROR_MESSAGE_LENGTH;
         nimBufferDisplay = aimBuffer + AIM_BUFFER_LENGTH;
         tamBuffer        = nimBufferDisplay + NIM_BUFFER_LENGTH;
       #else
         tmpString        = (char *)malloc(TMP_STR_LENGTH);
         errorMessage     = (char *)malloc(ERROR_MESSAGE_LENGTH);
         aimBuffer        = (char *)malloc(AIM_BUFFER_LENGTH);
         nimBufferDisplay = (char *)malloc(NIM_BUFFER_LENGTH);
         tamBuffer        = (char *)malloc(TAM_BUFFER_LENGTH);
      #endif // DMCP_BUILD
    }
    memset(tmpString,        0, TMP_STR_LENGTH);
    memset(errorMessage,     0, ERROR_MESSAGE_LENGTH);
    memset(aimBuffer,        0, AIM_BUFFER_LENGTH);
    memset(nimBufferDisplay, 0, NIM_BUFFER_LENGTH);
    memset(tamBuffer,        0, TAM_BUFFER_LENGTH);

    // Empty program initialization
    beginOfProgramMemory        = (uint8_t *)(ram + freeMemoryRegions[0].sizeInBlocks);
    currentStep                 = beginOfProgramMemory;
    firstFreeProgramByte        = beginOfProgramMemory + 2;
    firstDisplayedStep          = beginOfProgramMemory;
    firstDisplayedStepNumber    = 0;
    *(beginOfProgramMemory + 0) = (ITM_END >> 8) | 0x80;
    *(beginOfProgramMemory + 1) =  ITM_END       & 0xff;
    *(beginOfProgramMemory + 2) = 255; // .END.
    *(beginOfProgramMemory + 3) = 255; // .END.
    freeProgramBytes            = 0;
    scanLabelsAndPrograms();

    // "Not found glyph" initialization
    if(glyphNotFound.data == NULL) {
      glyphNotFound.data = malloc(38);
    }
    xcopy(glyphNotFound.data, "\xff\xf8\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\xff\xf8", 38);

    // Initialization of user key assignments
    xcopy(kbd_usr, kbd_std, sizeof(kbd_std));
    //kbd_usr[ 0].keyLblAim   = CHR_A_GRAVE;
    //kbd_usr[ 0].fShiftedAim = CHR_A_GRAVE;
    //kbd_usr[ 4].keyLblAim   = CHR_E_ACUTE;
    //kbd_usr[ 4].fShiftedAim = CHR_E_ACUTE;
    //kbd_usr[18].fShifted    = -MNU_VARS;
    //kbd_usr[18].gShifted    = CST_54;
    //kbd_usr[19].fShifted    = ITM_SW;
    //kbd_usr[19].gShifted    = ITM_SXY;
    //kbd_usr[20].gShifted    = ITM_LYtoM;

    // initialize the global registers
    for(calcRegister_t regist=0; regist<FIRST_LOCAL_REGISTER; regist++) {
      setRegisterDataType(regist, dtReal34, AM_NONE);
      memPtr = allocWp43s(TO_BYTES(REAL34_SIZE));
      setRegisterDataPointer(regist, memPtr);
      real34Zero(memPtr);
    }

    // initialize the 9+1 saved stack registers
    for(calcRegister_t regist=SAVED_REGISTER_X; regist<=LAST_SAVED_REGISTER; regist++) {
      setRegisterDataType(regist, dtReal34, AM_NONE);
      memPtr = allocWp43s(TO_BYTES(REAL34_SIZE));
      setRegisterDataPointer(regist, memPtr);
      real34Zero(memPtr);
    }

    // Clear global flags
    memset(globalFlags, 0, sizeof(globalFlags));

    // allocating space for the named variable list
    allNamedVariablePointer = allocWp43s(TO_BYTES(1)); //  1 block for the number of named variables
    allNamedVariablePointer->numberOfNamedVariables = 0;

    // allocate space for the local register list
    allLocalRegisterPointer = allocWp43s(TO_BYTES(1)); //  1 block for the number of local registers and the local flags
    numberOfLocalFlags = 0;
    allLocalRegisterPointer->numberOfLocalRegisters = 0;
    allLocalRegisterPointer->localFlags = 0;

    #ifdef PC_BUILD
      debugWindow = DBG_REGISTERS;
    #endif

    decContextDefault(&ctxtReal34, DEC_INIT_DECQUAD);

    decContextDefault(&ctxtReal39, DEC_INIT_DECQUAD);
    ctxtReal39.digits = 39;
    ctxtReal39.traps  = 0;

    decContextDefault(&ctxtReal51, DEC_INIT_DECQUAD);
    ctxtReal51.digits = 51;
    ctxtReal51.traps  = 0;

    decContextDefault(&ctxtReal75, DEC_INIT_DECQUAD);
    ctxtReal75.digits = 75;
    ctxtReal75.traps  = 0;

    decContextDefault(&ctxtReal1071,  DEC_INIT_DECQUAD);
    ctxtReal1071.digits = 1071;
    ctxtReal1071.traps  = 0;

    //decContextDefault(&ctxtReal2139,  DEC_INIT_DECQUAD);
    //ctxtReal2139.digits = 2139;
    //ctxtReal2139.traps  = 0;

    statisticalSumsPointer = NULL;
    savedStatisticalSumsPointer = NULL;

    shortIntegerMode = SIM_2COMPL;
    fnSetWordSize(64);

    groupingGap = 3;

    systemFlags = 0;
    displayFormat = DF_ALL;
    displayFormatDigits = 0;
    currentAngularMode = AM_DEGREE;
    denMax = MAX_DENMAX;
    setSystemFlag(FLAG_DENANY);
    setSystemFlag(FLAG_MULTx);
    setSystemFlag(FLAG_DECIMP);
    setSystemFlag(FLAG_AUTOFF);
    setSystemFlag(FLAG_TDM24); // time format = 24H
    setSystemFlag(FLAG_YMD);   // date format = yyyy-mm-dd
    setSystemFlag(FLAG_ASLIFT);

    hourGlassIconEnabled = false;
    watchIconEnabled = false;
    serialIOIconEnabled = false;
    printerIconEnabled = false;
    thereIsSomethingToUndo = false;
    shiftF = false;
    shiftG = false;

    significantDigits = 0;
    roundingMode = RM_HALF_EVEN;
    ctxtReal34.round = DEC_ROUND_HALF_EVEN;
    displayStack = 4;

    initFontBrowser();
    currentFlgScr = 0;
    currentRegisterBrowserScreen = 9999;
    softmenuStackPointer = 0;
    aimBuffer[0] = 0;
    lastErrorCode = 0;

    gammaLanczosCoefficients = (real51_t *)const_gammaC01;
    angle180 = (real39_t *)const_180;
    angle90  = (real39_t *)const_90;
    angle45  = (real39_t *)const_45;

    #ifndef TESTSUITE_BUILD
      resetAlphaSelectionBuffer();
    #endif

    #ifdef TESTSUITE_BUILD
      calcMode = CM_NORMAL;
    #else
      calcModeNormal();
    #endif // TESTSUITE_BUILD

    #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
      debugMemAllocation = true;
    #endif

    // RNG initialisation
    pcg32_srandom(0x1963073019931121ULL, 0x1995062319981019ULL);

    alphaSelectionMenu = ASM_NONE;
    lastFcnsMenuPos = 0;
    lastMenuMenuPos = 0;
    lastCnstMenuPos = 0;
    lastSyFlMenuPos = 0;
    lastAIntMenuPos = 0;
    firstGregorianDay = 1752;
    exponentLimit = 6145;
    lastIntegerBase = 0;
    temporaryInformation = TI_RESET;

    // The following lines are test data
    addTestPrograms();
    //fnSetFlag(  3);
    //fnSetFlag( 11);
    //fnSetFlag( 33);
    //fnSetFlag( 34);
    //fnSetFlag( 52);
    //fnSetFlag( 62);
    //fnSetFlag( 77);
    //fnSetFlag( 85);
    //setSystemFlag(FLAG_CARRY);
    //setSystemFlag(FLAG_SPCRES);

    //allocateLocalRegisters(2);
    //fnSetFlag(FIRST_LOCAL_REGISTER+0);
    //fnSetFlag(NUMBER_OF_GLOBAL_FLAGS+2);
    //reallocateRegister(FIRST_LOCAL_REGISTER+0, dtReal16, REAL16_SIZE, RT_REAL);
    //stringToReal16("5.555", REGISTER_REAL34_DATA(FIRST_LOCAL_REGISTER));

    //strcpy(tmpString, "Pure ASCII string requiring 38 bytes!");
    //reallocateRegister(FIRST_LOCAL_REGISTER+1, dtString, TO_BLOCKS(strlen(tmpString) + 1), AM_NONE);
    //strcpy(REGISTER_STRING_DATA(FIRST_LOCAL_REGISTER + 1), tmpString);


    //allocateNamedVariable("Z" STD_a_DIARESIS "hler");
    //allocateNamedVariable(STD_omega STD_SUB_1);
    //allocateNamedVariable(STD_omega STD_SUB_2);

    #if (DEBUG_PANEL == 1)
      debugWindow = DBG_REGISTERS;
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chkHexaString), false);
      refreshDebugPanel();
    #endif
  }
}



void backToSystem(uint16_t unusedParamButMandatory) {
  #ifdef PC_BUILD
    fnOff(NOPARAM);
  #endif

  #ifdef DMCP_BUILD
    backToDMCP = true;
  #endif
}
