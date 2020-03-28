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

void fnDisplayOvr                 (uint16_t displayOvr);
void fnProductSign                (uint16_t productSign);
void fnRadixMark                  (uint16_t radixMark);
void fnDisplayFormatFix           (uint16_t displayFormat);
void fnDisplayFormatSci           (uint16_t displayFormat);
void fnDisplayFormatEng           (uint16_t displayFormat);
void fnDisplayFormatAll           (uint16_t displayFormat);
//void fnDisplayFormatDsp           (uint16_t displayFormat);
void fnDisplayFormatGap           (uint16_t gap);
void fnShow                       (uint16_t unusedParamButMandatory);
void fnShow_SCROLL                (uint16_t fnShow_param);
void real34ToDisplayString        (const real34_t *real34, uint32_t tag, char *displayString, const font_t *font, int16_t maxWidth, int16_t displayHasNDigits);
void realToDisplayString2         (const real34_t *real34, char *displayString, int16_t displayHasNDigits);
void complex34ToDisplayString     (const complex34_t *complex34, char *displayString, const font_t *font, int16_t maxWidth, int16_t displayHasNDigits);
void complex34ToDisplayString2    (const complex34_t *complex34, char *displayString, int16_t displayHasNDigits);
void fractionToDisplayString      (calcRegister_t regist, char *displayString);
void angle34ToDisplayString2      (const real34_t *angle34, uint8_t mode, char *displayString, int16_t displayHasNDigits);
void shortIntegerToDisplayString  (calcRegister_t regist, char *displayString, const font_t **font);
void longIntegerToDisplayString   (calcRegister_t regist, char *displayString, int32_t strLg, int16_t maxWidth, int16_t maxExp);
void dateToDisplayString          (calcRegister_t regist, char *displayString);
void timeToDisplayString          (calcRegister_t regist, char *displayString);
void exponentToDisplayString      (int32_t exponent, char *displayString, char *displayValueString, bool_t nimMode);
void supNumberToDisplayString     (int32_t supNumber, char *displayString, char *displayValueString, bool_t insertGap, bool_t isExponent);
void subNumberToDisplayString     (int32_t subNumber, char *displayString, char *displayValueString);
void longIntegerToAllocatedString (longInteger_t lgInt, char *str, int32_t strLg);

int16_t SHOWregis;                               //JMSHOW
