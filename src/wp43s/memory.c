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
 * \file memory.c
 ***********************************************/

#include "wp43s.h"

int32_t getFreeRamMemory(void) {
  int32_t freeMem, i;

  freeMem = 0;
  for(i=0; i<numberOfFreeMemoryRegions; i++) {
    freeMem += freeMemoryRegions[i].sizeInBlocks;
  }

  return TO_BYTES(freeMem);
}

#ifndef DMCP_BUILD
  void debugMemory(const char *message) {
    printf("\n%s\nWP43S owns %6" PRIu64 " bytes and GMP owns %6" PRIu64 " bytes (%" PRId32 " bytes free)\n", message, (uint64_t)wp43sMemInBytes, (uint64_t)gmpMemInBytes, getFreeRamMemory());
    printf("    Addr   Size\n");
    for(int i=0; i<numberOfFreeMemoryRegions; i++) {
      printf("%2d%6u%7u\n", i, freeMemoryRegions[i].address, freeMemoryRegions[i].sizeInBlocks);
    }
    printf("\n");
  }
#endif // !DMCP_BUILD





void *allocWp43s(size_t sizeInBytes) {
  #ifndef DMCP_BUILD
    //if(debugMemAllocation) printf("allocWp43s\n");
  #endif // !DMCP_BUILD

  sizeInBytes = TO_BYTES(TO_BLOCKS(sizeInBytes));
  wp43sMemInBytes += sizeInBytes;

  #ifndef DMCP_BUILD
    //if(debugMemAllocation) printf("WP43S claims %6" PRIu64 " bytes\n", sizeInBytes);
  #endif // !DMCP_BUILD
  return wp43sAllocate(sizeInBytes);
}

void *reallocWp43s(void *pcMemPtr, size_t oldSizeInBytes, size_t newSizeInBytes) {
  #ifndef DMCP_BUILD
    //if(debugMemAllocation) printf("reallocWp43s\n");
  #endif // !DMCP_BUILD

  newSizeInBytes = TO_BYTES(TO_BLOCKS(newSizeInBytes));
  oldSizeInBytes = TO_BYTES(TO_BLOCKS(oldSizeInBytes));

  wp43sMemInBytes += newSizeInBytes - oldSizeInBytes;

  #ifndef DMCP_BUILD
    //if(debugMemAllocation) printf("WP43S claimed %6" PRIu64 " bytes, freed %6" PRIu64 " bytes and holds now %6" PRIu64 " bytes\n", (uint64_t)newSizeInBytes, (uint64_t)oldSizeInBytes, (uint64_t)wp43sMemInBytes);
  #endif // !DMCP_BUILD
  return wp43sReallocate(pcMemPtr, oldSizeInBytes, newSizeInBytes);
}

void freeWp43s(void *pcMemPtr, size_t sizeInBytes) {
  #ifndef DMCP_BUILD
    //if(debugMemAllocation) printf("freeWp43s\n");
  #endif // !DMCP_BUILD

  sizeInBytes = TO_BYTES(TO_BLOCKS(sizeInBytes));
  wp43sMemInBytes -= sizeInBytes;

  #ifndef DMCP_BUILD
    //if(debugMemAllocation) printf("WP43S frees  %6" PRIu64 " bytes\n", sizeInBytes);
  #endif // !DMCP_BUILD
  wp43sFree(pcMemPtr, sizeInBytes);
}





void *allocGmp(size_t sizeInBytes) {
  #ifndef DMCP_BUILD
    //if(debugMemAllocation) printf("allocGmp\n");
  #endif // !DMCP_BUILD

  sizeInBytes = TO_BYTES(TO_BLOCKS(sizeInBytes));
  gmpMemInBytes += sizeInBytes;

  #ifndef DMCP_BUILD
    //if(debugMemAllocation) printf("GMP claimed %6" PRIu64 " bytes and holds now %6" PRIu64 " bytes\n", (uint64_t)sizeInBytes, (uint64_t)gmpMemInBytes);
  #endif // !DMCP_BUILD
  //return wp43sAllocate(sizeInBytes);
  return malloc(sizeInBytes);
}

void *reallocGmp(void *pcMemPtr, size_t oldSizeInBytes, size_t newSizeInBytes) {
  #ifndef DMCP_BUILD
    //if(debugMemAllocation) printf("reallocGmp\n");
  #endif // !DMCP_BUILD

  newSizeInBytes = TO_BYTES(TO_BLOCKS(newSizeInBytes));
  oldSizeInBytes = TO_BYTES(TO_BLOCKS(oldSizeInBytes));

  gmpMemInBytes += newSizeInBytes - oldSizeInBytes;

  #ifndef DMCP_BUILD
    //if(debugMemAllocation) printf("GMP claimed %6" PRIu64 " bytes, freed %6" PRIu64 " bytes and holds now %6" PRIu64 " bytes\n", (uint64_t)newSizeInBytes, (uint64_t)oldSizeInBytes, (uint64_t)gmpMemInBytes);
  #endif // !DMCP_BUILD
  //return wp43sReallocate(pcMemPtr, oldSizeInBytes, newSizeInBytes);
  return realloc(pcMemPtr, newSizeInBytes);
}

void freeGmp(void *pcMemPtr, size_t sizeInBytes) {
  #ifndef DMCP_BUILD
    //if(debugMemAllocation) printf("freeGmp\n");
  #endif // !DMCP_BUILD

  sizeInBytes = TO_BYTES(TO_BLOCKS(sizeInBytes));
  gmpMemInBytes -= sizeInBytes;

  #ifndef DMCP_BUILD
    //if(debugMemAllocation) printf("GMP freed   %6" PRIu64 " bytes and holds now %6" PRIu64 " bytes\n", (uint64_t)sizeInBytes, (uint64_t)gmpMemInBytes);
  #endif // !DMCP_BUILD
  //wp43sFree(pcMemPtr, sizeInBytes);
  free(pcMemPtr);
}





void *wp43sAllocate(size_t sizeInBytes) {
  uint16_t sizeInBlocks = (sizeInBytes == 0 ? 1 : TO_BLOCKS(sizeInBytes));
  uint16_t minSizeInBlocks = 65535u, minBlock = WP43S_NULL;
  int i;
  void *pcMemPtr;

  #ifndef DMCP_BUILD
    //if(debugMemAllocation) printf("Allocating %" PRIu64 " bytes (%" PRIu16 " blocks)\n", (uint64_t)TO_BYTES(sizeInBlocks), sizeInBlocks);
  #endif // !DMCP_BUILD

  // Search the smalest hole where the claimed block fits
  //debugMemory();
  for(i=0; i<numberOfFreeMemoryRegions; i++) {
    if(freeMemoryRegions[i].sizeInBlocks == sizeInBlocks) {
      #ifndef DMCP_BUILD
        //if(debugMemAllocation) printf("The block found is the size of the one claimed at address %u\n", freeMemoryRegions[i].address);
      #endif // !DMCP_BUILD
      pcMemPtr = TO_PCMEMPTR(freeMemoryRegions[i].address);
      xcopy(freeMemoryRegions + i, freeMemoryRegions + i + 1, (numberOfFreeMemoryRegions-i-1) * sizeof(freeMemoryRegion_t));
      numberOfFreeMemoryRegions--;
      //debugMemory("wp43sAllocate: found a memory region with the exact requested size!");
      return pcMemPtr;
    }
    else if(freeMemoryRegions[i].sizeInBlocks > sizeInBlocks && freeMemoryRegions[i].sizeInBlocks < minSizeInBlocks) {
      minSizeInBlocks = freeMemoryRegions[i].sizeInBlocks;
      minBlock = i;
    }
  }

  if(minBlock == WP43S_NULL) {
    #ifdef DMCP_BUILD
      backToSystem(NOPARAM);
    #else // !DMCP_BUILD
      minSizeInBlocks = 0;
      for(i=0; i<numberOfFreeMemoryRegions; i++) {
        minSizeInBlocks += freeMemoryRegions[i].sizeInBlocks;
      }
      printf("\nOUT OF MEMORY\nMemory claimed: %" PRIu64 " bytes\nFragmented free memory: %u bytes\n", (uint64_t)TO_BYTES(sizeInBlocks), TO_BYTES(minSizeInBlocks));
      exit(-3);
    #endif // DMCP_BUILD
  }

  #ifndef DMCP_BUILD
    //if(debugMemAllocation) printf("The block found is larger than the one claimed\n");
  #endif // !DMCP_BUILD
  pcMemPtr = TO_PCMEMPTR(freeMemoryRegions[minBlock].address);
  freeMemoryRegions[minBlock].address += sizeInBlocks;
  freeMemoryRegions[minBlock].sizeInBlocks -= sizeInBlocks;

  //debugMemory("wp43sAllocate: allocated within the smalest memory region found that is large enough.");
  return pcMemPtr;
}

void *wp43sReallocate(void *pcMemPtr, size_t oldSizeInBytes, size_t newSizeInBytes) {
  void *newMemPtr;

  // GMP never calls realloc with pcMemPtr beeing NULL
  if(oldSizeInBytes == 0) {
    oldSizeInBytes = 1;
  }

  if(newSizeInBytes == 0) {
    newSizeInBytes = 1;
  }

  newSizeInBytes = TO_BYTES(TO_BLOCKS(newSizeInBytes));
  oldSizeInBytes = TO_BYTES(TO_BLOCKS(oldSizeInBytes));
  #ifndef DMCP_BUILD
    //printf("Allocating %zd bytes and freeing %zd bytes\n", newSizeInBytes, oldSizeInBytes);
  #endif // !DMCP_BUILD

  newMemPtr = wp43sAllocate(newSizeInBytes);
  xcopy(newMemPtr, pcMemPtr, min(newSizeInBytes, oldSizeInBytes));
  wp43sFree(pcMemPtr, oldSizeInBytes);

  return newMemPtr;
}

void wp43sFree(void *pcMemPtr, size_t sizeInBytes) {
  uint16_t ramPtr, sizeInBlocks, addr;
  int32_t i, j;
  bool_t done;

  // GMP never calls free with pcMemPtr beeing NULL
  if(pcMemPtr == NULL) {
    return;
  }

  if(sizeInBytes == 0) {
    sizeInBytes = 1;
  }
  ramPtr = TO_WP43SMEMPTR(pcMemPtr);
  sizeInBlocks = TO_BLOCKS(sizeInBytes);
  #ifndef DMCP_BUILD
    //printf("Freeing %zd bytes\n", TO_BYTES(sizeInBlocks));
  #endif // !DMCP_BUILD

  done = false;

  // is the freed block just before an other free block?
  addr = ramPtr + sizeInBlocks;
  for(i=0; i<numberOfFreeMemoryRegions && freeMemoryRegions[i].address<=addr; i++) {
    if(freeMemoryRegions[i].address == addr) {
      freeMemoryRegions[i].address = ramPtr;
      freeMemoryRegions[i].sizeInBlocks += sizeInBlocks;
      sizeInBlocks = freeMemoryRegions[i].sizeInBlocks;
      j = i;
      done = true;
      break;
    }
  }

  // is the freed block just after an other free block?
  for(i=0; i<numberOfFreeMemoryRegions && freeMemoryRegions[i].address+freeMemoryRegions[i].sizeInBlocks<=ramPtr; i++) {
    if(freeMemoryRegions[i].address + freeMemoryRegions[i].sizeInBlocks == ramPtr) {
      freeMemoryRegions[i].sizeInBlocks += sizeInBlocks;
      if(done) {
        xcopy(freeMemoryRegions + j, freeMemoryRegions + j + 1, (numberOfFreeMemoryRegions-j-1) * sizeof(freeMemoryRegion_t));
        numberOfFreeMemoryRegions--;
      }
      else {
        done = true;
      }
      break;
    }
  }

  // new free block
  if(!done) {
    if(numberOfFreeMemoryRegions == MAX_FREE_REGION) {
      #ifdef DMCP_BUILD
        backToSystem(NOPARAM);
      #else // !DMCP_BUILD
        printf("\n**********************************************************************\n");
        printf("* The maximum number of free memory blocks has been exceeded!        *\n");
        printf("* This number must be increased or the compaction function improved. *\n");
        printf("**********************************************************************\n");
        exit(-2);
      #endif // DMCP_BUILD
    }

    i = 0;
    while(i<numberOfFreeMemoryRegions && freeMemoryRegions[i].address < ramPtr) {
      i++;
    }

    if(i < numberOfFreeMemoryRegions) {
      xcopy(freeMemoryRegions + i + 1, freeMemoryRegions + i, (numberOfFreeMemoryRegions-i) * sizeof(freeMemoryRegion_t));
    }

    freeMemoryRegions[i].address = ramPtr;
    freeMemoryRegions[i].sizeInBlocks = sizeInBlocks;
    numberOfFreeMemoryRegions++;
  }

  //debugMemory("wp43sFree : end");
}


void resizeProgramMemory(uint16_t newSizeInBlocks) {
  uint16_t currentSizeInBlocks = RAM_SIZE - freeMemoryRegions[numberOfFreeMemoryRegions - 1].address - freeMemoryRegions[numberOfFreeMemoryRegions - 1].sizeInBlocks;
  uint16_t deltaBlocks, blocksToMove = 0;
  uint8_t *newProgramMemoryPointer = NULL;

  #ifndef DMCP_BUILD
    //printf("currentSizeInBlocks = %u    newSizeInBlocks = %u\n", currentSizeInBlocks, newSizeInBlocks);
    //printf("currentAddress      = %u\n", TO_WP43SMEMPTR(beginOfProgramMemory));
  #endif // !DMCP_BUILD
  if(newSizeInBlocks == currentSizeInBlocks) { // Nothing to do
    return;
  }

  if(newSizeInBlocks > currentSizeInBlocks) { // Increase program memory size
    deltaBlocks = newSizeInBlocks - currentSizeInBlocks;
    if(newSizeInBlocks - currentSizeInBlocks > freeMemoryRegions[numberOfFreeMemoryRegions - 1].sizeInBlocks) { // Out of memory
      #ifdef DMCP_BUILD
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
      #ifndef DMCP_BUILD
        //printf("Increasing program memory by copying %u blocks from %u to %u\n", currentSizeInBlocks, TO_WP43SMEMPTR(beginOfProgramMemory), TO_WP43SMEMPTR(newProgramMemoryPointer));
      #endif // !DMCP_BUILD
      freeMemoryRegions[numberOfFreeMemoryRegions - 1].sizeInBlocks -= deltaBlocks;
    }
  }
  else { // Decrease program memory size
    deltaBlocks = currentSizeInBlocks - newSizeInBlocks;
    blocksToMove = newSizeInBlocks;
    newProgramMemoryPointer = beginOfProgramMemory + TO_BYTES(deltaBlocks);
    firstFreeProgramByte += TO_BYTES(deltaBlocks);
    #ifndef DMCP_BUILD
      //printf("Decreasing program memory by copying %u blocks from %u to %u\n", newSizeInBlocks, TO_WP43SMEMPTR(beginOfProgramMemory), TO_WP43SMEMPTR(newProgramMemoryPointer));
    #endif // !DMCP_BUILD
    freeMemoryRegions[numberOfFreeMemoryRegions - 1].sizeInBlocks += deltaBlocks;
  }

  xcopy(newProgramMemoryPointer, beginOfProgramMemory, TO_BYTES(blocksToMove));
  beginOfProgramMemory = newProgramMemoryPointer;
  //debugMemory("resizeProgramMemory : end");
}


#ifdef PC_BUILD
  void ramDump(void) {
    for(calcRegister_t regist=0; regist<FIRST_LOCAL_REGISTER; regist++) {
      printf("Global register      %4u: dataPointer=(block %5u)       dataType=%2u=%s           tag=%2u=%s\n", regist, globalRegister[regist].pointerToRegisterData, globalRegister[regist].dataType, getDataTypeName(globalRegister[regist].dataType, false, true), globalRegister[regist].tag, getRegisterTagName(regist, true));
    }

    //if(currentNumberOfLocalRegisters) {
    //  printf("Saved stack register %4u: dataPointer=(block %5u)       dataType=%2u=%s           tag=%2u=%s\n", regist, globalRegister[regist].pointerToRegisterData, globalRegister[regist].dataType, getDataTypeName(globalRegister[regist].dataType, false, true), globalRegister[regist].tag, getRegisterTagName(regist, true));
    //}

    for(calcRegister_t regist=FIRST_SAVED_STACK_REGISTER; regist<=LAST_SAVED_STACK_REGISTER + 1; regist++) {

    }

    fprintf(stdout, "| block | hex               dec | hec      dec | hex  dec |\n");
    for(uint16_t block=0; block<RAM_SIZE*0+8; block++) {
      fprintf(stdout, "+-------+-----------------------+--------------+----------+\n");
      fprintf(stdout, "| %5u | %08x = %10u | %04x = %5u | %02x = %3u |\n", block, *(uint32_t *)(ram + block), *(uint32_t *)(ram + block), *(uint16_t *)(ram + block), *(uint16_t *)(ram + block), *(uint8_t *)(ram + block), *(uint8_t *)(ram + block));
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
