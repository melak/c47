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

#include "flags.h"
#include "gui.h"
#include "items.h"
#include "keyboard.h"
#include "longIntegerType.h"
#include "memory.h"
#include "saveRestoreCalcState.h"
#include "screen.h"
#include "timer.h"
#include <stdio.h>

#include "wp43s.h"

#ifdef PC_BUILD
  #ifndef RPIWSMD
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
    calcKeyboard_t       calcKeyboard[43];
    int                  currentBezel; // 0=normal, 1=AIM, 2=TAM

    #ifdef EXPORT_ITEMS
      int sortItems(void const *a, void const *b) {
        return compareString(a, b, CMP_EXTENSIVE);
      }
    #endif

    int main(int argc, char* argv[]) {
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

      #ifdef EXPORT_ITEMS
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
        printf("be displayed with the WP43S_StandardFont!\n");
        for(int i=0; i<nbrItems; i++) {
          stringToUtf8(name[i], (uint8_t *)nameUtf8);
          printf("%s\n", nameUtf8);
        }
        exit(0);
      #endif

      gtk_init(&argc, &argv);
      setupUI();

      restoreCalc();
      //ramDump();
      //refreshScreen();

      gdk_threads_add_timeout(SCREEN_REFRESH_PERIOD, refreshLcd, NULL); // refreshLcd is called every SCREEN_REFRESH_PERIOD ms

      fnTimerReset();
      fnTimerConfig(TO_TIMER_APP, execTimerApp, 0);
      //--fnTimerConfig(TO_SHOW_NOP, execNOPTimeout, TO_SHOW_NOP);
      gdk_threads_add_timeout(5, refreshTimer, NULL);

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
  #endif // !RPIWSMD

  #ifdef RPIWSMD
    #include <termios.h>
    #include <unistd.h>

    bool_t               backToConsole;
    //char                debugString[10000];
    //int                 currentBezel; // 0=normal, 1=AIM, 2=TAM

    void enableRawMode(int minNbChar) {
      struct termios term;
      tcgetattr(STDIN_FILENO, &term);
      term.c_lflag &= ~(ICANON | ECHO);
      term.c_cc[VMIN]  = minNbChar;
      tcsetattr(STDIN_FILENO, TCSANOW, &term);
    }

    void disableRawMode(void) {
      struct termios term;
      tcgetattr(STDIN_FILENO, &term);
      term.c_lflag |= ICANON | ECHO;
      tcsetattr(STDIN_FILENO, TCSANOW, &term);
    }

    int main(void) {
      unsigned char c;

      wp43sMemInBlocks = 0;
      gmpMemInBytes = 0;
      mp_set_memory_functions(allocGmp, reallocGmp, freeGmp);

      if(strcmp(indexOfItems[LAST_ITEM].itemSoftmenuName, "Last item") != 0) {
        printf("The last item of indexOfItems[] is not \"Last item\"\n");
        exit(1);
      }

      restoreCalc();

      gdk_threads_add_timeout(SCREEN_REFRESH_PERIOD, refreshLcd, NULL); // refreshLcd is called every SCREEN_REFRESH_PERIOD ms

      fnTimerReset();
      fnTimerConfig(TO_TIMER_APP, execTimerApp, 0);
      gdk_threads_add_timeout(5, refreshTimer, NULL);

      if(getSystemFlag(FLAG_AUTXEQ)) {
        clearSystemFlag(FLAG_AUTXEQ);
        if(programRunStop != PGM_RUNNING) {
          screenUpdatingMode = SCRUPD_AUTO;
          runFunction(ITM_RS);
        }
        refreshScreen();
      }

      backToConsole = false;
      while(!backToConsole) {
        enableRawMode(1);
        c = getchar(); // 1st char
        //printf("getchar() done: %02x\n", c);
        enableRawMode(0);
        switch(c) {
          case 'c':  btnClicked(NULL, "14");          break; // +/-
          case 'C':  btnClicked(NULL, "09");          break; // CC
          case 'e':  btnClicked(NULL, "04");          break; // e^x
          case 'E':  btnClicked(NULL, "15");          break; // E
          case 'f':
          case 'F':  btnClicked(NULL, "10");          break; // f
          case 'g':
          case 'G':  btnClicked(NULL, "11");          break; // g
          case 'h':
          case 'H':  copyScreenToClipboard();         break;
          case 'i':
          case 'I':  btnClicked(NULL, "00");          break; // 1/x
          case 'l':  btnClicked(NULL, "03");          break; // ln
          case 'L':  btnClicked(NULL, "01");          break; // EXP
          case 'q':
          case 'Q':  btnClicked(NULL, "05");          break; // √x
          case 'r':
          case 'R':  btnClicked(NULL, "07");          break; // RCL
          case 's':
          case 'S':  btnClicked(NULL, "06");          break; // STO
          case 't':
          case 'T':  btnClicked(NULL, "02");          break; // TRI
          case 'x':  copyRegisterXToClipboard();      break;
          case 'X':  btnClicked(NULL, "21");          break; // XEQ
          case 'z':  copyStackRegistersToClipboard(); break;
          case 'Z':  copyAllRegistersToClipboard();   break;
          case '-':  btnClicked(NULL, "27");          break; // -
          case '+':  btnClicked(NULL, "32");          break; // +
          case '*':  btnClicked(NULL, "22");          break; // *
          case '/':  btnClicked(NULL, "17");          break; // /
          case '0':  btnClicked(NULL, "33");          break; // 0
          case '1':  btnClicked(NULL, "28");          break; // 1
          case '2':  btnClicked(NULL, "29");          break; // 2
          case '3':  btnClicked(NULL, "30");          break; // 3
          case '4':  btnClicked(NULL, "23");          break; // 4
          case '5':  btnClicked(NULL, "24");          break; // 5
          case '6':  btnClicked(NULL, "25");          break; // 6
          case '7':  btnClicked(NULL, "18");          break; // 7
          case '8':  btnClicked(NULL, "19");          break; // 8
          case '9':  btnClicked(NULL, "20");          break; // 9
          case '.':
          case ',':  btnClicked(NULL, "34");          break; // .
          case 0x09: btnClicked(NULL, "13");          break; // x⇄y
          case 0x0a: btnClicked(NULL, "12");          break; // ENTER↑
          case 0x12: btnClicked(NULL, "35");          break; // R/S
          case 0x7f: btnClicked(NULL, "16");          break; // Bksp
          case 0x1b: // Escape
            c = getchar(); // 1b + 2nd char
            switch(c) {
              case 0xff: btnClicked(NULL, "36");
                         ungetc(c, stdin); c = getchar(); // I dont know why this line is necessary!?
                         break;
              case 0x5b:
                c = getchar(); // 1b 5b + 3rd char
                switch(c) {
                  case 0x5b:
                    c = getchar(); // 1b 5b 5b + 4th char
                    switch(c) {
                      case 0x41: btnFnClicked(NULL, "1"); break; // F1
                      case 0x42: btnFnClicked(NULL, "2"); break; // F2
                      case 0x43: btnFnClicked(NULL, "3"); break; // F3
                      case 0x44: btnFnClicked(NULL, "4"); break; // F4
                      case 0x45: btnFnClicked(NULL, "5"); break; // F5
                      default:   printf("1b 5b 5b + %02x ???\n", c);
                    }
                    break;
                  case 0x31:
                    c = getchar(); // 1b 5b 31 + 4th char
                    switch(c) {
                      case 0x37: if((c = getchar()) == 0x7e) btnFnClicked(NULL, "6"); else printf("1b 5b 31 37 + %02x ???\n", c); break; // F6
                      case 0x38: if((c = getchar()) == 0x7e) printf("F7\n");          else printf("1b 5b 31 38 + %02x ???\n", c); break;
                      case 0x39: if((c = getchar()) == 0x7e) printf("F8\n");          else printf("1b 5b 31 39 + %02x ???\n", c); break;
                      case 0x7e: printf("Home\n"); break;
                      default:   printf("1b 5b 31 + %02x ???\n", c);
                    }
                    break;
                  case 0x32:
                    c = getchar(); // 1b 5b 32 4th char
                    switch(c) {
                      case 0x30: if((c = getchar()) == 0x7e) printf("F9\n");  else printf("1b 5b 32 30 + %02x ???\n", c); break;
                      case 0x31: if((c = getchar()) == 0x7e) printf("F10\n"); else printf("1b 5b 32 31 + %02x ???\n", c); break;
                      case 0x33: if((c = getchar()) == 0x7e) printf("F11\n"); else printf("1b 5b 32 33 + %02x ???\n", c); break;
                      case 0x34: if((c = getchar()) == 0x7e) printf("F12\n"); else printf("1b 5b 32 34 + %02x ???\n", c); break;
                      case 0x7e: printf("Insert\n"); break;
                      default: printf("1b 5b 32 + %02x ???\n", c);
                    }
                    break;
                  case 0x33: if((c = getchar()) == 0x7e) printf("Delete\n");     else printf("1b 5b 33 + %02x ???\n", c); break;
                  case 0x34: if((c = getchar()) == 0x7e) printf("End\n");        else printf("1b 5b 34 + %02x ???\n", c); break;
                  case 0x35: if((c = getchar()) == 0x7e) printf("Pg Up\n");      else printf("1b 5b 35 + %02x ???\n", c); break;
                  case 0x36: if((c = getchar()) == 0x7e) btnClicked(NULL, "08"); else printf("1b 5b 36 + %02x ???\n", c); break; // Pg Down = R↓
                  case 0x41: btnClicked(NULL, "26");       break; // ↑
                  case 0x42: btnClicked(NULL, "31");       break; // ↓
                  case 0x43: printf("Right\n");            break;
                  case 0x44: printf("Left\n");             break;
                  case 0x47: printf("Center arrow pad\n"); break;
                  default:   printf("1b 5b + %02x ???\n", c);
                }
                break;
              default: printf("1b + %02x ???\n", c);
            }
            break;
          default: printf("????? %02x\n", c);
        }
      }

      disableRawMode();

      return 0;
    }
  #endif // RPIWSMD
#endif // PC_BUILD
