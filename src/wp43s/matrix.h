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

/**
 * \file matrix.h
 */

#ifndef MATRIX_H
#define MATRIX_H

#include "typeDefinitions.h"

#ifndef TESTSUITE_BUILD
  #define MATRIX_LINE_WIDTH_LARGE      120
  #define MATRIX_LINE_WIDTH_SMALL      93
  //#define MATRIX_CHAR_LEN              30
#endif // TESTSUITE_BUILD

/**
 * Creates new Matrix of size y->m x x ->n.
 *
 * \param[in] unusedParamButMandatory
 */
void       fnNewMatrix                    (uint16_t unusedParamButMandatory);
/**
 * Opens the Matrix Editor.
 *
 * \param[in] regist
 */
void       fnEditMatrix                   (uint16_t regist);
/**
 * Recalls old element in the Matrix Editor.
 *
 * \param[in] unusedParamButMandatory
 */
void       fnOldMatrix                    (uint16_t unusedParamButMandatory);
/**
 * Go to an element in the Matrix Editor.
 *
 * \param[in] unusedParamButMandatory
 */
void       fnGoToElement                  (uint16_t unusedParamButMandatory);
void       fnGoToRow                      (uint16_t row);
void       fnGoToColumn                   (uint16_t col);
/**
 * Set grow mode.
 *
 * \param[in] growFlag
 */
void       fnSetGrowMode                  (uint16_t growFlag);
/**
 * Increment or decrement of register I as row pointer.
 *
 * \param[in] mode
 */
void       fnIncDecI                      (uint16_t mode);
/**
 * Increment or decrement of register J as column pointer.
 *
 * \param[in] mode
 */
void       fnIncDecJ                      (uint16_t mode);
/**
 * Insert a row.
 *
 * \param[in] unusedParamButMandatory
 */
void       fnInsRow                       (uint16_t unusedParamButMandatory);
/**
 * Delete a row.
 *
 * \param[in] unusedParamButMandatory
 */
void       fnDelRow                       (uint16_t unusedParamButMandatory);

/**
 * (Re-)dimension matrix X.
 *
 * \param[in] regist
 */
void       fnSetMatrixDimensions          (uint16_t regist);
/**
 * Get dimensions of matrix X.
 *
 * \param[in] unusedParamButMandatory
 */
void       fnGetMatrixDimensions          (uint16_t unusedParamButMandatory);
/**
 * Transpose matrix.
 *
 * \param[in] unusedParamButMandatory
 */
void       fnTranspose                    (uint16_t unusedParamButMandatory);
/**
 * LU decomposition.
 *
 * \param[in] unusedParamButMandatory
 */
void       fnLuDecomposition              (uint16_t unusedParamButMandatory);
/**
 * Determinant.
 *
 * \param[in] unusedParamButMandatory
 */
void       fnDeterminant                  (uint16_t unusedParamButMandatory);
/**
 * Invert a square matrix.
 *
 * \param[in] unusedParamButMandatory
 */
void       fnInvertMatrix                 (uint16_t unusedParamButMandatory);
/**
 * Euclidean norm of matrix X.
 *
 * \param[in] unusedParamButMandatory
 */
void       fnEuclideanNorm                (uint16_t unusedParamButMandatory);
/**
 * Row sum of matrix X.
 *
 * \param[in] unusedParamButMandatory
 */
void       fnRowSum                       (uint16_t unusedParamButMandatory);
/**
 * Row norm of matrix X.
 *
 * \param[in] unusedParamButMandatory
 */
void       fnRowNorm                      (uint16_t unusedParamButMandatory);

/**
 * Index a named matrix.
 *
 * \param[in] regist
 */
void       fnIndexMatrix                  (uint16_t regist);
/**
 * Get submatrix of the indexed matrix.
 *
 * \param[in] unusedParamButMandatory
 */
void       fnGetMatrix                    (uint16_t unusedParamButMandatory);
/**
 * Put submatrix to the indexed matrix.
 *
 * \param[in] unusedParamButMandatory
 */
void       fnPutMatrix                    (uint16_t unusedParamButMandatory);
/**
 * Swap rows of the indexed matrix.
 *
 * \param[in] unusedParamButMandatory
 */
void       fnSwapRows                     (uint16_t unusedParamButMandatory);

/**
 * Initialize simultaneous linear equation solver.
 *
 * \param[in] numberOfUnknowns
 */
void       fnSimultaneousLinearEquation   (uint16_t numberOfUnknowns);
void       fnEditLinearEquationMatrixA    (uint16_t unusedParamButMandatory);
void       fnEditLinearEquationMatrixB    (uint16_t unusedParamButMandatory);
void       fnEditLinearEquationMatrixX    (uint16_t unusedParamButMandatory);

#ifndef TESTSUITE_BUILD
  /**
   * Initialize a real matrix.
   *
   * \param[out] matrix
   * \param[in] rows
   * \param[in] cols
   */
  void     realMatrixInit                 (real34Matrix_t *matrix, uint16_t rows, uint16_t cols);
  /**
   * Free a real matrix.
   *
   * \param[in] matrix
   */
  void     realMatrixFree                 (real34Matrix_t *matrix);
  /**
   * Returns identity matrix of given size.
   *
   * \param[in] matrix
   */
  void     realMatrixIdentity             (real34Matrix_t *matrix, uint16_t size);
  /**
   * Redemention a real matrix.
   *
   * \param[in] matrix
   */
  void     realMatrixRedim                (real34Matrix_t *matrix, uint16_t rows, uint16_t cols);

  /**
   * Initialize a complex matrix.
   *
   * \param[out] matrix
   * \param[in] rows
   * \param[in] cols
   */
  void     complexMatrixInit              (complex34Matrix_t *matrix, uint16_t rows, uint16_t cols);
  /**
   * Free a complex matrix.
   *
   * \param[in] matrix
   */
  void     complexMatrixFree              (complex34Matrix_t *matrix);
  /**
   * Returns identity matrix of given size in complex34Matrix_t.
   *
   * \param[in] matrix
   */
  void     complexMatrixIdentity          (complex34Matrix_t *matrix, uint16_t size);
  /**
   * Redemention a complex matrix.
   *
   * \param[in] matrix
   */
  void     complexMatrixRedim             (complex34Matrix_t *matrix, uint16_t rows, uint16_t cols);

  /**
   * Displays the matrix editor.
   */
  void     showMatrixEditor               (void);
  void     mimEnter                       (bool_t commit);
  void     mimAddNumber                   (int16_t item);
  void     mimFinalize                    (void);
  void     mimRestore                     (void);
  /**
   * Displays a real matrix.
   *
   * \param[in] matrix
   */
  void     showRealMatrix                 (const real34Matrix_t *matrix);
  int16_t  getRealMatrixColumnWidths      (const real34Matrix_t *matrix, const font_t *font, int16_t *colWidth, int16_t *rPadWidth, int16_t *digits);
  /**
   * Displays a complex matrix.
   *
   * \param[in] matrix
   */
  void     showComplexMatrix              (const complex34Matrix_t *matrix);
  int16_t  getComplexMatrixColumnWidths   (const complex34Matrix_t *matrix, const font_t *font, int16_t *colWidth, int16_t *colWidth_r, int16_t *colWidth_i, int16_t *rPadWidth_r, int16_t *rPadWidth_i, int16_t *digits);

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

  void     copyComplexMatrix              (const complex34Matrix_t *matrix, complex34Matrix_t *res);
  void     linkToComplexMatrixRegister    (calcRegister_t regist, complex34Matrix_t *linkedMatrix);
  void     insRowComplexMatrix            (complex34Matrix_t *matrix, uint16_t beforeRowNo);
  void     delRowComplexMatrix            (complex34Matrix_t *matrix, uint16_t beforeRowNo);
  void     transposeComplexMatrix         (const complex34Matrix_t *matrix, complex34Matrix_t *res);

  void     addRealMatrices                (const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res);
  void     subtractRealMatrices           (const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res);

  void     addComplexMatrices             (const complex34Matrix_t *y, const complex34Matrix_t *x, complex34Matrix_t *res);
  void     subtractComplexMatrices        (const complex34Matrix_t *y, const complex34Matrix_t *x, complex34Matrix_t *res);

  void     multiplyRealMatrix             (const real34Matrix_t *matrix, const real34_t *x, real34Matrix_t *res);
  void     _multiplyRealMatrix            (const real34Matrix_t *matrix, const real_t *x, real34Matrix_t *res, realContext_t *realContext);
  void     multiplyRealMatrices           (const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res);

  void     multiplyComplexMatrix          (const complex34Matrix_t *matrix, const real34_t *xr, const real34_t *xi, complex34Matrix_t *res);
  void     _multiplyComplexMatrix         (const complex34Matrix_t *matrix, const real_t *xr, const real_t *xi, complex34Matrix_t *res, realContext_t *realContext);
  void     multiplyComplexMatrices        (const complex34Matrix_t *y, const complex34Matrix_t *x, complex34Matrix_t *res);

  uint16_t realVectorSize                 (const real34Matrix_t *matrix);
  void     dotRealVectors                 (const real34Matrix_t *y, const real34Matrix_t *x, real34_t *res);
  void     crossRealVectors               (const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res);

  uint16_t complexVectorSize              (const complex34Matrix_t *matrix);
  void     dotComplexVectors              (const complex34Matrix_t *y, const complex34Matrix_t *x, real34_t *res_r, real34_t *res_i);
  void     crossComplexVectors            (const complex34Matrix_t *y, const complex34Matrix_t *x, complex34Matrix_t *res);

  void     WP34S_LU_decomposition         (const real34Matrix_t *matrix, real34Matrix_t *lu, uint16_t *p);
  void     realMatrixSwapRows             (const real34Matrix_t *matrix, real34Matrix_t *res, uint16_t a, uint16_t b);
  void     detRealMatrix                  (const real34Matrix_t *matrix, real34_t *res);
  void     WP34S_matrix_inverse           (const real34Matrix_t *matrix, real34Matrix_t *res);
  void     divideRealMatrix               (const real34Matrix_t *matrix, const real34_t *x, real34Matrix_t *res);
  void     _divideRealMatrix              (const real34Matrix_t *matrix, const real_t *x, real34Matrix_t *res, realContext_t *realContext);
  void     divideRealMatrices             (const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res);

  void     complex_LU_decomposition       (const complex34Matrix_t *matrix, complex34Matrix_t *lu, uint16_t *p);
  void     complexMatrixSwapRows          (const complex34Matrix_t *matrix, complex34Matrix_t *res, uint16_t a, uint16_t b);
  void     detComplexMatrix               (const complex34Matrix_t *matrix, real34_t *res_r, real34_t *res_i);
  void     complex_matrix_inverse         (const complex34Matrix_t *matrix, complex34Matrix_t *res);
  void     divideComplexMatrix            (const complex34Matrix_t *matrix, const real34_t *xr, const real34_t *xi, complex34Matrix_t *res);
  void     _divideComplexMatrix           (const complex34Matrix_t *matrix, const real_t *xr, const real_t *xi, complex34Matrix_t *res, realContext_t *realContext);
  void     divideComplexMatrices          (const complex34Matrix_t *y, const complex34Matrix_t *x, complex34Matrix_t *res);

  void     WP34S_matrix_linear_eqn        (const real34Matrix_t *a, const real34Matrix_t *b, real34Matrix_t *r);
  void     complex_matrix_linear_eqn      (const complex34Matrix_t *a, const complex34Matrix_t *b, complex34Matrix_t *r);
#endif // TESTSUITE_BUILD

void       elementwiseRema                (void (*f)(void));
void       elementwiseRema_UInt16         (void (*f)(uint16_t), uint16_t param);
void       elementwiseRemaLonI            (void (*f)(void));
void       elementwiseRemaReal            (void (*f)(void));
void       elementwiseRemaShoI            (void (*f)(void));

void       elementwiseCxma                (void (*f)(void));
void       elementwiseCxma_UInt16         (void (*f)(uint16_t), uint16_t param);
void       elementwiseCxmaLonI            (void (*f)(void));
void       elementwiseCxmaReal            (void (*f)(void));
void       elementwiseCxmaShoI            (void (*f)(void));
void       elementwiseCxmaCplx            (void (*f)(void));

#ifndef TESTSUITE_BUILD
  void     callByIndexedMatrix            (bool_t (*real_f)(real34Matrix_t *), bool_t (*complex_f)(complex34Matrix_t *));
#endif // TESTSUITE_BUILD

#endif // MATRIX_H
