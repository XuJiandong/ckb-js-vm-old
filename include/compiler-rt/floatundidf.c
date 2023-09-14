#define DOUBLE_PRECISION

#include "fp_lib.h"

// Support for systems that don't have hardware floating-point; there are no
// flags to set, and we don't want to code-gen to an unknown soft-float
// implementation.

COMPILER_RT_ABI double __floatundidf(du_int a) {
    if (a == 0) return 0.0;
    const unsigned N = sizeof(du_int) * CHAR_BIT;
    int sd = N - __builtin_clzll(a);  // number of significant digits
    int e = sd - 1;                   // exponent
    if (sd > DBL_MANT_DIG) {
        //  start:
        //  0000000000000000000001xxxxxxxxxxxxxxxxxxxxxxPQxxxxxxxxxxxxxxxxxx
        //  finish:
        //  000000000000000000000000000000000000001xxxxxxxxxxxxxxxxxxxxxxPQR
        //                                                12345678901234567890123456
        //  1 = msb 1 bit
        //  P = bit DBL_MANT_DIG-1 bits to the right of 1
        //  Q = bit DBL_MANT_DIG bits to the right of 1
        //  R = "or" of all bits to the right of Q
        switch (sd) {
            case DBL_MANT_DIG + 1:
                a <<= 1;
                break;
            case DBL_MANT_DIG + 2:
                break;
            default:
                a = (a >> (sd - (DBL_MANT_DIG + 2))) |
                    ((a & ((du_int)(-1) >> ((N + DBL_MANT_DIG + 2) - sd))) !=
                     0);
        };
        // finish:
        a |= (a & 4) != 0;  // Or P into R
        ++a;                // round - this step may add a significant bit
        a >>= 2;            // dump Q and R
        // a is now rounded to DBL_MANT_DIG or DBL_MANT_DIG+1 bits
        if (a & ((du_int)1 << DBL_MANT_DIG)) {
            a >>= 1;
            ++e;
        }
        // a is now rounded to DBL_MANT_DIG bits
    } else {
        a <<= (DBL_MANT_DIG - sd);
        // a is now rounded to DBL_MANT_DIG bits
    }
    double_bits fb;
    fb.u.s.high = ((su_int)(e + 1023) << 20) |       // exponent
                  ((su_int)(a >> 32) & 0x000FFFFF);  // mantissa-high
    fb.u.s.low = (su_int)a;                          // mantissa-low
    return fb.f;
}
