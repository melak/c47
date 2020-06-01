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
  calcModeAim(NOPARAM); // Alpha Input Mode
}



uint16_t convertItemToSubOrSup(uint16_t item, int16_t subOrSup) {
  nextChar = NC_NORMAL;

  if(subOrSup == NC_SUBSCRIPT) {
    switch(item) {
      case CHR_alpha    : return CHR_SUB_alpha;
      case CHR_delta    : return CHR_SUB_delta;
      case CHR_mu       : return CHR_SUB_mu;
      case CHR_SUN      : return CHR_SUB_SUN;
      case CHR_h        : return CHR_SUB_h;
      case CHR_t        : return CHR_SUB_t;
      case CHR_INFINITY : return CHR_SUB_INFINITY;
      case CHR_s        : return CHR_SUB_s;
      case CHR_PLUS     : return CHR_SUB_PLUS;
      case CHR_MINUS    : return CHR_SUB_MINUS;
      case CHR_0        : return CHR_SUB_0;
      case CHR_1        : return CHR_SUB_1;
      case CHR_2        : return CHR_SUB_2;
      case CHR_3        : return CHR_SUB_3;
      case CHR_4        : return CHR_SUB_4;
      case CHR_5        : return CHR_SUB_5;
      case CHR_6        : return CHR_SUB_6;
      case CHR_7        : return CHR_SUB_7;
      case CHR_8        : return CHR_SUB_8;
      case CHR_9        : return CHR_SUB_9;
      case CHR_a        : return CHR_SUB_a;
      case CHR_b        : return CHR_SUB_b;
      case CHR_c        : return CHR_SUB_c;
      case CHR_d        : return CHR_SUB_d;
      case CHR_e        : return CHR_SUB_e;
      case CHR_i        : return CHR_SUB_i;
      case CHR_j        : return CHR_SUB_j;
      case CHR_k        : return CHR_SUB_k;
      case CHR_l        : return CHR_SUB_l;
      case CHR_m        : return CHR_SUB_m;
      case CHR_n        : return CHR_SUB_n;
      case CHR_o        : return CHR_SUB_o;
      case CHR_p        : return CHR_SUB_p;
      case CHR_q        : return CHR_SUB_q;
      case CHR_u        : return CHR_SUB_u;
      case CHR_v        : return CHR_SUB_v;
      case CHR_w        : return CHR_SUB_w;
      case CHR_x        : return CHR_SUB_x;
      case CHR_y        : return CHR_SUB_y;
      case CHR_z        : return CHR_SUB_z;
      case CHR_A        : return CHR_SUB_A;
      case CHR_B        : return CHR_SUB_B;
      case CHR_C        : return CHR_SUB_C;
      case CHR_D        : return CHR_SUB_D;
      case CHR_E        : return CHR_SUB_E;
      case CHR_F        : return CHR_SUB_F;
      case CHR_G        : return CHR_SUB_G;
      case CHR_H        : return CHR_SUB_H;
      case CHR_I        : return CHR_SUB_I;
      case CHR_J        : return CHR_SUB_J;
      case CHR_K        : return CHR_SUB_K;
      case CHR_L        : return CHR_SUB_L;
      case CHR_M        : return CHR_SUB_M;
      case CHR_N        : return CHR_SUB_N;
      case CHR_O        : return CHR_SUB_O;
      case CHR_P        : return CHR_SUB_P;
      case CHR_Q        : return CHR_SUB_Q;
      case CHR_R        : return CHR_SUB_R;
      case CHR_S        : return CHR_SUB_S;
      case CHR_T        : return CHR_SUB_T;
      case CHR_U        : return CHR_SUB_U;
      case CHR_V        : return CHR_SUB_V;
      case CHR_W        : return CHR_SUB_W;
      case CHR_X        : return CHR_SUB_X;
      case CHR_Y        : return CHR_SUB_Y;
      case CHR_Z        : return CHR_SUB_Z;
      default           : return item;
    }
  }
  else if(subOrSup == NC_SUPERSCRIPT) {
    switch(item) {
      case CHR_a        : return CHR_SUP_a;
      case CHR_x        : return CHR_SUP_x;
      case CHR_INFINITY : return CHR_SUP_INFINITY;
      case CHR_PLUS     : return CHR_SUP_PLUS;
      case CHR_MINUS    : return CHR_SUP_MINUS;
      case CHR_0        : return CHR_SUP_0;
      case CHR_1        : return CHR_SUP_1;
      case CHR_2        : return CHR_SUP_2;
      case CHR_3        : return CHR_SUP_3;
      case CHR_4        : return CHR_SUP_4;
      case CHR_5        : return CHR_SUP_5;
      case CHR_6        : return CHR_SUP_6;
      case CHR_7        : return CHR_SUP_7;
      case CHR_8        : return CHR_SUP_8;
      case CHR_9        : return CHR_SUP_9;
      case CHR_f        : return CHR_SUP_f;
      case CHR_g        : return CHR_SUP_g;
      case CHR_h        : return CHR_SUP_h;
      case CHR_r        : return CHR_SUP_r;
      case CHR_T        : return CHR_SUP_T;
      default           : return item;
    }
  }

  return item;
}



int32_t findFirstItem(const char *twoLetters) {
  const int16_t *first, *middle, *last;

  first = softmenu[softmenuStack[softmenuStackPointer-1].softmenu].softkeyItem;
  last = first + softmenu[softmenuStack[softmenuStackPointer-1].softmenu].numItems - 1;
  while(*last == ITM_NULL) {
    last--;
  }

  middle = first + (last - first) / 2;
  //const int16_t *f = softmenu[softmenuStack[softmenuStackPointer-1].softmenu].softkeyItem;
  //printf("\n----------------------------------\nfirst  = %3" FMT64S "   %3d\n", (int64_t)(first - f), *first);
  //printf("middle = %3" FMT64S "   %3d\n", (int64_t)(middle - f), *middle);
  //printf("last   = %3" FMT64S "   %3d\n", (int64_t)(last - f), *last);
  while(first + 1 < last) {
    if(compareString(twoLetters, indexOfItems[abs(*middle)].itemCatalogName, CMP_CLEANED_STRING_ONLY) <= 0) {
      last = middle;
    }
    else {
      first = middle;
    }

    middle = first + (last - first) / 2;
  //printf("\nfirst  = %3" FMT64S "   %3d\n", (int64_t)(first - f), *first);
  //printf("middle = %3" FMT64S "   %3d\n", (int64_t)(middle - f), *middle);
  //printf("last   = %3" FMT64S "   %3d\n", (int64_t)(last - f), *last);
  }

  if(compareString(twoLetters, indexOfItems[abs(*first)].itemCatalogName, CMP_CLEANED_STRING_ONLY) <= 0) {
    //printf("first\n");
    return first - softmenu[softmenuStack[softmenuStackPointer-1].softmenu].softkeyItem;
  }
  else {
    //printf("last\n");
    return last - softmenu[softmenuStack[softmenuStackPointer-1].softmenu].softkeyItem;
  }
}


void resetAlphaSelectionBuffer(void) {
  lgCatalogSelection = 0;
  alphaSelectionTimer = 0;
  asmBuffer[0] = 0;
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
      item = convertItemToSubOrSup(item, nextChar);

      if(stringByteLength(aimBuffer) + stringByteLength(indexOfItems[item].itemSoftmenuName) >= AIM_BUFFER_LENGTH) { /// TODO this error should never happen but who knows!
        sprintf(errorMessage, "In function addItemToBuffer: the AIM input buffer is full! %d bytes for now", AIM_BUFFER_LENGTH);
        displayBugScreen(errorMessage);
      }
      else {
        strncpy(aimBuffer + stringNextGlyph(aimBuffer, stringLastGlyph(aimBuffer)), indexOfItems[item].itemSoftmenuName, stringByteLength(indexOfItems[item].itemSoftmenuName) + 1);
        if(stringWidth(aimBuffer, &standardFont, true, true) > SCREEN_WIDTH - 8) { // 8 is the width of the cursor
          btnClicked(NULL, "16"); // back space
          #ifdef PC_BUILD
            showInfoDialog("In function addItemToBuffer:", "the aimBuffer string is too wide!", NULL, NULL);
          #endif
        }
        else {
          xCursor = showString(aimBuffer, &standardFont, 1, Y_POSITION_OF_AIM_LINE + 6, vmNormal, true, true);
        }
      }
    }

    else if(calcMode == CM_TAM) {
      if(item==ITM_Max || item==ITM_Min || item==ITM_ADD || item==ITM_SUB || item==ITM_MULT || item==ITM_DIV || item==ITM_Config || item==ITM_Stack || item==ITM_dddEL || item==ITM_dddIJ) { // Operation
        tamOperation = item;
        tamTransitionSystem(TT_OPERATION);
      }
      else if(tamFunction == ITM_toINT && item == ITM_ST_D) {
        tamTransitionSystem(TT_BASE10);
      }
      else if(tamFunction == ITM_toINT && item == ITM_HEX) {
        tamTransitionSystem(TT_BASE16);
      }
      else if(REGISTER_X <= indexOfItems[item].param && indexOfItems[item].param <= REGISTER_K) { // Lettered register
        tamLetteredRegister = indexOfItems[item].param;
        tamTransitionSystem(TT_LETTER);
      }
      /*else if(namedVariable) {
        tamTransitionSystem(TT_VARIABLE);
      }*/
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
      else if((tamMode == TM_FLAGR || tamMode == TM_FLAGW)) {
        tamTransitionSystem(TT_NOTHING);
      }
      else {
        tamTransitionSystem(TT_NOTHING);
      }
    }

    else if(calcMode == CM_ASM || calcMode == CM_ASM_OVER_TAM || calcMode == CM_ASM_OVER_AIM) {
      int32_t firstItem = 0, pos;

      if(item == KEY_BACKSPACE) {
        calcModeNormal();
        return;
      }

      else {
        if(stringGlyphLength(indexOfItems[item].itemSoftmenuName) == 1) {
          pos = lgCatalogSelection++;
          if(asmBuffer[pos] != 0) {
            pos++;
          }

          asmBuffer[pos++] = indexOfItems[item].itemSoftmenuName[0];
          if(indexOfItems[item].itemSoftmenuName[0] & 0x80) { // 2 bytes
            asmBuffer[pos++] = indexOfItems[item].itemSoftmenuName[1];
          }
          asmBuffer[pos] = 0;

          firstItem = findFirstItem(asmBuffer);
        }
      }

      softmenuStack[softmenuStackPointer-1].firstItem = firstItem;
      showSoftmenuCurrentPart();
      setCatalogLastPos();
      alphaSelectionTimer = getUptimeMs();
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
  int16_t lastChar, index;
  uint8_t savedNimNumberPart;
  bool_t done;

  if(calcMode == CM_NORMAL) {
    switch(item) {
      case ITM_EXPONENT :
        if( eRPN ) {                 //JM NEWERPN vv  NOTE: Rather force a stack lift before NIM (than after ENTER) so that normal ENTER SLS can be used for cancelling ENTER using NOP
            calcModeNim(NOPARAM);
            STACK_LIFT_DISABLE;      
        } 
        else {                       //JM NEWERPN ^^
        calcModeNim(NOPARAM);
        }                            //JM NEWERPN
        nimBuffer[0] = '+';
        nimBuffer[1] = '1';
        nimBuffer[2] = '.';
        nimBuffer[3] = 0;
        nimNumberPart = NP_REAL_FLOAT_PART;
        lastIntegerBase = 0;
        break;

      case CHR_PERIOD :
        if( eRPN ) {                 //JM NEWERPN vv
            calcModeNim(NOPARAM);
            STACK_LIFT_DISABLE;      
        } 
        else {                       //JM NEWERPN ^^
        calcModeNim(NOPARAM);
        }                            //JM NEWERPN
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
        if( eRPN ) {                 //JM NEWERPN vv
            calcModeNim(NOPARAM);
            STACK_LIFT_DISABLE;      
        } 
        else {                       //JM NEWERPN ^^
        calcModeNim(NOPARAM);
        }                            //JM NEWERPN
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

            strcat(nimBuffer, indexOfItems[item].itemSoftmenuName);
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

            if(atoi(nimBuffer + exponentSignLocation) > exponentLimit || atoi(nimBuffer + exponentSignLocation) < -exponentLimit) {
              nimBuffer[strlen(nimBuffer) - 1] = 0;
            }
          }
          else {
            if(nimBuffer[exponentSignLocation + 1] == '0') {
              nimBuffer[strlen(nimBuffer) - 1] = 0;
            }

            strcat(nimBuffer, indexOfItems[item].itemSoftmenuName);

            if(atoi(nimBuffer + exponentSignLocation) > exponentLimit || atoi(nimBuffer + exponentSignLocation) < -exponentLimit) {
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
            strcat(nimBuffer, indexOfItems[item].itemSoftmenuName);

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

            strcat(nimBuffer, indexOfItems[item].itemSoftmenuName);
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

            if(atoi(nimBuffer + imaginaryExponentSignLocation) > exponentLimit || atoi(nimBuffer + imaginaryExponentSignLocation) < -exponentLimit) {
              nimBuffer[strlen(nimBuffer) - 1] = 0;
            }
          }
          else {
            if(nimBuffer[imaginaryExponentSignLocation + 1] == '0') {
              nimBuffer[strlen(nimBuffer) - 1] = 0;
            }

            strcat(nimBuffer, indexOfItems[item].itemSoftmenuName);

            if(atoi(nimBuffer + imaginaryExponentSignLocation) > exponentLimit || atoi(nimBuffer + imaginaryExponentSignLocation) < -exponentLimit) {
              nimBuffer[strlen(nimBuffer) - 1] = 0;
            }
          }
          break;

        default :
          if(nimNumberPart == NP_EMPTY) {
            nimNumberPart = NP_INT_10;
            //debugNIM();
          }

          strcat(nimBuffer, indexOfItems[item].itemSoftmenuName);
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

        strcat(nimBuffer, indexOfItems[item].itemSoftmenuName);
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

      lastIntegerBase = 0;

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

      lastIntegerBase = 0;

      switch(nimNumberPart) {
        case NP_INT_10 :
          strcat(nimBuffer, "."); // no break here
          #ifndef __APPLE__
            __attribute__ ((fallthrough));
          #endif
        case NP_REAL_FLOAT_PART :
          strcat(nimBuffer, "e+");
          exponentSignLocation = strlen(nimBuffer) - 1;

          nimNumberPart = NP_REAL_EXPONENT;
          //debugNIM();
          break;

        case NP_COMPLEX_INT_PART :
          strcat(nimBuffer, "."); // no break here
          #ifndef __APPLE__
            __attribute__ ((fallthrough));
          #endif
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

      lastIntegerBase = 0;

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

      lastIntegerBase = 0;

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
          #ifndef __APPLE__
            __attribute__ ((fallthrough));
          #endif

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
      if(nimNumberPart == NP_COMPLEX_INT_PART && nimBuffer[strlen(nimBuffer) - 1] == 'i') {
        done = true;
        strcat(nimBuffer, "3.141592653589793238462643383279503");
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

    case KEY_EXIT1 :
      done = true;
      closeNim();
      if(calcMode != CM_NIM && lastErrorCode == 0) {
        STACK_LIFT_ENABLE;
        return;
      }
      break;

    case ITM_LOG10 :                                                                //JM layout different
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

    case ITM_1ONX :                                                                 //JM layout different Added B for binary
      if(nimNumberPart == NP_INT_BASE && nimBuffer[strlen(nimBuffer) - 1] == '#') { // Binary. Only works in direct NIM
        done = true;

        strcat(nimBuffer, "2");
      }
      break;

    case ITM_DMS :
      if(nimNumberPart == NP_INT_10 || nimNumberPart == NP_REAL_FLOAT_PART) {
        done = true;

        closeNim();
        if(calcMode != CM_NIM && lastErrorCode == 0) {
          if(getRegisterDataType(REGISTER_X) == dtLongInteger) {
            convertLongIntegerRegisterToReal34Register(REGISTER_X, REGISTER_X);
          }

          checkDms34(REGISTER_REAL34_DATA(REGISTER_X));
          setRegisterAngularMode(REGISTER_X, AM_DMS);

          refreshRegisterLine(REGISTER_X);
          STACK_LIFT_ENABLE;
          return;
        }
      }
      break;

    default : keyActionProcessed = false;
  }

  if(done) {
    //Convert nimBuffer to display string

    strcpy(nimBufferDisplay, STD_SPACE_HAIR);

    switch(nimNumberPart) {
      case NP_INT_10 :          // +12345
      case NP_INT_16 :          // +123AB
      case NP_INT_BASE :        // +123AB#16
        nimBufferToDisplayBuffer(nimBuffer, nimBufferDisplay + 2);
        break;

      case NP_REAL_FLOAT_PART : // +12345.6789
        nimBufferToDisplayBuffer(nimBuffer, nimBufferDisplay + 2);
        break;

      case NP_REAL_EXPONENT : // +12345.678e+3
        nimBufferToDisplayBuffer(nimBuffer, nimBufferDisplay + 2);

        exponentToDisplayString(atoi(nimBuffer + exponentSignLocation), nimBufferDisplay + stringByteLength(nimBufferDisplay), NULL, true, STD_SPACE_PUNCTUATION);
        if(nimBuffer[exponentSignLocation + 1] == 0 && nimBuffer[exponentSignLocation] == '-') {
          strcat(nimBufferDisplay, STD_SUP_MINUS);
        }
        else if(nimBuffer[exponentSignLocation + 1] == '0' && nimBuffer[exponentSignLocation] == '+') {
          strcat(nimBufferDisplay, STD_SUP_0);
        }
        break;

      case NP_FRACTION_DENOMINATOR : // +123 12/7
        nimBufferToDisplayBuffer(nimBuffer, nimBufferDisplay + 2);
        strcat(nimBufferDisplay, STD_SPACE_4_PER_EM);

        for(index=2; nimBuffer[index]!=' '; index++); // The ending semi colon is OK here
        supNumberToDisplayString(atoi(nimBuffer + index + 1), nimBufferDisplay + stringByteLength(nimBufferDisplay), NULL, true, STD_SPACE_PUNCTUATION);

        strcat(nimBufferDisplay, "/");

        for(; nimBuffer[index]!='/'; index++); // The ending semi colon is OK here
        if(nimBuffer[++index] != 0) {
          subNumberToDisplayString(atoi(nimBuffer + index), nimBufferDisplay + stringByteLength(nimBufferDisplay), NULL);
        }
        break;

      case NP_COMPLEX_INT_PART :   // +1.2+i15
      case NP_COMPLEX_FLOAT_PART : // +1.2+i15.69
      case NP_COMPLEX_EXPONENT :   // +1.2+i15.69e2
        // Real part
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
          exponentToDisplayString(atoi(nimBuffer + exponentSignLocation), nimBufferDisplay + stringByteLength(nimBufferDisplay), NULL, true, STD_SPACE_PUNCTUATION);
          if(nimBuffer[exponentSignLocation + 1] == 0 && nimBuffer[exponentSignLocation] == '-') {
            strcat(nimBufferDisplay, STD_SUP_MINUS);
          }
          else if(nimBuffer[exponentSignLocation + 1] == '0' && nimBuffer[exponentSignLocation] == '+') {
            strcat(nimBufferDisplay, STD_SUP_0);
          }
        }

        nimNumberPart = savedNimNumberPart;

        // Complex "separator"
        if(getSystemFlag(FLAG_POLAR)) { // polar mode
          strcat(nimBufferDisplay, STD_SPACE_4_PER_EM STD_MEASURED_ANGLE STD_SPACE_4_PER_EM);
          if(nimBuffer[imaginaryMantissaSignLocation] == '-') {
            strcat(nimBufferDisplay, "-");
          }
        }
        else { // rectangular mode
          if(strncmp(nimBufferDisplay + stringByteLength(nimBufferDisplay) - 2, STD_SPACE_HAIR, 2) != 0) {
            strcat(nimBufferDisplay, STD_SPACE_HAIR);
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

        // Imaginary part
        if(nimBuffer[imaginaryMantissaSignLocation+2] != 0) {
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
            exponentToDisplayString(atoi(nimBuffer + imaginaryExponentSignLocation), nimBufferDisplay + stringByteLength(nimBufferDisplay), NULL, true, STD_SPACE_PUNCTUATION);
            if(nimBuffer[imaginaryExponentSignLocation + 1] == 0 && nimBuffer[imaginaryExponentSignLocation] == '-') {
              strcat(nimBufferDisplay, STD_SUP_MINUS);
            }
            else if(nimBuffer[imaginaryExponentSignLocation + 1] == '0' && nimBuffer[imaginaryExponentSignLocation] == '+') {
              strcat(nimBufferDisplay, STD_SUP_0);
            }
          }

          nimNumberPart = savedNimNumberPart;
        }
        break;

      default :
        sprintf(errorMessage, "In function addItemToNimBuffer: %d is an unexpected nimNumberPart value while converting buffer to display!", nimNumberPart);
        displayBugScreen(errorMessage);
    }

    if(!getSystemFlag(FLAG_DECIMP)) {
      for(index=stringByteLength(nimBufferDisplay) - 1; index>0; index--) {
        if(nimBufferDisplay[index] == '.') {
          nimBufferDisplay[index] = ',';
        }
      }
    }

    refreshRegisterLine(NIM_REGISTER_LINE);
  }

  else {
    closeNim();
    if(calcMode != CM_NIM) {
      if(item == ITM_pi || indexOfItems[item].func == fnConstant) {
        STACK_LIFT_ENABLE;
      }

      if(lastErrorCode == 0) {
        showFunctionName(item, 10);
      }
    }
  }
}



static int16_t insertGapIP(char *displayBuffer, int16_t numDigits, int16_t nth) {
  if(groupingGap == 0)         return 0; // no gap when none is required!
  if(numDigits <= groupingGap) return 0; // there are less than groupingGap digits
  if(nth + 1 == numDigits)     return 0; // no gap after the last digit

  if((numDigits - nth) % groupingGap == 1 || groupingGap == 1) {
    strcpy(displayBuffer, STD_SPACE_PUNCTUATION);
    return 2;
  }

  return 0;
}

static int16_t insertGapFP(char *displayBuffer, int16_t numDigits, int16_t nth) {
  if(groupingGap == 0)         return 0; // no gap when none is required!
  if(numDigits <= groupingGap) return 0; // there are less than groupingGap digits
  if(nth + 1 == numDigits)     return 0; // no gap after the last digit

  if(nth % groupingGap == groupingGap - 1) {
    strcpy(displayBuffer, STD_SPACE_PUNCTUATION);
    return 2;
  }

  return 0;
}



void nimBufferToDisplayBuffer(const char *nimBuffer, char *displayBuffer) {
  int16_t numDigits, source, dest;

  if(*nimBuffer == '-') {
    *(displayBuffer++) = '-';
  }
  nimBuffer++;

  int16_t groupingGapM = groupingGap;                       //JMGAP vv
  switch(nimNumberPart) {
    case NP_INT_10:                     // +12345 longint; Do not change groupingGap. Leave at user setting, default 3.
    case NP_INT_BASE:                   // +123AB#16.    ; Change groupinggap from user selection to this table, for entry
      switch(lastIntegerBase) {
        case  0: groupingGap = groupingGapM; break;
        case  2: groupingGap = 4; break;
        case  3: groupingGap = 3; break;
        case  4: groupingGap = 2; break;
        case  5:
        case  6:
        case  7:
        case  8:
        case  9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15: groupingGap = 3; break;
        case 16: groupingGap = 2; break;
        default: break;
      }
      break;
    case NP_INT_16:                     // +123AB.       ; Change to 2 for hex.
      groupingGap = 2;
      break;
    default:
      break;
  }                                                         //JMGAP ^^

  for(numDigits=0; nimBuffer[numDigits]!=0 && nimBuffer[numDigits]!='e' && nimBuffer[numDigits]!='.' && nimBuffer[numDigits]!=' ' && nimBuffer[numDigits]!='#' && nimBuffer[numDigits]!='+' && nimBuffer[numDigits]!='-'; numDigits++); // The semicolon here is OK

  for(source=0, dest=0; source<numDigits; source++) {
    displayBuffer[dest++] = nimBuffer[source];
    dest += insertGapIP(displayBuffer + dest, numDigits, source);
  }

  groupingGap = groupingGapM;                               //JMGAP
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



int16_t getOperation(void) {
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
        default :         return ITM_STO;
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
        default :         return ITM_RCL;
      }
      break;

    default :             return tamFunction;
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
            if(tamCurrentOperation == ITM_Config || tamCurrentOperation == ITM_Stack) {
              break;
            }

            if(tamOperation == ITM_dddEL || tamOperation == ITM_dddIJ) {
              tamCurrentOperation = tamOperation;
              indexOfItems[getOperation()].func(NOPARAM);
              calcModeNormal();
              return;
            }

            strcpy(transitionSystemOperation, tamOperation==ITM_Max ? STD_UP_ARROW : (tamOperation==ITM_Min ? STD_DOWN_ARROW : (tamOperation==ITM_ADD ? "+" : (tamOperation==ITM_SUB ? "-" : (tamOperation==ITM_MULT ? STD_CROSS : (tamOperation==ITM_DIV ? "/" : (tamOperation==ITM_Config ? "CFG" : (tamOperation==ITM_Stack ? "S" : "???"))))))));
            sprintf(tamBuffer, "%s%s __", indexOfItems[getOperation()].itemCatalogName, transitionSystemOperation);

            tamCurrentOperation = tamOperation;

            if(tamCurrentOperation == ITM_Config || tamCurrentOperation == ITM_Stack) {
              break;
            }

            transitionSystemState = 1;
          }
          break;

        case TT_LETTER :
          if(tamMode != TM_VALUE && tamMode != TM_VALUE_CHB) {
            indexOfItems[getOperation()].func(tamLetteredRegister);
            calcModeNormal();
            return;
          }
          break;

        case TT_VARIABLE :
          if(tamMode != TM_VALUE && tamMode != TM_VALUE_CHB) {
          }
          break;

        case TT_DIGIT :
          tamNumber = tamDigit;
          if(tamNumber < tamNumberMin) {
            sprintf(tamBuffer, "%s %d_", indexOfItems[getOperation()].itemCatalogName, tamNumber);
            transitionSystemState = 2;
          }
          else if(tamNumber > tamNumberMax) {
          }
          else if(tamNumber*10 > tamNumberMax) {
            indexOfItems[getOperation()].func(tamNumber);
            calcModeNormal();
            return;
          }
          else {
            sprintf(tamBuffer, "%s %d_", indexOfItems[getOperation()].itemCatalogName, tamNumber);
            transitionSystemState = 2;
          }
          break;

        case TT_DOT :
          if(tamMode != TM_VALUE && tamMode != TM_VALUE_CHB) {
            if(((tamMode == TM_FLAGR || tamMode == TM_FLAGW) && numberOfLocalFlags > 0) || ((tamMode != TM_FLAGR && tamMode != TM_FLAGW) && allLocalRegisterPointer->numberOfLocalRegisters != 0)) {
              sprintf(tamBuffer, "%s .__", indexOfItems[getOperation()].itemCatalogName);
              transitionSystemState = 3;
            }
          }
          break;

        case TT_INDIRECT :
          sprintf(tamBuffer, "%s " STD_RIGHT_ARROW "__", indexOfItems[getOperation()].itemCatalogName);
          transitionSystemState = 5;
          break;

        case TT_BACKSPACE :
          calcModeNormal();
          return;
          break;

        case TT_BASE10 :
          indexOfItems[getOperation()].func(10);
          calcModeNormal();
          return;
          break;

        case TT_BASE16 :
          indexOfItems[getOperation()].func(16);
          calcModeNormal();
          return;
          break;

        default : {}
      }
      break;

    //////////////////////////////
    // OPo __
    case 1 : // RCL+, RCL-, RCL×, RCL/, RCL^, RCLv, STO+, STO-, STO×, STO/, STO^ or RCLv
      strcpy(transitionSystemOperation, tamOperation==ITM_Max ? STD_UP_ARROW : (tamOperation==ITM_Min ? STD_DOWN_ARROW : (tamOperation==ITM_ADD ? "+" : (tamOperation==ITM_SUB ? "-" : (tamOperation==ITM_MULT ? STD_CROSS : (tamOperation==ITM_DIV ? "/" : "???"))))));

      switch(tamTransition) {
        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s __   ", indexOfItems[getOperation()].itemCatalogName);
          transitionSystemState = 0;
          break;

        case TT_OPERATION :
          if(tamOperation==tamCurrentOperation) {
            sprintf(tamBuffer, "%s __   ", indexOfItems[getOperation()].itemCatalogName);
            transitionSystemState = 0;
          }
          else {
            tamCurrentOperation = tamOperation;
            strcpy(transitionSystemOperation, tamOperation==ITM_Max ? STD_UP_ARROW : (tamOperation==ITM_Min ? STD_DOWN_ARROW : (tamOperation==ITM_ADD ? "+" : (tamOperation==ITM_SUB ? "-" : (tamOperation==ITM_MULT ? STD_CROSS : (tamOperation==ITM_DIV ? "/" : "???"))))));
            sprintf(tamBuffer, "%s%s __", indexOfItems[getOperation()].itemCatalogName, transitionSystemOperation);
          }
          break;

        case TT_LETTER :
          indexOfItems[getOperation()].func(tamLetteredRegister);
          calcModeNormal();
          return;

        case TT_VARIABLE :
          break;

        case TT_DIGIT :
          tamNumber = tamDigit;
          sprintf(tamBuffer, "%s%s %d_", indexOfItems[getOperation()].itemCatalogName, transitionSystemOperation, tamNumber);
          transitionSystemState = 9;
          break;

        case TT_DOT :
          if(allLocalRegisterPointer->numberOfLocalRegisters != 0) {
            sprintf(tamBuffer, "%s%s .__", indexOfItems[getOperation()].itemCatalogName, transitionSystemOperation);
            transitionSystemState = 10;
          }
          break;

        case TT_INDIRECT :
          sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW "__", indexOfItems[getOperation()].itemCatalogName, transitionSystemOperation);
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
            indexOfItems[getOperation()].func(tamNumber*10 + tamDigit);
            calcModeNormal();
            return;
          }
          break;

        case TT_ENTER :
          if(tamNumberMin <= tamNumber && tamNumber <= tamNumberMax) {
            indexOfItems[getOperation()].func(tamNumber);
            calcModeNormal();
            return;
          }
          break;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s __", indexOfItems[getOperation()].itemCatalogName);
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
          if(((tamMode == TM_FLAGR || tamMode == TM_FLAGW) && tamNumber < numberOfLocalFlags) || ((tamMode != TM_FLAGR && tamMode != TM_FLAGW) && tamNumber < allLocalRegisterPointer->numberOfLocalRegisters)) {
            if(((tamMode == TM_FLAGR || tamMode == TM_FLAGW) && tamNumber*10 >= numberOfLocalFlags) || ((tamMode != TM_FLAGR && tamMode != TM_FLAGW) && tamNumber*10 >= allLocalRegisterPointer->numberOfLocalRegisters)) {
              indexOfItems[getOperation()].func(tamNumber + FIRST_LOCAL_REGISTER);
              calcModeNormal();
              return;
            }
            else {
              sprintf(tamBuffer, "%s .%d_", indexOfItems[getOperation()].itemCatalogName, tamNumber);
              transitionSystemState = 4;
            }
          }
          break;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s __ ", indexOfItems[getOperation()].itemCatalogName);
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
          if(((tamMode == TM_FLAGR || tamMode == TM_FLAGW) && tamNumber*10 + tamDigit < numberOfLocalFlags) || ((tamMode != TM_FLAGR && tamMode != TM_FLAGW) && tamNumber*10 + tamDigit < allLocalRegisterPointer->numberOfLocalRegisters)) {
            indexOfItems[getOperation()].func(tamNumber*10 + tamDigit + FIRST_LOCAL_REGISTER);
            calcModeNormal();
            return;
          }
          break;

        case TT_ENTER :
          indexOfItems[getOperation()].func(tamNumber + FIRST_LOCAL_REGISTER);
          calcModeNormal();
          return;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s .__", indexOfItems[getOperation()].itemCatalogName);
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
            indexOfItems[getOperation()].func(value);
          }
          calcModeNormal();
          return;
          break;

        case TT_VARIABLE :
          break;

        case TT_DIGIT :
          tamNumber = tamDigit;
          sprintf(tamBuffer, "%s " STD_RIGHT_ARROW "%d_", indexOfItems[getOperation()].itemCatalogName, tamNumber);
          transitionSystemState = 6;
          break;

        case TT_DOT :
          if(allLocalRegisterPointer->numberOfLocalRegisters != 0) {
            sprintf(tamBuffer, "%s " STD_RIGHT_ARROW ".__", indexOfItems[getOperation()].itemCatalogName);
            transitionSystemState = 7;
          }
          break;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s __ ", indexOfItems[getOperation()].itemCatalogName);
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
            indexOfItems[getOperation()].func(value);
          }
          calcModeNormal();
          return;

        case TT_ENTER :
          value = indirectAddressing(tamNumber, tamNumberMin, tamNumberMax);

          if(lastErrorCode == 0) {
            indexOfItems[getOperation()].func(value);
          }
          calcModeNormal();
          return;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s " STD_RIGHT_ARROW "__", indexOfItems[getOperation()].itemCatalogName);
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
          if(((tamMode == TM_FLAGR || tamMode == TM_FLAGW) && tamNumber < numberOfLocalFlags) || ((tamMode != TM_FLAGR && tamMode != TM_FLAGW) && tamNumber < allLocalRegisterPointer->numberOfLocalRegisters)) {
            if(((tamMode == TM_FLAGR || tamMode == TM_FLAGW) && tamNumber*10 >= numberOfLocalFlags) || ((tamMode != TM_FLAGR && tamMode != TM_FLAGW) && tamNumber*10 >= allLocalRegisterPointer->numberOfLocalRegisters)) {
              value = indirectAddressing(tamNumber + FIRST_LOCAL_REGISTER, tamNumberMin, tamNumberMax);

              if(lastErrorCode == 0) {
                indexOfItems[getOperation()].func(value);
              }
              calcModeNormal();
              return;
            }
            else {
              sprintf(tamBuffer, "%s " STD_RIGHT_ARROW ".%d_", indexOfItems[getOperation()].itemCatalogName, tamNumber);
              transitionSystemState = 8;
            }
          }
          break;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s " STD_RIGHT_ARROW "__ ", indexOfItems[getOperation()].itemCatalogName);
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
          if(((tamMode == TM_FLAGR || tamMode == TM_FLAGW) && tamNumber*10 + tamDigit < numberOfLocalFlags) || ((tamMode != TM_FLAGR && tamMode != TM_FLAGW) && tamNumber*10 + tamDigit < allLocalRegisterPointer->numberOfLocalRegisters)) {
            value = indirectAddressing(tamNumber*10 + tamDigit + FIRST_LOCAL_REGISTER, tamNumberMin, tamNumberMax);

            if(lastErrorCode == 0) {
              indexOfItems[getOperation()].func(value);
            }
            calcModeNormal();
            return;
          }
          break;

        case TT_ENTER :
          value = indirectAddressing(tamNumber + FIRST_LOCAL_REGISTER, tamNumberMin, tamNumberMax);

          if(lastErrorCode == 0) {
            indexOfItems[getOperation()].func(value);
          }
          calcModeNormal();
          return;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s " STD_RIGHT_ARROW ".__", indexOfItems[getOperation()].itemCatalogName);
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
          indexOfItems[getOperation()].func(tamNumber*10 + tamDigit);
          calcModeNormal();
          return;

        case TT_ENTER :
          indexOfItems[getOperation()].func(tamNumber);
          calcModeNormal();
          return;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s%s __", indexOfItems[getOperation()].itemCatalogName, transitionSystemOperation);
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
          if(tamNumber < allLocalRegisterPointer->numberOfLocalRegisters) {
            if(tamNumber > tamNumberMax) {
            }
            else if(tamNumber*10 >= allLocalRegisterPointer->numberOfLocalRegisters) {
              indexOfItems[getOperation()].func(tamNumber + FIRST_LOCAL_REGISTER);
              calcModeNormal();
              return;
            }
            else {
              sprintf(tamBuffer, "%s%s .%d_", indexOfItems[getOperation()].itemCatalogName, transitionSystemOperation, tamNumber);
              transitionSystemState = 11;
            }
          }
          break;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s%s __ ", indexOfItems[getOperation()].itemCatalogName, transitionSystemOperation);
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
          if(tamNumber*10 + tamDigit < allLocalRegisterPointer->numberOfLocalRegisters) {
            indexOfItems[getOperation()].func(tamNumber*10 + tamDigit + FIRST_LOCAL_REGISTER);
            calcModeNormal();
            return;
          }
          break;

        case TT_ENTER :
          if(tamNumber < allLocalRegisterPointer->numberOfLocalRegisters) {
            indexOfItems[getOperation()].func(tamNumber + FIRST_LOCAL_REGISTER);
            calcModeNormal();
            return;
          }
          break;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s%s .__", indexOfItems[getOperation()].itemCatalogName, transitionSystemOperation);
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
          regist = indirectAddressing(tamLetteredRegister, 0, FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters);

          if(lastErrorCode == 0) {
            indexOfItems[getOperation()].func(regist);
          }
          calcModeNormal();
          return;

        case TT_VARIABLE :
          break;

        case TT_DIGIT :
          tamNumber = tamDigit;
          sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW "%d_", indexOfItems[getOperation()].itemCatalogName, transitionSystemOperation, tamNumber);
          transitionSystemState = 13;
          break;

        case TT_DOT :
          if(allLocalRegisterPointer->numberOfLocalRegisters != 0) {
            sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW ".__", indexOfItems[getOperation()].itemCatalogName, transitionSystemOperation);
            transitionSystemState = 14;
          }
          break;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s%s __ ", indexOfItems[getOperation()].itemCatalogName, transitionSystemOperation);
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
          regist = indirectAddressing(tamNumber*10 + tamDigit, 0, FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters);

          if(lastErrorCode == 0) {
            indexOfItems[getOperation()].func(regist);
          }
          calcModeNormal();
          return;

        case TT_ENTER :
          regist = indirectAddressing(tamNumber, 0, FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters);

          if(lastErrorCode == 0) {
            indexOfItems[getOperation()].func(regist);
          }
          calcModeNormal();
          return;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW "__", indexOfItems[getOperation()].itemCatalogName, transitionSystemOperation);
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
          if(tamDigit < allLocalRegisterPointer->numberOfLocalRegisters) {
            tamNumber = tamDigit;
            sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW ".%d_", indexOfItems[getOperation()].itemCatalogName, transitionSystemOperation, tamNumber);
            transitionSystemState = 15;
          }
          break;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW "__ ", indexOfItems[getOperation()].itemCatalogName, transitionSystemOperation);
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
          if(tamNumber*10 + tamDigit < allLocalRegisterPointer->numberOfLocalRegisters) {
            regist = indirectAddressing(tamNumber*10 + tamDigit + FIRST_LOCAL_REGISTER, 0, FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters);

            if(lastErrorCode == 0) {
              indexOfItems[getOperation()].func(regist);
            }
            calcModeNormal();
            return;
          }
          break;

        case TT_ENTER :
          regist = indirectAddressing(tamNumber + FIRST_LOCAL_REGISTER, 0, FIRST_LOCAL_REGISTER + allLocalRegisterPointer->numberOfLocalRegisters);

          if(lastErrorCode == 0) {
            indexOfItems[getOperation()].func(regist);
          }
          calcModeNormal();
          return;

        case TT_BACKSPACE :
          sprintf(tamBuffer, "%s%s " STD_RIGHT_ARROW ".__", indexOfItems[getOperation()].itemCatalogName, transitionSystemOperation);
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
    clearRegisterLine(TAM_REGISTER_LINE, false, false);
  }
  showString(tamBuffer, &standardFont, 25, Y_POSITION_OF_TAM_LINE + 6, vmNormal, true, true);
}



void closeNim(void) {
  if(nimNumberPart == NP_INT_10) {                //JM Input default type vv
    switch (Input_Default) {
    case ID_43S:                                  //   Do nothing, this is default LI/DP
    case ID_LI:                                   //   Do nothing, because default is LI/DP 
      break;
    case ID_DP:                                   //   Do Real default for DP
    case ID_CPXDP:                                //                       CPX
      nimNumberPart = NP_REAL_FLOAT_PART;
      break;
    case ID_SI:                                   //   lastIntegerBase is set in fnInDefault; I do not set it here, as the user can change it of course.
      break;
    }
  }                                               //JM ^^
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
        nimBuffer[++lastChar] = '0';                    //JMCPX. CHANGED FROM "1" to '0'. PLEASE CONSIDER KEEPING IT '0'. IT MAKES NO SENSE DEFAULTING TO 0+1xi IF ABORTING CC ENTRY.
        nimBuffer[lastChar + 1] = 0;
      }

      if((nimBuffer[0] != '-' || nimBuffer[1] != 0) && (nimBuffer[lastChar] != '-')) { // The buffer is not just the minus sign AND The last char of the buffer is not the minus sign
        calcModeNormal();

        if(nimNumberPart == NP_INT_10) {
          longInteger_t lgInt;

          longIntegerInit(lgInt);
          stringToLongInteger(nimBuffer + (nimBuffer[0] == '+' ? 1 : 0), 10, lgInt);
          convertLongIntegerToLongIntegerRegister(lgInt, REGISTER_X);
          longIntegerFree(lgInt);
        }
        else if(nimNumberPart == NP_INT_BASE) {
          //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          // Any change in this part must be reported in the function strToShortInteger from file testSuite.c after the line: else if(nimNumberPart == NP_INT_BASE) {
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
              displayCalcErrorMessage(ERROR_INVALID_INTEGER_INPUT, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                showInfoDialog("In function closeNIM:", "there is a non numeric character in the base of the integer!", NULL, NULL);
              #endif
              return;
            }
          }

          base = atoi(nimBuffer + posHash + 1);
          if(base < 2 || base > 16) {
            displayCalcErrorMessage(ERROR_INVALID_INTEGER_INPUT, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
             showInfoDialog("In function closeNIM:", "the base of the integer must be from 2 to 16!", NULL, NULL);
            #endif
            return;
          }

          for(i=nimBuffer[0] == '-' ? 1 : 0; i<posHash; i++) {
            if((nimBuffer[i] > '9' ? nimBuffer[i] - 'A' + 10 : nimBuffer[i] - '0') >= base) {
              displayCalcErrorMessage(ERROR_INVALID_INTEGER_INPUT, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
              #if (EXTRA_INFO_ON_CALC_ERROR == 1)
                sprintf(errorMessage, "digit %c is not allowed in base %d!", nimBuffer[i], base);
                showInfoDialog("In function closeNIM:", errorMessage, NULL, NULL);
              #endif

              restoreStack();
              return;
            }
          }

          longIntegerInit(value);
          nimBuffer[posHash] = 0;
          stringToLongInteger(nimBuffer + (nimBuffer[0] == '+' ? 1 : 0), base, value);

          // maxVal = 2^shortIntegerWordSize
          longIntegerInit(maxVal);
          if(shortIntegerWordSize >= 1 && shortIntegerWordSize <= 64) {
            longInteger2Pow(shortIntegerWordSize, maxVal);
          }
          else {
            sprintf(errorMessage, "In function closeNIM: %d is an unexpected value for shortIntegerWordSize!", shortIntegerWordSize);
            displayBugScreen(errorMessage);
            longIntegerFree(maxVal);
            longIntegerFree(value);
            return;
          }

          // minVal = -maxVal/2
          longIntegerInit(minVal);
          longIntegerSetZero(minVal); // Mandatory! Else segmentation fault at next instruction
          longIntegerDivideUInt(maxVal, 2, minVal); // minVal = maxVal / 2
          longIntegerSetNegativeSign(minVal); // minVal = -minVal

          if(shortIntegerMode != SIM_UNSIGN) {
            longIntegerDivideUInt(maxVal, 2, maxVal); // maxVal /= 2
          }

          longIntegerSubtractUInt(maxVal, 1, maxVal); // maxVal--

          if(shortIntegerMode == SIM_UNSIGN) {
            longIntegerSetZero(minVal); // minVal = 0
          }

          if(shortIntegerMode == SIM_1COMPL || shortIntegerMode == SIM_SIGNMT) {
            longIntegerAddUInt(minVal, 1, minVal); // minVal++
          }

          if(longIntegerCompare(value, minVal) < 0 || longIntegerCompare(value, maxVal) > 0) {
            displayCalcErrorMessage(ERROR_WORD_SIZE_TOO_SMALL, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
              char strMin[22], strMax[22];
              longIntegerToAllocatedString(minVal, strMin, sizeof(strMin));
              longIntegerToAllocatedString(maxVal, strMax, sizeof(strMax));
              sprintf(errorMessage, "For word size of %d bit%s and integer mode %s,", shortIntegerWordSize, shortIntegerWordSize>1 ? "s" : "", getShortIntegerModeName(shortIntegerMode));
              sprintf(errorMessage + ERROR_MESSAGE_LENGTH/2, "the entered number must be from %s to %s!", strMin, strMax);
              showInfoDialog("In function closeNIM:", errorMessage, errorMessage + ERROR_MESSAGE_LENGTH/2, NULL);
            #endif
            longIntegerFree(maxVal);
            longIntegerFree(minVal);
            longIntegerFree(value);
            return;
          }

          reallocateRegister(REGISTER_X, dtShortInteger, SHORT_INTEGER_SIZE, base);

          char strValue[22];
          longIntegerToAllocatedString(value, strValue, sizeof(strValue));

          uint64_t val = strtoull(strValue + (longIntegerIsNegative(value) ? 1 : 0), NULL, 10); // when value is negative: discard the minus sign

          if(shortIntegerMode == SIM_UNSIGN) {
          }
          else if(shortIntegerMode == SIM_2COMPL) {
            if(longIntegerIsNegative(value)) {
              val = (~val + 1) & shortIntegerMask;
            }
          }
          else if(shortIntegerMode == SIM_1COMPL) {
            if(longIntegerIsNegative(value)) {
              val = ~val & shortIntegerMask;
            }
          }
          else if(shortIntegerMode == SIM_SIGNMT) {
            if(longIntegerIsNegative(value)) {
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

          longIntegerFree(maxVal);
          longIntegerFree(minVal);
          longIntegerFree(value);
        }
        else if(nimNumberPart == NP_REAL_FLOAT_PART || nimNumberPart == NP_REAL_EXPONENT) {
          
            if(Input_Default == ID_CPXDP) {                                         //JM Input default type
              reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE); //JM Input default type
              stringToReal34(nimBuffer, REGISTER_REAL34_DATA(REGISTER_X));          //JM Input default type
              stringToReal34("0", REGISTER_IMAG34_DATA(REGISTER_X));                //JM Input default type
            }                                                                       //JM Input default type
            else {                                                                  //JM Input default type
            reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
            stringToReal34(nimBuffer, REGISTER_REAL34_DATA(REGISTER_X));
            }                                                                       //JM Input default type

        }
        else if(nimNumberPart == NP_FRACTION_DENOMINATOR) {
          int16_t i, posSpace, posSlash, lg;
          int32_t integer, numer, denom;
          real34_t temp;

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
              displayCalcErrorMessage(ERROR_BAD_INPUT, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
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
             displayCalcErrorMessage(ERROR_BAD_INPUT, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
             #if (EXTRA_INFO_ON_CALC_ERROR == 1)
               showInfoDialog("In function parseNimString:", "there is a non numeric character in the numerator part of the fraction!", NULL, NULL);
             #endif
             return;
            }
          }

          for(i=posSlash+1; i<lg; i++) {
            if(nimBuffer[i]<'0' || nimBuffer[i]>'9') {
              displayCalcErrorMessage(ERROR_BAD_INPUT, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
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

          if(denom == 0 && !getSystemFlag(FLAG_SPCRES)) {
            displayCalcErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, NIM_REGISTER_LINE);
            #if (EXTRA_INFO_ON_CALC_ERROR == 1)
              showInfoDialog("In function parseNimString:", "the denominator of the fraction should not be 0!", "Unless D flag (Danger) is set.", NULL);
            #endif
            return;
          }

          reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
          int32ToReal34(numer, REGISTER_REAL34_DATA(REGISTER_X));
          int32ToReal34(denom, &temp);
          real34Divide(REGISTER_REAL34_DATA(REGISTER_X), &temp, REGISTER_REAL34_DATA(REGISTER_X));
          int32ToReal34(integer, &temp);
          real34Add(REGISTER_REAL34_DATA(REGISTER_X), &temp, REGISTER_REAL34_DATA(REGISTER_X));
          if(nimBuffer[0] == '-') {
            real34SetNegativeSign(REGISTER_REAL34_DATA(REGISTER_X));
          }

          if(!getSystemFlag(FLAG_FRACT)) {
            if(integer == 0) {
              clearSystemFlag(FLAG_PROPFR); // d/c
            }
            else {
              setSystemFlag(FLAG_PROPFR); // a b/c
            }
          }
        }
        else if(nimNumberPart == NP_COMPLEX_INT_PART || nimNumberPart == NP_COMPLEX_FLOAT_PART || nimNumberPart == NP_COMPLEX_EXPONENT) {
          int16_t imaginarySign;

          if(nimBuffer[imaginaryMantissaSignLocation] == '+') {
            imaginarySign = 1;
          }
          else {
            imaginarySign = -1;
          }
          nimBuffer[imaginaryMantissaSignLocation] = 0;

          reallocateRegister(REGISTER_X, dtComplex34, COMPLEX34_SIZE, AM_NONE);
          stringToReal34(nimBuffer, REGISTER_REAL34_DATA(REGISTER_X));

          stringToReal34(nimBuffer + imaginaryMantissaSignLocation + 2, REGISTER_IMAG34_DATA(REGISTER_X));
          if(imaginarySign == -1) {
            real34SetNegativeSign(REGISTER_IMAG34_DATA(REGISTER_X));
          }

          if(getSystemFlag(FLAG_POLAR)) { // polar mode
            if(real34CompareEqual(REGISTER_REAL34_DATA(REGISTER_X), const34_0)) {
              real34Zero(REGISTER_IMAG34_DATA(REGISTER_X));
            }
            else {
              real_t magnitude, theta;

              real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &magnitude);
              real34ToReal(REGISTER_IMAG34_DATA(REGISTER_X), &theta);
              convertAngleFromTo(&theta, currentAngularMode, AM_RADIAN, &ctxtReal39);
              if(realCompareLessThan(&magnitude, const_0)) {
                realSetPositiveSign(&magnitude);
                realAdd(&theta, const_pi, &theta, &ctxtReal39);
                WP34S_Mod(&theta, const1071_2pi, &theta, &ctxtReal39);
              }
              realPolarToRectangular(&magnitude, &theta, &magnitude, &theta, &ctxtReal39); // theta in radian
              realToReal34(&magnitude, REGISTER_REAL34_DATA(REGISTER_X));
              realToReal34(&theta,     REGISTER_IMAG34_DATA(REGISTER_X));
            }
          }
          fnSetFlag(FLAG_CPXRES);
    
//        fnRefreshRadioState(RB_BCR, true);                                    //dr
          fnRefreshComboxState(CB_JC, JC_BCR, true);                            //dr
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
