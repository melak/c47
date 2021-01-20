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

//JMvv
  extern bool_t   doRefreshSoftMenu;                                                                              //dr  
  void     FN_handler();                                                                                          //JM LONGPRESS
  void     Shft_handler();                                                                                        //JM LONGPRESS f/g
  void     LongpressKey_handler();                                                                                //JM LONGPRESS CLX
  void     Shft_stop();                                                                                           //JM reset shift after  4s
  void     underline(int16_t y);                                                                                  //JM SHIFT LINE
  void     clear_ul(void);                                                                                        //JMUL
  void     clearScreen_old(bool_t clearStatusBar, bool_t clearRegisterLines, bool_t clearSoftkeys);               //JMOLD
  uint8_t  last_CM;  //Do extern !!
//JM^^

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
  void     copyRegisterToClipboardString      (calcRegister_t regist, char *clipboardString);                   //JMCSV Added for textfiles 
  void     refreshLcd                         (void);
#else // !DMCP_BUILD
  void     lcd_fill_rect                      (uint32_t x, uint32_t y, uint32_t dx, uint32_t 	dy, int val); // clone fron the DMCP function
  void     setBlackPixel                      (uint32_t x, uint32_t y);
  void     setWhitePixel                      (uint32_t x, uint32_t y);
  void     invert_Pixel                       (uint32_t x, uint32_t y);                                          //JM
#endif // DMCP_BUILD

#ifndef TESTSUITE_BUILD
//JM vv
  void     refreshFn                            (uint16_t timerType);                                           //dr - general timeout handler 
//  uint8_t  combinationFonts;    //TO REMOVE from .h
//  uint8_t  maxiC;               //TO REMOVE from .h                                                                                            //JM global flags for character control:  enlarged letters
  extern uint8_t  compressString;                                                                               //JM global flags for character control: compressString
  extern uint8_t  lines;
  extern uint16_t current_cursor_x;
  extern uint16_t current_cursor_y;
  extern int16_t  displayAIMbufferoffset;


  //Stack string large font display
  #define STACK_X_STR_LRG_FONT
  #define STACK_STR_MED_FONT
  #define STACK_X_STR_MED_FONT
  #undef  STACK_X_STR_MED_FONT //not needed as the full and half fonts are the same width

  //mode
  #define stdNoEnlarge     0                                                                                    //JM vv compress, enlarge, small fonts
  #define stdEnlarge       1
  #define stdnumEnlarge    2
  #define numSmall         3
  #define numHalf          4

  //comp
  #define nocompress       0
  #define stdcompress      1

  uint32_t showStringC43                      (const char *string, int mode, int comp, uint32_t x, uint32_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols );
  uint32_t stringWidthC43                     (const char *str,    int mode, int comp,                                                bool_t withLeadingEmptyRows, bool_t withEndingEmptyRows);
  uint32_t showStringEdC43                    (uint32_t lastline, int16_t offset, int16_t edcursor, const char *string, uint32_t x, uint32_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols, bool_t noshow);
                                                                                                               //JM ^^
  void     underline_softkey                  (int16_t xSoftkey, int16_t ySoftKey, bool_t dontclear);          //JM LONGPRESS
  void     refresh_gui                        (void);                                                          //JM
  void     force_refresh                      (void);                                                          //JM SCREEN
//JM^^

void     refreshScreen                        (void);
  //int      getPixel                           (uint32_t x, uint32_t y);
  uint32_t showString                         (const char *str,   const font_t *font, uint32_t x, uint32_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols);
  uint32_t showGlyph                          (const char *ch,    const font_t *font, uint32_t x, uint32_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols);
  uint32_t showGlyphCode                      (uint16_t charCode, const font_t *font, uint32_t x, uint32_t y, videoMode_t videoMode, bool_t showLeadingCols, bool_t showEndingCols);
  void     hideCursor                         (void);
  void     showFunctionName                   (int16_t item, int16_t delayInMs);
  void     hideFunctionName                   (void);
  void     clearRegisterLine                  (calcRegister_t regist, bool_t clearTop, bool_t clearBottom);
  void     refreshRegisterLine                (calcRegister_t regist);
#endif // TESTSUITE_BUILD
