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

/**
 * \file gui.h
 */
#ifndef GUI_H
#define GUI_H

#include "defines.h"
#include <stdint.h>

#ifndef TESTSUITE_BUILD
  void fnOff                       (uint16_t unsuedParamButMandatory);
  /**
   * Sets the calc mode to normal.
   */
  void calcModeNormal              (void);
  /**
   * Sets the calc mode to alpha input mode.
   *
   * \param[in] unusedButMandatoryParameter
   */
  void calcModeAim                 (uint16_t unusedButMandatoryParameter);
  /**
   * Sets the calc mode to number input mode.
   *
   * \param[in] unusedButMandatoryParameter
   */
  void calcModeNim                 (uint16_t unusedButMandatoryParameter);
  /**
   * Sets the calc mode to alpha selection menu if needed.
   */
  void enterAsmModeIfMenuIsACatalog(int16_t id);
  /**
   * Leaves the alpha selection mode.
   */
  void leaveAsmMode                (void);
#endif // TESTSUITE_BUILD

#ifdef PC_BUILD
  /**
   * Creates the calc's GUI window with all the widgets.
   */
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

#endif // GUI_H
