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

//#define displayStackSHOIDISP 2                  //JMSHOIDISP  displayStackSHOIDISP=1: 3 lines of X-repeats
                                                  //JMSHOIDISP  displayStackSHOIDISP=2: 2 lines of X-repeats
                                                  //JMSHOIDISP  displayStackSHOIDISP=3: 1 lines of X-repeats
                                                  //JMSHOIDISP  displayStackSHOIDISP=4: 0 lines of X-repeats

extern bool_t   doRefreshSoftMenu;                             //dr

void     fnScreenDump                       (uint16_t unusedButMandatoryParameter);

void     FN_handler();                       //JM LONGPRESS
void     Shft_handler();                     //JM LONGPRESS f/g
void     Clx_handler();                      //JM LONGPRESS CLX
void     Shft_stop();                        //JM reset shift after  4s
void     underline(int16_t y);               //JM SHIFT LINE
void     clear_ul(void);                     //JMUL
void     clearScreen_old(bool_t clearStatusBar, bool_t clearRegisterLines, bool_t clearSoftkeys);      //JMOLD
uint8_t  last_CM;



#ifdef PC_BUILD
  gboolean drawScreen                         (GtkWidget *widget, cairo_t *cr, gpointer data);
  void     copyScreenToClipboard              (void);
  void     copyRegisterXToClipboard           (void);
  void     copyStackRegistersToClipboardString(char *clipboardString);
  void     copyStackRegistersToClipboard      (void);
  void     copyAllRegistersToClipboard        (void);
  void     copyRegisterToClipboardString      (calcRegister_t regist, char *clipboardString);
  gboolean refreshLcd                         (gpointer data);
#endif

#ifdef DMCP_BUILD
  void     copyRegisterToClipboardString      (calcRegister_t regist, char *clipboardString);                   //JMCSV Added for textfiles 
  void     refreshLcd                         (void);
#endif

#ifndef TESTSUITE_BUILD
void     refreshFn                            (uint16_t timerType);                                             //dr - general timeout handler 
uint8_t  compressString;                                                                                        //JM compressString
uint8_t  miniC;                                                              //JM miniature letters
void     underline_softkey                    (int16_t xSoftkey, int16_t ySoftKey, bool_t dontclear);           //JM LONGPRESS
void     JM_DOT                               (int16_t xx, int16_t yy);                                         //JMDOT
void     refresh_gui                          (void);
void     force_refresh                        (void);                                                  		//JM SCREEN
void     clearScreen                          (void);
void     refreshScreen                        (void);
void     setPixel                             (int16_t x, int16_t y);
void     clearPixel                           (int16_t x, int16_t y);
void     invertPixel                          (int16_t x, int16_t y);
int      getPixel                             (int16_t x, int16_t y);
int16_t  showString                           (const char *str,   const font_t *font, int16_t x, int16_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols);
int16_t  showGlyph                            (const char *ch,    const font_t *font, int16_t x, int16_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols);
int16_t  showGlyphCode                        (uint16_t charCode, const font_t *font, int16_t x, int16_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols);
void     hideCursor                           (void);
void     showFunctionName                     (int16_t item, int16_t delayInMs);
void     hideFunctionName                     (void);
void     clearRegisterLine                    (calcRegister_t regist, bool_t clearTop, bool_t clearBottom);
void     refreshRegisterLine                  (calcRegister_t regist);
#endif
