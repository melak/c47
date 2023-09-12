// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: Copyright The WP43 and C47 Authors

#include "calcMode.h"

#include "bufferize.h"
#include "debug.h"
#include "error.h"
#include "flags.h"
#include "hal/gui.h"
#include "items.h"
#include "c43Extensions/jm.h"
#include "keyboard.h"
#include "mathematics/matrix.h"
#include "registers.h"
#include "saveRestoreCalcState.h"
#include "screen.h"
#include "softmenus.h"
#include "stack.h"
#include "timer.h"
#include <string.h>
#include <unistd.h>

#include "wp43.h"

#if !defined(TESTSUITE_BUILD)
  void fnOff(uint16_t unusedParamButMandatory) {
    shiftF = false;
    shiftG = false;

    fnStopTimerApp();

    #if defined(PC_BUILD)
      if(matrixIndex != INVALID_VARIABLE) {
        if(getRegisterDataType(matrixIndex) == dtReal34Matrix) {
          if(openMatrixMIMPointer.realMatrix.matrixElements) {
          realMatrixFree(&openMatrixMIMPointer.realMatrix);
        }
        }
        else if(getRegisterDataType(matrixIndex) == dtComplex34Matrix) {
          if(openMatrixMIMPointer.complexMatrix.matrixElements) {
          complexMatrixFree(&openMatrixMIMPointer.complexMatrix);
        }
      }
      }
      saveCalc();
      gtk_main_quit();
    #endif // PC_BUILD

    #if defined(DMCP_BUILD)
      SET_ST(STAT_PGM_END);
    #endif // DMCP_BUILD
  }



  void calcModeNormal(void) {
    #if defined(PC_BUILD)
      char tmp[200]; sprintf(tmp,"^^^^### calcModeNormal"); jm_show_comment(tmp);
    #endif // PC_BUILD
    calcMode = CM_NORMAL;

    if(softmenuStack[0].softmenuId == 1) { // MyAlpha
      softmenuStack[0].softmenuId = 0; // MyMenu
    }

    clearSystemFlag(FLAG_ALPHA);
    hideCursor();
    cursorEnabled = false;

    #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
      calcModeNormalGui();
    #endif // PC_BUILD && (SCREEN_800X480 == 0)
  }



  void calcModeAim(uint16_t unusedButMandatoryParameter) {
    #if defined(PC_BUILD)
      char tmp[200]; sprintf(tmp,"^^^^### calcModeAim"); jm_show_comment(tmp);
    #endif // PC_BUILD

    if(!tam.mode) {
            showSoftmenu(-MNU_ALPHA);        //JM ALPHA-HOME  Change to initialize the menu stack. it was true.
    }

    alphaCase = AC_UPPER;
    nextChar = NC_NORMAL;
    numLock = false;
    scrLock = NC_NORMAL;

    if(!tam.mode && calcMode != CM_ASSIGN && calcMode != CM_PEM) {
      calcMode = CM_AIM;
      liftStack();

      clearRegisterLine(AIM_REGISTER_LINE, true, true);
      xCursor = 1;
      yCursor = Y_POSITION_OF_AIM_LINE + 6;
      cursorFont = &standardFont;
      cursorEnabled = true;
    }

    if(softmenuStack[0].softmenuId == 0) { // MyMenu
      softmenuStack[0].softmenuId = 1; // MyAlpha
    }

    setSystemFlag(FLAG_ALPHA);

    #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
      calcModeAimGui();
    #endif // PC_BUILD && (SCREEN_800X480 == 0)
  }



  void enterAsmModeIfMenuIsACatalog(int16_t id) {
    switch(-id) {
      case MNU_FCNS: {
        catalog = CATALOG_FCNS;
        break;
      }
      case MNU_CONST: {
        catalog = CATALOG_CNST;
        break;
      }
      case MNU_MENUS: {
        catalog = CATALOG_MENU;
        break;
      }
      case MNU_SYSFL: {
        catalog = CATALOG_SYFL;
        break;
      }
      case MNU_ALPHAINTL: {
        catalog = CATALOG_AINT;
        break;
      }
      case MNU_ALPHAintl: {
        catalog = CATALOG_aint;
        break;
      }
      case MNU_PROG:
      case MNU_PROGS: {
        catalog = CATALOG_PROG;
        break;
      }
      case MNU_VAR: {
        catalog = CATALOG_VAR;
        break;
      }
      case MNU_MATRS: {
        catalog = CATALOG_MATRS;
        break;
      }
      case MNU_STRINGS: {
        catalog = CATALOG_STRINGS;
        break;
      }
      case MNU_DATES: {
        catalog = CATALOG_DATES;
        break;
      }
      case MNU_TIMES: {
        catalog = CATALOG_TIMES;
        break;
      }
      case MNU_ANGLES: {
        catalog = CATALOG_ANGLES;
        break;
      }
      case MNU_SINTS: {
        catalog = CATALOG_SINTS;
        break;
      }
      case MNU_LINTS: {
        catalog = CATALOG_LINTS;
        break;
      }
      case MNU_REALS: {
        catalog = CATALOG_REALS;
        break;
      }
      case MNU_CPXS: {
        catalog = CATALOG_CPXS;
        break;
      }
      case MNU_Solver:
      case MNU_Sf:
      case MNU_1STDERIV:
      case MNU_2NDDERIV:
      case MNU_MVAR: {
        catalog = CATALOG_MVAR;
        break;
      }
      default: {
        catalog = CATALOG_NONE;
      }
    }
    #if defined(PC_BUILD)
      char tmp[200]; sprintf(tmp,"^^^^### enterAsmMode catalog=%d",catalog); jm_show_comment(tmp);
    #endif // PC_BUILD

    if(catalog) {
      if(calcMode == CM_NIM) {
        closeNim();
      }
      if(calcMode != CM_PEM || !getSystemFlag(FLAG_ALPHA)) {
        if(calcMode != CM_AIM && calcMode != CM_EIM) {
          alphaCase = AC_UPPER;
          nextChar = NC_NORMAL;
          numLock = false;
          scrLock = NC_NORMAL;
        }


        clearSystemFlag(FLAG_ALPHA);
        resetAlphaSelectionBuffer();

        #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
          if(catalog != CATALOG_MVAR) {
            calcModeAimGui();
          }
        #endif // PC_BUILD && (SCREEN_800X480 == 0)
      }
    }
  }



  void leaveAsmMode(void) {
    catalog = CATALOG_NONE;

    #if defined(PC_BUILD) && (SCREEN_800X480 == 0)
      if(tam.mode && !tam.alpha) {
        calcModeTamGui();
      }
      else if(calcMode == CM_AIM || (tam.mode && tam.alpha)) {
        calcModeAimGui();
      }
      else if(calcMode == CM_NORMAL || calcMode == CM_PEM || calcMode == CM_MIM || calcMode == CM_ASSIGN) {
        calcModeNormalGui();
      }
    #endif // PC_BUILD && (SCREEN_800X480 == 0)
  }



  void calcModeNim(uint16_t unusedButMandatoryParameter) {
    #if defined(DEBUGUNDO)
      printf(">>> saveForUndo from gui: calcModeNim\n");
    #endif // DEBUGUNDO
    #if defined(PC_BUILD)
      char tmp[200]; sprintf(tmp,"^^^^### calcModeNim"); jm_show_comment(tmp);
    #endif // PC_BUILD
    saveForUndo();
    if(lastErrorCode == ERROR_RAM_FULL) {
      displayCalcErrorMessage(ERROR_RAM_FULL, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
      #if(EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function calcModeNim:", "there is not enough memory to save for undo!", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }

    calcMode = CM_NIM;
    clearSystemFlag(FLAG_ALPHA);

    liftStack();
    real34Zero(REGISTER_REAL34_DATA(REGISTER_X));

    aimBuffer[0] = 0;
    hexDigits = 0;

    if(!checkHP) clearRegisterLine(NIM_REGISTER_LINE, true, true);
    xCursor = 1;
    cursorEnabled = true;
    cursorFont = &numericFont;
  }
#endif // !TESTSUITE_BUILD
