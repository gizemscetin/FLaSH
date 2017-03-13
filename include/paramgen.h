#ifndef PARAMGEN_H
#define PARAMGEN_H


#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include "arith.h"

using namespace NTL;

enum ParamType {Test, Secure};
typedef int CircuitDepth;

class ParamGen
{
    public:
        ParamGen();
        virtual ~ParamGen();

        ParamGen(NoiseBound noise_bound, PtextMod ptext_mod, CtextMod ctext_mod, PolyMod poly_mod);
        ParamGen(NoiseBound noise_bound, PtextMod ptext_mod, CtextMod ctext_mod, PolyDegree deg, PolyType type, int ptext_count = 1);

        const Ring& ctext_ring() const { return ctext_ring_; };
        const Ring& noise_ring() const { return noise_ring_; };
        const Ring& ptext_ring() const { return ptext_ring_; };
        const Ring& key_ring() const { return key_ring_; };

        // Check security?

        // Given circuit depth, check noise growth?
        long FindSmallestCoeffMod(CircuitDepth d, double std_dev = 6.0, int add_count = 1);
        long ComputeNoise(double noise, double pi_, double beta_, double nu, double kappa, double tau, double omega = 1, double adds = 1, double err = 6.0);
        long ComputeNoiseNoEval();


    protected:

    private:
        Ring ctext_ring_;   // Z_q/Phi(x)
        Ring noise_ring_;   // Z_B/Phi(x)
        Ring ptext_ring_;   // Z_p/x^k -> where k depends on the ptext encoding
        Ring key_ring_;     // Z_q/Phi(x)
};

#endif // PARAMGEN_H
