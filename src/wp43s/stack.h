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

void fnClX         (uint16_t unusedParamButMandatory);
void fnDrop        (uint16_t unusedParamButMandatory);
void liftStack     (void);
void fnDropY       (uint16_t unusedParamButMandatory);
void fnRollUp      (uint16_t unusedParamButMandatory);
void fnRollDown    (uint16_t unusedParamButMandatory);
void fnDisplayStack(uint16_t numberOfStackLines);
void fnSwapXY      (uint16_t unusedParamButMandatory);
void fnFillStack   (uint16_t unusedParamButMandatory);
void fnGetStackSize(uint16_t unusedParamButMandatory);
void fnClearStack  (uint16_t unusedParamButMandatory);
void saveStack     (void);
void restoreStack  (void);
void refreshStack  (void);
#ifdef PC_BUILD
  void stackLiftEnable (void);
  void stackLiftDisable(void);
#endif
