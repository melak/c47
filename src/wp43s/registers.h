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

#define FIRST_LOCAL_REGISTER 112

#define REGISTER_X  100
#define REGISTER_Y  101
#define REGISTER_Z  102
#define REGISTER_T  103
#define REGISTER_A  104
#define REGISTER_B  105
#define REGISTER_C  106
#define REGISTER_D  107
#define REGISTER_L  108
#define REGISTER_I  109
#define REGISTER_J  110
#define REGISTER_K  111

// If one of the 4 next defines is changed: change also xxxLINE in screen.h
#define AIM_REGISTER_LINE REGISTER_X
#define TAM_REGISTER_LINE REGISTER_T
#define NIM_REGISTER_LINE REGISTER_X
#define ERR_REGISTER_LINE REGISTER_Z

#define FIRST_TEMPORARY_REGISTER         2000
#define NUMBER_OF_TEMPORARY_REGISTERS       8 // 7 are used here: [3] [1/x] [f] [a b/c] [1/x]

#define SAVED_REGISTER_X 3000
#define SAVED_REGISTER_Y 3001
#define SAVED_REGISTER_Z 3002
#define SAVED_REGISTER_T 3003
#define SAVED_REGISTER_A 3004
#define SAVED_REGISTER_B 3005
#define SAVED_REGISTER_C 3006
#define SAVED_REGISTER_D 3007
#define SAVED_REGISTER_L 3008

#define getStackTop()                      (stackSize == SS_4 ? REGISTER_T : REGISTER_D)

#define freeRegisterData(regist)           freeWp43s(getRegisterDataPointer(regist), getRegisterFullSize(regist))



///////////////////////////////////////////////////////
// Register numbering:
//    0 to  111 global resisters
//  112 to  199 local registers (.00 to .87)
// 1000 to 1999 named registers
// 2000 to 2007 temporary registers
// 3000 to 3007 saved stack registers (UNDO item)


/********************************************//**
 * \typedef dataType_t
 * \brief Different data types
 ***********************************************/
typedef enum {
  dtLongInteger     =  0,  ///< Z arbitrary precision integer
  dtReal16          =  1,  ///< R single precision real (64 bits)
  dtComplex16       =  2,  ///< C single precision complex (2x 64 bits), RegDataInfo contains rectangular or polar mode
  dtAngle16         =  3,  ///< Single precision angle
  dtTime            =  4,  ///< Time
  dtDate            =  5,  ///< Date in various formats
  dtString          =  6,  ///< Alphanumeric string
  dtReal16Matrix    =  7,  ///< Single precision vector or matrix
  dtComplex16Matrix =  8,  ///< Double precision vector or matrix
  dtShortInteger    =  9,  ///< Short integer (64 bit)
  dtReal34          = 10,  ///< R double precision real (128 bits)
  dtComplex34       = 11,  ///< C double precision complex (2x 128 bits)
  dtAngle34         = 12   ///< Double precision angle
  //dtLabel           = 13,  ///< Label
  //dtSystemInteger   = 14,  ///< System integer (64 bits)
  //dtFlags           = 15,  ///< Flags
  //dtConfig          = 16,  ///< Configuration
  //dtPgmStep         = 17,  ///< Program step
  //dtDirectory       = 18,  ///< Program
} dataType_t; // 4 bits (NOT 5 BITS)

typedef union {
  uint32_t descriptor;
  struct {
    uint32_t dataPointer     : 16; // Memory block number
    uint32_t dataType        :  4; // dtLongInteger, dtReal16, ...
    uint32_t tag             :  5; // TAG_NONE, Short integer base, AM_DEGREE, AM_DMS,...
    uint32_t notUsed         :  2;
    uint32_t variableNameLen :  5;
  };
} registerDescriptor_t;


uint32_t          getRegisterDataType             (calcRegister_t regist);
void             *getRegisterDataPointer          (calcRegister_t regist);
uint32_t          getRegisterTag                  (calcRegister_t regist);
uint32_t          getRegisterNameLength           (calcRegister_t regist);
char             *getRegisterNamePointer          (calcRegister_t regist);
uint32_t          getRegisterMaxDataLength        (calcRegister_t regist);
void              setRegisterDataType             (calcRegister_t regist, uint16_t dataType, uint32_t tag);
void              setRegisterDataPointer          (calcRegister_t regist, void *memPtr);
void              setRegisterTag                  (calcRegister_t regist, uint32_t tag);
void              setRegisterNameLength           (calcRegister_t regist, uint16_t length);
void              setRegisterNamePointer          (calcRegister_t regist, void *namePointer);
void              setRegisterMaxDataLength        (calcRegister_t regist, uint32_t maxDataLen);
void              allocateLocalRegisters          (uint16_t n);
void              allocateNamedVariable           (const char *variableName);
uint32_t          getRegisterFullSize             (calcRegister_t regist);
void              clearRegister                   (calcRegister_t regist);
void              fnClearRegisters                (uint16_t unusedParamButMandatory);
void              fnGetLocR                       (uint16_t unusedParamButMandatory);
void              fnConvertXToReal34              (uint16_t unusedParamButMandatory);
void              fnConvertXToReal16              (uint16_t unusedParamButMandatory);
void              adjustResult                    (calcRegister_t result, bool_t dropY, bool_t setCpxRes, calcRegister_t op1, calcRegister_t op2, calcRegister_t op3);
void              copySourceRegisterToDestRegister(calcRegister_t rSource, calcRegister_t rDest);
void              fnStore                         (uint16_t r);
void              fnStoreAdd                      (uint16_t r);
void              fnStoreSub                      (uint16_t r);
void              fnStoreMult                     (uint16_t r);
void              fnStoreDiv                      (uint16_t r);
void              fnStoreMin                      (uint16_t r);
void              fnStoreMax                      (uint16_t r);
void              fnStoreConfig                   (uint16_t r);
void              fnStoreStack                    (uint16_t r);
void              fnStoreElement                  (uint16_t r);
void              fnStoreIJ                       (uint16_t r);
void              fnRecall                        (uint16_t r);
void              fnRecallAdd                     (uint16_t r);
void              fnRecallSub                     (uint16_t r);
void              fnRecallMult                    (uint16_t r);
void              fnRecallDiv                     (uint16_t r);
void              fnRecallMin                     (uint16_t r);
void              fnRecallMax                     (uint16_t r);
void              fnRecallConfig                  (uint16_t r);
void              fnRecallStack                   (uint16_t r);
void              fnRecallElement                 (uint16_t r);
void              fnRecallIJ                      (uint16_t r);
void              fnXLessThan                     (uint16_t unusedParamButMandatory);
int16_t           indirectAddressing              (calcRegister_t regist, int16_t minValue, int16_t maxValue);
void              printReal16ToConsole            (const real16_t *value);
void              printReal34ToConsole            (const real34_t *value);
void              printComplex16ToConsole         (const complex16_t *value);
void              printComplex34ToConsole         (const complex34_t *value);
void              printReal51ToConsole            (const real51_t *value);
void              printReal451ToConsole           (const real451_t *value);
void              printLongIntegerToConsole       (longInteger_t value);
void              reallocateRegister              (calcRegister_t regist, uint32_t dataType, uint32_t dataSizeWithoutDataLen, uint32_t tag);
calcRegister_t    allocateTemporaryRegister       (void);
void              freeTemporaryRegister           (calcRegister_t tmpReg);

#ifdef DMCP_BUILD
  void            printRegisterToConsole          (calcRegister_t regist, int16_t line);
#else
  void            printRegisterToConsole          (calcRegister_t regist);
#endif
void              printRegisterDescriptorToConsole(calcRegister_t regist);

#define getRegisterAngularMode(reg)            getRegisterTag(reg)
#define setRegisterAngularMode(reg, am)        setRegisterTag(reg, am)
#define getRegisterShortIntegerBase(reg)       getRegisterTag(reg)
#define setRegisterShortIntegerBase(reg, base) setRegisterTag(reg, base)
#define getRegisterLongIntegerSign(reg)        getRegisterTag(reg)
#define setRegisterLongIntegerSign(reg, sign)  setRegisterTag(reg, sign)

#ifdef TESTSUITE_BUILD
  void            printRegisterToString           (calcRegister_t regist, char *registerContent);
#endif // TESTSUITE_BUILD
