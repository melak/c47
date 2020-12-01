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
 * \file xthRoot.h
 ***********************************************/
// Coded by JM, based on power.h

void fnXthRoot      (uint16_t unusedButMandatoryParameter);
void xthRootError   (void);

//      RegYRegX
void xthRootLonILonI(void);
void xthRootLonIShoI(void);
void xthRootLonIReal(void);
void xthRootLonICplx(void);

//      RegYRegX
void xthRootRemaLonI(void);
void xthRootRemaShoI(void);
void xthRootRemaReal(void);
void xthRootRemaCplx(void);

//      RegYRegX
void xthRootCxmaLonI(void);
void xthRootCxmaShoI(void);
void xthRootCxmaReal(void);
void xthRootCxmaCplx(void);

//      RegYRegX
void xthRootShoILonI(void);
void xthRootShoIShoI(void);
void xthRootShoIReal(void);
void xthRootShoICplx(void);

//      RegYRegX
void xthRootRealLonI(void);
void xthRootRealShoI(void);
void xthRootRealReal(void);
void xthRootRealCplx(void);

//      RegYRegX
void xthRootCplxLonI(void);
void xthRootCplxShoI(void);
void xthRootCplxReal(void);
void xthRootCplxCplx(void);
