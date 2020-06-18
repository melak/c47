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
 * \file charString.h
 ***********************************************/

int16_t  stringLastGlyph  (const char *str);
int16_t  stringNextGlyph  (const char *str, int16_t pos);
int32_t  stringByteLength (const char *str);
int32_t  stringGlyphLength(const char *str);
int16_t  stringWidth      (const char *str, const font_t *font, bool_t withLeadingEmptyRows, bool_t withEndingEmptyRows);
void     codePointToUtf8  (uint32_t codePoint, uint8_t *utf8);
uint32_t utf8ToCodePoint  (const uint8_t *utf8, uint32_t *codePoint);
void     stringToUtf8     (const char *str, uint8_t *utf8);
void     utf8ToString     (const uint8_t *utf8, char *str);

uint8_t compressWidth;                                      //JM compressWidth
