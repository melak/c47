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
 * \file addition.h
 ***********************************************/

void fnAdd      (uint16_t unusedButMandatoryParameter);
void addError   (void);

//      RegYRegX
void addLonILonI(void);
void addLonITime(void);
void addLonIDate(void);
void addLonIShoI(void);
void addLonIReal(void);
void addLonICplx(void);

//      RegYRegX
void addTimeLonI(void);
void addTimeTime(void);
void addTimeReal(void);

//      RegYRegX
void addDateLonI(void);
void addDateReal(void);

//      RegYRegX
void addStriLonI(void);
void addStriTime(void);
void addStriStri(void);
void addStriRema(void);
void addStriCxma(void);
void addStriDate(void);
void addStriShoI(void);
void addStriReal(void);
void addStriCplx(void);

//      RegYRegX
void addRemaRema(void);
void addRemaCxma(void);

//      RegYRegX
void addCxmaRema(void);
void addCxmaCxma(void);

//      RegYRegX
void addShoILonI(void);
void addShoIShoI(void);
void addShoIReal(void);
void addShoICplx(void);

//      RegYRegX
void addRealLonI(void);
void addRealTime(void);
void addRealDate(void);
void addRealShoI(void);
void addRealReal(void);
void addRealCplx(void);

//      RegYRegX
void addCplxLonI(void);
void addCplxShoI(void);
void addCplxReal(void);
void addCplxCplx(void);
