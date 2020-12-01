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

void fnClX         (uint16_t unusedButMandatoryParameter);
void fnDrop        (uint16_t unusedButMandatoryParameter);
void liftStack     (void);
void fnDropY       (uint16_t unusedButMandatoryParameter);
void fnRollUp      (uint16_t unusedButMandatoryParameter);
void fnRollDown    (uint16_t unusedButMandatoryParameter);
void fnDisplayStack(uint16_t numberOfStackLines);
void fnSwapX       (uint16_t regist);
void fnSwapY       (uint16_t regist);
void fnSwapZ       (uint16_t regist);
void fnSwapT       (uint16_t regist);
void fnSwapXY      (uint16_t unusedButMandatoryParameter);
void fnShuffle     (uint16_t unusedButMandatoryParameter);
void fnFillStack   (uint16_t unusedButMandatoryParameter);
void fnGetStackSize(uint16_t unusedButMandatoryParameter);
void fnClearStack  (uint16_t unusedButMandatoryParameter);
void fnUndo        (uint16_t unusedButMandatoryParameter);
void saveForUndo   (void);
void undo          (void);
