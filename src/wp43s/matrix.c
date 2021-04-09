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
 * \file matrix.c
 ***********************************************/

#include "wp43s.h"

#ifndef TESTSUITE_BUILD
real34Matrix_t       *openMatrixMIMPointer;
bool_t                matEditMode;

/********************************************//**
 * \brief creates new Matrix of size y->m x x ->n
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnNewMatrix(uint16_t unusedParamButMandatory) {
  uint32_t rows, cols;
  longInteger_t tmp_lgInt;
  real34Matrix_t* matrix;

  //Get Size from REGISTER_X and REGISTER_Y
  if(((getRegisterDataType(REGISTER_X) != dtLongInteger) && (getRegisterDataType(REGISTER_X) != dtReal34)) ||
    ((getRegisterDataType(REGISTER_Y) != dtLongInteger) && (getRegisterDataType(REGISTER_Y) != dtReal34))) {
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "invalid data type %s and %s", getRegisterDataTypeName(REGISTER_Y, true, false), getRegisterDataTypeName(REGISTER_X, true, false));
        moreInfoOnError("In function fnNewMatrix:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
  }

  if(getRegisterDataType(REGISTER_X) == dtLongInteger)
    convertLongIntegerRegisterToLongInteger(REGISTER_X, tmp_lgInt);
  else // dtReal34
    convertReal34ToLongInteger(REGISTER_REAL34_DATA(REGISTER_X), tmp_lgInt, DEC_ROUND_DOWN);
  longIntegerToUInt(tmp_lgInt, cols);

  if(getRegisterDataType(REGISTER_X) == dtLongInteger)
    convertLongIntegerRegisterToLongInteger(REGISTER_Y, tmp_lgInt);
  else // dtReal34
    convertReal34ToLongInteger(REGISTER_REAL34_DATA(REGISTER_Y), tmp_lgInt, DEC_ROUND_DOWN);
  longIntegerToUInt(tmp_lgInt, rows);

  longIntegerFree(tmp_lgInt);

  //Initialize Memory for Matrix
  matrix = realMatrixInit(rows, cols);

  //Drop X_Register and Y_Register
  fnDropY(NOPARAM);
  convertReal34MatrixToReal34MatrixRegister(matrix, REGISTER_X);
  setSystemFlag(FLAG_ASLIFT);

  realMatrixFree(matrix);
}

/********************************************//**
 * \brief Opens the Matrix Editor
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnEditMatrix(uint16_t unusedParamButMandatory) {
  if((getRegisterDataType(REGISTER_X) == dtReal34Matrix) || (getRegisterDataType(REGISTER_X) == dtComplex34Matrix)) {
    calcMode = CM_MIM;

    getMatrixFromRegister(REGISTER_X);

    showSoftmenu(-MNU_M_EDIT);
    setIRegisterAsInt(true, 0);
    setJRegisterAsInt(true, 0);
    aimBuffer[0] = 0;
    nimBufferDisplay[0] = 0;
    showMatrixEditor();
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(REGISTER_X));
    moreInfoOnError("In function fnEditMatrix:", errorMessage, "is not a matrix.", "");
    #endif
  }
}


/********************************************//**
 * \brief Initialize a real matrix
 *
 * \param[in] rows uint16_t
 * \param[in] cols uint16_t
 * \return real34Matrix_t *
 ***********************************************/
real34Matrix_t *realMatrixInit(uint16_t rows, uint16_t cols) {
  real34Matrix_t *matrix;

  //Allocate Memory for Matrix
  matrix = allocWp43s(TO_BLOCKS((rows * cols) * sizeof(real34_t) + sizeof(registerHeader_t)));

  matrix->header.matrixColumns = cols;
  matrix->header.matrixRows = rows;

  //Initialize with 0.
  for(uint32_t i = 0; i < rows * cols; i++) {
    real34Copy(const34_0, &matrix->matrixElements[i]);
  }

  return matrix;
}


/********************************************//**
 * \brief Free a real matrix
 *
 * \param[in] matrix real34Matrix_t *
 * \return void
 ***********************************************/
void realMatrixFree(real34Matrix_t *matrix) {
  uint16_t cols = matrix->header.matrixColumns;
  uint16_t rows = matrix->header.matrixRows;

  freeWp43s(matrix, TO_BLOCKS((rows * cols) * sizeof(real34_t) + sizeof(registerHeader_t)));
}


/********************************************//**
 * \brief Displays a Matrix
 *
 * \return void
 ***********************************************/
void showMatrixEditor() {

  int rows = openMatrixMIMPointer->header.matrixRows;
  int cols = openMatrixMIMPointer->header.matrixColumns;
  int16_t Y_POS = Y_POSITION_OF_REGISTER_X_LINE;

  Y_POS = Y_POSITION_OF_REGISTER_X_LINE - NUMERIC_FONT_HEIGHT;

  bool_t colVector = false;
  if (cols == 1) {
    colVector = true;
    cols = rows;
    rows = 1;
  }

  //tbd: Implement right behavior (add row, add col on change)
  if (!getSystemFlag(FLAG_GROW)) {
    if (getIRegisterAsInt(true) < 0) {
      setIRegisterAsInt(true, rows - 1);
      setJRegisterAsInt(true, (getJRegisterAsInt(true) == 0) ? cols - 1 : getJRegisterAsInt(true) - 1);
    } else if (getIRegisterAsInt(true) == rows) {
      setIRegisterAsInt(true, 0);
      setJRegisterAsInt(true, (getJRegisterAsInt(true) == cols - 1) ? 0 : getJRegisterAsInt(true) + 1);
    }

    if (getJRegisterAsInt(true) < 0) {
      setJRegisterAsInt(true, cols - 1);
      setIRegisterAsInt(true, (getIRegisterAsInt(true) == 0) ? rows - 1 : getIRegisterAsInt(true) - 1);
    } else if (getJRegisterAsInt(true) == cols) {
      setJRegisterAsInt(true, 0);
      setIRegisterAsInt(true, (getIRegisterAsInt(true) == rows - 1) ? 0 : getIRegisterAsInt(true) + 1);
    }
  }
  else {
    //GROW tbd
  }

  int16_t matSelRow = getIRegisterAsInt(true);
  int16_t matSelCol = getJRegisterAsInt(true);

  videoMode_t vm = vmNormal;
  for(int i = 0; i < rows; i++) {
    showString("[", &numericFont, 1, Y_POS - (rows -1 - i) * NUMERIC_FONT_HEIGHT, vmNormal, true, false);
    for(int j = 0; j< cols; j++) {
      real34ToDisplayString(&openMatrixMIMPointer->matrixElements[i*cols+j], amNone, tmpString, &numericFont, 5, 10, true, STD_SPACE_4_PER_EM);
      if (matSelRow == i && matSelCol == j) {
        vm = vmReverse;
      } else {
        vm = vmNormal;
      }
      showString(tmpString, &numericFont, 5 + j * MATRIX_LINE_WIDTH_LARGE, Y_POS - (rows -1 -i) * NUMERIC_FONT_HEIGHT, vm, true, false);
    }
    showString("]", &numericFont, 10 + cols * MATRIX_LINE_WIDTH_LARGE, Y_POS - (rows -1 -i) * NUMERIC_FONT_HEIGHT, vmNormal, true, false);
    if (colVector == true) {
        showString("T", &numericFont, 20 + cols * MATRIX_LINE_WIDTH_LARGE, Y_POS - (rows -1 -i) * NUMERIC_FONT_HEIGHT, vmNormal, true, false);
    }
  }

  sprintf(tmpString, "%" PRIi16";%" PRIi16"= %s", matSelRow+1, matSelCol+1, nimBufferDisplay);
  if(aimBuffer[0] == 0)
    real34ToDisplayString(&openMatrixMIMPointer->matrixElements[matSelRow*cols+matSelCol], amNone, &tmpString[strlen(tmpString)], &numericFont, 5, 10, true, STD_SPACE_4_PER_EM);
  xCursor = showString(tmpString, &numericFont, 1, Y_POS + NUMERIC_FONT_HEIGHT, vmNormal, true, false) + 3;
  yCursor = Y_POSITION_OF_NIM_LINE;
}

void mimEnter(void) {
  int cols = openMatrixMIMPointer->header.matrixColumns;
  int16_t row = getIRegisterAsInt(true);
  int16_t col = getJRegisterAsInt(true);

  if(aimBuffer[0] != 0) {
    stringToReal34(aimBuffer, &openMatrixMIMPointer->matrixElements[row * cols + col]);

    aimBuffer[0] = 0;
    nimBufferDisplay[0] = 0;
    hideCursor();
    cursorEnabled = false;

    convertReal34MatrixToReal34MatrixRegister(openMatrixMIMPointer, REGISTER_X);
    setSystemFlag(FLAG_ASLIFT);
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
      else if(nimNumberPart == NP_REAL_FLOAT_PART) {
        return; // fractional element is unsupported
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
        int cols = openMatrixMIMPointer->header.matrixColumns;
        int16_t row = getIRegisterAsInt(true);
        int16_t col = getJRegisterAsInt(true);

        real34ChangeSign(&openMatrixMIMPointer->matrixElements[row * cols + col]);

        convertReal34MatrixToReal34MatrixRegister(openMatrixMIMPointer, REGISTER_X);
        setSystemFlag(FLAG_ASLIFT);
        return;
      }
      break;

    default:
      return;
  }
  addItemToNimBuffer(item);
  calcMode = CM_MIM;
}

void mimFinalize(void) {
  if(openMatrixMIMPointer) {
    realMatrixFree(openMatrixMIMPointer);
    openMatrixMIMPointer = NULL;
  }
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
  real34Matrix_t* matrix;

  if (getRegisterDataType(regist) != dtReal34Matrix) {
    #ifdef PC_BUILD
    sprintf(errorMessage, "DataType %" PRIu32, getRegisterDataType(regist));
    moreInfoOnError("In function getMatrixFromRegister:", errorMessage, "is not dataType dtRealMatrix.", "");
    #endif
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
    return;
  }

  if(openMatrixMIMPointer) realMatrixFree(openMatrixMIMPointer);
  convertReal34MatrixRegisterToReal34Matrix(regist, &matrix);


  //NOT WORKING//
  openMatrixMIMPointer = matrix;
  //END NOT WORKING//
}

//Row of Matric
int16_t getIRegisterAsInt(bool_t asArrayPointer) {
  int16_t ret;
  longInteger_t tmp_lgInt;

  convertLongIntegerRegisterToLongInteger(REGISTER_I, tmp_lgInt);
  longIntegerToInt(tmp_lgInt, ret);

  longIntegerFree(tmp_lgInt);

  if (asArrayPointer) ret--;

  return ret;
}

//Col of Matrix
int16_t getJRegisterAsInt(bool_t asArrayPointer) {
  int16_t ret;
  longInteger_t tmp_lgInt;

  convertLongIntegerRegisterToLongInteger(REGISTER_J, tmp_lgInt);
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



/* Addition and subtraction */
static void addSubRealMatrices(const real34Matrix_t *y, const real34Matrix_t *x, bool_t subtraction, real34Matrix_t **res) {
  uint16_t rows = y->header.matrixRows;
  uint16_t cols = y->header.matrixColumns;
  int32_t i;

  if((y->header.matrixColumns != x->header.matrixColumns) || (y->header.matrixRows != x->header.matrixRows)) {
    *res = NULL; // Matrix mismatch
    return;
  }

  *res = realMatrixInit(rows, cols);
  for(i = 0; i < cols * rows; ++i) {
    if(subtraction) {
      real34Subtract(&y->matrixElements[i], &x->matrixElements[i], &(*res)->matrixElements[i]);
    }
    else {
      real34Add(&y->matrixElements[i], &x->matrixElements[i], &(*res)->matrixElements[i]);
    }
  }
}

void addRealMatrices(const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t **res) {
  addSubRealMatrices(y, x, false, res);
}

void subtractRealMatrices(const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t **res) {
  addSubRealMatrices(y, x, true, res);
}

#endif
