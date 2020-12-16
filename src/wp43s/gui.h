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
 * \file gui.h
 ***********************************************/

#ifndef TESTSUITE_BUILD
  void fnOff                       (uint16_t unsuedParamButMandatory);
  void calcModeNormal              (void);
  void calcModeAim                 (uint16_t unusedButMandatoryParameter);
  void calcModeNim                 (uint16_t unusedButMandatoryParameter);
  void enterAsmModeIfMenuIsACatalog(int16_t id);
  void enterTamMode                (void);
  void leaveAsmMode                (void);
  void leaveTamMode                (void);
#endif // TESTSUITE_BUILD

#ifdef PC_BUILD
  void setupUI                     (void);
  #if (SCREEN_800X480 == 0)
    void calcModeNormalGui         (void);
    void calcModeAimGui            (void);
    void calcModeTamGui            (void);
  #endif // SCREEN_800X480
#endif // PC_BUILD

#ifndef DMCP_BUILD
  void strReplace                  (char *haystack, const char *needle, const char *newNeedle);
#endif // DMCP_BUILD
