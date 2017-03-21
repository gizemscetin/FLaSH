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

        void Encrypt(CiphertextArray &ctext, int byte_message);
        void Encrypt(Ciphertext &ctext, const Plaintext &ptext);
        void Encrypt(FntruCiphertext &ctext, const Plaintext &ptext);
        void Decrypt(Plaintext &ptext, const Ciphertext &ctext);
        void Decrypt(PlaintextArray &ptext, const CiphertextArray &ctext);
        void Decrypt(Plaintext &ptext, const FntruCiphertext &ctext);

        void AND(FntruCiphertext &out, const FntruCiphertext &in1, const FntruCiphertext &in2);
        void AND(Ciphertext &out, const Ciphertext &in1, const FntruCiphertext &in2);

        void XOR(FntruCiphertext &out, const FntruCiphertext &in1, const FntruCiphertext &in2);
        void XOR(Ciphertext &out, const Ciphertext &in1, const FntruCiphertext &in2);

        void NOT(FntruCiphertext &out, const FntruCiphertext &in1);
        void NOT(Ciphertext &out, const Ciphertext &in1);

        void EQ(Ciphertext &out, const Ciphertext &in1, const Ciphertext &in2);
        void EQ(Ciphertext &out, const FntruCiphertext &in1, const FntruCiphertext &in2);


        const Encrypter* encrypter() const { return encrypter_;};
        const Decrypter* decrypter() const { return decrypter_;};
        const ParamGen* param_generator() const { return param_generator_;};

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
