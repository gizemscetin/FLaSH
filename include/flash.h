#ifndef FLASH_H
#define FLASH_H

#include "paramgen.h"
#include "keygen.h"
#include "encrypter.h"
#include "decrypter.h"

using namespace NTL;

class Flash
{
    public:
        Flash();
        virtual ~Flash();

        void InitParams();
        void InitKeys();
        void InitCrypter();

        //void GetKeys();
        //void PrintKeys();
        //void TestKeys();
        //void SetKeys();

        const Encrypter* encrypter() const { return encrypter_;};
        const Decrypter* decrypter() const { return decrypter_;};

    protected:

    private:
        Encrypter* encrypter_ = nullptr;
        Decrypter* decrypter_ = nullptr;
        KeyGen* key_generator_ = nullptr;
        ParamGen* param_generator_ = nullptr;

        KeyPair keys_;
};

#endif // FLASH_H
