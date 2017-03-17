#ifndef FLASH_H
#define FLASH_H

#include "paramgen.h"
#include "keygen.h"
#include "encrypter.h"
#include "decrypter.h"

using namespace NTL;

typedef CiphertextArray FntruCiphertext;

class Flash
{
    public:
        Flash(FheType fhe = flattened) { fhe_type_ = fhe; };
        virtual ~Flash();

        void InitParams(ParamType type = Test, PtextMod p = to_ZZ(2), NoiseBound B = to_ZZ(1), CircuitDepth d = 0);
        void InitKeys();
        void InitCrypter();
        void Evaluator();

        //void GetKeys();
        //void PrintKeys();
        //void TestKeys();
        //void SetKeys();

        void Encrypt(Ciphertext &ctext, const Plaintext &ptext);
        void Encrypt(FntruCiphertext &ctext, const Plaintext &ptext, int block_count = 1);
        void Decrypt(Plaintext &ptext, const Ciphertext &ctext);
        void Decrypt(Plaintext &ptext, const FntruCiphertext &ctext);

        //const Encrypter* encrypter() const { return encrypter_;};
        //const Decrypter* decrypter() const { return decrypter_;};

    protected:

    private:
        Encrypter* encrypter_ = nullptr;
        Decrypter* decrypter_ = nullptr;
        KeyGen* key_generator_ = nullptr;
        ParamGen* param_generator_ = nullptr;

        KeyPair keys_;
        FheType fhe_type_;  // Will be used in error distribution, key gen and encryption/decryption
};

#endif // FLASH_H
