#ifndef ENCRYPTER_H
#define ENCRYPTER_H

#include "keygen.h"
#include "paramgen.h"
#include "arith.h"
#include <vector>

using namespace NTL;

typedef ZZX Plaintext;
typedef ZZX Ciphertext;
typedef vector<Plaintext> PlaintextArray;
typedef vector<Ciphertext> CiphertextArray;

class Encrypter
{
    public:
        Encrypter();
        virtual ~Encrypter();
        Encrypter(const PtextMod &ptext_mod, const Ring &ctext_ring, const Ring &noise_ring, const PublicKey &public_key);

        void Encrypt(Ciphertext &ctext, const Plaintext &ptext) const;

    protected:

    private:
        PtextMod ptext_mod_;
        Ring ctext_ring_;
        Ring noise_ring_;
        PublicKey public_key_;
};

#endif // ENCRYPTER_H