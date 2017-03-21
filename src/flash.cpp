#include "flash.h"

Flash::~Flash()
{
    if(encrypter_ != nullptr) delete encrypter_;
    if(decrypter_ != nullptr) delete decrypter_;
    if(param_generator_ != nullptr) delete param_generator_;
    if(key_generator_ != nullptr) delete key_generator_;
}


/*
** Default values:
** type = Test,
** fhe = flattened
** p = to_ZZ(2),
** B = to_ZZ(1),
** d = 0
*/
void Flash::InitParams(ParamType type, PtextMod p, NoiseBound B, CircuitDepth d)
{
    if(param_generator_ != nullptr)
        delete param_generator_;
    param_generator_ = new ParamGen(fhe_type_, type, p, B, d);
}

void Flash::InitKeys()
{
    // KeyGen
    if(param_generator_ == nullptr)
    {
        cout << "Parameters missing: try calling InitParams first." << endl;
        return;
    }
    key_generator_ = new KeyGen(param_generator_->key_ring(), param_generator_->noise_ring(), param_generator_->ptext_ring());
    key_generator_->GenerateNewKeys(keys_);

    InitCrypter();
}

void Flash::InitCrypter()
{
    if(param_generator_ == nullptr)
    {
        cout << "Parameters missing: try calling InitParams first." << endl;
        return;
    }
    if(key_generator_ == nullptr)
    {
        cout << "Keys missing: try calling InitKeys first." << endl;
        return;
    }
    // Encryptor
        encrypter_= new Encrypter(param_generator_->ptext_ring().coeff_mod, param_generator_->ctext_ring(),
                                  param_generator_->noise_ring() , keys_.pub_key);

    // Decryptor
        decrypter_ = new Decrypter(param_generator_->ptext_ring().coeff_mod, param_generator_->ctext_ring(), keys_.sec_key);
}

void Flash::Evaluator()
{
    if(param_generator_ == nullptr)
    {
        cout << "Parameters missing: try calling InitParams first." << endl;
        return;
    }
    if(key_generator_ == nullptr)
    {
        cout << "Keys missing: try calling InitKeys first." << endl;
        return;
    }

}

void Flash::Encrypt(CiphertextArray &ctext, int byte_message)
{
    PlaintextArray ptext;
    PolyBlockDecompose(ptext, to_ZZX(byte_message), 8);
    encrypter_->Encrypt(ctext, ptext);
}

void Flash::Encrypt(Ciphertext &ctext, const Plaintext &ptext)
{
    encrypter_->Encrypt(ctext, ptext);
}
void Flash::Encrypt(FntruCiphertext &ctext, const Plaintext &ptext)
{
    encrypter_->Encrypt(ctext, ptext, param_generator_->block_count());
}
void Flash::Decrypt(Plaintext &ptext, const Ciphertext &ctext)
{
    decrypter_->Decrypt(ptext, ctext);
}
void Flash::Decrypt(PlaintextArray &ptext, const CiphertextArray &ctext)
{
    ptext.SetLength(ctext.length());
    for(int i=0; i<ptext.length(); i++)
    {
        decrypter_->Decrypt(ptext[i], ctext[i]);
    }
}
void Flash::Decrypt(Plaintext &ptext, const FntruCiphertext &ctext)
{
    decrypter_->Decrypt(ptext, ctext);
}


void Flash::AND(FntruCiphertext &out, const FntruCiphertext &in1, const FntruCiphertext &in2)
{
    out.SetLength(param_generator_->block_count());
    for(int i=0; i<param_generator_->block_count(); i++)
    {
        CiphertextArray in1_blocks;
        PolyBlockDecompose(in1_blocks, in1[i], param_generator_->block_count(), param_generator_->block_size());
        PolyVectorDotProduct(out[i], in1_blocks, in2, param_generator_->ctext_ring());
    }
}

void Flash::AND(Ciphertext &out, const Ciphertext &in1, const FntruCiphertext &in2)
{
    CiphertextArray in1_blocks;
    PolyBlockDecompose(in1_blocks, in1, param_generator_->block_count(), param_generator_->block_size());
    PolyVectorDotProduct(out, in1_blocks, in2, param_generator_->ctext_ring());
}

void Flash::XOR(FntruCiphertext &out, const FntruCiphertext &in1, const FntruCiphertext &in2)
{
    out.SetLength(param_generator_->block_count());
    for(int i=0; i<param_generator_->block_count(); i++)
    {
        PolyAddPoly(out[i], in1[i], in2[i], param_generator_->ctext_ring());
    }
}

void Flash::XOR(Ciphertext &out, const Ciphertext &in1, const FntruCiphertext &in2)
{
    PolyAddPoly(out, in1, in2[0], param_generator_->ctext_ring());
}

void Flash::NOT(FntruCiphertext &out, const FntruCiphertext &in)
{
    out.SetLength(param_generator_->block_count());
    ZZ scalar(1);
    for(int i=0; i<param_generator_->block_count(); i++)
    {
        PolyAddScalar(out[i], (-1*in[i]), scalar, param_generator_->ctext_ring());
        scalar *= param_generator_->block_size();
    }
}

void Flash::NOT(Ciphertext &out, const Ciphertext &in)
{
    PolyAddScalar(out, (-1*in), 1, param_generator_->ctext_ring());
}


void Flash::EQ(Ciphertext &out, const Ciphertext &in1, const Ciphertext &in2)
{
    XOR(out, in1, in2);
    NOT(out, out);
}

void Flash::EQ(Ciphertext &out, const FntruCiphertext &in1, const FntruCiphertext &in2)
{
    XOR(out, in1, in2);
    NOT(out, out);
}


