#pragma once

#include "DESedeSecretKey.h"
#include "DES.h"
#include "BlockCipherAlgorithm.h"

class DESede {
    public:
        static constexpr uint8_t BLOCK_SIZE = DES::BLOCK_SIZE;
        static constexpr uint8_t BLOCK_SIZE_BYTES = DES::BLOCK_SIZE_BYTES;

        using Block = DES::Block;
        using SecretKey = DESedeSecretKey;

    private:
        DESedeSecretKey key;

        Block run(const Block plainBits, bool encrypt) const;

    public:
        void  setKey(const SecretKey &key);
        Block encrypt(const Block plainBits) const;
        Block decrypt(const Block plainBits) const;    
};

static_assert(BlockCipherAlgorithm<DESede>);