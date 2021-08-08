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

#include "timer.h"

#include "registerValueConversions.h"
#include "stack.h"

#include "wp43s.h"


kb_timer_t  timer[TMR_NUMBER];
#ifdef PC_BUILD
gint64      timerLastCalled;
#endif
#ifdef DMCP_BUILD
uint32_t    timerLastCalled;
#endif


uint32_t getUptimeMs(void) {
  #ifdef DMCP_BUILD
    return (uint32_t)sys_current_ms();
  #elif !defined(TESTSUITE_BUILD)
    return (uint32_t)(g_get_monotonic_time() / 1000);
  #else // TESTSUITE_BUILD
    return 0;
  #endif // DMCP_BUILD ; TESTSUITE_BUILD
}


void fnTicks(uint16_t unusedButMandatoryParameter) {
  uint32_t tim;
  longInteger_t lgInt;

  tim = getUptimeMs() / 100;

  liftStack();
  longIntegerInit(lgInt);
  uIntToLongInteger(tim, lgInt);
  convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
  longIntegerFree(lgInt);
}



void fnRebuildTimerRefresh(void) {
#ifdef DMCP_BUILD
  bool_t hasRunning = false;
  uint32_t next;

  for(int i = 0; i < TMR_NUMBER; i++) {
    if (timer[i].state == TMR_RUNNING) {
      hasRunning = true;
      next = timer[i].timer_will_expire;
      if(nextTimerRefresh != 0 && next < nextTimerRefresh) {
        nextTimerRefresh = next;
      }
      if(nextTimerRefresh == 0) {
        nextTimerRefresh = next;
      }
    }
  }

  if(!hasRunning) {
    nextTimerRefresh = 0;
  }
#endif
}


#ifdef PC_BUILD
/********************************************//**
 * \brief Refreshes timer. This function is
 * called every 5 ms by a GTK timer.
 *
 * \param[in] data gpointer Not used
 * \return gboolean         What will happen next?
 *                          * true  = timer will call this function again
 *                          * false = timer stops calling this function
 ***********************************************/
gboolean refreshTimer(gpointer data) {      // This function is called every 5 ms by a GTK timer
  gint64 now = g_get_monotonic_time();

  if(now < timerLastCalled) {
    for(int i = 0; i < TMR_NUMBER; i++) {
      if(timer[i].state == TMR_RUNNING) {
        timer[i].state = TMR_COMPLETED;
        timer[i].func(timer[i].param);      // Callback to configured function
      }
    }
  }
  else {
    for(int i = 0; i < TMR_NUMBER; i++) {
      if(timer[i].state == TMR_RUNNING) {
        if(timer[i].timer_will_expire <= now) {
          timer[i].state = TMR_COMPLETED;
          timer[i].func(timer[i].param);    // Callback to configured function
        }
      }
    }
  }

  timerLastCalled = now;

//fnRebuildTimerRefresh();

  return TRUE;
}
#endif

#ifdef DMCP_BUILD
void refreshTimer(void) {                   // This function is called when nextTimerRefresh has been elapsed
  uint32_t now = (uint32_t)sys_current_ms();

  if(now < timerLastCalled) {
    for(int i = 0; i < TMR_NUMBER; i++) {
      if(timer[i].state == TMR_RUNNING) {
        timer[i].state = TMR_COMPLETED;
        timer[i].func(timer[i].param);      // Callback to configured function
      }
    }
  }
  else {
    for(int i = 0; i < TMR_NUMBER; i++) {
      if(timer[i].state == TMR_RUNNING) {
        if(timer[i].timer_will_expire <= now) {
          timer[i].state = TMR_COMPLETED;
          timer[i].func(timer[i].param);    // Callback to configured function
        }
      }
    }
  }

  timerLastCalled = now;

  fnRebuildTimerRefresh();
}
#endif


void fnTimerDummyTest(uint16_t param) {
#if defined(PC_BUILD) || defined (TESTSUITE_BUILD)
  printf("fnTimerDummyTest called  %u\n", param);
#endif
}



void fnTimerReset(void) {
#ifndef TESTSUITE_BUILD
  timerLastCalled = 0;
#endif

  for(int i = 0; i < TMR_NUMBER; i++) {
    timer[i].state = TMR_UNUSED;
    timer[i].func = fnTimerDummyTest;
    timer[i].param = 0;
  }

  fnRebuildTimerRefresh();
}



void fnTimerConfig(uint8_t nr, void(*func)(uint16_t), uint16_t param) {
  if(nr < TMR_NUMBER) {
    timer[nr].func  = func;
    timer[nr].param = param;
    timer[nr].state = TMR_STOPPED;
  }
  fnRebuildTimerRefresh();
}



void fnTimerStart(uint8_t nr, uint16_t param, uint32_t time) {
#ifdef DMCP_BUILD
  uint32_t now = (uint32_t)sys_current_ms();
#endif
#ifdef PC_BUILD
  gint64 now = g_get_monotonic_time();
#endif
#ifndef TESTSUITE_BUILD
  timerLastCalled = now;
#endif

  if(nr < TMR_NUMBER) {
    timer[nr].param = param;
#ifdef DMCP_BUILD
    timer[nr].timer_will_expire = (uint32_t)(now + time);
#endif
#ifdef PC_BUILD
    timer[nr].timer_will_expire = (gint64)(now + time *1000);
    if(timer[nr].timer_will_expire < 0) {
      timer[nr].timer_will_expire = time *1000;
    }
#endif
    timer[nr].state = TMR_RUNNING;
  }
  fnRebuildTimerRefresh();
}



void fnTimerStop(uint8_t nr) {
  if(nr < TMR_NUMBER && timer[nr].state != TMR_UNUSED) {
    timer[nr].state = TMR_STOPPED;
  }
  fnRebuildTimerRefresh();
}


void fnTimerExec(uint8_t nr) {
  if(nr < TMR_NUMBER && timer[nr].state == TMR_RUNNING) {
    timer[nr].state = TMR_COMPLETED;
    timer[nr].func(timer[nr].param);        // Callback to configured function
  }
}



void fnTimerDel(uint8_t nr) {
  if(nr < TMR_NUMBER) {
    timer[nr].state = TMR_UNUSED;
  }
  fnRebuildTimerRefresh();
}



uint8_t fnTimerGetStatus(uint8_t nr) {
  uint8_t result = TMR_UNUSED;
  
  if(nr < TMR_NUMBER) {
    result = timer[nr].state;
  }

  return result;
}
