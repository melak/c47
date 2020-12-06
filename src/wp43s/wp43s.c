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

//#define JMSHOWCODES


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
bool_t                funcOK;
bool_t                keyActionProcessed;
const font_t         *fontForShortInteger;
const font_t         *cursorFont;

// Variables stored in RAM
realContext_t         ctxtReal34;   //   34 digits
realContext_t         ctxtReal39;   //   39 digits: used for 34 digits intermediate calculations
realContext_t         ctxtReal51;   //   51 digits: used for 34 digits intermediate calculations
realContext_t         ctxtReal75;   //   75 digits: used in SLVQ
realContext_t         ctxtReal1071; // 1071 digits: used in radian angle reduction
//realContext_t         ctxtReal2139; // 2139 digits: used for really big modulo
uint16_t              globalFlags[7];
char                  tmpStr3000[TMP_STR_LENGTH];
char                  errorMessage[ERROR_MESSAGE_LENGTH];
char                  aimBuffer[AIM_BUFFER_LENGTH]; // aimBuffer is also used for NIM
char                  nimBufferDisplay[NIM_BUFFER_LENGTH];
char                  tamBuffer[TAM_BUFFER_LENGTH];
char                  asmBuffer[5];
char                  oldTime[8];
char                  dateTimeString[12];
softmenuStack_t       softmenuStack[SOFTMENU_STACK_SIZE];          //JM bugfix
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
int16_t               lastSyFlMenuPos;
int16_t               lastAIntMenuPos;
int16_t               showFunctionNameItem;
int16_t               longpressDelayedkey2;          //JM
int16_t               longpressDelayedkey3;               //JM
int16_t               T_cursorPos;                 //JMCURSOR
int16_t               SHOWregis;                   //JMSHOW
int16_t               ListXYposition;              //JMSHOW
int16_t               mm_MNU_HOME;                 //JM
int16_t               mm_MNU_ALPHA;                //JM
int16_t               MY_ALPHA_MENU = MY_ALPHA_MENU_CNST;  //JM
int16_t               fnXEQMENUpos;                //JMXEQ
uint16_t              numberOfLocalFlags;
uint16_t              glyphRow[NUMBER_OF_GLYPH_ROWS];
dataBlock_t          *allLocalRegisterPointer;
dataBlock_t          *allNamedVariablePointer;
dataBlock_t          *statisticalSumsPointer;
dataBlock_t          *savedStatisticalSumsPointer;
uint16_t              programCounter;
uint16_t              xCursor;
uint16_t              yCursor;
uint16_t              tamMode;
uint32_t              firstGregorianDay;
uint32_t              denMax;
uint32_t              lastIntegerBase;
uint32_t              alphaSelectionTimer;
uint8_t               softmenuStackPointer;
uint8_t               softmenuStackPointerBeforeAIM;
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
uint8_t               displayStackSHOIDISP;          //JM SHOIDISP
bool_t                numLock;                       //JM
uint8_t               alphaCase;
uint8_t               numLinesNumericFont;
uint8_t               numLinesStandardFont;
uint8_t               cursorEnabled;
uint8_t               nimNumberPart;
uint8_t               hexDigits;
uint8_t               lastErrorCode;
uint8_t               serialIOIconEnabled;
uint8_t               temporaryInformation;
uint8_t               rbrMode;
uint8_t               numScreensNumericFont;
uint8_t               currentAngularMode;
bool_t   doRefreshSoftMenu;                                    //dr
bool_t                jm_FG_LINE;                              //JM Screen / keyboard operation setup
bool_t                jm_FG_DOTS;                              //JM Screen / keyboard operation setup
bool_t                jm_G_DOUBLETAP;                          //JM Screen / keyboard operation setup
bool_t                jm_HOME_SUM;                             //JMHOME
bool_t                jm_HOME_MIR;                             //JMHOME
bool_t                jm_HOME_FIX;                             //JMHOME
bool_t                jm_LARGELI;
uint8_t               SigFigMode;                              //JM SIGFIG
bool_t                eRPN;                                    //JM eRPN Create a flag to enable or disable eRPN. See bufferize.c
bool_t                HOME3;                                   //JM HOME Create a flag to enable or disable triple shift HOME3.
bool_t                ShiftTimoutMode;                         //JM SHIFT Create a flag to enable or disable SHIFT TIMER CANCEL.
bool_t                Home3TimerMode;                          //JM HOME Create a flag to enable or disable HOME TIMER CANCEL.
bool_t                UNITDisplay;                             //JM UNITDisplay
bool_t                SH_BASE_HOME;                            //JM BASEHOME
bool_t                SH_BASE_AHOME;                           //JM BASEHOME
int16_t               Norm_Key_00_VAR;                         //JM USER NORMAL
uint8_t               Input_Default;                           //JM Input Default
float                 graph_xmin;                              //JM Graph
float                 graph_xmax;                              //JM Graph
float                 graph_ymin;                              //JM Graph
float                 graph_ymax;                              //JM Graph
float                 graph_dx;                                //JM Graph
float                 graph_dy;                                //JM Graph
bool_t                extentx;                                 //JM Graph
bool_t                extenty;                                 //JM Graph
bool_t                jm_VECT;                                 //JM GRAPH
bool_t                jm_NVECT;                                //JM GRAPH
bool_t                jm_SCALE;                                //JM GRAPH
bool_t                Aspect_Square;                           //JM GRAPH
bool_t                PLOT_LINE;                               //JM GRAPH
bool_t                PLOT_CROSS;                              //JM GRAPH
bool_t                PLOT_BOX;                                //JM GRAPH
bool_t                PLOT_INTG;                               //JM GRAPH
bool_t                PLOT_DIFF;                               //JM GRAPH
bool_t                PLOT_RMS;                                //JM GRAPH
bool_t                PLOT_SHADE;                              //JM GRAPH
#ifdef INLINE_TEST                                             //vv dr
bool_t                testEnabled;                             //
uint16_t              testBitset;                              //
#endif                                                         //^^

bool_t                hourGlassIconEnabled;
bool_t                watchIconEnabled;
bool_t                printerIconEnabled;
bool_t                shiftF;
bool_t                shiftG;
bool_t                showContent;
bool_t                rbr1stDigit;
bool_t                updateDisplayValueX;
bool_t                thereIsSomethingToUndo;
bool_t                AlphaSelectionBufferTimerRunning;        //JM
calcKey_t             kbd_usr[37];
calcRegister_t        errorMessageRegisterLine;
uint64_t              shortIntegerMask;
uint64_t              shortIntegerSignBit;
uint64_t              systemFlags;
uint64_t              savedSystemFlags;
glyph_t               glyphNotFound = {.charCode = 0x0000, .colsBeforeGlyph = 0, .colsGlyph = 13, .colsAfterGlyph = 0, .rowsGlyph = 19};
char                  displayValueX[DISPLAY_VALUE_LEN];
int16_t               exponentSignLocation;
int16_t               denominatorLocation;
int16_t               imaginaryExponentSignLocation;
int16_t               imaginaryMantissaSignLocation;
int16_t               exponentLimit;
int16_t               showFunctionNameCounter;
size_t                gmpMemInBytes;
size_t                wp43sMemInBytes;
freeMemoryRegion_t    freeMemoryRegions[MAX_FREE_REGION];
int32_t               numberOfFreeMemoryRegions;
int32_t               lgCatalogSelection;
void                  (*confirmedFunction)(uint16_t);
real51_t              const *gammaLanczosCoefficients;
real39_t              const *angle180;
real39_t              const *angle90;
real39_t              const *angle45;
pcg32_random_t        pcg32_global = PCG32_INITIALIZER;
const char            digits[17] = "0123456789ABCDEF";
#ifdef DMCP_BUILD
  #ifdef JMSHOWCODES                                           //JM Test
    int8_t            telltale_pos;                            //JM Test
    int8_t            telltale_lastkey;                        //JM Test
  #endif                                                       //JM Test 
  uint32_t            nextTimerRefresh;                        //dr timer substitute for refreshTimer()
#ifdef BUFFER_CLICK_DETECTION
  uint32_t            timeStampKey;                                             //dr - internal keyBuffer POC
#endif
  bool_t              backToDMCP;
  uint32_t            nextScreenRefresh; // timer substitute for refreshLcd(), which does cursor blinking and other stuff
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
  numberOfFreeMemoryRegions = 1;
  freeMemoryRegions[0].address = 0;
  freeMemoryRegions[0].sizeInBlocks = RAM_SIZE;

  glyphNotFound.data   = malloc(38);
  xcopy(glyphNotFound.data, "\xff\xf8\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\xff\xf8", 38);

//JM Where commented, fnReset is over-writing the content of setupdefaults. fnReset is in config.c
         //JM vv note: Overwritten by fnReset
  // Initialization of user key assignments
  xcopy(kbd_usr, kbd_std, sizeof(kbd_std));
  //kbd_usr[ 0].keyLblAim   = ITM_A_GRAVE;
  //kbd_usr[ 0].fShiftedAim = ITM_A_GRAVE;
  //kbd_usr[ 4].keyLblAim   = ITM_E_ACUTE;
  //kbd_usr[ 4].fShiftedAim = ITM_E_ACUTE;
  //kbd_usr[18].fShifted    = -MNU_VARS;
  //kbd_usr[18].gShifted    = CST_54;
  //kbd_usr[19].fShifted    = ITM_SW;
  //kbd_usr[19].gShifted    = ITM_SXY;
  //kbd_usr[20].gShifted    = ITM_LYtoM;
         //JM ^^ note: Overwritten by fnReset

  // initialize the global registers
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
  savedStatisticalSumsPointer = NULL;

  fnSetWordSize(64); // word size from 1 to 64
  fnIntegerMode(SIM_2COMPL);

  groupingGap = 3;

  systemFlags = 0;
  displayFormat = DF_ALL;
  displayFormatDigits = 0;
  setSystemFlag(FLAG_TDM24); // time format = 24H
  clearSystemFlag(FLAG_CPXj);
  fnAngularMode(AM_DEGREE);
  setSystemFlag(FLAG_DENANY);
  denMax = MAX_DENMAX;
  clearSystemFlag(FLAG_LEAD0);
  setSystemFlag(FLAG_MULTx);
  clearSystemFlag(FLAG_FRACT);
  clearSystemFlag(FLAG_PROPFR);
  setSystemFlag(FLAG_DECIMP);
  clearSystemFlag(FLAG_CPXRES);
  clearSystemFlag(FLAG_POLAR);
  clearSystemFlag(FLAG_ALLENG);
  setSystemFlag(FLAG_AUTOFF);
  clearSystemFlag(FLAG_SSIZE8);
  clearSystemFlag(FLAG_MDY); // date format
  clearSystemFlag(FLAG_DMY); // date format
  setSystemFlag(FLAG_YMD);   // date format
  clearSystemFlag(FLAG_OVERFLOW);
  clearSystemFlag(FLAG_CARRY);
  clearSystemFlag(FLAG_USER);
  clearSystemFlag(FLAG_LOWBAT);
  clearSystemFlag(FLAG_USB);

  hourGlassIconEnabled = false;
  programCounter = 0;
  watchIconEnabled = false;
  serialIOIconEnabled = false;
  printerIconEnabled = false;
  thereIsSomethingToUndo = false;

  significantDigits = 0;
  fnRoundingMode(RM_HALF_EVEN); // DEC_ROUND_HALF_EVEN
  fnDisplayStack(4);

  shiftF = false;
  shiftG = false;

  reset_jm_defaults(false);                                       //JM

  fnXEQMENUpos = 0;                                            //JM Find fnXEQMENU in the indexOfItems array
  while(indexOfItems[fnXEQMENUpos].func != fnXEQMENU) {
     fnXEQMENUpos++;
  }
  uint16_t ix;                                                 //JM Reset XEQM
  ix = 0;
  while(ix<18) {
    indexOfItemsXEQM[+8*ix]=0;
    strcpy(indexOfItemsXEQM +8*ix, indexOfItems[fnXEQMENUpos+ix].itemSoftmenuName);
    ix++;    
  }

  mm_MNU_HOME       = mm(-MNU_HOME);                           //JM
  mm_MNU_ALPHA      = mm(-MNU_ALPHA);                          //JM
  if(SH_BASE_AHOME) MY_ALPHA_MENU = mm_MNU_ALPHA; else MY_ALPHA_MENU = MY_ALPHA_MENU_CNST;              //JM 
  T_cursorPos = 0;                                             //JMCURSOR
  longpressDelayedkey3 = 0;                                           //JM
  doRefreshSoftMenu = true;                                    //dr
  ULFL = false;                                                //JM Underline
  ULGL = false;                                                //JM Underline
  FN_state = ST_0_INIT;                                        //JM FN-DOUBLE
  FN_key_pressed = 0;                                          //JM LONGPRESS FN
  FN_key_pressed_last = 0;                                     //JM LONGPRESS FN
  FN_timeouts_in_progress = false;                             //JM LONGPRESS FN
  Shft_timeouts = false;                                       //JM SHIFT NEW
  FN_timed_out_to_RELEASE_EXEC = false;                        //JM LONGPRESS FN
  FN_timed_out_to_NOP = false;                                 //JM LONGPRESS FN
#ifdef INLINE_TEST                                             //vv dr
  testEnabled = false;                                         //dr
  testBitset = 0x0000;                                         //dr
#endif                                                         //^^
  strcpy(filename_csv,"DEFAULT.CSV");                          //JMCSV
  mem__32=0;                                                   //JMCSV

  JM_SHIFT_HOME_TIMER1 = 1;                                    //JM TIMER
  JM_ASN_MODE = 0;                                             //JM ASSIGN
  //Load_HOME();                                               //JMHOMEDEMO: NOTE REMOVE comments TO MAKE JMHOME DEMO WORK
  telltale = 0;                                                //JMGRAPH MEM
  #ifndef TESTSUITE_BUILD                                      //JM
    clear_ul();                                                //JMUL
  #endif // TESTSUITE_BUILD                                    //JM
  ListXYposition = 0;                                          //JM
  #ifndef TESTSUITE_BUILD
//    showShiftState();
  #endif // TESTSUITE_BUILD
  initFontBrowser();
  currentFlgScr = 0;
  currentRegisterBrowserScreen = 9999;

  softmenuStackPointer = 0;

  aimBuffer[0] = 0;

  setSystemFlag(FLAG_ASLIFT);

  lastErrorCode = 0;

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
  lastSyFlMenuPos = 0;
  lastAIntMenuPos = 0;

  exponentLimit = 6145;                                        //JMMAX

  #ifdef TESTSUITE_BUILD
    calcMode = CM_NORMAL;
    clearSystemFlag(FLAG_ALPHA);
  #else
    calcModeNormal();
  #endif // TESTSUITE_BUILD

  #if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
    debugMemAllocation = true;
  #endif
}



#ifdef PC_BUILD
int main(int argc, char* argv[]) {
  #if defined __APPLE__
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
  //fnReset(CONFIRMED);

  refreshScreen();

  gdk_threads_add_timeout(SCREEN_REFRESH_PERIOD, refreshLcd, NULL); // refreshLcd is called every SCREEN_REFRESH_PERIOD ms
  fnTimerReset();                                                    //dr timeouts for kb handling
  fnTimerConfig(TO_FG_LONG, refreshFn, TO_FG_LONG/*, 580*/);
  fnTimerConfig(TO_CL_LONG, refreshFn, TO_CL_LONG/*, 500*/);
  fnTimerConfig(TO_FG_TIMR, refreshFn, TO_FG_TIMR/*, 4000*/);
  fnTimerConfig(TO_FN_LONG, refreshFn, TO_FN_LONG/*, 450*/);
  fnTimerConfig(TO_FN_EXEC, execFnTimeout, 0/*, 150*/);
  fnTimerConfig(TO_3S_CTFF, shiftCutoff, TO_3S_CTFF/*, 600*/);
  fnTimerConfig(TO_CL_DROP, fnTimerDummyTest, TO_CL_DROP/*, 500*/);
//fnTimerConfig(TO_KB_ACTV, fnTimerDummyTest, TO_KB_ACTV/*, 6000*/);  //dr no keyboard scan boost for emulator
  gdk_threads_add_timeout(5, refreshTimer, NULL);                     //dr refreshTimer is called every 5 ms    //^^

  gtk_main();

  return 0;
}
#endif

#ifdef DMCP_BUILD
void program_main(void) {
  int key = 0;
  char charKey[3];
#ifdef BUFFER_KEY_COUNT
  int keyCount = 0;                                                             //dr - internal keyBuffer POC
#endif
#ifdef BUFFER_CLICK_DETECTION
  timeStampKey = (uint32_t)sys_current_ms();                                    //dr - internal keyBuffer POC
#endif
  int count_refreshes = 0;                                                      //dr clock down refresh after 1 minute of no keystroke
//bool_t wp43sKbdLayout;                                       //dr - no keymap is used
uint16_t currentVolumeSetting, savedVoluleSetting;             //used for beep signaling screen shot

  wp43sMemInBytes = 0;
  gmpMemInBytes = 0;
  mp_set_memory_functions(allocGmp, reallocGmp, freeGmp);

  lcd_clear_buf();
/*lcd_putsAt(t24, 4, "Press the bottom left key.");            //vv dr - no keymap is used
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

  lcd_clear_buf();*/                                           //^^
  setupDefaults();

  fnReset(CONFIRMED);
  refreshScreen();

  #ifdef JMSHOWCODES                                           //JM test
  telltale_lastkey = 0;                                        //JM test
  telltale_pos = 0;                                            //JM test
  #endif                                                       //JM test

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

refreshScreen();
longIntegerFree(li);*/

  backToDMCP = false;

  lcd_forced_refresh();                                        //JM 
  nextScreenRefresh = sys_current_ms() + SCREEN_REFRESH_PERIOD;
  fnTimerReset();                                              //vv dr timeouts for kb handling
  fnTimerConfig(TO_FG_LONG, refreshFn, TO_FG_LONG/*, 580*/);
  fnTimerConfig(TO_CL_LONG, refreshFn, TO_CL_LONG/*, 500*/);
  fnTimerConfig(TO_FG_TIMR, refreshFn, TO_FG_TIMR/*, 4000*/);
  fnTimerConfig(TO_FN_LONG, refreshFn, TO_FN_LONG/*, 450*/);
  fnTimerConfig(TO_FN_EXEC, execFnTimeout, 0/*, 150*/);
  fnTimerConfig(TO_3S_CTFF, shiftCutoff, TO_3S_CTFF/*, 600*/);
  fnTimerConfig(TO_CL_DROP, fnTimerDummyTest, TO_CL_DROP/*, 500*/);
  fnTimerConfig(TO_KB_ACTV, fnTimerDummyTest, TO_KB_ACTV/*, 6000*/);
  nextTimerRefresh = 0;                                        //vv

  // Status flags:
  //   ST(STAT_PGM_END)   - Indicates that program should go to off state (set by auto off timer)
  //   ST(STAT_SUSPENDED) - Program signals it is ready for off and doesn't need to be woken-up again
  //   ST(STAT_OFF)       - Program in off state (OS goes to sleep and only [EXIT] key can wake it up again)
  //   ST(STAT_RUNNING)   - OS doesn't sleep in this mode
  while(!backToDMCP) {
    if(ST(STAT_PGM_END) && ST(STAT_SUSPENDED)) { // Already in off mode and suspended
      CLR_ST(STAT_RUNNING);
      sys_sleep();
    } else if ((!ST(STAT_PGM_END) && key_empty() && emptyKeyBuffer())) {        // Just wait if no keys available.      //dr - internal keyBuffer POC
      uint32_t sleepTime = max(1, nextScreenRefresh - sys_current_ms());        //vv dr timer without DMCP timer
      if(nextTimerRefresh != 0) {
        uint32_t timeoutTime = max(1, nextTimerRefresh - sys_current_ms());
        sleepTime = min(sleepTime, timeoutTime);
      }
      if(fnTimerGetStatus(TO_KB_ACTV) == TMR_RUNNING) {
        sleepTime = min(sleepTime, 40);
      }
      if(fnTimerGetStatus(TO_FN_EXEC) == TMR_RUNNING) {
        sleepTime = min(sleepTime, 15);
      }                                                                         //^^
      CLR_ST(STAT_RUNNING);
      sys_timer_start(TIMER_IDX_SCREEN_REFRESH, max(1, sleepTime));             // wake up for screen refresh           //dr
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

    // Fetch the key
    //  < 0 -> No key event
    //  > 0 -> Key pressed
    // == 0 -> Key released
//  key = key_pop();                                        //dr - removed because of internal keyBuffer POC
    
                                                            //vv dr - internal keyBuffer POC
    // uint8_t outKeyCount;
    // == 0 -> Key released
    // == 1 -> Key pressed
    // == 2 -> The same key pressed twice.
    // == 3 -> The same key pressed three times.
    // not compatible to pc routine
    
    uint8_t outKey;
    keyBuffer_pop();

#ifdef BUFFER_CLICK_DETECTION
    uint32_t timeSpan_1;
    uint32_t timeSpan_B;
#ifdef BUFFER_KEY_COUNT
    uint8_t outKeyCount;
    if(outKeyBuffer(&outKey, &outKeyCount, &timeStampKey, &timeSpan_1, &timeSpan_B) == BUFFER_SUCCESS) {
      key = outKey;
      keyCount = outKeyCount;
//    if(outKeyCount > 0) {
//      do someting
//    }
    }
#else
    if(outKeyBuffer(&outKey, &timeStampKey, &timeSpan_1, &timeSpan_B) == BUFFER_SUCCESS) {
      key = outKey;
    }
#endif
#else
#ifdef BUFFER_KEY_COUNT
    uint8_t outKeyCount;
    if(outKeyBuffer(&outKey, &outKeyCount) == BUFFER_SUCCESS) {
      key = outKey;
      keyCount = outKeyCount;
    }
#else
    if(outKeyBuffer(&outKey) == BUFFER_SUCCESS) {
      key = outKey;
    }
#endif
#endif
    else {
      key = -1;
    }                                                       //^^

    //The 3 lines below to see in the top left screen corner the pressed keycode
    //char sysLastKeyCh[5];
    //sprintf(sysLastKeyCh, "%2d", sys_last_key());
    //showString(sysLastKeyCh, &standardFont, 0, 0, vmReverse, true, true);

    if(key == 44 ) { //sys_last_key DISP for special SCREEN DUMP key code. To be 16 but shift decoding already done to 44 in DMCP
      resetShiftState();                                       //JM to avoid f or g top left of the screen

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
   
   #ifdef JMSHOWCODES 
    fnDisplayStack(1);
    //Show key codes
    if(sys_last_key()!=telltale_lastkey) {
       telltale_lastkey = sys_last_key();
       telltale_pos++;
       telltale_pos = telltale_pos & 0x03;
       char aaa[100];
       sprintf   (aaa,"k=%d d=%ld      ",key, timeSpan);
       showString(aaa, &standardFont, 300, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_T - REGISTER_X), vmNormal, true, true);
       sprintf   (aaa,"Rel=%d, nop=%d, St=%d, Key=%d, FN_kp=%d   ",FN_timed_out_to_RELEASE_EXEC, FN_timed_out_to_NOP, FN_state, sys_last_key(), FN_key_pressed);
       showString(aaa, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Z - REGISTER_X), vmNormal, true, true);
       sprintf   (aaa,"%4d(%4ld)<<",sys_last_key(),timeSpan);
       showString(aaa, &standardFont, telltale_pos*90+ 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Y - REGISTER_X), vmNormal, true, true);
     }
    #endif

    if(38 <= key && key <=43) {
      sprintf(charKey, "%c", key+11);
      btnFnPressed(charKey);
    //lcd_refresh_dma();
    }
    else if(1 <= key && key <= 37) {
      sprintf(charKey, "%02d", key - 1);
      btnPressed(charKey);
    //lcd_refresh_dma();
    }
    else if(key == 0 && FN_key_pressed != 0) {                 //JM, key=0 is release, therefore there must have been a press before that. If the press was a FN key, FN_key_pressed > 0 when it comes back here for release.
      btnFnReleased(NULL);                                     //    in short, it can only execute FN release after there was a FN press.
    //lcd_refresh_dma();
    }
    else if(key == 0) {
      btnReleased(NULL);
    //lcd_refresh_dma();
    }

    if(key >= 0) {                                          //dr
      lcd_refresh_dma();
      fnTimerStart(TO_KB_ACTV, TO_KB_ACTV, JM_TO_KB_ACTV);  //dr
      count_refreshes = 10;                                 //dr
    }

    uint32_t now = sys_current_ms();
    if(nextTimerRefresh != 0 && nextTimerRefresh <= now) {  //vv dr Timer
      refreshTimer();
    }                                                       //^^
    now = sys_current_ms();                                 //vv dr
    if(nextScreenRefresh <= now) {
      nextScreenRefresh += SCREEN_REFRESH_PERIOD;
      count_refreshes++;
      if(count_refreshes > 384)
        nextScreenRefresh += SCREEN_REFRESH_PERIOD;
      if(nextScreenRefresh < now) {
        nextScreenRefresh = now + SCREEN_REFRESH_PERIOD;    // we were out longer than expected; just skip ahead.
      }
      refreshLcd();
      if(key >= 0) {
        lcd_refresh();
      }
      else {
        lcd_refresh_wait();
      }
    }                                                       //^^
  }
}
#endif

#ifdef TESTSUITE_BUILD
#include "testSuite.h"

int main(int argc, char* argv[]) {
  #if defined __APPLE__
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
  #endif

  wp43sMemInBytes = 0;
  gmpMemInBytes = 0;
  mp_set_memory_functions(allocGmp, reallocGmp, freeGmp);

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
