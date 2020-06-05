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
#define AIM_REGISTER_LINE        REGISTER_X
#define TAM_REGISTER_LINE        REGISTER_T
#define NIM_REGISTER_LINE        REGISTER_X
#define ERR_REGISTER_LINE        REGISTER_Z
#define TRUE_FALSE_REGISTER_LINE REGISTER_Z

#define SAVED_REGISTER_X    2000
#define SAVED_REGISTER_Y    2001
#define SAVED_REGISTER_Z    2002
#define SAVED_REGISTER_T    2003
#define SAVED_REGISTER_A    2004
#define SAVED_REGISTER_B    2005
#define SAVED_REGISTER_C    2006
#define SAVED_REGISTER_D    2007
#define SAVED_REGISTER_L    2008
#define LAST_SAVED_REGISTER 2009
#define TEMP_REGISTER       2009

#define getStackTop()                         (getSystemFlag(FLAG_SSIZE8) ? REGISTER_D : REGISTER_T)

#define freeRegisterData(regist)              freeWp43s((void *)getRegisterDataPointer(regist), TO_BYTES(getRegisterFullSize(regist)))

#define storeToDtConfigDescriptor(config)     (configToStore->config = config)
#define recallFromDtConfigDescriptor(config)  (config = configToRecall->config)

#define getRecalledSystemFlag(sf)             ((configToRecall->systemFlags &   ((uint64_t)1 << (sf & 0x3fff))) != 0)
#define setSystemFlagToRecalled(sf)           (getRecalledSystemFlag(sf)) ? (setSystemFlag(sf)) : (clearSystemFlag(sf))

///////////////////////////////////////////////////////
// Register numbering:
//    0 to  111 global resisters
//  112 to  211 local registers (from .00 to .99) this are 100 local registers but TAM allows only a parameter from 0 to 99 (without indirection)
// 1000 to 1999 named variables
// 2000 to 2009 saved stack registers (UNDO item)


/********************************************//**
 * \typedef dataType_t
 * \brief Different data types
 ***********************************************/
typedef enum {
  dtLongInteger     =  0,  ///< Z arbitrary precision integer
  dtReal34          =  1,  ///< R double precision real (128 bits)
  dtComplex34       =  2,  ///< C double precision complex (2x 128 bits)
  dtTime            =  3,  ///< Time
  dtDate            =  4,  ///< Date in various formats
  dtString          =  5,  ///< Alphanumeric string
  dtReal34Matrix    =  6,  ///< Double precision vector or matrix
  dtComplex34Matrix =  7,  ///< Double precision complex vector or matrix
  dtShortInteger    =  8,  ///< Short integer (64 bit)
  dtConfig          =  9,  ///< Configuration
  //dtLabel           = 10,  ///< Label
  //dtSystemInteger   = 11,  ///< System integer (64 bits)
  //dtFlags           = 12,  ///< Flags
  //dtPgmStep         = 13,  ///< Program step
  //dtDirectory       = 14,  ///< Program
} dataType_t; // 4 bits (NOT 5 BITS)

typedef struct {
  uint8_t   shortIntegerMode;
  uint8_t   shortIntegerWordSize;
  uint8_t   displayFormat;
  uint8_t   displayFormatDigits;
  uint8_t   groupingGap;
  uint8_t   currentAngularMode;
  uint8_t   displayStack;
  uint8_t   curveFitting;
  uint8_t   roundingMode;
  uint32_t  denMax;
  uint32_t  firstGregorianDay;
  uint64_t  systemFlags;
  calcKey_t kbd_usr[37];

  //    int16_t   Norm_Key_00_VAR;                                           //JMCFGvv
  uint8_t SigFigMode;
  bool_t eRPN;
  bool_t HOME3;
  bool_t ShiftTimoutMode;
  bool_t Home3TimerMode;
  bool_t UNITDisplay;
  bool_t SH_BASE_HOME;
  bool_t SH_BASE_AHOME;
  int16_t Norm_Key_00_VAR;
  uint8_t Input_Default;
  bool_t jm_FG_LINE;
  bool_t jm_FG_DOTS;
  bool_t jm_G_DOUBLETAP;
  float graph_xmin;
  float graph_xmax;
  float graph_ymin;
  float graph_ymax;
  float graph_dx;
  float graph_dy;
  bool_t jm_VECT;
  bool_t jm_HOME_SUM;
  bool_t jm_HOME_MIR;
  bool_t jm_HOME_FIX;                                                        //JMCFG^^

} dtConfigDescriptor_t;

typedef union {
  uint32_t descriptor;
  struct {
    uint32_t dataPointer     : 16; // Memory block number
    uint32_t dataType        :  4; // dtLongInteger, dtReal16, ...
    uint32_t tag             :  5; // AM_NONE, Short integer base, AM_DEGREE, AM_DMS,...
    uint32_t notUsed         :  7;
  };
} registerDescriptor_t;

// Header for datatype string, long integer, and matrix
typedef union {
  uint32_t data;
  struct {
    uint16_t dataMaxLength;          // String max length (includind terminating \0) in blocks or Long integer max length in blocks
    uint16_t numberOfNamedVariables;
  };
  struct {
    uint16_t variableNameLen;       // Size of the name in blocs: 1 to 4, up to 15 bytes = 7 double byte glyphs + trailing 0
    uint16_t ptrToVariableName;
  };
  struct {
    uint16_t localFlags;             // 16 local flags
    uint16_t numberOfLocalRegisters;
  };
  struct {
    uint16_t matrixLines;            // Number of lines in the matrix
    uint16_t matrixColumns;          // Number of columns in the matrix
  };
} dataBlock_t;


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
void         fnClearRegisters                (uint16_t unusedParamButMandatory);
void         fnGetLocR                       (uint16_t unusedParamButMandatory);
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
void         fnStoreElement                  (uint16_t unusedParamButMandatory);
void         fnStoreIJ                       (uint16_t unusedParamButMandatory);
void         fnRecall                        (uint16_t r);
void         fnLastX                         (uint16_t unusedParamButMandatory);
void         fnRecallAdd                     (uint16_t r);
void         fnRecallSub                     (uint16_t r);
void         fnRecallMult                    (uint16_t r);
void         fnRecallDiv                     (uint16_t r);
void         fnRecallMin                     (uint16_t r);
void         fnRecallMax                     (uint16_t r);
void         fnRecallConfig                  (uint16_t r);
void         fnRecallStack                   (uint16_t r);
void         fnRecallElement                 (uint16_t unusedParamButMandatory);
void         fnRecallIJ                      (uint16_t unusedParamButMandatory);
void         fnXLessThan                     (uint16_t unusedParamButMandatory);
int16_t      indirectAddressing              (calcRegister_t regist, int16_t minValue, int16_t maxValue);
void         printReal34ToConsole            (const real34_t *value, const char *before, const char *after);
void         printRealToConsole              (const real_t *value, const char *before, const char *after);
void         printComplex34ToConsole         (const complex34_t *value, const char *before, const char *after);
void         printLongIntegerToConsole       (const longInteger_t value, const char *before, const char *after);
void         reallocateRegister              (calcRegister_t regist, uint32_t dataType, uint16_t dataSizeWithoutDataLenBlocks, uint32_t tag);
void         printRegisterToConsole          (calcRegister_t regist, const char *before, const char *after);
void         printRegisterDescriptorToConsole(calcRegister_t regist);
void         fnToReal                        (uint16_t unusedParamButMandatory);
void         fnDec                           (uint16_t r);
void         fnInc                           (uint16_t r);


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

