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
 * \file decNumberWrappers.h
 ***********************************************/

#define real16_t                                          decDouble
#define real34_t                                          decQuad
#define real51_t                                          decNumber
typedef struct {
  int32_t digits;      /* Count of digits in the coefficient; >0    */
  int32_t exponent;    /* Unadjusted exponent, unbiased, in         */
                       /* range: -1999999997 through 999999999      */
  uint8_t bits;        /* Indicator bits (see above)                */
                       /* Coefficient, from least significant unit  */
  decNumberUnit lsu[(451+DECDPUN-1)/DECDPUN];
} real451_t;

typedef struct {real16_t x[2];}                           complex16_t;
typedef struct {real34_t x[2];}                           complex34_t;

#define REAL16_SIZE                                       sizeof(real16_t)
#define REAL34_SIZE                                       sizeof(real34_t)
#define COMPLEX16_SIZE                                    (2*sizeof(real16_t))
#define COMPLEX34_SIZE                                    (2*sizeof(real34_t))
#define REAL51_SIZE                                       sizeof(real51_t)
#define REAL451_SIZE                                      sizeof(real451_t)

#define POINTER_TO_REGISTER_NAME(a)                       ((char     *)(ram + getRegisterNamePointer(a)))
#define POINTER_TO_LOCAL_FLAGS                            ((uint16_t *)(ram + allLocalRegisterPointer))
#define POINTER_TO_LOCAL_REGISTER(a)                      ((uint32_t *)(ram + allLocalRegisterPointer + 2u + 4u*(a)))
#define POINTER_TO_NAMED_REGISTER(a)                      ((uint32_t *)(ram + allNamedRegisterPointer      + 6u*(a)))
#define POINTER_TO_POINTER_TO_NAMED_REGISTER_NAME(a)      ((uint16_t *)(ram + allNamedRegisterPointer + 4u + 6u*(a)))
#define POINTER_TO_NAMED_REGISTER_NAME(a)                 ((char     *)(ram + (*POINTER_TO_POINTER_TO_NAMED_REGISTER_NAME(a) << 1)))
#define RAM_REAL16(a)                                     ((real16_t *)(ram + (a)))
#define RAM_REAL34(a)                                     ((real34_t *)(ram + (a)))


#define REGISTER_DATA(a)                                  ((void        *)(ram + getRegisterDataPointer(a)))
#define REGISTER_REAL16_DATA(a)                           ((real16_t    *)(ram + getRegisterDataPointer(a)))
#define REGISTER_IMAG16_DATA(a)                           ((real16_t    *)(ram + getRegisterDataPointer(a) + REAL16_SIZE))
#define REGISTER_COMPLEX16_DATA(a)                        ((complex16_t *)(ram + getRegisterDataPointer(a)))
#define REGISTER_REAL34_DATA(a)                           ((real34_t    *)(ram + getRegisterDataPointer(a)))
#define REGISTER_IMAG34_DATA(a)                           ((real34_t    *)(ram + getRegisterDataPointer(a) + REAL34_SIZE))
#define REGISTER_COMPLEX34_DATA(a)                        ((complex34_t *)(ram + getRegisterDataPointer(a)))
#define REGISTER_STRING_DATA(a)                           ((char        *)(ram + getRegisterDataPointer(a) + 2)) // Memory pointer to the string of a register
#define REGISTER_LONG_INTEGER_DATA(a)                     ((uint8_t     *)(ram + getRegisterDataPointer(a) + 2)) // Memory pointer to the long integer of a register
#define REGISTER_DATA_MAX_LEN(a)                          ((uint16_t    *)(ram + getRegisterDataPointer(a)))     // Memory pointer to the lenght of string or long integer
#define REGISTER_SHORT_INTEGER_DATA(a)                    ((uint64_t    *)(ram + getRegisterDataPointer(a)))
#define VARIABLE_REAL16_DATA(a)                           ((real16_t    *)(a))
#define VARIABLE_IMAG16_DATA(a)                           ((real16_t    *)((char *)(a) + REAL16_SIZE))
#define VARIABLE_COMPLEX16_DATA(a)                        ((complex16_t *)(a))
#define VARIABLE_REAL34_DATA(a)                           ((real34_t    *)(a))
#define VARIABLE_IMAG34_DATA(a)                           ((real34_t    *)((char *)(a) + REAL34_SIZE))
#define VARIABLE_COMPLEX34_DATA(a)                        ((complex34_t *)(a))


#define real16ToString(source, destination)               decDoubleToString        (source,      destination)
#define real34ToString(source, destination)               decQuadToString          (source,      destination)
#define real51ToString(source, destination)               decNumberToString        (source,      destination)
#define real451ToString(source, destination)              decNumberToString        (source,      destination)
#define stringToReal16(source, destination)               decDoubleFromString      (destination, source, &ctxtReal16)
#define stringToReal34(source, destination)               decQuadFromString        (destination, source, &ctxtReal34)
#define stringToReal51(source, destination)               decNumberFromString      (destination, source, &ctxtReal51)
#define stringToReal451(source, destination)              decNumberFromString      ((decNumber *)(destination), source, &ctxtReal451)
#define stringToReal51Ctxt(source, destination, ctxt)     decNumberFromString      (destination, source, ctxt)
#define int32ToReal16(source, destination)                decDoubleFromInt32       (destination, source)
#define int32ToReal34(source, destination)                decQuadFromInt32         (destination, source)
#define int32ToReal51(source, destination)                decNumberFromInt32       (destination, source)
#define uInt32ToReal16(source, destination)               decDoubleFromUInt32      (destination, source)
#define uInt32ToReal34(source, destination)               decQuadFromUInt32        (destination, source)
#define uInt32ToReal51(source, destination)               decNumberFromUInt32      (destination, source)


#define real16ToReal34(source, destination)               decDoubleToWider         (source,      destination)
#define real34ToReal16(source, destination)               decDoubleFromWider       (destination, source, &ctxtReal16)
#define real16ToReal51(source, destination)               decDoubleToNumber        (source,      destination)
#define real34ToReal51(source, destination)               decQuadToNumber          (source,      destination)
#define real34ToReal451(source, destination)              decQuadToNumber          (source,      destination)
#define real51ToReal16(source, destination)               decDoubleFromNumber      (destination, source, &ctxtReal16)
#define real51ToReal34(source, destination)               decQuadFromNumber        (destination, source, &ctxtReal34)
#define real451ToReal34(source, destination)              decQuadFromNumber        (destination, source, &ctxtReal34)
#define real16AbsToReal16(source, destination)            decDoubleCopyAbs         (destination, source)
#define real34AbsToReal34(source, destination)            decQuadCopyAbs           (destination, source)
#define real51AbsToReal51(source, destination)            decNumberCopyAbs         (destination, source)
#define real16ToUInt32(source)                            decDoubleToUInt32        (source,      &ctxtReal16, DEC_ROUND_DOWN)
#define real34ToUInt32(source)                            decQuadToUInt32          (source,      &ctxtReal34, DEC_ROUND_DOWN)
#define real51ToUInt32(source)                            decNumberToUInt32        (source,      &ctxtReal51)
#define real16ToInt32(source)                             decDoubleToInt32         (source,      &ctxtReal16, DEC_ROUND_DOWN)
#define real34ToInt32(source)                             decQuadToInt32           (source,      &ctxtReal34, DEC_ROUND_DOWN)
#define real51ToInt32(source)                             decNumberToInt32         (source,      &ctxtReal51)

#define real16ToIntegral(source, destination)             decDoubleToIntegralValue (destination, source, &ctxtReal16, DEC_ROUND_DOWN)
#define real34ToIntegral(source, destination)             decQuadToIntegralValue   (destination, source, &ctxtReal34, DEC_ROUND_DOWN)
#define real16ToIntegralRound(source, destination)        decDoubleToIntegralValue (destination, source, &ctxtReal16, DEC_ROUND_HALF_UP)
#define real34ToIntegralRound(source, destination)        decQuadToIntegralValue   (destination, source, &ctxtReal34, DEC_ROUND_HALF_UP)


#define real16IsInfinite(source)                          decDoubleIsInfinite      (source)
#define real34IsInfinite(source)                          decQuadIsInfinite        (source)
#define real51IsInfinite(source)                          decNumberIsInfinite      (source)
#define real16IsNaN(source)                               decDoubleIsNaN           (source)
#define real34IsNaN(source)                               decQuadIsNaN             (source)
#define real51IsNaN(source)                               decNumberIsNaN           (source)
#define real451IsNaN(source)                              decNumberIsNaN           (source)
#define real16IsZero(source)                              decDoubleIsZero          (source)
#define real34IsZero(source)                              decQuadIsZero            (source)
#define real51IsZero(source)                              decNumberIsZero          (source)
#define real16IsNegative(source)                          decDoubleIsNegative      (source)
#define real34IsNegative(source)                          decQuadIsNegative        (source)
#define real51IsNegative(source)                          decNumberIsNegative      (source)
#define real16IsPositive(source)                          decDoubleIsPositive      (source)
#define real34IsPositive(source)                          decQuadIsPositive        (source)
#define real51IsPositive(source)                          (! decNumberIsNegative(source) && ! decNumberIsZero(source))
#define real16IsSpecial(source)                           (decDoubleIsNaN(source) || decDoubleIsSignaling(source) || decDoubleIsInfinite(source))
#define real34IsSpecial(source)                           (decQuadIsNaN(source)   || decQuadIsSignaling(source)   || decQuadIsInfinite(source))
#define real51IsSpecial(source)                           (decNumberIsSpecial(source))
#define real16GetCoefficient(source, destination)         decDoubleGetCoefficient  (source, (uint8_t *)(destination))
#define real34GetCoefficient(source, destination)         decQuadGetCoefficient    (source, (uint8_t *)(destination))
#define real16GetExponent(source)                         decDoubleGetExponent     (source)
#define real34GetExponent(source)                         decQuadGetExponent       (source)


#define real16Zero(destination)                           decDoubleZero            (destination)
#define real34Zero(destination)                           decQuadZero              (destination)
#define real51Zero(destination)                           decNumberZero            (destination)
#define real16Add(operand1, operand2, res)                decDoubleAdd             (res, operand1, operand2, &ctxtReal16)
#define real34Add(operand1, operand2, res)                decQuadAdd               (res, operand1, operand2, &ctxtReal34)
#define real51Add(operand1, operand2, res)                decNumberAdd             (res, operand1, operand2, &ctxtReal51)
#define real16Subtract(operand1, operand2, res)           decDoubleSubtract        (res, operand1, operand2, &ctxtReal16)
#define real34Subtract(operand1, operand2, res)           decQuadSubtract          (res, operand1, operand2, &ctxtReal34)
#define real51Subtract(operand1, operand2, res)           decNumberSubtract        (res, operand1, operand2, &ctxtReal51)
#define real16Multiply(operand1, operand2, res)           decDoubleMultiply        (res, operand1, operand2, &ctxtReal16)
#define real34Multiply(operand1, operand2, res)           decQuadMultiply          (res, operand1, operand2, &ctxtReal34)
#define real51Multiply(operand1, operand2, res)           decNumberMultiply        (res, operand1, operand2, &ctxtReal51)
#define real16Divide(operand1, operand2, res)             decDoubleDivide          (res, operand1, operand2, &ctxtReal16)
#define real34Divide(operand1, operand2, res)             decQuadDivide            (res, operand1, operand2, &ctxtReal34)
#define real51Divide(operand1, operand2, res)             decNumberDivide          (res, operand1, operand2, &ctxtReal51)
#define real16CompareTotal(operand1, operand2, res)       decDoubleCompareTotal    (res, operand1, operand2)
#define real34CompareTotal(operand1, operand2, res)       decQuadCompareTotal      (res, operand1, operand2)
#define real51CompareTotal(operand1, operand2, res)       decNumberCompareTotal    (res, operand1, operand2, &ctxtReal51)
#define real16Compare(operand1, operand2, res)            decDoubleCompare         (res, operand1, operand2, &ctxtReal16)
#define real34Compare(operand1, operand2, res)            decQuadCompare           (res, operand1, operand2, &ctxtReal34)
#define real51Compare(operand1, operand2, res)            decNumberCompare         (res, operand1, operand2, &ctxtReal51)
#define real16Remainder(operand1, operand2, res)          decDoubleRemainder       (res, operand1, operand2, &ctxtReal16)
#define real34Remainder(operand1, operand2, res)          decQuadRemainder         (res, operand1, operand2, &ctxtReal34)
#define real451Remainder(operand1, operand2, res)         decNumberRemainder       (res, operand1, operand2, &ctxtReal451)
#define real16FMA(factor1, factor2, term, res)            decDoubleFMA             (res, factor1,  factor2,  term, &ctxtReal16)
#define real34FMA(factor1, factor2, term, res)            decQuadFMA               (res, factor1,  factor2,  term, &ctxtReal34)
#define real51FMA(factor1, factor2, term, res)            decNumberFMA             (res, factor1,  factor2,  term, &ctxtReal51)



#define real16ChangeSign(operand)                         (operand)->bytes[ 7] ^= 0x80
#define real34ChangeSign(operand)                         (operand)->bytes[15] ^= 0x80
#define real51ChangeSign(operand)                         (operand)->bits      ^= 0x80
#define real16SetPositiveSign(operand)                    (operand)->bytes[ 7] &= 0x7F
#define real34SetPositiveSign(operand)                    (operand)->bytes[15] &= 0x7F
#define real51SetPositiveSign(operand)                    (operand)->bits      &= 0x7F
#define real16SetNegativeSign(operand)                    (operand)->bytes[ 7] |= 0x80
#define real34SetNegativeSign(operand)                    (operand)->bytes[15] |= 0x80
#define real51SetNegativeSign(operand)                    (operand)->bits      |= 0x80


#define complex16ChangeSign(operand)                      {real16ChangeSign((real16_t *)(operand));      real16ChangeSign((real16_t *)((char *)(operand) + REAL16_SIZE));}
#define complex34ChangeSign(operand)                      {real34ChangeSign((real34_t *)(operand));      real34ChangeSign((real34_t *)((char *)(operand) + REAL34_SIZE));}
#define complex16SetPositiveSign(operand)                 {real16SetPositiveSign((real16_t *)(operand)); real16SetPositiveSign((real16_t *)((char *)(operand) + REAL16_SIZE));}
#define complex34SetPositiveSign(operand)                 {real34SetPositiveSign((real34_t *)(operand)); real34SetPositiveSign((real34_t *)((char *)(operand) + REAL34_SIZE));}
#define complex16SetNegativeSign(operand)                 {real16SetNegativeSign((real16_t *)(operand)); real16SetNegativeSign((real16_t *)((char *)(operand) + REAL16_SIZE));}
#define complex34SetNegativeSign(operand)                 {real34SetNegativeSign((real34_t *)(operand)); real34SetNegativeSign((real34_t *)((char *)(operand) + REAL34_SIZE));}


#define real16Copy(source, destination)                   memcpy(destination, source, REAL16_SIZE)
#define real34Copy(source, destination)                   memcpy(destination, source, REAL34_SIZE)
#define real51Copy(source, destination)                   decNumberCopy((decNumber *)(destination), (decNumber *)(source))
#define real451Copy(source, destination)                  decNumberCopy((decNumber *)(destination), (decNumber *)(source))
#define complex16Copy(source, destination)                memcpy(destination, source, COMPLEX16_SIZE)
#define complex34Copy(source, destination)                memcpy(destination, source, COMPLEX34_SIZE)
#define angleCopy(source, destination)                    memcpy(destination, source, ANGLE_SIZE)


#define real51Log10(operand, res)                         decNumberLog10           (res, operand, &ctxtReal51)
#define real51Power(operand1, operand2, res)              decNumberPower           (res, operand1, operand2, &ctxtReal51)
#define real51SquareRoot(operand, res)                    decNumberSquareRoot      (res, operand, &ctxtReal51)
#define real51Exp(operand, res)                           decNumberExp             (res, operand, &ctxtReal51)
#define real51Ln(operand, res)                            decNumberLn              (res, operand, &ctxtReal51)


#define real16Log10(operand, res)                         {real51_t real51;           real16ToReal51(operand,  &real51);                                      real51Log10(&real51, &real51);             real51ToReal16(&real51,  res);}
#define real16Power(operand1, operand2, res)              {real51_t real51a, real51b; real16ToReal51(operand1, &real51a); real16ToReal51(operand2, &real51b); real51Power(&real51a, &real51b, &real51b); real51ToReal16(&real51b, res);}
#define real16SquareRoot(operand, res)                    {real51_t real51;           real16ToReal51(operand,  &real51);                                      real51SquareRoot(&real51, &real51);        real51ToReal16(&real51,  res);}
#define real16Exp(operand, res)                           {real51_t real51;           real16ToReal51(operand,  &real51);                                      real51Exp(&real51, &real51);               real51ToReal16(&real51,  res);}
#define real16Ln(operand, res)                            {real51_t real51;           real16ToReal51(operand,  &real51);                                      real51Ln(&real51, &real51);                real51ToReal16(&real51,  res);}


#define real34Log10(operand, res)                         {real51_t real51;           real34ToReal51(operand,  &real51);                                      real51Log10(&real51, &real51);             real51ToReal34(&real51,  res);}
#define real34Power(operand1, operand2, res)              {real51_t real51a, real51b; real34ToReal51(operand1, &real51a); real34ToReal51(operand2, &real51b); real51Power(&real51a, &real51b, &real51b); real51ToReal34(&real51b, res);}
#define real34SquareRoot(operand, res)                    {real51_t real51;           real34ToReal51(operand,  &real51);                                      real51SquareRoot(&real51, &real51);        real51ToReal34(&real51,  res);}
#define real34Exp(operand, res)                           {real51_t real51;           real34ToReal51(operand,  &real51);                                      real51Exp(&real51, &real51);               real51ToReal34(&real51,  res);}
#define real34Ln(operand, res)                            {real51_t real51;           real34ToReal51(operand,  &real51);                                      real51Ln(&real51, &real51);                real51ToReal34(&real51,  res);}
