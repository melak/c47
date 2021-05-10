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

#include "matrix.h"

#include <string.h>
#include "bufferize.h"
#include "charString.h"
#include "constantPointers.h"
#include "conversionAngles.h"
#include "debug.h"
#include "defines.h"
#include "display.h"
#include "error.h"
#include "flags.h"
#include "fonts.h"
#include "items.h"
#include "longIntegerType.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/division.h"
#include "mathematics/multiplication.h"
#include "mathematics/toPolar.h"
#include "memory.h"
#include "realType.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "screen.h"
#include "softmenus.h"
#include "stack.h"
#include "store.h"
#include "ui/tam.h"

#include "wp43s.h"

#ifndef TESTSUITE_BUILD
any34Matrix_t         openMatrixMIMPointer;
bool_t                matEditMode;
uint16_t              scrollRow;
uint16_t              scrollColumn;
uint16_t              tmpRow;
uint16_t              matrixIndex = INVALID_VARIABLE;



static bool_t getArg(calcRegister_t regist, real_t *arg) {
  if(getRegisterDataType(regist) == dtLongInteger) {
    convertLongIntegerRegisterToReal(regist, arg, &ctxtReal39);
  }
  else if(getRegisterDataType(regist) == dtReal34) {
    real34ToReal(REGISTER_REAL34_DATA(regist), arg);
    realToIntegralValue(arg, arg, DEC_ROUND_DOWN, &ctxtReal39);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot accept %s as the argument", getRegisterDataTypeName(regist, true, false));
      moreInfoOnError("In function getArg:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  return true;
}

static bool_t getDimensionArg(uint32_t *rows, uint32_t *cols) {
  longInteger_t tmp_lgInt;

  //Get Size from REGISTER_X and REGISTER_Y
  if(((getRegisterDataType(REGISTER_X) != dtLongInteger) && (getRegisterDataType(REGISTER_X) != dtReal34)) ||
    ((getRegisterDataType(REGISTER_Y) != dtLongInteger) && (getRegisterDataType(REGISTER_Y) != dtReal34))) {
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "invalid data type %s and %s", getRegisterDataTypeName(REGISTER_Y, true, false), getRegisterDataTypeName(REGISTER_X, true, false));
        moreInfoOnError("In function fnNewMatrix:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return false;
  }

  if(getRegisterDataType(REGISTER_X) == dtLongInteger)
    convertLongIntegerRegisterToLongInteger(REGISTER_X, tmp_lgInt);
  else // dtReal34
    convertReal34ToLongInteger(REGISTER_REAL34_DATA(REGISTER_X), tmp_lgInt, DEC_ROUND_DOWN);
  if(longIntegerIsNegativeOrZero(tmp_lgInt)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      char strbuf[32];
      longIntegerToAllocatedString(tmp_lgInt, strbuf, 32);
      sprintf(errorMessage, "invalid number of columns");
      moreInfoOnError("In function fnNewMatrix:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  longIntegerToUInt(tmp_lgInt, *cols);

  if(getRegisterDataType(REGISTER_Y) == dtLongInteger)
    convertLongIntegerRegisterToLongInteger(REGISTER_Y, tmp_lgInt);
  else // dtReal34
    convertReal34ToLongInteger(REGISTER_REAL34_DATA(REGISTER_Y), tmp_lgInt, DEC_ROUND_DOWN);
  if(longIntegerIsNegativeOrZero(tmp_lgInt)) {
    displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      char strbuf[32];
      longIntegerToAllocatedString(tmp_lgInt, strbuf, 32);
      sprintf(errorMessage, "invalid number of rows");
      moreInfoOnError("In function fnNewMatrix:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }
  longIntegerToUInt(tmp_lgInt, *rows);

  longIntegerFree(tmp_lgInt);
  return true;
}



static bool_t swapRowsReal(real34Matrix_t *matrix) {
  real_t ry, rx, rrows;
  uint16_t a, b;

  int32ToReal(matrix->header.matrixRows, &rrows);

  if((!getArg(REGISTER_Y, &ry)) || (!getArg(REGISTER_X, &rx))) return false;

  realToInt32(&ry, a);
  realToInt32(&rx, b);
  if(realIsPositive(&rx) && realIsPositive(&ry) && realCompareLessEqual(&rx, &rrows) && realCompareLessEqual(&ry, &rrows)) {
    if(!realCompareEqual(&ry, &rx))
      realMatrixSwapRows(matrix, matrix, a - 1, b - 1);
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "rows %" PRIu16 " and/or %" PRIu16 " out of range", a, b);
      moreInfoOnError("In function swapRowsReal:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }

  return true;
}

static bool_t swapRowsComplex(complex34Matrix_t *matrix) {
  real_t ry, rx, rrows;
  uint16_t a, b;

  int32ToReal(matrix->header.matrixRows, &rrows);

  if((!getArg(REGISTER_Y, &ry)) || (!getArg(REGISTER_X, &rx))) return false;

  realToInt32(&ry, a);
  realToInt32(&rx, b);
  if(realIsPositive(&rx) && realIsPositive(&ry) && realCompareLessEqual(&rx, &rrows) && realCompareLessEqual(&ry, &rrows)) {
    if(!realCompareEqual(&ry, &rx))
      complexMatrixSwapRows(matrix, matrix, a - 1, b - 1);
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "rows %" PRIu16 " and/or %" PRIu16 " out of range", a, b);
      moreInfoOnError("In function swapRowsComplex:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }

  return true;
}



static bool_t getMatrixReal(real34Matrix_t *matrix) {
  real_t ry, rx, rrows, rcols;
  uint16_t a, b, r, c;

  const int16_t i = getIRegisterAsInt(true);
  const int16_t j = getJRegisterAsInt(true);

  int32ToReal(matrix->header.matrixRows    - i, &rrows);
  int32ToReal(matrix->header.matrixColumns - j, &rcols);

  if((!getArg(REGISTER_Y, &ry)) || (!getArg(REGISTER_X, &rx))) return false;

  realToInt32(&ry, a);
  realToInt32(&rx, b);
  if(realIsPositive(&rx) && realIsPositive(&ry) && realCompareLessEqual(&rx, &rcols) && realCompareLessEqual(&ry, &rrows)) {
    real34Matrix_t mat;
    fnDropY(NOPARAM);
    realMatrixInit(&mat, a, b);
    for(r = 0; r < a; ++r)
      for(c = 0; c < b; ++c)
        real34Copy(&matrix->matrixElements[(r + i) * matrix->header.matrixColumns + c + j], &mat.matrixElements[r * b + c]);
    convertReal34MatrixToReal34MatrixRegister(&mat, REGISTER_X);
    realMatrixFree(&mat);
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "%" PRIu16 " " STD_CROSS " %" PRIu16 " out of range", a, b);
      moreInfoOnError("In function getMatrixReal:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }

  return false;
}

static bool_t getMatrixComplex(complex34Matrix_t *matrix) {
  real_t ry, rx, rrows, rcols;
  uint16_t a, b, r, c;

  const int16_t i = getIRegisterAsInt(true);
  const int16_t j = getJRegisterAsInt(true);

  int32ToReal(matrix->header.matrixRows    - i, &rrows);
  int32ToReal(matrix->header.matrixColumns - j, &rcols);

  if((!getArg(REGISTER_Y, &ry)) || (!getArg(REGISTER_X, &rx))) return false;

  realToInt32(&ry, a);
  realToInt32(&rx, b);
  if(realIsPositive(&rx) && realIsPositive(&ry) && realCompareLessEqual(&rx, &rcols) && realCompareLessEqual(&ry, &rrows)) {
    complex34Matrix_t mat;
    fnDropY(NOPARAM);
    complexMatrixInit(&mat, a, b);
    for(r = 0; r < a; ++r)
      for(c = 0; c < b; ++c)
        complex34Copy(&matrix->matrixElements[(r + i) * matrix->header.matrixColumns + c + j], &mat.matrixElements[r * b + c]);
    convertComplex34MatrixToComplex34MatrixRegister(&mat, REGISTER_X);
    complexMatrixFree(&mat);
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "%" PRIu16 " " STD_CROSS " %" PRIu16 " out of range", a, b);
      moreInfoOnError("In function getMatrixComplex:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }

  return false;
}



static bool_t putMatrixReal(real34Matrix_t *matrix) {
  uint16_t r, c;
  real34Matrix_t mat;

  const int16_t i = getIRegisterAsInt(true);
  const int16_t j = getJRegisterAsInt(true);

  if(getRegisterDataType(REGISTER_X) != dtReal34Matrix) {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "%s is not a real matrix", getRegisterDataTypeName(REGISTER_X, true, false));
      moreInfoOnError("In function putMatrixReal:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }

  linkToRealMatrixRegister(REGISTER_X, &mat);
  if((mat.header.matrixRows + i) <= matrix->header.matrixRows && (mat.header.matrixColumns + j) <= matrix->header.matrixColumns) {
    for(r = 0; r < mat.header.matrixRows; ++r)
      for(c = 0; c < mat.header.matrixColumns; ++c)
        real34Copy(&mat.matrixElements[r * mat.header.matrixColumns + c], &matrix->matrixElements[(r + i) * matrix->header.matrixColumns + c + j]);
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "%" PRIu16 " " STD_CROSS " %" PRIu16 " out of range", mat.header.matrixRows, mat.header.matrixColumns);
      moreInfoOnError("In function putMatrixReal:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }

  return true;
}

static bool_t putMatrixComplex(complex34Matrix_t *matrix) {
  uint16_t r, c;
  complex34Matrix_t mat;

  const int16_t i = getIRegisterAsInt(true);
  const int16_t j = getJRegisterAsInt(true);

  if(getRegisterDataType(REGISTER_X) != dtComplex34Matrix) {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "%s is not a complex matrix", getRegisterDataTypeName(REGISTER_X, true, false));
      moreInfoOnError("In function putMatrixComplex:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }

  linkToComplexMatrixRegister(REGISTER_X, &mat);
  if((mat.header.matrixRows + i) <= matrix->header.matrixRows && (mat.header.matrixColumns + j) <= matrix->header.matrixColumns) {
    for(r = 0; r < mat.header.matrixRows; ++r)
      for(c = 0; c < mat.header.matrixColumns; ++c)
        complex34Copy(&mat.matrixElements[r * mat.header.matrixColumns + c], &matrix->matrixElements[(r + i) * matrix->header.matrixColumns + c + j]);
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "%" PRIu16 " " STD_CROSS " %" PRIu16 " out of range", mat.header.matrixRows, mat.header.matrixColumns);
      moreInfoOnError("In function putMatrixComplex:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return false;
  }

  return true;
}



static bool_t incIReal(real34Matrix_t *matrix) {
  setIRegisterAsInt(true, getIRegisterAsInt(true) + 1);
  wrapIJ(matrix->header.matrixRows, matrix->header.matrixColumns);
  return false;
}

static bool_t decIReal(real34Matrix_t *matrix) {
  setIRegisterAsInt(true, getIRegisterAsInt(true) - 1);
  wrapIJ(matrix->header.matrixRows, matrix->header.matrixColumns);
  return false;
}

static bool_t incJReal(real34Matrix_t *matrix) {
  setJRegisterAsInt(true, getJRegisterAsInt(true) + 1);
  if(wrapIJ(matrix->header.matrixRows, matrix->header.matrixColumns)) {
    insRowRealMatrix(matrix, matrix->header.matrixRows);
    return true;
  }
  else {
    return false;
  }
}

static bool_t decJReal(real34Matrix_t *matrix) {
  setJRegisterAsInt(true, getJRegisterAsInt(true) - 1);
  wrapIJ(matrix->header.matrixRows, matrix->header.matrixColumns);
  return false;
}

static bool_t incIComplex(complex34Matrix_t *matrix) {
  return incIReal((real34Matrix_t *)matrix);
}

static bool_t decIComplex(complex34Matrix_t *matrix) {
  return decIReal((real34Matrix_t *)matrix);
}

static bool_t incJComplex(complex34Matrix_t *matrix) {
  setJRegisterAsInt(true, getJRegisterAsInt(true) + 1);
  if(wrapIJ(matrix->header.matrixRows, matrix->header.matrixColumns)) {
    insRowComplexMatrix(matrix, matrix->header.matrixRows);
    return true;
  }
  else {
    return false;
  }
}

static bool_t decJComplex(complex34Matrix_t *matrix) {
  return decJReal((real34Matrix_t *)matrix);
}



static void initSimEqValue(calcRegister_t regist, uint16_t rows, uint16_t cols) {
  if(getRegisterDataType(regist) != dtReal34Matrix && getRegisterDataType(regist) != dtComplex34Matrix) {
    real34Matrix_t matrix;
    realMatrixInit(&matrix, rows, cols);
    convertReal34MatrixToReal34MatrixRegister(&matrix, regist);
    realMatrixFree(&matrix);
  }
  else if(getRegisterDataType(regist) == dtReal34Matrix && (REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixRows != rows || REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixColumns != cols)) {
    real34Matrix_t matrix;
    convertReal34MatrixRegisterToReal34Matrix(regist, &matrix);
    realMatrixRedim(&matrix, rows, cols);
    convertReal34MatrixToReal34MatrixRegister(&matrix, regist);
    realMatrixFree(&matrix);
  }
  else if(getRegisterDataType(regist) == dtComplex34Matrix && (REGISTER_COMPLEX34_MATRIX_DBLOCK(regist)->matrixRows != rows || REGISTER_COMPLEX34_MATRIX_DBLOCK(regist)->matrixColumns != cols)) {
    complex34Matrix_t matrix;
    convertComplex34MatrixRegisterToComplex34Matrix(regist, &matrix);
    complexMatrixRedim(&matrix, rows, cols);
    convertComplex34MatrixToComplex34MatrixRegister(&matrix, regist);
    complexMatrixFree(&matrix);
  }
}
#endif // TESTSUITE_BUILD



void fnNewMatrix(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  uint32_t rows, cols;
  real34Matrix_t matrix;

  if(!getDimensionArg(&rows, &cols)) return;

  //Initialize Memory for Matrix
  realMatrixInit(&matrix, rows, cols);

  //Drop X_Register and Y_Register
  fnDropY(NOPARAM);
  convertReal34MatrixToReal34MatrixRegister(&matrix, REGISTER_X);
  setSystemFlag(FLAG_ASLIFT);

  realMatrixFree(&matrix);
#endif // TESTSUITE_BUILD
}

void fnEditMatrix(uint16_t regist) {
#ifndef TESTSUITE_BUILD
  const uint16_t reg = (regist == NOPARAM) ? REGISTER_X : regist;
  if((getRegisterDataType(reg) == dtReal34Matrix) || (getRegisterDataType(reg) == dtComplex34Matrix)) {
    calcMode = CM_MIM;
    matrixIndex = reg;

    getMatrixFromRegister(reg);

    setIRegisterAsInt(true, 0);
    setJRegisterAsInt(true, 0);
    aimBuffer[0] = 0;
    nimBufferDisplay[0] = 0;
    scrollRow = scrollColumn = 0;
    showMatrixEditor();
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(reg));
    moreInfoOnError("In function fnEditMatrix:", errorMessage, "is not a matrix.", "");
    #endif
  }
#endif // TESTSUITE_BUILD
}



void fnOldMatrix(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  if(calcMode == CM_MIM) {
    aimBuffer[0] = 0;
    nimBufferDisplay[0] = 0;
    hideCursor();
    cursorEnabled = false;

    if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
      if(openMatrixMIMPointer.realMatrix.matrixElements) realMatrixFree(&openMatrixMIMPointer.realMatrix);
      convertReal34MatrixRegisterToReal34Matrix(matrixIndex, &openMatrixMIMPointer.realMatrix);
    }
    else {
      if(openMatrixMIMPointer.complexMatrix.matrixElements) complexMatrixFree(&openMatrixMIMPointer.complexMatrix);
      convertComplex34MatrixRegisterToComplex34Matrix(matrixIndex, &openMatrixMIMPointer.complexMatrix);
    }
  }
  else {
    displayCalcErrorMessage(ERROR_OPERATION_UNDEFINED, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "works in MIM only");
    moreInfoOnError("In function fnOldMatrix:", errorMessage, NULL, NULL);
    #endif
  }
#endif // TESTSUITE_BUILD
}



void fnGoToElement(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  if(calcMode == CM_MIM) {
    mimEnter(false);
    runFunction(ITM_M_GOTO_ROW);
  }
  else {
    displayCalcErrorMessage(ERROR_OPERATION_UNDEFINED, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
      sprintf(errorMessage, "works in MIM only");
      moreInfoOnError("In function fnGoToElement:", errorMessage, NULL, NULL);
    #endif
  }
#endif // TESTSUITE_BUILD
}

void fnGoToRow(uint16_t row) {
#ifndef TESTSUITE_BUILD
  if(calcMode == CM_MIM) {
    tmpRow = row;
  }
  else {
    displayCalcErrorMessage(ERROR_OPERATION_UNDEFINED, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
      sprintf(errorMessage, "works in MIM only");
      moreInfoOnError("In function fnGoToRow:", errorMessage, NULL, NULL);
    #endif
  }
#endif // TESTSUITE_BUILD
}

void fnGoToColumn(uint16_t col) {
#ifndef TESTSUITE_BUILD
  if(calcMode == CM_MIM) {
    if(tmpRow == 0 || tmpRow > openMatrixMIMPointer.header.matrixRows || col == 0 || col > openMatrixMIMPointer.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "(%" PRIu16 ", %" PRIu16 ") out of range", tmpRow, col);
        moreInfoOnError("In function putGoToColumn:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
        convertReal34MatrixToReal34MatrixRegister(&openMatrixMIMPointer.realMatrix, matrixIndex);
      }
      else {
        convertComplex34MatrixToComplex34MatrixRegister(&openMatrixMIMPointer.complexMatrix, matrixIndex);
      }
      setIRegisterAsInt(false, tmpRow);
      setJRegisterAsInt(false, col);
    }
  }
  else {
    displayCalcErrorMessage(ERROR_OPERATION_UNDEFINED, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
      sprintf(errorMessage, "works in MIM only");
      moreInfoOnError("In function fnGoToColumn:", errorMessage, NULL, NULL);
    #endif
  }
#endif // TESTSUITE_BUILD
}



void fnSetGrowMode(uint16_t growFlag) {
  if(growFlag) {
    setSystemFlag(FLAG_GROW);
  }
  else {
    clearSystemFlag(FLAG_GROW);
  }
}



void fnIncDecI(uint16_t mode) {
#ifndef TESTSUITE_BUILD
  callByIndexedMatrix((mode == DEC_FLAG) ? decIReal : incIReal, (mode == DEC_FLAG) ? decIComplex : incIComplex);
#endif // TESTSUITE_BUILD
}



void fnIncDecJ(uint16_t mode) {
#ifndef TESTSUITE_BUILD
  callByIndexedMatrix((mode == DEC_FLAG) ? decJReal : incJReal, (mode == DEC_FLAG) ? decJComplex : incJComplex);
#endif // TESTSUITE_BUILD
}



void fnInsRow(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  if(calcMode == CM_MIM) {
    mimEnter(false);
    if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
      insRowRealMatrix(&openMatrixMIMPointer.realMatrix, getIRegisterAsInt(true));
    }
    else {
      insRowComplexMatrix(&openMatrixMIMPointer.complexMatrix, getIRegisterAsInt(true));
    }
    mimEnter(true);
  }
  else {
    displayCalcErrorMessage(ERROR_OPERATION_UNDEFINED, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
      sprintf(errorMessage, "works in MIM only");
      moreInfoOnError("In function fnGoToElement:", errorMessage, NULL, NULL);
    #endif
  }
#endif // TESTSUITE_BUILD
}



void fnDelRow(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  if(calcMode == CM_MIM) {
    mimEnter(false);
    if(openMatrixMIMPointer.header.matrixRows > 1) {
      if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
        delRowRealMatrix(&openMatrixMIMPointer.realMatrix, getIRegisterAsInt(true));
      }
      else {
        delRowComplexMatrix(&openMatrixMIMPointer.complexMatrix, getIRegisterAsInt(true));
      }
    }
    mimEnter(true);
  }
  else {
    displayCalcErrorMessage(ERROR_OPERATION_UNDEFINED, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
      sprintf(errorMessage, "works in MIM only");
      moreInfoOnError("In function fnGoToElement:", errorMessage, NULL, NULL);
    #endif
  }
#endif // TESTSUITE_BUILD
}



void fnSetMatrixDimensions(uint16_t regist) {
#ifndef TESTSUITE_BUILD
  uint32_t y, x;

  if(!getDimensionArg(&y, &x)) {
  }
  else if(getRegisterDataType(regist) == dtReal34Matrix) {
    real34Matrix_t matrix;

    convertReal34MatrixRegisterToReal34Matrix(regist, &matrix);
    realMatrixRedim(&matrix, y, x);
    convertReal34MatrixToReal34MatrixRegister(&matrix, regist);
    realMatrixFree(&matrix);
  }
  else if(getRegisterDataType(regist) == dtComplex34Matrix) {
    complex34Matrix_t matrix;

    convertComplex34MatrixRegisterToComplex34Matrix(regist, &matrix);
    complexMatrixRedim(&matrix, y, x);
    convertComplex34MatrixToComplex34MatrixRegister(&matrix, regist);
    complexMatrixFree(&matrix);
  }
  else {
    real34Matrix_t matrix;

    realMatrixInit(&matrix, y, x);
    convertReal34MatrixToReal34MatrixRegister(&matrix, regist);
    realMatrixFree(&matrix);
  }
#endif // TESTSUITE_BUILD
}




void fnGetMatrixDimensions(uint16_t unusedButMandatoryParameter) {
#ifndef TESTSUITE_BUILD
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix || getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    const uint16_t rows = REGISTER_DATA(REGISTER_X)->matrixRows;
    const uint16_t cols = REGISTER_DATA(REGISTER_X)->matrixColumns;
    longInteger_t li;

    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    liftStack();
    reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);

    longIntegerInit(li);
    uIntToLongInteger(rows, li);
    convertLongIntegerToLongIntegerRegister(li, REGISTER_Y);
    uIntToLongInteger(cols, li);
    convertLongIntegerToLongIntegerRegister(li, REGISTER_X);
    longIntegerFree(li);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnGetMatrixDimensions:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
#endif // TESTSUITE_BUILD
}




void fnTranspose(uint16_t unusedButMandatoryParameter) {
#ifndef TESTSUITE_BUILD
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
    real34Matrix_t x, res;

    linkToRealMatrixRegister(REGISTER_X, &x);
    transposeRealMatrix(&x, &res);
    convertReal34MatrixToReal34MatrixRegister(&res, REGISTER_X);
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    complex34Matrix_t x, res;

    linkToComplexMatrixRegister(REGISTER_X, &x);
    transposeComplexMatrix(&x, &res);
    convertComplex34MatrixToComplex34MatrixRegister(&res, REGISTER_X);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnTranspose:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
#endif // TESTSUITE_BUILD
}




void fnLuDecomposition(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
    real34Matrix_t x, res;
    uint16_t *p;

    convertReal34MatrixRegisterToReal34Matrix(REGISTER_X, &x);

    if(x.header.matrixRows != x.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "not a square matrix (%d" STD_CROSS "%d)",
                x.header.matrixRows, x.header.matrixColumns);
        moreInfoOnError("In function fnLuDecomposition:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      p = allocWp43s(x.header.matrixRows * sizeof(uint16_t));
      WP34S_LU_decomposition(&x, &res, p);
      if(res.matrixElements) {
        realMatrixFree(&x);
        realMatrixIdentity(&x, res.header.matrixColumns);
        for(uint16_t i = 0; i < res.header.matrixColumns; ++i) {
          realMatrixSwapRows(&x, &x, i, p[i]);
        }
        liftStack();
        convertReal34MatrixToReal34MatrixRegister(&res, REGISTER_X);
        convertReal34MatrixToReal34MatrixRegister(&x, REGISTER_Y);
        realMatrixFree(&res);
        setSystemFlag(FLAG_ASLIFT);
      }
      else {
        displayCalcErrorMessage(ERROR_SINGULAR_MATRIX, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "attempt to LU-decompose a singular matrix");
          moreInfoOnError("In function fnLuDecomposition:", errorMessage, NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      }
      freeWp43s(p, x.header.matrixRows * sizeof(uint16_t));
    }

    realMatrixFree(&x);
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    complex34Matrix_t x, res;
    real34Matrix_t pivot;
    uint16_t *p;

    convertComplex34MatrixRegisterToComplex34Matrix(REGISTER_X, &x);

    if(x.header.matrixRows != x.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "not a square matrix (%d" STD_CROSS "%d)",
                x.header.matrixRows, x.header.matrixColumns);
        moreInfoOnError("In function fnLuDecomposition:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      p = allocWp43s(x.header.matrixRows * sizeof(uint16_t));
      complex_LU_decomposition(&x, &res, p);
      if(res.matrixElements) {
        realMatrixIdentity(&pivot, res.header.matrixColumns);
        for(uint16_t i = 0; i < res.header.matrixColumns; ++i) {
          realMatrixSwapRows(&pivot, &pivot, i, p[i]);
        }
        liftStack();
        convertComplex34MatrixToComplex34MatrixRegister(&res, REGISTER_X);
        convertReal34MatrixToReal34MatrixRegister(&pivot, REGISTER_Y);
        complexMatrixFree(&res);
        realMatrixFree(&pivot);
        setSystemFlag(FLAG_ASLIFT);
      }
      else {
        displayCalcErrorMessage(ERROR_SINGULAR_MATRIX, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "attempt to LU-decompose a singular matrix");
          moreInfoOnError("In function fnLuDecomposition:", errorMessage, NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      }
      freeWp43s(p, x.header.matrixRows * sizeof(uint16_t));
    }

    complexMatrixFree(&x);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnLuDecomposition:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
#endif // TESTSUITE_BUILD
}



void fnDeterminant(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
    real34Matrix_t x;
    real34_t res;

    linkToRealMatrixRegister(REGISTER_X, &x);

    if(x.header.matrixRows != x.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "not a square matrix (%d" STD_CROSS "%d)",
                x.header.matrixRows, x.header.matrixColumns);
        moreInfoOnError("In function fnDeterminant:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      detRealMatrix(&x, &res);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      real34Copy(&res, REGISTER_REAL34_DATA(REGISTER_X));
    }
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    complex34Matrix_t x;
    real34_t res_r, res_i;

    linkToComplexMatrixRegister(REGISTER_X, &x);

    if(x.header.matrixRows != x.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "not a square matrix (%d" STD_CROSS "%d)",
                x.header.matrixRows, x.header.matrixColumns);
        moreInfoOnError("In function fnDeterminant:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      detComplexMatrix(&x, &res_r, &res_i);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
      real34Copy(&res_r, REGISTER_REAL34_DATA(REGISTER_X));
      real34Copy(&res_i, REGISTER_IMAG34_DATA(REGISTER_X));
    }
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnLuDecomposition:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
#endif // TESTSUITE_BUILD
}



void fnInvertMatrix(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
    real34Matrix_t x, res;

    linkToRealMatrixRegister(REGISTER_X, &x);

    if(x.header.matrixRows != x.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "not a square matrix (%d" STD_CROSS "%d)",
                x.header.matrixRows, x.header.matrixColumns);
        moreInfoOnError("In function fnInvertMatrix:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      WP34S_matrix_inverse(&x, &res);
      if(res.matrixElements) {
        convertReal34MatrixToReal34MatrixRegister(&res, REGISTER_X);
        realMatrixFree(&res);
        setSystemFlag(FLAG_ASLIFT);
      }
      else {
        displayCalcErrorMessage(ERROR_SINGULAR_MATRIX, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "attempt to invert a singular matrix");
          moreInfoOnError("In function fnInvertMatrix:", errorMessage, NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      }
    }
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    complex34Matrix_t x, res;

    linkToComplexMatrixRegister(REGISTER_X, &x);

    if(x.header.matrixRows != x.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "not a square matrix (%d" STD_CROSS "%d)",
                x.header.matrixRows, x.header.matrixColumns);
        moreInfoOnError("In function fnInvertMatrix:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      complex_matrix_inverse(&x, &res);
      if(res.matrixElements) {
        convertComplex34MatrixToComplex34MatrixRegister(&res, REGISTER_X);
        complexMatrixFree(&res);
        setSystemFlag(FLAG_ASLIFT);
      }
      else {
        displayCalcErrorMessage(ERROR_SINGULAR_MATRIX, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "attempt to invert a singular matrix");
          moreInfoOnError("In function fnInvertMatrix:", errorMessage, NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      }
    }
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnInvertMatrix:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
#endif // TESTSUITE_BUILD
}



void fnEuclideanNorm(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
    real34Matrix_t matrix;
    real_t elem, sum;

    linkToRealMatrixRegister(REGISTER_X, &matrix);

    realZero(&sum);
    for(int i = 0; i < matrix.header.matrixRows * matrix.header.matrixColumns; ++i) {
      real34ToReal(&matrix.matrixElements[i], &elem);
      realMultiply(&elem, &elem, &elem, &ctxtReal39);
      realAdd(&sum, &elem, &sum, &ctxtReal39);
    }
    realSquareRoot(&sum, &sum, &ctxtReal39);

    // `matrix` invalidates here
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&sum, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    complex34Matrix_t matrix;
    real_t elem, sum;

    linkToComplexMatrixRegister(REGISTER_X, &matrix);

    realZero(&sum);
    for(int i = 0; i < matrix.header.matrixRows * matrix.header.matrixColumns; ++i) {
      real34ToReal(VARIABLE_REAL34_DATA(&matrix.matrixElements[i]), &elem);
      realMultiply(&elem, &elem, &elem, &ctxtReal39);
      realAdd(&sum, &elem, &sum, &ctxtReal39);
      real34ToReal(VARIABLE_IMAG34_DATA(&matrix.matrixElements[i]), &elem);
      realMultiply(&elem, &elem, &elem, &ctxtReal39);
      realAdd(&sum, &elem, &sum, &ctxtReal39);
    }
    realSquareRoot(&sum, &sum, &ctxtReal39);

    // `matrix` invalidates here
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&sum, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnInvertMatrix:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
#endif // TESTSUITE_BUILD
}



void fnRowSum(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
    real34Matrix_t x, res;
    real_t sum, elem;
    linkToRealMatrixRegister(REGISTER_X, &x);

    realMatrixInit(&res, x.header.matrixRows, 1);
    for(uint16_t i = 0; i < x.header.matrixRows; ++i) {
      realZero(&sum);
      for(uint16_t j = 0; j < x.header.matrixColumns; ++j) {
        real34ToReal(&x.matrixElements[i * x.header.matrixColumns + j], &elem);
        realAdd(&sum, &elem, &sum, &ctxtReal39);
      }
      realToReal34(&sum, &res.matrixElements[i]);
    }

    convertReal34MatrixToReal34MatrixRegister(&res, REGISTER_X);
    realMatrixFree(&res);
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    complex34Matrix_t x, res;
    real_t sumr, sumi, elem;
    linkToComplexMatrixRegister(REGISTER_X, &x);

    complexMatrixInit(&res, x.header.matrixRows, 1);
    for(uint16_t i = 0; i < x.header.matrixRows; ++i) {
      realZero(&sumr); realZero(&sumi);
      for(uint16_t j = 0; j < x.header.matrixColumns; ++j) {
        real34ToReal(VARIABLE_REAL34_DATA(&x.matrixElements[i * x.header.matrixColumns + j]), &elem);
        realAdd(&sumr, &elem, &sumr, &ctxtReal39);
        real34ToReal(VARIABLE_IMAG34_DATA(&x.matrixElements[i * x.header.matrixColumns + j]), &elem);
        realAdd(&sumi, &elem, &sumi, &ctxtReal39);
      }
      realToReal34(&sumr, VARIABLE_REAL34_DATA(&res.matrixElements[i]));
      realToReal34(&sumi, VARIABLE_IMAG34_DATA(&res.matrixElements[i]));
    }

    convertComplex34MatrixToComplex34MatrixRegister(&res, REGISTER_X);
    complexMatrixFree(&res);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnInvertMatrix:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
#endif // TESTSUITE_BUILD
}



void fnRowNorm(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  if(getRegisterDataType(REGISTER_X) == dtReal34Matrix) {
    real34Matrix_t x;
    real_t norm, sum, elem;
    linkToRealMatrixRegister(REGISTER_X, &x);

    realZero(&norm);
    for(uint16_t i = 0; i < x.header.matrixRows; ++i) {
      realZero(&sum);
      for(uint16_t j = 0; j < x.header.matrixColumns; ++j) {
        real34ToReal(&x.matrixElements[i * x.header.matrixColumns + j], &elem);
        realSetPositiveSign(&elem);
        realAdd(&sum, &elem, &sum, &ctxtReal39);
      }
      if(realCompareGreaterThan(&sum, &norm))
        realCopy(&sum, &norm);
    }

    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&norm, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else if(getRegisterDataType(REGISTER_X) == dtComplex34Matrix) {
    complex34Matrix_t x;
    real_t norm, sum, elem, imag;
    linkToComplexMatrixRegister(REGISTER_X, &x);

    realZero(&norm);
    for(uint16_t i = 0; i < x.header.matrixRows; ++i) {
      realZero(&sum);
      for(uint16_t j = 0; j < x.header.matrixColumns; ++j) {
        real34ToReal(VARIABLE_REAL34_DATA(&x.matrixElements[i * x.header.matrixColumns + j]), &elem);
        real34ToReal(VARIABLE_IMAG34_DATA(&x.matrixElements[i * x.header.matrixColumns + j]), &imag);
        realMultiply(&elem, &elem, &elem, &ctxtReal39);
        realMultiply(&imag, &imag, &imag, &ctxtReal39);
        realAdd(&elem, &imag, &elem, &ctxtReal39);
        realSquareRoot(&elem, &elem, &ctxtReal39);
        realAdd(&sum, &elem, &sum, &ctxtReal39);
      }
      if(realCompareGreaterThan(&sum, &norm))
        realCopy(&sum, &norm);
    }

    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    realToReal34(&norm, REGISTER_REAL34_DATA(REGISTER_X));
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnInvertMatrix:", errorMessage, "is not a matrix.", "");
    #endif
  }

  adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
#endif // TESTSUITE_BUILD
}



void fnIndexMatrix(uint16_t regist) {
#ifndef TESTSUITE_BUILD
  if((getRegisterDataType(regist) == dtReal34Matrix) || (getRegisterDataType(regist) == dtComplex34Matrix)) {
    matrixIndex = regist;
    setIRegisterAsInt(false, 1);
    setJRegisterAsInt(false, 1);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(regist));
    moreInfoOnError("In function fnIndexMatrix:", errorMessage, "is not a matrix.", "");
    #endif
  }
#endif // TESTSUITE_BUILD
}



void fnGetMatrix(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  callByIndexedMatrix(getMatrixReal, getMatrixComplex);
#endif // TESTSUITE_BUILD
}



void fnPutMatrix(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  callByIndexedMatrix(putMatrixReal, putMatrixComplex);
#endif // TESTSUITE_BUILD
}



void fnSwapRows(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  callByIndexedMatrix(swapRowsReal, swapRowsComplex);
#endif // TESTSUITE_BUILD
}



void fnSimultaneousLinearEquation(uint16_t numberOfUnknowns) {
#ifndef TESTSUITE_BUILD
  initSimEqValue(findOrAllocateNamedVariable("Mat_A"), numberOfUnknowns, numberOfUnknowns);
  initSimEqValue(findOrAllocateNamedVariable("Mat_B"), numberOfUnknowns, 1);
  initSimEqValue(findOrAllocateNamedVariable("Mat_X"), numberOfUnknowns, 1);
  showSoftmenu(-MNU_SIMQ);
  showSoftmenu(-MNU_TAM);
  numberOfTamMenusToPop = 1;
#endif // TESTSUITE_BUILD
}

void fnEditLinearEquationMatrixA(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  fnEditMatrix(findNamedVariable("Mat_A"));
#endif // TESTSUITE_BUILD
}

void fnEditLinearEquationMatrixB(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  fnEditMatrix(findNamedVariable("Mat_B"));
#endif // TESTSUITE_BUILD
}

void fnEditLinearEquationMatrixX(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  if(findNamedVariable("Mat_A") == INVALID_VARIABLE || findNamedVariable("Mat_B") == INVALID_VARIABLE || findNamedVariable("Mat_X") == INVALID_VARIABLE) {
    displayCalcErrorMessage(ERROR_UNDEF_SOURCE_VAR, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "At least one of Mat_A, Mat_B or Mat_X is missing");
      moreInfoOnError("In function fnEditLinearEquationMatrixX:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else if(getRegisterDataType(findNamedVariable("Mat_A")) == dtReal34Matrix && getRegisterDataType(findNamedVariable("Mat_B")) == dtReal34Matrix) {
    real34Matrix_t a, b, x;
    linkToRealMatrixRegister(findNamedVariable("Mat_A"), &a);
    linkToRealMatrixRegister(findNamedVariable("Mat_B"), &b);
    WP34S_matrix_linear_eqn(&a, &b, &x);
    if(x.matrixElements) {
      convertReal34MatrixToReal34MatrixRegister(&x, findNamedVariable("Mat_X"));
      realMatrixFree(&x);
    }
  }
  else if(getRegisterDataType(findNamedVariable("Mat_A")) == dtReal34Matrix && getRegisterDataType(findNamedVariable("Mat_B")) == dtComplex34Matrix) {
    complex34Matrix_t a, b, x;
    convertReal34MatrixRegisterToComplex34Matrix(findNamedVariable("Mat_A"), &a);
    linkToComplexMatrixRegister(findNamedVariable("Mat_B"), &b);
    complex_matrix_linear_eqn(&a, &b, &x);
    if(x.matrixElements) {
      convertComplex34MatrixToComplex34MatrixRegister(&x, findNamedVariable("Mat_X"));
      complexMatrixFree(&x);
    }
    complexMatrixFree(&a);
  }
  else if(getRegisterDataType(findNamedVariable("Mat_A")) == dtComplex34Matrix && getRegisterDataType(findNamedVariable("Mat_B")) == dtReal34Matrix) {
    complex34Matrix_t a, b, x;
    linkToComplexMatrixRegister(findNamedVariable("Mat_A"), &a);
    convertReal34MatrixRegisterToComplex34Matrix(findNamedVariable("Mat_B"), &b);
    complex_matrix_linear_eqn(&a, &b, &x);
    if(x.matrixElements) {
      convertComplex34MatrixToComplex34MatrixRegister(&x, findNamedVariable("Mat_X"));
      complexMatrixFree(&x);
    }
    complexMatrixFree(&b);
  }
  else if(getRegisterDataType(findNamedVariable("Mat_A")) == dtComplex34Matrix && getRegisterDataType(findNamedVariable("Mat_B")) == dtComplex34Matrix) {
    complex34Matrix_t a, b, x;
    linkToComplexMatrixRegister(findNamedVariable("Mat_A"), &a);
    linkToComplexMatrixRegister(findNamedVariable("Mat_B"), &b);
    complex_matrix_linear_eqn(&a, &b, &x);
    if(x.matrixElements) {
      convertComplex34MatrixToComplex34MatrixRegister(&x, findNamedVariable("Mat_X"));
      complexMatrixFree(&x);
    }
  }
  if(lastErrorCode == ERROR_NONE) {
    liftStack();
    copySourceRegisterToDestRegister(findNamedVariable("Mat_X"), REGISTER_X);
    popSoftmenu();
  }
#endif // TESTSUITE_BUILD
}



#ifndef TESTSUITE_BUILD
void realMatrixInit(real34Matrix_t *matrix, uint16_t rows, uint16_t cols) {
  //Allocate Memory for Matrix
  matrix->matrixElements = allocWp43s(TO_BLOCKS((rows * cols) * sizeof(real34_t)));

  matrix->header.matrixColumns = cols;
  matrix->header.matrixRows = rows;

  //Initialize with 0.
  for(uint32_t i = 0; i < rows * cols; i++) {
    real34Copy(const34_0, &matrix->matrixElements[i]);
  }
}


void realMatrixFree(real34Matrix_t *matrix) {
  uint16_t cols = matrix->header.matrixColumns;
  uint16_t rows = matrix->header.matrixRows;

  freeWp43s(matrix->matrixElements, TO_BLOCKS((rows * cols) * sizeof(real34_t)));
  matrix->matrixElements = NULL;
  matrix->header.matrixRows = matrix->header.matrixColumns = 0;
}


void realMatrixIdentity(real34Matrix_t *matrix, uint16_t size) {
  realMatrixInit(matrix, size, size);
  for(uint16_t i = 0; i < size; ++i) {
    real34Copy(const34_1, &matrix->matrixElements[i * size + i]);
  }
}


void realMatrixRedim(real34Matrix_t *matrix, uint16_t rows, uint16_t cols) {
  real34Matrix_t newMatrix;
  uint32_t elements;

  realMatrixInit(&newMatrix, rows, cols);
  elements = matrix->header.matrixRows * matrix->header.matrixColumns;
  if(elements > rows * cols) elements = rows * cols;
  for(uint32_t i = 0; i < elements; ++i)
    real34Copy(&matrix->matrixElements[i], &newMatrix.matrixElements[i]);
  realMatrixFree(matrix);
  matrix->header.matrixRows = newMatrix.header.matrixRows;
  matrix->header.matrixColumns = newMatrix.header.matrixColumns;
  matrix->matrixElements = newMatrix.matrixElements;
}


void complexMatrixInit(complex34Matrix_t *matrix, uint16_t rows, uint16_t cols) {
  //Allocate Memory for Matrix
  matrix->matrixElements = allocWp43s(TO_BLOCKS((rows * cols) * sizeof(complex34_t)));

  matrix->header.matrixColumns = cols;
  matrix->header.matrixRows = rows;

  //Initialize with 0.
  for(uint32_t i = 0; i < rows * cols; i++) {
    real34Copy(const34_0, VARIABLE_REAL34_DATA(&matrix->matrixElements[i]));
    real34Copy(const34_0, VARIABLE_IMAG34_DATA(&matrix->matrixElements[i]));
  }
}


void complexMatrixFree(complex34Matrix_t *matrix) {
  uint16_t cols = matrix->header.matrixColumns;
  uint16_t rows = matrix->header.matrixRows;

  freeWp43s(matrix->matrixElements, TO_BLOCKS((rows * cols) * sizeof(complex34_t)));
  matrix->matrixElements = NULL;
  matrix->header.matrixRows = matrix->header.matrixColumns = 0;
}


void complexMatrixIdentity(complex34Matrix_t *matrix, uint16_t size) {
  complexMatrixInit(matrix, size, size);
  for(uint16_t i = 0; i < size; ++i) {
    real34Copy(const34_1, VARIABLE_REAL34_DATA(&matrix->matrixElements[i * size + i]));
    real34Copy(const34_0, VARIABLE_IMAG34_DATA(&matrix->matrixElements[i * size + i]));
  }
}


void complexMatrixRedim(complex34Matrix_t *matrix, uint16_t rows, uint16_t cols) {
  complex34Matrix_t newMatrix;
  uint32_t elements;

  complexMatrixInit(&newMatrix, rows, cols);
  elements = matrix->header.matrixRows * matrix->header.matrixColumns;
  if(elements > rows * cols) elements = rows * cols;
  for(uint32_t i = 0; i < elements; ++i) {
    complex34Copy(&matrix->matrixElements[i], &newMatrix.matrixElements[i]);
  }
  complexMatrixFree(matrix);
  matrix->header.matrixRows = newMatrix.header.matrixRows;
  matrix->header.matrixColumns = newMatrix.header.matrixColumns;
  matrix->matrixElements = newMatrix.matrixElements;
}


void showMatrixEditor() {
  int rows = openMatrixMIMPointer.header.matrixRows;
  int cols = openMatrixMIMPointer.header.matrixColumns;
  int16_t width = 0;

  if(softmenuStack[0].firstItem != -MNU_M_EDIT)
    showSoftmenu(-MNU_M_EDIT);

  bool_t colVector = false;
  if(cols == 1 && rows > 1) {
    colVector = true;
    cols = rows;
    rows = 1;
  }

  if(wrapIJ(colVector ? cols : rows, colVector ? 1 : cols)) {
    if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
      insRowRealMatrix(&openMatrixMIMPointer.realMatrix, rows);
      convertReal34MatrixToReal34MatrixRegister(&openMatrixMIMPointer.realMatrix, matrixIndex);
    }
    else {
      insRowComplexMatrix(&openMatrixMIMPointer.complexMatrix, rows);
      convertComplex34MatrixToComplex34MatrixRegister(&openMatrixMIMPointer.complexMatrix, matrixIndex);
    }
  }

  int16_t matSelRow = colVector ? getJRegisterAsInt(true) : getIRegisterAsInt(true);
  int16_t matSelCol = colVector ? getIRegisterAsInt(true) : getJRegisterAsInt(true);

  if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
    if(matSelCol == 0 || cols <= 4)
      scrollColumn = 0;
    else if(matSelCol == cols)
      scrollColumn = matSelCol - 3;
    else if(matSelCol < scrollColumn)
      scrollColumn = matSelCol;
    else if(matSelCol > scrollColumn + 2)
      scrollColumn = matSelCol - 2;
  }
  else {
    if(matSelCol == 0 || cols <= 2)
      scrollColumn = 0;
    else if(matSelCol == cols)
      scrollColumn = matSelCol - 1;
    else if(matSelCol < scrollColumn)
      scrollColumn = matSelCol;
    else if(matSelCol > scrollColumn + 1)
      scrollColumn = matSelCol - 1;
  }

  if(matSelRow == 0 || rows <= 5) {
    scrollRow = 0;
  }
  else if(matSelRow == rows - 1) {
    scrollRow = matSelRow - 4;
  }
  else if(matSelRow < scrollRow + 1) {
    scrollRow = matSelRow - 1;
  }
  else if(matSelRow > scrollRow + 3) {
    scrollRow = matSelRow - 3;
  }

  if(getRegisterDataType(matrixIndex) == dtReal34Matrix)
    showRealMatrix(&openMatrixMIMPointer.realMatrix);
  else
    showComplexMatrix(&openMatrixMIMPointer.complexMatrix);

  sprintf(tmpString, "%" PRIi16";%" PRIi16"= %s", (int16_t)(colVector ? matSelCol+1 : matSelRow+1), (int16_t)(colVector ? 1 : matSelCol+1), nimBufferDisplay);
  width = stringWidth(tmpString, &numericFont, true, true) + 1;
  if(aimBuffer[0] == 0) {
    if(getRegisterDataType(matrixIndex) == dtReal34Matrix)
      real34ToDisplayString(&openMatrixMIMPointer.realMatrix.matrixElements[matSelRow*cols+matSelCol], amNone, &tmpString[strlen(tmpString)], &numericFont, SCREEN_WIDTH - width, NUMBER_OF_DISPLAY_DIGITS, true, STD_SPACE_4_PER_EM);
    else
      complex34ToDisplayString(&openMatrixMIMPointer.complexMatrix.matrixElements[matSelRow*cols+matSelCol], &tmpString[strlen(tmpString)], &numericFont, SCREEN_WIDTH - width, NUMBER_OF_DISPLAY_DIGITS, true, STD_SPACE_4_PER_EM);

    showString(tmpString, &numericFont, 0, Y_POSITION_OF_NIM_LINE, vmNormal, true, false);
  }
  else {
    displayNim(tmpString, "", 0, 0);
  }
}

void mimEnter(bool_t commit) {
  int cols = openMatrixMIMPointer.header.matrixColumns;
  int16_t row = getIRegisterAsInt(true);
  int16_t col = getJRegisterAsInt(true);

  if(aimBuffer[0] != 0) {
    if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
      real34_t *real34Ptr = &openMatrixMIMPointer.realMatrix.matrixElements[row * cols + col];

      if(nimNumberPart == NP_FRACTION_DENOMINATOR) {
        closeNimWithFraction(real34Ptr);
      }
      else if(nimNumberPart == NP_COMPLEX_INT_PART || nimNumberPart == NP_COMPLEX_FLOAT_PART || nimNumberPart == NP_COMPLEX_EXPONENT) {
        complex34_t *complex34Ptr;
        complex34Matrix_t cxma;
        convertReal34MatrixToComplex34Matrix(&openMatrixMIMPointer.realMatrix, &cxma);
        realMatrixFree(&openMatrixMIMPointer.realMatrix);
        convertComplex34MatrixToComplex34MatrixRegister(&cxma, matrixIndex);
        openMatrixMIMPointer.complexMatrix.header.matrixRows = cxma.header.matrixRows;
        openMatrixMIMPointer.complexMatrix.header.matrixColumns = cxma.header.matrixColumns;
        openMatrixMIMPointer.complexMatrix.matrixElements = cxma.matrixElements;
        complex34Ptr = &openMatrixMIMPointer.complexMatrix.matrixElements[row * cols + col];
        closeNimWithComplex(VARIABLE_REAL34_DATA(complex34Ptr), VARIABLE_IMAG34_DATA(complex34Ptr));
      }
      else {
        stringToReal34(aimBuffer, real34Ptr);
      }
    }
    else {
      complex34_t *complex34Ptr = &openMatrixMIMPointer.complexMatrix.matrixElements[row * cols + col];

      if(nimNumberPart == NP_FRACTION_DENOMINATOR) {
        closeNimWithFraction(VARIABLE_REAL34_DATA(complex34Ptr));
        real34Zero(VARIABLE_IMAG34_DATA(complex34Ptr));
      }
      else if(nimNumberPart == NP_COMPLEX_INT_PART || nimNumberPart == NP_COMPLEX_FLOAT_PART || nimNumberPart == NP_COMPLEX_EXPONENT) {
        closeNimWithComplex(VARIABLE_REAL34_DATA(complex34Ptr), VARIABLE_IMAG34_DATA(complex34Ptr));
      }
      else {
        stringToReal34(aimBuffer, VARIABLE_REAL34_DATA(complex34Ptr));
        real34Zero(VARIABLE_IMAG34_DATA(complex34Ptr));
      }
    }

    aimBuffer[0] = 0;
    nimBufferDisplay[0] = 0;
    hideCursor();
    cursorEnabled = false;

    setSystemFlag(FLAG_ASLIFT);
  }
  if(commit) {
    if(getRegisterDataType(matrixIndex) == dtReal34Matrix)
      convertReal34MatrixToReal34MatrixRegister(&openMatrixMIMPointer.realMatrix, matrixIndex);
    else
      convertComplex34MatrixToComplex34MatrixRegister(&openMatrixMIMPointer.complexMatrix, matrixIndex);
  }
}

void mimAddNumber(int16_t item) {
  switch(item) {
    case ITM_EXPONENT :
      if(aimBuffer[0] == 0) {
        aimBuffer[0] = '+';
        aimBuffer[1] = '1';
        aimBuffer[2] = '.';
        aimBuffer[3] = 0;
        nimNumberPart = NP_REAL_FLOAT_PART;
        cursorEnabled = true;
        cursorFont = &numericFont;
        lastIntegerBase = 0;
      }
      break;

    case ITM_PERIOD :
      if(aimBuffer[0] == 0) {
        aimBuffer[0] = '+';
        aimBuffer[1] = '0';
        aimBuffer[2] = 0;
        nimNumberPart = NP_INT_10;
        cursorEnabled = true;
        cursorFont = &numericFont;
        lastIntegerBase = 0;
      }
      break;

    case ITM_0 :
    case ITM_1 :
    case ITM_2 :
    case ITM_3 :
    case ITM_4 :
    case ITM_5 :
    case ITM_6 :
    case ITM_7 :
    case ITM_8 :
    case ITM_9 :
      if(aimBuffer[0] == 0) {
        aimBuffer[0] = '+';
        aimBuffer[1] = 0;
        nimNumberPart = NP_INT_10;
        cursorEnabled = true;
        cursorFont = &numericFont;
        lastIntegerBase = 0;
      }
      break;

    case ITM_BACKSPACE :
      if(aimBuffer[0] == 0) {
        const int cols = openMatrixMIMPointer.header.matrixColumns;
        const int16_t row = getIRegisterAsInt(true);
        const int16_t col = getJRegisterAsInt(true);

        if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
          real34Zero(&openMatrixMIMPointer.realMatrix.matrixElements[row * cols + col]);
        }
        else {
          real34Zero(VARIABLE_REAL34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[row * cols + col]));
          real34Zero(VARIABLE_IMAG34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[row * cols + col]));
        }
        setSystemFlag(FLAG_ASLIFT);
        return;
      }
      else if((aimBuffer[0] == '+') && (aimBuffer[1] != 0) && (aimBuffer[2] == 0)) {
        aimBuffer[1] = 0;
        hideCursor();
        cursorEnabled = false;
      }
      break;

    case ITM_CHS :
      if(aimBuffer[0] == 0) {
        const int cols = openMatrixMIMPointer.header.matrixColumns;
        const int16_t row = getIRegisterAsInt(true);
        const int16_t col = getJRegisterAsInt(true);

        if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
          real34ChangeSign(&openMatrixMIMPointer.realMatrix.matrixElements[row * cols + col]);
        }
        else {
          real34ChangeSign(VARIABLE_REAL34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[row * cols + col]));
          real34ChangeSign(VARIABLE_IMAG34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[row * cols + col]));
        }
        setSystemFlag(FLAG_ASLIFT);
        return;
      }
      break;

    case ITM_CC :
      if(aimBuffer[0] == 0) return;
      break;

    default:
      return;
  }
  addItemToNimBuffer(item);
  calcMode = CM_MIM;
}

void mimFinalize(void) {
  if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
    if(openMatrixMIMPointer.realMatrix.matrixElements)
      realMatrixFree(&openMatrixMIMPointer.realMatrix);
  }
  else if(getRegisterDataType(matrixIndex) == dtComplex34Matrix) {
    if(openMatrixMIMPointer.complexMatrix.matrixElements)
      complexMatrixFree(&openMatrixMIMPointer.complexMatrix);
  }
  matrixIndex = INVALID_VARIABLE;
}

void mimRestore(void) {
  uint16_t idx = matrixIndex;
  mimFinalize();
  if(idx != INVALID_VARIABLE) {
    getMatrixFromRegister(idx);
    matrixIndex = idx;
  }
}


void showRealMatrix(const real34Matrix_t *matrix) {
  int rows = matrix->header.matrixRows;
  int cols = matrix->header.matrixColumns;
  int16_t Y_POS = Y_POSITION_OF_REGISTER_X_LINE;
  int16_t X_POS = 0;
  int16_t totalWidth = 0, width = 0;
  const font_t *font;
  int16_t fontHeight = NUMERIC_FONT_HEIGHT;
  int16_t maxWidth = MATRIX_LINE_WIDTH_LARGE * 3 - 20;
  int16_t colWidth[4] = {}, rPadWidth[20] = {};
  const bool_t forEditor = matrix == &openMatrixMIMPointer.realMatrix;
  const uint16_t sRow = forEditor ? scrollRow : 0;
  const uint16_t sCol = forEditor ? scrollColumn : 0;
  const uint16_t tmpDisplayFormat = displayFormat;
  const uint8_t tmpDisplayFormatDigits = displayFormatDigits;

  Y_POS = Y_POSITION_OF_REGISTER_X_LINE - NUMERIC_FONT_HEIGHT;

  bool_t colVector = false;
  if(cols == 1 && rows > 1) {
    colVector = true;
    cols = rows;
    rows = 1;
  }

  const int maxCols = cols > 4 ? 4 : cols;
  const int maxRows = rows > 5 ? 5 : rows;

  int16_t matSelRow = colVector ? getJRegisterAsInt(true) : getIRegisterAsInt(true);
  int16_t matSelCol = colVector ? getIRegisterAsInt(true) : getJRegisterAsInt(true);

  videoMode_t vm = vmNormal;

  if(forEditor) {
    clearRegisterLine(REGISTER_X, true, true);
    clearRegisterLine(REGISTER_Y, true, true);
    if(rows >= 2) clearRegisterLine(REGISTER_Z, true, true);
    if(rows >= 3) clearRegisterLine(REGISTER_T, true, true);
  }

  font = &numericFont;
  if((rows >= 4) || (cols >= 4) || (displayFormat != DF_ALL && displayFormatDigits > 3)) {
smallFont:
    font = &standardFont;
    fontHeight = STANDARD_FONT_HEIGHT;
    Y_POS = Y_POSITION_OF_REGISTER_X_LINE - STANDARD_FONT_HEIGHT + 2;
    maxWidth = MATRIX_LINE_WIDTH_SMALL * 4 - 20;
  }

  if(!forEditor) Y_POS += REGISTER_LINE_HEIGHT;
  const bool_t rightEllipsis = (cols > maxCols) && (cols > maxCols + sCol - 1);
  const bool_t leftEllipsis = (sCol > 0);
  int16_t digits;

  totalWidth = getRealMatrixColumnWidths(matrix, font, colWidth, rPadWidth, &digits);
  if(totalWidth > maxWidth) {
    if(font == &numericFont) {
      goto smallFont;
    }
    else {
      displayFormat = DF_SCI;
      displayFormatDigits = 3;
      getRealMatrixColumnWidths(matrix, font, colWidth, rPadWidth, &digits);
    }
  }
  int16_t baseWidth = (leftEllipsis ? stringWidth(STD_ELLIPSIS, font, true, true) : 0) +
    (rightEllipsis ? stringWidth(STD_ELLIPSIS, font, true, true) + 11 : 0);
  for(int j = 0; j< maxCols - (maxCols == cols ? 0 : 1); j++) baseWidth += colWidth[j] + 20;

  if(!forEditor) X_POS = SCREEN_WIDTH - 2 - ((colVector ? (font == &standardFont ? 12 : 18) + stringWidth(STD_SUP_T, font, true, true) : (font == &standardFont ? 5 : 10) + stringWidth("]", font, true, true)) + baseWidth);

  for(int i = 0; i < maxRows; i++) {
    int16_t colX = 20;
    showString((maxRows == 1) ? "[" : (i == 0) ? STD_MAT_TL : (i + 1 == maxRows) ? STD_MAT_BL : STD_MAT_ML, font, X_POS + 1, Y_POS - (maxRows -1 - i) * fontHeight, vmNormal, true, false);
    if(leftEllipsis) {
      showString(STD_ELLIPSIS, font, X_POS + 10, Y_POS - (maxRows -1 -i) * fontHeight, vmNormal, true, false);
      colX += stringWidth(STD_ELLIPSIS, font, true, true);
    }
    for(int j = 0; j< maxCols - ((cols > maxCols && !rightEllipsis) ? 1 : 0); j++) {
      if(((i == maxRows - 1) && (rows > maxRows + sRow)) || ((j == maxCols - 1) && rightEllipsis) || ((i == 0) && (sRow > 0))) {
        strcpy(tmpString, STD_ELLIPSIS);
        vm = vmNormal;
      }
      else {
        bool_t neg = real34IsNegative(&matrix->matrixElements[(i+sRow)*cols+j+sCol]);
        tmpString[0] = neg ? '-' : ' '; tmpString[1] = 0;
        real34SetPositiveSign(&matrix->matrixElements[(i+sRow)*cols+j+sCol]);
        real34ToDisplayString(&matrix->matrixElements[(i+sRow)*cols+j+sCol], amNone, &tmpString[strlen(tmpString)], font, colWidth[j], displayFormat == DF_ALL ? digits : 15, true, STD_SPACE_4_PER_EM);
        if(neg) real34SetNegativeSign(&matrix->matrixElements[(i+sRow)*cols+j+sCol]);
        if (forEditor && matSelRow == (i + sRow) && matSelCol == (j + sCol)) {
          const int16_t minusWidth = stringWidth("-", font, true, true);
          lcd_fill_rect(X_POS + 3 + colX - minusWidth, Y_POS - (maxRows -1 -i) * fontHeight, colWidth[j] + minusWidth + 3, font == &numericFont ? 32 : 20, 0xFF);
          vm = vmReverse;
        } else {
          vm = vmNormal;
        }
      }
      width = stringWidth(tmpString, font, true, true) + 1;
      showString(tmpString, font, X_POS + (font == &numericFont ? 8 : 5) + colX + (((j == maxCols - 1) && rightEllipsis) ? 8 - width : (colWidth[j] - width) - rPadWidth[i * 4 + j]), Y_POS - (maxRows -1 -i) * fontHeight, vm, true, false);
      colX += colWidth[j] + 20;
    }
    showString((maxRows == 1) ? "]" : (i == 0) ? STD_MAT_TR : (i + 1 == maxRows) ? STD_MAT_BR : STD_MAT_MR, font, X_POS + (font == &standardFont ? 7 : 10) + baseWidth, Y_POS - (maxRows -1 -i) * fontHeight, vmNormal, true, false);
    if (colVector == true) {
      showString(STD_SUP_T, font, X_POS + (font == &standardFont ? 14 : 18) + baseWidth, Y_POS - (maxRows -1 -i) * fontHeight, vmNormal, true, false);
    }
  }

  displayFormat = tmpDisplayFormat;
  displayFormatDigits = tmpDisplayFormatDigits;

}

int16_t getRealMatrixColumnWidths(const real34Matrix_t *matrix, const font_t *font, int16_t *colWidth, int16_t *rPadWidth, int16_t *digits) {
  const bool_t colVector = matrix->header.matrixColumns == 1 && matrix->header.matrixRows > 1;
  const int rows = colVector ? 1 : matrix->header.matrixRows;
  const int cols = colVector ? matrix->header.matrixRows : matrix->header.matrixColumns;
  const int maxCols = cols > 4 ? 4 : cols;
  const int maxRows = rows > 5 ? 5 : rows;
  const bool_t forEditor = matrix == &openMatrixMIMPointer.realMatrix;
  const uint16_t sRow = forEditor ? scrollRow : 0;
  const uint16_t sCol = forEditor ? scrollColumn : 0;
  const int16_t maxWidth = (font == &numericFont) ? (MATRIX_LINE_WIDTH_LARGE * 3 - 20) : (MATRIX_LINE_WIDTH_SMALL * 4 - 20);
  int16_t totalWidth = 0, width = 0;
  int16_t maxRightWidth[4] = {};
  int16_t maxLeftWidth[4] = {};

  for(int k = 15; k >= 1; k--) {
    if(displayFormat == DF_ALL) *digits = k;
    for(int i = 0; i < maxRows; i++) {
      for(int j = 0; j < maxCols; j++) {
        real34_t r34Val;
        real34Copy(&matrix->matrixElements[(i+sRow)*cols+j+sCol], &r34Val);
        real34SetPositiveSign(&r34Val);
        real34ToDisplayString(&r34Val, amNone, tmpString, font, maxWidth, displayFormat == DF_ALL ? k : 15, true, STD_SPACE_4_PER_EM);
        width = stringWidth(tmpString, font, true, true) + 1;
        rPadWidth[i * 4 + j] = 0;
        for(char *xStr = tmpString; *xStr != 0; xStr++) {
          if(((displayFormat != DF_ENG && (displayFormat != DF_ALL || !getSystemFlag(FLAG_ALLENG))) && (*xStr == '.' || *xStr == ',')) ||
             ((displayFormat == DF_ENG || (displayFormat == DF_ALL && getSystemFlag(FLAG_ALLENG))) && xStr[0] == (char)0x80 && (xStr[1] == (char)0x87 || xStr[1] == (char)0xd7))) {
            rPadWidth[i * 4 + j] = stringWidth(xStr, font, true, true) + 1;
            if(maxRightWidth[j] < rPadWidth[i * 4 + j]) maxRightWidth[j] = rPadWidth[i * 4 + j];
            break;
          }
        }
        if(maxLeftWidth[j] < (width - rPadWidth[i * 4 + j])) maxLeftWidth[j] = (width - rPadWidth[i * 4 + j]);
      }
    }
    for(int i = 0; i < maxRows; i++) {
      for(int j = 0; j < maxCols; j++) {
        rPadWidth[i * 4 + j] -= maxRightWidth[j];
        rPadWidth[i * 4 + j] *= -1;
      }
    }
    for(int j = 0; j < maxCols; j++) {
      colWidth[j] = (maxLeftWidth[j] + maxRightWidth[j]);
      totalWidth += colWidth[j] + 20;
    }
    if(displayFormat != DF_ALL) {
      break;
    }
    else if(totalWidth <= maxWidth) {
      *digits = k;
      break;
    }
    else if(k > 1) {
      totalWidth = 0;
      for(int j = 0; j < maxCols; j++) {
        maxRightWidth[j] = 0;
        maxLeftWidth[j] = 0;
      }
    }
  }
  return totalWidth;
}


/********************************************//**
 * \brief Displays a complex matrix
 *
 * \param[in] matrix complex34Matrix_t *
 * \return void
 ***********************************************/
void showComplexMatrix(const complex34Matrix_t *matrix) {
  int rows = matrix->header.matrixRows;
  int cols = matrix->header.matrixColumns;
  int16_t Y_POS = Y_POSITION_OF_REGISTER_X_LINE;
  int16_t X_POS = 0;
  int16_t totalWidth = 0, width = 0;
  const font_t *font;
  int16_t fontHeight = NUMERIC_FONT_HEIGHT;
  int16_t maxWidth = MATRIX_LINE_WIDTH_LARGE * 3 - 20;
  int16_t colWidth[2] = {}, colWidth_r[2] = {}, colWidth_i[2] = {}, rPadWidth_r[10] = {}, rPadWidth_i[10] = {};
  const bool_t forEditor = matrix == &openMatrixMIMPointer.complexMatrix;
  const uint16_t sRow = forEditor ? scrollRow : 0;
  const uint16_t sCol = forEditor ? scrollColumn : 0;
  const uint16_t tmpDisplayFormat = displayFormat;
  const uint8_t tmpDisplayFormatDigits = displayFormatDigits;

  Y_POS = Y_POSITION_OF_REGISTER_X_LINE - NUMERIC_FONT_HEIGHT;

  bool_t colVector = false;
  if(cols == 1 && rows > 1) {
    colVector = true;
    cols = rows;
    rows = 1;
  }

  const int maxCols = cols > 2 ? 2 : cols;
  const int maxRows = rows > 5 ? 5 : rows;

  int16_t matSelRow = colVector ? getJRegisterAsInt(true) : getIRegisterAsInt(true);
  int16_t matSelCol = colVector ? getIRegisterAsInt(true) : getJRegisterAsInt(true);

  videoMode_t vm = vmNormal;

  if(forEditor) {
    clearRegisterLine(REGISTER_X, true, true);
    clearRegisterLine(REGISTER_Y, true, true);
    if(rows >= 2) clearRegisterLine(REGISTER_Z, true, true);
    if(rows >= 3) clearRegisterLine(REGISTER_T, true, true);
  }

  font = &numericFont;
  if((rows >= 4) || (cols >= 3) || (displayFormat != DF_ALL && displayFormatDigits > 3)) {
smallFont:
    font = &standardFont;
    fontHeight = STANDARD_FONT_HEIGHT;
    Y_POS = Y_POSITION_OF_REGISTER_X_LINE - STANDARD_FONT_HEIGHT + 2;
    maxWidth = MATRIX_LINE_WIDTH_SMALL * 4 - 20;
  }

  if(!forEditor) Y_POS += REGISTER_LINE_HEIGHT;
  const bool_t rightEllipsis = (cols > maxCols) && (cols > maxCols + sCol);
  const bool_t leftEllipsis = (sCol > 0);
  int16_t digits;

  totalWidth = getComplexMatrixColumnWidths(matrix, font, colWidth, colWidth_r, colWidth_i, rPadWidth_r, rPadWidth_i, &digits);
  if(totalWidth > maxWidth) {
    if(font == &numericFont) {
      goto smallFont;
    }
    else {
      displayFormat = DF_SCI;
      displayFormatDigits = 3;
      getComplexMatrixColumnWidths(matrix, font, colWidth, colWidth_r, colWidth_i, rPadWidth_r, rPadWidth_i, &digits);
    }
  }
  int16_t baseWidth = (leftEllipsis ? stringWidth(STD_ELLIPSIS, font, true, true) : 0) +
    (rightEllipsis ? stringWidth(STD_ELLIPSIS, font, true, true) + 11 : 0);
  for(int j = 0; j < maxCols; j++) baseWidth += colWidth[j] + 20;
  if(colWidth_r[0] == 0) {
    baseWidth -= stringWidth("+", font, true, true);
  }

  if(!forEditor) X_POS = SCREEN_WIDTH - 2 - ((colVector ? (font == &standardFont ? 12 : 18) + stringWidth(STD_SUP_T, font, true, true) : (font == &standardFont ? 5 : 10) + stringWidth("]", font, true, true)) + baseWidth);

  for(int i = 0; i < maxRows; i++) {
    int16_t colX = 20;
    showString((maxRows == 1) ? "[" : (i == 0) ? STD_MAT_TL : (i + 1 == maxRows) ? STD_MAT_BL : STD_MAT_ML, font, X_POS + 1, Y_POS - (maxRows -1 - i) * fontHeight, vmNormal, true, false);
    if(!getSystemFlag(FLAG_POLAR) && colWidth_r[0] == 0) {
      colX -= stringWidth("+", font, true, true);
    }
    if(leftEllipsis) {
      showString(STD_ELLIPSIS, font, X_POS + 10, Y_POS - (maxRows -1 -i) * fontHeight, vmNormal, true, false);
      colX += stringWidth(STD_ELLIPSIS, font, true, true);
    }
    for(int j = 0; j< maxCols + (rightEllipsis ? 1 : 0); j++) {
      real34_t re, im;
      if(getSystemFlag(FLAG_POLAR)) { // polar mode
        real_t x, y;
        real34ToReal(VARIABLE_REAL34_DATA(&matrix->matrixElements[(i+sRow)*cols+j+sCol]), &x);
        real34ToReal(VARIABLE_IMAG34_DATA(&matrix->matrixElements[(i+sRow)*cols+j+sCol]), &y);
        realRectangularToPolar(&x, &y, &x, &y, &ctxtReal39);
        convertAngleFromTo(&y, amRadian, currentAngularMode, &ctxtReal39);
        realToReal34(&x, &re);
        realToReal34(&y, &im);
      }
      else { // rectangular mode
        real34Copy(VARIABLE_REAL34_DATA(&matrix->matrixElements[(i+sRow)*cols+j+sCol]), &re);
        real34Copy(VARIABLE_IMAG34_DATA(&matrix->matrixElements[(i+sRow)*cols+j+sCol]), &im);
      }

      if(((i == maxRows - 1) && (rows > maxRows + sRow)) || ((j == maxCols) && rightEllipsis) || ((i == 0) && (sRow > 0))) {
        strcpy(tmpString, STD_ELLIPSIS);
        vm = vmNormal;
      }
      else {
        bool_t neg = real34IsNegative(&re);
        tmpString[0] = neg ? '-' : ' '; tmpString[1] = 0;
        if(!getSystemFlag(FLAG_POLAR))real34SetPositiveSign(&re);
        real34ToDisplayString(&re, amNone, &tmpString[strlen(tmpString)], font, colWidth_r[j], displayFormat == DF_ALL ? digits : 15, true, STD_SPACE_4_PER_EM);
        if(neg) real34SetNegativeSign(&re);
        if (forEditor && matSelRow == (i + sRow) && matSelCol == (j + sCol)) {
          const int16_t minusWidth = stringWidth("-", font, true, true);
          lcd_fill_rect(X_POS + 3 + colX - minusWidth, Y_POS - (maxRows -1 -i) * fontHeight, colWidth[j] + minusWidth + 3, font == &numericFont ? 32 : 20, 0xFF);
          vm = vmReverse;
        } else {
          vm = vmNormal;
        }
      }
      if(strcmp(tmpString, STD_ELLIPSIS) == 0 || !real34IsZero(&re) || real34IsZero(&im)) {
        width = stringWidth(tmpString, font, true, true) + 1;
        showString(tmpString, font, X_POS + (font == &numericFont ? 8 : 5) + colX + (((j == maxCols) && rightEllipsis) ? 8 - width : (colWidth_r[j] - width) - rPadWidth_r[i * 2 + j]), Y_POS - (maxRows -1 -i) * fontHeight, vm, true, false);
      }
      if(strcmp(tmpString, STD_ELLIPSIS) != 0) {
        if(!real34IsZero(&im)) {
          bool_t neg = real34IsNegative(&im);
          int16_t cpxUnitWidth;

          if(getSystemFlag(FLAG_POLAR)) { // polar mode
            strcpy(tmpString, STD_SPACE_4_PER_EM STD_MEASURED_ANGLE STD_SPACE_4_PER_EM);
          }
          else { // rectangular mode
            strcpy(tmpString, "+");
            strcat(tmpString, COMPLEX_UNIT);
            strcat(tmpString, PRODUCT_SIGN);
          }
          cpxUnitWidth = width = stringWidth(tmpString, font, true, true);
          if(!getSystemFlag(FLAG_POLAR)) {
            if(neg) tmpString[0] = '-';
            if(!neg && real34IsZero(&re)) {
              strcpy(tmpString, COMPLEX_UNIT);
              strcat(tmpString, PRODUCT_SIGN);
            }
          }
          showString(tmpString, font, X_POS + (font == &numericFont ? 8 : 5) + colX + colWidth_r[j] + (width - stringWidth(tmpString, font, true, true)), Y_POS - (maxRows -1 -i) * fontHeight, vm, true, false);

          if(!getSystemFlag(FLAG_POLAR)) real34SetPositiveSign(&im);
          real34ToDisplayString(&im, amNone, tmpString, font, colWidth_i[j], displayFormat == DF_ALL ? digits : 15, true, STD_SPACE_4_PER_EM);
          if(neg) real34SetNegativeSign(&im);
          width = stringWidth(tmpString, font, true, true) + 1;
          showString(tmpString, font, X_POS + (font == &numericFont ? 8 : 5) + colX + colWidth_r[j] + cpxUnitWidth + (((j == maxCols - 1) && rightEllipsis) ? 0 : (colWidth_i[j] - width) - rPadWidth_i[i * 2 + j]), Y_POS - (maxRows -1 -i) * fontHeight, vm, true, false);
        }
      }
      colX += colWidth[j] + 20;
    }
    showString((maxRows == 1) ? "]" : (i == 0) ? STD_MAT_TR : (i + 1 == maxRows) ? STD_MAT_BR : STD_MAT_MR, font, X_POS + (font == &standardFont ? 7 : 10) + baseWidth, Y_POS - (maxRows -1 -i) * fontHeight, vmNormal, true, false);
    if (colVector == true) {
      showString(STD_SUP_T, font, X_POS + (font == &standardFont ? 14 : 18) + baseWidth, Y_POS - (maxRows -1 -i) * fontHeight, vmNormal, true, false);
    }
  }

  displayFormat = tmpDisplayFormat;
  displayFormatDigits = tmpDisplayFormatDigits;

}

int16_t getComplexMatrixColumnWidths(const complex34Matrix_t *matrix, const font_t *font, int16_t *colWidth, int16_t *colWidth_r, int16_t *colWidth_i, int16_t *rPadWidth_r, int16_t *rPadWidth_i, int16_t *digits) {
  const bool_t colVector = matrix->header.matrixColumns == 1 && matrix->header.matrixRows > 1;
  const int rows = colVector ? 1 : matrix->header.matrixRows;
  const int cols = colVector ? matrix->header.matrixRows : matrix->header.matrixColumns;
  const int maxCols = cols > 2 ? 2 : cols;
  const int maxRows = rows > 5 ? 5 : rows;
  const bool_t forEditor = matrix == &openMatrixMIMPointer.complexMatrix;
  const uint16_t sRow = forEditor ? scrollRow : 0;
  const uint16_t sCol = forEditor ? scrollColumn : 0;
  const int16_t maxWidth = (font == &numericFont) ? (MATRIX_LINE_WIDTH_LARGE * 3 - 20) : (MATRIX_LINE_WIDTH_SMALL * 4 - 20);
  int16_t totalWidth = 0, width = 0;
  int16_t maxRightWidth_r[2] = {};
  int16_t maxLeftWidth_r[2] = {};
  int16_t maxRightWidth_i[2] = {};
  int16_t maxLeftWidth_i[2] = {};

  uint16_t cpxUnitWidth;
  if(getSystemFlag(FLAG_POLAR)) { // polar mode
    strcpy(tmpString, STD_SPACE_4_PER_EM STD_MEASURED_ANGLE STD_SPACE_4_PER_EM);
  }
  else { // rectangular mode
    strcpy(tmpString, "+");
    strcat(tmpString, COMPLEX_UNIT);
    strcat(tmpString, PRODUCT_SIGN);
  }
  cpxUnitWidth = stringWidth(tmpString, font, true, true);

  for(int k = 15; k >= 1; k--) {
    if(displayFormat == DF_ALL) *digits = k;
    for(int i = 0; i < maxRows; i++) {
      for(int j = 0; j < maxCols; j++) {
        complex34_t c34Val;
        complex34Copy(&matrix->matrixElements[(i+sRow)*cols+j+sCol], &c34Val);
        if(getSystemFlag(FLAG_POLAR)) { // polar mode
          real_t x, y;
          real34ToReal(VARIABLE_REAL34_DATA(&c34Val), &x);
          real34ToReal(VARIABLE_IMAG34_DATA(&c34Val), &y);
          realRectangularToPolar(&x, &y, &x, &y, &ctxtReal39);
          convertAngleFromTo(&y, amRadian, currentAngularMode, &ctxtReal39);
          realToReal34(&x, VARIABLE_REAL34_DATA(&c34Val));
          realToReal34(&y, VARIABLE_IMAG34_DATA(&c34Val));
        }

        rPadWidth_r[i * 2 + j] = 0;
        if(!real34IsZero(VARIABLE_REAL34_DATA(&c34Val)) || real34IsZero(VARIABLE_IMAG34_DATA(&c34Val))) {
          real34SetPositiveSign(VARIABLE_REAL34_DATA(&c34Val));
          real34ToDisplayString(VARIABLE_REAL34_DATA(&c34Val), amNone, tmpString, font, maxWidth, displayFormat == DF_ALL ? k : 15, true, STD_SPACE_4_PER_EM);
          width = stringWidth(tmpString, font, true, true) + 1;
          for(char *xStr = tmpString; *xStr != 0; xStr++) {
            if(((displayFormat != DF_ENG && (displayFormat != DF_ALL || !getSystemFlag(FLAG_ALLENG))) && (*xStr == '.' || *xStr == ',')) ||
               ((displayFormat == DF_ENG || (displayFormat == DF_ALL && getSystemFlag(FLAG_ALLENG))) && xStr[0] == (char)0x80 && (xStr[1] == (char)0x87 || xStr[1] == (char)0xd7))) {
              rPadWidth_r[i * 2 + j] = stringWidth(xStr, font, true, true) + 1;
              if(maxRightWidth_r[j] < rPadWidth_r[i * 2 + j]) maxRightWidth_r[j] = rPadWidth_r[i * 2 + j];
              break;
            }
          }
          if(maxLeftWidth_r[j] < (width - rPadWidth_r[i * 2 + j])) maxLeftWidth_r[j] = (width - rPadWidth_r[i * 2 + j]);
        }

        rPadWidth_i[i * 2 + j] = 0;
        if(!real34IsZero(VARIABLE_IMAG34_DATA(&c34Val))) {
          if(!getSystemFlag(FLAG_POLAR))real34SetPositiveSign(VARIABLE_IMAG34_DATA(&c34Val));
          real34ToDisplayString(VARIABLE_IMAG34_DATA(&c34Val), amNone, tmpString, font, maxWidth, displayFormat == DF_ALL ? k : 15, true, STD_SPACE_4_PER_EM);
          width = stringWidth(tmpString, font, true, true) + 1;
          for(char *xStr = tmpString; *xStr != 0; xStr++) {
            if(((displayFormat != DF_ENG && (displayFormat != DF_ALL || !getSystemFlag(FLAG_ALLENG))) && (*xStr == '.' || *xStr == ',')) ||
               ((displayFormat == DF_ENG || (displayFormat == DF_ALL && getSystemFlag(FLAG_ALLENG))) && xStr[0] == (char)0x80 && (xStr[1] == (char)0x87 || xStr[1] == (char)0xd7))) {
              rPadWidth_i[i * 2 + j] = stringWidth(xStr, font, true, true) + 1;
              if(maxRightWidth_i[j] < rPadWidth_i[i * 2 + j]) maxRightWidth_i[j] = rPadWidth_i[i * 2 + j];
              break;
            }
          }
          if(maxLeftWidth_i[j] < (width - rPadWidth_i[i * 2 + j])) maxLeftWidth_i[j] = (width - rPadWidth_i[i * 2 + j]);
        }
      }
    }
    for(int i = 0; i < maxRows; i++) {
      for(int j = 0; j < maxCols; j++) {
        rPadWidth_r[i * 2 + j] -= maxRightWidth_r[j];
        rPadWidth_r[i * 2 + j] *= -1;
        rPadWidth_i[i * 2 + j] -= maxRightWidth_i[j];
        rPadWidth_i[i * 2 + j] *= -1;
      }
    }
    for(int j = 0; j < maxCols; j++) {
      colWidth_r[j] = maxLeftWidth_r[j] + maxRightWidth_r[j];
      colWidth_i[j] = maxLeftWidth_i[j] + maxRightWidth_i[j];
      colWidth[j] = colWidth_r[j] + (colWidth_i[j] > 0 ? (cpxUnitWidth + colWidth_i[j]) : 0);
      totalWidth += colWidth[j] + 20;
    }
    if(displayFormat != DF_ALL) {
      break;
    }
    else if(totalWidth <= maxWidth) {
      *digits = k;
      break;
    }
    else if(k > 1) {
      totalWidth = 0;
      for(int j = 0; j < maxCols; j++) {
        maxRightWidth_r[j] = 0;
        maxLeftWidth_r[j] = 0;
        maxRightWidth_i[j] = 0;
        maxLeftWidth_i[j] = 0;
      }
    }
  }
  return totalWidth;
}


/*
void storeMatrixToXRegister(real34Matrix_t *matrix) {

  setSystemFlag(FLAG_ASLIFT);
  liftStack();
  clearSystemFlag(FLAG_ASLIFT);

  //WORKING//
  //openMatrixMIMPointer = matrix;
  // END WORKING//

  convertReal34MatrixToReal34MatrixRegister(matrix, REGISTER_X);
}
*/

void getMatrixFromRegister(calcRegister_t regist) {
  if(getRegisterDataType(regist) == dtReal34Matrix) {
    real34Matrix_t matrix;

    if(openMatrixMIMPointer.realMatrix.matrixElements) realMatrixFree(&openMatrixMIMPointer.realMatrix);
    convertReal34MatrixRegisterToReal34Matrix(regist, &matrix);

    openMatrixMIMPointer.realMatrix = matrix;
  }
  else if(getRegisterDataType(regist) == dtComplex34Matrix) {
    complex34Matrix_t matrix;

    if(openMatrixMIMPointer.complexMatrix.matrixElements) complexMatrixFree(&openMatrixMIMPointer.complexMatrix);
    convertComplex34MatrixRegisterToComplex34Matrix(regist, &matrix);

    openMatrixMIMPointer.complexMatrix = matrix;
  }
  else {
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(regist));
    moreInfoOnError("In function getMatrixFromRegister:", errorMessage, "is not dataType dtRealMatrix.", "");
    #endif
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    return;
  }

}

//Row of Matrix
int16_t getIRegisterAsInt(bool_t asArrayPointer) {
  int16_t ret;
  longInteger_t tmp_lgInt;

  if(getRegisterDataType(REGISTER_I) == dtLongInteger)
    convertLongIntegerRegisterToLongInteger(REGISTER_I, tmp_lgInt);
  else if(getRegisterDataType(REGISTER_I) == dtReal34)
    convertReal34ToLongInteger(REGISTER_REAL34_DATA(REGISTER_I), tmp_lgInt, DEC_ROUND_DOWN);
  else
    longIntegerInit(tmp_lgInt);
  longIntegerToInt(tmp_lgInt, ret);

  longIntegerFree(tmp_lgInt);

  if (asArrayPointer) ret--;

  return ret;
}

//Col of Matrix
int16_t getJRegisterAsInt(bool_t asArrayPointer) {
  int16_t ret;
  longInteger_t tmp_lgInt;

  if(getRegisterDataType(REGISTER_J) == dtLongInteger)
    convertLongIntegerRegisterToLongInteger(REGISTER_J, tmp_lgInt);
  else if(getRegisterDataType(REGISTER_J) == dtReal34)
    convertReal34ToLongInteger(REGISTER_REAL34_DATA(REGISTER_J), tmp_lgInt, DEC_ROUND_DOWN);
  else
    longIntegerInit(tmp_lgInt);
  longIntegerToInt(tmp_lgInt, ret);

  longIntegerFree(tmp_lgInt);

  if (asArrayPointer) ret--;

  return ret;
}

//Row of Matrix
void setIRegisterAsInt(bool_t asArrayPointer, int16_t toStore) {
  if (asArrayPointer) toStore++;
  longInteger_t tmp_lgInt;
  longIntegerInit(tmp_lgInt);

  intToLongInteger(toStore, tmp_lgInt);
  convertLongIntegerToLongIntegerRegister(tmp_lgInt, REGISTER_I);

  longIntegerFree(tmp_lgInt);
}

//ColOfMatrix
void setJRegisterAsInt(bool_t asArrayPointer, int16_t toStore) {
  if (asArrayPointer) toStore++;
  longInteger_t tmp_lgInt;
  longIntegerInit(tmp_lgInt);

  intToLongInteger(toStore, tmp_lgInt);
  convertLongIntegerToLongIntegerRegister(tmp_lgInt, REGISTER_J);

  longIntegerFree(tmp_lgInt);
}

bool_t wrapIJ(uint16_t rows, uint16_t cols) {
  if(getIRegisterAsInt(true) < 0) {
    setIRegisterAsInt(true, rows - 1);
    setJRegisterAsInt(true, (getJRegisterAsInt(true) == 0) ? cols - 1 : getJRegisterAsInt(true) - 1);
  } else if(getIRegisterAsInt(true) == rows) {
    setIRegisterAsInt(true, 0);
    setJRegisterAsInt(true, (getJRegisterAsInt(true) == cols - 1) ? 0 : getJRegisterAsInt(true) + 1);
  }

  if(getJRegisterAsInt(true) < 0) {
    setJRegisterAsInt(true, cols - 1);
    setIRegisterAsInt(true, (getIRegisterAsInt(true) == 0) ? rows - 1 : getIRegisterAsInt(true) - 1);
  } else if(getJRegisterAsInt(true) == cols) {
    setJRegisterAsInt(true, 0);
    setIRegisterAsInt(true, ((!getSystemFlag(FLAG_GROW)) && (getIRegisterAsInt(true) == rows - 1)) ? 0 : getIRegisterAsInt(true) + 1);
  }

  return getIRegisterAsInt(true) == rows;
}



/* Duplicate */
void copyRealMatrix(const real34Matrix_t *matrix, real34Matrix_t *res) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;

  realMatrixInit(res, rows, cols);
  for(i = 0; i < cols * rows; ++i) {
    real34Copy(&matrix->matrixElements[i], &res->matrixElements[i]);
  }
}

void copyComplexMatrix(const complex34Matrix_t *matrix, complex34Matrix_t *res) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;

  complexMatrixInit(res, rows, cols);
  for(i = 0; i < cols * rows; ++i) {
    complex34Copy(&matrix->matrixElements[i], &res->matrixElements[i]);
  }
}



/* Link to real matrix register (data not copied) */
void linkToRealMatrixRegister(calcRegister_t regist, real34Matrix_t *linkedMatrix) {
  linkedMatrix->header.matrixRows    = REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixRows;
  linkedMatrix->header.matrixColumns = REGISTER_REAL34_MATRIX_DBLOCK(regist)->matrixColumns;
  linkedMatrix->matrixElements       = REGISTER_REAL34_MATRIX_M_ELEMENTS(regist);
}

/* Link to complex matrix register (data not copied) */
void linkToComplexMatrixRegister(calcRegister_t regist, complex34Matrix_t *linkedMatrix) {
  linkedMatrix->header.matrixRows    = REGISTER_COMPLEX34_MATRIX_DBLOCK(regist)->matrixRows;
  linkedMatrix->header.matrixColumns = REGISTER_COMPLEX34_MATRIX_DBLOCK(regist)->matrixColumns;
  linkedMatrix->matrixElements       = REGISTER_COMPLEX34_MATRIX_M_ELEMENTS(regist);
}



/* Insert a row */
void insRowRealMatrix(real34Matrix_t *matrix, uint16_t beforeRowNo) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;
  real34Matrix_t newMat;

  realMatrixInit(&newMat, rows + 1, cols);
  for(i = 0; i < beforeRowNo * cols; ++i) {
    real34Copy(&matrix->matrixElements[i], &newMat.matrixElements[i]);
  }
  for(i = 0; i < cols; ++i) {
    real34Copy(const34_0, &newMat.matrixElements[beforeRowNo * cols + i]);
  }
  for(i = beforeRowNo * cols; i < cols * rows; ++i) {
    real34Copy(&matrix->matrixElements[i], &newMat.matrixElements[i + cols]);
  }

  realMatrixFree(matrix);
  matrix->header.matrixRows    = newMat.header.matrixRows;
  matrix->header.matrixColumns = newMat.header.matrixColumns;
  matrix->matrixElements       = newMat.matrixElements;
}

void insRowComplexMatrix(complex34Matrix_t *matrix, uint16_t beforeRowNo) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;
  complex34Matrix_t newMat;

  complexMatrixInit(&newMat, rows + 1, cols);
  for(i = 0; i < beforeRowNo * cols; ++i) {
    complex34Copy(&matrix->matrixElements[i], &newMat.matrixElements[i]);
  }
  for(i = 0; i < cols; ++i) {
    real34Copy(const34_0, VARIABLE_REAL34_DATA(&newMat.matrixElements[beforeRowNo * cols + i]));
    real34Copy(const34_0, VARIABLE_IMAG34_DATA(&newMat.matrixElements[beforeRowNo * cols + i]));
  }
  for(i = beforeRowNo * cols; i < cols * rows; ++i) {
    complex34Copy(&matrix->matrixElements[i], &newMat.matrixElements[i + cols]);
  }

  complexMatrixFree(matrix);
  matrix->header.matrixRows    = newMat.header.matrixRows;
  matrix->header.matrixColumns = newMat.header.matrixColumns;
  matrix->matrixElements       = newMat.matrixElements;
}



/* Delete a row */
void delRowRealMatrix(real34Matrix_t *matrix, uint16_t beforeRowNo) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;
  real34Matrix_t newMat;

  realMatrixInit(&newMat, rows - 1, cols);
  for(i = 0; i < beforeRowNo * cols; ++i) {
    real34Copy(&matrix->matrixElements[i], &newMat.matrixElements[i]);
  }
  for(i = (beforeRowNo + 1) * cols; i < cols * rows; ++i) {
    real34Copy(&matrix->matrixElements[i], &newMat.matrixElements[i - cols]);
  }

  realMatrixFree(matrix);
  matrix->header.matrixRows    = newMat.header.matrixRows;
  matrix->header.matrixColumns = newMat.header.matrixColumns;
  matrix->matrixElements       = newMat.matrixElements;
}

void delRowComplexMatrix(complex34Matrix_t *matrix, uint16_t beforeRowNo) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;
  complex34Matrix_t newMat;

  complexMatrixInit(&newMat, rows - 1, cols);
  for(i = 0; i < beforeRowNo * cols; ++i) {
    complex34Copy(&matrix->matrixElements[i], &newMat.matrixElements[i]);
  }
  for(i = (beforeRowNo + 1) * cols; i < cols * rows; ++i) {
    complex34Copy(&matrix->matrixElements[i], &newMat.matrixElements[i - cols]);
  }

  complexMatrixFree(matrix);
  matrix->header.matrixRows    = newMat.header.matrixRows;
  matrix->header.matrixColumns = newMat.header.matrixColumns;
  matrix->matrixElements       = newMat.matrixElements;
}


/* Transpose */
void transposeRealMatrix(const real34Matrix_t *matrix, real34Matrix_t *res) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i, j;

  realMatrixInit(res, cols, rows);
  for(i = 0; i < rows; ++i) {
    for(j = 0; j < cols; ++j) {
      real34Copy(&matrix->matrixElements[i * cols + j], &res->matrixElements[j * rows + i]);
    }
  }
}

void transposeComplexMatrix(const complex34Matrix_t *matrix, complex34Matrix_t *res) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i, j;

  complexMatrixInit(res, cols, rows);
  for(i = 0; i < rows; ++i) {
    for(j = 0; j < cols; ++j) {
      complex34Copy(&matrix->matrixElements[i * cols + j], &res->matrixElements[j * rows + i]);
    }
  }
}



/* Addition and subtraction */
static void addSubRealMatrices(const real34Matrix_t *y, const real34Matrix_t *x, bool_t subtraction, real34Matrix_t *res) {
  const uint16_t rows = y->header.matrixRows;
  const uint16_t cols = y->header.matrixColumns;
  int32_t i;

  if((y->header.matrixColumns != x->header.matrixColumns) || (y->header.matrixRows != x->header.matrixRows)) {
    res->matrixElements = NULL; // Matrix mismatch
    res->header.matrixRows = res->header.matrixColumns = 0;
    return;
  }

  if((y != res) && (x != res)) realMatrixInit(res, rows, cols);
  for(i = 0; i < cols * rows; ++i) {
    if(subtraction) {
      real34Subtract(&y->matrixElements[i], &x->matrixElements[i], &res->matrixElements[i]);
    }
    else {
      real34Add(&y->matrixElements[i], &x->matrixElements[i], &res->matrixElements[i]);
    }
  }
}

void addRealMatrices(const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res) {
  addSubRealMatrices(y, x, false, res);
}

void subtractRealMatrices(const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res) {
  addSubRealMatrices(y, x, true, res);
}

static void addSubComplexMatrices(const complex34Matrix_t *y, const complex34Matrix_t *x, bool_t subtraction, complex34Matrix_t *res) {
  const uint16_t rows = y->header.matrixRows;
  const uint16_t cols = y->header.matrixColumns;
  int32_t i;

  if((y->header.matrixColumns != x->header.matrixColumns) || (y->header.matrixRows != x->header.matrixRows)) {
    res->matrixElements = NULL; // Matrix mismatch
    res->header.matrixRows = res->header.matrixColumns = 0;
    return;
  }

  if((y != res) && (x != res)) complexMatrixInit(res, rows, cols);
  for(i = 0; i < cols * rows; ++i) {
    if(subtraction) {
      real34Subtract(VARIABLE_REAL34_DATA(&y->matrixElements[i]), VARIABLE_REAL34_DATA(&x->matrixElements[i]), VARIABLE_REAL34_DATA(&res->matrixElements[i]));
      real34Subtract(VARIABLE_IMAG34_DATA(&y->matrixElements[i]), VARIABLE_IMAG34_DATA(&x->matrixElements[i]), VARIABLE_IMAG34_DATA(&res->matrixElements[i]));
    }
    else {
      real34Add(VARIABLE_REAL34_DATA(&y->matrixElements[i]), VARIABLE_REAL34_DATA(&x->matrixElements[i]), VARIABLE_REAL34_DATA(&res->matrixElements[i]));
      real34Add(VARIABLE_IMAG34_DATA(&y->matrixElements[i]), VARIABLE_IMAG34_DATA(&x->matrixElements[i]), VARIABLE_IMAG34_DATA(&res->matrixElements[i]));
    }
  }
}

void addComplexMatrices(const complex34Matrix_t *y, const complex34Matrix_t *x, complex34Matrix_t *res) {
  addSubComplexMatrices(y, x, false, res);
}

void subtractComplexMatrices(const complex34Matrix_t *y, const complex34Matrix_t *x, complex34Matrix_t *res) {
  addSubComplexMatrices(y, x, true, res);
}



/* Multiplication */
void multiplyRealMatrix(const real34Matrix_t *matrix, const real34_t *x, real34Matrix_t *res) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;

  if(matrix != res) realMatrixInit(res, rows, cols);
  for(i = 0; i < cols * rows; ++i) {
    real34Multiply(&matrix->matrixElements[i], x, &res->matrixElements[i]);
  }
}

void _multiplyRealMatrix(const real34Matrix_t *matrix, const real_t *x, real34Matrix_t *res, realContext_t *realContext) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;
  real_t y;

  if(matrix != res) realMatrixInit(res, rows, cols);
  for(i = 0; i < cols * rows; ++i) {
    real34ToReal(&matrix->matrixElements[i], &y);
    realMultiply(&y, x, &y, realContext);
    realToReal34(&y, &matrix->matrixElements[i]);
  }
}

void multiplyRealMatrices(const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res) {
  const uint16_t rows = y->header.matrixRows;
  const uint16_t iter = y->header.matrixColumns;
  const uint16_t cols = x->header.matrixColumns;
  int32_t i, j, k;
  real_t sum, prod, p, q;

  if(y->header.matrixColumns != x->header.matrixRows) {
    res->matrixElements = NULL; // Matrix mismatch
    res->header.matrixRows = res->header.matrixColumns = 0;
    return;
  }

  realMatrixInit(res, rows, cols);
  for(i = 0; i < rows; ++i) {
    for(j = 0; j < cols; ++j) {
      realCopy(const_0, &sum);
      realCopy(const_0, &prod);
      for(k = 0; k < iter; ++k) {
        real34ToReal(&y->matrixElements[i * iter + k], &p);
        real34ToReal(&x->matrixElements[k * cols + j], &q);
        realMultiply(&p, &q, &prod, &ctxtReal39);
        realAdd(&sum, &prod, &sum, &ctxtReal39);
      }
      realToReal34(&sum, &res->matrixElements[i * cols + j]);
    }
  }
}

void multiplyComplexMatrix(const complex34Matrix_t *matrix, const real34_t *xr, const real34_t *xi, complex34Matrix_t *res) {
  real_t _xr, _xi;

  real34ToReal(xr, &_xr); real34ToReal(xi, &_xi);
  _multiplyComplexMatrix(matrix, &_xr, &_xi, res, &ctxtReal39);
}

void _multiplyComplexMatrix(const complex34Matrix_t *matrix, const real_t *xr, const real_t *xi, complex34Matrix_t *res, realContext_t *realContext) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;
  real_t yr, yi;

  if(matrix != res) complexMatrixInit(res, rows, cols);
  for(i = 0; i < cols * rows; ++i) {
    real34ToReal(VARIABLE_REAL34_DATA(&matrix->matrixElements[i]), &yr);
    real34ToReal(VARIABLE_IMAG34_DATA(&matrix->matrixElements[i]), &yi);
    mulComplexComplex(&yr, &yi, xr, xi, &yr, &yi, &ctxtReal39);
    realToReal34(&yr, VARIABLE_REAL34_DATA(&res->matrixElements[i]));
    realToReal34(&yi, VARIABLE_IMAG34_DATA(&res->matrixElements[i]));
  }
}

void multiplyComplexMatrices(const complex34Matrix_t *y, const complex34Matrix_t *x, complex34Matrix_t *res) {
  const uint16_t rows = y->header.matrixRows;
  const uint16_t iter = y->header.matrixColumns;
  const uint16_t cols = x->header.matrixColumns;
  int32_t i, j, k;
  real_t sumr, prodr, pr, qr;
  real_t sumi, prodi, pi, qi;

  if(y->header.matrixColumns != x->header.matrixRows) {
    res->matrixElements = NULL; // Matrix mismatch
    res->header.matrixRows = res->header.matrixColumns = 0;
    return;
  }

  complexMatrixInit(res, rows, cols);
  for(i = 0; i < rows; ++i) {
    for(j = 0; j < cols; ++j) {
      realCopy(const_0, &sumr);  realCopy(const_0, &sumi);
      realCopy(const_0, &prodr); realCopy(const_0, &prodi);
      for(k = 0; k < iter; ++k) {
        real34ToReal(VARIABLE_REAL34_DATA(&y->matrixElements[i * iter + k]), &pr);
        real34ToReal(VARIABLE_IMAG34_DATA(&y->matrixElements[i * iter + k]), &pi);
        real34ToReal(VARIABLE_REAL34_DATA(&x->matrixElements[k * cols + j]), &qr);
        real34ToReal(VARIABLE_IMAG34_DATA(&x->matrixElements[k * cols + j]), &qi);
        mulComplexComplex(&pr, &pi, &qr, &qi, &prodr, &prodi, &ctxtReal39);
        realAdd(&sumr, &prodr, &sumr, &ctxtReal39);
        realAdd(&sumi, &prodi, &sumi, &ctxtReal39);
      }
      realToReal34(&sumr, VARIABLE_REAL34_DATA(&res->matrixElements[i * cols + j]));
      realToReal34(&sumi, VARIABLE_IMAG34_DATA(&res->matrixElements[i * cols + j]));
    }
  }
}



/* Vectors */
uint16_t realVectorSize(const real34Matrix_t *matrix) {
  if((matrix->header.matrixColumns != 1) && (matrix->header.matrixRows != 1))
    return 0;
  else
    return matrix->header.matrixColumns * matrix->header.matrixRows;
}

void dotRealVectors(const real34Matrix_t *y, const real34Matrix_t *x, real34_t *res) {
  const uint16_t elements = realVectorSize(y);
  int32_t i;
  real_t sum, prod, p, q;

  if((realVectorSize(y) == 0) || (realVectorSize(x) == 0) || (realVectorSize(y) != realVectorSize(x))) {
    realToReal34(const_NaN, res); // Not a vector or mismatched
    return;
  }

  realCopy(const_0, &sum);
  realCopy(const_0, &prod);
  for(i = 0; i < elements; ++i) {
    real34ToReal(&y->matrixElements[i], &p);
    real34ToReal(&x->matrixElements[i], &q);
    realMultiply(&p, &q, &prod, &ctxtReal39);
    realAdd(&sum, &prod, &sum, &ctxtReal39);
  }
  realToReal34(&sum, res);
}

void crossRealVectors(const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res) {
  const uint16_t elementsY = realVectorSize(y);
  const uint16_t elementsX = realVectorSize(x);
  real_t a1, a2, a3, b1, b2, b3, p, q;

  if((elementsY == 0) || (elementsX == 0) || (elementsY > 3) || (elementsX > 3)) {
    return; // Not a vector or mismatched
  }

  real34ToReal(                 &y->matrixElements[0]            , &a1);
  real34ToReal(elementsY >= 2 ? &y->matrixElements[1] : const34_0, &a2);
  real34ToReal(elementsY >= 3 ? &y->matrixElements[2] : const34_0, &a3);

  real34ToReal(                 &x->matrixElements[0]            , &b1);
  real34ToReal(elementsX >= 2 ? &x->matrixElements[1] : const34_0, &b2);
  real34ToReal(elementsX >= 3 ? &x->matrixElements[2] : const34_0, &b3);

  realMatrixInit(res, 1, 3);

  realMultiply(&a2, &b3, &p, &ctxtReal39); realMultiply(&a3, &b2, &q, &ctxtReal39);
  realSubtract(&p, &q, &p, &ctxtReal39); realToReal34(&p, &res->matrixElements[0]);

  realMultiply(&a3, &b1, &p, &ctxtReal39); realMultiply(&a1, &b3, &q, &ctxtReal39);
  realSubtract(&p, &q, &p, &ctxtReal39); realToReal34(&p, &res->matrixElements[1]);

  realMultiply(&a1, &b2, &p, &ctxtReal39); realMultiply(&a2, &b1, &q, &ctxtReal39);
  realSubtract(&p, &q, &p, &ctxtReal39); realToReal34(&p, &res->matrixElements[2]);
}

uint16_t complexVectorSize(const complex34Matrix_t *matrix) {
  return realVectorSize((const real34Matrix_t *)matrix);
}

void dotComplexVectors(const complex34Matrix_t *y, const complex34Matrix_t *x, real34_t *res_r, real34_t *res_i) {
  const uint16_t elements = complexVectorSize(y);
  int32_t i;
  real_t sumr, prodr, pr, qr;
  real_t sumi, prodi, pi, qi;

  if((complexVectorSize(y) == 0) || (complexVectorSize(x) == 0) || (complexVectorSize(y) != complexVectorSize(x))) {
    realToReal34(const_NaN, res_r); realToReal34(const_NaN, res_i); // Not a vector or mismatched
    return;
  }

  realCopy(const_0, &sumr);  realCopy(const_0, &sumi);
  realCopy(const_0, &prodr); realCopy(const_0, &prodi);
  for(i = 0; i < elements; ++i) {
    real34ToReal(VARIABLE_REAL34_DATA(&y->matrixElements[i]), &pr); real34ToReal(VARIABLE_IMAG34_DATA(&y->matrixElements[i]), &pi);
    real34ToReal(VARIABLE_REAL34_DATA(&x->matrixElements[i]), &qr); real34ToReal(VARIABLE_IMAG34_DATA(&x->matrixElements[i]), &qi);
    mulComplexComplex(&pr, &pi, &qr, &qi, &prodr, &prodi, &ctxtReal39);
    realAdd(&sumr, &prodr, &sumr, &ctxtReal39);
    realAdd(&sumi, &prodi, &sumi, &ctxtReal39);
  }
  realToReal34(&sumr, res_r);
  realToReal34(&sumi, res_i);
}

void crossComplexVectors(const complex34Matrix_t *y, const complex34Matrix_t *x, complex34Matrix_t *res) {
  const uint16_t elementsY = complexVectorSize(y);
  const uint16_t elementsX = complexVectorSize(x);
  real_t a1r, a2r, a3r, b1r, b2r, b3r, pr, qr;
  real_t a1i, a2i, a3i, b1i, b2i, b3i, pi, qi;

  if((elementsY == 0) || (elementsX == 0) || (elementsY > 3) || (elementsX > 3)) {
    return; // Not a vector or mismatched
  }

  real34ToReal(                 VARIABLE_REAL34_DATA(&y->matrixElements[0])            , &a1r);
  real34ToReal(                 VARIABLE_IMAG34_DATA(&y->matrixElements[0])            , &a1i);
  real34ToReal(elementsY >= 2 ? VARIABLE_REAL34_DATA(&y->matrixElements[1]) : const34_0, &a2r);
  real34ToReal(elementsY >= 2 ? VARIABLE_IMAG34_DATA(&y->matrixElements[1]) : const34_0, &a2i);
  real34ToReal(elementsY >= 3 ? VARIABLE_REAL34_DATA(&y->matrixElements[2]) : const34_0, &a3r);
  real34ToReal(elementsY >= 3 ? VARIABLE_IMAG34_DATA(&y->matrixElements[2]) : const34_0, &a3i);

  real34ToReal(                 VARIABLE_REAL34_DATA(&x->matrixElements[0])            , &b1r);
  real34ToReal(                 VARIABLE_IMAG34_DATA(&x->matrixElements[0])            , &b1i);
  real34ToReal(elementsX >= 2 ? VARIABLE_REAL34_DATA(&x->matrixElements[1]) : const34_0, &b2r);
  real34ToReal(elementsX >= 2 ? VARIABLE_IMAG34_DATA(&x->matrixElements[1]) : const34_0, &b2i);
  real34ToReal(elementsX >= 3 ? VARIABLE_REAL34_DATA(&x->matrixElements[2]) : const34_0, &b3r);
  real34ToReal(elementsX >= 3 ? VARIABLE_IMAG34_DATA(&x->matrixElements[2]) : const34_0, &b3i);

  complexMatrixInit(res, 1, 3);

  mulComplexComplex(&a2r, &a2i, &b3r, &b3i, &pr, &pi, &ctxtReal39);
  mulComplexComplex(&a3r, &a3i, &b2r, &b2i, &qr, &qi, &ctxtReal39);
  realSubtract(&pr, &qr, &pr, &ctxtReal39), realSubtract(&pi, &qi, &pi, &ctxtReal39);
  realToReal34(&pr, VARIABLE_REAL34_DATA(&res->matrixElements[0]));
  realToReal34(&pi, VARIABLE_IMAG34_DATA(&res->matrixElements[0]));

  mulComplexComplex(&a3r, &a3i, &b1r, &b1i, &pr, &pi, &ctxtReal39);
  mulComplexComplex(&a1r, &a1i, &b3r, &b3i, &qr, &qi, &ctxtReal39);
  realSubtract(&pr, &qr, &pr, &ctxtReal39), realSubtract(&pi, &qi, &pi, &ctxtReal39);
  realToReal34(&pr, VARIABLE_REAL34_DATA(&res->matrixElements[1]));
  realToReal34(&pi, VARIABLE_IMAG34_DATA(&res->matrixElements[1]));

  mulComplexComplex(&a1r, &a1i, &b2r, &b2i, &pr, &pi, &ctxtReal39);
  mulComplexComplex(&a2r, &a2i, &b1r, &b1i, &qr, &qi, &ctxtReal39);
  realSubtract(&pr, &qr, &pr, &ctxtReal39), realSubtract(&pi, &qi, &pi, &ctxtReal39);
  realToReal34(&pr, VARIABLE_REAL34_DATA(&res->matrixElements[2]));
  realToReal34(&pi, VARIABLE_IMAG34_DATA(&res->matrixElements[2]));
}



/* LU decomposition routine borrowed from WP 34s */
void WP34S_LU_decomposition(const real34Matrix_t *matrix, real34Matrix_t *lu, uint16_t *p) {
  int i, j, k;
  int pvt;
  real_t max, t, u;
  real_t *tmpMat;

  const uint16_t m = matrix->header.matrixRows;
  const uint16_t n = matrix->header.matrixColumns;

  if(matrix->header.matrixRows != matrix->header.matrixColumns) {
    if(matrix != lu) {
      lu->matrixElements = NULL; // Matrix is not square
      lu->header.matrixRows = lu->header.matrixColumns = 0;
    }
    return;
  }

  if(matrix != lu) copyRealMatrix(matrix, lu);

  tmpMat = allocWp43s(m * n * REAL_SIZE);
  for(i = 0; i < n; i++) {
    for(j = 0; j < n; j++) {
      real34ToReal(&lu->matrixElements[i * n + j], &tmpMat[i * n + j]);
    }
  }

  for(k = 0; k < n; k++) {
    /* Find the pivot row */
    pvt = k;
    realCopy(&tmpMat[k * n + k], &u);
    realCopyAbs(&u, &max);
    for(j = k + 1; j < n; j++) {
      realCopy(&tmpMat[j * n + k], &t);
      realCopyAbs(&t, &u);
      if (realCompareGreaterThan(&u, &max)) {
        realCopy(&u, &max);
        pvt = j;
      }
    }
    if(p != NULL)
      *p++ = pvt;

    /* pivot if required */
    if(pvt != k) {
      for(i = 0; i < n; i++) {
        realCopy(&tmpMat[k   * n + i], &t                  );
        realCopy(&tmpMat[pvt * n + i], &tmpMat[k   * n + i]);
        realCopy(&t,                   &tmpMat[pvt * n + i]);
      }
    }

    /* Check for singular */
    realCopy(&tmpMat[k * n + k], &t);
    if(realIsZero(&t)) {
      realMatrixFree(lu);
      return;
    }

    /* Find the lower triangular elements for column k */
    for(i = k + 1; i < n; i++) {
      realCopy(&tmpMat[k * n + k], &t);
      realCopy(&tmpMat[i * n + k], &u);
      realDivide(&u, &t, &max, &ctxtReal39);
      realCopy(&max, &tmpMat[i * n + k]);
    }
    /* Update the upper triangular elements */
    for(i = k + 1; i < n; i++) {
      for(j = k + 1; j < n; j++) {
        realCopy(&tmpMat[i * n + k], &t);
        realCopy(&tmpMat[k * n + j], &u);
        realMultiply(&t, &u, &max, &ctxtReal39);
        realCopy(&tmpMat[i * n + j], &t);
        realSubtract(&t, &max, &u, &ctxtReal39);
        realCopy(&u, &tmpMat[i * n + j]);
      }
    }
  }

  for(i = 0; i < n; i++) {
    for(j = 0; j < n; j++) {
      realToReal34(&tmpMat[i * n + j], &lu->matrixElements[i * n + j]);
    }
  }

  freeWp43s(tmpMat, m * n * REAL_SIZE);
}

void complex_LU_decomposition(const complex34Matrix_t *matrix, complex34Matrix_t *lu, uint16_t *p) {
  int i, j, k;
  int pvt;
  real_t max, t, u, v;
  real_t *tmpMat;

  const uint16_t m = matrix->header.matrixRows;
  const uint16_t n = matrix->header.matrixColumns;

  if(matrix->header.matrixRows != matrix->header.matrixColumns) {
    if(matrix != lu) {
      lu->matrixElements = NULL; // Matrix is not square
      lu->header.matrixRows = lu->header.matrixColumns = 0;
    }
    return;
  }

  if(matrix != lu) copyComplexMatrix(matrix, lu);

  tmpMat = allocWp43s(m * n * REAL_SIZE * 2);
  for(i = 0; i < n; i++) {
    for(j = 0; j < n; j++) {
      real34ToReal(VARIABLE_REAL34_DATA(&lu->matrixElements[i * n + j]), &tmpMat[(i * n + j) * 2    ]);
      real34ToReal(VARIABLE_IMAG34_DATA(&lu->matrixElements[i * n + j]), &tmpMat[(i * n + j) * 2 + 1]);
    }
  }

  for(k = 0; k < n; k++) {
    /* Find the pivot row */
    pvt = k;
    realMultiply(&tmpMat[(k * n + k) * 2    ], &tmpMat[(k * n + k) * 2    ], &u, &ctxtReal39);
    realMultiply(&tmpMat[(k * n + k) * 2 + 1], &tmpMat[(k * n + k) * 2 + 1], &v, &ctxtReal39);
    realAdd(&u, &v, &u, &ctxtReal39);
    realSquareRoot(&u, &max, &ctxtReal39);
    for(j = k + 1; j < n; j++) {
      realMultiply(&tmpMat[(j * n + k) * 2    ], &tmpMat[(j * n + k) * 2    ], &t, &ctxtReal39);
      realMultiply(&tmpMat[(j * n + k) * 2 + 1], &tmpMat[(j * n + k) * 2 + 1], &v, &ctxtReal39);
      realAdd(&t, &v, &t, &ctxtReal39);
      realSquareRoot(&t, &u, &ctxtReal39);
      if(realCompareGreaterThan(&u, &max)) {
        realCopy(&u, &max);
        pvt = j;
      }
    }
    if(p != NULL)
      *p++ = pvt;

    /* pivot if required */
    if(pvt != k) {
      for(i = 0; i < n; i++) {
        realCopy(&tmpMat[(k   * n + i) * 2    ], &t                            );
        realCopy(&tmpMat[(pvt * n + i) * 2    ], &tmpMat[(k   * n + i) * 2    ]);
        realCopy(&t,                             &tmpMat[(pvt * n + i) * 2    ]);
        realCopy(&tmpMat[(k   * n + i) * 2 + 1], &t                            );
        realCopy(&tmpMat[(pvt * n + i) * 2 + 1], &tmpMat[(k   * n + i) * 2 + 1]);
        realCopy(&t,                             &tmpMat[(pvt * n + i) * 2 + 1]);
      }
    }

    /* Check for singular */
    if(realIsZero(&tmpMat[(k * n + k) * 2]) && realIsZero(&tmpMat[(k * n + k) * 2 + 1])) {
      complexMatrixFree(lu);
      return;
    }

    /* Find the lower triangular elements for column k */
    for(i = k + 1; i < n; i++) {
      divComplexComplex(&tmpMat[(i * n + k) * 2], &tmpMat[(i * n + k) * 2 + 1], &tmpMat[(k * n + k) * 2], &tmpMat[(k * n + k) * 2 + 1], &t, &u, &ctxtReal39);
      realCopy(&t, &tmpMat[(i * n + k) * 2    ]);
      realCopy(&u, &tmpMat[(i * n + k) * 2 + 1]);
    }
    /* Update the upper triangular elements */
    for(i = k + 1; i < n; i++) {
      for(j = k + 1; j < n; j++) {
        mulComplexComplex(&tmpMat[(i * n + k) * 2], &tmpMat[(i * n + k) * 2 + 1], &tmpMat[(k * n + j) * 2], &tmpMat[(k * n + j) * 2 + 1], &t, &u, &ctxtReal39);
        realCopy(&tmpMat[(i * n + j) * 2], &v), realCopy(&tmpMat[(i * n + j) * 2 + 1], &max);
        realSubtract(&v,   &t, &tmpMat[(i * n + j) * 2    ], &ctxtReal39);
        realSubtract(&max, &u, &tmpMat[(i * n + j) * 2 + 1], &ctxtReal39);
      }
    }
  }

  for(i = 0; i < n; i++) {
    for(j = 0; j < n; j++) {
      realToReal34(&tmpMat[(i * n + j) * 2    ], VARIABLE_REAL34_DATA(&lu->matrixElements[i * n + j]));
      realToReal34(&tmpMat[(i * n + j) * 2 + 1], VARIABLE_IMAG34_DATA(&lu->matrixElements[i * n + j]));
    }
  }

  freeWp43s(tmpMat, m * n * REAL_SIZE * 2);
}



/* Swap 2 rows */
void realMatrixSwapRows(const real34Matrix_t *matrix, real34Matrix_t *res, uint16_t a, uint16_t b) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  uint16_t i;
  real34_t t;

  if(matrix != res) copyRealMatrix(matrix, res);
  if((a < rows) && (b < rows) && (a != b)) {
    for(i = 0; i < cols; i++) {
      real34Copy(&res->matrixElements[a * cols + i], &t);
      real34Copy(&res->matrixElements[b * cols + i], &res->matrixElements[a * cols + i]);
      real34Copy(&t,                                 &res->matrixElements[b * cols + i]);
    }
  }
}

void complexMatrixSwapRows(const complex34Matrix_t *matrix, complex34Matrix_t *res, uint16_t a, uint16_t b) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  uint16_t i;
  real34_t t;

  if(matrix != res) copyComplexMatrix(matrix, res);
  if((a < rows) && (b < rows) && (a != b)) {
    for(i = 0; i < cols; i++) {
      real34Copy(VARIABLE_REAL34_DATA(&res->matrixElements[a * cols + i]), &t);
      real34Copy(VARIABLE_REAL34_DATA(&res->matrixElements[b * cols + i]), VARIABLE_REAL34_DATA(&res->matrixElements[a * cols + i]));
      real34Copy(&t,                                                       VARIABLE_REAL34_DATA(&res->matrixElements[b * cols + i]));
      real34Copy(VARIABLE_IMAG34_DATA(&res->matrixElements[a * cols + i]), &t);
      real34Copy(VARIABLE_IMAG34_DATA(&res->matrixElements[b * cols + i]), VARIABLE_IMAG34_DATA(&res->matrixElements[a * cols + i]));
      real34Copy(&t,                                                       VARIABLE_IMAG34_DATA(&res->matrixElements[b * cols + i]));
    }
  }
}



/* Determinant */
void detRealMatrix(const real34Matrix_t *matrix, real34_t *res) {
  const uint16_t n = matrix->header.matrixColumns;
  uint16_t *p;
  real34Matrix_t lu;
  real_t t, u;

  if(matrix->header.matrixRows != n) {
    realToReal34(const_NaN, res);
    return;
  }

  p = allocWp43s(TO_BLOCKS(matrix->header.matrixRows * sizeof(uint16_t)));

  WP34S_LU_decomposition(matrix, &lu, p);
  realCopy(const_1, &t);
  if(lu.matrixElements) {
    for(uint16_t i = 0; i < n; ++i) {
      real34ToReal(&lu.matrixElements[i * n + i], &u);
      //if(p[i] != i) realChangeSign(&u);
      realMultiply(&t, &u, &t, &ctxtReal51);
    }
    realToReal34(&t, res);
  }
  else { // singular
    real34Copy(const34_0, res);
  }

  freeWp43s(p, TO_BLOCKS(matrix->header.matrixRows * sizeof(uint16_t)));
}

void detComplexMatrix(const complex34Matrix_t *matrix, real34_t *res_r, real34_t *res_i) {
  const uint16_t n = matrix->header.matrixColumns;
  uint16_t *p;
  complex34Matrix_t lu;
  real_t tr, ti, ur, ui;

  if(matrix->header.matrixRows != n) {
    realToReal34(const_NaN, res_r);
    realToReal34(const_NaN, res_i);
    return;
  }

  p = allocWp43s(TO_BLOCKS(matrix->header.matrixRows * sizeof(uint16_t)));

  complex_LU_decomposition(matrix, &lu, p);
  realCopy(const_1, &tr), realCopy(const_0, &ti);
  if(lu.matrixElements) {
    for(uint16_t i = 0; i < n; ++i) {
      real34ToReal(VARIABLE_REAL34_DATA(&lu.matrixElements[i * n + i]), &ur);
      real34ToReal(VARIABLE_IMAG34_DATA(&lu.matrixElements[i * n + i]), &ui);
      mulComplexComplex(&tr, &ti, &ur, &ui, &tr, &ti, &ctxtReal51);
    }
    realToReal34(&tr, res_r); realToReal34(&ti, res_i);
  }
  else { // singular
    real34Copy(const34_0, res_r); real34Copy(const34_0, res_i);
  }

  freeWp43s(p, TO_BLOCKS(matrix->header.matrixRows * sizeof(uint16_t)));
}



/* Solve the linear equation Ax = b.
 * We do this by utilising the LU decomposition passed in in A and solving
 * the linear equation Ly = b for y, where L is the lower diagonal triangular
 * matrix with unity along the diagonal.  Then we solve the linear system
 * Ux = y, where U is the upper triangular matrix.
 */
static void WP34S_matrix_pivoting_solve(const real34Matrix_t *LU, real_t *b, uint16_t *pivot, real_t *x, realContext_t *realContext) {
  const uint16_t n = LU->header.matrixColumns;
  uint16_t i, k;
  real_t r, t;

  /* Solve the first linear equation Ly = b */
  for(k = 0; k < n; k++) {
    if(k != pivot[k]) {
      real_t swap;
      realCopy(&b[k], &swap);
      realCopy(&b[pivot[k]], &b[k]);
      realCopy(&swap, &b[pivot[k]]);
    }
    realCopy(&b[k], x + k);
    for(i = 0; i < k; i++) {
      real34ToReal(&LU->matrixElements[k * n + i], &r);
      realMultiply(&r, x + i, &t, realContext);
      realSubtract(x + k, &t, x + k, realContext);
    }
  }

  /* Solve the second linear equation Ux = y */
  for(k = n; k > 0; k--) {
    --k;
    for(i = k + 1; i < n; i++) {
      real34ToReal(&LU->matrixElements[k * n + i], &r);
      realMultiply(&r, x + i, &t, realContext);
      realSubtract(x + k, &t, x + k, realContext);
    }
    real34ToReal(&LU->matrixElements[k * n + k], &r);
    realDivide(x + k, &r, x + k, realContext);
    ++k;
  }
}

static void complex_matrix_pivoting_solve(const complex34Matrix_t *LU, real_t *b, uint16_t *pivot, real_t *x, realContext_t *realContext) {
  const uint16_t n = LU->header.matrixColumns;
  uint16_t i, k;
  real_t rr, ri, tr, ti;

  /* Solve the first linear equation Ly = b */
  for(k = 0; k < n; k++) {
    if(k != pivot[k]) {
      real_t swap;
      realCopy(&b[k * 2], &swap);
      realCopy(&b[pivot[k] * 2], &b[k * 2]);
      realCopy(&swap, &b[pivot[k] * 2]);
      realCopy(&b[k * 2 + 1], &swap);
      realCopy(&b[pivot[k] * 2 + 1], &b[k * 2 + 1]);
      realCopy(&swap, &b[pivot[k] * 2 + 1]);
    }
    realCopy(&b[k * 2    ], x + (k * 2    ));
    realCopy(&b[k * 2 + 1], x + (k * 2 + 1));
    for(i = 0; i < k; i++) {
      real34ToReal(VARIABLE_REAL34_DATA(&LU->matrixElements[k * n + i]), &rr);
      real34ToReal(VARIABLE_IMAG34_DATA(&LU->matrixElements[k * n + i]), &ri);
      mulComplexComplex(&rr, &ri, x + i * 2, x + (i * 2 + 1), &tr, &ti, realContext);
      realSubtract(x + (k * 2    ), &tr, x + (k * 2    ), realContext);
      realSubtract(x + (k * 2 + 1), &ti, x + (k * 2 + 1), realContext);
    }
  }

  /* Solve the second linear equation Ux = y */
  for(k = n; k > 0; k--) {
    --k;
    for(i = k + 1; i < n; i++) {
      real34ToReal(VARIABLE_REAL34_DATA(&LU->matrixElements[k * n + i]), &rr);
      real34ToReal(VARIABLE_IMAG34_DATA(&LU->matrixElements[k * n + i]), &ri);
      mulComplexComplex(&rr, &ri, x + i * 2, x + (i * 2 + 1), &tr, &ti, realContext);
      realSubtract(x + (k * 2    ), &tr, x + (k * 2    ), realContext);
      realSubtract(x + (k * 2 + 1), &ti, x + (k * 2 + 1), realContext);
    }
    real34ToReal(VARIABLE_REAL34_DATA(&LU->matrixElements[k * n + k]), &rr);
    real34ToReal(VARIABLE_IMAG34_DATA(&LU->matrixElements[k * n + k]), &ri);
    divComplexComplex(x + (k * 2), x + (k * 2 + 1), &rr, &ri, x + (k * 2), x + (k * 2 + 1), realContext);
    ++k;
  }
}

/* Invert a matrix
 * Do this by calculating the LU decomposition and solving lots of systems
 * of linear equations.
 */
void WP34S_matrix_inverse(const real34Matrix_t *matrix, real34Matrix_t *res) {
  const uint16_t n = matrix->header.matrixColumns;
  real_t *x;
  real34Matrix_t lu;
  uint16_t *pivots;
  uint16_t i, j;
  real_t *b;

  if(matrix->header.matrixRows != matrix->header.matrixColumns) {
    if(matrix != res) {
      res->matrixElements = NULL; // Matrix is not square
      res->header.matrixRows = res->header.matrixColumns = 0;
    }
    return;
  }

  pivots = allocWp43s(TO_BLOCKS(matrix->header.matrixRows * sizeof(uint16_t)));
  WP34S_LU_decomposition(matrix, &lu, pivots);
  if(lu.matrixElements == NULL) {
    freeWp43s(pivots, TO_BLOCKS(matrix->header.matrixRows * sizeof(uint16_t)));
    if(matrix != res) {
      res->matrixElements = NULL; // Singular matrix
      res->header.matrixRows = res->header.matrixColumns = 0;
    }
    return;
  }

  if(matrix != res) copyRealMatrix(matrix, res);

  x = allocWp43s(res->header.matrixRows * REAL_SIZE);
  b = allocWp43s(res->header.matrixRows * REAL_SIZE);
  for(i = 0; i < n; i++) {
    for(j = 0; j < n; j++)
      realCopy((i == j) ? const_1 : const_0, &b[j]);
    WP34S_matrix_pivoting_solve(&lu, b, pivots, x, &ctxtReal39);
    for(j = 0; j < n; j++)
      realToReal34(x + j, &res->matrixElements[j * n + i]);
  }
  freeWp43s(b, res->header.matrixRows * REAL_SIZE);
  freeWp43s(x, res->header.matrixRows * REAL_SIZE);
  freeWp43s(pivots, TO_BLOCKS(matrix->header.matrixRows * sizeof(uint16_t)));
  realMatrixFree(&lu);
}

void complex_matrix_inverse(const complex34Matrix_t *matrix, complex34Matrix_t *res) {
  const uint16_t n = matrix->header.matrixColumns;
  real_t *x;
  complex34Matrix_t lu;
  uint16_t *pivots;
  uint16_t i, j;
  real_t *b;

  if(matrix->header.matrixRows != matrix->header.matrixColumns) {
    if(matrix != res) {
      res->matrixElements = NULL; // Matrix is not square
      res->header.matrixRows = res->header.matrixColumns = 0;
    }
    return;
  }

  pivots = allocWp43s(TO_BLOCKS(matrix->header.matrixRows * sizeof(uint16_t)));
  complex_LU_decomposition(matrix, &lu, pivots);
  if(lu.matrixElements == NULL) {
    freeWp43s(pivots, TO_BLOCKS(matrix->header.matrixRows * sizeof(uint16_t)));
    if(matrix != res) {
      res->matrixElements = NULL; // Singular matrix
      res->header.matrixRows = res->header.matrixColumns = 0;
    }
    return;
  }

  if(matrix != res) copyComplexMatrix(matrix, res);

  x = allocWp43s(res->header.matrixRows * REAL_SIZE * 2);
  b = allocWp43s(res->header.matrixRows * REAL_SIZE * 2);
  for(i = 0; i < n; i++) {
    for(j = 0; j < n; j++) {
      realCopy((i == j) ? const_1 : const_0, &b[j * 2    ]);
      realCopy(                     const_0, &b[j * 2 + 1]);
    }
    complex_matrix_pivoting_solve(&lu, b, pivots, x, &ctxtReal39);
    for(j = 0; j < n; j++) {
      realToReal34(x + (j * 2    ), VARIABLE_REAL34_DATA(&res->matrixElements[j * n + i]));
      realToReal34(x + (j * 2 + 1), VARIABLE_IMAG34_DATA(&res->matrixElements[j * n + i]));
    }
  }
  freeWp43s(b, res->header.matrixRows * REAL_SIZE * 2);
  freeWp43s(x, res->header.matrixRows * REAL_SIZE * 2);
  freeWp43s(pivots, TO_BLOCKS(matrix->header.matrixRows * sizeof(uint16_t)));
  complexMatrixFree(&lu);
}



/* Division */
void divideRealMatrix(const real34Matrix_t *matrix, const real34_t *x, real34Matrix_t *res) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;

  if(matrix != res) realMatrixInit(res, rows, cols);
  for(i = 0; i < cols * rows; ++i) {
    real34Divide(&matrix->matrixElements[i], x, &res->matrixElements[i]);
  }
}

void _divideRealMatrix(const real34Matrix_t *matrix, const real_t *x, real34Matrix_t *res, realContext_t *realContext) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;
  real_t y;

  if(matrix != res) realMatrixInit(res, rows, cols);
  for(i = 0; i < cols * rows; ++i) {
    real34ToReal(&matrix->matrixElements[i], &y);
    realDivide(&y, x, &y, realContext);
    realToReal34(&y, &res->matrixElements[i]);
  }
}

void divideRealMatrices(const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res) {
  real34Matrix_t invX;

  if(y->header.matrixColumns != x->header.matrixRows || x->header.matrixRows != x->header.matrixColumns) {
    res->matrixElements = NULL; // Matrix mismatch
    res->header.matrixRows = res->header.matrixColumns = 0;
    return;
  }

  WP34S_matrix_inverse(x, &invX);
  if(invX.matrixElements == NULL) {
    if(y != res && x != res) {
      res->matrixElements = NULL; // Singular matrix
      res->header.matrixRows = res->header.matrixColumns = 0;
    }
    return;
  }
  multiplyRealMatrices(y, &invX, res);
}

void divideComplexMatrix(const complex34Matrix_t *matrix, const real34_t *xr, const real34_t *xi, complex34Matrix_t *res) {
  real_t _xr, _xi;

  real34ToReal(xr, &_xr); real34ToReal(xi, &_xi);
  _divideComplexMatrix(matrix, &_xr, &_xi, res, &ctxtReal39);
}

void _divideComplexMatrix(const complex34Matrix_t *matrix, const real_t *xr, const real_t *xi, complex34Matrix_t *res, realContext_t *realContext) {
  const uint16_t rows = matrix->header.matrixRows;
  const uint16_t cols = matrix->header.matrixColumns;
  int32_t i;
  real_t yr, yi;

  if(matrix != res) complexMatrixInit(res, rows, cols);
  for(i = 0; i < cols * rows; ++i) {
    real34ToReal(VARIABLE_REAL34_DATA(&matrix->matrixElements[i]), &yr);
    real34ToReal(VARIABLE_IMAG34_DATA(&matrix->matrixElements[i]), &yi);
    divComplexComplex(&yr, &yi, xr, xi, &yr, &yi, realContext);
    realToReal34(&yr, VARIABLE_REAL34_DATA(&res->matrixElements[i]));
    realToReal34(&yi, VARIABLE_IMAG34_DATA(&res->matrixElements[i]));
  }
}

void divideComplexMatrices(const complex34Matrix_t *y, const complex34Matrix_t *x, complex34Matrix_t *res) {
  complex34Matrix_t invX;

  if(y->header.matrixColumns != x->header.matrixRows || x->header.matrixRows != x->header.matrixColumns) {
    res->matrixElements = NULL; // Matrix mismatch
    res->header.matrixRows = res->header.matrixColumns = 0;
    return;
  }

  complex_matrix_inverse(x, &invX);
  if(invX.matrixElements == NULL) {
    if(y != res && x != res) {
      res->matrixElements = NULL; // Singular matrix
      res->header.matrixRows = res->header.matrixColumns = 0;
    }
    return;
  }
  multiplyComplexMatrices(y, &invX, res);
}



/* Solve a system of linear equations Ac = b
 */
void WP34S_matrix_linear_eqn(const real34Matrix_t *a, const real34Matrix_t *b, real34Matrix_t *r) {
  const uint16_t n = a->header.matrixRows;
  int i;
  real34Matrix_t mat;
  real_t *cv = NULL;
  uint16_t *pivots = NULL;
  real_t *bv = NULL;

  if(r != a && r != b) {
    r->matrixElements = NULL;
    r->header.matrixRows = r->header.matrixColumns = 0;
  }

  if(a->header.matrixColumns != n) {
    displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "not a square matrix (%d" STD_CROSS "%d)",
              n, a->header.matrixColumns);
      moreInfoOnError("In function WP34S_matrix_linear_eqn:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }
  if(b->header.matrixRows != n || b->header.matrixColumns != 1) {
    displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "not a column vector or size mismatch (%d" STD_CROSS "%d)",
              b->header.matrixRows, b->header.matrixColumns);
      moreInfoOnError("In function WP34S_matrix_linear_eqn:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  /* Everything is happy so far -- decompose */
  pivots = allocWp43s(TO_BLOCKS(sizeof(uint16_t) * n));
  WP34S_LU_decomposition(a, &mat, pivots);
  if(!mat.matrixElements) {
    freeWp43s(&pivots, TO_BLOCKS(sizeof(uint16_t) * n));
    displayCalcErrorMessage(ERROR_SINGULAR_MATRIX, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "attempt to LU-decompose a singular matrix");
      moreInfoOnError("In function WP34S_matrix_linear_eqn:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  /* And solve */
  if(r == a)
    realMatrixFree(r);
  if(r != b)
    realMatrixInit(r, n, 1);
  cv = allocWp43s(n * REAL_SIZE);
  bv = allocWp43s(n * REAL_SIZE);
  for(i = 0; i < n; i++) {
    real34ToReal(&b->matrixElements[i] , bv + i);
    pivots[i] = i;
  }
  WP34S_matrix_pivoting_solve(&mat, bv, pivots, cv, &ctxtReal39);
  for(i = 0; i < n; i++)
    realToReal34(cv + i, &r->matrixElements[i]);
  freeWp43s(pivots, TO_BLOCKS(sizeof(uint16_t) * n));
  freeWp43s(cv, n * REAL_SIZE);
  freeWp43s(bv, n * REAL_SIZE);
  return;
}

#if 0
void complex_matrix_linear_eqn(const complex34Matrix_t *a, const complex34Matrix_t *b, complex34Matrix_t *r) {
  const uint16_t n = a->header.matrixRows;
  int i;
  complex34Matrix_t mat;
  real_t *cv = NULL;
  uint16_t *pivots = NULL;
  real_t *bv = NULL;

  if(r != a && r != b) {
    r->matrixElements = NULL;
    r->header.matrixRows = r->header.matrixColumns = 0;
  }

  if(a->header.matrixColumns != n) {
    displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "not a square matrix (%d" STD_CROSS "%d)",
              n, a->header.matrixColumns);
      moreInfoOnError("In function complex_matrix_linear_eqn:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }
  if(b->header.matrixRows != n || b->header.matrixColumns != 1) {
    displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "not a column vector or size mismatch (%d" STD_CROSS "%d)",
              b->header.matrixRows, b->header.matrixColumns);
      moreInfoOnError("In function complex_matrix_linear_eqn:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  /* Everything is happy so far -- decompose */
  pivots = allocWp43s(TO_BLOCKS(sizeof(uint16_t) * n));
  complex_LU_decomposition(a, &mat, pivots);
  if(!mat.matrixElements) {
    freeWp43s(&pivots, TO_BLOCKS(sizeof(uint16_t) * n));
    displayCalcErrorMessage(ERROR_SINGULAR_MATRIX, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "attempt to LU-decompose a singular matrix");
      moreInfoOnError("In function complex_matrix_linear_eqn:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  /* And solve */
  if(r == a)
    complexMatrixFree(r);
  if(r != b)
    complexMatrixInit(r, n, 1);
  cv = allocWp43s(n * REAL_SIZE * 2);
  bv = allocWp43s(n * REAL_SIZE * 2);
  for(i = 0; i < n; i++) {
    real34ToReal(VARIABLE_REAL34_DATA(&b->matrixElements[i]) , bv + i * 2    );
    real34ToReal(VARIABLE_IMAG34_DATA(&b->matrixElements[i]) , bv + i * 2 + 1);
    pivots[i] = i;
  }
  complex_matrix_pivoting_solve(&mat, bv, pivots, cv, &ctxtReal39);
  for(i = 0; i < n; i++) {
    realToReal34(cv + i * 2    , VARIABLE_REAL34_DATA(&r->matrixElements[i]));
    realToReal34(cv + i * 2 + 1, VARIABLE_IMAG34_DATA(&r->matrixElements[i]));
  }
  freeWp43s(pivots, TO_BLOCKS(sizeof(uint16_t) * n));
  freeWp43s(cv, n * REAL_SIZE * 2);
  freeWp43s(bv, n * REAL_SIZE * 2);
  return;
}
#endif

void complex_matrix_linear_eqn(const complex34Matrix_t *a, const complex34Matrix_t *b, complex34Matrix_t *r) {
  complex34Matrix_t inv_a;

  if(a->header.matrixColumns != a->header.matrixRows) {
    displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "not a square matrix (%d" STD_CROSS "%d)",
              a->header.matrixRows, a->header.matrixColumns);
      moreInfoOnError("In function complex_matrix_linear_eqn:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }
  if(b->header.matrixRows != a->header.matrixRows || b->header.matrixColumns != 1) {
    displayCalcErrorMessage(ERROR_MATRIX_MISMATCH, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "not a column vector or size mismatch (%d" STD_CROSS "%d)",
              b->header.matrixRows, b->header.matrixColumns);
      moreInfoOnError("In function complex_matrix_linear_eqn:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  complex_matrix_inverse(a, &inv_a);
  if(!inv_a.matrixElements) {
    displayCalcErrorMessage(ERROR_SINGULAR_MATRIX, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "attempt to invert a singular matrix");
      moreInfoOnError("In function complex_matrix_linear_eqn:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  multiplyComplexMatrices(&inv_a, b, r);
  complexMatrixFree(&inv_a);
}
#endif



/* Elementwise function call */
void elementwiseRema(void (*f)(void)) {
#ifndef TESTSUITE_BUILD
  real34Matrix_t x;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertReal34MatrixRegisterToReal34Matrix(REGISTER_X, &x);

  for(int i = 0; i < x.header.matrixRows * x.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&x.matrixElements[i], REGISTER_REAL34_DATA(REGISTER_X));
    f();
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &x.matrixElements[i]);
  }

  convertReal34MatrixToReal34MatrixRegister(&x, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  realMatrixFree(&x);
#endif // TESTSUITE_BUILD
}

void elementwiseRema_UInt16(void (*f)(uint16_t), uint16_t param) {
#ifndef TESTSUITE_BUILD
  real34Matrix_t x;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertReal34MatrixRegisterToReal34Matrix(REGISTER_X, &x);

  for(int i = 0; i < x.header.matrixRows * x.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&x.matrixElements[i], REGISTER_REAL34_DATA(REGISTER_X));
    f(param);
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &x.matrixElements[i]);
  }

  convertReal34MatrixToReal34MatrixRegister(&x, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  realMatrixFree(&x);
#endif // TESTSUITE_BUILD
}

void elementwiseRemaLonI(void (*f)(void)) {
#ifndef TESTSUITE_BUILD
  real34Matrix_t y;
  longInteger_t x;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertReal34MatrixRegisterToReal34Matrix(REGISTER_Y, &y);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  for(int i = 0; i < y.header.matrixRows * y.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&y.matrixElements[i], REGISTER_REAL34_DATA(REGISTER_Y));
    convertLongIntegerToLongIntegerRegister(x, REGISTER_X);
    f();
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &y.matrixElements[i]);
  }

  convertReal34MatrixToReal34MatrixRegister(&y, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  longIntegerFree(x);
  realMatrixFree(&y);
#endif // TESTSUITE_BUILD
}

void elementwiseRemaReal(void (*f)(void)) {
#ifndef TESTSUITE_BUILD
  real34Matrix_t y;
  real34_t x;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertReal34MatrixRegisterToReal34Matrix(REGISTER_Y, &y);
  real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &x);

  for(int i = 0; i < y.header.matrixRows * y.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&y.matrixElements[i], REGISTER_REAL34_DATA(REGISTER_Y));
    real34Copy(&x, REGISTER_REAL34_DATA(REGISTER_X));
    f();
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &y.matrixElements[i]);
  }

  convertReal34MatrixToReal34MatrixRegister(&y, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  realMatrixFree(&y);
#endif // TESTSUITE_BUILD
}

void elementwiseRemaShoI(void (*f)(void)) {
#ifndef TESTSUITE_BUILD
  real34Matrix_t y;
  uint64_t x; uint32_t base; int16_t sign;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertReal34MatrixRegisterToReal34Matrix(REGISTER_Y, &y);
  convertShortIntegerRegisterToUInt64(REGISTER_X, &sign, &x);
  base = getRegisterShortIntegerBase(REGISTER_X);

  for(int i = 0; i < y.header.matrixRows * y.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&y.matrixElements[i], REGISTER_REAL34_DATA(REGISTER_Y));
    convertUInt64ToShortIntegerRegister(sign, x, base, REGISTER_X);
    f();
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &y.matrixElements[i]);
  }

  convertReal34MatrixToReal34MatrixRegister(&y, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  realMatrixFree(&y);
#endif // TESTSUITE_BUILD
}



void elementwiseCxma(void (*f)(void)) {
#ifndef TESTSUITE_BUILD
  complex34Matrix_t x;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertComplex34MatrixRegisterToComplex34Matrix(REGISTER_X, &x);

  for(int i = 0; i < x.header.matrixRows * x.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    complex34Copy(&x.matrixElements[i], REGISTER_COMPLEX34_DATA(REGISTER_X));
    f();
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    if(getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtReal34) {
      real34_t val;
      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &val);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
      real34Copy(&val, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
    }
    complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), &x.matrixElements[i]);
  }

  convertComplex34MatrixToComplex34MatrixRegister(&x, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  complexMatrixFree(&x);
#endif // TESTSUITE_BUILD
}

void elementwiseCxma_UInt16(void (*f)(uint16_t), uint16_t param) {
#ifndef TESTSUITE_BUILD
  complex34Matrix_t x;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertComplex34MatrixRegisterToComplex34Matrix(REGISTER_X, &x);

  for(int i = 0; i < x.header.matrixRows * x.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    complex34Copy(&x.matrixElements[i], REGISTER_COMPLEX34_DATA(REGISTER_X));
    f(param);
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    if(getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtReal34) {
      real34_t val;
      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &val);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
      real34Copy(&val, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
    }
    complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), &x.matrixElements[i]);
  }

  convertComplex34MatrixToComplex34MatrixRegister(&x, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  complexMatrixFree(&x);
#endif // TESTSUITE_BUILD
}

void elementwiseCxmaLonI(void (*f)(void)) {
#ifndef TESTSUITE_BUILD
  complex34Matrix_t y;
  longInteger_t x;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertComplex34MatrixRegisterToComplex34Matrix(REGISTER_Y, &y);
  convertLongIntegerRegisterToLongInteger(REGISTER_X, x);

  for(int i = 0; i < y.header.matrixRows * y.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_Y, dtComplex34, COMPLEX34_SIZE, amNone);
    complex34Copy(&y.matrixElements[i], REGISTER_COMPLEX34_DATA(REGISTER_Y));
    convertLongIntegerToLongIntegerRegister(x, REGISTER_X);
    f();
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    if(getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtReal34) {
      real34_t val;
      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &val);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
      real34Copy(&val, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
    }
    complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), &y.matrixElements[i]);
  }

  convertComplex34MatrixToComplex34MatrixRegister(&y, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  longIntegerFree(x);
  complexMatrixFree(&y);
#endif // TESTSUITE_BUILD
}

void elementwiseCxmaReal(void (*f)(void)) {
#ifndef TESTSUITE_BUILD
  complex34Matrix_t y;
  real34_t x;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertComplex34MatrixRegisterToComplex34Matrix(REGISTER_Y, &y);
  real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &x);

  for(int i = 0; i < y.header.matrixRows * y.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_Y, dtComplex34, COMPLEX34_SIZE, amNone);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    complex34Copy(&y.matrixElements[i], REGISTER_COMPLEX34_DATA(REGISTER_Y));
    real34Copy(&x, REGISTER_REAL34_DATA(REGISTER_X));
    f();
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    if(getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtReal34) {
      real34_t val;
      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &val);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
      real34Copy(&val, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
    }
    complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), &y.matrixElements[i]);
  }

  convertComplex34MatrixToComplex34MatrixRegister(&y, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  complexMatrixFree(&y);
#endif // TESTSUITE_BUILD
}

void elementwiseCxmaShoI(void (*f)(void)) {
#ifndef TESTSUITE_BUILD
  complex34Matrix_t y;
  uint64_t x; uint32_t base; int16_t sign;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertComplex34MatrixRegisterToComplex34Matrix(REGISTER_Y, &y);
  convertShortIntegerRegisterToUInt64(REGISTER_X, &sign, &x);
  base = getRegisterShortIntegerBase(REGISTER_X);

  for(int i = 0; i < y.header.matrixRows * y.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_Y, dtComplex34, COMPLEX34_SIZE, amNone);
    complex34Copy(&y.matrixElements[i], REGISTER_COMPLEX34_DATA(REGISTER_Y));
    convertUInt64ToShortIntegerRegister(sign, x, base, REGISTER_X);
    f();
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    if(getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtReal34) {
      real34_t val;
      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &val);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
      real34Copy(&val, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
    }
    complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), &y.matrixElements[i]);
  }

  convertComplex34MatrixToComplex34MatrixRegister(&y, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  complexMatrixFree(&y);
#endif // TESTSUITE_BUILD
}

void elementwiseCxmaCplx(void (*f)(void)) {
#ifndef TESTSUITE_BUILD
  complex34Matrix_t y;
  complex34_t x;
  const bool_t cpxres = getSystemFlag(FLAG_CPXRES);

  clearSystemFlag(FLAG_CPXRES);
  convertComplex34MatrixRegisterToComplex34Matrix(REGISTER_Y, &y);
  complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), &x);

  for(int i = 0; i < y.header.matrixRows * y.header.matrixColumns; ++i) {
    reallocateRegister(REGISTER_Y, dtComplex34, COMPLEX34_SIZE, amNone);
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    complex34Copy(&y.matrixElements[i], REGISTER_COMPLEX34_DATA(REGISTER_Y));
    complex34Copy(&x, REGISTER_COMPLEX34_DATA(REGISTER_X));
    f();
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    if(getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtReal34) {
      real34_t val;
      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &val);
      reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
      real34Copy(&val, REGISTER_REAL34_DATA(REGISTER_X));
      real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
    }
    complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), &y.matrixElements[i]);
  }

  convertComplex34MatrixToComplex34MatrixRegister(&y, REGISTER_X);
  if(cpxres) setSystemFlag(FLAG_CPXRES);

  complexMatrixFree(&y);
#endif // TESTSUITE_BUILD
}



#ifndef TESTSUITE_BUILD
void callByIndexedMatrix(bool_t (*real_f)(real34Matrix_t *), bool_t (*complex_f)(complex34Matrix_t *)) {
  const int16_t i = getIRegisterAsInt(true);
  const int16_t j = getJRegisterAsInt(true);

  if(matrixIndex == INVALID_VARIABLE || !regInRange(matrixIndex)) {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Cannot execute, destination register is out of range: %d", matrixIndex);
      moreInfoOnError("In function callByIndexedMatrix:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
  else if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
    real34Matrix_t mat;
    convertReal34MatrixRegisterToReal34Matrix(matrixIndex, &mat);
    if(i < 0 || i >= mat.header.matrixRows || j < 0 || j >= mat.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "Cannot execute: element (%" PRId16 ", %" PRId16 ") out of range", (int16_t)(i + 1), (int16_t)(j + 1));
        moreInfoOnError("In function callByIndexedMatrix:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      if(real_f(&mat)) convertReal34MatrixToReal34MatrixRegister(&mat, matrixIndex);
    }
    realMatrixFree(&mat);
  }
  else if(getRegisterDataType(matrixIndex) == dtComplex34Matrix) {
    complex34Matrix_t mat;
    convertComplex34MatrixRegisterToComplex34Matrix(matrixIndex, &mat);
    if(i < 0 || i >= mat.header.matrixRows || j < 0 || j >= mat.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "Cannot execute: element (%" PRId16 ", %" PRId16 ") out of range", (int16_t)(i + 1), (int16_t)(j + 1));
        moreInfoOnError("In function callByIndexedMatrix:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      if(complex_f(&mat)) convertComplex34MatrixToComplex34MatrixRegister(&mat, matrixIndex);
    }
    complexMatrixFree(&mat);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Cannot execute: something other than a matrix is indexed %s", getRegisterDataTypeName(REGISTER_X, true, false));
      moreInfoOnError("In function callByIndexedMatrix:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}
#endif // TESTSUITE_BUILD
