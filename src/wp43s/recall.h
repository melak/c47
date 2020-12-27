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
 * \file recall.h
 ***********************************************/


void         fnRecall       (uint16_t r);
void         fnLastX        (uint16_t unusedButMandatoryParameter);
void         fnRecallAdd    (uint16_t r);
void         fnRecallSub    (uint16_t r);
void         fnRecallMult   (uint16_t r);
void         fnRecallDiv    (uint16_t r);
void         fnRecallMin    (uint16_t r);
void         fnRecallMax    (uint16_t r);
void         fnRecallConfig (uint16_t r);
void         fnRecallStack  (uint16_t r);
void         fnRecallElement(uint16_t unusedButMandatoryParameter);
void         fnRecallIJ     (uint16_t unusedButMandatoryParameter);

