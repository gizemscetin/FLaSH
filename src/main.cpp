#include "test.h"
#include "batcher.h"
#include <NTL/ZZ_pXFactoring.h>
#include <NTL/ZZ_pEX.h>

using namespace std;
using namespace NTL;


// Test Flash library
int main()
{
    //TestSort();

    //TestBatching();

    CoeffMod q(2);
    PolyMod cyc_poly = FindCyclotomic(31);
    Ring R(q, cyc_poly);
    Batcher B(R);

    vector<long> sigma = B.GetPermutation(6);
    for(int i=0; i<sigma.size(); i++)
    {
        cout << sigma[i] << " -> ";
    }
    cout << endl << endl;




/*
    vec_ZZX A;
    ZZX A_out;

    A.SetLength(6);

    A[0] = 1;
    //A[1] = 0;
    //A[2] = 1;
    //A[3] = 1;
    //A[4] = 0;
    //A[5] = 1;

    //PolyInit(A[0], 1, Monomial);
    PolyInit(A[1], 1, Monomial);
    //PolyInit(A[2], 2, Monomial);
    //PolyInit(A[3], 1, Monomial);
    //PolyInit(A[4], 1, Monomial);
    //PolyInit(A[5], 1, Monomial);


    for(int i=0; i<A.length(); i++)
        cout << A[i] << endl;

    A_out = B.batch(A);
    cout << endl << "I_CRT = " << A_out << endl << endl;


    ZZX x_six;
    PolyInit(x_six, 6, Monomial);
    cout << "x^6 = " << x_six << endl;
    PolyEvaluate(A_out, A_out, x_six, R);

    A = B.unbatch(A_out, B.factor_count());
    for(int i=0; i<A.length(); i++)
        cout << A[i] << endl;
    cout << endl << endl;
*/

/*

    Flash F;
    F.InitParams(true);
    F.InitKeys();
    F.InitCrypter();

    vec_ZZX A, B;
    ZZX A_out, B_out;
    FntruCiphertext A_enc, B_enc;
    Plaintext A_pt, B_pt;

    A.SetLength(3);

    A[0] = 0;
    A[1] = 1;
    //PolyInit(A[1], 1, Monomial);
    //PolyInit(A[2], 2, Monomial);

    F.Batch(A_out, A);
    cout << endl << "I_CRT = " << A_out << endl << endl;

    F.Unbatch(A, A_out, F.slot_count());
    for(int i=0; i<A.length(); i++)
        cout << A[i] << endl;
    cout << endl << endl;
*/

/*
    ZZX x_six;
    PolyInit(x_six, 6, Monomial);
    PolyEvaluate(A_out, A_out, x_six, F.param_generator()->ptext_ring());

    F.Unbatch(A, A_out, F.slot_count());
    for(int i=0; i<A.length(); i++)
        cout << A[i] << endl;

    cout << endl << endl;

    PolyEvaluate(A_out, A_out, x_six, F.param_generator()->ptext_ring());

    F.Unbatch(A, A_out, F.slot_count());
    for(int i=0; i<A.length(); i++)
        cout << A[i] << endl;

    cout << endl << endl;
*/
    // Test
/*
    ZZX x_six;
    PolyInit(x_six, 6, Monomial);
    SecretKey f = F.keys().sec_key;
    cout << f << endl;
    PolyEvaluate(f, f, x_six, F.param_generator()->key_ring());
    cout << f << endl;

    F.Encrypt(A_enc, A_out);
    F.Permute(B_enc, A_enc);
    F.Decrypt(B_pt, B_enc, f);

    F.Unbatch(B, B_pt, F.slot_count());
    for(int i=0; i<B.length(); i++)
        cout << B[i] << endl;
*/
    return 0;
}
