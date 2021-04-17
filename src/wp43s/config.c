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

#include "config.h"

#include "browsers/fontBrowser.h"
#include "bufferize.h"
#include "charString.h"
#include "constantPointers.h"
#include "debug.h"
#include "error.h"
#include "flags.h"
#include "gui.h"
#include "items.h"
#include "keyboard.h"
#include "memory.h"
#include "programming/manage.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "stack.h"
#include "stats.h"
#include <stdlib.h>
#include <string.h>

#include "wp43s.h"



/********************************************//**
 * \brief Sets the calc to meet Chinese standards
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnConfigChina(uint16_t unusedButMandatoryParameter) {
  setSystemFlag(FLAG_DECIMP);
  groupingGap = 4;
  setSystemFlag(FLAG_TDM24); // time format = 24H
  clearSystemFlag(FLAG_DMY); // date format
  clearSystemFlag(FLAG_MDY); // date format
  setSystemFlag(FLAG_YMD);   // date format
  firstGregorianDay = 2433191 /* 1 Oct 1949 */;  // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet European standards
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnConfigEurope(uint16_t unusedButMandatoryParameter) {
  clearSystemFlag(FLAG_DECIMP);
  groupingGap = 3;
  setSystemFlag(FLAG_TDM24); // time format = 24H
  clearSystemFlag(FLAG_MDY); // date format
  clearSystemFlag(FLAG_YMD); // date format
  setSystemFlag(FLAG_DMY);   // date format
  firstGregorianDay = 2299161 /* 15 Oct 1582 */; // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet Indian standards
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnConfigIndia(uint16_t unusedButMandatoryParameter) {
  setSystemFlag(FLAG_DECIMP);
  groupingGap = 3;
  setSystemFlag(FLAG_TDM24); // time format = 24H
  clearSystemFlag(FLAG_MDY); // date format
  clearSystemFlag(FLAG_YMD); // date format
  setSystemFlag(FLAG_DMY);   // date format
  firstGregorianDay = 2361222 /* 14 Sept 1752 */; // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet Japanese standards
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnConfigJapan(uint16_t unusedButMandatoryParameter) {
  setSystemFlag(FLAG_DECIMP);
  groupingGap = 3;
  setSystemFlag(FLAG_TDM24); // time format = 24H
  clearSystemFlag(FLAG_MDY); // date format
  clearSystemFlag(FLAG_DMY); // date format
  setSystemFlag(FLAG_YMD);   // date format
  firstGregorianDay = 2405160 /* 1 Jan 1873 */; // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet English standards
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnConfigUk(uint16_t unusedButMandatoryParameter) {
  setSystemFlag(FLAG_DECIMP);
  groupingGap = 3;
  clearSystemFlag(FLAG_TDM24); // time format = 12H
  clearSystemFlag(FLAG_MDY);   // date format
  clearSystemFlag(FLAG_YMD);   // date format
  setSystemFlag(FLAG_DMY);     // date format
  firstGregorianDay = 2361222 /* 14 Sept 1752 */; // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet American standards
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnConfigUsa(uint16_t unusedButMandatoryParameter) {
  setSystemFlag(FLAG_DECIMP);
  groupingGap = 3;
  clearSystemFlag(FLAG_TDM24); // time format = 12H
  clearSystemFlag(FLAG_YMD);   // date format
  clearSystemFlag(FLAG_DMY);   // date format
  setSystemFlag(FLAG_MDY);     // date format
  firstGregorianDay = 2361222 /* 14 Sept 1752 */; // JDN of the first day in the Gregorian calendar
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
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnWho(uint16_t unusedButMandatoryParameter) {
  temporaryInformation = TI_WHO;
 }



/********************************************//**
 * \brief Displays the version of this software
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnVersion(uint16_t unusedButMandatoryParameter) {
  temporaryInformation = TI_VERSION;
}



/********************************************//**
 * \brief Sets X to the amount of free RAM
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnFreeMemory(uint16_t unusedButMandatoryParameter) {
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
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnGetRoundingMode(uint16_t unusedButMandatoryParameter) {
  longInteger_t rounding;

  liftStack();

  longIntegerInit(rounding);
  uIntToLongInteger(roundingMode, rounding);
  convertLongIntegerToLongIntegerRegister(rounding, REGISTER_X);
  longIntegerFree(rounding);
}



/********************************************//**
 * \brief Sets the rounding mode
 *
 * \param[in] RM uint16_t
 * \return void
 ***********************************************/
void fnSetRoundingMode(uint16_t RM) {
  roundingMode = RM;
}

// "enum rounding" does not match with the specification of WP 43s rounding mode.
// So you need roundingModeTable[roundingMode] rather than roundingMode
// to specify rounding mode in the real number functions.
TO_QSPI const enum rounding roundingModeTable[7] = {
  DEC_ROUND_HALF_EVEN, DEC_ROUND_HALF_UP, DEC_ROUND_HALF_DOWN,
  DEC_ROUND_UP, DEC_ROUND_DOWN, DEC_ROUND_CEILING, DEC_ROUND_FLOOR
};



/********************************************//**
 * \brief Sets X to the value of the integer mode
 *
 * \return void
 ***********************************************/
void fnGetIntegerSignMode(uint16_t unusedButMandatoryParameter) {
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
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnGetWordSize(uint16_t unusedButMandatoryParameter) {
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
 * \param[in] unusedButMandatoryParameter uint16_t
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
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnFreeFlashMemory(uint16_t unusedButMandatoryParameter) {
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
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnBatteryVoltage(uint16_t unusedButMandatoryParameter) {
  real_t value;

  liftStack();

  #ifdef PC_BUILD
    int32ToReal(3100, &value);
  #endif // PC_BUILD

  #ifdef DMCP_BUILD
    int32ToReal(get_vbat(), &value);
  #endif // DMCP_BUILD

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
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnGetSignificantDigits(uint16_t unusedButMandatoryParameter) {
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



void fnFractionType(uint16_t unusedButMandatoryParameter) {
  if(getSystemFlag(FLAG_FRACT)) {
    flipSystemFlag(FLAG_PROPFR);
  }
  else {
    setSystemFlag(FLAG_FRACT);
    setSystemFlag(FLAG_PROPFR);
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



void fnRange(uint16_t unusedButMandatoryParameter) {
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
    #endif //  (EXTRA_INFO_ON_CALC_ERROR == 1)
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



void fnGetRange(uint16_t unusedButMandatoryParameter) {
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
      freeWp43s(savedStatisticalSumsPointer, NUMBER_OF_STATISTICAL_SUMS * REAL_SIZE);
    }

    // Clear local registers
    allocateLocalRegisters(0);

    // Clear registers including stack, I, J, K and L
    for(regist=0; regist<FIRST_LOCAL_REGISTER; regist++) {
      clearRegister(regist);
    }

    // Clear saved stack registers
    for(regist=FIRST_SAVED_STACK_REGISTER; regist<=LAST_TEMP_REGISTER; regist++) {
      clearRegister(regist);
    }
    thereIsSomethingToUndo = false;

    // TODO: clear (or delete) named variables

    // Clear global flags
    fnClFAll(CONFIRMED);

    temporaryInformation = TI_NO_INFO;
  }
}



void addTestPrograms(void) {
  uint32_t numberOfBytesUsed, numberOfBytesForTheTestPrograms = TO_BYTES(TO_BLOCKS(8364)); // Multiple of 4

  resizeProgramMemory(TO_BLOCKS(numberOfBytesForTheTestPrograms));
  firstDisplayedStep            = beginOfProgramMemory;
  currentStep                   = beginOfProgramMemory;
  currentLocalStepNumber        = 1;
  firstDisplayedLocalStepNumber = 0;

  #ifdef DMCP_BUILD
    if(f_open(ppgm_fp, "testPgms.bin", FA_READ) != FR_OK) {
      *(beginOfProgramMemory)     = 255; // .END.
      *(beginOfProgramMemory + 1) = 255; // .END.
      firstFreeProgramByte = beginOfProgramMemory;
      freeProgramBytes = numberOfBytesForTheTestPrograms - 2;
    }
    else {
      UINT bytesRead;
      f_read(ppgm_fp, &numberOfBytesUsed,   sizeof(numberOfBytesUsed), &bytesRead);
      f_read(ppgm_fp, beginOfProgramMemory, numberOfBytesUsed,         &bytesRead);
      f_close(ppgm_fp);

      firstFreeProgramByte = beginOfProgramMemory + (numberOfBytesUsed - 2);
      freeProgramBytes = numberOfBytesForTheTestPrograms - numberOfBytesUsed;
    }

    scanLabelsAndPrograms();
  #else // !DMCP_BUILD
    FILE *testPgms;

    testPgms = fopen("res/dmcp/testPgms.bin", "rb");
    if(testPgms == NULL) {
      printf("Cannot open file res/dmcp/testPgms.bin\n");
      *(beginOfProgramMemory)     = 255; // .END.
      *(beginOfProgramMemory + 1) = 255; // .END.
      firstFreeProgramByte = beginOfProgramMemory;
      freeProgramBytes = numberOfBytesForTheTestPrograms - 2;
    }
    else {
      ignore_result(fread(&numberOfBytesUsed, 1, sizeof(numberOfBytesUsed), testPgms));
      printf("%u bytes\n", numberOfBytesUsed);
      if(numberOfBytesUsed > numberOfBytesForTheTestPrograms) {
        printf("Increase allocated memory for programs!\n");
        fclose(testPgms);
        exit(0);
      }
      ignore_result(fread(beginOfProgramMemory, 1, numberOfBytesUsed, testPgms));
      fclose(testPgms);

      firstFreeProgramByte = beginOfProgramMemory + (numberOfBytesUsed - 2);
      freeProgramBytes = numberOfBytesForTheTestPrograms - numberOfBytesUsed;
    }

    printf("freeProgramBytes = %u\n", freeProgramBytes);

    scanLabelsAndPrograms();
    #ifndef TESTSUITE_BUILD
      leavePem();
    #endif // TESTSUITE_BUILD
    printf("freeProgramBytes = %u\n", freeProgramBytes);
    //listPrograms();
    //listLabelsAndPrograms();
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
       #else // !DMCP_BUILD
         tmpString        = (char *)malloc(TMP_STR_LENGTH);
         errorMessage     = (char *)malloc(WRITE_BUFFER_LEN);
       #endif // DMCP_BUILD

       aimBuffer        = errorMessage + ERROR_MESSAGE_LENGTH;
       nimBufferDisplay = aimBuffer + AIM_BUFFER_LENGTH;
       tamBuffer        = nimBufferDisplay + NIM_BUFFER_LENGTH;

       tmpStringLabelOrVariableName = tmpString + 1000;
    }
    memset(tmpString,        0, TMP_STR_LENGTH);
    memset(errorMessage,     0, ERROR_MESSAGE_LENGTH);
    memset(aimBuffer,        0, AIM_BUFFER_LENGTH);
    memset(nimBufferDisplay, 0, NIM_BUFFER_LENGTH);
    memset(tamBuffer,        0, TAM_BUFFER_LENGTH);

    // Empty program initialization
    beginOfProgramMemory          = (uint8_t *)(ram + freeMemoryRegions[0].sizeInBlocks);
    currentStep                   = beginOfProgramMemory;
    firstFreeProgramByte          = beginOfProgramMemory + 2;
    firstDisplayedStep            = beginOfProgramMemory;
    firstDisplayedLocalStepNumber = 0;
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

    // initialize 9 real34 reserved variables: ACC, ↑Lim, ↓Lim, FV, i%/a, NPER, PER/a, PMT, and PV
    for(int i=0; i<9; i++) {
      real34Zero(allocWp43s(REAL34_SIZE));
    }

    // initialize 1 long integer reserved variables: GRAMOD
    #ifdef OS64BIT
      memPtr = allocWp43s(3);
      ((dataBlock_t *)memPtr)->dataMaxLength = 2;
    #else // !OS64BIT
      memPtr = allocWp43s(2);
      ((dataBlock_t *)memPtr)->dataMaxLength = 1;
    #endif // OS64BIT

    // initialize the global registers
    memset(globalRegister, 0, sizeof(globalRegister));
    for(calcRegister_t regist=0; regist<=LAST_GLOBAL_REGISTER; regist++) {
      setRegisterDataType(regist, dtReal34, amNone);
      memPtr = allocWp43s(REAL34_SIZE);
      setRegisterDataPointer(regist, memPtr);
      real34Zero(memPtr);
    }

    // initialize the NUMBER_OF_SAVED_STACK_REGISTERS + the NUMBER_OF_TEMP_REGISTERS
    memset(savedStackRegister, 0, sizeof(savedStackRegister));
    for(calcRegister_t regist=FIRST_SAVED_STACK_REGISTER; regist<=LAST_TEMP_REGISTER; regist++) {
      setRegisterDataType(regist, dtReal34, amNone);
      memPtr = allocWp43s(REAL34_SIZE);
      setRegisterDataPointer(regist, memPtr);
      real34Zero(memPtr);
    }

    // Clear global flags
    memset(globalFlags, 0, sizeof(globalFlags));

    // allocate space for the local register list
    allSubroutineLevels.numberOfSubroutineLevels = 1;
    currentSubroutineLevelData = allocWp43s(3);
    allSubroutineLevels.ptrToSubroutineLevel0Data = TO_WP43SMEMPTR(currentSubroutineLevelData);
    currentReturnProgramNumber = 0;
    currentReturnLocalStep = 0;
    currentNumberOfLocalRegisters = 0; // No local register
    currentNumberOfLocalFlags = 0; // No local flags
    currentSubroutineLevel = 0;
    currentPtrToNextLevel = WP43S_NULL;
    currentPtrToPreviousLevel = WP43S_NULL;
    currentLocalFlags = NULL;
    currentLocalRegisters = NULL;

    // allocate space for the named variable list
    numberOfNamedVariables = 0;
    allNamedVariables = NULL;


    allocateNamedVariable("Mat_A", dtReal34Matrix, REAL34_SIZE + 1);
    memPtr = getRegisterDataPointer(FIRST_NAMED_VARIABLE);
    ((dataBlock_t *)memPtr)->matrixRows = 1;
    ((dataBlock_t *)memPtr)->matrixColumns = 1;
    real34Zero(memPtr + 4);

    allocateNamedVariable("Mat_B", dtReal34Matrix, REAL34_SIZE + 1);
    memPtr = getRegisterDataPointer(FIRST_NAMED_VARIABLE + 1);
    ((dataBlock_t *)memPtr)->matrixRows = 1;
    ((dataBlock_t *)memPtr)->matrixColumns = 1;
    real34Zero(memPtr + 4);

    allocateNamedVariable("Mat_X", dtReal34Matrix, REAL34_SIZE + 1);
    memPtr = getRegisterDataPointer(FIRST_NAMED_VARIABLE + 2);
    ((dataBlock_t *)memPtr)->matrixRows = 1;
    ((dataBlock_t *)memPtr)->matrixColumns = 1;
    real34Zero(memPtr + 4);

    #ifdef PC_BUILD
      debugWindow = DBG_REGISTERS;
    #endif // PC_BUILD

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
    timeDisplayFormatDigits = 0;
    currentAngularMode = amDegree;
    denMax = MAX_DENMAX;
    setSystemFlag(FLAG_DENANY);
    setSystemFlag(FLAG_MULTx);
    setSystemFlag(FLAG_DECIMP);
    setSystemFlag(FLAG_AUTOFF);
    setSystemFlag(FLAG_TDM24); // time format = 24H
    setSystemFlag(FLAG_YMD);   // date format = yyyy-mm-dd
    setSystemFlag(FLAG_ASLIFT);
    setSystemFlag(FLAG_PROPFR);

    hourGlassIconEnabled = false;
    watchIconEnabled = false;
    serialIOIconEnabled = false;
    printerIconEnabled = false;
    thereIsSomethingToUndo = false;
    tam.alpha = false;
    fnKeyInCatalog = false;
    shiftF = false;
    shiftG = false;

    significantDigits = 0;
    roundingMode = RM_HALF_EVEN;
    ctxtReal34.round = DEC_ROUND_HALF_EVEN;
    displayStack = 4;

    initFontBrowser();
    currentFlgScr = 0;
    currentRegisterBrowserScreen = 9999;

    memset(softmenuStack, 0, sizeof(softmenuStack)); // This works because the ID of MyMenu is 0

    aimBuffer[0] = 0;
    lastErrorCode = 0;

    gammaLanczosCoefficients = (real51_t *)const_gammaC01;
    angle180 = (real39_t *)const_pi;
    angle90  = (real39_t *)const_piOn2;
    angle45  = (real39_t *)const_piOn4;

    #ifndef TESTSUITE_BUILD
      resetAlphaSelectionBuffer();
    #endif // !TESTSUITE_BUILD

    #ifdef TESTSUITE_BUILD
      calcMode = CM_NORMAL;
    #else // TESTSUITE_BUILD
      calcModeNormal();
    #endif // TESTSUITE_BUILD

    #if defined(PC_BUILD) || defined(TESTSUITE_BUILD)
      debugMemAllocation = true;
    #endif // PC_BUILD || TESTSUITE_BUILD

    // RNG initialisation
    pcg32_srandom(0x1963073019931121ULL, 0x1995062319981019ULL);

    tam.mode = 0;
    catalog = CATALOG_NONE;
    memset(lastCatalogPosition, 0, NUMBER_OF_CATALOGS * sizeof(lastCatalogPosition[0]));
    firstGregorianDay = 2361222 /* 14 Sept 1752 */;
    exponentLimit = 6145;
    lastIntegerBase = 0;
    temporaryInformation = TI_RESET;

    #ifdef DMCP_BUILD //JM temporary USER mode setup to run standard 43S on C43 template. Only change is this. SIN changes to TRIG; COS to f; TAN to g. Yellow shift remain normal f. C43 specific menus just fall away.
      fnSetFlag(FLAG_USER);

      kbd_usr[0].primary=ITM_SIGMAPLUS;                 kbd_usr[0].fShifted=ITM_NULL;                     kbd_usr[0].gShifted=ITM_TGLFRT;                   kbd_usr[0].keyLblAim=ITM_NULL;                    kbd_usr[0].primaryAim=ITM_A;                      kbd_usr[0].fShiftedAim=-MNU_ALPHAINTL;            kbd_usr[0].gShiftedAim=ITM_ALPHA;                 kbd_usr[0].primaryTam=ITM_REG_A;                  
      kbd_usr[1].primary=ITM_1ONX;                      kbd_usr[1].fShifted=ITM_YX;                       kbd_usr[1].gShifted=ITM_toINT;                    kbd_usr[1].keyLblAim=ITM_NUMBER_SIGN;             kbd_usr[1].primaryAim=ITM_B;                      kbd_usr[1].fShiftedAim=ITM_NUMBER_SIGN;           kbd_usr[1].gShiftedAim=ITM_BETA;                  kbd_usr[1].primaryTam=ITM_REG_B;                  
      kbd_usr[2].primary=ITM_SQUAREROOTX;               kbd_usr[2].fShifted=ITM_SQUARE;                   kbd_usr[2].gShifted=ITM_DMS;                      kbd_usr[2].keyLblAim=ITM_CHECK_MARK;              kbd_usr[2].primaryAim=ITM_C;                      kbd_usr[2].fShiftedAim=ITM_CHECK_MARK;            kbd_usr[2].gShiftedAim=ITM_CHI;                   kbd_usr[2].primaryTam=ITM_REG_C;                  
      kbd_usr[3].primary=ITM_LOG10;                     kbd_usr[3].fShifted=ITM_10x;                      kbd_usr[3].gShifted=ITM_dotD;                     kbd_usr[3].keyLblAim=ITM_NULL;                    kbd_usr[3].primaryAim=ITM_D;                      kbd_usr[3].fShiftedAim=ITM_NULL;                  kbd_usr[3].gShiftedAim=ITM_DELTA;                 kbd_usr[3].primaryTam=ITM_REG_D;                  
      kbd_usr[4].primary=ITM_LN;                        kbd_usr[4].fShifted=ITM_EXP;                      kbd_usr[4].gShifted=ITM_toREC;                    kbd_usr[4].keyLblAim=ITM_NULL;                    kbd_usr[4].primaryAim=ITM_E;                      kbd_usr[4].fShiftedAim=ITM_NULL;                  kbd_usr[4].gShiftedAim=ITM_EPSILON;               kbd_usr[4].primaryTam=ITM_NULL;                   
      kbd_usr[5].primary=ITM_XEQ;                       kbd_usr[5].fShifted=ITM_AIM;                      kbd_usr[5].gShifted=ITM_toPOL;                    kbd_usr[5].keyLblAim=ITM_NULL;                    kbd_usr[5].primaryAim=ITM_F;                      kbd_usr[5].fShiftedAim=ITM_NULL;                  kbd_usr[5].gShiftedAim=ITM_NULL;                  kbd_usr[5].primaryTam=ITM_alpha;                  
      kbd_usr[6].primary=ITM_STO;                       kbd_usr[6].fShifted=ITM_MAGNITUDE;                kbd_usr[6].gShifted=ITM_ANGLE;                    kbd_usr[6].keyLblAim=ITM_NULL;                    kbd_usr[6].primaryAim=ITM_G;                      kbd_usr[6].fShiftedAim=ITM_NULL;                  kbd_usr[6].gShiftedAim=ITM_GAMMA;                 kbd_usr[6].primaryTam=ITM_NULL;                   
      kbd_usr[7].primary=ITM_RCL;                       kbd_usr[7].fShifted=ITM_PC;                       kbd_usr[7].gShifted=ITM_DELTAPC;                  kbd_usr[7].keyLblAim=ITM_NULL;                    kbd_usr[7].primaryAim=ITM_H;                      kbd_usr[7].fShiftedAim=ITM_NULL;                  kbd_usr[7].gShiftedAim=ITM_ETA;                   kbd_usr[7].primaryTam=ITM_HEX;                    
      kbd_usr[8].primary=ITM_Rdown;                     kbd_usr[8].fShifted=ITM_CONSTpi;                  kbd_usr[8].gShifted=ITM_XTHROOT;                  kbd_usr[8].keyLblAim=ITM_NULL;                    kbd_usr[8].primaryAim=ITM_I;                      kbd_usr[8].fShiftedAim=ITM_NULL;                  kbd_usr[8].gShiftedAim=ITM_IOTA;                  kbd_usr[8].primaryTam=ITM_REG_I;                  
      kbd_usr[9].primary=-MNU_TRI;                      kbd_usr[9].fShifted=ITM_DROP;                     kbd_usr[9].gShifted=ITM_FILL;                     kbd_usr[9].keyLblAim=ITM_NULL;                    kbd_usr[9].primaryAim=ITM_J;                      kbd_usr[9].fShiftedAim=ITM_NULL;                  kbd_usr[9].gShiftedAim=ITM_THETA;                 kbd_usr[9].primaryTam=ITM_REG_J;                  
      kbd_usr[10].primary=ITM_SHIFTf;                   kbd_usr[10].fShifted=ITM_NULL;                    kbd_usr[10].gShifted=ITM_NULL;                    kbd_usr[10].keyLblAim=ITM_NULL;                   kbd_usr[10].primaryAim=ITM_K;                     kbd_usr[10].fShiftedAim=ITM_NULL;                 kbd_usr[10].gShiftedAim=ITM_KAPPA;                kbd_usr[10].primaryTam=ITM_REG_K;                 
      kbd_usr[11].primary=ITM_SHIFTg;                   kbd_usr[11].fShifted=ITM_NULL;                    kbd_usr[11].gShifted=ITM_NULL;                    kbd_usr[11].keyLblAim=ITM_NULL;                   kbd_usr[11].primaryAim=ITM_L;                     kbd_usr[11].fShiftedAim=ITM_NULL;                 kbd_usr[11].gShiftedAim=ITM_LAMBDA;               kbd_usr[11].primaryTam=ITM_REG_L;                 
      kbd_usr[12].primary=ITM_ENTER;                    kbd_usr[12].fShifted=ITM_CC;                      kbd_usr[12].gShifted=-MNU_CPX;                    kbd_usr[12].keyLblAim=ITM_ENTER;                  kbd_usr[12].primaryAim=ITM_ENTER;                 kbd_usr[12].fShiftedAim=ITM_NULL;                 kbd_usr[12].gShiftedAim=ITM_NULL;                 kbd_usr[12].primaryTam=ITM_ENTER;                 
      kbd_usr[13].primary=ITM_XexY;                     kbd_usr[13].fShifted=ITM_LASTX;                   kbd_usr[13].gShifted=ITM_Rup;                     kbd_usr[13].keyLblAim=ITM_ex;                     kbd_usr[13].primaryAim=ITM_M;                     kbd_usr[13].fShiftedAim=ITM_ex;                   kbd_usr[13].gShiftedAim=ITM_MU;                   kbd_usr[13].primaryTam=ITM_NULL;                  
      kbd_usr[14].primary=ITM_CHS;                      kbd_usr[14].fShifted=-MNU_MODE;                   kbd_usr[14].gShifted=-MNU_STK;                    kbd_usr[14].keyLblAim=ITM_PLUS_MINUS;             kbd_usr[14].primaryAim=ITM_N;                     kbd_usr[14].fShiftedAim=ITM_PLUS_MINUS;           kbd_usr[14].gShiftedAim=ITM_NU;                   kbd_usr[14].primaryTam=ITM_NULL;                  
      kbd_usr[15].primary=ITM_EXPONENT;                 kbd_usr[15].fShifted=-MNU_DSP;                    kbd_usr[15].gShifted=-MNU_EXP;                    kbd_usr[15].keyLblAim=ITM_NULL;                   kbd_usr[15].primaryAim=ITM_O;                     kbd_usr[15].fShiftedAim=ITM_UP_ARROW;             kbd_usr[15].gShiftedAim=ITM_OMICRON;              kbd_usr[15].primaryTam=ITM_NULL;                  
      kbd_usr[16].primary=ITM_BACKSPACE;                kbd_usr[16].fShifted=ITM_UNDO;                    kbd_usr[16].gShifted=-MNU_CLR;                    kbd_usr[16].keyLblAim=ITM_BACKSPACE;              kbd_usr[16].primaryAim=ITM_BACKSPACE;             kbd_usr[16].fShiftedAim=ITM_UNDO;                 kbd_usr[16].gShiftedAim=-MNU_CLR;                 kbd_usr[16].primaryTam=ITM_BACKSPACE;             
      kbd_usr[17].primary=ITM_UP;                       kbd_usr[17].fShifted=ITM_BST;                     kbd_usr[17].gShifted=ITM_RBR;                     kbd_usr[17].keyLblAim=ITM_UP;                     kbd_usr[17].primaryAim=ITM_UP;                    kbd_usr[17].fShiftedAim=ITM_NULL;                 kbd_usr[17].gShiftedAim=ITM_UP_ARROW;             kbd_usr[17].primaryTam=ITM_UP;                    
      kbd_usr[18].primary=ITM_7;                        kbd_usr[18].fShifted=-MNU_EQN;                    kbd_usr[18].gShifted=ITM_NULL;                    kbd_usr[18].keyLblAim=ITM_7;                      kbd_usr[18].primaryAim=ITM_P;                     kbd_usr[18].fShiftedAim=ITM_7;                    kbd_usr[18].gShiftedAim=ITM_PI;                   kbd_usr[18].primaryTam=ITM_7;                     
      kbd_usr[19].primary=ITM_8;                        kbd_usr[19].fShifted=-MNU_ADV;                    kbd_usr[19].gShifted=-MNU_CONST;                  kbd_usr[19].keyLblAim=ITM_8;                      kbd_usr[19].primaryAim=ITM_Q;                     kbd_usr[19].fShiftedAim=ITM_8;                    kbd_usr[19].gShiftedAim=ITM_NULL;                 kbd_usr[19].primaryTam=ITM_8;                     
      kbd_usr[20].primary=ITM_9;                        kbd_usr[20].fShifted=-MNU_MATX;                   kbd_usr[20].gShifted=-MNU_XFN;                    kbd_usr[20].keyLblAim=ITM_9;                      kbd_usr[20].primaryAim=ITM_R;                     kbd_usr[20].fShiftedAim=ITM_9;                    kbd_usr[20].gShiftedAim=ITM_RHO;                  kbd_usr[20].primaryTam=ITM_9;                     
      kbd_usr[21].primary=ITM_DIV;                      kbd_usr[21].fShifted=-MNU_STAT;                   kbd_usr[21].gShifted=-MNU_SUMS;                   kbd_usr[21].keyLblAim=ITM_OBELUS;                 kbd_usr[21].primaryAim=ITM_S;                     kbd_usr[21].fShiftedAim=ITM_OBELUS;               kbd_usr[21].gShiftedAim=ITM_SIGMA;                kbd_usr[21].primaryTam=ITM_DIV;                   
      kbd_usr[22].primary=ITM_DOWN;                     kbd_usr[22].fShifted=ITM_SST;                     kbd_usr[22].gShifted=ITM_STATUS;                  kbd_usr[22].keyLblAim=ITM_DOWN;                   kbd_usr[22].primaryAim=ITM_DOWN;                  kbd_usr[22].fShiftedAim=ITM_NULL;                 kbd_usr[22].gShiftedAim=ITM_DOWN_ARROW;           kbd_usr[22].primaryTam=ITM_DOWN;                  
      kbd_usr[23].primary=ITM_4;                        kbd_usr[23].fShifted=ITM_NULL;                    kbd_usr[23].gShifted=-MNU_CLK;                    kbd_usr[23].keyLblAim=ITM_4;                      kbd_usr[23].primaryAim=ITM_T;                     kbd_usr[23].fShiftedAim=ITM_4;                    kbd_usr[23].gShiftedAim=ITM_TAU;                  kbd_usr[23].primaryTam=ITM_4;                     
      kbd_usr[24].primary=ITM_5;                        kbd_usr[24].fShifted=-MNU_ANGLECONV;              kbd_usr[24].gShifted=-MNU_UNITCONV;               kbd_usr[24].keyLblAim=ITM_5;                      kbd_usr[24].primaryAim=ITM_U;                     kbd_usr[24].fShiftedAim=ITM_5;                    kbd_usr[24].gShiftedAim=ITM_PHI;                  kbd_usr[24].primaryTam=ITM_5;                     
      kbd_usr[25].primary=ITM_6;                        kbd_usr[25].fShifted=-MNU_FLAGS;                  kbd_usr[25].gShifted=-MNU_BITS;                   kbd_usr[25].keyLblAim=ITM_6;                      kbd_usr[25].primaryAim=ITM_V;                     kbd_usr[25].fShiftedAim=ITM_6;                    kbd_usr[25].gShiftedAim=ITM_PSI;                  kbd_usr[25].primaryTam=ITM_6;                     
      kbd_usr[26].primary=ITM_MULT;                     kbd_usr[26].fShifted=-MNU_PROB;                   kbd_usr[26].gShifted=-MNU_INTS;                   kbd_usr[26].keyLblAim=ITM_CROSS;                  kbd_usr[26].primaryAim=ITM_W;                     kbd_usr[26].fShiftedAim=ITM_CROSS;                kbd_usr[26].gShiftedAim=ITM_OMEGA;                kbd_usr[26].primaryTam=ITM_MULT;                  
      kbd_usr[27].primary=ITM_SHIFTf;                   kbd_usr[27].fShifted=ITM_NULL;                    kbd_usr[27].gShifted=ITM_NULL;                    kbd_usr[27].keyLblAim=ITM_SHIFTf;                 kbd_usr[27].primaryAim=ITM_SHIFTf;                kbd_usr[27].fShiftedAim=ITM_NULL;                 kbd_usr[27].gShiftedAim=ITM_NULL;                 kbd_usr[27].primaryTam=ITM_SHIFTf;                
      kbd_usr[28].primary=ITM_1;                        kbd_usr[28].fShifted=ITM_ASSIGN;                  kbd_usr[28].gShifted=ITM_NULL;                    kbd_usr[28].keyLblAim=ITM_1;                      kbd_usr[28].primaryAim=ITM_X;                     kbd_usr[28].fShiftedAim=ITM_1;                    kbd_usr[28].gShiftedAim=ITM_XI;                   kbd_usr[28].primaryTam=ITM_1;                     
      kbd_usr[29].primary=ITM_2;                        kbd_usr[29].fShifted=ITM_USERMODE;                kbd_usr[29].gShifted=-MNU_LOOP;                   kbd_usr[29].keyLblAim=ITM_2;                      kbd_usr[29].primaryAim=ITM_Y;                     kbd_usr[29].fShiftedAim=ITM_2;                    kbd_usr[29].gShiftedAim=ITM_UPSILON;              kbd_usr[29].primaryTam=ITM_2;                     
      kbd_usr[30].primary=ITM_3;                        kbd_usr[30].fShifted=-MNU_PARTS;                  kbd_usr[30].gShifted=-MNU_TEST;                   kbd_usr[30].keyLblAim=ITM_3;                      kbd_usr[30].primaryAim=ITM_Z;                     kbd_usr[30].fShiftedAim=ITM_3;                    kbd_usr[30].gShiftedAim=ITM_ZETA;                 kbd_usr[30].primaryTam=ITM_3;                     
      kbd_usr[31].primary=ITM_SUB;                      kbd_usr[31].fShifted=-MNU_FIN;                    kbd_usr[31].gShifted=-MNU_ALPHAFN;                kbd_usr[31].keyLblAim=ITM_MINUS;                  kbd_usr[31].primaryAim=ITM_UNDERSCORE;            kbd_usr[31].fShiftedAim=ITM_MINUS;                kbd_usr[31].gShiftedAim=ITM_NULL;                 kbd_usr[31].primaryTam=ITM_SUB;                   
      kbd_usr[32].primary=ITM_EXIT;                     kbd_usr[32].fShifted=ITM_OFF;                     kbd_usr[32].gShifted=-MNU_PRINT;                  kbd_usr[32].keyLblAim=ITM_EXIT;                   kbd_usr[32].primaryAim=ITM_EXIT;                  kbd_usr[32].fShiftedAim=ITM_OFF;                  kbd_usr[32].gShiftedAim=ITM_PRINTER;              kbd_usr[32].primaryTam=ITM_EXIT;                  
      kbd_usr[33].primary=ITM_0;                        kbd_usr[33].fShifted=ITM_VIEW;                    kbd_usr[33].gShifted=ITM_TIMER;                   kbd_usr[33].keyLblAim=ITM_0;                      kbd_usr[33].primaryAim=ITM_COLON;                 kbd_usr[33].fShiftedAim=ITM_0;                    kbd_usr[33].gShiftedAim=ITM_NULL;                 kbd_usr[33].primaryTam=ITM_0;                     
      kbd_usr[34].primary=ITM_PERIOD;                   kbd_usr[34].fShifted=ITM_SHOW;                    kbd_usr[34].gShifted=-MNU_INFO;                   kbd_usr[34].keyLblAim=ITM_PERIOD;                 kbd_usr[34].primaryAim=ITM_COMMA;                 kbd_usr[34].fShiftedAim=ITM_PERIOD;               kbd_usr[34].gShiftedAim=-MNU_ALPHADOT;            kbd_usr[34].primaryTam=ITM_PERIOD;                
      kbd_usr[35].primary=ITM_RS;                       kbd_usr[35].fShifted=ITM_PR;                      kbd_usr[35].gShifted=-MNU_PFN;                    kbd_usr[35].keyLblAim=ITM_NULL;                   kbd_usr[35].primaryAim=ITM_QUESTION_MARK;         kbd_usr[35].fShiftedAim=ITM_SLASH;                kbd_usr[35].gShiftedAim=-MNU_ALPHAMATH;           kbd_usr[35].primaryTam=ITM_NULL;                  
      kbd_usr[36].primary=ITM_ADD;                      kbd_usr[36].fShifted=-MNU_CATALOG;                kbd_usr[36].gShifted=-MNU_IO;                     kbd_usr[36].keyLblAim=ITM_PLUS;                   kbd_usr[36].primaryAim=ITM_SPACE;                 kbd_usr[36].fShiftedAim=ITM_PLUS;                 kbd_usr[36].gShiftedAim=-MNU_ALPHAINTL;           kbd_usr[36].primaryTam=ITM_ADD;                   

    #endif


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

    //allocateLocalRegisters(3);
    //fnSetFlag(FIRST_LOCAL_REGISTER+0);
    //fnSetFlag(NUMBER_OF_GLOBAL_FLAGS+2);
    //reallocateRegister(FIRST_LOCAL_REGISTER+0, dtReal16, REAL16_SIZE, RT_REAL);
    //stringToReal16("5.555", REGISTER_REAL34_DATA(FIRST_LOCAL_REGISTER));

    //strcpy(tmpString, "Pure ASCII string requiring 38 bytes!");
    //reallocateRegister(FIRST_LOCAL_REGISTER+1, dtString, TO_BLOCKS(strlen(tmpString) + 1), amNone);
    //strcpy(REGISTER_STRING_DATA(FIRST_LOCAL_REGISTER + 1), tmpString);


    //allocateNamedVariable("Z" STD_a_DIARESIS "hler");
    //allocateNamedVariable(STD_omega STD_SUB_1);
    //allocateNamedVariable(STD_omega STD_SUB_2);

    #if (DEBUG_PANEL == 1)
      debugWindow = DBG_REGISTERS;
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chkHexaString), false);
      refreshDebugPanel();
    #endif //  (DEBUG_PANEL == 1)
  }
}



void backToSystem(uint16_t unusedButMandatoryParameter) {
  #ifdef PC_BUILD
    fnOff(NOPARAM);
  #endif // PC_BUILD

  #ifdef DMCP_BUILD
    backToDMCP = true;
  #endif // DMCP_BUILD
}
