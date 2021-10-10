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

/**
 * \file memory.h
 */
#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>
#include "typeDefinitions.h"

void    *wp43sAllocate        (size_t sizeInBlocks);
void    *wp43sReallocate      (void *pcMemPtr, size_t oldSizeInBlocks, size_t newSizeInBlocks);
void    wp43sFree             (void *pcMemPtr, size_t sizeInBlocks);

// The 6 followoing functions are only there to know who allocates and frees memory
void    *allocWp43s           (size_t sizeInBlocks);
void    *reallocWp43s         (void *pcMemPtr, size_t oldSizeInBlocks, size_t newSizeInBlocks);
void    freeWp43s             (void *pcMemPtr, size_t sizeInBlocks);

void    *allocGmp             (size_t sizeInBytes);
void    *reallocGmp           (void *pcMemPtr, size_t oldSizeInBytes, size_t newSizeInBytes);
void    freeGmp               (void *pcMemPtr, size_t sizeInBytes);

int32_t getFreeRamMemory      (void);
void    resizeProgramMemory   (uint16_t newSizeInBlocks);
bool_t  isMemoryBlockAvailable(size_t sizeInBlocks);

#ifndef DMCP_BUILD
  void    debugMemory         (const char *message);
#endif // DMCP_BUILD

#ifdef PC_BUILD
  void ramDump(void);
#endif // PC_BUILD

// The following macros are for avoid crash in case that the memory is full. The corresponding label `cleanup_***` is needed AFTER freeing the memory.
#define checkedAllocate2(var, size, label) var = allocWp43s(size); if(!var) {displayCalcErrorMessage(ERROR_RAM_FULL, ERR_REGISTER_LINE, NIM_REGISTER_LINE); goto label;}
#define checkedAllocate(var, size) checkedAllocate2(var, size, cleanup_##var)

#endif // MEMORY_H
