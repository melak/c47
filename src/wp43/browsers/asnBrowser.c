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
#include "c43Extensions/radioButtonCatalog.h"
#include "screen.h"
#include "softmenus.h"
#include <string.h>

#include "wp43.h"



#if !defined(TESTSUITE_BUILD)
  #ifndef SAVE_SPACE_DM42_8
  static void fnAsnDisplay(uint8_t page) {                // Heavily modified by JM from the original fnShow
    int xx,yy;
    int kk = 0;
    int16_t key;
    int16_t pixelsPerSoftKey;
    xx = 0;
    yy = 1;
    clearScreen();
        switch(page) {
          case 1:   showString("primary key assignment schedule", &standardFont, 30, 20, vmNormal, false, false); break;
          case 2:   showString("f-key assignment schedule", &standardFont, 30, 20, vmNormal, false, false); break;
          case 3:   showString("g-key assignment schedule", &standardFont, 30, 20, vmNormal, false, false); break;
          default:break;
        }

    for(key=0; key<37; key++) {
      if(key == 6 || key ==12 || key == 17 || key == 22 || key == 27 || key == 32) {
          xx = 0;
          yy ++;
      }
      if(key == 12) pixelsPerSoftKey = (int)((float)SCREEN_WIDTH / 3.0f + 0.5f); else
      if(key <  12) pixelsPerSoftKey = (int)((float)SCREEN_WIDTH / 6.0f + 0.5f); else
                    pixelsPerSoftKey = (int)((float)SCREEN_WIDTH / 5.0f + 0.5f);

      if(getSystemFlag(FLAG_USER)) {
        switch(page) {
          case 1: kk = kbd_usr[key].primary; break;
          case 2: kk = kbd_usr[key].fShifted; break;
          case 3: kk = kbd_usr[key].gShifted; break;
          default:break;
        }
      } else {
        switch(page) {
          case 1: kk = kbd_std[key].primary; break;
          case 2: kk = kbd_std[key].fShifted; break;
          case 3: kk = kbd_std[key].gShifted; break;
          default:break;
        }
      }

      showKey(indexOfItems[max(kk,-kk)].itemSoftmenuName, xx*pixelsPerSoftKey, xx*pixelsPerSoftKey+pixelsPerSoftKey, 20+yy*SOFTMENU_HEIGHT, 20+(yy+1)*SOFTMENU_HEIGHT, xx == 5, kk > 0 ? vmNormal : vmReverse, true, true, NOVAL, NOVAL, NOTEXT);

      if(getSystemFlag(FLAG_USER) && 
          ( ((page == 1) && (kbd_std[key].primary == kbd_usr[key].primary)  ) || 
            ((page == 2) && (kbd_std[key].fShifted == kbd_usr[key].fShifted)) || 
            ((page == 3) && (kbd_std[key].gShifted == kbd_usr[key].gShifted))    )
        ) {
          greyOutBox(xx*pixelsPerSoftKey, xx*pixelsPerSoftKey+pixelsPerSoftKey, 20+yy*SOFTMENU_HEIGHT, 20+(yy+1)*SOFTMENU_HEIGHT);
      }
    xx++;
    }

    temporaryInformation = TI_NO_INFO;
  }
  #endif //SAVE_SPACE_DM42_8
#endif // !TESTSUITE_BUILD


void fnAsnViewer(int16_t unusedButMandatoryParameter) {
#if !defined(TESTSUITE_BUILD)
  #ifndef SAVE_SPACE_DM42_8
    hourGlassIconEnabled = false;
    if(calcMode != CM_ASN_BROWSER) {
      previousCalcMode = calcMode;
      calcMode = CM_ASN_BROWSER;
      clearSystemFlag(FLAG_ALPHA);
      return;
    }
  fnAsnDisplay(currentAsnScr);
  #endif //SAVE_SPACE_DM42_8
#endif // !TESTSUITE_BUILD

  }
