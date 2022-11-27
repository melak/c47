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

/*TO_QSPI*/ const char *commonBugScreenMessages[NUMBER_OF_BUG_SCREEN_MESSAGES] = {
/*  0 */  "In function %s:%d is an unexpected value for %s!",
/*  1 */  "In function %s: unexpected calcMode value (%" PRIu8 ") while processing key %s!",
/*  2 */  "In function reallocateRegister: %" PRIu16 " is an unexpected numByte value for %s! It should be %s=%" PRIu16 "!",
/*  3 */  "In function %s: no named variables defined!",
/*  4 */  "In function %s: %d is an unexpected value returned by findGlyph!",
/*  5 */  "In function %s: %" PRIu32 " is an unexpected %s value!",
/*  6 */  "In function %s: data type %s is unknown!",
/*  7 */  "In function %s: regist=%" PRId16 " must be less than %d!",
/*  8 */  "In function %s: %s %" PRId16 " is not defined! Must be from 0 to %" PRIu16,
/*  9 */  "In function %s: unexpected case while processing key %s! %" PRIu8 " is an unexpected value for rbrMode.",
};

TO_QSPI const uint8_t errorMessages00[] = {'N','o',' ','e','r','r','o','r',0};
TO_QSPI const uint8_t errorMessages01[] = {'A','n',' ','a','r','g','u','m','e','n','t',' ','e','x','c','e','e','d','s',' ','t','h','e',' ','f','u','n','c','t','i','o','n',' ','d','o','m','a','i','n',0};
TO_QSPI const uint8_t errorMessages02[] = {'B','a','d',' ','t','i','m','e',' ','o','r',' ','d','a','t','e',' ','i','n','p','u','t',0};
TO_QSPI const uint8_t errorMessages03[] = {'U','n','d','e','f','i','n','e','d',' ','o','p','-','c','o','d','e',0};
TO_QSPI const uint8_t errorMessages04[] = {'O','v','e','r','f','l','o','w',' ','a','t',' ','+',STD_INFINITY[0],STD_INFINITY[1],0};
TO_QSPI const uint8_t errorMessages05[] = {'O','v','e','r','f','l','o','w',' ','a','t',' ','-',STD_INFINITY[0],STD_INFINITY[1],0};
TO_QSPI const uint8_t errorMessages06[] = {'N','o',' ','s','u','c','h',' ','l','a','b','e','l',' ','f','o','u','n','d',0};
TO_QSPI const uint8_t errorMessages07[] = {'N','o',' ','s','u','c','h',' ','f','u','n','c','t','i','o','n',0};
TO_QSPI const uint8_t errorMessages08[] = {'O','u','t',' ','o','f',' ','r','a','n','g','e',0};
TO_QSPI const uint8_t errorMessages09[] = {'I','l','l','e','g','a','l',' ','d','i','g','i','t',' ','i','n',' ','i','n','t','e','g','e','r',' ','i','n','p','u','t',' ','f','o','r',' ','t','h','i','s',' ','b','a','s','e',0};
TO_QSPI const uint8_t errorMessages10[] = {'I','n','p','u','t',' ','i','s',' ','t','o','o',' ','l','o','n','g',0};
TO_QSPI const uint8_t errorMessages11[] = {'R','A','M',' ','i','s',' ','f','u','l','l',0};
TO_QSPI const uint8_t errorMessages12[] = {'S','t','a','c','k',' ','c','l','a','s','h',0};
TO_QSPI const uint8_t errorMessages13[] = {'O','p','e','r','a','t','i','o','n',' ','i','s',' ','u','n','d','e','f','i','n','e','d',' ','i','n',' ','t','h','i','s',' ','m','o','d','e',0};
TO_QSPI const uint8_t errorMessages14[] = {'W','o','r','d',' ','s','i','z','e',' ','i','s',' ','t','o','o',' ','s','m','a','l','l',0};
TO_QSPI const uint8_t errorMessages15[] = {'T','o','o',' ','f','e','w',' ','d','a','t','a',' ','p','o','i','n','t','s',' ','f','o','r',' ','t','h','i','s',' ','s','t','a','t','i','s','t','i','c',0};
TO_QSPI const uint8_t errorMessages16[] = {'D','i','s','t','r','i','b','u','t','i','o','n',' ','p','a','r','a','m','e','t','e','r',' ','o','u','t',' ','o','f',' ','v','a','l','i','d',' ','r','a','n','g','e',0};
TO_QSPI const uint8_t errorMessages17[] = {'I','/','O',' ','e','r','r','o','r',0};
TO_QSPI const uint8_t errorMessages18[] = {'I','n','v','a','l','i','d',' ','o','r',' ','c','o','r','r','u','p','t','e','d',' ','d','a','t','a',0};
TO_QSPI const uint8_t errorMessages19[] = {'F','l','a','s','h',' ','m','e','m','o','r','y',' ','i','s',' ','w','r','i','t','e',' ','p','r','o','t','e','c','t','e','d',0};
TO_QSPI const uint8_t errorMessages20[] = {'N','o',' ','r','o','o','t',' ','f','o','u','n','d',0};
TO_QSPI const uint8_t errorMessages21[] = {'M','a','t','r','i','x',' ','m','i','s','m','a','t','c','h',0};
TO_QSPI const uint8_t errorMessages22[] = {'S','i','n','g','u','l','a','r',' ','m','a','t','r','i','x',0};
TO_QSPI const uint8_t errorMessages23[] = {'F','l','a','s','h',' ','m','e','m','o','r','y',' ','i','s',' ','f','u','l','l',0};
TO_QSPI const uint8_t errorMessages24[] = {'I','n','v','a','l','i','d',' ','i','n','p','u','t',' ','d','a','t','a',' ','t','y','p','e',' ','f','o','r',' ','t','h','i','s',' ','o','p','e','r','a','t','i','o','n',0};
TO_QSPI const uint8_t errorMessages25[] = {0};
TO_QSPI const uint8_t errorMessages26[] = {'P','l','e','a','s','e',' ','e','n','t','e','r',' ','a',' ','N','E','W',' ','n','a','m','e',0};
TO_QSPI const uint8_t errorMessages27[] = {'C','a','n','n','o','t',' ','d','e','l','e','t','e',' ','a',' ','p','r','e','d','e','f','i','n','e','d',' ','i','t','e','m',0};
TO_QSPI const uint8_t errorMessages28[] = {'N','o',' ','s','t','a','t','i','s','t','i','c',' ','d','a','t','a',' ','p','r','e','s','e','n','t',0};
TO_QSPI const uint8_t errorMessages29[] = {'I','t','e','m',' ','t','o',' ','b','e',' ','c','o','d','e','d',0};
TO_QSPI const uint8_t errorMessages30[] = {'F','u','n','c','t','i','o','n',' ','t','o',' ','b','e',' ','c','o','d','e','d',' ','f','o','r',' ','t','h','a','t',' ','d','a','t','a',' ','t','y','p','e',0};
TO_QSPI const uint8_t errorMessages31[] = {'I','n','p','u','t',' ','d','a','t','a',' ','t','y','p','e','s',' ','d','o',' ','n','o','t',' ','m','a','t','c','h',0};
TO_QSPI const uint8_t errorMessages32[] = {'T','h','i','s',' ','s','y','s','t','e','m',' ','f','l','a','g',' ','i','s',' ','w','r','i','t','e',' ','p','r','o','t','e','c','t','e','d',0};
TO_QSPI const uint8_t errorMessages33[] = {'O','u','t','p','u','t',' ','w','o','u','l','d',' ','e','x','c','e','e','d',' ','1','9','6',' ','c','h','a','r','a','c','t','e','r','s',0};
TO_QSPI const uint8_t errorMessages34[] = {'T','h','i','s',' ','d','o','e','s',' ','n','o','t',' ','w','o','r','k',' ','w','i','t','h',' ','a','n',' ','e','m','p','t','y',' ','s','t','r','i','n','g',0};
TO_QSPI const uint8_t errorMessages35[] = {'N','o',' ','b','a','c','k','u','p',' ','d','a','t','a',' ','f','o','u','n','d',0};
TO_QSPI const uint8_t errorMessages36[] = {'U','n','d','e','f','i','n','e','d',' ','s','o','u','r','c','e',' ','v','a','r','i','a','b','l','e',0};
TO_QSPI const uint8_t errorMessages37[] = {'T','h','i','s',' ','v','a','r','i','a','b','l','e',' ','i','s',' ','w','r','i','t','e',' ','p','r','o','t','e','c','t','e','d',0};
TO_QSPI const uint8_t errorMessages38[] = {'N','o',' ','m','a','t','r','i','x',' ','i','n','d','e','x','e','d',0};
TO_QSPI const uint8_t errorMessages39[] = {'N','o','t',' ','e','n','o','u','g','h',' ','m','e','m','o','r','y',' ','f','o','r',' ','s','u','c','h',' ','a',' ','m','a','t','r','i','x',0};
TO_QSPI const uint8_t errorMessages40[] = {'N','o',' ','e','r','r','o','r','s',' ','f','o','r',' ','s','e','l','e','c','t','e','d',' ','m','o','d','e','l',0};
TO_QSPI const uint8_t errorMessages41[] = {'L','a','r','g','e',' ',STD_DELTA[0],STD_DELTA[1],' ','a','n','d',' ','o','p','p','o','s','i','t','e',' ','s','i','g','n','s',',',' ','m','a','y',' ','b','e',' ','a',' ','p','o','l','e',0};
TO_QSPI const uint8_t errorMessages42[] = {'S','o','l','v','e','r',' ','r','e','a','c','h','e','d',' ','l','o','c','a','l',' ','e','x','t','r','e','m','u','m',',',' ','n','o',' ','r','o','o','t',0};
TO_QSPI const uint8_t errorMessages43[] = {STD_GREATER_EQUAL[0],STD_GREATER_EQUAL[1],'1',' ','i','n','i','t','i','a','l',' ','g','u','e','s','s',' ','l','i','e','s',' ','o','u','t',' ','o','f',' ','t','h','e',' ','d','o','m','a','i','n',0};
TO_QSPI const uint8_t errorMessages44[] = {'T','h','e',' ','f','u','n','c','t','i','o','n',' ','v','a','l','u','e',' ','l','o','o','k',' ','c','o','n','s','t','a','n','t',0};
TO_QSPI const uint8_t errorMessages45[] = {'S','y','n','t','a','x',' ','e','r','r','o','r',' ','i','n',' ','t','h','i','s',' ','e','q','u','a','t','i','o','n',0};
TO_QSPI const uint8_t errorMessages46[] = {'T','h','i','s',' ','e','q','u','a','t','i','o','n',' ','f','o','r','m','u','l','a',' ','i','s',' ','t','o','o',' ','c','o','m','p','l','e','x',0};
TO_QSPI const uint8_t errorMessages47[] = {'T','h','i','s',' ','i','t','e','m',' ','c','a','n','n','o','t',' ','b','e',' ','a','s','s','i','g','n','e','d',' ','h','e','r','e',0};
TO_QSPI const uint8_t errorMessages48[] = {'I','n','v','a','l','i','d',' ','n','a','m','e',0};
TO_QSPI const uint8_t errorMessages49[] = {'T','o','o',' ','m','a','n','y',' ','v','a','r','i','a','b','l','e','s',0};
TO_QSPI const uint8_t errorMessages50[] = {'N','o','n','-','p','r','o','g','r','a','m','m','a','b','l','e',' ','c','o','m','m','a','n','d',',',' ','p','l','e','a','s','e',' ','r','e','m','o','v','e',0};
TO_QSPI const uint8_t errorMessages51[] = {'N','o',' ','g','l','o','b','a','l',' ','l','a','b','e','l',' ','i','n',' ','t','h','i','s',' ','p','r','o','g','r','a','m',0};
TO_QSPI const uint8_t errorMessages52[] = {'B','a','d',' ','i','n','p','u','t',0}; // This error is not in ReM and cannot occur (theoretically).
const uint8_t *errorMessages[NUMBER_OF_ERROR_CODES] = {
  errorMessages00,
  errorMessages01,
  errorMessages02,
  errorMessages03,
  errorMessages04,
  errorMessages05,
  errorMessages06,
  errorMessages07,
  errorMessages08,
  errorMessages09,
  errorMessages10,
  errorMessages11,
  errorMessages12,
  errorMessages13,
  errorMessages14,
  errorMessages15,
  errorMessages16,
  errorMessages17,
  errorMessages18,
  errorMessages19,
  errorMessages20,
  errorMessages21,
  errorMessages22,
  errorMessages23,
  errorMessages24,
  errorMessages25,
  errorMessages26,
  errorMessages27,
  errorMessages28,
  errorMessages29,
  errorMessages30,
  errorMessages31,
  errorMessages32,
  errorMessages33,
  errorMessages34,
  errorMessages35,
  errorMessages36,
  errorMessages37,
  errorMessages38,
  errorMessages39,
  errorMessages40,
  errorMessages41,
  errorMessages42,
  errorMessages43,
  errorMessages44,
  errorMessages45,
  errorMessages46,
  errorMessages47,
  errorMessages48,
  errorMessages49,
  errorMessages50,
  errorMessages51,
  errorMessages52,
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
    sprintf(errorMessage, commonBugScreenMessages[BUGMSG_VALUE_FOR], "displayCalcErrorMessage", errorCode, "errorCode");
    displayBugScreen(errorMessage);
  }

  else if(errMessageRegisterLine > REGISTER_T || errMessageRegisterLine < REGISTER_X) {
    sprintf(errorMessage, commonBugScreenMessages[BUGMSG_VALUE_FOR], "displayCalcErrorMessage", errMessageRegisterLine, "errMessageRegisterLine");
    sprintf(errorMessage + strlen(errorMessage), "Must be from 100 (register X) to 103 (register T)");
    displayBugScreen(errorMessage);
  }

  else if(errRegisterLine > REGISTER_T || errRegisterLine < REGISTER_X) {
    sprintf(errorMessage, commonBugScreenMessages[BUGMSG_VALUE_FOR], "displayCalcErrorMessage", errRegisterLine, "errRegisterLine");
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
