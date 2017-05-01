#ifndef TEST_H
#define TEST_H

#include "flash.h"

using namespace std;
using namespace NTL;

typedef vector<FntruCiphertext> FntruVector;
typedef vector<FntruVector> FntruMatrix;


void TestBitEncryptDecrypt();
void TestBitAND();
void TestBitXOR();
void TestBitNAND();
void TestBitEQ();
void TestBitLT();
void TestBitSXL();

void TestByteEncryptDecrypt();
void TestByteEQ();
void TestByteLT();

void TestSort(ParamType type = Secure);
void TestRank();
void TestDirectSort();
void TestGreedySort();
void TestPolyRankSort();
void TestOddEvenMergeSort();


void GetRandomEncryptions(FntruMatrix &out, int base = 256, int count = 1);

#endif
