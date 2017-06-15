#include "paramgen.h"

ParamGen::ParamGen(FheType fhe, ParamType type, PtextMod ptext_mod, NoiseBound noise_bound, CircuitDepth d, bool BatchingOn)
{
    if(type == Test)
    {
        PolyDegree n(7);                               // Small degree for testing
        CtextMod q(GenPrime_ZZ(30));                     // small coefficients for faster arithmetic
        if(BatchingOn)
            set_rings(noise_bound, ptext_mod, q, n, Cyclotomic);
        else
            set_rings(noise_bound, ptext_mod, q, n, MonomialPlusOne);
        //FindSmallestCoeffMod(d);                        // check if q is large enough to handle the noise

        block_size_ = 2;
    }
    else if(fhe == leveled)
    {
        PolyDegree n(2*GenGermainPrime_long(12)+1);      // Safe Prime *if leveled LTV is being used*, due to Subfield Attack
        CtextMod q(GenPrime_ZZ(10));                         // small coefficients for faster arithmetic
        set_rings(noise_bound, ptext_mod, q, n, MonomialPlusOne);
        // To do : for leveled there must be a list of values -> {q^{d+1}, q^{d}, ..., q^2, q}

        block_size_ = 1 << 16;
    }
    else
    {
        PolyDegree n(8191);                         // F-ntru has higher noise --> better security
        CtextMod q(GenPrime_ZZ(30));
        if(BatchingOn)
            set_rings(noise_bound, ptext_mod, q, n, Cyclotomic);
        else
            set_rings(noise_bound, ptext_mod, q, n, MonomialPlusOne);
        //FindSmallestCoeffMod(d);

        block_size_ = 2;

        // To do : for secure fntru change the error distributions
        // In Section 5 of https://eprint.iacr.org/2016/315.pdf
        // sigma_key and sigma_err
    }
    block_count_ = ceil(BitLength(ctext_ring().coeff_mod)/BitLength(block_size_));
}

/*
ParamGen::ParamGen(NoiseBound noise_bound, PtextMod ptext_mod, CtextMod ctext_mod, PolyMod poly_mod)
{
    set_rings(noise_bound, ptext_mod, ctext_mod, poly_mod);
}
*/
/*
ParamGen::ParamGen(FheType fhe, NoiseBound noise_bound, PtextMod ptext_mod, CtextMod ctext_mod, PolyDegree deg, PolyType type, int ptext_count)
{
    // To do : check Fhe type
    set_rings(noise_bound, ptext_mod, ctext_mod, deg, type);
    PolyInit(ptext_ring_.poly_mod, ptext_count, Monomial);
}
*/

ParamGen::~ParamGen()
{
    //dtor
}

void ParamGen::set_rings(NoiseBound noise_bound, PtextMod ptext_mod, CtextMod ctext_mod, PolyDegree deg, PolyType type)
{
    PolyMod poly_mod;
    PolyInit(poly_mod, deg, type);
    set_rings(noise_bound, ptext_mod, ctext_mod, poly_mod);
}

void ParamGen::set_rings(NoiseBound noise_bound, PtextMod ptext_mod, CtextMod ctext_mod, PolyMod poly_mod)
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

long ParamGen::FindSmallestCoeffMod(CircuitDepth d, double std_dev, int add_count)
{
    // At the end of decryption, i.e. f*c reduced in the ring
    // fc max norm should be smaller than q/2 to prevent wrap around
    if(d == 0)
    {
        long noise_length = BitLength(ComputeNoiseNoEval());
        long noise_capacity = BitLength(ctext_ring().coeff_mod) - 1;
        cout << "Noise capacity : " << noise_length << "/" << noise_capacity << endl;

        if(noise_length >= noise_capacity)
        {
            noise_capacity = noise_length + 1;
            // New coefficient modulus, aka q:
            CtextMod q(GenPrime_ZZ(noise_capacity + 1));
            ctext_ring_.coeff_mod = q;
            key_ring_.coeff_mod = q;        // Public key uses the same coeff modulus

            cout << "New capacity : " << noise_length << "/" << noise_capacity << endl;
        }
        return BitLength(ctext_ring().coeff_mod);  // q bit length
    }
    return 0;
}
/*
B0fa(n,q,K,B,mu)=mu*((sqrt(n)*aa*B^2 + SM*n*B*w*((SM+1)*B+1)*lq)*K + sqrt(n)*SM*(SM*B+1))
Bia(n,Bim,q,K,B,mu)=mu*((sqrt(n)*aa*Bim^2+SM*n*B*w*((SM+1)*B+1)*lq)*K+sqrt(n)*SM*(SM*B+1))
*/

long ParamGen::ComputeNoiseNoEval()
{
    // fc = pgs + pef + mf
    //    = pgs + p^2ef' + pe + mpf' + m
    //      p*n*(B^2) + n*((p*B)^2) + pB + (p^2)*B + p
    long n = deg(ctext_ring().poly_mod);
    long p = to_long(ptext_ring().coeff_mod);
    long B = to_long(noise_ring().coeff_mod);
    return SqrRoot(n)*p*(pow(B,2)) + SqrRoot(n)*(pow((p*B),2)) + p*B + (pow(p,2))*B + p;
}

long ParamGen::ComputeNoise(double noise, double pi, double beta, double nu, double kappa, double tau, double omega, double adds, double err)
{
    return err*((adds*SqrRoot(nu)*noise*noise + pi*nu*beta*pow(2, omega)*((pi+1)*beta+1)*tau)/(pow(2,kappa)) + SqrRoot(nu)*pi*(pi*beta+1));
}
