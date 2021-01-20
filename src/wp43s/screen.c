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
 * \file screen.c Screen related functions
 ***********************************************/

#include "wp43s.h"

#ifdef PC_BUILD
  /********************************************//**
   * \brief Draws the calc's screen on the PC window widget
   *
   * \param[in] widget GtkWidget* Not used
   * \param[in] cr cairo_t*
   * \param[in] data gpointer     Not used
   * \return gboolean
   ***********************************************/
  gboolean drawScreen(GtkWidget *widget, cairo_t *cr, gpointer data) {
    cairo_surface_t *imageSurface;

    imageSurface = cairo_image_surface_create_for_data((unsigned char *)screenData, CAIRO_FORMAT_RGB24, SCREEN_WIDTH, SCREEN_HEIGHT, screenStride * 4);
    #if defined(RASPBERRY) && (SCREEN_800X480 == 1)
      cairo_scale(cr, 2.0, 2.0);
    #endif // defined(RASPBERRY) && (SCREEN_800X480 == 1)
    cairo_set_source_surface(cr, imageSurface, 0, 0);
    cairo_surface_mark_dirty(imageSurface);
    #if defined(RASPBERRY) && (SCREEN_800X480 == 1)
      cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_FAST);
    #endif // defined(RASPBERRY) && (SCREEN_800X480 == 1)
    cairo_paint(cr);
    cairo_surface_destroy(imageSurface);

    screenChange = false;

    return FALSE;
  }



  void copyScreenToClipboard(void) {
    cairo_surface_t *imageSurface;
    GtkClipboard *clipboard;

    clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_clear(clipboard);
    gtk_clipboard_set_text(clipboard, "", 0); //JM FOUND TIP TO PROPERLY CLEAR CLIPBOARD: https://stackoverflow.com/questions/2418487/clear-the-system-clipboard-using-the-gtk-lib-in-c/2419673#2419673

    imageSurface = cairo_image_surface_create_for_data((unsigned char *)screenData, CAIRO_FORMAT_RGB24, SCREEN_WIDTH, SCREEN_HEIGHT, screenStride * 4);
    gtk_clipboard_set_image(clipboard, gdk_pixbuf_get_from_surface(imageSurface, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
  }


  #endif                                                //JMCSV
  #if defined (PC_BUILD) || defined (DMCP_BUILD)        //JMCSV

  static void angularUnitToString(uint32_t angularMode, char *string) {
    switch(angularMode) {
      case AM_DEGREE: strcpy(string, STD_DEGREE); break;
      case AM_DMS:    strcpy(string, "d.ms");     break;
      case AM_RADIAN: strcpy(string, "r");        break;
      case AM_MULTPI: strcpy(string, STD_pi);     break;
      case AM_GRAD:   strcpy(string, "g");        break;
      case AM_NONE:   break;
      default:        strcpy(string, "?");
    }
  }



  void copyRegisterToClipboardString(calcRegister_t regist, char *clipboardString) {
    longInteger_t lgInt;
    int16_t base, sign, n;
    uint64_t shortInt;
    char tmp2[TMP_STR_LENGTH];                           //JMCSV

    switch(getRegisterDataType(regist)) {
      case dtLongInteger:
        convertLongIntegerRegisterToLongInteger(regist, lgInt);
        longIntegerToAllocatedString(lgInt, tmpString, TMP_STR_LENGTH);
        longIntegerFree(lgInt);
        tmp2[0]=0;                                         //JMCSV add apostrophies
        strcat(tmp2,"\"");                                 //JMCSV
        strcat(tmp2,tmpString);                           //JMCSV
        //strcpy(tmpString,tmp2);                         //JMCSV
        tmp2[TMP_STR_LENGTH-1]=0;                          //JMCSV trying a better way, in case the terminating 0 is not copied
        xcopy(tmpString,tmp2,min(TMP_STR_LENGTH-3,stringByteLength(tmp2)+1 ));  //JMCSV trying a better way
        strcat(tmpString,"\"");                           //JMCSV
        break;

      case dtTime:
        strcpy(tmpString, "time to clipboard to be coded");
        break;

      case dtDate:
        strcpy(tmpString, "date to clipboard to be coded");
        break;

      case dtString:
        xcopy(tmpString + TMP_STR_LENGTH/2, REGISTER_STRING_DATA(regist), stringByteLength(REGISTER_STRING_DATA(regist)) + 1);
        stringToUtf8(tmpString + TMP_STR_LENGTH/2, (uint8_t *)tmpString);
        tmp2[0]=0;                                         //JMCSV add apostrophies
        strcat(tmp2,"\"");                                 //JMCSV
        strcat(tmp2,tmpString);                           //JMCSV
        strcpy(tmpString,tmp2);                           //JMCSV
        strcat(tmpString,"\"");                           //JMCSV
        break;

      case dtReal34Matrix:
        strcpy(tmpString, "real matrix to clipboard is to be coded");
        break;

      case dtComplex34Matrix:
        strcpy(tmpString, "complex matrix to clipboard is to be coded");
        break;

      case dtShortInteger:
        convertShortIntegerRegisterToUInt64(regist, &sign, &shortInt);
        base = getRegisterShortIntegerBase(regist);

        n = ERROR_MESSAGE_LENGTH - 100;
        sprintf(errorMessage + n--, "#%d%s (word size = %u)", base, CSV_TAB, shortIntegerWordSize);  //JMCSV added comma

        if(shortInt == 0) {
          errorMessage[n--] = '0';
        }
        else {
          while(shortInt != 0) {
            errorMessage[n--] = digits[shortInt % base];
            shortInt /= base;
          }
          if(sign) {
            errorMessage[n--] = '-';
          }
        }
        n++;

        strcpy(tmpString, errorMessage + n);
        break;

      case dtReal34:
        real34ToString(REGISTER_REAL34_DATA(regist), tmpString + TMP_STR_LENGTH/2);
        if(strchr(tmpString + TMP_STR_LENGTH/2, '.') == NULL && strchr(tmpString + TMP_STR_LENGTH/2, 'E') == NULL) {
          strcat(tmpString + TMP_STR_LENGTH/2, ".");
        }
        angularUnitToString(getRegisterAngularMode(regist), tmpString + TMP_STR_LENGTH/2 + strlen(tmpString + TMP_STR_LENGTH/2));
        stringToUtf8(tmpString + TMP_STR_LENGTH/2, (uint8_t *)tmpString);
        break;

      case dtComplex34:
        real34ToString(REGISTER_REAL34_DATA(regist), tmpString);
        if(real34IsNegative(REGISTER_IMAG34_DATA(regist))) {
          strcat(tmpString, " - ix");
          real34SetPositiveSign(REGISTER_IMAG34_DATA(regist));
          real34ToString(REGISTER_IMAG34_DATA(regist), tmpString + strlen(tmpString));
          real34SetNegativeSign(REGISTER_IMAG34_DATA(regist));
        }
        else {
          strcat(tmpString, " + ix");
          real34ToString(REGISTER_IMAG34_DATA(regist), tmpString + strlen(tmpString));
        }
        break;

      case dtConfig:
        xcopy(tmpString, "Config data", 19);
        break;

      default:
        sprintf(tmpString, "In function copyRegisterXToClipboard, the data type %" PRIu32 " is unknown.", getRegisterDataType(regist));
    }

    strcpy(clipboardString, tmpString);
  }

  #endif                                                //JMCSV
  #ifdef PC_BUILD                                       //JMCSV


  void copyRegisterXToClipboard(void) {
    GtkClipboard *clipboard;
    char clipboardString[3000];

    clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_clear(clipboard);
    gtk_clipboard_set_text(clipboard, "", 0); //JM FOUND TIP TO PROPERLY CLEAR CLIPBOARD: https://stackoverflow.com/questions/2418487/clear-the-system-clipboard-using-the-gtk-lib-in-c/2419673#2419673

    copyRegisterToClipboardString(REGISTER_X, clipboardString);

    gtk_clipboard_set_text(clipboard, tmpString, -1);
  }



  void copyStackRegistersToClipboardString(char *clipboardString) {
    char *ptr = clipboardString;

    strcpy(ptr, "K = ");
    ptr += 4;
    copyRegisterToClipboardString(REGISTER_K, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "J = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_J, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "I = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_I, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "L = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_L, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "D = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_D, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "C = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_C, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "B = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_B, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "A = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_A, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "T = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_T, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "Z = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_Z, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "Y = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_Y, ptr);

    ptr += strlen(ptr);
    strcpy(ptr, LINEBREAK "X = ");
    ptr += strlen(ptr);
    copyRegisterToClipboardString(REGISTER_X, ptr);
  }



  void copyStackRegistersToClipboard(void) {
    GtkClipboard *clipboard;
    char clipboardString[10000];

    clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_clear(clipboard);
    gtk_clipboard_set_text(clipboard, "", 0); //JM FOUND TIP TO PROPERLY CLEAR CLIPBOARD: https://stackoverflow.com/questions/2418487/clear-the-system-clipboard-using-the-gtk-lib-in-c/2419673#2419673

    copyStackRegistersToClipboardString(clipboardString);

    gtk_clipboard_set_text(clipboard, clipboardString, -1);
  }



  void copyAllRegistersToClipboard(void) {
    GtkClipboard *clipboard;
    char clipboardString[15000], *ptr = clipboardString;

    clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_clear(clipboard);
    gtk_clipboard_set_text(clipboard, "", 0); //JM FOUND TIP TO PROPERLY CLEAR CLIPBOARD: https://stackoverflow.com/questions/2418487/clear-the-system-clipboard-using-the-gtk-lib-in-c/2419673#2419673

    copyStackRegistersToClipboardString(ptr);

    for(int32_t regist=99; regist>=0; --regist) {
      ptr += strlen(ptr);
      sprintf(ptr, LINEBREAK "R%02d = ", regist);
      ptr += strlen(ptr);
      copyRegisterToClipboardString(regist, ptr);
    }

    for(int32_t regist=currentNumberOfLocalRegisters-1; regist>=0; --regist) {
      ptr += strlen(ptr);
      sprintf(ptr, LINEBREAK "R.%02d = ", regist);
      ptr += strlen(ptr);
      copyRegisterToClipboardString(FIRST_LOCAL_REGISTER + regist, ptr);
    }

    if(statisticalSumsPointer != NULL) {
      char sumName[40];
      for(int32_t sum=0; sum<NUMBER_OF_STATISTICAL_SUMS; sum++) {
        ptr += strlen(ptr);

        switch(sum) {
          case  0: strcpy(sumName,           "n             "           ); break;
          case  1: strcpy(sumName, STD_SIGMA "(x)          "            ); break;
          case  2: strcpy(sumName, STD_SIGMA "(y)          "            ); break;
          case  3: strcpy(sumName, STD_SIGMA "(x" STD_SUP_2 ")         "); break;
          case  4: strcpy(sumName, STD_SIGMA "(x" STD_SUP_2 "y)        "); break;
          case  5: strcpy(sumName, STD_SIGMA "(y" STD_SUP_2 ")         "); break;
          case  6: strcpy(sumName, STD_SIGMA "(xy)         "            ); break;
          case  7: strcpy(sumName, STD_SIGMA "(ln(x)" STD_CROSS "ln(y))"); break;
          case  8: strcpy(sumName, STD_SIGMA "(ln(x))      "            ); break;
          case  9: strcpy(sumName, STD_SIGMA "(ln" STD_SUP_2 "(x))     "); break;
          case 10: strcpy(sumName, STD_SIGMA "(y ln(x))    "            ); break;
          case 11: strcpy(sumName, STD_SIGMA "(ln(y))      "            ); break;
          case 12: strcpy(sumName, STD_SIGMA "(ln" STD_SUP_2 "(y))     "); break;
          case 13: strcpy(sumName, STD_SIGMA "(x ln(y))    "            ); break;
          case 14: strcpy(sumName, STD_SIGMA "(ln(y)/x)    "            ); break;
          case 15: strcpy(sumName, STD_SIGMA "(x" STD_SUP_2 "/y)       "); break;
          case 16: strcpy(sumName, STD_SIGMA "(1/x)        "            ); break;
          case 17: strcpy(sumName, STD_SIGMA "(1/x" STD_SUP_2 ")       "); break;
          case 18: strcpy(sumName, STD_SIGMA "(x/y)        "            ); break;
          case 19: strcpy(sumName, STD_SIGMA "(1/y)        "            ); break;
          case 20: strcpy(sumName, STD_SIGMA "(1/y" STD_SUP_2 ")       "); break;
          case 21: strcpy(sumName, STD_SIGMA "(x" STD_SUP_3 ")         "); break;
          case 22: strcpy(sumName, STD_SIGMA "(x" STD_SUP_4 ")         "); break;
          case 23: strcpy(sumName,           "x min         "           ); break;
          case 24: strcpy(sumName,           "x max         "           ); break;
          case 25: strcpy(sumName,           "y min         "           ); break;
          case 26: strcpy(sumName,           "y max         "           ); break;
          default: strcpy(sumName,           "?             "           );
        }

        sprintf(ptr, LINEBREAK "SR%02d = ", sum);
        ptr += strlen(ptr);
        stringToUtf8(sumName, (uint8_t *)ptr);
        ptr += strlen(ptr);
        strcpy(ptr, " = ");
        ptr += strlen(ptr);
        realToString(statisticalSumsPointer + REAL_SIZE * sum, tmpString);
        if(strchr(tmpString, '.') == NULL && strchr(tmpString, 'E') == NULL) {
          strcat(tmpString, ".");
        }
        strcpy(ptr, tmpString);
      }
    }

    gtk_clipboard_set_text(clipboard, clipboardString, -1);
  }



  /********************************************//**
   * \brief Refreshes calc's screen. This function is
   * called every SCREEN_REFRESH_PERIOD ms by a GTK timer.
   * * make the cursor blink if needed
   * * refresh date and time in the status bar if needed
   * * refresh the whole screen if needed
   *
   * \param[in] data gpointer Not used
   * \return gboolean         What will happen next?
   *                          * true  = timer will call this function again
   *                          * false = timer stops calling this function
   ***********************************************/
  #define cursorCycle 3                      //JM cursor vv
  int8_t cursorBlinkCounter;                 //JM cursor ^^
  gboolean refreshLcd(gpointer unusedData) { // This function is called every SCREEN_REFRESH_PERIOD ms by a GTK timer
    // Cursor blinking
    static bool_t cursorBlink=true;

    if(cursorEnabled) {
      if(++cursorBlinkCounter > cursorCycle) {         //JM cursor vv
        cursorBlinkCounter = 0;
        if(cursorBlink) {
          showGlyph(STD_CURSOR, cursorFont, xCursor, yCursor, vmNormal, true, false);
          }                                              //JM cursor ^^
        else {
          hideCursor();
        }
      cursorBlink = !cursorBlink;
      }
    }

    // Function name display
    if(showFunctionNameCounter > 0) {
      showFunctionNameCounter -= SCREEN_REFRESH_PERIOD;
      if(showFunctionNameCounter <= 0) {
        hideFunctionName();
        showFunctionName(ITM_NOP, 0);
      }
    }

    // Update date and time
    getTimeString(dateTimeString);
    if(strcmp(dateTimeString, oldTime)) {
      strcpy(oldTime, dateTimeString);
      #if (DEBUG_INSTEAD_STATUS_BAR != 1)
        showDateTime();
      #endif // (DEBUG_INSTEAD_STATUS_BAR != 1)
    }

    // If LCD has changed: update the GTK screen
    if(screenChange) {
      #if (__linux__ == 1) && (DEBUG_PANEL == 1)
        refreshDebugPanel();
      #endif // (__linux__ == 1) && (DEBUG_PANEL == 1)

      gtk_widget_queue_draw(screen);
      while(gtk_events_pending()) {
        gtk_main_iteration();
      }
    }

    // Alpha selection timer
    if(AlphaSelectionBufferTimerRunning) {         //JMvv
      if(catalog/* && alphaSelectionTimer != 0 && (getUptimeMs() - alphaSelectionTimer) > 3000*/) { // More than 3 seconds elapsed since last keypress
        timeoutAlphaSelectionBuffer();             //JM^^
      }
    }

    return TRUE;
  }
#elif defined DMCP_BUILD
  #define cursorCycle 3                      //JM cursor vv
  int8_t cursorBlinkCounter;                 //JM cursor ^^
  void refreshLcd(void) {// This function is called roughly every SCREEN_REFRESH_PERIOD ms from the main loop
    // Cursor blinking
    static bool_t cursorBlink=true;

    if(cursorEnabled) {
      if(++cursorBlinkCounter > cursorCycle) {         //JM cursor vv
      cursorBlinkCounter = 0;
      if(cursorBlink) {
        showGlyph(STD_CURSOR, cursorFont, xCursor, yCursor, vmNormal, true, false);
      }                                              //JM cursor ^^
      else {
        hideCursor();
      }
      cursorBlink = !cursorBlink;
    }
  }

    // Function name display
    if(showFunctionNameCounter>0) {
      showFunctionNameCounter -= SCREEN_REFRESH_PERIOD;
      if(showFunctionNameCounter <= 0) {
        hideFunctionName();
        showFunctionName(ITM_NOP, 0);
      }
    }

    // Update date and time
    getTimeString(dateTimeString);
    if(strcmp(dateTimeString, oldTime)) {
      strcpy(oldTime, dateTimeString);
      #if (DEBUG_INSTEAD_STATUS_BAR != 1)
        showDateTime();
      #endif // (DEBUG_INSTEAD_STATUS_BAR != 1)

      if(!getSystemFlag(FLAG_AUTOFF)) {
        reset_auto_off();
      }


    }

    if(usb_powered() == 1) {
      if(!getSystemFlag(FLAG_USB)) {
        setSystemFlag(FLAG_USB);
        clearSystemFlag(FLAG_LOWBAT);
        showHideUsbLowBattery();
      }
    }
    else {
      if(getSystemFlag(FLAG_USB)) {
        clearSystemFlag(FLAG_USB);
      }

      if(get_vbat() < 2000) {
        if(!getSystemFlag(FLAG_LOWBAT)) {
          setSystemFlag(FLAG_LOWBAT);
          showHideUsbLowBattery();
        }
        SET_ST(STAT_PGM_END);
      }
      else if(get_vbat() < 2500) {
        if(!getSystemFlag(FLAG_LOWBAT)) {
          setSystemFlag(FLAG_LOWBAT);
          showHideUsbLowBattery();
        }
      }
      else {
        if(getSystemFlag(FLAG_LOWBAT)) {
          clearSystemFlag(FLAG_LOWBAT);
          showHideUsbLowBattery();
        }
      }
    }

    // Alpha selection timer
  if(AlphaSelectionBufferTimerRunning) {         //JMvv
    if(catalog)/* && alphaSelectionTimer != 0 && (getUptimeMs() - alphaSelectionTimer) > 3000)*/ { // More than 3 seconds elapsed since last keypress
      timeoutAlphaSelectionBuffer();             //JM^^
    }
  }
}
#endif // PC_BUILD DMCP_BUILD



#ifndef TESTSUITE_BUILD
void refreshFn(uint16_t timerType) {                        //vv dr - general timeout handler
  if(timerType == TO_FG_LONG) { Shft_handler(); }
  if(timerType == TO_CL_LONG) { LongpressKey_handler(); }
  if(timerType == TO_FG_TIMR) { Shft_stop(); }
  if(timerType == TO_FN_LONG) { FN_handler(); }
}                                                           //^^



void underline(int16_t y) {                     //JM
  int16_t i;
   for( i = 0; i < 6; i++ ){
     if(calcMode != CM_GRAPH || ((calcMode == CM_GRAPH) && (i <= 1)))
       underline_softkey(i, y, true);
   }
}                                               //JM


uint32_t ul;
void clear_ul(void) {
  ULGL = false;
  ULFL = false;
  ul = 0;                                       //JM Set all bits 00-23 to zero
}
                                                //JM vv LONGPRESS.   false auto clears
void underline_softkey(int16_t xSoftkey, int16_t ySoftKey, bool_t dontclear) {
  int16_t x, y, x1, y1, x2, y2;
  uint32_t tmp;


  if(calcMode == CM_GRAPH && xSoftkey >= 2) {
      return;
  }

  if(jm_FG_LINE) {

//JMUL all changed  vv  
    if(!dontclear) {                            //JM Recursively call the same routine to clear the previous line
      for(y=0; y<ySoftKey; y++) {
        tmp = ul;
        if( ((tmp >> (y*6+xSoftkey)) & 1U)) {   //JM To check a bit, shift the number n to the right, then bitwise AND it:
          underline_softkey(xSoftkey,y,true);
        }
      }
    }
    ul ^= 1UL << (ySoftKey*6+xSoftkey);         //JM The XOR operator (^) can be used to toggle a bit.

//JMUL all changed  ^^

    if(0 <= xSoftkey && xSoftkey <= 5) {
      x1 = 67 * xSoftkey - 1;
      x2 = x1 + 67;
    } else {
      x1 = 0;
      x2 = 0;
    }

    if(0 <= ySoftKey && ySoftKey <= 2) {
      y1 = 217 - SOFTMENU_HEIGHT * ySoftKey;
      y2 = y1 + SOFTMENU_HEIGHT;
    } else {
      y1 = 0;
      y2 = 0;
    }

    y = y2-3-1;
    if(y>=0) {                                  //JM Make provision for out of range parameter, used to not plot the line and only for the recursive line removal
      for(x=x2-66+1; x<min(x2-1,SCREEN_WIDTH); x++) {
        if(mod(x, 2) == 0) {
            invert_Pixel  ((uint32_t) x, (uint32_t) y);
            invert_Pixel  ((uint32_t) x, (uint32_t) (y+2));
        }
        else {
            invert_Pixel  (x, y+1);
        }
      }
    }
  }
}                                            //JM ^^



void FN_handler() {                          //JM FN LONGPRESS vv Handler FN Key shift longpress handler
                                             //   Processing cycles here while the key is pressed, that is, after PRESS #1, waiting for RELEASE #2
  if((FN_state = ST_1_PRESS1) && FN_timeouts_in_progress && (FN_key_pressed != 0)) {

    if(fnTimerGetStatus(TO_FN_LONG) == TMR_COMPLETED) {
      FN_handle_timed_out_to_EXEC = false;
      if(!shiftF && !shiftG) {                            //   Current shift state
        shiftF = true;        //S_shF();                  //   New shift state
        showShiftState();
        refreshRegisterLine(REGISTER_T); //clearRegisterLine(Y_POSITION_OF_REGISTER_T_LINE - 4, REGISTER_LINE_HEIGHT); //JM FN clear the previous shift function name
        showFunctionName(nameFunction(FN_key_pressed-37,6),0);
        FN_timed_out_to_RELEASE_EXEC = true;
        underline_softkey(FN_key_pressed-38,1, false);
        fnTimerStart(TO_FN_LONG, TO_FN_LONG, JM_TO_FN_LONG);          //dr
        #ifdef FN_TIME_DEBUG1
          printf("Handler 1, KEY=%d \n",FN_key_pressed);
        #endif
      }
      else if(shiftF && !shiftG) {
        shiftG = true;                  
        shiftF = false;                 
        showShiftState();
        refreshRegisterLine(REGISTER_T); //clearRegisterLine(Y_POSITION_OF_REGISTER_T_LINE - 4, REGISTER_LINE_HEIGHT); //JM FN clear the previous shift function name
        showFunctionName(nameFunction(FN_key_pressed-37,12),0);
        FN_timed_out_to_RELEASE_EXEC = true;
        underline_softkey(FN_key_pressed-38,2, false);
        fnTimerStart(TO_FN_LONG, TO_FN_LONG, JM_TO_FN_LONG);          //dr
        #ifdef FN_TIME_DEBUG1
          printf("Handler 2, KEY=%d \n",FN_key_pressed);
        #endif
      }
      else if((!shiftF && shiftG) || (shiftF && shiftG)) {
        refreshRegisterLine(REGISTER_T); //clearRegisterLine(Y_POSITION_OF_REGISTER_T_LINE - 4, REGISTER_LINE_HEIGHT); //JM FN clear the previous shift function name
        showFunctionName(ITM_NOP, 0);
        FN_timed_out_to_NOP = true;
        underline_softkey(FN_key_pressed-38,3, false);   //  Purposely select row 3 which does not exist, just to activate the 'clear previous line'
        FN_timeouts_in_progress = false;
        fnTimerStop(TO_FN_LONG);                                      //dr
        #ifdef FN_TIME_DEBUG1
          printf("Handler 3, KEY=%d \n",FN_key_pressed);
        #endif
      }
    }
  }
}                                        //JM ^^



void Shft_handler() {                        //JM SHIFT NEW vv
  if(Shft_timeouts) {

    if(fnTimerGetStatus(TO_FG_LONG) == TMR_COMPLETED) {
      fnTimerStop(TO_3S_CTFF);
      if(!shiftF && !shiftG) {
        shiftF = true;
        fnTimerStart(TO_FG_LONG, TO_FG_LONG, JM_TO_FG_LONG);
        showShiftState();
      }
      else if(shiftF && !shiftG) {
        shiftG = true;
        shiftF = false;
        fnTimerStart(TO_FG_LONG, TO_FG_LONG, JM_TO_FG_LONG);
        showShiftState();
      }
      else if((!shiftF && shiftG) || (shiftF && shiftG)) {
        Shft_timeouts = false;
        fnTimerStop(TO_FG_LONG);
        fnTimerStop(TO_FG_TIMR);
        resetShiftState();                        //force into no shift state, i.e. to wait
        if(HOME3) {
          #ifdef PC_BUILD          
            jm_show_calc_state("screen.c: Shft_handler: HOME3");
          #endif //PC_BUILD
          if((softmenuStack[0].softmenuId == mm_MNU_HOME)) {              //JM shifts    //softmenuStackPointerJM
            popSoftmenu();                                                                                                  //JM shifts
          }
          else {
            if(calcMode == CM_AIM) {                                                                                        //JM shifts
            }
            else {                                                                                                          //JM SHIFTS
              showSoftmenu(-MNU_HOME);                                                                          //JM shifts  //JM ALPHA-HOME
            }                                                                                                               //JM shifts
          }
        showSoftmenuCurrentPart();
        }
      }
    }
  }
}                                        //JM ^^



void LongpressKey_handler() {
  if(fnTimerGetStatus(TO_CL_LONG) == TMR_COMPLETED) {
    if(JM_auto_longpress_enabled != 0) {
      showFunctionName(JM_auto_longpress_enabled, 1000);            //fnClearStack(0);
      JM_auto_longpress_enabled = 0;
    } 
  }
}



void Shft_stop() {
  Shft_timeouts = false;
  fnTimerStop(TO_FG_LONG);
  fnTimerStop(TO_FG_TIMR);
  resetShiftState();                        //force into no shift state, i.e. to wait
}
  #ifndef DMCP_BUILD
    /********************************************//**
     * \brief Sets a black pixel on the screen.
     *
     * \param[in] x uint32_t x coordinate from 0 (left) to 399 (right)
     * \param[in] y uint32_t y coordinate from 0 (top) to 239 (bottom)
     * \return void
     ***********************************************/
    void setBlackPixel(uint32_t x, uint32_t y) {
      //if(y >= (uint32_t)(-6)) return;  //JM allowing allowing -1..-5 for top row text
    
      if(x>=SCREEN_WIDTH || y>=SCREEN_HEIGHT) {
        printf("In function setBlackPixel: x=%u or %d, y=%u or %d outside the screen!\n", x, (int32_t)(x), y, (int32_t)(y) );
        return;
      }

      *(screenData + y*screenStride + x) = ON_PIXEL;
      screenChange = true;
    }



    /********************************************//**
     * \brief Sets a white pixel on the screen.
     *
     * \param[in] x uint32_t x coordinate from 0 (left) to 399 (right)
     * \param[in] y uint32_t y coordinate from 0 (top) to 239 (bottom)
     * \return void
     ***********************************************/
    void setWhitePixel(uint32_t x, uint32_t y) {
      //if(y >= (uint32_t)(-6)) return;  //JM allowing allowing -1..-5 for top row text
      
      if(x>=SCREEN_WIDTH || y>=SCREEN_HEIGHT) {
        printf("In function setWhitePixel: x=%u or %d, y=%u or %d outside the screen!\n", x, (int32_t)(x), y, (int32_t)(y) );
        return;
      }

      *(screenData + y*screenStride + x) = OFF_PIXEL;
      screenChange = true;
    }


  /********************************************//**                       //JM
   * \brief Inverts a pixel on the screen (white/black).
   *
   * \param[in] x uint32_t x coordinate from 0 (left) to 399 (right)
   * \param[in] y uint32_t y coordinate from 0 (top) to 239 (bottom)
   * \return void
   ***********************************************/
void invert_Pixel(uint32_t x, uint32_t y) {           //JM
    if(x>=SCREEN_WIDTH || y>=SCREEN_HEIGHT) {
      printf("In function invert_Pixel: x=%d, y=%d outside the screen!\n", x, y);
      return;
    }

    *(screenData + y*screenStride + x) = *(screenData + y*screenStride + x) ^ 0xffffff;
    screenChange = true;
}



int16_t clearScreenCounter = 0;                       //JM ClearScreen Test
void lcd_fill_rect(uint32_t x, uint32_t y, uint32_t dx, uint32_t 	dy, int val) {
    uint32_t line, col, pixelColor, *pixel, endX = x + dx, endY = y + dy;

    //if(y >= (uint32_t)(-100)) return;  //JM allowing -100 to measure the size in pixels; allowing -1..-5 for top row text

    if(x==0 && y==0 && dx==SCREEN_WIDTH && dy==SCREEN_HEIGHT) {  //JMTOCHECK is this needed?
      printf(">>> screen.c: clearScreen: clearScreenCounter=%d\n",clearScreenCounter++);    //JMYY ClearScreen Test  #endif
      clear_ul(); //JMUL
    }
      if(endX > SCREEN_WIDTH || endY > SCREEN_HEIGHT) {
        printf("In function lcd_fill_rect: x=%u, y=%u, dx=%u, dy=%u, val=%d outside the screen!\n", x, y, dx, dy, val);
        return;
      }

      pixelColor = (val == LCD_SET_VALUE ? OFF_PIXEL : ON_PIXEL);
      for(line=y; line<endY; line++) {
        for(col=x, pixel=screenData + line*screenStride + x; col<endX; col++, pixel++) {
          *pixel = pixelColor;
        }
      }

    screenChange = true;
  }
#endif //DMCP_BUILD




//JM: If maxiC is set, then, if a glyph is not found in numericfont, it will be fetched and enlarged from standardfont

uint8_t  combinationFonts = combinationFontsDefault;
uint8_t  miniC = 0;                                                              //JM miniature letters
uint8_t  maxiC = 0;                                                              //JM ENLARGE letters. Use Numericfont & combinationFontsDefault=2;
bool_t   noShow = false;                                                         //JM
uint8_t  displaymode = stdNoEnlarge;

/********************************************//**
 * \brief Displays a glyph using it's Unicode code point
 *
 * \param[in] charCode uint16_t      Unicode code point of the glyph to display
 * \param[in] font font_t*           Font to use
 * \param[in] x uint32_t             x coordinate where to display the glyph
 * \param[in] y uint32_t             y coordinate where to display the glyph
 * \param[in] videoMode videoMode_t  Video mode normal or reverse
 * \param[in] showLeadingCols bool_t Display the leading empty columns
 * \param[in] showEndingCols bool_t  Display the ending empty columns
 * \return uint32_t                  x coordinate for the next glyph
 ***********************************************/
  uint32_t showGlyphCode(uint16_t charCode, const font_t *font, uint32_t x, uint32_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols) {
  uint32_t col, row, xGlyph, endingCols;
  int32_t  glyphId;
  int8_t   byte, *data;
  const glyph_t  *glyph;
  int8_t rep_enlarge;

  bool_t enlarge = false;                                   //JM ENLARGE vv
  if(combinationFonts == stdnumEnlarge || combinationFonts == numHalf) {
    if(maxiC == 1 && font == &numericFont) {                //JM allow enlargements
      glyphId = findGlyph(font, charCode);
      if(glyphId < 0) {           //JM if there is not a large glyph, enlarge the small letter
        enlarge = true;
        font = &standardFont;
      }
    }
  } else
  if(combinationFonts == 1) {
    if(maxiC == 1 && font == &standardFont) {                //JM allow enlargements
      enlarge = true;
    }                                                       //JM ENLARGE ^^
  }

    glyphId = findGlyph(font, charCode);
    if(glyphId >= 0) {
      glyph = (font->glyphs) + glyphId;
    }
    else if(glyphId == -1) {
      generateNotFoundGlyph(-1, charCode);
      glyph = &glyphNotFound;
    }
    else if(glyphId == -2) {
      generateNotFoundGlyph(-2, charCode);
      glyph = &glyphNotFound;
    }
    else {
      glyph = NULL;
    }

    if(glyph == NULL) {
      sprintf(errorMessage, "In function showGlyphCode: %" PRIi32 " is an unexpected value returned by fingGlyph!", glyphId);
      displayBugScreen(errorMessage);
      return 0;
    }

  data = (int8_t *)glyph->data;
  uint32_t y0 = y;                                                   //JMmini 0-reference
  xGlyph      = showLeadingCols ? glyph->colsBeforeGlyph : 0;
  endingCols  = showEndingCols ? glyph->colsAfterGlyph : 0;

#define REDUCT_A 3
#define REDUCT_B 4
#define REDUCT_OFF 3

  // Clearing the space needed by the glyph
  if(enlarge && combinationFonts !=0) rep_enlarge = 2; else rep_enlarge = 1;                //JM ENLARGE
  if(!noShow) lcd_fill_rect(x, y, ((xGlyph + glyph->colsGlyph + endingCols) >> miniC), rep_enlarge*((glyph->rowsAboveGlyph + glyph->rowsGlyph + glyph->rowsBelowGlyph) >> miniC)-(rep_enlarge-1)*4, (videoMode == vmNormal ? LCD_SET_VALUE : LCD_EMPTY_VALUE));  //JMmini
  if(displaymode == numHalf) {y += (glyph->rowsAboveGlyph*REDUCT_A/REDUCT_B);} else {y += glyph->rowsAboveGlyph;}        //JM REDUCE
  //x += xGlyph; //JM

  // Drawing the glyph
  for(row=0; row<glyph->rowsGlyph; row++, y++) {
    if(displaymode == numHalf) {if((int)((REDUCT_A*row+REDUCT_OFF)) % REDUCT_B == 0) y--;}                           //JM REDUCE
    if(enlarge && combinationFonts !=0) rep_enlarge = 1; else rep_enlarge = 0;                //JM ENLARGE
    while (rep_enlarge >= 0) {                                             //JM ENLARGE ^^

        // Drawing the columns of the glyph
      int32_t bit = 7;
      for(col=0; col<glyph->colsGlyph; col++) {
        if(bit == 7) {
            //        byte = *(data++);
            byte = *(data);                                                  //JM ENLARGE
            if(rep_enlarge == 0) data++;                                     //JM ENLARGE
            if(miniC!=0) byte = (uint8_t)byte | (((uint8_t)byte) << 1);           //JMmini
        }

        if(byte & 0x80) {// MSB set
          if(videoMode == vmNormal) { // Black pixel for white background
            if(!noShow) setBlackPixel(x+((xGlyph+col) >> miniC), y0+((y-y0) >> miniC));       //JMmini
          }
          else { // White pixel for black background
            if(!noShow) setWhitePixel(x+((xGlyph+col) >> miniC), y0+((y-y0) >> miniC));       //JMmini
          }
        }

        byte <<= 1;

        if(--bit == -1) {
          bit = 7;
        }
      }
      if(rep_enlarge == 1 && row!=3 && row!=6 && row!=9 && row!=12) y++;     //JM ENLARGE vv do not advance the row counter for four rows, to match the row height of the enlarge font
      rep_enlarge--;
    }
  }
  return x + ((xGlyph + glyph->colsGlyph + endingCols) >> miniC);        //JMmini
}



  /********************************************//**
   * \brief Displays the first glyph of a string
   *
   * \param[in] ch const char*         String whose first glyph is to display
   * \param[in] font font_t*           Font to use
   * \param[in] x int16_t              x coordinate where to display the glyph
   * \param[in] y int16_t              y coordinate where to display the glyph
   * \param[in] videoMode videoMode_t  Video mode normal or reverse
   * \param[in] showLeadingCols bool_t Display the leading empty columns
   * \param[in] showEndingCols bool_t  Display the ending empty columns
   * \return int16_t                   x coordinate for the next glyph
   ***********************************************/
  uint32_t showGlyph(const char *ch, const font_t *font, uint32_t x, uint32_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols) {
    uint16_t charCode;

    charCode = (uint8_t)*ch;
    if(charCode &0x0080) {
      charCode = (charCode << 8) | (uint8_t)*(ch+1);
    }
    return showGlyphCode(charCode, font, x, y, videoMode, showLeadingCols, showEndingCols);
  }


  uint8_t  compressString = 0;                                                              //JM compressString

  /********************************************//**
   * \brief Displays a 0 terminated string
   *
   * \param[in] string const char*     String whose first glyph is to display
   * \param[in] font font_t*           Font to use
   * \param[in] x uint32_t             x coordinate where to display the glyph
   * \param[in] y uint32_t             y coordinate where to display the glyph
   * \param[in] videoMode videoMode_t  Video mode normal or reverse
   * \param[in] showLeadingCols bool_t Display the leading empty columns
   * \param[in] showEndingCols bool_t  Display the ending empty columns
   * \return uint32_t                  x coordinate for the next glyph
   ***********************************************/
  uint32_t showString(const char *string, const font_t *font, uint32_t x, uint32_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols) {
    uint16_t ch, lg;
    bool_t   slc, sec;

    lg = stringByteLength(string);

    ch = 0;
    while(string[ch] != 0) {
      if(lg == 1 || (lg == 2 && (string[0] & 0x80))) {// The string is 1 glyph long
        slc = showLeadingCols;
        sec = showEndingCols;
      }
      else if(ch == 0) {// First glyph
        slc = showLeadingCols;
        sec = true;
      }
      else if(ch == lg-1 || (ch == lg-2 && (string[ch] & 0x80))) {// Last glyph
        slc = true;
        sec = showEndingCols;
      }
      else {// Glyph between first and last glyph
        slc = true;
        sec = true;
      }

      uint16_t charCode = (uint8_t)string[ch++];
      if(charCode & 0x80) {// MSB set?
        charCode = (charCode<<8) | (uint8_t)string[ch++];
      }

      x = showGlyphCode(charCode, font, x, y, videoMode, slc, sec) - compressString;        //JM compressString
    }
    compressString = 0;        //JM compressString
    return x;
  }




  uint32_t showStringC43(const char *string, int mode, int comp, uint32_t x, uint32_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols ) {
    int combinationFontsM = combinationFonts;
    if(combinationFontsDefault == 0) mode = stdNoEnlarge;
    
    compressString = comp;
    displaymode = mode;             // miniC and maxiC to be depreciated in favour of displaymode
    if(mode == stdNoEnlarge)         { miniC = 0 ; maxiC = 0; combinationFonts = combinationFontsDefault; x = showString(string, &standardFont, x, y, videoMode, showLeadingCols, showEndingCols );    } else
      if(mode == stdEnlarge)         { miniC = 0 ; maxiC = 1; combinationFonts = stdEnlarge;              x = showString(string, &standardFont, x, y, videoMode, showLeadingCols, showEndingCols );    } else
        if(mode == stdnumEnlarge)    { miniC = 0 ; maxiC = 1; combinationFonts = stdnumEnlarge;           x = showString(string, &numericFont , x, y, videoMode, showLeadingCols, showEndingCols );    } else
           if(mode == numSmall)      { miniC = 1 ; maxiC = 0; combinationFonts = combinationFontsDefault; x = showString(string, &numericFont , x, y, videoMode, showLeadingCols, showEndingCols );    } else
             if(mode == numHalf)     { miniC = 0 ; maxiC = 1; combinationFonts = numHalf;                 x = showString(string, &numericFont , x, y, videoMode, showLeadingCols, showEndingCols );    } else
               x = 0;
    
    miniC = 0; maxiC = 0; combinationFonts = combinationFontsM; compressString = 0; noShow = false; displaymode = stdNoEnlarge;
    return x;
  }



  uint32_t  stringWidthC43(const char *str, int mode, int comp, bool_t withLeadingEmptyRows, bool_t withEndingEmptyRows){
     noShow = true;
     return showStringC43(str, mode, comp, 0, 0, vmNormal, withLeadingEmptyRows, withEndingEmptyRows );
     //noShow = false; //no need to redo
  }



uint8_t lines    = 2;      //lines   0
uint8_t y_offset = 3;
uint8_t x_offset = 0;      //pixels 40
uint16_t current_cursor_x = 0;
uint16_t current_cursor_y = 0;
int16_t  displayAIMbufferoffset;
//#define lines 5                                                          //JMCURSOR vv
//#define y_offset 2    //lines   0
uint32_t showStringEdC43(uint32_t lastline, int16_t offset, int16_t edcursor, const char *string, uint32_t x, uint32_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols, bool_t noshow) {
  uint16_t ch, charCode, lg;
  int16_t  glyphId;
  bool_t   slc, sec;
  uint32_t  numPixels, orglastlines, tmpxy;
  const    glyph_t *glyph;
  uint8_t  editlines     = 5 ; 
//  uint8_t  maxbeforejump = 30; 
  uint8_t  yincr         = 21; 
  const    font_t *font;


  if(combinationFonts == 2) {
    font = &numericFont;                             //JM ENLARGE
  } else {
    font = &standardFont;                            //JM ENLARGE
  }


  lg = stringByteLength(string + offset);
  //printf("######## Printing:%s offset:%d length:%d:%s\n",string, offset, lg, string + offset);
  //printf("##>> %d x:%d y:%d \n",edcursor, x,y);
  if(combinationFonts !=0) {
    editlines     = 3 ;       //JM ENLARGE 5    number of editing lines                                        //JMCURSOR vv
//    maxbeforejump = 21;       //JM ENLARGE 30   number of bytes in string before jumpuing to multi line 
    yincr         = 35;       //JM ENLARGE 21   distasnce between editing wrapped lines
    x_offset      = 0;    //pixels 40
    if(/*lg>maxbeforejump*/ stringWidth(string + offset, &numericFont, showLeadingCols, showEndingCols) > SCREEN_WIDTH - 50 ) {  //jump from large letters to small letters
      if(/*lg > 3*maxbeforejump*/ stringWidth(string + offset, &numericFont, showLeadingCols, showEndingCols) > SCREEN_WIDTH * 3 - 70 ) combinationFonts = 0;  //Auto change to small font after some characters
      lines = editlines;
      y_offset = 1;
    } else {
      lines = 2;              //jump back to small letters
      y_offset = 3;
      last_CM = 253; //Force redraw
    }
  } else {
    editlines     = 5 ;       //JM ENLARGE 5    number of editing lines                                        //JMCURSOR vv
//    maxbeforejump = 30;       //JM ENLARGE 30   number of bytes in string before jumpuing to multi line 
    yincr         = 21;       //JM ENLARGE 21   distasnce between editing wrapped lines
    x_offset      = 0;    //pixels 40
    if(/*lg>maxbeforejump*/ stringWidth(string + offset, &standardFont, showLeadingCols, showEndingCols) > SCREEN_WIDTH - 50 ) {
      lines = editlines;
      y_offset = 2;
    } else {
      lines = 2;
      y_offset = 3;
      combinationFonts = combinationFontsDefault;  //Auto change back to large font
      last_CM = 253; //Force redraw
    }
  }
  if(lines == editlines || lg == 0) last_CM = 253; //Force redraw if 
  //printf("^^^^ combinationFonts=%d editlines=%d yincr=%d %d %d \n",combinationFonts, editlines, yincr, lines, last_CM);

  orglastlines = lastline;

  if(lastline > y_offset) {
    if(!noshow) clearScreen_old(false, true,false);
    x = x_offset; 
    y = (yincr-1) + y_offset * (yincr-1);
  }
  //****************************************
  ch = offset;
  while(string[ch] != 0) {

    //printf("%3d:%3d ",ch,(uint8_t)string[ch]);
    
    if(lg == 1 || (lg == 2 && (string[offset] & 0x80))) {// The string is 1 glyph long
      slc = showLeadingCols;
      sec = showEndingCols;
    }
    else if(ch == 0) {// First glyph
      slc = showLeadingCols;
      sec = true;
    }
    else if(ch == lg-1 || (ch == lg-2 && (string[ch] & 0x80))) {// Last glyph
      slc = true;
      sec = showEndingCols;
    }
    else {// Glyph between first and last glyph
      slc = true;
      sec = true;
    }

    //printf("^^^^ ch=%d edcursor=%d string[ch]=%d \n",ch, edcursor, string[ch]);

    if(ch == edcursor) {                 //draw cursor
       current_cursor_x = x;
       current_cursor_y = y;
       tmpxy = y-1;
       while (tmpxy < y + (yincr+1)) {
         if(!noshow) setBlackPixel(x,tmpxy);
         if(!noshow) setBlackPixel(x+1,tmpxy);
         tmpxy++;
       }
       x+=2;
    }

    charCode = (uint8_t)string[ch++];                         //get glyph code
    if(charCode & 0x80) {// MSB set?
      charCode = (charCode<<8) | (uint8_t)string[ch++];
    }
    glyph = NULL;
    glyphId = findGlyph(font, charCode);
    if(glyphId >= 0) {
      glyph = (font->glyphs) + glyphId;
    }
    else if(glyphId == -1) {                   //JMvv
      generateNotFoundGlyph(-1, charCode);
      glyph = &glyphNotFound;
    }
    else if(glyphId == -2) {
      generateNotFoundGlyph(-2, charCode);
      glyph = &glyphNotFound;
    }
    else {
      glyph = NULL;
    }                                         //JM^^


    numPixels = 0;

    numPixels += glyph->colsGlyph + glyph->colsAfterGlyph + glyph->colsBeforeGlyph;    // get width of glyph
    //printf(">>> lastline=%d string[ch]=%d x=%d numPixels=%d", lastline, string[ch], x, numPixels);
    if(string[ch]== 0) numPixels += 8;
    //printf("±±± lastline=%d string[ch]=%d x=%d numPixels=%d\n", lastline, string[ch], x, numPixels);

    if(x + numPixels > SCREEN_WIDTH-1 && lastline == orglastlines) { 
      x = x_offset; 
      y += yincr; 
      lastline--;
    } else    
    if(x + numPixels > SCREEN_WIDTH-1 && lastline > 1) { 
      x = 1; 
      y += yincr; 
      lastline--;
    } else
    if(x + numPixels > SCREEN_WIDTH-1 && lastline <= 1) {   
      xCursor = x;
      yCursor = y;
      return x;
    }

    maxiC = 1;                                                                            //JM
      x = showGlyphCode(charCode, font, x, y, videoMode, slc, sec) - compressString;        //JM compressString
    maxiC = 0;                                                                            //JM

  }
  //printf("\n");

  xCursor = x;
  yCursor = y;
  compressString = 0;                                                                     //JM compressString
  return xCursor;
}

                                                          //JMCURSOR ^^


void refresh_gui(void) {                                        //JM vv
#ifdef PC_BUILD
  while(gtk_events_pending()) {
    gtk_main_iteration();
  }
#endif
}


void force_refresh(void) {
#ifdef PC_BUILD
  gtk_widget_queue_draw(screen);

//FULL UPDATE (UGLY)
#ifdef FULLUPDATE
   refresh_gui();
#endif

#endif
#if DMCP_BUILD
  lcd_forced_refresh ();
#endif

//printf(">>> screenc:force_refresh\n");

}                                                              //JM ^^






  /********************************************//**
   * \brief Hides the cursor
   *
   * \param void
   * \return void
   ***********************************************/
  void hideCursor(void) {
    if(cursorEnabled) {
      if(cursorFont == &standardFont) {
        lcd_fill_rect(xCursor, yCursor + 10,  6,  6, LCD_SET_VALUE);
      }
      else {
        lcd_fill_rect(xCursor, yCursor + 15, 13, 13, LCD_SET_VALUE);
      }
    }
  }




/********************************************//**
 * \brief Displays the function of the
 * currently pressed button in the
 * upper left corner of the T register line
 *
 * \param[in] item     int16_t  Item ID to show
 * \param[in] counter  int8_t   number of 1/10 seconds until NOP
 * \return void
 ***********************************************/
void showFunctionName(int16_t item, int16_t delayInMs) {
  char padding[20];                                          //JM
  if(item == ITM_NOP && delayInMs == 0) {                        //JMvv Handle second and third longpress
    if(longpressDelayedkey2 != 0) {                              //  If a delayed key2 is defined, qeue it
      item = longpressDelayedkey2; 
      delayInMs = FUNCTION_NOPTIME;
      longpressDelayedkey2 = 0;
    } else
    if(longpressDelayedkey3 != 0) {                              //  If a delayed key3 is defined, qeue it
      item = longpressDelayedkey3; 
      delayInMs = FUNCTION_NOPTIME;
      longpressDelayedkey3 = 0;
    }
  }                                                              //JM^^
  showFunctionNameItem = item;
  if(running_program_jm) return;                             //JM
  showFunctionNameCounter = delayInMs;
  strcpy(padding,indexOfItems[abs(item)].itemCatalogName);   //JM
  strcat(padding,"    ");                                    //JM
  if(stringWidth(padding, &standardFont, true, true) + /*1*/ 20 + lineTWidth > SCREEN_WIDTH) {                //JM
    clearRegisterLine(REGISTER_T, true, false);
  }
  showString(padding, &standardFont, /*1*/ 20, Y_POSITION_OF_REGISTER_T_LINE /*+ 6*/, vmNormal, true, true);      //JM
}



/********************************************//**
 * \brief Hides the function name in the
 * upper left corner of the T register line
 * and clears the counter
 *
 * \param void
 * \return void
 ***********************************************/
void hideFunctionName(void) {
  showFunctionNameItem = 0;
  showFunctionNameCounter = 0;
  if(running_program_jm) return;                             //JM
  if(calcMode!=CM_AIM) refreshRegisterLine(REGISTER_T);                           //JM DO NOT CHANGE BACK TO CLEARING ONLY A SHORT PIECE. CHANGED IN TWEAKED AS WELL>
}



  /********************************************//**
   * \brief Clears one register line
   *
   * \param[in] yStart int16_t y coordinate from where starting to clear
   * \return void
   ***********************************************/
  void clearRegisterLine(calcRegister_t regist, bool_t clearTop, bool_t clearBottom) {
    if(REGISTER_X <= regist && regist <= REGISTER_T) {
      uint32_t yStart = Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X);
      uint32_t height = 32;

      if(clearTop) {
        yStart -= 4;
        height += 4;
      }

      if(clearBottom) {
        height += 4;
        if(regist == REGISTER_X) {
          height += 3;
        }
      }

      lcd_fill_rect(0, yStart, SCREEN_WIDTH, height, LCD_SET_VALUE);
    }
  }


uint8_t   displayStack_m = 255;                                                  //JMSHOIDISP
/********************************************//**
 * \brief Displays one register line
 *
 * \param[in] regist int16_t Register line to display
 * \return void
 ***********************************************/
void refreshRegisterLine(calcRegister_t regist) {
  int32_t w;
  int16_t wLastBaseNumeric, wLastBaseStandard, prefixWidth, lineWidth = 0;
  char prefix[18], lastBase[4];

if(displayStackSHOIDISP != 0 && lastIntegerBase != 0 && getRegisterDataType(REGISTER_X) == dtShortInteger) { //JMSHOI                   
  if(displayStack != 4-displayStackSHOIDISP) {displayStack_m = displayStack;}   //JMSHOI
  fnDisplayStack(4-displayStackSHOIDISP);                                       //JMSHOI             
} else {                                                                        //JMSHOI 
  if(displayStack_m != 255) {                                                   //JMSHOI
    fnDisplayStack(displayStack_m);                                             //JMSHOI
    displayStack_m = 255;                                                       //JMSHOI
  } else {
    //fnDisplayStack(4);  //removed because it clamps DSTACK to 4
                                                                                //displayStack_m = 255;//is already 255
  }                                                                             //JMSHOI
}                                                                               //JMSHOI

  #if (DEBUG_PANEL == 1)
    refreshDebugPanel();
  #endif

    if((calcMode != CM_BUG_ON_SCREEN) && (calcMode != CM_GRAPH) && (calcMode != CM_LISTXY)) {               //JM
      clearRegisterLine(regist, true, (regist != REGISTER_Y));

      #ifdef PC_BUILD
        #if (DEBUG_REGISTER_L == 1 || SHOW_MEMORY_STATUS == 1)
          char tmpStr[1000];
        #endif // (DEBUG_REGISTER_L == 1 || SHOW_MEMORY_STATUS == 1)
        #if (DEBUG_REGISTER_L == 1)
          char string1[1000], string2[1000], *p;
          uint16_t i;

          strcpy(string1, "L = ");

          if(getRegisterDataType(REGISTER_L) == dtReal34) {
            strcat(string1, "real34 = ");
            formatReal34Debug(string2, (real34_t *)getRegisterDataPointer(REGISTER_L));
            strcat(string2, " ");
            strcat(string2, getAngularModeName(getRegisterAngularMode(REGISTER_L)));
          }

          else if(getRegisterDataType(REGISTER_L) == dtComplex34) {
            strcat(string1, "complex34 = ");
            formatComplex34Debug(string2, (void *)getRegisterDataPointer(REGISTER_L));
          }

          else if(getRegisterDataType(REGISTER_L) == dtString) {
            strcat(string1, "string = ");
            for(i=0, p=REGISTER_STRING_DATA(REGISTER_L); i<=stringByteLength(REGISTER_STRING_DATA(REGISTER_L)); i++, p++) {
              string2[i] = *p;
            }
          }

          else if(getRegisterDataType(REGISTER_L) == dtShortInteger) {
            strcat(string1, "short integer = ");
            shortIntegerToDisplayString(REGISTER_L, string2, false);
            strcat(string2, STD_SPACE_3_PER_EM);
            strcat(string2, getShortIntegerModeName(shortIntegerMode));
          }

          else if(getRegisterDataType(REGISTER_L) == dtLongInteger) {
            strcat(string1, "long integer = ");
            longIntegerRegisterToDisplayString(REGISTER_L, string2, sizeof(string2), SCREEN_WIDTH, 50, STD_SPACE_PUNCTUATION, true);
          }

          else if(getRegisterDataType(REGISTER_L) == dtConfig) {
            strcat(string1, "Configuration data");
            string2[0] = 0;
          }

          else {
            sprintf(string2, "data type %s not supported for now!", getRegisterDataTypeName(REGISTER_L, false, false));
          }

          stringToUtf8(string1, (uint8_t *)tmpStr);
          stringToUtf8(string2, (uint8_t *)tmpStr + 500);

          gtk_label_set_label(GTK_LABEL(lblRegisterL1), tmpStr);
          gtk_label_set_label(GTK_LABEL(lblRegisterL2), tmpStr + 500);
          gtk_widget_show(lblRegisterL1);
          gtk_widget_show(lblRegisterL2);
        #endif // (DEBUG_REGISTER_L == 1)
        #if (SHOW_MEMORY_STATUS == 1)
          char string[1000];

          sprintf(string, "%" PRId32 " bytes free (%" PRId32 " region%s), C43 %" PRIu64 " bytes, GMP %" PRIu64 " bytes -> should always be 0", getFreeRamMemory(), numberOfFreeMemoryRegions, numberOfFreeMemoryRegions==1 ? "" : "s", TO_BYTES((uint64_t)wp43sMemInBlocks), (uint64_t)gmpMemInBytes);
          stringToUtf8(string, (uint8_t *)tmpStr);
          gtk_label_set_label(GTK_LABEL(lblMemoryStatus), tmpStr);
          gtk_widget_show(lblMemoryStatus);
        #endif // (SHOW_MEMORY_STATUS == 1)
      #endif // PC_BUILD

      #if defined VERBOSE_SCREEN && defined PC_BUILD
        printf("^^^^Display Register: %d temporaryInformation: %d\n",regist, temporaryInformation);
      #endif //VERBOSE_SCREEN



      if(temporaryInformation == TI_ARE_YOU_SURE && regist == REGISTER_X) {
        showString("Are you sure?", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
      }

      else if(temporaryInformation == TI_WHO && regist == REGISTER_X) {
        clearRegisterLine(REGISTER_Y, true, true); //JM ID
        showString(WHO, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
        showString(WHO2, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X + 1) + 6, vmNormal, true, true);      // JM ID
      }

      else if(temporaryInformation == TI_VERSION && regist == REGISTER_X) {
        showString(VERSION, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
      }

      else if(temporaryInformation == TI_FALSE && regist == TRUE_FALSE_REGISTER_LINE) {
        sprintf(tmpString, "false");
        showString(tmpString, &standardFont, 1, Y_POSITION_OF_TRUE_FALSE_LINE + 6, vmNormal, true, true);
      }

      else if(temporaryInformation == TI_TRUE && regist == TRUE_FALSE_REGISTER_LINE) {
        sprintf(tmpString, "true");
        showString(tmpString, &standardFont, 1, Y_POSITION_OF_TRUE_FALSE_LINE + 6, vmNormal, true, true);
      }

      else if(temporaryInformation == TI_RESET && regist == REGISTER_X) {
        sprintf(tmpString, "Data, programs, and definitions cleared");
        w = stringWidth(tmpString, &standardFont, true, true);
        showString(tmpString, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
      }

      else if(temporaryInformation == TI_SAVED && regist == REGISTER_X) {
        showString("Saved", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
      }

      else if(temporaryInformation == TI_BACKUP_RESTORED && regist == REGISTER_X) {
        showString("Backup restored", &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
      }


//Original SHOW

      else if((temporaryInformation == TI_SHOW_REGISTER) && regist == REGISTER_T) { // L1
      w = stringWidth(tmpString, &standardFont, true, true);
      showString(tmpString, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*0, vmNormal, true, true);
    }

      else if((temporaryInformation == TI_SHOW_REGISTER) && regist == REGISTER_Z && tmpString[300] != 0) { // L2 & L3
      w = stringWidth(tmpString + 300, &standardFont, true, true);
      showString(tmpString + 300, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*1, vmNormal, true, true);

        if(tmpString[600]) {
          w = stringWidth(tmpString + 600, &standardFont, true, true);
          showString(tmpString + 600, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*2, vmNormal, true, true);
        }
      }

      else if((temporaryInformation == TI_SHOW_REGISTER) && regist == REGISTER_Y && tmpString[900] != 0) { // L4 & L5
      w = stringWidth(tmpString + 900, &standardFont, true, true);
      showString(tmpString + 900, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*3, vmNormal, true, true);

        if(tmpString[1200]) {
          w = stringWidth(tmpString + 1200, &standardFont, true, true);
          showString(tmpString + 1200, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*4, vmNormal, true, true);
        }
      }

      else if((temporaryInformation == TI_SHOW_REGISTER) && regist == REGISTER_X && tmpString[1500] != 0) { // L6 & L7
      w = stringWidth(tmpString + 1500, &standardFont, true, true);
      showString(tmpString + 1500, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*5, vmNormal, true, true);

        if(tmpString[1800]) {
          w = stringWidth(tmpString + 1800, &standardFont, true, true);
          showString(tmpString + 1800, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + 21*6, vmNormal, true, true);
        }
      }


// NEW SHOW
                                                                         //JMSHOW vv

        else if(temporaryInformation == TI_SHOW_REGISTER_SMALL) {
          #define line_h0 21
          switch(regist) {
            // L1
            case REGISTER_T: w = stringWidth(tmpString, &standardFont, true, true);
                             showString(tmpString, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true);
                             #if defined VERBOSE_SCREEN && defined PC_BUILD
                               printf("^^^^NEW SHOW A: Display Register T: %s\n",tmpString);
                             #endif //VERBOSE_SCREEN
                             break;
            // L2 & L3
            case REGISTER_Z: w = stringWidth(tmpString + 300, &standardFont, true, true);
                             showString(tmpString + 300, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h0, vmNormal, true, true);
                             #if defined VERBOSE_SCREEN && defined PC_BUILD
                               printf("^^^^NEW SHOW A: Display Register Z: %s\n",tmpString + 300);
                             #endif //VERBOSE_SCREEN
                             if(tmpString[600]) {
                               w = stringWidth(tmpString + 600, &standardFont, true, true);
                               showString(tmpString + 600, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h0*2, vmNormal, true, true);
                               #if defined VERBOSE_SCREEN && defined PC_BUILD
                                 printf("^^^^NEW SHOW A: Display Register Z2: %s\n",tmpString + 600);
                               #endif //VERBOSE_SCREEN
                             }
                             break;
            // L4 & L5
            case REGISTER_Y: w = stringWidth(tmpString + 900, &standardFont, true, true);
                             showString(tmpString + 900, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h0*3, vmNormal, true, true);
                             #if defined VERBOSE_SCREEN && defined PC_BUILD
                               printf("^^^^NEW SHOW A: Display Register Y: %s\n",tmpString + 900);
                             #endif //VERBOSE_SCREEN
                             if(tmpString[1200]) {
                               w = stringWidth(tmpString + 1200, &standardFont, true, true);
                               showString(tmpString + 1200, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h0*4, vmNormal, true, true);
                               #if defined VERBOSE_SCREEN && defined PC_BUILD
                                 printf("^^^^NEW SHOW A: Display Register Y2: %s\n",tmpString + 1200);
                               #endif //VERBOSE_SCREEN
                             }
                             break;
            // L6 & L7
            case REGISTER_X: w = stringWidth(tmpString + 1500, &standardFont, true, true);
                             showString(tmpString + 1500, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h0*5, vmNormal, true, true);
                             #if defined VERBOSE_SCREEN && defined PC_BUILD
                               printf("^^^^NEW SHOW A: Display Register X: %s\n",tmpString + 1500);
                             #endif //VERBOSE_SCREEN
                             if(tmpString[1800]) {
                               w = stringWidth(tmpString + 1800, &standardFont, true, true);
                               showString(tmpString + 1800, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h0*6, vmNormal, true, true);
                               #if defined VERBOSE_SCREEN && defined PC_BUILD
                                 printf("^^^^NEW SHOW A: Display Register X2: %s\n",tmpString + 1800);
                               #endif //VERBOSE_SCREEN
                             }
                             break;
            default: {}
          }
        }
                                                                         //JMSHOW ^^


        else if(temporaryInformation == TI_SHOW_REGISTER_BIG) {
          #define line_h1 38
          switch(regist) {
            // L1
            case REGISTER_T: w = stringWidthC43(tmpString, stdnumEnlarge, nocompress, true, true);
                             showStringC43(tmpString, stdnumEnlarge, nocompress, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true);
                             #if defined VERBOSE_SCREEN && defined PC_BUILD
                               printf("^^^^NEW SHOW: Display Register T: %s\n",tmpString);
                             #endif //VERBOSE_SCREEN
                             break;
            // L2 & L3
            case REGISTER_Z: w = stringWidthC43(tmpString + 300, stdnumEnlarge, nocompress, true, true);
                             showStringC43(tmpString + 300, stdnumEnlarge, nocompress,  SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h1, vmNormal, true, true);
                             #if defined VERBOSE_SCREEN && defined PC_BUILD
                               printf("^^^^NEW SHOW: Display Register Z: %s\n",tmpString + 300);
                             #endif //VERBOSE_SCREEN
                             break;
            // L4 & L5
            case REGISTER_Y: w = stringWidthC43(tmpString + 600, stdnumEnlarge, nocompress, true, true);
                             showStringC43(tmpString + 600, stdnumEnlarge, nocompress,  SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h1*2, vmNormal, true, true);
                             #if defined VERBOSE_SCREEN && defined PC_BUILD
                               printf("^^^^NEW SHOW: Display Register Y: %s\n",tmpString + 600);
                             #endif //VERBOSE_SCREEN
                             break;
            // L6 & L7
            case REGISTER_X: w = stringWidthC43(tmpString + 900, stdnumEnlarge, nocompress, true, true);
                             showStringC43(tmpString + 900, stdnumEnlarge, nocompress,  SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_T_LINE + line_h1*3, vmNormal, true, true);
                             #if defined VERBOSE_SCREEN && defined PC_BUILD
                               printf("^^^^NEW SHOW: Display Register X: %s\n",tmpString + 900);
                             #endif //VERBOSE_SCREEN
                             break;
            default: {}
          }
//          if(getRegisterDataType(REGISTER_X) == dtReal34) {
//            real34ToDisplayString(REGISTER_REAL34_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X), tmpString, &numericFont, SCREEN_WIDTH, 34, true, STD_SPACE_PUNCTUATION);
//          }
        }
                                                                         //JMSHOW ^^


    else if(regist < REGISTER_X + displayStack || (lastErrorCode != 0 && regist == errorMessageRegisterLine)) {
      prefixWidth = 0;

        if(lastErrorCode != 0 && regist == errorMessageRegisterLine) {
          if(stringWidth(errorMessages[lastErrorCode], &standardFont, true, true) <= SCREEN_WIDTH - 1) {
            showString(errorMessages[lastErrorCode], &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
          }
          else {
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
              sprintf(errorMessage, "Error message %" PRIu8 " is too wide!", lastErrorCode);
              moreInfoOnError("In function refreshRegisterLine:", errorMessage, errorMessages[lastErrorCode], NULL);
            #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
            sprintf(tmpString, "Error message %" PRIu8 " is too wide!", lastErrorCode);
            w = stringWidth(tmpString, &standardFont, true, true);
            showString(tmpString, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, true, true);
          }
        }

        else if(regist == NIM_REGISTER_LINE && calcMode == CM_NIM) {
          if(lastIntegerBase != 0) {
            lastBase[0] = '#';
            if(lastIntegerBase > 9) {
              lastBase[1] = '1';
              lastBase[2] = '0' + (lastIntegerBase - 10);
              lastBase[3] = 0;
            }
            else {
              lastBase[1] = '0' + lastIntegerBase;
              lastBase[2] = 0;
            }
            wLastBaseNumeric  = stringWidth(lastBase, &numericFont,  true, true);
            wLastBaseStandard = stringWidth(lastBase, &standardFont, true, true);
          }
          else {
            wLastBaseNumeric  = 0;
            wLastBaseStandard = 0;
          }

          if(stringWidth(nimBufferDisplay, &numericFont, true, true) + wLastBaseNumeric <= SCREEN_WIDTH - 16) { // 16 is the numeric font cursor width
            xCursor = showString(nimBufferDisplay, &numericFont, 0, Y_POSITION_OF_NIM_LINE, vmNormal, true, true);
            yCursor = Y_POSITION_OF_NIM_LINE;
            cursorFont = &numericFont;

            if(lastIntegerBase != 0) {
              showString(lastBase, &numericFont, xCursor + 16, Y_POSITION_OF_NIM_LINE, vmNormal, true, true);
            }
          }
          else if(stringWidth(nimBufferDisplay, &standardFont, true, true) + wLastBaseStandard <= SCREEN_WIDTH - 8) { // 8 is the standard font cursor width
            xCursor = showString(nimBufferDisplay, &standardFont, 0, Y_POSITION_OF_NIM_LINE + 6, vmNormal, true, true);
            yCursor = Y_POSITION_OF_NIM_LINE + 6;
            cursorFont = &standardFont;

            if(lastIntegerBase != 0) {
              showString(lastBase, &standardFont, xCursor + 8, Y_POSITION_OF_NIM_LINE + 6, vmNormal, true, true);
            }
          }
          else {
            w = stringByteLength(nimBufferDisplay) + 1;
            xcopy(tmpString,        nimBufferDisplay, w);
            xcopy(tmpString + 1500, nimBufferDisplay, w);
            while(stringWidth(tmpString, &standardFont, true, true) >= SCREEN_WIDTH) {
              w = stringLastGlyph(tmpString);
              tmpString[w] = 0;
            }

            if(stringWidth(tmpString + 1500 + w, &standardFont, true, true) + wLastBaseStandard > SCREEN_WIDTH - 8) { // 8 is the standard font cursor width
              btnClicked(NULL, "16"); // back space
            }
            else {
              showString(tmpString, &standardFont, 0, Y_POSITION_OF_NIM_LINE - 3, vmNormal, true, true);

              xCursor = showString(tmpString + 1500 + w, &standardFont, 0, Y_POSITION_OF_NIM_LINE + 18, vmNormal, true, true);
              yCursor = Y_POSITION_OF_NIM_LINE + 18;
              cursorFont = &standardFont;

              if(lastIntegerBase != 0) {
                showString(lastBase, &standardFont, xCursor + 8, Y_POSITION_OF_NIM_LINE + 18, vmNormal, true, true);
              }
            }
          }
        }

      else if(regist == AIM_REGISTER_LINE && calcMode == CM_AIM) {




  //JMCURSOR vv
        int16_t tmplen = stringByteLength(aimBuffer);
        if(T_cursorPos > tmplen) {T_cursorPos = tmplen;}     //Do range checking in case the cursor starts off outside of range
        if(T_cursorPos < 0)      {T_cursorPos = tmplen;}     //Do range checking in case the cursor starts off outside of range
        int16_t cnt;
        displayAIMbufferoffset = 0;                                         //Determine offset to be able to display the latter part of the string

        int16_t sw=SCREEN_WIDTH-1-x_offset;              //Calculate the total available pixels in the number of lines available.
        if(lines==1 && y_offset<1) {sw = SCREEN_WIDTH-1 - x_offset;} 
        else if(lines==2 && y_offset<=1) {sw = (SCREEN_WIDTH-1 - x_offset)*(2-y_offset);}
        else if(lines>2) {sw = (SCREEN_WIDTH-1 - x_offset)*2 + (lines-2)*(SCREEN_WIDTH-1);}
        cnt = 0;
        do {
          //printf(">>>a %d %d >? %d\n", stringWidth(aimBuffer + displayAIMbufferoffset, font, true, true), - stringWidth(aimBuffer + T_cursorPos, font, true, true), sw);
          while(/*cnt++ < 100 && */
               ((int32_t)stringWidthC43(aimBuffer + displayAIMbufferoffset, combinationFonts ,nocompress, true, true) - (int32_t)stringWidthC43(aimBuffer + T_cursorPos, combinationFonts ,nocompress, true, true) +lines*15 > sw) &&     //assume max of 15 pixels lost at the end of each line 
          	   (displayAIMbufferoffset <= tmplen) && 
          	   (displayAIMbufferoffset + 10 < T_cursorPos)
          	   ) 
            {
              //printf(">>>b %d %d >? %d\n", stringWidth(aimBuffer + displayAIMbufferoffset, font, true, true), - stringWidth(aimBuffer + T_cursorPos, font, true, true), sw);
              displayAIMbufferoffset = stringNextGlyph(aimBuffer, displayAIMbufferoffset);
            }

          showStringEdC43(lines ,displayAIMbufferoffset, T_cursorPos, aimBuffer, 1, Y_POSITION_OF_NIM_LINE - 3, vmNormal, true, true, false);  //display up to the cursor

          if(xCursor > SCREEN_WIDTH-1) displayAIMbufferoffset = stringNextGlyph(aimBuffer, displayAIMbufferoffset);
          //printf(">>>c length:%d T_cursorPos:%d displayAIMbufferoffset:%d x:%d y:%d\n",tmplen, T_cursorPos, displayAIMbufferoffset, xCursor, yCursor);
        } while(cnt++ < 150 && xCursor > SCREEN_WIDTH-1);


        if(T_cursorPos == tmplen) cursorEnabled = true; else cursorEnabled = false; 
        if(combinationFonts == 2) {
          cursorFont = &numericFont;                             //JM ENLARGE
        } else {
          cursorFont = &standardFont;                            //JM ENLARGE
        }
      }
  //JMCURSOR  ^^


        else if(   getSystemFlag(FLAG_FRACT)
                && (    getRegisterDataType(regist) == dtReal34
                     && (
                            (   real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(regist), const34_1e_4)
                             && real34CompareAbsLessThan(REGISTER_REAL34_DATA(regist), const34_1e6)
                            )
                         || real34IsZero(REGISTER_REAL34_DATA(regist))
                        )
                   )
               ) {
          fractionToDisplayString(regist, tmpString);

          w = stringWidth(tmpString, &numericFont, false, true);
          lineWidth = w;
          if(w <= SCREEN_WIDTH) {
            showString(tmpString, &numericFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
          }
          else {
            w = stringWidth(tmpString, &standardFont, false, true);
            lineWidth = w;
            if(w > SCREEN_WIDTH) {
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                moreInfoOnError("In function refreshRegisterLine:", "Fraction representation too wide!", tmpString, NULL);
              #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
              strcpy(tmpString, "Fraction representation too wide!");
              w = stringWidth(tmpString, &standardFont, false, true);
              lineWidth = w;
            }
            showString(tmpString, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
          }
        }

        else if(getRegisterDataType(regist) == dtReal34) {
          if(temporaryInformation == TI_RADIUS_THETA) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "r" STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, STD_theta STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_X_Y) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "x" STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, "y" STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_RE_IM) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "Im" STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, "Re" STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_SUMX_SUMY) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_SIGMA "x" STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, STD_SIGMA "y" STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_XMIN_YMIN) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "x" STD_SUB_m STD_SUB_i STD_SUB_n STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, "y" STD_SUB_m STD_SUB_i STD_SUB_n STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_XMAX_YMAX) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "x" STD_SUB_m STD_SUB_a STD_SUB_x STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, "y" STD_SUB_m STD_SUB_a STD_SUB_x STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_MEANX_MEANY) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_x_BAR STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, STD_y_BAR STD_SPACE_FIGURE "=");
               prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
           }

          else if(temporaryInformation == TI_SAMPLSTDDEV) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "s" STD_SUB_x STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, "s" STD_SUB_y STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_POPLSTDDEV) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_sigma STD_SUB_x STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, STD_sigma STD_SUB_y STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_STDERR) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "s" STD_SUB_m STD_SUB_x STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, "s" STD_SUB_m STD_SUB_y STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_GEOMMEANX_GEOMMEANY) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_x_BAR STD_SUB_G STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, STD_y_BAR STD_SUB_G STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_GEOMSAMPLSTDDEV) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_epsilon STD_SUB_x STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, STD_epsilon STD_SUB_y STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_GEOMPOPLSTDDEV) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_epsilon STD_SUB_m STD_SUB_x STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, STD_epsilon STD_SUB_m STD_SUB_y STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_GEOMSTDERR) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_epsilon STD_SUB_p STD_SUB_x STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, STD_epsilon STD_SUB_p STD_SUB_y STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_WEIGHTEDMEANX) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_x_BAR STD_SUB_w STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_WEIGHTEDSAMPLSTDDEV) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "s" STD_SUB_w STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_WEIGHTEDPOPLSTDDEV) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_sigma STD_SUB_w STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_WEIGHTEDSTDERR) {
            if(regist == REGISTER_X) {
              strcpy(prefix, "s" STD_SUB_m STD_SUB_w STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_HARMMEANX_HARMMEANY) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_x_BAR STD_SUB_H STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, STD_y_BAR STD_SUB_H STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

          else if(temporaryInformation == TI_RMSMEANX_RMSMEANY) {
            if(regist == REGISTER_X) {
              strcpy(prefix, STD_x_BAR STD_SUB_R STD_SUB_M STD_SUB_S STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
            else if(regist == REGISTER_Y) {
              strcpy(prefix, STD_y_BAR STD_SUB_R STD_SUB_M STD_SUB_S STD_SPACE_FIGURE "=");
              prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            }
          }

        else if(temporaryInformation == TI_STATISTIC_SUMS) {
          if(regist == REGISTER_Y) {
            realToInt32(SIGMA_N, w);
            sprintf(prefix, "Data point %03" PRId32, w);
            prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
            lcd_fill_rect(0, Y_POSITION_OF_REGISTER_Y_LINE - 2, SCREEN_WIDTH, 1, LCD_EMPTY_VALUE);
          }
        }
            else if(temporaryInformation == TI_ABC) {                             //JM EE \/
              if(regist == REGISTER_X) {
                strcpy(prefix, "c" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Y) {
                strcpy(prefix, "b" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Z) {
                strcpy(prefix, "a" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
            }

            else if(temporaryInformation == TI_ABBCCA) {
              if(regist == REGISTER_X) {
                strcpy(prefix, "ca" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Y) {
                strcpy(prefix, "bc" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Z) {
                strcpy(prefix, "ab" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
            }

            else if(temporaryInformation == TI_012) {
              if(regist == REGISTER_X) {
                strcpy(prefix, "sym2" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Y) {
                strcpy(prefix, "sym1" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Z) {
                strcpy(prefix, "sym0" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
            }

            if(prefixWidth > 0) {
              if(regist == REGISTER_X) {
                showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + TEMPORARY_INFO_OFFSET, vmNormal, true, true);
              } else
              if(regist == REGISTER_Y) {
                showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_Y_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_Y) + TEMPORARY_INFO_OFFSET, vmNormal, true, true);
              } else
              if(regist == REGISTER_Z) {
                showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_Z_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_Z) + TEMPORARY_INFO_OFFSET, vmNormal, true, true);
              }
            }
                                                                       //JM EE ^


          real34ToDisplayString(REGISTER_REAL34_DATA(regist), getRegisterAngularMode(regist), tmpString, &numericFont, SCREEN_WIDTH - prefixWidth, NUMBER_OF_DISPLAY_DIGITS, true, STD_SPACE_PUNCTUATION);

          w = stringWidth(tmpString, &numericFont, false, true);
          lineWidth = w;
          if(prefixWidth > 0) {
            showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + TEMPORARY_INFO_OFFSET, vmNormal, true, true);
          }
          showString(tmpString, &numericFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
        }

          //JM else if(getRegisterDataType(regist) == dtComplex34) {                                                                                                      //JM EE Removed and replaced with the below
          //JM complex34ToDisplayString(REGISTER_COMPLEX34_DATA(regist), tmpString, &numericFont, SCREEN_WIDTH, NUMBER_OF_DISPLAY_DIGITS, true, STD_SPACE_PUNCTUATION);   //JM EE Removed and replaced with the below

          else if(getRegisterDataType(regist) == dtComplex34) {
            if(temporaryInformation == TI_ABC) {                             //JM EE \/
              if(regist == REGISTER_X) {
                strcpy(prefix, "c" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Y) {
                strcpy(prefix, "b" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Z) {
                strcpy(prefix, "a" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
            }

            else if(temporaryInformation == TI_ABBCCA) {
              if(regist == REGISTER_X) {
                strcpy(prefix, "ca" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Y) {
                strcpy(prefix, "bc" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Z) {
                strcpy(prefix, "ab" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
            }

            else if(temporaryInformation == TI_012) {
              if(regist == REGISTER_X) {
                strcpy(prefix, "sym2" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Y) {
                strcpy(prefix, "sym1" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
              else if(regist == REGISTER_Z) {
                strcpy(prefix, "sym0" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
            }

            if(prefixWidth > 0) {
              if(regist == REGISTER_X) {
                showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + TEMPORARY_INFO_OFFSET, vmNormal, true, true);
              } else
              if(regist == REGISTER_Y) {
                showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_Y_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_Y) + TEMPORARY_INFO_OFFSET, vmNormal, true, true);
              } else
              if(regist == REGISTER_Z) {
                showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_Z_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_Z) + TEMPORARY_INFO_OFFSET, vmNormal, true, true);
              }
            }
                                                                       //JM EE ^



            complex34ToDisplayString(REGISTER_COMPLEX34_DATA(regist), tmpString, &numericFont, SCREEN_WIDTH - prefixWidth, NUMBER_OF_DISPLAY_DIGITS,true, STD_SPACE_PUNCTUATION);

          w = stringWidth(tmpString, &numericFont, false, true);
          lineWidth = w;
          showString(tmpString, &numericFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
        }

        else if(getRegisterDataType(regist) == dtString) {
                                                                                  //JMvv

       //JM REGISTER STRING LARGE FONTS
        #ifdef STACK_X_STR_LRG_FONT
          //This is for X
          w = stringWidthC43(REGISTER_STRING_DATA(regist), stdnumEnlarge, nocompress, false, true);
          if(regist == REGISTER_X && w<SCREEN_WIDTH) {
            lineWidth = w; //slighly incorrect if special characters are there as well.
            showStringC43(REGISTER_STRING_DATA(regist), stdnumEnlarge, nocompress, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, false, true);
          } else                                                                   //JM
        #endif

          #ifdef STACK_X_STR_MED_FONT
            //This is for X
            if(regist == REGISTER_X && stringWidthC43(REGISTER_STRING_DATA(regist), numHalf, nocompress, false, true) < SCREEN_WIDTH) {
              w = stringWidthC43(REGISTER_STRING_DATA(regist), numHalf,    nocompress, false, true);
              lineWidth = w;
              showStringC43(REGISTER_STRING_DATA(regist), numHalf, nocompress, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, false, true);
            } else                                                                   //JM
          #endif

            #ifdef STACK_STR_MED_FONT
              //This is for Y, Z & T
              if(regist >= REGISTER_Y && regist <= REGISTER_T && stringWidthC43(REGISTER_STRING_DATA(regist), numHalf, nocompress, false, true) < SCREEN_WIDTH) {
                w = stringWidthC43(REGISTER_STRING_DATA(regist), numHalf, nocompress, false, true);
                lineWidth = w;
                showStringC43(REGISTER_STRING_DATA(regist), numHalf, nocompress, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, false, true);
              } else                                                                   //JM
            #endif
          //JM ^^ large fonts


        { //printf("^^^^#### combinationFonts=%d maxiC=%d miniC=%d displaymode=%d\n",combinationFonts, maxiC, miniC, displaymode);
          w = stringWidth(REGISTER_STRING_DATA(regist), &standardFont, false, true);

          if(w >= SCREEN_WIDTH) {
            if(regist == REGISTER_X) {
              xcopy(tmpString, REGISTER_STRING_DATA(regist), stringByteLength(REGISTER_STRING_DATA(regist)) + 1);
              do {
                tmpString[stringLastGlyph(tmpString)] = 0;
                w = stringWidth(tmpString, &standardFont, false, true);
              } while(w >= SCREEN_WIDTH);
              showString(tmpString, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - 3, vmNormal, false, true);

              w = stringByteLength(tmpString);
              xcopy(tmpString, REGISTER_STRING_DATA(regist) + w, stringByteLength(REGISTER_STRING_DATA(regist) + w) + 1);
              w = stringWidth(tmpString, &standardFont, false, true);
              if(w >= SCREEN_WIDTH) {
                do {
                  tmpString[stringLastGlyph(tmpString)] = 0;
                  w = stringWidth(tmpString, &standardFont, false, true);
                } while(w >= SCREEN_WIDTH - 14); // 14 is the width of STD_ELLIPSIS
                xcopy(tmpString + stringByteLength(tmpString), STD_ELLIPSIS, 3);
                w += 14;
              }
              showString(tmpString, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE + 18, vmNormal, false, true);
            }
            else {
              xcopy(tmpString, REGISTER_STRING_DATA(regist), stringByteLength(REGISTER_STRING_DATA(regist)) + 1);
              do {
                tmpString[stringLastGlyph(tmpString)] = 0;
                w = stringWidth(tmpString, &standardFont, false, true);
              } while(w >= SCREEN_WIDTH - 14); // 14 is the width of STD_ELLIPSIS
              xcopy(tmpString + stringByteLength(tmpString), STD_ELLIPSIS, 3);
              w += 14;
              lineWidth = w;
              showString(tmpString, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, false, true);
            }
          }
          else {
            lineWidth = w;
            showString(REGISTER_STRING_DATA(regist), &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, false, true);
          }
        }
      }

      else if(getRegisterDataType(regist) == dtShortInteger) {
        shortIntegerToDisplayString(regist, tmpString, true);
        showString(tmpString, fontForShortInteger, SCREEN_WIDTH - stringWidth(tmpString, fontForShortInteger, false, true), Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + (fontForShortInteger == &standardFont ? 6 : 0), vmNormal, false, true);
        //JM SHOIDISP // use the top part of the screen for HEX and BIN    //JM vv SHOIDISP
        if(displayStack == 4-displayStackSHOIDISP && lastIntegerBase != 0) {
          if(displayStack == 1){
            copySourceRegisterToDestRegister(REGISTER_Y,TEMP_REGISTER_1);
            copySourceRegisterToDestRegister(REGISTER_X,REGISTER_Y);
            setRegisterTag(REGISTER_Y, 2);
            shortIntegerToDisplayString(REGISTER_Y, tmpString, true);
            if(stringWidth(tmpString, fontForShortInteger, false, true) + stringWidth("  X: ", &standardFont, false, true) <= SCREEN_WIDTH) {
              showString("  X: ", &standardFont, 0, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Y - REGISTER_X) + (fontForShortInteger == &standardFont ? 6 : 0), vmNormal, false, true);
            }
            showString(tmpString, fontForShortInteger, SCREEN_WIDTH - stringWidth(tmpString, fontForShortInteger, false, true), Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Y - REGISTER_X) + (fontForShortInteger == &standardFont ? 6 : 0), vmNormal, false, true);
            copySourceRegisterToDestRegister(TEMP_REGISTER_1,REGISTER_Y);
          }
          if(displayStack == 1 || displayStack == 2){
            copySourceRegisterToDestRegister(REGISTER_Z,TEMP_REGISTER_1);
            copySourceRegisterToDestRegister(REGISTER_X,REGISTER_Z);
            if(displayStack == 2) setRegisterTag(REGISTER_Z, 2); else 
              if(displayStack == 1) setRegisterTag(REGISTER_Z, 8);
            shortIntegerToDisplayString(REGISTER_Z, tmpString, true);
            if(stringWidth(tmpString, fontForShortInteger, false, true) + stringWidth("  X: ", &standardFont, false, true) <= SCREEN_WIDTH) {
              showString("  X: ", &standardFont, 0, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Z - REGISTER_X) + (fontForShortInteger == &standardFont ? 6 : 0), vmNormal, false, true);
            }
            showString(tmpString, fontForShortInteger, SCREEN_WIDTH - stringWidth(tmpString, fontForShortInteger, false, true), Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_Z - REGISTER_X) + (fontForShortInteger == &standardFont ? 6 : 0), vmNormal, false, true);
            copySourceRegisterToDestRegister(TEMP_REGISTER_1,REGISTER_Z);
          }
          if(displayStack == 1 || displayStack == 2 || displayStack ==3){
            copySourceRegisterToDestRegister(REGISTER_T,TEMP_REGISTER_1);
            copySourceRegisterToDestRegister(REGISTER_X,REGISTER_T);
            setRegisterTag(REGISTER_T, 16);
            shortIntegerToDisplayString(REGISTER_T, tmpString, true);
            if(stringWidth(tmpString, fontForShortInteger, false, true) + stringWidth("  X: ", &standardFont, false, true) <= SCREEN_WIDTH) {
              showString("  X: ", &standardFont, 0, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_T - REGISTER_X) + (fontForShortInteger == &standardFont ? 6 : 0), vmNormal, false, true);
            }
            showString(tmpString, fontForShortInteger, SCREEN_WIDTH - stringWidth(tmpString, fontForShortInteger, false, true), Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(REGISTER_T - REGISTER_X) + (fontForShortInteger == &standardFont ? 6 : 0), vmNormal, false, true);
            copySourceRegisterToDestRegister(TEMP_REGISTER_1,REGISTER_T);
          }
            if(displayStack == 3)
              lcd_fill_rect(0, Y_POSITION_OF_REGISTER_Z_LINE - 2, SCREEN_WIDTH, 1, 0xFF); else
            if(displayStack == 2)
              lcd_fill_rect(0, Y_POSITION_OF_REGISTER_Y_LINE - 2, SCREEN_WIDTH, 1, 0xFF); else
            if(displayStack == 1)
              lcd_fill_rect(0, Y_POSITION_OF_REGISTER_X_LINE - 2, SCREEN_WIDTH, 1, 0xFF);            
        }                                                                 //JM ^^

              }

      else if(getRegisterDataType(regist) == dtLongInteger) {

             if(temporaryInformation == TI_ms) {                             //JMms vv
              if(regist == REGISTER_X) {
                strcpy(prefix, "t (ms)" STD_SPACE_FIGURE "=");
                prefixWidth = stringWidth(prefix, &standardFont, true, true) + 1;
              }
            }

            if(prefixWidth > 0) {
              if(regist == REGISTER_X) {
                showString(prefix, &standardFont, 1, Y_POSITION_OF_REGISTER_X_LINE + TEMPORARY_INFO_OFFSET - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, true, true);
              }
            }                                                               //JMms ^^

          longIntegerRegisterToDisplayString(regist, tmpString, TMP_STR_LENGTH, SCREEN_WIDTH - prefixWidth, 50, STD_SPACE_PUNCTUATION, true);          //JMms added prefix   //JM added last parameter: Allow LARGELI

          w = stringWidth(tmpString, &numericFont, false, true);
          lineWidth = w;

          if(w <= SCREEN_WIDTH) {
            showString(tmpString, &numericFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
          }
          else {
            w = stringWidth(tmpString, &standardFont, false, true);
            if(w > SCREEN_WIDTH) {
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                moreInfoOnError("In function refreshRegisterLine:", "Long integer representation too wide!", tmpString, NULL);
              #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
              strcpy(tmpString, "Long integer representation too wide!");
            }
            w = stringWidth(tmpString, &standardFont, false, true);
            lineWidth = w;
            showString(tmpString, &standardFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, false, true);
          }
        }

        else if(getRegisterDataType(regist) == dtConfig) {
          xcopy(tmpString, "Configuration data", 19);
          w = stringWidth(tmpString, &numericFont, false, true);
          lineWidth = w;
          showString(tmpString, &numericFont, SCREEN_WIDTH - w, Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X), vmNormal, false, true);
        }

        else {
          sprintf(tmpString, "Displaying %s: to be coded!", getRegisterDataTypeName(regist, true, false));
          showString(tmpString, &standardFont, SCREEN_WIDTH - stringWidth(tmpString, &standardFont, false, true), Y_POSITION_OF_REGISTER_X_LINE - REGISTER_LINE_HEIGHT*(regist - REGISTER_X) + 6, vmNormal, false, true);
        }
      }

      if(regist == REGISTER_T) {
        lineTWidth = lineWidth;
      }
    }
  }



  static void displayShiftAndTamBuffer(void) {
    if(shiftF) {
      showGlyph(STD_SUP_f, &numericFont, 0, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true); // f is pixel 4+8+3 wide
    }
    else if(shiftG) {
      showGlyph(STD_SUP_g, &numericFont, 0, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true); // g is pixel 4+10+1 wide
    }

    if(tamMode) {
      if(calcMode == CM_PEM) { // Variable line to display TAM informations
        lcd_fill_rect(45+20, tamOverPemYPos, 168, 20, LCD_SET_VALUE);
        showString(tamBuffer, &standardFont, 75+20, tamOverPemYPos, vmNormal,  false, false);
      }
      else { // Fixed line to display TAM informations
        if(shiftF || shiftG) {
          lcd_fill_rect(18, Y_POSITION_OF_TAM_LINE, 120, 32, LCD_SET_VALUE);
        }
        else {
          lcd_fill_rect(0, Y_POSITION_OF_TAM_LINE, 138, 32, LCD_SET_VALUE);
        }
        showString(tamBuffer, &standardFont, 18, Y_POSITION_OF_TAM_LINE + 6, vmNormal, true, true);
      }
    }
  }



int16_t refreshScreenCounter = 0;        //JM
uint8_t last_CM = 255;
void refreshScreen(void) {
if (running_program_jm) return;          //JM TEST PROGRAM!
#ifdef PC_BUILD
  jm_show_calc_state("refreshScreen");
  printf(">>> refreshScreenCounter=%d calcMode=%d last_CM=%d \n",refreshScreenCounter++, calcMode, last_CM);    //JMYY
#endif
#ifdef INLINE_TEST
  if(testEnabled) { fnSwStart(3); }     //dr
#endif

  //clearScreen();  //JM do not use this clearscreen. Rather use the distributed clearscreens, WITH the if(last_CM != calcMode)


  if(calcMode!=CM_AIM && calcMode!=CM_NIM && calcMode!=CM_GRAPH && calcMode!=CM_LISTXY) {last_CM = 254;}  //JM Force NON-CM_AIM and NON-CM_NIM to refresh to be compatible to 43S 

  switch(calcMode) {
    case CM_FLAG_BROWSER:
      last_CM = calcMode;
      clearScreen();
      flagBrowser(NOPARAM);
      refreshStatusBar();
      break;

    case CM_FONT_BROWSER:
      last_CM = calcMode;
      clearScreen();
      fontBrowser(NOPARAM);
      refreshStatusBar();
      break;

    case CM_REGISTER_BROWSER:
      last_CM = calcMode;
      clearScreen();
      registerBrowser(NOPARAM);
      refreshStatusBar();
      break;

    case CM_PEM:
      clearScreen();
      showSoftmenuCurrentPart();
      fnPem(NOPARAM);
      displayShiftAndTamBuffer();
      refreshStatusBar();
      break;

    case CM_NORMAL:
    case CM_AIM:
    case CM_NIM:
    case CM_ASSIGN:
    case CM_ERROR_MESSAGE:
    case CM_CONFIRMATION:
    case CM_LISTXY:                     //JM
    case CM_GRAPH:                      //JM
#ifdef INLINE_TEST
  if(testEnabled) { fnSwStart(0); }     //dr
#endif
      if(last_CM != calcMode) {
        if(calcMode != CM_GRAPH && calcMode != CM_LISTXY) {      //JM
          clearScreen();
        // The ordering of the 4 lines below is important for SHOW (temporaryInformation == TI_SHOW_REGISTER)
          refreshRegisterLine(REGISTER_T);
          refreshRegisterLine(REGISTER_Z);
          refreshRegisterLine(REGISTER_Y);
        }                               //JM
      }
#ifdef INLINE_TEST
  if(testEnabled) { fnSwStop(0); }      //dr
#endif
#ifdef INLINE_TEST
  if(testEnabled) { fnSwStart(1); }     //dr
#endif
      if(calcMode != CM_GRAPH && calcMode!=CM_LISTXY) {        //JM
        refreshRegisterLine(REGISTER_X);
      }                                 //JM
#ifdef INLINE_TEST
  if(testEnabled) { fnSwStop(1); }      //dr
#endif

#ifdef INLINE_TEST
  if(testEnabled) { fnSwStart(2); }     //dr
#endif
      if((last_CM != calcMode) || (doRefreshSoftMenu)) {
        last_CM = calcMode;
        doRefreshSoftMenu = false;

        displayShiftAndTamBuffer();

        if (calcMode != CM_LISTXY) showSoftmenuCurrentPart();

        hourGlassIconEnabled = false;
        refreshStatusBar();

        if(calcMode == CM_GRAPH) {     //JM v
          graph_plotmem();
        }                              //JM ^
        if(calcMode == CM_LISTXY) {     //JM v
          fnStatList();
        }                              //JM ^


      }
#ifdef INLINE_TEST
  if(testEnabled) { fnSwStop(2); }      //dr
#endif
      break;

      default: {}
    }

  #ifndef DMCP_BUILD
    refreshLcd(NULL);
  #endif // !DMCP_BUILD

#ifdef INLINE_TEST
  if(testEnabled) { fnSwStop(3); }      //dr
#endif
}
#endif // !TESTSUITE_BUILD



void fnScreenDump(uint16_t unusedButMandatoryParameter) {
  #ifdef PC_BUILD
    FILE *bmp;
    char bmpFileName[22];
    time_t rawTime;
    struct tm *timeInfo;
    int32_t x, y;
    uint32_t uint32;
    uint16_t uint16;
    uint8_t  uint8;

    time(&rawTime);
    timeInfo = localtime(&rawTime);

    strftime(bmpFileName, 22, "%Y%m%d-%H%M%S00.bmp", timeInfo);
    bmp = fopen(bmpFileName, "wb");

    fwrite("BM", 1, 2, bmp);        // Offset 0x00  0  BMP header

    uint32 = (SCREEN_WIDTH/8 * SCREEN_HEIGHT) + 610;
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x02  2  File size

    uint32 = 0;
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x06  6  unused

    uint32 = 0x00000082;
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x0a 10  Offset where the bitmap data can be found

    uint32 = 0x0000006c;
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x0e 14  Number of bytes in DIB header

    uint32 = SCREEN_WIDTH;
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x12 18  Bitmap width

    uint32 = SCREEN_HEIGHT;
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x16 22  Bitmap height

    uint16 = 0x0001;
    fwrite(&uint16, 1, 2, bmp);     // Offset 0x1a 26  Number of planes

    uint16 = 0x0001;
    fwrite(&uint16, 1, 2, bmp);     // Offset 0x1c 28  Number of bits per pixel

    uint32 = 0;
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x1e 30  Compression

    uint32 = 0x000030c0;
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x22 34  Size of bitmap data (including padding)

    uint32 = 0x00001a7c; // 6780 pixels/m
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x26 38  Horizontal print resolution

    uint32 = 0x00001a7c; // 6780 pixels/m
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x2a 42  Vertical print resolution

    uint32 = 0x00000002;
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x2e 46  Number of colors in the palette

    uint32 = 0x00000002;
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x32 50  Number of important colors

    uint32 = 0x73524742;
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x36  ???

    uint32 = 0;
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x3a  ???
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x3e  ???
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x42  ???
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x46  ???
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x4a  ???
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x4e  ???
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x52  ???
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x56  ???
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x5a  ???
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x5e  ???
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x62  ???
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x66  ???

    uint32 = 0x00000002;
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x6a  ???

    uint32 = 0;
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x6e  ???
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x72  ???
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x76  ???

    uint32 = 0x00dff5cc; // light green
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x7a  RGB color for 0

    uint32 = 0;
    fwrite(&uint32, 1, 4, bmp);     // Offset 0x7e  RGB color for 1

    // Offset 0x82  bit map data
    uint16 = 0;
    uint32 = 0;
    for(y=SCREEN_HEIGHT-1; y>=0; y--) {
      for(x=0; x<SCREEN_WIDTH; x++) {
        uint8 <<= 1;
        if(*(screenData + y*screenStride + x) == ON_PIXEL) {
          uint8 |= 1;
        }

        if((x % 8) == 7) {
          fwrite(&uint8, 1, 1, bmp);
          uint8 = 0;
        }
      }
      fwrite(&uint16, 1, 2, bmp); // Padding
    }


    fclose(bmp);
  #endif // PC_BUILD

  #ifdef DMCP_BUILD
    resetShiftState();                  //JM To avoid f or g top left of the screen, clear to make sure
    create_screenshot(0);
  #endif // DMCP_BUILD
}



#ifndef TESTSUITE_BUILD 
/********************************************//**   //JM vv
 * \brief Clears parts of the screen
 *
 * \param[in] clearStatusBar bool_t Clear the status bar
 * \param[in] clearRegLines bool_t  Clear the register lines
 * \param[in] clearSoftkeys bool_t  Clear the softkey area
 * \return void
 ***********************************************/
void clearScreen_old(bool_t clearStatusBar, bool_t clearRegisterLines, bool_t clearSoftkeys) {      //JMOLD
  if(clearStatusBar) {
    lcd_fill_rect(0, 0, SCREEN_WIDTH, 20, 0);
  }
  if(clearRegisterLines) {
    lcd_fill_rect(0, 20, SCREEN_WIDTH, 151, 0);
  }
  if(clearSoftkeys) {
    clear_ul(); //JMUL
    lcd_fill_rect(0, 171, SCREEN_WIDTH, 69, 0);
    lcd_fill_rect(0, 171-5, 20, 5, 0);
  }
}                                                       //JM ^^
#endif //TESTSUITE_BUILD
