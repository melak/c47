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
uint16_t     getVariableNameLength           (calcRegister_t regist);
char        *getVariableNamePointer          (calcRegister_t regist);
uint16_t     getRegisterMaxDataLength        (calcRegister_t regist);
void         setRegisterDataType             (calcRegister_t regist, uint16_t dataType, uint32_t tag);
void         setRegisterDataPointer          (calcRegister_t regist, void *memPtr);
void         setRegisterTag                  (calcRegister_t regist, uint32_t tag);
void         setVariableNameLength           (calcRegister_t regist, uint16_t length);
void         setVariableNamePointer          (calcRegister_t regist, void *namePointer);
void         setRegisterMaxDataLength        (calcRegister_t regist, uint16_t maxDataLen);
void         allocateLocalRegisters          (uint16_t n);
void         allocateNamedVariable           (const char *variableName);
uint16_t     getRegisterFullSize             (calcRegister_t regist);
void         clearRegister                   (calcRegister_t regist);
void         fnClearRegisters                (uint16_t confirmation);
void         fnGetLocR                       (uint16_t unusedButMandatoryParameter);
void         adjustResult                    (calcRegister_t result, bool_t dropY, bool_t setCpxRes, calcRegister_t op1, calcRegister_t op2, calcRegister_t op3);
void         copySourceRegisterToDestRegister(calcRegister_t rSource, calcRegister_t rDest);
void         fnStore                         (uint16_t r);
void         fnStoreAdd                      (uint16_t r);
void         fnStoreSub                      (uint16_t r);
void         fnStoreMult                     (uint16_t r);
void         fnStoreDiv                      (uint16_t r);
void         fnStoreMin                      (uint16_t r);
void         fnStoreMax                      (uint16_t r);
void         fnStoreConfig                   (uint16_t r);
void         fnStoreStack                    (uint16_t r);
void         fnStoreElement                  (uint16_t unusedButMandatoryParameter);
void         fnStoreIJ                       (uint16_t unusedButMandatoryParameter);
void         fnRecall                        (uint16_t r);
void         fnLastX                         (uint16_t unusedButMandatoryParameter);
void         fnRecallAdd                     (uint16_t r);
void         fnRecallSub                     (uint16_t r);
void         fnRecallMult                    (uint16_t r);
void         fnRecallDiv                     (uint16_t r);
void         fnRecallMin                     (uint16_t r);
void         fnRecallMax                     (uint16_t r);
void         fnRecallConfig                  (uint16_t r);
void         fnRecallStack                   (uint16_t r);
void         fnRecallElement                 (uint16_t unusedButMandatoryParameter);
void         fnRecallIJ                      (uint16_t unusedButMandatoryParameter);
void         fnXLessThan                     (uint16_t unusedButMandatoryParameter);
int16_t      indirectAddressing              (calcRegister_t regist, int16_t minValue, int16_t maxValue);
void         reallocateRegister              (calcRegister_t regist, uint32_t dataType, uint16_t dataSizeWithoutDataLenBlocks, uint32_t tag);
void         fnToReal                        (uint16_t unusedButMandatoryParameter);
void         fnDec                           (uint16_t r);
void         fnInc                           (uint16_t r);
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


//=============================================================================
// Register Comparison and Min/Max functions
//-----------------------------------------------------------------------------

bool_t registerCmp(calcRegister_t reg1, calcRegister_t reg2, int8_t *result);
void registerCmpError(calcRegister_t regist1, calcRegister_t regist2);

void registerMax(calcRegister_t reg1, calcRegister_t reg2, calcRegister_t dest);
void registerMin(calcRegister_t reg1, calcRegister_t reg2, calcRegister_t dest);

