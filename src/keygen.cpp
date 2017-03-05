#include "keygen.h"

KeyGen::KeyGen()
{
    //ctor
}


KeyGen::KeyGen(const Ring &key_ring, const Ring &noise_ring, const PtextMod &ptext_mod)
{
    key_ring_.coeff_mod = key_ring.coeff_mod;
    key_ring_.poly_mod = key_ring.poly_mod;

    noise_ring_.coeff_mod = noise_ring.coeff_mod;
    noise_ring_.poly_mod = noise_ring.poly_mod;

    ptext_mod_ = ptext_mod;
}

KeyGen::KeyGen(const Ring &key_ring, const Ring &noise_ring, const Ring &ptext_ring)
{
    key_ring_.coeff_mod = key_ring.coeff_mod;
    key_ring_.poly_mod = key_ring.poly_mod;

    noise_ring_.coeff_mod = noise_ring.coeff_mod;
    noise_ring_.poly_mod = noise_ring.poly_mod;

    ptext_mod_ = ptext_ring.coeff_mod;
}

void KeyGen::GenerateNewSecretKey(SecretKey &key)
{
    /* Secret Key generation from the paper:
    ** f = p * f' + 1
    */
    PolySampleBalanced(key, noise_ring_);
    PolyMultScalar(key, key, ptext_mod_, key_ring_);
    PolyAddScalar(key, key, 1, key_ring_);
}

void KeyGen::GenerateNewPublicKey(PublicKey &key, const SecretKey &sec_key_inv)
{
    /* Public Key generation from the paper:
    ** h = p * g * f_inv
    */
    PolySampleBalanced(key, noise_ring_);
    PolyMultPoly(key, key, sec_key_inv, key_ring_);
    PolyMultScalar(key, key, ptext_mod_, key_ring_);
}

void KeyGen::GenerateNewKeys(KeyPair &key)
{
    GenerateNewSecretKey(key.sec_key);

    // Check if the secret key is invertible
    SecretKey sec_key_inv;
    while(!PolyInverse(sec_key_inv, key.sec_key, key_ring_))
    {
        // Generate new secret key and try inverting
        GenerateNewSecretKey(key.sec_key);
        PolyInverse(sec_key_inv, key.sec_key, key_ring_);
    }
    GenerateNewPublicKey(key.pub_key, sec_key_inv);
}

void KeyGen::TestKeys(const KeyPair &key)
{
    cout    << "SK : " << key.sec_key << endl
            << "PK : " << key.pub_key << endl;

    ZZX temp;
    PolyMultPoly(temp, key.sec_key, key.pub_key, key_ring_);
    PolyBalanceCoeff(temp, temp, key_ring_.coeff_mod);
    PolyReduceCoeff(temp, temp, ptext_mod_);
    if (temp == 0)
        cout << "Key test: Passed." << endl;
    else
        cout << "Key test: Failed." << endl << temp << endl;
}

KeyGen::~KeyGen()
{
    //dtor
}
