#include "batcher.h"

Batcher::Batcher(const Ring &R)
{
    //ctor
    p_ = R.coeff_mod;
    ZZX helper_poly;
    PolyInit(helper_poly, to_long(R.coeff_mod), Monomial);
    ZZ_p::init(R.coeff_mod);
    mod_ = to_ZZ_pX(R.poly_mod);
    ZZ_pX h = to_ZZ_pX(helper_poly);
    // (m = cyc degree) "m | 2^d - 1"
    long m = deg(R.poly_mod) + 1; // Assuming "m" is a prime
    long d = log(m+1)/log(2.0);

    EDF(factors_, mod_, h, d);

    // Quick Check
    //cout << "d * t = " << d * factor_count() << endl;

    n_.SetLength(factors_.length());
    //n_inv_.SetLength(factors_.length());

    for(int i=0; i<factors_.length(); i++)
    {
        n_[i] = mod_/factors_[i];
        ZZ_pE::init(factors_[i]);
        ZZ_pE f, f_inv;
        f = to_ZZ_pE(n_[i]);
        f_inv = inv(f);
        n_[i] *= rep(f_inv);
    }
}


ZZX Batcher::batch(const vector<ZZX> &in)
{
    ZZ_p::init(p_);
    ZZ_pE::init(mod_);

    ZZ_pX sum(0);
    for(int i=0; i<in.size(); i++)
    {
        sum += (to_ZZ_pX(in[i]) * n_[i]);
    }
    ZZ_pE out = to_ZZ_pE(sum);
    return to_ZZX(rep(out));
}

ZZX Batcher::batch(const vec_ZZX &in)
{
    ZZ_p::init(p_);
    ZZ_pE::init(mod_);

    ZZ_pX sum(0);
    for(int i=0; i<in.length(); i++)
    {
        sum += (to_ZZ_pX(in[i]) * n_[i]);
    }
    ZZ_pE out = to_ZZ_pE(sum);
    return to_ZZX(rep(out));
}
vec_ZZX Batcher::unbatch(const ZZX &in, int cnt)
{
    vec_ZZX out;
    ZZ_p::init(p_);
    for(int i=0; i<cnt; i++)
    {
        ZZ_pE::init(factors_[i]);
        ZZ_pE temp = to_ZZ_pE(to_ZZ_pX(in));
        out.append(to_ZZX(rep(temp)));
    }
    return out;
}


Batcher::~Batcher()
{
    //dtor
}