// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: Copyright The WP43 Authors

#include "saveRestoreCalcState.h"
#include "saveRestorePrograms.h"

#include "assign.h"
#include "calcMode.h"
#include "charString.h"
#include "config.h"
#include "display.h"
#include "error.h"
#include "flags.h"
#include "hal/gui.h"
#include "hal/io.h"
#include "items.h"
#include "mathematics/matrix.h"
#include "memory.h"
#include "plotstat.h"
#include "programming/flash.h"
#include "programming/lblGtoXeq.h"
#include "programming/manage.h"
#include "programming/nextStep.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "solver/equation.h"
#include "solver/graph.h"
#include "sort.h"
#include "stats.h"
#include "ui/tam.h"
#if defined(DMCP_BUILD)
  #include <dmcp.h>
//  #include <wp43-dmcp.h>
#endif // DMCP_BUILD
#include <stdbool.h>
#include <string.h>

#include "wp43.h"

#define PROGRAM_VERSION                     01  // Original version
#define OLDEST_COMPATIBLE_PROGRAM_VERSION   01  // Original version
#define BACKUP_FORMAT                       00  // Same program format as in backup file
#define TEXT_FORMAT                         01  // Text program format - for future use

// Structure of the program file.
// Format: ASCII
// Each line after line 6 contains the decimal value of a program byte
//
//  +-----+---------------------------+
//  | Line|         Content           |
//  +-----+---------------------------+
//  |  1  |   "PROGRAM_FILE_FORMAT"   |
//  |  2  |       <file format>       |
//  |  3  |"WP43_program_file_version"|
//  |  4  |   <program file version>  |
//  |  5  |         "PROGRAM"         |
//  |  6  |       <program size>      |
//  |  7  |    <first program byte>   |
//  | ... |            ...            |
//  |  n  |    <last program byte>    |
//  +-----+---------------------------+
//



static void _addSpaceAfterPrograms(uint16_t size) {
  if(freeProgramBytes < size) {
    uint8_t *oldBeginOfProgramMemory = beginOfProgramMemory;
    uint32_t programSizeInBlocks = RAM_SIZE - freeMemoryRegions[numberOfFreeMemoryRegions - 1].address - freeMemoryRegions[numberOfFreeMemoryRegions - 1].sizeInBlocks;
    uint32_t newProgramSizeInBlocks = TO_BLOCKS(TO_BYTES(programSizeInBlocks) - freeProgramBytes + size);
    freeProgramBytes      += TO_BYTES(newProgramSizeInBlocks - programSizeInBlocks);
    resizeProgramMemory(newProgramSizeInBlocks);
    if(programList[currentProgramNumber - 1].step > 0) { // RAM
      currentStep.ram           = currentStep.ram           - oldBeginOfProgramMemory + beginOfProgramMemory;
      firstDisplayedStep.ram    = firstDisplayedStep.ram    - oldBeginOfProgramMemory + beginOfProgramMemory;
      beginOfCurrentProgram.ram = beginOfCurrentProgram.ram - oldBeginOfProgramMemory + beginOfProgramMemory;
      endOfCurrentProgram.ram   = endOfCurrentProgram.ram   - oldBeginOfProgramMemory + beginOfProgramMemory;
    }
  }

  firstFreeProgramByte   += size;
  freeProgramBytes       -= size;
}


static bool _addEndNeeded(void) {
  if(firstFreeProgramByte <= beginOfProgramMemory) {
    return false;
  }
  if(firstFreeProgramByte == beginOfProgramMemory + 1) {
    return true;
  }
  if(isAtEndOfProgram(firstFreeProgramByte - 2)) {
    return false;
  }
  return true;
}



void fnSaveProgram(uint16_t label) {
  #if !defined(TESTSUITE_BUILD)
    uint32_t programVersion = PROGRAM_VERSION;
    ioFilePath_t path;
    //char tmpString[3000];           //The concurrent use of the global tmpString
    //                                //as target does not work while the source is at
    //                                //tmpRegisterString = tmpString + START_REGISTER_VALUE;
    //                                //Temporary solution is to use a local variable of sufficient length for the target.
    uint32_t i;
    int ret;

    #if defined(DMCP_BUILD)
      // Don't pass through if the power is insufficient
      if(power_check_screen()) {
        return;
      }
    #endif // DMCP_BUILD

    // Find program boundaries
    const uint16_t savedCurrentLocalStepNumber = currentLocalStepNumber;
    uint16_t savedCurrentProgramNumber = currentProgramNumber;
    // no argument – need to save current program
    if(label == 0 && !tam.alpha && tam.digitsSoFar == 0) {
    }
    // Existing global label
    else if(label >= FIRST_LABEL && label <= LAST_LABEL) {
      fnGoto(label);
    }
    // Invalid label
    else {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      #if(EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "label %" PRIu16 " is not a global label", label);
        moreInfoOnError("In function fnSaveProgram:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }

    // program in flash memory : return without saving
    if(programList[currentProgramNumber - 1].step < 0) { // flash memory
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      return;
    }

    path = ioPathSaveProgram;
    ret = ioFileOpen(path, ioModeWrite);

    if(ret != FILE_OK ) {
      if(ret == FILE_CANCEL ) {
        return;
      }
      else {
        #if !defined(DMCP_BUILD)
          printf("Cannot save program!\n");
        #endif
        displayCalcErrorMessage(ERROR_CANNOT_WRITE_FILE, ERR_REGISTER_LINE, REGISTER_X);
        return;
      }
    }

    // PROGRAM file version
    sprintf(tmpString, "PROGRAM_FILE_FORMAT\n%" PRIu8 "\n", (uint8_t)BACKUP_FORMAT);
    ioFileWrite(tmpString, strlen(tmpString));
    sprintf(tmpString, "C47_program_file_version\n%" PRIu32 "\n", (uint32_t)programVersion);
    ioFileWrite(tmpString, strlen(tmpString));

    // Program
    size_t currentSizeInBytes = endOfCurrentProgram.ram - ((currentProgramNumber == (numberOfPrograms - numberOfProgramsInFlash)) ? 2 : 0) - beginOfCurrentProgram.ram;
    sprintf(tmpString, "PROGRAM\n%" PRIu32 "\n", (uint32_t)currentSizeInBytes);
    ioFileWrite(tmpString, strlen(tmpString));

    // Save program bytes
    for(i=0; i<currentSizeInBytes; i++) {
      sprintf(tmpString, "%" PRIu8 "\n", beginOfCurrentProgram.ram[i]);
      ioFileWrite(tmpString, strlen(tmpString));
    }
    // If last program in memory then add .END. statement
    if(currentProgramNumber == (numberOfPrograms - numberOfProgramsInFlash)) {
      sprintf(tmpString, "255\n255\n");
      ioFileWrite(tmpString, strlen(tmpString));
    }

    ioFileClose();

    currentLocalStepNumber = savedCurrentLocalStepNumber;
    currentProgramNumber = savedCurrentProgramNumber;

    temporaryInformation = TI_SAVED;
  #endif // !TESTSUITE_BUILD
}


void fnLoadProgram(uint16_t unusedButMandatoryParameter) {
  ioFilePath_t path;
  uint32_t pgmSizeInByte;
  uint32_t i;
  uint8_t *startOfProgram;
  int ret;

  path = ioPathLoadProgram;
  ret = ioFileOpen(path, ioModeRead);

  if(ret != FILE_OK ) {
    if(ret == FILE_CANCEL ) {
      return;
    }
    else {
      displayCalcErrorMessage(ERROR_CANNOT_READ_FILE, ERR_REGISTER_LINE, REGISTER_X);
      return;
    }
  }

  //Check save file version
  uint32_t loadedVersion = 0;
  readLine(tmpString);
  if(strcmp(tmpString, "PROGRAM_FILE_FORMAT") == 0) {
    readLine(aimBuffer); // Format of program instructions (ignore now, there is only one format)
  }
  else {
    #if !defined(TESTSUITE_BUILD)
      sprintf(tmpString," \nThis is not a C47 program\n\nIt will not be loaded.");
      show_warning(tmpString);
    #endif // TESTSUITE_BUILD
    ioFileClose();
    return;
  }
  readLine(aimBuffer); // param
  readLine(tmpString); // value
  if(strcmp(aimBuffer, "C47_program_file_version") == 0) {
    loadedVersion = stringToUint32(tmpString);
    if(loadedVersion < OLDEST_COMPATIBLE_PROGRAM_VERSION) { // Program incompatibility
      #if !defined(TESTSUITE_BUILD)
        sprintf(tmpString, " \n   !!! Program version is too old !!!\nNot compatible with current version\n \nIt will not be loaded.");
        show_warning(tmpString);
      #endif // TESTSUITE_BUILD
      ioFileClose();
      return;
    }
  }
  else {
    if(strcmp(aimBuffer, "WP43_program_file_version") == 0) {
      loadedVersion = stringToUint32(tmpString);
      #if !defined(TESTSUITE_BUILD)
        sprintf(tmpString," \nThis is a WP43 program\nWP43 program support is experimental\nSome instructions may not be \ncompatible with the C47 and may\ncrash the calculator.");
        show_warning(tmpString);
      #endif // TESTSUITE_BUILD
    }
    else {
      #if !defined(TESTSUITE_BUILD)
        sprintf(tmpString, " \nThis is not a C47 program\n \nIt will not be loaded.");
        show_warning(tmpString);
      #endif // TESTSUITE_BUILD
      ioFileClose();
      return;
    }
  }
  readLine(aimBuffer); // param
  readLine(tmpString); // value
  if(strcmp(aimBuffer, "PROGRAM") == 0) {
    pgmSizeInByte = stringToUint32(tmpString);
  }
  else {
    ioFileClose();
    return;
  }

  if(_addEndNeeded()) {
    _addSpaceAfterPrograms(2);
    *(firstFreeProgramByte - 2) = (ITM_END >> 8) | 0x80;
    *(firstFreeProgramByte - 1) =  ITM_END       & 0xff;
    *(firstFreeProgramByte    ) = 0xffu;
    *(firstFreeProgramByte + 1) = 0xffu;
    scanLabelsAndPrograms();
  }

  _addSpaceAfterPrograms(pgmSizeInByte);
  startOfProgram = firstFreeProgramByte - pgmSizeInByte;
  for(i=0; i<pgmSizeInByte; i++) {
    readLine(tmpString); // One byte
    startOfProgram[i] = stringToUint8(tmpString);
  }

  *(firstFreeProgramByte    ) = 0xffu;
  *(firstFreeProgramByte + 1) = 0xffu;
  scanLabelsAndPrograms();

  ioFileClose();

  if(loadedVersion < OLDEST_COMPATIBLE_PROGRAM_VERSION) { // Program incompatibility
    sprintf(tmpString," \n"
                      "   !!! Program version is too old !!!\n"
                      "Not compatible with current version\n"
                      " \n"
                      "It will not be loaded.");
    #if !defined(TESTSUITE_BUILD)
      show_warning(tmpString);
    #endif // TESTSUITE_BUILD
    return;
  }

  temporaryInformation = TI_PROGRAM_LOADED;
}
