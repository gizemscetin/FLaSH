#include "arith.h"
#include "flash.h"

using namespace std;
using namespace NTL;


// Test Flash library
int main()
{
    Flash F;
    F.InitParams(Test);
    F.InitKeys();
    F.InitCrypter();

    Plaintext m(1);
    cout << "Message: " <<  m << endl;
    Ciphertext c;
    Plaintext pt;

    F.Encrypt(c, m);
    cout << "Ciphertext : " << endl << c << endl;
    F.Decrypt(pt, c);
    cout << "Plaintext : " << pt << endl;


    CiphertextArray cs;
    F.Encrypt(cs, m);
    cout << cs << endl;
    F.Decrypt(pt, cs);
    cout << "Plaintext : " << pt << endl;

    return 0;
}
