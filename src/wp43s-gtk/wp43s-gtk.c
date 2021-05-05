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
 * \file wp43s-gtk.c
 ***********************************************/

#include "gui.h"
#include "items.h"
#include "longIntegerType.h"
#include "memory.h"
#include "saveRestoreCalcState.h"
#include "screen.h"

#include "wp43s.h"

#ifdef PC_BUILD
  bool_t              calcLandscape;
  bool_t              calcAutoLandscapePortrait;
  GtkWidget           *screen;
  GtkWidget           *frmCalc;
  int16_t             screenStride;
  int16_t             debugWindow;
  uint32_t            *screenData;
  bool_t              screenChange;
  char                debugString[10000];
  #if (DEBUG_REGISTER_L == 1)
    GtkWidget         *lblRegisterL1;
    GtkWidget         *lblRegisterL2;
  #endif // (DEBUG_REGISTER_L == 1)
  #if (SHOW_MEMORY_STATUS == 1)
    GtkWidget         *lblMemoryStatus;
  #endif // (SHOW_MEMORY_STATUS == 1)
#endif // PC_BUILD

#ifdef PC_BUILD
  int main(int argc, char* argv[]) {
    #ifdef CODEBLOCKS_OVER_SCORE // Since December 27th 2020 when running in code::blocks, we are no longer in the correct directory! Why?
      (*strstr(argv[0], "/bin/")) = 0;
      ignore_result(chdir(argv[0]));
    #endif // CODEBLOCKS_OVER_SCORE

    #ifdef __APPLE__
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

  #ifdef __MINGW64__
    system("chcp 65001"); // Configure for UTF-8 output on the Windows console
  #endif // __MINGW64__

    wp43sMemInBlocks = 0;
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
      printf("The last item of indexOfItems[] is not \"Last item\"\n");
      exit(1);
    }

    gtk_init(&argc, &argv);
    setupUI();

    restoreCalc();
    //ramDump();
    refreshScreen();

    gdk_threads_add_timeout(SCREEN_REFRESH_PERIOD, refreshLcd, NULL); // refreshLcd is called every SCREEN_REFRESH_PERIOD ms

    gtk_main();

    return 0;
  }
#endif // PC_BUILD
