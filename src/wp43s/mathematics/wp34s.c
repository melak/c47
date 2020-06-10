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
 * \file wp34s.c
 ***********************************************/

#include "wp43s.h"


/******************************************************
 * This functions are borrowed from the WP34S project
 ******************************************************/

// Have to be careful here to ensure that every function we call can handle
// the increased size of the numbers we're using.
void WP34S_Cvt2RadSinCosTan(const real_t *an, uint32_t angularMode, real_t *sinOut, real_t *cosOut, real_t *tanOut, realContext_t *realContext) {
  bool_t sinNeg = false, cosNeg = false, swap = false;
  real_t angle;

  if(realIsNaN(an)) {
    if(sinOut != NULL) {
     realCopy(const_NaN, sinOut);
    }
    if(cosOut != NULL) {
      realCopy(const_NaN, cosOut);
    }
    if(tanOut != NULL) {
      realCopy(const_NaN, tanOut);
    }
   return;
  }

  realCopy(an, &angle);

  if(angularMode == AM_DMS) {
    angularMode = AM_DEGREE;
    convertAngleFromTo(&angle, AM_DMS, AM_DEGREE, realContext);
  }

  // sin(-x) = -sin(x), cos(-x) = cos(x)
  if(realIsNegative(&angle)) {
    sinNeg = true;
    realSetPositiveSign(&angle);
  }

  switch(angularMode) {
    case AM_DEGREE:
      WP34S_Mod(&angle, const_360,     &angle, realContext); // mod(angle, 360°) --> angle
      break;

    case AM_GRAD:
      WP34S_Mod(&angle, const_400,     &angle, realContext); // mod(angle, 400g) --> angle
      break;

    case AM_RADIAN:
      WP34S_Mod(&angle, const1071_2pi, &angle, realContext); // mod(angle, 2pi) --> angle
      break;

    case AM_MULTPI:
      WP34S_Mod(&angle, const_2,       &angle, realContext); // mod(angle, 2) --> angle
      break;

    default: {}
  }

  // sin(180+x) = -sin(x), cos(180+x) = -cos(x)
  if(realCompareGreaterEqual(&angle, (real_t *)(angle180 + angularMode))) {        // angle >= 180°
    realSubtract(&angle, (real_t *)(angle180 + angularMode), &angle, realContext); // angle - 180° --> angle
    sinNeg = !sinNeg;
    cosNeg = !cosNeg;
  }

  // sin(90+x) = cos(x), cos(90+x) = -sin(x)
  if(realCompareGreaterEqual(&angle, (real_t *)(angle90 + angularMode))) {        // angle >= 90°
    realSubtract(&angle, (real_t *)(angle90 + angularMode), &angle, realContext); // angle - 90° --> angle
    swap = true;
    cosNeg = !cosNeg;
  }

  // sin(90-x) = cos(x), cos(90-x) = sin(x)
  if(realCompareEqual(&angle, (real_t *)(angle45 + angularMode))) { // angle == 45°
    if(sinOut != NULL) {
     realCopy(const_root2on2, sinOut);
    }
    if(cosOut != NULL) {
      realCopy(const_root2on2, cosOut);
    }
    if(tanOut != NULL) {
      realCopy(const_1, tanOut);
    }
  }
  else { // angle < 90
    if(realCompareGreaterThan(&angle, (real_t *)(angle45 + angularMode))) {         // angle > 45°
      realSubtract((real_t *)(angle90 + angularMode), &angle, &angle, realContext); // 90° - angle  --> angle
      swap = !swap;
    }

    convertAngleFromTo(&angle, angularMode, AM_RADIAN, realContext);
    WP34S_SinCosTanTaylor(&angle, swap, swap?cosOut:sinOut, swap?sinOut:cosOut, tanOut, realContext); // angle in radian
  }

  if(sinOut != NULL) {
    if(sinNeg) {
      realSetNegativeSign(sinOut);
      if(tanOut != NULL) {
        realSetNegativeSign(tanOut);
      }
    }
    if(realIsZero(sinOut)) {
      realSetPositiveSign(sinOut);
      if(tanOut != NULL) {
        realSetPositiveSign(tanOut);
      }
    }
    realPlus(sinOut, sinOut, realContext);
  }

  if(cosOut != NULL) {
    if(cosNeg) {
      realSetNegativeSign(cosOut);
      if(tanOut != NULL) {
        realChangeSign(tanOut);
      }
    }
    if(realIsZero(cosOut)) {
      realSetPositiveSign(cosOut);
    }
    realPlus(cosOut, cosOut, realContext);
  }

  if(tanOut != NULL && realIsZero(cosOut)) {
    realSetPositiveSign(tanOut);
    realPlus(tanOut, tanOut, realContext);
  }
}


// Calculate sin, cos by Taylor series and tan by division
void WP34S_SinCosTanTaylor(const real_t *a, bool_t swap, real_t *sinOut, real_t *cosOut, real_t *tanOut, realContext_t *realContext) { // a in radian
  real_t angle, a2, t, j, z, sin, cos, compare;
  int i, odd;
  bool_t endSin = (sinOut == NULL), endCos = (cosOut == NULL);
  int32_t cmp, savedContextDigits;

  savedContextDigits = realContext->digits;
  if(realContext->digits > 51) {
    realContext->digits = 75;
  }
  else {
    realContext->digits = 51;
  }

  realCopy(a, &angle);
  realMultiply(&angle, &angle, &a2, realContext);
  uInt32ToReal(1, &j);
  uInt32ToReal(1, &t);
  uInt32ToReal(1, &sin);
  uInt32ToReal(1, &cos);

  for(i=1; !(endSin && endCos) && i<1000; i++) { // i goes up to 31 max in the test suite
    odd = i & 1;

    realAdd(&j, const_1, &j, realContext);
    realDivide(&a2, &j, &z, realContext);
    realMultiply(&t, &z, &t, realContext);

    if(!endCos) {
      realCopy(&cos, &z);

      if(odd) {
        realSubtract(&cos, &t, &cos, realContext);
      }
      else {
        realAdd(&cos, &t, &cos, realContext);
      }

      realCompare(&cos, &z, &compare, realContext);
      realToInt32(&compare, cmp);
      endCos = (cmp == 0);
    }

    realAdd(&j, const_1, &j, realContext);
    realDivide(&t, &j, &t, realContext);

    if(!endSin) {
      realCopy(&sin, &z);

      if(odd) {
        realSubtract(&sin, &t, &sin, realContext);
      }
      else {
        realAdd(&sin, &t, &sin, realContext);
      }

      realCompare(&sin, &z, &compare, realContext);
      realToInt32(&compare, cmp);
      endSin = (cmp == 0);
    }
  }

  if(realIsZero(&cos)) {
    realSetPositiveSign(&cos);
  }

  if(realIsZero(&sin)) {
    realSetPositiveSign(&sin);
  }

  realMultiply(&sin, &angle, &sin, realContext);

  realContext->digits = savedContextDigits;

  if(sinOut != NULL) {
    realPlus(&sin, sinOut, realContext);
  }

  if(cosOut != NULL) {
    realPlus(&cos, cosOut, realContext);
  }

  if(tanOut != NULL) {
    if(sinOut == NULL || cosOut == NULL) {
      realCopy(const_NaN, tanOut);
    }
    else {
      if(swap) {
        realDivide(&cos, &sin, tanOut, realContext);
      }
      else {
        realDivide(&sin, &cos, tanOut, realContext);
      }
    }
  }
}



void WP34S_Atan(const real_t *x, real_t *angle, realContext_t *realContext) {
  real_t a, b, a2, t, j, z, last;
  int doubles = 0;
  int invert;
  int n;
  int neg = realIsNegative(x);

  realCopy(x, &a);

  // arrange for a >= 0
  if(neg) {
    realChangeSign(&a);
  }

  // reduce range to 0 <= a < 1, using atan(x) = pi/2 - atan(1/x)
  invert = realCompareGreaterThan(&a, const_1);
  if(invert) {
    realDivide(const_1, &a, &a, realContext);
  }

  // Range reduce to small enough limit to use taylor series using:
  //  tan(x/2) = tan(x)/(1+sqrt(1+tan(x)²))
  for(n=0; n<1000; n++) {
    if(realCompareLessEqual(&a, const_1on10)) {
      break;
    }
    doubles++;
    // a = a/(1+sqrt(1+a²)) -- at most 3 iterations.
    realMultiply(&a, &a, &b, realContext);
    realAdd(&b, const_1, &b, realContext);
    realSquareRoot(&b, &b, realContext);
    realAdd(&b, const_1, &b, realContext);
    realDivide(&a, &b, &a, realContext);
  }

  // Now Taylor series
  // atan(x) = x(1-x²/3+x⁴/5-x⁶/7...)
  // We calculate pairs of terms and stop when the estimate doesn't change
  realCopy(const_3, angle);
  realCopy(const_5, &j);
  realMultiply(&a, &a, &a2, realContext); // a²
  realCopy(&a2, &t);
  realDivide(&t, angle, angle, realContext); // s = 1-t/3 -- first two terms
  realSubtract(const_1, angle, angle, realContext);

  do { // Loop until there is no digits changed
    realCopy(angle, &last);

    realMultiply(&t, &a2, &t, realContext);
    realDivide(&t, &j, &z, realContext);
    realAdd(angle, &z, angle, realContext);

    realAdd(&j, const_2, &j, realContext);

    realMultiply(&t, &a2, &t, realContext);
    realDivide(&t, &j, &z, realContext);
    realSubtract(angle, &z, angle, realContext);

    realAdd(&j, const_2, &j, realContext);

  } while(!realCompareEqual(angle, &last));

  realMultiply(angle, &a, angle, realContext);

  while(doubles) {
    realAdd(angle, angle, angle, realContext);
    doubles--;
  }

  if(invert) {
    realSubtract(const_piOn2, angle, angle, realContext);
  }

  if(neg) {
    realChangeSign(angle);
  }
}



void WP34S_Atan2(const real_t *y, const real_t *x, real_t *atan, realContext_t *realContext) {
  real_t r, t;
  const bool_t xNeg = realIsNegative(x);
  const bool_t yNeg = realIsNegative(y);

  if(realIsNaN(x) || realIsNaN(y)) {
    realCopy(const_NaN, atan);
    return;
  }

  if(realCompareEqual(y, const_0)) {
    if(yNeg) {
      if(realCompareEqual(x, const_0)) {
        if(xNeg) {
          realMinus(const_pi, atan, realContext);
        }
        else {
          realCopy(y, atan);
        }
      }
      else if(xNeg) {
        realMinus(const_pi, atan, realContext);
      }
      else {
        realCopy(y, atan);
      }
    }
    else {
      if(realCompareEqual(x, const_0)) {
        if(xNeg) {
          realCopy(const_pi, atan);
        }
        else {
          realZero(atan);
        }
      }
      else if(xNeg) {
        realCopy(const_pi, atan);
      }
      else {
        realZero(atan);
      }
    }
    return;
  }

  if(realCompareEqual(x, const_0)) {
    realCopy(const_piOn2, atan);
    if(yNeg) {
      realSetNegativeSign(atan);
    }
    return;
  }

  if(realIsInfinite(x)) {
    if(xNeg) {
      if(realIsInfinite(y)) {
        realCopy(const_3piOn4, atan);
        if(yNeg) {
          realSetNegativeSign(atan);
        }
      }
      else {
        realCopy(const_pi, atan);
        if(yNeg) {
          realSetNegativeSign(atan);
        }
      }
    }
    else {
      if(realIsInfinite(y)) {
        realCopy(const_piOn4, atan);
        if(yNeg) {
          realSetNegativeSign(atan);
        }
      }
      else {
        realZero(atan);
        if(yNeg) {
          realSetNegativeSign(atan);
        }
      }
    }
    return;
  }

  if(realIsInfinite(y)) {
    realCopy(const_piOn2, atan);
    if(yNeg) {
      realSetNegativeSign(atan);
    }
    return;
  }

  realDivide(y, x, &t, realContext);
  WP34S_Atan(&t, &r, realContext);
  if(xNeg) {
    realCopy(const_pi, &t);
    if(yNeg) {
     realSetNegativeSign(&t);
    }
  }
  else {
    realZero(&t);
  }

  realAdd(&r, &t, atan, realContext);
  if(realCompareEqual(atan, const_0) && yNeg) {
    realSetNegativeSign(atan);
  }
}



void WP34S_Asin(const real_t *x, real_t *angle, realContext_t *realContext) {
  real_t abx, z;

  if(realIsNaN(x)) {
    realCopy(const_NaN, angle);
    return;
  }

  realCopyAbs(x, &abx);
  if(realCompareGreaterThan(&abx, const_1)) {
    realCopy(const_NaN, angle);
    return;
  }

  // angle = 2*atan(x/(1+sqrt(1-x*x)))
  realMultiply(x, x, &z, realContext);
  realSubtract(const_1, &z, &z, realContext);
  realSquareRoot(&z, &z, realContext);
  realAdd(&z, const_1, &z, realContext);
  realDivide(x, &z, &z, realContext);
  WP34S_Atan(&z, &abx, realContext);
  realAdd(&abx, &abx, angle, realContext);
}



void WP34S_Acos(const real_t *x, real_t *angle, realContext_t *realContext) {
  real_t abx, z;

  if(realIsNaN(x)) {
    realCopy(const_NaN, angle);
    return;
  }

  realCopyAbs(x, &abx);
  if(realCompareGreaterThan(&abx, const_1)) {
    realCopy(const_NaN, angle);
    return;
  }

  // angle = 2*atan((1-x)/sqrt(1-x*x))
  if(realCompareEqual(x, const_1)) {
    realZero(angle);
  }
  else {
    realMultiply(x, x, &z, realContext);
    realSubtract(const_1, &z, &z, realContext);
    realSquareRoot(&z, &z, realContext);
    realSubtract(const_1, x, &abx, realContext);
    realDivide(&abx, &z, &z, realContext);
    WP34S_Atan(&z, &abx, realContext);
    realAdd(&abx, &abx, angle, realContext);
  }
}



static void WP34S_Calc_Gamma_LnGamma_Lanczos(const real_t *xin, real_t *res, bool_t calculateLnGamma, realContext_t *realContext) {
  real_t r, s, t, u, v, x;
  int32_t k, savedContextDigits;

  savedContextDigits = realContext->digits;
  if(realContext->digits < 51) {
    realContext->digits = 51;
  }

  realSubtract(xin, const_1, &x, realContext);
  realZero(&s);
  realAdd(&x, const_29, &t, realContext);
  for(k=28; k>=0; k--) {
    realDivide((real_t *)(gammaLanczosCoefficients + k), &t, &u, realContext);
    realSubtract(&t, const_1, &t, realContext);
    realAdd(&s, &u, &s, realContext);
  }

  realAdd(&s, const_gammaC00, &t, realContext);
  WP34S_Ln(&t, &s, realContext);

  //  r = z + g + 0.5;
  realAdd(&x, const_gammaR, &r, realContext); // const_gammaR is g + 0.5

  //  r = log(R[0][0]) + (z+0.5) * log(r) - r;
  WP34S_Ln(&r, &u, realContext);
  realAdd(&x, const_1on2, &t, realContext);
  realMultiply(&u, &t, &v, realContext);

  realSubtract(&v, &r, &u, realContext);

  if(calculateLnGamma) {
    realAdd(&u, &s, &x, realContext);
  }
  else {
    realAdd(&u, &s, &x, realContext);
    realExp(&x, &x, realContext);
  }

  realContext->digits = savedContextDigits;

  realPlus(&x, res, realContext);
}


// common code for the [GAMMA] and LN[GAMMA]
static void WP34S_Gamma_LnGamma(const real_t *xin, const bool_t calculateLnGamma, real_t *res, realContext_t *realContext) {
  real_t x, t;
  bool_t reflect = false;

  // Check for special cases
  if(realIsSpecial(xin)) {
    if(realIsInfinite(xin) && realIsPositive(xin)) {
      realCopy(const_plusInfinity, res);
      return;
    }

    realCopy(const_NaN, res);
    return;
  }

  // Handle x approximately zero case
  if(realCompareAbsLessThan(xin, const_1e_24)) {
    if(realIsZero(xin)) {
      realCopy(const_NaN, res);
      return;
    }
    realDivide(const_1, xin, &x, realContext);
    realSubtract(&x, const_egamma, res, realContext);
    if(calculateLnGamma) {
      WP34S_Ln(res, res, realContext);
    }
    return;
  }

  // Correct our argument and begin the inversion if it is negative
  if(realCompareLessEqual(xin, const_0)) {
    reflect = true;
    realSubtract(const_1, xin, &t, realContext); // t = 1 - xin
    if(realIsAnInteger(&t)) {
      realCopy(const_NaN, res);
      return;
    }
  }
  else {
    // Provide a fast path evaluation for positive integer arguments that aren't too large
    // The threshold for overflow is 205! (i.e. 204! is within range and 205! isn't).
    if(realIsAnInteger(xin) && realCompareLessEqual(xin, const_205)) {
      realSubtract(xin, const_1, &x, realContext); // x = xin - 1
      realCopy(const_1, res);
      while(realCompareGreaterEqual(&x, const_2)) {
        realMultiply(res, &x, res, realContext);
        realSubtract(&x, const_1, &x, realContext);
      }
      if(calculateLnGamma) {
        WP34S_Ln(res, res, realContext);
      }
      return;
    }
    realCopy(xin, &t); // t = xin
  }

  WP34S_Calc_Gamma_LnGamma_Lanczos(&t, res, calculateLnGamma, realContext); // t=1-xin or t=xin and res=gamma(xin) or res=lngamma(xin) or res=gamma(1-xin) or res=lngamma(1-xin)

  if(reflect) {
    // figure out xin * PI mod 2PI
    WP34S_Mod(xin, const_2, &t, realContext);
    realMultiply(&t, const_pi, &t, realContext);                   // t = xin·pi
    WP34S_SinCosTanTaylor(&t, false, &x, NULL, NULL, realContext); // x = sin(xin·pi)

    if(calculateLnGamma) {
      realDivide(const_pi, &x, &t, realContext);                   // t = pi / sin(pi·xin)
      WP34S_Ln(&t, &t, realContext);                               // t = ln(pi / sin(pi·xin))
      realSubtract(&t, res, res, realContext);                     // res = ln(pi / sin(pi·xin)) - lngamma(1-xin)
    }
    else {
      realMultiply(&x, res, &t, realContext);                      // t = sin(pi·xin) × gamma(1-xin)
      realDivide(const_pi, &t, res, realContext);                  // res = pi / (sin(pi·xin)·gamma(1-xin))
    }
  }
}



void WP34S_Factorial(const real_t *xin, real_t *res, realContext_t *realContext) {
  real_t x;

  realAdd(xin, const_1, &x, realContext);
  WP34S_Gamma_LnGamma(&x, false, res, realContext);
}



void WP34S_Gamma(const real_t *xin, real_t *res, realContext_t *realContext) {
  real_t x;

  realCopy(xin, &x);
  WP34S_Gamma_LnGamma(&x, false, res, realContext);
}



void WP34S_LnGamma(const real_t *xin, real_t *res, realContext_t *realContext) {
  real_t x;

  realCopy(xin, &x);
  WP34S_Gamma_LnGamma(&x, true, res, realContext);
}



/* Natural logarithm.
 *
 * Take advantage of the fact that we store our numbers in the form: m * 10^e
 * so log(m * 10^e) = log(m) + e * log(10)
 * do this so that m is always in the range 0.1 <= m < 2.  However if the number
 * is already in the range 0.5 .. 1.5, this step is skipped.
 *
 * Then use the fact that ln(x²) = 2 * ln(x) to range reduce the mantissa
 * into 1/sqrt(2) <= m < 2.
 *
 * Finally, apply the series expansion:
 *   ln(x) = 2(a+a³/3+a⁵/5+...) where a=(x-1)/(x+1)
 * which converges quickly for an argument near unity.
 */
void WP34S_Ln(const real_t *xin, real_t *res, realContext_t *realContext) {
  real_t z, t, f, n, m, i, v, w, e;
  int32_t expon;

  if(realIsSpecial(xin)) {
    if(realIsNaN(xin) || realIsNegative(xin)) {
      realCopy(const_NaN, res);
      return;
    }

    realCopy(const_plusInfinity, res);
    return;
  }

  if(realCompareLessEqual(xin, const_0)) {
    if(realIsNegative(xin)) {
      realCopy(const_NaN, res);
      return;
    }

    realCopy(const_minusInfinity, res);
    return;
  }

  realCopy(xin, &z);
  realCopy(const_2, &f);
  realSubtract(xin, const_1, &t, realContext);
  realCopy(&t, &v);
  realSetPositiveSign(&v);
  if(realCompareGreaterThan(&v, const_1on2)) {
    expon = z.exponent + z.digits;
    z.exponent = -z.digits;
  }
  else {
    expon = 0;
  }

  /* The too high case never happens
  while(dn_le(const_2, &z)) {
    realMultiply(&f, const_2, &f, realContext);
    realSquareRoot(&z, &z, realContext);
  } */

  // Range reduce the value by repeated square roots.
  // Making the constant here larger will reduce the number of later
  // iterations at the expense of more square root operations.
  while(realCompareLessEqual(&z, const_root2on2)) {
    realMultiply(&f, const_2, &f, realContext);
    realSquareRoot(&z, &z, realContext);
  }

  realAdd(&z, const_1, &t, realContext);
  realSubtract(&z, const_1, &v, realContext);
  realDivide(&v, &t, &n, realContext);
  realCopy(&n, &v);
  realMultiply(&v, &v, &m, realContext);
  realCopy(const_3, &i);

  int32ToReal(1 - realContext->digits, &t); // t is the exponent
  realPower(const_10, &t, &z, realContext); // z is the max error

  for(;;) {
    realMultiply(&m, &n, &n, realContext);
    realDivide(&n, &i, &e, realContext);
    realAdd(&v, &e, &w, realContext);
    if(WP34S_RelativeError(&w, &v, &z, realContext)) {
      break;
    }
    realCopy(&w, &v);
    realAdd(&i, const_2, &i, realContext);
  }

  realMultiply(&f, &w, res, realContext);
  if(expon == 0) {
    return;
  }

  int32ToReal(expon, &e);
  realMultiply(&e, const_ln10, &w, realContext);
  realAdd(res, &w, res, realContext);
}



void WP34S_Log(const real_t *xin, const real_t *base, real_t *res, realContext_t *realContext) {
  real_t y;

  if(realIsSpecial(xin)) {
    if(realIsNaN(xin) || realIsNegative(xin)) {
      realCopy(const_NaN, res);
      return;
    }

    realCopy(const_plusInfinity, res);
    return;
  }

  WP34S_Ln(xin, &y, realContext);

  realDivide(&y, base, res, realContext);
}



void WP34S_Log2(const real_t *xin, real_t *res, realContext_t *realContext) {
  WP34S_Log(xin, const_ln2, res, realContext);
}



void WP34S_Log10(const real_t *xin, real_t *res, realContext_t *realContext) {
  WP34S_Log(xin, const_ln10, res, realContext);
}



void WP34S_Logxy(const real_t *yin, const real_t *xin, real_t *res, realContext_t *realContext) {
  real_t lx;

  WP34S_Ln(xin, &lx, realContext);
  WP34S_Log(yin, &lx, res, realContext);
}



bool_t WP34S_RelativeError(const real_t *x, const real_t *y, const real_t *tol, realContext_t *realContext) {
  real_t a;

  if(realCompareEqual(x, const_0)) {
    realCopyAbs(y, &a);
    return realCompareLessThan(&a, tol);
  }

  realSubtract(x, y, &a, realContext);
  realDivide(&a, x, &a, realContext);
  realSetPositiveSign(&a);
  return realCompareLessThan(&a, tol);
}


/* Hyperbolic functions.
 * We start with a utility routine that calculates sinh and cosh.
 * We do the sihn as (e^x - 1) (e^x + 1) / (2 e^x) for numerical stability
 * reasons if the value of x is smallish.
 */
void WP34S_SinhCosh(const real_t *x, real_t *sinhOut, real_t *coshOut, realContext_t *realContext) {
  real_t t, u, v;

  if(realIsNaN(x)) {
    if(sinhOut != NULL) {
      realCopy(const_NaN, sinhOut);
    }
    if(coshOut != NULL) {
      realCopy(const_NaN, coshOut);
    }
    return;
  }

  if(sinhOut != NULL) {
    if(realCompareAbsLessThan(x, const_1on2)) {
      WP34S_ExpM1(x, &u, realContext);                         // u = e^x - 1
      realMultiply(&u, const_1on2, &t, realContext);           // t = (e^x - 1) / 2

      realAdd(&u, const_1, &u, realContext);                   // u = e^x
      realDivide(&t, &u, &v, realContext);                     // v = (e^x - 1) / 2e^x

      realAdd(&u, const_1, &u, realContext);                   // u = e^x + 1
      realMultiply(&u, &v, sinhOut, realContext);              // sinhOut = (e^x - 1)(e^x + 1) / 2e^x
    }
    else {
      realExp(x, &u, realContext);                             // u = e^x
      realDivide(const_1, &u, &v, realContext);                // v = e^-x
      realSubtract(&u, &v, sinhOut, realContext);              // sinhOut = (e^x + e^-x)
      realMultiply(sinhOut, const_1on2, sinhOut, realContext); // sinhOut = (e^x + e^-x)/2
    }
  }
  if(coshOut != NULL) {
   realExp(x, &u, realContext);                                // u = e^x
   realDivide(const_1, &u, &v, realContext);                   // v = e^-x
   realAdd(&u, &v, coshOut, realContext);                      // coshOut = (e^x + e^-x)
   realMultiply(coshOut, const_1on2, coshOut, realContext);    // coshOut = (e^x + e^-x)/2
  }
}



void WP34S_Tanh(const real_t *x, real_t *res, realContext_t *realContext) {
  if(realIsNaN(x)) {
    realCopy(const_NaN, res);
  }
  else if(realCompareAbsGreaterThan(x, const_47)) { // equals 1 to 39 digits
    realCopy((realIsPositive(x) ? const_1 : const__1), res);
  }
  else {
    real_t a, b;

    realAdd(x, x, &a, realContext);        // a = 2x
    WP34S_ExpM1(&a, &b, realContext);      // b = exp(2x) - 1
    realAdd(&b, const_2, &a, realContext); // a = exp(2x) - 1 + 2 = exp(2x) + 1
    realDivide(&b, &a, res, realContext);  // res = (exp(2x) - 1) / (exp(2x) + 1)
  }
}



void WP34S_ArcSinh(const real_t *x, real_t *res, realContext_t *realContext) {
  real_t a;

  realMultiply(x, x, &a, realContext);   // a = x²
  realAdd(&a, const_1, &a, realContext); // a = x² + 1
  realSquareRoot(&a, &a, realContext);   // a = sqrt(x²+1)
  realAdd(&a, const_1, &a, realContext); // a = sqrt(x²+1)+1
  realDivide(x, &a, &a, realContext);    // a = x / (sqrt(x²+1)+1)
  realAdd(&a, const_1, &a, realContext); // a = x / (sqrt(x²+1)+1) + 1
  realMultiply(x, &a, &a, realContext);  // y = x * (x / (sqrt(x²+1)+1) + 1)
  WP34S_Ln1P(&a, res, realContext);      // res = ln(1 + (x * (x / (sqrt(x²+1)+1) + 1)))
}



void WP34S_ArcCosh(const real_t *xin, real_t *res, realContext_t *realContext) {
  real_t x, z;

  realCopy(xin, &x);
  realMultiply(&x, &x, res, realContext);      // res = x²
  realSubtract(res, const_1, &z, realContext); // z = x² - 1
  realSquareRoot(&z, res, realContext);        // res = sqrt(x²-1)
  realAdd(res, &x, &z, realContext);           // z = x + sqrt(x²-1)
  WP34S_Ln(&z, res, realContext);              // res = ln(x + sqrt(x²-1))
}



void WP34S_ArcTanh(const real_t *x, real_t *res, realContext_t *realContext) {
  real_t y, z;

  if(realIsNaN(x)) {
    realCopy(const_NaN, res);
  }

  // Not the obvious formula but more stable...
  realSubtract(const_1, x, &z, realContext);      // z = 1-x
  realDivide(x, &z, &y, realContext);             // y = x / (1-x)
  realMultiply(&y, const_2, &z, realContext);     // z = 2x / (1-x)
  WP34S_Ln1P(&z, &y, realContext);                // y = ln(1 + 2x / (1-x))
  realMultiply(&y, const_1on2, res, realContext); // res = ln(1 + 2x / (1-x)) / 2
}



/* ln(1+x) */
void WP34S_Ln1P(const real_t *x, real_t *res, realContext_t *realContext) {
  real_t u, v, w;

  if(realIsSpecial(x) || realIsZero(x)) {
    realCopy(x, res);
  }
  else {
    realAdd(x, const_1, &u, realContext);       // u = x+1
    realSubtract(&u, const_1, &v, realContext); // v = x
    if(realIsZero(&v)) {
      realCopy(x, res);
    }
    else {
      realDivide(x, &v, &w, realContext);
      WP34S_Ln(&u, &v, realContext);
      realMultiply(&v, &w, res, realContext);
    }
  }
}



/* exp(x)-1 */
void WP34S_ExpM1(const real_t *x, real_t *res, realContext_t *realContext) {
  real_t u, v, w;

  if(realIsInfinite(x)) {
    if(realIsPositive(x)) {
      realCopy(const_plusInfinity, res);
    }
    else {
      realCopy(const__1, res);
    }
  }

  realExp(x, &u, realContext);
  realSubtract(&u, const_1, &v, realContext);
  if(realIsZero(&v)) { // |x| is very little
    realCopy(x, res);
  }
  else {
    if(realCompareEqual(&v, const__1)) {
      realCopy(const__1, res);
    }
    else {
      realMultiply(&v, x, &w, realContext);
      WP34S_Ln(&u, &v, realContext);
      realDivide(&w, &v, res, realContext);
    }
  }
}



void WP34S_CalcComplexLnGamma_Lanczos(const real_t *zReal, const real_t *zImag, real_t *resReal, real_t *resImag, realContext_t *realContext) {
  real_t rReal, sReal, tReal, uReal, vReal;
  real_t rImag, sImag, tImag, uImag, vImag;
  int k;
  int32_t savedContextDigits;

  savedContextDigits = realContext->digits;
  if(realContext->digits < 51) {
    realContext->digits = 51;
  }

  realZero(&uReal);
  realZero(&uImag);
  realAdd(zReal, const_29, &tReal, realContext);
  realCopy(zImag, &tImag);
  for (k=28; k>=0; k--) {
    divRealComplex((real_t *)(gammaLanczosCoefficients + k), &tReal, &tImag, &sReal, &sImag, realContext);
    realSubtract(&tReal, const_1, &tReal, realContext);
    realAdd(&uReal, &sReal, &uReal, realContext);
    realAdd(&uImag, &sImag, &uImag, realContext);
  }
  realAdd(&uReal, const_gammaC00, &tReal, realContext);
  realCopy(&uImag, &tImag);
  lnComplex(&tReal, &tImag, &sReal, &sImag, realContext);  // (s1, s2)

  realAdd(zReal, const_gammaR, &rReal, realContext);
  realCopy(zImag, &rImag);
  lnComplex(&rReal, &rImag, &uReal, &uImag, realContext);

  realAdd(zReal, const_1on2, &tReal, realContext);
  realCopy(zImag, &tImag);
  mulComplexComplex(&tReal, &tImag, &uReal, &uImag, &vReal, &vImag, realContext);

  realSubtract(&vReal, &rReal, &uReal, realContext);
  realSubtract(&vImag, zImag, &uImag, realContext);

  realContext->digits = savedContextDigits;

  realAdd(&uReal, &sReal, resReal, realContext);
  realAdd(&uImag, &sImag, resImag, realContext);
}


static void WP34S_ComplexGammaLnGamma(const real_t *zReal, const real_t *zImag, const bool_t calculateLnGamma, real_t *resReal, real_t *resImag, realContext_t *realContext) {
  real_t sinPiZReal, tReal, uReal, xReal;
  real_t sinPiZImag, tImag, uImag, xImag;
  bool_t reflect = false;

  // Check for special cases
  if(realIsSpecial(zReal) || realIsSpecial(zImag)) {
    if(realIsNaN(zReal) || realIsNaN(zImag)) {
      realCopy(const_NaN, resReal);
      realCopy(const_NaN, resImag);
    }
    else {
      if(realIsInfinite(zReal)) {
        if(realIsInfinite(zImag) || realIsNegative(zReal)) {
          realCopy(const_NaN, resReal);
          realCopy(const_NaN, resImag);
        }
        else {
          realCopy(const_plusInfinity, resReal);
          realZero(resImag);
        }
      }
      else {
        realZero(resReal);
        realZero(resImag);
      }
    }
    return;
  }

  // Correct our argument and begin the inversion if it is negative
  if(realIsNegative(zReal)) {
    reflect = true;
    realSubtract(const_1, zReal, &tReal, realContext);
    if(realIsZero(zImag) && realIsAnInteger(&tReal)) {
      realCopy(const_NaN, resReal);
      realCopy(const_NaN, resImag);
      return;
    }
    realSubtract(&tReal, const_1, &xReal, realContext);
    realMinus(zImag, &xImag, realContext);
  }
  else {
    realSubtract(zReal, const_1, &xReal, realContext);
    realCopy(zImag, &xImag);
  }

  // Sum the series
  WP34S_CalcComplexLnGamma_Lanczos(&xReal, &xImag, resReal, resImag, realContext);
  if(!calculateLnGamma) {
    expComplex(resReal, resImag, resReal, resImag, realContext);
  }

  // Finally invert if we started with a negative argument
  if(reflect) {
    realMultiply(zReal, const_pi, &tReal, realContext);
    realMultiply(zImag, const_pi, &tImag, realContext);
    sinComplex(&tReal, &tImag, &sinPiZReal, &sinPiZImag, realContext);
    if(!calculateLnGamma) {
      mulComplexComplex(&sinPiZReal, &sinPiZImag, resReal, resImag, &uReal, &uImag, realContext);
      divRealComplex(const_pi, &uReal, &uImag, resReal, resImag, realContext);
    }
    else {
      divRealComplex(const_pi, &sinPiZReal, &sinPiZImag, &uReal, &uImag, realContext);
      lnComplex(&uReal, &uImag, &tReal, &tImag, realContext);
      realSubtract(&tReal, resReal, resReal, realContext);
      realSubtract(&tImag, resImag, resImag, realContext);
    }
  }
}


void WP34S_ComplexGamma(const real_t *zinReal, const real_t *zinImag, real_t *resReal, real_t *resImag, realContext_t *realContext) {
  real_t zReal, zImag;

  realCopy(zinReal, &zReal);
  realCopy(zinImag, &zImag);
  WP34S_ComplexGammaLnGamma(&zReal, &zImag, false, resReal, resImag, realContext);
}

void WP34S_ComplexLnGamma(const real_t *zinReal, const real_t *zinImag, real_t *resReal, real_t *resImag, realContext_t *realContext) {
  real_t zReal, zImag;

  realCopy(zinReal, &zReal);
  realCopy(zinImag, &zImag);
  WP34S_ComplexGammaLnGamma(&zReal, &zImag, true, resReal, resImag, realContext);
}



void WP34S_Mod(const real_t *x, const real_t *y, real_t *res, realContext_t *realContext) {
 /* Declare a structure large enough to hold a really long number.
  * This structure is likely to be larger than is required.
  */
 real1071_t out;

 realDivideRemainder(x, y, (real_t *)&out, &ctxtReal1071);
 realPlus((real_t *)&out, res, realContext);
}


//void WP34S_BigMod(const real_t *x, const real_t *y, real_t *res, realContext_t *realContext) {
// /* Declare a structure large enough to hold a really long number.
//  * This structure is likely to be larger than is required.
//  */
// real2139_t out;
//
// realDivideRemainder(x, y, &out, &ctxtReal2139);
// realPlus((real_t *)&out, res, realContext);
//}


void *xcopy(void *dest, const void *source, int n) {
  char       *pDest   = (char *)dest;
  const char *pSource = (char *)source;

  if(pSource > pDest) {
    while(n--) {
      *pDest++ = *pSource++;
    }
  }
  else if (pSource < pDest) {
    while(n--) {
      pDest[n] = pSource[n];
    }
  }

  return dest;
}
