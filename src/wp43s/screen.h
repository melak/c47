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

#define SCREEN_WIDTH          400
#define SCREEN_HEIGHT         240
#define ON_PIXEL              0x303030 // blue red green
#define OFF_PIXEL             0xe0e0e0 // blue red green
#define SOFTMENU_STACK_SIZE   7 // maximum is 14 else we need to increase LENGTH_SOFTMENUSTKPTR
#define TEMPORARY_INFO_OFFSET 16
#define REGISTER_LINE_HEIGHT  37

#define Y_POSITION_OF_REGISTER_T_LINE  24 // 135 - REGISTER_LINE_HEIGHT*(registerNumber - REGISTER_X)
#define Y_POSITION_OF_REGISTER_Z_LINE  61
#define Y_POSITION_OF_REGISTER_Y_LINE  98
#define Y_POSITION_OF_REGISTER_X_LINE 135

// If one of the 4 next defines is changed: change also xxxREG in registers.h
#define Y_POSITION_OF_AIM_LINE        Y_POSITION_OF_REGISTER_X_LINE
#define Y_POSITION_OF_TAM_LINE        Y_POSITION_OF_REGISTER_T_LINE
#define Y_POSITION_OF_NIM_LINE        Y_POSITION_OF_REGISTER_X_LINE
#define Y_POSITION_OF_ERR_LINE        Y_POSITION_OF_REGISTER_Z_LINE

/********************************************//**
 * \typedef videoMode_t
 * \brief Video mode: normal video or reverse video
 ***********************************************/
typedef enum {
  vmNormal,  ///< Normal mode: black on white background
  vmReverse  ///< Reverse mode: white on black background
} videoMode_t; // 1 bit

int16_t  showFunctionNameItem;
int8_t   showFunctionNameCounter;

#ifdef PC_BUILD
gboolean drawScreen           (GtkWidget *widget, cairo_t *cr, gpointer data);
void     copyScreenToClipboard(void);
void     waitAndSee           (void);
gboolean refreshScreen        (gpointer data);
#endif

#ifndef TESTSUITE_BUILD
void     setPixel             (int16_t x, int16_t y);
void     clearPixel           (int16_t x, int16_t y);
void     invertPixel          (int16_t x, int16_t y);
int      getPixel             (int16_t x, int16_t y);
int16_t  showString           (const char *str,   const font_t *font, int16_t x, int16_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols);
int16_t  showGlyph            (const char *ch,    const font_t *font, int16_t x, int16_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols);
int16_t  showGlyphCode        (uint16_t charCode, const font_t *font, int16_t x, int16_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols);
void     clearScreen          (bool_t clearStatusBar, bool_t clearRegisterLines, bool_t clearSoftkeys);
void     showCursor           (void);
void     hideCursor           (void);
void     showFunctionName     (int16_t item, int8_t counter);
void     hideFunctionName     (void);
void     clearRegisterLine    (int16_t yStart, int16_t height);
void     refreshRegisterLine  (calcRegister_t regist);
#endif
