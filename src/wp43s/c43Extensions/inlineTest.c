/* This file is part of 43C.
 *
 * 43C is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * 43C is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 43C.  If not, see <http://www.gnu.org/licenses/>.
 */

/********************************************//**
 * \file inlineTest.c
 ***********************************************/

#include "c43Extensions/inlineTest.h"

#include "display.h"
#include "fonts.h"
#include "c43Extensions/jm.h"
#include "c43Extensions/radioButtonCatalog.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "screen.h"
#include "stack.h"
#include <string.h>

#include "wp43s.h"


#ifdef INLINE_TEST
uint32_t swStart[4];
uint32_t swStop[4];
/********************************************//**
 * \brief Start StopWatch
 *
 * \param[in] void
 * \return void
 ***********************************************/
void fnSwStart(uint8_t nr) {
#ifdef DMCP_BUILD
  swStart[nr] = sys_current_ms();
#endif
#ifdef PC_BUILD
  swStart[nr] = g_get_monotonic_time();
#endif
}





/** integer to string
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
char* itoa(int value, char* result, int base) {      //DONE
    // check that the base if valid
    if (base < 2 || base > 16) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}




/********************************************//**
 * \brief Stop StopWatch
 *
 * \param[in] void
 * \return void
 ***********************************************/
void fnSwStop(uint8_t nr) {
#ifdef DMCP_BUILD
  swStop[nr] = sys_current_ms();
#endif
#ifdef PC_BUILD
  swStop[nr] = g_get_monotonic_time();
#endif
#ifndef TESTSUITE_BUILD
  uint32_t swTime = swStop[nr] - swStart[nr];
  char snum[50];
#endif
#ifdef DMCP_BUILD
  showString("ms:", &standardFont, 30, 40 +nr*20, vmNormal, false, false);
#endif
#ifdef PC_BUILD
  showString(STD_mu "s:", &standardFont, 30, 40 +nr*20, vmNormal, false, false);
#endif
#ifndef TESTSUITE_BUILD
  itoa(nr, snum, 10);
  showString(snum, &standardFont, 20, 40 +nr*20, vmNormal, false, false);
  itoa(swTime, snum, 10);
  strcat(snum, "         ");
  showString(snum, &standardFont, 60, 40 +nr*20, vmNormal, false, false);
#endif
}



/********************************************//**
 * \brief Sets/resets flag
 *
 * \param[in] drConfig uint16_t
 * \return void
 ***********************************************/
void fnSetInlineTest(uint16_t drConfig) {
  switch(drConfig) {
  case JC_ITM_TST:
    testEnabled = !testEnabled;
    fnRefreshState();                                       //jm
    break;

  default:
    break;
  }
}



/********************************************//**
 * \brief Get value of testBitset to X
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnGetInlineTestBsToX(uint16_t unusedButMandatoryParameter) {
  char snum[10];
  longInteger_t mem;
  longIntegerInit(mem);
  liftStack();
  
  itoa(testBitset, snum, 10);
  stringToLongInteger(snum,10,mem);

  convertLongIntegerToLongIntegerRegister(mem, REGISTER_X);
  longIntegerFree(mem);

//refreshStack(); --
}



/********************************************//**
 * \brief Set X to testBitset value
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnSetInlineTestXToBs(uint16_t unusedButMandatoryParameter) {
  uint16_t X_REG;
  longInteger_t lgInt;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToLongInteger(REGISTER_X, lgInt);
    longIntegerToAllocatedString(lgInt, tmpString, TMP_STR_LENGTH);
    longIntegerToInt(lgInt,X_REG);
    longIntegerFree(lgInt);
    testBitset = mod(X_REG, 0x10000);
  }
}



/********************************************//**
 * \brief sys_free_mem
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnSysFreeMem(uint16_t unusedButMandatoryParameter) {
  real_t value;

  saveForUndo();
  liftStack();

  #ifdef PC_BUILD
    int32ToReal(2147483647, &value);
  #endif

  #ifdef DMCP_BUILD
    int32ToReal(sys_free_mem(), &value);
  #endif

//realDivide(&value, const_1000, &value, &ctxtReal39);
  realToReal34(&value, REGISTER_REAL34_DATA(REGISTER_X));
}



/********************************************//**
 * \brief Test if bit is set in testBitset
 *
 * \param[in] bit uint8_t
 * \return void
 ***********************************************/
bool_t fnTestBitIsSet(uint8_t bit) {
  uint16_t new_num = testBitset >> (bit);

  return (new_num & 1);
}
#endif
