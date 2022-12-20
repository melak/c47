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
    bool_t finished = false;
    uint32_t tmpi, loopFff, loopNn;
    longInteger_t iCounter;
    longIntegerInit(iCounter);
    longInteger_t iLoopFff;
    longIntegerInit(iLoopFff);
    real34_t counter, tmp2;
    real_t resultX, resultR;

    fnToReal(NOPARAM);                                                                  // Convert Counter to Real ccccc.00000
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &counter);                             //       ccccc.fffii
    convertReal34ToLongInteger(REGISTER_REAL34_DATA(REGISTER_X), iCounter, DEC_ROUND_DOWN);//    ccccc
                                                                                        // loop control number in the form ccccc.fffii, with 
                                                                                        // ccccc being the current counter value, 
                                                                                        // fff the final counter value, 
                                                                                        // ii the DECREMENT size (default is 1).
    real34ToIntegralValue(REGISTER_REAL34_DATA(REGISTER_X), &tmp2, DEC_ROUND_DOWN);     //       ccccc
    real34Subtract(REGISTER_REAL34_DATA(REGISTER_X), &tmp2, &tmp2);                     //           0.fffii
    real34Multiply(&tmp2, const34_1e6, &tmp2);                                          //      fffii0.
    tmpi = (uint32_t)(real34ToUInt32(&tmp2) / 10);                                      //       fffii
    loopFff = (uint16_t)(tmpi / 100);                                                   //         fff
    loopNn  = (uint16_t)(tmpi - (loopFff * 100));                                       //          ii
    if(loopNn == 0) {
      loopNn = 1;
    }

    uIntToLongInteger(loopFff, iLoopFff);

    realCopy(prod ? const_1 : const_0, &resultR);                                       // Initialize real accumulator

    ++currentSolverNestingDepth;
    setSystemFlag(FLAG_SOLVING);


    while(1) {

        hourGlassIconEnabled = true;
        showHideHourGlass();

        convertLongIntegerToLongIntegerRegister(iCounter, REGISTER_X);
        finished = longIntegerCompare(iCounter, iLoopFff) <= 0;                    //Prepare the finished flag if it is entered with zero
        #if defined(VERBOSE_COUNTER)
          printLongIntegerToConsole(iCounter," iCounter: "," ");
          printLongIntegerToConsole(iLoopFff," iLoopFff: "," ");
        #endif //VERBOSE_COUNTER

        fnToReal(NOPARAM);
        if(lastErrorCode != ERROR_NONE) {
          break;
        }
        fnFillStack(NOPARAM);
        dynamicMenuItem = -1;
        execProgram(label);
        if(lastErrorCode != ERROR_NONE) {
          break;
        }
        fnToReal(NOPARAM);


        if(lastErrorCode != ERROR_NONE) {
          break;
        }
        if(prod) {
          real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &resultX);
          realMultiply(&resultR, &resultX, &resultR, &ctxtReal75);
        }
        else {
          real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &resultX);
          realAdd(&resultR, &resultX, &resultR, &ctxtReal75);
        }

        #if defined(VERBOSE_COUNTER)
          printRealToConsole(&resultX," X: ", " ");
          printRealToConsole(&resultR," X: ", "\n");
        #endif //VERBOSE_COUNTER


        #if defined(DMCP_BUILD)
          if(keyWaiting()) {
              showString("key Waiting ...", &standardFont, 20, 40, vmNormal, false, false);
            break;
          }
        #endif //DMCP_BUILD

        if(finished) {
          break;
        }

        longIntegerSubtractUInt(iCounter, loopNn, iCounter);
    }


    if(lastErrorCode == ERROR_NONE) {
        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
        realToReal34(&resultR, REGISTER_REAL34_DATA(REGISTER_X));
    }

    temporaryInformation = TI_NO_INFO;
    if(programRunStop == PGM_WAITING) {
      programRunStop = PGM_STOPPED;
    }
    adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);

    if((--currentSolverNestingDepth) == 0) {
      clearSystemFlag(FLAG_SOLVING);
    }

    
    longIntegerFree(iLoopFff);
    longIntegerFree(iCounter);


    hourGlassIconEnabled = false;
    showHideHourGlass();
  }



  static void _checkArgument(uint16_t label, bool_t prod) {
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
          moreInfoOnError("In function _checkArgument:", errorMessage, NULL, NULL);
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
        moreInfoOnError("In function _checkArgument:", errorMessage, NULL, NULL);
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
