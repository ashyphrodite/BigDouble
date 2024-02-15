#ifndef BIG_DOUBLE_H
#define BIG_DOUBLE_H

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

struct bigdouble_s {
    double mantissa;
    int64_t exponent;
};
typedef struct bigdouble_s BigDouble;


BigDouble BD_New_int(int64_t number);
BigDouble BD_New_double(double number);

void BD_clone(BigDouble *self, BigDouble other);
BigDouble BD_Clone(BigDouble a);

void BD_abs(BigDouble *self);
BigDouble BD_Abs(BigDouble a);

void BD_negate(BigDouble *self);
BigDouble BD_Negate(BigDouble a);

double BD_Sign(BigDouble a);

void BD_round(BigDouble *self, unsigned int precision);
BigDouble BD_Round(BigDouble a, unsigned int precision);

void BD_add(BigDouble *self, BigDouble augend);
BigDouble BD_Add(BigDouble a, BigDouble b);

void BD_subtract(BigDouble *self, BigDouble subtrahend);
BigDouble BD_Subtract(BigDouble a, BigDouble b);

void BD_muliply(BigDouble *self, BigDouble multiplier);
BigDouble BD_Multiply(BigDouble a, BigDouble b);

void BD_reciprocate(BigDouble *self);
BigDouble BD_Reciprocate(BigDouble a);

void BD_divide(BigDouble *self, BigDouble divisor);
BigDouble BD_Divide(BigDouble a, BigDouble b);

int BD_equals(BigDouble a, BigDouble b);

int BD_compareTo(BigDouble a, BigDouble b);

BigDouble BD_Max(BigDouble left, BigDouble right);
BigDouble BD_Min(BigDouble left, BigDouble right);

double BD_Log10_double(BigDouble a);
BigDouble BD_Log10(BigDouble a);

double BD_AbsLog10_double(BigDouble a);
BigDouble BD_AbsLog10(BigDouble a);

double BD_Log2_double(BigDouble a);
BigDouble BD_Log2(BigDouble a);

double BD_Ln_double(BigDouble a);
BigDouble BD_Ln(BigDouble a);

BigDouble BD_Pow10_int(int64_t power);
BigDouble BD_Pow10_double(double power);

void BD_pow_double(BigDouble *self, double power);
BigDouble BD_Pow_double(BigDouble a, double power);

void BD_sqrt(BigDouble *self);
BigDouble BD_Sqrt(BigDouble a);

void BD_clamp(BigDouble *self, BigDouble min, BigDouble max);
BigDouble BD_Clamp(BigDouble a, BigDouble min, BigDouble max);

void BD_clampMin(BigDouble *self, BigDouble min);
BigDouble BD_ClampMin(BigDouble a, BigDouble min);

void BD_clampMax(BigDouble *self, BigDouble max);
BigDouble BD_ClampMax(BigDouble a, BigDouble max);

BigDouble BD_ArraySum(BigDouble *array, size_t size);
BigDouble BD_ArrayProduct(BigDouble *array, size_t size);

#endif