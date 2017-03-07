#include "paramgen.h"

ParamGen::ParamGen()
{
    //ctor
}

ParamGen::ParamGen(NoiseBound noise_bound, PtextMod ptext_mod, CtextMod ctext_mod, PolyMod poly_mod)
{
    noise_ring_.coeff_mod = noise_bound;
    ptext_ring_.coeff_mod = ptext_mod;
    ctext_ring_.coeff_mod = ctext_mod;
    key_ring_.coeff_mod = ctext_mod;

    noise_ring_.poly_mod = poly_mod;
    ptext_ring_.poly_mod = poly_mod;
    ctext_ring_.poly_mod = poly_mod;
    key_ring_.poly_mod = poly_mod;
}

ParamGen::ParamGen(NoiseBound noise_bound, PtextMod ptext_mod, CtextMod ctext_mod, PolyDegree deg, PolyType type)
{
    noise_ring_.coeff_mod = noise_bound;
    ptext_ring_.coeff_mod = ptext_mod;
    ctext_ring_.coeff_mod = ctext_mod;
    key_ring_.coeff_mod = ctext_mod;

    PolyInit(ctext_ring_.poly_mod, deg, type);
    noise_ring_.poly_mod = ctext_ring_.poly_mod;
    ptext_ring_.poly_mod = ctext_ring_.poly_mod;
    key_ring_.poly_mod = ctext_ring_.poly_mod;
}


ParamGen::~ParamGen()
{
    //dtor
}

void ParamGen::FindSmallestCoeffMod(CircuitDepth d)
{
    // fc max norm should be smaller than q/2 to prevent wrap around
    ComputeNorm(d);

    // noisea[0]=B0fa(nn,qq, BB,KK,tdd)
    // B0fa(n,q,K,B,mu)=mu*( (sqrt(n)*aa*B^2 + SM*n*B*w*((SM+1)*B+1)*lq)*K + sqrt(n)*SM*(SM*B+1) )
}


/*
** This method finds the norm after decryption
*/
void ParamGen::ComputeNorm(CircuitDepth d)
{
    // We need to find the initial norm
    // fc = pgs + pef + mf
    //      n*p*(B^2) + n*((p*B)^2) + (p^2)*B
    long norm, n1, n2, p, B;
    n1 = deg(key_ring_.poly_mod);
    n2 = deg(noise_ring_.poly_mod);
}
