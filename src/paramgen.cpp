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

ParamGen::ParamGen(NoiseBound noise_bound, PtextMod ptext_mod, CtextMod ctext_mod, PolyDegree deg, PolyType type, int ptext_count)
{
    noise_ring_.coeff_mod = noise_bound;
    ptext_ring_.coeff_mod = ptext_mod;
    ctext_ring_.coeff_mod = ctext_mod;
    key_ring_.coeff_mod = ctext_mod;

    PolyInit(ctext_ring_.poly_mod, deg, type);
    noise_ring_.poly_mod = ctext_ring_.poly_mod;
    PolyInit(ptext_ring_.poly_mod, ptext_count, Monomial);
    key_ring_.poly_mod = ctext_ring_.poly_mod;
}


ParamGen::~ParamGen()
{
    //dtor
}

long ParamGen::FindSmallestCoeffMod(CircuitDepth d, double std_dev, int add_count)
{
    // At the end of decryption, i.e. f*c reduced in the ring
    // fc max norm should be smaller than q/2 to prevent wrap around
    if(d == 0)
    {
        long noise_length = ceil(log(ComputeNoiseNoEval())/log(2.0));
        long noise_capacity = ceil(log(ctext_ring().coeff_mod)/log(2.0)) - 1;
        //cout << "q : " << ctext_ring().coeff_mod << endl;
        cout << "Noise capacity : " << noise_length << "/" << noise_capacity << endl;

        if(noise_length >= noise_capacity)
        {
            noise_capacity = noise_length + 1;
            // New coefficient modulus, aka q:
            CtextMod q(GenPrime_ZZ(noise_capacity + 1));
            ctext_ring_.coeff_mod = q;
            key_ring_.coeff_mod = q;        // Public key uses the same coeff modulus

            //cout << "New q : " << q << endl;
            cout << "New capacity : " << noise_length << "/" << noise_capacity << endl;
        }
        return ceil(log(ctext_ring().coeff_mod)/log(2.0));  // q bit length
    }
    if(d == 1)
    {

    }
}



/*    double old_noise = to_long(noise_ring().coeff_mod);
    cout << "Initial noise = " << old_noise << endl;

    int block_length = 1;
    int coefficient_length = log(ctext_ring().coeff_mod)/log(2.0) + 1;
    cout << "Cutting q = " << ctext_ring().coeff_mod << " and bit size = " << coefficient_length << " bits" << endl;
    int current_coefficient_length = d*coefficient_length + coefficient_length;
    current_coefficient_length = (current_coefficient_length + block_length - 1)/block_length;
    cout << "Current q bit size = " << current_coefficient_length << " bits" << endl;

    double poly_degree = deg(ctext_ring().poly_mod);
    cout << "Degree = " << poly_degree << endl;

    double ptext_mod = to_long(ptext_ring().coeff_mod);
    double noise_mod = to_long(noise_ring().coeff_mod);

    double new_noise = ComputeNoise(old_noise, ptext_mod, noise_mod, poly_degree, coefficient_length, current_coefficient_length, block_length, add_count, std_dev);
    cout << "Final noise = " << new_noise << endl;
    // noisea[0]=B0fa(nn,qq, BB,KK,tdd)
    // B0fa(n,q,K,B,mu)=mu*( (sqrt(n)*aa*B^2 + SM*n*B*w*((SM+1)*B+1)*lq)*K + sqrt(n)*SM*(SM*B+1) )
    */
}

long ParamGen::ComputeNoiseNoEval()
{
    // fc = pgs + pef + mf
    //      n*p*(B^2) + n*((p*B)^2) + (p^2)*B
    long n = deg(ctext_ring().poly_mod);
    long p = to_long(ptext_ring().coeff_mod);
    long B = to_long(noise_ring().coeff_mod);
    return SqrRoot(n)*p*(pow(B,2)) + SqrRoot(n)*(pow((p*B),2)) + (pow(p,2))*B;
}
/*
B0fa(n,q,K,B,mu)=mu*((sqrt(n)*aa*B^2 + SM*n*B*w*((SM+1)*B+1)*lq)*K + sqrt(n)*SM*(SM*B+1))
Bia(n,Bim,q,K,B,mu)=mu*((sqrt(n)*aa*Bim^2+SM*n*B*w*((SM+1)*B+1)*lq)*K+sqrt(n)*SM*(SM*B+1))
*/

long ParamGen::ComputeNoise(double noise, double pi, double beta, double nu, double kappa, double tau, double omega, double adds, double err)
{
    return err*((adds*SqrRoot(nu)*noise*noise + pi*nu*beta*pow(2, omega)*((pi+1)*beta+1)*tau)/(pow(2,kappa)) + SqrRoot(nu)*pi*(pi*beta+1));
}
