#ifndef KEYGEN_H
#define KEYGEN_H


#include <NTL/ZZX.h>
#include "paramgen.h"
#include "arith.h"

using namespace NTL;

typedef ZZX PublicKey;
typedef ZZX SecretKey;


struct KeyPair
{
    PublicKey pub_key;      // h in the papers
    SecretKey sec_key;      // f in the papers
};


class KeyGen
{
    public:
        KeyGen();
        virtual ~KeyGen();
        KeyGen(const Ring &key_ring, const Ring &noise_ring, const PtextMod &ptext_mod);
        KeyGen(const Ring &key_ring, const Ring &noise_ring, const Ring &ptext_ring);

        void GenerateNewSecretKey(SecretKey &key);
        void GenerateNewPublicKey(PublicKey &key, const SecretKey &sec_key_inv);
        void GenerateNewKeys(KeyPair &key);
        void TestKeys(const KeyPair &key);

    protected:

    private:
        Ring key_ring_, noise_ring_;    // q, B in the papers
        PtextMod ptext_mod_;            // p in the papers

        void TestSecretKeyInv(const SecretKey &sec_key_inv);
};

#endif // KEYGEN_H
