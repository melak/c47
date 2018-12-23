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
 * \file gui.c
 ***********************************************/

#include "wp43s.h"

#ifdef PC_BUILD
GtkWidget *frmCalc;
GtkWidget *grid;
GtkWidget *backgroundImage;
GtkWidget *lblFSoftkeyArea, *lblGSoftkeyArea; //, *lblSoftkeyArea1, *lblSoftkeyArea2;
GtkWidget *lblBehindScreen, *screen;

GtkWidget *btn11,   *btn12,   *btn13,   *btn14,   *btn15,   *btn16;

GtkWidget *btn21,   *btn22,   *btn23,   *btn24,   *btn25,   *btn26;
GtkWidget *lbl21F,  *lbl22F,  *lbl23F,  *lbl24F,  *lbl25F,  *lbl26F;
GtkWidget *lbl21G,  *lbl22G,  *lbl23G,  *lbl24G,  *lbl25G,  *lbl26G;
GtkWidget *lbl21L,  *lbl22L,  *lbl23L,  *lbl24L,  *lbl25L,  *lbl26L;
GtkWidget *lbl21Gr, *lbl22Gr, *lbl23Gr, *lbl24Gr, *lbl25Gr, *lbl26Gr;

GtkWidget *btn31,   *btn32,   *btn33,   *btn34,   *btn35,   *btn36;
GtkWidget *lbl31F,  *lbl32F,  *lbl33F,  *lbl34F,  *lbl35F,  *lbl36F;
GtkWidget *lbl31G,  *lbl32G,  *lbl33G,  *lbl34G,  *lbl35G,  *lbl36G;
GtkWidget *lbl31L,  *lbl32L,  *lbl33L,  *lbl34L,  *lbl35L,  *lbl36L;
GtkWidget                     *lbl33H;
GtkWidget *lbl31Gr, *lbl32Gr, *lbl33Gr, *lbl34Gr, *lbl35Gr, *lbl36Gr;

GtkWidget *btn41,   *btn42,   *btn43,   *btn44,   *btn45;
GtkWidget *lbl41F,  *lbl42F,  *lbl43F,  *lbl44F,  *lbl45F;
GtkWidget *lbl41G,  *lbl42G,  *lbl43G,  *lbl44G,  *lbl45G;
GtkWidget *lbl41L,  *lbl42L,  *lbl43L,  *lbl44L,  *lbl45L;
GtkWidget           *lbl42H,  *lbl43P,  *lbl44P;
GtkWidget *lbl41Gr, *lbl42Gr, *lbl43Gr, *lbl44Gr, *lbl45Gr;

GtkWidget *btn51,   *btn52,   *btn53,   *btn54,   *btn55;
GtkWidget *lbl51F,  *lbl52F,  *lbl53F,  *lbl54F,  *lbl55F;
GtkWidget *lbl51G,  *lbl52G,  *lbl53G,  *lbl54G,  *lbl55G;
GtkWidget *lbl51L,  *lbl52L,  *lbl53L,  *lbl54L,  *lbl55L;
GtkWidget *lbl51Gr, *lbl52Gr, *lbl53Gr, *lbl54Gr, *lbl55Gr;

GtkWidget *btn61,   *btn62,   *btn63,   *btn64,   *btn65;
GtkWidget *lbl61F,  *lbl62F,  *lbl63F,  *lbl64F,  *lbl65F;
GtkWidget *lbl61G,  *lbl62G,  *lbl63G,  *lbl64G,  *lbl65G;
GtkWidget *lbl61L,  *lbl62L,  *lbl63L,  *lbl64L,  *lbl65L;
GtkWidget                               *lbl64H;
GtkWidget *lbl61Gr, *lbl62Gr, *lbl63Gr, *lbl64Gr, *lbl65Gr;

GtkWidget *btn71,   *btn72,   *btn73,   *btn74,   *btn75;
GtkWidget *lbl71F,  *lbl72F,  *lbl73F,  *lbl74F,  *lbl75F;
GtkWidget *lbl71G,  *lbl72G,  *lbl73G,  *lbl74G,  *lbl75G;
GtkWidget *lbl71L,  *lbl72L,  *lbl73L,  *lbl74L,  *lbl75L;
GtkWidget *lbl71H,  *lbl72H;
GtkWidget *lbl71Gr, *lbl72Gr, *lbl73Gr, *lbl74Gr, *lbl75Gr;

GtkWidget *btn81,   *btn82,   *btn83,   *btn84,   *btn85;
GtkWidget *lbl81F,  *lbl82F,  *lbl83F,  *lbl84F,  *lbl85F;
GtkWidget *lbl81G,  *lbl82G,  *lbl83G,  *lbl84G,  *lbl85G;
GtkWidget *lbl81L,  *lbl82L,  *lbl83L,  *lbl84L,  *lbl85L;
GtkWidget *lbl81H,            *lbl83H,  *lbl84H;
GtkWidget *lbl81Gr, *lbl82Gr, *lbl83Gr, *lbl84Gr, *lbl85Gr;
GtkWidget                                         *lblOn;

#if (DEBUG_PANEL == 1)
GtkWidget *lbl1[DEBUG_LINES], *lbl2[DEBUG_LINES];
GtkWidget *btnBitFields, *btnFlags, *btnRegisters, *btnLocalRegisters, *btnStatisticalSums, *btnNamedRegisters, *btnTmpAndSavedStackRegisters;
GtkWidget *chkHexaString;
int16_t debugWidgetDx, debugWidgetDy;
#endif

char      *cssData;



static gint destroyCalc(GtkWidget* w, GdkEventAny* e, gpointer data) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("destroyCalc");
  #endif

  saveCalc();
  gtk_main_quit();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("destroyCalc");
  #endif

  return 0;
}



gboolean keyPressed(GtkWidget *w, GdkEventKey *event, gpointer data) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("nakeyPressedme");
  #endif

  //printf("%d\n", event->keyval);
  switch (event->keyval) {
    case 65470:
      //printf("key pressed: F1\n");
      btnFnClicked(w, "1");
      break;

    case 65471:
      //printf("key pressed: F2\n");
      btnFnClicked(w, "2");
      break;

    case 65472:
      //printf("key pressed: F3\n");
      btnFnClicked(w, "3");
      break;

    case 65473:
     //printf("key pressed: F4\n");
     btnFnClicked(w, "4");
     break;

    case 65474:
      //printf("key pressed: F5\n");
      btnFnClicked(w, "5");
      break;

    case 65475:
      //printf("key pressed: F6\n");
      btnFnClicked(w, "6");
      break;

    case 73:
    case 105:
      //printf("key pressed: i 1/x\n");
      btnClicked(w, "00");
      break;

    case 89:
    case 121:
      //printf("key pressed: y y^x\n");
      btnClicked(w, "01");
      break;

    case 84:
    case 116:
      //printf("key pressed: T TRI\n");
      btnClicked(w, "02");
      break;

    case 76:
    case 108:
      //printf("key pressed: l ln\n");
      btnClicked(w, "03");
      break;

    case 101:
      //printf("key pressed: e e^x\n");
      btnClicked(w, "04");
      break;

    case 81:
    case 113:
      //printf("key pressed: Q Quadradwurzel\n");
      btnClicked(w, "05");
      break;

    case 83:
    case 115:
      //printf("key pressed: STO\n");
      btnClicked(w, "06");
      break;

    case 82:
    case 114:
      //printf("key pressed: RCL\n");
      btnClicked(w, "07");
      break;

    case 65366:
      //printf("key pressed: PgDn Roll down\n");
      btnClicked(w, "08");
      break;

    case 85:
    case 117:
      //printf("key pressed: UM\n");
      btnClicked(w, "09");
      break;

    case 70:
    case 102:
      //printf("key pressed: f\n");
      btnClicked(w, "10");
      break;

    case 71:
    case 103:
      //printf("key pressed: g\n");
      btnClicked(w, "11");
      break;

    case 65421:
    case 65293:
      //printf("key pressed: ENTER\n");
      btnClicked(w, "12");
      break;

    case 65289:
      //printf("key pressed: TAB x<>y\n");
      btnClicked(w, "13");
      break;

    case 99:
    case 67:
      //printf("key pressed: C CHS +/-\n");
      btnClicked(w, "14");
      break;

    case 69:
      //printf("key pressed: E\n");
      btnClicked(w, "15");
      break;

    case 65288:
      //printf("key pressed: Backspace\n");
      btnClicked(w, "16");
      break;

    case 47:
    case 65455:
      //printf("key pressed: /\n");
      btnClicked(w, "17");
      break;

    case 55:
    case 65463:
      //printf("key pressed: 7\n");
      btnClicked(w, "18");
      break;

    case 56:
    case 65464:
      //printf("key pressed: 8\n");
      btnClicked(w, "19");
      break;

    case 57:
    case 65465:
      //printf("key pressed: 9\n");
      btnClicked(w, "20");
      break;

    case 88:
    case 120:
      //printf("key pressed: XEQ\n");
      btnClicked(w, "21");
      break;

    case 42:
    case 65450:
      //printf("key pressed: *\n");
      btnClicked(w, "22");
      break;

    case 52:
    case 65460:
      //printf("key pressed: 4\n");
      btnClicked(w, "23");
      break;

    case 53:
    case 65461:
      //printf("key pressed: 5\n");
      btnClicked(w, "24");
      break;

    case 54:
    case 65462:
      //printf("key pressed: 6\n");
      btnClicked(w, "25");
      break;

    case 65362:
      //printf("key pressed: up\n");
      btnClicked(w, "26");
      break;

    case 45:
    case 65453:
      //printf("key pressed: -\n");
      btnClicked(w, "27");
      break;

    case 49:
    case 65457:
      //printf("key pressed: 1\n");
      btnClicked(w, "28");
      break;

    case 50:
    case 65458:
      //printf("key pressed: 2\n");
      btnClicked(w, "29");
      break;

    case 51:
    case 65459:
      //printf("key pressed: 3\n");
      btnClicked(w, "30");
      break;

    case 65364:
      //printf("key pressed: down\n");
      btnClicked(w, "31");
      break;

    case 43:
    case 65451:
      //printf("key pressed: +\n");
      btnClicked(w, "32");
      break;

    case 48:
    case 65456:
      //printf("key pressed: 0\n");
      btnClicked(w, "33");
      break;

    case 44:
    case 46:
    case 65454:
      //printf("key pressed: .\n");
      btnClicked(w, "34");
      break;

    case 65508:
    case 65507:
      //printf("key pressed: Ctrl R/S\n");
      btnClicked(w, "35");
      break;

    case 65307: // ESC
      //printf("key pressed: EXIT\n");
      btnClicked(w, "36");
      break;

    case 104: // H
      //printf("key pressed: \n");
      copyScreenToClipboard();
      break;

    default:
      break;
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("keyPressed");
  #endif

  return FALSE;
}



void strReplace(char *haystack, const char *needle, const char *newNeedle) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("strReplace");
  #endif

  ////////////////////////////////////////////////////////
  // There MUST be enough memory allocated to *haystack //
  // when strlen(newNeedle) > strlen(needle)            //
  ////////////////////////////////////////////////////////
  char *tmpString, *needleLocation;
  int  needleLg;

  while(strstr(haystack, needle) != NULL) {
    needleLg = strlen(needle);
    needleLocation = strstr(haystack, needle);
    tmpString = malloc(strlen(needleLocation + needleLg) + 1);
    if(tmpString == NULL) {
      showInfoDialog("In function strReplace:", "error allocating memory for tmpString!", NULL, NULL);
      exit(1);
    }

    strcpy(tmpString, needleLocation + needleLg);
    *strstr(haystack, needle) = 0;
    strcat(haystack, newNeedle);
    strcat(haystack, tmpString);
    free(tmpString);
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("strReplace");
  #endif
}



/********************************************//**
 * \brief Reads the CSS file to configure the calc's GUI style
 *
 * \param void
 * \return void
 ***********************************************/
void prepareCssData(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("prepareCssData");
  #endif

  FILE *cssFile;
  char *toReplace, *replaceWith, needle[100], newNeedle[100];
  int  i, fileLg;

  #define CSSFILE "wp43s_pre.css"

  // Convert the pre-CSS data to CSS data
  cssFile = fopen(CSSFILE, "rb");
  if(cssFile == NULL) {
    showInfoDialog("In function prepareCssData:", "error opening file " CSSFILE "!", NULL, NULL);
    exit(1);
  }

  // Get the file length
  fseek(cssFile, 0L, SEEK_END);
  fileLg = ftell(cssFile);
  fseek(cssFile, 0L, SEEK_SET);

  cssData = malloc(2*fileLg); // To be sure there is enough space
  if(cssData == NULL) {
    showInfoDialog("In function prepareCssData:", "error allocating 10000 bytes for CSS data", NULL, NULL);
    exit(1);
  }

  fread(cssData, 1, fileLg, cssFile);
  fclose(cssFile);
  cssData[fileLg] = 0;

  toReplace = strstr(cssData, "/* Replace $");
  while(toReplace != NULL) {
    toReplace += 11;
    i = -1;
    while(toReplace[++i] != ' ') {
      needle[i] = toReplace[i];
    }
    needle[i] = 0;

    *toReplace = ' ';

    replaceWith = strstr(toReplace, " with ");
    if(replaceWith == NULL) {
      showInfoDialog("In function prepareCssData:", "Can't find \" with \" after \"/* Replace $\" in CSS file " CSSFILE, NULL, NULL);
      exit(1);
    }

    replaceWith[1] = ' ';
    replaceWith += 6;
    i = -1;
    while(replaceWith[++i] != ' ') {
      newNeedle[i] = replaceWith[i];
    }
    newNeedle[i] = 0;

    strReplace(toReplace, needle, newNeedle);

    toReplace = strstr(cssData, "/* Replace $");
  }

  if(strstr(cssData, "$") != NULL) {
    showInfoDialog("In function prepareCssData:", "There is still an unreplaced $ in the CSS file!\nPlease check file " CSSFILE, NULL, NULL);
    printf("%s\n", cssData);
    exit(1);
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("prepareCssData");
  #endif
}



/********************************************//**
 * \brief Hides all the widgets on the calc GUI
 *
 * \param void
 * \return void
 ***********************************************/
void hideAllWidgets(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("hideAllWidgets");
  #endif

  gtk_widget_hide(btn11);
  gtk_widget_hide(btn12);
  gtk_widget_hide(btn13);
  gtk_widget_hide(btn14);
  gtk_widget_hide(btn15);
  gtk_widget_hide(btn16);

  gtk_widget_hide(btn21);
  gtk_widget_hide(btn22);
  gtk_widget_hide(btn23);
  gtk_widget_hide(btn24);
  gtk_widget_hide(btn25);
  gtk_widget_hide(btn26);

  gtk_widget_hide(lbl21F);
  gtk_widget_hide(lbl21G);
  gtk_widget_hide(lbl21L);
  gtk_widget_hide(lbl22F);
  gtk_widget_hide(lbl22G);
  gtk_widget_hide(lbl22L);
  gtk_widget_hide(lbl23F);
  gtk_widget_hide(lbl23G);
  gtk_widget_hide(lbl23L);
  gtk_widget_hide(lbl24F);
  gtk_widget_hide(lbl24G);
  gtk_widget_hide(lbl24L);
  gtk_widget_hide(lbl25F);
  gtk_widget_hide(lbl25G);
  gtk_widget_hide(lbl25L);
  gtk_widget_hide(lbl26F);
  gtk_widget_hide(lbl26G);
  gtk_widget_hide(lbl26L);
  gtk_widget_hide(lbl21Gr);
  gtk_widget_hide(lbl22Gr);
  gtk_widget_hide(lbl23Gr);
  gtk_widget_hide(lbl24Gr);
  gtk_widget_hide(lbl25Gr);
  gtk_widget_hide(lbl26Gr);

  gtk_widget_hide(btn31);
  gtk_widget_hide(btn32);
  gtk_widget_hide(btn33);
  gtk_widget_hide(btn34);
  gtk_widget_hide(btn35);
  gtk_widget_hide(btn36);

  gtk_widget_hide(lbl31F);
  gtk_widget_hide(lbl31G);
  gtk_widget_hide(lbl31L);
  gtk_widget_hide(lbl32F);
  gtk_widget_hide(lbl32G);
  gtk_widget_hide(lbl32L);
  gtk_widget_hide(lbl33F);
  gtk_widget_hide(lbl33G);
  gtk_widget_hide(lbl33H);
  gtk_widget_hide(lbl33L);
  gtk_widget_hide(lbl34F);
  gtk_widget_hide(lbl34G);
  gtk_widget_hide(lbl34L);
  gtk_widget_hide(lbl35F);
  gtk_widget_hide(lbl35G);
  gtk_widget_hide(lbl35L);
  gtk_widget_hide(lbl36F);
  gtk_widget_hide(lbl36G);
  gtk_widget_hide(lbl36L);
  gtk_widget_hide(lbl31Gr);
  gtk_widget_hide(lbl32Gr);
  gtk_widget_hide(lbl33Gr);
  gtk_widget_hide(lbl34Gr);
  gtk_widget_hide(lbl35Gr);
  gtk_widget_hide(lbl36Gr);

  gtk_widget_hide(btn41);
  gtk_widget_hide(btn42);
  gtk_widget_hide(btn43);
  gtk_widget_hide(btn44);
  gtk_widget_hide(btn45);

  gtk_widget_hide(lbl41F);
  gtk_widget_hide(lbl41G);
  gtk_widget_hide(lbl41L);
  gtk_widget_hide(lbl42F);
  gtk_widget_hide(lbl42G);
  gtk_widget_hide(lbl42H);
  gtk_widget_hide(lbl42L);
  gtk_widget_hide(lbl43F);
  gtk_widget_hide(lbl43G);
  gtk_widget_hide(lbl43L);
  gtk_widget_hide(lbl43P);
  gtk_widget_hide(lbl44F);
  gtk_widget_hide(lbl44G);
  gtk_widget_hide(lbl44L);
  gtk_widget_hide(lbl44P);
  gtk_widget_hide(lbl45F);
  gtk_widget_hide(lbl45G);
  gtk_widget_hide(lbl45L);
  gtk_widget_hide(lbl41Gr);
  gtk_widget_hide(lbl42Gr);
  gtk_widget_hide(lbl43Gr);
  gtk_widget_hide(lbl44Gr);
  gtk_widget_hide(lbl45Gr);

  gtk_widget_hide(btn51);
  gtk_widget_hide(btn52);
  gtk_widget_hide(btn53);
  gtk_widget_hide(btn54);
  gtk_widget_hide(btn55);

  gtk_widget_hide(lbl51F);
  gtk_widget_hide(lbl51G);
  gtk_widget_hide(lbl51L);
  gtk_widget_hide(lbl52F);
  gtk_widget_hide(lbl52G);
  gtk_widget_hide(lbl52L);
  gtk_widget_hide(lbl53F);
  gtk_widget_hide(lbl53G);
  gtk_widget_hide(lbl53L);
  gtk_widget_hide(lbl54F);
  gtk_widget_hide(lbl54G);
  gtk_widget_hide(lbl54L);
  gtk_widget_hide(lbl55F);
  gtk_widget_hide(lbl55G);
  gtk_widget_hide(lbl55L);
  gtk_widget_hide(lbl51Gr);
  gtk_widget_hide(lbl52Gr);
  gtk_widget_hide(lbl53Gr);
  gtk_widget_hide(lbl54Gr);
  gtk_widget_hide(lbl55Gr);

  gtk_widget_hide(btn61);
  gtk_widget_hide(btn62);
  gtk_widget_hide(btn63);
  gtk_widget_hide(btn64);
  gtk_widget_hide(btn65);

  gtk_widget_hide(lbl61F);
  gtk_widget_hide(lbl61G);
  gtk_widget_hide(lbl61L);
  gtk_widget_hide(lbl62F);
  gtk_widget_hide(lbl62G);
  gtk_widget_hide(lbl62L);
  gtk_widget_hide(lbl63F);
  gtk_widget_hide(lbl63G);
  gtk_widget_hide(lbl63L);
  gtk_widget_hide(lbl64F);
  gtk_widget_hide(lbl64G);
  gtk_widget_hide(lbl64L);
  gtk_widget_hide(lbl64H);
  gtk_widget_hide(lbl65F);
  gtk_widget_hide(lbl65G);
  gtk_widget_hide(lbl65L);
  gtk_widget_hide(lbl61Gr);
  gtk_widget_hide(lbl62Gr);
  gtk_widget_hide(lbl63Gr);
  gtk_widget_hide(lbl64Gr);
  gtk_widget_hide(lbl65Gr);

  gtk_widget_hide(btn71);
  gtk_widget_hide(btn72);
  gtk_widget_hide(btn73);
  gtk_widget_hide(btn74);
  gtk_widget_hide(btn75);

  gtk_widget_hide(lbl71F);
  gtk_widget_hide(lbl71G);
  gtk_widget_hide(lbl71L);
  gtk_widget_hide(lbl71H);
  gtk_widget_hide(lbl72F);
  gtk_widget_hide(lbl72G);
  gtk_widget_hide(lbl72L);
  gtk_widget_hide(lbl72H);
  gtk_widget_hide(lbl73F);
  gtk_widget_hide(lbl73G);
  gtk_widget_hide(lbl73L);
  gtk_widget_hide(lbl74F);
  gtk_widget_hide(lbl74G);
  gtk_widget_hide(lbl74L);
  gtk_widget_hide(lbl75F);
  gtk_widget_hide(lbl75G);
  gtk_widget_hide(lbl75L);
  gtk_widget_hide(lbl71Gr);
  gtk_widget_hide(lbl72Gr);
  gtk_widget_hide(lbl73Gr);
  gtk_widget_hide(lbl74Gr);
  gtk_widget_hide(lbl75Gr);

  gtk_widget_hide(btn81);
  gtk_widget_hide(btn82);
  gtk_widget_hide(btn83);
  gtk_widget_hide(btn84);
  gtk_widget_hide(btn85);

  gtk_widget_hide(lbl81F);
  gtk_widget_hide(lbl81G);
  gtk_widget_hide(lbl81L);
  gtk_widget_hide(lbl81H);
  gtk_widget_hide(lbl82F);
  gtk_widget_hide(lbl82G);
  gtk_widget_hide(lbl82L);
  gtk_widget_hide(lbl83F);
  gtk_widget_hide(lbl83G);
  gtk_widget_hide(lbl83L);
  gtk_widget_hide(lbl83H);
  gtk_widget_hide(lbl84F);
  gtk_widget_hide(lbl84G);
  gtk_widget_hide(lbl84L);
  gtk_widget_hide(lbl84H);
  gtk_widget_hide(lbl85F);
  gtk_widget_hide(lbl85G);
  gtk_widget_hide(lbl85L);
  gtk_widget_hide(lbl81Gr);
  gtk_widget_hide(lbl82Gr);
  gtk_widget_hide(lbl83Gr);
  gtk_widget_hide(lbl84Gr);
  gtk_widget_hide(lbl85Gr);

  gtk_widget_hide(lblOn);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("hideAllWidgets");
  #endif
}



void moveLabels(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("moveLabels");
  #endif

  int            xPos, yPos;
  GtkRequisition lblF, lblG;

  if(calcLandscape) {
    xPos = X_LEFT_LANDSCAPE;
    yPos = Y_TOP_LANDSCAPE;
  }
  else {
    xPos = X_LEFT_PORTRAIT;
    yPos = Y_TOP_PORTRAIT + DELTA_KEYS_Y;
  }

  #if defined(__MINGW64__)
   yPos += 5;
  #endif

  gtk_widget_get_preferred_size(  lbl21F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl21G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl21F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl21G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl21Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl21Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl22F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl22G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl22F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl22G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl22Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl22Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl23F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl23G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl23F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl23G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl23Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl23Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl24F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl24G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl24F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl24G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl24Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl24Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl25F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl25G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl25F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl25G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl25Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl25Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl26F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl26G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl26F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl26G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y;
  gtk_widget_get_preferred_size(  lbl31F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl31G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl31F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl31G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl31Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl31Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl32F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl32G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl32F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl32G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl32Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl32Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl33F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl33G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl33F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl33G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl33Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl33Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl34F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl34G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl34F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl34G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl34Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl34Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl35F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl35G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl35F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl35G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl36F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl36G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl36F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl36G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y;
  gtk_widget_get_preferred_size(  lbl41F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl41G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl41F, (2*xPos+KEY_WIDTH_1+DELTA_KEYS_X-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl41G, (2*xPos+KEY_WIDTH_1+DELTA_KEYS_X+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);

  xPos += 2*DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl42F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl42G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl42F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl42G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl42Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl42Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl43F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl43G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl43F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl43G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl43Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl43Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl44F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl44G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl44F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl44G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl44Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl44Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X;
  gtk_widget_get_preferred_size(  lbl45F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl45G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl45F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl45G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_widget_get_preferred_size(  lbl51F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl51G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl51F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl51G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl51Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl51Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 18;
  gtk_widget_get_preferred_size(  lbl52F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl52G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl52F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl52G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl52Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl52Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl53F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl53G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl53F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl53G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl53Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl53Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl54F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl54G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl54F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl54G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl54Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl54Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl55F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl55G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl55F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl55G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_widget_get_preferred_size(  lbl61F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl61G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl61F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl61G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl61Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl61Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 18;
  gtk_widget_get_preferred_size(  lbl62F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl62G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl62F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl62G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl62Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl62Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl63F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl63G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl63F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl63G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl63Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl63Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl64F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl64G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl64F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl64G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl64Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl64Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl65F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl65G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl65F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl65G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_widget_get_preferred_size(  lbl71F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl71G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl71F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl71G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl71Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl71Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 18;
  gtk_widget_get_preferred_size(  lbl72F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl72G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl72F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl72G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl72Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl72Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl73F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl73G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl73F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl73G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl73Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl73Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl74F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl74G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl74F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl74G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl74Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl74Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl75F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl75G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl75F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl75G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_widget_get_preferred_size(  lbl81F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl81G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl81F, (2*xPos+KEY_WIDTH_1-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl81G, (2*xPos+KEY_WIDTH_1+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl81Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl81Gr, xPos+KEY_WIDTH_1*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 18;
  gtk_widget_get_preferred_size(  lbl82F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl82G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl82F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl82G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl82Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl82Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl83F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl83G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl83F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl83G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_widget_get_preferred_size(  lbl83Gr, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl83Gr, xPos+KEY_WIDTH_2*2/3,                              yPos - Y_OFFSET_GREEK);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl84F, NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl84G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lbl84F, (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);
  gtk_fixed_move(GTK_FIXED(grid), lbl84G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);

  xPos += DELTA_KEYS_X + 17;
  gtk_widget_get_preferred_size(  lbl85F, NULL, &lblF);
  gtk_fixed_move(GTK_FIXED(grid), lbl85F, (2*xPos+KEY_WIDTH_2-lblF.width+2)/2, yPos - Y_OFFSET_SHIFTED_LABEL);

  gtk_widget_get_preferred_size(  lblOn,  NULL, &lblF);
  gtk_widget_get_preferred_size(  lbl85G, NULL, &lblG);
  gtk_fixed_move(GTK_FIXED(grid), lblOn,  (2*xPos+KEY_WIDTH_2-lblF.width-GAP-lblG.width+2)/2, yPos + 38);
  gtk_fixed_move(GTK_FIXED(grid), lbl85G, (2*xPos+KEY_WIDTH_2+lblF.width+GAP-lblG.width+2)/2, yPos + 38);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("moveLabels");
  #endif
}



void labelCaptionNormal(const calcKey_t *key, GtkWidget *button, GtkWidget *lblF, GtkWidget *lblG, GtkWidget *lblL) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("labelCaptionNormal");
  #endif

  uint8_t lbl[22];

  if(key->primary == 0) {
    lbl[0] = 0;
  }
  else {
    stringToUtf8(indexOfItems[max(key->primary, -key->primary)].itemPrinted, lbl);
  }

  if(strcmp((char *)lbl, "CATALOG") == 0 && key->keyId != 41) {
    lbl[3] = 0;
  }

  gtk_button_set_label(GTK_BUTTON(button), (gchar *)lbl);

  if(key->primary == KEY_f) {
    gtk_widget_set_name(button, "calcKeyF");
  }
  else if(key->primary == KEY_g) {
    gtk_widget_set_name(button, "calcKeyG");
  }
  else if((key->primary >= CHR_0 && key->primary <= CHR_9) || key->primary == CHR_PERIOD) {
    gtk_widget_set_name(button, "calcNumericKey");
  }
  else {
    gtk_widget_set_name(button, "calcKey");
  }

  stringToUtf8(indexOfItems[max(key->fShifted, -key->fShifted)].itemPrinted, lbl);

  if(key->fShifted == 0) {
    lbl[0] = 0;
  }
  else if(strcmp((char *)lbl, "CATALOG") == 0 && key->keyId != 41) {
    lbl[3] = 0;
  }

  gtk_label_set_label(GTK_LABEL(lblF), (gchar *)lbl);
  if(key->fShifted < 0) gtk_widget_set_name(lblF, "fShiftedUnderline"); else  gtk_widget_set_name(lblF, "fShifted");

  stringToUtf8(indexOfItems[max(key->gShifted, -key->gShifted)].itemPrinted, lbl);
  if(key->gShifted == 0) {
    lbl[0] = 0;
  }
  else if(strcmp((char *)lbl, "CATALOG") == 0 && key->keyId != 41) {
    lbl[3] = 0;
  }

  gtk_label_set_label(GTK_LABEL(lblG), (gchar *)lbl);
  if(key->gShifted < 0) gtk_widget_set_name(lblG, "gShiftedUnderline"); else  gtk_widget_set_name(lblG, "gShifted");

  stringToUtf8(indexOfItems[key->primaryAim].itemPrinted, lbl);
  if(key->primaryAim == 0) {
    lbl[0] = 0;
  }

  if(lbl[0] == ' ') {
    lbl[0] = '_';
  }

  gtk_label_set_label(GTK_LABEL(lblL), (gchar *)lbl);
  gtk_widget_set_name(lblL, "letter");

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("labelCaptionNormal");
  #endif
}



void labelCaptionAim(const calcKey_t *key, GtkWidget *button, GtkWidget *lblGreek, GtkWidget *lblL) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("labelCaptionAim");
  #endif

  uint8_t lbl[22];

  if(key->keyLblAim == 0) {
    lbl[0] = 0;
  }
  else {
    stringToUtf8(indexOfItems[max(key->keyLblAim, -key->keyLblAim)].itemPrinted, lbl);
  }

  if(strcmp((char *)lbl, "CATALOG") == 0 && key->keyId != 41) {
    lbl[3] = 0;
  }

  gtk_button_set_label(GTK_BUTTON(button), (gchar *)lbl);
  if(key->keyLblAim == KEY_f) {
    gtk_widget_set_name(button, "calcKeyF");
  }
  else if(key->keyLblAim == KEY_g) {
    gtk_widget_set_name(button, "calcKeyG");
  }
  else {
    if(key->keyLblAim == key->fShiftedAim && key->keyLblAim != ITM_NULL) {
      gtk_widget_set_name(button, "calcKeyGoldenBorder");
    }
    else {
      gtk_widget_set_name(button, "calcKey");
    }
  }


  if(CHR_ALPHA <= key->gShiftedAim && key->gShiftedAim <= CHR_OMEGA) {
    stringToUtf8(indexOfItems[key->gShiftedAim].itemPrinted, lbl);
    lbl[2] = ' ';
    lbl[3] = 0;
    stringToUtf8(indexOfItems[key->gShiftedAim + 36].itemPrinted, lbl + 3);
  }
  else {
    stringToUtf8(indexOfItems[max(key->gShiftedAim, -key->gShiftedAim)].itemPrinted, lbl);
  }

  if(key->gShiftedAim == 0) {
    lbl[0] = 0;
  }
  else if(strcmp((char *)lbl, "CATALOG") == 0 && key->keyId != 41) {
    lbl[3] = 0;
  }

  gtk_label_set_label(GTK_LABEL(lblGreek), (gchar *)lbl);

  if(key->gShiftedAim < 0) gtk_widget_set_name(lblGreek, "greekUnderline"); else  gtk_widget_set_name(lblGreek, "greek");


  if(key->primaryAim == 0) {
    lbl[0] = 0;
  }
  else {
   stringToUtf8(indexOfItems[key->primaryAim].itemPrinted, lbl);
  }

  if(lbl[0] == ' ') {
    lbl[0] = '_';
  }

  gtk_label_set_label(GTK_LABEL(lblL), (gchar *)lbl);
  gtk_widget_set_name(lblL, "letter");

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("labelCaptionAim");
  #endif
}



void labelCaptionTam(const calcKey_t *key, GtkWidget *button) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("labelCaptionTam");
  #endif

  uint8_t lbl[22];

  if(key->primaryTam == 0) {
    lbl[0] = 0;
  }
  else {
    stringToUtf8(indexOfItems[key->primaryTam].itemPrinted, lbl);
  }

  gtk_button_set_label(GTK_BUTTON(button), (gchar *)lbl);

  if(key->primaryTam == KEY_f) {
    gtk_widget_set_name(button, "calcKeyF");
  }
  else if(key->primaryTam == KEY_g) {
    gtk_widget_set_name(button, "calcKeyG");
  }
  else {
    gtk_widget_set_name(button, "calcKey");
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("labelCaptionTam");
  #endif
}



void calcModeNormalGui(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("calcModeNormalGui");
  #endif

  const calcKey_t *keys;

  keys = userModeEnabled ? kbd_usr : kbd_std;

  hideAllWidgets();

  labelCaptionNormal(keys++, btn21, lbl21F, lbl21G, lbl21L);
  labelCaptionNormal(keys++, btn22, lbl22F, lbl22G, lbl22L);
  labelCaptionNormal(keys++, btn23, lbl23F, lbl23G, lbl23L);
  labelCaptionNormal(keys++, btn24, lbl24F, lbl24G, lbl24L);
  labelCaptionNormal(keys++, btn25, lbl25F, lbl25G, lbl25L);
  labelCaptionNormal(keys++, btn26, lbl26F, lbl26G, lbl26L);

  labelCaptionNormal(keys++, btn31, lbl31F, lbl31G, lbl31L);
  labelCaptionNormal(keys++, btn32, lbl32F, lbl32G, lbl32L);
  labelCaptionNormal(keys++, btn33, lbl33F, lbl33G, lbl33L);
  labelCaptionNormal(keys++, btn34, lbl34F, lbl34G, lbl34L);
  labelCaptionNormal(keys++, btn35, lbl35F, lbl35G, lbl35L);
  labelCaptionNormal(keys++, btn36, lbl36F, lbl36G, lbl36L);

  labelCaptionNormal(keys++, btn41, lbl41F, lbl41G, lbl41L);
  labelCaptionNormal(keys++, btn42, lbl42F, lbl42G, lbl42L);
  labelCaptionNormal(keys++, btn43, lbl43F, lbl43G, lbl43L);
  labelCaptionNormal(keys++, btn44, lbl44F, lbl44G, lbl44L);
  labelCaptionNormal(keys++, btn45, lbl45F, lbl45G, lbl45L);

  labelCaptionNormal(keys++, btn51, lbl51F, lbl51G, lbl51L);
  labelCaptionNormal(keys++, btn52, lbl52F, lbl52G, lbl52L);
  labelCaptionNormal(keys++, btn53, lbl53F, lbl53G, lbl53L);
  labelCaptionNormal(keys++, btn54, lbl54F, lbl54G, lbl54L);
  labelCaptionNormal(keys++, btn55, lbl55F, lbl55G, lbl55L);

  labelCaptionNormal(keys++, btn61, lbl61F, lbl61G, lbl61L);
  labelCaptionNormal(keys++, btn62, lbl62F, lbl62G, lbl62L);
  labelCaptionNormal(keys++, btn63, lbl63F, lbl63G, lbl63L);
  labelCaptionNormal(keys++, btn64, lbl64F, lbl64G, lbl64L);
  labelCaptionNormal(keys++, btn65, lbl65F, lbl65G, lbl65L);

  labelCaptionNormal(keys++, btn71, lbl71F, lbl71G, lbl71L);
  labelCaptionNormal(keys++, btn72, lbl72F, lbl72G, lbl72L);
  labelCaptionNormal(keys++, btn73, lbl73F, lbl73G, lbl73L);
  labelCaptionNormal(keys++, btn74, lbl74F, lbl74G, lbl74L);
  labelCaptionNormal(keys++, btn75, lbl75F, lbl75G, lbl75L);

  labelCaptionNormal(keys++, btn81, lbl81F, lbl81G, lbl81L);
  labelCaptionNormal(keys++, btn82, lbl82F, lbl82G, lbl82L);
  labelCaptionNormal(keys++, btn83, lbl83F, lbl83G, lbl83L);
  labelCaptionNormal(keys++, btn84, lbl84F, lbl84G, lbl84L);
  labelCaptionNormal(keys++, btn85, lbl85F, lbl85G, lbl85L);

  gtk_widget_show(btn11);
  gtk_widget_show(btn12);
  gtk_widget_show(btn13);
  gtk_widget_show(btn14);
  gtk_widget_show(btn15);
  gtk_widget_show(btn16);

  gtk_widget_show(btn21);
  gtk_widget_show(btn22);
  gtk_widget_show(btn23);
  gtk_widget_show(btn24);
  gtk_widget_show(btn25);
  gtk_widget_show(btn26);

  gtk_widget_show(lbl21F);
  gtk_widget_show(lbl21G);
  gtk_widget_show(lbl21L);
  gtk_widget_show(lbl22F);
  gtk_widget_show(lbl22G);
  gtk_widget_show(lbl22L);
  gtk_widget_show(lbl23F);
  gtk_widget_show(lbl23G);
  gtk_widget_show(lbl23L);
  gtk_widget_show(lbl24F);
  gtk_widget_show(lbl24G);
  gtk_widget_show(lbl24L);
  gtk_widget_show(lbl25F);
  gtk_widget_show(lbl25G);
  gtk_widget_show(lbl25L);
  gtk_widget_show(lbl26L);
  gtk_widget_show(lbl26F);
  gtk_widget_show(lbl26G);

  gtk_widget_show(btn31);
  gtk_widget_show(btn32);
  gtk_widget_show(btn33);
  gtk_widget_show(btn34);
  gtk_widget_show(btn35);
  gtk_widget_show(btn36);

  gtk_widget_show(lbl31F);
  gtk_widget_show(lbl31G);
  gtk_widget_show(lbl31L);
  gtk_widget_show(lbl32F);
  gtk_widget_show(lbl32G);
  gtk_widget_show(lbl32L);
  gtk_widget_show(lbl33F);
  gtk_widget_show(lbl33G);
  gtk_widget_show(lbl33H);
  gtk_widget_show(lbl33L);
  gtk_widget_show(lbl34F);
  gtk_widget_show(lbl34G);
  gtk_widget_show(lbl34L);

  gtk_widget_show(btn41);
  gtk_widget_show(btn42);
  gtk_widget_show(btn43);
  gtk_widget_show(btn44);
  gtk_widget_show(btn45);

  gtk_widget_show(lbl41F);
  gtk_widget_show(lbl41G);
  gtk_widget_show(lbl42F);
  gtk_widget_show(lbl42G);
  gtk_widget_show(lbl42H);
  gtk_widget_show(lbl42L);
  gtk_widget_show(lbl43F);
  gtk_widget_show(lbl43G);
  gtk_widget_show(lbl43L);
  gtk_widget_show(lbl43P);
  gtk_widget_show(lbl44F);
  gtk_widget_show(lbl44G);
  gtk_widget_show(lbl44L);
  gtk_widget_show(lbl44P);
  gtk_widget_show(lbl45F);
  gtk_widget_show(lbl45G);

  gtk_widget_show(btn51);
  gtk_widget_show(btn52);
  gtk_widget_show(btn53);
  gtk_widget_show(btn54);
  gtk_widget_show(btn55);

  gtk_widget_show(lbl51F);
  gtk_widget_show(lbl51G);
  gtk_widget_show(lbl51L);
  gtk_widget_show(lbl52F);
  gtk_widget_show(lbl52G);
  gtk_widget_show(lbl52L);
  gtk_widget_show(lbl53F);
  gtk_widget_show(lbl53G);
  gtk_widget_show(lbl53L);
  gtk_widget_show(lbl54F);
  gtk_widget_show(lbl54G);
  gtk_widget_show(lbl54L);
  gtk_widget_show(lbl55F);
  gtk_widget_show(lbl55G);
  gtk_widget_show(lbl55L);

  gtk_widget_show(btn61);
  gtk_widget_show(btn62);
  gtk_widget_show(btn63);
  gtk_widget_show(btn64);
  gtk_widget_show(btn65);

  gtk_widget_show(lbl61F);
  gtk_widget_show(lbl61G);
  gtk_widget_show(lbl61L);
  gtk_widget_show(lbl62F);
  gtk_widget_show(lbl62G);
  gtk_widget_show(lbl62L);
  gtk_widget_show(lbl63F);
  gtk_widget_show(lbl63G);
  gtk_widget_show(lbl63L);
  gtk_widget_show(lbl64F);
  gtk_widget_show(lbl64G);
  gtk_widget_show(lbl64L);
  gtk_widget_show(lbl64H);
  gtk_widget_show(lbl65F);
  gtk_widget_show(lbl65G);

  gtk_widget_show(btn71);
  gtk_widget_show(btn72);
  gtk_widget_show(btn73);
  gtk_widget_show(btn74);
  gtk_widget_show(btn75);

  gtk_widget_show(lbl71F);
  gtk_widget_show(lbl71G);
  gtk_widget_show(lbl71L);
  gtk_widget_show(lbl71H);
  gtk_widget_show(lbl72F);
  gtk_widget_show(lbl72G);
  gtk_widget_show(lbl72L);
  gtk_widget_show(lbl72H);
  gtk_widget_show(lbl73F);
  gtk_widget_show(lbl73G);
  gtk_widget_show(lbl73L);
  gtk_widget_show(lbl74F);
  gtk_widget_show(lbl74G);
  gtk_widget_show(lbl74L);
  gtk_widget_show(lbl75F);
  gtk_widget_show(lbl75G);

  gtk_widget_show(btn81);
  gtk_widget_show(btn82);
  gtk_widget_show(btn83);
  gtk_widget_show(btn84);
  gtk_widget_show(btn85);

  gtk_widget_show(lbl81F);
  gtk_widget_show(lbl81G);
  gtk_widget_show(lbl81L);
  gtk_widget_show(lbl81H);
  gtk_widget_show(lbl82F);
  gtk_widget_show(lbl82G);
  gtk_widget_show(lbl82L);
  gtk_widget_show(lbl83F);
  gtk_widget_show(lbl83G);
  gtk_widget_show(lbl83L);
  gtk_widget_show(lbl83H);
  gtk_widget_show(lbl84F);
  gtk_widget_show(lbl84G);
  gtk_widget_show(lbl84H);
  gtk_widget_show(lbl84L);
  gtk_widget_show(lbl85F);
  gtk_widget_show(lbl85G);

  gtk_widget_show(lblOn);

  moveLabels();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("calcModeNormalGui");
  #endif
}



void calcModeAimGui(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("calcModeAimGui");
  #endif

  const calcKey_t *keys;

  keys = userModeEnabled ? kbd_usr : kbd_std;

  hideAllWidgets();

  labelCaptionAim(keys++, btn21, lbl21Gr, lbl21L);
  labelCaptionAim(keys++, btn22, lbl22Gr, lbl22L);
  labelCaptionAim(keys++, btn23, lbl23Gr, lbl23L);
  labelCaptionAim(keys++, btn24, lbl24Gr, lbl24L);
  labelCaptionAim(keys++, btn25, lbl25Gr, lbl25L);
  labelCaptionAim(keys++, btn26, lbl26Gr, lbl26L);

  labelCaptionAim(keys++, btn31, lbl31Gr, lbl31L);
  labelCaptionAim(keys++, btn32, lbl32Gr, lbl32L);
  labelCaptionAim(keys++, btn33, lbl33Gr, lbl33L);
  labelCaptionAim(keys++, btn34, lbl34Gr, lbl34L);
  labelCaptionAim(keys++, btn35, lbl35Gr, lbl35L);
  labelCaptionAim(keys++, btn36, lbl36Gr, lbl36L);

  labelCaptionAim(keys++, btn41, lbl41Gr, lbl41L);
  labelCaptionAim(keys++, btn42, lbl42Gr, lbl42L);
  labelCaptionAim(keys++, btn43, lbl43Gr, lbl43L);
  labelCaptionAim(keys++, btn44, lbl44Gr, lbl44L);
  labelCaptionAim(keys++, btn45, lbl45Gr, lbl45L);

  labelCaptionAim(keys++, btn51, lbl51Gr, lbl51L);
  labelCaptionAim(keys++, btn52, lbl52Gr, lbl52L);
  labelCaptionAim(keys++, btn53, lbl53Gr, lbl53L);
  labelCaptionAim(keys++, btn54, lbl54Gr, lbl54L);
  labelCaptionAim(keys++, btn55, lbl55Gr, lbl55L);

  labelCaptionAim(keys++, btn61, lbl61Gr, lbl61L);
  labelCaptionAim(keys++, btn62, lbl62Gr, lbl62L);
  labelCaptionAim(keys++, btn63, lbl63Gr, lbl63L);
  labelCaptionAim(keys++, btn64, lbl64Gr, lbl64L);
  labelCaptionAim(keys++, btn65, lbl65Gr, lbl65L);

  labelCaptionAim(keys++, btn71, lbl71Gr, lbl71L);
  labelCaptionAim(keys++, btn72, lbl72Gr, lbl72L);
  labelCaptionAim(keys++, btn73, lbl73Gr, lbl73L);
  labelCaptionAim(keys++, btn74, lbl74Gr, lbl74L);
  labelCaptionAim(keys++, btn75, lbl75Gr, lbl75L);

  labelCaptionAim(keys++, btn81, lbl81Gr, lbl81L);
  labelCaptionAim(keys++, btn82, lbl82Gr, lbl82L);
  labelCaptionAim(keys++, btn83, lbl83Gr, lbl83L);
  labelCaptionAim(keys++, btn84, lbl84Gr, lbl84L);
  labelCaptionAim(keys++, btn85, lbl85Gr, lbl85L);

  gtk_widget_show(lblOn);

  gtk_widget_show(btn11);
  gtk_widget_show(btn12);
  gtk_widget_show(btn13);
  gtk_widget_show(btn14);
  gtk_widget_show(btn15);
  gtk_widget_show(btn16);

  gtk_widget_show(btn21);
  gtk_widget_show(btn22);
  gtk_widget_show(btn23);
  gtk_widget_show(btn24);
  gtk_widget_show(btn25);
  gtk_widget_show(btn26);

  gtk_widget_show(lbl21L);
  gtk_widget_show(lbl22L);
  gtk_widget_show(lbl23L);
  gtk_widget_show(lbl24L);
  gtk_widget_show(lbl25L);
  gtk_widget_show(lbl26L);
  //gtk_widget_show(lbl26F);
  //gtk_widget_show(lbl26G);
  gtk_widget_show(lbl21Gr);
  gtk_widget_show(lbl22Gr);
  gtk_widget_show(lbl23Gr);
  gtk_widget_show(lbl24Gr);
  gtk_widget_show(lbl25Gr);

  gtk_widget_show(btn31);
  gtk_widget_show(btn32);
  gtk_widget_show(btn33);
  gtk_widget_show(btn34);
  gtk_widget_show(btn35);
  gtk_widget_show(btn36);

  gtk_widget_show(lbl31F);
  gtk_widget_show(lbl31L);
  gtk_widget_show(lbl32L);
  gtk_widget_show(lbl33H);
  gtk_widget_show(lbl33L);
  gtk_widget_show(lbl34L);
  gtk_widget_show(lbl31Gr);
  gtk_widget_show(lbl32Gr);
  gtk_widget_show(lbl33Gr);
  gtk_widget_show(lbl34Gr);

  gtk_widget_show(btn41);
  gtk_widget_show(btn42);
  gtk_widget_show(btn43);
  gtk_widget_show(btn44);
  gtk_widget_show(btn45);

  gtk_widget_show(lbl41F);
  gtk_widget_show(lbl42L);
  gtk_widget_show(lbl43L);
  gtk_widget_show(lbl43P);
  gtk_widget_show(lbl44L);
  gtk_widget_show(lbl44P);
  //gtk_widget_show(lbl45F);
  //gtk_widget_show(lbl45G);
  gtk_widget_show(lbl42Gr);
  gtk_widget_show(lbl43Gr);
  gtk_widget_show(lbl44Gr);

  gtk_widget_show(btn51);
  gtk_widget_show(btn52);
  gtk_widget_show(btn53);
  gtk_widget_show(btn54);
  gtk_widget_show(btn55);

  gtk_widget_show(lbl51L);
  gtk_widget_show(lbl52L);
  gtk_widget_show(lbl53L);
  gtk_widget_show(lbl54L);
  gtk_widget_show(lbl55L);
  gtk_widget_show(lbl51Gr);
  gtk_widget_show(lbl52Gr);
  gtk_widget_show(lbl53Gr);
  gtk_widget_show(lbl54Gr);

  gtk_widget_show(btn61);
  gtk_widget_show(btn62);
  gtk_widget_show(btn63);
  gtk_widget_show(btn64);
  gtk_widget_show(btn65);

  gtk_widget_show(lbl61L);
  gtk_widget_show(lbl62L);
  gtk_widget_show(lbl63L);
  gtk_widget_show(lbl64L);
  //gtk_widget_show(lbl65F);
  //gtk_widget_show(lbl65G);
  gtk_widget_show(lbl61Gr);
  gtk_widget_show(lbl62Gr);
  gtk_widget_show(lbl63Gr);
  gtk_widget_show(lbl64Gr);

  gtk_widget_show(btn71);
  gtk_widget_show(btn72);
  gtk_widget_show(btn73);
  gtk_widget_show(btn74);
  gtk_widget_show(btn75);

  gtk_widget_show(lbl71L);
  gtk_widget_show(lbl72L);
  gtk_widget_show(lbl73L);
  gtk_widget_show(lbl74L);
  //gtk_widget_show(lbl75F);
  //gtk_widget_show(lbl75G);
  gtk_widget_show(lbl71Gr);
  gtk_widget_show(lbl72Gr);
  gtk_widget_show(lbl73Gr);
  gtk_widget_show(lbl74Gr);

  gtk_widget_show(btn81);
  gtk_widget_show(btn82);
  gtk_widget_show(btn83);
  gtk_widget_show(btn84);
  gtk_widget_show(btn85);

  gtk_widget_show(lbl81L);
  gtk_widget_show(lbl82L);
  gtk_widget_show(lbl83L);
  gtk_widget_show(lbl84L);
  gtk_widget_show(lbl85G);
  gtk_widget_show(lbl81Gr);
  gtk_widget_show(lbl82Gr);
  gtk_widget_show(lbl83Gr);

  gtk_widget_show(lblOn);

  gtk_widget_show(lbl31G);
  gtk_widget_show(lbl41G);
  moveLabels();
  gtk_widget_hide(lbl31G);
  gtk_widget_hide(lbl41G);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("calcModeAimGui");
  #endif
}



void calcModeTamGui(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("calcModeTamGui");
  #endif

  const calcKey_t *keys;

  keys = userModeEnabled ? kbd_usr : kbd_std;

  hideAllWidgets();

  labelCaptionTam(keys++, btn21);
  labelCaptionTam(keys++, btn22);
  labelCaptionTam(keys++, btn23);
  labelCaptionTam(keys++, btn24);
  labelCaptionTam(keys++, btn25);
  labelCaptionTam(keys++, btn26);

  labelCaptionTam(keys++, btn31);
  labelCaptionTam(keys++, btn32);
  labelCaptionTam(keys++, btn33);
  labelCaptionTam(keys++, btn34);
  labelCaptionTam(keys++, btn35);
  labelCaptionTam(keys++, btn36);

  labelCaptionTam(keys++, btn41);
  labelCaptionTam(keys++, btn42);
  labelCaptionTam(keys++, btn43);
  labelCaptionTam(keys++, btn44);
  labelCaptionTam(keys++, btn45);

  labelCaptionTam(keys++, btn51);
  labelCaptionTam(keys++, btn52);
  labelCaptionTam(keys++, btn53);
  labelCaptionTam(keys++, btn54);
  labelCaptionTam(keys++, btn55);

  labelCaptionTam(keys++, btn61);
  labelCaptionTam(keys++, btn62);
  labelCaptionTam(keys++, btn63);
  labelCaptionTam(keys++, btn64);
  labelCaptionTam(keys++, btn65);

  labelCaptionTam(keys++, btn71);
  labelCaptionTam(keys++, btn72);
  labelCaptionTam(keys++, btn73);
  labelCaptionTam(keys++, btn74);
  labelCaptionTam(keys++, btn75);

  labelCaptionTam(keys++, btn81);
  labelCaptionTam(keys++, btn82);
  labelCaptionTam(keys++, btn83);
  labelCaptionTam(keys++, btn84);
  labelCaptionTam(keys++, btn85);

  hideAllWidgets();

  gtk_widget_show(btn11);
  gtk_widget_show(btn12);
  gtk_widget_show(btn13);
  gtk_widget_show(btn14);
  gtk_widget_show(btn15);
  gtk_widget_show(btn16);

  gtk_widget_show(btn21);
  gtk_widget_show(btn22);
  gtk_widget_show(btn23);
  gtk_widget_show(btn24);
  gtk_widget_show(btn25);
  gtk_widget_show(btn26);

  gtk_widget_show(btn31);
  gtk_widget_show(btn32);
  gtk_widget_show(btn33);
  gtk_widget_show(btn34);
  gtk_widget_show(btn35);
  gtk_widget_show(btn36);

  gtk_widget_show(btn41);
  gtk_widget_show(btn42);
  gtk_widget_show(btn43);
  gtk_widget_show(btn44);
  gtk_widget_show(btn45);

  gtk_widget_show(btn51);
  gtk_widget_show(btn52);
  gtk_widget_show(btn53);
  gtk_widget_show(btn54);
  gtk_widget_show(btn55);

  gtk_widget_show(btn61);
  gtk_widget_show(btn62);
  gtk_widget_show(btn63);
  gtk_widget_show(btn64);
  gtk_widget_show(btn65);

  gtk_widget_show(btn71);
  gtk_widget_show(btn72);
  gtk_widget_show(btn73);
  gtk_widget_show(btn74);
  gtk_widget_show(btn75);

  gtk_widget_show(btn81);
  gtk_widget_show(btn82);
  gtk_widget_show(btn83);
  gtk_widget_show(btn84);
  gtk_widget_show(btn85);

  moveLabels();

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("calcModeTamGui");
  #endif
}



void configureCallback(GtkWindow *window, GdkEvent *event, gpointer data) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("configureCallback");
  #endif

  allowScreenUpdate = false;
  //printf("x=%d y=%d\n", event->configure.x, event->configure.y);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("configureCallback");
  #endif
 }



/********************************************//**
 * \brief Creates the calc's GUI window with all the widgets
 *
 * \param void
 * \return void
 ***********************************************/
void setupUI(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("setupUI");
  #endif

  int            numBytes, xPos, yPos;
  GError         *error;
  GtkCssProvider *cssProvider;
  GdkDisplay     *cssDisplay;
  GdkScreen      *cssScreen;

  prepareCssData();

  cssProvider = gtk_css_provider_new();
  cssDisplay  = gdk_display_get_default();
  cssScreen   = gdk_display_get_default_screen(cssDisplay);
  gtk_style_context_add_provider_for_screen(cssScreen, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);

  error = NULL;
  gtk_css_provider_load_from_data(cssProvider, cssData, -1, &error);
  if(error != NULL) {
    showInfoDialog("In function setupUI:", "error while loading CSS style sheet " CSSFILE, NULL, NULL);
    exit(1);
  }
  g_object_unref(cssProvider);
  free(cssData);

  // Get the monitor geometry to determine whether the calc is portrait or landscape
  GdkRectangle monitor;
  gdk_monitor_get_geometry(gdk_display_get_monitor(gdk_display_get_default(), 0), &monitor);
  //gdk_screen_get_monitor_geometry(gdk_screen_get_default(), 0, &monitor);

  if(calcAutoLandscapePortrait) {
    calcLandscape = (monitor.height <  1025);
  }

  // The main window
  frmCalc = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  if(calcLandscape) {
    #if (DEBUG_PANEL == 1)
      gtk_window_set_default_size(GTK_WINDOW(frmCalc), 1000, 540);
      debugWidgetDx = 0;
      debugWidgetDy = 545;
    #else
      gtk_window_set_default_size(GTK_WINDOW(frmCalc), 1000, 540);
    #endif
  }
  else {
    #if (DEBUG_PANEL == 1)
      gtk_window_set_default_size(GTK_WINDOW(frmCalc),  1530, 980);
      debugWidgetDx = 531;
      debugWidgetDy = 0;
    #else
      gtk_window_set_default_size(GTK_WINDOW(frmCalc),  526, 980);
    #endif
  }

  gtk_widget_set_name(frmCalc, "mainWindow");
  gtk_window_set_resizable (GTK_WINDOW(frmCalc), FALSE);
  gtk_window_set_title(GTK_WINDOW(frmCalc), "WP 43S");
  g_signal_connect(frmCalc, "destroy", G_CALLBACK(destroyCalc), NULL);
  g_signal_connect(frmCalc, "key_press_event", G_CALLBACK(keyPressed), NULL);

  gtk_widget_add_events(GTK_WIDGET(frmCalc), GDK_CONFIGURE);
  g_signal_connect(G_OBJECT(frmCalc), "configure-event", G_CALLBACK(configureCallback), NULL);

  // Fixed grid to freely put widgets on it
  grid = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(frmCalc), grid);



  // Backround image
  if(calcLandscape) {
    backgroundImage = gtk_image_new_from_file("dm42lshort.png");
  }
  else {
    backgroundImage = gtk_image_new_from_file("dm42l.png");
  }

  gtk_fixed_put(GTK_FIXED(grid), backgroundImage,  0, 0);


  // Areas for the g shifted softkeys
  lblGSoftkeyArea = gtk_label_new("");
  gtk_widget_set_name(lblGSoftkeyArea, "gSoftkeyArea");
  gtk_widget_set_size_request(lblGSoftkeyArea, 438, 25);
  gtk_fixed_put(GTK_FIXED(grid), lblGSoftkeyArea, 44, 72+168);



  // Area for the f shifted softkeys
  lblFSoftkeyArea = gtk_label_new("");
  gtk_widget_set_name(lblFSoftkeyArea, "fSoftkeyArea");
  gtk_widget_set_size_request(lblFSoftkeyArea, 438, 25);
  gtk_fixed_put(GTK_FIXED(grid), lblFSoftkeyArea, 44, 72+168+25);



  // Area for the softkeys
  //lblSoftkeyArea1 = gtk_label_new("");
  //gtk_widget_set_name(lblSoftkeyArea1, "softkeyArea");
  //gtk_widget_set_size_request(lblSoftkeyArea1, 460, 40);
  //gtk_fixed_put(GTK_FIXED(grid), lblSoftkeyArea1, 33, 72+168+50);

  //lblSoftkeyArea2 = gtk_label_new("");
  //gtk_widget_set_name(lblSoftkeyArea2, "softkeyArea");
  //gtk_widget_set_size_request(lblSoftkeyArea2, 460, 44);
  //gtk_fixed_put(GTK_FIXED(grid), lblSoftkeyArea2, 33, 72+168+50+66);



  // Behind screen
  lblBehindScreen = gtk_label_new("");
  gtk_widget_set_name(lblBehindScreen, "behindScreen");
  gtk_widget_set_size_request(lblBehindScreen, 412, 252);
  gtk_fixed_put(GTK_FIXED(grid), lblBehindScreen, 57, 66);



  // LCD screen 400x240
  screen = gtk_drawing_area_new();
  gtk_widget_set_size_request(screen, SCREEN_WIDTH, SCREEN_HEIGHT);
  gtk_fixed_put(GTK_FIXED(grid), screen, 63, 72);
  screenStride = cairo_format_stride_for_width(CAIRO_FORMAT_RGB24, SCREEN_WIDTH)/4;
  numBytes = screenStride * SCREEN_HEIGHT * 4;
  screenData = malloc(numBytes);
  if(screenData == NULL) {
    sprintf(errorMessage, "error allocating %d x %d = %d bytes for screenData", screenStride*4, SCREEN_HEIGHT, numBytes);
    showInfoDialog("In function setupUI:", errorMessage, NULL, NULL);
    exit(1);
  }

  clearScreen(true, true, true);
  g_signal_connect(screen, "draw", G_CALLBACK(drawScreen), NULL);



  // 1st row: F1 to F6 buttons
  btn11 = gtk_button_new_with_label("^");
  btn12 = gtk_button_new_with_label("^");
  btn13 = gtk_button_new_with_label("^");
  btn14 = gtk_button_new_with_label("^");
  btn15 = gtk_button_new_with_label("^");
  btn16 = gtk_button_new_with_label("^");

  gtk_widget_set_size_request(btn11, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn12, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn13, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn14, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn15, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn16, KEY_WIDTH_1, 0);

  gtk_widget_set_name(btn11, "calcKey");
  gtk_widget_set_name(btn12, "calcKey");
  gtk_widget_set_name(btn13, "calcKey");
  gtk_widget_set_name(btn14, "calcKey");
  gtk_widget_set_name(btn15, "calcKey");
  gtk_widget_set_name(btn16, "calcKey");

  g_signal_connect(btn11, "clicked", G_CALLBACK(btnFnClicked), "1");
  g_signal_connect(btn12, "clicked", G_CALLBACK(btnFnClicked), "2");
  g_signal_connect(btn13, "clicked", G_CALLBACK(btnFnClicked), "3");
  g_signal_connect(btn14, "clicked", G_CALLBACK(btnFnClicked), "4");
  g_signal_connect(btn15, "clicked", G_CALLBACK(btnFnClicked), "5");
  g_signal_connect(btn16, "clicked", G_CALLBACK(btnFnClicked), "6");

  xPos = X_LEFT_PORTRAIT;
  yPos = Y_TOP_PORTRAIT;
  gtk_fixed_put(GTK_FIXED(grid), btn11, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn12, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn13, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn14, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn15, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn16, xPos, yPos);



  // 2nd row
  btn21   = gtk_button_new();
  btn22   = gtk_button_new();
  btn23   = gtk_button_new();
  btn24   = gtk_button_new();
  btn25   = gtk_button_new();
  btn26   = gtk_button_new();
  lbl21F  = gtk_label_new("");
  lbl22F  = gtk_label_new("");
  lbl23F  = gtk_label_new("");
  lbl24F  = gtk_label_new("");
  lbl25F  = gtk_label_new("");
  lbl26F  = gtk_label_new("");
  lbl21G  = gtk_label_new("");
  lbl22G  = gtk_label_new("");
  lbl23G  = gtk_label_new("");
  lbl24G  = gtk_label_new("");
  lbl25G  = gtk_label_new("");
  lbl26G  = gtk_label_new("");
  lbl21L  = gtk_label_new("");
  lbl22L  = gtk_label_new("");
  lbl23L  = gtk_label_new("");
  lbl24L  = gtk_label_new("");
  lbl25L  = gtk_label_new("");
  lbl26L  = gtk_label_new("");
  lbl21Gr = gtk_label_new("");
  lbl22Gr = gtk_label_new("");
  lbl23Gr = gtk_label_new("");
  lbl24Gr = gtk_label_new("");
  lbl25Gr = gtk_label_new("");
  lbl26Gr = gtk_label_new("");

  gtk_widget_set_size_request(btn21, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn22, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn23, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn24, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn25, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn26, KEY_WIDTH_1, 0);

  g_signal_connect(btn21, "clicked", G_CALLBACK(btnClicked), "00");
  g_signal_connect(btn22, "clicked", G_CALLBACK(btnClicked), "01");
  g_signal_connect(btn23, "clicked", G_CALLBACK(btnClicked), "02");
  g_signal_connect(btn24, "clicked", G_CALLBACK(btnClicked), "03");
  g_signal_connect(btn25, "clicked", G_CALLBACK(btnClicked), "04");
  g_signal_connect(btn26, "clicked", G_CALLBACK(btnClicked), "05");

  gtk_fixed_put(GTK_FIXED(grid), lbl21F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl22F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl23F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl24F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl25F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl26F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl21G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl22G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl23G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl24G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl25G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl26G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl21Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl22Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl23Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl24Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl25Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl26Gr, 0, 0);

  if(calcLandscape) {
    xPos = X_LEFT_LANDSCAPE;
    yPos = Y_TOP_LANDSCAPE;
  }
  else {
    xPos = X_LEFT_PORTRAIT;
    yPos += DELTA_KEYS_Y;
  }

  gtk_fixed_put(GTK_FIXED(grid), btn21,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl21L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn22,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl22L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn23,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl23L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn24,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl24L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn25,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl25L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn26,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl26L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);



  // 3rd row
  btn31   = gtk_button_new();
  btn32   = gtk_button_new();
  btn33   = gtk_button_new();
  btn34   = gtk_button_new();
  btn35   = gtk_button_new();
  btn36   = gtk_button_new();
  lbl31F  = gtk_label_new("");
  lbl32F  = gtk_label_new("");
  lbl33F  = gtk_label_new("");
  lbl34F  = gtk_label_new("");
  lbl35F  = gtk_label_new("");
  lbl36F  = gtk_label_new("");
  lbl31G  = gtk_label_new("");
  lbl32G  = gtk_label_new("");
  lbl33G  = gtk_label_new("");
  lbl34G  = gtk_label_new("");
  lbl35G  = gtk_label_new("");
  lbl36G  = gtk_label_new("");
  lbl33H  = gtk_label_new("\u2193"); // Low case
  lbl31L  = gtk_label_new("");
  lbl32L  = gtk_label_new("");
  lbl33L  = gtk_label_new("");
  lbl34L  = gtk_label_new("");
  lbl35L  = gtk_label_new("");
  lbl36L  = gtk_label_new("");
  lbl31Gr = gtk_label_new("");
  lbl32Gr = gtk_label_new("");
  lbl33Gr = gtk_label_new("");
  lbl34Gr = gtk_label_new("");
  lbl35Gr = gtk_label_new("");
  lbl36Gr = gtk_label_new("");

  gtk_widget_set_size_request(btn31, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn32, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn33, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn34, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn35, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn36, KEY_WIDTH_1, 0);

  gtk_widget_set_name(lbl33H,  "fShifted");

  g_signal_connect(btn31, "clicked", G_CALLBACK(btnClicked), "06");
  g_signal_connect(btn32, "clicked", G_CALLBACK(btnClicked), "07");
  g_signal_connect(btn33, "clicked", G_CALLBACK(btnClicked), "08");
  g_signal_connect(btn34, "clicked", G_CALLBACK(btnClicked), "09");
  g_signal_connect(btn35, "clicked", G_CALLBACK(btnClicked), "10");
  g_signal_connect(btn36, "clicked", G_CALLBACK(btnClicked), "11");

  gtk_fixed_put(GTK_FIXED(grid), lbl31F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl32F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl33F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl34F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl35F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl36F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl31G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl32G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl33G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl34G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl35G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl36G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl31Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl32Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl33Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl34Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl35Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl36Gr, 0, 0);

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y;
  gtk_fixed_put(GTK_FIXED(grid), btn31,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl31L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn32,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl32L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn33,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl33L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl33H, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos -  1);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn34,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl34L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn35,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl35L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn36,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl36L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);



  // 4th row
  btn41   = gtk_button_new();
  btn42   = gtk_button_new();
  btn43   = gtk_button_new();
  btn44   = gtk_button_new();
  btn45   = gtk_button_new();
  lbl41F  = gtk_label_new("");
  lbl42F  = gtk_label_new("");
  lbl43F  = gtk_label_new("");
  lbl44F  = gtk_label_new("");
  lbl45F  = gtk_label_new("");
  lbl41G  = gtk_label_new("");
  lbl42G  = gtk_label_new("");
  lbl43G  = gtk_label_new("");
  lbl44G  = gtk_label_new("");
  lbl45G  = gtk_label_new("");
  lbl42H  = gtk_label_new("\u03b7"); // eta
  lbl43P  = gtk_label_new("");
  lbl44P  = gtk_label_new("\u2191"); // Up case
  lbl41L  = gtk_label_new("");
  lbl42L  = gtk_label_new("");
  lbl43L  = gtk_label_new("");
  lbl44L  = gtk_label_new("");
  lbl45L  = gtk_label_new("");
  lbl41Gr = gtk_label_new("");
  lbl42Gr = gtk_label_new("");
  lbl43Gr = gtk_label_new("");
  lbl44Gr = gtk_label_new("");
  lbl45Gr = gtk_label_new("");

  gtk_widget_set_size_request(btn41, KEY_WIDTH_1 + DELTA_KEYS_X, 0);
  gtk_widget_set_size_request(btn42, KEY_WIDTH_1,              0);
  gtk_widget_set_size_request(btn43, KEY_WIDTH_1,              0);
  gtk_widget_set_size_request(btn44, KEY_WIDTH_1,              0);
  gtk_widget_set_size_request(btn45, KEY_WIDTH_1,              0);

  gtk_widget_set_name(lbl43P,  "fShifted");
  gtk_widget_set_name(lbl42H,  "gShifted");
  gtk_widget_set_name(lbl44P,  "fShifted");

  g_signal_connect(btn41, "clicked", G_CALLBACK(btnClicked), "12");
  g_signal_connect(btn42, "clicked", G_CALLBACK(btnClicked), "13");
  g_signal_connect(btn43, "clicked", G_CALLBACK(btnClicked), "14");
  g_signal_connect(btn44, "clicked", G_CALLBACK(btnClicked), "15");
  g_signal_connect(btn45, "clicked", G_CALLBACK(btnClicked), "16");

  gtk_fixed_put(GTK_FIXED(grid), lbl41F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl42F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl43F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl44F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl45F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl41G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl42G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl43G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl44G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl45G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl41Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl42Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl43Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl44Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl45Gr, 0, 0);

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y;
  gtk_fixed_put(GTK_FIXED(grid), btn41,  xPos,                          yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl41L, xPos + KEY_WIDTH_1 + DELTA_KEYS_X + 4, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X*2;
  gtk_fixed_put(GTK_FIXED(grid), btn42,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl42L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl42H, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos -  1);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn43,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl43L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl43P, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos -  1);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn44,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl44L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl44P, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos -  1);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn45,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl45L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);



  // 5th row
  btn51   = gtk_button_new();
  btn52   = gtk_button_new();
  btn53   = gtk_button_new();
  btn54   = gtk_button_new();
  btn55   = gtk_button_new();
  lbl51F  = gtk_label_new("");
  lbl52F  = gtk_label_new("");
  lbl53F  = gtk_label_new("");
  lbl54F  = gtk_label_new("");
  lbl55F  = gtk_label_new("");
  lbl51G  = gtk_label_new("");
  lbl52G  = gtk_label_new("");
  lbl53G  = gtk_label_new("");
  lbl54G  = gtk_label_new("");
  lbl55G  = gtk_label_new("");
  lbl51L  = gtk_label_new("");
  lbl52L  = gtk_label_new("");
  lbl53L  = gtk_label_new("");
  lbl54L  = gtk_label_new("");
  lbl55L  = gtk_label_new("");
  lbl51Gr = gtk_label_new("");
  lbl52Gr = gtk_label_new("");
  lbl53Gr = gtk_label_new("");
  lbl54Gr = gtk_label_new("");
  lbl55Gr = gtk_label_new("");

  gtk_widget_set_size_request(btn51, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn52, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn53, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn54, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn55, KEY_WIDTH_2, 0);

  g_signal_connect(btn51, "clicked", G_CALLBACK(btnClicked), "17");
  g_signal_connect(btn52, "clicked", G_CALLBACK(btnClicked), "18");
  g_signal_connect(btn53, "clicked", G_CALLBACK(btnClicked), "19");
  g_signal_connect(btn54, "clicked", G_CALLBACK(btnClicked), "20");
  g_signal_connect(btn55, "clicked", G_CALLBACK(btnClicked), "21");

  gtk_fixed_put(GTK_FIXED(grid), lbl51F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl52F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl53F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl54F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl55F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl51G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl52G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl53G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl54G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl55G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl51Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl52Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl53Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl54Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl55Gr, 0, 0);

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_fixed_put(GTK_FIXED(grid), btn51,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl51L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X + 18;
  gtk_fixed_put(GTK_FIXED(grid), btn52,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl52L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn53,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl53L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn54,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl54L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn55,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl55L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);



  // 6th row
  btn61   = gtk_button_new();
  btn61   = gtk_button_new();
  btn62   = gtk_button_new();
  btn63   = gtk_button_new();
  btn64   = gtk_button_new();
  btn65   = gtk_button_new();
  lbl61F  = gtk_label_new("");
  lbl62F  = gtk_label_new("");
  lbl63F  = gtk_label_new("");
  lbl64F  = gtk_label_new("");
  lbl65F  = gtk_label_new("");
  lbl61G  = gtk_label_new("");
  lbl62G  = gtk_label_new("");
  lbl63G  = gtk_label_new("");
  lbl64G  = gtk_label_new("");
  lbl65G  = gtk_label_new("");
  lbl64H  = gtk_label_new("\u03b8");
  lbl61L  = gtk_label_new("");
  lbl62L  = gtk_label_new("");
  lbl63L  = gtk_label_new("");
  lbl64L  = gtk_label_new("");
  lbl65L  = gtk_label_new("");
  lbl61Gr = gtk_label_new("");
  lbl62Gr = gtk_label_new("");
  lbl63Gr = gtk_label_new("");
  lbl64Gr = gtk_label_new("");
  lbl65Gr = gtk_label_new("");

  gtk_widget_set_size_request(btn61, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn62, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn63, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn64, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn65, KEY_WIDTH_2, 0);

  gtk_widget_set_name(lbl64H,  "gShifted");

  g_signal_connect(btn61, "clicked", G_CALLBACK(btnClicked), "22");
  g_signal_connect(btn62, "clicked", G_CALLBACK(btnClicked), "23");
  g_signal_connect(btn63, "clicked", G_CALLBACK(btnClicked), "24");
  g_signal_connect(btn64, "clicked", G_CALLBACK(btnClicked), "25");
  g_signal_connect(btn65, "clicked", G_CALLBACK(btnClicked), "26");

  gtk_fixed_put(GTK_FIXED(grid), lbl61F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl62F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl63F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl64F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl65F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl61G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl62G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl63G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl64G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl65G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl61Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl62Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl63Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl64Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl65Gr, 0, 0);

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_fixed_put(GTK_FIXED(grid), btn61,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl61L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X + 18;
  gtk_fixed_put(GTK_FIXED(grid), btn62,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl62L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn63,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl63L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn64,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl64L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl64H, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos -  1);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn65,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl65L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);



  // 7th row
  btn71   = gtk_button_new();
  btn72   = gtk_button_new();
  btn73   = gtk_button_new();
  btn74   = gtk_button_new();
  btn75   = gtk_button_new();
  lbl71F  = gtk_label_new("");
  lbl72F  = gtk_label_new("");
  lbl73F  = gtk_label_new("");
  lbl74F  = gtk_label_new("");
  lbl75F  = gtk_label_new("");
  lbl71G  = gtk_label_new("");
  lbl72G  = gtk_label_new("");
  lbl73G  = gtk_label_new("");
  lbl74G  = gtk_label_new("");
  lbl75G  = gtk_label_new("");
  lbl71H  = gtk_label_new("M");      // Menu alphaMATH for AIM
  lbl72H  = gtk_label_new("\u03c8"); // psi
  lbl71L  = gtk_label_new("");
  lbl72L  = gtk_label_new("");
  lbl73L  = gtk_label_new("");
  lbl74L  = gtk_label_new("");
  lbl75L  = gtk_label_new("");
  lbl71Gr = gtk_label_new("");
  lbl72Gr = gtk_label_new("");
  lbl73Gr = gtk_label_new("");
  lbl74Gr = gtk_label_new("");
  lbl75Gr = gtk_label_new("");

  gtk_widget_set_size_request(btn71, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn72, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn73, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn74, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn75, KEY_WIDTH_2, 0);

  gtk_widget_set_name(lbl71H,  "gShiftedUnderline");
  gtk_widget_set_name(lbl72H,  "gShifted");

  g_signal_connect(btn71, "clicked", G_CALLBACK(btnClicked), "27");
  g_signal_connect(btn72, "clicked", G_CALLBACK(btnClicked), "28");
  g_signal_connect(btn73, "clicked", G_CALLBACK(btnClicked), "29");
  g_signal_connect(btn74, "clicked", G_CALLBACK(btnClicked), "30");
  g_signal_connect(btn75, "clicked", G_CALLBACK(btnClicked), "31");

  gtk_fixed_put(GTK_FIXED(grid), lbl71F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl72F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl73F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl74F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl75F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl71G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl72G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl73G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl74G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl75G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl71Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl72Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl73Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl74Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl75Gr, 0, 0);

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_fixed_put(GTK_FIXED(grid), btn71,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl71L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl71H, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos -  1);

  xPos += DELTA_KEYS_X + 18;
  gtk_fixed_put(GTK_FIXED(grid), btn72,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl72L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl72H, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos -  1);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn73,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl73L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn74,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl74L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn75,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl75L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);



  // 8th row
  btn81   = gtk_button_new();
  btn82   = gtk_button_new();
  btn83   = gtk_button_new();
  btn84   = gtk_button_new();
  btn85   = gtk_button_new();
  lbl81F  = gtk_label_new("");
  lbl82F  = gtk_label_new("");
  lbl83F  = gtk_label_new("");
  lbl84F  = gtk_label_new("");
  lbl85F  = gtk_label_new("");
  lbl81G  = gtk_label_new("");
  lbl82G  = gtk_label_new("");
  lbl83G  = gtk_label_new("");
  lbl84G  = gtk_label_new("");
  lbl85G  = gtk_label_new("");
  lbl81H  = gtk_label_new("I");      // Menu alphaINTL for AIM
  lbl83H  = gtk_label_new("\u2218"); // Menu alphaDOT for AIM
  lbl84H  = gtk_label_new("\u2399"); // Printer
  lbl81L  = gtk_label_new("");
  lbl82L  = gtk_label_new("");
  lbl83L  = gtk_label_new("");
  lbl84L  = gtk_label_new("");
  lbl85L  = gtk_label_new("");
  lbl81Gr = gtk_label_new("");
  lbl82Gr = gtk_label_new("");
  lbl83Gr = gtk_label_new("");
  lbl84Gr = gtk_label_new("");
  lbl85Gr = gtk_label_new("");
  lblOn   = gtk_label_new("ON");

  gtk_widget_set_size_request(btn81, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn82, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn83, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn84, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn85, KEY_WIDTH_2, 0);

  gtk_widget_set_name(lbl81H, "gShiftedUnderline");
  gtk_widget_set_name(lbl83H, "gShiftedUnderline");
  gtk_widget_set_name(lbl84H, "fShifted");
  gtk_widget_set_name(lblOn,  "On");

  g_signal_connect(btn81, "clicked", G_CALLBACK(btnClicked), "32");
  g_signal_connect(btn82, "clicked", G_CALLBACK(btnClicked), "33");
  g_signal_connect(btn83, "clicked", G_CALLBACK(btnClicked), "34");
  g_signal_connect(btn84, "clicked", G_CALLBACK(btnClicked), "35");
  g_signal_connect(btn85, "clicked", G_CALLBACK(btnClicked), "36");

  gtk_fixed_put(GTK_FIXED(grid), lbl81F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl82F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl83F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl84F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl85F,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl81G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl82G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl83G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl84G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl85G,  0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl81Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl82Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl83Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl84Gr, 0, 0);
  gtk_fixed_put(GTK_FIXED(grid), lbl85Gr, 0, 0);

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_fixed_put(GTK_FIXED(grid), btn81,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl81L, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl81H, xPos + KEY_WIDTH_1 + X_OFFSET_LETTER, yPos -  1);

  xPos += DELTA_KEYS_X + 18;
  gtk_fixed_put(GTK_FIXED(grid), btn82,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl82L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn83,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl83L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl83H, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos -  1);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn84,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl84L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);
  gtk_fixed_put(GTK_FIXED(grid), lbl84H, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos -  1);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn85,  xPos,                         yPos);
  gtk_fixed_put(GTK_FIXED(grid), lbl85L, xPos + KEY_WIDTH_2 + X_OFFSET_LETTER, yPos + Y_OFFSET_LETTER);

  gtk_fixed_put(GTK_FIXED(grid), lblOn,   0, 0);

  // The debug window
  #if (DEBUG_PANEL == 1)
    for(int i=0; i<DEBUG_LINES; i++) {
      lbl1[i] = gtk_label_new("");
      gtk_widget_set_name(lbl1[i], "debugDejaVu");
      gtk_fixed_put(GTK_FIXED(grid), lbl1[i], 1 + debugWidgetDx, 26 + i*14 + debugWidgetDy);
      lbl2[i] = gtk_label_new("");
      gtk_widget_set_name(lbl2[i], "debugWP43S");
      gtk_fixed_put(GTK_FIXED(grid), lbl2[i], 270 + debugWidgetDx, 25 + i*14 + debugWidgetDy);
    }

    btnBitFields                 = gtk_button_new_with_label("Bitfields");
    btnFlags                     = gtk_button_new_with_label("Flags");
    btnRegisters                 = gtk_button_new_with_label("Registers");
    btnLocalRegisters            = gtk_button_new_with_label("Local registers");
    btnStatisticalSums           = gtk_button_new_with_label("Statistical sums");
    btnNamedRegisters            = gtk_button_new_with_label("Named registers");
    btnTmpAndSavedStackRegisters = gtk_button_new_with_label("Tmp & saved stack registers");
    chkHexaString                = gtk_check_button_new_with_label("Strings in hexadecimal form");

    gtk_widget_set_name(btnBitFields,                 "debugButton");
    gtk_widget_set_name(btnFlags,                     "debugButton");
    gtk_widget_set_name(btnRegisters,                 "debugButton");
    gtk_widget_set_name(btnLocalRegisters,            "debugButton");
    gtk_widget_set_name(btnStatisticalSums,           "debugButton");
    gtk_widget_set_name(btnNamedRegisters,            "debugButton");
    gtk_widget_set_name(btnTmpAndSavedStackRegisters, "debugButton");
    gtk_widget_set_name(chkHexaString,                "debugCheckbox");

    g_signal_connect(btnBitFields,                 "clicked", G_CALLBACK(btnBitFieldsClicked),                 NULL);
    g_signal_connect(btnFlags,                     "clicked", G_CALLBACK(btnFlagsClicked),                     NULL);
    g_signal_connect(btnRegisters,                 "clicked", G_CALLBACK(btnRegistersClicked),                 NULL);
    g_signal_connect(btnLocalRegisters,            "clicked", G_CALLBACK(btnLocalRegistersClicked),            NULL);
    g_signal_connect(btnStatisticalSums,           "clicked", G_CALLBACK(btnStatisticalSumsClicked),           NULL);
    g_signal_connect(btnNamedRegisters,            "clicked", G_CALLBACK(btnNamedRegistersClicked),            NULL);
    g_signal_connect(btnTmpAndSavedStackRegisters, "clicked", G_CALLBACK(btnTmpAndSavedStackRegistersClicked), NULL);
    g_signal_connect(chkHexaString,                "clicked", G_CALLBACK(chkHexaStringClicked),                NULL);

    gtk_fixed_put(GTK_FIXED(grid), btnBitFields,                   1 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), btnFlags,                      60 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), btnRegisters,                 101 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), btnLocalRegisters,            166 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), btnStatisticalSums,           260 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), btnNamedRegisters,            360 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), btnTmpAndSavedStackRegisters, 465 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), chkHexaString,                630 + debugWidgetDx, 1 + debugWidgetDy);

    gtk_widget_show(btnBitFields);
    gtk_widget_show(btnFlags);
    gtk_widget_show(btnRegisters);
    gtk_widget_show(btnLocalRegisters);
    gtk_widget_show(btnStatisticalSums);
    gtk_widget_show(btnNamedRegisters);
    gtk_widget_show(btnTmpAndSavedStackRegisters);
    gtk_widget_show(chkHexaString);

    debugWindow = DBG_REGISTERS;
  #endif

  gtk_widget_show_all(frmCalc);

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("setupUI");
  #endif
}



void saveCalc(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("saveCalc");
  #endif

  size_t size;

  FILE *backup;
  backup = fopen("backup.bin", "wb");
  if (backup == NULL) {
    printf("Cannot save calc's memory in file backup.bin!\n");
    exit(0);
  }

  if(calcMode == CM_CONFIRMATION) {
    calcMode = previousCalcMode;
    refreshRegisterLine(REGISTER_X);
  }

  printf("Begin of calc's backup\n");

  size  = fwrite(ram,                                 1, RAM_SIZE,                                   backup); //printf("%8lu ram\n",                                (unsigned long)size);
  size  = fwrite(flags,                               1, sizeof(flags),                              backup); //printf("%8lu flags\n",                              (unsigned long)size);
  size  = fwrite(tmpStr3000,                          1, TMP_STR_LENGTH,                             backup); //printf("%8lu tmpStr3000\n",                         (unsigned long)size);
  size  = fwrite(errorMessage,                        1, ERROR_MESSAGE_LENGTH,                       backup); //printf("%8lu errorMessage\n",                       (unsigned long)size);
  size  = fwrite(aimBuffer,                           1, AIM_BUFFER_LENGTH,                          backup); //printf("%8lu aimBuffer\n",                          (unsigned long)size);
  size  = fwrite(nimBuffer,                           1, NIM_BUFFER_LENGTH,                          backup); //printf("%8lu nimBuffer\n",                          (unsigned long)size);
  size  = fwrite(nimBufferDisplay,                    1, NIM_BUFFER_LENGTH,                          backup); //printf("%8lu nimBufferDisplay\n",                   (unsigned long)size);
  size  = fwrite(tamBuffer,                           1, TAM_BUFFER_LENGTH,                          backup); //printf("%8lu tamBuffer\n",                          (unsigned long)size);
  size  = fwrite(oldTime,                             1, 8,                                          backup); //printf("%8lu oldTime\n",                            (unsigned long)size);
  size  = fwrite(dateTimeString,                      1, 12,                                         backup); //printf("%8lu dateTimeString\n",                     (unsigned long)size);
  size  = fwrite(softmenuStack,                       1, sizeof(softmenuStack),                      backup); //printf("%8lu softmenuStack\n",                      (unsigned long)size);
  size  = fwrite(reg,                                 1, sizeof(reg),                                backup); //printf("%8lu reg\n",                                (unsigned long)size);
  size  = fwrite(savedStackRegister,                  1, sizeof(savedStackRegister),                 backup); //printf("%8lu savedStackRegister\n",                 (unsigned long)size);
  size  = fwrite(tempRegister,                        1, sizeof(tempRegister),                       backup); //printf("%8lu tempRegister\n",                       (unsigned long)size);
  size  = fwrite(kbd_usr,                             1, sizeof(kbd_usr),                            backup); //printf("%8lu kbd_usr\n",                            (unsigned long)size);
  size  = fwrite(row,                                 1, sizeof(row),                                backup); //printf("%8lu row\n",                                (unsigned long)size);
  size  = fwrite(transitionSystemOperation,           1, 4,                                          backup); //printf("%8lu transitionSystemOperation\n",          (unsigned long)size);
  size += fwrite(screenData,                          1, 4 * screenStride * SCREEN_HEIGHT,           backup); //printf("%8lu screenData\n",                         (unsigned long)size);
  size += fwrite(&tamFunction,                        1, sizeof(tamFunction),                        backup); //printf("%8lu tamFunction\n",                        (unsigned long)size);
  size += fwrite(&tamNumber,                          1, sizeof(tamNumber),                          backup); //printf("%8lu tamNumber\n",                          (unsigned long)size);
  size += fwrite(&tamNumberMin,                       1, sizeof(tamNumberMin),                       backup); //printf("%8lu tamNumberMin\n",                       (unsigned long)size);
  size += fwrite(&tamNumberMax,                       1, sizeof(tamNumberMax),                       backup); //printf("%8lu tamNumberMax\n",                       (unsigned long)size);
  size += fwrite(&tamDigit,                           1, sizeof(tamDigit),                           backup); //printf("%8lu tamDigit\n",                           (unsigned long)size);
  size += fwrite(&tamOperation,                       1, sizeof(tamOperation),                       backup); //printf("%8lu tamOperation\n",                       (unsigned long)size);
  size += fwrite(&tamLetteredRegister,                1, sizeof(tamLetteredRegister),                backup); //printf("%8lu tamLetteredRegister\n",                (unsigned long)size);
  size += fwrite(&tamCurrentOperation,                1, sizeof(tamCurrentOperation),                backup); //printf("%8lu tamCurrentOperation\n",                (unsigned long)size);
  size += fwrite(&result,                             1, sizeof(result),                             backup); //printf("%8lu result\n",                             (unsigned long)size);
  size += fwrite(&op1,                                1, sizeof(op1),                                backup); //printf("%8lu op1\n",                                (unsigned long)size);
  size += fwrite(&op2,                                1, sizeof(op2),                                backup); //printf("%8lu op2\n",                                (unsigned long)size);
  size += fwrite(&numberOfLocalRegisters,             1, sizeof(numberOfLocalRegisters),             backup); //printf("%8lu numberOfLocalRegisters\n",             (unsigned long)size);
  size += fwrite(&numberOfNamedRegisters,             1, sizeof(numberOfNamedRegisters),             backup); //printf("%8lu numberOfNamedRegisters\n",             (unsigned long)size);
  size += fwrite(&allLocalRegisterPointer,            1, sizeof(allLocalRegisterPointer),            backup); //printf("%8lu allLocalRegisterPointer\n",            (unsigned long)size);
  size += fwrite(&allNamedRegisterPointer,            1, sizeof(allNamedRegisterPointer),            backup); //printf("%8lu allNamedRegisterPointer\n",            (unsigned long)size);
  size += fwrite(&statisticalSumsPointer,             1, sizeof(statisticalSumsPointer),             backup); //printf("%8lu statisticalSumsPointer\n",             (unsigned long)size);
  size += fwrite(&firstFreeByte,                      1, sizeof(firstFreeByte),                      backup); //printf("%8lu firstFreeByte\n",                      (unsigned long)size);
  size += fwrite(&lastFreeByte,                       1, sizeof(lastFreeByte),                       backup); //printf("%8lu lastFreeByte\n",                       (unsigned long)size);
  size += fwrite(&programCounter,                     1, sizeof(programCounter),                     backup); //printf("%8lu programCounter\n",                     (unsigned long)size);
  size += fwrite(&xCursor,                            1, sizeof(xCursor),                            backup); //printf("%8lu xCursor\n",                            (unsigned long)size);
  size += fwrite(&yCursor,                            1, sizeof(yCursor),                            backup); //printf("%8lu yCursor\n",                            (unsigned long)size);
  size += fwrite(&firstGregorianDay,                  1, sizeof(firstGregorianDay),                  backup); //printf("%8lu firstGregorianDay\n",                  (unsigned long)size);
  size += fwrite(&denMax,                             1, sizeof(denMax),                             backup); //printf("%8lu denMax\n",                             (unsigned long)size);
  size += fwrite(&softmenuStackPointer,               1, sizeof(softmenuStackPointer),               backup); //printf("%8lu softmenuStackPointer\n",               (unsigned long)size);
  size += fwrite(&transitionSystemStatus,             1, sizeof(transitionSystemStatus),             backup); //printf("%8lu transitionSystemStatus\n",             (unsigned long)size);
  size += fwrite(&cursorBlinkCounter,                 1, sizeof(cursorBlinkCounter),                 backup); //printf("%8lu cursorBlinkCounter\n",                 (unsigned long)size);
  size += fwrite(&currentRegisterBrowserScreen,       1, sizeof(currentRegisterBrowserScreen),       backup); //printf("%8lu currentRegisterBrowserScreen\n",       (unsigned long)size);
  size += fwrite(&currentFntScr,                      1, sizeof(currentFntScr),                      backup); //printf("%8lu currentFntScr\n",                      (unsigned long)size);
  size += fwrite(&currentFlgScr,                      1, sizeof(currentFlgScr),                      backup); //printf("%8lu currentFlgScr\n",                      (unsigned long)size);
  size += fwrite(&displayFormat,                      1, sizeof(displayFormat),                      backup); //printf("%8lu displayFormat\n",                      (unsigned long)size);
  size += fwrite(&displayFormatDigits,                1, sizeof(displayFormatDigits),                backup); //printf("%8lu displayFormatDigits\n",                (unsigned long)size);
  size += fwrite(&smallIntegerWordSize,               1, sizeof(smallIntegerWordSize),               backup); //printf("%8lu smallIntegerWordSize\n",               (unsigned long)size);
  size += fwrite(&denominatorMode,                    1, sizeof(denominatorMode),                    backup); //printf("%8lu denominatorMode\n",                    (unsigned long)size);
  size += fwrite(&significantDigits,                  1, sizeof(significantDigits),                  backup); //printf("%8lu significantDigits\n",                  (unsigned long)size);
  size += fwrite(&smallIntegerMode,                   1, sizeof(smallIntegerMode),                   backup); //printf("%8lu smallIntegerMode\n",                   (unsigned long)size);
  size += fwrite(&angularMode,                        1, sizeof(angularMode),                        backup); //printf("%8lu angularMode\n",                        (unsigned long)size);
  size += fwrite(&groupingGap,                        1, sizeof(groupingGap),                        backup); //printf("%8lu groupingGap\n",                        (unsigned long)size);
  size += fwrite(&dateFormat,                         1, sizeof(dateFormat),                         backup); //printf("%8lu dateFormat\n",                         (unsigned long)size);
  size += fwrite(&curveFitting,                       1, sizeof(curveFitting),                       backup); //printf("%8lu curveFitting\n",                       (unsigned long)size);
  size += fwrite(&roundingMode,                       1, sizeof(roundingMode),                       backup); //printf("%8lu roundingMode\n",                       (unsigned long)size);
  size += fwrite(&calcMode,                           1, sizeof(calcMode),                           backup); //printf("%8lu calcMode\n",                           (unsigned long)size);
  size += fwrite(&nextChar,                           1, sizeof(nextChar),                           backup); //printf("%8lu nextChar\n",                           (unsigned long)size);
  size += fwrite(&complexUnit,                        1, sizeof(complexUnit),                        backup); //printf("%8lu complexUnit\n",                        (unsigned long)size);
  size += fwrite(&displayLeadingZeros,                1, sizeof(displayLeadingZeros),                backup); //printf("%8lu displayLeadingZeros\n",                (unsigned long)size);
  size += fwrite(&productSign,                        1, sizeof(productSign),                        backup); //printf("%8lu productSign\n",                        (unsigned long)size);
  size += fwrite(&fractionType,                       1, sizeof(fractionType),                       backup); //printf("%8lu fractionType\n",                       (unsigned long)size);
  size += fwrite(&radixMark,                          1, sizeof(radixMark),                          backup); //printf("%8lu radixMark\n",                          (unsigned long)size);
  size += fwrite(&displayModeOverride,                1, sizeof(displayModeOverride),                backup); //printf("%8lu displayModeOverride\n",                (unsigned long)size);
  size += fwrite(&stackSize,                          1, sizeof(stackSize),                          backup); //printf("%8lu stackSize\n",                          (unsigned long)size);
  size += fwrite(&complexMode,                        1, sizeof(complexMode),                        backup); //printf("%8lu complexMode\n",                        (unsigned long)size);
  size += fwrite(&alphaCase,                          1, sizeof(alphaCase),                          backup); //printf("%8lu alphaCase\n",                          (unsigned long)size);
  size += fwrite(&hourGlassIconEnabled,               1, sizeof(hourGlassIconEnabled),               backup); //printf("%8lu hourGlassIconEnabled\n",               (unsigned long)size);
  size += fwrite(&watchIconEnabled,                   1, sizeof(watchIconEnabled),                   backup); //printf("%8lu watchIconEnabled\n",                   (unsigned long)size);
  size += fwrite(&userModeEnabled,                    1, sizeof(userModeEnabled),                    backup); //printf("%8lu userModeEnabled\n",                    (unsigned long)size);
  size += fwrite(&serialIOIconEnabled,                1, sizeof(serialIOIconEnabled),                backup); //printf("%8lu serialIOIconEnabled\n",                (unsigned long)size);
  size += fwrite(&printerIconEnabled,                 1, sizeof(printerIconEnabled),                 backup); //printf("%8lu printerIconEnabled\n",                 (unsigned long)size);
  size += fwrite(&batteryIconEnabled,                 1, sizeof(batteryIconEnabled),                 backup); //printf("%8lu batteryIconEnabled\n",                 (unsigned long)size);
  size += fwrite(&cursorEnabled,                      1, sizeof(cursorEnabled),                      backup); //printf("%8lu cursorEnabled\n",                      (unsigned long)size);
  size += fwrite(&cursorFont,                         1, sizeof(cursorFont),                         backup); //printf("%8lu cursorFont\n",                         (unsigned long)size);
  size += fwrite(&stackLiftEnabled,                   1, sizeof(stackLiftEnabled),                   backup); //printf("%8lu stackLiftEnabled\n",                   (unsigned long)size);
  size += fwrite(&savedStackLiftEnabled,              1, sizeof(savedStackLiftEnabled),              backup); //printf("%8lu savedStackLiftEnabled\n",              (unsigned long)size);
  size += fwrite(&shiftF,                             1, sizeof(shiftF),                             backup); //printf("%8lu shiftF\n",                             (unsigned long)size);
  size += fwrite(&shiftG,                             1, sizeof(shiftG),                             backup); //printf("%8lu shiftG\n",                             (unsigned long)size);
  //size += fwrite(&displayDateTime,                    1, sizeof(displayDateTime),                    backup); //printf("%8lu displayDateTime\n",                    (unsigned long)size);
  size += fwrite(&timeFormat,                         1, sizeof(timeFormat),                         backup); //printf("%8lu timeFormat\n",                         (unsigned long)size);
  size += fwrite(&tamMode,                            1, sizeof(tamMode),                            backup); //printf("%8lu tamMode\n",                            (unsigned long)size);
  size += fwrite(&rbrMode,                            1, sizeof(rbrMode),                            backup); //printf("%8lu rbrMode\n",                            (unsigned long)size);
  size += fwrite(&showContent,                        1, sizeof(showContent),                        backup); //printf("%8lu showContent\n",                        (unsigned long)size);
  size += fwrite(&numScreensNumericFont,              1, sizeof(numScreensNumericFont),              backup); //printf("%8lu numScreensNumericFont\n",              (unsigned long)size);
  size += fwrite(&numLinesNumericFont,                1, sizeof(numLinesNumericFont),                backup); //printf("%8lu numLinesNumericFont\n",                (unsigned long)size);
  size += fwrite(&numLinesStandardFont,               1, sizeof(numLinesStandardFont),               backup); //printf("%8lu numLinesStandardFont\n",               (unsigned long)size);
  size += fwrite(&numScreensStandardFont,             1, sizeof(numScreensStandardFont),             backup); //printf("%8lu numScreensStandardFont\n",             (unsigned long)size);
  size += fwrite(&previousCalcMode,                   1, sizeof(previousCalcMode),                   backup); //printf("%8lu previousCalcMode\n",                   (unsigned long)size);
  size += fwrite(&displayRealAsFraction,              1, sizeof(displayRealAsFraction),              backup); //printf("%8lu displayRealAsFraction\n",              (unsigned long)size);
  size += fwrite(&lastErrorCode,                      1, sizeof(lastErrorCode),                      backup); //printf("%8lu lastErrorCode\n",                      (unsigned long)size);
  size += fwrite(&nimNumberPart,                      1, sizeof(nimNumberPart),                      backup); //printf("%8lu nimNumberPart\n",                      (unsigned long)size);
  size += fwrite(&displayStack,                       1, sizeof(displayStack),                       backup); //printf("%8lu displayStack\n",                       (unsigned long)size);
  size += fwrite(&hexDigits,                          1, sizeof(hexDigits),                          backup); //printf("%8lu hexDigits\n",                          (unsigned long)size);
  size += fwrite(&errorMessageRegisterLine,           1, sizeof(errorMessageRegisterLine),           backup); //printf("%8lu errorMessageRegisterLine\n",           (unsigned long)size);
  size += fwrite(&errorRegisterLine,                  1, sizeof(errorRegisterLine),                  backup); //printf("%8lu errorRegisterLine\n",                  (unsigned long)size);
  size += fwrite(&smallIntegerMask,                   1, sizeof(smallIntegerMask),                   backup); //printf("%8lu smallIntegerMask\n",                   (unsigned long)size);
  size += fwrite(&smallIntegerSignBit,                1, sizeof(smallIntegerSignBit),                backup); //printf("%8lu smallIntegerSignBit\n",                (unsigned long)size);
  size += fwrite(&temporaryInformation,               1, sizeof(temporaryInformation),               backup); //printf("%8lu temporaryInformation\n",               (unsigned long)size);
  size += fwrite(&glyphNotFound,                      1, sizeof(glyphNotFound),                      backup); //printf("%8lu glyphNotFound\n",                      (unsigned long)size);
  size += fwrite(&allowScreenUpdate,                  1, sizeof(allowScreenUpdate),                  backup); //printf("%8lu allowScreenUpdate\n",                  (unsigned long)size);
  size += fwrite(&funcOK,                             1, sizeof(funcOK),                             backup); //printf("%8lu funcOK\n",                             (unsigned long)size);
  size += fwrite(&screenChange,                       1, sizeof(screenChange),                       backup); //printf("%8lu screenChange\n",                       (unsigned long)size);
  size += fwrite(&exponentSignLocation,               1, sizeof(exponentSignLocation),               backup); //printf("%8lu exponentSignLocation\n",               (unsigned long)size);
  size += fwrite(&imaginaryExponentSignLocation,      1, sizeof(imaginaryExponentSignLocation),      backup); //printf("%8lu imaginaryExponentSignLocation\n",      (unsigned long)size);
  size += fwrite(&imaginaryMantissaSignLocation,      1, sizeof(imaginaryMantissaSignLocation),      backup); //printf("%8lu imaginaryMantissaSignLocation\n",      (unsigned long)size);

  printf("%" FMT32U " bytes saved\n", (uint32_t)size);

  fclose(backup);
  printf("End of calc's backup\n");

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("saveCalc");
  #endif
}



void restoreCalc(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("restoreCalc");
  #endif

  size_t size;

  FILE *backup;
  backup = fopen("backup.bin", "rb");
  if (backup == NULL) {
    printf("Cannot restore calc's memory from file backup.bin! Performing RESET\n");
    fnReset(CONFIRMED);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("restoreCalc");
    #endif

    return;
  }

  printf("Begin of calc's restoration\n");

  size  = fread(ram,                                 1, RAM_SIZE,                                   backup); //printf("%8lu ram\n",                                (unsigned long)size);
  size  = fread(flags,                               1, sizeof(flags),                              backup); //printf("%8lu flags\n",                              (unsigned long)size);
  size  = fread(tmpStr3000,                          1, TMP_STR_LENGTH,                             backup); //printf("%8lu tmpStr3000\n",                         (unsigned long)size);
  size  = fread(errorMessage,                        1, ERROR_MESSAGE_LENGTH,                       backup); //printf("%8lu errorMessage\n",                       (unsigned long)size);
  size  = fread(aimBuffer,                           1, AIM_BUFFER_LENGTH,                          backup); //printf("%8lu aimBuffer\n",                          (unsigned long)size);
  size  = fread(nimBuffer,                           1, NIM_BUFFER_LENGTH,                          backup); //printf("%8lu nimBuffer\n",                          (unsigned long)size);
  size  = fread(nimBufferDisplay,                    1, NIM_BUFFER_LENGTH,                          backup); //printf("%8lu nimBufferDisplay\n",                   (unsigned long)size);
  size  = fread(tamBuffer,                           1, TAM_BUFFER_LENGTH,                          backup); //printf("%8lu tamBuffer\n",                          (unsigned long)size);
  size  = fread(oldTime,                             1, 8,                                          backup); //printf("%8lu oldTime\n",                            (unsigned long)size);
  size  = fread(dateTimeString,                      1, 12,                                         backup); //printf("%8lu dateTimeString\n",                     (unsigned long)size);
  size  = fread(softmenuStack,                       1, sizeof(softmenuStack),                      backup); //printf("%8lu softmenuStack\n",                      (unsigned long)size);
  size  = fread(reg,                                 1, sizeof(reg),                                backup); //printf("%8lu reg\n",                                (unsigned long)size);
  size  = fread(savedStackRegister,                  1, sizeof(savedStackRegister),                 backup); //printf("%8lu savedStackRegister\n",                 (unsigned long)size);
  size  = fread(tempRegister,                        1, sizeof(tempRegister),                       backup); //printf("%8lu tempRegister\n",                       (unsigned long)size);
  size  = fread(kbd_usr,                             1, sizeof(kbd_usr),                            backup); //printf("%8lu kbd_usr\n",                            (unsigned long)size);
  size  = fread(row,                                 1, sizeof(row),                                backup); //printf("%8lu row\n",                                (unsigned long)size);
  size  = fread(transitionSystemOperation,           1, 4,                                          backup); //printf("%8lu transitionSystemOperation\n",          (unsigned long)size);
  size += fread(screenData,                          1, 4 * screenStride * SCREEN_HEIGHT,           backup); //printf("%8lu screenData\n",                         (unsigned long)size);
  size += fread(&tamFunction,                        1, sizeof(tamFunction),                        backup); //printf("%8lu tamFunction\n",                        (unsigned long)size);
  size += fread(&tamNumber    ,                      1, sizeof(tamNumber),                          backup); //printf("%8lu tamNumber\n",                          (unsigned long)size);
  size += fread(&tamNumberMin,                       1, sizeof(tamNumberMin),                       backup); //printf("%8lu tamNumberMin\n",                       (unsigned long)size);
  size += fread(&tamNumberMax,                       1, sizeof(tamNumberMax),                       backup); //printf("%8lu tamNumberMax\n",                       (unsigned long)size);
  size += fread(&tamDigit,                           1, sizeof(tamDigit),                           backup); //printf("%8lu tamDigit\n",                           (unsigned long)size);
  size += fread(&tamOperation,                       1, sizeof(tamOperation),                       backup); //printf("%8lu tamOperation\n",                       (unsigned long)size);
  size += fread(&tamLetteredRegister,                1, sizeof(tamLetteredRegister),                backup); //printf("%8lu tamLetteredRegister\n",                (unsigned long)size);
  size += fread(&tamCurrentOperation,                1, sizeof(tamCurrentOperation),                backup); //printf("%8lu tamCurrentOperation\n",                (unsigned long)size);
  size += fread(&result,                             1, sizeof(result),                             backup); //printf("%8lu result\n",                             (unsigned long)size);
  size += fread(&op1,                                1, sizeof(op1),                                backup); //printf("%8lu op1\n",                                (unsigned long)size);
  size += fread(&op2,                                1, sizeof(op2),                                backup); //printf("%8lu op2\n",                                (unsigned long)size);
  size += fread(&numberOfLocalRegisters,             1, sizeof(numberOfLocalRegisters),             backup); //printf("%8lu numberOfLocalRegisters\n",             (unsigned long)size);
  size += fread(&numberOfNamedRegisters,             1, sizeof(numberOfNamedRegisters),             backup); //printf("%8lu numberOfNamedRegisters\n",             (unsigned long)size);
  size += fread(&allLocalRegisterPointer,            1, sizeof(allLocalRegisterPointer),            backup); //printf("%8lu allLocalRegisterPointer\n",            (unsigned long)size);
  size += fread(&allNamedRegisterPointer,            1, sizeof(allNamedRegisterPointer),            backup); //printf("%8lu allNamedRegisterPointer\n",            (unsigned long)size);
  size += fread(&statisticalSumsPointer,             1, sizeof(statisticalSumsPointer),             backup); //printf("%8lu statisticalSumsPointer\n",             (unsigned long)size);
  size += fread(&firstFreeByte,                      1, sizeof(firstFreeByte),                      backup); //printf("%8lu firstFreeByte\n",                      (unsigned long)size);
  size += fread(&lastFreeByte,                       1, sizeof(lastFreeByte),                       backup); //printf("%8lu lastFreeByte\n",                       (unsigned long)size);
  size += fread(&programCounter,                     1, sizeof(programCounter),                     backup); //printf("%8lu programCounter\n",                     (unsigned long)size);
  size += fread(&xCursor,                            1, sizeof(xCursor),                            backup); //printf("%8lu xCursor\n",                            (unsigned long)size);
  size += fread(&yCursor,                            1, sizeof(yCursor),                            backup); //printf("%8lu yCursor\n",                            (unsigned long)size);
  size += fread(&firstGregorianDay,                  1, sizeof(firstGregorianDay),                  backup); //printf("%8lu firstGregorianDay\n",                  (unsigned long)size);
  size += fread(&denMax,                             1, sizeof(denMax),                             backup); //printf("%8lu denMax\n",                             (unsigned long)size);
  size += fread(&softmenuStackPointer,               1, sizeof(softmenuStackPointer),               backup); //printf("%8lu softmenuStackPointer\n",               (unsigned long)size);
  size += fread(&transitionSystemStatus,             1, sizeof(transitionSystemStatus),             backup); //printf("%8lu transitionSystemStatus\n",             (unsigned long)size);
  size += fread(&cursorBlinkCounter,                 1, sizeof(cursorBlinkCounter),                 backup); //printf("%8lu cursorBlinkCounter\n",                 (unsigned long)size);
  size += fread(&currentRegisterBrowserScreen,       1, sizeof(currentRegisterBrowserScreen),       backup); //printf("%8lu currentRegisterBrowserScreen\n",       (unsigned long)size);
  size += fread(&currentFntScr,                      1, sizeof(currentFntScr),                      backup); //printf("%8lu currentFntScr\n",                      (unsigned long)size);
  size += fread(&currentFlgScr,                      1, sizeof(currentFlgScr),                      backup); //printf("%8lu currentFlgScr\n",                      (unsigned long)size);
  size += fread(&displayFormat,                      1, sizeof(displayFormat),                      backup); //printf("%8lu displayFormat\n",                      (unsigned long)size);
  size += fread(&displayFormatDigits,                1, sizeof(displayFormatDigits),                backup); //printf("%8lu displayFormatDigits\n",                (unsigned long)size);
  size += fread(&smallIntegerWordSize,               1, sizeof(smallIntegerWordSize),               backup); //printf("%8lu smallIntegerWordSize\n",               (unsigned long)size);
  size += fread(&denominatorMode,                    1, sizeof(denominatorMode),                    backup); //printf("%8lu denominatorMode\n",                    (unsigned long)size);
  size += fread(&significantDigits,                  1, sizeof(significantDigits),                  backup); //printf("%8lu significantDigits\n",                  (unsigned long)size);
  size += fread(&smallIntegerMode,                   1, sizeof(smallIntegerMode),                   backup); //printf("%8lu smallIntegerMode\n",                   (unsigned long)size);
  size += fread(&angularMode,                        1, sizeof(angularMode),                        backup); //printf("%8lu angularMode\n",                        (unsigned long)size);
  size += fread(&groupingGap,                        1, sizeof(groupingGap),                        backup); //printf("%8lu groupingGap\n",                        (unsigned long)size);
  size += fread(&dateFormat,                         1, sizeof(dateFormat),                         backup); //printf("%8lu dateFormat\n",                         (unsigned long)size);
  size += fread(&curveFitting,                       1, sizeof(curveFitting),                       backup); //printf("%8lu curveFitting\n",                       (unsigned long)size);
  size += fread(&roundingMode,                       1, sizeof(roundingMode),                       backup); //printf("%8lu roundingMode\n",                       (unsigned long)size);
  size += fread(&calcMode,                           1, sizeof(calcMode),                           backup); //printf("%8lu calcMode\n",                           (unsigned long)size);
  size += fread(&nextChar,                           1, sizeof(nextChar),                           backup); //printf("%8lu nextChar\n",                           (unsigned long)size);
  size += fread(&complexUnit,                        1, sizeof(complexUnit),                        backup); //printf("%8lu complexUnit\n",                        (unsigned long)size);
  size += fread(&displayLeadingZeros,                1, sizeof(displayLeadingZeros),                backup); //printf("%8lu displayLeadingZeros\n",                (unsigned long)size);
  size += fread(&productSign,                        1, sizeof(productSign),                        backup); //printf("%8lu productSign\n",                        (unsigned long)size);
  size += fread(&fractionType,                       1, sizeof(fractionType),                       backup); //printf("%8lu fractionType\n",                       (unsigned long)size);
  size += fread(&radixMark,                          1, sizeof(radixMark),                          backup); //printf("%8lu radixMark\n",                          (unsigned long)size);
  size += fread(&displayModeOverride,                1, sizeof(displayModeOverride),                backup); //printf("%8lu displayModeOverride\n",                (unsigned long)size);
  size += fread(&stackSize,                          1, sizeof(stackSize),                          backup); //printf("%8lu stackSize\n",                          (unsigned long)size);
  size += fread(&complexMode,                        1, sizeof(complexMode),                        backup); //printf("%8lu complexMode\n",                        (unsigned long)size);
  size += fread(&alphaCase,                          1, sizeof(alphaCase),                          backup); //printf("%8lu alphaCase\n",                          (unsigned long)size);
  size += fread(&hourGlassIconEnabled,               1, sizeof(hourGlassIconEnabled),               backup); //printf("%8lu hourGlassIconEnabled\n",               (unsigned long)size);
  size += fread(&watchIconEnabled,                   1, sizeof(watchIconEnabled),                   backup); //printf("%8lu watchIconEnabled\n",                   (unsigned long)size);
  size += fread(&userModeEnabled,                    1, sizeof(userModeEnabled),                    backup); //printf("%8lu userModeEnabled\n",                    (unsigned long)size);
  size += fread(&serialIOIconEnabled,                1, sizeof(serialIOIconEnabled),                backup); //printf("%8lu serialIOIconEnabled\n",                (unsigned long)size);
  size += fread(&printerIconEnabled,                 1, sizeof(printerIconEnabled),                 backup); //printf("%8lu printerIconEnabled\n",                 (unsigned long)size);
  size += fread(&batteryIconEnabled,                 1, sizeof(batteryIconEnabled),                 backup); //printf("%8lu batteryIconEnabled\n",                 (unsigned long)size);
  size += fread(&cursorEnabled,                      1, sizeof(cursorEnabled),                      backup); //printf("%8lu cursorEnabled\n",                      (unsigned long)size);
  size += fread(&cursorFont,                         1, sizeof(cursorFont),                         backup); //printf("%8lu cursorFont\n",                         (unsigned long)size);
  size += fread(&stackLiftEnabled,                   1, sizeof(stackLiftEnabled),                   backup); //printf("%8lu stackLiftEnabled\n",                   (unsigned long)size);
  size += fread(&savedStackLiftEnabled,              1, sizeof(savedStackLiftEnabled),              backup); //printf("%8lu savedStackLiftEnabled\n",              (unsigned long)size);
  size += fread(&shiftF,                             1, sizeof(shiftF),                             backup); //printf("%8lu shiftF\n",                             (unsigned long)size);
  size += fread(&shiftG,                             1, sizeof(shiftG),                             backup); //printf("%8lu shiftG\n",                             (unsigned long)size);
  //size += fread(&displayDateTime,                    1, sizeof(displayDateTime),                    backup); //printf("%8lu displayDateTime\n",                    (unsigned long)size);
  size += fread(&timeFormat,                         1, sizeof(timeFormat),                         backup); //printf("%8lu timeFormat\n",                         (unsigned long)size);
  size += fread(&tamMode,                            1, sizeof(tamMode),                            backup); //printf("%8lu tamMode\n",                            (unsigned long)size);
  size += fread(&rbrMode,                            1, sizeof(rbrMode),                            backup); //printf("%8lu rbrMode\n",                            (unsigned long)size);
  size += fread(&showContent,                        1, sizeof(showContent),                        backup); //printf("%8lu showContent\n",                        (unsigned long)size);
  size += fread(&numScreensNumericFont,              1, sizeof(numScreensNumericFont),              backup); //printf("%8lu numScreensNumericFont\n",              (unsigned long)size);
  size += fread(&numLinesNumericFont,                1, sizeof(numLinesNumericFont),                backup); //printf("%8lu numLinesNumericFont\n",                (unsigned long)size);
  size += fread(&numLinesStandardFont,               1, sizeof(numLinesStandardFont),               backup); //printf("%8lu numLinesStandardFont\n",               (unsigned long)size);
  size += fread(&numScreensStandardFont,             1, sizeof(numScreensStandardFont),             backup); //printf("%8lu numScreensStandardFont\n",             (unsigned long)size);
  size += fread(&previousCalcMode,                   1, sizeof(previousCalcMode),                   backup); //printf("%8lu previousCalcMode\n",                   (unsigned long)size);
  size += fread(&displayRealAsFraction,              1, sizeof(displayRealAsFraction),              backup); //printf("%8lu displayRealAsFraction\n",              (unsigned long)size);
  size += fread(&lastErrorCode,                      1, sizeof(lastErrorCode),                      backup); //printf("%8lu lastErrorCode\n",                      (unsigned long)size);
  size += fread(&nimNumberPart,                      1, sizeof(nimNumberPart),                      backup); //printf("%8lu nimNumberPart\n",                      (unsigned long)size);
  size += fread(&displayStack,                       1, sizeof(displayStack),                       backup); //printf("%8lu displayStack\n",                       (unsigned long)size);
  size += fread(&hexDigits,                          1, sizeof(hexDigits),                          backup); //printf("%8lu hexDigits\n",                          (unsigned long)size);
  size += fread(&errorMessageRegisterLine,           1, sizeof(errorMessageRegisterLine),           backup); //printf("%8lu errorMessageRegisterLine\n",           (unsigned long)size);
  size += fread(&errorRegisterLine,                  1, sizeof(errorRegisterLine),                  backup); //printf("%8lu errorRegisterLine\n",                  (unsigned long)size);
  size += fread(&smallIntegerMask,                   1, sizeof(smallIntegerMask),                   backup); //printf("%8lu smallIntegerMask\n",                   (unsigned long)size);
  size += fread(&smallIntegerSignBit,                1, sizeof(smallIntegerSignBit),                backup); //printf("%8lu smallIntegerSignBit\n",                (unsigned long)size);
  size += fread(&temporaryInformation,               1, sizeof(temporaryInformation),               backup); //printf("%8lu temporaryInformation\n",               (unsigned long)size);

  size += fread(&glyphNotFound,                      1, sizeof(glyphNotFound),                      backup); //printf("%8lu glyphNotFound\n",                      (unsigned long)size);
  glyphNotFound.data   = malloc(38);
  strcpy(glyphNotFound.data, "\xff\xf8\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\x80\x08\xff\xf8");

  size += fread(&allowScreenUpdate,                  1, sizeof(allowScreenUpdate),                  backup); //printf("%8lu allowScreenUpdate\n",                  (unsigned long)size);
  size += fread(&funcOK,                             1, sizeof(funcOK),                             backup); //printf("%8lu funcOK\n",                             (unsigned long)size);
  size += fread(&screenChange,                       1, sizeof(screenChange),                       backup); //printf("%8lu screenChange\n",                       (unsigned long)size);
  size += fread(&exponentSignLocation,               1, sizeof(exponentSignLocation),               backup); //printf("%8lu exponentSignLocation\n",               (unsigned long)size);
  size += fread(&imaginaryExponentSignLocation,      1, sizeof(imaginaryExponentSignLocation),      backup); //printf("%8lu imaginaryExponentSignLocation\n",      (unsigned long)size);
  size += fread(&imaginaryMantissaSignLocation,      1, sizeof(imaginaryMantissaSignLocation),      backup); //printf("%8lu imaginaryMantissaSignLocation\n",      (unsigned long)size);

  printf("%" FMT32U " bytes restored\n", (uint32_t)size);

  fclose(backup);
  printf("End of calc's restoration\n");

  if(calcMode == CM_NORMAL)                calcModeNormalGui();
  else if(calcMode == CM_AIM)              calcModeAimGui();
  else if(calcMode == CM_TAM)              calcModeTamGui();
  else if(calcMode == CM_NIM)              calcModeNormalGui();
  else if(calcMode == CM_REGISTER_BROWSER) calcModeNormalGui();
  else if(calcMode == CM_FLAG_BROWSER)     calcModeNormalGui();
  else if(calcMode == CM_FONT_BROWSER)     calcModeNormalGui();
  else {
    sprintf(errorMessage, "In function restoreCalc: %" FMT8U " is an unexpected value for calcMode", calcMode);
    displayBugScreen(errorMessage);
  }

  getTimeString(dateTimeString);
  oldTime[0] = 0;

  if(stackLiftEnabled) {
    #if (STACK_LIFT_DEBUG == 1)
      stackLiftEnable();
    #else
      stackLiftEnabled = true;
    #endif
  }
  else {
    #if (STACK_LIFT_DEBUG == 1)
      stackLiftDisable();
    #else
      stackLiftEnabled = false;
    #endif
  }

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("restoreCalc");
  #endif
}
#endif


void fnOff(uint16_t unsuedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("fnOff");
  #endif

  resetShiftState();

  #ifdef PC_BUILD
    saveCalc();
    gtk_main_quit();
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("fnOff");
  #endif
}



/********************************************//**
 * \brief Sets the calc mode to normal
 *
 * \param void
 * \return void
 ***********************************************/
void calcModeNormal(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("calcModeNormal");
  #endif

  if(calcMode == CM_TAM) {
    popSoftmenu();
    #if (STACK_LIFT_DEBUG == 1)
      stackLiftEnable();
    #else
      stackLiftEnabled = true;
    #endif
  }

  calcMode = CM_NORMAL;
  hideCursor();
  cursorEnabled = false;

  #ifdef PC_BUILD
    calcModeNormalGui();
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("calcModeNormal");
  #endif
}



/********************************************//**
 * \brief Sets the calc mode to alpha input mode
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void calcModeAIM(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("calcModeAIM");
  #endif

  showSoftmenu(NULL, -MNU_MyAlpha, true);
  calcMode = CM_AIM;
  alphaCase = AC_UPPER;
  showAlphaMode();
  nextChar = NC_NORMAL;

  clearRegisterLine(Y_POSITION_OF_AIM_LINE, 32);
  xCursor = 1;
  yCursor = Y_POSITION_OF_AIM_LINE + 6;
  cursorFont = CF_STANDARD;
  cursorEnabled = true;

  #ifdef PC_BUILD
    calcModeAimGui();
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("calcModeAIM");
  #endif
}



/********************************************//**
 * \brief Sets the calc mode to number input mode
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void calcModeNIM(uint16_t unusedParamButMandatory) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("calcModeNIM");
  #endif

  saveStack();

  calcMode = CM_NIM;
  //nimTempRegister = allocateTemporaryRegister();

  liftStack(dtReal16, REAL16_SIZE);
  real16Zero(POINTER_TO_REGISTER_DATA(REGISTER_X));
  refreshStack();

  nimBuffer[0] = 0;
  hexDigits = 0;

  clearRegisterLine(Y_POSITION_OF_NIM_LINE, 32);
  xCursor = 1;
  yCursor = Y_POSITION_OF_NIM_LINE;
  cursorFont = CF_NUMERIC;
  cursorEnabled = true;

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("calcModeNIM");
  #endif
}



/********************************************//**
 * \brief Sets the calc mode to temporary alpha mode
 *
 * \param[in] tamMode int16_t    TAM mode
 *                               TAM       = general operation code
 *                               TAMCMP    = comparison operationcode
 *                               TAMSTORCL = STO or RCL operation code
 * \param[in] opCode const char* Operation code
 * \param[in] minN int16_t       Min value in TAM mode
 * \param[in] maxN int16_t       Max value in TAM mode
 * \return void
 ***********************************************/
void calcModeTAM(void) {
  #if (LOG_FUNCTIONS == 1)
    enteringFunction("calcModeTAM");
  #endif

  if(calcMode == CM_NIM) {
    closeNim();
  }

  if(tamMode == TM_NORMAL) {
    showSoftmenu(NULL, -MNU_TAM, true);
  }
  else if(tamMode == TM_CMP) {
    showSoftmenu(NULL, -MNU_TAMCMP, true);
  }
  else if(tamMode == TM_STO_RCL) {
    showSoftmenu(NULL, -MNU_TAMSTORCL, true);
  }
  else {
    sprintf(errorMessage, "In function calcModeTAM: %" FMT8U " is an unexpected value for tamMode!", tamMode);
    displayBugScreen(errorMessage);

    #if (LOG_FUNCTIONS == 1)
      leavingFunction("calcModeTAM");
    #endif

    return;
  }

  calcMode = CM_TAM;

  clearRegisterLine(Y_POSITION_OF_TAM_LINE, 32);

  strcat(tamBuffer, " __");
  showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE+6, vmNormal, true, true);

  transitionSystemStatus = 0;

  #ifdef PC_BUILD
    calcModeTamGui();
  #endif

  #if (LOG_FUNCTIONS == 1)
    leavingFunction("calcModeTAM");
  #endif
}
