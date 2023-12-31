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
#if !defined(MATRIX_H)
  #define MATRIX_H

  #include "typeDefinitions.h"

  /**
   * Creates new Matrix of size y->m x x ->n.
   *
   * \param[in] unusedParamButMandatory
   */
  void       fnNewMatrix                    (uint16_t unusedParamButMandatory);

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
   * Angle between vectors X and Y.
   *
   * \param[in] unusedParamButMandatory
   */
  void       fnVectorAngle                  (uint16_t unusedParamButMandatory);


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

  /**
   * QR decomposition. Square matrices only.
   * Returns unitary matrix Q in Y and upper triangular matrix R in X.
   *
   * \param[in] unusedParamButMandatory
   */
  void       fnQrDecomposition              (uint16_t unusedParamButMandatory);

  /**
   * Calculate eigenvalues.
   * Generally returns in a diagonal matrix.
   *
   * \param[in] unusedParamButMandatory
   */
  void       fnEigenvalues                  (uint16_t unusedParamButMandatory);

  void       fnEigenvectors                 (uint16_t unusedParamButMandatory);

  /**
   * Saves the STATS matrix if STATS is available.
   *
   * \return true if succeeded or not needed to backup, false if unsuccessful allocation
   */
  bool_t   saveStatsMatrix                (void);

  /**
   * Restores the STATS matrix if backed up STATS is available.
   *
   * \return true if succeeded, otherwise false
   */
  bool_t   recallStatsMatrix              (void);

  /**
   * Initialize a real matrix.
   *
   * \param[out] matrix
   * \param[in] rows
   * \param[in] cols
   * \return true if succeeded, false otherwise
   */
  bool_t   realMatrixInit                 (real34Matrix_t *matrix, uint16_t rows, uint16_t cols);

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

  #if !defined(TESTSUITE_BUILD)
    /**
     * Redemention a real matrix.
     *
     * \param[in] matrix
     */
    void     realMatrixRedim                (real34Matrix_t *matrix, uint16_t rows, uint16_t cols);
  #endif // !TESTSUITE_BUILD

  /**
   * Initialize a complex matrix.
   *
   * \param[out] matrix
   * \param[in] rows
   * \param[in] cols
   * \return true if succeeded, false otherwise
   */
  bool_t   complexMatrixInit              (complex34Matrix_t *matrix, uint16_t rows, uint16_t cols);

  /**
   * Free a complex matrix.
   *
   * \param[in] matrix
   */
  void     complexMatrixFree              (complex34Matrix_t *matrix);

  #if !defined(TESTSUITE_BUILD)
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
    void     mimRunFunction                 (int16_t func, uint16_t param);
    void     mimFinalize                    (void);
    void     mimRestore                     (void);

    /**
     * Displays a real matrix.
     *
     * \param[in] matrix
     * \param[in] prefixWidth
     */
    void     showRealMatrix                 (const real34Matrix_t *matrix, int16_t prefixWidth);

    /**
     * Calculates width of columns of a real matrix.
     *
     * \param[in] matrix
     * \param[in] prefixWidth
     * \param[in] font
     * \param[out] colWidth Column width. This must be initialized as int16_t[MATRIX_MAX_COLUMNS].
     * \param[out] rPadWidth Right padding of each elements. This is for alignment of decimal points. This must be initialized as int16_t[MATRIX_MAX_ROWS * MATRIX_MAX_COLUMNS].
     * \param[out] digits Number of digits which will be shown. This is for adjustment in ALL mode.
     * \param[in] maxCols Maximum number of columns to allow
     * \return Width of the matrix excluding brackets
     */
    int16_t  getRealMatrixColumnWidths      (const real34Matrix_t *matrix, int16_t prefixWidth, const font_t *font, int16_t *colWidth, int16_t *rPadWidth, int16_t *digits, uint16_t maxCols);

    /**
     * Displays a complex matrix.
     *
     * \param[in] matrix
     * \param[in] prefixWidth
     */
    void     showComplexMatrix              (const complex34Matrix_t *matrix, int16_t prefixWidth);

    /**
     * Calculates width of columns of a complex matrix.
     *
     * \param[in] matrix
     * \param[in] prefixWidth
     * \param[in] font
     * \param[out] colWidth Column width. This must be initialized as int16_t[MATRIX_MAX_COLUMNS].
     * \param[out] colWidth_r Column width of real part. This must be initialized as int16_t[MATRIX_MAX_COLUMNS].
     * \param[out] colWidth_i Column width of imaginary part. This must be initialized as int16_t[MATRIX_MAX_COLUMNS].
     * \param[out] rPadWidth_r Right padding of real part of each elements. This is for alignment of decimal points. This must be initialized as int16_t[MATRIX_MAX_ROWS * MATRIX_MAX_COLUMNS].
     * \param[out] rPadWidth_i Right padding of imaginary part of each elements. This is for alignment of decimal points. This must be initialized as int16_t[MATRIX_MAX_ROWS * MATRIX_MAX_COLUMNS].
     * \param[out] digits Number of digits which will be shown. This is for adjustment in ALL mode.
     * \param[in] maxCols Maximum number of columns to allow
     * \return Width of the matrix excluding brackets
     */
    int16_t  getComplexMatrixColumnWidths   (const complex34Matrix_t *matrix, int16_t prefixWidth, const font_t *font, int16_t *colWidth, int16_t *colWidth_r, int16_t *colWidth_i,
                                             int16_t *rPadWidth_r, int16_t *rPadWidth_i, int16_t *digits, uint16_t maxCols);

    void     getMatrixFromRegister          (calcRegister_t regist);
  #endif // !TESTSUITE_BUILD

  /**
   * Creates a zero matrix at given register.
   *
   * \param[in] regist
   * \param[in] rows
   * \param[in] cols
   * \param[in] complex  true for complex matrix, false for real matrix.
   * \return true if succeeded, false otherwise
   */
  bool_t   initMatrixRegister             (calcRegister_t regist, uint16_t rows, uint16_t cols, bool_t complex);

  #if !defined(TESTSUITE_BUILD)
    /**
     * Redimentions the matrix at given register.
     * Shrinking the matrix is in situ while enlarging the matrix is not.
     * The elements are preserved as many as possible.
     *
     * \warning This function invalidates variables assosiated by \link linkToRealMatrixRegister() \endlink/\link linkToComplexMatrixRegister() \endlink
     *          if you are making the matrix larger.
     * \warning Redo \link linkToRealMatrixRegister() \endlink/\link linkToComplexMatrixRegister() \endlink to use the variables again.
     * \param[in] regist
     * \param[in] rows
     * \param[in] cols
     * \return true if succeeded, false otherwise
     */
    bool_t   redimMatrixRegister            (calcRegister_t regist, uint16_t rows, uint16_t cols);

    /**
     * Allocates a named matrix. Redimentions if the matrix already existed.
     *
     * \param[in] name
     * \param[in] rows
     * \param[in] cols
     * \return register ID where the allocated matrix lies. \c INVALID_VARIABLE if allocation failed.
     */
    calcRegister_t allocateNamedMatrix      (const char *name, uint16_t rows, uint16_t cols);

    /**
     * Appends a row to the matrix at given register.
     * For named matrix, use together with findNamedVariable().
     *
     * \warning This function invalidates variables assosiated by \link linkToRealMatrixRegister() \endlink/\link linkToComplexMatrixRegister() \endlink.
     * \warning Redo \link linkToRealMatrixRegister() \endlink/\link linkToComplexMatrixRegister() \endlink to use the variables again.
     * \param[in] regist
     * \return true if succeeded, false otherwise
     */
    bool_t   appendRowAtMatrixRegister      (calcRegister_t regist);
  #endif // !TESTSUITE_BUILD

  void     copyRealMatrix                 (const real34Matrix_t *matrix, real34Matrix_t *res);

  #if !defined(TESTSUITE_BUILD)
    void     insRowRealMatrix               (real34Matrix_t *matrix, uint16_t beforeRowNo);
    void     delRowRealMatrix               (real34Matrix_t *matrix, uint16_t beforeRowNo);
  #endif // !TESTSUITE_BUILD
  void     transposeRealMatrix            (const real34Matrix_t *matrix, real34Matrix_t *res);

  void     copyComplexMatrix              (const complex34Matrix_t *matrix, complex34Matrix_t *res);

  #if !defined(TESTSUITE_BUILD)
    void     insRowComplexMatrix            (complex34Matrix_t *matrix, uint16_t beforeRowNo);
    void     delRowComplexMatrix            (complex34Matrix_t *matrix, uint16_t beforeRowNo);
  #endif // !TESTSUITE_BUILD
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

  void     euclideanNormRealMatrix        (const real34Matrix_t *matrix, real34_t *res);
  void     euclideanNormComplexMatrix     (const complex34Matrix_t *matrix, real34_t *res);

  uint16_t realVectorSize                 (const real34Matrix_t *matrix);
  void     dotRealVectors                 (const real34Matrix_t *y, const real34Matrix_t *x, real34_t *res);
  void     crossRealVectors               (const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res);

  uint16_t complexVectorSize              (const complex34Matrix_t *matrix);
  void     dotComplexVectors              (const complex34Matrix_t *y, const complex34Matrix_t *x, real34_t *res_r, real34_t *res_i);
  void     crossComplexVectors            (const complex34Matrix_t *y, const complex34Matrix_t *x, complex34Matrix_t *res);

  void     vectorAngle                    (const real34Matrix_t *y, const real34Matrix_t *x, real34_t *radians);

  void     WP34S_LU_decomposition         (const real34Matrix_t *matrix, real34Matrix_t *lu, uint16_t *p);
  void     realMatrixSwapRows             (const real34Matrix_t *matrix, real34Matrix_t *res, uint16_t a, uint16_t b);
  void     detRealMatrix                  (const real34Matrix_t *matrix, real34_t *res);
  void     invertRealMatrix               (const real34Matrix_t *matrix, real34Matrix_t *res);
  void     divideRealMatrix               (const real34Matrix_t *matrix, const real34_t *x, real34Matrix_t *res);
  void     _divideRealMatrix              (const real34Matrix_t *matrix, const real_t *x, real34Matrix_t *res, realContext_t *realContext);
  void     divideByRealMatrix             (const real34_t *y, const real34Matrix_t *matrix, real34Matrix_t *res);
  void     _divideByRealMatrix            (const real_t *y, const real34Matrix_t *matrix, real34Matrix_t *res, realContext_t *realContext);
  void     divideRealMatrices             (const real34Matrix_t *y, const real34Matrix_t *x, real34Matrix_t *res);

  void     complex_LU_decomposition       (const complex34Matrix_t *matrix, complex34Matrix_t *lu, uint16_t *p);
  #if !defined(TESTSUITE_BUILD)
    void     complexMatrixSwapRows          (const complex34Matrix_t *matrix, complex34Matrix_t *res, uint16_t a, uint16_t b);
  #endif // !TESTSUITE_BUILD
  void     detComplexMatrix               (const complex34Matrix_t *matrix, real34_t *res_r, real34_t *res_i);
  void     invertComplexMatrix            (const complex34Matrix_t *matrix, complex34Matrix_t *res);
  void     divideComplexMatrix            (const complex34Matrix_t *matrix, const real34_t *xr, const real34_t *xi, complex34Matrix_t *res);
  void     _divideComplexMatrix           (const complex34Matrix_t *matrix, const real_t *xr, const real_t *xi, complex34Matrix_t *res, realContext_t *realContext);
  void     divideByComplexMatrix          (const real34_t *yr, const real34_t *yi, const complex34Matrix_t *matrix, complex34Matrix_t *res);
  void     _divideByComplexMatrix         (const real_t *yr, const real_t *yi, const complex34Matrix_t *matrix, complex34Matrix_t *res, realContext_t *realContext);
  void     divideComplexMatrices          (const complex34Matrix_t *y, const complex34Matrix_t *x, complex34Matrix_t *res);

  #if !defined(TESTSUITE_BUILD)
    void     real_matrix_linear_eqn         (const real34Matrix_t *a, const real34Matrix_t *b, real34Matrix_t *r);
    void     complex_matrix_linear_eqn      (const complex34Matrix_t *a, const complex34Matrix_t *b, complex34Matrix_t *r);
  #endif // !TESTSUITE_BUILD

  void     real_QR_decomposition          (const real34Matrix_t *matrix, real34Matrix_t *q, real34Matrix_t *r);
  void     complex_QR_decomposition       (const complex34Matrix_t *matrix, complex34Matrix_t *q, complex34Matrix_t *r);

  void     realEigenvalues                (const real34Matrix_t *matrix, real34Matrix_t *res, real34Matrix_t *ires);
  void     complexEigenvalues             (const complex34Matrix_t *matrix, complex34Matrix_t *res);
  void     realEigenvectors               (const real34Matrix_t *matrix, real34Matrix_t *res, real34Matrix_t *ires);
  void     complexEigenvectors            (const complex34Matrix_t *matrix, complex34Matrix_t *res);
  #if !defined(TESTSUITE_BUILD)
    void     callByIndexedMatrix            (bool_t (*real_f)(real34Matrix_t *), bool_t (*complex_f)(complex34Matrix_t *));
  #endif // !TESTSUITE_BUILD

  void       linkToRealMatrixRegister       (calcRegister_t regist, real34Matrix_t *linkedMatrix);
  void       linkToComplexMatrixRegister    (calcRegister_t regist, complex34Matrix_t *linkedMatrix);

  void       elementwiseRema                (void (*f)(void));
  void       elementwiseRema_UInt16         (void (*f)(uint16_t), uint16_t param);
  void       elementwiseRemaLonI            (void (*f)(void));
  void       elementwiseRemaReal            (void (*f)(void));
  void       elementwiseRemaShoI            (void (*f)(void));
  void       elementwiseRealRema            (void (*f)(void));

  void       elementwiseCxma                (void (*f)(void));
  void       elementwiseCxma_UInt16         (void (*f)(uint16_t), uint16_t param);
  void       elementwiseCxmaLonI            (void (*f)(void));
  void       elementwiseCxmaReal            (void (*f)(void));
  void       elementwiseCxmaShoI            (void (*f)(void));
  void       elementwiseCxmaCplx            (void (*f)(void));
  void       elementwiseRealCxma            (void (*f)(void));
#endif // !MATRIX_H
