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
 * \file statusBar.h
 ***********************************************/
#ifndef STATUSBAR_H
#define STATUSBAR_H

#ifndef TESTSUITE_BUILD
  void refreshStatusBar       (void);
  void showDateTime           (void);
  void showRealComplexResult  (void);
  void showComplexMode        (void);
  void showAngularMode        (void);
  void showFracMode           (void);
  void showIntegerMode        (void);
  void showMatrixMode         (void);
  void showOverflowCarry      (void);
  void showHideAlphaMode      (void);
  void showHideHourGlass      (void);
  void showHidePgmBegin       (void);
  void showHideWatch          (void);
  void showHideSerialIO       (void);
  void showHidePrinter        (void);
  void showHideUserMode       (void);
  #ifdef DMCP_BUILD
    void showHideUsbLowBattery(void);
  #else // !DMCP_BUILD
    void showHideStackLift    (void);
  #endif // DMCP_BUILD
#endif // !TESTSUITE_BUILD

#endif // STATUSBAR_H
