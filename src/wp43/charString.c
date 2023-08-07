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
 * \file charString.c
 ***********************************************/

#include "charString.h"

#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "fonts.h"

#include "wp43.h"




static void _calculateStringWidth(const char *str, const font_t *font, bool_t withLeadingEmptyRows, bool_t withEndingEmptyRows, int16_t *width, const char **resultStr) {
  int16_t ch, numPixels, charCode, glyphId;
  const glyph_t *glyph;
  bool_t  firstChar;
  #if defined(GENERATE_CATALOGS)
    uint8_t doubling = false;
  #else //GENERATE_CATALOGS
    uint8_t doubling = (font == &numericFont && temporaryInformation == TI_NO_INFO && checkHP) ? DOUBLING : 4u;
  #endif //GENERATE_CATALOGS

//  const font_t  *font;  //JM

  glyph = NULL;
  firstChar = true;
  numPixels = 0;
  ch = 0;
  while(str[ch] != 0) {
    charCode = (uint8_t)str[ch++];
    if(charCode & 0x80) { // MSB set
      charCode = (charCode<<8) | (uint8_t)str[ch++];
    }

/*
    font = font1;                             //JM auto font change for enlarged alpha fonts vv
    #if !defined(TESTSUITE_BUILD)
      if(combinationFonts == 2) {
        if(maxiC == 1 && font == &numericFont) {
    #endif // !TESTSUITE_BUILD
    glyphId = findGlyph(font, charCode);
    if(glyphId < 0) {                         //JM if there is not a large glyph, width of the small letter
      font = &standardFont;
    }
    #if !defined(TESTSUITE_BUILD)
        }
      }                                       //JM ^^
    #endif // !TESTSUITE_BUILD
*/
    if(charCode != 1u) {                          //If the special ASCII 01, then skip the width and font not found portions
      glyphId = findGlyph(font, charCode);
      if(glyphId >= 0) {
        glyph = (font->glyphs) + glyphId;
      }
      else if(glyphId == -1) {
        generateNotFoundGlyph(-1, charCode);
        glyph = &glyphNotFound;
      }
      else if(glyphId == -2) {
        generateNotFoundGlyph(-2, charCode);
        glyph = &glyphNotFound;
      }
      else {
        glyph = NULL;
      }

    if(glyph == NULL) {
      #if defined(GENERATE_CATALOGS)
        printf("\n---------------------------------------------------------------------------\n"
                 "In function stringWidth: %d is an unexpected value returned by findGlyph!"
               "/n---------------------------------------------------------------------------\n", glyphId);
      #else // !GENERATE_CATALOGS
        sprintf(errorMessage, commonBugScreenMessages[bugMsgValueReturnedByFindGlyph], "stringWidth", glyphId);
        displayBugScreen(errorMessage);
      #endif // GENERATE_CATALOGS
      *width = 0;
      return;
    }

      numPixels += (doubling*(glyph->colsGlyph + glyph->colsAfterGlyph)) >> 2;
      if(firstChar) {
        firstChar = false;
        if(withLeadingEmptyRows) {
          numPixels += (doubling*(glyph->colsBeforeGlyph)) >> 2;
        }
      }
      else {
        numPixels += (doubling*(glyph->colsBeforeGlyph)) >> 2;
      }

      if(resultStr != NULL) { // for stringAfterPixels
        if(numPixels > *width) {
          break;
        }
        else {
          *resultStr = str + ch;
        }
      }
    }
  }

  if(glyph != NULL && withEndingEmptyRows == false) {
    numPixels -= doubling*(glyph->colsAfterGlyph);
    if(resultStr != NULL && numPixels <= *width) { // for stringAfterPixels
      if((**resultStr) & 0x80) {
        *resultStr += 2;
      }
      else if((**resultStr) != 0) {
        *resultStr += 1;
      }
    }
  }
  *width = numPixels;
  return;
}


int16_t stringWidth(const char *str, const font_t *font, bool_t withLeadingEmptyRows, bool_t withEndingEmptyRows) {
  int16_t width = 0;
  _calculateStringWidth(str, font, withLeadingEmptyRows, withEndingEmptyRows, &width, NULL);
  return width;
}


char *stringAfterPixels(const char *str, const font_t *font, int16_t width, bool_t withLeadingEmptyRows, bool_t withEndingEmptyRows) {
  const char *resultStr = str;
  _calculateStringWidth(str, font, withLeadingEmptyRows, withEndingEmptyRows, &width, &resultStr);
  return (char *)resultStr;
}


int16_t stringNextGlyphNoEndCheck_JM(const char *str, int16_t pos) {    //Not checking for beyond terminator. Use only if no risk for pos > length(str)
int16_t posinc = 0;
  if(str[pos] == 0) return pos;

  if(str[pos] & 0x80) {
    posinc = 2;
    if(str[pos+1] == 0 || str[pos+2] == 0) return pos+1;
  }
  else {
    posinc = 1;
    if(str[pos+1] == 0) return pos+1;
  }

  pos += posinc;
  return pos;
}


int16_t stringNextGlyph(const char *str, int16_t pos) {
  int16_t lg;

  lg = stringByteLength(str);
  if(pos >= lg) {
    return lg;
  }

  pos += (str[pos] & 0x80) ? 2 : 1;

  if(pos >= lg) {
    return lg;
  }
  else {
   return pos;
  }
}


int16_t stringPrevGlyph(const char *str, int16_t pos) {
  int16_t prev = 0;
  int16_t lg;

  lg = stringByteLength(str);
  if(pos >= lg) {
    pos = lg;
  }

  if(pos <= 1) {
    return 0;
  }
  else {
    int16_t cnt = 0;
    while(str[cnt] != 0 && cnt < pos) {
      prev = cnt;
      cnt = stringNextGlyph(str, cnt);
    }
  }
  return prev;
}


int16_t stringLastGlyph(const char *str) {
  int16_t lastGlyph;

  if(str == NULL) {
    lastGlyph = -1;
  }
  else {
    int16_t lg = stringByteLength(str), next = 0;
    for(lastGlyph=0;;) {
      if(lastGlyph >= lg) {
        next = lg;
      }
      else {
        next += (str[lastGlyph] & 0x80) ? 2 : 1;

        if(next > lg) {
          next = lg;
        }
      }

      if(next == lg) {
        break;
      }

      lastGlyph = next;
    }
  }
  return lastGlyph;
}


int32_t stringByteLength(const char *str) {
  int32_t len = 0;

  while(*str != 0) {
    if(*str & 0x80) {
      str += 2;
      len += 2;
    }
    else {
      str++;
      len++;
    }
  }
  return len;
}


int32_t stringGlyphLength(const char *str) {
  int32_t len = 0;

  while(*str != 0) {
    if(*str & 0x80) {
      str += 2;
      len++;
    }
    else {
      str++;
      len++;
    }
  }
  return len;
}


void codePointToUtf8(uint32_t codePoint, uint8_t *utf8) { // WP43 supports only unicode code points from 0x0000 to 0x7FFF
  if(codePoint <= 0x00007F) {
    utf8[0] = codePoint;
    utf8[1] = 0;
    utf8[2] = 0;
    utf8[3] = 0;
    utf8[4] = 0;
  }

  else if(codePoint <= 0x0007FF) {
    utf8[0] = 0xC0 | (codePoint >> 6);
    utf8[1] = 0x80 | (codePoint &  0x3F);
    utf8[2] = 0;
    utf8[3] = 0;
    utf8[4] = 0;
  }

  else /*if(codePoint <= 0x00FFFF)*/ {
    utf8[0] = 0xE0 |  (codePoint >> 12);
    utf8[1] = 0x80 | ((codePoint >>  6) & 0x3F);
    utf8[2] = 0x80 | ((codePoint      ) & 0x3F);
    utf8[3] = 0;
    utf8[4] = 0;
  }

  /*else if(codePoint <= 0x1FFFFF) {
    utf8[0] = 0xF0 |  (codePoint >> 18);
    utf8[1] = 0x80 | ((codePoint >> 12) & 0x3F);
    utf8[2] = 0x80 | ((codePoint >>  6) & 0x3F);
    utf8[3] = 0x80 | ((codePoint      ) & 0x3F);
    utf8[4] = 0;
  }

  else if(codePoint <= 0x3FFFFFF) {
    utf8[0] = 0xF8 |  (codePoint >> 24);
    utf8[1] = 0x80 | ((codePoint >> 18) & 0x3F);
    utf8[2] = 0x80 | ((codePoint >> 12) & 0x3F);
    utf8[3] = 0x80 | ((codePoint >>  6) & 0x3F);
    utf8[4] = 0x80 | ((codePoint      ) & 0x3F);
    utf8[5] = 0;
  }

  else if(codePoint <= 0x7FFFFFFF) {
    utf8[0] = 0xFC |  (codePoint >> 30);
    utf8[1] = 0x80 | ((codePoint >> 24) & 0x3F);
    utf8[2] = 0x80 | ((codePoint >> 18) & 0x3F);
    utf8[3] = 0x80 | ((codePoint >> 12) & 0x3F);
    utf8[4] = 0x80 | ((codePoint >>  6) & 0x3F);
    utf8[5] = 0x80 | ((codePoint      ) & 0x3F);
    utf8[6] = 0;
  }*/
}


uint32_t utf8ToCodePoint(const uint8_t *utf8, uint32_t *codePoint) { // WP43 supports only unicode code points from 0x0000 to 0x7FFF
  if((*utf8 & 0x80) == 0) {
    *codePoint = *utf8;
    return 1;
  }

  else if((*utf8 & 0xE0) == 0xC0) {
    *codePoint =  (*utf8       & 0x1F) << 6;
    *codePoint |= (*(utf8 + 1) & 0x3F);
    return 2;
  }

  else /*if((*utf8 & 0xF0) == 0xE0)*/ {
    *codePoint =  (*utf8       & 0x0F) << 12;
    *codePoint |= (*(utf8 + 1) & 0x3F) <<  6;
    *codePoint |= (*(utf8 + 2) & 0x3F);
    return 3;
  }

  /*else if((*utf8 & 0xF8) == 0xF0) {
    *codePoint =  (*utf8       & 0x07) << 18;
    *codePoint |= (*(utf8 + 1) & 0x3F) << 12;
    *codePoint |= (*(utf8 + 2) & 0x3F) <<  6;
    *codePoint |= (*(utf8 + 3) & 0x3F);
    return 4;
  }

  else if((*utf8 & 0xFC) == 0xF8) {
    *codePoint =  (*utf8       & 0x03) << 24;
    *codePoint |= (*(utf8 + 1) & 0x3F) << 18;
    *codePoint |= (*(utf8 + 2) & 0x3F) << 12;
    *codePoint |= (*(utf8 + 3) & 0x3F) <<  6;
    *codePoint |= (*(utf8 + 4) & 0x3F);
    return 5;
  }

  else if((*utf8 & 0xFE) == 0xFC) {
    *codePoint =  (*utf8       & 0x01) << 30;
    *codePoint |= (*(utf8 + 1) & 0x3F) << 24;
    *codePoint |= (*(utf8 + 2) & 0x3F) << 18;
    *codePoint |= (*(utf8 + 3) & 0x3F) << 12;
    *codePoint |= (*(utf8 + 4) & 0x3F) <<  6;
    *codePoint |= (*(utf8 + 5) & 0x3F);
    return 6;
  }*/

  return 0;
}


void stringToUtf8(const char *str, uint8_t *utf8) {
  int16_t len;

  len = stringGlyphLength(str);

  if(len == 0) {
    *utf8 = 0;
    return;
  }

  for(int16_t i=0; i<len; i++) {
    if(*str & 0x80) {
      codePointToUtf8(((uint8_t)*str & 0x7F) * 256u + (uint8_t)str[1], utf8);
      str += 2;
      while(*utf8) {
        utf8++;
      }
    }
    else {
      *utf8 = *str;
      utf8++;
      str++;
      *utf8 = 0;
    }
  }
}


void utf8ToString(const uint8_t *utf8, char *str) {
  uint32_t codePoint;

  while(*utf8) {
    utf8 += utf8ToCodePoint(utf8, &codePoint);
    if(codePoint < 0x0080) {
      *(str++) = codePoint;
    }
    else {
      codePoint |= 0x8000;
      *(str++) = codePoint >> 8;
      *(str++) = codePoint & 0x00FF;
    }
  }
  *str = 0;
}


typedef struct {
  char     *item_in;            ///<
  char     *item_out;           ///<
} function_t2;

TO_QSPI const function_t2 indexOfStrings2[] = {
              //number                  replacement string
//XPORTP CODE 2023-07-15
              {STD_NOT,                       "<>"},
              {STD_DEGREE,                    "deg"},
              {STD_PLUS_MINUS,                "+-"},
              {STD_A_GRAVE,                   "`A"},
              {STD_CROSS,                     "*"},
              {STD_U_GRAVE,                   "`U"},
              {STD_a_GRAVE,                   "`a"},
              {STD_DIVIDE,                    "/"},
              {STD_u_GRAVE,                   "`u"},
              {STD_A_BREVE,                   "`A"},
              {STD_a_BREVE,                   "`a"},
              {STD_E_MACRON,                  "`E"},
              {STD_e_MACRON,                  "`e"},
              {STD_I_MACRON,                  "`I"},
              {STD_i_MACRON,                  "`i"},
              {STD_I_BREVE,                   "`I"},
              {STD_i_BREVE,                   "`i"},
              {STD_U_BREVE,                   "`U"},
              {STD_u_BREVE,                   "`u"},
              {STD_Y_CIRC,                    "`Y"},
              {STD_y_CIRC,                    "`y"},
              {STD_y_BAR,                     "y_mean"},
              {STD_x_BAR,                     "x_mean"},
              {STD_x_CIRC,                    "x_circ"},
              {STD_ALPHA,                     "Alpha"},
              {STD_BETA,                      "Beta"},
              {STD_GAMMA,                     "Gamma"},
              {STD_DELTA,                     "Delta"},
              {STD_EPSILON,                   "Epsilon"},
              {STD_ZETA,                      "Zeta"},
              {STD_PI,                        "Pi"},
              {STD_SIGMA,                     "Sigma"},
              {STD_PHI,                       "Phi"},
              {STD_CHI,                       "Chi"},
              {STD_PSI,                       "Psi"},
              {STD_alpha,                     "alpha"},
              {STD_beta,                      "beta"},
              {STD_gamma,                     "gamma"},
              {STD_delta,                     "delta"},
              {STD_epsilon,                   "epsilon"},
              {STD_zeta,                      "zeta"},
              {STD_lambda,                    "lambda"},
              {STD_mu,                        "mu"},
              {STD_pi,                        "pi"},
              {STD_sigma,                     "sigma"},
              {STD_phi,                       "phi"},
              {STD_chi,                       "chi"},
              {STD_psi,                       "psi"},
              {STD_omega,                     "omega"},
              {STD_ELLIPSIS,                  "…"},
              {STD_BINARY_ONE,                "1"},
              {STD_SUP_MINUS_1,               "`-1"},
              {STD_SUB_E_OUTLINE,             "`E"},
              {STD_SUB_PLUS,                  "`+"},
              {STD_SUB_MINUS,                 "`-"},
              {STD_SUP_ASTERISK,              "`*"},
              {STD_SUP_INFINITY,              "inf"},
              {STD_SUB_INFINITY,              "inf"},
              {STD_PLANCK,                    "Planck"},
              {STD_PLANCK_2PI,                "Planck2pi"},
              {STD_SUP_PLUS,                  "+"},
              {STD_SUP_MINUS,                 "-"},
              {STD_BINARY_ZERO,               "0"},
              {STD_INFINITY,                  "inf"},
              {STD_MEASURED_ANGLE,            "angle"},
              {STD_INTEGRAL,                  "integral"},
              {STD_ALMOST_EQUAL,              "approx"},
              {STD_NOT_EQUAL,                 "<>"},
              {STD_LESS_EQUAL,                "<="},
              {STD_GREATER_EQUAL,             ">="},
              {STD_SUB_EARTH,                 "`earth"},
              {STD_SUB_alpha,                 "`alpha"},
              {STD_SUB_delta,                 "`delta"},
              {STD_SUB_mu,                    "`mu"},
              {STD_SUB_SUN,                   "`sun"},
              {STD_PRINTER,                   "Print"},
              {STD_UK,                        "UK"},
              {STD_US,                        "US"},
              {STD_GAUSS_BLACK_L,             "GAUSS_BL_L "},
              {STD_GAUSS_WHITE_R,             "GAUSS_WH_R "},
              {STD_GAUSS_WHITE_L,             "GAUSS_WH_L "},
              {STD_GAUSS_BLACK_R,             "GAUSS_BL_R "},
              {STD_SUB_10,                    "10^"},
              {STD_EulerE,                    "e"},
};


  bool_t _getText(uint8_t a1, uint8_t a2, char *str) {
    //printf("_getText %u %u : ",(uint8_t)a1,(uint8_t)a2);
    str[0] = 0;
    uint_fast16_t n = nbrOfElements(indexOfStrings2);
    for(uint_fast16_t i=0; i<n; i++) {
      if((uint8_t)a1 == (uint8_t)(indexOfStrings2[i].item_in[0]) && (uint8_t)a2 == (uint8_t)(indexOfStrings2[i].item_in[1])) {
        //printf("(%u):%u %u %s\n", i,(uint8_t)(indexOfStrings2[i].item_in[0]), (uint8_t)(indexOfStrings2[i].item_in[1]),indexOfStrings2[i].item_out);
        strcpy(str, indexOfStrings2[i].item_out);
        break;
      }
    }
    return str[0] != 0;
  }



void stringToASCII(const char *str, char *ascii) {
  int16_t len;
  uint8_t a1, a2;
  char aa[32];

  len = stringGlyphLength(str);

  if(len == 0) {
    *ascii = 0;
    return;
  }

  for(int16_t i=0; i<len; i++) {  // WP43 supports only unicode code points from 0x0000 to 0x7FFF
    if(*str & 0x80) { 
      a1=(uint8_t)*str;
      str++;
      a2=(uint8_t)*str;
      str++;

      //replacement table
      if(_getText(a1, a2, aa)) {        
        int16_t j = 0;
        while (aa[j] != 0) {
          *ascii = aa[j++];
          ascii++;
        }
        ascii--;
      } else
      //arrows
      if(a1==(uint8_t)(STD_RIGHT_DOUBLE_ARROW[0]) && a2==(uint8_t)(STD_RIGHT_DOUBLE_ARROW[1])) {
        *ascii = '>'; ascii++;
        *ascii = '>'; //to change to >>        
      } else
      if((a1==(uint8_t)(STD_RIGHT_DASHARROW[0]) && a2==(uint8_t)(STD_RIGHT_DASHARROW[1])) ||
         (a1==(uint8_t)(STD_RIGHT_ARROW[0]) && a2==(uint8_t)(STD_RIGHT_ARROW[1])) ||
         (a1==(uint8_t)(STD_RIGHT_SHORT_ARROW[0]) && a2==(uint8_t)(STD_RIGHT_SHORT_ARROW[1]))
        ) {
        *ascii = '-'; ascii++;
        *ascii = '>'; //to change to ->
      } else
      if((a1==(uint8_t)(STD_LEFT_DASHARROW[0]) && a2==(uint8_t)(STD_LEFT_DASHARROW[1])) ||
         (a1==(uint8_t)(STD_LEFT_ARROW[0]) && a2==(uint8_t)(STD_LEFT_ARROW[1]))
        ) {
        *ascii = '<'; ascii++;
        *ascii = '-'; //to change to ->
      } else
      if((a1==(uint8_t)(STD_UP_DASHARROW[0]) && a2==(uint8_t)(STD_UP_DASHARROW[1])) ||
         (a1==(uint8_t)(STD_UP_ARROW[0]) && a2==(uint8_t)(STD_UP_ARROW[1])) ||
         (a1==(uint8_t)(STD_HOLLOW_UP_ARROW[0]) && a2==(uint8_t)(STD_HOLLOW_UP_ARROW[1])) ) {
        *ascii = '^'; 
      } else
      if((a1==(uint8_t)(STD_DOWN_DASHARROW[0]) && a2==(uint8_t)(STD_DOWN_DASHARROW[1])) ||
         (a1==(uint8_t)(STD_DOWN_ARROW[0]) && a2==(uint8_t)(STD_DOWN_ARROW[1])) ||
         (a1==(uint8_t)(STD_HOLLOW_DOWN_ARROW[0]) && a2==(uint8_t)(STD_HOLLOW_DOWN_ARROW[1])) ) {
        *ascii = 'v';
      } else
     if(a1==(uint8_t)(STD_LEFT_RIGHT_ARROWS[0]) && a2==(uint8_t)(STD_LEFT_RIGHT_ARROWS[1])) {
        *ascii = '>'; //to change to ><
        ascii++;
        *ascii = '<'; //to change to ><
      } else
      //diverse
      if(a1==(uint8_t)(STD_op_i              [0]) && a2==(uint8_t)(STD_op_i              [1])) *ascii = 'i'; else
      if(a1==(uint8_t)(STD_op_j              [0]) && a2==(uint8_t)(STD_op_j              [1])) *ascii = 'j'; else
      if(a1==(uint8_t)(STD_BINARY_ONE        [0]) && a2==(uint8_t)(STD_BINARY_ONE        [1])) *ascii = '1'; else
      if(a1==(uint8_t)(STD_BINARY_ZERO       [0]) && a2==(uint8_t)(STD_BINARY_ZERO       [1])) *ascii = '0'; else
      //seps
      if(a1==(uint8_t)(STD_RIGHT_TACK        [0]) && a2==(uint8_t)(STD_RIGHT_TACK        [1])) *ascii = '\''; else
      if(a1==(uint8_t)(STD_WDOT              [0]) && a2==(uint8_t)(STD_WDOT              [1])) *ascii = '.'; else
      if(a1==(uint8_t)(STD_DOT               [0]) && a2==(uint8_t)(STD_DOT               [1])) *ascii = '.'; else
      if(a1==(uint8_t)(STD_WPERIOD           [0]) && a2==(uint8_t)(STD_WPERIOD           [1])) *ascii = '.'; else
      if(a1==(uint8_t)(STD_WCOMMA            [0]) && a2==(uint8_t)(STD_WCOMMA            [1])) *ascii = ','; else
      if(a1==(uint8_t)(STD_NQUOTE            [0]) && a2==(uint8_t)(STD_NQUOTE            [1])) *ascii = '\''; else
      if(a1==(uint8_t)(STD_INV_BRIDGE        [0]) && a2==(uint8_t)(STD_INV_BRIDGE        [1])) *ascii = ','; else
      if(a1==(uint8_t)(STD_SPACE_EM          [0]) && a2==(uint8_t)(STD_SPACE_EM          [1])) *ascii = ' '; else
      if(a1==(uint8_t)(STD_SPACE_3_PER_EM    [0]) && a2==(uint8_t)(STD_SPACE_3_PER_EM    [1])) *ascii = ' '; else
      if(a1==(uint8_t)(STD_SPACE_4_PER_EM    [0]) && a2==(uint8_t)(STD_SPACE_4_PER_EM    [1])) *ascii = ' '; else
      if(a1==(uint8_t)(STD_SPACE_6_PER_EM    [0]) && a2==(uint8_t)(STD_SPACE_6_PER_EM    [1])) *ascii = ' '; else
      if(a1==(uint8_t)(STD_SPACE_FIGURE      [0]) && a2==(uint8_t)(STD_SPACE_FIGURE      [1])) *ascii = ' '; else
      if(a1==(uint8_t)(STD_SPACE_PUNCTUATION [0]) && a2==(uint8_t)(STD_SPACE_PUNCTUATION [1])) *ascii = ' '; else
      if(a1==(uint8_t)(STD_SPACE_HAIR        [0]) && a2==(uint8_t)(STD_SPACE_HAIR        [1])) *ascii = ' '; else
      //RANGE SUP/SUB/BASE
      if((a1==(uint8_t)(STD_SUP_0            [0]) && (a2>=(uint8_t)(STD_SUP_0            [1]) && a2<=(uint8_t)(STD_SUP_9  [1]))) ) {*ascii = ('0'+a2)-(uint8_t)(STD_SUP_0 [1]);} else
      if((a1==(uint8_t)(STD_SUP_a            [0]) && (a2>=(uint8_t)(STD_SUP_a            [1]) && a2<=(uint8_t)(STD_SUP_z  [1]))) ) {*ascii = ('a'+a2)-(uint8_t)(STD_SUP_a [1]);} else
      if((a1==(uint8_t)(STD_SUP_A            [0]) && (a2>=(uint8_t)(STD_SUP_A            [1]) && a2<=(uint8_t)(STD_SUP_Z  [1]))) ) {*ascii = ('A'+a2)-(uint8_t)(STD_SUP_A [1]);} else
      if((a1==(uint8_t)(STD_SUB_0            [0]) && (a2>=(uint8_t)(STD_SUB_0            [1]) && a2<=(uint8_t)(STD_SUB_9  [1]))) ) {*ascii = ('0'+a2)-(uint8_t)(STD_SUB_0 [1]);} else
      if((a1==(uint8_t)(STD_SUB_a            [0]) && (a2>=(uint8_t)(STD_SUB_a            [1]) && a2<=(uint8_t)(STD_SUB_z  [1]))) ) {*ascii = ('a'+a2)-(uint8_t)(STD_SUB_a [1]);} else
      if((a1==(uint8_t)(STD_SUB_A            [0]) && (a2>=(uint8_t)(STD_SUB_A            [1]) && a2<=(uint8_t)(STD_SUB_Z  [1]))) ) {*ascii = ('A'+a2)-(uint8_t)(STD_SUB_A [1]);} else
      if((a1==(uint8_t)(STD_BASE_1           [0]) && (a2>=(uint8_t)(STD_BASE_1           [1]) && a2<=(uint8_t)(STD_BASE_9 [1]))) ) {*ascii = '#';  ascii++; *ascii = ('1'+a2)-(uint8_t)(STD_BASE_1[1]);} else
      if((a1==(uint8_t)(STD_BASE_10          [0]) && (a2>=(uint8_t)(STD_BASE_10          [1]) && a2<=(uint8_t)(STD_BASE_16[1]))) ) {*ascii = '#';  ascii++; *ascii =  '1'; ascii++; *ascii = ('0'+a2)-(uint8_t)(STD_BASE_10[1]);} else
      //RANGE INTERNATIONAL AND EXTENDED ASCII
      if(a1>=0x81 && a1<=0x83) *ascii = '_'; else //All international characters use 0x00 which is not otherwise used in C47
      //RANGE QUOTES
      if(a1==(uint8_t)(STD_LEFT_SINGLE_QUOTE[0]) && (a2>=(uint8_t)(STD_LEFT_SINGLE_QUOTE[1]) && a2<=(uint8_t)(STD_SINGLE_HIGH_QUOTE[1])) ) *ascii = '\''; else
      if(a1==(uint8_t)(STD_LEFT_DOUBLE_QUOTE[0]) && (a2>=(uint8_t)(STD_LEFT_DOUBLE_QUOTE[1]) && a2<=(uint8_t)(STD_DOUBLE_HIGH_QUOTE[1])) ) *ascii = '"'; else
      {
        #ifdef PC_BUILD
          printf("Not decoded, replace with _: --a1=%u--a2=%u\n",a1,a2);
        #endif// PC_BUILD
        *ascii = 0x5F;    // underscore
      }
    }
    else {
      *ascii = *str;
      str++;
    }
    ascii++;
    *ascii = 0;
  }
}

void *xcopy(void *dest, const void *source, int n) {
  char       *pDest   = (char *)dest;
  const char *pSource = (char *)source;

  if(pSource > pDest) {
    while(n--) {
      *pDest++ = *pSource++;
    }
  }
  else if(pSource < pDest) {
    while(n--) {
      pDest[n] = pSource[n];
    }
  }

  return dest;
}


char *stringAppend(char *dest, const char *source) {
  const size_t l = stringByteLength(source);
  return (char *)memcpy(dest, source, l + 1) + l;
}


#if !defined(DMCP_BUILD)
void strReplace(char *haystack, const char *needle, const char *newNeedle) {
  ////////////////////////////////////////////////////////
  // There MUST be enough memory allocated to *haystack //
  // when strlen(newNeedle) > strlen(needle)            //
  ////////////////////////////////////////////////////////
  char *str, *needleLocation;
  int  needleLg;

  while(strstr(haystack, needle) != NULL) {
    needleLg = strlen(needle);
    needleLocation = strstr(haystack, needle);
    str = malloc(strlen(needleLocation + needleLg) + 1);
    #if defined(PC_BUILD) && !defined(GENERATE_CATALOGS)
      if(str == NULL) {
        moreInfoOnError("In function strReplace:", "error allocating memory for str!", NULL, NULL);
        exit(1);
      }
    #endif // PC_BUILD && !GENERATE_CATALOGS

    strcpy(str, needleLocation + needleLg);
    *strstr(haystack, needle) = 0;
    strcat(haystack, newNeedle);
    strcat(haystack, str);
    free(str);
  }
}
#endif // !DMCP_BUILD
