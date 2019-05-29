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
  radixMark = RM_PERIOD;
  groupingGap = 4;
  refreshStack();
  timeFormat = TF_H24;
  dateFormat = DF_YMD;
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
  radixMark = RM_COMMA;
  groupingGap = 3;
  refreshStack();
  timeFormat = TF_H24;
  dateFormat = DF_DMY;
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
  radixMark = RM_PERIOD;
  groupingGap = 3;
  refreshStack();
  timeFormat = TF_H24;
  dateFormat = DF_DMY;
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
  radixMark = RM_PERIOD;
  groupingGap = 3;
  refreshStack();
  timeFormat = TF_H24;
  dateFormat = DF_YMD;
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
  radixMark = RM_PERIOD;
  groupingGap = 3;
  refreshStack();
  timeFormat = TF_H12;
  dateFormat = DF_DMY;
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
  radixMark = RM_PERIOD;
  groupingGap = 3;
  refreshStack();
  timeFormat = TF_H12;
  dateFormat = DF_MDY;
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

  uIntToLongInteger(lastFreeByte - firstFreeByte + 1, &mem);
  convertLongIntegerToLongIntegerRegister(&mem, REGISTER_X);

  refreshStack();
}



/********************************************//**
 * \brief Sets X to the value of the rounding mode
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGetRoundingMode(uint16_t unusedParamButMandatory) {
  longInteger_t rm;

  liftStack();

  uIntToLongInteger(roundingMode, &rm);
  convertLongIntegerToLongIntegerRegister(&rm, REGISTER_X);

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

  uIntToLongInteger((shortIntegerMode==SIM_2COMPL ? 2 : (shortIntegerMode==SIM_1COMPL ? 1 : (shortIntegerMode==SIM_UNSIGN ? 0 : -1))), &ism);
  convertLongIntegerToLongIntegerRegister(&ism, REGISTER_X);

  refreshStack();
}



/********************************************//**
 * \brief Gets the word size
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGetWordSize(uint16_t unusedParamButMandatory) {
  longInteger_t ws;

  liftStack();

  uIntToLongInteger(shortIntegerWordSize, &ws);
  convertLongIntegerToLongIntegerRegister(&ws, REGISTER_X);

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
    shortIntegerSignBit = (uint64_t)1 << 63;
  }
  else {
    shortIntegerMask    = ((uint64_t)1 << shortIntegerWordSize) - 1;
    shortIntegerSignBit = (uint64_t)1 << (shortIntegerWordSize - 1);
  }
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

  uIntToLongInteger(getFreeFlash(), &flashMem);
  convertLongIntegerToLongIntegerRegister(&flashMem, REGISTER_X);

  refreshStack();
}



/********************************************//**
 * \brief Sets X to the battery voltage
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnBatteryVoltage(uint16_t unusedParamButMandatory) {
  liftStack();

  #ifdef PC_BUILD
    uInt32ToReal16(3100, REGISTER_REAL16_DATA(REGISTER_X));
  #endif

  #ifdef DMCP_BUILD
    uInt32ToReal16(read_power_voltage(), REGISTER_REAL16_DATA(REGISTER_X));
  #endif

  real16Divide(REGISTER_REAL16_DATA(REGISTER_X), const16_1000, REGISTER_REAL16_DATA(REGISTER_X));
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

  uIntToLongInteger(significantDigits, &sigDigits);
  convertLongIntegerToLongIntegerRegister(&sigDigits, REGISTER_X);

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

  if(RM == 0) ctxtReal16.round = DEC_ROUND_HALF_EVEN;
  else if(RM == 1) ctxtReal16.round = DEC_ROUND_HALF_UP;
  else if(RM == 2) ctxtReal16.round = DEC_ROUND_HALF_DOWN;
  else if(RM == 3) ctxtReal16.round = DEC_ROUND_UP;
  else if(RM == 4) ctxtReal16.round = DEC_ROUND_DOWN;
  else if(RM == 5) ctxtReal16.round = DEC_ROUND_CEILING;
  else if(RM == 6) ctxtReal16.round = DEC_ROUND_FLOOR;
  else {
    sprintf(errorMessage, "In function fnRoundingMode: %d is an unexpected value for RM! Must be from 0 to 6", RM);
    displayBugScreen(errorMessage);
  }

  ctxtReal34.round = ctxtReal16.round;
}



/********************************************//**
 * \brief Sets the angular mode
 *
 * \param[in] am uint16_t Angular mode
 * \return void
 ***********************************************/
void fnAngularMode(uint16_t am) {
  currentAngularMode = am;
  //if(am == AM_DMS && (getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtReal16 || getRegisterDataType(REGISTER_X) == dtReal34)) {
  //  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
  //    convertLongIntegerRegisterToAngleRegister(REGISTER_X, REGISTER_X);
  //    convertAngleToInternal(REGISTER_ANGLE_DATA(REGISTER_X), AM_DMS);
  //    setRegisterDataType(REGISTER_X, dtAngle);
  //    setRegisterTag(REGISTER_X, AM_DMS);
  //  }
  //
  //  convertAngleToInternal(REGISTER_ANGLE_DATA(REGISTER_X), AM_DMS);
  //  setRegisterDataType(REGISTER_X, dtAngle);
  //  setRegisterTag(REGISTER_X, AM_DMS);
  //
  //  refreshRegisterLine(REGISTER_X);
  //}

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
}

/********************************************//**
 * \brief Defines the complex display mode: rectangular or polar
 *
 * \param[in] complexMode uint16_t
 * \return void
 ***********************************************/
void fnComplexMode(uint16_t cm) {
  complexMode = cm;
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


    // initialize the temporary registers
    for(regist=FIRST_TEMPORARY_REGISTER; regist<FIRST_TEMPORARY_REGISTER+NUMBER_OF_TEMPORARY_REGISTERS; regist++) {
      tempRegistersInUse[regist - FIRST_TEMPORARY_REGISTER] = false;
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
  if(statisticalSumsPointer != 0) {
    freeMemory(statisticalSumsPointer, 14*REAL34_SIZE);
    statisticalSumsPointer = 0;
  }
}



void fnReset(uint16_t confirmation) {
  if(confirmation == NOT_CONFIRMED) {
    setConfirmationMode(fnReset);
  }
  else {
    //printf("Running RESET\n");
    clearScreen(true, true, true);

    fnClAll(CONFIRMED); // Clears pgm and registers

    fnTimeFormat(TF_H24);
    fnIntegerMode(SIM_2COMPL);
    fnDisplayFormatAll(0);
    fnDisplayFormatGap(3);
    fnComplexUnit(CU_I);
    fnAngularMode(AM_DEGREE);
    fnDenMode(DM_ANY);
    fnDenMax(0);
    fnDisplayStack(4);
    firstGregorianDay = 1752;
    fnCurveFitting(CF_LINEAR_FITTING);
    fnLeadingZeros(false);
    fnProductSign(PS_CROSS);
    fnFractionType(FT_PROPER);
    fnRadixMark(RM_PERIOD);
    fnRoundingMode(RM_HALF_EVEN);
    fnDisplayOvr(DO_SCI);
    fnStackSize(SS_4);
    //tDisp = -1;
    fnSetWordSize(64);
    fnDateFormat(DF_YMD);
    fnComplexMode(CM_RECTANGULAR);
    showRealComplexResult();
    allocateLocalRegisters(0);

    displayRealAsFraction = false;
    STACK_LIFT_DISABLE;
    showOverflowCarry();
    hideUserMode();

    // Initialization of user key assignments
    memcpy(kbd_usr, kbd_std, sizeof(kbd_std));

    #ifndef TESTSUITE_BUILD
      while(softmenuStackPointer > 0) {
        popSoftmenu();
      }
    #endif // TESTSUITE_BUILD

    oldTime[0] = 0;

    temporaryInformation = TI_RESET;

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
    //stringToReal16("5.555", REGISTER_REAL16_DATA(FIRST_LOCAL_REGISTER));

    //strcpy(tmpStr3000, "Pure ASCII string requiring 38 bytes!");
    //reallocateRegister(FIRST_LOCAL_REGISTER+1, dtString, strlen(tmpStr3000), 0);
    //strcpy(REGISTER_STRING_DATA(FIRST_LOCAL_REGISTER + 1), tmpStr3000);


    //allocateNamedRegister("Z" STD_a_DIARESIS "hler");
    //allocateNamedRegister(STD_omega STD_SUB_1);
    //allocateNamedRegister(STD_omega STD_SUB_2);

    #if (DEBUG_PANEL == 1)
      debugWindow = DBG_REGISTERS;
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chkHexaString), false);
      refreshDebugPanel();
    #endif

    refreshStack();
  }
}



void backToSystem(uint16_t unusedParamButMandatory) {
  #ifdef PC_BUILD
    fnOff(NOPARAM);
  #endif

  #ifdef DMCP_BUILD
    endOfProgram = true;
  #endif
}
