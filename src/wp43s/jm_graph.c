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
 * \file jmgraph.c Graphing module
 ***********************************************/

/* ADDITIONAL C43 functions and routines */


#include "wp43s.h"
#include "math.h"

//#define STATDEBUG


//Note: graph_xmin, graph_xmax set from X.FN GRAPH

void autoscale(void) {
}

void defaultscale(void) {
}





void Fn_Lbl_A(void) {                                   //Temporary RPN function
	  fnAngularMode(AM_RADIAN);                           //Does not belong here -- it is repeated. It is convenient.
    fnStore(99);             // STO 99

    // (sin(x)/x + sin(10x)/5) / 2 + 2/x 
    fnSin(0);                // SIN
    fnRCL(99);               // RCL 99
    fnDivide(0);             // /
 
    fnRCL(99);               // RCL 99
    fnStrInputReal34("10");  // 10
    fnMultiply(0);           // *
    fnSin(0);                // SIN
    fnStrInputReal34("5");   // 5
    fnDivide(0);             // /
 
    fnAdd(0);                // +    

    fnStrInputReal34("2");   // 2
    fnDivide(0);             // /

    fnStrInputReal34("2");   // 2
    fnRCL(99);               // RCL 99
    fnDivide(0);             // /
    fnAdd(0);                // +    
}



void Fn_Lbl_B(void) {                                   //Temporary RPN function
	  fnAngularMode(AM_RADIAN);                           //Does not belong here -- it is repeated. It is convenient.
    fnStore(99);             // STO 99
    fnSin(0);                // SIN    
    fnRCL(99);
    fnSquare(0);             // square
    fnSin(0);                // SIN
    fnAdd(0);                // +    
}


void Fn_Lbl_C(void) {                                   //Temporary RPN function
//    fnAngularMode(AM_RADIAN);                           //Does not belong here -- it is repeated. It is convenient.
//    fnStore(99);             // STO 99
//    fnSin(0);                // SIN    
//    fnRCL(99);
//    fnDivide(0);             // /
    fnSinc(0);

}







//FOR STAT GRANH. LEAVES ANSWER IN X & Y
void Fn_Lbl_D(void) {                                   //Temporary RPN function 0.1x 
#ifndef TESTSUITE_BUILD
    fnStore(99);             // STO 99                   ( 0.1x^2 + 0.15x + (rand-0.5) * 0.4 ) + 2.5
    runFunction(ITM_ENTER);  // ENTER          // /
    fnStrInputReal34("0.1"); // 0.1
    runFunction(ITM_MULT);   // *              // /
    runFunction(ITM_MULT);   // *              // /
    fnRCL(99);               // RCL 99
    fnStrInputReal34("0.15"); // 0.15
    runFunction(ITM_MULT);    // *             // /
    runFunction(ITM_ADD);     // +             // /

    runFunction(ITM_RAN);     // fnRandom(0);  //    
    fnStrInputReal34("-0.5"); // -0.5
    runFunction(ITM_ADD);     // +             // /

    fnStrInputReal34("0.4");  // 0.4
    runFunction(ITM_MULT);    // *             // /

    runFunction(ITM_ADD);     // +             // /
    fnStrInputReal34("2.5");  // 2.5
    runFunction(ITM_ADD);     // +             // /
    fnRCL(99);
#endif
}


void Fn_Lbl_E(void) {                                   //Temporary RPN function
#ifndef TESTSUITE_BUILD
    fnStore(99);             // STO 99
    fnStrInputReal34("0.6"); // 0.2
    runFunction(ITM_MULT);   // *             // /

    runFunction(ITM_RAN);    // fnRandom(0);                //    
    fnStrInputReal34("-0.5"); // -0.5
    runFunction(ITM_ADD);    // +             // /

    fnStrInputReal34("0.4"); // 0.4
    runFunction(ITM_MULT);   // *             // /

    runFunction(ITM_ADD);    // +             // /
    fnRCL(99);               //leaving y in Y and x in X
#endif
}


void Fn_Lbl_F(void) {                                   //Temporary RPN function
#ifndef TESTSUITE_BUILD
    fnStrInputReal34("-1"); // -0.5
    runFunction(ITM_ADD);    // +             // /

    fnStore(99);             // STO 99
    fnStrInputReal34("12"); // 0.2
    runFunction(ITM_MULT);   // *             // /

    runFunction(ITM_sin);    // fnRandom(0);                //    
    fnRCL(99);               //leaving y in Y and x in X
    fnStrInputReal34("12"); // 0.2
    runFunction(ITM_MULT);   // *             // /
    runFunction(ITM_cos);    // fnRandom(0);                //    
#endif
}



void execute_rpn_function(int16_t nbr){
  #ifdef STATDEBUG
  printf("nbr=%d   x=%s \n",nbr,tmpStr3000);
  #endif

  if(nbr == 1) {
    Fn_Lbl_A();
  } else
  if(nbr == 2) {
    Fn_Lbl_B();
  } else
  if(nbr == 3) {
    Fn_Lbl_C();
  } else
  if(nbr == 4) {
    Fn_Lbl_D();
  } else
  if(nbr == 5) {
    Fn_Lbl_E();
  } else
  if(nbr == 6) {
    Fn_Lbl_F();
  }
}





void graph_setupmemory(void) {
  int i;
  if(telltale != MEM_INITIALIZED) {
    gr_x = (float*)malloc(LIM * sizeof(float)); 
    gr_y = (float*)malloc(LIM * sizeof(float)); 
    telltale = MEM_INITIALIZED;
    ix_count = 0;
  }
  if ((telltale != MEM_INITIALIZED) || (gr_x == NULL || gr_y == NULL)) { 
  #ifdef PC_BUILD
     showInfoDialog("In function graph_setupmemory:", "error allocating memory for graph!", NULL, NULL);
     exit(1);
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







void graph_sigmaplus(int8_t plusminus, real_t *xx, real_t *yy) {    //Called from STAT module from fnSigma(), to store the x,y pair to the memory structure.
  int16_t cnt;
  float x; 
  float y;

  if(jm_VECT) {plotmode = _VECT;} else {plotmode = _SCAT;}

  if(telltale != MEM_INITIALIZED) {
    graph_setupmemory();
//    runFunction(ITM_CLSIGMA);
  }


  //Convert from X register to float
  realToString(yy, tmpStr3000);
  y = strtof (tmpStr3000, NULL);

  //printf("y=%f ",y);

  //Convert from X register to float
  realToString(xx, tmpStr3000);
  x = strtof (tmpStr3000, NULL);

  //printf("x=%f ",x);

  #ifndef TESTSUITE_BUILD
  test_xy(x,y);               //Write to CSV file
  #endif

  if(plotmode == _VECT ) {
    ix_count++;               //Only used for VECT
    cnt = ix_count;
  } else {
    //Convert from real to int
    realToString(SIGMA_N, tmpStr3000);
    cnt = atoi (tmpStr3000);
    #ifdef STATDEBUG
    printf("Count: %s, %d\n",tmpStr3000,cnt);
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
int16_t screen_window_x(float x_min, float x, float x_max) {
int16_t temp;
if (Aspect_Square) {
  temp = (x-x_min)/(x_max-x_min)*(SCREEN_HEIGHT_GRAPH-1);
  if (temp>SCREEN_HEIGHT_GRAPH-1) {temp=SCREEN_HEIGHT_GRAPH-1;}
  else if (temp<0) {temp=0;}
  return temp+SCREEN_WIDTH-SCREEN_HEIGHT_GRAPH;

  } else 

  if (!Aspect_Square) {  //FULL SCREEN
    temp = ((x-x_min)/(x_max-x_min)*(SCREEN_WIDTH_GRAPH-1));
    //printf("--> %d (%f %f)  ",temp, x_min,x_max);
    if (temp>SCREEN_WIDTH_GRAPH-1) {temp=SCREEN_WIDTH_GRAPH-1;}
    else if (temp<0) {temp=0;}
    //printf("--> %d \n",temp);
    #ifdef PC_BUILD
      if(temp<0 || temp>399) {printf("In function screen_window_x JM X EXCEEDED %d",temp);}
    #endif
    return temp;
  }
}
#endif

int16_t screen_window_y(float y_min, float y, float y_max) {
int16_t temp;
  temp = (y-y_min)/(y_max-y_min)*(SCREEN_HEIGHT_GRAPH-1 - SCREEN_MIN_GRAPH);
    if (temp>SCREEN_HEIGHT_GRAPH-1 - SCREEN_MIN_GRAPH) {temp=SCREEN_HEIGHT_GRAPH-1 - SCREEN_MIN_GRAPH;}
  else if (temp<0) {temp=0;}

  #ifdef PC_BUILD
    if(SCREEN_HEIGHT_GRAPH-1 - temp<0 || SCREEN_HEIGHT_GRAPH-1 - temp>239) {printf("In function screen_window_y JM Y EXCEEDED %d %d",temp,SCREEN_HEIGHT_GRAPH-1 - temp);}
  #endif
  return (SCREEN_HEIGHT_GRAPH-1 - temp);
}



//###################################################################################
//GRAPH  // Build test data 
void graph_demo(uint8_t nbr, float x_min, float x_max) {
  #ifndef TESTSUITE_BUILD
  float x; 
  fnClearStack(0);

  if(telltale != MEM_INITIALIZED) {
    graph_setupmemory();
    runFunction(ITM_CLSIGMA);
  }

  fnAngularMode(AM_RADIAN);

  //  for(x=x_min; x<=x_max; x+=(x_max-x_min)/SCREEN_WIDTH_GRAPH) {
  for(x=x_min; x<=x_max; x+=0.99999*(x_max-x_min)/SCREEN_WIDTH_GRAPH*10) {    //Reduxced the amount of sample data from 400 points to 40 points

    //convert float to X register
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
    snprintf(tmpStr3000, sizeof(tmpStr3000), "%.16f", x);
    stringToReal34(tmpStr3000, REGISTER_REAL34_DATA(REGISTER_X));

    //leaving y in Y and x in X
    execute_rpn_function(nbr);
    fnSigma(1);
  }
  fnClearStack(0);
  runFunction(ITM_SIGMAx);
  #endif
}

//###################################################################################
void placePixel(uint16_t x, uint8_t y) {
#ifndef TESTSUITE_BUILD
  if(x<SCREEN_WIDTH_GRAPH && x>0 && y<SCREEN_HEIGHT_GRAPH && y>1+SCREEN_MIN_GRAPH) {
    setPixel(x,y);
  }
#endif
}


//###################################################################################
float auto_tick(float tick_int_f) {
    //Obtain scaling of ticks, to about 20 intervals left to right.
  //float tick_int_f = (x_max-x_min)/20;                                                 //printf("tick interval:%f ",tick_int_f);
  snprintf(tmpStr3000, sizeof(tmpStr3000), "%.1e", tick_int_f);
  char tx[4];
  tx[0] = tmpStr3000[0];
  tx[1] = tmpStr3000[1];
  tx[2] = tmpStr3000[2];
  tx[3] = 0;
  //printf("tick0 %f orgstr %s tx %s \n",tick_int_f, tmpStr3000, tx);
  tick_int_f = strtof (tx, NULL);                                        //printf("string:%s converted:%f \n",tmpStr3000, tick_int_f);
  //printf("tick1 %f orgstr %s tx %s \n",tick_int_f, tmpStr3000, tx);
  if(tick_int_f > 0   && tick_int_f <=  0.3)  {tmpStr3000[0] = '0'; tmpStr3000[2]='2'; } else
  if(tick_int_f > 0.3 && tick_int_f <=  0.6)  {tmpStr3000[0] = '0'; tmpStr3000[2]='5'; } else
  if(tick_int_f > 0.6 && tick_int_f <=  1.3)  {tmpStr3000[0] = '1'; tmpStr3000[2]='0'; } else
  if(tick_int_f > 1.3 && tick_int_f <=  1.7)  {tmpStr3000[0] = '1'; tmpStr3000[2]='5'; } else
  if(tick_int_f > 1.7 && tick_int_f <=  3.0)  {tmpStr3000[0] = '2'; tmpStr3000[2]='0'; } else
  if(tick_int_f > 3.0 && tick_int_f <=  6.5)  {tmpStr3000[0] = '5'; tmpStr3000[2]='0'; } else
  if(tick_int_f > 6.5 && tick_int_f <=  9.9)  {tmpStr3000[0] = '7'; tmpStr3000[2]='5'; }
  tick_int_f = strtof (tmpStr3000, NULL);                                        //printf("string:%s converted:%f \n",tmpStr3000, tick_int_f);
  //printf("tick2 %f str %s tx %s \n",tick_int_f, tmpStr3000, tx);
  return tick_int_f;
}
//--------------------------------------------------------------------------------



//###################################################################################
  float tick_int_x;
  float tick_int_y;

  float x_min; 
  float x_max;
  float y_min;
  float y_max;
//  float tick_x;
//  float tick_y;
  uint16_t xzero;
  uint8_t yzero;



//###################################################################################
void graph_axis (void){
  #ifndef TESTSUITE_BUILD
  uint16_t cnt;

  //GRAPH RANGE
  //  graph_xmin= -3*3.14150;  graph_xmax= 2*3.14159;
  //  graph_ymin= -2;          graph_ymax= +2;


  //GRAPH ZERO AXIS
  yzero = screen_window_y(y_min,0,y_max);
  xzero = screen_window_x(x_min,0,x_max);

  //DRAW AXIS
  cnt = 0;  
  while (cnt!=SCREEN_WIDTH_GRAPH-1) { 
      setPixel(cnt,yzero); 
      cnt++; 
    }
  cnt = SCREEN_MIN_GRAPH;  
  while (cnt!=SCREEN_HEIGHT_GRAPH-1) { 
      setPixel(xzero,cnt); 
      cnt++; 
    }

  force_refresh();

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

  snprintf(tmpStr3000, sizeof(tmpStr3000), "x: %.3f/div  y: %.3f/div", tick_int_x,tick_int_y);
  showString(tmpStr3000, &standardFont, 1, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true);  //JM

  float x; 
  float y;
  for(y=0; y<=y_max; y+=tick_int_y) {       //draw y ticks
    cnt = screen_window_y(y_min,y,y_max);
    setPixel(max(xzero-1,0),cnt); //tick
    setPixel(min(xzero+1,SCREEN_WIDTH_GRAPH-1),cnt); //tick
  }  
  for(y=0; y>=y_min; y+=-tick_int_y) {       //draw y ticks
    cnt = screen_window_y(y_min,y,y_max);
    setPixel(max(xzero-1,0),cnt); //tick
    setPixel(min(xzero+1,SCREEN_WIDTH_GRAPH-1),cnt); //tick
  }  

  for(x=0; x<=x_max; x+=tick_int_x) {       //draw x ticks
    cnt = screen_window_x(x_min,x,x_max);
      setPixel(cnt,min(yzero+1,SCREEN_HEIGHT_GRAPH)); //tick
      setPixel(cnt,max(yzero-1,0)); //tick
   }
  for(x=0; x>=x_min; x+=-tick_int_x) {       //draw x ticks
    cnt = screen_window_x(x_min,x,x_max);
      setPixel(cnt,min(yzero+1,SCREEN_HEIGHT_GRAPH)); //tick
      setPixel(cnt,max(yzero-1,0)); //tick
   }

  for(y=0; y<=y_max; y+=tick_int_y*5) {       //draw y ticks
    cnt = screen_window_y(y_min,y,y_max);
    setPixel(max(xzero-2,0),cnt); //tick
    setPixel(min(xzero+2,SCREEN_WIDTH_GRAPH-1),cnt); //tick
    setPixel(max(xzero-3,0),cnt); //tick
    setPixel(min(xzero+3,SCREEN_WIDTH_GRAPH-1),cnt); //tick
  }  
  for(y=0; y>=y_min; y+=-tick_int_y*5) {       //draw y ticks
    cnt = screen_window_y(y_min,y,y_max);
    setPixel(max(xzero-2,0),cnt); //tick
    setPixel(min(xzero+2,SCREEN_WIDTH_GRAPH-1),cnt); //tick
    setPixel(max(xzero-3,0),cnt); //tick
    setPixel(min(xzero+3,SCREEN_WIDTH_GRAPH-1),cnt); //tick
  }  

  for(x=0; x<=x_max; x+=tick_int_x*5) {       //draw x ticks
    cnt = screen_window_x(x_min,x,x_max);
      setPixel(cnt,min(yzero+2,SCREEN_HEIGHT_GRAPH)); //tick
      setPixel(cnt,max(yzero-2,0)); //tick
      setPixel(cnt,min(yzero+3,SCREEN_HEIGHT_GRAPH)); //tick
      setPixel(cnt,max(yzero-3,0)); //tick
   }
  for(x=0; x>=x_min; x+=-tick_int_x*5) {       //draw x ticks
    cnt = screen_window_x(x_min,x,x_max);
      setPixel(cnt,min(yzero+2,SCREEN_HEIGHT_GRAPH)); //tick
      setPixel(cnt,max(yzero-2,0)); //tick
      setPixel(cnt,min(yzero+3,SCREEN_HEIGHT_GRAPH)); //tick
      setPixel(cnt,max(yzero-3,0)); //tick
   }

  force_refresh();
  #endif
}




//####################################################
//######### PLOT MEM #################################
//####################################################

void graph_plotmem(void) {
  #ifndef TESTSUITE_BUILD

      void plotline(uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn) {              // Plots line from xo,yo to xn,yn; uses temporary x1,y1
        uint16_t x1;  //range 0-399
        uint8_t  y1;  //range 0-239
        #ifdef STATDEBUG
        printf("%d %d   %d %d \n",xo,yo,xn,yn);
        #endif
        if(xo > xn) {
          for(x1=xo; x1!=xn; x1-=1) {
            y1 = yo + (x1-xo)*(yn-yo)/(xn-xo);
            placePixel(x1,y1);
          }
        } 
        else if(xo < xn) {
          for(x1=xo; x1!=xn; x1+=1) {
            y1 = yo + (x1-xo)*(yn-yo)/(xn-xo);
            placePixel(x1,y1);
          }
        }

        if(yo > yn) {
          for(y1=yo; y1!=yn; y1-=1) {
            x1 = xo + (y1-yo)*(xn-xo)/(yn-yo);
            placePixel(x1,y1);
          }
        } 
        else if(yo < yn) {
          for(y1=yo; y1!=yn; y1+=1) {
            x1 = xo + (y1-yo)*(xn-xo)/(yn-yo);
            placePixel(x1,y1);
          }
        } else {
          placePixel(xn,yn);
        }
      }


      void plotarrow(uint16_t xo, uint8_t yo, uint16_t xn, uint8_t yn) {              // Plots line from xo,yo to xn,yn; uses temporary x1,y1
        float dx, dy, ddx, ddy, zz, zzz;
        ddy = yn-yo;
        ddx = xn-xo;
        zz  = sqrt(ddy*ddy+ddx*ddx);
        zzz = 3;
        dy  = ddy * (zzz/zz);
        dx  = ddx * (zzz/zz);
        #ifdef STATDEBUG
        printf("%d %d  %d %d  ddx=%f, ddy=%f, zz=%f  zzz=%f, dx=%f, dy=%f \n",xo, yo, xn, yn, ddx,ddy,zz,zzz,dx,dy);
        #endif
        if (!(xo==xn && yo==yn)){
          plotline(xn+(-3*dx +dy), yn+(-3*dy -dx), xn, yn);
          plotline(xn+(-3*dx -dy), yn+(-3*dy +dx), xn, yn);
        } else {
          placePixel(xn,yn);
        }
      }


      void plotcross(uint16_t xn, uint8_t yn) {              // Plots line from xo,yo to xn,yn; uses temporary x1,y1
        placePixel(xn,yn);                     //PLOT a cross
        placePixel(xn-1,yn-1);
        placePixel(xn-1,yn+1);
        placePixel(xn+1,yn-1);
        placePixel(xn+1,yn+1);
        placePixel(xn-2,yn-2);
        placePixel(xn-2,yn+2);
        placePixel(xn+2,yn-2);
        placePixel(xn+2,yn+2);
      }



  uint16_t cnt, ix, statnum;
  uint16_t xo, xn, xN; 
  uint8_t yo, yn, yN;
  float x; 
  float y;
  float sx, sy;

  statnum = 0;
  if(jm_VECT) {plotmode = _VECT;} else {plotmode = _SCAT;}

  if(telltale == MEM_INITIALIZED) {

    runFunction(ITM_NSIGMA);

    if(plotmode != _VECT) {
      //Convert from real to int
      realToString(SIGMA_N, tmpStr3000);
      statnum = atoi (tmpStr3000);
    } else {
      statnum = ix_count;
    }
   
    runFunction(ITM_DROP);

    #ifdef STATDEBUG
    printf("n=%d\n",statnum);
    #endif 

  }


  if(telltale == MEM_INITIALIZED && statnum >= 2) {
    //GRAPH SETUP
    calcMode = CM_BUG_ON_SCREEN;              //Hack to prevent calculator to restart operation. Used to view graph
    clearScreen(false,true,true);

    //AUTOSCALE
    x_min = 1e127;
    x_max = -1e127;
    y_min = 1e127;
    y_max = -1e127;
    #ifdef STATDEBUG
    printf("Axis0: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif
    if(plotmode != _VECT) {
      for(cnt=0; (cnt < LIM && cnt < statnum); cnt++) {
        #ifdef STATDEBUG
        printf("Axis0a: x: %f y: %f   \n",gr_x[cnt], gr_y[cnt]);   
        #endif
        if(gr_x[cnt] < x_min) {x_min = gr_x[cnt];}
        if(gr_x[cnt] > x_max) {x_max = gr_x[cnt];}
        if(gr_y[cnt] < y_min) {y_min = gr_y[cnt];}
        if(gr_y[cnt] > y_max) {y_max = gr_y[cnt];}
        #ifdef STATDEBUG
        printf("Axis0b: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
        #endif
      }
    } else {
      sx =0;
      sy =0;
      for(cnt=0; (cnt < LIM && cnt < statnum); cnt++) {            //### Note XXX E- will stuff up statnum!
        sx = sx + gr_x[cnt];
        sy = sy + gr_y[cnt];
        if(sx < x_min) {x_min = sx;}
        if(sx > x_max) {x_max = sx;}
        if(sy < y_min) {y_min = sy;}
        if(sy > y_max) {y_max = sy;}
      }      
    }


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
    if(x_min>0 && x_max>0) {if(x_min<=x_max) {x_min = -0.05*x_max;} else {x_min = 0;}}
    if(x_min<0 && x_max<0) {if(x_min>=x_max) {x_min = -0.05*x_max;} else {x_max = 0;}}
    if(y_min>0 && y_max>0) {if(y_min<=y_max) {y_min = -0.05*x_max;} else {y_min = 0;}}
    if(y_min<0 && y_max<0) {if(y_min>=y_max) {y_min = -0.05*x_max;} else {y_max = 0;}}

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
    x_min = x_min - dx * 0.015;
    y_min = y_min - dy * 0.015;
    x_max = x_max + dx * 0.015;
    y_max = y_max + dy * 0.015;


    #ifdef STATDEBUG
    printf("Axis3: x: %f -> %f y: %f -> %f   \n",x_min, x_max, y_min, y_max);   
    #endif

    graph_axis();

      /* //Sample data
      gr_x[0] = 0; gr_y[0] = 1;
      gr_x[1] = 1; gr_y[1] = 0.7;
      gr_x[2] = 2; gr_y[2] = 0.9;
      gr_x[3] = 3; gr_y[3] = 0.2;
      gr_x[4] = 4; gr_y[4] = 0.2;
      gr_x[5] = 5; gr_y[5] = 0.3;
      gr_x[6] = 6; gr_y[6] = 0.301;
      statnum = 7;
      */


    if(plotmode != _VECT) {
      yn = screen_window_y(y_min,gr_y[0],y_max);
      xn = screen_window_x(x_min,gr_x[0],x_max);
      xN = xn;
      yN = yn;
    } else {
      yn = screen_window_y(y_min,0,y_max);
      xn = screen_window_x(x_min,0,x_max);
      xN = xn;
      yN = yn;      
    }

    sx = 0;
    sy = 0;
    //GRAPH
    ix = 0;
    for (ix = 0; (ix < LIM && ix < statnum); ++ix) {
      if(plotmode != _VECT) {
        x = gr_x[ix];
        y = gr_y[ix];
      } else {
        sx = sx + gr_x[ix];
        sy = sy + gr_y[ix];
        x = sx;
        y = sy;
      }
      #ifdef STATDEBUG
      printf("plotting graph table[%d] = x:%f y:%f \n",ix,x,y);
      #endif
      xo = xN;
      yo = yN;
      xN = screen_window_x(x_min,x,x_max);
      yN = screen_window_y(y_min,y,y_max);

      if(xN<SCREEN_WIDTH_GRAPH && xN>0 && yN<SCREEN_HEIGHT_GRAPH && yN>1+SCREEN_MIN_GRAPH) {
  //      yo = yn;                              //old , new, to be able to draw a line between samples
        yn = yN;
  //      xo = xn;
        xn = xN;


        if(plotmode != _VECT) {
            plotcross(xn,yn);
        } else {
            plotarrow(xo, yo, xn, yn);
        }

        //placePixel(xn,yn);
        plotline(xo, yo, xn, yn);

      }
    }


  } else {
    displayCalcErrorMessage(ERROR_NO_SUMMATION_DATA, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "There is no statistical data available!");
      showInfoDialog("In function graph_plotmem:", errorMessage, NULL, NULL);
    #endif
    refreshStack();
  }
#endif
}




//-----------------------------------------------------//-----------------------------------------------------
                    //-----------------------------------------------------//-----------------------------------------------------

                    void graph_draw(uint8_t nbr, float x_min, float x_max, float y_min, float y_max, float tick_x, float tick_y, uint16_t xzero, uint8_t yzero) {
                      #ifndef TESTSUITE_BUILD
                      uint16_t cnt;
                      real_t tmpy;
                      uint8_t yo; 
                      uint8_t yn;
                      yn = 0;
                      float x; 
                      float y;
                      uint16_t x1;  //range 0-399
                      uint8_t  y1;  //range 0-239

                      for(y=y_min; y<=y_max; y+=tick_y) {
                        cnt = screen_window_y(y_min,y,y_max);
                        setPixel(xzero-1,cnt); //tick
                        setPixel(xzero-2,cnt); //tick
                        setPixel(xzero+1,cnt); //tick
                        setPixel(xzero+1,cnt); //tick
                      }  

                      //GRAPH
                      cnt = 0;
                      for(x=x_min; x<=x_max; x+=(x_max-x_min)/SCREEN_WIDTH_GRAPH) {

                        float a_ft = (x/( tick_x));          //Draw ticks
                        if(a_ft<0) { a_ft=-a_ft; }
                        int16_t a_int = (int) a_ft;
                        float a_frac = a_ft - a_int;
                        if(a_frac < (x_max-x_min)/300) {               //Frac < 6.6 % is deemed close enough
                          setPixel(cnt,yzero+1); //tick
                          setPixel(cnt,yzero-1); //tick
                          setPixel(cnt,yzero+2); //tick
                          setPixel(cnt,yzero-2); //tick
                          force_refresh();
                        }

                        //convert float to X register
                        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, AM_NONE);
                        snprintf(tmpStr3000, sizeof(tmpStr3000), "%.16f", x);
                        stringToReal34(tmpStr3000, REGISTER_REAL34_DATA(REGISTER_X));

                        execute_rpn_function(nbr);

                        //Convert from X register to float
                        real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &tmpy);
                        realToString(&tmpy, tmpStr3000);
                        y = strtof (tmpStr3000, NULL);

                        yo = yn;   //old , new
                        yn = screen_window_y(y_min,y,y_max);

                        //printf("Calc: cnt = %d xy[%f %f]  yold->new(%d -> %d)\n",cnt, x, y, yo, yn);
                        cnt++;


                      if(cnt > 0) {       //Fill in all y coords if coords are skipped due to large dy/dx.
                          #ifndef TESTSUITE_BUILD
                          x1 = cnt-1;     //First half on cnt-1, second half on cnt. Not implemented yet., All on cnt-1.
                          #endif
                          if(yo > yn) {
                            for(y1=yo; y1!=yn; y1-=1) {
                              setPixel(x1,y1);
                            }
                          } 
                          else if(yo < yn) {
                            for(y1=yo; y1!=yn; y1+=1) {
                            setPixel(x1,y1);
                            }
                          } else {
                            setPixel(x1,yn);
                          }
                        setPixel(cnt,SCREEN_HEIGHT_GRAPH- (0)       );
                        setPixel(cnt,SCREEN_HEIGHT_GRAPH- (SCREEN_HEIGHT_GRAPH-SCREEN_MIN_GRAPH-1) );
                        }
                      }
                    fnDrop(0);
                    #endif
                    }



                    void fnGraph_old (uint16_t selection){
                      #ifndef TESTSUITE_BUILD
                      uint16_t cnt;

                      //GRAPH RANGE
                      //  graph_xmin= -3*3.14150;  graph_xmax= 2*3.14159;
                      //  graph_ymin= -2;          graph_ymax= +2;

                      fnClearStack(0);

                      //GRAPH SETUP
                      calcMode = CM_BUG_ON_SCREEN;              //Hack to prevent calculator to restart operation. Used to view graph
                      clearScreen(false,true,true);

                      //GRAPH ZERO AXIS
                      uint8_t  yzero;
                      uint16_t xzero;
                      yzero = screen_window_y(graph_ymin,0,graph_ymax);
                      xzero = screen_window_x(graph_xmin,0,graph_xmax);

                      //DRAW AXIS
                      cnt = 0;  
                      while (cnt!=SCREEN_WIDTH_GRAPH-1) { 
                          setPixel(cnt,yzero); 
                          cnt++; 
                        }
                      cnt = SCREEN_MIN_GRAPH;  
                      while (cnt!=SCREEN_HEIGHT_GRAPH-1) { 
                          setPixel(xzero,cnt); 
                          cnt++; 
                        }

                      force_refresh();

                      float tick_int_x;
                      if(graph_dx == 0) {
                        tick_int_x = auto_tick((graph_xmax-graph_xmin)/20);
                      } else {
                        tick_int_x = graph_dx;
                      }

                      float tick_int_y;
                      if(graph_dy == 0) {
                        tick_int_y = auto_tick((graph_ymax-graph_ymin)/20);
                      } else {
                        tick_int_y = graph_dy;
                      }

                      snprintf(tmpStr3000, sizeof(tmpStr3000), "x: %.3f/div  y: %.3f/div", tick_int_x,tick_int_y);
                      showString(tmpStr3000, &standardFont, 1, Y_POSITION_OF_REGISTER_T_LINE, vmNormal, true, true);  //JM
                      graph_draw(selection, graph_xmin, graph_xmax, graph_ymin, graph_ymax, tick_int_x, tick_int_y, xzero, yzero);

                      #endif
                    }

                    //-----------------------------------------------------//-----------------------------------------------------



void fnStatList(uint16_t unusedParamButMandatory) {
  char tmpstr[100];
  int16_t ix, ixx, statnum;

  #ifndef TESTSUITE_BUILD
  if(telltale == MEM_INITIALIZED) {
    //GRAPH SETUP
    calcMode = CM_BUG_ON_SCREEN;              //Hack to prevent calculator to restart operation. Used to view graph
    clearScreen(false,true,true);
  }
  #endif

  if(jm_VECT) {plotmode = _VECT;} else {plotmode = _SCAT;}

  if(telltale == MEM_INITIALIZED) {

    #ifndef TESTSUITE_BUILD
    runFunction(ITM_NSIGMA);
    #endif

    if(plotmode != _VECT) {
      //Convert from real to int
      realToString(SIGMA_N, tmpStr3000);
      statnum = atoi (tmpStr3000);
    } else {
      statnum = ix_count;
    }

    #ifndef TESTSUITE_BUILD
    runFunction(ITM_DROP);
    #endif
   
    print_linestr("Stat data",true);
    #ifdef STATDEBUG
    printf("Stat data %d - %d\n",statnum-1, max(0, statnum-1-6) );
    #endif

    for (ix = 0; (ix < min(10,min(LIM, statnum))); ++ix) {
      ixx = statnum - ix - 1;
      sprintf(tmpstr,"[%d]  x:%f,  y:%f",ixx+1, gr_x[ixx],gr_y[ixx]);
      print_linestr(tmpstr,false);
      #ifdef STATDEBUG
      printf("%d:%s\n",ixx,tmpstr);
      #endif
    }
  }
}



//-----------------------------------------------------//-----------------------------------------------------

#define randnum(min, max)  ((rand() % (int)(((max) + 1) - (min))) + (min))


void fnGraph (uint16_t func){
  switch (func) 
  {
	  case 1:   fnGraph_old(3);
              break;
	  case 3:   graph_demo(randnum(4,6), graph_xmin, graph_xmax);
	            break;
	  case 4:   graph_plotmem();
	            break;

    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:  graph_demo(func-10, graph_xmin, graph_xmax);
              break;

	  default:;
   }
}




