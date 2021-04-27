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
 * \file registerValueConversions.h
 ***********************************************/
#ifndef REGISTERVALUECONVERSIONS_H
#define REGISTERVALUECONVERSIONS_H

#include "decNumberWrappers.h"
#include "gmpWrappers.h"
#include "typeDefinitions.h"

void convertLongIntegerRegisterToReal34Register      (calcRegister_t source, calcRegister_t destination);
void convertLongIntegerRegisterToShortIntegerRegister(calcRegister_t source, calcRegister_t destination);
void convertLongIntegerRegisterToReal34              (calcRegister_t source, real34_t *destination);
void convertLongIntegerRegisterToReal                (calcRegister_t source, real_t *destination, realContext_t *ctxt);
void convertLongIntegerRegisterToLongInteger         (calcRegister_t regist, longInteger_t longInteger);

void convertLongIntegerToLongIntegerRegister         (const longInteger_t longInteger, calcRegister_t regist);
void convertLongIntegerToShortIntegerRegister        (longInteger_t lgInt, uint32_t base, calcRegister_t destination);
void convertLongIntegerToReal                        (longInteger_t source, real_t *destination, realContext_t *ctxt);

void convertShortIntegerRegisterToReal34Register     (calcRegister_t source, calcRegister_t destination);
void convertShortIntegerRegisterToLongIntegerRegister(calcRegister_t source, calcRegister_t destination);
void convertShortIntegerRegisterToLongInteger        (calcRegister_t source, longInteger_t lgInt);
void convertShortIntegerRegisterToReal               (calcRegister_t source, real_t *destination, realContext_t *ctxt);

void convertShortIntegerRegisterToUInt64             (calcRegister_t regist, int16_t *sign, uint64_t *value);
void convertUInt64ToShortIntegerRegister             (int16_t sign, uint64_t value, uint32_t base, calcRegister_t regist);

void convertReal34ToLongInteger                      (const real34_t *real34, longInteger_t lgInt, enum rounding mode);
void convertReal34ToLongIntegerRegister              (const real34_t *real34, calcRegister_t dest, enum rounding mode);
void convertRealToLongInteger                        (const real_t *real, longInteger_t lgInt, enum rounding mode);
void convertRealToLongIntegerRegister                (const real_t *real, calcRegister_t dest, enum rounding mode);
void realToIntegralValue                             (const real_t *source, real_t *destination, enum rounding mode, realContext_t *realContext);
void realToUInt32                                    (const real_t *re, enum rounding mode, uint32_t *value32, bool_t *overflow);

void convertTimeRegisterToReal34Register             (calcRegister_t source, calcRegister_t destination);
void convertReal34RegisterToTimeRegister             (calcRegister_t source, calcRegister_t destination);
void convertLongIntegerRegisterToTimeRegister        (calcRegister_t source, calcRegister_t destination);

void convertDateRegisterToReal34Register             (calcRegister_t source, calcRegister_t destination);
void convertReal34RegisterToDateRegister             (calcRegister_t source, calcRegister_t destination);

#ifndef TESTSUITE_BUILD
  void convertReal34MatrixRegisterToReal34Matrix     (calcRegister_t regist, real34Matrix_t *matrix);
  void convertReal34MatrixToReal34MatrixRegister     (const real34Matrix_t *matrix, calcRegister_t regist);
#endif // TESTSUITE_BUILD

#endif // REGISTERVALUECONVERSIONS_H
