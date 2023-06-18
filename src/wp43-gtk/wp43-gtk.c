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
 * \file wp43-gtk.c
 ***********************************************/

#include "flags.h"
#include "gtkGui.h"
#include "items.h"
#include "keyboard.h"
#include "hal/gui.h"
#include "calcMode.h"
#include "c43Extensions/keyboardTweak.h"
#include "longIntegerType.h"
#include "memory.h"
#include "saveRestoreCalcState.h"
#include "screen.h"
#include "timer.h"

#include "wp43.h"

#if defined(PC_BUILD)
  bool_t              calcLandscape;
  bool_t              calcAutoLandscapePortrait;
  GtkWidget           *screen;
  GtkWidget           *frmCalc;
  int16_t             screenStride;
  int16_t             debugWindow;
  uint32_t            *screenData;
  bool_t              screenChange;
  char                debugString[10000];
  #if(DEBUG_REGISTER_L == 1)
    GtkWidget         *lblRegisterL1;
    GtkWidget         *lblRegisterL2;
  #endif // (DEBUG_REGISTER_L == 1)
  #if(SHOW_MEMORY_STATUS == 1)
    GtkWidget         *lblMemoryStatus;
  #endif // (SHOW_MEMORY_STATUS == 1)
  calcKeyboard_t       calcKeyboard[43];
  int                  currentBezel; // 0=normal, 1=AIM, 2=TAM

  #if defined(EXPORT_ITEMS)
    int sortItems(void const *a, void const *b) {
      return compareString(a, b, CMP_EXTENSIVE);
    }
  #endif // EXPORT_ITEMS

  int main(int argc, char* argv[]) {
    #if defined(__APPLE__)
      // we take the directory where the application is as the root for this application.
      // in argv[0] is the application itself. We strip the name of the app by searching for the last '/':
      if(argc>=1) {
        char *curdir = malloc(1000);
        // find last /:
        char *s = strrchr(argv[0], '/');
        if(s != 0) {
          // take the directory before the appname:
          strncpy(curdir, argv[0], s-argv[0]);
          chdir(curdir);
          free(curdir);
        }
      }
    #endif // __APPLE__

    wp43MemInBlocks = 0;
    gmpMemInBytes = 0;
    mp_set_memory_functions(allocGmp, reallocGmp, freeGmp);

    calcLandscape             = false;
    calcAutoLandscapePortrait = true;

    for(int arg=1; arg<argc; arg++) {
      if(strcmp(argv[arg], "--landscape") == 0) {
        calcLandscape             = true;
        calcAutoLandscapePortrait = false;
      }

      if(strcmp(argv[arg], "--portrait") == 0) {
        calcLandscape             = false;
        calcAutoLandscapePortrait = false;
      }

      if(strcmp(argv[arg], "--auto") == 0) {
        calcLandscape             = false;
        calcAutoLandscapePortrait = true;
      }
    }

    if(strcmp(indexOfItems[LAST_ITEM].itemSoftmenuName, "Last item") != 0) {
      printf("The last item (%u)of indexOfItems[] is not \"Last item\", but is %s\n",LAST_ITEM,indexOfItems[LAST_ITEM].itemSoftmenuName);
      exit(1);
    }

    #if defined(EXPORT_ITEMS)
      char name[LAST_ITEM][16], nameUtf8[25];
      int cat, nbrItems = 0;
      for(int i=1; i<LAST_ITEM; i++) {
        cat = indexOfItems[i].status & CAT_STATUS;
        if(cat == CAT_FNCT || cat == CAT_CNST || cat == CAT_SYFL || cat == CAT_RVAR) {
          strncpy(name[nbrItems++], indexOfItems[i].itemCatalogName, 16);
        }
      }
      qsort(name, nbrItems, 16, sortItems);
      printf("To be meaningfull, the list below must\n");
      printf("be displayed with the C47__StandardFont!\n");
      for(int i=0; i<nbrItems; i++) {
        stringToUtf8(name[i], (uint8_t *)nameUtf8);
        printf("%s\n", nameUtf8);
      }
      exit(0);
    #endif // EXPORT_ITEMS

    gtk_init(&argc, &argv);
    setupUI();

    // Without the following 8 lines of code
    // the f- and g-shifted labels are
    // miss aligned! I dont know why!
    calcModeAimGui();
    while(gtk_events_pending()) {
      gtk_main_iteration();
    }
    calcModeNormalGui();
    while(gtk_events_pending()) {
      gtk_main_iteration();
    }

    restoreCalc();

    //ramDump();
    refreshScreen();

    gdk_threads_add_timeout(SCREEN_REFRESH_PERIOD, refreshLcd, NULL); // refreshLcd is called every SCREEN_REFRESH_PERIOD ms
    fnTimerReset();                                                    //dr timeouts for kb handling
    fnTimerConfig(TO_FG_LONG, refreshFn, TO_FG_LONG/*, 580*/);
    fnTimerConfig(TO_CL_LONG, refreshFn, TO_CL_LONG/*, 800*/);
    fnTimerConfig(TO_FG_TIMR, refreshFn, TO_FG_TIMR/*, 4000*/);
    fnTimerConfig(TO_FN_LONG, refreshFn, TO_FN_LONG/*, 400*/);
    fnTimerConfig(TO_FN_EXEC, execFnTimeout, 0/*, 150*/);
    fnTimerConfig(TO_3S_CTFF, shiftCutoff, TO_3S_CTFF/*, 600*/);
    fnTimerConfig(TO_CL_DROP, fnTimerDummyTest, TO_CL_DROP/*, 500*/);
  //fnTimerConfig(TO_AUTO_REPEAT, execAutoRepeat, 0/*, 200*/);          //dr no autorepeat for emulator
    fnTimerConfig(TO_TIMER_APP, execTimerApp, 0/*, 100*/);
    fnTimerConfig(TO_ASM_ACTIVE, refreshFn, TO_ASM_ACTIVE);
  //fnTimerConfig(TO_KB_ACTV, fnTimerDummyTest, TO_KB_ACTV/*, 6000*/);  //dr no keyboard scan boost for emulator
    gdk_threads_add_timeout(5, refreshTimer, NULL);                     //dr refreshTimer is called every 5 ms    //^^

//  fnTimerReset();
//  fnTimerConfig(TO_KB_ACTV, fnTimerDummyTest, TO_KB_ACTV);
//  fnTimerConfig(TO_TIMER_APP, execTimerApp, 0);
//--fnTimerConfig(TO_SHOW_NOP, execNOPTimeout, TO_SHOW_NOP);
//  gdk_threads_add_timeout(5, refreshTimer, NULL);

    if(getSystemFlag(FLAG_AUTXEQ)) {
      clearSystemFlag(FLAG_AUTXEQ);
      if(programRunStop != PGM_RUNNING) {
        screenUpdatingMode = SCRUPD_AUTO;
        runFunction(ITM_RS);
      }
      refreshScreen();
    }

    gtk_main();

      return 0;
    }
#endif // PC_BUILD
