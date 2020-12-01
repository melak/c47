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
 * \file remainder.h
 ***********************************************/

void fnRmd(uint16_t unusedButMandatoryParameter);
void rmdError(void);

//      RegYRegX
void rmdLonILonI(void);
void rmdLonIShoI(void);
void rmdLonIReal(void);

//      RegYRegX
void rmdShoILonI(void);
void rmdShoIShoI(void);
void rmdShoIReal(void);

//      RegYRegX
void rmdRealLonI(void);
void rmdRealShoI(void);
void rmdRealReal(void);
