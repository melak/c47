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

#include "wp43s.h"



uint8_t  compressWidth = 0;                                 //JM compressWidth
/********************************************//**
 * \brief Calculates a string width in pixel using a certain font
 *
 * \param[in] str const char*             String whose length is to calculate
 * \param[in] font font_t*                Font
 * \param[in] withLeadingEmptyRows bool_t With the leading empty rows
 * \param[in] withEndingEmptyRows bool_t  With the ending empty rows
 * \return int16_t                        Width in pixel of the string
 ***********************************************/
int16_t stringWidth(const char *str, const font_t *font, bool_t withLeadingEmptyRows, bool_t withEndingEmptyRows) {
  int16_t ch, numPixels, charCode, glyphId;
  const glyph_t *glyph;
  bool_t  firstChar;

  glyph = NULL;
  firstChar = true;
  numPixels = 0;
  ch = 0;
  while(str[ch] != 0) {
    charCode = (uint8_t)str[ch++];
    if(charCode & 0x80) { // MSB set
      charCode = (charCode<<8) | (uint8_t)str[ch++];
    }

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
      #ifdef GENERATE_CATALOGS
        printf("\n---------------------------------------------------------------------------\n"
                 "In function stringWidth: %d is an unexpected value returned by findGlyph!"
               "/n---------------------------------------------------------------------------\n", glyphId);
      #else
        sprintf(errorMessage, "In function stringWidth: %d is an unexpected value returned by findGlyph!", glyphId);
        displayBugScreen(errorMessage);
      #endif
      return 0;
    }

    numPixels += glyph->colsGlyph + glyph->colsAfterGlyph;
    if(compressWidth > 0) { numPixels--; }                  //JM
    if(firstChar) {
      firstChar = false;
      if(withLeadingEmptyRows) {
        numPixels += glyph->colsBeforeGlyph;
      }
    }
    else {
      numPixels += glyph->colsBeforeGlyph;
    }
  }

  if(glyph != NULL && withEndingEmptyRows == false) {
    numPixels -= glyph->colsAfterGlyph;
  }
  return numPixels;
}



/********************************************//**
 * \brief Returns a pointer to the glyph after pos a string
 *
 * \param[in] str const char*
 * \param[in] pos int16_t       Location after which search the next glyph
 * \return int16_t              Pointer to the glyph after pos
 ***********************************************/
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



/********************************************//**
 * \brief Returns a pointer to the last glyph of a string
 *
 * \param[in] str const char*
 * \return int16_t              Pointer to the last glyph
 ***********************************************/
int16_t stringLastGlyph(const char *str) {
  int16_t lastGlyph, next, lg;

  next = 0;

  if(str == NULL) {
    lastGlyph = -1;
  }
  else {
    lg = stringByteLength(str);
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



/********************************************//**
 * \brief Returns a string length in byte
 *
 * \param[in] str const char*
 * \return int32_t
 ***********************************************/
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



/********************************************//**
 * \brief Returns a string length in glyphs
 *
 * \param[in] str const char*
 * \return int32_t
 ***********************************************/
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



/********************************************//**
 * \brief Converts an unicode code point to utf8
 *
 * \param[in] codePoint uint32_t Unicode code point
 * \param[in] utf8 uint8_t*      utf8 string
 * \return void
 ***********************************************/
void codePointToUtf8(uint32_t codePoint, uint8_t *utf8) { // code point must be from 0x0 to 0x10FFFF
  if(codePoint <= 0x00007F) {
    utf8[0] = codePoint;
    utf8[1] = 0;
    utf8[2] = 0;
    utf8[3] = 0;
  }
  else if(codePoint <= 0x0007FF) {
    utf8[0] = 0xC0 | ((codePoint >> 6) & 0x1F);
    utf8[1] = 0x80 | ((codePoint     ) & 0x3F);
    utf8[2] = 0;
    utf8[3] = 0;
  }
  else if(codePoint <= 0x00FFFF) {
    utf8[0] = 0xE0 | ((codePoint >> 12) & 0x0F);
    utf8[1] = 0x80 | ((codePoint >>  6) & 0x3F);
    utf8[2] = 0x80 | ((codePoint      ) & 0x3F);
    utf8[3] = 0;
  }
  else {
    utf8[0] = 0xF0 | ((codePoint >> 18) & 0x07);
    utf8[1] = 0x80 | ((codePoint >> 12) & 0x3F);
    utf8[2] = 0x80 | ((codePoint >>  6) & 0x3F);
    utf8[3] = 0x80 | ((codePoint      ) & 0x3F);
  }

  utf8[4] = 0;
}


void stringToUtf8(const char *str, uint8_t *utf8) {
  int16_t  len;

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
