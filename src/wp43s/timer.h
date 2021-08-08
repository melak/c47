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

/**
 * \file timer.h
 */
#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#ifdef PC_BUILD
  #include <gtk/gtk.h>
  #include <gdk/gdk.h>
#endif // PC_BUILD


uint32_t getUptimeMs(void);
void     fnTicks    (uint16_t unusedButMandatoryParameter);


#ifdef PC_BUILD
gboolean refreshTimer         (gpointer data);
#endif
#ifdef DMCP_BUILD
void     refreshTimer         (void);
#endif


void     fnTimerReset         (void);
void     fnTimerDummyTest     (uint16_t param);
void     fnTimerConfig        (uint8_t nr, void(*func)(uint16_t), uint16_t param);
void     fnTimerStart         (uint8_t nr, uint16_t param, uint32_t time);      // Start Timer, 0..n-1
void     fnTimerStop          (uint8_t nr);                                     // Stop Timer, 0..n-1
void     fnTimerExec          (uint8_t nr);                                     // Execute Timer, 0..n-1
void     fnTimerDel           (uint8_t nr);                                     // Delete Timer, 0..n-1
uint8_t  fnTimerGetStatus     (uint8_t nr);



/********************************************//**
 * \typedef timer_t
 * \brief Structure keeping the information for one timer
 ***********************************************/
#define TMR_UNUSED    0
#define TMR_STOPPED   1
#define TMR_RUNNING   2
#define TMR_COMPLETED 3



typedef struct {
  void     (*func)(uint16_t); ///< Function called to execute the timer
  uint16_t param;             ///< 1st parameter to the above
#ifndef PC_BUILD
  uint32_t timer_will_expire; ///<
#else
  gint64   timer_will_expire; ///<
#endif
  uint8_t  state;             ///<
} kb_timer_t;


#endif // TIMER_H
