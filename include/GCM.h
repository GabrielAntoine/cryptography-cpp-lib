#pragma once

#include "BlockCipherMode.h"
#include "AES.h"
#include "CTR.h"

template <size_t keySize>
    requires AESKeySize<keySize>
class GCM : public CTR<AES<keySize>> {
public:
    using Algorithm = CTR<AES<keySize>>::Algorithm;
    using IV = CTR<AES<keySize>>::IV;
    
    static constexpr uint8_t BLOCK_SIZE = toByteCount(Algorithm::BLOCK_SIZE);
    static constexpr uint8_t PREFERRED_IV_SIZE = 12;

    using Tag         = ByteArray<BLOCK_SIZE>;
    using TagSpan     = ByteSpan<BLOCK_SIZE>;
    using HalfTag     = ByteArray<BLOCK_SIZE / 2>;
    using HalfTagSpan = ByteSpan<BLOCK_SIZE / 2>;

private:
    ByteArray<> rawIv;
    IV j0;
    ByteArray<> aad;
    
public:
    void setAlgorithm(const Algorithm &algorithm);
    void setIV(ByteSpan<> iv);
    void setAAD(ByteSpan<> aad);
    ByteArray<> encrypt(ByteSpan<> bytes) const;
    ByteArray<> decrypt(ByteSpan<> bytes) const;

private:
    Tag getHashSubkey() const;
    IV  getEncryptedJ0() const;
    Tag ghash(ByteSpan<> bytes, Tag initialTag = Tag{std::byte(0)}) const;
    Tag createTag(ByteSpan<> ciphertext) const;

    static_assert(BlockCipherMode<GCM<keySize>>);
};

class TagMismatchException : public std::exception {
    const char* what() const noexcept override {
        return "Tag mismatch";
    }
};

#include "GCM.tpp"
