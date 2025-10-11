#pragma once

#include "DESedeSecretKey.h"
#include "DES.h"

class DESede {
    public:
        static constexpr uint8_t BLOCK_SIZE = DES::BLOCK_SIZE;

    private:
        DESedeSecretKey key;

        DES::Block run(const DES::Block plainBits, bool encrypt) const;

    public:
        void       setKey(const DESedeSecretKey &key);
        DES::Block encrypt(const DES::Block plainBits) const;
        DES::Block decrypt(const DES::Block plainBits) const;    
};