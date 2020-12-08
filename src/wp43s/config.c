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
  setSystemFlag(FLAG_DECIMP);
  groupingGap = 4;
  setSystemFlag(FLAG_TDM24); // time format = 24H
  clearSystemFlag(FLAG_DMY); // date format
  clearSystemFlag(FLAG_MDY); // date format
  setSystemFlag(FLAG_YMD);   // date format
  firstGregorianDay = 1949;  // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet European standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigEurope(uint16_t unusedParamButMandatory) {
  clearSystemFlag(FLAG_DECIMP);
  groupingGap = 3;
  setSystemFlag(FLAG_TDM24); // time format = 24H
  clearSystemFlag(FLAG_MDY); // date format
  clearSystemFlag(FLAG_YMD); // date format
  setSystemFlag(FLAG_DMY);   // date format
  firstGregorianDay = 1582;  // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet Indian standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigIndia(uint16_t unusedParamButMandatory) {
  setSystemFlag(FLAG_DECIMP);
  groupingGap = 3;
  setSystemFlag(FLAG_TDM24); // time format = 24H
  clearSystemFlag(FLAG_MDY); // date format
  clearSystemFlag(FLAG_YMD); // date format
  setSystemFlag(FLAG_DMY);   // date format
  firstGregorianDay = 1752;  // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet Japanese standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigJapan(uint16_t unusedParamButMandatory) {
  setSystemFlag(FLAG_DECIMP);
  groupingGap = 3;
  setSystemFlag(FLAG_TDM24); // time format = 24H
  clearSystemFlag(FLAG_MDY); // date format
  clearSystemFlag(FLAG_DMY); // date format
  setSystemFlag(FLAG_YMD);   // date format
  firstGregorianDay = 1873;  // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet English standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigUk(uint16_t unusedParamButMandatory) {
  setSystemFlag(FLAG_DECIMP);
  groupingGap = 3;
  clearSystemFlag(FLAG_TDM24); // time format = 12H
  clearSystemFlag(FLAG_MDY);   // date format
  clearSystemFlag(FLAG_YMD);   // date format
  setSystemFlag(FLAG_DMY);     // date format
  firstGregorianDay = 1752;    // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the calc to meet American standards
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnConfigUsa(uint16_t unusedParamButMandatory) {
  setSystemFlag(FLAG_DECIMP);
  groupingGap = 3;
  clearSystemFlag(FLAG_TDM24); // time format = 12H
  clearSystemFlag(FLAG_YMD);   // date format
  clearSystemFlag(FLAG_DMY);   // date format
  setSystemFlag(FLAG_MDY);     // date format
  firstGregorianDay = 1752;    // JDN of the first day in the Gregorian calendar
}



/********************************************//**
 * \brief Sets the integer mode
 *
 * \param[in] mode uint16_t Integer mode
 * \return void
 ***********************************************/
void fnIntegerMode(uint16_t mode) {
  shortIntegerMode = mode;
  
  fnRefreshRadioState(RB_IM, mode);                            //dr

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
 }



/********************************************//**
 * \brief Displays the version of this software
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnVersion(uint16_t unusedParamButMandatory) {
  temporaryInformation = TI_VERSION;
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

  fnRefreshRadioState(RB_WS, WS);                              //dr

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
}



/********************************************//**
 * \brief Sets X to the battery voltage
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnBatteryVoltage(uint16_t unusedParamButMandatory) {
  real_t value;

  liftStack();

  #ifdef PC_BUILD
    int32ToReal(3100, &value);
  #endif

  #ifdef DMCP_BUILD
    int32ToReal(get_vbat(), &value);
  #endif

  realDivide(&value, const_1000, &value, &ctxtReal39);
  realToReal34(&value, REGISTER_REAL34_DATA(REGISTER_X));
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

  fnRefreshRadioState(RB_AM, am);                              //dr

}



void fnFractionType(uint16_t unusedParamButMandatory) {
  if(getSystemFlag(FLAG_FRACT)) {
    flipSystemFlag(FLAG_PROPFR);
  }
  else {
    setSystemFlag(FLAG_FRACT);
  }
}



void setConfirmationMode(void (*func)(uint16_t)) {
  previousCalcMode = calcMode;
  cursorEnabled = false;
  calcMode = CM_CONFIRMATION;
  clearSystemFlag(FLAG_ALPHA);
  confirmedFunction = func;
  temporaryInformation = TI_ARE_YOU_SURE;
}



void fnRange(uint16_t unusedParamButMandatory) {
  longInteger_t longInt;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_X, longInt);
  }
  else if(getRegisterDataType(REGISTER_X) == dtReal34) {
    convertReal34ToLongInteger(REGISTER_REAL34_DATA(REGISTER_X), longInt, DEC_ROUND_DOWN);
  }
  else {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot use %s for setting RANGE", getRegisterDataTypeName(REGISTER_X, true, true));
      moreInfoOnError("In function fnRange:", errorMessage, NULL, NULL);
    #endif
    return;
  }

  longIntegerSetPositiveSign(longInt);

  if(longIntegerCompareInt(longInt, 6145) > 0) {
    exponentLimit = 6145;
  }
  else if(longIntegerCompareInt(longInt, 99) < 0) {
    exponentLimit = 99;
  }
  else {
    exponentLimit = (int16_t)(longInt->_mp_d[0]); // OK for 32 and 64 bit limbs
  }

  longIntegerFree(longInt);
#ifndef TESTSUITE_BUILD
//JMXX  showSoftmenuCurrentPart();                       //JM
#endif
}



void fnGetRange(uint16_t unusedParamButMandatory) {
  longInteger_t range;

  liftStack();

  longIntegerInit(range);
  uIntToLongInteger(exponentLimit, range);
  convertLongIntegerToLongIntegerRegister(range, REGISTER_X);
  longIntegerFree(range);
}



void fnClAll(uint16_t confirmation) {
  if(confirmation == NOT_CONFIRMED) {
    setConfirmationMode(fnClAll);
  }
  else {
    calcRegister_t regist;

    fnClPAll(CONFIRMED);  // Clears all the programs
    fnClSigma(CONFIRMED); // Clears and releases the memory of all statistical sums
    if(savedStatisticalSumsPointer != NULL) {
      freeWp43s(savedStatisticalSumsPointer, NUMBER_OF_STATISTICAL_SUMS * TO_BYTES(REAL_SIZE));
    }

    // Clear local registers
    allocateLocalRegisters(0);

    // Clear registers including stack, I, J, K and L
    for(regist=0; regist<FIRST_LOCAL_REGISTER; regist++) {
      clearRegister(regist);
    }

    // Clear saved stack registers
    for(regist=FIRST_SAVED_STACK_REGISTER; regist<=TEMP_REGISTER; regist++) {
      clearRegister(regist);
    }
    thereIsSomethingToUndo = false;

    // TODO: clear (or delete) named variables

    // Clear flags
    for(int32_t sixteenFlags=0; sixteenFlags<7; sixteenFlags++) { // 7 times uint16_t = 112 flags
      globalFlags[sixteenFlags] = 0;
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
    temporaryInformation = TI_NO_INFO;
  }
}



void fnClSigma(uint16_t unusedParamButMandatory) {
  if(statisticalSumsPointer != NULL) {
    freeWp43s(statisticalSumsPointer, NUMBER_OF_STATISTICAL_SUMS * TO_BYTES(REAL_SIZE));
    graph_end();                                               //JMGRAPH
    statisticalSumsPointer = NULL;
  }
}



void fnReset(uint16_t confirmation) {
  if(confirmation == NOT_CONFIRMED) {
    setConfirmationMode(fnReset);
  }
  else {
    fnClAll(CONFIRMED); // Clears pgm and registers

    systemFlags = 0;
    setSystemFlag(FLAG_TDM24);
    fnIntegerMode(SIM_2COMPL);
    fnDisplayFormatAll(3);                                     //JM Default
    fnDisplayFormatGap(3);
    clearSystemFlag(FLAG_CPXj);
    fnAngularMode(AM_DEGREE);
    clearSystemFlag(FLAG_DENANY);                              //JM Default
    fnDenMax(0);                                               //JM Default
    fnDisplayStack(4);
    firstGregorianDay = 1752;
    clearSystemFlag(FLAG_LEAD0);
    setSystemFlag(FLAG_MULTx);
    setSystemFlag(FLAG_DECIMP);
    fnRoundingMode(RM_HALF_EVEN);
    clearSystemFlag(FLAG_ALLENG);
    setSystemFlag(FLAG_AUTOFF);
    setSystemFlag(FLAG_SSIZE8);                                //JM Default
    //tDisp = -1;
    fnSetWordSize(64);
    clearSystemFlag(FLAG_MDY); // date format
    clearSystemFlag(FLAG_DMY); // date format
    setSystemFlag(FLAG_YMD);   // date format
    clearSystemFlag(FLAG_POLAR);
    setSystemFlag(FLAG_CPXRES);                                //JM Default
    allocateLocalRegisters(0);

    clearSystemFlag(FLAG_FRACT);
    clearSystemFlag(FLAG_PROPFR);
    clearSystemFlag(FLAG_OVERFLOW);
    clearSystemFlag(FLAG_CARRY);
    clearSystemFlag(FLAG_ASLIFT);
    clearSystemFlag(FLAG_USER);
    clearSystemFlag(FLAG_LOWBAT);
    clearSystemFlag(FLAG_USB);

    hourGlassIconEnabled = false;
    programCounter = 0;
    watchIconEnabled = false;
    serialIOIconEnabled = false;
    printerIconEnabled = false;
    thereIsSomethingToUndo = false;

//    showRealComplexResult();                                   //JM, from wp43s.c
    reset_jm_defaults(true); 

    // Initialization of user key assignments
    xcopy(kbd_usr, kbd_std, sizeof(kbd_std));

    // RNG initialisation
    pcg32_srandom(0x1963073019931121ULL, 0x1995062319981019ULL);

    lastIntegerBase = 0;

    lastFcnsMenuPos = 0;
    lastMenuMenuPos = 0;
    lastCnstMenuPos = 0;
    lastSyFlMenuPos = 0;
    lastAIntMenuPos = 0;

    #ifndef TESTSUITE_BUILD
      initFontBrowser();
    #endif
    currentFlgScr = 0;
    currentRegisterBrowserScreen = 9999;

    #ifndef TESTSUITE_BUILD
      while(softmenuStackPointer > 0) {
        popSoftmenu();
      }
      calcModeNormal();
      if(SH_BASE_HOME && softmenuStackPointer == 0) pushSoftmenu(mm_MNU_HOME); //JM Reset to BASE MENU HOME;   
    #endif // TESTSUITE_BUILD

    exponentLimit = 6145;                                      //JMMAX
    temporaryInformation = TI_RESET;
    aimBuffer[0] = 0;

    //JM Default USER
    fnUserJM(USER_RESET);                                      //JM USER
//    kbd_usr[0].primary     = ITM_CC;                         //JM CPX TEMP DEFAULT        //JM note. over-writing the content of setupdefaults
//    kbd_usr[0].gShifted    = KEY_TYPCON_UP;                  //JM TEMP DEFAULT            //JM note. over-writing the content of setupdefaults
//    kbd_usr[0].fShifted    = KEY_TYPCON_DN;                  //JM TEMP DEFAULT            //JM note. over-writing the content of setupdefaults

    // The following lines are test data
    //kbd_usr[ 0].keyLblAim   = ITM_A_GRAVE;
    //kbd_usr[ 0].fShiftedAim = ITM_A_GRAVE;
    //kbd_usr[ 4].keyLblAim   = ITM_E_ACUTE;
    //kbd_usr[ 4].fShiftedAim = ITM_E_ACUTE;
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
    //setSystemFlag(FLAG_CARRY);
    //setSystemFlag(FLAG_SPCRES);

    //allocateLocalRegisters(2);
    //fnSetFlag(FIRST_LOCAL_REGISTER+0);
    //fnSetFlag(NUMBER_OF_GLOBAL_FLAGS+2);
    //reallocateRegister(FIRST_LOCAL_REGISTER+0, dtReal16, REAL16_SIZE, RT_REAL);
    //stringToReal16("5.555", REGISTER_REAL34_DATA(FIRST_LOCAL_REGISTER));

    //strcpy(tmpStr3000, "Pure ASCII string requiring 38 bytes!");
    //reallocateRegister(FIRST_LOCAL_REGISTER+1, dtString, TO_BLOCKS(strlen(tmpStr3000) + 1), AM_NONE);
    //strcpy(REGISTER_STRING_DATA(FIRST_LOCAL_REGISTER + 1), tmpStr3000);


    //allocateNamedVariable("Z" STD_a_DIARESIS "hler");
    //allocateNamedVariable(STD_omega STD_SUB_1);
    //allocateNamedVariable(STD_omega STD_SUB_2);

    #if (DEBUG_PANEL == 1)
      debugWindow = DBG_REGISTERS;
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chkHexaString), false);
      refreshDebugPanel();
    #endif

    //JM                                                       //JM TEMPORARY TEST DATA IN REGISTERS
    fnStrtoX("Rel48a, Lock tests, pre-PEM-XEQ, 2020-12-08");
    fnStore(102);
    fnDrop(0);

    #ifdef JM_LAYOUT_1A
    fnStrtoX("C43 LAYOUT1 (Main C43 template)");
    #endif
    #ifdef JM_LAYOUT_2_DM42_STRICT
    fnStrtoX("C43 LAYOUT42 (For unmodified DM42)");
    #endif
    fnStore(103);
    fnDrop(0);


    //JM                                                       //JM TEMPORARY TEST DATA IN REGISTERS
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

    doRefreshSoftMenu = true;     //jm dr
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
