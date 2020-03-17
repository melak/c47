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
 * \file mask.c
 ***********************************************/

#include "wp43s.h"



/********************************************//**
 * \brief MASKL n ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnMaskl(uint16_t numberOfBits) {
  if(numberOfBits > shortIntegerWordSize) {
    displayCalcErrorMessage(ERROR_WORD_SIZE_TOO_SMALL, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot calculate MASKL(%d) word size is %d", numberOfBits, shortIntegerWordSize);
      showInfoDialog("In function fnMaskl:", errorMessage, NULL, NULL);
    #endif
  }
  else {
    uint64_t mask;

    saveStack();
    liftStack();

    if(numberOfBits == 0) {
      mask = 0;
    }
    else {
      mask = (((1ULL << numberOfBits) - 1) & shortIntegerMask) << (shortIntegerWordSize - numberOfBits);
    }

    reallocateRegister(REGISTER_X, dtShortInteger, SHORT_INTEGER_SIZE, 2);
    *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = mask;

    refreshStack();
  }
}



/********************************************//**
 * \brief MASKR n ==> regX
 * enables stack lift and refreshes the stack
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnMaskr(uint16_t numberOfBits) {
  if(numberOfBits > shortIntegerWordSize) {
    displayCalcErrorMessage(ERROR_WORD_SIZE_TOO_SMALL, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "cannot calculate MASKR(%d) word size is %d", numberOfBits, shortIntegerWordSize);
      showInfoDialog("In function fnMaskr:", errorMessage, NULL, NULL);
    #endif
  }
  else {
    uint64_t mask;

    saveStack();
    liftStack();

    if(numberOfBits == 0) {
      mask = 0;
    }
    else {
      mask = (1ULL << numberOfBits) - 1;
    }

    reallocateRegister(REGISTER_X, dtShortInteger, SHORT_INTEGER_SIZE, 2);
    *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = mask;

    refreshStack();
  }
}
