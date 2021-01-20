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
 * along with 43S.  If not, see <htempp://www.gnu.org/licenses/>.
 */

/********************************************//** //JM
 * \file graphs.c Graphing module
 ***********************************************/

/* C43 functions and routines written by JM, adapted from C43 to WP43S by JM */


#include "wp43s.h"
#include "math.h"

//#define STATDEBUG

#define Y_non_square_min 0 //SCREEN_MIN_GRAPH


graphtype *gr_x;
graphtype *gr_y;

//Note: graph_xmin, graph_xmax set from X.FN GRAPH
float    tick_int_x;
float    tick_int_y;
float    x_min; 
float    x_max;
float    y_min;
float    y_max;
uint32_t xzero;
uint32_t yzero;
bool_t invalid_intg = true;
bool_t invalid_diff = true;
bool_t invalid_rms = true;
  


void graph_reset(void){
  graph_dx      = 0;
  graph_dy      = 0;
  extentx       = false;
  extenty       = false;
  jm_VECT       = false;
  jm_NVECT      = false;
  jm_SCALE      = false;
  Aspect_Square = true;
  PLOT_LINE     = true;
  PLOT_CROSS    = false;
  PLOT_BOX      = false;
  PLOT_INTG     = false;
  PLOT_DIFF     = false;
  PLOT_RMS      = false;
  PLOT_SHADE    = false;
}


void fnClGrf(uint16_t unusedButMandatoryParameter) {
  graph_reset();
  fnClSigma(0);
  fnRefreshComboxState(CB_JC, JC_PLINE, PLOT_LINE);                //jm
}


void fnPline(uint16_t unusedButMandatoryParameter) {
  PLOT_LINE = !PLOT_LINE;
  fnRefreshComboxState(CB_JC, JC_PLINE, PLOT_LINE);                //jm
  fnPlot(0);
}


void fnPcros(uint16_t unusedButMandatoryParameter) {
  PLOT_CROSS = !PLOT_CROSS;
  if(PLOT_CROSS) PLOT_BOX = false;
  fnRefreshComboxState(CB_JC, JC_PCROS, PLOT_CROSS);                //jm
  fnPlot(0);
}


void fnPbox (uint16_t unusedButMandatoryParameter) {
  PLOT_BOX = !PLOT_BOX;
  if(PLOT_BOX) PLOT_CROSS = false;
  fnRefreshComboxState(CB_JC, JC_PBOX, PLOT_BOX);                //jm
  fnPlot(0);
}

void fnPintg (uint16_t unusedButMandatoryParameter) {
  PLOT_INTG = !PLOT_INTG;
  if(!PLOT_INTG) PLOT_SHADE = false;
  fnRefreshComboxState(CB_JC, JC_INTG, PLOT_INTG);                //jm
  fnPlot(0);
}

void fnPdiff (uint16_t unusedButMandatoryParameter) {
  PLOT_DIFF = !PLOT_DIFF;
  fnRefreshComboxState(CB_JC, JC_DIFF, PLOT_DIFF);                //jm
  fnPlot(0);
}

void fnPrms (uint16_t unusedButMandatoryParameter) {
  PLOT_RMS = !PLOT_RMS;
  fnRefreshComboxState(CB_JC, JC_RMS, PLOT_RMS);                //jm
  fnPlot(0);
}

void fnPvect (uint16_t unusedButMandatoryParameter) {
  jm_VECT = !jm_VECT;
  if(jm_VECT) {jm_NVECT = false;}
  fnRefreshComboxState(CB_JC, JC_VECT, jm_VECT);                //jm
  fnPlot(0);
}    

void fnPNvect (uint16_t unusedButMandatoryParameter) {
  jm_NVECT = !jm_NVECT;
  if(jm_NVECT) {jm_VECT = false;}
  fnRefreshComboxState(CB_JC, JC_NVECT, jm_NVECT);                //jm
  fnPlot(0);
}    

void fnScale (uint16_t unusedButMandatoryParameter) {
  jm_SCALE = !jm_SCALE;
  fnRefreshComboxState(CB_JC, JC_SCALE, jm_SCALE);                //jm
  fnPlot(0);
}    

void fnPshade (uint16_t unusedButMandatoryParameter) {
  PLOT_SHADE = !PLOT_SHADE;
  if(PLOT_SHADE) PLOT_INTG = true;
  fnRefreshComboxState(CB_JC, JC_SHADE, PLOT_SHADE);                //jm
  fnPlot(0);
}    

void fnPx (uint16_t unusedButMandatoryParameter) {
  extentx = !extentx;
  fnRefreshComboxState(CB_JC, JC_EXTENTX, extentx);                //jm
  fnPlot(0);
}

void fnPy (uint16_t unusedButMandatoryParameter) {
  extenty = !extenty;
  fnRefreshComboxState(CB_JC, JC_EXTENTY, extenty);                //jm
  fnPlot(0);
}

void fnPlot(uint16_t unusedButMandatoryParameter) {
  Aspect_Square = true;
  if(calcMode != CM_GRAPH){previousCalcMode = calcMode;}
  if(previousCalcMode == CM_GRAPH) {
    previousCalcMode = CM_NORMAL;
  }
  calcMode = CM_GRAPH;
  #ifndef TESTSUITE_BUILD
    if(softmenu[softmenuStack[0].softmenuId].menuItem != -MNU_PLOT) {
      showSoftmenu(-MNU_PLOT);                         //JM MENU Prevent resetting the softmenu to the default no 1 page position
    }
  #endif
  doRefreshSoftMenu = true;             //Plot graph is part of refreshScreen
}

void fnPlotLS(uint16_t unusedButMandatoryParameter) {
  Aspect_Square = false;
  if(calcMode != CM_GRAPH){previousCalcMode = calcMode;}
  if(previousCalcMode == CM_GRAPH) {
    previousCalcMode = CM_NORMAL;
  }
  calcMode = CM_GRAPH;
  doRefreshSoftMenu = true;             //Plot graph is part of refreshScreen
}


void fnListXY(uint16_t unusedButMandatoryParameter) {
  #ifndef TESTSUITE_BUILD
    if(telltale == MEM_INITIALIZED) {
      calcMode = CM_LISTXY; //Used to view graph/listing
      ListXYposition = 0;
    }
  #endif
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
      gr_x[i] = 0;
      gr_y[i] = 0; 
    }
  }
}


void graph_end(void) {
  free(gr_x);
  free(gr_y);
  telltale = 0;
}


graphtype grf_x(int i) {
  if (jm_NVECT) {return gr_y[i];}
  else {return gr_x[i];}
}

graphtype grf_y(int i) {
  if (jm_NVECT) {return gr_x[i];}
  else {return gr_y[i];}
}



void graph_sigmaplus(int8_t plusminus, real_t *xx, real_t *yy) {    //Called from STAT module from fnSigma(), to store the x,y pair to the memory structure.
  int16_t cnt;
  graphtype x; 
  graphtype y;

  if(jm_VECT || jm_NVECT) {plotmode = _VECT;} else {plotmode = _SCAT;}

  if(telltale != MEM_INITIALIZED) {
    graph_setupmemory();
//    runFunction(ITM_CLSIGMA);
  }


  //Convert from X register to graphtype
  realToString(yy, tmpString);
  y = strtof (tmpString, NULL);

  //printf("y=%f ",y);

  //Convert from X register to graphtype
  realToString(xx, tmpString);
  x = strtof (tmpString, NULL);

  //printf("x=%f ",x);

  #ifndef TESTSUITE_BUILD
  export_xy_to_file(x,y);     //Write to CSV file
  #endif

  if(plotmode == _VECT ) {
    ix_count++;               //Only used for VECT
    cnt = ix_count;
  } else {
    //Convert from real to int
    realToInt32(SIGMA_N, cnt);
    ix_count = cnt;                          //ix_count increments in VECT with Σ-, where SIGMA_N decrements with Σ- 
                                             //if VECT is changed mid-process, it will cause x_count to assume SIGMA_N, which  will throw away the last values stored.
//    realToString(SIGMA_N, tmpString);
//    cnt = stringToInt16 (tmpString);
    #ifdef STATDEBUG
    printf("Count: %s, %d\n",tmpString,cnt);
    #endif

  }
  //printf("Adding to graph table[%d] = x:%f y:%f\n",cnt,x,y);

  if(plusminus == 1) {
    gr_x[cnt-1]=x;
    gr_y[cnt-1]=y;
    #ifdef STATDEBUG
    printf("Index: [%d]=(%f,%f)\n",cnt-1,x,y);
    #endif
  } else {
    if(plusminus == -1) {
      if(plotmode == _VECT ) {
        gr_x[cnt-1]=-x;
        gr_y[cnt-1]=-y;
        #ifdef STATDEBUG
        printf("Index: [%d]=(%f,%f)\n",cnt-1,-x,-y);
        #endif
      } else {
        // Non-vector mode TODO
      }
    }
  }
}


//###################################################################################
#ifndef TESTSUITE_BUILD
  int16_t screen_window_x(graphtype x_min, graphtype x, graphtype x_max) {
    int16_t temp;
    if (Aspect_Square) {
      temp = (x-x_min)/(x_max-x_min)*(SCREEN_HEIGHT_GRAPH-1);
      if (temp>SCREEN_HEIGHT_GRAPH-1) {temp=SCREEN_HEIGHT_GRAPH-1;}
      else if (temp<0) {temp=0;}
      return temp+SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;
    } 
    else {  //FULL SCREEN
      temp = ((x-x_min)/(x_max-x_min)*(SCREEN_WIDTH_GRAPH-1));
      //printf("--> %d (%f %f)  ",temp, x_min,x_max);
      if (temp>SCREEN_WIDTH_GRAPH-1) {temp=SCREEN_WIDTH_GRAPH-1;}
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
    if (!Aspect_Square) minn = Y_non_square_min;
    else minn = 0;

    temp = (y-y_min)/(y_max-y_min)*(SCREEN_HEIGHT_GRAPH-1 - minn);
      if (temp>SCREEN_HEIGHT_GRAPH-1 - minn) {temp=SCREEN_HEIGHT_GRAPH-1 - minn;}
    else if (temp<0) {temp=0;}

    #ifdef PC_BUILD
      if(SCREEN_HEIGHT_GRAPH-1 - temp<0 || SCREEN_HEIGHT_GRAPH-1 - temp>239) {printf("In function screen_window_y Y EXCEEDED %d %d",temp,SCREEN_HEIGHT_GRAPH-1 - temp);}
    #endif
    return (SCREEN_HEIGHT_GRAPH-1 - temp);
  }

#endif


//###################################################################################
void placePixel(uint32_t x, uint32_t y) {
#ifndef TESTSUITE_BUILD
  uint32_t minn;
  if (!Aspect_Square) minn = Y_non_square_min;
  else minn = 0;
    
  if(x<SCREEN_WIDTH_GRAPH && x>0 && y<SCREEN_HEIGHT_GRAPH && y>1+minn) {
    setBlackPixel(x,y);
  }
#endif //!TESTSUITE_BUILD
}

void removePixel(uint32_t x, uint32_t y) {
#ifndef TESTSUITE_BUILD
  uint32_t minn;
  if (!Aspect_Square) minn = Y_non_square_min;
  else minn = 0;

  if(x<SCREEN_WIDTH_GRAPH && x>0 && y<SCREEN_HEIGHT_GRAPH && y>1+minn) {
    setWhitePixel(x,y);
  }
#endif //!TESTSUITE_BUILD
}

void clearScreenPixels() {
#ifndef TESTSUITE_BUILD
  if (Aspect_Square) {
    lcd_fill_rect(SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH, 0, SCREEN_HEIGHT_GRAPH, SCREEN_HEIGHT_GRAPH, 0);
    lcd_fill_rect(0, Y_POSITION_OF_REGISTER_T_LINE, SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH, 171-5-Y_POSITION_OF_REGISTER_T_LINE+1, 0);
    lcd_fill_rect(19, 171-5, SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH-19+1, 5, 0);
  }
  else
    lcd_fill_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT_GRAPH, 0);
#endif //!TESTSUITE_BUILD

}                                                       //JM ^^


#ifndef TESTSUITE_BUILD
      void plotarrow(uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn) {              // Plots line from xo,yo to xn,yn; uses temporary x1,y1
        float dx, dy, ddx, dydx, zz, zzz;
        dydx = yn-yo;
        ddx = xn-xo;
        zz  = sqrt(dydx*dydx+ddx*ddx);
        zzz = 3;
        dy  = dydx * (zzz/zz);
        dx  = ddx * (zzz/zz);
        #ifdef STATDEBUG
        printf("%d %d  %d %d  ddx=%f, dydx=%f, zz=%f  zzz=%f, dx=%f, dy=%f \n",xo, yo, xn, yn, ddx,dydx,zz,zzz,dx,dy);
        #endif
        if (!(xo==xn && yo==yn)){
          plotline(xn+(-3*dx +dy), yn+(-3*dy -dx), xn, yn);
          plotline(xn+(-3*dx -dy), yn+(-3*dy +dx), xn, yn);
        } else {
          placePixel(xn,yn);
        }
      }


      void plotdeltabig(uint16_t xn, uint8_t yn) {              // Plots ldifferential sign; uses temporary x1,y1
        plotline(xn+0,yn-2,     xn+0+5,yn-2+8);
        plotline(xn+0+5,yn-2+8, xn+0-5,yn-2+8);
        plotline(xn+0-5,yn-2+8, xn+0,yn-2    );
      }


      void plotdelta(uint16_t xn, uint8_t yn) {              // Plots ldifferential sign; uses temporary x1,y1
        placePixel(xn+0,yn-2);                               //   PLOT a delta
        placePixel(xn-1,yn-1);
        placePixel(xn-1,yn+0);
        placePixel(xn-2,yn+1);
        placePixel(xn-2,yn+2);
        placePixel(xn+1,yn-1);
        placePixel(xn+1,yn-0);
        placePixel(xn+2,yn+1);
        placePixel(xn+2,yn+2);
        placePixel(xn-1,yn+2);
        placePixel(xn  ,yn+2);
        placePixel(xn+1,yn+2);
      }

      void plotintbig(uint16_t xn, uint8_t yn) {                // Plots integral sign; uses temporary x1,y1
        plotline(xn-0,yn-2  ,     xn+3,yn-2);
        plotline(xn-0,yn-2+1,     xn+3,yn-2+1);
        plotline(xn-3,yn-2+8,     xn+0,yn-2+8);
        plotline(xn-3,yn-2+9,     xn+0,yn-2+9);

        plotline(xn+0,yn-2+7,     xn+0,yn-2+0);
        plotline(xn+1,yn-2+7,     xn+1,yn-2+0);
      }

      void plotint(uint16_t xn, uint8_t yn) {                // Plots integral sign; uses temporary x1,y1
        placePixel(xn,yn);                                   //   PLOT a I
        placePixel(xn,yn-1);
        placePixel(xn,yn-2);
        placePixel(xn,yn+1);
        placePixel(xn,yn+2);
        placePixel(xn+1,yn-2);
        placePixel(xn-1,yn+2);
      }

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


      void plotrms(uint16_t xn, uint8_t yn) {                // Plots line from xo,yo to xn,yn; uses temporary x1,y1
        placePixel(xn+1,yn-1);                               //   PLOT a box
        placePixel(xn-1,yn-1);
        placePixel(xn-0,yn-1);
        placePixel(xn+1,yn);                               //   PLOT a box
        placePixel(xn-1,yn);
        placePixel(xn-0,yn);
        placePixel(xn+1,yn+1);                               //   PLOT a box
        placePixel(xn-1,yn+1);
        placePixel(xn-0,yn+1);

      }



      void plotrect(uint16_t a, uint8_t b, uint16_t c, uint8_t d) {                // Plots rectangle from xo,yo to xn,yn; uses temporary x1,y1
        plotline(a, b, c, b);
        plotline(a, b, a, d);
        plotline(c, d, c, b);
        plotline(c, d, a, d);
      }


      void plottriangle(uint16_t a, uint8_t b, uint16_t c, uint8_t d) {                // Plots rectangle from xo,yo to xn,yn; uses temporary x1,y1
        plotline(a, b, c, b);
        plotline(a, b, c, d);
        plotline(c, d, c, b);
      }




#endif



//###################################################################################
float auto_tick(float tick_int_f) {
    //Obtain scaling of ticks, to about 20 intervals left to right.
  //graphtype tick_int_f = (x_max-x_min)/20;                                                 //printf("tick interval:%f ",tick_int_f);
  snprintf(tmpString, TMP_STR_LENGTH, "%.1e", tick_int_f);
  char tx[4];
  tx[0] = tmpString[0];
  tx[1] = tmpString[1];
  tx[2] = tmpString[2];
  tx[3] = 0;
  //printf("tick0 %f orgstr %s tx %s \n",tick_int_f, tmpString, tx);
  tick_int_f = strtof (tx, NULL);                                        //printf("string:%s converted:%f \n",tmpString, tick_int_f);
  //printf("tick1 %f orgstr %s tx %s \n",tick_int_f, tmpString, tx);
  if(tick_int_f > 0   && tick_int_f <=  0.3)  {tmpString[0] = '0'; tmpString[2]='2'; } else
  if(tick_int_f > 0.3 && tick_int_f <=  0.6)  {tmpString[0] = '0'; tmpString[2]='5'; } else
  if(tick_int_f > 0.6 && tick_int_f <=  1.3)  {tmpString[0] = '1'; tmpString[2]='0'; } else
  if(tick_int_f > 1.3 && tick_int_f <=  1.7)  {tmpString[0] = '1'; tmpString[2]='5'; } else
  if(tick_int_f > 1.7 && tick_int_f <=  3.0)  {tmpString[0] = '2'; tmpString[2]='0'; } else
  if(tick_int_f > 3.0 && tick_int_f <=  6.5)  {tmpString[0] = '5'; tmpString[2]='0'; } else
  if(tick_int_f > 6.5 && tick_int_f <=  9.9)  {tmpString[0] = '7'; tmpString[2]='5'; }
  tick_int_f = strtof (tmpString, NULL);                                        //printf("string:%s converted:%f \n",tmpString, tick_int_f);
  //printf("tick2 %f str %s tx %s \n",tick_int_f, tmpString, tx);
  return tick_int_f;
}


//###################################################################################
void graph_axis (void){
  #ifndef TESTSUITE_BUILD
  uint32_t cnt;
  uint32_t ypos = Y_POSITION_OF_REGISTER_T_LINE -11 + 12 * 5;

  //GRAPH RANGE
  //  graph_xmin= -3*3.14150;  graph_xmax= 2*3.14159;
  //  graph_ymin= -2;          graph_ymax= +2;


  clearScreenPixels();

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


  uint16_t ii;
  uint16_t oo;
  char outstr[300];
  void convertDigits(void) {
    switch (tmpString[ii]) {
      case  48: outstr[oo++] = 0xa0; outstr[oo++] = 0x80; break; //.
      case  49: outstr[oo++] = 0xa0; outstr[oo++] = 0x81; break; //.
      case  50: outstr[oo++] = 0xa0; outstr[oo++] = 0x82; break; //.
      case  51: outstr[oo++] = 0xa0; outstr[oo++] = 0x83; break; //.
      case  52: outstr[oo++] = 0xa0; outstr[oo++] = 0x84; break; //.
      case  53: outstr[oo++] = 0xa0; outstr[oo++] = 0x85; break; //.
      case  54: outstr[oo++] = 0xa0; outstr[oo++] = 0x86; break; //.
      case  55: outstr[oo++] = 0xa0; outstr[oo++] = 0x87; break; //.
      case  56: outstr[oo++] = 0xa0; outstr[oo++] = 0x88; break; //.
      case  57: outstr[oo++] = 0xa0; outstr[oo++] = 0x89; break; //.

      case 120: outstr[oo++] = 0xa4; outstr[oo++] = 0xb3; break; //x
      case 121: outstr[oo++] = 0xa4; outstr[oo++] = 0xb4; break; //y
      case 'a': outstr[oo++] = 0xa4; outstr[oo++] = 0x9c; break; //a
      case 's': outstr[oo++] = 0xa4; outstr[oo++] = 0xae; break; //s
      case  58: outstr[oo++] = 0xa2; outstr[oo++] = 0x36; break; //:
      case  43: outstr[oo++] = 0xa0; outstr[oo++] = 0x8a; break; //+
      case  45: outstr[oo++] = 0xa0; outstr[oo++] = 0x8b; break; //-
      case  46: outstr[oo++] = 0xa0; outstr[oo++] = 0x1a; break; //.
      case  47: outstr[oo++] = 0xa4; outstr[oo++] = 0x25; break; ///
      case 116: outstr[oo++] = 0xa0; outstr[oo++] = 0x9c; break; //t
      case 105: outstr[oo++] = 0xa4; outstr[oo++] = 0xa4; break; //i
      case 99 : outstr[oo++] = 0xa4; outstr[oo++] = 0x9e; break; //c
      case 107: outstr[oo++] = 0xa4; outstr[oo++] = 0xa6; break; //k
      default : outstr[oo++] = tmpString[ii];
    }
  }



  snprintf(tmpString, TMP_STR_LENGTH, "y %.3f/tick  ",tick_int_y);
  ii = 0;
  oo = 0;
  outstr[0]=0;
  while (tmpString[ii] != 0) {
    convertDigits();
    ii++;
  }
  outstr[oo]=0;
  showString(outstr, &standardFont, 1, ypos, vmNormal, true, true);  //JM
  ypos -= 12;


  snprintf(tmpString, TMP_STR_LENGTH, "x %.3f/tick  ", tick_int_x);
  ii = 0;
  oo = 0;
  outstr[0]=0;
  while (tmpString[ii] != 0) {
    convertDigits();
    ii++;
  }
  outstr[oo]=0;
  showString(outstr, &standardFont, 1, ypos, vmNormal, true, true);  //JM
  ypos -= 12;


  //GRAPH ZERO AXIS
  yzero = screen_window_y(y_min,0,y_max);
  xzero = screen_window_x(x_min,0,x_max);

  uint32_t minnx, minny;
  if (!Aspect_Square) {
    minny = Y_non_square_min;
    minnx = 0;
  }
  else {
    minny = 0;
    minnx = SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;
  }

  tmpString[0]=0;                                  //If the axis is on the edge supress it, and label accordingly
  uint8_t axisdisp = 
           (!( yzero == SCREEN_HEIGHT_GRAPH-1  || yzero == minny) ? 2:0) + 
           (!(  xzero == SCREEN_WIDTH-1        || xzero == minnx) ? 1:0) ;
  switch( axisdisp )  {
    case 0: strcpy(tmpString,"          "); break;
    case 1: snprintf(tmpString, TMP_STR_LENGTH, "y-axis x 0"); break;
    case 2: snprintf(tmpString, TMP_STR_LENGTH, "x-axis y 0"); break;
    case 3: snprintf(tmpString, TMP_STR_LENGTH, "axis 0.0 "); break;
    default:break;
  }


  //Change to the small characters and fabricate a small = char
    ii = 0;
    oo = 0;
    outstr[0]=0;
    while (tmpString[ii] != 0) {
      convertDigits();
      ii++;
    }
  outstr[oo]=0;
  ii=showString(outstr, &standardFont, 1, ypos, vmNormal, true, true);  //JM
  if(tmpString[ stringByteLength(tmpString)-1 ] == '0') {
    #define sp 15
    plotline((uint16_t)(ii-17),(uint8_t)(ypos+2+sp),(uint16_t)(ii-11),(uint8_t)(ypos+2+sp));
    plotline((uint16_t)(ii-17),(uint8_t)(ypos+1+sp),(uint16_t)(ii-11),(uint8_t)(ypos+1+sp));
    plotline((uint16_t)(ii-17),(uint8_t)(ypos-1+sp),(uint16_t)(ii-11),(uint8_t)(ypos-1+sp));
    plotline((uint16_t)(ii-17),(uint8_t)(ypos-2+sp),(uint16_t)(ii-11),(uint8_t)(ypos-2+sp));
  }
  ypos += 48;
  


  if(PLOT_INTG && !invalid_intg) {
    snprintf(tmpString, TMP_STR_LENGTH, "  Trapezoid integral");
    showStringC43(tmpString, numSmall, nocompress, 1, ypos, vmNormal, true, true);  //JM
    plotintbig( 5, ypos+4+4-2 );
    plotrect(5+4-1, (ypos+4+4-2+2)-1, 5+4+2, (ypos+4+4-2+2)+2);
    ypos += 20;
  }

  if(PLOT_DIFF && !invalid_diff) {
    snprintf(tmpString, TMP_STR_LENGTH, "  Num. differential");
    showStringC43(tmpString, numSmall, nocompress, 1, ypos, vmNormal, true, true);  //JM
    plotdeltabig( 6, ypos+4+4-2);
    ypos += 20;
  }

  if(PLOT_RMS && !invalid_rms) {
    snprintf(tmpString, TMP_STR_LENGTH, "  RMSy");
    showStringC43(tmpString, numSmall, nocompress, 1, ypos, vmNormal, true, true);  //JM
    plotrms( 6, ypos+4+4-2);
    plotrect(6-1, (ypos+4+4-2)-1, 6+2, (ypos+4+4-2)+2);
    ypos += 20;
  }


  force_refresh();



  //SEPARATING LINE IF SQUARE
  cnt = minny;
  while (cnt!=SCREEN_HEIGHT_GRAPH) { 
    if (Aspect_Square) {
        setBlackPixel(minnx-1,cnt);   
        setBlackPixel(minnx-2,cnt);   
    }
    cnt++; 
  }

  #ifdef STATDEBUG
    printf("xzero=%d yzero=%d   \n",xzero,yzero);
  #endif


  graphtype x; 
  graphtype y;

  if( !(yzero == SCREEN_HEIGHT_GRAPH-1 || yzero == minny)) {
    //DRAW XAXIS
    if (Aspect_Square) {
      cnt = minnx;
    } else {
      cnt = 0;
    }  
    while (cnt!=SCREEN_WIDTH_GRAPH-1) { 
      setBlackPixel(cnt,yzero); 
      cnt++; 
    }

   force_refresh();

   for(x=0; x<=x_max; x+=tick_int_x) {       //draw x ticks
      cnt = screen_window_x(x_min,x,x_max);
        setBlackPixel(cnt,min(yzero+1,SCREEN_HEIGHT_GRAPH-1)); //tick
        setBlackPixel(cnt,max(yzero-1,minny)); //tick
     }
    for(x=0; x>=x_min; x+=-tick_int_x) {       //draw x ticks
      cnt = screen_window_x(x_min,x,x_max);
        setBlackPixel(cnt,min(yzero+1,SCREEN_HEIGHT_GRAPH-1)); //tick
        setBlackPixel(cnt,max(yzero-1,minny)); //tick
     }
    for(x=0; x<=x_max; x+=tick_int_x*5) {       //draw x ticks
      cnt = screen_window_x(x_min,x,x_max);
        setBlackPixel(cnt,min(yzero+2,SCREEN_HEIGHT_GRAPH-1)); //tick
        setBlackPixel(cnt,max(yzero-2,minny)); //tick
        setBlackPixel(cnt,min(yzero+3,SCREEN_HEIGHT_GRAPH-1)); //tick
        setBlackPixel(cnt,max(yzero-3,minny)); //tick
     }
    for(x=0; x>=x_min; x+=-tick_int_x*5) {       //draw x ticks
      cnt = screen_window_x(x_min,x,x_max);
        setBlackPixel(cnt,min(yzero+2,SCREEN_HEIGHT_GRAPH-1)); //tick
        setBlackPixel(cnt,max(yzero-2,minny)); //tick
        setBlackPixel(cnt,min(yzero+3,SCREEN_HEIGHT_GRAPH-1)); //tick
        setBlackPixel(cnt,max(yzero-3,minny)); //tick
     }
  }



  if( !(xzero == SCREEN_WIDTH-1 || xzero == minnx)) {

    //Write North arrow
    if(jm_NVECT) {
      showString("N", &standardFont, xzero-4, minny+14, vmNormal, true, true);
      tmpString[0]=0x80 | 0x22;
      tmpString[1]=0x06;
      tmpString[2]=0;
      showString(tmpString, &standardFont, xzero-4, minny+0, vmNormal, true, true);
    }

    //DRAW YAXIS
    lcd_fill_rect(xzero,minny,1,SCREEN_HEIGHT_GRAPH-minny,0xFF);

    force_refresh();

    for(y=0; y<=y_max; y+=tick_int_y) {       //draw y ticks
      cnt = screen_window_y(y_min,y,y_max);
      setBlackPixel(max(xzero-1,0),cnt); //tick
      setBlackPixel(min(xzero+1,SCREEN_WIDTH_GRAPH-1),cnt); //tick
    }  
    for(y=0; y>=y_min; y+=-tick_int_y) {       //draw y ticks
      cnt = screen_window_y(y_min,y,y_max);
      setBlackPixel(max(xzero-1,0),cnt); //tick
      setBlackPixel(min(xzero+1,SCREEN_WIDTH_GRAPH-1),cnt); //tick
    }  
    for(y=0; y<=y_max; y+=tick_int_y*5) {       //draw y ticks
      cnt = screen_window_y(y_min,y,y_max);
      setBlackPixel(max(xzero-2,0),cnt); //tick
      setBlackPixel(min(xzero+2,SCREEN_WIDTH_GRAPH-1),cnt); //tick
      setBlackPixel(max(xzero-3,0),cnt); //tick
      setBlackPixel(min(xzero+3,SCREEN_WIDTH_GRAPH-1),cnt); //tick
    }  
    for(y=0; y>=y_min; y+=-tick_int_y*5) {       //draw y ticks
      cnt = screen_window_y(y_min,y,y_max);
      setBlackPixel(max(xzero-2,0),cnt); //tick
      setBlackPixel(min(xzero+2,SCREEN_WIDTH_GRAPH-1),cnt); //tick
      setBlackPixel(max(xzero-3,0),cnt); //tick
      setBlackPixel(min(xzero+3,SCREEN_WIDTH_GRAPH-1),cnt); //tick
    }  
  }

  force_refresh();
  #endif
}


void plotline(uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn) {              // Plots line from xo,yo to xn,yn; uses temporary x1,y1
   pixelline(xo,yo,xn,yn,1);
 }


void pixelline(uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn, bool_t vmNormal) {              // Plots line from xo,yo to xn,yn; uses temporary x1,y1
    uint16_t x1;  //range 0-399
    uint8_t  y1;  //range 0-239
    #ifdef STATDEBUG
    printf("pixelline: xo,yo,xn,yn: %d %d   %d %d \n",xo,yo,xn,yn);
    #endif
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
    } else {
        if(vmNormal) placePixel(xn,yn); else removePixel(xn,yn);
    }
  }


//####################################################
//######### PLOT MEM #################################
//####################################################

void graph_plotmem(void) {
  #ifndef TESTSUITE_BUILD

  uint16_t cnt, ix, statnum;
  uint16_t xo, xn, xN; 
  uint8_t yo, yn, yN;
  graphtype x; 
  graphtype y;
  graphtype sx, sy;
  graphtype ddx = FLoatingMax;
  graphtype dydx = FLoatingMax;
  graphtype inty = 0;
  graphtype inty0 = 0;
  graphtype inty_off = 0;
  graphtype rmsy = 0;

//printf("TEST %d %d\n",screen_window_x(-0.405573,0.45,0.689633), screen_window_y(-0.405573,0.45,0.689633));
//printf("TEST %d %d\n",screen_window_x(0,1,1), screen_window_y(0,1,1));

  void plotDiff(void) {
    if(ddx != 0) dydx = (grf_y(ix) - grf_y(ix-1)) / ddx; else dydx = FLoatingMax;  //Differential
  }

  void plotInt(void) {
    inty = inty + (grf_y(ix) + grf_y(ix-1)) / 2 * ddx;
  }

  void plotRms(void) {
    rmsy = sqrt (
    	          ( 
    	          	(grf_x(ix-1)-grf_x(0)) * rmsy * rmsy +
    	         (  ddx * (grf_y(ix) + grf_y(ix-1) ) / 2   *   (grf_y(ix) + grf_y(ix-1) ) / 2) 
    	                                                                                     )  / (grf_x(ix)-grf_x(0)));
  }

  statnum = 0;

  graph_axis();                        //Draw the axis on any uncontrolled scale to start. Maybe optimize by remembering if there is an image on screen Otherwise double axis draw.

  if(jm_VECT || jm_NVECT) {plotmode = _VECT;} else {plotmode = _SCAT;}

  if(telltale == MEM_INITIALIZED && checkMinimumDataPoints(const_2)) {

    runFunction(ITM_NSIGMA);

    if(plotmode != _VECT) {
      realToInt32(SIGMA_N, statnum);
    } else {
      statnum = ix_count;
    }
   
    runFunction(ITM_DROP);

    #ifdef STATDEBUG
    printf("statnum n=%d\n",statnum);
    #endif 
  }

    runFunction(ITM_XRMS);                                       //Determine the RMS of the y for an arbitrary integral offset
    runFunction(ITM_DROP);
    real34ToString(REGISTER_REAL34_DATA(REGISTER_X), tmpString);
    inty_off = strtof (tmpString, NULL);
    runFunction(ITM_DROP);



  if(telltale == MEM_INITIALIZED && statnum >= 2) {
    //GRAPH SETUP
//    if(!Aspect_Square) {
//      clearScreen();
  //    refreshStatusBar();
//    }

    //AUTOSCALE
    x_min = FLoatingMax;
    x_max = FLoatingMin;
    y_min = FLoatingMax;
    y_max = FLoatingMin;
    #ifdef STATDEBUG
    printf("Axis0: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif
    if(plotmode != _VECT) {

      invalid_intg = false;                                                      //integral scale
      invalid_diff = false;                                                      //Differential dydx scale
      invalid_rms  = false;                                                      //RMSy

//#################################################### vvv SCALING LOOP DIFF INTG RMS vvv #########################
/**/      if(PLOT_DIFF || PLOT_INTG || PLOT_RMS) {
/**/        inty = inty_off;                                                          //  integral starting constant co-incides with graph
/**/
/**/        for (ix = 0; (ix < LIM && ix < statnum); ++ix) { 
/**/          if(ix !=0){
/**/            ddx = grf_x(ix) - grf_x(ix-1);                                            //used in DIFF and INT
/**/            if(ddx<=0) {                                                              //Cannot get slop or area if x is not growing in positive dierection
/**/      		    x_min = FLoatingMax;
/**/      		    x_max = FLoatingMin;
/**/      		    y_min = FLoatingMax;
/**/      		    y_max = FLoatingMin;
/**/      		    invalid_diff = true;
/**/                invalid_intg = true;
/**/                invalid_rms  = true;
/**/      		    break;
/**/            } else {          	
/**/              if(grf_x(ix) < x_min) {x_min = grf_x(ix);}
/**/              if(grf_x(ix) > x_max) {x_max = grf_x(ix);}
/**/              if(PLOT_DIFF) {
/**/                plotDiff(); //dydx                                            //Differential
/**/                if(dydx < y_min) {y_min = dydx;}
/**/                if(dydx > y_max) {y_max = dydx;}
/**/              }
/**/              if(PLOT_INTG) {
/**/                plotInt();   //inty                                          //integral
/**/                if(inty < y_min) {y_min = inty;}
/**/                if(inty > y_max) {y_max = inty;}
/**/              }
/**/              if(PLOT_RMS) {
/**/                plotRms();   //inty                                          //integral
/**/                if(rmsy < y_min) {y_min = rmsy;}
/**/                if(rmsy > y_max) {y_max = rmsy;}
/**/              }
/**/
/**/            }
/**/          } 
/**/        }
/**/      }
//#################################################### ^^^ SCALING LOOP ^^^ #########################


      #ifdef STATDEBUG
      printf("Axis0b1: x: %f -> %f y: %f -> %f  %d \n",x_min, x_max, y_min, y_max, invalid_diff);
      #endif

//#################################################### vvv SCALING LOOP  vvv #########################
/**/       if(PLOT_BOX || PLOT_LINE || PLOT_CROSS || !(PLOT_DIFF || PLOT_INTG)) {  //XXXX
/**/        for(cnt=0; (cnt < LIM && cnt < statnum); cnt++) {
/**/          #ifdef STATDEBUG
/**/          printf("Axis0a: x: %f y: %f   \n",grf_x(cnt), grf_y(cnt));   
/**/          #endif
/**/          if(grf_x(cnt) < x_min) {x_min = grf_x(cnt);}
/**/          if(grf_x(cnt) > x_max) {x_max = grf_x(cnt);}
/**/          if(grf_y(cnt) < y_min) {y_min = grf_y(cnt);}
/**/          if(grf_y(cnt) > y_max) {y_max = grf_y(cnt);}
/**/          #ifdef STATDEBUG
/**/          printf("Axis0b: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
/**/          #endif
/**/        }
/**/      }
/**/    } 
/**/
/**/    else {                 //VECTOR
/**/      sx =0;
/**/      sy =0;
/**/      for(cnt=0; (cnt < LIM && cnt < statnum); cnt++) {            //### Note XXX E- will stuff up statnum!
/**/        sx = sx + grf_x(cnt);
/**/        sy = sy + grf_y(cnt);
/**/        if(sx < x_min) {x_min = sx;}
/**/        if(sx > x_max) {x_max = sx;}
/**/        if(sy < y_min) {y_min = sy;}
/**/        if(sy > y_max) {y_max = sy;}
/**/      }      
/**/    }
//#################################################### ^^^ SCALING LOOP ^^^ #########################


    //Manipulate the obtained axes positions

    #ifdef STATDEBUG
    printf("Axis1a: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif

    //Check and correct if min and max is swapped
    if(x_min>0 && x_min > x_max*0.99) {x_min = x_min - (-x_max+x_min)* 1.1;}
    if(x_min<0 && x_min > x_max*0.99) {x_min = x_min + (-x_max+x_min)* 1.1;}


    #ifdef STATDEBUG
    printf("Axis1b: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif


    //Always include the 0 axis
    if(!extentx) {
      if(x_min>0 && x_max>0) {if(x_min<=x_max) {x_min = -0.05*x_max;} else {x_min = 0;}}
      if(x_min<0 && x_max<0) {if(x_min>=x_max) {x_min = -0.05*x_max;} else {x_max = 0;}}
    }
    if(!extenty) {
      if(y_min>0 && y_max>0) {if(y_min<=y_max) {y_min = -0.05*y_max;} else {y_min = 0;}}
      if(y_min<0 && y_max<0) {if(y_min>=y_max) {y_min = -0.05*y_max;} else {y_max = 0;}}
    }

    //Cause scales to be the same    
    if(jm_SCALE) {
      x_min = min(x_min,y_min);
      x_max = max(x_max,y_max);
      y_min = x_min;
      y_max = x_max;
    }

    #ifdef STATDEBUG
    printf("Axis2: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif

    //Create a visible axis on the edge
//    x_min = 1.05 * x_min;
//    x_max = 1.05 * x_max;
//    y_min = 1.05 * y_min;
//    y_max = 1.05 * y_max;

    float dx = x_max-x_min;
    float dy = y_max-y_min;

    if (dy == 0) {
      dy = 1;
      y_max = y_min + dy/2;
      y_min = y_max - dy;
    }
    if (dx == 0) {
      dx = 1;
      x_max = x_min + dx/2;
      x_min = x_max - dx;
    }
   
    x_min = x_min - dx * 0.015;
    y_min = y_min - dy * 0.015;
    x_max = x_max + dx * 0.015;
    y_max = y_max + dy * 0.015;


    #ifdef STATDEBUG
    printf("Axis3a: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif

    graph_axis();

    #ifdef STATDEBUG
    printf("Axis3b: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif


    if(plotmode != _VECT) {
      yn = screen_window_y(y_min,grf_y(0),y_max);
      xn = screen_window_x(x_min,grf_x(0),x_max);
      xN = xn;
      yN = yn;
    } else {
      yn = screen_window_y(y_min,0,y_max);
      xn = screen_window_x(x_min,0,x_max);
      xN = xn;
      yN = yn;      
    }

    #ifdef STATDEBUG
    printf("Axis3c: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif

    sx = 0;
    sy = 0;
    //GRAPH
    ix = 0;
    inty = inty_off;                                                         //  integral starting constant co-incides with graph
    rmsy = 0;

//#################################################### vvv MAIN GRAPH LOOP vvv #########################
/**/    for (ix = 0; (ix < LIM && ix < statnum); ++ix) {
/**/      if(plotmode != _VECT) {
/**/
/**/        x = 0;
/**/        y = 0;
/**/
/**/        if(ix !=0 && ( (PLOT_DIFF && !invalid_diff) || (PLOT_INTG && !invalid_intg) || (PLOT_RMS && !invalid_rms) )) {                                                               //Differential dydx
/**/          ddx = grf_x(ix) - grf_x(ix-1);
/**/          plotDiff();   //dydx                                          //Differential
/**/          inty0 = inty;
/**/          plotInt();    //inty                                         //integral
/**/          plotRms();    //RMSy
/**/          x = (grf_x(ix) + grf_x(ix-1))/2;
/**/          if(PLOT_DIFF) y = dydx;                 //y is the default graph
/**/          if(PLOT_RMS)  y = rmsy;                 //y is the default graph
/**/          if(PLOT_INTG) y = inty;                 //y is the default graph
/**/        }
/**/
/**/        if(PLOT_BOX || PLOT_LINE || PLOT_CROSS) {
/**/          x = grf_x(ix);
/**/          y = grf_y(ix);
/**/        }
/**/
/**/      } else {
/**/        sx = sx + grf_x(ix);
/**/        sy = sy + grf_y(ix);
/**/        x = sx;
/**/        y = sy;
/**/      }
/**/      xo = xN;
/**/      yo = yN;
/**/      xN = screen_window_x(x_min,x,x_max);
/**/      yN = screen_window_y(y_min,y,y_max);
/**/
/**/      #ifdef STATDEBUG
/**/      printf("plotting graph table[%d] = x:%f y:%f dydx:%f inty:%f xN:%d yN:%d ",ix,x,y,dydx,inty,  xN,yN);
/**/      printf(" ... x-ddx/2=%d dydx=%d inty=%d\n",screen_window_x( x_min, x-ddx/2, x_max), screen_window_y( y_min, dydx, y_max), screen_window_y( y_min, inty, y_max) );
/**/      #endif
/**/
/**/      int16_t minny,minnx;
/**/      if (!Aspect_Square) {minny = Y_non_square_min; minnx = 0;}
/**/      else {minny = 0; minnx = SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;}
/**/      if(xN<SCREEN_WIDTH_GRAPH && xN>minnx && yN<SCREEN_HEIGHT_GRAPH && yN>minny) {
/**/  //      yo = yn;                              //old , new, to be able to draw a line between samples
/**/        yn = yN;
/**/  //      xo = xn;
/**/        xn = xN;
/**/
/**/        #ifdef STATDEBUG
/**/          printf("invalid_diff=%d invalid_intg=%d invalid_rms=%d \n",invalid_diff,invalid_intg,invalid_rms);
/**/        #endif
/**/
/**/
/**/        if(plotmode != _VECT) {
/**/
/**/          #ifdef STATDEBUG
/**/            printf("Not _VECT\n");
/**/          #endif
/**/
/**/          if(PLOT_CROSS) {
/**/            #ifdef STATDEBUG
/**/              printf("Plotting cross to x=%d y=%d\n",xn,yn);
/**/            #endif
/**/            plotcross(xn,yn);
/**/          }
/**/
/**/          if(PLOT_BOX) {
/**/            #ifdef STATDEBUG
/**/              printf("Plotting box to x=%d y=%d\n",xn,yn);
/**/            #endif
/**/            plotbox(xn,yn);
/**/          }
/**/          //else placePixel(xn,yn);
/**/
/**/          if(PLOT_DIFF && !invalid_diff && ix != 0) {
/**/            #ifdef STATDEBUG
/**/              printf("Plotting Delta x=%f dy=%f \n",x-ddx/2,dydx);
/**/            #endif
/**/            plotdelta( screen_window_x( x_min, x-ddx/2, x_max), screen_window_y( y_min, dydx, y_max) );
/**/          }
/**/
/**/
/**/          if(PLOT_RMS && !invalid_rms && ix != 0) {
/**/            #ifdef STATDEBUG
/**/              printf("Plotting RMSy x=%f rmsy=%f \n",x-ddx/2,rmsy);
/**/            #endif
/**/            plotrms( screen_window_x( x_min, x-ddx/2, x_max), screen_window_y( y_min, rmsy, y_max) );
/**/          }
/**/
/**/
/**/
/**/          if(PLOT_INTG && !invalid_intg && ix !=0) {
/**/            #ifdef STATDEBUG
/**/              printf("Plotting Integral x=%f intg(x)=%f\n",x-ddx/2,inty);
/**/            #endif
/**/            uint16_t xN0   =screen_window_x( x_min, grf_x(ix-1), x_max);
/**///          uint16_t xN1   =screen_window_x( x_min, grf_x(ix), x_max);
/**/            uint16_t yNoff =screen_window_y( y_min, inty_off, y_max);
/**/            uint16_t yN0   =screen_window_y( y_min, inty0, y_max);
/**/            uint16_t yNintg=screen_window_y( y_min, inty, y_max);
/**/            uint16_t xAvg  =((xN0+xN) >> 1);
/**/            
/**/            if(abs((int16_t)(xN-xN0)>=6)) {plotint( xAvg, yNintg );} else
/**/                                          {//placePixel( xAvg, yNintg );
/**/                                           plotrect(xAvg-1, yNintg-1, xAvg+1, yNintg+1);}
/**/
/**/            if(abs((int16_t)(xN-xN0)>=6)) {plotline(xN, yNintg, xAvg+2, yNintg);plotline(xAvg-2, yNintg, xN0, yNintg);} else
/**/            if(abs((int16_t)(xN-xN0)>=4)) {plotline(xN, yNintg, xAvg+2, yNintg);plotline(xAvg-2, yNintg, xN0, yNintg);}
/**/
/**/            if(PLOT_SHADE) {
/**/              if(abs((int16_t)(xN-xN0)>=6)) {
/**/                plotrect(xN0,yNoff,xN,yN0);
/**/                plotrect(xN0,yN0,xN,yNintg);
/**/                plotline(xN0,yN0,xN,yNintg);
/**/              } else {
/**/                plotrect(xN0,yNoff,xN,yNintg);
/**/                plotrect(xN0,yN0,xN,yNintg);
/**/              }
/**/            }
/**/
/**/
/**/          }
/**/
/**/        } else {
/**/          #ifdef STATDEBUG
/**/            printf("Plotting arrow\n");
/**/          #endif
/**/          plotarrow(xo, yo, xn, yn);
/**/        }
/**/
/**/        if(PLOT_LINE) {
/**/          #ifdef STATDEBUG
/**/            printf("Plotting line to x=%d y=%d\n",xn,yn);
/**/          #endif
/**/          plotline(xo, yo, xn, yn);
/**/        }
/**/
/**/      } else {
/**/        #ifdef PC_BUILD
/**/          printf("Not plotted: ");
/**/          if(!(xN<SCREEN_WIDTH_GRAPH )) printf("xN<SCREEN_WIDTH_GRAPH; ");
/**/          if(!(xN>minnx              )) printf("xN>minnx; ");
/**/          if(!(yN<SCREEN_HEIGHT_GRAPH)) printf("yN<SCREEN_HEIGHT_GRAPH");
/**/          if(!(yN>1+minny            )) printf("yN>1+minny; ");
/**/                    printf("Not plotted: xN=%d<SCREEN_WIDTH_GRAPH=%d && xN=%d>minnx=%d && yN=%d<SCREEN_HEIGHT_GRAPH=%d && yN=%d>1+minny=%d\n",xN,SCREEN_WIDTH_GRAPH,xN,minnx,yN,SCREEN_HEIGHT_GRAPH,yN,1+minny);
/**/        #endif
/**/      }
/**/    }
//#################################################### ^^^ MAIN GRAPH LOOP ^^^ #########################


  } else {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "There is no statistical data available!");
      moreInfoOnError("In function graph_plotmem:", errorMessage, NULL, NULL);
    #endif
  }
#endif
}




//-----------------------------------------------------//-----------------------------------------------------


void fnStatList() {
  #ifndef TESTSUITE_BUILD
  char tmpstr1[100], tmpstr2[100];
  int16_t ix, ixx, statnum;

    if(telltale == MEM_INITIALIZED) {
      clearScreen();
      refreshStatusBar();
    }

  if(jm_VECT || jm_NVECT) {plotmode = _VECT;} else {plotmode = _SCAT;}

  if(telltale == MEM_INITIALIZED) {
    runFunction(ITM_NSIGMA);
    if(plotmode != _VECT) {
      realToInt32(SIGMA_N, statnum);
      sprintf(tmpString, "Stat data: N = %d",statnum);
    } else {
      statnum = ix_count;
      sprintf(tmpString, "Stat data: Vector IndexCount = %d",statnum);
    }

    runFunction(ITM_DROP);
    print_linestr(tmpString,true);
    #ifdef STATDEBUG
      printf("Stat data %d - %d (%s)\n",statnum-1, max(0, statnum-1-6), tmpString );
    #endif

    if( statnum - 0 - 1 + ListXYposition > statnum-1) ListXYposition = 0; else
    if( statnum - (min(10,min(LIM, statnum))-1) - 1 + ListXYposition < 0) ListXYposition = - (statnum - (min(10,min(LIM, statnum))-1) - 1);

    for (ix = 0; (ix < min(10,min(LIM, statnum))); ++ix) {
      ixx = statnum - ix - 1 + ListXYposition;

      if(((fabs(grf_x(ixx)) > 0.000999 || grf_x(ixx) == 0) && fabs(grf_x(ixx)) < 1000000)) 
        sprintf(tmpstr1,"[%3d] x%19.7f, ",ixx+1, grf_x(ixx));
      else
        sprintf(tmpstr1,"[%3d] x%19.7e, ",ixx+1, round(grf_x(ixx)*1e10)/1e10);

      if(((fabs(grf_y(ixx)) > 0.000999 || grf_y(ixx) == 0) && fabs(grf_y(ixx)) < 1000000))
        sprintf(tmpstr2,"y%19.7f", grf_y(ixx));
      else
        sprintf(tmpstr2,"y%19.7e", round(grf_y(ixx)*1e10)/1e10);

      strcat(tmpstr1,tmpstr2);

      print_numberstr(tmpstr1,false);
      #ifdef STATDEBUG
        printf("%d:%s\n",ixx,tmpstr1);
      #endif
    }
  }
  #endif
}





