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
 * \file normal.c
 ***********************************************/

#include "distributions/normal.h"

#include "constantPointers.h"
#include "error.h"
#include "fonts.h"
#include "mathematics/comparisonReals.h"
#include "mathematics/rsd.h"
#include "mathematics/wp34s.h"
#include "registers.h"
#include "registerValueConversions.h"

#include "wp43.h"

#if defined(SAVE_SPACE_DM42_16)
  void fnNormalP   (uint16_t unusedButMandatoryParameter){}
  void fnNormalL   (uint16_t unusedButMandatoryParameter){}
  void fnNormalR   (uint16_t unusedButMandatoryParameter){}
  void fnNormalI   (uint16_t unusedButMandatoryParameter){}
  void fnLogNormalP(uint16_t unusedButMandatoryParameter){}
  void fnLogNormalL(uint16_t unusedButMandatoryParameter){}
  void fnLogNormalR(uint16_t unusedButMandatoryParameter){}
  void fnLogNormalI(uint16_t unusedButMandatoryParameter){}
  void WP34S_Pdf_Q   (const real_t *x, real_t *res, realContext_t *realContext){}
  void WP34S_Cdfu_Q  (const real_t *x, real_t *res, realContext_t *realContext){}
  void WP34S_Cdf_Q   (const real_t *x, real_t *res, realContext_t *realContext){}
  void WP34S_qf_q_est(const real_t *x, real_t *res, real_t* resY, realContext_t *realContext){}
  void WP34S_Qf_Q    (const real_t *x, real_t *res, realContext_t *realContext){}
  void fnStdNormalP(uint16_t unusedButMandatoryParameter){}
  void fnStdNormalL(uint16_t unusedButMandatoryParameter){}
  void fnStdNormalR(uint16_t unusedButMandatoryParameter){}
  void fnStdNormalI(uint16_t unusedButMandatoryParameter){}
#else
  enum normalType {stdNormal, paramNormal, logNormal};

  #if(EXTRA_INFO_ON_CALC_ERROR == 1)
    #define loadParamNormal(v, reg, regName) loadParamNormal_(v, reg, regName)
    static bool_t loadParamNormal_(real_t *v, int reg, const char *regName) {
  #else
    #define loadParamNormal(v, reg, regName) loadParamNormal_(v, reg)
    static bool_t loadParamNormal_(real_t *v, int reg) {
  #endif
    if(getRegisterDataType(reg) == dtReal34) {
      real34ToReal(REGISTER_REAL34_DATA(reg), v);
    }
    else if(getRegisterDataType(reg) == dtLongInteger) { // long integer
      convertLongIntegerRegisterToReal(reg, v, &ctxtReal39);
    }
    else {
      displayDomainErrorMessage(ERROR_INVALID_DATA_TYPE_FOR_OP, ERR_REGISTER_LINE, REGISTER_X);
      #if(EXTRA_INFO_ON_CALC_ERROR == 1)
        sprintf(errorMessage, "Value in register %s must be of the real or long integer type", regName);
        moreInfoOnError("In function checkParamNormal:", errorMessage, NULL, NULL);
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


  static bool_t checkParamNormal(enum normalType type, real_t *x, real_t *i, real_t *j) {
    if(!loadParamNormal(x, REGISTER_X, "X")) {
      goto err;
    }
    if(type == stdNormal) {
      return true;
    }
    if(!loadParamNormal(i, REGISTER_I, "I") || !loadParamNormal(j, REGISTER_J, "J")) {
      goto err;
    }

    if(realIsZero(j) || realIsNegative(j)) {
      displayDomainErrorMessage(ERROR_INVALID_DISTRIBUTION_PARAM, ERR_REGISTER_LINE, REGISTER_X);
      #if(EXTRA_INFO_ON_CALC_ERROR == 1)
        moreInfoOnError("In function checkParamNormal:", "cannot calculate for " STD_sigma " " STD_LESS_EQUAL " 0", NULL, NULL);
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


  static void normalP(enum normalType type) {
    real_t val, alval, mu, sigma, ans;
    const int stdn = type == stdNormal;
    const int logn = type == logNormal;

    if(!saveLastX()) {
      return;
    }

    if(checkParamNormal(type, &val, &mu, &sigma)) {
      if(logn && realIsZero(&val)) {
        realZero(&ans);
      }
      else if(logn && realIsNegative(&val)) {
        displayDomainErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if(EXTRA_INFO_ON_CALC_ERROR == 1)
          moreInfoOnError("In function fnLogNormalP:", "cannot calculate for x < 0", NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      }
      else {
        if(!stdn) {
          if(logn) {
            realCopy(&val, &alval);
            WP34S_Ln(&alval, &val, &ctxtReal39);
          }
          realSubtract(&val, &mu, &val, &ctxtReal39);
          realDivide(&val, &sigma, &val, &ctxtReal39);
        }
        WP34S_Pdf_Q(&val, &ans, &ctxtReal39);
        if(logn) {
          realDivide(&ans, &sigma, &ans, &ctxtReal39);
          realDivide(&ans, &alval, &ans, &ctxtReal39);
        }
      }
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      convertRealToReal34ResultRegister(&ans, REGISTER_X);
    }

    adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  }


  static void normalL(enum normalType type) {
    real_t val, mu, sigma, ans;
    const int stdn = type == stdNormal;
    const int logn = type == logNormal;

    if(!saveLastX()) {
      return;
    }

    if(checkParamNormal(type, &val, &mu, &sigma)) {
      if(logn && realIsZero(&val)) {
        realZero(&ans);
      }
      else if(logn && realIsNegative(&val)) {
        displayDomainErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if(EXTRA_INFO_ON_CALC_ERROR == 1)
          moreInfoOnError("In function fnLogNormalP:", "cannot calculate for x < 0", NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      }
      else {
        if(!stdn) {
          if(logn) {
            WP34S_Ln(&val, &val, &ctxtReal39);
          }
          realSubtract(&val, &mu, &val, &ctxtReal39);
          realDivide(&val, &sigma, &val, &ctxtReal39);
        }
        WP34S_Cdf_Q(&val, &ans, &ctxtReal39);
      }
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      convertRealToReal34ResultRegister(&ans, REGISTER_X);
    }

    adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  }


  static void normalR(enum normalType type) {
    real_t val, mu, sigma, ans;
    const int stdn = type == stdNormal;
    const int logn = type == logNormal;

    if(!saveLastX()) {
      return;
    }

    if(checkParamNormal(type, &val, &mu, &sigma)) {
      if(logn && realIsZero(&val)) {
        realCopy(const_1, &ans);
      }
      else if(logn && realIsNegative(&val)) {
        displayDomainErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if(EXTRA_INFO_ON_CALC_ERROR == 1)
          moreInfoOnError("In function fnLogNormalP:", "cannot calculate for x < 0", NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
      }
      else {
        if(!stdn) {
          if(logn) {
            WP34S_Ln(&val, &val, &ctxtReal39);
          }
          realSubtract(&val, &mu, &val, &ctxtReal39);
          realDivide(&val, &sigma, &val, &ctxtReal39);
        }
        WP34S_Cdfu_Q(&val, &ans, &ctxtReal39);
      }
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      convertRealToReal34ResultRegister(&ans, REGISTER_X);
    }

    adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  }


  static void normalI(enum normalType type) {
    real_t val, mu, sigma, ans;
    const int stdn = type == stdNormal;
    const int logn = type == logNormal;

    if(!saveLastX()) {
      return;
    }

    if(checkParamNormal(type, &val, &mu, &sigma)) {
      if(realCompareLessEqual(&val, const_0) || realCompareGreaterEqual(&val, const_1)) {
        displayDomainErrorMessage(ERROR_ARG_EXCEEDS_FUNCTION_DOMAIN, ERR_REGISTER_LINE, REGISTER_X);
        #if(EXTRA_INFO_ON_CALC_ERROR == 1)
          moreInfoOnError("In function fnNormalI:", "the argument must be 0 < x < 1", NULL, NULL);
        #endif // (EXTRA_INFO_ON_CALC_ERROR == 1)
        if(getSystemFlag(FLAG_SPCRES)) {
          convertRealToReal34ResultRegister(const_NaN, REGISTER_X);
        }
        return;
      }
      WP34S_Qf_Q(&val, &ans, &ctxtReal39);
      if(!stdn) {
        realMultiply(&ans, &sigma, &ans, &ctxtReal39);
       realAdd(&ans, &mu, &ans, &ctxtReal39);
        if(logn) {
          realExp(&ans, &ans, &ctxtReal39);
        }
      }
      reallocateRegister(REGISTER_X, dtReal34, REAL34_SIZE, amNone);
      convertRealToReal34ResultRegister(&ans, REGISTER_X);
    }

    adjustResult(REGISTER_X, false, false, REGISTER_X, -1, -1);
  }



  void fnStdNormalP(uint16_t unusedButMandatoryParameter) {
    normalP(stdNormal);
  }

  void fnStdNormalL(uint16_t unusedButMandatoryParameter) {
    normalL(stdNormal);
  }

  void fnStdNormalR(uint16_t unusedButMandatoryParameter) {
    normalR(stdNormal);
  }

  void fnStdNormalI(uint16_t unusedButMandatoryParameter) {
    normalI(stdNormal);
  }



  void fnNormalP(uint16_t unusedButMandatoryParameter) {
    normalP(paramNormal);
  }

  void fnNormalL(uint16_t unusedButMandatoryParameter) {
    normalL(paramNormal);
  }

  void fnNormalR(uint16_t unusedButMandatoryParameter) {
    normalR(paramNormal);
  }

  void fnNormalI(uint16_t unusedButMandatoryParameter) {
    normalI(paramNormal);
  }



  void fnLogNormalP(uint16_t unusedButMandatoryParameter) {
    normalP(logNormal);
  }

  void fnLogNormalL(uint16_t unusedButMandatoryParameter) {
    normalL(logNormal);
  }

  void fnLogNormalR(uint16_t unusedButMandatoryParameter) {
    normalR(logNormal);
  }

  void fnLogNormalI(uint16_t unusedButMandatoryParameter) {
    normalI(logNormal);
  }



  /******************************************************
   * This functions are borrowed from the WP34S project
   ******************************************************/

  static void cdf_q(const real_t *x, real_t *res, realContext_t *realContext, bool_t upper) {
    real_t p;

    if(upper) {
      if(realIsNegative(x)) {
        goto cdfu_q_flip;
      }
      cdf_q_flip:
    realMultiply(x, x, res, realContext);
    realMultiply(res, const_1on2, res, realContext);
      WP34S_GammaP(res, const_1on2, res, realContext, true, false);
    realMultiply(res, const_1on2, res, realContext);
      realSquareRoot(const_pi, &p, realContext);
      realDivide(res, &p, res, realContext);
      return;
    }
    else {
      if(realIsNegative(x)) {
        goto cdf_q_flip;
      }
      cdfu_q_flip:
    realMultiply(x, x, res, realContext);
    realMultiply(res, const_1on2, res, realContext);
      WP34S_GammaP(res, const_1on2, res, realContext, false, false);
      realSquareRoot(const_pi, &p, realContext);
      realDivide(res, &p, res, realContext);
      realAdd(res, const_1, res, realContext);
    realMultiply(res, const_1on2, res, realContext);
      return;
    }
  }

  void WP34S_Pdf_Q(const real_t *x, real_t *res, realContext_t *realContext) {
    real_t p;
  realMultiply(x, x, res, realContext);
  realMultiply(res, const_1on2, res, realContext);
    realChangeSign(res);
   realExp(res, res, realContext);
   realSquareRoot(const_2pi, &p, realContext);
   realDivide(res, &p, res, realContext);
 }

  void WP34S_Cdfu_Q(const real_t *x, real_t *res, realContext_t *realContext) {
    cdf_q(x, res, realContext, true);
  }

  void WP34S_Cdf_Q(const real_t *x, real_t *res, realContext_t *realContext) {
    cdf_q(x, res, realContext, false);
  }

  /* This routine that returns a signed guess for the Normal quantile.
   * GNQ takes any 0 < p < 1 and returns a positive or negative estimate
   * for the Normal quantile.
   */
  void WP34S_qf_q_est(const real_t *x, real_t *res, real_t* resY, realContext_t *realContext) {
    real_t p, q, r, s;
    bool_t isSmall = false;

    // qf_q_int_est
    realMultiply(x, const__1, &p, realContext);
    realAdd(&p, const_1, &p, realContext);
    if(realCompareLessThan(x, &p)) {
      isSmall = true;
      realCopy(x, &p);
    }

  realMultiply(const_2, const_1on10, &q, realContext);

    if(realCompareLessThan(&p, &q)) {
      WP34S_Ln(&p, &q, realContext);
      realMultiply(&q, const_2, &q, realContext);
      realChangeSign(&q);
      realSubtract(&q, const_1, &r, realContext);
      realMultiply(&r, const_2pi, &r, realContext);
      realSquareRoot(&r, &r, realContext);
      realMultiply(&r, &p, &r, realContext);
      WP34S_Ln(&r, &r, realContext);
      realMultiply(&r, const_2, &r, realContext);
      realChangeSign(&r);
      realSquareRoot(&r, &r, realContext);
      int32ToReal(264, &s);
      realDivide(&s, const_1000, &s, realContext);
      realDivide(&s, &q, &q, realContext);
      realAdd(&q, &r, &q, realContext);
    }
    else { // qf_q_mid
      realMultiply(&p, const__1, &q, realContext);
      realAdd(&q, const_1on2, &q, realContext);
      realSquareRoot(const_2pi, &r, realContext);
      realMultiply(&q, &r, &q, realContext);
      realMultiply(&q, &q, &r, realContext);
      realMultiply(&r, &q, &r, realContext);
      realDivide(&r, const_5, &r, realContext);
      realAdd(&q, &r, &q, realContext);
    }

    realCopy(&q, res);
    if(resY) {
      realCopy(&p, resY);
    }
    // qf_q_signfix
    if(isSmall) {
      realChangeSign(res);
    }
  }

  void WP34S_Qf_Q(const real_t *x, real_t *res, realContext_t *realContext) {
    real_t p, q, r, s, reg0;
    bool_t half = false;
    int32_t loops;
    WP34S_qf_q_est(x, &p, &reg0, realContext);
    if(realIsNegative(&p)) {
      half = true;
    }
    loops = 2;
    realSetPositiveSign(&p);
    realAdd(&p, const_1, &q, realContext);
    roundToSignificantDigits(&q, &q, 3, realContext);
    if(realCompareEqual(&q, const_1)) {
      --loops;
      realMultiply(&p, const_1e8, &q, realContext);
      realMultiply(&q, const_1e8, &q, realContext);
      if(realCompareGreaterThan(&q, const_1)) {
        goto qf_q_calc;
      }
    }
    else {
      qf_q_calc:
      do { // qf_q_refine
        if(realCompareGreaterEqual(&p, const_1)) {
          WP34S_Cdfu_Q(&p, &q, realContext);
          realSubtract(&q, &reg0, &q, realContext);
        }
        else { // qf_q_small
          realMultiply(&p, &p, &q, realContext);
          realMultiply(&q, const_1on2, &q, realContext);
          WP34S_GammaP(&q, const_1on2, &r, realContext, false, true);
          realMultiply(&r, const_1on2, &q, realContext);
          realChangeSign(&q);
          realSubtract(const_1on2, &reg0, &r, realContext);
          realAdd(&q, &r, &q, realContext);
        }
        // qf_q_common
        WP34S_Pdf_Q(&p, &r, realContext);
        realDivide(&q, &r, &q, realContext);
        realMultiply(&q, &q, &r, realContext);
        realMultiply(&r, &q, &r, realContext);
        realMultiply(&p, &p, &s, realContext);
        realMultiply(&s, const_2, &s, realContext);
        realAdd(&s, const_1, &s, realContext);
        realMultiply(&r, &s, &r, realContext);
        int32ToReal(6, &s);
        realDivide(&r, &s, &r, realContext);
        realMultiply(&p, const_1on2, &s, realContext);
        realMultiply(&s, &q, &s, realContext);
        realMultiply(&s, &q, &s, realContext);
        realAdd(&s, &r, &r, realContext);
        realAdd(&r, &q, &q, realContext);
        realAdd(&q, &p, &p, realContext);
        // SHOW_CONVERGENCE
      } while(--loops > 0);
    }
    // qf_q_out
    if(half) {
      realChangeSign(&p);
    }
    realCopy(&p, res);
  }

#endif //SAVE_SPACE_DM42_16

