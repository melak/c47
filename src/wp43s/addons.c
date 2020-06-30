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


//fnArg for real and longints in addition to the standard complex. Simply returns 0 angle

void fnArg_all(uint16_t unusedParamButMandatory) {
longInteger_t li;

  if(getRegisterDataType(REGISTER_X) == dtReal34 || getRegisterDataType(REGISTER_X) == dtLongInteger) {       //JM vv add the obvious case that a number has 0/180 degrees. Why error for this.


    if(getRegisterDataType(REGISTER_X) == dtLongInteger) { 
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
    } 
    else
    if(getRegisterDataType(REGISTER_X) == dtReal34) { 
      if(real34IsPositive(REGISTER_REAL34_DATA(REGISTER_X)) || real34IsZero(REGISTER_REAL34_DATA(REGISTER_X))){
        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
        realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X));
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE, currentAngularMode);
        setRegisterAngularMode(REGISTER_X, currentAngularMode);
      } 
    else if(real34IsNegative(REGISTER_REAL34_DATA(REGISTER_X))) {
        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
        realToReal34(const_180, REGISTER_REAL34_DATA(REGISTER_X));
        convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), AM_DEGREE, currentAngularMode);
        setRegisterAngularMode(REGISTER_X, currentAngularMode);
      }
    }
  	adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  }
  else 
  	fnArg(0);
}



void fnToPolar2(uint16_t unusedParamButMandatory) {
  //saveStack();       //Not savestack because fnToPolar already has savestack and fnComplexMode does not need.
  if(getRegisterDataType(REGISTER_X) == dtComplex34) {
//    fnComplexMode(CM_POLAR);
      fnSetFlag(FLAG_POLAR);

  }
  else
    fnToPolar(0);
}



void fnToRect2(uint16_t unusedParamButMandatory) {
  //saveStack();       //Not savestack because fnToRect already has savestack and fnComplexMode does not need.
  if(getRegisterDataType(REGISTER_X) == dtComplex34) {
 //     fnComplexMode(CM_RECTANGULAR);
      fnClearFlag(FLAG_POLAR);
    }
    else
      fnToRect(0);
}





void fnTo_ms(uint16_t unusedParamButMandatory) {
    saveStack();
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
    refreshRegisterLine(REGISTER_X);
  }






void fnMultiplySI(uint16_t multiplier) {
    //saveStack();  not needed as this is included in mult and div
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

	STACK_LIFT_ENABLE;
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
    refreshRegisterLine(REGISTER_X);

}




void fn_cnst_op_j(uint16_t unusedParamButMandatory) {
  saveStack();
  STACK_LIFT_ENABLE;
  liftStack();
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(const_1, REGISTER_IMAG34_DATA(REGISTER_X));
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  refreshStack();
}

void fn_cnst_op_aa(uint16_t unusedParamButMandatory) {
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(const_1on2, REGISTER_REAL34_DATA(REGISTER_X));  //-0.5 - 0.866
  realToReal34(const_rt3on2, REGISTER_IMAG34_DATA(REGISTER_X));
  fnChangeSign(ITM_CHS);
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  refreshStack();
}

void fn_cnst_op_a(uint16_t unusedParamButMandatory) {
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(const_1on2, REGISTER_REAL34_DATA(REGISTER_X));  //-0.5 + 0.866i  : op a
  fnChangeSign(ITM_CHS);
  realToReal34(const_rt3on2, REGISTER_IMAG34_DATA(REGISTER_X));
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  refreshStack();
}

void fn_cnst_0_cpx(uint16_t unusedParamButMandatory) {
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X));      // 0+i0
  realToReal34(const_0, REGISTER_IMAG34_DATA(REGISTER_X));
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  refreshStack();
}

void fn_cnst_1_cpx(uint16_t unusedParamButMandatory) {
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
  realToReal34(const_1, REGISTER_REAL34_DATA(REGISTER_X));      // 0+i0
  realToReal34(const_0, REGISTER_IMAG34_DATA(REGISTER_X));
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  refreshStack();
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
      refreshStack();
    }
  }
}



/********************************************//**
 * \brief CONVERT DATA TYPES UP
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnJMup(uint16_t unusedParamButMandatory) {
  // >>
  /*
  if Angle mode: change to Real as applicable using .d.
  If SHORTINT: change to Real
  if Real change to LONGINT
  */
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  int32_t dataTypeX = getRegisterDataType(REGISTER_X);

  if(dataTypeX == dtReal34 && getRegisterAngularMode(REGISTER_X) != AM_NONE) {
    fnToReal(0);  
  }
  else

  if(dataTypeX == dtShortInteger) {
    convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
  else

  if(dataTypeX == dtReal34) {
    JM_convertReal34ToLongInteger(CONFIRMED);
  }

  refreshStack();
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

      refreshStack();
    }
  }
}


/********************************************//**
 * \brief CONVERT DATA TYPES DOWN
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnJMdown(uint16_t unusedParamButMandatory) {
  // <<
  /*
  if Angle mode: change Real, as applicable using .d
  If LONGINT: change to Real
  if Real change to ShortInt
  */
  saveStack();
  copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);

  int32_t dataTypeX = getRegisterDataType(REGISTER_X);

  if(dataTypeX == dtReal34 && getRegisterAngularMode(REGISTER_X) != AM_NONE) {
    fnToReal(0);  
  }
  else

  if(dataTypeX == dtLongInteger) {
    convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
  }
  else

  if(dataTypeX == dtReal34) {
    JM_convertReal34ToShortInteger(CONFIRMED);
  }

  refreshStack();
}


//Rounding
void fnJM_2SI(uint16_t unusedParamButMandatory) {       //Convert Real to Longint; Longint to shortint; shortint to longint
  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      if(lastIntegerBase != 0) {
        fnChangeBase(lastIntegerBase);                  //This converts shortint, longint and real to shortint!
      }
      else {
        fnChangeBase(10);                               //This converts shortint, longint and real to shortint!
      }
      break;
    case dtReal34:
      //ipReal();                                         //This converts real to longint!
      fnRoundi(0);
      break;
    case dtShortInteger:
      convertShortIntegerRegisterToLongIntegerRegister(REGISTER_X, REGISTER_X); //This shortint to longint!
      break;
    default:
      break;
  }
  refreshStack();
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

  refreshStack();
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
  refreshStack();
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

