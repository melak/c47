/* This file is part of WP 43S.
 *
 * WP 43S is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WP 43S is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WP 43S.  If not, see <http://www.gnu.org/licenses/>.
 */

/* ADDITIONAL C43 functions and routines */

/********************************************//**
 * \file addons.c
 ***********************************************/


/*

Math changes:

1. addon.c: Added fnArg_all which uses fnArg, but gives a result of 0 for a real 
   and longint input. The testSuite is not ifluenced. Not needed to modify |x|, 
   as it already works for a real and longint.
   (testSuite not in use for fnArg, therefore also not added)

2. bufferize.c: closenim: changed the default for (0 CC EXIT to 0) instead of i. 
   (testSuite not ifluenced).

3. addon.c: Added functions fnToPolar2 and fnToRect2 which uses the original 
   fnToPolar and fnToRect but first checks if X is a complex value, if it is,
   it does a POLAR or RECT command to change the display mode, and if not, 
   it calls R>P or P>R, using both standard functions.

Todo 


All the below: because both Last x and savestack does not work due to multiple steps.

  5. Added Star > Delta. Change and put in separate c file, and sort out savestack.

  6. vice versa

  7. SYM>ABC

  8. ABC>SYM

  9. e^theta. redo in math file, 

  10. three phase Ohms Law: 17,18,19


 Check for savestack in jm.c





*/





#include "wp43s.h"


void fneRPN(uint16_t state) {
  if(state == 1) eRPN = true;
  else if (state == 0) eRPN = false;
}



void fnCFGsettings(uint16_t unusedButMandatoryParameter) {
  #ifndef TESTSUITE_BUILD
  runFunction(ITM_FF);
  showSoftmenu(-MNU_SYSFL);
  #endif
}



void fnClAIM(uint16_t unusedButMandatoryParameter) {  //clear input buffe
  if(calcMode == CM_NIM) {
    strcpy(aimBuffer,"+");
    fnKeyBackspace(0);
    //printf("|%s|\n",aimBuffer);
  }
  lastIntegerBase = 0;
#ifndef TESTSUITE_BUILD
//  while(softmenuStackPointer > 0 /*softmenuStackPointerBeforeAIM*/ ) {     //JMMENU was 0, to POP OFF ALL MENUS; changed by Martin to before AIM
//JMTOCHECK2
    popSoftmenu();
//  }
  calcModeNormal();
  refreshScreen();
  fnKeyExit(0);      //Call fnkeyExit to ensure the correct home screen is brought up, if HOME is selected.
#endif
}


//fnArg for real and longints in addition to the standard complex. Simply returns 0 angle

void fnArg_all(uint16_t unusedButMandatoryParameter) {
longInteger_t li;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {       //JM vv add the obvious case that a number has 0/180 degrees. Why error for this.
    convertLongIntegerRegisterToLongInteger(REGISTER_X, li);
    if(longIntegerIsPositive(li) || longIntegerIsZero(li)) {
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X));
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE, currentAngularMode);
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
    } 
    else if(longIntegerIsNegative(li)) {
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
      realToReal34(const_180, REGISTER_REAL34_DATA(REGISTER_X));
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE, currentAngularMode);
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
    }
    longIntegerFree(li);
  	adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  }
  else 
  	fnArg(0);
}




void fnToPolar2(uint16_t unusedButMandatoryParameter) {
  if(getRegisterDataType(REGISTER_X) == dtComplex34) {
//    fnComplexMode(CM_POLAR);
      fnSetFlag(FLAG_POLAR);

  }
  else
    fnToPolar(0);
}



void fnToRect2(uint16_t unusedButMandatoryParameter) {
  if(getRegisterDataType(REGISTER_X) == dtComplex34) {
 //     fnComplexMode(CM_RECTANGULAR);
      fnClearFlag(FLAG_POLAR);
    }
    else
      fnToRect(0);
}



void fnRoundi2(uint16_t unusedButMandatoryParameter) {
  if(getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtShortInteger) {
    //nothing
  }
  else
    fnRoundi(0);
}


void fnRound2(uint16_t unusedButMandatoryParameter) {
  if(getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtShortInteger) {
    //nothing
  }
  else
    fnRound(0);
}




void fnTo_ms(uint16_t unusedButMandatoryParameter) {
    copySourceRegisterToDestRegister(REGISTER_L, TEMP_REGISTER);   // STO TMP

    if(getRegisterDataType(REGISTER_X) == dtShortInteger) {convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);}
    if(getRegisterDataType(REGISTER_X) == dtLongInteger)  {convertLongIntegerRegisterToReal34Register (REGISTER_X, REGISTER_X);}
    if(getRegisterDataType(REGISTER_X) == dtReal34) {
      if(getRegisterAngularMode(REGISTER_X) == AM_NONE) {setRegisterAngularMode(REGISTER_X, currentAngularMode);}
      if(getRegisterAngularMode(REGISTER_X) == AM_NONE) {printf("Still ADM AM_NONE. Something is wrong\n");}

      if(getRegisterAngularMode(REGISTER_X) != AM_DMS /*&& getRegisterAngularMode(REGISTER_X) != AM_HMS*/) {
        #ifndef TESTSUITE_BUILD
          runFunction(ITM_toDMS);
        #endif
      }
      else 
      {
/*    if(getRegisterAngularMode(REGISTER_X) == AM_DMS ) {   //JM wait for future HMS
        runFunction(ITM_toHMS); break;
      }
      else { */
      #ifndef TESTSUITE_BUILD
        switch (getRegisterAngularMode(REGISTER_X)) {
          case AM_DEGREE: {runFunction(ITM_DEGto);  } break;
          case AM_DMS   : {runFunction(ITM_DMSto);  } break;
          case AM_GRAD  : {runFunction(ITM_GRADto); } break;
          case AM_RADIAN: {runFunction(ITM_RADto);  } break;
          case AM_MULTPI: {runFunction(ITM_MULPIto);} break;
          default: break;
        }
      #endif
      }
    }
    copySourceRegisterToDestRegister(TEMP_REGISTER, REGISTER_L);   // STO TMP
  }






void fnMultiplySI(uint16_t multiplier) {
    copySourceRegisterToDestRegister(REGISTER_L, TEMP_REGISTER);   // STO TMP
	char mult[20];
	char divi[20];
	mult[0]=0;
	divi[0]=0;
	switch (multiplier) {
		case 100 +  0: strcpy(mult,"1");                     break; //unity
		case 100 +  3: strcpy(mult, "1000");                 break; //kilo
		case 100 +  6: strcpy(mult, "1000000");              break; //mega
		case 100 +  9: strcpy(mult, "1000000000");           break; //giga
		case 100 + 12: strcpy(mult, "1000000000000");        break; //tera
		case 100 + 15: strcpy(mult, "1000000000000000");     break; //peta
		case 100 + 18: strcpy(mult, "1000000000000000000");  break; //exa
		case 100 -  3: strcpy(divi, "1000");                 break; //milli
		case 100 -  6: strcpy(divi, "1000000");              break; //micro
		case 100 -  9: strcpy(divi, "1000000000");           break; //nano
		case 100 - 12: strcpy(divi, "1000000000000");        break; //pico
		case 100 - 15: strcpy(divi, "1000000000000000");     break; //femto
		case 100 - 18: strcpy(divi, "1000000000000000000");  break; //atto
		default: break;
	}

	setSystemFlag(FLAG_ASLIFT);
	liftStack();
	longInteger_t lgInt;
	longIntegerInit(lgInt);

    if(mult[0]!=0) {
        stringToLongInteger(mult + (mult[0] == '+' ? 1 : 0), 10, lgInt);
        convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
        longIntegerFree(lgInt);
        fnMultiply(0);
    } else
    if(divi[0]!=0) {
        stringToLongInteger(divi + (divi[0] == '+' ? 1 : 0), 10, lgInt);
        convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
        longIntegerFree(lgInt);
        fnDivide(0);
    }

    adjustResult(REGISTER_X, false, false, REGISTER_X, REGISTER_Y, -1);
    copySourceRegisterToDestRegister(TEMP_REGISTER, REGISTER_L);   // STO TMP

}




void fn_cnst_op_j(uint16_t unusedButMandatoryParameter) {
  setSystemFlag(FLAG_ASLIFT);
  liftStack();
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(const_1, REGISTER_IMAG34_DATA(REGISTER_X));
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}

void fn_cnst_op_aa(uint16_t unusedButMandatoryParameter) {
  setSystemFlag(FLAG_ASLIFT);
  liftStack();
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(const_1on2, REGISTER_REAL34_DATA(REGISTER_X));  //-0.5 - 0.866
  realToReal34(const_rt3on2, REGISTER_IMAG34_DATA(REGISTER_X));
  chsCplx();
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}

void fn_cnst_op_a(uint16_t unusedButMandatoryParameter) {
  setSystemFlag(FLAG_ASLIFT);
  liftStack();
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(const_1on2, REGISTER_REAL34_DATA(REGISTER_X));  //-0.5 + 0.866i  : op a
  chsReal();
  realToReal34(const_rt3on2, REGISTER_IMAG34_DATA(REGISTER_X));
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}

void fn_cnst_0_cpx(uint16_t unusedButMandatoryParameter) {
  setSystemFlag(FLAG_ASLIFT);
  liftStack();
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X));      // 0+i0
  realToReal34(const_0, REGISTER_IMAG34_DATA(REGISTER_X));
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}

void fn_cnst_1_cpx(uint16_t unusedButMandatoryParameter) {
  setSystemFlag(FLAG_ASLIFT);
  liftStack();
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(const_1, REGISTER_REAL34_DATA(REGISTER_X));      // 0+i0
  realToReal34(const_0, REGISTER_IMAG34_DATA(REGISTER_X));
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}






void JM_convertReal34ToLongInteger(uint16_t confirmation) {
  if(!real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    real34_t x;
    real34ToIntegralValue(REGISTER_REAL34_DATA(REGISTER_X), &x, DEC_ROUND_DOWN);
    real34Subtract(REGISTER_REAL34_DATA(REGISTER_X), &x , &x);
    if(real34IsZero(&x)) { confirmation = CONFIRMED; }
    if(confirmation == NOT_CONFIRMED) {
      setConfirmationMode(JM_convertReal34ToLongInteger);
    }
    else {
//      convertReal34ToLongIntegerRegister(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_X, DEC_ROUND_DOWN);
      ipReal();                                        //This converts real to longint!
    }
  }
}




void fnLongInt (uint16_t unusedButMandatoryParameter) {
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  int32_t dataTypeX = getRegisterDataType(REGISTER_X);

  if(dataTypeX == dtReal34 && getRegisterAngularMode(REGISTER_X) != AM_NONE) {
    fnToReal(0);  
    JM_convertReal34ToLongInteger(CONFIRMED);
  }
  else

  if(dataTypeX == dtShortInteger) {
    convertShortIntegerRegisterToLongIntegerRegister(REGISTER_X, REGISTER_X);
  }
  else

  if(dataTypeX == dtReal34) {
    JM_convertReal34ToLongInteger(CONFIRMED);
  }  
  lastIntegerBase = 0;                                                      //JMNIM clear lastintegerbase, to switch off hex mode
  fnRefreshRadioState(0, 0);                                                //JMNIM
}








//Integral Part
void JM_convertReal34ToShortInteger(uint16_t confirmation) {
  if(!real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))) {
    real34_t x;
    real34ToIntegralValue(REGISTER_REAL34_DATA(REGISTER_X), &x, DEC_ROUND_DOWN);
    real34Subtract(REGISTER_REAL34_DATA(REGISTER_X), &x , &x);
    if(real34IsZero(&x)) { confirmation = CONFIRMED; }
    if(confirmation == NOT_CONFIRMED) {
      setConfirmationMode(JM_convertReal34ToShortInteger);
    }
    else {
      //convertReal34ToLongIntegerRegister(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_X, DEC_ROUND_DOWN);
      ipReal();                                        //This converts real to longint!

      if(lastIntegerBase != 0) {
        fnChangeBase(lastIntegerBase);                 //This converts shortint, longint and real to shortint!
      }
      else {
        fnChangeBase(10);                              //This converts shortint, longint and real to shortint!
      }

    }
  }
}




//Rounding
void fnJM_2SI(uint16_t unusedButMandatoryParameter) {       //Convert Real to Longint; Longint to shortint; shortint to longint


  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      if(lastIntegerBase >= 2 && lastIntegerBase <= 16) {
//        fnChangeBase(lastIntegerBase);                  //This converts shortint, longint and real to shortint!
        convertLongIntegerRegisterToShortIntegerRegister(REGISTER_X, REGISTER_X);
        setRegisterShortIntegerBase(REGISTER_X, lastIntegerBase);
      }
      else {
//        fnChangeBase(10);                               //This converts shortint, longint and real to shortint!
        convertLongIntegerRegisterToShortIntegerRegister(REGISTER_X, REGISTER_X);
        setRegisterShortIntegerBase(REGISTER_X, 10);
      }
      break;
    case dtReal34:
      //ipReal();                                         //This converts real to longint!
      fnRoundi(0);
      break;
    case dtShortInteger:
      convertShortIntegerRegisterToLongIntegerRegister(REGISTER_X, REGISTER_X); //This shortint to longint!
      lastIntegerBase = 0;                                                      //JMNIM clear lastintegerbase, to switch off hex mode
      fnRefreshRadioState(0, 0);                                                //JMNIM
      break;
    default:
      break;
  }
}






/********************************************//**
 * \Set SIGFIG mode
 *
 * FROM DISPLAY.C
 ***********************************************/
void fnDisplayFormatSigFig(uint16_t displayFormatN) {   //DONE          //JM SIGFIG
  displayFormat = DF_FIX;
  displayFormatDigits = displayFormatN;
  clearSystemFlag(FLAG_FRACT);
  SigFigMode = displayFormatN;                                    //JM SIGFIG
  UNITDisplay = false;                                            //JM SIGFIG display Reset

  fnRefreshRadioState(RB_DI, DF_SF);
}                                                                 //JM SIGFIG



/********************************************//**
 * \Set UNIT mode
 *
 * FROM DISPLAY.C
 ***********************************************/
void fnDisplayFormatUnit(uint16_t displayFormatN) {    //DONE           //JM UNIT
  displayFormat = DF_ENG;
  displayFormatDigits = displayFormatN;
  clearSystemFlag(FLAG_FRACT);
  SigFigMode = 0;                                                 //JM UNIT Sigfig works in FIX mode and it makes not sense in UNIT (ENG) mode
  UNITDisplay = true;                                             //JM UNIT display

  fnRefreshRadioState(RB_DI, DF_UN);
// Convert longint to real, to force UNIT to work. 
//  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
//    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
//  }
}                                                                 //JM UNIT



/* JM UNIT********************************************//**                                                JM UNIT
 * \brief Adds the power of 10 using numeric font to displayString                                        JM UNIT
 *        Converts to units like m, M, k, etc.                                                            JM UNIT
 * \param[out] displayString char*     Result string                                                      JM UNIT
 * \param[in]  exponent int32_t Power of 10 to format                                                     JM UNIT
 * \return void                                                                                           JM UNIT
 ***********************************************                                                          JM UNIT */
void exponentToUnitDisplayString(int32_t exponent, char *displayString, char *displayValueString, bool_t nimMode, const char *separator) {               //JM UNIT
  if     (exponent == -15) { displayString[0] = ' '; displayString[1] = 'f'; displayString[2] = 0; }    //JM UNIT
  else if(exponent == -12) { displayString[0] = ' '; displayString[1] = 'p'; displayString[2] = 0; }    //JM UNIT
  else if(exponent == -9 ) { displayString[0] = ' '; displayString[1] = 'n'; displayString[2] = 0; }    //JM UNIT
  else if(exponent == -6 ) { displayString[0] = ' '; displayString[1] = STD_mu[0]; displayString[2] = STD_mu[1]; displayString[3] = 0; }   //JM UNIT
  else if(exponent == -3 ) { displayString[0] = ' '; displayString[1] = 'm'; displayString[2] = 0; }    //JM UNIT
  else if(exponent ==  3 ) { displayString[0] = ' '; displayString[1] = 'k'; displayString[2] = 0; }    //JM UNIT
  else if(exponent ==  6 ) { displayString[0] = ' '; displayString[1] = 'M'; displayString[2] = 0; }    //JM UNIT
  else if(exponent ==  9 ) { displayString[0] = ' '; displayString[1] = 'G'; displayString[2] = 0; }    //JM UNIT
  else if(exponent == 12 ) { displayString[0] = ' '; displayString[1] = 'T'; displayString[2] = 0; }    //JM UNIT
  else {                                                                                                //JM UNIT
    strcpy(displayString, PRODUCT_SIGN);                                                                //JM UNIT Below, copy of
    displayString += 2;                                                                                 //JM UNIT exponentToDisplayString in display.c
    strcpy(displayString, STD_SUB_10);                                                                  //JM UNIT
    displayString += 2;                                                                                 //JM UNIT
    displayString[0] = 0;                                                                               //JM UNIT
    if(nimMode) {                                                                                       //JM UNIT
      if(exponent != 0) {                                                                               //JM UNIT
        supNumberToDisplayString(exponent, displayString, displayValueString, false, separator);                                 //JM UNIT
      }                                                                                                 //JM UNIT
    }                                                                                                   //JM UNIT
    else {                                                                                              //JM UNIT
      supNumberToDisplayString(exponent, displayString, displayValueString, false, separator);                                   //JM UNIT
    }                                                                                                   //JM UNIT
  }                                                                                                     //JM UNIT
}                                                                                                       //JM UNIT




void fnAngularModeJM(uint16_t AMODE) {
  if (AMODE == AM_HMS) AMODE = AM_DMS;
  fnCvtFromCurrentAngularMode(AMODE);
  fnAngularMode(AMODE);
  fnRefreshRadioState(0, 0);

}



void shrinkNimBuffer (void) {                              //JMNIM vv
  int16_t lastChar;                                        //if digits in NIMBUFFER, ensure switch to NIM,
  int16_t hexD = 0;
  bool_t  reached_end = false;
  lastChar = strlen(aimBuffer) - 1;
  if(lastChar >= 1) {
    int16_t ix = 0;
    while(aimBuffer[ix]!=0) {                              //if # found in nimbuffer, return and do nothing
      if(aimBuffer[ix] >= 'A') {
        hexD++;
      }
      if(aimBuffer[ix]=='#' || aimBuffer[ix]=='.' || reached_end) {               //chr(35) = "#"
        aimBuffer[ix]=0; 
        reached_end = true;
        //printf(">>> ***A # found. hexD=%d\n",hexD);
      }
      else {
        //printf(">>> ***B # not found in %s:%d=%d hexD=%d\n",nimBuffer,ix,nimBuffer[ix],hexD);
      }
      ix++;
    }
    if (hexD > 0) nimNumberPart = NP_INT_16;
    else nimNumberPart = NP_INT_10;
//    calcMode = CM_NIM;
  }
}                                                          //JMNIM ^^


void fnChangeBaseJM (uint16_t BASE) {
  //printf(">>> §§§ fnChangeBaseJMa Calmode:%d, nimbuffer:%s, lastbase:%d, nimnumberpart:%d\n",calcMode,nimBuffer,lastIntegerBase, nimNumberPart);
  shrinkNimBuffer();
  //printf(">>> §§§ fnChangeBaseJMb Calmode:%d, nimbuffer:%s, lastbase:%d, nimnumberpart:%d\n",calcMode,nimBuffer,lastIntegerBase, nimNumberPart);

  if(BASE == lastIntegerBase) {
    lastIntegerBase = 0;
    fnRefreshRadioState(0, 0);
  }
  else {
    fnChangeBase(BASE);
  }
  #ifndef TESTSUITE_BUILD
  nimBufferToDisplayBuffer(aimBuffer, nimBufferDisplay + 2);
  #endif
}





void fnChangeBaseMNU(uint16_t BASE) {
#ifndef TESTSUITE_BUILD
    //printf(">>> §§§ fnChangeBaseMNUa Calmode:%d, nimbuffer:%s, lastbase:%d, nimnumberpart:%d\n",calcMode,nimBuffer,lastIntegerBase, nimNumberPart);
    shrinkNimBuffer();
    //printf(">>> §§§ fnChangeBaseMNUb Calmode:%d, nimbuffer:%s, lastbase:%d, nimnumberpart:%d\n",calcMode,nimBuffer,lastIntegerBase, nimNumberPart);

    if(lastIntegerBase == 0 && calcMode == CM_NORMAL && BASE > 1 && BASE <=16) {
      //printf(">>> §§§fnChangeBaseMNc CM_NORMAL: convert non-shortint-mode to %d & return\n",BASE);
      fnChangeBaseJM(BASE);
      return;
    }

    if(lastIntegerBase == 0 && calcMode == CM_NORMAL && BASE == NOPARAM) {
      //printf(">>> §§§fnChangeBaseMNd CM_NORMAL: convert non-shortint-mode to TAM\n");
      runFunction(ITM_toINT);
      return;
    }

    if(lastIntegerBase != 0 && calcMode == CM_NORMAL && BASE == NOPARAM) {
      //printf(">>> §§§fnChangeBaseMNc CM_NORMAL: convert non-shortint-mode to %d & return\n",BASE);
      runFunction(ITM_toINT);
      return;
    }

    if(BASE > 1 && BASE <=16 ) {                                                //BASIC CONVERSION IN X REGISTER, OR DIGITS IN NIMBUFFER IF IN CM_NORMAL
      //printf(">>> §§§1 convert base to %d & return\n",BASE);
      fnChangeBaseJM(BASE);
      nimBufferToDisplayBuffer(aimBuffer, nimBufferDisplay + 2);
      return;
    }

    if(aimBuffer[0]==0 && calcMode == CM_NORMAL && BASE == NOPARAM) {           //IF # FROM MENU & TAM.
      //printf(">>> §§§2 # FROM MENU: nimBuffer[0]=0; use runfunction\n");
      runFunction(ITM_toINT);
      nimBufferToDisplayBuffer(aimBuffer, nimBufferDisplay + 2);
      return;
    }

    if(aimBuffer[0]!=0 && calcMode == CM_NIM) {                                 //IF # FROM MENU, while in NIM, just add to NimBuffer
      //printf(">>> §§§3 # nimBuffer in use, addItemToNimBuffer\n");
      addItemToNimBuffer (ITM_toINT);
      nimBufferToDisplayBuffer(aimBuffer, nimBufferDisplay + 2);
      return;
    }

#endif
}


/********************************************//**
 * \brief Set Input_Default
 *
 * \param[in] inputDefault uint16_t
 * \return void
 ***********************************************/
void fnInDefault(uint16_t inputDefault) {              //DONE
  Input_Default = inputDefault;

  if(Input_Default == ID_SI) {
    lastIntegerBase = 10;
  }
  else {
    lastIntegerBase = 0;
  }

  fnRefreshRadioState(RB_ID, inputDefault);
}



void fnByteShortcutsS(uint16_t size) {                //JM POC BASE2 vv
  fnSetWordSize(size);
  fnIntegerMode(SIM_2COMPL);
}


void fnByteShortcutsU(uint16_t size) {
  fnSetWordSize(size);
  fnIntegerMode(SIM_UNSIGN);  
}

void fnByte(uint16_t command) {
  switch(command) {
    case 1: fnSl(1); break;
    case 2: fnSr(1); break;
    case 3: fnRl(1); break;
    case 4: fnRr(1); break;
    case 5: fnSwapEndian(16);break;            //FWORD
    case 6: fnSwapEndian(8); break;           //FBYTE
    default: break;
  }
}                                                      //JM POC BASE2 ^^






