#include "arith.h"
#include "flash.h"

using namespace std;
using namespace NTL;


// Test Flash library
int main()
{
    Flash F;
    F.InitParams(Secure);
    F.InitKeys();
    F.InitCrypter();

    Plaintext m(1);
    cout << "Message: " <<  m << endl;
    Ciphertext c;
    Plaintext pt;

    F.encrypter()->Encrypt(c, m);
    //cout << "Ciphertext : " << endl << c << endl;
    F.decrypter()->Decrypt(pt, c);
    cout << "Plaintext : " << pt << endl;

    return 0;
}
