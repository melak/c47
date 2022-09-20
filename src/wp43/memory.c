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

#include "memory.h"

#include "charString.h"
#include "config.h"
#include "core/freeList.h"
#include "debug.h"
#include "items.h"
#include "registers.h"

#include "wp43.h"

int32_t getFreeRamMemory(void) {
  int32_t freeMem, i;

  freeMem = 0;
  for(i=0; i<numberOfFreeMemoryRegions; i++) {
    freeMem += freeMemoryRegions[i].sizeInBlocks;
  }

  return TO_BYTES(freeMem);
}

#if !defined(DMCP_BUILD)
  void debugMemory(const char *message) {
    printf("\n%s\nWP43 owns %6" PRIu64 " bytes and GMP owns %6" PRIu64 " bytes (%" PRId32 " bytes free)\n", message, (uint64_t)TO_BYTES(wp43MemInBlocks), (uint64_t)gmpMemInBytes, getFreeRamMemory());
    printf("    Addr   Size\n");
    for(int i=0; i<numberOfFreeMemoryRegions; i++) {
      printf("%2d%6u%7u\n", i, freeMemoryRegions[i].address, freeMemoryRegions[i].sizeInBlocks);
    }
    printf("\n");
  }
#endif // !DMCP_BUILD

bool_t isMemoryBlockAvailable(size_t sizeInBlocks) {
  int i;

  for(i=0; i<numberOfFreeMemoryRegions; i++) {
    if(freeMemoryRegions[i].sizeInBlocks >= sizeInBlocks) {
      return true;
    }
  }

  return false;
}





void *allocWp43(size_t sizeInBlocks) {
  #if !defined(DMCP_BUILD)
    //if(debugMemAllocation) {
    //  printf("allocWp43\n");
    //}
  #endif // !DMCP_BUILD

  //wp43MemInBlocks += sizeInBlocks;

  #if !defined(DMCP_BUILD)
    //if(debugMemAllocation) {
    //  printf("WP43 claims %6" PRIu64 " blocks\n", sizeInBlocks);
    //}
  #endif // !DMCP_BUILD
  //return freeListAlloc(sizeInBlocks);

  void *allocated = freeListAlloc(sizeInBlocks);
  if(allocated) {
    wp43MemInBlocks += sizeInBlocks;
    return allocated;
  }
  else {
    return NULL;
  }
}

void *reallocWp43(void *pcMemPtr, size_t oldSizeInBlocks, size_t newSizeInBlocks) {
  #if !defined(DMCP_BUILD)
    //if(debugMemAllocation) {
    //  printf("reallocWp43\n");
    //}
  #endif // !DMCP_BUILD

  //wp43MemInBlocks += newSizeInBlocks - oldSizeInBlocks;

  #if !defined(DMCP_BUILD)
    //if(debugMemAllocation) {
    //  printf("WP43 claimed %6" PRIu64 " blocks, freed %6" PRIu64 " blocks and holds now %6" PRIu64 " blocks\n", (uint64_t)newSizeInBlocks, (uint64_t)oldSizeInBlocks, (uint64_t)wp43MemInBlocks);
    //}
  #endif // !DMCP_BUILD
  //return freeListRealloc(pcMemPtr, oldSizeInBlocks, newSizeInBlocks);

  void *allocated = freeListRealloc(pcMemPtr, oldSizeInBlocks, newSizeInBlocks);
  if(allocated) {
    wp43MemInBlocks += newSizeInBlocks - oldSizeInBlocks;
    return allocated;
  }
  else {
    return NULL;
  }
}

void freeWp43(void *pcMemPtr, size_t sizeInBlocks) {
  if(pcMemPtr == NULL) {
    return;
  }

  #if !defined(DMCP_BUILD)
    //if(debugMemAllocation) {
    //  printf("freeWp43\n");
    //}
  #endif // !DMCP_BUILD

  wp43MemInBlocks -= sizeInBlocks;

  #if !defined(DMCP_BUILD)
    //if(debugMemAllocation) {
    //  printf("WP43 frees  %6" PRIu64 " blocks\n", sizeInBlocks);
    //}
  #endif // !DMCP_BUILD
  freeListFree(pcMemPtr, sizeInBlocks);
}





void *allocGmp(size_t sizeInBytes) {
  #if !defined(DMCP_BUILD)
    //if(debugMemAllocation) {
    //  printf("allocGmp\n");
    //}
  #endif // !DMCP_BUILD

  sizeInBytes = TO_BYTES(TO_BLOCKS(sizeInBytes));
  gmpMemInBytes += sizeInBytes;

  #if !defined(DMCP_BUILD)
    //if(debugMemAllocation) {
    //  printf("GMP claimed %6" PRIu64 " bytes and holds now %6" PRIu64 " bytes\n", (uint64_t)sizeInBytes, (uint64_t)gmpMemInBytes);
    //}
  #endif // !DMCP_BUILD
  //return freeListAlloc(TO_BLOCKS(sizeInBytes));
  return malloc(sizeInBytes);
}

void *reallocGmp(void *pcMemPtr, size_t oldSizeInBytes, size_t newSizeInBytes) {
  #if !defined(DMCP_BUILD)
    //if(debugMemAllocation) {
    //  printf("reallocGmp\n");
    //}
  #endif // !DMCP_BUILD

  newSizeInBytes = TO_BYTES(TO_BLOCKS(newSizeInBytes));
  oldSizeInBytes = TO_BYTES(TO_BLOCKS(oldSizeInBytes));

  gmpMemInBytes += newSizeInBytes - oldSizeInBytes;

  #if !defined(DMCP_BUILD)
    //if(debugMemAllocation) {
    //  printf("GMP claimed %6" PRIu64 " bytes, freed %6" PRIu64 " bytes and holds now %6" PRIu64 " bytes\n", (uint64_t)newSizeInBytes, (uint64_t)oldSizeInBytes, (uint64_t)gmpMemInBytes);
    //}
  #endif // !DMCP_BUILD
  //return freeListRealloc(pcMemPtr, TO_BLOCKS(oldSizeInBytes), TO_BLOCKS(newSizeInBytes));
  return realloc(pcMemPtr, newSizeInBytes);
}

void freeGmp(void *pcMemPtr, size_t sizeInBytes) {
  #if !defined(DMCP_BUILD)
    //if(debugMemAllocation) {
    //  printf("freeGmp\n");
    //}
  #endif // !DMCP_BUILD

  sizeInBytes = TO_BYTES(TO_BLOCKS(sizeInBytes));
  gmpMemInBytes -= sizeInBytes;

  #if !defined(DMCP_BUILD)
    //if(debugMemAllocation) {
    //  printf("GMP freed   %6" PRIu64 " bytes and holds now %6" PRIu64 " bytes\n", (uint64_t)sizeInBytes, (uint64_t)gmpMemInBytes);
    //}
  #endif // !DMCP_BUILD
  //freeListFree(pcMemPtr, TO_BLOCKS(sizeInBytes));
  free(pcMemPtr);
}



void resizeProgramMemory(uint16_t newSizeInBlocks) {
  uint16_t currentSizeInBlocks = RAM_SIZE - freeMemoryRegions[numberOfFreeMemoryRegions - 1].address - freeMemoryRegions[numberOfFreeMemoryRegions - 1].sizeInBlocks;
  uint16_t deltaBlocks, blocksToMove = 0;
  uint8_t *newProgramMemoryPointer = NULL;

  #if !defined(DMCP_BUILD)
    //printf("currentSizeInBlocks = %u    newSizeInBlocks = %u\n", currentSizeInBlocks, newSizeInBlocks);
    //printf("currentAddress      = %u\n", TO_WP43MEMPTR(beginOfProgramMemory));
  #endif // !DMCP_BUILD
  if(newSizeInBlocks == currentSizeInBlocks) { // Nothing to do
    return;
  }

  if(newSizeInBlocks > currentSizeInBlocks) { // Increase program memory size
    deltaBlocks = newSizeInBlocks - currentSizeInBlocks;
    if(newSizeInBlocks - currentSizeInBlocks > freeMemoryRegions[numberOfFreeMemoryRegions - 1].sizeInBlocks) { // Out of memory
      #if defined(DMCP_BUILD)
        backToSystem(NOPARAM);
      #else // !DMCP_BUILD
        int32_t freeMemory = 0;
        for(int32_t i=0; i<numberOfFreeMemoryRegions; i++) {
          freeMemory += freeMemoryRegions[i].sizeInBlocks;
        }
        printf("\nOUT OF MEMORY\nMemory claimed: %" PRIu64 " bytes\nFragmented free memory: %" PRIu64 " bytes\n", (uint64_t)TO_BYTES(deltaBlocks), (uint64_t)TO_BYTES(freeMemory));
        exit(-3);
      #endif // DMCP_BUILD
    }
    else { // There is plenty of memory available
      blocksToMove = currentSizeInBlocks;
      newProgramMemoryPointer = beginOfProgramMemory - TO_BYTES(deltaBlocks);
      firstFreeProgramByte -= TO_BYTES(deltaBlocks);
      #if !defined(DMCP_BUILD)
        //printf("Increasing program memory by copying %u blocks from %u to %u\n", currentSizeInBlocks, TO_WP43MEMPTR(beginOfProgramMemory), TO_WP43MEMPTR(newProgramMemoryPointer));
      #endif // !DMCP_BUILD
      freeMemoryRegions[numberOfFreeMemoryRegions - 1].sizeInBlocks -= deltaBlocks;
    }
  }
  else { // Decrease program memory size
    deltaBlocks = currentSizeInBlocks - newSizeInBlocks;
    blocksToMove = newSizeInBlocks;
    newProgramMemoryPointer = beginOfProgramMemory + TO_BYTES(deltaBlocks);
    firstFreeProgramByte += TO_BYTES(deltaBlocks);
    #if !defined(DMCP_BUILD)
      //printf("Decreasing program memory by copying %u blocks from %u to %u\n", newSizeInBlocks, TO_WP43MEMPTR(beginOfProgramMemory), TO_WP43MEMPTR(newProgramMemoryPointer));
    #endif // !DMCP_BUILD
    freeMemoryRegions[numberOfFreeMemoryRegions - 1].sizeInBlocks += deltaBlocks;
  }

  xcopy(newProgramMemoryPointer, beginOfProgramMemory, TO_BYTES(blocksToMove));
  beginOfProgramMemory = newProgramMemoryPointer;
  //debugMemory("resizeProgramMemory : end");
}


#if defined(PC_BUILD)
  static void findBlockUsage(uint16_t block) {
    tmpString[0] = 0;

    for(int i=0; i<NUMBER_OF_RESERVED_VARIABLES; i++) {
      if(allReservedVariables[i].header.pointerToRegisterData == block) {
        int j;
        for(j=1; j<=*(allReservedVariables[i].reservedVariableName); j++) {
          tmpString[99 + j] = allReservedVariables[i].reservedVariableName[j];
        }
        tmpString[99 + j] = 0;
        stringToUtf8(tmpString + 100, (uint8_t *)errorMessage);
        sprintf(tmpString, "Reserved variable %s data: %s (%u)", errorMessage, getDataTypeName(allReservedVariables[i].header.dataType, false, false), getRegisterFullSize(i + FIRST_RESERVED_VARIABLE));
        return;
      }
    }

    for(int i=0; i<=LAST_GLOBAL_REGISTER; i++) {
      if(globalRegister[i].pointerToRegisterData == block) {
        sprintf(tmpString, "Global register %02d data: %s (%u)", i, getDataTypeName(globalRegister[i].dataType, false, false), getRegisterFullSize(i));
        return;
      }
    }

    for(int i=0; i<NUMBER_OF_SAVED_STACK_REGISTERS; i++) {
      if(savedStackRegister[i].pointerToRegisterData == block) {
        sprintf(tmpString, "Saved stack register %d data: %s (%u)", i + FIRST_SAVED_STACK_REGISTER, getDataTypeName(savedStackRegister[i].dataType, false, false), getRegisterFullSize(i + FIRST_SAVED_STACK_REGISTER));
        return;
      }
    }

    for(int i=NUMBER_OF_SAVED_STACK_REGISTERS; i<NUMBER_OF_SAVED_STACK_REGISTERS + NUMBER_OF_TEMP_REGISTERS; i++) {
      if(savedStackRegister[i].pointerToRegisterData == block) {
        sprintf(tmpString, "Temporary register %d data: %s (%u)", i + FIRST_SAVED_STACK_REGISTER, getDataTypeName(savedStackRegister[i].dataType, false, false), getRegisterFullSize(i + FIRST_SAVED_STACK_REGISTER));
        return;
      }
    }

    for(int i=0; i<numberOfNamedVariables; i++) {
      if(TO_WP43MEMPTR(allNamedVariables + i) == block) {
        int j;
        for(j=1; j<=*(allNamedVariables[i].variableName); j++) {
          tmpString[99 + j] = allNamedVariables[i].variableName[j];
        }
        tmpString[99 + j] = 0;
        stringToUtf8(tmpString + 100, (uint8_t *)errorMessage);
        sprintf(tmpString, "Named variable %d header: %s %s (%u)", i, errorMessage, getDataTypeName(allNamedVariables[i].header.dataType, false, false), getRegisterFullSize(i + FIRST_NAMED_VARIABLE));
        return;
      }

      if(allNamedVariables[i].header.pointerToRegisterData == block) {
        int j;
        for(j=1; j<=*(allNamedVariables[i].variableName); j++) {
          tmpString[99 + j] = allNamedVariables[i].variableName[j];
        }
        tmpString[99 + j] = 0;
        stringToUtf8(tmpString + 100, (uint8_t *)errorMessage);
        sprintf(tmpString, "Named variable %d data: %s %s (%u)", i, errorMessage, getDataTypeName(allNamedVariables[i].header.dataType, false, false), getRegisterFullSize(i + FIRST_NAMED_VARIABLE));
        return;
      }
    }

    dataBlock_t *savedCurrerntSubroutineLevelData = currentSubroutineLevelData;
    currentSubroutineLevelData = TO_PCMEMPTR(allSubroutineLevels.ptrToSubroutineLevel0Data);

    while(currentSubroutineLevelData) {
      if(TO_WP43MEMPTR(currentSubroutineLevelData) == block) {
        sprintf(tmpString, "Subroutine level %u data (3)", currentSubroutineLevel);
        currentSubroutineLevelData = savedCurrerntSubroutineLevelData;
        return;
      }

      if(currentNumberOfLocalFlags != 0 && TO_WP43MEMPTR(currentSubroutineLevelData + 3) == block) {
        sprintf(tmpString, "Subroutine level %u local flags (1)", currentSubroutineLevel);
        currentSubroutineLevelData = savedCurrerntSubroutineLevelData;
        return;
      }

      for(int i=0; i<currentNumberOfLocalRegisters; i++) {
        if(TO_WP43MEMPTR(currentSubroutineLevelData + 4 + i) == block) {
          sprintf(tmpString, "Subroutine level %u local register .%02d header (1)", currentSubroutineLevel, i);
          currentSubroutineLevelData = savedCurrerntSubroutineLevelData;
          return;
        }

        if(((registerHeader_t *)(currentSubroutineLevelData + 4 + i))->pointerToRegisterData == block) {
          sprintf(tmpString, "Subroutine level %u local register .%02d data: %s", currentSubroutineLevel, i, getDataTypeName(((registerHeader_t *)(currentSubroutineLevelData + 4 + i))->dataType, false, false));
          currentSubroutineLevelData = savedCurrerntSubroutineLevelData;
          return;
        }
      }

      currentSubroutineLevelData = TO_PCMEMPTR(currentPtrToNextLevel);
    }

    currentSubroutineLevelData = savedCurrerntSubroutineLevelData;

    if(TO_WP43MEMPTR(beginOfProgramMemory) == block) {
      sprintf(tmpString, "Begin of program memory");
      return;
    }

    if(TO_WP43MEMPTR(allNamedVariables) == block) {
      sprintf(tmpString, "All named variables");
      return;
    }

    if(statisticalSumsPointer != NULL) {
      if(TO_WP43MEMPTR(SIGMA_N) == block) {
        sprintf(tmpString, "real75 n data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_X) == block) {
        sprintf(tmpString, "real75 Σx data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_Y) == block) {
        sprintf(tmpString, "real75 Σy data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_Y) == block) {
        sprintf(tmpString, "real75 Σy data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_X2) == block) {
        sprintf(tmpString, "real75 Σx² data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_X2Y) == block) {
        sprintf(tmpString, "real75 Σx²y data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_Y2) == block) {
        sprintf(tmpString, "real75 Σy² data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_XY) == block) {
        sprintf(tmpString, "real75 Σxy data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_lnXlnY) == block) {
        sprintf(tmpString, "real75 Σlnxlny data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_lnX) == block) {
        sprintf(tmpString, "real75 Σlnx data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_ln2X) == block) {
        sprintf(tmpString, "real75 Σln²x data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_YlnX) == block) {
        sprintf(tmpString, "real75 Σylnx data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_lnY) == block) {
        sprintf(tmpString, "real75 Σlny data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_ln2Y) == block) {
        sprintf(tmpString, "real75 Σln²y data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_XlnY) == block) {
        sprintf(tmpString, "real75 Σxlny data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_X2lnY) == block) {
        sprintf(tmpString, "real75 Σx²lny data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_lnYonX) == block) {
        sprintf(tmpString, "real75 Σlny/x data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_X2onY) == block) {
        sprintf(tmpString, "real75 Σx²/y data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_1onX) == block) {
        sprintf(tmpString, "real75 Σ1/x data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_1onX2) == block) {
        sprintf(tmpString, "real75 Σ1/x² data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_XonY) == block) {
        sprintf(tmpString, "real75 Σx/y data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_1onY) == block) {
        sprintf(tmpString, "real75 Σ1/y data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_1onY2) == block) {
        sprintf(tmpString, "real75 Σ1/y² data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_X3) == block) {
        sprintf(tmpString, "real75 Σx³ data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_X4) == block) {
        sprintf(tmpString, "real75 Σx⁴ data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_XMIN) == block) {
        sprintf(tmpString, "real75 xmin data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_XMAX) == block) {
        sprintf(tmpString, "real75 xmax data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_YMIN) == block) {
        sprintf(tmpString, "real75 ymin data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_YMAX) == block) {
        sprintf(tmpString, "real75 ymax data");
        return;
      }
    }

    if(savedStatisticalSumsPointer != NULL) {
      int32_t shift = (savedStatisticalSumsPointer - statisticalSumsPointer);
      if(TO_WP43MEMPTR(SIGMA_N) + shift == block) {
        sprintf(tmpString, "real75 saved for undo n data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_X) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σx data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_Y) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σy data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_Y) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σy data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_X2) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σx² data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_X2Y) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σx²y data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_Y2) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σy² data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_XY) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σxy data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_lnXlnY) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σlnxlny data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_lnX) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σlnx data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_ln2X) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σln²x data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_YlnX) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σylnx data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_lnY) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σlny data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_ln2Y) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σln²y data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_XlnY) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σxlny data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_X2lnY) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σx²lny data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_lnYonX) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σlny/x data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_X2onY) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σx²/y data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_1onX) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σ1/x data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_1onX2) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σ1/x² data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_XonY) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σx/y data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_1onY) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σ1/y data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_1onY2) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σ1/y² data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_X3) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σx³ data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_X4) + shift == block) {
        sprintf(tmpString, "real75 saved for undo Σx⁴ data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_XMIN) + shift == block) {
        sprintf(tmpString, "real75 saved for undo xmin data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_XMAX) + shift == block) {
        sprintf(tmpString, "real75 saved for undo xmax data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_YMIN) + shift == block) {
        sprintf(tmpString, "real75 saved for undo ymin data");
        return;
      }

      if(TO_WP43MEMPTR(SIGMA_YMAX) + shift == block) {
        sprintf(tmpString, "real75 saved for undo ymax data");
        return;
      }
    }

    for(int i=0; i<numberOfFreeMemoryRegions; i++) {
      if(freeMemoryRegions[i].address == block && freeMemoryRegions[i].sizeInBlocks == 1) {
        sprintf(tmpString, "Free memory block %d: (1)", i);
        return;
      }

      if(freeMemoryRegions[i].address == block) {
        sprintf(tmpString, "Free memory block %d begin: (%u)", i, freeMemoryRegions[i].sizeInBlocks);
        return;
      }

      if(freeMemoryRegions[i].address + freeMemoryRegions[i].sizeInBlocks - 1 == block) {
        sprintf(tmpString, "Free memory block %d end: (%u)", i, freeMemoryRegions[i].sizeInBlocks);
        return;
      }
    }
  }

  void ramDump(void) {
    for(calcRegister_t regist=0; regist<FIRST_LOCAL_REGISTER; regist++) {
      printf("Global register    %3u: dataPointer=(block %5u)     dataType=%2u=%s       tag=%2u=%s\n",
                                 regist,                 globalRegister[regist].pointerToRegisterData,
                                                                           globalRegister[regist].dataType,
                                                                               getDataTypeName(globalRegister[regist].dataType, false, true),
                                                                                            globalRegister[regist].tag,
                                                                                                getRegisterTagName(regist, true));
    }

    for(calcRegister_t regist=0; regist<NUMBER_OF_SAVED_STACK_REGISTERS + NUMBER_OF_TEMP_REGISTERS; regist++) {
      printf("SavStk register   %4u: dataPointer=(block %5u)     dataType=%2u=%s       tag=%2u=%s\n",
                                regist + FIRST_SAVED_STACK_REGISTER,
                                                        savedStackRegister[regist].pointerToRegisterData,
                                                                          savedStackRegister[regist].dataType,
                                                                              getDataTypeName(savedStackRegister[regist].dataType, false, true),
                                                                                           savedStackRegister[regist].tag,
                                                                                               getRegisterTagName(regist + FIRST_SAVED_STACK_REGISTER, true));
    }

    printf("\nallSubroutineLevels: numberOfSubroutineLevels=%u  ptrToSubroutineLevel0Data=%u\n", allSubroutineLevels.numberOfSubroutineLevels, allSubroutineLevels.ptrToSubroutineLevel0Data);
    printf("  Level rtnPgm rtnStep nbrLocalFlags nbrLocRegs level     next previous\n");
    currentSubroutineLevelData = TO_PCMEMPTR(allSubroutineLevels.ptrToSubroutineLevel0Data);
    currentLocalFlags = (currentNumberOfLocalFlags == 0 ? NULL : currentSubroutineLevelData + 3);
    currentLocalRegisters = (registerHeader_t *)(currentNumberOfLocalRegisters == 0 ? NULL : currentSubroutineLevelData + (currentLocalFlags == NULL ? 3 : 4));
    for(int level=0; level<allSubroutineLevels.numberOfSubroutineLevels; level++) {
      printf("  %5d %6d %7u %13u %10u %5u %8u %8u\n",
                level,
                    currentReturnProgramNumber,
                        currentReturnLocalStep,
                            currentNumberOfLocalFlags,
                                 currentNumberOfLocalRegisters,
                                      currentSubroutineLevel,
                                          currentPtrToNextLevel,
                                              currentPtrToPreviousLevel);

      for(calcRegister_t regist=0; regist<currentNumberOfLocalRegisters; regist++) {
        printf("        Local register     .%2u: dataPointer=(block %5u)       dataType=%2u=%s           tag=%2u=%s\n",
                                            regist + FIRST_LOCAL_REGISTER,
                                                                    currentLocalRegisters[regist].pointerToRegisterData,
                                                                                        currentLocalRegisters[regist].dataType,
                                                                                            getDataTypeName(currentLocalRegisters[regist].dataType, false, true),
                                                                                                             currentLocalRegisters[regist].tag,
                                                                                                                 getRegisterTagName(regist + FIRST_LOCAL_REGISTER, true));
      }

      if(currentPtrToNextLevel != WP43_NULL) {
        currentSubroutineLevelData = TO_PCMEMPTR(currentPtrToNextLevel);
        currentLocalFlags = (currentNumberOfLocalFlags == 0 ? NULL : currentSubroutineLevelData + 3);
        currentLocalRegisters = (registerHeader_t *)(currentNumberOfLocalRegisters == 0 ? NULL : currentSubroutineLevelData + (currentLocalFlags == NULL ? 3 : 4));
      }
    }

    fprintf(stdout, "\n| block | hex               dec | hec      dec | hex  dec |\n");
    for(uint16_t block=0; block<RAM_SIZE; block++) {
      fprintf(stdout, "+-------+-----------------------+--------------+----------+\n");
      fprintf(stdout, "| %5u | %08x = %10u | %04x = %5u | %02x = %3u |  ", block, *(uint32_t *)(ram + block), *(uint32_t *)(ram + block), *(uint16_t *)(ram + block), *(uint16_t *)(ram + block), *(uint8_t *)(ram + block), *(uint8_t *)(ram + block));
      findBlockUsage(block);
      fprintf(stdout, "%s\n", tmpString);
      //fprintf(stdout, "|       |                       |              +----------+\n");
      fprintf(stdout, "|       |                       |              | %02x = %3u |\n", *((uint8_t *)(ram + block) + 1), *((uint8_t *)(ram + block) + 1));
      //fprintf(stdout, "|       |                       +--------------+----------+\n");
      fprintf(stdout, "|       |                       | %04x = %5u | %02x = %3u |\n", *((uint16_t *)(ram + block) + 1), *((uint16_t *)(ram + block) + 1), *((uint8_t *)(ram + block) + 2), *((uint8_t *)(ram + block) + 2));
      //fprintf(stdout, "|       |                       |              +----------+\n");
      fprintf(stdout, "|       |                       |              | %02x = %3u |\n", *((uint8_t *)(ram + block) + 3), *((uint8_t *)(ram + block) + 3));
    }
    fprintf(stdout, "+-------+-----------------------+--------------+----------+\n");
  }
#endif // PC_BUILD
