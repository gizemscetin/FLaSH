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
** p = to_ZZ(2),
** B = to_ZZ(1),
** d = 0
*/
void Flash::InitParams(ParamType type, PtextMod p, NoiseBound B, CircuitDepth d)
{
    if(type == Test)
    {
        PolyDegree n(27);               // small degree for testing
        CtextMod q(GenPrime_ZZ(8));     // small coefficients for faster arithmetic
        InitParams(B, p, q, n, MonomialPlusOne);
        // Check if the coeff size is big enough to handle
        // the final noise wrt given circuit depth.
        param_generator_->FindSmallestCoeffMod(d);
    }
    else if(type == Secure)
    {
        PolyDegree n(2*GenGermainPrime_long(9)+1);     // Safe Prime, due to Subfield Attack
        CtextMod q(GenPrime_ZZ(8));
        InitParams(B, p, q, n, MonomialPlusOne);

        // Check if the coeff size is big enough to handle
        // the final noise wrt given circuit depth.
        param_generator_->FindSmallestCoeffMod(d);

        // Check if it is secure with the new q
        //param_generator_.CheckSecurity();
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

