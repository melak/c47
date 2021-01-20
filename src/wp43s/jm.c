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

/********************************************//** //JM
 * \file jm.c JM_TEST
 ***********************************************/

/* ADDITIONAL C43 functions and routines */


#include "wp43s.h"



#ifdef PC_BUILD
  void jm_show_calc_state(char comment[]) {
  #ifdef PC_BUILD_TELLTALE
    printf("\n%s-----------------------------------------------------------------------------------------------------------------\n",comment);
    printf(".  calcMode: %d   last_CM=%d (CM_AIM=%d)  doRefreshSoftMenu=%d    lastErrorCode=%d\n",calcMode, last_CM, CM_AIM,doRefreshSoftMenu,lastErrorCode);
    printf(".  softmenuStack[0].softmenuId=%d       MY_ALPHA_MENU=%d    softmenu[softmenuStack[0].softmenuId].menuItem=%d -MNU_ALPHA=%d\n",softmenuStack[0].softmenuId,MY_ALPHA_MENU,softmenu[softmenuStack[0].softmenuId].menuItem, -MNU_ALPHA);
    printf(".  ");int8_t ix=0; while(ix<SOFTMENU_STACK_SIZE) {printf("(%d)=%5d ", ix, softmenuStack[ix].softmenuId); ix++;} printf("\n");
    printf(".  ");       ix=0; while(ix<SOFTMENU_STACK_SIZE) {printf("%9s ", indexOfItems[-softmenu[softmenuStack[ix].softmenuId].menuItem].itemSoftmenuName  ); ix++;} printf("\n");
    printf(".  (tammode=%d, catalog=%d)\n",tamMode, catalog);
    printf(".  (mm_MNU_HOME=%d, mm_MNU_ALPHA=%d)\n",mm_MNU_HOME, mm_MNU_ALPHA);
    jm_show_comment("calcstate END:");
  #endif  
  }


  void jm_show_comment(char comment[]) {
  #ifdef PC_BUILD_VERBOSE2
    char tmp[600];
    strcpy(tmp,comment);
    strcat(tmp,"                                                                                                                                                                ");
    tmp[130]=0;
    printf("....%130s calcMode=%4d last_CM=%4d tammode=%5d catalog=%5d Id=%4d Name=%8s F=%d G=%d \n",tmp, calcMode, last_CM, tamMode, catalog, softmenuStack[0].softmenuId, indexOfItems[-softmenu[softmenuStack[0].softmenuId].menuItem].itemSoftmenuName,shiftF,shiftG);
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
    Home3TimerMode = true;                                       //JM SHIFT Default. Create a flag to enable or disable SHIFT TIMER MODE FOR HOME.
    UNITDisplay = false;                                         //JM HOME Default. Create a flag to enable or disable UNIT display
    SH_BASE_HOME   = false;                                      //JM
    SH_BASE_AHOME  = true;                                       //JM
    Norm_Key_00_VAR  = ITM_SIGMAPLUS;                            //JM
    Input_Default =  ID_43S;                                     //JM Input Default
    jm_FG_LINE = true;                                           //JM Screen / keyboard operation setup
    jm_NO_BASE_SCREEN = true;                                           //JM Menu system default
    jm_G_DOUBLETAP = true;                                       //JM Screen / keyboard operation setup
    graph_xmin = -3*3.14159265;                                //JM GRAPH
    graph_xmax = +3*3.14159265;                                //JM GRAPH
    graph_ymin = -2;                                           //JM GRAPH
    graph_ymax = +2;                                           //JM GRAPH

    graph_reset();

    jm_HOME_SUM = false;                                       //JMHOME
    jm_HOME_MIR = true;                                        //JMHOME
    jm_HOME_FIX = false;                                       //JMHOME
    #if defined(DMCP_BUILD) || defined(JM_LAYOUT_2_DM42_STRICT)//JM LAYOUT 2. DM42 STRICT.
    jm_HOME_SUM = false;                                       //JMHOME
    jm_HOME_MIR = true;                                        //JMHOME
    jm_HOME_FIX = false;                                       //JMHOME
    #endif
    jm_LARGELI=true;
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


    fnClAIM(0);
    if(toload) XEQMENU_loadAllfromdisk();

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
    fnRefreshComboxState(CB_JC, JC_ERPN, eRPN);                                 //dr
    break;

  case JC_FG_LINE:                                          //JM
    jm_FG_LINE = !jm_FG_LINE;
    fnRefreshComboxState(CB_JC, JC_FG_LINE, jm_FG_LINE);                        //jm
    break;

  case JC_NO_BASE_SCREEN:                                          //JM
    jm_NO_BASE_SCREEN = !jm_NO_BASE_SCREEN;
    fnRefreshComboxState(CB_JC, JC_NO_BASE_SCREEN, jm_NO_BASE_SCREEN);                        //jm
    break;

  case JC_G_DOUBLETAP:                                      //JM
    jm_G_DOUBLETAP = !jm_G_DOUBLETAP;
    fnRefreshComboxState(CB_JC, JC_G_DOUBLETAP, jm_G_DOUBLETAP);                //jm
    break;

  case JC_HOME_TRIPLE:                                      //JM HOME.3
    HOME3 = !HOME3;
    //fnInfo(HOME3);
    fnRefreshComboxState(CB_JC, JC_HOME_TRIPLE, HOME3);                         //dr
    break;

  case JC_SHFT_4s:                                          //JM SH_4s
    ShiftTimoutMode = !ShiftTimoutMode;
    //fnInfo(ShiftTimoutMode);
    fnRefreshComboxState(CB_JC, JC_SHFT_4s, ShiftTimoutMode);                   //dr
    break;

  case JC_BASE_HOME:                                        //JM HOME
    SH_BASE_HOME = !SH_BASE_HOME;
    //fnInfo(SH_BASE_HOME);
    fnRefreshComboxState(CB_JC, JC_BASE_HOME, SH_BASE_HOME);                    //dr
    break;

  case JC_BASE_AHOME:                                       //JM aHOME
    SH_BASE_AHOME = !SH_BASE_AHOME;
    //fnInfo(SH_BASE_AHOME);
    if(SH_BASE_AHOME) MY_ALPHA_MENU = mm_MNU_ALPHA;
    else              MY_ALPHA_MENU = MY_ALPHA_MENU_CNST;
    fnRefreshComboxState(CB_JC, JC_BASE_AHOME, SH_BASE_AHOME);                  //dr
    break;

  case JC_SH_3T:                                            //JM SH.3T
    Home3TimerMode = !Home3TimerMode;
    //fnInfo(Home3TimerMode);
    fnRefreshComboxState(CB_JC, JC_SH_3T, Home3TimerMode);                      //dr
    break;

  case JC_H_SUM:                                      //JM
    jm_HOME_SUM = !jm_HOME_SUM;
    fnRefreshComboxState(CB_JC, JC_H_SUM, jm_HOME_SUM);                //jm
    break;

  case JC_H_MIR:                                      //JM
    jm_HOME_MIR = !jm_HOME_MIR;
    fnRefreshComboxState(CB_JC, JC_H_MIR, jm_HOME_MIR);                //jm
    break;

  case JC_H_FIX:                                      //JM
    jm_HOME_FIX = !jm_HOME_FIX;
    fnRefreshComboxState(CB_JC, JC_H_FIX, jm_HOME_FIX);                //jm
    break;

  case JC_LARGELI:                                      //JM
    jm_LARGELI = !jm_LARGELI;
    fnRefreshComboxState(CB_JC, JC_LARGELI, jm_LARGELI);                //jm
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



   case JC_BLZ:                                              //JM bit LeadingZeros
     fnFlipFlag(FLAG_LEAD0);                                       //dr
     break;

   case JC_BCR:                                              //JM bit ComplexResult
     fnFlipFlag(FLAG_CPXRES);                                     //dr
     break;

   case JC_BSR:                                              //JM bit ComplexResult
     fnFlipFlag(FLAG_SPCRES);                                     //dr
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
  Norm_Key_00_VAR = sigmaAssign;
  fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
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

  case JM_INP_DFLT:
    if(Input_Default == ID_43S) { stringToLongInteger("0",10,mem); }
    else if(Input_Default == ID_DP) { stringToLongInteger("2",10,mem); }
    else if(Input_Default == ID_CPXDP) { stringToLongInteger("4",10,mem); }
    else if(Input_Default == ID_SI) { stringToLongInteger("6",10,mem); }
    else if(Input_Default == ID_LI) { stringToLongInteger("7",10,mem); }
    break;

  default:
    break;
  }

  convertLongIntegerToLongIntegerRegister(mem, REGISTER_X);
  longIntegerFree(mem);

}





/** integer to string
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
char* itoa(int value, char* result, int base) {      //DONE
    // check that the base if valid
    if (base < 2 || base > 16) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}



/********************************************//**
 * \brief Get item-value of assigned key to X
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnGetSigmaAssignToX(uint16_t unusedButMandatoryParameter) {       //DONE
  char snum[10];
  longInteger_t mem;
  longIntegerInit(mem);
  liftStack();

  itoa(Norm_Key_00_VAR, snum, 10);
  stringToLongInteger(snum,10,mem);

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



/********************************************//**
 * \brief
 *
 * \param[in] JM_KEY uint16_t
 * \return void
 ***********************************************/
void fnJMUSERmode(uint16_t JM_KEY) {      //DONE
  int16_t X_REG;
  longInteger_t lgInt;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);
    longIntegerToAllocatedString(lgInt, tmpString, TMP_STR_LENGTH);
    longIntegerToInt(lgInt,X_REG);
    longIntegerFree(lgInt);
  //printf("Xreg %d\n", X_REG);
    if(JM_KEY >= 256) {
      kbd_usr[JM_KEY - 256].primary = X_REG;
    //printf(".primary %d\n", kbd_usr[JM_KEY - 256].primary);
      fnSetFlag(FLAG_USER);
    }
  }
}



/********************************************//**
 * \brief
 *
 * \param[in] JM_KEY uint16_t
 * \return void
 ***********************************************/
void fnJMUSERmode_f(uint16_t JM_KEY) {      //DONE
  int16_t X_REG;
  longInteger_t lgInt;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);
    longIntegerToAllocatedString(lgInt, tmpString, TMP_STR_LENGTH);
    longIntegerToInt(lgInt,X_REG);
    longIntegerFree(lgInt);
  //printf("Xreg %d\n", X_REG);
    if(JM_KEY >= 256) {
      kbd_usr[JM_KEY - 256].fShifted = X_REG;
    //printf(".fShifted %d\n", kbd_usr[JM_KEY - 256].fShifted);
      fnSetFlag(FLAG_USER);
    }
  }
}



/********************************************//**
 * \brief
 *
 * \param[in] JM_KEY uint16_t
 * \return void
 ***********************************************/
void fnJMUSERmode_g(uint16_t JM_KEY) {      //DONE
  int16_t X_REG;
  longInteger_t lgInt;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);
    longIntegerToAllocatedString(lgInt, tmpString, TMP_STR_LENGTH);
    longIntegerToInt(lgInt,X_REG);
    longIntegerFree(lgInt);
  //printf("Xreg %d\n", X_REG);
    if(JM_KEY >= 256) {
      kbd_usr[JM_KEY - 256].gShifted = X_REG;
      //printf(".gShifted %d\n", kbd_usr[JM_KEY - 256].gShifted);
      fnSetFlag(FLAG_USER);
    }
  }
}



//---------------------------------------------


void fnStrtoX(char aimBuffer[]) {      //DONE
  setSystemFlag(FLAG_ASLIFT);   // 5
  liftStack();
  int16_t mem = stringByteLength(aimBuffer);
  reallocateRegister(REGISTER_X, dtString, mem, AM_NONE);
  xcopy(REGISTER_STRING_DATA(REGISTER_X), aimBuffer, mem + 1);
}



void fnStrInputReal34(char inp1[]) {  // CONVERT STRING to REAL IN X      //DONE
  tmpString[0] = 0;
  strcat(tmpString, inp1);
  setSystemFlag(FLAG_ASLIFT);   // 5
  liftStack();
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  stringToReal34(tmpString, REGISTER_REAL34_DATA(REGISTER_X));
}



void fnStrInputLongint(char inp1[]) {  // CONVERT STRING to Longint X      //DONE
  tmpString[0]=0;
  strcat(tmpString, inp1);
  setSystemFlag(FLAG_ASLIFT);   // 5
  liftStack();

  longInteger_t lgInt;
  longIntegerInit(lgInt);
  stringToLongInteger(tmpString + (tmpString[0] == '+' ? 1 : 0), 10, lgInt);
  convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
  longIntegerFree(lgInt);
}




void fnRCL(int16_t inp) {      //DONE
  setSystemFlag(FLAG_ASLIFT);
  if(inp == TEMP_REGISTER_1) {
    liftStack();
    copySourceRegisterToDestRegister(inp, REGISTER_X);
  } else {
  fnRecall(inp);
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
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
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
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);   // STO TMP
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
      ramDump();
    #endif
  }
  


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
  case USER_DEFAULTS:                                           //USER_DEFAULTS FOR USER: E+ CC
    kbd_usr[0].primary     = ITM_CC;
    kbd_usr[0].fShifted    = ITM_RI;
    kbd_usr[0].gShifted    = ITM_LI;
    Norm_Key_00_VAR        = ITM_CC;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;


#if defined (JM_LAYOUT_1A)  && !defined (TESTSUITE_BUILD)                          //JM LAYOUT 1A.
  case USER_COMPLEX:                                            //USER_COMPLEX FOR USER: U^ ENTER^ CC
    kbd_usr[12].fShifted   = ITM_CC;                            //JM Changed CPX menu therefore USER MODE changes
    kbd_usr[0].primary     = -MNU_MyMenu;
    kbd_usr[0].fShifted    = ITM_RI;
    kbd_usr[0].gShifted    = ITM_LI;
    Norm_Key_00_VAR        = -MNU_MyMenu;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;

  case USER_CC:                                            //USER_COMPLEX FOR USER: U^ ENTER^ CC
    kbd_usr[0].primary     = ITM_CC;
    Norm_Key_00_VAR        = ITM_CC;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;
#endif

#if defined (JM_LAYOUT_2_DM42_STRICT) && !defined (TESTSUITE_BUILD)              //JM LAYOUT 42
  case USER_COMPLEX:                                            //USER_COMPLEX FOR USER: U^ ENTER^ CC
    //kbd_usr[12].gShifted   = ITM_CC;
    kbd_usr[0].primary     = -MNU_MyMenu;
    kbd_usr[0].fShifted    = ITM_RI;
    kbd_usr[0].gShifted    = ITM_LI;
    Norm_Key_00_VAR        = -MNU_MyMenu;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;

  case USER_CC:                                            //USER_COMPLEX FOR USER: U^ ENTER^ CC
    kbd_usr[0].primary     = ITM_CC;
    Norm_Key_00_VAR        = ITM_CC;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;
#endif


  case USER_V43LT:

    fnUserJM(USER_V43);

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


    kbd_usr[4].primary    = ITM_STO;
    kbd_usr[4].fShifted   = ITM_MAGNITUDE;
    kbd_usr[4].gShifted   = ITM_ANGLE;
    kbd_usr[4].primaryAim     = ITM_E;

    kbd_usr[5].primary    = ITM_RCL;
    kbd_usr[5].fShifted   = ITM_PC;
    kbd_usr[5].gShifted   = ITM_DELTAPC;
    kbd_usr[5].primaryAim     = ITM_F;

    Norm_Key_00_VAR        = ITM_USERMODE;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;


  case USER_V43:          //USER

    fnUserJM(USER_RESET);

    kbd_usr[0].primary      = ITM_1ONX;
    kbd_usr[0].fShifted     = ITM_YX;
    kbd_usr[0].gShifted     = ITM_HASH_JM/*ITM_toINT*/;

    kbd_usr[1].primary      = ITM_SQUAREROOTX;
    kbd_usr[1].fShifted     = ITM_SQUARE;
    kbd_usr[1].gShifted     = ITM_ms;

    kbd_usr[2].primary      = ITM_LOG10;
    kbd_usr[2].fShifted     = ITM_10x;
    kbd_usr[2].gShifted     = ITM_dotD;

    kbd_usr[3].primary      = ITM_LN;
    kbd_usr[3].fShifted     = ITM_EXP;
    kbd_usr[3].gShifted     = ITM_TGLFRT;

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
    kbd_usr[27].fShifted    = -MNU_STK;
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

    Norm_Key_00_VAR         = ITM_1ONX;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;



  case USER_C43:          //USER
    fnUserJM(USER_RESET);
    kbd_usr[0].primary=ITM_SIGMAPLUS;                 kbd_usr[0].fShifted=ITM_RI;                       kbd_usr[0].gShifted=ITM_TGLFRT;                   kbd_usr[0].keyLblAim=ITM_NULL;                    kbd_usr[0].primaryAim=ITM_A;                      kbd_usr[0].fShiftedAim=ITM_a;                     kbd_usr[0].gShiftedAim=ITM_ALPHA;                 kbd_usr[0].primaryTam=ITM_REG_A;                  
    kbd_usr[1].primary=ITM_1ONX;                      kbd_usr[1].fShifted=ITM_YX;                       kbd_usr[1].gShifted=ITM_HASH_JM;                  kbd_usr[1].keyLblAim=ITM_NUMBER_SIGN;             kbd_usr[1].primaryAim=ITM_B;                      kbd_usr[1].fShiftedAim=ITM_b;                     kbd_usr[1].gShiftedAim=ITM_BETA;                  kbd_usr[1].primaryTam=ITM_REG_B;                  
    kbd_usr[2].primary=ITM_SQUAREROOTX;               kbd_usr[2].fShifted=ITM_SQUARE;                   kbd_usr[2].gShifted=ITM_ms;                       kbd_usr[2].keyLblAim=ITM_CHECK_MARK;              kbd_usr[2].primaryAim=ITM_C;                      kbd_usr[2].fShiftedAim=ITM_c;                     kbd_usr[2].gShiftedAim=ITM_CHI;                   kbd_usr[2].primaryTam=ITM_REG_C;                  
    kbd_usr[3].primary=ITM_LOG10;                     kbd_usr[3].fShifted=ITM_10x;                      kbd_usr[3].gShifted=ITM_dotD;                     kbd_usr[3].keyLblAim=ITM_NULL;                    kbd_usr[3].primaryAim=ITM_D;                      kbd_usr[3].fShiftedAim=ITM_d;                     kbd_usr[3].gShiftedAim=ITM_DELTA;                 kbd_usr[3].primaryTam=ITM_REG_D;                  
    kbd_usr[4].primary=ITM_LN;                        kbd_usr[4].fShifted=ITM_EXP;                      kbd_usr[4].gShifted=ITM_toREC2;                   kbd_usr[4].keyLblAim=ITM_NULL;                    kbd_usr[4].primaryAim=ITM_E;                      kbd_usr[4].fShiftedAim=ITM_e;                     kbd_usr[4].gShiftedAim=ITM_EPSILON;               kbd_usr[4].primaryTam=ITM_NULL;                   
    kbd_usr[5].primary=ITM_XEQ;                       kbd_usr[5].fShifted=ITM_AIM;                      kbd_usr[5].gShifted=ITM_toPOL2;                   kbd_usr[5].keyLblAim=ITM_NULL;                    kbd_usr[5].primaryAim=ITM_F;                      kbd_usr[5].fShiftedAim=ITM_f;                     kbd_usr[5].gShiftedAim=ITM_DIGAMMA;               kbd_usr[5].primaryTam=ITM_alpha;                  
    kbd_usr[6].primary=ITM_STO;                       kbd_usr[6].fShifted=ITM_MAGNITUDE;                kbd_usr[6].gShifted=ITM_ANGLE;                    kbd_usr[6].keyLblAim=ITM_NULL;                    kbd_usr[6].primaryAim=ITM_G;                      kbd_usr[6].fShiftedAim=ITM_g;                     kbd_usr[6].gShiftedAim=ITM_GAMMA;                 kbd_usr[6].primaryTam=ITM_NULL;                   
    kbd_usr[7].primary=ITM_RCL;                       kbd_usr[7].fShifted=ITM_PC;                       kbd_usr[7].gShifted=ITM_DELTAPC;                  kbd_usr[7].keyLblAim=ITM_NULL;                    kbd_usr[7].primaryAim=ITM_H;                      kbd_usr[7].fShiftedAim=ITM_h;                     kbd_usr[7].gShiftedAim=ITM_ETA;                   kbd_usr[7].primaryTam=ITM_HEX;                    
    kbd_usr[8].primary=ITM_Rdown;                     kbd_usr[8].fShifted=ITM_CONSTpi;                  kbd_usr[8].gShifted=ITM_XTHROOT;                  kbd_usr[8].keyLblAim=ITM_NULL;                    kbd_usr[8].primaryAim=ITM_I;                      kbd_usr[8].fShiftedAim=ITM_i;                     kbd_usr[8].gShiftedAim=ITM_IOTA;                  kbd_usr[8].primaryTam=ITM_REG_I;                  
    kbd_usr[9].primary=ITM_sin;                       kbd_usr[9].fShifted=ITM_arcsin;                   kbd_usr[9].gShifted=ITM_GTO;                      kbd_usr[9].keyLblAim=ITM_NULL;                    kbd_usr[9].primaryAim=ITM_J;                      kbd_usr[9].fShiftedAim=ITM_j;                     kbd_usr[9].gShiftedAim=ITM_THETA;                 kbd_usr[9].primaryTam=ITM_REG_J;                  
    kbd_usr[10].primary=ITM_cos;                      kbd_usr[10].fShifted=ITM_arccos;                  kbd_usr[10].gShifted=ITM_LBL;                     kbd_usr[10].keyLblAim=ITM_NULL;                   kbd_usr[10].primaryAim=ITM_K;                     kbd_usr[10].fShiftedAim=ITM_k;                    kbd_usr[10].gShiftedAim=ITM_KAPPA;                kbd_usr[10].primaryTam=ITM_REG_K;                 
    kbd_usr[11].primary=ITM_tan;                      kbd_usr[11].fShifted=ITM_arctan;                  kbd_usr[11].gShifted=ITM_RTN;                     kbd_usr[11].keyLblAim=ITM_NULL;                   kbd_usr[11].primaryAim=ITM_L;                     kbd_usr[11].fShiftedAim=ITM_l;                    kbd_usr[11].gShiftedAim=ITM_LAMBDA;               kbd_usr[11].primaryTam=ITM_REG_L;                 
    kbd_usr[12].primary=ITM_ENTER;                    kbd_usr[12].fShifted=KEY_COMPLEX;                 kbd_usr[12].gShifted=-MNU_CPX;                    kbd_usr[12].keyLblAim=ITM_ENTER;                  kbd_usr[12].primaryAim=ITM_ENTER;                 kbd_usr[12].fShiftedAim=ITM_XEDIT;                kbd_usr[12].gShiftedAim=ITM_NULL;                 kbd_usr[12].primaryTam=ITM_ENTER;                 
    kbd_usr[13].primary=ITM_XexY;                     kbd_usr[13].fShifted=ITM_LASTX;                   kbd_usr[13].gShifted=ITM_Rup;                     kbd_usr[13].keyLblAim=ITM_ex;                     kbd_usr[13].primaryAim=ITM_M;                     kbd_usr[13].fShiftedAim=ITM_m;                    kbd_usr[13].gShiftedAim=ITM_MU;                   kbd_usr[13].primaryTam=ITM_NULL;                  
    kbd_usr[14].primary=ITM_CHS;                      kbd_usr[14].fShifted=-MNU_MODE;                   kbd_usr[14].gShifted=-MNU_STK;                    kbd_usr[14].keyLblAim=ITM_PLUS_MINUS;             kbd_usr[14].primaryAim=ITM_N;                     kbd_usr[14].fShiftedAim=ITM_n;                    kbd_usr[14].gShiftedAim=ITM_NU;                   kbd_usr[14].primaryTam=ITM_NULL;                  
    kbd_usr[15].primary=ITM_EXPONENT;                 kbd_usr[15].fShifted=-MNU_DSP;                    kbd_usr[15].gShifted=-MNU_EXP;                    kbd_usr[15].keyLblAim=ITM_NULL;                   kbd_usr[15].primaryAim=ITM_O;                     kbd_usr[15].fShiftedAim=ITM_o;                    kbd_usr[15].gShiftedAim=ITM_OMICRON;              kbd_usr[15].primaryTam=ITM_NULL;                  
    kbd_usr[16].primary=ITM_BACKSPACE;                kbd_usr[16].fShifted=ITM_UNDO;                    kbd_usr[16].gShifted=-MNU_CLR;                    kbd_usr[16].keyLblAim=ITM_BACKSPACE;              kbd_usr[16].primaryAim=ITM_BACKSPACE;             kbd_usr[16].fShiftedAim=ITM_CLA;                  kbd_usr[16].gShiftedAim=ITM_NULL;                 kbd_usr[16].primaryTam=ITM_BACKSPACE;             
    kbd_usr[17].primary=ITM_UP1;                      kbd_usr[17].fShifted=ITM_BST;                     kbd_usr[17].gShifted=ITM_RBR;                     kbd_usr[17].keyLblAim=ITM_UP1;                    kbd_usr[17].primaryAim=ITM_UP1;                   kbd_usr[17].fShiftedAim=CHR_caseUP;               kbd_usr[17].gShiftedAim=ITM_UP_ARROW;             kbd_usr[17].primaryTam=ITM_UP1;                   
    kbd_usr[18].primary=ITM_7;                        kbd_usr[18].fShifted=-MNU_EQN;                    kbd_usr[18].gShifted=-MNU_HOME;                   kbd_usr[18].keyLblAim=ITM_7;                      kbd_usr[18].primaryAim=ITM_P;                     kbd_usr[18].fShiftedAim=ITM_p;                    kbd_usr[18].gShiftedAim=ITM_PI;                   kbd_usr[18].primaryTam=ITM_7;                     
    kbd_usr[19].primary=ITM_8;                        kbd_usr[19].fShifted=-MNU_ADV;                    kbd_usr[19].gShifted=-MNU_CONST;                  kbd_usr[19].keyLblAim=ITM_8;                      kbd_usr[19].primaryAim=ITM_Q;                     kbd_usr[19].fShiftedAim=ITM_q;                    kbd_usr[19].gShiftedAim=ITM_QOPPA;                kbd_usr[19].primaryTam=ITM_8;                     
    kbd_usr[20].primary=ITM_9;                        kbd_usr[20].fShifted=-MNU_MATX;                   kbd_usr[20].gShifted=-MNU_XFN;                    kbd_usr[20].keyLblAim=ITM_9;                      kbd_usr[20].primaryAim=ITM_R;                     kbd_usr[20].fShiftedAim=ITM_r;                    kbd_usr[20].gShiftedAim=ITM_RHO;                  kbd_usr[20].primaryTam=ITM_9;                     
    kbd_usr[21].primary=ITM_DIV;                      kbd_usr[21].fShifted=-MNU_STAT;                   kbd_usr[21].gShifted=-MNU_SUMS;                   kbd_usr[21].keyLblAim=ITM_OBELUS;                 kbd_usr[21].primaryAim=ITM_S;                     kbd_usr[21].fShiftedAim=ITM_s;                    kbd_usr[21].gShiftedAim=ITM_SIGMA;                kbd_usr[21].primaryTam=ITM_DIV;                   
    kbd_usr[22].primary=ITM_DOWN1;                    kbd_usr[22].fShifted=ITM_SST;                     kbd_usr[22].gShifted=ITM_FLGSV;                   kbd_usr[22].keyLblAim=ITM_DOWN1;                  kbd_usr[22].primaryAim=ITM_DOWN1;                 kbd_usr[22].fShiftedAim=CHR_caseDN;               kbd_usr[22].gShiftedAim=ITM_DOWN_ARROW;           kbd_usr[22].primaryTam=ITM_DOWN1;                 
    kbd_usr[23].primary=ITM_4;                        kbd_usr[23].fShifted=-MNU_BASE;                   kbd_usr[23].gShifted=-MNU_CLK;                    kbd_usr[23].keyLblAim=ITM_4;                      kbd_usr[23].primaryAim=ITM_T;                     kbd_usr[23].fShiftedAim=ITM_t;                    kbd_usr[23].gShiftedAim=ITM_TAU;                  kbd_usr[23].primaryTam=ITM_4;                     
    kbd_usr[24].primary=ITM_5;                        kbd_usr[24].fShifted=-MNU_ANGLECONV;              kbd_usr[24].gShifted=-MNU_UNITCONV;               kbd_usr[24].keyLblAim=ITM_5;                      kbd_usr[24].primaryAim=ITM_U;                     kbd_usr[24].fShiftedAim=ITM_u;                    kbd_usr[24].gShiftedAim=ITM_PHI;                  kbd_usr[24].primaryTam=ITM_5;                     
    kbd_usr[25].primary=ITM_6;                        kbd_usr[25].fShifted=-MNU_FLAGS;                  kbd_usr[25].gShifted=-MNU_BITS;                   kbd_usr[25].keyLblAim=ITM_6;                      kbd_usr[25].primaryAim=ITM_V;                     kbd_usr[25].fShiftedAim=ITM_v;                    kbd_usr[25].gShiftedAim=ITM_PSI;                  kbd_usr[25].primaryTam=ITM_6;                     
    kbd_usr[26].primary=ITM_MULT;                     kbd_usr[26].fShifted=-MNU_PROB;                   kbd_usr[26].gShifted=-MNU_INTS;                   kbd_usr[26].keyLblAim=ITM_CROSS;                  kbd_usr[26].primaryAim=ITM_W;                     kbd_usr[26].fShiftedAim=ITM_w;                    kbd_usr[26].gShiftedAim=ITM_OMEGA;                kbd_usr[26].primaryTam=ITM_MULT;                  
    kbd_usr[27].primary=KEY_fg;                       kbd_usr[27].fShifted=ITM_NULL;                    kbd_usr[27].gShifted=ITM_NULL;                    kbd_usr[27].keyLblAim=KEY_fg;                     kbd_usr[27].primaryAim=KEY_fg;                    kbd_usr[27].fShiftedAim=ITM_NULL;                 kbd_usr[27].gShiftedAim=ITM_NULL;                 kbd_usr[27].primaryTam=KEY_fg;                    
    kbd_usr[28].primary=ITM_1;                        kbd_usr[28].fShifted=ITM_ASSIGN;                  kbd_usr[28].gShifted=-MNU_ASN;                    kbd_usr[28].keyLblAim=ITM_1;                      kbd_usr[28].primaryAim=ITM_X;                     kbd_usr[28].fShiftedAim=ITM_x;                    kbd_usr[28].gShiftedAim=ITM_XI;                   kbd_usr[28].primaryTam=ITM_1;                     
    kbd_usr[29].primary=ITM_2;                        kbd_usr[29].fShifted=ITM_USERMODE;                kbd_usr[29].gShifted=-MNU_LOOP;                   kbd_usr[29].keyLblAim=ITM_2;                      kbd_usr[29].primaryAim=ITM_Y;                     kbd_usr[29].fShiftedAim=ITM_y;                    kbd_usr[29].gShiftedAim=ITM_UPSILON;              kbd_usr[29].primaryTam=ITM_2;                     
    kbd_usr[30].primary=ITM_3;                        kbd_usr[30].fShifted=-MNU_PARTS;                  kbd_usr[30].gShifted=-MNU_TEST;                   kbd_usr[30].keyLblAim=ITM_3;                      kbd_usr[30].primaryAim=ITM_Z;                     kbd_usr[30].fShiftedAim=ITM_z;                    kbd_usr[30].gShiftedAim=ITM_ZETA;                 kbd_usr[30].primaryTam=ITM_3;                     
    kbd_usr[31].primary=ITM_SUB;                      kbd_usr[31].fShifted=-MNU_FIN;                    kbd_usr[31].gShifted=-MNU_ALPHAFN;                kbd_usr[31].keyLblAim=ITM_MINUS;                  kbd_usr[31].primaryAim=ITM_UNDERSCORE;            kbd_usr[31].fShiftedAim=ITM_MINUS;                kbd_usr[31].gShiftedAim=ITM_SAMPI;                kbd_usr[31].primaryTam=ITM_SUB;                   
    kbd_usr[32].primary=ITM_EXIT1;                    kbd_usr[32].fShifted=ITM_OFF;                     kbd_usr[32].gShifted=-MNU_PRINT;                  kbd_usr[32].keyLblAim=ITM_EXIT1;                  kbd_usr[32].primaryAim=ITM_EXIT1;                 kbd_usr[32].fShiftedAim=ITM_OFF;                  kbd_usr[32].gShiftedAim=ITM_PRINTER;              kbd_usr[32].primaryTam=ITM_EXIT1;                 
    kbd_usr[33].primary=ITM_0;                        kbd_usr[33].fShifted=ITM_VIEW;                    kbd_usr[33].gShifted=ITM_TIMER;                   kbd_usr[33].keyLblAim=ITM_0;                      kbd_usr[33].primaryAim=ITM_COLON;                 kbd_usr[33].fShiftedAim=ITM_0;                    kbd_usr[33].gShiftedAim=-MNU_ALPHA;               kbd_usr[33].primaryTam=ITM_0;                     
    kbd_usr[34].primary=ITM_PERIOD;                   kbd_usr[34].fShifted=ITM_SHOW;                    kbd_usr[34].gShifted=-MNU_INFO;                   kbd_usr[34].keyLblAim=ITM_PERIOD;                 kbd_usr[34].primaryAim=ITM_COMMA;                 kbd_usr[34].fShiftedAim=ITM_PERIOD;               kbd_usr[34].gShiftedAim=-MNU_ALPHADOT;            kbd_usr[34].primaryTam=ITM_PERIOD;                
    kbd_usr[35].primary=ITM_RS;                       kbd_usr[35].fShifted=ITM_PR;                      kbd_usr[35].gShifted=-MNU_PFN;                    kbd_usr[35].keyLblAim=ITM_NULL;                   kbd_usr[35].primaryAim=ITM_QUESTION_MARK;         kbd_usr[35].fShiftedAim=ITM_SLASH;                kbd_usr[35].gShiftedAim=-MNU_ALPHAMATH;           kbd_usr[35].primaryTam=ITM_NULL;                  
    kbd_usr[36].primary=ITM_ADD;                      kbd_usr[36].fShifted=-MNU_CATALOG;                kbd_usr[36].gShifted=-MNU_IO;                     kbd_usr[36].keyLblAim=ITM_PLUS;                   kbd_usr[36].primaryAim=ITM_SPACE;                 kbd_usr[36].fShiftedAim=ITM_PLUS;                 kbd_usr[36].gShiftedAim=-MNU_ALPHAINTL;           kbd_usr[36].primaryTam=ITM_ADD;                   
    fnSetFlag(FLAG_USER);
    break;



  case USER_DM42:
    fnUserJM(USER_RESET);
    kbd_usr[0].primary=ITM_SIGMAPLUS;                 kbd_usr[0].fShifted=ITM_SIGMAMINUS;               kbd_usr[0].gShifted=ITM_TGLFRT;                   kbd_usr[0].keyLblAim=ITM_NULL;                    kbd_usr[0].primaryAim=ITM_A;                      kbd_usr[0].fShiftedAim=ITM_a;                     kbd_usr[0].gShiftedAim=ITM_ALPHA;                 kbd_usr[0].primaryTam=ITM_REG_A;                  
    kbd_usr[1].primary=ITM_1ONX;                      kbd_usr[1].fShifted=ITM_YX;                       kbd_usr[1].gShifted=ITM_HASH_JM;                  kbd_usr[1].keyLblAim=ITM_NUMBER_SIGN;             kbd_usr[1].primaryAim=ITM_B;                      kbd_usr[1].fShiftedAim=ITM_b;                     kbd_usr[1].gShiftedAim=ITM_BETA;                  kbd_usr[1].primaryTam=ITM_REG_B;                  
    kbd_usr[2].primary=ITM_SQUAREROOTX;               kbd_usr[2].fShifted=ITM_SQUARE;                   kbd_usr[2].gShifted=ITM_ms;                       kbd_usr[2].keyLblAim=ITM_CHECK_MARK;              kbd_usr[2].primaryAim=ITM_C;                      kbd_usr[2].fShiftedAim=ITM_c;                     kbd_usr[2].gShiftedAim=ITM_CHI;                   kbd_usr[2].primaryTam=ITM_REG_C;                  
    kbd_usr[3].primary=ITM_LOG10;                     kbd_usr[3].fShifted=ITM_10x;                      kbd_usr[3].gShifted=ITM_dotD;                     kbd_usr[3].keyLblAim=ITM_NULL;                    kbd_usr[3].primaryAim=ITM_D;                      kbd_usr[3].fShiftedAim=ITM_d;                     kbd_usr[3].gShiftedAim=ITM_DELTA;                 kbd_usr[3].primaryTam=ITM_REG_D;                  
    kbd_usr[4].primary=ITM_LN;                        kbd_usr[4].fShifted=ITM_EXP;                      kbd_usr[4].gShifted=ITM_toREC2;                   kbd_usr[4].keyLblAim=ITM_NULL;                    kbd_usr[4].primaryAim=ITM_E;                      kbd_usr[4].fShiftedAim=ITM_e;                     kbd_usr[4].gShiftedAim=ITM_EPSILON;               kbd_usr[4].primaryTam=ITM_NULL;                   
    kbd_usr[5].primary=ITM_XEQ;                       kbd_usr[5].fShifted=ITM_GTO;                      kbd_usr[5].gShifted=ITM_toPOL2;                   kbd_usr[5].keyLblAim=ITM_NULL;                    kbd_usr[5].primaryAim=ITM_F;                      kbd_usr[5].fShiftedAim=ITM_f;                     kbd_usr[5].gShiftedAim=ITM_DIGAMMA;               kbd_usr[5].primaryTam=ITM_alpha;                  
    kbd_usr[6].primary=ITM_STO;                       kbd_usr[6].fShifted=KEY_COMPLEX;                  kbd_usr[6].gShifted=ITM_MAGNITUDE;                kbd_usr[6].keyLblAim=ITM_NULL;                    kbd_usr[6].primaryAim=ITM_G;                      kbd_usr[6].fShiftedAim=ITM_g;                     kbd_usr[6].gShiftedAim=ITM_GAMMA;                 kbd_usr[6].primaryTam=ITM_NULL;                   
    kbd_usr[7].primary=ITM_RCL;                       kbd_usr[7].fShifted=ITM_PC;                       kbd_usr[7].gShifted=ITM_ANGLE;                    kbd_usr[7].keyLblAim=ITM_NULL;                    kbd_usr[7].primaryAim=ITM_H;                      kbd_usr[7].fShiftedAim=ITM_h;                     kbd_usr[7].gShiftedAim=ITM_ETA;                   kbd_usr[7].primaryTam=ITM_HEX;                    
    kbd_usr[8].primary=ITM_Rdown;                     kbd_usr[8].fShifted=ITM_CONSTpi;                  kbd_usr[8].gShifted=ITM_XTHROOT;                  kbd_usr[8].keyLblAim=ITM_NULL;                    kbd_usr[8].primaryAim=ITM_I;                      kbd_usr[8].fShiftedAim=ITM_i;                     kbd_usr[8].gShiftedAim=ITM_IOTA;                  kbd_usr[8].primaryTam=ITM_REG_I;                  
    kbd_usr[9].primary=ITM_sin;                       kbd_usr[9].fShifted=ITM_arcsin;                   kbd_usr[9].gShifted=ITM_CC;                       kbd_usr[9].keyLblAim=ITM_NULL;                    kbd_usr[9].primaryAim=ITM_J;                      kbd_usr[9].fShiftedAim=ITM_j;                     kbd_usr[9].gShiftedAim=ITM_THETA;                 kbd_usr[9].primaryTam=ITM_REG_J;                  
    kbd_usr[10].primary=ITM_cos;                      kbd_usr[10].fShifted=ITM_arccos;                  kbd_usr[10].gShifted=ITM_LBL;                     kbd_usr[10].keyLblAim=ITM_NULL;                   kbd_usr[10].primaryAim=ITM_K;                     kbd_usr[10].fShiftedAim=ITM_k;                    kbd_usr[10].gShiftedAim=ITM_KAPPA;                kbd_usr[10].primaryTam=ITM_REG_K;                 
    kbd_usr[11].primary=ITM_tan;                      kbd_usr[11].fShifted=ITM_arctan;                  kbd_usr[11].gShifted=ITM_RTN;                     kbd_usr[11].keyLblAim=ITM_NULL;                   kbd_usr[11].primaryAim=ITM_L;                     kbd_usr[11].fShiftedAim=ITM_l;                    kbd_usr[11].gShiftedAim=ITM_LAMBDA;               kbd_usr[11].primaryTam=ITM_REG_L;                 
    kbd_usr[12].primary=ITM_ENTER;                    kbd_usr[12].fShifted=ITM_AIM;                     kbd_usr[12].gShifted=-MNU_CPX;                    kbd_usr[12].keyLblAim=ITM_ENTER;                  kbd_usr[12].primaryAim=ITM_ENTER;                 kbd_usr[12].fShiftedAim=ITM_XEDIT;                kbd_usr[12].gShiftedAim=ITM_NULL;                 kbd_usr[12].primaryTam=ITM_ENTER;                 
    kbd_usr[13].primary=ITM_XexY;                     kbd_usr[13].fShifted=ITM_LASTX;                   kbd_usr[13].gShifted=ITM_Rup;                     kbd_usr[13].keyLblAim=ITM_ex;                     kbd_usr[13].primaryAim=ITM_M;                     kbd_usr[13].fShiftedAim=ITM_m;                    kbd_usr[13].gShiftedAim=ITM_MU;                   kbd_usr[13].primaryTam=ITM_NULL;                  
    kbd_usr[14].primary=ITM_CHS;                      kbd_usr[14].fShifted=-MNU_MODE;                   kbd_usr[14].gShifted=-MNU_STK;                    kbd_usr[14].keyLblAim=ITM_PLUS_MINUS;             kbd_usr[14].primaryAim=ITM_N;                     kbd_usr[14].fShiftedAim=ITM_n;                    kbd_usr[14].gShiftedAim=ITM_NU;                   kbd_usr[14].primaryTam=ITM_NULL;                  
    kbd_usr[15].primary=ITM_EXPONENT;                 kbd_usr[15].fShifted=-MNU_DSP;                    kbd_usr[15].gShifted=-MNU_EXP;                    kbd_usr[15].keyLblAim=ITM_NULL;                   kbd_usr[15].primaryAim=ITM_O;                     kbd_usr[15].fShiftedAim=ITM_o;                    kbd_usr[15].gShiftedAim=ITM_OMICRON;              kbd_usr[15].primaryTam=ITM_NULL;                  
    kbd_usr[16].primary=ITM_BACKSPACE;                kbd_usr[16].fShifted=-MNU_CLR;                    kbd_usr[16].gShifted=ITM_UNDO;                    kbd_usr[16].keyLblAim=ITM_BACKSPACE;              kbd_usr[16].primaryAim=ITM_BACKSPACE;             kbd_usr[16].fShiftedAim=ITM_CLA;                  kbd_usr[16].gShiftedAim=ITM_NULL;                 kbd_usr[16].primaryTam=ITM_BACKSPACE;             
    kbd_usr[17].primary=ITM_UP1;                      kbd_usr[17].fShifted=ITM_BST;                     kbd_usr[17].gShifted=ITM_RBR;                     kbd_usr[17].keyLblAim=ITM_UP1;                    kbd_usr[17].primaryAim=ITM_UP1;                   kbd_usr[17].fShiftedAim=CHR_caseUP;               kbd_usr[17].gShiftedAim=ITM_UP_ARROW;             kbd_usr[17].primaryTam=ITM_UP1;                   
    kbd_usr[18].primary=ITM_7;                        kbd_usr[18].fShifted=-MNU_EQN;                    kbd_usr[18].gShifted=-MNU_INFO;                   kbd_usr[18].keyLblAim=ITM_7;                      kbd_usr[18].primaryAim=ITM_P;                     kbd_usr[18].fShiftedAim=ITM_p;                    kbd_usr[18].gShiftedAim=ITM_PI;                   kbd_usr[18].primaryTam=ITM_7;                     
    kbd_usr[19].primary=ITM_8;                        kbd_usr[19].fShifted=-MNU_ADV;                    kbd_usr[19].gShifted=-MNU_CONST;                  kbd_usr[19].keyLblAim=ITM_8;                      kbd_usr[19].primaryAim=ITM_Q;                     kbd_usr[19].fShiftedAim=ITM_q;                    kbd_usr[19].gShiftedAim=ITM_QOPPA;                kbd_usr[19].primaryTam=ITM_8;                     
    kbd_usr[20].primary=ITM_9;                        kbd_usr[20].fShifted=-MNU_MATX;                   kbd_usr[20].gShifted=-MNU_XFN;                    kbd_usr[20].keyLblAim=ITM_9;                      kbd_usr[20].primaryAim=ITM_R;                     kbd_usr[20].fShiftedAim=ITM_r;                    kbd_usr[20].gShiftedAim=ITM_RHO;                  kbd_usr[20].primaryTam=ITM_9;                     
    kbd_usr[21].primary=ITM_DIV;                      kbd_usr[21].fShifted=-MNU_STAT;                   kbd_usr[21].gShifted=-MNU_SUMS;                   kbd_usr[21].keyLblAim=ITM_OBELUS;                 kbd_usr[21].primaryAim=ITM_S;                     kbd_usr[21].fShiftedAim=ITM_s;                    kbd_usr[21].gShiftedAim=ITM_SIGMA;                kbd_usr[21].primaryTam=ITM_DIV;                   
    kbd_usr[22].primary=ITM_DOWN1;                    kbd_usr[22].fShifted=ITM_SST;                     kbd_usr[22].gShifted=ITM_FLGSV;                   kbd_usr[22].keyLblAim=ITM_DOWN1;                  kbd_usr[22].primaryAim=ITM_DOWN1;                 kbd_usr[22].fShiftedAim=CHR_caseDN;               kbd_usr[22].gShiftedAim=ITM_DOWN_ARROW;           kbd_usr[22].primaryTam=ITM_DOWN1;                 
    kbd_usr[23].primary=ITM_4;                        kbd_usr[23].fShifted=-MNU_BASE;                   kbd_usr[23].gShifted=-MNU_FIN;                    kbd_usr[23].keyLblAim=ITM_4;                      kbd_usr[23].primaryAim=ITM_T;                     kbd_usr[23].fShiftedAim=ITM_t;                    kbd_usr[23].gShiftedAim=ITM_TAU;                  kbd_usr[23].primaryTam=ITM_4;                     
    kbd_usr[24].primary=ITM_5;                        kbd_usr[24].fShifted=-MNU_ANGLECONV;              kbd_usr[24].gShifted=-MNU_UNITCONV;               kbd_usr[24].keyLblAim=ITM_5;                      kbd_usr[24].primaryAim=ITM_U;                     kbd_usr[24].fShiftedAim=ITM_u;                    kbd_usr[24].gShiftedAim=ITM_PHI;                  kbd_usr[24].primaryTam=ITM_5;                     
    kbd_usr[25].primary=ITM_6;                        kbd_usr[25].fShifted=-MNU_FLAGS;                  kbd_usr[25].gShifted=-MNU_BITS;                   kbd_usr[25].keyLblAim=ITM_6;                      kbd_usr[25].primaryAim=ITM_V;                     kbd_usr[25].fShiftedAim=ITM_v;                    kbd_usr[25].gShiftedAim=ITM_PSI;                  kbd_usr[25].primaryTam=ITM_6;                     
    kbd_usr[26].primary=ITM_MULT;                     kbd_usr[26].fShifted=-MNU_PROB;                   kbd_usr[26].gShifted=-MNU_INTS;                   kbd_usr[26].keyLblAim=ITM_CROSS;                  kbd_usr[26].primaryAim=ITM_W;                     kbd_usr[26].fShiftedAim=ITM_w;                    kbd_usr[26].gShiftedAim=ITM_OMEGA;                kbd_usr[26].primaryTam=ITM_MULT;                  
    kbd_usr[27].primary=KEY_fg;                       kbd_usr[27].fShifted=ITM_NULL;                    kbd_usr[27].gShifted=ITM_NULL;                    kbd_usr[27].keyLblAim=KEY_fg;                     kbd_usr[27].primaryAim=KEY_fg;                    kbd_usr[27].fShiftedAim=ITM_NULL;                 kbd_usr[27].gShiftedAim=ITM_NULL;                 kbd_usr[27].primaryTam=KEY_fg;                    
    kbd_usr[28].primary=ITM_1;                        kbd_usr[28].fShifted=ITM_ASSIGN;                  kbd_usr[28].gShifted=-MNU_ASN;                    kbd_usr[28].keyLblAim=ITM_1;                      kbd_usr[28].primaryAim=ITM_X;                     kbd_usr[28].fShiftedAim=ITM_x;                    kbd_usr[28].gShiftedAim=ITM_XI;                   kbd_usr[28].primaryTam=ITM_1;                     
    kbd_usr[29].primary=ITM_2;                        kbd_usr[29].fShifted=ITM_USERMODE;                kbd_usr[29].gShifted=-MNU_LOOP;                   kbd_usr[29].keyLblAim=ITM_2;                      kbd_usr[29].primaryAim=ITM_Y;                     kbd_usr[29].fShiftedAim=ITM_y;                    kbd_usr[29].gShiftedAim=ITM_UPSILON;              kbd_usr[29].primaryTam=ITM_2;                     
    kbd_usr[30].primary=ITM_3;                        kbd_usr[30].fShifted=-MNU_PFN;                    kbd_usr[30].gShifted=-MNU_TEST;                   kbd_usr[30].keyLblAim=ITM_3;                      kbd_usr[30].primaryAim=ITM_Z;                     kbd_usr[30].fShiftedAim=ITM_z;                    kbd_usr[30].gShiftedAim=ITM_ZETA;                 kbd_usr[30].primaryTam=ITM_3;                     
    kbd_usr[31].primary=ITM_SUB;                      kbd_usr[31].fShifted=-MNU_IO;                     kbd_usr[31].gShifted=-MNU_ALPHAFN;                kbd_usr[31].keyLblAim=ITM_MINUS;                  kbd_usr[31].primaryAim=ITM_UNDERSCORE;            kbd_usr[31].fShiftedAim=ITM_MINUS;                kbd_usr[31].gShiftedAim=ITM_SAMPI;                kbd_usr[31].primaryTam=ITM_SUB;                   
    kbd_usr[32].primary=ITM_EXIT1;                    kbd_usr[32].fShifted=ITM_OFF;                     kbd_usr[32].gShifted=-MNU_PRINT;                  kbd_usr[32].keyLblAim=ITM_EXIT1;                  kbd_usr[32].primaryAim=ITM_EXIT1;                 kbd_usr[32].fShiftedAim=ITM_OFF;                  kbd_usr[32].gShiftedAim=ITM_PRINTER;              kbd_usr[32].primaryTam=ITM_EXIT1;                 
    kbd_usr[33].primary=ITM_0;                        kbd_usr[33].fShifted=-MNU_HOME;                   kbd_usr[33].gShifted=ITM_TIMER;                   kbd_usr[33].keyLblAim=ITM_0;                      kbd_usr[33].primaryAim=ITM_COLON;                 kbd_usr[33].fShiftedAim=ITM_0;                    kbd_usr[33].gShiftedAim=-MNU_ALPHA;               kbd_usr[33].primaryTam=ITM_0;                     
    kbd_usr[34].primary=ITM_PERIOD;                   kbd_usr[34].fShifted=ITM_SHOW;                    kbd_usr[34].gShifted=ITM_VIEW;                    kbd_usr[34].keyLblAim=ITM_PERIOD;                 kbd_usr[34].primaryAim=ITM_COMMA;                 kbd_usr[34].fShiftedAim=ITM_PERIOD;               kbd_usr[34].gShiftedAim=-MNU_ALPHADOT;            kbd_usr[34].primaryTam=ITM_PERIOD;                
    kbd_usr[35].primary=ITM_RS;                       kbd_usr[35].fShifted=ITM_PR;                      kbd_usr[35].gShifted=-MNU_CLK;                    kbd_usr[35].keyLblAim=ITM_NULL;                   kbd_usr[35].primaryAim=ITM_QUESTION_MARK;         kbd_usr[35].fShiftedAim=ITM_SLASH;                kbd_usr[35].gShiftedAim=-MNU_ALPHAMATH;           kbd_usr[35].primaryTam=ITM_NULL;                  
    kbd_usr[36].primary=ITM_ADD;                      kbd_usr[36].fShifted=-MNU_CATALOG;                kbd_usr[36].gShifted=-MNU_PARTS;                  kbd_usr[36].keyLblAim=ITM_PLUS;                   kbd_usr[36].primaryAim=ITM_SPACE;                 kbd_usr[36].fShiftedAim=ITM_PLUS;                 kbd_usr[36].gShiftedAim=-MNU_ALPHAINTL;           kbd_usr[36].primaryTam=ITM_ADD;                   
    fnSetFlag(FLAG_USER);
    break;



  case USER_43S:          //USER
    fnUserJM(USER_RESET);
    kbd_usr[0].primary=ITM_1ONX;                      kbd_usr[0].fShifted=ITM_TGLFRT;                   kbd_usr[0].gShifted=-MNU_ALPHAFN;                 kbd_usr[0].keyLblAim=ITM_NULL;                    kbd_usr[0].primaryAim=ITM_A;                      kbd_usr[0].fShiftedAim=-MNU_ALPHAINTL;            kbd_usr[0].gShiftedAim=ITM_ALPHA;                 kbd_usr[0].primaryTam=ITM_REG_A;                  
    kbd_usr[1].primary=ITM_YX;                        kbd_usr[1].fShifted=ITM_toINT;                    kbd_usr[1].gShifted=-MNU_EXP;                     kbd_usr[1].keyLblAim=ITM_NUMBER_SIGN;             kbd_usr[1].primaryAim=ITM_B;                      kbd_usr[1].fShiftedAim=ITM_NUMBER_SIGN;           kbd_usr[1].gShiftedAim=ITM_BETA;                  kbd_usr[1].primaryTam=ITM_REG_B;                  
    kbd_usr[2].primary=-MNU_TRI;                      kbd_usr[2].fShifted=ITM_DMS;                      kbd_usr[2].gShifted=ITM_CONSTpi;                  kbd_usr[2].keyLblAim=ITM_NULL;                    kbd_usr[2].primaryAim=ITM_C;                      kbd_usr[2].fShiftedAim=ITM_NULL;                  kbd_usr[2].gShiftedAim=ITM_GAMMA;                 kbd_usr[2].primaryTam=ITM_REG_C;                  
    kbd_usr[3].primary=ITM_LN;                        kbd_usr[3].fShifted=ITM_dotD;                     kbd_usr[3].gShifted=ITM_LOG10;                    kbd_usr[3].keyLblAim=ITM_NULL;                    kbd_usr[3].primaryAim=ITM_D;                      kbd_usr[3].fShiftedAim=ITM_NULL;                  kbd_usr[3].gShiftedAim=ITM_DELTA;                 kbd_usr[3].primaryTam=ITM_REG_D;                  
    kbd_usr[4].primary=ITM_EXP;                       kbd_usr[4].fShifted=ITM_toHMS;                    kbd_usr[4].gShifted=ITM_10x;                      kbd_usr[4].keyLblAim=ITM_NULL;                    kbd_usr[4].primaryAim=ITM_E;                      kbd_usr[4].fShiftedAim=ITM_NULL;                  kbd_usr[4].gShiftedAim=ITM_EPSILON;               kbd_usr[4].primaryTam=ITM_NULL;                   
    kbd_usr[5].primary=ITM_SQUARE;                    kbd_usr[5].fShifted=ITM_AIM;                      kbd_usr[5].gShifted=ITM_SQUAREROOTX;              kbd_usr[5].keyLblAim=ITM_CHECK_MARK;              kbd_usr[5].primaryAim=ITM_F;                      kbd_usr[5].fShiftedAim=ITM_CHECK_MARK;            kbd_usr[5].gShiftedAim=ITM_PHI;                   kbd_usr[5].primaryTam=ITM_alpha;                  
    kbd_usr[6].primary=ITM_STO;                       kbd_usr[6].fShifted=ITM_ASSIGN;                   kbd_usr[6].gShifted=ITM_SAVE;                     kbd_usr[6].keyLblAim=ITM_NULL;                    kbd_usr[6].primaryAim=ITM_G;                      kbd_usr[6].fShiftedAim=ITM_ASSIGN;                kbd_usr[6].gShiftedAim=ITM_GAMMA;                 kbd_usr[6].primaryTam=ITM_NULL;                   
    kbd_usr[7].primary=ITM_RCL;                       kbd_usr[7].fShifted=ITM_RBR;                      kbd_usr[7].gShifted=ITM_VIEW;                     kbd_usr[7].keyLblAim=ITM_NULL;                    kbd_usr[7].primaryAim=ITM_H;                      kbd_usr[7].fShiftedAim=ITM_RBR;                   kbd_usr[7].gShiftedAim=ITM_CHI;                   kbd_usr[7].primaryTam=ITM_HEX;                    
    kbd_usr[8].primary=ITM_Rdown;                     kbd_usr[8].fShifted=ITM_Rup;                      kbd_usr[8].gShifted=-MNU_CPX;                     kbd_usr[8].keyLblAim=ITM_NULL;                    kbd_usr[8].primaryAim=ITM_I;                      kbd_usr[8].fShiftedAim=ITM_DOWN_ARROW;            kbd_usr[8].gShiftedAim=ITM_IOTA;                  kbd_usr[8].primaryTam=ITM_REG_I;                  
    kbd_usr[9].primary=ITM_CC;                        kbd_usr[9].fShifted=ITM_MAGNITUDE;                kbd_usr[9].gShifted=ITM_ANGLE;                    kbd_usr[9].keyLblAim=ITM_NULL;                    kbd_usr[9].primaryAim=ITM_J;                      kbd_usr[9].fShiftedAim=ITM_NULL;                  kbd_usr[9].gShiftedAim=ITM_ETA;                   kbd_usr[9].primaryTam=ITM_REG_J;                  
    kbd_usr[10].primary=ITM_SHIFTf;                   kbd_usr[10].fShifted=ITM_NULL;                    kbd_usr[10].gShifted=ITM_NULL;                    kbd_usr[10].keyLblAim=ITM_SHIFTf;                 kbd_usr[10].primaryAim=ITM_SHIFTf;                kbd_usr[10].fShiftedAim=ITM_NULL;                 kbd_usr[10].gShiftedAim=ITM_SNAP;                 kbd_usr[10].primaryTam=ITM_SHIFTf;                
    kbd_usr[11].primary=ITM_SHIFTg;                   kbd_usr[11].fShifted=ITM_NULL;                    kbd_usr[11].gShifted=ITM_NULL;                    kbd_usr[11].keyLblAim=ITM_SHIFTg;                 kbd_usr[11].primaryAim=ITM_SHIFTg;                kbd_usr[11].fShiftedAim=ITM_USERMODE;             kbd_usr[11].gShiftedAim=ITM_NULL;                 kbd_usr[11].primaryTam=ITM_SHIFTg;                
    kbd_usr[12].primary=ITM_ENTER;                    kbd_usr[12].fShifted=ITM_STATUS;                  kbd_usr[12].gShifted=ITM_DROP;                    kbd_usr[12].keyLblAim=ITM_ENTER;                  kbd_usr[12].primaryAim=ITM_ENTER;                 kbd_usr[12].fShiftedAim=ITM_STATUS;               kbd_usr[12].gShiftedAim=ITM_DROP;                 kbd_usr[12].primaryTam=ITM_ENTER;                 
    kbd_usr[13].primary=ITM_XexY;                     kbd_usr[13].fShifted=ITM_FILL;                    kbd_usr[13].gShifted=-MNU_STK;                    kbd_usr[13].keyLblAim=ITM_ex;                     kbd_usr[13].primaryAim=ITM_K;                     kbd_usr[13].fShiftedAim=ITM_ex;                   kbd_usr[13].gShiftedAim=ITM_KAPPA;                kbd_usr[13].primaryTam=ITM_REG_K;                 
    kbd_usr[14].primary=ITM_CHS;                      kbd_usr[14].fShifted=ITM_DELTAPC;                 kbd_usr[14].gShifted=-MNU_FIN;                    kbd_usr[14].keyLblAim=ITM_PLUS_MINUS;             kbd_usr[14].primaryAim=ITM_L;                     kbd_usr[14].fShiftedAim=ITM_PLUS_MINUS;           kbd_usr[14].gShiftedAim=ITM_LAMBDA;               kbd_usr[14].primaryTam=ITM_REG_L;                 
    kbd_usr[15].primary=ITM_EXPONENT;                 kbd_usr[15].fShifted=ITM_SHOW;                    kbd_usr[15].gShifted=-MNU_DSP;                    kbd_usr[15].keyLblAim=ITM_NULL;                   kbd_usr[15].primaryAim=ITM_M;                     kbd_usr[15].fShiftedAim=ITM_UP_ARROW;             kbd_usr[15].gShiftedAim=ITM_MU;                   kbd_usr[15].primaryTam=ITM_NULL;                  
    kbd_usr[16].primary=ITM_BACKSPACE;                kbd_usr[16].fShifted=ITM_UNDO;                    kbd_usr[16].gShifted=-MNU_CLR;                    kbd_usr[16].keyLblAim=ITM_BACKSPACE;              kbd_usr[16].primaryAim=ITM_BACKSPACE;             kbd_usr[16].fShiftedAim=ITM_UNDO;                 kbd_usr[16].gShiftedAim=-MNU_CLR;                 kbd_usr[16].primaryTam=ITM_BACKSPACE;             
    kbd_usr[17].primary=ITM_DIV;                      kbd_usr[17].fShifted=ITM_RMD;                     kbd_usr[17].gShifted=ITM_MOD;                     kbd_usr[17].keyLblAim=ITM_SLASH;                  kbd_usr[17].primaryAim=ITM_N;                     kbd_usr[17].fShiftedAim=ITM_SLASH;                kbd_usr[17].gShiftedAim=ITM_NU;                   kbd_usr[17].primaryTam=ITM_DIV;                   
    kbd_usr[18].primary=ITM_7;                        kbd_usr[18].fShifted=-MNU_CONST;                  kbd_usr[18].gShifted=-MNU_ASN;                    kbd_usr[18].keyLblAim=ITM_7;                      kbd_usr[18].primaryAim=ITM_O;                     kbd_usr[18].fShiftedAim=ITM_7;                    kbd_usr[18].gShiftedAim=ITM_OMEGA;                kbd_usr[18].primaryTam=ITM_7;                     
    kbd_usr[19].primary=ITM_8;                        kbd_usr[19].fShifted=ITM_USERMODE;                kbd_usr[19].gShifted=ITM_SNAP;                    kbd_usr[19].keyLblAim=ITM_8;                      kbd_usr[19].primaryAim=ITM_P;                     kbd_usr[19].fShiftedAim=ITM_8;                    kbd_usr[19].gShiftedAim=ITM_PI;                   kbd_usr[19].primaryTam=ITM_8;                     
    kbd_usr[20].primary=ITM_9;                        kbd_usr[20].fShifted=-MNU_HOME;                   kbd_usr[20].gShifted=ITM_RTN;                     kbd_usr[20].keyLblAim=ITM_9;                      kbd_usr[20].primaryAim=ITM_Q;                     kbd_usr[20].fShiftedAim=ITM_9;                    kbd_usr[20].gShiftedAim=ITM_RTN;                  kbd_usr[20].primaryTam=ITM_9;                     
    kbd_usr[21].primary=ITM_XEQ;                      kbd_usr[21].fShifted=ITM_GTO;                     kbd_usr[21].gShifted=ITM_LBL;                     kbd_usr[21].keyLblAim=ITM_NULL;                   kbd_usr[21].primaryAim=ITM_NULL;                  kbd_usr[21].fShiftedAim=ITM_NULL;                 kbd_usr[21].gShiftedAim=ITM_NULL;                 kbd_usr[21].primaryTam=ITM_NULL;                  
    kbd_usr[22].primary=ITM_MULT;                     kbd_usr[22].fShifted=ITM_XFACT;                   kbd_usr[22].gShifted=-MNU_PROB;                   kbd_usr[22].keyLblAim=ITM_CROSS;                  kbd_usr[22].primaryAim=ITM_R;                     kbd_usr[22].fShiftedAim=ITM_PROD_SIGN;            kbd_usr[22].gShiftedAim=ITM_RHO;                  kbd_usr[22].primaryTam=ITM_MULT;                  
    kbd_usr[23].primary=ITM_4;                        kbd_usr[23].fShifted=-MNU_STAT;                   kbd_usr[23].gShifted=-MNU_SUMS;                   kbd_usr[23].keyLblAim=ITM_4;                      kbd_usr[23].primaryAim=ITM_S;                     kbd_usr[23].fShiftedAim=ITM_4;                    kbd_usr[23].gShiftedAim=ITM_SIGMA;                kbd_usr[23].primaryTam=ITM_4;                     
    kbd_usr[24].primary=ITM_5;                        kbd_usr[24].fShifted=ITM_toREC;                   kbd_usr[24].gShifted=ITM_toPOL;                   kbd_usr[24].keyLblAim=ITM_5;                      kbd_usr[24].primaryAim=ITM_T;                     kbd_usr[24].fShiftedAim=ITM_5;                    kbd_usr[24].gShiftedAim=ITM_TAU;                  kbd_usr[24].primaryTam=ITM_5;                     
    kbd_usr[25].primary=ITM_6;                        kbd_usr[25].fShifted=-MNU_UNITCONV;               kbd_usr[25].gShifted=-MNU_ANGLECONV;              kbd_usr[25].keyLblAim=ITM_6;                      kbd_usr[25].primaryAim=ITM_U;                     kbd_usr[25].fShiftedAim=ITM_6;                    kbd_usr[25].gShiftedAim=ITM_THETA;                kbd_usr[25].primaryTam=ITM_6;                     
    kbd_usr[26].primary=ITM_UP1;                      kbd_usr[26].fShifted=ITM_BST;                     kbd_usr[26].gShifted=-MNU_FLAGS;                  kbd_usr[26].keyLblAim=ITM_UP1;                    kbd_usr[26].primaryAim=CHR_caseUP;                kbd_usr[26].fShiftedAim=ITM_UP_ARROW;             kbd_usr[26].gShiftedAim=-MNU_FLAGS;               kbd_usr[26].primaryTam=ITM_UP1;                   
    kbd_usr[27].primary=ITM_SUB;                      kbd_usr[27].fShifted=-MNU_BITS;                   kbd_usr[27].gShifted=-MNU_INTS;                   kbd_usr[27].keyLblAim=ITM_MINUS;                  kbd_usr[27].primaryAim=ITM_V;                     kbd_usr[27].fShiftedAim=ITM_MINUS;                kbd_usr[27].gShiftedAim=-MNU_ALPHAMATH;           kbd_usr[27].primaryTam=ITM_SUB;                   
    kbd_usr[28].primary=ITM_1;                        kbd_usr[28].fShifted=-MNU_ADV;                    kbd_usr[28].gShifted=-MNU_EQN;                    kbd_usr[28].keyLblAim=ITM_1;                      kbd_usr[28].primaryAim=ITM_W;                     kbd_usr[28].fShiftedAim=ITM_1;                    kbd_usr[28].gShiftedAim=ITM_PSI;                  kbd_usr[28].primaryTam=ITM_1;                     
    kbd_usr[29].primary=ITM_2;                        kbd_usr[29].fShifted=-MNU_MATX;                   kbd_usr[29].gShifted=-MNU_XFN;                    kbd_usr[29].keyLblAim=ITM_2;                      kbd_usr[29].primaryAim=ITM_X;                     kbd_usr[29].fShiftedAim=ITM_2;                    kbd_usr[29].gShiftedAim=ITM_XI;                   kbd_usr[29].primaryTam=ITM_2;                     
    kbd_usr[30].primary=ITM_3;                        kbd_usr[30].fShifted=ITM_TIMER;                   kbd_usr[30].gShifted=-MNU_CLK;                    kbd_usr[30].keyLblAim=ITM_3;                      kbd_usr[30].primaryAim=ITM_Y;                     kbd_usr[30].fShiftedAim=ITM_3;                    kbd_usr[30].gShiftedAim=ITM_UPSILON;              kbd_usr[30].primaryTam=ITM_3;                     
    kbd_usr[31].primary=ITM_DOWN1;                    kbd_usr[31].fShifted=ITM_SST;                     kbd_usr[31].gShifted=-MNU_MODE;                   kbd_usr[31].keyLblAim=ITM_DOWN1;                  kbd_usr[31].primaryAim=CHR_caseDN;                kbd_usr[31].fShiftedAim=ITM_DOWN_ARROW;           kbd_usr[31].gShiftedAim=-MNU_MODE;                kbd_usr[31].primaryTam=ITM_DOWN1;                 
    kbd_usr[32].primary=ITM_ADD;                      kbd_usr[32].fShifted=-MNU_IO;                     kbd_usr[32].gShifted=-MNU_PRINT;                  kbd_usr[32].keyLblAim=ITM_PLUS;                   kbd_usr[32].primaryAim=ITM_Z;                     kbd_usr[32].fShiftedAim=ITM_PLUS;                 kbd_usr[32].gShiftedAim=ITM_ZETA;                 kbd_usr[32].primaryTam=ITM_ADD;                   
    kbd_usr[33].primary=ITM_0;                        kbd_usr[33].fShifted=-MNU_LOOP;                   kbd_usr[33].gShifted=-MNU_TEST;                   kbd_usr[33].keyLblAim=ITM_0;                      kbd_usr[33].primaryAim=ITM_NULL;                  kbd_usr[33].fShiftedAim=ITM_0;                    kbd_usr[33].gShiftedAim=ITM_PRINTER;              kbd_usr[33].primaryTam=ITM_0;                     
    kbd_usr[34].primary=ITM_PERIOD;                   kbd_usr[34].fShifted=-MNU_PARTS;                  kbd_usr[34].gShifted=-MNU_INFO;                   kbd_usr[34].keyLblAim=ITM_PERIOD;                 kbd_usr[34].primaryAim=ITM_COMMA;                 kbd_usr[34].fShiftedAim=ITM_PERIOD;               kbd_usr[34].gShiftedAim=-MNU_ALPHADOT;            kbd_usr[34].primaryTam=ITM_PERIOD;                
    kbd_usr[35].primary=ITM_RS;                       kbd_usr[35].fShifted=ITM_PR;                      kbd_usr[35].gShifted=-MNU_PFN;                    kbd_usr[35].keyLblAim=ITM_NULL;                   kbd_usr[35].primaryAim=ITM_SPACE;                 kbd_usr[35].fShiftedAim=ITM_NULL;                 kbd_usr[35].gShiftedAim=ITM_NULL;                 kbd_usr[35].primaryTam=ITM_NULL;                  
    kbd_usr[36].primary=ITM_EXIT1;                    kbd_usr[36].fShifted=-MNU_CATALOG;                kbd_usr[36].gShifted=ITM_OFF;                     kbd_usr[36].keyLblAim=ITM_EXIT1;                  kbd_usr[36].primaryAim=ITM_EXIT1;                 kbd_usr[36].fShiftedAim=-MNU_CATALOG;             kbd_usr[36].gShiftedAim=ITM_OFF;                  kbd_usr[36].primaryTam=ITM_EXIT1;                 
    fnSetFlag(FLAG_USER);
    break;
    
    

  case USER_SHIFTS:                                             //USER_SHIFTS 25          //JM Sectioon to be put on a menu
  
    fnUserJM(USER_C43);
  
    kbd_usr[0].primary     = ITM_USERMODE;
    kbd_usr[9].primary     = -MNU_TRI;
    kbd_usr[9].fShifted    = ITM_USERMODE;
       //kbd_usr[9].gShifted    = ITM_RTN;
       //kbd_usr[9].fShifted    = ITM_NULL;
    kbd_usr[10].primary    = ITM_SHIFTf;
    kbd_usr[10].fShifted   = ITM_NULL;
    kbd_usr[10].gShifted   = ITM_NULL;
    kbd_usr[11].primary    = ITM_SHIFTg;
    kbd_usr[11].fShifted   = ITM_NULL;
    kbd_usr[11].gShifted   = ITM_NULL;
    Norm_Key_00_VAR        = ITM_USERMODE;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;

  //LEFT SHIFTS Not used, and disabled from KEYS menu
  case USER_SHIFTS2:                                             //USER_SHIFTS 25          //JM Sectioon to be put on a menu
    kbd_usr[0].primary     = ITM_USERMODE;
    kbd_usr[9].primary     = -MNU_TRI;
    kbd_usr[9].fShifted    = ITM_USERMODE;

    kbd_usr[10].primary    = ITM_STO;
    kbd_usr[10].fShifted   = ITM_MAGNITUDE;
    kbd_usr[10].gShifted   = ITM_ANGLE;
    kbd_usr[11].primary    = ITM_RCL;
    kbd_usr[11].fShifted   = ITM_PC;
    kbd_usr[11].gShifted   = ITM_DELTAPC;

    kbd_usr[6].primary    = ITM_SHIFTf;
    kbd_usr[6].fShifted   = ITM_NULL;
    kbd_usr[6].gShifted   = ITM_NULL;
    kbd_usr[7].primary    = ITM_SHIFTg;
    kbd_usr[7].fShifted   = ITM_NULL;
    kbd_usr[7].gShifted   = ITM_NULL;
    Norm_Key_00_VAR        = ITM_USERMODE;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;



  case USER_SIGMAPLUS:                                              //USER_DEFAULTS FOR USER: E+ E+
    kbd_usr[0].primary     = ITM_SIGMAPLUS;
    Norm_Key_00_VAR        = ITM_SIGMAPLUS;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;

  case USER_PRGM:                                              //USER_DEFAULTS FOR USER: E+ PRGM
    kbd_usr[0].primary     = ITM_PR;
    Norm_Key_00_VAR        = ITM_PR;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;

  case USER_USER:                                              //USER_DEFAULTS FOR USER: E+ USER
    kbd_usr[0].primary     = ITM_USERMODE;
    Norm_Key_00_VAR        = ITM_USERMODE;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;

  case USER_HOME:                                              //USER_DEFAULTS FOR USER: E+ USER
    kbd_usr[0].primary     = -MNU_HOME;
    Norm_Key_00_VAR        = -MNU_HOME;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;

  case USER_ALPHA:                                              //USER_DEFAULTS FOR USER: E+ ALPHA
    kbd_usr[0].primary     = ITM_AIM;
    Norm_Key_00_VAR        = ITM_AIM;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;

  case USER_MYM:                                              //USER_DEFAULTS FOR USER: E+ MYM
    kbd_usr[0].primary     = -MNU_MyMenu;
    Norm_Key_00_VAR        = -MNU_MyMenu;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;

  case USER_GSHFT:                                           //USER_DEFAULTS FOR USER: E+ SHIFT G
    kbd_usr[0].primary     = ITM_SHIFTg;
    kbd_usr[0].gShifted    = ITM_NULL;
    kbd_usr[0].fShifted    = ITM_NULL;
    Norm_Key_00_VAR        = ITM_SHIFTg;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnSetFlag(FLAG_USER);
    break;

  case USER_RESET:                                              //USER_RESET 26
    xcopy(kbd_usr, kbd_std, sizeof(kbd_std));
    Norm_Key_00_VAR        = ITM_SIGMAPLUS;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    fnClearFlag(FLAG_USER); //userModeEnabled = false;
    break;

/*
  case JM_ASSIGN:                 //A non 0 and non 32766 value means the FN NUMBER is in JM_ASSIGN, AND KEYBOARD.C will wait for a key to be assigned to                                     //USER_RESET 27
    JM_ASN_MODE = ITM_CC;         //TEMPORARY TEST FUNCTION
    break;
*/

  case JM_SEEK_FN:      //32766 in KEYBOARD.C will wait for a key. SEEK FUNCTION,         //USER_RESET 27
    JM_ASN_MODE = 32766;
    #ifndef TESTSUITE_BUILD
      clearScreen_old(false,true,false);
      showString("Select (EXIT Aborts)",                   &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_T - REGISTER_X), vmNormal, true, true);
      showString("  Key1: function from keys",             &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Z - REGISTER_X), vmNormal, true, true);
      showString("  Key2: Assigned key (E+ to E and ",     &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Y - REGISTER_X), vmNormal, true, true);
      showString("  [/] [*] [-] [+] [R/S]) (EXIT aborts)", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_X - REGISTER_X), vmNormal, true, true);
    #endif
    previousCalcMode = calcMode;
    calcMode = CM_BUG_ON_SCREEN;
    break;

  default:
    break;
  }
}




void fnKEYSELECT(void) {                                        //JM ASSIGN - REMEMBER NEXT KEYBOARD FUNCTION
  if(JM_ASN_MODE == ITM_EXIT1 || JM_ASN_MODE == ITM_BACKSPACE) {
    JM_ASN_MODE = 0;
    #ifndef TESTSUITE_BUILD
        clearScreen_old(false,true,false);
        showString("Abandoned or illegal function", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Y - REGISTER_X), vmNormal, true, true);
    #endif
  }
  else {
    #ifndef TESTSUITE_BUILD
        //clearScreen_old(false,true,false);
        //showString("Select key: top 4 lines excl. FN1-6 & [<-],", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Y - REGISTER_X), vmNormal, true, true);
        //showString("incl. [/] [*] [-] [+] [R/S].   EXIT aborts.", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_X - REGISTER_X), vmNormal, true, true);
    #endif
    calcMode = CM_NORMAL;

    fnFlipFlag(FLAG_USER);
    fnFlipFlag(FLAG_USER);
//    userModeEnabled = true;                                     //JM Get out ouf USER MODE to select key in next step
  //  toggleUserMode();
  }
}



//JM Check if JM ASSIGN IS IN PROGRESS AND CAPTURE THE FUNCTION AND KEY TO BE ASSIGNED
//gets here only after valid function and any key is selected
void fnASSIGN(int16_t JM_ASN_MODE, int16_t tempkey) {           //JM ASSIGN - REMEMBER NEXT KEYBOARD FUNCTION
  switch(tempkey) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 21:
    case 26:
    case 31:
    case 35:
    case 36:

      //JM_convertIntegerToShortIntegerRegister(tempkey, 10, REGISTER_X);
      //JM_convertIntegerToShortIntegerRegister(JM_ASN_MODE, 10, REGISTER_X);
      if(shiftF) {
        (kbd_usr + tempkey)->fShifted = JM_ASN_MODE;  //Assign function into keyboard array
      }
      else if(shiftG) {
        (kbd_usr + tempkey)->gShifted = JM_ASN_MODE;  //Assign function into keyboard array
      }
      else {
        (kbd_usr + tempkey)->primary = JM_ASN_MODE;   //Assign function into keyboard array
      }

      fnSetFlag(FLAG_USER);
      break;
    default:
#ifndef TESTSUITE_BUILD
//        clearScreen(false,true,false);
        showString("Invalid key", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Z - REGISTER_X), vmNormal, true, true);
#endif
    break;
  }
}





