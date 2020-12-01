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
 * \file softmenu.h
 ***********************************************/

uint8_t *getNthString           (uint8_t *ptr, int16_t n); // Starting with string 0 (the 1st string is returned for n=0)
void     fnDynamicMenu          (uint16_t unusedButMandatoryParameter);
#ifndef TESTSUITE_BUILD
  void   showSoftkey            (const char *label, int16_t xSoftkey, int16_t ySoftKey, videoMode_t videoMode, bool_t topLine, bool_t bottomLine);
  void   showSoftmenuCurrentPart(void);
  void   showSoftmenu           (const char *menu, int16_t id, int16_t initOrPushOnMenuStack);
  void   initSoftmenuStack      (int16_t softmenuId);
  void   pushSoftmenu           (int16_t softmenuId);
  void   popSoftmenu            (void);
  void   setCatalogLastPos      (void);
#endif // !TESTSUITE_BUILD
