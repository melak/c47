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
 * \file config.c
 ***********************************************/

#include "wp43s.h"



/********************************************//**
 * \brief Sets the calc to meet Chinese standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigChina(uint16_t unusedParamButMandatory) {
  fnRadixMark(RM_PERIOD);
  groupingGap = 4;
  refreshStack();
  fnTimeFormat(TF_H24);
  fnDateFormat(DF_YMD);
  oldTime[0] = 0;
  firstGregorianDay = 1949; // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet European standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigEurope(uint16_t unusedParamButMandatory) {
  fnRadixMark(RM_COMMA);
  groupingGap = 3;
  refreshStack();
  fnTimeFormat(TF_H24);
  fnDateFormat(DF_DMY);
  oldTime[0] = 0;
  firstGregorianDay = 1582; // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet Indian standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigIndia(uint16_t unusedParamButMandatory) {
  fnRadixMark(RM_PERIOD);
  groupingGap = 3;
  refreshStack();
  fnTimeFormat(TF_H24);
  fnDateFormat(DF_DMY);
  oldTime[0] = 0;
  firstGregorianDay = 1752; // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet Japanese standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigJapan(uint16_t unusedParamButMandatory) {
  fnRadixMark(RM_PERIOD);
  groupingGap = 3;
  refreshStack();
  fnTimeFormat(TF_H24);
  fnDateFormat(DF_YMD);
  oldTime[0] = 0;
  firstGregorianDay = 1873; // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet English standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigUk(uint16_t unusedParamButMandatory) {
  fnRadixMark(RM_PERIOD);
  groupingGap = 3;
  refreshStack();
  fnTimeFormat(TF_H12);
  fnDateFormat(DF_DMY);
  oldTime[0] = 0;
  firstGregorianDay = 1752; // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet American standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigUsa(uint16_t unusedParamButMandatory) {
  fnRadixMark(RM_PERIOD);
  groupingGap = 3;
  refreshStack();
  fnTimeFormat(TF_H12);
  fnDateFormat(DF_MDY);
  oldTime[0] = 0;
  firstGregorianDay = 1752; // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the integer mode
 *
 * \param[in] mode uint16_t Integer mode
 * \return void
 ***********************************************/
void fnIntegerMode(uint16_t mode) {
  shortIntegerMode = mode;
  
  fnRefreshRadioState(RB_IM, mode);                                            //dr

  showIntegerMode();
  refreshStack();
}

/********************************************//**
 * \brief Sets the the leading zeros mode in integer mode
 *
 * \param[in] dlz uint16_t
 * \return void
 ***********************************************/
void fnLeadingZeros(uint16_t dlz) {
  displayLeadingZeros = dlz;

//fnRefreshRadioState(RB_BLZ, dlz);                                             //dr
  fnRefreshComboxState(CB_JC, JC_BLZ, displayLeadingZeros);                     //dr

  refreshStack();
}



/********************************************//**
 * \brief Displays credits to the brave men who
 * made this project work.
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnWho(uint16_t unusedParamButMandatory) {
  temporaryInformation = TI_WHO;
  refreshRegisterLine(REGISTER_X);
 }



/********************************************//**
 * \brief Displays the version of this software
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnVersion(uint16_t unusedParamButMandatory) {
  temporaryInformation = TI_VERSION;
  refreshRegisterLine(REGISTER_X);
}



/********************************************//**
 * \brief Sets X to the amount of free RAM
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnFreeMemory(uint16_t unusedParamButMandatory) {
  longInteger_t mem;

  liftStack();

  longIntegerInit(mem);
  uIntToLongInteger(getFreeRamMemory(), mem);
  convertLongIntegerToLongIntegerRegister(mem, REGISTER_X);
  longIntegerFree(mem);

  refreshStack();
}



/********************************************//**
 * \brief Sets X to the value of the rounding mode
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGetRoundingMode(uint16_t unusedParamButMandatory) {
  longInteger_t rounding;

  liftStack();

  longIntegerInit(rounding);
  uIntToLongInteger(roundingMode, rounding);
  convertLongIntegerToLongIntegerRegister(rounding, REGISTER_X);
  longIntegerFree(rounding);

  refreshStack();
}



/********************************************//**
 * \brief Sets X to the value of the integer mode
 *
 * \return void
 ***********************************************/
void fnGetIntegerSignMode(uint16_t unusedParamButMandatory) {
  longInteger_t ism;

  liftStack();

  longIntegerInit(ism);
  uIntToLongInteger((shortIntegerMode==SIM_2COMPL ? 2 : (shortIntegerMode==SIM_1COMPL ? 1 : (shortIntegerMode==SIM_UNSIGN ? 0 : -1))), ism);
  convertLongIntegerToLongIntegerRegister(ism, REGISTER_X);
  longIntegerFree(ism);

  refreshStack();
}



/********************************************//**
 * \brief Gets the word size
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGetWordSize(uint16_t unusedParamButMandatory) {
  longInteger_t wordSize;

  liftStack();

  longIntegerInit(wordSize);
  uIntToLongInteger(shortIntegerWordSize, wordSize);
  convertLongIntegerToLongIntegerRegister(wordSize, REGISTER_X);
  longIntegerFree(wordSize);

  refreshStack();
}



/********************************************//**
 * \brief Sets the word size
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnSetWordSize(uint16_t WS) {
  bool_t reduceWordSize;
  if(WS == 0) {
    WS = 64;
  }

  reduceWordSize = (WS < shortIntegerWordSize);

  shortIntegerWordSize = WS;
  showIntegerMode();

  if(shortIntegerWordSize == 64) {
    shortIntegerMask    = -1;
  }
  else {
    shortIntegerMask    = ((uint64_t)1 << shortIntegerWordSize) - 1;
  }

  shortIntegerSignBit = (uint64_t)1 << (shortIntegerWordSize - 1);
  //printf("shortIntegerMask  =   %08x-%08x\n", (unsigned int)(shortIntegerMask>>32), (unsigned int)(shortIntegerMask&0xffffffff));
  //printf("shortIntegerSignBit = %08x-%08x\n", (unsigned int)(shortIntegerSignBit>>32), (unsigned int)(shortIntegerSignBit&0xffffffff));

  if(reduceWordSize) {
    // reduce the word size of integers on the stack
    for(calcRegister_t regist=REGISTER_X; regist<=getStackTop(); regist++) {
      if(getRegisterDataType(regist) == dtShortInteger) {
        *(REGISTER_SHORT_INTEGER_DATA(regist)) &= shortIntegerMask;
      }
    }

    // reduce the word size of integers in the L register
    if(getRegisterDataType(REGISTER_L) == dtShortInteger) {
      *(REGISTER_SHORT_INTEGER_DATA(REGISTER_L)) &= shortIntegerMask;
    }
  }

  fnRefreshRadioState(RB_WS, WS);                                               //dr

  refreshStack();
}



/********************************************//**
 * \brief Sets X to the amount of free flash memory
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnFreeFlashMemory(uint16_t unusedParamButMandatory) {
  longInteger_t flashMem;

  liftStack();

  longIntegerInit(flashMem);
  uIntToLongInteger(getFreeFlash(), flashMem);
  convertLongIntegerToLongIntegerRegister(flashMem, REGISTER_X);
  longIntegerFree(flashMem);

  refreshStack();
}



/********************************************//**
 * \brief Sets X to the battery voltage
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnBatteryVoltage(uint16_t unusedParamButMandatory) {
  real39_t value;

  liftStack();

  #ifdef PC_BUILD
    int32ToReal(3100, &value);
  #endif

  #ifdef DMCP_BUILD
    int32ToReal(get_vbat(), &value);
  #endif

  realDivide(&value, const_1000, &value, &ctxtReal39);
  realToReal34(&value, REGISTER_REAL34_DATA(REGISTER_X));
  refreshStack();
}



/********************************************//**
 * \brief Retrives the amount of free flash memory
 *
 * \param void
 * \return uint32_t Number of bytes free
 ***********************************************/
uint32_t getFreeFlash(void) {
  return 1234567u;
}



/********************************************//**
 * \brief Sets X to the number of signficant digits
 * rounding after each operation
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGetSignificantDigits(uint16_t unusedParamButMandatory) {
  longInteger_t sigDigits;

  liftStack();

  longIntegerInit(sigDigits);
  uIntToLongInteger(significantDigits, sigDigits);
  convertLongIntegerToLongIntegerRegister(sigDigits, REGISTER_X);
  longIntegerFree(sigDigits);

  refreshStack();
}



/********************************************//**
 * \brief Sets the rounding mode
 *
 * \param[in] RM uint16_t Rounding mode
 * \return void
 ***********************************************/
void fnRoundingMode(uint16_t RM) {
  roundingMode = RM;

  if(RM == 0) ctxtReal34.round = DEC_ROUND_HALF_EVEN;
  else if(RM == 1) ctxtReal34.round = DEC_ROUND_HALF_UP;
  else if(RM == 2) ctxtReal34.round = DEC_ROUND_HALF_DOWN;
  else if(RM == 3) ctxtReal34.round = DEC_ROUND_UP;
  else if(RM == 4) ctxtReal34.round = DEC_ROUND_DOWN;
  else if(RM == 5) ctxtReal34.round = DEC_ROUND_CEILING;
  else if(RM == 6) ctxtReal34.round = DEC_ROUND_FLOOR;
  else {
    sprintf(errorMessage, "In function fnRoundingMode: %d is an unexpected value for RM! Must be from 0 to 6", RM);
    displayBugScreen(errorMessage);
  }
}



/********************************************//**
 * \brief Sets the angular mode
 *
 * \param[in] am uint16_t Angular mode
 * \return void
 ***********************************************/
void fnAngularMode(uint16_t am) {
  currentAngularMode = am;

  fnRefreshRadioState(RB_AM, am);                                               //dr

  showAngularMode();
  refreshStack();
}



void setConfirmationMode(void (*func)(uint16_t)) {
  previousCalcMode = calcMode;
  calcMode = CM_CONFIRMATION;
  confirmedFunction = func;
  temporaryInformation = TI_ARE_YOU_SURE;
  refreshStack();
}



/********************************************//**
 * \brief Defines the complex unit i or j
 *
 * \param[in] complexUnit uint16_t
 * \return void
 ***********************************************/
void fnComplexUnit(uint16_t cu) {
  complexUnit = cu;

  fnRefreshRadioState(RB_CU, cu);                                               //dr

  refreshStack();
}

/********************************************//**
 * \brief Defines if an operation result can be complex or not
 *
 * \param[in] complexRes uint16_t
 * \return void
 ***********************************************/
void fnComplexResult(uint16_t complexResult) {
  complexResult ? fnSetFlag(FLAG_CPXRES) : fnClearFlag(FLAG_CPXRES);

//fnRefreshRadioState(RB_BCR, complexResult);                                   //dr
  fnRefreshComboxState(CB_JC, JC_BCR, complexResult);                           //dr
}

/********************************************//**
 * \brief Defines the complex display mode: rectangular or polar
 *
 * \param[in] complexMode uint16_t
 * \return void
 ***********************************************/
void fnComplexMode(uint16_t cm) {
  complexMode = cm;

  fnRefreshRadioState(RB_CM, cm);                                               //dr

  showComplexMode();
  refreshStack();
}



void fnClAll(uint16_t confirmation) {
  if(confirmation == NOT_CONFIRMED) {
    setConfirmationMode(fnClAll);
  }
  else {
//printf("Running CLALL\n");
    calcRegister_t regist;

    fnClPAll(CONFIRMED);  // Clears all the programs
    fnClSigma(CONFIRMED); // Clears and releases the memory of all statistical sums

    // Clear local registers
    allocateLocalRegisters(0);
    //for(regist=FIRST_LOCAL_REGISTER; regist<FIRST_LOCAL_REGISTER + numberOfLocalRegisters; regist++) {
    //  clearRegister(regist);
    //}

    // Clear registers including stack, I, J, K and L
    for(regist=0; regist<FIRST_LOCAL_REGISTER; regist++) {
      clearRegister(regist);
    }

    // Clear flags
    for(int32_t sixteenFlags=0; sixteenFlags<7; sixteenFlags++) { // 7 times uint16_t = 112 flags
      flags[sixteenFlags] = 0;
    }

    temporaryInformation = TI_NO_INFO;
  }
}



void fnClPAll(uint16_t confirmation) {
  if(confirmation == NOT_CONFIRMED) {
    setConfirmationMode(fnClPAll);
  }
  else {
    //printf("Running CLPALL\n");
    // TODO: actually clear all programs
    programCounter = 0;
    showPgmBegin();
    temporaryInformation = TI_NO_INFO;
  }
}



void fnClSigma(uint16_t unusedParamButMandatory) {
  if(statisticalSumsPointer != NULL) {
    freeWp43s(statisticalSumsPointer, NUMBER_OF_STATISTICAL_SUMS*REAL34_SIZE);
    graph_end();  //JMGRAPH
    statisticalSumsPointer = NULL;
  }
}



void fnReset(uint16_t confirmation) {
  if(confirmation == NOT_CONFIRMED) {
    setConfirmationMode(fnReset);
  }
  else {
    clearScreen(true, true, true);

    fnClAll(CONFIRMED); // Clears pgm and registers
    fnTimeFormat(TF_H24);              //JMnote This line is over-writing the content of setupdefaults
    fnIntegerMode(SIM_2COMPL);         //JMnote This line is over-writing the content of setupdefaults
    fnDisplayFormatAll(3);          //JM DEFAULT //JMnote This line is over-writing the content of setupdefaults
    fnDisplayFormatGap(3);             //JMnote This line is over-writing the content of setupdefaults
    fnComplexUnit(CU_I);               //JMnote This line is over-writing the content of setupdefaults
    fnAngularMode(AM_DEGREE);          //JMnote This line is over-writing the content of setupdefaults
    fnDenMode(DM_ANY);                 //JMnote This line is over-writing the content of setupdefaults
    fnDenMax(0);                       //JMnote This line is over-writing the content of setupdefaults
    fnDisplayStack(4);                 //JMnote This line is over-writing the content of setupdefaults
    firstGregorianDay = 1752;          //JMnote This line is over-writing the content of setupdefaults
    fnCurveFitting(CF_LINEAR_FITTING); //JMnote This line is over-writing the content of setupdefaults
    fnLeadingZeros(true);           //JM DEFAULT //JMnote This line is over-writing the content of setupdefaults
    fnProductSign(PS_CROSS);           //JMnote This line is over-writing the content of setupdefaults
    fnRadixMark(RM_PERIOD);            //JMnote This line is over-writing the content of setupdefaults
    fnRoundingMode(RM_HALF_EVEN);      //JMnote This line is over-writing the content of setupdefaults
    fnDisplayOvr(DO_SCI);              //JMnote This line is over-writing the content of setupdefaults
    fnStackSize(SS_8);              //JM DEFAULT //JMnote This line is over-writing the content of setupdefaults
    //tDisp = -1;
    fnSetWordSize(64);                 //JMnote This line is over-writing the content of setupdefaults
    fnDateFormat(DF_YMD);              //JMnote This line is over-writing the content of setupdefaults
    fnComplexMode(CM_RECTANGULAR);     //JMnote This line is over-writing the content of setupdefaults
    showRealComplexResult();
    allocateLocalRegisters(0);
    fnComplexResult(true);          //JM DEFAULT //JMnote This line is over-writing the content of setupdefaults
    fnSetFlag(FLAG_DANGER);         //JM infinity etc.

    fractionType = FT_NONE;
    STACK_LIFT_DISABLE;
    showOverflowCarry();
    hideUserMode();

    graph_xmin = -3*3.14159265;                                    //JM GRAPH
    graph_xmax = +3*3.14159265;                                    //JM GRAPH
    graph_ymin = -2;                                               //JM GRAPH
    graph_ymax = +2;                                               //JM GRAPH
    graph_dx   = 0;                                                //JM GRAPH
    graph_dy   = 0;                                                //JM GRAPH




    // Initialization of user key assignments
    memcpy(kbd_usr, kbd_std, sizeof(kbd_std));

    // RNG initialisation
    pcg32_srandom(0x1963073019931121ULL, 0x1995062319981019ULL);

    lastIntegerBase = 0;

    lastFcnsMenuPos = 0;
    lastMenuMenuPos = 0;
    lastCnstMenuPos = 0;

    currentFntScr = 0;
    currentFlgScr = 0;
    currentRegisterBrowserScreen = 9999;

    // initialize the RadioButton/Checkbox items
    fnRebuildRadioState();                                                      //dr build RadioButton, Checkbox

    #ifndef TESTSUITE_BUILD
      while(softmenuStackPointer > 0) {
        popSoftmenu();
      }
    #endif // TESTSUITE_BUILD

    oldTime[0] = 0;

    temporaryInformation = TI_RESET;

    //JM Default USER
    fnUserJM(USER_RESET);                        //JM USER
//    kbd_usr[0].primary     = KEY_CC;           //JM CPX TEMP DEFAULT         //JM bug. over-writing the content of setupdefaults
//    kbd_usr[0].gShifted    = KEY_TYPCON_UP;    //JM TEMP DEFAULT            //JM bug. over-writing the content of setupdefaults
//    kbd_usr[0].fShifted    = KEY_TYPCON_DN;    //JM TEMP DEFAULT            //JM bug. over-writing the content of setupdefaults

    // The following lines are test data
    //kbd_usr[ 0].keyLblAim   = CHR_A_GRAVE;
    //kbd_usr[ 0].fShiftedAim = CHR_A_GRAVE;
    //kbd_usr[ 4].keyLblAim   = CHR_E_ACUTE;
    //kbd_usr[ 4].fShiftedAim = CHR_E_ACUTE;
    //kbd_usr[18].fShifted    = -MNU_VARS;
    //kbd_usr[18].gShifted    = CST_54;
    //kbd_usr[19].fShifted    = ITM_SW;
    //kbd_usr[19].gShifted    = ITM_SXY;
    //kbd_usr[20].gShifted    = ITM_LYtoM;

    //fnSetFlag(  3);
    //fnSetFlag( 11);
    //fnSetFlag( 33);
    //fnSetFlag( 34);
    //fnSetFlag( 52);
    //fnSetFlag( 62);
    //fnSetFlag( 77);
    //fnSetFlag( 85);
    //fnSetFlag(FLAG_CARRY);
    //fnSetFlag(FLAG_DANGER);
    //showOverflowCarry();

    //allocateLocalRegisters(2);
    //fnSetFlag(FIRST_LOCAL_REGISTER+0);
    //fnSetFlag(NUMBER_OF_LOCAL_FLAGS+2);
    //reallocateRegister(FIRST_LOCAL_REGISTER+0, dtReal16, REAL16_SIZE, RT_REAL);
    //stringToReal16("5.555", REGISTER_REAL34_DATA(FIRST_LOCAL_REGISTER));

    //strcpy(tmpStr3000, "Pure ASCII string requiring 38 bytes!");
    //reallocateRegister(FIRST_LOCAL_REGISTER+1, dtString, strlen(tmpStr3000), AM_NONE);
    //strcpy(REGISTER_STRING_DATA(FIRST_LOCAL_REGISTER + 1), tmpStr3000);


    //allocateNamedVariable("Z" STD_a_DIARESIS "hler");
    //allocateNamedVariable(STD_omega STD_SUB_1);
    //allocateNamedVariable(STD_omega STD_SUB_2);

    #if (DEBUG_PANEL == 1)
      debugWindow = DBG_REGISTERS;
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chkHexaString), false);
      refreshDebugPanel();
    #endif

    //JM TEMPORARY TEST DATA IN REGISTERS
    fnStrtoX("Reg 11,12 & 13 have: The 3 cubes = 3.");
    fnStore(10);
    fnDrop(0);
    fnStrInputLongint("569936821221962380720");
    fnStore(11);
    fnDrop(0);
    fnStrInputLongint("-569936821113563493509");
    fnStore(12);
    fnDrop(0);
    fnStrInputLongint("-472715493453327032");
    fnStore(13);
    fnDrop(0);

    fnStrtoX("Reg 15, 16 & 17 have: The 3 cubes = 42.");
    fnStore(14);
    fnDrop(0);
    fnStrInputLongint("-80538738812075974");
    fnStore(15);
    fnDrop(0);
    fnStrInputLongint("80435758145817515");
    fnStore(16);
    fnDrop(0);
    fnStrInputLongint("12602123297335631");
    fnStore(17);
    fnDrop(0);

    fnStrtoX("37 digits of pi, Reg19 / Reg20.");
    fnStore(18);
    fnDrop(0);
    fnStrInputLongint("2646693125139304345");
    fnStore(19);
    fnDrop(0);
    fnStrInputLongint("842468587426513207");
    fnStore(20);
    fnDrop(0);

    fnStrtoX("Primes: Carol, Kynea, repunit, Woodal");
    fnStore(21);
    fnDrop(0);
    fnStrInputLongint("18014398241046527");
    fnStore(22);
    fnDrop(0);
    fnStrInputLongint("18446744082299486207");
    fnStore(23);
    fnDrop(0);
    fnStrInputLongint("7369130657357778596659");
    fnStore(24);
    fnDrop(0);
    fnStrInputLongint("195845982777569926302400511");
    fnStore(25);
    fnDrop(0);
    fnStrInputLongint("4776913109852041418248056622882488319");
    fnStore(26);
    fnDrop(0);
    fnStrInputLongint("225251798594466661409915431774713195745814267044878909733007331390393510002687");
    fnStore(27);
    fnDrop(0);




    refreshStack();
  }
}



void backToSystem(uint16_t unusedParamButMandatory) {
  #ifdef PC_BUILD
    fnOff(NOPARAM);
  #endif

  #ifdef DMCP_BUILD
    backToDMCP = true;
  #endif
}
