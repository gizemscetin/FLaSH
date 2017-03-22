#include "test.h"

void TestBitEncryptDecrypt()
{
    Flash F;
    F.InitParams();
    F.InitKeys();
    F.InitCrypter();

    Plaintext m(1);
    cout << "Message: " <<  m << endl;
    FntruCiphertext ct;
    Plaintext pt;

    F.Encrypt(ct, m);
    F.Decrypt(pt, ct);
    cout << "Decrypted : " << pt << endl;
}
void TestBitAND()
{
    Flash F;
    F.InitParams();
    F.InitKeys();
    F.InitCrypter();

    Plaintext m1(rand()%2);
    Plaintext m2(rand()%2);
    FntruCiphertext ct, ct1, ct2;
    Plaintext pt, pt1, pt2;

    F.AND(ct, ct1, ct2);
    F.Decrypt(pt, ct);
    cout << m1 << " AND " << m2 << " : " << pt << endl;
}
void TestBitXOR()
{
    Flash F;
    F.InitParams();
    F.InitKeys();
    F.InitCrypter();

    Plaintext m1(rand()%2);
    Plaintext m2(rand()%2);
    FntruCiphertext ct, ct1, ct2;
    Plaintext pt, pt1, pt2;

    F.XOR(ct, ct1, ct2);
    F.Decrypt(pt, ct);
    cout << m1 << " XOR " << m2 << " : " << pt << endl;
}
void TestBitNAND()
{

}
void TestBitEQ()
{

}
void TestBitLT()
{

}

void TestBitSXL()
{
    Flash F;
    F.InitParams();
    F.InitKeys();
    F.InitCrypter();

    Plaintext m(1);
    cout << "Message: " <<  m << endl;
    FntruCiphertext ct;
    Plaintext pt;

    F.Encrypt(ct, m);
    F.SXL(ct, ct, 1);
    F.Decrypt(pt, ct);
    cout << "Decrypted : " << pt << endl;
}

void TestByteEncryptDecrypt()
{
    Flash F;
    F.InitParams();
    F.InitKeys();
    F.InitCrypter();

    int m = rand()%256;
    FntruCiphertextArray ct_list;
    F.Encrypt(ct_list, m);
    PlaintextArray pt_list;
    F.Decrypt(pt_list, ct_list);
    Plaintext pt;
    PolyBlockDecomposeInverse(pt, pt_list);
    cout << "Decrypted : " << pt << endl;
}
void TestByteEQ()
{
    Flash F;
    F.InitParams();
    F.InitKeys();
    F.InitCrypter();

    int m1 = rand()%256; int m2 = rand()%256;
    FntruCiphertextArray ct1, ct2;
    F.Encrypt(ct1, m1);
    F.Encrypt(ct2, m2);
    FntruCiphertext ct;
    F.EQ(ct, ct1, ct2);
    Plaintext pt;
    F.Decrypt(pt, ct);
    cout << m1 << " = " << m2 << " : " << pt << endl;
}
void TestByteLT()
{
    Flash F;
    F.InitParams();
    F.InitKeys();
    F.InitCrypter();

    int m1 = rand()%256; int m2 = rand()%256;
    FntruCiphertextArray ct1, ct2;
    F.Encrypt(ct1, m1);
    F.Encrypt(ct2, m2);
    FntruCiphertext ct;
    F.LT(ct, ct1, ct2);
    Plaintext pt;
    F.Decrypt(pt, ct);
    cout << m1 << " < " << m2 << " : " << pt << endl;
}

void TestSort()
{
    srand (time(NULL));
    int input_set_size = 10;

    Flash F;
    F.InitParams();
    F.InitKeys();
    F.InitCrypter();

    vector<int> m;
    vector<FntruCiphertextArray> ct;
    cout << "Input list : ";
    for(int i=0; i<10; i++)
    {
        m.push_back(rand()%256);
        FntruCiphertextArray temp;
        F.Encrypt(temp, m.back());
        ct.push_back(temp);
        cout << m.back() << " ";
    }
    cout << endl;

    // Compare the first number with every other number
    vector<FntruCiphertext> lt;
    vector<FntruCiphertext> lt_not;
    for(int i=1; i<m.size(); i++)
    {
        FntruCiphertext temp;
        F.LT(temp, ct[0], ct[i]);
        lt.push_back(temp);
        F.NOT(temp, temp);
        lt_not.push_back(temp);
    }

    for(int i=0; i<lt.size(); i++)
    {
        F.SXL(lt_not[i], lt_not[i]);
        F.XOR(lt_not[i], lt_not[i], lt[i]);
    }

    FntruCiphertext result = lt_not[0];
    for(int i=1; i<lt.size(); i++)
    {
        F.AND(result, result, lt_not[i]);
    }

    Plaintext pt;
    F.Decrypt(pt, result);
    cout << "The rank of element " << m[0] << " : " << deg(pt) << endl;
}
