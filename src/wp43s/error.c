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
 * \file error.c
 ***********************************************/

#include "wp43s.h"

const char *errorMessages[NUMBER_OF_ERROR_CODES] = {
/*  0 */  "No error",
/*  1 */  "An argument exceeds the function domain",
/*  2 */  "Bad time or date input",
/*  3 */  "Undefined op-code",
/*  4 */  "Overflow at +" STD_INFINITY,
/*  5 */  "Overflow at -" STD_INFINITY,
/*  6 */  "No such label found",
/*  7 */  "No such function",
/*  8 */  "Out of range",
/*  9 */  "Invalid integer input",
/* 10 */  "Input is too long",
/* 11 */  "RAM is full",
/* 12 */  "Stack clash",
/* 13 */  "Operation is undefined in this mode",
/* 14 */  "Word size is too small",
/* 15 */  "Too few data points for this statistic",
/* 16 */  "Invalid distribution parameter",
/* 17 */  "I/O error",
/* 18 */  "Invalid or corrupted data",
/* 19 */  "Flash memory is write protected",
/* 20 */  "No root found",
/* 21 */  "Matrix mismatch",
/* 22 */  "Singular matrix",
/* 23 */  "Flash memory is full",
/* 24 */  "Invalid input data type for this operation",
/* 25 */  "",
/* 26 */  "Please enter a NEW name",
/* 27 */  "Cannot delete a predefined item",
/* 28 */  "Bad input"
};



#ifdef PC_BUILD
/********************************************//**
 * \brief Displays an error message like a pop up
 *
 * \param[in] m1 const char* 1st part of the message
 * \param[in] m2 const char* 2nd part of the message
 * \param[in] m3 const char* 3rd part of the message
 * \param[in] m4 const char* 4th part of the message
 * \return void
 ***********************************************/
void showInfoDialog(const char *m1, const char *m2, const char *m3, const char *m4) {
  GtkWidget *dialog;
  uint8_t utf8m1[2000], utf8m2[2000], utf8m3[2000], utf8m4[2000];

  if(m1 == NULL) {
    stringToUtf8("No error message available!", utf8m1);
    dialog = gtk_message_dialog_new(GTK_WINDOW(frmCalc), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE, "%s", utf8m1);
  }
  else if(m2 == NULL) {
    stringToUtf8(m1, utf8m1);
    dialog = gtk_message_dialog_new(GTK_WINDOW(frmCalc), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE, "%s", utf8m1);
  }
  else if(m3 == NULL) {
    stringToUtf8(m1, utf8m1);
    stringToUtf8(m2, utf8m2);
    dialog = gtk_message_dialog_new(GTK_WINDOW(frmCalc), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE, "%s\n%s", utf8m1, utf8m2);
  }
  else if(m4 == NULL) {
    stringToUtf8(m1, utf8m1);
    stringToUtf8(m2, utf8m2);
    stringToUtf8(m3, utf8m3);
    dialog = gtk_message_dialog_new(GTK_WINDOW(frmCalc), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE, "%s\n%s\n%s", utf8m1, utf8m2, utf8m3);
  }
  else {
    stringToUtf8(m1, utf8m1);
    stringToUtf8(m2, utf8m2);
    stringToUtf8(m3, utf8m3);
    stringToUtf8(m4, utf8m4);
    dialog = gtk_message_dialog_new(GTK_WINDOW(frmCalc), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE, "%s\n%s\n%s\n%s", utf8m1, utf8m2, utf8m3, utf8m4);
  }

  gtk_widget_set_name(dialog, "errorBox");
  gtk_window_set_title(GTK_WINDOW(dialog), "WP 43S error message");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}
#endif


void displayCalcErrorMessage(uint8_t errorCode, calcRegister_t errMessageRegisterLine, calcRegister_t errRegisterLine) {
  if(errorCode >= NUMBER_OF_ERROR_CODES || errorCode <= 0) {
    sprintf(errorMessage, "In function displayCalcErrorMessage: %d is an unexpected value for errorCode!", errorCode);
    displayBugScreen(errorMessage);
  }

  else if(errMessageRegisterLine > REGISTER_T || errMessageRegisterLine < REGISTER_X) {
    sprintf(errorMessage, "In function displayCalcErrorMessage: %d is an unexpected value for errMessageRegisterLine! Must be from 100 (register X) to 103 (register T)", errMessageRegisterLine);
    displayBugScreen(errorMessage);
  }

  else if(errRegisterLine > REGISTER_T || errRegisterLine < REGISTER_X) {
    sprintf(errorMessage, "In function displayCalcErrorMessage: %d is an unexpected value for errRegisterLine! Must be from 100 (register X) to 103 (register T)", errRegisterLine);
    displayBugScreen(errorMessage);
  }

  else {
    lastErrorCode            = errorCode;
    errorMessageRegisterLine = errMessageRegisterLine;
    errorRegisterLine        = errRegisterLine;
    refreshRegisterLine(errMessageRegisterLine);
  }
}



void nextWord(const char *str, int16_t *pos, char *word) {
  int16_t i = 0;

  while(str[*pos] != 0 && str[*pos] != ' ') {
    word[i++] = str[(*pos)++];
  }

  word[i] = 0;

  while(str[*pos] != 0 && str[*pos] == ' ') {
    (*pos)++;
  }
}



/********************************************//**
 * \brief Displays an error message.
 *        When this happens it's likely a bug!
 *
 * \param[in] message char* The message
 * \return void
 ***********************************************/
void displayBugScreen(const char *msg) {
  if(calcMode != CM_BUG_ON_SCREEN) {
    int16_t y, pos;
    char line[100], word[50], message[1000];
    bool_t firstWordOfLine;

    previousCalcMode = calcMode;
    calcMode = CM_BUG_ON_SCREEN;
    cursorEnabled = false;
    hideCursor();

    clearScreen(false, true, true);

    y = 20;
    showString("This is most likely a bug in the firmware!", &standardFont, 1, y, vmNormal, true, false);
    y += 20;

    strcpy(message, msg);
    strcat(message, " Try to reproduce this and report a bug. Press EXIT to leave." );

    pos = 0;
    line[0] = 0;
    firstWordOfLine = true;

    nextWord(message, &pos, word);
    while(word[0] != 0) {
      if(stringWidth(line, &standardFont, true, true) + (firstWordOfLine ? 0 : 4) + stringWidth(word, &standardFont, true, true) >= SCREEN_WIDTH) { // 4 is the width of STD_SPACE_PUNCTUATION
        showString(line, &standardFont, 1, y, vmNormal, true, false);
        y += 20;
        line[0] = 0;
        firstWordOfLine = true;
      }

      if(firstWordOfLine) {
        strcpy(line, word);
        firstWordOfLine = false;
      }
      else {
        strcat(line, STD_SPACE_PUNCTUATION);
        strcat(line, word);
      }

      nextWord(message, &pos, word);
    }

    if(line[0] != 0) {
      showString(line, &standardFont, 1, y, vmNormal, true, false);
    }
  }
}
