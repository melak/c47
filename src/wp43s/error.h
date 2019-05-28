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

#define NO_ERROR                      0
#define EXCEEDS_FUNCTION_DOMAIN_ERROR 1
#define BAD_TIME_OR_DATE_ERROR        2
#define UNDEFINED_OPCODE_ERROR        3
#define POSITIVE_OVERFLOW_ERROR       4
#define NEGATIVE_OVERFLOW_ERROR       5
#define LABEL_NOT_FOUND_ERROR         6
#define FUNCTION_NOT_FOUND_ERROR      7
#define OUT_OF_RANGE_ERROR            8
#define INVALID_INTEGER_INPUT_ERROR   9
#define INPUT_TOO_LONG_ERROR          10
#define RAM_FULL_ERROR                11
#define STACK_CLASH_ERROR             12
#define OPERATION_UNDEFINED_ERROR     13
#define WORD_SIZE_TOO_SMALL_ERROR     14
#define TOO_FEW_DATA_ERROR            15
#define INVALID_DISTRIBUTION_ERROR    16
#define IO_ERROR                      17
#define INVALID_CORRUPTED_DATA_ERROR  18
#define FLASH_MEMORY_WRITE_ERROR      19
#define NO_ROOT_FOUND_ERROR           20
#define MATRIX_MISMATCH_ERROR         21
#define SINGULAR_MATRIX_ERROR         22
#define FLASH_MEMORY_FULL_ERROR       23
#define INVALID_DATA_INPUT_ERROR      24
#define EMPTY_ERROR                   25
#define ENTER_NEW_NAME_ERROR          26
#define CANNOT_DELETE_ITEM            27
#define BAD_INPUT_ERROR               28
#define ITEM_TO_BE_CODED_ERROR        29
#define FUNCTION_TO_BE_CODED_ERROR    30

#define NUMBER_OF_ERROR_CODES         31

#ifndef TESTSUITE_BUILD
void displayBugScreen       (const char *message);
#endif

void displayCalcErrorMessage(uint8_t errorCode, calcRegister_t errMessageRegisterLine, calcRegister_t errRegisterLine);

#ifdef PC_BUILD
void showInfoDialog         (const char *m1, const char *m2, const char *m3, const char *m4);
#endif
