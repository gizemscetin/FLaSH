#include "arith.h"

void PolySample(ZZX &poly, const Ring &r)
{
    clear(poly);
    int degree = deg(r.poly_mod);
    poly.SetLength(degree);
	for(int i=0; i<degree; i++)
		poly[i] = RandomBnd(r.coeff_mod);
    poly.normalize();
}

void PolySampleBalanced(ZZX &poly, const Ring &r)
{
    clear(poly);
    int degree = deg(r.poly_mod);
    poly.SetLength(degree);
	for(int i=0; i<degree; i++)
    {
        poly[i] = RandomBnd(2*r.coeff_mod+1);
        poly[i] -= r.coeff_mod;
    }
    poly.normalize();
}

void PolyAddScalar(ZZX &out, const ZZX &in1, int in2, const Ring &r)
{
    // Use NTL ZZ_pX addition
    ZZ_p::init(r.coeff_mod);
	ZZ_pX temp1(to_ZZ_pX(in1));
	temp1 += in2;
	out = to_ZZX(temp1);
}
void PolyAddScalar(ZZX &out, const ZZX &in1, const ZZ &in2, const Ring &r)
{
    // Use NTL ZZ_pX addition
    ZZ_p::init(r.coeff_mod);
	ZZ_pX temp1(to_ZZ_pX(in1));
	temp1 += to_ZZ_p(in2);
	out = to_ZZX(temp1);
}
void PolyAddPoly(ZZX &out, const ZZX &in1, const ZZX &in2, const Ring &r)
{
    // Use NTL ZZ_pX addition
    ZZ_p::init(r.coeff_mod);
	ZZ_pX temp1(to_ZZ_pX(in1));
	ZZ_pX temp2(to_ZZ_pX(in2));
	temp1 += temp2;
	out = to_ZZX(temp1);
}

void PolyMultScalar(ZZX &out, const ZZX &in1, int in2, const Ring &r)
{
    PolyMultScalar(out, in1, to_ZZ(in2), r);
}
void PolyMultScalar(ZZX &out, const ZZX &in1, const ZZ &in2, const Ring &r)
{
    // Use NTL ZZ_pX multiplication
    ZZ_p::init(r.coeff_mod);
	ZZ_pX temp(to_ZZ_pX(in1));
	temp *= to_ZZ_p(in2);
	out = to_ZZX(temp);
}

void PolyMultPoly(ZZX &out, const ZZX &in1, const ZZX &in2, const Ring &r)
{
    // Use NTL ZZ_pE multiplication
    ZZ_p::init(r.coeff_mod);
	ZZ_pX phi;
	phi = to_ZZ_pX(r.poly_mod);
	ZZ_pE::init(phi);

	ZZ_pE temp1, temp2;
	temp1 = to_ZZ_pE(to_ZZ_pX(in1));
	temp2 = to_ZZ_pE(to_ZZ_pX(in2));
	mul(temp1, temp1, temp2);
	out = to_ZZX(rep(temp1));
}

bool PolyInverse(ZZX &out, const ZZX &in, const Ring &r)
{
	ZZ_p::init(r.coeff_mod);
	ZZ_pX phi;
	phi = to_ZZ_pX(r.poly_mod);
	ZZ_pE::init(phi);

	ZZ_pE f_, f_inv_;
	f_ = to_ZZ_pE(to_ZZ_pX(in));
	try{
        f_inv_ = inv(f_);
        ZZ_pX tp = rep(f_inv_);
        out = to_ZZX(tp);
        return true;
	}
	catch(ArithmeticErrorObject obj)
	{
        cout << "Error : could not find an inverse." << endl;
        return false;
    }
}

void PolyReduceCoeff(ZZX &out, const ZZX &in, int mod)
{
    PolyReduceCoeff(out, in, to_ZZ(mod));
}

void PolyReduceCoeff(ZZX &out, const ZZX &in, const ZZ &mod)
{
    ZZ_p::init(mod);
	out = to_ZZX(to_ZZ_pX(in));
}

void PolyBalanceCoeff(ZZX &out, const ZZX &in, const ZZ &mod)
{
    PolyReduceCoeff(out, in, mod);
    for(int i=0; i<=deg(out); i++)
    {
        if(out[i] > (mod-1)/2)
            out[i] -= mod;
    }
    out.normalize();
}

void PolyInit(ZZX &out, int degree, PolyType type)
{
    out = ZZX(INIT_MONO, degree, 1);
    if (type == MonomialPlusOne)
        out[0] = 1;
    else if (type == MonomialMinusOne)
        out[0] = -1;
}








