#include "decrypter.h"

Decrypter::Decrypter()
{
    //ctor
}

Decrypter::Decrypter(const PtextMod &ptext_mod, const Ring &ctext_ring, const SecretKey &secret_key)
{
    ptext_mod_ = ptext_mod;

    ctext_ring_.coeff_mod = ctext_ring.coeff_mod;
    ctext_ring_.poly_mod = ctext_ring.poly_mod;

    secret_key_ = secret_key;
}

void Decrypter::Decrypt(Plaintext &ptext, const Ciphertext &ctext) const
{
    /* Decryption method from the paper:
    ** m = cf mod p
    */
    PolyMultPoly(ptext, ctext, secret_key_, ctext_ring_);
    PolyBalanceCoeff(ptext, ptext, ctext_ring_.coeff_mod);
    PolyReduceCoeff(ptext, ptext, ptext_mod_);
}

void Decrypter::Decrypt(Plaintext &ptext, const CiphertextArray &ctext) const
{
    // Decrypt the first ciphertext in the vector
    Decrypt(ptext, ctext[0]);
}

void Decrypter::Decrypt(Plaintext &ptext, const Ciphertext &ctext, const SecretKey &sk) const
{
    /* Decryption method from the paper:
    ** m = cf mod p
    */
    PolyMultPoly(ptext, ctext, sk, ctext_ring_);
    PolyBalanceCoeff(ptext, ptext, ctext_ring_.coeff_mod);
    PolyReduceCoeff(ptext, ptext, ptext_mod_);
}

void Decrypter::Decrypt(Plaintext &ptext, const CiphertextArray &ctext, const SecretKey &sk) const
{
    // Decrypt the first ciphertext in the vector
    Decrypt(ptext, ctext[0], sk);
}

Decrypter::~Decrypter()
{
    //dtor
}
