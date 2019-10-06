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

#if (DEBUG_PANEL == 1)
  #define DEBUG_LINES 68
  extern GtkWidget *lbl1[DEBUG_LINES], *lbl2[DEBUG_LINES];
  extern GtkWidget *btnBitFields, *btnFlags, *btnRegisters, *btnLocalRegisters, *btnTmpAndSavedStackRegisters;
  extern GtkWidget *chkHexaString;
  extern int16_t debugWidgetDx, debugWidgetDy;

  void   btnBitFieldsClicked          (GtkWidget* w ,gpointer data);
  void   btnFlagsClicked              (GtkWidget* w ,gpointer data);
  void   btnRegistersClicked          (GtkWidget* w ,gpointer data);
  void   btnLocalRegistersClicked     (GtkWidget* w ,gpointer data);
  void   btnStatisticalSumsClicked    (GtkWidget* w ,gpointer data);
  void   btnNamedVariablesClicked     (GtkWidget* w ,gpointer data);
  void   btnSavedStackRegistersClicked(GtkWidget* w ,gpointer data);
  void   chkHexaStringClicked         (GtkWidget* w ,gpointer data);
  void   refreshDebugPanel            (void);

  char * getDenModeName                     (uint16_t dm);
  char * getDisplayFormatName               (uint16_t df);
  char * getTimeFormatName                  (uint16_t tf);
  char * getDateFormatName                  (uint16_t df);
  char * getBooleanName                     (bool_t b);
  char * getRbrModeName                     (uint16_t mode);
  char * getCurveFittingName                (uint16_t cf);
  char * getRoundingModeName                (uint16_t rm);
  char * getCalcModeName                    (uint16_t cm);
  char * getNextCharName                    (uint16_t nc);
  char * getComplexUnitName                 (uint16_t cu);
  char * getProductSignName                 (uint16_t ps);
  char * getFractionTypeName                (uint16_t ft);
  char * getRadixMarkName                   (uint16_t rm);
  char * getDisplayOvrName                  (uint16_t dio);
  char * getStackSizeName                   (uint16_t ss);
  char * getComplexModeName                 (uint16_t cm);
  char * getAlphaCaseName                   (uint16_t ac);
  char * getCursorFontName                  (uint16_t cf);
  void   memoryDump                         (bool_t bitFields, bool_t globalFlags, bool_t globalRegisters, bool_t localFlags, bool_t FIRSTLOCALREGISTERs, bool_t otherVars);
#endif

#if (DEBUG_PANEL == 1) || (DEBUG_REGISTER_L == 1)
  void   formatReal16Debug                  (char *str, void *addr);
  void   formatComplex16Debug               (char *str, void *addr);
  void   formatReal34Debug                  (char *str, void *addr);
  void   formatComplex34Debug               (char *str, void *addr);
#endif

char *getDataTypeName                     (uint16_t dt, bool_t article, bool_t padWithBlanks);
char *getRegisterDataTypeName             (calcRegister_t regist, bool_t article, bool_t padWithBlanks);
char *getRegisterTagName                  (calcRegister_t regist, bool_t padWithBlanks);
char *getShortIntegerModeName             (uint16_t im);
char *getAngularModeName                  (uint16_t angularMode);
void  debugNIM                            (void);

#ifdef PC_BUILD
  void dumpScreenToConsole(void);
#endif

#if defined(PC_BUILD )|| defined(TESTSUITE_BUILD)
  void testRegisters  (const char *text);
  void memoryDump2    (const char *text);
#endif
