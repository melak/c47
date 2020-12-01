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
 * \file memory.h
 ***********************************************/

void    *wp43sAllocate     (size_t sizeInBytes);
void    *wp43sReallocate   (void *pcMemPtr, size_t oldSizeInBytes, size_t newSizeInBytes);
void    wp43sFree          (void *pcMemPtr, size_t sizeInBytes);

// The 6 followoing functions are only there to know who allocates and frees memory
void    *allocWp43s        (size_t sizeInBytes);
void    *reallocWp43s      (void *pcMemPtr, size_t oldSizeInBytes, size_t newSizeInBytes);
void    freeWp43s          (void *pcMemPtr, size_t sizeInBytes);

void    *allocGmp          (size_t sizeInBytes);
void    *reallocGmp        (void *pcMemPtr, size_t oldSizeInBytes, size_t newSizeInBytes);
void    freeGmp            (void *pcMemPtr, size_t sizeInBytes);

int32_t getFreeRamMemory   (void);
void    resizeProgramMemory(uint16_t newSizeInBlocks);

#ifndef DMCP_BUILD
  void    debugMemory      (const char *message);
#endif // DMCP_BUILD
