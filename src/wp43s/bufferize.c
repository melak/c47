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
 * \file bufferize.c
 ***********************************************/

#include "wp43s.h"



#ifndef TESTSUITE_BUILD
void fnAim(uint16_t unusedParamButMandatory) {
  resetShiftState();
  calcModeAIM(NOPARAM); // Alpha Input Mode
}


/********************************************//**
 * \brief Adds an item to the alpha buffer
 *
 * \param[in] item uint16_t
 * \return void
 ***********************************************/
void addItemToBuffer(uint16_t item) {
  if(item == NOPARAM) {
    displayBugScreen("In function addItemToBuffer: item should not be NOPARAM=7654!");
  }
  else {
    if(calcMode == CM_AIM) {
      switch(nextChar) {
        case NC_SUBSCRIPT :
          switch(item) {
            case CHR_alpha    : item = CHR_SUB_alpha;    break;
            case CHR_delta    : item = CHR_SUB_delta;    break;
            case CHR_mu       : item = CHR_SUB_mu;       break;
            case CHR_SUN      : item = CHR_SUB_SUN;      break;
            case CHR_h        : item = CHR_SUB_h;        break;
            case CHR_t        : item = CHR_SUB_t;        break;
            case CHR_INFINITY : item = CHR_SUB_INFINITY; break;
            case CHR_s        : item = CHR_SUB_s;        break;
            case CHR_PLUS     : item = CHR_SUB_PLUS;     break;
            case CHR_MINUS    : item = CHR_SUB_MINUS;    break;
            case CHR_0        : item = CHR_SUB_0;        break;
            case CHR_1        : item = CHR_SUB_1;        break;
            case CHR_2        : item = CHR_SUB_2;        break;
            case CHR_3        : item = CHR_SUB_3;        break;
            case CHR_4        : item = CHR_SUB_4;        break;
            case CHR_5        : item = CHR_SUB_5;        break;
            case CHR_6        : item = CHR_SUB_6;        break;
            case CHR_7        : item = CHR_SUB_7;        break;
            case CHR_8        : item = CHR_SUB_8;        break;
            case CHR_9        : item = CHR_SUB_9;        break;
            case CHR_a        : item = CHR_SUB_a;        break;
            case CHR_b        : item = CHR_SUB_b;        break;
            case CHR_c        : item = CHR_SUB_c;        break;
            case CHR_d        : item = CHR_SUB_d;        break;
            case CHR_e        : item = CHR_SUB_e;        break;
            case CHR_i        : item = CHR_SUB_i;        break;
            case CHR_j        : item = CHR_SUB_j;        break;
            case CHR_k        : item = CHR_SUB_k;        break;
            case CHR_l        : item = CHR_SUB_l;        break;
            case CHR_m        : item = CHR_SUB_m;        break;
            case CHR_n        : item = CHR_SUB_n;        break;
            case CHR_o        : item = CHR_SUB_o;        break;
            case CHR_p        : item = CHR_SUB_p;        break;
            case CHR_q        : item = CHR_SUB_q;        break;
            case CHR_u        : item = CHR_SUB_u;        break;
            case CHR_v        : item = CHR_SUB_v;        break;
            case CHR_w        : item = CHR_SUB_w;        break;
            case CHR_x        : item = CHR_SUB_x;        break;
            case CHR_y        : item = CHR_SUB_y;        break;
            case CHR_z        : item = CHR_SUB_z;        break;
            case CHR_A        : item = CHR_SUB_A;        break;
            case CHR_B        : item = CHR_SUB_B;        break;
            case CHR_C        : item = CHR_SUB_C;        break;
            case CHR_D        : item = CHR_SUB_D;        break;
            case CHR_E        : item = CHR_SUB_E;        break;
            case CHR_F        : item = CHR_SUB_F;        break;
            case CHR_G        : item = CHR_SUB_G;        break;
            case CHR_H        : item = CHR_SUB_H;        break;
            case CHR_I        : item = CHR_SUB_I;        break;
            case CHR_J        : item = CHR_SUB_J;        break;
            case CHR_K        : item = CHR_SUB_K;        break;
            case CHR_L        : item = CHR_SUB_L;        break;
            case CHR_M        : item = CHR_SUB_M;        break;
            case CHR_N        : item = CHR_SUB_N;        break;
            case CHR_O        : item = CHR_SUB_O;        break;
            case CHR_P        : item = CHR_SUB_P;        break;
            case CHR_Q        : item = CHR_SUB_Q;        break;
            case CHR_R        : item = CHR_SUB_R;        break;
            case CHR_S        : item = CHR_SUB_S;        break;
            case CHR_T        : item = CHR_SUB_T;        break;
            case CHR_U        : item = CHR_SUB_U;        break;
            case CHR_V        : item = CHR_SUB_V;        break;
            case CHR_W        : item = CHR_SUB_W;        break;
            case CHR_X        : item = CHR_SUB_X;        break;
            case CHR_Y        : item = CHR_SUB_Y;        break;
            case CHR_Z        : item = CHR_SUB_Z;        break;
            default : {}
          }
          break;

        case NC_SUPERSCRIPT :
          switch(item) {
            case CHR_a        : item = CHR_SUP_a;        break;
            case CHR_x        : item = CHR_SUP_x;        break;
            case CHR_INFINITY : item = CHR_SUP_INFINITY; break;
            case CHR_PLUS     : item = CHR_SUP_PLUS;     break;
            case CHR_MINUS    : item = CHR_SUP_MINUS;    break;
            case CHR_0        : item = CHR_SUP_0;        break;
            case CHR_1        : item = CHR_SUP_1;        break;
            case CHR_2        : item = CHR_SUP_2;        break;
            case CHR_3        : item = CHR_SUP_3;        break;
            case CHR_4        : item = CHR_SUP_4;        break;
            case CHR_5        : item = CHR_SUP_5;        break;
            case CHR_6        : item = CHR_SUP_6;        break;
            case CHR_7        : item = CHR_SUP_7;        break;
            case CHR_8        : item = CHR_SUP_8;        break;
            case CHR_9        : item = CHR_SUP_9;        break;
            case CHR_f        : item = CHR_SUP_f;        break;
            case CHR_g        : item = CHR_SUP_g;        break;
            case CHR_h        : item = CHR_SUP_h;        break;
            case CHR_r        : item = CHR_SUP_r;        break;
            case CHR_T        : item = CHR_SUP_T;        break;
            default : {}
          }
        break;

        default : {}
      }

      if(stringByteLength(aimBuffer) + stringByteLength(indexOfItems[item].itemPrinted) >= AIM_BUFFER_LENGTH) {
        sprintf(errorMessage, "In function addItemToBuffer: the AIM input buffer is full! %d bytes for now", AIM_BUFFER_LENGTH);
        displayBugScreen(errorMessage);
      }
      else {
        strcat(aimBuffer, indexOfItems[item].itemPrinted);
        if(stringWidth(aimBuffer, &standardFont, true, true) >= SCREEN_WIDTH-9) {
          btnClicked(NULL, "16"); // back space
          #ifdef PC_BUILD
            showInfoDialog("In function addItemToBuffer:", "the aimBuffer string is too wide!", NULL, NULL);
          #endif
        }
        else {
          xCursor = showString(aimBuffer, &standardFont, 1, Y_POSITION_OF_AIM_LINE + 6, vmNormal, true, true);
        }
      }

      nextChar = NC_NORMAL;
    }

    else if(calcMode == CM_TAM) {
      if(item==ITM_Max || item==ITM_Min || item==ITM_ADD || item==ITM_SUB || item==ITM_MULT || item==ITM_DIV || item==ITM_Config || item==ITM_Stack || item==ITM_dddEL || item==ITM_dddIJ) { // Operation
        tamOperation = item;
        tamTransitionSystem(TT_OPERATION);
      }
      else if(REGISTER_X <= indexOfItems[item].param && indexOfItems[item].param <= REGISTER_K) { // Lettered register
        tamLetteredRegister = indexOfItems[item].param;
        tamTransitionSystem(TT_LETTER);
      }
      //else if(namedVariable) {
      //  tamTransitionSystem(TT_VARIABLE);
      //}
      else if(CHR_0 <= item && item <= CHR_9) { // Digits from 0 to 9
        tamDigit = item - CHR_0;
        tamTransitionSystem(TT_DIGIT);
      }
      else if(item == ITM_ENTER) {
        tamTransitionSystem(TT_ENTER);
      }
      else if(item == CHR_PERIOD) { // --> .
        tamTransitionSystem(TT_DOT);
      }
      else if(item == ITM_INDIRECTION) { // --> Indirection
        tamTransitionSystem(TT_INDIRECT);
      }
      else if(item == KEY_BACKSPACE) {
        tamTransitionSystem(TT_BACKSPACE);
      }
      else if(item == CHR_alpha) {
        #ifdef PC_BUILD
          showInfoDialog("In function addItemToBuffer:", STD_alpha " to be coded!", NULL, NULL);
        #endif
        tamTransitionSystem(TT_NOTHING);
      }
      else {
        tamTransitionSystem(TT_NOTHING);
      }
    }

    else if(calcMode == CM_NIM) {
      addItemToNimBuffer(item);
    }

    else {
      funcOK = false;
      return;
    }

    funcOK = true;
  }
}



void addItemToNimBuffer(int16_t item) {
  bool_t isRealDp(const char *buffer) {
    int16_t firstDigit, lastDigit, numDigits, decimalSeparator, exponent, exponentLocation;

    //printf("   %s : ", buffer);
    // The first digit is the first character that is neither a . nor a 0
    firstDigit = 0;
    while(buffer[firstDigit] != 0 && (buffer[firstDigit] == '0' || buffer[firstDigit] == '.')) {
      firstDigit++;
    }
    if(buffer[firstDigit] == 0) {
      lastDigit = firstDigit;
    }
    else {
      // The last digit is the last character that is neither a . nor a 0
      lastDigit = firstDigit;
      while(buffer[lastDigit] != 0 && buffer[lastDigit] != 'e' && buffer[lastDigit] != '+' && buffer[lastDigit] != '-') {
        lastDigit++;
      }
      lastDigit--;

      while(buffer[lastDigit] == '0' || buffer[lastDigit] == '.') {
        lastDigit--;
      }
    }

    for(decimalSeparator=0; buffer[decimalSeparator]!=0 && buffer[decimalSeparator]!='.'; decimalSeparator++);

    numDigits = lastDigit - firstDigit + (firstDigit<=decimalSeparator && decimalSeparator<=lastDigit ? 0 : 1);
    //printf("firstDigit=%d  lastDigit=%d  numDigits=%d  decimalSeparator=%d\n", firstDigit, lastDigit, numDigits, decimalSeparator);

    if(numDigits > 16) {
      return true;
    }
    else {
      exponentLocation = 0;
      while(buffer[exponentLocation] != 0 && buffer[exponentLocation] != 'e') {
        exponentLocation++;
      }
      if(buffer[exponentLocation] == 'e') {
        exponent = atoi(buffer + exponentLocation + 1);
        //printf("      exponentLocation=%d  exponent=%d\n", exponentLocation, exponent);
        exponent += decimalSeparator - firstDigit - (decimalSeparator > firstDigit ? 1 : 0);
        //printf("      corrected exponent=%d\n", exponent);
        return (exponent < -383 || exponent > 384);
      }
    }

    return false;
  }

  int16_t lastChar, index;
  uint8_t savedNimNumberPart;
  bool_t done;

  if(calcMode == CM_NORMAL) {
    switch(item) {
      case ITM_EXPONENT :
        calcModeNIM(NOPARAM);
        nimBuffer[0] = '+';
        nimBuffer[1] = '1';
        nimBuffer[2] = 0;
        nimNumberPart = NP_REAL_FLOAT_PART;
        break;

      case CHR_PERIOD :
        calcModeNIM(NOPARAM);
        nimBuffer[0] = '+';
        nimBuffer[1] = '0';
        nimBuffer[2] = 0;
        nimNumberPart = NP_INT_10;
        break;

      case CHR_0 :
      case CHR_1 :
      case CHR_2 :
      case CHR_3 :
      case CHR_4 :
      case CHR_5 :
      case CHR_6 :
      case CHR_7 :
      case CHR_8 :
      case CHR_9 :
      case CHR_A :
      case CHR_B :
      case CHR_C :
      case CHR_D :
      case CHR_E :
      case CHR_F :
        calcModeNIM(NOPARAM);
        nimBuffer[0] = '+';
        nimBuffer[1] = 0;
        nimNumberPart = NP_EMPTY;
        break;

      default :
        sprintf(errorMessage, "In function addItemToNimBuffer: %d is an unexpected item value when initializing NIM!", item);
        displayBugScreen(errorMessage);
        return;
    }

    //debugNIM();
  }

  done = false;

  switch(item) {
    case CHR_0 :
    case CHR_1 :
    case CHR_2 :
    case CHR_3 :
    case CHR_4 :
    case CHR_5 :
    case CHR_6 :
    case CHR_7 :
    case CHR_8 :
    case CHR_9 :
      done = true;

      switch(nimNumberPart) {
        case NP_INT_10 :
          if(item == CHR_0) {
            if(nimBuffer[1] != '0') {
              strcat(nimBuffer, "0");
            }
          }
          else {
            if(nimBuffer[1] == '0') {
              nimBuffer[1] = 0;
            }

            strcat(nimBuffer, indexOfItems[item].itemPrinted);
          }
          break;

        case NP_REAL_EXPONENT :
          if(item == CHR_0) {
            if(nimBuffer[exponentSignLocation + 1] == '0') {
              nimBuffer[strlen(nimBuffer) - 1] = 0;
            }

            if(nimBuffer[exponentSignLocation + 1] != 0 || nimBuffer[exponentSignLocation] == '+') {
              strcat(nimBuffer, "0");
            }

            if(atoi(nimBuffer + exponentSignLocation) > 9999 || atoi(nimBuffer + exponentSignLocation) < -9999) {
              nimBuffer[strlen(nimBuffer) - 1] = 0;
            }
          }
          else {
            if(nimBuffer[exponentSignLocation + 1] == '0') {
              nimBuffer[strlen(nimBuffer) - 1] = 0;
            }

            strcat(nimBuffer, indexOfItems[item].itemPrinted);

            if(atoi(nimBuffer + exponentSignLocation) > 9999 || atoi(nimBuffer + exponentSignLocation) < -9999) {
              nimBuffer[strlen(nimBuffer) - 1] = 0;
            }
          }
          break;

        case NP_FRACTION_DENOMINATOR :
          if(item == CHR_0) {
            strcat(nimBuffer, "0");

            if(nimBuffer[denominatorLocation] == '0') {
              nimBuffer[denominatorLocation] = 0;
            }

            if(atoi(nimBuffer + denominatorLocation) > 9999) {
              nimBuffer[strlen(nimBuffer) - 1] = 0;
            }
          }
          else {
            strcat(nimBuffer, indexOfItems[item].itemPrinted);

            if(atoi(nimBuffer + denominatorLocation) > 9999) {
              nimBuffer[strlen(nimBuffer) - 1] = 0;
            }
          }
          break;

        case NP_COMPLEX_INT_PART :
          if(item == CHR_0) {
            if(nimBuffer[imaginaryMantissaSignLocation + 2] != '0') {
              strcat(nimBuffer, "0");
            }
          }
          else {
            if(nimBuffer[imaginaryMantissaSignLocation + 2] == '0') {
              nimBuffer[imaginaryMantissaSignLocation + 2] = 0;
            }

            strcat(nimBuffer, indexOfItems[item].itemPrinted);
          }
          break;

        case NP_COMPLEX_EXPONENT :
          if(item == CHR_0) {
            if(nimBuffer[imaginaryExponentSignLocation + 1] == '0') {
              nimBuffer[strlen(nimBuffer) - 1] = 0;
            }

            if(nimBuffer[imaginaryExponentSignLocation + 1] != 0 || nimBuffer[imaginaryExponentSignLocation] == '+') {
              strcat(nimBuffer, "0");
            }

            if(atoi(nimBuffer + imaginaryExponentSignLocation) > 9999 || atoi(nimBuffer + imaginaryExponentSignLocation) < -9999) {
              nimBuffer[strlen(nimBuffer) - 1] = 0;
            }
          }
          else {
            if(nimBuffer[imaginaryExponentSignLocation + 1] == '0') {
              nimBuffer[strlen(nimBuffer) - 1] = 0;
            }

            strcat(nimBuffer, indexOfItems[item].itemPrinted);

            if(atoi(nimBuffer + imaginaryExponentSignLocation) > 9999 || atoi(nimBuffer + imaginaryExponentSignLocation) < -9999) {
              nimBuffer[strlen(nimBuffer) - 1] = 0;
            }
          }
          break;

        default :
          if(nimNumberPart == NP_EMPTY) {
            nimNumberPart = NP_INT_10;
            //debugNIM();
          }

          strcat(nimBuffer, indexOfItems[item].itemPrinted);
      }
      break;

    case CHR_A :
    case CHR_B :
    case CHR_C :
    case CHR_D :
    case CHR_E :
    case CHR_F :
      done = true;

      if(nimNumberPart == NP_EMPTY || nimNumberPart == NP_INT_10 || nimNumberPart == NP_INT_16) {
        if(nimBuffer[1] == '0') {
          nimBuffer[1] = 0;
        }

        strcat(nimBuffer, indexOfItems[item].itemPrinted);
        hexDigits++;

        nimNumberPart = NP_INT_16;
        //debugNIM();
      }
      break;

    case CHR_PERIOD :
      done = true;

      if(nimBuffer[strlen(nimBuffer)-1] == 'i') {
        strcat(nimBuffer, "0");
      }

      switch(nimNumberPart) {
        case NP_INT_10 :
          strcat(nimBuffer, ".");

          nimNumberPart = NP_REAL_FLOAT_PART;
          //debugNIM();
          break;

        case NP_REAL_FLOAT_PART :
          if(nimBuffer[strlen(nimBuffer) - 1] == '.') {
            strcat(nimBuffer, "0");
          }

          for(uint16_t i=0; i<strlen(nimBuffer); i++) {
            if(nimBuffer[i] == '.') {
              nimBuffer[i] = ' ';
              break;
            }
          }
          strcat(nimBuffer, "/");

          denominatorLocation = strlen(nimBuffer);
          nimNumberPart = NP_FRACTION_DENOMINATOR;
          //debugNIM();
          break;

        case NP_COMPLEX_INT_PART :
          strcat(nimBuffer, ".");

          nimNumberPart = NP_COMPLEX_FLOAT_PART;
          //debugNIM();
          break;

        default : {}
      }
      break;

    case ITM_EXPONENT :
      done = true;

      if(nimBuffer[strlen(nimBuffer)-1] == 'i') {
        strcat(nimBuffer, "1");
      }

      switch(nimNumberPart) {
        case NP_INT_10 :
          strcat(nimBuffer, "."); // no break here
          __attribute__ ((fallthrough));

        case NP_REAL_FLOAT_PART :
          strcat(nimBuffer, "e+");
          exponentSignLocation = strlen(nimBuffer) - 1;

          nimNumberPart = NP_REAL_EXPONENT;
          //debugNIM();
          break;

        case NP_COMPLEX_INT_PART :
          strcat(nimBuffer, "."); // no break here
          __attribute__ ((fallthrough));

        case NP_COMPLEX_FLOAT_PART :
          strcat(nimBuffer, "e+");
          imaginaryExponentSignLocation = strlen(nimBuffer) - 1;

          nimNumberPart = NP_COMPLEX_EXPONENT;
          //debugNIM();
          break;

        default : {}
      }
      break;

    case ITM_toINT : // #
      done = true;

      if(nimNumberPart == NP_INT_10 || nimNumberPart == NP_INT_16) {
        strcat(nimBuffer, "#");

        nimNumberPart = NP_INT_BASE;
        //debugNIM();
      }
      break;

    case ITM_CHS : // +/-
      done = true;

      switch(nimNumberPart) {
        case NP_INT_10 :
        case NP_INT_16 :
        case NP_INT_BASE :
        case NP_REAL_FLOAT_PART :
        case NP_FRACTION_DENOMINATOR :
          if(nimBuffer[0] == '+') {
            nimBuffer[0] = '-';
          }
          else {
            nimBuffer[0] = '+';
          }
          break;

        case NP_REAL_EXPONENT :
          if(nimBuffer[exponentSignLocation] == '+') {
            nimBuffer[exponentSignLocation] = '-';
            if(nimBuffer[exponentSignLocation + 1] == '0') {
              nimBuffer[strlen(nimBuffer) - 1] = 0;
            }
          }
          else {
            nimBuffer[exponentSignLocation] = '+';
          }
          break;

        case NP_COMPLEX_INT_PART :
        case NP_COMPLEX_FLOAT_PART :
          if(nimBuffer[imaginaryMantissaSignLocation] == '+') {
            nimBuffer[imaginaryMantissaSignLocation] = '-';
          }
          else {
            nimBuffer[imaginaryMantissaSignLocation] = '+';
          }
          break;

        case NP_COMPLEX_EXPONENT :
          if(nimBuffer[imaginaryExponentSignLocation] == '+') {
            nimBuffer[imaginaryExponentSignLocation] = '-';
            if(nimBuffer[imaginaryExponentSignLocation + 1] == '0') {
              nimBuffer[strlen(nimBuffer) - 1] = 0;
            }
          }
          else {
            nimBuffer[imaginaryExponentSignLocation] = '+';
          }
          break;

        default : {}
      }
      break;

    case KEY_CC :
      lastChar = strlen(nimBuffer) - 1;

      done = true;

      switch(nimNumberPart) {
       case NP_REAL_EXPONENT :
          if((nimBuffer[lastChar] == '+' || nimBuffer[lastChar] == '-') && nimBuffer[lastChar - 1] == 'e') {
            nimBuffer[lastChar - 1] = 0;
          }
          else if(nimBuffer[lastChar] == 'e') {
            nimBuffer[lastChar] = 0;
          }
          else {
            imaginaryMantissaSignLocation = strlen(nimBuffer);
            strcat(nimBuffer, "+i");

            nimNumberPart = NP_COMPLEX_INT_PART;
            //debugNIM();
          }
          break;

        case NP_INT_10 :
          strcat(nimBuffer, "."); // no break here
          __attribute__ ((fallthrough));

        case NP_REAL_FLOAT_PART :
          imaginaryMantissaSignLocation = strlen(nimBuffer);
          strcat(nimBuffer, "+i");

          nimNumberPart = NP_COMPLEX_INT_PART;
          //debugNIM();
          break;

        default : {}
      }
      break;

    case ITM_pi :
      done = true;

      if(nimNumberPart == NP_COMPLEX_INT_PART && nimBuffer[strlen(nimBuffer) - 1] == 'i') {
        strcat(nimBuffer, "3.141592653589793");
      }
      break;

    case KEY_BACKSPACE :
      lastChar = strlen(nimBuffer) - 1;

      done = true;

      switch(nimNumberPart) {
        case NP_INT_10 :
          break;

        case NP_INT_16 :
          if(nimBuffer[lastChar] >= 'A') {
            hexDigits--;
          }

          if(hexDigits == 0) {
            nimNumberPart = NP_INT_10;
            //debugNIM();
          }
          break;

        case NP_INT_BASE :
          if(nimBuffer[lastChar] == '#') {
            if(hexDigits > 0) {
              nimNumberPart = NP_INT_16;
            }
            else {
              nimNumberPart = NP_INT_10;
            }
            //debugNIM();
          }
          break;

        case NP_REAL_FLOAT_PART :
          if(nimBuffer[lastChar] == '.') {
            nimNumberPart = NP_INT_10;
            //debugNIM();
          }
          break;

        case NP_REAL_EXPONENT :
          if(nimBuffer[lastChar] == '+' || nimBuffer[lastChar] == '-') {
            nimBuffer[lastChar--] = 0;
          }

          if(nimBuffer[lastChar] == 'e') {
            nimNumberPart = NP_INT_10;
            for(int16_t i=0; i<lastChar; i++) {
              if(nimBuffer[i] == '.') {
                nimNumberPart = NP_REAL_FLOAT_PART;
                break;
              }
            }
            //debugNIM();
          }
          break;

        case NP_FRACTION_DENOMINATOR :
          if(nimBuffer[lastChar] == '/') {
            nimNumberPart = NP_REAL_FLOAT_PART;
            for(int16_t i=0; i<lastChar; i++) {
              if(nimBuffer[i] == ' ') {
                nimBuffer[i] = '.';
                break;
              }
            }
            //debugNIM();
          }
          break;

        case NP_COMPLEX_INT_PART :
          if(nimBuffer[lastChar] == 'i') {
            nimNumberPart = NP_INT_10;
            for(int16_t i=0; i<lastChar; i++) {
              if(nimBuffer[i] == 'e') {
                nimNumberPart = NP_REAL_EXPONENT;
                break;
              }
              if(nimBuffer[i] == '.') {
                nimNumberPart = NP_REAL_FLOAT_PART;
              }
            }
            //debugNIM();
            lastChar--;
          }
          break;

        case NP_COMPLEX_FLOAT_PART :
          if(nimBuffer[lastChar] == '.') {
            nimNumberPart = NP_COMPLEX_INT_PART;
            //debugNIM();
          }
          break;

        case NP_COMPLEX_EXPONENT :
          if(nimBuffer[lastChar] == '+' || nimBuffer[lastChar] == '-') {
            nimBuffer[lastChar--] = 0;
          }

          if(nimBuffer[lastChar] == 'e') {
            nimNumberPart = NP_COMPLEX_INT_PART;
            for(int16_t i=imaginaryMantissaSignLocation+2; i<lastChar; i++) {
              if(nimBuffer[i] == '.') {
                nimNumberPart = NP_COMPLEX_FLOAT_PART;
                break;
              }
            }
            //debugNIM();
          }
          break;

        default : {}
      }

      nimBuffer[lastChar--] = 0;

      if(lastChar == -1 || (lastChar == 0 && nimBuffer[0] == '+')) {
        calcModeNormal();
        restoreStack();
      }
      break;

    case KEY_EXIT :
      done = true;
      closeNim();
      if(calcMode != CM_NIM && lastErrorCode == 0) {
        STACK_LIFT_ENABLE;
        return;
      }
      break;

    case ITM_ENTER :
      done = true;
      closeNim();
      if(calcMode != CM_NIM && lastErrorCode == 0) {
        if(nimNumberPart == NP_COMPLEX_INT_PART || nimNumberPart == NP_COMPLEX_FLOAT_PART || nimNumberPart == NP_COMPLEX_EXPONENT) {
          fnSetFlag(FLAG_CPXRES);
        }

        STACK_LIFT_ENABLE;
        liftStack();
        copySourceRegisterToDestRegister(REGISTER_Y, REGISTER_X);
        refreshStack();
        STACK_LIFT_DISABLE;
        return;
      }
      break;

    case ITM_LN :
      if(nimNumberPart == NP_INT_BASE && nimBuffer[strlen(nimBuffer) - 1] == '#') { // D for decimal base
        done = true;

        strcat(nimBuffer, "10");
      }
      break;

    case ITM_RCL :
      if(nimNumberPart == NP_INT_BASE && nimBuffer[strlen(nimBuffer) - 1] == '#') { // H for hexadecimal base
        done = true;

        strcat(nimBuffer, "16");
      }
      break;

    case ITM_DMS :
      if(nimNumberPart == NP_INT_10 || nimNumberPart == NP_REAL_FLOAT_PART) {
        done = true;

        closeNim();
        if(calcMode != CM_NIM && lastErrorCode == 0) {
          if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
            convertLongIntegerRegisterToReal16Register(REGISTER_X, REGISTER_X);
          }

          checkDms16(REGISTER_REAL16_DATA(REGISTER_X));
          setRegisterDataType(REGISTER_X, dtAngle16, AM_DMS);

          refreshRegisterLine(REGISTER_X);
          STACK_LIFT_ENABLE;
          return;
        }
      }
      break;

    default : {}
  }

  if(done) {
    //printf("nimBuffer = '%s'\n", nimBuffer);
    //Convert nimBuffer to display string

    nimInputIsReal34 = false;
    strcpy(nimBufferDisplay, NUM_SPACE_HAIR);

    switch(nimNumberPart) {
      case NP_INT_10 :          // +12345
      case NP_INT_16 :          // +123AB
      case NP_INT_BASE :        // +123AB#16
        nimBufferToDisplayBuffer(nimBuffer, nimBufferDisplay + 2);
        break;

      case NP_REAL_FLOAT_PART : // +12345.6789
        nimBufferToDisplayBuffer(nimBuffer, nimBufferDisplay + 2);

        //printf("NP_REAL_FLOAT_PART before nimInputIsReal34=%s  ", getBooleanName(nimInputIsReal34));
        nimInputIsReal34 = nimInputIsReal34 || isRealDp(nimBuffer + 1);
        //printf("after nimInputIsReal34=%s\n", getBooleanName(nimInputIsReal34));
        break;

      case NP_REAL_EXPONENT : // +12345.678e+3
        nimBufferToDisplayBuffer(nimBuffer, nimBufferDisplay + 2);

        exponentToDisplayString(atoi(nimBuffer + exponentSignLocation), nimBufferDisplay + stringByteLength(nimBufferDisplay), true);
        if(nimBuffer[exponentSignLocation + 1] == 0 && nimBuffer[exponentSignLocation] == '-') {
          strcat(nimBufferDisplay, NUM_SUP_MINUS);
        }
        else if(nimBuffer[exponentSignLocation + 1] == '0' && nimBuffer[exponentSignLocation] == '+') {
          strcat(nimBufferDisplay, NUM_SUP_0);
        }

        //printf("NP_REAL_EXPONENT before nimInputIsReal34=%s  ", getBooleanName(nimInputIsReal34));
        nimInputIsReal34 = nimInputIsReal34 || isRealDp(nimBuffer + 1);
        //printf("after nimInputIsReal34=%s\n", getBooleanName(nimInputIsReal34));
        break;

      case NP_FRACTION_DENOMINATOR : // +123 12/7
        nimBufferToDisplayBuffer(nimBuffer, nimBufferDisplay + 2);
        strcat(nimBufferDisplay, NUM_SPACE_4_PER_EM);

        for(index=2; nimBuffer[index]!=' '; index++); // The ending semi colon is OK here
        supNumberToDisplayString(atoi(nimBuffer + index + 1), nimBufferDisplay + stringByteLength(nimBufferDisplay), true);

        strcat(nimBufferDisplay, "/");

        for(; nimBuffer[index]!='/'; index++); // The ending semi colon is OK here
        if(nimBuffer[++index] != 0) {
          subNumberToDisplayString(atoi(nimBuffer + index), nimBufferDisplay + stringByteLength(nimBufferDisplay));
        }
        break;

      case NP_COMPLEX_INT_PART :   // +1.2+i15
      case NP_COMPLEX_FLOAT_PART : // +1.2+i15.69
      case NP_COMPLEX_EXPONENT :   // +1.2+i15.69e2
        // Real part
        //printf("NP_COMPLEX real part before nimInputIsReal34=%s  ", getBooleanName(nimInputIsReal34));
        nimInputIsReal34 = nimInputIsReal34 || isRealDp(nimBuffer + 1);
        //printf("after nimInputIsReal34=%s\n", getBooleanName(nimInputIsReal34));
        savedNimNumberPart = nimNumberPart;

        for(index=2; index<imaginaryMantissaSignLocation && nimBuffer[index] != '.'; index++); // The ending semi colon is OK here
        if(index < imaginaryMantissaSignLocation) { // There is a decimal part in the real part
          nimNumberPart = NP_REAL_FLOAT_PART;
        }

        for(index=2; index<imaginaryMantissaSignLocation && nimBuffer[index] != 'e'; index++); // The ending semi colon is OK here
        if(index < imaginaryMantissaSignLocation) { // There is an exposant in the real part
          nimNumberPart = NP_REAL_EXPONENT;
        }

        nimBufferToDisplayBuffer(nimBuffer, nimBufferDisplay + 2);

        if(nimNumberPart == NP_REAL_EXPONENT) {
          exponentToDisplayString(atoi(nimBuffer + exponentSignLocation), nimBufferDisplay + stringByteLength(nimBufferDisplay), true);
          if(nimBuffer[exponentSignLocation + 1] == 0 && nimBuffer[exponentSignLocation] == '-') {
            strcat(nimBufferDisplay, NUM_SUP_MINUS);
          }
          else if(nimBuffer[exponentSignLocation + 1] == '0' && nimBuffer[exponentSignLocation] == '+') {
            strcat(nimBufferDisplay, NUM_SUP_0);
          }
        }

        nimNumberPart = savedNimNumberPart;

        // Complex "separator"
        if(complexMode == CM_RECTANGULAR) {
          if(strncmp(nimBufferDisplay + stringByteLength(nimBufferDisplay) - 2, NUM_SPACE_HAIR, 2) != 0) {
            strcat(nimBufferDisplay, NUM_SPACE_HAIR);
          }

          if(nimBuffer[imaginaryMantissaSignLocation] == '-') {
            strcat(nimBufferDisplay, "-");
          }
          else {
            strcat(nimBufferDisplay, "+");
          }
          strcat(nimBufferDisplay, COMPLEX_UNIT);
          strcat(nimBufferDisplay, PRODUCT_SIGN);
        }
        else {
          strcat(nimBufferDisplay, NUM_SPACE_4_PER_EM NUM_MEASURED_ANGLE NUM_SPACE_4_PER_EM);
          if(nimBuffer[imaginaryMantissaSignLocation] == '-') {
            strcat(nimBufferDisplay, "-");
          }
        }

        // Imaginary part
        if(nimBuffer[imaginaryMantissaSignLocation+2] != 0) {
          //printf("NP_COMPLEX imaginary part before nimInputIsReal34=%s  ", getBooleanName(nimInputIsReal34));
          nimInputIsReal34 = nimInputIsReal34 || isRealDp(nimBuffer + imaginaryMantissaSignLocation + 2);
          //printf("after nimInputIsReal34=%s\n", getBooleanName(nimInputIsReal34));

          savedNimNumberPart = nimNumberPart;

          for(index=imaginaryMantissaSignLocation+1; index<(int16_t)strlen(nimBuffer) && nimBuffer[index] != '.'; index++); // The ending semi colon is OK here
          if(index < (int16_t)strlen(nimBuffer)) { // There is a decimal part in the real part
            nimNumberPart = NP_REAL_FLOAT_PART;
          }

          for(index=imaginaryMantissaSignLocation+1; index<(int16_t)strlen(nimBuffer) && nimBuffer[index] != 'e'; index++); // The ending semi colon is OK here
          if(index < (int16_t)strlen(nimBuffer)) { // There is an exposant in the real part
            nimNumberPart = NP_REAL_EXPONENT;
          }

          nimBufferToDisplayBuffer(nimBuffer + imaginaryMantissaSignLocation + 1, nimBufferDisplay + stringByteLength(nimBufferDisplay));

          if(nimNumberPart == NP_REAL_EXPONENT) {
            exponentToDisplayString(atoi(nimBuffer + imaginaryExponentSignLocation), nimBufferDisplay + stringByteLength(nimBufferDisplay), true);
            if(nimBuffer[imaginaryExponentSignLocation + 1] == 0 && nimBuffer[imaginaryExponentSignLocation] == '-') {
              strcat(nimBufferDisplay, NUM_SUP_MINUS);
            }
            else if(nimBuffer[imaginaryExponentSignLocation + 1] == '0' && nimBuffer[imaginaryExponentSignLocation] == '+') {
              strcat(nimBufferDisplay, NUM_SUP_0);
            }
          }

          nimNumberPart = savedNimNumberPart;
        }
        break;

      default :
        sprintf(errorMessage, "In function addItemToNimBuffer: %d is an unexpected nimNumberPart value while converting buffer to display!", nimNumberPart);
        displayBugScreen(errorMessage);
    }

    if(radixMark == RM_COMMA) {
      for(index=stringByteLength(nimBufferDisplay) - 1; index>0; index--) {
        if(nimBufferDisplay[index] == '.') {
          nimBufferDisplay[index] = ',';
        }
      }
    }

    if(nimInputIsReal34) { // replace . or , by the corresponding double precision . or ,
      for(index=stringByteLength(nimBufferDisplay) - 1; index>0; index--) {
        if(nimBufferDisplay[index] == '.') {
          for(int i=stringByteLength(nimBufferDisplay); i>=index; i--) {
            nimBufferDisplay[i+1] = nimBufferDisplay[i];
          }
          strncpy(nimBufferDisplay + index, NUM_PERIOD34, 2);
        }
        else if(nimBufferDisplay[index] == ',') {
          for(int i=stringByteLength(nimBufferDisplay); i>=index; i--) {
            nimBufferDisplay[i+1] = nimBufferDisplay[i];
          }
          strncpy(nimBufferDisplay + index, NUM_COMMA34, 2);
        }
      }
    }

    refreshRegisterLine(NIM_REGISTER_LINE);
  }

  else {
    closeNim();
    if(calcMode != CM_NIM && lastErrorCode == 0) {
      showFunctionName(item, 10);
    }
  }
}



void nimBufferToDisplayBuffer(const char *nimBuffer, char *displayBuffer) {
  int16_t insertGapIP(char *displayBuffer, int16_t numDigits, int16_t nth) {
    if(groupingGap == 0)         return 0; // no gap when none is required!
    if(numDigits <= groupingGap) return 0; // there are less than groupingGap digits
    if(nth + 1 == numDigits)     return 0; // no gap after the last digit

    if((numDigits - nth) % groupingGap == 1 || groupingGap == 1) {
      strcpy(displayBuffer, NUM_SPACE_PUNCTUATION);
      return 2;
    }

    return 0;
  }

  int16_t insertGapFP(char *displayBuffer, int16_t numDigits, int16_t nth) {
    if(groupingGap == 0)         return 0; // no gap when none is required!
    if(numDigits <= groupingGap) return 0; // there are less than groupingGap digits
    if(nth + 1 == numDigits)     return 0; // no gap after the last digit

    if(nth % groupingGap == groupingGap - 1) {
      strcpy(displayBuffer, NUM_SPACE_PUNCTUATION);
      return 2;
    }

    return 0;
  }

  int16_t numDigits, source, dest;

  if(*nimBuffer == '-') {
    *(displayBuffer++) = '-';
  }
  nimBuffer++;

  for(numDigits=0; nimBuffer[numDigits]!=0 && nimBuffer[numDigits]!='e' && nimBuffer[numDigits]!='.' && nimBuffer[numDigits]!=' ' && nimBuffer[numDigits]!='#' && nimBuffer[numDigits]!='+' && nimBuffer[numDigits]!='-'; numDigits++); // The semicolon here is OK

  for(source=0, dest=0; source<numDigits; source++) {
    displayBuffer[dest++] = nimBuffer[source];
    dest += insertGapIP(displayBuffer + dest, numDigits, source);
  }

  displayBuffer[dest] = 0;

  if(nimNumberPart == NP_REAL_FLOAT_PART || nimNumberPart == NP_REAL_EXPONENT) {
    displayBuffer[dest++] = '.';

    nimBuffer += source + 1;

    for(numDigits=0; nimBuffer[numDigits]!=0 && nimBuffer[numDigits]!='e' && nimBuffer[numDigits]!='+' && nimBuffer[numDigits]!='-'; numDigits++); // The semicolon here is OK

    for(source=0; source<numDigits; source++) {
      displayBuffer[dest++] = nimBuffer[source];
      dest += insertGapFP(displayBuffer + dest, numDigits, source);
    }

    displayBuffer[dest] = 0;
  }

  else if(nimNumberPart == NP_INT_BASE) {
    strcpy(displayBuffer + dest, nimBuffer + numDigits);
  }
}



int16_t getStoRclOperation(void) {
  switch(tamFunction) {
    case ITM_STO :
      switch(tamCurrentOperation) {
        case ITM_ADD    : return ITM_STOPLUS;
        case ITM_SUB    : return ITM_STOMINUS;
        case ITM_MULT   : return ITM_STOMULT;
        case ITM_DIV    : return ITM_STODIV;
        case ITM_Max    : return ITM_STOMAX;
        case ITM_Min    : return ITM_STOMIN;
        case ITM_Config : return ITM_STOCFG;
        case ITM_Stack  : return ITM_STOS;
        case ITM_dddEL  : return ITM_STOEL;
        case ITM_dddIJ  : return ITM_STOIJ;
        default :
          sprintf(errorMessage, "In function getStoRclOperation: status=0 tamTransition=TT_OPERATION tamCurrentOperation=ITM_STO. %d is an unexpected value for tamCurrentOperation!", tamCurrentOperation);
          displayBugScreen(errorMessage);
          return ITM_NOP;
      }
    break;

    case ITM_RCL :
      switch(tamCurrentOperation) {
        case ITM_ADD    : return ITM_RCLPLUS;
        case ITM_SUB    : return ITM_RCLMINUS;
        case ITM_MULT   : return ITM_RCLMULT;
        case ITM_DIV    : return ITM_RCLDIV;
        case ITM_Max    : return ITM_RCLMAX;
        case ITM_Min    : return ITM_RCLMIN;
        case ITM_Config : return ITM_RCLCFG;
        case ITM_Stack  : return ITM_RCLS;
        case ITM_dddEL  : return ITM_RCLEL;
        case ITM_dddIJ  : return ITM_RCLIJ;
        default :
          sprintf(errorMessage, "In function getStoRclOperation: status=0 tamTransition=TT_OPERATION tamCurrentOperation=ITM_RCL. %d is an unexpected value for tamCurrentOperation!", tamCurrentOperation);
          displayBugScreen(errorMessage);
          return ITM_NOP;
      }
      break;

    default :
      sprintf(errorMessage, "In function getStoRclOperation: status=0 tamTransition=TT_OPERATION. %d is an unexpected value for tamFunction!", tamFunction);
      displayBugScreen(errorMessage);
      return ITM_NOP;
  }
}



void tamTransitionSystem(uint16_t tamTransition) {
  calcRegister_t value, regist;

  switch(transitionSystemState) {
    //////////////////////////////
    // OP __
    case 0 :
      switch(tamTransition) {
        case TT_OPERATION :
          if(tamMode == TM_STORCL) {
            strcpy(transitionSystemOperation, tamOperation==ITM_Max ? STD_UP_ARROW : (tamOperation==ITM_Min ? STD_DOWN_ARROW : (tamOperation==ITM_ADD ? "+" : (tamOperation==ITM_SUB ? "-" : (tamOperation==ITM_MULT ? STD_CROSS : (tamOperation==ITM_DIV ? "/" : (tamOperation==ITM_Config ? "CFG" : (tamOperation==ITM_Stack ? "S" : (tamOperation==ITM_dddEL ? "EL" : "IJ")))))))));
            sprintf(tamBuffer, "%s%s __", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
            tamCurrentOperation = tamOperation;
            transitionSystemState = 1;
          }
          break;

        case TT_LETTER :
          if(tamMode != TM_VALUE) {
            indexOfItems[tamFunction].func(tamLetteredRegister);
            calcModeNormal();
            return;
          }
          break;

        case TT_VARIABLE :
          if(tamMode != TM_VALUE) {
          }
          break;

        case TT_DIGIT :
          tamNumber = tamDigit;
          if(tamNumber < tamNumberMin) {
            sprintf(tamBuffer, "%s %d_", indexOfItems[tamFunction].itemPrinted, tamNumber);
            transitionSystemState = 2;
          }
          else if(tamNumber > tamNumberMax) {
          }
          else if(tamNumber*10 > tamNumberMax) {
            indexOfItems[tamFunction].func(tamNumber);
            calcModeNormal();
            return;
          }
          else {
            sprintf(tamBuffer, "%s %d_", indexOfItems[tamFunction].itemPrinted, tamNumber);
            transitionSystemState = 2;
          }
          break;

        case TT_DOT :
          if(tamMode != TM_VALUE) {
            if((tamMode == TM_FLAG && numberOfLocalFlags > 0) || (tamMode != TM_FLAG && numberOfLocalRegisters > 0)) {
              sprintf(tamBuffer, "%s .__", indexOfItems[tamFunction].itemPrinted);
              transitionSystemState = 3;
            }
          }
          break;

        case TT_INDIRECT :
          sprintf(tamBuffer, "%s " STD_RIGHT_ARROW "__", indexOfItems[tamFunction].itemPrinted);
          transitionSystemState = 5;
          break;

        case TT_BACKSPACE :
          calcModeNormal();
          return;
          break;

        default : {}
      }
      break;

    //////////////////////////////
    // OPo __
    case 1 : // RCL+, RCL-, RCL×, RCL/, RCL^, RCLv, STO+, STO-, STO×, STO/, STO^ or RCLv
      strcpy(transitionSystemOperation, tamOperation==ITM_Max ? STD_UP_ARROW : (tamOperation==ITM_Min ? STD_DOWN_ARROW : (tamOperation==ITM_ADD ? "+" : (tamOperation==ITM_SUB ? "-" : (tamOperation==ITM_MULT ? STD_CROSS : (tamOperation==ITM_DIV ? "/" : (tamOperation==ITM_Config ? "CFG" : (tamOperation==ITM_Stack ? "S" : (tamOperation==ITM_dddEL ? "EL" : "IJ")))))))));

      switch(tamTransition) {
        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s __   ", indexOfItems[tamFunction].itemPrinted);
          transitionSystemState = 0;
          break;

        case TT_OPERATION :
          if(tamOperation==tamCurrentOperation) {
            sprintf(tamBuffer, "%s __   ", indexOfItems[tamFunction].itemPrinted);
            transitionSystemState = 0;
          }
          else {
            tamCurrentOperation = tamOperation;
            strcpy(transitionSystemOperation, tamOperation==ITM_Max ? STD_UP_ARROW : (tamOperation==ITM_Min ? STD_DOWN_ARROW : (tamOperation==ITM_ADD ? "+" : (tamOperation==ITM_SUB ? "-" : (tamOperation==ITM_MULT ? STD_CROSS : (tamOperation==ITM_DIV ? "/" : (tamOperation==ITM_Config ? "CFG" : (tamOperation==ITM_Stack ? "S" : (tamOperation==ITM_dddEL ? "EL" : "IJ")))))))));
            sprintf(tamBuffer, "%s%s __", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
          }
          break;

        case TT_LETTER :
          indexOfItems[getStoRclOperation()].func(tamLetteredRegister);
          calcModeNormal();
          return;

        case TT_VARIABLE :
          break;

        case TT_DIGIT :
          tamNumber = tamDigit;
          sprintf(tamBuffer, "%s%s %d_", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation, tamNumber);
          transitionSystemState = 9;
          break;

        case TT_DOT :
          if(numberOfLocalRegisters > 0) {
            sprintf(tamBuffer, "%s%s .__", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
            transitionSystemState = 10;
          }
          break;

        case TT_INDIRECT :
          sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW "__", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
          transitionSystemState = 12;
          break;

        default : {}
      }
      break;

    //////////////////////////////
    // OP d_
    case 2 :
      switch(tamTransition) {
        case TT_DIGIT :
          if(tamNumberMin <= (tamNumber*10 + tamDigit) && (tamNumber*10 + tamDigit) <= tamNumberMax) {
            indexOfItems[tamFunction].func(tamNumber*10 + tamDigit);
            calcModeNormal();
            return;
          }
          break;

        case TT_ENTER :
          if(tamNumberMin <= tamNumber && tamNumber <= tamNumberMax) {
            indexOfItems[tamFunction].func(tamNumber);
            calcModeNormal();
            return;
          }
          break;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s __", indexOfItems[tamFunction].itemPrinted);
          transitionSystemState = 0;
          break;

        default : {}
      }
      break;

    //////////////////////////////
    // OP .__
    case 3 :
      // Here we are sure that:
      // numberOfLocalFlags     > 0 in the case of a flag parameter
      // numberOfLocalRegisters > 0 in the case of a register parameter
      switch(tamTransition) {
        case TT_DIGIT :
          tamNumber = tamDigit;
          if((tamMode == TM_FLAG && tamNumber < numberOfLocalFlags) || (tamMode != TM_FLAG && tamNumber < numberOfLocalRegisters)) {
            if((tamMode == TM_FLAG && tamNumber*10 >= numberOfLocalFlags) || (tamMode != TM_FLAG && tamNumber*10 >= numberOfLocalRegisters)) {
              indexOfItems[tamFunction].func(tamNumber + FIRST_LOCAL_REGISTER);
              calcModeNormal();
              return;
            }
            else {
              sprintf(tamBuffer, "%s .%d_", indexOfItems[tamFunction].itemPrinted, tamNumber);
              transitionSystemState = 4;
            }
          }
          break;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s __ ", indexOfItems[tamFunction].itemPrinted);
          transitionSystemState = 0;
          break;

        default : {}
      }
      break;

    //////////////////////////////
    // OP .d_
    case 4 :
      // Here we are sure that:
      // 0 <= tamNumber < numberOfLocalFlags      in the case of a flag parameter
      // 0 <= tamNumber < numberOfLocalRegisters  in the case of a register parameter
      switch(tamTransition) {
        case TT_DIGIT :
          if((tamMode == TM_FLAG && tamNumber*10 + tamDigit < numberOfLocalFlags) || (tamMode != TM_FLAG && tamNumber*10 + tamDigit < numberOfLocalRegisters)) {
            indexOfItems[tamFunction].func(tamNumber*10 + tamDigit + FIRST_LOCAL_REGISTER);
            calcModeNormal();
            return;
          }
          break;

        case TT_ENTER :
          indexOfItems[tamFunction].func(tamNumber + FIRST_LOCAL_REGISTER);
          calcModeNormal();
          return;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s .__", indexOfItems[tamFunction].itemPrinted);
          transitionSystemState = 3;
          break;

        default : {}
      }
      break;

    //////////////////////////////
    // OP -->__
    case 5 :
      switch(tamTransition) {
        case TT_LETTER :
          value = indirectAddressing(tamLetteredRegister, tamNumberMin, tamNumberMax);

          if(lastErrorCode == 0) {
            indexOfItems[tamFunction].func(value);
          }
          calcModeNormal();
          return;
          break;

        case TT_VARIABLE :
          break;

        case TT_DIGIT :
          tamNumber = tamDigit;
          sprintf(tamBuffer, "%s " STD_RIGHT_ARROW "%d_", indexOfItems[tamFunction].itemPrinted, tamNumber);
          transitionSystemState = 6;
          break;

        case TT_DOT :
          if(numberOfLocalRegisters > 0) {
            sprintf(tamBuffer, "%s " STD_RIGHT_ARROW ".__", indexOfItems[tamFunction].itemPrinted);
            transitionSystemState = 7;
          }
          break;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s __ ", indexOfItems[tamFunction].itemPrinted);
          transitionSystemState = 0;
          break;

        default : {}
      }
      break;

    //////////////////////////////
    // OP -->d_
    case 6 :
      switch(tamTransition) {
        case TT_DIGIT :
          value = indirectAddressing(tamNumber*10 + tamDigit, tamNumberMin, tamNumberMax);

          if(lastErrorCode == 0) {
            indexOfItems[tamFunction].func(value);
          }
          calcModeNormal();
          return;

        case TT_ENTER :
          value = indirectAddressing(tamNumber, tamNumberMin, tamNumberMax);

          if(lastErrorCode == 0) {
            indexOfItems[tamFunction].func(value);
          }
          calcModeNormal();
          return;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s " STD_RIGHT_ARROW "__", indexOfItems[tamFunction].itemPrinted);
          transitionSystemState = 5;
          break;

        default : {}
      }
      break;

    //////////////////////////////
    // OP -->.__
    case 7 :
      // Here we are sure that:
      // numberOfLocalRegisters > 0
      switch(tamTransition) {
        case TT_DIGIT :
          tamNumber = tamDigit;
          if((tamMode == TM_FLAG && tamNumber < numberOfLocalFlags) || (tamMode != TM_FLAG && tamNumber < numberOfLocalRegisters)) {
            if((tamMode == TM_FLAG && tamNumber*10 >= numberOfLocalFlags) || (tamMode != TM_FLAG && tamNumber*10 >= numberOfLocalRegisters)) {
              value = indirectAddressing(tamNumber + FIRST_LOCAL_REGISTER, tamNumberMin, tamNumberMax);

              if(lastErrorCode == 0) {
                indexOfItems[tamFunction].func(value);
              }
              calcModeNormal();
              return;
            }
            else {
              sprintf(tamBuffer, "%s " STD_RIGHT_ARROW ".%d_", indexOfItems[tamFunction].itemPrinted, tamNumber);
              transitionSystemState = 8;
            }
          }
          break;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s " STD_RIGHT_ARROW "__ ", indexOfItems[tamFunction].itemPrinted);
          transitionSystemState = 5;
          break;

        default : {}
      }
      break;

    //////////////////////////////
    // OP -->.d_
    case 8 :
      // Here we are sure that:
      // 0 <= tamNumber < numberOfLocalRegisters
      switch(tamTransition) {
        case TT_DIGIT :
          if((tamMode == TM_FLAG && tamNumber*10 + tamDigit < numberOfLocalFlags) || (tamMode != TM_FLAG && tamNumber*10 + tamDigit < numberOfLocalRegisters)) {
            value = indirectAddressing(tamNumber*10 + tamDigit + FIRST_LOCAL_REGISTER, tamNumberMin, tamNumberMax);

            if(lastErrorCode == 0) {
              indexOfItems[tamFunction].func(value);
            }
            calcModeNormal();
            return;
          }
          break;

        case TT_ENTER :
          value = indirectAddressing(tamNumber + FIRST_LOCAL_REGISTER, tamNumberMin, tamNumberMax);

          if(lastErrorCode == 0) {
            indexOfItems[tamFunction].func(value);
          }
          calcModeNormal();
          return;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s " STD_RIGHT_ARROW ".__", indexOfItems[tamFunction].itemPrinted);
          transitionSystemState = 7;
          break;

        default : {}
      }
      break;

    //////////////////////////////
    // OPo d_
    case 9 :
      switch(tamTransition) {
        case TT_DIGIT :
          indexOfItems[getStoRclOperation()].func(tamNumber*10 + tamDigit);
          calcModeNormal();
          return;

        case TT_ENTER :
          indexOfItems[getStoRclOperation()].func(tamNumber);
          calcModeNormal();
          return;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s%s __", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
          transitionSystemState = 1;
          break;

        default : {}
      }
      break;

    //////////////////////////////
    // OPo .__
    case 10 :
      // Here we are sure that:
      // numberOfLocalRegisters > 0
      switch(tamTransition) {
        case TT_DIGIT :
          tamNumber = tamDigit;
          if(tamNumber < numberOfLocalRegisters) {
            if(tamNumber > tamNumberMax) {
            }
            else if(tamNumber*10 >= numberOfLocalRegisters) {
              indexOfItems[tamFunction].func(tamNumber + FIRST_LOCAL_REGISTER);
              calcModeNormal();
              return;
            }
            else {
              sprintf(tamBuffer, "%s%s .%d_", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation, tamNumber);
              transitionSystemState = 11;
            }
          }
          break;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s%s __ ", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
          transitionSystemState = 1;
          break;

        default : {}
      }
      break;

    //////////////////////////////
    // OPo .d_
    case 11 :
      switch(tamTransition) {
        case TT_DIGIT :
          if(tamNumber*10 + tamDigit < numberOfLocalRegisters) {
            indexOfItems[getStoRclOperation()].func(tamNumber*10 + tamDigit + FIRST_LOCAL_REGISTER);
            calcModeNormal();
            return;
          }
          break;

        case TT_ENTER :
          if(tamNumber < numberOfLocalRegisters) {
            indexOfItems[getStoRclOperation()].func(tamNumber + FIRST_LOCAL_REGISTER);
            calcModeNormal();
            return;
          }
          break;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s%s .__", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
          transitionSystemState = 10;
          break;

        default : {}
      }
      break;

    //////////////////////////////
    // OPo -->__
    case 12 :
      switch(tamTransition) {
        case TT_LETTER :
          regist = indirectAddressing(tamLetteredRegister, 0, FIRST_LOCAL_REGISTER + numberOfLocalRegisters);

          if(lastErrorCode == 0) {
            indexOfItems[getStoRclOperation()].func(regist);
          }
          calcModeNormal();
          return;

        case TT_VARIABLE :
          break;

        case TT_DIGIT :
          tamNumber = tamDigit;
          sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW "%d_", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation, tamNumber);
          transitionSystemState = 13;
          break;

        case TT_DOT :
          if(numberOfLocalRegisters > 0) {
            sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW ".__", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
            transitionSystemState = 14;
          }
          break;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s%s __ ", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
          transitionSystemState = 1;
          break;

        default : {}
      }
      break;

    //////////////////////////////
    // OPo -->d_
    case 13 :
      switch(tamTransition) {
        case TT_DIGIT :
          regist = indirectAddressing(tamNumber*10 + tamDigit, 0, FIRST_LOCAL_REGISTER + numberOfLocalRegisters);

          if(lastErrorCode == 0) {
            indexOfItems[getStoRclOperation()].func(regist);
          }
          calcModeNormal();
          return;

        case TT_ENTER :
          regist = indirectAddressing(tamNumber, 0, FIRST_LOCAL_REGISTER + numberOfLocalRegisters);

          if(lastErrorCode == 0) {
            indexOfItems[getStoRclOperation()].func(regist);
          }
          calcModeNormal();
          return;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW "__", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
          transitionSystemState = 12;
          break;

        default : {}
      }
      break;

    //////////////////////////////
    // OPo -->.__
    case 14 :
      // Here we are sure that:
      // numberOfLocalRegisters > 0
      switch(tamTransition) {
        case TT_DIGIT :
          if(tamDigit < numberOfLocalRegisters) {
            tamNumber = tamDigit;
            sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW ".%d_", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation, tamNumber);
            transitionSystemState = 15;
          }
          break;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW "__ ", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
          transitionSystemState = 12;
          break;

        default : {}
      }
      break;

    //////////////////////////////
    // OPo -->.d_
    case 15 :
      switch(tamTransition) {
        case TT_DIGIT :
          if(tamNumber*10 + tamDigit < numberOfLocalRegisters) {
            regist = indirectAddressing(tamNumber*10 + tamDigit + FIRST_LOCAL_REGISTER, 0, FIRST_LOCAL_REGISTER + numberOfLocalRegisters);

            if(lastErrorCode == 0) {
              indexOfItems[getStoRclOperation()].func(regist);
            }
            calcModeNormal();
            return;
          }
          break;

        case TT_ENTER :
          regist = indirectAddressing(tamNumber + FIRST_LOCAL_REGISTER, 0, FIRST_LOCAL_REGISTER + numberOfLocalRegisters);

          if(lastErrorCode == 0) {
            indexOfItems[tamFunction].func(regist);
          }
          calcModeNormal();
          return;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW ".__", indexOfItems[tamFunction].itemPrinted, transitionSystemOperation);
          transitionSystemState = 14;
          break;

        default : {}
      }
      break;

    //////////////////////////////
    // This should never happen
    default :
      sprintf(errorMessage, "In function tamTransitionSystem: unknown state %" FMT16U " of the TAM transition system! This should never happen!", transitionSystemState);
      displayBugScreen(errorMessage);
      return;
  }

  if(stringWidth(tamBuffer, &standardFont, true, true) + 1 + lineTWidth > SCREEN_WIDTH) {
    clearRegisterLine(Y_POSITION_OF_TAM_LINE, 32);
  }
  showString(tamBuffer, &standardFont, 1, Y_POSITION_OF_TAM_LINE + 6, vmNormal, true, true);
}



void closeNim(void) {
  if((nimNumberPart == NP_INT_10 || nimNumberPart == NP_INT_16) && lastIntegerBase != 0) {
    strcat(nimBuffer, "#0");
    int16_t basePos = strlen(nimBuffer) - 1;
    if(lastIntegerBase <= 9) {
      nimBuffer[basePos] += lastIntegerBase;
    }
    else {
      nimBuffer[basePos++] = '1';
      nimBuffer[basePos] = '0';
      nimBuffer[basePos + 1] = 0;
      nimBuffer[basePos] += lastIntegerBase - 10;
    }

    nimNumberPart = NP_INT_BASE;
  }
  else {
    lastIntegerBase = 0;
  }

  int16_t lastChar = strlen(nimBuffer) - 1;

  if(nimNumberPart != NP_INT_16) { // We need a # and a base
    if(nimNumberPart != NP_INT_BASE || nimBuffer[lastChar] != '#') { // We need a base
      if((nimNumberPart == NP_COMPLEX_EXPONENT || nimNumberPart == NP_REAL_EXPONENT) && (nimBuffer[lastChar] == '+' || nimBuffer[lastChar] == '-') && nimBuffer[lastChar - 1] == 'e') {
        nimBuffer[--lastChar] = 0;
        lastChar--;
      }
      else if(nimNumberPart == NP_REAL_EXPONENT && nimBuffer[lastChar] == 'e') {
        nimBuffer[lastChar--] = 0;
      }

      if(nimNumberPart == NP_COMPLEX_INT_PART && (nimBuffer[lastChar] == 'i' || nimBuffer[lastChar-1]*256 + nimBuffer[lastChar]*256 == 0xa221)) { // complex i or measured angle
        nimBuffer[++lastChar] = '1';
        nimBuffer[lastChar + 1] = 0;
      }

      if((nimBuffer[0] != '-' || nimBuffer[1] != 0) && (nimBuffer[lastChar] != '-')) { // The buffer is not just the minus sign AND The last char of the buffer is not the minus sign
        calcModeNormal();

        if(nimNumberPart == NP_INT_10) {
          longInteger_t tmp;

          stringToLongInteger(nimBuffer + (nimBuffer[0] == '+' ? 1 : 0), 10, &tmp);
          convertLongIntegerToLongIntegerRegister(&tmp, REGISTER_X);
        }
        else if(nimNumberPart == NP_INT_BASE) {
          //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          // Any change in this part is to be reported in the function strToShortInteger from file testSuite.c after the line: else if(nimNumberPart == NP_INT_BASE) {
          //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          longInteger_t minVal, value, maxVal;
          int16_t posHash, i, lg;
          int32_t base;

          lg = strlen(nimBuffer);
          posHash = 0;
          for(i=1; i<lg; i++) {
            if(nimBuffer[i] == '#') {
              posHash = i;
              break;
            }
          }

          for(i=posHash+1; i<lg; i++) {
            if(nimBuffer[i]<'0' || nimBuffer[i]>'9') {
              // This should never happen
              displayCalcErrorMessage(9, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                showInfoDialog("In function closeNIM:", "there is a non numeric character in the base of the integer!", NULL, NULL);
              #endif
              return;
            }
          }

          base = atoi(nimBuffer + posHash + 1);
          if(base < 2 || base > 16) {
            displayCalcErrorMessage(9, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
             showInfoDialog("In function closeNIM:", "the base of the integer must be from 2 to 16!", NULL, NULL);
            #endif
            return;
          }

          for(i=nimBuffer[0] == '-' ? 1 : 0; i<posHash; i++) {
            if((nimBuffer[i] > '9' ? nimBuffer[i] - 'A' + 10 : nimBuffer[i] - '0') >= base) {
              displayCalcErrorMessage(9, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                sprintf(errorMessage, "digit %c is not allowed in base %d!", nimBuffer[i], base);
                showInfoDialog("In function closeNIM:", errorMessage, NULL, NULL);
              #endif

              restoreStack();
              return;
            }
          }

          stringToLongInteger(nimBuffer + (nimBuffer[0] == '+' ? 1 : 0), base, &value);

          // maxVal = 2^shortIntegerWordSize
          if(shortIntegerWordSize >= 1 && shortIntegerWordSize <= 64) {
            longInteger2Exp(shortIntegerWordSize, &maxVal);
          }
          else {
            sprintf(errorMessage, "In function closeNIM: %d is an unexpected value for shortIntegerWordSize!", shortIntegerWordSize);
            displayBugScreen(errorMessage);
            return;
          }

          // minVal = -maxVal/2
          longIntegerSetZero(&minVal); // Mandatory! Else segmentation fault at next instruction
          longIntegerDivideUInt(&maxVal, 2, &minVal); // minVal = maxVal / 2
          longIntegerSetNegativeSign(&minVal); // minVal = -minVal

          if(shortIntegerMode != SIM_UNSIGN) {
            longIntegerDivideUInt(&maxVal, 2, &maxVal); // maxVal /= 2
          }

          longIntegerSubtractUInt(&maxVal, 1, &maxVal); // maxVal--

          if(shortIntegerMode == SIM_UNSIGN) {
            longIntegerSetZero(&minVal); // minVal = 0
          }

          if(shortIntegerMode == SIM_1COMPL || shortIntegerMode == SIM_SIGNMT) {
            longIntegerAddUInt(&minVal, 1, &minVal); // minVal++
          }

          if(longIntegerCompare(&value, &minVal) == LONG_INTEGER_LESS_THAN || longIntegerCompare(&value, &maxVal) == LONG_INTEGER_GREATER_THAN) {
            displayCalcErrorMessage(14, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
              char strMin[22], strMax[22];
              longIntegerToString(&minVal, strMin, 10);
              longIntegerToString(&maxVal, strMax, 10);
              sprintf(errorMessage, "For word size of %d bit%s and integer mode %s,", shortIntegerWordSize, shortIntegerWordSize>1 ? "s" : "", getShortIntegerModeName(shortIntegerMode));
              sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "the entered number must be from %s to %s!", strMin, strMax);
              showInfoDialog("In function closeNIM:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
            #endif
            return;
          }

          reallocateRegister(REGISTER_X, dtShortInteger, SHORT_INTEGER_SIZE, base);

          char strValue[22];
          longIntegerToString(&value, strValue, 10);

          uint64_t val;
          if(value.sign) {
            val = atoll(strValue + 1); // value is negative: discard the minus sign
          }
          else {
            val = atoll(strValue); // value is positive
          }

          if(shortIntegerMode == SIM_UNSIGN) {
          }
          else if(shortIntegerMode == SIM_2COMPL) {
            if(value.sign) {
              val = (~val + 1) & shortIntegerMask;
            }
          }
          else if(shortIntegerMode == SIM_1COMPL) {
            if(value.sign) {
              val = ~val & shortIntegerMask;
            }
          }
          else if(shortIntegerMode == SIM_SIGNMT) {
            if(value.sign) {
              val = (val & shortIntegerMask) | shortIntegerSignBit;
            }
          }
          else {
            sprintf(errorMessage, "In function closeNIM: %d is an unexpected value for shortIntegerMode!", shortIntegerMode);
            displayBugScreen(errorMessage);
            *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = 0;
          }

          *(REGISTER_SHORT_INTEGER_DATA(REGISTER_X)) = val;
          lastIntegerBase = base;
        }
        else if(nimNumberPart == NP_REAL_FLOAT_PART || nimNumberPart == NP_REAL_EXPONENT) {
          if(nimInputIsReal34) {
            reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, TAG_NONE);
            stringToReal34(nimBuffer, REGISTER_REAL34_DATA(REGISTER_X));
          }
          else {
            reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);
            stringToReal16(nimBuffer, REGISTER_REAL16_DATA(REGISTER_X));
          }
          if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
            displayCalcErrorMessage(1, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
              showInfoDialog("In function closeNIM:", "the absolute value of a real must be less than 10^385!", "Unless D flag (Danger) is set.", NULL);
            #endif
          }
        }
        else if(nimNumberPart == NP_FRACTION_DENOMINATOR) {
          int16_t i, posSpace, posSlash, lg;
          int32_t integer, numer, denom;
          real16_t temp;

          lg = strlen(nimBuffer);

          posSpace = 0;
          for(i=2; i<lg; i++) {
            if(nimBuffer[i] == ' ') {
              posSpace = i;
              break;
            }
          }

          for(i=1; i<posSpace; i++) {
            if(nimBuffer[i]<'0' || nimBuffer[i]>'9') { // This should never happen
              displayCalcErrorMessage(28, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                showInfoDialog("In function parseNimString:", "there is a non numeric character in the integer part of the fraction!", NULL, NULL);
              #endif
              return;
            }
          }

          posSlash = 0;
          for(i=posSpace+2; i<lg; i++) {
            if(nimBuffer[i] == '/') {
              posSlash = i;
              break;
            }
          }

          for(i=posSpace+1; i<posSlash; i++) {
            if(nimBuffer[i]<'0' || nimBuffer[i]>'9') { // This should never happen
             displayCalcErrorMessage(28, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
             #if (EXTRA_INFO_ON_CALC_ERROR == 1)
               showInfoDialog("In function parseNimString:", "there is a non numeric character in the numerator part of the fraction!", NULL, NULL);
             #endif
             return;
            }
          }

          for(i=posSlash+1; i<lg; i++) {
            if(nimBuffer[i]<'0' || nimBuffer[i]>'9') {
              displayCalcErrorMessage(28, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                showInfoDialog("In function parseNimString:", "there is a non numeric character in the denominator part of the fraction!", NULL, NULL);
              #endif
              return;
            }
          }

          nimBuffer[posSpace] = 0;
          nimBuffer[posSlash] = 0;
          integer = atoi(nimBuffer + 1);
          numer   = atoi(nimBuffer + posSpace + 1);
          denom   = atoi(nimBuffer + posSlash + 1);

          if(denom == 0 && !getFlag(FLAG_DANGER)) {
            displayCalcErrorMessage(1, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
              showInfoDialog("In function parseNimString:", "the denominator of the fraction should not be 0!", "Unless D flag (Danger) is set.", NULL);
            #endif
            return;
          }

          reallocateRegister(REGISTER_X, dtReal16, REAL16_SIZE, TAG_NONE);
          int32ToReal16(numer, REGISTER_REAL16_DATA(REGISTER_X));
          int32ToReal16(denom, &temp);
          real16Divide(REGISTER_REAL16_DATA(REGISTER_X), &temp, REGISTER_REAL16_DATA(REGISTER_X));
          int32ToReal16(integer, &temp);
          real16Add(REGISTER_REAL16_DATA(REGISTER_X), &temp, REGISTER_REAL16_DATA(REGISTER_X));
          if(nimBuffer[0] == '-')
           real16SetNegativeSign(REGISTER_REAL16_DATA(REGISTER_X));

          displayRealAsFraction = true;
          if(integer != 0) {
            fractionType = FT_PROPER;
          }
          else {
            fractionType = FT_IMPROPER;
          }
        }
        else if(nimNumberPart == NP_COMPLEX_INT_PART || nimNumberPart == NP_COMPLEX_FLOAT_PART || nimNumberPart == NP_COMPLEX_EXPONENT) {
          int16_t imaginarySign;

          reallocateRegister(REGISTER_X, dtComplex16, COMPLEX16_SIZE, TAG_NONE);

          if(nimBuffer[imaginaryMantissaSignLocation] == '+') {
            imaginarySign = 1;
          }
          else {
            imaginarySign = -1;
          }
          nimBuffer[imaginaryMantissaSignLocation] = 0;

          stringToReal16(nimBuffer, REGISTER_REAL16_DATA(REGISTER_X));
          if(real16IsInfinite(REGISTER_REAL16_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
            displayCalcErrorMessage(1, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
              showInfoDialog("In function closeNIM:", "the absolute value of the real part (or magnitude) of a complex must be less than 10^385!", "Unless D flag (Danger) is set.", NULL);
            #endif
          }

          stringToReal16(nimBuffer + imaginaryMantissaSignLocation + 2, REGISTER_IMAG16_DATA(REGISTER_X));
          if(imaginarySign == -1) {
            real16SetNegativeSign(REGISTER_IMAG16_DATA(REGISTER_X));
          }
          if(real16IsInfinite(REGISTER_IMAG16_DATA(REGISTER_X)) && !getFlag(FLAG_DANGER)) {
            displayCalcErrorMessage(1, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
              showInfoDialog("In function closeNIM:", "the absolute value of the imaginary part (or angle) of a complex must be less than 10^385!", "Unless D flag (Danger) is set.", NULL);
            #endif
          }

          if(complexMode == CM_POLAR) {
            if(real16CompareEqual(REGISTER_REAL16_DATA(REGISTER_X), const16_0)) {
              real16Zero(REGISTER_IMAG16_DATA(REGISTER_X));
            }
            else {
              real16_t magnitude16, theta16;

              real16Copy(REGISTER_REAL16_DATA(REGISTER_X), &magnitude16);
              real16Copy(REGISTER_IMAG16_DATA(REGISTER_X), &theta16);
              convertAngle16FromTo(&theta16, currentAngularMode, AM_RADIAN);
              if(real16CompareLessThan(&magnitude16, const16_0)) {
                real16SetPositiveSign(&magnitude16);
                real16Add(&theta16, const16_pi, &theta16);
                real16Remainder(&theta16, const16_2pi, &theta16);
              }
              real16PolarToRectangular(&magnitude16, &theta16, REGISTER_REAL16_DATA(REGISTER_X), REGISTER_IMAG16_DATA(REGISTER_X)); // theta16 in radian
              printf("X = "); printRegisterToConsole(REGISTER_X, 0); printf("\n");
            }
          }
        }
        else {
          sprintf(errorMessage, "In function closeNIM: %d is an unexpected nimNumberPart value!", nimNumberPart);
          displayBugScreen(errorMessage);
        }
      }
    }
  }

  refreshRegisterLine(NIM_REGISTER_LINE);
}
#endif
