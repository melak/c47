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
 * \file statusbar.c Status bar management
 ***********************************************/

#include "wp43s.h"



#ifndef TESTSUITE_BUILD
  /********************************************//**
   * \brief Refreshes the status bar
   *
   * \param void
   * \return void
   ***********************************************/
  void refreshStatusBar(void) {
    #if (DEBUG_INSTEAD_STATUS_BAR == 1)
      sprintf(tmpString, "%s%d %s/%s  mnu:%s fi:%d", catalog ? "asm:" : "", catalog, tamMode ? "/tam" : "", getCalcModeName(calcMode),indexOfItems[-softmenu[softmenuStack[0].softmenuId].menuItem].itemCatalogName, softmenuStack[0].firstItem);
      showString(tmpString, &standardFont, X_DATE, 0, vmNormal, true, true);
    #else // DEBUG_INSTEAD_STATUS_BAR != 1
    showDateTime();
    showRealComplexResult();
    showComplexMode();
    showAngularMode();
    showFracMode();
    showIntegerMode();
    showOverflowCarry();
    showHideAlphaMode();
    showHideHourGlass();
    showHidePgmBegin();
    showHideWatch();
    showHideSerialIO();
    showHidePrinter();
    showHideUserMode();
    #ifdef DMCP_BUILD
      showHideUsbLowBattery();
    #else // !DMCP_BUILD
      showHideStackLift();
    #endif // DMCP_BUILD
    showHideASB();                            //JM
    #endif // DEBUG_INSTEAD_STATUS_BAR == 1
  }



  /********************************************//**
   * \brief Displays date and time in the status bar
   *
   * \param void
   * \return void
   ***********************************************/
  void showDateTime(void) {
    lcd_fill_rect(0, 0, X_REAL_COMPLEX, 20, LCD_SET_VALUE);

    getDateString(dateTimeString);
    uint32_t x = showString(dateTimeString, &standardFont, X_DATE, 0, vmNormal, true, true);
    x = showGlyph(getSystemFlag(FLAG_TDM24) ? STD_SPACE_FIGURE : STD_SPACE_3_PER_EM, &standardFont, x, 0, vmNormal, true, true); // is 0+0+8 pixel wide

    getTimeString(dateTimeString);
    showString(dateTimeString, &standardFont, x, 0, vmNormal, true, false);
  }



  /********************************************//**
   * \brief Displays the complex result mode C or R in the status bar
   *
   * \param void
   * \return void
   ***********************************************/
  void showRealComplexResult(void) {
    if(getSystemFlag(FLAG_CPXRES)) {
      showGlyph(STD_COMPLEX_C, &standardFont, X_REAL_COMPLEX, 0, vmNormal, true, false); // Complex C is 0+8+3 pixel wide
    }
    else {
      showGlyph(STD_REAL_R,    &standardFont, X_REAL_COMPLEX, 0, vmNormal, true, false); // Real R    is 0+8+3 pixel wide
    }
  }



  /********************************************//**
   * \brief Displays the complex mode rectangular or polar in the status bar
   *
   * \param void
   * \return void
   ***********************************************/
  void showComplexMode(void) {
    if(getSystemFlag(FLAG_POLAR)) { // polar mode
     showGlyph(STD_SUN,           &standardFont, X_COMPLEX_MODE, 0, vmNormal, true, true); // Sun         is 0+12+2 pixel wide
    }
    else { // rectangular mode
     showGlyph(STD_RIGHT_ANGLE,   &standardFont, X_COMPLEX_MODE, 0, vmNormal, true, true); // Right angle is 0+12+2 pixel wide
    }
  }



  /********************************************//**
   * \brief Displays the angular mode in the status bar
   *
   * \param void
   * \return void
   *
   ***********************************************/
  void showAngularMode(void) {
    uint32_t x = 0;

    x = showGlyph(STD_MEASURED_ANGLE, &standardFont, X_ANGULAR_MODE, 0, vmNormal, true, true); // Angle is 0+9+3 pixel wide

    switch(currentAngularMode) {
      case AM_DEGREE: showGlyph(STD_DEGREE,             &standardFont, x, 0, vmNormal, true, false); // °  is 0+6 pixel wide
                      break;

      case AM_DMS:    showGlyph(STD_RIGHT_DOUBLE_QUOTE, &standardFont, x, 0, vmNormal, true, false); // "  is 0+6 pixel wide
                      break;

      case AM_RADIAN: showGlyph(STD_SUP_r,              &standardFont, x, 0, vmNormal, true, false); // r  is 0+6 pixel wide
                      break;

      case AM_MULTPI: showGlyph(STD_pi,                     &standardFont, x, 0, vmNormal, true, false); // pi is 0+9 pixel wide
                      break;

      case AM_GRAD:   showGlyph(STD_SUP_g,              &standardFont, x, 0, vmNormal, true, false); // g  is 0+6 pixel wide
                      break;

      default:        showGlyph(STD_QUESTION_MARK, &standardFont, x, 0, vmNormal, true, false); // ?
    }
  }



/********************************************//**
 * \brief Displays the faction mode in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void showFracMode(void) {
    char str20[20];                                   //JM vv KEYS
    char str40[40];

    void conv() {
      str40[0]=0;
      int16_t x = 0;
      int16_t y = 0;
      while(str20[x]!=0) {
        if(str20[x]>='A' && str20[x]<='Z') {
          str40[y++] = 0xa4;
          str40[y++] = str20[x++] + 0x75;
          str40[y] = 0;
        } 
        else if(str20[x]>='0' && str20[x]<='9') {
          str40[y++] = 0xa0;
          str40[y++] = str20[x++] + 0x50;
          str40[y] = 0;
        } 
        else {
          str40[y++] = str20[x++];
          str40[y] = 0;
        }
      }
    }                                                  //JM ^^ KEYS

  showString(STD_SPACE_EM STD_SPACE_EM STD_SPACE_EM STD_SPACE_EM STD_SPACE_EM, &standardFont, X_INTEGER_MODE-12*5, 0, vmNormal, true, true); // STD_SPACE_EM is 0+0+12 pixel wide

  uint32_t x = 0;

  if(lastIntegerBase > 0 && lastIntegerBase <= 16) {                               //JMvv HEXKEYS
    str20[0]=0;
    
    if(lastIntegerBase>10 && lastIntegerBase<=16){
      x = showString("#KEY", &standardFont, X_FRAC_MODE, 0 , vmNormal, true, true);//-4 looks good
      strcpy(str20,"A"); conv();
      x = showString(str40,  &standardFont, x, -4 , vmNormal, true, true);         //-4 looks good
      x = showString("-",    &standardFont, x,  2 , vmNormal, true, true);         //-4 looks good
      strcpy(str20,"F"); conv();
      x = showString(str40,  &standardFont, x, -4 , vmNormal, true, true);         //-4 looks good

    } else
    if(lastIntegerBase>=2 && lastIntegerBase<=10){
      x = showString("#BASE", &standardFont, X_FRAC_MODE, 0, vmNormal, true, true); //-4 looks good
    }
    return;
  }                                                                                //JM^^


  if(getSystemFlag(FLAG_DENANY) && denMax == MAX_DENMAX) {
    showString("/max", &standardFont, X_FRAC_MODE, 0, vmNormal, true, true);
  }
  else {
    if((getSystemFlag(FLAG_DENANY) && denMax != MAX_DENMAX) || !getSystemFlag(FLAG_DENANY)) {
      sprintf(errorMessage, "/%" PRIu32, denMax);
      x = showString(errorMessage, &standardFont, X_FRAC_MODE, 0, vmNormal, true, true);
    }

      if(!getSystemFlag(FLAG_DENANY)) {
        if(getSystemFlag(FLAG_DENFIX)) {
         showGlyphCode('f',  &standardFont, x, 0, vmNormal, true, false); // f is 0+7+3 pixel wide
        }
        else {
         showString(PRODUCT_SIGN, &standardFont, x, 0, vmNormal, true, false); // STD_DOT is 0+3+2 pixel wide and STD_CROSS is 0+7+2 pixel wide
        }
      }
    }
  }



  /********************************************//**
   * \brief Displays the integer mode icon in the status bar
   *
   * \param void
   * \return void
   ***********************************************/
  void showIntegerMode(void) {
    if(shortIntegerWordSize <= 9) {
      sprintf(errorMessage, STD_SPACE_FIGURE "%" PRIu8 ":%c", shortIntegerWordSize, shortIntegerMode==SIM_1COMPL?'1':(shortIntegerMode==SIM_2COMPL?'2':(shortIntegerMode==SIM_UNSIGN?'u':(shortIntegerMode==SIM_SIGNMT?'s':'?'))));
    }
    else {
      sprintf(errorMessage, "%" PRIu8 ":%c", shortIntegerWordSize, shortIntegerMode==SIM_1COMPL?'1':(shortIntegerMode==SIM_2COMPL?'2':(shortIntegerMode==SIM_UNSIGN?'u':(shortIntegerMode==SIM_SIGNMT?'s':'?'))));
    }

    showString(errorMessage, &standardFont, X_INTEGER_MODE, 0, vmNormal, true, true);
  }



  /********************************************//**
   * \brief Displays the overflow flag and the carry flag
   *
   * \param void
   * \return void
   ***********************************************/
  void showOverflowCarry(void) {
    showGlyph(STD_OVERFLOW_CARRY, &standardFont, X_OVERFLOW_CARRY, 0, vmNormal, true, false); // STD_OVERFLOW_CARRY is 0+6+3 pixel wide

    if(!getSystemFlag(FLAG_OVERFLOW)) { // Overflow flag is cleared
      lcd_fill_rect(X_OVERFLOW_CARRY, 2, 6, 7, LCD_SET_VALUE);
    }

    if(!getSystemFlag(FLAG_CARRY)) { // Carry flag is cleared
      lcd_fill_rect(X_OVERFLOW_CARRY, 12, 6, 7, LCD_SET_VALUE);
    }
  }



  /********************************************//**
   * \brief Shows or hides the alpha mode
   *
   * \param void
   * \return void
   ***********************************************/
  void showHideAlphaMode(void) {
    int status=0;
    if(calcMode == CM_AIM || catalog) {

      if(numLock && !shiftF && !shiftG) {
          if(alphaCase == AC_UPPER)              { status = 3 - (nextChar == NC_SUBSCRIPT ? 2 : nextChar == NC_SUPERSCRIPT ? 1:0); } else
          if(alphaCase == AC_LOWER)              { status = 6 - (nextChar == NC_SUBSCRIPT ? 2 : nextChar == NC_SUPERSCRIPT ? 1:0); }
        } else

      if(alphaCase == AC_LOWER && shiftF){
        setSystemFlag(FLAG_alphaCAP);              status = 12 - (nextChar == NC_SUBSCRIPT ? 2 : nextChar == NC_SUPERSCRIPT ? 1:0); //A	
      } else
		    if(alphaCase == AC_UPPER && shiftF){
		      clearSystemFlag(FLAG_alphaCAP);          status = 18 - (nextChar == NC_SUBSCRIPT ? 2 : nextChar == NC_SUPERSCRIPT ? 1:0);   //a	
		    } else
		      if(alphaCase == AC_UPPER)  { //UPPER
		        setSystemFlag(FLAG_alphaCAP);
		        if(shiftF)                           { status = 12 - (nextChar == NC_SUBSCRIPT ? 2 : nextChar == NC_SUPERSCRIPT ? 1:0); } else
		        if(shiftG)                           { status =  9 - (nextChar == NC_SUBSCRIPT ? 2 : nextChar == NC_SUPERSCRIPT ? 1:0); } else
		        if(!shiftG && !shiftF && !numLock)   { status = 12 - (nextChar == NC_SUBSCRIPT ? 2 : nextChar == NC_SUPERSCRIPT ? 1:0); }
		      } else
		        if(alphaCase == AC_LOWER)  { //LOWER
				      clearSystemFlag(FLAG_alphaCAP);
              if(shiftF)                         { status = 18 - (nextChar == NC_SUBSCRIPT ? 2 : nextChar == NC_SUPERSCRIPT ? 1:0); } else
				      if(shiftG)                         { status = 15 - (nextChar == NC_SUBSCRIPT ? 2 : nextChar == NC_SUPERSCRIPT ? 1:0); } else
				      if(!shiftG && !shiftF && !numLock) { status = 18 - (nextChar == NC_SUBSCRIPT ? 2 : nextChar == NC_SUPERSCRIPT ? 1:0); }
				    }

      if(status >0 && status <=18) {
        showGlyphCode(' ',    &standardFont, X_ALPHA_MODE, 0, vmNormal, true, true); // is 0+0+10 pixel wide
        switch(status) {
          case  1: showString(STD_SUB_N, &standardFont, X_ALPHA_MODE, -2, vmNormal, true, false); break; //sub    // STD_ALPHA is 0+9+2 pixel wide
          case  2: showString(STD_SUB_N, &standardFont, X_ALPHA_MODE,-11, vmNormal, true, false); break; //sup   
          case  3: showString(STD_num,   &standardFont, X_ALPHA_MODE,  0, vmNormal, true, false); break; //normal
        
          case  4: showString(STD_SUB_n, &standardFont, X_ALPHA_MODE, -2, vmNormal, true, false); break; //sub   
          case  5: showString(STD_SUB_n, &standardFont, X_ALPHA_MODE,-11, vmNormal, true, false); break; //sup   
          case  6: showString(STD_n,     &standardFont, X_ALPHA_MODE,  0, vmNormal, true, false); break; //normal
          
          case  7: showString(STD_OMEGA, &standardFont, X_ALPHA_MODE,  0, vmNormal, true, false); break; //sub   
          case  8: showString(STD_OMEGA, &standardFont, X_ALPHA_MODE,  0, vmNormal, true, false); break; //sup   
          case  9: showString(STD_OMEGA, &standardFont, X_ALPHA_MODE,  0, vmNormal, true, false); break; //normal
          
          case 10: showString(STD_SUB_A, &standardFont, X_ALPHA_MODE, -2, vmNormal, true, false); break; //sub   
          case 11: showString(STD_SUB_A, &standardFont, X_ALPHA_MODE, -11, vmNormal, true, false); break; //sup   //not possible
          case 12: showString(STD_A    , &standardFont, X_ALPHA_MODE,  0, vmNormal, true, false); break; //normal
    
          case 13: showString(STD_omega, &standardFont, X_ALPHA_MODE,  0, vmNormal, true, false); break; //sub   
          case 14: showString(STD_omega, &standardFont, X_ALPHA_MODE,  0, vmNormal, true, false); break; //sup   
          case 15: showString(STD_omega, &standardFont, X_ALPHA_MODE,  0, vmNormal, true, false); break; //normal
          
          case 16: showString(STD_SUB_a, &standardFont, X_ALPHA_MODE, -2, vmNormal, true, false); break; //sub   
          case 17: showString(STD_SUB_a, &standardFont, X_ALPHA_MODE, -11, vmNormal, true, false); break; //sup    //not possible 
          case 18: showString(STD_a    , &standardFont, X_ALPHA_MODE,  0, vmNormal, true, false); break; //normal
          default:;
        }
      }
    }
    else {
      clearSystemFlag(FLAG_alphaCAP);
    }
  }


  /********************************************//**
   * \brief Shows or hides the hourglass icon in the status bar
   *
   * \param void
   * \return void
   ***********************************************/
  void showHideHourGlass(void) {
    if(hourGlassIconEnabled) {
      showGlyph(STD_HOURGLASS, &standardFont, X_HOURGLASS, 0, vmNormal, true, false); // is 0+11+3 pixel wide
    }
  }



  /********************************************//**
   * \brief Shows or hides the program begin icon in the status bar
   *
   * \param void
   * \return void
   ***********************************************/
  void showHidePgmBegin(void) {
    if(currentStep == beginOfCurrentProgram) {
      showGlyph(STD_PGM_BEGIN, &standardFont, X_PROGRAM_BEGIN, 0, vmNormal, true, false); // is 0+10+3 pixel wide
    }
  }



  /********************************************//**
   * \brief Shows or hides the watch icon in the status bar
   *
   * \param void
   * \return void
   ***********************************************/
  void showHideWatch(void) {
    if(watchIconEnabled) {
      showGlyph(STD_WATCH,                    &standardFont, X_WATCH, 0, vmNormal, true, false); // is 0+13+1 pixel wide
    }
  }



  /********************************************//**
   * \brief Shows or hides the serial I/O icon in the status bar
   *
   * \param void
   * \return void
   ***********************************************/
  void showHideSerialIO(void) {
    if(serialIOIconEnabled) {
      showGlyph(STD_SERIAL_IO, &standardFont, X_SERIAL_IO, 0, vmNormal, true, false); // is 0+8+3 pixel wide
    }
  }



  /********************************************//**
   * \brief Shows or hides the printer icon in the status bar
   *
   * \param void
   * \return void
   ***********************************************/
  void showHidePrinter(void) {
    if(printerIconEnabled) {
      showGlyph(STD_PRINTER,   &standardFont, X_PRINTER, 0, vmNormal, true, false); // is 0+12+3 pixel wide
    }
  }




void showHideASB(void) {                     //JMvv
  if(AlphaSelectionBufferTimerRunning) {
    light_ASB_icon();
  }
  else {
    kill_ASB_icon();
  }
}                                             //JM^^




/********************************************//**
 * \brief Shows or hides the user mode icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void showHideUserMode(void) {
  if(getSystemFlag(FLAG_USER)) {
    showGlyph(STD_USER_MODE, &standardFont, X_USER_MODE, 0, vmNormal, false, false); // STD_USER_MODE is 0+12+2 pixel wide
  }
  #ifndef DMCP_BUILD
    refreshModeGui(); //JM Added here to force icon update in Gui
  #endif //!TESTSUITE_BUILD
}
  


  #ifdef DMCP_BUILD
    /********************************************//**
     * \brief Shows or hides the USB or low battery icon in the status bar
     *
     * \param void
     * \return void
     ***********************************************/
    void showHideUsbLowBattery(void) {
      if(getSystemFlag(FLAG_USB)) {
        showGlyph(STD_USB, &standardFont, X_BATTERY, 0, vmNormal, true, false); // is 0+9+2 pixel wide
      }
      else {
        if(getSystemFlag(FLAG_LOWBAT)) {
          showGlyph(STD_BATTERY, &standardFont, X_BATTERY, 0, vmNormal, true, false); // is 0+10+1 pixel wide
        }
      }
    }
  #endif // DMCP_BUILD



  #ifndef DMCP_BUILD
    /********************************************//**
     * \brief Shows or hides the USB icon in the status bar
     *
     * \param void
     * \return void
     ***********************************************/
    void showHideStackLift(void) {
      if(getSystemFlag(FLAG_ASLIFT)) {
        // Draw S
        setBlackPixel(392,  1);
        setBlackPixel(393,  1);
        setBlackPixel(394,  1);
        setBlackPixel(391,  2);
        setBlackPixel(395,  2);
        setBlackPixel(391,  3);
        setBlackPixel(392,  4);
        setBlackPixel(393,  4);
        setBlackPixel(394,  4);
        setBlackPixel(395,  5);
        setBlackPixel(391,  6);
        setBlackPixel(395,  6);
        setBlackPixel(392,  7);
        setBlackPixel(393,  7);
        setBlackPixel(394,  7);

        // Draw L
        setBlackPixel(391, 10);
        setBlackPixel(391, 11);
        setBlackPixel(391, 12);
        setBlackPixel(391, 13);
        setBlackPixel(391, 14);
        setBlackPixel(391, 15);
        setBlackPixel(391, 16);
        setBlackPixel(392, 16);
        setBlackPixel(393, 16);
        setBlackPixel(394, 16);
        setBlackPixel(395, 16);
      }
      else {
        // Erase S
        setWhitePixel(392,  1);
        setWhitePixel(393,  1);
        setWhitePixel(394,  1);
        setWhitePixel(391,  2);
        setWhitePixel(395,  2);
        setWhitePixel(391,  3);
        setWhitePixel(392,  4);
        setWhitePixel(393,  4);
        setWhitePixel(394,  4);
        setWhitePixel(395,  5);
        setWhitePixel(391,  6);
        setWhitePixel(395,  6);
        setWhitePixel(392,  7);
        setWhitePixel(393,  7);
        setWhitePixel(394,  7);

        // Erase L
        setWhitePixel(391, 10);
        setWhitePixel(391, 11);
        setWhitePixel(391, 12);
        setWhitePixel(391, 13);
        setWhitePixel(391, 14);
        setWhitePixel(391, 15);
        setWhitePixel(391, 16);
        setWhitePixel(392, 16);
        setWhitePixel(393, 16);
        setWhitePixel(394, 16);
        setWhitePixel(395, 16);
      }
    }
  #endif // !DMCP_BUILD
#endif // !TESTSUITE_BUILD
