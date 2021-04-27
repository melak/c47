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
 * \file curveFitting.h
 ***********************************************/
#ifndef CURVEFITTING_H
#define CURVEFITTING_H

#include <stdint.h>

#include "wp43s.h"

#define useFLOAT   0
#define useREAL4   4
#define useREAL39 39


extern   realContext_t           *realContext;
void     fnCurveFitting          (uint16_t curveFitting);
void     processCurvefitSelection(uint16_t selection, real_t *RR_, real_t *SMI_, real_t *aa0, real_t *aa1, real_t *aa2);
void     fnProcessLR             (uint16_t unusedButMandatoryParameter);
uint16_t lrCountOnes             (uint16_t curveFitting);
void     yIsFnx                  (uint8_t  USEFLOAT, uint16_t selection, double x, double *y, double a0, double a1, double a2, real_t *XX, real_t *YY, real_t *RR, real_t *SMI, real_t *aa0, real_t *aa1, real_t *aa2);
void     fnYIsFnx                (uint16_t unusedButMandatoryParameter);
void     fnXIsFny                (uint16_t unusedButMandatoryParameter);
void     fnCurveFittingLR        (uint16_t unusedButMandatoryParameter);

#endif // CURVEFITTING_H
