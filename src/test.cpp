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
    for(int i=0; i<4; i++)
    {
        m.push_back(rand()%256);
        //m.push_back(i+1);
        FntruCiphertextArray temp;
        F.Encrypt(temp, m.back());
        ct.push_back(temp);
        cout << m.back() << " ";
    }
    cout << endl;

    // Compare a number with every other number
    FntruMatrix M;
    for(int i=0; i<m.size(); i++)
    {
        FntruVector row(m.size());
        for(int j=0; j<i; j++)
        {
            F.NOT(row[j], M[j][i]);
        }
        for(int j=i+1; j<m.size(); j++)
        {
            F.LT(row[j], ct[i], ct[j]);
        }
        M.push_back(row);
    }

    FntruMatrix RankPoly(m.size());
    for(int i=0; i<m.size(); i++)
    {
        for(int j=0; j<m.size(); j++)
        {
            if (i != j)
            {
                FntruCiphertext temp;
                F.SXL(temp, M[j][i]);
                F.XOR(temp, temp, M[i][j]);
                RankPoly[i].push_back(temp);
            }
        }
    }

    FntruVector R(m.size());
    for(int i=0; i<m.size(); i++)
    {
        R[i] = RankPoly[i][0];
        for(int j=1; j<RankPoly[i].size(); j++)
        {
            F.AND(R[i], R[i], RankPoly[i][j]);
        }
        for(int j=0; j<ct[i].size(); j++)
        {
            F.AND(ct[i][j], ct[i][j], R[i]);
        }
    }

    FntruCiphertextArray Y;
    for(int j=0; j<ct[0].size(); j++)
    {
        Y.push_back(ct[0][j]);
    }
    for(int i=1; i<ct.size(); i++)
    {
        for(int j=0; j<ct[i].size(); j++)
        {
            F.XOR(Y[j], Y[j], ct[i][j]);
        }
    }

    PlaintextArray pt;
    F.Decrypt(pt, Y);
    Plaintext output;
    PolyBlockDecomposeInverse(output, pt);
    cout << "Sorted list : " << output << endl;
}

void TestRank()
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
    for(int i=0; i<4; i++)
    {
        m.push_back(rand()%256);
        FntruCiphertextArray temp;
        F.Encrypt(temp, m.back());
        ct.push_back(temp);
        cout << m.back() << " ";
    }
    cout << endl;

    // Compare a number with every other number
    FntruMatrix M;
    for(int i=0; i<m.size(); i++)
    {
        FntruVector row(m.size());
        for(int j=0; j<i; j++)
        {
            F.NOT(row[j], M[j][i]);
        }
        for(int j=i+1; j<m.size(); j++)
        {
            F.LT(row[j], ct[i], ct[j]);
        }
        M.push_back(row);
    }

    FntruMatrix RankPoly(m.size());
    for(int i=0; i<m.size(); i++)
    {
        for(int j=0; j<m.size(); j++)
        {
            if (i != j)
            {
                FntruCiphertext temp;
                F.SXL(temp, M[j][i]);
                F.XOR(temp, temp, M[i][j]);
                RankPoly[i].push_back(temp);
            }
        }
    }

    FntruVector Y(m.size());
    for(int i=0; i<m.size(); i++)
    {
        Y[i] = RankPoly[i][0];
        for(int j=1; j<RankPoly[i].size(); j++)
        {
            F.AND(Y[i], Y[i], RankPoly[i][j]);
        }

        Plaintext pt;
        F.Decrypt(pt, Y[i]);
        cout << "The rank of element " << m[i] << " : " << deg(pt) << endl;
    }
}
