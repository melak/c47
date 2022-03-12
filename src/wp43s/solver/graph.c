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
 * \file graph.c Graphing module
 ***********************************************/

#include "solver/graph.h"

#include "defines.h"
#include "mathematics/comparisonReals.h"
#include "charString.h"
#include "constantPointers.h"
#include "error.h"
#include "flags.h"
#include "items.h"
#include "mathematics/invert.h"
#include "matrix.h"
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
#include "wp43s.h"
#include <math.h>
#include <string.h>


#ifdef PC_BUILD
  //Verbose directives can be simulataneously selected
  //#define VERBOSE_SOLVER00   // minimal text
  //#define VERBOSE_SOLVER0  // a lot less text
  //#define VERBOSE_SOLVER1  // a lot less text
  //#define VERBOSE_SOLVER2  // verbose a lot
#else
  #undef VERBOSE_SOLVER00
  #undef VERBOSE_SOLVER0
  #undef VERBOSE_SOLVER1
  #undef VERBOSE_SOLVER2
#endif


//Todo: involve https://en.wikipedia.org/wiki/Brent%27s_method#Brent's_method

#define COMPLEXKICKER true       //flag to allow conversion to complex plane if no convergenge found
#define CHANGE_TO_MOD_SECANT 0   //at iteration nn go to the modified secant method. 0 means immediately
#define CONVERGE_FACTOR 1.0f        //
#define NUMBERITERATIONS 35      // Must be smaller than LIM (see STATS)


char     plotStatMx[8];



#ifndef TESTSUITE_BUILD
	static void fnRCL(int16_t inp) { //DONE
	  setSystemFlag(FLAG_ASLIFT);
	  if(inp == TEMP_REGISTER_1) {
	    liftStack();
	    copySourceRegisterToDestRegister(inp, REGISTER_X);
	  }
	  else {
	    fnRecall(inp);
	  }
	}

	static void fnStrtoX(char buff[]) {
	  setSystemFlag(FLAG_ASLIFT); // 5
	  liftStack();
	  int16_t mem = stringByteLength(buff) + 1;
	  reallocateRegister(REGISTER_X, dtString, TO_BLOCKS(mem), amNone);
	  xcopy(REGISTER_STRING_DATA(REGISTER_X), buff, mem);
	  setSystemFlag(FLAG_ASLIFT);
	}

  static void convertDoubleToReal34RegisterPush(double x, calcRegister_t destination) {
    setSystemFlag(FLAG_ASLIFT);
    liftStack();
    convertDoubleToReal34Register(x, destination);
    setSystemFlag(FLAG_ASLIFT);
  }
#endif


void fnPlot(uint16_t unusedButMandatoryParameter) {
  lastPlotMode = PLOT_NOTHING;
  fnPlotStat(PLOT_GRAPH);
}


#ifndef TESTSUITE_BUILD
  static void initialize_function(void){
    if(graphVariable > 0) {
      #ifdef PC_BUILD
        //printf(">>> graphVariable = %i\n", graphVariable);
        if(lastErrorCode != 0) { 
          #ifdef VERBOSE_SOLVER00
          printf("ERROR CODE in initialize_functionA: %u\n",lastErrorCode); 
          #endif //VERBOSE_SOLVER1
          lastErrorCode = 0;
        }
      #endif //PC_BUILD
    } else {
      #ifdef PC_BUILD
        //printf(">>> graphVariable = %i\n", graphVariable);
          #ifdef VERBOSE_SOLVER00
          printf("ERROR CODE in initialize_functionB: %u\n",lastErrorCode); 
          #endif //VERBOSE_SOLVER1
      #endif //PC_BUILD    	
    }
  }
#endif //TESTSUITE_BUILD


#ifndef TESTSUITE_BUILD
  static void execute_rpn_function(void){
    if(graphVariable <= 0 || graphVariable > 65535) return;

    calcRegister_t regStats = graphVariable;
    if(regStats != INVALID_VARIABLE) {
      fnStore(regStats);                  //place X register into x
      fnEqCalc(0);
      #ifdef PC_BUILD
        if(lastErrorCode != 0) { 
          #ifdef VERBOSE_SOLVER00
          printf("ERROR CODE in execute_rpn_function/fnEqCalc: %u\n",lastErrorCode); 
          #endif //VERBOSE_SOLVER1
          lastErrorCode = 0;
        }
      #endif
      fnRCL(regStats);
      #if (defined VERBOSE_SOLVER0) && (defined PC_BUILD)
        printRegisterToConsole(REGISTER_X,">>> Calc x=","");
        printRegisterToConsole(REGISTER_Y," y=","");
      #endif
    } else {
      #ifdef PC_BUILD
        #ifdef VERBOSE_SOLVER00
        printf("ERROR in execute_rpn_function; invalid variable: %u\n",lastErrorCode); 
        #endif //VERBOSE_SOLVER1
        lastErrorCode = 0;
      #endif
    }
  }

  static bool_t regIsLowerThanTol(calcRegister_t REG, calcRegister_t TOL) {
    return ( (real34IsZero(REGISTER_REAL34_DATA(REG)) && (getRegisterDataType(REG) == dtComplex34 ? real34IsZero(REGISTER_IMAG34_DATA(REG)) : 1 )) 

           ||
           
           ( (real34CompareAbsLessThan(REGISTER_REAL34_DATA(REG), REGISTER_REAL34_DATA(TOL))) && 
                  (getRegisterDataType(REG) == dtComplex34 ? real34CompareAbsLessThan(REGISTER_IMAG34_DATA(REG), REGISTER_REAL34_DATA(TOL)) : 1 ) 
           ) 
         );
  }


#define ADD_RAN true
static void divFunction(bool_t addRandom, calcRegister_t TOL) {
  if((real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) && (getRegisterDataType(REGISTER_Y) == dtComplex34 ? real34IsZero(REGISTER_IMAG34_DATA(REGISTER_Y)) : 1 ) )
    || real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || (getRegisterDataType(REGISTER_Y) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y)) : 0 ) ) {
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
    #ifdef PC_BUILD
      printf(">>> ADD random number to denominator to prevent infinite result\n");
    #endif
    convertDoubleToReal34RegisterPush(1e-6, REGISTER_X);
    runFunction(ITM_ADD);
    runFunction(ITM_RAN);
    runFunction(ITM_ADD);
  }
  runFunction(ITM_DIV);
}


int16_t osc = 0;
uint8_t DXR = 0, DYR = 0, DXI = 0, DYI = 0;


void check_osc(uint8_t ii){
   switch (ii & 0b00111111) {
     case 0b001111:
     case 0b011110:
     case 0b111100:
     case 0b010101:
     case 0b101010:

     case 0b011011:
     case 0b110110:
     case 0b101101: osc++;
     default:;
   }
   switch (ii) {
     case 0b01001001:
     case 0b10010010:
     case 0b00100100: osc++;
     default:;
   }
}

#endif //TESTSUITE_BUILD

//###################################################################################
//PLOTTER

#ifndef TESTSUITE_BUILD
  int32_t drawMxN(void){
    uint16_t rows = 0;
    if(plotStatMx[0]!='D') return 0;
    calcRegister_t regStats = findNamedVariable(plotStatMx);
    if(regStats == INVALID_VARIABLE) {
      return 0;
    }
    if(isStatsMatrix(&rows,plotStatMx)) {
      real34Matrix_t stats;
      linkToRealMatrixRegister(regStats, &stats);
      return stats.header.matrixRows;
    } else return 0;
  }
#endif // TESTSUITE_BUILD



void fnClDrawMx(void) {
  if(plotStatMx[0]!='D') strcpy(plotStatMx,"DrwMX");
  calcRegister_t regStats = findNamedVariable(plotStatMx);
  if(regStats == INVALID_VARIABLE) {
    allocateNamedVariable(plotStatMx, dtReal34, REAL34_SIZE);
    regStats = findNamedVariable(plotStatMx);
  }
  clearRegister(regStats);                  // this should change to delete the named variable STATS once the delete function is available. Until then write 0.0 into STATS.
  if(regStats == INVALID_VARIABLE) {
    displayCalcErrorMessage(ERROR_NO_MATRIX_INDEXED, ERR_REGISTER_LINE, REGISTER_X); // Invalid input data type for this operation
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "DrwMX matrix not created");
      moreInfoOnError("In function fnClPlotData:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}

#ifndef TESTSUITE_BUILD
  static void AddtoDrawMx() {
    real_t x, y;
    uint16_t rows = 0, cols;
    calcRegister_t regStats = findNamedVariable(plotStatMx);
    if(!isStatsMatrix(&rows,plotStatMx)) {
      regStats = allocateNamedMatrix(plotStatMx, 1, 2);
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
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "additional matrix line not added; rows = %i",rows);
        moreInfoOnError("In function AddtoDrawMx:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
  }
#endif // TESTSUITE_BUILD


void graph_eqn(uint16_t mode) {
  #ifndef TESTSUITE_BUILD
    if(graphVariable <= 0 || graphVariable > 65535) return;
    calcMode = CM_GRAPH;
    double x; 
    fnClearStack(0);

    if(mode == 1) {
      fnClDrawMx();
    }
    for(x=x_min; x<=x_max; x+=0.99999*(x_max-x_min)/SCREEN_WIDTH_GRAPH*10) {    //Reduced the amount of sample data from 400 points to 40 points
      convertDoubleToReal34RegisterPush(x, REGISTER_X);      
      //leaving y in Y and x in X
      execute_rpn_function();
      AddtoDrawMx();

      #if (defined VERBOSE_SOLVER0) && (defined PC_BUILD)
        printf(">>> Into DrwMX:%i points ",drawMxN());
        printRegisterToConsole(REGISTER_X,"X:","");
        printRegisterToConsole(REGISTER_Y," Y:","\n");
      #endif
      #ifdef PC_BUILD
        if(lastErrorCode == 24) { printf("ERROR CODE CANNOT STAT COMPLEX RESULT, ignored\n"); lastErrorCode = 0;}
      #endif //PC_BUILD
    }

    fnClearStack(0);
    fnPlot(0);
  #endif
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


#ifndef TESTSUITE_BUILD
  static void graph_solver() {         //Input parameters in registers SREG_STARTX0, SREG_STARTX1
    if(graphVariable <= 0 || graphVariable > 65535) return;

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
      #ifdef PC_BUILD
        printf(">>> ADD random number to second input parameter to prevent infinite result\n");
      #endif
      runFunction(ITM_ADD);
      runFunction(ITM_RAN);
      runFunction(ITM_ADD);
      fnStore(SREG_X0);
    } else {
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
    convertDoubleToReal34RegisterPush(1E-30, REGISTER_X); 
    fnStore(SREG_TOL);                                   // tolerance

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
    } else {
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


                          #if ((defined VERBOSE_SOLVER00) || (defined VERBOSE_SOLVER0) || (defined VERBOSE_SOLVER1) || (defined VERBOSE_SOLVER2)) && (defined PC_BUILD)
                            printf("INIT:   ix=%d \n",ix); 
                            printRegisterToConsole(SREG_X0,"Init X0= ","\n");
                            printRegisterToConsole(SREG_X1,"Init X1= ","\n");
                            printRegisterToConsole(SREG_X2,"Init X2= ","\n");
                            printRegisterToConsole(SREG_Y0,"Init Y0= ","\n");
                            printRegisterToConsole(SREG_Y1,"Init Y1= ","\n");
                            printRegisterToConsole(SREG_Y2,"Init Y2= ","\n");
                          #endif //


  //###############################################################################################################
  //#################################################### Iteration start ##########################################
    while(ix<NUMBERITERATIONS && !checkNaN && !checkzero) 
    {
                    #ifdef PC_BUILD
                      if(lastErrorCode != 0) { 
                        printf(">>> ERROR CODE INITIALLY NON-ZERO = %d <<<<<\n",lastErrorCode); 
                        return;
                      }
                    #endif

      //assumes X2 is in R91
      //Identify oscillations in real or imag: increment osc flag
  //    osc = 0;

      if( (real34IsNegative(REGISTER_REAL34_DATA(SREG_DX)) && real34IsPositive(REGISTER_REAL34_DATA(SREG_Xold))) ||
          (real34IsPositive(REGISTER_REAL34_DATA(SREG_DX)) && real34IsNegative(REGISTER_REAL34_DATA(SREG_Xold))) ) {
          DXR = (DXR << 1) + 1;       
      } else DXR = DXR << 1;

      if( (real34IsNegative(REGISTER_REAL34_DATA(SREG_DY)) && real34IsPositive(REGISTER_REAL34_DATA(SREG_Yold))) ||
          (real34IsPositive(REGISTER_REAL34_DATA(SREG_DY)) && real34IsNegative(REGISTER_REAL34_DATA(SREG_Yold))) ) {
          DYR = (DYR << 1) + 1;       
      } else DYR = DYR << 1;
      
      if((getRegisterDataType(SREG_DX) == dtComplex34 && getRegisterDataType(SREG_Xold) == dtComplex34) &&
           ((real34IsNegative(REGISTER_IMAG34_DATA(SREG_DX)) && real34IsPositive(REGISTER_IMAG34_DATA(SREG_Xold))) ||
            (real34IsPositive(REGISTER_IMAG34_DATA(SREG_DX)) && real34IsNegative(REGISTER_IMAG34_DATA(SREG_Xold))) )) {
               DXI = (DXI << 1) + 1;       
      } else DXI = DXI << 1;
      
      if((getRegisterDataType(SREG_DY) == dtComplex34 && getRegisterDataType(SREG_Yold) == dtComplex34) &&
           ((real34IsNegative(REGISTER_IMAG34_DATA(SREG_DY)) && real34IsPositive(REGISTER_IMAG34_DATA(SREG_Yold))) ||
            (real34IsPositive(REGISTER_IMAG34_DATA(SREG_DY)) && real34IsNegative(REGISTER_IMAG34_DATA(SREG_Yold))) )) {
               DYI = (DYI << 1) + 1;       
      } else DYI = DYI << 1;

     check_osc(DXR);
     check_osc(DYR);
     check_osc(DXI);
     check_osc(DYI);

      //If osc flag is active, that is any delta polarity change, then increment oscillation count
      if(osc > 0) oscillations++; else
        oscillations = max(0,oscillations-1);

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
      } else {
        convergent = max(0,convergent-1);
      }
                      #ifdef VERBOSE_SOLVER0
                        printf("##### ix= %d osc= %d  conv= %d n",ix, oscillations, convergent);
                      #endif //VERBOSE_SOLVER1
                      #ifdef VERBOSE_SOLVER1
                        printf("################################### ix= %d osc= %d  conv= %d ###########################################\n",ix, oscillations, convergent);
                      #endif //VERBOSE_SOLVER1

      if(convergent > 6 && oscillations > 3) {

                      #ifdef PC_BUILD
                        printf("    --   reset detection from =convergent%i and oscillations=%i to ", convergent, oscillations);
                      #endif
        convergent = 2;
        oscillations = 2;
                      #ifdef PC_BUILD
                        printf("%i and %i\n", convergent, oscillations);
                      #endif
      }

  // If increment is oscillating it is assumed that it is unstable and needs to have a complex starting value
      //assumes X2 is in R91
      if( (((oscillations >= 3) && (ixd > 9) && (convergent <= 2) )
         //|| (oscillations == 0 && convergent > 6 && real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DX), const34_1e_4) && (getRegisterDataType(SREG_DX) == dtComplex34 ? real34CompareAbsLessThan(REGISTER_IMAG34_DATA(SREG_DX), const34_1e_4) : 1 )  )

      )) {
        if( COMPLEXKICKER && (kicker ==0) && (convergent <= 1) ) kicker = kicker +2;
        ixd = 0;
        oscillations = 0;
        convergent = 0;
                      #if (defined VERBOSE_SOLVER2) && (defined PC_BUILD)
                        printRegisterToConsole(SREG_X2,"\n>>>>>>>>>> from ","");
                      #endif //VERBOSE_SOLVER2
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

                      #if ((defined VERBOSE_SOLVER00) || (defined VERBOSE_SOLVER0)) && (defined PC_BUILD)
                        printf("------- Kicked oscillation, #%d, ", kicker);
                        printRegisterToConsole(REGISTER_X," multiplied: ","\n");
                      #endif  //VERBOSE_SOLVER1

        kicker++;

        runFunction(ITM_MULT);              //add just to force it complex  //
        fnStore(SREG_X2); //replace X2 value                                //

                      #if (defined VERBOSE_SOLVER2) && (defined PC_BUILD)
                        printRegisterToConsole(SREG_X2," to ","\n");
                      #endif //VERBOSE_SOLVER2
      }


  //@@@@@@@@@@@@@@@@@ CALCULATE NEW Y2, AND PLAUSIBILITY @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
      fnRCL(SREG_X2);                                       // get (X2,Y2)
      execute_rpn_function();                               // leaving y2 in Y and x2 in X
      copySourceRegisterToDestRegister(REGISTER_Y,SREG_Y2); // y2

                      #if (defined VERBOSE_SOLVER1) && (defined PC_BUILD)
                        printf("    :   ix=%d",ix);
                        printRegisterToConsole(SREG_X2," X2="," ");
                        printRegisterToConsole(SREG_Y2," Y2=","\n");
                      #endif //VERBOSE_SOLVER2

      // y2 in Y and x2 in X
      checkzero = checkzero ||   regIsLowerThanTol(SREG_Y2,SREG_TOL);
      checkNaN  = checkNaN  ||   real34IsNaN(REGISTER_REAL34_DATA(SREG_X2)) || (getRegisterDataType(SREG_X2) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(SREG_X2)) : 0 ) ||
                                 real34IsNaN(REGISTER_REAL34_DATA(SREG_Y2)) || (getRegisterDataType(SREG_Y2) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(SREG_Y2)) : 0 ) ; 
                    
                      #if ((defined VERBOSE_SOLVER00) || (defined VERBOSE_SOLVER0)) && (defined PC_BUILD)
                      if(checkNaN || ix==NUMBERITERATIONS-1 || checkzero) {
                         printf("-->A Endflags zero: Y2r=0:%u Y2i=0:%u X2r=NaN:%u X2i=NaN:%u Y2r=NaN:%u Y2i=NaN%u \n",
                          (uint16_t)real34IsZero(REGISTER_REAL34_DATA(SREG_Y2)),(uint16_t)real34IsZero(REGISTER_IMAG34_DATA(SREG_Y2)),
                          (uint16_t)real34IsNaN (REGISTER_REAL34_DATA(SREG_X2)),(uint16_t)real34IsNaN (REGISTER_IMAG34_DATA(SREG_X2)),
                          (uint16_t)real34IsNaN (REGISTER_REAL34_DATA(SREG_Y2)),(uint16_t)real34IsNaN (REGISTER_IMAG34_DATA(SREG_Y2))
                          );
                      }
                      #endif //VERBOSE_SOLVER1
                      #if (defined VERBOSE_SOLVER2) && (defined PC_BUILD)
                        printf("   ix=%d checkend=%d X2=",ix, checkNaN || ix==NUMBERITERATIONS-1 || checkzero); 
                        printRegisterToConsole(SREG_X2,"","");
                        printRegisterToConsole(SREG_Y2,"Y2=","\n");
                      #endif //VERBOSE_SOLVER2



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
                          #if (defined VERBOSE_SOLVER00) || (defined VERBOSE_SOLVER0) || (defined VERBOSE_SOLVER1) || (defined VERBOSE_SOLVER2)
                            printf("%3i ---------- Using normal Secant dydx 2-samples - osc=%d conv=%d",ix, oscillations, convergent);
                          #endif            
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
                          #if (defined VERBOSE_SOLVER00) || (defined VERBOSE_SOLVER0) || (defined VERBOSE_SOLVER1) || (defined VERBOSE_SOLVER2)
                            printf("%3i ---------- Using Secant with 3 samples dy/dx -- osc=%d conv=%d",ix, oscillations, convergent);
                          #endif
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
                          #if (defined VERBOSE_SOLVER2) && (defined PC_BUILD)
                            fnInvert(0);
                            printRegisterToConsole(REGISTER_X," SLOPE=","\n");
                            fnInvert(0); 
                          #endif

          fnRCL(SREG_Y2);      // determine increment in x
          runFunction(ITM_MULT);       // increment to x is: y1 . DX/DY
          fnRCL(SREG_F);       // factor to stabilize Newton method. factor=1 is straight. factor=0.1 converges 10x slower.
          runFunction(ITM_MULT);       // increment to x

                          #if (defined VERBOSE_SOLVER1) && (defined PC_BUILD)
                            printRegisterToConsole(SREG_F,"    Factor=        ","\n");
                            printRegisterToConsole(SREG_X1,"    New X =        "," - ");
                            printRegisterToConsole(REGISTER_X," - (",")\n");
                          #endif

          fnRCL(SREG_X2);
          runFunction(ITM_XexY);
          runFunction(ITM_SUB);       // subtract as per Newton, x1 - f/f'
          fnStore(SREG_X2N);          // store temporarily to new x2n
                          #if (defined VERBOSE_SOLVER00) || (defined VERBOSE_SOLVER0) || (defined VERBOSE_SOLVER1) || (defined VERBOSE_SOLVER2)
                          printf("  ");
                          printRegisterToConsole(SREG_X2N,"New X=","\n");
  //                        printRegisterToConsole(REGISTER_Y,"Secant DeltaX=","\n");
                          #endif //VERBOSE_SOLVER1

  } else 



  {
       // ---------- Modified 3 point Secant ------------
    if((ix == 0) || (!checkzero && !checkNaN)) {

                          #if (defined VERBOSE_SOLVER00) || (defined VERBOSE_SOLVER0) || (defined VERBOSE_SOLVER1) || (defined VERBOSE_SOLVER2)
                          printf("%3i ---------- Modified 3 point Secant ------------ osc=%d conv=%d",ix, oscillations, convergent);
                          #endif //VERBOSE_SOLVER1
      
      fnRCL(SREG_Y2);fnRCL(SREG_Y0);runFunction(ITM_SUB);fnStore(SREG_DY);
      fnRCL(SREG_X2);fnRCL(SREG_X0);runFunction(ITM_SUB);fnStore(SREG_DX);
      divFunction(!ADD_RAN, SREG_TOL);

      fnStore(SREG_TMP);

                          #if (defined VERBOSE_SOLVER1) && (defined PC_BUILD)
                            printRegisterToConsole(SREG_TMP," m1=","\n");
                          #endif //VERBOSE_SOLVER2

      fnRCL(SREG_Y2);fnRCL(SREG_Y1);runFunction(ITM_SUB);runFunction(ITM_MULT);

                          #if (defined VERBOSE_SOLVER1) && (defined PC_BUILD)
                            printRegisterToConsole(REGISTER_X," term1 lower=","\n");
                          #endif //VERBOSE_SOLVER2
      fnStore(SREG_L1);

      fnRCL(SREG_TMP);
      fnRCL(SREG_Y1);fnRCL(SREG_Y0);runFunction(ITM_SUB);
      fnRCL(SREG_X1);fnRCL(SREG_X0);runFunction(ITM_SUB);
      divFunction(!ADD_RAN, SREG_TOL);

                          #if (defined VERBOSE_SOLVER1) && (defined PC_BUILD)
                            printRegisterToConsole(REGISTER_X," m2=","\n");
                          #endif //VERBOSE_SOLVER2
      runFunction(ITM_SUB);
                          #if (defined VERBOSE_SOLVER1) && (defined PC_BUILD)
                            printRegisterToConsole(REGISTER_X," m1-m2 diff=","\n");
                          #endif //VERBOSE_SOLVER2
      fnRCL(SREG_Y2);
                          #if (defined VERBOSE_SOLVER1) && (defined PC_BUILD)
                            printRegisterToConsole(REGISTER_X," Y2=","\n");
                          #endif //VERBOSE_SOLVER2
      runFunction(ITM_MULT);
                          #if (defined VERBOSE_SOLVER1) && (defined PC_BUILD)
                            printRegisterToConsole(REGISTER_X," term2 lower=","\n");
                          #endif //VERBOSE_SOLVER2
      fnRecall(SREG_L1);
      runFunction(ITM_XexY);
      runFunction(ITM_SUB);
                          #if (defined VERBOSE_SOLVER1) && (defined PC_BUILD)
                            printRegisterToConsole(REGISTER_X," lower diff=","\n");
                          #endif //VERBOSE_SOLVER2
      fnRCL(SREG_Y2);fnRCL(SREG_Y1);runFunction(ITM_SUB);  //Y2-Y1
      runFunction(ITM_XexY);
      //get the 1/slope
      divFunction(!ADD_RAN, SREG_TOL);
      fnStore(SREG_TMP);
                          #if (defined VERBOSE_SOLVER1) && (defined PC_BUILD)
                            printRegisterToConsole(REGISTER_X," 1/slope=","\n");
                          #endif //VERBOSE_SOLVER2
      fnRCL(SREG_Y0);              // determine increment in x
      runFunction(ITM_MULT);       // increment to x is: y1 . DX/DY
      fnRCL(SREG_F);               // factor to stabilize Newton method. factor=1 is straight. factor=0.1 converges 10x slower.
      runFunction(ITM_MULT);       // increment to x
                          #if (defined VERBOSE_SOLVER1) && (defined PC_BUILD)
                            printRegisterToConsole(SREG_F,"    Factor=        ","\n");
                            printRegisterToConsole(SREG_X0,"    New X =        "," - (");
                            printRegisterToConsole(REGISTER_X,"",")\n");
                          #endif //VERBOSE_SOLVER1

                          #if (defined VERBOSE_SOLVER00) || (defined VERBOSE_SOLVER0) || (defined VERBOSE_SOLVER1) || (defined VERBOSE_SOLVER2)
                          printf("  ");
                          printRegisterToConsole(REGISTER_X,"DeltaX=","\n");
                          #endif //VERBOSE_SOLVER1
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
     if(   !checkzero && !checkNaN &&
           ( (getRegisterDataType(SREG_Y0) == dtReal34) && 
             (getRegisterDataType(SREG_Y2) == dtReal34) && 
             (real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(SREG_Y0),const34_1e_6)) && 
             (real34CompareAbsGreaterThan(REGISTER_REAL34_DATA(SREG_Y2),const34_1e_6)) &&
              (
                ( (real34IsNegative(REGISTER_REAL34_DATA(SREG_Y0))) && (real34IsPositive(REGISTER_REAL34_DATA(SREG_Y2))) ) ||
                ( (real34IsNegative(REGISTER_REAL34_DATA(SREG_Y2))) && (real34IsPositive(REGISTER_REAL34_DATA(SREG_Y0))) )
              ) &&

           !((real34CompareGreaterEqual(REGISTER_REAL34_DATA(SREG_X2N),REGISTER_REAL34_DATA(SREG_X0)) &&
           real34CompareGreaterEqual(REGISTER_REAL34_DATA(SREG_X2),REGISTER_REAL34_DATA(SREG_X2N)) ) ||
           (real34CompareLessEqual(REGISTER_REAL34_DATA(SREG_X2N),REGISTER_REAL34_DATA(SREG_X0)) &&
           real34CompareLessEqual(REGISTER_REAL34_DATA(SREG_X2),REGISTER_REAL34_DATA(SREG_X2N)) ) )

          ) 
        ) { bisect = true;
                          #if (defined VERBOSE_SOLVER00) || (defined VERBOSE_SOLVER0) || (defined VERBOSE_SOLVER1) || (defined VERBOSE_SOLVER2)
                            printf(" Using Bisection method: Y bracketed\n");
                          #endif
          }
    if(bisect){
      fnRCL(SREG_X0);
      fnRCL(SREG_X2);
      runFunction(ITM_ADD);
      convertDoubleToReal34RegisterPush(2.0, REGISTERX);       //Leaving (x1+x2)/2
      divFunction(!ADD_RAN, SREG_TOL);
      fnStore(SREG_X2N);   // store temporarily to new x2n
    }
  */
  //#############################################


                          #if (defined VERBOSE_SOLVER1) && (defined PC_BUILD)
                            printf("               ");printRegisterToConsole(SREG_DX,"DX=","");printRegisterToConsole(SREG_DY,"DY=","\n");                                   
                            printf("               ");printRegisterToConsole(SREG_X0,"X0=","");printRegisterToConsole(SREG_Y0,"Y0=","\n");
                            printf("   -------> newX2: ");printRegisterToConsole(SREG_X2N,"","\n");
                            printf("               ");printRegisterToConsole(SREG_X1,"X1=","");printRegisterToConsole(SREG_Y1,"Y1=","\n");
                            printf("               ");printRegisterToConsole(SREG_X2,"X2=","");printRegisterToConsole(SREG_Y2,"Y2=","\n");
                          #endif //VERBOSE_SOLVER1


      copySourceRegisterToDestRegister(SREG_Y1,SREG_Y0); //old y1 copied to y0
      copySourceRegisterToDestRegister(SREG_X1,SREG_X0); //old x1 copied to x0

      copySourceRegisterToDestRegister(SREG_Y2,SREG_Y1); //old y2 copied to y1
      copySourceRegisterToDestRegister(SREG_X2,SREG_X1); //old x2 copied to x1

      fnRCL(SREG_DY);   runFunction(ITM_ABS); //difference |dy| is in Y
      fnRCL(SREG_DX);   runFunction(ITM_ABS); //difference |dx| is in X


      checkzero = checkzero || 
                  ( real34IsZero(REGISTER_REAL34_DATA(REGISTER_X)) && real34IsZero(REGISTER_REAL34_DATA(REGISTER_Y)) ) ||
                  ( (real34CompareAbsLessThan(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(SREG_TOL))) && 
                    (real34CompareAbsLessThan(REGISTER_REAL34_DATA(REGISTER_Y), REGISTER_REAL34_DATA(SREG_TOL)))          ) ;


      checkNaN = checkNaN || 
                  real34IsNaN(REGISTER_REAL34_DATA(REGISTER_X)) || (getRegisterDataType(REGISTER_X) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_X)) : 0 ) ||
                  real34IsNaN(REGISTER_REAL34_DATA(REGISTER_Y)) || (getRegisterDataType(REGISTER_Y) == dtComplex34 ? real34IsNaN(REGISTER_IMAG34_DATA(REGISTER_Y)) : 0 ) ; 


                          #if (defined VERBOSE_SOLVER00) || (defined VERBOSE_SOLVER0) || (defined VERBOSE_SOLVER1)
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
                          #endif //VERBOSE_SOLVER
      ix++; ixd++;


                          #if (defined VERBOSE_SOLVER2) && (defined PC_BUILD)
                            if(!checkNaN && !(ix==NUMBERITERATIONS) && !checkzero) {
                              printf("END     ix=%d |DX|<TOL:%d ",ix,real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DX), REGISTER_REAL34_DATA(SREG_TOL)));
                              printRegisterToConsole(SREG_DX,"","\n");
                              printf("END     ix=%d |DY|<TOL:%d ",ix,real34CompareAbsLessThan(REGISTER_REAL34_DATA(SREG_DY), REGISTER_REAL34_DATA(SREG_TOL)));
                              printRegisterToConsole(SREG_DY,"","\n");
                              printRegisterToConsole(REGISTER_Y,"END     DY=","\n");
                            }
                          #endif //VERBOSE_SOLVER2

                          #if (defined VERBOSE_SOLVER1) && (defined PC_BUILD)
                            printRegisterToConsole(SREG_DX,">>> DX=","");
                            printRegisterToConsole(SREG_DY," DY=","");
                            printRegisterToConsole(SREG_TMP," 1/SLOPE=","\n");
                          #endif

      copySourceRegisterToDestRegister(SREG_X2N,SREG_X2);  //new x2    

      //plots the ix vs abs.difference
      // |dy| is still in Y
      // replace X with ix
      // plot (ix,|dy|)
      if(!checkNaN && ix<NUMBERITERATIONS-1 && !checkzero) {
        convertDoubleToReal34RegisterPush((double)ix, REGISTER_X);
        if(getRegisterDataType(REGISTER_X) == dtReal34 && getRegisterDataType(REGISTER_Y) == dtReal34)  {
          runFunction(ITM_SIGMAPLUS);

                          #ifdef PC_BUILD
                            if(lastErrorCode == 24) { 
                              #ifdef VERBOSE_SOLVER1
                              printf("ERROR CODE CANNOT STAT COMPLEX ignored\n"); 
                              #endif //VERBOSE_SOLVER1
                              lastErrorCode = 0;
                            } else
                            if(lastErrorCode != 0) { 
                              #ifdef VERBOSE_SOLVER1
                              printf("ERROR CODE\n"); 
                              #endif //VERBOSE_SOLVER1
                              lastErrorCode = 0;
                            }
                          #endif
        }
      }

      #ifdef DMCP_BUILD
        lcd_refresh();
      #else // !DMCP_BUILD
        refreshLcd(NULL);
      #endif // DMCP_BUILD

    }


  //Iterations end

    uint16_t displayFormatDigitsN = displayFormatDigits;
    uint16_t displayFormatN = displayFormat;
    bool_t   FLAG_FRACTN;
    if (getSystemFlag(FLAG_FRACT)) FLAG_FRACTN = true; else FLAG_FRACTN = false;
    clearSystemFlag(FLAG_FRACT);
    displayFormat = DF_FIX;
    displayFormatDigits = 3;


    clearScreenPixels();
      #ifdef DMCP_BUILD
        lcd_refresh();
      #else // !DMCP_BUILD
        refreshLcd(NULL);
      #endif // DMCP_BUILD

    runFunction(ITM_CLSTK);
    fnStrtoX("Xo= ");
    fnRCL(SREG_STARTX0);
    runFunction(ITM_ADD);
    fnStrtoX(" -> ");
    runFunction(ITM_ADD);
    fnRCL(SREG_STARTX1);
    runFunction(ITM_ADD);
    fnStrtoX(", ");
    runFunction(ITM_ADD);

    displayFormat = DF_ALL;
    fnStrtoX("Itr=");
    convertDoubleToReal34RegisterPush(ix*1.0, REGISTER_X);
    runFunction(ITM_ADD);
    displayFormat = DF_FIX;
    fnStrtoX(";");
    runFunction(ITM_ADD);

    if(((ix >= NUMBERITERATIONS-1) && !checkzero) || checkNaN) {
   
      fnStrtoX(" Root not found. |d|=");
      runFunction(ITM_ADD);

      fnRCL(SREG_X1);
      fnRCL(SREG_X2);
      runFunction(ITM_SUB);
      runFunction(ITM_ABS);
      displayFormat = DF_SCI;
      runFunction(ITM_ADD);

    } else {
      fnStrtoX(" f(x)=");
      fnRCL(SREG_Y2);
      runFunction(ITM_ADD);
      fnStrtoX(" at x= ");
      displayFormat = DF_FIX;
      runFunction(ITM_ADD);
      runFunction(ITM_ADD);  
    }

    displayFormat = DF_ALL;
    fnRCL(SREG_X1);
    fnRCL(SREG_X2);
    
  //  if( ix * 1.0  >  NUMBERITERATIONS * 0.5) runFunction(ITM_PLOT_XY);

    if (FLAG_FRACTN) setSystemFlag(FLAG_FRACT) else clearSystemFlag(FLAG_FRACT);
    displayFormat = displayFormatN;
    displayFormatDigits = displayFormatDigitsN;

    SAVED_SIGMA_LAct = 0;   //prevent undo of last stats add action. REMOVE when STATS are not used anymore

  }
#endif




//-----------------------------------------------------//-----------------------------------------------------


void fnEqSolvGraph (uint16_t func) {
#ifndef TESTSUITE_BUILD
  hourGlassIconEnabled = true;
  showHideHourGlass();
  #ifdef DMCP_BUILD
    lcd_refresh();
  #else // !DMCP_BUILD
    refreshLcd(NULL);
  #endif // DMCP_BUILD

//  if(!(currentSolverStatus & SOLVER_STATUS_READY_TO_EXECUTE)) return;


  if(graphVariable >= FIRST_NAMED_VARIABLE && graphVariable <= LAST_NAMED_VARIABLE) {
    #if ((defined VERBOSE_SOLVER00) || (defined VERBOSE_SOLVER0)) && (defined PC_BUILD)
      printf("graphVariable accepted: %i\n", graphVariable);
    #endif
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "unexpected parameter %u", graphVariable);
      moreInfoOnError("In function fnEqSolvGraph:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  }


  //initialize x
  currentSolverStatus &= ~SOLVER_STATUS_READY_TO_EXECUTE;

  switch (func) {
     case EQ_SOLVE:{
            thereIsSomethingToUndo = false;
            saveForUndo();

            fnClDrawMx();
            statGraphReset();

            double ix1 = convertRegisterToDouble(REGISTER_X);
            double ix0 = convertRegisterToDouble(REGISTER_Y);
            #if ((defined VERBOSE_SOLVER00) || (defined VERBOSE_SOLVER0)) && (defined PC_BUILD)
              printRegisterToConsole(REGISTER_Y,">>> ix0=","");
              printRegisterToConsole(REGISTER_X," ix1=","\n");
            #endif
            calcRegister_t SREG_STARTX0 = __STARTX0;
            calcRegister_t SREG_STARTX1 = __STARTX1;
            copySourceRegisterToDestRegister(REGISTER_Y,SREG_STARTX0);
            copySourceRegisterToDestRegister(REGISTER_X,SREG_STARTX1);
            if(ix1>ix0 + 0.01 && ix1!=DOUBLE_NOT_INIT && ix0!=DOUBLE_NOT_INIT) { //pre-condition the plotter
              x_min = ix0;
              x_max = ix1;
            }
            #if ((defined VERBOSE_SOLVER00) || (defined VERBOSE_SOLVER0)) && (defined PC_BUILD)
              printf("xmin:%f, xmax:%f\n",x_min,x_max);
            #endif
            initialize_function();
            graph_solver();
            break;
          }
     case EQ_PLOT: {
            double ix1 = convertRegisterToDouble(REGISTER_X);
            double ix0 = convertRegisterToDouble(REGISTER_Y);
            #if ((defined VERBOSE_SOLVER00) || (defined VERBOSE_SOLVER0)) && (defined PC_BUILD)
              printRegisterToConsole(REGISTER_Y,">>> ix0=","");
              printRegisterToConsole(REGISTER_X," ix1=","\n");
            #endif

            
            #ifdef DEBUGUNDO
              printf(">>> saveForUndo from fnEqSolvGraph:");
            #endif //DEBUGUNDO
            
            thereIsSomethingToUndo = false;
            saveForUndo();

            fnClDrawMx();
            statGraphReset();

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
              if (fabs(x_min)<0.0001 || fabs(x_max)<0.0001) x_d = 10; //abort old values and show -1 to 1
              x_min = x_min - 0.1 * x_d;
              x_max = x_max + 0.1 * x_d;
            }
            #if ((defined VERBOSE_SOLVER00) || (defined VERBOSE_SOLVER0)) && (defined PC_BUILD)
              printf("xmin:%f, xmax:%f\n",x_min,x_max);
            #endif

            initialize_function();
            graph_eqn(0);
            break;
          }
     default:;
     }
#endif //TESTSUITE_BUILD
}








