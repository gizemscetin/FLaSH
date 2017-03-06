#include "flash.h"

Flash::Flash()
{

}

Flash::~Flash()
{
    if(encrypter_ != nullptr) delete encrypter_;
    if(decrypter_ != nullptr) delete decrypter_;
    if(param_generator_ != nullptr) delete param_generator_;
    if(key_generator_ != nullptr) delete key_generator_;
}

/*
void Flash::InitParams()
{
    // Default Parameters
    PtextMod p(2);
    PolyDegree n(1024);
    NoiseBound B(1);
    CtextMod q(GenPrime_ZZ(20));

    if(param_generator_ != nullptr) delete param_generator_;
    param_generator_ = new ParamGen(B, p, q, n, MonomialPlusOne);
}
*/

void Flash::InitParams(ParamType type)
{
    if(type == Toy)
    {
        PtextMod p(2);
        PolyDegree n(64);
        NoiseBound B(1);
        CtextMod q(GenPrime_ZZ(10));
        InitParams(B, p, q, n, MonomialPlusOne);
    }
    else if(type == Secure)
    {
        PtextMod p(2);
        PolyDegree n(2*GenGermainPrime_long(10)+1);     // Safe Prime, due to Subfield Attack
        NoiseBound B(1);
        CtextMod q(GenPrime_ZZ(30));

    }
}

void Flash::InitParams(NoiseBound B, PtextMod p, CtextMod q, PolyDegree n, PolyType t)
{
    if(param_generator_ != nullptr)
        delete param_generator_;
    param_generator_ = new ParamGen(B, p, q, n, t);
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

