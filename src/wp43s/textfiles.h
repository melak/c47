/* This file is part of WP 43S.
 *
 * WP 43S is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WP 43S is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WP 43S.  If not, see <http://www.gnu.org/licenses/>.
 */

/* ADDITIONAL C43 functions and routines */


/********************************************//** //JM
 * \file jmgraph.c Graphing module
 ***********************************************/

/* ADDITIONAL WP43C functions and routines */

char         filename_csv[40]; //JMMAX                //JM_CSV
uint32_t     tmp__32;                                 //JM_CSV
uint32_t     mem__32;                                 //JM_CSV

#define      CSV_NEWLINE "\n"
#define      CSV_TAB     "\t"
#define      CSV_CR      "\r"
#define      CSV_SLASH   "\\"
#define      CSV_STR     "\""
 
int16_t      testjm(void);
int16_t      test_xy(float x, float y);
void         fnP_All_Regs(uint16_t unusedParamButMandatory);
//void         print_line(bool_t line_init);
void         print_linestr(const char line1[TMP_STR_LENGTH], bool_t line_init);
int16_t      test_line(char *inputstring);
