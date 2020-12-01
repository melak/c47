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
  int16_t determineFunctionKeyItem(const char *data) {
    int16_t item = ITM_NOP;
    dynamicMenuItem = -1;

      int16_t itemShift = (shiftF ? 6 : (shiftG ? 12 : 0));
      int16_t fn = *(data) - '0';
      const softmenu_t *sm;
      int16_t row, menuId = softmenuStack[softmenuStackPointer].softmenuId;
      int16_t firstItem = softmenuStack[softmenuStackPointer].firstItem;
      printf("^^^^^@ softmenuStackPointer=%d menuId=%d menuItem=%d\n",softmenuStackPointer, menuId, -softmenu[menuId].menuItem);
      if(softmenuStackPointer > 0 || !(menuId==0 && jm_FG_DOTS) ) {
        	switch(-softmenu[menuId].menuItem) {
          case MNU_PROG:
            dynamicMenuItem = firstItem + itemShift + (fn - 1);
            item = (dynamicMenuItem >= dynamicSoftmenu[menuId].numItems ? ITM_NOP : MNU_DYNAMIC);
            break;

          default:
            sm = &softmenu[menuId];
            row = min(3, (sm->numItems + modulo(firstItem - sm->numItems, 6))/6 - firstItem/6) - 1;
            if(itemShift/6 <= row && firstItem + itemShift + (fn - 1) < sm->numItems) {
              item = (sm->softkeyItem)[firstItem + itemShift + (fn - 1)] % 10000;

            int16_t ix_fn = 0;                                 /*JMEXEC XXX vv*/
            if(func_lookup(fn,itemShift,&ix_fn)) item = ix_fn;
                                                             /*JMEXEC XXX ^^*/

            if(item == ITM_PROD_SIGN) {
              item = (getSystemFlag(FLAG_MULTx) ? ITM_DOT : ITM_CROSS);
            }
          }
        }
      }

      else {              //if there is no SoftMenu showing
        if(fn>=1 && fn<=6) {
          if (itemShift ==0) {
          //JM FN KEYS DIRECTLY ACCESSIBLE IF NO MENUS ARE UP;                       // FN Key will be the same as the yellow label underneath it, even if USER keys were selected.
            temporaryInformation = TI_NO_INFO; item = ( !getSystemFlag(FLAG_USER) ? (kbd_std[fn-1].fShifted) : (kbd_usr[fn-1].fShifted) );  //Function key follows if the yellow key top 4 buttons are changed from default.      
          } else {
          //JM FN KEYS DIRECTLY ACCESSIBLE IF NO MENUS ARE UP;                       // FN Key will be the same as the blue label underneath it, even if USER keys were selected.
            temporaryInformation = TI_NO_INFO; item = ( !getSystemFlag(FLAG_USER) ? (kbd_std[fn-1].gShifted) : (kbd_usr[fn-1].gShifted) );  //Function key follows if the yellow key top 4 buttons are changed from default.              
          }
        } else {
          item = 0;
        }
      }
    return item;
  }



  /********************************************//**
   * \brief Simulate a function key click.
   *
   * \param notUsed GtkWidget* The button to pass to btnFnPressed and btnFnReleased
   * \param data gpointer String containing the key ID
   * \return void
   ***********************************************/
  #ifdef PC_BUILD
    void btnFnClicked(GtkWidget *notUsed, gpointer data) {
  #endif
  #ifdef DMCP_BUILD
    void btnFnClicked(void *notUsed, void *data) {
  #endif
    
      executeFunction(data);
    }
    
  #ifdef PC_BUILD
    void btnFnClickedP(GtkWidget *notUsed, gpointer data) { //JM Added this portion to be able to go to NOP on emulator
      GdkEvent mouseButton;
      mouseButton.button.button = 1;
      mouseButton.type = 0;
      btnFnPressed(notUsed, &mouseButton, data);
    }
    void btnFnClickedR(GtkWidget *notUsed, gpointer data) { //JM Added this portion to be able to go to NOP on emulator
      GdkEvent mouseButton;
      mouseButton.button.button = 1;
      mouseButton.type = 0;
      btnFnReleased(notUsed, &mouseButton, data);
    }
  #endif // PC_BUILD



  /********************************************//**
   * \brief A calc function key was pressed
   *
   * \param notUsed GtkWidget*
   * \param data gpointer pointer to a string containing the key number pressed: 00=1/x, ..., 36=EXIT
   * \return void
   ***********************************************/
  #ifdef PC_BUILD
    void btnFnPressed(GtkWidget *notUsed, GdkEvent *event, gpointer data) {
      if(event->type == GDK_DOUBLE_BUTTON_PRESS || event->type == GDK_TRIPLE_BUTTON_PRESS) { // double click
        return;
      }
      if(event->button.button == 2) { // Middle click
        shiftF = true;
        shiftG = false;
      }
      if(event->button.button == 3) { // Right click
        shiftF = false;
        shiftG = true;
      }
      if(calcMode != CM_REGISTER_BROWSER && calcMode != CM_FLAG_BROWSER && calcMode != CM_FLAG_BROWSER_OLD && calcMode != CM_FONT_BROWSER) {
        int16_t item = determineFunctionKeyItem((char *)data);

    //    resetShiftState();                                 //JM still needs the shifts active prior to cancelling them
        if(item != ITM_NOP /*&& item != ITM_NULL*/) {        //JM still need to run the longpress even if no function populated in FN, ie NOP or NULL
          lastErrorCode = 0;

    //    #if(FN_KEY_TIMEOUT_TO_NOP == 1)                    //JM vv Rmove the possibility for error by removing code that may conflict with the state machine
    //      showFunctionName(item, 1000); // 1000ms = 1s
    //    #else // (FN_KEY_TIMEOUT_TO_NOP == 0)
    //    showFunctionNameItem = item;
            btnFnPressed_StateMachine(NULL, data);        //JM ^^ This calls original state analysing btnFnPressed routing, which is now renamed to "statemachine" in keyboardtweaks
    //    #endif // (FN_KEY_TIMEOUT_TO_NOP == 1)
        }
        else {
          showFunctionNameItem = ITM_NOP;
        }
      }
    }
  #endif // PC_BUILD

  #ifdef DMCP_BUILD
    void btnFnPressed(void *data) {
      if(calcMode != CM_REGISTER_BROWSER && calcMode != CM_FLAG_BROWSER && calcMode != CM_FLAG_BROWSER_OLD && calcMode != CM_FONT_BROWSER) {
        int16_t item = determineFunctionKeyItem((char *)data);

    //    resetShiftState();                                 //JM still needs the shifts active prior to cancelling them
        if(item != ITM_NOP /*&& item != ITM_NULL*/) {        //JM still need to run the longpress even if no function populated in FN, ie NOP or NULL
          lastErrorCode = 0;
    //    #if(FN_KEY_TIMEOUT_TO_NOP == 1)                    //JM vv Rmove the possibility for error by removing code that may conflict with the state machine
    //      showFunctionName(item, 1000); // 1000ms = 1s
    //    #else // (FN_KEY_TIMEOUT_TO_NOP == 0)
    //    showFunctionNameItem = item;
            btnFnPressed_StateMachine(NULL, data);        //JM ^^ This calls original state analysing btnFnPressed routing, which is now renamed to "statemachine" in keyboardtweaks
    //    #endif // (FN_KEY_TIMEOUT_TO_NOP == 1)
        }
        else {
          showFunctionNameItem = ITM_NOP;
        }
      }
    }
  #endif // DMCP_BUILD



  /********************************************//**
   * \brief A calc function key was released
   *
   * \param notUsed GtkWidget*
   * \param data gpointer pointer to a string containing the key number pressed: 00=1/x, ..., 36=EXIT
   * \return void
   ***********************************************/
  #ifdef PC_BUILD
    void btnFnReleased(GtkWidget *notUsed, GdkEvent *event, gpointer data) {
  #endif // PC_BUILD
  #ifdef DMCP_BUILD
    void btnFnReleased(void *data) {
  #endif // DMCP_BUILD
    if(calcMode != CM_REGISTER_BROWSER && calcMode != CM_FLAG_BROWSER && calcMode != CM_FLAG_BROWSER_OLD && calcMode != CM_FONT_BROWSER) {
    btnFnReleased_StateMachine(NULL, data);            //This function does the longpress differentiation, and calls ExecuteFunctio below, via fnbtnclicked
    }
  }

  /********************************************//**
   * \brief Executes one function from a softmenu
   * \return void
   ***********************************************/
  void executeFunction(const char *data) {
    if(calcMode != CM_REGISTER_BROWSER && calcMode != CM_FLAG_BROWSER && calcMode != CM_FLAG_BROWSER_OLD && calcMode != CM_FONT_BROWSER) {
  
      int16_t item = ITM_NOP;
      item = determineFunctionKeyItem((char *)data);
      resetShiftState();
  
        //printf("%d--\n",calcMode);
      {
        if(calcMode != CM_CONFIRMATION) {
          lastErrorCode = 0;

          if(softmenuStackPointer > 0) {
            if(calcMode == CM_ASM) {
              calcModeNormal();
            }
            else if(calcMode == CM_ASM_OVER_TAM || calcMode == CM_ASM_OVER_TAM_OVER_PEM) {
              reallyRunFunction(getOperation(), indexOfItems[item].param); // TODO: check why the param is taken from item and not from getOperation
              if(softmenu[softmenuStack[softmenuStackPointer].softmenuId].menuItem != -MNU_SYSFL) { //JM V JM MENU Prevent resetting the softmenu to the default no 1 page position
                calcModeNormal();
              }                                                                                     //JM ^
  #ifdef PC_BUILD
  printf(">>>   refreshScreen1 from keyboard.c executeFunction\n");
  #endif
              refreshScreen();
              return;
            }
            else if(calcMode == CM_ASM_OVER_AIM) {
              calcMode = CM_AIM;
              addItemToBuffer(item);
              calcMode = CM_ASM_OVER_AIM;
  #ifdef PC_BUILD
  printf(">>>   refreshScreen2 from keyboard.c executeFunction\n");
  #endif
              refreshScreen();
              return;
            }
            else if(calcMode == CM_ASM_OVER_PEM) { // TODO: is that correct
              calcModeNormal();
              calcMode = CM_PEM;
              runFunction(item);
              refreshScreen();
              return;
            }

          }
           // Broken the IF STATEMENT, because I want the FN keys to be active if there are no softmenus
          {
            if(item < 0) { // softmenu
              if(item != -MNU_SYSFL || (calcMode != CM_TAM && calcMode != CM_TAM_OVER_PEM) || transitionSystemState == 0) {
                showSoftmenu(NULL, item, MS_PUSH);
              }
            }
            else if((calcMode == CM_NORMAL || calcMode == CM_NIM) && (ITM_0<=item && item<=ITM_F)) {
              addItemToNimBuffer(item);
            }
            else if(calcMode == CM_TAM || calcMode == CM_TAM_OVER_PEM) {
              addItemToBuffer(item);
            }
            else if(item > 0) { // function
              if(calcMode == CM_NIM && item != ITM_CC) {
                if(item!=ITM_HASH_JM ) {closeNim();}                     //JMNIM Allow NIM not closed, so that JMNIM can change the bases without ierrors thrown 
                if(calcMode != CM_NIM) {
                  if(indexOfItems[item].func == fnConstant) {
                    setSystemFlag(FLAG_ASLIFT);
                  }
                }
              }

              if(lastErrorCode == 0) {
                temporaryInformation = TI_NO_INFO;
                runFunction(item);
              }
            }
          }
        }
      }
  #ifdef PC_BUILD
  printf(">>>  refreshScreen3 from keyboard.c executeFunction\n");
  #endif
      refreshScreen();
    }
  }


#define stringToKeyNumber(data)         ((*((char *)data) - '0')*10 + *(((char *)data)+1) - '0')

  int16_t determineItem(const char *data) {
    int16_t result;
    const calcKey_t *key;

    dynamicMenuItem = -1;
//    key = getSystemFlag(FLAG_USER) ? (kbd_usr + (*data - '0')*10 + *(data+1) - '0') : (kbd_std + (*data - '0')*10 + *(data+1) - '0');
//    key = getSystemFlag(FLAG_USER) && ((calcMode == CM_NORMAL) || (calcMode == CM_NIM)) ? (kbd_usr + stringToKeyNumber(data)) : (kbd_std + stringToKeyNumber(data));    //JM Added (calcMode == CM_NORMAL) to prevent user substitution in AIM and TAM

  int8_t key_no = stringToKeyNumber(data);
  if (kbd_usr[36].primaryTam == ITM_EXIT1) //opposite keyboard V43 LT, 43S, V43 RT
    key = getSystemFlag(FLAG_USER) ? (kbd_usr + key_no) : (kbd_std + key_no);
  else
    key = getSystemFlag(FLAG_USER) && ((calcMode == CM_NORMAL) || (calcMode == CM_AIM) || (calcMode == CM_NIM) || (calcMode == CM_GRAPH) || (calcMode == CM_LISTXY)) ? (kbd_usr + key_no) : (kbd_std + key_no);    //JM Added (calcMode == CM_NORMAL) to prevent user substitution in AIM and TAM

  fnTimerExec(TO_FN_EXEC);                                  //dr execute queued fn

  switch(key->primary) {                              //JMSHOW vv
    case      ITM_UP1:
    case      ITM_DOWN1: break;                       //JM SHOWregis unchanged
    default:  SHOWregis = 9999; break;     
  }                                                   //JMSHOW ^^
  //printf("###\n"); //JMEXEC

  Setup_MultiPresses( key->primary );



    // Shift f pressed and JM REMOVED shift g not active
    if(key->primary == ITM_SHIFTf && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_TAM || calcMode == CM_TAM_OVER_PEM || calcMode == CM_NIM || calcMode == CM_ASM || calcMode == CM_ASM_OVER_TAM || calcMode == CM_ASM_OVER_TAM_OVER_PEM || calcMode == CM_ASM_OVER_AIM || calcMode == CM_PEM || calcMode == CM_ASM_OVER_PEM || calcMode == CM_GRAPH)) {    //JM Mode added
      temporaryInformation = TI_NO_INFO;
      lastErrorCode = 0;

      fnTimerStop(TO_FG_LONG);                                //dr
      fnTimerStop(TO_FG_TIMR);                                //dr
  
      shiftF = !shiftF;
      shiftG = false;                                         //JM no shifted menu on g-shift-key as in WP43S
      showShiftState();

      return ITM_NOP;
    }

    // Shift g pressed and JM REMOVED shift f not active
    else if(key->primary == ITM_SHIFTg && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_TAM || calcMode == CM_TAM_OVER_PEM || calcMode == CM_NIM || calcMode == CM_ASM || calcMode == CM_ASM_OVER_TAM || calcMode == CM_ASM_OVER_TAM_OVER_PEM || calcMode == CM_ASM_OVER_AIM || calcMode == CM_PEM || calcMode == CM_ASM_OVER_PEM || calcMode == CM_GRAPH)) {
      temporaryInformation = TI_NO_INFO;
      lastErrorCode = 0;

      fnTimerStop(TO_FG_LONG);                                //dr
      fnTimerStop(TO_FG_TIMR);                                //dr

      shiftG = !shiftG;
      shiftF = false;                                         //JM no shifted menu on g-shift-key as in WP43S
      showShiftState();

      return ITM_NOP;
    }

    // JM Shift f pressed  //JM shifts change f/g to a single function key toggle to match DM42 keyboard
    // JM Inserted new section and removed old f and g key processing sections
    else if(key->primary == KEY_fg && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_TAM || calcMode == CM_TAM_OVER_PEM || calcMode == CM_NIM || calcMode == CM_ASM || calcMode == CM_ASM_OVER_TAM || calcMode == CM_ASM_OVER_TAM_OVER_PEM || calcMode == CM_ASM_OVER_AIM || calcMode == CM_GRAPH)) {   //JM shifts
      Shft_timeouts = true;                         //JM SHIFT NEW
      fnTimerStart(TO_FG_LONG, TO_FG_LONG, JM_TO_FG_LONG);    //vv dr
      if(ShiftTimoutMode) {
        fnTimerStart(TO_FG_TIMR, TO_FG_TIMR, JM_SHIFT_TIMER); //^^
      }
      temporaryInformation = TI_NO_INFO;
                                                                                                                                //JM shifts
      if(lastErrorCode != 0) {                                                                                                  //JM shifts
        lastErrorCode = 0;                                                                                                      //JM shifts
      }                                                                                                                         //JM shifts

      fg_processing_jm();

      showShiftState();                                                                                                         //JM shifts

      return ITM_NOP;
    }                                                                                                                           //JM shifts


    if((calcMode == CM_NIM || calcMode == CM_NORMAL) && lastIntegerBase >= 11 && (key_no >= 0 && key_no <= 5 )) {               //JMNIM vv Added direct A-F for hex entry
      result = shiftF ? key->fShifted :
               shiftG ? key->gShifted :
                        key->primaryAim;
           //printf(">>> ±±±§§§ keys key:%d result:%d Calmode:%d, nimbuffer:%s, lastbase:%d, nimnumberpart:%d\n",key_no, result, calcMode,nimBuffer,lastIntegerBase, nimNumberPart);
           return result;  
    } else                                                                                                                      //JM^^

    if(calcMode == CM_NORMAL || calcMode == CM_NIM || calcMode == CM_FONT_BROWSER || calcMode == CM_FLAG_BROWSER || calcMode == CM_FLAG_BROWSER_OLD || calcMode == CM_REGISTER_BROWSER || calcMode == CM_BUG_ON_SCREEN || calcMode == CM_CONFIRMATION || calcMode == CM_PEM || calcMode == CM_GRAPH  || calcMode == CM_LISTXY) {  //JM added modes
      result = shiftF ? key->fShifted :
               shiftG ? key->gShifted :
                        key->primary;
    }
    else if(calcMode == CM_AIM || calcMode == CM_ASM || calcMode == CM_ASM_OVER_TAM || calcMode == CM_ASM_OVER_TAM_OVER_PEM || calcMode == CM_ASM_OVER_AIM || calcMode == CM_ASM_OVER_PEM) {
      result = shiftF ? key->fShiftedAim :
               shiftG ? key->gShiftedAim :
                        key->primaryAim;

    }
    else if(calcMode == CM_TAM || calcMode == CM_TAM_OVER_PEM) {
      result = key->primaryTam; // No shifted function in TAM
    }
    else {
      displayBugScreen("In function determineItem: item was not determined!");
      result = 0;
    }

    Check_Assign_in_progress(&result, key_no);  //JM

    Check_MultiPresses(&result, key_no);        //JM

    if(result == ITM_PROD_SIGN) {
      result = (getSystemFlag(FLAG_MULTx) ? ITM_CROSS : ITM_DOT);
    }

    resetShiftState();

    return result;
  }



  /********************************************//**
   * \brief Simulate a button click.
   *
   * \param notUsed GtkWidget* The button to pass to btnPressed and btnReleased
   * \param data gpointer String containing the key ID
   * \return void
   ***********************************************/
  #ifdef PC_BUILD
    void btnClicked(GtkWidget *notUsed, gpointer data) {
      GdkEvent mouseButton;
      mouseButton.button.button = 1;
      mouseButton.type = 0;

      btnPressed(notUsed, &mouseButton, data);
      btnReleased(notUsed, &mouseButton, data);
  }
  #endif // PC_BUILD

  #ifdef DMCP_BUILD
    void btnClicked(void *unused, void *data) {
      btnPressed(data);
      btnReleased(data);
    }
  #endif // DMCP_BUILD



  #ifdef PC_BUILD
  void btnClickedP(GtkWidget *w, gpointer data) {                          //JM PRESSED FOR KEYBOARD F REPEAT
    GdkEvent mouseButton;
    mouseButton.button.button = 1;
    mouseButton.type = 0;
    btnPressed(w, &mouseButton, data);
  }

  void btnClickedR(GtkWidget *w, gpointer data) {                          //JM PRESSED FOR KEYBOARD F REPEAT
    GdkEvent mouseButton;
    mouseButton.button.button = 1;
    btnReleased(w, &mouseButton, data);
  }
  #endif


  bool_t checkShifts(const char *data) {
    const calcKey_t *key;

    int8_t key_no = stringToKeyNumber(data);

    key = getSystemFlag(FLAG_USER) && ((calcMode == CM_NORMAL) || (calcMode == CM_AIM) || (calcMode == CM_NIM)) ? (kbd_usr + key_no) : (kbd_std + key_no);

    if(key->primary == ITM_SHIFTf || key->primary == ITM_SHIFTg || key->primary == KEY_fg) {
      return true;
    } else {
      return false;
    }
  }



  /********************************************//**
   * \brief A calc button was pressed
   *
   * \param notUsed GtkWidget*
   * \param data gpointer pointer to a string containing the key number pressed: 00=1/x, ..., 36=EXIT
   * \return void
   ***********************************************/
  #ifdef PC_BUILD
    void btnPressed(GtkWidget *notUsed, GdkEvent *event, gpointer data) {

      if(event->type == GDK_DOUBLE_BUTTON_PRESS || event->type == GDK_TRIPLE_BUTTON_PRESS) { // return unprocessed for double or triple click
        return;
      }
      if(event->button.button == 2) { // Middle click
        shiftF = true;
        shiftG = false;
      }
      if(event->button.button == 3) { // Right click
        shiftF = false;
        shiftG = true;
      }
      int16_t item = determineItem((char *)data);

      showFunctionNameItem = 0;
      if(item != ITM_NOP && item != ITM_NULL) {

      #ifdef PC_BUILD
        char tmp[200];
        sprintf(tmp,"keyboard.c: btnPressed --> processKeyAction(%d) which is str:%s",item,(char *)data);
        jm_show_calc_state(tmp);
      #endif

        processKeyAction(item);
        if(!keyActionProcessed) {
          showFunctionName(item, 1000); // 1000ms = 1s
        }
      }
    }
  #endif // PC_BUILD

  #ifdef DMCP_BUILD
    void btnPressed(void *data) {
      int16_t item = determineItem((char *)data);

      showFunctionNameItem = 0;
      if(item != ITM_NOP && item != ITM_NULL) {
        processKeyAction(item);
        if(!keyActionProcessed) {
          showFunctionName(item, 1000); // 1000ms = 1s
        }
      }
    }
  #endif // DMCP_BUILD



  /********************************************//**
   * \brief A calc button was released
   *
   * \param notUsed GtkWidget*
   * \param data gpointer pointer to a string containing the key number pressed: 00=1/x, ..., 36=EXIT
   * \return void
   ***********************************************/
  #ifdef PC_BUILD
    void btnReleased(GtkWidget *notUsed, GdkEvent *event, gpointer data) {
      jm_show_calc_state("##### keyboard.c: btnReleased begin");

      int16_t item;
      Shft_timeouts = false;                         //JM SHIFT NEW
      JM_auto_longpress_enabled = 0;                 //JM TIMER CLRCLSTK ON LONGPRESS

      if(showFunctionNameItem != 0) {
        item = showFunctionNameItem;
        hideFunctionName();
        //printf("^^^^ item=%d\n",item);
        if(item < 0) {
          showSoftmenu(NULL, item, MS_PUSH);
        }
        else {
          runFunction(item);
        }
      }

      if(!checkShifts((char *)data)) {
        printf(">>> btnReleased (%s):   refreshScreen from keyboard.c  which is the main normal place for it.\n", (char *)data);
        jm_show_calc_state("      ##### keyboard.c: btnReleased end");
        refreshScreen(); //JM PROBLEM. THIS MUST BE REMOVED FOR MOST CASES
      }
    }
  #endif // PC_BUILD

  #ifdef DMCP_BUILD
    void btnReleased(void *data) {
      int16_t item;
      Shft_timeouts = false;                         //JM SHIFT NEW
      JM_auto_longpress_enabled = 0;                 //JM TIMER CLRCLSTK ON LONGPRESS
      if(showFunctionNameItem != 0) {
        item = showFunctionNameItem;
        hideFunctionName();
        if(item < 0) {
          showSoftmenu(NULL, item, MS_PUSH);
        }
        else {
          runFunction(item);
        }
      }
//      else if(keyAutoRepeat) {         //JMTOCHECK AUTOREPR
//        btnPressed(data);
//      }

  if(!checkShifts((char *)data)) {
    refreshScreen(); //JM PROBLEM. THIS MUST BE REMOVED FOR MOST CASES
    }
  }
  #endif //DMCP_BUILD



  /********************************************//**
   * \brief A calc button was pressed
   *
   * \param w GtkWidget*
   * \param data gpointer pointer to a string containing the key number pressed: 00=1/x, ..., 36=EXIT
   * \return void
   ***********************************************/
  void processKeyAction(int16_t item) {
    keyActionProcessed = false;

    if(lastErrorCode != 0 && item != ITM_EXIT1 && item != ITM_BACKSPACE) {
      lastErrorCode = 0;
    }

    temporaryInformation = TI_NO_INFO;

    switch(item) {
      case ITM_BACKSPACE:

        keyActionProcessed = true;   //JM move this to before fnKeyBackspace to allow fnKeyBackspace to cancel it if needed to allow this function via timing out to NOP, and this is incorporated with the CLRDROP
        fnKeyBackspace(NOPARAM);
        if(calcMode == CM_NIM || calcMode == CM_AIM) refreshRegisterLine(NIM_REGISTER_LINE); //JM No if needed, it does nothing if not in NIM. TO DISPLAY NUMBER KEYPRESS DIRECTLY AFTER PRESS, NOT ONLY UPON RELEASE          break;
        break;

    case ITM_UP1:
      keyActionProcessed = true;   //JM swapped to before fnKeyUp to be able to check if key was processed below. Chose to process it here, as fnKeyUp does not have access to item.
      fnKeyUp(NOPARAM);
      if(!keyActionProcessed){     //JMvv
         keyActionProcessed = true;
         addItemToBuffer(ITM_UP_ARROW);    //Let the arrows produce arrow up and arrow down in ALPHA mode
      }                            //JM^^
      break;

    case ITM_DOWN1:
      keyActionProcessed = true;   //swapped to before fnKeyUp to be able to check if key was processed below. Chose to process it here, as fnKeyUp does not have access to item.
      fnKeyDown(NOPARAM);
      if(!keyActionProcessed){     //JM
         keyActionProcessed = true;
         addItemToBuffer(ITM_DOWN_ARROW);    //Let the arrows produce arrow up and arrow down in ALPHA mode
      }                            //JM^^
      break;


    case ITM_BST:                    //JMvv used for arrows in AIM
      if(calcMode == CM_AIM) {
        keyActionProcessed = true;
        fnT_ARROW(ITM_T_LEFT_ARROW);
      } else
      if(calcMode == CM_GRAPH  || calcMode == CM_LISTXY) {
        keyActionProcessed = true;
      }
      if(!keyActionProcessed){
         keyActionProcessed = true;
         addItemToBuffer(ITM_BST);
      }
      break;

    case ITM_SST:
      if(calcMode == CM_AIM) {
        keyActionProcessed = true;
        fnT_ARROW(ITM_T_RIGHT_ARROW);
      } else
      if(calcMode == CM_GRAPH || calcMode == CM_LISTXY) {
        keyActionProcessed = true;
      }
      if(!keyActionProcessed){
         keyActionProcessed = true;
         addItemToBuffer(ITM_SST);
      }     
      break;                       //JM^^


      case ITM_EXIT1:
        fnKeyExit(NOPARAM);
        keyActionProcessed = true;
        break;

      case ITM_CC:
      case ITM_ENTER:
      case ITM_dotD:
        if(calcMode == CM_REGISTER_BROWSER || calcMode == CM_FLAG_BROWSER || calcMode == CM_FLAG_BROWSER_OLD || calcMode == CM_FONT_BROWSER || calcMode == CM_GRAPH  || calcMode == CM_LISTXY) {  //JM added mode
          keyActionProcessed = true;
        }
        break;

    case CHR_case: {
      int16_t sm = softmenu[softmenuStack[softmenuStackPointer].softmenuId].menuItem;                                      //JMvv
      if(alphaCase == AC_LOWER) {
        alphaCase = AC_UPPER;
        if(sm == -MNU_alpha_omega || sm == -MNU_ALPHAintl) {
          softmenuStack[softmenuStackPointer].softmenuId--; // Switch case menu
        }
      } else {
        alphaCase = AC_LOWER;
        if(sm == -MNU_ALPHA_OMEGA || sm == -MNU_ALPHAINTL) {
          softmenuStack[softmenuStackPointer].softmenuId++; // Switch case menu
        }
      }
      showAlphaModeonGui(); //dr JM, see keyboardtweaks
//JMXX      showSoftmenuCurrentPart();
      keyActionProcessed = true;
      }
      break;                                                                                                               //JM^^

      default:
        switch(calcMode) {
          case CM_NORMAL:
            if(item == ITM_EXPONENT || item==ITM_PERIOD || ((ITM_0<=item && item<=ITM_9) || ((ITM_A <= item && item <= ITM_F) && lastIntegerBase >= 11) ) ) { //JMNIM Added direct A-F for hex entry
              addItemToNimBuffer(item);
              keyActionProcessed = true;
              refreshRegisterLine(REGISTER_X);           //JM to force direct display
            }
            // Following commands do not timeout to NOP
            else if(/*item == ITM_UNDO ||JM*/ item == ITM_BST || item == ITM_SST || item == ITM_PR || item == ITM_AIM) {     //UNDO removed from if as it should time out
              runFunction(item);
              keyActionProcessed = true;
            }
            break;

          case CM_AIM:
            if(alphaCase == AC_LOWER && (ITM_A <= item && item <= ITM_Z)) {
              addItemToBuffer(item + 26);
              keyActionProcessed = true;
            }

            else if((ITM_A <= item && item <= ITM_Z) || item == ITM_COLON || item == ITM_COMMA || item == ITM_QUESTION_MARK || item == ITM_SPACE || item == ITM_UNDERSCORE )  {  //JM vv DIRECT LETTERS
              addItemToBuffer(item);
              keyActionProcessed = true;
            }                                           //JM ^^

            else if(alphaCase == AC_LOWER && ( (ITM_ALPHA <= item && item <= ITM_OMEGA) || (ITM_QOPPA <= item && item <= ITM_SAMPI) ))  {  //JM GREEK
              addItemToBuffer(item + 36);
              keyActionProcessed = true;
            }

            else if(item == ITM_DOWN_ARROW) {
              nextChar = NC_SUBSCRIPT;
              keyActionProcessed = true;
            }

            else if(item == ITM_UP_ARROW) {
              nextChar = NC_SUPERSCRIPT;
              keyActionProcessed = true;
            }
            refreshRegisterLine(AIM_REGISTER_LINE);   //JM  No if needed, it does nothing if not in NIM. TO DISPLAY NUMBER KEYPRESS DIRECTLY AFTER PRESS, NOT ONLY UPON RELEASE          break;
            break;

          case CM_TAM:
          case CM_TAM_OVER_PEM:
            addItemToBuffer(item);
            keyActionProcessed = true;
            break;

          case CM_ASM:
          case CM_ASM_OVER_TAM:
          case CM_ASM_OVER_TAM_OVER_PEM:
          case CM_ASM_OVER_AIM:
          case CM_ASM_OVER_PEM:
            if(alphaCase==AC_LOWER && (ITM_A<=item && item<=ITM_Z)) {
              addItemToBuffer(item + 26);
              keyActionProcessed = true;
            }

            else if(alphaCase==AC_LOWER && ((ITM_ALPHA<=item && item<=ITM_OMEGA) || (ITM_QOPPA<=item && item<=ITM_SAMPI) ))  {  //JM GREEK
              addItemToBuffer(item + 36);
              keyActionProcessed = true;
            }

            else if(item == ITM_DOWN_ARROW || item == ITM_UP_ARROW) {
              addItemToBuffer(item);
              keyActionProcessed = true;
            }
            break;

          case CM_NIM:
            keyActionProcessed = true;
            addItemToNimBuffer(item);

            if( ((ITM_0 <= item && item <= ITM_9) || ((ITM_A <= item && item <= ITM_F) && lastIntegerBase >= 11) ) || item == ITM_CHS || item == ITM_EXPONENT || item == ITM_PERIOD) {   //JMvv Direct keypresses; //JMNIM Added direct A-F for hex entry
              refreshRegisterLine(REGISTER_X);
            }                                                                                   //JM^^
            break;

          case CM_REGISTER_BROWSER:
            if(item == ITM_PERIOD) {
              rbr1stDigit = true;
              if(rbrMode == RBR_GLOBAL) {
                if(allLocalRegisterPointer->numberOfLocalRegisters > 0) {
                  rbrMode = RBR_LOCAL;
                  currentRegisterBrowserScreen = FIRST_LOCAL_REGISTER;
                }
                else if(allNamedVariablePointer->numberOfNamedVariables > 0) {
                  rbrMode = RBR_NAMED;
                  currentRegisterBrowserScreen = FIRST_NAMED_VARIABLE;
                }
              }
              else if(rbrMode == RBR_LOCAL) {
                if(allNamedVariablePointer->numberOfNamedVariables > 0) {
                  rbrMode = RBR_NAMED;
                  currentRegisterBrowserScreen = FIRST_NAMED_VARIABLE;
                }
                else {
                  rbrMode = RBR_GLOBAL;
                  currentRegisterBrowserScreen = REGISTER_X;
                }
              }
              else if(rbrMode == RBR_NAMED) {
                rbrMode = RBR_GLOBAL;
                currentRegisterBrowserScreen = REGISTER_X;
              }
            }
            else if(item == ITM_RS) {
              rbr1stDigit = true;
              showContent = !showContent;
            }
            else if(item == ITM_RCL) {
              rbr1stDigit = true;
              if(rbrMode == RBR_GLOBAL || rbrMode == RBR_LOCAL) {
                calcMode = previousCalcMode;
                fnRecall(currentRegisterBrowserScreen);
                setSystemFlag(FLAG_ASLIFT);
              }
              else if(rbrMode == RBR_NAMED) {
              }
            }
            else if(ITM_0 <= item && item <= ITM_9 && (rbrMode == RBR_GLOBAL || rbrMode == RBR_LOCAL)) {
              if(rbr1stDigit) {
                rbr1stDigit = false;
                rbrRegister = item - ITM_0;
              }
              else {
                rbr1stDigit = true;
                rbrRegister = rbrRegister*10 + item - ITM_0;

                if(rbrMode == RBR_GLOBAL) {
                  currentRegisterBrowserScreen = rbrRegister;
                }
                else {
                  rbrRegister = (rbrRegister >= allLocalRegisterPointer->numberOfLocalRegisters ? 0 : rbrRegister);
                  currentRegisterBrowserScreen = FIRST_LOCAL_REGISTER + rbrRegister;
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

          case CM_LISTXY:                     //JM VV
          case CM_GRAPH:
            if(item == ITM_EXIT1 || item == ITM_BACKSPACE) {
              calcMode = previousCalcMode;
            }
            keyActionProcessed = true;
            break;                            //JM ^^


          case CM_CONFIRMATION:
            if(item == ITEM_CONF_Y || item == ITM_XEQ) { // Yes or XEQ
              calcMode = previousCalcMode;
              temporaryInformation = TI_NO_INFO;
              confirmedFunction(CONFIRMED);
            }

            else if(item == ITEM_CONF_N || item == ITM_EXIT1) { // No
              calcMode = previousCalcMode;
              temporaryInformation = TI_NO_INFO;
            }

            keyActionProcessed = true;
            break;

          case CM_PEM:
            break;

          default:
            sprintf(errorMessage, "In function processKeyAction: %" PRIu8 " is an unexpected value while processing calcMode!", calcMode);
            displayBugScreen(errorMessage);
        }
    }
          #ifdef RECORDLOG
          if(keyActionProcessed) {                         //JMEXEC
            capture_sequence("keyActionProcessed:", item);  //JMEXEC
          }                                                //JMEXEC
          #endif
        }
#endif // TESTSUITE_BUILD



/********************************************//**
 * \brief Processing ENTER key
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnKeyEnter(uint16_t unusedButMandatoryParameter) {
  doRefreshSoftMenu = true;     //dr
  #ifndef TESTSUITE_BUILD
  switch(calcMode) {
    case CM_NORMAL:
      if( !eRPN ) {                                    //JM NEWERPN
        setSystemFlag(FLAG_ASLIFT);

        liftStack();
        copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
        //printf("ERPN--1\n");
        clearSystemFlag(FLAG_ASLIFT);
      }                                               //JM NEWERPN vv
      else {
        if(getSystemFlag(FLAG_ASLIFT)) {
          liftStack();
          copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
          //printf("ERPN--2\n");
        }   
      clearSystemFlag(FLAG_ASLIFT);                   //JM NEWERPN (COMMENT: THESE ARE NOT NEEDED AS IT GET OVERWRITTEN BY RUNFN)
      }                                               //JM NEWERPN ^^
      break;

      case CM_AIM:
      case CM_ASM_OVER_AIM:
        calcModeNormal();
        while(softmenuStackPointer > softmenuStackPointerBeforeAIM) {
          popSoftmenu();
        }

        if(aimBuffer[0] == 0) {
          undo();
        }
        else {
          int16_t len = stringByteLength(aimBuffer) + 1;

          reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len), AM_NONE);
          xcopy(REGISTER_STRING_DATA(REGISTER_X), aimBuffer, len);

          if( !eRPN ) {                                    //JM NEWERPN
            setSystemFlag(FLAG_ASLIFT);
            liftStack();
            clearSystemFlag(FLAG_ASLIFT);

            copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
            aimBuffer[0] = 0;
          } else {
              setSystemFlag(FLAG_ASLIFT);
              aimBuffer[0] = 0;
          }
        }
        break;

      case CM_NIM:
        closeNim();

      if( !eRPN ) {                                    //JM NEWERPN
        setSystemFlag(FLAG_ASLIFT);
        liftStack();
        clearSystemFlag(FLAG_ASLIFT);

        copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
      } else {
        if(getSystemFlag(FLAG_ASLIFT)) {
          liftStack();
          clearSystemFlag(FLAG_ASLIFT);
          copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
        }
      }
      break;

      case CM_TAM:
      case CM_TAM_OVER_PEM:
        tamTransitionSystem(TT_ENTER);
        break;

      case CM_ASM:
      case CM_ASM_OVER_TAM:
      case CM_ASM_OVER_TAM_OVER_PEM:
      case CM_ASM_OVER_PEM:
      case CM_REGISTER_BROWSER:
      case CM_FLAG_BROWSER:
      case CM_FONT_BROWSER:
      case CM_ERROR_MESSAGE:
      case CM_BUG_ON_SCREEN:
      case CM_FLAG_BROWSER_OLD:           //JM
      case CM_LISTXY:                     //JM
      case CM_GRAPH:                      //JM
        break;

      case CM_CONFIRMATION:
        calcMode = previousCalcMode;
        confirmedFunction(CONFIRMED);
        break;

      default:
        sprintf(errorMessage, "In function fnKeyEnter: unexpected calcMode value (%" PRIu8 ") while processing key ENTER!", calcMode);
        displayBugScreen(errorMessage);
    }
  #endif // TESTSUITE_BUILD
}



/********************************************//**
 * \brief Processing EXIT key
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnKeyExit(uint16_t unusedButMandatoryParameter) {
#ifndef TESTSUITE_BUILD
  int16_t tmp1 = softmenu[softmenuStack[softmenuStackPointer].softmenuId].menuItem;            //JM
  doRefreshSoftMenu = true;     //dr
  
  #ifdef PC_BUILD
    jm_show_calc_state("fnKeyExit");
  #endif

  switch(calcMode) {
    case CM_NORMAL:
      if(lastErrorCode != 0) {
        lastErrorCode = 0;
      }
      else if(softmenuStackPointer > 0) {
            popSoftmenu();
      }
      break;

      case CM_AIM:
        if(softmenuStackPointer <= 0 || tmp1 == -MNU_ALPHA || tmp1 == -MNU_T_EDIT || tmp1 == -MNU_MyAlpha) {  //JM
        if(softmenuStackPointer >  0 && tmp1 == -MNU_T_EDIT) {popSoftmenu();}    //JM
        popSoftmenu();
        calcModeNormal();

          if(aimBuffer[0] == 0) {
            undo();
          }
          else {
            int16_t len = stringByteLength(aimBuffer) + 1;

            reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len), AM_NONE);

            xcopy(REGISTER_STRING_DATA(REGISTER_X), aimBuffer, len);
            aimBuffer[0] = 0;

            setSystemFlag(FLAG_ASLIFT);
          }
        }
      break;

      case CM_NIM:
        addItemToNimBuffer(ITM_EXIT1);
        break;

      case CM_TAM:
      case CM_TAM_OVER_PEM:
      case CM_ASM:
        calcModeNormal();
        break;

      case CM_PEM:
        if(softmenuStackPointer > 0) {
          popSoftmenu();
          break;
        }

        if(freeProgramBytes >= 4) { // Push the programs to the end of RAM
          uint32_t newProgramSize = (uint32_t)((uint8_t *)(ram + RAM_SIZE) - beginOfProgramMemory) - (freeProgramBytes & 0xfffc);
          currentStep        += (freeProgramBytes & 0xfffc);
          firstDisplayedStep += (freeProgramBytes & 0xfffc);
          freeProgramBytes &= 0x03;
          resizeProgramMemory(TO_BLOCKS(newProgramSize));
          scanLabelsAndPrograms();
        }

        calcModeNormal();
        break;

      case CM_ASM_OVER_TAM:
      case CM_ASM_OVER_TAM_OVER_PEM:
        transitionSystemState = 0;
        calcModeTam();
        sprintf(tamBuffer, "%s __", indexOfItems[getOperation()].itemCatalogName);
        tamTransitionSystem(TT_NOTHING);

        if(tmp1 == -MNU_SYSFL) {                                                       //JM auto recover out of SYSFL
          calcModeNormal();
         }                                                                             //JM auto recover out of SYSFL
        break;

      case CM_ASM_OVER_AIM:
        calcModeAim(NOPARAM);
        break;

      case CM_ASM_OVER_PEM: // TODO: is that correct
        calcModeNormal();
        calcMode = CM_PEM;
        break;

    case CM_REGISTER_BROWSER:
    case CM_FLAG_BROWSER:
    case CM_FLAG_BROWSER_OLD:           //JM
    case CM_FONT_BROWSER:
      rbr1stDigit = true;
      calcMode = previousCalcMode;
      break;

    case CM_BUG_ON_SCREEN:
    case CM_LISTXY:                     //JM
      calcMode = previousCalcMode;
      break;

    case CM_GRAPH:                      //JM vv
      calcMode = previousCalcMode;
      softmenuStack[softmenuStackPointer].firstItem = 0;
      break;                            //JM ^^

    case CM_CONFIRMATION:
      calcMode = previousCalcMode;
      temporaryInformation = TI_NO_INFO;
      break;

      default:
        sprintf(errorMessage, "In function fnKeyExit: unexpected calcMode value (%" PRIu8 ") while processing key EXIT!", calcMode);
        displayBugScreen(errorMessage);
    }
  #endif // TESTSUITE_BUILD
}



/********************************************//**
 * \brief Processing CC key
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnKeyCC(uint16_t complex_Type) {    //JM Using 'unusedButMandatoryParameter' complex_Type=KEY_COMPLEX
  doRefreshSoftMenu = true;     //dr
  #ifndef TESTSUITE_BUILD
    uint32_t dataTypeX;
    uint32_t dataTypeY;

  // The switch statement is broken up here, due to multiple conditions.                      //JM
  if((calcMode == CM_NIM) && (complex_Type == KEY_COMPLEX)) addItemToNimBuffer(ITM_EXIT1);    //JM Allow COMPLEX to be used from NIM
  if(calcMode == CM_NORMAL || ((calcMode == CM_NIM) && (complex_Type == KEY_COMPLEX))) {      //JM
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
          moreInfoOnError("In function fnKeyCC:", errorMessage, NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      }
      return;                            //JM
  }

  switch(calcMode) {                     //JM
    case CM_NIM:
      addItemToNimBuffer(ITM_CC);
      break;

      case CM_ASM:
      case CM_ASM_OVER_TAM:
      case CM_ASM_OVER_TAM_OVER_PEM:
      case CM_ASM_OVER_AIM:
      case CM_ASM_OVER_PEM:
      case CM_REGISTER_BROWSER:
      case CM_FLAG_BROWSER:
      case CM_FONT_BROWSER:
      case CM_FLAG_BROWSER_OLD:           //JM
      case CM_LISTXY:                     //JM
      case CM_GRAPH:                      //JM
        break;

      default:
        sprintf(errorMessage, "In function fnKeyCC: unexpected calcMode value (%" PRIu8 ") while processing key CC!", calcMode);
        displayBugScreen(errorMessage);
    }
  #endif // TESTSUITE_BUILD
}



/********************************************//**
 * \brief Processing BACKSPACE key
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnKeyBackspace(uint16_t unusedButMandatoryParameter) {
  #ifndef TESTSUITE_BUILD
    uint16_t lg;
    uint8_t *nextStep;

    switch(calcMode) {
      case CM_NORMAL:
        if(lastErrorCode != 0) {
          lastErrorCode = 0;
        }
        else {
          runFunction(ITM_CLX);
        }
        break;

    case CM_AIM:
      if(stringByteLength(aimBuffer) > 0) {

//JMCURSORvv SPLIT STRING AT CURSOR POSITION
          uint8_t T_cursorPos_tmp;
          T_cursorPos_tmp = aimBuffer[T_cursorPos];
          aimBuffer[T_cursorPos] = 0;                  //break it at the current cursor
          lg = stringLastGlyph(aimBuffer);             //find beginning of last glyoh, to delete
          aimBuffer[lg] = 0;                           //delete it
          aimBuffer[T_cursorPos] = T_cursorPos_tmp;    //Restore broken glyph in middle at break point
          uint16_t ix = 0;
          while(aimBuffer[ix+T_cursorPos] != 0) {      //copy second part to append to first part
            aimBuffer[ix+lg] = aimBuffer[ix+T_cursorPos];
            ix++;
          }
          aimBuffer[ix+lg]=0;                          //end new buffer
          //fnT_ARROW(ITM_T_LEFT_ARROW);                               //move cursor one left; removed on 2020-12-01
//JMCURSOR^^
      }
      break;

      case CM_ASM_OVER_AIM:
        if(stringByteLength(aimBuffer) > 0) {
          lg = stringLastGlyph(aimBuffer);
          aimBuffer[lg] = 0;
          xCursor = showString(aimBuffer, &standardFont, 1, Y_POSITION_OF_AIM_LINE + 6, vmNormal, true, true);
        }
        break;

      case CM_NIM:
        addItemToNimBuffer(ITM_BACKSPACE);
        break;

      case CM_TAM:
      case CM_TAM_OVER_PEM:
        tamTransitionSystem(TT_BACKSPACE);
        break;

      case CM_ASM:
        addItemToBuffer(ITM_BACKSPACE);
        break;

      case CM_ASM_OVER_TAM:
      case CM_ASM_OVER_TAM_OVER_PEM:
      case CM_ASM_OVER_PEM:
        break;

    case CM_REGISTER_BROWSER:
    case CM_FLAG_BROWSER:
    case CM_FLAG_BROWSER_OLD:           //JM
    case CM_FONT_BROWSER:
      calcMode = previousCalcMode;
      break;

      case CM_BUG_ON_SCREEN:
        calcMode = previousCalcMode;
        break;

    case CM_LISTXY:                     //JM vv
    case CM_GRAPH:
      calcMode = previousCalcMode;
      keyActionProcessed = true;
      break;                            //JM ^^

    case CM_CONFIRMATION:
      calcMode = previousCalcMode;
      temporaryInformation = TI_NO_INFO;
      break;

      case CM_PEM:
        nextStep = findNextStep(currentStep);
        if(*nextStep != 255 || *(nextStep + 1) != 255) { // Not the last END
          deleteStepsFromTo(currentStep, nextStep);
        }
        break;

      default:
        sprintf(errorMessage, "In function fnKeyBackspace: unexpected calcMode value (%" PRIu8 ") while processing key BACKSPACE!", calcMode);
        displayBugScreen(errorMessage);
    }
  #endif // TESTSUITE_BUILD
}



#ifndef TESTSUITE_BUILD
  static void menuUp(void) {
    int16_t menuId = softmenuStack[softmenuStackPointer].softmenuId;
    int16_t sm = softmenu[menuId].menuItem;
    if((sm == -MNU_alpha_omega || sm == -MNU_ALPHAintl) && alphaCase == AC_LOWER) {
      alphaCase = AC_UPPER;
      softmenuStack[softmenuStackPointer].softmenuId--; // Switch to the upper case menu
    }
    else if((sm == -MNU_ALPHADOT || sm == -MNU_ALPHAMATH) && alphaCase == AC_LOWER) {
      alphaCase = AC_UPPER;
    }
    else {
      int16_t itemShift = (alphaSelectionMenu == ASM_NONE ? 18 : 6);

      if((softmenuStack[softmenuStackPointer].firstItem + itemShift) < (menuId < NUMBER_OF_DYNAMIC_SOFTMENUS ? dynamicSoftmenu[menuId].numItems : softmenu[menuId].numItems)) {
        softmenuStack[softmenuStackPointer].firstItem += itemShift;
      }
      else {
        softmenuStack[softmenuStackPointer].firstItem = 0;
      }

      setCatalogLastPos();
    }
  }


static void menuUp_org(void) {

      if(softmenuStackPointer > 0  && softmenuStack[softmenuStackPointer].softmenuId != MY_ALPHA_MENU) { // || softmenu[softmenuStack[softmenuStackPointer].softmenuId].menuId != -MNU_ALPHA)) {
        int16_t sm = softmenu[softmenuStack[softmenuStackPointer].softmenuId].menuItem;
        if((sm == -MNU_alpha_omega || sm == -MNU_ALPHAintl) && alphaCase == AC_LOWER && arrowCasechange) {  //JMcase
          alphaCase = AC_UPPER;
          showAlphaModeonGui(); //dr JM, see keyboardtweaks
          softmenuStack[softmenuStackPointer].softmenuId--; // Switch to the upper case menu
        }

        else if((sm == -MNU_ALPHADOT || sm == -MNU_ALPHAMATH || sm == -MNU_ALPHA) && alphaCase == AC_LOWER && arrowCasechange) {  //JMcase
          alphaCase = AC_UPPER;
          showAlphaModeonGui(); //dr JM, see keyboardtweaks
        }
        else {
          int16_t itemShift = alphaSelectionMenu == ASM_NONE ? 18 : 6;

          if((softmenuStack[softmenuStackPointer].firstItem + itemShift) < softmenu[softmenuStack[softmenuStackPointer].softmenuId].numItems) {         //JM
            softmenuStack[softmenuStackPointer].firstItem += itemShift;
            //JM Include or exclude HOME menu screens  //JMHOME
            #define A1 3   //HAAKON //jm_HOME_MIR
            #define A2 9   //HAAKON
            #define B1 10  //NIGEL  //jm_HOME_SUM
            #define B2 11  //NIGEL
            #define C1 12  //JACO   //jm_HOME_FIX
            #define C2 18  //JACO
            int16_t smm = softmenu[softmenuStack[softmenuStackPointer].softmenuId].menuItem;
            //printf("--1:%d %d %d menuId:%d item:%d  \n",jm_HOME_MIR,jm_HOME_SUM,jm_HOME_FIX,smm,softmenuStack[softmenuStackPointer].firstItem/18);
            if (!jm_HOME_MIR && smm == -MNU_HOME && softmenuStack[softmenuStackPointer].firstItem == A1*18) {softmenuStack[softmenuStackPointer].firstItem = (A2+1)*18;} 
            if (!jm_HOME_SUM && smm == -MNU_HOME && softmenuStack[softmenuStackPointer].firstItem == B1*18) {softmenuStack[softmenuStackPointer].firstItem = (B2+1)*18;} 
            if (!jm_HOME_FIX && smm == -MNU_HOME && softmenuStack[softmenuStackPointer].firstItem == C1*18) {softmenuStack[softmenuStackPointer].firstItem = (C2+1)*18;}
            //smm = softmenu[softmenuStack[softmenuStackPointer].softmenuId].menuId;
            //printf(   "--2:      menuId:%d item:%d  \n",smm,softmenuStack[softmenuStackPointer].firstItem/18);
          }
          else {
            softmenuStack[softmenuStackPointer].firstItem = 0;
          }

          setCatalogLastPos();
        }

      }
}
#endif // TESTSUITE_BUILD

/********************************************//**
 * \brief Processing UP key
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnKeyUp(uint16_t unusedButMandatoryParameter) {
  doRefreshSoftMenu = true;     //dr
  #ifndef TESTSUITE_BUILD  
  if(calcMode == CM_NORMAL && softmenuStackPointer == 0)  {fnShow_SCROLL(1); return;}             //JMSHOW
  int16_t menuId;

  switch(calcMode) {
    case CM_NORMAL:
    case CM_AIM:
    case CM_NIM:
    case CM_ASM:
    case CM_ASM_OVER_TAM:
    case CM_ASM_OVER_TAM_OVER_PEM:
    case CM_ASM_OVER_AIM:
    case CM_ASM_OVER_PEM:
    case CM_GRAPH:                  //JM
      doRefreshSoftMenu = true;     //jm
      resetAlphaSelectionBuffer();
      menuId = softmenuStack[softmenuStackPointer].softmenuId;

        //JM Arrow up and down if no menu other than AHOME of MyA       //JMvv
        if(!arrowCasechange && (softmenuStackPointer > 0) && (
          menuId == mm_MNU_ALPHA        ||
          menuId == MY_ALPHA_MENU_CNST  ||
          softmenu[menuId].menuItem == -MNU_T_EDIT)) {fnT_ARROW(ITM_UP1);}
              //ignoring the base menu, MY_ALPHA_MENU below
              // make this keyActionProcessed = false; to have arrows up and down placed in bufferize
              // make arrowCasechnage true
                                                                       //JM^^
        else

        if(softmenuStackPointer > 0 && (   (menuId <  NUMBER_OF_DYNAMIC_SOFTMENUS && dynamicSoftmenu[menuId].numItems > 18)
                                        || (menuId >= NUMBER_OF_DYNAMIC_SOFTMENUS &&        softmenu[menuId].numItems > 18))) {
        menuUp_org();
      }
      else {
//        if(alphaCase != AC_UPPER) {
  //        alphaCase = AC_UPPER;
    //      showAlphaModeonGui(); //dr JM, see keyboardtweaks
      //  }
      }
      break;

      case CM_TAM:
      case CM_TAM_OVER_PEM:
        addItemToBuffer(ITM_Max);
        break;

      case CM_REGISTER_BROWSER:
        rbr1stDigit = true;
        if(rbrMode == RBR_GLOBAL) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen + 1, FIRST_LOCAL_REGISTER);
        }
        else if(rbrMode == RBR_LOCAL) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - FIRST_LOCAL_REGISTER + 1, allLocalRegisterPointer->numberOfLocalRegisters) + FIRST_LOCAL_REGISTER;
        }
        else if(rbrMode == RBR_NAMED) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - FIRST_NAMED_VARIABLE + 1, allNamedVariablePointer->numberOfNamedVariables) + FIRST_NAMED_VARIABLE;
        }
        else {
          sprintf(errorMessage, "In function fnKeyUp: unexpected case while processing key UP! %" PRIu8 " is an unexpected value for rbrMode.", rbrMode);
          displayBugScreen(errorMessage);
        }
        break;

    case CM_FLAG_BROWSER:
      currentFlgScr--;                          //JM removed the 3-x part
     break;

    case CM_FLAG_BROWSER_OLD:                   //JMvv
      currentFlgScr--;
      break;                                    //JM^^

    case CM_FONT_BROWSER:
      if(currentFntScr >= 2) {
        currentFntScr--;
      }
      break;

      case CM_PEM:
        resetAlphaSelectionBuffer();
        menuId = softmenuStack[softmenuStackPointer].softmenuId;
        if(softmenuStackPointer > 0 && (   (menuId <  NUMBER_OF_DYNAMIC_SOFTMENUS && dynamicSoftmenu[menuId].numItems > 18)
                                        || (menuId >= NUMBER_OF_DYNAMIC_SOFTMENUS &&        softmenu[menuId].numItems > 18))) {
          menuUp();
        }
        else {
          fnBst(NOPARAM);
        }
        break;

      case CM_LISTXY:                     //JM vv
        ListXYposition += 10;
        break;                            //JM ^^
          
      default:
        sprintf(errorMessage, "In function fnKeyUp: unexpected calcMode value (%" PRIu8 ") while processing key UP!", calcMode);
        displayBugScreen(errorMessage);
    }
  #endif // TESTSUITE_BUILD
}



#ifndef TESTSUITE_BUILD
  static void menuDown(void) {
    int16_t menuId = softmenuStack[softmenuStackPointer].softmenuId;
    int16_t sm = softmenu[menuId].menuItem;
    if((sm == -MNU_ALPHA_OMEGA || sm == -MNU_ALPHAINTL) && alphaCase == AC_UPPER) {
      alphaCase = AC_LOWER;
      softmenuStack[softmenuStackPointer].softmenuId++; // Switch to the lower case menu
    }
    else if((sm == -MNU_ALPHADOT || sm == -MNU_ALPHAMATH) && alphaCase == AC_UPPER) {
      alphaCase = AC_LOWER;
    }
    else {
      int16_t itemShift = (alphaSelectionMenu == ASM_NONE ? 18 : 6);

      if((softmenuStack[softmenuStackPointer].firstItem - itemShift) >= 0) {
        softmenuStack[softmenuStackPointer].firstItem -= itemShift;
      }
      else if((softmenuStack[softmenuStackPointer].firstItem - itemShift) >= -5) {
        softmenuStack[softmenuStackPointer].firstItem = 0;
      }
      else {
        if(menuId < NUMBER_OF_DYNAMIC_SOFTMENUS) {
          softmenuStack[softmenuStackPointer].firstItem = ((dynamicSoftmenu[menuId].numItems - 1)/6) / (itemShift/6) * itemShift;
        }
        else {
          softmenuStack[softmenuStackPointer].firstItem = ((       softmenu[menuId].numItems - 1)/6) / (itemShift/6) * itemShift;
        }
      }

      setCatalogLastPos();
    }
  }


void menuDown_org(void) {

      if(softmenuStackPointer > 0  && softmenuStack[softmenuStackPointer].softmenuId != MY_ALPHA_MENU) { //&& softmenu[softmenuStack[softmenuStackPointer].softmenuId].menuId != -MNU_T_EDIT)) { // || softmenu[softmenuStack[softmenuStackPointer].softmenuId].menuId != -MNU_ALPHA)) {
        int16_t sm = softmenu[softmenuStack[softmenuStackPointer].softmenuId].menuItem;
        if((sm == -MNU_ALPHA_OMEGA || sm == -MNU_ALPHAINTL) && alphaCase == AC_UPPER && arrowCasechange) {  //JMcase
          alphaCase = AC_LOWER;
          showAlphaModeonGui(); //dr JM, see keyboardtweaks
          softmenuStack[softmenuStackPointer].softmenuId++; // Switch to the lower case menu
        }
        else if((sm == -MNU_ALPHADOT || sm == -MNU_ALPHAMATH || sm == -MNU_ALPHA) && alphaCase == AC_UPPER && arrowCasechange) {  //JMcase
          alphaCase = AC_LOWER;
          showAlphaModeonGui(); //dr JM, see keyboardtweaks
        }
      else {
          int16_t itemShift = alphaSelectionMenu == ASM_NONE ? 18 : 6;

          if((softmenuStack[softmenuStackPointer].firstItem - itemShift) >= 0) {
            softmenuStack[softmenuStackPointer].firstItem -= itemShift;
            //JM Include or exclude HOME menu screens  //JMHOME
            int16_t smm = softmenu[softmenuStack[softmenuStackPointer].softmenuId].menuItem;
            if (!jm_HOME_FIX && smm == -MNU_HOME && softmenuStack[softmenuStackPointer].firstItem == C2*18) {softmenuStack[softmenuStackPointer].firstItem = (C1-1)*18;}
            if (!jm_HOME_SUM && smm == -MNU_HOME && softmenuStack[softmenuStackPointer].firstItem == B2*18) {softmenuStack[softmenuStackPointer].firstItem = (B1-1)*18;} 
            if (!jm_HOME_MIR && smm == -MNU_HOME && softmenuStack[softmenuStackPointer].firstItem == A2*18) {softmenuStack[softmenuStackPointer].firstItem = (A1-1)*18;}
          }
          else {
            if((softmenuStack[softmenuStackPointer].firstItem - itemShift) >= -5) {
              softmenuStack[softmenuStackPointer].firstItem = 0;
            }
            else {
              softmenuStack[softmenuStackPointer].firstItem = ((softmenu[softmenuStack[softmenuStackPointer].softmenuId].numItems - 1)/6) / (itemShift/6) * itemShift;
            }
          }

          setCatalogLastPos();
        }
      }
}
#endif // TESTSUITE_BUILD


/********************************************//**
 * \brief Processing DOWN key
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnKeyDown(uint16_t unusedButMandatoryParameter) {
  doRefreshSoftMenu = true;     //dr
  #ifndef TESTSUITE_BUILD
  int16_t menuId;
  if(calcMode == CM_NORMAL && softmenuStackPointer == 0)  {fnShow_SCROLL(2); return;}             //JMSHOW

  switch(calcMode) {
    case CM_NORMAL:                     //JMSHOW vv
    case CM_AIM:
    case CM_NIM:
    case CM_ASM:
    case CM_ASM_OVER_TAM:
    case CM_ASM_OVER_TAM_OVER_PEM:
    case CM_ASM_OVER_AIM:
    case CM_ASM_OVER_PEM:
    case CM_GRAPH:                  //JM
      doRefreshSoftMenu = true;     //jm
      resetAlphaSelectionBuffer();
      menuId = softmenuStack[softmenuStackPointer].softmenuId;

        //JM Arrow up and down if no menu other than AHOME of MyA       //JMvv
        if(!arrowCasechange && (softmenuStackPointer > 0) && (
          menuId == mm_MNU_ALPHA        ||
          menuId == MY_ALPHA_MENU_CNST  ||
          softmenu[menuId].menuItem == -MNU_T_EDIT)) {fnT_ARROW(ITM_DOWN1);}
              //ignoring the base menu, MY_ALPHA_MENU below
              // make this keyActionProcessed = false; to have arrows up and down placed in bufferize
              // make arrowCasechnage true
                                                                       //JM^^
        else

        if(softmenuStackPointer > 0 && (   (menuId <  NUMBER_OF_DYNAMIC_SOFTMENUS && dynamicSoftmenu[menuId].numItems > 18)
                                        || (menuId >= NUMBER_OF_DYNAMIC_SOFTMENUS &&        softmenu[menuId].numItems > 18))) {
          menuDown();
        }
        else {
//          if(alphaCase != AC_LOWER) {
  //          alphaCase = AC_LOWER;
    //        showAlphaModeonGui(); //dr JM, see keyboardtweaks
      //    }
        }
        break;

      case CM_TAM:
      case CM_TAM_OVER_PEM:
        addItemToBuffer(ITM_Min);
        break;


      case CM_REGISTER_BROWSER:
        rbr1stDigit = true;
        if(rbrMode == RBR_GLOBAL) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - 1, FIRST_LOCAL_REGISTER);
        }
        else if(rbrMode == RBR_LOCAL) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - FIRST_LOCAL_REGISTER - 1, allLocalRegisterPointer->numberOfLocalRegisters) + FIRST_LOCAL_REGISTER;
        }
        else if(rbrMode == RBR_NAMED) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - 1000 - 1, allNamedVariablePointer->numberOfNamedVariables) + 1000;
        }
        else {
          sprintf(errorMessage, "In function fnKeyDown: unexpected case while processing key DOWN! %" PRIu8 " is an unexpected value for rbrMode.", rbrMode);
          displayBugScreen(errorMessage);
        }
        break;

    case CM_FLAG_BROWSER:
      currentFlgScr++;                          //JM removed the 3-x part
      break;

    case CM_FLAG_BROWSER_OLD:                   //JMvv
      currentFlgScr++;
      break;                                    //JM^^

    case CM_FONT_BROWSER:
      if(currentFntScr < numScreensNumericFont + numScreensStandardFont) {
        currentFntScr++;
      }
      break;

      case CM_PEM:
        resetAlphaSelectionBuffer();
        menuId = softmenuStack[softmenuStackPointer].softmenuId;
        if(softmenuStackPointer > 0 && (   (menuId <  NUMBER_OF_DYNAMIC_SOFTMENUS && dynamicSoftmenu[menuId].numItems > 18)
                                        || (menuId >= NUMBER_OF_DYNAMIC_SOFTMENUS &&        softmenu[menuId].numItems > 18))) {
          menuDown();
        }
        else {
          fnSst(NOPARAM);
        }
        break;

    case CM_LISTXY:                     //JM vv
      ListXYposition -= 10;
      break;                            //JM ^^
          
    default:
      sprintf(errorMessage, "In function fnKeyDown: unexpected calcMode value (%" PRIu8 ") while processing key DOWN!", calcMode);
      displayBugScreen(errorMessage);
  }
  #endif // TESTSUITE_BUILD
}



/********************************************//**
 * \brief Processing .d key
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnKeyDotD(uint16_t unusedButMandatoryParameter) {
  #ifndef TESTSUITE_BUILD
    switch(calcMode) {
      case CM_NORMAL:
        if(getSystemFlag(FLAG_FRACT)) {
          clearSystemFlag(FLAG_FRACT);
        }
        else {
          runFunction(ITM_toREAL);
        }
        break;

      case CM_NIM:
        #if (EXTRA_INFO_ON_CALC_ERROR == 1)
          moreInfoOnError("In function fnKeyDotD:", "the data type date is to be coded!", NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        break;

      case CM_REGISTER_BROWSER:
      case CM_FLAG_BROWSER:
      case CM_FLAG_BROWSER_OLD:           //JM
      case CM_FONT_BROWSER:
      case CM_LISTXY:                     //JM
      case CM_GRAPH:                      //JM
      break;

      default:
        sprintf(errorMessage, "In function fnKeyDotD: unexpected calcMode value (%" PRIu8 ") while processing key .d!", calcMode);
        displayBugScreen(errorMessage);
    }
  #endif // TESTSUITE_BUILD
}
