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
 * \file lnbeta.h
 ***********************************************/

void fnLnBeta      (uint16_t unusedButMandatoryParameter);

void lnbetaError   (void);

void lnbetaLonILonI(void);
void lnbetaRealLonI(void);
void lnbetaCplxLonI(void);

void lnbetaLonIReal(void);
void lnbetaRealReal(void);
void lnbetaCplxReal(void);

void lnbetaLonICplx(void);
void lnbetaRealCplx(void);
void lnbetaCplxCplx(void);
