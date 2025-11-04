#pragma once

#include "AESSecretKey.h"
#include "BlockCipherAlgorithm.h"

template <size_t keySize>
    requires AESKeySize<keySize>
class AES {
    public:
        static constexpr uint8_t BLOCK_SIZE        = 128;
        static constexpr uint8_t BLOCK_SIZE_BYTES  = toByteCount(BLOCK_SIZE);
        static constexpr uint8_t MATRIX_LENGTH     = 4;

        // AES uses std::bitset only to be compatible with the BlockCipherAlgorithm concept
        using Block     = std::bitset<BLOCK_SIZE>;
        using SecretKey = AESSecretKey<keySize>;
        using Matrix    = ByteArray<BLOCK_SIZE_BYTES>; // Not a array<array> for compatibility

    private:
        SecretKey key;

        std::byte& matrixAt(Matrix& matrix, const int col, const int row) const;

        void addRoundKey(Matrix &matrix, const int round) const;
        
        template <AES_TABLES::SBOX_TYPE sbox>
        void substituteBytes(Matrix &matrix) const;

        void shiftRows(Matrix &matrix) const;
        void invShiftRows(Matrix &matrix) const;

        template <AES_TABLES::MIX_COLUMNS_MATRIX_TYPE mixColumnsMatrix>
        void mixColumns(Matrix &matrix) const;

    public:
        void  setKey(const SecretKey &key);
        Block encrypt(const Block &plaintext) const;
        Block decrypt(const Block &ciphertext) const; 

    static_assert(BlockCipherAlgorithm<AES<keySize>>);
};


#include "AES.tpp"
