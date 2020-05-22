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
GtkWidget *grid;
GtkWidget *backgroundImage, *bezelImage;
GtkWidget *lblFSoftkeyArea, *lblGSoftkeyArea;
GtkWidget *lblBehindScreen;

GtkWidget *btn11,   *btn12,   *btn13,   *btn14,   *btn15,   *btn16;
GtkWidget *btn21,   *btn22,   *btn23,   *btn24,   *btn25,   *btn26;
GtkWidget *btn31,   *btn32,   *btn33,   *btn34,   *btn35,   *btn36;
GtkWidget *btn41,   *btn42,   *btn43,   *btn44,   *btn45;
GtkWidget *btn51,   *btn52,   *btn53,   *btn54,   *btn55;
GtkWidget *btn61,   *btn62,   *btn63,   *btn64,   *btn65;
GtkWidget *btn71,   *btn72,   *btn73,   *btn74,   *btn75;
GtkWidget *btn81,   *btn82,   *btn83,   *btn84,   *btn85;

#if (DEBUG_PANEL == 1)
  GtkWidget *lbl1[DEBUG_LINES], *lbl2[DEBUG_LINES];
  GtkWidget *btnBitFields, *btnFlags, *btnRegisters, *btnLocalRegisters, *btnStatisticalSums, *btnNamedVariables, *btnSavedStackRegisters;
  GtkWidget *chkHexaString;
  int16_t debugWidgetDx, debugWidgetDy;
#endif

char *cssData;



static gint destroyCalc(GtkWidget* w, GdkEventAny* e, gpointer data) {
  saveCalc();
  gtk_main_quit();

  return 0;
}



gboolean keyPressed(GtkWidget *w, GdkEventKey *event, gpointer data) {
  //printf("%d\n", event->keyval);
  switch (event->keyval) {
    case 65470: // F1
      //printf("key pressed: F1\n");
      btnFnClicked(w, "1");
      break;

    case 65471: // F2
      //printf("key pressed: F2\n");
      btnFnClicked(w, "2");
      break;

    case 65472: // F3
      //printf("key pressed: F3\n");
      btnFnClicked(w, "3");
      break;

    case 65473: // F4
      //printf("key pressed: F4\n");
      btnFnClicked(w, "4");
      break;

    case 65474: // F5
      //printf("key pressed: F5\n");
      btnFnClicked(w, "5");
      break;

    case 65475: // F6
      //printf("key pressed: F6\n");
      btnFnClicked(w, "6");
      break;

    case 73:  // I
    case 105: // i
      //printf("key pressed: i 1/x\n");
      btnClicked(w, "00");
      break;

    case 89:  // Y
    case 121: // y
      //printf("key pressed: y y^x\n");
      btnClicked(w, "01");
      break;

    case 84:  // T
    case 116: // t
      //printf("key pressed: T TRI\n");
      btnClicked(w, "02");
      break;

    case 76:  // L
    case 108: // l
      //printf("key pressed: l ln\n");
      btnClicked(w, "03");
      break;

    case 101: // e
      //printf("key pressed: e e^x\n");
      btnClicked(w, "04");
      break;

    case 81:  // Q
    case 113: // q
      //printf("key pressed: Q Quadrad\n");
      btnClicked(w, "05");
      break;

    case 83:  // S
    case 115: // s
      //printf("key pressed: s STO\n");
      btnClicked(w, "06");
      break;

    case 82:  // R
    case 114: // r
      //printf("key pressed: r RCL\n");
      btnClicked(w, "07");
      break;

    case 65366: // PgDn
      //printf("key pressed: PgDn Roll down\n");
      btnClicked(w, "08");
      break;

    case 67: // C
      //printf("key pressed: C CC\n");
      btnClicked(w, "09");
      break;

    case 70:  // F
    case 102: // f
      //printf("key pressed: f\n");
      btnClicked(w, "10");
      break;

    case 71:  // G
    case 103: // g
      //printf("key pressed: g\n");
      btnClicked(w, "11");
      break;

    case 65293: // Enter
    case 65421: // Enter numeric pad
      //printf("key pressed: ENTER\n");
      btnClicked(w, "12");
      break;

    case 65289: // Tab
      //printf("key pressed: Tab x<>y\n");
      btnClicked(w, "13");
      break;

    case 99: // c
      //printf("key pressed: c CHS +/-\n");
      btnClicked(w, "14");
      break;

    case 69: // E
      //printf("key pressed: E\n");
      btnClicked(w, "15");
      break;

    case 65288: // Backspace
      //printf("key pressed: Backspace\n");
      btnClicked(w, "16");
      break;

    case 47:    // /
    case 65455: // /
      //printf("key pressed: /\n");
      btnClicked(w, "17");
      break;

    case 55:    // 7
    case 65463: // 7
      //printf("key pressed: 7\n");
      btnClicked(w, "18");
      break;

    case 56:    // 8
    case 65464: // 8
      //printf("key pressed: 8\n");
      btnClicked(w, "19");
      break;

    case 57:    // 9
    case 65465: // 9
      //printf("key pressed: 9\n");
      btnClicked(w, "20");
      break;

    case 88:  // X
      //printf("key pressed: XEQ\n");
      btnClicked(w, "21");
      break;

    case 42:    // *
    case 65450: // *
      //printf("key pressed: *\n");
      btnClicked(w, "22");
      break;

    case 52:    // 4
    case 65460: // 4
      //printf("key pressed: 4\n");
      btnClicked(w, "23");
      break;

    case 53:    // 5
    case 65461: // 5
      //printf("key pressed: 5\n");
      btnClicked(w, "24");
      break;

    case 54:    // 6
    case 65462: // 6
      //printf("key pressed: 6\n");
      btnClicked(w, "25");
      break;

    case 65362: // up arrow
      //printf("key pressed: up\n");
      btnClicked(w, "26");
      break;

    case 45:    // -
    case 65453: // -
      //printf("key pressed: -\n");
      btnClicked(w, "27");
      break;

    case 49:    // 1
    case 65457: // 1
      //printf("key pressed: 1\n");
      btnClicked(w, "28");
      break;

    case 50:    // 2
    case 65458: // 2
      //printf("key pressed: 2\n");
      btnClicked(w, "29");
      break;

    case 51:    // 3
    case 65459: // 3
      //printf("key pressed: 3\n");
      btnClicked(w, "30");
      break;

    case 65364: // down arrow
      //printf("key pressed: down\n");
      btnClicked(w, "31");
      break;

    case 43:    // +
    case 65451: // +
      //printf("key pressed: +\n");
      btnClicked(w, "32");
      break;

    case 48:    // 0
    case 65456: // 0
      //printf("key pressed: 0\n");
      btnClicked(w, "33");
      break;

    case 44:    // ,
    case 46:    // .
    case 65454: // .
      //printf("key pressed: .\n");
      btnClicked(w, "34");
      break;

    case 65507: // left Ctrl
    case 65508: // right Ctrl
      //printf("key pressed: Ctrl R/S\n");
      btnClicked(w, "35");
      break;

    case 65307: // Esc
      //printf("key pressed: EXIT\n");
      btnClicked(w, "36");
      break;

    case 72:  // H
    case 104: // h
      //printf("key pressed: h Hardcopy to clipboard\n");
      copyScreenToClipboard();
      break;

    case 120: // x
      //printf("key pressed: x copy register X to clipboard\n");
      copyRegisterXToClipboard();
      break;

    case 122: // z
      //printf("key pressed: z copy stack registers to clipboard\n");
      copyStackRegistersToClipboard();
      break;

    case 90: // Z
      //printf("key pressed: Z copy all registers to clipboard\n");
      copyAllRegistersToClipboard();
      break;

    default:
      break;
  }
  return FALSE;
}
#endif


#ifndef DMCP_BUILD
void strReplace(char *haystack, const char *needle, const char *newNeedle) {
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
    #ifdef PC_BUILD
      if(tmpString == NULL) {
        showInfoDialog("In function strReplace:", "error allocating memory for tmpString!", NULL, NULL);
        exit(1);
      }
    #endif

    strcpy(tmpString, needleLocation + needleLg);
    *strstr(haystack, needle) = 0;
    strcat(haystack, newNeedle);
    strcat(haystack, tmpString);
    free(tmpString);
  }
}
#endif


#ifdef PC_BUILD
/********************************************//**
 * \brief Reads the CSS file to configure the calc's GUI style
 *
 * \param void
 * \return void
 ***********************************************/
void prepareCssData(void) {
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
}



void labelCaptionNormal(const calcKey_t *key, GtkWidget *button) {
  if(key->primary == KEY_f) {
    gtk_widget_set_name(button, "calcKeyF");
  }
  else if(key->primary == KEY_g) {
    gtk_widget_set_name(button, "calcKeyG");
  }
  else {
    gtk_widget_set_name(button, "calcKey");
  }
}



void labelCaptionAim(const calcKey_t *key, GtkWidget *button) {
  if(key->keyLblAim == KEY_f) {
    gtk_widget_set_name(button, "calcKeyF");
  }
  else if(key->keyLblAim == KEY_g) {
    gtk_widget_set_name(button, "calcKeyG");
  }
  else {
    if((key->fShiftedAim == key->keyLblAim || key->fShiftedAim == CHR_PROD_SIGN) && key->keyLblAim != ITM_NULL) {
      gtk_widget_set_name(button, "calcKeyGoldenBorder");
    }
    else {
      gtk_widget_set_name(button, "calcKey");
    }
  }
}



void calcModeNormalGui(void) {
  const calcKey_t *keys;

  keys = kbd_std;

  labelCaptionNormal(keys++, btn21);
  labelCaptionNormal(keys++, btn22);
  labelCaptionNormal(keys++, btn23);
  labelCaptionNormal(keys++, btn24);
  labelCaptionNormal(keys++, btn25);
  labelCaptionNormal(keys++, btn26);

  labelCaptionNormal(keys++, btn31);
  labelCaptionNormal(keys++, btn32);
  labelCaptionNormal(keys++, btn33);
  labelCaptionNormal(keys++, btn34);
  labelCaptionNormal(keys++, btn35);
  labelCaptionNormal(keys++, btn36);

  labelCaptionNormal(keys++, btn41);
  labelCaptionNormal(keys++, btn42);
  labelCaptionNormal(keys++, btn43);
  labelCaptionNormal(keys++, btn44);
  labelCaptionNormal(keys++, btn45);

  labelCaptionNormal(keys++, btn51);
  labelCaptionNormal(keys++, btn52);
  labelCaptionNormal(keys++, btn53);
  labelCaptionNormal(keys++, btn54);
  labelCaptionNormal(keys++, btn55);

  labelCaptionNormal(keys++, btn61);
  labelCaptionNormal(keys++, btn62);
  labelCaptionNormal(keys++, btn63);
  labelCaptionNormal(keys++, btn64);
  labelCaptionNormal(keys++, btn65);

  labelCaptionNormal(keys++, btn71);
  labelCaptionNormal(keys++, btn72);
  labelCaptionNormal(keys++, btn73);
  labelCaptionNormal(keys++, btn74);
  labelCaptionNormal(keys++, btn75);

  labelCaptionNormal(keys++, btn81);
  labelCaptionNormal(keys++, btn82);
  labelCaptionNormal(keys++, btn83);
  labelCaptionNormal(keys++, btn84);
  labelCaptionNormal(keys++, btn85);

  gtk_image_set_from_file((GtkImage *)bezelImage, "artwork/bezel_Normal.png");

  gtk_button_set_image(GTK_BUTTON(btn21), gtk_image_new_from_file("artwork/key_21_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn22), gtk_image_new_from_file("artwork/key_22_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn23), gtk_image_new_from_file("artwork/key_23_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn24), gtk_image_new_from_file("artwork/key_24_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn25), gtk_image_new_from_file("artwork/key_25_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn26), gtk_image_new_from_file("artwork/key_26_normal.png"));

  gtk_button_set_image(GTK_BUTTON(btn31), gtk_image_new_from_file("artwork/key_31_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn32), gtk_image_new_from_file("artwork/key_32_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn33), gtk_image_new_from_file("artwork/key_33_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn34), gtk_image_new_from_file("artwork/key_34_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn35), gtk_image_new_from_file("artwork/key_35_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn36), gtk_image_new_from_file("artwork/key_36_normal.png"));

  gtk_button_set_image(GTK_BUTTON(btn41), gtk_image_new_from_file("artwork/key_41_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn42), gtk_image_new_from_file("artwork/key_42_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn43), gtk_image_new_from_file("artwork/key_43_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn44), gtk_image_new_from_file("artwork/key_44_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn45), gtk_image_new_from_file("artwork/key_45_normal.png"));

  gtk_button_set_image(GTK_BUTTON(btn51), gtk_image_new_from_file("artwork/key_51_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn52), gtk_image_new_from_file("artwork/key_52_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn53), gtk_image_new_from_file("artwork/key_53_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn54), gtk_image_new_from_file("artwork/key_54_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn55), gtk_image_new_from_file("artwork/key_55_normal.png"));

  gtk_button_set_image(GTK_BUTTON(btn61), gtk_image_new_from_file("artwork/key_61_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn62), gtk_image_new_from_file("artwork/key_62_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn63), gtk_image_new_from_file("artwork/key_63_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn64), gtk_image_new_from_file("artwork/key_64_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn65), gtk_image_new_from_file("artwork/key_65_normal.png"));

  gtk_button_set_image(GTK_BUTTON(btn71), gtk_image_new_from_file("artwork/key_71_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn72), gtk_image_new_from_file("artwork/key_72_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn73), gtk_image_new_from_file("artwork/key_73_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn74), gtk_image_new_from_file("artwork/key_74_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn75), gtk_image_new_from_file("artwork/key_75_normal.png"));

  gtk_button_set_image(GTK_BUTTON(btn81), gtk_image_new_from_file("artwork/key_81_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn82), gtk_image_new_from_file("artwork/key_82_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn83), gtk_image_new_from_file("artwork/key_83_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn84), gtk_image_new_from_file("artwork/key_84_normal.png"));
  gtk_button_set_image(GTK_BUTTON(btn85), gtk_image_new_from_file("artwork/key_85_normal.png"));
}



void calcModeAimGui(void) {
  const calcKey_t *keys;

  keys = kbd_std;

  labelCaptionAim(keys++, btn21);
  labelCaptionAim(keys++, btn22);
  labelCaptionAim(keys++, btn23);
  labelCaptionAim(keys++, btn24);
  labelCaptionAim(keys++, btn25);
  labelCaptionAim(keys++, btn26);

  labelCaptionAim(keys++, btn31);
  labelCaptionAim(keys++, btn32);
  labelCaptionAim(keys++, btn33);
  labelCaptionAim(keys++, btn34);
  labelCaptionAim(keys++, btn35);
  labelCaptionAim(keys++, btn36);

  labelCaptionAim(keys++, btn41);
  labelCaptionAim(keys++, btn42);
  labelCaptionAim(keys++, btn43);
  labelCaptionAim(keys++, btn44);
  labelCaptionAim(keys++, btn45);

  labelCaptionAim(keys++, btn51);
  labelCaptionAim(keys++, btn52);
  labelCaptionAim(keys++, btn53);
  labelCaptionAim(keys++, btn54);
  labelCaptionAim(keys++, btn55);

  labelCaptionAim(keys++, btn61);
  labelCaptionAim(keys++, btn62);
  labelCaptionAim(keys++, btn63);
  labelCaptionAim(keys++, btn64);
  labelCaptionAim(keys++, btn65);

  labelCaptionAim(keys++, btn71);
  labelCaptionAim(keys++, btn72);
  labelCaptionAim(keys++, btn73);
  labelCaptionAim(keys++, btn74);
  labelCaptionAim(keys++, btn75);

  labelCaptionAim(keys++, btn81);
  labelCaptionAim(keys++, btn82);
  labelCaptionAim(keys++, btn83);
  labelCaptionAim(keys++, btn84);
  labelCaptionAim(keys++, btn85);

  gtk_image_set_from_file((GtkImage *)bezelImage, "artwork/bezel_AIM.png");

  gtk_button_set_image(GTK_BUTTON(btn21), gtk_image_new_from_file("artwork/key_empty.png"));
  gtk_button_set_image(GTK_BUTTON(btn22), gtk_image_new_from_file("artwork/key_22_AIM.png"));
  gtk_button_set_image(GTK_BUTTON(btn23), gtk_image_new_from_file("artwork/key_empty.png"));
  gtk_button_set_image(GTK_BUTTON(btn24), gtk_image_new_from_file("artwork/key_empty.png"));
  gtk_button_set_image(GTK_BUTTON(btn25), gtk_image_new_from_file("artwork/key_empty.png"));
  gtk_button_set_image(GTK_BUTTON(btn26), gtk_image_new_from_file("artwork/key_26_AIM.png"));

  gtk_button_set_image(GTK_BUTTON(btn31), gtk_image_new_from_file("artwork/key_empty.png"));
  gtk_button_set_image(GTK_BUTTON(btn32), gtk_image_new_from_file("artwork/key_empty.png"));
  gtk_button_set_image(GTK_BUTTON(btn33), gtk_image_new_from_file("artwork/key_empty.png"));
  gtk_button_set_image(GTK_BUTTON(btn34), gtk_image_new_from_file("artwork/key_empty.png"));

  gtk_button_set_image(GTK_BUTTON(btn42), gtk_image_new_from_file("artwork/key_42_AIM.png"));
  gtk_button_set_image(GTK_BUTTON(btn43), gtk_image_new_from_file("artwork/key_43_AIM.png"));
  gtk_button_set_image(GTK_BUTTON(btn44), gtk_image_new_from_file("artwork/key_empty.png"));

  gtk_button_set_image(GTK_BUTTON(btn55), gtk_image_new_from_file("artwork/key_empty.png"));

  gtk_button_set_image(GTK_BUTTON(btn84), gtk_image_new_from_file("artwork/key_empty.png"));
}



void calcModeTamGui(void) {
  const calcKey_t *keys;

  keys = kbd_std;

  labelCaptionNormal(keys++, btn21);
  labelCaptionNormal(keys++, btn22);
  labelCaptionNormal(keys++, btn23);
  labelCaptionNormal(keys++, btn24);
  labelCaptionNormal(keys++, btn25);
  labelCaptionNormal(keys++, btn26);

  labelCaptionNormal(keys++, btn31);
  labelCaptionNormal(keys++, btn32);
  labelCaptionNormal(keys++, btn33);
  labelCaptionNormal(keys++, btn34);
  labelCaptionNormal(keys++, btn35);
  labelCaptionNormal(keys++, btn36);

  labelCaptionNormal(keys++, btn41);
  labelCaptionNormal(keys++, btn42);
  labelCaptionNormal(keys++, btn43);
  labelCaptionNormal(keys++, btn44);
  labelCaptionNormal(keys++, btn45);

  labelCaptionNormal(keys++, btn51);
  labelCaptionNormal(keys++, btn52);
  labelCaptionNormal(keys++, btn53);
  labelCaptionNormal(keys++, btn54);
  labelCaptionNormal(keys++, btn55);

  labelCaptionNormal(keys++, btn61);
  labelCaptionNormal(keys++, btn62);
  labelCaptionNormal(keys++, btn63);
  labelCaptionNormal(keys++, btn64);
  labelCaptionNormal(keys++, btn65);

  labelCaptionNormal(keys++, btn71);
  labelCaptionNormal(keys++, btn72);
  labelCaptionNormal(keys++, btn73);
  labelCaptionNormal(keys++, btn74);
  labelCaptionNormal(keys++, btn75);

  labelCaptionNormal(keys++, btn81);
  labelCaptionNormal(keys++, btn82);
  labelCaptionNormal(keys++, btn83);
  labelCaptionNormal(keys++, btn84);
  labelCaptionNormal(keys++, btn85);

  gtk_image_set_from_file((GtkImage *)bezelImage, "artwork/bezel_TAM.png");

  gtk_button_set_image(GTK_BUTTON(btn21), gtk_image_new_from_file("artwork/key_21_TAM.png"));
  gtk_button_set_image(GTK_BUTTON(btn22), gtk_image_new_from_file("artwork/key_22_TAM.png"));
  gtk_button_set_image(GTK_BUTTON(btn23), gtk_image_new_from_file("artwork/key_23_TAM.png"));
  gtk_button_set_image(GTK_BUTTON(btn24), gtk_image_new_from_file("artwork/key_24_TAM.png"));
  gtk_button_set_image(GTK_BUTTON(btn25), gtk_image_new_from_file("artwork/key_empty.png"));
  gtk_button_set_image(GTK_BUTTON(btn26), gtk_image_new_from_file("artwork/key_26_TAM.png"));

  gtk_button_set_image(GTK_BUTTON(btn31), gtk_image_new_from_file("artwork/key_empty.png"));
  gtk_button_set_image(GTK_BUTTON(btn32), gtk_image_new_from_file("artwork/key_empty.png"));
  gtk_button_set_image(GTK_BUTTON(btn33), gtk_image_new_from_file("artwork/key_33_TAM.png"));
  gtk_button_set_image(GTK_BUTTON(btn34), gtk_image_new_from_file("artwork/key_34_TAM.png"));

  gtk_button_set_image(GTK_BUTTON(btn42), gtk_image_new_from_file("artwork/key_42_TAM.png"));
  gtk_button_set_image(GTK_BUTTON(btn43), gtk_image_new_from_file("artwork/key_43_TAM.png"));
  gtk_button_set_image(GTK_BUTTON(btn44), gtk_image_new_from_file("artwork/key_empty.png"));

  gtk_button_set_image(GTK_BUTTON(btn55), gtk_image_new_from_file("artwork/key_empty.png"));

  gtk_button_set_image(GTK_BUTTON(btn84), gtk_image_new_from_file("artwork/key_empty.png"));
}



void configureCallback(GtkWindow *window, GdkEvent *event, gpointer data) {
  allowScreenUpdate = false;
  //printf("x=%d y=%d\n", event->configure.x, event->configure.y);
 }



/********************************************//**
 * \brief Creates the calc's GUI window with all the widgets
 *
 * \param void
 * \return void
 ***********************************************/
void setupUI(void) {
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
    calcLandscape = (monitor.height < 1025);
  }

  // The main window
  frmCalc = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  if(calcLandscape) {
    #if (DEBUG_PANEL == 1)
      gtk_window_set_default_size(GTK_WINDOW(frmCalc), 1000, 1530);
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
    backgroundImage = gtk_image_new_from_file("artwork/dm42lshort.png");
  }
  else {
    backgroundImage = gtk_image_new_from_file("artwork/dm42l.png");
  }

  gtk_fixed_put(GTK_FIXED(grid), backgroundImage, 0, 0);

  bezelImage = gtk_image_new_from_file("artwork/normal.png");
  gtk_fixed_put(GTK_FIXED(grid), bezelImage, (calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT) - 15, calcLandscape ? 4 : 424);

  // Areas for the g shifted softkeys
  lblGSoftkeyArea = gtk_label_new("");
  gtk_widget_set_name(lblGSoftkeyArea, "gSoftkeyArea");
  gtk_widget_set_size_request(lblGSoftkeyArea, 438, 24);
  gtk_fixed_put(GTK_FIXED(grid), lblGSoftkeyArea, 44, 72+170);



  // Area for the f shifted softkeys
  lblFSoftkeyArea = gtk_label_new("");
  gtk_widget_set_name(lblFSoftkeyArea, "fSoftkeyArea");
  gtk_widget_set_size_request(lblFSoftkeyArea, 438, 24);
  gtk_fixed_put(GTK_FIXED(grid), lblFSoftkeyArea, 44, 72+170+24);



  // Behind screen
  lblBehindScreen = gtk_label_new("");
  gtk_widget_set_name(lblBehindScreen, "behindScreen");
  gtk_widget_set_size_request(lblBehindScreen, 412, 252);
  gtk_fixed_put(GTK_FIXED(grid), lblBehindScreen, 57, 66);



  // LCD screen 400x240
  screen = gtk_drawing_area_new();
  gtk_widget_set_size_request(screen, SCREEN_WIDTH, SCREEN_HEIGHT);
  gtk_widget_set_tooltip_text(GTK_WIDGET(screen), "Copy to clipboard:\nh -> screen image\nx -> register X\nz -> lettered registers\nZ -> all registers");
  gtk_fixed_put(GTK_FIXED(grid), screen, 63, 72);
  screenStride = cairo_format_stride_for_width(CAIRO_FORMAT_RGB24, SCREEN_WIDTH)/4;
  numBytes = screenStride * SCREEN_HEIGHT * 4;
  screenData = malloc(numBytes);
  if(screenData == NULL) {
    sprintf(errorMessage, "error allocating %d x %d = %d bytes for screenData", screenStride * 4, SCREEN_HEIGHT, numBytes);
    showInfoDialog("In function setupUI:", errorMessage, NULL, NULL);
    exit(1);
  }

  clearScreen(true, true, true);
  g_signal_connect(screen, "draw", G_CALLBACK(drawScreen), NULL);


  #if (DEBUG_REGISTER_L == 1)
    lblRegisterL1 = gtk_label_new("");
    lblRegisterL2 = gtk_label_new("");
    gtk_widget_set_name(lblRegisterL1, "registerL");
    gtk_widget_set_name(lblRegisterL2, "registerL");
    gtk_fixed_put(GTK_FIXED(grid), lblRegisterL1, 5, 28);
    gtk_fixed_put(GTK_FIXED(grid), lblRegisterL2, 5, 46);
  #endif

  #if (SHOW_MEMORY_STATUS == 1)
    lblMemoryStatus = gtk_label_new("");
    gtk_widget_set_name(lblMemoryStatus, "memoryStatus");
    gtk_fixed_put(GTK_FIXED(grid), lblMemoryStatus, 5, 5);
  #endif

  // 1st row: F1 to F6 buttons
  btn11 = gtk_button_new_with_label("");
  btn12 = gtk_button_new_with_label("");
  btn13 = gtk_button_new_with_label("");
  btn14 = gtk_button_new_with_label("");
  btn15 = gtk_button_new_with_label("");
  btn16 = gtk_button_new_with_label("");

  gtk_widget_set_tooltip_text(GTK_WIDGET(btn11), "F1");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn12), "F2");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn13), "F3");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn14), "F4");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn15), "F5");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn16), "F6");

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

  g_signal_connect(btn11, "pressed", G_CALLBACK(btnFnPressed), "1");
  g_signal_connect(btn12, "pressed", G_CALLBACK(btnFnPressed), "2");
  g_signal_connect(btn13, "pressed", G_CALLBACK(btnFnPressed), "3");
  g_signal_connect(btn14, "pressed", G_CALLBACK(btnFnPressed), "4");
  g_signal_connect(btn15, "pressed", G_CALLBACK(btnFnPressed), "5");
  g_signal_connect(btn16, "pressed", G_CALLBACK(btnFnPressed), "6");
  g_signal_connect(btn11, "released", G_CALLBACK(btnFnReleased), "1");
  g_signal_connect(btn12, "released", G_CALLBACK(btnFnReleased), "2");
  g_signal_connect(btn13, "released", G_CALLBACK(btnFnReleased), "3");
  g_signal_connect(btn14, "released", G_CALLBACK(btnFnReleased), "4");
  g_signal_connect(btn15, "released", G_CALLBACK(btnFnReleased), "5");
  g_signal_connect(btn16, "released", G_CALLBACK(btnFnReleased), "6");

  gtk_widget_set_focus_on_click(btn11, FALSE);
  gtk_widget_set_focus_on_click(btn12, FALSE);
  gtk_widget_set_focus_on_click(btn13, FALSE);
  gtk_widget_set_focus_on_click(btn14, FALSE);
  gtk_widget_set_focus_on_click(btn15, FALSE);
  gtk_widget_set_focus_on_click(btn16, FALSE);

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
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn21), "i");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn22), "y");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn23), "t");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn24), "l");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn25), "e");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn26), "q");

  gtk_widget_set_size_request(btn21, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn22, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn23, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn24, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn25, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn26, KEY_WIDTH_1, 0);

  g_signal_connect(btn21, "pressed", G_CALLBACK(btnPressed), "00");
  g_signal_connect(btn22, "pressed", G_CALLBACK(btnPressed), "01");
  g_signal_connect(btn23, "pressed", G_CALLBACK(btnPressed), "02");
  g_signal_connect(btn24, "pressed", G_CALLBACK(btnPressed), "03");
  g_signal_connect(btn25, "pressed", G_CALLBACK(btnPressed), "04");
  g_signal_connect(btn26, "pressed", G_CALLBACK(btnPressed), "05");
  g_signal_connect(btn21, "released", G_CALLBACK(btnReleased), "00");
  g_signal_connect(btn22, "released", G_CALLBACK(btnReleased), "01");
  g_signal_connect(btn23, "released", G_CALLBACK(btnReleased), "02");
  g_signal_connect(btn24, "released", G_CALLBACK(btnReleased), "03");
  g_signal_connect(btn25, "released", G_CALLBACK(btnReleased), "04");
  g_signal_connect(btn26, "released", G_CALLBACK(btnReleased), "05");

  if(calcLandscape) {
    xPos = X_LEFT_LANDSCAPE;
    yPos = Y_TOP_LANDSCAPE;
  }
  else {
    xPos = X_LEFT_PORTRAIT;
    yPos += DELTA_KEYS_Y;
  }

  gtk_fixed_put(GTK_FIXED(grid), btn21, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn22, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn23, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn24, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn25, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn26, xPos, yPos);



  // 3rd row
  btn31   = gtk_button_new();
  btn32   = gtk_button_new();
  btn33   = gtk_button_new();
  btn34   = gtk_button_new();
  btn35   = gtk_button_new();
  btn36   = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn31), "s");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn32), "r");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn33), "Page Down");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn34), "upper C");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn35), "f");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn36), "g");

  gtk_widget_set_size_request(btn31, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn32, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn33, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn34, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn35, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn36, KEY_WIDTH_1, 0);

  g_signal_connect(btn31, "pressed", G_CALLBACK(btnPressed), "06");
  g_signal_connect(btn32, "pressed", G_CALLBACK(btnPressed), "07");
  g_signal_connect(btn33, "pressed", G_CALLBACK(btnPressed), "08");
  g_signal_connect(btn34, "pressed", G_CALLBACK(btnPressed), "09");
  g_signal_connect(btn35, "pressed", G_CALLBACK(btnPressed), "10");
  g_signal_connect(btn36, "pressed", G_CALLBACK(btnPressed), "11");
  g_signal_connect(btn31, "released", G_CALLBACK(btnReleased), "06");
  g_signal_connect(btn32, "released", G_CALLBACK(btnReleased), "07");
  g_signal_connect(btn33, "released", G_CALLBACK(btnReleased), "08");
  g_signal_connect(btn34, "released", G_CALLBACK(btnReleased), "09");
  g_signal_connect(btn35, "released", G_CALLBACK(btnReleased), "10");
  g_signal_connect(btn36, "released", G_CALLBACK(btnReleased), "11");

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y;
  gtk_fixed_put(GTK_FIXED(grid), btn31, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn32, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn33, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn34, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn35, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn36, xPos, yPos);



  // 4th row
  btn41   = gtk_button_new();
  btn42   = gtk_button_new();
  btn43   = gtk_button_new();
  btn44   = gtk_button_new();
  btn45   = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn41), "Enter");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn42), "Tab");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn43), "c");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn44), "E");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn45), "Backspace");

  gtk_widget_set_size_request(btn41, KEY_WIDTH_1 + DELTA_KEYS_X, 0);
  gtk_widget_set_size_request(btn42, KEY_WIDTH_1,              0);
  gtk_widget_set_size_request(btn43, KEY_WIDTH_1,              0);
  gtk_widget_set_size_request(btn44, KEY_WIDTH_1,              0);
  gtk_widget_set_size_request(btn45, KEY_WIDTH_1,              0);

  g_signal_connect(btn41, "pressed", G_CALLBACK(btnPressed), "12");
  g_signal_connect(btn42, "pressed", G_CALLBACK(btnPressed), "13");
  g_signal_connect(btn43, "pressed", G_CALLBACK(btnPressed), "14");
  g_signal_connect(btn44, "pressed", G_CALLBACK(btnPressed), "15");
  g_signal_connect(btn45, "pressed", G_CALLBACK(btnPressed), "16");
  g_signal_connect(btn41, "released", G_CALLBACK(btnReleased), "12");
  g_signal_connect(btn42, "released", G_CALLBACK(btnReleased), "13");
  g_signal_connect(btn43, "released", G_CALLBACK(btnReleased), "14");
  g_signal_connect(btn44, "released", G_CALLBACK(btnReleased), "15");
  g_signal_connect(btn45, "released", G_CALLBACK(btnReleased), "16");

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y;
  gtk_fixed_put(GTK_FIXED(grid), btn41, xPos, yPos);

  xPos += DELTA_KEYS_X*2;
  gtk_fixed_put(GTK_FIXED(grid), btn42, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn43, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn44, xPos, yPos);

  xPos += DELTA_KEYS_X;
  gtk_fixed_put(GTK_FIXED(grid), btn45, xPos, yPos);



  // 5th row
  btn51   = gtk_button_new();
  btn52   = gtk_button_new();
  btn53   = gtk_button_new();
  btn54   = gtk_button_new();
  btn55   = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn51), "/");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn52), "7");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn53), "8");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn54), "9");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn55), "upper X");

  gtk_widget_set_size_request(btn51, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn52, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn53, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn54, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn55, KEY_WIDTH_2, 0);

  g_signal_connect(btn51, "pressed", G_CALLBACK(btnPressed), "17");
  g_signal_connect(btn52, "pressed", G_CALLBACK(btnPressed), "18");
  g_signal_connect(btn53, "pressed", G_CALLBACK(btnPressed), "19");
  g_signal_connect(btn54, "pressed", G_CALLBACK(btnPressed), "20");
  g_signal_connect(btn55, "pressed", G_CALLBACK(btnPressed), "21");
  g_signal_connect(btn51, "released", G_CALLBACK(btnReleased), "17");
  g_signal_connect(btn52, "released", G_CALLBACK(btnReleased), "18");
  g_signal_connect(btn53, "released", G_CALLBACK(btnReleased), "19");
  g_signal_connect(btn54, "released", G_CALLBACK(btnReleased), "20");
  g_signal_connect(btn55, "released", G_CALLBACK(btnReleased), "21");

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_fixed_put(GTK_FIXED(grid), btn51, xPos, yPos);

  xPos += DELTA_KEYS_X + 18;
  gtk_fixed_put(GTK_FIXED(grid), btn52, xPos, yPos);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn53, xPos, yPos);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn54, xPos, yPos);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn55, xPos, yPos);



  // 6th row
  btn61   = gtk_button_new();
  btn62   = gtk_button_new();
  btn63   = gtk_button_new();
  btn64   = gtk_button_new();
  btn65   = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn61), "*");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn62), "4");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn63), "5");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn64), "6");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn65), "Up");

  gtk_widget_set_size_request(btn61, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn62, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn63, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn64, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn65, KEY_WIDTH_2, 0);

  g_signal_connect(btn61, "pressed", G_CALLBACK(btnPressed), "22");
  g_signal_connect(btn62, "pressed", G_CALLBACK(btnPressed), "23");
  g_signal_connect(btn63, "pressed", G_CALLBACK(btnPressed), "24");
  g_signal_connect(btn64, "pressed", G_CALLBACK(btnPressed), "25");
  g_signal_connect(btn65, "pressed", G_CALLBACK(btnPressed), "26");
  g_signal_connect(btn61, "released", G_CALLBACK(btnReleased), "22");
  g_signal_connect(btn62, "released", G_CALLBACK(btnReleased), "23");
  g_signal_connect(btn63, "released", G_CALLBACK(btnReleased), "24");
  g_signal_connect(btn64, "released", G_CALLBACK(btnReleased), "25");
  g_signal_connect(btn65, "released", G_CALLBACK(btnReleased), "26");

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_fixed_put(GTK_FIXED(grid), btn61, xPos, yPos);

  xPos += DELTA_KEYS_X + 18;
  gtk_fixed_put(GTK_FIXED(grid), btn62, xPos, yPos);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn63, xPos, yPos);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn64, xPos, yPos);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn65, xPos, yPos);



  // 7th row
  btn71   = gtk_button_new();
  btn72   = gtk_button_new();
  btn73   = gtk_button_new();
  btn74   = gtk_button_new();
  btn75   = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn71), "-");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn72), "1");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn73), "2");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn74), "3");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn75), "Down");

  gtk_widget_set_size_request(btn71, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn72, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn73, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn74, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn75, KEY_WIDTH_2, 0);

  g_signal_connect(btn71, "pressed", G_CALLBACK(btnPressed), "27");
  g_signal_connect(btn72, "pressed", G_CALLBACK(btnPressed), "28");
  g_signal_connect(btn73, "pressed", G_CALLBACK(btnPressed), "29");
  g_signal_connect(btn74, "pressed", G_CALLBACK(btnPressed), "30");
  g_signal_connect(btn75, "pressed", G_CALLBACK(btnPressed), "31");
  g_signal_connect(btn71, "released", G_CALLBACK(btnReleased), "27");
  g_signal_connect(btn72, "released", G_CALLBACK(btnReleased), "28");
  g_signal_connect(btn73, "released", G_CALLBACK(btnReleased), "29");
  g_signal_connect(btn74, "released", G_CALLBACK(btnReleased), "30");
  g_signal_connect(btn75, "released", G_CALLBACK(btnReleased), "31");

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_fixed_put(GTK_FIXED(grid), btn71, xPos, yPos);

  xPos += DELTA_KEYS_X + 18;
  gtk_fixed_put(GTK_FIXED(grid), btn72, xPos, yPos);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn73, xPos, yPos);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn74, xPos, yPos);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn75, xPos, yPos);



  // 8th row
  btn81   = gtk_button_new();
  btn82   = gtk_button_new();
  btn83   = gtk_button_new();
  btn84   = gtk_button_new();
  btn85   = gtk_button_new();
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn81), "+");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn82), "0");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn83), ". ,");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn84), "Ctrl");
  gtk_widget_set_tooltip_text(GTK_WIDGET(btn85), "Esc");

  gtk_widget_set_size_request(btn81, KEY_WIDTH_1, 0);
  gtk_widget_set_size_request(btn82, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn83, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn84, KEY_WIDTH_2, 0);
  gtk_widget_set_size_request(btn85, KEY_WIDTH_2, 0);

  g_signal_connect(btn81, "pressed", G_CALLBACK(btnPressed), "32");
  g_signal_connect(btn82, "pressed", G_CALLBACK(btnPressed), "33");
  g_signal_connect(btn83, "pressed", G_CALLBACK(btnPressed), "34");
  g_signal_connect(btn84, "pressed", G_CALLBACK(btnPressed), "35");
  g_signal_connect(btn85, "pressed", G_CALLBACK(btnPressed), "36");
  g_signal_connect(btn81, "released", G_CALLBACK(btnReleased), "32");
  g_signal_connect(btn82, "released", G_CALLBACK(btnReleased), "33");
  g_signal_connect(btn83, "released", G_CALLBACK(btnReleased), "34");
  g_signal_connect(btn84, "released", G_CALLBACK(btnReleased), "35");
  g_signal_connect(btn85, "released", G_CALLBACK(btnReleased), "36");

  xPos = calcLandscape ? X_LEFT_LANDSCAPE : X_LEFT_PORTRAIT;

  yPos += DELTA_KEYS_Y + 1;
  gtk_fixed_put(GTK_FIXED(grid), btn81, xPos, yPos);

  xPos += DELTA_KEYS_X + 18;
  gtk_fixed_put(GTK_FIXED(grid), btn82, xPos, yPos);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn83, xPos, yPos);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn84, xPos, yPos);

  xPos += DELTA_KEYS_X + 17;
  gtk_fixed_put(GTK_FIXED(grid), btn85, xPos, yPos);

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

    btnBitFields           = gtk_button_new_with_label("Bitfields");
    btnFlags               = gtk_button_new_with_label("Flags");
    btnRegisters           = gtk_button_new_with_label("Registers");
    btnLocalRegisters      = gtk_button_new_with_label("Local registers");
    btnStatisticalSums     = gtk_button_new_with_label("Statistical sums");
    btnNamedVariables      = gtk_button_new_with_label("Named variables");
    btnSavedStackRegisters = gtk_button_new_with_label("Saved stack registers");
    chkHexaString          = gtk_check_button_new_with_label("Strings in hexadecimal form");

    gtk_widget_set_name(btnBitFields,           "debugButton");
    gtk_widget_set_name(btnFlags,               "debugButton");
    gtk_widget_set_name(btnRegisters,           "debugButton");
    gtk_widget_set_name(btnLocalRegisters,      "debugButton");
    gtk_widget_set_name(btnStatisticalSums,     "debugButton");
    gtk_widget_set_name(btnNamedVariables,      "debugButton");
    gtk_widget_set_name(btnSavedStackRegisters, "debugButton");
    gtk_widget_set_name(chkHexaString,          "debugCheckbox");

    g_signal_connect(btnBitFields,           "clicked", G_CALLBACK(btnBitFieldsClicked),           NULL);
    g_signal_connect(btnFlags,               "clicked", G_CALLBACK(btnFlagsClicked),               NULL);
    g_signal_connect(btnRegisters,           "clicked", G_CALLBACK(btnRegistersClicked),           NULL);
    g_signal_connect(btnLocalRegisters,      "clicked", G_CALLBACK(btnLocalRegistersClicked),      NULL);
    g_signal_connect(btnStatisticalSums,     "clicked", G_CALLBACK(btnStatisticalSumsClicked),     NULL);
    g_signal_connect(btnNamedVariables,      "clicked", G_CALLBACK(btnNamedVariablesClicked),      NULL);
    g_signal_connect(btnSavedStackRegisters, "clicked", G_CALLBACK(btnSavedStackRegistersClicked), NULL);
    g_signal_connect(chkHexaString,          "clicked", G_CALLBACK(chkHexaStringClicked),          NULL);

    gtk_fixed_put(GTK_FIXED(grid), btnBitFields,             1 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), btnFlags,                60 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), btnRegisters,           101 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), btnLocalRegisters,      166 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), btnStatisticalSums,     260 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), btnNamedVariables,      360 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), btnSavedStackRegisters, 465 + debugWidgetDx, 1 + debugWidgetDy);
    gtk_fixed_put(GTK_FIXED(grid), chkHexaString,          630 + debugWidgetDx, 1 + debugWidgetDy);

    gtk_widget_show(btnBitFields);
    gtk_widget_show(btnFlags);
    gtk_widget_show(btnRegisters);
    gtk_widget_show(btnLocalRegisters);
    gtk_widget_show(btnStatisticalSums);
    gtk_widget_show(btnNamedVariables);
    gtk_widget_show(btnSavedStackRegisters);
    gtk_widget_show(chkHexaString);

    debugWindow = DBG_REGISTERS;
  #endif

  gtk_widget_show_all(frmCalc);
}
#endif



#ifndef TESTSUITE_BUILD
void fnOff(uint16_t unsuedParamButMandatory) {
  resetShiftState();

  #ifdef PC_BUILD
    saveCalc();
    gtk_main_quit();
  #endif

  #ifdef DMCP_BUILD
    SET_ST(STAT_PGM_END);
  #endif
}



/********************************************//**
 * \brief Sets the calc mode to normal
 *
 * \param void
 * \return void
 ***********************************************/
void calcModeNormal(void) {
  if(calcMode == CM_TAM || calcMode == CM_ASM_OVER_TAM) {
    popSoftmenu();
    refreshRegisterLine(TAM_REGISTER_LINE);
    STACK_LIFT_ENABLE;
  }

  calcMode = CM_NORMAL;
  clearSystemFlag(FLAG_ALPHA);
  hideCursor();
  cursorEnabled = false;
  showAlphaMode();

  #ifdef PC_BUILD
    calcModeNormalGui();
  #endif
}



/********************************************//**
 * \brief Sets the calc mode to alpha input mode
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void calcModeAim(uint16_t unusedParamButMandatory) {
  if(calcMode == CM_ASM_OVER_AIM) {
    popSoftmenu();
    calcMode = CM_AIM;
  }
  else {
    if(softmenuStackPointer == 0) {
      showSoftmenu(NULL, -MNU_MyAlpha, false);
    }
    alphaCase = AC_UPPER;
    calcMode = CM_AIM;
    showAlphaMode();
    nextChar = NC_NORMAL;

    saveStack();
    liftStack();
    refreshStack();

    clearRegisterLine(Y_POSITION_OF_AIM_LINE - 4, REGISTER_LINE_HEIGHT);
    xCursor = 1;
    yCursor = Y_POSITION_OF_AIM_LINE + 6;
    cursorFont = CF_STANDARD;
    cursorEnabled = true;
  }

  setSystemFlag(FLAG_ALPHA);

  #ifdef PC_BUILD
    calcModeAimGui();
  #endif
}



/********************************************//**
 * \brief Sets the calc mode to alpha selection menu
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void calcModeAsm(void) {
  if(calcMode == CM_NIM) {
    closeNim();
  }

  if(calcMode != CM_AIM) {
    alphaCase = AC_UPPER;
    showAlphaMode();
    nextChar = NC_NORMAL;
  }

  calcMode = CM_ASM;
  clearSystemFlag(FLAG_ALPHA);
  resetAlphaSelectionBuffer();

  #ifdef PC_BUILD
    calcModeAimGui();
  #endif
}



/********************************************//**
 * \brief Sets the calc mode to number input mode
 *
 * \param[in] unusedParamButMandatory uint16_t
 * \return void
 ***********************************************/
void calcModeNim(uint16_t unusedParamButMandatory) {
  saveStack();

  calcMode = CM_NIM;
  clearSystemFlag(FLAG_ALPHA);

  liftStack();
  real34Zero(REGISTER_REAL34_DATA(REGISTER_X));
  refreshStack();

  nimBuffer[0] = 0;
  hexDigits = 0;

  clearRegisterLine(Y_POSITION_OF_NIM_LINE - 4, REGISTER_LINE_HEIGHT);
  xCursor = 1;
  yCursor = Y_POSITION_OF_NIM_LINE;
  cursorFont = CF_NUMERIC;
  cursorEnabled = true;
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
void calcModeTam(void) {
  if(calcMode == CM_NIM) {
    closeNim();
  }
  else if(calcMode == CM_ASM_OVER_TAM) {
    popSoftmenu();
  }

  if(tamMode == TM_VALUE || tamMode == TM_VALUE_CHB || tamMode == TM_REGISTER) {
    showSoftmenu(NULL, -MNU_TAM, true);
  }
  else if(tamMode == TM_CMP) {
    showSoftmenu(NULL, -MNU_TAMCMP, true);
  }
  else if(tamMode == TM_FLAGR || tamMode == TM_FLAGW) {
    showSoftmenu(NULL, -MNU_TAMFLAG, true);
  }
  else if(tamMode == TM_STORCL) {
    showSoftmenu(NULL, -MNU_TAMSTORCL, true);
  }
  else {
    sprintf(errorMessage, "In function calcModeTam: %" FMT8U " is an unexpected value for tamMode!", tamMode);
    displayBugScreen(errorMessage);
    return;
  }

  calcMode = CM_TAM;
  clearSystemFlag(FLAG_ALPHA);

  strcat(tamBuffer, " __");
  if(stringWidth(tamBuffer, &standardFont, true, true) + 1 + lineTWidth > SCREEN_WIDTH) {
    clearRegisterLine(Y_POSITION_OF_REGISTER_T_LINE - 4, REGISTER_LINE_HEIGHT);
  }
  showString(tamBuffer, &standardFont, 25, Y_POSITION_OF_TAM_LINE + 6, vmNormal, true, true);

  transitionSystemState = 0;
  tamCurrentOperation = 0;

  #ifdef PC_BUILD
    calcModeTamGui();
  #endif
}
#endif
