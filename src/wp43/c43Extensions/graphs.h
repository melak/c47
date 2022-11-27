/* This file is part of WP43.
 *
 * W43 is free software: you can redistribute it and/or modify
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

/********************************************//** //JM
 * \file graph.c Graphing module
 ***********************************************/

/* ADDITIONAL C43 functions and routines */

//graphs.h

#ifndef GRAPHS_H
#define GRAPHS_H

#include "defines.h"
#include <stdint.h>

//Graph functions
TO_QSPI void    graph_reset        (void);
TO_QSPI void    plotrect           (uint16_t a, uint8_t b, uint16_t c, uint8_t d);
TO_QSPI void    fnClGrf            (uint16_t unusedButMandatoryParameter);
TO_QSPI void    fnPline            (uint16_t unusedButMandatoryParameter);
TO_QSPI void    fnPcros            (uint16_t unusedButMandatoryParameter);
TO_QSPI void    fnPbox             (uint16_t unusedButMandatoryParameter);
TO_QSPI void    fnPintg            (uint16_t unusedButMandatoryParameter);
TO_QSPI void    fnPdiff            (uint16_t unusedButMandatoryParameter);
TO_QSPI void    fnPrms             (uint16_t unusedButMandatoryParameter);
TO_QSPI void    fnPvect            (uint16_t unusedButMandatoryParameter);
TO_QSPI void    fnPNvect           (uint16_t unusedButMandatoryParameter);
TO_QSPI void    fnScale            (uint16_t unusedButMandatoryParameter);
TO_QSPI void    fnPshade           (uint16_t unusedButMandatoryParameter);
TO_QSPI void    fnPzoom            (uint16_t param);
TO_QSPI void    fnPx               (uint16_t unusedButMandatoryParameter);
TO_QSPI void    fnPy               (uint16_t unusedButMandatoryParameter);
TO_QSPI void    fnListXY           (uint16_t unusedButMandatoryParameter);
TO_QSPI void    fnStatList         (void);
TO_QSPI void    graph_plotmem      (void);
TO_QSPI void    fnPlotSQ           (uint16_t unusedButMandatoryParameter);
TO_QSPI void    fnPlotReset        (uint16_t unusedButMandatoryParameter);
TO_QSPI void    fnPlotStatAdv      (uint16_t unusedButMandatoryParameter);

#endif //GRAPHS_H
