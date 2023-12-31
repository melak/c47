/* This file is part of WP43.
 *
 * WP43 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WP43 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WP43.  If not, see <http://www.gnu.org/licenses/>.
 */

/* ADDITIONAL C43 functions and routines */


/********************************************//** //JM
 * \file jmgraph.c TEXTFILES module
 ***********************************************/

#if !defined(TEXTFILES_H)
#define TEXTFILES_H

#include "defines.h"
#include "typeDefinitions.h"
#include <stdint.h>

void         print_inlinestr(const char *line1, bool_t endline);
void         print_Register_line(calcRegister_t regist, char *before, char *after, bool_t line_init);
void         displaywords(char *line1);
int16_t      export_string_to_file(const char line1[TMP_STR_LENGTH]);
void         stackregister_csv_out(int16_t reg_b, int16_t reg_e);
void         aimBuffer_csv_out(void);
void         copyRegisterToClipboardString2(calcRegister_t regist, char *clipboardString);

#endif // !TEXTFILES_H
