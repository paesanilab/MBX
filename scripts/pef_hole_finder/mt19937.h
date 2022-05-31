#ifndef MT19937_H
#define MT19937_H

#include <string>
#include <stdint.h>

namespace kit {

/* Copyright (C) 1997 Makoto Matsumoto and Takuji Nishimura.       */
/* Any feedback is very welcome. For any question, comments,       */
/* see http://www.math.keio.ac.jp/matumoto/emt.html or email       */
/* matumoto@math.keio.ac.jp                                        */

struct mt19937 {
    typedef uint32_t data_type;

    mt19937() throw();
    explicit mt19937(data_type) throw();

    void seed(data_type) throw();

    data_type operator()() throw();
    data_type operator()(data_type, data_type) throw();
    double operator()(const double&, const double&) throw();  // (0.0, 1.0]

    double random_double() throw();
    double random_gaussian() throw();  // FIXTHIS

   private:
    int32_t mti;
    data_type mt[624];
};

}  // namespace kit

#endif  // MT19937_H
