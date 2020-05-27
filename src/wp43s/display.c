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
 * \file display.c
 ***********************************************/

#include "wp43s.h"

/********************************************//**
 * \brief Sets the display override mode and refreshes the stack
 *
 * \param[in] displayOvr uint16_t Display override mode
 * \return void
 ***********************************************/
void fnDisplayOvr(uint16_t displayOvr) {
  displayModeOverride = displayOvr;

//  fnRefreshRadioState(RB_DO, displayOvr);                                       //dr

  refreshStack();
}



/********************************************//**
 * \brief Sets the product sign and refreshes the stack
 *
 * \param[in] productSign uint16_t Product sign
 * \return void
 ***********************************************/
void fnProductSign(uint16_t ps) {
  productSign = ps;

  fnRefreshRadioState(RB_PS, ps);                                               //dr

  showFracMode();
  refreshStack();
}



/********************************************//**
 * \brief Sets the display format FIX and refreshes the stack
 *
 * \param[in] displayFormatN uint16_t Display format
 * \return void
 ***********************************************/
void fnDisplayFormatFix(uint16_t displayFormatN) {
  displayFormat = DF_FIX;
  displayFormatDigits = displayFormatN;
  clearSystemFlag(FLAG_FRACT);
  SigFigMode = 0;                                                //JM SIGFIG Reset SIGFIG 
  UNITDisplay = false;                                           //JM UNIT display Reset

  fnRefreshRadioState(RB_DI, DF_FIX);                                           //dr

  refreshStack();
}



/********************************************//**
 * \brief Sets the display format SCI and refreshes the stack
 *
 * \param[in] displayFormatN uint16_t Display format
 * \return void
 ***********************************************/
void fnDisplayFormatSci(uint16_t displayFormatN) {
  displayFormat = DF_SCI;
  displayFormatDigits = displayFormatN;
  clearSystemFlag(FLAG_FRACT);
  SigFigMode = 0;                                                //JM SIGFIG Reset SIGFIG 
  UNITDisplay = false;                                           //JM UNIT display Reset

  fnRefreshRadioState(RB_DI, DF_SCI);                                           //dr

  refreshStack();
}



/********************************************//**
 * \brief Sets the display format ENG and refreshes the stack
 *
 * \param[in] displayFormatN uint16_t Display format
 * \return void
 ***********************************************/
void fnDisplayFormatEng(uint16_t displayFormatN) {
  displayFormat = DF_ENG;
  displayFormatDigits = displayFormatN;
  clearSystemFlag(FLAG_FRACT);
  SigFigMode = 0;                                                //JM SIGFIG Reset SIGFIG 
  UNITDisplay = false;                                           //JM UNIT display Reset

  fnRefreshRadioState(RB_DI, DF_ENG);                                           //dr

  refreshStack();
}



/********************************************//**
 * \brief Sets the display format ALL and refreshes the stack
 *
 * \param[in] displayFormatN uint16_t Display format
 * \return void
 ***********************************************/
void fnDisplayFormatAll(uint16_t displayFormatN) {
  //if(0 <= displayFormatN && displayFormatN <= 15) {
  displayFormat = DF_ALL;
  displayFormatDigits = displayFormatN;
  clearSystemFlag(FLAG_FRACT);
  SigFigMode = 0;                                                //JM SIGFIG Reset SIGFIG
  UNITDisplay = false;                                           //JM UNIT display Reset

  fnRefreshRadioState(RB_DI, DF_ALL);                                         //dr

  refreshStack();
}



/********************************************//**
 * \brief Sets the number of digits afer the period and refreshes the stack
 *
 * \param[in] displayFormatN uint16_t Display format
 * \return void
 ***********************************************/
/*void fnDisplayFormatDsp(uint16_t displayFormatN) {
  displayFormatDigits = displayFormatN;
  displayRealAsFraction = false;

  if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
    convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
  }
  #ifdef PC_BUILD
    else if(getRegisterDataType(REGISTER_X) == dtShortInteger) {
      showInfoDialog("In function fnDisplayFormatDsp:", "converting an integer to a real16", "is to be coded", NULL);
    }
  #endif

  refreshStack();
}*/



/********************************************//**
 * \brief Sets the GAP in number represemtation and refreshes the stack
 *
 * \param[in] gap uint16_t
 * \return void
 ***********************************************/
void fnDisplayFormatGap(uint16_t gap) {
 if(gap == 1 || gap == 2) {
   gap = 0;
 }
  groupingGap = gap;
  refreshStack();
}



/********************************************//**
 * \brief Adds the power of 10 using numeric font to displayString
 *
 * \param[out] displayString char*     Result string
 * \param[in]  exponent int32_t Power of 10 to format
 * \return void
 ***********************************************/
void exponentToDisplayString(int32_t exponent, char *displayString, char *displayValueString, bool_t nimMode, const char *separator) {
  strcpy(displayString, PRODUCT_SIGN);
  displayString += 2;
  strcpy(displayString, STD_SUB_10);
  displayString += 2;
  displayString[0] = 0;

  if(displayValueString != NULL) {
    *displayValueString++ = 'e';
    *displayValueString = 0;
  }

  if(nimMode) {
    if(exponent != 0) {
      supNumberToDisplayString(exponent, displayString, displayValueString, false, separator);
    }
  }
  else {
    supNumberToDisplayString(exponent, displayString, displayValueString, false, separator);
  }
}



void supNumberToDisplayString(int32_t supNumber, char *displayString, char *displayValueString, bool_t insertGap, const char *separator) {
  if(displayValueString != NULL) {
    sprintf(displayValueString, "%" FMT32S, supNumber);
  }

  if(supNumber == 0) {
    strcat(displayString, STD_SUP_0);
  }
  else {
    int16_t digit, digitCount=0;
    bool_t greaterThan9999;

    if(supNumber < 0) {
      supNumber = -supNumber;
      strcat(displayString, STD_SUP_MINUS);
      displayString += 2;
    }

    greaterThan9999 = (supNumber > 9999);
    while(supNumber > 0) {
      digit = supNumber % 10;
      supNumber /= 10;

      xcopy(displayString + 2, displayString, stringByteLength(displayString) + 1);

      if(digit <= 1) {
        *(displayString)     = *(STD_SUP_0);
        *(displayString + 1) = *(STD_SUP_0 + 1);
        displayString[1] += digit;
      }
      else if(digit <= 3) {
        *(displayString)     = *(STD_SUP_2);
        *(displayString + 1) = *(STD_SUP_2 + 1);
        displayString[1] += digit-2;
      }
      else {
        *(displayString)     = *(STD_SUP_4);
        *(displayString + 1) = *(STD_SUP_4 + 1);
        displayString[1] += digit-4;
      }

      if(insertGap && greaterThan9999 && supNumber > 0 && groupingGap != 0 && ((++digitCount) % groupingGap) == 0) {
        xcopy(displayString + 2, displayString, stringByteLength(displayString) + 1);
        *(displayString)     = *(separator);
        *(displayString + 1) = *(separator + 1);
      }
    }
  }

  strcat(displayString, STD_SPACE_HAIR);
}



void subNumberToDisplayString(int32_t subNumber, char *displayString, char *displayValueString) {
  if(displayValueString != NULL) {
    sprintf(displayValueString, "%" FMT32S, subNumber);
  }

  if(subNumber < 0) {
    subNumber = -subNumber;
    strcat(displayString, STD_SUB_MINUS);
    displayString += 2;
  }

  if(subNumber == 0) {
    strcat(displayString, STD_SUB_0);
  }
  else {
    int16_t digit;

    while(subNumber > 0) {
      digit = subNumber % 10;
      subNumber /= 10;

      xcopy(displayString + 2, displayString, stringByteLength(displayString) + 1);

      *(displayString)     = *(STD_SUB_0);
      *(displayString + 1) = *(STD_SUB_0 + 1);
      displayString[1] += digit;
    }
  }
}



void real34ToDisplayString(const real34_t *real34, uint32_t angulaMode, char *displayString, const font_t *font, int16_t maxWidth, int16_t displayHasNDigits, bool_t limitExponent, const char *separator) {
  uint8_t savedDisplayFormatDigits = displayFormatDigits;

  if(updateDisplayValueX) {
    displayValueX[0] = 0;
  }

  if(angulaMode == AM_NONE) {
    realToDisplayString2(real34, displayString, displayHasNDigits, limitExponent, separator);
  }
  else {
    angle34ToDisplayString2(real34, angulaMode, displayString, displayHasNDigits, limitExponent, separator);
  }

  while(stringWidth(displayString, font, true, true) > maxWidth) {
    if(displayFormat == DF_ALL) {
      if(displayHasNDigits == 2) {
        break;
      }
      displayHasNDigits--;
    }
    else {
      if(displayFormatDigits == 0) {
        break;
      }
      displayFormatDigits--;
    }

    if(updateDisplayValueX) {
      displayValueX[0] = 0;
    }

    if(angulaMode == AM_NONE) {
      realToDisplayString2(real34, displayString, displayHasNDigits, limitExponent, separator);
    }
    else {
      angle34ToDisplayString2(real34, angulaMode, displayString, displayHasNDigits, limitExponent, separator);
    }
  }
  displayFormatDigits = savedDisplayFormatDigits;
}



/********************************************//**
 * \brief Formats a real
 *
 * \param[out] displayString char* Result string
 * \param[in]  x const real16_t*  Value to format
 * \return void
 ***********************************************/
void realToDisplayString2(const real34_t *real34, char *displayString, int16_t displayHasNDigits, bool_t limitExponent, const char *separator) {
  #undef MAX_DIGITS
  #define MAX_DIGITS 37 // 34 + 1 before (used when rounding from 9.999 to 10.000) + 2 after (used for rounding and ENG display mode)

  bool_t displayoverflow = false;  //JM Need to know if overflowing. Otherwise UNIT cannot work.

  uint8_t charIndex, valueIndex, digitToRound=0;
  uint8_t *bcd;
  int16_t digitsToDisplay=0, numDigits, digitPointer, firstDigit, lastDigit, i, digitCount, digitsToTruncate, exponent;
  int32_t sign;
  bool_t  firstDigitAfterPeriod=true;
  real34_t value34;
  real_t value;

  real34ToReal(real34, &value);
  ctxtReal39.digits = displayHasNDigits;
  realPlus(&value, &value, &ctxtReal39);
  ctxtReal39.digits = 39;
  realToReal34(&value, &value34);
  if(real34IsNegative(real34)) {
    real34SetNegativeSign(&value34);
  }

  bcd = (uint8_t *)(tmpStr3000 + 256 - MAX_DIGITS);
  memset(bcd, 0, MAX_DIGITS);

  sign = real34GetCoefficient(&value34, bcd + 1);
  exponent = real34GetExponent(&value34);

  // Calculate the number of significant digits
  for(digitPointer=1; digitPointer<=MAX_DIGITS-3; digitPointer++) {
    if(bcd[digitPointer] != 0) {
      break;
    }
  }

  if(digitPointer >= MAX_DIGITS-2) { // *real = 0.0
    firstDigit = 0;
    lastDigit  = 0;
    numDigits  = 1;
    exponent   = 0;
  }
  else {
    firstDigit = digitPointer;

    for(digitPointer=MAX_DIGITS-3; digitPointer>=1; digitPointer--) {
      if(bcd[digitPointer] == 0) {
        exponent++;
      }
      else {
        break;
      }
    }
    lastDigit = digitPointer;

    numDigits = lastDigit - firstDigit;
    exponent += numDigits++;
  }

/*  if(limitExponent && abs(exponent) > exponentLimit) {
    if(exponent > exponentLimit) {
      if(real34IsPositive(&value34)) {
        realToReal34(const_plusInfinity, &value34);
      }
      else {
        realToReal34(const_minusInfinity, &value34);
      }
    }
    else if(exponent < -exponentLimit) {
      real34Zero(&value34);

      bcd = (uint8_t *)(tmpStr3000 + 256 - MAX_DIGITS);
      memset(bcd, 0, MAX_DIGITS);

      sign = 0;
      exponent = 0;
      firstDigit = 0;
      lastDigit  = 0;
      numDigits  = 1;
      exponent   = 0;
    }
  }*/

  if(limitExponent && abs(exponent) > exponentLimit) {
    if(exponent > exponentLimit) {
      if(real34IsPositive(&value34)) {
        strcpy(displayString, STD_LEFT_SINGLE_QUOTE STD_INFINITY STD_RIGHT_SINGLE_QUOTE);
        if(updateDisplayValueX) {
          strcpy(displayValueX + strlen(displayValueX), "9e9999");
        }
      }
      else {
        strcpy(displayString, "-" STD_LEFT_SINGLE_QUOTE STD_INFINITY STD_RIGHT_SINGLE_QUOTE);
        if(updateDisplayValueX) {
          strcpy(displayValueX + strlen(displayValueX), "-9e9999");
        }
      }
      return;
    }
    else if(exponent < -exponentLimit) {
      if(real34IsPositive(&value34)) {
        strcpy(displayString, STD_LEFT_SINGLE_QUOTE "0." STD_RIGHT_SINGLE_QUOTE);
        if(updateDisplayValueX) {
          strcpy(displayValueX + strlen(displayValueX), "0");
        }
      }
      else {
        strcpy(displayString, STD_LEFT_SINGLE_QUOTE "0." STD_RIGHT_SINGLE_QUOTE);
        if(updateDisplayValueX) {
          strcpy(displayValueX + strlen(displayValueX), "0");
        }
      }
      return;
    }
  }

  if(real34IsInfinite(&value34)) {
    if(real34IsNegative(&value34)) {
      strcpy(displayString, "-" STD_INFINITY);
      if(updateDisplayValueX) {
        strcpy(displayValueX + strlen(displayValueX), "-9e9999");
      }
    }
    else {
      strcpy(displayString, STD_INFINITY);
      if(updateDisplayValueX) {
        strcpy(displayValueX + strlen(displayValueX), "9e9999");
      }
    }
    return;
  }

  if(real34IsNaN(&value34)) {
    real34ToString(&value34, displayString);
    if(updateDisplayValueX) {
      real34ToString(&value34, displayValueX + strlen(displayValueX));
    }
    return;
  }

//JM /* ***************************************      SIGFIG ********************************** */
//    SigFigMode                                                                      //JM SIGFIG
                                                                                      //JM SIGFIG
    uint8_t  SigFig;                                                                  //JM SIGFIG
    SigFig = SigFigMode + 1;                                                          //JM SIGFIG
                                                                                      //JM SIGFIG
    uint8_t  SigFigTmp;                                                               //JM SIGFIG
    uint8_t  SigFigCnt;                                                               //JM SIGFIG
                                                                                      //JM SIGFIG
  if(SigFigMode >= 1) {                                                               //JM SIGFIG 0 is disabled
                                                                                      //JM SIGFIG
   SigFigTmp = firstDigit + SigFig - 1;                                               //JM SIGFIG
                                                                                      //JM SIGFIG
  //JM SIGFIG - Round the displayed number                                            //JM SIGFIG
   if(bcd[SigFigTmp + 1] >= 5) {                                                      //JM SIGFIG
      bcd[SigFigTmp]++;                                                               //JM SIGFIG
   }                                                                                  //JM SIGFIG
                                                                                      //JM SIGFIG
  //JM SIGFIG - Transfer the carry                                                    //JM SIGFIG
    while(bcd[SigFigTmp] == 10) {                                                     //JM SIGFIG
      bcd[SigFigTmp--] = 0;                                                           //JM SIGFIG
      bcd[SigFigTmp]++;                                                               //JM SIGFIG
    }                                                                                 //JM SIGFIG
                                                                                      //JM SIGFIG
  //JM SIGFIG - Case when 9.9999 rounds to 10.0000                                    //JM SIGFIG
    if(SigFigTmp < firstDigit) {                                                      //JM SIGFIG
      firstDigit--;                                                                   //JM SIGFIG
      lastDigit = firstDigit;                                                         //JM SIGFIG
      numDigits = 1;                                                                  //JM SIGFIG
      exponent++;                                                                     //JM SIGFIG
    }                                                                                 //JM SIGFIG
                                                                                      //JM SIGFIG
  //JM SIGFIG - blank out non-sig digits to the right                                 //JM SIGFIG
    if(firstDigit + SigFig <= lastDigit) {                                            //JM SIGFIG
       for (digitCount = firstDigit + SigFig; digitCount <= lastDigit; digitCount++) {  //JM SIGFIG
        bcd[digitCount] = 0;                                                          //JM SIGFIG
        }                                                                             //JM SIGFIG
    }                                                                                 //JM SIGFIG
  }                                                                                   //JM SIGFIG
//JM /* ***************************************        SIGFIG ******************************** */



  charIndex = 0;
  valueIndex = (updateDisplayValueX ? strlen(displayValueX) : 0);

  //////////////
  // ALL mode //
  //////////////
  if(displayFormat == DF_ALL) {
    if(exponent >= displayHasNDigits || (displayFormatDigits != 0 && exponent < -(int32_t)displayFormatDigits) || (displayFormatDigits == 0 && exponent < numDigits - displayHasNDigits)) { // Display in SCI or ENG format
      digitsToDisplay = numDigits - 1;
      digitToRound    = firstDigit + digitsToDisplay;
    }
    else { // display all digits without ten exponent factor
      // Number of digits to truncate
      digitsToTruncate = max(numDigits - exponent, displayHasNDigits) - displayHasNDigits;
      numDigits -= digitsToTruncate;
      lastDigit -= digitsToTruncate;

      // Round the displayed number
      if(bcd[lastDigit+1] >= 5) {
        bcd[lastDigit]++;
      }

      // Transfert the carry
      while(bcd[lastDigit] == 10) {
        bcd[lastDigit--] = 0;
        numDigits--;
        bcd[lastDigit]++;
      }

      // Case when 9.9999 rounds to 10.0000
      if(lastDigit < firstDigit) {
        firstDigit--;
        lastDigit = firstDigit;
        numDigits = 1;
        exponent++;
      }

      // The sign
      if(sign) {
        displayString[charIndex++] = '-';
        if(updateDisplayValueX) {
          displayValueX[valueIndex++] = '-';
        }
      }

      if(exponent < 0) { // negative exponent
        // first 0 and radix mark
        displayString[charIndex++] = '0';
        if(updateDisplayValueX) {
          displayValueX[valueIndex++] = '0';
        }
        displayString[charIndex] = 0;
        strcat(displayString, RADIX34_MARK_STRING);
        charIndex++;
        if(updateDisplayValueX) {
          displayValueX[valueIndex++] = '.';
        }

        // Zeros before first significant digit
        for(digitCount=0, i=exponent+1; i<0; i++, digitCount--) {
          if(digitCount != 0 && groupingGap != 0 && digitCount%(uint16_t)groupingGap == 0) {
            xcopy(displayString + charIndex, separator, 2);
            charIndex += 2;
          }
          displayString[charIndex++] = '0';
          if(updateDisplayValueX) {
            displayValueX[valueIndex++] = '0';
          }
        }

        // Significant digits
        for(digitPointer=firstDigit; digitPointer<firstDigit+min(displayHasNDigits - 1 - exponent, numDigits); digitPointer++, digitCount--) {
          if(digitCount != 0 && groupingGap != 0 && digitCount%(uint16_t)groupingGap == 0) {
            xcopy(displayString + charIndex, separator, 2);
            charIndex += 2;
          }
          displayString[charIndex++] = '0' + bcd[digitPointer];
          if(updateDisplayValueX) {
            displayValueX[valueIndex++] = '0' + bcd[digitPointer];
          }
        }
      }
      else { // zero or positive exponent
        for(digitCount=exponent, digitPointer=firstDigit; digitPointer<=lastDigit + max(exponent - numDigits + 1, 0); digitPointer++, digitCount--) {
          if(digitCount != -1 && digitCount != exponent && groupingGap != 0 && modulo(digitCount, (uint16_t)groupingGap) == (uint16_t)groupingGap - 1) {
            xcopy(displayString + charIndex, separator, 2);
            charIndex += 2;
          }

          // Significant digit or zero
          if(digitPointer <= lastDigit) {
            displayString[charIndex++] = '0' + bcd[digitPointer];
            if(updateDisplayValueX) {
              displayValueX[valueIndex++] = '0' + bcd[digitPointer];
            }
          }
          else {
            displayString[charIndex++] = '0';
            if(updateDisplayValueX) {
              displayValueX[valueIndex++] = '0';
            }
          }

          // Radix mark
          if(digitCount == 0) {
            displayString[charIndex] = 0;
            strcat(displayString, RADIX34_MARK_STRING);
            charIndex++;
            if(updateDisplayValueX) {
              displayValueX[valueIndex++] = '.';
            }
          }
        }
      }

      displayString[charIndex] = 0;
      if(updateDisplayValueX) {
        displayValueX[valueIndex] = 0;
      }
      return;
    }
  displayoverflow = true;  //JM Need to know if overflowing. Otherwise UNIT cannot work.
  }

  //////////////
  // FIX mode //
  //////////////
  if(displayFormat == DF_FIX) {
    if(exponent >= displayHasNDigits || exponent < -(int32_t)displayFormatDigits) { // Display in SCI or ENG format
      digitsToDisplay = displayFormatDigits;
      digitToRound    = min(firstDigit + digitsToDisplay, lastDigit);
    }
    else { // display fix number of digits without ten exponent factor
      // Number of digits to truncate
      digitsToTruncate = max(numDigits - (int16_t)displayFormatDigits - exponent - 1, 0);
      numDigits -= digitsToTruncate;
      lastDigit -= digitsToTruncate;

      if(SigFigMode == 0) {                       //JM SIGFIG vv
      // Round the displayed number
      if(bcd[lastDigit+1] >= 5) {
        bcd[lastDigit]++;
      }

      // Transfert the carry
      while(bcd[lastDigit] == 10) {
        bcd[lastDigit--] = 0;
        numDigits--;
        bcd[lastDigit]++;
      }

      // Case when 9.9999 rounds to 10.0000
      if(lastDigit < firstDigit) {
        firstDigit--;
        lastDigit = firstDigit;
        numDigits = 1;
        exponent++;
      }
      }                                           //JM SIGFIG ^^

      // The sign
      if(sign) {
        displayString[charIndex++] = '-';
        if(updateDisplayValueX) {
          displayValueX[valueIndex++] = '-';
        }
      }

      if(exponent < 0) { // negative exponent
        // first 0 and radix mark
        displayString[charIndex++] = '0';
        if(updateDisplayValueX) {
          displayValueX[valueIndex++] = '0';
        }
        displayString[charIndex] = 0;
        strcat(displayString, RADIX34_MARK_STRING);
        charIndex++;
        if(updateDisplayValueX) {
          displayValueX[valueIndex++] = '.';
        }

        // Zeros before first significant digit
        for(digitCount=0, i=exponent+1; i<0; i++, digitCount--) {
          if(digitCount!=0 && groupingGap!=0 && digitCount%(uint16_t)groupingGap==0) {
            xcopy(displayString + charIndex, separator, 2);
            charIndex += 2;
          }
          displayString[charIndex++] = '0';
          if(updateDisplayValueX) {
            displayValueX[valueIndex++] = '0';
          }
        }

        if(SigFigMode != 0) { lastDigit = firstDigit + SigFigMode; }                          //JM SIGFIG: Neg exponent

        // Significant digits
        for(digitPointer=firstDigit; digitPointer<=lastDigit; digitPointer++, digitCount--) {
          if(digitCount!=0 && groupingGap!=0 && digitCount%(uint16_t)groupingGap==0) {
            xcopy(displayString + charIndex, separator, 2);
            charIndex += 2;
          }
          displayString[charIndex++] = '0' + bcd[digitPointer];
          if(updateDisplayValueX) {
            displayValueX[valueIndex++] = '0' + bcd[digitPointer];
          }
        }

        if(SigFigMode == 0) {                                                                 //JM SIGFIG: Not required to pad more zerooes, as all sigfig zeroes are included above
        // Zeros after last significant digit
        for(i=1; i<=(int16_t)displayFormatDigits+exponent+1-numDigits; i++, digitCount--) {
          if(groupingGap!=0 && digitCount%(uint16_t)groupingGap==0) {
            xcopy(displayString + charIndex, separator, 2);
            charIndex += 2;
          }
          displayString[charIndex++] = '0';
          if(updateDisplayValueX) {
            displayValueX[valueIndex++] = '0';
          }
        }
        }                                                                                     //JM SIGFIG ^
      }
      else { // zero or positive exponent
        SigFigTmp = 0;                                                                        //JM SIGFIG
        SigFigCnt = 0;                                                                        //JM SIGFIG
        for(digitCount=exponent, digitPointer=firstDigit; digitPointer<=firstDigit + min(exponent + (int16_t)displayFormatDigits, 15); digitPointer++, digitCount--) {
          if(digitCount!=-1 && digitCount!=exponent && groupingGap!=0 && modulo(digitCount, (uint16_t)groupingGap) == (uint16_t)groupingGap - 1) {
            xcopy(displayString + charIndex, separator, 2);
            charIndex += 2;
          }

          if((SigFigMode == 0) || (SigFigTmp == 0) || (SigFigTmp == 1 && SigFigCnt+1 <= SigFig)) {  //JM SIGFIG: Put '0' unless number of sigfig digits already reached
          // Significant digit or zero
          if(digitPointer <= lastDigit) {
            displayString[charIndex++] = '0' + bcd[digitPointer];
            if(updateDisplayValueX) {
              displayValueX[valueIndex++] = '0' + bcd[digitPointer];
            }
          }
          else {
            displayString[charIndex++] = '0';
            if(updateDisplayValueX) {
              displayValueX[valueIndex++] = '0';
            }
          }
            SigFigCnt++;                                                                       //JM SIGFIG
          }                                                                                    //JM SIGFIG

          // Radix mark
          if(digitCount == 0) {
            SigFigTmp = 1;                                                                     //JM SIGFIG Set flag to signal decimal point was reached.
            displayString[charIndex] = 0;
            strcat(displayString, RADIX34_MARK_STRING);
            charIndex++;
            if(updateDisplayValueX) {
              displayValueX[valueIndex++] = '.';
            }
          }
        }
      }

      displayString[charIndex] = 0;
      if(updateDisplayValueX) {
        displayValueX[valueIndex] = 0;
      }
      return;
    }
  displayoverflow = true;  //JM Need to know if overflowing. Otherwise UNIT cannot work.
  }

  //////////////
  // SCI mode //
  //////////////
  if((displayoverflow && getSystemFlag(FLAG_ALLSCI)) || displayFormat == DF_SCI) {
    // Round the displayed number
    if((!displayoverflow /*&& !getSystemFlag(FLAG_ALLSCI)*/)) {                      //JM The only condition needed is whether it was in FIX/ALL or not
      digitsToDisplay = displayFormatDigits;
      digitToRound    = min(firstDigit + (int16_t)displayFormatDigits, lastDigit);
    }

    if(bcd[digitToRound + 1] >= 5) {
      bcd[digitToRound]++;
    }

    // Transfert the carry
    while(bcd[digitToRound] == 10) {
      bcd[digitToRound--] = 0;
      numDigits--;
      bcd[digitToRound]++;
    }

    // Case when 9.9999 rounds to 10.0000
    if(digitToRound < firstDigit) {
      firstDigit--;
      lastDigit = firstDigit;
      numDigits = 1;
      exponent++;
    }

    // Sign
    if(sign) {
      displayString[charIndex++] = '-';
      if(updateDisplayValueX) {
        displayValueX[valueIndex++] = '-';
      }
    }

    // First digit
    displayString[charIndex++] = '0' + bcd[firstDigit];
    if(updateDisplayValueX) {
      displayValueX[valueIndex++] = '0' + bcd[firstDigit];
    }

    // Radix mark
    displayString[charIndex] = 0;
    strcat(displayString, RADIX34_MARK_STRING);
    charIndex++;
    if(updateDisplayValueX) {
      displayValueX[valueIndex++] = '.';
    }

    // Significant digits
    for(digitCount=-1, digitPointer=firstDigit+1; digitPointer<firstDigit+min(numDigits, digitsToDisplay+1); digitPointer++, digitCount--) {
      if(!firstDigitAfterPeriod && groupingGap!=0 && modulo(digitCount, (uint16_t)groupingGap) == (uint16_t)groupingGap - 1) {
        xcopy(displayString + charIndex, separator, 2);
        charIndex += 2;
      }
      else {
        firstDigitAfterPeriod = false;
      }

      displayString[charIndex++] = '0' + bcd[digitPointer];
      if(updateDisplayValueX) {
        displayValueX[valueIndex++] = '0' + bcd[digitPointer];
      }
    }

    // The ending zeros
    for(digitPointer=0; digitPointer<=digitsToDisplay-numDigits; digitPointer++, digitCount--) {
      if(!firstDigitAfterPeriod && groupingGap!=0 && modulo(digitCount, (uint16_t)groupingGap) == (uint16_t)groupingGap - 1) {
        xcopy(displayString + charIndex, separator, 2);
        charIndex += 2;
      }
      else {
        firstDigitAfterPeriod = false;
      }

      displayString[charIndex++] = '0';
      if(updateDisplayValueX) {
        displayValueX[valueIndex++] = '0';
      }
    }

    displayString[charIndex] = 0;
    if(updateDisplayValueX) {
      displayValueX[valueIndex] = 0;
    }

    if(exponent != 0) {
      if(updateDisplayValueX) {
        exponentToDisplayString(exponent, displayString + charIndex, displayValueX + valueIndex, false, separator);
      }
      else {
        exponentToDisplayString(exponent, displayString + charIndex, NULL,                       false, separator);
      }
    }
    return;
  }

  //////////////
  // ENG mode //
  //////////////
  if((displayoverflow && !getSystemFlag(FLAG_ALLSCI)) || displayFormat == DF_ENG) {
    // Round the displayed number
    if((!displayoverflow /*&& getSystemFlag(FLAG_ALLSCI)*/)) {                      //JM The only condition needed is whether it was in FIX/ALL or not
      digitsToDisplay = displayFormatDigits;
      digitToRound    = min(firstDigit + digitsToDisplay, lastDigit);
    }

    if(bcd[digitToRound + 1] >= 5) {
      bcd[digitToRound]++;
    }

    bcd[digitToRound + 1] = 0;
    bcd[digitToRound + 2] = 0;

    // Transfert the carry
    while(bcd[digitToRound] == 10) {
      bcd[digitToRound--] = 0;
      numDigits--;
      bcd[digitToRound]++;
    }

    // Case when 9.9999 rounds to 10.0000
    if(digitToRound < firstDigit) {
      firstDigit--;
      lastDigit = firstDigit;
      numDigits = 1;
      exponent++;
    }

    // The sign
    if(sign) {
      displayString[charIndex++] = '-';
      if(updateDisplayValueX) {
        displayValueX[valueIndex++] = '-';
      }
    }

    // Digits before radix mark
    displayString[charIndex++] = '0' + bcd[firstDigit];
    if(updateDisplayValueX) {
      displayValueX[valueIndex++] = '0' + bcd[firstDigit];
    }
    firstDigit++;
    numDigits--;
    digitsToDisplay--;
    while(modulo(exponent, 3) != 0) {
      exponent--;
      displayString[charIndex++] = '0' + bcd[firstDigit];
      if(updateDisplayValueX) {
        displayValueX[valueIndex++] = '0' + bcd[firstDigit];
      }
      firstDigit++;
      numDigits--;
      digitsToDisplay--;
    }

    // Radix Mark
    displayString[charIndex] = 0;
    strcat(displayString, RADIX34_MARK_STRING);
    charIndex++;
    if(updateDisplayValueX) {
      displayValueX[valueIndex++] = '.';
    }

    // Digits after radix mark
    for(digitCount=-1, digitPointer=firstDigit; digitPointer<firstDigit+min(numDigits, digitsToDisplay+1); digitPointer++, digitCount--) {
      if(!firstDigitAfterPeriod && groupingGap!=0 && modulo(digitCount, (uint16_t)groupingGap) == (uint16_t)groupingGap - 1) {
        xcopy(displayString + charIndex, separator, 2);
        charIndex += 2;
      }
      else {
        firstDigitAfterPeriod = false;
      }

      displayString[charIndex++] = '0' + bcd[digitPointer];
      if(updateDisplayValueX) {
        displayValueX[valueIndex++] = '0' + bcd[digitPointer];
      }
    }

    // The ending zeros
    for(digitPointer=0; digitPointer<=digitsToDisplay-max(0, numDigits); digitPointer++, digitCount--) {
      if(!firstDigitAfterPeriod && groupingGap!=0 && modulo(digitCount, (uint16_t)groupingGap) == (uint16_t)groupingGap - 1) {
        xcopy(displayString + charIndex, separator, 2);
        charIndex += 2;
      }
      else {
        firstDigitAfterPeriod = false;
      }

      displayString[charIndex++] = '0';
      if(updateDisplayValueX) {
        displayValueX[valueIndex++] = '0';
      }
    }

    displayString[charIndex] = 0;
    if(updateDisplayValueX) {
      displayValueX[valueIndex] = 0;
    }

    if(exponent != 0) {
      if(UNITDisplay == false) {                                                        //JM UNIT
        if(updateDisplayValueX) {
          exponentToDisplayString(exponent, displayString + charIndex, displayValueX + valueIndex, false, separator);
        }
        else {
          exponentToDisplayString(exponent, displayString + charIndex, NULL,                       false, separator);
        }
      }                                                                                 //JM UNIT
      else {                                                                            //JM UNIT
        exponentToUnitDisplayString(exponent, displayString + charIndex, displayValueX + valueIndex, false, separator);          //JM UNIT
      }                                                                                 //JM UNIT
    }
  }
}



void complex34ToDisplayString(const complex34_t *complex34, char *displayString, const font_t *font, int16_t maxWidth, int16_t displayHasNDigits, bool_t limitExponent, const char *separator) {
  uint8_t savedDisplayFormatDigits = displayFormatDigits;

  if(updateDisplayValueX) {
    displayValueX[0] = 0;
  }

  complex34ToDisplayString2(complex34, displayString, displayHasNDigits, limitExponent, separator);
  while(stringWidth(displayString, font, true, true) > maxWidth) {
    if(displayFormat == DF_ALL) {
      if(displayHasNDigits == 2) {
        break;
      }
      displayHasNDigits--;
    }
    else {
      if(displayFormatDigits == 0) {
        break;
      }
      displayFormatDigits--;
    }

    if(updateDisplayValueX) {
      displayValueX[0] = 0;
    }

    complex34ToDisplayString2(complex34, displayString, displayHasNDigits, limitExponent, separator);
  }
  displayFormatDigits = savedDisplayFormatDigits;
}



void complex34ToDisplayString2(const complex34_t *complex34, char *displayString, int16_t displayHasNDigits, bool_t limitExponent, const char *separator) {
  int16_t i=100;
  real34_t real34, imag34;
  real_t real, imagIc;

  if(getSystemFlag(FLAG_RECTN)) { // rectangular mode
    real34Copy(VARIABLE_REAL34_DATA(complex34), &real34);
    real34Copy(VARIABLE_IMAG34_DATA(complex34), &imag34);
  }
  else { // polar mode
    real34ToReal(VARIABLE_REAL34_DATA(complex34), &real);
    real34ToReal(VARIABLE_IMAG34_DATA(complex34), &imagIc);
    realRectangularToPolar(&real, &imagIc, &real, &imagIc, &ctxtReal39); // imagIc in radian
    convertAngleFromTo(&imagIc, AM_RADIAN, currentAngularMode, &ctxtReal39);
    realToReal34(&real, &real34);
    realToReal34(&imagIc, &imag34);
  }

  realToDisplayString2(&real34, displayString, displayHasNDigits, limitExponent, separator);

  if(updateDisplayValueX) {
    if(getSystemFlag(FLAG_RECTN)) {
      strcat(displayValueX, "i");
    }
    else {
      strcat(displayValueX, "j");
    }
  }

  realToDisplayString2(&imag34, displayString + i, displayHasNDigits, limitExponent, separator);

  if(getSystemFlag(FLAG_RECTN)) { // rectangular mode
    if(strncmp(displayString + stringByteLength(displayString) - 2, STD_SPACE_HAIR, 2) != 0) {
      strcat(displayString, STD_SPACE_HAIR);
    }

    if(displayString[i] == '-') {
      strcat(displayString, "-");
      i++;
    }
    else {
      strcat(displayString, "+");
    }

    strcat(displayString, COMPLEX_UNIT);
    strcat(displayString, PRODUCT_SIGN);
    xcopy(strchr(displayString, '\0'), displayString + i, strlen(displayString + i) + 1);
  }
  else { // polar mode
    strcat(displayString, STD_SPACE_4_PER_EM STD_MEASURED_ANGLE STD_SPACE_4_PER_EM);
    angle34ToDisplayString2(&imag34, currentAngularMode, displayString + stringByteLength(displayString), displayHasNDigits, limitExponent, separator);
  }
}



/********************************************//**
 * \brief formats a fraction
 *
 * \param regist calcRegister_t
 * \param displayString char*
 * \return void
 ***********************************************/
void fractionToDisplayString(calcRegister_t regist, char *displayString) {
  int16_t  sign, lessEqualGreater;
  uint64_t intPart, numer, denom;
  int16_t  u, insertAt, endingZero, gap;

  //printf("regist = "); printRegisterToConsole(regist); printf("\n");
  fraction(regist, &sign, &intPart, &numer, &denom, &lessEqualGreater);
  //printf("result of fraction(...) = %c%" FMT64U " %" FMT64U "/%" FMT64U "\n", sign==-1 ? '-' : ' ', intPart, numer, denom);

  if(getSystemFlag(FLAG_PROPFR)) { // a b/c
    if(updateDisplayValueX) {
      sprintf(displayValueX, "%s%" FMT64U " %" FMT64U "/%" FMT64U, (sign == -1 ? "-" : ""), intPart, numer, denom);
    }

    if(sign == -1) {
      displayString[0] = '-';
      displayString[1] = 0;
      endingZero = 1;
    }
    else {
      displayString[0] = 0;
      endingZero = 0;

      if(updateDisplayValueX) {
      }
    }

    // Integer part
    insertAt = endingZero;
    gap = -1;
    do {
      gap++;
      if(gap == groupingGap) {
        gap = 0;
        endingZero++;
        xcopy(displayString + insertAt + 2, displayString + insertAt, endingZero++ - insertAt);
        *(displayString + insertAt)     = *(STD_SPACE_PUNCTUATION);
        *(displayString + insertAt + 1) = *(STD_SPACE_PUNCTUATION + 1);
      }

      u = intPart % 10;
      intPart /= 10;
      endingZero++;
      xcopy(displayString + insertAt + 1, displayString + insertAt, endingZero - insertAt);
      displayString[insertAt] = '0' + u;
    } while(intPart != 0);

    strcat(displayString, STD_SPACE_PUNCTUATION);
    endingZero += 2;
  }

  else { // FT_IMPROPER d/c
    if(updateDisplayValueX) {
      sprintf(displayValueX, "%s%" FMT64U "/%" FMT64U, (sign == -1 ? "-" : ""), numer, denom);
    }

    if(sign == -1) {
      strcpy(displayString, STD_SUP_MINUS);
      endingZero = 2;
    }
    else {
      displayString[0] = 0;
      endingZero = 0;
    }
  }

  // Numerator
  insertAt = endingZero;
  gap = -1;
  do {
    gap++;
    if(gap == groupingGap) {
      gap = 0;
      endingZero++;
      xcopy(displayString + insertAt + 2, displayString + insertAt, endingZero++ - insertAt);
      *(displayString + insertAt)     = *(STD_SPACE_PUNCTUATION);
      *(displayString + insertAt + 1) = *(STD_SPACE_PUNCTUATION + 1);
    }

    u = numer % 10;
    numer /= 10;
    endingZero++;
    xcopy(displayString + insertAt + 2, displayString + insertAt, endingZero++ - insertAt);

    if(u <= 1) {
      *(displayString + insertAt)     = *(STD_SUP_0);
      *(displayString + insertAt + 1) = *(STD_SUP_0 + 1);
      displayString[insertAt + 1] += u;
    }
    else if(u <= 3) {
      *(displayString + insertAt)     = *(STD_SUP_2);
      *(displayString + insertAt + 1) = *(STD_SUP_2 + 1);
      displayString[insertAt + 1] += u - 2;
    }
    else {
      *(displayString + insertAt)     = *(STD_SUP_4);
      *(displayString + insertAt + 1) = *(STD_SUP_4 + 1);
      displayString[insertAt + 1] += u - 4;
    }

  } while(numer != 0);


  // Fraction bar
  strcat(displayString, "/");
  endingZero++;


  // Denominator
  insertAt = endingZero;
  gap = -1;
  do {
    gap++;
    if(gap == groupingGap) {
      gap = 0;
      endingZero++;
      xcopy(displayString + insertAt + 2, displayString + insertAt, endingZero++ - insertAt);
      *(displayString + insertAt)     = *(STD_SPACE_PUNCTUATION);
      *(displayString + insertAt + 1) = *(STD_SPACE_PUNCTUATION + 1);
    }

    u = denom % 10;
    denom /= 10;
    endingZero++;
    xcopy(displayString + insertAt + 2, displayString + insertAt, endingZero++ - insertAt);
    *(displayString + insertAt)     = *(STD_SUB_0);
    *(displayString + insertAt + 1) = *(STD_SUB_0 + 1);
    displayString[insertAt + 1] += u;
  } while(denom != 0);

  // Comparison sign
       if(lessEqualGreater == -1) strcat(displayString, STD_SPACE_PUNCTUATION "<" STD_SPACE_HAIR);
  else if(lessEqualGreater ==  0) strcat(displayString, STD_SPACE_PUNCTUATION "=" STD_SPACE_HAIR);
  else if(lessEqualGreater ==  1) strcat(displayString, STD_SPACE_PUNCTUATION ">" STD_SPACE_HAIR);
  else {
    strcat(displayString, STD_SPACE_PUNCTUATION "?");
    sprintf(errorMessage, "In function fractionToDisplayString: %d is an unexpected value for lessEqualGreater!", lessEqualGreater);
    displayBugScreen(errorMessage);
  }
}



void angle34ToDisplayString2(const real34_t *angle34, uint8_t mode, char *displayString, int16_t displayHasNDigits, bool_t limitExponent, const char *separator) {
  if(mode == AM_DMS) {
    char degStr[27];
    uint32_t m, s, fs;
    int16_t sign;
    bool_t overflow;

    real_t temp, degrees, minutes, seconds;

    real34ToReal(angle34, &temp);

    sign = realIsNegative(&temp) ? -1 : 1;
    realSetPositiveSign(&temp);

    // Get the degrees
    realToIntegralValue(&temp, &degrees, DEC_ROUND_DOWN, &ctxtReal39);

    // Get the minutes
    realSubtract(&temp, &degrees, &temp, &ctxtReal39);
    realMultiply(&temp, const_100, &temp, &ctxtReal39);
    realToIntegralValue(&temp, &minutes, DEC_ROUND_DOWN, &ctxtReal39);

    // Get the seconds
    realSubtract(&temp, &minutes, &temp, &ctxtReal39);
    realMultiply(&temp, const_100, &temp, &ctxtReal39);
    realToIntegralValue(&temp, &seconds, DEC_ROUND_DOWN, &ctxtReal39);

    // Get the fractional seconds
    realSubtract(&temp, &seconds, &temp, &ctxtReal39);
    realMultiply(&temp, const_100, &temp, &ctxtReal39);

    realToUInt32(&temp, DEC_ROUND_DOWN, &fs, &overflow);
    realToUInt32(&seconds, DEC_ROUND_DOWN, &s, &overflow);
    realToUInt32(&minutes, DEC_ROUND_DOWN, &m, &overflow);

    if(fs >= 100) {
      fs -= 100;
      s++;
    }

    if(s >= 60) {
      s -= 60;
      m++;
    }

    if(m >= 60) {
      m -= 60;
      realAdd(&degrees, const_1, &degrees, &ctxtReal39);
    }

    realToString(&degrees, degStr);
    for(int32_t i=0; degStr[i]!=0; i++) {
      if(degStr[i] == '.') {
        degStr[i] = 0;
        break;
      }
    }

    sprintf(displayString, "%s%s" STD_DEGREE "%s%" FMT32U STD_QUOTE "%s%" FMT32U "%s%02" FMT32U STD_DOUBLE_QUOTE,
                            sign==-1 ? "-" : "",
                              degStr,         m < 10 ? STD_SPACE_FIGURE : "",
                                                m,                   s < 10 ? STD_SPACE_FIGURE : "",
                                                                       s,         RADIX34_MARK_STRING,
                                                                                    fs);
  }
  else {
    realToDisplayString2(angle34, displayString, displayHasNDigits, limitExponent, separator);

         if(mode == AM_DEGREE) strcat(displayString, STD_DEGREE);
    else if(mode == AM_GRAD)   strcat(displayString, STD_SUP_g);
    else if(mode == AM_MULTPI) strcat(displayString, STD_pi);
    else if(mode == AM_RADIAN) strcat(displayString, STD_SUP_r);
    else {
      strcat(displayString, "?");
      sprintf(errorMessage, "In function angle34ToDisplayString2: %" FMT8U " is an unexpected value for mode!", mode);
      displayBugScreen(errorMessage);
    }
  }
}



void shortIntegerToDisplayString(calcRegister_t regist, char *displayString, bool_t determineFont) {
  int16_t i, j, k, unit, gap, digit, bitsPerDigit, maxDigits, base;
  uint64_t number, sign;
  static const char digits[17] = "0123456789ABCDEF";

  base    = getRegisterTag(regist);
  number  = *(REGISTER_SHORT_INTEGER_DATA(regist));

  if(base <= 1 || base >= 17) {
    sprintf(errorMessage, "In function shortIntegerToDisplayString: %d is an unexpected value for base!", base);
    displayBugScreen(errorMessage);
    base = 10;
  }

  number &= shortIntegerMask;

  if(shortIntegerMode == SIM_UNSIGN || base == 2 || base == 4 || base == 8 || base == 16) {
    sign = 0;
  }
  else {
    sign = number & shortIntegerSignBit;
  }

  if(sign) {
    if(shortIntegerMode == SIM_2COMPL) {
      number |= ~shortIntegerMask;
      number = ~number + 1;
    }
    else if(shortIntegerMode == SIM_1COMPL) {
      number = ~number;
    }
    else if(shortIntegerMode == SIM_SIGNMT) {
      number &= ~shortIntegerSignBit;
    }
    else {
      sprintf(errorMessage, "In function shortIntegerToDisplayString: %d is an unexpected value for shortIntegerMode!", shortIntegerMode);
      displayBugScreen(errorMessage);
    }

    number &= shortIntegerMask;
  }

  i = ERROR_MESSAGE_LENGTH / 2;

  if(number == 0) {
    displayString[i++] = '0';
    digit = 1;
  }
  else {
    digit = 0;
  }

  if(groupingGap == 0) {
    gap = 0;
  }
  else {
    if(base == 2) {
      gap = 4;
    }
    else if(base == 4 || base == 16) { //JMGAP vv
      gap = 2;
    }
    else if(base == 8) {
      gap = 3;                         //  keeping base == 8 separate as opposed to just dropping it from base == 4 and base == 16  allows it to be changed back to 2 easily.
    }                                  //JMGAP ^^
    else {
      gap = 3;
    }
  }

  while(number) {
    if(gap != 0 && digit != 0 && digit%gap == 0) {
      displayString[i++] = ' ';
    }
    digit++;

    unit = number % base;
    number /= base;
    displayString[i++] = digits[unit];
  }

  // Add leading zeros
  if(getSystemFlag(FLAG_LEAD0)) {
         if(base ==  2) bitsPerDigit = 1;
    else if(base ==  4) bitsPerDigit = 2;
    else if(base ==  8) bitsPerDigit = 3;
    else if(base == 16) bitsPerDigit = 4;
    else                bitsPerDigit = 0;

    if(bitsPerDigit != 0) {
      maxDigits = shortIntegerWordSize / bitsPerDigit;
      if(shortIntegerWordSize % bitsPerDigit) {
        maxDigits++;
      }

      while(digit < maxDigits) {
        if(gap != 0 && digit%gap == 0) {
          displayString[i++] = ' ';
        }
        digit++;

        displayString[i++] = '0';
      }
    }
  }

  if(sign) {
    displayString[i++] = '-';
  }

  if(determineFont) { // The font is not yet determined
    // 1st try: numeric font digits from 30 to 39
    fontForShortInteger = &numericFont;

    for(k=i-1, j=0; k>=ERROR_MESSAGE_LENGTH / 2; k--, j++) {
      if(displayString[k] == ' ') {
        displayString[j++] = 0xa0;
        displayString[j]   = 0x08;
      }
      else {
        displayString[j] = displayString[k];
      }
    }
    displayString[j] = 0;

    strcat(displayString, STD_BASE_2);
    displayString[strlen(displayString) - 1] += base - 2;

    if(stringWidth(displayString, fontForShortInteger, false, false) < SCREEN_WIDTH) {
      return;
    }

    // 2nd try: numeric font digits from 2487 to 2490
    for(k=i-1, j=0; k>=ERROR_MESSAGE_LENGTH / 2; k--, j++) {
      if(displayString[k] == ' ') {
        displayString[j++] = 0xa0;
        displayString[j]   = 0x08;
      }
      else if(0x30 <= displayString[k] && displayString[k] <= 0x39) {
        displayString[j++] = 0xa4;
        displayString[j] = 0x87 - '0' + displayString[k];
      }
      else {
        displayString[j] = displayString[k];
      }
    }
    displayString[j] = 0;

    strcat(displayString, STD_BASE_2);
    displayString[strlen(displayString) - 1] += base - 2;

    if(stringWidth(displayString, fontForShortInteger, false, false) < SCREEN_WIDTH) {
      return;
    }

    // 3rd try: standard font digits from 30 to 39
    fontForShortInteger = &standardFont;

    for(k=i-1, j=0; k>=ERROR_MESSAGE_LENGTH / 2; k--, j++) {
      if(displayString[k] == ' ') {
        displayString[j++] = 0xa0;
        displayString[j]   = 0x08;
      }
      else {
       displayString[j] = displayString[k];
      }
    }
    displayString[j] = 0;

    strcat(displayString, STD_BASE_2);
    displayString[strlen(displayString) - 1] += base - 2;

    if(temporaryInformation == TI_SHOW_REGISTER_BIG || stringWidth(displayString, fontForShortInteger, false, false) < SCREEN_WIDTH) {     //JMSHOW
      return;
    }

    // 4th and last try: standard font digits 220e and 2064 (binary)
    for(k=i-1, j=0; k>=ERROR_MESSAGE_LENGTH / 2; k--, j++) {
      if(displayString[k] == ' ') {
        displayString[j++] = 0xa0;
        displayString[j]   = 0x08;
      }
      else if(displayString[k] == '0') {
        displayString[j++] = 0xa2;
        displayString[j] = 0x0e;
      }
      else if(displayString[k] == '1') {
        displayString[j++] = 0xa0;
        displayString[j] = 0x27;
      }
      else {
        displayString[j] = displayString[k];
      }
    }
    displayString[j] = 0;

    strcat(displayString, STD_BASE_2);
    displayString[strlen(displayString) - 1] += base - 2;

    if(stringWidth(displayString, fontForShortInteger, false, false) < SCREEN_WIDTH) {
      return;
    }

    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function shortIntegerToDisplayString: the integer data representation is too wide (1)!", displayString, NULL, NULL);
    #endif

    strcpy(displayString, "Integer data representation to wide!");
  }

  else { // the font is already determined (standard font)
    fontForShortInteger = &standardFont;

    // 1st try: standard font digits from 30 to 39
    for(k=i-1, j=0; k>=ERROR_MESSAGE_LENGTH / 2; k--, j++) {
      if(displayString[k] == ' ') {
        displayString[j++] = 0xa0;
        displayString[j]   = 0x08;
      }
      else {
        displayString[j] = displayString[k];
      }
    }
    displayString[j] = 0;

    strcat(displayString, STD_BASE_2);
    displayString[strlen(displayString) - 1] += base - 2;

    if(stringWidth(displayString, fontForShortInteger, false, false) < SCREEN_WIDTH) {
      return;
    }

    // 2nd and last try: standard font digits 220e and 2064 (binary)
    for(k=i-1, j=0; k>=ERROR_MESSAGE_LENGTH / 2; k--, j++) {
      if(displayString[k] == ' ') {
        displayString[j++] = 0xa0;
        displayString[j]   = 0x08;
      }
      else if(displayString[k] == '0') {
        displayString[j++] = 0xa2;
        displayString[j] = 0x0e;
      }
      else if(displayString[k] == '1') {
        displayString[j++] = 0xa0;
        displayString[j] = 0x27;
      }
      else {
       displayString[j] = displayString[k];
      }
    }
    displayString[j] = 0;

    strcat(displayString, STD_BASE_2);
    displayString[strlen(displayString) - 1] += base - 2;

    if(stringWidth(displayString, fontForShortInteger, false, false) < SCREEN_WIDTH) {
      return;
    }

    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      showInfoDialog("In function shortIntegerToDisplayString: the integer data representation is too wide (2)!", displayString, NULL, NULL);
    #endif

    strcpy(displayString, "Integer data representation to wide!");
  }
}



void longIntegerRegisterToDisplayString(calcRegister_t regist, char *displayString, int32_t strLg, int16_t max_Width, int16_t maxExp, const char *separator) { //JM mod
  int16_t len, exponentStep;
  uint32_t exponentShift, exponentShiftLimit;
  longInteger_t lgInt;
  int16_t maxWidth;                                         //JM align longints

  convertLongIntegerRegisterToLongInteger(regist, lgInt);

  if(longIntegerIsNegative(lgInt)) {maxWidth = max_Width;}  //JM align longints
  else {maxWidth = max_Width - 8;}                          //JM align longints

  exponentShift = (longIntegerBits(lgInt) - 1) * 0.3010299956639811952137;
  exponentStep = (groupingGap == 0 ? 1 : groupingGap);
  exponentShift = (exponentShift / exponentStep + 1) * exponentStep;
  exponentShiftLimit = (maxExp / exponentStep + 1) * exponentStep;
  if(exponentShift > exponentShiftLimit) {
    exponentShift -= exponentShiftLimit;

    // Why do the following lines not work (for a big exponentShift) instead of the for loop below ?
    //longIntegerInitSizeInBits(divisor, longIntegerBits(lgInt));
    //longIntegerPowerUIntUInt(10u, exponentShift, divisor);
    //longIntegerDivide(lgInt, divisor, lgInt);
    //longIntegerFree(divisor);
    for(int32_t i=(int32_t)exponentShift; i>=1; i--) {
      if(i >= 9)      {longIntegerDivideUInt(lgInt, 1000000000, lgInt); i -= 8;}
      else if(i >= 4) {longIntegerDivideUInt(lgInt,      10000, lgInt); i -= 3;}
      else            {longIntegerDivideUInt(lgInt,         10, lgInt);}
    }
  }
  else {
    exponentShift = 0;
  }

  longIntegerToAllocatedString(lgInt, displayString, strLg);
  if(updateDisplayValueX) {
    strcpy(displayValueX, displayString);
  }

  longIntegerFree(lgInt);

  if(groupingGap > 0) {
    len = strlen(displayString);
    for(int16_t i=len - groupingGap; i>0; i-=groupingGap) {
      if(i != 1 || displayString[0] != '-') {
        xcopy(displayString + i + 2, displayString + i, len - i + 1);
        displayString[i] = *separator;
        displayString[i + 1] = *(separator + 1);
        len += 2;
      }
    }
  }

  if(stringWidth(displayString, &standardFont, false, false) > maxWidth) {
    char exponentString[14], lastRemovedDigit;
    int16_t lastChar, stringStep, tenExponent;

    stringStep = (groupingGap == 0 ? 1 : groupingGap + 2);
    tenExponent = exponentStep + exponentShift;
    lastChar = strlen(displayString) - stringStep;
    lastRemovedDigit = displayString[lastChar + 2];
    displayString[lastChar] = 0;
    if(updateDisplayValueX) {
      displayValueX[strlen(displayValueX) - max(groupingGap, 1)] = 0;
    }
    exponentString[0] = 0;
    exponentToDisplayString(tenExponent, exponentString, NULL, false, separator);
    while(stringWidth(displayString, &standardFont, false, true) + stringWidth(exponentString, &standardFont, true, false) > maxWidth) {
      lastChar -= stringStep;
      tenExponent += exponentStep;
      lastRemovedDigit = displayString[lastChar + 2];
      displayString[lastChar] = 0;
      if(updateDisplayValueX) {
        displayValueX[strlen(displayValueX) - max(groupingGap, 1)] = 0;
      }
      exponentString[0] = 0;
      exponentToDisplayString(tenExponent, exponentString, NULL, false, separator);
    }

    if(lastRemovedDigit >= '5') { // Round up
      lastChar = strlen(displayString) - 1;
      displayString[lastChar]++;
      while(displayString[lastChar] > '9') {
        displayString[lastChar--] = '0';
        while(lastChar >= 0 && (displayString[lastChar] < '0' || displayString[lastChar] > '9')) lastChar--;
        if(lastChar >= 0) {
          displayString[lastChar]++;
        }
        else { // We are rounding up from 9999... to 10000...
          lastChar = (displayString[0] == '-' ? 1 : 0);
          xcopy(displayString + lastChar + 1, displayString + lastChar, strlen(displayString) + 1);
          displayString[lastChar] = '1';
          if(groupingGap != 0 && displayString[lastChar + groupingGap + 2] == *(separator + 1)) { // We need to insert a new goup separator
            xcopy(displayString + lastChar + 3, displayString + lastChar + 1, strlen(displayString));
            displayString[lastChar + 1] = *separator;
            displayString[lastChar + 2] = *(separator + 1);
          }

          // Has the string become too long?
          if(stringWidth(displayString, &standardFont, false, true) + stringWidth(exponentString, &standardFont, true, false) > maxWidth) {
            lastChar = strlen(displayString) - stringStep;
            tenExponent += exponentStep;
            displayString[lastChar] = 0;
            if(updateDisplayValueX) {
              displayValueX[strlen(displayValueX) - max(groupingGap, 1)] = 0;
            }
            exponentString[0] = 0;
            exponentToDisplayString(tenExponent, exponentString, NULL, false, separator);
          }
        }
      }

      if(updateDisplayValueX) {
        lastChar = strlen(displayValueX) - 1;
        displayValueX[lastChar]++;
        while(lastChar>0 && '0' <= displayValueX[lastChar - 1] && displayValueX[lastChar - 1] <= '9' && displayValueX[lastChar] > '9') {
          displayValueX[lastChar--] = '0';
          displayValueX[lastChar]++;
        }

        if(displayValueX[lastChar] > '9') { // We are rounding 9999... to 10000...
          xcopy(displayValueX + 1, displayValueX, strlen(displayValueX) + 1);
          displayValueX[lastChar++] = '1';
          displayValueX[lastChar] = '0';
        }
      }
    }

    strcat(displayString, exponentString);

    if(updateDisplayValueX) {
      sprintf(displayValueX + strlen(displayValueX), "e%d", tenExponent);
    }
  }
}



void longIntegerToAllocatedString(const longInteger_t lgInt, char *str, int32_t strLen) {
  int32_t digits, stringLen, counter;
  longInteger_t x;

  str[0] = '0';
  str[1] = 0;
  if(lgInt->_mp_size == 0) {
  //if(longIntegerIsZero(lgInt)) {
    return;
  }

  //digits = longIntegerBase10Digits(lgInt); // GMP documentation says the result can be 1 to big
  digits = mpz_sizeinbase(lgInt, 10); // GMP documentation says the result can be 1 to big
  //if(longIntegerIsNegative(lgInt)) {
  if(lgInt->_mp_size < 0) {
    stringLen = digits + 2; // 1 for the trailing 0 and 1 for the minus sign
    str[0] = '-';
  }
  else {
    stringLen = digits + 1; // 1 for the trailing 0
  }

  if(strLen < stringLen) {
    printf("In function longIntegerToAllocatedString: the string str (%" FMT32S " bytes) is too small to hold the base 10 representation of lgInt, %" FMT32S " are needed!\n", strLen, stringLen);
    return;
  }

  str[stringLen - 1] = 0;

  //longIntegerInitSizeInBits(x, longIntegerBits(lgInt));
  mpz_init2(x, mpz_sizeinbase(lgInt, 2));

  //longIntegerAddUInt(lgInt, 0, x);
  mpz_add_ui(x, lgInt, 0);

  //longIntegerSetPositiveSign(x);
  x->_mp_size =  abs(x->_mp_size);


  stringLen -= 2; // set stringLen to the last digit of the base 10 representation
  counter = digits;
  //while(!longIntegerIsZero(x)) {
  while(x->_mp_size != 0) {
    str[stringLen--] = '0' + mpz_tdiv_ui(x, 10);

    //longIntegerDivideUInt(x, 10, x);
    mpz_tdiv_q_ui(x, x, 10);

    counter--;
  }

  if(counter == 1) { // digit was 1 too big
    xcopy(str + stringLen, str + stringLen + 1, digits);
  }

  //longIntegerFree(x);
  mpz_clear(x);
}



void dateToDisplayString(calcRegister_t regist, char *displayString) {
  sprintf(displayString, "%" FMT64S, *(int64_t *)(REGISTER_DATA(regist)));
}



void timeToDisplayString(calcRegister_t regist, char *displayString) {
  sprintf(displayString, "%" FMT64S, *(int64_t *)(REGISTER_DATA(regist)));
}



void fnShow(uint16_t unusedParamButMandatory) {
  uint8_t savedDisplayFormat = displayFormat, savedDisplayFormatDigits = displayFormatDigits;
  uint8_t savedSigFigMode = SigFigMode;           //JM
  bool_t savedUNITDisplay = UNITDisplay;          //JM

  int16_t source, dest, last, d, maxWidth, offset, bytesProcessed;
  real34_t real34;
  char *separator;
  bool_t thereIsANextLine;

  displayFormat = DF_ALL;
  displayFormatDigits = 0;
  SigFigMode = 0;                                //JM SIGFIG
  UNITDisplay = false;                           //JM SIGFIG

  tmpStr3000[   0] = 0; // L1
  tmpStr3000[ 300] = 0; // L2
  tmpStr3000[ 600] = 0; // L3
  tmpStr3000[ 900] = 0; // L4
  tmpStr3000[1200] = 0; // L5
  tmpStr3000[1500] = 0; // L6
  tmpStr3000[1800] = 0; // L7

  temporaryInformation = TI_SHOW_REGISTER;

  switch(getRegisterDataType(REGISTER_X)) {
    case dtLongInteger:
      separator = STD_SPACE_4_PER_EM;
      longIntegerRegisterToDisplayString(REGISTER_X, tmpStr3000 + 2100, TMP_STR_LENGTH, 3200, 400, separator);

      last = 2100 + stringByteLength(tmpStr3000 + 2100);
      source = 2100;
      dest = 0;

      if(groupingGap == 0) {
        maxWidth = SCREEN_WIDTH - stringWidth("0", &standardFont, true, true);
      }
      else {
        maxWidth = SCREEN_WIDTH - stringWidth("0", &standardFont, true, true)*groupingGap - stringWidth(separator, &standardFont, true, true);
      }

      for(d=0; d<=1800 ; d+=300) {
        dest = d;
        while(source < last && stringWidth(tmpStr3000 + d, &standardFont, true, true) <= maxWidth) {
          do {
            tmpStr3000[dest] = tmpStr3000[source];
            if(tmpStr3000[dest] & 0x80) {
              tmpStr3000[++dest] = tmpStr3000[++source];
            }
            source++;
            tmpStr3000[++dest] = 0;
          } while(source < last && groupingGap > 0 && (tmpStr3000[source] != *separator || tmpStr3000[source + 1] != *(separator + 1)));
        }
      }

      if(source < last) { // The long integer is too long
        xcopy(tmpStr3000 + dest - 2, STD_ELLIPSIS, 2);
        xcopy(tmpStr3000 + dest, STD_SPACE_6_PER_EM, 2);
        tmpStr3000[dest + 2] = 0;
      }
      break;

    case dtReal34:
      real34ToDisplayString(REGISTER_REAL34_DATA(REGISTER_X), getRegisterAngularMode(REGISTER_X), tmpStr3000, &standardFont, 2000, 34, false, STD_SPACE_4_PER_EM);
      break;

    case dtComplex34:
      // Real part
      separator = STD_SPACE_4_PER_EM;
      real34ToDisplayString(REGISTER_REAL34_DATA(REGISTER_X), AM_NONE, tmpStr3000, &standardFont, 2000, 34, false, separator);

      // +/- i×
      real34Copy(REGISTER_IMAG34_DATA(REGISTER_X), &real34);
      last = 300;
      while(tmpStr3000[last]) last++;
      xcopy(tmpStr3000 + last++, (real34IsNegative(&real34) ? "-" : "+"), 1);
      xcopy(tmpStr3000 + last++, COMPLEX_UNIT, 1);
      xcopy(tmpStr3000 + last, PRODUCT_SIGN, 3);

      // Imaginary part
      real34SetPositiveSign(&real34);
      real34ToDisplayString(&real34, AM_NONE, tmpStr3000 + 600, &standardFont, 2000, 34, false, separator);

      if(stringWidth(tmpStr3000 + 300, &standardFont, true, true) + stringWidth(tmpStr3000 + 600, &standardFont, true, true) <= SCREEN_WIDTH) {
        last = 300;
        while(tmpStr3000[last]) last++;
        xcopy(tmpStr3000 + last, tmpStr3000 + 600,  strlen(tmpStr3000 + 600) + 1);
        tmpStr3000[600] = 0;
      }

      if(stringWidth(tmpStr3000, &standardFont, true, true) + stringWidth(tmpStr3000 + 300, &standardFont, true, true) <= SCREEN_WIDTH) {
        last = 0;
        while(tmpStr3000[last]) last++;
        xcopy(tmpStr3000 + last, tmpStr3000 +  300, strlen(tmpStr3000 + 300) + 1);
        xcopy(tmpStr3000 + 300, tmpStr3000 + 600, strlen(tmpStr3000 + 600) + 1);
        tmpStr3000[600] = 0;
      }
      break;

    case dtString:
      offset = 0;
      thereIsANextLine = true;
      bytesProcessed = 0;
      while(thereIsANextLine) {
        xcopy(tmpStr3000 + offset, REGISTER_STRING_DATA(REGISTER_X) + bytesProcessed, stringByteLength(REGISTER_STRING_DATA(REGISTER_X) + bytesProcessed) + 1);
        thereIsANextLine = false;
        while(stringWidth(tmpStr3000 + offset, &standardFont, false, true) >= SCREEN_WIDTH) {
          tmpStr3000[offset + stringLastGlyph(tmpStr3000 + offset)] = 0;
          thereIsANextLine = true;
        }
        bytesProcessed += stringByteLength(tmpStr3000 + offset);
        offset += 300;
        tmpStr3000[offset] = 0;
      }
      break;

    default:
      temporaryInformation = TI_NO_INFO;
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "cannot SHOW %s", getRegisterDataTypeName(REGISTER_X, true, false));
        showInfoDialog("In function fnShow:", errorMessage, NULL, NULL);
      #endif
      return;
  }

  refreshRegisterLine(REGISTER_T);
  if(tmpStr3000[ 300]) refreshRegisterLine(REGISTER_Z);
  if(tmpStr3000[ 900]) refreshRegisterLine(REGISTER_Y);
  if(tmpStr3000[1500]) refreshRegisterLine(REGISTER_X);

  displayFormat = savedDisplayFormat;
  displayFormatDigits = savedDisplayFormatDigits;
  SigFigMode = savedSigFigMode;                            //JM SIGFIG
  UNITDisplay = savedUNITDisplay;                          //JM SIGFIG

}





void fnShow_SCROLL(uint16_t fnShow_param) {                // Heavily modified by JM from the original fnShow
  uint8_t savedDisplayFormat = displayFormat, savedDisplayFormatDigits = displayFormatDigits, savedSigFigMode = SigFigMode;
  bool_t savedUNITDisplay = UNITDisplay;
  bool_t thereIsANextLine;
  int16_t source, dest, last, d, maxWidth, i, offset, bytesProcessed;
  real34_t real34;
  char *separator;

  displayFormat = DF_ALL;
  displayFormatDigits = 0;

  #define lowest_SHOW REGISTER_X //0                // Lowest register. Change to 0 for all registers, or use REGISTER_X 
  switch(fnShow_param) {
    case NOPARAM:
    case 0:  SHOWregis = REGISTER_X;
             break;
    case 1:  if(SHOWregis==9999) {SHOWregis = REGISTER_X;}
             else
             {
               SHOWregis++;                         //Activated by KEY_UP
               if(SHOWregis > REGISTER_K) {
                 SHOWregis = lowest_SHOW;  
               }
             }
             break;   
    case 2:  if(SHOWregis==9999) {SHOWregis = REGISTER_X;}
             else
             {
               SHOWregis--;                         //Activate by Key_DOWN
               if(SHOWregis < lowest_SHOW) {
                 SHOWregis = REGISTER_K;  
               } 
             }
             break; 
    case 99:                                        //RESET every time a key is pressed.
             SHOWregis = REGISTER_X;
             return;
             break;
    default: 
      break;
  }


  void SHOW_reset(void){
    tmpStr3000[   0] = 0; // L1
    tmpStr3000[ 300] = 0; // L2
    tmpStr3000[ 600] = 0; // L3
    tmpStr3000[ 900] = 0; // L4
    tmpStr3000[1200] = 0; // L5
    tmpStr3000[1500] = 0; // L6
    tmpStr3000[1800] = 0; // L7

    temporaryInformation = TI_SHOW_REGISTER;

    tmpStr3000[   0] = 0; // JM Initialise
    tmpStr3000[2100] = 0; // JM temp
    tmpStr3000[2400] = 0; // JM temp


    if(SHOWregis >= 0 && SHOWregis < 100) {
      snprintf(tmpStr3000 + 2100, 10, "%d:", SHOWregis);
    } else
    switch (SHOWregis) {
      case REGISTER_X: strcpy(tmpStr3000 + 2100, "X: "); break;
      case REGISTER_Y: strcpy(tmpStr3000 + 2100, "Y: "); break;
      case REGISTER_Z: strcpy(tmpStr3000 + 2100, "Z: "); break;
      case REGISTER_T: strcpy(tmpStr3000 + 2100, "T: "); break;
      case REGISTER_A: strcpy(tmpStr3000 + 2100, "A: "); break;
      case REGISTER_B: strcpy(tmpStr3000 + 2100, "B: "); break;
      case REGISTER_C: strcpy(tmpStr3000 + 2100, "C: "); break;
      case REGISTER_D: strcpy(tmpStr3000 + 2100, "D: "); break;
      case REGISTER_L: strcpy(tmpStr3000 + 2100, "L: "); break;
      case REGISTER_I: strcpy(tmpStr3000 + 2100, "I: "); break;
      case REGISTER_J: strcpy(tmpStr3000 + 2100, "J: "); break;
      case REGISTER_K: strcpy(tmpStr3000 + 2100, "K: "); break;
      default: break;
    }
  }

  clearScreen(false, true, false); //Comment out to retain screen content while SHOW
  SHOW_reset();
  
  switch(getRegisterDataType(SHOWregis)) {
    case dtLongInteger:
      separator = STD_SPACE_4_PER_EM;
      longIntegerRegisterToDisplayString(SHOWregis, tmpStr3000 + 2103, TMP_STR_LENGTH, 7*400 - 8, 350, STD_SPACE_4_PER_EM);

      last = 2100 + stringByteLength(tmpStr3000 + 2100);
      source = 2100;
      dest = 0;

 
      { //printf("2: %d\n",stringGlyphLength(tmpStr3000 + 2100));
        temporaryInformation = TI_SHOW_REGISTER_BIG;
        if(groupingGap == 0) {
          maxWidth = SCREEN_WIDTH - stringWidth("0", &numericFont, true, true);
        }
        else {
          char tmp[4];
          strcpy(tmp,"0");
          strcat(tmp,separator);
          maxWidth = SCREEN_WIDTH - stringWidth(tmp , &numericFont, true, true);          //Add the separator that gets added to the last character
        }

        for(d=0; d<=1800 ; d+=300) {
          dest = d;
          while(source < last && stringWidth(tmpStr3000 + d, &numericFont, true, true) <=  maxWidth) {
            tmpStr3000[dest] = tmpStr3000[source];
            if(tmpStr3000[dest] & 0x80) {
              tmpStr3000[++dest] = tmpStr3000[++source];
            }
            source++;
            tmpStr3000[++dest] = 0;
          }

          while (source < last && groupingGap!=0 && !(tmpStr3000[dest-2] & 0x80)) //Not in the last line. Eat away characters at the end to line up the last space.
            {
              dest--; 
              source--;
            }
          tmpStr3000[dest] = 0;

          if(!(source < last) && groupingGap!=0 && !(tmpStr3000[dest+0] == *(separator + 0) && (tmpStr3000[dest+1] == *(separator + 1)))) {               //Last line
            tmpStr3000[dest+0] = *(separator + 0); //0xa0;       //Add a space to the very end to space last line nicely.
            tmpStr3000[dest+1] = *(separator + 1); //0x05;
            tmpStr3000[dest+2] = 0;
            dest+=2;
          }
        }
      }      

      if(stringByteLength(tmpStr3000 + 1200) != 2) { 

        SHOW_reset();
        longIntegerRegisterToDisplayString(SHOWregis, tmpStr3000 + 2103, TMP_STR_LENGTH, 7*400 - 8, 350, STD_SPACE_4_PER_EM);

        last = 2100 + stringByteLength(tmpStr3000 + 2100);
        source = 2100;
        dest = 0;

        //printf("1: %d\n",stringGlyphLength(tmpStr3000 + 2100));

        if(groupingGap == 0) {
          maxWidth = SCREEN_WIDTH - stringWidth("0", &standardFont, true, true);
        }
        else {
          maxWidth = SCREEN_WIDTH - stringWidth("0", &standardFont, true, true)*groupingGap - stringWidth(separator, &standardFont, true, true);
        }

        for(d=0; d<=1800 ; d+=300) {
          dest = d;
          while(source < last && stringWidth(tmpStr3000 + d, &standardFont, true, true) <= maxWidth) {
            do {
              tmpStr3000[dest] = tmpStr3000[source];
              if(tmpStr3000[dest] & 0x80) {
                tmpStr3000[++dest] = tmpStr3000[++source];
              }
              source++;
              tmpStr3000[++dest] = 0;
            } while(source < last && groupingGap > 0 && (tmpStr3000[source] != *separator || tmpStr3000[source + 1] != *(separator + 1)));
          }
        }
        if(source < last) { // The long integer is too long
          xcopy(tmpStr3000 + dest - 2, STD_ELLIPSIS, 2);
          xcopy(tmpStr3000 + dest, STD_SPACE_6_PER_EM, 2);
          tmpStr3000[dest + 2] = 0;
        }
      } 
      break;


    case dtReal34:
      temporaryInformation = TI_SHOW_REGISTER_BIG;
      separator = STD_SPACE_4_PER_EM;
      real34ToDisplayString(REGISTER_REAL34_DATA(SHOWregis), getRegisterAngularMode(SHOWregis), tmpStr3000 + 2103, &numericFont, 2000, 34, false, separator);
      last = 2100 + stringByteLength(tmpStr3000 + 2100);
      source = 2100;
      for(d=0; d<=900 ; d+=300) {
        dest = d;
        while(source < last && stringWidth(tmpStr3000 + d, &numericFont, true, true) <= SCREEN_WIDTH - 8*2) {
          tmpStr3000[dest] = tmpStr3000[source];
          if(tmpStr3000[dest] & 0x80) {
            tmpStr3000[++dest] = tmpStr3000[++source];
          }
          source++;
          tmpStr3000[++dest] = 0;
        }
      }
      break;


    case dtComplex34:
      temporaryInformation = TI_SHOW_REGISTER_BIG;

      // Real part
      separator = STD_SPACE_4_PER_EM;
      real34ToDisplayString(REGISTER_REAL34_DATA(SHOWregis), AM_NONE, tmpStr3000, &numericFont, 2000, 34, false, separator);
      for(i=stringByteLength(tmpStr3000) - 1; i>0; i--) {
        if(tmpStr3000[i] == 0x08) {
          tmpStr3000[i] = 0x05;
        }
      }
  
      real34Copy(REGISTER_IMAG34_DATA(REGISTER_X), &real34);
      last = 300;
      while(tmpStr3000[last]) last++;
      xcopy(tmpStr3000 + last++, (real34IsNegative(&real34) ? "-" : "+"), 1);
      xcopy(tmpStr3000 + last++, COMPLEX_UNIT, 1);
      xcopy(tmpStr3000 + last, PRODUCT_SIGN, 3);

      // Imaginary part
      real34SetPositiveSign(&real34);
      real34ToDisplayString(&real34, AM_NONE, tmpStr3000 + 600, &numericFont, 2000, 34, false, separator);
      for(i=stringByteLength(tmpStr3000 + 600) - 1; i>0; i--) {
        if(tmpStr3000[600 + i] == 0x08) {
          tmpStr3000[600 + i] = 0x05;
        }
      }
  
//vv      strncat(tmpStr3000 + 300, tmpStr3000 +  600, 299); //add +i. and imag
//vv      tmpStr3000[600] = 0;

      last = 300;
      while(tmpStr3000[last]) last++;
      xcopy(tmpStr3000 + last, tmpStr3000 + 600,  strlen(tmpStr3000 + 600) + 1);
      tmpStr3000[600] = 0;
//^^

      if(stringWidth(tmpStr3000, &numericFont, true, true) + stringWidth(tmpStr3000 + 300, &numericFont, true, true) <= 2*SCREEN_WIDTH) {
        strncat(tmpStr3000, tmpStr3000 +  300, 299);
        tmpStr3000[300] = 0;
  
//vv      strncat(tmpStr3000 + 2103, tmpStr3000 + 0, 299-3);  //COPY REAL
//vv      tmpStr3000[0] = 0;

      last = 2103;
      while(tmpStr3000[last]) last++;
      xcopy(tmpStr3000 + last, tmpStr3000 + 0,  strlen(tmpStr3000 + 0) + 1);
      tmpStr3000[0] = 0;
//^^
  
//vv      strcpy(tmpStr3000 + 2400, tmpStr3000 + 300);        //COPY IMAG
//vv      tmpStr3000[300] = 0;
//new
      last = 2400;
//      while(tmpStr3000[last]) last++;
      xcopy(tmpStr3000 + last, tmpStr3000 + 300,  strlen(tmpStr3000 + 300) + 1);
      tmpStr3000[300] = 0;
//^^
  
      last = 2100 + stringByteLength(tmpStr3000 + 2100);
      source = 2100;
      for(d=0; d<=300 ; d+=300) {
        dest = d;
        while(source < last && stringWidth(tmpStr3000 + d, &numericFont, true, true) <= SCREEN_WIDTH - 8*2) {
          tmpStr3000[dest] = tmpStr3000[source];
          if(tmpStr3000[dest] & 0x80) {
            tmpStr3000[++dest] = tmpStr3000[++source];
          }
          source++;
          tmpStr3000[++dest] = 0;
        }
      }
      
      last = 2400 + stringByteLength(tmpStr3000 + 2400);
      source = 2400;
      for(d=600; d<=900 ; d+=300) {
        dest = d;
        while(source < last && stringWidth(tmpStr3000 + d, &numericFont, true, true) <= SCREEN_WIDTH - 8*2) {
          tmpStr3000[dest] = tmpStr3000[source];
          if(tmpStr3000[dest] & 0x80) {
            tmpStr3000[++dest] = tmpStr3000[++source];
          }
          source++;
          tmpStr3000[++dest] = 0;
        }
      }
  
      if (tmpStr3000[300]==0) {                          //shift up if line is empty
//vv new       strcpy(tmpStr3000 + 300, tmpStr3000 + 600);
xcopy(tmpStr3000 + 300, tmpStr3000 + 600,  min(300,strlen(tmpStr3000 + 600) + 1));
//vv new        strcpy(tmpStr3000 + 600, tmpStr3000 + 900);
xcopy(tmpStr3000 + 600, tmpStr3000 + 900,  min(300,strlen(tmpStr3000 + 900) + 1));
        tmpStr3000[900] = 0;
      }
  
      if (tmpStr3000[600]==0) {                          //shift up if line is empty
//vv new        strcpy(tmpStr3000 + 600, tmpStr3000 + 900);
xcopy(tmpStr3000 + 600, tmpStr3000 + 900,  min(300,strlen(tmpStr3000 + 900) + 1));
        tmpStr3000[900] = 0;
      }
      break;


    case dtShortInteger:
      temporaryInformation = TI_SHOW_REGISTER_BIG;

      shortIntegerToDisplayString(SHOWregis, tmpStr3000 + 2103, true);
    
      if(getRegisterTag(SHOWregis) == 2) {
        source = 2100;
        dest = 2400;
        while(tmpStr3000[source] !=0 ) {
          if((uint8_t)(tmpStr3000[source]) == 160 && (uint8_t)(tmpStr3000[source+1]) == 39) {
            source++;
            tmpStr3000[dest++]=49;
          } else
            if((uint8_t)(tmpStr3000[source]) == 162 && (uint8_t)(tmpStr3000[source+1]) == 14) {
              source++;
              tmpStr3000[dest++]=48;
            } else {
              tmpStr3000[dest++] = tmpStr3000[source];
            }
          source++;
        }
        tmpStr3000[dest]=0;
      } else {
        strcpy(tmpStr3000 + 2400,tmpStr3000 + 2100);
      }
 

      last = 2400 + stringByteLength(tmpStr3000 + 2400);
      source = 2400;
      for(d=0; d<=900 ; d+=300) {
        dest = d;
        if(dest != 0){strcat(tmpStr3000 + dest,"  ");dest+=2;}               //space below the T:
        while(source < last && stringWidth(tmpStr3000 + d, &numericFont, true, true) <= SCREEN_WIDTH - 8*2) {
          tmpStr3000[dest] = tmpStr3000[source];
          if(tmpStr3000[dest] & 0x80) {
            tmpStr3000[++dest] = tmpStr3000[++source];
          }
          source++;
          tmpStr3000[++dest] = 0;
        }

        if(source < last && groupingGap!=0) {                  //Not in the last line
          if(!(tmpStr3000[dest-2] & 0x80)) {dest--; source--;} //Eat away characters at the end to line up the last space
          if(!(tmpStr3000[dest-2] & 0x80)) {dest--; source--;}
          if(!(tmpStr3000[dest-2] & 0x80)) {dest--; source--;}
          if(!(tmpStr3000[dest-2] & 0x80)) {dest--; source--;}
          tmpStr3000[dest] = 0;
        }            
      }
      break;


    case dtString:
      SHOW_reset();
      offset = 0;
      thereIsANextLine = true;
      bytesProcessed = 0;
        strcpy(tmpStr3000 + 2103, "'");
        strncat(tmpStr3000 + 2100, REGISTER_STRING_DATA(SHOWregis), stringByteLength(REGISTER_STRING_DATA(SHOWregis)) + 4+1);
        strcat(tmpStr3000 + 2100, "'");

      while(thereIsANextLine) {
        xcopy(tmpStr3000 + offset, tmpStr3000 + (2100+bytesProcessed), stringByteLength(tmpStr3000 + (2100+bytesProcessed + 1)));
        thereIsANextLine = false;
        while(stringWidth(tmpStr3000 + offset, &standardFont, false, true) >= SCREEN_WIDTH) {
          tmpStr3000[offset + stringLastGlyph(tmpStr3000 + offset)] = 0;
          thereIsANextLine = true;
        }
        bytesProcessed += stringByteLength(tmpStr3000 + offset);
        offset += 300;
        tmpStr3000[offset] = 0;
      }
      break;


/*
    case dtString:

      temporaryInformation = TI_SHOW_REGISTER_BIG;
      strcpy(tmpStr3000 + 2103, "'");
      strncat(tmpStr3000 + 2100, REGISTER_STRING_DATA(SHOWregis), stringByteLength(REGISTER_STRING_DATA(SHOWregis)) + 1);
      strcat(tmpStr3000 + 2100, "'");
      last = 2100 + stringByteLength(tmpStr3000 + 2100);
      source = 2100;
      for(d=0; d<=1200 ; d+=300) {
        dest = d;
        while(source < last && stringWidth(tmpStr3000 + d, &numericFont, true, true) <= SCREEN_WIDTH - 8*2) {
          tmpStr3000[dest] = tmpStr3000[source];
          if(tmpStr3000[dest] & 0x80) {
            tmpStr3000[++dest] = tmpStr3000[++source];
          }
          source++;
          tmpStr3000[++dest] = 0;
        }
      }

      if(tmpStr3000[1200] != 0) {
        SHOW_reset();
        strcpy(tmpStr3000 + 2103, "'");
        strncat(tmpStr3000 + 2100, REGISTER_STRING_DATA(SHOWregis), stringByteLength(REGISTER_STRING_DATA(SHOWregis)) + 1);
        strcat(tmpStr3000 + 2100, "'");
        last = 2100 + stringByteLength(tmpStr3000 + 2100);
        source = 2100;
        dest = 0;
        for(d=0; d<=1800 ; d+=300) {
          dest = d;
          while(source < last && stringWidth(tmpStr3000 + d, &standardFont, true, true) <= SCREEN_WIDTH - 8*2) {
            tmpStr3000[dest] = tmpStr3000[source];
            if(tmpStr3000[dest] & 0x80) {
              tmpStr3000[++dest] = tmpStr3000[++source];
            }
            source++;
            tmpStr3000[++dest] = 0;
          }
        }
        if(source < last) { // The string is too long
        xcopy(tmpStr3000 + dest - 2, STD_ELLIPSIS, 2);
        xcopy(tmpStr3000 + dest, STD_SPACE_6_PER_EM, 2);
        tmpStr3000[dest + 2] = 0;
        }
      }
      break;
*/
/*
    case dtString:
      offset = 0;
      thereIsANextLine = true;
      bytesProcessed = 0;
      while(thereIsANextLine) {
        xcopy(tmpStr3000 + offset, REGISTER_STRING_DATA(REGISTER_X) + bytesProcessed, stringByteLength(REGISTER_STRING_DATA(REGISTER_X) + bytesProcessed) + 1);
        thereIsANextLine = false;
        while(stringWidth(tmpStr3000 + offset, &standardFont, false, true) >= SCREEN_WIDTH) {
          tmpStr3000[offset + stringLastGlyph(tmpStr3000 + offset)] = 0;
          thereIsANextLine = true;
        }
        bytesProcessed += stringByteLength(tmpStr3000 + offset);
        offset += 300;
        tmpStr3000[offset] = 0;
      }
      break;
*/


    default:
      temporaryInformation = TI_NO_INFO;
      displayCalcErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, SHOWregis);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "cannot SHOW %s%s", tmpStr3000 + 2100, getRegisterDataTypeName(SHOWregis, true, false));
        showInfoDialog("In function fnShow:", errorMessage, NULL, NULL);
      #endif
      return;
    }
  }


    if (temporaryInformation == TI_SHOW_REGISTER) {
      refreshRegisterLine(REGISTER_T);
      if(tmpStr3000[ 300]) refreshRegisterLine(REGISTER_Z);
      if(tmpStr3000[ 900]) refreshRegisterLine(REGISTER_Y);
      if(tmpStr3000[1500]) refreshRegisterLine(REGISTER_X);
    } else
    if (temporaryInformation == TI_SHOW_REGISTER_BIG) {
      refreshRegisterLine(REGISTER_T);
      if(tmpStr3000[ 300]) refreshRegisterLine(REGISTER_Z);
      if(tmpStr3000[ 600]) refreshRegisterLine(REGISTER_Y);
      if(tmpStr3000[ 900]) refreshRegisterLine(REGISTER_X);  
    }
 
  displayFormat = savedDisplayFormat;
  displayFormatDigits = savedDisplayFormatDigits;
  SigFigMode = savedSigFigMode;                            //JM SIGFIG
  UNITDisplay = savedUNITDisplay;                          //JM SIGFIG


}

