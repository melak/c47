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
 * \file timer.c The timer application
 ***********************************************/

#include "wp43s.h"

#ifndef TESTSUITE_BUILD
  uint32_t getUptimeMs(void) {
    #ifdef DMCP_BUILD
      return (uint32_t)sys_current_ms();
    #endif
    #ifdef PC_BUILD
      return (uint32_t)(g_get_monotonic_time() / 1000);
    #endif
  }
#endif


void fnTicks(uint16_t unusedParamButMandatory) {
#ifndef TESTSUITE_BUILD
  uint32_t tim;
  longInteger_t lgInt;


  tim = getUptimeMs();
  tim = tim / 100;

  liftStack();
  longIntegerInit(lgInt);
  uIntToLongInteger(tim, lgInt);
  convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
  longIntegerFree(lgInt);

  refreshStack();
#endif
}
