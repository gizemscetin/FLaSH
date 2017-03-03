#include "homlib.h"

using namespace std;
using namespace NTL;


// Test Flash library
int main()
{

    LaSH L;

    ZZX m, b;

    CipherText c, result;

    m = 1;
    L.Encrypt(c, m);

    L.Mult(c, c);

    L.Decrypt(b, c);
    cout << c << endl;
cout << b << endl;

    return 0;
}
