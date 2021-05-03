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
 * \file matrix.h
 ***********************************************/

#ifndef MATRIX_H
#define MATRIX_H

#include "typeDefinitions.h"

#ifndef TESTSUITE_BUILD
  #define MATRIX_LINE_WIDTH_LARGE      120
  #define MATRIX_LINE_WIDTH_SMALL      93
  //#define MATRIX_CHAR_LEN              30
#endif // TESTSUITE_BUILD

void       fnNewMatrix                    (uint16_t unusedParamButMandatory);
void       fnEditMatrix                   (uint16_t regist);
void       fnOldMatrix                    (uint16_t unusedParamButMandatory);
void       fnGoToElement                  (uint16_t unusedParamButMandatory);
void       fnGoToRow                      (uint16_t row);
void       fnGoToColumn                   (uint16_t col);
void       fnSetGrowMode                  (uint16_t growFlag);
void       fnIncDecI                      (uint16_t mode);
void       fnIncDecJ                      (uint16_t mode);
void       fnInsRow                       (uint16_t unusedParamButMandatory);
void       fnDelRow                       (uint16_t unusedParamButMandatory);

void       fnSetMatrixDimensions          (uint16_t regist);
void       fnGetMatrixDimensions          (uint16_t unusedParamButMandatory);
void       fnTranspose                    (uint16_t unusedParamButMandatory);
void       fnLuDecomposition              (uint16_t unusedParamButMandatory);
void       fnDeterminant                  (uint16_t unusedParamButMandatory);
void       fnInvertMatrix                 (uint16_t unusedParamButMandatory);
void       fnEuclideanNorm                (uint16_t unusedParamButMandatory);
void       fnRowSum                       (uint16_t unusedParamButMandatory);
void       fnRowNorm                      (uint16_t unusedParamButMandatory);

void       fnIndexMatrix                  (uint16_t regist);
void       fnGetMatrix                    (uint16_t unusedParamButMandatory);
void       fnPutMatrix                    (uint16_t unusedParamButMandatory);
void       fnSwapRows                     (uint16_t unusedParamButMandatory);

void       fnSimultaneousLinearEquation   (uint16_t numberOfUnknowns);
void       fnEditLinearEquationMatrixA    (uint16_t unusedParamButMandatory);
void       fnEditLinearEquationMatrixB    (uint16_t unusedParamButMandatory);
void       fnEditLinearEquationMatrixX    (uint16_t unusedParamButMandatory);

#ifndef TESTSUITE_BUILD
  void     realMatrixInit                 (real34Matrix_t *matrix, uint16_t rows, uint16_t cols);
  void     realMatrixFree                 (real34Matrix_t *matrix);
  void     realMatrixIdentity             (real34Matrix_t *matrix, uint16_t size);
  void     realMatrixRedim                (real34Matrix_t *matrix, uint16_t rows, uint16_t cols);

  void     showMatrixEditor               (void);
  void     mimEnter                       (bool_t commit);
  void     mimAddNumber                   (int16_t item);
  void     mimFinalize                    (void);
  void     mimRestore                     (void);
  void     showRealMatrix                 (const real34Matrix_t *matrix);
  int16_t  getRealMatrixColumnWidths      (const real34Matrix_t *matrix, const font_t *font, int16_t *colWidth, int16_t *rPadWidth, int16_t *digits);

  void     getMatrixFromRegister          (calcRegister_t regist);

  int16_t  getIRegisterAsInt              (bool_t asArrayPointer);
  int16_t  getJRegisterAsInt              (bool_t asArrayPointer);
  void     setIRegisterAsInt              (bool_t asArrayPointer, int16_t toStore);
  void     setJRegisterAsInt              (bool_t asArrayPointer, int16_t toStore);

  bool_t   wrapIJ                         (uint16_t rows, uint16_t cols);

  void     copyRealMatrix                 (const real34Matrix_t *matrix, real34Matrix_t *res);
  void     linkToRealMatrixRegister       (calcRegister_t regist, real34Matrix_t *linkedMatrix);
  void     insRowRealMatrix               (real34Matrix_t *matrix, uint16_t beforeRowNo);
  void     delRowRealMatrix               (real34Matrix_t *matrix, uint16_t beforeRowNo);
  void     transposeRealMatrix            (const real34Matrix_t *matrix, real34Matrix_t *res);

  void     addRealMatrices                (const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res);
  void     subtractRealMatrices           (const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res);

  void     multiplyRealMatrix             (const real34Matrix_t *matrix, const real34_t *x, real34Matrix_t *res);
  void     multiplyRealMatrices           (const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res);

  uint16_t realVectorSize                 (const real34Matrix_t *matrix);
  void     dotRealVectors                 (const real34Matrix_t *y, const real34Matrix_t *x, real34_t *res);
  void     crossRealVectors               (const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res);

  void     WP34S_LU_decomposition         (const real34Matrix_t *matrix, real34Matrix_t *lu, uint16_t *p);
  void     realMatrixSwapRows             (const real34Matrix_t *matrix, real34Matrix_t *res, uint16_t a, uint16_t b);
  void     detRealMatrix                  (const real34Matrix_t *matrix, real34_t *res);
  void     WP34S_matrix_inverse           (const real34Matrix_t *matrix, real34Matrix_t *res);
  void     divideRealMatrix               (const real34Matrix_t *matrix, const real34_t *x, real34Matrix_t *res);
  void     divideRealMatrices             (const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res);

  void     WP34S_matrix_linear_eqn        (const real34Matrix_t *a, const real34Matrix_t *b, real34Matrix_t *r);
#endif // TESTSUITE_BUILD

void       elementwiseRema                (void (*f)(void));
void       elementwiseRema_UInt16         (void (*f)(uint16_t), uint16_t param);
void       elementwiseRemaLonI            (void (*f)(void));
void       elementwiseRemaReal            (void (*f)(void));
void       elementwiseRemaShoI            (void (*f)(void));

#ifndef TESTSUITE_BUILD
  void     callByIndexedMatrix            (bool_t (*real_f)(real34Matrix_t *), void *reserved);
#endif // TESTSUITE_BUILD

#endif // MATRIX_H
