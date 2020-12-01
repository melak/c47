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
 * \file saveRestoreCalcState.h
 ***********************************************/

#if defined(PC_BUILD) || defined(DMCP_BUILD)
  void   saveCalc      (void);
  void   restoreCalc   (void);
#endif // PC_BUILD || DMCP_BUILD

void     fnSave        (uint16_t unusedButMandatoryParameter);
void     fnLoad        (uint16_t loadMode);
uint8_t  stringToUint8 (const char *str);
uint16_t stringToUint16(const char *str);
uint32_t stringToUint32(const char *str);
uint64_t stringToUint64(const char *str);
int16_t  stringToInt16 (const char *str);
int32_t  stringToInt32 (const char *str);
