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
 * \file comparisonReals.h
 ***********************************************/

bool_t  real16CompareAbsGreaterThan (const real16_t *number1, const real16_t *number2);
bool_t  real16CompareAbsLessThan    (const real16_t *number1, const real16_t *number2);
bool_t  real16CompareEqual          (const real16_t *number1, const real16_t *number2);
bool_t  real16CompareGreaterEqual   (const real16_t *number1, const real16_t *number2);
bool_t  real16CompareGreaterThan    (const real16_t *number1, const real16_t *number2);
bool_t  real16CompareLessEqual      (const real16_t *number1, const real16_t *number2);
bool_t  real16CompareLessThan       (const real16_t *number1, const real16_t *number2);

bool_t  real34CompareAbsGreaterThan (const real34_t *number1, const real34_t *number2);
bool_t  real34CompareAbsGreaterEqual(const real34_t *number1, const real34_t *number2);
bool_t  real34CompareAbsLessThan    (const real34_t *number1, const real34_t *number2);
bool_t  real34CompareEqual          (const real34_t *number1, const real34_t *number2);
bool_t  real34CompareGreaterEqual   (const real34_t *number1, const real34_t *number2);
bool_t  real34CompareGreaterThan    (const real34_t *number1, const real34_t *number2);
bool_t  real34CompareLessEqual      (const real34_t *number1, const real34_t *number2);
bool_t  real34CompareLessThan       (const real34_t *number1, const real34_t *number2);

bool_t  realCompareAbsGreaterThan   (const real_t *number1, const real_t *number2);
bool_t  realCompareAbsLessThan      (const real_t *number1, const real_t *number2);
bool_t  realCompareEqual            (const real_t *number1, const real_t *number2);
bool_t  realCompareGreaterEqual     (const real_t *number1, const real_t *number2);
bool_t  realCompareGreaterThan      (const real_t *number1, const real_t *number2);
bool_t  realCompareLessEqual        (const real_t *number1, const real_t *number2);
bool_t  realCompareLessThan         (const real_t *number1, const real_t *number2);

bool_t  real16IsAnInteger           (const real16_t *x);
bool_t  real34IsAnInteger           (const real34_t *x);
bool_t  realIsAnInteger             (const real_t *x);
int16_t realIdenticalDigits         (real_t *a, real_t *b);
