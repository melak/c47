/* This file is part of C43.
 *
 * C43 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * C43 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with C43.  If not, see <http://www.gnu.org/licenses/>.
 */

/********************************************//**
 * \file eulersFormula.c
 ***********************************************/

#if !defined(EULERSFORMULA_H)
#define EULERSFORMULA_H

#include "defines.h"
#include <stdint.h>

void fnEulersFormula(uint16_t unusedButMandatoryParameter);
  
#if (EXTRA_INFO_ON_CALC_ERROR == 1)
void eulersFormulaError  (void);
  #else // (EXTRA_INFO_ON_CALC_ERROR != 1)
#define eulersFormulaError typeError
#endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  
void eulersFormulaCplx       (void);
void eulersFormulaReal       (void);
void eulersFormulaLongint    (void);

#endif // !EULERSFORMULA_H
