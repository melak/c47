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

#define X_DATE             1
#define X_REAL_COMPLEX   133
#define X_COMPLEX_MODE   143
#define X_ANGULAR_MODE   157
#define X_FRAC_MODE      185
#define X_INTEGER_MODE   260
#define X_OVERFLOW_CARRY 292
#define X_ALPHA_MODE     300
#define X_HOURGLASS      311
#define X_PROGRAM_BEGIN  324
#define X_WATCH          336
#define X_SERIAL_IO      351
#define X_PRINTER        361
#define X_USER_MODE      375
#define X_BATTERY        389

#ifndef TESTSUITE_BUILD
void refreshStatusBar     (void);
void showDateTime         (void);
void showRealComplexResult(void);
void showComplexMode      (void);
void showAngularMode      (void);
void showFracMode         (void);
void showIntegerMode      (void);
void showOverflowCarry    (void);
void showHideAlphaMode    (void);
void showHideHourGlass    (void);
void showHidePgmBegin     (void);
void showHideWatch        (void);
void showHideSerialIO     (void);
void showHidePrinter      (void);
void showHideUserMode     (void);
void showHideLowBattery   (void);
#endif
