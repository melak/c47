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
 * \file power.h
 ***********************************************/

void fnPower    (uint16_t unusedButMandatoryParameter);
void powError   (void);

uint8_t PowerComplex(const real_t *yReal, const real_t *yImag, const real_t *xReal, const real_t *xImag,
                     real_t *rReal, real_t *rImag, realContext_t *realContext);


//      RegYRegX
void powLonILonI(void);
void powLonIShoI(void);
void powLonIReal(void);
void powLonICplx(void);

//      RegYRegX
void powRemaLonI(void);
void powRemaShoI(void);
void powRemaReal(void);
void powRemaCplx(void);

//      RegYRegX
void powCxmaLonI(void);
void powCxmaShoI(void);
void powCxmaReal(void);
void powCxmaCplx(void);

//      RegYRegX
void powShoILonI(void);
void powShoIShoI(void);
void powShoIReal(void);
void powShoICplx(void);

//      RegYRegX
void powRealLonI(void);
void powRealShoI(void);
void powRealReal(void);
void powRealCplx(void);

//      RegYRegX
void powCplxLonI(void);
void powCplxShoI(void);
void powCplxReal(void);
void powCplxCplx(void);
