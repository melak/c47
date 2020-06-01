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
 * \file keyboard.c Keyboard management
 ***********************************************/

#include "wp43s.h"

#ifndef TESTSUITE_BUILD
/********************************************//**
 * \brief Displays the f or g shift state in the
 * upper left corner of the T register line
 *
 * \param void
 * \return void
 ***********************************************/
void showShiftState(void) {
  if(calcMode != CM_REGISTER_BROWSER && calcMode != CM_FLAG_BROWSER && calcMode != CM_FLAG_BROWSER_OLD && calcMode != CM_FONT_BROWSER) {
//  if(shiftStateChanged) {                                                     //dr
      if(shiftF) {
        showGlyph(STD_SUP_f, &numericFont, 0, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true); // f is pixel 4+8+3 wide
        show_f_jm();        //JM KeyboardTweaks.c
      }
      else if(shiftG) {
        showGlyph(STD_SUP_g, &numericFont, 0, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true); // g is pixel 4+10+1 wide
        show_g_jm();        //JM KeyboardTweaks.c
      }
      else {
        refreshRegisterLine(REGISTER_T);
        clear_fg_jm();      //JM KeyboardTweaks.c
        if(TAM_REGISTER_LINE == REGISTER_T && (calcMode == CM_TAM || calcMode == CM_ASM || calcMode == CM_ASM_OVER_TAM || calcMode == CM_ASM_OVER_AIM)) {
          showString(tamBuffer, &standardFont, 25, Y_POSITION_OF_TAM_LINE + 6, vmNormal, true, true);
        }
      }
//    shiftStateChanged = false;                                                //vv dr
//  }                                                                           //^^
  }
}




/********************************************//**
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



int16_t determineFunctionKeyItem(const char *data) {
  int16_t row, item = ITM_NOP;
  const softmenu_t *sm;
  int16_t itemShift, fn = *(data) - '0';

  if(shiftF) {
    itemShift = 6;
  }
  else if(shiftG) {
    itemShift = 12;
  }
  else {
    itemShift = 0;
  }

  if(softmenuStackPointer > 0) {
    sm = &softmenu[softmenuStack[softmenuStackPointer - 1].softmenu];
    row = min(3, (sm->numItems + modulo(softmenuStack[softmenuStackPointer - 1].firstItem - sm->numItems, 6))/6 - softmenuStack[softmenuStackPointer - 1].firstItem/6) - 1;

    if(itemShift/6 <= row && softmenuStack[softmenuStackPointer - 1].firstItem + itemShift + (fn - 1) < sm->numItems) {
      item = (sm->softkeyItem)[softmenuStack[softmenuStackPointer - 1].firstItem + itemShift + (fn - 1)];

      if(item > 0) {          //TO CHECK
        item %= 10000;
      }

      int16_t ix_fn;                            //JMXXX
      ix_fn = 0;                                /*JMEXEC XXX vv*/
      if(func_lookup(fn,itemShift,&ix_fn)) {
        //printf("---%d\n",ix_fn);
        item = ix_fn;
      }                                         /*JMEXEC XXX ^^*/

      if(item == CHR_PROD_SIGN) {
        item = (getSystemFlag(FLAG_MULTx) ? CHR_DOT : CHR_CROSS);
      }


    }
  }
  else {
    switch(fn) {
      //JM FN KEYS DIRECTLY ACCESSIBLE IF NO MENUS ARE UP;                       // FN Key will be the same as the yellow label underneath it, even if USER keys were selected.
      case 1: {resetTemporaryInformation(); item = ( ITM_SIGMAMINUS ) ;} break;  //ITM_pi
      case 2: {resetTemporaryInformation(); item = ( !getSystemFlag(FLAG_USER) ? (kbd_std[1].fShifted) : (kbd_usr[1].fShifted) ) ;} break;  //Function key follows if the yellow key top 4 buttons are changed from default.
      case 3: {resetTemporaryInformation(); item = ( !getSystemFlag(FLAG_USER) ? (kbd_std[2].fShifted) : (kbd_usr[2].fShifted) ) ;} break;  //Function key follows if the yellow key top 4 buttons are changed from default.
      case 4: {resetTemporaryInformation(); item = ( !getSystemFlag(FLAG_USER) ? (kbd_std[3].fShifted) : (kbd_usr[3].fShifted) ) ;} break;  //Function key follows if the yellow key top 4 buttons are changed from default.
      case 5: {resetTemporaryInformation(); item = ( !getSystemFlag(FLAG_USER) ? (kbd_std[4].fShifted) : (kbd_usr[4].fShifted) ) ;} break;  //Function key follows if the yellow key top 4 buttons are changed from default.
      case 6: {resetTemporaryInformation(); item = ( ITM_XFACT ) ;} break;       //ITM_XTHROOT
      default:{item = 0;} break;
    }
  }

  return item;
}


/********************************************//**
 * \brief Simulate a function key click.
 *
 * \param w GtkWidget* The button to pass to btnFnPressed and btnFnReleased
 * \param data gpointer String containing the key ID
 * \return void
 //JM btnFnClicked is called by gui.c keyPressed
 ***********************************************/
#ifdef PC_BUILD
void btnFnClicked(GtkWidget *w, gpointer data) {
#endif
#ifdef DMCP_BUILD
void btnFnClicked(void *w, void *data) {
#endif
//  int16_t fn = *((char *)data) - '0';

  if(calcMode != CM_CONFIRMATION) {
    allowScreenUpdate = true;

    if(lastErrorCode != 0) {
      lastErrorCode = 0;
      refreshStack();
    }
    executeFunction(data);
  }
}



/********************************************//**
 * \brief A calc function key was pressed
 *
 * \param w GtkWidget*
 * \param data gpointer pointer to a string containing the key number pressed: 00=1/x, ..., 36=EXIT
 * \return void
 ***********************************************/
#ifdef PC_BUILD
void btnFnPressed(GtkWidget *notUsed, gpointer data) {
#endif
#ifdef DMCP_BUILD
void btnFnPressed(void *notUsed, void *data) {
#endif
  int16_t item = determineFunctionKeyItem((char *)data);

  if(item != ITM_NOP /*&& item != ITM_NULL*/) {            //JM still need to run the longpress even if no function populated in FN, ie NOP or NULL
//    resetShiftState();                                 //JM still need the shifts active prior to cancelling them

    if(lastErrorCode != 0) {
      lastErrorCode = 0;
      refreshStack();
    }

//    #if(FN_KEY_TIMEOUT_TO_NOP == 1)                    //JM vv Rmove the possibility for error
//      showFunctionName(item, 10);
//    #else
//      showFunctionNameItem = item;
        btnFnPressed_StateMachine(notUsed, data);        //JM ^^ This calls original state analysing btnFnPressed routing, which is now renamed to "statemachine" in keyboardtweaks
//    #endif
  }
  else {
    showFunctionNameItem = ITM_NOP;
  }
}



/********************************************//**
 * \brief A calc function key was released
 *
 * \param w GtkWidget*
 * \param data gpointer pointer to a string containing the key number pressed: 00=1/x, ..., 36=EXIT
 * \return void
 ***********************************************/
#ifdef PC_BUILD
void btnFnReleased(GtkWidget *w, gpointer data) {
#endif
#ifdef DMCP_BUILD
void btnFnReleased(void *w, void *data) {
#endif

  btnFnReleased_StateMachine(w, data);

}



/********************************************//**
 * \brief Executes one function from a softmenu
 * \return void
 ***********************************************/
void executeFunction(const char *data) {
  int16_t item = ITM_NOP;
  item = determineFunctionKeyItem((char *)data);
  resetShiftState();


  //printf("%d--\n",calcMode);
  if(calcMode != CM_CONFIRMATION) {
    allowScreenUpdate = true;

    if(lastErrorCode != 0) {
      lastErrorCode = 0;
      refreshStack();
    }


//************************************************** TOCHECK vv 

      if(softmenuStackPointer > 0) {
        if(calcMode == CM_ASM) {
          calcModeNormal();
        }
        else if(calcMode == CM_ASM_OVER_TAM) {
          indexOfItems[getOperation()].func(indexOfItems[item].param);
          calcModeNormal();
          return;
        }
        else if(calcMode == CM_ASM_OVER_AIM) {
          calcMode = CM_AIM;
          addItemToBuffer(item);
          calcMode = CM_ASM_OVER_AIM;
          return;
        }
      }

// TO CHECK TOCHECK (2)
//        else if(calcMode == CM_ASM_OVER_AIM) {
//          calcModeAim(NOPARAM);
//          addItemToBuffer(item);
//          return;
//        }
//************************************************* TOCHECK ^^
      {
        if(item < 0) { // softmenu
          if(item != -MNU_SYSFL || calcMode != CM_TAM || transitionSystemState == 0) {
            showSoftmenu(NULL, item, true);
          }
        }
        else if((calcMode == CM_NORMAL || calcMode == CM_NIM) && (CHR_0<=item && item<=CHR_F)) {
          addItemToNimBuffer(item);
        }
        else if(calcMode == CM_TAM) {
          addItemToBuffer(item);
        }
        else if(item > 0) { // function
          if(calcMode == CM_NIM && item != KEY_CC) {
            closeNim();
            if(calcMode != CM_NIM) {
              if(indexOfItems[item].func == fnConstant) {
                STACK_LIFT_ENABLE;
              }
            }
          }

          if(lastErrorCode == 0) {
            resetTemporaryInformation();
            runFunction(item);
          }
        }
      }
    }
  }



#define stringToKeyNumber(data)         ((*((char *)data) - '0')*10 + *(((char *)data)+1) - '0')

int16_t determineItem(const char *data) {
  int16_t result;
  const calcKey_t *key;

  //key = getSystemFlag(FLAG_USER) ? (kbd_usr + (*data - '0')*10 + *(data+1) - '0') : (kbd_std + (*data - '0')*10 + *(data+1) - '0');
  key = getSystemFlag(FLAG_USER) && ((calcMode == CM_NORMAL) || (calcMode == CM_AIM) || (calcMode == CM_NIM)) ? (kbd_usr + stringToKeyNumber(data)) : (kbd_std + stringToKeyNumber(data));    //JM Added (calcMode == CM_NORMAL) to prevent user substitution in AIM and TAM

  allowScreenUpdate = true;

  fnTimerExec(TO_FN_EXEC);                                  //dr execute queued fn

  switch(key->primary) {                              //JMSHOW vv
    case      KEY_UP1:
    case      KEY_DOWN1: break;                       //JM SHOWregis unchanged
    default:  SHOWregis = 9999; break;     
  }                                                   //JMSHOW ^^
  //printf("###\n"); //JMEXEC

  Setup_MultiPresses( key->primary );



  // JM Shift f pressed  //JM shifts change f/g to a single function key toggle to match DM42 keyboard
  // JM Inserted new section and removed old f and g key processing sections
  if(key->primary == KEY_fg && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_TAM || calcMode == CM_NIM || calcMode == CM_ASM || calcMode == CM_ASM_OVER_TAM || calcMode == CM_ASM_OVER_AIM)) {   //JM shifts
    Shft_timeouts = true;                         //JM SHIFT NEW
    fnTimerStart(TO_FG_LONG, TO_FG_LONG, JM_TO_FG_LONG);    //vv dr
    if(ShiftTimoutMode) {
      fnTimerStart(TO_FG_TIMR, TO_FG_TIMR, JM_SHIFT_TIMER); //^^
    }
    resetTemporaryInformation();
                                                                                                                              //JM shifts
    if(lastErrorCode != 0) {                                                                                                  //JM shifts
      lastErrorCode = 0;                                                                                                      //JM shifts
      refreshStack();                                                                                                         //JM shifts
    }                                                                                                                         //JM shifts

    fg_processing_jm();

    showShiftState();                                                                                                         //JM shifts

    return ITM_NOP;

  }                                                                                                                           //JM shifts


  // Shift f pressed and JM REMOVED shift g not active
  else if(key->primary == KEY_f && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_TAM || calcMode == CM_NIM || calcMode == CM_ASM || calcMode == CM_ASM_OVER_TAM || calcMode == CM_ASM_OVER_AIM)) {
    resetTemporaryInformation();

    if(lastErrorCode != 0) {
      lastErrorCode = 0;
      refreshStack();
    }

    fnTimerStop(TO_FG_LONG);                                //dr
    fnTimerStop(TO_FG_TIMR);                                //dr

    shiftF = !shiftF;
    shiftG = false;                                         //JM no shifted menu on g-shift-key as in WP43S
//    shiftStateChanged = true; //JM

    showShiftState();

    return ITM_NOP;
  }

  // Shift g pressed and JM REMOVED shift f not active
  else if(key->primary == KEY_g && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_TAM || calcMode == CM_NIM || calcMode == CM_ASM || calcMode == CM_ASM_OVER_TAM || calcMode == CM_ASM_OVER_AIM)) {
    resetTemporaryInformation();

    if(lastErrorCode != 0) {
      lastErrorCode = 0;
      refreshStack();
    }

    fnTimerStop(TO_FG_LONG);                                //dr
    fnTimerStop(TO_FG_TIMR);                                //dr

    shiftG = !shiftG;
    shiftF = false;                                         //JM no shifted menu on g-shift-key as in WP43S
//    shiftStateChanged = true; //JM

    showShiftState();

    return ITM_NOP;
  }

  if(calcMode == CM_NORMAL || calcMode == CM_NIM || calcMode == CM_FONT_BROWSER || calcMode == CM_FLAG_BROWSER || calcMode == CM_FLAG_BROWSER_OLD || calcMode == CM_REGISTER_BROWSER || calcMode == CM_BUG_ON_SCREEN || calcMode == CM_CONFIRMATION) {
    result = shiftF ? key->fShifted :
             shiftG ? key->gShifted :
                      key->primary;
  }
  else if(calcMode == CM_AIM || calcMode == CM_ASM || calcMode == CM_ASM_OVER_TAM || calcMode == CM_ASM_OVER_AIM) {
    result = shiftF ? key->fShiftedAim :
             shiftG ? key->gShiftedAim :
                      key->primaryAim;

  }
  else if(calcMode == CM_TAM) {
    result = key->primaryTam; // No shifted function in TAM
  }
  else {
    displayBugScreen("In function determineItem: item was not determined!");
    result = 0;
  }

  Check_Assign_in_progress(&result, stringToKeyNumber(data));  //JM

  Check_MultiPresses(&result);                                 //JM

  if(result == CHR_PROD_SIGN) {
    result = (getSystemFlag(FLAG_MULTx) ? CHR_CROSS : CHR_DOT);
  }

  resetShiftState();

  return result;
}



/********************************************//**
 * \brief Simulate a button click.
 *
 * \param w GtkWidget* The button to pass to btnPressed and btnReleased
 * \param data gpointer String containing the key ID
 * \return void
 ***********************************************/
#ifdef PC_BUILD
void btnClicked(GtkWidget *w, gpointer data) {
#endif
#ifdef DMCP_BUILD
void btnClicked(void *w, void *data) {
#endif
  btnPressed(w, data);
  btnReleased(w, data);
}



/********************************************//**
 * \brief A calc button was pressed
 *
 * \param w GtkWidget*
 * \param data gpointer pointer to a string containing the key number pressed: 00=1/x, ..., 36=EXIT
 * \return void
 ***********************************************/
#ifdef PC_BUILD
void btnPressed(GtkWidget *notUsed, gpointer data) {
#endif
#ifdef DMCP_BUILD
void btnPressed(void *notUsed, void *data) {
#endif
  int16_t item = determineItem((char *)data);

  showFunctionNameItem = 0;        //JM TOCHECK
  if(item != ITM_NOP && item != ITM_NULL) {
    processKeyAction(item);
    if(!keyActionProcessed) {
      showFunctionName(item, 10);
    }
  }
}



/********************************************//**
 * \brief A calc button was released
 *
 * \param w GtkWidget*
 * \param data gpointer pointer to a string containing the key number pressed: 00=1/x, ..., 36=EXIT
 * \return void
 ***********************************************/
#ifdef PC_BUILD
void btnReleased(GtkWidget *notUsed, gpointer data) {
#endif
#ifdef DMCP_BUILD
void btnReleased(void *notUsed, void *data) {
#endif
  Shft_timeouts = false;                         //JM SHIFT NEW
  JM_auto_longpress_enabled = 0;                 //JM TIMER CLRCLSTK ON LONGPRESS
  if(showFunctionNameItem != 0) {
    int16_t item = showFunctionNameItem;
    hideFunctionName();
    if(item < 0) {
      showSoftmenu(NULL, item, calcMode == CM_AIM ? true : false);
    }
    else {
      runFunction(item);
    }
  }
}



/********************************************//**
 * \brief A calc button was pressed
 *
 * \param w GtkWidget*
 * \param data gpointer pointer to a string containing the key number pressed: 00=1/x, ..., 36=EXIT
 * \return void
 ***********************************************/
void processKeyAction(int16_t item) {
  keyActionProcessed = false;

  if(lastErrorCode != 0 && item != KEY_EXIT1 && item != KEY_BACKSPACE) {
    lastErrorCode = 0;
    refreshStack();
  }

  resetTemporaryInformation();
  switch(item) {
    case KEY_BACKSPACE:
//    fnKeyBackspace(NOPARAM);     //JM vv remove this, to allow this function via timing out to NOP, and this is incorporated with the CLRDROP
//    keyActionProcessed = true;   //JM ^^
      break;

    case KEY_UP1:
      keyActionProcessed = true;   //JM swapped to before fnKeyUp to be able to check if key was processed below. Chose to process it here, as fnKeyUp does not have access to item.
      fnKeyUp(NOPARAM);
      if(!keyActionProcessed){     //JMvv
         keyActionProcessed = true;
         addItemToBuffer(CHR_UP_ARROW);    //Let the arrows produce arrow up and arrow down in ALPHA mode
      }                            //JM^^
      break;

    case KEY_DOWN1:
      keyActionProcessed = true;   //swapped to before fnKeyUp to be able to check if key was processed below. Chose to process it here, as fnKeyUp does not have access to item.
      fnKeyDown(NOPARAM);
      if(!keyActionProcessed){     //JM
         keyActionProcessed = true;
         addItemToBuffer(CHR_DOWN_ARROW);    //Let the arrows produce arrow up and arrow down in ALPHA mode
      }                            //JM^^
      break;

    case KEY_EXIT1:
      fnKeyExit(NOPARAM);
      keyActionProcessed = true;
      break;

    case KEY_CC:
    case ITM_ENTER:
    case KEY_dotD:
      if(calcMode == CM_REGISTER_BROWSER || calcMode == CM_FLAG_BROWSER || calcMode == CM_FLAG_BROWSER_OLD || calcMode == CM_FONT_BROWSER) {
        keyActionProcessed = true;
      }
      break;

    case CHR_case: {
      int16_t sm = softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId;                                                       //JMvv
      if(alphaCase == AC_LOWER) {
        alphaCase = AC_UPPER;
        if(sm == -MNU_alpha_omega || sm == -MNU_a_z || sm == -MNU_ALPHAintl) {
          softmenuStack[softmenuStackPointer - 1].softmenu--; // Switch case menu
        }
      } else {
        alphaCase = AC_LOWER;
        if(sm == -MNU_ALPHA_OMEGA || sm == -MNU_A_Z || sm == -MNU_ALPHAINTL) {
          softmenuStack[softmenuStackPointer - 1].softmenu++; // Switch case menu
        }
      }
      showAlphaModeonGui(); //dr JM, see keyboardtweaks
      showSoftmenuCurrentPart();
      keyActionProcessed = true;
      }
      break;                                                                        //JM^^

    default:
      switch(calcMode) {
        case CM_NORMAL:
          if(item == ITM_EXPONENT || item==CHR_PERIOD || (CHR_0<=item && item<=CHR_9)) {
            addItemToNimBuffer(item);
            keyActionProcessed = true;
          }
          // Following commands do not timeout to NOP
          else if(/*item == KEY_UNDO ||JM*/ item == KEY_BST || item == KEY_SST || item == ITM_PR || item == ITM_AIM) {     //UNDO should time out
            runFunction(item);
            keyActionProcessed = true;
          }
          break;

        case CM_AIM:
          if(alphaCase==AC_LOWER && (CHR_A<=item && item<=CHR_Z)) {
            addItemToBuffer(item + 26);
            keyActionProcessed = true;
          }

          else if(alphaCase==AC_LOWER && ( (CHR_ALPHA<=item && item<=CHR_OMEGA) || (CHR_QOPPA<=item && item<=CHR_SAMPI) ))  {  //JM GREEK
            addItemToBuffer(item + 36);
            keyActionProcessed = true;
          }

          else if(item == CHR_DOWN_ARROW) {
            nextChar = NC_SUBSCRIPT;
            keyActionProcessed = true;
          }

          else if(item == CHR_UP_ARROW) {
            nextChar = NC_SUPERSCRIPT;
            keyActionProcessed = true;
          }
          break;

        case CM_TAM:
          addItemToBuffer(item);
          keyActionProcessed = true;
          break;

        case CM_ASM:
        case CM_ASM_OVER_TAM:
        case CM_ASM_OVER_AIM:
          if(alphaCase==AC_LOWER && (CHR_A<=item && item<=CHR_Z)) {
            addItemToBuffer(item + 26);
            keyActionProcessed = true;
          }

          else if(alphaCase==AC_LOWER && ((CHR_ALPHA<=item && item<=CHR_OMEGA) || (CHR_QOPPA<=item && item<=CHR_SAMPI) ))  {  //JM GREEK
            addItemToBuffer(item + 36);
            keyActionProcessed = true;
          }

          else if(item == CHR_DOWN_ARROW || item == CHR_UP_ARROW) {
            addItemToBuffer(item);
            keyActionProcessed = true;
          }
          break;

        case CM_NIM:
          keyActionProcessed = true;
          addItemToNimBuffer(item);
          break;

        case CM_REGISTER_BROWSER:
          if(item == CHR_PERIOD) {
            rbr1stDigit = true;
            if(rbrMode == RBR_GLOBAL) {
              rbrMode = RBR_LOCAL;
              currentRegisterBrowserScreen = FIRST_LOCAL_REGISTER;
            }
            else if(rbrMode == RBR_LOCAL) {
              rbrMode = RBR_GLOBAL;
              currentRegisterBrowserScreen = REGISTER_X;
            }
            else if(rbrMode == RBR_NAMED) {
              rbrMode = RBR_LOCAL;
              currentRegisterBrowserScreen = FIRST_LOCAL_REGISTER;
            }
            registerBrowser(NOPARAM);
          }
          else if(item == ITM_RS) {
            rbr1stDigit = true;
            showContent = !showContent;
            registerBrowser(NOPARAM);
          }
          else if(item == ITM_RCL) {
            rbr1stDigit = true;
            if(rbrMode == RBR_GLOBAL || rbrMode == RBR_LOCAL) {
              calcMode = previousCalcMode;
              oldTime[0] = 0;
              showDateTime();
              clearScreen(false, true, true);
              fnRecall(currentRegisterBrowserScreen);
              STACK_LIFT_ENABLE;
              refreshStack();
            }
            else if(rbrMode == RBR_NAMED) {
            }
          }
          else if(CHR_0 <= item && item <= CHR_9 && (rbrMode == RBR_GLOBAL || rbrMode == RBR_LOCAL)) {
            if(rbr1stDigit) {
              rbr1stDigit = false;
              rbrRegister = item - CHR_0;
            }
            else {
              rbr1stDigit = true;
              rbrRegister = rbrRegister*10 + item - CHR_0;

              if(rbrMode == RBR_GLOBAL) {
                currentRegisterBrowserScreen = rbrRegister;
                registerBrowser(NOPARAM);
              }
              else {
                rbrRegister = (rbrRegister >= allLocalRegisterPointer->numberOfLocalRegisters ? 0 : rbrRegister);
                currentRegisterBrowserScreen = FIRST_LOCAL_REGISTER + rbrRegister;
                registerBrowser(NOPARAM);
              }
            }
          }

          keyActionProcessed = true;
          break;

        case CM_FLAG_BROWSER:
        case CM_FLAG_BROWSER_OLD:           //JM
        case CM_FONT_BROWSER:
        case CM_ERROR_MESSAGE:
        case CM_BUG_ON_SCREEN:
          keyActionProcessed = true;
          break;

        case CM_CONFIRMATION:
          if(item == ITEM_CONF_Y || item == ITM_XEQ) { // Yes or XEQ
            calcMode = previousCalcMode;
            temporaryInformation = TI_NO_INFO;
            confirmedFunction(CONFIRMED);
            refreshStack();
          }

          else if(item == ITEM_CONF_N || item == KEY_EXIT1) { // No
            calcMode = previousCalcMode;
            temporaryInformation = TI_NO_INFO;
            refreshStack();
          }

          keyActionProcessed = true;
          break;

        default:
          sprintf(errorMessage, "In function btnPressed: %" FMT8U " is an unexpected value while processing calcMode!", calcMode);
          displayBugScreen(errorMessage);
      }
  }
}
#endif // TESTSUITE_BUILD



/********************************************//**
 * \brief Processing ENTER key
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnKeyEnter(uint16_t unusedParamButMandatory) {
  #ifndef TESTSUITE_BUILD
  switch(calcMode) {
    case CM_NORMAL:
      if( !eRPN ) {                                    //JM NEWERPN
        STACK_LIFT_ENABLE;

        liftStack();
        copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
        refreshStack();
        //printf("ERPN--1\n");
        STACK_LIFT_DISABLE;
      }                                               //JM NEWERPN vv
      else {
        if(getSystemFlag(FLAG_ASLIFT)) {
          liftStack();
          copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
          refreshStack();
          //printf("ERPN--2\n");
        }   
      STACK_LIFT_DISABLE;                           //JM NEWERPN (COMMENT: THESE ARE NOT NEEDED AS IT GET OVERWRITTEN BY RUNFN)
      }                                               //JM NEWERPN ^^
      break;

    case CM_AIM:
    case CM_ASM_OVER_AIM:
      calcModeNormal();
        while(softmenuStackPointer > softmenuStackPointerBeforeAIM) {                   //JMMENU was 0, to POP OFF ALL MENUS; changed by Martin to before AIM
        popSoftmenu();
      }

      if(aimBuffer[0] == 0) {
        restoreStack();
      }
      else {
        int16_t len = stringByteLength(aimBuffer) + 1;

        reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len), AM_NONE);
        xcopy(REGISTER_STRING_DATA(REGISTER_X), aimBuffer, len);

        if( !eRPN ) {                                    //JM NEWERPN
          STACK_LIFT_ENABLE;
          liftStack();
          STACK_LIFT_DISABLE;
          copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
          aimBuffer[0] = 0;
          refreshStack();
        } else {
          if(getSystemFlag(FLAG_ASLIFT)) {
          liftStack();
          STACK_LIFT_DISABLE;
          copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
          aimBuffer[0] = 0;
          refreshStack();          
          }
        }
      }

      refreshStack();
      break;

    case CM_NIM:
      closeNim();

      if( !eRPN ) {                                    //JM NEWERPN
        STACK_LIFT_ENABLE;
        liftStack();
        STACK_LIFT_DISABLE;
        copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
        refreshStack();
      } else {
        if(getSystemFlag(FLAG_ASLIFT)) {
        liftStack();
        STACK_LIFT_DISABLE;
        copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
        refreshStack();          
        }
      }
      break;

    case CM_TAM:
      tamTransitionSystem(TT_ENTER);
      break;

    case CM_ASM:
    case CM_ASM_OVER_TAM:
    case CM_REGISTER_BROWSER:
    case CM_FLAG_BROWSER:
    case CM_FLAG_BROWSER_OLD:           //JM
    case CM_FONT_BROWSER:
    case CM_ERROR_MESSAGE:
    case CM_BUG_ON_SCREEN:
      break;

    case CM_CONFIRMATION:
      calcMode = previousCalcMode;
      confirmedFunction(CONFIRMED);
      refreshStack();
      break;

    default:
      sprintf(errorMessage, "In function fnKeyEnter: unexpected calcMode value (%" FMT8U ") while processing key ENTER!", calcMode);
      displayBugScreen(errorMessage);
  }
  #endif
}



/********************************************//**
 * \brief Processing EXIT key
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnKeyExit(uint16_t unusedParamButMandatory) {
  #ifndef TESTSUITE_BUILD
  switch(calcMode) {
    case CM_NORMAL:
      if(lastErrorCode != 0) {
        lastErrorCode = 0;
        refreshStack();
      }
      else if(softmenuStackPointer > 0) {
        popSoftmenu();
      }
      break;

    case CM_AIM:
        if(softmenuStack[softmenuStackPointer-1].softmenu == MY_ALPHA_MENU) {
//JM_TOCHECK if(/*(*/ softmenuStackPointer == 1 /*) && (softmenuStack[softmenuStackPointer-1].softmenu == MY_ALPHA_MENU)*/) {      //JM ALPHA-HOME make sure we are at the bottom of the stack
        calcModeNormal();
        popSoftmenu();

        if(aimBuffer[0] == 0) {
          restoreStack();
        }
        else {
          int16_t len = stringByteLength(aimBuffer) + 1;

          reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len), AM_NONE);

          xcopy(REGISTER_STRING_DATA(REGISTER_X), aimBuffer, len);
          aimBuffer[0] = 0;

          STACK_LIFT_ENABLE;
        }

        refreshStack();
      }
      else {
popSoftmenu();                        //TOCHECK
if(softmenuStackPointer == 0) {
softmenuStackPointerBeforeAIM = 0;
showSoftmenu(NULL, -MNU_MyAlpha, false);
}

//JM TOCHECK while(softmenuStackPointer > 0) {         //JMMENU POP OFF ALL MENUS
//          popSoftmenu();
//        }
      }
      break;

    case CM_NIM:
      addItemToNimBuffer(KEY_EXIT1);
      break;

    case CM_TAM:
      calcModeNormal();
      refreshStack();
      break;

    case CM_ASM:
      calcModeNormal();
      break;

    case CM_ASM_OVER_TAM:
      transitionSystemState = 0;
      calcModeTam();
      sprintf(tamBuffer, "%s __", indexOfItems[getOperation()].itemCatalogName);
      tamTransitionSystem(TT_NOTHING);
      break;

    case CM_ASM_OVER_AIM:
      calcModeAim(NOPARAM);
      break;

    case CM_REGISTER_BROWSER:
    case CM_FLAG_BROWSER:
    case CM_FLAG_BROWSER_OLD:           //JM
    case CM_FONT_BROWSER:
      rbr1stDigit = true;
      calcMode = previousCalcMode;
      clearScreen(false, true, true);
      refreshStack();
      showSoftmenuCurrentPart();
      oldTime[0] = 0;
      showDateTime();
      if(calcMode == CM_AIM) {
        clearRegisterLine(AIM_REGISTER_LINE, true, true);
        xCursor = showString(aimBuffer, &standardFont, 1, Y_POSITION_OF_AIM_LINE + 6, vmNormal, true, true);
        cursorEnabled = true;
      }
      break;

    case CM_BUG_ON_SCREEN:
      calcMode = previousCalcMode;
      clearScreen(false, true, true);
      refreshStack();
      showSoftmenuCurrentPart();
      break;

    case CM_CONFIRMATION:
      calcMode = previousCalcMode;
      temporaryInformation = TI_NO_INFO;
      refreshStack();
      break;

    default:
      sprintf(errorMessage, "In function fnKeyExit: unexpected calcMode value (%" FMT8U ") while processing key EXIT!", calcMode);
      displayBugScreen(errorMessage);
  }
  #endif
}



/********************************************//**
 * \brief Processing CC key
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnKeyCC(uint16_t complex_Type) {    //JM Using 'unusedParamButMandatory' complex_Type=KEY_COMPLEX
  #ifndef TESTSUITE_BUILD
  uint32_t dataTypeX;
  uint32_t dataTypeY;

  //JM The switch statement is broken up here, due to multiple conditions.
  if(calcMode == CM_NORMAL || ((calcMode == CM_NIM) && (complex_Type == KEY_COMPLEX))) {
      dataTypeX = getRegisterDataType(REGISTER_X);
      dataTypeY = getRegisterDataType(REGISTER_Y);

      if(   (dataTypeX == dtReal34 || dataTypeX == dtLongInteger)
         && (dataTypeY == dtReal34 || dataTypeY == dtLongInteger)) {
        runFunction(ITM_REtoCX);
      }
      else if(dataTypeX == dtComplex34) {
        runFunction(ITM_CXtoRE);
      }
      else {
        displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "You cannot use CC with %s in X and %s in Y!", getDataTypeName(getRegisterDataType(REGISTER_X), true, false), getDataTypeName(getRegisterDataType(REGISTER_Y), true, false));
          showInfoDialog("In function btnPressed:", errorMessage, NULL, NULL);
        #endif
      }
      return;
  }

  switch(calcMode) {
    case CM_NIM:
      addItemToNimBuffer(KEY_CC);
      break;

    case CM_ASM:
    case CM_ASM_OVER_TAM:
    case CM_ASM_OVER_AIM:
      break;

    case CM_REGISTER_BROWSER:
    case CM_FLAG_BROWSER:
    case CM_FLAG_BROWSER_OLD:           //JM
    case CM_FONT_BROWSER:
      break;

    default:
      sprintf(errorMessage, "In function fnKeyCC: unexpected calcMode value (%" FMT8U ") while processing key CC!", calcMode);
      displayBugScreen(errorMessage);
  }
  #endif
}



/********************************************//**
 * \brief Processing BACKSPACE key
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnKeyBackspace(uint16_t unusedParamButMandatory) {
  #ifndef TESTSUITE_BUILD
  uint16_t lg, x, y, newXCursor;

  switch(calcMode) {
    case CM_NORMAL:
      if(lastErrorCode != 0) {
        lastErrorCode = 0;
        refreshStack();
      }
      else {
        runFunction(ITM_CLX);
      }
      break;

    case CM_AIM:
    case CM_ASM_OVER_AIM:
      if(stringByteLength(aimBuffer) > 0) {
        lg = stringLastGlyph(aimBuffer);
        aimBuffer[lg] = 0;
        newXCursor = showString(aimBuffer, &standardFont, 1, Y_POSITION_OF_AIM_LINE + 6, vmNormal, true, true);
        for(x=newXCursor; x<xCursor+6; x++) {
          for(y=Y_POSITION_OF_AIM_LINE+6; y<Y_POSITION_OF_AIM_LINE+26; y++) {
            clearPixel(x, y);
          }
        }
        xCursor = newXCursor;
      }
      break;

    case CM_NIM:
      addItemToNimBuffer(KEY_BACKSPACE);
      break;

    case CM_TAM:
      tamTransitionSystem(TT_BACKSPACE);
      break;

    case CM_ASM:
      addItemToBuffer(KEY_BACKSPACE);
      break;

    case CM_ASM_OVER_TAM:
      break;

    case CM_REGISTER_BROWSER:
    case CM_FLAG_BROWSER:
    case CM_FLAG_BROWSER_OLD:           //JM
    case CM_FONT_BROWSER:
      calcMode = previousCalcMode;
      clearScreen(false, true, true);
      refreshStack();
      showSoftmenuCurrentPart();
      oldTime[0] = 0;
      showDateTime();
      break;

    case CM_BUG_ON_SCREEN:
      calcMode = previousCalcMode;
      clearScreen(false, true, true);
      refreshStack();
      showSoftmenuCurrentPart();
      break;

    case CM_CONFIRMATION:
      calcMode = previousCalcMode;
      temporaryInformation = TI_NO_INFO;
      refreshStack();
      break;

    default:
      sprintf(errorMessage, "In function fnKeyBackspace: unexpected calcMode value (%" FMT8U ") while processing key BACKSPACE!", calcMode);
      displayBugScreen(errorMessage);
  }
  #endif
}



/********************************************//**
 * \brief Processing UP key
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnKeyUp(uint16_t unusedParamButMandatory) {
  #ifndef TESTSUITE_BUILD
  int16_t itemShift;

  if(calcMode == CM_NORMAL && softmenuStackPointer == 0)  {fnShow_SCROLL(1);}             //JMSHOW

  switch(calcMode) {
    case CM_NORMAL:
    case CM_AIM:
    case CM_NIM:
    case CM_ASM:
    case CM_ASM_OVER_TAM:
    case CM_ASM_OVER_AIM:
      resetAlphaSelectionBuffer();
      if((softmenuStackPointer > 0)  && softmenuStack[softmenuStackPointer - 1].softmenu != MY_ALPHA_MENU && softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId != -MNU_ALPHA) {
        int16_t sm = softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId;
        if((sm == -MNU_alpha_omega || sm == -MNU_a_z || sm == -MNU_ALPHAintl) && alphaCase == AC_LOWER && arrowCasechange) {  //JMcase
          alphaCase = AC_UPPER;
          showAlphaModeonGui(); //dr JM, see keyboardtweaks
          softmenuStack[softmenuStackPointer - 1].softmenu--; // Switch to the upper case menu
          showSoftmenuCurrentPart();
        }

        else if((sm == -MNU_ALPHADOT || sm == -MNU_ALPHAMATH || sm == -MNU_ALPHA) && alphaCase == AC_LOWER && arrowCasechange) {  //JMcase
          alphaCase = AC_UPPER;
          showAlphaModeonGui(); //dr JM, see keyboardtweaks
        }
        else {
          itemShift = alphaSelectionMenu == ASM_NONE ? 18 : 6;

          if((softmenuStack[softmenuStackPointer - 1].firstItem + itemShift) < softmenu[softmenuStack[softmenuStackPointer-1].softmenu].numItems) {         //JM
            softmenuStack[softmenuStackPointer - 1].firstItem += itemShift;
            //JM Include or exclude HOME menu screens  //JMHOME
            #define A1 3   //HAAKON //jm_HOME_MIR
            #define A2 9   //HAAKON
            #define B1 10  //NIGEL  //jm_HOME_SUM
            #define B2 11  //NIGEL
            #define C1 12  //JACO   //jm_HOME_FIX
            #define C2 18  //JACO
            int16_t smm = softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId;
            //printf("--1:%d %d %d menuId:%d item:%d  \n",jm_HOME_MIR,jm_HOME_SUM,jm_HOME_FIX,smm,softmenuStack[softmenuStackPointer - 1].firstItem/18);
            if (!jm_HOME_MIR && smm == -MNU_HOME && softmenuStack[softmenuStackPointer - 1].firstItem == A1*18) {softmenuStack[softmenuStackPointer - 1].firstItem = (A2+1)*18;} 
            if (!jm_HOME_SUM && smm == -MNU_HOME && softmenuStack[softmenuStackPointer - 1].firstItem == B1*18) {softmenuStack[softmenuStackPointer - 1].firstItem = (B2+1)*18;} 
            if (!jm_HOME_FIX && smm == -MNU_HOME && softmenuStack[softmenuStackPointer - 1].firstItem == C1*18) {softmenuStack[softmenuStackPointer - 1].firstItem = (C2+1)*18;}
            //smm = softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId;
            //printf(   "--2:      menuId:%d item:%d  \n",smm,softmenuStack[softmenuStackPointer - 1].firstItem/18);
            showSoftmenuCurrentPart();
          }
          else {
            softmenuStack[softmenuStackPointer - 1].firstItem = 0;

            showSoftmenuCurrentPart();
          }

          setCatalogLastPos();
        }
      }
      else {
        //JM Arrow up and down if no menu other than AHOME of MyA
        if(!arrowCasechange && (softmenuStackPointer > 0) && (softmenuStack[softmenuStackPointer - 1].softmenu == MY_ALPHA_MENU || softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId == -MNU_ALPHA)) {
           keyActionProcessed = false;
        }
        else {
          if(alphaCase != AC_UPPER) {
            alphaCase = AC_UPPER;
            showAlphaModeonGui(); //dr JM, see keyboardtweaks
          }
        }
      }
      break;

    case CM_TAM:
      addItemToBuffer(ITM_Max);
      break;

    case CM_REGISTER_BROWSER:
      rbr1stDigit = true;
      if(rbrMode == RBR_GLOBAL) {
        currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen + 1, FIRST_LOCAL_REGISTER);
        registerBrowser(NOPARAM);
      }
      else if(rbrMode == RBR_LOCAL) {
        currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - FIRST_LOCAL_REGISTER + 1, allLocalRegisterPointer->numberOfLocalRegisters) + FIRST_LOCAL_REGISTER;
        registerBrowser(NOPARAM);
      }
      else if(rbrMode == RBR_NAMED) {
        currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - 1000 + 1, allNamedVariablePointer->numberOfNamedVariables) + 1000;
        registerBrowser(NOPARAM);
      }
      else {
        sprintf(errorMessage, "In function btnPressed: unexpected case while processing key UP! %" FMT8U " is an unexpected value for rbrMode.", rbrMode);
        displayBugScreen(errorMessage);
      }
      break;

    case CM_FLAG_BROWSER:
      currentFlgScr--;                          //JM removed the 3-x part
      if(currentFlgScr==0) {currentFlgScr=3;}   //JM
      flagBrowser(NOPARAM);
     break;

    case CM_FLAG_BROWSER_OLD:              //JMvv
      currentFlgScr--;
      if(currentFlgScr==0) {currentFlgScr=3;}
      flagBrowser_old(NOPARAM);
      break;                               //JM^^

    case CM_FONT_BROWSER:
      if(currentFntScr >= 2) {
        currentFntScr--;
      }
      fontBrowser(NOPARAM);
      break;

    default:
      sprintf(errorMessage, "In function fnKeyUp: unexpected calcMode value (%" FMT8U ") while processing key UP!", calcMode);
      displayBugScreen(errorMessage);
  }
  #endif
}



/********************************************//**
 * \brief Processing DOWN key
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnKeyDown(uint16_t unusedParamButMandatory) {
  #ifndef TESTSUITE_BUILD
  int16_t itemShift;

  if(calcMode == CM_NORMAL && softmenuStackPointer == 0)  {fnShow_SCROLL(2);}             //JMSHOW

  switch(calcMode) {
    case CM_NORMAL:                     //JMSHOW vv
    case CM_AIM:
    case CM_NIM:
    case CM_ASM:
    case CM_ASM_OVER_TAM:
    case CM_ASM_OVER_AIM:
      resetAlphaSelectionBuffer();
      if((softmenuStackPointer > 0)  && softmenuStack[softmenuStackPointer - 1].softmenu != MY_ALPHA_MENU && softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId != -MNU_ALPHA) {
        int16_t sm = softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId;
        if((sm == -MNU_ALPHA_OMEGA || sm == -MNU_A_Z || sm == -MNU_ALPHAINTL) && alphaCase == AC_UPPER && arrowCasechange) {  //JMcase
          alphaCase = AC_LOWER;
          showAlphaModeonGui(); //dr JM, see keyboardtweaks
          softmenuStack[softmenuStackPointer - 1].softmenu++; // Switch to the lower case menu
          showSoftmenuCurrentPart();
        }
        else if((sm == -MNU_ALPHADOT || sm == -MNU_ALPHAMATH || sm == -MNU_ALPHA) && alphaCase == AC_UPPER && arrowCasechange) {  //JMcase
          alphaCase = AC_LOWER;
          showAlphaModeonGui(); //dr JM, see keyboardtweaks
        }
      else {
          itemShift = alphaSelectionMenu == ASM_NONE ? 18 : 6;

          if((softmenuStack[softmenuStackPointer - 1].firstItem - itemShift) >= 0) {
            softmenuStack[softmenuStackPointer - 1].firstItem -= itemShift;
            //JM Include or exclude HOME menu screens  //JMHOME
            int16_t smm = softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId;
            if (!jm_HOME_FIX && smm == -MNU_HOME && softmenuStack[softmenuStackPointer - 1].firstItem == C2*18) {softmenuStack[softmenuStackPointer - 1].firstItem = (C1-1)*18;}
            if (!jm_HOME_SUM && smm == -MNU_HOME && softmenuStack[softmenuStackPointer - 1].firstItem == B2*18) {softmenuStack[softmenuStackPointer - 1].firstItem = (B1-1)*18;} 
            if (!jm_HOME_MIR && smm == -MNU_HOME && softmenuStack[softmenuStackPointer - 1].firstItem == A2*18) {softmenuStack[softmenuStackPointer - 1].firstItem = (A1-1)*18;}
/*JM*/      showSoftmenuCurrentPart();
          }
          else {
            if((softmenuStack[softmenuStackPointer - 1].firstItem - itemShift) >= -5) {
              softmenuStack[softmenuStackPointer - 1].firstItem = 0;
            }
            else {
              softmenuStack[softmenuStackPointer - 1].firstItem = ((softmenu[softmenuStack[softmenuStackPointer-1].softmenu].numItems - 1)/6) / (itemShift/6) * itemShift;
            }
            showSoftmenuCurrentPart();
          }

          setCatalogLastPos();
        }
      }
      else {
        //JM Arrow up and down if no menu other than AHOME of MyA //JMvv
        if(!arrowCasechange && (softmenuStackPointer > 0) && (softmenuStack[softmenuStackPointer - 1].softmenu == MY_ALPHA_MENU || softmenu[softmenuStack[softmenuStackPointer - 1].softmenu].menuId == -MNU_ALPHA)) {
          keyActionProcessed = false;
        }                                                         //JM^^
        else {
          if(alphaCase != AC_LOWER && arrowCasechange) { //JM
            alphaCase = AC_LOWER;
            showAlphaModeonGui(); //dr JM, see keyboardtweaks
          }
        }
      }
      break;

    case CM_TAM:
      addItemToBuffer(ITM_Min);
      break;


    case CM_REGISTER_BROWSER:
      rbr1stDigit = true;
      if(rbrMode == RBR_GLOBAL) {
        currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - 1, FIRST_LOCAL_REGISTER);
        registerBrowser(NOPARAM);
      }
      else if(rbrMode == RBR_LOCAL) {
        currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - FIRST_LOCAL_REGISTER - 1, allLocalRegisterPointer->numberOfLocalRegisters) + FIRST_LOCAL_REGISTER;
        registerBrowser(NOPARAM);
      }
      else if(rbrMode == RBR_NAMED) {
        currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - 1000 - 1, allNamedVariablePointer->numberOfNamedVariables) + 1000;
        registerBrowser(NOPARAM);
      }
      else {
        sprintf(errorMessage, "In function btnPressed: unexpected case while processing key DOWN! %" FMT8U " is an unexpected value for rbrMode.", rbrMode);
        displayBugScreen(errorMessage);
      }
      break;

    case CM_FLAG_BROWSER:
      currentFlgScr++;                          //JM removed the 3-x part
      if(currentFlgScr==4) {currentFlgScr=1;}   //JM
      flagBrowser(NOPARAM);
      break;

    case CM_FLAG_BROWSER_OLD:              //JMvv
      currentFlgScr++;
      if(currentFlgScr==4) {currentFlgScr=1;}
      flagBrowser_old(NOPARAM);
      break;                               //JM^^

    case CM_FONT_BROWSER:
      if(currentFntScr < numScreensNumericFont+numScreensStandardFont) {
        currentFntScr++;
      }
      fontBrowser(NOPARAM);
      break;

    default:
      sprintf(errorMessage, "In function fnKeyDown: unexpected calcMode value (%" FMT8U ") while processing key DOWN!", calcMode);
      displayBugScreen(errorMessage);
  }
  #endif
}



/********************************************//**
 * \brief Processing .d key
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fnKeyDotD(uint16_t unusedParamButMandatory) {
  #ifndef TESTSUITE_BUILD
  switch(calcMode) {
    case CM_NORMAL:
      if(getSystemFlag(FLAG_FRACT)) {
        clearSystemFlag(FLAG_FRACT);
        refreshStack();
      }
      else {
        runFunction(ITM_toREAL);
      }
      break;

    case CM_NIM:
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        showInfoDialog("In function btnPressed:", "the data type date is to be coded!", NULL, NULL);
      #endif
      break;

    case CM_REGISTER_BROWSER:
    case CM_FLAG_BROWSER:
    case CM_FLAG_BROWSER_OLD:           //JM
    case CM_FONT_BROWSER:
      break;

    default:
      sprintf(errorMessage, "In function fnKeyDotD: unexpected calcMode value (%" FMT8U ") while processing key .d!", calcMode);
      displayBugScreen(errorMessage);
  }
  #endif
}
