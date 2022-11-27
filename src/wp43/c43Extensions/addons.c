/* This file is part of WP43.
 *
 * WP43 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WP43 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WP43.  If not, see <http://www.gnu.org/licenses/>.
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

#include "c43Extensions/addons.h"

#include "assign.h"
#include "bufferize.h"
#include "c43Extensions/keyboardTweak.h"
#include "calcMode.h"
#include "charString.h"
#include "config.h"
#include "constantPointers.h"
#include "conversionAngles.h"
#include "curveFitting.h"
#include "dateTime.h"
#include "debug.h"
#include "display.h"
#include "error.h"
#include "flags.h"
#include "fonts.h"
#include "c43Extensions/graphText.h"
#include "hal/gui.h"
#include "integers.h"
#include "items.h"
#include "c43Extensions/jm.h"
#include "keyboard.h"
//#include "mathematics/mathematics.h"
#include "mathematics/arg.h"
#include "mathematics/changeSign.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/division.h"
#include "mathematics/multiplication.h"
#include "mathematics/round.h"
#include "mathematics/roundi.h"
#include "mathematics/toPolar.h"
#include "mathematics/toRect.h"
#include "plotstat.h"
#include "c43Extensions/radioButtonCatalog.h"
#include "c43Extensions/keyboardTweak.h"
#include "realType.h"
#include "recall.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "logicalOps/rotateBits.h"
#include "screen.h"
#include "softmenus.h"
#include "stack.h"
#include "statusBar.h"
#include "c43Extensions/textfiles.h"
#include <string.h>

#include "wp43.h"



TO_QSPI void fnPlotStatJM(uint16_t mode) {
  fnCurveFitting(0);
  fnPlotStat(mode);
}


TO_QSPI void fneRPN(uint16_t state) {
  if(state == 1)
    eRPN = true;
  else if(state == 0)
    eRPN = false;
}




TO_QSPI void fnShoiXRepeats(uint16_t numberOfRepeats) {           //JM SHOIDISP
  displayStackSHOIDISP = numberOfRepeats;                 //   0-3
  fnRefreshState();
/*
  if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
    fnChangeBaseJM(getRegisterTag(REGISTER_X));
  } else {
    if(lastIntegerBase > 1 && lastIntegerBase <= 16) {
      fnChangeBaseJM(lastIntegerBase);
    }
  }
*/
}


TO_QSPI void fnCFGsettings(uint16_t unusedButMandatoryParameter) {
#ifndef TESTSUITE_BUILD
  runFunction(ITM_FF);
  showSoftmenu(-MNU_SYSFL);
#endif
}


TO_QSPI void fnClrMod(uint16_t unusedButMandatoryParameter) {        //clear input buffe
  #ifdef PC_BUILD
    jm_show_comment("^^^^fnClrModa");
  #endif //PC_BUILD
  #ifndef TESTSUITE_BUILD
    resetKeytimers();  //JM

    temporaryInformation = TI_NO_INFO;
    if(calcMode == CM_NIM) {
      strcpy(aimBuffer, "+");
      fnKeyBackspace(0);
      //printf("|%s|\n",aimBuffer);
    }
    lastIntegerBase = 0;

  #ifndef TESTSUITE_BUILD
    uint_fast8_t ix = 0;
    while(ix < SOFTMENU_STACK_SIZE && softmenuStack[0].softmenuId != 0) {
    #ifdef PC_BUILD
      jm_show_comment("^^^^fnClrModb");
    #endif //PC_BUILD
      popSoftmenu();
      ix++;
    }

    if(displayStack_m >= 1 && displayStack_m <= 4 /*&& displayStack_m != 255*/) { //JMSHOI
      fnDisplayStack(displayStack_m);     //JMSHOI
      displayStack_m = 255;               //JMSHOI
    }
    else {
      fnDisplayStack(4);                  //removed because it clamps DSTACK to 4
    }                                     //JMSHOI

    calcModeNormal();
    refreshScreen();
    fnKeyExit(0);                         //Call fnkeyExit to ensure the correct home screen is brought up, if HOME is selected.
    popSoftmenu();
  #endif
#endif //TESTSUITE_BUILD
}

//fnArg for real and longints in addition to the standard complex. Simply returns 0 angle

TO_QSPI void fnArg_all(uint16_t unusedButMandatoryParameter) {
  longInteger_t li;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {    //JM vv add the obvious case that a number has 0/180 degrees. Why error for this.
    convertLongIntegerRegisterToLongInteger(REGISTER_X, li);
    if(longIntegerIsPositive(li) || longIntegerIsZero(li)) {
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X));
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), amDegree, currentAngularMode);
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
    }
    else if(longIntegerIsNegative(li)) {
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      realToReal34(const_180, REGISTER_REAL34_DATA(REGISTER_X));
      convertAngle34FromTo(REGISTER_REAL34_DATA(REGISTER_X), amDegree, currentAngularMode);
      setRegisterAngularMode(REGISTER_X, currentAngularMode);
    }
    longIntegerFree(li);
    adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  }
  else
    fnArg(0);
}


TO_QSPI void fnToPolar2(uint16_t unusedButMandatoryParameter) {
  if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    //    fnComplexMode(CM_POLAR);
    fnSetFlag(FLAG_POLAR);
  }
  else
    fnToPolar(0);
}


TO_QSPI void fnToRect2(uint16_t unusedButMandatoryParameter) {
  if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    //     fnComplexMode(CM_RECTANGULAR);
    fnClearFlag(FLAG_POLAR);
  }
  else
    fnToRect(0);
}

TO_QSPI void fnRoundi2(uint16_t unusedButMandatoryParameter) {
  if(getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtShortInteger) {
    //nothing
  }
  else
    fnRoundi(0);
}


TO_QSPI void fnRound2(uint16_t unusedButMandatoryParameter) {
  if(getRegisterDataType(REGISTER_X) == dtLongInteger || getRegisterDataType(REGISTER_X) == dtShortInteger) {
    //nothing
  }
  else
    fnRound(0);
}

//=-=-=-=-=-=-==-=-
//input is time or DMS
//output is sexagesima coded decimal ddd.mmsssssss in the form of a normal decimal
TO_QSPI void fnFrom_ms(uint16_t unusedButMandatoryParameter){
  #ifndef TESTSUITE_BUILD
    char tmpString100[100];
    char tmpString100_OUT[100];
    tmpString100[0]=0;
    tmpString100_OUT[0]=0;
     
    if(getRegisterDataType(REGISTER_X) == dtTime) {
      temporaryInformation = TI_FROM_MS_TIME;
    }
    else if(getRegisterDataType(REGISTER_X) == dtReal34 && getRegisterAngularMode(REGISTER_X) == amDMS) {
      temporaryInformation = TI_FROM_MS_DEG;
    }
    else {
      temporaryInformation = TI_NO_INFO;
    }

    if(temporaryInformation != TI_NO_INFO) {
      if(temporaryInformation == TI_FROM_MS_TIME) {
        copyRegisterToClipboardString2(REGISTER_X, tmpString100);
      }
      if(temporaryInformation == TI_FROM_MS_DEG) {
        real34ToDisplayString(REGISTER_REAL34_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X), tmpString100, &standardFont, SCREEN_WIDTH, NUMBER_OF_DISPLAY_DIGITS, false, " ", true);
        int16_t tmp_i = 0;
        while(tmpString100[tmp_i] != 0 && tmpString100[tmp_i+1] != 0) { //pre-condition the dd.mmssss to replaxce spaces with zeroes
          //printf("%c %d",tmpString100[tmp_i],tmpString100[tmp_i]);
          if((uint8_t)tmpString100[tmp_i] == 128 && (uint8_t)tmpString100[tmp_i+1] == 176) {
            tmpString100[tmp_i] = ' ';
            tmpString100[tmp_i+1] = 'o';
          }
          if((uint8_t)tmpString100[tmp_i] == 'o' && (uint8_t)tmpString100[tmp_i+1] == ' ') {
            tmpString100[tmp_i+1] = '0';
          }
          if((uint8_t)tmpString100[tmp_i] == ':' && (uint8_t)tmpString100[tmp_i+1] == ' ') {
            tmpString100[tmp_i+1] = '0';
          }
          if((uint8_t)tmpString100[tmp_i] == '\'' && (uint8_t)tmpString100[tmp_i+1] == ' ') {
            tmpString100[tmp_i+1] = '0';
          }
          tmp_i++;
        }
      }

      //printf(" ------- 002 >>>%s<<<\n",tmpString100);

      int16_t tmp_j, tmp_i;
      tmp_i = tmp_j = 0;
      bool_t decimalflag = false;
      while(tmpString100[tmp_i] != 0) {
      //printf("%c %d",(uint8_t)tmpString100[tmp_i],(uint8_t)tmpString100[tmp_i]);
        switch ((uint8_t)tmpString100[tmp_i]) {
          case '0' :
          case '1' :
          case '2' :
          case '3' :
          case '4' :
          case '5' :
          case '6' :
          case '7' :
          case '8' :
          case '9' :
          case '+' :
          case '-' :
            //printf("-\n");
            tmpString100_OUT[tmp_j]=(uint8_t)tmpString100[tmp_i];
            tmpString100_OUT[++tmp_j]=0;
            break;
          case 'o' :
          case ':' :
          case '.' :
          case ',' :
            if(!decimalflag) {
              //printf("decimal\n");
              decimalflag = true;
              tmpString100_OUT[tmp_j]='.';
              tmpString100_OUT[++tmp_j]=0;
            }
            break;
          default:
            //printf("ignore.\n");
            break;
          }
          tmp_i++;
        }
      if(tmpString100_OUT[0] != 0) {
        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
        stringToReal34(tmpString100_OUT,REGISTER_REAL34_DATA(REGISTER_X));
        printf("\n ------- 003 >>>%s<<<\n",tmpString100_OUT);
      }
    }

    
//    stringToReal(tmpString100,&value,&ctxtReal39);
#endif //TESTSUITE_BUILD
}


/*
* If in direct entry, accept h.ms, example 1.23 [.ms] would be 1:23:00. Do not change the ADM.
* If closed in X: and X is REAL/integer, then convert this to h.ms. Do not change the ADM.
* If closed in X: and X is already a Time in visible hms like 1:23:45, then change the time to REAL, then tag the REAL with d.ms (‘’) in the form 1°23’45.00’’. Do not change the ADM.
* if closed in X: and X is already d.ms, then convert X to time in h:ms.Do not change the ADM.
*/
//
TO_QSPI void fnTo_ms(uint16_t unusedButMandatoryParameter) {
#ifndef TESTSUITE_BUILD
  switch(calcMode) { //JM
  case CM_NIM:
    addItemToNimBuffer(ITM_ms);
    break;

  case CM_NORMAL:
    copySourceRegisterToDestRegister(REGISTER_L, TEMP_REGISTER_1); // STO TMP

    switch(getRegisterDataType(REGISTER_X)) { //if integer, make a real
    case dtShortInteger:
      convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      break;
    case dtLongInteger:
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      break;
    default:
      break;
    }

    if(getRegisterDataType(REGISTER_X) == dtReal34) {
      if(getRegisterAngularMode(REGISTER_X) == amDMS || getRegisterAngularMode(REGISTER_X) == amDegree) {
        fnKeyDotD(0);
      }
      if(getRegisterAngularMode(REGISTER_X) == amNone) {
        fnToHms(0);
      }
    }
    else if(getRegisterDataType(REGISTER_X) == dtTime) {
      fnToHr(0);
      setRegisterAngularMode(REGISTER_X, amDegree);
      fnCvtFromCurrentAngularMode(amDMS);
    }

    copySourceRegisterToDestRegister(TEMP_REGISTER_1, REGISTER_L); // STO TMP
    break;

  case CM_REGISTER_BROWSER:
  case CM_FLAG_BROWSER:
  case CM_FONT_BROWSER:
  case CM_PLOT_STAT:
  case CM_LISTXY: //JM
  case CM_GRAPH:  //JM
    break;

  default:
    sprintf(errorMessage, "In function fnTo_ms: unexpected calcMode value (%" PRIu8 ") while processing key .ms!", calcMode);
    displayBugScreen(errorMessage);
  }
#endif // !TESTSUITE_BUILD
}


TO_QSPI void addzeroes(char *st, uint8_t ix) {
  uint_fast8_t iy;
  strcpy(st, "1");
  for(iy = 0; iy < ix; iy++) {
    strcat(st, "0");
  }
}

TO_QSPI void fnMultiplySI(uint16_t multiplier)
{
  copySourceRegisterToDestRegister(REGISTER_L, TEMP_REGISTER_1); // STO TMP
  char mult[20];
  char divi[20];
  mult[0] = 0;
  divi[0] = 0;
  if(multiplier > 100 && multiplier <= 100 + 18) addzeroes(mult, multiplier - 100); else //JM optimized
  if(multiplier < 100 && multiplier >= 100 - 18) addzeroes(divi, 100 - multiplier); else //JM optimized
  if(multiplier == 100) strcpy(mult, "1");  //JM optimized
/* JM optimized
  switch(multiplier) {
  case 100 + 0:     strcpy(mult, "1");      break; //unity
  case 100 + 3:     addzeroes(mult, 3);     break; //kilo
  case 100 + 6:     addzeroes(mult, 6);     break; //mega
  case 100 + 9:     addzeroes(mult, 9);     break; //giga
  case 100 + 12:    addzeroes(mult, 12);    break; //tera
  case 100 + 15:    addzeroes(mult, 15);    break; //peta
  case 100 + 18:    addzeroes(mult, 18);    break; //exa
  case 100 - 3:     addzeroes(divi, 3);     break; //milli
  case 100 - 6:     addzeroes(divi, 6);     break; //micro
  case 100 - 9:     addzeroes(divi, 9);     break; //nano
  case 100 - 12:    addzeroes(divi, 12);    break; //pico
  case 100 - 15:    addzeroes(divi, 15);    break; //femto
  case 100 - 18:    addzeroes(divi, 18);    break; //atto
  default:    break;
  }
*/
  setSystemFlag(FLAG_ASLIFT);
  liftStack();
  longInteger_t lgInt;
  longIntegerInit(lgInt);

  if(mult[0] != 0) {
    stringToLongInteger(mult + (mult[0] == '+' ? 1 : 0), 10, lgInt);
    convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
    longIntegerFree(lgInt);
    fnMultiply(0);
  }
  else if(divi[0] != 0) {
    stringToLongInteger(divi + (divi[0] == '+' ? 1 : 0), 10, lgInt);
    convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
    longIntegerFree(lgInt);
    fnDivide(0);
  }

  adjustResult(REGISTER_X, false, false, REGISTER_X, REGISTER_Y, -1);
  copySourceRegisterToDestRegister(TEMP_REGISTER_1, REGISTER_L); // STO TMP
}


static TO_QSPI void cpxToStk(const real_t *real1, const real_t *real2) {
  setSystemFlag(FLAG_ASLIFT);
  liftStack();
  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
  realToReal34(real1, REGISTER_REAL34_DATA(REGISTER_X));
  realToReal34(real2, REGISTER_IMAG34_DATA(REGISTER_X));
  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}

TO_QSPI void fn_cnst_op_j(uint16_t unusedButMandatoryParameter) {
  cpxToStk(const_0, const_1);
//  setSystemFlag(FLAG_ASLIFT);
//  liftStack();
//  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
//  realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X));
//  realToReal34(const_1, REGISTER_IMAG34_DATA(REGISTER_X));
//  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}


TO_QSPI void fn_cnst_op_aa(uint16_t unusedButMandatoryParameter) {
  cpxToStk(const_1on2, const_root3on2);
  chsCplx();
//  setSystemFlag(FLAG_ASLIFT);
//  liftStack();
//  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
//  realToReal34(const_1on2, REGISTER_REAL34_DATA(REGISTER_X)); //-0.5 - 0.866
//  realToReal34(const_root3on2, REGISTER_IMAG34_DATA(REGISTER_X));
//  chsCplx();
//  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}


TO_QSPI void fn_cnst_op_a(uint16_t unusedButMandatoryParameter) {
  cpxToStk(const_1on2, const_root3on2);
  chsReal();
//  setSystemFlag(FLAG_ASLIFT);
//  liftStack();
//  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
//  realToReal34(const_1on2, REGISTER_REAL34_DATA(REGISTER_X)); //-0.5 + 0.866i  : op a
//  chsReal();
//  realToReal34(const_root3on2, REGISTER_IMAG34_DATA(REGISTER_X));
//  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}


TO_QSPI void fn_cnst_0_cpx(uint16_t unusedButMandatoryParameter) {
  cpxToStk(const_0, const_0);
//  setSystemFlag(FLAG_ASLIFT);
//  liftStack();
//  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
//  realToReal34(const_0, REGISTER_REAL34_DATA(REGISTER_X)); // 0+i0
//  realToReal34(const_0, REGISTER_IMAG34_DATA(REGISTER_X));
//  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}


TO_QSPI void fn_cnst_1_cpx(uint16_t unusedButMandatoryParameter) {
  cpxToStk(const_1, const_0);
//  setSystemFlag(FLAG_ASLIFT);
//  liftStack();
//  reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
//  realToReal34(const_1, REGISTER_REAL34_DATA(REGISTER_X)); // 0+i0
//  realToReal34(const_0, REGISTER_IMAG34_DATA(REGISTER_X));
//  adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
}



//Rounding
TO_QSPI void fnJM_2SI(uint16_t unusedButMandatoryParameter) { //Convert Real to Longint; Longint to shortint; shortint to longint

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
    fnRefreshState();                                                         //JMNIM
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
TO_QSPI void fnDisplayFormatSigFig(uint16_t displayFormatN) { //DONE          //JM SIGFIG
  displayFormat = DF_FIX;
  displayFormatDigits = displayFormatN;
  clearSystemFlag(FLAG_FRACT);
  constantFractionsOn = false; //JM
  SigFigMode = displayFormatN; //JM SIGFIG
  UNITDisplay = false;         //JM SIGFIG display Reset
  DM_Cycling = 0;

  fnRefreshState();
} //JM SIGFIG

/********************************************//**
 * \Set UNIT mode
 *
 * FROM DISPLAY.C
 ***********************************************/
TO_QSPI void fnDisplayFormatUnit(uint16_t displayFormatN) { //DONE           //JM UNIT
  displayFormat = DF_ENG;
  displayFormatDigits = displayFormatN;
  clearSystemFlag(FLAG_FRACT);
  constantFractionsOn = false; //JM
  SigFigMode = 0;     //JM UNIT Sigfig works in FIX mode and it makes not sense in UNIT (ENG) mode
  UNITDisplay = true; //JM UNIT display
  DM_Cycling = 0;


  fnRefreshState();
  // Convert longint to real, to force UNIT to work.
//if (getRegisterDataType(REGISTER_X) == dtLongInteger) {
//  convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
//}
} //JM UNIT

/* JM UNIT********************************************//**                                                JM UNIT
 * \brief Adds the power of 10 using numeric font to displayString                                        JM UNIT
 *        Converts to units like m, M, k, etc.                                                            JM UNIT
 * \param[out] displayString char*     Result string                                                      JM UNIT
 * \param[in]  exponent int32_t Power of 10 to format                                                     JM UNIT
 * \return void                                                                                           JM UNIT
 ***********************************************                                                          JM UNIT */
TO_QSPI void exponentToUnitDisplayString(int32_t exponent, char *displayString, char *displayValueString, bool_t nimMode, const char *separator) {               //JM UNIT
  displayString[0] = ' ';
  displayString[1] = 0;
  displayString[2] = 0;

  if(SI_All) {
    switch(exponent) {
      case -30 : displayString[1] = 'q'; break;
      case -27 : displayString[1] = 'r'; break;
      case -24 : displayString[1] = 'y'; break;
      case -21 : displayString[1] = 'z'; break;
      case -18 : displayString[1] = 'a'; break;
      case  18 : displayString[1] = 'E'; break;
      case  21 : displayString[1] = 'Z'; break;
      case  24 : displayString[1] = 'Y'; break;
      case  27 : displayString[1] = 'R'; break;
      case  30 : displayString[1] = 'Q'; break;
      default: break;
    }
  }

  switch(exponent) {
    case -15 : displayString[1] = 'f'; break;
    case -12 : displayString[1] = 'p'; break;
    case -9  : displayString[1] = 'n'; break;
    case -6  : displayString[1] = STD_mu[0]; displayString[2] = STD_mu[1]; displayString[3] = 0;  break;   //JM UNIT
    case -3  : displayString[1] = 'm'; break;
    case  3  : displayString[1] = 'k'; break;
    case  6  : displayString[1] = 'M'; break;
    case  9  : displayString[1] = 'G'; break;
    case 12  : displayString[1] = 'T'; break;
    case 15  : displayString[1] = 'P'; break;
    default: break;
  }

  if(displayString[1] == 0) {
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


TO_QSPI void fnDisplayFormatCycle (uint16_t unusedButMandatoryParameter) {
  if(DM_Cycling == 0 && softmenu[softmenuStack[0].softmenuId].menuItem == -MNU_HOME) {
    fnDisplayFormatUnit(displayFormatDigits);
  } else
  if(displayFormat == DF_ALL) {
    fnDisplayFormatSigFig(displayFormatDigits);
  } else
  if(displayFormat == DF_FIX && SigFigMode != 0 ) { //DF_SF)
    fnDisplayFormatUnit(displayFormatDigits);
  } else
  if(displayFormat == DF_ENG && UNITDisplay) { //DF_UN)
    fnDisplayFormatFix(displayFormatDigits);
  } else
  if(displayFormat == DF_FIX && SigFigMode == 0) {
    fnDisplayFormatSci(displayFormatDigits);
  } else
  if(displayFormat == DF_SCI) {
    fnDisplayFormatEng(displayFormatDigits); 
  } else
  if(displayFormat == DF_ENG && !UNITDisplay) {
    fnDisplayFormatAll(displayFormatDigits);
  }
  DM_Cycling = 1;
}



//change the current state from the old state?

TO_QSPI void fnAngularModeJM(uint16_t AMODE) { //Setting to HMS does not change AM

  copySourceRegisterToDestRegister(REGISTER_X, TEMP_REGISTER_1);
  if(AMODE == TM_HMS) {
    if(getRegisterDataType(REGISTER_X) == dtTime)
      goto to_return;
    if(getRegisterDataType(REGISTER_X) == dtReal34 && getRegisterAngularMode(REGISTER_X) != amNone)
      fnCvtFromCurrentAngularMode(amDegree);
    fnKeyDotD(0);
    fnToHms(0); //covers longint & real
  }
  else {
    if(getRegisterDataType(REGISTER_X) == dtTime) {
      fnToHr(0); //covers time
      setRegisterAngularMode(REGISTER_X, amDegree);
      fnCvtFromCurrentAngularMode(AMODE);
      //fnAngularMode(AMODE);                             Remove updating of ADM to the same mode
    }

    if((getRegisterDataType(REGISTER_X) != dtReal34) || ((getRegisterDataType(REGISTER_X) == dtReal34) && getRegisterAngularMode(REGISTER_X) == amNone)) {
      fnKeyDotD(0); //convert longint, and strip all angles to real.
      uint16_t currentAngularModeOld = currentAngularMode;
      fnAngularMode(AMODE);
      fnCvtFromCurrentAngularMode(currentAngularMode);
      currentAngularMode = currentAngularModeOld;       //Remove updating of ADM to the same mode
    }
    else { //convert existing tagged angle, and set the ADM
      fnCvtFromCurrentAngularMode(AMODE);
      //fnAngularMode(AMODE);                             Remove updating of ADM to the same mode
    }
  }
#ifndef TESTSUITE_BUILD
  fnRefreshState();
  refreshStatusBar();
#endif //!TESTSUITE_BUILD

  to_return:
  copySourceRegisterToDestRegister(TEMP_REGISTER_1, REGISTER_L);
}


TO_QSPI void fnDRG(uint16_t unusedButMandatoryParameter) {
  if(getRegisterDataType(REGISTER_X) == dtComplex34) {
    goto to_return;
  } 
printf("@@@@\n");
  copySourceRegisterToDestRegister(REGISTER_X, TEMP_REGISTER_1);
  uint16_t dest = 9999;

  if(getRegisterDataType(REGISTER_X) == dtShortInteger) {                  // If shortinteger in X, convert to real
    convertShortIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
    setRegisterAngularMode(REGISTER_X, amNone); //is probably none already
  } else if(getRegisterDataType(REGISTER_X) == dtLongInteger) {                   // If longinteger in X, convert to real
      convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
      setRegisterAngularMode(REGISTER_X, amNone); //is probably none already
    }

  if(getRegisterDataType(REGISTER_X) == dtReal34) {                        // if real
    dest = getRegisterAngularMode(REGISTER_X);

    if(dest != amNone && dest != currentAngularMode && DRG_Cycling != 1) {   //first step: convert tagged angle to ADM
      fnCvtToCurrentAngularMode(dest);
      goto to_return;
    }

    DRG_Cycling = 1;
    switch(dest) {
      case amNone:      dest = currentAngularMode;  break; //converts from to the same, i.e. get to current angle mode
      case amRadian:    dest = amGrad;              break;
      case amGrad:      dest = amDegree;            break;
      case amDegree:    dest = amRadian;            break;
      case amDMS:       dest = amDegree;            break;
      case amMultPi:    dest = amRadian;            break; //do not support Mulpi but at least get out of it
      default:      break;
    }
    fnCvtFromCurrentAngularMode(dest);
    //currentAngularMode = dest;          //remove setting of ADM!
  }

/* Remove complex number support and cycling 8-level stack support
  else //if(getRegisterDataType(REGISTER_X) == dtComplex34)
  {
    dest = currentAngularMode;
    switch(dest) {
      case amNone:      dest = currentAngularMode;  break; //converts from to the same, i.e. get to current angle mode
      case amRadian:    dest = amGrad;              break;
      case amGrad:      dest = amDegree;            break;
      case amDegree:    dest = amRadian;            break;
      case amDMS:       dest = amDegree;            break;
      case amMultPi:    dest = amRadian;            break; //do not support Mulpi but at least get out of it
      default:      break;
    }
    currentAngularMode = dest;
  }

  if(dest != 9999) {
    if(getRegisterDataType(REGISTER_Y) == dtReal34 && getRegisterAngularMode(REGISTER_Y) != amNone) fnCvtFromCurrentAngularModeRegister(REGISTER_Y, dest);
    if(getRegisterDataType(REGISTER_Z) == dtReal34 && getRegisterAngularMode(REGISTER_Z) != amNone) fnCvtFromCurrentAngularModeRegister(REGISTER_Z, dest);
    if(getRegisterDataType(REGISTER_T) == dtReal34 && getRegisterAngularMode(REGISTER_T) != amNone) fnCvtFromCurrentAngularModeRegister(REGISTER_T, dest);
    if(getRegisterDataType(REGISTER_A) == dtReal34 && getRegisterAngularMode(REGISTER_A) != amNone) fnCvtFromCurrentAngularModeRegister(REGISTER_A, dest);
    if(getRegisterDataType(REGISTER_B) == dtReal34 && getRegisterAngularMode(REGISTER_B) != amNone) fnCvtFromCurrentAngularModeRegister(REGISTER_B, dest);
    if(getRegisterDataType(REGISTER_C) == dtReal34 && getRegisterAngularMode(REGISTER_C) != amNone) fnCvtFromCurrentAngularModeRegister(REGISTER_C, dest);
    if(getRegisterDataType(REGISTER_D) == dtReal34 && getRegisterAngularMode(REGISTER_D) != amNone) fnCvtFromCurrentAngularModeRegister(REGISTER_D, dest);
  }
*/
  to_return:
    copySourceRegisterToDestRegister(TEMP_REGISTER_1, REGISTER_L);
}


TO_QSPI void fnDRGto(uint16_t unusedButMandatoryParameter) {
  fnDRG(0);
  fnAngularMode(getRegisterAngularMode(REGISTER_X));
}

TO_QSPI void shrinkNimBuffer(void) {                      //JMNIM vv
  int16_t lastChar; //if digits in NIMBUFFER, ensure switch to NIM,
  int16_t hexD = 0;
  bool_t reached_end = false;
  lastChar = strlen(aimBuffer) - 1;
  if(lastChar >= 1) {
    uint_fast16_t ix = 0;
    while(aimBuffer[ix] != 0) { //if # found in nimbuffer, return and do nothing
      if(aimBuffer[ix] >= 'A') {
        hexD++;
      }
      if(aimBuffer[ix] == '#' || aimBuffer[ix] == '.' || reached_end) { //chr(35) = "#"
        aimBuffer[ix] = 0;
        reached_end = true;
        //printf(">>> ***A # found. hexD=%d\n",hexD);
      }
      else {
        //printf(">>> ***B # not found in %s:%d=%d hexD=%d\n",nimBuffer,ix,nimBuffer[ix],hexD);
      }
      ix++;
    }
    if(hexD > 0)
      nimNumberPart = NP_INT_16;
    else
      nimNumberPart = NP_INT_10;
//   calcMode = CM_NIM;
  }
}                                                 //JMNIM ^^


TO_QSPI void fnChangeBaseJM(uint16_t BASE) {
  //printf(">>> §§§ fnChangeBaseJMa Calmode:%d, nimbuffer:%s, lastbase:%d, nimnumberpart:%d\n", calcMode, nimBuffer, lastIntegerBase, nimNumberPart);
  shrinkNimBuffer();
  //printf(">>> §§§ fnChangeBaseJMb Calmode:%d, nimbuffer:%s, lastbase:%d, nimnumberpart:%d\n", calcMode, nimBuffer, lastIntegerBase, nimNumberPart);

  if(BASE == lastIntegerBase) {
    lastIntegerBase = 0;
    fnRefreshState();
  }
  else {
    fnChangeBase(BASE);
  }
#ifndef TESTSUITE_BUILD
  nimBufferToDisplayBuffer(aimBuffer, nimBufferDisplay + 2);
#endif
}


TO_QSPI void fnChangeBaseMNU(uint16_t BASE) {
#ifndef TESTSUITE_BUILD

  if(calcMode == CM_AIM) {
    addItemToBuffer(ITM_toINT);
    return;
  }

  //printf(">>> §§§ fnChangeBaseMNUa Calmode:%d, nimbuffer:%s, lastbase:%d, nimnumberpart:%d\n", calcMode, nimBuffer, lastIntegerBase, nimNumberPart);
  shrinkNimBuffer();
  //printf(">>> §§§ fnChangeBaseMNUb Calmode:%d, nimbuffer:%s, lastbase:%d, nimnumberpart:%d\n", calcMode, nimBuffer, lastIntegerBase, nimNumberPart);

  if(lastIntegerBase == 0 && calcMode == CM_NORMAL && BASE > 1 && BASE <= 16) {
    //printf(">>> §§§fnChangeBaseMNc CM_NORMAL: convert non-shortint-mode to %d & return\n", BASE);
    fnChangeBaseJM(BASE);
    return;
  }

  if(calcMode == CM_NORMAL && BASE == NOPARAM) {
    //printf(">>> §§§fnChangeBaseMNd CM_NORMAL: convert non-shortint-mode to TAM\n");
    runFunction(ITM_toINT);
    return;
  }

  if(BASE > 1 && BASE <= 16) { //BASIC CONVERSION IN X REGISTER, OR DIGITS IN NIMBUFFER IF IN CM_NORMAL
    //printf(">>> §§§1 convert base to %d & return\n", BASE);
    fnChangeBaseJM(BASE);
    nimBufferToDisplayBuffer(aimBuffer, nimBufferDisplay + 2);
    return;
  }

  if(aimBuffer[0] == 0 && calcMode == CM_NORMAL && BASE == NOPARAM) { //IF # FROM MENU & TAM.
    //printf(">>> §§§2 # FROM MENU: nimBuffer[0]=0; use runfunction\n");
    runFunction(ITM_toINT);
    nimBufferToDisplayBuffer(aimBuffer, nimBufferDisplay + 2);
    return;
  }

  if(aimBuffer[0] != 0 && calcMode == CM_NIM) { //IF # FROM MENU, while in NIM, just add to NimBuffer
    //printf(">>> §§§3 # nimBuffer in use, addItemToNimBuffer\n");
    addItemToNimBuffer(ITM_toINT);
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
TO_QSPI void fnInDefault(uint16_t inputDefault) { //DONE
  Input_Default = inputDefault;

  if(Input_Default == ID_SI) {
    lastIntegerBase = 10;
  }
  else {
    lastIntegerBase = 0;
  }

  fnRefreshState();
}


TO_QSPI void fnByteShortcutsS(uint16_t size) { //JM POC BASE2 vv
  fnSetWordSize(size);
  fnIntegerMode(SIM_2COMPL);
}


TO_QSPI void fnByteShortcutsU(uint16_t size) {
  fnSetWordSize(size);
  fnIntegerMode(SIM_UNSIGN);
}


TO_QSPI void fnByte(uint16_t command) {
  switch(command) {
  case 1:     fnSl(1);            break;
  case 2:     fnSr(1);            break;
  case 3:     fnRl(1);            break;
  case 4:     fnRr(1);            break;
  case 5:     fnSwapEndian(16);   break; //FWORD
  case 6:     fnSwapEndian(8);    break; //FBYTE
  default:                        break;
  }
} //JM POC BASE2 ^^


TO_QSPI void fnP_All_Regs(uint16_t option) {

#ifndef TESTSUITE_BUILD
  if(calcMode != CM_NORMAL) {
    #ifdef DMCP_BUILD
    beep(440, 50);
    beep(4400, 50);
    beep(440, 50);
    #endif //DMCP_BUILD
    return;
  }

  create_filename(".REGS.TSV");

#if (VERBOSE_LEVEL >= 1)
  clearScreen();
  print_linestr("Output regs to drive:", true);
  print_linestr(filename_csv, false);
#endif

  switch(option) {
  case 0: {                   //All registers
    stackregister_csv_out(REGISTER_X, REGISTER_D);
    stackregister_csv_out(REGISTER_L, REGISTER_K);
    stackregister_csv_out(0, 99);
//stackregister_csv_out(FIRST_LOCAL_REGISTER,FIRST_LOCAL_REGISTER+100);
  }
  break;

  case 1: {                   //Stack only
    stackregister_csv_out(REGISTER_X, REGISTER_D);
  }
  break;

  case 2: {                   //Global Registers
    stackregister_csv_out(0, 99);
  }
  break;

  case 3: {                   //USER Registers
    stackregister_csv_out(FIRST_LOCAL_REGISTER, LAST_LOCAL_REGISTER);
  }
  break;

  default:
    break;
  }
#endif //TESTSUITE_BUILD
}

TO_QSPI void printf_cpx(calcRegister_t regist) {
#ifdef PC_BUILD
  if(getRegisterDataType(regist) == dtReal34 || getRegisterDataType(regist) == dtComplex34) {
    real34ToString(REGISTER_REAL34_DATA(regist), tmpString);
    if(strchr(tmpString, '.') == NULL && strchr(tmpString, 'E') == NULL) {
      strcat(tmpString, ".");
    }
    printf("Reg(%d) REAL = %s ", regist, tmpString);
  }
  if(getRegisterDataType(regist) == dtComplex34) {
    real34ToString(REGISTER_IMAG34_DATA(regist), tmpString);
    if(strchr(tmpString, '.') == NULL && strchr(tmpString, 'E') == NULL) {
      strcat(tmpString, ".");
    }
    printf("IMAG = %s ", tmpString);
  }
  if(getRegisterDataType(regist) != dtReal34 && getRegisterDataType(regist) != dtComplex34)
    printf("Neither real nor complex");
#endif //PC_BUILD
}


TO_QSPI void print_stck(){
  #ifdef PC_BUILD
  printf("Lasterrorcode=%d\n",lastErrorCode);
  printf("REGISTER T: ");printf_cpx(REGISTER_T);printf("\n");
  printf("REGISTER Z: ");printf_cpx(REGISTER_Z);printf("\n");
  printf("REGISTER Y: ");printf_cpx(REGISTER_Y);printf("\n");
  printf("REGISTER X: ");printf_cpx(REGISTER_X);printf("\n");
  #endif //PC_BUILD
}


TO_QSPI void doubleToXRegisterReal34(double x) { //Convert from double to X register REAL34
  setSystemFlag(FLAG_ASLIFT);
  liftStack();
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
  snprintf(tmpString, TMP_STR_LENGTH, "%.16e", x);
  stringToReal34(tmpString, REGISTER_REAL34_DATA(REGISTER_X));
  //adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  setSystemFlag(FLAG_ASLIFT);
}


TO_QSPI void fnStrtoX(char aimBuffer[]) {                             //DONE
  setSystemFlag(FLAG_ASLIFT); // 5
  liftStack();
  int16_t mem = stringByteLength(aimBuffer) + 1;
  reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(mem), amNone);
  xcopy(REGISTER_STRING_DATA(REGISTER_X), aimBuffer, mem);
  setSystemFlag(FLAG_ASLIFT);
}


TO_QSPI void fnStrInputReal34(char inp1[]) { // CONVERT STRING to REAL IN X      //DONE
  tmpString[0] = 0;
  strcat(tmpString, inp1);
  setSystemFlag(FLAG_ASLIFT); // 5
  liftStack();
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
  stringToReal34(tmpString, REGISTER_REAL34_DATA(REGISTER_X));
  setSystemFlag(FLAG_ASLIFT);
}


TO_QSPI void fnStrInputLongint(char inp1[]) { // CONVERT STRING to Longint X      //DONE
  tmpString[0] = 0;
  strcat(tmpString, inp1);
  setSystemFlag(FLAG_ASLIFT); // 5
  liftStack();

  longInteger_t lgInt;
  longIntegerInit(lgInt);
  stringToLongInteger(tmpString + (tmpString[0] == '+' ? 1 : 0), 10, lgInt);
  convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
  longIntegerFree(lgInt);
  setSystemFlag(FLAG_ASLIFT);
}


TO_QSPI void fnRCL(int16_t inp) { //DONE
  setSystemFlag(FLAG_ASLIFT);
  if(inp == TEMP_REGISTER_1) {
    liftStack();
    copySourceRegisterToDestRegister(inp, REGISTER_X);
  }
  else {
    fnRecall(inp);
  }
}


TO_QSPI double convert_to_double(calcRegister_t regist) { //Convert from X register to double
  double y;
  real_t tmpy;
  switch(getRegisterDataType(regist)) {
  case dtLongInteger:
    convertLongIntegerRegisterToReal(regist, &tmpy, &ctxtReal39);
    break;
  case dtReal34:
    real34ToReal(REGISTER_REAL34_DATA(regist), &tmpy);
    break;
  default:
    return 0;
    break;
  }
  realToString(&tmpy, tmpString);
  y = strtof(tmpString, NULL);
  return y;
}

TO_QSPI void timeToReal34(uint16_t hms) { //always 24 hour time;
  calcRegister_t regist = REGISTER_X;
  real34_t real34, value34, tmp34, h34, m34, s34;
  int32_t sign;
  uint32_t digits, tDigits = 0u, bDigits;
  bool_t isValid12hTime = false; //, isAfternoon = false;

  real34Copy(REGISTER_REAL34_DATA(regist), &real34);
  sign = real34IsNegative(&real34);

  // Pre-rounding
  int32ToReal34(10, &value34);
  int32ToReal34(10, &tmp34);
  for(bDigits = 0; bDigits < (isValid12hTime ? 14 : 16); ++bDigits) {
    if(real34CompareAbsLessThan(&h34, &value34))
      break;
    real34Multiply(&value34, &tmp34, &value34);
  }
  tDigits = isValid12hTime ? 14 : 16;
  isValid12hTime = false;

  for(digits = bDigits; digits < tDigits; ++digits) {
    real34Multiply(&real34, &value34, &real34);
  }
  real34ToIntegralValue(&real34, &real34, DEC_ROUND_HALF_UP);
  for(digits = bDigits; digits < tDigits; ++digits) {
    real34Divide(&real34, &value34, &real34);
  }
  tDigits = 0u;
  real34SetPositiveSign(&real34);

  // Seconds
  //real34ToIntegralValue(&real34, &s34, DEC_ROUND_DOWN);
  real34Copy(&real34, &s34);
  real34Subtract(&real34, &s34, &real34); // Fractional part
  int32ToReal34(60, &value34);
  // Minutes
  real34Divide(&s34, &value34, &m34);
  real34ToIntegralValue(&m34, &m34, DEC_ROUND_DOWN);
  real34DivideRemainder(&s34, &value34, &s34);
  // Hours
  real34Divide(&m34, &value34, &h34);
  real34ToIntegralValue(&h34, &h34, DEC_ROUND_DOWN);
  real34DivideRemainder(&m34, &value34, &m34);

  switch(hms) {
  case 0: //h
    int32ToReal34(sign ? -1 : +1, &value34);
    real34Multiply(&h34, &value34, &h34);
    reallocateRegister(regist, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&h34, REGISTER_REAL34_DATA(regist));
    break;

  case 1: //m
    int32ToReal34(sign ? -1 : +1, &value34);
    real34Multiply(&m34, &value34, &m34);
    reallocateRegister(regist, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&m34, REGISTER_REAL34_DATA(regist));
    break;

  case 2: //s
    int32ToReal34(sign ? -1 : +1, &value34);
    real34Multiply(&s34, &value34, &s34);
    reallocateRegister(regist, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&s34, REGISTER_REAL34_DATA(regist));
    break;

  default:
    break;
  }
}


TO_QSPI void dms34ToReal34(uint16_t dms) {
  real34_t angle34;
  calcRegister_t regist = REGISTER_X;
  real34_t value34, d34, m34, s34, fs34;
  real34Copy(REGISTER_REAL34_DATA(regist), &angle34);

  //    char degStr[27];
  uint32_t m, s, fs;
  int16_t sign;
  bool_t overflow;

  real_t temp, degrees, minutes, seconds;

  real34ToReal(&angle34, &temp);

  sign = realIsNegative(&temp) ? -1 : 1;
  realSetPositiveSign(&temp);

  // Get the degrees
  realToIntegralValue(&temp, &degrees, DEC_ROUND_DOWN, &ctxtReal39);

  // Get the minutes
  realSubtract(&temp, &degrees, &temp, &ctxtReal39);
  realMultiply(&temp, const_100, &temp, &ctxtReal39);
  realToIntegralValue(&temp, &minutes, DEC_ROUND_DOWN, &ctxtReal39);

  // Get the seconds
  realSubtract(&temp, &minutes, &temp, &ctxtReal39);
  realMultiply(&temp, const_100, &temp, &ctxtReal39);
  realToIntegralValue(&temp, &seconds, DEC_ROUND_DOWN, &ctxtReal39);

  // Get the fractional seconds
  realSubtract(&temp, &seconds, &temp, &ctxtReal39);
  realMultiply(&temp, const_100, &temp, &ctxtReal39);

  realToUInt32(&temp, DEC_ROUND_DOWN, &fs, &overflow);
  realToUInt32(&seconds, DEC_ROUND_DOWN, &s, &overflow);
  realToUInt32(&minutes, DEC_ROUND_DOWN, &m, &overflow);

  if(fs >= 100) {
    fs -= 100;
    s++;
  }

  if(s >= 60) {
    s -= 60;
    m++;
  }

  if(m >= 60)  {
    m -= 60;
    realAdd(&degrees, const_1, &degrees, &ctxtReal39);
  }

  switch(dms)  {
  case 0: //d
    int32ToReal34(sign, &value34);
    realToReal34(&degrees, &d34);
    real34Multiply(&d34, &value34, &d34);
    reallocateRegister(regist, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&d34, REGISTER_REAL34_DATA(regist));
    break;

  case 1: //m
    int32ToReal34(m, &m34);
    int32ToReal34(sign, &value34);
    real34Multiply(&m34, &value34, &m34);
    reallocateRegister(regist, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&m34, REGISTER_REAL34_DATA(regist));
    break;

  case 2: //s
    int32ToReal34(fs, &fs34);
    int32ToReal34(100, &value34);
    real34Divide(&fs34, &value34, &fs34);

    int32ToReal34(s, &s34);
    real34Add(&s34, &fs34, &s34);

    int32ToReal34(sign, &value34);
    real34Multiply(&s34, &value34, &s34);
    reallocateRegister(regist, dtReal34, REAL34_SIZE, amNone);
    real34Copy(&s34, REGISTER_REAL34_DATA(regist));
    break;

  default:
    break;
  }
}


TO_QSPI void notSexa(void) {
  copySourceRegisterToDestRegister(REGISTER_L, REGISTER_X);
  displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
  sprintf(errorMessage, "data type %s cannot be converted!", getRegisterDataTypeName(REGISTER_X, false, false));
  moreInfoOnError("In function notSexagecimal:", errorMessage, NULL, NULL);
#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
}


TO_QSPI void fnHrDeg(uint16_t unusedButMandatoryParameter) {
  if(!saveLastX()) return;
  if(getRegisterAngularMode(REGISTER_X) == amDMS && getRegisterDataType(REGISTER_X) == dtReal34)  dms34ToReal34(0);
  else if(getRegisterDataType(REGISTER_X) == dtTime)  timeToReal34(0); 
  else                                                notSexa();
}
TO_QSPI void fnMinute(uint16_t unusedButMandatoryParameter) {
  if(!saveLastX()) return;
  if(getRegisterAngularMode(REGISTER_X) == amDMS && getRegisterDataType(REGISTER_X) == dtReal34)  dms34ToReal34(1);
  else if(getRegisterDataType(REGISTER_X) == dtTime)  timeToReal34(1);
  else                                                notSexa();
}
TO_QSPI void fnSecond(uint16_t unusedButMandatoryParameter){
  if(!saveLastX()) return;
  if(getRegisterAngularMode(REGISTER_X) == amDMS && getRegisterDataType(REGISTER_X) == dtReal34)  dms34ToReal34(2);
  else if(getRegisterDataType(REGISTER_X) == dtTime)  timeToReal34(2);
  else                                                notSexa();
}

TO_QSPI void fnTimeTo(uint16_t unusedButMandatoryParameter) {

  if(!saveLastX()) return;

  if(getRegisterAngularMode(REGISTER_X) == amDMS && getRegisterDataType(REGISTER_X) == dtReal34) {
    dms34ToReal34(0);
    liftStack();
    copySourceRegisterToDestRegister(REGISTER_L, REGISTER_X);
    dms34ToReal34(1);
    liftStack();
    copySourceRegisterToDestRegister(REGISTER_L, REGISTER_X);
    dms34ToReal34(2);
  }
  else if(getRegisterDataType(REGISTER_X) == dtTime) {
    timeToReal34(0);
    liftStack();
    copySourceRegisterToDestRegister(REGISTER_L, REGISTER_X);
    timeToReal34(1);
    liftStack();
    copySourceRegisterToDestRegister(REGISTER_L, REGISTER_X);
    timeToReal34(2);
  }
  else {
    notSexa();
    return;
  }
}


/********************************************//**
 * \brief Check if time is valid (e.g. 10:61:61 is invalid)
 *
 * \param[in] hour real34_t*
 * \param[in] minute real34_t*
 * \param[in] second real34_t*
 * \return bool_t true if valid
 ***********************************************/
TO_QSPI bool_t isValidTime(const real34_t *hour, const real34_t *minute, const real34_t *second) {
  real34_t val;

  // second
  real34ToIntegralValue(second, &val, DEC_ROUND_FLOOR), real34Subtract(second, &val, &val);
  if(!real34IsZero(&val))       return false;
  int32ToReal34(0, &val), real34Compare(second, &val, &val);
  if(real34ToInt32(&val) < 0)   return false;
  int32ToReal34(59, &val), real34Compare(second, &val, &val);
  if(real34ToInt32(&val) > 0)   return false;

  // minute
  real34ToIntegralValue(minute, &val, DEC_ROUND_FLOOR), real34Subtract(minute, &val, &val);
  if(!real34IsZero(&val))       return false;
  int32ToReal34(0, &val), real34Compare(minute, &val, &val);
  if(real34ToInt32(&val) < 0)   return false;
  int32ToReal34(59, &val), real34Compare(minute, &val, &val);
  if(real34ToInt32(&val) > 0)   return false;

  // hour
  real34ToIntegralValue(hour, &val, DEC_ROUND_FLOOR), real34Subtract(hour, &val, &val);
  if(!real34IsZero(&val))       return false;
  int32ToReal34(0, &val), real34Compare(hour, &val, &val);
  if(real34ToInt32(&val) < 0)   return false;
  int32ToReal34(23, &val), real34Compare(hour, &val, &val);
  if(real34ToInt32(&val) > 0)   return false;
  // Valid time
  return true;
}


TO_QSPI void fnToTime(uint16_t unusedButMandatoryParameter) {
  real34_t hr, m, s, tmp;
  real34_t *part[3];
  calcRegister_t r[3] = {REGISTER_Z, REGISTER_Y, REGISTER_X};
  uint_fast8_t i;

  if(!saveLastX()) return;

  part[0] = &hr;
  part[1] = &m;
  part[2] = &s; //hrMs

  for(i = 0; i < 3; ++i) {
    switch(getRegisterDataType(r[i])) {
    case dtLongInteger:
      convertLongIntegerRegisterToReal34(r[i], part[i]);
      break;

    case dtReal34:
      if(getRegisterAngularMode(r[i])) {
        real34ToIntegralValue(REGISTER_REAL34_DATA(r[i]), part[i], DEC_ROUND_DOWN);
        break;
      }
      /* fallthrough */

    default:
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "data type %s cannot be converted to a time!", getRegisterDataTypeName(r[i], false, false));
      moreInfoOnError("In function fnToTime:", errorMessage, NULL, NULL);
#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }
  }

  if(!isValidTime(&hr, &m, &s)) {
    displayCalcErrorMessage(ERROR_BAD_TIME_OR_DATE_INPUT, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
    moreInfoOnError("In function fnToTime:", "Invalid time input!", NULL, NULL);
#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return;
  }

  // valid date
  fnDropY(NOPARAM);
  fnDropY(NOPARAM);

  int32ToReal34(3600, &tmp);
  real34Multiply(&tmp, &hr, &hr); //hr is now seconds
  int32ToReal34(60, &tmp);
  real34Multiply(&tmp, &m, &m); //m is now seconds
  real34Add(&hr, &m, &hr);
  real34Add(&hr, &s, &hr);

  reallocateRegister(REGISTER_X, dtTime, REAL34_SIZE, amNone);
  real34Copy(&hr, REGISTER_REAL34_DATA(REGISTER_X));
}




// *******************************************************************

TO_QSPI int32_t getD(const real34_t *val) {
/*
** Adapted from:
** https://www.ics.uci.edu/~eppstein/numth/frap.c
**
** find rational approximation to given real number
** David Eppstein / UC Irvine / 8 Aug 1993
** With corrections from Arno Formella, May 2008
**
** based on the theory of continued fractions
** if x = a1 + 1/(a2 + 1/(a3 + 1/(a4 + ...)))
** then best approximation is found by truncating this series
** (with some adjustments in the last term).
**
** Note the fraction can be recovered as the first column of the matrix
**  ( a1 1 ) ( a2 1 ) ( a3 1 ) ...
**  ( 1  0 ) ( 1  0 ) ( 1  0 )
** Instead of keeping the sequence of continued fraction terms,
** we just keep the last partial product of these matrices.
*/

    real34_t xx, temp;
    real34Copy(val, &xx);

    int32_t m[2][2];
    int32_t maxden = denMax; /*999*/
    int32_t ai;
    
    /* initialize matrix */
    m[0][0] = m[1][1] = 1;
    m[0][1] = m[1][0] = 0;

    /* loop finding terms until denom gets too big */
    while (m[1][0] *  ( ai = real34ToInt32(&xx) ) + m[1][1] <= maxden) {
      int32_t t;
      t = m[0][0] * ai + m[0][1];
      m[0][1] = m[0][0];
      m[0][0] = t;
      t = m[1][0] * ai + m[1][1];
      m[1][1] = m[1][0];
      m[1][0] = t;

      int32ToReal34(ai,&temp);
      real34Subtract(&xx,&temp,&xx);
      if(real34IsZero(&xx)) break;  // AF: division by zero
      real34Divide(const34_1,&xx,&xx);
      
      int32ToReal34(0x7FFFFFFF,&temp);
      if(real34CompareGreaterThan(&xx,&temp)) break;  // AF: representation failure
      } 

	//int nn = (double) m[0][0];
    int32_t dd = (double) m[1][0];
	//printf(">>> %i / %i \n",nn,dd);

    if(dd == 0) dd = 1;
    return dd;
}



TO_QSPI void changeToSup(char *str){
  char strtmp[100];
  strcpy(strtmp,str);
  int16_t u, src = 0;
  int16_t insertAt = 0;
  while (strtmp[src]!=0) {
      u = strtmp[src]-48;   
      if(u <= 1 && u >= 0) {
        str[insertAt]     = STD_SUP_0[0];
        str[insertAt + 1] = STD_SUP_0[1];
        str[insertAt + 1] += u;
        insertAt += 2;
      }
      else if(u <= 3 && u >= 0) {
        str[insertAt]     = STD_SUP_2[0];
        str[insertAt + 1] = STD_SUP_2[1];
        str[insertAt + 1] += u - 2;
        insertAt += 2;
      }
      else if(u <= 9 && u >= 0) {
        str[insertAt]     = STD_SUP_4[0];
        str[insertAt + 1] = STD_SUP_4[1];
        str[insertAt + 1] += u - 4;
        insertAt += 2;
      } else {
        str[insertAt]     = strtmp[src];
        insertAt ++;
      }
      src++;
    }
  str[insertAt]=0;
}

TO_QSPI void changeToSub(char *str){
  char strtmp[100];
  strcpy(strtmp,str);
  int16_t u, src = 0;
  int16_t insertAt = 0;
  while (strtmp[src]!=0) {
      u = strtmp[src]-48;   
      if(u <= 9 && u >= 0) {
        str[insertAt]     = STD_SUB_0[0];
        str[insertAt + 1] = STD_SUB_0[1];
        str[insertAt + 1] += u;
        insertAt += 2;
      } else {
        str[insertAt]     = strtmp[src];
        insertAt ++;
      }
      src++;
    }
  str[insertAt]=0;
}


  //without mixedNumber flag, improper fractions are allowed: In WP43 misnomer: FLAG_PROPFR = MixedNumber = a b/c
  real34_t result_fp1;
  bool_t checkForAndChange_(char *displayString, const real34_t *value34, const real_t *constant, const real34_t *tol34, const char *constantStr,  bool_t frontSpace) {
    //printf(">>> constantFractionsMode %i\n",constantFractionsMode);
    bool_t mixedNumber = getSystemFlag(FLAG_PROPFR) && !(constantFractionsMode == CF_COMPLEX1 || constantFractionsMode == CF_COMPLEX2);
    //printf(">>>## mixedNumber %u\n",mixedNumber);
    real34_t multConstant34, constant_34;
    real34_t val, val1, result, result_ip, result_fp;
    real_t constDiv;
    char denomStr[20], wholePart[30], resstr[100], tmpstr[50];
    denomStr[0]=0;
    wholePart[0]=0;
    resstr[0]=0;
    int32_t resultingInteger = 0;
    real34CopyAbs(value34, &val);
    real34Copy(value34, &val1);               //initialize val1 as a fallback value
	real_t newConstant, tempResult, tempresult_ip, tempresult_fp;
	real_t valr;
	real34ToReal(&val,&valr);

    char sign[2];
    if(real34IsPositive(value34)) strcpy(sign,"+"); else strcpy(sign,"-");

    realToReal34(constant,&constant_34);
    real34Divide(&val,&constant_34,&multConstant34);

    real34_t tmpr34;
    uInt32ToReal34(2097151,&tmpr34);
    //printReal34ToConsole(&multConstant34,"multConstant34="," > ");
    //printReal34ToConsole(&tmpr34,"tmpr34="," \n");
    if(real34CompareAbsGreaterThan(&multConstant34,&tmpr34)) {
      return false;
    }

    //See if the multiplier to the constant has a whole denominator
    //printReal34ToConsole(&multConstant34,"Check n/d :","\n");
    int32_t smallestDenom = getD(&multConstant34);
    if(smallestDenom>1) sprintf(denomStr,"/%i",(int)smallestDenom);
    //printf(">>># %i\n",smallestDenom);

    //Create a new constant comprising the constant divided by the whole denominator
    int32ToReal(smallestDenom, &constDiv);
    realDivide(constant, &constDiv, &newConstant, &ctxtReal39);

    //See if there is a whole multiple of the new constant
    realDivide(&valr,&newConstant,&tempResult,&ctxtReal39);
    realToIntegralValue(&tempResult, &tempresult_ip, DEC_ROUND_HALF_UP,&ctxtReal39);
    realSubtract(&tempResult, &tempresult_ip, &tempresult_fp, &ctxtReal39);

	realToReal34(&tempResult,&result);
	realToReal34(&tempresult_fp,&result_fp);
	realToReal34(&tempresult_ip,&result_ip);

    //printReal34ToConsole(&result_fp,"fp:","--\n");


    resultingInteger = abs(real34ToInt32(&result_ip));
    //printf(">>> %i ",resultingInteger);
    //if /*&& resultingInteger > (int32_t)denMax 999*/

    uInt32ToReal34(2147483647,&tmpr34); //3355443
    //printReal34ToConsole(&result_ip,"result_ip="," > ");
    //printReal34ToConsole(&tmpr34,"tmpr34="," \n");
    if(real34CompareAbsGreaterThan(&result_ip,&tmpr34)) {
    //printf("<<< break1 >>>\n");
      return false;
    }

    if(resultingInteger > 1 && real34CompareAbsLessThan(&result_fp,tol34)) {
      //a whole multiple of the constant exists
      real34Divide(&val, &result_ip, &val1);
      //printf(">>>Resultinginteger:%i SmallestDenom:%i\n",resultingInteger,smallestDenom);
      if (resultingInteger > smallestDenom  &&  smallestDenom > 1  && resultingInteger != 0 &&  mixedNumber) {
         int32_t tmp = resultingInteger / smallestDenom;
         resultingInteger = resultingInteger - (tmp * smallestDenom);
         if(constantStr[0]==0) {
             sprintf(wholePart,"%i%s",(int)tmp,sign);
         }
         else {
           if(tmp == 1)
             sprintf(wholePart,"%s%s",constantStr,sign);
           else
             sprintf(wholePart,"%i%s%s%s",(int)tmp,PRODUCT_SIGN,constantStr,sign);
         }
      }
      if(constantStr[0]==0) {
        sprintf(tmpstr,"%i",(int)resultingInteger);
        changeToSup(tmpstr);
        sprintf(resstr,"%s%s",wholePart,tmpstr);
      }
      else {
        if(resultingInteger == 1) {
          sprintf(resstr,"%s", wholePart);
        }
        else {
          sprintf(tmpstr,"%i%s",(int)resultingInteger,PRODUCT_SIGN);
//          changeToSup(tmpstr);
          sprintf(resstr,"%s%s",wholePart,tmpstr);
        }
      }
    //printf(">>> %s\n",resstr);
    }

    //printf(">>>@@@ §%s§%s§%s§ %i %i\n",resstr,constantStr,denomStr, (int16_t)stringByteLength(resstr)-1, resstr[stringByteLength(resstr)-1]);
    changeToSub(denomStr);
    if((resstr[stringByteLength(resstr)-1]==' ' || resstr[max(0,stringByteLength(resstr)-1)]==0) &&  denomStr[0]=='/' && constantStr[0]==0) {
      sprintf(tmpstr,STD_SUP_1 "%s",denomStr);
      strcpy(denomStr,tmpstr);
    }
    //printf(">>>@@@ §%s§%s§%s§\n",resstr,constantStr,denomStr);


    displayString[0]=0;
    if(real34CompareAbsLessThan(&result_fp,tol34)) {

      if(sign[0]=='+') {
        if(frontSpace) {
          strcat(displayString, " ");
          if(resstr[0]!=0) strcat(displayString, resstr);
          strcat(displayString,constantStr);
          strcat(displayString,denomStr);
        }
        else {
          if(resstr[0]!=0) strcat(displayString, resstr);
          strcat(displayString,constantStr);
          strcat(displayString,denomStr);
        }
      }
      else {
        strcat(displayString, "-");
        if(resstr[0]!=0) strcat(displayString, resstr);
        strcat(displayString,constantStr);
        strcat(displayString,denomStr);
      }


      if(constantFractionsMode == CF_COMPLEX1) {      //In case of complex, mark the accuracy of the first real fp
        real34Copy(&result_fp,&result_fp1);
      }
      else {
        if (constantFractionsMode == CF_COMPLEX2) {   //In case of complex, use  accuracy of the real and imag fp
          if (real34IsZero(&result_fp1) && real34IsZero(&result_fp)) {
          }
          else {
            strcat(displayString,STD_ALMOST_EQUAL);   //If either complex part is non-zero the show ~
          }
        }
        else {                                        //If neither complex parts, then it must be real
          if(real34IsZero(&result_fp)) {
            strcat(displayString,"");
          }
          else { 
            strcat(displayString,"" STD_ALMOST_EQUAL);
          }
        }
      }

      return true;
    } 
    else {
      return false;
    }
  }




TO_QSPI void fnConstantR(uint16_t constantAddr, uint16_t *constNr, real_t *rVal) {

  uint16_t constant =constantAddr;
  *constNr = constant;
//printf(">>> %u\n",constant);
  if(constant < NUMBER_OF_CONSTANTS_39) { // 39 digit constants
    realCopy((real_t *)(constants + constant * TO_BYTES(REAL39_SIZE)), rVal);
  }
  else if(constant < NUMBER_OF_CONSTANTS_39 + NUMBER_OF_CONSTANTS_51) { // 51 digit constants (gamma coefficients)
    realCopy((real_t *)(constants + NUMBER_OF_CONSTANTS_39 * TO_BYTES(REAL39_SIZE)
                                      + (constant - NUMBER_OF_CONSTANTS_39) * TO_BYTES(REAL51_SIZE)), rVal);
  }
  else if(constant < NUMBER_OF_CONSTANTS_39 + NUMBER_OF_CONSTANTS_51 + NUMBER_OF_CONSTANTS_1071) { // 1071 digit constant
    realCopy((real_t *)(constants + NUMBER_OF_CONSTANTS_39 * TO_BYTES(REAL39_SIZE) + NUMBER_OF_CONSTANTS_51 * TO_BYTES(REAL51_SIZE)
                                      + (constant - NUMBER_OF_CONSTANTS_39 - NUMBER_OF_CONSTANTS_51) * TO_BYTES(REAL1071_SIZE)), rVal);
  }
  else { // 34 digit constants
    real34ToReal((real_t *)(constants + NUMBER_OF_CONSTANTS_39 * TO_BYTES(REAL39_SIZE) + NUMBER_OF_CONSTANTS_51 * TO_BYTES(REAL51_SIZE) + NUMBER_OF_CONSTANTS_1071 * TO_BYTES(REAL1071_SIZE)
                                    + (constant - NUMBER_OF_CONSTANTS_39 - NUMBER_OF_CONSTANTS_51 - NUMBER_OF_CONSTANTS_1071) * TO_BYTES(REAL34_SIZE)), rVal);
  }
}



TO_QSPI void fnSafeReset (uint16_t unusedButMandatoryParameter) {
  if(!jm_FG_LINE && !jm_G_DOUBLETAP && !ShiftTimoutMode && !HOME3) {
    jm_FG_LINE     = true;
    jm_G_DOUBLETAP = true;
    ShiftTimoutMode= true;
    HOME3          = true;
  } else
  {
    jm_FG_LINE     = false;
    jm_G_DOUBLETAP = false;
    ShiftTimoutMode= false;
    HOME3          = false;    
  }
}


TO_QSPI void fnRESET_MyM_Mya(void){
//Pre-assign the MyMenu                   //JM
    #ifndef TESTSUITE_BUILD
    jm_NO_BASE_SCREEN = true;                                           //JM prevent slow updating of 6 menu items
    for(int8_t fn = 1; fn <= 6; fn++) {
      //itemToBeAssigned = ( !getSystemFlag(FLAG_USER) ? (kbd_std[fn-1].fShifted) : (kbd_usr[fn-1].fShifted) );  //Function key follows if the yellow key
      itemToBeAssigned = menu_HOME[fn -1];  //Function key follows if the yellow key
      assignToMyMenu(fn - 1);
      }
    jm_NO_BASE_SCREEN = false;                                           //JM Menu system default (removed from reset_jm_defaults)

    itemToBeAssigned = -MNU_ALPHA;
    assignToMyAlpha(5);
    #endif // TESTSUITE_BUILD
}


//Softmenus:
//--------------------------------------------------------------------------------------------

//JM To determine the menu number for a given menuId          //JMvv
TO_QSPI int16_t mm(int16_t id) {
  int16_t m;
  m = 0;
  if(id != 0) { // Search by ID
    while(softmenu[m].menuItem != 0) {
      //printf(">>> mm %d %d %d %s \n",id, m, softmenu[m].menuItem, indexOfItems[-softmenu[m].menuItem].itemSoftmenuName);
      if(softmenu[m].menuItem == id) {
       //printf("####>> mm() broken out id=%i m=%i\n",id,m);
       break;
      }
      m++;
    }
  }
  return m;
}                                                             //JM^^

#ifndef TESTSUITE_BUILD


//vv EXTRA DRAWINGS FOR RADIO_BUTTON AND CHECK_BOX
#define JM_LINE2_DRAW
#undef JM_LINE2_DRAW
#ifdef JM_LINE2_DRAW
TO_QSPI void JM_LINE2(uint32_t xx, uint32_t yy) {                          // To draw the lines for radiobutton on screen
  uint32_t x, y;
  y = yy-3-1;
  for(x=xx-66+1; x<min(xx-1,SCREEN_WIDTH); x++) {
    if(mod(x, 2) == 0) {
      setBlackPixel(x, y);
      setBlackPixel(x, y+2);
    }
    else {
      setBlackPixel(x, y+1);
    }
  }
}
#endif //JM_LINE2_DRAW


#define RB_EXTRA_BORDER
//#undef RB_EXTRA_BORDER
#define RB_CLEAR_CENTER
#undef RB_CLEAR_CENTER
#ifdef RB_EXTRA_BORDER
TO_QSPI void rbColumnCcccccc(uint32_t xx, uint32_t yy) {
  lcd_fill_rect(xx,yy+2,1,7,  0);
}
#endif //RB_EXTRA_BORDER



TO_QSPI void rbColumnCcSssssCc(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy+8,1,2,  0);
#endif //RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy+3,1,5,  0xFF);
#ifdef RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy+1,1,1,  0);
#endif //RB_EXTRA_BORDER
}



TO_QSPI void rbColumnCcSssssssCc(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy+9,1,2,  0);
#endif //RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy+2,1,7,  0xFF);
#ifdef RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy,1,2,  0);
#endif //RB_EXTRA_BORDER
}



TO_QSPI void rbColumnCSssCccSssC(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+10);
#endif //RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy+7,1,3,  0xFF);
  lcd_fill_rect(xx,yy+4,1,3,  0);
  lcd_fill_rect(xx,yy+1,1,3,  0xFF);
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+0);
#endif //RB_EXTRA_BORDER
}



TO_QSPI void rbColumnCSsCSssCSsC(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+10);
#endif //RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy+8,1,2,  0xFF);
  setWhitePixel (xx,yy+7);
  lcd_fill_rect(xx,yy+4,1,3,  0xFF);
  setWhitePixel (xx,yy+3);
  lcd_fill_rect(xx,yy+1,1,2,  0xFF);
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+0);
#endif //RB_EXTRA_BORDER
}



TO_QSPI void rbColumnCcSsNnnSsCc(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy+9,1,2,  0);
#endif //RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy+7,1,2,  0xFF);
#ifdef RB_CLEAR_CENTER
  lcd_fill_rect(xx,yy+4,1,3,  0);
#endif //RB_CLEAR_CENTER
  lcd_fill_rect(xx,yy+2,1,2,  0xFF);
#ifdef RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy+0,1,2,  0);
#endif //RB_EXTRA_BORDER
}



TO_QSPI void rbColumnCSsNnnnnSsC(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+10);
#endif //RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy+8,1,2,  0xFF);
#ifdef RB_CLEAR_CENTER
  lcd_fill_rect(xx,yy+3,1,5,  0);
#endif //RB_CLEAR_CENTER
  lcd_fill_rect(xx,yy+1,1,2,  0xFF);
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+0);
#endif //RB_EXTRA_BORDERf
}



TO_QSPI void rbColumnCSNnnnnnnSC(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+10);
#endif //RB_EXTRA_BORDER
  setBlackPixel (xx,yy+9);
#ifdef RB_CLEAR_CENTER
  lcd_fill_rect(xx,yy+2,1,7,  0);
#endif //RB_CLEAR_CENTER
  setBlackPixel (xx,yy+1);
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+0);
#endif //RB_EXTRA_BORDER
}



#ifdef RB_EXTRA_BORDER
TO_QSPI void cbColumnCcccccccccc(uint32_t xx, uint32_t yy) {
  lcd_fill_rect(xx,yy+0,1,11,  0);
}
#endif



TO_QSPI void cbColumnCSssssssssC(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+10);
#endif //RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy+1,1,9,  0xFF);
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+0);
#endif //RB_EXTRA_BORDER
}



TO_QSPI void cbColumnCSsCccccSsC(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+10);
#endif //RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy+8,1,2,  0xFF);
  lcd_fill_rect(xx,yy+3,1,5,  0);
  lcd_fill_rect(xx,yy+1,1,2,  0xFF);
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+0);
#endif //RB_EXTRA_BORDER
}



TO_QSPI void cbColumnCSNnnnnnnSC(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+10);
#endif //RB_EXTRA_BORDER
  setBlackPixel (xx,yy+9);
#ifdef RB_CLEAR_CENTER
  lcd_fill_rect(xx,yy+2,1,7,  0);
#endif //RB_CLEAR_CENTER
  setBlackPixel (xx,yy+1);
#ifdef RB_EXTRA_BORDER
  setWhitePixel (xx,yy+0);
#endif //RB_EXTRA_BORDER
}



TO_QSPI void RB_CHECKED(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  rbColumnCcccccc(xx+0, yy);
#endif
  rbColumnCcSssssCc(xx+1, yy);
  rbColumnCcSssssssCc(xx+2, yy);
  rbColumnCSssCccSssC(xx+3, yy);
  rbColumnCSsCSssCSsC(xx+4, yy);
  rbColumnCSsCSssCSsC(xx+5, yy);
  rbColumnCSsCSssCSsC(xx+6, yy);
  rbColumnCSssCccSssC(xx+7, yy);
  rbColumnCcSssssssCc(xx+8, yy);
  rbColumnCcSssssCc(xx+9, yy);
//#ifdef RB_EXTRA_BORDER
//  rbColumnCcccccc(xx+10, yy);
//#endif
}



TO_QSPI void RB_UNCHECKED(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  rbColumnCcccccc(xx+0, yy);
#endif //RB_EXTRA_BORDER
  rbColumnCcSssssCc(xx+1, yy);
  rbColumnCcSsNnnSsCc(xx+2, yy);
  rbColumnCSsNnnnnSsC(xx+3, yy);
  rbColumnCSNnnnnnnSC(xx+4, yy);
  rbColumnCSNnnnnnnSC(xx+5, yy);
  rbColumnCSNnnnnnnSC(xx+6, yy);
  rbColumnCSsNnnnnSsC(xx+7, yy);
  rbColumnCcSsNnnSsCc(xx+8, yy);
  rbColumnCcSssssCc(xx+9, yy);
//#ifdef RB_EXTRA_BORDER
//  rbColumnCcccccc(xx+10, yy);
//#endif //RB_EXTRA_BORDER
}



TO_QSPI void CB_CHECKED(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy-1,10,11,0);
  cbColumnCcccccccccc(xx+0, yy);
#endif //RB_EXTRA_BORDER
  cbColumnCSssssssssC(xx+1, yy);
  cbColumnCSssssssssC(xx+2, yy);
  cbColumnCSsCccccSsC(xx+3, yy);
  rbColumnCSsCSssCSsC(xx+4, yy);
  rbColumnCSsCSssCSsC(xx+5, yy);
  rbColumnCSsCSssCSsC(xx+6, yy);
  cbColumnCSsCccccSsC(xx+7, yy);
  cbColumnCSssssssssC(xx+8, yy);
  cbColumnCSssssssssC(xx+9, yy);
//#ifdef RB_EXTRA_BORDER
//  cbColumnCcccccccccc(xx+10, yy);
//#endif //RB_EXTRA_BORDER
}



TO_QSPI void CB_UNCHECKED(uint32_t xx, uint32_t yy) {
#ifdef RB_EXTRA_BORDER
  lcd_fill_rect(xx,yy-1,10,11,0);
  cbColumnCcccccccccc(xx+0, yy);
#endif
  cbColumnCSssssssssC(xx+1, yy);
  cbColumnCSNnnnnnnSC(xx+2, yy);
  cbColumnCSNnnnnnnSC(xx+3, yy);
  cbColumnCSNnnnnnnSC(xx+4, yy);
  cbColumnCSNnnnnnnSC(xx+5, yy);
  cbColumnCSNnnnnnnSC(xx+6, yy);
  cbColumnCSNnnnnnnSC(xx+7, yy);
  cbColumnCSNnnnnnnSC(xx+8, yy);
  cbColumnCSssssssssC(xx+9, yy);
//#ifdef RB_EXTRA_BORDER
//  cbColumnCcccccccccc(xx+10, yy);
//#endif //RB_EXTRA_BORDER
}
//^^



#endif //TESTSUITE_BUILD

TO_QSPI void fnSetBCD (uint16_t bcd) {
  switch (bcd) {
    case JC_BCD:  {
    	bcdDisplay = !bcdDisplay;
      if(lastIntegerBase == 0) {
        fnChangeBaseJM(10);
      }
    }
    break;
    case BCD9c :  
    case BCD10c:
    case BCDu  :  bcdDisplaySign = bcd; break;
    case JC_TOPHEX : topHex = !topHex;
    default:break;
  }
}


TO_QSPI void fnSetSI_All (uint16_t unusedButMandatoryParameter) {
  SI_All = !SI_All;
}


