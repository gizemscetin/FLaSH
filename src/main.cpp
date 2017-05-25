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

    vec_ZZX A, B;
    ZZX A_out, B_out;

    A.SetLength(2);

    A[0] = 1;
    A[1] = 0;

    F.Batch(A_out, A);
    cout << A_out << endl;

    ZZX one;
    PolyInit(one, 1, Monomial);
    PolyMultPoly(A_out, A_out, one, F.param_generator()->ptext_ring());
    cout << A_out << endl;
    //cout << F.param_generator()->ptext_ring().poly_mod << endl;

    F.Unbatch(B, A_out, F.slot_count());
    for(int i=0; i<B.length(); i++)
        cout << B[i] << endl;


    return 0;
}
