/* This file is part of WP 43S.
 *
 * WP 43S is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WP 43S is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WP 43S.  If not, see <http://www.gnu.org/licenses/>.
 */

/* ADDITIONAL C43 functions and routines */


/********************************************//**
 * \file keyboardTweak.h
 ***********************************************/


//#ifdef DMCP_BUILD
//extern uint32_t nextTimerRefresh;
//#endif

void     showAlphaModeonGui   (void);
void     resetShiftState      (void);
void     showShiftState       (void);


void     processKeyAction    (int16_t item); //JM

#ifndef TESTSUITE_BUILD
bool_t   DOT_G_painted, DOT_F_painted;                                      //JM OPTIMISE the dot placement by removing the same pixels placed. No need for the full redraw of the softmenu.
void 	 DOT_F();
void 	 DOT_G();
void 	 DOT_F_clear();
void 	 DOT_G_clear();
void     show_f_jm           (void);
void     show_g_jm           (void);
void     clear_fg_jm         (void);
void     fg_processing_jm    (void);
void Check_Assign_in_progress(int16_t  * result, int16_t tempkey);

bool_t   func_lookup         (int16_t  fn, int16_t itemShift, int16_t *funk);
void     execFnTimeout       (uint16_t key                    );                         //dr - delayed call of the primary function key
void     shiftCutoff         (uint16_t unusedParamButMandatory);     //dr - press shift three times within one second to call HOME timer
void     Check_MultiPresses  (int16_t  * result, int8_t key_no);
void     Setup_MultiPresses  (int16_t  result                 );
int16_t  nameFunction        (int16_t  fn, int16_t itemShift  );   //JM LONGPRESS FN

#endif

#ifdef PC_BUILD
gboolean refreshTimer         (gpointer data);
void     btnFnPressed_StateMachine (GtkWidget *w, gpointer data);
void     btnFnReleased_StateMachine(GtkWidget *w, gpointer data);

#endif
#ifdef DMCP_BUILD
void     refreshTimer         (void);


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

void     keyBuffer_pop        ();
uint8_t  inKeyBuffer          (uint8_t byte);
#ifdef BUFFER_CLICK_DETECTION
#ifdef BUFFER_KEY_COUNT
uint8_t  outKeyBuffer         (uint8_t *pByte, uint8_t *pByteCount, uint32_t *pTime, uint32_t *pTimeSpan_1, uint32_t *pTimeSpan_B);
#else
uint8_t  outKeyBuffer         (uint8_t *pByte, uint32_t *pTime, uint32_t *pTimeSpan_1, uint32_t *pTimeSpan_B);
#endif
#else
#ifdef BUFFER_KEY_COUNT
uint8_t  outKeyBuffer         (uint8_t *pByte, uint8_t *pByteCount);
#else
uint8_t  outKeyBuffer         (uint8_t *pByte);
#endif
#endif
uint8_t  outKeyBufferDoubleClick();
bool_t   fullKeyBuffer        ();
bool_t   emptyKeyBuffer       ();                           //^^


void     btnFnPressed_StateMachine (void *w, void *data);
void     btnFnReleased_StateMachine(void *w, void *data);
#endif

void     fnTimerReset         (void);
void     fnTimerDummyTest     (uint16_t param);
void     fnTimerConfig        (uint8_t nr, void(*func)(uint16_t), uint16_t param/*, uint32_t time*/);
void     fnTimerStart         (uint8_t nr, uint16_t param, uint32_t time);      // Start Timer, 0..n-1
void     fnTimerStop          (uint8_t nr);                                     // Stop Timer, 0..n-1
void     fnTimerExec          (uint8_t nr);                                     // Execute Timer, 0..n-1
void     fnTimerDel           (uint8_t nr);                                     // Delete Timer, 0..n-1
uint8_t  fnTimerGetStatus     (uint8_t nr);



/********************************************//**
 * \typedef timer_t
 * \brief Structure keeping the information for one timer
 ***********************************************/
#define TMR_NUMBER    8

#define TMR_UNUSED    0
#define TMR_STOPPED   1
#define TMR_RUNNING   2
#define TMR_COMPLETED 3



typedef struct {
  void     (*func)(uint16_t); ///< Function called to execute the timer
  uint16_t param;             ///< 1st parameter to the above
//uint32_t time;              ///<
#ifndef PC_BUILD
  uint32_t timer_will_expire; ///<
#else
  gint64   timer_will_expire; ///<
#endif
  uint8_t  state;             ///<
} kb_timer_t;


void fnT_ARROW(uint16_t command);


