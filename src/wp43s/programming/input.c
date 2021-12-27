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
 * \file input.c
 ***********************************************/

#include "programming/input.h"
#include "defines.h"
#include "debug.h"
#include "error.h"
#include "items.h"
#include "keyboard.h"
#include "longIntegerType.h"
#include "recall.h"
#include "registerValueConversions.h"
#include "screen.h"
#include "softmenus.h"
#include "stack.h"
#include "timer.h"
#include "ui/tam.h"
#include "wp43s.h"
#ifdef PC_BUILD
  #include <unistd.h>
#endif // PC_BUILD

void fnInput(uint16_t regist) {
  if(programRunStop == PGM_RUNNING) {
    programRunStop = PGM_WAITING;
    currentInputVariable = regist;
    fnRecall(regist);
  }
}



void fnVarMnu(uint16_t label) {
#ifndef TESTSUITE_BUILD
  currentMvarLabel = label;
  showSoftmenu(-MNU_MVAR);
#endif // TESTSUITE_BUILD
}



void fnPause(uint16_t duration) {
#ifndef TESTSUITE_BUILD
  uint8_t previousProgramRunStop = programRunStop;
  if(tam.mode) tamLeaveMode();
  programRunStop = PGM_PAUSED;
  if(previousProgramRunStop != PGM_RUNNING) {
    refreshScreen();
  }
  #ifdef DMCP_BUILD
    lcd_refresh();
    for(uint16_t i = 0; i < duration && (programRunStop == PGM_PAUSED || programRunStop == PGM_KEY_PRESSED_WHILE_PAUSED); ++i) {
      int key = key_pop();
      key = convertKeyCode(key);
      if(key > 0) {
        if((key == 36 || key == 37) && previousProgramRunStop == PGM_RUNNING) {
          previousProgramRunStop = programRunStop = PGM_WAITING;
        }
        setLastKeyCode(key);
        fnTimerStart(TO_KB_ACTV, TO_KB_ACTV, 60000);
        wait_for_key_release(0);
        key_pop();
        break;
      }
      sys_delay(100);
    }
  #else // !DMCP_BUILD
    refreshLcd(NULL);
    for(uint16_t i = 0; i < duration && (programRunStop == PGM_PAUSED || programRunStop == PGM_KEY_PRESSED_WHILE_PAUSED); ++i) {
      if(previousProgramRunStop != PGM_RUNNING) {
        refreshScreen();
        refreshLcd(NULL);
      }
      gtk_main_iteration_do(FALSE);
      usleep(100000);
    }
    if(programRunStop == PGM_WAITING) previousProgramRunStop = PGM_WAITING;
  #endif // DMCP_BUILD
  programRunStop = previousProgramRunStop;
  if(programRunStop != PGM_RUNNING) {
    refreshScreen();
    #ifdef DMCP_BUILD
      lcd_refresh();
    #else // !DMCP_BUILD
      refreshLcd(NULL);
    #endif // DMCP_BUILD
  }
#endif // TESTSUITE_BUILD
}



void fnKey(uint16_t regist) {
  // no key was pressed
  if(lastKeyCode == 0) {
    temporaryInformation = TI_TRUE;
  }

  // a key was pressed
  else {
    temporaryInformation = TI_FALSE;
    if(regist <= LAST_NAMED_VARIABLE) {
      longInteger_t kc;
      longIntegerInit(kc);
      uIntToLongInteger(lastKeyCode, kc);
      convertLongIntegerToLongIntegerRegister(kc, regist);
      longIntegerFree(kc);
      lastKeyCode = 0;
    }
    else {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "register %u is out of range", regist);
        moreInfoOnError("In function fnKey:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }
}



void fnKeyType(uint16_t keyCode) {
  longInteger_t kt;
  longIntegerInit(kt);
  switch(keyCode) {
    case 82: uIntToLongInteger( 0, kt); break;
    case 72: uIntToLongInteger( 1, kt); break;
    case 73: uIntToLongInteger( 2, kt); break;
    case 74: uIntToLongInteger( 3, kt); break;
    case 62: uIntToLongInteger( 4, kt); break;
    case 63: uIntToLongInteger( 5, kt); break;
    case 64: uIntToLongInteger( 6, kt); break;
    case 52: uIntToLongInteger( 7, kt); break;
    case 53: uIntToLongInteger( 8, kt); break;
    case 54: uIntToLongInteger( 9, kt); break;

    case 44:
    case 45:
    case 83: uIntToLongInteger(10, kt); break;

    case 35:
    case 36: uIntToLongInteger(11, kt); break;

    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16: uIntToLongInteger(12, kt); break;

    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 31:
    case 32:
    case 33:
    case 34:
    case 41:
    case 43:
    case 46:
    case 51:
    case 55:
    case 61:
    case 65:
    case 71:
    case 75:
    case 81:
    case 84:
    case 85: uIntToLongInteger(13, kt); break;

    default:
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "keycode %u is out of range", keyCode);
        moreInfoOnError("In function fnKeyType:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      longIntegerFree(kt);
      return;
  }
  liftStack();
  convertLongIntegerToLongIntegerRegister(kt, REGISTER_X);
  longIntegerFree(kt);
}
