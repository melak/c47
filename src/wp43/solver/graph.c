// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: Copyright The WP43 and C47 Authors


/********************************************//** //JM
 * \file graph.c Graphing module
 ***********************************************/

#include "solver/graph.h"

#include "c43Extensions/addons.h"
#include "defines.h"
#include "mathematics/comparisonReals.h"
#include "charString.h"
#include "constantPointers.h"
#include "error.h"
#include "flags.h"
#include "c43Extensions/graphs.h"
#include "c43Extensions/graphText.h"
#include "items.h"
#include "mathematics/invert.h"
#include "mathematics/matrix.h"
#include "plotstat.h"
#include "recall.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "screen.h"
#include "softmenus.h"
#include "solver/equation.h"
#include "statusBar.h"
#include "stack.h"
#include "stats.h"
#include "store.h"
#include "wp43.h"
#include <math.h>
#include <string.h>


#if defined(PC_BUILD)
  //Verbose directives can be simulataneously selected
  //#define VERBOSE_SOLVER00   // minimal text
  //#define VERBOSE_SOLVER0  // a lot less text
  //#define VERBOSE_SOLVER1  // a lot less text
  //#define VERBOSE_SOLVER2  // verbose a lot
#else // !PC_BUILD
  #undef VERBOSE_SOLVER00
  #undef VERBOSE_SOLVER0
  #undef VERBOSE_SOLVER1
  #undef VERBOSE_SOLVER2
#endif // PC_BUILD


//Todo: involve https://en.wikipedia.org/wiki/Brent%27s_method#Brent's_method

#define COMPLEXKICKER true       //flag to allow conversion to complex plane if no convergenge found
#define CHANGE_TO_MOD_SECANT 0   //at iteration nn go to the modified secant method. 0 means immediately
#define CONVERGE_FACTOR 1.0f        //
#define NUMBERITERATIONS 35      // Must be smaller than LIM (see STATS)


#if !defined(TESTSUITE_BUILD)
static void fnPlot(uint16_t unusedButMandatoryParameter) {
    lastPlotMode = PLOT_NOTHING;
    strcpy(plotStatMx, "DrwMX");
    PLOT_LINE = true;
    PLOT_SHADE = true;
    fnPlotSQ(0);
}


static void convertDoubleToReal34RegisterPush(double x, calcRegister_t destination) {
  setSystemFlag(FLAG_ASLIFT);
  liftStack();
  convertDoubleToReal34Register(x, destination);
  setSystemFlag(FLAG_ASLIFT);
}


#if !defined(SAVE_SPACE_DM42_13GRF)
  static void initialize_function(void){
    if(graphVariable > 0) {
      #if defined(PC_BUILD)
        //printf(">>> graphVariable = %i\n", graphVariable);
        if(lastErrorCode != 0) {
          #if defined(VERBOSE_SOLVER00)
          printf("ERROR CODE in initialize_functionA: %u\n",lastErrorCode);
          #endif // VERBOSE_SOLVER00
          lastErrorCode = 0;
        }
      #endif //PC_BUILD
    }
    else {
      #if defined(PC_BUILD)
        //printf(">>> graphVariable = %i\n", graphVariable);
        #if defined(VERBOSE_SOLVER00)
          printf("ERROR CODE in initialize_functionB: %u\n",lastErrorCode);
        #endif // VERBOSE_SOLVER00
      #endif //PC_BUILD
    }
  }
#endif // !SAVE_SPACE_DM42_13GRF


static void execute_rpn_function(void){
  if(graphVariable <= 0 || graphVariable > 65535) {
    #if defined(PC_BUILD) //PC_BUILD
      printf("Error: No graph variable %u\n",graphVariable);
    #endif //PC_BUILD
    return;
  }

  calcRegister_t regStats = graphVariable;
  if(regStats != INVALID_VARIABLE) {
    fnStore(regStats);                  //place X register into x

    #if defined(PC_BUILD) //PC_BUILD
      printf("Graph variable x=%u: ",graphVariable);
      printRegisterToConsole(graphVariable, " = ","\n");
    #endif //PC_BUILD

    fnEqCalc(0);

    #if defined(PC_BUILD) //PC_BUILD
      printf("Graph variable y ");
      printRegisterToConsole(REGISTER_X, " = ","\n");
    #endif //PC_BUILD

    #if defined(PC_BUILD)
      if(lastErrorCode != 0) {
        #if defined(VERBOSE_SOLVER00)
        printf("ERROR CODE in execute_rpn_function/fnEqCalc: %u\n",lastErrorCode);
        #endif // VERBOSE_SOLVER00
        lastErrorCode = 0;
      }
    #endif // PC_BUILD
    fnRCL(regStats);
    #if defined(VERBOSE_SOLVER0) && defined(PC_BUILD)
      printRegisterToConsole(REGISTER_X,">>> Calc x=","");
      printRegisterToConsole(REGISTER_Y," y=","");
    #endif // VERBOSE_SOLVER0 && PC_BUILD
  }
  else {
    #if defined(PC_BUILD)
      #if defined(VERBOSE_SOLVER00)
      printf("ERROR in execute_rpn_function; invalid variable: %u\n",lastErrorCode);
      #endif // VERBOSE_SOLVER00
      lastErrorCode = 0;
    #endif
  }
}

#if !defined(SAVE_SPACE_DM42_13GRF)
  static bool_t regIsLowerThanTol(calcRegister_t REG, calcRegister_t TOL) {
    return (    (real34IsZero(REGISTER_REAL34_DATA(REG)) && (getRegisterDataType(REG) == dtComplex34 ? real34IsZero(REGISTER_IMAG34_DATA(REG)) : 1 ))
             || (    (real34CompareAbsLessThan(REGISTER_REAL34_DATA(REG), REGISTER_REAL34_DATA(TOL)))
                  && (getRegisterDataType(REG) == dtComplex34 ? real34CompareAbsLessThan(REGISTER_IMAG34_DATA(REG), REGISTER_REAL34_DATA(TOL)) : 1)
                )
           );
  }


#define ADD_RAN true
static void divFunction(bool_t addRandom, calcRegister_t TOL) {
  if(  (real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && (getRegisterDataType(REGISTER_Y) == dtComplex34 ? real34IsZero(REGISTER_IMAG34_DATA(REGISTER_Y)) : 1 ) )
     || real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))
     || (getRegisterDataType(REGISTER_Y) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y)) : 0 ) ) {
    fnDrop(0);
    fnDrop(0);
    convertDoubleToReal34RegisterPush(0.0, REGISTER_X);
    return;
  }
  if(real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || (getRegisterDataType(REGISTER_X) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X)) : 0 ) ) {
    fnDrop(0);
    fnDrop(0);
    convertDoubleToReal34RegisterPush(0.0, REGISTER_X);
    return;
  }
  if(!addRandom && (real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) && (getRegisterDataType(REGISTER_X) == dtComplex34 ? real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X)) : 1 ) )) {
    fnDrop(0);
    fnDrop(0);
    convertDoubleToReal34RegisterPush(1e30, REGISTER_X);
    return;
  }
  if(addRandom && regIsLowerThanTol(REGISTER_X, TOL)) {
      #if defined(PC_BUILD)
      printf(">>> ADD random number to denominator to prevent infinite result\n");
      #endif // PC_BUILD
    convertDoubleToReal34RegisterPush(1e-6, REGISTER_X);
    runFunction(ITM_ADD);
    runFunction(ITM_RAN);
    runFunction(ITM_ADD);
  }
  runFunction(ITM_DIV);
}
#endif // !SAVE_SPACE_DM42_13GRF


int16_t osc = 0;
uint8_t DXR = 0, DYR = 0, DXI = 0, DYI = 0;


  void check_osc(uint8_t ii){
    switch(ii & 0b00111111) {
      case 0b001111:
      case 0b011110:
      case 0b111100:
      case 0b010101:
      case 0b101010:
      case 0b011011:
      case 0b110110:
      case 0b101101:
        osc++;
        break;
      default: ;
    }
    switch(ii) {
      case 0b01001001:
      case 0b10010010:
      case 0b00100100:
        osc++;
        break;
      default: ;
    }
  }

//###################################################################################
//PLOTTER
  int32_t drawMxN(void){
    uint16_t rows = 0;
    if(plotStatMx[0]!='D') {
      return 0;
    }
    calcRegister_t regStats = findNamedVariable(plotStatMx);
    if(regStats == INVALID_VARIABLE) {
      return 0;
    }
    if(isStatsMatrix(&rows,plotStatMx)) {
      real34Matrix_t stats;
      linkToRealMatrixRegister(regStats, &stats);
      return stats.header.matrixRows;
    }
    else {
      return 0;
    }
  }
#endif // !TESTSUITE_BUILD


  void fnClDrawMx(void) {
    PLOT_ZOOM = 0;
    calcRegister_t regStats = findNamedVariable("DrwMX");
    if(regStats != INVALID_VARIABLE) {
      fnDeleteVariable(regStats);
    };
  }


#if !defined(TESTSUITE_BUILD)
  static void AddtoDrawMx() {
    real_t x, y;
    uint16_t rows = 0, cols;
    calcRegister_t regStats = regStatsXY;
    if(!isStatsMatrixN(&rows,regStats)) {
      regStats = allocateNamedMatrix(plotStatMx, 1, 2);
      regStatsXY = regStats;
      real34Matrix_t stats;
      linkToRealMatrixRegister(regStats, &stats);
      realMatrixInit(&stats,1,2);
    }
    else {
      if(appendRowAtMatrixRegister(regStats)) {
      }
      else {
        regStats = INVALID_VARIABLE;
      }
    }
    if(regStats != INVALID_VARIABLE) {

    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), &x);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_Y), &y);

      real34Matrix_t stats;
      linkToRealMatrixRegister(regStats, &stats);
      rows = stats.header.matrixRows;
      cols = stats.header.matrixColumns;
      realToReal34(&x, &stats.matrixElements[(rows-1) * cols    ]);
      realToReal34(&y, &stats.matrixElements[(rows-1) * cols + 1]);
    }
    else {
      displayCalcErrorMessage(ERROR_NOT_ENOUGH_MEMORY_FOR_NEW_MATRIX, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
      #if(EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "additional matrix line not added; rows = %i",rows);
        moreInfoOnError("In function AddtoDrawMx:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }
#endif // !TESTSUITE_BUILD



void graph_eqn(uint16_t mode) {
  #if !defined(TESTSUITE_BUILD)
    regStatsXY = findNamedVariable(plotStatMx);
    double x;
    double x01 = x_min;
    double y01 = 0;
    double y02 = 0;
    double dy;
    double dx0 = (x_max-x_min)/SCREEN_WIDTH_GRAPH*10;
    double dx = dx0;
    double grad2 = 1;
    double grad1 = 1;
    double grad0 = 1;
    int16_t count = 0;
    int16_t ss0 = 0;
    int16_t ss1 = 0;
    int16_t ss2 = 0;
    #define SS1 1.8 //1.4  grad2/grad2 threshold for 50% dx
    #define SS2 2.4 //2    grad2/grad2 threshold for jumping back
    #define FINE 9   // (was 20) number of steps to jump
    #define JMP 0.8  // Jump back: 9 x 0.2 (was 20 x 0.1) : -0.8 -0.6 -0.4 -0.2 0 0.2 0.4 0.6 0.8, i.e. 9 steps back for discontinuity (0.9)
    #define dJMP 0.2 // Fine movement in p.u. ddx
    uint8_t discontinuityDetected = 0;
    bool_t  grad2IncreaseDetected = false;
    double yAvg = 0.1;
    int loop = 0;


    if(graphVariable <= 0 || graphVariable > 65535) {
      regStatsXY = INVALID_VARIABLE;
      return;
    }

    #if defined (LOW_GRAPH_ACC)
      //Change to SDIGS digit operation for graphs;
      if(significantDigitsForEqnGraphs <= 6) {
        ctxtReal34.digits = significantDigitsForEqnGraphs;
        ctxtReal39.digits = significantDigitsForEqnGraphs+3;
        ctxtReal51.digits = significantDigitsForEqnGraphs+6;
        ctxtReal75.digits = significantDigitsForEqnGraphs+9;
      }
    #endif

    if(!GRAPHMODE) { //Change over hourglass to the left side
      clearScreenOld(clrStatusBar, !clrRegisterLines, !clrSoftkeys);
    }
    calcMode = CM_GRAPH;
    hourGlassIconEnabled = true;       //clear the current portion of statusbar
    showHideHourGlass();
    refreshStatusBar();


    fnClearStack(0);

    convertDoubleToReal34RegisterPush(x_max, REGISTER_X);
    execute_rpn_function();
    yAvg += 2 * fabs(convertRegisterToDouble(REGISTER_Y));

    if(mode == initDrwMx) {
      fnClDrawMx();
      strcpy(plotStatMx,"DrwMX");
    }
    #if defined(DEBUG_GR)
      printf("dx0=%f discontinuityDetected:%u grad2IncreaseDetected:%u\n",dx0, discontinuityDetected, grad2IncreaseDetected);
    #endif // DEBUG_GR

    //Main loop, default is 40 x 6 point gaps, across the 240 wide screen
    //  If the grad2 is increasing, then the dx is reduced.
    //  That helps going forward, but not looking a the last sample which already jumped too far, so the next part steps back.
    //  The 0.99999*dx increment is to purposely stay off integer fractions, which is then less likely to land on easy roots
    for(x=x_min; x<=x_max; x+=0.99999*dx) {
      x = fmax(x_min,x);
      x = fmin(x_max,x);
      convertDoubleToReal34RegisterPush(x, REGISTER_X);
      //leaving y in Y and x in X
      execute_rpn_function();

      y02 = convertRegisterToDouble(REGISTER_Y);
      dy = y02 - y01;
      grad2  = dy / (x - x01);
      ss0 = ss1;
      ss1 = ss2;
      ss2 = grad2 == 0 ? 0 : grad2 > 0 ? 1 : -1;
      grad0 = grad1;
      grad1 = grad2;


      #if defined(DEBUG_GR)
        printRegisterToConsole(REGISTER_X,"X:","");
        printRegisterToConsole(REGISTER_Y," Y:","");
        printf("%f %f grad2/grad1=%f grad1/grad2=%f \n",grad2, grad1, grad2/grad1, grad1/grad2);
        printf("ss1 %i ss2 %i y01 %6f y02 %6f\n",ss1,ss2,y01,y02);
      #endif // DEBUG_GR


      if(grad1 != 0 && grad2 != 0) {
        grad2IncreaseDetected = (
          (fabs(y02/y01) > 1.01 && fabs(grad2/grad1) > SS2) ||                //increase in grad2
          (fabs(y01/y02) > 1.01 && fabs(grad1/grad2) > SS2) ||                //increase in grad2
          (ss0 == 1  && ss1 == -1 && ss2 == 1) ||                              //grad2 reversal checking
          (ss0 == -1 && ss1 == 1  && ss2 == -1) ||                             //grad2 reversal checking
          (             ss1 == 1  && ss2 == -1  && y01 > 0 && y02 < 0) ||      //grad2 reversal checking
          (             ss1 == -1 && ss2 == 1   && y01 < 0 && y02 > 0)     );  //grad2 reversal checking
      } else grad2IncreaseDetected = false;


      if(count == 0) {                        //accumulate an average value starting with inflated 2x start value;
        yAvg += 2 * fabs(y02);
      }
      else if(fabs(y02) > yAvg){
          yAvg += fabs(y02) / count;
      }

      if(discontinuityDetected == 0) {
        if((real34IsInfinite(REGISTER_REAL34_DATA(REGISTER_X)) || ((getRegisterDataType(REGISTER_X) == dtComplex34) && (real34IsInfinite(REGISTER_IMAG34_DATA(REGISTER_X)))))
        || (real34IsNaN     (REGISTER_REAL34_DATA(REGISTER_X)) || ((getRegisterDataType(REGISTER_X) == dtComplex34) && (real34IsNaN     (REGISTER_IMAG34_DATA(REGISTER_X)))))
        ||((fabs(grad2)/6 > fabs(0.8*grad0 + 0.2*grad1) ) && count > 3)
        ||((fabs(y02) > 10 * yAvg) && count > 3)
        ) {
          discontinuityDetected = FINE;
        }
      }

      if((discontinuityDetected == FINE) || (discontinuityDetected == 0 && grad2IncreaseDetected)) {
        #if defined(DEBUG_GR)
          printf("jumping %f %f grad2/grad1=%f  grad1/grad2=%f\n",grad2, grad1, grad2/grad1, grad1/grad2 );
        #endif // DEBUG_GR
        x -= dx * (JMP);
        convertDoubleToReal34RegisterPush(x, REGISTER_X);
        execute_rpn_function();
        y02 = convertRegisterToDouble(REGISTER_Y);
        grad2  = (y02 - y01) / (x - x01);
        ss0 = ss1;
        ss1 = ss2;
        ss2 = grad2 == 0 ? 0 : grad2 > 0 ? 1 : -1;
        #if defined(DEBUG_GR)
          printf("Jump back\n");
          printRegisterToConsole(REGISTER_X,"X:","");
          printRegisterToConsole(REGISTER_Y," Y:","");
          printf("%f %f grad2/grad1=%f grad1/grad2=%f \n",grad2, grad1, grad2/grad1, grad1/grad2);
        #endif // DEBUG_GR
      }

      if(discontinuityDetected > 0 && discontinuityDetected <= FINE) {
        dx = dJMP * dx0;
      } else if(grad2 == 0 || grad1 == 0) {
        dx = dx0;
      } else if(grad2IncreaseDetected){
        dx = dx0 * ( (grad2/grad1 > SS1 || grad1/grad2 > SS1)  ? 0.5 : 1.0);     //50% dx0 if increased grad2 detected
      } else dx = dx0;

      #if defined(DEBUG_GR)
        printf("Graph: dx=%f drawMxN=%i\n",dx,drawMxN());
      #endif // DEBUG_GR

      AddtoDrawMx();
      grad1 = grad2;
      y01 = y02;
      x01 = x;

      #if defined(VERBOSE_SOLVER0) && defined(PC_BUILD)
        printf(">>> Into DrwMX:%i points ",drawMxN());
        printRegisterToConsole(REGISTER_X,"X:","");
        printRegisterToConsole(REGISTER_Y," Y:","\n");
      #endif // VERBOSE_SOLVER0 && PC_BUILD
      #if defined(PC_BUILD)
        if(lastErrorCode == 24) {
          printf("ERROR CODE CANNOT STAT COMPLEX RESULT, ignored\n"); lastErrorCode = 0;
        }
      #endif //PC_BUILD

      if(discontinuityDetected != 0) discontinuityDetected --;
      count++;
      if(count > 60) break;

      #if defined(DEBUG_GR)
        printf("dx0=%f dx=%f yAvg=%f count=%i discontinuityDetected:%u grad2IncreaseDetected:%u\n",dx0, dx, yAvg, count, discontinuityDetected, grad2IncreaseDetected);
      #endif // DEBUG_GR

      printHalfSecUpdate_Integer(timed, "Iter: ",loop++); //timed
      #if defined(DMCP_BUILD)
        if(keyWaiting()) {
          printHalfSecUpdate_Integer(force+1, "Interrupted Iter:",loop);
          fnClearStack(0);
          calcMode = CM_NORMAL;
          screenUpdatingMode = SCRUPD_AUTO;
          break;
        }
      #endif //DMCP_BUILD

    }

    #if defined (LOW_GRAPH_ACC)
      //Change to SDIGS digit operation for fresh stack;
      ctxtReal34.digits = 34; //Change back to normal operation for stack;
      ctxtReal39.digits = 39; //Change back to 39 digit operation for stack;
    #endif //LOW_GRAPH_ACC

    fnClearStack(0);

    #if defined (LOW_GRAPH_ACC)
      //Change to SDIGS digit operation for screen graphs;
      ctxtReal34.digits = significantDigitsForScreen;
      ctxtReal39.digits = significantDigitsForScreen+3;
    #endif //LOW_GRAPH_ACC

    fnPlot(0);

    #if defined (LOW_GRAPH_ACC)
      //Change to normal operation for graphs;
      ctxtReal34.digits = 34;
      ctxtReal39.digits = 39;
      ctxtReal51.digits = 51;
      ctxtReal75.digits = 75;
    #endif //LOW_GRAPH_ACC    

  #endif // !TESTSUITE_BUILD
}


void graph_stat(uint16_t unusedButMandatoryParameter) {
  #if !defined(TESTSUITE_BUILD)
    calcMode = CM_GRAPH;
    reDraw = true;

    lastPlotMode = PLOT_NOTHING;
    strcpy(plotStatMx,"STATS");

    PLOT_LINE = true;
    PLOT_SHADE = true;

    fnClearStack(0);
    fnPlotSQ(0);
  #endif // !TESTSUITE_BUILD
}


//###################################################################################
//SOLVER

#define __STARTX0 81
#define __STARTX1 82
#define __TICKS  83
#define __TMP    84
#define __Xold   85
#define __Yold   86
#define __Y0     87
#define __X0     88
#define __X1     89
#define __X2     90
#define __Y1     91
#define __Y2     92
#define __X2N    93
#define __DX     94
#define __DY     95
#define __F      96
#define __TOL    97
#define __L1     98


#if !defined(SAVE_SPACE_DM42_13GRF)
#if !defined(TESTSUITE_BUILD)
  static void complexSolver() {         //Input parameters in registers SREG_STARTX0, SREG_STARTX1
    if(graphVariable <= 0 || graphVariable > 65535) {
      return;
    }

    calcMode = CM_NO_UNDO;
    calcRegister_t SREG_TMP  = __TMP ;
    calcRegister_t SREG_Xold = __Xold; //: x old difference
    calcRegister_t SREG_Yold = __Yold; //: y old difference
    calcRegister_t SREG_Y0   = __Y0  ; //: y0
    calcRegister_t SREG_X0   = __X0  ; //: x0    //x0 stored but noever recalled
    calcRegister_t SREG_X1   = __X1  ; //: x1
    calcRegister_t SREG_X2   = __X2  ; //: x2
    calcRegister_t SREG_Y1   = __Y1  ; //: y1
    calcRegister_t SREG_Y2   = __Y2  ; //: y2
    calcRegister_t SREG_X2N  = __X2N ; //: temporary new x2
    calcRegister_t SREG_DX   = __DX  ; //: x difference
    calcRegister_t SREG_DY   = __DY  ; //: y diffe rence
    calcRegister_t SREG_F    = __F   ; //: faxctor
    calcRegister_t SREG_TOL  = __TOL ; //: tolerance
    calcRegister_t SREG_L1   = __L1  ; //: temporary stoar
    calcRegister_t SREG_STARTX0 = __STARTX0;
    calcRegister_t SREG_STARTX1 = __STARTX1;
    calcRegister_t SREG_TICKS = __TICKS;

    runFunction(ITM_CLSTK);
    runFunction(ITM_RAD);
    clearSystemFlag(FLAG_SSIZE8);
    setSystemFlag(FLAG_CPXRES);
    int16_t ix,ixd;
    int16_t oscillations = 0;
    int16_t convergent = 0;
    bool_t checkNaN = false;
    bool_t checkzero = false;
    osc = 0;
    DXR = 0, DYR = 0, DXI = 0, DYI = 0;
    ix = 0; ixd = 0;
    int16_t kicker = 0;
    int loop = 0;


    // Initialize all temporary registers
    // Registers are being used in the DEMO data programs

    convertDoubleToReal34RegisterPush(1.0, REGISTER_X);
    fnRCL(SREG_STARTX1);
    runFunction(ITM_MULT);     //Convert input to REAL
    fnStore(SREG_X1);
    convertDoubleToReal34RegisterPush(1.0, REGISTER_X);
    fnRCL(SREG_STARTX0);
    runFunction(ITM_MULT);     //Convert input to REAL

    //if input parameters X0 and X1 are the same, add a random number to X0
    if(real34CompareEqual(REGISTER_REAL34_DATA(SREG_X1), REGISTER_REAL34_DATA(REGISTER_X))) {
      convertDoubleToReal34RegisterPush(1e-3, REGISTER_X);
      #if defined(PC_BUILD)
        printf(">>> ADD random number to second input parameter to prevent infinite result\n");
      #endif
      runFunction(ITM_ADD);
      runFunction(ITM_RAN);
      runFunction(ITM_ADD);
      fnStore(SREG_X0);
    }
    else {
      fnStore(SREG_X0);
    }

    runFunction(ITM_TICKS);
    fnStore(SREG_TICKS);
    convertDoubleToReal34RegisterPush(0.0, REGISTER_X);
    fnStore(SREG_TMP);
    fnStore(SREG_Xold);
    fnStore(SREG_Yold);
    fnStore(SREG_X2N);
    convertDoubleToReal34RegisterPush(CONVERGE_FACTOR, REGISTER_X);
    fnStore(SREG_F);                                     // factor
    convertDoubleToReal34RegisterPush(1E-1, REGISTER_X);
    fnStore(SREG_DX);                                    // initial value for difference comparison must be larger than tolerance
    fnStore(SREG_DY);                                    // initial value for difference comparison must be larger than tolerance

    real_t tol;
    convergenceTolerence(&tol);
    realToReal34(&tol,REGISTER_REAL34_DATA(SREG_TOL));   // tolerance

    fnRCL(SREG_X0);          //determined third starting point using the slope or secant
    execute_rpn_function();
    copySourceRegisterToDestRegister(REGISTER_Y,SREG_Y0);
    fnRCL(SREG_X1);
    execute_rpn_function();
    copySourceRegisterToDestRegister(REGISTER_Y,SREG_Y1);


    checkzero = checkzero ||   regIsLowerThanTol(SREG_Y0,SREG_TOL);
    if(checkzero) {
      copySourceRegisterToDestRegister(SREG_Y0,SREG_Y2);
      copySourceRegisterToDestRegister(SREG_X0,SREG_X2);
    }
    else {
      checkzero = checkzero ||   regIsLowerThanTol(SREG_Y1,SREG_TOL);
      if(checkzero) {
        copySourceRegisterToDestRegister(SREG_Y1,SREG_Y2);
        copySourceRegisterToDestRegister(SREG_X1,SREG_X2);
      }
    }

    if(!checkzero) {
      fnRCL(SREG_X1);
      fnRCL(SREG_X0);
      runFunction(ITM_SUB);                         //dx=x1-x0
      fnRCL(SREG_Y1);
      fnRCL(SREG_Y0);
      runFunction(ITM_SUB);                         //dy=y1-y0
      divFunction(ADD_RAN, SREG_TOL);               //dx/dy
      fnRCL(SREG_Y1);
      runFunction(ITM_MULT);                        //deltaX = x1 - x2 = Y1 / (dy/dx) = Y1 x 1/(dy/dx) = Y1 x dx/dy

      fnRCL(SREG_X1);
      runFunction(ITM_XexY);
      runFunction(ITM_SUB);
      fnStore(SREG_X2);
      execute_rpn_function();
      copySourceRegisterToDestRegister(REGISTER_Y,SREG_Y2);
    }


    #if(defined(VERBOSE_SOLVER00) || defined(VERBOSE_SOLVER0) || defined(VERBOSE_SOLVER1) || defined(VERBOSE_SOLVER2)) && defined(PC_BUILD)
      printf("INIT:   ix=%d \n",ix);
      printRegisterToConsole(SREG_X0,"Init X0= ","\n");
      printRegisterToConsole(SREG_X1,"Init X1= ","\n");
      printRegisterToConsole(SREG_X2,"Init X2= ","\n");
      printRegisterToConsole(SREG_Y0,"Init Y0= ","\n");
      printRegisterToConsole(SREG_Y1,"Init Y1= ","\n");
      printRegisterToConsole(SREG_Y2,"Init Y2= ","\n");
    #endif // (VERBOSE_SOLVER00 || VERBOSE_SOLVER0 || VERBOSE_SOLVER1 || VERBOSE_SOLVER2) && PC_BUILD


    //###############################################################################################################
    //#################################################### Iteration start ##########################################
    while(ix<NUMBERITERATIONS && !checkNaN && !checkzero) {
      #if defined(PC_BUILD)
        if(lastErrorCode != 0) {
          printf(">>> ERROR CODE INITIALLY NON-ZERO = %d <<<<<\n",lastErrorCode);
          goto to_return;
        }
      #endif //PC_BUILD

      //assumes X2 is in R91
      //Identify oscillations in real or imag: increment osc flag
      //osc = 0;

      if( (real34IsNegative(REGISTER_REAL34_DATA(SREG_DX)) && real34IsPositive(REGISTER_REAL34_DATA(SREG_Xold))) ||
          (real34IsPositive(REGISTER_REAL34_DATA(SREG_DX)) && real34IsNegative(REGISTER_REAL34_DATA(SREG_Xold))) ) {
        DXR = (DXR << 1) + 1;
      }
      else {
        DXR = DXR << 1;
      }

      if( (real34IsNegative(REGISTER_REAL34_DATA(SREG_DY)) && real34IsPositive(REGISTER_REAL34_DATA(SREG_Yold))) ||
          (real34IsPositive(REGISTER_REAL34_DATA(SREG_DY)) && real34IsNegative(REGISTER_REAL34_DATA(SREG_Yold))) ) {
        DYR = (DYR << 1) + 1;
      }
      else {
        DYR = DYR << 1;
      }

      if((getRegisterDataType(SREG_DX) == dtComplex34 && getRegisterDataType(SREG_Xold) == dtComplex34) &&
           ((real34IsNegative(REGISTER_IMAG34_DATA(SREG_DX)) && real34IsPositive(REGISTER_IMAG34_DATA(SREG_Xold))) ||
            (real34IsPositive(REGISTER_IMAG34_DATA(SREG_DX)) && real34IsNegative(REGISTER_IMAG34_DATA(SREG_Xold))) )) {
        DXI = (DXI << 1) + 1;
      }
      else {
        DXI = DXI << 1;
      }

      if((getRegisterDataType(SREG_DY) == dtComplex34 && getRegisterDataType(SREG_Yold) == dtComplex34) &&
           ((real34IsNegative(REGISTER_IMAG34_DATA(SREG_DY)) && real34IsPositive(REGISTER_IMAG34_DATA(SREG_Yold))) ||
            (real34IsPositive(REGISTER_IMAG34_DATA(SREG_DY)) && real34IsNegative(REGISTER_IMAG34_DATA(SREG_Yold))) )) {
        DYI = (DYI << 1) + 1;
      }
      else {
        DYI = DYI << 1;
      }

     check_osc(DXR);
     check_osc(DYR);
     check_osc(DXI);
     check_osc(DYI);

      //If osc flag is active, that is any delta polarity change, then increment oscillation count
      if(osc > 0) {
        oscillations++;
      }
      else {
        oscillations = max(0,oscillations-1);
      }

      //If converging, increment convergence counter
      if((!real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(SREG_DX), REGISTER_REAL34_DATA(SREG_Xold)) &&
         (getRegisterDataType(SREG_DX) == dtComplex34 && getRegisterDataType(SREG_Xold) == dtComplex34 ?
           !real34CompareAbsGreaterThan(REGISTER_IMAG34_DATA(SREG_DX), REGISTER_IMAG34_DATA(SREG_Xold)) : true))
         &&
         (!real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(SREG_DY), REGISTER_REAL34_DATA(SREG_Yold)) &&
         (getRegisterDataType(SREG_DY) == dtComplex34 && getRegisterDataType(SREG_Yold) == dtComplex34 ?
           !real34CompareAbsGreaterThan(REGISTER_IMAG34_DATA(SREG_DY), REGISTER_IMAG34_DATA(SREG_Yold)) : true))
        ) {
          convergent++;
      }
      else {
        convergent = max(0,convergent-1);
      }
      #if defined(VERBOSE_SOLVER0)
                        printf("##### ix= %d osc= %d  conv= %d n",ix, oscillations, convergent);
      #endif // VERBOSE_SOLVER0
      #if defined(VERBOSE_SOLVER1)
                        printf("################################### ix= %d osc= %d  conv= %d ###########################################\n",ix, oscillations, convergent);
                      #endif //VERBOSE_SOLVER1

      if(convergent > 6 && oscillations > 3) {

        #if defined(PC_BUILD)
                        printf("    --   reset detection from =convergent%i and oscillations=%i to ", convergent, oscillations);
        #endif // PC_BUILD
        convergent = 2;
        oscillations = 2;
        #if defined(PC_BUILD)
                        printf("%i and %i\n", convergent, oscillations);
        #endif // PC_BUILD
      }

      // If increment is oscillating it is assumed that it is unstable and needs to have a complex starting value
      //assumes X2 is in R91
      if((((oscillations >= 3) && (ixd > 9) && (convergent <= 2))
         //|| (oscillations == 0 && convergent > 6 && real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DX), const34_1e_4) && (getRegisterDataType(SREG_DX) == dtComplex34 ? real34CompareAbsLessThan(REGISTER_IMAG34_DATA(SREG_DX), const34_1e_4) : 1 )  )
        )) {
        if(COMPLEXKICKER && (kicker ==0) && (convergent <= 1)) {
          kicker = kicker +2;
        }
        ixd = 0;
        oscillations = 0;
        convergent = 0;
        #if defined(VERBOSE_SOLVER2) && defined(PC_BUILD)
          printRegisterToConsole(SREG_X2,"\n>>>>>>>>>> from ","");
        #endif // VERBOSE_SOLVER2 && PC_BUILD
        fnRCL(SREG_X2);

        //when kicker = 0, then factor is small negative real
        //after that, it is complex, in the first quardrant, multplied by a alrger number every time
        convertDoubleToReal34RegisterPush(  - (kicker +0.001) / 100.0, REGISTER_X);
        if(kicker > 0) {
          runFunction(ITM_SQUAREROOTX);
          convertDoubleToReal34RegisterPush(    (kicker+0.001) / 100.0, REGISTER_X);
          runFunction(ITM_SQUAREROOTX);
          runFunction(ITM_ADD);
          convertDoubleToReal34RegisterPush(pow(-2.0,kicker), REGISTER_X);
          runFunction(ITM_MULT);
        }

        #if(defined(VERBOSE_SOLVER00) || defined(VERBOSE_SOLVER0)) && defined(PC_BUILD)
          printf("------- Kicked oscillation, #%d, ", kicker);
          printRegisterToConsole(REGISTER_X," multiplied: ","\n");
        #endif  // (VERBOSE_SOLVER00 || VERBOSE_SOLVER0) && PC_BUILD

        kicker++;

        runFunction(ITM_MULT);              //add just to force it complex  //
        fnStore(SREG_X2); //replace X2 value                                //

        #if defined(VERBOSE_SOLVER2) && defined(PC_BUILD)
          printRegisterToConsole(SREG_X2," to ","\n");
        #endif // VERBOSE_SOLVER2 && PC_BUILD
      }

      //@@@@@@@@@@@@@@@@@ CALCULATE NEW Y2, AND PLAUSIBILITY @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
      fnRCL(SREG_X2);                                       // get (X2,Y2)
      execute_rpn_function();                               // leaving y2 in Y and x2 in X
      copySourceRegisterToDestRegister(REGISTER_Y,SREG_Y2); // y2

      #if defined(VERBOSE_SOLVER1) && defined(PC_BUILD)
        printf("    :   ix=%d",ix);
        printRegisterToConsole(SREG_X2," X2="," ");
        printRegisterToConsole(SREG_Y2," Y2=","\n");
      #endif // VERBOSE_SOLVER1 && PC_BUILD

      // y2 in Y and x2 in X
      checkzero = checkzero ||   regIsLowerThanTol(SREG_Y2,SREG_TOL);
      checkNaN  = checkNaN  ||   real34IsNaN(REGISTER_REAL34_DATA(SREG_X2)) || (getRegisterDataType(SREG_X2) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(SREG_X2)) : 0 ) ||
                                 real34IsNaN(REGISTER_REAL34_DATA(SREG_Y2)) || (getRegisterDataType(SREG_Y2) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(SREG_Y2)) : 0 ) ;

      #if(defined(VERBOSE_SOLVER00) || defined(VERBOSE_SOLVER0)) && defined(PC_BUILD)
        if(checkNaN || ix==NUMBERITERATIONS-1 || checkzero) {
          printf("-->A Endflags zero: Y2r=0:%u Y2i=0:%u X2r=NaN:%u X2i=NaN:%u Y2r=NaN:%u Y2i=NaN%u \n",
                  (uint16_t)real34IsZero(REGISTER_REAL34_DATA(SREG_Y2)),(uint16_t)real34IsZero(REGISTER_IMAG34_DATA(SREG_Y2)),
                  (uint16_t)real34IsNaN (REGISTER_REAL34_DATA(SREG_X2)),(uint16_t)real34IsNaN (REGISTER_IMAG34_DATA(SREG_X2)),
                  (uint16_t)real34IsNaN (REGISTER_REAL34_DATA(SREG_Y2)),(uint16_t)real34IsNaN (REGISTER_IMAG34_DATA(SREG_Y2))
                );
        }
      #endif // (VERBOSE_SOLVER00 || VERBOSE_SOLVER0) && PC_BUILD

      #if defined(VERBOSE_SOLVER2) && defined(PC_BUILD)
        printf("   ix=%d checkend=%d X2=",ix, checkNaN || ix==NUMBERITERATIONS-1 || checkzero);
        printRegisterToConsole(SREG_X2,"","");
        printRegisterToConsole(SREG_Y2,"Y2=","\n");
      #endif // VERBOSE_SOLVER2 && PC_BUILD

      //*************** DETERMINE DX and DY, to calculate the slope (or the inverse of the slope in this case) *******************
      copySourceRegisterToDestRegister(SREG_DX,SREG_Xold);  // store old DELTA values, for oscillation check
      copySourceRegisterToDestRegister(SREG_DY,SREG_Yold);  // store old DELTA values, for oscillation check

      if(ix < CHANGE_TO_MOD_SECANT) {              //Secant and Newton approximation methods
        if(ix < 3)  {
          //###########################
          //  normal Secant, 2-sample slope
          //  DX = X2 - X1 in YREGISTER
          //  DY = Y2 - Y1 in XREGISTER
          //###########################
          #if defined(VERBOSE_SOLVER00) || defined(VERBOSE_SOLVER0) || defined(VERBOSE_SOLVER1) || defined(VERBOSE_SOLVER2)
            printf("%3i ---------- Using normal Secant dydx 2-samples - osc=%d conv=%d",ix, oscillations, convergent);
          #endif // VERBOSE_SOLVER00 || VERBOSE_SOLVER0 || VERBOSE_SOLVER1 || VERBOSE_SOLVER2
          fnRCL  (SREG_X2); fnRCL(SREG_X1); runFunction(ITM_SUB);      // dx
          fnStore(SREG_DX);                                     // store difference for later
          fnRCL  (SREG_Y2); fnRCL(SREG_Y1); runFunction(ITM_SUB);      // dy
          fnStore(SREG_DY);                                     // store difference for later
          //Leave DX in YREG, and DY in XREG, so DX/DY can be computed
        }

        else {
          //###########################
          // normal secant with 3 sample slope
          //  DX = X2 - X1 in YREGISTER
          //  DY = Y2 - Y1 in XREGISTER
          //###########################
          //  The second order accurate one-sided finite difference formula for the first derivative, formule 32, of
          //  ChE 205 — Formulas for Numerical Differentiation
          //  Handout 5 05/08/02: from Pauli
          #if defined(VERBOSE_SOLVER00) || defined(VERBOSE_SOLVER0) || defined(VERBOSE_SOLVER1) || defined(VERBOSE_SOLVER2)
            printf("%3i ---------- Using Secant with 3 samples dy/dx -- osc=%d conv=%d",ix, oscillations, convergent);
          #endif // VERBOSE_SOLVER00 || VERBOSE_SOLVER0 || VERBOSE_SOLVER1 || VERBOSE_SOLVER2
          fnRCL      (SREG_X2); fnRCL(SREG_X1); runFunction(ITM_SUB); //Determine x2-x1
          fnStore    (SREG_DX);  //store difference DX for later
          fnRCL      (SREG_X1);
          runFunction(ITM_XexY);
          runFunction(ITM_SUB);
          fnStore    (SREG_X0);          //determine the new x0 by subtracting DX
          execute_rpn_function(); //determine the new f(x0)
          copySourceRegisterToDestRegister(REGISTER_Y,SREG_Y0); //set y0 to the result f(x0)
          //do DX = 2 (x2-x1)
          fnRCL      (SREG_DX);
          convertDoubleToReal34RegisterPush(2.0, REGISTER_X);//calculate 2(x2-x1)
          runFunction(ITM_MULT);             // DX = 2 delta x
          //do DY = (fi−2 − 4fi−1 + 3fi)
          fnRCL      (SREG_Y0);              //y0
          fnRCL      (SREG_Y1);
          convertDoubleToReal34RegisterPush(4.0, REGISTER_X);
          runFunction(ITM_MULT);
          runFunction(ITM_SUB);               //-4.y1
          fnRCL      (SREG_Y2);
          convertDoubleToReal34RegisterPush(3.0, REGISTER_X);
          runFunction(ITM_MULT);
          runFunction(ITM_ADD);                    //+3.y2
          fnStore    (SREG_DY);
          //-3-sample slope-  //Leave DX in YREG, and DY in XREG, so DX/DY can be computed
        }
        //###########################
        //  Start with DX and DY FROM EITHER 2- or 3- SAMPLE SECANT
        //
        //
        //###########################
        divFunction(!ADD_RAN, SREG_TOL);
        #if defined(VERBOSE_SOLVER2) && defined(PC_BUILD)
          fnInvert(0);
          printRegisterToConsole(REGISTER_X," SLOPE=","\n");
          fnInvert(0);
        #endif // VERBOSE_SOLVER2 && PC_BUILD

        fnRCL(SREG_Y2);      // determine increment in x
        runFunction(ITM_MULT);       // increment to x is: y1 . DX/DY
        fnRCL(SREG_F);       // factor to stabilize Newton method. factor=1 is straight. factor=0.1 converges 10x slower.
        runFunction(ITM_MULT);       // increment to x

        #if defined(VERBOSE_SOLVER1) && defined(PC_BUILD)
          printRegisterToConsole(SREG_F,"    Factor=        ","\n");
          printRegisterToConsole(SREG_X1,"    New X =        "," - ");
          printRegisterToConsole(REGISTER_X," - (",")\n");
        #endif // VERBOSE_SOLVER1 && PC_BUILD

        fnRCL(SREG_X2);
        runFunction(ITM_XexY);
        runFunction(ITM_SUB);       // subtract as per Newton, x1 - f/f'
        fnStore(SREG_X2N);          // store temporarily to new x2n
        #if defined(VERBOSE_SOLVER00) || defined(VERBOSE_SOLVER0) || defined(VERBOSE_SOLVER1) || defined(VERBOSE_SOLVER2)
          printf("  ");
          printRegisterToConsole(SREG_X2N,"New X=","\n");
          //printRegisterToConsole(REGISTER_Y,"Secant DeltaX=","\n");
        #endif // VERBOSE_SOLVER00 || VERBOSE_SOLVER0 || VERBOSE_SOLVER1 || VERBOSE_SOLVER2
      }
      else {
        // ---------- Modified 3 point Secant ------------
        if((ix == 0) || (!checkzero && !checkNaN)) {
          #if defined(VERBOSE_SOLVER00) || defined(VERBOSE_SOLVER0) || defined(VERBOSE_SOLVER1) || defined(VERBOSE_SOLVER2)
            printf("%3i ---------- Modified 3 point Secant ------------ osc=%d conv=%d",ix, oscillations, convergent);
          #endif // VERBOSE_SOLVER00 || VERBOSE_SOLVER0 || VERBOSE_SOLVER1 || VERBOSE_SOLVER2

          fnRCL(SREG_Y2);fnRCL(SREG_Y0);runFunction(ITM_SUB);fnStore(SREG_DY);
          fnRCL(SREG_X2);fnRCL(SREG_X0);runFunction(ITM_SUB);fnStore(SREG_DX);
          divFunction(!ADD_RAN, SREG_TOL);

          fnStore(SREG_TMP);

          #if defined(VERBOSE_SOLVER1) && defined(PC_BUILD)
            printRegisterToConsole(SREG_TMP," m1=","\n");
          #endif // VERBOSE_SOLVER1 && PC_BUILD

          fnRCL(SREG_Y2);fnRCL(SREG_Y1);runFunction(ITM_SUB);runFunction(ITM_MULT);

          #if defined(VERBOSE_SOLVER1) && defined(PC_BUILD)
            printRegisterToConsole(REGISTER_X," term1 lower=","\n");
          #endif // VERBOSE_SOLVER1 && PC_BUILD
          fnStore(SREG_L1);

          fnRCL(SREG_TMP);
          fnRCL(SREG_Y1);fnRCL(SREG_Y0);runFunction(ITM_SUB);
          fnRCL(SREG_X1);fnRCL(SREG_X0);runFunction(ITM_SUB);
          divFunction(!ADD_RAN, SREG_TOL);

          #if defined(VERBOSE_SOLVER1) && defined(PC_BUILD)
            printRegisterToConsole(REGISTER_X," m2=","\n");
          #endif // VERBOSE_SOLVER1 && PC_BUILD
          runFunction(ITM_SUB);
          #if defined(VERBOSE_SOLVER1) && defined(PC_BUILD)
            printRegisterToConsole(REGISTER_X," m1-m2 diff=","\n");
          #endif // VERBOSE_SOLVER1 && PC_BUILD
          fnRCL(SREG_Y2);
          #if defined(VERBOSE_SOLVER1) && defined(PC_BUILD)
            printRegisterToConsole(REGISTER_X," Y2=","\n");
          #endif // VERBOSE_SOLVER1 && PC_BUILD
          runFunction(ITM_MULT);
          #if defined(VERBOSE_SOLVER1) && defined(PC_BUILD)
            printRegisterToConsole(REGISTER_X," term2 lower=","\n");
          #endif // VERBOSE_SOLVER1 && PC_BUILD
          fnRecall(SREG_L1);
          runFunction(ITM_XexY);
          runFunction(ITM_SUB);
          #if defined(VERBOSE_SOLVER1) && defined(PC_BUILD)
          printRegisterToConsole(REGISTER_X," lower diff=","\n");
          #endif // VERBOSE_SOLVER1 && PC_BUILD
          fnRCL(SREG_Y2);fnRCL(SREG_Y1);runFunction(ITM_SUB);  //Y2-Y1
          runFunction(ITM_XexY);
          //get the 1/slope
          divFunction(!ADD_RAN, SREG_TOL);
          fnStore(SREG_TMP);
          #if defined(VERBOSE_SOLVER1) && defined(PC_BUILD)
            printRegisterToConsole(REGISTER_X," 1/slope=","\n");
          #endif // VERBOSE_SOLVER1 && PC_BUILD
          fnRCL(SREG_Y0);              // determine increment in x
          runFunction(ITM_MULT);       // increment to x is: y1 . DX/DY
          fnRCL(SREG_F);               // factor to stabilize Newton method. factor=1 is straight. factor=0.1 converges 10x slower.
          runFunction(ITM_MULT);       // increment to x
          #if defined(VERBOSE_SOLVER1) && defined(PC_BUILD)
            printRegisterToConsole(SREG_F,"    Factor=        ","\n");
            printRegisterToConsole(SREG_X0,"    New X =        "," - (");
            printRegisterToConsole(REGISTER_X,"",")\n");
          #endif // VERBOSE_SOLVER1 && PC_BUILD

          #if defined(VERBOSE_SOLVER00) || defined(VERBOSE_SOLVER0) || defined(VERBOSE_SOLVER1) || defined(VERBOSE_SOLVER2)
            printf("  ");
            printRegisterToConsole(REGISTER_X,"DeltaX=","\n");
          #endif // VERBOSE_SOLVER00 || VERBOSE_SOLVER0 || VERBOSE_SOLVER1 || VERBOSE_SOLVER2
          fnRCL(SREG_X0);
          runFunction(ITM_XexY);
          runFunction(ITM_SUB);        // subtract as per Newton, x1 - f/f'

          //try round numbers
          if(convergent > 3 && ix > 6 && oscillations == 0 && real34CompareLessEqual(REGISTER_REAL34_DATA(REGISTER_X),const34_1e_4)) {
            convergent = 0;
            double ix1 = convertRegisterToDouble(REGISTER_X);
            convertDoubleToReal34RegisterPush(roundf(1000.0 * ix1)/1000.0, REGISTER_X);
          }

          fnStore(SREG_X2N);           // store temporarily to new x2n
        }
      }



      /* Not in use
      //Experimental bisection method to kick out  in case of real arguments
      bool_t bisect = false;
      if(    !checkzero && !checkNaN
          && (    (getRegisterDataType(SREG_Y0) == dtReal34)
               && (getRegisterDataType(SREG_Y2) == dtReal34)
               && (real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(SREG_Y0),const34_1e_6))
               && (real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(SREG_Y2),const34_1e_6))
               && ((    (real34IsNegative(REGISTER_REAL34_DATA(SREG_Y0))) && (real34IsPositive(REGISTER_REAL34_DATA(SREG_Y2))))
                     || ((real34IsNegative(REGISTER_REAL34_DATA(SREG_Y2))) && (real34IsPositive(REGISTER_REAL34_DATA(SREG_Y0))))
                  )
               && !(    (    real34CompareGreaterEqual(REGISTER_REAL34_DATA(SREG_X2N),REGISTER_REAL34_DATA(SREG_X0))
                          && real34CompareGreaterEqual(REGISTER_REAL34_DATA(SREG_X2),REGISTER_REAL34_DATA(SREG_X2N)))
                     ||
                            (real34CompareLessEqual(REGISTER_REAL34_DATA(SREG_X2N),REGISTER_REAL34_DATA(SREG_X0))
                          && real34CompareLessEqual(REGISTER_REAL34_DATA(SREG_X2),REGISTER_REAL34_DATA(SREG_X2N)))
                   )
             )
        ) {
        bisect = true;
        #if defined(VERBOSE_SOLVER00) || defined(VERBOSE_SOLVER0) || defined(VERBOSE_SOLVER1) || defined(VERBOSE_SOLVER2)
          printf(" Using Bisection method: Y bracketed\n");
        #endif // VERBOSE_SOLVER00 || VERBOSE_SOLVER0 || VERBOSE_SOLVER1 || VERBOSE_SOLVER2
      }
      if(bisect) {
        fnRCL(SREG_X0);
        fnRCL(SREG_X2);
        runFunction(ITM_ADD);
        convertDoubleToReal34RegisterPush(2.0, REGISTERX);       //Leaving (x1+x2)/2
        divFunction(!ADD_RAN, SREG_TOL);
        fnStore(SREG_X2N);   // store temporarily to new x2n
      }
      */
      //#############################################


      #if defined(VERBOSE_SOLVER1) && defined(PC_BUILD)
        printf("               ");printRegisterToConsole(SREG_DX,"DX=","");printRegisterToConsole(SREG_DY,"DY=","\n");
        printf("               ");printRegisterToConsole(SREG_X0,"X0=","");printRegisterToConsole(SREG_Y0,"Y0=","\n");
        printf("   -------> newX2: ");printRegisterToConsole(SREG_X2N,"","\n");
        printf("               ");printRegisterToConsole(SREG_X1,"X1=","");printRegisterToConsole(SREG_Y1,"Y1=","\n");
        printf("               ");printRegisterToConsole(SREG_X2,"X2=","");printRegisterToConsole(SREG_Y2,"Y2=","\n");
      #endif // VERBOSE_SOLVER1 && PC_BUILD

      copySourceRegisterToDestRegister(SREG_Y1,SREG_Y0); //old y1 copied to y0
      copySourceRegisterToDestRegister(SREG_X1,SREG_X0); //old x1 copied to x0

      copySourceRegisterToDestRegister(SREG_Y2,SREG_Y1); //old y2 copied to y1
      copySourceRegisterToDestRegister(SREG_X2,SREG_X1); //old x2 copied to x1

      fnRCL(SREG_DY);   runFunction(ITM_ABS); //difference |dy| is in Y
      fnRCL(SREG_DX);   runFunction(ITM_ABS); //difference |dx| is in X


      checkzero =    checkzero
                  || (real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) && real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)))
                  || (    (real34CompareAbsLessThan(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(SREG_TOL)))
                       && (real34CompareAbsLessThan(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(SREG_TOL))));


      checkNaN =    checkNaN
                 || real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X))
                 || (getRegisterDataType(REGISTER_X) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X)) : 0)
                 || real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y))
                 || (getRegisterDataType(REGISTER_Y) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y)) : 0);


      #if defined(VERBOSE_SOLVER00) || defined(VERBOSE_SOLVER0) || defined(VERBOSE_SOLVER1)
        if(checkzero) {
          printf("--B1 Checkzero\n");
        }
        if(checkNaN) {
          printf("--B2 CheckNaN\n");
        }
        if(checkNaN || ix==NUMBERITERATIONS-1 || checkzero) {
          printf("--B3 Endflags: |DXr|=0:%u |DXr|<TOL:%u  |DXi|=0:%u |DYr|<TOL:%u |DYr|=0:%u |DYi|=0:%u |DXr|=NaN:%u |DXi|=NaN:%u |DYr|=NaN:%u |DYi|=NaN:%u \n",
                  (uint16_t)real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)),
                  (uint16_t)(real34CompareAbsLessThan(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(SREG_TOL))),
                  (uint16_t)real34IsZero(REGISTER_IMAG34_DATA(REGISTER_X)),
                  (uint16_t)real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)),
                  (uint16_t)(real34CompareAbsLessThan(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(SREG_TOL))),
                  (uint16_t)real34IsZero(REGISTER_IMAG34_DATA(REGISTER_Y)),
                  (uint16_t)real34IsNaN (REGISTER_REAL34_DATA(REGISTER_X)),
                  (uint16_t)real34IsNaN (REGISTER_IMAG34_DATA(REGISTER_X)),
                  (uint16_t)real34IsNaN (REGISTER_REAL34_DATA(REGISTER_Y)),
                  (uint16_t)real34IsNaN (REGISTER_IMAG34_DATA(REGISTER_Y))
                );
        }
      #endif // VERBOSE_SOLVER00 || VERBOSE_SOLVER0 || VERBOSE_SOLVER1
      ix++;
      ixd++;


      #if defined(VERBOSE_SOLVER2) && defined(PC_BUILD)
        if(!checkNaN && !(ix==NUMBERITERATIONS) && !checkzero) {
          printf("END     ix=%d |DX|<TOL:%d ",ix,real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DX), REGISTER_REAL34_DATA(SREG_TOL)));
          printRegisterToConsole(SREG_DX,"","\n");
          printf("END     ix=%d |DY|<TOL:%d ",ix,real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DY), REGISTER_REAL34_DATA(SREG_TOL)));
          printRegisterToConsole(SREG_DY,"","\n");
          printRegisterToConsole(REGISTER_Y,"END     DY=","\n");
        }
      #endif // VERBOSE_SOLVER2 && PC_BUILD

      #if defined(VERBOSE_SOLVER1) && defined(PC_BUILD)
        printRegisterToConsole(SREG_DX,">>> DX=","");
        printRegisterToConsole(SREG_DY," DY=","");
        printRegisterToConsole(SREG_TMP," 1/SLOPE=","\n");
      #endif // VERBOSE_SOLVER1 && PC_BUILD

      copySourceRegisterToDestRegister(SREG_X2N,SREG_X2);  //new x2

      //plots the ix vs abs.difference
      // |dy| is still in Y
      // replace X with ix
      // plot (ix,|dy|)


      #if defined(PC_BUILD)
        printf("ix = %i ", ix);
        printRegisterToConsole(SREG_X1,"X = "," ");
        printRegisterToConsole(REGISTER_Y,"Y = ","\n");
      #endif // PC_BUILD


      #if defined(DMCP_BUILD)
        lcd_refresh();
      #else // !DMCP_BUILD
        refreshLcd(NULL);
      #endif // DMCP_BUILD

      printHalfSecUpdate_Integer(timed, "Iter: ",loop++); //timed
      #if defined(DMCP_BUILD)
        if(keyWaiting()) {
          printHalfSecUpdate_Integer(force+1, "Interrupted Iter:",loop);
          fnClearStack(0);
          calcMode = CM_NORMAL;
          screenUpdatingMode = SCRUPD_AUTO;
          break;
        }
      #endif //DMCP_BUILD
    }


    //Iterations end

    bool_t   FLAG_FRACTN;
    if(getSystemFlag(FLAG_FRACT)) {
      FLAG_FRACTN = true;
    }
    else {
      FLAG_FRACTN = false;
    }
    clearSystemFlag(FLAG_FRACT);


    clearScreenPixels();
    #if defined(DMCP_BUILD)
      lcd_refresh();
    #else // !DMCP_BUILD
      refreshLcd(NULL);
    #endif // DMCP_BUILD

    convertDoubleToReal34RegisterPush(0.0, REGISTER_X);

    if(((ix >= NUMBERITERATIONS-1) && !checkzero) || checkNaN) {
      convertDoubleToReal34RegisterPush(0.0, REGISTER_X);
      //fnStrtoX(" Root not found. |d|=");
    }
    else {
      fnRCL(SREG_Y2);
    }

    fnRCL(SREG_X1);
    fnRCL(SREG_X2);

    //if(ix * 1.0  >  NUMBERITERATIONS * 0.5) {
    //  runFunction(ITM_PLOT_STAT);
    //}

    if(FLAG_FRACTN) {
      setSystemFlag(FLAG_FRACT)
    }
    else {
      clearSystemFlag(FLAG_FRACT);
    }

    #if defined(PC_BUILD)
      to_return:
    #endif // PC_BUILD
    calcMode = CM_NORMAL;
    SAVED_SIGMA_LAct = 0;   //prevent undo of last stats add action. REMOVE when STATS are not used anymore
    return;
  }
#endif //SAVE_SPACE_DM42_13GRF
#endif // !TESTSUITE_BUILD


//-----------------------------------------------------//-----------------------------------------------------
void fnEqSolvGraph (uint16_t func) {
#if !defined(SAVE_SPACE_DM42_13GRF)
#if !defined(TESTSUITE_BUILD)
  hourGlassIconEnabled = true;
  showHideHourGlass();
  #if defined(DMCP_BUILD)
    lcd_refresh();
  #else // !DMCP_BUILD
    refreshLcd(NULL);
  #endif // DMCP_BUILD

  graphVariable = currentSolverVariable;
  if(graphVariable<0) {
    graphVariable = -graphVariable;
  }

  if(graphVariable >= FIRST_NAMED_VARIABLE && graphVariable <= LAST_NAMED_VARIABLE) {
    #if(defined(VERBOSE_SOLVER00) || defined(VERBOSE_SOLVER0)) && defined(PC_BUILD)
      printf("graphVariable accepted: %i\n", graphVariable);
    #endif // (VERBOSE_SOLVER00 || VERBOSE_SOLVER0) && PC_BUILD
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if(EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "unexpected parameter %u", graphVariable);
      moreInfoOnError("In function fnEqSolvGraph:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
    return;
  }


  //initialize x
  currentSolverStatus &= ~SOLVER_STATUS_READY_TO_EXECUTE;

  switch(func) {
    case EQ_SOLVE: {
      printStatus(1,errorMessages[COMPLEX_SOLVER],force);
      fnClDrawMx();
      strcpy(plotStatMx,"DrwMX");
      statGraphReset();

      double ix1 = convertRegisterToDouble(REGISTER_X);
      double ix0 = convertRegisterToDouble(REGISTER_Y);
      #if(defined(VERBOSE_SOLVER00) || defined(VERBOSE_SOLVER0)) && defined(PC_BUILD)
        printRegisterToConsole(REGISTER_Y,">>> ix0=","");
        printRegisterToConsole(REGISTER_X," ix1=","\n");
      #endif // (VERBOSE_SOLVER00 || VERBOSE_SOLVER0) && PC_BUILD
      calcRegister_t SREG_STARTX0 = __STARTX0;
      calcRegister_t SREG_STARTX1 = __STARTX1;
      copySourceRegisterToDestRegister(REGISTER_Y,SREG_STARTX0);
      copySourceRegisterToDestRegister(REGISTER_X,SREG_STARTX1);
      if(ix1>ix0 + 0.01 && ix1!=DOUBLE_NOT_INIT && ix0!=DOUBLE_NOT_INIT) { //pre-condition the plotter
        x_min = ix0;
        x_max = ix1;
      }
      #if(defined(VERBOSE_SOLVER00) || defined(VERBOSE_SOLVER0)) && defined(PC_BUILD)
        printf("xmin:%f, xmax:%f\n",x_min,x_max);
      #endif // (VERBOSE_SOLVER00 || VERBOSE_SOLVER0) && PC_BUILD
      initialize_function();
      complexSolver();
      break;
    }
    case EQ_PLOT: {
      printStatus(1,errorMessages[GRAPHING],force);
      double ix1 = convertRegisterToDouble(REGISTER_X);
      double ix0 = convertRegisterToDouble(REGISTER_Y);
      #if(defined(VERBOSE_SOLVER00) || defined(VERBOSE_SOLVER0)) && defined(PC_BUILD)
        printRegisterToConsole(REGISTER_Y,">>> ix0=","");
        printRegisterToConsole(REGISTER_X," ix1=","\n");
      #endif // (VERBOSE_SOLVER00 || VERBOSE_SOLVER0) && PC_BUILD

      fnClDrawMx();
      strcpy(plotStatMx,"DrwMX");
      //statGraphReset();    //C43 removed to allow changing of graph params

      fnDrop(0);
      fnDrop(0);
      if(ix1>ix0 + 0.01 && ix1!=DOUBLE_NOT_INIT && ix0!=DOUBLE_NOT_INIT) { //pre-condition the plotter
        x_min = ix0;
        x_max = ix1;
      }
      if(x_min > x_max) { //swap if entered in incorrect sequence
        float kk = x_max;
        x_max = x_min;
        x_min = kk;
      }
      float x_d = fabs(x_max-x_min);
      if(x_d < 0.0001) { //too close together for float type
        if(fabs(x_min)<0.0001 || fabs(x_max)<0.0001) { //abort old values and show -1 to 1
          x_d = 10;
        }
        x_min = x_min - 0.1 * x_d;
        x_max = x_max + 0.1 * x_d;
      }
      #if(defined(VERBOSE_SOLVER00) || defined(VERBOSE_SOLVER0)) && defined(PC_BUILD)
        printf("xmin:%f, xmax:%f\n",x_min,x_max);
      #endif // (VERBOSE_SOLVER00 || VERBOSE_SOLVER0) && PC_BUILD

      initialize_function();
      graph_eqn(noInitDrwMx);
      break;
    }
    default: ;
  }
#endif // !TESTSUITE_BUILD
#endif // SAVE_SPACE_DM42_13GRF
}
