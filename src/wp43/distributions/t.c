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
 * \file t.c
 ***********************************************/

#include "distributions/t.h"

#include "constantPointers.h"
#include "distributions/normal.h"
#include "error.h"
#include "fonts.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/wp34s.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43.h"

#if defined(SAVE_SPACE_DM42_15)
  void fnT_P   (uint16_t unusedButMandatoryParameter){}
  void fnT_L   (uint16_t unusedButMandatoryParameter){}
  void fnT_R   (uint16_t unusedButMandatoryParameter){}
  void fnT_I   (uint16_t unusedButMandatoryParameter){}
  void WP34S_Pdf_T (const real_t *x, const real_t *nu, real_t *res, realContext_t *realContext){}
  void WP34S_Cdfu_T(const real_t *x, const real_t *nu, real_t *res, realContext_t *realContext){}
  void WP34S_Cdf_T (const real_t *x, const real_t *nu, real_t *res, realContext_t *realContext){}
  void WP34S_Qf_T  (const real_t *x, const real_t *nu, real_t *res, realContext_t *realContext){}

#else
  static bool_t checkParamT(real_t *x, real_t *i) {
    if(   ((getRegisterDataType(REGISTER_X) != dtReal34) && (getRegisterDataType(REGISTER_X) != dtLongInteger))
       || ((getRegisterDataType(REGISTER_I) != dtReal34) && (getRegisterDataType(REGISTER_I) != dtLongInteger))) {
        displayDomainErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
        #if(EXTRA_INFO_ON_CALC_ERROR == 1)
          sprintf(errorMessage, "Values in register X and I must be of the real or long integer type");
          moreInfoOnError("In function checkParamT:", errorMessage, NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        goto err;
    }

    if(getRegisterDataType(REGISTER_X) == dtReal34) {
      real34ToReal(REGISTER_REAL34_DATA(REGISTER_X), x);
    }
    else { // long integer
      convertLongIntegerRegisterToReal(REGISTER_X, x, &ctxtReal39);
    }

    if(getRegisterDataType(REGISTER_I) == dtReal34) {
      real34ToReal(REGISTER_REAL34_DATA(REGISTER_I), i);
    }
    else {// long integer
      convertLongIntegerRegisterToReal(REGISTER_I, i, &ctxtReal39);
    }

    if(realIsZero(i) || realIsNegative(i)) {
      displayDomainErrorMessage(ERROR_INVALID_DISTRIBUTION_PARAM, ERR_REGISTER_LINE, REGISTER_X);
      #if(EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function checkParamT:", "cannot calculate for " STD_nu " " STD_LESS_EQUAL " 0", NULL, NULL);
      #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      goto err;
    }
    return true;

  err:
    if(getSystemFlag(FLAG_SPCRES)) {
      convertRealToReal34ResultRegister(const_NaN, REGISTER_X);
    }
    return false;
  }


  void fnT_P(uint16_t unusedButMandatoryParameter) {
    real_t val, ans, dof;

    if(!saveLastX()) {
      return;
    }

    if(checkParamT(&val, &dof)) {
      WP34S_Pdf_T(&val, &dof, &ans, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      convertRealToReal34ResultRegister(&ans, REGISTER_X);
    }

    adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  }


  void fnT_L(uint16_t unusedButMandatoryParameter) {
    real_t val, ans, dof;

    if(!saveLastX()) {
      return;
    }

    if(checkParamT(&val, &dof)) {
      WP34S_Cdf_T(&val, &dof, &ans, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      convertRealToReal34ResultRegister(&ans, REGISTER_X);
    }

    adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  }


  void fnT_R(uint16_t unusedButMandatoryParameter) {
    real_t val, ans, dof;

    if(!saveLastX()) {
      return;
    }

    if(checkParamT(&val, &dof)) {
      WP34S_Cdfu_T(&val, &dof, &ans, &ctxtReal39);
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      convertRealToReal34ResultRegister(&ans, REGISTER_X);
    }

    adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  }


  void fnT_I(uint16_t unusedButMandatoryParameter) {
    real_t val, ans, dof;

    if(!saveLastX()) {
      return;
    }

    if(checkParamT(&val, &dof)) {
      if(realCompareLessEqual(&val, const_0) || realCompareGreaterEqual(&val, const_1)) {
        displayDomainErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if(EXTRA_INFO_ON_CALC_ERROR == 1)
          moreInfoOnError("In function fnT_I:", "the argument must be 0 < x < 1", NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        if(getSystemFlag(FLAG_SPCRES)) {
          convertRealToReal34ResultRegister(const_NaN, REGISTER_X);
        }
        return;
      }
      WP34S_Qf_T(&val, &dof, &ans, &ctxtReal39);
      if(realIsNaN(&ans)) {
        displayDomainErrorMessage(ERROR_NO_ROOT_FOUND, ERR_REGISTER_LINE, REGISTER_X);
        #if(EXTRA_INFO_ON_CALC_ERROR == 1)
          moreInfoOnError("In function fnT_I:", "WP34S_Qf_T did not converge", NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      }
      else {
        reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
        convertRealToReal34ResultRegister(&ans, REGISTER_X);
      }
    }

    adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  }



  /******************************************************
   * This functions are borrowed from the WP34S project
   ******************************************************/

  void WP34S_Pdf_T(const real_t *x, const real_t *nu, real_t *res, realContext_t *realContext) {
    real_t p, q, r, s, i;

    realMultiply(x, x, &p, realContext);
    realMultiply(nu, const_1on2, &q, realContext);
    WP34S_LnGamma(&q, &r, realContext);
    realAdd(const_1on2, &q, &i, realContext);
    WP34S_LnGamma(&i, &s, realContext);
    realSubtract(&s, &r, &q, realContext);
    realDivide(&p, nu, &p, realContext);
    WP34S_Ln1P(&p, &p, realContext);
    realMultiply(&p, &i, &p, realContext);
    realSubtract(&q, &p, &p, realContext);
    realExp(&p, &p, realContext);
    realMultiply(const_pi, nu, &q, realContext);
    realSquareRoot(&q, &q, realContext);
    realDivide(&p, &q, res, realContext);
  }

  static void cdf_t(const real_t *x, const real_t *nu, real_t *res, realContext_t *realContext) {
    real_t p, q, r;
    bool_t invert = false;

    realCopy(x, &p);

    if(realIsInfinite(&p)) {
      // cdf_t_infinite
      realCopy(realIsNegative(&p) ? const_0 : const_1, res);
      return;
    }
    if(realIsZero(&p)) {
      // dist_ret_half
      realCopy(const_1on2, res);
      return;
    }
    if(realIsPositive(&p)) {
      // cdf_t_invert
      invert = true;
    }

    realMultiply(&p, &p, &p, realContext);
    if(realCompareGreaterEqual(&p, const_1)) {
      realAdd(&p, nu, &p, realContext);
      realDivide(nu, &p, &p, realContext);
      realMultiply(nu, const_1on2, &q, realContext);
      WP34S_betai(const_1on2, &q, &p, &r, realContext);
      realMultiply(&r, const_1on2, res, realContext);
    }
    else { // cdf_t_small
      realAdd(&p, nu, &q, realContext);
      realDivide(&p, &q, &p, realContext);
      realMultiply(nu, const_1on2, &q, realContext);
      WP34S_betai(&p, const_1on2, &q, &r, realContext);
      realMultiply(&r, const_1on2, &r, realContext);
      realChangeSign(&r);
      realAdd(&r, const_1on2, res, realContext);
    }
    // cdf_t_invert
    if(invert) {
      realSubtract(const_1, res, res, realContext);
    }
  }

  void WP34S_Cdfu_T(const real_t *x, const real_t *nu, real_t *res, realContext_t *realContext) {
    real_t xn;

    realMultiply(x, const__1, &xn, realContext);
    cdf_t(&xn, nu, res, realContext);
  }

  void WP34S_Cdf_T(const real_t *x, const real_t *nu, real_t *res, realContext_t *realContext) {
    cdf_t(x, nu, res, realContext);
  }

  void WP34S_Qf_T(const real_t *x, const real_t *nu, real_t *res, realContext_t *realContext) {
    real_t p, q, r, s, a, reg0;
    bool_t neg = false;
    int32_t loops;
    realSubtract(const_1, x, &p, realContext);
    if(realCompareLessThan(x, &p)) {
      neg = true;
      realCopy(x, &p);
    }
    realCopy(&p, &reg0);
    realSquareRoot(nu, &p, realContext);
    loops = 7;
    int32ToReal(7, &q);
    realAdd(&p, &q, &p, realContext);
    realMultiply(nu, const__1, &q, realContext);
    realPower(&p, &q, &p, realContext);
    realMultiply(&p, const_1on4, &a, realContext);
    if(realCompareLessEqual(&reg0, &a)) {
      realMultiply(nu, const_2, &p, realContext);
      realMultiply(&reg0, &p, &q, realContext);
      realSubtract(const_1on4, const_1, &r, realContext);
      realAdd(&p, &r, &p, realContext);
      realDivide(const_pi, &p, &p, realContext);
      realSquareRoot(&p, &p, realContext);
      realMultiply(&p, &q, &q, realContext);
      realDivide(const_1, nu, &r, realContext);
      realPower(&q, &r, &q, realContext);
      realSquareRoot(nu, &r, realContext);
      realDivide(&r, &q, &p, realContext);
    }
    else { // qf_t_tail
      WP34S_qf_q_est(&reg0, &p, NULL, realContext);
      realMultiply(&p, &p, &p, realContext);
      realMultiply(const_eE, nu, &r, realContext);
      realDivide(const_1, &r, &r, realContext);
      realAdd(&r, const_1, &r, realContext);
      realMultiply(&p, &r, &p, realContext);
      realDivide(&p, nu, &p, realContext);
      realExp(&p, &p, realContext);
      realSubtract(&p, const_1, &p, realContext);
      realMultiply(&p, nu, &p, realContext);
      realSquareRoot(&p, &p, realContext);
    }
    do { // qf_t_loop
      realMultiply(&p, &p, &q, realContext);
      if(realCompareGreaterEqual(&q, const_1)) {
        WP34S_Cdfu_T(&p, nu, &q, realContext);
        realSubtract(&q, &reg0, &q, realContext);
      }
      else { // qf_t_small
        realAdd(&q, nu, &r, realContext);
        realDivide(&q, &r, &q, realContext);
        realMultiply(nu, const_1on2, &r, realContext);
        WP34S_betai(&r, const_1on2, &q, &s, realContext);
        realMultiply(&s, const_1on2, &s, realContext);
        realChangeSign(&s);
        realSubtract(const_1on2, &reg0, &q, realContext);
        realAdd(&s, &q, &q, realContext);
      }
      // qf_t_step
      WP34S_Pdf_T(&p, nu, &r, realContext);
      realDivide(&q, &r, &q, realContext);
      realMultiply(&q, &r, &r, realContext);
      realAdd(nu, const_1, &s, realContext);
      realMultiply(&r, &s, &r, realContext);
      realMultiply(&p, &p, &s, realContext);
      realAdd(&s, nu, &s, realContext);
      realMultiply(&s, const_2, &s, realContext);
      realDivide(&r, &s, &r, realContext);
      realSubtract(&r, const_1, &r, realContext);
      realDivide(&q, &r, &q, realContext);
      realChangeSign(&q);
      realAdd(&p, &q, &q, realContext);
      //SHOW_CONVERGENCE
      realCopy(const_1, &r); r.exponent -= 32 /*14*/;
      if(WP34S_RelativeError(&q, &p, &r, realContext)) {
        realCopy(&q, res);
        goto qf_t_exit;
      }
      realCopy(&q, &p);
    } while(--loops > 0);

    realCopy(const_NaN, res); // ERR 20

    qf_t_exit:
    if(neg) {
      realChangeSign(res);
    }
  }

#endif //SAVE_SPACE_DM42_15

