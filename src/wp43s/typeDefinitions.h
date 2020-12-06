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
 * \file typeDefinitions.h
 ***********************************************/


/********************************************//**
 * \union multiplyDivide_t
 * \brief used for unit conversions
 ***********************************************/
typedef enum {
  multiply,
  divide
} multiplyDivide_t;


/********************************************//**
 * \struct calcKey_t
 * \brief Structure keeping the informations for one key
 ***********************************************/
typedef struct {
  int16_t keyId;       ///< ID of the key
  int16_t primary;     ///< ID of the primary function of the key
  int16_t fShifted;    ///< ID of the f shifted function of the key
  int16_t gShifted;    ///< ID of the g shifted function of the key
  int16_t keyLblAim;   ///< ID of the main label of the key
  int16_t primaryAim;  ///< ID of the primary AIM function: latin letters
  int16_t fShiftedAim; ///< ID of the f shifted AIM function:
  int16_t gShiftedAim; ///< ID of the g shifted AIM function: greek letters
  int16_t primaryTam;  ///< ID of the primary TAM function of the key
} calcKey_t;


/********************************************//**
 * \typedef glyph_t
 * \brief Structure keeping the informations for one glyph
 ***********************************************/
typedef struct {
  uint16_t charCode;         ///< Unicode code point
  uint8_t  colsBeforeGlyph;  ///< Number of empty columns before the glyph
  uint8_t  colsGlyph;        ///< Number of columns of the glyph
  uint8_t  colsAfterGlyph;   ///< Number of empty columns afer the glyph
  uint8_t  rowsAboveGlyph;   ///< Number of empty rows above the glyph
  uint8_t  rowsGlyph;        ///< Number of rows of the glyph
  uint8_t  rowsBelowGlyph;   ///< Number of empty rows below the glypg
  int16_t  rank1;            ///< Rank of the replacement glyph
  int16_t  rank2;            ///< Rank of the glyph
  char     *data;            ///< Hexadecimal data representing the glyph.
                             ///< There are rowsGlyph x (colsGlyph rounded up to 8 bit boundary) bytes
} glyph_t;


/********************************************//**
 * \typedef font_t
 * \brief Font description
 ***********************************************/
typedef struct {
  int8_t  id;              ///< 0=numeric 1=standard
  uint16_t numberOfGlyphs; ///< Number of glyphs in the font
  glyph_t glyphs[];        ///< Pointer to the glyph description structure
} font_t;


/********************************************//**
 * \typedef freeMemoryRegion_t
 * \brief Keeps track of free memory regions
 ***********************************************/
typedef struct {
  uint16_t address;      ///< Address of the free memory region
  uint16_t sizeInBlocks; ///< Sise in blocks of the free memory region
} freeMemoryRegion_t;


/********************************************//**
 * \typedef dataType_t
 * \brief Different data types
 ***********************************************/
typedef enum {
  dtLongInteger     =  0,  ///< Z arbitrary precision integer
  dtReal34          =  1,  ///< R double precision real (128 bits)
  dtComplex34       =  2,  ///< C double precision complex (2x 128 bits)
  dtTime            =  3,  ///< Time
  dtDate            =  4,  ///< Date in various formats
  dtString          =  5,  ///< Alphanumeric string
  dtReal34Matrix    =  6,  ///< Double precision vector or matrix
  dtComplex34Matrix =  7,  ///< Double precision complex vector or matrix
  dtShortInteger    =  8,  ///< Short integer (64 bit)
  dtConfig          =  9,  ///< Configuration
  //dtLabel           = 10,  ///< Label
  //dtSystemInteger   = 11,  ///< System integer (64 bits)
  //dtFlags           = 12,  ///< Flags
  //dtPgmStep         = 13,  ///< Program step
  //dtDirectory       = 14,  ///< Program
} dataType_t; // 4 bits (NOT 5 BITS)


/********************************************//**
 * \typedef dtConfigDescriptor_t
 * \brief Configuration for STOCFG and RCLCFG
 ***********************************************/
typedef struct {
  uint8_t   shortIntegerMode;
  uint8_t   shortIntegerWordSize;
  uint8_t   displayFormat;
  uint8_t   displayFormatDigits;
  uint8_t   groupingGap;
  uint8_t   currentAngularMode;
  uint8_t   displayStack;
  uint8_t   roundingMode;
  uint32_t  denMax;
  uint32_t  firstGregorianDay;
  uint64_t  systemFlags;
  calcKey_t kbd_usr[37];
} dtConfigDescriptor_t;


/********************************************//**
 * \typedef registerDescriptor_t
 * \brief 32 bits describing the register
 ***********************************************/
typedef union {
  uint32_t descriptor;
  struct {
    uint32_t dataPointer     : 16; ///< Memory block number
    uint32_t dataType        :  4; ///< dtLongInteger, dtReal16, ...
    uint32_t tag             :  5; ///< Short integer base or angular mode
    uint32_t notUsed         :  7; ///< 7 free bits
  };
} registerDescriptor_t;


// Header for datatype string, long integer, and matrix
/********************************************//**
 * \typedef dataBlock_t
 * \brief Different kind of data blocks
 ***********************************************/
typedef union {
  uint32_t data;
  struct {
    uint16_t dataMaxLength;          ///< String max length (includind terminating \0) in blocks or Long integer max length in blocks
    uint16_t numberOfNamedVariables; ///< Number of existing named variables
  };
  struct {
    uint16_t variableNameLen;        ///< Size of the name in blocs: 1 to 4, up to 15 bytes = 7 double byte glyphs + trailing 0
    uint16_t ptrToVariableName;      ///< Pointer to the name of the variable
  };
  struct {
    uint16_t localFlags;             ///< 16 local flags
    uint16_t numberOfLocalRegisters; ///< Number of declared local registers
  };
  struct {
    uint16_t matrixLines;            ///< Number of lines in the matrix
    uint16_t matrixColumns;          ///< Number of columns in the matrix
  };
} dataBlock_t;


/********************************************//**
 * \typedef videoMode_t
 * \brief Video mode: normal video or reverse video
 ***********************************************/
typedef enum {
  vmNormal,  ///< Normal mode: black on white background
  vmReverse  ///< Reverse mode: white on black background
} videoMode_t; // 1 bit


/********************************************//**
 * \struct softmenu_t
 * \brief Structure keeping the informations for one softmenu
 ***********************************************/
typedef struct {
  int16_t menuItem;           ///< ID of the menu. The item is always negative and -item must be in the indexOfItems area
  int16_t numItems;           ///< Number of items in the softmenu (must be a multiple of 6 for now)
  const int16_t *softkeyItem; ///< Pointer to the first item of the menu
} softmenu_t;


/********************************************//**
 * \struct dynamicSoftmenu_t
 * \brief Structure keeping the informations for one variable softmenu
 ***********************************************/
typedef struct {
  int16_t menuItem;           ///< ID of the menu. The item is always negative and -item must be in the indexOfItems area
  int16_t numItems;           ///< Number of items in the dynamic softmenu (must be a multiple of 6 for now)
  uint8_t *menuContent;       ///< Pointer to the menu content
} dynamicSoftmenu_t;


/********************************************//**
 * \struct softmenuStack_t
 * \brief Stack of softmenus
 ***********************************************/
typedef struct {
  int16_t softmenuId; ///< Softmenu ID = rank in dynamicSoftmenu or softmenu
  int16_t firstItem;  ///< Current first item on the screen (unshifted F1 = bottom left)
} softmenuStack_t;


/********************************************//**
 * \typedef bool_t
 * \brief Boolean type
 ***********************************************/
typedef enum {
  false = 0,     ///< Value for false
  true  = !false ///< Value for true
} bool_t; // 1 bit


/********************************************//**
 * \typedef calcRegister_t
 * \brief A type for calculator registers
 ***********************************************/
typedef int16_t calcRegister_t;


/********************************************//**
 * \typedef item_t
 * \brief Structure keeping the information for one item
 ***********************************************/
typedef struct {
  void     (*func)(uint16_t); ///< Function called to execute the item
  uint16_t param;             ///< 1st parameter to the above function
  char     *itemCatalogName;  ///< Name of the item in the catalogs
  char     *itemSoftmenuName; ///< Representation of the item in the menus or on the keyboard
  uint16_t tamMin;            ///< Minimul value for TAM argument
  uint16_t tamMax;            ///< Maximal value for TAM argument
  char     catalog;           ///< Menu of CATALOG in which the item is located: see #define CAT_* in defines.h
  uint8_t  stackLiftStatus;   ///< Stack lift status after item execution.
  uint8_t  undoStatus;        ///< Undo status after item execution.
} item_t;


/********************************************//**
 * \typedef labelList_t
 * \brief Structure keeping the information for a program label
 ***********************************************/
typedef struct {
  int16_t  program;             ///< Program id: <0 for FLASH and >0 for RAM
  int32_t  step;                ///< Step number of the label: <0 for a local label and >0 for a global label
  uint8_t  *labelPointer;       ///< Pointer to the byte after the 0x01 op code (LBL)
  uint8_t  *instructionPointer; ///< Pointer to the instructiuon following the label
} labelList_t;


/********************************************//**
 * \typedef programList_t
 * \brief Structure keeping the information for a program
 ***********************************************/
typedef struct {
  int32_t  step;                ///< (Step number + 1) of the program begin: <0 for a FLASH program and >0 for a RAM program
  uint8_t  *instructionPointer; ///< Pointer to the program begin
} programList_t;
