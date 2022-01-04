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

/********************************************//** //JM
 * \file jmgraph.c Graphing module
 ***********************************************/


#ifndef GRAPH_H
#define GRAPH_H

#include <stdint.h>


#define EQ_SOLVE 0
#define EQ_PLOT  1
#define EQ_PLOT2 2

void fnEqSolvGraph (uint16_t func);
void graph_eqn(uint16_t unusedButMandatoryParameter);

#endif // GRAPH_H
