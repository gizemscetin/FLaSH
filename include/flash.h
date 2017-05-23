#ifndef FLASH_H
#define FLASH_H

#include "paramgen.h"
#include "keygen.h"
#include "encrypter.h"
#include "decrypter.h"
#include "arith.h"
#include "batcher.h"
#include <vector>
#include <time.h>

using namespace NTL;

typedef CiphertextArray FntruCiphertext;
typedef vector<FntruCiphertext> FntruCiphertextArray;

class Flash
{
    public:
        Flash(FheType fhe = flattened) { fhe_type_ = fhe; };
        virtual ~Flash();

        void InitParams(bool BatchingOn = false, ParamType type = Test, PtextMod p = to_ZZ(2), NoiseBound B = to_ZZ(1), CircuitDepth d = 0);
        void InitKeys();
        void InitCrypter();
        void Evaluator();

        //void GetKeys();
        //void PrintKeys();
        //void TestKeys();
        //void SetKeys();

        void Batch(Plaintext &out, const PlaintextArray &in);
        void Unbatch(PlaintextArray &out, const Plaintext &in, int ptext_count);

        void Encrypt(CiphertextArray &ctext, int byte_message);
        void Encrypt(Ciphertext &ctext, const Plaintext &ptext);
        void Encrypt(FntruCiphertextArray &ctext, int byte_message);
        void Encrypt(FntruCiphertext &ctext, const Plaintext &ptext);
        void Decrypt(Plaintext &ptext, const Ciphertext &ctext);
        void Decrypt(PlaintextArray &ptext, const CiphertextArray &ctext);
        void Decrypt(Plaintext &ptext, const FntruCiphertext &ctext);
        void Decrypt(PlaintextArray &ptext, const FntruCiphertextArray &ctext);

        void AND(FntruCiphertext &out, const FntruCiphertext &in1, const FntruCiphertext &in2);
        void AND(Ciphertext &out, const Ciphertext &in1, const FntruCiphertext &in2);

        void XOR(FntruCiphertext &out, const FntruCiphertext &in1, const FntruCiphertext &in2);
        void XOR(Ciphertext &out, const Ciphertext &in1, const FntruCiphertext &in2);

        void NOT(FntruCiphertext &out, const FntruCiphertext &in1);
        void NOT(Ciphertext &out, const Ciphertext &in1);

        void EQ(Ciphertext &out, const Ciphertext &in1, const FntruCiphertext &in2);
        void EQ(FntruCiphertext &out, const FntruCiphertext &in1, const FntruCiphertext &in2);
        void EQ(FntruCiphertext &out, const FntruCiphertextArray &in1, const FntruCiphertextArray &in2);

        void LT(Ciphertext &out, const Ciphertext &in1, const FntruCiphertext &in2);
        void LT(FntruCiphertext &out, const FntruCiphertext &in1, const FntruCiphertext &in2);
        void LT(FntruCiphertext &out, const FntruCiphertextArray &in1, const FntruCiphertextArray &in2);

        void SXL(FntruCiphertext &out, const FntruCiphertext &in, int shift_amount=1);

        const Encrypter* encrypter() const { return encrypter_;};
        const Decrypter* decrypter() const { return decrypter_;};
        const ParamGen* param_generator() const { return param_generator_;};
        const KeyGen* key_generator() const { return key_generator_;};
        const Batcher* batcher() const { return batcher_; };


        int slot_count() { return batcher_->factor_count(); };

    protected:

    private:
        Encrypter* encrypter_ = nullptr;
        Decrypter* decrypter_ = nullptr;
        KeyGen* key_generator_ = nullptr;
        ParamGen* param_generator_ = nullptr;
        Batcher* batcher_ = nullptr;

        KeyPair keys_;
        FheType fhe_type_;  // Will be used in error distribution, key gen and encryption/decryption
};

#endif // FLASH_H
