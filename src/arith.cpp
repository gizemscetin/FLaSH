#include "arith.h"

int BlockLength(ZZ in, ZZ block_size)
{
    return ceil(log(in)/log(block_size));
}
int BlockLength(int in, int block_size)
{
    return ceil(log(in)/log(block_size));
}
int BitLength(ZZ in)
{
    return ceil(log(in)/log(2.0));
}
int BitLength(int in)
{
    return ceil(log(in)/log(2.0));
}

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
void PolyAddPoly(vec_ZZX &out, const vec_ZZX &in1, const vec_ZZX &in2, const Ring &r)
{
    out.SetLength(in1.length());
    for(int i=0; i<out.length(); i++)
    {
        PolyAddPoly(out[i], in1[i], in2[i], r);
    }
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

void PolyShiftLeft(ZZX &out, const ZZX &in, int shift_amount, const Ring &r)
{
    out = in << shift_amount;
    out = out % r.poly_mod;
    PolyReduceCoeff(out, out, r.coeff_mod);
}

void PolyShiftLeft(vec_ZZX &out, const vec_ZZX &in, int shift_amount, const Ring &r)
{
    out.SetLength(in.length());
    for(int i=0; i<in.length(); i++)
    {
        PolyShiftLeft(out[i], in[i], shift_amount, r);
    }
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
    else if (type == Cyclotomic)
    {
        out[0] = -1;
        ZZX x_minus_1 = ZZX(INIT_MONO, 1, 1) - 1;
        out /= x_minus_1;
    }
}

/*
** Given p(x), this function returns:
** p(x), 2p(x), 4p(x), 8p(x), ...
*/

void PolyPowersOfTwo(vec_ZZX &out, const ZZX &in, int cnt)
{
    out.kill();
    ZZ factor(1);
    for(int i=0; i<cnt; i++)
    {
        out.append(in*factor);
        factor *= 2;
    }
}


/*
** Given p(x) = a0 + a1x + ... ,
** this function returns:
** p0(x) = a00 + a10x + ...
** p1(x) = a01 + a11x + ...
** p2(x) = a02 + a12x + ...
** where aij is j'th bit of coefficient ai
*/
void PolyBlockDecompose(vec_ZZX &out, const ZZX &in, int block_cnt, int block_size)
{
    out.SetLength(block_cnt);

    int coeff_cnt = deg(in) + 1;
    for(int i=0; i<coeff_cnt; i++)
    {
        ZZ temp_coeff = in[i];
        for(int j=0; j<block_cnt; j++)
        {
            SetCoeff(out[j], i, temp_coeff%block_size);
            temp_coeff = temp_coeff/block_size;
        }
    }
}

void PolyBlockDecomposeInverse(ZZX &out, const vec_ZZX &in, int block_size)
{
    clear(out);
    ZZ factor(1);
    for(int i=0; i<in.length(); i++)
    {
        out += (in[i]*factor);
        factor *= block_size;
    }
}

void PolyVectorDotProduct(ZZX &out, const vec_ZZX &in1, const vec_ZZX &in2, const Ring &r)
{
    clear(out);
    int poly_count = in1.length();
    for(int i=0; i<poly_count; i++)
    {
        ZZX temp;
        PolyMultPoly(temp, in1[i], in2[i], r);
        PolyAddPoly(out, out, temp, r);
    }
}

void PolyEvaluate(ZZX &out, const ZZX &in1, const ZZX &in2, const Ring &r)
{
    ZZ_p::init(r.coeff_mod);
	ZZ_pX phi;
	phi = to_ZZ_pX(r.poly_mod);
	ZZ_pE::init(phi);

	ZZ_pE out_temp, in_temp;
	in_temp = to_ZZ_pE(to_ZZ_pX(in2));


    out_temp = to_ZZ_pE(in1[0]);
    for(int i=1; i<=deg(in1); i++)
    {
        out_temp += (in_temp * to_ZZ_pE(in1[i]));
        in_temp *= to_ZZ_pE(to_ZZ_pX(in2));
    }
    out = to_ZZX(rep(out_temp));
}



