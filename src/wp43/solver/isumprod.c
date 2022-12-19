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
 * \file isumprod.c
 ***********************************************/

#include "solver/isumprod.h"

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
  static void _programmableiSumProd(uint16_t label, bool_t prod) {
    longInteger_t resultLi, xLi;
    longInteger_t paramX, paramY, paramZ, iCounter;
    longIntegerInit(paramX);
    longIntegerInit(paramY);
    longIntegerInit(paramZ);
    convertLongIntegerRegisterToLongInteger(REGISTER_Z, paramZ); //From
    convertLongIntegerRegisterToLongInteger(REGISTER_Y, paramY); //To
    convertLongIntegerRegisterToLongInteger(REGISTER_X, paramX); //Step
    longIntegerInit(iCounter);
    convertLongIntegerRegisterToLongInteger(REGISTER_Z, iCounter); //Counter
    longIntegerInit(xLi);
    //uIntToLongInteger(0, xLi);
    longIntegerInit(resultLi);
    uIntToLongInteger(prod ? 1 : 0, resultLi);                             //Initialize long integer accumulator
    bool_t finished = false;
    
    longIntegerSubtract(paramY, paramZ, xLi);
    longIntegerModulo(xLi, paramX, xLi);
    //printLongIntegerToConsole(xLi,"MOD: "," \n");


    if((longIntegerCompare(paramY, paramZ) > 0 && longIntegerCompareUInt(paramX, 0) <=0) || (longIntegerCompare(paramY, paramZ) < 0 && longIntegerCompareUInt(paramX, 0) >=0)) {
      displayCalcErrorMessage(ERROR_BAD_INPUT, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "Counter will not count to destination");
        moreInfoOnError("In function _programmableiSumProd:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    } else
    if(!longIntegerIsZero(xLi)) {
      displayCalcErrorMessage(ERROR_BAD_INPUT, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "Destination cannot be reached");
        moreInfoOnError("In function _programmableiSumProd:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    } else {

      ++currentSolverNestingDepth;
      setSystemFlag(FLAG_SOLVING);


      while(lastErrorCode == ERROR_NONE) {
        hourGlassIconEnabled = true;
        showHideHourGlass();

        finished = longIntegerCompare(iCounter, paramY) == 0;

        convertLongIntegerToLongIntegerRegister(iCounter, REGISTER_X);
        fnFillStack(NOPARAM);

        //printRegisterToConsole(REGISTER_X,"INPUT: "," - ");
        dynamicMenuItem = -1;
        execProgram(label);
        if(lastErrorCode != ERROR_NONE) {
          break;
        }
        //printRegisterToConsole(REGISTER_X,"OUTPUT: "," = ");

        if(getRegisterDataType(REGISTER_X) != dtLongInteger) {
          lastErrorCode = ERROR_INVALID_DATA_TYPE_FOR_OP;
          break;
        }


        convertLongIntegerRegisterToLongInteger(REGISTER_X, xLi); //Result
        if(prod) {
          longIntegerMultiply(resultLi, xLi, resultLi);
        }
        else {
          longIntegerAdd(resultLi, xLi, resultLi);
        }
        //printLongIntegerToConsole(resultLi,"OUTPUT: "," \n");
        longIntegerAdd(iCounter, paramX, iCounter);
              

        #if defined(DMCP_BUILD)
          if(keyWaiting()) {
              showString("key Waiting ...", &standardFont, 20, 40, vmNormal, false, false);
            break;
          }
        #endif //DMCP_BUILD

        if(finished) {
          break;
        }
      }

      if(lastErrorCode == ERROR_NONE) {
        convertLongIntegerToLongIntegerRegister(resultLi, REGISTER_X);
      } else {
        displayCalcErrorMessage(lastErrorCode, ERR_REGISTER_LINE, REGISTER_X);
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "Error while calculating");
          moreInfoOnError("In function _programmableiSumProd:", errorMessage, NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)

      }


      longIntegerFree(paramX);
      longIntegerFree(paramY);
      longIntegerFree(paramZ);
      longIntegerFree(iCounter);
      longIntegerInit(xLi);
      longIntegerFree(resultLi);



      temporaryInformation = TI_NO_INFO;
      if(programRunStop == PGM_WAITING) {
        programRunStop = PGM_STOPPED;
      }
      adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
    }

    if((--currentSolverNestingDepth) == 0) {
      clearSystemFlag(FLAG_SOLVING);
    }

    hourGlassIconEnabled = false;
    showHideHourGlass();
  }


static bool_t _checkRegisters(void) {
  if(getRegisterDataType(REGISTER_X) != dtLongInteger) {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Long integer expected");
      moreInfoOnError("In function _checkiArgument:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return true;
  } else if(getRegisterDataType(REGISTER_Y) != dtLongInteger) {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_Y);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Long integer expected");
      moreInfoOnError("In function _checkiArgument:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return true;
  } else if(getRegisterDataType(REGISTER_Z) != dtLongInteger) {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_Z);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "Long integer expected");
      moreInfoOnError("In function _checkiArgument:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    return true;
  }
  return false;
}


static  void _checkiArgument(uint16_t label, bool_t prod) {
    if(label >= FIRST_LABEL && label <= LAST_LABEL) {
      if(!_checkRegisters()) {
        _programmableiSumProd(label, prod);
      }
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
          moreInfoOnError("In function _checkiArgument:", errorMessage, NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      } else {
        if(!_checkRegisters()) {
          _programmableiSumProd(label, prod);
        }
      }

    }
    else {
      displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "unexpected parameter %u", label);
        moreInfoOnError("In function _checkiArgument:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }
#endif // !TESTSUITE_BUILD

void fnProgrammableiSum(uint16_t label) {
  #if !defined(TESTSUITE_BUILD)
    _checkiArgument(label, false);
  #endif // !TESTSUITE_BUILD
}

void fnProgrammableiProduct(uint16_t label) {
  #if !defined(TESTSUITE_BUILD)
    _checkiArgument(label, true);
  #endif // !TESTSUITE_BUILD
}
