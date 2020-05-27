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

/********************************************//**
 * \file arctan.h
 ***********************************************/

void fnArg_all      (uint16_t unusedParamButMandatory);
void fnToPolar2     (uint16_t unusedParamButMandatory);
void fnToRect2      (uint16_t unusedParamButMandatory);
void fnTo_ms        (uint16_t unusedParamButMandatory);
void fnMultiplySI   (uint16_t multiplier);
void fn_cnst_op_j   (uint16_t unusedParamButMandatory);
void fn_cnst_op_aa  (uint16_t unusedParamButMandatory);
void fn_cnst_op_a   (uint16_t unusedParamButMandatory);
void fn_cnst_0_cpx  (uint16_t unusedParamButMandatory);
void fn_cnst_1_cpx  (uint16_t unusedParamButMandatory);
void fnJMup         (uint16_t unusedParamButMandatory);
void fnJMdown       (uint16_t unusedParamButMandatory);
void fnJM_2SI       (uint16_t unusedParamButMandatory);

//for display.c
void fnDisplayFormatSigFig      (uint16_t displayFormatN);
void fnDisplayFormatUnit        (uint16_t displayFormatN);
void exponentToUnitDisplayString(int32_t exponent, char *displayString, char *displayValueString, bool_t nimMode, const char *separator);
