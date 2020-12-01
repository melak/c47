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
 * \file division.h
 ***********************************************/

void fnDivide(uint16_t unusedButMandatoryParameter);
void divError(void);

void divComplexComplex(const real_t *numerReal, const real_t *numerImag, const real_t *denomReal, const real_t *denomImag,    real_t *quotientReal, real_t *quotientImag, realContext_t *realContext);
void divRealComplex   (const real_t *numer,     const real_t *denomReal, const real_t *denomImag,       real_t *quotientReal, real_t *quotientImag,                       realContext_t *realContext);

//      RegYRegX
void divLonILonI(void);
void divLonIShoI(void);
void divLonIReal(void);
void divLonICplx(void);

//      RegYRegX
void divTimeLonI(void);
void divTimeShoI(void);
void divTimeReal(void);

//      RegYRegX
void divRemaLonI(void);
void divRemaShoI(void);
void divRemaReal(void);
void divRemaCplx(void);

//      RegYRegX
void divCxmaLonI(void);
void divCxmaShoI(void);
void divCxmaReal(void);
void divCxmaCplx(void);

//      RegYRegX
void divShoILonI(void);
void divShoIShoI(void);
void divShoIReal(void);
void divShoICplx(void);

//      RegYRegX
void divRealLonI(void);
void divRealShoI(void);
void divRealReal(void);
void divRealCplx(void);

//      RegYRegX
void divCplxLonI(void);
void divCplxShoI(void);
void divCplxReal(void);
void divCplxCplx(void);
