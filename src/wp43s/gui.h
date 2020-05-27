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

#define GAP                     6
#define Y_OFFSET_LETTER        18
#define X_OFFSET_LETTER         3
#define Y_OFFSET_SHIFTED_LABEL 25
#define Y_OFFSET_GREEK         27

#define DELTA_KEYS_X           78
#define DELTA_KEYS_Y           74
#define KEY_WIDTH_1            47
#define KEY_WIDTH_2            56

#define X_LEFT_PORTRAIT        45
#define X_LEFT_LANDSCAPE      544
#define Y_TOP_PORTRAIT        376
#define Y_TOP_LANDSCAPE        30

#ifndef TESTSUITE_BUILD
  void fnOff            (uint16_t unsuedParamButMandatory);
  void calcModeNormal   (void);
  void calcModeAsm      (void);
  void calcModeTam      (void);
  void calcModeAim      (uint16_t unusedParamButMandatory);
  void calcModeNim      (uint16_t unusedParamButMandatory);
#endif // TESTSUITE_BUILD

#ifdef PC_BUILD
  void setupUI          (void);
  void moveLabels       (void);     //JM Keep here
  void calcModeNormalGui(void);
  void calcModeAimGui   (void);
  void calcModeTamGui   (void);
#endif

#ifndef DMCP_BUILD
  void strReplace       (char *haystack, const char *needle, const char *newNeedle);
#endif
