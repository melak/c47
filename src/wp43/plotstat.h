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

/**
 * \file plot.h
 ***********************************************/
#if !defined(PLOTSTAT_H)
#define PLOTSTAT_H

#include "realType.h"
#include "typeDefinitions.h"
#include <stdint.h>

#define   useFLOAT           0
#define   useREAL4           4
#define   useREAL39         39
#define   zoomfactor     0.05f     // default is 0.05, which is 5% space around the data points. Use 0.05 * 40 for wide view
#define   numberIntervals   50     // default 50, auto increase if jumps found
#define   fittedcurveboxes   0     // default 0 = smooth line
#define   USEFLOATING useFLOAT     // USEFLOATING can be used to change the STATS graphing to work using different number types.
                                   //   Note that limited precision is required as only the pixels on screen need to be considered
                                   //   useFLOAT is much faster plotting STATS graphs on the real hardware than useREAL4 and useREAL39
#define   FLoatingMax    1e38f     //convenient round figures used for maxima and minima determination
#define   FLoatingMin    -1e38f


//Graph options
extern  float    graph_dx;
extern  float    graph_dy;
extern  bool_t   roundedTicks;
extern  bool_t   extentx;
extern  bool_t   extenty;
extern  bool_t   PLOT_VECT;
extern  bool_t   PLOT_NVECT;
extern  bool_t   PLOT_SCALE;
extern  bool_t   Aspect_Square;
extern  bool_t   PLOT_LINE;
extern  bool_t   PLOT_CROSS;
extern  bool_t   PLOT_BOX;
extern  bool_t   PLOT_INTG;
extern  bool_t   PLOT_DIFF;
extern  bool_t   PLOT_RMS;
extern  bool_t   PLOT_SHADE;
extern  bool_t   PLOT_AXIS;
extern  int8_t   PLOT_ZMX;
extern  int8_t   PLOT_ZMY;
extern  uint8_t  PLOT_ZOOM;
  extern  uint8_t  drawHistogram;

#define _VECT 0
#define _SCAT 1

extern  int8_t   plotmode;    //VECTOR or SCATTER
extern  float    tick_int_x;
extern  float    tick_int_y;
extern  float    x_min;
extern  float    x_max;
extern  float    y_min;
extern  float    y_max;
extern  uint32_t xzero;
extern  uint32_t yzero;



//Screen limits
#define SCREEN_MIN_GRAPH    20
#define SCREEN_HEIGHT_GRAPH SCREEN_HEIGHT
#define SCREEN_WIDTH_GRAPH  SCREEN_WIDTH
#define SCREEN_NONSQ_HMIN   0



//Utility functions
void    placePixel         (uint32_t x, uint32_t y);
void    removePixel        (uint32_t x, uint32_t y);
void    clearScreenPixels  ();
void    plotcross          (uint16_t xn, uint8_t yn);                      // Plots line from xo,yo to xn,yn; uses temporary x1,y1
void    plotbox            (uint16_t xn, uint8_t yn);                      // Plots line from xo,yo to xn,yn; uses temporary x1,y1
void    plotrect           (uint16_t a, uint8_t b, uint16_t c, uint8_t d); // Plots rectangle from xo,yo to xn,yn; uses temporary x1,y1
void    pixelline          (uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn, bool_t vmNormal);              // Plots line from xo,yo to xn,yn; uses temporary x1,y1
void    plotline           (uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn);
void    plotline2          (uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn);
void    graphAxisDraw      (void);
void    graph_axis         (void);
float   auto_tick          (float tick_int_f);


//graph functions
float grf_x(int i);
float grf_y(int i);

char * radixProcess(const char * ss);
void eformat_eng2 (char* s02, const char* s01, double inreal, int8_t digits, const char* s05);
char * padEquals(const char * ss);


  #if !defined(TESTSUITE_BUILD)
int16_t screen_window_x(float x_min, float x, float x_max);
int16_t screen_window_y(float y_min, float y, float y_max);
  #endif // !TESTSUITE_BUILD

void    statGraphReset     (void);
void    graphPlotstat      (uint16_t selection);
void    graphDrawLRline    (uint16_t selection);
//void    fnPlotClose        (uint16_t unusedButMandatoryParameter);
void    fnPlotCloseSmi     (uint16_t unusedButMandatoryParameter);
void    fnPlotStat         (uint16_t unusedButMandatoryParameter);
void    fnPlotZoom         (uint16_t unusedButMandatoryParameter);
#endif // !PLOTSTAT_H
