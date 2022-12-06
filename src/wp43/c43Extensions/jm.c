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

/********************************************//** //JM
 * \file jm.c JM_TEST
 ***********************************************/

/* ADDITIONAL C43 functions and routines */

#include "c43Extensions/jm.h"

#include "c43Extensions/addons.h"
#include "charString.h"
#include "display.h"
#include "flags.h"
#include "config.h"
#include "c43Extensions/graphs.h"
#include "c43Extensions/graphText.h"
#include "items.h"
#include "c43Extensions/keyboardTweak.h"
#include "keyboard.h"
#include "mathematics/mathematics.h"
#include "memory.h"
#include "c43Extensions/radioButtonCatalog.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "screen.h"
#include "stack.h"
#include "stats.h"
#include "c43Extensions/xeqm.h"
#include <string.h>

#include "wp43.h"



#ifdef PC_BUILD

  #ifdef PC_BUILD_TELLTALE
    static char * getCalcModeName(uint16_t cm) {
      if(cm == CM_NORMAL)                return "normal ";
      if(cm == CM_AIM)                   return "aim    ";
      if(cm == CM_EIM)                   return "eim    ";
      if(cm == CM_PEM)                   return "pem    ";
      if(cm == CM_NIM)                   return "nim    ";
      if(cm == CM_ASSIGN)                return "assign ";
      if(cm == CM_REGISTER_BROWSER)      return "reg.bro";
      if(cm == CM_FLAG_BROWSER)          return "flg.bro";
      if(cm == CM_FONT_BROWSER)          return "fnt.bro";
      if(cm == CM_PLOT_STAT)             return "plot.st";
      if(cm == CM_GRAPH)                 return "plot.gr";
      if(cm == CM_ERROR_MESSAGE)         return "err.msg";
      if(cm == CM_BUG_ON_SCREEN)         return "bug.scr";
      if(cm == CM_MIM)                   return "mim    ";
      if(cm == CM_EIM)                   return "eim    ";
      if(cm == CM_TIMER)                 return "timer  ";
      if(cm == CM_CONFIRMATION)          return "confirm";
      if(cm == CM_LISTXY)                return "listxy ";    //JM

      return "???    ";
    }
    static char * getAlphaCaseName(uint16_t ac) {
      if(ac == AC_LOWER) return "lower";
      if(ac == AC_UPPER) return "upper";

      return "???  ";
    }
  #endif //PC_BUILD_TELLTALE


  void jm_show_calc_state(char comment[]) {
  #ifdef PC_BUILD_TELLTALE
    printf("\n%s--------------------------------------------------------------------------------\n",comment);
    printf(".  calcMode: %s   last_CM=%s  AlphaCase=%s  doRefreshSoftMenu=%d    lastErrorCode=%d\n",getCalcModeName(calcMode), getCalcModeName(last_CM), getAlphaCaseName(alphaCase), doRefreshSoftMenu,lastErrorCode);
    printf(".  softmenuStack[0].softmenuId=%d       MY_ALPHA_MENU=%d    softmenu[softmenuStack[0].softmenuId].menuItem=%d -MNU_ALPHA=%d\n",softmenuStack[0].softmenuId,MY_ALPHA_MENU,softmenu[softmenuStack[0].softmenuId].menuItem, -MNU_ALPHA);
    printf(".  ");int8_t ix=0; while(ix<SOFTMENU_STACK_SIZE) {printf("(%d)=%5d ", ix, softmenuStack[ix].softmenuId); ix++;} printf("\n");
    printf(".  ");       ix=0; while(ix<SOFTMENU_STACK_SIZE) {printf("%9s ", indexOfItems[-softmenu[softmenuStack[ix].softmenuId].menuItem].itemSoftmenuName  ); ix++;} printf("\n");
    printf(".  (tam.mode=%d, catalog=%d) (mm_MNU_HOME=%d, mm_MNU_ALPHA=%d)\n",tam.mode, catalog, mm_MNU_HOME, mm_MNU_ALPHA);
    jm_show_comment("calcstate END:");
  #endif //PC_BUILD_TELLTALE
  }


  void jm_show_comment(char comment[]) {
  #ifdef PC_BUILD_VERBOSE2
    char tmp[600];
    strcpy(tmp,comment);
    strcat(tmp,"                                                                                                                                                                ");
    tmp[130]=0;
    printf("....%s calcMode=%4d last_CM=%4d tam.mode=%5d catalog=%5d Id=%4d Name=%8s F=%d G=%d \n",tmp, calcMode, last_CM, tam.mode, catalog, softmenuStack[0].softmenuId, indexOfItems[-softmenu[softmenuStack[0].softmenuId].menuItem].itemSoftmenuName,shiftF,shiftG);
  //  printf("....%s\n",tmp);  
  #endif  
  }
#endif //PC_BUILD


void reset_jm_defaults(int16_t toload) {

    SHOWregis = 9999;                                          //JMSHOW

    //JM defaults vv: CONFIG STO/RCL
    SigFigMode = 0;                                              //JM SIGFIG Default 0.
    eRPN = true;                 //TEMPORARY FOR TESTING         //JM eRPN Default. Create a flag to enable or disable eRPN. See bufferize.c
    HOME3 = true;                                                //JM HOME Default. Create a flag to enable or disable triple shift HOME3.
    ShiftTimoutMode = true;                                      //JM SHIFT Default. Create a flag to enable or disable SHIFT TIMER CANCEL.
    UNITDisplay = false;                                         //JM HOME Default. Create a flag to enable or disable UNIT display
    SH_BASE_HOME   = false;                                      //JM
    Norm_Key_00_VAR  = ITM_SIGMAPLUS;                            //JM
    Input_Default =  ID_43S;                                     //JM Input Default
    bcdDisplay = false;
    topHex = true;
    bcdDisplaySign = BCDu;
    LongPressM = RB_M1234;
    LongPressF = RB_F124;

    jm_FG_LINE = true;                                           //JM Screen / keyboard operation setup
    jm_G_DOUBLETAP = true;                                       //JM Screen / keyboard operation setup
    graph_xmin = -3*3.14159265;                                //JM GRAPH
    graph_xmax = -graph_xmin;                                  //JM GRAPH
    graph_ymin = -2;                                           //JM GRAPH
    graph_ymax = +2;                                           //JM GRAPH

    graph_reset();

    jm_HOME_SUM = false;                                       //JMHOME
    jm_LARGELI=true;
    constantFractions=false;                                   //JM
    constantFractionsMode = CF_NORMAL;
    constantFractionsOn=false;
    running_program_jm=false;                                  //JM program is running flag
    indic_x=0;                                                 //JM program progress indicators
    indic_y=0;                                                 //JM program progress indicators

    setSystemFlag(FLAG_SPCRES);                                //JM default infinity etc.
    clearSystemFlag(FLAG_DENFIX);                              //JM default
    denMax = 64;                                               //JM default

    fnShoiXRepeats(2);                                         //JM SHOIDISP
    ListXYposition = 0;


     //JM defaults ^^

    lastIntegerBase = 0;                                       //JM
                                                               //Find fnXEQMENU in the indexOfItems array
    fnXEQMENUpos = 0;
    while(indexOfItems[fnXEQMENUpos].func != fnXEQMENU) {
       fnXEQMENUpos++;
    }

                                                               //Reset XEQM
    uint16_t ix;
    ix = 0;
    while(ix<18) {
      indexOfItemsXEQM[+8*ix]=0;
      strcpy(indexOfItemsXEQM +8*ix, indexOfItems[fnXEQMENUpos+ix].itemSoftmenuName);
      ix++;    
    }


    fnClrMod(0);
    if(toload) XEQMENU_loadAllfromdisk();

    displayAIMbufferoffset = 0;
    T_cursorPos = 0;

}



/********************************************//**
 * \brief Sets/resets flag
 *
 * \param[in] jmConfig uint16_t
 * \return void
 ***********************************************/
void fnSetSetJM(uint16_t jmConfig) {                //DONE        //JM Set/Reset setting
  switch(jmConfig) {
  case JC_ERPN:                                             //JM eRPN
    eRPN = !eRPN;
    //fnInfo(eRPN);
    fnRefreshState();                                 //drJM
    break;

  case JC_FG_LINE:                                          //JM
    jm_FG_LINE = !jm_FG_LINE;
    fnRefreshState();                                 //drJM
    break;

  case JC_BASE_SCREEN:                                          //JM
    jm_BASE_SCREEN = !jm_BASE_SCREEN;
    fnRefreshState();                                 //drJM
    break;

  case JC_G_DOUBLETAP:                                      //JM
    jm_G_DOUBLETAP = !jm_G_DOUBLETAP;
    fnRefreshState();                                 //drJM
    break;

  case JC_HOME_TRIPLE:                                      //JM HOME.3
    HOME3 = !HOME3;
    fnRefreshState();                                 //drJM
    break;

  case JC_SHFT_4s:                                          //JM SH_4s
    ShiftTimoutMode = !ShiftTimoutMode;
    fnRefreshState();                                 //drJM
    break;

  case JC_BASE_HOME:                                        //JM HOME
    SH_BASE_HOME = !SH_BASE_HOME;
    fnRefreshState();                                 //drJM
    break;

  case JC_H_SUM:                                      //JM
    jm_HOME_SUM = !jm_HOME_SUM;
    fnRefreshState();                                 //drJM
    break;

  case JC_LARGELI:                                      //JM
    jm_LARGELI = !jm_LARGELI;
    fnRefreshState();                                 //drJM
    break;

  case JC_EXFRAC:                                      //JM
    constantFractions = !constantFractions;
    if(constantFractions) {
      if(getSystemFlag(FLAG_FRACT)) {
        clearSystemFlag(FLAG_FRACT);
        constantFractionsOn = true;
      }
    } else {
      if(constantFractionsOn) {
        setSystemFlag(FLAG_FRACT);
        constantFractionsOn = false;
      }
    }
    fnRefreshState();                                 //drJM
    break;

   case RX_COMMA:               //DONR
     fnClearFlag(FLAG_DECIMP);
     break;

   case RX_PERIOD:               //DONR
     fnSetFlag(FLAG_DECIMP);
     break;

   case TF_H12:               //DONR
     fnClearFlag(FLAG_TDM24);
     break;

   case TF_H24:               //DONR
     fnSetFlag(FLAG_TDM24);
     break;

   case CU_I:               //DONR
     fnClearFlag(FLAG_CPXj);
     break;

   case CU_J:               //DONR
     fnSetFlag(FLAG_CPXj);
     break;

   case PS_DOT:
     fnClearFlag(FLAG_MULTx);
     break;

   case PS_CROSS:               //DONR
     fnSetFlag(FLAG_MULTx);
     break;

   case SS_4:                 //DONE
     fnClearFlag(FLAG_SSIZE8);
     break;

   case SS_8:          //DONE
     fnSetFlag(FLAG_SSIZE8);
     break;

   case CM_RECTANGULAR:          //DONE
     fnClearFlag(FLAG_POLAR);
     break;

   case CM_POLAR:                 //DONE
     fnSetFlag(FLAG_POLAR);
     break;

   case DO_SCI:
     fnClearFlag(FLAG_ALLENG);
     break;

   case DO_ENG:
     fnSetFlag(FLAG_ALLENG);
     break;

   case DM_ANY:                                              //JM 
     fnFlipFlag(FLAG_DENANY);
     break;

   case DM_FIX:                                              //JM 
     fnFlipFlag(FLAG_DENFIX);
     break;


   case JC_BLZ:                                              //JM bit LeadingZeros
     fnFlipFlag(FLAG_LEAD0);                                       //dr
     break;

   case JC_BCR:                                              //JM bit ComplexResult
     fnFlipFlag(FLAG_CPXRES);                                     //dr
     break;

   case JC_BSR:                                              //JM bit ComplexResult
     fnFlipFlag(FLAG_SPCRES);                                     //dr
     break;

   case JC_FRC:                                              //JM bit ComplexResult
     fnFlipFlag(FLAG_FRCSRN);                                     //dr
     break;


   case JC_NL:                                              //JM call numlock
     numLock = !numLock;
     showAlphaModeonGui(); //dr JM, see keyboardtweaks
     break;

   case JC_UC:                                              //JM call flip case
     if(alphaCase == AC_LOWER) 
       alphaCase = AC_UPPER; 
     else
       alphaCase = AC_LOWER;
     showAlphaModeonGui(); //dr JM, see keyboardtweaks
     break;


  default:
    break;
  }
}




/********************************************//** XXX
 * \brief Set Norm_Key_00_VAR
 *
 * \param[in] sigmaAssign uint16_t
 * \return void
 ***********************************************/
void fnSigmaAssign(uint16_t sigmaAssign) {             //DONE
  int16_t tt = (int16_t)sigmaAssign;
  Norm_Key_00_VAR = tt - 16384;
  fnRefreshState();                                 //drJM
  fnClearFlag(FLAG_USER);
}



/********************************************//**
 * \brief Show flag value
 * \param[in] jmConfig to display uint16_t
 * \return void
 ***********************************************/
void fnShowJM(uint16_t jmConfig) {                               //DONE
  longInteger_t mem;
  longIntegerInit(mem);
  saveForUndo();
  liftStack();

  switch(jmConfig) {
  case JC_ERPN:
    uIntToLongInteger(eRPN ? 1:0, mem);
    break;
  default:
    break;
  }

  convertLongIntegerToLongIntegerRegister(mem, REGISTER_X);
  longIntegerFree(mem);

}


/********************************************//**
 * \brief Get item-value of assigned key to X
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnGetSigmaAssignToX(uint16_t unusedButMandatoryParameter) {       //DONE
  longInteger_t mem;
  longIntegerInit(mem);
  liftStack();

  uIntToLongInteger(Norm_Key_00_VAR, mem);

  convertLongIntegerToLongIntegerRegister(mem, REGISTER_X);
  longIntegerFree(mem);

}



//JM CONFIGURE USER MODE - ASSIGN KEYS

/********************************************//**
 * \brief
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnJM_GetXToNORMmode(uint16_t unusedButMandatoryParameter) {      //DONE
  int16_t X_REG;
  longInteger_t lgInt;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);
    longIntegerToAllocatedString(lgInt, tmpString, TMP_STR_LENGTH);
    longIntegerToInt(lgInt,X_REG);
    longIntegerFree(lgInt);
    //printf("Xreg %d\n", X_REG);
    Norm_Key_00_VAR = X_REG;
    fnClearFlag(FLAG_USER);
  }
}








uint16_t nprimes = 0;
/********************************************//**
 * RPN PROGRAM.
 *
 * \param[in] JM_OPCODE uint16_t
 * \return void
 ***********************************************/
void fnJM(uint16_t JM_OPCODE) {
#define JMTEMP    TEMP_REGISTER_1 // 98
#define JM_TEMP_I REGISTER_I // 97
#define JM_TEMP_J REGISTER_J // 96
#define JM_TEMP_K REGISTER_K // 95

#ifndef SAVE_SPACE_DM42_6
  if(JM_OPCODE == 6) {                                          //Delta to Star   ZYX to ZYX; destroys IJKL & JMTEMP
    saveForUndo();
    setSystemFlag(FLAG_ASLIFT);
    copySourceRegisterToDestRegister(REGISTER_X, JM_TEMP_I);   // STO I
    copySourceRegisterToDestRegister(REGISTER_Y, JM_TEMP_J);   // STO J
    copySourceRegisterToDestRegister(REGISTER_Z, JM_TEMP_K);   // STO K
    fnAdd(0);                                                   // +
    fnSwapXY(0);                                                // X<>Y

    fnAdd(0);                                                   // +
    copySourceRegisterToDestRegister(REGISTER_X, JMTEMP);       // STO JMTEMP
    fnRCL(JM_TEMP_K);                                          // RCL I
    fnRCL(JM_TEMP_J);                                          // RCL J     // z = (zx yz) / (x+y+z)
    fnMultiply(0);                                              // *
    fnSwapXY(0);                                                // X<>Y
    fnDivide(0);                                                // /

    fnRCL(JMTEMP);                                              // RCL JMTEMP
    fnRCL(JM_TEMP_I);                                          // RCL J
    fnRCL(JM_TEMP_J);                                          // RCL K     // y = (xy yz) / (x+y+z)
    fnMultiply(0);                                              // *
    fnSwapXY(0);                                                // X<>Y
    fnDivide(0);                                                // /

    fnRCL(JMTEMP);                                              // RCL JMTEMP
    fnRCL(JM_TEMP_I);                                          // RCL I
    fnRCL(JM_TEMP_K);                                          // RCL K     // z = (xy zx) / (x+y+z)
    fnMultiply(0);                                              // *
    fnSwapXY(0);                                                // X<>Y
    fnDivide(0);                                                // /

    copySourceRegisterToDestRegister(JM_TEMP_I, REGISTER_L);   // STO

    temporaryInformation = TI_ABC;

    adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
    adjustResult(REGISTER_Y, false, true, REGISTER_Y, -1, -1);
    adjustResult(REGISTER_Z, false, true, REGISTER_Z, -1, -1);
  }
  else

  if(JM_OPCODE == 7) {                                          //Star to Delta ZYX to ZYX; destroys IJKL & JMTEMP
    saveForUndo();
    setSystemFlag(FLAG_ASLIFT);
    copySourceRegisterToDestRegister(REGISTER_X, JM_TEMP_I);   // STO I
    copySourceRegisterToDestRegister(REGISTER_Y, JM_TEMP_J);   // STO J
    copySourceRegisterToDestRegister(REGISTER_Z, JM_TEMP_K);   // STO K

    fnMultiply(0);                          //IJ                // *
    fnSwapXY(0);
    fnRCL(JM_TEMP_I);                                          // RCL J
    fnMultiply(0);                          //IK                // *
    fnAdd(0);
    fnRCL(JM_TEMP_J);                                          // RCL J
    fnRCL(JM_TEMP_K);                                          // RCL K
    fnMultiply(0);                          //JK                // *
    fnAdd(0);
    copySourceRegisterToDestRegister(REGISTER_X, JMTEMP);       // STO JMTEMP
                                                                //
    fnRCL(JM_TEMP_J);                                          //      zx = () / y
    fnDivide(0);                                                // 

    fnRCL(JMTEMP);                                              // RCL JMTEMP    
    fnRCL(JM_TEMP_I);                                          //      yz = () / x
    fnDivide(0);                                                //

    fnRCL(JMTEMP);                                              // RCL JMTEMP    
    fnRCL(JM_TEMP_K);                                          //      xy = () / z
    fnDivide(0);                                                //

    copySourceRegisterToDestRegister(JM_TEMP_I, REGISTER_L);   // STO

    temporaryInformation = TI_ABBCCA;
    adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
    adjustResult(REGISTER_Y, false, true, REGISTER_Y, -1, -1);
    adjustResult(REGISTER_Z, false, true, REGISTER_Z, -1, -1);
  }
  else

  if(JM_OPCODE == 8) {                                          //SYMMETRICAL COMP to ABC   ZYX to ZYX; destroys IJKL
    saveForUndo();
    setSystemFlag(FLAG_ASLIFT);
    copySourceRegisterToDestRegister(REGISTER_X, JM_TEMP_I);   // STO I  //A2
    copySourceRegisterToDestRegister(REGISTER_Y, JM_TEMP_J);   // STO J  //A1
    copySourceRegisterToDestRegister(REGISTER_Z, JM_TEMP_K);   // STO K  //A0
    fnAdd(0);                                                   // +
    fnAdd(0);                                                   // + Va = Vao + Va1 +Va2

    setSystemFlag(FLAG_ASLIFT);
//    liftStack();
    fn_cnst_op_a(0);
    fnRCL(JM_TEMP_I);                                       // A2
    fnMultiply(0);                                              // * a
    setSystemFlag(FLAG_ASLIFT);
//    liftStack();
    fn_cnst_op_aa(0);
    fnRCL(JM_TEMP_J);                                       // A1
    fnMultiply(0);                                              // * aa
    fnAdd(0);                                                   // +
    fnRCL(JM_TEMP_K);                                       // A0
    fnAdd(0);                                                   // + Vb = Vao + aaVa1 +aVa2

    setSystemFlag(FLAG_ASLIFT);
//    liftStack();
    fn_cnst_op_aa(0);
    fnRCL(JM_TEMP_I);                                       // A2
    fnMultiply(0);                                              // * a
    setSystemFlag(FLAG_ASLIFT);
//    liftStack();
    fn_cnst_op_a(0);
    fnRCL(JM_TEMP_J);                                       // A1
    fnMultiply(0);                                              // * aa
    fnAdd(0);                                                   // +
    fnRCL(JM_TEMP_K);                                       // A0
    fnAdd(0);                                                   // + Vb = Vao + aaVa1 +aVa2

    copySourceRegisterToDestRegister(JM_TEMP_I, REGISTER_L);   // STO

    temporaryInformation = TI_ABC;
  }
  else

  if(JM_OPCODE == 9) {                                          //ABC to SYMMETRICAL COMP   ZYX to ZYX; destroys IJKL & JMTEMP
    saveForUndo();
    setSystemFlag(FLAG_ASLIFT);
    copySourceRegisterToDestRegister(REGISTER_X, JM_TEMP_I);  // STO I  //c
    copySourceRegisterToDestRegister(REGISTER_Y, JM_TEMP_J);  // STO J  //b
    copySourceRegisterToDestRegister(REGISTER_Z, JM_TEMP_K);  // STO K  //a
    fnAdd(0);                                                  // +
    fnAdd(0);                                                  // + Va0 = (Va + Vb +Vc)/3
    liftStack();
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, amNone);
    stringToReal34("3", REGISTER_REAL34_DATA(REGISTER_X));
    stringToReal34("0", REGISTER_IMAG34_DATA(REGISTER_X));      //
    copySourceRegisterToDestRegister(REGISTER_X, JMTEMP);       // STO
    fnDivide(0);

    setSystemFlag(FLAG_ASLIFT);
//    liftStack();
    fn_cnst_op_a(0);
    fnRCL(JM_TEMP_J);                                       // VB
    fnMultiply(0);                                              // * a
    setSystemFlag(FLAG_ASLIFT);
//    liftStack();
    fn_cnst_op_aa(0);
    fnRCL(JM_TEMP_I);                                       // VC
    fnMultiply(0);                                              // * aa
    fnAdd(0);                                                   // +
    fnRCL(JM_TEMP_K);                                       // VA
    fnAdd(0);                                                   // + V1 = (VA +aVb +aaVc) /3
    fnRCL(JMTEMP);                                              // 3
    fnDivide(0);                                                // /


    setSystemFlag(FLAG_ASLIFT);
//    liftStack();
    fn_cnst_op_aa(0);
    fnRCL(JM_TEMP_J);                                       // VB
    fnMultiply(0);                                              // * a
    setSystemFlag(FLAG_ASLIFT);
//    liftStack();
    fn_cnst_op_a(0);
    fnRCL(JM_TEMP_I);                                       // VC
    fnMultiply(0);                                              // * aa
    fnAdd(0);                                                   // +
    fnRCL(JM_TEMP_K);                                       // VA
    fnAdd(0);                                                   // + V1 = (VA +aVb +aaVc) /3
    fnRCL(JMTEMP);                                              // 3
    fnDivide(0);                                                // /

    copySourceRegisterToDestRegister(JM_TEMP_I, REGISTER_L);   // STO


    temporaryInformation = TI_012;
  }
  else

  if(JM_OPCODE == 10) {                                         //e^theta.j j
    saveForUndo();
    if(!saveLastX()) return;                                    // STO TMP
    fn_cnst_op_j(0);
    fnMultiply(0);                                              // * aa
    fnExp(0);
  }
  else

  if(JM_OPCODE == 11) {                                         //STO Z
    saveForUndo();
    setSystemFlag(FLAG_ASLIFT);                                          //  Registers: Z:90-92  V:93-95  I:96-98  XYZ
    copySourceRegisterToDestRegister(REGISTER_X, 90);
    copySourceRegisterToDestRegister(REGISTER_Y, 91);
    copySourceRegisterToDestRegister(REGISTER_Z, 92);
  }
  else

  if(JM_OPCODE == 13) {                                         //STO V
    saveForUndo();
    setSystemFlag(FLAG_ASLIFT);                                          //  Registers: Z:90-92  V:93-95  I:96-98  XYZ
    copySourceRegisterToDestRegister(REGISTER_X, 93);
    copySourceRegisterToDestRegister(REGISTER_Y, 94);
    copySourceRegisterToDestRegister(REGISTER_Z, 95);
  }
  else

  if(JM_OPCODE == 15) {                                         //STO I
    saveForUndo();
    setSystemFlag(FLAG_ASLIFT);                                          //  Registers: Z:90-92  V:93-95  I:96-98  XYZ
    copySourceRegisterToDestRegister(REGISTER_X, 96);
    copySourceRegisterToDestRegister(REGISTER_Y, 97);
    copySourceRegisterToDestRegister(REGISTER_Z, 98);
  }
  else

  if(JM_OPCODE == 12) {                                         //RCL Z
    saveForUndo();
    fnRCL(92);
    fnRCL(91);
    fnRCL(90);
  }
  else

  if(JM_OPCODE == 14) {                                         //RCL V
    saveForUndo();
    fnRCL(95);
    fnRCL(94);
    fnRCL(93);
  }
  else

  if(JM_OPCODE == 16) {                                         //RCL I
    saveForUndo();
    fnRCL(98);
    fnRCL(97);
    fnRCL(96);
  }
  else

  if(JM_OPCODE == 17) {                                         // V/I
    saveForUndo();
    fnRCL(95);
    fnRCL(98);
    fnDivide(0);
    fnRCL(94);
    fnRCL(97);
    fnDivide(0);
    fnRCL(93);
    fnRCL(96);
    fnDivide(0);
  }
  else

  if(JM_OPCODE == 18) {                                         // IZ
    saveForUndo();
    fnRCL(98);
    fnRCL(92);
    fnMultiply(0);
    fnRCL(97);
    fnRCL(91);
    fnMultiply(0);
    fnRCL(96);
    fnRCL(91);
    fnMultiply(0);
  }
  else

  if(JM_OPCODE == 19) {                                         // V/Z
    saveForUndo();
    fnRCL(95);
    fnRCL(92);
    fnDivide(0);
    fnRCL(94);
    fnRCL(91);
    fnDivide(0);
    fnRCL(93);
    fnRCL(90);
    fnDivide(0);
  }
  else

  if(JM_OPCODE == 20) {                                         //Copy Create X>ABC
    saveForUndo();
    setSystemFlag(FLAG_ASLIFT);
    copySourceRegisterToDestRegister(REGISTER_X, JM_TEMP_I);

    fnRCL(JM_TEMP_I);                                          //
    setSystemFlag(FLAG_ASLIFT);
//    liftStack();
    fn_cnst_op_a(0);
    fnMultiply(0);

    fnRCL(JM_TEMP_I);                                          //
    setSystemFlag(FLAG_ASLIFT);
//    liftStack();
    fn_cnst_op_aa(0);
    copySourceRegisterToDestRegister(REGISTER_X, JM_TEMP_J);
    fnMultiply(0);

    temporaryInformation = TI_ABC;

  }
  else

  if(JM_OPCODE == 45) {                                         //PRIME stats
    #ifdef PC_BUILD
//      ramDump();
    #endif

  char line1[700];
  //Create a 3x3 A-matrix
  strcpy(line1,"XEQC43 ERPN RECT 3 ENTER 3 M.NEW STO 99 DROP INDEX 99 1 ENTER 1 STOIJ DROP DROP");
  strcat(line1, " 1 STOEL J+ STOEL J+ STOEL");
  strcat(line1, " J+ STOEL DROP 0.5 ENTER CHS 3 ENTER SQRT 2 / CHS COMPLEX J+ STOEL COMPLEX CHS COMPLEX J+ STOEL");
  strcat(line1, " 1 J+ STOEL DROP J+ STOEL X^2 J+ STOEL DROP");
  strcat(line1, " RCL 99 ");
  fnXEQMexecute(line1);
    }
  else

  if(JM_OPCODE == 46) {                                         //PRIME stats
  char line1[700];
  //Create a 3x1 matrix from Z Y X
  strcpy(line1,"XEQC43 ERPN 3 ENTER 1 M.NEW STO 99 DROP INDEX 99 3 ENTER 1 STOIJ DROP DROP STOEL DROP  I- STOEL DROP  I-  STOEL DROP RCL 99 ");
  fnXEQMexecute(line1);
    }
  else


  if(JM_OPCODE == 47) {                                         //PRIME stats
  char line1[700];
  //Create a ZYX form a 3x1 matrix
  strcpy(line1,"XEQC43 ERPN STO 99 INDEX 99 DROP 1 ENTER 1 STOIJ DROP DROP RCLEL I+ RCLEL I+ RCLEL ");
  fnXEQMexecute(line1);
    }


  if(JM_OPCODE == 48) {                                         //f.g
    #ifndef TESTSUITE_BUILD
      btnClicked(NULL, "27");
    #endif
    }
  
#endif //SAVE_SPACE_DM42_6

// Item 255 is NOP

}


/********************************************//**
 * \brief Sets/resets USER
 *
 * \param[in] jmConfig uint16_t
 * \return void
 ***********************************************/
void fnUserJM(uint16_t jmUser) {
  switch(jmUser) {


//---KEYS SIGMA+ ALLOCATIONS: COPY SIGMA+ USER MODE primary to -> ALLMODE
//-----------------------------------------------------------------------
  case USER_COPY:
    kbd_usr[0].primary     = Norm_Key_00_VAR;
    fnRefreshState();
    fnSetFlag(FLAG_USER);
    break;


  #ifndef SAVE_SPACE_DM42 
    case USER_E43:
        fnUserJM(USER_KRESET);
        fnShowVersion(USER_E43);

        kbd_usr[0].primary      = ITM_STO;
        kbd_usr[1].primary      = ITM_RCL;
        kbd_usr[2].primary      = ITM_1ONX;
        kbd_usr[2].fShifted     = ITM_XTHROOT;
        kbd_usr[3].primary      = ITM_SQUAREROOTX;
        kbd_usr[3].fShifted     = ITM_SQUARE;
        kbd_usr[4].primary      = ITM_LOG10;
        kbd_usr[4].fShifted     = ITM_10x;
        kbd_usr[5].primary      = ITM_LN;
        kbd_usr[5].fShifted     = ITM_EXP;

        kbd_usr[6].primary      = ITM_SHIFTf;
        kbd_usr[6].fShifted     = ITM_NULL;
        kbd_usr[6].gShifted     = ITM_NULL;
        kbd_usr[6].keyLblAim     = ITM_SHIFTf;
        kbd_usr[6].primaryAim     = ITM_SHIFTf;
        kbd_usr[6].fShiftedAim     = ITM_NULL;
        kbd_usr[6].gShiftedAim     = ITM_NULL;
        kbd_usr[6].primaryTam     = ITM_SHIFTf;

        kbd_usr[7].primary      = ITM_SHIFTg;
        kbd_usr[7].fShifted     = ITM_NULL;
        kbd_usr[7].gShifted     = ITM_NULL;
        kbd_usr[7].keyLblAim     = ITM_SHIFTg;
        kbd_usr[7].primaryAim     = ITM_SHIFTg;
        kbd_usr[7].fShiftedAim     = ITM_NULL;
        kbd_usr[7].gShiftedAim     = ITM_NULL;
        kbd_usr[7].primaryTam     = ITM_SHIFTg;

        kbd_usr[8].fShifted     = ITM_DRG;
        kbd_usr[8].gShifted     = ITM_CONSTpi;
        
          kbd_usr[13].gShifted  = -MNU_STK;
          kbd_usr[14].fShifted  = ITM_MAGNITUDE;
          kbd_usr[14].gShifted  = ITM_ARG;
          kbd_usr[15].fShifted  = -MNU_ANGLECONV;
          kbd_usr[24].fShifted  = -MNU_MODE; //ITM_NULL;

        kbd_usr[17].primary      = ITM_XEQ;
        kbd_usr[17].fShifted     = ITM_AIM;
        kbd_usr[17].gShifted     = ITM_USERMODE;

        kbd_usr[22].primary      = ITM_UP1;
        kbd_usr[22].fShifted     = ITM_BST;
        kbd_usr[22].gShifted     = ITM_RBR;

        kbd_usr[27].primary      = ITM_DOWN1;
        kbd_usr[27].fShifted     = ITM_SST;
        kbd_usr[27].gShifted     = ITM_FLGSV;

        kbd_usr[29].fShifted     = -MNU_DISP;

        kbd_usr[ 8].primaryAim     = ITM_G;
        kbd_usr[ 9].primaryAim     = ITM_H;
        kbd_usr[10].primaryAim     = ITM_I;
        kbd_usr[11].primaryAim     = ITM_J;
        kbd_usr[13].primaryAim     = ITM_K;
        kbd_usr[14].primaryAim     = ITM_L;
        kbd_usr[15].primaryAim     = ITM_M;
        kbd_usr[18].primaryAim     = ITM_N;
        kbd_usr[19].primaryAim     = ITM_O;
        kbd_usr[20].primaryAim     = ITM_P;
        kbd_usr[21].primaryAim     = ITM_Q;
        kbd_usr[23].primaryAim     = ITM_R;
        kbd_usr[24].primaryAim     = ITM_S;
        kbd_usr[25].primaryAim     = ITM_T;
        kbd_usr[26].primaryAim     = ITM_U;
        kbd_usr[28].primaryAim     = ITM_V;
        kbd_usr[29].primaryAim     = ITM_W;
        kbd_usr[30].primaryAim     = ITM_X;
        kbd_usr[31].primaryAim     = ITM_Y;
        kbd_usr[33].primaryAim     = ITM_Z;

        Norm_Key_00_VAR        = ITM_USERMODE;
        fnRefreshState();
        fnSetFlag(FLAG_USER);
      break;
    #endif //SAVE_SPACE_DM42 


    #ifndef SAVE_SPACE_DM42 
        case USER_N43:          //USER
          fnUserJM(USER_KRESET);
          fnShowVersion(USER_N43);

        fnRefreshState();
        fnSetFlag(FLAG_USER);
      break;
    #endif //SAVE_SPACE_DM42 




    #ifndef SAVE_SPACE_DM42 
        case USER_V43:          //USER
          fnUserJM(USER_KRESET);
          fnShowVersion(USER_V43);

          kbd_usr[0].primary      = ITM_1ONX;
          kbd_usr[0].fShifted     = ITM_YX;
          kbd_usr[0].gShifted     = ITM_TGLFRT;

          kbd_usr[1].primary      = ITM_SQUAREROOTX;
          kbd_usr[1].fShifted     = ITM_SQUARE;
          kbd_usr[1].gShifted     = ITM_HASH_JM/*ITM_toINT*/;

          kbd_usr[2].primary      = ITM_LOG10;
          kbd_usr[2].fShifted     = ITM_10x;
          kbd_usr[2].gShifted     = ITM_ms;

          kbd_usr[3].primary      = ITM_LN;
          kbd_usr[3].fShifted     = ITM_EXP;
          kbd_usr[3].gShifted     = ITM_dotD;

          kbd_usr[4].primary      = ITM_SHIFTf;
          kbd_usr[4].fShifted     = ITM_NULL;
          kbd_usr[4].gShifted     = ITM_NULL;
          kbd_usr[4].keyLblAim     = ITM_SHIFTf;
          kbd_usr[4].primaryAim     = ITM_SHIFTf;
          kbd_usr[4].fShiftedAim     = ITM_NULL;
          kbd_usr[4].gShiftedAim     = ITM_NULL;
          kbd_usr[4].primaryTam     = ITM_SHIFTf;

          kbd_usr[5].primary      = ITM_SHIFTg;
          kbd_usr[5].fShifted     = ITM_NULL;
          kbd_usr[5].gShifted     = ITM_NULL;
          kbd_usr[5].keyLblAim     = ITM_SHIFTg;
          kbd_usr[5].primaryAim     = ITM_SHIFTg;
          kbd_usr[5].fShiftedAim     = ITM_NULL;
          kbd_usr[5].gShiftedAim     = ITM_NULL;
          kbd_usr[5].primaryTam     = ITM_SHIFTg;

          kbd_usr[6].primaryAim     = ITM_E;
          kbd_usr[7].primaryAim     = ITM_F;
          kbd_usr[8].primaryAim     = ITM_G;
          kbd_usr[9].primaryAim     = ITM_H;
          kbd_usr[10].primaryAim     = ITM_I;
          kbd_usr[11].primaryAim     = ITM_J;

          kbd_usr[13].primaryAim     = ITM_K;
          kbd_usr[14].primaryAim     = ITM_L;
          kbd_usr[15].primaryAim     = ITM_M;

          kbd_usr[17].primary     = ITM_SUB;
          kbd_usr[17].fShifted    = -MNU_STAT;
          kbd_usr[17].gShifted    = -MNU_SUMS;
          kbd_usr[17].keyLblAim     = ITM_MINUS;
          kbd_usr[17].primaryAim     = ITM_N;
          kbd_usr[17].fShiftedAim     = ITM_MINUS;
          kbd_usr[17].primaryTam     = ITM_SUB;

          kbd_usr[18].primaryAim     = ITM_O;
          kbd_usr[19].primaryAim     = ITM_P;
          kbd_usr[20].primaryAim     = ITM_Q;

          kbd_usr[21].primary     = ITM_XEQ;
          kbd_usr[21].fShifted    = ITM_AIM;
          kbd_usr[21].gShifted    = ITM_USERMODE;
          kbd_usr[21].keyLblAim     = ITM_NULL;
          kbd_usr[21].primaryAim     = ITM_R;
          kbd_usr[21].fShiftedAim     = ITM_NULL;
          kbd_usr[21].gShiftedAim     = ITM_NULL;
          kbd_usr[21].primaryTam     = ITM_alpha;

          kbd_usr[22].primary     = ITM_ADD;
          kbd_usr[22].fShifted    = -MNU_PROB;
          kbd_usr[22].gShifted    = -MNU_INTS;
          kbd_usr[22].keyLblAim     = ITM_PLUS;
          kbd_usr[22].primaryAim     = ITM_S;
          kbd_usr[22].fShiftedAim     = ITM_PLUS;
          kbd_usr[22].primaryTam     = ITM_ADD;

          kbd_usr[26].primary     = ITM_UP1;
          kbd_usr[26].fShifted    = ITM_BST;
          kbd_usr[26].gShifted    = ITM_RBR;
          kbd_usr[26].keyLblAim     = ITM_UP1;
          kbd_usr[26].primaryAim     = ITM_UP1;  //arrow
          kbd_usr[26].fShiftedAim     = CHR_caseUP;
          kbd_usr[26].gShiftedAim     = ITM_UP_ARROW;
          kbd_usr[26].primaryTam     = ITM_UP1;

          kbd_usr[27].primary     = ITM_MULT;       //ex-f/g
          kbd_usr[27].fShifted    = -MNU_FIN;
          kbd_usr[27].gShifted    = -MNU_ALPHAFN;
          kbd_usr[27].keyLblAim     = ITM_CROSS;
          kbd_usr[27].primaryAim     = ITM_W;
          kbd_usr[27].fShiftedAim     = ITM_CROSS;
          kbd_usr[27].gShiftedAim     = ITM_NULL;
          kbd_usr[27].primaryTam     = ITM_MULT;

          kbd_usr[31].primary     = ITM_DOWN1;
          kbd_usr[31].fShifted    = ITM_SST;
          kbd_usr[31].gShifted    = ITM_FLGSV;
          kbd_usr[31].keyLblAim     = ITM_DOWN1;
          kbd_usr[31].primaryAim     = ITM_DOWN1;  //arrow
          kbd_usr[31].fShiftedAim     = CHR_caseDN;
          kbd_usr[31].gShiftedAim     = ITM_DOWN_ARROW;
          kbd_usr[31].primaryTam     = ITM_DOWN1;

          kbd_usr[32].primary     = ITM_DIV;   //ex-exit
          kbd_usr[32].fShifted    = -MNU_CATALOG;
          kbd_usr[32].gShifted    = -MNU_PRINT;
          kbd_usr[32].keyLblAim     = ITM_OBELUS;
          kbd_usr[32].primaryAim     = ITM_SPACE;
          kbd_usr[32].fShiftedAim     = ITM_OBELUS;
          kbd_usr[32].gShiftedAim     = ITM_PRINTER;
          kbd_usr[32].primaryTam     = ITM_DIV;

          kbd_usr[36].primary     = ITM_EXIT1;
          kbd_usr[36].fShifted    = ITM_OFF;
          kbd_usr[36].gShifted    = -MNU_IO;
          kbd_usr[36].keyLblAim     = ITM_EXIT1;
          kbd_usr[36].primaryAim     = ITM_EXIT1;  //
          kbd_usr[36].fShiftedAim     = ITM_OFF;
          kbd_usr[36].primaryTam     = ITM_EXIT1;

//          Norm_Key_00_VAR         = ITM_1ONX;
          fnRefreshState();                                 //drJM
          fnSetFlag(FLAG_USER);
          break;
    #endif //SAVE_SPACE_DM42 




    #ifndef SAVE_SPACE_DM42 
      case USER_D43:          //USER
        fnUserJM(USER_KRESET);
        fnShowVersion(USER_D43);

        kbd_usr[6].primary      = ITM_STO;
        kbd_usr[7].primary      = ITM_RCL;
        kbd_usr[9].primary      = ITM_1ONX;
        kbd_usr[6].fShifted     = ITM_XTHROOT;//8
        kbd_usr[2].primary      = ITM_SQUAREROOTX;
        kbd_usr[2].fShifted     = ITM_SQUARE;
        kbd_usr[1].primary      = ITM_LOG10;  //0
        kbd_usr[1].fShifted     = ITM_10x;    //0
        kbd_usr[2].primary      = ITM_LN;     //1
        kbd_usr[2].fShifted     = ITM_EXP;    //1
        kbd_usr[0].primary=ITM_SQUAREROOTX;   //2
        kbd_usr[0].fShifted=ITM_SQUARE;       //2

        kbd_usr[10].primary      = ITM_SHIFTf;
        kbd_usr[10].fShifted     = ITM_NULL;
        kbd_usr[10].gShifted     = ITM_NULL;
        kbd_usr[10].keyLblAim     = ITM_SHIFTf;
        kbd_usr[10].primaryAim     = ITM_SHIFTf;
        kbd_usr[10].fShiftedAim     = ITM_NULL;
        kbd_usr[10].gShiftedAim     = ITM_NULL;
        kbd_usr[10].primaryTam     = ITM_SHIFTf;

        kbd_usr[11].primary      = ITM_SHIFTg;
        kbd_usr[11].fShifted     = ITM_NULL;
        kbd_usr[11].gShifted     = ITM_NULL;
        kbd_usr[11].keyLblAim     = ITM_SHIFTg;
        kbd_usr[11].primaryAim     = ITM_SHIFTg;
        kbd_usr[11].fShiftedAim     = ITM_NULL;
        kbd_usr[11].gShiftedAim     = ITM_NULL;
        kbd_usr[11].primaryTam     = ITM_SHIFTg;

        kbd_usr[9].fShifted     = ITM_DRG;
        kbd_usr[9].gShifted     = ITM_CONSTpi;

        kbd_usr[3].primary=ITM_sin;
        kbd_usr[3].fShifted=ITM_arcsin;
        kbd_usr[6].gShifted=ITM_GTO;
        kbd_usr[4].primary=ITM_cos;
        kbd_usr[4].fShifted=ITM_arccos;
        kbd_usr[7].gShifted=ITM_LBL;
        kbd_usr[5].primary=ITM_tan;
        kbd_usr[5].fShifted=ITM_arctan;
        kbd_usr[8].gShifted=ITM_RTN;
        kbd_usr[8].fShifted=ITM_RI;//6
        kbd_usr[7].fShifted=ITM_YX; 

          kbd_usr[13].gShifted  = -MNU_STK;
          kbd_usr[14].fShifted  = ITM_MAGNITUDE;
          kbd_usr[14].gShifted  = ITM_ARG;
          kbd_usr[15].fShifted  = -MNU_ANGLECONV;
          kbd_usr[24].fShifted  = -MNU_MODE; //ITM_NULL;
          kbd_usr[29].fShifted  = -MNU_DISP;

          /*kbd_usr[6].primaryAim     = ITM_E;
          kbd_usr[7].primaryAim     = ITM_F;
          kbd_usr[8].primaryAim     = ITM_G;
          kbd_usr[9].primaryAim     = ITM_H;*/
          kbd_usr[10].primaryAim     = ITM_NULL;
          kbd_usr[11].primaryAim     = ITM_NULL;

          kbd_usr[13].primaryAim     = ITM_K;
          kbd_usr[14].primaryAim     = ITM_L;
          kbd_usr[15].primaryAim     = ITM_M;

          kbd_usr[21].primary      = ITM_DIV;
          kbd_usr[21].fShifted     = -MNU_STAT;
          kbd_usr[21].gShifted     = -MNU_SUMS;
          kbd_usr[21].keyLblAim    = ITM_OBELUS;
          kbd_usr[21].fShiftedAim  = ITM_OBELUS;
          kbd_usr[21].primaryTam   = ITM_DIV;


          kbd_usr[17].primary       = ITM_XEQ;
          kbd_usr[17].fShifted      = ITM_AIM;
          kbd_usr[17].gShifted      = ITM_USERMODE;
          kbd_usr[17].keyLblAim     = ITM_NULL;
          kbd_usr[17].fShiftedAim   = ITM_NULL;
          kbd_usr[17].gShiftedAim   = ITM_NULL;
          kbd_usr[17].primaryTam    = ITM_alpha;

          kbd_usr[26].primary       = ITM_MULT;
          kbd_usr[26].fShifted      = -MNU_PROB;
          kbd_usr[26].gShifted      = -MNU_INTS;
          kbd_usr[26].keyLblAim     = ITM_CROSS;
          kbd_usr[26].fShiftedAim   = ITM_CROSS;
          kbd_usr[26].primaryTam    = ITM_MULT;

          kbd_usr[22].primary       = ITM_UP1;
          kbd_usr[22].fShifted      = ITM_BST;
          kbd_usr[22].gShifted      = ITM_RBR;
          kbd_usr[22].keyLblAim     = ITM_UP1;
          kbd_usr[22].primaryAim    = ITM_UP1;  //arrow
          kbd_usr[22].fShiftedAim   = CHR_caseUP;
          kbd_usr[22].gShiftedAim   = ITM_UP_ARROW;
          kbd_usr[22].primaryTam    = ITM_UP1;

          kbd_usr[31].primary       = ITM_SUB;       //ex-f/g
          kbd_usr[31].fShifted      = -MNU_FIN;
          kbd_usr[31].gShifted      = -MNU_ALPHAFN;
          kbd_usr[31].keyLblAim     = ITM_MINUS;
          kbd_usr[31].fShiftedAim   = ITM_MINUS;
          kbd_usr[31].gShiftedAim   = ITM_NULL;
          kbd_usr[31].primaryTam    = ITM_SUB;

          kbd_usr[27].primary       = ITM_DOWN1;
          kbd_usr[27].fShifted      = ITM_SST;
          kbd_usr[27].gShifted      = ITM_FLGSV;
          kbd_usr[27].keyLblAim     = ITM_DOWN1;
          kbd_usr[27].primaryAim    = ITM_DOWN1;  //arrow
          kbd_usr[27].fShiftedAim   = CHR_caseDN;
          kbd_usr[27].gShiftedAim   = ITM_DOWN_ARROW;
          kbd_usr[27].primaryTam    = ITM_DOWN1;

          kbd_usr[36].primary         = ITM_ADD;   //ex-exit
          kbd_usr[36].fShifted        = -MNU_CATALOG;
          kbd_usr[36].gShifted        = -MNU_PRINT;
          kbd_usr[36].keyLblAim       = ITM_PLUS;
          kbd_usr[36].primaryAim      = ITM_SPACE;
          kbd_usr[36].fShiftedAim     = ITM_PLUS;
          kbd_usr[36].gShiftedAim     = ITM_PLUS;
          kbd_usr[36].primaryTam      = ITM_ADD;

          kbd_usr[32].primary        = ITM_EXIT1;
          kbd_usr[32].fShifted       = ITM_OFF;
          kbd_usr[32].gShifted       = -MNU_IO;
          kbd_usr[32].keyLblAim      = ITM_EXIT1;
          kbd_usr[32].primaryAim     = ITM_EXIT1;  //
          kbd_usr[32].fShiftedAim    = ITM_OFF;
          kbd_usr[32].primaryTam     = ITM_EXIT1;

          kbd_usr[18].primaryAim     = ITM_N;
          kbd_usr[19].primaryAim     = ITM_O;
          kbd_usr[20].primaryAim     = ITM_P;
          kbd_usr[21].primaryAim     = ITM_Q;
          kbd_usr[23].primaryAim     = ITM_R;
          kbd_usr[24].primaryAim     = ITM_S;
          kbd_usr[25].primaryAim     = ITM_T;
          kbd_usr[26].primaryAim     = ITM_U;
          kbd_usr[28].primaryAim     = ITM_V;
          kbd_usr[29].primaryAim     = ITM_W;
          kbd_usr[30].primaryAim     = ITM_X;
          kbd_usr[31].primaryAim     = ITM_Y;
          kbd_usr[33].primaryAim     = ITM_Z;


//          Norm_Key_00_VAR         = ITM_1ONX;
          fnRefreshState();                                 //drJM
          fnSetFlag(FLAG_USER);
      break;
    #endif //SAVE_SPACE_DM42 



//---KEYS PROFILE: C43
//------------------------
  #ifndef SAVE_SPACE_DM42 
    case USER_C43:          //USER
      fnUserJM(USER_KRESET);
      fnShowVersion(USER_C43);
      fnSetFlag(FLAG_USER);
    break;
  #endif //SAVE_SPACE_DM42 


//---KEYS PROFILE: DM42
//------------------------
  case USER_DM42:
    fnUserJM(USER_KRESET);
    fnShowVersion(USER_DM42);
    xcopy(kbd_usr, kbd_std_DM42, sizeof(kbd_std_DM42));
    fnSetFlag(FLAG_USER);
    break;


//---KEYS PROFILE: WP43
//------------------------
  case USER_43S:
    fnUserJM(USER_KRESET);
    fnShowVersion(USER_43S);
    xcopy(kbd_usr, kbd_std_WP43, sizeof(kbd_std_WP43));
    kbd_usr[10].primary=KEY_fg;
    kbd_usr[10].keyLblAim=KEY_fg;
    kbd_usr[10].primaryAim=KEY_fg;
    kbd_usr[10].gShiftedAim=ITM_NULL;
    kbd_usr[10].gShifted=ITM_NULL;
    kbd_usr[10].primaryTam=KEY_fg;
    kbd_usr[11].fShiftedAim=ITM_NULL;
    kbd_usr[11].fShifted=ITM_NULL;
    kbd_usr[18].gShifted=ITM_SNAP;  
    kbd_usr[18].fShifted=-MNU_ASN; 
    kbd_usr[19].fShifted=ITM_USERMODE;
    fnSetFlag(FLAG_USER);
    break;


//---KEYS PROFILE: C43-ALTA
//-------------------------
  case USER_C43ALTA:                                             //USER_SHIFTS 25          //JM Sectioon to be put on a menu      
      fnUserJM(USER_KRESET);
      fnShowVersion(USER_C43ALTA);
      xcopy(kbd_usr, kbd_std_C43AltA, sizeof(kbd_std_C43AltA));
      Norm_Key_00_VAR        = ITM_USERMODE;
      fnRefreshState();
      fnSetFlag(FLAG_USER);
    break;


//---KEYS PROFILE: C43-ALT
//-------------------------
  case USER_C43ALT:                                             //USER_SHIFTS 25          //JM Sectioon to be put on a menu
      fnUserJM(USER_C43ALTA);
      fnShowVersion(USER_C43ALT);
      kbd_usr[0].primary     = ITM_SIGMAPLUS;
      Norm_Key_00_VAR        = ITM_SIGMAPLUS;
      kbd_usr[10].primary    = ITM_SHIFTf;
      kbd_usr[27].primary    = KEY_fg;
      kbd_usr[27].fShifted   = ITM_NULL;
      kbd_usr[27].gShifted   = ITM_NULL;
      fnRefreshState();
      fnSetFlag(FLAG_USER);
    break;



  case USER_MRESET:                                              //USER_KRESET 26
    fnRESET_MyM();
    fnShowVersion(USER_MRESET);
    break;

  case USER_ARESET:                                              //USER_KRESET 26
    fnRESET_Mya();
    fnShowVersion(USER_ARESET);
    break;

  case USER_KRESET:                                              //USER_KRESET 26
    fnShowVersion(USER_KRESET);
    xcopy(kbd_usr, kbd_std, sizeof(kbd_std));
    Norm_Key_00_VAR        = ITM_SIGMAPLUS;
    fnRefreshState();                                 //drJM
    fnClearFlag(FLAG_USER); //userModeEnabled = false;
    break;


  default:
    break;
  }
}






