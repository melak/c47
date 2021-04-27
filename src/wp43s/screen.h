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
 * \file screen.h
 ***********************************************/
#ifndef SCREEN_H
#define SCREEN_H

#include "typeDefinitions.h"
#include <stdint.h>
#ifdef PC_BUILD
  #include <gtk/gtk.h>
  #include <gdk/gdk.h>
#endif // PC_BUILD

void       fnScreenDump                       (uint16_t unusedButMandatoryParameter);

#ifdef PC_BUILD
  gboolean drawScreen                         (GtkWidget *widget, cairo_t *cr, gpointer data);
  void     copyScreenToClipboard              (void);
  void     copyRegisterXToClipboard           (void);
  void     copyStackRegistersToClipboardString(char *clipboardString);
  void     copyStackRegistersToClipboard      (void);
  void     copyAllRegistersToClipboard        (void);
  void     copyRegisterToClipboardString      (calcRegister_t regist, char *clipboardString);
  gboolean refreshLcd                         (gpointer unusedData);
#endif // PC_BUILD

#ifdef DMCP_BUILD
  void     refreshLcd                         (void);
#else // !DMCP_BUILD
  void     lcd_fill_rect                      (uint32_t x, uint32_t y, uint32_t dx, uint32_t dy, int val); // clone fron the DMCP function
  void     setBlackPixel                      (uint32_t x, uint32_t y);
  void     setWhitePixel                      (uint32_t x, uint32_t y);
#endif // DMCP_BUILD

#ifndef TESTSUITE_BUILD
  void     refreshScreen                      (void);
  //void     invertPixel                        (uint32_t x, uint32_t y);
  //int      getPixel                           (uint32_t x, uint32_t y);
  uint32_t showString                         (const char *str,   const font_t *font, uint32_t x, uint32_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols);
  uint32_t showGlyph                          (const char *ch,    const font_t *font, uint32_t x, uint32_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols);
  uint32_t showGlyphCode                      (uint16_t charCode, const font_t *font, uint32_t x, uint32_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols);
  void     hideCursor                         (void);
  void     showFunctionName                   (int16_t item, int16_t delayInMs);
  void     hideFunctionName                   (void);
  void     clearRegisterLine                  (calcRegister_t regist, bool_t clearTop, bool_t clearBottom);
  void     refreshRegisterLine                (calcRegister_t regist);
  void     displayNim                         (const char *nim, const char *lastBase, int16_t wLastBaseNumeric, int16_t wLastBaseStandard);
#endif // TESTSUITE_BUILD

#endif // SCREEN_H
