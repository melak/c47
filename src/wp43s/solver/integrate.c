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
 * \file integrate.c
 ***********************************************/

#include "solver/integrate.h"

#include "defines.h"
#include "error.h"
#include "items.h"
#include "programming/manage.h"
#include "registers.h"
#include "softmenus.h"
#include "wp43s.h"

void fnPgmInt(uint16_t label) {
  if(label >= FIRST_LABEL && label <= LAST_LABEL) {
    currentSolverProgram = label - FIRST_LABEL;
  }
  else if(label >= REGISTER_X && label <= REGISTER_T) {
    // Interactive mode
    char buf[4];
    switch(label) {
      case REGISTER_X:        buf[0] = 'X'; break;
      case REGISTER_Y:        buf[0] = 'Y'; break;
      case REGISTER_Z:        buf[0] = 'Z'; break;
      case REGISTER_T:        buf[0] = 'T'; break;
      default: /* unlikely */ buf[0] = 0;
    }
    buf[1] = 0;
    label = findNamedLabel(buf);
    if(label == INVALID_VARIABLE) {
      displayCalcErrorMessage(ERROR_LABEL_NOT_FOUND, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "string '%s' is not a named label", buf);
        moreInfoOnError("In function fnPgmInt:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      currentSolverProgram = label - FIRST_LABEL;
    }
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "unexpected parameter %u", label);
      moreInfoOnError("In function fnPgmInt:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}

void fnIntegrate(uint16_t labelOrVariable) {
  if((labelOrVariable >= FIRST_LABEL && labelOrVariable <= LAST_LABEL) || (labelOrVariable >= REGISTER_X && labelOrVariable <= REGISTER_T)) {
    // Interactive mode
    fnPgmInt(labelOrVariable);
    if(lastErrorCode == ERROR_NONE)
      currentSolverStatus = SOLVER_STATUS_INTERACTIVE | SOLVER_STATUS_INTEGRATING;
  }
  else if(labelOrVariable == RESERVED_VARIABLE_ACC || labelOrVariable == RESERVED_VARIABLE_ULIM || labelOrVariable == RESERVED_VARIABLE_LLIM) {
    fnToReal(NOPARAM);
    if(lastErrorCode == ERROR_NONE) {
      reallocateRegister(labelOrVariable, dtReal34, REAL34_SIZE, amNone);
      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(labelOrVariable));
      switch(labelOrVariable) {
        case RESERVED_VARIABLE_ACC:  temporaryInformation = TI_ACC;  break;
        case RESERVED_VARIABLE_ULIM: temporaryInformation = TI_ULIM; break;
        case RESERVED_VARIABLE_LLIM: temporaryInformation = TI_LLIM; break;
      }
    }
  }
  else if(labelOrVariable >= FIRST_NAMED_VARIABLE && labelOrVariable <= LAST_NAMED_VARIABLE) {
    // Execute
    // mockup
#ifndef TESTSUITE_BUILD
    printf("Integrator to be coded\n"); fflush(stdout);
#endif /* TESTSUITE_BUILD */
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "unexpected parameter %u", labelOrVariable);
      moreInfoOnError("In function fnIntegrate:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}

void fnIntVar(uint16_t unusedButMandatoryParameter) {
#ifndef TESTSUITE_BUILD
  const char *var = (char *)getNthString(dynamicSoftmenu[softmenuStack[0].softmenuId].menuContent, dynamicMenuItem);
  const uint16_t regist = findOrAllocateNamedVariable(var);
  currentSolverVariable = regist;
  if(currentSolverStatus & SOLVER_STATUS_READY_TO_EXECUTE) {
    showSoftmenu(-MNU_Sfdx);
  }
  else {
    reallyRunFunction(ITM_STO, regist);
    currentSolverStatus |= SOLVER_STATUS_READY_TO_EXECUTE;
    temporaryInformation = TI_SOLVER_VARIABLE;
  }
#endif /* TESTSUITE_BUILD */
}
