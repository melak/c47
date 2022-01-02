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
 * \file flash.c
 ***********************************************/

#include "programming/flash.h"
#include "defines.h"
#include "error.h"
#include "items.h"
#include "memory.h"
#include "programming/nextStep.h"
#include "wp43s.h"
#include <string.h>

#define LIBDATA               ppgm_fp // The FIL *ppgm_fp pointer is provided by DMCP

#ifdef DMCP_BUILD
  #define FLASH_PGM_DIR  "LIBRARY"
  #define FLASH_PGM_FILE "wp43s.dat"
#else // !DMCP_BUILD
  #define FLASH_PGM_FILE "wp43s.dat"
#endif // DMCP_BUILD

static void save(const void *buffer, uint32_t size, void *stream) {
  #ifdef DMCP_BUILD
    UINT bytesWritten;
    f_write(stream, buffer, size, &bytesWritten);
  #else // !DMCP_BUILD
    fwrite(buffer, 1, size, stream);
  #endif // DMCP_BUILD
}

static uint32_t load(void *buffer, uint32_t size, void *stream) {
  #ifdef DMCP_BUILD
    UINT bytesRead;
    f_read(stream, buffer, size, &bytesRead);
    return(bytesRead);
  #else // !DMCP_BUILD
    return(fread(buffer, 1, size, stream));
  #endif // DMCP_BUILD
}

static void seek(uint32_t pos, void *stream) {
  #ifdef DMCP_BUILD
    f_lseek(stream, pos);
  #else // !DMCP_BUILD
    fseek(stream, pos, SEEK_SET);
  #endif // DMCP_BUILD
}



void deleteFromFlashPgmLibrary(uint32_t fromAddr, uint32_t toAddr) {
  #ifdef DMCP_BUILD
    if(f_open(LIBDATA, FLASH_PGM_DIR "\\" FLASH_PGM_FILE, FA_READ | FA_WRITE | FA_OPEN_EXISTING) != FR_OK) {
      displayCalcErrorMessage(ERROR_NO_BACKUP_DATA, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function deleteFromFlashPgmLibrary: cannot find or read backup data file wp43s.sav", NULL, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }
  #else // !DMCP_BUILD
    FILE *ppgm_fp;

    if((LIBDATA = fopen(FLASH_PGM_FILE, "r+b")) == NULL) {
      displayCalcErrorMessage(ERROR_NO_BACKUP_DATA, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function deleteFromFlashPgmLibrary: cannot find or read backup data file wp43s.sav", NULL, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }
  #endif // DMCP_BUILD

  --toAddr; // argument is 1-based
  --fromAddr; // must convert to 0-based
  do {
    seek(toAddr, LIBDATA);
    load(tmpString, FLASH_PGM_PAGE_SIZE, LIBDATA);
    seek(fromAddr, LIBDATA);
    save(tmpString, FLASH_PGM_PAGE_SIZE, LIBDATA);
    fromAddr += FLASH_PGM_PAGE_SIZE;
    toAddr += FLASH_PGM_PAGE_SIZE;
  } while(toAddr < (sizeOfFlashPgmLibrary + 2));

  #ifdef DMCP_BUILD
    f_close(LIBDATA);
  #else // !DMCP_BUILD
    fclose(LIBDATA);
  #endif //DMCP_BUILD
}



void readStepInFlashPgmLibrary(uint8_t *buffer, uint16_t bufferSize, uint32_t pointer) {
  #ifdef DMCP_BUILD
    if(f_open(LIBDATA, FLASH_PGM_DIR "\\" FLASH_PGM_FILE, FA_READ) != FR_OK) {
      displayCalcErrorMessage(ERROR_NO_BACKUP_DATA, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function scanFlashProgramLibrary: cannot find or read backup data file wp43s.sav", NULL, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }
  #else // !DMCP_BUILD
    FILE *ppgm_fp;

    if((LIBDATA = fopen(FLASH_PGM_FILE, "rb")) == NULL) {
      displayCalcErrorMessage(ERROR_NO_BACKUP_DATA, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function scanFlashProgramLibrary: cannot find or read backup data file wp43s.sav", NULL, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }
  #endif // DMCP_BUILD

  seek(pointer - 1, LIBDATA);
  load(buffer, bufferSize, LIBDATA);

  #ifdef DMCP_BUILD
    f_close(LIBDATA);
  #else // !DMCP_BUILD
    fclose(LIBDATA);
  #endif //DMCP_BUILD
}



void scanFlashPgmLibrary(void) {
  #ifdef DMCP_BUILD
    if(f_open(LIBDATA, FLASH_PGM_DIR "\\" FLASH_PGM_FILE, FA_READ) != FR_OK) {
      initFlashPgmLibrary();
      if(f_open(LIBDATA, FLASH_PGM_DIR "\\" FLASH_PGM_FILE, FA_READ) != FR_OK) {
        displayCalcErrorMessage(ERROR_NO_BACKUP_DATA, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          moreInfoOnError("In function scanFlashProgramLibrary: cannot find or read backup data file wp43s.sav", NULL, NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        return;
      }
    }
  #else // !DMCP_BUILD
    FILE *ppgm_fp;

    if((LIBDATA = fopen(FLASH_PGM_FILE, "rb")) == NULL) {
      initFlashPgmLibrary();
      if((LIBDATA = fopen(FLASH_PGM_FILE, "rb")) == NULL) {
        displayCalcErrorMessage(ERROR_NO_BACKUP_DATA, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          moreInfoOnError("In function scanFlashProgramLibrary: cannot find or read backup data file wp43s.sav", NULL, NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        return;
      }
    }
  #endif // DMCP_BUILD

  uint8_t *nextStep, *step = (uint8_t *)tmpString;
  int32_t seekPos = 0;
  uint32_t stepNumber = 0;

  freeWp43s(flashLabelList, TO_BLOCKS(sizeof(labelList_t)) * numberOfLabelsInFlash);
  freeWp43s(flashProgramList, TO_BLOCKS(sizeof(programList_t)) * numberOfProgramsInFlash);

  numberOfLabelsInFlash = 0;
  numberOfProgramsInFlash = 1;

  load(tmpString, FLASH_PGM_PAGE_SIZE + 32, LIBDATA);
  while(*step != 255 || *(step + 1) != 255) { // .END.
    if(*step == ITM_LBL) { // LBL
      numberOfLabelsInFlash++;
    }
    if((*step & 0x7f) == (ITM_END >> 8) && *(step + 1) == (ITM_END & 0xff)) { // END
      numberOfProgramsInFlash++;
    }
    step = findNextStep_ram(step);
    if(step >= (uint8_t *)tmpString + FLASH_PGM_PAGE_SIZE) {
      seekPos += FLASH_PGM_PAGE_SIZE;
      seek(seekPos, LIBDATA);
      load(tmpString, FLASH_PGM_PAGE_SIZE + 32, LIBDATA);
      step -= FLASH_PGM_PAGE_SIZE;
    }
  }

  sizeOfFlashPgmLibrary = (uint32_t)(((intptr_t)step - (intptr_t)tmpString) + seekPos);

  flashLabelList = allocWp43s(TO_BLOCKS(sizeof(labelList_t)) * numberOfLabelsInFlash);
  if(flashLabelList == NULL) {
    // unlikely
    lastErrorCode = ERROR_RAM_FULL;
    return;
  }

  flashProgramList = allocWp43s(TO_BLOCKS(sizeof(programList_t)) * numberOfProgramsInFlash);
  if(flashProgramList == NULL) {
    // unlikely
    lastErrorCode = ERROR_RAM_FULL;
    return;
  }

  seekPos = 0;
  seek(seekPos, LIBDATA);
  load(tmpString, FLASH_PGM_PAGE_SIZE + 32, LIBDATA);

  numberOfLabelsInFlash = 0;
  step = (uint8_t *)tmpString;
  flashProgramList[0].instructionPointer = (void *)(-1);
  flashProgramList[0].step = -1;

  numberOfProgramsInFlash = 1;
  stepNumber = 1;
  while(*step != 255 || *(step + 1) != 255) { // .END.
    nextStep = findNextStep_ram(step);
    if(*step == 1) { // LBL
      flashLabelList[numberOfLabelsInFlash].program = -numberOfProgramsInFlash;
      if(*(step + 1) <= 109) { // Local label
        flashLabelList[numberOfLabelsInFlash].step = -stepNumber;
        flashLabelList[numberOfLabelsInFlash].labelPointer = (void *)(step - (uint8_t *)tmpString + 1 + seekPos + 1);
      }
      else { // Global label
        flashLabelList[numberOfLabelsInFlash].step = stepNumber;
        flashLabelList[numberOfLabelsInFlash].labelPointer = (void *)(step - (uint8_t *)tmpString + 2 + seekPos + 1);
      }

      flashLabelList[numberOfLabelsInFlash].instructionPointer = (void *)(nextStep - (uint8_t *)tmpString + seekPos + 1);
      numberOfLabelsInFlash++;
    }

    if((*step & 0x7f) == (ITM_END >> 8) && *(step + 1) == (ITM_END & 0xff)) { // END
      flashProgramList[numberOfProgramsInFlash].instructionPointer = (void *)(step - (uint8_t *)tmpString + 2 + seekPos + 1);
      flashProgramList[numberOfProgramsInFlash].step = -(stepNumber + 1);
      numberOfProgramsInFlash++;
    }

    step = nextStep;
    stepNumber++;

    if(step >= (uint8_t *)tmpString + FLASH_PGM_PAGE_SIZE) {
      seekPos += FLASH_PGM_PAGE_SIZE;
      seek(seekPos, LIBDATA);
      load(tmpString, FLASH_PGM_PAGE_SIZE + 32, LIBDATA);
      step -= FLASH_PGM_PAGE_SIZE;
    }
  }

  #ifdef DMCP_BUILD
    f_close(LIBDATA);
  #else // !DMCP_BUILD
    fclose(LIBDATA);
  #endif //DMCP_BUILD
}



void initFlashPgmLibrary(void) {
  #ifdef DMCP_BUILD
    FRESULT result;

    sys_disk_write_enable(1);
    check_create_dir(FLASH_PGM_DIR);
    result = f_open(LIBDATA, FLASH_PGM_DIR "\\" FLASH_PGM_FILE, FA_CREATE_ALWAYS | FA_WRITE);
    if(result != FR_OK) {
      sys_disk_write_enable(0);
      return;
    }
  #else // !DMCP_BUILD
    FILE *ppgm_fp;

    LIBDATA = fopen(FLASH_PGM_FILE, "wb");
    if(LIBDATA == NULL) {
      printf("Cannot SAVE in file " FLASH_PGM_FILE "!\n");
      return;
    }
  #endif // DMCP_BUILD

  memset(tmpString, 0, FLASH_PGM_PAGE_SIZE);
  tmpString[0] = -1;
  tmpString[1] = -1;
  save(tmpString, FLASH_PGM_PAGE_SIZE, LIBDATA);
  tmpString[0] = 0;
  tmpString[1] = 0;
  for(int i = 1; i < FLASH_PGM_NUMBER_OF_PAGES; ++i) {
    save(tmpString, FLASH_PGM_PAGE_SIZE, LIBDATA);
  }

  #ifdef DMCP_BUILD
    f_close(LIBDATA);
    sys_disk_write_enable(0);
  #else // !DMCP_BUILD
    fclose(LIBDATA);
  #endif // DMCP_BUILD
}
