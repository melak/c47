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

#define MY_ALPHA_MENU    0  // This is the index of the MyAlpha   softmenu in the softmenu[] array
#define SOFTMENU_HEIGHT 23

/********************************************//**
 * \struct softmenu_t
 * \brief Structure keeping the informations for one softmenu
 ***********************************************/
typedef struct {
  int16_t menuId;             ///< ID of the menu. The ID is always negative and -ID must be in the indexOfItems area
  int16_t numItems;           ///< Number of items in the softmenu (must be a multiple of 6 for now)
  const int16_t *softkeyItem; ///< Pointer to the first item of the menu
} softmenu_t;

/********************************************//**
 * \struct softmenuStack_t
 * \brief Stack of softmenus
 ***********************************************/
typedef struct {
  int16_t softmenu;  ///< Softmenu ID
  int16_t firstItem; ///< Current first item on the screen (unshifted F1 = bottom left)
} softmenuStack_t;

void showSoftkey            (const char *label, int16_t xSoftkey, int16_t ySoftKey, videoMode_t videoMode, bool_t topLine, bool_t bottomLine, int8_t showCb, int16_t showValue); //dr
void showSoftmenuCurrentPart(void);
void showSoftmenu           (const char *menu, int16_t id, bool_t submenu);
void initSoftmenuStack      (int16_t softmenu);
void pushSoftmenu           (int16_t softmenu);
void popSoftmenu            (void);
void setCatalogLastPos      (void);
//void Load_HOME              (void);   //JMHOMEDEMO: NOTE REMOVE comment TO MAKE JMHOME DEMO WORK

const int16_t menu_A_HOME[360];   //JM
const int16_t menu_HOME[360];     //JM
