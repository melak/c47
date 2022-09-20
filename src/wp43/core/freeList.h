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
 * \file core/freeList.h
 */
#if !defined(FREELIST_H)
  #define FREELIST_H

  #include <stddef.h>

  /**
   * Allocate memory.
   * Allocates a given amount of memory and returns a pointer to the start of
   * that memory. The allocator doesn't keep track of how much memory has been
   * allocated, so this must be kept for when calling the freeListFree function.
   * Any allocated memory must be freed with freeListFree or reallocated with
   * freeListRealloc. Allocated memory is whatever was previously in memory (it
   * is not zeroed).
   *
   * \param[in] sizeInBlocks size of the memory in blocks, not bytes
   * \return pointer to allocated memory
   */
  void *freeListAlloc(size_t sizeInBlocks);

  /**
   * Reallocate memory.
   * Increase or decrease the amount of memory of a previous allocation. Since
   * the amount of memory allocated isn't kept by the allocator, this must be
   * provided along with the new required size. The pointer returned will differ
   * from the one provided and the old pointer will be freed so should no longer
   * be used. Data is copied over to the new location. If the size of the allocation
   * is reduced, then the copy will only be for the new size. If the size is increased
   * then the extra data will be whatever was previously in memory (it is not zeroed).
   *
   * \param[in] pcMemPtr pointer to the previously allocated memory
   * \param[in] oldSizeInBlocks size used when this pointer was allocated
   * \param[in] newSizeInBlocks new required size for the allocation
   * \return pointer to the allocated memory
   */
  void *freeListRealloc(void *pcMemPtr, size_t oldSizeInBlocks, size_t newSizeInBlocks);

  /**
   * Free allocated memory.
   * Frees memory previous allocated with freeListAlloc or freeListRealloc.
   * This must be called at some point after memory has been allocated. The
   * allocator does not keep track of how much memory has been allocated, so
   * this must be provided. If the memory has been reallocated with freeListRealloc
   * then the size of the last alloc should be provided, along with the latest
   * pointer allocated.
   *
   * \param[in] pcMemPtr pointer to the previously allocated memory
   * \param[in] sizeInBlocks size used when this pointer was allocated
   */
  void freeListFree(void *pcMemPtr, size_t sizeInBlocks);

#endif // !FREELIST_H
