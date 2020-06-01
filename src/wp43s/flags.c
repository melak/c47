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
 * \file flags.c
 ***********************************************/

#include "wp43s.h"

void systemFlagAction(uint16_t systemFlag, uint16_t action) {
  switch(systemFlag) {
    case FLAG_YMD:
    case FLAG_DMY:
    case FLAG_MDY:
    case FLAG_TDM24:    oldTime[0] = 0;
                        break;

    case FLAG_ALLENG:
    case FLAG_DECIMP:
    case FLAG_MULTx:
    case FLAG_CPXj:     refreshStack();
                        break;

    case FLAG_LEAD0:    refreshStack();
                        switch(action) {
                          case 0: flags[108/16] &= ~(1u << (108%16));
                                  break;
                          case 1: flags[108/16] |=   1u << (108%16);
                                  break;
                          case 2: flags[108/16] ^=   1u << (108%16);
                                  break;
                          default: {}
                        }
                        break;

    case FLAG_POLAR:    showComplexMode();
                        refreshStack();
                        switch(action) {
                          case 0: flags[100/16] &= ~(1u << (100%16));
                                  break;
                          case 1: flags[100/16] |=   1u << (100%16);
                                  break;
                          case 2: flags[100/16] ^=   1u << (100%16);
                                  break;
                          default: {}
                        }
                        break;

    case FLAG_CPXRES:   showRealComplexResult();
                        switch(action) {
                          case 0: flags[109/16] &= ~(1u << (109%16));
                                  break;
                          case 1: flags[109/16] |=   1u << (109%16);
                                  break;
                          case 2: flags[109/16] ^=   1u << (109%16);
                                  break;
                          default: {}
                        }
                        break;

    case FLAG_SPCRES:   switch(action) {
                          case 0: flags[107/16] &= ~(1u << (107%16));
                                  break;
                          case 1: flags[107/16] |=   1u << (107%16);
                                  break;
                          case 2: flags[107/16] ^=   1u << (107%16);
                                  break;
                          default: {}
                        }
                        break;

    case FLAG_TRACE:   switch(action) {
                          case 0: flags[103/16] &= ~(1u << (103%16));
                                  break;
                          case 1: flags[103/16] |=   1u << (103%16);
                                  break;
                          case 2: flags[103/16] ^=   1u << (103%16);
                                  break;
                          default: {}
                        }
                        break;

    case FLAG_FRACT:
    case FLAG_DENANY:
    case FLAG_DENFIX:   showFracMode();
                        refreshStack();
                        break;

    case FLAG_CARRY:    showOverflowCarry();
                        switch(action) {
                          case 0: flags[106/16] &= ~(1u << (106%16));
                                  break;
                          case 1: flags[106/16] |=   1u << (106%16);
                                  break;
                          case 2: flags[106/16] ^=   1u << (106%16);
                                  break;
                          default: {}
                        }
                        break;

    case FLAG_OVERFLOW: showOverflowCarry();
                        switch(action) {
                          case 0: flags[105/16] &= ~(1u << (105%16));
                                  break;
                          case 1: flags[105/16] |=   1u << (105%16);
                                  break;
                          case 2: flags[105/16] ^=   1u << (105%16);
                                  break;
                          default: {}
                        }
                        break;

    case FLAG_USER:     showHideUserMode();
                        break;

    case FLAG_LOWBAT:   showHideLowBattery();
                        break;

    default: {}
  }




  if(getSystemFlag(systemFlag))                                     //JM vv
    switch (systemFlag) {              //SET
      case FLAG_DMY:     fnRefreshRadioState (RB_DF,DF_DMY);break;
      case FLAG_MDY:     fnRefreshRadioState (RB_DF,DF_MDY);break;
      case FLAG_YMD:     fnRefreshRadioState (RB_DF,DF_YMD);break;

      case FLAG_DECIMP:  fnRefreshRadioState (RB_RM, RM_PERIOD);refreshStack();break;
      case FLAG_TDM24:   fnRefreshRadioState (RB_TF,TF_H24);break;
      case FLAG_CPXj:    fnRefreshRadioState (RB_CU,CU_J); refreshStack();break;
      case FLAG_LEAD0:   fnRefreshComboxState(CB_JC, JC_BLZ, true); refreshStack();break;
      case FLAG_MULTx:   fnRefreshRadioState (RB_PS, PS_CROSS);break;
      case FLAG_SSIZE8:  fnRefreshRadioState (RB_SS, SS_8);break;
      case FLAG_POLAR:   fnRefreshRadioState (RB_CM, CM_POLAR); showComplexMode(); refreshStack();    break;
      case FLAG_CPXRES:  fnRefreshComboxState(CB_JC, JC_BCR, true);showRealComplexResult(); break;
      default:;
    }
    else
      switch (systemFlag) {          //RESET
      case FLAG_DECIMP:  fnRefreshRadioState (RB_RM, RM_COMMA);refreshStack();break;
      case FLAG_TDM24:   fnRefreshRadioState (RB_TF, TF_H12);break;
      case FLAG_CPXj:    fnRefreshRadioState (RB_CU, CU_I); refreshStack();break;
      case FLAG_LEAD0:   fnRefreshComboxState(CB_JC, JC_BLZ, false); refreshStack();break;
      case FLAG_MULTx:   fnRefreshRadioState (RB_PS, PS_DOT);break;
      case FLAG_SSIZE8:  fnRefreshRadioState (RB_SS, SS_4);break;
      case FLAG_POLAR:   fnRefreshRadioState (RB_CM, CM_RECTANGULAR); showComplexMode(); refreshStack();break;
      case FLAG_CPXRES:  fnRefreshComboxState(CB_JC, JC_BCR, false);showRealComplexResult();break;
      default:;
    }                                                            //JM vv





}

/********************************************//**
 * \brief Returns the status of a flag
 *
 * \param[in] flag uint16_t
 * \return bool_t
 ***********************************************/
bool_t getFlag(uint16_t flag) {
  if(flag & 0x8000) { // System flag
    return getSystemFlag(flag);
  }
  else if(flag < NUMBER_OF_GLOBAL_FLAGS) { // Global flag
    return (flags[flag/16] & (1u << (flag%16))) != 0;
  }
  else { // Local flag
    if(numberOfLocalFlags != 0) {
      flag -= NUMBER_OF_GLOBAL_FLAGS;
      if(flag < NUMBER_OF_LOCAL_FLAGS) {
        return (allLocalRegisterPointer->localFlags & (1u << flag)) != 0;
      }
      else {
        sprintf(errorMessage, "In function getFlag: local flag %" FMT16U " is not defined! Must be from 0 to %d.", flag, NUMBER_OF_LOCAL_FLAGS - 1);
        displayBugScreen(errorMessage);
      }
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function getFlag:", "no local flags defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
    #endif
  }
  return false;
 }



/********************************************//**
 * \brief Returns the status of a system flag
 *
 * \param[in] systemFlag uint16_t
 * \return void
 ***********************************************/
void fnGetSystemFlag(uint16_t systemFlag) {
  if(getSystemFlag(systemFlag)) {
    temporaryInformation = TI_TRUE;
  }
  else {
    temporaryInformation = TI_FALSE;
  }

  refreshRegisterLine(TAM_REGISTER_LINE);
  refreshRegisterLine(REGISTER_X);
 }



/********************************************//**
 * \brief Sets a flag
 *
 * \param[in] flag uint16_t
 * \return void
 ***********************************************/
void fnSetFlag(uint16_t flag) {
  if(flag & 0x8000) { // System flag
    if(isSystemFlagWriteProtected(flag)) {
      temporaryInformation = TI_NO_INFO;
      displayCalcErrorMessage(ERROR_WRITE_PROTECTED_SYSTEM_FLAG, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "protected system flag (%" FMT16U ")!", flag & 0x3fff);
        showInfoDialog("In function fnSetFlag:", "Tying to set a write", errorMessage, NULL);
      #endif
      return;
    }
    else {
      setSystemFlag(flag);
    }
  }
  else if(flag < NUMBER_OF_GLOBAL_FLAGS) { // Global flag
    switch(flag) {
      case FLAG_B: setSystemFlag(FLAG_OVERFLOW); break;
      case FLAG_C: setSystemFlag(FLAG_CARRY);    break;
      case FLAG_D: setSystemFlag(FLAG_SPCRES);   break;
      case FLAG_I: setSystemFlag(FLAG_CPXRES);   break;
      case FLAG_L: setSystemFlag(FLAG_LEAD0);    break;
      case FLAG_T: setSystemFlag(FLAG_TRACE);    break;
      case FLAG_X: setSystemFlag(FLAG_POLAR);    break;
      default: flags[flag/16] |= 1u << (flag%16);
    }
  }
  else { // Local flag
    if(numberOfLocalFlags != 0) {
      flag -= NUMBER_OF_GLOBAL_FLAGS;
      if(flag < NUMBER_OF_LOCAL_FLAGS) {
        allLocalRegisterPointer->localFlags |=  (1u << flag);
      }
      else {
        sprintf(errorMessage, "In function fnSetFlag: local flag %" FMT16U " is not defined! Must be from 0 to %d.", flag, NUMBER_OF_LOCAL_FLAGS - 1);
        displayBugScreen(errorMessage);
      }
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function fnSetFlag:", "no local flags defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
    #endif
  }

  refreshRegisterLine(TAM_REGISTER_LINE);
}



/********************************************//**
 * \brief Clears a flag
 *
 * \param[in] flags uint16_t
 * \return void
 ***********************************************/
void fnClearFlag(uint16_t flag) {
  if(flag & 0x8000) { // System flag
    if(isSystemFlagWriteProtected(flag)) {
      temporaryInformation = TI_NO_INFO;
      displayCalcErrorMessage(ERROR_WRITE_PROTECTED_SYSTEM_FLAG, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "protected system flag (%" FMT16U ")!", flag & 0x3fff);
        showInfoDialog("In function fnClearFlag:", "Tying to clear a write", errorMessage, NULL);
      #endif
      return;
    }
    else {
      clearSystemFlag(flag);
    }
  }
  else if(flag < NUMBER_OF_GLOBAL_FLAGS) { // Global flag
    switch(flag) {
      case FLAG_B: clearSystemFlag(FLAG_OVERFLOW); break;
      case FLAG_C: clearSystemFlag(FLAG_CARRY);    break;
      case FLAG_D: clearSystemFlag(FLAG_SPCRES);   break;
      case FLAG_I: clearSystemFlag(FLAG_CPXRES);   break;
      case FLAG_L: clearSystemFlag(FLAG_LEAD0);    break;
      case FLAG_T: clearSystemFlag(FLAG_TRACE);    break;
      case FLAG_X: clearSystemFlag(FLAG_POLAR);    break;
      default: flags[flag/16] &= ~(1u << (flag%16));
    }
  }
  else { // Local flag
    if(numberOfLocalFlags != 0) {
      flag -= NUMBER_OF_GLOBAL_FLAGS;
      if(flag < NUMBER_OF_LOCAL_FLAGS) {
        allLocalRegisterPointer->localFlags &= ~(1u << flag);
      }
      else {
        sprintf(errorMessage, "In function fnClearFlag: local flag %" FMT16U " is not defined! Must be from 0 to %d.", flag, NUMBER_OF_LOCAL_FLAGS - 1);
        displayBugScreen(errorMessage);
      }
    }
    #ifdef PC_BUILD
    else {
     showInfoDialog("In function fnClearFlag:", "no local flags defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
   #endif
  }

  refreshRegisterLine(TAM_REGISTER_LINE);
}



/********************************************//**
 * \brief Flips a flag
 *
 * \param[in] flags uint16_t
 * \return void
 ***********************************************/
void fnFlipFlag(uint16_t flag) {
  if(flag & 0x8000) { // System flag
    if(isSystemFlagWriteProtected(flag)) {
      temporaryInformation = TI_NO_INFO;
      displayCalcErrorMessage(ERROR_WRITE_PROTECTED_SYSTEM_FLAG, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "protected system flag (%" FMT16U ")!", flag & 0x3fff);
        showInfoDialog("In function fnFlipFlag:", "Tying to flip a write", errorMessage, NULL);
      #endif
      return;
    }
    else {
      flipSystemFlag(flag);
    }
  }
  else if(flag < NUMBER_OF_GLOBAL_FLAGS) { // Global flag
    switch(flag) {
      case FLAG_B: flipSystemFlag(FLAG_OVERFLOW); break;
      case FLAG_C: flipSystemFlag(FLAG_CARRY);    break;
      case FLAG_D: flipSystemFlag(FLAG_SPCRES);   break;
      case FLAG_I: flipSystemFlag(FLAG_CPXRES);   break;
      case FLAG_L: flipSystemFlag(FLAG_LEAD0);    break;
      case FLAG_T: flipSystemFlag(FLAG_TRACE);    break;
      case FLAG_X: flipSystemFlag(FLAG_POLAR);    break;
      default: flags[flag/16] ^=  1u << (flag%16);
    }
  }
  else { // Local flag
    if(numberOfLocalFlags != 0) {
      flag -= NUMBER_OF_GLOBAL_FLAGS;
      if(flag < NUMBER_OF_LOCAL_FLAGS) {
        allLocalRegisterPointer->localFlags ^=  (1u << flag);
      }
      else {
        sprintf(errorMessage, "In function fnFlipFlag: local flag %" FMT16U " is not defined! Must be from 0 to %d.", flag, NUMBER_OF_LOCAL_FLAGS - 1);
        displayBugScreen(errorMessage);
      }
    }
    #ifdef PC_BUILD
    else {
      showInfoDialog("In function fnFlipFlag:", "no local flags defined!", "To do so, you can find LocR here:", "[g] [P.FN] [g] [F5]");
    }
    #endif
  }

  refreshRegisterLine(TAM_REGISTER_LINE);
}



/********************************************//**
 * \brief Clear all global and local flags
 *
 * \param[in] flags uint16_t
 * \return void
 ***********************************************/
void fnClFAll(uint16_t unusedParamButMandatory) {
  memset(flags, 0, sizeof(flags));
  clearSystemFlag(FLAG_OVERFLOW);
  clearSystemFlag(FLAG_CARRY);
  clearSystemFlag(FLAG_SPCRES);
  clearSystemFlag(FLAG_CPXRES);
  clearSystemFlag(FLAG_LEAD0);
  clearSystemFlag(FLAG_TRACE);
  clearSystemFlag(FLAG_POLAR);

  if(numberOfLocalFlags != 0) {
    allLocalRegisterPointer->localFlags = 0;
  }
}



void fnIsFlagClear(uint16_t flag) {
  temporaryInformation = (getFlag(flag) ? TI_FALSE : TI_TRUE);

  refreshRegisterLine(TAM_REGISTER_LINE);
  refreshRegisterLine(TRUE_FALSE_REGISTER_LINE);
}



void fnIsFlagClearClear(uint16_t flag) {
  temporaryInformation = (getFlag(flag) ? TI_FALSE : TI_TRUE);
  fnClearFlag(flag);

  refreshRegisterLine(TAM_REGISTER_LINE);
  refreshRegisterLine(TRUE_FALSE_REGISTER_LINE);
}



void fnIsFlagClearSet(uint16_t flag) {
  temporaryInformation = (getFlag(flag) ? TI_FALSE : TI_TRUE);
  fnSetFlag(flag);

  refreshRegisterLine(TAM_REGISTER_LINE);
  refreshRegisterLine(TRUE_FALSE_REGISTER_LINE);
}



void fnIsFlagClearFlip(uint16_t flag) {
  temporaryInformation = (getFlag(flag) ? TI_FALSE : TI_TRUE);
  fnFlipFlag(flag);

  refreshRegisterLine(TAM_REGISTER_LINE);
  refreshRegisterLine(TRUE_FALSE_REGISTER_LINE);
}



void fnIsFlagSet(uint16_t flag) {
  temporaryInformation = (getFlag(flag) ? TI_TRUE : TI_FALSE);

  refreshRegisterLine(TAM_REGISTER_LINE);
  refreshRegisterLine(TRUE_FALSE_REGISTER_LINE);
}




void fnIsFlagSetClear(uint16_t flag) {
  temporaryInformation = (getFlag(flag) ? TI_TRUE : TI_FALSE);
  fnClearFlag(flag);

  refreshRegisterLine(TAM_REGISTER_LINE);
  refreshRegisterLine(TRUE_FALSE_REGISTER_LINE);
}




void fnIsFlagSetSet(uint16_t flag) {
  temporaryInformation = (getFlag(flag) ? TI_TRUE : TI_FALSE);
  fnSetFlag(flag);

  refreshRegisterLine(TAM_REGISTER_LINE);
  refreshRegisterLine(TRUE_FALSE_REGISTER_LINE);
}




void fnIsFlagSetFlip(uint16_t flag) {
  temporaryInformation = (getFlag(flag) ? TI_TRUE : TI_FALSE);
  fnFlipFlag(flag);

  refreshRegisterLine(TAM_REGISTER_LINE);
  refreshRegisterLine(TRUE_FALSE_REGISTER_LINE);
}
