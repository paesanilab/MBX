/* Copyright (C) 1997 Makoto Matsumoto and Takuji Nishimura.       */
/* Any feedback is very welcome. For any question, comments,       */
/* see http://www.math.keio.ac.jp/matumoto/emt.html or email       */
/* matumoto@math.keio.ac.jp                                        */

#include <cmath>
#include <cassert>

#include "mt19937.h"

/* Period parameters */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0df   /* constant vector a */
#define UPPER_MASK 0x80000000 /* most significant w-r bits */
#define LOWER_MASK 0x7fffffff /* least significant r bits */

/* Tempering parameters */
#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000
#define TEMPERING_SHIFT_U(y) (y >> 11)
#define TEMPERING_SHIFT_S(y) (y << 7)
#define TEMPERING_SHIFT_T(y) (y << 15)
#define TEMPERING_SHIFT_L(y) (y >> 18)

namespace kit {

///////////////////////////////////////////////////////////////////////////////

mt19937::mt19937() throw() { seed(data_type(4357)); }

///////////////////////////////////////////////////////////////////////////////

mt19937::mt19937(data_type s) throw() { seed(s); }

///////////////////////////////////////////////////////////////////////////////

void mt19937::seed(data_type n) throw() {
    if (n == 0) n = data_type(0x6b842129);  // from glib-2.0.6

    /* setting initial seeds to mt[N] using         */
    /* the generator Line 25 of Table 1 in          */
    /* [KNUTH 1981, The Art of Computer Programming */
    /*    Vol. 2 (2nd Ed.), pp102]                  */

    mt[0] = n & (~0);
    for (mti = 1; mti < N; ++mti) mt[mti] = (69069 * mt[mti - 1]) & (~0);
}

///////////////////////////////////////////////////////////////////////////////

mt19937::data_type mt19937::operator()() throw() {
    /* mag01[x] = x * MATRIX_A  for x=0,1 */
    static data_type mag01[2] = {0, MATRIX_A};
    data_type y;

    if (mti >= N) { /* generate N words at one time */

        int kk(0);

        for (; kk < N - M; ++kk) {
            y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
            mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1];
        }

        for (; kk < N - 1; ++kk) {
            y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
            mt[kk] = mt[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1];
        }

        y = (mt[N - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
        mt[N - 1] = mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1];

        mti = 0;
    }

    y = mt[mti++];
    y ^= TEMPERING_SHIFT_U(y);
    y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
    y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
    y ^= TEMPERING_SHIFT_L(y);

    return y;
}

///////////////////////////////////////////////////////////////////////////////

mt19937::data_type mt19937::operator()(data_type begin, data_type end) throw() {
    return data_type((*this)(double(begin), double(end)));
}

///////////////////////////////////////////////////////////////////////////////

double mt19937::random_double() throw() {
    data_type a = (*this)() >> 5;
    data_type b = (*this)() >> 6;

    return (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

///////////////////////////////////////////////////////////////////////////////

double mt19937::random_gaussian() throw() {
    static double surplus;
    static bool surplus_ready(false);

    if (surplus_ready) {
        surplus_ready = false;
        return surplus;
    } else {
        double x1, x2, w;

        do {
            x1 = 2 * random_double() - 1.0;
            x2 = 2 * random_double() - 1.0;
            w = x1 * x1 + x2 * x2;
        } while (w >= 1.0);

        w = std::sqrt((-2 * std::log(w)) / w);

        surplus = x1 * w;
        surplus_ready = true;

        return x2 * w;
    }
}

///////////////////////////////////////////////////////////////////////////////

double mt19937::operator()(const double& begin, const double& end) throw() {
    return (end - begin) * random_double() + begin;
}

///////////////////////////////////////////////////////////////////////////////

}  // namespace kit
