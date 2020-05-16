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

void systemFlagAction(uint16_t systemFlag) {
  switch(systemFlag) {
    case FLAG_YMD:
    case FLAG_DMY:
    case FLAG_MDY:
    case FLAG_TDM:      oldTime[0] = 0;
                        break;

    case FLAG_DECIMP:
    case FLAG_LEAD0:
    case FLAG_CPXj:     refreshStack();
                        break;

    case FLAG_RECTN:    showComplexMode();
                        refreshStack();
                        break;

    case FLAG_CPXRES:   showRealComplexResult();
                        break;

    case FLAG_FRACT:
    case FLAG_DENANY:
    case FLAG_DENFIX:   showFracMode();
                        refreshStack();
                        break;

    case FLAG_CARRY:
    case FLAG_OVERFLOW: showOverflowCarry();
                        break;

    case FLAG_USER:     showHideUserMode();
                        break;

    case FLAG_LOWBAT:   showHideLowBattery();
                        break;

    default: {}
  }
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
    flags[flag/16] |= 1u << (flag%16);
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
    flags[flag/16] &= ~(1u << (flag%16));
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
    flags[flag/16] ^=  1u << (flag%16);
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
  showRealComplexResult();
  showOverflowCarry();

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
