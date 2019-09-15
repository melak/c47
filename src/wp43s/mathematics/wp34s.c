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

/* Have to be careful here to ensure that every function we call can handle
 * the increased size of the numbers we're using.
 */
void WP34S_cvt_2rad_sincostan(const realIc_t *an, uint32_t angularMode, realIc_t *sin, realIc_t *cos, realIc_t *tan) {
	 bool_t sinNeg = false, cosNeg = false, swap = false;
  realIc_t angle;
  real451_t angle451;

	 if(realIcIsNaN(an)) {
	   if(sin != NULL) {
	 	  realIcCopy(const_NaN, sin);
	 	 }
	   if(cos != NULL) {
	  	  realIcCopy(const_NaN, cos);
	  	}
	   if(tan != NULL) {
	  	  realIcCopy(const_NaN, tan);
	  	}
   return;
	 }

  realIcCopy(an, &angle);

	 if(angularMode == AM_DMS) {
    angularMode = AM_DEGREE;
    convertAngleIcFromTo(&angle, AM_DMS, AM_DEGREE);
	 }

	 // sin(-x) = -sin(x), cos(-x) = cos(x)
	 if(realIcIsNegative(&angle)) {
	  	sinNeg = true;
	  	realIcSetPositiveSign(&angle);
	 }

 	realIcCopy(&angle, (realIc_t *)&angle451);
 	switch(angularMode) {
    case AM_DEGREE:
    	 real451Remainder(&angle451, const_360,    &angle451); // mod(angle, 360°) --> angle
     	break;

    case AM_GRAD:
    	 real451Remainder(&angle451, const_400,    &angle451); // mod(angle, 360°) --> angle
     	break;

    case AM_RADIAN:
    	 real451Remainder(&angle451, const451_2pi, &angle451); // mod(angle, 360°) --> angle
     	break;

    case AM_MULTPI:
    	 real451Remainder(&angle451, const_2,      &angle451); // mod(angle, 360°) --> angle
     	break;

    default: {}
 	}

	 realIcPlus((realIc_t *)&angle451, &angle);

	 // sin(180+x) = -sin(x), cos(180+x) = -cos(x)
 	if(realIcCompareGreaterEqual(&angle, angle180 + angularMode)) { // angle >= 180°
	  	realIcSubtract(&angle, angle180 + angularMode, &angle);       // angle - 180° --> angle
	  	sinNeg = !sinNeg;
	  	cosNeg = !cosNeg;
  }

	 // sin(90+x) = cos(x), cos(90+x) = -sin(x)
	 if(realIcCompareGreaterEqual(&angle, angle90 + angularMode)) {      // angle >= 90°
		  realIcSubtract(&angle, angle90 + angularMode, &angle); // angle - 90° --> angle
		  swap = true;
		  cosNeg = !cosNeg;
  }

	 // sin(90-x) = cos(x), cos(90-x) = sin(x)
	 if(realIcCompareEqual(&angle, angle45 + angularMode)) { // angle == 45°
	   if(sin != NULL) {
	 	  realIcCopy(const_root2on2, sin);
	 	 }
	   if(cos != NULL) {
	  	  realIcCopy(const_root2on2, cos);
	  	}
	   if(tan != NULL) {
	  	  realIcCopy(const_1, tan);
	  	}
  }
	 else { // angle < 90
		  if(realIcCompareGreaterThan(&angle, angle45 + angularMode)) { // angle > 45°
		   	realIcSubtract(angle90 + angularMode, &angle, &angle);        // 90° - angle  --> angle
			   swap = !swap;
   	}

    convertAngleIcFromTo(&angle, angularMode, AM_RADIAN);
		  WP34S_sincostanTaylor(&angle, swap, swap?cos:sin, swap?sin:cos, tan); // angle in radian
  }

  if(sin != NULL) {
    if(sinNeg) {
      realIcSetNegativeSign(sin);
      if(tan != NULL) realIcSetNegativeSign(tan);
    }
		  if(realIcIsZero(sin)) {
      realIcSetPositiveSign(sin);
      if(tan != NULL) {
        realIcSetPositiveSign(tan);
      }
		  }
  }

  if(cos != NULL) {
    if(cosNeg) {
      realIcSetNegativeSign(cos);
      if(tan != NULL) realIcChangeSign(tan);
    }
		  if(realIcIsZero(cos)) {
      realIcSetPositiveSign(cos);
		  }
  }
}


/* Calculate sin, cos by Taylor series and tan by division
 */
void WP34S_sincostanTaylor(const realIc_t *a, bool_t swap, realIc_t *sinOut, realIc_t *cosOut, realIc_t *tanOut) { // a in radian
 	real51_t angle, a2, t, j, z, sin, cos, compare;
 	int i, odd;
 	bool_t finSin = (sinOut == NULL), finCos = (cosOut == NULL);
 	int32_t cmp;

 	realIcCopy(a, &angle);
 	real51Multiply(&angle, &angle, &a2);
 	uInt32ToReal51(1, &j);
 	uInt32ToReal51(1, &t);
 	uInt32ToReal51(1, &sin);
 	uInt32ToReal51(1, &cos);

 	for(i=1; !(finSin && finCos) && i < 1000; i++) {
	  	odd = i & 1;

  		real51Add(&j, const_1, &j);
	  	real51Divide(&a2, &j, &z);
  		real51Multiply(&t, &z, &t);

	  	if(!finCos) {
	   		real51Copy(&cos, &z);

		   	if(odd) {
			   	 real51Subtract(&cos, &t, &cos);
      }
			   else {
			   	 real51Add(&cos, &t, &cos);
      }

      real51Compare(&cos, &z, &compare);
      real51ToInt32(&compare, cmp);
		   	finCos = (cmp == 0);
		  }

  		real51Add(&j, const_1, &j);
		  real51Divide(&t, &j, &t);

	  	if(!finSin) {
		   	real51Copy(&sin, &z);

			   if(odd) {
		   	  real51Subtract(&sin, &t, &sin);
		   	}
		   	else {
		   	  real51Add(&sin, &t, &sin);
		    }

      real51Compare(&sin, &z, &compare);
      real51ToInt32(&compare, cmp);
		   	finSin = (cmp == 0);
	  	}
	 }

	 if(sinOut != NULL) {
		  real51Multiply(&sin, &angle, &sin);
		  //realIcCopy(&sin, sinOut);
		  realIcAdd(&sin, const_0, sinOut);
	 }

 	if(cosOut != NULL) {
 		 //realIcCopy(&cos, cosOut);
		  realIcAdd(&cos, const_0, cosOut);
		}

 	if(tanOut != NULL) {
 	  if(sinOut == NULL || cosOut == NULL) {
      realIcCopy(const_NaN, tanOut);
 	  }
 	  else {
 	    if(swap) {
   	    realIcDivide(&cos, &sin, tanOut);
   	  }
 	    else {
   	    realIcDivide(&sin, &cos, tanOut);
 	    }
 	  }
		}
}



void WP34S_atan(const realIc_t *x, realIc_t *angle) {
 	realIc_t a, b, a2, t, j, z, last;
 	int doubles = 0;
 	int invert;
 	int n;
 	int neg = realIcIsNegative(x);

  realIcCopy(x, &a);

 	// arrange for a >= 0
 	if(neg) {
 		 realIcChangeSign(&a);
 	}

 	// reduce range to 0 <= a < 1, using atan(x) = pi/2 - atan(1/x)
 	invert = realIcCompareGreaterThan(&a, const_1);
 	if(invert) {
 		 realIcDivide(const_1, &a, &a);
 	}

 	// Range reduce to small enough limit to use taylor series using:
	 //  tan(x/2) = tan(x)/(1+sqrt(1+tan(x)²))
	 for(n=0; n<1000; n++) {
	  	if(realIcCompareLessEqual(&a, const_1on10)) {
		  	 break;
		  }
		  doubles++;
		  // a = a/(1+sqrt(1+a²)) -- at most 3 iterations.
	  	realIcMultiply(&a, &a, &b);
		  realIcAdd(&b, const_1, &b);
		  realIcSquareRoot(&b, &b);
		  realIcAdd(&b, const_1, &b);
		  realIcDivide(&a, &b, &a);
	 }

	 // Now Taylor series
	 // atan(x) = x(1-x²/3+x⁴/5-x⁶/7...)
	 // We calculate pairs of terms and stop when the estimate doesn't change
 	realIcCopy(const_3, angle);
 	realIcCopy(const_5, &j);
 	realIcMultiply(&a, &a, &a2);	// a²
 	realIcCopy(&a2, &t);
 	realIcDivide(&t, angle, angle);	// s = 1-t/3 -- first two terms
 	realIcSubtract(const_1, angle, angle);

 	do {	// Loop until there is no digits changed
	  	realIcCopy(angle, &last);

  		realIcMultiply(&t, &a2, &t);
	  	realIcDivide(&t, &j, &z);
		  realIcAdd(angle, &z, angle);

		  realIcAdd(&j, const_2, &j);

  		realIcMultiply(&t, &a2, &t);
	  	realIcDivide(&t, &j, &z);
		  realIcSubtract(angle, &z, angle);

		  realIcAdd(&j, const_2, &j);

	 } while(!realIcCompareEqual(angle, &last));

	 realIcMultiply(angle, &a, angle);

	 while(doubles) {
  		realIcAdd(angle, angle, angle);
  		doubles--;
	 }

 	if(invert) {
  	 realIcSubtract(const_piOn2, angle, angle);
  }

 	if(neg) {
 		 realIcChangeSign(angle);
 	}
}



void WP34S_atan2(const realIc_t *y, const realIc_t *x, realIc_t *atan) {
	 realIc_t r, t;
	 const bool_t xNeg = realIcIsNegative(x);
	 const bool_t yNeg = realIcIsNegative(y);

	 if(realIcIsNaN(x) || realIcIsNaN(y)) {
	   realIcCopy(const_NaN, atan);
		  return;
	 }

	 if(realIcCompareEqual(y, const_0)) {
		  if(yNeg) {
			   if(realIcCompareEqual(x, const_0)) {
				    if(xNeg) {
					     realIcCopy(const_pi, atan);
					     realIcSetNegativeSign(atan);
				    }
				    else {
					     realIcCopy(y, atan);
					   }
			   }
			   else if(xNeg) {
			     realIcCopy(const_pi, atan);
			     realIcSetNegativeSign(atan);
			   }
      else {
			     realIcCopy(y, atan);
			   }
		  }
    else {
			   if(realIcCompareEqual(x, const_0)) {
			    	if(xNeg) {
 			      realIcCopy(const_pi, atan);
 			    }
				    else {
					     realIcZero(atan);
					   }
			   }
			   else if(xNeg) {
		      realIcCopy(const_pi, atan);
		    }
		   	else {
			     realIcZero(atan);
			   }
		  }
		  return;
	 }

  if(realIcCompareEqual(x, const_0)) {
	  	realIcCopy(const_piOn2, atan);
	  	if(yNeg) {
      realIcSetNegativeSign(atan);
    }
		  return;
	 }

	 if(realIcIsInfinite(x)) {
	  	if(xNeg) {
		   	if(realIcIsInfinite(y)) {
			    	realIcCopy(const_3piOn4, atan);
		    		if(yNeg) {
          realIcSetNegativeSign(atan);
        }
			   }
			   else {
			    	realIcCopy(const_pi, atan);
			    	if(yNeg) {
          realIcSetNegativeSign(atan);
        }
			   }
		  }
		  else {
		   	if(realIcIsInfinite(y)) {
			    	realIcCopy(const_piOn4, atan);
	    			if(yNeg) {
          realIcSetNegativeSign(atan);
        }
			   }
			   else {
		    		realIcZero(atan);
		    		if(yNeg) {
          realIcSetNegativeSign(atan);
        }
			   }
		  }
		  return;
	 }

 	if(realIcIsInfinite(y)) {
	  	realIcCopy(const_piOn2, atan);
	  	if(yNeg) {
      realIcSetNegativeSign(atan);
    }
		  return;
	 }

 	realIcDivide(y, x, &t);
	 WP34S_atan(&t, &r);
	 if(xNeg) {
	  	realIcCopy(const_pi, &t);
	  	if(yNeg) {
     realIcSetNegativeSign(&t);
    }
	 }
	 else {
	 	 realIcZero(&t);
	 }

 	realIcAdd(&r, &t, atan);
 	if(realIcCompareEqual(atan, const_0) && yNeg) {
    realIcSetNegativeSign(atan);
  }
}



void WP34S_asin(const realIc_t *x, realIc_t *angle) {
	 realIc_t abx, z;

	 if(realIcIsNaN(x)) {
	  	realIcCopy(const_NaN, angle);
	  	return;
	 }

  realIcCopyAbs(x, &abx);
	 if(realIcCompareGreaterThan(&abx, const_1)) {
	  	realIcCopy(const_NaN, angle);
	  	return;
	 }

	 // angle = 2*atan(x/(1+sqrt(1-x*x)))
	 realIcMultiply(x, x, &z);
	 realIcSubtract(const_1, &z, &z);
  realIcSquareRoot(&z, &z);
	 realIcAdd(&z, const_1, &z);
	 realIcDivide(x, &z, &z);
	 WP34S_atan(&z, &abx);
	 realIcAdd(&abx, &abx, angle);
}



void WP34S_acos(const realIc_t *x, realIc_t *angle) {
 	realIc_t abx, z;

	 if(realIcIsNaN(x)) {
	  	realIcCopy(const_NaN, angle);
	  	return;
	 }

  realIcCopyAbs(x, &abx);
	 if(realIcCompareGreaterThan(&abx, const_1)) {
	  	realIcCopy(const_NaN, angle);
	  	return;
	 }

	 // angle = 2*atan((1-x)/sqrt(1-x*x))
	 if(realIcCompareEqual(x, const_1)) {
	 	 realIcZero(angle);
	 }
	 else {
 	 	realIcMultiply(x, x, &z);
  		realIcSubtract(const_1, &z, &z);
    realIcSquareRoot(&z, &z);
  		realIcSubtract(const_1, x, &abx);
  		realIcDivide(&abx, &z, &z);
  	 WP34S_atan(&z, &abx);
  	 realIcAdd(&abx, &abx, angle);
	 }
}



/* Check if a number is an integer.
 */
bool_t WP34S_is_int(const realIc_t *x) {
 	realIc_t r, y;

 	if(realIcIsNaN(x)) {
 		 return false;
 	}
 	if(realIcIsInfinite(x)) {
 		 return true;
 	}

 	realIcToIntegralValue(x, &y);
 	realIcSubtract(x, &y, &r);

 	return realIcCompareEqual(&r, const_0);
}



static void WP34S_CalcLnGamma(const realIc_t *xin, realIc_t *res) {
	 realIc_t r, s, t, u, v;
	 int32_t k;

 	realIcZero(&s);
 	realIcAdd(xin, const_21, &t);
 	for(k=20; k>=0; k--) {
	  	realIcDivide(gammaConstants + k, &t, &u);
	  	realIcSubtract(&t, const_1, &t);
	  	realIcAdd(&s, &u, &s);
  }

	 realIcAdd(&s, const_gammaC00, &t);
	 WP34S_Ln(&t, &s);

  //		r = z + g + .5;
	 realIcAdd(xin, const_gammaR, &r);

  //		r = log(R[0][0]) + (z+.5) * log(r) - r;
	 WP34S_Ln(&r, &u);
	 realIcAdd(xin, const_0_5, &t);
	 realIcMultiply(&u, &t, &v);

	 realIcSubtract(&v, &r, &u);
	 realIcAdd(&u, &s, res);
}



// common code for the [GAMMA] and LN[GAMMA]
static void WP34S_Gamma_LnGamma(const realIc_t *xin, const bool_t calculateLn, realIc_t *res) {
	 realIc_t x, t;
	 int reflec = 0;

	 // Check for special cases
	 if(realIcIsSpecial(xin)) {
	  	if(realIcIsInfinite(xin) && !realIcIsNegative(xin)) {
	  	  realIcCopy(const_plusInfinity, res);
		  	 return;
    }

 	  realIcCopy(const_NaN, res);
		  return;
	 }

	 // Handle x approximately zero case
	 realIcCopy(xin, &t);
	 realIcSetPositiveSign(&t);
	 if(realIcCompareLessThan(&t, const_1e_24)) {
	  	if(realIcCompareEqual(xin, const_0)) {
   	  realIcCopy(const_NaN, res);
      return;
    }
		  realIcDivide(const_1, xin, &x);
	  	realIcSubtract(&x, const_egamma, res);
	  	if(calculateLn) {
	  	 WP34S_Ln(res, res);
	  	}
		  return;
	 }

	 // Correct our argument and begin the inversion if it is negative
	 if(realIcCompareLessEqual(xin, const_0)) {
	  	reflec = 1;
	  	realIcSubtract(const_1, xin, &t);
	  	if(WP34S_is_int(&t)) {
   	  realIcCopy(const_NaN, res);
	  		 return;
    }
	  	realIcSubtract(&t, const_1, &x);
	 }
	 else {
	  	realIcSubtract(xin, const_1, &x);

	 	 // Provide a fast path evaluation for positive integer arguments that aren't too large
	 	 // The threshold for overflow is 205! (i.e. 204! is within range and 205! isn't).
	 	 if(WP34S_is_int(&x) && !realIcCompareEqual(xin, const_0) && realIcCompareLessEqual(&x, const_204)) {
	 	  	realIcCopy(const_1, res);
	 	  	while(realIcCompareGreaterThan(&x, const_0)) {
	 	 	  	realIcMultiply(res, &x, res);
	 	 		  realIcSubtract(&x, const_1, &x);
	 	 	 }
	 	 	 if(calculateLn) {
	 	 	 	 WP34S_Ln(res, res);
	 	 	 	 return;
      }
	 	  	return;
	 	 }
	 }

 	WP34S_CalcLnGamma(&x, res);

 	if(!calculateLn) {
 	  realIcExp(res, res);
 	}

 	// Finally invert if we started with a negative argument
 	if(reflec) {
 	 	// figure out xin * PI mod 2PI
 	 	realIcRemainder(xin, const_2, &t);
 	 	realIcMultiply(&t, const_pi, &t);
 	 	WP34S_sincostanTaylor(&t, false, &x, NULL, NULL); // t in radian
  		if(calculateLn) {
  		 	realIcDivide(const_pi, &x, &t);
  		 	WP34S_Ln(&t, &t);
  		 	realIcSubtract(&t, res, res);
  		}
  		else {
  	 		realIcMultiply(&x, res, &t);
  	 		realIcDivide(const_pi, &t, res);
  	 }
  }
}



void WP34S_Factorial(const realIc_t *xin, realIc_t *res) {
	 realIc_t x;

	 realIcAdd(xin, const_1, &x);
	 WP34S_Gamma_LnGamma(&x, false, res);
}



void WP34S_Gamma(const realIc_t *xin, realIc_t *res) {
	 realIc_t x;

  realIcCopy(xin, &x);
	 WP34S_Gamma_LnGamma(&x, false, res);
}



void WP34S_LnGamma(const realIc_t *xin, realIc_t *res) {
	 realIc_t x;

  realIcCopy(xin, &x);
	 WP34S_Gamma_LnGamma(&x, true, res);
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
void WP34S_Ln(const realIc_t *xin, realIc_t *res) {
 	realIc_t z, t, f, n, m, i, v, w, e;
 	int32_t expon;

 	if(realIcIsSpecial(xin)) {
	  	if(realIcIsNaN(xin) || realIcIsNegative(xin)) {
  	   realIcCopy(const_NaN, res);
		  	 return;
    }

	   realIcCopy(const_plusInfinity, res);
	  	return;
  }

	 if(realIcCompareLessEqual(xin, const_0)) {
	  	if(realIcIsNegative(xin)) {
  	   realIcCopy(const_NaN, res);
		  	 return;
    }

	   realIcCopy(const_minusInfinity, res);
		  return;
	 }

	 realIcCopy(xin, &z);
	 realIcCopy(const_2, &f);
	 realIcSubtract(xin, const_1, &t);
	 realIcCopy(&t, &v);
	 realIcSetPositiveSign(&v);
	 if(realIcCompareGreaterThan(&v, const_0_5)) {
	  	expon = z.exponent + z.digits;
	  	z.exponent = -z.digits;
	 }
	 else {
		  expon = 0;
		}

  /* The too high case never happens
 	while(dn_le(const_2, &z)) {
	  	realIcMultiply(&f, const_2, &f);
		  realIcSquareRoot(&z, &z);
  } */

	 // Range reduce the value by repeated square roots.
	 // Making the constant here larger will reduce the number of later
	 // iterations at the expense of more square root operations.
	 while(realIcCompareLessEqual(&z, const_root2on2)) {
	  	realIcMultiply(&f, const_2, &f);
		  realIcSquareRoot(&z, &z);
  }

 	realIcAdd(&z, const_1, &t);
 	realIcSubtract(&z, const_1, &v);
 	realIcDivide(&v, &t, &n);
 	realIcCopy(&n, &v);
 	realIcMultiply(&v, &v, &m);
 	realIcCopy(const_3, &i);

	 for(;;) {
  		realIcMultiply(&m, &n, &n);
	  	realIcDivide(&n, &i, &e);
	  	realIcAdd(&v, &e, &w);
	  	if(WP34S_relative_error(&w, &v, const_1e_37)) {
	  		 break;
	  	}
	  	realIcCopy(&w, &v);
	  	realIcAdd(&i, const_2, &i);
	 }

	 realIcMultiply(&f, &w, res);
	 if(expon == 0) {
  	 return;
  }

	 int32ToRealIc(expon, &e);
	 realIcMultiply(&e, const_ln10, &w);
	 realIcAdd(res, &w, res);
}



void WP34S_Log(const realIc_t *xin, const realIc_t *base, realIc_t *res) {
 	realIc_t y;

 	if(realIcIsSpecial(xin)) {
	  	if(realIcIsNaN(xin) || realIcIsNegative(xin)) {
  	   realIcCopy(const_NaN, res);
		  	 return;
    }

	   realIcCopy(const_plusInfinity, res);
	  	return;
  }

	 WP34S_Ln(xin, &y);

 	realIcDivide(&y, base, res);
}



void WP34S_Log2(const realIc_t *xin, realIc_t *res) {
 	WP34S_Log(xin, const_ln2, res);
}



void WP34S_Log10(const realIc_t *xin, realIc_t *res) {
 	WP34S_Log(xin, const_ln10, res);
}



void WP34S_Logxy(const realIc_t *yin, const realIc_t *xin, realIc_t *res) {
  realIc_t lx;

	 WP34S_Ln(xin, &lx);
	 WP34S_Log(yin, &lx, res);
}



bool_t WP34S_relative_error(const realIc_t *x, const realIc_t *y, const realIc_t *tol) {
 	realIc_t a;

 	if(realIcCompareEqual(x, const_0)) {
 	  realIcCopyAbs(y, &a);
 		 return realIcCompareLessThan(&a, tol);
  }

 	realIcSubtract(x, y, &a);
 	realIcDivide(&a, x, &a);
 	realIcSetPositiveSign(&a);
 	return realIcCompareLessThan(&a, tol);
}


/* Hyperbolic functions.
 * We start with a utility routine that calculates sinh and cosh.
 * We do the sihn as (e^x - 1) (e^x + 1) / (2 e^x) for numerical stability
 * reasons if the value of x is smallish.
 */
void WP34S_sinhcosh(const realIc_t *x, realIc_t *sinhOut, realIc_t *coshOut) {
 	realIc_t t, u, v;

 	if(sinhOut != NULL) {
	  	if(realIcCompareAbsLessThan(x, const_1on2)) {
      WP34S_Expm1(x, &u);                           // u = e^x - 1
    		realIcMultiply(&u, const_1on2, &t);           // t = (e^x - 1) / 2

   			realIcAdd(&u, const_1, &u);                   // u = e^x
   			realIcDivide(&t, &u, &v);                     // v = (e^x - 1) / 2e^x

   			realIcAdd(&u, const_1, &u);                   // u = e^x + 1
   			realIcMultiply(&u, &v, sinhOut);              // sinhOut = (e^x - 1)(e^x + 1) / 2e^x
  		}
  		else {
    		realIcExp(x, &u);			                          // u = e^x
  		  realIcDivide(const_1, &u, &v);              		// v = e^-x
    		realIcSubtract(&u, &v, sinhOut);	             // sinhOut = (e^x + e^-x)
  	  	realIcMultiply(sinhOut, const_1on2, sinhOut); // sinhOut = (e^x + e^-x)/2
  		}
 	}
 	if(coshOut != NULL) {
 		realIcExp(x, &u);			                             // u = e^x
 		realIcDivide(const_1, &u, &v);                 		// v = e^-x
 		realIcAdd(&u, &v, coshOut);	                     // coshOut = (e^x + e^-x)
 		realIcMultiply(coshOut, const_1on2, coshOut);    // coshOut = (e^x + e^-x)/2
 	}
}



void WP34S_tanh(const realIc_t *x, realIc_t *res) {
 	if(realIcCompareAbsGreaterThan(x, const_47)) { // equals 1 to 39 digits
    realIcCopy((realIcIsPositive(x) ? const_1 : const__1), res);
 	}
 	else {
 	  realIc_t a, b;

   	realIcAdd(x, x, &a);        // a = 2x
   	WP34S_Expm1(&a, &b);        // b = exp(2x) - 1
   	realIcAdd(&b, const_2, &a); // a = exp(2x) - 1 + 2 = exp(2x) + 1
	   realIcDivide(&b, &a, res); // res = (exp(2x) - 1) / (exp(2x) + 1)
 	}
}



void WP34S_ArcSinh(const realIc_t *x, realIc_t *res) {
	 realIc_t a;

 	realIcMultiply(x, x, &a);	 	// a = x²
 	realIcAdd(&a, const_1, &a);	// a = x² + 1
  realIcSquareRoot(&a, &a);		 // a = sqrt(x²+1)
 	realIcAdd(&a, const_1, &a);	// a = sqrt(x²+1)+1
 	realIcDivide(x, &a, &a);    // a = x / (sqrt(x²+1)+1)
 	realIcAdd(&a, const_1, &a);	// a = x / (sqrt(x²+1)+1) + 1
  realIcMultiply(x, &a, &a);  // y = x * (x / (sqrt(x²+1)+1) + 1)
 	WP34S_Ln1p(&a, res);        // res = ln(1 + (x * (x / (sqrt(x²+1)+1) + 1)))
}



void WP34S_ArcCosh(const realIc_t *xin, realIc_t *res) {
 	realIc_t x, z;

  realIcCopy(xin, &x);
 	realIcMultiply(&x, &x, res);	     // res = x²
 	realIcSubtract(res, const_1, &z);	// z = x² - 1
 	realIcSquareRoot(&z, res);	      	// res = sqrt(x²-1)
 	realIcAdd(res, &x, &z);	         	// z = x + sqrt(x²-1)
 	WP34S_Ln(&z, res);                // res = ln(x + sqrt(x²-1))
}



void WP34S_ArcTanh(const realIc_t *x, realIc_t *res) {
 	realIc_t y, z;

 	// Not the obvious formula but more stable...
 	realIcSubtract(const_1, x, &z);      // z = 1-x
  realIcDivide(x, &z, &y);             // y = x / (1-x)
 	realIcMultiply(&y, const_2, &z);     // z = 2x / (1-x)
 	WP34S_Ln1p(&z, &y);                  // y = ln(1 + 2x / (1-x))
  realIcMultiply(&y, const_1on2, res); // res = ln(1 + 2x / (1-x)) / 2
}



/* ln(1+x) */
void WP34S_Ln1p(const realIc_t *x, realIc_t *res) {
	 realIc_t u, v, w;

 	if(realIcIsZero(x)) {
 		 realIcZero(res);
 	}
 	else {
   	realIcAdd(x, const_1, &u);       // u = x+1
   	realIcSubtract(&u, const_1, &v); // v = x
 	  if(realIcIsZero(&v)) {
   		 realIcCopy(x, res);
   	}
   	else {
   	  realIcDivide(x, &v, &w);
     	WP34S_Ln(&u, &v);
   	  realIcMultiply(&v, &w, res);
   	}
 	}
}



/* exp(x)-1 */
void WP34S_Expm1(const realIc_t *x, realIc_t *res) {
 	realIc_t u, v, w;

 	realIcExp(x, &u);
 	realIcSubtract(&u, const_1, &v);
 	if(realIcIsZero(&v)) { // |x| is very little
 		 realIcCopy(x, res);
 	}
 	else {
 	  if(realIcCompareEqual(&v, const__1)) {
   		 realIcCopy(const__1, res);
   	}
   	else {
   	  realIcMultiply(&v, x, &w);
   	  WP34S_Ln(&u, &v);
   	  realIcDivide(&w, &v, res);
   	}
 	}
}



