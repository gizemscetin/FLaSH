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
