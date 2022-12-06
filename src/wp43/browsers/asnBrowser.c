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
 * \file asnBrowser.c The assign browser application
 ***********************************************/

#include "browsers/registerBrowser.h"

#include "flags.h"
#include "softmenus.h"
#include <string.h>

#include "wp43.h"



#if !defined(TESTSUITE_BUILD)
#ifndef SAVE_SPACE_DM42_8
  void fnAsnViewer(int16_t unusedButMandatoryParameter) {

printf("currentAsnScr = %d\n",currentAsnScr);

    if(calcMode != CM_ASN_BROWSER) {
      previousCalcMode = calcMode;
      calcMode = CM_ASN_BROWSER;
      clearSystemFlag(FLAG_ALPHA);
      return;
    }


  fnTest(currentAsnScr);

  }
    #endif //SAVE_SPACE_DM42_8
#endif // !TESTSUITE_BUILD
