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

#include "error.h"

#include "charString.h"
#include "constantPointers.h"
#include "debug.h"
#include "flags.h"
#include "fonts.h"
#include "mathematics/comparisonReals.h"
#include "realType.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "screen.h"
#include <string.h>

#include "wp43.h"

TO_QSPI static const char commonBugScreenMessage00[] = "In function %s:%d is an unexpected value for %s!";
TO_QSPI static const char commonBugScreenMessage01[] = "In function %s: unexpected calcMode value (%" PRIu8 ") while processing key %s!";
TO_QSPI static const char commonBugScreenMessage02[] = "In function reallocateRegister: %" PRIu16 " is an unexpected numByte value for %s! It should be %s=%" PRIu16 "!";
TO_QSPI static const char commonBugScreenMessage03[] = "In function %s: no named variables defined!";
TO_QSPI static const char commonBugScreenMessage04[] = "In function %s: %d is an unexpected value returned by findGlyph!";
TO_QSPI static const char commonBugScreenMessage05[] = "In function %s: %" PRIu32 " is an unexpected %s value!";
TO_QSPI static const char commonBugScreenMessage06[] = "In function %s: data type %s is unknown!";
TO_QSPI static const char commonBugScreenMessage07[] = "In function %s: regist=%" PRId16 " must be less than %d!";
TO_QSPI static const char commonBugScreenMessage08[] = "In function %s: %s %" PRId16 " is not defined! Must be from 0 to %" PRIu16;
TO_QSPI static const char commonBugScreenMessage09[] = "In function %s: unexpected case while processing key %s! %" PRIu8 " is an unexpected value for rbrMode.";

const char *commonBugScreenMessages[NUMBER_OF_BUG_SCREEN_MESSAGES] = {
  commonBugScreenMessage00,
  commonBugScreenMessage01,
  commonBugScreenMessage02,
  commonBugScreenMessage03,
  commonBugScreenMessage04,
  commonBugScreenMessage05,
  commonBugScreenMessage06,
  commonBugScreenMessage07,
  commonBugScreenMessage08,
  commonBugScreenMessage09,
};

TO_QSPI static const char errorMessage00[] = "No error";
TO_QSPI static const char errorMessage01[] = "An argument exceeds the function domain";
TO_QSPI static const char errorMessage02[] = "Bad time or date input";
TO_QSPI static const char errorMessage03[] = "Undefined op-code";
TO_QSPI static const char errorMessage04[] = "Overflow at +" STD_INFINITY;
TO_QSPI static const char errorMessage05[] = "Overflow at -" STD_INFINITY;
TO_QSPI static const char errorMessage06[] = "No such label found";
TO_QSPI static const char errorMessage07[] = "No such function";
TO_QSPI static const char errorMessage08[] = "Out of range";
TO_QSPI static const char errorMessage09[] = "Illegal digit in integer input for this base";
TO_QSPI static const char errorMessage10[] = "Input is too long";
TO_QSPI static const char errorMessage11[] = "RAM is full";
TO_QSPI static const char errorMessage12[] = "Stack clash";
TO_QSPI static const char errorMessage13[] = "Operation is undefined in this mode";
TO_QSPI static const char errorMessage14[] = "Word size is too small";
TO_QSPI static const char errorMessage15[] = "Too few data points for this statistic";
TO_QSPI static const char errorMessage16[] = "Distribution parameter out of valid range";
TO_QSPI static const char errorMessage17[] = "I/O error";
TO_QSPI static const char errorMessage18[] = "Invalid or corrupted data";
TO_QSPI static const char errorMessage19[] = "Flash memory is write protected";
TO_QSPI static const char errorMessage20[] = "No root found";
TO_QSPI static const char errorMessage21[] = "Matrix mismatch";
TO_QSPI static const char errorMessage22[] = "Singular matrix";
TO_QSPI static const char errorMessage23[] = "Flash memory is full";
TO_QSPI static const char errorMessage24[] = "Invalid input data type for this operation";
TO_QSPI static const char errorMessage25[] = "";
TO_QSPI static const char errorMessage26[] = "Please enter a NEW name";
TO_QSPI static const char errorMessage27[] = "Cannot delete a predefined item";
TO_QSPI static const char errorMessage28[] = "No statistic data present";
TO_QSPI static const char errorMessage29[] = "Item to be coded";
TO_QSPI static const char errorMessage30[] = "Function to be coded for that data type";
TO_QSPI static const char errorMessage31[] = "Input data types do not match";
TO_QSPI static const char errorMessage32[] = "This system flag is write protected";
TO_QSPI static const char errorMessage33[] = "Output would exceed 196 characters";
TO_QSPI static const char errorMessage34[] = "This does not work with an empty string";
TO_QSPI static const char errorMessage35[] = "No backup data found";
TO_QSPI static const char errorMessage36[] = "Undefined source variable";
TO_QSPI static const char errorMessage37[] = "This variable is write protected";
TO_QSPI static const char errorMessage38[] = "No matrix indexed";
TO_QSPI static const char errorMessage39[] = "Not enough memory for such a matrix";
TO_QSPI static const char errorMessage40[] = "No errors for selected model";
TO_QSPI static const char errorMessage41[] = "Large " STD_DELTA " and opposite signs, may be a pole";
TO_QSPI static const char errorMessage42[] = "Solver reached local extremum, no root";
TO_QSPI static const char errorMessage43[] = STD_GREATER_EQUAL "1 initial guess lies out of the domain";
TO_QSPI static const char errorMessage44[] = "The function value look static constant";
TO_QSPI static const char errorMessage45[] = "Syntax error in this equation";
TO_QSPI static const char errorMessage46[] = "This equation formula is too complex";
TO_QSPI static const char errorMessage47[] = "This item cannot be assigned here";
TO_QSPI static const char errorMessage48[] = "Invalid name";
TO_QSPI static const char errorMessage49[] = "Too many variables";
TO_QSPI static const char errorMessage50[] = "Non-programmable command, please remove";
TO_QSPI static const char errorMessage51[] = "No global label in this program";
TO_QSPI static const char errorMessage52[] = "Bad input"; // This error is not in ReM and cannot occur (theoretically).
const char *errorMessages[NUMBER_OF_ERROR_CODES] = {
  errorMessage00,
  errorMessage01,
  errorMessage02,
  errorMessage03,
  errorMessage04,
  errorMessage05,
  errorMessage06,
  errorMessage07,
  errorMessage08,
  errorMessage09,
  errorMessage10,
  errorMessage11,
  errorMessage12,
  errorMessage13,
  errorMessage14,
  errorMessage15,
  errorMessage16,
  errorMessage17,
  errorMessage18,
  errorMessage19,
  errorMessage20,
  errorMessage21,
  errorMessage22,
  errorMessage23,
  errorMessage24,
  errorMessage25,
  errorMessage26,
  errorMessage27,
  errorMessage28,
  errorMessage29,
  errorMessage30,
  errorMessage31,
  errorMessage32,
  errorMessage33,
  errorMessage34,
  errorMessage35,
  errorMessage36,
  errorMessage37,
  errorMessage38,
  errorMessage39,
  errorMessage40,
  errorMessage41,
  errorMessage42,
  errorMessage43,
  errorMessage44,
  errorMessage45,
  errorMessage46,
  errorMessage47,
  errorMessage48,
  errorMessage49,
  errorMessage50,
  errorMessage51,
  errorMessage52,
};


#if defined(PC_BUILD)
  /********************************************//**
   * \brief Displays an error message like a pop up
   *
   * \param[in] m1 const char* 1st part of the message
   * \param[in] m2 const char* 2nd part of the message
   * \param[in] m3 const char* 3rd part of the message
   * \param[in] m4 const char* 4th part of the message
   * \return void
   ***********************************************/
  void moreInfoOnError(const char *m1, const char *m2, const char *m3, const char *m4) {
    uint8_t utf8m1[2000], utf8m2[2000], utf8m3[2000], utf8m4[2000];

    if(m1 == NULL) {
      stringToUtf8("No error message available!", utf8m1);
      printf("\n%s\n", utf8m1);
    }
    else if(m2 == NULL) {
      stringToUtf8(m1, utf8m1);
      printf("\n%s\n\n", utf8m1);
    }
    else if(m3 == NULL) {
      stringToUtf8(m1, utf8m1);
      stringToUtf8(m2, utf8m2);
      printf("\n%s\n%s\n\n", utf8m1, utf8m2);
    }
    else if(m4 == NULL) {
      stringToUtf8(m1, utf8m1);
      stringToUtf8(m2, utf8m2);
      stringToUtf8(m3, utf8m3);
      printf("\n%s\n%s\n%s\n\n", utf8m1, utf8m2, utf8m3);
    }
    else {
      stringToUtf8(m1, utf8m1);
      stringToUtf8(m2, utf8m2);
      stringToUtf8(m3, utf8m3);
      stringToUtf8(m4, utf8m4);
      printf("\n%s\n%s\n%s\n%s\n\n", utf8m1, utf8m2, utf8m3, utf8m4);
    }
  }
#endif // PC_BUILD



void fnRaiseError(uint16_t errorCode) {
  displayCalcErrorMessage((uint8_t)errorCode, ERR_REGISTER_LINE, REGISTER_X);
}



void fnErrorMessage(uint16_t unusedButMandatoryParameter) {
  real34_t r, maxErr;
  uInt32ToReal34(NUMBER_OF_ERROR_CODES, &maxErr);

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger: {
      convertLongIntegerRegisterToReal34(REGISTER_X, &r);
      break;
    }

    case dtReal34:
      if(getRegisterAngularMode(REGISTER_X) == amNone) {
        real34Copy(REGISTER_REAL34_DATA(REGISTER_X), &r);
        break;
      }
      /* fallthrough */


    default: {
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "data type %s cannot be used for this function!", getRegisterDataTypeName(REGISTER_X, false, false));
        moreInfoOnError("In function fnErrorMessage:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      return;
    }
  }
  #pragma GCC diagnostic pop

  if(real34CompareLessEqual(const34_1, &r) && real34CompareLessThan(&r, &maxErr)) {
    displayCalcErrorMessage((uint8_t)real34ToUInt32(&r), ERR_REGISTER_LINE, REGISTER_X);
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "the argument is not less than %u or is negative!", NUMBER_OF_ERROR_CODES);
      moreInfoOnError("In function fnErrorMessage:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}



void displayCalcErrorMessage(uint8_t errorCode, calcRegister_t errMessageRegisterLine, calcRegister_t errRegisterLine) {
  if(errorCode >= NUMBER_OF_ERROR_CODES || errorCode == 0) {
    sprintf(errorMessage, commonBugScreenMessages[bugMsgValueFor], "displayCalcErrorMessage", errorCode, "errorCode");
    displayBugScreen(errorMessage);
  }

  else if(errMessageRegisterLine > REGISTER_T || errMessageRegisterLine < REGISTER_X) {
    sprintf(errorMessage, commonBugScreenMessages[bugMsgValueFor], "displayCalcErrorMessage", errMessageRegisterLine, "errMessageRegisterLine");
    sprintf(errorMessage + strlen(errorMessage), "Must be from 100 (register X) to 103 (register T)");
    displayBugScreen(errorMessage);
  }

  else if(errRegisterLine > REGISTER_T || errRegisterLine < REGISTER_X) {
    sprintf(errorMessage, commonBugScreenMessages[bugMsgValueFor], "displayCalcErrorMessage", errRegisterLine, "errRegisterLine");
    sprintf(errorMessage + strlen(errorMessage), "Must be from 100 (register X) to 103 (register T)");
    displayBugScreen(errorMessage);
  }

  else {
    lastErrorCode            = errorCode;
    errorMessageRegisterLine = errMessageRegisterLine;
  }
}



#if !defined(TESTSUITE_BUILD)
  void nextWord(const char *str, int16_t *pos, char *word) {
    int16_t i = 0;

    while(str[*pos] != 0 && str[*pos] != ' ') {
      word[i++] = str[(*pos)++];
    }

    word[i] = 0;

    while(str[*pos] == ' ') {
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
      clearSystemFlag(FLAG_ALPHA);
      hideCursor();
      cursorEnabled = false;

      lcd_fill_rect(0, 20, SCREEN_WIDTH, 220, LCD_SET_VALUE);

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
#endif // !TESTSUITE_BUILD



/********************************************//**
 * \brief Data type error, common function
 *
 * \param void
 * \return void
 ***********************************************/
#if (EXTRA_INFO_ON_CALC_ERROR != 1)
  void typeError(void) {
    displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
  }
#endif // (EXTRA_INFO_ON_CALC_ERROR != 1)
