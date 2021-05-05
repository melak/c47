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

#include "plotstat.h"

#include "charString.h"
#include "constantPointers.h"
#include "curveFitting.h"
#include "debug.h"
#include "error.h"
#include "flags.h"
#include "fonts.h"
#include "items.h"
#include "keyboard.h"
#include "mathematics/variance.h"
#include "registers.h"
#include "screen.h"
#include "softmenus.h"
#include "stack.h"
#include "stats.h"
#include "statusBar.h"
#include <math.h>
#include <string.h>
#include <time.h>

#include "wp43s.h"


static char tmp_names1[30];
char * padEquals(const char * ss);


// This module originates and is part of the C43 fork, and is copied here. 
// Do not change the shared functions otherwise the C43 fork will break. JM 2021-03-20

#ifndef TESTSUITE_BUILD
  static real_t RR,SMI,aa0,aa1,aa2; //L.R. variables
#endif //TESTSUITE_BUILD


#define roundedTicks false    //todo make variable
graphtype *gr_x;
graphtype *gr_y;
graphtype telltale;
uint16_t  ix_count;
  
float     graph_dx;           // Many unused functions in WP43S. Do not change the variables.
float     graph_dy; 
bool_t    extentx;
bool_t    extenty;
bool_t    jm_VECT;
bool_t    jm_NVECT;
bool_t    jm_SCALE;
bool_t    Aspect_Square;
bool_t    PLOT_LINE;
bool_t    PLOT_CROSS;
bool_t    PLOT_BOX;
bool_t    PLOT_INTG;
bool_t    PLOT_DIFF;
bool_t    PLOT_RMS;
bool_t    PLOT_SHADE;
bool_t    PLOT_AXIS;
int8_t    PLOT_ZMX;
int8_t    PLOT_ZMY;
uint8_t   PLOT_ZOOM;

int8_t    plotmode;
float     tick_int_x;
float     tick_int_y;
float     x_min; 
float     x_max;
float     y_min;
float     y_max;
uint32_t  xzero;
uint32_t  yzero;


void statGraphReset(void){
  graph_dx      = 0;
  graph_dy      = 0;
  extentx       = true;
  extenty       = true;
  jm_VECT       = false;
  jm_NVECT      = false;
  jm_SCALE      = false;
  Aspect_Square = true;
  PLOT_LINE     = false;
  PLOT_CROSS    = false;
  PLOT_BOX      = true;
  PLOT_INTG     = false;
  PLOT_DIFF     = false;
  PLOT_RMS      = false;
  PLOT_SHADE    = false;
  PLOT_AXIS     = false;
  PLOT_ZMX      = 0;
  PLOT_ZMY      = 0;
  PLOT_ZOOM     = 0;

  plotmode      = _SCAT;      //VECTOR or SCATTER
  tick_int_x    = 0;          //to show axis: tick_in_x & y = 10, PLOT_AXIS = true
  tick_int_y    = 0;
}


void graph_setupmemory(void) {
  int i;
  if(telltale != MEM_INITIALIZED) {
    gr_x = (graphtype*)malloc(LIM * sizeof(graphtype)); 
    memset(gr_x, 0,           LIM * sizeof(graphtype));
    gr_y = (graphtype*)malloc(LIM * sizeof(graphtype)); 
    memset(gr_y, 0,           LIM * sizeof(graphtype));
    telltale = MEM_INITIALIZED;
    ix_count = 0;
  }
  if ((telltale != MEM_INITIALIZED) || (gr_x == NULL || gr_y == NULL)) { 
  #ifdef PC_BUILD
     moreInfoOnError("In function graph_setupmemory:", "error allocating memory for graph!", NULL, NULL);
     exit(1);
  #endif
  } else
  {
  #ifdef PC_BUILD
    printf("^^@@ Two arrays of %u bytes each created, i.e. %u blocks total\n",(uint32_t) (LIM * sizeof(graphtype)), (uint32_t)(2 * LIM * sizeof(graphtype) / 4));
  #endif
  }
  
  if((telltale==MEM_INITIALIZED) && (gr_x != NULL) && (gr_y != NULL)){
    for (i = 0; i < LIM; ++i) { 
      #if defined STATDEBUG && defined PC_BUILD
        gr_x[i] = 85.1f;
        gr_y[i] = 170.1f; 
        if(fabs(gr_x[i]-85.1f) > 0.00001f || fabs(gr_y[i]-170.1f) > 0.00001f) printf("### ERROR MEMORY TEST A %i: x should be 85.1 and is %f; y should be 170.1 and is %f\n",i,gr_x[i],gr_y[i]);
        gr_x[i] = 170.1;
        gr_y[i] = 85.1; 
        if(fabs(gr_x[i]-170.1f) > 0.00001f || fabs(gr_y[i]-85.1f) > 0.00001f) printf("### ERROR MEMORY TEST B %i: x should be 170.1 and is %f; y should be 85.1 and is %f\n",i,gr_x[i],gr_y[i]);
      #endif
      gr_x[i] = 0.0f;
      gr_y[i] = 0.0f; 
      #if defined STATDEBUG && defined PC_BUILD
        if(gr_x[i]!=0.0f || gr_y[i]!=0.0f) printf("### ERROR MEMORY TEST C %i: x should be 170.1 and is %f; y should be 85.1 and is %f\n",i,gr_x[i],gr_y[i]);
      #endif
    }
  } else {
    #if defined STATDEBUG && defined PC_BUILD
      printf("### ERROR MEMORY TEST D NOT INITIALIZED\n");
    #endif //PC_BUILD
  }
}

void graph_end(void) {
  free(gr_x);
  free(gr_y);
  telltale = 0;
}



//Pauli volunteered this fuction, rev 1 2021-10-10
#if DECDPUN != 3
#error DECDPUN must be 3
#endif
float fnRealToFloat(const real_t *r){
    int s = 0;
    int j, n, e;
    static const float exps[] = {
        1.e-45, 1.e-44, 1.e-43, 1.e-42, 1.e-41, 1.e-40, 1.e-39, 1.e-38,
        1.e-37, 1.e-36, 1.e-35, 1.e-34, 1.e-33, 1.e-32, 1.e-31, 1.e-30,
        1.e-29, 1.e-28, 1.e-27, 1.e-26, 1.e-25, 1.e-24, 1.e-23, 1.e-22,
        1.e-21, 1.e-20, 1.e-19, 1.e-18, 1.e-17, 1.e-16, 1.e-15, 1.e-14,
        1.e-13, 1.e-12, 1.e-11, 1.e-10, 1.e-9,  1.e-8,  1.e-7,  1.e-6,
        1.e-5,  1.e-4,  1.e-3,  1.e-2,  1.e-1,  1.e0,   1.e1,   1.e2,
        1.e3,   1.e4,   1.e5,   1.e6,   1.e7,   1.e8,   1.e9,   
        1.e10,  1.e11,  1.e12,  1.e13,  1.e14,  1.e15,  1.e16,  1.e17,
        1.e18,  1.e19,  1.e20,  1.e21,  1.e22,  1.e23,  1.e24,  1.e25,
        1.e26,  1.e27,  1.e28,  1.e29,  1.e30,  1.e31,  1.e32,  1.e33,
        1.e34,  1.e35,  1.e36,  1.e37,  1.e38
    };

    if (realIsSpecial(r)) {
        if (realIsNaN(r))
            return 0. / 0.;
        goto infinite;
    }
    if (realIsZero(r))
        goto zero;

    j = (r->digits + DECDPUN-1) / DECDPUN;
    while (j > 0)
        if ((s = r->lsu[--j]) != 0)
            break;
    for (n = 0; --j >= 0 && n < 2; n++)
        s = (s * 1000) + r->lsu[j];
    if (realIsNegative(r))
        s = -s;
    e = r->exponent + (j + 1) * DECDPUN;
    if (e < -45) {
zero:
        return realIsPositive(r) ? 0. : -0.;
    }
    if (e > 38) {
infinite:
        if (realIsPositive(r))
            return 1. / 0.;
        return -1. / 0.;
    }
    return (float)s * exps[e + 45];
}

//#define realToReal39(source, destination) decQuadFromNumber ((real39_t *)(destination), source, &ctxtReal39)

void realToFloat(const real_t *vv, graphtype *v) {
  *v = fnRealToFloat(vv);
  #ifdef PC_BUILD
    char tmpString1[100];                      //allow for 75 digits
    realToString(vv, tmpString1);
    //printf("Convert vv REAL %s --> Float %f\n",tmpString1,*v);
  #endif
}

void realToDouble1(const real_t *vv, double *v) {
  *v = fnRealToFloat(vv);
  #ifdef PC_BUILD
    char tmpString1[100];                      //allow for 75 digits
    realToString(vv, tmpString1);
    //printf("Convert vv REAL %s --> Double %lf\n",tmpString1,*v);
  #endif
}


void graph_sigmaplus(int8_t plusminus, real_t *xx, real_t *yy) {    //Called from STAT module from fnSigma(), to store the x,y pair to the memory structure.
  int16_t cnt;
  graphtype x,y;
  realToInt32(SIGMA_N, cnt);

  if(cnt <= LIM) {
    if(jm_VECT || jm_NVECT) {
      plotmode = _VECT;
    } else {
      plotmode = _SCAT;
    }

    if(telltale != MEM_INITIALIZED) {
      graph_setupmemory();
    }

    //Convert from X register to graphtype
    realToFloat(yy,&y);
    //printf("y=%f ",y);

    //Convert from X register to graphtype
    realToFloat(xx, &x);
    //printf("x=%f ",x);

    #ifndef TESTSUITE_BUILD
    //export_xy_to_file(x,y);                  //Write to CSV file
    #endif

    if(plotmode == _VECT ) {
      ix_count++;                              //Only used for VECT
      cnt = ix_count;
    } else {
      ix_count = cnt;                          //ix_count increments in VECT with Σ-, where SIGMA_N decrements with Σ- 
                                               //if VECT is changed mid-process, it will cause x_count to assume SIGMA_N, which  will throw away the last values stored.
      #if defined STATDEBUG && defined PC_BUILD
        printf("Count: %s, %d\n",tmpString,cnt);
      #endif
    }
    //printf("Adding to graph table[%d] = x:%f y:%f\n",cnt,x,y);

    if(plusminus == 1) {
      gr_x[cnt-1]=x;
      gr_y[cnt-1]=y;
      #if defined STATDEBUG && defined PC_BUILD
        printf("Index: [%d]=(%f,%f)\n",cnt-1,x,y);
      #endif
    } else {
      if(plusminus == -1) {
        if(plotmode == _VECT ) {
          gr_x[cnt-1]=-x;
          gr_y[cnt-1]=-y;
          #if defined STATDEBUG && defined PC_BUILD
            printf("Index: [%d]=(%f,%f)\n",cnt-1,-x,-y);
          #endif
        } else {
          // Non-vector mode TODO
        }
      }
    }
  }  
  else {
      printf("In function SUM+ or SUM-:%u There is insufficient plot memory available, but stats totals still accumulate!\n",plusminus);
  }
}


graphtype grf_x(int i) {
  if (jm_NVECT) {
    return gr_y[i];
  }
  else {
    return gr_x[i];
  }
}

graphtype grf_y(int i) {
  if (jm_NVECT) {
    return gr_x[i];
  }
  else {
    return gr_y[i];
  }
}


#ifndef TESTSUITE_BUILD
  int16_t screen_window_x(graphtype x_min, graphtype x, graphtype x_max) {
    int16_t temp; graphtype tempr;
    if (Aspect_Square) {
      tempr = ((x-x_min)/(x_max-x_min)*(graphtype)(SCREEN_HEIGHT_GRAPH-1.0f));
      temp = (int16_t) tempr;
      if (temp>SCREEN_HEIGHT_GRAPH-1) {
        temp=SCREEN_HEIGHT_GRAPH-1;
      }
      else if (temp<0) {temp=0;}
      return temp+SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;
    } 
    else {  //FULL SCREEN
      tempr = ((x-x_min)/(x_max-x_min)*(graphtype)(SCREEN_WIDTH_GRAPH-1.0f));
      temp = (int16_t) tempr;
      //printf("--> %d (%f %f)  ",temp, x_min,x_max);
      if (temp>SCREEN_WIDTH_GRAPH-1) {
        temp=SCREEN_WIDTH_GRAPH-1;
      }
      else if (temp<0) {temp=0;}
      //printf("--> %d \n",temp);
      #ifdef PC_BUILD
        if(temp<0 || temp>399) {printf("In function screen_window_x X EXCEEDED %d",temp);}
      #endif
      return temp;
    }
  }


  int16_t screen_window_y(graphtype y_min, graphtype y, graphtype y_max) {
    int16_t temp, minn;
    graphtype tempr;
    if (!Aspect_Square) minn = SCREEN_NONSQ_HMIN;
    else minn = 0;

    tempr = ((y-y_min)/(y_max-y_min)*(graphtype)(SCREEN_HEIGHT_GRAPH-1.0f - minn));
    temp = (int16_t) tempr;
    if (temp>SCREEN_HEIGHT_GRAPH-1 - minn) {
      temp=SCREEN_HEIGHT_GRAPH-1 - minn;
    }
    else if (temp<0) {temp=0;}

    #ifdef PC_BUILD
      if(SCREEN_HEIGHT_GRAPH-1 - temp<0 || SCREEN_HEIGHT_GRAPH-1 - temp>239) {printf("In function screen_window_y Y EXCEEDED %d %d",temp,SCREEN_HEIGHT_GRAPH-1 - temp);}
    #endif
    return (SCREEN_HEIGHT_GRAPH-1 - temp);
  }
#endif


void placePixel(uint32_t x, uint32_t y) {
#ifndef TESTSUITE_BUILD
  uint32_t minn;
  if (!Aspect_Square) minn = SCREEN_NONSQ_HMIN;
  else minn = 0;
    
  if(x<SCREEN_WIDTH_GRAPH && x>0 && y<SCREEN_HEIGHT_GRAPH && y>1+minn) {
    setBlackPixel(x,y);
  }
#endif //!TESTSUITE_BUILD
}


void removePixel(uint32_t x, uint32_t y) {
#ifndef TESTSUITE_BUILD
  uint32_t minn;
  if (!Aspect_Square) minn = SCREEN_NONSQ_HMIN;
  else minn = 0;

  if(x<SCREEN_WIDTH_GRAPH && x>0 && y<SCREEN_HEIGHT_GRAPH && y>1+minn) {
    setWhitePixel(x,y);
  }
#endif //!TESTSUITE_BUILD
}


void clearScreenPixels(void) {
#ifndef TESTSUITE_BUILD
  if (Aspect_Square) {
    lcd_fill_rect(SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH, 0, SCREEN_HEIGHT_GRAPH, SCREEN_HEIGHT_GRAPH, 0);
    lcd_fill_rect(0, Y_POSITION_OF_REGISTER_T_LINE, SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH, 171-5-Y_POSITION_OF_REGISTER_T_LINE+1, 0);
    lcd_fill_rect(19, 171-5, SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH-19+1, 5, 0);
  }
  else
    lcd_fill_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT_GRAPH, 0);
#endif //!TESTSUITE_BUILD
}


#ifndef TESTSUITE_BUILD
void plotcross(uint16_t xn, uint8_t yn) {              // Plots line from xo,yo to xn,yn; uses temporary x1,y1
  placePixel(xn,yn);                                   //   PLOT a cross
  placePixel(xn-1,yn-1);
  placePixel(xn-1,yn+1);
  placePixel(xn+1,yn-1);
  placePixel(xn+1,yn+1);
  placePixel(xn-2,yn-2);
  placePixel(xn-2,yn+2);
  placePixel(xn+2,yn-2);
  placePixel(xn+2,yn+2);
}


void plotbox(uint16_t xn, uint8_t yn) {                // Plots line from xo,yo to xn,yn; uses temporary x1,y1
  placePixel(xn-2,yn-2);                               //   PLOT a box
  placePixel(xn-2,yn-1);
  placePixel(xn-1,yn-2);
  placePixel(xn-2,yn+2);
  placePixel(xn-2,yn+1);
  placePixel(xn-1,yn+2);
  placePixel(xn+2,yn-2);
  placePixel(xn+1,yn-2);
  placePixel(xn+2,yn-1);
  placePixel(xn+2,yn+2);
  placePixel(xn+2,yn+1);
  placePixel(xn+1,yn+2);
}


static void plotrect(uint16_t a, uint8_t b, uint16_t c, uint8_t d) {                // Plots rectangle from xo,yo to xn,yn; uses temporary x1,y1
  plotline(a, b, c, b);
  plotline(a, b, a, d);
  plotline(c, d, c, b);
  plotline(c, d, a, d);
}


void plotbox_fat(uint16_t xn, uint8_t yn) {                                         // Plots line from xo,yo to xn,yn; uses temporary x1,y1
  plotrect(xn-3,yn-3,xn+3,yn+3);
  plotrect(xn-2,yn-2,xn+2,yn+2);
}
#endif //!TESTSUITE_BUILD


void plotline(uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn) {                   // Plots line from xo,yo to xn,yn; uses temporary x1,y1
   pixelline(xo,yo,xn,yn,1);
 }

void plotline2(uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn) {                   // Plots line from xo,yo to xn,yn; uses temporary x1,y1
#define offset 0.55f
   pixelline(xo,yo,xn,yn,1);
   pixelline(xo-1,yo,xn-1,yn,1);
   pixelline(xo,yo-1,xn,yn-1,1);
 }






/*
void pixelline_org(uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn, bool_t vmNormal) { // Plots line from xo,yo to xn,yn; uses temporary x1,y1
    uint16_t x1;  //range 0-399
    uint8_t  y1;  //range 0-239
    #if defined STATDEBUG_VERBOSE && defined PC_BUILD
      printf("pixelline: xo,yo,xn,yn: %d %d   %d %d \n",xo,yo,xn,yn);
    #endif
    if(xo == xn && yo == yn) {
      if(vmNormal) placePixel(xn,yn); else removePixel(xn,yn);
      return;
    } 
    else
    if(xo > xn) {
      for(x1=xo; x1!=xn; x1-=1) {
        y1 = yo + (x1-xo)*(yn-yo)/(xn-xo);
        if(vmNormal) placePixel(x1,y1); else removePixel(x1,y1);
      }
    } 
    else if(xo < xn) {
      for(x1=xo; x1!=xn; x1+=1) {
        y1 = yo + (x1-xo)*(yn-yo)/(xn-xo);
        if(vmNormal) placePixel(x1,y1); else removePixel(x1,y1);
      }
    }

    if(yo > yn) {
      for(y1=yo; y1!=yn; y1-=1) {
        x1 = xo + (y1-yo)*(xn-xo)/(yn-yo);
        if(vmNormal) placePixel(x1,y1); else removePixel(x1,y1);
      }
    } 
    else if(yo < yn) {
      for(y1=yo; y1!=yn; y1+=1) {
        x1 = xo + (y1-yo)*(xn-xo)/(yn-yo);
        if(vmNormal) placePixel(x1,y1); else removePixel(x1,y1);
      }
    }
  }
*/



/*
//Bresenham variable width - not really working well. glitchy.
  //http://members.chello.at/~easyfilter/bresenham.html
void pixelline_width(uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn, bool_t vmNormal)                 //Bresenham line width: plotLineWidth
{ 
#define thres 0.0f
float wd = 1.0f;
   int dx = abs(xn-xo), sx = xo < xn ? 1 : -1; 
   int dy = abs(yn-yo), sy = yo < yn ? 1 : -1; 
   int err = dx-dy, e2, x2, y2;                                // error value e_xy
   float ed = dx+dy == 0 ? 1 : sqrt((float)dx*dx+(float)dy*dy);
   
   for (wd = (wd+1)/2; ; ) {                                   // pixel loop
      if(abs(err-dx+dy)/ed-wd+1>thres) {if(vmNormal) placePixel(xo,yo); else removePixel(xo,yo);}
      e2 = err; x2 = xo;
      if (2*e2 >= -dx) {                                           // x step
         for (e2 += dy, y2 = yo; e2 < ed*wd && (yn != y2 || dx > dy); e2 += dx)
            if(abs(e2)/ed-wd+1>thres) {if(vmNormal) placePixel(xo, y2 += sy); else removePixel(xo, y2 += sy);}
         if (xo == xn) break;
         e2 = err; err -= dy; xo += sx; 
      } 
      if (2*e2 <= dy) {                                            // y step
         for (e2 = dx-e2; e2 < ed*wd && (xn != x2 || dx < dy); e2 += dy)
            if(abs(e2)/ed-wd+1>thres) {if(vmNormal) placePixel(x2 += sx, yo); else removePixel(x2 += sx, yo);}
         if (yo == yn) break;
         err += dx; yo += sy; 
      }
   }
}
*/







//Exhange the name of this routine with pixelline() above to try Bresenham
void pixelline(uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn, bool_t vmNormal) { // Plots line from xo,yo to xn,yn; uses temporary x1,y1
    #if defined STATDEBUG_VERBOSE && defined PC_BUILD
      printf("pixelline: xo,yo,xn,yn: %d %d   %d %d \n",xo,yo,xn,yn);
    #endif

   //Bresenham line drawing: Pauli's link. Also here: http://forum.6502.org/viewtopic.php?f=10&t=2247&start=555
   int dx =  abs((int)(xn)-(int)(xo)), sx = xo<xn ? 1 : -1;
   int dy = -abs((int)(yn)-(int)(yo)), sy = yo<yn ? 1 : -1;
   int err = dx+dy, e2; /* error value e_xy */
 
   for(;;){  /* loop */
      if(vmNormal) placePixel(xo,yo); else removePixel(xo,yo);
      if (xo==xn && yo==yn) break;
      e2 = 2*err;
      if (e2 >= dy) { err += dy; xo += sx; } /* e_xy+e_x > 0 */
      if (e2 <= dx) { err += dx; yo += sy; } /* e_xy+e_y < 0 */
   }


/* Another Bresenham example
    //Bresenham line draw algo. https://stackoverflow.com/questions/62651042/how-to-implement-bresenhams-line-algorithm-in-c-when-trying-to-draw-a-line-in-b
    int dx = abs((int)(xn - xo)), sx = xo < xn ? 1 : -1;
    int dy = abs((int)(yn - yo)), sy = yo < yn ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;
    for (;;) {
      // setPixel(xo,yo,Matrix);
      if(vmNormal) placePixel(xo,yo); else removePixel(xo,yo);
      if (xo == xn && yo == yn)
        break;
      e2 = err;
      if (e2 > -dx) {
        err -= dy;
        xo += sx;
      }
      if (e2 < dy) {
        err += dx;
        yo += sy;
      }
    }
*/


}





void force_refresh1(void) {
#ifdef PC_BUILD
  gtk_widget_queue_draw(screen);
  //FULL UPDATE (UGLY)
  #ifdef FULLUPDATE
     refresh_gui();
  #endif //FULLUPDATE
#endif //PC_BUILD
#if DMCP_BUILD
  lcd_forced_refresh();
#endif //PC_BUILD
}


void graphAxisDraw (void){
  #ifndef TESTSUITE_BUILD
  uint32_t cnt;

  clearScreenPixels();
  //GRAPH ZERO AXIS
  yzero = screen_window_y(y_min,0,y_max);
  xzero = screen_window_x(x_min,0,x_max);

  uint32_t minnx, minny;
  if (!Aspect_Square) {
    minny = SCREEN_NONSQ_HMIN;
    minnx = 0;
  }
  else {
    minny = 0;
    minnx = SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;
  }


  //SEPARATING LINE IF SQUARE
  cnt = minny;
  while (cnt!=SCREEN_HEIGHT_GRAPH) { 
    if (Aspect_Square) {
        setBlackPixel(minnx-1,cnt);   
        setBlackPixel(minnx-2,cnt);   
    }
    cnt++; 
  }

  #if defined STATDEBUG && defined PC_BUILD
    printf("xzero=%d yzero=%d   \n",(int)xzero,(int)yzero);
  #endif

  graphtype x; 
  graphtype y;

  if( PLOT_AXIS && !(yzero == SCREEN_HEIGHT_GRAPH-1 || yzero == minny)) {
    //DRAW XAXIS
    if (Aspect_Square) {
      cnt = minnx;
    } else {
      cnt = 0;
    }

    while (cnt!=SCREEN_WIDTH_GRAPH-1) { 
      setBlackPixel(cnt,yzero); 
      #if defined STATDEBUG && defined PC_BUILD
        printf("cnt=%d   \n",(int)cnt);
      #endif
      cnt++; 
    }

   force_refresh1();

   if(0<x_max && 0>x_min) {
     for(x=0; x<=x_max; x+=tick_int_x) {                         //draw x ticks
        #if defined STATDEBUG && defined PC_BUILD
          printf(">> x=%d   \n",(int)x);
        #endif
        cnt = screen_window_x(x_min,x,x_max);
        //printf(">>>>>A %f %d ",x,cnt);
          setBlackPixel(cnt,min(yzero+1,SCREEN_HEIGHT_GRAPH-1)); //tick
          setBlackPixel(cnt,max(yzero-1,minny));                 //tick
       }
      for(x=0; x>=x_min; x+=-tick_int_x) {                       //draw x ticks
        cnt = screen_window_x(x_min,x,x_max);
        //printf(">>>>>B %f %d ",x,cnt);
          setBlackPixel(cnt,min(yzero+1,SCREEN_HEIGHT_GRAPH-1)); //tick
          setBlackPixel(cnt,max(yzero-1,minny));                 //tick
       }
      for(x=0; x<=x_max; x+=tick_int_x*5) {                      //draw x ticks
        cnt = screen_window_x(x_min,x,x_max);
          setBlackPixel(cnt,min(yzero+2,SCREEN_HEIGHT_GRAPH-1)); //tick
          setBlackPixel(cnt,max(yzero-2,minny));                 //tick
          setBlackPixel(cnt,min(yzero+3,SCREEN_HEIGHT_GRAPH-1)); //tick
          setBlackPixel(cnt,max(yzero-3,minny));                 //tick
       }
      for(x=0; x>=x_min; x+=-tick_int_x*5) {                     //draw x ticks
        cnt = screen_window_x(x_min,x,x_max);
          setBlackPixel(cnt,min(yzero+2,SCREEN_HEIGHT_GRAPH-1)); //tick
          setBlackPixel(cnt,max(yzero-2,minny));                 //tick
          setBlackPixel(cnt,min(yzero+3,SCREEN_HEIGHT_GRAPH-1)); //tick
          setBlackPixel(cnt,max(yzero-3,minny));                 //tick
       }
   } else {
     for(x=x_min; x<=x_max; x+=tick_int_x) {                     //draw x ticks
        #if defined STATDEBUG && defined PC_BUILD
          printf(">>>x=%d   \n",(int)x);
        #endif
        cnt = screen_window_x(x_min,x,x_max);
        //printf(">>>>>A %f %d ",x,cnt);
          setBlackPixel(cnt,min(yzero+1,SCREEN_HEIGHT_GRAPH-1)); //tick
          setBlackPixel(cnt,max(yzero-1,minny));                 //tick
       }
      for(x=x_min; x<=x_max; x+=tick_int_x*5) {                  //draw x ticks
        #if defined STATDEBUG && defined PC_BUILD
          printf(">>>>x=%d   \n",(int)x);
        #endif
        cnt = screen_window_x(x_min,x,x_max);
          setBlackPixel(cnt,min(yzero+2,SCREEN_HEIGHT_GRAPH-1)); //tick
          setBlackPixel(cnt,max(yzero-2,minny));                 //tick
          setBlackPixel(cnt,min(yzero+3,SCREEN_HEIGHT_GRAPH-1)); //tick
          setBlackPixel(cnt,max(yzero-3,minny));                 //tick
       }
     }
   }


  if( PLOT_AXIS && !(xzero == SCREEN_WIDTH-1 || xzero == minnx)) {
    //Write North arrow
    if(jm_NVECT) {
      showString("N", &standardFont, xzero-4, minny+14, vmNormal, true, true);
      tmpString[0]=(char)((uint8_t)0x80 | (uint8_t)0x22);
      tmpString[1]=0x06;
      tmpString[2]=0;
      showString(tmpString, &standardFont, xzero-4, minny+0, vmNormal, true, true);
    }

    //DRAW YAXIS
    lcd_fill_rect(xzero,minny,1,SCREEN_HEIGHT_GRAPH-minny,0xFF);

    force_refresh1();
    if(0<y_max && 0>y_min) {
      for(y=0; y<=y_max; y+=tick_int_y) {                     //draw y ticks
        #if defined STATDEBUG && defined PC_BUILD
          printf(">>y=%d   \n",(int)y);
        #endif
        cnt = screen_window_y(y_min,y,y_max);
        setBlackPixel(max(xzero-1,0),cnt);                    //tick
        setBlackPixel(min(xzero+1,SCREEN_WIDTH_GRAPH-1),cnt); //tick
      }  
      for(y=0; y>=y_min; y+=-tick_int_y) {                    //draw y ticks
        #if defined STATDEBUG && defined PC_BUILD
          printf(">>>y=%d   \n",(int)y);
        #endif
        cnt = screen_window_y(y_min,y,y_max);
        setBlackPixel(max(xzero-1,0),cnt);                    //tick
        setBlackPixel(min(xzero+1,SCREEN_WIDTH_GRAPH-1),cnt); //tick
      }  
      for(y=0; y<=y_max; y+=tick_int_y*5) {                   //draw y ticks
        #if defined STATDEBUG && defined PC_BUILD
          printf(">>>>y=%d   \n",(int)y);
        #endif
        cnt = screen_window_y(y_min,y,y_max);
        setBlackPixel(max(xzero-2,0),cnt);                    //tick
        setBlackPixel(min(xzero+2,SCREEN_WIDTH_GRAPH-1),cnt); //tick
        setBlackPixel(max(xzero-3,0),cnt);                    //tick
        setBlackPixel(min(xzero+3,SCREEN_WIDTH_GRAPH-1),cnt); //tick
      }  
      for(y=0; y>=y_min; y+=-tick_int_y*5) {                  //draw y ticks
        #if defined STATDEBUG && defined PC_BUILD
          printf(">>>>>y=%d   \n",(int)y);
        #endif
        cnt = screen_window_y(y_min,y,y_max);
        setBlackPixel(max(xzero-2,0),cnt);                    //tick
        setBlackPixel(min(xzero+2,SCREEN_WIDTH_GRAPH-1),cnt); //tick
        setBlackPixel(max(xzero-3,0),cnt);                    //tick
        setBlackPixel(min(xzero+3,SCREEN_WIDTH_GRAPH-1),cnt); //tick
      }  
    } else {
      for(y=y_min; y<=y_max; y+=tick_int_y) {                 //draw y ticks
        #if defined STATDEBUG && defined PC_BUILD
          printf(">>>>>>>y=%d   \n",(int)y);
        #endif
        cnt = screen_window_y(y_min,y,y_max);
        setBlackPixel(max(xzero-1,0),cnt);                    //tick
        setBlackPixel(min(xzero+1,SCREEN_WIDTH_GRAPH-1),cnt); //tick
      }  
      for(y=y_min; y<=y_max; y+=tick_int_y*5) {               //draw y ticks
        #if defined STATDEBUG && defined PC_BUILD
          printf(">>>>>>>>y=%d   \n",(int)y);
        #endif
        cnt = screen_window_y(y_min,y,y_max);
        setBlackPixel(max(xzero-2,0),cnt);                    //tick
        setBlackPixel(min(xzero+2,SCREEN_WIDTH_GRAPH-1),cnt); //tick
        setBlackPixel(max(xzero-3,0),cnt);                    //tick
        setBlackPixel(min(xzero+3,SCREEN_WIDTH_GRAPH-1),cnt); //tick
      }  
    }
  }
  force_refresh1();
  #endif
}


float auto_tick(float tick_int_f) {
  return tick_int_f;
  //Obtain scaling of ticks, to about 20 intervals left to right.
  //graphtype tick_int_f = (x_max-x_min)/20;                                                 //printf("tick interval:%f ",tick_int_f);
  snprintf(tmpString, TMP_STR_LENGTH, "%.1e", tick_int_f);
  char tx[4];
  tx[0] = tmpString[0]; //expecting the form 6.5e+01
  tx[1] = tmpString[1]; //the decimal radix is copied over, so region setting should not affect it
  tx[2] = tmpString[2]; //the exponent is stripped
  tx[3] = 0;
  //printf("tick0 %f orgstr %s tx %s \n",tick_int_f, tmpString, tx);
  tick_int_f = strtof (tx, NULL);
  //tick_int_f = (float)(tx[0]-48) + (float)(tx[2]-48)/10.0f;
  //printf("tick1 %f orgstr %s tx %s \n",tick_int_f, tmpString, tx);

  if(tick_int_f > 0   && tick_int_f <=  0.3)  {tmpString[0] = '0'; tmpString[2]='2'; } else
  if(tick_int_f > 0.3 && tick_int_f <=  0.6)  {tmpString[0] = '0'; tmpString[2]='5'; } else
  if(tick_int_f > 0.6 && tick_int_f <=  1.3)  {tmpString[0] = '1'; tmpString[2]='0'; } else
  if(tick_int_f > 1.3 && tick_int_f <=  1.7)  {tmpString[0] = '1'; tmpString[2]='5'; } else
  if(tick_int_f > 1.7 && tick_int_f <=  3.0)  {tmpString[0] = '2'; tmpString[2]='0'; } else
  if(tick_int_f > 3.0 && tick_int_f <=  6.5)  {tmpString[0] = '5'; tmpString[2]='0'; } else
  if(tick_int_f > 6.5 && tick_int_f <=  9.9)  {tmpString[0] = '7'; tmpString[2]='5'; }

  if (roundedTicks) tick_int_f = strtof (tmpString, NULL);                                        //printf("string:%s converted:%f \n",tmpString, tick_int_f);

  //printf("tick2 %f str %s tx %s \n",tick_int_f, tmpString, tx);
  return tick_int_f;
}

void graph_axis (void){
  #ifndef TESTSUITE_BUILD
    graph_dx = 0; //XXX override manual setting from GRAPH to auto, temporarily. Can program these to fixed values.
    graph_dy = 0;

    if(graph_dx == 0) {
      tick_int_x = auto_tick((x_max-x_min)/20);
    } else {
      tick_int_x = graph_dx;
    }

    if(graph_dy == 0) {
      tick_int_y = auto_tick((y_max-y_min)/20);
    } else {
      tick_int_y = graph_dy;
    }
  #endif //TESTSUITE_BUILD
  graphAxisDraw();
}


char * radixProcess(const char * ss) {  //  .  HIERDIE WERK GLAD NIE
  int8_t ix = 0, iy = 0;
  tmp_names1[0]=0;
  while( ss[ix] != 0 ){
    if(ss[ix]==',' || ss[ix]=='.') {
      tmp_names1[iy++] = getSystemFlag(FLAG_DECIMP) ? '.' : ',';
      tmp_names1[iy] = 0;
    }
    else
    if(ss[ix]=='#') {
      tmp_names1[iy++] = getSystemFlag(FLAG_DECIMP) ? ',' : ';';
      tmp_names1[iy] = 0;
    } 
    else {
      tmp_names1[iy++] = ss[ix];
      tmp_names1[iy] = 0;      
    }
    ix++;
  }
  return tmp_names1;
}


void eformat (char* s02, const char* s01, double inreal, uint8_t prec, const char* s05) {
  char s03[100];
  if(((fabs(inreal) > 1000000.0 || fabs(inreal) < 0.001)) && (inreal != 0.0)) {
    sprintf(s03,"%.*e",prec,inreal);
  } else {
    sprintf(s03,"%.*f",prec,inreal);
  }
  strcpy(s02,s01);
  if(inreal > 0) strcat(s02,"");  //in place of negative sign
  strcat(s02,eatSpacesMid(radixProcess(s03)));
  strcat(s02,s05);
}


void eformat_fix3 (char* s02, const char* s01, double inreal) {
  char *sign;
  char s03[100]; char s04[100];
  if(inreal<0.0) {
    sign = "-"; 
    inreal = -inreal;
  }
  else sign = " ";
  if(((fabs(inreal) > 100000000.0f || fabs(inreal) < 0.001f)) && (inreal != 0.0f)) {
    sprintf(s03,"%s%.3e",sign,inreal);
  } else {
    sprintf(s03,"%s%.3f",sign,inreal);
  }
  strcpy(s02,s01);

  if(inreal > 0) strcpy(s04," ");  //in place of negative sign
  strcat(s04,s03);
  strcat(s02,eatSpacesMid(radixProcess(s04)));
}



// Remove trailing zeroes from float strings
static char * eatZeroesEnd(const char * ss) {
  int8_t ix;
  strcpy(tmp_names1,ss);
  ix = stringByteLength(tmp_names1)-1;
  while( ix > 0 ){
    if(tmp_names1[ix]=='0' || tmp_names1[ix]==' ') {
      tmp_names1[ix]=0;
    } 
    else {
      break;
    }
    ix--;
  }
  if(tmp_names1[ix]=='.' || tmp_names1[ix]==',') {
    tmp_names1[ix]=0;
  } 
  return tmp_names1;
}


char * padEquals(const char * ss) {
  int8_t ix = 0, iy = 0;
  tmp_names1[0]=0;
  while( ss[ix] != 0 ){
    if(ss[ix]=='=') {
      tmp_names1[iy++] = STD_SPACE_PUNCTUATION[0];
      tmp_names1[iy++] = STD_SPACE_PUNCTUATION[1];
      tmp_names1[iy++] = '=';
      tmp_names1[iy++] = STD_SPACE_PUNCTUATION[0];
      tmp_names1[iy++] = STD_SPACE_PUNCTUATION[1];
      tmp_names1[iy] = 0;
    } 
    else {
      tmp_names1[iy++] = ss[ix];
      tmp_names1[iy] = 0;      
    }
    ix++;
  }
  return tmp_names1;
}


//parts taken from:
//  http://jkorpela.fi/c/eng.html
//  David Hoerl 
static char *eng(double value, int digits)
{
     double display, fract, old;
     int expof10;
     static char result[30], *sign;

//     assert(isnormal(value)); // could also return NULL

     if(value < 0.0) {
         sign = "-";
         value = -value;
     } else {
         sign = " ";
     }

     old = value;

     // correctly round to desired precision
     expof10 = lrint( floor( log10(value) ) );
     value *= pow(10.0, digits - 1 - expof10);

     fract = modf(value, &display);
     if(fract >= 0.5) display += 1.0;

     value = display * pow(10.0, expof10 - digits + 1);


     if(expof10 > 0)
         expof10 = (expof10/3)*3;
     else
         expof10 = ((-expof10+3)/3)*(-3);

     value *= pow(10.0, -expof10);
     if (value >= 1000.0) {
         value /= 1000.0;
         expof10 += 3;
     }
     else if(value >= 100.0)
         digits -= 2;
     else if(value >= 10.0)
         digits -= 1;

     if(isnan(old) || isinf(old)) sprintf(result,"%s%f",sign,old); else
     if(old>999.9       && old<100000.0) sprintf(result,"%s%.i",  sign,(int)old); else
     if(old>99.9999     && old<1000.0) sprintf(result,"%s%.i",  sign,(int)old); else
     if(old>9.99999     && old<100.0 ) sprintf(result,"%s%.*f", sign, 1+digits+1-3, old); else
     if(old>0.999999    && old<10.0  ) sprintf(result,"%s%.*f", sign, digits+2-3  , old); else
     if(old>0.0999999   && old<1.0   ) sprintf(result,"%s%.*f", sign, 2+digits+3-3, old); else
     if(old>0.00999999  && old<0.1   ) sprintf(result,"%s%.*f", sign, 1+digits+4-3, old); else
     if(old == 0.0)                    sprintf(result,"%s%.*f", " ",  digits-1    , old); else
     if(digits-1 <= 0)                   sprintf(result,"%s%.0fe%d", sign, value, expof10); else
                                       sprintf(result,"%s%.*fe%d", sign, digits-1, value, expof10);
     return result;
}


void eformat_eng2 (char* s02, const char* s01, double inreal, int8_t digits, const char* s05) {
  char s03[100];
  strcpy(s03,eng(inreal, digits));
  strcpy(s02,s01);
  strcat(s02,eatSpacesMid(radixProcess(s03)));
  strcat(s02,s05);
}


void graphPlotstat(uint16_t selection){
  #if defined STATDEBUG && defined PC_BUILD
    printf("#####>>> graphPlotstat: selection:%u:%s  lastplotmode:%u  lrSelection:%u lrChosen:%u\n",selection, getCurveFitModeName(selection), lastPlotMode, lrSelection, lrChosen);
  #endif //STATDEBUG
  #ifndef TESTSUITE_BUILD
  uint16_t  cnt, ix, statnum;
  uint16_t  xo, xn, xN; 
  uint8_t   yo, yn, yN;
  graphtype x; 
  graphtype y;

  statnum = 0;
//  graphAxisDraw();                        //Draw the axis on any uncontrolled scale to start. Maybe optimize by remembering if there is an image on screen Otherwise double axis draw.
graph_axis();
  plotmode = _SCAT;

  if(telltale == MEM_INITIALIZED && checkMinimumDataPoints(const_2)) {
    realToInt32(SIGMA_N, statnum);   
    #if defined STATDEBUG && defined PC_BUILD
      printf("statnum n=%d\n",statnum);
    #endif 
  }

  if(telltale == MEM_INITIALIZED && statnum >= 2) {
    //AUTOSCALE
    x_min = FLoatingMax;
    x_max = FLoatingMin;
    y_min = FLoatingMax;
    y_max = FLoatingMin;
    #if defined STATDEBUG && defined PC_BUILD
      printf("Axis0: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif


    //#################################################### vvv SCALING LOOP  vvv
    for(cnt=0; (cnt < LIM && cnt < statnum); cnt++) {
      #if defined STATDEBUG && defined PC_BUILD
        printf("Axis0a: x: %f y: %f   \n",grf_x(cnt), grf_y(cnt));   
      #endif
      if(grf_x(cnt) < x_min) {x_min = grf_x(cnt);}
      if(grf_x(cnt) > x_max) {x_max = grf_x(cnt);}
      if(grf_y(cnt) < y_min) {y_min = grf_y(cnt);}
      if(grf_y(cnt) > y_max) {y_max = grf_y(cnt);}
      #if defined STATDEBUG && defined PC_BUILD
        printf("Axis0b: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
      #endif
    }
    //##  ################################################## ^^^ SCALING LOOP ^^^
    #if defined STATDEBUG && defined PC_BUILD
      printf("Axis1a: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif
  
    //Check and correct if min and max is swapped
    if(x_min>0.0f && x_min > x_max) {x_min = x_min - (-x_max+x_min)* 1.1f;}
    if(x_min<0.0f && x_min > x_max) {x_min = x_min + (-x_max+x_min)* 1.1f;}
    #if defined STATDEBUG && defined PC_BUILD
      printf("Axis1b: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif
  
    //Always include the 0 axis
    if(!extentx) {
      if(x_min>0.0f && x_max>0.0f) {if(x_min<=x_max) {x_min = -0.05f*x_max;} else {x_min = 0.0f;}}
      if(x_min<0.0f && x_max<0.0f) {if(x_min>=x_max) {x_min = -0.05f*x_max;} else {x_max = 0.0f;}}
    }
    if(!extenty) {
      if(y_min>0.0f && y_max>0.0f) {if(y_min<=y_max) {y_min = -0.05f*y_max;} else {y_min = 0.0f;}}
      if(y_min<0.0f && y_max<0.0f) {if(y_min>=y_max) {y_min = -0.05f*y_max;} else {y_max = 0.0f;}}
    }
  
    //Cause scales to be the same    
    if(jm_SCALE) {
      x_min = min(x_min,y_min);
      x_max = max(x_max,y_max);
      y_min = x_min;
      y_max = x_max;
    }
  
    //Calc zoom scales    
    if(PLOT_ZMX != 0) {
      x_min = pow(2.0f,-PLOT_ZMX) * x_min;
      x_max = pow(2.0f,-PLOT_ZMX) * x_max;
    }
    if(PLOT_ZMY != 0) {
      y_min = pow(2.0f,-PLOT_ZMY) * y_min;
      y_max = pow(2.0f,-PLOT_ZMY) * y_max;      
    }
    #if defined STATDEBUG && defined PC_BUILD
      printf("Axis2: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif
  
    graphtype dx = x_max-x_min;
    graphtype dy = y_max-y_min;
  
    if (dy == 0.0f) {
      dy = 1.0f;
      y_max = y_min + dy/2.0f;
      y_min = y_max - dy;
    }
    if (dx == 0.0f) {
      dx = 1.0f;
      x_max = x_min + dx/2.0f;
      x_min = x_max - dx;
    }
    
    x_min = x_min - dx * zoomfactor * (pow(4.5f,(int8_t)(PLOT_ZOOM & 0x03)));
    y_min = y_min - dy * zoomfactor * (pow(4.5f,(int8_t)(PLOT_ZOOM & 0x03)));
    x_max = x_max + dx * zoomfactor * (pow(4.5f,(int8_t)(PLOT_ZOOM & 0x03)));
    y_max = y_max + dy * zoomfactor * (pow(4.5f,(int8_t)(PLOT_ZOOM & 0x03)));
    #if defined STATDEBUG && defined PC_BUILD
      printf("Axis3a: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif
  
    //graphAxisDraw();
    graph_axis();
    yn = screen_window_y(y_min,grf_y(0),y_max);
    xn = screen_window_x(x_min,grf_x(0),x_max);
    xN = xn;
    yN = yn;
    #if defined STATDEBUG && defined PC_BUILD
      printf("Axis3c: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif
  
  
    //#################################################### vvv MAIN GRAPH LOOP vvv
    for (ix = 0; (ix < LIM && ix < statnum); ++ix) {
      x = grf_x(ix);
      y = grf_y(ix);
      xo = xN;
      yo = yN;
      xN = screen_window_x(x_min,x,x_max);
      yN = screen_window_y(y_min,y,y_max);
    
      #if defined STATDEBUG && defined PC_BUILD
        printf("plotting graph table[%d] = x:%f y:%f xN:%d yN:%d ",ix,x,y,  xN,yN);
      #endif
    
      int16_t minN_y,minN_x;
      if (!Aspect_Square) {
        minN_y = SCREEN_NONSQ_HMIN; 
        minN_x = 0;
      }
      else {
        minN_y = 0; 
        minN_x = SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;
      }
      if(xN<SCREEN_WIDTH_GRAPH && xN>minN_x && yN<SCREEN_HEIGHT_GRAPH && yN>minN_y) {
        yn = yN;
        xn = xN;
    
        if(PLOT_CROSS) {
          #if defined STATDEBUG && defined PC_BUILD
            printf("Plotting cross to x=%d y=%d\n",xn,yn);
          #endif
          plotcross(xn,yn);
        }
    
        if(PLOT_BOX) {
          #if defined STATDEBUG && defined PC_BUILD
            printf("Plotting box to x=%d y=%d\n",xn,yn);
          #endif
          plotbox_fat(xn,yn);
        }
  
        if(PLOT_LINE) {
          #if defined STATDEBUG && defined PC_BUILD
            printf("Plotting line to x=%d y=%d\n",xn,yn);
          #endif
          plotline(xo, yo, xn, yn);
        }
      } 
      else {
        #ifdef PC_BUILD
          printf("Not plotted point: (%u %u) ",xN,yN);
          if(!(xN < SCREEN_WIDTH_GRAPH ))  printf("x>>%u ",SCREEN_WIDTH_GRAPH); else
          if(!(xN > minN_x              )) printf("x<<%u ",minN_x);
          if(!(yN < SCREEN_HEIGHT_GRAPH))  printf("y>>%u ",SCREEN_HEIGHT_GRAPH); else
          if(!(yN > 1+minN_y            )) printf("y<<%u ",1+minN_y);
          printf("\n");
          //printf("Not plotted: xN=%d<SCREEN_WIDTH_GRAPH=%d && xN=%d>minN_x=%d && yN=%d<SCREEN_HEIGHT_GRAPH=%d && yN=%d>1+minN_y=%d\n",xN,SCREEN_WIDTH_GRAPH,xN,minN_x,yN,SCREEN_HEIGHT_GRAPH,yN,1+minN_y);
        #endif
      }
    }
    //#################################################### ^^^ MAIN GRAPH LOOP ^^^          



  if(selection != 0) {
    processCurvefitSelection(selection, &RR, &SMI, &aa0, &aa1, &aa2);
    realMultiply(&RR,&RR,&RR,&ctxtReal39);
    drawline(selection, &RR, &SMI, &aa0, &aa1, &aa2);
  }


  } else {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "There is no statistical data available!");
      moreInfoOnError("In function graphPlotstat:", errorMessage, NULL, NULL);
    #endif
  }
#endif
}


#ifndef TESTSUITE_BUILD
  void drawline(uint16_t selection, real_t *RR, real_t *SMI, real_t *aa0, real_t *aa1, real_t *aa2){
    real_t XX,YY;
    if(!selection) return;
    #if defined STATDEBUG && defined PC_BUILD
      printf("#####>>> drawline: selection:%u:%s  lastplotmode:%u  lrSelection:%u lrChosen:%u\n",selection, getCurveFitModeName(selection), lastPlotMode, lrSelection, lrChosen);
    #endif //STATDEBUG
    float rr,smi,a0,a1,a2;
    int32_t nn;
    char ss[100], tt[100];


    realToFloat(RR , &rr );
    realToFloat(SMI, &smi);
    realToFloat(aa0, &a0 );
    realToFloat(aa1, &a1 );
    realToFloat(aa2, &a2 );
    realToInt32(SIGMA_N, nn);  

    #if defined STATDEBUG && defined PC_BUILD
      printf("plotting line: a2 %f a1 %f a0 %f\n",a2,a1,a0);
    #endif //STATDEBUG
    if((selection==0 && a2 == 0 && a1 == 0 && a0 == 0)) {
      #if defined STATDEBUG && defined PC_BUILD
        printf("return, nothing selected, zero parameters, nothing to draw\n");
      #endif //STATDEBUG
      return;
    }
    double  ix;
    uint16_t  xo = 0, xn, xN = 0; 
    uint8_t   yo = 0, yn, yN = 0;
    double    x = x_min; 
    double    y = 0.0;
    int16_t   Intervals = numberIntervals; //increase resulution in beginning and end of graph, to get a better starting and ending point in y
    uint16_t  iterations = 0;

    for (ix = (double)x_min; iterations < 2000 && x < x_max+(x_max-x_min)*0.5 && xN != SCREEN_WIDTH-1; iterations++) {       //Variable accuracy line plot
      
      xo = xN;
      yo = yN;

      for(int xx =1; xx<50; xx++) {
        x = ix + (double)(x_max-x_min)/(double)(Intervals) * (double)(1.0/(double)xx );
        if(USEFLOATING != 0) {
          //TODO create REAL from x (double) if REALS will be used
          sprintf(ss,"%f",x); stringToReal(ss,&XX,&ctxtReal39);
        }
        yIsFnx( USEFLOATING, selection, x, &y, a0, a1, a2, &XX, &YY, RR, SMI, aa0, aa1, aa2);
        xN = screen_window_x(x_min,(graphtype)x,x_max);
        yN = screen_window_y(y_min,(graphtype)y,y_max);
        if(abs((int)yN-(int)yo)<=2 && abs((int)xN-(int)xo)<=2) break;
      }
      ix = x;

      if(iterations > 0) {  //Allow for starting values to accumulate in the registers at ix = 0
        #if defined STATDEBUG && defined PC_BUILD
          printf("plotting graph: jm:%i iter:%u ix:%f I.vals:%u ==>xmin:%f (x:%f) xmax:%f ymin:%f (y:%f) ymax:%f xN:%d yN:%d \n",jumpMonitor,iterations,ix,Intervals,x_min,x,x_max,y_min,y,y_max,  xN,yN);
        #endif
        int16_t minN_y,minN_x;
        if (!Aspect_Square) {minN_y = SCREEN_NONSQ_HMIN; minN_x = 0;}
        else {minN_y = 0; minN_x = SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;}

        #define tol 4
        if(xN<SCREEN_WIDTH_GRAPH && xN>minN_x && yN<SCREEN_HEIGHT_GRAPH-tol && yN>minN_y) {
          yn = yN;
          xn = xN;
          #if defined STATDEBUG_VERBOSE && defined PC_BUILD
            printf("Plotting box to x=%d y=%d\n",xn,yn);
          #endif
          if(fittedcurveboxes) plotbox(xn,yn);
          if(xo < SCREEN_WIDTH_GRAPH && xo > minN_x && yo < SCREEN_HEIGHT_GRAPH-tol && yo > minN_y) {
            #if defined STATDEBUG_VERBOSE && defined PC_BUILD
              printf("Plotting line to x=%d y=%d\n",xn,yn);
            #endif
            plotline2(xo, yo, xn, yn);
          }
        } 
        else {
          #if defined STATDEBUG && defined PC_BUILD
            printf("Not plotted line: (%u %u) ",xN,yN);
            if(!(xN < SCREEN_WIDTH_GRAPH ))  printf("x>>%u ",SCREEN_WIDTH_GRAPH); else
            if(!(xN > minN_x              )) printf("x<<%u ",minN_x);
            if(!(yN < SCREEN_HEIGHT_GRAPH))  printf("y>>%u ",SCREEN_HEIGHT_GRAPH); else
            if(!(yN > 1+minN_y            )) printf("y<<%u ",1+minN_y);
            printf("\n");
          #endif //STATDEBUG
        }
      }
    }

    #define horOffsetR 109 //digit righ side aliognment
    #define autoinc 19 //text line spacing
    #define autoshift -5 //text line spacing
    #define horOffset 1 //labels from the left
    int16_t index = -1;
    if(selection!=0) {
      strcpy(ss,eatSpacesEnd(getCurveFitModeName(selection)));
      if(lrCountOnes(lrSelection)>1 && selection == lrChosen) strcat(ss,lrChosen == 0 ? "" : STD_SUP_ASTERISK);
        showString(ss, &standardFont, horOffset, Y_POSITION_OF_REGISTER_Z_LINE + autoinc * index++ -10 +autoshift, vmNormal, false, false);

      if(selection != CF_GAUSS_FITTING && selection != CF_CAUCHY_FITTING) {
        strcpy(ss,"y="); strcat(ss,getCurveFitModeFormula(selection)); showString(padEquals(ss), &standardFont, horOffset, Y_POSITION_OF_REGISTER_Z_LINE + autoinc * index++ -7 +autoshift, vmNormal, false, false);
      } else {
        strcpy(ss,"y="); strcat(ss,getCurveFitModeFormula(selection)); showString(          ss, &standardFont, horOffset, Y_POSITION_OF_REGISTER_Z_LINE + autoinc * index++ -7 +autoshift, vmNormal, false, false);        
      }
      
      if(softmenu[softmenuStack[0].softmenuId].menuItem != -MNU_PLOT_STAT) {
        sprintf(ss,"%d",(int)nn);              showString(padEquals(ss), &standardFont, horOffsetR - stringWidth(ss, &standardFont, false, false), Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index  -2 +autoshift, vmNormal, false, false);
        sprintf(ss, STD_SPACE_PUNCTUATION STD_SPACE_PUNCTUATION "n=");                     showString(padEquals(ss), &standardFont, horOffset, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++ -2 +autoshift, vmNormal, false, false);
      }

      if(selection != CF_ORTHOGONAL_FITTING) {
        eformat_eng2(ss,"",a0,3,"");           showString(padEquals(ss), &standardFont, horOffsetR - stringWidth(ss, &standardFont, false, false), Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index  -4 +autoshift, vmNormal, false, false);
        strcpy(ss,"a" STD_SUB_0 "=");          showString(padEquals(ss), &standardFont, horOffset, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++   -4 +autoshift, vmNormal, false, false);

        eformat_eng2(ss,"",a1,3,"");           showString(padEquals(ss), &standardFont, horOffsetR - stringWidth(ss, &standardFont, false, false), Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index  -1 +autoshift, vmNormal, false, false);
        strcpy(ss,"a" STD_SUB_1 "=");          showString(padEquals(ss), &standardFont, horOffset, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++   -1 +autoshift, vmNormal, false, false);

        if(selection == CF_PARABOLIC_FITTING || selection == CF_GAUSS_FITTING || selection == CF_CAUCHY_FITTING) { 
          eformat_eng2(ss,"",a2,3,"");         showString(padEquals(ss), &standardFont, horOffsetR - stringWidth(ss, &standardFont, false, false), Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index  -1 +autoshift, vmNormal, false, false);      
          strcpy(ss,"a" STD_SUB_2 "=");        showString(padEquals(ss), &standardFont, horOffset, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++   -1 +autoshift, vmNormal, false, false);
        }

        eformat(ss,"",rr,4,"");                showString(padEquals(ss), &standardFont, horOffsetR - stringWidth(ss, &standardFont, false, false), Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index  +2  +autoshift, vmNormal, false, false);      
        strcpy(ss,"r" STD_SUP_2 "=");          showString(padEquals(ss), &standardFont, horOffset, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++   +2 +autoshift, vmNormal, false, false);

        eformat_eng2(ss,"(",x_max,2,""); 
        eformat_eng2(tt,radixProcess("#"),y_max,2,")");
        strcat(tt,ss);                    nn = showString(padEquals(ss), &standardFont,160-2 - stringWidth(tt, &standardFont, false, false), Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index        +autoshift, vmNormal, false, false);      
        eformat_eng2(ss,radixProcess("#"),y_max,2,")");      showString(padEquals(ss), &standardFont,nn+3, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++      +autoshift, vmNormal, false, false);      
        eformat_eng2(ss,"(",x_min,2,"");  nn = showString(padEquals(ss), &standardFont,horOffset, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index    -2  +autoshift, vmNormal, false, false);      
        eformat_eng2(ss,radixProcess("#"),y_min,2,")");      showString(padEquals(ss), &standardFont,nn+3, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++  -2  +autoshift, vmNormal, false, false);      
        
      }
      else {
        eformat_fix3(ss,"",a0);                showString(padEquals(ss), &standardFont, horOffsetR - stringWidth(ss, &standardFont, false, false), Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index  -4 +autoshift, vmNormal, false, false);
        strcpy(ss,"a" STD_SUB_0 "=");          showString(padEquals(ss), &standardFont, horOffset, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++   -4 +autoshift, vmNormal, false, false);
        eformat_fix3(ss,"",a1);                showString(padEquals(ss), &standardFont, horOffsetR - stringWidth(ss, &standardFont, false, false), Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index  -1 +autoshift, vmNormal, false, false);
        strcpy(ss,"a" STD_SUB_1 "=");          showString(padEquals(ss), &standardFont, horOffset, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++   -1 +autoshift, vmNormal, false, false);
        if(softmenu[softmenuStack[0].softmenuId].menuItem == -MNU_PLOT_STAT) {
          if(nn>=30) {
            eformat_eng2(ss,"",smi,3,"");      showString(padEquals(ss), &standardFont, horOffsetR - stringWidth(ss, &standardFont, false, false), Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index  +1 +autoshift, vmNormal, false, false);
          }
          strcpy(ss,"s" STD_SUB_m STD_SUB_i "=");showString(padEquals(ss), &standardFont, horOffset, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++   +1 +autoshift, vmNormal, false, false);
        } else {
          eformat(ss,"",rr,4,"");              showString(padEquals(ss), &standardFont, horOffsetR - stringWidth(ss, &standardFont, false, false), Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index  +2  +autoshift, vmNormal, false, false);      
          strcpy(ss,"r" STD_SUP_2 "=");        showString(padEquals(ss), &standardFont, horOffset, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++   +2 +autoshift, vmNormal, false, false);
        }

        //eformat(ss,"x,y" STD_SUB_m STD_SUB_i STD_SUB_n "=", x_min,5);
        //showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++ -2 +autoshift, vmNormal, false, false);
        //eformat(ss,"x,y" STD_SUB_m STD_SUB_a STD_SUB_x "=", x_max,5);
        //showString(ss, &standardFont, 0, Y_POSITION_OF_REGISTER_Z_LINE + autoinc*index++ -2 +autoshift, vmNormal, false, false);
      }
    }
  }
#endif //TESTSUITE_BUILD



void fnPlotClose(uint16_t unusedButMandatoryParameter){
  lastPlotMode = PLOT_NOTHING;
  plotSelection = 0;
  calcMode = CM_NORMAL;
  fnKeyExit(0);
  fnUndo(0);
}


void fnPlotCloseSmi(uint16_t unusedButMandatoryParameter){
  lastPlotMode = PLOT_NOTHING;
  plotSelection = 0;
  calcMode = CM_NORMAL;
  fnKeyExit(0);
  fnUndo(0);
  fnMinExpStdDev(0);
}


//** Called from keyboard
//** plotSelection = 0 means that no curve fit is plotted
//
void fnPlotStat(uint16_t plotMode){
#if defined STATDEBUG && defined PC_BUILD
  printf("fnPlotStat1: plotSelection = %u; Plotmode=%u\n",plotSelection,plotMode);
  printf("#####>>> fnPlotStat1: plotSelection:%u:%s  Plotmode:%u lastplotmode:%u  lrSelection:%u lrChosen:%u\n",plotSelection, getCurveFitModeName(plotSelection), plotMode, lastPlotMode, lrSelection, lrChosen);
#endif //STATDEBUG

#if defined STATDEBUG && defined PC_BUILD
  uint16_t i;
  int16_t cnt;
  realToInt32(SIGMA_N, cnt);
  printf("Stored values\n");
  for (i = 0; i < LIM && i < cnt; ++i) { 
    printf("i = %3u x = %9f; y = %9f\n",i,gr_x[i],gr_y[i]);
  }
#endif //STATDEBUG


jm_SCALE = false;

#ifndef TESTSUITE_BUILD
  if (!(lastPlotMode == PLOT_NOTHING || lastPlotMode == PLOT_START)) plotMode = lastPlotMode;
  calcMode = CM_PLOT_STAT;
  statGraphReset(); 
  if(plotMode == PLOT_START){
    plotSelection = 0;
  }
  if(plotMode == PLOT_LR && lrSelection != 0) {
    plotSelection = lrSelection;
  }

  hourGlassIconEnabled = true;
  showHideHourGlass();

  #ifdef DMCP_BUILD
    lcd_refresh();
  #else // !DMCP_BUILD
    refreshLcd(NULL);
  #endif // DMCP_BUILD

  switch(plotMode) {
    case PLOT_LR:
    case PLOT_NXT:
    case PLOT_REV:
         if(softmenu[softmenuStack[0].softmenuId].menuItem != -MNU_PLOT_LR) {
           showSoftmenu(-MNU_PLOT_LR);
         }
         break;
    case PLOT_ORTHOF:
    case PLOT_START:
         jm_SCALE = true;
         if(softmenu[softmenuStack[0].softmenuId].menuItem != -MNU_PLOT_STAT) {
           showSoftmenu(-MNU_PLOT_STAT);
         }
         break;
    case PLOT_NOTHING:
         break;
    default: break;
  }

  if(plotMode != PLOT_START) {
    fnPlotRegressionLine(plotMode);
  }
  else {
    lastPlotMode = plotMode;
  }
#endif //TESTSUITE_BUILD
}


void fnPlotRegressionLine(uint16_t plotMode){
  #if defined STATDEBUG && defined PC_BUILD
  printf("fnPlotRegressionLine: plotSelection = %u; Plotmode=%u\n",plotSelection,plotMode);
  #endif //STATDEBUG

  switch(plotMode) {
    case PLOT_ORTHOF: 
      plotSelection = CF_ORTHOGONAL_FITTING;
      lrChosen = CF_ORTHOGONAL_FITTING;
      break;

      //Show data and one curve fit selected: Scans lrSelection from LSB and stop after the first one is found. If a chosen curve is there, override.
      //printf("#####X %u %u \n",plotSelection, lrSelection);
    case PLOT_NXT:
      plotSelection = plotSelection << 1;
      if(plotSelection == 0){
        plotSelection = 1;
      }

    //  while((plotSelection != ( (lrSelection == 0 ? 1023 : lrSelection) & plotSelection)) && (plotSelection < 1024)){ //fast forward to selected LR
    //    plotSelection = plotSelection << 1;
    //  }
      if(plotSelection >= 1024) {
        plotSelection = 0;  //purposely change to zero graph display to give a no-line view
      }
      break;

    case PLOT_REV:
      if(plotSelection == 0){
        plotSelection = 1024; //wraparound, will still shift right 1
      }
      plotSelection = plotSelection >> 1;
      if(plotSelection >= 1024){
        plotSelection = 0;  //purposely change to zero graph display to give a no line view
      }
      break;

    case PLOT_LR:
      //Show data and one curve fit selected: Scans lrSelection from LSB and stop after the first one is found. If a chosen curve is there, override.
      plotSelection = lrChosen;
      if(plotSelection == 0) plotSelection = 1;
      while((plotSelection != ( (lrSelection == 0 ? 1023 : lrSelection) & plotSelection)) && (plotSelection < 1024)){
        plotSelection = plotSelection << 1;
      }
      if(plotSelection >= 1024) plotSelection = 0;  //purposely change to zero graph display
      break;

    case PLOT_START:
      plotMode = PLOT_ORTHOF;
      break;

    case PLOT_NOTHING: 
      break;
    default:break;
  }
}


void fnPlotZoom(uint16_t unusedButMandatoryParameter){
   PLOT_ZOOM = (PLOT_ZOOM + 1) & 0x03;
  if(PLOT_ZOOM != 0) PLOT_AXIS = true; else PLOT_AXIS = false;
   #ifndef TESTSUITE_BUILD
     void refreshScreen(void);
   #endif //TESTSUITE_BUILD
}

/*
//DEMO: Arbitrary distribution to test. Close to a Normal.
void fnStatDemo0(uint16_t unusedButMandatoryParameter){
  #ifndef TESTSUITE_BUILD
  plotSelection = 0;
  runFunction(ITM_CLSIGMA);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-5.0",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("1.3887943864964E-11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-4.9",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("3.73757132794424E-11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-4.8",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9.85950557599145E-11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-4.7",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("2.54938188039194E-10",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-4.6",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("6.46143177310602E-10",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-4.5",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("1.60522805518558E-09",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-4.4",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("3.90893843426479E-09",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-4.3",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9.33028757450481E-09",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-4.2",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("2.18295779512542E-08",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-4.1",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("5.00621802076691E-08",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-4.0",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("1.12535174719256E-07",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-3.9",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("2.47959601804496E-07",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-3.8",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("5.35534780279297E-07",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-3.7",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("1.13372713874794E-06",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-3.6",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("2.35257520000972E-06",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-3.5",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("4.78511739212891E-06",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-3.4",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9.54016287307904E-06",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-3.3",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("1.86437423315165E-05",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-3.2",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("3.57128496416346E-05",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-3.1",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("6.70548243028099E-05",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-3.0",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.000123409804086678",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-2.9",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.000222629856918886",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-2.8",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.000393669040655073",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-2.7",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.000682328052756367",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-2.6",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.00115922917390458",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-2.5",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.00193045413622769",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-2.4",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.00315111159844441",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-2.3",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.00504176025969093",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-2.2",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.00790705405159337",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-2.1",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.0121551783299148",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-2.0",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.0183156388887341",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-1.9",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.0270518468663502",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-1.8",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.0391638950989869",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-1.7",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.0555762126114828",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-1.6",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.0773047404432994",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-1.5",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.105399224561864",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-1.4",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.140858420921045",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-1.3",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.184519523992989",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-1.2",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.236927758682121",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-1.1",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.298197279429887",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-1.0",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.367879441171442",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-0.9",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.44485806622294",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-0.8",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.527292424043048",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-0.7",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.612626394184415",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-0.6",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.69767632607103",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-0.5",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.778800783071404",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-0.4",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.852143788966211",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-0.3",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.913931185271228",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-0.2",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.960789439152323",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-0.1",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.990049833749168",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "0.0",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("1",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "0.1",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.990049833749168",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "0.2",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.960789439152324",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "0.3",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.913931185271229",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "0.4",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.852143788966212",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "0.5",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.778800783071406",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "0.6",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.697676326071032",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "0.7",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.612626394184417",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "0.8",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.527292424043049",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "0.9",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.444858066222942",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "1.0",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.367879441171443",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "1.1",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.298197279429888",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "1.2",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.236927758682122",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "1.3",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.18451952399299",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "1.4",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.140858420921045",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "1.5",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.105399224561865",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "1.6",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.0773047404432999",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "1.7",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.0555762126114832",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "1.8",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.0391638950989871",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "1.9",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.0270518468663504",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "2.0",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.0183156388887342",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "2.1",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.012155178329915",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "2.2",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.00790705405159345",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "2.3",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.00504176025969098",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "2.4",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.00315111159844444",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "2.5",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.00193045413622771",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "2.6",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.00115922917390459",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "2.7",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.000682328052756376",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "2.8",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.000393669040655078",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "2.9",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.000222629856918889",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "3.0",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.000123409804086679",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "3.1",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("6.70548243028109E-05",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "3.2",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("3.57128496416351E-05",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "3.3",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("1.86437423315168E-05",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "3.4",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9.54016287307918E-06",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "3.5",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("4.78511739212897E-06",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "3.6",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("2.35257520000976E-06",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "3.7",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("1.13372713874796E-06",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "3.8",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("5.35534780279306E-07",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "3.9",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("2.47959601804501E-07",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "4.0",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("1.12535174719258E-07",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "4.1",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("5.00621802076701E-08",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "4.2",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("2.18295779512548E-08",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "4.3",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9.33028757450501E-09",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "4.4",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("3.90893843426488E-09",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "4.5",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("1.60522805518562E-09",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "4.6",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("6.46143177310618E-10",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "4.7",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("2.54938188039201E-10",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "4.8",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("9.85950557599169E-11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34( "4.9",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("3.73757132794435E-11",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    fnCurveFitting(0);
    runFunction(ITM_LR);
    runFunction(ITM_PLOT_LR);
   #endif //TESTSUITE_BUILD
  }

//DEMO: Randomized linear
void fnStatDemo10(uint16_t unusedButMandatoryParameter){
  #ifndef TESTSUITE_BUILD
    int8_t ix;
    runFunction(ITM_CLSIGMA);
    plotSelection = 0;
    srand((unsigned int)time(NULL));
    for(ix=0; ix!=10; ix++) {
      runFunction(ITM_RAN);
      setSystemFlag(FLAG_ASLIFT);
      liftStack();
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      int32ToReal34(2000+ix/2,REGISTER_REAL34_DATA(REGISTER_X));
      runFunction(ITM_ADD);
      runFunction(ITM_RAN);
      setSystemFlag(FLAG_ASLIFT);
      liftStack();
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      int32ToReal34(ix,REGISTER_REAL34_DATA(REGISTER_X));
      runFunction(ITM_ADD);
      runFunction(ITM_SIGMAPLUS);
      }
    fnCurveFitting(0);
    runFunction(ITM_LR);
    runFunction(ITM_PLOT_LR);
  #endif //TESTSUITE_BUILD
}

void fnStatDemo2(uint16_t unusedButMandatoryParameter){
  #ifndef TESTSUITE_BUILD
    plotSelection = 0;
    runFunction(ITM_CLSIGMA);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("-0.1",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.0905",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0000",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("1.0000",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("+0.1",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.0905",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0.01",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("0.8",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    fnCurveFitting(0);
    runFunction(ITM_LR);
    runFunction(ITM_PLOT_LR);
  #endif //TESTSUITE_BUILD
}


//DEMO: Randomized linear
void fnStatDemo1(uint16_t unusedButMandatoryParameter){
  #ifndef TESTSUITE_BUILD
    int8_t ix;
    time_t t;
    srand((unsigned) time(&t));
    runFunction(ITM_CLSIGMA);
    plotSelection = 0;
    srand((unsigned int)time(NULL));
    for(ix=0; ix!=100; ix++) {

      int mv = 11000 + rand() % 22;  //instrument measuring RMS voltage of an 11 kV installation, with +- 0.1% variance, offset to the + for convenience

      setSystemFlag(FLAG_ASLIFT);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      int32ToReal34(mv+rand()%4,REGISTER_REAL34_DATA(REGISTER_X)); // reading 1 has additional +0 to +3 variance to the said random number

      setSystemFlag(FLAG_ASLIFT);
      liftStack();
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      int32ToReal34(mv+rand()%4,REGISTER_REAL34_DATA(REGISTER_X)); // reading 2 has additional +0 to +3 variance to the said random number

      runFunction(ITM_SIGMAPLUS);
      }
    runFunction(ITM_PLOT);
    runFunction(ITM_PLOT_CENTRL);
  #endif //TESTSUITE_BUILD
}




//DEMO: 4 points to simulate a distribution, from p105 of OM
void fnStatDemo105(uint16_t unusedButMandatoryParameter){
  #ifndef TESTSUITE_BUILD
    plotSelection = 0;
    runFunction(ITM_CLSIGMA);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("2",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("30",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("2.5",REGISTER_REAL34_DATA(REGISTER_X));stringToReal34("50",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("3.5",REGISTER_REAL34_DATA(REGISTER_X));stringToReal34("90",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("4",REGISTER_REAL34_DATA(REGISTER_X));stringToReal34("130",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("4.5",REGISTER_REAL34_DATA(REGISTER_X));stringToReal34("150",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    fnCurveFitting(0);
    runFunction(ITM_LR);
    runFunction(ITM_PLOT_LR);
  #endif //TESTSUITE_BUILD
}

//DEMO: points to simulate a distribution, from p107 of OM
void fnStatDemo107(uint16_t unusedButMandatoryParameter){
  #ifndef TESTSUITE_BUILD
    plotSelection = 0;
    runFunction(ITM_CLSIGMA);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1945",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("696",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1955",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("1330",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1965",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("1750",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1971",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("2243",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1973",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("2484",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1950",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("994",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1960",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("1512",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1970",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("2162",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("1972",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("2382",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    fnCurveFitting(0);
    runFunction(ITM_LR);
    runFunction(ITM_PLOT_LR);
  #endif //TESTSUITE_BUILD
}

//DEMO:  points to simulate a distribution, from p109 of OM
void fnStatDemo109(uint16_t unusedButMandatoryParameter){
  #ifndef TESTSUITE_BUILD
    plotSelection = 0;
    runFunction(ITM_CLSIGMA);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("0",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("4.63",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("20",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("5.78",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("40",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("6.61",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("60",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("7.21",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone); reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);stringToReal34("80",REGISTER_REAL34_DATA(REGISTER_X)); stringToReal34("7.78",REGISTER_REAL34_DATA(REGISTER_Y));runFunction(ITM_SIGMAPLUS);
    fnCurveFitting(0);
    runFunction(ITM_LR);
    runFunction(ITM_PLOT_LR);
  #endif //TESTSUITE_BUILD
}

*/
