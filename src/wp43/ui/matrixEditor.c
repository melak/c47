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
 * \file ui/matrixEditor.c
 ***********************************************/

#include "ui/matrixEditor.h"

#include "bufferize.h"
#include "c43Extensions/jm.h"
#include "calcMode.h"
#include "charString.h"
#include "constantPointers.h"
#include "conversionAngles.h"
#include "display.h"
#include "error.h"
#include "flags.h"
#include "fonts.h"
#include "hal/gui.h"
#include "longIntegerType.h"
#include "items.h"
#include "mathematics/matrix.h"
#include "mathematics/toPolar.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "screen.h"
#include "softmenus.h"
#include "wp43.h"
#include <string.h>

#if !defined(TESTSUITE_BUILD)
  any34Matrix_t         openMatrixMIMPointer;
  bool_t                matEditMode;
  uint16_t              scrollRow;
  uint16_t              scrollColumn;
  uint16_t              tmpRow;
  uint16_t              matrixIndex = INVALID_VARIABLE;

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
#endif // !defined(TESTSUITE_BUILD)

void fnEditMatrix(uint16_t regist) {
  #if !defined(TESTSUITE_BUILD)
  saveStatsMatrix();
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
      #if defined(PC_BUILD)
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(reg));
    moreInfoOnError("In function fnEditMatrix:", errorMessage, "is not a matrix.", "");
      #endif // PC_BUILD
    }
  #endif // !TESTSUITE_BUILD
}


void fnOldMatrix(uint16_t unusedParamButMandatory) {
  #if !defined(TESTSUITE_BUILD)
  if(calcMode == CM_MIM) {
    aimBuffer[0] = 0;
    nimBufferDisplay[0] = 0;
    hideCursor();
    cursorEnabled = false;

    if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
        if(openMatrixMIMPointer.realMatrix.matrixElements) {
          realMatrixFree(&openMatrixMIMPointer.realMatrix);
        }
      convertReal34MatrixRegisterToReal34Matrix(matrixIndex, &openMatrixMIMPointer.realMatrix);
    }
    else {
        if(openMatrixMIMPointer.complexMatrix.matrixElements) {
          complexMatrixFree(&openMatrixMIMPointer.complexMatrix);
        }
      convertComplex34MatrixRegisterToComplex34Matrix(matrixIndex, &openMatrixMIMPointer.complexMatrix);
    }
  }
  else {
    displayCalcErrorMessage(ERROR_OPERATION_UNDEFINED, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
      #if defined(PC_BUILD)
    sprintf(errorMessage, "works in MIM only");
    moreInfoOnError("In function fnOldMatrix:", errorMessage, NULL, NULL);
      #endif // PC_BUILD
    }
  #endif // !TESTSUITE_BUILD
}


void fnGoToElement(uint16_t unusedParamButMandatory) {
  #if !defined(TESTSUITE_BUILD)
  if(calcMode == CM_MIM) {
    mimEnter(false);
    runFunction(ITM_M_GOTO_ROW);
  }
  else {
    displayCalcErrorMessage(ERROR_OPERATION_UNDEFINED, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
      #if defined(PC_BUILD)
      sprintf(errorMessage, "works in MIM only");
      moreInfoOnError("In function fnGoToElement:", errorMessage, NULL, NULL);
      #endif // PC_BUILD
    }
  #endif // !TESTSUITE_BUILD
}


void fnGoToRow(uint16_t row) {
  #if !defined(TESTSUITE_BUILD)
  if(calcMode == CM_MIM) {
    tmpRow = row;
  }
  else {
    displayCalcErrorMessage(ERROR_OPERATION_UNDEFINED, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
      #if defined(PC_BUILD)
      sprintf(errorMessage, "works in MIM only");
      moreInfoOnError("In function fnGoToRow:", errorMessage, NULL, NULL);
      #endif // PC_BUILD
    }
  #endif // !TESTSUITE_BUILD
}


void fnGoToColumn(uint16_t col) {
  #if !defined(TESTSUITE_BUILD)
  if(calcMode == CM_MIM) {
    if(tmpRow == 0 || tmpRow > openMatrixMIMPointer.header.matrixRows || col == 0 || col > openMatrixMIMPointer.header.matrixColumns) {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      #if(EXTRA_INFO_ON_CALC_ERROR == 1)
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
      calcModeNormalGui();
  }
  else {
    displayCalcErrorMessage(ERROR_OPERATION_UNDEFINED, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
      #if defined(PC_BUILD)
      sprintf(errorMessage, "works in MIM only");
      moreInfoOnError("In function fnGoToColumn:", errorMessage, NULL, NULL);
      #endif // PC_BUILD
    }
  #endif // !TESTSUITE_BUILD
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
  #if !defined(TESTSUITE_BUILD)
  callByIndexedMatrix((mode == DEC_FLAG) ? decIReal : incIReal, (mode == DEC_FLAG) ? decIComplex : incIComplex);
  #endif // !TESTSUITE_BUILD
}


void fnIncDecJ(uint16_t mode) {
  #if !defined(TESTSUITE_BUILD)
  callByIndexedMatrix((mode == DEC_FLAG) ? decJReal : incJReal, (mode == DEC_FLAG) ? decJComplex : incJComplex);
  #endif // !TESTSUITE_BUILD
}


void fnInsRow(uint16_t unusedParamButMandatory) {
  #if !defined(TESTSUITE_BUILD)
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
      #if defined(PC_BUILD)
      sprintf(errorMessage, "works in MIM only");
      moreInfoOnError("In function fnGoToElement:", errorMessage, NULL, NULL);
      #endif // PC_BUILD
    }
  #endif // !TESTSUITE_BUILD
}


void fnDelRow(uint16_t unusedParamButMandatory) {
  #if !defined(TESTSUITE_BUILD)
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
      #if defined(PC_BUILD)
      sprintf(errorMessage, "works in MIM only");
      moreInfoOnError("In function fnGoToElement:", errorMessage, NULL, NULL);
      #endif // PC_BUILD
    }
  #endif // !TESTSUITE_BUILD
}


#if !defined(TESTSUITE_BUILD)
//Row of Matrix
int16_t getIRegisterAsInt(bool_t asArrayPointer) {
  int16_t ret;
  longInteger_t tmp_lgInt;

  if(getRegisterDataType(REGISTER_I) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_I, tmp_lgInt);
  }
  else if(getRegisterDataType(REGISTER_I) == dtReal34) {
    convertReal34ToLongInteger(REGISTER_REAL34_DATA(REGISTER_I), tmp_lgInt, DEC_ROUND_DOWN);
  }
  else {
    longIntegerInit(tmp_lgInt);
  }
  longIntegerToInt(tmp_lgInt, ret);

  longIntegerFree(tmp_lgInt);

  if(asArrayPointer) {
    ret--;
  }

  return ret;
}

//Col of Matrix
int16_t getJRegisterAsInt(bool_t asArrayPointer) {
  int16_t ret;
  longInteger_t tmp_lgInt;

  if(getRegisterDataType(REGISTER_J) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_J, tmp_lgInt);
  }
  else if(getRegisterDataType(REGISTER_J) == dtReal34) {
    convertReal34ToLongInteger(REGISTER_REAL34_DATA(REGISTER_J), tmp_lgInt, DEC_ROUND_DOWN);
  }
  else {
    longIntegerInit(tmp_lgInt);
  }
  longIntegerToInt(tmp_lgInt, ret);

  longIntegerFree(tmp_lgInt);

  if(asArrayPointer) {
    ret--;
  }

  return ret;
}

//Row of Matrix
void setIRegisterAsInt(bool_t asArrayPointer, int16_t toStore) {
  if(asArrayPointer) {
    toStore++;
  }
  longInteger_t tmp_lgInt;
  longIntegerInit(tmp_lgInt);

  intToLongInteger(toStore, tmp_lgInt);
  convertLongIntegerToLongIntegerRegister(tmp_lgInt, REGISTER_I);

  longIntegerFree(tmp_lgInt);
}

//ColOfMatrix
void setJRegisterAsInt(bool_t asArrayPointer, int16_t toStore) {
  if(asArrayPointer) {
    toStore++;
  }
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
  }
  else {
    if(getIRegisterAsInt(true) == rows) {
      setIRegisterAsInt(true, 0);
      setJRegisterAsInt(true, (getJRegisterAsInt(true) == cols - 1) ? 0 : getJRegisterAsInt(true) + 1);
    }
  }

  if(getJRegisterAsInt(true) < 0) {
    setJRegisterAsInt(true, cols - 1);
    setIRegisterAsInt(true, (getIRegisterAsInt(true) == 0) ? rows - 1 : getIRegisterAsInt(true) - 1);
  }
  else {
    if(getJRegisterAsInt(true) == cols) {
      setJRegisterAsInt(true, 0);
      setIRegisterAsInt(true, ((!getSystemFlag(FLAG_GROW)) && (getIRegisterAsInt(true) == rows - 1)) ? 0 : getIRegisterAsInt(true) + 1);
    }
  }

  return getIRegisterAsInt(true) == rows;
}

void showMatrixEditor() {
  int rows = openMatrixMIMPointer.header.matrixRows;
  int cols = openMatrixMIMPointer.header.matrixColumns;
  int16_t width = 0;

  for(int i = 0; i < SOFTMENU_STACK_SIZE; i++) {
    if(softmenu[softmenuStack[i].softmenuId].menuItem == -MNU_M_EDIT) {
      width = 1;
      break;
    }
  }
  if(width == 0) {
    showSoftmenu(-MNU_M_EDIT);
  }
  if(softmenu[softmenuStack[0].softmenuId].menuItem == -MNU_M_EDIT) {
    calcModeNormalGui();
  }

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

  if(aimBuffer[0] == 0) {
    clearRegisterLine(NIM_REGISTER_LINE, true, true);
    if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
      showRealMatrix(&openMatrixMIMPointer.realMatrix, 0);
    }
    else {
      showComplexMatrix(&openMatrixMIMPointer.complexMatrix, 0);
    }
  }
  else {
    clearRegisterLine(NIM_REGISTER_LINE, false, true);
  }

  sprintf(tmpString, "%" PRIi16 ";%" PRIi16 "=" STD_SPACE_4_PER_EM "%s%s%s", (int16_t)(colVector ? matSelCol+1 : matSelRow+1), (int16_t)(colVector ? 1 : matSelCol+1), aimBuffer[0] == 0 ? STD_SPACE_HAIR : "", (aimBuffer[0] == 0 || aimBuffer[0] == '-') ? "" : " ", nimBufferDisplay);
  width = stringWidth(tmpString, &numericFont, true, true) + 1;
  if(aimBuffer[0] == 0) {
    if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
      real34ToDisplayString(&openMatrixMIMPointer.realMatrix.matrixElements[matSelRow*cols+matSelCol], amNone, &tmpString[strlen(tmpString)], &numericFont, SCREEN_WIDTH - width, NUMBER_OF_DISPLAY_DIGITS, true, true);
    }
    else {
      complex34ToDisplayString(&openMatrixMIMPointer.complexMatrix.matrixElements[matSelRow*cols+matSelCol], &tmpString[strlen(tmpString)], &numericFont, SCREEN_WIDTH - width, NUMBER_OF_DISPLAY_DIGITS, true, true, currentAngularMode, getSystemFlag(FLAG_POLAR));
    }

    showString(tmpString, &numericFont, 0, Y_POSITION_OF_NIM_LINE, vmNormal, true, false);
  }
  else {
    if(aimBuffer[0] != 0 && aimBuffer[strlen(aimBuffer)-1]=='/') {
      char lastBase[12];
      char *lb = lastBase;
      if(lastDenominator >= 1000) {
        *(lb++) = STD_SUB_0[0];
        *(lb++) = STD_SUB_0[1] + (lastDenominator / 1000);
      }
      if(lastDenominator >= 100) {
        *(lb++) = STD_SUB_0[0];
        *(lb++) = STD_SUB_0[1] + (lastDenominator % 1000 / 100);
      }
      if(lastDenominator >= 10) {
        *(lb++) = STD_SUB_0[0];
        *(lb++) = STD_SUB_0[1] + (lastDenominator % 100 / 10);
      }
      *(lb++) = STD_SUB_0[0];
      *(lb++) = STD_SUB_0[1] + (lastDenominator % 10);
      *(lb++) = 0;
      displayNim(tmpString, lastBase, stringWidth(lastBase, &numericFont, true, true), stringWidth(lastBase, &standardFont, true, true));
    }
    else {
      displayNim(tmpString, "", 0, 0);
    }
  }

  if(temporaryInformation == TI_SHOW_REGISTER && calcMode == CM_MIM) {
    mimShowElement();
    clearRegisterLine(REGISTER_T, true, true);
    refreshRegisterLine(REGISTER_T);
    if(tmpString[300]) {
      clearRegisterLine(REGISTER_Z, true, true);
      refreshRegisterLine(REGISTER_Z);
    }
  }

  if(lastErrorCode != ERROR_NONE) {
    refreshRegisterLine(errorMessageRegisterLine);
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
    if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
      convertReal34MatrixToReal34MatrixRegister(&openMatrixMIMPointer.realMatrix, matrixIndex);
    }
    else {
      convertComplex34MatrixToComplex34MatrixRegister(&openMatrixMIMPointer.complexMatrix, matrixIndex);
    }
  }
  updateMatrixHeightCache();
}

static void _resetCursorPos() {
  clearRegisterLine(NIM_REGISTER_LINE, false, true);
  sprintf(tmpString, "%" PRIi16";%" PRIi16"= ", (int16_t)getIRegisterAsInt(false), (int16_t)getJRegisterAsInt(false));
  xCursor = showString(tmpString, &numericFont, 0, Y_POSITION_OF_NIM_LINE, vmNormal, true, true) + 1;
  yCursor = Y_POSITION_OF_NIM_LINE;
  cursorEnabled = true;
  cursorFont = &numericFont;
  lastIntegerBase = 0;
}

void mimAddNumber(int16_t item) {
  const int cols = openMatrixMIMPointer.header.matrixColumns;
  const int16_t row = getIRegisterAsInt(true);
  const int16_t col = getJRegisterAsInt(true);

  switch(item) {
      case ITM_EXPONENT: {
      if(aimBuffer[0] == 0) {
        aimBuffer[0] = '+';
        aimBuffer[1] = '1';
        aimBuffer[2] = '.';
        aimBuffer[3] = 0;
        nimNumberPart = NP_REAL_FLOAT_PART;
        _resetCursorPos();
      }
      break;
      }

      case ITM_PERIOD: {
      if(aimBuffer[0] == 0) {
        aimBuffer[0] = '+';
        aimBuffer[1] = '0';
        aimBuffer[2] = 0;
        nimNumberPart = NP_INT_10;
        _resetCursorPos();
      }
      break;
      }

    case ITM_0 :
    case ITM_1 :
    case ITM_2 :
    case ITM_3 :
    case ITM_4 :
    case ITM_5 :
    case ITM_6 :
    case ITM_7 :
    case ITM_8 :
      case ITM_9: {
      if(aimBuffer[0] == 0) {
        aimBuffer[0] = '+';
        aimBuffer[1] = 0;
        nimNumberPart = NP_INT_10;
        _resetCursorPos();
      }
      break;
      }

      case ITM_BACKSPACE: {
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
      }

      case ITM_CHS: {
      if(aimBuffer[0] == 0) {
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
      }

      case ITM_op_j:
      case ITM_CC: {
        if(aimBuffer[0] == 0) {
          return;
        }
      break;
      }

      case ITM_CONSTpi: {
      if(aimBuffer[0] == 0) {
        if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
          realToReal34(const_pi, &openMatrixMIMPointer.realMatrix.matrixElements[row * cols + col]);
        }
        else {
          realToReal34(const_pi, VARIABLE_REAL34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[row * cols + col]));
          real34Zero(VARIABLE_IMAG34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[row * cols + col]));
        }
      }
      else if(nimNumberPart == NP_COMPLEX_INT_PART && aimBuffer[strlen(aimBuffer) - 1] == 'i') {
        strcat(aimBuffer, "3.141592653589793238462643383279503");
        reallyRunFunction(ITM_ENTER, NOPARAM);
      }
      return;
      }

      default: {
      return;
  }
    }
  addItemToNimBuffer(item);
  calcMode = CM_MIM;
}

void mimRunFunction(int16_t func, uint16_t param) {
  int16_t i = getIRegisterAsInt(true);
  int16_t j = getJRegisterAsInt(true);
  bool_t isComplex = (getRegisterDataType(matrixIndex) == dtComplex34Matrix);
  real34_t re, im, re1, im1;
  bool_t converted = false;
  bool_t liftStackFlag = getSystemFlag(FLAG_ASLIFT);

  if(isComplex) {
    real34Copy(VARIABLE_REAL34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j]), &re1);
    real34Copy(VARIABLE_IMAG34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j]), &im1);
  }
  else {
    real34Copy(&openMatrixMIMPointer.realMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j], &re1);
    real34Zero(&im1);
  }

  mimEnter(true);
  clearSystemFlag(FLAG_ASLIFT);
  lastErrorCode = ERROR_NONE;

  if(isComplex) {
    real34Copy(VARIABLE_REAL34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j]), &re);
    real34Copy(VARIABLE_IMAG34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j]), &im);
  }
  else {
    real34Copy(&openMatrixMIMPointer.realMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j], &re);
    real34Zero(&im);
  }
  if(isComplex) {
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    real34Copy(&re, REGISTER_REAL34_DATA(REGISTER_X));
    real34Copy(&im, REGISTER_IMAG34_DATA(REGISTER_X));
  }
  else {
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&re, REGISTER_REAL34_DATA(REGISTER_X));
  }

  reallyRunFunction(func, param);

  switch(getRegisterDataType(REGISTER_X)) {
      case dtLongInteger: {
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      break;
      }
      case dtShortInteger: {
      convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      break;
      }
    case dtReal34:
      case dtComplex34: {
      break;
      }
      default: {
      lastErrorCode = ERROR_INVALID_DATA_TYPE_FOR_OP;
  }
    }

  if(lastErrorCode == ERROR_NONE) {
    if(isComplex && getRegisterDataType(REGISTER_X) == dtComplex34) {
      complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), &openMatrixMIMPointer.complexMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j]);
    }
    else if(isComplex) {
      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), VARIABLE_REAL34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j]));
      real34Zero(                                  VARIABLE_IMAG34_DATA(&openMatrixMIMPointer.complexMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j]));
    }
    else if(getRegisterDataType(REGISTER_X) == dtComplex34) { // Convert to a complex matrix
      complex34Matrix_t cxma;
      complex34_t ans;

      complex34Copy(REGISTER_COMPLEX34_DATA(REGISTER_X), &ans);
      converted = true;
      convertReal34MatrixToComplex34Matrix(&openMatrixMIMPointer.realMatrix, &cxma);
      realMatrixFree(&openMatrixMIMPointer.realMatrix);
      convertComplex34MatrixToComplex34MatrixRegister(&cxma, matrixIndex);
      openMatrixMIMPointer.complexMatrix.header.matrixRows = cxma.header.matrixRows;
      openMatrixMIMPointer.complexMatrix.header.matrixColumns = cxma.header.matrixColumns;
      openMatrixMIMPointer.complexMatrix.matrixElements = cxma.matrixElements;

      complex34Copy(&ans, &openMatrixMIMPointer.complexMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j]);
    }
    else {
      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &openMatrixMIMPointer.realMatrix.matrixElements[i * openMatrixMIMPointer.header.matrixColumns + j]);
    }
  }

  if(matrixIndex == REGISTER_X && !converted) {
    if(isComplex) {
      complex34Matrix_t linkedMatrix;
      convertComplex34MatrixToComplex34MatrixRegister(&openMatrixMIMPointer.complexMatrix, REGISTER_X);
      linkToComplexMatrixRegister(REGISTER_X, &linkedMatrix);
      real34Copy(&re1, VARIABLE_REAL34_DATA(&linkedMatrix.matrixElements[i * linkedMatrix.header.matrixColumns + j]));
      real34Copy(&im1, VARIABLE_IMAG34_DATA(&linkedMatrix.matrixElements[i * linkedMatrix.header.matrixColumns + j]));
    }
    else {
      real34Matrix_t linkedMatrix;
      convertReal34MatrixToReal34MatrixRegister(&openMatrixMIMPointer.realMatrix, REGISTER_X);
      linkToRealMatrixRegister(REGISTER_X, &linkedMatrix);
      real34Copy(&re1, &linkedMatrix.matrixElements[i * linkedMatrix.header.matrixColumns + j]);
    }
  }

    if(liftStackFlag) {
      setSystemFlag(FLAG_ASLIFT);
    }

  updateMatrixHeightCache();
    #if defined(PC_BUILD)
    refreshLcd(NULL);
  #endif // PC_BUILD
}

void mimFinalize(void) {
  if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
      if(openMatrixMIMPointer.realMatrix.matrixElements) {
      realMatrixFree(&openMatrixMIMPointer.realMatrix);
  }
    }
  else if(getRegisterDataType(matrixIndex) == dtComplex34Matrix) {
      if(openMatrixMIMPointer.complexMatrix.matrixElements) {
      complexMatrixFree(&openMatrixMIMPointer.complexMatrix);
  }
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

#define NUMERIC_FONT_HEIGHT_ (NUMERIC_FONT_HEIGHT - 4)        // reduce font spacing to easily bind the matrix lines without any complicated pixel manipulation 
#define STANDARD_FONT_HEIGHT_ (STANDARD_FONT_HEIGHT - 2)      // reduce font spacing to easily bind the matrix lines without any complicated pixel manipulation

void showRealMatrix(const real34Matrix_t *matrix, int16_t prefixWidth) {
  int rows = matrix->header.matrixRows;
  int cols = matrix->header.matrixColumns;
  int16_t Y_POS = Y_POSITION_OF_REGISTER_X_LINE;
  int16_t X_POS = 0;
  int16_t totalWidth = 0, width = 0;
  const font_t *font;
  int16_t fontHeight = NUMERIC_FONT_HEIGHT_;
  int16_t maxWidth = MATRIX_LINE_WIDTH - prefixWidth;
  int16_t colWidth[MATRIX_MAX_COLUMNS] = {}, rPadWidth[MATRIX_MAX_ROWS * MATRIX_MAX_COLUMNS] = {};
  const bool_t forEditor = matrix == &openMatrixMIMPointer.realMatrix;
  const uint16_t sRow = forEditor ? scrollRow : 0;
  uint16_t sCol = forEditor ? scrollColumn : 0;
  const uint16_t tmpDisplayFormat = displayFormat;
  const uint8_t tmpDisplayFormatDigits = displayFormatDigits;

  Y_POS = Y_POSITION_OF_REGISTER_X_LINE - NUMERIC_FONT_HEIGHT_;

  bool_t colVector = false;
  if(cols == 1 && rows > 1) {
    colVector = true;
    cols = rows;
    rows = 1;
  }

  uint16_t maxCols = cols > MATRIX_MAX_COLUMNS ? MATRIX_MAX_COLUMNS : cols;
  const uint16_t maxRows = rows > MATRIX_MAX_ROWS ? MATRIX_MAX_ROWS : rows;
    if(maxCols + sCol >= cols) {
      maxCols = cols - sCol;
    }

  int16_t matSelRow = colVector ? getJRegisterAsInt(true) : getIRegisterAsInt(true);
  int16_t matSelCol = colVector ? getIRegisterAsInt(true) : getJRegisterAsInt(true);

  videoMode_t vm = vmNormal;

  font = &numericFont;
  if(rows >= (forEditor ? 4 : 5)){
smallFont:
    font = &standardFont;
    fontHeight = STANDARD_FONT_HEIGHT_;
    Y_POS = Y_POSITION_OF_REGISTER_X_LINE - STANDARD_FONT_HEIGHT_ + 2;
    //maxWidth = MATRIX_LINE_WIDTH_SMALL * 4 - 20;
  }

    if(!forEditor) {
      Y_POS += REGISTER_LINE_HEIGHT;
    }
  const bool_t rightEllipsis = (cols > maxCols) && (cols > maxCols + sCol);
  const bool_t leftEllipsis = (sCol > 0);
  int16_t digits;

    if(prefixWidth > 0) {
      Y_POS = Y_POSITION_OF_REGISTER_T_LINE - REGISTER_LINE_HEIGHT + 1 + maxRows * fontHeight;
    }
    if(prefixWidth > 0 && font == &standardFont) {
      Y_POS += (maxRows == 1 ? STANDARD_FONT_HEIGHT_ : REGISTER_LINE_HEIGHT - STANDARD_FONT_HEIGHT_);
    }

    int16_t baseWidth = (leftEllipsis ? stringWidth(STD_ELLIPSIS " ", font, true, true) : 0) + (rightEllipsis ? stringWidth(" " STD_ELLIPSIS, font, true, true) : 0);
  int16_t mtxWidth = getRealMatrixColumnWidths(matrix, prefixWidth, font, colWidth, rPadWidth, &digits, maxCols);
  bool_t noFix = (mtxWidth < 0);
  mtxWidth = abs(mtxWidth);
  totalWidth = baseWidth + mtxWidth;
  if(displayFormat == DF_ALL && noFix) {
    displayFormat = getSystemFlag(FLAG_ALLENG) ? DF_ENG : DF_SCI;
    displayFormatDigits = digits;
  }
  if(totalWidth > maxWidth || leftEllipsis) {
    if(font == &numericFont) {
      displayFormat = tmpDisplayFormat;
      displayFormatDigits = tmpDisplayFormatDigits;
      goto smallFont;
    }
    else {
      displayFormat = DF_SCI;
      displayFormatDigits = 3;
      mtxWidth = getRealMatrixColumnWidths(matrix, prefixWidth, font, colWidth, rPadWidth, &digits, maxCols);
      noFix = (mtxWidth < 0);
      mtxWidth = abs(mtxWidth);
      totalWidth = baseWidth + mtxWidth;
      if(totalWidth > maxWidth) {
        maxCols--;
        goto smallFont;
      }
    }
  }
  if(forEditor) {
    if((matSelCol < sCol) && leftEllipsis) {
      scrollColumn--;
      sCol--;
      goto smallFont;
    }
    else if((matSelCol >= sCol + maxCols) && rightEllipsis) {
      scrollColumn++;
      sCol++;
      goto smallFont;
    }
  }
    for(int j = 0; j < maxCols; j++) {
      baseWidth += colWidth[j] + stringWidth(STD_SPACE_FIGURE, font, true, true);
    }
  baseWidth -= stringWidth(STD_SPACE_FIGURE, font, true, true);

    if(prefixWidth > 0) {
      X_POS = prefixWidth;
    }
    else if(!forEditor) {
      X_POS = SCREEN_WIDTH - ((colVector ? stringWidth("[]" STD_SUP_T, font, true, true) : stringWidth("[]", font, true, true)) + baseWidth) - (font == &standardFont ? 0 : 1);
    }

  if(forEditor) {
    clearRegisterLine(REGISTER_X, true, true);
    clearRegisterLine(REGISTER_Y, true, true);
      if(rows >= (font == &standardFont ? 3 : 2)) {
        clearRegisterLine(REGISTER_Z, true, true);
      }
      if(rows >= (font == &standardFont ? 4 : 3)) {
        clearRegisterLine(REGISTER_T, true, true);
      }
  }
  else if(prefixWidth > 0) {
    clearRegisterLine(REGISTER_T, true, true);
      if(rows >= 2) {
        clearRegisterLine(REGISTER_Z, true, true);
      }
      if(rows >= (font == &standardFont ? 4 : 3)) {
        clearRegisterLine(REGISTER_Y, true, true);
      }
      if(rows == 4 && font != &standardFont) {
        clearRegisterLine(REGISTER_X, true, true);
      }
  }

  for(int i = 0; i < maxRows; i++) {
    int16_t colX = stringWidth("[", font, true, true);
    showString((maxRows == 1) ? "[" : (i == 0) ? STD_MAT_TL : (i + 1 == maxRows) ? STD_MAT_BL : STD_MAT_ML, font, X_POS + 1, Y_POS - (maxRows -1 - i) * fontHeight, vmNormal, true, false);
    if(leftEllipsis) {
      showString(STD_ELLIPSIS " ", font, X_POS + stringWidth("[", font, true, true), Y_POS - (maxRows -1 -i) * fontHeight, vmNormal, true, false);
      colX += stringWidth(STD_ELLIPSIS " ", font, true, true);
    }
    for(int j = 0; j < maxCols + (rightEllipsis ? 1 : 0); j++) {
      if(((i == maxRows - 1) && (rows > maxRows + sRow)) || ((j == maxCols) && rightEllipsis) || ((i == 0) && (sRow > 0))) {
        strcpy(tmpString, " " STD_ELLIPSIS);
        vm = vmNormal;
      }
      else {
        real34ToDisplayString(&matrix->matrixElements[(i+sRow)*cols+j+sCol], amNone, tmpString, font, colWidth[j], displayFormat == DF_ALL ? digits : 15, true, true);
        if(forEditor && matSelRow == (i + sRow) && matSelCol == (j + sCol)) {
          lcd_fill_rect(X_POS + colX, Y_POS - (maxRows -1 -i) * fontHeight, colWidth[j], font == &numericFont ? 32 : 20, 0xFF);
          vm = vmReverse;
        }
        else {
          vm = vmNormal;
        }
      }
      width = stringWidth(tmpString, font, true, true) + 1;
      showString(tmpString, font, X_POS + colX + (((j == maxCols) && rightEllipsis) ? -stringWidth(" ", font, true, true) : (colWidth[j] - width) - rPadWidth[i * MATRIX_MAX_COLUMNS + j]), Y_POS - (maxRows -1 -i) * fontHeight, vm, true, false);
      colX += colWidth[j] + stringWidth(STD_SPACE_FIGURE, font, true, true);
    }
    showString((maxRows == 1) ? "]" : (i == 0) ? STD_MAT_TR : (i + 1 == maxRows) ? STD_MAT_BR : STD_MAT_MR, font, X_POS + stringWidth("[", font, true, true) + baseWidth, Y_POS - (maxRows -1 -i) * fontHeight, vmNormal, true, false);
    if(colVector == true) {
      showString(STD_SUP_T, font, X_POS + stringWidth("[]", font, true, true) + baseWidth, Y_POS - (maxRows -1 -i) * fontHeight, vmNormal, true, false);
    }
  }

  displayFormat = tmpDisplayFormat;
  displayFormatDigits = tmpDisplayFormatDigits;

}

int16_t getRealMatrixColumnWidths(const real34Matrix_t *matrix, int16_t prefixWidth, const font_t *font, int16_t *colWidth, int16_t *rPadWidth, int16_t *digits, uint16_t maxCols) {
  const bool_t colVector = matrix->header.matrixColumns == 1 && matrix->header.matrixRows > 1;
  const int rows = colVector ? 1 : matrix->header.matrixRows;
  const int cols = colVector ? matrix->header.matrixRows : matrix->header.matrixColumns;
  const int maxRows = rows > MATRIX_MAX_ROWS ? MATRIX_MAX_ROWS : rows;
  const bool_t forEditor = matrix == &openMatrixMIMPointer.realMatrix;
  const uint16_t sRow = forEditor ? scrollRow : 0;
  const uint16_t sCol = forEditor ? scrollColumn : 0;
  const int16_t maxWidth = MATRIX_LINE_WIDTH - prefixWidth;
  int16_t totalWidth = 0, width = 0;
  int16_t maxRightWidth[MATRIX_MAX_COLUMNS] = {};
  int16_t maxLeftWidth[MATRIX_MAX_COLUMNS] = {};
  const int16_t exponentOutOfRange = 0x4000;
  bool_t noFix = false; const int16_t dspDigits = displayFormatDigits;

  begin:
  for(int k = 15; k >= 1; k--) {
      if(displayFormat == DF_ALL) {
        *digits = k;
      }
    if(displayFormat == DF_ALL && noFix) { // something like SCI
      displayFormat = getSystemFlag(FLAG_ALLENG) ? DF_ENG : DF_SCI;
      displayFormatDigits = k;
    }
    for(int i = 0; i < maxRows; i++) {
      for(int j = 0; j < maxCols; j++) {
        real34_t r34Val;
        real34Copy(&matrix->matrixElements[(i+sRow)*cols+j+sCol], &r34Val);
        real34SetPositiveSign(&r34Val);
        real34ToDisplayString(&r34Val, amNone, tmpString, font, maxWidth, displayFormat == DF_ALL ? k : 15, true, true);
        if(displayFormat == DF_ALL && !noFix && strstr(tmpString, STD_SUB_10)) { // something like SCI
          noFix = true;
          totalWidth = 0;
            for(int p = 0; p < MATRIX_MAX_COLUMNS; ++p) {
            maxRightWidth[p] = maxLeftWidth[p] = 0;
            }
          goto begin; // redo
        }
        width = stringWidth(tmpString, font, true, true) + 1;
        rPadWidth[i * MATRIX_MAX_COLUMNS + j] = 0;
        if(strstr(tmpString, ".") || strstr(tmpString, ",")) {
          for(char *xStr = tmpString; *xStr != 0; xStr++) {
            if(((displayFormat != DF_ENG && (displayFormat != DF_ALL || !getSystemFlag(FLAG_ALLENG))) && (*xStr == '.' || *xStr == ',')) ||
               ((displayFormat == DF_ENG || (displayFormat == DF_ALL && getSystemFlag(FLAG_ALLENG))) && xStr[0] == (char)0x80 && (xStr[1] == (char)0x87 || xStr[1] == (char)0xd7))) {
              rPadWidth[i * MATRIX_MAX_COLUMNS + j] = stringWidth(xStr, font, true, true) + 1;
                if(maxRightWidth[j] < rPadWidth[i * MATRIX_MAX_COLUMNS + j]) {
                  maxRightWidth[j] = rPadWidth[i * MATRIX_MAX_COLUMNS + j];
                }
              break;
            }
          }
            if(maxLeftWidth[j] < (width - rPadWidth[i * MATRIX_MAX_COLUMNS + j])) {
              maxLeftWidth[j] = (width - rPadWidth[i * MATRIX_MAX_COLUMNS + j]);
            }
        }
        else {
          rPadWidth[i * MATRIX_MAX_COLUMNS + j] = width | exponentOutOfRange;
        }
      }
    }
    for(int i = 0; i < maxRows; i++) {
      for(int j = 0; j < maxCols; j++) {
        if(rPadWidth[i * MATRIX_MAX_COLUMNS + j] & exponentOutOfRange) {
          if((maxLeftWidth[j] + maxRightWidth[j]) < (rPadWidth[i * MATRIX_MAX_COLUMNS + j] & (~exponentOutOfRange))) {
            maxLeftWidth[j] = (rPadWidth[i * MATRIX_MAX_COLUMNS + j] & (~exponentOutOfRange)) - maxRightWidth[j];
          }
        }
      }
    }
    for(int i = 0; i < maxRows; i++) {
      for(int j = 0; j < maxCols; j++) {
        if(rPadWidth[i * MATRIX_MAX_COLUMNS + j] & exponentOutOfRange) {
          rPadWidth[i * MATRIX_MAX_COLUMNS + j] = 0;
        }
        else {
          rPadWidth[i * MATRIX_MAX_COLUMNS + j] -= maxRightWidth[j];
          rPadWidth[i * MATRIX_MAX_COLUMNS + j] *= -1;
        }
      }
    }
    for(int j = 0; j < maxCols; j++) {
      colWidth[j] = (maxLeftWidth[j] + maxRightWidth[j]);
      totalWidth += colWidth[j] + stringWidth(STD_SPACE_FIGURE, font, true, true) * 2;
    }
    totalWidth -= stringWidth(STD_SPACE_FIGURE, font, true, true);
    if(noFix) {
      displayFormat = DF_ALL;
      displayFormatDigits = dspDigits;
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
  return totalWidth * (noFix ? -1 : 1);
}


void showComplexMatrix(const complex34Matrix_t *matrix, int16_t prefixWidth) {
  int rows = matrix->header.matrixRows;
  int cols = matrix->header.matrixColumns;
  int16_t Y_POS = Y_POSITION_OF_REGISTER_X_LINE;
  int16_t X_POS = 0;
  int16_t totalWidth = 0, width = 0;
  const font_t *font;
  int16_t fontHeight = NUMERIC_FONT_HEIGHT_;
  int16_t maxWidth = MATRIX_LINE_WIDTH - prefixWidth;
  int16_t colWidth[MATRIX_MAX_COLUMNS] = {}, colWidth_r[MATRIX_MAX_COLUMNS] = {}, colWidth_i[MATRIX_MAX_COLUMNS] = {};
  int16_t rPadWidth_r[MATRIX_MAX_ROWS * MATRIX_MAX_COLUMNS] = {}, rPadWidth_i[MATRIX_MAX_ROWS * MATRIX_MAX_COLUMNS] = {};
  const bool_t forEditor = matrix == &openMatrixMIMPointer.complexMatrix;
  const uint16_t sRow = forEditor ? scrollRow : 0;
  uint16_t sCol = forEditor ? scrollColumn : 0;
  const uint16_t tmpDisplayFormat = displayFormat;
  const int16_t tmpExponentLimit = exponentLimit;
  const uint8_t tmpDisplayFormatDigits = displayFormatDigits;
  const bool_t tmpMultX = getSystemFlag(FLAG_MULTx);

  Y_POS = Y_POSITION_OF_REGISTER_X_LINE - NUMERIC_FONT_HEIGHT_;

  bool_t colVector = false;
  if(cols == 1 && rows > 1) {
    colVector = true;
    cols = rows;
    rows = 1;
  }

  int maxCols = cols > MATRIX_MAX_ROWS ? MATRIX_MAX_ROWS : cols;
  const int maxRows = rows > MATRIX_MAX_COLUMNS ? MATRIX_MAX_COLUMNS : rows;

  int16_t matSelRow = colVector ? getJRegisterAsInt(true) : getIRegisterAsInt(true);
  int16_t matSelCol = colVector ? getIRegisterAsInt(true) : getJRegisterAsInt(true);

  videoMode_t vm = vmNormal;
    if(maxCols + sCol >= cols) {
      maxCols = cols - sCol;
    }

  font = &numericFont;
  if(rows >= (forEditor ? 4 : 5)) {
smallFont:
    font = &standardFont;
    fontHeight = STANDARD_FONT_HEIGHT_;
    Y_POS = Y_POSITION_OF_REGISTER_X_LINE - STANDARD_FONT_HEIGHT_ + 2;
    //maxWidth = MATRIX_LINE_WIDTH_SMALL * 4 - 20;
  }

    if(!forEditor) {
      Y_POS += REGISTER_LINE_HEIGHT;
    }
  bool_t rightEllipsis = (cols > maxCols) && (cols > maxCols + sCol);
  bool_t leftEllipsis = (sCol > 0);
  int16_t digits;

    if(prefixWidth > 0) {
      Y_POS = Y_POSITION_OF_REGISTER_T_LINE - REGISTER_LINE_HEIGHT + 1 + maxRows * fontHeight;
    }
    if(prefixWidth > 0 && font == &standardFont) {
      Y_POS += (maxRows == 1 ? STANDARD_FONT_HEIGHT_ : REGISTER_LINE_HEIGHT - STANDARD_FONT_HEIGHT_);
    }

    int16_t baseWidth = (leftEllipsis ? stringWidth(STD_ELLIPSIS " ", font, true, true) : 0) + (rightEllipsis ? stringWidth(STD_ELLIPSIS, font, true, true) : 0);
  totalWidth = baseWidth + getComplexMatrixColumnWidths(matrix, prefixWidth, font, colWidth, colWidth_r, colWidth_i, rPadWidth_r, rPadWidth_i, &digits, maxCols);
  if(totalWidth > maxWidth || leftEllipsis) {
    if(font == &numericFont) {
      goto smallFont;
    }
    else if(exponentLimit > 99) {
      exponentLimit = 99;
      goto smallFont;
    }
    else {
      displayFormat = DF_SCI;
      displayFormatDigits = 2;
      clearSystemFlag(FLAG_MULTx);
      totalWidth = baseWidth + getComplexMatrixColumnWidths(matrix, prefixWidth, font, colWidth, colWidth_r, colWidth_i, rPadWidth_r, rPadWidth_i, &digits, maxCols);
      if(totalWidth > maxWidth) {
        maxCols--;
        goto smallFont;
      }
    }
  }
  if(forEditor) {
    if(matSelCol < sCol) {
      scrollColumn--;
      sCol--;
      goto smallFont;
    }
    else if(matSelCol >= sCol + maxCols) {
      scrollColumn++;
      sCol++;
      goto smallFont;
    }
  }
    for(int j = 0; j < maxCols; j++) {
      baseWidth += colWidth[j] + stringWidth(STD_SPACE_FIGURE, font, true, true);
    }
  baseWidth -= stringWidth(STD_SPACE_FIGURE, font, true, true);

    if(prefixWidth > 0) {
      X_POS = prefixWidth;
    }
    else if(!forEditor) {
      X_POS = SCREEN_WIDTH - ((colVector ? stringWidth("[]" STD_SUP_T, font, true, true) : stringWidth("[]", font, true, true)) + baseWidth) - (font == &standardFont ? 0 : 1);
    }

  if(forEditor) {
    clearRegisterLine(REGISTER_X, true, true);
    clearRegisterLine(REGISTER_Y, true, true);
      if(rows >= (font == &standardFont ? 3 : 2)) {
        clearRegisterLine(REGISTER_Z, true, true);
      }
      if(rows >= (font == &standardFont ? 4 : 3)) {
        clearRegisterLine(REGISTER_T, true, true);
      }
  }
  else if(prefixWidth > 0) {
    clearRegisterLine(REGISTER_T, true, true);
      if(rows >= 2) {
        clearRegisterLine(REGISTER_Z, true, true);
      }
      if(rows >= (font == &standardFont ? 4 : 3)) {
        clearRegisterLine(REGISTER_Y, true, true);
      }
      if(rows == 4 && font != &standardFont) {
        clearRegisterLine(REGISTER_X, true, true);
      }
  }

  for(int i = 0; i < maxRows; i++) {
    int16_t colX = stringWidth("[", font, true, true);
    showString((maxRows == 1) ? "[" : (i == 0) ? STD_MAT_TL : (i + 1 == maxRows) ? STD_MAT_BL : STD_MAT_ML, font, X_POS + 1, Y_POS - (maxRows -1 - i) * fontHeight, vmNormal, true, false);
    if(leftEllipsis) {
      showString(STD_ELLIPSIS " ", font, X_POS + stringWidth("[", font, true, true), Y_POS - (maxRows -1 -i) * fontHeight, vmNormal, true, false);
      colX += stringWidth(STD_ELLIPSIS " ", font, true, true);
    }
    for(int j = 0; j < maxCols + (rightEllipsis ? 1 : 0); j++) {
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
        tmpString[0] = 0;
        constantFractionsMode = CF_COMPLEX1;  //JM
        real34ToDisplayString(&re, amNone, tmpString, font, colWidth_r[j], displayFormat == DF_ALL ? digits : 15, true, true);
        if(forEditor && matSelRow == (i + sRow) && matSelCol == (j + sCol)) {
          lcd_fill_rect(X_POS + colX, Y_POS - (maxRows -1 -i) * fontHeight, colWidth[j], font == &numericFont ? 32 : 20, 0xFF);
          vm = vmReverse;
        }
        else {
          vm = vmNormal;
        }
      }
      width = stringWidth(tmpString, font, true, true) + 1;
      showString(tmpString, font, X_POS + colX + (((j == maxCols) && rightEllipsis) ? stringWidth(STD_SPACE_FIGURE, font, true, true) - width : (colWidth_r[j] - width) - rPadWidth_r[i * MATRIX_MAX_COLUMNS + j]), Y_POS - (maxRows -1 -i) * fontHeight, vm, true, false);
      if(strcmp(tmpString, STD_ELLIPSIS) != 0) {
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
          if(neg) {
            tmpString[0] = '-';
            real34SetPositiveSign(&im);
          }
        }
        showString(tmpString, font, X_POS + colX + colWidth_r[j] + (width - stringWidth(tmpString, font, true, true)), Y_POS - (maxRows -1 -i) * fontHeight, vm, true, false);

        constantFractionsMode = CF_COMPLEX2; //JM
        real34ToDisplayString(&im, getSystemFlag(FLAG_POLAR) ? currentAngularMode : amNone, tmpString, font, colWidth_i[j], displayFormat == DF_ALL ? digits : 15, true, false);
        width = stringWidth(tmpString, font, true, true) + 1;
        showString(tmpString, font, X_POS + colX + colWidth_r[j] + cpxUnitWidth + (((j == maxCols - 1) && rightEllipsis) ? 0 : (colWidth_i[j] - width) - rPadWidth_i[i * MATRIX_MAX_COLUMNS + j]), Y_POS - (maxRows -1 -i) * fontHeight, vm, true, false);
      }
      colX += colWidth[j] + stringWidth(STD_SPACE_FIGURE, font, true, true);
    }
    showString((maxRows == 1) ? "]" : (i == 0) ? STD_MAT_TR : (i + 1 == maxRows) ? STD_MAT_BR : STD_MAT_MR, font, X_POS + stringWidth("[", font, true, true) + baseWidth, Y_POS - (maxRows -1 -i) * fontHeight, vmNormal, true, false);
    if(colVector == true) {
      showString(STD_SUP_T, font, X_POS + stringWidth("[]", font, true, true) + baseWidth, Y_POS - (maxRows -1 -i) * fontHeight, vmNormal, true, false);
    }
  }

  displayFormat = tmpDisplayFormat;
  displayFormatDigits = tmpDisplayFormatDigits;
  exponentLimit = tmpExponentLimit;
    if(tmpMultX) {
      setSystemFlag(FLAG_MULTx);
    }
}

int16_t getComplexMatrixColumnWidths(const complex34Matrix_t *matrix, int16_t prefixWidth, const font_t *font, int16_t *colWidth, int16_t *colWidth_r, int16_t *colWidth_i, int16_t *rPadWidth_r, int16_t *rPadWidth_i, int16_t *digits, uint16_t maxCols) {
  const bool_t colVector = matrix->header.matrixColumns == 1 && matrix->header.matrixRows > 1;
  const int rows = colVector ? 1 : matrix->header.matrixRows;
  const int cols = colVector ? matrix->header.matrixRows : matrix->header.matrixColumns;
  const int maxRows = rows > MATRIX_MAX_ROWS ? MATRIX_MAX_ROWS : rows;
  const bool_t forEditor = matrix == &openMatrixMIMPointer.complexMatrix;
  const uint16_t sRow = forEditor ? scrollRow : 0;
  const uint16_t sCol = forEditor ? scrollColumn : 0;
  const int16_t maxWidth = MATRIX_LINE_WIDTH - prefixWidth;
  int16_t totalWidth = 0, width = 0;
  int16_t maxRightWidth_r[MATRIX_MAX_COLUMNS] = {};
  int16_t maxLeftWidth_r[MATRIX_MAX_COLUMNS] = {};
  int16_t maxRightWidth_i[MATRIX_MAX_COLUMNS] = {};
  int16_t maxLeftWidth_i[MATRIX_MAX_COLUMNS] = {};
  const int16_t exponentOutOfRange = 0x4000;

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
      if(displayFormat == DF_ALL) {
        *digits = k;
      }
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

        rPadWidth_r[i * MATRIX_MAX_COLUMNS + j] = 0;
        real34SetPositiveSign(VARIABLE_REAL34_DATA(&c34Val));
        constantFractionsMode = CF_COMPLEX1; //JM
        real34ToDisplayString(VARIABLE_REAL34_DATA(&c34Val), amNone, tmpString, font, maxWidth, displayFormat == DF_ALL ? k : 15, true, true);
        width = stringWidth(tmpString, font, true, true) + 1;
        if(strstr(tmpString, ".") || strstr(tmpString, ",")) {
          for(char *xStr = tmpString; *xStr != 0; xStr++) {
            if(((displayFormat != DF_ENG && (displayFormat != DF_ALL || !getSystemFlag(FLAG_ALLENG))) && (*xStr == '.' || *xStr == ',')) ||
               ((displayFormat == DF_ENG || (displayFormat == DF_ALL && getSystemFlag(FLAG_ALLENG))) && xStr[0] == (char)0x80 && (xStr[1] == (char)0x87 || xStr[1] == (char)0xd7))) {
              rPadWidth_r[i * MATRIX_MAX_COLUMNS + j] = stringWidth(xStr, font, true, true) + 1;
                if(maxRightWidth_r[j] < rPadWidth_r[i * MATRIX_MAX_COLUMNS + j]) {
                  maxRightWidth_r[j] = rPadWidth_r[i * MATRIX_MAX_COLUMNS + j];
                }
              break;
            }
          }
            if(maxLeftWidth_r[j] < (width - rPadWidth_r[i * MATRIX_MAX_COLUMNS + j])) {
              maxLeftWidth_r[j] = (width - rPadWidth_r[i * MATRIX_MAX_COLUMNS + j]);
            }
        }
        else {
          rPadWidth_r[i * MATRIX_MAX_COLUMNS + j] = width | exponentOutOfRange;
        }

        rPadWidth_i[i * MATRIX_MAX_COLUMNS + j] = 0;
        if(!getSystemFlag(FLAG_POLAR)) {
          real34SetPositiveSign(VARIABLE_IMAG34_DATA(&c34Val));
        }
        constantFractionsMode = CF_COMPLEX2; //JM
        real34ToDisplayString(VARIABLE_IMAG34_DATA(&c34Val), getSystemFlag(FLAG_POLAR) ? currentAngularMode : amNone, tmpString, font, maxWidth, displayFormat == DF_ALL ? k : 15, true, false);
        width = stringWidth(tmpString, font, true, true) + 1;
        if(strstr(tmpString, ".") || strstr(tmpString, ",")) {
          for(char *xStr = tmpString; *xStr != 0; xStr++) {
            if(((displayFormat != DF_ENG && (displayFormat != DF_ALL || !getSystemFlag(FLAG_ALLENG))) && (*xStr == '.' || *xStr == ',')) ||
               ((displayFormat == DF_ENG || (displayFormat == DF_ALL && getSystemFlag(FLAG_ALLENG))) && xStr[0] == (char)0x80 && (xStr[1] == (char)0x87 || xStr[1] == (char)0xd7))) {
              rPadWidth_i[i * MATRIX_MAX_COLUMNS + j] = stringWidth(xStr, font, true, true) + 1;
                if(maxRightWidth_i[j] < rPadWidth_i[i * MATRIX_MAX_COLUMNS + j]) {
                  maxRightWidth_i[j] = rPadWidth_i[i * MATRIX_MAX_COLUMNS + j];
                }
              break;
            }
          }
            if(maxLeftWidth_i[j] < (width - rPadWidth_i[i * MATRIX_MAX_COLUMNS + j])) {
              maxLeftWidth_i[j] = (width - rPadWidth_i[i * MATRIX_MAX_COLUMNS + j]);
            }
        }
        else {
          rPadWidth_i[i * MATRIX_MAX_COLUMNS + j] = width | exponentOutOfRange;
        }
      }
    }
    for(int i = 0; i < maxRows; i++) {
      for(int j = 0; j < maxCols; j++) {
        if(rPadWidth_r[i * MATRIX_MAX_COLUMNS + j] & exponentOutOfRange) {
          if((maxLeftWidth_r[j] + maxRightWidth_r[j]) < (rPadWidth_r[i * MATRIX_MAX_COLUMNS + j] & (~exponentOutOfRange))) {
            maxLeftWidth_r[j] = (rPadWidth_r[i * MATRIX_MAX_COLUMNS + j] & (~exponentOutOfRange)) - maxRightWidth_r[j];
          }
        }
        if(rPadWidth_i[i * MATRIX_MAX_COLUMNS + j] & exponentOutOfRange) {
          if((maxLeftWidth_i[j] + maxRightWidth_i[j]) < (rPadWidth_i[i * MATRIX_MAX_COLUMNS + j] & (~exponentOutOfRange))) {
            maxLeftWidth_i[j] = (rPadWidth_i[i * MATRIX_MAX_COLUMNS + j] & (~exponentOutOfRange)) - maxRightWidth_i[j];
          }
        }
      }
    }
    for(int i = 0; i < maxRows; i++) {
      for(int j = 0; j < maxCols; j++) {
        if(rPadWidth_r[i * MATRIX_MAX_COLUMNS + j] & exponentOutOfRange) {
          rPadWidth_r[i * MATRIX_MAX_COLUMNS + j] = 0;
        }
        else {
          rPadWidth_r[i * MATRIX_MAX_COLUMNS + j] -= maxRightWidth_r[j];
          rPadWidth_r[i * MATRIX_MAX_COLUMNS + j] *= -1;
        }
        if(rPadWidth_i[i * MATRIX_MAX_COLUMNS + j] & exponentOutOfRange) {
          rPadWidth_i[i * MATRIX_MAX_COLUMNS + j] = 0;
        }
        else {
          rPadWidth_i[i * MATRIX_MAX_COLUMNS + j] -= maxRightWidth_i[j];
          rPadWidth_i[i * MATRIX_MAX_COLUMNS + j] *= -1;
        }
      }
    }
    for(int j = 0; j < maxCols; j++) {
      colWidth_r[j] = maxLeftWidth_r[j] + maxRightWidth_r[j];
      colWidth_i[j] = maxLeftWidth_i[j] + maxRightWidth_i[j];
      colWidth[j] = colWidth_r[j] + (colWidth_i[j] > 0 ? (cpxUnitWidth + colWidth_i[j]) : 0);
      totalWidth += colWidth[j] + stringWidth(STD_SPACE_FIGURE, font, true, true) * 2;
    }
    totalWidth -= stringWidth(STD_SPACE_FIGURE, font, true, true);
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
#endif // !defined(TESTSUITE_BUILD)
