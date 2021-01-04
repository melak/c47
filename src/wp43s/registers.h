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
 * \file registers.h
 ***********************************************/


uint32_t     getRegisterDataType             (calcRegister_t regist);
dataBlock_t *getRegisterDataPointer          (calcRegister_t regist);
uint32_t     getRegisterTag                  (calcRegister_t regist);
uint16_t     getRegisterMaxDataLength        (calcRegister_t regist);
void         setRegisterDataType             (calcRegister_t regist, uint16_t dataType, uint32_t tag);
void         setRegisterDataPointer          (calcRegister_t regist, void *memPtr);
void         setRegisterTag                  (calcRegister_t regist, uint32_t tag);
void         setRegisterMaxDataLength        (calcRegister_t regist, uint16_t maxDataLen);
void         allocateLocalRegisters          (uint16_t n);
void         allocateNamedVariable           (const char *variableName, dataType_t dataType, uint16_t fullDataSizeInBlocks);
uint16_t     getRegisterFullSize             (calcRegister_t regist);
void         clearRegister                   (calcRegister_t regist);
void         fnClearRegisters                (uint16_t confirmation);
void         fnGetLocR                       (uint16_t unusedButMandatoryParameter);
void         adjustResult                    (calcRegister_t result, bool_t dropY, bool_t setCpxRes, calcRegister_t op1, calcRegister_t op2, calcRegister_t op3);
void         copySourceRegisterToDestRegister(calcRegister_t rSource, calcRegister_t rDest);
void         fnXLessThan                     (uint16_t unusedButMandatoryParameter);
int16_t      indirectAddressing              (calcRegister_t regist, int16_t minValue, int16_t maxValue);
void         reallocateRegister              (calcRegister_t regist, uint32_t dataType, uint16_t dataSizeWithoutDataLenBlocks, uint32_t tag);
void         fnToReal                        (uint16_t unusedButMandatoryParameter);
#ifndef DMCP_BUILD
  void       printReal34ToConsole            (const real34_t *value, const char *before, const char *after);
  void       printRealToConsole              (const real_t *value, const char *before, const char *after);
  void       printComplex34ToConsole         (const complex34_t *value, const char *before, const char *after);
  void       printLongIntegerToConsole       (const longInteger_t value, const char *before, const char *after);
  void       printRegisterToConsole          (calcRegister_t regist, const char *before, const char *after);
  void       printRegisterDescriptorToConsole(calcRegister_t regist);
#endif // DMCP_BUILD


#define getRegisterAngularMode(reg)            getRegisterTag(reg)
#define setRegisterAngularMode(reg, am)        setRegisterTag(reg, am)
#define getRegisterShortIntegerBase(reg)       getRegisterTag(reg)
#define setRegisterShortIntegerBase(reg, base) setRegisterTag(reg, base)
#define getRegisterLongIntegerSign(reg)        getRegisterTag(reg)
#define setRegisterLongIntegerSign(reg, sign)  setRegisterTag(reg, sign)

#ifdef TESTSUITE_BUILD
  void    printRegisterToString           (calcRegister_t regist, char *registerContent);
#endif // TESTSUITE_BUILD
