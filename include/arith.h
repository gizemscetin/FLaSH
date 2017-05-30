#ifndef ARITH_H
#define ARITH_H

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <NTL/ZZ_pE.h>
#include <iostream>

using namespace std;
using namespace NTL;


typedef int PolyDegree;
typedef ZZ NoiseBound;
typedef ZZ PtextMod;
typedef ZZ CtextMod;
typedef ZZ CoeffMod;
typedef ZZX PolyMod;

enum PolyType {Monomial, MonomialPlusOne, MonomialMinusOne, Cyclotomic};
struct Ring
{
    Ring() {}
    Ring(CoeffMod q, PolyMod m) : coeff_mod(q), poly_mod(m) {}
    CoeffMod coeff_mod;     // q in the papers
    PolyMod poly_mod;       // Phi(x) in the papers
};

//int bitlength(int in){return ceil(log(in)/log(2.0));};
int BitLength(int in);
int BitLength(ZZ in);
int BlockLength(int in, int block_size);
int BlockLength(ZZ in, ZZ block_size);

void PolySample(ZZX &poly, const Ring &r);
void PolySampleBalanced(ZZX &poly, const Ring &r);

void PolyAddScalar(ZZX &out, const ZZX &in1, int in2, const Ring &r);
void PolyAddScalar(ZZX &out, const ZZX &in1, const ZZ &in2, const Ring &r);

void PolyAddPoly(ZZX &out, const ZZX &in1, const ZZX &in2, const Ring &r);
void PolyAddPoly(vec_ZZX &out, const vec_ZZX &in1, const vec_ZZX &in2, const Ring &r);

void PolyMultScalar(ZZX &out, const ZZX &in1, int in2, const Ring &r);
void PolyMultScalar(ZZX &out, const ZZX &in1, const ZZ &in2, const Ring &r);

void PolyShiftLeft(ZZX &out, const ZZX &in, int shift_amount, const Ring &r);
void PolyShiftLeft(vec_ZZX &out, const vec_ZZX &in, int shift_amount, const Ring &r);

void PolyMultPoly(ZZX &out, const ZZX &in1, const ZZX &in2, const Ring &r);
bool PolyInverse(ZZX &out, const ZZX &in, const Ring &r);

void PolyReduceCoeff(ZZX &out, const ZZX &in, int mod);
void PolyReduceCoeff(ZZX &out, const ZZX &in, const ZZ &mod);
void PolyBalanceCoeff(ZZX &out, const ZZX &in, const ZZ &mod);

void PolyInit(ZZX &out, int degree, PolyType type);

void PolyPowersOfTwo(vec_ZZX &out, const ZZX &in, int cnt);

void PolyBlockDecompose(vec_ZZX &out, const ZZX &in, int block_cnt, int block_size = 2);
void PolyBlockDecomposeInverse(ZZX &out, const vec_ZZX &in, int block_size = 2);

void PolyVectorDotProduct(ZZX &out, const vec_ZZX &in1, const vec_ZZX &in2, const Ring &r);

void PolyEvaluate(ZZX &out, const ZZX &in1, const ZZX &in2, const Ring &r);


#endif // ARITH_H
