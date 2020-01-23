/* This file is part of 43C.
 *
 * 43C is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * 43C is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 43C.  If not, see <http://www.gnu.org/licenses/>.
 */

/********************************************//**
 * \file radioButtonCatalog.h
 ***********************************************/


#define RB_FALSE    0
#define RB_TRUE     1
#define CB_FALSE    2
#define CB_TRUE     3

#define ITEM_NOT_CODED     -127


int8_t   fnCbIsSet             (int16_t item);
void     fnRefreshRadioState   (char rb, uint16_t mode);
void     fnRefreshComboxState  (char rb, uint16_t param, bool_t state);
int16_t  fnItemShowValue       (int16_t item);
void     fnRebuildRadioState   (void);
char*    figlabel              (const char* label, int16_t showValue);  //DR TOCHECK


/********************************************//**
 * \typedef radiocb_t
 * \brief Structure keeping the information for one item
 ***********************************************/
#define RB_NO   ' '
#define RB_AM   '!' // AngularMode
#define RB_CM   '"' // ComplexMode
#define RB_CU   '#' // ComplexUnit
#define RB_CF   '$' // CurveFitting
#define RB_DF   '%' // DateFormat
#define RB_DM   '&' // DenominatorMode
#define RB_DI   '\''// DisplayFormat
#define RB_DO   '(' // DisplayModeOverride
#define RB_FT   ')' // FractionType
#define RB_IM   '*' // IntegerMode
#define RB_PS   '+' // ProductSign
#define RB_RM   ',' // RadixMark
#define RB_SS   '-' // StackSize
#define RB_TF   '.' // TimeFormat
#define RB_WS   '/' // WordSize
//#define RB_BCR  '0' // ComplexResult
//#define RB_BLZ  '1' // LeadingZeros
#define RB_SA   '>' // SigmaAssign
#define RB_ID   '?' // InputDefault
#define CB_NO   '@'
#define CB_JC   'A' // CheckBox 



typedef struct {
//uint16_t itemNr;            ///<
//uint16_t param;             ///< 1st parameter to the above function
  uint8_t  state;             ///<
//char     radioButton;       ///< Menu of RADIO in which the item is located: see #define RB_*
} radiocb_t;



typedef struct {
  uint16_t itemNr;            ///<
  uint16_t param;             ///< 1st parameter to the above function
//uint8_t  state;             ///<
  char     radioButton;       ///< Menu of RADIO in which the item is located: see #define RB_*
} radiocb_eeprom_t;
