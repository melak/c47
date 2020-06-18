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

#define NUMERIC_FONT_HEIGHT                      36
#define STANDARD_FONT_HEIGHT                     22
#define NUMBER_OF_NUMERIC_FONT_LINES_PER_SCREEN   5
#define NUMBER_OF_STANDARD_FONT_LINES_PER_SCREEN  8

#ifndef TESTSUITE_BUILD
  void initFontBrowser(void);
  void fontBrowser    (uint16_t unusedParamButMandatory);
#endif
