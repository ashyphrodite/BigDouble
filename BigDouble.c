#include "BigDouble.h"

static const int8_t MAX_SIGNIFICANT_DIGITS  = 17;
static const int64_t EXPONENT_LIMIT         = INT64_MAX;
static const int16_t DOUBLE_EXP_MAX         = 308;
static const int16_t DOUBLE_EXP_MIN         = -307;
static const double TOLERANCE               = 1E-18;
static const int8_t INT_EXP_MAX             = 9;
static const double NAN_                    = (0.0 / 0.0);
static const double INFINITY_               = (1.0 / 0.0);

const BigDouble BD_NAN                      = {NAN_, DOUBLE_EXP_MIN};
const BigDouble BD_INFINITY                 = {INFINITY_, DOUBLE_EXP_MIN};
const BigDouble BD_ZERO                     = {0.0, 0};
const BigDouble BD_ONE                      = {1.0, 0};
const BigDouble BD_MATH_E                   = {2.718281828459045, 0};

static bool AreEqual(double a, double b) {
    return fabs(a - b) < TOLERANCE;
}

static bool isFinite(double a) {
    return !isnan(a) && !isinf(a);
}

static bool isZero(double a) {
    return fabs(a) < DBL_MIN;
}

static bool isInteger(double a) {
    return isZero(fmod(fabs(a), 1));
}

static bool is10(BigDouble a) {
    return a.exponent == 1 && isZero(a.mantissa - 1);
}

// using lookup table because pow(10, exponent) is inaccurate when exponent is large
// generated using a python script
// shall you change the min and max exponents,use this script to regenerate the table
/*
DOUBLE_EXP_MIN = -307
DOUBLE_EXP_MAX = 308
ELEMENTS_PER_LINE = 30

with open('output.txt', 'w') as f:
    f.write("static const double Powers[] = {\n")
    for i in range(DOUBLE_EXP_MIN, DOUBLE_EXP_MAX + 1, ELEMENTS_PER_LINE):
        f.write("    " + ", ".join(f"1E{j}" for j in range(i, min(i + ELEMENTS_PER_LINE, DOUBLE_EXP_MAX + 1))) + ",\n")
    f.write("};\n")
 */
static const double POWERS[] = {
    1E-307, 1E-306, 1E-305, 1E-304, 1E-303, 1E-302, 1E-301, 1E-300, 1E-299, 1E-298, 1E-297, 1E-296, 1E-295, 1E-294, 1E-293, 1E-292, 1E-291, 1E-290, 1E-289, 1E-288, 1E-287, 1E-286, 1E-285, 1E-284, 1E-283, 1E-282, 1E-281, 1E-280, 1E-279,
    1E-278, 1E-277, 1E-276, 1E-275, 1E-274, 1E-273, 1E-272, 1E-271, 1E-270, 1E-269, 1E-268, 1E-267, 1E-266, 1E-265, 1E-264, 1E-263, 1E-262, 1E-261, 1E-260, 1E-259, 1E-258, 1E-257, 1E-256, 1E-255, 1E-254, 1E-253, 1E-252, 1E-251, 1E-250, 1E-249,
    1E-248, 1E-247, 1E-246, 1E-245, 1E-244, 1E-243, 1E-242, 1E-241, 1E-240, 1E-239, 1E-238, 1E-237, 1E-236, 1E-235, 1E-234, 1E-233, 1E-232, 1E-231, 1E-230, 1E-229, 1E-228, 1E-227, 1E-226, 1E-225, 1E-224, 1E-223, 1E-222, 1E-221, 1E-220, 1E-219,
    1E-218, 1E-217, 1E-216, 1E-215, 1E-214, 1E-213, 1E-212, 1E-211, 1E-210, 1E-209, 1E-208, 1E-207, 1E-206, 1E-205, 1E-204, 1E-203, 1E-202, 1E-201, 1E-200, 1E-199, 1E-198, 1E-197, 1E-196, 1E-195, 1E-194, 1E-193, 1E-192, 1E-191, 1E-190, 1E-189,
    1E-188, 1E-187, 1E-186, 1E-185, 1E-184, 1E-183, 1E-182, 1E-181, 1E-180, 1E-179, 1E-178, 1E-177, 1E-176, 1E-175, 1E-174, 1E-173, 1E-172, 1E-171, 1E-170, 1E-169, 1E-168, 1E-167, 1E-166, 1E-165, 1E-164, 1E-163, 1E-162, 1E-161, 1E-160, 1E-159,
    1E-158, 1E-157, 1E-156, 1E-155, 1E-154, 1E-153, 1E-152, 1E-151, 1E-150, 1E-149, 1E-148, 1E-147, 1E-146, 1E-145, 1E-144, 1E-143, 1E-142, 1E-141, 1E-140, 1E-139, 1E-138, 1E-137, 1E-136, 1E-135, 1E-134, 1E-133, 1E-132, 1E-131, 1E-130, 1E-129,
    1E-128, 1E-127, 1E-126, 1E-125, 1E-124, 1E-123, 1E-122, 1E-121, 1E-120, 1E-119, 1E-118, 1E-117, 1E-116, 1E-115, 1E-114, 1E-113, 1E-112, 1E-111, 1E-110, 1E-109, 1E-108, 1E-107, 1E-106, 1E-105, 1E-104, 1E-103, 1E-102, 1E-101, 1E-100, 1E-99,
    1E-98, 1E-97, 1E-96, 1E-95, 1E-94, 1E-93, 1E-92, 1E-91, 1E-90, 1E-89, 1E-88, 1E-87, 1E-86, 1E-85, 1E-84, 1E-83, 1E-82, 1E-81, 1E-80, 1E-79, 1E-78, 1E-77, 1E-76, 1E-75, 1E-74, 1E-73, 1E-72, 1E-71, 1E-70, 1E-69,
    1E-68, 1E-67, 1E-66, 1E-65, 1E-64, 1E-63, 1E-62, 1E-61, 1E-60, 1E-59, 1E-58, 1E-57, 1E-56, 1E-55, 1E-54, 1E-53, 1E-52, 1E-51, 1E-50, 1E-49, 1E-48, 1E-47, 1E-46, 1E-45, 1E-44, 1E-43, 1E-42, 1E-41, 1E-40, 1E-39,
    1E-38, 1E-37, 1E-36, 1E-35, 1E-34, 1E-33, 1E-32, 1E-31, 1E-30, 1E-29, 1E-28, 1E-27, 1E-26, 1E-25, 1E-24, 1E-23, 1E-22, 1E-21, 1E-20, 1E-19, 1E-18, 1E-17, 1E-16, 1E-15, 1E-14, 1E-13, 1E-12, 1E-11, 1E-10, 1E-9,
    1E-8, 1E-7, 1E-6, 1E-5, 1E-4, 1E-3, 1E-2, 1E-1, 1E0, 1E1, 1E2, 1E3, 1E4, 1E5, 1E6, 1E7, 1E8, 1E9, 1E10, 1E11, 1E12, 1E13, 1E14, 1E15, 1E16, 1E17, 1E18, 1E19, 1E20, 1E21,
    1E22, 1E23, 1E24, 1E25, 1E26, 1E27, 1E28, 1E29, 1E30, 1E31, 1E32, 1E33, 1E34, 1E35, 1E36, 1E37, 1E38, 1E39, 1E40, 1E41, 1E42, 1E43, 1E44, 1E45, 1E46, 1E47, 1E48, 1E49, 1E50, 1E51,
    1E52, 1E53, 1E54, 1E55, 1E56, 1E57, 1E58, 1E59, 1E60, 1E61, 1E62, 1E63, 1E64, 1E65, 1E66, 1E67, 1E68, 1E69, 1E70, 1E71, 1E72, 1E73, 1E74, 1E75, 1E76, 1E77, 1E78, 1E79, 1E80, 1E81,
    1E82, 1E83, 1E84, 1E85, 1E86, 1E87, 1E88, 1E89, 1E90, 1E91, 1E92, 1E93, 1E94, 1E95, 1E96, 1E97, 1E98, 1E99, 1E100, 1E101, 1E102, 1E103, 1E104, 1E105, 1E106, 1E107, 1E108, 1E109, 1E110, 1E111,
    1E112, 1E113, 1E114, 1E115, 1E116, 1E117, 1E118, 1E119, 1E120, 1E121, 1E122, 1E123, 1E124, 1E125, 1E126, 1E127, 1E128, 1E129, 1E130, 1E131, 1E132, 1E133, 1E134, 1E135, 1E136, 1E137, 1E138, 1E139, 1E140, 1E141,
    1E142, 1E143, 1E144, 1E145, 1E146, 1E147, 1E148, 1E149, 1E150, 1E151, 1E152, 1E153, 1E154, 1E155, 1E156, 1E157, 1E158, 1E159, 1E160, 1E161, 1E162, 1E163, 1E164, 1E165, 1E166, 1E167, 1E168, 1E169, 1E170, 1E171,
    1E172, 1E173, 1E174, 1E175, 1E176, 1E177, 1E178, 1E179, 1E180, 1E181, 1E182, 1E183, 1E184, 1E185, 1E186, 1E187, 1E188, 1E189, 1E190, 1E191, 1E192, 1E193, 1E194, 1E195, 1E196, 1E197, 1E198, 1E199, 1E200, 1E201,
    1E202, 1E203, 1E204, 1E205, 1E206, 1E207, 1E208, 1E209, 1E210, 1E211, 1E212, 1E213, 1E214, 1E215, 1E216, 1E217, 1E218, 1E219, 1E220, 1E221, 1E222, 1E223, 1E224, 1E225, 1E226, 1E227, 1E228, 1E229, 1E230, 1E231,
    1E232, 1E233, 1E234, 1E235, 1E236, 1E237, 1E238, 1E239, 1E240, 1E241, 1E242, 1E243, 1E244, 1E245, 1E246, 1E247, 1E248, 1E249, 1E250, 1E251, 1E252, 1E253, 1E254, 1E255, 1E256, 1E257, 1E258, 1E259, 1E260, 1E261,
    1E262, 1E263, 1E264, 1E265, 1E266, 1E267, 1E268, 1E269, 1E270, 1E271, 1E272, 1E273, 1E274, 1E275, 1E276, 1E277, 1E278, 1E279, 1E280, 1E281, 1E282, 1E283, 1E284, 1E285, 1E286, 1E287, 1E288, 1E289, 1E290, 1E291,
    1E292, 1E293, 1E294, 1E295, 1E296, 1E297, 1E298, 1E299, 1E300, 1E301, 1E302, 1E303, 1E304, 1E305, 1E306, 1E307, 1E308,
};

static double powersOf10(int exponent) {
    return POWERS[-DOUBLE_EXP_MIN - 1 + exponent];
}

static void BD_normalize(BigDouble *self) {
    if (self->mantissa >= 1 && self->mantissa < 10) {
        return;
    }
    if (isZero(self->mantissa)) {
        self->mantissa = 0.0;
        self->exponent = 0;
        return;
    }
    int64_t tempEmponext = floor(log10(fabs(self->mantissa)));
    self->mantissa = (tempEmponext == DOUBLE_EXP_MIN)
        ? self->mantissa * 10 / 1E-307
        : self->mantissa / powersOf10(tempEmponext);
    self->exponent += tempEmponext;
}

static BigDouble BD_Normalize(double mantissa, int64_t exponent) {
    BigDouble result = {mantissa, exponent};
    BD_normalize(&result);
    return result;
}

void BD_init(BigDouble *self, double mantissa, int64_t expoenent) {
    self->mantissa = mantissa;
    self->exponent = expoenent;
    BD_normalize(self);
}

BigDouble BD_New(double mantissa, int64_t exponent) {
    BigDouble result = {mantissa, exponent};
    BD_normalize(&result);
    return result;
}

BigDouble BD_New_int(int64_t number) {
    BigDouble result = {number, 0};
    BD_normalize(&result);
    return result;
}

BigDouble BD_New_double(double number) {
    BigDouble result = {number, 0};
    BD_normalize(&result);
    return result;
}

// clone is pointless because you can just do BigDouble a = b
void BD_clone(BigDouble *self, BigDouble other) {
    self->mantissa = other.mantissa;
    self->exponent = other.exponent;
}

BigDouble BD_Clone(BigDouble a) {
    return (BigDouble) {a.mantissa, a.exponent};
}

void BD_abs(BigDouble *self) {
    self->mantissa = abs(self->mantissa);
}

BigDouble BD_Abs(BigDouble a) {
    return (BigDouble) {abs(a.mantissa), a.exponent};
}

void BD_negate(BigDouble *self) {
    self->mantissa = -self->mantissa;
}

BigDouble BD_Negate(BigDouble a) {
    return (BigDouble) {-a.mantissa, a.exponent};
}

double BD_Sign(BigDouble a) {
    return (a.mantissa < 0) ? -1.0 : (a.mantissa > 0) ? 1.0 : 0;
}

void BD_round(BigDouble *self, unsigned int precision) {
    if (self->exponent < -1) {
        self->mantissa = 0;
        self->exponent = 0;
    } else if (self->exponent + precision < MAX_SIGNIFICANT_DIGITS) {
        self->mantissa = round(self->mantissa * powersOf10(self->exponent + precision)) / powersOf10(self->exponent + precision);
    }
}

BigDouble BD_Round(BigDouble a, unsigned int precision) {
    BigDouble result = a;
    BD_round(&result, precision);
    return result;
}

void BD_add(BigDouble *self, BigDouble augend) {
    if (isZero(self->mantissa)) {
        self->mantissa = augend.mantissa;
        self->exponent = augend.exponent;
    } else if (!isZero(augend.mantissa)){
        BigDouble bigger;
        BigDouble smaller;
        if (self->exponent > augend.exponent) {
            bigger = *self;
            smaller = augend;
        } else {
            bigger = augend;
            smaller = *self;
        }

        if (bigger.exponent - smaller.exponent > MAX_SIGNIFICANT_DIGITS) {
            self->mantissa = bigger.mantissa;
            self->exponent = bigger.exponent;
        } else {
            // have to do this because adding numbers that were once integers but scaled down is imprecise.
            // Example: 299 + 18E
            self->mantissa = round(1E14 * bigger.mantissa + 1E14 * smaller.mantissa * powersOf10(smaller.exponent - bigger.exponent));
            self->exponent = bigger.exponent - 14;
            BD_normalize(self);
        }
    }
}

BigDouble BD_Add(BigDouble a, BigDouble b) {
    BigDouble result = a;
    BD_add(&result, b);
    return result;
}

void BD_subtract(BigDouble *self, BigDouble subtrahend) {
    BD_add(self, BD_Negate(subtrahend));
}

BigDouble BD_Subtract(BigDouble a, BigDouble b) {
    BigDouble result = a;
    BD_subtract(&result, b);
    return result;
}

void BD_muliply(BigDouble *self, BigDouble multiplier) {
    self->mantissa *= multiplier.mantissa;
    self->exponent += multiplier.exponent;
    BD_normalize(self);
}

BigDouble BD_Multiply(BigDouble a, BigDouble b) {
    BigDouble result = a;
    BD_muliply(&result, b);
    return result;
}

void BD_reciprocate(BigDouble *self) {
    if (isZero(self->mantissa)) {
        *self = BD_INFINITY;
    } else {
        self->mantissa = 1.0 / self->mantissa;
        self->exponent = -self->exponent;
        BD_normalize(self);
    }
}

BigDouble BD_Reciprocate(BigDouble a) {
    BigDouble result = a;
    BD_reciprocate(&result);
    return result;
}

void BD_divide(BigDouble *self, BigDouble divisor) {
    BD_muliply(self, BD_Reciprocate(divisor));
}

BigDouble BD_Divide(BigDouble a, BigDouble b) {
    BigDouble result = a;
    BD_divide(&result, b);
    return result;
}

int BD_equals(BigDouble a, BigDouble b) {
    return AreEqual(a.mantissa, b.mantissa) && a.exponent == b.exponent;
}

int BD_compareTo(BigDouble a, BigDouble b) {
    if (a.exponent > b.exponent) {
        return 1;
    } else if (a.exponent < b.exponent) {
        return -1;
    } else {
        return (a.mantissa > b.mantissa) ? 1 : (a.mantissa < b.mantissa) ? -1 : 0;
    }
}

static void BD_MaxInPlace(BigDouble *self, BigDouble other) {
    if (BD_compareTo(*self, other)) {
        *self = other;
    }
}

BigDouble BD_Max(BigDouble left, BigDouble right) {
    return (BD_compareTo(left, right) > 0) ? left : right;
}

static void BD_MinInPlace(BigDouble *self, BigDouble other) {
    if (BD_compareTo(*self, other)) {
        *self = other;
    }
}

BigDouble BD_Min(BigDouble left, BigDouble right) {
    return (BD_compareTo(left, right) < 0) ? left : right;
}

double BD_Log10_double(BigDouble a) {
    return a.exponent + log10(a.mantissa);
}

BigDouble BD_Log10(BigDouble a) {
    return BD_New_double(BD_Log10_double(a));
}

double BD_AbsLog10_double(BigDouble a) {
    return a.exponent + log10(fabs(a.mantissa));
}

BigDouble BD_AbsLog10(BigDouble a) {
    return BD_New_double(BD_AbsLog10_double(a));
}

double BD_Log2_double(BigDouble a) {
    return 3.32192809488736234787 * BD_Log10_double(a);
}

BigDouble BD_Log2(BigDouble a) {
    return BD_New_double(BD_Log2_double(a));
}

double BD_Ln_double(BigDouble a) {
    return 2.30258509299404568402 * BD_Log10_double(a);
}

BigDouble BD_Ln(BigDouble a) {
    return BD_New_double(BD_Ln_double(a));
}

BigDouble BD_Pow10_int(int64_t power) {
    return BD_New(1.0, power);
}

BigDouble BD_Pow10_double(double power) {
    if (isInteger(power)) {
        return BD_Pow10_int(power);
    } else {
        return BD_New(NAN_, DOUBLE_EXP_MIN);
    }
}

static void BD_power_internal(BigDouble *self, double power) {
    // UN-SAFETY: Accuracy not guaranteed beyond ~9~11 decimal places.

    // Fast track: If (this.exponent*power) is an integer and mantissa^power fits in a Number, we can do a very fast method.
    double temp = self->exponent * power;
    double newMantissa;
    if (isInteger(temp) && isFinite(temp) && abs(temp) < EXPONENT_LIMIT) {
        newMantissa = pow(self->mantissa, power);
        if (isFinite(newMantissa)) {
            self->mantissa = newMantissa;
            self->exponent = (int64_t) temp;
            BD_normalize(self);
            return;
        }
    }
    // Same speed and usually more accurate. (An arbitrary-precision version of this calculation is used in break_break_infinity.js, sacrificing performance for utter accuracy.)
    double newExponent = (temp >= 0)
        ? floor(temp)
        : ceil(temp);
    double residue = temp - newExponent;
    newMantissa = pow(10, power * log10(self->mantissa) + residue);
    if (isFinite(newMantissa)) {
        self->mantissa = newMantissa;
        self->exponent = (int64_t) newExponent;
        BD_normalize(self);
    } else {
        //UN-SAFETY: This should return NaN when mantissa is negative and value is noninteger.
        BigDouble result = BD_Pow10_double(power * BD_AbsLog10_double(*self));
        self->mantissa = (BD_Sign(*self) < 0 && AreEqual(fmod(power, 2), 1))
            ? -result.mantissa
            : result.mantissa;
        self->exponent = result.exponent;
    }
}

void BD_pow_double(BigDouble *self, double power) {
    bool powerIsInteger = isInteger(power);
    if (BD_compareTo(*self, BD_ZERO) == 0 && !powerIsInteger) {
        self->mantissa = BD_NAN.mantissa;
        self->exponent = BD_NAN.exponent;
    } else {
        if (is10(*self) && powerIsInteger) {
            BigDouble result = BD_Pow10_double(power);
            self->mantissa = result.mantissa;
            self->exponent = result.exponent;
        } else {
            BD_power_internal(self, power);
        }
            
    }
}

void BD_sqrt(BigDouble *self) {
    if (self->mantissa < 0) {
        self->mantissa = NAN_;
        self->exponent = DOUBLE_EXP_MIN;
    } else if (self->exponent % 2 != 0) {
        self->mantissa = sqrt(self->mantissa) * 3.1622776601683795;
        self->exponent = floor(self->exponent / 2);
    } else {
        self->mantissa = sqrt(self->mantissa);
        self->exponent = floor(self->exponent / 2);
    }
}

BigDouble BD_Sqrt(BigDouble a) {
    BigDouble result = a;
    BD_sqrt(&result);
    return result;
}

void BD_clamp(BigDouble *self, BigDouble min, BigDouble max) {
    // use BD_min and BD_max
    if (BD_compareTo(*self, min) < 0) {
        *self = min;
    } else if (BD_compareTo(*self, max) > 0) {
        *self = max;
    }
}

BigDouble BD_Clamp(BigDouble a, BigDouble min, BigDouble max) {
    BigDouble result = a;
    BD_clamp(&result, min, max);
    return result;
}

void BD_clampMin(BigDouble *self, BigDouble min) {
    if (BD_compareTo(*self, min) < 0) {
        *self = min;
    }
}

BigDouble BD_ClampMin(BigDouble a, BigDouble min) {
    BigDouble result = a;
    BD_clampMin(&result, min);
    return result;
}

void BD_clampMax(BigDouble *self, BigDouble max) {
    if (BD_compareTo(*self, max) > 0) {
        *self = max;
    }
}

BigDouble BD_ClampMax(BigDouble a, BigDouble max) {
    BigDouble result = a;
    BD_clampMax(&result, max);
    return result;
}

BigDouble BD_Pow_double(BigDouble a, double power) {
    BigDouble result = a;
    BD_pow_double(&result, power);
    return a;
}

BigDouble BD_ArraySum(BigDouble *array, size_t size) {
    BigDouble result = BD_ZERO;
    size_t i;
    for (i = 0; i < size; i++) {
        BD_add(&result, array[i]);
    }
    return result;
}

BigDouble BD_ArrayProduct(BigDouble *array, size_t size) {
    BigDouble result = BD_ONE;
    size_t i;
    for (i = 0; i < size; i++) {
        BD_muliply(&result, array[i]);
    }
    return result;
}
