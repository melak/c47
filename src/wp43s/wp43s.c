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
 * \file wp43s.c
 ***********************************************/

#include "wp43s.h"

#if defined(PC_BUILD) || defined(TESTSUITE_BUILD)
  bool_t             debugMemAllocation;
#endif // PC_BUILD || TESTSUITE_BUILD
#ifdef PC_BUILD
  bool_t             calcLandscape;
  bool_t             calcAutoLandscapePortrait;
  GtkWidget          *screen;
  GtkWidget          *frmCalc;
  int16_t            screenStride;
  int16_t            debugWindow;
  uint32_t           *screenData;
  bool_t             screenChange;
  char               debugString[10000];
  #if (DEBUG_REGISTER_L == 1)
    GtkWidget        *lblRegisterL1;
    GtkWidget        *lblRegisterL2;
  #endif // (DEBUG_REGISTER_L == 1)
  #if (SHOW_MEMORY_STATUS == 1)
    GtkWidget        *lblMemoryStatus;
  #endif // (SHOW_MEMORY_STATUS == 1)
#endif // PC_BUILD

const font_t         *fontForShortInteger;
const font_t         *cursorFont;
const char            digits[17] = "0123456789ABCDEF";
real51_t              const *gammaLanczosCoefficients;
real39_t              const *angle180;
real39_t              const *angle90;
real39_t              const *angle45;
void                  (*confirmedFunction)(uint16_t);

// Variables stored in RAM
bool_t                funcOK;
bool_t                keyActionProcessed;
bool_t                hourGlassIconEnabled;
bool_t                watchIconEnabled;
bool_t                printerIconEnabled;
bool_t                shiftF;
bool_t                shiftG;
bool_t                showContent;
bool_t                rbr1stDigit;
bool_t                updateDisplayValueX;
bool_t                thereIsSomethingToUndo;
bool_t                lastProgramListEnd;
bool_t                programListEnd;
bool_t                serialIOIconEnabled;
bool_t                neverUsed;

realContext_t         ctxtReal34;   //   34 digits
realContext_t         ctxtReal39;   //   39 digits: used for 34 digits intermediate calculations
realContext_t         ctxtReal51;   //   51 digits: used for 34 digits intermediate calculations
realContext_t         ctxtReal75;   //   75 digits: used in SLVQ
realContext_t         ctxtReal1071; // 1071 digits: used in radian angle reduction
//realContext_t         ctxtReal2139; // 2139 digits: used for really big modulo

registerHeader_t      globalRegister[NUMBER_OF_GLOBAL_REGISTERS];
registerHeader_t      savedStackRegister[NUMBER_OF_SAVED_STACK_REGISTERS + 1]; // +1 for the temporary register
registerHeader_t     *currentLocalRegisters;

dataBlock_t           allSubroutineLevels;
dataBlock_t          *allNamedVariablePointer;
dataBlock_t          *statisticalSumsPointer;
dataBlock_t          *savedStatisticalSumsPointer;
dataBlock_t          *ram = NULL;
dataBlock_t          *currentLocalFlags;
dataBlock_t          *currentSubroutineLevelData;

softmenuStack_t       softmenuStack[SOFTMENU_STACK_SIZE];
calcKey_t             kbd_usr[37];
calcRegister_t        errorMessageRegisterLine;
glyph_t               glyphNotFound = {.charCode = 0x0000, .colsBeforeGlyph = 0, .colsGlyph = 13, .colsAfterGlyph = 0, .rowsGlyph = 19, .data = NULL};
freeMemoryRegion_t    freeMemoryRegions[MAX_FREE_REGION];
pcg32_random_t        pcg32_global = PCG32_INITIALIZER;
labelList_t          *labelList = NULL;
programList_t        *programList = NULL;

char                 *tmpString = NULL;
char                 *errorMessage;
char                 *aimBuffer; // aimBuffer is also used for NIM
char                 *nimBufferDisplay;
char                 *tamBuffer;
char                  asmBuffer[5];
char                  oldTime[8];
char                  dateTimeString[12];
char                  displayValueX[DISPLAY_VALUE_LEN];

uint8_t               transitionSystemState;
uint8_t               numScreensStandardFont;
uint8_t               currentFntScr;
uint8_t               currentFlgScr;
uint8_t               displayFormat;
uint8_t               displayFormatDigits;
uint8_t               shortIntegerWordSize;
uint8_t               significantDigits;
uint8_t               shortIntegerMode;
uint8_t               previousCalcMode;
uint8_t               groupingGap;
uint8_t               roundingMode;
uint8_t               calcMode;
uint8_t               nextChar;
uint8_t               displayStack;
uint8_t               alphaCase;
uint8_t               numLinesNumericFont;
uint8_t               numLinesStandardFont;
uint8_t               cursorEnabled;
uint8_t               nimNumberPart;
uint8_t               hexDigits;
uint8_t               lastErrorCode;
uint8_t               temporaryInformation;
uint8_t               rbrMode;
uint8_t               numScreensNumericFont;
uint8_t               currentAngularMode;
uint8_t              *beginOfProgramMemory;
uint8_t              *beginOfCurrentProgram;
uint8_t              *endOfCurrentProgram;
uint8_t              *firstFreeProgramByte;
uint8_t              *firstDisplayedStep;
uint8_t              *currentStep;

int16_t               tamFunction;
int16_t               tamNumber;
int16_t               tamNumberMin;
int16_t               tamNumberMax;
int16_t               tamDigit;
int16_t               tamOperation;
int16_t               tamLetteredRegister;
int16_t               tamCurrentOperation;
int16_t               currentRegisterBrowserScreen;
int16_t               lineTWidth;
int16_t               rbrRegister;
int16_t               catalog;
int16_t               lastCatalogPosition[NUMBER_OF_CATALOGS];
int16_t               showFunctionNameItem;
int16_t               exponentSignLocation;
int16_t               denominatorLocation;
int16_t               imaginaryExponentSignLocation;
int16_t               imaginaryMantissaSignLocation;
int16_t               exponentLimit;
int16_t               showFunctionNameCounter;
int16_t               dynamicMenuItem;
int16_t              *menu_RAM;
int16_t               numberOfTamMenusToPop;

uint16_t              globalFlags[7];
uint16_t              freeProgramBytes;
uint16_t              glyphRow[NUMBER_OF_GLYPH_ROWS];
uint16_t              firstDisplayedLocalStepNumber;
uint16_t              numberOfLabels;
uint16_t              numberOfPrograms;
uint16_t              tamMode;
uint16_t              currentLocalStepNumber;
uint16_t              currentProgramNumber;

int32_t               numberOfFreeMemoryRegions;
int32_t               lgCatalogSelection;

uint32_t              firstGregorianDay;
uint32_t              denMax;
uint32_t              lastIntegerBase;
uint32_t              alphaSelectionTimer;
uint32_t              xCursor;
uint32_t              yCursor;
uint32_t              tamOverPemYPos;

uint64_t              shortIntegerMask;
uint64_t              shortIntegerSignBit;
uint64_t              systemFlags;
uint64_t              savedSystemFlags;

size_t                gmpMemInBytes;
size_t                wp43sMemInBytes;

#ifdef DMCP_BUILD
  bool_t              backToDMCP;
  int                 keyAutoRepeat;
  int16_t             previousItem;
  uint32_t            nextScreenRefresh; // timer substitute for refreshLcd(), which does cursor blinking and other stuff
#endif // DMCP_BUILD


#ifdef PC_BUILD
  int main(int argc, char* argv[]) {
    #ifdef CODEBLOCKS_OVER_SCORE // Since December 27th 2020 when running in code::blocks, we are no more in the correct directory! Why?
      (*strstr(argv[0], "/bin/")) = 0;
      chdir(argv[0]);
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

    wp43sMemInBytes = 0;
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
ramDump();
    refreshScreen();

    gdk_threads_add_timeout(SCREEN_REFRESH_PERIOD, refreshLcd, NULL); // refreshLcd is called every SCREEN_REFRESH_PERIOD ms

    gtk_main();

    return 0;
  }
#endif // PC_BUILD

#ifdef DMCP_BUILD
  void program_main(void) {
    int key = 0;
    char charKey[3];
    bool_t wp43sKbdLayout;
    uint16_t currentVolumeSetting, savedVoluleSetting; // used for beep signaling screen shot

    wp43sMemInBytes = 0;
    gmpMemInBytes = 0;
    mp_set_memory_functions(allocGmp, reallocGmp, freeGmp);

    lcd_clear_buf();
    lcd_putsAt(t24, 4, "Press the bottom left key.");
    lcd_refresh();
    while(key != 33 && key != 37) {
      key = key_pop();
      while(key == -1) {
        sys_sleep();
        key = key_pop();
      }
    }

    wp43sKbdLayout = (key == 37); // bottom left key
    key = 0;

    lcd_clear_buf();
    fnReset(CONFIRMED);
    refreshScreen();

    #if 0
      longInteger_t li;
      uint32_t addr, min, max, *ptr;

      min = 1;
      max = 100000000;
      while(min+1 < max) {
        ptr = malloc((max + min) >> 1);
        if(ptr) {
          free(ptr);
          min = (max + min) >> 1;
        }
        else {
          max = (max + min) >> 1;
        }
      }

      ptr = malloc(min);
      xcopy(&addr, &ptr, 4);
      free(ptr);
      longIntegerInit(li);
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 16, 50);

      uIntToLongInteger(min, li);
      convertLongIntegerToShortIntegerRegister(li, 10, 51);

      ptr = (uint32_t *)qspi_user_addr();
      xcopy(&addr, &ptr, 4);
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 16, 52);

      addr = (uint32_t)qspi_user_size(); // QSPI user size in bytes
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 10, 53);

      ptr = (uint32_t *)&ram;
      xcopy(&addr, &ptr, 4);
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 16, 54);

      ptr = (uint32_t *)&indexOfItems;
      xcopy(&addr, &ptr, 4);
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 16, 55);

      ptr = (uint32_t *)ppgm_fp;
      xcopy(&addr, &ptr, 4);
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 16, 56);

      ptr = (uint32_t *)get_reset_state_file();
      xcopy(&addr, &ptr, 4);
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 16, 57);

      addr = 0x38; // RESET_STATE_FILE_SIZE;
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 10, 58);

      ptr = (uint32_t *)aux_buf_ptr();
      xcopy(&addr, &ptr, 4);
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 16, 59);

      addr = AUX_BUF_SIZE;
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 10, 60);

      ptr = (uint32_t *)write_buf_ptr();
      xcopy(&addr, &ptr, 4);
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 16, 61);

      addr = (uint32_t)write_buf_size();
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 10, 62);

      addr = (uint32_t)get_hw_id();
      uIntToLongInteger(addr, li);
      convertLongIntegerToShortIntegerRegister(li, 10, 63);

      longIntegerFree(li);
    #endif // 1

    backToDMCP = false;

    lcd_refresh();
    nextScreenRefresh = sys_current_ms() + SCREEN_REFRESH_PERIOD;
    //runner_key_tout_init(0); // Enables fast auto repeat

    // Status flags:
    //   ST(STAT_PGM_END)   - Indicates that program should go to off state (set by auto off timer)
    //   ST(STAT_SUSPENDED) - Program signals it is ready for off and doesn't need to be woken-up again
    //   ST(STAT_OFF)       - Program in off state (OS goes to sleep and only [EXIT] key can wake it up again)
    //   ST(STAT_RUNNING)   - OS doesn't sleep in this mode
    while(!backToDMCP) {
      if(ST(STAT_PGM_END) && ST(STAT_SUSPENDED)) { // Already in off mode and suspended
        CLR_ST(STAT_RUNNING);
        sys_sleep();
      }
      else if((!ST(STAT_PGM_END) && key_empty())) {         // Just wait if no keys available.
        CLR_ST(STAT_RUNNING);
        sys_timer_start(TIMER_IDX_SCREEN_REFRESH, max(1, nextScreenRefresh - sys_current_ms()));  // wake up for screen refresh
        sys_sleep();
        sys_timer_disable(TIMER_IDX_SCREEN_REFRESH);
      }

      // Wakeup in off state or going to sleep
      if(ST(STAT_PGM_END) || ST(STAT_SUSPENDED)) {
        if(!ST(STAT_SUSPENDED)) {
          // Going to off mode
          lcd_set_buf_cleared(0); // Mark no buffer change region
          draw_power_off_image(1);

          LCD_power_off(0);
          SET_ST(STAT_SUSPENDED);
          SET_ST(STAT_OFF);
        }
        // Already in OFF -> just continue to sleep above
        continue;
      }

      // Well, we are woken-up
      SET_ST(STAT_RUNNING);

      // Clear suspended state, because now we are definitely reached the active state
      CLR_ST(STAT_SUSPENDED);

      // Get up from OFF state
      if(ST(STAT_OFF)) {
        LCD_power_on();
        rtc_wakeup_delay(); // Ensure that RTC readings after power off will be OK

        CLR_ST(STAT_OFF);

        if(!lcd_get_buf_cleared()) {
          lcd_forced_refresh(); // Just redraw from LCD buffer
        }
      }

      // Key is ready -> clear auto off timer
      if(!key_empty()) {
        reset_auto_off();
      }

      if(wp43sKbdLayout) {
        /////////////////////////////////////////////////
        // For key reassignment see:
        // https://technical.swissmicros.com/dm42/devel/dmcp_devel_manual/#_system_key_table
        //
        // Output of keymap2layout keymap.txt
        //
        //    +-----+-----+-----+-----+-----+-----+
        // 1: | F1  | F2  | F3  | F4  | F5  | F6  |
        //    |38:38|39:39|40:40|41:41|42:42|43:43|
        //    +-----+-----+-----+-----+-----+-----+
        // 2: | 1/x |Sum+ | SIN | LN  | LOG |SQRT |
        //    | 1: 2| 2: 1| 3:10| 4: 5| 5: 4| 6: 3|
        //    +-----+-----+-----+-----+-----+-----+
        // 3: | STO | RCL | RDN | COS | TAN |SHIFT|
        //    | 7: 7| 8: 8| 9: 9|10:11|11:12|12:28|
        //    +-----+-----+-----+-----+-----+-----+
        // 4: |   ENTER   |x<>y | CHS |  E  | <-- |
        //    |   13:13   |14:14|15:15|16:16|17:17|
        //    +-----------+-----+-----+-----+-----+
        // 5: |  DIV |   7  |   8  |   9  |  XEQ  |
        //    | 18:22| 19:19| 20:20| 21:21| 22: 6 |
        //    +------+------+------+------+-------+
        // 6: |  MUL |   4  |   5  |   6  |  UP   |
        //    | 23:27| 24:24| 25:25| 26:26| 27:18 |
        //    +------+------+------+------+-------+
        // 7: |  SUB |   1  |   2  |   3  | DOWN  |
        //    | 28:32| 29:29| 30:30| 31:31| 32:23 |
        //    +------+------+------+------+-------+
        // 8: |  ADD |   0  |  DOT |  RUN | EXIT  |
        //    | 33:37| 34:34| 35:35| 36:36| 37:33 |
        //    +------+------+------+------+-------+
        //
        // Fetch the key
        //  < 0 -> No key event
        //  > 0 -> Key pressed
        // == 0 -> Key released
        //key = key_pop();
        key = runner_get_key_delay(&keyAutoRepeat, 10, 50, 50, 100); // TODO: make the autorepeat faster
        //key = runner_get_key(&keyAutoRepeat);

        //The switch instruction below is implemented as follows e.g. for the up arrow key on the WP43S layout:
        //  the output of keymap2layout for this key is UP 27:18 so we need the line:
        //    case 18: key = 27; break;
        switch(key) {               // Original
          case  1: key =  2; break; // SUM+
          case  2: key =  1; break; // 1/x
          case  3: key =  6; break; // SQRT
          case  4: key =  5; break; // LOG
          case  5: key =  4; break; // LN
          case  6: key = 22; break; // XEQ
        //case  7: key =  7; break; // STO
        //case  8: key =  8; break; // RCL
        //case  9: key =  9; break; // RDN
          case 10: key =  3; break; // SIN
          case 11: key = 10; break; // COS
          case 12: key = 11; break; // TAN
        //case 13: key = 13; break; // ENTER
        //case 14: key = 14; break; // x<>y
        //case 15: key = 15; break; // +/-
        //case 16: key = 16; break; // E
        //case 17: key = 17; break; // <--
          case 18: key = 27; break; // UP
        //case 19: key = 19; break; // 7
        //case 20: key = 20; break; // 8
        //case 21: key = 21; break; // 9
          case 22: key = 18; break; // /
          case 23: key = 32; break; // DOWN
        //case 24: key = 24; break; // 4
        //case 25: key = 25; break; // 5
        //case 26: key = 26; break; // 6
          case 27: key = 23; break; // x
          case 28: key = 12; break; // SHIFT
        //case 29: key = 29; break; // 1
        //case 30: key = 30; break; // 2
        //case 31: key = 31; break; // 3
          case 32: key = 28; break; // -
          case 33: key = 37; break; // EXIT
        //case 34: key = 34; break; // 0
        //case 35: key = 35; break; // .
        //case 36: key = 36; break; // R/S
          case 37: key = 33; break; // +
          default: {}
        }

        //The 3 lines below to see in the top left screen corner the pressed keycode
        //char sysLastKeyCh[5];
        //sprintf(sysLastKeyCh, "c%02d", key);
        //showString(sysLastKeyCh, &standardFont, 0, 0, vmReverse, true, true);

        //The line below to emit a beep
        //while(get_beep_volume() < 11) beep_volume_up(); start_buzzer_freq(220000); sys_delay(200); stop_buzzer();
      }
      else {
        // Fetch the key
        //  < 0 -> No key event
        //  > 0 -> Key pressed
        // == 0 -> Key released
        //key = key_pop();
        key = runner_get_key_delay(&keyAutoRepeat, 10, 50, 50, 100); // TODO: make the autorepeat faster
        //key = runner_get_key(&keyAutoRepeat);

        //The 3 lines below to see in the top left screen corner the pressed keycode
        //char sysLastKeyCh[5];
        //sprintf(sysLastKeyCh, " %02d", key);
        //showString(sysLastKeyCh, &standardFont, 0, 0, vmReverse, true, true);
      }

      if(keyAutoRepeat) {
        if(key == 27 || key == 32) { // UP or DOWN keys
          //beep(2200, 50);
          key = 0; // to trigger btnReleased
        }
        else {
          key = -1;
        }
      }

      if(key == 44) { //DISP for special SCREEN DUMP key code. To be 16 but shift decoding already done to 44 in DMCP
        shiftF = false;
        shiftG = false; //To avoid f or g top left of the screen, clear again to make sure

        currentVolumeSetting = get_beep_volume();
        savedVoluleSetting = currentVolumeSetting;
        while(currentVolumeSetting < 11) {
          beep_volume_up();
          currentVolumeSetting = get_beep_volume();
        }

        start_buzzer_freq(100000); //Click before screen dump
        sys_delay(5);
        stop_buzzer();

        create_screenshot(0);      //Screen dump

        start_buzzer_freq(400000); //Click after screen dump
        sys_delay(5);
        stop_buzzer();

        while(currentVolumeSetting != savedVoluleSetting) { //Restore volume
          beep_volume_down();
          currentVolumeSetting = get_beep_volume();
        }
      }

      if(38 <= key && key <=43) { // Function key
        sprintf(charKey, "%c", key+11);
        btnFnPressed(charKey);
        lcd_refresh();
      }
      else if(1 <= key && key <= 37) { // Not a function key
        sprintf(charKey, "%02d", key - 1);
        btnPressed(charKey);
        lcd_refresh();
      }
      else if(key == 0) { // Autorepeat
        if(charKey[1] == 0) { // Last key pressed was one of the 6 function keys
          btnFnReleased(charKey);
        }
        else { // Last key pressed was not one of the 6 function keys
          //beep(440, 50);
          btnReleased(charKey);
        }
        keyAutoRepeat = 0;
        lcd_refresh();
      }

      uint32_t now = sys_current_ms();
      if(nextScreenRefresh <= now) {
        nextScreenRefresh = now + SCREEN_REFRESH_PERIOD;
        refreshLcd();
        lcd_refresh();
      }
    }
  }
#endif // DMCP_BUILD

#ifdef TESTSUITE_BUILD
  #include "testSuite.h"

  int main(int argc, char* argv[]) {
    #ifdef CODEBLOCKS_OVER_SCORE // Since December 27th 2020 when running in code::blocks, we are no more in the correct directory! Why?
      (*strstr(argv[0], "/bin/")) = 0;
      chdir(argv[0]);
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

    wp43sMemInBytes = 0;
    gmpMemInBytes = 0;
    mp_set_memory_functions(allocGmp, reallocGmp, freeGmp);

    fnReset(CONFIRMED);

    /*
    longInteger_t li;
    longIntegerInit(li);
    uIntToLongInteger(1, li);
    convertLongIntegerToLongIntegerRegister(li, REGISTER_Z);
    uIntToLongInteger(2, li);
    convertLongIntegerToLongIntegerRegister(li, REGISTER_Y);
    uIntToLongInteger(2203, li);
    convertLongIntegerToLongIntegerRegister(li, REGISTER_X);
    fnPower(NOPARAM);
    fnSwapXY(NOPARAM);
    fnSubtract(NOPARAM);
    printf("a\n");
    fnIsPrime(NOPARAM);
    printf("b\n");
    longIntegerFree(li);
    return 0;
    */


    processTests();
    printf("The memory owned by GMP should be 0 bytes. Else report a bug please!\n");
    debugMemory("End of testsuite");

    return 0;
  }
#endif // TESTSUITE_BUILD
