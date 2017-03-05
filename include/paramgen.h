#ifndef PARAMGEN_H
#define PARAMGEN_H


#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include "arith.h"

using namespace NTL;



class ParamGen
{
    public:
        ParamGen();
        virtual ~ParamGen();

        ParamGen(NoiseBound noise_bound, PtextMod ptext_mod, CtextMod ctext_mod, PolyMod poly_mod);
        ParamGen(NoiseBound noise_bound, PtextMod ptext_mod, CtextMod ctext_mod, PolyDegree deg, PolyType type);

        const Ring& ctext_ring() const { return ctext_ring_; };
        const Ring& noise_ring() const { return noise_ring_; };
        const Ring& ptext_ring() const { return ptext_ring_; };
        const Ring& key_ring() const { return key_ring_; };

        // Check Security?


    protected:

    private:
        Ring ctext_ring_;
        Ring noise_ring_;
        Ring ptext_ring_;
        Ring key_ring_;     // Same as ctext ring, but still...
};

#endif // PARAMGEN_H
