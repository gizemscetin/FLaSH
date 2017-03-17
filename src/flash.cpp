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
** Default values:
** type = Test,
** fhe = flattened
** p = to_ZZ(2),
** B = to_ZZ(1),
** d = 0
*/
void Flash::InitParams(ParamType type, FheType fhe, PtextMod p, NoiseBound B, CircuitDepth d)
{
    if(param_generator_ != nullptr)
        delete param_generator_;
    param_generator_ = new ParamGen(fhe, type, p, B, d);
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
