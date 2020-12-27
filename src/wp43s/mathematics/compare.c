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
 * \file compare.c
 ***********************************************/

#include "wp43s.h"



static void (* const cmpFunc[NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS][NUMBER_OF_DATA_TYPES_FOR_CALCULATIONS])(calcRegister_t reg1, calcRegister_t reg2, int8_t *result) = {
// reg1 |    reg2 ==>    1                    2                    3          4                    5                    6                    7              8               9                    10
//      V                Long integer         Real34               Complex34  Time                 Date                 String               Real34 mat     Complex34 mat   Short integer        Config data
/*  1 Long integer  */ { registerCmpLonILonI, registerCmpLonIReal, NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           registerCmpLonIShoI, NULL},
/*  2 Real34        */ { registerCmpRealLonI, registerCmpRealReal, NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           registerCmpRealShoI, NULL},
/*  3 Complex34     */ { NULL,                NULL,                NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           NULL,                NULL},
/*  4 Time          */ { NULL,                NULL,                NULL,      registerCmpTimeTime, NULL,                NULL,                NULL,          NULL,           NULL,                NULL},
/*  5 Date          */ { NULL,                NULL,                NULL,      NULL,                registerCmpDateDate, NULL,                NULL,          NULL,           NULL,                NULL},
/*  6 String        */ { NULL,                NULL,                NULL,      NULL,                NULL,                registerCmpStriStri, NULL,          NULL,           NULL,                NULL},
/*  7 Real34 mat    */ { NULL,                NULL,                NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           NULL,                NULL},
/*  8 Complex34 mat */ { NULL,                NULL,                NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           NULL,                NULL},
/*  9 Short integer */ { registerCmpShoILonI, registerCmpShoIReal, NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           registerCmpShoIShoI, NULL},
/* 10 Config data   */ { NULL,                NULL,                NULL,      NULL,                NULL,                NULL,                NULL,          NULL,           NULL,                NULL}
};



bool_t registerCmp(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  void (*func)(calcRegister_t, calcRegister_t, int8_t*) = cmpFunc[getRegisterDataType(regist1)][getRegisterDataType(regist2)];

  if(func == NULL)
    return false;

  func(regist1, regist2, result);

  return true;
}



void registerCmpError(calcRegister_t regist1, calcRegister_t regist2) {
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  #if (EXTRA_INFO_ON_CALC_ERROR == 1)
    sprintf(errorMessage, "cannot get compare: %s", getRegisterDataTypeName(regist1, true, false));
    sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "and %s", getRegisterDataTypeName(regist2, true, false));
    moreInfoOnError("In function registerCmp:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
  #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
}



void registerCmpLonILonI(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Long Integer, regist2 = Long Integer

  longInteger_t r1, r2;

  convertLongIntegerRegisterToLongInteger(regist1, r1);
  convertLongIntegerRegisterToLongInteger(regist2, r2);

  int8_t value = longIntegerCompare(r1, r2);

  longIntegerFree(r1);
  longIntegerFree(r2);

  *result = value;
}



void registerCmpLonIShoI(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Long Integer, regist2 = Short Integer

  longInteger_t r1, r2;

  convertLongIntegerRegisterToLongInteger(regist1, r1);
  convertShortIntegerRegisterToLongInteger(regist2, r2);

  int8_t value = longIntegerCompare(r1, r2);

  longIntegerFree(r1);
  longIntegerFree(r2);

  *result = value;
}



void registerCmpLonIReal(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Long Integer, regist2 = Real

  real_t r1, r2;

  convertLongIntegerRegisterToReal(regist1, &r1, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &r2);

  if(realCompareEqual(&r1, &r2))
    *result = 0;
  else
    *result = realCompareGreaterThan(&r1, &r2) ? 1 : -1;
}



void registerCmpTimeTime(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  fnToBeCoded();
}



void registerCmpDateDate(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  fnToBeCoded();
}



void registerCmpStriStri(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
 *result = compareString(REGISTER_STRING_DATA(regist1), REGISTER_STRING_DATA(regist2), CMP_EXTENSIVE);
}

void registerCmpShoILonI(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Short Integer, regist2 = Long Integer

  longInteger_t r1, r2;

  convertShortIntegerRegisterToLongInteger(regist1, r1);
  convertLongIntegerRegisterToLongInteger(regist2, r2);

  int8_t value = longIntegerCompare(r1, r2);

  longIntegerFree(r1);
  longIntegerFree(r2);

  *result = value;
}



void registerCmpShoIShoI(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Short Integer, regist2 = Short Integer

  longInteger_t r1, r2;

  convertShortIntegerRegisterToLongInteger(regist1, r1);
  convertShortIntegerRegisterToLongInteger(regist2, r2);

  int8_t value = longIntegerCompare(r1, r2);

  longIntegerFree(r1);
  longIntegerFree(r2);

  *result = value;
}



void registerCmpShoIReal(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Short Integer, regist2 = Real

  real_t r1, r2;

  convertShortIntegerRegisterToReal(regist1, &r1, &ctxtReal39);
  real34ToReal(REGISTER_REAL34_DATA(regist2), &r2);

  if(realCompareEqual(&r1, &r2))
    *result = 0;
  else
    *result = realCompareGreaterThan(&r1, &r2) ? 1 : -1;
}



void registerCmpRealLonI(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Real, regist2 = Long Integer

  real_t r1, r2;

  real34ToReal(REGISTER_REAL34_DATA(regist1), &r1);
  convertLongIntegerRegisterToReal(regist2, &r2, &ctxtReal39);

  if(realCompareEqual(&r1, &r2))
    *result = 0;
  else
    *result = realCompareGreaterThan(&r1, &r2) ? 1 : -1;
}



void registerCmpRealShoI(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Real, regist2 = Short Integer

  real_t r1, r2;

  real34ToReal(REGISTER_REAL34_DATA(regist1), &r1);
  convertShortIntegerRegisterToReal(regist2, &r2, &ctxtReal39);

  if(realCompareEqual(&r1, &r2))
    *result = 0;
  else
    *result = realCompareGreaterThan(&r1, &r2) ? 1 : -1;
}



void registerCmpRealReal(calcRegister_t regist1, calcRegister_t regist2, int8_t *result) {
  // regist1 = Real, regist2 = Real

  real_t r1, r2;

  real34ToReal(REGISTER_REAL34_DATA(regist1), &r1);
  real34ToReal(REGISTER_REAL34_DATA(regist2), &r2);

  if(realCompareEqual(&r1, &r2))
    *result = 0;
  else
    *result = realCompareGreaterThan(&r1, &r2) ? 1 : -1;
}



void registerMax(calcRegister_t regist1, calcRegister_t regist2, calcRegister_t dest) {
  int8_t result = 0;

  if(!registerCmp(regist1, regist2, &result))
    registerCmpError(regist1, regist2);
  else if(result != 0) {
    copySourceRegisterToDestRegister(result>0 ? regist1 : regist2, dest);
    if(getRegisterDataType(dest) == dtShortInteger) {
      *(REGISTER_SHORT_INTEGER_DATA(dest)) &= shortIntegerMask;
    }
  }
}



void registerMin(calcRegister_t regist1, calcRegister_t regist2, calcRegister_t dest) {
  int8_t result = 0;

  if(!registerCmp(regist1, regist2, &result))
    registerCmpError(regist1, regist2);
  else if(result != 0) {
    copySourceRegisterToDestRegister(result>0 ? regist2 : regist1, dest);
    if(getRegisterDataType(dest) == dtShortInteger) {
      *(REGISTER_SHORT_INTEGER_DATA(dest)) &= shortIntegerMask;
    }
  }
}
