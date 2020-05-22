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
 * \file bufferize.h
 ***********************************************/

#define AIM_BUFFER_LENGTH  200
#define TAM_BUFFER_LENGTH   30
#define NIM_BUFFER_LENGTH  200

#define TT_OPERATION       0
#define TT_LETTER          1
#define TT_VARIABLE        2
#define TT_DIGIT           3
#define TT_ENTER           4
#define TT_DOT             5
#define TT_INDIRECT        6
#define TT_BACKSPACE       7
#define TT_BASE10          8
#define TT_BASE16          9
#define TT_NOTHING        10

#ifndef TESTSUITE_BUILD
  void    fnAim                    (uint16_t unusedParamButMandatory);
  void    resetAlphaSelectionBuffer(void);
  void    addItemToBuffer          (uint16_t item);
  void    addItemToNimBuffer       (int16_t item);
  void    tamTransitionSystem      (uint16_t tamEvent);
  void    closeNim                 (void);
  void    nimBufferToDisplayBuffer (const char *nimBuffer, char *displayBuffer);
  int16_t getOperation             (void);
#endif
