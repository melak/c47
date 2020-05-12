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

/********************************************//**
 * \brief Returns the status of a flag
 *
 * \param[in] f uint16_t
 * \return bool_t
 ***********************************************/
bool_t getFlag(uint16_t f) {
  if(f < NUMBER_OF_GLOBAL_FLAGS) {
    return (flags[f/16] & (1u << (f%16))) != 0;
  }
  else {
    if(numberOfLocalFlags != 0) {
      f -= NUMBER_OF_GLOBAL_FLAGS;
      if(f < NUMBER_OF_LOCAL_FLAGS) {
        return (allLocalRegisterPointer->localFlags & (1u << f)) != 0;
      }
      else {
        sprintf(errorMessage, "In function getFlag: local flag %" FMT16U " is not defined! Must be from 0 to %d.", f, NUMBER_OF_LOCAL_FLAGS - 1);
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
 * \brief Sets a flag
 *
 * \param[in] f uint16_t
 * \return void
 ***********************************************/
void fnSetFlag(uint16_t f) {
  if(f < NUMBER_OF_GLOBAL_FLAGS) {
    flags[f/16] |= 1u << (f%16);

    if(f == FLAG_CPXRES) {
      showRealComplexResult();
    }
    else if(f == FLAG_OVERFLOW || f == FLAG_CARRY) {
      showOverflowCarry();
    }
  }
  else {
    if(numberOfLocalFlags != 0) {
      f -= NUMBER_OF_GLOBAL_FLAGS;
      if(f < NUMBER_OF_LOCAL_FLAGS) {
        allLocalRegisterPointer->localFlags |=  (1u << f);
      }
      else {
        sprintf(errorMessage, "In function fnSetFlag: local flag %" FMT16U " is not defined! Must be from 0 to %d.", f, NUMBER_OF_LOCAL_FLAGS - 1);
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
 * \param[in] f uint16_t
 * \return void
 ***********************************************/
void fnClearFlag(uint16_t f) {
  if(f < NUMBER_OF_GLOBAL_FLAGS) {
    flags[f/16] &= ~(1u << (f%16));

    if(f == FLAG_CPXRES) {
      showRealComplexResult();
    }
    else if(f == FLAG_OVERFLOW || f == FLAG_CARRY) {
      showOverflowCarry();
    }
  }
  else {
    if(numberOfLocalFlags != 0) {
      f -= NUMBER_OF_GLOBAL_FLAGS;
      if(f < NUMBER_OF_LOCAL_FLAGS) {
        allLocalRegisterPointer->localFlags &= ~(1u << f);
      }
      else {
        sprintf(errorMessage, "In function fnClearFlag: local flag %" FMT16U " is not defined! Must be from 0 to %d.", f, NUMBER_OF_LOCAL_FLAGS - 1);
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
 * \param[in] f uint16_t
 * \return void
 ***********************************************/
void fnFlipFlag(uint16_t f) {
  if(f < NUMBER_OF_GLOBAL_FLAGS) {
    flags[f/16] ^=  1u << (f%16);

    if(f == FLAG_CPXRES) {
      showRealComplexResult();
    }
    else if(f == FLAG_OVERFLOW || f == FLAG_CARRY) {
      showOverflowCarry();
    }
  }
  else {
    if(numberOfLocalFlags != 0) {
      f -= NUMBER_OF_GLOBAL_FLAGS;
      if(f < NUMBER_OF_LOCAL_FLAGS) {
        allLocalRegisterPointer->localFlags ^=  (1u << f);
      }
      else {
        sprintf(errorMessage, "In function fnFlipFlag: local flag %" FMT16U " is not defined! Must be from 0 to %d.", f, NUMBER_OF_LOCAL_FLAGS - 1);
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
 * \param[in] f uint16_t
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



void fnIsFlagClear(uint16_t f) {
  temporaryInformation = (getFlag(f) ? TI_FALSE : TI_TRUE);

  refreshRegisterLine(TAM_REGISTER_LINE);
  refreshRegisterLine(TRUE_FALSE_REGISTER_LINE);
}



void fnIsFlagClearClear(uint16_t f) {
  temporaryInformation = (getFlag(f) ? TI_FALSE : TI_TRUE);
  fnClearFlag(f);

  refreshRegisterLine(TAM_REGISTER_LINE);
  refreshRegisterLine(TRUE_FALSE_REGISTER_LINE);
}



void fnIsFlagClearSet(uint16_t f) {
  temporaryInformation = (getFlag(f) ? TI_FALSE : TI_TRUE);
  fnSetFlag(f);

  refreshRegisterLine(TAM_REGISTER_LINE);
  refreshRegisterLine(TRUE_FALSE_REGISTER_LINE);
}



void fnIsFlagClearFlip(uint16_t f) {
  temporaryInformation = (getFlag(f) ? TI_FALSE : TI_TRUE);
  fnFlipFlag(f);

  refreshRegisterLine(TAM_REGISTER_LINE);
  refreshRegisterLine(TRUE_FALSE_REGISTER_LINE);
}



void fnIsFlagSet(uint16_t f) {
  temporaryInformation = (getFlag(f) ? TI_TRUE : TI_FALSE);

  refreshRegisterLine(TAM_REGISTER_LINE);
  refreshRegisterLine(TRUE_FALSE_REGISTER_LINE);
}




void fnIsFlagSetClear(uint16_t f) {
  temporaryInformation = (getFlag(f) ? TI_TRUE : TI_FALSE);
  fnClearFlag(f);

  refreshRegisterLine(TAM_REGISTER_LINE);
  refreshRegisterLine(TRUE_FALSE_REGISTER_LINE);
}




void fnIsFlagSetSet(uint16_t f) {
  temporaryInformation = (getFlag(f) ? TI_TRUE : TI_FALSE);
  fnSetFlag(f);

  refreshRegisterLine(TAM_REGISTER_LINE);
  refreshRegisterLine(TRUE_FALSE_REGISTER_LINE);
}




void fnIsFlagSetFlip(uint16_t f) {
  temporaryInformation = (getFlag(f) ? TI_TRUE : TI_FALSE);
  fnFlipFlag(f);

  refreshRegisterLine(TAM_REGISTER_LINE);
  refreshRegisterLine(TRUE_FALSE_REGISTER_LINE);
}
