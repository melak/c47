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
  showHideLowBattery();
}



/********************************************//**
 * \brief Displays date and time in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void showDateTime(void) {
  int16_t x;

  showGlyph(STD_SPACE_EM, &standardFont, X_REAL_COMPLEX-12, 0, vmNormal, true, true); // Clear garbage after time STD_SPACE_EM is 0+0+12 pixel wide
  showGlyph(STD_SPACE_EM, &standardFont, X_REAL_COMPLEX-24, 0, vmNormal, true, true); // Clear garbage after time STD_SPACE_EM is 0+0+12 pixel wide

  getDateString(dateTimeString);
  x = showString(dateTimeString, &standardFont, X_DATE, 0, vmNormal, true, true);

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
  int16_t x;

  x = showGlyph(STD_MEASURED_ANGLE, &standardFont, X_ANGULAR_MODE, 0, vmNormal, true, true); // Angle is 0+9+3 pixel wide

  if(currentAngularMode == AM_DEGREE) {
    x = showGlyph(STD_DEGREE,             &standardFont, x, 0, vmNormal, true, true); // °  is 0+6+2 pixel wide
    showGlyph(STD_SPACE_HAIR,             &standardFont, x, 0, vmNormal, true, true); //    is 0+0+1 pixel wide
  }
  else if(currentAngularMode == AM_GRAD) {
    x = showGlyph(STD_SUP_g,              &standardFont, x, 0, vmNormal, true, true); // g  is 0+6+2 pixel wide
    showGlyph(STD_SPACE_HAIR,             &standardFont, x, 0, vmNormal, true, true); //    is 0+0+1 pixel wide
  }
  else if(currentAngularMode == AM_RADIAN) {
    x = showGlyph(STD_SUP_r,              &standardFont, x, 0, vmNormal, true, true); // r  is 0+6+2 pixel wide
    showGlyph(STD_SPACE_HAIR,             &standardFont, x, 0, vmNormal, true, true); //    is 0+0+1 pixel wide
  }
  else if(currentAngularMode == AM_MULTPI) {
    showGlyph(STD_pi,                     &standardFont, x, 0, vmNormal, true, true); // pi is 0+9+2 pixel wide
  }
  else if(currentAngularMode == AM_DMS) {
    x = showGlyph(STD_RIGHT_DOUBLE_QUOTE, &standardFont, x, 0, vmNormal, true, true); // "  is 0+6+2 pixel wide
    showGlyph(STD_SPACE_HAIR,             &standardFont, x, 0, vmNormal, true, true); //    is 0+0+1 pixel wide
  }
  else {
    sprintf(errorMessage, "In function showAngularMode: %" FMT8U " is an unexpected value for currentAngularMode!", currentAngularMode);
    displayBugScreen(errorMessage);
  }
}



/********************************************//**
 * \brief Displays the faction mode in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void showFracMode(void) {
  int16_t x = 0;

  showString(STD_SPACE_EM STD_SPACE_EM STD_SPACE_EM STD_SPACE_EM STD_SPACE_EM, &standardFont, X_INTEGER_MODE-12*5, 0, vmNormal, true, true); // STD_SPACE_EM is 0+0+12 pixel wide

  if(getSystemFlag(FLAG_DENANY) && denMax == MAX_DENMAX) {
    showString("/max", &standardFont, X_FRAC_MODE, 0, vmNormal, true, true);
  }
  else {
    if((getSystemFlag(FLAG_DENANY) && denMax!=MAX_DENMAX) || !getSystemFlag(FLAG_DENANY)) {
      sprintf(tmpStr3000, "/%" FMT32U, denMax);
      x = showString(tmpStr3000, &standardFont, X_FRAC_MODE, 0, vmNormal, true, true);
    }

    if(!getSystemFlag(FLAG_DENANY)) {
      if(getSystemFlag(FLAG_DENFIX)) {
       showGlyphCode('f',  &standardFont, x, 0, vmNormal, true, true); // f is 0+7+3 pixel wide
      }
      else {
       showString(PRODUCT_SIGN, &standardFont, x, 0, vmNormal, true, true); // STD_DOT is 0+3+2 pixel wide and STD_CROSS is 0+7+2 pixel wide
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
    sprintf(tmpStr3000, STD_SPACE_FIGURE "%" FMT8U ":%c", shortIntegerWordSize, shortIntegerMode==SIM_1COMPL?'1':(shortIntegerMode==SIM_2COMPL?'2':(shortIntegerMode==SIM_UNSIGN?'u':(shortIntegerMode==SIM_SIGNMT?'s':'?'))));
  }
  else {
    sprintf(tmpStr3000, "%" FMT8U ":%c", shortIntegerWordSize, shortIntegerMode==SIM_1COMPL?'1':(shortIntegerMode==SIM_2COMPL?'2':(shortIntegerMode==SIM_UNSIGN?'u':(shortIntegerMode==SIM_SIGNMT?'s':'?'))));
  }

  showString(tmpStr3000, &standardFont, X_INTEGER_MODE, 0, vmNormal, true, true);
}



/********************************************//**
 * \brief Displays the overflow flag and the carry flag
 *
 * \param void
 * \return void
 ***********************************************/
void showOverflowCarry(void) {
  int16_t x, y;

  showGlyph(STD_OVERFLOW_CARRY, &standardFont, X_OVERFLOW_CARRY, 0, vmNormal, true, false); // STD_OVERFLOW_CARRY is 0+6+3 pixel wide

  if(!getSystemFlag(FLAG_OVERFLOW)) { // Overflow flag is cleared
    for(x=X_OVERFLOW_CARRY; x<X_OVERFLOW_CARRY+6; x++) {
      for(y=2; y<=8; y++) {
        clearPixel(x, y);
      }
    }
  }

  if(!getSystemFlag(FLAG_CARRY)) { // Carry flag is cleared
    for(x=X_OVERFLOW_CARRY; x<X_OVERFLOW_CARRY+6; x++) {
      for(y=12; y<=18; y++) {
        clearPixel(x, y);
      }
    }
  }
}



/********************************************//**
 * \brief Shows or hides the alpha mode
 *
 * \param void
 * \return void
 ***********************************************/
void showHideAlphaMode(void) {
  if(calcMode == CM_AIM || calcMode == CM_ASM || calcMode == CM_ASM_OVER_TAM || calcMode == CM_ASM_OVER_AIM) {
    if(alphaCase == AC_UPPER) {
      showString(STD_ALPHA, &standardFont, X_ALPHA_MODE, 0, vmNormal, true, false); // STD_ALPHA is 0+9+2 pixel wide
      setSystemFlag(FLAG_alphaCAP);
    }
    else {
      showString(STD_alpha, &standardFont, X_ALPHA_MODE, 0, vmNormal, true, false); // STD_alpha is 0+9+2 pixel wide
      clearSystemFlag(FLAG_alphaCAP);
    }
  }
  else {
    showGlyphCode(' ',  &standardFont, X_ALPHA_MODE, 0, vmNormal, true, true); // is 0+0+10 pixel wide
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
  else {
    showGlyph(STD_SPACE_EM,  &standardFont, X_HOURGLASS, 0, vmNormal, true, true);  // STD_SPACE_EM is 12 pixel wide
  }
}



/********************************************//**
 * \brief Shows or hides the program begin icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void showHidePgmBegin(void) {
  if(programCounter == 0) {
    showGlyph(STD_PGM_BEGIN, &standardFont, X_PROGRAM_BEGIN, 0, vmNormal, true, false); // is 0+10+3 pixel wide
  }
  else {
    showString(STD_SPACE_EM, &standardFont, X_PROGRAM_BEGIN, 0, vmNormal, true, true); // STD_SPACE_EM is 12 pixel wide
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
  else {
    showString(STD_SPACE_EM STD_SPACE_HAIR, &standardFont, X_WATCH, 0, vmNormal, true, true);  // STD_SPACE_EM is 12 pixel wide
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
  else {
    showGlyphCode(' ',       &standardFont, X_SERIAL_IO, 0, vmNormal, true, true);  // is 10 pixel wide
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
  else {
    showString(STD_SPACE_EM, &standardFont, X_PRINTER, 0, vmNormal, true, true);  // STD_SPACE_EM is 12 pixel wide
  }
}



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
  else {
    showString(STD_SPACE_EM, &standardFont, X_USER_MODE, 0, vmNormal, true, true);   // STD_SPACE_EM is 12 pixel wide
  }

  #ifdef PC_BUILD
    showSoftmenuCurrentPart();                       //JM TOCHECK. Suspect in here to force icon update in Gui
    if(calcMode == CM_NORMAL) calcModeNormalGui();
    else if(calcMode == CM_AIM || calcMode == CM_ASM_OVER_AIM) calcModeAimGui();
    else if(calcMode == CM_TAM || calcMode == CM_ASM_OVER_TAM) calcModeTamGui();
  #endif
}



/********************************************//**
 * \brief Shows or hides the battery icon in the status bar
 *
 * \param void
 * \return void
 ***********************************************/
void showHideLowBattery(void) {
  if(getSystemFlag(FLAG_LOWBAT)) {
    showGlyph(STD_BATTERY, &standardFont, X_BATTERY, 0, vmNormal, true, false); // is 0+10+2 pixel wide
  }
  else {
    showGlyphCode(' ',     &standardFont, X_BATTERY, 0, vmNormal, true, true);  // is 10 pixel wide
  }
}
#endif
