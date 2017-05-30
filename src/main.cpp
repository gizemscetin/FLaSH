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



    Flash F;
    F.InitParams(true);
    F.InitKeys();
    F.InitCrypter();

    vec_ZZX A, B;
    ZZX A_out, B_out;
    FntruCiphertext A_enc, B_enc;
    Plaintext A_pt, B_pt;

    A.SetLength(6);

    A[0] = 0;
    A[1] = 1;
    A[2] = 1;
    A[3] = 1;
    A[4] = 1;
    A[5] = 1;

    F.Batch(A_out, A);
    cout << A_out << endl;

    //F.Encrypt(A_enc, A_out);
    //F.Decrypt(A_pt, A_enc);
    //cout << A_pt << endl;

    ZZX x_six;
    PolyInit(x_six, 6, Monomial);
    PolyEvaluate(A_out, A_out, x_six, F.param_generator()->ctext_ring());

    F.Unbatch(A, A_out, F.slot_count());
    for(int i=0; i<A.length(); i++)
        cout << A[i] << endl;

    cout << endl << endl;

    // Test

    //F.Permute(B_enc, A_enc);
    //F.Decrypt(B_pt, B_enc);

    //F.Unbatch(B, B_pt, F.slot_count());
    //for(int i=0; i<B.length(); i++)
    //    cout << B[i] << endl;




    return 0;
}
