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
    int16_t row, menuId = softmenuStack[0].softmenuId;
    int16_t firstItem = softmenuStack[0].firstItem;

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

          if(item == ITM_PROD_SIGN) {
            item = (getSystemFlag(FLAG_MULTx) ? ITM_DOT : ITM_CROSS);
          }
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
      GdkEvent mouseButton;
      mouseButton.button.button = 1;

      btnFnPressed(notUsed, &mouseButton, data);
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
      if(event->button.button == 2) { // Middle click
        shiftF = true;
        shiftG = false;
      }
      if(event->button.button == 3) { // Right click
        shiftF = false;
        shiftG = true;
      }
      if(calcMode != CM_REGISTER_BROWSER && calcMode != CM_FLAG_BROWSER && calcMode != CM_FONT_BROWSER) {
        int16_t item = determineFunctionKeyItem((char *)data);

        shiftF = false;
        shiftG = false;
        if(item != ITM_NOP && item != ITM_NULL) {
          lastErrorCode = 0;

          #if (FN_KEY_TIMEOUT_TO_NOP == 1)
            showFunctionName(item, 1000); // 1000ms = 1s
          #else // (FN_KEY_TIMEOUT_TO_NOP == 0)
            showFunctionNameItem = item;
          #endif // (FN_KEY_TIMEOUT_TO_NOP == 1)
        }
        else {
          showFunctionNameItem = ITM_NOP;
        }
      }
    }
  #endif // PC_BUILD

  #ifdef DMCP_BUILD
    void btnFnPressed(void *data) {
      if(calcMode != CM_REGISTER_BROWSER && calcMode != CM_FLAG_BROWSER && calcMode != CM_FONT_BROWSER) {
        int16_t item = determineFunctionKeyItem((char *)data);

        shiftF = false;
        shiftG = false;
        if(item != ITM_NOP && item != ITM_NULL) {
          lastErrorCode = 0;

          #if (FN_KEY_TIMEOUT_TO_NOP == 1)
            showFunctionName(item, 1000); // 1000ms = 1s
          #else // (FN_KEY_TIMEOUT_TO_NOP == 0)
            showFunctionNameItem = item;
          #endif // (FN_KEY_TIMEOUT_TO_NOP == 1)
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
    if(calcMode != CM_REGISTER_BROWSER && calcMode != CM_FLAG_BROWSER && calcMode != CM_FONT_BROWSER) {
      if(showFunctionNameItem != 0) {
        int16_t item = showFunctionNameItem;
        #if (FN_KEY_TIMEOUT_TO_NOP == 1)
          hideFunctionName();
        #endif // (FN_KEY_TIMEOUT_TO_NOP == 1)

        if(calcMode != CM_CONFIRMATION) {
          lastErrorCode = 0;

          if(item < 0) { // softmenu
            showSoftmenu(item);
            refreshScreen();
            return;
          }
          else if(catalog && tamMode) {
            reallyRunFunction(getOperation(), indexOfItems[item].param); // TODO: check why the param is taken from item and not from getOperation
            leaveTamMode();
            refreshScreen();
            return;
          }
          else if(calcMode == CM_NORMAL && catalog) {
            leaveAsmMode();
          }
          else if(calcMode == CM_AIM && catalog) {
            addItemToBuffer(item);
            refreshScreen();
            return;
          }
          else if(calcMode == CM_PEM && catalog) { // TODO: is that correct
            runFunction(item);
            refreshScreen();
            return;
          }

          if((calcMode == CM_NORMAL || calcMode == CM_NIM) && (ITM_0<=item && item<=ITM_F)) {
            addItemToNimBuffer(item);
          }
          else if(tamMode) {
            addItemToBuffer(item);
          }
          else if(item > 0) { // function
            if(calcMode == CM_NIM && item != ITM_CC) {
              closeNim();
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

      refreshScreen();
    }
  }



  int16_t determineItem(const char *data) {
    int16_t result;
    const calcKey_t *key;

    dynamicMenuItem = -1;
    key = getSystemFlag(FLAG_USER) ? (kbd_usr + (*data - '0')*10 + *(data+1) - '0') : (kbd_std + (*data - '0')*10 + *(data+1) - '0');

    // Shift f pressed and shift g not active
    if(key->primary == ITM_SHIFTf && !shiftG && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_NIM || calcMode == CM_PEM)) {
      temporaryInformation = TI_NO_INFO;
      lastErrorCode = 0;
      shiftF = !shiftF;
      return ITM_NOP;
    }

    // Shift g pressed and shift f not active
    else if(key->primary == ITM_SHIFTg && !shiftF && (calcMode == CM_NORMAL || calcMode == CM_AIM || calcMode == CM_NIM || calcMode == CM_PEM)) {
      temporaryInformation = TI_NO_INFO;
      lastErrorCode = 0;
      shiftG = !shiftG;
      return ITM_NOP;
    }

    if(calcMode == CM_AIM || catalog) {
      result = shiftF ? key->fShiftedAim :
               shiftG ? key->gShiftedAim :
                        key->primaryAim;

    }
    else if(tamMode) {
      result = key->primaryTam; // No shifted function in TAM
    }
    else if(calcMode == CM_NORMAL || calcMode == CM_NIM || calcMode == CM_FONT_BROWSER || calcMode == CM_FLAG_BROWSER || calcMode == CM_REGISTER_BROWSER || calcMode == CM_BUG_ON_SCREEN || calcMode == CM_CONFIRMATION || calcMode == CM_PEM) {
      result = shiftF ? key->fShifted :
               shiftG ? key->gShifted :
                        key->primary;
    }
    else {
      displayBugScreen("In function determineItem: item was not determined!");
      result = 0;
    }

    if(result == ITM_PROD_SIGN) {
      result = (getSystemFlag(FLAG_MULTx) ? ITM_CROSS : ITM_DOT);
    }

    shiftF = false;
    shiftG = false;

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
      showFunctionNameItem = 0;
      int16_t item = determineItem((char *)data);
      if(item != ITM_NOP && item != ITM_NULL) {
        processKeyAction(item);
        if(!keyActionProcessed) {
          showFunctionName(item, 1000); // 1000ms = 1s
        }
      }
    }
  #endif // PC_BUILD

  #ifdef DMCP_BUILD
    void btnPressed(void *data) {
      int16_t item;

      if(keyAutoRepeat) {
        //beep(880, 50);
        item = previousItem;
      }
      else {
        item = determineItem((char *)data);
        previousItem = item;
      }

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
      int16_t item;

      if(showFunctionNameItem != 0) {
        item = showFunctionNameItem;
        hideFunctionName();
        if(item < 0) {
          showSoftmenu(item);
        }
        else {
          runFunction(item);
        }
      }
      refreshScreen();
    }
  #endif // PC_BUILD

  #ifdef DMCP_BUILD
    void btnReleased(void *data) {
      int16_t item;

      if(showFunctionNameItem != 0) {
        item = showFunctionNameItem;
        hideFunctionName();
        if(item < 0) {
          showSoftmenu(item);
        }
        else {
          runFunction(item);
        }
      }
      else if(keyAutoRepeat) {
        btnPressed(data);
      }

      refreshScreen();
    }
  #endif // DMCP_BUILD


  void leavePem(void) {
    if(freeProgramBytes >= 4) { // Push the programs to the end of RAM
      uint32_t newProgramSize = (uint32_t)((uint8_t *)(ram + RAM_SIZE) - beginOfProgramMemory) - (freeProgramBytes & 0xfffc);
      currentStep        += (freeProgramBytes & 0xfffc);
      firstDisplayedStep += (freeProgramBytes & 0xfffc);
      freeProgramBytes &= 0x03;
      resizeProgramMemory(TO_BLOCKS(newProgramSize));
      scanLabelsAndPrograms();
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

    if(lastErrorCode != 0 && item != ITM_EXIT && item != ITM_BACKSPACE) {
      lastErrorCode = 0;
    }

    temporaryInformation = TI_NO_INFO;

    switch(item) {
      case ITM_BACKSPACE:
        fnKeyBackspace(NOPARAM);
        keyActionProcessed = true;
        break;

      case ITM_UP:
        fnKeyUp(NOPARAM);
        keyActionProcessed = true;
        break;

      case ITM_DOWN:
        fnKeyDown(NOPARAM);
        keyActionProcessed = true;
        break;

      case ITM_EXIT:
        fnKeyExit(NOPARAM);
        keyActionProcessed = true;
        break;

      case ITM_CC:
      case ITM_ENTER:
      case ITM_dotD:
        if(calcMode == CM_REGISTER_BROWSER || calcMode == CM_FLAG_BROWSER || calcMode == CM_FONT_BROWSER) {
          keyActionProcessed = true;
        }
        break;

      default:
        if(catalog) {
          if(ITM_A <= item && item <= ITM_Z && alphaCase == AC_LOWER) {
            addItemToBuffer(item + 26);
            keyActionProcessed = true;
          }

          else if(ITM_ALPHA <= item && item <= ITM_OMEGA && alphaCase == AC_LOWER) {
            addItemToBuffer(item + 36);
            keyActionProcessed = true;
          }

          else if(item == ITM_DOWN_ARROW || item == ITM_UP_ARROW) {
            addItemToBuffer(item);
            keyActionProcessed = true;
          }
          break;
        }
        else if(tamMode) {
          addItemToBuffer(item);
          keyActionProcessed = true;
          break;
        }
        else {
          switch(calcMode) {
            case CM_NORMAL:
              if(item == ITM_EXPONENT || item == ITM_PERIOD || (ITM_0 <= item && item <= ITM_9)) {
                addItemToNimBuffer(item);
                keyActionProcessed = true;
              }
              // Following commands do not timeout to NOP
              else if(item == ITM_UNDO || item == ITM_BST || item == ITM_SST || item == ITM_PR || item == ITM_AIM) {
                runFunction(item);
                keyActionProcessed = true;
              }
              break;

            case CM_AIM:
              if(alphaCase == AC_LOWER && (ITM_A <= item && item <= ITM_Z)) {
                addItemToBuffer(item + 26);
                keyActionProcessed = true;
              }

              else if(alphaCase == AC_LOWER && (ITM_ALPHA <= item && item <= ITM_OMEGA)) {
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
              break;

            case CM_NIM:
              keyActionProcessed = true;
              addItemToNimBuffer(item);
              break;

            case CM_REGISTER_BROWSER:
              if(item == ITM_PERIOD) {
                rbr1stDigit = true;
                if(rbrMode == RBR_GLOBAL) {
                  if(currentLocalRegisters != NULL) {
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
                    rbrRegister = (rbrRegister >= currentSubroutineLevelData[1].numberOfLocalRegisters ? 0 : rbrRegister);
                    currentRegisterBrowserScreen = FIRST_LOCAL_REGISTER + rbrRegister;
                  }
                }
              }

              keyActionProcessed = true;
              break;

            case CM_FLAG_BROWSER:
            case CM_FONT_BROWSER:
            case CM_ERROR_MESSAGE:
            case CM_BUG_ON_SCREEN:
              keyActionProcessed = true;
              break;

            case CM_CONFIRMATION:
              if(item == ITM_3 || item == ITM_XEQ) { // Yes or XEQ
                calcMode = previousCalcMode;
                temporaryInformation = TI_NO_INFO;
                confirmedFunction(CONFIRMED);
              }

              else if(item == ITM_7) { // No
                calcMode = previousCalcMode;
                temporaryInformation = TI_NO_INFO;
              }

              keyActionProcessed = true;
              break;

            case CM_PEM:
              if(item == ITM_PR) {
                leavePem();
                calcModeNormal();
                keyActionProcessed = true;
              }
              else if(item == ITM_OFF) {
                fnOff(NOPARAM);
                keyActionProcessed = true;
              }
              break;

            default:
              sprintf(errorMessage, "In function processKeyAction: %" PRIu8 " is an unexpected value while processing calcMode!", calcMode);
              displayBugScreen(errorMessage);
          }
        }
    }
  }



  static void menuUp(void) {
    int16_t menuId = softmenuStack[0].softmenuId;
    int16_t sm = softmenu[menuId].menuItem;
    if((sm == -MNU_alpha_omega || sm == -MNU_ALPHAintl) && alphaCase == AC_LOWER) {
      alphaCase = AC_UPPER;
      softmenuStack[0].softmenuId--; // Switch to the upper case menu
    }
    else if((sm == -MNU_ALPHADOT || sm == -MNU_ALPHAMATH) && alphaCase == AC_LOWER) {
      alphaCase = AC_UPPER;
    }
    else {
      int16_t itemShift = (catalog == CATALOG_NONE ? 18 : 6);

      if((softmenuStack[0].firstItem + itemShift) < (menuId < NUMBER_OF_DYNAMIC_SOFTMENUS ? dynamicSoftmenu[menuId].numItems : softmenu[menuId].numItems)) {
        softmenuStack[0].firstItem += itemShift;
      }
      else {
        softmenuStack[0].firstItem = 0;
      }

      setCatalogLastPos();
    }
  }



  static void menuDown(void) {
    int16_t menuId = softmenuStack[0].softmenuId;
    int16_t sm = softmenu[menuId].menuItem;
    if((sm == -MNU_ALPHA_OMEGA || sm == -MNU_ALPHAINTL) && alphaCase == AC_UPPER) {
      alphaCase = AC_LOWER;
      softmenuStack[0].softmenuId++; // Switch to the lower case menu
    }
    else if((sm == -MNU_ALPHADOT || sm == -MNU_ALPHAMATH) && alphaCase == AC_UPPER) {
      alphaCase = AC_LOWER;
    }
    else {
      int16_t itemShift = (catalog == CATALOG_NONE ? 18 : 6);

      if((softmenuStack[0].firstItem - itemShift) >= 0) {
        softmenuStack[0].firstItem -= itemShift;
      }
      else if((softmenuStack[0].firstItem - itemShift) >= -5) {
        softmenuStack[0].firstItem = 0;
      }
      else {
        if(menuId < NUMBER_OF_DYNAMIC_SOFTMENUS) {
          softmenuStack[0].firstItem = ((dynamicSoftmenu[menuId].numItems - 1)/6) / (itemShift/6) * itemShift;
        }
        else {
          softmenuStack[0].firstItem = ((       softmenu[menuId].numItems - 1)/6) / (itemShift/6) * itemShift;
        }
      }

      setCatalogLastPos();
    }
  }
#endif // !TESTSUITE_BUILD



/********************************************//**
 * \brief Processing ENTER key
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnKeyEnter(uint16_t unusedButMandatoryParameter) {
  #ifndef TESTSUITE_BUILD
    if(tamMode) {
      tamTransitionSystem(TT_ENTER);
      return;
    }

    switch(calcMode) {
      case CM_NORMAL:
        setSystemFlag(FLAG_ASLIFT);

        liftStack();
        copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);

        clearSystemFlag(FLAG_ASLIFT);
        break;

      case CM_AIM:
        calcModeNormal();
        popSoftmenu();

        if(aimBuffer[0] == 0) {
          undo();
        }
        else {
          int16_t len = stringByteLength(aimBuffer) + 1;

          reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(len), AM_NONE);
          xcopy(REGISTER_STRING_DATA(REGISTER_X), aimBuffer, len);

          setSystemFlag(FLAG_ASLIFT);
          liftStack();
          clearSystemFlag(FLAG_ASLIFT);

          copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
          aimBuffer[0] = 0;
        }
        break;

      case CM_NIM:
        closeNim();

        setSystemFlag(FLAG_ASLIFT);
        liftStack();
        clearSystemFlag(FLAG_ASLIFT);

        copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
        break;

      case CM_REGISTER_BROWSER:
      case CM_FLAG_BROWSER:
      case CM_FONT_BROWSER:
      case CM_ERROR_MESSAGE:
      case CM_BUG_ON_SCREEN:
        break;

      case CM_CONFIRMATION:
        calcMode = previousCalcMode;
        confirmedFunction(CONFIRMED);
        break;

      default:
        sprintf(errorMessage, "In function fnKeyEnter: unexpected calcMode value (%" PRIu8 ") while processing key ENTER!", calcMode);
        displayBugScreen(errorMessage);
    }
  #endif // !TESTSUITE_BUILD
}



/********************************************//**
 * \brief Processing EXIT key
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnKeyExit(uint16_t unusedButMandatoryParameter) {
  #ifndef TESTSUITE_BUILD
    if(catalog) {
      leaveAsmMode();
      return;
    }

    if(tamMode) {
      leaveTamMode();
      return;
    }

    switch(calcMode) {
      case CM_NORMAL:
        if(lastErrorCode != 0) {
          lastErrorCode = 0;
        }
        popSoftmenu();
        break;

      case CM_AIM:
        if(softmenuStack[0].softmenuId <= 1) { // MyMenu or MyAlpha is displayed
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
        else {
          popSoftmenu();
        }
        break;

      case CM_NIM:
        addItemToNimBuffer(ITM_EXIT);
        break;

      case CM_PEM:
        if(softmenuStack[0].softmenuId > 1) { // not MyMenu and not MyAlpha
          popSoftmenu();
          break;
        }

        leavePem();
        calcModeNormal();
        break;

      //case CM_ASM_OVER_TAM:
      //case CM_ASM_OVER_TAM_OVER_PEM:
      //  transitionSystemState = 0;
      //  calcModeTam();
      //  sprintf(tamBuffer, "%s __", indexOfItems[getOperation()].itemCatalogName);
      //  tamTransitionSystem(TT_NOTHING);
      //  break;

      //case CM_ASM_OVER_AIM:
      //  calcModeAim(NOPARAM);
      //  break;

      //case CM_ASM_OVER_PEM: // TODO: is that correct
      //  calcModeNormal();
      //  calcMode = CM_PEM;
       // break;

      case CM_REGISTER_BROWSER:
      case CM_FLAG_BROWSER:
      case CM_FONT_BROWSER:
        rbr1stDigit = true;
        calcMode = previousCalcMode;
        break;

      case CM_BUG_ON_SCREEN:
        calcMode = previousCalcMode;
        break;

      case CM_CONFIRMATION:
        calcMode = previousCalcMode;
        temporaryInformation = TI_NO_INFO;
        break;

      default:
        sprintf(errorMessage, "In function fnKeyExit: unexpected calcMode value (%" PRIu8 ") while processing key EXIT!", calcMode);
        displayBugScreen(errorMessage);
    }
  #endif // !TESTSUITE_BUILD
}



/********************************************//**
 * \brief Processing CC key
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnKeyCC(uint16_t unusedButMandatoryParameter) {
  #ifndef TESTSUITE_BUILD
    uint32_t dataTypeX;
    uint32_t dataTypeY;

    switch(calcMode) {
      case CM_NORMAL:
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
        break;

      case CM_NIM:
        addItemToNimBuffer(ITM_CC);
        break;

      case CM_REGISTER_BROWSER:
      case CM_FLAG_BROWSER:
      case CM_FONT_BROWSER:
        break;

      default:
        sprintf(errorMessage, "In function fnKeyCC: unexpected calcMode value (%" PRIu8 ") while processing key CC!", calcMode);
        displayBugScreen(errorMessage);
    }
  #endif // !TESTSUITE_BUILD
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

    if(tamMode) {
      tamTransitionSystem(TT_BACKSPACE);
      return;
    }

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
        if(catalog) {
          if(stringByteLength(aimBuffer) > 0) {
            lg = stringLastGlyph(aimBuffer);
            aimBuffer[lg] = 0;
            xCursor = showString(aimBuffer, &standardFont, 1, Y_POSITION_OF_AIM_LINE + 6, vmNormal, true, true);
          }
        }
        else if(stringByteLength(aimBuffer) > 0) {
          lg = stringLastGlyph(aimBuffer);
          aimBuffer[lg] = 0;
        }
        break;

      case CM_NIM:
        addItemToNimBuffer(ITM_BACKSPACE);
        break;

      //case CM_ASM_OVER_NORMAL:
      //  addItemToBuffer(ITM_BACKSPACE);
      //  break;

      case CM_REGISTER_BROWSER:
      case CM_FLAG_BROWSER:
      case CM_FONT_BROWSER:
        calcMode = previousCalcMode;
        break;

      case CM_BUG_ON_SCREEN:
        calcMode = previousCalcMode;
        break;

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
  #endif // !TESTSUITE_BUILD
}



/********************************************//**
 * \brief Processing UP key
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnKeyUp(uint16_t unusedButMandatoryParameter) {
  #ifndef TESTSUITE_BUILD
    int16_t menuId;

    if(tamMode && !catalog) {
      addItemToBuffer(ITM_Max);
      return;
    }

    switch(calcMode) {
      case CM_NORMAL:
      case CM_AIM:
      case CM_NIM:
        resetAlphaSelectionBuffer();
        menuId = softmenuStack[0].softmenuId;
        if(menuId > 1 && (   (menuId <  NUMBER_OF_DYNAMIC_SOFTMENUS && dynamicSoftmenu[menuId].numItems > 18)
                          || (menuId >= NUMBER_OF_DYNAMIC_SOFTMENUS &&        softmenu[menuId].numItems > 18))) {
          menuUp();
        }
        else {
          if(alphaCase != AC_UPPER) {
            alphaCase = AC_UPPER;
          }
        }
        break;

      case CM_REGISTER_BROWSER:
        rbr1stDigit = true;
        if(rbrMode == RBR_GLOBAL) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen + 1, FIRST_LOCAL_REGISTER);
        }
        else if(rbrMode == RBR_LOCAL) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - FIRST_LOCAL_REGISTER + 1, currentSubroutineLevelData[1].numberOfLocalRegisters) + FIRST_LOCAL_REGISTER;
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
        currentFlgScr = 3 - currentFlgScr;
       break;

      case CM_FONT_BROWSER:
        if(currentFntScr >= 2) {
          currentFntScr--;
        }
        break;

      case CM_PEM:
        resetAlphaSelectionBuffer();
        menuId = softmenuStack[0].softmenuId;
        if(menuId > 1 && (   (menuId <  NUMBER_OF_DYNAMIC_SOFTMENUS && dynamicSoftmenu[menuId].numItems > 18)
                          || (menuId >= NUMBER_OF_DYNAMIC_SOFTMENUS &&        softmenu[menuId].numItems > 18))) {
          menuUp();
        }
        else {
          fnBst(NOPARAM);
        }
        break;

      default:
        sprintf(errorMessage, "In function fnKeyUp: unexpected calcMode value (%" PRIu8 ") while processing key UP!", calcMode);
        displayBugScreen(errorMessage);
    }
  #endif // !TESTSUITE_BUILD
}



/********************************************//**
 * \brief Processing DOWN key
 *
 * \param[in] unusedButMandatoryParameter uint16_t
 * \return void
 ***********************************************/
void fnKeyDown(uint16_t unusedButMandatoryParameter) {
  #ifndef TESTSUITE_BUILD
    int16_t menuId;

    if(tamMode && !catalog) {
      addItemToBuffer(ITM_Min);
      return;
    }

    switch(calcMode) {
      case CM_NORMAL:
      case CM_AIM:
      case CM_NIM:
        resetAlphaSelectionBuffer();
        menuId = softmenuStack[0].softmenuId;
        if(menuId > 1 && (   (menuId <  NUMBER_OF_DYNAMIC_SOFTMENUS && dynamicSoftmenu[menuId].numItems > 18)
                          || (menuId >= NUMBER_OF_DYNAMIC_SOFTMENUS &&        softmenu[menuId].numItems > 18))) {
          menuDown();
        }
        else {
          if(alphaCase != AC_LOWER) {
            alphaCase = AC_LOWER;
          }
        }
        break;

      case CM_REGISTER_BROWSER:
        rbr1stDigit = true;
        if(rbrMode == RBR_GLOBAL) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - 1, FIRST_LOCAL_REGISTER);
        }
        else if(rbrMode == RBR_LOCAL) {
          currentRegisterBrowserScreen = modulo(currentRegisterBrowserScreen - FIRST_LOCAL_REGISTER - 1, currentSubroutineLevelData[1].numberOfLocalRegisters) + FIRST_LOCAL_REGISTER;
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
        currentFlgScr = 3 - currentFlgScr;
        break;

      case CM_FONT_BROWSER:
        if(currentFntScr < numScreensNumericFont + numScreensStandardFont) {
          currentFntScr++;
        }
        break;

      case CM_PEM:
        resetAlphaSelectionBuffer();
        menuId = softmenuStack[0].softmenuId;
        if(menuId > 1 && (   (menuId <  NUMBER_OF_DYNAMIC_SOFTMENUS && dynamicSoftmenu[menuId].numItems > 18)
                          || (menuId >= NUMBER_OF_DYNAMIC_SOFTMENUS &&        softmenu[menuId].numItems > 18))) {
          menuDown();
        }
        else {
          fnSst(NOPARAM);
        }
        break;

      default:
        sprintf(errorMessage, "In function fnKeyDown: unexpected calcMode value (%" PRIu8 ") while processing key DOWN!", calcMode);
        displayBugScreen(errorMessage);
    }
  #endif // !TESTSUITE_BUILD
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
      case CM_FONT_BROWSER:
        break;

      default:
        sprintf(errorMessage, "In function fnKeyDotD: unexpected calcMode value (%" PRIu8 ") while processing key .d!", calcMode);
        displayBugScreen(errorMessage);
    }
  #endif // !TESTSUITE_BUILD
}
