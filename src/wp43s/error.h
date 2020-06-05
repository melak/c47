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
 * \file error.h
 ***********************************************/

#define ERROR_NONE                          0
#define ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN   1
#define ERROR_BAD_TIME_OR_DATE_INPUT        2
#define ERROR_UNDEFINED_OPCODE              3
#define ERROR_OVERFLOW_PLUS_INF             4
#define ERROR_OVERFLOW_MINUS_INF            5
#define ERROR_LABEL_NOT_FOUND               6
#define ERROR_FUNCTION_NOT_FOUND            7
#define ERROR_OUT_OF_RANGE                  8
#define ERROR_INVALID_INTEGER_INPUT         9
#define ERROR_INPUT_TOO_LONG               10
#define ERROR_RAM_FULL                     11
#define ERROR_STACK_CLASH                  12
#define ERROR_OPERATION_UNDEFINED          13
#define ERROR_WORD_SIZE_TOO_SMALL          14
#define ERROR_TOO_FEW_DATA                 15
#define ERROR_INVALID_DISTRIBUTION_PARAM   16
#define ERROR_IO                           17
#define ERROR_INVALID_CORRUPTED_DATA       18
#define ERROR_FLASH_MEMORY_WRITE_PROTECTED 19
#define ERROR_NO_ROOT_FOUND                20
#define ERROR_MATRIX_MISMATCH              21
#define ERROR_SINGULAR_MATRIX              22
#define ERROR_FLASH_MEMORY_FULL            23
#define ERROR_INVALID_DATA_TYPE_FOR_OP     24
#define ERROR_WP34S_COMPAT                 25
#define ERROR_ENTER_NEW_NAME               26
#define ERROR_CANNOT_DELETE_PREDEF_ITEM    27
#define ERROR_NO_SUMMATION_DATA            28
#define ERROR_ITEM_TO_BE_CODED             29
#define ERROR_FUNCTION_TO_BE_CODED         30
#define ERROR_INPUT_DATA_TYPE_NOT_MATCHING 31
#define ERROR_WRITE_PROTECTED_SYSTEM_FLAG  32
#define ERROR_STRING_WOULD_BE_TOO_LONG     33
#define ERROR_EMPTY_STRING                 34
#define ERROR_BAD_INPUT                    35 // This error is not in ReM and cannot occur (theoretically).

#define NUMBER_OF_ERROR_CODES              36

#ifndef TESTSUITE_BUILD
void displayBugScreen       (const char *message);
#endif

void displayCalcErrorMessage(uint8_t errorCode, calcRegister_t errMessageRegisterLine, calcRegister_t errRegisterLine);

#ifdef PC_BUILD
void showInfoDialog         (const char *m1, const char *m2, const char *m3, const char *m4);
#endif
