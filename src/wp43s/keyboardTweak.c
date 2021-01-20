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
  #ifdef PC_BUILD
    char tmp[200]; sprintf(tmp,"^^^^showAlphaModeonGui\n"); jm_show_comment(tmp);
  #endif //PC_BUILD
  if(calcMode == CM_AIM || tamMode)    //vv dr JM
  {
    #ifndef TESTSUITE_BUILD
    showHideAlphaMode();
    #endif
    #ifdef PC_BUILD     //dr - new AIM
      calcModeAimGui();
    #endif
  }                                               //^^
doRefreshSoftMenu = true;     //jm
}




void showShiftState(void) {
#ifndef TESTSUITE_BUILD
#ifdef PC_BUILD_TELLTALE
  printf("    >>> showShiftState: calcMode=%d\n",calcMode );
#endif //PC_BUILD_TELLTALE
  if(calcMode != CM_REGISTER_BROWSER && calcMode != CM_FLAG_BROWSER && calcMode != CM_FONT_BROWSER && temporaryInformation != TI_SHOW_REGISTER_BIG && temporaryInformation != TI_SHOW_REGISTER_SMALL && temporaryInformation != TI_SHOW_REGISTER) {
    if(shiftF) {          //SEE screen.c:refreshScreen
      showGlyph(STD_SUP_f, &numericFont, 0, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true); // f is pixel 4+8+3 wide
      show_f_jm();
      showHideAlphaMode();
    }
    else if(shiftG) {     //SEE screen.c:refreshScreen
      showGlyph(STD_SUP_g, &numericFont, 0, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true); // g is pixel 4+10+1 wide
      show_g_jm();
      showHideAlphaMode();
    }
    else {
      showGlyph(" ", &numericFont, 0, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true); // space clears the f and g
      clear_fg_jm();
      showHideAlphaMode();
    }
  }
#endif
}




/********************************************//**  //JM Retain this because of convenience
 * \brief Resets shift keys status and clears the
 * corresponding area on the screen
 *
 * \param void
 * \return void
 *
 ***********************************************/
void resetShiftState(void) {
  if(shiftF || shiftG) {                                                        //vv dr
    shiftF = false;
    shiftG = false;
    showShiftState();
  }                                                                             //^^
}





#ifndef TESTSUITE_BUILD
/********************************************//**
 * \brief Displays the f or g shift state in the
 * upper left corner of the T register line
 *
 * \param void
 * \return void
 ***********************************************/

void show_f_jm(void){
          //showSoftmenuCurrentPart();                                                //JM - Redraw boxes etc after shift is shown
//JMTOCHECK2        if(softmenuStackPointer >= 0) {                                            //JM - Display dot in the f - line
          if(!FN_timeouts_in_progress) {
            if(!ULFL) {
              underline(1);
              ULFL = !ULFL;
              doRefreshSoftMenu = true;
            }
            if(ULGL) {
              underline(2);
              ULGL = !ULGL;
              doRefreshSoftMenu = true;
            }
          }
    //    }                                                                         //JM - Display dot in the f - line
}


void show_g_jm(void){
      //showSoftmenuCurrentPart();                                                //JM - Redraw boxes etc after shift is shown
//JMTOCHECK2        if(softmenuStackPointer >= 0) {                                            //JM - Display dot in the g - line
          if(!FN_timeouts_in_progress) {
            if(ULFL) {
              underline(1);
              ULFL = !ULFL;
              doRefreshSoftMenu = true;
            }
            if(!ULGL) {
              underline(2);
              ULGL = !ULGL;
              doRefreshSoftMenu = true;
            }
          }
  //      }                                                                         //JM - Display dot in the g - line
}


void clear_fg_jm(void){
      //  showSoftmenuCurrentPart();            //JM TO REMOVE STILL !!             //JM - Redraw boxes etc after shift is shown
        if(!FN_timeouts_in_progress) {        //Cancel lines
          if(ULFL) {
            underline(1);
            ULFL = !ULFL;
            doRefreshSoftMenu = true;
          }
          if(ULGL) {
            underline(2);
            ULGL = !ULGL;
            doRefreshSoftMenu = true;
          }
        }

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
              #ifdef PC_BUILD
              //printf("HOME3 %d %d\n",softmenuStack[softmenuStackPointer].softmenu, mm_MNU_HOME);
              jm_show_calc_state("keyboardtweak.c: fg_processing_jm: HOME3");
              #endif //PC_BUILD
              if((softmenuStack[0].softmenuId == mm_MNU_HOME)) {              //JM shifts
                 //printf("popping\n");
                 popSoftmenu();                                                                                                //JM shifts
              }
              else {
                if(calcMode == CM_AIM) {                                                                                      //JM shifts
                  processKeyAction(CHR_num);
                }
                else {                                                                                                        //JM SHIFTS
                  showSoftmenu(-MNU_HOME);                                                                        //JM shifts  //JM ALPHA-HOME
                }                                                                                                             //JM shifts                                                                                                                              //JM shifts
              }
            showSoftmenuCurrentPart();
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
    *result = ITM_EXIT1;                     // EXIT key to exit when done and cancel shifts
  }
  //JM ASSIGN - GET KEY & ASSIGN MEMORY FUNCTION JM_ASN_MODE // JM_ASN_MODE contains the command to be put on a key. 0 if not active
  else if(JM_ASN_MODE != 0) {                // GET KEY
    fnASSIGN(JM_ASN_MODE, tempkey);          // CHECKS FOR INVALID KEYS IN HERE
    JM_ASN_MODE = 0;                         // Catchall - cancel the mode once it had the opportunity to be handled. Whether handled or not.
    *result = ITM_EXIT1;                     // EXIT key to exit when done and cancel shifts
  }
  //JM NORMKEY _ CHANGE NORMAL MODE KEY SIGMA+ TO SOMETHING ELSE vv
  else if((calcMode == CM_NORMAL || calcMode == CM_NIM) && (!getSystemFlag(FLAG_USER) && !shiftF && !shiftG && ( tempkey == 0) && ((kbd_std + 0)->primary == *result) )) {
    *result = Norm_Key_00_VAR;
  }
}


/*
  //JM ASSIGN - GET FUNCTION NUMBER --------------------------------------------------------------------------------
  if(JM_ASN_MODE == 32766) {            //JM Check if JM ASSIGN IS IN PROGRESS AND CAPTURE THE FUNCTION AND KEY TO BE ASSIGNED
                                        //JM GET FUNCTION NUMBER: If seek is pressed, a function can be chosen and pressed.
    JM_ASN_MODE = result;               //JM The result is the function number, item number, asnd is placed in
    fnKEYSELECT();                           //JM Place in auto trigger register, ready for next keypress
    result = ITM_EXIT1;                      //JM EXIT key to exit when done and cancel shifts
  }

  //JM ASSIGN - GET KEY & ASSIGN MEMORY FUNCTION JM_ASN_MODE
                                             //JM JM_ASN_MODE contains the command to be put on a key. 0 if not active
  else if(JM_ASN_MODE != 0) {                //JM GET KEY
    fnASSIGN(JM_ASN_MODE, stringToKeyNumber(data));          //JM CHECKS FOR INVALID KEYS IN HERE
    JM_ASN_MODE = 0;                         //JM Catchall - cancel the mode once it had the opportunity to be handled. Whether handled or not.
    result = ITM_EXIT1;                       //JM EXIT key to exit when done and cancel shifts
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
  ix0 = softmenuStack[0].firstItem;
  ix_sm = softmenu[softmenuStack[0].softmenuId].menuItem;
  
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
      case ITM_BACKSPACE: tmp = ITM_DROP; break;      //Set up backspace double click to DROP
      //case ITM_XEQ      : tmp = -MNU_XXEQ; break;      //XEQ XEQMENU, removed as it does not properly work on double clieck. It still accesses XEQ
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



void Check_MultiPresses(int16_t * result, int8_t key_no){          //Set up longpress
  int16_t longpressDelayedkey1 = 0;
  longpressDelayedkey2 = 0;
  longpressDelayedkey3 = 0;

  if(longpressDelayedkey1 == 0 && (calcMode == CM_NORMAL || calcMode == CM_NIM)) {    //longpress yellow math functions on the first 14 keys
    for(int i=0; i<=16; i++) {      //16 //0=E+  6=STO  12=ENTER 13=X<>Y 14=CHS 15=E 16=BKSPC
      if(key_no == i 
        #ifndef TESTING
          && i!=14       //Do not allow longpress CHS
          && i!=15       //Do not allow longpress EEX as EEX is a leading edge button
        #endif
        && i!=16) {    //Do not allow longpress BKSPC
        if(getSystemFlag(FLAG_USER)) {
          longpressDelayedkey1 = kbd_usr[i].fShifted;
          if(i!=13 && i!=14 && i!=15) longpressDelayedkey3 = kbd_usr[i].gShifted;
        }
        else {
          longpressDelayedkey1 = kbd_std[i].fShifted;
          if(
               i!=12     //Do not allow second longpress ENTER
          	&& i!=13     //Do not allow second longpress x<>y
          #ifndef TESTING
          	&& i!=14     //Do not allow second longpress CHS
          	&& i!=15     //Do not allow second longpress EEX
          #endif
          ) longpressDelayedkey3 = kbd_std[i].gShifted;
        }
        break;
      }
    }
  }                                                                         //yellow and blue function keys ^^

  if(calcMode == CM_NORMAL) {
    switch(*result) {
      case ITM_XEQ      : longpressDelayedkey2=longpressDelayedkey1;   longpressDelayedkey1 = -MNU_XXEQ; break;    //XEQ longpress to XEQMENU 
      case ITM_BACKSPACE: longpressDelayedkey2=longpressDelayedkey1;   longpressDelayedkey1 = ITM_CLSTK; break;    //backspace longpress to CLSTK
      case ITM_EXIT1    :                                              longpressDelayedkey1 = ITM_CLAIM; break;    //EXIT longpress DOES CLAIM
      //case ITM_CHS    :                                              longpressDelayedkey1 = ITM_XexY;  break;     //sample on CHS, operating X<>Y. XEQ must still be created.
      default:;
    }
  } else

	  if(calcMode == CM_NIM) {
	    switch(*result) {
	      case ITM_XEQ      : longpressDelayedkey2=longpressDelayedkey1; longpressDelayedkey1 = -MNU_XXEQ; break;    //XEQ longpress to XEQMENU 
	      case ITM_BACKSPACE:                                            longpressDelayedkey1 = ITM_CLN;   break;    //BACKSPACE longpress clears input buffer
        case ITM_EXIT1    :                                            longpressDelayedkey1 = ITM_CLAIM; break;    //EXIT longpress DOES CLAIM
	      default:;
	    }
	  } else

		  if(calcMode == CM_AIM) {
		    switch(*result) {
          case ITM_BACKSPACE:                                          longpressDelayedkey1 = ITM_CLA;   break;     //BACKSPACE longpress clears input buffer
          case ITM_EXIT1    :                                          longpressDelayedkey1 = ITM_CLAIM; break;     //EXIT longpress DOES CLAIM
          case ITM_ENTER    :                                          longpressDelayedkey1 = ITM_XEDIT; break;
		      default:;
		    }
		  }

  if(longpressDelayedkey1 !=0) {                                      //if activated key pressed 
    JM_auto_longpress_enabled = longpressDelayedkey1;
    fnTimerStart(TO_CL_LONG, TO_CL_LONG, JM_TO_CL_LONG);    //dr

    if(JM_auto_doublepress_enabled != 0) {
       hideFunctionName();    
       undo();
       showFunctionName(JM_auto_doublepress_enabled, FUNCTION_NOPTIME);  //JM CLRDROP
       *result = JM_auto_doublepress_enabled;
       fnTimerStop(TO_CL_DROP);         //JM TIMER CLRDROP ON DOUBLE BACKSPACE
       setSystemFlag(FLAG_ASLIFT);      //JM TIMER CLRDROP ON DOUBLE BACKSPACE
    }
  }

}



//******************* JM LONGPRESS & JM DOUBLE CLICK START *******************************

int16_t nameFunction(int16_t fn, int16_t itemShift) {                       //JM LONGPRESS vv
  int16_t row, func;
  int16_t ix_fn;  //JMXXX
  func = 0;
  const softmenu_t *sm;

//JMTOCHECK2
  if(/*softmenuStackPointer > 0*/true) {
    sm = &softmenu[softmenuStack[0].softmenuId];
    row = min(3, (sm->numItems + modulo(softmenuStack[0].firstItem - sm->numItems, 6))/6 - softmenuStack[0].firstItem/6) - 1;

    if(itemShift/6 <= row && softmenuStack[0].firstItem + itemShift + (fn - 1) < sm->numItems) {
      func = (sm->softkeyItem)[softmenuStack[0].firstItem + itemShift + (fn - 1)];
/*XXX*/
      ix_fn = 0;
      if(func_lookup(fn,itemShift,&ix_fn)) {
        //printf("---%d\n",ix_fn);
        func = ix_fn;
      }
/*XXX*/

      if(func == ITM_PROD_SIGN) {
        func = (getSystemFlag(FLAG_MULTx) ? ITM_DOT : ITM_CROSS);
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
void btnFnPressed_StateMachine(GtkWidget *unused, gpointer data) { 
#endif
#ifdef DMCP_BUILD
void btnFnPressed_StateMachine(void *unused, void *data) {
#endif
  bool_t exexute_double_g;
  bool_t double_click_detected;

//#ifdef INLINE_TEST
//  if(testEnabled) { fnSwStart(1); }     //dr
//#endif

  FN_timed_out_to_RELEASE_EXEC = false;

  //Change states according to PRESS/RELEASE incoming sequence
  if(FN_state == ST_2_REL1) {
    FN_state = ST_3_PRESS2;
  }
  else {
    FN_state =  ST_1_PRESS1;
  }

  if(FN_state == ST_3_PRESS2 && fnTimerGetStatus(TO_FN_EXEC) != TMR_RUNNING) {  //JM BUGFIX (INVERTED) The first  usage did not work due to the timer which was in stopped mode, not in expired mode.
    //----------------Copied here
    char charKey[3];
    underline_softkey(FN_key_pressed-38, 3, false);   //Purposely in row 3 which does not exist, just to activate the clear previous line
    sprintf(charKey, "%c", FN_key_pressed + 11);
    hideFunctionName();

    //IF 2-->3 is longer than double click time, then move back to state 1
    FN_timeouts_in_progress = false;
    FN_state = ST_1_PRESS1;
  }

  FN_key_pressed = *((char *)data) - '0' + 37;                            //to render 38-43, as per original keypress


/* REVISION 0 OF QUICKKEYS. WORKING ##################
Replaced this with the2x chages of jm_G_DOUBLETAP && calcMode != CM_AIM

  //BYPASS LONG/DOUBLE CLICK FOR IDENTIFIED QUICK KEYS, i.e. NAV KEYS, ARROWS ETC.
  int16_t tmp1 = nameFunction(FN_key_pressed-37,0);
  if( FN_state == ST_1_PRESS1 && 
       (   tmp1 == ITM_T_RIGHT_ARROW 
        || tmp1 == ITM_T_LEFT_ARROW 
        || tmp1 == ITM_T_RRIGHT_ARROW 
        || tmp1 == ITM_T_LLEFT_ARROW 
        || tmp1 == ITM_T_DOWN_ARROW
        || tmp1 == ITM_T_UP_ARROW
        ) ) {
     char charKey[3];
     sprintf(charKey, "%c", FN_key_pressed + 11);
     btnFnClicked(w, charKey);                                             //Execute    
     fnTimerStop(TO_FN_EXEC);
     FN_handle_timed_out_to_EXEC=false;
     return;
  }
   ################################################## */


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

  //printf("^^^^ softmenu=%d -MNU_ALPHA=%d currentFirstItem=%d\n", softmenu[softmenuStack[0].softmenuId].menuItem, -MNU_ALPHA, softmenuStack[0].firstItem);
  //**************JM DOUBLE CLICK DETECTION ******************************* // JM FN-DOUBLE
  double_click_detected = false;                                            //JM FN-DOUBLE - Dip detection flag
  if((jm_G_DOUBLETAP && /*calcMode != CM_AIM*/ softmenu[softmenuStack[0].softmenuId].menuItem != -MNU_ALPHA )) {
    if(exexute_double_g) {
      if(FN_key_pressed !=0 && FN_key_pressed == FN_key_pressed_last) {     //Identified valid double press dip, the same key in rapid succession
        shiftF = false;                                                     //JM
        shiftG = true;                                                      //JM
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
    }                                                                       //further shifts are done within FN_handler
  }
//#ifdef INLINE_TEST
//  if(testEnabled) { fnSwStop(1); }      //dr
//#endif
}






//*************----------*************------- FN KEY RELEASED -------***************-----------------
#ifdef PC_BUILD                                                             //JM LONGPRESS FN
void btnFnReleased_StateMachine(GtkWidget *unused, gpointer data) {              //JM LONGPRESS FN
#endif
#ifdef DMCP_BUILD
void btnFnReleased_StateMachine(void *unused, void *data) {
#endif
//#ifdef INLINE_TEST
//  if(testEnabled) { fnSwStart(2); }     //dr
//#endif

  if(FN_state == ST_3_PRESS2) {
    FN_state =  ST_4_REL2;
  }
  else {
    FN_state =  ST_2_REL1;
  }

  if((jm_G_DOUBLETAP && /*calcMode != CM_AIM*/ softmenu[softmenuStack[0].softmenuId].menuItem != -MNU_ALPHA) && FN_state == ST_2_REL1 && FN_handle_timed_out_to_EXEC) {
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

    if(!FN_timed_out_to_NOP && fnTimerGetStatus(TO_FN_EXEC) != TMR_RUNNING) {
      btnFnClicked(unused, charKey);                                             //Execute
    }

   if (!(calcMode == CM_REGISTER_BROWSER || calcMode == CM_FLAG_BROWSER || calcMode == CM_FONT_BROWSER || calcMode == CM_GRAPH  || calcMode == CM_LISTXY)) {
     if(FN_timed_out_to_NOP) showSoftmenuCurrentPart();                           //Clear any possible underline residues
   }


    resetShiftState();
    FN_cancel();
  }
//**************JM LONGPRESS AND JM DOUBLE ^^ *********************************************   // JM FN-DOUBLE
//#ifdef INLINE_TEST
//  if(testEnabled) { fnSwStop(2); }      //dr
//#endif
}



void execFnTimeout(uint16_t key) {                          //dr - delayed call of the primary function key
  char charKey[3];
  sprintf(charKey, "%c", key + 11);

  btnFnClicked(NULL, (char *)charKey);
}



void shiftCutoff(uint16_t unusedButMandatoryParameter) {        //dr - press shift three times within one second to call HOME timer
  fnTimerStop(TO_3S_CTFF);
}


//---------------------------------------------------------------


#endif //TESTSUITE_BUILD




uint16_t numlockReplacements(uint16_t item, bool_t NL, bool_t SHFT) {
  uint16_t item1 = 0;
  if(ITM_A +26 <= item && item <= ITM_Z +26) 
    {if(keyReplacements(item-26, &item1, NL, SHFT)) return item1;else return item;}
  else
    {if(keyReplacements(item, &item1, NL, SHFT)) return item1;else return item;}
}


//Note item1 MUST be set to 0 prior to calling.
bool_t keyReplacements(uint16_t item, uint16_t * item1, bool_t NL, bool_t SHFT) {

                if(NL && !SHFT) {                           //JMvv Numlock translation: Assumes lower case  is NOT active
                  switch(item) {
                    case ITM_P             : * item1 = ITM_7;      break;
                    case ITM_Q             : * item1 = ITM_8;      break;
                    case ITM_R             : * item1 = ITM_9;      break;
                    case ITM_T             : * item1 = ITM_4;      break;
                    case ITM_U             : * item1 = ITM_5;      break;
                    case ITM_V             : * item1 = ITM_6;      break;
                    case ITM_X             : * item1 = ITM_1;      break;
                    case ITM_Y             : * item1 = ITM_2;      break;
                    case ITM_Z             : * item1 = ITM_3;      break;
                    case CHR_num           : * item1 = 0;          break;
                    case CHR_case          : * item1 = 0;          break;
                    case ITM_O             : * item1 = ITM_EEXCHR; break; //STD_SUB_E_OUTLINE
                    case ITM_S             : * item1 = ITM_OBELUS; break;
                    case ITM_W             : * item1 = ITM_MULT;   break;
                    case ITM_UNDERSCORE    : * item1 = ITM_MINUS;  break;
                    case ITM_SPACE         : * item1 = ITM_PLUS;   break;
                    case ITM_QUESTION_MARK : * item1 = ITM_SLASH;  break;
                    case ITM_COMMA         : * item1 = ITM_PERIOD; break;
                    case ITM_COLON         : * item1 = ITM_0;      break;

                    default: 
                         #ifdef PC_BUILD
                           jm_show_comment("^^^^processKeyAction1/keyReplacements:CM_AIM: Numlock active but number not handled");
                         #endif //PC_BUILD
//                       item1 = item;     //this is the non-number character which is now handled below.
                         break;
                  }
                } else

                if(NL && SHFT) {                           //JMvv Numlock translation: Assumes lower case  is NOT active
                  switch(item) {
                    case ITM_MINUS  : * item1 = ITM_UNDERSCORE   ;  break;
                    case ITM_PLUS   : * item1 = ITM_SPACE        ;  break;
                    case ITM_SLASH  : * item1 = ITM_QUESTION_MARK;  break;
                    case ITM_PERIOD : * item1 = ITM_COMMA        ;  break;
                    case ITM_0      : * item1 = ITM_COLON        ;  break;

                    default: 
                         #ifdef PC_BUILD
                           jm_show_comment("^^^^processKeyAction2/keyReplacements:CM_AIM: Numlock active but number not handled");
                         #endif //PC_BUILD
//                       item1 = item;     //this is the non-number character which is now handled below.
                         break;
                  }
                } else//JM Exception, to change 0 to ";", when !NL & SHFT-0

                if(!NL && SHFT) {                           //JMvv Numlock translation: Assumes lower case  is NOT active
                  switch(item) {
                    case ITM_0      : * item1 = ITM_SEMICOLON        ;  break;
                    default: 
                         #ifdef PC_BUILD
                           jm_show_comment("^^^^processKeyAction3/keyReplacements:CM_AIM: Numlock active but number not handled");
                         #endif //PC_BUILD
//                       item1 = item;     //this is the non-number character which is now handled below.
                         break;
                  }
                }


                return *item1 != 0;
}






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



#ifdef DMCP_BUILD                                           //vv dr - internal keyBuffer POC

//#define JMSHOWCODES_KB0   // top line left    Key value from DM42. Not necessarily pushed to buffer.
//#define JMSHOWCODES_KB1   // top line middle  Key and  dT value
//#define JMSHOWCODES_KB2   // main screen      Telltales keys, times, etc.
//#define JMSHOWCODES_KB3   // top line right   Single Double Triple

void keyBuffer_pop()
{
  int tmpKey;

  do {
    tmpKey = -1;
    if(!fullKeyBuffer()) {
      tmpKey = key_pop();
      if(tmpKey >= 0) {
        inKeyBuffer(tmpKey);
      }
    }
  } while (tmpKey >= 0);
}


#ifdef JMSHOWCODES_KB0
  uint16_t tmpxx = 1;
#endif
#ifdef BUFFER_CLICK_DETECTION
  kb_buffer_t buffer = {{}, {}, 0, 0};
#else
  kb_buffer_t buffer = {{}, 0, 0};
#endif
//
// Stellt 1 Byte in den Ringbuffer
//
// Returns:
//     BUFFER_FAIL       der Ringbuffer ist voll. Es kann kein weiteres Byte gespeichert werden
//     BUFFER_SUCCESS    das Byte wurde gespeichert
//
uint8_t inKeyBuffer(uint8_t byte)
{
  #ifdef BUFFER_CLICK_DETECTION
    uint32_t now  = (uint32_t)sys_current_ms();
  #endif
  uint8_t  next = ((buffer.write + 1) & BUFFER_MASK);

  if(buffer.read == next) {

    return BUFFER_FAIL;  // voll
  }


// EXPERIMENT Do not allow the same key to be stored multiple times. Only key changes stored.
  if(buffer.data[(buffer.write - 1) & BUFFER_MASK] == byte) {
  #ifdef JMSHOWCODES_KB0
    char aaa[12];
    sprintf   (aaa,"%2d ",byte);
    showString(aaa,&standardFont, tmpxx++, 1, vmNormal, true, true);
  #endif       

    return BUFFER_FAIL;  // doppelt
  }
// END EXPERIMENT

  #ifdef JMSHOWCODES_KB0
    tmpxx = 1;
  #endif
  buffer.data[buffer.write & BUFFER_MASK] = byte;
  #ifdef BUFFER_CLICK_DETECTION
    buffer.time[buffer.write & BUFFER_MASK] = now;
  #endif
  buffer.write = next;

  return BUFFER_SUCCESS;
}



//
// Holt 1 Byte aus dem Ringbuffer, sofern mindestens eines abholbereit ist
//
// Returns:
//     BUFFER_FAIL       der Ringbuffer ist leer. Es kann kein Byte geliefert werden.
//     BUFFER_SUCCESS    1 Byte wurde geliefert
//
#ifdef BUFFER_CLICK_DETECTION
#ifdef BUFFER_KEY_COUNT
uint8_t outKeyBuffer(uint8_t *pKey, uint8_t *pKeyCount, uint32_t *pTime, uint32_t *pTimeSpan_1, uint32_t *pTimeSpan_B)
#else
uint8_t outKeyBuffer(uint8_t *pKey, uint32_t *pTime, uint32_t *pTimeSpan_1, uint32_t *pTimeSpan_B)
#endif
#else
#ifdef BUFFER_KEY_COUNT
uint8_t outKeyBuffer(uint8_t *pKey, uint8_t *pKeyCount)
#else
uint8_t outKeyBuffer(uint8_t *pKey)
#endif
#endif
{
  if(buffer.read == buffer.write) {

    return BUFFER_FAIL;  // leer
  }

  uint8_t actKey = buffer.data[buffer.read];
  *pKey = actKey;
#ifdef BUFFER_KEY_COUNT
  uint8_t keyCount = 0;
  if(actKey > 0) {
    keyCount++;
    if(buffer.data[(buffer.read - 2) & BUFFER_MASK] == actKey) {
      keyCount++;
      if(buffer.data[(buffer.read - 4) & BUFFER_MASK] == actKey) {
        keyCount++;
      }
    }
  }
  *pKeyCount = keyCount;
#endif

#ifdef BUFFER_CLICK_DETECTION
  uint32_t actTime = buffer.time[buffer.read];
  *pTime = actTime;
  uint32_t oldTime;
  
  oldTime = buffer.time[(buffer.read - 1) & BUFFER_MASK];
  if(actTime >= oldTime) {
    *pTimeSpan_1 = actTime - oldTime;
  }
  else {            // protect uint overflow 
    *pTimeSpan_1 = actTime;
  }

#ifdef BUFFER_KEY_COUNT
  if(keyCount > 2) {
    oldTime = buffer.time[(buffer.read - 4) & BUFFER_MASK];
  }
  else {
#else
  {
#endif
    oldTime = buffer.time[(buffer.read - 2) & BUFFER_MASK];
  }
  if(actTime >= oldTime) {
    *pTimeSpan_B = actTime - oldTime;
  }
  else {            // protect uint overflow 
    *pTimeSpan_B = actTime;
  }
#endif
  
  buffer.read = (buffer.read + 1) & BUFFER_MASK;

#ifdef BUFFER_CLICK_DETECTION
  uint8_t detectionResult = outKeyBufferDoubleClick();
#ifdef JMSHOWCODES_KB1
  fnDisplayStack(2);
  char aaa[50];
  sprintf (aaa,"k=%2d dT=%5lu:%d",*pByte, *pTimeSpan, (uint8_t)tmpTime);
  showString(aaa, &standardFont, 220, 1, vmNormal, true, true);
#endif
#endif
  
  return BUFFER_SUCCESS;
}



/* Switching profile, single press, double press and triple press:


SINGLEPRESS:
unknown  waiting   pres    rel
---D3--x_____D2____x---D1--x_
      t-3         t-2     t-1


DOUBLEPRESS:
unknown  waiting   pres    rel    pres
-------x___________x-------x______x-
      t-4    D3   t-3  D2 t-2 D1 t-1


TRIPLEPRESS:
unknown  waiting   pres    rel    pres   rel    pres
-------x___________x-------x______x------x______x-
                          t-4 D3 t-3 D2 t-2 D1 t-1



Circular key buffer:

 Pointers are ready to read and ready to write.
 Pointers are incremented after action.
 Allow writing of 3 keys ahead, into: R+0, R+1, R+2. R+3 is blocked.
 Allow reading of 3 keys up to R=W, which is not read.

 +------------------------------------+
 |   1                                | emptyKeyBuffer, not used
 | R 2 W   R=W: Empty buffer, cannot  | outKeyBuffer
 |   3          read.                 |
 |   4                                |    (Will not pop a key which is not pushed. Can look at 4 places back directly at the stack. )
 +------------------------------------+
 |   1 W   W+1 NOT= R: Space for 1+   | isMoreBufferSpace, used by keyBuffer_pop writing to the buffer only when there is space
 | R 2                                |
 |   3                                |
 |   4                                |
 +------------------------------------+
 |   1                                | inKeyBuffer
 | R 2                                |
 |   3 W   R+1=W: Write buffer full.  |    (must avoid this one, because the key is lost then. Must rather test first and keep the key in the DM42 buffer)
 |   4            Failed. Return      |
 +------------------------------------+

 +-------------------------------------------------------------------------------+
 |   1 W    X     iv.block access to inKeyBuffer, i.e. keep key in DM42 buffer.  |
 | R 2 ov      i.write o and move down v                                         |
 |   3 o v      ii.write o and move down v                                       |
 |   4 o  v      iii.write o and move down v                                     |
 +-------------------------------------------------------------------------------+

*/



// Returns: true if double click
uint8_t outKeyBufferDoubleClick()
{
#ifdef BUFFER_CLICK_DETECTION
     //WARNING! this triggers conseq double click to be 'triple' click but does not check it is the SAME key. 
     //Buffer of 4 to short for that. Buffer of 8 sufficient.
     //Can be fixed by having a single byte added to the rolling stack catching the key which was rolled out

  int16_t dTime_1, dTime_2, dTime_3;
  bool_t  doubleclicked, tripleclicked;
  uint8_t outDoubleclick;

  //note Delta Time 1 is the most recent, Delta time 3 is the oldest
  dTime_1 = (uint16_t) buffer.time[(buffer.read-1) & BUFFER_MASK] - (uint16_t) buffer.time[(buffer.read-2) & BUFFER_MASK];
  dTime_2 = (uint16_t) buffer.time[(buffer.read-2) & BUFFER_MASK] - (uint16_t) buffer.time[(buffer.read-3) & BUFFER_MASK];
  dTime_3 = (uint16_t) buffer.time[(buffer.read-3) & BUFFER_MASK] - (uint16_t) buffer.time[(buffer.read-4) & BUFFER_MASK];

  #define D1 150 //400 //space before last press, released time
  #define D2 200 //length of first press, pressed down time

  doubleclicked = 
         buffer.data[(buffer.read-1) & BUFFER_MASK] != 0   //check that the last incoming keys was a press, not a release
      && buffer.data[(buffer.read-1) & BUFFER_MASK] == buffer.data[(buffer.read-3) & BUFFER_MASK]   //check that the two last keys are the same, otherwise itis a glisando 
      && (dTime_1 > 10 ) && (dTime_1 < D1)                //check no chatter > 10 ms & released width is not longer than limit
      && (dTime_2 > 10 ) && (dTime_2 < D2);               //check no chatter > 10 ms & pressed width is not longer than limit

  if(dTime_1+dTime_2 > D1+D2) doubleclicked = false;


  #define TD1 150 //space before last press, released time
  #define TD2 200 //length of middle press, pressed down time
  #define TD3 150 //space before middle press, i.e. released time after first press

  tripleclicked = 
         buffer.data[(buffer.read-1) & BUFFER_MASK] != 0   //check that the last incoming keys was a press, not a release
      && buffer.data[(buffer.read-1) & BUFFER_MASK] == buffer.data[(buffer.read-3) & BUFFER_MASK]   //check that the two last keys are the same, otherwise itis a glisando 
      && buffer.data[(buffer.read-1) & BUFFER_MASK] == buffer.data[(buffer.read-5) & BUFFER_MASK]   //check that the previous key is the same. If buffer is 4 long only, it will wrap and not check the first triple press
      && (dTime_1 > 10 ) && (dTime_1 < TD1)                //check no chatter > 10 ms & released width is not longer than limit
      && (dTime_2 > 10 ) && (dTime_2 < TD2)                //check no chatter > 10 ms & pressed width is not longer than limit
      && (dTime_3 > 10 ) && (dTime_3 < TD3);               //check no chatter > 10 ms & pressed width is not longer than limit

  if(dTime_1+dTime_2+dTime_3 > TD1+TD2+TD3) tripleclicked = false;

  if(tripleclicked) outDoubleclick = 3; else
  if(doubleclicked) outDoubleclick = 2; else
  if(buffer.data[(buffer.read-1) & BUFFER_MASK] != 0) outDoubleclick = 1; 
  else outDoubleclick = 0;


  #ifdef JMSHOWCODES_KB2 
    char line[50], line1[100];
    fnDisplayStack(2);
    sprintf(line1,"R%-1dW%-1d -1:%-5d -2:%-5d -3:%-5d -4:%-5d  ",
      (uint16_t)buffer.read,(uint16_t)buffer.write,
      (uint16_t)buffer.time[(buffer.read-1) & BUFFER_MASK], (uint16_t)buffer.time[(buffer.read-2) & BUFFER_MASK],(uint16_t)buffer.time[(buffer.read-3) & BUFFER_MASK],(uint16_t)buffer.time[(buffer.read-4) & BUFFER_MASK] );
    showString(line1, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_T - REGISTER_X), vmNormal, true, true);
    sprintf(line,"B-1:%d %d %d %d",(uint16_t)buffer.data[(buffer.read-1) & BUFFER_MASK], (uint16_t)buffer.data[(buffer.read-2) & BUFFER_MASK],(uint16_t)buffer.data[(buffer.read-3) & BUFFER_MASK],(uint16_t)buffer.data[(buffer.read-4) & BUFFER_MASK] );
    sprintf(line1,"%-16s   D1:%d D2:%d D3:%d    ",       
      line,dTime_1, dTime_2, dTime_3);
    showString(line1, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Z - REGISTER_X), vmNormal, true, true);
  #endif

  #ifdef JMSHOWCODES_KB3
    char line2[10];
    line2[0]=0;
    if( outDoubleclick == 1) {
      strcat(line2,"S");
      showString(line2, &standardFont, SCREEN_WIDTH-11, 0, vmNormal, true, true);
    }
    else 
    if( outDoubleclick == 2) {
      strcat(line2,"D");
      showString(line2, &standardFont, SCREEN_WIDTH-11, 0, vmNormal, true, true);
    }
    else 
    if( outDoubleclick == 3) {
      strcat(line2,"T");
      showString(line2, &standardFont, SCREEN_WIDTH-11, 0, vmNormal, true, true);
    }
    else {
      strcat(line2," ");
      showString(line2, &standardFont, SCREEN_WIDTH-11, 0, vmNormal, true, true);
      //refreshStatusBar();
    }
  #endif

  return outDoubleclick;
#else
  return 255;
#endif
}



// Returns:
//     true              der Ringbuffer ist voll
bool_t fullKeyBuffer()
{
  return buffer.read == ((buffer.write + 1) & BUFFER_MASK);
}


// Returns:
//     true              der Ringbuffer ist leer
bool_t emptyKeyBuffer()
{
  return buffer.read == buffer.write;
}
#endif                                                      //^^



//########################################

void fnT_ARROW(uint16_t command) {
#ifndef TESTSUITE_BUILD
  uint16_t ixx;
  uint16_t current_cursor_x_old;
  uint16_t current_cursor_y_old;

  #ifdef PC_BUILD
    char tmp[200]; sprintf(tmp,"^^^^fnT_ARROW: command=%d current_cursor_x=%d current_cursor_y=%d \n",command,current_cursor_x, current_cursor_y); jm_show_comment(tmp);
  #endif //PC_BUILD


  switch (command) {

     case ITM_T_LEFT_ARROW /*STD_LEFT_ARROW */ : 
        T_cursorPos = stringPrevGlyph(aimBuffer, T_cursorPos);
        break;

     case ITM_T_RIGHT_ARROW /*STD_RIGHT_ARROW*/ : 

        T_cursorPos = stringNextGlyph(aimBuffer, T_cursorPos);
        break;

     case ITM_T_LLEFT_ARROW /*STD_FARLEFT_ARROW */ :
        ixx = 0;
        while(ixx<10) {
          fnT_ARROW(ITM_T_LEFT_ARROW);
          ixx++;
        }
        break;

     case ITM_T_RRIGHT_ARROW /*STD_FARRIGHT_ARROW*/ :
        ixx = 0;
        while(ixx<10) {
          fnT_ARROW(ITM_T_RIGHT_ARROW);
          ixx++;
        }
        break;


     case ITM_T_UP_ARROW /*UP */ :                      //JMCURSOR try make the cursor upo be more accurate. Add up the char widths...
        ixx = 0;
        current_cursor_x_old = current_cursor_x;
        current_cursor_y_old = current_cursor_y;
        fnT_ARROW(ITM_T_RIGHT_ARROW);
        while(ixx < 75 && (current_cursor_x >= current_cursor_x_old+5 || current_cursor_y == current_cursor_y_old)) {
          fnT_ARROW(ITM_T_LEFT_ARROW);
          showStringEdC43(lines ,displayAIMbufferoffset, T_cursorPos, aimBuffer, 1, -100, vmNormal, true, true, true);  //display up to the cursor

          //printf("###^^^ %d %d %d %d %d\n",ixx,current_cursor_x, current_cursor_x_old, current_cursor_y, current_cursor_y_old);
          ixx++;
        }
        break;


     case ITM_T_DOWN_ARROW /*DN*/ :
        ixx = 0;
        current_cursor_x_old = current_cursor_x;
        current_cursor_y_old = current_cursor_y;
        fnT_ARROW(ITM_T_LEFT_ARROW);
        while(ixx < 75 && (current_cursor_x+5 <= current_cursor_x_old || current_cursor_y == current_cursor_y_old)) {
          fnT_ARROW(ITM_T_RIGHT_ARROW);
          showStringEdC43(lines ,displayAIMbufferoffset, T_cursorPos, aimBuffer, 1, -100, vmNormal, true, true, true);  //display up to the cursor
          ixx++;

          //printf("###^^^ %d %d %d %d %d\n",ixx,current_cursor_x, current_cursor_x_old, current_cursor_y, current_cursor_y_old);
        }
        break;



     case ITM_UP1 /*HOME */ :
        T_cursorPos = 0;
        break;


     case ITM_DOWN1 /*END*/ :
        T_cursorPos = stringByteLength(aimBuffer) - 1;
        T_cursorPos = stringNextGlyph(aimBuffer, T_cursorPos);
        fnT_ARROW(ITM_T_RIGHT_ARROW);
        break;


     default: break;
  }
  //printf(">>> T_cursorPos %d",T_cursorPos);
  if(T_cursorPos > stringNextGlyph(aimBuffer, stringLastGlyph(aimBuffer))) T_cursorPos = stringNextGlyph(aimBuffer, stringLastGlyph(aimBuffer));
  if(T_cursorPos < 0) T_cursorPos = 0;
  //printf(">>> T_cursorPos limits %d\n",T_cursorPos);
#endif
}



