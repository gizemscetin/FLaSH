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
    m_ = deg(R.poly_mod) + 1; // Assuming "m" is a prime
    long d = log(m_+1)/log(to_double(p_));
    //long d = 1;

    EDF(factors_, mod_, h, d);

    // Quick Check
    //cout << "d * t = " << d * factor_count() << endl;
    //cout << "Cyclotomic = " << R.poly_mod << endl;
    //cout << "Factor count = " << factor_count() << endl
    //     << "Factor degree = " << d << endl;
    //for(int i=0; i<factors_.length(); i++)
    //{
    //   cout << "F_" << i << " = " << factors_[i] << endl;
    //}

    n_.SetLength(factors_.length());

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

vec_ZZX Batcher::unbatch(const ZZX &in)
{
    return unbatch(in, factors_.length());
}

vec_ZZX Batcher::ApplyMappings(const vec_ZZX &in, long j)
{
    vector<long> permutation = GetPermutation(j);

    ZZX x_j;
    long pow_j = 1;

    vec_ZZX out;
    out.SetLength(in.length());
    out[0] = in[0];

    for(int i = 1; i<in.length(); i++)
    {
        j_powers_.push_back(pow_j);

        pow_j = (pow_j * j) % m_;

        PolyInit(x_j, pow_j, Monomial);
        Ring R(p_, to_ZZX(factors_[permutation[i]]));
        PolyEvaluate(out[permutation[i]], in[i], x_j, R);
        //cout << "Perm! " << permutation[i] << " : " << out[permutation[i]] << endl;
    }

    for(int i=in.length()-1; i<permutation.size(); i++)
    {
        j_powers_.push_back(pow_j);
        pow_j = (pow_j * j) % m_;
    }

    return out;
}


vec_ZZX Batcher::ApplyInverseMappings(const vec_ZZX &in, long j)
{
    vec_ZZX out;
    out.SetLength(in.length());
    out[0] = in[0];

    ZZX x_j;
    Ring R(p_, to_ZZX(factors_[0]));
    for(int i=1; i<in.length(); i++)
    {
        PolyInit(x_j, j_powers_[j_powers_.size()-i], Monomial);
        PolyEvaluate(out[i], in[i], x_j, R);
    }
    return out;
}

vector<long> Batcher::GetPermutation(long j)
{
    ZZX x_j;
    PolyInit(x_j, j, Monomial);

    Ring R(p_, to_ZZX(mod_));

    vector<long> out(factors_.length());

    int index = 0;
    out[0] = 0;

    for(int i=1; i<factors_.length(); i++)
    {
        ZZX temp = to_ZZX(n_[index]);
        PolyEvaluate(temp, temp, x_j, R);

        vec_ZZX permuted = unbatch(temp);
        for(int j=0; j<permuted.length(); j++)
        {
            if(permuted[j] == 1)
            {
                out[i] = j;
                index = j;
                break;
            }
        }
    }

    return out;
}

void Batcher::ReorderFactorsForRightCyclicShift(long j)
{
    vec_ZZ_pX factors = factors_;
    vec_ZZ_pX n = n_;
    vector<long> permutation = GetPermutation(j);
    for(int i=0; i<permutation.size(); i++)
    {
        factors_[i] = factors[permutation[i]];
        n_[i] = n[permutation[i]];
    }
}


Batcher::~Batcher()
{
    //dtor
}
