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
 * \file keyboardTweak.c
 ***********************************************/

#include "wp43s.h"


kb_timer_t  timer[TMR_NUMBER];
#ifdef PC_BUILD
gint64      timerLastCalled;
#endif
#ifdef DMCP_BUILD
uint32_t    timerLastCalled;
#endif



void showAlphaModeonGui(void) {
  if(calcMode == CM_AIM || calcMode == CM_ASM || CM_ASM_OVER_TAM || CM_ASM_OVER_AIM)    //vv dr JM
  {
    showHideAlphaMode();
    #ifdef PC_BUILD     //dr - new AIM
      calcModeAimGui();
    #endif
  }                                               //^^
}




void fnKeyCase(uint16_t unusedParamButMandatory) {    //JM CASE JM CAPS
  #ifndef TESTSUITE_BUILD
    if(alphaCase == AC_LOWER) {
      alphaCase = AC_UPPER;
    } else {
      alphaCase = AC_LOWER;
    }

    showAlphaModeonGui(); //dr JM, see keyboardtweaks

    showSoftmenuCurrentPart();
    setCatalogLastPos();
  #endif
}





#ifndef TESTSUITE_BUILD
/********************************************//**
 * \brief Displays the f or g shift state in the
 * upper left corner of the T register line
 *
 * \param void
 * \return void
 ***********************************************/
bool_t DOT_G_painted, DOT_F_painted;                                      //JM OPTIMISE the dot placement by removing the same pixels placed. No need for the full redraw of the softmenu.

void DOT_F() {
  JM_DOT( -1, 201 );                                                      //JM - Display dot in the f - line
  JM_DOT( 392, 201 );                                                     //JM - Display dot in the f - line
  DOT_F_painted = !DOT_F_painted;
}

void DOT_G() {
  JM_DOT( -1, 175-2 );                                                      //JM - Display dot in the g - line
  JM_DOT( 392, 175-2 );                                                     //JM - Display dot in the g - line
  JM_DOT( -1, 182+3 );                                                      //JM - Display dot in the g - line
  JM_DOT( 392, 182+3 );                                                     //JM - Display dot in the g - line
  DOT_G_painted = !DOT_G_painted;
}

void DOT_F_clear() {
  if(DOT_F_painted) {
    DOT_F();
  }
}

void DOT_G_clear() {
  if(DOT_G_painted) {
    DOT_G();
  }
}


void show_f_jm(void){
          //showSoftmenuCurrentPart();                                                //JM - Redraw boxes etc after shift is shown
        if(softmenuStackPointer > 0) {                                            //JM - Display dot in the f - line
          DOT_G_clear();
          DOT_F();
          if(!FN_timeouts_in_progress) {
            if(!ULFL) {
              underline(1);
              ULFL = !ULFL;
            }
            if(ULGL) {
              underline(2);
              ULGL = !ULGL;
            }
          }
        }                                                                         //JM - Display dot in the f - line
}


void show_g_jm(void){
      //showSoftmenuCurrentPart();                                                //JM - Redraw boxes etc after shift is shown
        if(softmenuStackPointer > 0) {                                            //JM - Display dot in the g - line
          DOT_F_clear(); //cancel dots
          DOT_G();
          if(!FN_timeouts_in_progress) {
            if(ULFL) {
              underline(1);
              ULFL = !ULFL;
            }
            if(!ULGL) {
              underline(2);
              ULGL = !ULGL;
            }
          }
        }                                                                         //JM - Display dot in the g - line
}


void clear_fg_jm(void){
        //DOT_G_clear(); //cancel dots
        showSoftmenuCurrentPart();            //JM TO REMOVE STILL !!             //JM - Redraw boxes etc after shift is shown
        DOT_G_painted = false;
        DOT_F_painted = false;
}


void fg_processing_jm(void) {
    if(ShiftTimoutMode || Home3TimerMode) {
      if(Home3TimerMode) {
        if(fnTimerGetStatus(TO_3S_CTFF) == TMR_RUNNING) {
          JM_SHIFT_HOME_TIMER1++;
          if(JM_SHIFT_HOME_TIMER1 >= 3) {
            fnTimerStop(TO_3S_CTFF);
            shiftF = false;  // Set it up, for flags to be cleared below.
            shiftG = true;
            if(HOME3) {
              if((softmenuStackPointer > 0) && (softmenuStackPointer_MEM == softmenuStackPointer)) {                          //JM shifts
                popSoftmenu();                                                                                                //JM shifts
              }
              else {
                if(calcMode == CM_AIM) {                                                                                      //JM shifts
                  showSoftmenu(NULL, -MNU_ALPHA, true);                                                                       //JM shifts //JM ALPHA-HOME  ALPHA AIM OR NIM
                }
                else {                                                                                                        //JM SHIFTS
                  showSoftmenu(NULL, -MNU_HOME, true);                                                                        //JM shifts  //JM ALPHA-HOME
                }                                                                                                             //JM shifts                                                                                                                              //JM shifts
                softmenuStackPointer_MEM = softmenuStackPointer;                                                              //JM shifts
              }
            }                                                                                                                 //JM shifts
          }
        }
        if(fnTimerGetStatus(TO_3S_CTFF) == TMR_STOPPED) {
          JM_SHIFT_HOME_TIMER1 = 1;
          fnTimerStart(TO_3S_CTFF, TO_3S_CTFF, JM_TO_3S_CTFF);        //dr
        }
      }
    }

    if(!shiftF && !shiftG) {                                                                                                  //JM shifts
      shiftF = true;                                                                                                          //JM shifts
      shiftG = false;
    }                                                                                                                         //JM shifts
    else if(shiftF && !shiftG) {                                                                                              //JM shifts
      shiftF = false;                                                                                                         //JM shifts
      shiftG = true;                                                                                                          //JM shifts
    }
    else if(!shiftF && shiftG) {                                                                                              //JM shifts
      shiftF = false;                                                                                                         //JM shifts
      shiftG = false;                                                                                                         //JM shifts
    }
    else if(shiftF && shiftG) {                                                                                               //JM shifts  should never be possible. included for completeness
      shiftF = false;                                                                                                         //JM shifts
      shiftG = false;                                                                                                         //JM shifts
    }                                                                                                                         //JM shifts
}




void  Check_Assign_in_progress(int16_t * result, int16_t tempkey) {
  //JM ASSIGN - GET FUNCTION NUMBER          // Check if JM ASSIGN IS IN PROGRESS AND CAPTURE THE FUNCTION AND KEY TO BE ASSIGNED
  if(JM_ASN_MODE == 32766) {                 // GET FUNCTION NUMBER: If seek is pressed, a function can be chosen and pressed.
    JM_ASN_MODE = *result;                   // The result is the function number, item number, asnd is placed in
    fnKEYSELECT();                           // Place in auto trigger register, ready for next keypress
    *result = KEY_EXIT1;                     // EXIT key to exit when done and cancel shifts
  }
  //JM ASSIGN - GET KEY & ASSIGN MEMORY FUNCTION JM_ASN_MODE // JM_ASN_MODE contains the command to be put on a key. 0 if not active
  else if(JM_ASN_MODE != 0) {                // GET KEY
    fnASSIGN(JM_ASN_MODE, tempkey);          // CHECKS FOR INVALID KEYS IN HERE
    JM_ASN_MODE = 0;                         // Catchall - cancel the mode once it had the opportunity to be handled. Whether handled or not.
    *result = KEY_EXIT1;                     // EXIT key to exit when done and cancel shifts
  }
  //JM NORMKEY _ CHANGE NORMAL MODE KEY SIGMA+ TO SOMETHING ELSE vv
  else if((calcMode == CM_NORMAL || calcMode == CM_NIM) && (!getSystemFlag(FLAG_USER) && !shiftF && !shiftG && ( tempkey == 0) )) {
    *result = Norm_Key_00_VAR;
  }
}


/*
  //JM ASSIGN - GET FUNCTION NUMBER --------------------------------------------------------------------------------
  if(JM_ASN_MODE == 32766) {            //JM Check if JM ASSIGN IS IN PROGRESS AND CAPTURE THE FUNCTION AND KEY TO BE ASSIGNED
                                        //JM GET FUNCTION NUMBER: If seek is pressed, a function can be chosen and pressed.
    JM_ASN_MODE = result;               //JM The result is the function number, item number, asnd is placed in
    fnKEYSELECT();                           //JM Place in auto trigger register, ready for next keypress
    result = KEY_EXIT1;                      //JM EXIT key to exit when done and cancel shifts
  }

  //JM ASSIGN - GET KEY & ASSIGN MEMORY FUNCTION JM_ASN_MODE
                                             //JM JM_ASN_MODE contains the command to be put on a key. 0 if not active
  else if(JM_ASN_MODE != 0) {                //JM GET KEY
    fnASSIGN(JM_ASN_MODE, stringToKeyNumber(data));          //JM CHECKS FOR INVALID KEYS IN HERE
    JM_ASN_MODE = 0;                         //JM Catchall - cancel the mode once it had the opportunity to be handled. Whether handled or not.
    result = KEY_EXIT1;                       //JM EXIT key to exit when done and cancel shifts
  }

  //JM NORMKEY _ CHANGE NORMAL MODE KEY SIGMA+ TO SOMETHING ELSE vv
  else if((calcMode == CM_NORMAL || calcMode == CM_NIM) && (!getSystemFlag(FLAG_USER) && !shiftF && !shiftG && ( stringToKeyNumber(data) == 0) )) {
    //printf("%d", stringToKeyNumber(data));
    result = Norm_Key_00_VAR;
  } //JM ^^
  //JM    ^^^^^^^^^^^^^^^^^^^^^^^^^^^ --------------------------------------------------------------------------------
*/





//JM Changing the individual softmenu cell, based on the content of the assigned keyboard array
bool_t func_lookup(int16_t fn, int16_t itemShift, int16_t *funk) {
  int16_t ix_fn, ix_sm,ix, ix0;  //JMXXX
  bool_t tmp;
  tmp = false;

  ix = itemShift + (fn - 1);
  ix0 = softmenuStack[softmenuStackPointer - 1].firstItem;
  ix_sm = softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId;
  
  if(ix_sm == -MNU_HOME) {
    if(menu_A_HOME[ix0+ix]!=-1) {
      ix_fn = -9999; 
  
      //The problem was at this point menu_A_HOME[ix0+ix] could be -1.
      //Which means kbd_std or kbd_usr[-1] is looked up.
  
      if(menu_A_HOME[ix0+ix] < 100) {ix_fn = !getSystemFlag(FLAG_USER) ? (kbd_std[menu_A_HOME[ix0+ix]    ].primary ) : (kbd_usr[menu_A_HOME[ix0+ix]    ].primary );}  else           
      if(menu_A_HOME[ix0+ix] < 200) {ix_fn = !getSystemFlag(FLAG_USER) ? (kbd_std[menu_A_HOME[ix0+ix]-100].fShifted) : (kbd_usr[menu_A_HOME[ix0+ix]-100].fShifted);}  else
      if(menu_A_HOME[ix0+ix] < 300) {ix_fn = !getSystemFlag(FLAG_USER) ? (kbd_std[menu_A_HOME[ix0+ix]-200].gShifted) : (kbd_usr[menu_A_HOME[ix0+ix]-200].gShifted);}  else
      if(menu_A_HOME[ix0+ix]>= 300) {ix_fn = -9999;}
      //printf("--> MNU_HOME:%d (current menu %d): first item ix0:%d + ix:%d\n",MNU_HOME, ix_sm,ix0,ix);
      //printf("    menu_A_HOME looked up key:%d menu_HOME original softkey function: %d\n", menu_A_HOME[ix0+ix], menu_HOME[ix0+ix]);
      //printf("    Function on key: %d. ", ix_fn);
      //printf(   "  Use this function: %d ", (userModeEnabled && (menu_A_HOME[ix0+ix]!=-1)) );
      //printf(   "  %s\n", indexOfItems[ix_fn].itemSoftmenuName );
  
      //Handle normal mode key change in dynamic HOME menu
      if(ix_fn != -9999 && ix == 0 && !getSystemFlag(FLAG_USER) && menu_A_HOME[ix0+ix] == 0 && (calcMode == CM_NORMAL || calcMode == CM_NIM) && (Norm_Key_00_VAR != kbd_std[0].primary)){
        ix_fn = Norm_Key_00_VAR;
        tmp = true;
      }
  
      //Change direct entry functions from key allocation to special allocation
      if (ix_fn == ITM_toINT)  {    //  793   fnChangeBase, TM_VALUE_CHB, STD_RIGHT_ARROW "INT",                         "#"
          ix_fn = KEY_HASH;         //  1737  fnBASE_Hash,  NOPARAM, "##" STD_RIGHT_ARROW "INT", 
      }
/* else
      if (ix_fn == KEY_dotD) {      //  1527 fnJM, 255, "", ".d",
         ix_fn = ITM_DOTDEMU;       //  1935 fnKeyDotD, NOPARAM, "Dot.d"
      }
      //printf("    Function on key: %d. Use this function: %d %s\n", ix_fn, (userModeEnabled && (menu_A_HOME[ix0+ix]!=-1)), indexOfItems[ix_fn].itemSoftmenuName );
*/  
      *funk = ix_fn;
      return ix_fn != -9999 && ((getSystemFlag(FLAG_USER)) || (!getSystemFlag(FLAG_USER) && tmp));
    }
    return false;
  }
  return false;
}



void Setup_MultiPresses(int16_t result){
  JM_auto_doublepress_enabled = 0;                   //JM TIMER CLRDROP. Autodrop mean double click normal key.
  int16_t tmp = 0;
  if(calcMode == CM_NORMAL) {
    switch(result) {
      case KEY_BACKSPACE: tmp = ITM_DROP; break;      //Set up backspace double click to DROP
      case ITM_XEQ      : tmp = -MNU_XEQ; break;      //XEQ XEQMENU
      //case ITM_CHS      : tmp = ITM_XexY; break;      //sample on CHS, operating X<>Y. XEQ must still be created.
      default:;
    }
  }
  if(tmp != 0){
    if(fnTimerGetStatus(TO_CL_DROP) == TMR_RUNNING) {
      JM_auto_doublepress_enabled = tmp;
    }
    fnTimerStart(TO_CL_DROP, TO_CL_DROP, JM_CLRDROP_TIMER);       
  }
  fnTimerStop(TO_FG_LONG);                          //dr
  fnTimerStop(TO_FN_LONG);                          //dr
}



void Check_MultiPresses(int16_t * result){          //Set up longpress
  int16_t tmp = 0;

  if(calcMode == CM_NORMAL) {
    switch(*result) {
      case KEY_BACKSPACE: tmp = ITM_CLSTK; break;   //backspace longpress to CLSTK
      case ITM_XEQ      : tmp = -MNU_XEQ;  break;   //XEQ longpress to XEQMENU 
      //case ITM_CHS      : tmp = ITM_XexY;  break;   //sample on CHS, operating X<>Y. XEQ must still be created.
      default:;
    }
  }

  if(tmp == 0 && (calcMode == CM_NORMAL || calcMode == CM_NIM)) {
    if (*result == kbd_usr[ 0].primary)  { tmp =  !getSystemFlag(FLAG_USER) ? (kbd_std[ 0].fShifted) : (kbd_usr[ 0].fShifted); } else
    if (*result == kbd_usr[ 1].primary)  { tmp =  !getSystemFlag(FLAG_USER) ? (kbd_std[ 1].fShifted) : (kbd_usr[ 1].fShifted); } else
    if (*result == kbd_usr[ 2].primary)  { tmp =  !getSystemFlag(FLAG_USER) ? (kbd_std[ 2].fShifted) : (kbd_usr[ 2].fShifted); } else
    if (*result == kbd_usr[ 3].primary)  { tmp =  !getSystemFlag(FLAG_USER) ? (kbd_std[ 3].fShifted) : (kbd_usr[ 3].fShifted); } else
    if (*result == kbd_usr[ 4].primary)  { tmp =  !getSystemFlag(FLAG_USER) ? (kbd_std[ 4].fShifted) : (kbd_usr[ 4].fShifted); } else
    if (*result == kbd_usr[ 5].primary)  { tmp =  !getSystemFlag(FLAG_USER) ? (kbd_std[ 5].fShifted) : (kbd_usr[ 5].fShifted); } else
    if (*result == kbd_usr[ 6].primary)  { tmp =  !getSystemFlag(FLAG_USER) ? (kbd_std[ 6].fShifted) : (kbd_usr[ 6].fShifted); } else
    if (*result == kbd_usr[ 7].primary)  { tmp =  !getSystemFlag(FLAG_USER) ? (kbd_std[ 7].fShifted) : (kbd_usr[ 7].fShifted); } else
    if (*result == kbd_usr[ 8].primary)  { tmp =  !getSystemFlag(FLAG_USER) ? (kbd_std[ 8].fShifted) : (kbd_usr[ 8].fShifted); } else
    if (*result == kbd_usr[ 9].primary)  { tmp =  !getSystemFlag(FLAG_USER) ? (kbd_std[ 9].fShifted) : (kbd_usr[ 9].fShifted); } else
    if (*result == kbd_usr[10].primary)  { tmp =  !getSystemFlag(FLAG_USER) ? (kbd_std[10].fShifted) : (kbd_usr[10].fShifted); } else
    if (*result == kbd_usr[11].primary)  { tmp =  !getSystemFlag(FLAG_USER) ? (kbd_std[11].fShifted) : (kbd_usr[11].fShifted); } else
    if (*result == kbd_usr[12].primary)  { tmp =  !getSystemFlag(FLAG_USER) ? (kbd_std[12].fShifted) : (kbd_usr[12].fShifted); } else
    if (*result == kbd_usr[13].primary)  { tmp =  !getSystemFlag(FLAG_USER) ? (kbd_std[13].fShifted) : (kbd_usr[13].fShifted); }
  }

  if(tmp !=0) {                                      //if activated key pressed 
    JM_auto_longpress_enabled = tmp;
    fnTimerStart(TO_CL_LONG, TO_CL_LONG, JM_TO_CL_LONG);    //dr
    if(JM_auto_doublepress_enabled != 0) {
       hideFunctionName();    
       restoreStack();
       showFunctionName(JM_auto_doublepress_enabled, 10);  //JM CLRDROP
       #ifdef PC_BUILD
         refreshScreen(NULL);            //JM CLRDROP
       #elif DMCP_BUILD
         refreshScreen();                //JM CLRDROP
       #endif
       *result = JM_auto_doublepress_enabled;
       fnTimerStop(TO_CL_DROP);         //JM TIMER CLRDROP ON DOUBLE BACKSPACE
       STACK_LIFT_ENABLE;               //JM TIMER CLRDROP ON DOUBLE BACKSPACE
    }                                  //JM TIMER CLRDROP ON DOUBLE BACKSPACE
  }

}



//******************* JM LONGPRESS & JM DOUBLE CLICK START *******************************

int16_t nameFunction(int16_t fn, int16_t itemShift) {                       //JM LONGPRESS vv
  int16_t row, func;
  int16_t ix_fn;  //JMXXX
  func = 0;
  const softmenu_t *sm;

  if(softmenuStackPointer > 0) {
    sm = &softmenu[softmenuStack[softmenuStackPointer - 1].softmenu];
    row = min(3, (sm->numItems + modulo(softmenuStack[softmenuStackPointer - 1].firstItem - sm->numItems, 6))/6 - softmenuStack[softmenuStackPointer - 1].firstItem/6) - 1;

    if(itemShift/6 <= row && softmenuStack[softmenuStackPointer - 1].firstItem + itemShift + (fn - 1) < sm->numItems) {
      func = (sm->softkeyItem)[softmenuStack[softmenuStackPointer - 1].firstItem + itemShift + (fn - 1)];
/*XXX*/
      ix_fn = 0;
      if(func_lookup(fn,itemShift,&ix_fn)) {
        //printf("---%d\n",ix_fn);
        func = ix_fn;
      }
/*XXX*/

      if(func == CHR_PROD_SIGN) {
        func = (getSystemFlag(FLAG_MULTx) ? CHR_DOT : CHR_CROSS);
      }

      if(func < 0) {
        func = - func;
      }
    }
  }

  return func % 10000;
}


//CONCEPT - actual timing was changed:

/* Switching profile, to jump to g[FN]:
                   set g
____-----_______--------->
    P    R      P

   |>              btnFnPressed:  mark time and ignore if FN_double_click is not set
        |>         btnFnReleased: if t<t0 then set FN_double_click AND prevent normal MAIN keypress
               |>  btnFnPressed:  if t<t1 AND if FN_double_click then jump start to g[FN]


Standard procedure LONGPRESS:

   | 800 | 800 | 800 |    ms     Monitored while LONGPRESSED
___+-----|-----|-----|--> NOP    Press and time out to NOP
___+---x_|_____|_____|___        Press and release within Fx time. Fx selected upon release
___+-----|--x__|_____|___        Press and hold, release within f(Fx) time. f(Fx) selected upon release
___+-----|-----|--___|___        Press and hold and hold, release within g(Fx) time. g(Fx) selected upon release
         |     |     |

At x, at time 0, wait 75 ms to see if a press happens, i.e. double click.
- After 75 ms, execute function.
- If before 75 ms, key is pressed, jump to the beginning of g(Fx) state.

     STATE #1 #2 #3 #4
___________---___---_____       STATES indicated

----------x1 x2 x3 x4           X's indicate transitions X


Detail for x (transition press/release):

STATE1  | STATE2  | STATE3
        |t=0      |t=75 ms
--------E______   |             Showing release and immediate execution at "E" (previous system, no LONGPRESS no DOUBLE click)
--------x_________E_____        Showing release at x and delayed execution "E" (if double click is to be detected)
         #########              ## shows DEAD TIME where 43C does not react to the key release that already happened
                  ^             ^ shows the point where the x command is executed if no double press recognised.

--------x_______G-|-----        Starting with key already pressed, release at x, re-press shown at 60 ms, double click registered, g-shift activated, COMMAND displayed, timing started to NOP if not released.

--------x_________E             Starting with key already pressed, release at x, no repress within 75 ms execute Fx.
--------x_________|_C---        Starting with key already pressed, release at x, re-press shown at 85 ms, timing cycle C re-started on Fx.


Double click release:
Timing is reset to start 800 ms from G again
--------x_______G---E__        Starting with key already pressed, release at x, re-press shown at 60 ms, double click registered, g-shift activated, COMMAND displayed, timing started, key released

//Summary
  //Process starts in btnFnReleased, with a FN key released.
  //  do timecheck, i.e. zero the timer
  //At the next press, i.e. btnFnPressed, check for a 5 ms < re-press < 75 ms gap, which, indicates a double press. (5 ms = debounce check).
  //Set a flag here, to let the refresh cyclic check for 75 ms and, if exceeded, execute the function

*/



//**************JM DOUBLE CLICK SUPPORT vv **********************************
void FN_cancel() {
  FN_key_pressed = 0;
  FN_timeouts_in_progress = false;
  fnTimerStop(TO_FN_LONG);                                  //dr
}



//*************----------*************------- FN KEY PRESSED -------***************-----------------
#ifdef PC_BUILD                                                           //JM LONGPRESS FN
void btnFnPressed_StateMachine(GtkWidget *w, gpointer data) { 
#endif
#ifdef DMCP_BUILD
void btnFnPressed_StateMachine(void *w, void *data) {
#endif
  bool_t exexute_double_g;
  bool_t double_click_detected;

#ifdef INLINE_TEST
  if(testEnabled) { fnSwStart(1); }     //dr
#endif

  FN_timed_out_to_RELEASE_EXEC = false;

  //Change states according to PRESS/RELEASE incoming sequence
  if(FN_state == ST_2_REL1) {
    FN_state = ST_3_PRESS2;
  }
  else {
    FN_state =  ST_1_PRESS1;
  }

  if(FN_state == ST_3_PRESS2 && fnTimerGetStatus(TO_FN_EXEC) == TMR_COMPLETED) {
    //----------------Copied here
    char charKey[3];
    underline_softkey(FN_key_pressed-38, 3, false);   //Purposely in row 3 which does not exist, just to activate the clear previous line
    sprintf(charKey, "%c", FN_key_pressed + 11);
    hideFunctionName();
    clearRegisterLine(REGISTER_T, true,false); //JM FN clear the previous shift function name
    refreshRegisterLine(REGISTER_T);

    //IF 2-->3 is longer than double click time, then move back to state 1
    FN_timeouts_in_progress = false;
    FN_state = ST_1_PRESS1;
  }

  FN_key_pressed = *((char *)data) - '0' + 37;                            //to render 38-43, as per original keypress

  if(fnTimerGetStatus(TO_FN_EXEC) == TMR_RUNNING) {         //vv dr new try
    if(FN_key_pressed_last != FN_key_pressed) {       //first press
      fnTimerExec(TO_FN_EXEC);
      FN_handle_timed_out_to_EXEC = true;
      exexute_double_g = false;
    }
    else {                                            //second press
      FN_handle_timed_out_to_EXEC = false;
      exexute_double_g = true;
      fnTimerStop(TO_FN_EXEC);
    }
  }
  else {
    FN_handle_timed_out_to_EXEC = true;
    exexute_double_g = false;
  }                                                         //^^

  if(FN_state == ST_1_PRESS1) {
    FN_key_pressed_last = FN_key_pressed;
  }


  //**************JM DOUBLE CLICK DETECTION ******************************* // JM FN-DOUBLE
  double_click_detected = false;                                            //JM FN-DOUBLE - Dip detection flag
  if(jm_G_DOUBLETAP) {
    if(exexute_double_g) {
      if(FN_key_pressed !=0 && FN_key_pressed == FN_key_pressed_last) {     //Identified valid double press dip, the same key in rapid succession
        shiftF = false; //R_shF();                                          //JM
        shiftG = true;  //S_shG();                                          //JM
        double_click_detected = true;                                       //JM --> FORCE INTO LONGPRESS
        FN_handle_timed_out_to_EXEC = false;                //dr
      }
    }
    else {
      FN_timeouts_in_progress = false;         //still in no shift mode
      fnTimerStop(TO_FN_LONG);                              //dr
    }
  }

  //STAGE 1 AND 3 GO HERE
  //**************JM LONGPRESS ****************************************************
  if( (FN_state == ST_1_PRESS1 || FN_state == ST_3_PRESS2) && (!FN_timeouts_in_progress || double_click_detected) && FN_key_pressed != 0) {
    FN_timeouts_in_progress = true;
    fnTimerStart(TO_FN_LONG, TO_FN_LONG, JM_TO_FN_LONG);    //dr
    FN_timed_out_to_NOP = false;
    if(!shiftF && !shiftG) {
      showFunctionName(nameFunction(FN_key_pressed-37,0),0);
      underline_softkey(FN_key_pressed-38, 0, !true /*dontclear at first call*/); //JMUL inverted clearflag
    }
    else if(shiftF && !shiftG) {
      showFunctionName(nameFunction(FN_key_pressed-37,6),0);
      underline_softkey(FN_key_pressed-38, 1, !true /*dontclear at first call*/); //JMUL inverted clearflag
    }
    else if(!shiftF && shiftG) {
      showFunctionName(nameFunction(FN_key_pressed-37,12),0);
      underline_softkey(FN_key_pressed-38, 2, !true /*dontclear at first call*/); //JMUL inverted clearflag
    }                                                                      //further shifts are done within FN_handler
  }
#ifdef INLINE_TEST
  if(testEnabled) { fnSwStop(1); }      //dr
#endif
}






//*************----------*************------- FN KEY RELEASED -------***************-----------------
#ifdef PC_BUILD                                                             //JM LONGPRESS FN
void btnFnReleased_StateMachine(GtkWidget *w, gpointer data) {              //JM LONGPRESS FN
#endif
#ifdef DMCP_BUILD
void btnFnReleased_StateMachine(void *w, void *data) {
#endif
#ifdef INLINE_TEST
  if(testEnabled) { fnSwStart(2); }     //dr
#endif

  if(FN_state == ST_3_PRESS2) {
    FN_state =  ST_4_REL2;
  }
  else {
    FN_state =  ST_2_REL1;
  }

  if(jm_G_DOUBLETAP && FN_state == ST_2_REL1 && FN_handle_timed_out_to_EXEC) {
    uint8_t                      offset =  0;
    if(shiftF && !shiftG)      { offset =  6; }
    else if(!shiftF && shiftG) { offset = 12; }
    fnTimerStart(TO_FN_EXEC, FN_key_pressed + offset, JM_FN_DOUBLE_TIMER);
  }


     // **************JM LONGPRESS EXECUTE****************************************************
  char charKey[3];
  bool_t EXEC_pri;
  EXEC_pri = (FN_timeouts_in_progress && (FN_key_pressed != 0));
  // EXEC_FROM_LONGPRESS_RELEASE     EXEC_FROM_LONGPRESS_TIMEOUT  EXEC FN primary
  if( (FN_timed_out_to_RELEASE_EXEC || FN_timed_out_to_NOP || EXEC_pri ))  {                  //JM DOUBLE: If slower ON-OFF than half the limit (250 ms)
    underline_softkey(FN_key_pressed-38, 3, false);   //Purposely in row 3 which does not exist, just to activate the clear previous line
    sprintf(charKey, "%c", FN_key_pressed + 11);
    hideFunctionName();
    clearRegisterLine(REGISTER_T, true,false);  //JM FN clear the previous shift function name
    refreshRegisterLine(REGISTER_T);

    if(!FN_timed_out_to_NOP && fnTimerGetStatus(TO_FN_EXEC) != TMR_RUNNING) {
      btnFnClicked(w, charKey);                                             //Execute
    }
    resetShiftState();
    FN_cancel();
  }
//**************JM LONGPRESS AND JM DOUBLE ^^ *********************************************   // JM FN-DOUBLE
#ifdef INLINE_TEST
  if(testEnabled) { fnSwStop(2); }      //dr
#endif
}



void execFnTimeout(uint16_t key) {                          //dr - delayed call of the primary function key
  char charKey[3];
  sprintf(charKey, "%c", key + 11);
//  int16_t fn = *((char *)charKey) - '0';

  if(calcMode != CM_CONFIRMATION) {
    allowScreenUpdate = true;

    if(lastErrorCode != 0) {
      lastErrorCode = 0;
      refreshStack();
    }
    if(softmenuStackPointer > 0) {
      if(calcMode == CM_ASM) {
        calcModeNormal();
      }
      executeFunction((char *)charKey);
    }
    else {
      resetShiftState();
      executeFunction((char *)charKey);          //JM FN NOMENU KEYS
    }
  }
}



void shiftCutoff(uint16_t unusedParamButMandatory) {        //dr - press shift three times within one second to call HOME timer
  fnTimerStop(TO_3S_CTFF);
}


//---------------------------------------------------------------


#endif





void fnRebuildTimerRefresh(void) {
#ifdef DMCP_BUILD
  bool_t hasRunning = false;
  uint32_t next;
  
  for(int i=0; i<TMR_NUMBER; i++) {
    if(timer[i].state == TMR_RUNNING) {
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
 * called every 10 ms by a GTK timer.
 *
 * \param[in] data gpointer Not used
 * \return gboolean         What will happen next?
 *                          * true  = timer will call this function again
 *                          * false = timer stops calling this function
 ***********************************************/
gboolean refreshTimer(gpointer data) {      // This function is called every 5 ms by a GTK timer
  gint64 now = g_get_monotonic_time();

  if(now < timerLastCalled) {
    for(int i=0; i<TMR_NUMBER; i++) {
      if(timer[i].state == TMR_RUNNING) {
        timer[i].state = TMR_COMPLETED;
        timer[i].func(timer[i].param);      // Callback to configured function
      }
    }
  }
  else {
    for(int i=0; i<TMR_NUMBER; i++) {
      if(timer[i].state == TMR_RUNNING) {
        if(timer[i].timer_will_expire <= now)
        {
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
    for(int i=0; i<TMR_NUMBER; i++) {
      if(timer[i].state == TMR_RUNNING) {
        timer[i].state = TMR_COMPLETED;
        timer[i].func(timer[i].param);      // Callback to configured function
      }
    }
  }
  else {
    for(int i=0; i<TMR_NUMBER; i++) {
      if(timer[i].state == TMR_RUNNING) {
        if(timer[i].timer_will_expire <= now)
        {
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

  for(int i=0; i<TMR_NUMBER; i++) {
    timer[i].state = TMR_UNUSED;
    timer[i].func  = fnTimerDummyTest;
    timer[i].param = 0;
  }

  fnRebuildTimerRefresh();
}



void fnTimerConfig(uint8_t nr, void(*func)(uint16_t), uint16_t param/*, uint32_t time*/) {
  if(nr < TMR_NUMBER) {
    timer[nr].func  = func;
    timer[nr].param = param;
  //timer[nr].time  = time;
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
  //timer[nr].time = time;
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
