#ifndef DECRYPTER_H
#define DECRYPTER_H

#include "keygen.h"
#include "paramgen.h"
#include "arith.h"

using namespace NTL;

typedef ZZX Plaintext;
typedef ZZX Ciphertext;
typedef vec_ZZX PlaintextArray;
typedef vec_ZZX CiphertextArray;

class Decrypter
{
    public:
        Decrypter();
        virtual ~Decrypter();
        Decrypter(const PtextMod &ptext_mod, const Ring &ctext_ring, const SecretKey &secret_key);

        void Decrypt(Plaintext &ptext, const Ciphertext &ctext) const;
        void Decrypt(Plaintext &ptext, const CiphertextArray &ctext) const;

    protected:

    private:
        PtextMod ptext_mod_;
        Ring ctext_ring_;
        SecretKey secret_key_;
};

#endif // DECRYPTER_H
