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
 * \file sumprod.c
 ***********************************************/

#include "solver/sumprod.h"

#include "c43Extensions/addons.h"
#include "constantPointers.h"
#include "defines.h"
#include "error.h"
#include "flags.h"
#include "integers.h"
#include "items.h"
#include "mathematics/integerPart.h"
#include "mathematics/iteration.h"
#include "programming/lblGtoXeq.h"
#include "programming/manage.h"
#include "programming/nextStep.h"
#include "realType.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "screen.h"
#include "solver/solve.h"
#include "statusBar.h"
#include "stack.h"
#include "timer.h"
#include "wp43.h"

#if !defined(TESTSUITE_BUILD)
  static void _programmableSumProd(uint16_t label, bool_t prod) {
    real34_t counter;
    real_t result;
    longInteger_t resultLi, xLi;
    bool_t finished = false;
    bool_t isInteger = getRegisterDataType(REGISTER_X) == dtLongInteger;   //Set processing to real, if counter is not long integer. A way to force a real result.

    real34Copy(prod ? const34_1 : const34_0, &result);                     //Initialize real accumulator
    longIntegerInit(resultLi);
    longIntegerInit(xLi);
    uIntToLongInteger(prod ? 1 : 0, resultLi);                             //Initialize long integer accumulator
    uIntToLongInteger(0, xLi);


    ++currentSolverNestingDepth;
    setSystemFlag(FLAG_SOLVING);

    while(1) {
      hourGlassIconEnabled = true;
      showHideHourGlass();

      fnToReal(NOPARAM);                                                   //Force counter in X to real. I don't think the counter will change type after the loop started, so this could be moved to just before the loop start
      if(lastErrorCode != ERROR_NONE) {
        break;
      }
      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &counter);
      fnIp(NOPARAM);
      finished = longIntegerIsZeroRegister(REGISTER_X);                    //Prepare the finished flag if it is entered with zero
      fnFillStack(NOPARAM);

      dynamicMenuItem = -1;
      execProgram(label);
      if(lastErrorCode != ERROR_NONE) {
        break;
      }

      if(isInteger && getRegisterDataType(REGISTER_X) == dtReal34) {       //Latch method over to Real once any Real is calculated by the function
        isInteger = false;
        real_t xReal;
        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
        convertLongIntegerToReal(resultLi, &xReal, &ctxtReal75);
        realToReal34(&xReal, &result);
      }

      if(!isInteger) {
        fnToReal(NOPARAM);
        if(lastErrorCode != ERROR_NONE) {
          break;
        }
        if(prod) {
          real34Multiply(REGISTER_REAL34_DATA(REGISTER_X), &result, &result);
        }
        else {
          real34Add(REGISTER_REAL34_DATA(REGISTER_X), &result, &result);
        }
      } else {
        if(prod) {
          convertLongIntegerRegisterToLongInteger(REGISTER_X, xLi);
          longIntegerMultiply(resultLi, xLi, resultLi);
        }
        else {
          convertLongIntegerRegisterToLongInteger(REGISTER_X, xLi);
          longIntegerAdd(resultLi, xLi, resultLi);
        }
      }
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      real34Copy(&counter, REGISTER_REAL34_DATA(REGISTER_X));

      #if defined(DMCP_BUILD)
        if(keyWaiting()) {
            showString("key Waiting ...", &standardFont, 20, 40, vmNormal, false, false);
          break;
        }
      #endif //DMCP_BUILD

      if(finished) {
        break;
      }
      fnDse(REGISTER_X);
      finished = (temporaryInformation != TI_TRUE);
    }

    if(lastErrorCode == ERROR_NONE) {
      if(!isInteger) {
        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
        real34Copy(&result, REGISTER_REAL34_DATA(REGISTER_X));
      } else {
        convertLongIntegerToLongIntegerRegister(resultLi, REGISTER_X);
      }
    }

    longIntegerFree(resultLi);
    longIntegerFree(xLi);

    temporaryInformation = TI_NO_INFO;
    if(programRunStop == PGM_WAITING) {
      programRunStop = PGM_STOPPED;
    }
    adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);

    if((--currentSolverNestingDepth) == 0) {
      clearSystemFlag(FLAG_SOLVING);
    }

    hourGlassIconEnabled = false;
    showHideHourGlass();
  }

  void _checkArgument(uint16_t label, bool_t prod) {
    if(label >= FIRST_LABEL && label <= LAST_LABEL) {
      _programmableSumProd(label, prod);
    }
    else if(label >= REGISTER_X && label <= REGISTER_T) {
      // Interactive mode
      char buf[4];
      switch(label) {
        case REGISTER_X: {
          buf[0] = 'X';
          break;
        }
        case REGISTER_Y: {
          buf[0] = 'Y';
          break;
        }
        case REGISTER_Z: {
          buf[0] = 'Z';
          break;
        }
        case REGISTER_T: {
          buf[0] = 'T';
          break;
        }
        default: { /* unlikely */
          buf[0] = 0;
        }
      }
      buf[1] = 0;
      label = findNamedLabel(buf);
      if(label == INVALID_VARIABLE) {
        displayCalcErrorMessage(ERROR_LABEL_NOT_FOUND, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "string '%s' is not a named label", buf);
          moreInfoOnError("In function fnPgmSlv:", errorMessage, NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      }
      else {
        _programmableSumProd(label, prod);
      }
    }
    else {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "unexpected parameter %u", label);
        moreInfoOnError("In function fnPgmSlv:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }
#endif // !TESTSUITE_BUILD

void fnProgrammableSum(uint16_t label) {
  #if !defined(TESTSUITE_BUILD)
    _checkArgument(label, false);
  #endif // !TESTSUITE_BUILD
}

void fnProgrammableProduct(uint16_t label) {
  #if !defined(TESTSUITE_BUILD)
    _checkArgument(label, true);
  #endif // !TESTSUITE_BUILD
}
