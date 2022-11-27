/* This file is part of WP43.
 *
 * WP43 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WP43 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WP43.  If not, see <http://www.gnu.org/licenses/>.
 */

/* ADDITIONAL C43 functions and routines */


/********************************************//**
 * \file keyboardTweak.h
 ***********************************************/

#ifndef KEYBOARDTWEAK_H
#define KEYBOARDTWEAK_H

#include "defines.h"
#include "typeDefinitions.h"
#include <stdint.h>
#ifdef PC_BUILD
  #include <gtk/gtk.h>
  #include <gdk/gdk.h>
#endif // PC_BUILD

//#ifdef DMCP_BUILD
//extern uint32_t nextTimerRefresh;
//#endif

TO_QSPI void     keyClick(uint8_t length); 

TO_QSPI void     showAlphaModeonGui   (void);
TO_QSPI void     resetShiftState      (void);
TO_QSPI void     showShiftState       (void);
TO_QSPI void     fnSHIFTf(uint16_t unusedButMandatoryParameter);
TO_QSPI void     fnSHIFTg(uint16_t unusedButMandatoryParameter);

#ifndef TESTSUITE_BUILD
TO_QSPI void     show_f_jm           (void);
TO_QSPI void     show_g_jm           (void);
TO_QSPI void     clear_fg_jm         (void);
TO_QSPI void     fg_processing_jm    (void);
TO_QSPI void     Check_SigmaPlus_Assigned(int16_t  * result, int16_t tempkey);

TO_QSPI bool_t   func_lookup         (int16_t  fn, int16_t itemShift, int16_t *funk);
TO_QSPI void     execFnTimeout       (uint16_t key                    );                         //dr - delayed call of the primary function key
TO_QSPI void     shiftCutoff         (uint16_t unusedButMandatoryParameter);     //dr - press shift three times within one second to call HOME timer
TO_QSPI void     Check_MultiPresses  (int16_t  * result, int8_t key_no);
TO_QSPI void     Setup_MultiPresses  (int16_t  result                 );
TO_QSPI int16_t  nameFunction        (int16_t  fn, int16_t itemShift  );   //JM LONGPRESS FN
TO_QSPI void     resetKeytimers      (void);
#endif

TO_QSPI uint16_t numlockReplacements(uint16_t id, int16_t item, bool_t NL, bool_t SHFT, bool_t GSHFT);
TO_QSPI bool_t keyReplacements(int16_t item, int16_t * item1, bool_t NL, bool_t SHFT, bool_t GSHFT);

#ifdef PC_BUILD
TO_QSPI void     btnFnPressed_StateMachine (GtkWidget *unused, gpointer data);
TO_QSPI void     btnFnReleased_StateMachine(GtkWidget *unused, gpointer data);
#endif

#ifndef TESTSUITE_BUILD
TO_QSPI int16_t determineFunctionKeyItem_C43(const char *data, bool_t ShiftF, bool_t ShiftG);
#endif

#ifdef DMCP_BUILD
#define BUFFER_FAIL     0                                   //vv dr - internal keyBuffer POC
#define BUFFER_SUCCESS  1
#define BUFFER_MASK (BUFFER_SIZE-1) // Klammern auf keinen Fall vergessen

typedef struct {
  uint8_t   data[BUFFER_SIZE];
  #ifdef    BUFFER_CLICK_DETECTION
  uint32_t  time[BUFFER_SIZE];
  #endif
  uint8_t   read;   // zeigt auf das Feld mit dem ältesten Inhalt
  uint8_t   write;  // zeigt immer auf leeres Feld
} kb_buffer_t;

TO_QSPI void     keyBuffer_pop        ();
TO_QSPI uint8_t  inKeyBuffer          (uint8_t byte);
#ifdef BUFFER_CLICK_DETECTION
#ifdef BUFFER_KEY_COUNT
TO_QSPI uint8_t  outKeyBuffer         (uint8_t *pByte, uint8_t *pByteCount, uint32_t *pTime, uint32_t *pTimeSpan_1, uint32_t *pTimeSpan_B);
#else
TO_QSPI uint8_t  outKeyBuffer         (uint8_t *pByte, uint32_t *pTime, uint32_t *pTimeSpan_1, uint32_t *pTimeSpan_B);
#endif
#else
#ifdef BUFFER_KEY_COUNT
TO_QSPI uint8_t  outKeyBuffer         (uint8_t *pByte, uint8_t *pByteCount);
#else
TO_QSPI uint8_t  outKeyBuffer         (uint8_t *pByte);
#endif
#endif
TO_QSPI uint8_t  outKeyBufferDoubleClick();
TO_QSPI bool_t   fullKeyBuffer        ();
TO_QSPI bool_t   emptyKeyBuffer       ();                           //^^


TO_QSPI void     btnFnPressed_StateMachine (void *unused, void *data);
TO_QSPI void     btnFnReleased_StateMachine(void *unused, void *data);
#endif

TO_QSPI void fnCla                   (uint16_t unusedButMandatoryParameter);
TO_QSPI void fnCln                   (uint16_t unusedButMandatoryParameter);

TO_QSPI void fnT_ARROW(uint16_t command);

#endif // KEYBOARDTWEAK_H
