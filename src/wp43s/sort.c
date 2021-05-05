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

#include "sort.h"

#include "charString.h"
#include "fonts.h"

#include "wp43s.h"



int32_t compareString(const char *stra, const char *strb, int32_t comparisonType) {
  int32_t lga, lgb, i;
  int16_t posa, posb, ranka, rankb;
  uint16_t charCode;

  lga = stringGlyphLength(stra);
  lgb = stringGlyphLength(strb);

  // Compare the string using replacement glyphs
  posa = 0;
  posb = 0;
  for(i=0; i<min(lga, lgb); i++) {
    charCode = (uint8_t)stra[posa];
    if(charCode >= 0x80) {
      charCode = (charCode << 8) + (uint8_t)stra[posa + 1];
    }
    ranka = standardFont.glyphs[findGlyph(&standardFont, charCode)].rank1;

    charCode = (uint8_t)strb[posb];
    if(charCode >= 0x80) {
      charCode = (charCode << 8) + (uint8_t)strb[posb + 1];
    }
    rankb = standardFont.glyphs[findGlyph(&standardFont, charCode)].rank1;

    if(ranka < rankb) return -1;
    if(ranka > rankb) return 1;

    posa = stringNextGlyph(stra, posa);
    posb = stringNextGlyph(strb, posb);
  }

  if(lga < lgb) return -1;
  if(lga > lgb) return 1;

  // The strings using replacement glyphs are equal: comparing the original strings
  if(comparisonType == CMP_EXTENSIVE) {
    posa = 0;
    posb = 0;
    for(i=0; i<min(lga, lgb); i++) {
      charCode = (uint8_t)stra[posa];
      if(charCode >= 0x80) {
        charCode = (charCode << 8) + (uint8_t)stra[posa + 1];
      }
      ranka = standardFont.glyphs[findGlyph(&standardFont, charCode)].rank2;

      charCode = (uint8_t)strb[posb];
      if(charCode >= 0x80) {
        charCode = (charCode << 8) + (uint8_t)strb[posb + 1];
      }
      rankb = standardFont.glyphs[findGlyph(&standardFont, charCode)].rank2;

      if(ranka < rankb) return -1;
      if(ranka > rankb) return 1;

      posa = stringNextGlyph(stra, posa);
      posb = stringNextGlyph(strb, posb);
    }

    if(lga < lgb) return -1;
    if(lga > lgb) return 1;
  }

  return 0;
}
