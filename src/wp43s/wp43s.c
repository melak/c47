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

#if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
  bool_t             debugMemAllocation;
#endif
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
  #endif
  #if (SHOW_MEMORY_STATUS == 1)
    GtkWidget        *lblMemoryStatus;
  #endif
#endif

dataBlock_t          *ram;
bool_t                allowScreenUpdate;
bool_t                funcOK;
bool_t                keyActionProcessed;

// Variables stored in RAM
realContext_t         ctxtReal34;   //   34 digits
realContext_t         ctxtReal39;   //   39 digits: used for 34 digits intermediate calculations
realContext_t         ctxtReal51;   //   51 digits: used for 34 digits intermediate calculations
realContext_t         ctxtReal75;   //   75 digits: used in SLVQ
realContext_t         ctxtReal1071; // 1071 digits: used in radian angle reduction
//realContext_t         ctxtReal2139; // 2139 digits: used for really big modulo
uint16_t              flags[7];
char                  tmpStr3000[TMP_STR_LENGTH];
char                  errorMessage[ERROR_MESSAGE_LENGTH];
char                  aimBuffer[AIM_BUFFER_LENGTH];
char                  nimBuffer[NIM_BUFFER_LENGTH];
char                  nimBufferDisplay[NIM_BUFFER_LENGTH];
char                  tamBuffer[TAM_BUFFER_LENGTH];
char                  oldTime[8];
char                  dateTimeString[12];
softmenuStack_t       softmenuStack[7];
registerDescriptor_t  reg[112];
registerDescriptor_t  savedStackRegister[9+1];
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
int16_t               alphaSelectionMenu;
int16_t               lastFcnsMenuPos;
int16_t               lastMenuMenuPos;
int16_t               lastCnstMenuPos;
int16_t               SHOWregis;                   //JMSHOW
uint16_t              numberOfLocalFlags;
dataBlock_t          *allLocalRegisterPointer;
dataBlock_t          *allNamedVariablePointer;
dataBlock_t          *statisticalSumsPointer;
uint16_t              programCounter;
uint16_t              xCursor;
uint16_t              yCursor;
uint16_t              tamMode;
uint32_t              firstGregorianDay;
uint32_t              denMax;
uint32_t              lastIntegerBase;
uint32_t              alphaSelectionTimer;
uint8_t               softmenuStackPointer;
uint8_t               transitionSystemState;
uint8_t               cursorBlinkCounter;
uint8_t               numScreensStandardFont;
uint8_t               currentFntScr;
uint8_t               currentFlgScr;
uint8_t               displayFormat;
uint8_t               displayFormatDigits;
uint8_t               shortIntegerWordSize;
uint8_t               denominatorMode;
uint8_t               significantDigits;
uint8_t               shortIntegerMode;
uint8_t               previousCalcMode;
uint8_t               groupingGap;
uint8_t               dateFormat;
uint8_t               curveFitting;
uint8_t               roundingMode;
uint8_t               calcMode;
uint8_t               nextChar;
uint8_t               complexUnit;
uint8_t               displayStack;
uint8_t               productSign;
uint8_t               fractionType;
uint8_t               radixMark;
uint8_t               displayModeOverride;
uint8_t               stackSize;
uint8_t               complexMode;
uint8_t               alphaCase;
uint8_t               numLinesNumericFont;
uint8_t               numLinesStandardFont;
uint8_t               cursorEnabled;
uint8_t               cursorFont;
uint8_t               nimNumberPart;
uint8_t               hexDigits;
uint8_t               lastErrorCode;
uint8_t               serialIOIconEnabled;
uint8_t               timeFormat;
uint8_t               temporaryInformation;
uint8_t               rbrMode;
uint8_t               numScreensNumericFont;
uint8_t               currentAngularMode;
bool_t                hourGlassIconEnabled;
bool_t                watchIconEnabled;
bool_t                userModeEnabled;
bool_t                printerIconEnabled;
bool_t                batteryIconEnabled;
bool_t                shiftF;
bool_t                shiftG;
bool_t                shiftStateChanged;
bool_t                showContent;
bool_t                stackLiftEnabled;
bool_t                displayLeadingZeros;
bool_t                savedStackLiftEnabled;
bool_t                rbr1stDigit;
bool_t                updateDisplayValueX;
calcKey_t             kbd_usr[37];
calcRegister_t        errorMessageRegisterLine;
calcRegister_t        errorRegisterLine;
uint16_t              row[100];
uint64_t              shortIntegerMask;
uint64_t              shortIntegerSignBit;
glyph_t               glyphNotFound = {.charCode = 0x0000, .colsBeforeGlyph = 0, .colsGlyph = 13, .colsAfterGlyph = 0, .rowsGlyph = 19};
char                  transitionSystemOperation[4];
char                  displayValueX[DISPLAY_VALUE_LEN];
int16_t               exponentSignLocation;
int16_t               denominatorLocation;
int16_t               imaginaryExponentSignLocation;
int16_t               imaginaryMantissaSignLocation;
int16_t               exponentLimit;
size_t                gmpMemInBytes;
size_t                wp43sMemInBytes;
freeBlock_t           freeBlocks[MAX_FREE_BLOCKS];
int32_t               numberOfFreeBlocks;
int32_t               lgCatalogSelection;
void                  (*confirmedFunction)(uint16_t);
real51_t              const *gammaLanczosCoefficients;
real39_t              const *angle180;
real39_t              const *angle90;
real39_t              const *angle45;
pcg32_random_t        pcg32_global = PCG32_INITIALIZER;
#ifdef DMCP_BUILD
  bool_t              backToDMCP;
  uint32_t            nextScreenRefresh; // timer substitute for refreshScreen(), which does cursor blinking and other stuff
  #define TIMER_IDX_SCREEN_REFRESH 0     // use timer 0 to wake up for screen refresh
#endif // DMCP_BUILD


/********************************************//**
 * \brief Sets all the default values of the calc and refreshes the stack
 *
 * \param void
 * \return void
 ***********************************************/
void setupDefaults(void) {
  void *memPtr;

  ram = (dataBlock_t *)malloc(TO_BYTES(RAM_SIZE));
  memset(ram, 0, TO_BYTES(RAM_SIZE));
  numberOfFreeBlocks = 1;
  freeBlocks[0].address = 0;
  freeBlocks[0].sizeInBlocks = RAM_SIZE;

  glyphNotFound.data   = malloc(38);
  #ifndef __APPLE__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wstringop-truncation"
  #endif
  strncpy(glyphNotFound.data, "\xff\xf8\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\xff\xf8", 38);
  #ifndef __APPLE__
    #pragma GCC diagnostic pop
  #endif

  // Initialization of user key assignments
  xcopy(kbd_usr, kbd_std, sizeof(kbd_std));
  //kbd_usr[ 0].keyLblAim   = CHR_A_GRAVE;
  //kbd_usr[ 0].fShiftedAim = CHR_A_GRAVE;
  //kbd_usr[ 4].keyLblAim   = CHR_E_ACUTE;
  //kbd_usr[ 4].fShiftedAim = CHR_E_ACUTE;
  //kbd_usr[18].fShifted    = -MNU_VARS;
  //kbd_usr[18].gShifted    = CST_54;
  //kbd_usr[19].fShifted    = ITM_SW;
  //kbd_usr[19].gShifted    = ITM_SXY;
  //kbd_usr[20].gShifted    = ITM_LYtoM;

  // initialize the 112 global registers
  for(calcRegister_t regist=0; regist<FIRST_LOCAL_REGISTER; regist++) {
    setRegisterDataType(regist, dtReal34, AM_NONE);
    memPtr = allocWp43s(TO_BYTES(REAL34_SIZE));
    setRegisterDataPointer(regist, memPtr);
    real34Zero(memPtr);
  }

  // initialize the 9+1 saved stack registers
  for(calcRegister_t regist=SAVED_REGISTER_X; regist<=LAST_SAVED_REGISTER; regist++) {
    setRegisterDataType(regist, dtReal34, AM_NONE);
    memPtr = allocWp43s(TO_BYTES(REAL34_SIZE));
    setRegisterDataPointer(regist, memPtr);
    real34Zero(memPtr);
  }

  // allocating space for the named variable list
  allNamedVariablePointer = allocWp43s(TO_BYTES(1)); //  1 block for the number of named variables
  allNamedVariablePointer->numberOfNamedVariables = 0;

  // allocate space for the local register list
  allLocalRegisterPointer = allocWp43s(TO_BYTES(1)); //  1 block for the number of local registers and the local flags
  numberOfLocalFlags = 0;
  allLocalRegisterPointer->numberOfLocalRegisters = 0;
  allLocalRegisterPointer->localFlags = 0;

  #ifdef PC_BUILD
    debugWindow = DBG_REGISTERS;
  #endif
  allowScreenUpdate   = false;

  temporaryInformation = TI_NO_INFO;

  decContextDefault(&ctxtReal34, DEC_INIT_DECQUAD);

  decContextDefault(&ctxtReal39, DEC_INIT_DECQUAD);
  ctxtReal39.digits = 39;
  ctxtReal39.traps  = 0;

  decContextDefault(&ctxtReal51, DEC_INIT_DECQUAD);
  ctxtReal51.digits = 51;
  ctxtReal51.traps  = 0;

  decContextDefault(&ctxtReal75, DEC_INIT_DECQUAD);
  ctxtReal75.digits = 75;
  ctxtReal75.traps  = 0;

  decContextDefault(&ctxtReal1071,  DEC_INIT_DECQUAD);
  ctxtReal1071.digits = 1071;
  ctxtReal1071.traps  = 0;

  //decContextDefault(&ctxtReal2139,  DEC_INIT_DECQUAD);
  //ctxtReal2139.digits = 2139;
  //ctxtReal2139.traps  = 0;

  statisticalSumsPointer = NULL;

  fnSetWordSize(64); // word size from 1 to 64
  fnIntegerMode(SIM_2COMPL);

  groupingGap = 3;

  displayFormat = DF_ALL;
  displayFormatDigits = 0;
  fnTimeFormat(TF_H24);
  fnComplexUnit(CU_I);
  fnAngularMode(AM_DEGREE);
  fnDenMode(DM_ANY);
  denMax = DM_DENMAX;
  fnCurveFitting(CF_LINEAR_FITTING);
  fnLeadingZeros(false);
  fnProductSign(PS_CROSS);
  fractionType = FT_NONE;
  fnRadixMark(RM_PERIOD);
  fnComplexResult(false);
  fnComplexMode(CM_RECTANGULAR);
  fnDisplayOvr(DO_SCI);
  fnStackSize(SS_4);
  fnDateFormat(DF_YMD);
  showFracMode();
  significantDigits = 0;
  fnRoundingMode(RM_HALF_EVEN); // DEC_ROUND_HALF_EVEN
  fnDisplayStack(4);

  showDateTime();

  shiftF = false;
  shiftG = false;
  shiftStateChanged = false;
  #ifndef TESTSUITE_BUILD
    showShiftState();
  #endif // TESTSUITE_BUILD

  currentFntScr = 0;
  currentFlgScr = 0;
  currentRegisterBrowserScreen = 9999;
  SHOWregis = 9999;                                              //JMSHOW


  softmenuStackPointer = 0;

  aimBuffer[0] = 0;


  fnClearFlag(FLAG_OVERFLOW);
  fnClearFlag(FLAG_CARRY);
  showOverflowCarry();

  fnClearFlag(FLAG_CPXRES);
  showRealComplexResult();

  showAlphaMode();

  programCounter = 0;
  showPgmBegin();

  cursorBlinkCounter = 0;

  oldTime[0] = 0;

  STACK_LIFT_ENABLE;

  lastErrorCode = 0;

  refreshStack();

  allowScreenUpdate = true;

  hideUserMode();

  gammaLanczosCoefficients = (real51_t *)const_gammaC01;

  angle180 = (real39_t *)const_180;
  angle90  = (real39_t *)const_90;
  angle45  = (real39_t *)const_45;

  alphaSelectionMenu = ASM_NONE;

  #ifndef TESTSUITE_BUILD
    resetAlphaSelectionBuffer();
  #endif

  lastFcnsMenuPos = 0;
  lastMenuMenuPos = 0;
  lastCnstMenuPos = 0;

  exponentLimit = 6145;

  #ifdef TESTSUITE_BUILD
    calcMode = CM_NORMAL;
  #else
    calcModeNormal();
  #endif // TESTSUITE_BUILD

  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    debugMemAllocation = false;
  #endif
}



#ifdef PC_BUILD
int main(int argc, char* argv[]) {
  #if defined __APPLE__
    // we take the directory where the application is as the root for this application.
    // in argv[0] is the application itself. We strip the name of the app by searching for the last '/':
    if (argc>=1) {
      char *curdir = malloc(1000);
      // find last /:
      char *s = strrchr(argv[0], '/');
      if (s != 0) {
        // take the directory before the appname:
        strncpy(curdir, argv[0], s-argv[0]);
        chdir(curdir);
        free(curdir);
      }
    }
  #endif

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

  setupDefaults();

  restoreCalc();
  //fnReset(CONFIRMED);

  gdk_threads_add_timeout(100, refreshScreen, NULL); // refreshScreen is called every 100 ms

  gtk_main();

  return 0;
}
#endif

#ifdef DMCP_BUILD
void program_main(void) {
  int key = 0;
  char charKey[3];
  bool_t wp43sKbdLayout;

  wp43sMemInBytes = 0;
  gmpMemInBytes = 0;
  //mp_set_memory_functions(allocGmp, reallocGmp, freeGmp);

  // Initialization
  //program_init();

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
  setupDefaults();

/*longInteger_t li;
longIntegerInit(li);
uint32_t addr;

addr = (uint32_t)ram;
uIntToLongInteger(addr, li);
convertLongIntegerToShortIntegerRegister(li, 16, REGISTER_T);

addr = (uint32_t)row;
uIntToLongInteger(addr, li);
convertLongIntegerToShortIntegerRegister(li, 16, REGISTER_Z);

addr = (uint32_t)(&angle180);
uIntToLongInteger(addr, li);
convertLongIntegerToShortIntegerRegister(li, 16, REGISTER_Y);

addr = (uint32_t)indexOfItems;
uIntToLongInteger(addr, li);
convertLongIntegerToShortIntegerRegister(li, 16, REGISTER_X);

refreshStack();
longIntegerFree(li);*/

  backToDMCP = false;

  lcd_refresh();
  nextScreenRefresh = sys_current_ms()+100;

  // Status flags:
  //   ST(STAT_PGM_END)   - Indicates that program should go to off state (set by auto off timer)
  //   ST(STAT_SUSPENDED) - Program signals it is ready for off and doesn't need to be woken-up again
  //   ST(STAT_OFF)       - Program in off state (OS goes to sleep and only [EXIT] key can wake it up again)
  //   ST(STAT_RUNNING)   - OS doesn't sleep in this mode
  while(!backToDMCP) {
    if(ST(STAT_PGM_END) && ST(STAT_SUSPENDED)) { // Already in off mode and suspended
      CLR_ST(STAT_RUNNING);
      sys_sleep();
    } else if ((!ST(STAT_PGM_END) && key_empty())) {         // Just wait if no keys available.
      CLR_ST(STAT_RUNNING);
      sys_timer_start(TIMER_IDX_SCREEN_REFRESH, max(1, nextScreenRefresh-sys_current_ms()));  // wake up for screen refresh
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
      // https://www.swissmicros.com/dm42/devel/dmcp_devel_manual/#_system_key_table
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
      key = key_pop();

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
    }
    else {
      // Fetch the key
      //  < 0 -> No key event
      //  > 0 -> Key pressed
      // == 0 -> Key released
      key = key_pop();
    }

    //The 3 lines below to see in the top left screen corner the pressed keycode
    //char sysLastKeyCh[5];
    //sprintf(sysLastKeyCh, "%2d", sys_last_key());
    //showString(sysLastKeyCh, &standardFont, 0, 0, vmReverse, true, true);

    if(sys_last_key() == 44 ) { //DISP for special SCREEN DUMP key code. To be 16 but shift decoding already done to 44 in DMCP
      resetShiftState(); // to avoid f or g top left of the screen
      create_screenshot(0);
    }

    if(38 <= key && key <=43) {
      sprintf(charKey, "%c", key+11);
      btnFnPressed(NULL, charKey);
      lcd_refresh();
    }
    else if(1 <= key && key <= 37) {
      sprintf(charKey, "%02d", key - 1);
      btnPressed(NULL, charKey);
      lcd_refresh();
    }
    else if(key == 0) {
      if(charKey[1] == 0) { // Last key pressed was one of the 6 function keys
        btnFnReleased(NULL, NULL);
      }
      else { // Last key pressed was not one of the 6 function keys
        btnReleased(NULL, NULL);
      }
      lcd_refresh();
    }

    uint32_t now = sys_current_ms();
    if(nextScreenRefresh <= now) {
        nextScreenRefresh += 100;
        if(nextScreenRefresh < now)
          nextScreenRefresh = now + 100; // we were out longer than expected; just skip ahead.
        refreshScreen();
        lcd_refresh();
    }
  }
}
#endif

#ifdef TESTSUITE_BUILD
#include "testSuite.h"

int main(int argc, char* argv[]) {
  #if defined __APPLE__
    // we take the directory where the application is as the root for this application.
    // in argv[0] is the application itself. We strip the name of the app by searching for the last '/':
    if (argc>=1) {
      char *curdir = malloc(1000);
      // find last /:
      char *s = strrchr(argv[0], '/');
      if (s != 0) {
        // take the directory before the appname:
        strncpy(curdir, argv[0], s-argv[0]);
        chdir(curdir);
        free(curdir);
      }
    }
  #endif

  wp43sMemInBytes = 0;
  gmpMemInBytes = 0;
  //mp_set_memory_functions(allocGmp, reallocGmp, freeGmp);

  setupDefaults();

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
  debugMemory();

  return 0;
}
#endif
