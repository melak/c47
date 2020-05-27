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

/* ADDITIONAL C43 functions and routines */

//graph.h

//Memory structure
float*   gr_x;
float*   gr_y;
float    telltale;
uint16_t ix_count;
#define  LIM               400                 //Number of points
#define  MEM_INITIALIZED   199


//Screen limits
#define SCREEN_MIN_GRAPH 20
#define SCREEN_HEIGHT_GRAPH SCREEN_HEIGHT
#define SCREEN_WIDTH_GRAPH  SCREEN_WIDTH

#define _VECT 0
#define _SCAT 1

int8_t plotmode;                            //      _SCAT
#define Aspect_Square false

void fnGraph          (uint16_t func);
void graph_sigmaplus  (int8_t plusminus, real_t *xx, real_t *yy);   //Called from STAT module from fnSigma(), to store the x,y pair to the memory structure.

void graph_setupmemory(void);
void graph_end        (void);

void fnStatList       (uint16_t unusedParamButMandatory);


//TODO
void autoscale(void);
void defaultscale(void);
