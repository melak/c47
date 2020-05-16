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
 * \file fontBrowser.c
 ***********************************************/

#include "wp43s.h"



#ifndef TESTSUITE_BUILD
/********************************************//**
 * \brief The font browser application
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void fontBrowser(uint16_t unusedParamButMandatory) {
  uint16_t x, y, g, first;

  if(calcMode != CM_FONT_BROWSER) {
    previousCalcMode = calcMode;
    calcMode = CM_FONT_BROWSER;
    clearSystemFlag(FLAG_ALPHA);
  }

  if(currentFntScr == 0) { // Init
    currentFntScr = 1;

    numLinesNumericFont  = 0;
    for(g=0; g<numericFont.numberOfGlyphs;) {
      row[numLinesNumericFont] = numericFont.glyphs[g].charCode & 0xfff0;
      while(g<numericFont.numberOfGlyphs && ((numericFont.glyphs[g].charCode&0xfff0) == row[numLinesNumericFont])) {
        g++;
      }
      numLinesNumericFont++;
    }

    numScreensNumericFont = numLinesNumericFont / NUMBER_OF_NUMERIC_FONT_LINES_PER_SCREEN;
    if(numLinesNumericFont % NUMBER_OF_NUMERIC_FONT_LINES_PER_SCREEN != 0) {
      numScreensNumericFont++;
    }

    numLinesStandardFont  = 0;
    for(g=0; g<standardFont.numberOfGlyphs;) {
      row[numLinesNumericFont + numLinesStandardFont] = standardFont.glyphs[g].charCode & 0xfff0;
      while(g<standardFont.numberOfGlyphs && ((standardFont.glyphs[g].charCode&0xfff0) == row[numLinesNumericFont + numLinesStandardFont])) {
        g++;
      }
      numLinesStandardFont++;
    }

    numScreensStandardFont = numLinesStandardFont / NUMBER_OF_STANDARD_FONT_LINES_PER_SCREEN;
    if(numLinesStandardFont % NUMBER_OF_STANDARD_FONT_LINES_PER_SCREEN != 0) {
      numScreensStandardFont++;
    }
  }

  if(currentFntScr>=1 && currentFntScr<=numScreensNumericFont) { // Numeric font
    clearScreen(false, true, true);

    for(x=0; x<=9; x++) {
      showGlyphCode('0'+x, &standardFont, 50+20*x,     20, vmNormal, false, false);
    }
    for(x=0; x<=5; x++) {
      showGlyphCode('A'+x, &standardFont, 50+200+20*x, 20, vmNormal, false, false);
    }

    first = (currentFntScr-1) * NUMBER_OF_NUMERIC_FONT_LINES_PER_SCREEN;
    for(y=first; y<min(currentFntScr * NUMBER_OF_NUMERIC_FONT_LINES_PER_SCREEN, numLinesNumericFont); y++) {
      sprintf(tmpStr3000, "%04X", row[y]<0x8000 ? row[y] : row[y]-0x8000);
      showString(tmpStr3000, &standardFont, 5, NUMERIC_FONT_HEIGHT*(y-first)+43, vmNormal, false, false);
      for(x=0; x<=15; x++) {
        showGlyphCode(row[y]+x, &numericFont, 46+20*x, NUMERIC_FONT_HEIGHT*(y-first)+40, vmNormal, false, false);
      }
    }

    if(currentFntScr == 1) {
      showString("Numeric font. Press " STD_DOWN_ARROW " or EXIT", &standardFont, 5, 220, vmNormal, false, false);
    }
    else {
      showString("Numeric font. Press " STD_UP_ARROW ", " STD_DOWN_ARROW " or EXIT", &standardFont, 5, 220, vmNormal, false, false);
    }

    sprintf(tmpStr3000, "%d/%d", currentFntScr, numScreensNumericFont+numScreensStandardFont);
    showString(tmpStr3000, &standardFont, SCREEN_WIDTH - stringWidth(tmpStr3000, &standardFont, false, true), 220, vmNormal, false, true);
  }

  else if(currentFntScr>numScreensNumericFont && currentFntScr<=numScreensNumericFont+numScreensStandardFont) { // Standard font
    clearScreen(false, true, true);

    for(x=0; x<=9; x++) {
      showGlyphCode('0'+x, &standardFont, 50+20*x,     20, vmNormal, false, false);
    }
    for(x=0; x<=5; x++) {
      showGlyphCode('A'+x, &standardFont, 50+200+20*x, 20, vmNormal, false, false);
    }

    first = numLinesNumericFont + (currentFntScr-numScreensNumericFont-1) * NUMBER_OF_STANDARD_FONT_LINES_PER_SCREEN;
    for(y=first; y<min(numLinesNumericFont + (currentFntScr-numScreensNumericFont) * NUMBER_OF_STANDARD_FONT_LINES_PER_SCREEN, numLinesNumericFont+numLinesStandardFont); y++) {
      sprintf(tmpStr3000, "%04X", row[y]<0x8000 ? row[y] : row[y]-0x8000);
      showString(tmpStr3000, &standardFont, 5, STANDARD_FONT_HEIGHT*(y-first)+40, vmNormal, false, false);
      for(x=0; x<=15; x++) {
        showGlyphCode(row[y]+x, &standardFont, 50+20*x, STANDARD_FONT_HEIGHT*(y-first)+40, vmNormal, false, false);
      }
    }

    if(currentFntScr == numScreensNumericFont+numScreensStandardFont) {
      showString("Standard font. Press " STD_UP_ARROW " or EXIT", &standardFont, 5, 220, vmNormal, false, false);
    }
    else {
      showString("Standard font. Press " STD_UP_ARROW ", " STD_DOWN_ARROW " or EXIT", &standardFont, 5, 220, vmNormal, false, false);
    }

    sprintf(tmpStr3000, "%d/%d", currentFntScr, numScreensNumericFont+numScreensStandardFont);
    showString(tmpStr3000, &standardFont, SCREEN_WIDTH-stringWidth(tmpStr3000, &standardFont, false, true), 220, vmNormal, false, true);
  }
  else {
    clearScreen(false, true, true);
    displayBugScreen("In function showFonts: This should never happen!");
  }
}
#endif
