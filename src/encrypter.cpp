#include "encrypter.h"

Encrypter::Encrypter()
{
    //ctor
}

Encrypter::Encrypter(const PtextMod &ptext_mod, const Ring &ctext_ring, const Ring &noise_ring, const PublicKey &public_key)
{
    ptext_mod_ = ptext_mod;

    ctext_ring_.coeff_mod = ctext_ring.coeff_mod;
    ctext_ring_.poly_mod = ctext_ring.poly_mod;

    noise_ring_.coeff_mod = noise_ring.coeff_mod;
    noise_ring_.poly_mod = noise_ring.poly_mod;

    public_key_ = public_key;
}

Encrypter::~Encrypter()
{
    //dtor
}

void Encrypter::Encrypt(Ciphertext &ctext, const Plaintext &ptext) const
{
    /* Encryption method from the paper:
    ** c = hs + pe + m
    */
    ZZX temp;
    PolySampleBalanced(temp, noise_ring_);                  // s
    PolyMultPoly(ctext, public_key_, temp, ctext_ring_);    //hs
    PolySampleBalanced(temp, noise_ring_);                  // e
    PolyMultScalar(temp, temp, ptext_mod_, ctext_ring_);    // pe
    PolyAddPoly(temp, ctext, temp, ctext_ring_);            // hs + pe
    PolyAddPoly(ctext, ctext, ptext, ctext_ring_);          // hs + pe + m
}
