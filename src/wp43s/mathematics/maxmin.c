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
 * \file maxmin.c
 ***********************************************/
// Coded by JM

#include "wp43s.h"


/********************************************//**
 * \brief x sub M sub A sub X ==> regX, regY
 * enables stack lift and refreshes the stack.
 * regX = MAX x, regY = MAX y
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnMAXXY(uint16_t unusedParamButMandatory) {
real_t x,y;
  if(checkMinimumDataPoints(const_1)) {
    saveStack();

    liftStack();
    STACK_LIFT_ENABLE;
    liftStack();

    if(realCompareGreaterThan(MAX_X, const_minusInfinity))
      realCopy(MAX_X, &x);
    else
      realCopy(const_0, &x);

    if(realCompareGreaterThan(MAX_Y, const_minusInfinity))
      realCopy(MAX_X, &y);
    else
      realCopy(const_0, &y);

    realToReal34(MAX_X, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(MAX_Y, REGISTER_REAL34_DATA(REGISTER_Y));

    temporaryInformation = TI_MAXX_MAXY;
    refreshStack();
  }
}

/********************************************//**
 * \brief x sub M sub I sub N ==> regX, regY
 * enables stack lift and refreshes the stack.
 * regX = MAX x, regY = MAX y
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnMINXY(uint16_t unusedParamButMandatory) {
real_t x,y;
  if(checkMinimumDataPoints(const_1)) {
    saveStack();

    liftStack();
    STACK_LIFT_ENABLE;
    liftStack();

    if(realCompareLessThan(MIN_X, const_plusInfinity))
      realCopy(MAX_X, &x);
    else
      realCopy(const_0, &x);

    if(realCompareGreaterThan(MIN_Y, const_plusInfinity))
      realCopy(MAX_X, &y);
    else
      realCopy(const_0, &y);

    realToReal34(MIN_X, REGISTER_REAL34_DATA(REGISTER_X));
    realToReal34(MIN_Y, REGISTER_REAL34_DATA(REGISTER_Y));

    temporaryInformation = TI_MINX_MINY;
    refreshStack();
  }
}

