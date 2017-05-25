#ifndef BATCHER_H
#define BATCHER_H

#include "arith.h"
#include <NTL/ZZ_pXFactoring.h>
#include <NTL/ZZ_pX.h>
#include <vector>

using namespace NTL;

class Batcher
{
    public:
        Batcher(const Ring &R);
        virtual ~Batcher();

        int factor_count() { return factors_.length(); };

        ZZX batch(const vector<ZZX> &in);
        ZZX batch(const vec_ZZX &in);
        vec_ZZX unbatch(const ZZX &in);
        vec_ZZX unbatch(const ZZX &in, int cnt);



    protected:

    private:
        ZZ p_;
        ZZ_pX mod_;
        vec_ZZ_pX factors_;
        vec_ZZ_pX n_;
        //vec_ZZ_pX n_inv_;
};

#endif // BATCHER_H
