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

/********************************************//**
 * \file xeqm.h
 ***********************************************/
#if !defined(XEQM_H)
#define XEQM_H

#include "typeDefinitions.h"
#include <stdint.h>

extern bool_t running_program_jm;

bool_t strcompare            (const char *in1, const char *in2);
bool_t getXeqmText           (int16_t com, char *str);
void fnXEQMENU               (uint16_t unusedButMandatoryParameter);
void capture_sequence        (char *origin, uint16_t item);
void XEQMENU_loadAllfromdisk (void);
void fnXEQMSAVE              (uint16_t XEQM_no);
void fnXEQMLOAD              (uint16_t XEQM_no);
void fnXSWAP                 (uint16_t unusedButMandatoryParameter);
void fnXEQMXXEQ              (uint16_t unusedButMandatoryParameter);
void fnXEQNEW                (uint16_t unusedButMandatoryParameter);
void fnXEQMexecute           (char *line1);

#endif // !XEQM_H
