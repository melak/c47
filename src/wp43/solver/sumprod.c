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
#include "mathematics/comparisonReals.h"
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
    uint32_t      loop = 0;
    int16_t       finished = 0;
    real_t        resultX, resultR;
    real34_t      loopStep, loopTo, counter, compare, sign, rLoop;
    longInteger_t iLoop;
    fnToReal(NOPARAM);
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &loopStep);
    fnDrop(NOPARAM);
    fnToReal(NOPARAM);
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &loopTo);
    fnDrop(NOPARAM);
    fnToReal(NOPARAM);
    real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &counter); //Loopfrom
    realCopy(prod ? const_1 : const_0, &resultR);           //Initialize real accumulator

    
    real34Subtract(&loopTo, &counter, &rLoop);              //calculate the remaining iteration counter
    real34Divide(&rLoop, &loopStep, &rLoop);
    longIntegerInit(iLoop);
    convertReal34ToLongInteger(&rLoop, iLoop, DEC_ROUND_DOWN);
    loop = longIntegerModuloUInt(iLoop, 100000);
    longIntegerFree(iLoop);

    if(real34IsZero(&loopStep) || (real34CompareGreaterThan(&loopTo, &counter) && real34CompareLessEqual(&loopStep, const34_0)) || (real34CompareLessThan(&loopTo, &counter) && real34CompareGreaterEqual(&loopStep, const34_0))) {
      displayCalcErrorMessage(ERROR_BAD_INPUT, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "Counter will not count to destination");
        moreInfoOnError("In function _programmableiSumProd:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    } else {

      ++currentSolverNestingDepth;
      setSystemFlag(FLAG_SOLVING);


      while(lastErrorCode == ERROR_NONE) {
          hourGlassIconEnabled = true;
          showHideHourGlass();

          real34Compare(&counter, &loopTo, &compare);
          real34Compare(&loopStep, const34_0, &sign);
          real34Multiply(&compare, &sign, &compare);
          finished = real34ToInt32(&compare);                       //0 means equal
          if(finished > 0) {
            break;
          }
          real34Copy(&counter, REGISTER_REAL34_DATA(REGISTER_X));
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


          real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &resultX); //Result accumulated
          if(prod) {
            realMultiply(&resultR, &resultX, &resultR, &ctxtReal75);
          }
          else {
            realAdd(&resultR, &resultX, &resultR, &ctxtReal75);
          }

          #if defined(VERBOSE_COUNTER)
            printf(">>> Finished: %d ", finished);
            printReal34ToConsole(&counter," Cnt: ", " ");
            printRealToConsole(&resultX," X: ", " ");
            printRealToConsole(&resultR," SUM: ", "\n");
          #endif //VERBOSE_COUNTER

          real34Add(&counter, &loopStep, &counter);
          printHalfSecUpdate_Integer(timed, "Loop: ",loop--); //timed

          #if defined(DMCP_BUILD)
            if(keyWaiting()) {
                showString("key Waiting ...", &standardFont, 20, 40, vmNormal, false, false);
                printHalfSecUpdate_Integer(force+1, "Interrupted: ",loop);
              break;
            }
          #endif //DMCP_BUILD

          if(finished == 0) {
            break;
          }
      } //WHILE







      if(lastErrorCode == ERROR_NONE) {
          reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
          convertRealToReal34ResultRegister(&resultR, REGISTER_X);
          //realToReal34(&resultR, REGISTER_REAL34_DATA(REGISTER_X));
      } else {
        displayCalcErrorMessage(lastErrorCode, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "Error while calculating");
          moreInfoOnError("In function _programmableSumProd:", errorMessage, NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      }

      temporaryInformation = TI_NO_INFO;
      if(programRunStop == PGM_WAITING) {
        programRunStop = PGM_STOPPED;
      }
      adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
    } //MAIN IF
    if((--currentSolverNestingDepth) == 0) {
      clearSystemFlag(FLAG_SOLVING);
    }
    hourGlassIconEnabled = false;
    showHideHourGlass();

    printHalfSecUpdate_Integer(force+0, "Loop complete: ",loop);
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
