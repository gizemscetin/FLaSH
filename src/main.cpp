#include "arith.h"
#include "flash.h"

using namespace std;
using namespace NTL;


// Test Flash library
int main()
{
    Flash F;
    F.InitParams();
    F.InitKeys();
    F.InitCrypter();

    Plaintext m(1);
    cout << "Message: " <<  m << endl;
    Ciphertext c;
    Plaintext pt;

    F.Encrypt(c, m);
    //cout << "Ciphertext : " << endl << c << endl;
    F.Decrypt(pt, c);
    cout << "Plaintext : " << pt << endl;

    FntruCiphertext cs;
    F.Encrypt(cs, m);
    //cout << cs << endl;
    F.Decrypt(pt, cs);
    cout << "Plaintext : " << pt << endl;

    FntruCiphertext test_and;
    F.AND(test_and, cs, cs);
    //cout << test_and << endl;
    F.Decrypt(pt, test_and);
    cout << "AND : " << pt << endl;

    FntruCiphertext test_xor;
    F.XOR(test_xor, cs, cs);
    //cout << test_xor << endl;
    F.Decrypt(pt, test_xor);
    cout << "XOR : " << pt << endl;

    FntruCiphertext test_not;
    F.NOT(test_not, test_xor);
    //cout << test_xor << endl;
    F.Decrypt(pt, test_not);
    cout << "XNOR : " << pt << endl;

    CiphertextArray byte_test;
    F.Encrypt(byte_test, 10);
    PlaintextArray byte_msg;
    F.Decrypt(byte_msg, byte_test);
    cout << "Bits : " << byte_msg << endl;
    PolyBlockDecomposeInverse(pt, byte_msg);
    cout << "Message : " << pt << endl;

    return 0;
}
