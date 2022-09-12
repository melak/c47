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

/********************************************//**
 * \file integrate.c
 ***********************************************/

#include "solver/integrate.h"

#include "constantPointers.h"
#include "defines.h"
#include "error.h"
#include "flags.h"
#include "items.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/wp34s.h"
#include "programming/lblGtoXeq.h"
#include "programming/manage.h"
#include "realType.h"
#include "registers.h"
#include "registerValueConversions.h"
#include "softmenus.h"
#include "solver/equation.h"
#include "stack.h"
#include "wp43s.h"

void fnPgmInt(uint16_t label) {
  if(label >= FIRST_LABEL && label <= LAST_LABEL) {
    currentSolverProgram = label - FIRST_LABEL;
  }
  else if(label >= REGISTER_X && label <= REGISTER_T) {
    // Interactive mode
    char buf[4];
    switch(label) {
      case REGISTER_X:        buf[0] = 'X'; break;
      case REGISTER_Y:        buf[0] = 'Y'; break;
      case REGISTER_Z:        buf[0] = 'Z'; break;
      case REGISTER_T:        buf[0] = 'T'; break;
      default: /* unlikely */ buf[0] = 0;
    }
    buf[1] = 0;
    label = findNamedLabel(buf);
    if(label == INVALID_VARIABLE) {
      displayCalcErrorMessage(ERROR_LABEL_NOT_FOUND, ERR_REGISTER_LINE, REGISTER_X);
      #if (EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "string '%s' is not a named label", buf);
        moreInfoOnError("In function fnPgmInt:", errorMessage, NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
    }
    else {
      currentSolverProgram = label - FIRST_LABEL;
    }
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "unexpected parameter %u", label);
      moreInfoOnError("In function fnPgmInt:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}

void fnIntegrate(uint16_t labelOrVariable) {
  if((labelOrVariable >= FIRST_LABEL && labelOrVariable <= LAST_LABEL) || (labelOrVariable >= REGISTER_X && labelOrVariable <= REGISTER_T)) {
    // Interactive mode
    fnPgmInt(labelOrVariable);
    if(lastErrorCode == ERROR_NONE) {
      currentSolverStatus = SOLVER_STATUS_INTERACTIVE | SOLVER_STATUS_EQUATION_INTEGRATE;
    }
  }
  else if(labelOrVariable == RESERVED_VARIABLE_ACC || labelOrVariable == RESERVED_VARIABLE_ULIM || labelOrVariable == RESERVED_VARIABLE_LLIM) {
    fnToReal(NOPARAM);
    if(lastErrorCode == ERROR_NONE) {
      real34Copy(REGISTER_REAL34_DATA(REGISTER_X), REGISTER_REAL34_DATA(labelOrVariable));
      switch(labelOrVariable) {
        case RESERVED_VARIABLE_ACC:  temporaryInformation = TI_ACC;  break;
        case RESERVED_VARIABLE_ULIM: temporaryInformation = TI_ULIM; break;
        case RESERVED_VARIABLE_LLIM: temporaryInformation = TI_LLIM; break;
      }
    }
  }
  else if(labelOrVariable >= FIRST_NAMED_VARIABLE && labelOrVariable <= LAST_NAMED_VARIABLE) {
    real_t acc, ulim, llim, res;
    real34ToReal(REGISTER_REAL34_DATA(RESERVED_VARIABLE_ACC),  &acc);
    real34ToReal(REGISTER_REAL34_DATA(RESERVED_VARIABLE_ULIM), &ulim);
    real34ToReal(REGISTER_REAL34_DATA(RESERVED_VARIABLE_LLIM), &llim);
    integrate(labelOrVariable, &llim, &ulim, &acc, &res, &ctxtReal39);
    liftStack();
    liftStack();
    reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
    reallocateRegister(REGISTER_Y, dtReal34, REAL34_SIZE, amNone);
    convertRealToReal34ResultRegister(&res, REGISTER_X);
    convertRealToReal34ResultRegister(&acc, REGISTER_Y);
    temporaryInformation = TI_INTEGRAL;
    adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  }
  else {
    displayCalcErrorMessage(ERROR_OUT_OF_RANGE, ERR_REGISTER_LINE, REGISTER_X);
    #if (EXTRA_INFO_ON_CALC_ERROR == 1)
      sprintf(errorMessage, "unexpected parameter %u", labelOrVariable);
      moreInfoOnError("In function fnIntegrate:", errorMessage, NULL, NULL);
    #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
  }
}

void fnIntVar(uint16_t unusedButMandatoryParameter) {
  #if !defined(TESTSUITE_BUILD)
    const char *var = (char *)getNthString(dynamicSoftmenu[softmenuStack[0].softmenuId].menuContent, dynamicMenuItem);
    const uint16_t regist = findOrAllocateNamedVariable(var);
    currentSolverVariable = regist;
    if(currentSolverStatus & SOLVER_STATUS_READY_TO_EXECUTE) {
      showSoftmenu(-MNU_Sfdx);
    }
    else {
      reallyRunFunction(ITM_STO, regist);
      currentSolverStatus |= SOLVER_STATUS_READY_TO_EXECUTE;
      temporaryInformation = TI_SOLVER_VARIABLE;
    }
  #endif // !TESTSUITE_BUILD
}



static void _integratorIteration(void) {
  if(currentSolverStatus & SOLVER_STATUS_USES_FORMULA) {
    parseEquation(currentFormula, EQUATION_PARSER_XEQ, tmpString, tmpString + AIM_BUFFER_LENGTH);
  }
  else {
    dynamicMenuItem = -1;
    execProgram(currentSolverProgram + FIRST_LABEL);
  }
}



// The following routine is ported from WP34s.
// The description below is as is. May not applicable for WP43s.


// Double Exponential Integration for the wp34s calculator
//
// v1.4x-412 (20180312) by M. César Rodríguez GGPL
//
// USAGE:
//  Inputs:
//    - integration limits in X (upper limit) and Y (lower limit)
//  Outputs:
//    - approximate value of integral in X
//    - estimated error in Y
//    - upper integration limit in Z
//    - lower integration limit in T
//    - updates L (with upper limit)
//    - in SSIZE8 mode, all ABCD will be corrupted
//
//  Remarks:
//    - accepts +Infinite and -Infinite as integration limits. If none
//      of the limits is infinite, the method applied is the tanh-sinh
//      one. If both are infinite then the sinh-sinh method is selected.
//      Otherwise the exp-sinh method is used
//    - the program tries to get all displayed digits correct, so
//      increasing the number of displayed digits will (hopefully) give
//      closer approximations (needing more time). But, in DBLON mode, the
//      program ignores the number of displayed digits and does its best
//      to get as much correct digits as possible
//    - during program execution the succesive aproximations are
//      displayed (as with the built-in integration program). Pressing
//      [<-] will interrupt the program & the last approximation
//      will be returned. The estimated error will be 0 in this case
//    - if the computed error estimation is not much smaller than the computed
//      result, it is assumed that all digits of the result are corrupted by
//      roundoff. In such cases, the reported result is 0 and the reported
//      error estimation equals the sum of the absolute values of the computed
//      result and error. This usually happens when the integral evaluates to 0
//    - if the user has set the D flag, many errors when evaluating the
//      integrand (say 0/0 or 1/0, overflows or ...) will not raise an error,
//      but return infinite or NaN instead. Such conditions will be detected
//      by this program that will continue the computation simply ignoring
//      the offending point
//    - keep in mind that, when both integration limits are infinite (or
//      one is infinite and the other 0) the integrand may be evaluated
//      at really bigs/small points (up to 1e±199 in DBLOFF mode and
//      1e±3088 in DBLON mode). Ensure that your integrand program
//      behaves as expected for such arguments (you may consider to set
//      flag D in some circumstances)
//    - the double exponential method relies on the function to be
//      integrated being analytic over the integration interval, except,
//      perhaps, at the interval ends. Thus, if it is known that the
//      integrand, _or any of its derivatives_, has a discontinuity at some
//      point inside the integration interval, it is advised to break the
//      interval at such point and compute two separate integrals, one
//      at each side of the problematic point. Better results will be
//      get this way. Thus, beware of abs, fp, ip and such non analytic
//      functions inside the integrand program. Other methods (Romberg)
//      may behave better in this respect, but, on the other side, the
//      double exponential method manages discontinuities (of the
//      integrand or its derivatives) at the interval edges better and
//      is usually way faster that the Romberg method
//    - as many other quadrature methods (Romberg included), the
//      double exponential algorithm does not manage well highly
//      oscillatory integrands. Here, highly oscillatory means that the
//      integrand changes sign many (or infinite) times along the
//      integration interval
//
//  Method used:
//    The algorith used here is adapted from that described in:
//      H. Takahasi and M. Mori. "Double Exponential Formulas for Numerical
//      Integration." Publications of RIMS, Kyoto University 9 (1974),
//      721-741.
//    Another useful reference may be:
//      David H. Bailey, Xiaoye S. Li and Karthik Jeyabalan, "A comparison
//      of three high-precision quadrature schemes," Experimental
//      Mathematics, vol. 14 (2005), no. 3, pg 317-329.
//
// 264 steps
// 16 local registers

static void DEI_xeq_user(calcRegister_t regist, const real_t *x, real_t *res, realContext_t *realContext) {
  // call user's function  -------------------------------
  if(!realIsSpecial(x)) { // abscissa is good?
    bool_t d = getSystemFlag(FLAG_SPCRES);
    clearSystemFlag(FLAG_SPCRES);
    reallocateRegister(regist, dtReal34, REAL34_SIZE, amNone);
    realToReal34(x, REGISTER_REAL34_DATA(regist));
    fnFillStack(NOPARAM);
    //printReal34ToConsole(REGISTER_REAL34_DATA(regist), "", " -> ");
    _integratorIteration();
    //printReal34ToConsole(REGISTER_REAL34_DATA(REGISTER_X), "", "\n"); fflush(stdout);
    real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), res);
    if(realIsSpecial(res)) { // do not stop in error (if flag D was set)
      realZero(res);
    }
    if(d) {
      setSystemFlag(FLAG_SPCRES); // set flag D for internal calculations
    }
  }
  else { // DEI_bad_absc::
    realZero(res);
  }
}

static void _integrate(calcRegister_t regist, const real_t *a, const real_t *b, real_t *acc, real_t *res, realContext_t *realContext) { // Double-Exponential Integration
  real_t bma2;            // (b - a)/2, a & b are the integration limits
  real_t bpa2;            // (b + 2)/2
  real_t eps;             // epsilon
  real_t thr;             // convergence threshold
  real_t lvl;             // level counter
  real_t tm;              // maximum abscissa in the t domain
  real_t h;               // space between abscissas in the t domain
  real_t ssp;             // partial sum at each level
  real_t j;               // abscissa counter
  real_t ch;              // cosh(t)
  real_t w;               // weight
  real_t rp;              // abscissa (r) or fplus + fminus (p)
  real_t ss;              // value of integral
  real_t ss1;             // previous ss

  real_t tmp, z, y, x;

  bool_t rev   = false;   // b is < a
  bool_t ES    = false;   // expsinh mode
  bool_t bpa2z = false;   // bpa2 is zero in sinhsinh or expsinh modes
  bool_t left  = false;   // expsinh case with -Infinite limit
  bool_t TS    = false;   // tanhsinh mode
  bool_t lg0   = false;   // true if level > 0

  // check arguments  ************************************
  if(realIsNaN(a) || realIsNaN(b)) { // check for invalid limits
    realCopy(const_NaN, res); // a or b is NaN, exit
    realCopy(const_NaN, acc);
    return;
  }
  if(realCompareEqual(a, b)) { // check for equal limits
    realZero(res); // a == b, return 0
    realZero(acc);
    return;
  }
  // set mode flags, bma2 and bpa2  **********************
  if(realCompareGreaterThan(a, b)) { // a > b?
    rev = true; // yes, flag it
  }
  if(realIsInfinite(a)) { // a == ±inf?
    ES = true; // yes, flag it (temporarily in ES)
  }
  if(realIsInfinite(b)) { // b == ±inf?
    bpa2z = true; // yes, flag it (temporarily in bpa2z)
  }
  // set the ES, bpa2z, left & TS flags according to the
  // a & b values ++++++++++++++++++++++++++++++++++++++++
  if(ES && bpa2z) { // skip if a != ±inf or b != ±inf
    // here in the sinhsinh case  --------------
    realZero(&y);
    realCopy(const_2, &x); // 2*bpa2 = 0, 2*bma2 = 2
    ES = false; // as this flag was dirty
    // [0 2 b a]
  } // done with sinhsinh
  //DEI_inf_1::
  else if(!(ES || bpa2z)) { // skip if a == ±inf or b == ±inf
    // here in the tanhsinh case  --------------
    realAdd(b, a, &y, realContext); // 2*bpa2 ready
    realSubtract(b, a, &x, realContext);
    realSetPositiveSign(&x); // 2*bma2 ready
    TS = true; // flag the tanhsinh case
    // [|b-a| a+b a a]
  } // done with tanhsinh
  //DEI_inf_2::
  else {
    // here in the expsinh case ----------------
    const real_t *aa = a, *bb = b;
    if(realIsInfinite(b)) {
      bb = a; aa = b; // now X is the finite limit
    }
    if(realCompareGreaterThan(bb, aa)) { // finite limit > infinite one?
      left = true; // yes, left case
    }
    realAdd(bb, bb, &y, realContext); // 2*bpa2 ready
    bpa2z = false; // as this flag was dirty
    if(realIsZero(bb)) { // finite limit == 0?
      bpa2z = true; // yes, flag it
    }
    ES = true; // flag the expsinh case
    realCopy(const_2, &x); // 2*bma2 ready
  }
  //DEI_ba2_rdy::
                                               // here with 2*bpa & 2*bma2 on stack
                                               // and mode flags ready
  realDivide(&x, const_2, &bma2, realContext); // save bma2
  realDivide(&y, const_2, &bpa2, realContext); // save bpa2
  // [bpa2 bma2]
  // done with mode flags, bpa2 and bma2  ****************
  // compute precision parameters ************************
  // [rewritten here for WP43S]
  // [1e-34 -34 bpa2 bma2]
  realDivide(acc, const_10, &eps, realContext);
  realMultiply(&eps, &eps, &eps, realContext);
  if(realCompareAbsLessThan(&eps, const_1e_32)) {
    realCopy(const_1e_32, &eps); // save epsilon = 10^-37
  }
  realCopy(acc, &thr); // save the convergence threshold [<- ACC]
  real34ToReal(const34_7, &lvl); // maxlevel = round(log2(digits)) + 2 [pre-calculated]
  if(bpa2z) { // the sinhsinh case and the expsinh case
              // when the finite limit == 0 can use a
              // smaller epsilon
    realNextToward(const_0, const_1, &eps, &ctxtReal34); // save epsilon for such cases
  }
  // precision parameters (epsilon, thr, level) ready ****
  // compute maximum t value (tm) ************************
  // tm is computed in such a way that all abscissas are
  // representable at the given precision and that the
  // integrand is never evaluated at the limits
  // DEI_tanhsinh::
  if(TS) {// tanhsinh mode?
    // start tm computation for tanh-sinh mode
    realCopy(realCompareGreaterThan(&bma2, const_1) ? const_1 : &bma2, &x);
    realAdd(&x, &x, &x, realContext); // X = 2*MIN(1, bma2)
  } // goto remaining tm calculation
  // DEI_ss_es::
  else if(!bpa2z) { // neither SS nor ES with 0 limit?
    // start tm computation for ES with != 0 limit
    realDivide(const_1on2, &bpa2, &x, realContext);
    realSetPositiveSign(&x); // X = ABS(1/(2*bpa2))
  } // goto remaining tm calculation
  // DEI_ss_es0::
  else {
    // start tm computation for SS & ES0 cases
    realSquareRoot(&eps, &x, realContext); // X = SQRT(eps)
  }
  // DEI_cont::
  realDivide(&x, &eps, &x, realContext); // continue tm computation for all cases
  WP34S_Ln(&x, &x, realContext);
  if(!TS) { // not in TS mode?
    realAdd(&x, &x, &x, realContext); // 2*ln(...) for all cases except TS
  }
  realAdd(&x, &x, &x, realContext); // 4*ln(...) for all cases except TS (2*LN(...))
  realDivide(&x, const_pi, &x, realContext);
  WP34S_Ln(&x, &tm, realContext); // tm done
  // maximum t (tm) ready ********************************
  // level loop ******************************************
  realZero(&ss); realZero(&ss1);
  realCopy(&ss, &z); // to let Z = ss (which here is 0)
  realCopy(const_2, &h); // h = 2
  do { // DEI_lvl_loop::
    realCopy(&z, &ss1); // update ss1
    realCopy(const_0, &ssp); // ssp = 0
    realCopy(const_1, &j); // j = 1
    realDivide(&h, const_2, &h, realContext); // h /= 2
    realCopy(&h, &x); // X = t
    // j loop ++++++++++++++++++++++++++++++++++++++++++++++
    // compute abscissas and weights  ----------------------
    do { // DEI_j_loop::
      WP34S_SinhCosh(&x, NULL, &x, realContext); // cosh(t) (cosh is much faster than sinh/tanh)
      realCopy(&x, &ch); // save for later
      realMultiply(&x, &x, &x, realContext);
      realSubtract(&x, const_1, &x, realContext);
      realSquareRoot(&x, &x, realContext);
      realMultiply(&x, const_piOn2, &x, realContext); // pi/2*sqrt(cosh(t)^2 - 1) = pi/2*sinh(t)
      if(ES) { // ES mode?
        realExp(&x, &x, realContext); // yes, want EXP
      }
      else {
        WP34S_SinhCosh(&x, NULL, &x, realContext); // no, want COSH
      }
      realCopy(&x, &y); // FILL
      if(TS) { // TS mode?
        realMultiply(&x, &x, &x, realContext); // end weight computation for TS mode
        realDivide(const_1, &x, &x, realContext);
      } // DEI_not_ts::
      realCopy(&x, &w); // save weight
      // stack filled with exp/cosh of pi/2*sinh(t)
      realCopy(&y, &x);
      if(!ES) { // ES mode?
        realMultiply(&x, &x, &x, realContext);      // no ES mode, get r (the abscissa in the
        realSubtract(&x, const_1, &x, realContext); // normalized domain)
        realSquareRoot(&x, &x, realContext);
      } // DEI_es_skip::
      if(TS) { // TS mode?
        realDivide(&x, &y, &x, realContext); // yes, adjust r
      }
      if(left) {// ES mode -infinity?
        realChangeSign(&x); // yes, adjust r
      }
      realCopy(&x, &rp); // save normalized abscissa
      // done with abscissas and weights  --------------------
      // evaluate integrand ----------------------------------
      realMultiply(&x, &bma2, &x, realContext); // r*(b - a)/2
      realAdd(&x, &bpa2, &x, realContext); // (b + a)/2 + r*(b - a)/2
      DEI_xeq_user(regist, &x, &x, realContext); // f(bpa2 + bma2*r)
      realMultiply(&x, &w, &x, realContext); // fplus*w
      realCopy(&x, &tmp); realCopy(&rp, &x); realCopy(&tmp, &rp); // p = fplus*w stored, r in X
      realCopy(&x, &z);
      // RCL bpa2    // (b + a)/2
      // RCL bma2    // (b - a)/2
      if(!ES) { // ES mode?
        realMultiply(&bma2, &z, &x, realContext); // no ES mode, "normal" abscissa
        realSubtract(&bpa2, &x, &x, realContext);
      }
      else { // DEI_es_mode::
        realDivide(&bma2, &z, &x, realContext); // ES mode, "inverse" abscissa
        realAdd(&bpa2, &x, &x, realContext); // X = (b + a)/2 ± (b - a)/2*(r or 1/r)
      } // DEI_call_usr_m::
      DEI_xeq_user(regist, &x, &x, realContext); // f(bpa2 ± bma2*(r or 1/r))
      if(ES) { // ES mode?
        realDivide(&x, &w, &x, realContext); // ES mode, inverse weight, fminus/w
      }
      else {
        realMultiply(&x, &w, &x, realContext); // no ES mode, normal weight, fminus*w
      }
      realAdd(&x, &rp, &x, realContext); // p = fplus*w + fminus*(w or 1/w)
      realMultiply(&x, &ch, &x, realContext); // X = ch*p
      if(realIsSpecial(&x)) {
        realZero(&x); // safety check for weight
      }
      realAdd(&ssp, &x, &ssp, realContext); // ssp += p
      // done with integrand  --------------------------------
      // level 0 is special ----------------------------------
      if(lg0) { // is level == 0?
        realAdd(&j, const_1, &j, realContext); // no, sweep odd abscissas, j += 1
        // early test (mainly) for the sinhsinh case
        realCopyAbs(&x, &y); // X = ABS(p)
        realMultiply(&ssp, &eps, &x, realContext);
        realSetPositiveSign(&x); // X = ABS(ssp*eps), Y = ABS(p)
        if(realCompareGreaterEqual(&x, &y)) { // ABS(p) <= ABS(eps*eps)?
          break;
        }
      }
      // if level > 0 done  ----------------------------------
      // DEI_updte_j::
      realAdd(&j, const_1, &j, realContext); // j += 1
      realMultiply(&j, &h, &x, realContext); // X = t = j*h
    } while(realCompareLessEqual(&x, &tm)); // t <= tm?
                                            // yes, continue j loop
    // done with j loop ++++++++++++++++++++++++++++++++++++
    // DEI_j_exit::
    realCopy(&ssp, &x); // no, update ss. X = ssp
    realAdd(&ss, &x, &ss, realContext); // ss += ssp
    if(!lg0) { // is level > 0 ----------------------------
      realCopy(&x, &z);
      realCopy(const_1, &y); // no, add 1st series term
      if(left) { // left?
        realChangeSign(&y);
      }
      realCopy(&bpa2, &x); // (b + a)/2
      if(ES) {// ES mode?
        realAdd(&y, &x, &x, realContext);
      }
      DEI_xeq_user(regist, &x, &x, realContext); // f(bpa2 (+/-1 in ES mode))
      realAdd(&ss, &x, &ss, realContext); // ss += f(bpa2)
    }
    // done with level 0  ----------------------------------
    // apply constant coeffs to sum ------------------------
    // DEI_chk_conv::
    realMultiply(&ss, &bma2, &x, realContext); // ss*bma2
    realMultiply(&x, &h, &x, realContext); // ss*bma2*h
    realMultiply(&x, const_piOn2, &x, realContext); // ss*bma2*h*pi/2
    if(rev) { // reverse?
      realChangeSign(&x); // yes,so change sign
    }
    // done with constant coeffs  --------------------------
    // show progress  --------------------------------------
    //          TOP?        // show value of approximation
    //            MSG MSG_INTEGRATE
    // check convergence  ----------------------------------
    realCopy(&x, &z);
    realAdd(&ssp, &ssp, &y, realContext); // 2*ssp
    realSubtract(&y, &ss, &y, realContext); // 2*ssp - ss
    realSetPositiveSign(&y); // ABS(2*ssp - ss)
    realMultiply(&thr, &ss, &x, realContext); // thr*ss
    realSetPositiveSign(&x); // ABS(thr*ss)
    if(realCompareGreaterThan(&x, &y)) { // ABS(2*ssp - ss) < ABS(thr*ss)?
      break; // yes, computation done
    }
    lg0 = true; // mark level 0 done,
    realSubtract(&lvl, const_1, &lvl, realContext); // update level &...
  } while(realCompareGreaterEqual(&lvl, const_0)); // loop.
  // DEI_fin::
  realCopy(&z, &x); // recall result
  // done with level loop ********************************
  // compute error estimation and check for bad results **
  // here with result in X
  // DEI_chk_bad::
  realCopyAbs(&x, &y); // stack: ss-|ss|-ss-?
  realSubtract(&x, &ss1, &x, realContext); // stack: (ss-ss1)-|ss|-ss-?
  realSetPositiveSign(&x); // stack: err-|ss|-ss-?
  realMultiply(&x, const_10, &tmp, realContext); // stack: 10*err-err-|ss|-ss
  if(realCompareGreaterEqual(&tmp, &y)) { // 10*err < |ss|?
    // [tmp x y z]
    // no, bad result. stack: |ss|-err-10*err-ss
    // [y x tmp z]
    realAdd(&x, &y, &y, realContext); // stack: new_err-10*err-ss-ss
    // [0 y tmp z]
    // stack: 0-new_err-10*err-ss
    realZero(&z); // stack: 0-new_err-10*err-0
  } // DEI_res_okay::
  realCopy(&z, &x); realCopy(&tmp, &y); // stack: 0-err-... or ss-err-...
  // done with bad results  ******************************
  //  exit  **********************************************
  // DEI_exit::
  // [rewritten here for WP43S]
  realCopy(&x, res);
  realCopy(&y, acc);
}

void integrate(calcRegister_t regist, const real_t *a, const real_t *b, real_t *acc, real_t *res, realContext_t *realContext) {
  bool_t was_solving = getSystemFlag(FLAG_SOLVING);
  ++currentSolverNestingDepth;
  setSystemFlag(FLAG_INTING);
  clearSystemFlag(FLAG_SOLVING);
  _integrate(regist, a, b, acc, res, realContext);
  if((--currentSolverNestingDepth) == 0) {
    clearSystemFlag(FLAG_INTING);
  }
  else if(was_solving) {
    clearSystemFlag(FLAG_INTING);
    setSystemFlag(FLAG_SOLVING);
  }
}
