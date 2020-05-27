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


/*
void fnAngularMode(uint16_t am) {
  currentAngularMode = am;

  showAngularMode();
  refreshStack();
}

void fnComplexUnit(uint16_t cu) {
  complexUnit = cu;
  refreshStack();
}

void fnComplexResult(uint16_t complexResult) {
  complexResult ? fnSetFlag(FLAG_CPXRES) : fnClearFlag(FLAG_CPXRES);
}

void fnComplexMode(uint16_t cm) {
  complexMode = cm;
  showComplexMode();
  refreshStack();
}
*/


/********************************************//**
 * SEEMS TO BE WORKING. CALLED FROM ## in BASE
 *
 * FROM keyboard.c
 ***********************************************/
void fnBASE_Hash(uint16_t unusedParamButMandatory) {  //DONE
  int16_t lastChar;
  lastChar = strlen(nimBuffer) - 1;
  if(lastChar >= 1) {
    calcMode = CM_NIM;
#ifndef TESTSUITE_BUILD
    addItemToNimBuffer(ITM_toINT);
  }
  else {
    runFunction(ITM_toINT);
#endif
  }
}


/* NOTNEEDED
void fnKeyDotDjm(uint16_t unusedParamButMandatory) {      //FOR   HARDWIRED 
  userModeEnabledMEM = userModeEnabled;
  userModeEnabled = false;
  shiftF = false;                  //JM
  shiftG = true;                 //JM
#ifdef PC_BUILD
  btnClicked(NULL, "02");         //JM changed from 02
#endif
#ifdef DMCP_BUILD
  btnClicked(NULL, "02");         //JM changed from 02
#endif
  userModeEnabled = userModeEnabledMEM;
}
#endif
*/



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

  case JC_FG_DOTS:                                          //JM
    jm_FG_DOTS = !jm_FG_DOTS;
    fnRefreshComboxState(CB_JC, JC_FG_DOTS, jm_FG_DOTS);                        //jm
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
    fnRefreshComboxState(CB_JC, JC_BASE_AHOME, SH_BASE_AHOME);                  //dr
    break;

  case JC_SH_3T:                                            //JM SH.3T
    Home3TimerMode = !Home3TimerMode;
    //fnInfo(Home3TimerMode);
    fnRefreshComboxState(CB_JC, JC_SH_3T, Home3TimerMode);                      //dr
    break;

  case JC_VECT:                                      //JM
    jm_VECT = !jm_VECT;
    fnRefreshComboxState(CB_JC, JC_VECT, jm_VECT);                //jm
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






   case RM_COMMA:               //DONR
     fnClearFlag(FLAG_DECIMP);
     break;

   case RM_PERIOD:               //DONR
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

  case JC_BLZ:                                              //JM bit LeadingZeros
    fnFlipFlag(FLAG_LEAD0);                                       //dr
    break;

   case PS_CROSS:               //DONR
     fnSetFlag(FLAG_MULTx);
     break;

   case PS_DOT:
     fnClearFlag(FLAG_MULTx);
     break;

   case SS_8:          //DONE
     fnSetFlag(FLAG_SSIZE8);
     break;

   case SS_4:                 //DONE
     fnClearFlag(FLAG_SSIZE8);
     break;

   case CM_RECTANGULAR:          //DONE
     fnSetFlag(FLAG_RECTN);
     break;

   case CM_POLAR:                 //DONE
     fnClearFlag(FLAG_RECTN);
     break;

   case JC_BCR:                                              //JM bit ComplexResult
     fnFlipFlag(FLAG_CPXRES);                                     //dr
     break;




  default:
    break;
  }
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




void Show_User_Keys(void) {                           //DONE
  fnSetFlag(FLAG_USER);

//  userModeEnabled = false;
  //toggleUserMode();
}


void Show_Normal_Keys(void) {                         //DONE
  fnClearFlag(FLAG_USER);
//  userModeEnabled = true;
  //toggleUserMode();
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
  Show_Normal_Keys();
}



/********************************************//**
 * \brief Displays TRUE/FALSE information
 *
 * \param[in] f bool_t
 * \return void
 ***********************************************/
//void fnInfo(bool_t f) {
//  temporaryInformation = f ? TI_TRUE : TI_FALSE;
//  refreshRegisterLine(TAM_REGISTER_LINE);
//  refreshRegisterLine(REGISTER_X);
//}



/********************************************//**
 * \brief Show flag value
 * \param[in] jmConfig to display uint16_t
 * \return void
 ***********************************************/
void fnShowJM(uint16_t jmConfig) {                               //DONE
  longInteger_t mem;
  longIntegerInit(mem);
  liftStack();

  switch(jmConfig) {
  case JC_ERPN:
    if(eRPN) { stringToLongInteger("1",10,mem); }
    else { stringToLongInteger("0",10,mem); }
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

  refreshStack();
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
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnGetSigmaAssignToX(uint16_t unusedParamButMandatory) {       //DONE
  char snum[10];
  longInteger_t mem;
  longIntegerInit(mem);
  liftStack();

  itoa(Norm_Key_00_VAR, snum, 10);
  stringToLongInteger(snum,10,mem);

  convertLongIntegerToLongIntegerRegister(mem, REGISTER_X);
  longIntegerFree(mem);

  refreshStack();
}



//JM CONFIGURE USER MODE - ASSIGN KEYS

/********************************************//**
 * \brief
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnJM_GetXToNORMmode(uint16_t unusedParamButMandatory) {      //DONE
  int16_t X_REG;
  longInteger_t lgInt;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);
    longIntegerToAllocatedString(lgInt, tmpStr3000, TMP_STR_LENGTH);
    longIntegerToInt(lgInt,X_REG);
    longIntegerFree(lgInt);
    //printf("Xreg %d\n", X_REG);
    Norm_Key_00_VAR = X_REG;
    Show_Normal_Keys();
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
    longIntegerToAllocatedString(lgInt, tmpStr3000, TMP_STR_LENGTH);
    longIntegerToInt(lgInt,X_REG);
    longIntegerFree(lgInt);
  //printf("Xreg %d\n", X_REG);
    if(JM_KEY >= 256) {
      kbd_usr[JM_KEY - 256].primary = X_REG;
    //printf(".primary %d\n", kbd_usr[JM_KEY - 256].primary);
      Show_User_Keys();
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
    longIntegerToAllocatedString(lgInt, tmpStr3000, TMP_STR_LENGTH);
    longIntegerToInt(lgInt,X_REG);
    longIntegerFree(lgInt);
  //printf("Xreg %d\n", X_REG);
    if(JM_KEY >= 256) {
      kbd_usr[JM_KEY - 256].fShifted = X_REG;
    //printf(".fShifted %d\n", kbd_usr[JM_KEY - 256].fShifted);
      Show_User_Keys();
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
    longIntegerToAllocatedString(lgInt, tmpStr3000, TMP_STR_LENGTH);
    longIntegerToInt(lgInt,X_REG);
    longIntegerFree(lgInt);
  //printf("Xreg %d\n", X_REG);
    if(JM_KEY >= 256) {
      kbd_usr[JM_KEY - 256].gShifted = X_REG;
      //printf(".gShifted %d\n", kbd_usr[JM_KEY - 256].gShifted);
      Show_User_Keys();
    }
  }
}



//---------------------------------------------


void fnStrtoX(char aimBuffer[]) {      //DONE
  STACK_LIFT_ENABLE;   // 5
  liftStack();
  int16_t mem = stringByteLength(aimBuffer);
  reallocateRegister(REGISTER_X, dtString, mem, AM_NONE);
  xcopy(REGISTER_STRING_DATA(REGISTER_X), aimBuffer, mem + 1);
}



void fnStrInputReal34(char inp1[]) {  // CONVERT STRING to REAL IN X      //DONE
  tmpStr3000[0] = 0;
  strcat(tmpStr3000, inp1);
  STACK_LIFT_ENABLE;   // 5
  liftStack();
  reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
  stringToReal34(tmpStr3000, REGISTER_REAL34_DATA(REGISTER_X));
}



void fnStrInputLongint(char inp1[]) {  // CONVERT STRING to Longint X      //DONE
  tmpStr3000[0]=0;
  strcat(tmpStr3000, inp1);
  STACK_LIFT_ENABLE;   // 5
  liftStack();

  longInteger_t lgInt;
  longIntegerInit(lgInt);
  stringToLongInteger(tmpStr3000 + (tmpStr3000[0] == '+' ? 1 : 0), 10, lgInt);
  convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
  longIntegerFree(lgInt);
}




void fnRCL(int16_t inp) {      //DONE
  STACK_LIFT_ENABLE;
  if(inp == TEMP_REGISTER) {
    liftStack();
    copySourceRegisterToDestRegister(inp, REGISTER_X);
    refreshStack();
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
#define JMTEMP TEMP_REGISTER

  if(JM_OPCODE == 6) {                                          //Delta to Star   ZYX to ZYX; destroys IJKL & JMTEMP
    saveStack();
    STACK_LIFT_ENABLE;
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_I);   // STO I
    copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_J);   // STO J
    copySourceRegisterToDestRegister(REGISTER_Z, REGISTER_K);   // STO K
    fnAdd(0);                                                   // +
    fnSwapXY(0);                                                // X<>Y

    fnAdd(0);                                                   // +
    copySourceRegisterToDestRegister(REGISTER_X, JMTEMP);       // STO JMTEMP
    fnRCL(REGISTER_K);                                          // RCL I
    fnRCL(REGISTER_J);                                          // RCL J     // z = (zx yz) / (x+y+z)
    fnMultiply(0);                                              // *
    fnSwapXY(0);                                                // X<>Y
    fnDivide(0);                                                // /

    fnRCL(JMTEMP);                                              // RCL JMTEMP
    fnRCL(REGISTER_I);                                          // RCL J
    fnRCL(REGISTER_J);                                          // RCL K     // y = (xy yz) / (x+y+z)
    fnMultiply(0);                                              // *
    fnSwapXY(0);                                                // X<>Y
    fnDivide(0);                                                // /

    fnRCL(JMTEMP);                                              // RCL JMTEMP
    fnRCL(REGISTER_I);                                          // RCL I
    fnRCL(REGISTER_K);                                          // RCL K     // z = (xy zx) / (x+y+z)
    fnMultiply(0);                                              // *
    fnSwapXY(0);                                                // X<>Y
    fnDivide(0);                                                // /

    copySourceRegisterToDestRegister(REGISTER_I, REGISTER_L);   // STO

    temporaryInformation = TI_ABC;

    adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
    adjustResult(REGISTER_Y, false, true, REGISTER_Y, -1, -1);
    adjustResult(REGISTER_Z, false, true, REGISTER_Z, -1, -1);
//    refreshRegisterLine(REGISTER_X);
//    refreshRegisterLine(REGISTER_Y);
//    refreshRegisterLine(REGISTER_Z);
  }
  else

  if(JM_OPCODE == 7) {                                          //Star to Delta ZYX to ZYX; destroys IJKL & JMTEMP
    saveStack();
    STACK_LIFT_ENABLE;
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_I);   // STO I
    copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_J);   // STO J
    copySourceRegisterToDestRegister(REGISTER_Z, REGISTER_K);   // STO K

    fnMultiply(0);                          //IJ                // *
    fnSwapXY(0);
    fnRCL(REGISTER_I);                                          // RCL J
    fnMultiply(0);                          //IK                // *
    fnAdd(0);
    fnRCL(REGISTER_J);                                          // RCL J
    fnRCL(REGISTER_K);                                          // RCL K
    fnMultiply(0);                          //JK                // *
    fnAdd(0);
    copySourceRegisterToDestRegister(REGISTER_X, JMTEMP);       // STO JMTEMP
                                                                //
    fnRCL(REGISTER_J);                                          //      zx = () / y
    fnDivide(0);                                                // 

    fnRCL(JMTEMP);                                              // RCL JMTEMP    
    fnRCL(REGISTER_I);                                          //      yz = () / x
    fnDivide(0);                                                //

    fnRCL(JMTEMP);                                              // RCL JMTEMP    
    fnRCL(REGISTER_K);                                          //      xy = () / z
    fnDivide(0);                                                //

    copySourceRegisterToDestRegister(REGISTER_I, REGISTER_L);   // STO

    temporaryInformation = TI_ABBCCA;
    adjustResult(REGISTER_X, false, true, REGISTER_X, -1, -1);
    adjustResult(REGISTER_Y, false, true, REGISTER_Y, -1, -1);
    adjustResult(REGISTER_Z, false, true, REGISTER_Z, -1, -1);
//    refreshRegisterLine(REGISTER_X);
//    refreshRegisterLine(REGISTER_Y);
//    refreshRegisterLine(REGISTER_Z);
  }
  else

  if(JM_OPCODE == 8) {                                          //SYMMETRICAL COMP to ABC   ZYX to ZYX; destroys IJKL
    saveStack();
    STACK_LIFT_ENABLE;
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_I);   // STO I  //A2
    copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_J);   // STO J  //A1
    copySourceRegisterToDestRegister(REGISTER_Z, REGISTER_K);   // STO K  //A0
    fnAdd(0);                                                   // +
    fnAdd(0);                                                   // + Va = Vao + Va1 +Va2

    STACK_LIFT_ENABLE;
    liftStack();
    fn_cnst_op_a(0);
    fnRCL(REGISTER_I);                                       // A2
    fnMultiply(0);                                              // * a
    STACK_LIFT_ENABLE;
    liftStack();
    fn_cnst_op_aa(0);
    fnRCL(REGISTER_J);                                       // A1
    fnMultiply(0);                                              // * aa
    fnAdd(0);                                                   // +
    fnRCL(REGISTER_K);                                       // A0
    fnAdd(0);                                                   // + Vb = Vao + aaVa1 +aVa2

    STACK_LIFT_ENABLE;
    liftStack();
    fn_cnst_op_aa(0);
    fnRCL(REGISTER_I);                                       // A2
    fnMultiply(0);                                              // * a
    STACK_LIFT_ENABLE;
    liftStack();
    fn_cnst_op_a(0);
    fnRCL(REGISTER_J);                                       // A1
    fnMultiply(0);                                              // * aa
    fnAdd(0);                                                   // +
    fnRCL(REGISTER_K);                                       // A0
    fnAdd(0);                                                   // + Vb = Vao + aaVa1 +aVa2

    copySourceRegisterToDestRegister(REGISTER_I, REGISTER_L);   // STO

    temporaryInformation = TI_ABC;
    refreshRegisterLine(REGISTER_X);
    refreshRegisterLine(REGISTER_Y);
    refreshRegisterLine(REGISTER_Z);
  }
  else

  if(JM_OPCODE == 9) {                                          //ABC to SYMMETRICAL COMP   ZYX to ZYX; destroys IJKL & JMTEMP
    saveStack();
    STACK_LIFT_ENABLE;
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_I);  // STO I  //c
    copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_J);  // STO J  //b
    copySourceRegisterToDestRegister(REGISTER_Z, REGISTER_K);  // STO K  //a
    fnAdd(0);                                                  // +
    fnAdd(0);                                                  // + Va0 = (Va + Vb +Vc)/3
    liftStack();
    reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
    stringToReal34("3", REGISTER_REAL34_DATA(REGISTER_X));
    stringToReal34("0", REGISTER_IMAG34_DATA(REGISTER_X));      //
    refreshStack();
    copySourceRegisterToDestRegister(REGISTER_X, JMTEMP);       // STO
    fnDivide(0);


    STACK_LIFT_ENABLE;
    liftStack();
    fn_cnst_op_a(0);
    fnRCL(REGISTER_J);                                       // VB
    fnMultiply(0);                                              // * a
    STACK_LIFT_ENABLE;
    liftStack();
    fn_cnst_op_aa(0);
    fnRCL(REGISTER_I);                                       // VC
    fnMultiply(0);                                              // * aa
    fnAdd(0);                                                   // +
    fnRCL(REGISTER_K);                                       // VA
    fnAdd(0);                                                   // + V1 = (VA +aVb +aaVc) /3
    fnRCL(JMTEMP);                                              // 3
    fnDivide(0);                                                // /


    STACK_LIFT_ENABLE;
    liftStack();
    fn_cnst_op_aa(0);
    fnRCL(REGISTER_J);                                       // VB
    fnMultiply(0);                                              // * a
    STACK_LIFT_ENABLE;
    liftStack();
    fn_cnst_op_a(0);
    fnRCL(REGISTER_I);                                       // VC
    fnMultiply(0);                                              // * aa
    fnAdd(0);                                                   // +
    fnRCL(REGISTER_K);                                       // VA
    fnAdd(0);                                                   // + V1 = (VA +aVb +aaVc) /3
    fnRCL(JMTEMP);                                              // 3
    fnDivide(0);                                                // /

    copySourceRegisterToDestRegister(REGISTER_I, REGISTER_L);   // STO


    temporaryInformation = TI_012;
    refreshRegisterLine(REGISTER_X);
    refreshRegisterLine(REGISTER_Y);
    refreshRegisterLine(REGISTER_Z);
  }
  else

  if(JM_OPCODE == 10) {                                         //e^theta.j j
    saveStack();
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_L);   // STO TMP
    STACK_LIFT_ENABLE;
    liftStack();
    fn_cnst_op_j(0);
    fnMultiply(0);                                              // * aa
    fnExp(0);
    refreshStack();
  }
  else

  if(JM_OPCODE == 11) {                                         //STO Z
    saveStack();
    STACK_LIFT_ENABLE;                                          //  Registers: Z:90-92  V:93-95  I:96-98  XYZ
    copySourceRegisterToDestRegister(REGISTER_X, 90);
    copySourceRegisterToDestRegister(REGISTER_Y, 91);
    copySourceRegisterToDestRegister(REGISTER_Z, 92);
  }
  else

  if(JM_OPCODE == 13) {                                         //STO V
    saveStack();
    STACK_LIFT_ENABLE;                                          //  Registers: Z:90-92  V:93-95  I:96-98  XYZ
    copySourceRegisterToDestRegister(REGISTER_X, 93);
    copySourceRegisterToDestRegister(REGISTER_Y, 94);
    copySourceRegisterToDestRegister(REGISTER_Z, 95);
  }
  else

  if(JM_OPCODE == 15) {                                         //STO I
    saveStack();
    STACK_LIFT_ENABLE;                                          //  Registers: Z:90-92  V:93-95  I:96-98  XYZ
    copySourceRegisterToDestRegister(REGISTER_X, 96);
    copySourceRegisterToDestRegister(REGISTER_Y, 97);
    copySourceRegisterToDestRegister(REGISTER_Z, 98);
  }
  else

  if(JM_OPCODE == 12) {                                         //RCL Z
    saveStack();
    fnRCL(92);
    fnRCL(91);
    fnRCL(90);
  }
  else

  if(JM_OPCODE == 14) {                                         //RCL V
    saveStack();
    fnRCL(95);
    fnRCL(94);
    fnRCL(93);
  }
  else

  if(JM_OPCODE == 16) {                                         //RCL I
    saveStack();
    fnRCL(98);
    fnRCL(97);
    fnRCL(96);
  }
  else

  if(JM_OPCODE == 17) {                                         // V/I
    saveStack();
    fnRCL(95);
    fnRCL(98);
    fnDivide(0);
    fnRCL(94);
    fnRCL(97);
    fnDivide(0);
    fnRCL(93);
    fnRCL(96);
    fnDivide(0);
    refreshStack();
  }
  else

  if(JM_OPCODE == 18) {                                         // IZ
    saveStack();
    fnRCL(98);
    fnRCL(92);
    fnMultiply(0);
    fnRCL(97);
    fnRCL(91);
    fnMultiply(0);
    fnRCL(96);
    fnRCL(91);
    fnMultiply(0);
    refreshStack();
  }
  else

  if(JM_OPCODE == 19) {                                         // V/Z
    saveStack();
    fnRCL(95);
    fnRCL(92);
    fnDivide(0);
    fnRCL(94);
    fnRCL(91);
    fnDivide(0);
    fnRCL(93);
    fnRCL(90);
    fnDivide(0);
    refreshStack();
  }
  else

  if(JM_OPCODE == 20) {                                         //Copy Create X>ABC
    saveStack();
    STACK_LIFT_ENABLE;
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_I);

    fnRCL(REGISTER_I);                                          //
    STACK_LIFT_ENABLE;
    liftStack();
    fn_cnst_op_a(0);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_J);
    fnMultiply(0);

    fnRCL(REGISTER_I);                                          //
    STACK_LIFT_ENABLE;
    liftStack();
    fn_cnst_op_aa(0);
    copySourceRegisterToDestRegister(REGISTER_X, REGISTER_J);
    fnMultiply(0);

    refreshStack();
  }
  else

  if(JM_OPCODE == 21) {                                         //Graph
    saveStack();
    //Convert from X register to float
    real_t tmpy;
    fnToReal(0);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &tmpy);
    realToString(&tmpy, tmpStr3000);
    graph_xmin = strtof(tmpStr3000, NULL);
    //printf("%s %f\n",tmpStr3000,graph_xmin);
    fnDrop(0);
  }
  else
  if(JM_OPCODE == 22) {                                         //Graph
    saveStack();
    //Convert from X register to float
    real_t tmpy;
    fnToReal(0);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &tmpy);
    realToString(&tmpy, tmpStr3000);
    graph_xmax = strtof(tmpStr3000, NULL);
    //printf("%s %f\n",tmpStr3000,graph_xmax);
    fnDrop(0);
  }
  else
  if(JM_OPCODE == 23) {                                         //Graph
    saveStack();
    //Convert from X register to float
    real_t tmpy;
    fnToReal(0);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &tmpy);
    realToString(&tmpy, tmpStr3000);
    graph_ymin = strtof(tmpStr3000, NULL);
    //printf("%s %f\n",tmpStr3000,graph_ymin);
    fnDrop(0);
  }
  else
  if(JM_OPCODE == 24) {                                         //Graph
    saveStack();
    //Convert from X register to float
    real_t tmpy;
    fnToReal(0);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &tmpy);
    realToString(&tmpy, tmpStr3000);
    graph_ymax = strtof(tmpStr3000, NULL);
    //printf("%s %f\n",tmpStr3000,graph_ymax);
    fnDrop(0);
  }
  else
  if(JM_OPCODE == 25) {                                         //Graph
    saveStack();
    //Convert from X register to float
    real_t tmpy;
    fnToReal(0);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &tmpy);
    realToString(&tmpy, tmpStr3000);
    graph_dx = strtof(tmpStr3000, NULL);
    //printf("%s %f\n",tmpStr3000,graph_dx);
    fnDrop(0);
  }
  else
  if(JM_OPCODE == 26) {                                         //Graph
    saveStack();
    //Convert from X register to float
    real_t tmpy;
    fnToReal(0);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &tmpy);
    realToString(&tmpy, tmpStr3000);
    graph_dy = strtof(tmpStr3000, NULL);
    //printf("%s %f\n",tmpStr3000,graph_dy);
    fnDrop(0);
  }
  else
  if(JM_OPCODE == 27) {                                         //Graph
    saveStack();
    fnStrtoX("Type limits into X Register and press");
    fnStrtoX("[Xmin], [Xmax], [Ymin], [Ymax], [dX], [dY]");
    tmpStr3000[0]=0;
    char tmp[12];
    snprintf(tmp, 12, "%.5f, ", graph_xmin);
    strcat(tmpStr3000,tmp);
    snprintf(tmp, 12, "%.5f, ", graph_xmax);
    strcat(tmpStr3000,tmp);
    snprintf(tmp, 12, "%.5f, ", graph_ymin);
    strcat(tmpStr3000,tmp);
    snprintf(tmp, 12, "%.5f, ", graph_ymax);
    strcat(tmpStr3000,tmp);
    snprintf(tmp, 12, "%.3f, ", graph_dx);
    strcat(tmpStr3000,tmp);
    snprintf(tmp, 12, "%.3f", graph_dy);
    strcat(tmpStr3000,tmp);
    fnStrtoX(tmpStr3000);
    fnStrtoX("[PLOT] graphs, [SNAP] saves screen");
    refreshStack();
  }
  else



  if(JM_OPCODE == 31) {                                         //UNDO
    restoreStack();
  }
  else


  if(JM_OPCODE == 45) {                                         //PRIME stats
    #ifndef TESTSUITE_BUILD

    longInteger_t xx3;
    longIntegerInit(xx3);
    runFunction(ITM_CLSIGMA);

    //Get 'from' loop value from Z: 1 to 399 (default 0), for starting number exponents 10^(4*1) through 10^(4*399)
    uint16_t ix1, ixx1;
    if(getRegisterDataType(REGISTER_Z) != dtLongInteger) {
      convertReal34ToLongIntegerRegister(REGISTER_REAL34_DATA(REGISTER_Z), REGISTER_Z, DEC_ROUND_DOWN);
      refreshRegisterLine(REGISTER_Z);
    }
    if(getRegisterDataType(REGISTER_Z) == dtLongInteger) {
      convertLongIntegerRegisterToLongInteger(REGISTER_Z, xx3);
      longIntegerToAllocatedString(xx3, tmpStr3000, TMP_STR_LENGTH);
      longIntegerToInt(xx3,ix1);
    } else {ix1 = 0;}
    if(ix1 > 399) {ix1 = 0;}
    ixx1 = ix1;

    //Get 'to' (maximum) loops from Y: ix1 to 399  (default 0 or ix1), for starting number exponents 10^ix1 through 10^(4*399)
    uint16_t ix2;
    if(getRegisterDataType(REGISTER_Y) != dtLongInteger) {
      convertReal34ToLongIntegerRegister(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_Y, DEC_ROUND_DOWN);
      refreshRegisterLine(REGISTER_Y);
    }
    if(getRegisterDataType(REGISTER_Y) == dtLongInteger) {
      convertLongIntegerRegisterToLongInteger(REGISTER_Y, xx3);
      longIntegerToAllocatedString(xx3, tmpStr3000, TMP_STR_LENGTH);
      longIntegerToInt(xx3,ix2);
    } else {ix2 = 0;}
    if(ix2 < ix1) {ix2 = ix1;}
    if(ix2 > 399) {ix2 = ix1;}

    //Get number of repeated nextprimes from X from 1 to 100, default 10
    uint16_t ix3 = 0;
    if(getRegisterDataType(REGISTER_X) != dtLongInteger) {
      convertReal34ToLongIntegerRegister(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_X, DEC_ROUND_DOWN);
      refreshRegisterLine(REGISTER_X);
    }
    if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
      convertLongIntegerRegisterToLongInteger(REGISTER_X, xx3);
      longIntegerToAllocatedString(xx3, tmpStr3000, TMP_STR_LENGTH);
      longIntegerToInt(xx3,ix3);
    } else {ix3 = 10;}
    if(ix3 < 1 || ix3 > 100) {ix3 = 10;}
    nprimes = ix3;



    //Outer loop start
    while (ix1 <= ix2 ) {
      //printf("--%lld--%lld--\n",ix1,ix1*4);
      uIntToLongInteger(ix1*4, xx3);
      convertLongIntegerToLongIntegerRegister(xx3, REGISTER_X);
      adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
      tenPowLonI();

      clearScreen(false,true,true);
      sprintf(tmpStr3000,"i=10^4Z to i=10^4Y, n primes>i:ZYX %d %d %d|",ix1, ix2,ix3);
      #ifdef PC_BUILD
        printf(tmpStr3000);
      #endif
      print_linestr(tmpStr3000,true);

      fnJM(46);

      STACK_LIFT_ENABLE;
      liftStack();
      uIntToLongInteger(ix1*4, xx3);
      convertLongIntegerToLongIntegerRegister(xx3, REGISTER_X);
      adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);

      runFunction(ITM_SIGMAPLUS);
      STACK_LIFT_ENABLE;

      ix1++;
    }
    longIntegerFree(xx3);
    runFunction(ITM_DROP);

    if(ixx1!=ix2) {runFunction(ITM_PLOT);}
    #endif
  }
  else





 if(JM_OPCODE == 46) {                                         // NEXT |PRIME ROUTINE
    #ifndef TESTSUITE_BUILD
    uint32_t getUptimeMs0 = getUptimeMs();
    int16_t ix;

    calcMode = CM_BUG_ON_SCREEN;              //Hack to prevent calculator to restart operation. Used to view graph

    longInteger_t lgInt;
    longIntegerInit(lgInt);

    ix = 0;
    while (ix < nprimes ) {
      runFunction(ITM_NEXTP);
      fnStore(ix);

      char tmpstr[200];
      char tmpstr2[300];
      sprintf(tmpstr2,"[%d]:",ix);
      longIntegerRegisterToDisplayString(REGISTER_X, tmpstr, 200, 399-32, 50, STD_SPACE_4_PER_EM);
      strcat(tmpstr2,tmpstr);

      print_linestr(tmpstr2,false);

      ix++;

    }
    getUptimeMs0 = getUptimeMs() - getUptimeMs0;

    STACK_LIFT_ENABLE;
    liftStack();
    uIntToLongInteger(getUptimeMs0, lgInt);
    convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
    adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
    longIntegerFree(lgInt);
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
    kbd_usr[0].primary     = KEY_CC;
    kbd_usr[0].gShifted    = KEY_TYPCON_UP;
    kbd_usr[0].fShifted    = KEY_TYPCON_DN;
    Norm_Key_00_VAR        = KEY_CC;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    Show_User_Keys();
    break;


#if defined (JM_LAYOUT_1A)  && !defined (TESTSUITE_BUILD)                          //JM LAYOUT 1A.
  case USER_COMPLEX:                                            //USER_COMPLEX FOR USER: U^ ENTER^ CC
    kbd_usr[12].fShifted   = KEY_CC;                            //JM Changed CPX menu therefore USER MODE changes
    kbd_usr[0].primary     = -MNU_MyMenu;
    kbd_usr[0].gShifted    = KEY_TYPCON_UP;
    kbd_usr[0].fShifted    = KEY_TYPCON_DN;
    Norm_Key_00_VAR        = -MNU_MyMenu;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    Show_User_Keys();
    break;

  case USER_CC:                                            //USER_COMPLEX FOR USER: U^ ENTER^ CC
    kbd_usr[0].primary     = KEY_CC;
    Norm_Key_00_VAR        = KEY_CC;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    Show_User_Keys();
    break;
#endif

#if defined (JM_LAYOUT_2_DM42_STRICT) && !defined (TESTSUITE_BUILD)              //JM LAYOUT 42
  case USER_COMPLEX:                                            //USER_COMPLEX FOR USER: U^ ENTER^ CC
    //kbd_usr[12].gShifted   = KEY_CC;
    kbd_usr[0].primary     = -MNU_MyMenu;
    kbd_usr[0].gShifted    = KEY_TYPCON_UP;
    kbd_usr[0].fShifted    = KEY_TYPCON_DN;
    Norm_Key_00_VAR        = -MNU_MyMenu;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    Show_User_Keys();
    break;

  case USER_CC:                                            //USER_COMPLEX FOR USER: U^ ENTER^ CC
    kbd_usr[0].primary     = KEY_CC;
    Norm_Key_00_VAR        = KEY_CC;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    Show_User_Keys();
    break;
#endif


  case USER_V43:          //USER

    kbd_usr[0].primary      = ITM_1ONX;
    kbd_usr[0].fShifted     = ITM_YX;
    kbd_usr[0].gShifted     = ITM_toINT;

    kbd_usr[1].primary      = ITM_SQUAREROOTX;
    kbd_usr[1].fShifted     = ITM_SQUARE;
    kbd_usr[1].gShifted     = ITM_ms;

    kbd_usr[2].primary      = ITM_LOG10;
    kbd_usr[2].fShifted     = ITM_10x;
    kbd_usr[2].gShifted     = KEY_dotD;

    kbd_usr[3].primary      = ITM_LN;
    kbd_usr[3].fShifted     = ITM_EX;
    kbd_usr[3].gShifted     = ITM_TGLFRT;

    kbd_usr[4].primary      = KEY_f;
    kbd_usr[4].fShifted     = ITM_NULL;
    kbd_usr[4].gShifted     = ITM_NULL;
    kbd_usr[4].keyLblAim     = KEY_f;
    kbd_usr[4].primaryAim     = KEY_f;
     kbd_usr[4].fShiftedAim     = ITM_NULL;
     kbd_usr[4].gShiftedAim     = ITM_NULL;
    kbd_usr[4].primaryTam     = KEY_f;

    kbd_usr[5].primary      = KEY_g;
    kbd_usr[5].fShifted     = ITM_NULL;
    kbd_usr[5].gShifted     = ITM_NULL;
    kbd_usr[5].keyLblAim     = KEY_g;
    kbd_usr[5].primaryAim     = KEY_g;
     kbd_usr[5].fShiftedAim     = ITM_NULL;
     kbd_usr[5].gShiftedAim     = ITM_NULL;
    kbd_usr[5].primaryTam     = KEY_g;

    kbd_usr[6].primaryAim     = CHR_E;
    kbd_usr[7].primaryAim     = CHR_F;
    kbd_usr[8].primaryAim     = CHR_G;
    kbd_usr[9].primaryAim     = CHR_H;
    kbd_usr[10].primaryAim     = CHR_I;
    kbd_usr[11].primaryAim     = CHR_J;

    kbd_usr[13].primaryAim     = CHR_K;
    kbd_usr[14].primaryAim     = CHR_L;
    kbd_usr[15].primaryAim     = CHR_M;

    kbd_usr[17].primary     = ITM_SUB;
    kbd_usr[17].fShifted    = -MNU_STAT;
    kbd_usr[17].gShifted    = -MNU_SUMS;
    kbd_usr[17].keyLblAim     = CHR_MINUS;
    kbd_usr[17].primaryAim     = CHR_N;
    kbd_usr[17].fShiftedAim     = CHR_MINUS;
    kbd_usr[17].primaryTam     = ITM_SUB;

    kbd_usr[18].primaryAim     = CHR_O;
    kbd_usr[19].primaryAim     = CHR_P;
    kbd_usr[20].primaryAim     = CHR_Q;

    kbd_usr[21].primary     = ITM_XEQ;
    kbd_usr[21].fShifted    = ITM_AIM;
    kbd_usr[21].gShifted    = ITM_NULL;
    kbd_usr[21].keyLblAim     = ITM_NULL;
    kbd_usr[21].primaryAim     = CHR_R;
    kbd_usr[21].fShiftedAim     = ITM_NULL;
    kbd_usr[21].gShiftedAim     = ITM_NULL;
    kbd_usr[21].primaryTam     = CHR_alpha;

    kbd_usr[22].primary     = ITM_ADD;
    kbd_usr[22].fShifted    = -MNU_PROB;
    kbd_usr[22].gShifted    = -MNU_INTS;
    kbd_usr[22].keyLblAim     = CHR_PLUS;
    kbd_usr[22].primaryAim     = CHR_S;
    kbd_usr[22].fShiftedAim     = CHR_PLUS;
    kbd_usr[22].primaryTam     = ITM_ADD;

    kbd_usr[26].primary     = KEY_UP1;
    kbd_usr[26].fShifted    = KEY_BST;
    kbd_usr[26].gShifted    = ITM_RBR;
    kbd_usr[26].keyLblAim     = KEY_UP1;
    kbd_usr[26].primaryAim     = KEY_UP1;  //arrow
    kbd_usr[26].fShiftedAim     = KEY_BST;
    kbd_usr[26].gShiftedAim     = ITM_NULL;
    kbd_usr[26].primaryTam     = KEY_UP1;

    kbd_usr[27].primary     = ITM_MULT;       //ex-f/g
    kbd_usr[27].fShifted    = -MNU_STK;
    kbd_usr[27].gShifted    = -MNU_ALPHAFN;
    kbd_usr[27].keyLblAim     = CHR_CROSS;
    kbd_usr[27].primaryAim     = CHR_W;
    kbd_usr[27].fShiftedAim     = CHR_CROSS;
    kbd_usr[27].gShiftedAim     = MNU_ALPHAINTL;  //??
    kbd_usr[27].primaryTam     = ITM_MULT;

    kbd_usr[31].primary     = KEY_DOWN1;
    kbd_usr[31].fShifted    = KEY_SST;
    kbd_usr[31].gShifted    = ITM_STATUS;
    kbd_usr[31].keyLblAim     = KEY_DOWN1;
    kbd_usr[31].primaryAim     = KEY_DOWN1;  //arrow
    kbd_usr[31].fShiftedAim     = KEY_SST;
    kbd_usr[31].gShiftedAim     = ITM_NULL;
    kbd_usr[31].primaryTam     = KEY_DOWN1;

    kbd_usr[32].primary     = ITM_DIV;   //ex-exit
    kbd_usr[32].fShifted    = -MNU_CATALOG;
    kbd_usr[32].gShifted    = -MNU_PRINT;
    kbd_usr[32].keyLblAim     = CHR_DIVIDE;
    kbd_usr[32].primaryAim     = CHR_SPACE;
    kbd_usr[32].fShiftedAim     = CHR_DIVIDE;
    kbd_usr[32].gShiftedAim     = CHR_PRINTER;
    kbd_usr[32].primaryTam     = ITM_DIV;

    kbd_usr[36].primary     = KEY_EXIT1;
    kbd_usr[36].fShifted    = ITM_OFF;
    kbd_usr[36].gShifted    = -MNU_IO;
    kbd_usr[36].keyLblAim     = KEY_EXIT1;
    kbd_usr[36].primaryAim     = KEY_EXIT1;  //
    kbd_usr[36].fShiftedAim     = ITM_OFF;
    kbd_usr[36].primaryTam     = KEY_EXIT1;

    Norm_Key_00_VAR         = ITM_1ONX;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    Show_User_Keys();
    break;




  case USER_SHIFTS:                                             //USER_SHIFTS 25          //JM Sectioon to be put on a menu
    kbd_usr[0].primary     = KEY_USERMODE;
    kbd_usr[9].primary     = -MNU_TRI;
    kbd_usr[9].fShifted    = KEY_USERMODE;
       //kbd_usr[9].gShifted    = ITM_RTN;
       //kbd_usr[9].fShifted    = ITM_NULL;
    kbd_usr[10].primary    = KEY_f;
    kbd_usr[10].fShifted   = ITM_NULL;
    kbd_usr[10].gShifted   = ITM_NULL;
    kbd_usr[11].primary    = KEY_g;
    kbd_usr[11].fShifted   = ITM_NULL;
    kbd_usr[11].gShifted   = ITM_NULL;
    Norm_Key_00_VAR        = KEY_USERMODE;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    Show_User_Keys();
    break;


  case USER_SIGMAPLUS:                                              //USER_DEFAULTS FOR USER: E+ E+
    kbd_usr[0].primary     = ITM_SIGMAPLUS;
    Norm_Key_00_VAR        = ITM_SIGMAPLUS;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    Show_User_Keys();
    break;

  case USER_PRGM:                                              //USER_DEFAULTS FOR USER: E+ PRGM
    kbd_usr[0].primary     = ITM_PR;
    Norm_Key_00_VAR        = ITM_PR;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    Show_User_Keys();
    break;

  case USER_USER:                                              //USER_DEFAULTS FOR USER: E+ USER
    kbd_usr[0].primary     = KEY_USERMODE;
    Norm_Key_00_VAR        = KEY_USERMODE;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    Show_User_Keys();
    break;

  case USER_ALPHA:                                              //USER_DEFAULTS FOR USER: E+ ALPHA
    kbd_usr[0].primary     = ITM_AIM;
    Norm_Key_00_VAR        = ITM_AIM;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    Show_User_Keys();
    break;

  case USER_MYM:                                              //USER_DEFAULTS FOR USER: E+ MYM
    kbd_usr[0].primary     = -MNU_MyMenu;
    Norm_Key_00_VAR        = -MNU_MyMenu;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    Show_User_Keys();
    break;

  case USER_GSHFT:                                           //USER_DEFAULTS FOR USER: E+ SHIFT G
    kbd_usr[0].primary     = KEY_g;
    kbd_usr[0].gShifted    = ITM_NULL;
    kbd_usr[0].fShifted    = ITM_NULL;
    Norm_Key_00_VAR        = KEY_g;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    Show_User_Keys();
    break;

  case USER_RESET:                                              //USER_RESET 26
    xcopy(kbd_usr, kbd_std, sizeof(kbd_std));
    fnClearFlag(FLAG_USER); //userModeEnabled = false;
    Norm_Key_00_VAR        = ITM_SIGMAPLUS;
    fnRefreshRadioState(RB_SA, Norm_Key_00_VAR);
    Show_User_Keys();
    break;

/*
  case JM_ASSIGN:                 //A non 0 and non 32766 value means the FN NUMBER is in JM_ASSIGN, AND KEYBOARD.C will wait for a key to be assigned to                                     //USER_RESET 27
    JM_ASN_MODE = KEY_CC;         //TEMPORARY TEST FUNCTION
    break;
*/

  case JM_SEEK_FN:      //32766 in KEYBOARD.C will wait for a key. SEEK FUNCTION,         //USER_RESET 27
    JM_ASN_MODE = 32766;
#ifndef TESTSUITE_BUILD
    clearScreen(false,true,false);
    showString("Select function from keys: EXIT Aborts", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Z - REGISTER_X), vmNormal, true, true);
#endif
    break;

  default:
    break;
  }
}




void fnKEYSELECT(void) {                                        //JM ASSIGN - REMEMBER NEXT KEYBOARD FUNCTION
  if(JM_ASN_MODE == KEY_EXIT1 || JM_ASN_MODE == KEY_BACKSPACE) {
    JM_ASN_MODE = 0;
#ifndef TESTSUITE_BUILD
    showString("Abandoned or illegal function", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Y - REGISTER_X), vmNormal, true, true);
#endif
  }
  else {
#ifndef TESTSUITE_BUILD
    showString("Select key: top 4 lines excl. FN1-6 & [<-],", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Y - REGISTER_X), vmNormal, true, true);
    showString("incl. [/] [*] [-] [+] [R/S].   EXIT aborts.", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_X - REGISTER_X), vmNormal, true, true);
#endif

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
    case 0:              //Only allowable re-assigned keys in the case statement
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

      Show_User_Keys();
      refreshStack();
      break;
    default:
#ifndef TESTSUITE_BUILD
        clearScreen(false,true,false);
        showString("Invalid key", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Z - REGISTER_X), vmNormal, true, true);
#endif
    break;
  }
}




/*
void JM_convertIntegerToShortIntegerRegister(int16_t inp, uint32_t base, calcRegister_t destination) {
  char snum[10];
  itoa(inp, snum, base);
  longInteger_t mem;
  longIntegerInit(mem);
  liftStack();
  stringToLongInteger(snum,base,mem);
  convertLongIntegerToShortIntegerRegister(mem, base, destination);

  //setRegisterShortIntegerBase(destination, base);
  if(lastIntegerBase != 0) {
    fnChangeBase(lastIntegerBase);                     //This converts shortint, longint and real to shortint!
  }
  else {
    fnChangeBase(base);                                //This converts shortint, longint and real to shortint!
  }

  longIntegerFree(mem);
  refreshStack();
}
*/










//*********************
//JM\/\/\/\/
/*
#ifdef NOTNEEDED. KEPT FOR REFERENCE

void fnComplexCCCC_CPX(uint16_t unusedParamButMandatory) {      //JM HARDWAIRED FOR f[COMPLEX]
  userModeEnabledMEM = userModeEnabled;
  userModeEnabled = false;

#ifdef JM_LAYOUT_1A               //JM LAYOUT 1A. CHALLENGE.
  shiftF = true;                  //JM
  shiftG = false;                 //JM
  #ifdef PC_BUILD
  btnClicked(NULL, "12");         //JM changed from 02
  #endif
  #ifdef DMCP_BUILD
  btnClicked(NULL, "12");         //JM changed from 02
  #endif
#endif

#ifdef JM_LAYOUT_2_DM42_STRICT    //JM LAYOUT 2. DM42 STRICT.
  shiftF = true;                  //JM
  shiftG = false;                 //JM
  #ifdef PC_BUILD
  btnClicked(NULL, "06");         //JM changed from 02
  #endif

  #ifdef DMCP_BUILD
  btnClicked(NULL, "06");         //JM changed from 02
  #endif
#endif
  userModeEnabled = userModeEnabledMEM;
}



//void fnComplexCCCC_CC1(uint16_t unusedParamButMandatory) {      //FOR CC1  HARDWIRED FOR TOP LEFT BUTTON
//#ifndef TESTSUITE_BUILD
//  userModeEnabledMEM = userModeEnabled;
//  userModeEnabled = false;
//  shiftF = false;                 //JM
//  shiftG = false;                 //JM
//  int16_t Norm_Key_00_VAR_M;
//  Norm_Key_00_VAR_M = Norm_Key_00_VAR;   //JM Use key 00 as temporary location for CC, which is not normally on the 43C keyboard.
//  Norm_Key_00_VAR        = KEY_CC1;               //JM
//  btnClicked(NULL, "00");
//  Norm_Key_00_VAR        = Norm_Key_00_VAR_M;    //JM
//  userModeEnabled = userModeEnabledMEM;
//#endif
//}



void fnComplexCCCC_CC(uint16_t unusedParamButMandatory) {       //FOR CC  HARDWIRED FOR TOP LEFT BUTTON
#ifndef TESTSUITE_BUILD
  userModeEnabledMEM = userModeEnabled;
  userModeEnabled = false;
  shiftF = false;                 //JM
  shiftG = false;                 //JM
  int16_t Norm_Key_00_VAR_M;
  Norm_Key_00_VAR_M = Norm_Key_00_VAR;   //JM Use key 00 as temporary location for CC, which is not normally on the 43C keyboard.
  Norm_Key_00_VAR        = KEY_CC;               //JM
  btnClicked(NULL, "00");
  Norm_Key_00_VAR        = Norm_Key_00_VAR_M;    //JM
  userModeEnabled = userModeEnabledMEM;
#endif
}

#endif //NOTNEEDED
*/


//JM^^^^^^^



//FUTURE
/*
void ItemBrowser(uint16_t unusedParamButMandatory) {
  int16_t registerNameWidth;

  if(currentRegisterBrowserScreen == 9999) { // Init
    currentRegisterBrowserScreen = 0;
    rbrMode = RBR_GLOBAL;
    showContent = true;
    rbr1stDigit = true;
    calcMode = CM_REGISTER_BROWSER;
  }

  if(currentRegisterBrowserScreen < 9999) {
    clearScreen(false, true, true);

    int16_t regist;
    for(int16_t row=0; row<10; row++) {
      regist = (currentRegisterBrowserScreen + row); // % FIRST_LOCAL_REGISTER;

      itoa(regist, tmpStr3000, 10);
      registerNameWidth = showString(tmpStr3000, &standardFont, 1, 219-22*row, vmNormal, false, true);

      strcpy(tmpStr3000, "'");
      strcpy(tmpStr3000, indexOfItems[regist].itemPrinted);
      strcat(tmpStr3000, "'");
      if(stringWidth(tmpStr3000, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
        tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
        while(stringWidth(tmpStr3000, &standardFont, false, true) >= SCREEN_WIDTH - 12 - registerNameWidth) { // 12 is the width of STD_ELLIPSIS
          tmpStr3000[stringLastGlyph(tmpStr3000)] = 0;
        }
        strcat(tmpStr3000 + stringByteLength(tmpStr3000), STD_ELLIPSIS);
      }
      showString(tmpStr3000, &standardFont, SCREEN_WIDTH - stringWidth(tmpStr3000, &standardFont, false, true) - 1, 219-22*row, vmNormal, false, true);
    }
  }
}
*/






