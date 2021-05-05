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
 * \file registers.h
 */
#ifndef REGISTERS_H
#define REGISTERS_H

#include "longIntegerType.h"
#include "realType.h"
#include "typeDefinitions.h"
#include <stdint.h>

/**
 * Returns the data type of a register.
 *
 * \param[in] regist Register number
 * \return Data type
 */
uint32_t       getRegisterDataType             (calcRegister_t regist);
/**
 * Returns the data pointer of a register.
 *
 * \param[in] regist Register number
 * \return Data pointer
 */
dataBlock_t *  getRegisterDataPointer          (calcRegister_t regist);
/**
 * Returns the data information of a register.
 * This is the angular mode or base.
 *
 * \param[in] regist Register number
 * \return Angular mode
 */
uint32_t       getRegisterTag                  (calcRegister_t regist);
/**
 * Returns the max length of a string.
 * This is including the trailing 0, or a long integer in blocks.
 *
 * \param[in] regist Register number
 * \return Number of blocks
 */
uint16_t       getRegisterMaxDataLength        (calcRegister_t regist);
/**
 * Sets the data type of a register.
 *
 * \param[in] regist   Register number
 * \param[in] dataType Data type
 * \param[in] tag      Tag
 */
void           setRegisterDataType             (calcRegister_t regist, uint16_t dataType, uint32_t tag);
/**
 * Sets the data pointer of a register.
 *
 * \param[in] regist Register number
 * \param[in] memPtr Data pointer
 */
void           setRegisterDataPointer          (calcRegister_t regist, void *memPtr);
/**
 * Sets the data information of a register.
 * This is the angular mode or base.
 *
 * \param[in] regist Register number
 * \param[in] tag    Angular mode
 */
void           setRegisterTag                  (calcRegister_t regist, uint32_t tag);
/**
 * Sets the max length of string in blocks.
 *
 * \param[in] regist     Register number
 * \param[in] maxDataLen Max length of the string
 */
void           setRegisterMaxDataLength        (calcRegister_t regist, uint16_t maxDataLen);
/**
 * Allocates local registers.
 * Works when increasing and when decreasing the number of local registers.
 *
 * \param[in] numberOfRegistersToAllocate Number of registers to allocate
 */
void           allocateLocalRegisters          (uint16_t n);
/**
 * Allocates one named variable.
 *
 * \param[in] variableName         Variable name
 * \param[in] dataType             The content type of the variable
 * \param[in] fullDataSizeInBlocks How many blocks the named variable will require for storage
 */
void           allocateNamedVariable           (const char *variableName, dataType_t dataType, uint16_t fullDataSizeInBlocks);
/**
 * Retrieves the register number for the named variable.
 *
 * \param[in] variableName Register name
 * \return register number to be used by other functions, or INVALID_VARIABLE
 *         if not found
 */
calcRegister_t findNamedVariable               (const char *variableName);
/**
 * Retrieves the register number for the named variable, allocating it if it doesn't exist.
 *
 * \param[in] variableName Register name
 * \return Register number to be used by other functions, or INVALID_VARIABLE
 *         if not possible to allocate (all named variables defined)
 */
calcRegister_t findOrAllocateNamedVariable     (const char *variableName);
/**
 * Returns the full data size of a register in blocks.
 *
 * \param[in] regist Register number
 * \return Number of blocks. For a string this
 *         is the number of bytes reserved for
 *         the string (including the ending 0)
 *         plus 1 block holding the max size
 *         of the string.
 */
uint16_t       getRegisterFullSize             (calcRegister_t regist);
/**
 * Clears a register.
 * That is, set it to 0,0 real16.
 *
 * \param[in] regist Register number
 */
void           clearRegister                   (calcRegister_t regist);
/**
 * Clears all the regs.
 * This includes all globals and locals, which are set to 0,0 real16s
 *
 * \param[in] confirmation Current status of the confirmation of clearing registers
 */
void           fnClearRegisters                (uint16_t confirmation);
/**
 * Sets X to the number of local registers.
 *
 * \param[in] unusedButMandatoryParameter
 */
void           fnGetLocR                       (uint16_t unusedButMandatoryParameter);
void           adjustResult                    (calcRegister_t result, bool_t dropY, bool_t setCpxRes, calcRegister_t op1, calcRegister_t op2, calcRegister_t op3);
/**
 * Duplicates register source to register destination.
 *
 * \param[in] sourceRegister Source register
 * \param[in] destRegister   Destination register
 */
void           copySourceRegisterToDestRegister(calcRegister_t rSource, calcRegister_t rDest);
/**
 * Returns the integer part of the value of a register.
 *
 * \param regist Register
 */
int16_t        indirectAddressing              (calcRegister_t regist, bool_t valueIsRegister, int16_t minValue, int16_t maxValue);
void           reallocateRegister              (calcRegister_t regist, uint32_t dataType, uint16_t dataSizeWithoutDataLenBlocks, uint32_t tag);
void           fnToReal                        (uint16_t unusedButMandatoryParameter);
#ifndef DMCP_BUILD
  void         printReal34ToConsole            (const real34_t *value, const char *before, const char *after);
  void         printRealToConsole              (const real_t *value, const char *before, const char *after);
  void         printComplex34ToConsole         (const complex34_t *value, const char *before, const char *after);
  /**
   * Prints the content of a long integer to the console.
   *
   * \param[in] value  long integer value to print
   * \param[in] before text to display before the value
   * \param[in] after  text to display after the value
   */
  void         printLongIntegerToConsole       (const longInteger_t value, const char *before, const char *after);
  /**
   * Prints the content of a register to the console.
   *
   * \param[in] regist register number
   * \param[in] before text to display before the register value
   * \param[in] after  text to display after the register value
   */
  void         printRegisterToConsole          (calcRegister_t regist, const char *before, const char *after);
  void         printRegisterDescriptorToConsole(calcRegister_t regist);
#endif // DMCP_BUILD


#define getRegisterAngularMode(reg)            getRegisterTag(reg)
#define setRegisterAngularMode(reg, am)        setRegisterTag(reg, am)
#define getRegisterShortIntegerBase(reg)       getRegisterTag(reg)
#define setRegisterShortIntegerBase(reg, base) setRegisterTag(reg, base)
#define getRegisterLongIntegerSign(reg)        getRegisterTag(reg)
#define setRegisterLongIntegerSign(reg, sign)  setRegisterTag(reg, sign)

#ifdef TESTSUITE_BUILD
  /********************************************//**
   * \brief Prints the content of a register to a string
   *
   * \param r calcRegister_t Register number
   * \return void
   ***********************************************/
  void    printRegisterToString           (calcRegister_t regist, char *registerContent);
#endif // TESTSUITE_BUILD

#endif // REGISTERS_H
