#pragma once

#include "DESSecretKey.h"
#include "BlockCipherAlgorithm.h"

class DES {
    public:
        static constexpr uint8_t BLOCK_SIZE               = 64;
        static constexpr uint8_t HALF_BLOCK_SIZE          = 32;
        static constexpr uint8_t HALF_BLOCK_EXTENDED_SIZE = 48;
        static constexpr uint8_t SBOX_BLOCK_SIZE          =  6;
        static constexpr uint8_t BLOCK_SIZE_BYTES         =  8;


        using Block = std::bitset<BLOCK_SIZE>;
        using HalfBlock = std::bitset<HALF_BLOCK_SIZE>;
        using HalfBlockExtended = std::bitset<HALF_BLOCK_EXTENDED_SIZE>;
        using SboxBlock = std::bitset<SBOX_BLOCK_SIZE>;

        using SecretKey = DESSecretKey;

    private:
        SecretKey key;

        HalfBlock feistel(const HalfBlock block, SecretKey::RoundKey roundKey) const;
        Block run(const Block plainBits, bool encrypt) const;

    public:

        void  setKey(const SecretKey &key);
        Block encrypt(const Block plainBits) const;
        Block decrypt(const Block plainBits) const;    
};

// Class DES must satisfy the concept (interface) BlockCipherAlgorithm
static_assert(BlockCipherAlgorithm<DES>);